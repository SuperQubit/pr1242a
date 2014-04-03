#include "stm32F10x.h"
#include "stm32f10x_i2c.h"
#include "i2c1.h"


uint8_t I2C2_ANS;
uint8_t I2C2_rx_Buffer[8];
uint8_t I2C2_tx_Buffer[8];

uint8_t I2C2_FSM_State;



void I2C2_Delay(void)
{
unsigned int x;
unsigned char y;
    for(y = 0; y <4; y++)
    {
       for (x = 0; x<800; x++);
    }
}

/*
#define Timed(x) Timeout=0xFFFF; while(x) \ {if (Timeout-- == 0) goto errReturn;}

//Esempio d'uso

Timed(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY);




errRetunr:
	retunr(ERROR)

*/



void I2C2_Config(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;  
  	/* Enable the GPIO_SCL Clock */
		RCC_APB2PeriphClockCmd(SCL1_GPIO_CLK, ENABLE);

		/* Enable the GPIO_SDA Clock */
		RCC_APB2PeriphClockCmd(SDA1_GPIO_CLK, ENABLE);

	/* I2C1 Periph clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);


/* I2C1 SDA and SCL configuration */
GPIO_InitStructure.GPIO_Pin = SCL1_GPIO_PIN | SDA1_GPIO_PIN;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
GPIO_Init(GPIOB, &GPIO_InitStructure);


I2C_DeInit(I2C2);

I2C_Cmd(I2C2, ENABLE); /* All examples enable, then configure */


I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
I2C_InitStructure.I2C_OwnAddress1 = I2C2_SLAVE_ADDRESS7;
I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
I2C_InitStructure.I2C_ClockSpeed = 100000;
I2C_Init(I2C2, &I2C_InitStructure);


//I2C_Cmd(I2C2, ENABLE); /* All examples enable, then configure */

}


void I2C2_Init(void)
{
	
	I2C2_Config(); 
	
	I2C2_FSM_State=0;

}

void I2C2_Clk(void)
{

	
	//if (I2C2_ANS) HAL_LED_R_ON(); else HAL_LED_R_OFF();
	
}

/*******************************************************************************  
* Function Name  : I2C2_Byte_Write  
* Description    : Writes one byte to the EEPROM with a single WRITE   cycle. 
* Input          :- Slave Addres:  Addrss dello Slave   
									- pBuffer : pointer to the buffer containing the data to be   
*                    written to the EEPROM.  
*                  - WriteAddr : EEPROM's internal address to write to.  
*                  
* Output         : None  
* Return         : Error Code  
*******************************************************************************/  
uint8_t I2C2_Byte_Write(uint8_t Slave_Address,uint8_t* pBuffer, uint8_t WriteAddr)
{
 
    __IO uint32_t Timeout = 0;
	
 Timeout = 0xFFFF;
 	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
	{
   if (Timeout-- == 0)
   return (I2C_ERROR_EV1);
  }
 
  /* Send STRAT condition */
  I2C_GenerateSTART(I2C2, ENABLE);

  /* Test on EV5 and clear it */
  Timeout = 0xFFFF;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))  
  		{
            if (Timeout-- == 0)
            return (I2C_ERROR_EV5);
        }

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C2, Slave_Address, I2C_Direction_Transmitter);
			
			
  
  /* Test on EV6 and clear it */
  Timeout = 0xFFFF;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
            if (Timeout-- == 0)  return (I2C_ERROR_EV6);
               
        }
      
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C2, WriteAddr);
  
  /* Test on EV8 and clear it */
  Timeout = 0xFFFF;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
   {
            if (Timeout-- == 0)  return (I2C_ERROR_EV8);
               
        }
  

  /* Send the byte to be written */
  I2C_SendData(I2C2, *pBuffer); 
   
  /* Test on EV8 and clear it */
  Timeout = 0xFFFF;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
            if (Timeout-- == 0)  return (I2C_ERROR_EV8);
               
        }
  /* Send STOP condition */
  I2C_GenerateSTOP(I2C2, ENABLE);
return(I2C_ERROR_OK);

}

