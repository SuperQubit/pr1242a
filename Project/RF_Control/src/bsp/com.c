/******************** (C) COPYRIGHT 2012 ArSilicii ********************
* File Name          : com.c
* Author             : DL
* Version            : V1.00
* Date               : 21-10-2012
* Description        : COM port via UART
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "hal.h"
#include "com.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile struct COM mcom;

/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
// -----------------------------------------------------------
int  com_tx_empty(void)  //se=1 buffer di tx vuoto
// -----------------------------------------------------------
{
   return (mcom.tx0==mcom.tx1);
}

// -----------------------------------------------------------
int  com_tx_full(void)   //se=1 buffer di tx pieno
// -----------------------------------------------------------
{
   return (mcom.tx0==((mcom.tx1+1)%mcom.txsize));
}
// -----------------------------------------------------------
int com_rx_linefull(void)
// -----------------------------------------------------------
{	
	return(mcom.eol);
}
// -----------------------------------------------------------
int  com_rx_empty(void)  //se 1 buffer di rx vuoto
// -----------------------------------------------------------
{
   return (mcom.rx0==mcom.rx1);
}

// -----------------------------------------------------------
int  com_rx_size(void)   //n. di car. nel buffer rx
// -----------------------------------------------------------
{
    int size;
    size=mcom.rx1-mcom.rx0;
    if (size<0) size+=mcom.rxsize;
    return size;
}

// -----------------------------------------------------------
int  com_tx_size(void)   //n. di car. nel buffer tx
// -----------------------------------------------------------
{
    int size;
    size=mcom.tx1-mcom.tx0;
    if (size<0) size+=mcom.txsize;
    return size;
}

// -----------------------------------------------------------
void com_tx_start(int period)
// -----------------------------------------------------------
{
   char c;
   if(!mcom.txrun)
   {
      if(mcom.tx0!=mcom.tx1)   //se ci sono caratteri nel buffer software...
      {
        c = mcom.txbuff[mcom.tx0];		      //scoda
				mcom.tx0=(mcom.tx0+1)&(mcom.txsize-1);  //incrementa puntatore primo scod.
        mcom.txrun=1;
	      USART1_SendData((uint8_t) c); //invia 
	      if(period==1)
	      {
					USART1_ITXEConfig(ENABLE);
	      }
				else
				{
					USART1_ITXEConfig(DISABLE);						
				}
     }
   }
}

// -----------------------------------------------------------
//  tx0=primo carattere da trasmettere  (increm. da interrupt)
//  tx1=carattere da mettere nel buffer (incr. dat com_tx)
//  txsize=lungh del buffer
//  se (tx1+1)%txsize=tx0 buffer pieno
//
// accoda un carattere, quindi lo trasmette se:
// 1- buffer pieno  oppure se
// 2- la com non ha buffer
// 3- se phisic>=1
// -----------------------------------------------------------
int  com_tx_virtual(unsigned char c,int phisic)  
// -----------------------------------------------------------
{
    while(com_tx_full())                      //se buffer tx full
    {
        if(!mcom.txrun) com_tx_start(1);        //se tx non attivo lo attiva
    }
    mcom.txbuff[mcom.tx1]=c;                     //accoda il carattere 
    mcom.tx1=(mcom.tx1+1)%mcom.txsize;           //incrementa il puntatore
    if((!mcom.txrun)&&(phisic)) com_tx_start(phisic); //se tx non attivo lo attiva
    mcom.txchk+=c;
    return 0;
}

// -----------------------------------------------------------
int  COM_tx(unsigned char c)  //trasmette un carattere
// -----------------------------------------------------------
{
    return com_tx_virtual(c,1);
}

// -----------------------------------------------------------
//  rx0=primo carattere da leggere      (increm. da com_rx)
//  rx1=carattere da mettere nel buffer (increm. da interrupt)
//  rxsize=lungh del buffer
//  se rx1=rx0 buffer vuoto
// -----------------------------------------------------------
int  COM_rx(void)  //riceve un carattere (-1 se vuoto)
// -----------------------------------------------------------
{
   int ret;
   if (com_rx_empty()) return -1;
   ret=mcom.rxbuff[mcom.rx0] & 0xff;
   mcom.rx0=(mcom.rx0+1)%mcom.rxsize;
   return ret;
}

// -----------------------------------------------------------
//  buffer= puntatore al buffer
//	maxlen= numero massimo di caratteri
//  Se eolchar!=0 la stringa è limitata da eolchar
//  ritorna il numero di caratteri letto o -1 se vuoto
// -----------------------------------------------------------
int  COM_rxs(unsigned char * buffer,int maxlen)  //riceve una stringa (-1 se vuoto)
// -----------------------------------------------------------
{
   int ret,n=0;	
	if(mcom.eolchar)
	{
	 if(com_rx_linefull()==0)return -1;		
	}
   if(com_rx_empty()) return -1;
	  __disable_irq();
		mcom.eol=0;
	  while(((ret=COM_rx())!=-1) && (n++<maxlen))
		{
			*buffer++=(uint8_t)ret;
		}
		__enable_irq();
		*buffer=0;		
   return n;
}

// -----------------------------------------------------------
int  COM_txsl(unsigned char *c,int len)    //trasmette una stringa
// -----------------------------------------------------------
{
    int register i;
    for (i=0;i<len-1;i++)
    {
       com_tx_virtual(c[i],0);
    }
    com_tx_virtual(c[len-1],1);
    return 0;
}

// -----------------------------------------------------------
void  COM_RX_Flush(void)             //Svuota il buffer di ricezione
// -----------------------------------------------------------
{
  __disable_irq();
	mcom.rx0=0;
  mcom.rx1=0;
	mcom.eol=0;
	__enable_irq();
}

// -----------------------------------------------------------
int  COM_txs(unsigned char *c)             //trasmette una stringa fino al primo <0> escluso
// -----------------------------------------------------------
{
    return COM_txsl(c,strlen((char*)c));
}


/*******************************************************************************
* Function Name  : COM_EventHandler
* Description    : Gestisce gli eventi della seriale 
* Input          : None.
* Return         : None.
*******************************************************************************/
void COM_RX_EventHandler(void)
{	
			uint8_t c;
			c=(unsigned char) USART1_GetChar();
			//------------ ACTION
			if(mcom.eolchar)
			{
					if(mcom.eolchar==c)mcom.eol=1;
			}	 	
		 if (mcom.rx0!=((mcom.rx1+1)&(mcom.rxsize-1)))
		 {					
			  mcom.rxbuff[mcom.rx1]=c;//accoda
			  mcom.rx1=(mcom.rx1+1)&(mcom.rxsize-1);  //incrementa puntatore ultimo accod.
		 }			
	  	//------------ END ACTION		 
}

