#include "ModbusApi.h"
#include "MsgPackage.h"
#include "Log.h"

void printErrorMsg(ERRORFLAG code)
{
	switch (code)
	{
	case ERRORFLAG_NONE:
	{
		std::cout << "no error" << std::endl;
	}
	break;
	case ERRORFLAG_CONNECT:
	{
		std::cout << "disconnect with server" << std::endl;
	}
	break;
	case ERRORFLAG_WRITE:
	{
		std::cout << "write msg error" << std::endl;
	}
	break;
	case ERRORFLAG_READ:
	{
		std::cout << "read msg error" << std::endl;
	}
	break;
	case ERRORFLAG_CHECKINFO:
	{
		std::cout << "recv msg connot pair of send msg" << std::endl;
	}
	break;
	case ERRORFLAG_RECV_TIMEOUT:
	{
		std::cout << "recv msg time out" << std::endl;
	}
	break;
	case ERRORFLAG_INIT_SOCKETFD:
	{
		std::cout << "socket fd init error" << std::endl;
	}
	break;
	default:
	{
		std::cout << "error code con`t find:" << code << std::endl;
	}
		break;
	}
}

const Modbus* createModbusContext(const char* ip, int port)
{
	return new Modbus(ip, port);
}

bool connect2Server(Modbus* ctx)
{
	if(ctx)
		return ctx->connect2Server();
	return false;
}

bool closeConnect(Modbus* ctx)
{
	if (ctx != nullptr)
		ctx->close();
	return true;
}

void freeContext(Modbus* ctx)
{
	if(ctx)
		delete ctx;
}

int readIOStatus(Modbus* context, int func, int addr, int count, int16_t* dst)
{
	if (context == nullptr)
	{
		return MODBUS_RET_ERROR;
	}

	uint8_t req[_MIN_REQ_LENGTH];
	int req_len = _modbus_tcp_build_request_basis(context->getSlaveID(), context->getTid(), func, addr, count, req);
	int msg_len = _modbus_tcp_send_msg_pre(req, req_len);
	int ret = context->sendMsgToMe(req, msg_len);

	if (ret > 0)
	{
		uint8_t rsp[_MIN_REQ_LENGTH];
		int len = context->recvMsg(rsp, func, Response);

		int rc = check_confirmation(_MODBUS_TCP_HEADER_LENGTH, req, rsp, len);

		int offset = _MODBUS_TCP_HEADER_LENGTH + 2;
		int offset_end = offset + rc;
		int temp, bit;
		int pos = 0;
		for (int i = offset; i < offset_end; i++)
		{
			temp = rsp[i];
			for (bit = 0x01; (bit & 0xff) && (pos < count);)
			{
				dst[pos++] = (temp & bit) ? TRUE : FALSE;
				bit = bit << 1;
			}

		}
	}
	else
	{
		std::cout << "send error" << std::endl;
		context->flush();
		context->setErrorFlag(ERRORFLAG_CONNECT);
		return MODBUS_RET_ERROR;
	}
	return MODBUS_RET_OK;
};

int readRegisters(Modbus* context, int func, int addr, int count, int16_t* dst)
{
	if (context == nullptr)
	{
		return MODBUS_RET_ERROR;
	}

	uint8_t req[_MIN_REQ_LENGTH];
	int req_len = _modbus_tcp_build_request_basis(context->getSlaveID(), context->getTid(), func, addr, count, req);
	int msg_len = _modbus_tcp_send_msg_pre(req, req_len);
	int ret = context->sendMsgToMe(req, msg_len);

	if (ret > 0)
	{
		uint8_t rsp[_MIN_REQ_LENGTH];
		int len = context->recvMsg(rsp, func, Response);

		int rc = check_confirmation(_MODBUS_TCP_HEADER_LENGTH, req, rsp, len);

		if (rc == -1)
		{
			std::cout << "check error" << std::endl;
			context->flush();
			context->setErrorFlag(ERRORFLAG_CHECKINFO);
			return MODBUS_RET_ERROR;
		}

		int  offset = _MODBUS_TCP_HEADER_LENGTH;

		for (int i = 0; i < rc; i++) {
			/* shift reg hi_byte to temp OR with lo_byte */
			dst[i] = (rsp[offset + 2 + (i << 1)] << 8) |
				rsp[offset + 3 + (i << 1)];
		}
	}
	else
	{
		std::cout << "send fail" << std::endl;
		context->flush();
		context->setErrorFlag(ERRORFLAG_CONNECT);
		return MODBUS_RET_ERROR;
	}
	return MODBUS_RET_OK;
};

