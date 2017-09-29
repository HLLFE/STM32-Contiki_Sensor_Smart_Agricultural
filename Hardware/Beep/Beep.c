/**
********************************************************

  ******************************************************************************
  */
  
#include "Beep.h"   

uint16_t beep_flag = 0;


 /**
  * @brief  ��ʼ��GPIO
  * @param  ��
  * @retval ��
  */

void Beep_GPIO_Config(void)
{	
    GPIO_InitTypeDef GPIO_InitStructure;/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/

    RCC_APB2PeriphClockCmd(Beep_RCC_Periph, ENABLE ); /*����GPIOA������ʱ�� ����GPIOD������ʱ��*/

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;/*��������ģʽΪͨ���������*/   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; /*������������Ϊ50MHz */  	
    
    GPIO_InitStructure.GPIO_Pin = Beep_Pin;/*ѡ��Ҫ���Ƶ�GPIO����*/	
    GPIO_Init(Beep_Port, &GPIO_InitStructure );/*���ÿ⺯������ʼ��GPIO*/

    
    Beep_Off();
}

void Beep_On(void)
{
    GPIO_ResetBits(Beep_Port, Beep_Pin);
}

void Beep_Off(void)
{
    GPIO_SetBits(Beep_Port, Beep_Pin);
}

void SetBeepAlertSource(uint8_t alertSource)
{
    beep_flag |= 0x0001 << alertSource;
}

void ResetBeepAlertSource(uint8_t alertSource)
{
    beep_flag &= ~(uint16_t)(0x0001 << alertSource);
}

/*********************************************END OF FILE**********************/
