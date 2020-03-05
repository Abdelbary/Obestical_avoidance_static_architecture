/*
 * cmmon_macros.h
 *
 * Created: 2/17/2020 2:20:28 PM
 *  Author: mahmo
 */ 


#ifndef CMMON_MACROS_H_
#define CMMON_MACROS_H_
#include "std_types.h"

#define FCPU	  16000000UL
#define  ZERO     0
#define	 ONE	  1
#define SET_BIT(number,bit)			((number)|=((1)<<(bit)))
#define CLEAR_BIT(number,bit)		((number)&=(~((1)<<(bit))))
#define TOGGLE(number,bit)			((number) ^ ((1)<<(bit)))
#define SET_MASK(number,Mask)       ((number)|=(Mask))
#define CLEAR_MASK(number,Mask)     ((number)&=(~(Mask)))
#define TOGGLE_MASK(number,Mask)    ((number) ^= (Mask))
#define BIT_IS_SET(number,bit)  ((number)&(1<<(bit)))
#define BIT_IS_CLEARD(number,bit)   (!((number)&(1<<(bit))))
#define MASK_IS_SET(number,mask)	((number)&(mask))


#endif /* CMMON_MACROS_H_ */