/*
 * BCM.c
 *
 * Created: 3/2/2020 7:32:11 PM
 *  Author: mahmo
 */ 
#include "BCM.h"

/*	/binary search on lower bound value <= value
*
*
*/
#define STATE_IDLE					0
#define STATE_SENDING_BYTE			1
#define STATE_SEN_BYTE_COMPLETE		2
#define STATE_FRAM_SEND_COMLETE		3
#define STATE_FINISED				4
#define BCM_FRAM_HEADER_OVERHEAD	4 /*4 bytes added to user data buffer*/

/*USE THESE VALUES FOR gas8_init_cbanals_staus ARRAY*/
#define BCM_CHANAL_UNINTALIZED		-1
#define BCM_CHANAL_NO_TASKS			 0


typedef struct bcm_taskControlBlock{
	gstr_BCM_Task_cfg_t* bcmTask;/*user configuration*/
	uint8_t  u8_taskStatus;/*idle,sending,complete_sending*/
	uint8_t  u8_counter; /*counter to point at last sent byte*/
	uint64_t u8_checkSum;/*hold sum of the bytes*/
	uint8_t  u8_BCM_Id;/*to hold predetermined bcm_id*/
	uint8_t  u8_BCM_framSize;/*data size +2 bytes for numOfBytes + 1byte for BCM_ID +1 byte for checksum*/
}bcm_taskControlBlock_t;


static sint8_t gas8_init_chanals_stauts[BCM_MAX_CHANALS][BCM_MODES];		/*num of tasks in each comm chanal for uinit chanals value is -1*/
static bcm_taskControlBlock_t g2astr_BCM_Tasks[BCM_MAX_CHANALS][BCM_MODES][MAX_TASK_NUM]; /*for each comm chanal hold the max number of tasks*/




static void BCM_spiSentCBF(void)
{
	/*	-check for task status 
	*		-if task status == sending byte 
	*			-get data to send and send it
	*			-change state to send byte complete
	*		-else do nothing
	*/
	static uint8_t u8_SPI_TaskIndx = ZERO;
	/*get status*/
	uint8_t state ;
	uint8_t loop_counter = ZERO;
	do 
	{
			state = (g2astr_BCM_Tasks[BCM_SPI_CHANAL][BCM_SENDER][u8_SPI_TaskIndx]->u8_taskStatus);
			u8_SPI_TaskIndx++;
			u8_SPI_TaskIndx = (u8_SPI_TaskIndx%(gas8_init_chanals_stauts[BCM_SPI_CHANAL][BCM_SENDER]));
			loop_counter++;
	} while (state != STATE_SENDING_BYTE && loop_counter < (gas8_init_chanals_stauts[BCM_SPI_CHANAL][BCM_SENDER]));
	
	/*check for status*/
	/*get data and send it*/
	uint8_t u8_counter = (g2astr_BCM_Tasks[BCM_SPI_CHANAL][BCM_SENDER][u8_SPI_TaskIndx]->u8_counter);
	uint8_t u8_data = (g2astr_BCM_Tasks[BCM_SPI_CHANAL][BCM_SENDER][u8_SPI_TaskIndx]->bcmTask->buffer[u8_counter]);
	SPI_sendData(u8_data);
	/*change state to sending byte*/
	(g2astr_BCM_Tasks[BCM_SPI_CHANAL][BCM_SENDER][u8_SPI_TaskIndx]->u8_taskStatus = STATE_SENDING_BYTE);
}

static uint8_t bcm_uartSend()
{

	/*implement call back fun*/
}


static void bcm_uartResive(uint8_t value)
{
	/*implement call back fun*/
}

