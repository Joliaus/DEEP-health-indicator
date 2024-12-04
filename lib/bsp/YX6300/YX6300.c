#include "config.h"
#if USE_YX6300
#error "code en ébauche, non testé, non terminé"



#ifndef YX6300_UART_ID
#define YX6300_UART_ID	UART1_ID
#endif


#define FIRST_SONG	0x01
#define FIRST_SONG	0x03


void example(void)
{
	
	uint8_t data[2];
	data[0] = 0x00;
	data[1] = 0x00;
	YX6300_send_request(FIRST_SONG, FALSE, 2, data);
	
	uint16_t index_song = 4;
	uint8_t data[2];
	data[0] = HIGHINT(index_song);
	data[1] = LOWINT(index_song);
	YX6300_send_request(PLAY_WITH_INDEX, FALSE, 2, data);

	YX6300_send_request_with_2bytes_of_datas(PLAY_WITH_INDEX, FALSE, index_song);
}


void YX6300_send_request_with_2bytes_of_datas(uint8_t command, bool_e feedback, uint16_t data16)
{
	uint8_t msg[2+4+10];	//on fait le choix de refuser la demmande si datasize > 10
	uint8_t i = 0;
	msg[i++] = 0x7E;
	msg[i++] = 0xFF;
	msg[i++] = 2+4;	//length = (FF+length+command+feedback) + datasize
	msg[i++] = command;
	msg[i++] = (feedback)?1:0;
	
	msg[i++] = HIGHINT(data16);
	msg[i++] = LOWINT(data16);
	
	msg[i++] = 0xEF;
	
	UART_puts(YX6300_UART_ID, msg, i);
}



void YX6300_send_request(uint8_t command, bool_e feedback, uint8_t datasize, uint8_t * data)
{
	uint8_t msg[2+4+10];	//on fait le choix de refuser la demmande si datasize > 10
	
	uint8_t i = 0;
	
	if(datasize<=10)
	{
		msg[i++] = 0x7E;
		msg[i++] = 0xFF;
		msg[i++] = datasize+4;	//length = (FF+length+command+feedback) + datasize
		msg[i++] = command;
		msg[i++] = (feedback)?1:0;
		
		uint8_t d;
		for(d=0; d<datasize; d++)
			msg[i++] = data[d];
		
		msg[i++] = 0xEF;
		
		UART_puts(YX6300_UART_ID, msg, i);
	}
	else
	{
		debug_printf("you should correct this function or respect the datasize limit!\n);
	}
}

#endif
