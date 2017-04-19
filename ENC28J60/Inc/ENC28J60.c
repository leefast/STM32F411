#include "ENC28J60.h"

extern SPI_HandleTypeDef hspi1;
static uint8_t ENC_Bank;
static uint16_t NextPacketPtr;
uint8_t dataWatch8;
uint16_t dataWatch16;
void ENC_Reset(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);
}
uint8_t ENC_ReadOp(uint8_t oper, uint8_t addr)
{
	uint8_t spiData[2];
	EN_ENC;
	spiData[0] = (oper| (addr & ADDR_MASK));
	HAL_SPI_Transmit(&hspi1, spiData, 1, 100);
	if(addr & 0x80)
	{
		//HAL_SPI_Transmit(&hspi1, spiData, 1, 100);
		HAL_SPI_Receive(&hspi1, &spiData[1], 1, 100);
	}
	HAL_SPI_Receive(&hspi1, &spiData[1], 1, 100);
	DIS_ENC;
	
	return spiData[1];
}
void ENC_WriteOp(uint8_t oper, uint8_t addr, uint8_t data)
{
	uint8_t spiData[2];
	EN_ENC;
	spiData[0] = (oper| (addr & ADDR_MASK)); //((oper<<5)&0xE0)|(addr & ADDR_MASK);
	spiData[1] = data;
	HAL_SPI_Transmit(&hspi1, spiData, 2, 100);
	DIS_ENC;
}
uint8_t ENC_Read(uint8_t addr)
{
	ENC_SetBank(addr);
	return ENC_ReadOp(ENC_Read_CTRL_REG, addr);
}

void ENC_Write(uint8_t addr, uint8_t data)
{
	ENC_SetBank(addr);
	ENC_WriteOp(ENC_Write_CTRL_REG, addr, data);
}

uint16_t ENC_ReadWord( uint8_t addr)
{
	return ENC_Read(addr) + (ENC_Read(addr+1) << 8);
}
void ENC_WriteWord(uint8_t addrL, uint16_t data)
{
	ENC_Write(addrL, data);
	ENC_Write(addrL+1, data >> 8);
}
void ENC_SetMac(uint8_t *macaddr)
   {
   ENC_Write(MAADR5, macaddr[0]);   
   ENC_Write(MAADR4, macaddr[1]);
   ENC_Write(MAADR3, macaddr[2]);
   ENC_Write(MAADR2, macaddr[3]);
   ENC_Write(MAADR1, macaddr[4]);
   ENC_Write(MAADR0, macaddr[5]);
   }

void ENC_SetBank(uint8_t addr)
{
	if ((addr & BANK_MASK) != ENC_Bank) 
	{
		ENC_WriteOp(ENC_BIT_FIELD_CLR, ECON1, ECON1_BSEL1|ECON1_BSEL0);
		ENC_Bank = addr & BANK_MASK;
    ENC_WriteOp(ENC_BIT_FIELD_SET, ECON1, ENC_Bank>>5);
	}
}

void ENC_WritePhy(uint8_t addr, uint16_t data)
{
	ENC_Write(MIREGADR, addr);
	ENC_WriteWord(MIWRL, data);
	while (ENC_Read(MISTAT) & MISTAT_BUSY);
}

uint16_t ENC_ReadPhy(uint8_t addr)
{
	ENC_Write(MIREGADR, addr);							// pass the PHY address to the MII
	ENC_Write(MICMD, MICMD_MIIRD);					// Enable Read bit
	while (ENC_Read(MISTAT) & MISTAT_BUSY);	// Poll for end of reading
	ENC_Write(MICMD, 0x00);									// Disable MII Read
	return ENC_Read(MIRDL) + (ENC_Read(MIRDL+1) << 8);
}

