#define SCL1_GPIO_CLK       RCC_APB2Periph_GPIOB  
#define SCL1_GPIO_PORT      GPIOB
#define SCL1_GPIO_PIN       GPIO_Pin_10  


#define SDA1_GPIO_CLK       RCC_APB2Periph_GPIOB  
#define SDA1_GPIO_PORT      GPIOB
#define SDA1_GPIO_PIN       GPIO_Pin_11  

#define I2C1_GPIO_PORT      GPIOB


#define I2C1_SLAVE_ADDRESS7     0xFF   //Usato come mio indirizzo
#define I2C2_SLAVE_ADDRESS7     0xFF   //Usato come mio indirizzo


#define I2C2_SLAVE_ADDRESS_DS1086L     	0xB0   	//Indirizzo in DS1086L
#define ADDRESS_DS1086L_START	0x02


extern void I2C2_Init(void);
extern void I2C2_Clk(void);

extern uint8_t I2C2_ANS;
extern uint8_t I2C2_rx_Buffer[8];
extern uint8_t I2C2_tx_Buffer[8];

extern uint8_t I2C2_Byte_Write(uint8_t Slave_Address,uint8_t* pBuffer, uint8_t WriteAddr);
extern uint8_t I2C2_PageWrite(uint8_t I2C_Addr, u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite) ;
extern uint8_t I2C2_Buffer_Read(uint8_t I2C_Addr, uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);


#define I2C_ERROR_OK        (0)
#define I2C_ERROR_EV1       (1)
#define I2C_ERROR_EV2       (2)
#define I2C_ERROR_EV3       (3)
#define I2C_ERROR_EV4       (4)
#define I2C_ERROR_EV5       (5)
#define I2C_ERROR_EV6       (6)
#define I2C_ERROR_EV7       (7)
#define I2C_ERROR_EV8       (8)
#define I2C_ERROR_EV9       (9)


#define I2C1_ERROR_TIMEOUT     0xFFFF 
#define I2C2_ERROR_TIMEOUT     0xFFFF 


