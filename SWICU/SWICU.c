/*
 * SWICU.c
 *
 * Created: 2/18/2020 1:19:44 AM
 *  Author: mahmo
 */ 

/*- INCLUDES ----------------------------------------------*/
#include "SwICU.h"


/*- LOCAL MACROS ------------------------------------------*/


/* GICR */
#define INT1    7
#define INT0    6
#define INT2    5
#define IVSEL   1
#define IVCE    0

/* GIFR */
#define INTF1   7
#define INTF0   6
#define INTF2   5

/* MCUCR */
#define SE      7
#define SM2     6
#define SM1     5
#define SM0     4
#define ISC11   3
#define ISC10   2
#define ISC01   1
#define ISC00   0

/* MCUCSR */
#define JTD     7
#define ISC2    6
/* bit 5 reserved */
#define JTRF    4
#define WDRF    3
#define BORF    2
#define EXTRF   1
#define PORF    0

#define SWICU_TIMER2_RESLUTION	256UL

#define TIMER0STOPFLAG  1
#define TIMER0STARTFLAG 2

/*- LOCAL dataTypes ---------------------------------------*/


typedef enum En_externalInt_t{
	EN_INT0,
	EN_INT1,
	EN_INT2
}En_externalInt_t;

volatile uint16_t gu16SwICU_timer2_Overflow_Counts = ZERO;
volatile uint8_t  gu8SwICU_INT2_vec_enteranceFlag = ZERO;
volatile uint8_t  gu8SwICU_Timer2_Stop_Flag = FALSE;


/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
static ERROR_STATUS external_Int_Change_Edge(uint8_t en_externalInt,uint8_t swICU_Edge);
static ERROR_STATUS external_Int_Enable(uint8_t en_externalInt,uint8_t swICU_Edge);
static ERROR_STATUS SwICU_SetCfgEdge(uint8_t inputCaptureEdgeedge);
static ERROR_STATUS SwICU_Stop(void);
static ERROR_STATUS SwICU_Start(void);
/*- GLOBAL STATIC VARIABLES -------------------------------*/
/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/


ISR(INT2_vect)
{
	//Led_Toggle(LED_0);
	/*	check for value of overflows
	*	EVEN VALUE start timer 0
	*	ODD value stop timer 0
	*/
	PORTA_DIR   = HIGH;
	
	Timer_Start(TIMER_CH2,SWICU_TIMER2_RESLUTION);
	if(gu8SwICU_INT2_vec_enteranceFlag&TRUE)
	{
		SwICU_Stop();
		gu8SwICU_INT2_vec_enteranceFlag = FALSE;
		gu8SwICU_Timer2_Stop_Flag = TRUE;
		SwICU_SetCfgEdge(ICU_FALE_TO_RISE);
	}
	else
	{
		SwICU_Start();
		gu8SwICU_INT2_vec_enteranceFlag = TRUE;
		SwICU_SetCfgEdge(ICU_RISE_TO_FALL);
	}
}

ISR(TIMER2_OVF_vect)
{
	++gu16SwICU_timer2_Overflow_Counts;
}



/*			CONFIGIRE EXTERNAL INT
*	REGISTERS : GIFR  [INTF1 INTF0 INTF2]
*				GICR  [INT1 INT0 INT2]
*				MCUCR [ISC11 ISC10 ISC01 ISC00]
*				MCUCSR [ISCR2]
*/
static ERROR_STATUS external_Int_Enable(uint8_t en_externalInt,uint8_t swICU_Edge)
{
	uint8_t u8_fun_status = OK;
	
	if(en_externalInt > EN_INT0 || swICU_Edge > ICU_RISE_TO_FALL)
	{
		u8_fun_status = NOK;
	}
	
	switch (en_externalInt)
	{
		case EN_INT0:
			switch(swICU_Edge)
			{
				case ICU_FALE_TO_RISE:
				break;
				case ICU_RISE_TO_FALL:
				break;
			}
		break;
		case EN_INT1:
			switch(swICU_Edge)
			{
				case ICU_FALE_TO_RISE:
				break;
				case ICU_RISE_TO_FALL:
				break;
			}
		break;
		case EN_INT2:
			switch(swICU_Edge)
			{
				case ICU_FALE_TO_RISE:
				/*			 CONFIGRE EXTERNAL INT2
				*  1-disable INT2 in GUCR.INT2
				*  2-Configure raising or falling edge MCUCSR.IISC2
				*  3-clear INT2 flag in GIFR.INTF2
				*  4-enable INT2 in GUCR.INT2
				*  5-make pin direction input POTTB.PIN2
				*  6-enbale global int sei()
				*/
				CLEAR_BIT(GICR,INT2);
				SET_BIT(MCUCSR,ISC2);
				SET_BIT(GIFR,INTF2);/*CLEAR INT FLAG BY SET IT'S BIT TO 1*/
				SET_BIT(GICR,INT2);
				DIO_Cfg_s DIO_Cfg;
				DIO_Cfg.dir  = INPUT;
				DIO_Cfg.GPIO = GPIOB;
				DIO_Cfg.pins = BIT2;
				DIO_init(&DIO_Cfg);
				sei();
				break;
				case ICU_RISE_TO_FALL:
				break;
			}
		break;	
	}
	
	return u8_fun_status;
}