void ENC_Init(uint8_t *mac )
{
	ENC_Reset();
	uint8_t init=0;
	HAL_SPI_Transmit(&hspi1,&init,1,100);
	HAL_SPI_Receive(&hspi1,&init,1,100);
	// (1): Disable the chip CS pin
	EN_ENC;
	HAL_Delay(1);
	// (2): Perform soft reset to the ENCJ60 module
	ENC_WriteOp(ENC_SOFT_RESET, 0, ENC_SOFT_RESET);
	NextPacketPtr = RXSTART_INIT;
	HAL_Delay(2);
	// (3): Wait untill Clock is ready
	while(!ENC_ReadOp(ENC_Read_CTRL_REG, ESTAT) & ESTAT_CLKRDY);
	// (4): Initialise RX and TX buffer size
	ENC_WriteWord(ERXST, RXSTART_INIT);
	ENC_WriteWord(ERXND, RXSTOP_INIT);

	ENC_WriteWord(ETXST, TXSTART_INIT);
  ENC_WriteWord(ETXND, TXSTOP_INIT);
	
	ENC_WriteWord(ERXRDPT, RXSTART_INIT);
	ENC_WriteWord(ERXWRPT, RXSTART_INIT);
	
	dataWatch16 = ENC_ReadWord(ERXND);
	
	// (5): Receieve buffer filters
	ENC_Write(ERXFCON, ERXFCON_UCEN|ERXFCON_ANDOR|ERXFCON_CRCEN);
	 ENC_Write(ERXFCON, ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_PMEN);
   ENC_Write(EPMM0, 0x3f);
   ENC_Write(EPMM1, 0x30);
   ENC_Write(EPMCSL,0xf9);
	 ENC_Write(EPMCSH,0xf7);
	//dataWatch8 = ENC_Read(ERXFCON);
	// (6): MAC Control Register 1
	//ENC_Write(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS|MACON1_PASSALL);
	ENC_Write(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
  ENC_Write(MACON2, 0x00);
	//dataWatch8 = ENC_Read(ERXFCON);
	// (7): MAC Control Register 3
	ENC_WriteOp(ENC_BIT_FIELD_SET, MACON3,MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN);
	//	ENC_WriteOp(ENC_BIT_FIELD_SET, MACON3, MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN|MACON3_FULDPX);
  // (8): NON/Back to back gap
	// (9): Set Maximum framelenght
	ENC_WriteWord(MAMXFLL, MAX_FRAMELEN);	// Set Maximum frame length (any packet bigger will be discarded)
  ENC_Write(MABBIPG, 0x12);
	ENC_WriteWord(MAIPGL, 0x0C12);  // NonBackToBack gap
	// (10): Set the MAC address of the device
	ENC_SetMac(mac);
	dataWatch8 = ENC_Read(MAADR0);
	
	if(dataWatch8==mac[5]) HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);;
	
	//**********Advanced Initialisations************//
	// (1): Initialise PHY layer registers
	ENC_WritePhy(PHLCON, PHLCON_LED);  
	ENC_WritePhy(PHCON1, PHCON1_PDPXMD);
	ENC_WritePhy(PHCON2, PHCON2_HDLDIS);
	//ENC_WritePhy(PHLCON, 0x347A); 

	// (2): Enable Rx interrupt line
	ENC_SetBank(ECON1);
	
	ENC_WriteOp(ENC_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE);
	ENC_WriteOp(ENC_BIT_FIELD_SET, ECON1, ECON1_RXEN);
  ENC_WriteOp(ENC_BIT_FIELD_SET, EIR, EIR_PKTIF);
}

