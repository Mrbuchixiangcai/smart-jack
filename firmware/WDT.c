//΢�Ź��ںţ�����������
//���Ź�����
#include<WDT.h>

//��ʼ��WDT  ��Ƶ64 22.1184MHZ ��Լ1S
void WDT_Init(void)
{
	WDT_CONTR = 0x07;       //���Ź���ʱ�����ʱ����㹫ʽ: (12 * 32768 * PS) / FOSC (��)
	WDT_CONTR |= 0x20;      //�������Ź�
}
//ι��
void WDT_Clear(void)
{
	WDT_CONTR |= 0x10;
}
//

