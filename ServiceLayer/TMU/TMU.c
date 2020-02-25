/*
 * tmu.c
 *
 * Created: 2/24/2020 11:50:29 AM
 *  Author: mahmo
 */ 

#include "TMU.h"

#define MODULE_ERROR_NUM  -100
#define TAKEN				1

typedef struct TMU_obj_str
{
	/*object to hold data to the call back functions to call periodically*/
	uint8_t Id;
	void (*callB_fun)(void);
	uint8_t fire_tick;
	uint8_t current_ticks;
	uint8_t type;  /*periodic or one shot*/
	
}gstr_TMU_obj;

static uint8_t TMU_Init_flag = FALSE;
static uint8_t TMU_Timer_ch;
static gstr_TMU_obj gastr_TMU_ObjBuffer[TMU_OBJ_BUFFER_SIZE];
static sint8_t u8_TMU_objBufferHead = ZERO-ONE;
static uint8_t taken_Ids[TMU_OBJ_BUFFER_SIZE]; /*mark taken ids*/

ERROR_STATUS TMU_Init(gstr_TMU_cfg_t * tmu_cfg)
{
	sint16_t fun_error_status = OK;
	if(TMU_Init_flag || tmu_cfg == NULL || tmu_cfg->tick_reslution > TMU_MAX_TIMER_RESLUTION || tmu_cfg->timer_ch > TMU_TIMER_CH2 ) /*check for all errors*/
	{
		/************************************************************************ 
		 *	-module initialized before                          
		 *	-input validation.                                 
		 ************************************************************************/
		if (TMU_Init_flag)
		{
			fun_error_status = (MODULE_ERROR_NUM+MULTIPLE_INITALIZATION);
		}
		else if (tmu_cfg == NULL)
		{
			fun_error_status = (MODULE_ERROR_NUM+NULL_PTR_ERROR);
		}
		else if (tmu_cfg->tick_reslution > TMU_MAX_TIMER_RESLUTION 
				|| tmu_cfg->timer_ch > TMU_TIMER_CH2)
		{
			fun_error_status = (MODULE_ERROR_NUM+INVALAD_PARAMETER);
		}
	}
	else
	{
		TMU_Init_flag = TRUE;
		TMU_Timer_ch = tmu_cfg->timer_ch;
		Timer_cfg_s timer_cfg;
		switch(tmu_cfg->timer_ch)
		{
			case TMU_TIMER_CH0:
				timer_cfg.Timer_CH_NO = TIMER_CH0;
				timer_cfg.Timer_Polling_Or_Interrupt = TIMER_INTERRUPT_MODE;
				timer_cfg.Timer_Prescaler = TIMER_PRESCALER_256;
			break;
			case TMU_TIMER_CH1:
				timer_cfg.Timer_CH_NO = TIMER_CH1;
				timer_cfg.Timer_Polling_Or_Interrupt = TIMER_INTERRUPT_MODE;
				timer_cfg.Timer_Prescaler = TIMER_PRESCALER_256;
			break;
			case TMU_TIMER_CH2:
				timer_cfg.Timer_CH_NO = TIMER_CH2;
				timer_cfg.Timer_Polling_Or_Interrupt = TIMER_INTERRUPT_MODE;
				timer_cfg.Timer_Prescaler = TIMER_PRESCALER_256;
			break;
		}
		Timer_Init(&timer_cfg);
	}
	return fun_error_status;
}
ERROR_STATUS TMU_start(uint8_t Id,void (*callB_fun_ptr)(void),uint8_t lap_time,uint8_t type)
{
	sint16_t s16_fun_error_status = OK;
	
	if(Id > TMU_OBJ_BUFFER_SIZE || callB_fun_ptr == NULL || lap_time > TMU_MAX_LAP_TIME 
		|| type > ON_SHOT || TMU_Init_flag == FALSE || taken_Ids[Id] == TAKEN 
		|| (lap_time%(TMU_linkCfg.tick_reslution)) != ZERO /*lap_time not multiple of reslution*/)
	{
		if (callB_fun_ptr == NULL)
		{
			s16_fun_error_status = (MODULE_ERROR_NUM+NULL_PTR_ERROR);
		}
		else if(Id > TMU_OBJ_BUFFER_SIZE  ||taken_Ids[Id]  == TAKEN
				|| type > ON_SHOT  || lap_time > TMU_MAX_LAP_TIME
				|| (lap_time%(TMU_linkCfg.tick_reslution)))/*invalid parameter*/
		{
			s16_fun_error_status = (MODULE_ERROR_NUM+INVALAD_PARAMETER);
		}
		else /*uinitialize module error*/
		{
			s16_fun_error_status = (MODULE_ERROR_NUM+MODULE_NOT_INITALIZED);
		}
	}
	else
	{
	static uint8_t fun_frstTime_flag = FALSE;
	/*check for first time entrance to start timer*/
	if(!fun_frstTime_flag)
	{
		/*start timer*/
	
		Timer_Start(TMU_Timer_ch,TIMER0_1MS_PRESCALER256);
				
		fun_frstTime_flag = TRUE;
	}
	
	/*set how many resolution time it takes to fire event*/
	lap_time = (lap_time/(TMU_linkCfg.tick_reslution));
	/*TMU_obj_str {id,callBack fun , ticks ,current_ticks,type[periodic,onshot]} */
	gstr_TMU_obj tobj  = {Id,callB_fun_ptr,lap_time,ZERO,type};		
	u8_TMU_objBufferHead++; /*increase buffer head to next empty position*/
	gastr_TMU_ObjBuffer[u8_TMU_objBufferHead] = tobj;
	
	taken_Ids[Id] = TAKEN ;/*mark this id as taken*/
	}
	return s16_fun_error_status;
}



