////////////////////////////Module's Description////////////////////////////////
/*!
 * \file       FOTA.c
 * \brief      This is the FOTA library made by S4IoT
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
//YmVld2F6ZWFuYWxvZ2ljbw==
/////////////////////////End of Module Description//////////////////////////////
#include <FOTA.h>

#define SSL_ID 0
#define HTTP_RESPONSE_TIME 60
#define HTTP_READ_WAIT_TIME 150
#define TCP_CONTEXT_ID 1

uint8_t http_response[1000] = {0};
uint16_t response_size = 0;

/** Variable to check the return values*/
cm_q03_codes_t retval;

uint8_t file_name_read[] = "YmVld2F6ZWFuYWxvZ2ljbw==.bin";
fota_codes_t FOTA_Download_File(void)
{
    http_config_t http_config_struct;
    uint8_t url_get[] = "http://3.138.90.175:3000";
	uint16_t content_length = 0;
	uint8_t headers[] =
	        "GET / HTTP/1.1\r\nHost: 3.138.90.175:3000\r\ntoken:S4IOT\r\nversion:YmVld2F6ZWFuYWxvZ2ljbw==.bin\r\n\r\n";
	uint8_t count_attempts_dowload=0;

	http_config_struct.context_id = TCP_CONTEXT_ID;
	http_config_struct.request_header = request_enable;
	http_config_struct.response_header = response_disable;
	http_config_struct.sslctxID = SSL_ID;
	http_config_struct.data_type = plain;
	retval = CM_Q03_HTTP_Configure_Parameters(http_config_struct, Personal_Configuration);

	do
	{
        count_attempts_dowload++;
		retval = CM_Q03_HTTP_Set_URL(url_get, sizeof(url_get)-1);

		retval = CM_Q03_HTTP_Send_GET(HTTP_RESPONSE_TIME, headers, sizeof(headers)-1, &content_length);

		if(HTTPS_OK == retval)
		{
			retval = CM_Q03_HTTP_Read_via_File(HTTP_READ_WAIT_TIME, file_name_read, sizeof(file_name_read)-1);
		}else if(HTTPS_OK < retval && HTTPS_Unknown_error > retval)
		{
			if(HTTPS_Not_Found == retval)
			{
				return FOTA_Server_Not_Found;
			}else if(HTTPS_Timeout == retval)
			{
				return FOTA_Server_Timeout;
			}
			CM_Q03_HTTP_Read(HTTP_READ_WAIT_TIME, http_response, &response_size);
			return FOTA_Server_Error;
		}

	}while(Success != retval && count_attempts_dowload < 3);

	if(count_attempts_dowload >=3){
	    count_attempts_dowload=0;
        return FOTA_Server_Error;

	}
	return FOTA_Download_Success;
}

