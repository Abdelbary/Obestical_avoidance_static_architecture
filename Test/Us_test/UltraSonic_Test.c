/*
 * UltraSonic_Test.c
 *
 * Created: 2/20/2020 5:09:17 PM
 *  Author: mahmo
 */ 


#include "ultraSonic_Test.h"
#define FUN_STATUS_COUNT	2

static uint8_t gu8_Icu_Init_fun_status;
static uint8_t gu8_DIO_Init_fun_status;
static uint8_t gu8_Icu_ReadTime_status;
static uint8_t gu8_DIO_Write_status;
static uint8_t sau16_fun_status [] = {OK,NOK}; 

#ifndef GCC   /*compiling with avr32-gcc compiler*/
ERROR_STATUS DIO_Write (uint8_t GPIO, uint8_t pins, uint8_t value)__attribute__((weak));
ERROR_STATUS Icu_ReadTime(uint8_t Icu_Channel,uint8_t Icu_EdgeToEdge, uint64_t * Icu_Time)__attribute__((weak));
ERROR_STATUS DIO_init (DIO_Cfg_s *DIO_info)__attribute__((weak));
ERROR_STATUS Icu_Init(Icu_cfg_s * Icu_Cfg)__attribute__((weak));

#else  /*gcc compiler for testing*/
void _delay_ms(double __ms)
{
	/*Mooc function to do nothing*/
}
#endif


ERROR_STATUS DIO_Write (uint8_t GPIO, uint8_t pins, uint8_t value)
{
	
	return gu8_DIO_Write_status;
}



ERROR_STATUS Icu_ReadTime(uint8_t Icu_Channel,
uint8_t Icu_EdgeToEdge, uint64_t * Icu_Time)
{	
	return 	gu8_Icu_ReadTime_status;
}

ERROR_STATUS DIO_init (DIO_Cfg_s *DIO_info)
{
	return gu8_DIO_Init_fun_status;
}
ERROR_STATUS Icu_Init(Icu_cfg_s * Icu_Cfg)
{
	return gu8_Icu_Init_fun_status;

}



void Us_Module_Test(void)
{
	printf("\n\n\t=========================Us_Module_TEST=======================\n\n");
	US_Init_test();
	Us_Trigger_test();
	Us_GetDistance_Test();
}

void US_Init_test(void)
{
	uint8_t fun1stateIndx;
	uint8_t fun2stateIndx;
	uint8_t Us_init_fun_status;
	uint8_t testCaseNumber = 0;
	
	printf("\t\t-----------------Us_Init_TEST-------------\n");
	for(fun1stateIndx = ZERO ;
	fun1stateIndx < FUN_STATUS_COUNT ;
	fun1stateIndx++)
	{
		for (fun2stateIndx = ZERO ;
		fun2stateIndx < FUN_STATUS_COUNT ;
		fun2stateIndx++)
		{
			gu8_Icu_Init_fun_status = sau16_fun_status[fun1stateIndx];
			gu8_DIO_Init_fun_status = sau16_fun_status[fun2stateIndx];
			Us_init_fun_status = Us_Init();
			if(Us_init_fun_status == (gu8_Icu_Init_fun_status&&gu8_DIO_Init_fun_status) )
			printf("testCase %d  Passed.\n",testCaseNumber);
			else
			printf("testCase %d  Failed.\n",testCaseNumber);
			testCaseNumber++;
		}
	}
	
}

void Us_Trigger_test(void)
{
	
	uint8_t fun1stateIndx;
	uint8_t Us_Trigger_fun_status;
	uint8_t testCaseNumber = 0;
	printf("\t\t-----------------Us_Trigger_TEST----------\n");
	for(fun1stateIndx = ZERO ;
	fun1stateIndx < FUN_STATUS_COUNT ;
	fun1stateIndx++)
	{
		gu8_DIO_Write_status = sau16_fun_status[fun1stateIndx];
		Us_Trigger_fun_status = Us_Trigger();
		if(Us_Trigger_fun_status == (gu8_DIO_Write_status) )
		printf("testCase %d  Passed.\n",testCaseNumber);
		else
		printf("testCase %d  Failed.\n",testCaseNumber);
		testCaseNumber++;
		
	}
}

void Us_GetDistance_Test(void)
{

	uint8_t fun1stateIndx;
	uint8_t paramterStatusIndx;
	uint8_t Us_GetDistance_status;
	uint8_t testCaseNumber = 0;
	uint64_t distant;
	uint64_t* au8_distent_pointer_test [2] = {&distant,NULL};
	printf("\t\t-----------------Us_GetDistance_TEST-------------\n");
	
	for(fun1stateIndx = ZERO ;
	fun1stateIndx < FUN_STATUS_COUNT ;
	fun1stateIndx++)
	{
		for (paramterStatusIndx = ZERO ;
		paramterStatusIndx < FUN_STATUS_COUNT ;
		paramterStatusIndx++)
		{
			gu8_Icu_ReadTime_status  = sau16_fun_status[fun1stateIndx];
			Us_GetDistance_status = Us_GetDistance(au8_distent_pointer_test[paramterStatusIndx]);
			if(Us_GetDistance_status == (gu8_Icu_ReadTime_status&& (au8_distent_pointer_test[paramterStatusIndx] !=NULL)) )
				printf("testCase %d  Passed.\n",testCaseNumber);
			else
				printf("testCase %d  Failed.\n",testCaseNumber);
			testCaseNumber++;
		}
	}
	
	
}
