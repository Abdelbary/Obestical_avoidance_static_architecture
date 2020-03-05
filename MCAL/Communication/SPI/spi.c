#include "spi.h"


/* SPSR */
#define SPIF    7
#define WCOL    6
/* bits 5-1 reserved */
#define SPI2X   0

/* SPCR */
#define SPIE    7
#define SPE     6
#define DORD    5
#define MSTR    4
#define CPOL    3
#define CPHA    2
#define SPR1    1
#define SPR0    0
static SPI_CBF  spi_cbf;

ISR(SPI_STC_vect)
{
	spi_cbf();
}

ERROR_STATUS SPI_init(gstrSPI_spi_satus_t * spi_cfg){
	ERROR_STATUS u8_fun_status = OK;
	/************************************************************************
	*                     SPI INITIATION STEPS                            
	*	1-SET PINS DIRECTION DEPENDING ON MASTER OR SLAVE
	*	2-SET PRESCALER VALUE
	*	3-SET OPERATION MODE [INT NORMAL]
	*	4-SET SPEED MOD [NORMAL DOUBLE]
	*	5-SET SAMPLING AND SETUP MOD
	*	6-SET DATA DIRECTION
	*	7-ENABLE SPI 
	************************************************************************/

	switch(spi_cfg->spi_mod)
	{
	case SPI_MASTER_MOD:
	  /******** Configure SPI Master Pins *********
	 * SS(PB4)   --> Output
	 * MOSI(PB5) --> Output 
	 * MISO(PB6) --> Input
	 * SCK(PB7) --> Output
	 ********************************************/
		 SET_BIT(SPI_DIR_PORT,SPI_SS);
		 SET_BIT(SPI_DIR_PORT,SPI_MOSI);
		 SET_BIT(SPI_DIR_PORT,SPI_SCK);
		 CLEAR_BIT(SPI_DIR_PORT,SPI_MISO);
		 
		 
		 /*set ss to zero*/
		 CLEAR_BIT(SPI_DATA_PORT,SPI_SS);

	break;
	case SPI_SLAVE_MOD:
	/******** Configure SPI Slave Pins *********
	 * SS(PB4)   --> Input
	 * MOSI(PB5) --> Input
	 * MISO(PB6) --> Output
	 * SCK(PB7) --> Input
	 ********************************************/
		 CLEAR_BIT(SPI_DIR_PORT,SPI_SS);
		 CLEAR_BIT(SPI_DIR_PORT,SPI_MOSI);
		 CLEAR_BIT(SPI_DIR_PORT,SPI_SCK);
		 SET_BIT(SPI_DIR_PORT,SPI_MISO);
	
	break;
	}
	SET_MASK(SPCR,spi_cfg->spi_prescaler);
	SET_MASK(SPCR,spi_cfg->spi_sampling_mod);
	SET_MASK(SPCR,spi_cfg->spi_mod);
	SET_MASK(SPCR,spi_cfg->spi_data_order);
	SET_MASK(SPCR,spi_cfg->spi_opration_mod);
	SET_MASK(SPSR,spi_cfg->spi_speed_mod);
	spi_cbf =   (spi_cfg->spi_cbf);

	return u8_fun_status;
}


ERROR_STATUS SPI_sendData(const uint8_t data) 
{
	ERROR_STATUS u8_fun_status = OK;
	SPDR = data; //send data by SPI  
   	return u8_fun_status;
}

ERROR_STATUS SPI_getData(uint8_t * value)
{
	ERROR_STATUS u8_fun_status = OK;
	
	//return the received byte from SPI data register
	*value =  SPDR; 
	return u8_fun_status;
}

ERROR_STATUS spi_enable()
{
	SET_BIT(SPCR,SPE);
}