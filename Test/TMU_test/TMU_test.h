/*
 * TMU_test.h
 *
 * Created: 2/25/2020 3:23:51 PM
 *  Author: mahmo
 */ 


#ifndef TMU_TEST_H_
#define TMU_TEST_H_

ERROR_STATUS    TMU_Init_test(gstr_TMU_cfg_t * tmu_cfg);
ERROR_STATUS	TMU_start_test(uint8_t Id,void (*callB_fun_ptr)(void),uint8_t lap_time,uint8_t type);
ERROR_STATUS	TMU_Stop_test(uint8_t Id);
ERROR_STATUS	TMU_dispatcher_test(void);
ERROR_STATUS	TMU_DeInit_test(void);



#endif /* TMU_TEST_H_ */