#pragma once
#include <stdint.h>
#include <math.h>

#define COUNTER_TIME 1000
#define COUNTER_RATIO 3
#define JOY_RT 1<<0
#define JOY_LT 1<<1
#define JOY_DN 1<<2
#define JOY_UP 1<<3
#define JOY_OK 1<<15
#define SEG_A 1<<0
#define SEG_B 1<<1
#define SEG_C 1<<2
#define SEG_D 1<<3
#define SEG_E 1<<4
#define SEG_F 1<<5
#define SEG_G 1<<6
#define DIG(x) 1<<(1+x)

#define LED0 1<<6
#define LED1 1<<7
#define LED2 1<<8
#define LED3 1<<9
#define LED4 1<<4
#define LED5 1<<3
#define LED6 1<<5
#define LED7 1<<6
#define LED_R 1<<13
#define LED_G 1<<8
#define LED_B 1<<12

#define MAX_BAUDRATE_LENGTH 10
#define MAX_DATABIT_INFO 2
#define MAX_PARITY_INFO 11

#define M1_BIT_NR 28
#define M0_BIT_NR 12
#define PCE 1<<10
#define BLINK_AMOUNT 10

#define ALL_SEGMENTS SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G

typedef struct {
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFRL;
	volatile uint32_t AFRH;
	volatile uint32_t BRR;
} *GPIO_My;

typedef struct {
	volatile uint16_t CR1;
	volatile uint16_t CR1reserved;
	volatile uint8_t CR2;
	volatile uint8_t CR2reserved[3];
	volatile uint32_t RESERVED;
	volatile uint16_t DIER;
	volatile uint16_t DIERReserved;
	volatile uint16_t SR;
	volatile uint16_t SRReserved;
	volatile uint16_t EGR;
	volatile uint16_t EGRReserved;
	volatile uint32_t RESERVED2[3];
	volatile uint32_t CNT;
	volatile uint16_t PSC;
	volatile uint16_t PSCReserved;
	volatile uint16_t ARR;
	volatile uint16_t ARRReserved;
} *TIM_Basic;

#define GPIO_B ((GPIO_My)0x48000400UL)
#define GPIO_C ((GPIO_My)0x48000800UL)
#define GPIO_D ((GPIO_My)0x48000C00UL)
#define GPIO_E ((GPIO_My)0x48001000UL)
#define GPIO_G ((GPIO_My)0x48001800UL)

#define GPIOB_TO_LOW_FREQ_PIN 1 << 9

#define TIM6_CLK ((TIM_Basic)0x40001000UL)
#define ONE_PULSE_MODE_BIT 1<<3

#define RCC_APB1ENR1_REGISTER (volatile uint32_t*)0x40021058UL
#define RCC_APB1ENR2_REGISTER (volatile uint32_t*)0x4002105CUL
#define TIM6_ENABLE_PIN 1<<4
#define UIF 1<<0
#define ENABLE_FLAG 1<<0

#define GPIO_CLK (volatile uint32_t*)0x4002104C
#define RCC_AHB2ENR_MY (volatile uint32_t*)0x4002104C
#define GPIO_B_CLK_PIN 1<<1
#define GPIO_C_CLK_PIN 1<<2
#define GPIO_D_CLK_PIN 1<<3
#define GPIO_E_CLK_PIN 1<<4
#define GPIO_G_CLK_PIN 1<<6

#define GPIO_E_IDR (volatile uint32_t*)0x48001010UL

#define PWR_CR2_ADDRESS (volatile uint32_t*) 0x40007004
#define VDDIO2_PIN 1<<9

#define OUTPUT_MODE 0x01U
#define INPUT_MODE 0x00U
#define ALTERNATE_MODE 0x02U

#define NUMBER_OF_PINS 16U
#define SET_DOUBLE_ZEROS_MASK 0x03U

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define GET_DIGIT(number, n) ((number / (int)pow(10, n)) % 10)
