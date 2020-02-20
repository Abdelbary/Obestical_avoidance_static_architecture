/*
 * DIO.c
 *
 * Created: 2/17/2020 2:16:34 PM
 *  Author: mahmo
 */ 


/*- INCLUDES ----------------------------------------------*/
#include "DIO.h"
/*- LOCAL MACROS ------------------------------------------*/
/*- LOCAL Datatypes ---------------------------------------*/
/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
/*- GLOBAL STATIC VARIABLES -------------------------------*/
/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/
/*- APIs IMPLEMENTATION -----------------------------------*/

ERROR_STATUS DIO_init (DIO_Cfg_s *DIO_info)
{
	uint8_t u8_fun_status = OK;
    /*check for errors*/
	if((DIO_info->dir != OUTPUT && DIO_info->dir != INPUT) || 
		DIO_info->GPIO > GPIOD)
	{
		u8_fun_status = NOK;
	}
	/*if input is ok proceed with fun process*/
	else
	{
			switch(DIO_info->GPIO)
			{
				case GPIOA:
					switch(DIO_info->dir)
					{
						case INPUT:
							CLEAR_MASK(PORTA_DIR,DIO_info->pins);
						break;
						case OUTPUT:
							SET_MASK(PORTA_DIR,DIO_info->pins);
						break;
					}
				break;
				case GPIOB:
					switch(DIO_info->dir)
					{
						case INPUT:
						CLEAR_MASK(PORTB_DIR,DIO_info->pins);
						break;
						case OUTPUT:
						SET_MASK(PORTB_DIR,DIO_info->pins);
						break;
					}
				break;
				case GPIOC:
					switch(DIO_info->dir)
					{
						case INPUT:
						CLEAR_MASK(PORTC_DIR,DIO_info->pins);
						break;
						case OUTPUT:
						SET_MASK(PORTC_DIR,DIO_info->pins);
						break;
					}
				break;
				case GPIOD:
					switch(DIO_info->dir)
					{
						case INPUT:
						CLEAR_MASK(PORTD_DIR,DIO_info->pins);
						break;
						case OUTPUT:
						SET_MASK(PORTD_DIR,DIO_info->pins);
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


ERROR_STATUS DIO_Write (uint8_t GPIO, uint8_t pins, uint8_t value)
{
	uint8_t u8_fun_status = OK;
	/*check for errors*/
	if(GPIO > GPIOD || (value != HIGH && value !=LOW))
	{
		u8_fun_status = NOK;
	}
	/*if input is ok proceed with fun process*/
	else
	{
		switch(GPIO)
		{
			case GPIOA:
			switch(value)
			{
				case LOW:
				CLEAR_MASK(PORTA_DATA,pins);
				break;
				case HIGH:
				SET_MASK(PORTA_DATA,pins);
				break;
			}
			break;
			case GPIOB:
			switch(value)
			{
				case LOW:
				CLEAR_MASK(PORTB_DATA,pins);
				break;
				case HIGH:
				SET_MASK(PORTB_DATA,pins);
				break;
			}
			break;
			case GPIOC:
			switch(value)
			{
				case INPUT:
				CLEAR_MASK(PORTC_DATA,pins);
				break;
				case OUTPUT:
				SET_MASK(PORTC_DATA,pins);
				break;
			}
			break;
			case GPIOD:
			switch(value)
			{
				case INPUT:
				CLEAR_MASK(PORTD_DATA,pins);
				break;
				case OUTPUT:
				SET_MASK(PORTD_DATA,pins);
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


ERROR_STATUS DIO_Read (uint8_t GPIO,uint8_t pins, uint8_t *data)
{
	uint8_t u8_fun_status = OK;
	/*check for errors*/
	if(GPIO > GPIOD )
	{
		u8_fun_status = NOK;
	}
	/*if input is ok proceed with fun process*/
	switch(GPIO)
	{
		case GPIOA:
			*data = MASK_IS_SET(PORTA_DATA,pins);
		break;
		case GPIOB:
			*data = MASK_IS_SET(PORTB_DATA,pins);
		break;
		case GPIOC:
			*data = MASK_IS_SET(PORTC_DATA,pins);
		break;
		case GPIOD:
			*data = MASK_IS_SET(PORTD_DATA,pins);
		break;
		default:
			u8_fun_status = NOK;
		break;
	}
	return u8_fun_status;
}

ERROR_STATUS DIO_Toggle (uint8_t GPIO, uint8_t pins)
{
	uint8_t u8_fun_status = OK;
	/*check for errors*/
	if(GPIO > GPIOD )
	{
		u8_fun_status = NOK;
	}
	/*if input is ok proceed with fun process*/
	switch(GPIO)
	{
		case GPIOA:
			TOGGLE_MASK(PORTA_DATA,pins);
		break;
		case GPIOB:
			TOGGLE_MASK(PORTB_DATA,pins);
		break;
		case GPIOC:
			TOGGLE_MASK(PORTC_DATA,pins);
		break;
		case GPIOD:
			TOGGLE_MASK(PORTD_DATA,pins);
		break;
		default:
			u8_fun_status = NOK;
		break;
	}
	return u8_fun_status;
}