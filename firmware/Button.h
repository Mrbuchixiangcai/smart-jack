//΢�Ź��ںţ�����������
//����ɨ��
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include<STC15W.h>

void Button_Loop(void);

void Button_SetFun(unsigned char CH,unsigned char Type,void (*Fun)(void));//Type DΪ�㶯��CΪ����

void Button_Init(void);


unsigned char Button_GetState(unsigned char CH);

#endif