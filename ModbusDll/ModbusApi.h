#ifndef MODBUS_API_H
#define MODBUS_API_H

// ������dll�ӿ�

#include "Modbus.h"

/*
  ����modbus������
*/
extern "C" EXPORTS_DEMO const Modbus* createModbusContext(const char* ip, int port);

/*
  ���ӷ�����(PLC)
  ����ֵ��0 ���ӳɹ���-1 ʧ��
*/
extern "C" EXPORTS_DEMO int connect2Server(Modbus* ctx);

/*
  �Ͽ�����
*/
extern "C" EXPORTS_DEMO bool closeConnect(Modbus* ctx);

/*
  �ͷ���Դ
*/
extern "C" EXPORTS_DEMO void freeContext(Modbus* ctx);

/*
  �������״̬
  ����ֵ���Ѿ�����(true) �Ѿ��Ͽ�(false)
*/
extern "C" EXPORTS_DEMO bool isConnect(Modbus* ctx);

/*
  ���÷��ͳ�ʱʱ��
  Modbus *context��Modbusʵ��
  int sec ��
  int umesc ΢�� ʱ��
  ����ֵ�����óɹ�(true) ����ʧ��(false)
*/
extern "C" EXPORTS_DEMO bool setMaxRequestTimeOut(Modbus* ctx, uint32_t sec, uint32_t umesc);

/*
  ���ý��ճ�ʱʱ��
  Modbus *context��Modbusʵ��
  int sec ��
  int umesc ΢�� ʱ��
  ����ֵ�����óɹ�(true) ����ʧ��(false)
*/
extern "C" EXPORTS_DEMO bool setMaxResponseTimeOut(Modbus* ctx, uint32_t sec, uint32_t umesc);

/*
  ������:0x01
  ��ȡ��Ȧ״̬���ɶ�ȡ���������Ȧ��״̬
  Modbus *context��Modbusʵ��
  int addr: ��Ȧ��ַ
  int count:��ȡ��Ȧ�ĸ���
  int16_t* dst:������״ֵ̬
  ����ֵ:�ɹ�:MODBUS_RET_OK(0) ʧ��:MODBUS_RET_ERROR(-1)
*/
extern "C" EXPORTS_DEMO int readMulBit(Modbus* context, int addr, int count, uint16_t* dst);

/*
  ������:0x02
  ��ȡ����״̬���ɶ�ȡ������������״̬
  Modbus *context��Modbusʵ��
  int addr�������ַ
  int count����ȡ����ĸ���
  int16_t* dst:������״ֵ̬
  ����ֵ:�ɹ�:MODBUS_RET_OK(0) ʧ��:MODBUS_RET_ERROR(-1)
*/
extern "C" EXPORTS_DEMO int readInputMulBit(Modbus* context, int addr, int count, uint16_t* dst);

/*
  ������: 0x03
  ��ȡ���ּĴ�����ֵ���ɶ�ȡ����������뱣�ּĴ���
  Modbus *context��Modbusʵ��
  int addr�������ַ
  int count����ȡ���ּĴ����ĸ���
  int16_t* dst:�����ļĴ���ֵ
  ����ֵ:�ɹ�:MODBUS_RET_OK(0) ʧ��:MODBUS_RET_ERROR(-1)
*/
extern "C" EXPORTS_DEMO int readHoldMulRegister(Modbus* context, int addr, int count, uint16_t* dst);

/*
  ������:0x04
  ��ȡ����Ĵ�����ֵ���ɶ�ȡ���������������Ĵ���
  Modbus *context��Modbusʵ��
  int addr�������ַ
  int count����ȡ����Ĵ����ĸ���
  int16_t* dst:�����ļĴ���ֵ
  ����ֵ:�ɹ�:MODBUS_RET_OK(0) ʧ��:MODBUS_RET_ERROR(-1)
*/
extern "C" EXPORTS_DEMO int readInputMulRegister(Modbus* context, int addr, int count, uint16_t* dst);

/*
  ������:0x05
  д�뵥����Ȧ��״̬
  Modbus *context��Modbusʵ��
  int addr����Ȧ��ַ
  int value����Ȧ״̬
  ����ֵ:�ɹ�:MODBUS_RET_OK(0) ʧ��:MODBUS_RET_ERROR(-1)
*/
extern "C" EXPORTS_DEMO int writeSigleBit(Modbus* context, int addr, int value);

/*
  ������:0x06
  д�뵥���Ĵ���
  Modbus *context��Modbusʵ��
  int addr���Ĵ�����ַ
  int value���Ĵ�����ֵ
  ����ֵ:�ɹ�:MODBUS_RET_OK(0) ʧ��:MODBUS_RET_ERROR(-1)
*/
extern "C" EXPORTS_DEMO int writeSigleRegister(Modbus* context, int addr, int value);

/*
  ������:0x0f
  д����������Ȧ��״̬
  Modbus *context��Modbusʵ��
  int addr����Ȧ��ַ
  int count����Ȧ����
  const uint8_t *src����д������Ȧ״̬
  ����ֵ:�ɹ�:MODBUS_RET_OK(0) ʧ��:MODBUS_RET_ERROR(-1)
*/
extern "C" EXPORTS_DEMO int writeMulBit(Modbus* context, int addr, int count, uint16_t* src);

/*
  ������:0x10
  д���������Ĵ���
  Modbus *context��Modbusʵ��
  int addr���Ĵ�����ַ
  int count���Ĵ����ĸ���
  const uint16_t *src����д�����Ĵ�����ֵ
  ����ֵ:�ɹ�:MODBUS_RET_OK(0) ʧ��:MODBUS_RET_ERROR(-1)
*/
extern "C" EXPORTS_DEMO int writeMulRegister(Modbus* context, int addr, int count, uint16_t* src);

/*
  ��ӡ������Ϣ
  ERRORFLAG code : ������
*/
extern "C" EXPORTS_DEMO void printErrorMsg(Modbus* context);

#endif // MODBUS_API_H
