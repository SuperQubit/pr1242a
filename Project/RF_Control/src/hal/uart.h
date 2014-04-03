/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_H
#define __UART_H

	extern char USART1_RxBuffer[64];
	extern char USART1_RxCounter;
	extern char USART1_RxCmdTerminator;


typedef struct
{                    
  uint16_t data;
	uint16_t status;
} UART_Reg;

extern void USART1_Init(void) ;

extern void USART1_SetEventHandler (void (*RXHandler)(void)  ,void (*TXHandler)(void) );
extern ErrorStatus USART1_GetData(UART_Reg *reg);
extern uint16_t USART1_GetChar(void);
extern void USART1_SendData(uint16_t Data);
extern void USART1_ITXEConfig(FunctionalState val);

extern char USART1_SendChar(char ch);
extern void USART1_SendCStr(const char *str);
extern void USART1_SendStr(char *str);
extern void USART1_SendCRLF(void);
extern char USART1_RxCmdTermCheck(void);
extern void USART1_ClearAllBuffer(void);
extern void USART1_SendStr_Echo(void);
extern void USART1_CommandReset(void);

#endif 
