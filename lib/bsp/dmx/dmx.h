#ifndef DMX_H_
#define DMX_H_
#include "macro_types.h"

//switch
#define DMX8 GPIOB, GPIO_PIN_12
#define DMX7 GPIOB, GPIO_PIN_13
#define DMX6 GPIOB, GPIO_PIN_14
#define DMX5 GPIOB, GPIO_PIN_15
#define DMX4 GPIOA, GPIO_PIN_8
#define DMX3 GPIOA, GPIO_PIN_9
#define DMX2 GPIOA, GPIO_PIN_10
#define DMX1 GPIOA, GPIO_PIN_11
#define DMX0 GPIOB, GPIO_PIN_6

// lecture dmx
#define UART_DMX			UART2_ID
#define DMX_DATA_SIZE		8
#define MIN_DURATION_BETWEEN_FRAMES	10	//ms
#define SORTIE GPIOA, GPIO_PIN_1



typedef struct
{
	uint8_t mode; //canal 1
	uint8_t red; //canal 2
	uint8_t green; //canal 3
	uint8_t blue; // canal 4
	uint8_t strob; // canal 5
	uint8_t dimmer; //canal 6
}dmx_datas_t;


bool_e dmx_receive_state_machine(dmx_datas_t * dmx_datas);


#endif /* DMX_H_ */
