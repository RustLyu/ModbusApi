#ifndef MSG_PACKAGE_H
#define MSG_PACKAGE_H

// 消息封装,拆解

#include <stdint.h>

static int _modbus_tcp_send_msg_pre(uint8_t *req, int req_length)
{
	/* Substract the header length to the message length */
	int mbap_length = req_length - 6;

	req[4] = mbap_length >> 8;
	req[5] = mbap_length & 0x00FF;

	return req_length;
}


static uint8_t compute_meta_length_after_function(int function,
	MsgDirection msg_type)
{
	int length;

	if (msg_type == Request) 
	{
		if (function <= MODBUS_FC_WRITE_SINGLE_REGISTER) 
		{
			length = 4;
		}
		else if (function == MODBUS_FC_WRITE_MULTIPLE_COILS ||
			function == MODBUS_FC_WRITE_MULTIPLE_REGISTERS) 
		{
			length = 5;
		}
		else if (function == MODBUS_FC_MASK_WRITE_REGISTER) 
		{
			length = 6;
		}
		else if (function == MODBUS_FC_WRITE_AND_READ_REGISTERS) 
		{
			length = 9;
		}
		else 
		{
			/* MODBUS_FC_READ_EXCEPTION_STATUS, MODBUS_FC_REPORT_SLAVE_ID */
			length = 0;
		}
	}
	else 
	{
		/* MSG_CONFIRMATION */
		switch (function) {
		case MODBUS_FC_WRITE_SINGLE_COIL:
		case MODBUS_FC_WRITE_SINGLE_REGISTER:
		case MODBUS_FC_WRITE_MULTIPLE_COILS:
		case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
			length = 4;
			break;
		case MODBUS_FC_MASK_WRITE_REGISTER:
			length = 6;
			break;
		default:
			length = 1;
		}
	}

	return length;
}

static int compute_data_length_after_meta(int headLength, uint8_t *msg, MsgDirection msg_type)
{
	int function = msg[headLength];
	int length;

	if (msg_type == Request)
	{
		switch (function)
		{
		case MODBUS_FC_WRITE_MULTIPLE_COILS:
		case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
			length = msg[headLength + 5];
			break;
		case MODBUS_FC_WRITE_AND_READ_REGISTERS:
			length = msg[headLength + 9];
			break;
		default:
			length = 0;
		}
	}
	else
	{
		if (function <= MODBUS_FC_READ_INPUT_REGISTERS || function == MODBUS_FC_REPORT_SLAVE_ID || function == MODBUS_FC_WRITE_AND_READ_REGISTERS)
		{
			length = msg[headLength + 1];
		}
		else
		{
			length = 0;
		}
	}
	return length;
}

/* Computes the length of the expected response */
static unsigned int compute_response_length_from_request(int headlength, uint8_t *req)
{
	int length;
	const int offset = headlength;

	switch (req[offset]) {
	case MODBUS_FC_READ_COILS:
	case MODBUS_FC_READ_DISCRETE_INPUTS: {
		/* Header + nb values (code from write_bits) */
		int nb = (req[offset + 3] << 8) | req[offset + 4];
		length = 2 + (nb / 8) + ((nb % 8) ? 1 : 0);
	}
										 break;
	case MODBUS_FC_WRITE_AND_READ_REGISTERS:
	case MODBUS_FC_READ_HOLDING_REGISTERS:
	case MODBUS_FC_READ_INPUT_REGISTERS:
		/* Header + 2 * nb values */
		length = 2 + 2 * (req[offset + 3] << 8 | req[offset + 4]);
		break;
	case MODBUS_FC_READ_EXCEPTION_STATUS:
		length = 3;
		break;
	case MODBUS_FC_REPORT_SLAVE_ID:
		/* The response is device specific (the header provides the
		   length) */
		return -1;
	case MODBUS_FC_MASK_WRITE_REGISTER:
		length = 7;
		break;
	default:
		length = 5;
	}

	return offset + length;
}

static int _modbus_tcp_pre_check_confirmation(const uint8_t *req,
                                              const uint8_t *rsp)
{
    /* Check transaction ID */
    if (req[0] != rsp[0] || req[1] != rsp[1])
	{
        return -1;
    }

    /* Check protocol ID */
    if (rsp[2] != 0x0 && rsp[3] != 0x0) 
	{
        return -1;
    }

    return 0;
}

