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
 * \file       CM_Q03_FTP.h
 * \brief      This is the CM_Q03 FTP driver made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.\n
 *             **to identify the meaning of a CME error, please refer to the
 *             provided Quectel FTP(S) AT commands manual.\n
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

#include <CM_Q03_FTP.h>
#include "string.h"

/************** URC POS ************************/
#define QFTPCLOSE_POS				(22)
#define QFTPCWD_POS					(23)
#define QFTPDEL_POS					(24)
#define QFTPGET_POS					(25)
#define QFTPLEN_POS					(26)
#define QFTPMDTM_POS				(13)
#define QFTPMKDIR_POS				(27)
#define QFTPOPEN_POS				(28)
#define QFTPPUT_POS					(29)
#define QFTPPWD_POS					(14)
#define QFTPRENAME_POS				(30)
#define QFTPRMDIR_POS				(31)
#define QFTPSIZE_POS				(32)
#define QFTPSTAT_POS				(33)
/************** URC POS ************************/

#define FTP_TIMEOUT 				(1000000)
#define FTP_PROTOCOL_ERR_OFFSET		(193)

extern uint8_t character_command_end;
extern uint8_t command_comma;
extern uint8_t command_quotes;
extern uint8_t response_ok[2];
extern uint8_t g_interrupt_ri;
extern uint8_t mqtt_ctrl_z[2];

uint8_t ftp_configurations[11] = "AT+QFTPCFG=";
uint8_t ftp_filetype[10] = "\"filetype\"";
uint8_t ftp_transmode[11] = "\"transmode\"";
uint8_t ftp_contextid[11] = "\"contextid\"";
uint8_t ftp_rsptimeout[12] = "\"rsptimeout\"";
uint8_t ftp_ssltype[9] = "\"ssltype\"";
uint8_t ftp_sslctxid[10] = "\"sslctxid\"";
uint8_t ftp_data_address[14] = "\"data_address\"";

uint8_t ftp_account[9] = "\"account\"";

uint8_t ftp_open[12] = "AT+QFTPOPEN=";
uint8_t ftp_open_response[11] = "+QFTPOPEN: ";

uint8_t ftp_set_directory[11] = "AT+QFTPCWD=";
uint8_t ftp_set_directory_response[10] = "+QFTPCWD: ";

uint8_t ftp_get_directory[10] = "AT+QFTPPWD";
uint8_t ftp_get_directory_response[10] = "+QFTPPWD: ";

uint8_t ftp_upload_file[11] = "AT+QFTPPUT=";
uint8_t ftp_upload_file_response[10] = "+QFTPPUT: ";

uint8_t ftp_download_file[11] = "AT+QFTPGET=";
uint8_t ftp_download_file_response[10] = "+QFTPGET: ";

uint8_t ftp_file_size[12] = "AT+QFTPSIZE=";
uint8_t ftp_file_size_response[11] = "+QFTPSIZE: ";

uint8_t ftp_delete_file[11] = "AT+QFTPDEL=";
uint8_t ftp_delete_file_response[10] = "+QFTPDEL: ";

uint8_t ftp_create_folder[13] = "AT+QFTPMKDIR=";
uint8_t ftp_create_folder_response[12] = "+QFTPMKDIR: ";

uint8_t ftp_delete_folder[13] = "AT+QFTPRMDIR=";
uint8_t ftp_delete_folder_response[12] = "+QFTPRMDIR: ";

uint8_t ftp_get_modification_time[12] = "AT+QFTPMDTM=";
uint8_t ftp_get_modification_time_response[11] = "+QFTPMDTM: ";

uint8_t ftp_rename_file_folder[14] = "AT+QFTPRENAME=";
uint8_t ftp_rename_file_folder_response[13] = "+QFTPRENAME: ";

uint8_t ftp_get_lenght[10] = "AT+QFTPLEN";
uint8_t ftp_get_lenght_response[10] = "+QFTPLEN: ";

uint8_t ftp_get_status[11] = "AT+QFTPSTAT";
uint8_t ftp_get_status_response[11] = "+QFTPSTAT: ";

uint8_t ftp_close_server[12] = "AT+QFTPCLOSE";
uint8_t ftp_close_server_response[12] = "+QFTPCLOSE: ";

uint8_t ftp_timeout = 90;

extern u16_2_t qftpclose;
extern u16_2_t qftpcwd;
extern u16_2_t qftpdel;
extern u16_2_t qftpget;
extern u16_2_t qftplen;
extern u16_2_t qftpmkdir;
extern u16_2_t qftpopen;
extern u16_2_t qftpput;
extern u16_2_t qftprename;
extern u16_2_t qftprmdir;
extern u16_2_t qftpsize;
extern u16_2_t qftpstat;
extern uint8_t qftpmdtm[QFTPMDTM_SIZE];
extern uint8_t qftppwd[QFTPPWD_SIZE];
extern uint8_t urc_flags[URC_FLAG_SIZE];

extern volatile bool pit_channel_1_flag;

extern void CM_Q03_PIT_Initialization(void);
extern void CM_Q03_PIT_Start_Timer(uint32_t count_usec);
extern void CM_Q03_PIT_Stop_Timer(void);
extern volatile bool active_urc;


enum FTP_codes{
	_FTP_Opening_Server,
	_FTP_Server_open_and_idle,
	_FTP_Transferring_data_with_server,
	_FTP_Closing_server,
	_FTP_Server_is_close,
	_FTP_Unknown_error = 601,
	_FTP_server_blocked,
	_FTP_server_busy,
	_FTP_DNS_parse_failed,
	_FTP_Network_error,
	_FTP_Control_connection_closed,
	_FTP_Data_connection_closed,
	_FTP_Socket_closed_by_peer,
	_FTP_Timeout_error,
	_FTP_Invalid_parameter,
	_FTP_Failed_to_open_file,
	_FTP_File_position_invalid,
	_FTP_File_error,
	_FTP_Service_not_available_closing_control_connection,
	_FTP_Open_data_connection_failed,
	_FTP_Connection_closed_transfer_aborted,
	_FTP_Requested_file_action_not_taken,
	_FTP_Requested_action_aborted_local_error_in_processing,
	_FTP_Requested_action_not_taken_insufficient_system_storage,
	_FTP_Syntax_error_command_unrecognized,
	_FTP_Syntax_error_in_parameters_or_arguments,
	_FTP_Command_not_implemented,
	_FTP_Bad_sequence_of_commands,
	_FTP_Command_parameter_not_implemented,
	_FTP_not_logged_in,
	_FTP_Need_account_for_storing_file,
	_FTP_Requested_action_not_taken,
	_FTP_Requested_action_aborted_page_type_unknown,
	_FTP_Requested_file_action_aborted,
	_FTP_Requested_file_name_invalid,
	_FTP_SSL_authentication_failed
};

