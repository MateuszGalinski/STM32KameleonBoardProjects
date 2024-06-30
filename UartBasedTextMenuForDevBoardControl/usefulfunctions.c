#include "usefulfunctions.h"
#include "myDefines.h"
#include "lpuart.h"
#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
//#include "main.h"

uint8_t segments[] = {
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,	// 0
	SEG_B | SEG_C,	// 1
	SEG_A | SEG_B | SEG_D | SEG_E | SEG_G,	// 2
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_G,	// 3
	SEG_B | SEG_C | SEG_F | SEG_G,	// 4
	SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,	// 5
	SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,	// 6
	SEG_A | SEG_B | SEG_C,	// 7
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,	// 8
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G	// 9
};

uint16_t LEDSequence[8] = {LED0,LED1,LED2,LED3,LED4,LED5,LED6,LED7};
GPIO_My portSequence[8] = {GPIO_C,GPIO_C,GPIO_C,GPIO_C,GPIO_E,GPIO_D,GPIO_E,GPIO_E};

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

	Set_Moder(GPIO_E, (JOY_UP|JOY_DN|JOY_LT|JOY_RT|JOY_OK), INPUT_MODE);
}

int Check_Input(void){
	if((GPIO_E -> IDR & JOY_OK) == 0){
		return -1;
	} else {
		return 1;
	}
}

void Diodes_Init(void){
  *GPIO_CLK |= GPIO_D_CLK_PIN | GPIO_C_CLK_PIN | GPIO_E_CLK_PIN | GPIO_B_CLK_PIN;

  Set_Moder(GPIO_D, LED5|LED_R|LED_B, OUTPUT_MODE);
  GPIO_D -> ODR &= ~(LED5|LED_R|LED_B);

  Set_Moder(GPIO_C, LED0|LED1|LED2|LED3, OUTPUT_MODE);
  GPIO_C -> ODR &= ~(LED0|LED1|LED2|LED3);

  Set_Moder(GPIO_E, LED4|LED6|LED7, OUTPUT_MODE);
  GPIO_E -> ODR &= ~(LED4|LED6|LED7);

  Set_Moder(GPIO_B, LED_G, OUTPUT_MODE);
  GPIO_B -> ODR &= ~(LED_G);

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

void displayNumber(int number){
	for(int j = 0; j<4; j++){
		GPIO_B -> ODR &= ~(DIG(4)|DIG(3)|DIG(2)|DIG(1));
		GPIO_G -> ODR &= ~(ALL_SEGMENTS);
		GPIO_G -> ODR |= segments[GET_DIGIT(number,j)];
		GPIO_B -> ODR |= DIG(4-j);
		HAL_Delay(1);
	}
}

void setRGBLed(char* ledColor){
	if(strcasecmp(ledColor, "R") == 0){
		GPIO_D -> ODR |= LED_R;
	}

	if(strcasecmp(ledColor, "G") == 0){
		GPIO_B -> ODR |= LED_G;
	}

	if(strcasecmp(ledColor, "B") == 0){
		GPIO_D -> ODR |= LED_B;
	}
}

void resetRGBLed(char* ledColor){
	if(strcasecmp(ledColor, "R") == 0){
		GPIO_D -> ODR &= ~(LED_R);
	}

	if(strcasecmp(ledColor, "G") == 0){
		GPIO_B -> ODR &= ~(LED_G);
	}

	if(strcasecmp(ledColor, "B") == 0){
		GPIO_D -> ODR &= ~(LED_B);
	}
}

void toggleRGBLed(char* ledColor){
	if(strcasecmp(ledColor, "R") == 0){
		GPIO_D -> ODR ^= LED_R;
	}

	if(strcasecmp(ledColor, "G") == 0){
		GPIO_B -> ODR ^= LED_G;
	}

	if(strcasecmp(ledColor, "B") == 0){
		GPIO_D -> ODR ^= LED_B;
	}
}

int statusRGBLed(char* ledColor){
	if(strcasecmp(ledColor, "R") == 0){
		if(GPIO_D -> ODR & LED_R){
			return 1;
		}
	}

	if(strcasecmp(ledColor, "G") == 0){
		if(GPIO_B -> ODR & LED_G){
			return 1;
		}
	}

	if(strcasecmp(ledColor, "B") == 0){
		if(GPIO_D -> ODR & LED_B){
			return 1;
		}
	}

	return 0;
}

void setLed(uint16_t ledNumber){
	(portSequence[ledNumber]) -> ODR |= (LEDSequence[ledNumber]);
}

void resetLed(uint16_t ledNumber){
	(portSequence[ledNumber]) -> ODR &= (~(LEDSequence[ledNumber]));
}

void toggleLed(uint16_t ledNumber){
	(portSequence[ledNumber]) -> ODR ^= (LEDSequence[ledNumber]);
}

int getLedStatus(uint16_t ledNumber){
	if(((portSequence[ledNumber] -> ODR) & LEDSequence[ledNumber])){
		return 1;
	}

	return 0;
}


int statusJoy(char* JoyPosition){
	if(strcasecmp(JoyPosition, "C") == 0){
		if((GPIO_E -> IDR & JOY_OK) == 0){
			return 1;
		}
	}

	if(strcasecmp(JoyPosition, "L") == 0){
		if((GPIO_E -> IDR & JOY_LT) == 0){
			return 1;
		}
	}

	if(strcasecmp(JoyPosition, "R") == 0){
		if((GPIO_E -> IDR & JOY_RT) == 0){
			return 1;
		}
	}


	if(strcasecmp(JoyPosition, "U") == 0){
		if((GPIO_E -> IDR & JOY_UP) == 0){
			return 1;
		}
	}

	if(strcasecmp(JoyPosition, "D") == 0){
		if((GPIO_E -> IDR & JOY_DN) == 0){
			return 1;
		}
	}

	return 0;
}

void blinkLedInMain(int* blinkCounter, char** currentlyBlinkingLed){
	if(*blinkCounter >= BLINK_AMOUNT){
		*blinkCounter = 0;
		*currentlyBlinkingLed = "N";
	}

	if(strcasecmp(*currentlyBlinkingLed, "N") != 0){
		*blinkCounter += 1;
	}

	if(isdigit((unsigned char) *currentlyBlinkingLed[0])){
		int ledNum = atoi(*currentlyBlinkingLed);
		toggleLed(ledNum);
	} else {
		toggleRGBLed(*currentlyBlinkingLed);
	}
}


