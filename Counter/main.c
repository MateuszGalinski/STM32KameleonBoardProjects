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
#define DIG1 1<<2
#define DIG2 1<<3
#define DIG3 1<<4
#define DIG4 1<<5
#define SEG_GPIO GPIOG
#define DIGPORT GPIOB

#define LED0 1<<6
#define LED1 1<<7
#define LED2 1<<8
#define LED3 1<<9
#define LED4 1<<4
#define LED5 1<<3
#define LED6 1<<5
#define LED7 1<<6

#define ALL_SEGMENTS SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);
void SystemClock_Config(void);
static void JoyStick_Init(void);
static int Check_Input(void);
static void Diodes_Init(void);

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
  MX_GPIO_Init();
  JoyStick_Init();
  Diodes_Init();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_GPIO_WritePin(GPIOB, DIG4, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOG, ALL_SEGMENTS, GPIO_PIN_RESET);

  int i = 0;
  int counterFlag = 1;
  uint32_t startTime = 0;

  while (1)
  {
    /* USER CODE END WHILE */
	  HAL_GPIO_WritePin(GPIOG, ALL_SEGMENTS, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOG, segments[i], GPIO_PIN_SET);


	  startTime = HAL_GetTick();
	  while(HAL_GetTick() - startTime < COUNTER_TIME){
			counterFlag = Check_Input(); // returns 1 for button not pressed and -1 for button pressed
	  }

	  i+=counterFlag;
	  i=i%10;
	  if(i<0){
		  i=9;
	  }

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();

  GPIO_InitStruct.Pin = DIG1|DIG2|DIG3|DIG4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOB, DIG1|DIG2|DIG3|DIG4, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = ALL_SEGMENTS;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOG, ALL_SEGMENTS, GPIO_PIN_RESET);

}

static void JoyStick_Init(void){
	__HAL_RCC_GPIOE_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct_E = {0};

	GPIO_InitStruct_E.Pin = JOY_OK;
	GPIO_InitStruct_E.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct_E.Pull = GPIO_NOPULL;
	GPIO_InitStruct_E.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct_E);
}

static void Diodes_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct_D = {0};
  GPIO_InitTypeDef GPIO_InitStruct_C = {0};
  GPIO_InitTypeDef GPIO_InitStruct_E = {0};

  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  GPIO_InitStruct_D.Pin = LED5;
  GPIO_InitStruct_D.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct_D.Pull = GPIO_NOPULL;
  GPIO_InitStruct_D.Speed = GPIO_SPEED_FREQ_LOW;

  GPIO_InitStruct_C.Pin = LED0|LED1|LED2|LED3;
  GPIO_InitStruct_C.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct_C.Pull = GPIO_NOPULL;
  GPIO_InitStruct_C.Speed = GPIO_SPEED_FREQ_LOW;

  GPIO_InitStruct_E.Pin = LED4|LED6|LED7;
  GPIO_InitStruct_E.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct_E.Pull = GPIO_NOPULL;
  GPIO_InitStruct_E.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct_D);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct_C);
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct_E);

  HAL_GPIO_WritePin(GPIOD, LED5, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, LED0|LED1|LED2|LED3, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE, LED4|LED6|LED7, GPIO_PIN_RESET);

  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
}

static int Check_Input(void){
	if(HAL_GPIO_ReadPin(GPIOE, JOY_OK) == GPIO_PIN_RESET){
		return -1;
	} else {
		return 1;
	}
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