cm_q03_codes_t CM_Q03_FTP_Configure_Parameters(ftp_config_t ftp_config_struct,
										  	   configuration_select_t get_default_config){
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[30] = {0};
    uint8_t temp_buff[5] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};

    if (Default_Configuration == get_default_config)
    {
    	ftp_config_struct.file_type = binary;
    	ftp_config_struct.transmode = passive;
    	ftp_config_struct.contexid = 1;
    	ftp_config_struct.rsptimeout = 90;
    	ftp_config_struct.ssltype = FTP;
    	ftp_config_struct.sslctxid = 0;
    	ftp_config_struct.data_address = server_dispatched_address;
    }

    else
    {
        if (0 > ftp_config_struct.file_type || 1 < ftp_config_struct.file_type)
        {
            return User_Error;
        }
        if(0 > ftp_config_struct.transmode || 1 < ftp_config_struct.transmode)
        {
            return User_Error;
        }
        if(1 > ftp_config_struct.contexid || 3 < ftp_config_struct.contexid)
        {
            return User_Error;
        }
        if(20 > ftp_config_struct.rsptimeout || 180 < ftp_config_struct.rsptimeout)
        {
            return User_Error;
        }
        if(0 > ftp_config_struct.ssltype || 2 < ftp_config_struct.ssltype)
        {
            return User_Error;
        }
        if(0 > ftp_config_struct.sslctxid || 2 < ftp_config_struct.sslctxid)
        {
            return User_Error;
        }
        if(0 > ftp_config_struct.data_address || 1 < ftp_config_struct.data_address)
        {
            return User_Error;
        }
    }

    ftp_timeout = ftp_config_struct.rsptimeout;

    Concatenate_Command(cmd_buff, ftp_configurations, sizeof(ftp_configurations), &cmd_buff_size);
    Concatenate_Command(cmd_buff, ftp_filetype, sizeof(ftp_filetype), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(ftp_config_struct.file_type, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }


    memset(&cmd_buff[sizeof(ftp_configurations)], '\0', sizeof(cmd_buff)-sizeof(ftp_configurations));
    cmd_buff_size = sizeof(ftp_configurations);
    Concatenate_Command(cmd_buff, ftp_transmode, sizeof(ftp_transmode), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(ftp_config_struct.transmode, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }

    memset(&cmd_buff[sizeof(ftp_configurations)], '\0', sizeof(cmd_buff)-sizeof(ftp_configurations));
    cmd_buff_size = sizeof(ftp_configurations);
    Concatenate_Command(cmd_buff, ftp_contextid, sizeof(ftp_contextid), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(ftp_config_struct.contexid, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }

    memset(&cmd_buff[sizeof(ftp_configurations)], '\0', sizeof(cmd_buff)-sizeof(ftp_configurations));
    cmd_buff_size = sizeof(ftp_configurations);
    Concatenate_Command(cmd_buff, ftp_rsptimeout, sizeof(ftp_rsptimeout), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(ftp_config_struct.rsptimeout, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }

    memset(&cmd_buff[sizeof(ftp_configurations)], '\0', sizeof(cmd_buff)-sizeof(ftp_configurations));
    cmd_buff_size = sizeof(ftp_configurations);
    Concatenate_Command(cmd_buff, ftp_ssltype, sizeof(ftp_ssltype), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(ftp_config_struct.ssltype, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }

    memset(&cmd_buff[sizeof(ftp_configurations)], '\0', sizeof(cmd_buff)-sizeof(ftp_configurations));
    cmd_buff_size = sizeof(ftp_configurations);
    Concatenate_Command(cmd_buff, ftp_sslctxid, sizeof(ftp_sslctxid), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(ftp_config_struct.sslctxid, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }

    memset(&cmd_buff[sizeof(ftp_configurations)], '\0', sizeof(cmd_buff)-sizeof(ftp_configurations));
    cmd_buff_size = sizeof(ftp_configurations);
    Concatenate_Command(cmd_buff, ftp_data_address, sizeof(ftp_data_address), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(ftp_config_struct.data_address, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }

    return status;
}

cm_q03_codes_t CM_Q03_FTP_Configure_Account_Info(uint8_t *username, uint16_t username_size,
												 uint8_t *password, uint16_t password_size)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[537] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[50];

    if (0 > username_size || 255 < username_size)
    {
        return User_Error;
    }
    if (0 > password_size || 255 < password_size)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, ftp_configurations, sizeof(ftp_configurations), &cmd_buff_size);
    Concatenate_Command(cmd_buff, ftp_account, sizeof(ftp_account), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, username, username_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, password, password_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + FTP_CODES;
    }

    return status;
}

cm_q03_codes_t CM_Q03_FTP_Login_to_Server(uint8_t *hostname, uint16_t hostname_size, uint16_t port)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[221] = {0};
    uint8_t temp_buff[2] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[50] = {0};
//    uint8_t timeout_count = 0;
    cm_q03_codes_t code_return = 0;
//    uint8_t result[3] = {0};
//    uint16_t size_result = 0;
    urc_received_status_t status_flag=no_urc_received;
    uint8_t count_timeout=0;

    if (0 > hostname_size || 200 < hostname_size)
    {
        return User_Error;
    }

//    CM_Q01_Flush_URC();

    Concatenate_Command(cmd_buff, ftp_open, sizeof(ftp_open), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, hostname, hostname_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(port, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    CM_Q03_FTP_Clear_Login_to_Server_Status_Flag();
    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status + FTP_CODES;
	}

	CM_Q03_AP_Ready(1, 500);

    // Clean the PIT_1 flag
    pit_channel_1_flag = false;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer(1100000);

    do{
        status_flag=CM_Q03_FTP_Get_Login_to_Server_Status_Flag();

        if(pit_channel_1_flag && count_timeout < ftp_timeout){
            CM_Q03_PIT_Stop_Timer();
            pit_channel_1_flag = false;
            count_timeout++;
            CM_Q03_PIT_Start_Timer(1000000);
        }
        else if (pit_channel_1_flag && count_timeout == ftp_timeout){
            count_timeout++;
        }

    }while(count_timeout <= ftp_timeout && status_flag == no_urc_received);


    // Stop the count of the PIT_1
    CM_Q03_PIT_Stop_Timer();
    // Checks if the PIT_1 flag is active
    if(pit_channel_1_flag){
        // Deactivate the PIT_1 flag
        pit_channel_1_flag = false;
        // Return 1 to indicate an error
        return Timeout;
    }
    cm_q03_codes_t protocol_status;

    CM_Q03_FTP_Get_Login_to_Server_Status( &code_return,  &protocol_status);

//    CM_Q01_FTP_Clear_Login_to_Server_Status_Flag();

    CM_Q03_AP_Ready(0, 100);
    g_interrupt_ri = 0;
    CM_Q03_buffer_clean();

    return code_return;

//    status = Timeout;
//	while(ftp_timeout > (timeout_count++) && Timeout == status)
//	{
//		status = CM_Q01_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
//	}
//
//	if(Timeout == status)
//	{
//		return status;
//	}
//	else if(Success != status)
//	{
//		status = CM_Q01_Get_Response(rxbuff, buffer_size);
//		return status;
//	}
//
//	CM_Q01_AP_Ready(0, 500);
//
//	g_interrupt_ri = 0;
//	CM_Q01_buffer_clean();
//
//    status = CM_Q01_Compare_short_response(rxbuff, buffer_size, ftp_open_response,
//                                     sizeof(ftp_open_response), FTP_TIMEOUT);
//    if (0 != status)
//    {
//        status = CM_Q01_Get_Response(rxbuff, buffer_size);
//        return status;
//    }
//    status = CM_Q01_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result, &size_result,
//    															' ', ',');
//    code_return = Ascii_to_Code(result, size_result);
//
//    return code_return + FTP_CODES;

}

cm_q03_codes_t CM_Q03_FTP_Configure_Current_Directory(uint8_t *pathname, uint16_t pathname_size)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[269] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[50] = {0};
    uint8_t timeout_count = 0;
    uint16_t code_return = 0;
    uint8_t result[3] = {0};
    uint16_t size_result = 0;

    if (0 > pathname_size || 255 < pathname_size)
    {
        return User_Error;
    }

//    CM_Q01_Flush_URC();

    Concatenate_Command(cmd_buff, ftp_set_directory, sizeof(ftp_set_directory), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, pathname, pathname_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status + FTP_CODES;
	}

	CM_Q03_AP_Ready(1, 500);



//	status = Timeout;
//
//	while(ftp_timeout > (timeout_count++) && Timeout == status)
//	{
//		status = CM_Q01_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
//	}
//	if(Timeout == status)
//	{
//		return status;
//	}
//	else if(Success != status)
//	{
//		status = CM_Q01_Get_Response(rxbuff, buffer_size);
//		return status;
//	}
//	CM_Q01_AP_Ready(0, 500);
//	g_interrupt_ri = 0;
//	CM_Q01_buffer_clean();
//    status = CM_Q01_Compare_short_response(rxbuff, buffer_size, ftp_set_directory_response,
//                                     sizeof(ftp_set_directory_response), FTP_TIMEOUT);
//    if (0 != status)
//    {
//        status = CM_Q01_Get_Response(rxbuff, buffer_size);
//        return status;
//    }
//    status = CM_Q01_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result, &size_result,
//    																	' ', ',');
//    code_return = Ascii_to_Code(result, size_result);
//
//    if(code_return == Success)
//    {
//    	return Success;
//    }
//
//    return code_return + FTP_CODES;
}

cm_q03_codes_t CM_Q03_FTP_Get_Current_Directory(uint8_t *pathname, uint16_t *pathname_size)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[13] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[255] = {0};
    uint8_t timeout_count = 0;
    uint16_t code_return = 0;
    uint8_t result[3] = {0};
    uint16_t size_result = 0;
    uint8_t i = 2;

//    CM_Q01_Flush_URC();

    Concatenate_Command(cmd_buff, ftp_get_directory, sizeof(ftp_get_directory), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status + FTP_CODES;
	}

	CM_Q03_AP_Ready(1, 500);

	status = Timeout;

	while(ftp_timeout > (timeout_count++) && Timeout == status)
	{
		status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
	}
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}
	CM_Q03_AP_Ready(0, 500);
	g_interrupt_ri = 0;
	CM_Q03_buffer_clean();
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, ftp_get_directory_response,
                                     sizeof(ftp_get_directory_response), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }
    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result,
    															&size_result,' ', ',');
    code_return = Ascii_to_Code(result, size_result);
    if(Success == code_return)
    {
    	*pathname_size = buffer_size - sizeof(ftp_get_directory_response) + i;
    	while((sizeof(ftp_get_directory_response) + i) < buffer_size)
    	{
    		*pathname = rxbuff[sizeof(ftp_get_directory_response) + i];
    		pathname++;
    		i++;
    	}
    	return code_return;
    }

    return code_return + FTP_CODES;
}

