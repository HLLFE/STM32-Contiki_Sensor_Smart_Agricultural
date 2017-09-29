#include "dht11.h"
#include "delay.h"

 //////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//DHT11������ʪ�ȴ�������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
      
//��λDHT11
void DHT11_Out()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DHT11_Data_Pin;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_Init(DHT11_Data_Port, &GPIO_InitStructure);				 //��ʼ��IO��
}
void DHT11_IN()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DHT11_Data_Pin;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //�������
 	GPIO_Init(DHT11_Data_Port, &GPIO_InitStructure);				 //��ʼ��IO��
}
void DHT11_Rst(void)	   
{                 
	DHT11_Out(); 	//SET OUTPUT
    DHT11_DQ_OUT=0; 	//����DQ
    Delay_NOP_ms(50);    	//��������18ms
    DHT11_DQ_OUT=1; 	//DQ=1 
	Delay_NOP_us(60);     	//��������20~40us
}
//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
uint8_t DHT11_Check(void) 	   
{   
	uint8_t retry=0;
	DHT11_IN();//SET INPUT	 
    while (DHT11_DQ_IN&&retry<100)//DHT11������40~80us
	{
		retry++;
		Delay_NOP_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		Delay_NOP_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}
//��DHT11��ȡһ��λ
//����ֵ��1/0
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(DHT11_DQ_IN&&retry<100)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		Delay_NOP_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		Delay_NOP_us(1);
	}
	Delay_NOP_us(40);//�ȴ�40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}
//��DHT11��ȡһ���ֽ�
//����ֵ������������
uint8_t DHT11_Read_Byte(void)    
{        
    uint8_t i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}
//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *temp0,uint8_t *humi,uint8_t *humi0)    
{        
 	uint8_t buf[5];
	uint8_t i;
    
    DHT11_Init();
    
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*humi0=buf[1];
			*temp=buf[2];
			*temp0=buf[3];
		}
	}else return 1;
	return 0;	    
}
//��ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
//����1:������
//����0:����    	 
uint8_t DHT11_Init(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(DHT11_Data_RCC_Periph, ENABLE);	 //ʹ��PB�˿�ʱ��
 	GPIO_InitStructure.GPIO_Pin = DHT11_Data_Pin;				 //PB11�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(DHT11_Data_Port, &GPIO_InitStructure);				 //��ʼ��IO��
    
 	GPIO_SetBits(DHT11_Data_Port,DHT11_Data_Pin);						 //PB11 �����	
    return 1;
//	DHT11_Rst();  //��λDHT11
//	return DHT11_Check();//�ȴ�DHT11�Ļ�Ӧ
} 







