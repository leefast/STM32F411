#include "arp.h"
#include "ethernet.h"
extern uint8_t Mac[6];
extern uint8_t MyIpAddr[4];
uint8_t PacketIsArp(uint8_t *EthFrame,unsigned  int len)
	{
	uint8_t i=0; 
	if (len<41)
		{
	    return(0);
		}
	if(EthFrame[ETH_TYPE_H_P] != ETHTYPE_ARP_H_V || EthFrame[ETH_TYPE_L_P] != ETHTYPE_ARP_L_V)
		{
	    return(0);
		}
	while(i<4)
		{
	    if(EthFrame[ETH_ARP_DST_IP_P+i] != MyIpAddr[i])
			{
	        return(0);
	    }
	    i++;
		}
	return(1);
}	
void ARP_ProcessDataIn(uint8_t* EthFrame)
{
	uint8_t i=0;
	{
				// Doi dia chi MAC tu nguon thanh dich + dien dia chi MAC hien tai cua minh vao dia chi nguon 
        
				ETH_MakeHeader(EthFrame);
				// Doi ban tin thanh ban tin REPLY
				EthFrame[ETH_ARP_OPCODE_H_P]=ETH_ARP_OPCODE_REPLY_H_V;  
				EthFrame[ETH_ARP_OPCODE_L_P]=ETH_ARP_OPCODE_REPLY_L_V;
				//Dien dia chi MAC dich va MAC nguon cho khung ARP
				for(i=0;i<6;i++)
				{
						EthFrame[ETH_ARP_DST_MAC_P+i]=EthFrame[ETH_ARP_SRC_MAC_P+i];
						EthFrame[ETH_ARP_SRC_MAC_P+i]=Mac[i];
				}
				//Dien dia chi IP dich va IP nguon cho khung ARP
				for(i=0;i<4;i++)
				{
						EthFrame[ETH_ARP_DST_IP_P+i]=EthFrame[ETH_ARP_SRC_IP_P+i];
						EthFrame[ETH_ARP_SRC_IP_P+i]=MyIpAddr[i];
				}
				//Gui di ban tin tra loi
				//14 byte header ETH
				//28 byte header ARP, khong co data 
				ETH_SendFrame(42,EthFrame);
	}
}
