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

#include "lcdtp.h"
#include "xpt2046.h"
#include <time.h>
#include "image.h"
#include "interface.h"
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//Own variables
char charData[80];
int16_t pointX;
int16_t pointY;

//uart variable
char charValue[80];	
uint8_t rx_buffer[256];
uint8_t aRxBuffer;
uint8_t Uart4_Rx_Cnt = 0;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;

SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FSMC_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
static void MX_UART4_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define SYNC 0xAA
#define EXCODE 0x55

typedef struct
{
    uint8_t Brain_Data[36];
    uint8_t signal;
    uint8_t attention;
    uint8_t relax;
    volatile uint8_t receive_flag;
    volatile uint8_t wear_flag;
    volatile uint8_t off_flag;
    volatile uint8_t mode;
    
}Brain_DataTypeDef;

//button function to construct a button on a specific location
void user_pwm_set_frequency(uint16_t frequency){
MX_TIM4_Init();
HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);

}

void tone(int frequency, int time){
user_pwm_set_frequency(frequency);
HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
HAL_Delay(time);
HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
}

void Rickroll(){
HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
HAL_Delay(100);
tone(196,125);
tone(220,125);
tone(261,125);
tone(220,125);
tone(329,325);
HAL_Delay(50);
tone(329,375);
tone(293,750);
HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
}

void mii(){
HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
HAL_Delay(100);
tone(370,375);
HAL_Delay(125);
tone(440,250);
tone(554,500);
tone(440,500);
tone(370,250);
tone(294,188);
HAL_Delay(62);
tone(294,188);
HAL_Delay(62);	
tone(294,188);
HAL_Delay(1562);
	
tone(294,250);
tone(370,250);
tone(440,375);
HAL_Delay(125);
tone(370,250);
HAL_Delay(500);
tone(659,750);
tone(622,250);
tone(587,125);
HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
}

void timerEnds(int mode){
HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
HAL_Delay(100);
for(int i = 0; i<3; i++){
tone(999,100);
HAL_Delay(150);
tone(999,100);
HAL_Delay(650);
}
if(mode==0){
	//Pomodoro timer ends
	Rickroll();
}	

else if(mode == 1){
	//Short break timer ends
	mii();
}
HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
}

//touch2Display function


