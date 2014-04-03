


#define SPI_PORT_NSS1        GPIOB     // SPI nSS PORT
#define SPI_NSS1        GPIO_Pin_1      // SPI nSS PIN

#define SPI_NSS1_ON()     (SPI_PORT_NSS1->ODR |= SPI_NSS1) 
#define SPI_NSS1_OFF()   (SPI_PORT_NSS1->ODR &= ~SPI_NSS1) 
#define SPI_NSS1_TOG()    (SPI_PORT_NSS1->ODR ^= SPI_NSS1) 

#define SPI_SCK         GPIO_Pin_5      // SPI Clock
#define SPI_MISO        GPIO_Pin_6      // SPI MISO
#define SPI_MOSI        GPIO_Pin_7      // SPI MOSI


extern void SPI1_Init(void);
extern void SPI1_Clk(void);


extern uint16_t SPI1_Freq_Reg;

