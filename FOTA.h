////////////////////////////Module's Description////////////////////////////////
/*!
 * \file       FOTA.h
 * \brief      This is the FOTA library made by S4IoT
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

////////////////////////////////Include Guard///////////////////////////////////
#ifndef FOTA_H_
#define FOTA_H_
/////////////////////////////End of Include Guard///////////////////////////////

////////////////////////////////Includes Area///////////////////////////////////
#include <CM_Q03_File.h>
#include <CM_Q03_HTTP.h>
#include "fsl_flash.h"
#include "init_config_functions.h"
#include "fuel_gauge_lite.h"
///////////////////////////End of Includes Area/////////////////////////////////

/////////////////////////////Definitions Area///////////////////////////////////
#define BOOT_CONFIG_ADDRESS		(0x3FD4)

#define BOOT_CONFIG_TAG			(0x73626F70)
#define MEM_A_INIT				(0x04000)
#define MEM_A_SIZE				(0x1D000)
#define MEM_A_FINISH			(0x20FFF)
#define MEM_B_INIT				(0x21000)
#define MEM_B_SIZE				(0x1F000)
#define MEM_B_FINISH			(MEM_B_INIT + MEM_B_SIZE - 1)

#define APP_FILE_SIZE			(5360)
#define WRITE_SIZE				(256)

#define STACK_POINTER_ADDRESS	(0x20005800)

#define MEMORY_ERASED			(0xFFFFFFFF)
//////////////////////////End of Definitions Area///////////////////////////////

//////////////////////////Structures and Enumerators////////////////////////////
typedef struct {
	uint32_t tag;
	uint32_t Memory_A_Init;
	uint32_t Memory_B_Init;
	uint32_t Memory_A_Finish;
	uint32_t Memory_B_Finish;
	uint32_t Memory_A_Size;
	uint32_t Memory_B_Size;
	uint32_t Stack_Pointer;
	uint32_t Update_Write_Size;
	uint32_t ReturnCode;
}bootloader_config_t;

typedef enum{
	FOTA_Download_Success,
	FOTA_Server_Not_Found,
	FOTA_Server_Timeout,
	FOTA_Server_Error,
	FOTA_File_Not_Found,
	FOTA_Flash_Error,
	FOTA_Error_Open_File,
	FOTA_Write_File_Success,
	FOTA_Close_File_Error,
	FOTA_Delete_File_Error,
	FOTA_Currently_Updated
}fota_codes_t;
//////////////////////End of Structures and Enumerators/////////////////////////

//////////////////////////////Functions Description/////////////////////////////
fota_codes_t FOTA_Download_File(void);

fota_codes_t FOTA_Write_File_to_Memory(uint32_t sw_v);

void FOTA_Software_Reset(void);
//////////////////////////End of Functions Description//////////////////////////

/////////////////////////////Include Guard Closing//////////////////////////////

#endif /* FOTA_H_ */
/////////////////////////End of Include Guard Closing///////////////////////////
