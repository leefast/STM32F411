#include "stm32f4xx_hal.h"

#ifndef ENC__H
#define ENC__H


#define EN_ENC HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
#define DIS_ENC HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
#define LED_ON HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
// Operations Defines
#define ENC_Read_CTRL_REG	 	 0x00
#define ENC_Write_CTRL_REG   0x40 //0x02
#define ENC_BIT_FIELD_CLR		 0xA0 //0x05
#define ENC_BIT_FIELD_SET		 0x80 //0x04
#define ENC_SOFT_RESET		   0xFF //0x07

#define ENC_Read_BUF_MEM		 0x3A //0x01
#define ENC_Write_BUF_MEM		 0x7A //0x03

// Masks and some constants
#define ADDR_MASK									0x1F
#define BANK_MASK									0x60  //0110 0000
#define RXSTART_INIT							0x0000
#define RXSTOP_INIT								0x0BFF
#define TXSTART_INIT							0x0C00
#define TXSTOP_INIT								0x11FF
#define MAX_FRAMELEN							1500
#define MISTAT_BUSY								0x01
#define	ETHERNET_MIN_PACKET_LENGTH	0x3C


// Bank0 - control registers addresses
//Word
#define ERDPT											0x00
#define EWRPT											0x02
#define ETXST											0x04
#define ETXND											0x06
#define ERXST											0x08
#define ERXND											0x0A
#define ERXRDPT										0x0C
#define ERXWRPT										0x0E
//Byte
#define ERDPTL           (0x00|0x00)
#define ERDPTH           (0x01|0x00)
#define EWRPTL           (0x02|0x00)
#define EWRPTH           (0x03|0x00)
#define ETXSTL           (0x04|0x00)
#define ETXSTH           (0x05|0x00)
#define ETXNDL           (0x06|0x00)
#define ETXNDH           (0x07|0x00)
#define ERXSTL           (0x08|0x00)
#define ERXSTH           (0x09|0x00)
#define ERXNDL           (0x0A|0x00)
#define ERXNDH           (0x0B|0x00)
//ERXWRPTH:ERXWRPTL 
#define ERXRDPTL         (0x0C|0x00)
#define ERXRDPTH         (0x0D|0x00)
#define ERXWRPTL         (0x0E|0x00)
#define ERXWRPTH         (0x0F|0x00)
#define EDMASTL          (0x10|0x00)
#define EDMASTH          (0x11|0x00)
#define EDMANDL          (0x12|0x00)
#define EDMANDH          (0x13|0x00)
#define EDMADSTL         (0x14|0x00)
#define EDMADSTH         (0x15|0x00)
#define EDMACSL          (0x16|0x00)
#define EDMACSH          (0x17|0x00)


// Bank1 - control registers addresses

#define EHT0             (0x00|0x20)
#define EHT1             (0x01|0x20)
#define EHT2             (0x02|0x20)
#define EHT3             (0x03|0x20)
#define EHT4             (0x04|0x20)
#define EHT5             (0x05|0x20)
#define EHT6             (0x06|0x20)
#define EHT7             (0x07|0x20)
#define EPMM0            (0x08|0x20)
#define EPMM1            (0x09|0x20)
#define EPMM2            (0x0A|0x20)
#define EPMM3            (0x0B|0x20)
#define EPMM4            (0x0C|0x20)
#define EPMM5            (0x0D|0x20)
#define EPMM6            (0x0E|0x20)
#define EPMM7            (0x0F|0x20)
#define EPMCSL           (0x10|0x20)
#define EPMCSH           (0x11|0x20)
#define EPMOL            (0x14|0x20)
#define EPMOH            (0x15|0x20)
#define EWOLIE           (0x16|0x20)
#define EWOLIR           (0x17|0x20)
#define ERXFCON          (0x18|0x20)
#define EPKTCNT          (0x19|0x20)

// Bank2 - control registers addresses
#define MACON1           (0x00|0x40|0x80)
#define MACON2           (0x01|0x40|0x80)
#define MACON3           (0x02|0x40|0x80)
#define MACON4           (0x03|0x40|0x80)
#define MABBIPG          (0x04|0x40|0x80)
#define MAIPGL           (0x06|0x40|0x80)
#define MAIPGH           (0x07|0x40|0x80)
#define MACLCON1         (0x08|0x40|0x80)
#define MACLCON2         (0x09|0x40|0x80)
#define MAMXFLL          (0x0A|0x40|0x80)
#define MAMXFLH          (0x0B|0x40|0x80)
#define MAPHSUP          (0x0D|0x40|0x80)
#define MICON            (0x11|0x40|0x80)
#define MICMD            (0x12|0x40|0x80)
#define MIREGADR         (0x14|0x40|0x80)
#define MIWRL            (0x16|0x40|0x80)
#define MIWRH            (0x17|0x40|0x80)
#define MIRDL            (0x18|0x40|0x80)
#define MIRDH            (0x19|0x40|0x80)


// Bank3 - control registers addresses
#define MAADR1									  0x00|0x60|0x80
#define MAADR2									  0x03|0x60|0x80
#define MAADR3									  0x02|0x60|0x80
#define MAADR4									  0x05|0x60|0x80
#define MAADR5									  0x04|0x60|0x80
#define MAADR0									  0x01|0x60|0x80
#define MISTAT									  0x0A|0x60|0x80
#define EREVID									  0x12|0x60
#define EBSTSD          				 (0x06|0x60)
#define EBSTCON         				 (0x07|0x60)
#define EBSTCSL         				 (0x08|0x60)
#define EBSTCSH         				 (0x09|0x60)
#define ECOCON         				  	(0x15|0x60)
#define EFLOCON        				  	(0x17|0x60)
#define EPAUSL         				  (0x18|0x60)
#define EPAUSH          				 (0x19|0x60)
// Common registers
#define EIE												0x1B
#define EIR												0x1C
#define ESTAT											0x1D
#define ECON1											0x1F
#define ECON2											0x1E

