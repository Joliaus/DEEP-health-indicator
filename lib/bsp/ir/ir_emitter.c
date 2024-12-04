/*
 * ir.c
 *
 *  Created on: 25 mars 2020
 *      Author: Nirgal
 */

#include "config.h"
#if USE_IR_EMITTER

#include "stm32f1xx_hal.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"

#include "ir_emmiter.h"
#include "stm32f1_pwm.h"

#define pwm_off() 	PWM_set_duty(IR_EMITTER_TIMER, IR_EMITTER_CHANNEL, 0)
#define pwm_on() 	PWM_set_duty(IR_EMITTER_TIMER, IR_EMITTER_CHANNEL, 50)

/*	Pour tester en bouchonnage sans LED IR ni TSOP32238, on peut utiliser directement :
#define pwm_off() 	PWM_set_duty(IR_EMITTER_TIMER, IR_EMITTER_CHANNEL, 100)
#define pwm_on() 	PWM_set_duty(IR_EMITTER_TIMER, IR_EMITTER_CHANNEL, 0)
*/

volatile uint8_t code = 0;
static void EMITTER_process_ms(void);

void EMITTER_init(void)
{
	PWM_run(IR_EMITTER_TIMER,IR_EMITTER_CHANNEL, FALSE, 27, 0, FALSE);	//27us pour f = 37kHz

	//On ajoute la fonction process_ms à la liste des fonctions appelées automatiquement chaque ms par la routine d'interruption du périphérique SYSTICK
	Systick_add_callback_function(&EMITTER_process_ms);
}

/*
 * Nouveau code à envoyer !
 */
void EMITTER_event(code_e newcode)
{
	code = newcode;
}


void EMITTER_demo(void)
{
	EMITTER_init();
	while(1)
	{
		EMITTER_event(CODE_EXAMPLE);
		HAL_Delay(1000);
		EMITTER_event(CODE_OTHER_EXAMPLE);
		HAL_Delay(1000);
	}
}

///////////////////////////////////////////////////////////////////////////////////////

/*
 * cette fonction doit être appelée strictement toutes les ms... !
 */
static void EMITTER_process_ms(void)
{
	typedef enum
	{
		INIT,
		WAIT_CODE,
		SEND_CODE,
		LAST_BIT,
	}state_e;

	static state_e state = INIT;
	static uint8_t index = 0;
	static code_e current_code = 0;
	switch (state)
	{
		case INIT:
			state = WAIT_CODE;
			break;
		case WAIT_CODE:
			if (code != 0){
				current_code = code;	//on récupère le code en local...
				code = 0;				//on acquitte le code
				pwm_on();
				state = SEND_CODE;
				index = 0;
			}
			break;
		case SEND_CODE:
			if(current_code & (1<<index))	//concentrez-vous sur cette ligne !
				pwm_on();
			else
				pwm_off();
			index++;
			if(index == 8){
				state = LAST_BIT;
				current_code = 0;
			}
			break;
		case LAST_BIT:
			pwm_off();
			state= WAIT_CODE;
			break;
	}
}





#endif
