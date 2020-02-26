/*
 * TMU_test.c
 *
 * Created: 2/25/2020 3:23:35 PM
 *  Author: mahmo
 */ 
volatile uint16_t timer0_MS_flag;

#ifndef GCC
/*declare fun as weak to avoid multiple definition by linker*/
ERROR_STATUS    Timer_Init(Timer_cfg_s* Timer_cfg)__attribute__((weak)) ; 
ERROR_STATUS	Timer_Start(uint8_t Timer_CH_NO, uint16_t Timer_Count)__attribute__((weak));
ERROR_STATUS	Timer_DeInit(uint8_t timer_Ch)__attribute__((weak));
#endif


ERROR_STATUS    TMU_Init_test(gstr_TMU_cfg_t * tmu_cfg);
ERROR_STATUS	TMU_start_test(uint8_t Id,void (*callB_fun_ptr)(void),uint8_t lap_time,uint8_t type);
ERROR_STATUS	TMU_Stop_test(uint8_t Id);
ERROR_STATUS	TMU_dispatcher_test(void);
ERROR_STATUS	TMU_DeInit_test(void);