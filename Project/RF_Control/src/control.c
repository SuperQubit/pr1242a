/**
  ******************************************************************************
  * @file    control.c
  * @author  DL
  * @version V1.00
  * @date    22-04-2013
  * @brief   This file provides all the Control System firmware functions.
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




/** @defgroup CONTROL_Private_Functions
  * @{
  */

/**
  * @}
  */

/** @defgroup CONTROL_Public_Functions
  * @{
  */
tControlSysProperties ConSys,BaseConSys;
RAWSetPoint ActualSetPoint={1400,1800,2500};
const RAWSetPoint InitialSetPoint={1400,1800,2500};

//#define VOUT_MAX

//-------------------------------------- CONTROL SAMPLER
void CONTROL_Output(void)
{	
		DDS_AmplitudeSet(ActualSetPoint.DDS_AMPL);
		if(DDS_AmplitudeGet() < 800) BIAS_VAUX_SetValue(1); else BIAS_VAUX_SetValue(0);
  	SUPPLY_SetValue(ActualSetPoint.PWR_VBAR);
		BIAS_SetValue(ActualSetPoint.BIAS_SET);
		TIM4_PWM_Clk();
}

void CONTROL_Input(void)
{	

}

//------------------------------ LoadAnalyzer
tLoadProperties LoadProperties;
const tLoadProperties TableProperties[]=
{		//TYPE,CMODE,VMAX,IMAX,PMAX
		{UNKNOWN,CON_VMAX,200.0,0.5,40.0,12.0},
		{LIGHT,CON_VMAX,200.0,0.5,50.0,12.0},
		{RESISTIVE,CON_VMAXPMAX,120.0,1.5,150.0,8.0},
		{CAPACITIVE_SAFE,CON_VMAXPMAX,100.0,1.5,120.0,8.0},
		{CAPACITIVE,CON_VMAXPMAX,60.0,0.5,40.0,10.0}
};

tLoadType	LOAD_Analyzer(void)
{	
	if(FGauge.PAout< 1.0)
	{
		LoadProperties=TableProperties[UNKNOWN];
		return(UNKNOWN);
	}
	else
	{
			if(FGauge.PAout < 5.0)
			{
				LoadProperties=TableProperties[LIGHT];
				return(LIGHT);
			}			
			if(FGauge.Qr > 6)
			{
				LoadProperties=TableProperties[CAPACITIVE];
				return(CAPACITIVE);				
			}
			if(FGauge.Qr > 0.5)
			{
				LoadProperties=TableProperties[CAPACITIVE_SAFE];
				LoadProperties.VMAX-=FGauge.Qr*6.0;
				return(CAPACITIVE_SAFE);				
			}
			else
			{
				LoadProperties=TableProperties[RESISTIVE];
				return(RESISTIVE);				
			}						
	}
	return(UNKNOWN);
}

//-------------------------------------- SOA Control Routine
//--------
//--------

#define  VOUT_MAX 	(ConSys.SOAP.VMAX)
#define  IOUT_MAX 	(ConSys.SOAP.IMAX)
#define  POUT_MAX 	(ConSys.SOAP.PMAX)
#define  VDMIN_SET 	(ConSys.SOAP.VDMIN)
#define  fKamp 			(ConSys.SOAP.pKamp)
#define  fKdT 		  (ConSys.SOAP.pKdT)
#define  Kiup 	    (ConSys.SOAP.pKiup)
#define  Kidw 	    (ConSys.SOAP.pKidw)

//--------  VBAR Controller
#define VBAR_MAX			(VBAR2PWM(40))
#define VBAR_MIN			(VBAR2PWM(5))
#define VDMIN_MIS			(L4)
tSetPointResult VBAR_PIDController(uint16_t  VDMin_SetPoint)
{
		uint16_t NewSetPoint;
		uint16_t delta;	
	
		NewSetPoint=ActualSetPoint.PWR_VBAR;
		if(VDMIN_MIS>VDMin_SetPoint)
		{
				delta= VDMIS2PWMBAR(VDMIN_MIS-VDMin_SetPoint);
			  if(delta>=NewSetPoint)
				{
						NewSetPoint=0; 
				}
				else
				{
						NewSetPoint-=delta; 
				}							
		}
		else
		{
				delta= VDMIS2PWMBAR(VDMin_SetPoint-VDMIN_MIS);
				NewSetPoint+=delta; 
		}
		ActualSetPoint.PWR_VBAR=NewSetPoint;
		if(NewSetPoint> VBAR_MAX) ActualSetPoint.PWR_VBAR= VBAR_MAX; 
		if(NewSetPoint< VBAR_MIN) ActualSetPoint.PWR_VBAR= VBAR_MIN;
		return(SP_OK);
}

