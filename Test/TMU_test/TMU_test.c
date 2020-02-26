/*
 * TMU_test.c
 *
 * Created: 2/25/2020 3:23:35 PM
 *  Author: mahmo
 */ 
#include "TMU_test.h"
#define ERROR__TIMER_CH					3
#define TIMER_CHS_TESTS					4
#define TIMER_TICK_TEST					4
#define ERROR_TIMER_RESLUTION			(TMU_MAX_TIMER_RESLUTION+1)
#define FUN_STATUS_COUNT				2
#define POINTER_STATUS					2



#define ERROR_ID						TMU_OBJ_BUFFER_SIZE+ONE
#define ID_PARAMTER_TESTS				6
#define LAP_TIME_PARAMTER_TESTS			4
#define TYPES_PARAMTER_TESTS			3
#define ERROR_LAP_TIME					TMU_MAX_LAP_TIME+ONE
#define ERROR_TYPE_PARAMTER				ON_SHOT+ONE


volatile uint16_t timer0_MS_flag;

static uint8_t gu8_Timer_Init_fun_status;
static uint8_t gu8_Timer_Start_fun_status;
static uint8_t gu8_Timer_DeInit_status;
static sint16_t sau16_fun_status [FUN_STATUS_COUNT] = {OK,NOK};


#ifndef GCC
/*declare fun as weak to avoid multiple definition by linker*/
ERROR_STATUS    Timer_Init(Timer_cfg_s* Timer_cfg)__attribute__((weak)) ; 
ERROR_STATUS	Timer_Start(uint8_t Timer_CH_NO, uint16_t Timer_Count)__attribute__((weak));
ERROR_STATUS	Timer_DeInit(uint8_t timer_Ch)__attribute__((weak));
#endif



ERROR_STATUS    Timer_Init(Timer_cfg_s* Timer_cfg)
{
	return gu8_Timer_Init_fun_status;
}
ERROR_STATUS	Timer_Start(uint8_t Timer_CH_NO, uint16_t Timer_Count)
{
	return gu8_Timer_Start_fun_status;
}
ERROR_STATUS	Timer_DeInit(uint8_t timer_Ch)
{
	return gu8_Timer_DeInit_status;
}


void stub_comsumer_fun(void)
{
	
}

void    TMU_Init_test()
{
	/*
	*	TMU_INIT has one parameter and call one fun
	*   test each element of the struct,one stub fun and sturct pointer 4 test paramters needed
	*/
	
	/*test parameter for TMU_cfg structe->timer_chanal parameter and TMU_cfg structe->tick time*/
	
	
	uint8_t au8_Timer_ch [TIMER_CHS_TESTS] = { TMU_TIMER_CH0,TMU_TIMER_CH1,TMU_TIMER_CH2,ERROR__TIMER_CH};
	uint8_t au8_Tick_reslution [TIMER_TICK_TEST] = { 1,3,TMU_MAX_TIMER_RESLUTION,ERROR_TIMER_RESLUTION};
	gstr_TMU_cfg_t * str_tmu_cfg_pointer[POINTER_STATUS] = {&TMU_linkCfg,NULL};
	
	/*the test case counters for each paramter*/
	uint8_t u8_testParam1Counter = 0; /*struct pointer*/
	uint8_t u8_testParam2Counter = 0; /*timer ch*/
	uint8_t u8_testParam3Counter = 0; /*tick_reslution*/
	uint8_t u8_testParam4Counter = 0; /*stubfun_return_status*/
	sint16_t s16_expected_return = OK;
	uint8_t u8_testCaseNumber = 0;
	sint16_t s16_fun_return;
	for (u8_testParam1Counter = 0 ;u8_testParam1Counter < POINTER_STATUS ; u8_testParam1Counter++)
	{
		for ( u8_testParam2Counter = 0;u8_testParam2Counter <  TIMER_CHS_TESTS ; u8_testParam2Counter++ )
		{
			for ( u8_testParam3Counter = 0; u8_testParam3Counter<TIMER_TICK_TEST ; u8_testParam3Counter++)
			{
				for ( u8_testParam4Counter = 0; u8_testParam4Counter <FUN_STATUS_COUNT ;u8_testParam4Counter++  )
				{				
					
					/*give stub function values*/
					gu8_Timer_Init_fun_status = sau16_fun_status[u8_testParam4Counter];
					
					/*if cfg struct not null fill it with test cases paramters*/
					if (str_tmu_cfg_pointer[u8_testParam1Counter] != NULL )
					{
						TMU_linkCfg.timer_ch		= au8_Timer_ch[u8_testParam2Counter];
						TMU_linkCfg.tick_reslution  = au8_Tick_reslution[u8_testParam3Counter];
					}
					
					

					s16_fun_return = TMU_Init(str_tmu_cfg_pointer[u8_testParam1Counter]);


					if (str_tmu_cfg_pointer[u8_testParam1Counter] == NULL || au8_Timer_ch[u8_testParam2Counter]  == ERROR__TIMER_CH 
					 ||au8_Tick_reslution[u8_testParam3Counter]  == ERROR_TIMER_RESLUTION  || sau16_fun_status[u8_testParam4Counter]  == NOK)
					{
						s16_expected_return = NOK;
					}
					
					
					printf("%d %d %d\n",au8_Timer_ch[u8_testParam2Counter] ,au8_Tick_reslution[u8_testParam3Counter],sau16_fun_status[u8_testParam4Counter]);
					printf("%d %d %d\n",au8_Timer_ch[u8_testParam2Counter] ,au8_Tick_reslution[u8_testParam3Counter],sau16_fun_status[u8_testParam4Counter]);
					printf("%d %d\n",s16_expected_return,s16_fun_return);

					/*check for results*/
					if ((s16_fun_return == OK && s16_expected_return == NOK) || (s16_fun_return !=OK && s16_expected_return == OK)  )
						printf("testCase %d  Faild.\n",u8_testCaseNumber);
					else
						printf("testCase %d  Passed.\n",u8_testCaseNumber);
						
					/*deinit module*/
					if (s16_fun_return == OK)
						TMU_DeInit();

					s16_expected_return = OK;
					u8_testCaseNumber++;
				}
			}
		}
	}
	/*last test case reinitalize the module*/
	TMU_Init(&TMU_linkCfg);
	s16_fun_return = 	TMU_Init(&TMU_linkCfg);
	if(s16_fun_return != OK)
				printf("testCase %d  Passed.\n",u8_testCaseNumber);
}

