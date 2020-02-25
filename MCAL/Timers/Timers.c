/*
 * Timers.c
 *
 * Created: 2/17/2020 10:08:18 AM
 *  Author: mahmo
 */ 


/*- INCLUDES ----------------------------------------------*/
#include "Timer.h"
#include "../../interrupt.h"

/*- LOCAL MACROS ------------------------------------------*/ 

/* TIMSK_BITS */
#define OCIE2_BIT   7
#define TOIE2_BIT   6
#define TICIE1_BIT  5
#define OCIE1A_BIT  4
#define OCIE1B_BIT  3
#define TOIE1_BIT   2
#define OCIE0_BIT   1
#define TOIE0_BIT   0

/* TIFR_BITS */
#define OCF2_BIT    7
#define TOV2_BIT    6
#define ICF1_BIT    5
#define OCF1A_BIT   4
#define OCF1B_BIT   3
#define TOV1_BIT    2
#define OCF0_BIT    1
#define TOV0_BIT    0

/* ASSR */
/* bits 7-4 reserved */
#define AS2_BIT     3
#define TCN2UB_BIT  2
#define OCR2UB_BIT  1
#define TCR2UB_BIT  0

#define TIMER0_PRESCALER_CLEAR_MASK		0x07
#define TIMER1_PRESCALER_CLEAR_MASK     0x0007
#define TIMER2_PRESCALER_CLEAR_MASK     0x07

#define TIMER0_RESLUTION                255
#define TIMER2_RESLUTION                255
#define TIMER1_RESLUTION				65535

#define  TIMER0_MS_OVERFLOW				62

typedef enum En_timer0Mode_t{
	T0_NORMAL_MODE=0,T0_COMP_MODE=0x08
}En_timer0Mode_t;

typedef enum En_timer0OC_t{
	T0_OC0_DIS=0,T0_OC0_TOGGLE=0x10,T0_OC0_CLEAR=0x20,T0_OC0_SET=0x30
}En_timer0OC_t;

typedef enum En_timer0perscaler_t{
	T0_NO_CLOCK=0,T0_PRESCALER_NO=0x01,T0_PRESCALER_8=0x02,T0_PRESCALER_64=0x03,T0_PRESCALER_256=0x04,T0_PRESCALER_1024=0x05
}En_timer0perscaler_t;

typedef enum En_timer0Interrupt_t{
	T0_POLLING=0,T0_INTERRUPT_NORMAL=0x01,T0_INTERRUPT_CMP=0x02
}En_timer0Interrupt_t;


typedef enum En_timer0SwOWNFreq_t{
	FREQ1=100,FREQ2=1000,FREQ3=4000,FREQ4=16000
}En_timer0SwOWNFreq_t;

typedef enum En_time0Mod_t{
	T0_timerMod=0x00,T0_CounterRisingMod = 0X07,T0_CounterFallingMod=0x06
}En_time0Mod_t;

// for timer 1
typedef enum En_timer1Mode_t{
	T1_NORMAL_MODE=0x0000,T1_COMP_MODE_OCR1A_TOP=0x0008, T1_COMP_MODE_ICR1_TOP = 0x0018

}En_timer1Mode_t;
typedef enum En_timer1OC_t{
	T1_OC1_DIS=0,T1_OC1A_TOGGLE=0x4000,T1_OC1B_TOGGLE=0x1000,T1_OC1A_CLEAR=0x8000,T1_OC1B_CLEAR=0x2000,T1_OC1A_SET=0xC000,T1_OC1B_SET=0x3000
}En_timer1OC_t;

typedef enum En_timer1perscaler_t{
	T1_NO_CLOCK=0x0000,T1_PRESCALER_NO=0x0001,T1_PRESCALER_8=0x0002,T1_PRESCALER_64=0x0003,T1_PRESCALER_256=0x0004,T1_PRESCALER_1024=0x0005
}En_timer1perscaler_t;

