/*
 * BCM.c
 *
 * Created: 3/2/2020 7:32:11 PM
 *  Author: mahmo
 */ 
#include "BCM.h"


#define STATE_IDLE					0
#define STATE_SENDING_BYTE			1
#define STATE_SEN_BYTE_COMPLETE		2
#define BCM_FRAM_HEADER_OVERHEAD	4 /*4 bytes added to user data buffer*/

/*USE THESE VALUES FOR gas8_init_cbanals_staus ARRAY*/
#define BCM_CHANAL_UNINTALIZED		-1
#define BCM_CHANAL_NO_TASKS			 0


typedef struct bcm_taskControlBlock{
	gstr_BCM_Task_cfg_t bcmTask;/*user configuration*/
	uint8_t  u8_taskStatus;/*idle,sending,complete_sending*/
	uint8_t  u8_counter; /*counter to point at last sent byte*/
	uint64_t u8_checkSum;/*hold sum of the bytes*/
	uint8_t  u8_BCM_Id;/*to hold predetermined bcm_id*/
	uint8_t  u8_BCM_framSize;/*data size +2 bytes for numOfBytes + 1byte for BCM_ID +1 byte for checksum*/
}bcm_taskControlBlock_t;

typedef struct {
	uint8_t BCM_Ch_ID ;/*to hold predetermined bcm_id*/
	uint16_t* Data_size;
	uint32_t *Data_buffer;
	uint8_t CS;       /*hold sum of the bytes*/
}DATA_BUFFER_t;

/*add one dimentional for task mod*/
static sint8_t gas8_init_chanals_stauts[BCM_MAX_CHANALS];		/*num of tasks in each comm chanal for uinit chanals value is -1*/
static bcm_taskControlBlock_t g2astr_BCM_Tasks[BCM_MAX_CHANALS][MAX_TASK_NUM]; /*for each comm chanal hold the max number of tasks*/

/*indxr for each chanal to indicate  which task CBF will operate on */
static uint8_t u8_SPI_TaskIndx = ZERO;
static uint8_t u8_UART_TaskIndx;
static uint8_t u8_I2C_TaskIndx;


static void BCM_spiCBF(void)
{
	/*	-check for task status 
	*		-if task status == sending byte 
	*			-change state to sending
	*			-get data to send and send it
	*		-else do nothing
	*/
	/*get status*/
	uint8_t state = g2astr_BCM_Tasks[BCM_SPI_CHANAL][u8_SPI_TaskIndx].u8_taskStatus;
	/*check for status*/
	if (state == STATE_SENDING_BYTE)
	{
		/*get data and send it*/
		uint8_t u8_counter = g2astr_BCM_Tasks[BCM_SPI_CHANAL][u8_SPI_TaskIndx].u8_counter;
		uint8_t u8_data = g2astr_BCM_Tasks[BCM_SPI_CHANAL][u8_SPI_TaskIndx].bcmTask.buffer[u8_counter];
		
		/*change state to sending byte*/
		g2astr_BCM_Tasks[BCM_SPI_CHANAL][u8_SPI_TaskIndx].u8_taskStatus = STATE_SENDING_BYTE;
		SPI_sendData(u8_data);
	}
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
		
		/*unitailize all chanals*/
		for (;u8_count<BCM_MAX_CHANALS ; u8_count++)
				gas8_init_chanals_stauts[u8_count] = BCM_CHANAL_UNINTALIZED;
		
		
		
		
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
						sp_status.spi_cbf		     = BCM_spiCBF;
						
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
						sp_status.spi_cbf		     = BCM_spiCBF;
						
						SPI_init(&sp_status);
						//SPI_sendData(0);
					break;
					default: /*error state*/
					u8_fun_status = (BCM_MODULE_ERR+INVALAD_PARAMETER);
					break;
				}
				/*set spi chanal with zero tasks*/
				gas8_init_chanals_stauts[BCM_SPI_CHANAL] = BCM_CHANAL_NO_TASKS;
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
		/**/
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
		str_taskController.bcmTask = (*str_BCM_TaskCfg);
		str_taskController.u8_taskStatus = STATE_IDLE;
		str_taskController.u8_checkSum = ZERO;
		str_taskController.u8_BCM_Id =BCM_ID;
		str_taskController.u8_counter = ZERO;
		str_taskController.u8_BCM_framSize = str_BCM_TaskCfg->size + BCM_FRAM_HEADER_OVERHEAD;	
		
		
		/*set task in its chanal and its pos*/
		g2astr_BCM_Tasks[str_BCM_TaskCfg->chanal][gas8_init_chanals_stauts[str_BCM_TaskCfg->chanal]] = str_taskController;
		/*increment number of tasks for this chanal*/
		gas8_init_chanals_stauts[str_BCM_TaskCfg->chanal]++;
		/*lock the buffer*/
		*(str_BCM_TaskCfg->lock) = LOCK;
		
		

	}
	
	return u8_fun_status;
}


ERROR_STATUS BCM_RX_dispatcher();


ERROR_STATUS BCM_TX_dispatcher()
{
	/*loop through three chanal
	* -for initialized chanal loop through it's tasks
	* -for each task set task check for it's status
	*	 switch case on task state
	*		-case idle
	*			- send first byte through it's chanal[uart,spi,i2c]
	*			- change state to sending byte
	*		-case sendByteCompleted
	*			-increment task buffer counter
	*			-if last byte in buffer
	*				-change state to frame send complete
	*			-else 
	*				-change state to sending byte
	*		-case sending byte
	*			-do nothing
	*		-case frame send complete
	*			-unlock buffer, change state to completed,call back notifier function
	*/
}