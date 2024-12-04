/*
 * ir_receiver_nec.c
 *
 *  Created on: 09 june 2020
 *      Author: Nirgal
 */

#include "config.h"
#if USE_IR_RECEIVER_NEC


#warning "code non testé pour une télécommande IR en code NEC."
//https://www.sbprojects.net/knowledge/ir/nec.php
//https://techdocs.altium.com/display/FPGA/NEC+Infrared+Transmission+Protocol
/*
 * Ce module est conçu pour utiliser le composant TSOP 32238
 *
 */
#include "stm32f1xx_hal.h"
#include "stm32f1_gpio.h"
#include "stm32f1_timer.h"
#include "macro_types.h"
#include "systick.h"

#include "ir_emmiter.h"
#include "ir_receiver.h"

#include "stm32f1_extit.h"

#define PRECISION_US	50


typedef enum
{
	EVENT_UP,
	EVENT_DOWN,
	EVENT_TIMEOUT
}event_e;


static void RECEIVER_NEC_process_extit(void);
static void RECEIVER_NEC_state_machine(event_e event, uint32_t up_duration, uint32_t down_duration);
static volatile uint32_t overflow_index = 0;
static volatile uint64_t received_full_code = 0;

void RECEIVER_NEC_init(void)
{
	//Broche capteur en entree, déclenchant des ITs externes.
	BSP_GPIO_PinCfg(IR_RECEIVER_GPIO, IR_RECEIVER_PIN, GPIO_MODE_IT_RISING_FALLING,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);
	EXTIT_set_callback(&RECEIVER_NEC_process_extit, EXTI_gpiopin_to_pin_number(IR_RECEIVER_PIN), TRUE);
	//Systick_add_callback_function(&RECEIVER_NEC_process_ms);
	TIMER_run_us(IR_RECEIVER_TIMER, 64000, TRUE);
}

void RECEIVER_NEC_demo(void)
{
	RECEIVER_init();
	printf("Infra-red Receiver initialized\n");
	while(1)
	{
		if(received_full_code)
		{
			//received_full_code = 0xCAFEDECA00112233;
			printf("Nouveau code reçu : %lx%lx\n", (uint32_t)(received_full_code>>32), (uint32_t)received_full_code);
			received_full_code = 0;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////


#define NB_BITS				52
static volatile int8_t index = -1;
static volatile uint64_t full_code = 0;



void RECEIVER_NEC_process_extit(void)
{
	static uint32_t last_time = 0;
	static uint32_t t_up = 0, t_down = 0;
	static uint32_t up_duration = 0, down_duration = 0;
	event_e event;
	uint32_t current_time;
	bool_e current_signal;

	//EXTIT_disable(EXTI_gpiopin_to_pin_number(IR_RECEIVER_PIN));
	current_signal = !HAL_GPIO_ReadPin(IR_RECEIVER_GPIO, IR_RECEIVER_PIN);
	current_time = 64000*overflow_index + TIMER_read(IR_RECEIVER_TIMER);


	if(current_signal)
	{
		event = EVENT_UP;
		t_up = current_time;
		up_duration = t_up - t_down;
	}
	else
	{
		event = EVENT_DOWN;
		t_down = current_time;
		down_duration = t_down - t_up;
	}
	RECEIVER_NEC_state_machine(event, up_duration, down_duration);

	index = -1;
	full_code = 0;

	last_time = current_time;
}

void RECEIVER_NEC_process_ms(void)
{




}


#define	RESET_THE_CODE()	index = 0; code = 0
#define ADD_BIT_ONE()		code |= (1UL<<(32UL-index-1UL)); index++
#define ADD_BIT_ZERO()		index++


static void RECEIVER_NEC_state_machine(event_e event, uint32_t up_duration, uint32_t down_duration)
{
	typedef enum
	{
		WAIT_9000US_UP,
		WAIT_4500US_DOWN,
		RECEIVE_DATAS_UP_STATE,
		RECEIVE_DATAS_DOWN_STATE
	}state_e;
	static uint32_t code;
	static uint8_t index;
	static state_e state = WAIT_9000US_UP;
	if(event == EVENT_DOWN && up_duration >8000)
	{
		TIMER_write(IR_RECEIVER_TIMER, 0);
		state = WAIT_4500US_DOWN;		//état forcé.
	}

	switch(state)
	{
		case  WAIT_9000US_UP:
			if(event == EVENT_DOWN && up_duration > 8000)
				state = WAIT_4500US_DOWN;

			break;
		case WAIT_4500US_DOWN:
			if(event == EVENT_UP && down_duration > 4000 && down_duration < 5000)
				state = RECEIVE_DATAS_UP_STATE;
			RESET_THE_CODE();
			break;
		case RECEIVE_DATAS_UP_STATE:
			if(event == EVENT_DOWN && absolute(up_duration - 562) < PRECISION_US)
				state = RECEIVE_DATAS_DOWN_STATE;
			break;
		case RECEIVE_DATAS_DOWN_STATE:
			if(event == EVENT_UP)
			{
				if(absolute(up_duration - 562) < PRECISION_US)
				{
					ADD_BIT_ONE();
				}
				else if(absolute(up_duration - 1687) < PRECISION_US)
				{
					ADD_BIT_ZERO();
				}
				else
				{
					ADD_BIT_ZERO();	//??:!!! c'est anormal...
				}
				if(index == 32)
				{
					state =  WAIT_9000US_UP;
					received_full_code = code;
				}
				else
					state = RECEIVE_DATAS_UP_STATE;
			}
			else if(event == EVENT_TIMEOUT)
				state = WAIT_9000US_UP;

			break;
	}


}



void TIMER_handler_it(void)
{
	overflow_index++;

}


#endif
