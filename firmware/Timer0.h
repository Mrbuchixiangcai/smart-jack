//΢�Ź��ںţ�����������
//Timer0 22.1184MHZ
#ifndef __TIMER0_H__
#define __TIMER0_H__
#include "STC15W.h"

//��ʼ����ʱ��0
void Timer0_Init(void);
//ϵͳ��ʱ���ж�����ִ�еĺ���
void Timer0_SYS_APP_LOOP(void);
void Timer0_SYS_APP_LOOP_Message(void);
void Timer0_SYS_APP_LOOP_Once(void);

//���һ����������ʱ����
unsigned char Timer_0_Add_Fun(unsigned long Time,void (*Fun)(void));
unsigned char Timer_0_Add_Fun_Once(unsigned long Time,void (*Fun)(void));
//���һ����Ϣ�ͺ�����ӳ���ϵ
unsigned char Timer0_Add_MessageFun(unsigned char Message,void (*Fun)(void));
//��ϵͳ����һ���ź�
unsigned char Timer0_SendMessage(unsigned char Message);

#endif