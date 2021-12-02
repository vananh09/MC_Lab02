/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
void update7SEG(int index);
void updateClockBuffer();
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t segmentNumber[10] = {
		0x40,  // 0
		0x79,  // 1
		0x24,  // 2
		0x30,  // 3
		0x19,  // 4
		0x12,  // 5
		0x02,  // 6
		0x78,  // 7
		0x00,  // 8
		0x10   // 9
};

void display7SEG(uint8_t number){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, ((number>>0)&0x01));
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, ((number>>1)&0x01));
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, ((number>>2)&0x01));
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, ((number>>3)&0x01));
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, ((number>>4)&0x01));
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, ((number>>5)&0x01));
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, ((number>>6)&0x01));
}

// uint8_t number to display A character
uint8_t matrixLedCol[8] = {
		0xE7,0xC3,0x99,0x99,0x81,0x81,0x99,0x99
};
// storage uint8_t number
uint8_t matrix_led_buffer[8] = {
		0xE7,0xC3,0x99,0x99,0x81,0x81,0x99,0x99
};
void displayMatrixLed(uint8_t numberCol) {
	//display col from row selected
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, !(numberCol&0x80));
	numberCol = numberCol << 1;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, !(numberCol&0x80));
	numberCol = numberCol << 1;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, !(numberCol&0x80));
	numberCol = numberCol << 1;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, !(numberCol&0x80));
	numberCol = numberCol << 1;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, !(numberCol&0x80));
	numberCol = numberCol << 1;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, !(numberCol&0x80));
	numberCol = numberCol << 1;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, !(numberCol&0x80));
	numberCol = numberCol << 1;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, !(numberCol&0x80));
}
void updateLEDMatrix (int index) {
	switch ( index ) {
		case 0:
			displayMatrixLed(matrixLedCol[0]);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|
					 	 	 	 	 GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_9, 1);
			break ;
		case 1:
			displayMatrixLed(matrixLedCol[1]);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 0);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|
								 	 GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8, 1);
			break ;
		case 2:
			displayMatrixLed(matrixLedCol[2]);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|
								 	 GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8|GPIO_PIN_9, 1);
			break ;
		case 3:
			displayMatrixLed(matrixLedCol[3]);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, 0);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_13|
								 	 GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8|GPIO_PIN_9, 1);
			break ;
		case 4:
			displayMatrixLed(matrixLedCol[4]);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 0);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_13|
								 	 GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8|GPIO_PIN_9, 1);
			break ;
		case 5:
			displayMatrixLed(matrixLedCol[5]);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|
								 	 GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8|GPIO_PIN_9, 1);
			break ;
		case 6:
			displayMatrixLed(matrixLedCol[6]);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|
								 	 GPIO_PIN_15|GPIO_PIN_8|GPIO_PIN_9, 1);
			break ;
		case 7:
			displayMatrixLed(matrixLedCol[7]);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|
								 	 GPIO_PIN_14|GPIO_PIN_8|GPIO_PIN_9, 1);
			break ;
		default :
			break ;
	}
}
void MatrixAnimation(int bit) {
	matrixLedCol[0] = (matrix_led_buffer[0]>>bit)|(matrix_led_buffer[0]<<(8-bit));
	matrixLedCol[1] = (matrix_led_buffer[1]>>bit)|(matrix_led_buffer[1]<<(8-bit));
	matrixLedCol[2] = (matrix_led_buffer[2]>>bit)|(matrix_led_buffer[2]<<(8-bit));
	matrixLedCol[3] = (matrix_led_buffer[3]>>bit)|(matrix_led_buffer[3]<<(8-bit));
	matrixLedCol[4] = (matrix_led_buffer[4]>>bit)|(matrix_led_buffer[4]<<(8-bit));
	matrixLedCol[5] = (matrix_led_buffer[5]>>bit)|(matrix_led_buffer[5]<<(8-bit));
	matrixLedCol[6] = (matrix_led_buffer[6]>>bit)|(matrix_led_buffer[6]<<(8-bit));
	matrixLedCol[7] = (matrix_led_buffer[7]>>bit)|(matrix_led_buffer[7]<<(8-bit));
}
int timer0_counter = 0;
int timer0_flag = 0;
int timer1_counter = 0;
int timer1_flag = 0;
int timer2_counter = 0;
int timer2_flag = 0;
int TIMER_CYCLE = 10;
const int MAX_LED = 4;
int index_led = 0;
int index_led_matrix = 0;
int led_buffer [4] = {1 , 2 , 3 , 4};
int shirt_bit = 0;
//timer for led
void setTimer0(int duration) {
	timer0_counter = duration / TIMER_CYCLE;
	timer0_flag = 0;
}
void timer0_run() {
	if( timer0_counter > 0) {
		timer0_counter--;
		if( timer0_counter == 0) timer0_flag = 1;
	}
}
//timer for led7seg
void setTimer1(int duration) {
	timer1_counter = duration / TIMER_CYCLE;
	timer1_flag = 0;
}
void timer1_run() {
	if( timer1_counter > 0) {
		timer1_counter--;
		if( timer1_counter == 0) timer1_flag = 1;
	}
}
//timer for matrixled
void setTimer2(int duration) {
	timer2_counter = duration / TIMER_CYCLE;
	timer2_flag = 0;
}
void timer2_run() {
	if( timer2_counter > 0) {
		timer2_counter--;
		if( timer2_counter == 0) timer2_flag = 1;
	}
}
int hour = 15 , minute = 8 , second = 50;
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  setTimer0(1000); //set timer led 1s
  setTimer1(250); //set timer seg7led 250ms
  setTimer2(100); //set timer MatrixLed 100ms
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  if(timer0_flag == 1) {
		  setTimer0(1000);
		  second++;
		  if(second >= 60){
			  second = 0;
			  minute ++;
		  }
		  if(minute >= 60){
			  minute = 0;
			  hour ++;
		  }
		  if(hour >= 24){
			  hour = 0;
		  }
		  HAL_GPIO_TogglePin(GPIOA , GPIO_PIN_4);
		  HAL_GPIO_TogglePin(GPIOA , GPIO_PIN_5);
		  updateClockBuffer();
	  }
	  if(timer1_flag == 1) {
		  setTimer1(250);
		  	    switch (index_led){
		          case 0:
		        	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,0);
		        	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,1);
		        	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,1);
		        	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,1);
		        	  update7SEG(index_led);
		              index_led = 1;
		              break;
		          case 1:
		        	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,1);
		        	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,0);
		        	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,1);
		        	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,1);
		        	  update7SEG(index_led);
		        	  index_led = 2;
		              break;
		          case 2:
		        	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,1);
		        	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,1);
		        	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,0);
		        	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,1);
		              update7SEG(index_led);
		              index_led = 3;
		              break;
		          case 3:
		        	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,1);
		        	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,1);
		        	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,1);
		        	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,0);
		              update7SEG(index_led);
		              index_led = 0;
		              break;
		          }
	  }
	  if(timer2_flag == 1) {
		  setTimer2(100);
		  MatrixAnimation(shirt_bit);
		  updateLEDMatrix(index_led_matrix);
		  index_led_matrix++;
		  if(index_led_matrix > 7) {
			  index_led_matrix = 0;
			  shirt_bit++;
			  if(shirt_bit > 7) shirt_bit = 0;
		  }
	  }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA2 PA3 PA4 PA5
                           PA6 PA7 PA8 PA9
                           PA10 PA11 PA12 PA13
                           PA14 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB10
                           PB11 PB12 PB13 PB14
                           PB15 PB3 PB4 PB5
                           PB6 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef * htim) {
	timer0_run();
	timer1_run();
	timer2_run();
}
void update7SEG (int index) {
	switch (index) {
		case 0:
			// Display the first 7 SEG with led_buffer [0]
			display7SEG(segmentNumber[led_buffer[0]]);
			break;
		case 1:
			// Display the second 7 SEG with led_buffer [1]
			display7SEG(segmentNumber[led_buffer[1]]);
			break;
		case 2:
			// Display the third 7 SEG with led_buffer [2]
			display7SEG(segmentNumber[led_buffer[2]]);
			break;
		case 3:
			// Display the forth 7 SEG with led_buffer [3]
			display7SEG(segmentNumber[led_buffer[3]]);
			break;
		default :
			break;
	}
}

void updateClockBuffer() {
	led_buffer[0] = hour / 10;
	led_buffer[1] = hour % 10;
	led_buffer[2] = minute / 10;
	led_buffer[3] = minute % 10;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