void ENC_PacketSend(uint16_t len, uint8_t* packet)
{
	ENC_WriteWord(EWRPTL, TXSTART_INIT);
	
	ENC_WriteWord(ETXNDL, (TXSTART_INIT+len));
	
	ENC_WriteOp(ENC_Write_BUF_MEM, 0, 0x00);

	ENC_WriteBuf(len, packet);
	ENC_WriteOp(ENC_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
}
//void ENC_PacketSend(uint16_t len, uint8_t* dataBuf)
//{
//	uint8_t retry = 0;
//	
//	while(1)
//	{
//		ENC_WriteOp(ENC_BIT_FIELD_SET, ECON1, ECON1_TXRST);
//    ENC_WriteOp(ENC_BIT_FIELD_CLR, ECON1, ECON1_TXRST);
//		ENC_WriteOp(ENC_BIT_FIELD_CLR, EIR, EIR_TXERIF|EIR_TXIF);
//		
//		// prepare new transmission 
//		if(retry == 0)
//		{
//			ENC_WriteWord(EWRPT, TXSTART_INIT);
//			ENC_WriteWord(ETXND, TXSTART_INIT+len);
//			ENC_WriteOp(ENC_Write_BUF_MEM, 0, 0);  //line 485 ENCj60.cpp
//			ENC_WriteBuf(len, dataBuf);
//		}
//		
//		// initiate transmission
//		ENC_WriteOp(ENC_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
//		uint16_t count = 0;
//		while ((ENC_Read(EIR) & (EIR_TXIF | EIR_TXERIF)) == 0 && ++count < 1000U);
//		if (!(ENC_Read(EIR) & EIR_TXERIF) && count < 1000U) 
//		{
//       // no error; start new transmission
//       break;
//    }
//		// cancel previous transmission if stuck
//    ENC_WriteOp(ENC_BIT_FIELD_CLR, ECON1, ECON1_TXRTS);
//		break;
//	}
//}

void ENC_WriteBuf(uint16_t len, uint8_t* data)
{
	uint8_t spiData[1];
	EN_ENC;
	spiData[0] = ENC_Write_BUF_MEM;
	HAL_SPI_Transmit(&hspi1, spiData, 1, 100);
	HAL_SPI_Transmit(&hspi1, data, len, 100);
	DIS_ENC;
}
void ENC_ReadBuf(uint16_t len, uint8_t* data)
{
	EN_ENC;
	uint8_t mode;
	mode = ENC_Read_BUF_MEM;
	HAL_SPI_Transmit(&hspi1, &mode, 1, 100);
	HAL_SPI_Receive(&hspi1, data, len, 100);
	//*data='\0';
	DIS_ENC;
}
uint16_t ENC_PacketReceive(uint16_t maxlen, uint8_t* packet)
   {
   uint16_t rxstat;
   uint16_t len;
   if( ENC_Read(EPKTCNT) ==0 )  
      {
      return(0);
        }

   //Thiet lap con tro de doc du lieu tu goi tin nhan duoc       
   ENC_WriteWord(ERDPT, NextPacketPtr);

   //Doc gia tri con tro cua goi tin tiep theo
  
		NextPacketPtr  = ENC_ReadOp(ENC_Read_BUF_MEM, 0);
		NextPacketPtr |= (uint16_t)ENC_ReadOp(ENC_Read_BUF_MEM, 0)<<8;

   // Doc kich thuoc cua goi tin
   len  = ENC_ReadOp(ENC_Read_BUF_MEM, 0);
   len |= (uint16_t)ENC_ReadOp(ENC_Read_BUF_MEM, 0)<<8;
 

    len-=4; // xoa 4byte dem CRC
   //Doc trang thai cua bo nhan
   rxstat  = ENC_ReadOp(ENC_Read_BUF_MEM, 0);
   rxstat |= (uint16_t)ENC_ReadOp(ENC_Read_BUF_MEM, 0)<<8;
	len = ((len<maxlen)?(len) : (maxlen));

//    if (len>maxlen-1)
//      {
//        len=maxlen-1;
//       }
//    if ((rxstat & 0x80)==0)
//         {
//          len=0;
//          }
//      else
//         {
//            // Copy du lieu vao bo dem nhan
//            ENC_ReadBuf(len, packet);
//         }
	 ENC_ReadBuf(len, packet);
   // Chuyen con tro du lieu nhan toi phan dau cua goi tin tiep theo.
   // Giai phong bo nho ma ta vua doc ra
   ENC_WriteWord(ERXRDPT, (NextPacketPtr));
   
   ENC_WriteOp(ENC_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);
   return(len);
   }




