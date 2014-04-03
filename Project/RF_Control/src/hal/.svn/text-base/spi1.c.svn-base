#include "stm32F10x.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"

#include "spi1.h"
#include "spi2.h"

/*
Quest SPI è per comunicare con AD9832 per modificare la frequenza di esercizio.


0xFFFFFFFF --> 4.294.967.295


(5/25) * (0xFFFFFFFF) = 0x33333333  -->> 5 Mhz
(1/25) * (0xFFFFFFFF) = 0x33333333  -->> 1 Mhz



*/


#define KHZ_10 (uint32_t)1717986 
#define KHZ_1 (uint32_t)171799 


void SPI1_Delay(void)
{
unsigned int x;
unsigned char y;
    for(y = 0; y <4; y++)
    {
       for (x = 0; x<800; x++);
    }
}




void SPI1_Config(void)
{
 
        GPIO_InitTypeDef GPIO_InitStructure;
        SPI_InitTypeDef SPI_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);     // enable peripheral clocks

      // GPIO_InitStructure.GPIO_Pin = SPI_SCK | SPI_MISO | SPI_MOSI;    // Configure SCK, MISO, MOSI
				GPIO_InitStructure.GPIO_Pin = SPI_SCK | SPI_MOSI;    // Configure SCK, MISO, MOSI
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;               // 2/10/50 MHz
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                 // AF (Alternate Function)for SPI use
        GPIO_Init(GPIOA, &GPIO_InitStructure);

         RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);     // enable peripheral clocks 
				GPIO_InitStructure.GPIO_Pin = SPI_NSS1;                  // Configure NSS as Output push-pull
         GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;       // 2/10/50 MHz
         GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
         GPIO_Init(SPI_PORT_NSS1, &GPIO_InitStructure);

        // SPI configuration
        SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;      // send/recv simul
        SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                           // we are master
        SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;                      // 16-bit xfers
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
				
        SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;      				// compiler error #134 ???
				//SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;      				// compiler error #134 ???
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;      // 562.5Khz
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
        SPI_InitStructure.SPI_CRCPolynomial = 7;
        SPI_Init(SPI1, &SPI_InitStructure);         // Init SPI
        SPI_Cmd(SPI1, ENABLE);                          // Enable SPI




}

void SPI1_SendData_2_AD9835(uint16_t Data)
{
	//HAL_NSS_BOARD_OFF();
	SPI_NSS1_OFF(); 
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, Data);	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	//HAL_NSS_BOARD_ON();
	SPI_NSS1_ON() ;
}	



void SPI_Write_FrquencyRAW (uint32_t reg)
{
		/*Calcolo il Registro a 32 bit da spedire all'IC e lo spedisce...
		reg è il valore 16 bit della frequenza espressa in x1Khz ......100 -> 1MHz*/
	
	uint32_t tmp_reg_32,SPI_frq_reg;
	uint16_t tmp_reg_16;	
	
	SPI_frq_reg = (uint32_t) (reg);
	
	//Spedisco i primi 16bit
	SPI1_SendData_2_AD9835(0xC000);
	
	//Preparo il registro da scrivere  H_MSB
	tmp_reg_32=SPI_frq_reg;
	tmp_reg_32 = (tmp_reg_32 >> 	24) &(0x000000FF);
	tmp_reg_16 =0x0000;
	tmp_reg_16 =(0x3300) & (0xFF00);
	tmp_reg_16 = tmp_reg_16 + (uint16_t) (tmp_reg_32)	;
	
	SPI1_SendData_2_AD9835(tmp_reg_16);
	
	//Preparo il registro da scrivere  L_MSB	
	tmp_reg_32=SPI_frq_reg;
	tmp_reg_32 = (tmp_reg_32 >> 	16) &(0x000000FF);
	tmp_reg_16 =0x0000;
	tmp_reg_16 =(0x2200) & (0xFF00);
	tmp_reg_16 = tmp_reg_16 + (uint16_t) (tmp_reg_32)	;
	SPI1_SendData_2_AD9835(tmp_reg_16);

//Preparo il registro da scrivere  H_LSB	
	tmp_reg_32=SPI_frq_reg;
	tmp_reg_32 = (tmp_reg_32 >> 	8) &(0x000000FF);
	tmp_reg_16 =0x0000;
	tmp_reg_16 =(0x3100) & (0xFF00);
	tmp_reg_16 = tmp_reg_16 + (uint16_t) (tmp_reg_32)	;
	SPI1_SendData_2_AD9835(tmp_reg_16);

	//Preparo il registro da scrivere  L_LSB	
	tmp_reg_32=SPI_frq_reg;
	tmp_reg_32 = (tmp_reg_32 >> 0	) &(0x000000FF);
	tmp_reg_16 =0x0000;
	tmp_reg_16 =(0x2000) & (0xFF00);
	tmp_reg_16 = tmp_reg_16 + (uint16_t) (tmp_reg_32)	;
	SPI1_SendData_2_AD9835(tmp_reg_16);

	
	SPI1_SendData_2_AD9835(0xC000);	
	
	
}	