ERROR_STATUS BCM_init(gstr_BCM_cfg_t* bcm_cfg )
{
	/*
	*	1-check for errors
	*	2-switch on chanal
	*	3-switch on mode
	*/
	ERROR_STATUS u8_fun_status = OK;
	
	if(/*check for errros*/ 0)
	{
		
	}
	else
	{
		/*
		*	1-loop on chanals to make them unitalized
		*
		*/
		gstr_uart_cfg_t uart_cfg;
		gstrSPI_spi_satus_t sp_status;
		uint8_t u8_count = ZERO;
		uint8_t u8_chanaModCounter = ZERO;
		
		/*unitailize all chanals*/
		for(;u8_chanaModCounter < BCM_MODES ; u8_chanaModCounter++)
			for (;u8_count<BCM_MAX_CHANALS ; u8_count++)
					(gas8_init_chanals_stauts[u8_count][u8_chanaModCounter]) = BCM_CHANAL_UNINTALIZED;
		
		
		
		
		switch(bcm_cfg->chanal)
		{
			case BCM_UART_CHANAL: 
				/*configure uart*/
			    uart_cfg.uartTXCBF = bcm_uartSend;
				uart_cfg.uartRXCBF = bcm_uartResive;
				switch(bcm_cfg->mode)
				{
					case BCM_SENDER:
					break;
					case BCM_RECIVER:
					break;
					case BCM_SEND_RECIVE:
					break;
					default: /*error state*/
					u8_fun_status = (BCM_MODULE_ERR+INVALAD_PARAMETER);
					break;
				}
				Uart_Init(&uart_cfg);			
			break;
			case BCM_SPI_CHANAL:
				switch(bcm_cfg->mode)
				{
					/*
					*	-initalize module
					*	-make module init and has zero tasks
					*
					*/
					case BCM_SENDER:
						sp_status.spi_mod			 = SPI_MASTER_MOD;
						sp_status.spi_opration_mod	 = SPI_INT_MOD;
						sp_status.spi_prescaler		 = SPI_PRESCALER_128;
						sp_status.spi_speed_mod		 = SPI_NORMAL_SPEED;
						sp_status.spi_sampling_mod	 = SPI_SAMPLING_MOD_0;
						sp_status.spi_data_order	 = SPI_DATA_ORDER_LSB;
						sp_status.spi_cbf		     = BCM_spiSentCBF;
						
						SPI_init(&sp_status);
						//SPI_sendData(0);
					break;
					case BCM_RECIVER:
						sp_status.spi_mod			 = SPI_SLAVE_MOD;
						sp_status.spi_opration_mod	 = SPI_INT_MOD;
						sp_status.spi_prescaler		 = SPI_PRESCALER_128;
						sp_status.spi_speed_mod		 = SPI_NORMAL_SPEED;
						sp_status.spi_sampling_mod	 = SPI_SAMPLING_MOD_0;
						sp_status.spi_data_order	 = SPI_DATA_ORDER_LSB;
						sp_status.spi_cbf		     = BCM_spiSentCBF;
						
						SPI_init(&sp_status);
						//SPI_sendData(0);
					break;
					default: /*error state*/
					u8_fun_status = (BCM_MODULE_ERR+INVALAD_PARAMETER);
					break;
				}
				/*set spi chanal with zero tasks*/
				(gas8_init_chanals_stauts[BCM_SPI_CHANAL][bcm_cfg->mode]) = BCM_CHANAL_NO_TASKS;
			break;
			case BCM_I2C_CHANAL:
			break;
			default: /*error state*/
				u8_fun_status = (BCM_MODULE_ERR+INVALAD_PARAMETER);
			break;
		}
		
	}

	return u8_fun_status;
}


ERROR_STATUS BCM_setup(gstr_BCM_Task_cfg_t* str_BCM_TaskCfg)
{
	ERROR_STATUS u8_fun_status = OK;
	
	if (/*check for errors*/ 0)
	{
		/*from check if state*/
	}
	else
	{
		/*
		*	-set all bcm task control configration
		*	-set task in its position
		*	-increment number of tak for specified chanal
		*	-lock on buffer
		*	-send first byte to start transmition
		*/
		bcm_taskControlBlock_t str_taskController;
		str_taskController.bcmTask = (str_BCM_TaskCfg);
		str_taskController.u8_taskStatus = STATE_IDLE;
		str_taskController.u8_checkSum = ZERO;
		str_taskController.u8_BCM_Id =BCM_SPI_ID;
		str_taskController.u8_counter = ZERO;
		str_taskController.u8_BCM_framSize = str_BCM_TaskCfg->size + BCM_FRAM_HEADER_OVERHEAD;	
		
		
		/*set task in its chanal and its pos*/
		(g2astr_BCM_Tasks[str_BCM_TaskCfg->chanal][str_taskController.bcmTask->mode][(gas8_init_chanals_stauts[str_BCM_TaskCfg->chanal][str_taskController.bcmTask->mode])]) = &str_taskController;
		/*increment number of tasks for this chanal*/
		(gas8_init_chanals_stauts[str_BCM_TaskCfg->chanal][str_taskController.bcmTask->mode])++;
		/*lock the buffer*/
		*(str_BCM_TaskCfg->lock) = LOCK;
		
		

	}
	
	return u8_fun_status;
}


