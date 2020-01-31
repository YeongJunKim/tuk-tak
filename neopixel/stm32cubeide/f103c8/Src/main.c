/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "ws2812.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SHOOTER 			0
#define SHOULDER 			1
#define CHEST 				2
#define TEST_CASE1 			3 //blinking
#define TEST_CASE2 			4 //smoothing
#define TEST_CASE3 			5 //circle
#define TEST_CASE4 			6 //turn on
#define TEST_SHOOTER 		7
#define TEST_BLUETOOTH 		8
#define DEVICE SHOULDER

#define BLINK 0
#define SMOOTH 1
#define CIRCLE 2
#define SHOOTER_SERVO_MAX 1500
#define SHOOTER_SERVO_MIN 1100

#define SHOULDER_SERVO_MAX 1900
#define SHOULDER_SERVO_MIN 1100

#define HEAD_SERVO_MAX 1900
#define HEAD_SERVO_MIN 1100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
DMA_HandleTypeDef hdma_tim1_ch1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void run_led_sequence(uint32_t count, uint8_t type);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t rcvData[10] = {0,};
uint8_t sendData[10] = {0,};

uint32_t step = 0;
uint8_t taskFlag1 = 0;
uint8_t taskFlag2 = 0;
uint32_t taskCount1 = 0;
uint32_t taskCount2 = 0;
uint8_t values = 0;
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
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  uint32_t nowTick = 0;
  uint32_t pastTick = 0;
  ws2812Begin(8);

  HAL_UART_Receive_IT(&huart1, rcvData, 1);

//  HAL_TIMEx_PWMN_Start(&htim2, TIM_CHANNEL_1);
//  HAL_TIMEx_PWMN_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

  htim2.Instance->CCR1 = 110;
  htim2.Instance->CCR2 = 170;


#if(DEVICE == SHOULDER)	//have to init sequence
  htim2.Instance->CCR4 = 110;
  HAL_Delay(5000);
  htim2.Instance->CCR2 = 110;
  htim2.Instance->CCR3 = 110;
  HAL_Delay(5000);
#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  nowTick = HAL_GetTick();
	  HAL_UART_Receive_IT(&huart1, rcvData, 1);
#if(DEVICE == SHOOTER)
	  if(taskFlag1 == 0x02)
	  {
		  if(nowTick - pastTick > 50)
		  {
			  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
			  step++;
			  if(step < 5)	//10sec
			  {
				  taskCount1 = 0;
				  htim2.Instance->CCR1 = 113;
			  }
			  else if(step < 250)	//25sec - 12.5sec
			  {
				  taskCount1 = 0;
				  htim2.Instance->CCR1 = 166;
			  }
			  else if(step < 400)	//32.5sec - 12.5sec
			  {
				  taskCount1++;
				  htim2.Instance->CCR1 = 166;
				  run_led_sequence(taskCount1 , BLINK);
			  }
			  else if(step < 550)	//40sec - 12.5sec
			  {
				  taskCount1 = 0;
				  htim2.Instance->CCR1 = 113;

				  for(uint32_t i  = 0; i < 28 ; i++)
				  {
					  ws2812SetColor(i, 0, 0, 0);
				  }
			  }
			  else if(step < 600)	//42.5sec - 12.5sec
			  {
				  taskCount1 = 0;
				  step = 0;
				  taskFlag1 = 0;
			  }
			  pastTick = nowTick;
		  }
	  }
	  else
	  {
		  if(nowTick - pastTick > 50)
		  {

			  for(uint32_t i  = 0; i < 28 ; i++)
			  {
				  ws2812SetColor(i, 0, 0, 0);
			  }
			  pastTick = nowTick;
		  }
	  }
