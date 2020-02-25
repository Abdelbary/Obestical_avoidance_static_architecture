/*
 * SystemErrors.h
 *
 * Created: 2/25/2020 10:35:32 AM
 *  Author: mahmo
 */ 


#ifndef SYSTEMERRORS_H_
#define SYSTEMERRORS_H_
#include "..\..\common_macros.h"

#define OK							     0
#define	NULL_PTR_ERROR					-1
#define	MODULE_NOT_INITALIZED			-2
#define	MULTIPLE_INITALIZATION			-3
#define	STOP_WITHOUT_START				-4
#define	INVALAD_PARAMETER				-5
#define	MULTIPLE_START					-6
#define	FULL_BUFFER						-7

#define NOK								-20
#define ERROR_BUFFER_SIZE				500


void error_handler(sint16_t error_ID);

#endif /* SYSTEMERRORS_H_ */