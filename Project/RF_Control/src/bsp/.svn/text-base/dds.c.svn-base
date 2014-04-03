#include <stdint.h>
#include "hal.h"
#include "dds.h"


void DDS_FSELECT_Pin_Config(void)
{
	
 GPIO_InitTypeDef GPIO_InitStructure;
	
	 /* Used peripherals clock enable -------------------------------------------*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	 /* configure  as output push-pull  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
	
}


void DDS_SPREAD_Pin_Config(void)
{
	
 GPIO_InitTypeDef GPIO_InitStructure;
	
	 /* Used peripherals clock enable -------------------------------------------*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	 /* configure  as output push-pull  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);
	
}

void DDS_SCALE_Pin_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	 /* Used peripherals clock enable -------------------------------------------*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	 /* configure PC8 as output push-pull  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	
}


void DDS_FRQ_Set(uint16_t val)
{
		
	SPI1_Freq_Reg = val;
	FAST_ADC_SetTCFromFrequency(val);
}



void DDS_ADJ_Update(uint16_t val)
{
		
	if(val < 4095) 	TIM4_CH4_DUTY=val;
		else TIM4_CH4_DUTY=4095;
		
	TIM4->CCR4 = TIM4_CH4_DUTY;	
}

char DDS_AmplitudeSet(uint16_t val)
{
	if(val < 4095)
	{
		TIM4_CH4_DUTY=val;
		return(1);
	}
	else
	{	
		TIM4_CH4_DUTY=4095;
		return(0);
	}
}

uint16_t DDS_AmplitudeGet(void)
{
		//TIM4->CCR4 = TIM4_CH4_DUTY;
		return(TIM4_CH4_DUTY);
}

void DDS_Init(void)
{
		
	SPI1_Init();
	TIM4_PWM_Init();
	DDS_SPREAD_Pin_Config();
	DDS_SCALE_Pin_Config();
	DDS_FSELECT_Pin_Config();
}


void DDS_Clk(void)
{	
	SPI1_Clk();
	TIM4_PWM_Clk();
}


