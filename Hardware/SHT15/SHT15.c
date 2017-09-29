#include "SHT15.h"
#include <math.h>

/*************************************************************
  Function   ��SHT15_Dly  
  Description��SHT15ʱ����Ҫ����ʱ
  Input      : none        
  return     : none    
*************************************************************/
void SHT15_Dly(void)
{
    uint16_t i;
    for(i = 500; i > 0; i--);
}


/*************************************************************
  Function   ��SHT15_Config  
  Description����ʼ�� SHT15����
  Input      : none        
  return     : none    
*************************************************************/
void SHT15_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;        
    //��ʼ��SHT15����ʱ��
    RCC_APB2PeriphClockCmd(SHT15_RCC_Periph ,ENABLE);
            
    //DATA �������        
    GPIO_InitStructure.GPIO_Pin = SHT15_DATA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SHT15_DATA_PORT, &GPIO_InitStructure);
    //SCK �������
    GPIO_InitStructure.GPIO_Pin = SHT15_SCK_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SHT15_SCK_PORT, &GPIO_InitStructure);

    SHT15_ConReset();        //��λͨѶ
}


/*************************************************************
  Function   ��SHT15_DATAOut
  Description������DATA����Ϊ���
  Input      : none        
  return     : none    
*************************************************************/
void SHT15_DATAOut(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //DATA �������        
    GPIO_InitStructure.GPIO_Pin = SHT15_DATA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         
    GPIO_Init(SHT15_DATA_PORT, &GPIO_InitStructure);
}


/*************************************************************
  Function   ��SHT15_DATAIn  
  Description������DATA����Ϊ����
  Input      : none        
  return     : none    
*************************************************************/
void SHT15_DATAIn(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //DATA ��������        
    GPIO_InitStructure.GPIO_Pin = SHT15_DATA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(SHT15_DATA_PORT, &GPIO_InitStructure);
}


/*************************************************************
  Function   ��SHT15_WriteByte  
  Description��д1�ֽ�
  Input      : value:Ҫд����ֽ�        
  return     : err: 0-��ȷ  1-����    
*************************************************************/
uint8_t SHT15_WriteByte(uint8_t value)
{
    uint8_t i, err = 0;
    
    SHT15_DATAOut();                //����DATA������Ϊ���

    for(i = 0x80; i > 0; i /= 2)    //д1���ֽ�
    {
        if(i & value)
                SHT15_DATA_H();
        else
                SHT15_DATA_L();
        SHT15_Dly();
        SHT15_SCK_H();
        SHT15_Dly();
        SHT15_SCK_L();
        SHT15_Dly();
    }
    SHT15_DATAIn();                 //����DATA������Ϊ����,�ͷ�DATA��
    SHT15_SCK_H();
    err = SHT15_DATA_R();           //��ȡSHT15��Ӧ��λ
    SHT15_SCK_L();

    return err;
}

/*************************************************************
  Function   ��SHT15_ReadByte  
  Description����1�ֽ�����
  Input      : Ack: 0-��Ӧ��  1-Ӧ��        
  return     : err: 0-��ȷ 1-����    
*************************************************************/
uint8_t SHT15_ReadByte(uint8_t Ack)
{
    uint8_t i, val = 0;

    SHT15_DATAIn();                //����DATA������Ϊ����
    for(i = 0x80; i > 0; i /= 2)   //��ȡ1�ֽڵ�����
    {
        SHT15_Dly();
        SHT15_SCK_H();
        SHT15_Dly();
        if(SHT15_DATA_R())
        {
           val = (val | i);
        }
        SHT15_SCK_L();
    }
    SHT15_DATAOut();               //����DATA������Ϊ���
    if(Ack)
            SHT15_DATA_L();        //Ӧ��������ȥ������ȥ������(У������)
    else
            SHT15_DATA_H();        //��Ӧ���������˽���
    SHT15_Dly();
    SHT15_SCK_H();
    SHT15_Dly();
    SHT15_SCK_L();
    SHT15_Dly();

    return val;                    //���ض�����ֵ
}


