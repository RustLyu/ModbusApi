#ifndef MODBUS_API_H
#define MODBUS_API_H

// 导出的dll接口

#include "Modbus.h"

/*
  创建modbus上下文
*/
extern "C" EXPORTS_DEMO const Modbus* createModbusContext(const char* ip, int port);

/*
  连接服务器(PLC)
*/
extern "C" EXPORTS_DEMO bool connect2Server(Modbus* ctx);

/*
  断开连接
*/
extern "C" EXPORTS_DEMO bool closeConnect(Modbus* ctx);

/*
  释放资源
*/
extern "C" EXPORTS_DEMO void freeContext(Modbus* ctx);

/*
  功能码:0x01
  读取线圈状态，可读取多个连续线圈的状态
  Modbus *context：Modbus实例
  int addr: 线圈地址
  int count:读取线圈的个数
  int16_t* dst:传出的状态值
  返回值:成功:MODBUS_RET_OK(0) 失败:MODBUS_RET_ERROR(-1)
*/
extern "C" EXPORTS_DEMO int readMulBit(Modbus* context, int addr, int count, int16_t* dst);

/*
  功能码:0x02
  读取输入状态，可读取多个连续输入的状态
  Modbus *context：Modbus实例
  int addr：输入地址
  int count：读取输入的个数
  int16_t* dst:传出的状态值
  返回值:成功:MODBUS_RET_OK(0) 失败:MODBUS_RET_ERROR(-1)
*/
extern "C" EXPORTS_DEMO int readInputMulBit(Modbus* context, int addr, int count, int16_t* dst);

/*
  功能码: 0x03
  读取保持寄存器的值，可读取多个连续输入保持寄存器
  Modbus *context：Modbus实例
  int addr：输入地址
  int count：读取保持寄存器的个数
  int16_t* dst:传出的寄存器值
  返回值:成功:MODBUS_RET_OK(0) 失败:MODBUS_RET_ERROR(-1)
*/
extern "C" EXPORTS_DEMO int readHoldMulRegister(Modbus* context, int addr, int count, int16_t* dst);

/*
  功能码:0x04
  读取输入寄存器的值，可读取多个连续输入输入寄存器
  Modbus *context：Modbus实例
  int addr：输入地址
  int count：读取输入寄存器的个数
  int16_t* dst:传出的寄存器值
  返回值:成功:MODBUS_RET_OK(0) 失败:MODBUS_RET_ERROR(-1)
*/
extern "C" EXPORTS_DEMO int readInputMulRegister(Modbus* context, int addr, int count, int16_t* dst);

/*
  功能码:0x05
  写入单个线圈的状态
  Modbus *context：Modbus实例
  int addr：线圈地址
  int value：线圈状态
  返回值:成功:MODBUS_RET_OK(0) 失败:MODBUS_RET_ERROR(-1)
*/
extern "C" EXPORTS_DEMO int writeSigleBit(Modbus* context, int addr, int value);

/*
  功能码:0x06
  写入单个寄存器
  Modbus *context：Modbus实例
  int addr：寄存器地址
  int value：寄存器的值
  返回值:成功:MODBUS_RET_OK(0) 失败:MODBUS_RET_ERROR(-1)
*/
extern "C" EXPORTS_DEMO int writeSigleRegister(Modbus* context, int addr, int value);

/*
  功能码:0x0f
  写入多个连续线圈的状态
  Modbus *context：Modbus实例
  int addr：线圈地址
  int count：线圈个数
  const uint8_t *src：将写入多个线圈状态
  返回值:成功:MODBUS_RET_OK(0) 失败:MODBUS_RET_ERROR(-1)
*/
extern "C" EXPORTS_DEMO int writeMulBit(Modbus* context, int addr, int count, int16_t* src);

/*
  功能码:0x10
  写入多个连续寄存器
  Modbus *context：Modbus实例
  int addr：寄存器地址
  int count：寄存器的个数
  const uint16_t *src：将写入多个寄存器的值
  返回值:成功:MODBUS_RET_OK(0) 失败:MODBUS_RET_ERROR(-1)
*/
extern "C" EXPORTS_DEMO int writeMulRegister(Modbus* context, int addr, int count, int16_t* src);

/*
  打印错误信息
  ERRORFLAG code : 错误码
*/
extern "C" EXPORTS_DEMO void printErrorMsg(ERRORFLAG code);

#endif // MODBUS_API_H
