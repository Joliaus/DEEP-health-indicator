/*
 * MLX90614.c
 *
 *  Created on: 16 janv. 2016
 *      Author: Nirgal
 */

/**
 * Ce pilote permet d'interfacer le capteur de température à distance MLX90614.
 * Ce capteur permet la mesure d'une température ambiante ET d'une température de l'objet vu à distance.
 *
 * Comment utiliser ce pilote :
 *
 * Possibilité 1 : utiliser la fonction de démo : DEMO_MLX90614_statemachine(), qui doit être appelée en boucle.
 * Possibilité 2 :
 *    1- appeler MLX90614_init() une fois
 *    2- appeler quand vous le souhaitez les fonctions readObjectTempC() et readAmbiantTempC() qui renvoient les température ambiante et observées en degré Celsius.
 *
 * Par défaut, les broches utilisées sont celles de l'I2C1 : PB7 pour SDA et PB8 pour SCL.
 * Ces configurations sont modifiables dans la fonction MLX90614_init();
 */

/***************************************************
  Ce pilote est inspiré d'une librairie pour le capteur MLX90614 Temp Sensor dont les commentaires sont disponibles ci dessous.
  Designed specifically to work with the MLX90614 sensors in the
  adafruit shop
  ----> https://www.adafruit.com/products/1748
  ----> https://www.adafruit.com/products/1749
  These sensors use I2C to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/
#include "config.h"
#if USE_MLX90614
#include "MLX90614.h"
#include "stm32f1xx_hal.h"
#include "stm32f1_gpio.h"
#include "stm32f1_i2c.h"
#include "systick.h"
#include "tft_ili9341/stm32f1_ili9341.h"

 // I2C_HandleTypeDef hi2c;
  static volatile uint32_t t = 0;

  uint16_t read16(uint8_t a);

  static double temp = 0.0;

//Initialisation du capteur de température et de la vitesse d'horloge
void MLX90614_init(void)
{
	I2C_Init(MLX90614_I2C, 100000);
}


/*
 * Machine à états du capteur de température
 * Cette fonction doit être appelée dans la boucle de tâche de fond
 * Initialisation, lecture des températures ambiante et de l'objet
 */

running_e state_machine_MLX90614_mesure(bool_e ask_for_finish)
{
	typedef enum
	{
		INIT = 0,
		RUN,
		CLOSE
	}state_e;
	static state_e state = INIT;
	running_e ret = IN_PROGRESS;

	switch(state)
	{
		case INIT:
			MLX90614_init();
			Systick_add_callback_function(MLX90614_process_1ms);
			state = RUN;
			break;
		case RUN:
			if(ask_for_finish)
				state = CLOSE;
			if(!t)
			{
				double object;
				t = 500;
				object = readObjectTempC();
				if(object != temp){
				ILI9341_printf(220, 170, &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE, "%.1fC", object + 1);
					temp = object;
				}
			}
			break;
		case CLOSE:
			state = INIT;
			Systick_remove_callback_function(MLX90614_process_1ms);
			ret = END_OK;
			break;
	}

	return ret;
}

 running_e state_machine_MLX90614_resultat(bool_e ask_for_finish)
{
	typedef enum
	{
		INIT = 0,
		RUN,
		CLOSE
	}state_e;
	static state_e state = INIT;
	running_e ret = IN_PROGRESS;

	switch(state)
	{
		case INIT:
			MLX90614_init();
			Systick_add_callback_function(MLX90614_process_1ms);
			state = RUN;
			break;
		case RUN:
			if(ask_for_finish)
				state = CLOSE;
			if(!t)
			{
				double object;
				t = 500;
				object = readObjectTempC();
				if(object<35 || object>45){
					ILI9341_Fill(ILI9341_COLOR_WHITE);
					ILI9341_Puts(5,110, "Erreur ! Veuillez  reprendre la mesure", &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
								}
				else if(object>35 && object<38){
					ILI9341_Fill(ILI9341_COLOR_WHITE);
					ILI9341_Puts(5,110, "Vous etes en bonne sante", &Font_16x26, ILI9341_COLOR_GREEN, ILI9341_COLOR_WHITE);
				}
				else if(object>38){
					ILI9341_Fill(ILI9341_COLOR_WHITE);
					ILI9341_Puts(5,110, "Vous etes surement malade", &Font_16x26, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE);
					printf("Votre patient est sûrement malade, veuillez le contacter pour confirmer ! \n");
				}
			}
			break;
		case CLOSE:
			state = INIT;
			Systick_remove_callback_function(MLX90614_process_1ms);
			ret = END_OK;
			break;
	}

	return ret;
}

//Cette fonction doit être appelée toutes les ms.
void MLX90614_process_1ms(void)
{
	if(t)
		t--;
}

/*
 * Fonctions renvoyant les températures ambiantes et de l'objet
 * F pour Farenheit, C pour Celsius
 */
double readObjectTempF(void)
{
  return (readTemp(MLX90614_TOBJ1) * 9 / 5) + 32;
}


double readAmbientTempF(void)
{
  return (readTemp(MLX90614_TA) * 9 / 5) + 32;
}

double readObjectTempC(void)
{
  return readTemp(MLX90614_TOBJ1);
}


double readAmbientTempC(void)
{
  return readTemp(MLX90614_TA);
}

float readTemp(uint8_t reg)
{
  float temp;

  temp = read16(reg);
  temp *= .02;
  temp  -= 273.15;
  return temp;
}

/*********************************************************************/

uint16_t read16(uint8_t a)
{
  uint8_t ret[3];
  I2C_ReadMulti(MLX90614_I2C, MLX90614_I2CADDR<<1, a, ret, 3);
  return ret[0] | ((uint16_t)(ret[1])) << 8;
}
#endif