typedef enum En_timer1Interrupt_t{
	T1_POLLING=0,T1_INTERRUPT_NORMAL=0x04,T1_INTERRUPT_CMP_1B=0x08, T1_INTERRUPT_CMP_1A=0x10, T1_INTERRUPT_ICAPTURE = 0x20
}En_timer1Interrupt_t;

typedef enum En_time1Mod_t{
	T1_timerMod=0x0000,T1_CounterRisingMod = 0X0007,T1_CounterFallingMod=0x0006
}En_time1Mod_t;


// for timer 2
typedef enum En_timer2Mode_t{
	T2_NORMAL_MODE=0,T2_COMP_MODE=0x08
}En_timer2Mode_t;

typedef enum En_timer2OC_t{
	T2_OC2_DIS=0,T2_OC2_TOGGLE=0x10,T2_OC2_CLEAR=0x20,T2_OC2_SET=0x30
}En_timer2OC_t;

typedef enum En_timer2perscaler_t{
	T2_NO_CLOCK=0,T2_PRESCALER_NO=0x01,T2_PRESCALER_8=0x02, T2_PRESCALER_32=0x03, T2_PRESCALER_64=0x04, T2_PRESCALER_128=0x05, T2_PRESCALER_256 = 0x06, T2_PRESCALER_1024=0x07
}En_timer2perscaler_t;

typedef enum En_timer2Interrupt_t{
	T2_POLLING=0,T2_INTERRUPT_NORMAL=0x40,T2_INTERRUPT_CMP=0x80
}En_timer2Interrupt_t;


typedef enum En_timer2Asynchronous_t{
	T2_INERNAL_CLK=0,T2_EXTERNAL_CLK = 0X10
}En_timer2Asynchronous_t;

typedef enum En_time2Mod_t{
	T2_timerMod=0x00,T2_CounterRisingMod = 0X07,T2_CounterFallingMod=0x06
}En_time2Mod_t;



/*- LOCAL Data types --------------------------------------*/ 

/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/ 
/*- GLOBAL STATIC VARIABLES -------------------------------*/ 
static uint8_t	 gu8_timer0prescaler;
static uint8_t	 gu8_timer1prescaler;
static uint8_t	 gu8_timer2prescaler;

/*- GLOBAL EXTERN VARIABLES -------------------------------*/ 
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/ 
/*- APIs IMPLEMENTATION -----------------------------------*/

volatile static uint16_t gu16SwICU_timer0_Overflow_Counts = ZERO;
volatile  uint16_t timer0_MS_flag = FALSE;

ISR(TIMER0_OVF_vect)
{
	/*++gu16SwICU_timer0_Overflow_Counts;
	PORTA_DIR = 0xff;
	PORTA_DATA ^= 0xff
	if(gu16SwICU_timer0_Overflow_Counts == TIMER0_MS_OVERFLOW )
	{
		timer0_MS_flag = TRUE;
		gu16SwICU_timer0_Overflow_Counts = ZERO;
		TCNT0 = HALF_TICKS;
	}*/
	/*PORTA_DIR = 0xff;
	PORTA_DATA ^= 0xff;*/
	timer0_MS_flag  = TRUE;
	TCNT0 = TIMER0_RESLUTION-TIMER0_1MS_PRESCALER256;
}

