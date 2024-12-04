/*
 * SENO_1888.h
 *
 *  Created on: 17 mai 2019
 *      Author: nilepier
 */

#ifndef SENO_1888_H_
#define SENO_1888_H_

#include <stdint.h>
#include "macro_types.h"

#define MODULE_DEFAULT_ADDRESS	0xFFFFFFFF

#define COMMAND_ID_READ_SYS_PARAM		0x0f
#define OPEN_LED						0x50
#define CLOSE_LED						0x51
#define GET_IMG							0x01
#define GET_IMAGE_FREE					0x52
#define GET_ECHO						0x53
#define REG_MODEL						0x05

typedef enum
{
	PID_UNKNOW = 0,
	PID_COMMAND = 1,
	PID_DATA = 2,
	PID_ACK = 7,
	PID_END = 8,
}pid_e;

typedef struct
{
	uint32_t addr;
	pid_e pid;
	uint16_t lenght;
	uint8_t datas[256];
}frame_t;

typedef enum
{
	CONFIRMATION_CODE_OK = 0,
	CONFIRMATION_CODE_ERROR_RECEIVING_DATA_PACKET = 1,
	CONFIRMATION_CODE_NO_FINGER = 2,
	CONFIRMATION_CODE_FAIL_TO_ENROLL = 3,
	CONFIRMATION_CODE_FAIL_GENERATE_CHARACTER_FILE_DISORDER = 6,
	CONFIRMATION_CODE_FAIL_GENERATE_CHARACTER_FILE_WEAKNESS = 7,
	CONFIRMATION_CODE_FINGER_DOES_NOT_MATCH = 8,
	CONFIRMATION_CODE_FAIL_TO_FIND_MATCHING_FINGER = 9,
	CONFIRMATION_CODE_FAIL_TO_COMBINE_CHAR_FILES = 0x0A,
	CONFIRMATION_CODE_PAGE_ID = 0x0B,
	CONFIRMATION_CODE_FAIL_READING_TEMPLATE = 0x0C,
	CONFIRMATION_CODE_FAIL_UPLOADING_TEMPLATE = 0x0D,
	CONFIRMATION_CODE_FAIL_RECEIVE_DATA = 0x0E,
	CONFIRMATION_CODE_FAIL_UPLOADING_IMAGE = 0x0F,
	CONFIRMATION_CODE_FAIL_DELETE_TEMPLATE = 0x10,
	CONFIRMATION_CODE_FAIL_CLEAR_FINGER_LIBRARY = 0x11,
	CONFIRMATION_CODE_FAIL_GENERATE_IMAGE_WEAKNESS_PRIMARY_IMAGE= 0x15,
	CONFIRMATION_CODE_FAIL_WRITING_FLASH = 0x18,
	CONFIRMATION_CODE_FAIL_REGISTER_NUMBER = 0x1A,


}confirmation_code_e;

void send_setPwd(uint32_t address, uint32_t password);

void send_Img2Tz(uint32_t address, uint8_t buffer_id);

void send_store(uint32_t address, uint8_t buffer_id, uint16_t page_id);

void send_command_withoutparameters(uint32_t address, uint8_t command_id);

void send_search(uint32_t address, uint8_t buffer_id, uint16_t start_page, uint16_t page_num);



bool_e parse_frames(uint8_t byte, frame_t * pframe);

void send_command(frame_t * pframe);


#endif /* SENO_1888_H_ */
