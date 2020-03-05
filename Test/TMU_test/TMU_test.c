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
#define POINTER_STATE_TESTS					2



#define ERROR_ID						TMU_OBJ_BUFFER_SIZE+ONE
#define ID_PARAMTER_TESTS				6
#define LAP_TIME_PARAMTER_TESTS			4
#define TYPES_PARAMTER_TESTS			3
#define ERROR_LAP_TIME					TMU_MAX_LAP_TIME+ONE
#define ERROR_TYPE_PARAMTER				ON_SHOT+ONE

					
#define invald_ID                      200
volatile uint16_t timer0_MS_flag;

static uint8_t gu8_Timer_Init_fun_status;
static uint8_t gu8_Timer_Start_fun_status;
static uint8_t gu8_Timer_DeInit_status;
/*
static sint16_t sau16_fun_status [FUN_STATUS_COUNT] = {OK,NOK};
*/


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

void checkTestCase(sint16_t s16_expected_return , sint16_t s16_fun_return )
{
	static uint8_t u8_testCaseNumber = 0;
	printf("testCase %3d:	expected_return : %3d	 fun_return: %3d	",u8_testCaseNumber,s16_expected_return,s16_fun_return);
	/*check on test case*/
	if (s16_fun_return == s16_expected_return)
	printf("Passed.\n");
	else
	printf("Failed.\n");
	u8_testCaseNumber++;
}


ERROR_STATUS TMU_start_Id_rapper(uint8_t Id)
{
	return TMU_start(Id,stub_comsumer_fun,ONE,PERIODIC);
}


ERROR_STATUS TMU_start_lapTime_rapper(uint8_t lap_time)
{
	static uint8_t Id = ZERO;
	sint16_t s16_fun_status =  TMU_start(Id,stub_comsumer_fun,lap_time,PERIODIC);
	Id++;
	return s16_fun_status;
}

ERROR_STATUS TMU_start_callBFun_rapper(uint8_t CBF_indx)
{
	static uint8_t Id = ZERO;
	void (*fun_pointers_parameter[POINTER_STATE_TESTS])(void)= {stub_comsumer_fun,NULL};
	sint16_t s16_fun_status =  TMU_start(Id,fun_pointers_parameter[CBF_indx],ONE,PERIODIC);
	Id++;
	return s16_fun_status;
}

ERROR_STATUS TMU_start_types_rapper(uint8_t type)
{
	static uint8_t Id = ZERO;
	sint16_t s16_fun_status =  TMU_start(Id,stub_comsumer_fun,ONE,type);
	Id++;
	return s16_fun_status;
}
ERROR_STATUS TMU_init_timerCh_rapper(uint8_t parm)
{
	TMU_linkCfg.timer_ch = parm;
	TMU_linkCfg.tick_reslution = ONE;
	sint16_t s16_fun_status =  TMU_Init(&TMU_linkCfg);
	TMU_DeInit();
	return s16_fun_status;
}

ERROR_STATUS TMU_init_struct_rapper(uint8_t parm)
{
	gstr_TMU_cfg_t *cfg_struct_pointer[POINTER_STATE_TESTS] = {&TMU_linkCfg,NULL};

	sint16_t s16_fun_status =  TMU_Init(cfg_struct_pointer[parm]);
	TMU_DeInit();
	return s16_fun_status;
}
ERROR_STATUS TMU_init_timeReslution_rapper(uint8_t parm)
{
	TMU_linkCfg.timer_ch = ZERO;
	TMU_linkCfg.tick_reslution = parm;
	sint16_t s16_fun_status =  TMU_Init(&TMU_linkCfg);
	TMU_DeInit();
	return s16_fun_status;
}


void parameter_test(uint8_t* u8_testParamterValues,uint8_t u8_size ,ERROR_STATUS (*callB_fun)(uint8_t), sint16_t ERROR_VALUE)
{
	uint8_t u8_counter = 0 ;
	sint16_t s16_expected_return = OK;
	sint16_t s16_fun_return;
	for (;u8_counter < u8_size ;u8_counter++ )
	{
		s16_fun_return = callB_fun(u8_testParamterValues[u8_counter]);
		
		/*check for expected fun status*/
		if(u8_testParamterValues[u8_counter] == u8_testParamterValues[u8_size-ONE])
		s16_expected_return = (ERROR_VALUE);
		
		checkTestCase(s16_expected_return , s16_fun_return);

	}
}


