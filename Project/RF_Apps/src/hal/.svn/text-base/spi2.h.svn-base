

#define SPI2_PORT        GPIOB



#define SPI2_NSS2        GPIO_Pin_12      // SPI nSS
#define SPI2_SCK         GPIO_Pin_13      // SPI Clock
#define SPI2_MISO        GPIO_Pin_14      // SPI MISO
#define SPI2_MOSI        GPIO_Pin_15      // SPI MOSI


#define SPI2_PERIPH_CLK 	RCC_APB1Periph_SPI2
#define GPIO_PERIPH_CLK 	(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB)


extern void SPI2_SetEventHandler(void (*RXHandler)(void) ,void (*TXHandler)(void) );
extern void SPI2_EventHandler(void);
extern void SPI2_Init(void);
extern void SPI2_Clk(void);

extern volatile uint8_t SPI2_TimeOut;		//Azzerato sotto interrupt
extern volatile uint8_t RxIdx;						//Usato sotto interrupt
#define SPI2_BufferSize (10)				//Usato sotto Interrupt per riallineare


#define SPI2_RIDX_PERCPOWER		0
#define SPI2_RIDX_MODULATOR		2
#define SPI2_RIDX_MANIPOLO		4
#define SPI2_RIDX_FREQUENZA		6

#define SPI2_TIDX_APPPOWER		0
#define SPI2_TIDX_ACTPOWERR		1
#define SPI2_TIDX_RCTPOWER		2
#define SPI2_TIDX_MODULATOR		3
#define SPI2_TIDX_PERCPOWER		4
#define SPI2_TIDX_FREQUENZA		5
#define SPI2_TIDX_FREQREAL		6
#define SPI2_TIDX_MANIPOLOL		7

#define SPI2RX_PERCPOWER		(SPI2_Buffer_Rx[SPI2_RIDX_PERCPOWER])
#define SPI2RX_MODULATOR		(SPI2_Buffer_Rx[SPI2_RIDX_MODULATOR])
#define SPI2RX_MANIPOLO			(SPI2_Buffer_Rx[SPI2_RIDX_MANIPOLO])
#define SPI2RX_FREQUENZA		(SPI2_Buffer_Rx[SPI2_RIDX_FREQUENZA])

#define SPI2TX_APPPOWER		  (SPI2_Buffer_Tx[SPI2_TIDX_APPPOWER])
#define SPI2TX_ACTPOWERR		(SPI2_Buffer_Tx[SPI2_TIDX_ACTPOWERR])
#define SPI2TX_RCTPOWER		  (SPI2_Buffer_Tx[SPI2_TIDX_RCTPOWER])
#define SPI2TX_MODULATOR		(SPI2_Buffer_Tx[SPI2_TIDX_MODULATOR])
#define SPI2TX_PERCPOWER		(SPI2_Buffer_Tx[SPI2_TIDX_PERCPOWER])
#define SPI2TX_FREQUENZA		(SPI2_Buffer_Tx[SPI2_TIDX_FREQUENZA])
#define SPI2TX_FREQREAL		  (SPI2_Buffer_Tx[SPI2_TIDX_FREQREAL])
#define SPI2TX_MANIPOLOL		(SPI2_Buffer_Tx[SPI2_TIDX_MANIPOLOL])

extern volatile uint8_t SPI2_Buffer_Rx[SPI2_BufferSize];
extern volatile uint8_t SPI2_Buffer_Tx[SPI2_BufferSize];


