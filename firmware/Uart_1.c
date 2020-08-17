//���Դ���1����USBת����ֱ������   115200/22.1184MHZ
//΢�Ź��ںţ�����������
#include<Uart_1.h>
#include<Timer0.h>
#include "circle_queue.h"

static bit busy1=0;//COM1�õ��ķ���æ��־

static CircleQueue_t WIFI_Read_Buf;//WIFI���ջ�����
static unsigned char ReadingByteCount=0;
static void (*MessageFun)(void); //��֡������


//����1�жϴ������
void INT_Uart1(void) interrupt 4
{
	if(RI)//��ն��жϱ�־λ
	{
		RI = 0;
		ReadingByteCount++;
		Queue_Put(&WIFI_Read_Buf,SBUF);//���ݽ������
		if(SBUF == 0XFA)//�յ� 0XFA ����Ƭ����λ����д��
			IAP_CONTR = 0x60;
	}
	if(TI)//��շ����жϱ�־λ
	{
		TI = 0;
		busy1 = 0;
	}
}
// ����: ���ڳ�ʼ������.
void Init_Uart1(void)
{
	SCON = 0x50;//
	AUXR |= 0x01;//��ʱ��2��1T
	AUXR |= 0x04;
	T2L = 0xD0;		//�趨��ʱ��ֵ
	T2H = 0xFF;		//�趨��ʱ��ֵ  115200    22.1184MHZ
	AUXR |= 0x10;		//������ʱ��2
	ES = 1;//����COM1���п��ж�
	TI = 1;	
	
	Queue_Init(&WIFI_Read_Buf);
	MessageFun = 0;
}
//COM1����һ���ֽ�
void Send_Data1(unsigned char dat)
{
	busy1 = 1;
	SBUF = dat;
	while(busy1);
	busy1 = 1;
}
//����һ���ַ���
void Send_Str1(unsigned char *s)
{
	while(*s)
	{
		Send_Data1(*s++);
	}
}
//2msѭ�����ã���֡��
void Uart1_CheckMessageLoop(void)
{
	static unsigned char ReadingByteCount2=0;
	static unsigned char NoByteCount=0;
	
	if(ReadingByteCount > ReadingByteCount2)
	{
		NoByteCount=0;
		ReadingByteCount2 = ReadingByteCount;
	}
	else
	{
		NoByteCount ++;
	}
	
	if(NoByteCount > 5) //���10msû���յ����ݣ���ִ��֡������
	{
		NoByteCount = 0;
		ReadingByteCount = 0;
		ReadingByteCount2 = 0;
		if(MessageFun != 0)
			MessageFun();
	}
}
//�����յ�һ֡���ݺ�ִ�еĴ���
void Uart1_SetMessageFun(void (*Fun)(void))
{
	MessageFun = Fun;
}
//�ӽ��ջ�����ȡ��һ���ֽ���Ϣ
unsigned char Get_Byte_Uart1(unsigned char *dat)
{
	return(Queue_Get(&WIFI_Read_Buf,dat));
}
//��ս��ջ������Ķ���
void Clear_Uart1(void)
{
	Queue_Init(&WIFI_Read_Buf);
}
//
unsigned int Get_Byte_Count_Uart1(void)
{
	return (Queue_Length(&WIFI_Read_Buf));
}
//