void    TMU_Init_test()
{
	/*
	*	TMU_INIT has one parameter and call one fun
	*   test each element of the struct,one stub fun and struct pointer 4 test parameters needed
	*/
	/*note we are not handling the stub fun error return as it is not implemented*/
	/*test parameter for TMU_cfg struct->timer_chanal parameter and TMU_cfg structe->tick time*/
	
	sint16_t s16_expected_return = OK;
	sint16_t s16_fun_return;
	uint8_t au8_Timer_ch [TIMER_CHS_TESTS] = { TMU_TIMER_CH0,TMU_TIMER_CH1,TMU_TIMER_CH2,ERROR__TIMER_CH};
	uint8_t au8_Tick_reslution [TIMER_TICK_TEST] = { 1,3,TMU_MAX_TIMER_RESLUTION,ERROR_TIMER_RESLUTION};
	uint8_t str_tmu_cfg_pointer[POINTER_STATE_TESTS] = {0,1};
		
	
	parameter_test(au8_Timer_ch,TIMER_CHS_TESTS,TMU_init_timerCh_rapper,(TMU_MODULE_ERROR_NUM+INVALAD_PARAMETER));
	parameter_test(au8_Tick_reslution,TIMER_TICK_TEST,TMU_init_timeReslution_rapper,(TMU_MODULE_ERROR_NUM+INVALAD_PARAMETER));
	parameter_test(str_tmu_cfg_pointer,POINTER_STATE_TESTS,TMU_init_struct_rapper,(TMU_MODULE_ERROR_NUM+NULL_PTR_ERROR));

	/*last test case reinitialize the module*/
	TMU_Init(&TMU_linkCfg);
	s16_fun_return = TMU_Init(&TMU_linkCfg);
	s16_expected_return = (TMU_MODULE_ERROR_NUM+MULTIPLE_INITALIZATION);
	checkTestCase(s16_expected_return , s16_fun_return);
}




void TMU_start_test(void)
{
	uint8_t au8_task_Ids [ID_PARAMTER_TESTS] = { 1,2,3,4,TMU_OBJ_BUFFER_SIZE,ERROR_ID};
	uint8_t au8_lap_time [LAP_TIME_PARAMTER_TESTS] = {1,2,TMU_MAX_LAP_TIME,ERROR_LAP_TIME};
	uint8_t au8_CBF_state_indx[POINTER_STATE_TESTS] = {0,1};
	uint8_t au8_types[TYPES_PARAMTER_TESTS] = {PERIODIC,ONE,ERROR_TYPE_PARAMTER};
		
	sint16_t s16_expected_return = OK;
	sint16_t s16_fun_return;

	
	/*
	*	test scenario 1 
	*	module initalized with tick reslution of 1 ms and timer ch 0 
	*	-test all  parameters with initialized module
	*/
	
	TMU_Init(&TMU_linkCfg);
	parameter_test(au8_task_Ids,ID_PARAMTER_TESTS,TMU_start_Id_rapper,(TMU_MODULE_ERROR_NUM+INVALAD_PARAMETER));
	TMU_DeInit();
	TMU_Init(&TMU_linkCfg);
	parameter_test(au8_lap_time,LAP_TIME_PARAMTER_TESTS,TMU_start_lapTime_rapper,(TMU_MODULE_ERROR_NUM+INVALAD_PARAMETER));
	TMU_DeInit();
	TMU_Init(&TMU_linkCfg);	
	parameter_test(au8_CBF_state_indx,POINTER_STATE_TESTS,TMU_start_callBFun_rapper,(TMU_MODULE_ERROR_NUM+NULL_PTR_ERROR));
	TMU_DeInit();
	TMU_Init(&TMU_linkCfg);
	parameter_test(au8_types,TYPES_PARAMTER_TESTS,TMU_start_types_rapper,(TMU_MODULE_ERROR_NUM+INVALAD_PARAMETER));
	TMU_DeInit();
	
	
	/*
	*	scenario 1.1
	*	call start with same id
	*/
	TMU_Init(&TMU_linkCfg);
	TMU_start(ONE,stub_comsumer_fun,ONE,PERIODIC);	
	s16_fun_return = TMU_start(ONE,stub_comsumer_fun,ONE,PERIODIC);
	s16_expected_return = (TMU_MODULE_ERROR_NUM+INVALAD_PARAMETER);
	checkTestCase(s16_expected_return , s16_fun_return);
	TMU_DeInit();

	/*
	*	test scenario 2
	*	start tmu with no TMU_int
	*/
	s16_fun_return = TMU_start(ONE,stub_comsumer_fun,ONE,PERIODIC);
	s16_expected_return = (TMU_MODULE_ERROR_NUM+MODULE_NOT_INITALIZED);
	checkTestCase(s16_expected_return , s16_fun_return);

	/*	Test scenario 3
	*	start tmu after tmu_deinit
	*/
	TMU_Init(&TMU_linkCfg);
	TMU_DeInit();
	s16_fun_return = TMU_start(ONE,stub_comsumer_fun,ONE,PERIODIC);
	s16_expected_return = (TMU_MODULE_ERROR_NUM+MODULE_NOT_INITALIZED);
	checkTestCase(s16_expected_return , s16_fun_return);
}