cm_q03_codes_t CM_Q03_FTP_Upload_File(uint8_t *filename, uint16_t filename_size,
										uint8_t *localname, uint16_t localname_size)
{
	cm_q03_codes_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[331] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[50] = {0};
    uint8_t timeout_count = 0;
    cm_q03_codes_t code_return = 0;
    uint8_t result[3] = {0};
    uint16_t size_result = 0;

    uint8_t lname_string[4] = "UFS:";

    if (0 > filename_size || 255 < filename_size)
    {
        return User_Error;
    }

    if(0 > localname_size || 56 < localname_size)
    {
    	return User_Error;
    }

//    CM_Q01_Flush_URC();

    Concatenate_Command(cmd_buff, ftp_upload_file, sizeof(ftp_upload_file), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, filename, filename_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, lname_string, sizeof(lname_string), &cmd_buff_size);
    Concatenate_Command(cmd_buff, localname, localname_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);

    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status + FTP_CODES;
	}

	CM_Q03_AP_Ready(1, 500);

	status = Timeout;

	while(ftp_timeout > (timeout_count++) && Timeout == status)
	{
		status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
	}
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}
	CM_Q03_AP_Ready(0, 500);
	g_interrupt_ri = 0;
	CM_Q03_buffer_clean();
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, ftp_upload_file_response,
                                     sizeof(ftp_upload_file_response), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }
    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result,
    															&size_result,' ', ',');
    code_return = Ascii_to_Code(result, size_result);
    if(Success == code_return)
    {
    	return code_return;
    }
    return code_return + FTP_CODES;
}

