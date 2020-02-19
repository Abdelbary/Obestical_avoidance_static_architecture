/*
 * UltraSonic.c
 *
 * Created: 2/18/2020 2:50:52 PM
 *  Author: mahmo
 */ 

#include "ultraSonic.h"
#include <util/delay.h>


#define SHIFT_TO_UPPER_NIBBLE					4
#define	SHIFT_DIVIDE_BY_2						1
#define TICKS_FOR_ONE_ULTRA_SONIC_MICRO_SEC		58.0
#define TEN_DELAY								10
ERROR_STATUS Us_Init(void)
{
	
	/*
	*	1-initalize software  ICU.
	*	2-set trigger pin as Ouptut.
	*	3-set ekko pin as INPUT.
	*/
	uint8_t fun_status = OK;
	/*initalize software icu*/
	Icu_cfg_s icu_cfg;
	icu_cfg.ICU_Ch_No = ICU_CH2;
	icu_cfg.ICU_Ch_Timer = ICU_TIMER_CH2;
	fun_status = Icu_Init(&icu_cfg);
	/*initalize trigger pin*/
	DIO_Cfg_s DIO_Cfg;
	DIO_Cfg.GPIO = ICU_TRIGGER_PORT;
	DIO_Cfg.pins = ICU_TRIGGER_PIN;
	DIO_Cfg.dir	 = OUTPUT;
	DIO_init(&DIO_Cfg);

	/*initalize ekko pin*/
	DIO_Cfg.GPIO = ICU_EKKO_PORT;
	DIO_Cfg.pins = ICU_EKKO_PIN;
	DIO_Cfg.dir	 = INPUT;
	fun_status = DIO_init(&DIO_Cfg); 
	return fun_status;
}


ERROR_STATUS Us_Trigger(void)
{
	uint8_t fun_status = OK;
	
	fun_status = DIO_Write(ICU_TRIGGER_PORT,ICU_TRIGGER_PIN,HIGH);
	_delay_ms(TEN_DELAY);
	fun_status = DIO_Write(ICU_TRIGGER_PORT,ICU_TRIGGER_PIN,LOW);
	
	return fun_status;
}

ERROR_STATUS Us_GetDistance(uint64_t *Distance)
{
	uint8_t fun_status = OK;
	uint64_t ticks = ZERO;

	fun_status = Icu_ReadTime(ICU_TIMER_CH2,ICU_FALE_TO_RISE,&ticks);
	*Distance = ((ticks>>SHIFT_DIVIDE_BY_2)/TICKS_FOR_ONE_ULTRA_SONIC_MICRO_SEC);
	return fun_status;
}
