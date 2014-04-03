#include "hal.h"
#include "bsp.h"
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"



void Parser(char *sP)
{
	if (sP[0] == '$')
		{switch(sP[1])
		  {		
		    case 'm':
					TTY_cputs("OK\r");
        break;
        
				default:
			   TTY_cputs("UNK\r");
        break;				
       }
		}
	else  TTY_cputs("UNK\r");	
}

void PARSER_Init(void)
{
	logger_assert("READY\r");
}




uint8_t buff[64];

void PARSER_Clk(void)
{
	int len;
	if((len=COM_rxs(buff,64))!=-1)
	{		
		COM_txsl(buff,len);		
	}
	

}
