#include "hal.h"  	/*C'è il PWM per BIAS VBD1 -> PB.7 e VBD2 -> PB.8*/
#include "supply.h"


void SUPPLY_ON()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
}

void SUPPLY_OFF()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
}

void SUPPLY_FANON()
{
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void SUPPLY_FANOFF()
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void SUPPLY_SetValue(uint16_t val)
{		
	if(val < 4095) 	SUPPLY_PWMVALUE=val;
		else SUPPLY_PWMVALUE=4095;	
}


void SUPPLY_Init(void)
{
	
				GPIO_InitTypeDef GPIO_InitStructure;
	
  /* Used peripherals clock enable -------------------------------------------*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
  /* configure PC8 as output push-pull  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	
	 /* Used peripherals clock enable -------------------------------------------*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	 /* configure PC8 as output push-pull  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	
	
	SUPPLY_ON();
	SUPPLY_FANOFF();

}

void SUPPLY_Clk(void)
{


}
	


