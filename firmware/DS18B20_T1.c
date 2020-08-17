//΢�Ź��ںţ�����������
//��·18B20����
#include<DS18B20_T1.h>
#include<Uart_1.h>
#include<intrins.h>

#define	Debug_SendData	Send_Data1
#define	T_IO	P45		//T1  ע���ӦIO������ǿ���죬��������������û�����

static unsigned char DS18B20_RAM[10];//�ھŸ�Ԫ����DS18B20CRC����10��Ԫ���ǵ�Ƭ�������CRC
//һ΢����ʱ
static void Delay1us(unsigned int a)		//@22.1184MHz
{
	unsigned char i;
	unsigned int b=0;
	for(b=0;b<a;b++)
	{
		i = 3;
		while (--i);
	}
}
//��λ����
static void Reset_IO(void)
{
	unsigned char a;
	T_IO = 0;
	Delay1us(700);//�������ߣ���λ����
	T_IO = 1;
	Delay1us(35);//18B20�ȴ�
	a = T_IO; //��ȡIO��
	Delay1us(160);//18B20�������ߣ���ʾ�Լ�����
	Delay1us(480);//������λ
	//Debug_SendData(a);
}
//����һ���ֽ� ��λ��ǰ
static void Send_Byte_IO(unsigned char Dat)
{
	unsigned char i=0;
	for(i=0;i<8;i++)
	{
		T_IO = 0;
		Delay1us(5);
		T_IO = Dat & 0x01;
		Delay1us(30);//��������30us
		T_IO = 1;
		Delay1us(5);
		Dat >>= 1;
	}
	Delay1us(10);
}
//��ȡһ���ֽ� ��λ��ǰ
static unsigned char Read_Byte_IO(void)
{
	unsigned char i=0;
	unsigned char Dat=0;
	for(i=0;i<8;i++)
	{
		Dat >>= 1;
		T_IO = 0;
		Delay1us(5);//��������5us
		T_IO = 1;
		Delay1us(10);//���ͺ�15us���Ҷ�״̬
		if(T_IO)
			Dat |= 0x80;
		else
			Dat |= 0x00;
		Delay1us(5);
	}
	return Dat;
}
//CRCһ���ֽ�
static unsigned char CRC_One_Byte(unsigned char Dat)
{
	unsigned char i=0,crc_1byte=0;     
  for(i=0;i<8;i++)
  {
		if(((crc_1byte^Dat)&0x01))
    {
			crc_1byte^=0x18;
      crc_1byte>>=1;
      crc_1byte|=0x80;
    }
    else
			crc_1byte>>=1;
    Dat>>=1;
  }
  return crc_1byte;
}
//����һ�����ݵ�CRC���
static unsigned char CRC_Bytes(unsigned char *s,unsigned char Len)
{
	unsigned char CRC=0;
	while(Len--)
	{
		CRC = CRC_One_Byte(CRC^*s++);
	}
	return CRC;
}

//ѭ����ȡ�¶�����
void Read_DS18B20_T1_Loop(void)
{
	unsigned char i=0;
	
	Reset_IO();
	Send_Byte_IO(0XCC);//Skip ROM
	Send_Byte_IO(0XBE);//��ȡ�ڲ��Ĵ���

	for(i=0;i<9;i++)//��ȡ9���ֽ�
	{
		DS18B20_RAM[i] = Read_Byte_IO();
	}
	
	Reset_IO();
	Send_Byte_IO(0XCC);//Skip ROM
	Send_Byte_IO(0X44);//��ת��
	
	DS18B20_RAM[9]= CRC_Bytes(DS18B20_RAM,8);//��Ƭ������CRC
	
}
//�����¶�����
float Read_T1(void)
{
	float FloatTemperature;
	int Temp=0;
	if(DS18B20_RAM[8] == DS18B20_RAM[9])
	{
		Temp = DS18B20_RAM[1];//MSB
		Temp <<= 8;
		Temp += DS18B20_RAM[0];//LSB
		FloatTemperature = Temp;
		FloatTemperature *= 0.0625;
	}
	
	return FloatTemperature;
}