fota_codes_t FOTA_Write_File_to_Memory(uint32_t sw_v)
{
	bootloader_config_t Boot_Config =
	{
			BOOT_CONFIG_TAG,
			MEM_A_INIT,
			MEM_B_INIT,
			MEM_A_FINISH,
			MEM_B_FINISH,
			MEM_A_SIZE,
			MEM_B_SIZE,
			STACK_POINTER_ADDRESS,
			WRITE_SIZE,
			MEMORY_ERASED
	};
	uint32_t file_size = 0;
	uint32_t erase_size = 0;
	uint8_t erase_diff = 0;
	flash_config_t flashDriver = {0};
	status_t result = 0;
	uint32_t bootConfigAddress = BOOT_CONFIG_ADDRESS;
	uint32_t file_handler = 0;
	uint32_t applicationAddress = MEM_B_INIT;
	uint16_t length_read = WRITE_SIZE;
	uint8_t buffer_read[WRITE_SIZE] = {0};
	uint16_t buffer_read_size = 0;
	uint16_t read_size = 0;
	uint16_t count_read=0;
	bool first_mem=true;
	retval = CM_Q03_File_List_File(file_name_read, sizeof(file_name_read)-1, &file_size);

	if(Success != retval)
	{
		return FOTA_File_Not_Found;
	}

	erase_size = file_size;
	do
	{
		erase_diff = erase_size % 4;
		erase_size += erase_diff;
		if(erase_size > Boot_Config.Memory_B_Size)
		{
			erase_size = Boot_Config.Memory_B_Size;
		}
	}while(erase_diff && (Boot_Config.Memory_B_Size != erase_size));

	Boot_Config.Update_Write_Size = (erase_size << 8) | (WRITE_SIZE);

	/* Setup flash driver structure for device and initialize variables. */
	result = FLASH_Init(&flashDriver);

	result = FLASH_Erase(&flashDriver, bootConfigAddress, sizeof(bootloader_config_t), kFTFx_ApiEraseKey);
	result = FLASH_VerifyErase(&flashDriver, bootConfigAddress, sizeof(bootloader_config_t),kFTFx_MarginValueUser);

	if(Success != retval)
	{
		return FOTA_Flash_Error;
	}

	do
	{
		result = FLASH_Program(&flashDriver, BOOT_CONFIG_ADDRESS, (uint8_t*)&Boot_Config.tag,
								sizeof(bootloader_config_t));
		if (result == kStatus_FTFx_Success)
		{
			result = FLASH_VerifyProgram(&flashDriver, BOOT_CONFIG_ADDRESS, sizeof(bootloader_config_t),
											(uint8_t*)&Boot_Config.tag, kFTFx_MarginValueUser, NULL, NULL);
		}
	}while(result != kStatus_FTFx_Success);


	retval = CM_Q03_File_Open_File(file_name_read, sizeof(file_name_read)-1,
									open_read_only, &file_handler);
	if(Success != retval)
	{
	    CM_Q03_File_Close_File(file_handler);
        CM_Q03_File_Delete_File(file_name_read, sizeof(file_name_read)-1);

		return FOTA_Error_Open_File;
	}
	/* Erase Sector. */
	result = FLASH_Erase(&flashDriver, applicationAddress, erase_size, kFTFx_ApiEraseKey);
	/* Verify sector if it's been erased. */
	result = FLASH_VerifyErase(&flashDriver, applicationAddress, erase_size, kFTFx_MarginValueUser);

	while(applicationAddress < Boot_Config.Memory_B_Finish)
	{
		retval = CM_Q03_File_Read_File(file_handler, length_read, buffer_read, &buffer_read_size);
		LED_Control(LED_on);
		read_size = buffer_read_size;
		while(buffer_read_size % 4)
		{
			buffer_read[buffer_read_size++] = 0xFF;
		}

		if(first_mem){
		    first_mem=false;

		    count_read=16;

		    if(sw_v >= ((buffer_read[0]<<24)+(buffer_read[1]<<16)+(buffer_read[2]<<8)+buffer_read[3])){

		        retval = CM_Q03_File_Close_File(file_handler);
		        if(Success == retval)
		        {
		            retval = CM_Q03_File_Delete_File(file_name_read, sizeof(file_name_read)-1);
		            if(Success == retval)
		            {
		                return FOTA_Currently_Updated;
		            }
		        }else
		        {
		            return FOTA_Close_File_Error;
		        }
		    }

		}
		else{
		    count_read=0;
		}

        do{
            buffer_read[count_read]=buffer_read [count_read]^0xAA;
            count_read++;
        }while(count_read < WRITE_SIZE);


		do{
			/* Program user buffer into flash*/
			result = FLASH_Program(&flashDriver, applicationAddress, buffer_read, (uint32_t)buffer_read_size);
			if (result == kStatus_FTFx_Success)
			{
				/* Verify programming by Program Check command with user margin levels */
				result = FLASH_VerifyProgram(&flashDriver, applicationAddress, (uint32_t)buffer_read_size, buffer_read,
												kFTFx_MarginValueUser, NULL, NULL);
				if(result == kStatus_FTFx_Success)
				{
					applicationAddress += WRITE_SIZE;
					file_size -= read_size;
					LED_Control(LED_off);
				}
			}
		}while(result != kStatus_FTFx_Success);
		if(!file_size)
		{
			break;
		}
	}
	retval = CM_Q03_File_Close_File(file_handler);
	if(Success == retval)
	{
		retval = CM_Q03_File_Delete_File(file_name_read, sizeof(file_name_read)-1);
		if(Success == retval)
		{
			return FOTA_Write_File_Success;
		}
	}else
	{
		return FOTA_Close_File_Error;
	}
	return FOTA_Delete_File_Error;
}

void FOTA_Software_Reset(void)
{
  /* Generic way to request a reset from software for ARM Cortex */
  /* See https://community.freescale.com/thread/99740
     To write to this register, you must write 0x5FA to the VECTKEY field, otherwise the processor ignores the write.
     SYSRESETREQ will cause a system reset asynchronously, so need to wait afterwards.
   */
	SCB->AIRCR = (0x5FA<<SCB_AIRCR_VECTKEY_Pos)|SCB_AIRCR_SYSRESETREQ_Msk;
  for(;;) {
    /* wait until reset */
  }
}
