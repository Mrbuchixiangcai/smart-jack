//Timer0    22.1184MHZ
//΢�Ź��ںţ�����������
#include "Timer0.h"
#include "Uart_1.h"

#define	DebugSendByte	Send_Data1

#define	FOSC	22118400
#define	Timer_0_List_Count	20

struct 
{
	unsigned char Flag[Timer_0_List_Count];
	void (*Fun_Point_List[Timer_0_List_Count])(void);
	unsigned long Counter[Timer_0_List_Count];
	unsigned long Timer[Timer_0_List_Count];
}Timer0_Struct;

struct 
{
	unsigned char Flag[Timer_0_List_Count];
	void (*Fun_Point_List[Timer_0_List_Count])(void);
	unsigned long Counter[Timer_0_List_Count];
	unsigned long Timer[Timer_0_List_Count];
}Timer0_Struct_Once;

struct
{
	unsigned char MessageQueue[Timer_0_List_Count];
	unsigned char MessageList[Timer_0_List_Count];
	void (*MessageFun_Point_List[Timer_0_List_Count])(void);
	
}Timer0_Message_Struct;

unsigned char Timer0_Handler_Flag=0;
unsigned char Timer0_Handler_Flag_Message=0;
unsigned char Timer0_Handler_Flag_Once=0;
//��ʼ����ʱ��0
void Timer0_Init(void)
{
	unsigned char i=0;
	
	for(i=0;i<Timer_0_List_Count;i++)
	{
		Timer0_Struct.Flag[i] = 0;
		Timer0_Struct.Counter[i] = 0;
		
		Timer0_Struct_Once.Flag[i] = 0;
		Timer0_Struct_Once.Counter[i] = 0;
		
		Timer0_Message_Struct.MessageQueue[i] = 0;
		Timer0_Message_Struct.MessageList[i] = 0;
	}
	
	AUXR |= 0x80;//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;//���ö�ʱ��ģʽ
    TL0 = 0x9A;//���ö�ʱ��ֵ
	TH0 = 0xA9;//���ö�ʱ��ֵ
	TF0 = 0;//���TF0��־
    TR0 = 1;//������ʱ��
    ET0 = 1;//ʹ�ܶ�ʱ���ж�
	
}
//���һ����������ʱ����
unsigned char Timer_0_Add_Fun(unsigned long Time,void (*Fun)(void))
{
	unsigned char i=0;
	for(i=0;i<Timer_0_List_Count;i++)
	{
		if(Timer0_Struct.Flag[i] == 0)
		{
			Timer0_Struct.Flag[i] = 1;
			Timer0_Struct.Counter[i] = 0;
			Timer0_Struct.Fun_Point_List[i] = Fun;
			Timer0_Struct.Timer[i] = Time-1;
			return 1;
		}
	}
	return 0;
}
//���һ��һ����ִ�к�������ʱ����
unsigned char Timer_0_Add_Fun_Once(unsigned long Time,void (*Fun)(void))
{
	unsigned char i=0;
	for(i=0;i<Timer_0_List_Count;i++)
	{
		if(Timer0_Struct_Once.Flag[i] == 0)
		{
			Timer0_Struct_Once.Flag[i] = 1;
			Timer0_Struct_Once.Counter[i] = 0;
			Timer0_Struct_Once.Fun_Point_List[i] = Fun;
			Timer0_Struct_Once.Timer[i] = Time-1;
			return 1;
		}
	}
	return 0;
}


//���һ����Ϣ�ͺ�����ӳ���ϵ
unsigned char Timer0_Add_MessageFun(unsigned char Message,void (*Fun)(void))
{
	unsigned char i;
	for(i=0;i<Timer_0_List_Count;i++)
	{
		if(!Timer0_Message_Struct.MessageList[i])
		{
			Timer0_Message_Struct.MessageList[i] = Message;
			Timer0_Message_Struct.MessageFun_Point_List[i] = Fun;
			return 1;
		}
	}
	return 0;
}
//��ϵͳ����һ���ź�
unsigned char Timer0_SendMessage(unsigned char Message)
{
	unsigned char i;
	for(i=0;i<Timer_0_List_Count;i++)
	{
		if(!Timer0_Message_Struct.MessageQueue[i])
		{
			Timer0_Message_Struct.MessageQueue[i] = Message;
			return 1;
		}
	}
	return 0;
}
//ϵͳ��ʱ���ж�����ִ�еĺ���
void Timer0_SYS_APP_LOOP(void)
{
	unsigned char i=0,j=0;
	if(Timer0_Handler_Flag == 0)
		return;
	Timer0_Handler_Flag = 0;
	for(i=0;i<Timer_0_List_Count;i++) //ѭ����ʱ�ĺ���
	{
		if(Timer0_Struct.Flag[i])
		{
			Timer0_Struct.Counter[i] ++ ;
			if(Timer0_Struct.Counter[i] > Timer0_Struct.Timer[i])
			{
				Timer0_Struct.Counter[i] = 0;
				Timer0_Struct.Fun_Point_List[i]();
			}
		}
	}
	
	
}
//ϵͳѭ��ִ��-���䴦��
void Timer0_SYS_APP_LOOP_Message(void)
{
	unsigned char i=0,j=0;
	if(Timer0_Handler_Flag_Message == 0)
		return;
	Timer0_Handler_Flag_Message = 0;
	
	for(i=0;i<Timer_0_List_Count;i++) //������Ϣ�����еĺ���
	{
		if(Timer0_Message_Struct.MessageQueue[i])
		{
			for(j=0;j<Timer_0_List_Count;j++)
			{
				if(Timer0_Message_Struct.MessageQueue[i] == Timer0_Message_Struct.MessageList[j])
				{
					//DebugSendByte(Timer0_Message_Struct.MessageQueue[i]);
					Timer0_Message_Struct.MessageFun_Point_List[j]();
					j = Timer_0_List_Count + 10;
				}
			}
			Timer0_Message_Struct.MessageQueue[i] = 0;
		}
	}
}
//ϵͳѭ��������ִ�к�������
void Timer0_SYS_APP_LOOP_Once(void)
{
	unsigned char i=0,j=0;
	if(Timer0_Handler_Flag_Once == 0)
		return;
	Timer0_Handler_Flag_Once = 0;
	
	for(i=0;i<Timer_0_List_Count;i++) //һ���Զ�ʱ����
	{
		if(Timer0_Struct_Once.Flag[i])
		{
			Timer0_Struct_Once.Counter[i] ++ ;
			if(Timer0_Struct_Once.Counter[i] > Timer0_Struct_Once.Timer[i])
			{
				Timer0_Struct_Once.Counter[i] = 0;
				Timer0_Struct_Once.Flag[i] = 0;
				Timer0_Struct_Once.Fun_Point_List[i]();
				Timer0_Struct_Once.Flag[i] = 0;
			}
		}
	}
	
}
//�����ӵ��˶�ʱ��0.
void Timer0_Interrupt(void) interrupt 1
{
	Timer0_Handler_Flag = 1;
	Timer0_Handler_Flag_Message = 1;
	Timer0_Handler_Flag_Once = 1;
}

//

//