ERROR_STATUS timer_DeInit(uint8_t timer_Ch)
{
	uint8_t u8_fun_status = OK;
	if(timer_Ch > TIMER_CH2)
	{
		u8_fun_status = NOK;
	}
	else
	{
		switch(timer_Ch)
		{
			case TIMER_CH0:
				/*timer 0 set as oc interrupte mode */
				/*zero all bits & registers*/
				TCCR0   =  ZERO;
				TCNT0   =  ZERO;
				CLEAR_BIT(TIMSK,TOIE0_BIT);
				CLEAR_BIT(TIMSK,OCIE0_BIT);
				SET_BIT(TIFR,TOV0_BIT);/*inturrept flag bit cleared by writing 1 to it*/
			break;
			case TIMER_CH1:
				/*zero all registers
				* TCCR1 = TCTN1 = OCR1A = OCR1B = ICR1 = ZERO
				* TIMSK CLEAR BITS [TICIE1 OCIE1A OCIE1B TOIE1]
				* TIFR  CLEAR BITS [ICF1 OCF1A OCF1B TOV1]
				**/
				TCCR1   =  ZERO;
				TCNT1   =  ZERO;
				OCR1A   =  ZERO;
				OCR1B   =  ZERO;
				ICR1	=  ZERO;
	
				CLEAR_BIT(TIMSK,TICIE1_BIT);
				CLEAR_BIT(TIMSK,OCIE1A_BIT);
				CLEAR_BIT(TIMSK,OCIE1B_BIT);
				CLEAR_BIT(TIMSK,TOIE1_BIT);

				SET_BIT(TIFR,ICF1_BIT);
				SET_BIT(TIFR,OCF1A_BIT);
				SET_BIT(TIFR,OCF1B_BIT);
				SET_BIT(TIFR,TOV1_BIT);

			break;
			case TIMER_CH2:
				/*zero all bits & registers*/
				TCCR2   =  ZERO;
				TCNT2   =  ZERO;
				CLEAR_BIT(TIMSK,TOIE2_BIT);
				CLEAR_BIT(TIMSK,OCIE2_BIT);
				SET_BIT(TIFR,TOV2_BIT);
				CLEAR_BIT(ASSR,AS2_BIT);
			break;
		}	
	}
	
	return u8_fun_status;
}
ERROR_STATUS Timer_Init(Timer_cfg_s* Timer_cfg)
{
	uint8_t u8_fun_status = OK;
	switch(Timer_cfg->Timer_CH_NO){
		case TIMER_CH0:
			/*timer 0 set as oc interrupte mode */
			/*zero all bits & registers*/
			TCCR0   =  ZERO;
			TCNT0   =  ZERO;
			CLEAR_BIT(TIMSK,TOIE0_BIT);
			CLEAR_BIT(TIMSK,OCIE0_BIT);
			SET_BIT(TIFR,TOV0_BIT);/*inturrept flag bit cleared by writing 1 to it*/
			
			/* timer mode specify timer clk source either internal or 
			* external clk specified in the prescaler
			*/
			SET_MASK(TIMSK,T0_INTERRUPT_NORMAL); /*set timer0 as as int normal mode*/
			switch(Timer_cfg->Timer_Mode)
			{
				case TIMER_MODE:
					SET_MASK(TCCR0,T0_timerMod);
				break;
				case COUNTER_RISING_MODE:
					SET_MASK(TCCR0,T0_CounterRisingMod);
				break;
				case COUNTER_FALLING_MODE:
					SET_MASK(TCCR0,T0_CounterFallingMod);
				break;
				default:
				u8_fun_status = NOK;
				break;
			}
			/*check for valid prescaler*/
			switch(Timer_cfg->Timer_Prescaler)
			{
				case TIMER_NO_CLOCK:
				case TIMER_PRESCALER_NO:
				case TIMER_PRESCALER_8:
				case TIMER_PRESCALER_64:
				case TIMER_PRESCALER_256:
				case TIMER_PRESCALER_1024:
					/*valid prescaler*/
					gu8_timer0prescaler	=	Timer_cfg->Timer_Prescaler;
				break;
				default:
					u8_fun_status = NOK;
				break;
			}
			switch(Timer_cfg->Timer_Polling_Or_Interrupt)
			{
				case TIMER_POLLING_MODE:
					SET_MASK(TIMSK,T0_POLLING);
				break;
				case TIMER_INTERRUPT_MODE:
					SET_MASK(TIMSK,T0_NORMAL_MODE);
				break;
				default:
				u8_fun_status = NOK;
				break;
			}
		break;
		case TIMER_CH1:
			/**Registers needed
			*			[TCCR2] Timer2 control register [FOC2 WGM20 COM21 COM20 WGM21 CS22 CS21 CS20]
			*			[TCNT2]	Timer2/conutner register [keep value of ticks]
			*			[TIMR]	Timer Interrupt Mask register [TIE2 OCIE2]
			*			[TIFR]	Timer Interrupt Flag Register [TOV2 OCRF2]
			*			[ASSR]	Asynchronous Status Register [ AS2 TCN2UB OCR2UB TCR2UB] _just set the AS2 bit to 0 to work on internal clk_
			*			[SFIOR] Special Function IO Register [PSR2]
			*/

			/*zero all registers
			* TCCR1 = TCTN1 = OCR1A = OCR1B = ICR1 = ZERO
			* TIMSK CLEAR BITS [TICIE1 OCIE1A OCIE1B TOIE1]
			* TIFR  CLEAR BITS [ICF1 OCF1A OCF1B TOV1]
			**/
			TCCR1   =  ZERO;
			TCNT1   =  ZERO;
			OCR1A   =  ZERO;
			OCR1B   =  ZERO;
			ICR1	=  ZERO;
	
			CLEAR_BIT(TIMSK,TICIE1_BIT);
			CLEAR_BIT(TIMSK,OCIE1A_BIT);
			CLEAR_BIT(TIMSK,OCIE1B_BIT);
			CLEAR_BIT(TIMSK,TOIE1_BIT);

			SET_BIT(TIFR,ICF1_BIT);
			SET_BIT(TIFR,OCF1A_BIT);
			SET_BIT(TIFR,OCF1B_BIT);
			SET_BIT(TIFR,TOV1_BIT);
			
			
			/* timer mode specify timer clk source either internal or 
			* external clk specified in the prescaler
			*/
			switch(Timer_cfg->Timer_Mode)
			{
				case TIMER_MODE:
					SET_MASK(TCCR1,T1_timerMod);
				break;
				case COUNTER_RISING_MODE:
					SET_MASK(TCCR1,T1_CounterRisingMod);
				break;
				case COUNTER_FALLING_MODE:
					SET_MASK(TCCR1,T1_CounterFallingMod);
				break;
				default:
					u8_fun_status = NOK;
				break;
			}
				switch(Timer_cfg->Timer_Prescaler)
				{
					case TIMER_NO_CLOCK:
					case TIMER_PRESCALER_NO:
					case TIMER_PRESCALER_8:
					case TIMER_PRESCALER_64:
					case TIMER_PRESCALER_256:
					case TIMER_PRESCALER_1024:
						gu8_timer1prescaler	=	Timer_cfg->Timer_Prescaler;
					break;
					default:
						u8_fun_status = NOK;
					break;
				}
				switch(Timer_cfg->Timer_Polling_Or_Interrupt)
				{
					case TIMER_POLLING_MODE:
						SET_MASK(TIMSK,T1_POLLING);
					break;
					case TIMER_INTERRUPT_MODE:
						SET_MASK(TIMSK,T1_INTERRUPT_NORMAL);
					break;
					default:
					break;
				}
		break;
		case TIMER_CH2:
			/**Registers needed
			*			[TCCR2] Timer2 control register [FOC2 WGM20 COM21 COM20 WGM21 CS22 CS21 CS20]
			*			[TCNT2]	Timer2/conutner register [keep value of ticks]
			*			[TIMR]	Timer Interrupt Mask register [TIE2 OCIE2]
			*			[TIFR]	Timer Interrupt Flag Register [TOV2 OCRF2]
			*			[ASSR]	Asynchronous Status Register [ AS2 TCN2UB OCR2UB TCR2UB] _just set the AS2 bit to 0 to work on internal clk_
			*			[SFIOR] Special Function IO Register [PSR2]
			*/
			
			/*zero all bits & registers*/
			TCCR2   =  ZERO;
			TCNT2   =  ZERO;
			CLEAR_BIT(TIMSK,TOIE2_BIT);
			CLEAR_BIT(TIMSK,OCIE2_BIT);
			SET_BIT(TIFR,TOV2_BIT);
			CLEAR_BIT(ASSR,AS2_BIT);
			
			/* timer mode specify timer clk source either internal or 
			* external clk specified in the prescaler
			*/
			switch(Timer_cfg->Timer_Mode)
			{
				case TIMER2_INTERNAL_CLK:
					CLEAR_BIT(ASSR,AS2_BIT);
				break;
				case TIMER2_EXTERNAL_CLK:
					SET_BIT(ASSR,AS2_BIT);
				break;
			    default:
					u8_fun_status = NOK;
				break;
			}
				switch(Timer_cfg->Timer_Prescaler)
				{
					case TIMER_NO_CLOCK:
					case TIMER_PRESCALER_NO:
					case TIMER_PRESCALER_8:
					case TIMER_PRESCALER_32:
					case TIMER_PRESCALER_64:
					case TIMER_PRESCALER_128:
					case TIMER_PRESCALER_256:
					case TIMER_PRESCALER_1024:
						gu8_timer2prescaler	=	Timer_cfg->Timer_Prescaler;
					break;
					default:
						u8_fun_status = NOK;
					break;
				}
				switch(Timer_cfg->Timer_Polling_Or_Interrupt)
				{
					case TIMER_POLLING_MODE:
						SET_MASK(TIMSK,T2_POLLING);
					break;
					case TIMER_INTERRUPT_MODE:
						SET_MASK(TIMSK,T2_INTERRUPT_NORMAL);
					break;
					default:
					break;
				}
		break;
		default:
		break;
	}


	return u8_fun_status;
}

