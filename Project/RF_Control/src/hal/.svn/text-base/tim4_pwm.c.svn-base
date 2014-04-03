#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32F10x.h"
#include "stm32f10x_tim.h"
#include "tim4_pwm.h"




#define Period_BootValue  (uint16_t) (4094.0)
#define ARR_BootValue  Period_BootValue

#define CCR1_BootValue  (uint16_t)(800)
#define CCR2_BootValue  (uint16_t)((Period_BootValue))
#define CCR3_BootValue  (uint16_t)((Period_BootValue))
#define CCR4_BootValue  (uint16_t)(3800)
//#define CCR4_BootValue  (uint16_t)(0)



uint16_t PrescalerValue = 0;

void TIM4_Config(void)
{
	 /* -----------------------------------------------------------------------
    TIM4 Configuration: generate 4 PWM signals with 4 different duty cycles:
    The TIM4CLK frequency is set to SystemCoreClock (Hz), to get TIM4 counter
    clock at 24 MHz the Prescaler is computed as following:
     - Prescaler = (TIM4CLK / TIM4 counter clock) - 1
    SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
    and Connectivity line devices and to 24 MHz for Low-Density Value line and
    Medium-Density Value line devices

    The TIM4 is running at 36 KHz: TIM4 Frequency = TIM4 counter clock/(ARR + 1)
                                                  = 24 MHz / 666 = 36 KHz
    TIM4 Channel1 duty cycle = (TIM4_CCR1/ TIM4_ARR)* 100 = 50%
    TIM4 Channel2 duty cycle = (TIM4_CCR2/ TIM4_ARR)* 100 = 37.5%
    TIM4 Channel3 duty cycle = (TIM4_CCR3/ TIM4_ARR)* 100 = 25%
    TIM4 Channel4 duty cycle = (TIM4_CCR4/ TIM4_ARR)* 100 = 12.5%
  ----------------------------------------------------------------------- */
  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	/* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_DeInit(TIM4);
	
	/* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = ARR_BootValue;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */  
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_BootValue;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_BootValue;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_BootValue;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_BootValue;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
  
	TIM_ARRPreloadConfig(TIM4, ENABLE);

  /* TIM4 enable counter */
  TIM_Cmd(TIM4, ENABLE);
	
}



void TIM4_PWM_PORT_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  /*GPIOB Configuration: TIM4 channel 1, 2, 3 and 4 */
  GPIO_InitStructure.GPIO_Pin =  (GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7 |  GPIO_Pin_6) ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOB, &GPIO_InitStructure);

}




uint16_t TIM4_CH1_DUTY=CCR1_BootValue;
uint16_t TIM4_CH2_DUTY=CCR2_BootValue;
uint16_t TIM4_CH3_DUTY=CCR3_BootValue;
uint16_t TIM4_CH4_DUTY=CCR4_BootValue; /*Il valore di questo registro deve valere da 2^12-1 (4095) che rappresenta il 100%*/


void TIM4_PWM_Init(void)

{
	TIM4_Config();
	TIM4_PWM_PORT_Config();
}

void TIM4_PWM_Clk(void)

{
	TIM4->CCR1 = TIM4_CH1_DUTY;
	TIM4->CCR2 = TIM4_CH2_DUTY;
	TIM4->CCR3 = TIM4_CH3_DUTY;
	TIM4->CCR4 = TIM4_CH4_DUTY;

}