/*************************************************************
  Function   ��SHT15_TransStart  
  Description����ʼ�����źţ�ʱ�����£�
                     _____         ________
               DATA:      |_______|
                         ___     ___
               SCK : ___|   |___|   |______        
  Input      : none        
  return     : none    
*************************************************************/
void SHT15_TransStart(void)
{
    SHT15_DATAOut();               //����DATA������Ϊ���

    SHT15_DATA_H();
    SHT15_SCK_L();
    SHT15_Dly();
    SHT15_SCK_H();
    SHT15_Dly();
    SHT15_DATA_L();
    SHT15_Dly();
    SHT15_SCK_L();
    SHT15_Dly();
    SHT15_SCK_H();
    SHT15_Dly();
    SHT15_DATA_H();
    SHT15_Dly();
    SHT15_SCK_L();
}


/*************************************************************
  Function   ��SHT15_ConReset  
  Description��ͨѶ��λ��ʱ�����£�
                     _____________________________________________________         ________
               DATA:                                                      |_______|
                        _    _    _    _    _    _    _    _    _        ___     ___
               SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______
  Input      : none        
  return     : none    
*************************************************************/
void SHT15_ConReset(void)
{
    uint8_t i;

    SHT15_DATAOut();

    SHT15_DATA_H();
    SHT15_SCK_L();

    for(i = 0; i < 9; i++)              //����SCKʱ��9c��
    {
        SHT15_SCK_H();
        SHT15_Dly();
        SHT15_SCK_L();
        SHT15_Dly();
    }
    SHT15_TransStart();                 //��������
}



/*************************************************************
  Function   ��SHT15_SoftReset  
  Description����λ
  Input      : none        
  return     : err: 0-��ȷ  1-����    
*************************************************************/
uint8_t SHT15_SoftReset(void)
{
    uint8_t err = 0;

    SHT15_ConReset();                   //ͨѶ��λ
    err += SHT15_WriteByte(SOFTRESET);  //дRESET��λ����

    return err;
}


/*************************************************************
  Function   ��SHT15_ReadStatusReg  
  Description����״̬�Ĵ���
  Input      : p_value-���������ݣ�p_checksun-������У������       
  return     : err: 0-��ȷ  0-����    
*************************************************************/
uint8_t SHT15_ReadStatusReg(uint8_t *p_value, uint8_t *p_checksum)
{
    uint8_t err = 0;

    SHT15_TransStart();                                        //��ʼ����
    err = SHT15_WriteByte(STATUS_REG_R);//дSTATUS_REG_R��ȡ״̬�Ĵ�������
    *p_value = SHT15_ReadByte(ACK);                //��ȡ״̬����
    *p_checksum = SHT15_ReadByte(noACK);//��ȡ���������
    
    return err;
}



/*************************************************************
  Function   ��SHT15_WriteStatusReg  
  Description��д״̬�Ĵ���
  Input      : p_value-Ҫд�������ֵ       
  return     : err: 0-��ȷ  1-����    
*************************************************************/
uint8_t SHT15_WriteStatusReg(uint8_t *p_value)
{
        uint8_t err = 0;

        SHT15_TransStart();                     //��ʼ����
        err += SHT15_WriteByte(STATUS_REG_W);   //дSTATUS_REG_Wд״̬�Ĵ�������
        err += SHT15_WriteByte(*p_value);       //д������ֵ

        return err;
}



/*************************************************************
  Function   ��SHT15_Measure  
  Description������ʪ�ȴ�������ȡ��ʪ��
  Input      : p_value-������ֵ��p_checksum-������У����        
  return     : err: 0-��ȷ 1������    
*************************************************************/
uint8_t SHT15_Measure(uint16_t *p_value, uint8_t *p_checksum, uint8_t mode)
{
    uint8_t err = 0;
    uint32_t i;
    uint8_t value_H = 0;
    uint8_t value_L = 0;

    SHT15_TransStart();                                     //��ʼ����
    switch(mode)                                                         
    {
    case TEMP:                                              //�����¶�
        err += SHT15_WriteByte(MEASURE_TEMP);           //дMEASURE_TEMP�����¶�����
        break;
    case HUMI:
        err += SHT15_WriteByte(MEASURE_HUMI);           //дMEASURE_HUMI����ʪ������
        break;
    default:
        break;
    }
    if(err != 0)
    {
        return err;
    }
    SHT15_DATAIn();
    for(i = 0; i < 1200000; i++)                           //�ȴ�DATA�źű�����
    {
        if(SHT15_DATA_R() == 0) break;                  //��⵽DATA�������ˣ�����ѭ��
    }
    if(SHT15_DATA_R() == 1)                                //����ȴ���ʱ��
    {
        err += 1;
        return err;
    }
    value_H = SHT15_ReadByte(ACK);
    value_L = SHT15_ReadByte(ACK);
    *p_checksum = SHT15_ReadByte(noACK);                    //��ȡУ������
    *p_value = (value_H << 8) | value_L;
    return err;
}


