#include "usefulfunctions.h"

void My_Clock_Init(void){
	//RCC -> APB1ENR1 |= 1<<4;
	*RCC_APB1ENR1_REGISTER |= TIM6_ENABLE_PIN;
	TIM6_CLK -> CR1 |= ONE_PULSE_MODE_BIT;
	TIM6_CLK -> PSC = 3999;
	TIM6_CLK -> CNT = 0;
	//TIM6_CLK -> CR1 |= 1<<0;
}

void My_Delay(int miliseconds){
	TIM6_CLK -> ARR = miliseconds;
//	TIM6_CLK -> CNT = 0;
	TIM6_CLK -> CR1 |= ENABLE_FLAG;
	while(TIM6_CLK -> SR != 1){}
//	TIM6_CLK -> CR1 &= ~(1<<0);
	TIM6_CLK -> SR &= ~(UIF);
//	TIM6_CLK -> CNT = 0;
}

void Display_Init(void)
{
  *GPIO_CLK |= GPIO_B_CLK_PIN | GPIO_G_CLK_PIN;
  *PWR_CR2_ADDRESS |= VDDIO2_PIN;

  Set_Moder(GPIO_B, DIG(1)|DIG(2)|DIG(3)|DIG(4), OUTPUT_MODE);
  GPIO_B -> PUPDR &= ~(GPIOB_TO_LOW_FREQ_PIN);

  Set_Moder(GPIO_G, ALL_SEGMENTS, OUTPUT_MODE);

}

void JoyStick_Init(void){
	*GPIO_CLK |= GPIO_E_CLK_PIN;

	Set_Moder(GPIO_E, JOY_OK, INPUT_MODE);
}

int Check_Input(void){
	if((GPIO_E -> IDR & JOY_OK) == 0){
		return -1;
	} else {
		return 1;
	}
}

void Diodes_Init(void){
  *GPIO_CLK |= GPIO_D_CLK_PIN | GPIO_C_CLK_PIN | GPIO_E_CLK_PIN;

  Set_Moder(GPIO_D, LED5, OUTPUT_MODE);
  GPIO_D -> ODR &= ~(LED5);

  Set_Moder(GPIO_C, LED0|LED1|LED2|LED3, OUTPUT_MODE);
  GPIO_C -> ODR &= ~(LED0|LED1|LED2|LED3);

  Set_Moder(GPIO_E, LED4|LED6|LED7, OUTPUT_MODE);
  GPIO_E -> ODR &= ~(LED4|LED6|LED7);


//  *GPIO_CLK &= ~(GPIO_D_CLK_PIN | GPIO_C_CLK_PIN);
  *GPIO_CLK &= ~(GPIO_C_CLK_PIN);

}

void Set_Moder(GPIO_My port, uint32_t pins, uint32_t mode){

	uint32_t temp = port -> MODER;

	for(int position = 0; position < NUMBER_OF_PINS; position++){
		uint32_t currentPin = pins & (1<<position);

		if((1<<position) == currentPin){
			temp &= ~(SET_DOUBLE_ZEROS_MASK << (position * 2U));
			temp |= (mode << (position * 2U));
		}
	}

	port -> MODER = temp;
}
