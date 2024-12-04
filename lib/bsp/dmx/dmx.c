#include "stm32f1_gpio.h"
#include "dmx.h"
#include "stm32f1_uart.h"
#include "systick.h"

#if 0
//code non nettoyé, commenté, ....


uint16_t calculAdresse(void){ // Calcul de l'adresse
	uint16_t address;
	address = 0;
	//address |= (uint16_t)(HAL_GPIO_ReadPin(DMX0));
	address |= (uint16_t)(HAL_GPIO_ReadPin(DMX1)) << (uint16_t)(1);
	address |= (uint16_t)(HAL_GPIO_ReadPin(DMX2)) << (uint16_t)(2);
	address |= (uint16_t)(HAL_GPIO_ReadPin(DMX3)) << (uint16_t)(3);
	address |= (uint16_t)(HAL_GPIO_ReadPin(DMX4)) << (uint16_t)(4);
	address |= (uint16_t)(HAL_GPIO_ReadPin(DMX5)) << (uint16_t)(5);
	address |= (uint16_t)(HAL_GPIO_ReadPin(DMX6)) << (uint16_t)(6);
	address |= (uint16_t)(HAL_GPIO_ReadPin(DMX7)) << (uint16_t)(7);
	address |= (uint16_t)(HAL_GPIO_ReadPin(DMX8)) << (uint16_t)(8);
	//address = MAX(address, 1);
	return address;
}



static void get_dmx_datas_from_tab(uint8_t * tableau, dmx_datas_t * dmx_datas){ // Affectation des données à la structure
	uint16_t adresse; //adresse de la machine
	adresse = calculAdresse();

	dmx_datas->red = tableau[adresse+3];
	dmx_datas->green = tableau[adresse+4];
	dmx_datas->blue = tableau[adresse+5];
	dmx_datas->strob = tableau[adresse+6];
	dmx_datas->dimmer = tableau[adresse+7];
	dmx_datas->mode = tableau[adresse+8];
}



volatile uint32_t truc = 0;
static void process_ms(void){ // Calcul du temps sans données
	if(truc)
		truc--;
}


bool_e dmx_receive_state_machine(dmx_datas_t * dmx_datas){ // Machine à état pour recevoir les données
	BSP_GPIO_PinCfg(SORTIE, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);
	typedef enum{
		INIT = 0,
		WAIT_DATAS,
		RECEIVE_BYTES,
		COMPUTE
	}state_e;

	static state_e state = INIT;
	static uint16_t index = 0;
	static uint8_t datas[DMX_DATA_SIZE];
	bool_e ret;
	ret = FALSE;

	switch(state){
		case INIT:
			UART_init(UART_DMX,250000);
			Systick_add_callback_function(&process_ms);
			BSP_GPIO_PinCfg(DMX0, GPIO_MODE_INPUT,GPIO_PULLDOWN,GPIO_SPEED_FREQ_HIGH);
			BSP_GPIO_PinCfg(DMX1, GPIO_MODE_INPUT,GPIO_PULLDOWN,GPIO_SPEED_FREQ_HIGH);
			BSP_GPIO_PinCfg(DMX2, GPIO_MODE_INPUT,GPIO_PULLDOWN,GPIO_SPEED_FREQ_HIGH);
			BSP_GPIO_PinCfg(DMX3, GPIO_MODE_INPUT,GPIO_PULLDOWN,GPIO_SPEED_FREQ_HIGH);
			BSP_GPIO_PinCfg(DMX4, GPIO_MODE_INPUT,GPIO_PULLDOWN,GPIO_SPEED_FREQ_HIGH);
			BSP_GPIO_PinCfg(DMX5, GPIO_MODE_INPUT,GPIO_PULLDOWN,GPIO_SPEED_FREQ_HIGH);
			BSP_GPIO_PinCfg(DMX6, GPIO_MODE_INPUT,GPIO_PULLDOWN,GPIO_SPEED_FREQ_HIGH);
			BSP_GPIO_PinCfg(DMX7, GPIO_MODE_INPUT,GPIO_PULLDOWN,GPIO_SPEED_FREQ_HIGH);
			BSP_GPIO_PinCfg(DMX8, GPIO_MODE_INPUT,GPIO_PULLDOWN,GPIO_SPEED_FREQ_HIGH);
			state = WAIT_DATAS;
			break;


		case WAIT_DATAS:
			if(UART_data_ready(UART_DMX)){
				state = RECEIVE_BYTES;
				index = 0;
				//HAL_GPIO_WritePin(SORTIE, 0);
			}
			break;


		case RECEIVE_BYTES:
			if(UART_data_ready(UART_DMX)){
				if(index<DMX_DATA_SIZE)
				{
					datas[index] = UART_get_next_byte(UART_DMX);
					index++;
				}
				truc = MIN_DURATION_BETWEEN_FRAMES;
			}

			if(index == DMX_DATA_SIZE){
				state = COMPUTE;
			}
			else{
				if(!truc)
					state = COMPUTE;
			}
			break;


		case COMPUTE:
			get_dmx_datas_from_tab(datas, dmx_datas);
			ret = TRUE;
			state = WAIT_DATAS;
			HAL_GPIO_WritePin(SORTIE, 1);
			break;


		default:
			break;
	}

	return ret;
}
#endif

