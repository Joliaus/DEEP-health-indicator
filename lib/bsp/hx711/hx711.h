/*
 * hx711.h
 *
 *  Created on: 4 févr. 2020
 *      Author: Nirgal
 */

#ifndef BSP_HX711_HX711_H_
#define BSP_HX711_HX711_H_
#include "config.h"
#if USE_HX711

#include <stdint.h>
#include "macro_types.h"
typedef enum
{
	NEXT_CONVERSION_CHANNEL_A_GAIN_128 = 25,
	NEXT_CONVERSION_CHANNEL_B_GAIN_32 = 26,
	NEXT_CONVERSION_CHANNEL_B_GAIN_64 = 27
}next_conversion_mode_e;

#ifndef HX711_PD_SCK_GPIO
#define HX711_PD_SCK_GPIO	GPIOB
#endif

#ifndef HX711_PD_SCK_PIN
#define HX711_PD_SCK_PIN	GPIO_PIN_0
#endif

#ifndef HX711_DOUT_GPIO
#define HX711_DOUT_GPIO		GPIOB
#endif

#ifndef HX711_DOUT_PIN
#define HX711_DOUT_PIN		GPIO_PIN_1
#endif

running_e HX711_get_datas(next_conversion_mode_e next_conversion_mode, uint32_t * value);

#endif
#endif /* BSP_HX711_HX711_H_ */
