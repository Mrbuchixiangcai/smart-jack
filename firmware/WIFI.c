//΢�Ź��ںţ�����������
//EMW3080 ILOP �̼��������
#include<WIFI.h>
#include<Uart_1.h>
#include<Uart_2.h>
#include<Timer0.h>
#include<JSON.h>
#include<IAP_EEPROM.h>
#include<DS18B20_T1.h>
#include<Button.h>
#include<Relay.h>
#include<Mode.h>


#define	Debug_SendByte	Send_Data1
#define	Debug_SendStr	  Send_Str1

#define	WIFI_Send_Byte	Send_Data2
#define	WIFI_Send_Str		Send_Str2
#define	WIFI_Reset			P51
#define	WIFI_LED_IO			P53
#define	WIFIPower				P50


static unsigned char WIFI_ILOP_Connection = 0;
static unsigned char WIFI_LED_State = 0;

static void Delay1ms(unsigned int a)
{
	unsigned char i, j;
	unsigned int b=0;
	for(b=0;b<a;b++)
	{
		i = 29;
		j = 183;
		do
		{
			while (--j);
		}while (--i);
	}
}
//��������״ָ̬ʾ�� 300mS����һ��
void WIFI_LED_Loop(void)
{
	WIFI_LED_IO = ~WIFI_LED_IO ;
	if(WIFI_LED_State == 0)
		WIFI_LED_IO = 1;
}
//֡������
static void WIFI_MessageFun(void)
{
	unsigned int MessageCount=0;
	unsigned int i;
	unsigned char Dat;
	unsigned char ReadBuf[80];
	float LinFloat=0;
	
	MessageCount = Get_Byte_Count_WIFI();
	for(i=0;i<MessageCount;i++)
	{
		Get_Byte_WIFI(&Dat);
		ReadBuf[i] = Dat;
		//Debug_SendByte(Dat);
	}
	
	//Debug_SendStr(ReadBuf);
	
	ReadBuf[i] = 0;
	if(Compare_str(ReadBuf,"ERROR") != 255)
	{
		Debug_SendStr("ERROR\r\n");
		return;
	}
	if(Compare_str(ReadBuf,"ILOP,CONNECTED") != 255)
	{
		WIFI_ILOP_Connection = 1;
		WIFI_LED_State = 1;
		Timer0_SendMessage('A');
		return;
	}
	if(Compare_str(ReadBuf,"ILOP,CONNECTING") != 255)
	{
		WIFI_ILOP_Connection = 0;
		WIFI_LED_State = 0;
		return;
	}
	if(Compare_str(ReadBuf,"SETJSON,property") != 255) //������JSON������
	{
		Dat = Compare_str(ReadBuf,",{");
		Dat = Dat + 1;
		i = 0;
		while(ReadBuf[Dat])
		{
			ReadBuf[i++] = ReadBuf[Dat++];
		}
		ReadBuf[i++] = 0;//��ȡ��JSON�ַ���
		//Debug_SendStr(ReadBuf);
		
		if(Get_Key_Value(ReadBuf,"PowerSwitch",&LinFloat))
		{
			i = LinFloat;
			if(i)
			{
				Mode_OpenRelay();
			}
			else
			{
				CloseRelay(1);
			}
			return;
		}
		
	}
}
//�رջ���
void WIFI_CloseRTE(void)
{
	WIFI_Send_Str("AT+UARTE=OFF\r");
}
//����ILOP��Ϣ
void WIFI_SetILOP(void)
{
	EEPROMSecret Secret;
	unsigned char Buf[150]="AT+ILOPSET=";
	unsigned char BufLen=11;
	unsigned char *p;
	
	EEPROM_ReadSecret(&Secret);
	
	p = Secret.EEPROM_ProductKey;
	BufLen=11;
	while(*p)
	{
		Buf[BufLen++] = *p++;
	}
	Buf[BufLen++] = ',';
	
	p = Secret.EEPROM_ProductSecret;
	while(*p)
	{
		Buf[BufLen++] = *p++;
	}
	Buf[BufLen++] = ',';
	
	p = Secret.EEPROM_DeviceSecret;
	while(*p)
	{
		Buf[BufLen++] = *p++;
	}
	Buf[BufLen++] = ',';
	
	p = Secret.EEPROM_DeviceName;
	while(*p)
	{
		Buf[BufLen++] = *p++;
	}
	Buf[BufLen++] = '\r';
	Buf[BufLen++] = 0;
	
	WIFI_Send_Str(Buf);
}
//��ILOP����
void WIFI_StartILOP(void)
{
	WIFI_Send_Str("AT+ILOPSTART\r");
}
//��������
void WIFI_StartAWS(void)
{
	WIFI_Send_Str("AT+ILOPAWSSTART\r");
}
//����󶨹�ϵ�������ָ�����������
void WIFI_ResetAuthor(void)
{
	WIFI_Send_Str("AT+ILOPRESET\r");
}
//�ϱ�������Ϣ�������� �¶�+�̵���
void WIFI_SubStation(void)
{
	unsigned char SendBuf[100]="AT+ILOPSENDJSON=property,";
	unsigned char JsonStr[100]="";
	unsigned char Len=0;
	unsigned char LenStr[5];
	
	if(WIFI_ILOP_Connection == 0)
		return;
	
	Len = JSON_Join_Key(JsonStr,"CurrentTemperature",Read_T1());
	Len = JSON_Join_Key(JsonStr,"PowerSwitch",GetRelay(1));
	if(Len>99)
	{
		LenStr[0] = Len/100 + 0x30;
		LenStr[1] = Len/10%10 + 0x30;
		LenStr[2] = Len%10 + 0x30;
		LenStr[3] = '\r';
		LenStr[4] = 0;
	}
	else
	{
		LenStr[0] = Len/10 + 0x30;
		LenStr[1] = Len%10 + 0x30;
		LenStr[2] = '\r';
		LenStr[3] = 0;
	}
	Sum_Str_Tail(SendBuf,LenStr);
	Sum_Str_Tail(SendBuf,JsonStr);
	WIFI_Send_Str(SendBuf);
	//Debug_SendStr("Debug\r\n");
}
//��ʼ��WIFIģ��
void WIFI_Init(void)
{
	WIFI_Reset = 0;
	WIFIPower = 0;
	Delay1ms(100);
	WIFI_Reset = 1;
	WIFIPower = 1;
	Uart2_SetMessageFun(WIFI_MessageFun);
	
	Timer_0_Add_Fun_Once(2000,WIFI_CloseRTE);
	Timer_0_Add_Fun_Once(3000,WIFI_SetILOP);
	Timer_0_Add_Fun_Once(5000,WIFI_StartILOP);
	
	Timer_0_Add_Fun(300000,WIFI_SubStation);//������ϱ�һ���¶���Ϣ
}
//



//



