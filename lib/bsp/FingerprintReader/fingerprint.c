/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/
#include "stm32f1xx_hal.h"


#include "config.h"
#if USE_FINGERPRINT
#include "fingerprint.h"
#include "stm32f1_uart.h"
#include "macro_types.h"
#include "stm32f1_gpio.h"

#ifndef FINGERPRINT_UART_ID
	#define FINGERPRINT_UART_ID UART3_ID
#endif

void fingerprint_state_machine(void);

static volatile bool_e flag_answer_received = FALSE;
static volatile bool_e flag_ack = FALSE;
static volatile bool_e flag_ack_fail = FALSE;
static volatile bool_e flag_cannot_detect_finger = FALSE;
static volatile bool_e flag_fail_to_collect_finger = FALSE;
static volatile confirmation_code_e confirmation_code;
static volatile uint32_t t = 0;
static volatile uint16_t page_id = 0;

void fingerprint_process_ms(void)
{
	if(t)
		t--;
}


int fingerprint_process_main(void)
{

	static frame_t frame;


	uint8_t c;
	fingerprint_state_machine();

	if(UART_data_ready(FINGERPRINT_UART_ID))
	{
		c = UART_get_next_byte(FINGERPRINT_UART_ID);
		if(parse_frames(c, &frame))
		{
			if(frame.pid == PID_ACK)
			{
				flag_answer_received = TRUE;
				confirmation_code = frame.datas[0];
				switch(frame.datas[0])
				{
					case 0x00:
							flag_ack = TRUE;
							break;
					case 0x01:
							flag_ack_fail = TRUE;
							break;
					case 0x02:
							flag_cannot_detect_finger = TRUE;
							break;
					case 0x03:
							flag_fail_to_collect_finger = TRUE;
							break;
					case 0x0a:
							flag_ack_fail = TRUE;
							break;
					case 0x09:
							flag_ack_fail = TRUE;
							break;
				}
			}
			for(int i=0; i<frame.lenght && i<256; i++)
				printf("%08x ", frame.datas[i]);

		}
	}
}