cm_q03_codes_t CM_Q03_FTP_Create_File(uint8_t *filename, uint16_t filename_size,
										uint8_t *payload, uint16_t payload_size)
{
	uint8_t cmd_buff[283] = {0};
	uint16_t cmd_buff_size = 0;
	cm_q03_codes_t retval = Success;
	uint8_t rx_buff[50] = {0};
	uint16_t rx_buff_size = 0;

	uint8_t lname[4] = "COM:";
	uint8_t response_connect[7] = "CONNECT";
	uint16_t temp_index = 0;
    uint8_t timeout_count = 0;
    uint8_t result[3] = {0};
    uint16_t size_result = 0;

    uint8_t zero = '0';

    uint8_t file_length[5] = {'0'};
    uint16_t size_file_len = 0;

    if (0 > filename_size || 255 < filename_size)
    {
        return User_Error;
    }

//    CM_Q01_Flush_URC();

    Concatenate_Command(cmd_buff, ftp_upload_file, sizeof(ftp_upload_file), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, filename, filename_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, lname, sizeof(lname), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &zero, 1, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(payload_size, file_length, &size_file_len);
    Concatenate_Command(cmd_buff, file_length, size_file_len, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    retval = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);

    do
    {
    	retval = CM_Q03_Read(rx_buff, &rx_buff_size, 1000000);
    }while(ftp_timeout > (timeout_count++) && Timeout == retval);


    if(Timeout == retval)
    {
    	return retval;
    }

    retval = CM_Q03_Compare_response(rx_buff, rx_buff_size, response_connect, sizeof(response_connect),
    		sizeof(response_connect), &temp_index, 500000);

    if(Timeout == retval)
    {
    	return retval;
    }
    else if(Success != retval)
    {
    	retval = CM_Q03_Get_Response(rx_buff, rx_buff_size);
    	return retval + FTP_CODES;
    }

    retval = CM_Q03_Write(payload, payload_size, FTP_TIMEOUT);
    do
    {
    	retval = CM_Q03_Read(rx_buff, &rx_buff_size, FTP_TIMEOUT);
    }while(ftp_timeout > (timeout_count++) && Timeout == retval);
    retval = CM_Q03_Compare_response(rx_buff, rx_buff_size, response_ok, sizeof(response_ok), sizeof(response_ok), &temp_index, 500000);

    if(Timeout == retval)
    {
    	return retval;
    }
    else if(Success != retval)
    {
    	retval = CM_Q03_Get_Response(rx_buff, rx_buff_size);
    	return retval;
    }

	CM_Q03_AP_Ready(1, 500);

	retval = Timeout;

	while(ftp_timeout > (timeout_count++) && Timeout == retval)
	{
		retval = CM_Q03_Read(rx_buff, &rx_buff_size, FTP_TIMEOUT);
	}
	if(Timeout == retval)
	{
		return retval;
	}
	else if(Success != retval)
	{
		retval = CM_Q03_Get_Response(rx_buff, rx_buff_size);
		return retval;
	}
	CM_Q03_AP_Ready(0, 500);
	g_interrupt_ri = 0;
	CM_Q03_buffer_clean();
	retval = CM_Q03_Compare_short_response(rx_buff, rx_buff_size, ftp_upload_file_response,
                                     sizeof(ftp_upload_file_response), FTP_TIMEOUT);
    if (0 != retval)
    {
    	retval = CM_Q03_Get_Response(rx_buff, rx_buff_size);
        return retval;
    }
    retval = CM_Q03_Read_Response_Start_and_Stop_read_character(rx_buff, rx_buff_size, result,
    															&size_result,' ', ',');
    retval = Ascii_to_Code(result, size_result);
    if(Success == retval)
    {
    	return retval;
    }
    return retval + FTP_CODES;
}

cm_q03_codes_t CM_Q03_FTP_Download_File(uint8_t *filename, uint16_t filename_size,
										uint8_t *localname, uint16_t localname_size)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[331] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[50] = {0};
    uint8_t timeout_count = 0;
    uint16_t code_return = 0;
    uint8_t result[3] = {0};
    uint16_t size_result = 0;

    uint8_t lname_string[4] = "UFS:";

    if (0 > filename_size || 255 < filename_size)
    {
        return User_Error;
    }

    if(0 > localname_size || 56 < localname_size)
    {
    	return User_Error;
    }

//    CM_Q01_Flush_URC();

    Concatenate_Command(cmd_buff, ftp_download_file, sizeof(ftp_download_file), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, filename, filename_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, lname_string, sizeof(lname_string), &cmd_buff_size);
    Concatenate_Command(cmd_buff, localname, localname_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);

    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status + FTP_CODES;
	}

	CM_Q03_AP_Ready(1, 500);

	do
	{
		status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
	}while(ftp_timeout > (timeout_count++) && Timeout == status);

	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}

	CM_Q03_AP_Ready(0, 500);

	g_interrupt_ri = 0;
	CM_Q03_buffer_clean();

    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, ftp_download_file_response,
                                     sizeof(ftp_download_file_response), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }
    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result,
    															&size_result,' ', ',');
    code_return = Ascii_to_Code(result, size_result);
    if(Success == code_return)
    {
    	return code_return;
    }
    return code_return + FTP_CODES;
}

cm_q03_codes_t CM_Q03_FTP_Read_File(uint8_t *filename, uint16_t filename_size,
		uint8_t *payload, uint16_t *size_payload)
{
	uint8_t cmd_buff[275] = {0};
	uint16_t cmd_buff_size = 0;
	cm_q03_codes_t retval = Success;
	uint8_t rx_buff[50] = {0};
	uint16_t rx_buff_size = 0;

	uint8_t lname[4] = "COM:";
	uint8_t response_connect[7] = "CONNECT";
	uint16_t temp_index = 0;

	uint8_t timeout_count = 0;
	uint8_t result[3] = {0};
	uint16_t size_result = 0;

	if (0 > filename_size || 255 < filename_size)
	{
		return User_Error;
	}

	Concatenate_Command(cmd_buff, ftp_download_file, sizeof(ftp_download_file), &cmd_buff_size);
	Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
	Concatenate_Command(cmd_buff, filename, filename_size, &cmd_buff_size);
	Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
	Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
	Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
	Concatenate_Command(cmd_buff, lname, sizeof(lname), &cmd_buff_size);
	Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
	Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

	retval = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
	if(Timeout == retval)
	{
		return retval;
	}
	do
	{
		retval = CM_Q03_Read(rx_buff, &rx_buff_size, FTP_TIMEOUT);
	}while(ftp_timeout > (timeout_count++) && Timeout == retval);
	timeout_count = 0;
	if(Timeout == retval)
	{
		return retval;
	}

	retval = CM_Q03_Compare_response(rx_buff, rx_buff_size, response_connect, sizeof(response_connect),
			sizeof(response_connect), &temp_index, 500000);

	if(Timeout == retval)
	{
		return retval;
	}
	else if(Success != retval)
	{
		retval = CM_Q03_Get_Response(rx_buff, rx_buff_size);
		return retval + FTP_CODES;
	}

	retval = Timeout;

	while(ftp_timeout > (timeout_count++) && Timeout == retval)
	{
		retval = CM_Q03_Read_Response_Vector_Until(payload, size_payload, '\r', 5000000);
	}
	timeout_count = 0;
	if(Timeout == retval)
	{
		return retval;
	}

	retval = CM_Q03_Read(rx_buff, &rx_buff_size, FTP_TIMEOUT);

	retval = CM_Q03_Compare_response(rx_buff, rx_buff_size, response_ok, sizeof(response_ok),
			sizeof(response_ok), &temp_index, 500000);

	if(Timeout == retval)
	{
		return retval;
	}
	else if(Success != retval)
	{
		retval = CM_Q03_Get_Response(rx_buff, rx_buff_size);
		return retval;
	}


	CM_Q03_AP_Ready(1, 500);

	do
	{
		retval = CM_Q03_Read(rx_buff, &rx_buff_size, FTP_TIMEOUT);
	}while(ftp_timeout > (timeout_count++) && Timeout == retval);

	if(Timeout == retval)
	{
		return retval;
	}
	else if(Success != retval)
	{
		retval = CM_Q03_Get_Response(rx_buff, rx_buff_size);
		return retval;
	}

	CM_Q03_AP_Ready(0, 500);

	g_interrupt_ri = 0;


	retval = CM_Q03_Compare_short_response(rx_buff, rx_buff_size, ftp_download_file_response,
			sizeof(ftp_download_file_response), FTP_TIMEOUT);

	if (0 != retval)
	{
		retval = CM_Q03_Get_Response(rx_buff, rx_buff_size);
		return retval;
	}
	retval = CM_Q03_Read_Response_Start_and_Stop_read_character(rx_buff, rx_buff_size, result,
			&size_result,' ', ',');

	retval = Ascii_to_Code(result, size_result);
	if(Success == retval)
	{
		return retval;
	}
	return retval + FTP_CODES;
}

