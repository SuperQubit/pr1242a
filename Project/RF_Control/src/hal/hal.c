#include "hal.h"


static __IO uint32_t TimingDelay;

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void HAL_Delay(uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void HAL_TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

//--------------------------------------------------------------
//-----------     I2C Power Module
//--------------------------------------------------------------
volatile unsigned char HAL_I2CPM;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes uC peripherals, GPIOs, clocks used by I2CPM.
  * @param  None
  * @retval None
  */



	
//--------------------------------------------------------------
//-----------     HAL Solver
//--------------------------------------------------------------

#ifndef _NVIC_OFFSET_ADDRESS
#define _NVIC_OFFSET_ADDRESS  (0x0000)
#endif



void HAL_GPIO_LedBoard_Config(void)
{
	
  GPIO_InitTypeDef GPIO_InitStructure;
	
	 /* Used peripherals clock enable -------------------------------------------*/
  RCC_APB2PeriphClockCmd(HAL_LED1_PRPH_CLK, ENABLE);
	
	 /* configure PC8 as output push-pull  */
  GPIO_InitStructure.GPIO_Pin = HAL_LED1_PIN | HAL_LED2_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_Init(HAL_LED1_PORT, &GPIO_InitStructure);
	
	GPIO_ResetBits(HAL_LED1_PORT, HAL_LED1_PIN | HAL_LED2_PIN);		
}


void HAL_Init(void)
{
	HAL_Enable_CYCCNT();
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, _NVIC_OFFSET_ADDRESS);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	
  // Initialize and start the SysTick counter and its interrupt.
  // Setup SysTick Timer for 1 msec interrupts vedi l'Handle in stm32f10x_it.c void SysTick_Handler(void)
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }
	
	
	HAL_GPIO_LedBoard_Config();
	
	I2C2_Init();
	DS1086_Init();
	ClkGen_Init();
	//SPI1_Init();
	//SPI2_Init();
	//TIM4_PWM_Init();
	
		RCC_LSICmd(ENABLE);
    /* Wait till LSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET){}
	/* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency dispersion) */
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  /* IWDG counter clock: LSI/32 */
  IWDG_SetPrescaler(IWDG_Prescaler_32);
  /* Set counter reload value to obtain 250ms IWDG TimeOut.
     Counter Reload Value = 250ms/IWDG counter clock period
                          = 250ms / (LSI/32)
                          = 0.25s / (LsiFreq/32)
                          = LsiFreq/(32 * 4)
                          = LsiFreq/128
   */
  IWDG_SetReload(40000/128);
  /* Reload IWDG counter */
  IWDG_ReloadCounter();
  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  //IWDG_Enable();
	
}

void HAL_Clk(void)
{

}

