//--------------------------------------------------------------------------------------
//Dia chi vat ly hay dia chi MAC (lop Ethernet)
#ifndef PACKET_H
#define PACKET_H
#include "stm32f4xx_hal.h"

#define HTONS(s)		((s<<8) | (s>>8))	//danh cho bien 2 byte
#define HTONL(l)		((l<<24) | ((l&0x00FF0000l)>>8) | ((l&0x0000FF00l)<<8) | (l>>24))	//danh cho bien 4 byte


typedef struct EthAddr
{
	uint8_t addr[6];
}EthAddr;	



typedef struct ipAddr
{
	uint8_t addr[4];
}ipAddr;	
//--------------------------------------------------------------------------------------
//Ethernet header
//  Gom 14 byte:
//  06 byte dia chi dich
//  06 byte dia chi nguon
//  02 byte type (cho biet frame ethernet mang ben trong loai du lieu gi)
#define ETH_HEADER_LEN	14
typedef struct EthHeader
{
	 EthAddr desAddr;
	 EthAddr srcAddr;
	 uint16_t type;
}EthHeader;
//type Ethernet
#define ETH_TYPE_ARP 0x0806 //0806 do thanh ghi nen phai ghi nguoc
#define ETH_TYPE_IP  0x0800
#define ETH_TYPE_IP6 0x86dd

//--------------------------------------------------------------------------------------
//Cau truc IP header
typedef struct IPHeader
{
	uint8_t	verHdrLen;
	uint8_t	ToS;
	uint16_t	Len;
	uint16_t	IDNumber;
	uint16_t	Offset;
	uint8_t	TTL;
	uint8_t	Protocol;
	uint16_t	Checksum;
	ipAddr	srcIPAddr;
	ipAddr	desIPAddr;
	uint8_t	Option[4];
}IPHeader;
//--------------------------------------------------------------------------------------
//Cau truc ARP header
typedef struct ARPHeader
{
	uint16_t	hwType;
	uint16_t	protocol;
	uint8_t	hwLen;
	uint8_t	protoLen;
	uint16_t	opcode;
	EthAddr shwaddr;
	ipAddr sipaddr;
	//uint32_t sipaddr;
	EthAddr dhwaddr;
	ipAddr dipaddr;
	//uint32_t dipaddr;
}ARPHeader;
#define ARP_OPCODE_REQUEST	1
#define ARP_OPCODE_REPLY	2
#define ARP_HWTYPE_ETH		1
#define IP_HEADER_LEN	20

#define IP_PROTO_ICMP	1
#define IP_PROTO_TCP	6
#define IP_PROTO_UDP	17


//--------------------------------------------------------------------------------------
//Cau truc ICMP header
typedef struct ICMPHeader
{
	uint8_t	Type;
	uint8_t	Code;
	uint16_t	Checksum;
	uint16_t	ID;
	uint16_t	seqNumber;
}ICMPHeader;

#define ICMP_HEADER_LEN	8
#define ICMP_TYPE_ECHOREPLY		0
#define ICMP_TYPE_ECHOREQUEST	8


//--------------------------------------------------------------------------------------
//Cau truc TCP header
typedef struct TCPHeader
{
	uint16_t	srcPort;
	uint16_t	desPort;
	uint32_t	seqNumber;
	uint32_t	ackNumber;
	uint8_t		Offset;
	uint8_t		Flags;
	uint16_t	Window;
	uint16_t	Checksum;
	uint16_t	UrgentPtr;
	uint8_t	optdata[8];
}TCPHeader;

#define TCP_HEADER_LEN	20

#define TCP_NON_FLAG (0)
#define TCP_FIN_FLAG (1<<0)
#define TCP_SYN_FLAG (1<<1)
#define TCP_RST_FLAG (1<<2)
#define TCP_PSH_FLAG (1<<3)
#define TCP_ACK_FLAG (1<<4)
#define TCP_URG_FLAG (1<<5)
#define TCP_ECE_FLAG (1<<6)
#define TCP_CWR_FLAG (1<<7)

#endif