static int check_confirmation(Modbus* ctx, int headLength, uint8_t *req, uint8_t *rsp, int rsp_length)
{
	int rc;
	int rsp_length_computed;
	const int offset = headLength;
	const int function = rsp[offset];

	if (_modbus_tcp_pre_check_confirmation(req, rsp))
	{
		struct timeval tv;
		struct timeval *p_tv;
		tv.tv_sec = ctx->getResponseMaxTime().tv_sec;
		tv.tv_usec = ctx->getResponseMaxTime().tv_usec;
		p_tv = &tv;
		ctx->_sleep_response_timeout(p_tv);
		ctx->flush();
		return -1;
	}

	rsp_length_computed = compute_response_length_from_request(headLength, req);

	/* Exception code */
	if (function >= 0x80) 
	{
		return -1;
	}

	/* Check length */
	if ((rsp_length == rsp_length_computed ||
		rsp_length_computed == -1) &&
		function < 0x80) {
		int req_nb_value;
		int rsp_nb_value;

		/* Check function code */
		if (function != req[offset]) 
		{
			return -1;
		}

		/* Check the number of values is corresponding to the request */
		switch (function) {
		case MODBUS_FC_READ_COILS:
		case MODBUS_FC_READ_DISCRETE_INPUTS:
			/* Read functions, 8 values in a byte (nb
			 * of values in the request and byte count in
			 * the response. */
			req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
			req_nb_value = (req_nb_value / 8) + ((req_nb_value % 8) ? 1 : 0);
			rsp_nb_value = rsp[offset + 1];
			break;
		case MODBUS_FC_WRITE_AND_READ_REGISTERS:
		case MODBUS_FC_READ_HOLDING_REGISTERS:
		case MODBUS_FC_READ_INPUT_REGISTERS:
			/* Read functions 1 value = 2 bytes */
			req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
			rsp_nb_value = (rsp[offset + 1] / 2);
			break;
		case MODBUS_FC_WRITE_MULTIPLE_COILS:
		case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
			/* N Write functions */
			req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
			rsp_nb_value = (rsp[offset + 3] << 8) | rsp[offset + 4];
			break;
		case MODBUS_FC_REPORT_SLAVE_ID:
			/* Report slave ID (bytes received) */
			req_nb_value = rsp_nb_value = rsp[offset + 1];
			break;
		default:
			/* 1 Write functions & others */
			req_nb_value = rsp_nb_value = 1;
		}

		if (req_nb_value == rsp_nb_value) 
		{
			rc = rsp_nb_value;
		}
		else 
		{
			rc = -1;
		}
	}
	else 
	{
		rc = -1;
	}

	return rc;
}

/* Builds a TCP request header */
static int _modbus_tcp_build_request_basis(uint8_t slaveID, uint16_t& tid, int function,
	int addr, int nb,
	uint8_t *req)
{
	//modbus_tcp_t *ctx_tcp = ctx->backend_data;

	/* Increase transaction ID */
	if (tid < UINT16_MAX)
		++tid;
	else
		tid = 0;
	req[0] = tid >> 8;
	req[1] = tid & 0x00ff;

	/* Protocol Modbus */
	req[2] = 0;
	req[3] = 0;

	/* Length will be defined later by set_req_length_tcp at offsets 4
	   and 5 */

	req[6] = slaveID;
	req[7] = function;
	req[8] = addr >> 8;
	req[9] = addr & 0x00ff;
	req[10] = nb >> 8;
	req[11] = nb & 0x00ff;

	return _MODBUS_TCP_PRESET_REQ_LENGTH;
}

/* Builds a TCP response header */
static int _modbus_tcp_build_response_basis(uint8_t slaveID, uint16_t tid, int function, uint8_t *rsp)
{
	/* Extract from MODBUS Messaging on TCP/IP Implementation
	   Guide V1.0b (page 23/46):
	   The transaction identifier is used to associate the future
	   response with the request. */
	rsp[0] = tid >> 8;
	rsp[1] = tid & 0x00ff;

	/* Protocol Modbus */
	rsp[2] = 0;
	rsp[3] = 0;

	/* Length will be set later by send_msg (4 and 5) */

	/* The slave ID is copied from the indication */
	rsp[6] = slaveID;
	rsp[7] = function;

	return _MODBUS_TCP_PRESET_RSP_LENGTH;
}

#endif // MSG_PACKAGE_H
