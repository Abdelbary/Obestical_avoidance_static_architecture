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


/*main program compiled using AVR32-GCC*/
#ifndef GCC
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

int main(void)
{
	//sei();
	TMU_Init(&TMU_linkCfg);
	//Us_Module_Test();
	TMU_start(1,toogle_led,3,PERIODIC);
	TMU_start(2,toogle_led2,6,PERIODIC);
	/*TMU_Stop(1);
	TMU_Stop(2);*/
	
	//TMU_DeInit();
	while (1)
	{
		TMU_dispatcher();
	}
	/*sei();
	Car_SM_Init();
	
    while (1) 
    {
		Car_SM_Update();
    }*/
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