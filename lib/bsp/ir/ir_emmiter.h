/*
 * ir_emmiter.h
 *
 *  Created on: 25 mars 2020
 *      Author: Nirgal
 */

#ifndef BSP_IR_IR_EMMITER_H_
#define BSP_IR_IR_EMMITER_H_

typedef enum
{
	CODE_NONE 			= 0x00,		//codes recommandés : alternances de 0 et de 1, donc caractères hexa 5, 6, 9, A
	CODE_EXAMPLE 		= 0xA5,
	CODE_OTHER_EXAMPLE 	= 0x69
}code_e;

#if USE_IR_EMITTER

	#include "stm32f1_timer.h"

	#ifndef IR_EMITTER_TIMER
		#define IR_EMITTER_TIMER	TIMER1_ID
	#endif

	#ifndef IR_EMITTER_CHANNEL
		#define IR_EMITTER_CHANNEL	TIM_CHANNEL_1
	#endif


	void EMITTER_init(void);

	void EMITTER_event(code_e newcode);

	void EMITTER_demo(void);

#endif //USE_IR_EMITTER

#endif /* BSP_IR_IR_EMMITER_H_ */
