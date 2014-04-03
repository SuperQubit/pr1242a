//------------------------------- TABLE_1
#define MAXNUMVALUE (11)
const RAWSetPoint OC_LOOKTBL[MAXNUMVALUE]=
{
	{3500,400,2450},	
	{2980,600,2450},
	{2880,800,2450},
	{2850,1500,2450},
	{2800,1650,2400},
	{2700,1700,2400},
	{2700,2200,2400},
	{2450,2400,2600},
	{2380,2500,2600},
	{1980,2500,2920},
	{1900,2500,2900}
//	{1980,2600,2920},
//	{1900,2700,2900}
};


const RAWSetPoint CR300_LOOKTBL[MAXNUMVALUE]=
{
//	{2600,800,2200},
	{2600,400,2450},	
	{2400,1000,2150},
	{2300,1400,2100},
	{2250,1500,2050},
	{2200,1750,2000},
	{2120,1800,2000},
	{2050,1850,2000},
	{1950,1900,2000},
	{1880,1950,2000},
	{1800,2000,2000},
	{1700,2100,2000}
};

const RAWSetPoint CR150_LOOKTBL[MAXNUMVALUE]=
{
//	{2600,800,2200},
	{2600,800,2450},	
	{2400,1000,2150},
	{2300,1400,2100},
	{2250,1500,2050},
	{2200,1500,2000},
	{2120,1500,2000},
	{2050,1550,2000},
	{1950,1550,2000},
	{1880,1550,2000},
	{1800,1600,2000},
	{1700,1600,2000}
};

/*
const RAWSetPoint CR300_LOOKTBL[MAXNUMVALUE]=
{
//	{2600,800,2200},
	{2600,400,2450},	
	{2400,1000,2450},
	{2300,1400,2450},
	{2250,1500,2450},
	{2200,1750,2400},
	{2120,1800,2400},
	{2050,1850,2400},
	{1950,1900,2400},
	{1880,1950,2400},
	{1800,2000,2400},
	{1700,2100,2400}
	
};
*/


const RAWSetPoint CalibrationSetPoint={3800,1800,2482};
uint16_t lastPercent=0;

void CONTROL_CalcOCSetPoint(uint16_t percent)
{
			uint8_t idx,dx;			
			int32_t	x0,x1;
			RAWSetPoint X0,X1;	    
	    if(percent==0)
			{				
				ActualSetPoint=CalibrationSetPoint;
			}
			else
			{
				idx=percent/10;
				dx=percent%10;
				X0=OC_LOOKTBL[idx];
				X1=OC_LOOKTBL[idx+1];
				//DDS_AMPL
				x0=X0.DDS_AMPL;
				x1=X1.DDS_AMPL;
				ActualSetPoint.DDS_AMPL=  (uint16_t)(x0*(10-dx)+x1*dx)/10;
				//PWR_VBAR
				x0=X0.PWR_VBAR;
				x1=X1.PWR_VBAR;			
				ActualSetPoint.PWR_VBAR=  (uint16_t)(x0*(10-dx)+x1*dx)/10;
				//BIAS_SET
				x0=X0.BIAS_SET;
				x1=X1.BIAS_SET;
				ActualSetPoint.BIAS_SET=  (uint16_t)(x0*(10-dx)+x1*dx)/10;				
			}
			lastPercent=percent;
}

void CONTROL_CalcCRSetPoint(uint16_t percent,uint16_t res)
{
			uint8_t idx,dx;			
			int32_t	x0,x1;
			RAWSetPoint X0,X1;
		  const RAWSetPoint *CRX_LOOKTBL;
			
			if( res > 250 ){					
				CRX_LOOKTBL=CR300_LOOKTBL;
			}
			else
			{
				CRX_LOOKTBL=CR150_LOOKTBL;
			}
			
	    if(percent==0)
			{				
				ActualSetPoint=CalibrationSetPoint;
			}
			else
			{
				idx=percent/10;
				dx=percent%10;
				X0=CRX_LOOKTBL[idx];
				X1=CRX_LOOKTBL[idx+1];
				//DDS_AMPL
				x0=X0.DDS_AMPL;
				x1=X1.DDS_AMPL;
				ActualSetPoint.DDS_AMPL=  (uint16_t)(x0*(10-dx)+x1*dx)/10;
				//PWR_VBAR
				x0=X0.PWR_VBAR;
				x1=X1.PWR_VBAR;			
				ActualSetPoint.PWR_VBAR=  (uint16_t)(x0*(10-dx)+x1*dx)/10;
				//BIAS_SET
				x0=X0.BIAS_SET;
				x1=X1.BIAS_SET;
				ActualSetPoint.BIAS_SET=  (uint16_t)(x0*(10-dx)+x1*dx)/10;				
			}
			lastPercent=percent;
}


//-------------------------------------- TABLE_1 Control
void CONTROL_Clk_TABLE_1(void)
{
	static char OCState=2;
	
	switch(OCState)
	{
				case 1: //Open Circuit
					if(Zext < 1100.0 && Iout>0.02)
					{
						OCState=0;
						//CONTROL_CalcOCSetPoint(Percent);	
						CONTROL_CalcCRSetPoint(Percent,(uint16_t )Zext);				
					  CONTROL_Output();
					}
					else
					{
						if(Percent!= lastPercent)
						{
									CONTROL_CalcOCSetPoint(Percent);				
									CONTROL_Output();
						}						
					}
					break;
	
					case 0:	// Loaded
					if(Zext > 1200.0 || Power<1.0)
					{
						OCState=1;
						CONTROL_CalcOCSetPoint(Percent);				
					  CONTROL_Output();
					}
					else
					{
							if(Percent!= lastPercent)
							{
									CONTROL_CalcCRSetPoint(Percent,(uint16_t )Zext);				
									CONTROL_Output();
							}						
					}				
					break;
	}
			
}


//CONTROL_CalcOCSetPoint(0);
