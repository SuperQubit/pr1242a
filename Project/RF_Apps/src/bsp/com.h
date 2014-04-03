/******************** (C) COPYRIGHT 2012 ArSilicii ********************
* File Name          : com.h
* Author             : DL
* Version            : V1.00
* Date               : 21-10-2012
* Description        : COM port via UART header
********************************************************************************/

#define MAX_SERIAL_BUFFER_SIZE (64)
typedef struct COM
{
  unsigned int  rx0;      
  unsigned int  rx1;      
  unsigned int  tx0;      
  unsigned int  tx1;      
  unsigned char rxbuff[MAX_SERIAL_BUFFER_SIZE]; 
  unsigned char txbuff[MAX_SERIAL_BUFFER_SIZE];  
  unsigned char rxstatus; 
  unsigned char txstatus;  
  unsigned char rxchk;    
  unsigned char txchk;      
  unsigned int  rxsize;   
  unsigned int  txsize;   
  unsigned char txrun;    // 1=tx is running
	unsigned char eolchar;
	unsigned char eol;	
} COM;

extern void COM_Init(void);
extern char COM_Puts(char *buf);

extern void COM_RxHandler(void);
extern void COM_EventHandler(void);

extern int COM_txs(unsigned char *c);
extern int COM_txsl(unsigned char *c,int len);
extern int COM_tx(unsigned char c);
extern int COM_rx(void);
extern int COM_rxs(unsigned char * buffer,int maxlen);

/******************* (C) COPYRIGHT 2012 ArSilicii *****END OF FILE****/
