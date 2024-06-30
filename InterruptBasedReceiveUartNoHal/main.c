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
#include "myDefines.h"
#include "usefulfunctions.h"
#include "lpuart.h"
#include "fifo.h"
#include "ctype.h"
#define CHANGE_TIME 1000

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

static struct FIFO RXQueue;

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Display(int number, int failStatus);



void LPUART1_IRQHandler(void){
	char temp = LPUART -> RDR;

	if(!isdigit(temp)) {
		Send_Char('.');
	} else if (FIFO_IsFull(&RXQueue)) {
		Send_Char('x');
	} else {
		temp = temp - 48;
		FIFO_Put(&RXQueue, temp + '0');
		Send_Char(temp + '0');
	}


}
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
  LPUART_Init();
  Diodes_Init();
  Display_Init();
  NVIC_Init();
  FIFO_Init(&RXQueue);

  /* Initialize interrupts */


  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int startTime = HAL_GetTick();
  int currentDisplayNumber = 0;
  int getFailStatus = -1;

  while (1)
  {
    /* USER CODE END WHILE */

	if(HAL_GetTick() - startTime > CHANGE_TIME){
		char tempMain = '0';
		getFailStatus = FIFO_Get(&RXQueue, &tempMain);
		currentDisplayNumber = tempMain - 48;
		startTime = HAL_GetTick();
	}

	Display(currentDisplayNumber, getFailStatus);

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void Display(int number, int failStatus){
	GPIO_B -> ODR &= ~(DIG(4));
	if(failStatus != -1){
		GPIO_G -> ODR &= ~(ALL_SEGMENTS);
		GPIO_G -> ODR |= segments[number];
		GPIO_B -> ODR |= DIG(4);
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
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
