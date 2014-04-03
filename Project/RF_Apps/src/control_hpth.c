/**
  ******************************************************************************
  * @file    control.c (HTP_Heuristic_1)
  * @author  DL
  * @version V1.00
  * @date    1-07-2013
  * @brief   This file provides all the HTP_Heuristic_1 firmware functions.
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; COPYRIGHT 2013 ArSilicii</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "hal.h"
#include "bsp.h"
#include "diag.h"
#include "supervisor.h"
#include "control.h"
#include "control_htph.h"
//-----------------------------


#define SET_TIMEOUT(x)	(TOCounter=x)
#define WAIT_TIMEOUT()	 if(TOCounter)break
#define CHECK_TIMEOUT()	 (TOCounter==0)
#define UPDATE_TIMEOUT() if(TOCounter)TOCounter--
#define DEFINE_TIMEOUT()	static uint16_t TOCounter=0

void SetUpProperties( const tControlSysProperties *Prop)
{
	//ConSys.SOAP=Prop->SOAP;
	 ConSys=*Prop;
	 ConSys.SOAP.pKamp=Prop->SOAP.pKamp*(DiagReport.NKAMP);
}

enum {REST=0,ONLOAD,LOADLOST,OFF,WAIT_PULSE,ONPULSE,SHORT_PULSE_PREP,SHORT_PULSE,SHORT_PULSE_EPILOG,ONPULSE_EPILOG,WAIT_STEADYSTATE} FSM_State;

//-------------------------------------- FSM BALLS
const tControlSysProperties BALLS_Off=			{	MODE_SOA,CON_NONE,		{  0.0, 0.1,10.0,8.0,				0,1.5,		0.01,0.05	}};
const tControlSysProperties BALLS_PulseStim={	MODE_SOA,CON_NONE,		{	60.0, 0.5,20.0,10.0,			0,1.5,		0.01,0.05	}};
const tControlSysProperties BALLS_Rest=			{	MODE_SOA,CON_NONE,		{	80.0, 0.5,50.0,10.0,		  0,1.5,		0.01,0.05	}};
const tControlSysProperties BALLS_Run=			{ MODE_SOA,CON_VMAXPMAX,{200.0, 1.2 ,120.0,5.0, -0.1,1.5,		0.05,0.1	}};
const tControlSysProperties BALLS_ONPulse=	{	MODE_SOA,CON_VMAXPMAX,{140.0, 1.2 ,60.0,5.0,	-0.1,1.5,		0.1,0.1		}};
void _BALLS_FSM_Clk(void)
{
			static tPulse LastPulse=PULSE_MODE_CONTINUOS;
			float val;
			DEFINE_TIMEOUT();	
			UPDATE_TIMEOUT();
		
			if(HTPH_Properties.Percent==0)
			{
					FSM_State=OFF;
			}			
			
			if(LastPulse!=HTPH_Properties.Pulse)
			{
				switch(HTPH_Properties.Pulse)
				{
						case PULSE_MODE_CONTINUOS:							
							FSM_State=REST;
							SetUpProperties(&BALLS_Rest);		
							break;
					
						case PULSE_MODE_OFF:
							FSM_State=WAIT_PULSE;
							SetUpProperties(&BALLS_PulseStim);
							break;
						
						default:
							break;
				}
		}
			
		if(HTPH_Properties.Pulse==PULSE_MODE_CONTINUOS)
		{
			//------------------------ MODALITA' CONTINUA
			switch(FSM_State)
			{
				case REST: //Rest
					if((FGauge.PPout > 4.0)	&& (FGauge.Qr < 8.0))
					{
						FSM_State=ONLOAD;
						SetUpProperties(&BALLS_Run);
					}
					else
					{
								
											
					}	
					break;
				
				case ONLOAD: //On Load
					if(FGauge.PPout < 2.0)
					{
						FSM_State=LOADLOST;						
						SET_TIMEOUT(50);
					}
					else
					{
						if(FGauge.Qr > 8.0)
						{
							FSM_State=LOADLOST;							
							SET_TIMEOUT(50);
						}
						else
						{
								ConSys.SOAP.PMAX= HTPH_Properties.Percent*BALLS_Run.SOAP.PMAX;
							  val=BALLS_Run.SOAP.PMAX-FGauge.PAout;
								if(val<0.0) val=0.0; else val=val/BALLS_Run.SOAP.PMAX;							
							  ConSys.SOAP.VDMIN= BALLS_Run.SOAP.VDMIN*val+2.5;
						}
					}	
					break;
				
				case LOADLOST: // Lost Load
					if( (FGauge.Qr < 8.0) && (FGauge.PPout > 3.0) )
					{
						FSM_State=ONLOAD;							
					}
					WAIT_TIMEOUT();
					FSM_State=REST;
					SetUpProperties(&BALLS_Rest);
					break;	
				
				case OFF:
				 if(HTPH_Properties.Percent!=0)
				 {
							FSM_State=REST;
							SetUpProperties(&BALLS_Rest);					 
				 }else
				 {
							SetUpProperties(&BALLS_Off);
				 }
				 break;
				
				 default:
					 
					 break;
			}
		}
		else
		{
			//------------------------ MODALITA' IMPULSIVA
			switch(FSM_State)
			{
				case WAIT_PULSE:
					if((FGauge.PPout > 1.0)	&& (FGauge.Qr < 8.0))
					{					
							FSM_State=ONLOAD;
					}
					else
					{
						
					}	
				  break;

			  case ONLOAD:	
				 	if((FGauge.PPout < 1.0)	|| (FGauge.Qr > 8.0))
					{
						 FSM_State=WAIT_PULSE;
						 SetUpProperties(&BALLS_PulseStim); 
					}
					else
					{
						if(HTPH_Properties.Pulse)
						{
							FSM_State=ONPULSE;
							SET_TIMEOUT(HTPH_Properties.Pulse*2);
							SetUpProperties(&BALLS_ONPulse);
							ConSys.SOAP.VMAX= HTPH_Properties.Percent*BALLS_ONPulse.SOAP.VMAX+BALLS_PulseStim.SOAP.VMAX;					
						}
					}	
					break;
			
			 case ONPULSE:
				  if( (FGauge.Qr > 8.0))
				  {
						 FSM_State=OFF;
						 SetUpProperties(&BALLS_PulseStim); 
				  }
					 WAIT_TIMEOUT();
	 				 FSM_State=OFF;
					 SetUpProperties(&BALLS_PulseStim); 
					 break;					
			 
			 case OFF:
				 
				 break;
			 
			 default:
				 break;		
			}
		}

		LastPulse=HTPH_Properties.Pulse;
}

void _BALLS_FSM_Init(void)
{
	FSM_State=REST;
	SetUpProperties(&BALLS_Rest);
}
	




//-------------------------------------- FSM PLASTIC
const tControlSysProperties PLASTIC_Off=				{	MODE_SOA,CON_NONE,		{  0.0, 0.1,10.0,8.0,				0,1.5,		0.01,0.05	}};
const tControlSysProperties PLASTIC_PulseStim=	{	MODE_SOA,CON_NONE,		{	70.0, 0.5,20.0,10.0,			0,1.5,		0.01,0.05	}};
const tControlSysProperties PLASTIC_Rest=				{	MODE_SOA,CON_NONE,		{	90.0, 0.5,50.0,10.0,		  0,1.5,		0.01,0.02	}};
const tControlSysProperties PLASTIC_Run=				{ MODE_SOA,CON_VMAXPMAX,{200.0, 1.2 ,90.0,5.0,  -0.08,1.5,		0.01,0.02	}};
const tControlSysProperties PLASTIC_ONPulse=		{	MODE_SOA,CON_VMAXPMAX,{140.0, 1.2 ,100.0,5.0,	-0.09,1.5,		0.1,0.1		}};
#define CProp_Off 			 (PLASTIC_Off)
#define CProp_PulseStim  (PLASTIC_PulseStim)
#define CProp_Rest		   (PLASTIC_Rest)
#define CProp_Run		     (PLASTIC_Run)
#define CProp_ONPulse    (PLASTIC_ONPulse)

void _PLASTIC_FSM_Clk(void)
{
			static tPulse LastPulse=PULSE_MODE_CONTINUOS;
			float val;
			DEFINE_TIMEOUT();	
			UPDATE_TIMEOUT();
		
			if(HTPH_Properties.Percent==0)
			{
					FSM_State=OFF;
			}			
			
			if(LastPulse!=HTPH_Properties.Pulse)
			{
				switch(HTPH_Properties.Pulse)
				{
						case PULSE_MODE_CONTINUOS:							
							FSM_State=REST;
							SetUpProperties(&CProp_Rest);		
							break;
					
						case PULSE_MODE_OFF:
							FSM_State=WAIT_PULSE;
							SetUpProperties(&CProp_PulseStim);
							break;
						
						default:
							break;
				}
		}
			
		if(HTPH_Properties.Pulse==PULSE_MODE_CONTINUOS)
		{
			//------------------------ MODALITA' CONTINUA
			switch(FSM_State)
			{
				case REST: //Rest
					if((FGauge.PPout > 4.0)	&& (FGauge.Qr < 8.0))
					{
						FSM_State=ONLOAD;
						SetUpProperties(&CProp_Run);
					}
					else
					{
								
											
					}	
					break;
				
				case ONLOAD: //On Load
					if(FGauge.PPout < 3.0)
					{
						FSM_State=LOADLOST;						
						SET_TIMEOUT(250);
					}
					else
					{
						if(FGauge.Qr > 10.0)
						{
							FSM_State=LOADLOST;							
							SET_TIMEOUT(250);
						}
						else
						{
								ConSys.SOAP.PMAX= HTPH_Properties.Percent*CProp_Run.SOAP.PMAX+5.0;
							  val=CProp_Run.SOAP.PMAX-FGauge.PAout;
								if(val<0.0) val=0.0; else val=val/CProp_Run.SOAP.PMAX;							
							  ConSys.SOAP.VDMIN= CProp_Run.SOAP.VDMIN*val+2.5;
						}
					}	
					break;
				
				case LOADLOST: // Lost Load
					if( (FGauge.Qr < 8.0) && (FGauge.PPout > 4.0) )
					{
						FSM_State=ONLOAD;							
					}
					WAIT_TIMEOUT();
					FSM_State=WAIT_STEADYSTATE;
					SetUpProperties(&CProp_Rest);
					SOA_Control_Init();
					SET_TIMEOUT(1000);
					break;	
				
				case WAIT_STEADYSTATE:
					WAIT_TIMEOUT();
					FSM_State=REST;
					break;
					
				case OFF:
				 if(HTPH_Properties.Percent!=0)
				 {
							FSM_State=REST;
							SetUpProperties(&CProp_Rest);					 
				 }else
				 {
							SetUpProperties(&CProp_Off);
				 }
				 break;
				
				 default:
					 
					 break;
			}
		}
		else
		{
			//------------------------ MODALITA' IMPULSIVA
			switch(FSM_State)
			{
				case WAIT_PULSE:
					if((FGauge.PPout > 1.0)	&& (FGauge.Qr < 8.0))
					{					
							FSM_State=ONLOAD;
					}
					else
					{
						
					}	
				  break;

			  case ONLOAD:	
				 	if((FGauge.PPout < 1.0)	|| (FGauge.Qr > 8.0))
					{
						 FSM_State=WAIT_PULSE;
						 SetUpProperties(&CProp_PulseStim); 
					}
					else
					{
						if(HTPH_Properties.Pulse)
						{
							FSM_State=ONPULSE;
							SET_TIMEOUT(HTPH_Properties.Pulse*2);
							SetUpProperties(&CProp_ONPulse);
							ConSys.SOAP.VMAX= HTPH_Properties.Percent*CProp_ONPulse.SOAP.VMAX+CProp_PulseStim.SOAP.VMAX;					
						}
					}	
					break;
			
			 case ONPULSE:
				  if( (FGauge.Qr > 8.0))
				  {
						 FSM_State=OFF;
						 SetUpProperties(&CProp_PulseStim); 
				  }
					 WAIT_TIMEOUT();
	 				 FSM_State=OFF;
					 SetUpProperties(&CProp_PulseStim); 
					 break;					
			 
			 case OFF:
				 
				 break;
			 
			 default:
				 break;		
			}
		}

		LastPulse=HTPH_Properties.Pulse;
}

void _PLASTIC_FSM_Init(void)
{
	FSM_State=REST;
	SetUpProperties(&CProp_Rest);
}

//-------------------------------------- FSM CERAMIC
const tControlSysProperties CERAMIC_Off=				{	MODE_SOA,CON_NONE,		{  0.0, 0.1,10.0,8.0,				0,1.5,		0.01,0.05	}};
const tControlSysProperties CERAMIC_PulseStim=	{	MODE_SOA,CON_NONE,		{	60.0, 0.5,20.0,10.0,			0,1.5,		0.01,0.05	}};
const tControlSysProperties CERAMIC_Rest=				{	MODE_SOA,CON_NONE,		{	80.0, 0.5,50.0,10.0,		  0,1.5,		0.01,0.05	}};
const tControlSysProperties CERAMIC_Run=				{ MODE_SOA,CON_VMAXPMAX,{190.0, 1.2 ,120.0,5.0, -0.08,1.5,		0.05,0.1	}};
const tControlSysProperties CERAMIC_ONPulse=		{	MODE_SOA,CON_VMAXPMAX,{140.0, 1.2 ,60.0,5.0,	-0.08,1.5,		0.1,0.1		}};
#define CProp_Off 			 (CERAMIC_Off)
#define CProp_PulseStim (CERAMIC_PulseStim)
#define CProp_Rest		   (CERAMIC_Rest)
#define CProp_Run		   (CERAMIC_Run)
#define CProp_ONPulse   (CERAMIC_ONPulse)

void _CERAMIC_FSM_Clk(void)
{
			static tPulse LastPulse=PULSE_MODE_CONTINUOS;
			float val;
			DEFINE_TIMEOUT();	
			UPDATE_TIMEOUT();
		
			if(HTPH_Properties.Percent==0)
			{
					FSM_State=OFF;
			}			
			
			if(LastPulse!=HTPH_Properties.Pulse)
			{
				switch(HTPH_Properties.Pulse)
				{
						case PULSE_MODE_CONTINUOS:							
							FSM_State=REST;
							SetUpProperties(&CProp_Rest);		
							break;
					
						case PULSE_MODE_OFF:
							FSM_State=WAIT_PULSE;
							SetUpProperties(&CProp_PulseStim);
							break;
						
						default:
							break;
				}
		}
			
		if(HTPH_Properties.Pulse==PULSE_MODE_CONTINUOS)
		{
			//------------------------ MODALITA' CONTINUA
			switch(FSM_State)
			{
				case REST: //Rest
					if((FGauge.PPout > 4.0)	&& (FGauge.Qr < 8.0))
					{
						FSM_State=ONLOAD;
						SetUpProperties(&CProp_Run);
					}
					else
					{
								
											
					}	
					break;
				
				case ONLOAD: //On Load
					if(FGauge.PPout < 2.0)
					{
						FSM_State=LOADLOST;						
						SET_TIMEOUT(50);
					}
					else
					{
						if(FGauge.Qr > 8.0)
						{
							FSM_State=LOADLOST;							
							SET_TIMEOUT(50);
						}
						else
						{
								ConSys.SOAP.PMAX= HTPH_Properties.Percent*CProp_Run.SOAP.PMAX;
							  val=CProp_Run.SOAP.PMAX-FGauge.PAout;
								if(val<0.0) val=0.0; else val=val/CProp_Run.SOAP.PMAX;							
							  ConSys.SOAP.VDMIN= CProp_Run.SOAP.VDMIN*val+2.5;
						}
					}	
					break;
				
				case LOADLOST: // Lost Load
					if( (FGauge.Qr < 8.0) && (FGauge.PPout > 3.0) )
					{
						FSM_State=ONLOAD;							
					}
					WAIT_TIMEOUT();
					FSM_State=REST;
					SetUpProperties(&CProp_Rest);
					break;	
				
				case OFF:
				 if(HTPH_Properties.Percent!=0)
				 {
							FSM_State=REST;
							SetUpProperties(&CProp_Rest);					 
				 }else
				 {
							SetUpProperties(&CProp_Off);
				 }
				 break;
				
				 default:
					 
					 break;
			}
		}
		else
		{
			//------------------------ MODALITA' IMPULSIVA
			switch(FSM_State)
			{
				case WAIT_PULSE:
					if((FGauge.PPout > 1.0)	&& (FGauge.Qr < 8.0))
					{					
							FSM_State=ONLOAD;
					}
					else
					{
						
					}	
				  break;

			  case ONLOAD:	
				 	if((FGauge.PPout < 1.0)	|| (FGauge.Qr > 8.0))
					{
						 FSM_State=WAIT_PULSE;
						 SetUpProperties(&CProp_PulseStim); 
					}
					else
					{
						if(HTPH_Properties.Pulse)
						{
							FSM_State=ONPULSE;
							SET_TIMEOUT(HTPH_Properties.Pulse*2);
							SetUpProperties(&CProp_ONPulse);
							ConSys.SOAP.VMAX= HTPH_Properties.Percent*CProp_ONPulse.SOAP.VMAX+CProp_PulseStim.SOAP.VMAX;					
						}
					}	
					break;
			
			 case ONPULSE:
				  if( (FGauge.Qr > 8.0))
				  {
						 FSM_State=OFF;
						 SetUpProperties(&CProp_PulseStim); 
				  }
					 WAIT_TIMEOUT();
	 				 FSM_State=OFF;
					 SetUpProperties(&CProp_PulseStim); 
					 break;					
			 
			 case OFF:
				 
				 break;
			 
			 default:
				 break;		
			}
		}

		LastPulse=HTPH_Properties.Pulse;
}

void _CERAMIC_FSM_Init(void)
{
	FSM_State=REST;
	SetUpProperties(&CProp_Rest);
}

//-------------------------------------- FSM PENNAIL
const tControlSysProperties PENNAIL_Off=				{	MODE_SOA,CON_NONE,		{  0.0, 0.1,10.0,8.0,				0,1.5,		0.01,0.05	}};
const tControlSysProperties PENNAIL_PulseStim=	{	MODE_SOA,CON_NONE,		{	60.0, 0.5,10.0,10.0,			0,1.5,		0.01,0.05	}};
const tControlSysProperties PENNAIL_PulsePrep=	{	MODE_SOA,CON_NONE,		{	60.0, 0.5,10.0,10.0,			0,1.5,		0.01,0.05	}};
const tControlSysProperties PENNAIL_ShortPulse=	{	MODE_RAW,CON_NONE,		{	60.0, 0.5,20.0,10.0,			0,1.5,		0.01,0.05	}};
const tControlSysProperties PENNAIL_Rest=				{	MODE_SOA,CON_NONE,		{	80.0, 0.5,50.0,10.0,		  0,1.5,		0.01,0.05	}};
const tControlSysProperties PENNAIL_Run=				{ MODE_SOA,CON_VMAXPMAX,{190.0, 1.2 ,100.0,5.0, -0.1,1.5,		0.05,0.1	}};
const tControlSysProperties PENNAIL_ONPulse=		{	MODE_SOA,CON_VMAXPMAX,{140.0, 1.2 ,60.0,5.0,	-0.1,1.5,		0.1,0.1		}};

#define CProp_Off 			 (PENNAIL_Off)
#define CProp_PulseStim  (PENNAIL_PulseStim)
#define CProp_Rest		   (PENNAIL_Rest)
#define CProp_Run		     (PENNAIL_Run)
#define CProp_ONPulse    (PENNAIL_ONPulse)
#define CProp_ShortPulse (PENNAIL_ShortPulse)
#define CProp_PulsePrep  (PENNAIL_PulsePrep)

volatile RAWSetPoint LastSetPoint;

void _PENNAIL_FSM_Clk(void)
{
			static tPulse LastPulse=PULSE_MODE_CONTINUOS;
			float val;
			DEFINE_TIMEOUT();	
			UPDATE_TIMEOUT();
		
			if(HTPH_Properties.Percent==0)
			{
					FSM_State=OFF;
			}			
			
			if(LastPulse!=HTPH_Properties.Pulse)
			{
				switch(HTPH_Properties.Pulse)
				{
						case PULSE_MODE_CONTINUOS:							
							FSM_State=REST;
							SetUpProperties(&CProp_Rest);		
							break;
					
						case PULSE_MODE_OFF:
							FSM_State=WAIT_PULSE;
							SetUpProperties(&CProp_PulseStim);
							break;
						
						default:
							break;
				}
		}
			
		if(HTPH_Properties.Pulse==PULSE_MODE_CONTINUOS)
		{
			//------------------------ MODALITA' CONTINUA
			switch(FSM_State)
			{
				case REST: //Rest
					if((FGauge.PPout > 1.5)	&& (FGauge.Qr < 8.0))
					{
						FSM_State=ONLOAD;
						SetUpProperties(&CProp_Run);
					}
					else
					{
								
											
					}	
					break;
				
				case ONLOAD: //On Load
					if(FGauge.PPout < 1.0)
					{
						FSM_State=LOADLOST;						
						SET_TIMEOUT(50);
					}
					else
					{
						if(FGauge.Qr > 8.0)
						{
							FSM_State=LOADLOST;							
							SET_TIMEOUT(50);
						}
						else
						{
								ConSys.SOAP.PMAX= HTPH_Properties.Percent*CProp_Run.SOAP.PMAX;
							  val=CProp_Run.SOAP.PMAX-FGauge.PAout;
								if(val<0.0) val=0.0; else val=val/CProp_Run.SOAP.PMAX;							
							  ConSys.SOAP.VDMIN= CProp_Run.SOAP.VDMIN*val+2.5;
						}
					}	
					break;
				
				case LOADLOST: // Lost Load
					if( (FGauge.Qr < 8.0) && (FGauge.PPout > 1.0) )
					{
						FSM_State=ONLOAD;							
					}
					WAIT_TIMEOUT();
					FSM_State=REST;
					SetUpProperties(&CProp_Rest);
					break;	
				
				case OFF:
				 if(HTPH_Properties.Percent!=0)
				 {
							FSM_State=REST;
							SetUpProperties(&CProp_Rest);					 
				 }else
				 {
							SetUpProperties(&CProp_Off);
				 }
				 break;
				
				 default:
					 
					 break;
			}
		}
		else
		{
			//------------------------ MODALITA' IMPULSIVA
			switch(FSM_State)
			{
				case WAIT_PULSE:
					if((FGauge.PPout > 0.6)	&& (FGauge.Qr < 10.0))
					{					
						FSM_State=ONLOAD;
						SetUpProperties(&CProp_PulsePrep);						
					}
					else
					{
						
					}	
				  break;

			  case ONLOAD:	
				 	if((FGauge.PPout < 0.6)	|| (FGauge.Qr > 10.0))
					{
						 FSM_State=WAIT_PULSE;
						 SetUpProperties(&CProp_PulseStim);
					}
					else
					{
						if(HTPH_Properties.Pulse)
						{
							
							if(HTPH_Properties.Pulse > 50)
							{
								FSM_State=ONPULSE;
								LastSetPoint=ActualSetPoint;
								SET_TIMEOUT(HTPH_Properties.Pulse*2);
								SetUpProperties(&CProp_ONPulse);
								ConSys.SOAP.VMAX= HTPH_Properties.Percent*CProp_ONPulse.SOAP.VMAX+CProp_PulseStim.SOAP.VMAX;					
							}
							else
							{								
								FSM_State=SHORT_PULSE_PREP;
								SET_TIMEOUT(8);
								//--------------------------
								SetUpProperties(&CProp_ShortPulse);
								LastSetPoint=ActualSetPoint;
								ActualSetPoint.PWR_VBAR=LastSetPoint.PWR_VBAR+(unsigned short)(800*HTPH_Properties.Percent);										
								if(ActualSetPoint.PWR_VBAR>3200) ActualSetPoint.PWR_VBAR=3200;
								//ActualSetPoint.BIAS_SET=LastSetPoint.BIAS_SET-300;
								//--------------------------
							}							
							
						}
					}	
					break;
			 case SHORT_PULSE_PREP:
					WAIT_TIMEOUT();
					ActualSetPoint.BIAS_SET=LastSetPoint.BIAS_SET-(unsigned short)(((300*HTPH_Properties.Percent)+200)*DiagReport.NKAMP);			 
					ActualSetPoint.DDS_AMPL=(unsigned short)(2800-HTPH_Properties.Percent*2000);
					FSM_State=SHORT_PULSE;
					SET_TIMEOUT(HTPH_Properties.Pulse*2);			 
					break;
			 
			 
			 case SHORT_PULSE:
					WAIT_TIMEOUT();
	 				FSM_State=SHORT_PULSE_EPILOG;
					ActualSetPoint=LastSetPoint;
					SET_TIMEOUT(15);		
					break;	

			case SHORT_PULSE_EPILOG:
					WAIT_TIMEOUT();
	 				FSM_State=OFF;
					SetUpProperties(&CProp_PulseStim);  
					break;					 
					
			 case ONPULSE:
				  if( (FGauge.Qr > 8.0))
				  {
						 FSM_State=ONPULSE_EPILOG;
						 ActualSetPoint=LastSetPoint;												
						 SetUpProperties(&CProp_PulseStim); 
				  }
					 WAIT_TIMEOUT();
	 				 FSM_State=ONPULSE_EPILOG;
					 ActualSetPoint=LastSetPoint;						
					 SetUpProperties(&CProp_PulseStim); 
					 SET_TIMEOUT(15);	
					 break;					
			 
			case ONPULSE_EPILOG:
					 WAIT_TIMEOUT();
	 				 FSM_State=OFF;
					 SetUpProperties(&CProp_PulseStim); 
					 break;									
					
			 case OFF:
				 
				 break;
			 
			 default:
				 break;		
			}
		}

		LastPulse=HTPH_Properties.Pulse;
}

void _PENNAIL_FSM_Init(void)
{
	FSM_State=REST;
	SetUpProperties(&CProp_Rest);
}






//-------------------------------------- FSM EMI
const tControlSysProperties EMI_Off=				{	MODE_SOA,CON_NONE,		{  0.0, 0.1,10.0,8.0,				0,1.5,		0.01,0.05	}};
const tControlSysProperties EMI_Rest=				{	MODE_SOA,CON_NONE,		{	60.0, 0.5,50.0,8.0,		  0,1.5,		0.01,0.05	}};
const tControlSysProperties EMI_Run=				{ MODE_SOA,CON_VMAXPMAX,{120.0, 1.2 ,60.0,10.0,    -0.2,1.5,		0.05,0.1	}};

#define CProp_Off 			 (EMI_Off)
#define CProp_Rest		   (EMI_Rest)
#define CProp_Run		     (EMI_Run)

void _EMI_FSM_Clk(void)
{
			DEFINE_TIMEOUT();	
			UPDATE_TIMEOUT();
		
			if(HTPH_Properties.Percent==0)
			{
					FSM_State=OFF;
			}			
			
			//------------------------ MODALITA' CONTINUA
			switch(FSM_State)
			{
				case REST: //Rest
					if((FGauge.PPout > 1.5)	&& (FGauge.Qr < 8.0))
					{
						FSM_State=ONLOAD;
						SetUpProperties(&CProp_Run);
					}
					else
					{
								
											
					}	
					break;
				
				case ONLOAD: //On Load

  				if(FGauge.PPout < 1.0)
					{
						FSM_State=LOADLOST;						
						SET_TIMEOUT(5);
					}
					else
					{
						if(FGauge.Qr > 8.0)
						{
							FSM_State=LOADLOST;							
							SET_TIMEOUT(5);
						}
						else
						{
							ConSys.SOAP.PMAX= HTPH_Properties.Percent*CProp_Run.SOAP.PMAX;		
							if( HTPH_Properties.Percent < 0.8)
							{								
								ConSys.SOAP.pKamp= CProp_Run.SOAP.pKamp/(HTPH_Properties.Percent*0.5+0.6)*(DiagReport.NKAMP);
							}
							else
							{
								ConSys.SOAP.pKamp= CProp_Run.SOAP.pKamp*(DiagReport.NKAMP);
							}
							
							
							if(((HTPH_Properties.Frequency/10) & 1) ==1) 
							{
								GPIO_SetBits(GPIOB, GPIO_Pin_4);
							}
							else
							{
								GPIO_ResetBits(GPIOB, GPIO_Pin_4);
							}
							
						}
					}	
					break;
				
				case LOADLOST: // Lost Load
					if( (FGauge.Qr < 8.0) && (FGauge.PPout > 1.0) )
					{
						FSM_State=ONLOAD;							
					}
					WAIT_TIMEOUT();
					FSM_State=REST;
					SetUpProperties(&CProp_Rest);
					break;	
				
				case OFF:
				 if(HTPH_Properties.Percent!=0)
				 {
							FSM_State=REST;
							SetUpProperties(&CProp_Rest);					 
				 }else
				 {
							SetUpProperties(&CProp_Off);
				 }
				 break;
				
				 default:
					 
					 break;
			}
}

void _EMI_FSM_Init(void)
{
	FSM_State=REST;
	SetUpProperties(&CProp_Rest);
}

//-------------------------------------- FSM NO HAND
const tControlSysProperties NOHAND_Off=				{	MODE_SOA,CON_NONE,		{  0.0, 0.1,10.0,8.0,				0,1.5,		0.01,0.05	}};

#define CProp_Off 			 (NOHAND_Off)

void _NOHAND_FSM_Clk(void)
{

}

void _NOHAND_FSM_Init(void)
{
	FSM_State=REST;
	DDS_SPREAD_OFF();
	SetUpProperties(&CProp_Off);
}
//-------------------------------------- HTP_Heuristic_1 (HTPH1) Control
tHTPH_Properties HTPH_Properties;
uint8_t CONTROL_GetLoadStatus(void)
{
	return((FSM_State==ONLOAD)  || (FSM_State==ONPULSE));
}
uint8_t CONTROL_GetPulseStatus(void)
{
	return(FSM_State==ONPULSE);
}

void CONTROL_HTPH_Clk(void)
{		
	LOAD_Analyzer();
	switch(HTPH_Properties.Handpiece)
	{
		case NO_HANDPIECE:
				_NOHAND_FSM_Clk();
				break;
	
		case PLASTIC:
				_PLASTIC_FSM_Clk();
				break;
		case CERAMIC:
				_CERAMIC_FSM_Clk();
				break;
		case BALLS:
				_BALLS_FSM_Clk();
				break;
		case PEN:
				_PENNAIL_FSM_Clk();
				break;
		case NAIL:
				_EMI_FSM_Clk();
				break;
	}		
}

void CONTROL_HTPH_Init(void)
{	
	switch(HTPH_Properties.Handpiece)
	{
		case NO_HANDPIECE:
				_NOHAND_FSM_Init();
				break;
		case PLASTIC:
			  _PLASTIC_FSM_Init();
				break;
		case CERAMIC:
				_CERAMIC_FSM_Init();
				break;
		case BALLS:
				_BALLS_FSM_Init();
				break;
		case PEN:
				_PENNAIL_FSM_Init();
			  break;
		case NAIL:
				_EMI_FSM_Init();
				break;
	}			
}


uint16_t CONTROL_SetPulse(tPulse pulse)
{
	HTPH_Properties.Pulse=pulse;
	return(0);
}

void CONTROL_SetManipolo(tHandpiece handpiece)
{
	  if (HTPH_Properties.Handpiece!=handpiece)
		{
				HTPH_Properties.Handpiece=handpiece;
				CONTROL_HTPH_Init();
		}			
}


/**
  * @}
  */

/******************* (C) COPYRIGHT 2013 ArSilicii *****END OF FILE****/
