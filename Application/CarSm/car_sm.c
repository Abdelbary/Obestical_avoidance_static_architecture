/*
 * car_sm.c
 *
 * Created: 2/18/2020 10:05:48 PM
 *  Author: mahmo
 */ 

/*- INCLUDES ----------------------------------------------*/
#include "car_sm.h"

/*- LOCAL MACROS ------------------------------------------*/
#define STEERING_SPEED 30
#define DELAY_HUNDRED  100

/*- LOCAL Datatypes ---------------------------------------*/
typedef enum En_car_state_t{
	state_move_backword=0,state_turn=1,state_move_forward=2
}En_car_state_t;


typedef enum En_distant_ranges_t{
	distent_0 = 0,distent_25=25,distent_30 = 30
}En_distant_ranges_t;

/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
/*- GLOBAL STATIC VARIABLES -------------------------------*/

static En_car_state_t genu_car_state;


/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/
/*- APIs IMPLEMENTATION -----------------------------------*/





ERROR_STATUS Car_SM_Init(void)
{
	/*
	*	1-initiate steering
	*	2-initiate	ultraSonic 
	*/
	sint16_t fun_status = OK;
	
	fun_status = Steering_Init();
	fun_status = Us_Init();
	return fun_status;
}


ERROR_STATUS Car_SM_Update(void)
{
	uint8_t fun_status = OK;
	
	/*
	*	1-send trigger
	*	2-git ultrasonic dissent
	*	3-check for distant value to determine state machine state
	*	4-switch for state and excite crossponding actions
	*/
		uint64_t distant;
		Us_Trigger();
		Us_GetDistance(&distant);
		
		if(distant >= distent_0 && distant <= distent_25)
		{
			genu_car_state = state_move_backword;
		}
		else if(distant >distent_25 && distant <= distent_30)
		{
			genu_car_state = state_turn;
		}
		else 
		{
			genu_car_state = state_move_forward;		
		}
			
		/*switch for state machine state to determine taken action*/	
		switch(genu_car_state)
		{
			case state_move_backword:
				Steering_SteerCar(CAR_BACKWARD,STEERING_SPEED);
			break;
			case  state_turn:
				Steering_SteerCar(CAR_RIGHT,STEERING_SPEED);
			break;
			case state_move_forward:
				Steering_SteerCar(CAR_FORWARD,STEERING_SPEED);
			break;
		}
	/*delay to take 10 samples per seconed*/
	_delay_ms(DELAY_HUNDRED);
	return fun_status;
}