void TMU_start_test(void)
{
	uint8_t au8_task_Ids [ID_PARAMTER_TESTS] = { 1,2,3,4,TMU_OBJ_BUFFER_SIZE,ERROR_ID};
	uint8_t au8_lap_time [LAP_TIME_PARAMTER_TESTS] = {1,2,TMU_MAX_LAP_TIME,ERROR_LAP_TIME};
	void (*fun_pointers_parameter[2])(void) = {stub_comsumer_fun,NULL};
	uint8_t au8_types[TYPES_PARAMTER_TESTS] = {PERIODIC,ONE,ERROR_TYPE_PARAMTER};
	/*the test case counters for each paramter*/
	uint8_t u8_testParam1Counter = 0; /*struct pointer*/
	uint8_t u8_testParam2Counter = 0; /*task id*/
	uint8_t u8_testParam3Counter = 0; /*lap time*/
	uint8_t u8_testParam4Counter = 0; /*task type*/
	uint8_t u8_testParam5Counter = 0; /*fun return*/
	sint16_t s16_expected_return = OK;
	uint8_t u8_testCaseNumber = 0;
	sint16_t s16_fun_return;
	/*fine paramter*/
	uint8_t u8_fine_type = PERIODIC;
	uint8_t u8_fine_ID = ONE;
	uint8_t u8_fine_lap_time = ONE;
	for (;u8_testParam1Counter < ID_PARAMTER_TESTS ;u8_testParam1Counter++ )
	{
		TMU_start(au8_task_Ids[u8_testParam1Counter],stub_comsumer_fun,u8_fine_lap_time,u8_fine_type);
	}
}
/*
void TMU_Stop_test(void)
{
}*/
void TMU_dispatcher_test(void)
{
	uint8_t u8_testCaseNumber = 0;
	sint16_t s16_fun_return;
	/*test case 1 calling dispatcher without init tmu*/
	s16_fun_return = TMU_dispatcher();
	if (s16_fun_return == OK)
		printf("testCase %d  Failed.\n",u8_testCaseNumber++);
	else
		printf("testCase %d  Passed.\n",u8_testCaseNumber);
	u8_testCaseNumber++;
	
	
	/*test case 2 calling dispatcher without anyTasks*/
	TMU_Init(&TMU_linkCfg);
	s16_fun_return = TMU_dispatcher();
	if (s16_fun_return == OK)
		printf("testCase %d  Failed.\n",u8_testCaseNumber);
	else
		printf("testCase %d  Passed.\n",u8_testCaseNumber);
	u8_testCaseNumber++;

	/*test case 3 calling dispatcher with buffer full*/
	TMU_Init(&TMU_linkCfg);
	for(uint8_t u8_counter = 0 ; u8_counter <TMU_OBJ_BUFFER_SIZE ; u8_counter++)
	{
		TMU_start(u8_counter,stub_comsumer_fun,u8_counter+1,PERIODIC);
	}
	s16_fun_return = TMU_dispatcher();
	if (s16_fun_return != OK)
		printf("testCase %d  Failed.\n",u8_testCaseNumber);
	else
		printf("testCase %d  Passed.\n",u8_testCaseNumber);
	
}


void TMU_DeInit_test(void)
{
	sint16_t s16_expected_return = OK;
	uint8_t u8_testCaseNumber = 0;
	sint16_t s16_fun_return;

	/*test case 1 normal*/
	s16_fun_return = TMU_Init(&TMU_linkCfg);
	TMU_DeInit();
	
	if (s16_fun_return != OK )
	printf("testCase %d  Failed.\n",u8_testCaseNumber);
	else
	printf("testCase %d  Passed.\n",u8_testCaseNumber);	
	u8_testCaseNumber++;
	
	/*test case 2 nok*/
	TMU_DeInit();
	if (s16_fun_return == OK  )
	printf("testCase %d  Failed.\n",u8_testCaseNumber);
	else
	printf("testCase %d  Passed.\n",u8_testCaseNumber);
	

}