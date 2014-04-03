/**
  ******************************************************************************
  * @file    htp_msgbox.c
  * @author  DL
  * @version V1.00
  * @date    26-06-2013
  * @brief   This file provides all the HTP protocol decoder firmware functions.
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; COPYRIGHT 2013 ArSilicii</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "stm32F10x.h"
#include "hal.h"
#include "bsp.h"
#include "control.h"
#include "htp_msgbox.h"


volatile tHTPMailBox HTPMsg;
volatile tHTPInFrame LastInMessage;

void HTP_MessageHandler(void)
{
		HTPMsg.InMessage.PercPower=   SPI2RX_PERCPOWER;
		HTPMsg.InMessage.Frequenza=   SPI2RX_FREQUENZA;	
	  HTPMsg.InMessage.Manipolo=    SPI2RX_MANIPOLO;
		HTPMsg.InMessage.Modulazione= SPI2RX_MODULATOR;	
#ifdef HTP_V2
		HTPMsg.InMessage.Var_Power= 			 SPI2RX_VAR_POWER;
		HTPMsg.InMessage.Var_TriggerON =   SPI2RX_VAR_TRIGON;
	  HTPMsg.InMessage.Var_TriggerOFF =  SPI2RX_VAR_TRIGOFF;
		HTPMsg.InMessage.Var_Stimulus=	   SPI2RX_VAR_STIMULUS;
#endif
	  HTPMsg.Status=1;
}

void HTP_MessageFiller(void)
{	
	if(  FGauge.PPout >=0 )HTPMsg.OutMessage.Power_Active= (uint8_t) FGauge.PPout; else  HTPMsg.OutMessage.Power_Active=0;

	#ifdef HTP_V2
			HTPMsg.OutMessage.Power_App=(uint8_t) SLOW_Gauge.PWRDC;
			HTPMsg.OutMessage.Var_Power= 	SPI2RX_VAR_POWER;
			HTPMsg.OutMessage.Var_TriggerON =   SPI2RX_VAR_TRIGON;
			HTPMsg.OutMessage.Var_TriggerOFF =  SPI2RX_VAR_TRIGOFF;
			HTPMsg.OutMessage.Var_Stimulus=	SPI2RX_VAR_STIMULUS;
	#else
			HTPMsg.OutMessage.Power_App=(uint8_t) FGauge.PAout ;
	#endif
	
	HTPMsg.OutMessage.Power_Reactive=(uint8_t) FGauge.PQout ;	
	HTPMsg.OutMessage.PercPowerActual=HTPMsg.InMessage.PercPower;
	HTPMsg.OutMessage.Frequenza=HTPMsg.InMessage.Frequenza;
	HTPMsg.OutMessage.FrequenzaReal= HTPMsg.InMessage.Frequenza ;
	HTPMsg.OutMessage.Manipolo= HTPMsg.InMessage.Manipolo;
	HTPMsg.OutMessage.ModulatoreActual= CONTROL_GetPulseStatus()? HTPMsg.InMessage.Modulazione: 0;
	HTPMsg.OutMessage.LoadStatus= CONTROL_GetLoadStatus();
	
}

void HTP_MessageSampler(void)
{	
	SPI2TX_ACTPOWERR= HTPMsg.OutMessage.Power_Active;
	SPI2TX_APPPOWER=  HTPMsg.OutMessage.Power_App;
	SPI2TX_RCTPOWER=  HTPMsg.OutMessage.Power_Reactive;
	SPI2TX_PERCPOWER= HTPMsg.OutMessage.PercPowerActual;
	SPI2TX_FREQUENZA= HTPMsg.OutMessage.Frequenza;
	SPI2TX_FREQREAL=  HTPMsg.OutMessage.FrequenzaReal;
	SPI2TX_MANIPOLOL= HTPMsg.OutMessage.Manipolo | (HTPMsg.OutMessage.LoadStatus ?128:0);
	SPI2TX_MODULATOR= HTPMsg.OutMessage.ModulatoreActual;
	#ifdef HTP_V2
		SPI2TX_VAR_POWER=   HTPMsg.OutMessage.Var_Power;
		SPI2TX_VAR_TRIGON=  HTPMsg.OutMessage.Var_TriggerON;
	  SPI2TX_VAR_TRIGOFF= HTPMsg.OutMessage.Var_TriggerOFF;
		SPI2TX_VAR_STIMULUS=HTPMsg.OutMessage.Var_Stimulus;
	#endif
}

void HTP_MBOX_Init(void) 
{
		SPI2_SetEventHandler(HTP_MessageHandler,(void *)0);
	#ifdef HTP_V2
		SPI2_SetPacketSize(12);
	#else
		SPI2_SetPacketSize(8);
	#endif
		SPI2_Init();
}

void HTP_MBOX_Clk(void)
{
	static tHTPInFrame LastFrame ={0,0,0,0};
	
	if(HTPMsg.Status &  1)
	{		
		HTP_MessageFiller();
		HTP_MessageSampler();
		HTPMsg.Status=0;
		if(HTPMsg.InMessage.Modulazione!=LastFrame.Modulazione)
		{
			LastFrame.Modulazione=HTPMsg.InMessage.Modulazione;
			HTPMsg.OutMessage.ModulatoreActual=CONTROL_SetPulse(LastFrame.Modulazione);
		}
		
		if(HTPMsg.InMessage.Manipolo !=LastFrame.Manipolo)
		{
			LastFrame.Manipolo=HTPMsg.InMessage.Manipolo;
			CONTROL_SetManipolo(LastFrame.Manipolo);
		}

		if(HTPMsg.InMessage.Frequenza !=LastFrame.Frequenza)
		{
			LastFrame.Frequenza=HTPMsg.InMessage.Frequenza;			
			CONTROL_SetFrequency(LastFrame.Frequenza*10);
		}
		
		if(HTPMsg.InMessage.PercPower !=LastFrame.PercPower)
		{
			LastFrame.PercPower=HTPMsg.InMessage.PercPower;
			CONTROL_SetPercent(LastFrame.PercPower);
		}
		
		if(HTPMsg.InMessage.Var_TriggerON != LastFrame.Var_TriggerON)
		{
			LastFrame.Var_TriggerON=HTPMsg.InMessage.Var_TriggerON;
			CONTROL_SetVarThON(LastFrame.Var_TriggerON);
		}
		
		if(HTPMsg.InMessage.Var_TriggerOFF != LastFrame.Var_TriggerOFF)
		{
			LastFrame.Var_TriggerOFF=HTPMsg.InMessage.Var_TriggerOFF;
			CONTROL_SetVarThOFF(LastFrame.Var_TriggerOFF);
		}
		
		if(HTPMsg.InMessage.Var_Stimulus != LastFrame.Var_Stimulus)
		{
			LastFrame.Var_Stimulus=HTPMsg.InMessage.Var_Stimulus;
			CONTROL_SetVarStimulus(LastFrame.Var_Stimulus);
		}
		
		if(HTPMsg.InMessage.Var_Power != LastFrame.Var_Power)
		{
			LastFrame.Var_Power=HTPMsg.InMessage.Var_Power;
			CONTROL_SetVarMaxPower(LastFrame.Var_Power);
		}
		
		if( memcmp((void *)&LastInMessage, (void *)&HTPMsg.InMessage, sizeof(LastInMessage)))
		{
				LastInMessage=HTPMsg.InMessage;
			 // logger_assert("MSG: ");
			 //	logger_print("%08X ", *(uint32_t *)(&LastInMessage));
			 //	logger_print("%08X\r", *((uint32_t *)(&LastInMessage)+1));
		}
	}
}



