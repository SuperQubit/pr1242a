#include "stm32F10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_it.h"
#include "uart.h"
#include "hal.h"
//#include <stdio.h>
//#include <stdlib.h>
	char USART1_RxBuffer[64];
	char USART1_RxCounter=0;
	char USART1_RxCmdTerminator=0;


//------------------- USART1_LowLevel Interface

volatile UART_Reg _Reg;

ErrorStatus USART1_GetData(UART_Reg *reg)
{
	if(reg)
	{
			*reg=_Reg;
		
			return(SUCCESS);
	}
	else
	{
			return(ERROR);
	}
		
}

uint16_t USART1_GetChar(void)
{		
		return(_Reg.data);
}

void USART1_SendData(uint16_t Data)
{
	HAL_COM->DR = (Data & (uint16_t)0x01FF);	
}

void USART1_ITXEConfig(FunctionalState val)
{
	USART_ITConfig(HAL_COM, USART_IT_TXE, val);	
}

void (*_USART1_RX_Event)(void);
void (*_USART1_TX_Event)(void);

void USART1_IRQHandler(void)
{
	 //Rx
	 if (USART_GetITStatus(HAL_COM, USART_IT_RXNE) != RESET)
   {
     /* Send the received data to the PC Host*/
		 _Reg.status=(HAL_COM->SR);
		 HAL_COM->SR=0;
		 _Reg.data=USART_ReceiveData(HAL_COM);		 		
		 
		 if( _USART1_RX_Event)	_USART1_RX_Event();
		 
		 USART_ClearITPendingBit(HAL_COM, USART_IT_RXNE);		
   }

 	 /* If overrun condition occurs, clear the ORE flag and recover communication */
   if (USART_GetFlagStatus(HAL_COM, USART_FLAG_ORE) != RESET)
   {
     (void)USART_ReceiveData(HAL_COM);
     USART_ClearITPendingBit(HAL_COM, USART_IT_RXNE);
   }		
	 
	 //buffer hardware TX vuoto
   if(USART_GetITStatus(HAL_COM, USART_IT_TXE) != RESET)
   {   
				if( _USART1_TX_Event)	_USART1_TX_Event();
				USART_ClearITPendingBit(HAL_COM, USART_IT_TXE);
   }
}
 
void USART1_SetEventHandler(void (*RXHandler)(void) ,void (*TXHandler)(void) )
{
	  __disable_irq();
		_USART1_RX_Event=RXHandler;
		_USART1_TX_Event=TXHandler;
	 __enable_irq();
}

//------------------ USART1 Initialization
 void UART_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USARTx1Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
  
 /* Configure the GPIO ports */
void UART_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure USART1 Tx alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART1 Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void USART1_Init(void)
{
 
	USART_InitTypeDef USART_InitStructure;
	
	/* Enable USART1 clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* Enable GPIO and AFIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE);	
	
	UART_NVIC_Configuration();		
	UART_GPIO_Configuration();
	
  /* USART1 configuration ------------------------------------------------------*/
  /* USART1 configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control enabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
	USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);
  /* Enable the USART2 */
  USART_Cmd(USART1, ENABLE);	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
}

/**/


char USART1_SendChar (char ch) 
{  
		USART_SendData(USART1, (ch & 0xFF));
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		return(USART1->DR & 0xFF);

}

void USART1_SendStr(char *str)
{
	while(*str)
		{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1, *str++);
		}
}


void USART1_SendCRLF(void)
{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1, '\r');
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);   
		USART_SendData(USART1, '\n');
}


char USART1_RxCmdTermCheck(void)
{
 
	char tmp;
	if(USART1_RxCmdTerminator) tmp=1; else tmp=0;
	USART1_RxCmdTerminator=0;
	return(tmp); 
	
  
}


void USART1_ClearAllBuffer(void)
{
  uint8_t i;

  for (i=0;i <64;i++)
  { 
    USART1_RxBuffer[i]=0;
 }

}


void USART1_CommandReset(void)
{
  USART1_ClearAllBuffer();
	USART1_RxCounter = 0;
	USART1_RxCmdTerminator=0;
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		

}

void USART1_SendStr_Echo(void)
{
	char * str;
	str=USART1_RxBuffer;
	
	while(*str)
		{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1, *str++);
		}
	 USART1_RxCounter = 0;
   USART1_ClearAllBuffer();
		//Riabilito Interrupt
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		
}
