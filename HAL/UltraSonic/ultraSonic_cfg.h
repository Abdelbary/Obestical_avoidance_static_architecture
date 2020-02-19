/*
 * ultraSonic_cfg.h
 *
 * Created: 2/18/2020 3:15:17 PM
 *  Author: mahmo
 */ 
#include "../../MCAL/registers.h"

#ifndef ULTRASONIC_CFG_H_
#define ULTRASONIC_CFG_H_

#define ICU_TRIGGER_PORT GPIOD
#define ICU_TRIGGER_PIN  BIT0

#define ICU_EKKO_PORT    GPIOB
#define ICU_EKKO_PIN	 BIT2

#endif /* ULTRASONIC_CFG_H_ */