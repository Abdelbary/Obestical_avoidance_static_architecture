/*
 * CarStaticDesign.c
 *
 * Created: 2/17/2020 10:00:07 AM
 * Author : mahmo
 */ 

#include "MCAL/Timers/Timer.h"
#include "MCAL/DIO/DIO.h"
#include "MCAL/PWM/PWM.h"
#include "HAL/Motor/motor.h"
#include "SWICU/SWICU.h"
#include <util/delay.h>

int main(void)
{
    /* Replace with your application code */
/*
	Timer_cfg_s timer_cfg;
	timer_cfg.Timer_CH_NO				    =	TIMER_CH0;
	timer_cfg.Timer_Mode					=	TIMER_MODE;
	timer_cfg.Timer_Polling_Or_Interrupt	=	TIMER_INTERRUPT_MODE;
	timer_cfg.Timer_Prescaler				=   TIMER_PRESCALER_256;
	Timer_Init(&timer_cfg);
	Timer_Start(TIMER_CH0,50);
	DIO_Cfg_s dio_cfg;
	dio_cfg.dir = OUTPUT;
	dio_cfg.GPIO = GPIOB;
	dio_cfg.pins = FULL_PORT;
	DIO_init(&dio_cfg);
	uint8_t data;
	DIO_Write(GPIOB,10,HIGH);
	DIO_Read(GPIOB,0xff,&data);
	dio_cfg.GPIO = GPIOA;
	DIO_init(&dio_cfg);
	DIO_Write(GPIOA,data,HIGH);
	Pwm_Cfg_s pwm_cfg;
	pwm_cfg.Channel = PWM_CH1A|PWM_CH1B;
	pwm_cfg.Prescaler = PWM_PRESCALER_8;
	Pwm_Init(&pwm_cfg);
	Pwm_Start(PWM_CH1A|PWM_CH1B,99,1000);
*/
	Icu_cfg_s icu_cfg;
	icu_cfg.ICU_Ch_No = ICU_CH2;
	icu_cfg.ICU_Ch_Timer = ICU_TIMER_CH0;
	//PORTA_DIR  = 0xff;
	sei();





	Motor_Init(MOTOR_1);
	Motor_Direction(MOTOR_1,MOTOR_FORWARD);
	Motor_Start(MOTOR_1,100);
	Motor_Init(MOTOR_2);
	Motor_Direction(MOTOR_2,MOTOR_FORWARD);
	Motor_Start(MOTOR_2,100);

	
	/*Motor_SpeedUpdate(MOTOR_1,90);
	Motor_SpeedUpdate(MOTOR_2,90);*/
/*
	Motor_Stop(MOTOR_1);
	Motor_Stop(MOTOR_2);
	Motor_Start(MOTOR_1,100);
	Motor_Start(MOTOR_2,100);
*/
/*
	TCCR1 = 0b1111001000010010;
	ICR1  = 2000;
	OCR1A = 1000;
	OCR1B = 1000;
	PORTD_DIR = 0xff;
*/  
	PORTB_DIR |=0xF0;
	PORTD_DIR |=BIT0;
	PORTA_DIR |=0xff;
	uint32_t dis;
    while (1) 
    {
		Icu_Init(&icu_cfg);
		PORTD_DATA |= BIT0;
		_delay_ms(10);
		PORTD_DATA &= ~BIT0;
		Icu_ReadTime(icu_cfg.ICU_Ch_No,ICU_FALE_TO_RISE,&dis);
		dis = ((dis*8)/27.0);
		//dis = (dis*15)/200.0;
		TCNT1L = dis;
		PORTB_DATA = (PORTB_DATA&0x0F) | (dis<<4);
		
		if(dis < 15)
		{
			Motor_Stop(MOTOR_1);
			Motor_Stop(MOTOR_2);
		}
		else
		{
			Motor_Start(MOTOR_1,50);
			Motor_Start(MOTOR_1,50);
		}
		_delay_ms(100);
    }
}

