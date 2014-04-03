#include "stm32F10x.h"
#include "ClkGen.h"

uint16_t ClkGen_Counter;
uint16_t ClkGen_CounterLast;
uint16_t ClkGen_CounterPoly;

void ClkGen_Init (void)
{
  ClkGen_Counter=0;
  ClkGen_CounterLast=0;
  ClkGen_CounterPoly=0; 
}

void ClkGen_Clk(void)
{
    ClkGen_Counter++;		
}

void ClkGen_Ack(void)
{
	__disable_irq();
  ClkGen_CounterPoly = (ClkGen_Counter ^ ClkGen_CounterLast);
  ClkGen_CounterLast = ClkGen_Counter;
  __enable_irq();
}