void SPI_Write_Frquency (uint16_t reg)
{
		/*Calcolo il Registro a 32 bit da spedire all'IC e lo spedisce...
		reg è il valore 16 bit della frequenza espressa in x1Khz ......100 -> 1MHz*/
	
	uint32_t tmp_reg_32,SPI_frq_reg;
	uint16_t tmp_reg_16;	
	
	SPI_frq_reg = (uint32_t) (reg * KHZ_1);
	
	//Spedisco i primi 16bit
	SPI1_SendData_2_AD9835(0xC000);
	
	//Preparo il registro da scrivere  H_MSB
	tmp_reg_32=SPI_frq_reg;
	tmp_reg_32 = (tmp_reg_32 >> 	24) &(0x000000FF);
	tmp_reg_16 =0x0000;
	tmp_reg_16 =(0x3300) & (0xFF00);
	tmp_reg_16 = tmp_reg_16 + (uint16_t) (tmp_reg_32)	;
	
	SPI1_SendData_2_AD9835(tmp_reg_16);
	
	//Preparo il registro da scrivere  L_MSB	
	tmp_reg_32=SPI_frq_reg;
	tmp_reg_32 = (tmp_reg_32 >> 	16) &(0x000000FF);
	tmp_reg_16 =0x0000;
	tmp_reg_16 =(0x2200) & (0xFF00);
	tmp_reg_16 = tmp_reg_16 + (uint16_t) (tmp_reg_32)	;
	SPI1_SendData_2_AD9835(tmp_reg_16);

//Preparo il registro da scrivere  H_LSB	
	tmp_reg_32=SPI_frq_reg;
	tmp_reg_32 = (tmp_reg_32 >> 	8) &(0x000000FF);
	tmp_reg_16 =0x0000;
	tmp_reg_16 =(0x3100) & (0xFF00);
	tmp_reg_16 = tmp_reg_16 + (uint16_t) (tmp_reg_32)	;
	SPI1_SendData_2_AD9835(tmp_reg_16);

	//Preparo il registro da scrivere  L_LSB	
	tmp_reg_32=SPI_frq_reg;
	tmp_reg_32 = (tmp_reg_32 >> 0	) &(0x000000FF);
	tmp_reg_16 =0x0000;
	tmp_reg_16 =(0x2000) & (0xFF00);
	tmp_reg_16 = tmp_reg_16 + (uint16_t) (tmp_reg_32)	;
	SPI1_SendData_2_AD9835(tmp_reg_16);
	
//SPI1_SendData_2_AD9835(0x330A);
//SPI1_SendData_2_AD9835(0x223D);
//SPI1_SendData_2_AD9835(0x3170);
//SPI1_SendData_2_AD9835(0x20A3);
	
SPI1_SendData_2_AD9835(0xC000);	
	
}	

uint16_t SPI1_Freq_Reg;


void SPI1_Init(void)
{
		
	SPI1_Config();
	SPI1_Freq_Reg=1000;		
	
	SPI_Write_Frquency (SPI1_Freq_Reg);
	
		
	
}

uint32_t SPI_frq_regraw=0l;

void SPI1_Clk(void)
{

 if(SPI_frq_regraw){
		SPI_Write_FrquencyRAW (SPI_frq_regraw);
 
 }else
 {
	SPI_Write_Frquency (SPI1_Freq_Reg);
 }
}	

