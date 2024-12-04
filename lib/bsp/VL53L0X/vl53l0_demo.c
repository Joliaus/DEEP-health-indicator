/*
 * vl53l0_demo.c
 *
 *  Created on: 30 avr. 2020
 *      Author: Nirgal
 */


/*
 * VL53_becon.h
 *
 *  Created on: 3 nov. 2019
 *      Author: Guillaume D. / Nirgal
 */


#include "config.h"
#if USE_VL53L0
#include "vl53l0_demo.h"
#include "stm32f1xx_hal.h"
#include "vl53l0x_api.h"
#include "macro_types.h"
#include "systick.h"
#include "stm32f1_i2c.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "stm32f1_timer.h"

#define VL53L0_DISTANCEMODE_SHORT 1
#define VL53L0_DISTANCEMODE_LONG 2

#define VL53_RESET_STATE	0
#define VL53_ON_STATE		1


typedef enum{
	INIT,
	SENSOR_INIT,
	SET_ADDRESS,
	ASK_AND_GET_DATA,
	FAIL
}state_vl53_e;




typedef enum {
	LONG_RANGE 		= 0, /*!< Long range mode */
	HIGH_SPEED 		= 1, /*!< High speed mode */
	HIGH_ACCURACY	= 2, /*!< High accuracy mode */
} RangingConfig_e;

void VL53_Setup(VL53L0X_DEV Dev, RangingConfig_e rangingConfig, VL53L0X_DeviceModes deviceMode);

typedef struct{
	GPIO_TypeDef * GPIO_Port;
	uint16_t GPIO_Pin;
}VL53L0X_config;


/*
 * @brief structure d'un capteur VL53L0X
 */
typedef struct{
	bool_e enable;
	Uint16 data_ready;
	Uint16 distance;
	VL53L0X_RangeStatus rangeStatus;
	VL53L0X_Dev_t  dev;
	VL53L0X_RangingMeasurementData_t measurementDatas;
	state_vl53_e state;
	bool_e asked;
}BEACON_data_sensor;

#if VL53_NB>8
#error "la version actuelle du driver ne peut traiter que 8 capteurs au maximum"
#endif
/*
 * @brief Tableau contenant toutes les info des capteur d'une carte sensor_expander
 */
static BEACON_data_sensor sensors[VL53_NB];
static volatile bool_e flag_send_can_msg = FALSE;

static const VL53L0X_config sensor_reset_pins[VL53_NB] = {
#ifdef VL_0
		(VL53L0X_config){VL_0}
#endif
#ifdef VL_1
		,(VL53L0X_config){VL_1}
#endif
#ifdef VL_2
		,(VL53L0X_config){VL_2}
#endif
#ifdef VL_3
		,(VL53L0X_config){VL_3}
#endif
#ifdef VL_4
		,(VL53L0X_config){VL_4}
#endif
#ifdef VL_5
		,(VL53L0X_config){VL_4}
#endif
#ifdef VL_6
		,(VL53L0X_config){VL_4}
#endif
#ifdef VL_7
		,(VL53L0X_config){VL_4}
#endif
};

void VL53L0X_Demo(void)
{
	VL53L0X_init();
	TIMER_run_us(TIMER1_ID, 1000, TRUE);
	while(1)
	{
		VL53L0X_process_main();
	}
}

void TIMER1_user_handler_it(void)
{
	VL53L0X_process_1ms();
}

bool_e VL53L0X_init(void){
	for (uint16_t id = 0; id < VL53_NB; id++)
	{
		sensors[id].state = INIT;
		BSP_GPIO_PinCfg(sensor_reset_pins[id].GPIO_Port, sensor_reset_pins[id].GPIO_Pin,GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);
		HAL_GPIO_WritePin(sensor_reset_pins[id].GPIO_Port, sensor_reset_pins[id].GPIO_Pin, VL53_RESET_STATE);
	}
	I2C_Init(VL53L0X_I2C, 400000);		//TODO 400000

	return TRUE;
}


/*
 * Cette fonction doit être appelée toutes les ms.
 * MAIS PAS par le systick, car elle doit pouvoir être interrompue par l'it systick !
 */