// BitField Defines
// ENC_ ERXFCON Register Bit Definitions
#define ERXFCON_UCEN     0x80
#define ERXFCON_ANDOR    0x40
#define ERXFCON_CRCEN    0x20
#define ERXFCON_PMEN     0x10
#define ERXFCON_MPEN     0x08
#define ERXFCON_HTEN     0x04
#define ERXFCON_MCEN     0x02
#define ERXFCON_BCEN     0x01
// ENC_ EIE Register Bit Definitions
#define EIE_INTIE        0x80
#define EIE_PKTIE        0x40
#define EIE_DMAIE        0x20
#define EIE_LINKIE       0x10
#define EIE_TXIE         0x08
#define EIE_WOLIE        0x04
#define EIE_TXERIE       0x02
#define EIE_RXERIE       0x01
// ENC_ EIR Register Bit Definitions
#define EIR_PKTIF        0x40
#define EIR_DMAIF        0x20
#define EIR_LINKIF       0x10
#define EIR_TXIF         0x08
#define EIR_WOLIF        0x04
#define EIR_TXERIF       0x02
#define EIR_RXERIF       0x01
// ENC_ ESTAT Register Bit Definitions
#define ESTAT_INT        0x80
#define ESTAT_LATECOL    0x10
#define ESTAT_RXBUSY     0x04
#define ESTAT_TXABRT     0x02
#define ESTAT_CLKRDY     0x01
// ENC_ ECON2 Register Bit Definitions
#define ECON2_AUTOINC    0x80
#define ECON2_PKTDEC     0x40
#define ECON2_PWRSV      0x20
#define ECON2_VRPS       0x08
// ENC_ ECON1 Register Bit Definitions
#define ECON1_TXRST      0x80
#define ECON1_RXRST      0x40
#define ECON1_DMAST      0x20
#define ECON1_CSUMEN     0x10
#define ECON1_TXRTS      0x08
#define ECON1_RXEN       0x04
#define ECON1_BSEL1      0x02
#define ECON1_BSEL0      0x01
// ENC_ MACON1 Register Bit Definitions
#define MACON1_LOOPBK    0x10
#define MACON1_TXPAUS    0x08
#define MACON1_RXPAUS    0x04
#define MACON1_PASSALL   0x02
#define MACON1_MARXEN    0x01
// ENC_ MACON2 Register Bit Definitions
#define MACON2_MARST     0x80
#define MACON2_RNDRST    0x40
#define MACON2_MARXRST   0x08
#define MACON2_RFUNRST   0x04
#define MACON2_MATXRST   0x02
#define MACON2_TFUNRST   0x01
// ENC_ MACON3 Register Bit Definitions
#define MACON3_PADCFG2   0x80
#define MACON3_PADCFG1   0x40
#define MACON3_PADCFG0   0x20
#define MACON3_TXCRCEN   0x10
#define MACON3_PHDRLEN   0x08
#define MACON3_HFRMLEN   0x04
#define MACON3_FRMLNEN   0x02
#define MACON3_FULDPX    0x01
// ENC_ MICMD Register Bit Definitions
#define MICMD_MIISCAN    0x02
#define MICMD_MIIRD      0x01
// ENC_ MISTAT Register Bit Definitions
#define MISTAT_NVALID    0x04
#define MISTAT_SCAN      0x02
#define MISTAT_BUSY      0x01
// ENC_ PHY PHCON1 Register Bit Definitions
#define PHCON1_PRST      0x8000
#define PHCON1_PLOOPBK   0x4000
#define PHCON1_PPWRSV    0x0800
#define PHCON1_PDPXMD    0x0100
// ENC_ PHY PHSTAT1 Register Bit Definitions
#define PHSTAT1_PFDPX    0x1000
#define PHSTAT1_PHDPX    0x0800
#define PHSTAT1_LLSTAT   0x0004
#define PHSTAT1_JBSTAT   0x0002

// ENC_ Packet Control Byte Bit Definitions
#define PKTCTRL_PHUGEEN  0x08
#define PKTCTRL_PPADEN   0x04
#define PKTCTRL_PCRCEN   0x02
#define PKTCTRL_POVERRIDE 0x01


// PHY layer
#define PHCON2_HDLDIS							0x0100
#define PHLCON_LED								0x0122
// PHY registers
#define PHCON1           0x00
#define PHSTAT1          0x01
#define PHHID1           0x02
#define PHHID2           0x03
#define PHCON2           0x10
#define PHSTAT2          0x11
#define PHIE             0x12
#define PHIR             0x13
#define PHLCON           0x14


//Functions prototypes
void ENC_Reset(void);

uint8_t ENC_ReadOp(uint8_t oper, uint8_t addr);
void ENC_WriteOp(uint8_t oper, uint8_t addr, uint8_t data);

uint8_t ENC_Read( uint8_t addr);
void ENC_Write(uint8_t addr, uint8_t data);

uint16_t ENC_ReadWord( uint8_t addr);
void ENC_WriteWord(uint8_t addrL, uint16_t data);

void ENC_SetBank(uint8_t addr);

void ENC_Init( uint8_t *macaddr );

void ENC_WritePhy(uint8_t addr, uint16_t data);
uint16_t ENC_ReadPhy(uint8_t addr);

void ENC_PacketSend(uint16_t len, uint8_t* dataBuf);
uint16_t ENC_PacketReceive(uint16_t maxlen, uint8_t* packet);

void ENC_WriteBuf(uint16_t len, uint8_t* data);
void ENC_ReadBuf(uint16_t len, uint8_t* data);
#endif //ENC__H