#elif(DEVICE == SHOULDER)
	  if(nowTick - pastTick > 50)
	  {
		  if(taskFlag1 || taskFlag2)
		  {
			  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
			  if(taskFlag1)				//head
			  {
				  taskCount1++;
				  if(taskCount1 < 300)
				  {
					  // open linear servo head
					  htim2.Instance->CCR1 = 190;
				  }
				  else if(taskCount1 < 600)
				  {
					  // close linear servo head
					  htim2.Instance->CCR1 = 110;
				  }
				  else if(taskCount1 < 700)
				  {
					  // init head
					  htim2.Instance->CCR1 = 110;
					  taskCount1 = 0;
					  taskFlag1 = 0;
				  }
			  }
			  else
			  {
				  // init head
				  htim2.Instance->CCR1 = 110;
				  taskCount1 = 0;
				  taskFlag1 = 0;
			  }

			  if(taskFlag2)				//shoulder
			  {
				  taskCount2++;

				  if(taskCount2 < 150)				//50 * 150 ms
				  {
					  //open angular servo shoulder
					  ws2812ClearColor();
					  htim2.Instance->CCR2 = 110;	//1100us
					  htim2.Instance->CCR3 = 110;	//1100us
					  htim2.Instance->CCR4 = 110;
				  }
				  else if(taskCount2 < 600)			//50 * 600 ms
				  {
					  //open linear servo shooter
					  htim2.Instance->CCR2 = 130;
					  htim2.Instance->CCR3 = 130;
					  htim2.Instance->CCR4 = 160;
					  ws2812ClearColor();
				  }
				  else if(taskCount2 < 800)
				  {
					  //shooting
					  htim2.Instance->CCR2 = 140;
					  htim2.Instance->CCR3 = 140;
					  htim2.Instance->CCR4 = 110;
					  run_led_sequence(taskCount2, BLINK);
				  }
				  else if(taskCount2 < 1000)
				  {
					  //close linear servo shooter
					  htim2.Instance->CCR2 = 150;
					  htim2.Instance->CCR3 = 150;
					  htim2.Instance->CCR4 = 160;
					  ws2812ClearColor();
				  }
				  else if(taskCount2 < 1200)
				  {
					  //close angular servo shoulder
					  htim2.Instance->CCR2 = 110;
					  htim2.Instance->CCR3 = 110;
					  htim2.Instance->CCR4 = 180;
					  ws2812ClearColor();
					  //init head
					  taskCount2 = 0;
					  taskFlag2 = 0;
				  }
			  }
			  else
			  {
				  //close angular servo shoulder
				  htim2.Instance->CCR2 = 110;
				  htim2.Instance->CCR3 = 110;
				  ws2812ClearColor();
				  //init head
				  taskCount2 = 0;
				  taskFlag2 = 0;
			  }
		  }
		  else
		  {
			  // head init
			  // TODO do they move sequence?
			  // SHOULDER init
			  // TODO do they move sequence?
		  }
		  pastTick = nowTick;
	  }
#elif(DEVICE == CHEST)
	  {
		  if(nowTick - pastTick > 20)
		  {
			  if(step == 0)
			  {
				  taskCount1++;
				  run_led_sequence(taskCount1, SMOOTH);
				  if(taskCount1 == 100)
					  step = 1;
			  }
			  else if(step == 1)
			  {
				  taskCount1--;
				  run_led_sequence(taskCount1, SMOOTH);
				  if(taskCount1 == 0)
					  step = 2;
			  }
			  else if(step == 2)
			  {
				  step = 0;
				  taskCount1 = 0;
				  run_led_sequence(taskCount1, SMOOTH);
			  }

			  pastTick = nowTick;
		  }
	  }
#elif(DEVICE == TEST_CASE1)

	  if(nowTick - pastTick > 50)
	  {
		  taskCount1++;
		  run_led_sequence(taskCount1 , BLINK);
		  pastTick = nowTick;
	  }

#elif(DEVICE == TEST_CASE2)
	  if(nowTick - pastTick > 50)
	  {
		  taskCount1++;
		  values ++;
		  for(uint32_t i = 0 ; i < 28; i++)
		  {
			  ws2812SetColor(i, values, values, values);
		  }
		  if(values > 100)
			  values = 0;

		  pastTick = nowTick;
	  }
#elif(DEVICE == TEST_CASE3)
	  if(nowTick - pastTick > 10)
	  {
		  taskCount1++;
		  run_led_sequence(taskCount1, CIRCLE);
		  if(taskCount1 == 23)
		  {
			  taskCount1 = 0;
		  }
		  pastTick = nowTick;
	  }
#elif(DEVICE == TEST_CASE4)
	  if(nowTick - pastTick > 50)
	  {
		  for(uint32_t i  = 0; i < 28 ; i++)
		  {
			  ws2812SetColor(i, 120, 120, 120);
		  }
	  }
