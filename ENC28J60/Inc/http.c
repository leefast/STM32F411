#include "http.h"
#include "web.h"
#include "string.h"
uint16_t find_string(uint8_t *str1,uint8_t *str2,uint16_t len)
{
    {
    uint16_t i=0,j=0;
    while(i<len)
    {
        if(str2[i++] != str1[j++])
        {
            j = 0;
        }
        if(str1[j] == 0)
        {
            return(i-j);
        }
        if(str2[i] == 0){
            return(len);
      }
   }
   return(len);
   }
}
void HTTP_SendData(uint8_t *EthFrame,const uint8_t* array)
{
    uint16_t len=0; 
    while(array[len])
    {
        EthFrame[TCP_DATA_P+len]=array[len];
        len++;
    }
    /*while(len<size)
    {
        EthFrame[TCP_DATA_P+len]=web[len];
        #ifdef HTTP_VIEW
        putchar(web[len]);  
        #endif
        len++;  
    }*/
    TCP_SendData(EthFrame,len,1); 
  //  printf("\r\n");
}
//data point: vi tri bat dau mang du lieu http, cmd vi tri chuoi trong mang
void HTTP_ProcessDataIn(uint8_t *EthFrame,uint16_t data_point, uint16_t len)
{
            
						uint16_t cmd=0;  
						
            uint8_t str[20];//={"LED1"};
						
						//LED1
						sprintf(str,"LED1");
            cmd=find_string(str,&EthFrame[data_point],len);
            if(cmd!=len)
            {
               if(EthFrame[data_point+cmd+5]=='1')
                  {
                       LED_LAM_ON;
                  }
               if(EthFrame[data_point+cmd+5]=='0')
                  {
                      LED_LAM_OFF;
                  }
            } 
						//LED2
            sprintf(str,"LED2");
				
            cmd=find_string(str,&EthFrame[data_point],len);
            if(cmd!=len)
            {
               if(EthFrame[data_point+cmd+5]=='1')LED_LUC_ON;
               if(EthFrame[data_point+cmd+5]=='0')LED_LUC_OFF;
            }
						//LED3
            sprintf(str,"LED3=");
            cmd=find_string(str,&EthFrame[data_point],len);
            if(cmd!=len)
            {
               if(EthFrame[data_point+cmd+5]=='1')LED_CAM_ON;
               if(EthFrame[data_point+cmd+5]=='0')LED_CAM_OFF;
            }
						//GET
						sprintf(str,"GET");
						 cmd=find_string(str,&EthFrame[data_point],len);
            if(cmd!=len)
						{
							
								HTTP_SendData(EthFrame,web);
						}
						//xu li dang nhap username	
						//sprintf(str,"txtname");
//						cmd=find_string(str,&EthFrame[data_point],len);
//						uint8_t verify=0;
//						if(cmd!=len)
//						{	
//							for (uint8_t i=0;i<sizeof(username);i++)
//							{
//								
//									if(EthFrame[data_point+cmd+8+i]==username[i]) verify++;
//							}
//						}
					//	if (verify==1	)	{HTTP_SendData(EthFrame,web);verify=0;}
						//HTTP_SendData(EthFrame,http_pageheader_unauth);
						//HTTP_SendData(EthFrame,formlogin);
						
	}
