/*
 * PWM.c
 *
 * Created: 2/17/2020 4:09:06 PM
 *  Author: mahmo
 */ 

/*- INCLUDES ----------------------------------------------*/
#include "PWM.h"

/*- LOCAL MACROS ------------------------------------------*/
/* TIMSK_BITS */
#define OCIE2_BIT					    7
#define TOIE2_BIT					    6
#define TICIE1_BIT					    5
#define OCIE1A_BIT					    4
#define OCIE1B_BIT					    3
#define TOIE1_BIT					    2
#define OCIE0_BIT					    1
#define TOIE0_BIT					    0
									   
/* TIFR_BITS */						   
#define OCF2_BIT					    7
#define TOV2_BIT					    6
#define ICF1_BIT					    5
#define OCF1A_BIT					    4
#define OCF1B_BIT					    3
#define TOV1_BIT					    2
#define OCF0_BIT					    1
#define TOV0_BIT					    0

/* ASSR */
/* bits 7-4 reserved */
#define AS2_BIT							3
#define TCN2UB_BIT						2
#define OCR2UB_BIT						1
#define TCR2UB_BIT						0
#define FCPU							16000000UL  
#define HUNDERED_PRESENT				100
#define PRESCALER_256					256UL
#define TIMER0_RESLUTION                255
#define TIMER2_RESLUTION                255
#define TIMER1_RESLUTION				65535

#define OCR1A_BIT						0X20
#define OCR1B_BIT						0X10

#define MAX_FREQ						625UL
#define MIN_FREQ						245

#define TIMER0_PRESCALER_CLEAR_MASK		0x07
#define TIMER1_PRESCALER_CLEAR_MASK     0x0007
#define TIMER2_PRESCALER_CLEAR_MASK     0x07

/*- LOCAL DataTypes ---------------------------------------*/
// for timer 1
typedef enum En_timer1HardwarePWM_t{
	T1_OC1_DIS=0,T1_OC1A_TOGGLE=0x4000,T1_OC1B_TOGGLE=0x1000,T1_OC1A_CLEAR=0x8000,T1_OC1B_CLEAR=0x2000,T1_OC1A_SET=0xC000,T1_OC1B_SET=0x3000
}En_timer1HardwarePWM_t;

typedef enum En_timer1perscaler_t{
	T1_NO_CLOCK=0x0000,T1_PRESCALER_NO=0x0001,T1_PRESCALER_8=0x0002,T1_PRESCALER_64=0x0003,T1_PRESCALER_256=0x0004,T1_PRESCALER_1024=0x0005
}En_timer1perscaler_t;


typedef enum En_PWM_Timer1_Mode_t{
	T1_pwm_phaseC=0x0100,T1_pwmPC_9bits=0x0200,T1_pwm_PC_10bits=0X0300,T1_fastPWM_8bits=0X0108,T1_fastPWM_9bits=0X0208,T1_PWM_Freq_PC_ICR1_TOP=0X0010,T1_PWM_Freq_PC_OCR1A_TOP=0X0110
	,T1_PWM_PC_ICR1_TOP= 0X0210, T1_PWM_PC_OCR1A_TOP = 0X0310,T1_Fast_PWM_ICR1_TOP =0X0218 ,T1_Fast_PWM_OCR1A_TOP = 0X0318
}En_PWM_Timer1_Mode_t;


/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
static ERROR_STATUS validate_prescaler(uint8_t channel , uint8_t prescaler);
static  ERROR_STATUS validate_cnfg(Pwm_Cfg_s *Pwm_Cfg);
/*- GLOBAL STATIC VARIABLES -------------------------------*/

static uint8_t gu8_PWM_prescaler;
/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/
/*- APIs IMPLEMENTATION -----------------------------------*/
static ERROR_STATUS validate_prescaler(uint8_t channel , uint8_t prescaler)
{
	uint8_t u8_fun_status = OK;
		switch(channel)
		{
			case PWM_CH0:
			case PWM_CH1A:
			case PWM_CH1B:
			case PWM_CH1A_B:
			switch(prescaler)
			{
				case PWM_PRESCALER_NO:
				case PWM_PRESCALER_8:
				case PWM_PRESCALER_64:
				case PWM_PRESCALER_256:
				case PWM_PRESCALER_1024:
				/*valid prescaler*/
				break;
				default:
				u8_fun_status = NOK;
				break;
			}
			break;
			case PWM_CH2:
			switch(prescaler)
			{
				case PWM_PRESCALER_NO:
				case PWM_PRESCALER_8:
				case PWM_PRESCALER_32:
				case PWM_PRESCALER_64:
				case PWM_PRESCALER_128:
				case PWM_PRESCALER_256:
				case PWM_PRESCALER_1024:
				/*valid input*/
				break;
				default:
				u8_fun_status = NOK;
				break;
			}
			break;
		}
	return u8_fun_status;
}
static ERROR_STATUS validate_cnfg(Pwm_Cfg_s *Pwm_Cfg)
{
	uint8_t u8_fun_status = OK;

	if(Pwm_Cfg->Channel > PWM_CH2 || Pwm_Cfg == NULL)
	{
		u8_fun_status = NOK;
	}
	else
	{
	 validate_prescaler(Pwm_Cfg->Channel,Pwm_Cfg->Prescaler);
	}
	return u8_fun_status;
}


