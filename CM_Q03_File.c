////////////////////////////COPYRIGHT NOTICES///////////////////////////////////
/*
 * Copyright 2016-2018 NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
//////////////////////END OF COPYRIGHT NOTICES//////////////////////////////////

////////////////////////////Module's Description////////////////////////////////
/*!
 * \file       CM_Q03_File.h
 * \brief      This is the CM_Q03 File driver made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.\n
 *             **to identify the meaning of a CME error, please refer to the
 *             provided Quectel FILE AT commands manual.\n
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

#include <CM_Q03_File.h>
#include "string.h"

#define FILE_TIMEOUT 			(5000000)
#define CONNECT_SP_SIZE			(8)

extern uint8_t character_command_end;
extern uint8_t command_comma;
extern uint8_t command_quotes;
extern uint8_t response_ok[2];
extern uint8_t g_interrupt_ri;

enum File_codes{
    _File_Invalid_input_value=400,
    _File_Larger_than_the_size_of_the_file,
    _File_Read_zero_byte,
    _File_Drive_full,
    _File_File_not_found,
    _File_Invalid_file_name,
    _File_File_already_existed,
    _File_Fail_to_write_the_file,
    _File_Fail_to_open_the_file,
    _File_Fail_to_read_the_file,
    _File_Reach_the_max_number_of_file_allowed_to_be_opened,
    _File_The_file_read_only,
    _File_Invalid_file_descriptor,
    _File_Fail_to_list_the_file,
    _File_Fail_to_delete_the_file,
    _File_Fail_to_get_disk_info,
    _File_No_space,
    _File_Time_out,
    _File_File_too_large,
    _File_Invalid_parameter,
    _File_File_already_opened,

};

uint8_t file_space_information_of_ufs[14]={"AT+QFLDS=\"UFS\""};
uint8_t file_space_information_used_of_ufs[8]={"AT+QFLDS"};
uint8_t file_space_information_response[8]={"+QFLDS: "};

uint8_t file_list_file_name[9]={"AT+QFLST="};
uint8_t file_list_files_response[8]={"+QFLST: "};

uint8_t file_list_delete_file[9]={"AT+QFDEL="};

uint8_t file_upload_file[9]={"AT+QFUPL="};
uint8_t file_upload_file_response[8]={"+QFUPL: "};

uint8_t file_download_file[9]={"AT+QFDWL="};
uint8_t file_download_file_response[8]={"+QFDWL: "};

uint8_t file_open_file[10]={"AT+QFOPEN="};
uint8_t file_open_file_response[9]={"+QFOPEN: "};

uint8_t file_read_file[10]={"AT+QFREAD="};

uint8_t file_write_file[11]={"AT+QFWRITE="};
uint8_t file_write_file_response[10]={"+QFWRITE: "};

uint8_t file_set_file_pointer[10]={"AT+QFSEEK="};

uint8_t file_get_file_pointer[14]={"AT+QFPOSITION="};
uint8_t file_get_file_pointer_response[13]={"+QFPOSITION: "};

uint8_t file_close_file[11]={"AT+QFCLOSE="};
uint8_t file_delete_file[9] = {"AT+QFDEL="};

uint8_t file_Connect_response[7]={"CONNECT"};


cm_q03_codes_t CM_Q03_File_Get_Space_Used(uint32_t *size_used, uint32_t *file_number)
{

    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[9] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[30];
    uint8_t result[10] = {0};
    uint16_t size_result = 0;
    uint16_t temp_result=0;
    Concatenate_Command(cmd_buff, file_space_information_used_of_ufs, sizeof(file_space_information_used_of_ufs), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    Delay(50000);
    CM_Q03_buffer_clean();
    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FILE_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);

    if(Timeout == status)
    {
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, file_space_information_response,
                                     sizeof(file_space_information_response), FILE_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result,
                                                                &size_result,' ', ',');

    *size_used = Ascii_to_Number(result, size_result);

    size_result=0;

    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result,
                                                                &size_result,'+', ',');
    size_result+=2;
    while(size_result < buffer_size){
        result[temp_result]=rxbuff[size_result];
        size_result++;
        temp_result++;
    }
    size_result=temp_result;

    *file_number = Ascii_to_Number(result, size_result);

    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FILE_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    return status;
}

cm_q03_codes_t CM_Q03_File_Get_Space_Information(uint32_t *freesize, uint32_t *total_size)
{

    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[15] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[30];
    uint8_t result[10] = {0};
    uint16_t size_result = 0;
    uint16_t temp_result=0;

    Concatenate_Command(cmd_buff, file_space_information_of_ufs, sizeof(file_space_information_of_ufs), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FILE_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);

    if(Timeout == status)
    {
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, file_space_information_response,
                                     sizeof(file_space_information_response), FILE_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result,
                                                                &size_result,' ', ',');

    *freesize = Ascii_to_Number(result, size_result);

    size_result=0;

    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result,
                                                                &size_result,'+', ',');
    size_result+=2;
    while(size_result < buffer_size){
        result[temp_result]=rxbuff[size_result];
        size_result++;
        temp_result++;
    }
    size_result=temp_result;

    *total_size = Ascii_to_Number(result, size_result);

    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FILE_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    return status;
}

cm_q03_codes_t CM_Q03_File_List_File (uint8_t *filename, uint8_t filename_size, uint32_t *file_size)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[267] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[30];
    uint8_t result[10] = {0};
    uint16_t size_result = 0;
    uint16_t temp_result=0;

    if (0 >= filename_size || 80 < filename_size)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, file_list_file_name, sizeof(file_list_file_name), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, filename, filename_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FILE_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);

    if(Timeout == status)
    {
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, file_list_files_response,
                                     sizeof(file_list_files_response), FILE_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result,
                                                                &size_result,'+', ',');
    size_result+=2;
    while(size_result < buffer_size)
    {
        result[temp_result]=rxbuff[size_result];
        size_result++;
        temp_result++;
    }
    size_result=temp_result;

    *file_size = Ascii_to_Number(result, size_result);

    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FILE_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    return status;
}

cm_q03_codes_t CM_Q03_File_Upload_File(uint8_t *filename, uint8_t filename_size,
										uint8_t *buffer_write,
										uint16_t buffer_write_size)
{

    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[285] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[25] = {0};
    uint8_t temp_buff[10] = {0};
    uint8_t timeout = 5;

    if (0 >= filename_size || 80 < filename_size)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, file_upload_file, sizeof(file_upload_file), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, filename, filename_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);

    Dec_to_Ascii(buffer_write_size, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    memset(temp_buff, '\0', 10);

    Dec_to_Ascii((uint16_t)timeout, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    memset(temp_buff, '\0', 10);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FILE_TIMEOUT);
    Delay(50000);
    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);

    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, file_Connect_response,
                                     sizeof(file_Connect_response), FILE_TIMEOUT);

    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    status = CM_Q03_Write( buffer_write, buffer_write_size, FILE_TIMEOUT);

    CM_Q03_Return_To_Last_Character_End();

    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, file_upload_file_response,
                                     sizeof(file_upload_file_response), FILE_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FILE_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    return status;
}

cm_q03_codes_t CM_Q03_File_Download_File (uint8_t  *filename, uint8_t filename_size,
											uint8_t *buffer_read,
											uint16_t *buffer_read_size)
{

    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[267] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[490];

    if (0 >= filename_size || 80 < filename_size)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, file_download_file, sizeof(file_download_file), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, filename, filename_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FILE_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);

    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, file_Connect_response,
                                     sizeof(file_Connect_response), FILE_TIMEOUT);

    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);

        if(ERROR == status)
        {
            return status+FILE_CODES;
        }

        else
        {
            return status+FILE_CODES;
        }
    }

    status = CM_Q03_Read_Response_Vector_Until(buffer_read, buffer_read_size,'\r',FILE_TIMEOUT);

    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, file_download_file_response,
                                     sizeof(file_download_file_response), FILE_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FILE_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }


    return status;

}

cm_q03_codes_t CM_Q03_File_Open_File(uint8_t  *filename, uint8_t filename_size,
									open_file_mode_t mode, uint32_t *filehandle)
{

    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[95] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[20];
    uint8_t temp_buff = 0;
    uint8_t result[3] = {0};
    uint16_t size_result = 0;
    uint16_t temp_result=0;

    if (0 >= filename_size || 80 < filename_size)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, file_open_file, sizeof(file_open_file), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);

    Concatenate_Command(cmd_buff, filename, filename_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);

    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(mode, &temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, &temp_buff, sizeof(temp_buff), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FILE_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);

    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, file_open_file_response,
                                     sizeof(file_open_file_response), FILE_TIMEOUT);

    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result,
                                                                &size_result,'+', ' ');
    size_result+=2;
    while(size_result < buffer_size){
        result[temp_result]=rxbuff[size_result];
        size_result++;
        temp_result++;
    }
    size_result=temp_result;

    *filehandle=Ascii_to_Number(result,size_result);

    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FILE_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    return status;
}

cm_q03_codes_t CM_Q03_File_Read_File(uint32_t filehandle, uint16_t length_read,
										uint8_t *buffer_read,
										uint16_t *buffer_read_size)
{

    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[25] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[490];
    uint8_t temp_buff[10] = {0};
    uint8_t read_length_ASCII[6] = {0};
    uint32_t read_length = 0;
    uint8_t counter = 0;

    Concatenate_Command(cmd_buff, file_read_file, sizeof(file_read_file), &cmd_buff_size);
    Dec_to_Ascii(filehandle, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, sizeof(temp_buff), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    memset(temp_buff, '\0', 10);

    Dec_to_Ascii(length_read, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, sizeof(temp_buff), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    memset(temp_buff, '\0', 10);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FILE_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);

    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, file_Connect_response,
                                     sizeof(file_Connect_response), FILE_TIMEOUT);

    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    read_length = buffer_size - CONNECT_SP_SIZE;

    for(counter = 0 ; counter < read_length ; counter ++)
    {
    	read_length_ASCII[counter] = rxbuff[counter + CONNECT_SP_SIZE];
    }

    read_length = Ascii_to_Number(read_length_ASCII, read_length);

    status = CM_Q03_Read_Response_Vector_N_Characters(read_length, buffer_read, buffer_read_size, 1000000);

    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FILE_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    return status;
}

cm_q03_codes_t CM_Q03_File_Write_File(uint32_t filehandle, uint8_t *buffer_write,
										uint16_t buffer_write_size)
{

    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[35] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[25];
    uint8_t temp_buff[10] = {0};

    Concatenate_Command(cmd_buff, file_write_file, sizeof(file_write_file), &cmd_buff_size);
    Dec_to_Ascii(filehandle, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, sizeof(temp_buff), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    memset(temp_buff, '\0', 10);
    Dec_to_Ascii(buffer_write_size, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, sizeof(temp_buff), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    memset(temp_buff, '\0', 10);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FILE_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);

    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, file_Connect_response,
                                     sizeof(file_Connect_response), FILE_TIMEOUT);

    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    status = CM_Q03_Write( buffer_write, buffer_write_size, FILE_TIMEOUT);
    CM_Q03_Delay(1000);

    CM_Q03_Return_To_Last_Character_End();

    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, file_write_file_response,
                                     sizeof(file_write_file_response), FILE_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FILE_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    return status;
}

cm_q03_codes_t CM_Q03_File_Set_Pointer_Position(uint32_t filehandle, uint16_t offset,
												start_position_t start_position)
{

    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[34] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15];
    uint8_t temp_buff[10] = {0};

    Concatenate_Command(cmd_buff, file_set_file_pointer, sizeof(file_set_file_pointer), &cmd_buff_size);
    Dec_to_Ascii(filehandle, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    memset(temp_buff, '\0', 10);
    Dec_to_Ascii(offset, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    memset(temp_buff, '\0', 10);
    Dec_to_Ascii(start_position, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    memset(temp_buff, '\0', 10);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FILE_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);

    if(Timeout == status)
    {
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FILE_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    return status;
}

cm_q03_codes_t CM_Q03_File_Get_Pointer_Position(uint32_t filehandle,
												uint16_t *offset_pointer)
{

    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[25] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[19];
    uint8_t temp_buff[10] = {0};
    uint8_t result[3] = {0};
    uint16_t size_result = 0;
    uint16_t temp_result=0;

    Concatenate_Command(cmd_buff, file_get_file_pointer, sizeof(file_get_file_pointer), &cmd_buff_size);
    Dec_to_Ascii(filehandle, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, sizeof(temp_buff), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    memset(temp_buff, '\0', 10);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FILE_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);

    if(Timeout == status)
    {
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, file_get_file_pointer_response,
                                     sizeof(file_get_file_pointer_response), FILE_TIMEOUT);

    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result,
                                                                &size_result,'+', ' ');
    size_result+=2;
    while(size_result < buffer_size){
        result[temp_result]=rxbuff[size_result];
        size_result++;
        temp_result++;
    }
    size_result=temp_result;

    *offset_pointer = Ascii_to_Number(result, size_result);

    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FILE_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    return status;
}

cm_q03_codes_t CM_Q03_File_Close_File(uint32_t filehandle)
{

    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[22] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15];
    uint8_t temp_buff[10] = {0};

    Concatenate_Command(cmd_buff, file_close_file, sizeof(file_close_file), &cmd_buff_size);
    Dec_to_Ascii(filehandle, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, sizeof(temp_buff), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    memset(temp_buff, '\0', 10);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FILE_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);

    if(Timeout == status)
    {
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FILE_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    return status;
}

cm_q03_codes_t CM_Q03_File_Delete_File(uint8_t *filename, uint16_t filename_size)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[93] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15];

    if (0 >= filename_size || 80 < filename_size)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, file_delete_file, sizeof(file_delete_file), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, filename, filename_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FILE_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FILE_TIMEOUT);

    if(Timeout == status)
     {
         return status;
     }

     status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                      sizeof(response_ok), FILE_TIMEOUT);
     if(Timeout == status)
     {
         return status;
     }
     else if(Success != status)
     {
         status = CM_Q03_Get_Response(rxbuff, buffer_size);
         return status+FILE_CODES;
     }

     return status;
}