//--------  Amplitude PID Controller
#define VDDS_MAX			(3500)
#define VDDS_MIN			(800)
//#define VOUT_MIS			(M1-L1)
#define VOUT_MIS			(Vopp_i)
#define IOUT_MIS			(M2-L2)
#define DPID_DDS_DW(d)			((uint16_t)((d)*Kidw))
#define DPID_DDS_UP(d)			((uint16_t)((d)*Kiup))
tSetPointResult Amplitude_PIDController(uint16_t SetPoint)
{
	uint16_t NewSetPoint;
	uint16_t MisSample;
	uint16_t delta;
	
		MisSample=VOUT_MIS;
		NewSetPoint=ActualSetPoint.DDS_AMPL;
		
		if(MisSample>SetPoint)
		{
				//NewSetPoint+=1;
				delta= DPID_DDS_DW(MisSample-SetPoint);
				NewSetPoint+=delta; 
				if(delta> 100) {
						if(ActualSetPoint.PWR_VBAR < (VBAR_MIN+delta)) 
							ActualSetPoint.PWR_VBAR= VBAR_MIN;
						else
							ActualSetPoint.PWR_VBAR-=delta;					
				}
		}
		else
		{
			
				delta= DPID_DDS_UP(SetPoint-MisSample);
				if(delta>=NewSetPoint)
				{
						NewSetPoint=0; 
				}
				else
				{
						NewSetPoint-=delta; 
				}
			
				//if(NewSetPoint>1) NewSetPoint-=1; else NewSetPoint=0;
		}
		
		ActualSetPoint.DDS_AMPL=NewSetPoint;
		if(ActualSetPoint.DDS_AMPL > VDDS_MAX) ActualSetPoint.DDS_AMPL= VDDS_MAX; 
		if(ActualSetPoint.DDS_AMPL < VDDS_MIN) ActualSetPoint.DDS_AMPL= VDDS_MIN;
		
		//ActualSetPoint.PWR_VBAR= VBAR_MAX;
		return(SP_OK);
		
}

tSetPointResult Amplitude_IV_PIDController(uint16_t VSetPoint,uint16_t ISetPoint)
{
	uint16_t NewSetPoint;
	uint16_t delta;
	
	int32_t VSample,ISample;
	int32_t Error;
	
	
		VSample=VOUT_MIS;
		ISample=IOUT_MIS;
		NewSetPoint=ActualSetPoint.DDS_AMPL;
		Error=  (VSample-VSetPoint) + ((ISample>ISetPoint)? (ISample-ISetPoint):0);
	
		if(Error>0)
		{
				//NewSetPoint+=1;
				delta= (uint16_t)DPID_DDS_DW(Error);
				NewSetPoint+=delta; 				
		}
		else
		{			
				delta= (uint16_t) DPID_DDS_UP(-Error);
				if(delta>=NewSetPoint)
				{
						NewSetPoint=0; 
				}
				else
				{
						NewSetPoint-=delta; 
				}							
		}
		
		ActualSetPoint.DDS_AMPL=NewSetPoint;
		if(ActualSetPoint.DDS_AMPL > VDDS_MAX) ActualSetPoint.DDS_AMPL= VDDS_MAX; 
		if(ActualSetPoint.DDS_AMPL < VDDS_MIN) ActualSetPoint.DDS_AMPL= VDDS_MIN;
		
		//ActualSetPoint.PWR_VBAR= VBAR_MAX;
		return(SP_OK);
		
}

