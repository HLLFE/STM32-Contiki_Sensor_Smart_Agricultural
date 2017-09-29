#include "MQ-2.h"
#include "delay.h"
	   
void MQ02_Init()
{
    MQ02_GPIO_Config();
    ADC1_GPIO_Init();
}

void MQ02_GPIO_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(MQ02_AO_RCC_Periph, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 
	//PA1 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = MQ02_AO_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(MQ02_AO_GPIO_Port, &GPIO_InitStructure);	
}

uint16_t MQ02_Read_Value()
{
    uint16_t ADC_Value = 0;
    ADC_Value = Get_ADC1_Average(MQ02_ADC_CHANNEL,10);
    return ADC_Value;
}

float MQ02_Get_Percentage()
{
    return MQ02_Read_Value()/4095.0*100;
}









