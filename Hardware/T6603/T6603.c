#include "T6603.h"
#include "stdio.h"


const uint8_t T6603_Receive_HeadData[T6603_RECEIVE_PACKET_HEAD_LENGTH] = {0xFF,0xFA,0x02};
uint8_t T6603_Read_GAS_PPM_CMD[] = {0xFF,0xFE,0x02,0x02,0x03};

Uint8FIFOQueue T6603_ReceiveBytesFIFO_Queue;
Uint8FIFOQueue* T6603_ReceiveBytesFIFO_QueueHandle = &T6603_ReceiveBytesFIFO_Queue;
T6603_PacketNode T6603_GlobalPacket;

void T6603_Init(void)
{
	UART5_Config(19200);
}
void T6603_SendReadCMD(void)
{
    T6603_USART_SendBytesBuf(T6603_Read_GAS_PPM_CMD,sizeof(T6603_Read_GAS_PPM_CMD));
}

/*���ڷ�װ���ṩ����push������FIFO�Ľӿ�
*
*/
void T6603_PushReceiveByteIntoFIFO(uint8_t streamByteData)//���ڷ�װ���ṩ����push��FIFO�Ľӿ�
{
    Uint8FIFOPush(T6603_ReceiveBytesFIFO_QueueHandle, streamByteData);
}


/*��ֱ����FIFO�н���ɿ鲢��ӵ����հ�����
*
*
*/
void T6603_LoadQueueByteToPacketBlock(Uint8FIFOQueue* uint8FIFOQueueHandle)
{
    static bool isHeadAllEqual = false;
    T6603_PacketNode* T6603_packet_node;
    T6603_packet_node = &T6603_GlobalPacket;
    while(true)
    {
        if(!isHeadAllEqual)
        {
            if(Uint8FIFOGetQueueSize(uint8FIFOQueueHandle) < T6603_PROTOCOL_PACKET_LENGTH) return;
            while(true)//�˴�Ϊ����֡ͷ
            {
				uint8_t count;
                if(Uint8FIFOGetQueueSize(uint8FIFOQueueHandle) <= 0) return;    //���Ȳ���ʱ�˳�                     
                for(count=0;count<sizeof(T6603_Receive_HeadData)-1;count++)          //˳����λ  ���յ����ֽ�
                {
                    T6603_packet_node->packetHead[count] = T6603_packet_node->packetHead[count+1];
                }
                T6603_packet_node->packetHead[sizeof(T6603_Receive_HeadData)-1] = Uint8FIFOPop(uint8FIFOQueueHandle);//��ȡ����ֵ
          
                for(count=0,isHeadAllEqual=true;count<sizeof(T6603_Receive_HeadData);count++)      //�Ƚ��Ƿ����
                {
                    if(T6603_packet_node->packetHead[count] != T6603_Receive_HeadData[count]){isHeadAllEqual=false;break;}
                }
                if(isHeadAllEqual)break;
            }
        }
        if(Uint8FIFOGetQueueSize(uint8FIFOQueueHandle) < T6603_PROTOCOL_PACKET_LENGTH - sizeof(T6603_Receive_HeadData)) return;
        T6603_packet_node->CO2_PPM_ByteHigh = Uint8FIFOPop(uint8FIFOQueueHandle);
        T6603_packet_node->CO2_PPM_ByteLow = Uint8FIFOPop(uint8FIFOQueueHandle);
        isHeadAllEqual = false;
        T6603_packet_node->CO2_PPM_Value = (((uint16_t)T6603_packet_node->CO2_PPM_ByteHigh) << 8) + T6603_packet_node->CO2_PPM_ByteLow;
    }
}


/*���ڷ�װ���ṩ�����ȡ���ؽ���FIFO���еĽӿ�
*
*
*/
void T6603_LoadReceiveQueueByteToPacketBlock()
{
    T6603_LoadQueueByteToPacketBlock(T6603_ReceiveBytesFIFO_QueueHandle);
}

uint16_t T6603_getPacketNodeCO2_Value(T6603_PacketNode* T6603_packet_node)
{
    return T6603_packet_node->CO2_PPM_Value;
}
/*���ڷ�װ���ṩ�����ȡPMֵ�Ľӿ�
*
*
*/
uint16_t T6603_getCO2_PPM()
{
    return T6603_getPacketNodeCO2_Value(&T6603_GlobalPacket);
}