timeslot_e VL53L0X_process_1ms(void){
	static VL53L0X_Error erro = VL53L0X_ERROR_NONE;
	static uint8_t timeslot = 0;
	static bool_e flag_one_sensor_is_on_default_address = FALSE;
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, 1);
	for (uint16_t id = 0;  id < VL53_NB; id++)
	{
		switch (sensors[id].state)
		{
			case INIT:
				if(timeslot == (timeslot_e)id)	//Si le time_slot correspond à l'id parcouru.
				{
					sensors[id].enable = FALSE;
					if(flag_one_sensor_is_on_default_address == FALSE)
					{

						//RESET
						HAL_GPIO_WritePin(sensor_reset_pins[id].GPIO_Port ,sensor_reset_pins[id].GPIO_Pin, VL53_RESET_STATE);

						//delay 100us
						Delay_us(100);

						//SET
						HAL_GPIO_WritePin(sensor_reset_pins[id].GPIO_Port ,sensor_reset_pins[id].GPIO_Pin, VL53_ON_STATE);

						flag_one_sensor_is_on_default_address = TRUE;

						// ajout de l'i2c vers le capteur
						sensors[id].dev.I2cHandle = I2C_get_handle(VL53L0X_I2C);
						sensors[id].dev.I2cDevAddr = 0x52; //vl53L0X embedded address

						//sensors[id].time_slot.get = (timeslot_e)id + TIMESLOT_GET_BEGIN;
						sensors[id].distance = 0;
						sensors[id].data_ready = 0;

						sensors[id].state = SET_ADDRESS;
					}
				}
				break;


			case SET_ADDRESS:	// premier contact avec le capteur + changement de son adresse
				//2 timeslots après avoir lâché le reset, on peut causer au capteur
				//	(il emprunte exceptionennellement le timeslot de qn d'autre !)
				if(timeslot == ((timeslot_e)id + 2)%TIMESLOT_NB)
				{
					uint8_t new_address;
					new_address = (uint8_t)(sensors[id].dev.I2cDevAddr + 2*id + 2);

					erro = VL53L0X_WrByte(&sensors[id].dev, 0x88, 0x00); // Set I2C standard mode (400 KHz) before doing the first register access

					erro |= VL53L0X_SetDeviceAddress(&sensors[id].dev,new_address);
					//it_printf("[id=%d] SetI2CAddress : %d \n", id, erro);

					sensors[id].dev.I2cDevAddr = new_address;

					uint16_t asked_id;
					erro |= VL53L0X_RdWord(&sensors[id].dev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &asked_id);
					printf("[id=%d] GetSensorId : %d \n", id, erro);

					if (asked_id != 0xEEAA)
						erro |= 1;

					sensors[id].distance = 0;
					if(erro)
					{
						//échec, on reset le capteur, et on libère le flag.
						HAL_GPIO_WritePin(sensor_reset_pins[id].GPIO_Port ,sensor_reset_pins[id].GPIO_Pin, VL53_RESET_STATE);
						flag_one_sensor_is_on_default_address = FALSE;
						sensors[id].state = INIT;
						sensors[id].enable = FALSE;
					}
					else
					{
						//réussite du changement d'adresse, on libère le flag et on continue !
						sensors[id].state = SENSOR_INIT;
						flag_one_sensor_is_on_default_address = FALSE;
					}

				}
				break;
			case SENSOR_INIT:
				if(timeslot == (timeslot_e)id)
				{
					if(VL53L0X_DataInit(&sensors[id].dev) == 0)
					{
						sensors[id].dev.Present = 1;
						VL53_Setup(&sensors[id].dev, HIGH_SPEED, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING);	//VL53L0X_DEVICEMODE_SINGLE_RANGING
						erro = VL53L0X_StartMeasurement(&sensors[id].dev);
						if(!erro)
						{
							sensors[id].enable = TRUE;
							sensors[id].state = ASK_AND_GET_DATA;
						}
						else
						{
							HAL_GPIO_WritePin(sensor_reset_pins[id].GPIO_Port ,sensor_reset_pins[id].GPIO_Pin, VL53_RESET_STATE);
							sensors[id].enable = FALSE;
							sensors[id].state = INIT;
						}
					}
				}
				break;

			case ASK_AND_GET_DATA:
				if(timeslot == (timeslot_e)id)
				{

				//	VL53L0X_GetMeasurementDataReady(Dev, &NewDataReady);
					erro = VL53L0X_GetRangingMeasurementData(&sensors[id].dev,	&sensors[id].measurementDatas);
					//sensors[id].dev.Data.PalState = VL53L0X_STATE_IDLE;

					//erro |= VL53L0X_ClearInterruptMask(&sensors[id].dev, 0);



					if(!erro)
					{
						sensors[id].distance = sensors[id].measurementDatas.RangeMilliMeter;
						sensors[id].rangeStatus = sensors[id].measurementDatas.RangeStatus;
					}
					else
						sensors[id].state = FAIL;
				}
				break;

			case FAIL:
				sensors[id].state = INIT;
				HAL_GPIO_WritePin(sensor_reset_pins[id].GPIO_Port ,sensor_reset_pins[id].GPIO_Pin, VL53_RESET_STATE);
				break;

			default :
				break;
		}
	}
	/*for(Uint8 id = 0; id < VL53_NB; id++ ){
		it_printf("Id = %d %d\n", id, sensors[id].state);
	}*/
	if(timeslot == TIMESLOT_ASK_END)//TIMESLOT_GET_END)
	{
		flag_send_can_msg = TRUE;
	}
	timeslot_e ret;
	ret = timeslot;
	timeslot = (timeslot + 1) % TIMESLOT_NB;


	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, 0);
	return ret;
}