static ERROR_STATUS external_Int_Change_Edge(uint8_t en_externalInt,uint8_t swICU_Edge)
{
	uint8_t u8_fun_status = OK;
	
	if(en_externalInt > EN_INT0 || swICU_Edge > ICU_RISE_TO_FALL)
	{
		u8_fun_status = NOK;
	}
	
	switch (en_externalInt)
	{
		case EN_INT0:
			switch(swICU_Edge)
			{
				case ICU_FALE_TO_RISE:
				break;
				case ICU_RISE_TO_FALL:
				break;
			}
		break;
		case EN_INT1:
			switch(swICU_Edge)
			{
				case ICU_FALE_TO_RISE:
				break;
				case ICU_RISE_TO_FALL:
				break;
			}
		break;
		case EN_INT2:
			switch(swICU_Edge)
			{
				case ICU_FALE_TO_RISE:
					
					SET_BIT(MCUCSR,ISC2);

				break;
				case ICU_RISE_TO_FALL:
					CLEAR_BIT(MCUCSR,ISC2);
				break;
			}
		break;
		
	}
	return u8_fun_status;
}



ERROR_STATUS SwICU_SetCfgEdge(uint8_t inputCaptureEdgeedge){
	/*change the config of the ext pin 
	* rasing failling
	*/
	uint8_t u8_fun_status = OK;
	u8_fun_status = external_Int_Change_Edge(EN_INT2,inputCaptureEdgeedge);
	return u8_fun_status;
}


ERROR_STATUS SwICU_Stop(void){
	/*stop timer */
	uint8_t u8_fun_status = OK;
	u8_fun_status = Timer_Stop(TIMER_CH2);
	return u8_fun_status;
}


ERROR_STATUS SwICU_Start(void)
{
	/*start timer*/
	uint8_t u8_fun_status = OK;
	u8_fun_status = Timer_Start(TIMER_CH2,SWICU_TIMER2_RESLUTION);
	return u8_fun_status;
}


/*- APIs IMPLEMENTATION -----------------------------------*/


ERROR_STATUS Icu_Init(Icu_cfg_s * Icu_Cfg)
{
	/*validate input*/
	uint8_t u8_fun_status = OK;

		if(Icu_Cfg == NULL || Icu_Cfg->ICU_Ch_No > ICU_CH2 || 
			Icu_Cfg->ICU_Ch_Timer > ICU_TIMER_CH2)
		{
				u8_fun_status = NOK;
		}
		else
		{	
			/* 
			*  1-enable external interrupter with rising edge
			*  2-initalize timer with interrupte mode
			*  3-zero all used variables
			*/
			Timer_cfg_s str_timer_cfg;

			external_Int_Enable(Icu_Cfg->ICU_Ch_No,ICU_FALE_TO_RISE);
			switch(Icu_Cfg->ICU_Ch_Timer)
			{
				case TIMER_CH0:
						str_timer_cfg.Timer_CH_NO				    =	TIMER_CH0;
						str_timer_cfg.Timer_Mode					=	TIMER_MODE;
						str_timer_cfg.Timer_Polling_Or_Interrupt	=	TIMER_INTERRUPT_MODE;
						str_timer_cfg.Timer_Prescaler				=   TIMER_PRESCALER_256;
						Timer_Init(&str_timer_cfg);
				break;
				case TIMER_CH1:
				break;
				case TIMER_CH2:
						str_timer_cfg.Timer_CH_NO				    =	TIMER_CH2;
						str_timer_cfg.Timer_Mode					=	TIMER_MODE;
						str_timer_cfg.Timer_Polling_Or_Interrupt	=	TIMER_INTERRUPT_MODE;
						str_timer_cfg.Timer_Prescaler				=   TIMER_PRESCALER_8;
						Timer_Init(&str_timer_cfg);
				break;
			}
			gu16SwICU_timer2_Overflow_Counts = ZERO;
			gu8SwICU_INT2_vec_enteranceFlag = ZERO;
			gu8SwICU_Timer2_Stop_Flag = FALSE;
		}
		return u8_fun_status;
}

ERROR_STATUS Icu_ReadTime(uint8_t Icu_Channel,
						 uint8_t Icu_EdgeToEdge, uint64_t * Icu_Time)
{
	uint8_t u8_fun_status = OK;
	
	if(Icu_Channel > ICU_TIMER_CH2 || Icu_EdgeToEdge > ICU_FALE_TO_RISE || 
		Icu_Time == NULL)
	{
		u8_fun_status = NOK;
	}
	else
	{
	/*
	* poll for timer0stop flag then read value
	* read ticks value
	* set Timer0_Stop_Flag t0 false
	* zero the number of overflow
	* reinitialize timer0 to reset it
	*/
	while(gu8SwICU_Timer2_Stop_Flag == FALSE);
	uint16_t u16_timerRead ;
	Timer_GetValue(TIMER_CH2,&u16_timerRead);
	*Icu_Time = (gu16SwICU_timer2_Overflow_Counts*(uint64_t)SWICU_TIMER2_RESLUTION)+u16_timerRead;
	
	gu16SwICU_timer2_Overflow_Counts = ZERO;
	gu8SwICU_INT2_vec_enteranceFlag = ZERO;
	gu8SwICU_Timer2_Stop_Flag = FALSE;
	}
	return u8_fun_status;
}