void TMU_Stop_test(void)
{
	sint16_t s16_fun_return;
	sint16_t s16_expected_return = OK;	
	
	/* scenario one stop without init*/
	s16_fun_return = TMU_Stop(ONE);
	s16_expected_return = (TMU_MODULE_ERROR_NUM+MODULE_NOT_INITALIZED);
	checkTestCase(s16_expected_return , s16_fun_return);
	TMU_DeInit();
	/*-scenario two stop without start*/
	TMU_Init(&TMU_linkCfg);
	s16_fun_return = TMU_Stop(ONE);
	s16_expected_return = (TMU_MODULE_ERROR_NUM+INVALAD_PARAMETER);
	checkTestCase(s16_expected_return , s16_fun_return);
	TMU_DeInit();
	
	/*-scenario three stop invalid ID*/
	TMU_Init(&TMU_linkCfg);
	s16_fun_return =TMU_Stop(invald_ID);
	s16_expected_return = (TMU_MODULE_ERROR_NUM+INVALAD_PARAMETER);
	checkTestCase(s16_expected_return , s16_fun_return);
	TMU_DeInit();
	/*-scenario four stop same id twice*/
	TMU_Init(&TMU_linkCfg);
	TMU_start(ONE,stub_comsumer_fun,ONE,PERIODIC);
	TMU_Stop(ONE);
	s16_fun_return = TMU_Stop(ONE);
	s16_expected_return = (TMU_MODULE_ERROR_NUM+INVALAD_PARAMETER);
	checkTestCase(s16_expected_return , s16_fun_return);
	TMU_DeInit();
	/*-scenario five stop an existing id*/
	TMU_Init(&TMU_linkCfg);
	TMU_start(ONE,stub_comsumer_fun,ONE,PERIODIC);
	s16_fun_return = TMU_Stop(ONE);
	s16_expected_return = (OK);
	checkTestCase(s16_expected_return , s16_fun_return);
	TMU_DeInit();
}

void TMU_dispatcher_test(void)
{
	sint16_t s16_fun_return;
	sint16_t s16_expected_return = OK;
	/*test case 1 calling dispatcher without init tmu*/
	s16_fun_return = TMU_dispatcher();
	s16_expected_return = (TMU_MODULE_ERROR_NUM+MODULE_NOT_INITALIZED);
	checkTestCase(s16_expected_return , s16_fun_return);

	
	
	/*test case 2 calling dispatcher without anyTasks*/
	TMU_Init(&TMU_linkCfg);
	s16_fun_return = TMU_dispatcher();
	s16_expected_return = (OK);
	checkTestCase(s16_expected_return , s16_fun_return);
	TMU_DeInit();
	
	/*test case 3 calling dispatcher with buffer full*/
	TMU_Init(&TMU_linkCfg);
	for(uint8_t u8_counter = 0 ; u8_counter <TMU_OBJ_BUFFER_SIZE ; u8_counter++)
	{
		TMU_start(u8_counter,stub_comsumer_fun,u8_counter+1,PERIODIC);
	}
	s16_fun_return = TMU_dispatcher();
	s16_expected_return = (OK);
	checkTestCase(s16_expected_return , s16_fun_return);
	TMU_DeInit();
	
}


void TMU_DeInit_test(void)
{
	sint16_t s16_fun_return;
	sint16_t s16_expected_return = OK;

	/*test case 1 normal*/
	TMU_Init(&TMU_linkCfg);
	s16_fun_return = TMU_DeInit();
	s16_expected_return = (OK);
	checkTestCase(s16_expected_return , s16_fun_return);
	
	/*test case 2 nok*/
	s16_fun_return = TMU_DeInit();
	s16_expected_return = (TMU_MODULE_ERROR_NUM+MODULE_NOT_INITALIZED);
	checkTestCase(s16_expected_return , s16_fun_return);
}

void TMU_Module_run_test(void)
{
	printf("\n\n\t=========================TMU_Module_TEST=======================\n\n");
	printf("\n\t\t-----------------TMU_Init_TEST-------------\n");
	TMU_Init_test();
	printf("\n\t\t-----------------TMU_DeInit_TEST-----------\n");
	TMU_DeInit_test();
	printf("\n\t\t-----------------TMU_Start_TEST------------\n");
	TMU_start_test();
	printf("\n\t\t-----------------TMU_Stop_TEST-------------\n");
	TMU_DeInit_test();
	printf("\n\t\t-----------------TMU_Dispatcher_TEST-------\n");
	TMU_dispatcher_test();
	printf("\n\n\t=========================ENDED================================\n\n");

}