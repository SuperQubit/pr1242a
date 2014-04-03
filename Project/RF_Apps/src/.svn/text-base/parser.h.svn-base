//---------------- TTY BSP Api

#define		TTY_cputs(x)   			COM_txs((unsigned char *)x)
#define		TTY_putchar   			COM_tx
#define 	TTY_puts(x)					COM_txs((unsigned char *)x)
#define 	TTY_CommandReset		USART1_CommandReset
#define 	TTY_CommandCheck		USART1_RxCmdTermCheck
#define		TTY_CRLF						USART1_SendCRLF


extern void PARSER_Init(void);
extern void PARSER_Clk(void);
