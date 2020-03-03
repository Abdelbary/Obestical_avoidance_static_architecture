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

#define BUFFER_SIZE	100
/*main program compiled using AVR32-GCC*/
#ifndef GCC
static	uint8_t buffer[BUFFER_SIZE];
static	uint8_t lock1 = UNLOCK;
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
uint8_t sendTX(void)
{
	return shared;
}

void reciveRX(uint8_t u8_sentData)
{
	shared = u8_sentData+1;
	UartTX_Enable();
}

void spi(void)
{
	static char i=0;
	TCNT2 = SPDR;
	SPI_sendData(i);
	i++;
}

void bcm_notifier(ERROR_STATUS status)
{
	
}
int main(void)
{
	//gstr_uart_cfg_t uart_cfg = {sendTX,reciveRX};
	//Uart_Init(&uart_cfg);
	sei();
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
	
	gstr_BCM_cfg_t bcm_cfg={BCM_SPI_CHANAL,BCM_SENDER};
	BCM_init(&bcm_cfg);	
	
	for(uint8_t u8_counter = ZERO ; u8_counter < BUFFER_SIZE ; u8_counter++)
		buffer[u8_counter] = u8_counter;	
		
	gstr_BCM_Task_cfg_t bcm_task_cfg = {buffer,&lock1,bcm_notifier,
										BUFFER_SIZE,BCM_SPI_CHANAL,BCM_SENDER};
	BCM_setup(&bcm_task_cfg);
	
	while(1)
	{
	/*	SPDR = 0XFF;
	_delay_ms(10);*/
	}
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