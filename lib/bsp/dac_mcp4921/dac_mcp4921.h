/*
 * stm32f1.h
 *
 *  Created on: 26 nov. 2019
 *      Author: chupinal
 */

#ifndef STM32F1_H_
#define STM32F1_H_
#include "config.h"

#if USE_DAC_MCP4921
#include <stdint.h>


#ifndef DAC_CS_PIN
	#define DAC_CS_PIN			GPIOA, GPIO_PIN_4
#endif

#ifndef DAC_SPI
	#define DAC_SPI SPI1
#endif


//Bits de configuration du DAC
#define DAC_BIT_nA		0x8000
#define DAC_BIT_BUF		0x4000
#define DAC_BIT_nGA		0x2000
#define DAC_BIT_nSHDN	0x1000
#define DAC_MASK_DATAS	0x0FFF

typedef enum
{
	DAC_GAIN_AMPLIFIER_X2 = 0,				//permet d'amplifier par 2 la sortie
	DAC_GAIN_AMPLIFIER_X1 = DAC_BIT_nGA
}dac_gain_amplifier_e;

typedef enum
{
	DAC_UNBUFFERED,							//mode par défaut (Vref a une impédance de 165k, 7pF)
	DAC_BUFFERED = DAC_BIT_BUF,				//mode permettant une plus haute impédance d'entrée sur Vref
}dac_buffer_e;

typedef enum
{
	DAC_SHUTDOWN,							//le DAC est en veille
	DAC_RUN = DAC_BIT_nSHDN					//le DAC fonctionne normalement
}dac_shutdown_e;

//Fonction d'initialisation du DAC.
void DAC_init(void);

//Fonction simple pour demander au DAC de sortir une valeur...
void DAC_send(uint16_t data12);

//Fonction avancée, permettant d'accéder au gain, à la config du buffer de l'entrée Vref, et au bit Shutdown
void DAC_advanced_send(uint16_t data12, dac_gain_amplifier_e gain_amplifier, dac_buffer_e buffer, dac_shutdown_e shutdown);

//Fonction permettant de mettre le DAC en veille
void DAC_shutdown(void);


//Fonction blocante...
void DAC_demo(void);



#endif

#endif /* STM32F1_H_ */
