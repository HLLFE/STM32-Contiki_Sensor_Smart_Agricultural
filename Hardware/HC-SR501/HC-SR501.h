#ifndef __SR501_H
#define __SR501_H
#include "stm32f10x.h"
#include "stdbool.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
#define HCSR501_GPIO_Port GPIOC
#define HCSR501_GPIO_Pin  GPIO_Pin_1
#define HCSR501_GPIO_RCC_Periph RCC_APB2Periph_GPIOC

#define HCSR501_Delay_us    Delay_NOP_us

void HCSR501_Init(void);//��ʼ��
bool HCSR501_Read_Data(void);

		 				    
#endif
