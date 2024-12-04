/*
 * DS18B20.h
 *
 *  Created on: 3 mai 2021
 *      Author: Nirgal
 */

#ifndef BSP_DS18B20_H_
#define BSP_DS18B20_H_

#include "../appli/config.h"
#if USE_DS18B20

	int16_t DS18B20_get_temperature(void);

#endif
#endif /* BSP_DS18B20_H_ */
