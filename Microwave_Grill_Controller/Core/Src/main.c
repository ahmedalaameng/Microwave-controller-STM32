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
uint32_t uwPrescalerValue = 0;
/* USER CODE BEGIN PV */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
static const uint8_t keypad_matrix[]=
{

	'/', '*', '-', '+',
	'=', '9', '6', '3',
	'0', '8', '5', '2',
	'o', '7', '4', '1',

	/*'7', '8', '9', '/',
	'4', '5', '6', '*',
	'1', '2', '3', '-',
	'o', '0', '=', '+'*/

};
uint8_t keypad_scan()
{
	//PORTC=0b11111111;
	/*GPIO_TypeDef *GPIOx;
	GPIOx = GPIOA;
	GPIOx->ODR = 0b1111;*/
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
	uint8_t i;
	for (i=0;i<4;i++)
	{
		//CLRBIT(PORTC,i);
		HAL_Delay(1);
		//HAL_GPIO_WritePin(GPIOA,i,GPIO_PIN_RESET);
		if(i==0)
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
		if(i==1)
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
		if(i==2)
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
		if(i==3)
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==0) return keypad_matrix[0 + (i*4)];
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==0) return keypad_matrix[1 + (i*4)];
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0) return keypad_matrix[2 + (i*4)];
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==0) return keypad_matrix[3 + (i*4)];
		/*if (READBIT(PINC,4) == 0) return keypad_matrix[0 + (i*4)];
		if (READBIT(PINC,5) == 0) return keypad_matrix[1 + (i*4)];
		if (READBIT(PINC,6) == 0) return keypad_matrix[2 + (i*4)];
		if (READBIT(PINC,7) == 0) return keypad_matrix[3 + (i*4)];*/
		//SETBIT(PORTC,i);
		if(i==0)
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
		if(i==1)
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
		if(i==2)
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
		if(i==3)
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
		//HAL_GPIO_WritePin(GPIOA,i,GPIO_PIN_SET);
		//HAL_Delay(5);
	}
	return 0;
}
void lcd_init(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);

	//CLRBIT(PORTB, 2);	// RW = 0	write

	lcd_cmd(0X02);		// 4-bit mode
	lcd_cmd(0X28);		// 4-bit mode
	lcd_cmd(0X01);		// clear LCD
	lcd_cmd(0X0C);		// cursor : off
	lcd_cmd(0X06);		// cursor increment : to right
	lcd_cmd(0X80);		// set cursor at the beginning of line1
	//_delay_ms(20);
	HAL_Delay(20);
}

void lcd_cmd(uint8_t cmd)
{
	//CLRBIT(PORTB, 1);		// RS = 0	command
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	//PORTA &= 0X0F;			// clear data-pins
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
	//PORTA |= (cmd & 0XF0);	// write MSBs only
	GPIO_TypeDef *GPIOx;
	GPIOx = GPIOB;
	GPIOx->ODR |= (cmd & 0XF0);
	//GPIOx->BSRR |= (uint32_t)(cmd & 0XF0);
	// Enable
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_Delay(1);
	/*SETBIT(PORTB, 3);
	_delay_ms(1);
	CLRBIT(PORTB, 3);
	_delay_ms(1);*/
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);

	GPIOx->ODR |= (cmd << 4);
	//GPIOx->BSRR |= (uint32_t)(cmd << 4);
	//PORTA &= 0X0F;			// clear data-pins
	//PORTA |= (cmd << 4);	// write LSBs only
	// Enable
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_Delay(1);
	/*SETBIT(PORTB, 3);
	_delay_ms(1);
	CLRBIT(PORTB, 3);
	_delay_ms(1);*/
}

void lcd_write_data(uint8_t data)
{
	//SETBIT(PORTB, 1);		// RS = 1	data
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);

	//PORTA &= 0X0F;			// clear data-pins
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);

	//PORTA |= (data & 0XF0);	// write MSBs only
	GPIO_TypeDef *GPIOx;
	GPIOx = GPIOB;
	GPIOx->ODR |= (data & 0XF0);
	//GPIOx->BSRR |= (uint32_t)(data & 0XF0);
	// Enable
	/*SETBIT(PORTB, 3);
	_delay_ms(1);
	CLRBIT(PORTB, 3);
	_delay_ms(1);*/
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_Delay(1);

	/*PORTA &= 0X0F;			// clear data-pins
	PORTA |= (data << 4);	// write LSBs only*/
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);

	GPIOx->ODR |= (data << 4);
	//GPIOx->BSRR |= (uint32_t)(data << 4);
	// Enable
	/*SETBIT(PORTB, 3);
	_delay_ms(1);
	CLRBIT(PORTB, 3);
	_delay_ms(1);*/
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_Delay(1);
}

void lcd_write_string(uint8_t *string)
{
	uint8_t i = 0;
	while (*(string + i) != '\0')
	{
		lcd_write_data(*(string + i));
		i++;
	}
}

