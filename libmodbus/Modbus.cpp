#include "Modbus.h"
#include "MsgPackage.h"
#include "Log.h"

#if defined(_AIX) && !defined(MSG_DONTWAIT)
#define MSG_DONTWAIT MSG_NONBLOCK
#endif

static int _modbus_tcp_init_win32(void)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		fprintf(stderr, "WSAStartup() returned error code %d\n",
			(unsigned int)GetLastError());
		return -1;
	}
	return 0;
}

Modbus::Modbus(const char* ip, int port) :
	tid_(0), slaveID_(1), headerLength_(_MODBUS_TCP_HEADER_LENGTH), 
	ip_(ip), port_(port),
	socketFD_(0), isConnect_(false)
{
	addr_.sin_family = AF_INET;
	addr_.sin_port = htons(port);
	addr_.sin_addr.s_addr = inet_addr(ip);
	errorFlag_ = ERRORFLAG_NONE;
	requestMaxTime_.tv_sec = 0;
	requestMaxTime_.tv_usec = MODBUS_REQUEST_TIMEOUT;
	responseMaxTime_.tv_sec = 0;
	responseMaxTime_.tv_usec = MODBUS_RESPON_TIMEOUT;
	if (initSocketFD() == -1)
	{
		errorFlag_ = ERRORFLAG_INIT_SOCKETFD;
	}
}

Modbus::~Modbus()
{
	close();
}

int Modbus::initSocketFD()
{
	if (_modbus_tcp_init_win32() == -1)
	{
		return -1;
	}
	socketFD_ = socket(AF_INET, SOCK_STREAM, 0);
	BOOL bReuseaddr = TRUE;
	setsockopt(socketFD_, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReuseaddr, sizeof(BOOL));
	return 0;
}

int Modbus::connect2Server()
{
	int ret = connect(socketFD_, (struct sockaddr *)&addr_, sizeof(addr_));
	if (ret == -1)
	{
		std::cout << "connect failed" << std::endl;
		errorFlag_ = ERRORFLAG_CONNECT;
	}
	isConnect_ = true;
	return ret;
}

void Modbus::close()
{
	if(isConnect_)
		closesocket(socketFD_);
}

int Modbus::sendMsgToMe(uint8_t* msg, size_t len)
{
	int ret = send(socketFD_, (const char *)msg, len, 0);
	if (ret != len)
	{
		errorFlag_ = ERRORFLAG_WRITE;
	}
	return ret;
}

int Modbus::selectToTimeOut(fd_set *rset, struct timeval *tv)
{
	int s_rc;
	while ((s_rc = select(socketFD_, rset, NULL, NULL, tv)) == -1) 
	{
		if (errno == EINTR)
		{
			/* Necessary after an error */
			FD_ZERO(rset);
			FD_SET(socketFD_, rset);
		}
		else 
		{
			return -1;
		}
	}

	if (s_rc == 0) 
	{
		errorFlag_ = ERRORFLAG_RECV_TIMEOUT;
		//return -1;
	}
	return s_rc;
}

int Modbus::flush()
{
	int rc;
	int rc_sum = 0;
	do 
	{
		/* Extract the garbage from the socket */
		char devnull[MODBUS_TCP_MAX_ADU_LENGTH];
#ifndef OS_WIN32
		rc = recv(socketFD_, devnull, MODBUS_TCP_MAX_ADU_LENGTH, 50);
#else
		/* On Win32, it's a bit more complicated to not wait */
		fd_set rset;
		struct timeval tv;

		tv.tv_sec = 0;
		tv.tv_usec = 0;
		FD_ZERO(&rset);
		FD_SET(socketFD_, &rset);
		rc = select(socketFD_ + 1, &rset, NULL, NULL, &tv);
		if (rc == -1) 
		{
			errorFlag_ = ERRORFLAG_RECV_TIMEOUT;
			return -1;
		}

		if (rc == 1) 
		{
			/* There is data to flush */
			rc = recv(socketFD_, devnull, MODBUS_TCP_MAX_ADU_LENGTH, 0);
		}
#endif
		if (rc > 0) 
		{
			rc_sum += rc;
		}
	} while (rc == MODBUS_TCP_MAX_ADU_LENGTH);

	return rc_sum;
}

int Modbus::recvMsg(uint8_t* dst, int func, MsgDirection dire)
{
	fd_set rset;
	struct timeval tv;
	struct timeval *p_tv;
	int ret = 0;
	int nextreadlength = 0;
	int msg_length = 0;
	RecvMsgStep step;

	int rc = 0;

	FD_ZERO(&rset);
	FD_SET(socketFD_, &rset);

	tv.tv_sec = responseMaxTime_.tv_sec;
	tv.tv_usec = responseMaxTime_.tv_usec;
	p_tv = &tv;

	step = RecvMsgStep_Func;
	nextreadlength = headerLength_ + 1;
	while (nextreadlength > 0)
	{
		if ((ret = selectToTimeOut(&rset, p_tv)) == -1)
		{
			std::cout << "recv msg time out" << std::endl;
			errorFlag_ = ERRORFLAG_RECV_TIMEOUT;
			flush();
			return ret;
		}
		uint8_t msg[10000];
		rc = recv(socketFD_, (char*)dst + msg_length, 13, 0);
		//std::cout << "recv len:" << rc << std::endl;
		for (int i = 0; i < rc; i++)
			printf("<%.2X>", dst[msg_length + i]);
		msg_length += rc;
		nextreadlength -= rc;
		if (nextreadlength == 0)
		{
			if (step == RecvMsgStep_Func)
			{
				nextreadlength = compute_meta_length_after_function(dst[headerLength_], Response);
				if (nextreadlength != 0)
				{
					step = RecvMsgStep_Meta;
				}
			}
			else if (step == RecvMsgStep_Meta)
			{
				nextreadlength = compute_data_length_after_meta(headerLength_, dst, Response);
				if (nextreadlength > 0)
				{
					step = RecvMsgStep_Data;
				}
			}
		}
	}
	printf("\n");
	return msg_length;
}