/*
 * stm32f1_adc.c
 *
 *  Created on: 26 nov. 2019
 *      Author: chupinal
 *  Modified on: 13 mar. 2020
 *  	Author: nirgal
 */
#include "config.h"

#if USE_DAC_MCP4921
#include "dac_mcp4921.h"
#include "macro_types.h"
#include "stm32f1_spi.h"
#include "stm32f1_gpio.h"

static bool_e initialized = FALSE;

//Initialisation du DAC
void DAC_init(void)
{
	SPI_Init(DAC_SPI);
	BSP_GPIO_PinCfg(DAC_CS_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);
	initialized = TRUE;
}

//Fonction blocante...
void DAC_demo(void)
{
	DAC_init();
	uint16_t v = 0;
	int16_t way = 1;
	while(1)
	{
		HAL_Delay(1);
		DAC_send(v);
		v += way;
		if(v==0 || v==4095)
			way *= -1;
	}
}

//Permet d'envoyer le signal numérique
//sur le DAC
__INLINE void DAC_send(uint16_t data12)
{
	DAC_advanced_send(data12, DAC_GAIN_AMPLIFIER_X2, DAC_UNBUFFERED, DAC_RUN);
}

__INLINE void DAC_shutdown(void)
{
	DAC_advanced_send(0,DAC_GAIN_AMPLIFIER_X2,DAC_UNBUFFERED, DAC_RUN);
}

void DAC_advanced_send(uint16_t data12, dac_gain_amplifier_e gain_amplifier, dac_buffer_e buffer, dac_shutdown_e shutdown)
{
	if(!initialized)
		DAC_init();

	//CS low
	HAL_GPIO_WritePin(DAC_CS_PIN, 0);

	data12 &= DAC_MASK_DATAS;	//on tue les 4 bits de poids fort.
	data12 |= ((uint16_t)(shutdown)) | ((uint16_t)gain_amplifier) | ((uint16_t)buffer);
	uint8_t datas[2];
	datas[0] = HIGHINT(data12);
	datas[1] = LOWINT(data12);

	SPI_WriteMultiNoRegister(DAC_SPI, datas, 2);

	//CS high
	HAL_GPIO_WritePin(DAC_CS_PIN, 1);
}



#endif
