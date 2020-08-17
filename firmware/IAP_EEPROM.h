//΢�Ź��ںţ�����������
//ʹ�õ�Ƭ���ڲ���Flash��ģ��EEPROMʹ��
//STC15W4K48S4 ��Ƭ��������ʹ��IAP�����д�ڲ�Flash
//��ʼ��ַ 0X0000 ���� 0X27FF  ���У�512Byteһ������
//һ��20������  һ���������Դ洢һ������������Ϳ���ֱ�Ӳ������������޸�����

#ifndef	__IAP_EEPROM__H_
#define	__IAP_EEPROM__H_
#include "STC15W.h"
#include "intrins.h"

//��������  29���ֽ�
typedef struct _Config
{
	unsigned char 	a;
	unsigned int 		b;
	unsigned char 	c;
	unsigned char   d;
	unsigned int 		e;
	unsigned int 		f;
	unsigned int 		g;
	unsigned int 		h;
	unsigned int 		i;
	unsigned int 		j;
	unsigned int 		k;
	unsigned int 		l;
	unsigned int 		m;
	unsigned int 		n;
	unsigned int 		o;
	unsigned int 		p;
}EEPROMConfig;

//��Ԫ������ 130���ֽ�
typedef struct _ConfigSecret
{
	unsigned char 	EEPROM_ProductKey[20];
	unsigned char 	EEPROM_ProductSecret[30];
	unsigned char 	EEPROM_DeviceSecret[50];
	unsigned char 	EEPROM_DeviceName[30];
}EEPROMSecret;

//
void EEPROM_SaveSecret(EEPROMSecret *Secret);
void EEPROM_ReadSecret(EEPROMSecret *Secret);
void EEPROM_SaveConfig(EEPROMConfig *Config);
void EEPROM_ReadConfig(EEPROMConfig *Config);
void EEPROM_EnterFactoryMode(void);
void EEPROM_TestWriteSecret(void);
void EEPROM_FistStart(void);

#endif