cm_q03_codes_t CM_Q03_FTP_Get_File_Size(uint8_t *filename, uint16_t filename_size, uint16_t *filesize)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[270] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[50] = {0};
    uint8_t timeout_count = 0;
    uint16_t code_return = 0;
    uint8_t result[3] = {0};
    uint16_t size_result = 0;
    uint8_t i = 2;

    if (0 > filename_size || 255 < filename_size)
    {
        return User_Error;
    }

//    CM_Q01_Flush_URC();

    Concatenate_Command(cmd_buff, ftp_file_size, sizeof(ftp_file_size), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, filename, filename_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}

	status = Timeout;

	CM_Q03_AP_Ready(1, 500);
	while(ftp_timeout > (timeout_count++) && Timeout == status)
	{
		status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
	}
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}
	CM_Q03_AP_Ready(0, 500);
	g_interrupt_ri = 0;

	status = CM_Q03_Compare_short_response(rxbuff, buffer_size, ftp_file_size_response,
                                     sizeof(ftp_file_size_response), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }
    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result,
    															&size_result,' ', ',');
    code_return = Ascii_to_Code(result, size_result);
    if(Success == code_return)
    {
    	size_result = buffer_size - (sizeof(ftp_file_size_response) + i);
    	while((sizeof(ftp_file_size_response) + i) < buffer_size)
    	{
    		result[i-2] = rxbuff[sizeof(ftp_file_size_response) + i];
    		i++;
    	}
    	*filesize = Ascii_to_Code(result, size_result);
    	return code_return;
    }
    return code_return + FTP_CODES;
}

cm_q03_codes_t CM_Q03_FTP_Delete_File(uint8_t *filename, uint16_t filename_size)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[269] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[50] = {0};
    uint8_t timeout_count = 0;
    uint16_t code_return = 0;
    uint8_t result[3] = {0};
    uint16_t size_result = 0;

    if (0 > filename_size || 255 < filename_size)
    {
        return User_Error;
    }

//    CM_Q01_Flush_URC();

    Concatenate_Command(cmd_buff, ftp_delete_file, sizeof(ftp_delete_file), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, filename, filename_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}

	CM_Q03_AP_Ready(1, 500);
	while(ftp_timeout > (timeout_count++) && Timeout == status)
	{
		status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
	}
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}
	CM_Q03_AP_Ready(0, 500);
	g_interrupt_ri = 0;
	CM_Q03_buffer_clean();
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, ftp_delete_file_response,
                                     	   sizeof(ftp_delete_file_response), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }
    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result,
    															&size_result,' ', ',');
    code_return = Ascii_to_Code(result, size_result);
    if(Success == code_return)
    {
    	return code_return;
    }
    return code_return + FTP_CODES;
}

cm_q03_codes_t CM_Q03_FTP_Create_Folder(uint8_t *foldername, uint16_t foldername_size)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[271] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[50] = {0};
    uint8_t timeout_count = 0;
    uint16_t code_return = 0;
    uint8_t result[3] = {0};
    uint16_t size_result = 0;

    if (0 > foldername_size || 255 < foldername_size)
    {
        return User_Error;
    }

//    CM_Q01_Flush_URC();

    Concatenate_Command(cmd_buff, ftp_create_folder, sizeof(ftp_create_folder), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, foldername, foldername_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}

	status = Timeout;

	CM_Q03_AP_Ready(1, 500);
	while(ftp_timeout > (timeout_count++) && Timeout == status)
	{
		status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
	}
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}
	CM_Q03_AP_Ready(0, 500);
	g_interrupt_ri = 0;
	CM_Q03_buffer_clean();
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, ftp_create_folder_response,
                                     	   sizeof(ftp_create_folder_response), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }
    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result,
    															&size_result,' ', ',');
    code_return = Ascii_to_Code(result, size_result);
    if(Success == code_return)
    {
    	return code_return;
    }
    return code_return + FTP_CODES;
}

cm_q03_codes_t CM_Q03_FTP_Delete_Folder(uint8_t *foldername, uint16_t foldername_size)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[271] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[50] = {0};
    uint8_t timeout_count = 0;
    uint16_t code_return = 0;
    uint8_t result[3] = {0};
    uint16_t size_result = 0;

    if (0 > foldername_size || 255 < foldername_size)
    {
        return User_Error;
    }

//    CM_Q01_Flush_URC();

    Concatenate_Command(cmd_buff, ftp_delete_folder, sizeof(ftp_delete_folder), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, foldername, foldername_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}

	CM_Q03_AP_Ready(1, 500);
	while(ftp_timeout > (timeout_count++) && Timeout == status)
	{
		status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
	}
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}
	CM_Q03_AP_Ready(0, 500);
	g_interrupt_ri = 0;
	CM_Q03_buffer_clean();
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, ftp_delete_folder_response,
                                     	   sizeof(ftp_delete_folder_response), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }
    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result,
    															&size_result,' ', ',');
    code_return = Ascii_to_Code(result, size_result);
    if(Success == code_return)
    {
    	return code_return;
    }
    return code_return + FTP_CODES;
}

cm_q03_codes_t CM_Q03_FTP_Get_File_Modification_Time(uint8_t *filename, uint16_t filename_size,
													 uint8_t *modifytime, uint16_t *modifytime_size)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[270] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[50] = {0};
    uint8_t timeout_count = 0;
    uint16_t code_return = 0;
    uint8_t result[3] = {0};
    uint16_t size_result = 0;
    uint8_t i = 2;

    if (0 > filename_size || 255 < filename_size)
    {
        return User_Error;
    }

//    CM_Q01_Flush_URC();

    Concatenate_Command(cmd_buff, ftp_get_modification_time, sizeof(ftp_get_modification_time), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, filename, filename_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status + FTP_CODES;
	}

	status = Timeout;

	CM_Q03_AP_Ready(1, 500);
	while(ftp_timeout > (timeout_count++) && Timeout == status)
	{
		status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
	}
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}
	CM_Q03_AP_Ready(0, 500);
	g_interrupt_ri = 0;
	CM_Q03_buffer_clean();
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, ftp_get_modification_time_response,
                                     	   sizeof(ftp_get_modification_time_response), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }
    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result,
    															&size_result,' ', ',');
    code_return = Ascii_to_Code(result, size_result);
    if(Success == code_return)
    {
    	*modifytime_size = buffer_size - sizeof(ftp_get_modification_time_response) + i;
    	while((sizeof(ftp_get_modification_time_response) + i) < buffer_size)
    	{
    		*modifytime = rxbuff[sizeof(ftp_get_modification_time_response) + i];
    		modifytime++;
    		i++;
    	}
    	return code_return;
    }
    return code_return + FTP_CODES;
}

cm_q03_codes_t CM_Q03_FTP_Rename_File_or_Folder(uint8_t *oldname, uint16_t oldname_size,
												uint8_t *newname, uint16_t newname_size)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[530] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[50] = {0};
    uint8_t timeout_count = 0;
    uint16_t code_return = 0;
    uint8_t result[3] = {0};
    uint16_t size_result = 0;

    if (0 > oldname_size || 255 < oldname_size)
    {
        return User_Error;
    }
    if (0 > newname_size || 255 < newname_size)
    {
        return User_Error;
    }

