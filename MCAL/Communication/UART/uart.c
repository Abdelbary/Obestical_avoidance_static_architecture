/*
 * uart.c
 *
 * Created: 3/1/2020 8:19:11 PM
 *  Author: mahmo
 */ 

#include "uart.h"

#define BAUD        9600
#define UBBR_VALUE (((FCPU)/(BAUD*16UL))-1)


/* UCSRA */
#define RXC     7
#define TXC     6
#define UDRE    5
#define FE      4
#define DOR     3
#define PE      2
#define U2X     1
#define MPCM    0

/* UCSRB */
#define RXCIE   7
#define TXCIE   6
#define UDRIE   5
#define RXEN    4
#define TXEN    3
#define UCSZ2   2
#define RXB8    1
#define TXB8    0

/* UCSRC */
#define URSEL   7
#define UMSEL   6
#define UPM1    5
#define UPM0    4
#define USBS    3
#define UCSZ1   2
#define UCSZ0   1
#define UCPOL   0

static volatile uint8_t u8_UDREmptyFlag = TRUE;

volatile static UartTX_CBF TX_CBF;
volatile static UartRX_CBF RX_CBF;


ISR(USART_UDRE_vect)
{
	UDR  = TX_CBF();
	CLEAR_MASK(UCSRB,DATA_REGISTER_EMPTY_INT);
	u8_UDREmptyFlag = TRUE;
}
ISR(USART_RXC_vect)
{
	RX_CBF(UDR);
}	

void Uart_Init(gstr_uart_cfg_t* uart_cfg)
{
	/*
	*	1-set baud rate
	*	2-set data size to 8 bit
	*	3-enabel RX/TX inturrpte and enable TX/RX
	*	3-set number of stop bits to 1 , no parity , asyn mode,normal speed , ucpol = 0 , 
	*/
	TX_CBF = uart_cfg->uartTXCBF;
	RX_CBF = uart_cfg->uartRXCBF;
	/*setting baud rate*/
	UBRRL = UBBR_VALUE;
	
	/*setting 8 bits data with no stop bit,asyn mode,parity disabled,cpol = zero*/
	UCSRC = (UCSRC_SEL | UART_8BIT);
	
	/*enable both receive and transmit , enable transmit interrupt and receive interrupt*/
	UCSRB = (RECIEVE_COMPLETE_INT | RECIEVE_EN | TRANSMIT_EN);
}


/*
*  Description : Write a character to TXB
*
*  @param uint8_t  ch
*  @return void
*/
void UartTX_Enable()
{
	/*	
	*	1-set udrEmptyFlag to false	
	*	2-enable UDRE interrupter
	*/
	u8_UDREmptyFlag = FALSE;
	UCSRB |= DATA_REGISTER_EMPTY_INT;
	
}

uint8_t UDRStatus(void)
{
	return u8_UDREmptyFlag;
}
