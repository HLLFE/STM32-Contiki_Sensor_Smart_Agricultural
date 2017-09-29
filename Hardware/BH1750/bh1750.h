#ifndef __BH1750_H__
#define __BH1750_H__
#include "stm32f10x.h"
#include "stm32f10x_conf.h"


#define BH1750_Addr 0x46
#define BH1750_ON   0x01
#define BH1750_CON  0x10
#define BH1750_ONE  0x20
#define BH1750_RSET 0x07


void BH1750_Init(void);  //IO��ʼ����
void BH1750_Start(void);			//�ϵ磬����������ݼĴ���
void BH1750_Read(void);                               //�����Ķ�ȡ�ڲ��Ĵ�������
void BH1750_Convert(void);
float BH1750_GetLumen(void);

#endif