#elif(DEVICE == TEST_SHOOTER)
	  if(nowTick - pastTick > 50)
	  {
		  step++;
		  if(step < 200)
		  {
			  taskCount1=0;
			  htim2.Instance->CCR1 = 113;
		  }
		  else if(step < 500)
		  {
			  taskCount1=0;
			  htim2.Instance->CCR1 = 166;
		  }
		  else if(step < 650)
		  {
			  taskCount1++;
			  htim2.Instance->CCR1 = 166;
			  run_led_sequence(taskCount1 , BLINK);
		  }
		  else if(step > 700)
		  {
			  taskCount1=0;
			  step = 0;
			  htim2.Instance->CCR1 = 113;

			  for(uint32_t i  = 0; i < 28 ; i++)
			  {
				  ws2812SetColor(i, 0, 0, 0);
			  }
		  }
		  pastTick = nowTick;
	  }
#else
	  if(nowTick - pastTick > 50)
	  {
		  if(step == 1)
		  //for(uint32_t i = 0; i < 17; i ++)
		  {
			    sendData[0] = 0xFF;
			    HAL_UART_Transmit(&huart1, sendData, 1, 0xFFFF);
				//ws2812SetColor(i, 255, 255, 255);
		  }
		  if(step == 2)
		  //for(uint32_t i = 0; i < 17; i ++)
		  {
			    sendData[0] = 0x00;
			    HAL_UART_Transmit(&huart1, sendData, 1, 0xFFFF);
				//ws2812SetColor(i, 0, 0, 0);
		  }
		  if(step == 3)
		  {
			    sendData[0] = 0x01;
			    HAL_UART_Transmit(&huart1, sendData, 1, 0xFFFF);
				//ws2812SetColor(i, 255, 255, 255);
		  }
		  if(step == 3)
		  {
			    sendData[0] = 0x02;
			    HAL_UART_Transmit(&huart1, sendData, 1, 0xFFFF);
				//ws2812SetColor(i, 255, 255, 255);
		  }
		  if(step == 4)
		  {
			    sendData[0] = 0x80;
			    HAL_UART_Transmit(&huart1, sendData, 1, 0xFFFF);
		  }
		  if(step == 5)
		  {
			    sendData[0] = 0x82;
			    HAL_UART_Transmit(&huart1, sendData, 1, 0xFFFF);
		  }

			step++;
			if(step == 6)
				step = 1;
		  pastTick = nowTick;
	  }

#endif
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 93;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

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

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 720;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 20000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

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
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
#if(DEVICE == SHOOTER)
			{
				if(rcvData[0] == 0b00000010)
				{
					taskFlag1 = 0b00000010;
				}
			}
#elif(DEVICE == SHOULDER)
			{
				if(rcvData[0] & 0b10000000)
				{
					taskFlag2 = 1;
				}
				else if(rcvData[0] & 0b00000010)
				{
					taskFlag1 = 1;
				}
			}
#elif(DEVICE == CHEST)
			{
				if(rcvData[0] != 0)
				{
					taskFlag1 = 1;
				}
			}
#elif(DEVICE == TEST_CASE1)
			{

			}
#elif(DEVICE == TEST_CASE2)
			{

			}
#elif(DEVICE == TEST_CASE3)
			{

			}
#elif(DEVICE == TEST_BLUETOOTH)
			{
				if(rcvData[0] == 0xFF)
				{
					  for(uint32_t i = 0; i < 17; i ++)
					  {
							ws2812SetColor(i, 255, 255, 255);
					  }
				}
				else
				{
					  for(uint32_t i = 0; i < 17; i ++)
					  {
							ws2812SetColor(i, 0, 0, 0);
					  }
				}
			}
#endif


	}
}

void run_led_sequence(uint32_t count, uint8_t type)
{
	if(type == BLINK)
	{
		if(count%2 == 0)
		{
			  for(uint32_t i = 0; i < 28; i ++)
			  {
					ws2812SetColor(i, 0, 0, 0);
			  }
		}
		else
		{
			  for(uint32_t i = 0; i < 28; i ++)
			  {
					ws2812SetColor(i, (uint8_t)255, (uint8_t)255, (uint8_t)255);
			  }
		}
	}

	else if(type == SMOOTH)
	{
		uint8_t value = (uint8_t)count;
		for(uint32_t i = 0; i < 28; i ++)
		{
			ws2812SetColor(i, value, value, value);
		}
	}
	else if(type == CIRCLE)
	{
		for(uint32_t i = 0; i < 24; i ++)
		{
			if(i == count)
				ws2812SetColor(i, 255, 255, 255);
			else
				ws2812SetColor(i, 0, 0, 0);
		}
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
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