#define PSLIDE_UP			(20)
#define PSLIDE_DW			(20)
#define PoutC8		(VoutC*IoutC*256)
tSetPointResult Amplitude_PIV_PIDController(uint16_t PSetPoint, uint16_t VMAXSetPoint,uint16_t IMAXSetPoint)
{
	uint16_t NewSetPoint;
	uint16_t delta;
	
	int32_t VSample,ISample,PASample;
	int32_t Error;
	
	
		VSample=VOUT_MIS;
		ISample=IOUT_MIS;
		PASample= (VSample*ISample)>>8;	
		NewSetPoint=ActualSetPoint.DDS_AMPL;

		Error=0;
		if(VSample>VMAXSetPoint)
		{
				Error=VSample-VMAXSetPoint;
		}
		if(ISample>IMAXSetPoint)
		{
				Error+=ISample-IMAXSetPoint;
		}		
		//if(Error==0) Error=(PASample>PSetPoint)? PSLIDE_DW:-PSLIDE_UP;
		if(Error==0) Error=(PASample>PSetPoint)? (PASample-PSetPoint)/16:(PASample-PSetPoint)/16;
		//((ISample>IMAXSetPoint)? (ISample-IMAXSetPoint):0) + ((VSample>VMAXSetPoint)? (VSample-VMAXSetPoint)*8:0);
	
		if(Error>0)
		{
  			//NewSetPoint+=1;
				delta= (uint16_t)DPID_DDS_DW(Error);
				NewSetPoint+=delta; 				
		}
		else
		{			
				delta= (uint16_t) DPID_DDS_UP(-Error);
				if(delta>=NewSetPoint)
				{
						NewSetPoint=0; 
				}
				else
				{
						NewSetPoint-=delta; 
				}							
		}
		
		ActualSetPoint.DDS_AMPL=NewSetPoint;
		if(ActualSetPoint.DDS_AMPL > VDDS_MAX) ActualSetPoint.DDS_AMPL= VDDS_MAX; 
		if(ActualSetPoint.DDS_AMPL < VDDS_MIN) ActualSetPoint.DDS_AMPL= VDDS_MIN;		
		//ActualSetPoint.PWR_VBAR= VBAR_MAX;
		return(SP_OK);
		
}


//--------  BIAS Optimizer Controller
#define VBIAS_DELTA_UP			(800)
#define VBIAS_DELTA_DW			(-1400)
#define KAMPDDS_BIAS				(fKamp)
#define KDELTAT_BIAS				(fKdT)

uint16_t VBIAS_SETDYN;
tSetPointResult BIASOptimizer_Controller(int16_t Offset)
{
		int16_t delta;
		int32_t DDSAmp;

		DDSAmp=VDDS_MAX-ActualSetPoint.DDS_AMPL;
		if(DDSAmp<0) DDSAmp=0;
	
		delta=(int32_t)((DDSAmp)*KAMPDDS_BIAS+(TJp-DiagReport.TINI)*KDELTAT_BIAS+Offset);	
		if(delta> VBIAS_DELTA_UP)	delta=VBIAS_DELTA_UP;
		else
		if(delta< VBIAS_DELTA_DW) delta=VBIAS_DELTA_DW;	
		DDSAmp=delta+ DiagReport.VBIASP_BORDER;
		if(DDSAmp>0) VBIAS_SETDYN= DDSAmp; else VBIAS_SETDYN=0;
		
		ActualSetPoint.BIAS_SET= VBIAS_SETDYN;
		return(SP_OK);
}

void SOA_Control_Init()
{
		ActualSetPoint=InitialSetPoint;
}

void SOA_Control_SetIC(RAWSetPoint IC)
{	
	ActualSetPoint=IC;
}

void SOA_Control_Clk()
{
	
		VBAR_PIDController( VD2MIS(VDMIN_SET));
		Amplitude_PIV_PIDController((uint16_t)(POUT_MAX/PoutC8),(uint16_t)(VOUT_MAX/VoutC), (uint16_t)(IOUT_MAX/IoutC));
		BIASOptimizer_Controller(0);
}

