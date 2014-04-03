#include "stm32F10x.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"

#include "spi2.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

volatile uint8_t SPI2_Buffer_Rx[SPI2_BufferSize];
volatile uint8_t SPI2_Buffer_Tx[SPI2_BufferSize];

volatile uint8_t RxIdx = 0;
volatile uint8_t SPI2_TimeOut;

void SPI_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  /* Configure and enable SPI_SLAVE interrupt --------------------------------*/
  NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void SPI2_Init(void)
{

		GPIO_InitTypeDef GPIO_InitStructure;
		SPI_InitTypeDef SPI_InitStructure;

		SPI_NVIC_Configuration();	
	
	
		RCC_APB1PeriphClockCmd(SPI2_PERIPH_CLK, ENABLE);     // enable peripheral clocks				
		RCC_APB2PeriphClockCmd(GPIO_PERIPH_CLK, ENABLE);     // enable peripheral clocks	
	 
	
	
		GPIO_InitStructure.GPIO_Pin = SPI2_SCK | SPI2_MOSI;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(SPI2_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = SPI2_MISO;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(SPI2_PORT, &GPIO_InitStructure);
	

		GPIO_InitStructure.GPIO_Pin = SPI2_NSS2;                  // Configure NSS as Output push-pull
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(SPI2_PORT, &GPIO_InitStructure);

        // SPI configuration
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;      // send/recv simul
		
		
		SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;                           // we are master
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                      // 8-bit xfers
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;  //Con la nuova versione ho messo 2Edge invece di 1Edge
		
		//SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;      				// compiler error #134 ???
		SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;      				// compiler error #134 ???
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;      // 562.5Khz
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_CRCPolynomial = 7;
		SPI_Init(SPI2, &SPI_InitStructure);         // Init SPI
	 

		/*Abilito il flag dell'interrupt.Inizio*/
		
		/* Enable SPI_SLAVE RXNE interrupt */
		SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
		/*Abilito il flag dell'interrupt.Fine*/

		SPI_Cmd(SPI2, ENABLE);                          // Enable SPI

		RxIdx=0;
		SPI2_TimeOut=0;					
}

volatile uint8_t SPI2_Status=0;

void (*_SPI2_RX_Event)(void);
void (*_SPI2_TX_Event)(void);
volatile uint8_t _psize=SPI2_BufferSize;
volatile static uint8_t RCHK=0,TCHK=0;
	
void SPI2_EventHandler(void)
{	

	SPI2_TimeOut=0;
	SPI2_Status =0;
	//Ricevo il dato dal MOSI 
	RCHK	+= (SPI2_Buffer_Rx[RxIdx] = SPI_I2S_ReceiveData(SPI2));
  
	if (RxIdx++==_psize){
		if(RCHK==0)
		{ //Nuovo Messaggio ricevuto			
			if( _SPI2_RX_Event)	_SPI2_RX_Event();			
		}
		else
		{
			SPI2_Status =0x80;		
		}
		//	*p= (~CHK)+1;
		TCHK=RCHK=RxIdx=0;	
		
	}
	//Preparo il dato sul MISO
	TCHK +=  SPI2_Buffer_Tx[RxIdx];
	if (RxIdx==_psize)
	{
		SPI_I2S_SendData(SPI2, (~TCHK)+1);	
	}
	else
	{
			SPI_I2S_SendData(SPI2, SPI2_Buffer_Tx[RxIdx]);
	}
}

void SPI2_SetEventHandler(void (*RXHandler)(void) ,void (*TXHandler)(void) )
{
	  __disable_irq();
		_SPI2_RX_Event=RXHandler;
		_SPI2_TX_Event=TXHandler;
	 __enable_irq();
}

void SPI2_SetPacketSize(uint8_t psize)
{
	  __disable_irq();
	_psize=psize;
	 __enable_irq();
}

void SPI2_Clk(void)
{	
	SPI2_TimeOut++;
	
	if (SPI2_TimeOut == 5)
	{ 
		RCHK=TCHK=RxIdx=0;
	}	
	if( SPI2_TimeOut > 250)
	{
			SPI2_Status = 1;
			SPI2_TimeOut= 5;
	}
	
}

