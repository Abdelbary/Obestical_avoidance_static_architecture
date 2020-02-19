/*
 * CarStaticDesign.c
 *
 * Created: 2/17/2020 10:00:07 AM
 * Author : mahmo
 */ 

#include "Application/Steering/Steering.h"
#include "Application/CarSm/car_sm.h"

int main(void)
{
	sei();
	Car_SM_Init();
	
    while (1) 
    {
		Car_SM_Update();
    }
}

