#include "lpuart.h"
#include "myDefines.h"
#include "usefulfunctions.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void LPUART_Init(){
	*RCC_AHB2ENR_MY |= GPIO_C_CLK_PIN;
	*RCC_APB1ENR2_REGISTER |= RCC_APB1ENR2_LPUART1EN_MY;

	LPUART->BRR = OVER_SAMPLING_BPS(115200);
	LPUART->CR1 |= TE_BIT | UE_BIT;
	LPUART->RQR |= RXFRQ_BIT;

	Set_Moder(GPIO_C, TX_PIN | RX_PIN, ALTERNATE_MODE);
	GPIO_C -> AFRL |= (TX_AF8 | RX_AF8);

	LPUART->CR1 |= RE_BIT;
	LPUART->CR1 |= (1<<5);
}

void NVIC_Init(void){
	*NVIC_IABR2 |= LPUART_CALCULATED_FOR_NVIC;
	*NVIC_ISER2 |= LPUART_CALCULATED_FOR_NVIC;
}

void Send_Char(char c){
	while ( (LPUART -> ISR & TXE_BIT) == 0){};
	LPUART -> TDR = c;
//	while ( (LPUART -> ISR & TC_BIT) == 0){};
}



void Send_String(char* c){
	for(int i=0; i<strlen(c); i++){
		Send_Char(c[i]);
	}
}

char Recv_Char (void){
	while ((LPUART -> ISR & (RXNE_BIT)) == 0){};
	char Temp = LPUART -> RDR;
	return Temp;
}
