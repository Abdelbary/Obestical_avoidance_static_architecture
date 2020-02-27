/*
 * TMU_test.h
 *
 * Created: 2/25/2020 3:23:51 PM
 *  Author: mahmo
 */ 


#ifndef TMU_TEST_H_
#define TMU_TEST_H_

#include <stdio.h>
#include "../../common_macros.h"
#include "../../std_types.h"
#include "../../HAL/UltraSonic/ultraSonic.h"
#include "..\..\ServiceLayer\TMU\TMU.h"


void    TMU_Init_test();
void	TMU_start_test(void);
void	TMU_Stop_test(void);
void	TMU_dispatcher_test(void);
void	TMU_DeInit_test(void);
void    TMU_Module_run_test(void);


#endif /* TMU_TEST_H_ */