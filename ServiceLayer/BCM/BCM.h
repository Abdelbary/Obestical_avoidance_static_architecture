/*
 * BCM.h
 *
 * Created: 3/2/2020 7:31:36 PM
 *  Author: mahmo
 */ 


#ifndef BCM_H_
#define BCM_H_

#include "BCM_cfg.h"
#include "../../MCAL/Communication/SPI/spi.h"
#include "../../MCAL/Communication/UART/uart.h"

typedef void(*BCM_UserNotifier)(ERROR_STATUS status);


/*define your chanals here*/
#define BCM_UART_CHANAL		0
#define BCM_SPI_CHANAL		1
#define BCM_I2C_CHANAL		2

#define BCM_MAX_CHANALS		3 /*hardware avaliable chanales*/

/*use these values for mod in bcmTask_cfg*/
#define BCM_SENDER			0
#define BCM_RECIVER			1
//#define BCM_SEND_RECIVE		2

#define	BCM_MODES			2 

typedef struct gstr_BCM_cfg{
	uint8_t chanal;		/*BCM_UART_CHANAL ,BCM_SPI_CHANAL,BCM_I2C_CHANAL*/
	uint8_t mode;		/*BCM_SENDER,BCM_RECIVER,BCM_SEND_RECIVE*/
}gstr_BCM_cfg_t;

typedef struct bcmTask_cfg{
	uint8_t *buffer;			/*data buffer to receive or send data*/
	uint8_t *lock;				/*lock variable on buffer*/
	BCM_UserNotifier bcm_cbf;	/*notify function*/
	uint16_t size;				/*size of buffer*/
	uint8_t chanal;				/*uart spi i2c*/
	uint8_t mode;				/*send || recive*/
}gstr_BCM_Task_cfg_t;

/*
*
*
*
*/
ERROR_STATUS BCM_init(gstr_BCM_cfg_t* bcm_cfg );
/*
*
*
*
*/
ERROR_STATUS BCM_setup(gstr_BCM_Task_cfg_t* str_BCM_TaskCfg);
/*
*
*
*
*/
ERROR_STATUS BCM_RX_dispatcher();
/*
*
*
*
*/
ERROR_STATUS BCM_TX_dispatcher();

#endif /* BCM_H_ */