//    CM_Q01_Flush_URC();

    Concatenate_Command(cmd_buff, ftp_rename_file_folder, sizeof(ftp_rename_file_folder), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, oldname, oldname_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, newname, newname_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);

    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}

	status = Timeout;

	CM_Q03_AP_Ready(1, 500);
	while(ftp_timeout > (timeout_count++) && Timeout == status)
	{
		status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
	}
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}
	CM_Q03_AP_Ready(0, 500);
	g_interrupt_ri = 0;
	CM_Q03_buffer_clean();
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, ftp_rename_file_folder_response,
                                     	   sizeof(ftp_rename_file_folder_response), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }
    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result,
    															&size_result,' ', ',');
    code_return = Ascii_to_Code(result, size_result);
    if(Success == code_return)
    {
    	return code_return;
    }
    return code_return + FTP_CODES;
}

cm_q03_codes_t CM_Q03_FTP_Get_Lenght_Transferred_Data(uint16_t *transfer_length)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[13] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[50] = {0};
    uint8_t timeout_count = 0;
    uint16_t code_return = 0;
    uint8_t result[3] = {0};
    uint16_t size_result = 0;
    uint8_t i = 2;

//    CM_Q01_Flush_URC();
//	CM_Q01_buffer_clean();

    Concatenate_Command(cmd_buff, ftp_get_lenght, sizeof(ftp_get_lenght), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}

	status = Timeout;

	while(ftp_timeout > (timeout_count++) && Timeout == status)
	{
		status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
	}
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}

    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, ftp_get_lenght_response,
                                     	   sizeof(ftp_get_lenght_response), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }
    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result,
    															&size_result,' ', ',');
    code_return = Ascii_to_Code(result, size_result);
    if(Success == code_return)
    {
    	size_result = buffer_size - sizeof(ftp_get_lenght_response) + i;
    	while((sizeof(ftp_get_lenght_response) + i) < buffer_size)
    	{
    		result[i-2] = rxbuff[sizeof(ftp_get_lenght_response) + i];
    		i++;
    	}
    	*transfer_length = Ascii_to_Code(result, size_result);
    	return code_return;
    }
    return code_return + FTP_CODES;
}

cm_q03_codes_t CM_Q03_FTP_Get_Server_Status(void)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[13] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[50] = {0};
    uint8_t timeout_count = 0;
    uint16_t code_return = _FTP_Opening_Server;
    uint8_t i = 2;

//    CM_Q01_Flush_URC();

    Concatenate_Command(cmd_buff, ftp_get_status, sizeof(ftp_get_status), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}

    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }

	CM_Q03_AP_Ready(1, 500);
	while(ftp_timeout > (timeout_count++) && Timeout == status)
	{
		status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
	}
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}
	CM_Q03_AP_Ready(0, 500);
	g_interrupt_ri = 0;

	CM_Q03_buffer_clean();
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, ftp_get_status_response,
                                     	   sizeof(ftp_get_status_response), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }
	code_return += rxbuff[sizeof(ftp_get_status_response) + i] - ASCII_0;
    return code_return + FTP_CODES;
}

cm_q03_codes_t CM_Q03_FTP_Close_Server(void)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[14] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[50] = {0};
    uint8_t timeout_count = 0;
    uint16_t code_return = 0;
    uint8_t result[3] = {0};
    uint16_t size_result = 0;

//    CM_Q01_Flush_URC();

    Concatenate_Command(cmd_buff, ftp_close_server, sizeof(ftp_close_server), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, FTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), FTP_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}

	status = Timeout;

	CM_Q03_AP_Ready(1, 500);
	while(ftp_timeout > (timeout_count++) && Timeout == status)
	{
		status = CM_Q03_Read(rxbuff, &buffer_size, FTP_TIMEOUT);
	}
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}
	CM_Q03_AP_Ready(0, 500);
	g_interrupt_ri = 0;
	CM_Q03_buffer_clean();
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, ftp_close_server_response,
                                     	   sizeof(ftp_close_server_response), FTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }
    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, buffer_size, result,
    															&size_result,' ', ',');
    code_return = Ascii_to_Code(result, size_result);
    if(Success == code_return)
    {
    	return code_return;
    }
    return code_return + FTP_CODES;
}

urc_received_status_t CM_Q03_FTP_Get_Closed_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QFTPCLOSE_POS)]
											 >> (MODULE_8_SFT(QFTPCLOSE_POS))) & 0x01);
}

void CM_Q03_FTP_Clear_Closed_Status_Flag(void)
{
	urc_flags[DIV_BY_8(QFTPCLOSE_POS)] &= ~(1 << MODULE_8_SFT(QFTPCLOSE_POS));
}

void CM_Q03_FTP_Get_Closed_Status(cm_q03_codes_t *status, cm_q03_codes_t *protocol_status)
{
	if(Success == (cm_q03_codes_t)(qftpclose.u16_0))
	{
		(*status) = Success;
		(*protocol_status) = Success;
	}
	else
	{
		(*status) = (cm_q03_codes_t)(qftpclose.u16_0 + FTP_CODES);
		(*protocol_status) = (cm_q03_codes_t)(qftpclose.u16_1 + FTP_CODES
				+ FTP_PROTOCOL_ERR_OFFSET);
	}

	urc_flags[DIV_BY_8(QFTPCLOSE_POS)] &= ~(1 << MODULE_8_SFT(QFTPCLOSE_POS));
}

urc_received_status_t CM_Q03_FTP_Get_Configured_Directory_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QFTPCWD_POS)]
											 >> (MODULE_8_SFT(QFTPCWD_POS))) & 0x01);
}

void CM_Q03_FTP_Clear_Configured_Directory_Status_Flag(void)
{
	urc_flags[DIV_BY_8(QFTPCWD_POS)] &= ~(1 << MODULE_8_SFT(QFTPCWD_POS));
}

void CM_Q03_FTP_Get_Configured_Directory_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status)
{
	if(Success == (cm_q03_codes_t)(qftpcwd.u16_0))
	{
		(*status) = Success;
		(*protocol_status) = Success;
	}
	else
	{
		(*status) = (cm_q03_codes_t)(qftpcwd.u16_0 + FTP_CODES);
		(*protocol_status) = (cm_q03_codes_t)(qftpcwd.u16_1 + FTP_CODES
				+ FTP_PROTOCOL_ERR_OFFSET);
	}

	urc_flags[DIV_BY_8(QFTPCWD_POS)] &= ~(1 << MODULE_8_SFT(QFTPCWD_POS));
}

urc_received_status_t CM_Q03_FTP_Get_Deleted_File_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QFTPDEL_POS)]
											 >> (MODULE_8_SFT(QFTPDEL_POS))) & 0x01);
}

void CM_Q03_FTP_Clear_Deleted_File_Status_Flag(void)
{
	urc_flags[DIV_BY_8(QFTPDEL_POS)] &= ~(1 << MODULE_8_SFT(QFTPDEL_POS));
}