ERROR_STATUS Timer_Start(uint8_t Timer_CH_NO, uint16_t Timer_Count)
{
	uint8_t u8_fun_status = OK;
	switch(Timer_CH_NO)
	{
		case TIMER_CH0:
			if(Timer_Count > TIMER0_RESLUTION)
			{
				u8_fun_status = NOK;
			}
			else
			{
				/*set ocr0 VALUE, SET PRESCALER TO START*/
				TCNT0 = TIMER0_RESLUTION-Timer_Count;
				//CLEAR_MASK(TCCR0,TIMER0_PRESCALER_CLEAR_MASK);
				switch(gu8_timer0prescaler)
				{
					case TIMER_NO_CLOCK:
						SET_MASK(TCCR0,T0_NO_CLOCK);
					break;
					case TIMER_PRESCALER_NO:
						SET_MASK(TCCR0,T0_PRESCALER_NO);						
					break;				
					case TIMER_PRESCALER_8:
						SET_MASK(TCCR0,T0_PRESCALER_8);
					break;
					case TIMER_PRESCALER_64:
						SET_MASK(TCCR0,T0_PRESCALER_64);
					break;
					case TIMER_PRESCALER_256:
						SET_MASK(TCCR0,T0_PRESCALER_256);
					break;
					case TIMER_PRESCALER_1024:
						SET_MASK(TCCR0,T0_PRESCALER_1024);
					break;
					default:
						u8_fun_status = NOK;
					break;
				}
			}
		break;
		case TIMER_CH1:
			if(Timer_Count > TIMER1_RESLUTION)
			{
				u8_fun_status = NOK;
			}
			else
			{
				/*set TCNT VALUE, SET PRESCALER TO START*/
				TCNT1 = TIMER1_RESLUTION-Timer_Count;
				CLEAR_MASK(TCCR1,TIMER1_PRESCALER_CLEAR_MASK);
				switch(gu8_timer1prescaler)
				{
					case TIMER_NO_CLOCK:
						SET_MASK(TCCR1,T1_NO_CLOCK);
					break;
					case TIMER_PRESCALER_NO:
						SET_MASK(TCCR1,T1_PRESCALER_NO);
					break;
					case TIMER_PRESCALER_8:
						SET_MASK(TCCR1,T1_PRESCALER_8);
					break;
					case TIMER_PRESCALER_64:
						SET_MASK(TCCR1,T1_PRESCALER_64);
					break;
					case TIMER_PRESCALER_256:
						SET_MASK(TCCR1,T1_PRESCALER_256);
					break;
					case TIMER_PRESCALER_1024:
						SET_MASK(TCCR1,T1_PRESCALER_1024);
					break;
					default:
						u8_fun_status = NOK;
					break;
				}		
		}
		break;
		case TIMER_CH2:
			if(Timer_Count > TIMER2_RESLUTION)
			{
				u8_fun_status = NOK;
			}
			else
			{
				/*set TCNT VALUE, SET PRESCALER TO START*/
				TCNT2 = TIMER2_RESLUTION-Timer_Count;
				//CLEAR_MASK(TCCR2,TIMER2_PRESCALER_CLEAR_MASK);
				switch(gu8_timer2prescaler)
				{
					case TIMER_NO_CLOCK:
						SET_MASK(TCCR2,T2_NO_CLOCK);
					break;
					case TIMER_PRESCALER_NO:
						SET_MASK(TCCR2,T2_PRESCALER_NO);
					break;
					case TIMER_PRESCALER_8:
						SET_MASK(TCCR2,T2_PRESCALER_8);
					break;
					case TIMER_PRESCALER_32:
						SET_MASK(TCCR2,T2_PRESCALER_32);
					break;
					case TIMER_PRESCALER_64:
						SET_MASK(TCCR2,T2_PRESCALER_64);
					break;
					case T2_PRESCALER_128:
						SET_MASK(TCCR2,T2_PRESCALER_128);
					break;
					case TIMER_PRESCALER_256:
						SET_MASK(TCCR2,T2_PRESCALER_256);
					break;
					case TIMER_PRESCALER_1024:
						SET_MASK(TCCR2,T2_PRESCALER_1024);
					break;
					default:
						u8_fun_status = NOK;
					break;
				}
			}
		break;
	}
	return u8_fun_status;
}


