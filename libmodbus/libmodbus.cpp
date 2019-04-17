// libmodbus.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "Modbus.h"
#include "ModbusApi.h"
#include <windows.h>

union int32_int16
{
	int16_t int16Array[2];
	float value;
};

int main()
{
	Modbus* ctx = new Modbus("127.0.0.1", 502);
	//Modbus* ctx = new Modbus("127.0.0.1", 502);
	ctx->connect2Server();
	int i = 0;
	while (1)
	{
		int16_t dst[INIT_BUFFER_LENGTH];
		memset(dst, -1, INIT_BUFFER_LENGTH);
		int num = i % 4;
		if (num == 0)
		{
			//int16_t src[5];
			//src[0] = 1 << 13;
			//int s = sizeof(int16_t);
			////writeSigleRegister(ctx, 5, src[0]);
			//int ret = writeMulRegister(ctx, 5, 1, src);
			//std::cout << 16 << " /28: " << ret << std::endl;
			/*int16_t src[5];
			src[0] = 0;
			int s = sizeof(int16_t);
			writeSigleRegister(ctx, 5, src[0]);*/
			// 05 写入单线圈
			//writeSigleBit(ctx, 5, 1);
			//// 01
			//int ret = readMulBit(ctx, 0, 1, dst);
			//if (ret != -1)
			//{
			//	std::cout << "01 " << dst[0] << std::endl;
			//}
			//else
			//{
			//	printErrorMsg(ctx->getErrorFlag());
			//	//std::cout << "01 error" << std::endl;
			//}
		}
		else if (num == 1)
		{
			/*int16_t src[5];
			src[0] = 0;
			int s = sizeof(int16_t);
			writeSigleRegister(ctx, 5, src[0]);*/
		}
		else if (num == 2)
		{
			//{
			//	int16_t src[5];
			//	src[0] = 1 << 12;
			//	int s = sizeof(int16_t);
			//	//writeSigleRegister(ctx, 5, src[0]);
			//	int ret = writeMulRegister(ctx, 5, 1, src);
			//	std::cout << 16 << "/59 : " << ret << std::endl;
			//}
			int ret = readHoldMulRegister(ctx, 0, 1, dst);
			ret += 1;
			//// 03
			//int ret = readHoldMulRegister(ctx, 5, 1, dst);
			//if (ret != -1)
			//{
			//	std::cout << "03 " << dst[0] << std::endl;
			//}
			//else
			//{
			//	printErrorMsg(ctx->getErrorFlag());
			//	//std::cout << "03 error" << std::endl;
			//}
		}
		else if (num == 3)
		{
			//{
			//	int16_t src[5];
			//	src[0] = 0;
			//	int s = sizeof(int16_t);
			//	// writeSigleRegister(ctx, 5, src[0]);
			//	int ret = writeMulRegister(ctx, 5, 1, src);
			//}
		}
		++i;
		Sleep(300);
	}
	//ctx->close();
	std::cout << "Hello World!\n";
	delete ctx;
}
