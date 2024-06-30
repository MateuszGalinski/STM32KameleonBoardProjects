/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "math.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define COUNTER_TIME 1000
#define COUNTER_RATIO 3
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
#define TIM6_ENABLE_PIN 1<<4
#define UIF 1<<0
#define ENABLE_FLAG 1<<0

#define GPIO_CLK (volatile uint32_t*)0x4002104C
#define GPIO_B_CLK_PIN 1<<1
#define GPIO_C_CLK_PIN 1<<2
#define GPIO_D_CLK_PIN 1<<3
#define GPIO_E_CLK_PIN 1<<4
#define GPIO_G_CLK_PIN 1<<6

#define PWR_CR2_ADDRESS (volatile uint32_t*) 0x40007004
#define VDDIO2_PIN 1<<9

#define OUTPUT_MODE 0x01U
#define INPUT_MODE 0x00U

#define NUMBER_OF_PINS 16U
#define SET_DOUBLE_ZEROS_MASK 0x03U

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define GET_DIGIT(number, n) ((number / (int)pow(10, n)) % 10)
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void My_Clock_Init(void);
static void Display_Init(void);
void SystemClock_Config(void);
static void JoyStick_Init(void);
static int Check_Input(void);
static void Diodes_Init(void);
static void Set_Moder(GPIO_My port, uint32_t pins, uint32_t mode);
static void My_Delay(int miliseconds);

const uint8_t segments[] = {
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

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  My_Clock_Init();
  SystemClock_Config();
  Display_Init();
  JoyStick_Init();
  Diodes_Init();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  int counterTimer = 0;
  int i = 9997;
  int counterFlag = 1;

  while (1)
    {
      /* USER CODE END WHILE */
  	  for(int j = 0; j<4; j++){
  		  GPIO_B -> ODR &= ~(DIG(4)|DIG(3)|DIG(2)|DIG(1));
  		  GPIO_G -> ODR &= ~(ALL_SEGMENTS);
  		  GPIO_G -> ODR |= segments[GET_DIGIT(i,j)];
  		  GPIO_B -> ODR |= DIG(4-j);
  		  My_Delay(1);
  		  counterTimer++;
  	  }

  	  counterFlag = Check_Input();

  	  if(counterTimer > COUNTER_TIME/COUNTER_RATIO){
  		  i += counterFlag;
  		  counterTimer = 0;
  	  }

  	  if(i>9999){
		  i = 0;
	  }

	  if(i<0){
		  i=9999;
	  }
      /* USER CODE BEGIN 3 */
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
static void My_Clock_Init(void){
	//RCC -> APB1ENR1 |= 1<<4;
	*RCC_APB1ENR1_REGISTER |= TIM6_ENABLE_PIN;
	TIM6_CLK -> CR1 |= ONE_PULSE_MODE_BIT;
	TIM6_CLK -> PSC = 3999;
	TIM6_CLK -> CNT = 0;
	//TIM6_CLK -> CR1 |= 1<<0;
}

static void My_Delay(int miliseconds){
	TIM6_CLK -> ARR = miliseconds;
//	TIM6_CLK -> CNT = 0;
	TIM6_CLK -> CR1 |= ENABLE_FLAG;
	while(TIM6_CLK -> SR != 1){}
//	TIM6_CLK -> CR1 &= ~(1<<0);
	TIM6_CLK -> SR &= ~(UIF);
//	TIM6_CLK -> CNT = 0;
}

static void Display_Init(void)
{
  *GPIO_CLK |= GPIO_B_CLK_PIN | GPIO_G_CLK_PIN;
  *PWR_CR2_ADDRESS |= VDDIO2_PIN;

  Set_Moder(GPIO_B, DIG(1)|DIG(2)|DIG(3)|DIG(4), OUTPUT_MODE);
  GPIO_B -> PUPDR &= ~(GPIOB_TO_LOW_FREQ_PIN);

  Set_Moder(GPIO_G, ALL_SEGMENTS, OUTPUT_MODE);

}

static void JoyStick_Init(void){
	*GPIO_CLK |= GPIO_E_CLK_PIN;

	Set_Moder(GPIO_E, JOY_OK, INPUT_MODE);
}

static int Check_Input(void){
	if((GPIO_E -> IDR & JOY_OK) == 0){
		return -1;
	} else {
		return 1;
	}
}

static void Diodes_Init(void){
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

static void Set_Moder(GPIO_My port, uint32_t pins, uint32_t mode){

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

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
