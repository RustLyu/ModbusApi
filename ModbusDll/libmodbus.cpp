// libmodbus.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "Modbus.h"
#include "ModbusApi.h"

int main()
{
	Modbus* ctx = new Modbus("127.0.0.1", 502);
	ctx->connect2Server();
	int i = 0;
	while (1)
	{
		int16_t dst[INIT_BUFFER_LENGTH];
		memset(dst, 0, INIT_BUFFER_LENGTH);
		int num = i % 4;
		if (num == 0)
		{
			// 05
			writeSigleBit(ctx, 0, 1);
			// 01
			int ret = readMulBit(ctx, 0, 1, dst);
			if (ret != -1)
			{
				std::cout << "01 " << dst[0] << std::endl;
			}
			else
			{
				printErrorMsg(ctx->getErrorFlag());
				//std::cout << "01 error" << std::endl;
			}
		}
		else if (num == 1)
		{
			int8_t src[3];
			src[0] = 1;
			src[1] = 0;
			src[2] = 1;
			// 0f
			writeMulBit(ctx, 0, 3, src);
			// 02
			//readInputMulBit(ctx, 0, 1, dst);
			int ret = readMulBit(ctx, 0, 3, dst);
			if (ret != -1)
			{
				for (int i = 0; i < 3; ++i)
					std::cout << "02 " << dst[i] << std::endl;
			}
			else
			{
				printErrorMsg(ctx->getErrorFlag());
				//std::cout << "02 error" << std::endl;
			}
		}
		else if (num == 2)
		{
			// 06
			writeSigleRegister(ctx, 0, 99);
			// 03
			int ret = readHoldMulRegister(ctx, 0, 1, dst);
			if (ret != -1)
			{
				std::cout << "03 " << dst[0] << std::endl;
			}
			else
			{
				printErrorMsg(ctx->getErrorFlag());
				//std::cout << "03 error" << std::endl;
			}
		}
		else if (num == 3)
		{
			int8_t src[5];
			src[0] = 4;
			src[1] = 5;
			src[2] = 6;
			src[3] = 7;
			src[4] = 8;
			// 16
			writeMulRegister(ctx, 0, 5, src);
			// 04
			//readInputMulRegister(ctx, 0, 1, dst);
			int ret = readHoldMulRegister(ctx, 0, 5, dst);
			if (ret != -1)
			{
				for (int i = 0; i < 5; ++i)
					std::cout << "04 " << dst[i] << std::endl;
			}
			else
			{
				printErrorMsg(ctx->getErrorFlag());
				//std::cout << "04 error" << std::endl;
			}
		}
		++i;
		//Sleep(50);
	}
	//ctx->close();
	std::cout << "Hello World!\n";
	delete ctx;
}
