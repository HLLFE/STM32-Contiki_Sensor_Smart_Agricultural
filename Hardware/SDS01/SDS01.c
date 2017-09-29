#include "SDS01.h"
#include "stdio.h"

const uint8_t SDS01_Protocol_HeadData = 0xAA;
const uint8_t SDS01_Protocol_EnddingData = 0xAB;
Uint8FIFOQueue SDS01_ReceiveBytesFIFO_Queue;
Uint8FIFOQueue* SDS01_ReceiveBytesFIFO_QueueHandle = &SDS01_ReceiveBytesFIFO_Queue;
SDS01_PacketNode SDS01_GlobalPacket;

void SDS01_Init(void)
{
	UART4_Config(9600);
}

/*���ڷ�װ���ṩ����push������FIFO�Ľӿ�
*
*/
void SDS01_PushReceiveByteIntoFIFO(uint8_t streamByteData)//���ڷ�װ���ṩ����push��FIFO�Ľӿ�
{
    Uint8FIFOPush(SDS01_ReceiveBytesFIFO_QueueHandle, streamByteData);
}

uint8_t SDS01_CalculatePacketCheckSum(SDS01_PacketNode* SDS01_packet_node)
{
    uint8_t checkSum = 0;
    checkSum += SDS01_packet_node->PM2_5_ByteLow;
    checkSum += SDS01_packet_node->PM2_5_ByteHigh;
    checkSum += SDS01_packet_node->PM10_ByteLow;
    checkSum += SDS01_packet_node->PM10_ByteHigh;
    checkSum += SDS01_packet_node->ID_ByteLow;
    checkSum += SDS01_packet_node->ID_ByteHigh;
    return checkSum;
}

/*��ֱ����FIFO�н���ɿ鲢��ӵ����հ�����
*
*
*/
void SDS01_LoadQueueByteToPacketBlock(Uint8FIFOQueue* uint8FIFOQueueHandle)
{
    static bool isHeadAllEqual = false;
    SDS01_PacketNode* SDS01_packet_node;
    SDS01_packet_node = &SDS01_GlobalPacket;
    while(true)
    {
        if(!isHeadAllEqual)
        {
            if(Uint8FIFOGetQueueSize(uint8FIFOQueueHandle) < SDS01_PROTOCOL_PACKET_LENGTH) return;
            while(true)//�˴�Ϊ����֡ͷ
            {
                if(Uint8FIFOGetQueueSize(uint8FIFOQueueHandle) <= 0) return;    //���Ȳ���ʱ�˳�     
                SDS01_packet_node->packetHead = Uint8FIFOPop(uint8FIFOQueueHandle);//��ȡ����ֵ
                isHeadAllEqual=true;
                if(SDS01_packet_node->packetHead != SDS01_Protocol_HeadData)isHeadAllEqual=false;
                if(isHeadAllEqual)break;
            }
        }
        if(Uint8FIFOGetQueueSize(uint8FIFOQueueHandle) < SDS01_PROTOCOL_PACKET_LENGTH - sizeof(SDS01_Protocol_HeadData)) return;
        SDS01_packet_node->signalType = Uint8FIFOPop(uint8FIFOQueueHandle);
        SDS01_packet_node->PM2_5_ByteLow = Uint8FIFOPop(uint8FIFOQueueHandle);
        SDS01_packet_node->PM2_5_ByteHigh = Uint8FIFOPop(uint8FIFOQueueHandle);
        SDS01_packet_node->PM10_ByteLow = Uint8FIFOPop(uint8FIFOQueueHandle);
        SDS01_packet_node->PM10_ByteHigh = Uint8FIFOPop(uint8FIFOQueueHandle);
        SDS01_packet_node->ID_ByteLow = Uint8FIFOPop(uint8FIFOQueueHandle);
        SDS01_packet_node->ID_ByteHigh = Uint8FIFOPop(uint8FIFOQueueHandle);
        SDS01_packet_node->checkSum = Uint8FIFOPop(uint8FIFOQueueHandle);
        SDS01_packet_node->packetEndding = Uint8FIFOPop(uint8FIFOQueueHandle);
        isHeadAllEqual = false;
        if((SDS01_packet_node->checkSum != SDS01_CalculatePacketCheckSum(SDS01_packet_node)) || (SDS01_packet_node->packetEndding != SDS01_Protocol_EnddingData))
        {
            printf("Bad SDS01 packet check\r\n");
            continue;
        }
        SDS01_packet_node->PM2_5_Value = ((((uint16_t)SDS01_packet_node->PM2_5_ByteHigh) << 8) + SDS01_packet_node->PM2_5_ByteLow) / 10.0;
        SDS01_packet_node->PM10_Value = ((((uint16_t)SDS01_packet_node->PM10_ByteHigh) << 8) + SDS01_packet_node->PM10_ByteLow) / 10.0;
    }
}


/*���ڷ�װ���ṩ�����ȡ���ؽ���FIFO���еĽӿ�
*
*
*/
void SDS01_LoadReceiveQueueByteToPacketBlock()
{
    SDS01_LoadQueueByteToPacketBlock(SDS01_ReceiveBytesFIFO_QueueHandle);
}

float SDS01_getPacketNodePM2_5_Value(SDS01_PacketNode* SDS01_packet_node)
{
    return SDS01_packet_node->PM2_5_Value;
}
float SDS01_getPacketNodePM10_Value(SDS01_PacketNode* SDS01_packet_node)
{
    return SDS01_packet_node->PM10_Value;
}
/*���ڷ�װ���ṩ�����ȡPMֵ�Ľӿ�
*
*
*/
float SDS01_getPM2_5_Value()
{
    return SDS01_getPacketNodePM2_5_Value(&SDS01_GlobalPacket);
}

/*���ڷ�װ���ṩ�����ȡPMֵ�Ľӿ�
*
*
*/
float SDS01_getPM10_Value()
{
    return SDS01_getPacketNodePM10_Value(&SDS01_GlobalPacket);
}