void CM_Q03_FTP_Get_Deleted_File_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status)
{
	if(Success == (cm_q03_codes_t)(qftpdel.u16_0))
	{
		(*status) = Success;
		(*protocol_status) = Success;
	}
	else
	{
		(*status) = (cm_q03_codes_t)(qftpdel.u16_0 + FTP_CODES);
		(*protocol_status) = (cm_q03_codes_t)(qftpdel.u16_1 + FTP_CODES
				+ FTP_PROTOCOL_ERR_OFFSET);
	}

	urc_flags[DIV_BY_8(QFTPDEL_POS)] &= ~(1 << MODULE_8_SFT(QFTPDEL_POS));
}

urc_received_status_t CM_Q03_FTP_Get_Download_File_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QFTPGET_POS)]
											 >> (MODULE_8_SFT(QFTPGET_POS))) & 0x01);
}

void CM_Q03_FTP_Clear_Download_File_Status_Flag(void)
{
	urc_flags[DIV_BY_8(QFTPGET_POS)] &= ~(1 << MODULE_8_SFT(QFTPGET_POS));
}

void CM_Q03_FTP_Get_Download_File_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status, uint16_t *transfer_len)
{
	if(Success == (cm_q03_codes_t)(qftpget.u16_0))
	{
		(*transfer_len) = qftpget.u16_1;
		(*status) = Success;
		(*protocol_status) = Success;
	}

	else
	{
		(*transfer_len) = 0;
		(*status) = (cm_q03_codes_t)(qftpget.u16_0 + FTP_CODES);
		(*protocol_status) = (cm_q03_codes_t)(qftpget.u16_1 + FTP_CODES
				+ FTP_PROTOCOL_ERR_OFFSET);
	}

	urc_flags[DIV_BY_8(QFTPGET_POS)] &= ~(1 << MODULE_8_SFT(QFTPGET_POS));
}

urc_received_status_t CM_Q03_FTP_Get_Transfer_Length_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QFTPLEN_POS)]
											 >> (MODULE_8_SFT(QFTPLEN_POS))) & 0x01);
}

void CM_Q03_FTP_Clear_Transfer_Length_Status_Flag(void)
{
	urc_flags[DIV_BY_8(QFTPLEN_POS)] &= ~(1 << MODULE_8_SFT(QFTPLEN_POS));
}

void CM_Q03_FTP_Get_Transfer_Length_Status(cm_q03_codes_t *status,
		uint16_t *transferlength)
{
	(*status) = (cm_q03_codes_t)(qftplen.u16_0);
	(*transferlength) = (uint16_t)(qftplen.u16_1);

	urc_flags[DIV_BY_8(QFTPLEN_POS)] &= ~(1 << MODULE_8_SFT(QFTPLEN_POS));
}

urc_received_status_t CM_Q03_FTP_Get_Modify_Time_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QFTPMDTM_POS)]
											 >> (MODULE_8_SFT(QFTPMDTM_POS))) & 0x01);
}

void CM_Q03_FTP_Clear_Modify_Time_Status_Flag(void)
{
	urc_flags[DIV_BY_8(QFTPMDTM_POS)] &= ~(1 << MODULE_8_SFT(QFTPMDTM_POS));
}

void _CM_Q03_FTP_Get_Modify_Time_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status, uint8_t* mod_time,
		uint16_t *mod_time_size)
{
	uint8_t cnt = 0;

	(*status) = Success;
	(*protocol_status) = Success;
	(*mod_time_size) = 0;

	if('0' == (uint8_t)(qftpmdtm[cnt]))
	{
		cnt += 3;

		while('"' != qftpmdtm[cnt])
		{
			(*mod_time) = qftpmdtm[cnt];
			mod_time ++;
			(*mod_time_size) ++;

			cnt ++;
		}
	}

	else
	{
		while(',' != (uint8_t)(qftpmdtm[cnt]))
		{
			(*status) = MULT_BY_10(*status) + (qftpmdtm[cnt] - '0');

			cnt ++;
		}

		cnt ++;

		while('\r' != (uint8_t)(qftpmdtm[cnt]))
		{
			(*status) = MULT_BY_10(*status) + (qftpmdtm[cnt] - '0');

			cnt ++;
		}

		(*status) += FTP_CODES;
		(*protocol_status) += FTP_CODES + FTP_PROTOCOL_ERR_OFFSET;
	}

	urc_flags[DIV_BY_8(QFTPMDTM_POS)] &= ~(1 << MODULE_8_SFT(QFTPMDTM_POS));
	memset(qftpmdtm, '\0', sizeof(qftpmdtm));
}

urc_received_status_t CM_Q03_FTP_Get_Create_Folder_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QFTPMKDIR_POS)]
											 >> (MODULE_8_SFT(QFTPMKDIR_POS))) & 0x01);
}

void CM_Q03_FTP_Clear_Create_Folder_Status_Flag(void)
{
	urc_flags[DIV_BY_8(QFTPMKDIR_POS)] &= ~(1 << MODULE_8_SFT(QFTPMKDIR_POS));
}

void CM_Q03_FTP_Get_Create_Folder_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status)
{
	if(Success == (cm_q03_codes_t)(qftpmkdir.u16_0))
	{
		(*status) = Success;
		(*protocol_status) = Success;
	}

	else
	{
		(*status) = (cm_q03_codes_t)(qftpmkdir.u16_0 + FTP_CODES);
		(*protocol_status) = (cm_q03_codes_t)(qftpmkdir.u16_1 + FTP_CODES
				+ FTP_PROTOCOL_ERR_OFFSET);
	}

	urc_flags[DIV_BY_8(QFTPMKDIR_POS)] &= ~(1 << MODULE_8_SFT(QFTPMKDIR_POS));
}

urc_received_status_t CM_Q03_FTP_Get_Login_to_Server_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QFTPOPEN_POS)]
											 >> (MODULE_8_SFT(QFTPOPEN_POS))) & 0x01);
}

void CM_Q03_FTP_Clear_Login_to_Server_Status_Flag(void)
{
	urc_flags[DIV_BY_8(QFTPOPEN_POS)] &= ~(1 << MODULE_8_SFT(QFTPOPEN_POS));
}

void CM_Q03_FTP_Get_Login_to_Server_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status)
{
	if(Success == (cm_q03_codes_t)(qftpopen.u16_0))
	{
		(*status) = FTP_Opening_Server;
		(*protocol_status) = FTP_Opening_Server;
	}

	else
	{
		(*status) = (cm_q03_codes_t)(qftpopen.u16_0 + FTP_CODES);
		(*protocol_status) = (cm_q03_codes_t)(qftpclose.u16_1 + FTP_CODES
				+ FTP_PROTOCOL_ERR_OFFSET);
	}

	urc_flags[DIV_BY_8(QFTPOPEN_POS)] &= ~(1 << MODULE_8_SFT(QFTPOPEN_POS));
}

urc_received_status_t CM_Q03_FTP_Get_Upload_File_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QFTPPUT_POS)]
										 >> (MODULE_8_SFT(QFTPPUT_POS))) & 0x01);
}

