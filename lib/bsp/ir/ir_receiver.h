/*
 * ir_emmiter.h
 *
 *  Created on: 25 mars 2020
 *      Author: Nirgal
 */

#ifndef BSP_IR_IR_RECEIVER_H_
#define BSP_IR_IR_RECEIVER_H_

#include "config.h"
#if USE_IR_RECEIVER

	#ifndef IR_RECEIVER_GPIO
		#define IR_RECEIVER_GPIO	GPIOB
	#endif

	#ifndef IR_RECEIVER_PIN
		#define IR_RECEIVER_PIN		GPIO_PIN_0
	#endif

	#ifndef IR_RECEIVER_TIMER
		#define IR_RECEIVER_TIMER	TIMER1_ID
		#define TIMER_handler_it	TIMER1_user_handler_it
	#else
		#ifndef TIMER_handler_it
			#error "Si vous avez défini un IR_RECEIVER_TIMER, vous devez définir TIMER_handler_it également !"
		#endif
	#endif


#endif //USE_IR_RECEIVER

#endif /* BSP_IR_IR_RECEIVER_H_ */