int writeSingal(Modbus* context, int func, int addr, int value)
{
	if (context == nullptr)
	{
		return MODBUS_RET_ERROR;
	}

	uint8_t req[_MIN_REQ_LENGTH];
	int req_len = _modbus_tcp_build_request_basis(context->getSlaveID(), context->getTid(), func, addr, value, req);
	int msg_len = _modbus_tcp_send_msg_pre(req, req_len);
	int ret = context->sendMsgToMe(req, msg_len);

	if (ret > 0)
	{
		uint8_t rsp[_MIN_REQ_LENGTH];
		int len = context->recvMsg(rsp, func, Response);

		int rc = check_confirmation(_MODBUS_TCP_HEADER_LENGTH, req, rsp, len);

		if (rc == -1)
		{

			std::cout << "check error" << std::endl;
			context->flush();
			context->setErrorFlag(ERRORFLAG_CHECKINFO);
			return MODBUS_RET_ERROR;
		}
	}
	else
	{
		std::cout << "send failed" << std::endl;
		context->flush();
		context->setErrorFlag(ERRORFLAG_CONNECT);
		return MODBUS_RET_ERROR;
	}
	return MODBUS_RET_OK;
};

int readMulBit(Modbus* context, int addr, int count, int16_t* dst)
{
	return readIOStatus(context, MODBUS_FC_READ_COILS, addr, count, dst);
}

int readInputMulBit(Modbus* context, int addr, int count, int16_t* dst)
{
	return readIOStatus(context, MODBUS_FC_READ_DISCRETE_INPUTS, addr, count, dst);
}

int readHoldMulRegister(Modbus* context, int addr, int count, int16_t* dst)
{
	return readRegisters(context, MODBUS_FC_READ_HOLDING_REGISTERS, addr, count, dst);
}

int readInputMulRegister(Modbus* context, int addr, int count, int16_t* dst)
{
	return readRegisters(context, MODBUS_FC_READ_INPUT_REGISTERS, addr, count, dst);
}

int writeSigleBit(Modbus* context, int addr, int value)
{
	return writeSingal(context, MODBUS_FC_WRITE_SINGLE_COIL, addr, value ? 0xFF00 : 0);
}

int writeSigleRegister(Modbus* context, int addr, int value)
{
	return writeSingal(context, MODBUS_FC_WRITE_SINGLE_REGISTER, addr, value);
}

int writeMulBit(Modbus* context, int addr, int count, int16_t* src)
{
	if (context == nullptr)
	{
		return MODBUS_RET_ERROR;
	}

	if (count > MODBUS_MAX_WRITE_BITS)
	{
		return MODBUS_RET_ERROR;
	}
	uint8_t req[MAX_MESSAGE_LENGTH];
	int req_len = _modbus_tcp_build_request_basis(context->getSlaveID(), context->getTid(),
		MODBUS_FC_WRITE_MULTIPLE_COILS,
		addr, count, req);
	int byte_count = (count / 8) + ((count % 8) ? 1 : 0);
	req[req_len++] = byte_count;
	int bit_check = 0;
	int pos = 0;
	for (int i = 0; i < byte_count; i++)
	{
		int bit;
		bit = 0x01;
		req[req_len] = 0;
		while ((bit & 0xFF) && (bit_check++ < count))
		{
			if (src[pos++])
				req[req_len] |= bit;
			else
				req[req_len] &= ~bit;

			bit = bit << 1;
		}
		req_len++;
	}
	int msg_len = _modbus_tcp_send_msg_pre(req, req_len);
	int ret = context->sendMsgToMe(req, msg_len);
	if (ret < 0)
	{
		std::cout << "send error" << std::endl;
		context->flush();
		context->setErrorFlag(ERRORFLAG_CONNECT);
		return MODBUS_RET_ERROR;
	}
	else
	{
		uint8_t rsp[_MIN_REQ_LENGTH];
		int len = context->recvMsg(rsp, MODBUS_FC_WRITE_MULTIPLE_COILS, Response);

		check_confirmation(_MODBUS_TCP_HEADER_LENGTH, req, rsp, len);
	}
	return MODBUS_RET_OK;
}

int writeMulRegister(Modbus* context, int addr, int count, int16_t* src)
{
	if (context == NULL)
	{
		return MODBUS_RET_ERROR;
	}

	if (count > MODBUS_MAX_WRITE_REGISTERS)
	{
		return MODBUS_RET_ERROR;
	}
	uint8_t req[MAX_MESSAGE_LENGTH];
	int req_len = _modbus_tcp_build_request_basis(context->getSlaveID(), context->getTid(),
		MODBUS_FC_WRITE_MULTIPLE_REGISTERS,
		addr, count, req);
	int byte_count = count * 2;
	req[req_len++] = byte_count;

	for (int i = 0; i < count; i++) {
		req[req_len++] = src[i] >> 8;
		req[req_len++] = src[i] & 0x00FF;
	}
	int msg_len = _modbus_tcp_send_msg_pre(req, req_len);
	int ret = context->sendMsgToMe(req, msg_len);
	if (ret < 0)
	{
		std::cout << "send error" << std::endl;
		context->flush();
		context->setErrorFlag(ERRORFLAG_CONNECT);
		return MODBUS_RET_ERROR;
	}
	else
	{
		uint8_t rsp[_MIN_REQ_LENGTH];
		int len = context->recvMsg(rsp, MODBUS_FC_WRITE_MULTIPLE_COILS, Response);

		check_confirmation(_MODBUS_TCP_HEADER_LENGTH, req, rsp, len);
	}
	return MODBUS_RET_OK;
}
