/*
 * vl53l0_demo.h
 *
 *  Created on: 7 mai 2020
 *      Author: Nirgal
 */

#ifndef BSP_VL53L0X_VL53L0_DEMO_H_
#define BSP_VL53L0X_VL53L0_DEMO_H_

#include "config.h"
#if USE_VL53L0

	#include "macro_types.h"

	typedef enum
	{
		TIMESLOT_ASK_BEGIN = 0,
		TIMESLOT_ASK_END = VL53_NB-1,
		TIMESLOT_WAIT_TIME_DURING_MEASUREMENTS = 21,
		//TIMESLOT_GET_BEGIN = 21,
		//TIMESLOT_GET_END = 21+VL53_NB-1,
		TIMESLOT_NB
	}timeslot_e;


	void VL53L0X_Demo(void);

	bool_e VL53L0X_init(void);

	timeslot_e VL53L0X_process_1ms(void);

	void VL53L0X_process_main(void);


#endif


#endif /* BSP_VL53L0X_VL53L0_DEMO_H_ */