ERROR_STATUS TMU_Stop(uint8_t Id)
{
	sint16_t s16_fun_error_status = OK;
	
	if (Id>TMU_OBJ_BUFFER_SIZE || taken_Ids[Id] == TAKEN)
	{
		s16_fun_error_status = (MODULE_ERROR_NUM+INVALAD_PARAMETER);
	}
	/*
	*  -loop for element in the buffer until reach head
	*				- copy header obj to it 
	*				- decrement header
	*/
	for(uint8_t u8_bufferCnt = 0 ; u8_bufferCnt <=u8_TMU_objBufferHead ; u8_bufferCnt++)
	{
		if(Id == gastr_TMU_ObjBuffer[u8_bufferCnt].Id)
		{
			gastr_TMU_ObjBuffer[u8_bufferCnt] = gastr_TMU_ObjBuffer[u8_TMU_objBufferHead];
			u8_TMU_objBufferHead--;
		}
	}
	

	return s16_fun_error_status;
	
}



ERROR_STATUS	TMU_dispatcher(void)
{
	sint16_t s16_fun_error_status = OK;
	static uint8_t current_ticks = ZERO;
	static uint8_t new_tick_flag = FALSE;
	/*
	*	-check for new time tick
	*	-if true loop throw the TMU OBJ Buffer
	*	-increment current ticks 
	*	-check if the current ticks == tick time if so call the CBF
	*	-keep going until finish with all obj in TMU buffer
	*/
	
	/*	check for new time tick
	*	for each timer tick increase current tick and make timer0_MS_flag false
	*	if current tick == lap_time
	*		-zero current tick
	*		-fire new_tick_flag
	*/
	if (timer0_MS_flag)
	{
		timer0_MS_flag = FALSE;
		current_ticks++;
		if (current_ticks == TMU_linkCfg.tick_reslution)
		{
			current_ticks = ZERO;
			new_tick_flag = TRUE;
		}
	}
	
	if(new_tick_flag)
	{
		new_tick_flag = FALSE;
		for(uint8_t bufferCnt = 0 ; bufferCnt <=u8_TMU_objBufferHead ; bufferCnt++)
		{
			(gastr_TMU_ObjBuffer[bufferCnt].current_ticks)++;
			/*check for CBF call time*/
			if((gastr_TMU_ObjBuffer[bufferCnt].current_ticks)==(gastr_TMU_ObjBuffer[bufferCnt].fire_tick))
				{
					gastr_TMU_ObjBuffer[bufferCnt].current_ticks = ZERO;
					gastr_TMU_ObjBuffer[bufferCnt].callB_fun();
					
					/*handle one_shot functions*/
					if (gastr_TMU_ObjBuffer[bufferCnt].type == ON_SHOT)
					{
						TMU_Stop(gastr_TMU_ObjBuffer[bufferCnt].Id);
					}

				}
		}
	 }

	return s16_fun_error_status;
}


ERROR_STATUS TMU_DeInit(void)
{
	
	/*
	*	1-deinit timer
	*	2-set TMU_init_flag to zero
	*/
	sint16_t s16_fun_error_status = TRUE;
	
	if(TMU_Init_flag == FALSE)
	{
		/*report error*/
		s16_fun_error_status = (MODULE_ERROR_NUM+MODULE_NOT_INITALIZED);
	}
	else
	{
		switch(TMU_Timer_ch)
		{
			case TMU_TIMER_CH0:
				timer_DeInit(TIMER_CH0);
			break;
			case TMU_TIMER_CH1:
				timer_DeInit(TIMER_CH1);			
			break;
			case TMU_TIMER_CH2:
				timer_DeInit(TIMER_CH2);
			break;
		}
	TMU_Init_flag = FALSE;
	u8_TMU_objBufferHead = ZERO-ONE;
	}
	return s16_fun_error_status;
}

