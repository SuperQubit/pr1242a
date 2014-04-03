#include "stm32F10x.h"
#include "i2c1.h"

void Pippo_Delay(void)
{
unsigned int x;
unsigned char y;
    for(y = 0; y <4; y++)
    {
       for (x = 0; x<800; x++);
    }
}




uint16_t 	DS1086L_PRESCALER;
#define 	DS1086L_PRESCALER_OFFSET ((uint8_t)(0x02))

uint16_t 	DS1086L_DAC;
#define 	DS1086L_DAC_OFFSET ((uint8_t)(0x08))

uint16_t 	DS1086L_PRESCALER;
#define 	DS1086L_PRESCALER_OFFSET ((uint8_t)(0x02))

int_least8_t 	DS1086L_OFFSET;
#define 	DS1086L_OFFSET_OFFSET ((uint8_t)(0x0E))

uint8_t 	DS1086L_RANGE;
#define 	DS1086L_RANGE_OFFSET ((uint8_t)(0x37))


uint8_t 	DS1086L_ADDRESS;
#define 	DS1086L_ADDRESS_OFFSET ((uint8_t)(0x0D))

/*******************************************************************************  
* Function Name  : DS1086L_iRead 
* Description    : Legge un intero (2 byte) dallo spazio di memoria del DS1086  
*                  
* Input          : - pBuffer : pointer to the buffer containing the data to be   
*                    written to the EEPROM.  
*                  - WriteAddr : EEPROM's internal address to write to.  
*                  - NumByteToWrite : number of bytes to write to the EEPROM.  
* Output         : None  
* Return         : None  
*******************************************************************************/   
uint16_t DS1086L_iRead(uint8_t ADDR_REG) 
{
	uint16_t	tmp16_var;
	tmp16_var=0x0000;
	
	I2C2_ANS =I2C2_Buffer_Read(I2C2_SLAVE_ADDRESS_DS1086L, I2C2_rx_Buffer, ADDR_REG, 2 );
	
	
	if (!I2C2_ANS) 
	{
	
	tmp16_var = ((((uint16_t)(I2C2_rx_Buffer[0])) & (0x00FF)) << 8) + (((uint16_t)(I2C2_rx_Buffer[1])) & (0x00FF));
	}
	return(tmp16_var);
}

/*******************************************************************************  
* Function Name  : DS1086L_bRead 
* Description    : Legge un byte dallo spazio di memoria del DS1086  
*                  
* Input          : - ADDR_REG
*                  - 
* Output         : None  
* Return         : None  
*******************************************************************************/   
uint8_t DS1086L_bRead(uint8_t ADDR_REG) 
{
	uint8_t	tmp8_var;
	tmp8_var=0x00;
	
	I2C2_ANS =I2C2_Buffer_Read(I2C2_SLAVE_ADDRESS_DS1086L, I2C2_rx_Buffer, ADDR_REG, 1 );
	
	
	if (!I2C2_ANS) 
	{
	
	tmp8_var = I2C2_rx_Buffer[0];
	}
	return(tmp8_var);
}


void DS1086L_bWrite(uint8_t VALUE,uint8_t ADDR_REG) 
{
		
	I2C2_tx_Buffer[0]=VALUE;
	I2C2_ANS =I2C2_PageWrite(I2C2_SLAVE_ADDRESS_DS1086L, I2C2_tx_Buffer, ADDR_REG, 1) ;
	
}

void DS1086L_iWrite(uint16_t VALUE,uint8_t ADDR_REG) 
{
		
	I2C2_tx_Buffer[1]=  (uint8_t)((VALUE >> 0) & (0x00FF));
	I2C2_tx_Buffer[0]=	(uint8_t)((VALUE >> 8) & (0x00FF));
	
	I2C2_ANS =I2C2_PageWrite(I2C2_SLAVE_ADDRESS_DS1086L, I2C2_tx_Buffer, ADDR_REG, 2) ;
	
}





uint8_t Write_Command;
int_least8_t Write_bValue;
int16_t Write_iValue;



uint16_t DS1086L_PRESCALER_COMPOSITION(uint16_t ValuePrescaler)

{
	uint16_t tmp16;
	
	tmp16 = DS1086L_PRESCALER;
	
	if (ValuePrescaler < 16)
		{
		tmp16 &=~(0x03C0);
		ValuePrescaler= ((ValuePrescaler << 6) & (0x03C0));
		tmp16 |= (ValuePrescaler);
		}
		return(tmp16);
}

uint16_t DS1086L_DAC_COMPOSITION(uint16_t ValueDAC)

{
	uint16_t tmp16;
	tmp16=0;
			
	if (ValueDAC < 1024)
		{
		tmp16 = (ValueDAC << 6) & (0xFFC0);
		}
		return(tmp16);
}


