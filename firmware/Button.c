//΢�Ź��ںţ�����������
//����ɨ��
#include<Button.h>
#include<Uart_1.h>
#include<intrins.h>
#include<Timer0.h>

#define	Debug_SendData	Send_Data1

#define	Button_IO_0	P12		//����Ԥ��������Ҫ����Ӳ���ϻ�һ���������������������
#define	Button_IO_1	P16   //����



#define Button_ListCount		2  //����������
#define	Button_G_Time		15	 //��������ʱ��,*10ms
#define	Button_D_Time		80 //�㶯����ʱ�䣬*10ms
#define	Button_L_Time		200 //��������ʱ��,*10ms

static unsigned int Button_Timer[Button_ListCount];
static void (*DianDong_Fun_Point_List[Button_ListCount])(void);
static void (*ChangAn_Fun_Point_List[Button_ListCount])(void);

//��ȡĳIO��״̬
static unsigned char Button_ReadIO(unsigned char CH)
{
	if(CH == 0)
		return Button_IO_0;
	if(CH == 1)
		return Button_IO_1;
	return 0;
}
//���ݼ�⵽�ĵ㶯���߳�����������  Type DΪ�㶯��LΪ����
static void Button_RunFun(unsigned char CH,unsigned char Type)
{
	if(Type == 'D')
	{
		if(DianDong_Fun_Point_List[CH] == 0)
			return;
		DianDong_Fun_Point_List[CH]();
	}
	if(Type == 'L')
	{
		if(ChangAn_Fun_Point_List[CH] == 0)
			return;
		ChangAn_Fun_Point_List[CH]();
	}
}
//��ʼ����ť��صı���
void Button_Init(void)
{
	unsigned char i=0;
	Button_IO_0 = 1;
	Button_IO_1 = 1;
	for(i=0;i<Button_ListCount;i++)
	{
		DianDong_Fun_Point_List[i] = 0;
		ChangAn_Fun_Point_List[i] = 0;
		Button_Timer[i] = 0;
	}
}

//��ⰴ��ҵ���߼����ŵ�10msѭ���в����ظ�
void Button_Loop(void)
{
	unsigned char i;
	for(i=0;i<Button_ListCount;i++)
	{
		if(Button_ReadIO(i) == 0)
		{
			Button_Timer[i] ++ ;
			if(Button_Timer[i] == 65531)
			{
				Button_Timer[i] =  65530;
				continue;
			}
			if(Button_Timer[i] > Button_L_Time) //5��֮���ǳ���
			{
				Button_Timer[i] =  65530;
				Button_RunFun(i,'L');
				continue;
			}
		}
		else
		{
			if(Button_Timer[i] != 0)
			{
				if(Button_Timer[i] < Button_G_Time) //����
				{
					Button_Timer[i] = 0;
					continue;
				}
				if(Button_Timer[i] < Button_D_Time) //�㶯
				{
					Button_Timer[i] = 0;
					Button_RunFun(i,'D');
					continue;
				}
				if(Button_Timer[i] >= 65530) //�Ѿ������������
				{
					Button_Timer[i] = 0;
					continue;
				}
				if(Button_Timer[i] > Button_L_Time) //����
				{
					Button_Timer[i] = 0;
					Button_RunFun(i,'L');
					continue;
				}
			}
			Button_Timer[i] = 0;
		}
	}
}
//��ĳ��IO����Ӷ̰����߳����Ĵ����� Type DΪ�㶯��CΪ����
void Button_SetFun(unsigned char CH,unsigned char Type,void (*Fun)(void))
{
	if(Type == 'D')
	{
		DianDong_Fun_Point_List[CH] = Fun;
	}
	if(Type == 'C')
	{
		ChangAn_Fun_Point_List[CH] = Fun;
	}
}
//��ȡĳ��IO�ڵ�ǰ״̬
unsigned char Button_GetState(unsigned char CH)
{
	if(CH == 0)
	{
		return ! Button_IO_0;
	}
	
	return 0;
}
//



//

