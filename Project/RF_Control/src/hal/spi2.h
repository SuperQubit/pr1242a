#include "hal.h"
#define SPI2_BufferSize (13)				//Usato sotto Interrupt per riallineare


#define SPI2_PORT        GPIOB



#define SPI2_NSS2        GPIO_Pin_12      // SPI nSS
#define SPI2_SCK         GPIO_Pin_13      // SPI Clock
#define SPI2_MISO        GPIO_Pin_14      // SPI MISO
#define SPI2_MOSI        GPIO_Pin_15      // SPI MOSI


#define SPI2_PERIPH_CLK 	RCC_APB1Periph_SPI2
#define GPIO_PERIPH_CLK 	(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB)


extern void SPI2_SetEventHandler(void (*RXHandler)(void) ,void (*TXHandler)(void) );
extern void SPI2_SetPacketSize(uint8_t psize);
extern void SPI2_EventHandler(void);
extern void SPI2_Init(void);
extern void SPI2_Clk(void);

extern volatile uint8_t SPI2_TimeOut;		//Azzerato sotto interrupt
extern volatile uint8_t RxIdx;						//Usato sotto interrupt

extern volatile uint8_t SPI2_Buffer_Rx[SPI2_BufferSize];
extern volatile uint8_t SPI2_Buffer_Tx[SPI2_BufferSize];


