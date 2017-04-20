#include "NEC_Decode.h"
extern TIM_HandleTypeDef htim2;
extern DMA_HandleTypeDef hdma_tim2_ch1;

extern uint8_t type;
extern uint32_t header[2], data[32];
extern uint8_t decode[4];
void NEC_Init(void)
{
	HAL_TIM_IC_Start_DMA(&htim2,TIM_CHANNEL_1,data,2);
}	

void NEC_Processing(void)
{
	HAL_TIM_IC_Stop_DMA(&htim2,TIM_CHANNEL_1);
		//type = 0: Header Receive Interrupt, i=1: Data Receive Interrupt  
	 if(type==0)//type=0
	 {
		
		 //check NEC Frame Type: Data or Repeat   12500: boundary value
		 if(header[1] > 12500)//Data Frame
		 {
			 type=1;
			 HAL_TIM_IC_Start_DMA(&htim2,TIM_CHANNEL_1,data,32);
		 }
		 else                  //Repeat Frame
		 {
			
				/*Check Error : decode[0]: address
												decode[1]: negative address
												decode[2]: data
				                decode[3]: negative data*/
				
			 HAL_TIM_IC_Start_DMA(&htim2,TIM_CHANNEL_1,header,2);
		 }
	 }
	 else    //type=1
	 {
		 type=0;
		  //Decode Function
	         NEC_Decode(decode,data);
		 HAL_TIM_IC_Start_DMA(&htim2,TIM_CHANNEL_1,header,2);
	 }
}
void NEC_Decode(uint8_t *_decode,uint32_t* _data)
{

	 for (int pos = 0; pos < 32; pos++) {
          
            if (_data[pos]>2000) 
						{
                _decode[pos / 8] |= 1 << (pos % 8);      //Bit 1
            } 
							else 
						{
                _decode[pos / 8] &= ~(1 << (pos % 8));   //Bit 0
            }
					}
}
_Bool NEC_Check(uint8_t *_decode)
{
	uint8_t check = ~_decode[3];
	if(_decode[2]!=check)
		return 0;
	else 
		return 1;
}

