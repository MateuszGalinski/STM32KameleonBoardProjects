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
#define JOY_OK 1<<15
#define SEG_A 1<<0
#define SEG_B 1<<1
#define SEG_C 1<<2
#define SEG_D 1<<3
#define SEG_E 1<<4
#define SEG_F 1<<5
#define SEG_G 1<<6
#define DIG_1 1<<2
#define DIG_2 1<<3
#define DIG_3 1<<4
#define DIG_4 1<<5
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

#define GPIO_B_ODR (volatile uint32_t*)0x48000414UL
#define GPIO_C_ODR (volatile uint32_t*)0x48000814UL
#define GPIO_D_ODR (volatile uint32_t*)0x48000C14UL
#define GPIO_E_IDR (volatile uint32_t*)0x48001010UL
#define GPIO_E_ODR (volatile uint32_t*)0x48001014UL
#define GPIO_G_ODR (volatile uint32_t*)0x48001814UL

#define GPIO_B_MODER (volatile uint32_t*)0x48000400UL
#define GPIO_C_MODER (volatile uint32_t*)0x48000800UL
#define GPIO_D_MODER (volatile uint32_t*)0x48000C00UL
#define GPIO_E_MODER (volatile uint32_t*)0x48001000UL
#define GPIO_G_MODER (volatile uint32_t*)0x48001800UL

#define GPIO_B_PUPDR (volatile uint32_t*)0x4800040CUL

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
static void Display_Init(void);
void SystemClock_Config(void);
static void JoyStick_Init(void);
static int Check_Input(void);
static void Diodes_Init(void);
static void Set_Moder(volatile uint32_t* portModer, uint32_t pins, uint32_t mode);

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
  HAL_GPIO_WritePin(GPIOG, ALL_SEGMENTS, GPIO_PIN_RESET);

  int i = 9997;
  int counterFlag = 1;
  uint32_t startTime = 0;

  while (1)
  {
    /* USER CODE END WHILE */
	  for(int j = 0; j<4; j++){
		  *GPIO_B_ODR &= ~(DIG(4)|DIG(3)|DIG(2)|DIG(1));
		  *GPIO_G_ODR &= ~(ALL_SEGMENTS);
		  *GPIO_G_ODR |= segments[GET_DIGIT(i,j)];
		  *GPIO_B_ODR |= DIG(4-j);
		  HAL_Delay(1);
	  }


	  counterFlag = Check_Input();

	  if(HAL_GetTick() - startTime > COUNTER_TIME){
		  i+=counterFlag;
		  startTime = HAL_GetTick();
	  }

	  if(i>9999){
		  i = 0;
	  }

	  if(i<0){
		  i=9999;
	  }

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
static void Display_Init(void)
{
  *GPIO_CLK |= GPIO_B_CLK_PIN | GPIO_G_CLK_PIN;
  *PWR_CR2_ADDRESS |= VDDIO2_PIN;

  Set_Moder(GPIO_B_MODER, DIG_1|DIG_2|DIG_3|DIG_4, OUTPUT_MODE);
  *GPIO_B_PUPDR = 0;

  Set_Moder(GPIO_G_MODER, ALL_SEGMENTS, OUTPUT_MODE);

}

static void JoyStick_Init(void){
	*GPIO_CLK |= GPIO_E_CLK_PIN;

	Set_Moder(GPIO_E_MODER, JOY_OK, INPUT_MODE);
}

static int Check_Input(void){
	if((*GPIO_E_IDR & 1<<15) == 0){
		return -1;
	} else {
		return 1;
	}
}

static void Diodes_Init(void){
  *GPIO_CLK |= GPIO_D_CLK_PIN | GPIO_C_CLK_PIN | GPIO_E_CLK_PIN;

  Set_Moder(GPIO_D_MODER, LED5, OUTPUT_MODE);
  *GPIO_D_ODR &= ~(LED5);

  Set_Moder(GPIO_C_MODER, LED0|LED1|LED2|LED3, OUTPUT_MODE);
  *GPIO_C_ODR &= ~(LED0|LED1|LED2|LED3);

  Set_Moder(GPIO_E_MODER, LED4|LED6|LED7, OUTPUT_MODE);
  *GPIO_E_ODR &= ~(LED4|LED6|LED7);


  *GPIO_CLK &= ~(GPIO_D_CLK_PIN | GPIO_C_CLK_PIN);
}

static void Set_Moder(volatile uint32_t* portModer, uint32_t pins, uint32_t mode){

	uint32_t temp = *portModer;

	for(int position = 0; position < NUMBER_OF_PINS; position++){
		uint32_t currentPin = pins & (1<<position);

		if((1<<position) == currentPin){
			temp &= ~(SET_DOUBLE_ZEROS_MASK << (position * 2U));
			temp |= (mode << (position * 2U));
		}
	}

	*portModer = temp;
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
