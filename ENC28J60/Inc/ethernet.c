#include "ethernet.h"
#include "arp.h"
#include "ip.h"
#include "packet.h"

extern uint16_t len;
extern uint8_t ethBuffer[];
extern uint8_t Mac[6];
void ETH_Init(uint8_t * MacAddr)
{
   ENC_Init(MacAddr);
	 HAL_Delay(20);
}
void ETH_SendFrame(uint16_t len,uint8_t *EthFrame)
{
   ENC_PacketSend(len,EthFrame);
}
uint16_t ETH_GetFrame(uint16_t maxlen, uint8_t *EthFrame)
{
   return ENC_PacketReceive(maxlen,EthFrame);
}
/*void ETH_SetMac(uint8_t * MacAddr)
{
   ENC_Write(MAADR5, MacAddr[0]);   
   ENC_Write(MAADR4, MacAddr[1]);
   ENC_Write(MAADR3, MacAddr[2]);
   ENC_Write(MAADR2, MacAddr[3]);
   ENC_Write(MAADR1, MacAddr[4]);
   ENC_Write(MAADR0, MacAddr[5]);
}*/
void ETH_GetMac(uint8_t *MyMacAddr)
{
   MyMacAddr[0]=ENC_Read(MAADR5);
   MyMacAddr[1]=ENC_Read(MAADR4);
   MyMacAddr[2]=ENC_Read(MAADR3);
   MyMacAddr[3]=ENC_Read(MAADR2);
   MyMacAddr[4]=ENC_Read(MAADR1);
   MyMacAddr[5]=ENC_Read(MAADR0);
}
void ETH_MakeHeader(uint8_t *EthFrame)
{
   uint8_t i=0;
   //Chep dia chi Mac nguon thanh dia chi dich va dien dia chi cua no thanh dia chi nguon
   for(i=0;i<6;i++)
      {
        EthFrame[ETH_DST_MAC +i]=EthFrame[ETH_SRC_MAC +i];
        EthFrame[ETH_SRC_MAC +i]=Mac[i];
      }
}
void ETH_Service( uint16_t len,uint8_t *EthFrame)
{
   //Neu day la goi tin ARP
   if(PacketIsArp(EthFrame,len)==1)
   {
			//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
      ARP_ProcessDataIn(EthFrame);
      return;
   }
   //Neu day la goi tin IP
  else if(PacketIsIp(EthFrame,len)==1)
   {  
      IP_ProcessDataIn(len,EthFrame); 
			return;      
   }
   
}

//--------------------------------------------------------------------------------------
//Ham duoc goi lien tuc de thuc thi cac tac vu cua giao thuc ethernet