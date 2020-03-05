/*
 * uart.h
 *
 * Created: 3/1/2020 8:02:23 PM
 *  Author: mahmo
 */ 

#ifndef __UART_H__
#define __UART_H__
/*---------------------------------------- #includes ----------------------------------------*/
#include "../../../MCAL/DIO/DIO.h"
#include "../../registers.h"
#include "uart_cfg.h"
#include "../../../std_types.h"
#include "../../../interrupt.h"
/*---------------------------------------- #defines -----------------------------------------*/
#define BAUDE_RATE 103 /* this value generates baude rate of 9600 at Fosc = 16MHz */
typedef uint8_t(*UartTX_CBF)(void);
typedef void(*UartRX_CBF)(uint8_t);

typedef struct gstr_uart_cfg{
	UartTX_CBF uartTXCBF;
	UartRX_CBF uartRXCBF;
}gstr_uart_cfg_t;


/*
*  Description : Initializes USART.
*
*  @param void
*  @return void
*/
void Uart_Init(gstr_uart_cfg_t* uart_cfg);



/*
*  Description : Write a character to TXB
*
*  @param uint8_t  ch
*  @return void
*/
void UartTX_Enable();

uint8_t UDR_ReadyStatus(void);

#endif /* END OF __USART_H__ */



