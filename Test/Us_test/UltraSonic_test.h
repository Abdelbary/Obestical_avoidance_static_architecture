/*
 * UltraSonic_test.h
 *
 * Created: 2/22/2020 5:18:51 PM
 *  Author: mahmo
 */ 


#ifndef ULTRASONIC_TEST_H_
#define ULTRASONIC_TEST_H_

#include <stdio.h>
#include "../../common_macros.h"
#include "../../std_types.h"
#include "../../HAL/UltraSonic/ultraSonic.h"


extern void US_Init_test(void);
extern void Us_Trigger_test(void);
extern void Us_Module_Test(void);
extern void Us_GetDistance_Test(void);
#endif /* ULTRASONIC_TEST_H_ */