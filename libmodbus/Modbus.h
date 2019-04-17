#ifndef MODBUS_H
#define MODBUS_H

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Common.h"

class Modbus
{
public:
	Modbus(const char* ip, int port);
	Modbus(Modbus&) = delete;
	Modbus &operator=(const Modbus&) = delete;
	~Modbus();
private:
	std::string ip_;
	uint32_t port_;
	uint8_t slaveID_;
	uint32_t socketFD_;
	uint16_t tid_;
	sockaddr_in addr_;
	ERRORFLAG errorFlag_;
	uint8_t headerLength_;
	timeval requestMaxTime_;
	timeval responseMaxTime_;
	bool isConnect_;
public:
	int flush();
	void close();
	int connect2Server();
	int sendMsgToMe(uint8_t* msg, size_t len);
	int recvMsg(uint8_t* msg, int func, MsgDirection dire);
	int selectToTimeOut(fd_set *rset, struct timeval *tv);
	int getSlaveID()
	{
		return slaveID_;
	}

	uint16_t& getTid()
	{
		return tid_;
	}

	void setErrorFlag(ERRORFLAG flag)
	{
		errorFlag_ = flag;
	}

	ERRORFLAG getErrorFlag()
	{
		return errorFlag_;
	}

	void setRequestTimeOut(uint32_t sec, uint32_t microSeconds)
	{
		requestMaxTime_.tv_sec = sec;
		requestMaxTime_.tv_usec = microSeconds;
	}

	void setResponseTimeOut(uint32_t sec, uint32_t microSeconds)
	{
		responseMaxTime_.tv_sec = sec;
		responseMaxTime_.tv_usec = microSeconds;
	}
private:
	int initSocketFD();
};

#endif // MODBUS_H

