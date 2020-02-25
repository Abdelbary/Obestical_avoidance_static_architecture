/*
 * TMU.h
 *
 * Created: 2/24/2020 11:51:33 AM
 *  Author: mahmo
 */ 


#ifndef TMU_H_
#define TMU_H_

#include "..\..\std_types.h"
#include "..\..\common_macros.h"
#include "TMU_cfg.h"
#include "../../MCAL/Timers/Timer.h"
#include "../Error_Handler/SystemErrors.h"


#define TMU_TIMER_CH0      0
#define TMU_TIMER_CH1	   1
#define TMU_TIMER_CH2	   2

#define PERIODIC    0
#define ON_SHOT     1


typedef struct TMU_cfg_str
{
	uint8_t timer_ch;
	uint8_t tick_reslution; /* in milesec */
}gstr_TMU_cfg_t;


extern gstr_TMU_cfg_t TMU_linkCfg;

/*
* Description: TMU_init initialize TMU module with timer canal specified and 
*				and time tick reslution as multiple of 1 ms.
* @pram: tmu_cfg: timer_ch-> 
*							-TMU_TIMER_CH0
*							-TMU_TIMER_CH1
*							-TMU_TIMER_CH2
*				  tick_reslution->
*							-multiple of 1 ms
*@return: ERROR_STATUS [negative number describe module and type of errors]
*/
ERROR_STATUS    TMU_Init(gstr_TMU_cfg_t * tmu_cfg);

/*
* Description: TMU_start add new TMU_object to be served on specific time 
*				if it is the first time to enter function timer  is started.
* @pram: id -> from 0 to TMU_OBJ_BUFFER_SIZE.
*		 callB_fun_ptr-> addresss of the rotuen to be called.
*		 lap_time-> form 0 to TMU_MAX_LAP_TIME
*		 type->[PERIODIC,ONE_SHOT]
*@return: ERROR_STATUS [negative number describe module and type of errors]
*/
ERROR_STATUS	TMU_start(uint8_t Id,void (*callB_fun_ptr)(void),uint8_t lap_time,uint8_t type);

/*
* Description: TMU_stop stop task with specified id number by removing it from
*			   TMU Buffer.
* @pram: Id:  id of the task to be removed from TMU buffer 
*					0<=id<TMU_BUFFER_MAX_SIZE
*@return: ERROR_STATUS [negative number describe module and type of errors]
*/
ERROR_STATUS	TMU_Stop(uint8_t Id);

/*
* Description: TMU_dispatcher is the main TMU procedure, dispatcher check for new time
*				tick then loop through the TMU_objBuffer and execute tasks at their 
*				specified time.
* @pram: void
* @return: ERROR_STATUS [negative number describe module and type of errors]
*/
ERROR_STATUS	TMU_dispatcher(void);

/*
* Description: TMU_DeInit Danit TMU unite.
* @pram: void
* @return: ERROR_STATUS [negative number describe module and type of errors]
*/
ERROR_STATUS	TMU_DeInit(void);


#endif /* TMU_H_ */