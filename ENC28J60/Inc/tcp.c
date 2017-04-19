#include "tcp.h"
#include "http.h"
//#include "web.h"
uint16_t TcpHeaderLen;
uint16_t TcpDataLen;
uint8_t wwwport=80;
//uint8_t seqnum[4]={1,2,3,4};
uint32_t seqnum=0x11223344;
uint8_t PacketIsTcp(uint8_t * EthFrame)
{
   if (EthFrame[IP_PROTO_P]==IP_PROTO_TCP_V&&EthFrame[TCP_DST_PORT_H_P]==0&&EthFrame[TCP_DST_PORT_L_P]==wwwport)
      return 1;
   else 
      return 0;
}
uint8_t PacketIsTcpSyn(uint8_t * EthFrame)
{
   if (EthFrame[TCP_FLAGS_P] & TCP_FLAGS_SYN_V)
      return 1;
   else 
      return 0;
}
void TCP_SendSyn(uint8_t *EthFrame)
{
   uint16_t ck;
   ETH_MakeHeader(EthFrame);
   EthFrame[IP_TOTLEN_H_P]=0;
   EthFrame[IP_TOTLEN_L_P]=IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+4;
   IP_MakeHeader(EthFrame);
   EthFrame[TCP_FLAGS_P]=TCP_FLAGS_SYNACK_V;
   TCP_MakeHead(EthFrame,1,1,0);
   // calculate the checksum, len=8 (start from ip.src) + TCP_HEADER_LEN_PLAIN + 4 (one option: mss)
   ck=CheckSum(&EthFrame[IP_SRC_P],8+TCP_HEADER_LEN_PLAIN+4,2);
   EthFrame[TCP_CHECKSUM_H_P]=ck>>8;
   EthFrame[TCP_CHECKSUM_L_P]=ck& 0xff;
   //add 4 for option mss:
   //IP_ProcessDataOut(IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+4+ETH_HEADER_LEN,EthFrame);
   //make_tcp_synack_from_syn does already send the syn,ack
   ETH_SendFrame(IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+4+ETH_HEADER_LEN,EthFrame);
}
void TCP_SendAck( uint8_t *EthFrame)
{
   uint16_t j;
   ETH_MakeHeader(EthFrame);
   //fill the header:
   EthFrame[TCP_FLAGS_P]=TCP_FLAGS_ACK_V;
   if (TcpDataLen==0)
         {
           // if there is no data then we must still acknoledge one packet
           TCP_MakeHead(EthFrame,1,0,1); // no options
         }
   else
         {
           TCP_MakeHead(EthFrame,TcpDataLen,0,1); // no options
         }
   // total length field in the IP header must be set:
   // 20 bytes IP + 20 bytes tcp (when no options) 
   j=IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN;
   EthFrame[IP_TOTLEN_H_P]=j>>8;
   EthFrame[IP_TOTLEN_L_P]=j& 0xff;
   IP_MakeHeader(EthFrame);
   // calculate the checksum, len=8 (start from ip.src) + TCP_HEADER_LEN_PLAIN + data len
   j=CheckSum(&EthFrame[IP_SRC_P], 8+TCP_HEADER_LEN_PLAIN,2);
   EthFrame[TCP_CHECKSUM_H_P]=j>>8;
   EthFrame[TCP_CHECKSUM_L_P]=j& 0xff;
   ETH_SendFrame(IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+4+ETH_HEADER_LEN,EthFrame);
}
void TCP_SendData(uint8_t *EthFrame, uint16_t Len,uint8_t Fin)
{
   uint16_t j;
   if(Fin==1)EthFrame[TCP_FLAGS_P]=TCP_FLAGS_ACK_V|TCP_FLAGS_PUSH_V|TCP_FLAGS_FIN_V;
   else EthFrame[TCP_FLAGS_P]=TCP_FLAGS_ACK_V|TCP_FLAGS_PUSH_V;
   // total length field in the IP header must be set:
   // 20 bytes IP + 20 bytes tcp (when no options) + len of data
   j=IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+Len;
   EthFrame[IP_TOTLEN_H_P]=j>>8;
   EthFrame[IP_TOTLEN_L_P]=j&0xff;
   IP_FillCheckSum(EthFrame);
   // zero the checksum
   EthFrame[TCP_CHECKSUM_H_P]=0;
   EthFrame[TCP_CHECKSUM_L_P]=0;
   // calculate the checksum, len=8 (start from ip.src) + TCP_HEADER_LEN_PLAIN + data len
   j=CheckSum(&EthFrame[IP_SRC_P], 8+TCP_HEADER_LEN_PLAIN+Len,2);
   EthFrame[TCP_CHECKSUM_H_P]=j>>8;
   EthFrame[TCP_CHECKSUM_L_P]=j& 0xff;
   ETH_SendFrame(IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+Len+ETH_HEADER_LEN,EthFrame);
}
void TCP_ProcessDataIn(uint8_t *EthFrame)
{  
    uint16_t dat_p;
    uint16_t len=0;
    dat_p=(EthFrame[TCP_HEADER_LEN_P]>>4)*4+TCP_HEADER_P;
    len=(((uint16_t)EthFrame[IP_TOTLEN_H_P])<<8)|(EthFrame[IP_TOTLEN_L_P]&0xff)-IP_HEADER_LEN-(EthFrame[TCP_HEADER_LEN_P]>>4)*4;
    if(len<=0) 
    {
      len=0;
    }
  if (PacketIsTcpSyn(EthFrame)==1)
            {
                TCP_SendSyn(EthFrame);
                return;
            }
  if (EthFrame[TCP_FLAGS_P] & TCP_FLAGS_ACK_V)
            {
               TCP_GetLenDataHeader(EthFrame); 
               dat_p=TCP_GetDataPointer();
                if (len==0)
								 { 
													if (EthFrame[TCP_FLAGS_P] & TCP_FLAGS_FIN_V)
											 {
													TCP_SendAck(EthFrame); 
													return;
											 } 
													return;
								 }
								TCP_SendAck(EthFrame);                //send ack for http get  
               HTTP_ProcessDataIn(EthFrame,dat_p,TcpDataLen);
               return;
            }
}
void TCP_MakeHead(uint8_t *EthFrame,uint16_t len,uint8_t mss,uint8_t cp_seq)
{
   uint8_t i=0;
   uint8_t tseq;
   while(i<2)
      {
       EthFrame[TCP_DST_PORT_H_P+i]=EthFrame[TCP_SRC_PORT_H_P+i];
       EthFrame[TCP_SRC_PORT_H_P+i]=0; // clear source port
       i++;
      }
   // set source port  (http):
   EthFrame[TCP_SRC_PORT_L_P]=wwwport;
   i=4;
   while(i>0)
      {
       len=EthFrame[TCP_SEQ_H_P+i-1]+len;
       tseq=EthFrame[TCP_SEQACK_H_P+i-1];
       EthFrame[TCP_SEQACK_H_P+i-1]=0xff&len;
       if (cp_seq)
            {
              EthFrame[TCP_SEQ_H_P+i-1]=tseq;
          }
         else
            {
               EthFrame[TCP_SEQ_H_P+i-1]= 0;
          }
       len=len>>8;
       i--;
      }
   if (cp_seq==0)
      {
       // put inital seq number
       EthFrame[TCP_SEQ_H_P+0]= 0;
       EthFrame[TCP_SEQ_H_P+1]= 0;
       EthFrame[TCP_SEQ_H_P+2]= seqnum; 
       EthFrame[TCP_SEQ_H_P+3]= 0;
       seqnum+=2;
      }
   // zero the checksum
   EthFrame[TCP_CHECKSUM_H_P]=0;
   EthFrame[TCP_CHECKSUM_L_P]=0;
   if (mss)
         {
          // the only option we set is MSS to 1408:
          // 1408 in hex is 0x580
          EthFrame[TCP_OPTIONS_P]=2;
          EthFrame[TCP_OPTIONS_P+1]=4;
          EthFrame[TCP_OPTIONS_P+2]=0x05; 
          EthFrame[TCP_OPTIONS_P+3]=0x80;
          // 24 bytes:
          EthFrame[TCP_HEADER_LEN_P]=0x60;
         }
      else
         {
          // no options:
          // 20 bytes:
          EthFrame[TCP_HEADER_LEN_P]=0x50;
         }
}
void TCP_GetLenDataHeader(uint8_t *EthFrame)
{
   uint16_t temp=0;
   temp=EthFrame[IP_TOTLEN_H_P];
   temp=temp<<8;
   temp=temp|(EthFrame[IP_TOTLEN_L_P]&0xff);
   temp=temp-IP_HEADER_LEN;
   TcpHeaderLen=(EthFrame[TCP_HEADER_LEN_P]>>4)*4; 
   TcpDataLen=temp-TcpHeaderLen;
   /* TcpDataLen=(EthFrame[IP_TOTLEN_H_P]<<8)|(EthFrame[IP_TOTLEN_L_P]&0xff);
    TcpDataLen-=IP_HEADER_LEN;
    TcpHeaderLen=(EthFrame[TCP_HEADER_LEN_P]>>4)*4; // generate len in bytes;
    TcpDataLen-=TcpHeaderLen;*/
    if (TcpDataLen<=0)
      {
        TcpDataLen=0;
      }
}
uint16_t TCP_GetDataPointer(void)
{
   if (TcpDataLen)
         {
           return((uint16_t)TCP_SRC_PORT_H_P+TcpHeaderLen);
         }
   else
         {
           return(0);
         }
}