void COM_TX_EventHandler(void)
{	
	  //------------ ACTION
				 if(mcom.tx0!=mcom.tx1)   //se ci sono caratteri nel buffer software...
         {
               USART1_SendData((uint8_t) mcom.txbuff[mcom.tx0]);		  //scoda
							 mcom.tx0=(mcom.tx0+1)&(mcom.txsize-1);  //incrementa puntatore primo scod.
         }
         else                     //altrimenti fine trasmissione
         {
              mcom.txrun=0;		    //tx non attivo
							USART1_ITXEConfig(DISABLE);							
         
				 }
				 //------------ END ACTION
}




/*******************************************************************************
* Function Name  : COM_Init
* Description    : Initialize COM 
* Input          : None.
* Return         : None.
*******************************************************************************/
void COM_Init()
{
  mcom.txrun=0;
  mcom.rxsize=MAX_SERIAL_BUFFER_SIZE;
  mcom.txsize=MAX_SERIAL_BUFFER_SIZE;
  mcom.rx0=0;
  mcom.rx1=0;
  mcom.tx0=0;
  mcom.tx1=0;
  mcom.rxstatus=0;                //NESSUN MESSAGGIO RICEVUTO
  mcom.txstatus=0;                //NESSUN MESSAGGIO DA TRASMETTERE
  mcom.rxchk=0;
  mcom.txchk=0;
	mcom.eol=0;	
	mcom.eolchar=0x0D;	
	USART1_SetEventHandler(COM_RX_EventHandler,COM_TX_EventHandler);
	USART1_Init();
}


/******************* (C) COPYRIGHT 2012 ArSilicii *****END OF FILE****/
