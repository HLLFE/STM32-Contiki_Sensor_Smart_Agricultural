#include "BH1750.h"
#include "i2c.h"
#include "delay.h"
#include "stdio.h"

float BH1750_result_lx=0;
uint8_t BH1750_Data_Buf[2]={0};
uint16_t BH1750_result=0;

//��ʼ��
void BH1750_Init(void)
{
	I2C_Analog_Port_Init();
}

void BH1750_Cmd_Write(uint8_t cmd)
{
    I2C_Start();                  //��ʼ�ź�
    I2C_Send_Byte(BH1750_Addr+0);   //�����豸��ַ+д�ź�
	while(I2C_Wait_Ack())__NOP;;
    I2C_Send_Byte(cmd);    //�ڲ��Ĵ�����ַ
	while(I2C_Wait_Ack())__NOP;;
    I2C_Stop();                   //����ֹͣ�ź�
	Delay_NOP_ms(5);
}
void BH1750_Start(void)
{
	BH1750_Cmd_Write(BH1750_ON);	 //power on
	BH1750_Cmd_Write(BH1750_RSET);	//clear
	BH1750_Cmd_Write(BH1750_CON);  //һ��H�ֱ���ģʽ������120ms��֮���Զ��ϵ�ģʽ  
}
void BH1750_Read(void)
{   	
    I2C_Start();                          //��ʼ�ź�
    I2C_Send_Byte(BH1750_Addr+1);         //�����豸��ַ+���ź�
	while(I2C_Wait_Ack());
	BH1750_Data_Buf[0]=I2C_Read_Byte(1);  //����ACK
	BH1750_Data_Buf[1]=I2C_Read_Byte(0);  //����NACK

    I2C_Stop();                          //ֹͣ�ź�
    Delay_NOP_ms(5);
}
void BH1750_Convert(void)
{
	BH1750_result=BH1750_Data_Buf[0];
	BH1750_result=(BH1750_result<<8)+BH1750_Data_Buf[1];  //�ϳ����ݣ�����������
	BH1750_result_lx=(float)BH1750_result/1.2;
}

float BH1750_GetLumen(void)
{
    BH1750_Read();
    BH1750_Convert();
    return BH1750_result_lx;
}
