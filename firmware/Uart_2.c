//΢�Ź��ںţ�����������
//����2  115200bps 22.1184MHZ
#include<Uart_2.h>
#include "circle_queue.h"

#define S2RI  0x01              //S2CON.0
#define S2TI  0x02              //S2CON.1

static bit busy2=0;//COM2�õ��ķ���æ��־
static CircleQueue_t WIFI_Read_Buf;//WIFI���ջ�����
static unsigned char ReadingByteCount=0;
static void (*MessageFun)(void); //��֡������

//����2�жϴ������
void INT_Uart2(void) interrupt 8
{
 	if (S2CON & S2RI)
  {
    S2CON &= ~S2RI;         //���S2RIλ
		ReadingByteCount++;
		Queue_Put(&WIFI_Read_Buf,S2BUF);//���ݽ������
  }
   if (S2CON & S2TI)
   {
     S2CON &= ~S2TI;         //���S2TIλ
     busy2 = 0;               //��æ��־
   }
}
// ����: ���ڳ�ʼ������.
void Init_Uart2(void)
{
	S2CON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	T2L = 0xD0;		//�趨��ʱ��ֵ
	T2H = 0xFF;		//�趨��ʱ��ֵ 115200    22.1184MHZ
	AUXR |= 0x10;	//������ʱ��2
	IE2 = 0x01;    //ʹ�ܴ���2�ж�
	
	Queue_Init(&WIFI_Read_Buf);
	MessageFun = 0;
}
//COM2����һ���ֽ�
void Send_Data2(unsigned char dat)
{
	busy2 = 1;
	S2BUF = dat;
	
	//SBUF = dat;
	
	while(busy2);
	busy2 = 1;
}
//COM2����һ���ַ���
void Send_Str2(unsigned char *s)
{
	do
		Send_Data2(*s++);
	while(*s);
}
//2msѭ�����ã���֡��
void Uart2_CheckMessageLoop(void)
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
void Uart2_SetMessageFun(void (*Fun)(void))
{
	MessageFun = Fun;
}
//�ӽ��ջ�����ȡ��һ���ֽ���Ϣ
unsigned char Get_Byte_WIFI(unsigned char *dat)
{
	return(Queue_Get(&WIFI_Read_Buf,dat));
}
//��ս��ջ������Ķ���
void Clear_WIFI(void)
{
	Queue_Init(&WIFI_Read_Buf);
}
//
unsigned int Get_Byte_Count_WIFI(void)
{
	return (Queue_Length(&WIFI_Read_Buf));
}
//