ERROR_STATUS Pwm_Init(Pwm_Cfg_s *Pwm_Cfg)
{
	/*validate input*/
	uint8_t u8_fun_status = validate_cnfg(Pwm_Cfg);
	if(u8_fun_status == OK)
	{
		/************************************************************************
		* 1-clear all timer1 register
		* 2-set PWM MODE IN TCCRX
		* 3-set oc mode 
		* 4-update pwm_prescaler value with the prescaler value
		* 5-set OCR1A, OCR1B as output
		************************************************************************/

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
		
		/*set OCR1A OCR1B*/
		switch(Pwm_Cfg->Channel)
		{
			case PWM_CH0:
			break;
			case PWM_CH1A:
			break;
			case PWM_CH1B:
			break;
			case PWM_CH1A_B:
				/*set pwm mod as phase correct with OCR1A as top value*/
				TCCR1 |=T1_PWM_PC_ICR1_TOP;
				TCCR1 |= (T1_OC1A_CLEAR|T1_OC1B_CLEAR);
				gu8_PWM_prescaler = T1_PRESCALER_256 ;
				/*set OCR1A AND OCR1B AS OUTPUT*/
				PORTD_DIR |= (OCR1B_BIT|OCR1A_BIT);
			break;
			case PWM_CH2:
			break;
		}
	}
	return u8_fun_status;
}

ERROR_STATUS Pwm_Start(uint8_t Channel,uint8_t Duty,uint32_t Frequncy)
{	
	/************************************************************************
	*	1-validate input
	*	2-evaluate ticks TCNT value and OCR1A value
	*	3-set prescaler to enable pwm                                                                      
	7************************************************************************/
	uint8_t u8_fun_status = OK;
	uint32_t u32_ticks;
	uint32_t u32_duty_value;
	if(Frequncy < MIN_FREQ || Frequncy > MAX_FREQ || Channel > PWM_CH2 )
	{
		u8_fun_status = NOK;
	}
	/*multiply freq by 2 as Phase correct operates on half of the given freq*/
	Frequncy +=Frequncy;
	u32_ticks		=  ((FCPU/PRESCALER_256)/Frequncy);
	u32_duty_value	=  (((u32_ticks*Duty)/HUNDERED_PRESENT));

	switch(Channel)
	{
		case PWM_CH0:
		break;
		case PWM_CH1A:
		break;
		case PWM_CH1B:
		break;
		case PWM_CH1A_B:
		/*set TCNT1 to zero and OCR1A */
		TCNT1 = ZERO;
		ICR1  = u32_ticks;
		OCR1A = u32_duty_value;
		OCR1B = u32_duty_value;
		break;
		case PWM_CH2:
		break;
	} 
	TCCR1 |= gu8_PWM_prescaler;
	return u8_fun_status;
}


ERROR_STATUS Pwm_Update(uint8_t Channel,uint8_t Duty,uint32_t Frequncy)
{
/************************************************************************
	*	1-validate input
	*	2-evaluate ticks TCNT value and OCR1A value
	*	3-set prescaler to enable pwm                                                                      
	7************************************************************************/
	uint8_t fun_status = OK;
	uint32_t u32_ticks;
	uint32_t u32_duty_value;
	if(Frequncy < MIN_FREQ || Frequncy > MAX_FREQ || Channel > PWM_CH2 )
	{
		fun_status = NOK;
	}
	/*multiply freq by 2 as Phase correct operates on half of the given freq*/
	Frequncy +=Frequncy;
	u32_ticks		=  ((FCPU/PRESCALER_256)/Frequncy);
	u32_duty_value	=  (((u32_ticks*Duty)/HUNDERED_PRESENT));

	switch(Channel)
	{
		case PWM_CH0:
		break;
		case PWM_CH1A:
		break;
		case PWM_CH1B:
		break;
		case PWM_CH1A_B:
		/*set TCNT1 to zero and OCR1A */
		TCNT1 = ZERO;
		ICR1  = u32_ticks;
		OCR1A = u32_duty_value;
		OCR1B = u32_duty_value;
		break;
		case PWM_CH2:
		break;
	} 
	return fun_status;
}

ERROR_STATUS Pwm_Stop(uint8_t Channel)
{
	uint8_t u8_fun_status = OK;

	if(Channel > PWM_CH2)
	{
		u8_fun_status = NOK;
	}
	else
	{
		switch(Channel)
		{
			case PWM_CH0:
				CLEAR_MASK(TCCR0,TIMER0_PRESCALER_CLEAR_MASK);
			break;
			case PWM_CH1A:
				CLEAR_MASK(TCCR1,TIMER1_PRESCALER_CLEAR_MASK);
			break;
			case PWM_CH1B:
				CLEAR_MASK(TCCR1,TIMER1_PRESCALER_CLEAR_MASK);
			break;
			case PWM_CH1A_B:
				CLEAR_MASK(TCCR1,TIMER1_PRESCALER_CLEAR_MASK);
			break;
			case PWM_CH2:
				CLEAR_MASK(TCCR2,TIMER2_PRESCALER_CLEAR_MASK);
			break;
			default:
				u8_fun_status = NOK;
			break;
		}
	}
	return u8_fun_status;
}