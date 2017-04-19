/* Thanh Phat
To use this library, u must init Timer Input Capture DMA
In this Project, i use Timer 2 - Channel 1 - PA0 pin to use input capture FALLING EDGE
Timer tick 1us
First, I Start Input capture DMA to get header - time = duration between 2 falling edge
-----------Header Type---------
if Time > Type_Boundary (frequently 12.5 ms) -> Data Frame
else                                            Repeat Frame
----------Data Frame-----------
    addr--naddr--com--ncom
		bit 1: duration between 2 input capture > 2000us
		bit 0:                                    2000us
*/
#ifndef _NEC_DECODE_H
#define _NEC_DECODE_H
#include "stm32f4xx_hal.h"
//Start listening 
void NEC_Init(void);
//NEC Process
void NEC_Processing(void);
//return decode[4] from data[32]
void NEC_Decode(uint8_t *_decode,uint32_t* _data);
//check errot
_Bool NEC_Check(uint8_t *_decode);

#endif
