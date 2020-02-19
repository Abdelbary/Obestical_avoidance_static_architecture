/*
 * Steering.c
 *
 * Created: 2/18/2020 10:06:23 PM
 *  Author: mahmo
 */ 
#include "Steering.h"

static uint8_t gu8_firstTime = TRUE;

ERROR_STATUS Steering_Init(void)
{
	uint8_t fun_status = OK;
	/*Motor 1 init*/
	fun_status = Motor_Init(MOTOR_1);
	fun_status = Motor_Direction(MOTOR_1,MOTOR_FORWARD);
	/*Motor 2 init*/
	fun_status = Motor_Init(MOTOR_2);
	fun_status = Motor_Direction(MOTOR_2,MOTOR_FORWARD);
	return fun_status;
}

ERROR_STATUS Steering_SteerCar(uint8_t Steering_CarCmd, uint8_t speed)
{
	uint8_t fun_status = OK;
	if(gu8_firstTime)
	{
		gu8_firstTime = FALSE;
		/*if first time start two motors*/
		Motor_Start(MOTOR_1,speed);
		Motor_Start(MOTOR_2,speed);
	}
	
	/*set motor speed*/
	Motor_SpeedUpdate(MOTOR_1,speed);
	Motor_SpeedUpdate(MOTOR_2,speed);
	
	
	switch(Steering_CarCmd)
	{
		case CAR_STOP:
			Motor_Stop(MOTOR_1);
			Motor_Stop(MOTOR_2);
		break;
		case CAR_FORWARD:
			Motor_Direction(MOTOR_1,MOTOR_FORWARD);
			Motor_Direction(MOTOR_2,MOTOR_FORWARD);			
		break;
		case CAR_BACKWARD:
			Motor_Direction(MOTOR_1,MOTOR_BACKWARD);
			Motor_Direction(MOTOR_2,MOTOR_BACKWARD);
		break;
		case CAR_RIGHT:
			Motor_Direction(MOTOR_1,MOTOR_BACKWARD);
			Motor_Direction(MOTOR_2,MOTOR_FORWARD);
		break;
		case CAR_LEFT:
			Motor_Direction(MOTOR_1,MOTOR_FORWARD);
			Motor_Direction(MOTOR_2,MOTOR_BACKWARD);
		break;
		default:
			fun_status = NOK;
		break;
	}
	
	return fun_status;
}