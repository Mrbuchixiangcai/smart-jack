//΢�Ź��ںţ�����������
#include<STC15W.h>
#include<intrins.h>
#include<Uart_1.h>
#include<Uart_2.h>
#include<Timer0.h>
#include<DS18B20_T1.h>
#include<Button.h>
#include<Relay.h>
#include<WIFI.h>
#include<IAP_EEPROM.h>
#include<Mode.h>
#include<WDT.h>
#include<DeviceName.h>
/*
	ϵͳ�¼����ձ�
	A  Main_Reset						ϵͳ��λ����������ģʽʹ�á�����ʲôǰ�����ӳɹ�������A�¼�
	
*/

//�����λ
void Main_Reset(void)
{
	IAP_CONTR = 0x60;
}
//������������ģʽ�����޸��ڲ����á�
void Main_EnterFactory(void)
{
	Timer0_Init();
	WIFI_ResetAuthor();
	Timer_0_Add_Fun_Once(1000,WIFI_StartAWS);
	Timer_0_Add_Fun(150,Mode_3LED_Loop);
	Timer_0_Add_Fun(2,Uart2_CheckMessageLoop); //����ͨ��2��֡��
	Timer0_Add_MessageFun('A',Main_Reset);
	while(1)
	{
		Timer0_SYS_APP_LOOP();
		Timer0_SYS_APP_LOOP_Message();
		Timer0_SYS_APP_LOOP_Once();
		WDT_Clear();
	}
}
void main(void)
{
	P0M1=0;P0M0=0;P1M1=0;P1M0=0;
	P2M1=0;P2M0=0;P3M1=0;P3M0=0;
	P4M1=0;P4M0=0;P5M1=0;P5M0=0;//�ϵ��ʼ������IO��Ϊ��ͨIO
	WDT_Init();
	EEPROM_FistStart();
	Init_Uart1();
	Init_Uart2();
	Timer0_Init();
	Button_Init();
	WIFI_Init();
	EA = 1;
	
	Timer_0_Add_Fun(1010,Read_DS18B20_T1_Loop); //��ʱ��ȡDS18B20
	Timer_0_Add_Fun(10,Button_Loop); //�������ײ�ҵ��
	Timer_0_Add_Fun(2,Uart2_CheckMessageLoop); //����ͨ��2��֡��
	Timer_0_Add_Fun(2,Uart1_CheckMessageLoop); //����ͨ��1��֡��
	Timer_0_Add_Fun(100,WIFI_LED_Loop); //����״ָ̬ʾ��
	Timer_0_Add_Fun(1000,Mode_3LED_Loop); 
	
	Button_SetFun(1,'C',Main_EnterFactory);
	Button_SetFun(1,'D',Mode_DianDong);
	
	Uart1_SetMessageFun(Uart1_MessageFun);
	
	while(1)
	{
		Timer0_SYS_APP_LOOP();
		Timer0_SYS_APP_LOOP_Message();
		Timer0_SYS_APP_LOOP_Once();
		WDT_Clear();
	}
	
}