//testInterface function

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */
 
	if(Uart4_Rx_Cnt >= 255)
	{
		Uart4_Rx_Cnt = 0;
	}
	else
	{
		rx_buffer[Uart4_Rx_Cnt++] = aRxBuffer;
	}
	
	HAL_UART_Receive_IT(&huart4, (uint8_t *)&aRxBuffer, 1);
}

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
  MX_FSMC_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
	macXPT2046_CS_DISABLE();
	
	LCD_INIT();
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);

	//while( ! XPT2046_Touch_Calibrate () );   

	LCD_GramScan ( 1 );

	int pageCounter = 0;
	int timer = 0;
	
	char SignalValue[80];
	char AttentionValue[80];
	char RelaxValue[80];
	char WearFlag[80];
	
	
	Brain_DataTypeDef Brain_DataStruct;
	//init Brain datastruct
	volatile uint16_t over_time_counter = 0;
  Brain_DataStruct.receive_flag = 0;
  Brain_DataStruct.wear_flag = 0;
  Brain_DataStruct.mode = 3;
  HAL_UART_Receive_IT(&huart4,&aRxBuffer,1);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	
  while (1)
  {
    if ( ucXPT2046_TouchFlag == 1 )	         
    {
			Check_touchkey();			
      ucXPT2046_TouchFlag = 0;
			
			
    }	
//		if(myCheck()){
//		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);
//		HAL_Delay(500);
//		if(testInterface()==1){
//		loading();
//		pageCounter = mainMenu();
//		continue;
//		}
//		}
		HAL_Delay(50);		
		
		switch(pageCounter){
			case 0:
				HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);
				pageCounter = mainMenu();
				break;
			
			case 1: 
				loading();
				//timer = timerScreen();
				//pageCounter = timer%10;
				//timer = timer/10;
				pageCounter = timerCount(2500);
				break;
			
			case 2:
				//You can put your while loop here
			  LCD_Clear(0,0,240,340,WHITE);
				while (1)
				{
					LCD_DrawString(0, 0, "Data:");
          LCD_DrawString(0, 32, "small packet:");
					LCD_DrawString(0, 64, "SY");
					LCD_DrawString(20, 64, "SY");
					LCD_DrawString(40, 64, "PL");
					LCD_DrawString(60, 64, "CODE");
					LCD_DrawString(100, 64, "VL");
					LCD_DrawString(120, 64, "V1");
					LCD_DrawString(140, 64, "V2");
					LCD_DrawString(160, 64, "CHKSUM");
					LCD_DrawString(0, 120, "Big packet:");
					sprintf(charValue, "%d", 0);
					
					
					
					for(int i = 0; i<256; i++){
						
						sprintf(charValue,"%02x", rx_buffer[i]);
						LCD_DrawString(100 , 0, charValue);
						
						if(rx_buffer[i]==SYNC && rx_buffer[i+1]==SYNC && rx_buffer[i+2]==0x04) {
							sprintf(charValue,"%02x", rx_buffer[i]);
							LCD_DrawString(0, 96, charValue);
							sprintf(charValue,"%02x", rx_buffer[i+1]);
							LCD_DrawString(20, 96, charValue);
							sprintf(charValue,"%02x", rx_buffer[i+2]);
							LCD_DrawString(40, 96, charValue);
							sprintf(charValue,"%02x", rx_buffer[i+3]);
							LCD_DrawString(60, 96, charValue);
							sprintf(charValue,"%02x", rx_buffer[i+4]);
							LCD_DrawString(100, 96, charValue);
							sprintf(charValue,"%02x", rx_buffer[i+5]);
							LCD_DrawString(120, 96, charValue);
							sprintf(charValue,"%02x", rx_buffer[i+6]);
							LCD_DrawString(140, 96, charValue);
							sprintf(charValue,"%02x", rx_buffer[i+7]);
							LCD_DrawString(160, 96, charValue);
						}
						
						//Check packet header 
						
						if(rx_buffer[i]==SYNC && rx_buffer[i+1]==SYNC && rx_buffer[i+2]==0x20) {
							for (int k=0; k< 13;k++) {
								sprintf(charValue,"%02x", rx_buffer[i+k]);
								LCD_DrawString(0+18*k,232 ,charValue);
							}
							for (int k=13; k< 25;k++) {
								sprintf(charValue,"%02x", rx_buffer[i+k]);
								LCD_DrawString(0+18*(k-13),264 ,charValue);
							}
							for (int k=25; k< 36;k++) {
								sprintf(charValue,"%02x", rx_buffer[i+k]);
								LCD_DrawString(0+18*(k-25),296 ,charValue);
							}

							
							uint16_t checksum = 0;
							for(int n=0;n<32;n++) {
								Brain_DataStruct.Brain_Data[n]  = rx_buffer[i+3+n];
								checksum += Brain_DataStruct.Brain_Data[n];
							}
							checksum = (~checksum)&0xff;
							if (checksum == rx_buffer[i+35]) {
								checksum = 0;
								Brain_DataStruct.signal=Brain_DataStruct.Brain_Data[1];
								Brain_DataStruct.attention=Brain_DataStruct.Brain_Data[29];
								Brain_DataStruct.relax=Brain_DataStruct.Brain_Data[31];
								if ((Brain_DataStruct.signal != 29) && (Brain_DataStruct.signal != 54) &&
											(Brain_DataStruct.signal != 55) && (Brain_DataStruct.signal != 56) &&
											(Brain_DataStruct.signal != 80) && (Brain_DataStruct.signal != 81) &&
											(Brain_DataStruct.signal != 82) && (Brain_DataStruct.signal != 107)&&
											(Brain_DataStruct.signal != 200))
								{
									Brain_DataStruct.wear_flag = 1;
								}
								else
								{
									Brain_DataStruct.wear_flag = 0;
								}
							}
							
							LCD_DrawString(0, 150, "Signal:");
							sprintf(SignalValue,"%02x", Brain_DataStruct.signal);
							LCD_DrawString(120 ,150,SignalValue);
							
							LCD_DrawString(0, 170, "Attention:");
							sprintf(AttentionValue,"%02x", Brain_DataStruct.attention);
							LCD_DrawString(120 ,170 ,AttentionValue);

						  LCD_DrawString(0, 190, "Relax:");
							sprintf(RelaxValue,"%02x", Brain_DataStruct.relax);
							LCD_DrawString(120 ,190 ,RelaxValue);
							
						  LCD_DrawString(100, 120, "Wear:");
							sprintf(WearFlag,"%x", Brain_DataStruct.wear_flag);
							LCD_DrawString(150 ,120 ,WearFlag);
							
							break;
						}
						else {
							continue;
						}
						
						
						
					}
					
					//HAL_Delay(2000);
					
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
				}
				break;
			
			case 5:
				pageCounter = timerCount(2500);
				break;
			
			case 6:
			//Pomodoro timer ends.
				timerEnds(0);
				pageCounter = timerCount(500);
				break;
			
			case 7:
				//short break timer ends.
				timerEnds(1);
				pageCounter = timerCount(2500);
				break;
			
			default:
				pageCounter = 0;
				break;
		}
		
    /* USER CODE END WHILE */

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
  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_1);
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
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1199;
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
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 16;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 234;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_GATED;
  sSlaveConfig.InputTrigger = TIM_TS_ITR1;
  if (HAL_TIM_SlaveConfigSynchronization(&htim4, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 57600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

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
  huart1.Init.BaudRate = 115200;
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2|GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_5, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pins : PE2 PE0 PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PE3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PE4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

/* FSMC initialization function */
static void MX_FSMC_Init(void)
{

  /* USER CODE BEGIN FSMC_Init 0 */

  /* USER CODE END FSMC_Init 0 */

  FSMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FSMC_Init 1 */

  /* USER CODE END FSMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  /* Timing */
  Timing.AddressSetupTime = 15;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 255;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /** Disconnect NADV
  */

  __HAL_AFIO_FSMCNADV_DISCONNECTED();

  /* USER CODE BEGIN FSMC_Init 2 */

  /* USER CODE END FSMC_Init 2 */
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