ERROR_STATUS BCM_RX_dispatcher();


ERROR_STATUS BCM_TX_dispatcher()
{
	ERROR_STATUS u8_fun_error_status = OK;
	/*loop through three chanal
	* -for initialized chanal loop through it's tasks
	* -for each task set task check for it's status
	*	-if chanal is initiated
	*		loop thought chanal tasks
	*			 switch on chanal
	*			 case spi:
	*				 switch case on task state
	*					-case idle
	*						- send first byte through spi
	*						- change state to sending byte
	*					-case sendByteCompleted
	*						-increment task buffer counter
	*						-if last byte in buffer
	*							-change state to frame send complete
	*						-else
	*							-change state to sending byte
	*					-case sending byte
	*						-do nothing
	*						-case frame send complete
	*						-unlock buffer, change state to completed,call back notifier function
	*/
	
	uint8_t u8_BCM_chanalIndx =ZERO;
	
	/*dispatcher can serve only one request for each chanal at one super loop, recored task num to serve*/
	/*loop for each chanal on RX TASKS and serve only one task*/
	for (;u8_BCM_chanalIndx < BCM_MAX_CHANALS ; u8_BCM_chanalIndx++)
				if((gas8_init_chanals_stauts[u8_BCM_chanalIndx][BCM_SENDER]) != BCM_CHANAL_UNINTALIZED 
				&& (gas8_init_chanals_stauts[u8_BCM_chanalIndx][BCM_SENDER]) != BCM_CHANAL_NO_TASKS) /*chanal init and task exist*/
				{
					/*loop throught tasks in chanal*/
					uint8_t u8_taskINdxr = ZERO;
					for (;u8_taskINdxr < (gas8_init_chanals_stauts[u8_BCM_chanalIndx][BCM_SENDER]);u8_taskINdxr++)
					{
						/*get state and current task buffer counter and data to send*/
						uint8_t* state   =&(g2astr_BCM_Tasks[u8_BCM_chanalIndx][BCM_SENDER][u8_taskINdxr]->u8_taskStatus);	
						uint8_t* counter =&(g2astr_BCM_Tasks[u8_BCM_chanalIndx][BCM_SENDER][u8_taskINdxr]->u8_counter);
						uint8_t  bufferSize = (g2astr_BCM_Tasks[u8_BCM_chanalIndx][BCM_SENDER][u8_taskINdxr]->bcmTask->size);/*change it to cm fram size*/
						uint8_t data = ZERO; /*get data if counter didn't reach end of buffer*/
						if(*counter < g2astr_BCM_Tasks[u8_BCM_chanalIndx][BCM_SENDER][u8_taskINdxr]->u8_BCM_framSize)
							data = g2astr_BCM_Tasks[u8_BCM_chanalIndx][BCM_SENDER][u8_taskINdxr]->bcmTask->buffer[*counter];
						else
							*state = STATE_FRAM_SEND_COMLETE; /*change state*/
						switch(u8_BCM_chanalIndx)
						{
							/*	-case idle
							*		- send first byte through SPI
							*		- change state to sending byte
							*	-case sendByteCompleted
							*		-increment task buffer counter
							*		-if last byte in buffer
							*			-change state to frame send complete
							*		-else
							*			-change state to sending byte
							*	-case sending byte
							*		-do nothing
							*	-case frame send complete
							*		-unlock buffer, change state to completed,call back notifier function
							*/
							case BCM_SPI_CHANAL:
								switch(*state)
								{
									case STATE_IDLE:
										
										SPI_sendData(data);
										(*counter)++;
										if(*counter ==  bufferSize)
											*state = STATE_FRAM_SEND_COMLETE;
										else /*counter < bufferSize*/
											*state = STATE_SENDING_BYTE;
										
									break;
									case STATE_SENDING_BYTE:
										/*do nothing*/										  
									break;
									case STATE_SEN_BYTE_COMPLETE:
										SPI_sendData(data);
										(*counter)++;
										if(*counter ==  bufferSize)
											*state = STATE_FRAM_SEND_COMLETE;
										else /*counter < bufferSize*/
											*state = STATE_SENDING_BYTE;
									break;
								}
							break;
							case BCM_UART_CHANAL:
							break;
							case BCM_I2C_CHANAL:
							break;
						}
					}
					
					/*update to next task to serve*/
				}
	return u8_fun_error_status;
}
	