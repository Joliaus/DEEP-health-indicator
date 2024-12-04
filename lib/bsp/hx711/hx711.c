/*
 * hx711.c
 *
 *  Created on: 4 févr. 2020
 *      Author: Nirgal
 */
#include "hx711.h"
#if USE_HX711

#include "stm32f1xx_hal.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"

#define NB_BITS	24

static bool_e initialized = FALSE;

#define sck(b)	HAL_GPIO_WritePin(HX711_PD_SCK_GPIO, HX711_PD_SCK_PIN,b)
#define dout()	HAL_GPIO_ReadPin(HX711_DOUT_GPIO, HX711_DOUT_PIN)

void HX711_power_on(void);
void HX711_power_off(void);
static void wait_200ns(void);

void HX711_init(void)
{
	BSP_GPIO_PinCfg(HX711_PD_SCK_GPIO, HX711_PD_SCK_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(HX711_DOUT_GPIO, HX711_DOUT_PIN, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);
	HX711_power_on();
	initialized = TRUE;
}



running_e HX711_get_datas(next_conversion_mode_e next_conversion_mode, uint32_t * value)
{

	running_e ret = END_ERROR;
	uint8_t i;
	uint32_t v;

	uint8_t timeout;

	if(!initialized)
		HX711_init();

	//Wait until Dout at 0
	timeout = 100;
	do{
		if(dout() == 0)
			break;
		Delay_us(1);
	}while(timeout--);

	wait_200ns();	//Wait with Dout at 0 during at least 100ns.

	if(!timeout)
	{
		ret = END_TIMEOUT;
		return ret;
	}
	else	//le composant est présent car le timeout n'est pas écoulé !
	{
		v = 0;
		__disable_irq();
			//section critique. (délai max à l'état haut de sck : 50us !)
			for(i=0;i<NB_BITS; i++)
			{
				sck(1);
				wait_200ns();
				v |= (uint32_t)(dout()) << (23-i);
				sck(0);
				wait_200ns();
			}

			//Pulses supplémentaires...
			for(;i<next_conversion_mode; i++)
			{
				sck(1);
				wait_200ns();
				sck(0);
				wait_200ns();
			}
		__enable_irq();
		*value = v;
		ret = END_OK;
	}
	return ret;
}


void HX711_power_off(void)
{
	sck(1);
	Delay_us(50);
}

void HX711_power_on(void)
{
	sck(0);
}


static void wait_200ns(void)
{
	return;
}

#endif