//-------------------------------------- PRECOGNITIVE SOA Control
//--------
//--------
uint16_t Percent;
void CONTROL_Clk_PSOAMAXPWR(void)
{	
	switch(LOAD_Analyzer())
	{
		case LIGHT:
		case RESISTIVE:
		case CAPACITIVE_SAFE:
		case CAPACITIVE:
		case UNKNOWN:
			switch(LoadProperties.Mode)
			{
				default:
				case CON_NONE:
						break;
				case CON_VMAX:
						VOUT_MAX=(Percent*LoadProperties.VMAX/100.0);
						POUT_MAX=LoadProperties.PMAX;
						IOUT_MAX=LoadProperties.IMAX;
						VDMIN_SET=(LoadProperties.VDMIN);
						break;
				case CON_IMAX:
						IOUT_MAX=(Percent*LoadProperties.IMAX/100.0);
						POUT_MAX=LoadProperties.PMAX;
						VOUT_MAX=LoadProperties.VMAX;
						VDMIN_SET=(LoadProperties.VDMIN);
						break;
				case CON_PMAX:
						VOUT_MAX=(Percent*LoadProperties.VMAX/100.0);
						POUT_MAX=LoadProperties.PMAX;
						IOUT_MAX=LoadProperties.IMAX;
						VDMIN_SET=(LoadProperties.VDMIN);
						break;
				case CON_VMAXPMAX:
						POUT_MAX=(Percent*LoadProperties.PMAX/100.0);
				    if(Percent)	VOUT_MAX=(Percent*LoadProperties.VMAX/100.0)+80.0; else VOUT_MAX=0;
						IOUT_MAX=LoadProperties.IMAX;
						VDMIN_SET=(LoadProperties.VDMIN);
						break;			
			}
			break;		
		default:
				VOUT_MAX=80.0;
				VDMIN_SET=10.0;
				IOUT_MAX=0.7;
				POUT_MAX=20.0;
				break;
	}
	VBAR_PIDController( VD2MIS(VDMIN_SET));
	Amplitude_PIV_PIDController((uint16_t)(POUT_MAX/PoutC8),(uint16_t)(VOUT_MAX/VoutC), (uint16_t)(IOUT_MAX/IoutC));
	BIASOptimizer_Controller(0);
	CONTROL_Output();
}

void CONTROL_Init_PSOAMAXPWR(void)
{
	VOUT_MAX=80.0;
	VDMIN_SET=10.0;
	IOUT_MAX=0.7;
	POUT_MAX=20.0;
	Percent=0;	
}

//-------------------------------------- CLASS_A Control
void CONTROL_Clk_CLASS_A(void)
{
	
	
}

//-------------------------------------- FVIP Control
void CONTROL_Clk_FVIP(void)
{		
	LOAD_Analyzer();
	
	VBAR_PIDController( VD2MIS(VDMIN_SET));
	Amplitude_PIV_PIDController((uint16_t)(POUT_MAX/PoutC8),(uint16_t)(VOUT_MAX/VoutC), (uint16_t)(IOUT_MAX/IoutC));
	BIASOptimizer_Controller(0);
	
	CONTROL_Output();
}

void CONTROL_Init_FVIP(void)
{
	VOUT_MAX=80.0;
	VDMIN_SET=10.0;
	IOUT_MAX=0.7;
	POUT_MAX=20.0;
	Percent=0;	
}

//-------------------------------------- CONTROL SOLVER
tControlMethod Method;

void CONTROL_Init(void) 
{	
	CONTROL_Setup(NONE,NONE);
	ConSys.SOAP.pKamp=-0.05;
	ConSys.SOAP.pKdT=1.5;
	ConSys.SOAP.pKiup=0.05;
	ConSys.SOAP.pKidw=0.1;
}


void CONTROL_Clk(void) 
{
	switch(Method)
	{
		case NONE:
			
				break;
		
		case TABLE_1:			
//			  CONTROL_Clk_TABLE_1();				
				break;
		
		case PRECOG_SOA_MAXPWR:
				CONTROL_Clk_PSOAMAXPWR();		
				break;
		
		case FVIP:
				CONTROL_Clk_FVIP();	
				break;
		
		case HTPH1:			
			  CONTROL_HTPH_Clk();	
				if(ConSys.ControlMode == MODE_SOA)
				{
					SOA_Control_Clk();
				}
				CONTROL_Output();		
				break;		
	}	
}


void CONTROL_SetPercent(uint16_t percent)
{	
	Percent=percent;
	HTPH_Properties.Percent=((float)percent)/100.0;
}


void CONTROL_SetFrequency(uint16_t frequency)
{	
	if(HTPH_Properties.Frequency != frequency)
	{
		HTPH_Properties.Frequency= frequency;
		if(frequency==0)	{frequency=1000;}
		if(frequency<300) { frequency=300;}
		if(frequency>1200){ frequency=1200;}
		DDS_FRQ_Set(frequency);
	}	
}

void CONTROL_Setup(tControlMethod pMethod, tControlParameter pInitParameter)
{
		switch(pMethod)
		{
				case NONE:
						break;
				
				case PRECOG_SOA_MAXPWR:
					  CONTROL_Init_PSOAMAXPWR();				
						break;
				
				case FVIP:
						CONTROL_Init_FVIP();				
						break;
				
				case HTPH1:
						CONTROL_HTPH_Init();				
						break;
				
				default:
						break;
		}	
		Method=pMethod;		
}


/**
  * @}
  */

/******************* (C) COPYRIGHT 2013 ArSilicii *****END OF FILE****/
