/*
 * CarStaticDesign.c
 *
 * Created: 2/17/2020 10:00:07 AM
 * Author : mahmo
 */ 

#include "Application/Steering/Steering.h"
#include "Application/CarSm/car_sm.h"
#include "Test/Us_test/UltraSonic_test.h"
#include "ServiceLayer/TMU/TMU.h"
#include "MCAL/Communication/UART/uart.h"
#include "MCAL/Communication/SPI/spi.h"
#include "ServiceLayer/BCM/BCM.h"

/*main program compiled using AVR32-GCC*/
#ifndef GCC
#define  NEW_LINE	0x0D
#define BUFFER_SIZE	100
static	uint8_t buffer[BUFFER_SIZE];
static	uint8_t lock1 = UNLOCK;
uint8_t u8_counter = ZERO;

void bcm_ReciveNotifier(ERROR_STATUS status);
void bcm_SenderNotifier(ERROR_STATUS status);
gstr_BCM_Task_cfg_t gstr_BCM_ReciverTask_cfg = {buffer,&lock1,bcm_ReciveNotifier,ZERO,BCM_SPI_CHANAL,BCM_RECIVER};
gstr_BCM_Task_cfg_t gstr_BCM_SenderTask_cfg = {buffer,&lock1,bcm_SenderNotifier,ZERO,BCM_SPI_CHANAL,BCM_SENDER};

void toogle_led(void)
{
	 PORTA_DIR = 0xff;
	
	 PORTA_DATA ^= 0xF0;
}

void toogle_led2(void)
{
	PORTA_DIR = 0xff;
	
	PORTA_DATA ^= 0x0f;
}
static uint8_t shared;

uint8_t BCM_Reciver_sendTX(void)
{
	return shared;
}

void BCM_Reciver_reciveRX(uint8_t u8_sentData)
{
	shared = u8_sentData+1;
	UartTX_Enable();
}

uint8_t BCM_Sender_sendTX(void)
{
	return shared;
}

void BCM_Sender_reciveRX(uint8_t u8_sentData)
{
	/*shared = u8_sentData+1;
	UartTX_Enable();*/
	/*
	*	-send recived data to buffer
	*	-increment counter
	*/

	if (u8_sentData == NEW_LINE)
	{
		gstr_BCM_SenderTask_cfg.size = u8_counter;
		u8_counter = ZERO;
		BCM_setup(&gstr_BCM_SenderTask_cfg);
	}
	else
	{
		buffer[u8_counter]= u8_sentData;
		u8_counter++;
	}
	

}


/*
void spiCBF(void)
{
	static char i=0;
	SPI_sendData(i);
	i++;
}*/

void bcm_ReciveNotifier(ERROR_STATUS status)
{
	/*resetup for next round*/
	PORTA_DIR	 = 0xff;
	PORTA_DATA	 ^= 0xff;
	uint8_t u8_count = ZERO;
	/*send bytes throught u uart*/
	for (;u8_count < status ; u8_count++)
	{
		while(UDR_ReadyStatus() != TRUE);
		shared = buffer[u8_count];
		UartTX_Enable();
	}
		while(UDR_ReadyStatus() != TRUE);
		shared = NEW_LINE;
		UartTX_Enable();
	BCM_setup(&gstr_BCM_ReciverTask_cfg);
}

void bcm_SenderNotifier(ERROR_STATUS status)
{
	
}

void BCM_Sender(void)
{
	gstr_BCM_cfg_t bcm_cfg={BCM_SPI_CHANAL,BCM_SENDER};
	BCM_init(&bcm_cfg);
	
    gstr_uart_cfg_t uart_cfg = {BCM_Sender_sendTX,BCM_Sender_reciveRX};
	Uart_Init(&uart_cfg);
	sei();
	


	while(1)
	{
	/*	SPDR = 0XFF;
	_delay_ms(10);*/
		BCM_TX_dispatcher();

	}
}

void BCM_Rceiver(void)
{
	gstr_uart_cfg_t uart_cfg = {BCM_Reciver_sendTX,BCM_Reciver_reciveRX};
	Uart_Init(&uart_cfg);
	
	gstr_BCM_cfg_t bcm_cfg={BCM_SPI_CHANAL,BCM_RECIVER};
	BCM_init(&bcm_cfg);	

	BCM_setup(&gstr_BCM_ReciverTask_cfg);
	sei();

	while(1)
	{
	/*	SPDR = 0XFF;
	_delay_ms(10);*/
	BCM_RX_dispatcher();
	}
}
int main(void)
{
/*
	gstr_uart_cfg_t uart_cfg = {sendTX,reciveRX};
	Uart_Init(&uart_cfg);*/
	/*UartWriteTx('a');
	while(1)
	{
		uint8_t string[] = "mahmoud";
		uint8_t u8_count = ZERO;
		while (string[u8_count])
		{
			if (u8_TXComplteFlag)
			{
				UartWriteTx(string[u8_count]);
				u8_count++;
			}
		}
	}
	*/
	/*TMU_Init(&TMU_linkCfg);
	//Us_Module_Test();
	TMU_start(1,toogle_led,3,PERIODIC);
	TMU_start(2,toogle_led2,6,PERIODIC);
	/ *TMU_Stop(1);
	TMU_Stop(2);* /
	
	//TMU_DeInit();
	while (1)
	{
		TMU_dispatcher();
	}
	/ *sei();
	Car_SM_Init();
	
    while (1) 
    {
		Car_SM_Update();
    }* /*/
	//PORTB_DIR = 0xff;
	
	/*gstrSPI_spi_satus_t sp_status;
	sp_status.spi_mod			 = SPI_MASTER_MOD;
	sp_status.spi_opration_mod	 = SPI_INT_MOD;
	sp_status.spi_prescaler		 = SPI_PRESCALER_128;
	sp_status.spi_speed_mod		 = SPI_NORMAL_SPEED;
	sp_status.spi_sampling_mod	 = SPI_SAMPLING_MOD_0;
	sp_status.spi_data_order	 = SPI_DATA_ORDER_LSB;
	sp_status.spi_cbf		     = spi;
	SPI_init(&sp_status);
	
	SPI_sendData(0);
*/
	//BCM_Rceiver();
	BCM_Sender();
	return 0 ;
}
#endif

/*for gcc compiler used in unit-testing*/
#ifdef GCC

#include "Application/Steering/Steering.h"
#include "Application/CarSm/car_sm.h"
#include "Test/Us_test/UltraSonic_test.h"
#include "ServiceLayer/TMU/TMU.h"
#include "Test/TMU_test/TMU_test.h"

int main(void)
{
	 TMU_Module_run_test();

	return 0;
}
#endif // _DEBUG