//΢�Ź��ںţ�����������
//����ģʽ��ҵ���߼�
#include<Mode.h>
#include<IAP_EEPROM.h>
#include<Relay.h>
#include<Timer0.h>
#include<WIFI.h>
#include<Button.h>


#define	LED1	P07
#define	LED2	P06
#define	LED3	P05

void ModeCloseRelay(void)
{
	CloseRelay(1);
}
//�򿪼̵���������ʱ��
void Mode_OpenRelay(void)
{
	OpenRelay(1);
}
//����LEDһ����˸����ʾ��������
void Mode_3LED_Loop(void)
{
	LED1 = ~LED3;
	LED2 = ~LED3;
	LED3 = LED1;
}
//�����㶯�߼�
void Mode_DianDong(void)
{
	Relay_Turn();
	WIFI_SubStation();
}



//