/*************************************************************
  Function   ��SHT15_Calculate  
  Description��������ʪ�ȵ�ֵ
  Input      : Temp-�Ӵ������������¶�ֵ��Humi-�Ӵ�����������ʪ��ֵ
               p_humidity-�������ʵ�ʵ�ʪ��ֵ��p_temperature-�������ʵ���¶�ֵ        
  return     : none    
*************************************************************/
void SHT15_Calculate(uint16_t t, uint16_t rh, float *p_temperature, float *p_humidity)
{
    //�����������ֲ�����
    const float d1 = -39.55;
    const float d2 = +0.01;
    const float C1 = -4;
    const float C2 = +0.0405;
    const float C3 = -0.0000028;        
    const float T1 = +0.01;
    const float T2 = +0.00008;

    float RH_Lin;                                                     //RH����ֵ        
    float RH_Ture;                                                    //RH��ʵֵ
    float temp_C;

    temp_C = d1 + d2 * t;                                            //�����¶�ֵ        
    RH_Lin = C1 + C2 * rh + C3 * rh * rh;                            //����ʪ��ֵ
    RH_Ture = (temp_C -25) * (T1 + T2 * rh) + RH_Lin;                //ʪ�ȵ��¶Ȳ���������ʵ�ʵ�ʪ��ֵ
    RH_Ture = (RH_Ture > 100) ? 100 : RH_Ture;
    RH_Ture = (RH_Ture < 0.1) ? 0.1 : RH_Ture;                       //����ʪ��ֵ����

    *p_humidity = RH_Ture;
    *p_temperature = temp_C;

}


/*************************************************************
  Function   ��SHT15_CalcuDewPoint  
  Description������¶��
  Input      : h-ʵ�ʵ�ʪ�ȣ�t-ʵ�ʵ��¶�        
  return     : dew_point-¶��    
*************************************************************/
float SHT15_CalcuDewPoint(float t, float h)
{
    float logEx, dew_point;

    logEx = 0.66077 + 7.5 * t / (237.3 + t) + (log10(h) - 2);
    dew_point = ((0.66077 - logEx) * 237.3) / (logEx - 8.16077);

    return dew_point; 
}


//main�����ı�д��
//int main(void)
//{  
//        uint16_t humi_val, temp_val;
//        uint8_t err = 0, checksum = 0;
//        float humi_val_real = 0.0; 
//        float temp_val_real = 0.0;
//        float dew_point = 0.0;
//        
//        BSP_Init();
//        printf("\nSHT15���ҶȲ��Գ���!!!\n");
//        SHT15_Config();
//        while(1)
//        {
//                err += SHT15_Measure(&temp_val, &checksum, TEMP);                  //��ȡ�¶Ȳ���ֵ
//                err += SHT15_Measure(&humi_val, &checksum, HUMI);                  //��ȡʪ�Ȳ���ֵ
//                if(err != 0)
//                        SHT15_ConReset();
//                else
//                {
//                        SHT15_Calculate(temp_val, humi_val, &temp_val_real, &humi_val_real); //����ʵ�ʵ���ʪ��ֵ
//                        dew_point = SHT15_CalcuDewPoint(temp_val_real, humi_val_real);                 //����¶���¶�
//                } 
//                printf("��ǰ�����¶�Ϊ:%2.1f�棬ʪ��Ϊ:%2.1f%%��¶���¶�Ϊ%2.1f��\r\n", temp_val_real, humi_val_real, dew_point);
//                LED1_Toggle();
//                Delay_ms(1000);
//        }
//}





