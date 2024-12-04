/**
  ******************************************************************************
  * @file    main.c
  * @author  Nirgal
  * @date    03-July-2019
  * @brief   Default main function.
  ******************************************************************************
*/
#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"
#include "tft_ili9341/stm32f1_ili9341.h"
#include "MAX30102/MAX30102.h"


void writeLED(bool_e b)
{
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, b);
}

bool_e readButton(void)
{
	return !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
}

static volatile uint32_t t = 0;
void process_ms(void)
{
	if(t)
		t--;
}


int main(void)
{
	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la premi�re �tape de la fonction main().
  	HAL_Init();

	//Initialisation de l'UART2 � la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
		//Attention, les pins PA2 et PA3 ne sont pas reli�es jusqu'au connecteur de la Nucleo.
		//Ces broches sont redirig�es vers la sonde de d�bogage, la liaison UART �tant ensuite encapsul�e sur l'USB vers le PC de d�veloppement.
	UART_init(UART2_ID,115200);

	//"Indique que les printf sortent vers le p�riph�rique UART2."
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	//Initialisation du port de la led Verte (carte Nucleo)
	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);

	//Initialisation du port du bouton bleu (carte Nucleo)
	BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

	//On ajoute la fonction process_ms � la liste des fonctions appel�es automatiquement chaque ms par la routine d'interruption du p�riph�rique SYSTICK
	Systick_add_callback_function(&process_ms);

	while(1)
		{
		LED_MATRIX();
		main_state_machine();

		if(!t)
			{
			t = 200;
			HAL_GPIO_TogglePin(LED_GREEN_GPIO, LED_GREEN_PIN);
		}
	}

}

void main_state_machine(void)
{
	typedef enum
	{
		INIT = 0,
		ECRAN_PRINCIPAL,
		MESURE,
		RESULTAT
	}state_e;

	static state_e state = INIT;
	static state_e previous_state = INIT;
	bool_e entrance;
	entrance = (state!=previous_state)?TRUE:FALSE;
	previous_state = state;

	//bool_e current_button;
	//current_button =!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);

	switch(state)
	{
		case INIT:
			ILI9341_Init();
			state_machine_MLX90614_mesure();
			state_machine_MLX90614_resultat();
			state = ECRAN_PRINCIPAL;
			break;

		case ECRAN_PRINCIPAL:
			if(entrance){
				t=5000;
				ILI9341_Fill(ILI9341_COLOR_WHITE);
				ILI9341_Puts(5,80, "INDICATEUR DE SANTE", &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				ILI9341_Puts(5,120, "Veuillez placer le capteur a 1 cm de votre front pendant 10s", &Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE);
			}
			if(!t)
				state = MESURE;
			break;

		case MESURE:
			if(entrance){
				t=5000;
				ILI9341_Fill(ILI9341_COLOR_WHITE);
				ILI9341_Puts(5, 10, "INDICATEUR DE SANTE", &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				ILI9341_DrawLine(0, 50, 320, 50, ILI9341_COLOR_BLACK);
				ILI9341_Puts(5, 100, "BPM : ", &Font_16x26, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE);
				ILI9341_printf(5, 170, &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE, "Temperature :");

				state_machine_MLX90614_mesure();
				state_machine_MLX90614_mesure();
				state_machine_MLX90614_mesure();

			}
			if(!t)
				state = RESULTAT;
			break;

		case RESULTAT:
			if(entrance){
				t=10000;
				state_machine_MLX90614_resultat();
			}
			if(!t)
				state = ECRAN_PRINCIPAL;
			break;

		default:
			break;
	}
}