/*******************************************************************************  
* Function Name  : I2C_PageWrite  
* Description    : Writes more than one byte to the EEPROM with a single WRITE  
*                  cycle. The number of byte can't exceed the EEPROM page size.  
* Input          :  - I2C_Addre : Addrss dello Slave
										- pBuffer : pointer to the buffer containing the data to be   
*                    written to the EEPROM.  
*                  - WriteAddr : EEPROM's internal address to write to.  
*                  - NumByteToWrite : number of bytes to write to the EEPROM.  
* Output         : None  
* Return         : None  
*******************************************************************************/   
uint8_t  I2C2_PageWrite(uint8_t I2C_Addr, u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)   
{   
  
	  __IO uint32_t Timeout = 0;
       /* While the bus is busy */
      Timeout = 0xFFFF;
     while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
           {
            if (Timeout-- == 0)
                return (I2C_ERROR_EV1);
        }
	
	
	/* Send START condition */   
  I2C_GenerateSTART(I2C2, ENABLE);   
     
  /* Test on EV5 and clear it */
  Timeout = 0xFFFF;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
      {
            if (Timeout-- == 0)
                return (I2C_ERROR_EV5);
        }
				
     
  /* Send EEPROM address for write */   
  I2C_Send7bitAddress(I2C2, I2C_Addr, I2C_Direction_Transmitter);   
   
  
/* Test on EV6 and clear it */
  				Timeout = 0xFFFF;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
            if (Timeout-- == 0)  return (I2C_ERROR_EV6);
               
        }
  		
   
  /* Send the EEPROM's internal address to write to */       
  I2C_SendData(I2C2, WriteAddr);     
   
 
 /* Test on EV8 and clear it */
  Timeout = 0xFFFF;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
   {
            if (Timeout-- == 0)  return (I2C_ERROR_EV8);
               
        }

				
   
  /* While there is data to be written */   
  while(NumByteToWrite--)     
  {   
    /* Send the current byte */   
    I2C_SendData(I2C2, *pBuffer);    
   
    /* Point to the next byte to be written */   
    pBuffer++;    
     
    /* Test on EV8 and clear it */   
   
	Timeout = 0xFFFF;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
   {
            if (Timeout-- == 0)  return (I2C_ERROR_EV8);
               
        }
		
  }   
   
  /* Send STOP condition */   
  I2C_GenerateSTOP(I2C2, ENABLE); 

return(I2C_ERROR_OK);  
}   





/*******************************************************************************
* Function Name  : I2C2_BufferRead
* Description    : Reads a block of data from the EEPROM.
* Input          : - pBuffer : pointer to the buffer that receives the data read 
*                    from the EEPROM.
*                  - ReadAddr : EEPROM's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the EEPROM.
* Output         : None
* Return         : None
*******************************************************************************/

uint8_t I2C2_Buffer_Read(uint8_t I2C_Addr, uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
   
     __IO uint32_t Timeout = 0;
     
     
  /* While the bus is busy */
  
     Timeout = 0xFFFF;
     while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
           {
            if (Timeout-- == 0)
                return (I2C_ERROR_EV1);
        }
  
  
  /* Send START condition */
  I2C_GenerateSTART(I2C2, ENABLE);
  
  /* Test on EV5 and clear it */
  Timeout = 0xFFFF;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
      {
            if (Timeout-- == 0)
                return (I2C_ERROR_EV5);
        }
   
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C2, I2C_Addr, I2C_Direction_Transmitter);
				
				
  /* Test on EV6 and clear it */
  				Timeout = 0xFFFF;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
            if (Timeout-- == 0)  return (I2C_ERROR_EV6);
               
        }
  
  /* Clear EV6 by setting again the PE bit */
  
				I2C_Cmd(I2C2, ENABLE);

  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C2, ReadAddr);  

  /* Test on EV8 and clear it */
    Timeout = I2C2_ERROR_TIMEOUT;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
            if (Timeout-- == 0)
                return (I2C_ERROR_EV4);
        }
  
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C2, ENABLE);
  
  /* Test on EV5 and clear it */
    Timeout = I2C2_ERROR_TIMEOUT;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
    {
            if (Timeout-- == 0)
                return (I2C_ERROR_EV5);
        }
  
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(I2C2, I2C_Addr, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
    Timeout = I2C2_ERROR_TIMEOUT;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
     {
            if (Timeout-- == 0)
                return (I2C_ERROR_EV6);
        }
  
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C2, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C2, ENABLE);
    }

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      /* Read a byte from the EEPROM */
      *pBuffer = I2C_ReceiveData(I2C2);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;        
    }   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C2, ENABLE);
return(I2C_ERROR_OK);
}


