void lcd_write_number(uint16_t number)
{
	uint8_t l = 0;
	uint16_t temp = number;
	do
	{
		l++;
		temp /= 10;
	}
	while (temp != 0);
	// temp = 0 now
	while (number != 0)
	{
		temp *= 10;
		temp += number % 10;
		number /= 10;
	}
	while (l != 0)
	{
		lcd_write_data((temp % 10) + 48);
		temp /= 10;
		l--;
	}
}
uint8_t arr[4]={0,0,0,0},i=0;
uint8_t flag=0;
uint8_t seconds = 0;
uint8_t minutes = 0;
void delay(){
	if(flag==0){
		TIM2->CR1 |= (1 << 0);
		flag=1;
		while(flag==1);
	}
}
void beeb(){
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
}
void ready(){
    beeb();
    HAL_Delay(25);
    beeb();
    HAL_Delay(25);
    beeb();
    lcd_cmd(0x01);
    reset();
    while(1){
        lcd_cmd(0x80);
        lcd_write_string("Ready!");
        if(keypad_scan()){
            beeb();
            break;
        }

    }
}
uint8_t stop(){
    while(1){
        if(keypad_scan()=='o'){
            beeb();
            HAL_Delay(5);
            beeb();
            reset();
            return 1;
        } else if(keypad_scan()=='/'){
            beeb();
            return 0;
        }
    }
}
void reset(){
    uint8_t j;
    i=0;
    for(j=0;j<4;j++)
        arr[j]=0;
}
uint8_t start(){

	seconds = (arr[2]*10) + arr[3];
	minutes = (arr[0]*10) + arr[1];
    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_write_string("START");
    lcd_cmd(0x86);
    lcd_write_string("MM:SS");
    while(minutes>=0){
        while(seconds>0){
            lcd_cmd(0xC6);
            if(minutes<10)
                lcd_write_number(0);
            lcd_write_number(minutes);
            lcd_write_data(':');
            if(seconds<10)
                lcd_write_number(0);
            lcd_write_number(seconds);
            delay();
            seconds--;
            if(keypad_scan()=='*'){
                beeb();
                if(stop())
                    return 1;
                else
                    continue;
            }else if(keypad_scan()=='o'){
            	beeb();
            	reset();
            	return 1;
            }
        }
        if(minutes>0){
        	minutes--;
			seconds=59;
        }
        if(minutes == 0 && seconds==0)
        	break;

    }

    ready();
    return 0;
}
void home(){
    lcd_cmd(0x80);
    lcd_write_string("* Press *");
    lcd_cmd(0xc0);
    lcd_write_string("Micro or Grill");
}
void micro_time(){
    uint8_t data;
    lcd_cmd(0x01);
        while(1){
            lcd_cmd(0x80);
            lcd_write_string("MICRO");
            lcd_cmd(0x86);
            lcd_write_string("MM:SS");
            lcd_cmd(0xC6);
            lcd_write_number(arr[0]);
            lcd_write_number(arr[1]);
            lcd_write_data(':');
            lcd_write_number(arr[2]);
            lcd_write_number(arr[3]);
            if((data = keypad_scan()) && i<4 && data>=48 && data<=57){
                HAL_Delay(200);
                arr[i]=data-48;
                beeb();
                i++;
            }
            if(keypad_scan() == '*'){
                beeb();
                lcd_cmd(0x01);
                break;
            } else if(keypad_scan() == '/'){
                beeb();
                if(start()==0){
                    break;
                }
            }
        }
}
void grill_time(){
uint8_t data;
    lcd_cmd(0x01);
        while(1){
            lcd_cmd(0x80);
            lcd_write_string("GRILL");
            lcd_cmd(0x86);
            lcd_write_string("MM:SS");
            lcd_cmd(0xC6);
            lcd_write_number(arr[0]);
            lcd_write_number(arr[1]);
            lcd_write_data(':');
            lcd_write_number(arr[2]);
            lcd_write_number(arr[3]);
            if((data = keypad_scan()) && i<4 && data>=48 && data<=57){
            	HAL_Delay(200);
                arr[i]=data-48;
                beeb();
                i++;
            }
            if(keypad_scan() == '*'){
                beeb();
                lcd_cmd(0x01);
                break;
            } else if(keypad_scan() == '/'){
                beeb();
                if(start()==0){
                    break;
                }
            }
        }
}
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

  TIM2->CR1 |= (1 << 3);
  TIM2->CR1 &= (0 << 0);
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  	 lcd_init();
  	 uint8_t data;

  	 while(1) {
  		 home();
  		 data = keypad_scan();
  		 if(data == '+'){
  			 beeb();
  			 micro_time();
  		 } else if(data == '-'){
  			 beeb();
  			 grill_time();
  		 } else if(data == '*'){
  			 home();
  		 }
  		//HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
  		//delay();
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
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	//if(minutes>=0){

		flag=0;
	//}

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
  uwPrescalerValue = (uint32_t)(SystemCoreClock / 10000) - 1;
  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = uwPrescalerValue;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10000-1;
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

  if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK)
  {
  		  /* Starting Error */
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
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA8 PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4 PB5 PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