void VL53_Setup(VL53L0X_DEV Dev, RangingConfig_e rangingConfig, VL53L0X_DeviceModes deviceMode){
    int i;
    int status;
    uint8_t VhvSettings;
    uint8_t PhaseCal;
    uint32_t refSpadCount;
	uint8_t isApertureSpads;
	FixPoint1616_t signalLimit = (FixPoint1616_t)(0.25*65536);
	FixPoint1616_t sigmaLimit = (FixPoint1616_t)(18*65536);
	uint32_t timingBudget = 33000;
	uint8_t preRangeVcselPeriod = 14;
	uint8_t finalRangeVcselPeriod = 10;


		status=VL53L0X_StaticInit(Dev);
		if( status ){
			debug_printf("VL53L0X_StaticInit %d failed\n",i);
		}

		status = VL53L0X_PerformRefCalibration(Dev, &VhvSettings, &PhaseCal);
		if( status ){
		   debug_printf("VL53L0X_PerformRefCalibration failed\n");
		}

		status = VL53L0X_PerformRefSpadManagement(Dev, &refSpadCount, &isApertureSpads);
		if( status ){
		   debug_printf("VL53L0X_PerformRefSpadManagement failed\n");
		}


		status = VL53L0X_SetDeviceMode(Dev, deviceMode); // Setup in single ranging mode
		if( status ){
		   debug_printf("VL53L0X_SetDeviceMode failed\n");
		}

		status = VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1); // Enable Sigma limit
		if( status ){
		   debug_printf("VL53L0X_SetLimitCheckEnable failed\n");
		}

		status = VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1); // Enable Signa limit
		if( status ){
		   debug_printf("VL53L0X_SetLimitCheckEnable failed\n");
		}

		switch(rangingConfig) {
		case LONG_RANGE:
			signalLimit = (FixPoint1616_t)(0.1*65536);
			sigmaLimit = (FixPoint1616_t)(60*65536);
			timingBudget = 33000;
			preRangeVcselPeriod = 18;
			finalRangeVcselPeriod = 14;
			break;
		case HIGH_ACCURACY:
			signalLimit = (FixPoint1616_t)(0.25*65536);
			sigmaLimit = (FixPoint1616_t)(18*65536);
			timingBudget = 200000;
			preRangeVcselPeriod = 14;
			finalRangeVcselPeriod = 10;
			break;
		case HIGH_SPEED:
			signalLimit = (FixPoint1616_t)(0.25*65536);
			sigmaLimit = (FixPoint1616_t)(32*65536);
			timingBudget = 20000;
			preRangeVcselPeriod = 14;
			finalRangeVcselPeriod = 10;
			break;
		default:
			debug_printf("Not Supported");
		}

		status = VL53L0X_SetLimitCheckValue(Dev,  VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, signalLimit);
		if( status ){
		   debug_printf("VL53L0X_SetLimitCheckValue failed\n");
		}

		status = VL53L0X_SetLimitCheckValue(Dev,  VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, sigmaLimit);
		if( status ){
		   debug_printf("VL53L0X_SetLimitCheckValue failed\n");
		}

		status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(Dev,  timingBudget);
		if( status ){
		   debug_printf("VL53L0X_SetMeasurementTimingBudgetMicroSeconds failed\n");
		}

		status = VL53L0X_SetVcselPulsePeriod(Dev,  VL53L0X_VCSEL_PERIOD_PRE_RANGE, preRangeVcselPeriod);
		if( status ){
		   debug_printf("VL53L0X_SetVcselPulsePeriod failed\n");
		}

		status = VL53L0X_SetVcselPulsePeriod(Dev,  VL53L0X_VCSEL_PERIOD_FINAL_RANGE, finalRangeVcselPeriod);
		if( status ){
		   debug_printf("VL53L0X_SetVcselPulsePeriod failed\n");
		}

		status = VL53L0X_PerformRefCalibration(Dev, &VhvSettings, &PhaseCal);
		if( status ){
		   debug_printf("VL53L0X_PerformRefCalibration failed\n");
		}

		Dev->LeakyFirst=1;


}


void VL53L0X_process_main(void)
{
	char buf[100];
	uint8_t index = 0;
	if(flag_send_can_msg)
	{
		flag_send_can_msg = FALSE;

		for(uint8_t i = 0; i<VL53_NB; i++)
		{
			if(sensors[i].rangeStatus)
				index += (uint8_t)sprintf(buf+index,"E%x ",sensors[i].rangeStatus);
			else
				index += (uint8_t)sprintf(buf+index,"%d ",sensors[i].distance/10);
			for(;index<4*(i+1);)
				index += (uint8_t)sprintf(buf+index," ");
		}
		debug_printf("%s\n",buf);
	}
}


uint16_t VL53L0X_get_distance(uint8_t id)
{
	assert(id<VL53_NB);
	return sensors[id].distance;
}




#endif

