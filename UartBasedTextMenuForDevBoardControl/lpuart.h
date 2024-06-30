#pragma once
#include <stdint.h>

#define TX_PIN 1<<1
#define RX_PIN 1<<0
#define TX_AF8 1<<7
#define RX_AF8 1<<3
#define TXE_BIT 1<<7
#define TC_BIT 1<<6
#define RXNE_BIT 1<<5

#define RE_BIT 1<<2
#define TE_BIT 1<<3
#define UE_BIT 1<<0
#define RXFRQ_BIT 1<<3
#define RXNEIE 1<<5

#define LPUART_CALCULATED_FOR_NVIC 1<<6

#define OVER_SAMPLING_BPS(x) ((256 * 4000000) / (x));
#define RCC_APB1ENR2_LPUART1EN_MY 1<<0

//void LPUART_Init(void);
//void Send_String(char* c);
//void Send_Char(char c);
//char Recv_char(void);

typedef struct {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t BRR;
	volatile uint32_t RESEVED[2];
	volatile uint32_t RQR;
	volatile uint32_t ISR;
	volatile uint32_t ICR;
	volatile uint32_t RDR;
	volatile uint32_t TDR;
} *LPUART_MY;

#define LPUART ((LPUART_MY)0x40008000)
#define NVIC_ISER2 ((volatile uint32_t*)0xe000e108)
#define NVIC_IABR2 ((volatile uint32_t*)0xe000e208)

void LPUART_Init(void);
void NVIC_Init(void);
void Send_String(char* c);
void Send_Char(char c);
char Recv_Char(void);