void DS1086_ReadAllReg(void)
{
		
	DS1086L_PRESCALER = DS1086L_iRead(DS1086L_PRESCALER_OFFSET) &(0x03C0); 
	DS1086L_DAC = DS1086L_iRead(DS1086L_DAC_OFFSET) &(0xFFC0);
	DS1086L_OFFSET = (DS1086L_bRead(DS1086L_OFFSET_OFFSET)) &(0x1F); 
	DS1086L_ADDRESS = (DS1086L_bRead(DS1086L_ADDRESS_OFFSET)) &(0x0F);
	DS1086L_RANGE = (DS1086L_bRead(DS1086L_RANGE_OFFSET)) &(0x1F);
}


void DS1086_Write_Prescaler(uint16_t prescaler)

{
	DS1086L_iWrite(DS1086L_PRESCALER_COMPOSITION(prescaler),DS1086L_PRESCALER_OFFSET);
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();

Write_Command=0;	
	DS1086_ReadAllReg();	
	
	
}

void DS1086_Write_DAC(uint16_t dac)

{
	DS1086L_iWrite(DS1086L_DAC_COMPOSITION(dac),DS1086L_DAC_OFFSET);
	Write_Command=0;
	
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();	
	
	DS1086_ReadAllReg();	
	
	
}


void DS1086_Write_OFFSET(uint8_t offset)

{
	uint8_t tmp;
	tmp=0;
	DS1086L_RANGE = (DS1086L_bRead(DS1086L_RANGE_OFFSET)) &(0x1F);
	tmp= DS1086L_RANGE + offset;
	
	DS1086L_iWrite(tmp,DS1086L_OFFSET_OFFSET);
	Write_Command=0;
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();	
	
	DS1086_ReadAllReg();	
	
	
}




// Valid Spread 0,1,2,4,7
#define DS1086L_SPREAD	(2)

void DS1086_Init(void)
{
	
	Write_Command=0;
	Write_bValue=0;
	Write_iValue=0;
	
	DS1086L_PRESCALER=0;
	DS1086L_DAC=0;
	DS1086L_OFFSET=0;
	DS1086L_RANGE=0;
	DS1086L_ADDRESS=0xFF;
	
	DS1086L_iWrite(DS1086L_PRESCALER_COMPOSITION(1)| (DS1086L_SPREAD)<<11 ,DS1086L_PRESCALER_OFFSET);
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	DS1086L_iWrite(DS1086L_DAC_COMPOSITION(800),DS1086L_DAC_OFFSET);	
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	
	DS1086L_PRESCALER = DS1086L_iRead(DS1086L_PRESCALER_OFFSET) &(0x03C0); 
	DS1086L_DAC = DS1086L_iRead(DS1086L_DAC_OFFSET) &(0xFFC0);
	DS1086L_OFFSET = (DS1086L_bRead(DS1086L_OFFSET_OFFSET)) &(0x1F); 
	DS1086L_ADDRESS = (DS1086L_bRead(DS1086L_ADDRESS_OFFSET)) &(0x0F);
	DS1086L_RANGE = (DS1086L_bRead(DS1086L_RANGE_OFFSET)) &(0x1F);
}
	
	




void DS1086_WriteCommand()
{
	DS1086L_iWrite(DS1086L_PRESCALER_COMPOSITION(Write_iValue),DS1086L_PRESCALER_OFFSET);
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	DS1086L_iWrite(DS1086L_DAC_COMPOSITION(Write_iValue),DS1086L_DAC_OFFSET);	
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	
	DS1086L_bWrite(0xF0,0x0D);	
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	Pippo_Delay();
	
	
	DS1086L_PRESCALER = DS1086L_iRead(DS1086L_PRESCALER_OFFSET) &(0x03C0); 
	DS1086L_DAC = DS1086L_iRead(DS1086L_DAC_OFFSET)&(0xFFC0);
	DS1086L_OFFSET = (DS1086L_bRead(DS1086L_OFFSET_OFFSET)) &(0x1F); 
	DS1086L_ADDRESS = (DS1086L_bRead(DS1086L_ADDRESS_OFFSET)) &(0x0F); 
	DS1086L_RANGE = (DS1086L_bRead(DS1086L_RANGE_OFFSET)) &(0x1F);
}

void DS1086_Clk(void)

{
	/*
	
	if (Write_Command == 1)
		{
		DS1086_WriteCommand();
	  Write_Command=0;
	}
	*/
	//DS1086L_OFFSET = (DS1086L_bRead(DS1086L_OFFSET_OFFSET)) &(0x1F); 
	//DS1086L_RANGE = (DS1086L_bRead(DS1086L_RANGE_OFFSET))&(0x1F);	
	//DS1086L_PRESCALER = DS1086L_iRead(DS1086L_PRESCALER_OFFSET); 
	
	
	if (Write_Command == 1) {DS1086_Write_Prescaler(Write_iValue);}
	if (Write_Command == 2) {DS1086_Write_DAC(Write_iValue);} 
	if (Write_Command == 3) {DS1086_Write_OFFSET(Write_bValue);} 

	
}


