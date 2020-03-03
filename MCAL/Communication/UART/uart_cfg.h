/*
 * uart_cfg.h
 *
 * Created: 3/1/2020 8:04:56 PM
 *  Author: mahmo
 */ 


#ifndef UART_CFG_H_
#define UART_CFG_H_


/*---------------------------------------- #Include & #Define ---------------------------------*/
#define USART_GPIO      (GPIOD)
#define USART_RX_BIT    (BIT0)
#define USART_TX_BIT    (BIT1)
#define BAUD 9600       // define baud

/*------------------------------------------- Structs & Enums ---------------------------------*/



/*====================================
* ======== (UCSRC) CONFIG ENUMS ======
*=====================================*/
/* Select mode enum : whether Asynchronous or Synchronous */
typedef enum sel_mode
{
	ASYNCH_MODE = 0X00,
	SYNCH_MODE  = 0X40
}Sel_mode_t;

/* Parity mode selection enum */
typedef enum sel_parity
{
	PARITY_DISABLED = 0X00,
	PARITY_RESERVED = 0X10,
	PARITY_EVEN     = 0X20,
	PARITY_ODD      = 0X30
}Sel_parity_t;

/* Stop bit select mode enum : select whether stop bit will be 1 or 2 bits */
typedef enum sel_stop_bit
{
	STOP_1_BIT = 0X00,
	STOP_2_BIT = 0X08
}Sel_stop_bit_t;

/* Register select enum : select whether (UCSRC) or (UBRRH) */
typedef enum reg_sel
{
	UBRRH_SEL = 0X00,
	UCSRC_SEL = 0X80
}Reg_select_t;

typedef enum sent_bits
{
	UART_5BIT = 0x00,
	UART_6BIT = 0x02,
	UART_7BIT = 0x04,
	UART_8BIT = 0x06
}sent_bits_t;
/*====================================
* ======== (UCSRB) CONFIG ENUMS ======
*=====================================*/
/* Choose interrupt mode enum */
typedef enum interrupt_mode
{
	RECIEVE_COMPLETE_INT = 0X80,
	TRANSMIT_COMPLETE_INT = 0X40,
	DATA_REGISTER_EMPTY_INT = 0X20,
	INTERRUPT_DIS = 0x00
}Interrupt_mode_t;

/* Transmit complete interrupt enable enum */
typedef enum usart_dir
{
	RECIEVE_EN = 0X10,
	TRANSMIT_EN = 0X08,
}Usart_dir_t;

/*====================================
* ======== (UCSRA) CONFIG ENUMS ======
*=====================================*/
/* double usart speed mode */
typedef enum u2x
{
	U2X_EN = 0X02,
	U2X_DIS = 0X00,
}U2x;

/*====================================
* ======== Usart_init  STRUCT ========
*=====================================*/
typedef struct usart_init
{
	/*-------------- UCSRC config ----------*/
	Sel_mode_t usart_mode_sel;
	Sel_parity_t parity_mode_sel;
	Sel_stop_bit_t stop_bit_sel;
	Reg_select_t reg_sel_mode;
	/*------------- UCSRB config ----------*/
	Interrupt_mode_t interrupt_mode_sel;
	Usart_dir_t usart_dir_sel;
	/*------------- UCSRA config ----------*/
	U2x double_speed_select;
}gstr_usart_init_t;

extern const gstr_usart_init_t usart_init_config;







#endif /* UART_CFG_H_ */