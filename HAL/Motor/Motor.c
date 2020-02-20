/*
 * Motor.c
 *
 * Created: 2/17/2020 9:34:57 PM
 *  Author: mahmo
 */ 
#include "motor.h"

/*- INCLUDES ----------------------------------------------*/
/*- LOCAL MACROS ------------------------------------------*/
/*- LOCAL DataTypes ---------------------------------------*/
/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
/*- GLOBAL STATIC VARIABLES -------------------------------*/
static DIO_Cfg_s gstr_DIO_cfg;
static Pwm_Cfg_s gstr_pwm_cfg;
static uint8_t gu8_motor_1_init_status = FALSE;
static uint8_t gu8_motor_2_init_status = FALSE;
/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/
/*- APIs IMPLEMENTATION -----------------------------------*/



 ERROR_STATUS Motor_Init(uint8_t Motor_Number)
{
	uint8_t u8_fun_status = OK;
	

	if(Motor_Number > MOTOR_2)
	{
		u8_fun_status = NOK;
	}
	else
	{
		switch(Motor_Number)
		{
			case MOTOR_1:
				gstr_DIO_cfg.GPIO = MOTOR_EN_1_GPIO;
				gstr_DIO_cfg.dir  = OUTPUT;
				gstr_DIO_cfg.pins = (MOTOR_OUT_1A_BIT | MOTOR_OUT_1B_BIT | MOTOR_EN_1_BIT);
				gu8_motor_1_init_status = TRUE;
			break;
			case MOTOR_2:
				gstr_DIO_cfg.GPIO = MOTOR_EN_2_GPIO;
				gstr_DIO_cfg.dir  = OUTPUT;
				gstr_DIO_cfg.pins = (MOTOR_OUT_2B_BIT|MOTOR_OUT_2A_BIT|MOTOR_EN_2_BIT);
				gu8_motor_2_init_status = TRUE;
			break;
			default:
			u8_fun_status = NOK;
			break;
		}
			gstr_pwm_cfg.Channel = PWM_CH1A_B;
			gstr_pwm_cfg.Prescaler = PWM_PRESCALER_8;
			Pwm_Init(&gstr_pwm_cfg);
			DIO_init(&gstr_DIO_cfg);
			Pwm_Start(gstr_pwm_cfg.Channel,ZERO,MOTOR_FREQ);
	}
	return u8_fun_status;
}

ERROR_STATUS Motor_Direction(uint8_t Motor_Number, uint8_t Motor_Direction)
{
	uint8_t u8_fun_status = OK;
	if(Motor_Number > MOTOR_2 ||  Motor_Direction > MOTOR_BACKWARD)
	{
		u8_fun_status = NOK;
	}
	else
	{
		switch(Motor_Number)
		{
			case MOTOR_1:
				switch(Motor_Direction)
				{
					case MOTOR_FORWARD:
					DIO_Write(MOTOR_OUT_1A_GPIO,MOTOR_OUT_1A_BIT,HIGH);
					DIO_Write(MOTOR_OUT_1B_GPIO,MOTOR_OUT_1B_BIT,LOW);
					break;
					case MOTOR_BACKWARD:
					DIO_Write(MOTOR_OUT_1A_GPIO,MOTOR_OUT_1A_BIT,LOW);
					DIO_Write(MOTOR_OUT_1B_GPIO,MOTOR_OUT_1B_BIT,HIGH);
					break;
					case MOTOR_STOP:
					DIO_Write(MOTOR_OUT_1A_GPIO,MOTOR_OUT_1A_BIT,LOW);
					DIO_Write(MOTOR_OUT_1B_GPIO,MOTOR_OUT_1B_BIT,LOW);
					break;
					default:
						u8_fun_status = NOK;
					break;
				}	
			break;
			case MOTOR_2:
				switch(Motor_Direction)
				{
					case MOTOR_FORWARD:
					DIO_Write(MOTOR_OUT_2A_GPIO,MOTOR_OUT_2A_BIT,HIGH);
					DIO_Write(MOTOR_OUT_2B_GPIO,MOTOR_OUT_2B_BIT,LOW);
					break;
					case MOTOR_BACKWARD:
					DIO_Write(MOTOR_OUT_2A_GPIO,MOTOR_OUT_2A_BIT,LOW);
					DIO_Write(MOTOR_OUT_2B_GPIO,MOTOR_OUT_2B_BIT,HIGH);
					break;
					case MOTOR_STOP:
					DIO_Write(MOTOR_OUT_2A_GPIO,MOTOR_OUT_2A_BIT,LOW);
					DIO_Write(MOTOR_OUT_2B_GPIO,MOTOR_OUT_2B_BIT,LOW);
					break;
					default:
					u8_fun_status = NOK;
					break;
				}
			break;
			default:
				u8_fun_status = NOK;
			break;
		}
	}
	return u8_fun_status;
}


ERROR_STATUS Motor_Start(uint8_t Motor_Number, uint8_t Mot_Speed)
{
	uint8_t u8_fun_status = OK;
	
	if(Motor_Number > MOTOR_2 || Mot_Speed > 100)
	{	
		u8_fun_status = NOK;
	}
	else
	{
		switch(Motor_Number)
		{
			case MOTOR_1:
				Pwm_Start(gstr_pwm_cfg.Channel,Mot_Speed,MOTOR_FREQ);
			break;
			case MOTOR_2:
				Pwm_Start(gstr_pwm_cfg.Channel,Mot_Speed,MOTOR_FREQ);
			break;
			default:
				u8_fun_status = NOK;
			break;
		}
	}
	return u8_fun_status;
}

ERROR_STATUS Motor_SpeedUpdate(uint8_t Motor_Number, uint8_t Speed)
{
	uint8_t u8_fun_status = OK;

	if(Motor_Number > MOTOR_2 || Speed > 100)
	{
		u8_fun_status = NOK;
	}
	else
	{
		switch(Motor_Number)
		{
			case MOTOR_1:
			Pwm_Update(gstr_pwm_cfg.Channel,Speed,MOTOR_FREQ);
			break;
			case MOTOR_2:
			Pwm_Update(gstr_pwm_cfg.Channel,Speed,MOTOR_FREQ);
			break;
			default:
			u8_fun_status = NOK;
			break;
		}
	}
	return u8_fun_status;
}

ERROR_STATUS Motor_Stop(uint8_t Motor_Number)
{
	uint8_t u8_fun_status = OK;

	if(Motor_Number > MOTOR_2)
	{
		u8_fun_status = NOK;
	}
	else
	{
		switch(Motor_Number)
		{
			case MOTOR_1:
			Pwm_Update(gstr_pwm_cfg.Channel,ZERO,MOTOR_FREQ);
			break;
			case MOTOR_2:
			Pwm_Update(gstr_pwm_cfg.Channel,ZERO,MOTOR_FREQ);
			break;
			default:
			u8_fun_status = NOK;
			break;
		}
	}
	return u8_fun_status;
}

 ERROR_STATUS Motor_GetStatus(uint8_t Motor_Number, uint8_t* Mot_status)
{
	uint8_t u8_fun_status = OK;

	if(Motor_Number > MOTOR_2)
	{
		u8_fun_status = NOK;
	}
	else
	{
		switch(Motor_Number)
		{
			case MOTOR_1:
				*Mot_status = gu8_motor_1_init_status;
			break;
			case MOTOR_2:
				*Mot_status = gu8_motor_2_init_status;
			break;
			default:
			u8_fun_status = NOK;
			break;
		}
	}
	return u8_fun_status;
}