void fingerprint_state_machine(void){
	typedef enum
	 {
			 INIT = 0,
			 LED_ALLUME,
			 GET_IMAGE,
			 FAIL,
			 IMG_TZ,
			 REG_MOD,
			 STORE,
			 SEARCH,
			 KNOWN_DIGIT

	 }state_e;
	 static state_e state = INIT;
	 static state_e previous_state = INIT; //permet de sauvegarder l'état précédent
	 bool_e entrance;
	 entrance = (state!=previous_state)?TRUE:FALSE;
	 //le booléen entrance est vrai si on vient juste de changer d'état !
	 previous_state = state; //l'état courant est sauvegardé, ce sera le prochain "état précédent"
	 static bool_e img_index = 0;
	 static int8_t Font_11x18;
	 static uint8_t pageid;

	 switch(state)
	 {
			 case INIT:
				 	 HAL_Delay(300);
				 	 Systick_add_callback_function(&fingerprint_process_ms);
				 	pageid = 0;
				 	 state = LED_ALLUME;
				 	 break;
			 case LED_ALLUME:
				 	 if(entrance)
				 	 {
				 		 send_command_withoutparameters(MODULE_DEFAULT_ADDRESS, OPEN_LED);
				 		 t = 4000;
				 		 printf("envoi commande : allume led - \n");
				 	 }
				 	 if(flag_answer_received)
				 	 {
				 		 flag_answer_received = FALSE;
				 		 if(confirmation_code == CONFIRMATION_CODE_OK)
				 		 {
							 printf("OK Led-allume \n");
							 state = GET_IMAGE;
				 		 }
				 		 else
				 		 {
				 			state = FAIL;
							 printf("FAIL Led-allume \n");
				 		 }
				 	 }

				 	 if(!t){
				 		 printf("TIME OUT Led-allume \n");
				 		 state = FAIL;
				 	 }
				 	 break;
			 case GET_IMAGE:
				 	 if(entrance)
				 	 {
				 		 send_command_withoutparameters(MODULE_DEFAULT_ADDRESS, GET_IMG);
				 		 t = 4000;
				 	 }
				 	 if(flag_ack)
				 	 {
				 		 flag_ack = FALSE;
				 		 printf("get img OK\n");
						 state = IMG_TZ;
				 	 }
				 	 if(flag_ack_fail)
					 {
					 	 flag_ack_fail = FALSE;
						 state = FAIL;
						 printf("get img FAIL\n");
					 }
				 	 if(!t)
				 	 {
						 state = FAIL;
						 printf("TIME OUT get-image \n");
					 }
				 	 break;
			  case IMG_TZ:
			 		 if(entrance)
			 		 {
			 			 send_Img2Tz(MODULE_DEFAULT_ADDRESS, 0x01+img_index);
			 			 t = 4000;
			 		 }
			 		 if(flag_ack)
			 		 {
			 			 flag_ack = FALSE;
			 			 if(img_index == 0)
			 			 {
			 			 img_index = TRUE;
			 			 state = SEARCH;
			 			 }
			 			 else
			 			 {
			 			 img_index = FALSE;
			 			 state = SEARCH;
			 			 }
			 		  }
			 		  if(flag_ack_fail)
			 		  {
			 			 flag_ack_fail = FALSE;
			 			 state = FAIL;
			 			 printf("OK img-tz \n");
			 		  }
			 		  if(!t)
			 		  {
			 			 flag_ack_fail = FALSE;
			 			 state = FAIL;
			 			 printf("TIME OUT img-tz \n");
			 		  }
			 		  break;
			  case SEARCH:

			 		  if(entrance)
			 		  {
			 			  send_search(MODULE_DEFAULT_ADDRESS, 0x01, 0x0001, pageid);
			 			  t = 5000;
			 		  }
			 		  if(flag_ack)
			 		  {
			 			 if(!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
						  {
								flag_ack_fail = FALSE;
								state= REG_MOD;
						  }
			 			 else
			 			 {

			 				state = KNOWN_DIGIT;
			 			 }
			 		  }

			 		  if(flag_ack_fail)
			 		  {
			 			 if(!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
						  {
								flag_ack_fail = FALSE;
								state= REG_MOD;
						  }
						 else
						 {
							 printf("Entree refusee\n");
							 state = FAIL;
						 }
			 		  }

			 		  if(!t)
			 		  {
			 			  flag_ack_fail = FALSE;
			 			  state = FAIL;
			 			  printf("Retirez votre empreinte\n");
			 		  }
			 		  break;
			 	case REG_MOD:
			 				 if(entrance)
			 				 {
			 					send_command_withoutparameters(MODULE_DEFAULT_ADDRESS, REG_MODEL);
			 					printf("envoie commande REG_MOD\n");
			 				 	t = 3000;
			 				 }
			 				 if(flag_ack)
			 				 {
			 				 	 flag_ack = FALSE;
			 				 	 state = STORE;
			 				 	 printf("OK reg-mod \n");
			 				 }
			 				 if(flag_ack_fail)
			 				 {
			 					 	flag_ack_fail = FALSE;
			 				 		state = GET_IMAGE;
			 				 		printf("FAIL reg-mod \n");
			 				 }
			 				  if(!t){
			 					    flag_ack_fail = FALSE;
			 				 		state = FAIL;
			 				 		printf("TIME OUT reg-mod \n");
			 				  }
			 				  break;
			 	case STORE:
			 				 if(entrance)
			 				 {
			 						send_store(MODULE_DEFAULT_ADDRESS, 0x01, pageid);
			 						printf("Demande d'enregistrement à la pageid = %d\n",pageid);
			 						pageid++;
			 						t = 8000;
			 				 }
			 				 if(flag_ack)
			 				 {
			 						flag_ack = FALSE;
			 						printf("Empreinte enregistree\n");
			 						state = SEARCH;
			 				 }
			 				 if(flag_ack_fail)
			 				 {
			 					flag_ack_fail = FALSE;
			 					state = FAIL;
			 					printf("Echec de l'enregistrement\n");
			 				}
			 				if(!t)
			 				{
			 						flag_ack_fail = FALSE;
			 						state = FAIL;
			 						printf("TIME OUT store \n");
			 				}
			 				break;

			 	case KNOWN_DIGIT:
			 		if(entrance)
			 		{
			 			printf("Doigt reconnu !!!\n");
			 			send_command_withoutparameters(MODULE_DEFAULT_ADDRESS, OPEN_LED);
			 		}

			 		state = SEARCH;
			 		break;
			 case FAIL:
				 t=500;
				 state = GET_IMAGE;
				 break;

			 default:
			 	 break;
	 }

}









void send_setPwd(uint32_t address, uint32_t password)
{
	frame_t frame;
	frame.addr = address;
	frame.pid = PID_COMMAND;
	frame.lenght = 2+5;
	frame.datas[0] = 0x12;
	frame.datas[1] = (uint8_t)((password>>24)&0xFF);
	frame.datas[2] = (uint8_t)((password>>16)&0xFF);
	frame.datas[3] = (uint8_t)((password>>8)&0xFF);
	frame.datas[4] = (uint8_t)((password)&0xFF);
	send_command(&frame);
}


void send_genImg(uint32_t address, uint32_t password)
{
	frame_t frame;
	frame.addr = address;
	frame.pid = PID_COMMAND;
	frame.lenght = 2+1;
	frame.datas[0] = 0x01;
	send_command(&frame);
}


void send_Img2Tz(uint32_t address, uint8_t buffer_id){
	frame_t frame;
	frame.addr = address;
	frame.pid = PID_COMMAND;
	frame.lenght = 2+2;
	frame.datas[0] = 0x02;
	frame.datas[1] = buffer_id;
	send_command(&frame);
}

void send_store(uint32_t address, uint8_t buffer_id, uint16_t page_id){
	frame_t frame;
	frame.addr = address;
	frame.pid = PID_COMMAND;
	frame.lenght = 2+4;
	frame.datas[0] = 0x06;
	frame.datas[1] = buffer_id;
	frame.datas[2] = (uint8_t)((page_id>>8)&0xFF);
	frame.datas[3] = (uint8_t)((page_id)&0xFF);
	send_command(&frame);
}

void send_command_withoutparameters(uint32_t address, uint8_t command_id)
{
	frame_t frame;
	frame.addr = address;
	frame.pid = PID_COMMAND;
	frame.lenght = 2+1;
	frame.datas[0] = command_id;
	send_command(&frame);
}

void send_search(uint32_t address, uint8_t buffer_id, uint16_t start_page, uint16_t page_num)
{
	frame_t frame;
	frame.addr = address;
	frame.pid = PID_COMMAND;
	frame.lenght = 2+6;
	frame.datas[0] = 0x04;
	frame.datas[1] = buffer_id;
	frame.datas[2] = (uint8_t)((start_page>>8)&0xFF);
	frame.datas[3] = (uint8_t)((start_page)&0xFF);
	frame.datas[4] = (uint8_t)((page_num>>8)&0xFF);
	frame.datas[5] = (uint8_t)((page_num)&0xFF);
	send_command(&frame);
}




bool_e parse_frames(uint8_t byte, frame_t * pframe)
{
	typedef enum
	{
		STEP_HEADER_WAIT_EF,
		STEP_HEADER_WAIT_01,
		STEP_ADDR0,
		STEP_ADDR1,
		STEP_ADDR2,
		STEP_ADDR3,
		STEP_PID,
		STEP_LENGTH0,
		STEP_LENGTH1,
		STEP_DATAS,
		STEP_CS0,
		STEP_CS1
	}step_e;

	static step_e step = STEP_HEADER_WAIT_EF;
	static uint16_t size_datas = 0;
	static uint16_t checksum;
	static uint16_t sum;

	bool_e ret;
	ret = FALSE;
	if(step==STEP_PID)
		sum = byte;
	else if(step == STEP_LENGTH0 || step == STEP_LENGTH1 || step == STEP_DATAS)
		sum = sum + ((uint16_t)(byte));

	switch(step)
	{
		case STEP_HEADER_WAIT_EF:
			if(byte == 0xEF)
				step = STEP_HEADER_WAIT_01;
			break;
		case STEP_HEADER_WAIT_01:
			if(byte == 0x01)
				step = STEP_ADDR0;
			else
				step = STEP_HEADER_WAIT_EF;
			break;
		case STEP_ADDR0:
			pframe->addr = (uint32_t)byte<<24;
			step++;
			break;
		case STEP_ADDR1:
			pframe->addr |= (uint32_t)byte<<16;
			step++;
			break;
		case STEP_ADDR2:
			pframe->addr |= (uint32_t)byte<<8;
			step++;
			break;
		case STEP_ADDR3:
			pframe->addr |= (uint32_t)byte;
			step++;
			break;
		case STEP_PID:
			if(byte == PID_COMMAND || byte == PID_DATA || byte == PID_ACK || byte ==PID_END)
			{
				pframe->pid = byte;
				printf("pid OK=%d\n", pframe->pid);
			}
			else
			{
				printf("pid KO !!!=%d\n", pframe->pid);
				pframe->pid = PID_UNKNOW;
			}
			step++;
			break;
		case STEP_LENGTH0:
			pframe->lenght = U16FROMU8(byte, 0);
			step++;
			break;
		case STEP_LENGTH1:
			pframe->lenght |= ((uint16_t)(byte));

			if(pframe->lenght <= 256)
			{
				size_datas = 0;
				step = STEP_DATAS;
				printf("length=%d\n", pframe->lenght);
			}
			else
			{
				printf("mauvais length=%d\n", pframe->lenght);
				step = STEP_HEADER_WAIT_EF;
			}
			break;
		case STEP_DATAS:
			pframe->datas[size_datas] = byte;
			size_datas++;
			if(size_datas >= pframe->lenght - 2)
				step = STEP_CS0;
			break;
		case STEP_CS0:
			checksum = ((uint16_t)(byte))<<8;
			step++;
			break;
		case STEP_CS1:
			checksum |= ((uint16_t)(byte));
			if(checksum == sum)
			{
				ret = TRUE;
			}
			else
			{
				printf("mauvais checksum sum=%d, checksum=%d!\n", sum, checksum);
			}
			step = STEP_HEADER_WAIT_EF;
			break;
		default:
			break;
	}
	return ret;
}

void send_command(frame_t * pframe)
{
	uint8_t buf[300];
	uint16_t size = 0;
	buf[size++] = 0xEF;
	buf[size++] = 0x01;
	buf[size++] = (char)(pframe->addr>>24)&0xFF;
	buf[size++] = (char)(pframe->addr>>16)&0xFF;
	buf[size++] = (char)(pframe->addr>>8)&0xFF;
	buf[size++] = (char)(pframe->addr)&0xFF;
	buf[size++] = (pframe->pid);
	buf[size++] = (char)(pframe->lenght>>8)&0xFF;
	buf[size++] = (char)(pframe->lenght)&0xFF;
	for(int i = 0; i<pframe->lenght-2; i++)
		buf[size++] = pframe->datas[i];
	uint16_t sum = 0;
	for(int i = 6; i<6+1+pframe->lenght; i++)
	{
		sum+= (uint16_t)buf[i];
	}
	buf[size++] = (char)(sum>>8)&0xFF;
	buf[size++] = (char)(sum)&0xFF;
	UART_puts(FINGERPRINT_UART_ID, buf, size);
}

#endif