void CM_Q03_FTP_Clear_Upload_File_Status_Flag(void)
{
	urc_flags[DIV_BY_8(QFTPPUT_POS)] &= ~(1 << MODULE_8_SFT(QFTPPUT_POS));
}

void CM_Q03_FTP_Get_Upload_File_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status, uint16_t *transferlen)
{
	if(Success == (cm_q03_codes_t)(qftpput.u16_0))
	{
		(*status) = Success;
		(*protocol_status) = Success;
		(*transferlen) = (uint16_t)(qftpput.u16_1);
	}

	else
	{
		(*status) = (cm_q03_codes_t)(qftpput.u16_0 + FTP_CODES);
		(*protocol_status) = (cm_q03_codes_t)(qftpput.u16_1 + FTP_CODES
				+ FTP_PROTOCOL_ERR_OFFSET);
		(*transferlen) = 0;
	}

	urc_flags[DIV_BY_8(QFTPPUT_POS)] &= ~(1 << MODULE_8_SFT(QFTPPUT_POS));
}

urc_received_status_t CM_Q03_FTP_Get_Current_Directory_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QFTPPWD_POS)]
										 >> (MODULE_8_SFT(QFTPPWD_POS))) & 0x01);
}

void CM_Q03_FTP_Clear_Current_Directory_Status_Flag(void)
{
	urc_flags[DIV_BY_8(QFTPPWD_POS)] &= ~(1 << MODULE_8_SFT(QFTPPWD_POS));
}

void CM_Q03_FTP_Get_Current_Directory_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status, uint8_t *curr_path,
		uint16_t *curr_path_size)
{
	uint8_t cnt = 0;

	(*status) = Success;
	(*protocol_status) = Success;
	(*curr_path_size) = 0;

	if('0' == (uint8_t)(qftppwd[cnt]))
	{
		cnt += 3;

		while('"' != (uint8_t)(qftppwd[cnt]))
		{
			(*curr_path) = (uint8_t)(qftppwd[cnt]);
			curr_path ++;
			(*curr_path_size) ++;

			cnt ++;
		}
	}

	else
	{
		while(',' != (uint8_t)(qftppwd[cnt]))
		{
			(*status) = MULT_BY_10(*status) + (qftppwd[cnt] - '0');

			cnt ++;
		}

		cnt ++;

		while('\r' != (uint8_t)(qftppwd[cnt]))
		{
			(*status) = MULT_BY_10(*status) + (qftppwd[cnt] - '0');

			cnt ++;
		}

		(*status) += FTP_CODES;
		(*protocol_status) += FTP_CODES + FTP_PROTOCOL_ERR_OFFSET;
	}

	urc_flags[DIV_BY_8(QFTPPWD_POS)] &= ~(1 << MODULE_8_SFT(QFTPPWD_POS));
	memset(qftppwd, '\0', sizeof(qftppwd));
}

urc_received_status_t CM_Q03_FTP_Get_Rename_File_or_Folder_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QFTPRENAME_POS)]
										 >> (MODULE_8_SFT(QFTPRENAME_POS))) & 0x01);
}

void CM_Q03_FTP_Clear_Rename_File_or_Folder_Status_Flag(void)
{
	urc_flags[DIV_BY_8(QFTPRENAME_POS)] &= ~(1 << MODULE_8_SFT(QFTPRENAME_POS));
}

void CM_Q03_FTP_Get_Rename_File_or_Folder_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status)
{
	if(Success == (cm_q03_codes_t)(qftprename.u16_0))
	{
		(*status) = Success;
		(*protocol_status) = Success;
	}

	else
	{
		(*status) = (cm_q03_codes_t)(qftprename.u16_0 + FTP_CODES);
		(*protocol_status) = (cm_q03_codes_t)(qftprename.u16_1 + FTP_CODES
				+ FTP_PROTOCOL_ERR_OFFSET);
	}

	urc_flags[DIV_BY_8(QFTPRENAME_POS)] &= ~(1 << MODULE_8_SFT(QFTPRENAME_POS));
}

urc_received_status_t CM_Q03_FTP_Get_Delete_Folder_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QFTPRMDIR_POS)]
										 >> (MODULE_8_SFT(QFTPRMDIR_POS))) & 0x01);
}

void CM_Q03_FTP_Clear_Delete_Folder_Status_Flag(void)
{
	urc_flags[DIV_BY_8(QFTPRMDIR_POS)] &= ~(1 << MODULE_8_SFT(QFTPRMDIR_POS));
}

void CM_Q03_FTP_Get_Delete_Folder_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status)
{
	if(Success == (cm_q03_codes_t)(qftprmdir.u16_0))
	{
		(*status) = Success;
		(*protocol_status) = Success;
	}

	else
	{
		(*status) = (cm_q03_codes_t)(qftprmdir.u16_0 + FTP_CODES);
		(*protocol_status) = (cm_q03_codes_t)(qftprmdir.u16_1 + FTP_CODES
				+ FTP_PROTOCOL_ERR_OFFSET);
	}

	urc_flags[DIV_BY_8(QFTPRMDIR_POS)] &= ~(1 << MODULE_8_SFT(QFTPRMDIR_POS));
}

urc_received_status_t CM_Q03_FTP_Get_File_Size_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QFTPSIZE_POS)]
										 >> (MODULE_8_SFT(QFTPSIZE_POS))) & 0x01);
}

void CM_Q03_FTP_Clear_File_Size_Status_Flag(void)
{
	urc_flags[DIV_BY_8(QFTPSIZE_POS)] &= ~(1 << MODULE_8_SFT(QFTPSIZE_POS));
}

void CM_Q03_FTP_Get_File_Size_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status, uint16_t *file_size)
{
	if(Success == (cm_q03_codes_t)(qftpsize.u16_0))
	{
		(*status) = Success;
		(*protocol_status) = Success;
		(*file_size) = qftpsize.u16_1;
	}

	else
	{
		(*file_size) = 0;
		(*status) = (cm_q03_codes_t)(qftpsize.u16_0 + FTP_CODES);
		(*protocol_status) = (cm_q03_codes_t)(qftpsize.u16_1 + FTP_CODES
				+ FTP_PROTOCOL_ERR_OFFSET);
	}

	urc_flags[DIV_BY_8(QFTPSIZE_POS)] &= ~(1 << MODULE_8_SFT(QFTPSIZE_POS));
}

urc_received_status_t CM_Q03_FTP_Get_Server_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QFTPSTAT_POS)]
										 >> (MODULE_8_SFT(QFTPSTAT_POS))) & 0x01);
}

void CM_Q03_FTP_Clear_Server_Status_Flag(void)
{
	urc_flags[DIV_BY_8(QFTPSTAT_POS)] &= ~(1 << MODULE_8_SFT(QFTPSTAT_POS));
}

void CM_Q03_FTP_Retreive_Server_Status(cm_q03_codes_t *status, uint16_t *ftp_stat)
{
	(*status) = (cm_q03_codes_t)(qftpstat.u16_0);
	(*ftp_stat) = qftpstat.u16_1;

	urc_flags[DIV_BY_8(QFTPSTAT_POS)] &= ~(1 << MODULE_8_SFT(QFTPSTAT_POS));
}

