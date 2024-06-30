/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define BLINK_TIME 500
#define LED0 1<<6
#define LED1 1<<7
#define LED2 1<<8
#define LED3 1<<9
#define LED4 1<<4
#define LED5 1<<3
#define LED6 1<<5
#define LED7 1<<6

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define MY_PIN(x) 1<<x
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void Blink_Leds_To_Left(void);
static void Blink_Leds_To_Right(void);
static uint16_t Get_Led(int ledNumber);
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

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */

	  Blink_Leds_To_Left();
	  Blink_Leds_To_Right();

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks
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
  /** Initializes the CPU, AHB and APB busses clocks
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
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct_D = {0};
  GPIO_InitTypeDef GPIO_InitStruct_C = {0};
  GPIO_InitTypeDef GPIO_InitStruct_E = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin Output Level */

  /*Configure GPIO pin : PD12 */
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

}

/* USER CODE BEGIN 4 */
static uint16_t Get_Led(int ledNumber){
	switch(ledNumber){
		case 0:
			return LED0;
		case 1:
			return LED1;
		case 2:
			return LED2;
		case 3:
			return LED3;
		case 4:
			return LED4;
		case 5:
			return LED5;
		case 6:
			return LED6;
		case 7:
			return LED7;
	}

	return 0;
}

static void Blink_Leds_To_Left(void)
{
	for(int i = 0; i<4; i++){
		HAL_GPIO_WritePin(GPIOC, Get_Led(i), GPIO_PIN_SET);
		HAL_Delay(BLINK_TIME);
		HAL_GPIO_WritePin(GPIOC, Get_Led(i), GPIO_PIN_RESET);
		HAL_Delay(BLINK_TIME);
	}

	HAL_GPIO_WritePin(GPIOE, Get_Led(4), GPIO_PIN_SET);
	HAL_Delay(BLINK_TIME);
	HAL_GPIO_WritePin(GPIOE, Get_Led(4), GPIO_PIN_RESET);
	HAL_Delay(BLINK_TIME);

	HAL_GPIO_WritePin(GPIOD, Get_Led(5), GPIO_PIN_SET);
	HAL_Delay(BLINK_TIME);
	HAL_GPIO_WritePin(GPIOD, Get_Led(5), GPIO_PIN_RESET);
	HAL_Delay(BLINK_TIME);

	for(int i = 6; i<8; i++){
		HAL_GPIO_WritePin(GPIOE, Get_Led(i), GPIO_PIN_SET);
		HAL_Delay(BLINK_TIME);
		HAL_GPIO_WritePin(GPIOE, Get_Led(i), GPIO_PIN_RESET);
		HAL_Delay(BLINK_TIME);
	}
}

static void Blink_Leds_To_Right(void)
{
	for(int i = 7; i >= 6; i--){
		HAL_GPIO_WritePin(GPIOE, Get_Led(i), GPIO_PIN_SET);
		HAL_Delay(BLINK_TIME);
		HAL_GPIO_WritePin(GPIOE, Get_Led(i), GPIO_PIN_RESET);
		HAL_Delay(BLINK_TIME);
	}

	HAL_GPIO_WritePin(GPIOD, Get_Led(5), GPIO_PIN_SET);
	HAL_Delay(BLINK_TIME);
	HAL_GPIO_WritePin(GPIOD, Get_Led(5), GPIO_PIN_RESET);
	HAL_Delay(BLINK_TIME);

	HAL_GPIO_WritePin(GPIOE, Get_Led(4), GPIO_PIN_SET);
	HAL_Delay(BLINK_TIME);
	HAL_GPIO_WritePin(GPIOE, Get_Led(4), GPIO_PIN_RESET);
	HAL_Delay(BLINK_TIME);

	for(int i = 3; i>=0; i--){
		HAL_GPIO_WritePin(GPIOC, Get_Led(i), GPIO_PIN_SET);
		HAL_Delay(BLINK_TIME);
		HAL_GPIO_WritePin(GPIOC, Get_Led(i), GPIO_PIN_RESET);
		HAL_Delay(BLINK_TIME);
	}

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(char *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	while(1);
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