ERROR_STATUS Timer_Stop(uint8_t Timer_CH_NO)
{
	uint8_t u8_fun_status = OK;
	switch(Timer_CH_NO)
	{
		case TIMER_CH0:
			CLEAR_MASK(TCCR0,TIMER0_PRESCALER_CLEAR_MASK);			
		break;
		case TIMER_CH1:
			CLEAR_MASK(TCCR1,TIMER1_PRESCALER_CLEAR_MASK);
		break;
		case TIMER_CH2:
			CLEAR_MASK(TCCR2,TIMER2_PRESCALER_CLEAR_MASK);
		break;
		default:
			u8_fun_status = NOK;
		break;
	}
	return u8_fun_status;
}

ERROR_STATUS Timer_GetStatus(uint8_t Timer_CH_NO, uint8_t* Data)
{
	uint8_t u8_fun_status = OK;
	switch(Timer_CH_NO)
	{
		case TIMER_CH0:
			*Data = BIT_IS_SET(TIMSK,TOV0_BIT) ? TRUE : FALSE;
		break;
		case TIMER_CH1:
			*Data = BIT_IS_SET(TIMSK,TOV1_BIT) ? TRUE : FALSE;
		break;
		case TIMER_CH2:
			*Data = BIT_IS_SET(TIMSK,TOV2_BIT) ? TRUE : FALSE;
		break;
		default:
			u8_fun_status = NOK;
		break;
	}
	return u8_fun_status;
}

ERROR_STATUS Timer_GetValue(uint8_t Timer_CH_NO, uint16_t* Data)
{
	uint8_t u8_fun_status = OK;
	switch(Timer_CH_NO)
	{
		case TIMER_CH0:
			*Data = TCNT0;
		break;
		case TIMER_CH1:
			*Data = TCNT1;
		break;
		case TIMER_CH2:
			*Data = TCNT2;
		break;
		default:
			u8_fun_status = NOK;
		break;
	}
	return u8_fun_status;
}