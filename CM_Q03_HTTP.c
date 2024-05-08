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
 * \file       CM_Q03_HTTP.h
 * \brief      This is the CM_Q03 HTTP driver made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.\n
 *             **to identify the meaning of a CME error, please refer to the
 *             provided Quectel HTTP(S) AT commands manual.\n
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

#include <CM_Q03_HTTP.h>

#define HTTP_TIMEOUT 1000000

/************** URC POS ************************/
#define QHTTPGET_POS			(66)
#define QHTTPREAD_POS			(67)
#define QHTTPREADFILE_POS		(68)
#define QHTTPPOST_POS			(69)
#define QHTTPPOSTFILE_POS		(70)
/************** URC POS ************************/

extern uint8_t response_ok[2];
extern uint8_t character_response_end;
extern uint8_t character_command_end;
extern uint8_t command_comma;
extern uint8_t command_quotes;

extern u16_3_t qhttp_get;
extern int16_t qhttp_read;
extern int16_t qhttp_readfile;
extern u16_3_t qhttp_post;
extern u16_3_t qhttp_postfile;
extern uint8_t urc_flags[URC_FLAG_SIZE];

enum HTTPS_codes{
	_OK = 200,
	_Forbidden = 403,
	_Not_Found,
	_Conflict = 409,
	_Length_Required = 411,
	_Internal_Server_Error = 500,
	_Unknown_error = 701,
    _Timeout,
	_Busy,
	_UART_busy,
	_No_GET_POST_requests,
	_Network_Busy,
	_Network_Open_Failed,
	_Network_No_Configuration,
	_Network_Deactived,
	_Network_Error,
	_URL_Error,
	_Empty_URL,
	_IP_Address_Error,
	_DNS_error,
	_Socket_Create_error,
	_Socket_Connect_error,
	_Socket_Write_Error,
	_Socket_Closed,
	_Data_Encode_Error,
	_Data_Decode_Error,
	_Read_Timeout,
	_Response_Failed,
	_Incomming_Call_Busy,
	_Voice_Call_Busy,
	_Input_Timeout,
	_Wait_Data_Timeout,
	_Wait_Response_Timeout,
	_Memory_Allocation_Failed,
	_Invalid_Parameter
};

uint8_t http_configurations[12] = "AT+QHTTPCFG=";
uint8_t http_contextid[11] = "\"contextid\"";
uint8_t http_requestheader[15] = "\"requestheader\"";
uint8_t http_responsetheader[16] = "\"responseheader\"";
uint8_t http_sslctxid[10] = "\"sslctxid\"";
uint8_t http_contenttype[13] = "\"contenttype\"";

uint8_t http_url[12] = "AT+QHTTPURL=";
uint8_t http_get[12] = "AT+QHTTPGET=";
uint8_t http_post[13] = "AT+QHTTPPOST=";
uint8_t http_post_file[17] = "AT+QHTTPPOSTFILE=";
uint8_t http_read[13] = "AT+QHTTPREAD=";
uint8_t http_read_file[17] = "AT+QHTTPREADFILE=";

uint8_t http_connect_response[7] = "CONNECT";
uint8_t http_get_response[11] = "+QHTTPGET: ";
uint8_t http_post_response[12] = "+QHTTPPOST: ";
uint8_t http_post_file_response[16] = "+QHTTPPOSTFILE: ";
uint8_t http_read_response[12] = "+QHTTPREAD: ";
uint8_t http_read_file_response[16] = "+QHTTPREADFILE: ";


cm_q03_codes_t CM_Q03_HTTP_Configure_Parameters(http_config_t http_config_struct,
												configuration_select_t get_default_config)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[31] = {0};
    uint8_t temp_buff[2] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};

    if (Default_Configuration == get_default_config)
    {
    	http_config_struct.context_id = 1;
    	http_config_struct.request_header = request_disable;
    	http_config_struct.response_header = response_disable;
    	http_config_struct.sslctxID = 1;
    	http_config_struct.data_type = x_www_form_urlencoded;
    }

    else
    {
        if (1 > http_config_struct.context_id || 16 < http_config_struct.context_id)
        {
            return User_Error;
        }
        if(0 > http_config_struct.request_header || 1 < http_config_struct.request_header)
        {
            return User_Error;
        }
        if(0 > http_config_struct.response_header || 1 < http_config_struct.response_header)
        {
            return User_Error;
        }
        if(0 > http_config_struct.sslctxID || 5 < http_config_struct.sslctxID)
        {
            return User_Error;
        }
        if(0 > http_config_struct.data_type || 3 < http_config_struct.data_type)
        {
            return User_Error;
        }
    }

    Concatenate_Command(cmd_buff, http_configurations, sizeof(http_configurations), &cmd_buff_size);
    Concatenate_Command(cmd_buff, http_contextid, sizeof(http_contextid), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(http_config_struct.context_id, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    CM_Q03_Delay(50000);
    CM_Q03_buffer_clean();
    status = CM_Q03_Write(cmd_buff, cmd_buff_size, HTTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, HTTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), HTTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + HTTPS_CODES;
    }


    memset(&cmd_buff[sizeof(http_configurations)], '\0', sizeof(cmd_buff)-sizeof(http_configurations));
    cmd_buff_size = sizeof(http_configurations);
    Concatenate_Command(cmd_buff, http_requestheader, sizeof(http_requestheader), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(http_config_struct.request_header, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    CM_Q03_Delay(50000);
    CM_Q03_buffer_clean();
    status = CM_Q03_Write(cmd_buff, cmd_buff_size, HTTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, HTTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), HTTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + HTTPS_CODES;
    }

    memset(&cmd_buff[sizeof(http_configurations)], '\0', sizeof(cmd_buff)-sizeof(http_configurations));
    cmd_buff_size = sizeof(http_configurations);
    Concatenate_Command(cmd_buff, http_responsetheader, sizeof(http_responsetheader), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(http_config_struct.response_header, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    CM_Q03_Delay(50000);
    CM_Q03_buffer_clean();
    status = CM_Q03_Write(cmd_buff, cmd_buff_size, HTTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, HTTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), HTTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + HTTPS_CODES;
    }

    memset(&cmd_buff[sizeof(http_configurations)], '\0', sizeof(cmd_buff)-sizeof(http_configurations));
    cmd_buff_size = sizeof(http_configurations);
    Concatenate_Command(cmd_buff, http_sslctxid, sizeof(http_sslctxid), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(http_config_struct.sslctxID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    CM_Q03_Delay(50000);
    CM_Q03_buffer_clean();
    status = CM_Q03_Write(cmd_buff, cmd_buff_size, HTTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, HTTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), HTTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + HTTPS_CODES;
    }

    memset(&cmd_buff[sizeof(http_configurations)], '\0', sizeof(cmd_buff)-sizeof(http_configurations));
    cmd_buff_size = sizeof(http_configurations);
    Concatenate_Command(cmd_buff, http_contenttype, sizeof(http_contenttype), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(http_config_struct.data_type, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    CM_Q03_Delay(50000);
    CM_Q03_buffer_clean();
    status = CM_Q03_Write(cmd_buff, cmd_buff_size, HTTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, HTTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), HTTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + HTTPS_CODES;
    }

    return status;
}

cm_q03_codes_t CM_Q03_HTTP_Set_URL(uint8_t *url, uint16_t url_lenght)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[113] = {0};
    uint8_t temp_buff[2] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};

    if ((NULL == url) || (0 == url_lenght))
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, http_url, sizeof(http_url), &cmd_buff_size);
    Dec_to_Ascii(url_lenght, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    CM_Q03_Delay(50000);
    CM_Q03_buffer_clean();
    status = CM_Q03_Write(cmd_buff, cmd_buff_size, HTTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, HTTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, http_connect_response,
                                     sizeof(http_connect_response), HTTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + HTTPS_CODES;
    }

    status = CM_Q03_Write(url, url_lenght, HTTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, HTTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), HTTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + HTTPS_CODES;
    }
    return status;
}

cm_q03_codes_t CM_Q03_HTTP_Send_GET(uint16_t rsptime, uint8_t *data, uint16_t data_length, uint16_t *content_length)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[23] = {0};
    uint8_t temp_buff[5] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint16_t count_rsptime=0;

    Concatenate_Command(cmd_buff, http_get, sizeof(http_get), &cmd_buff_size);
    Dec_to_Ascii(rsptime, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    if(NULL != data)
    {
        Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
        Dec_to_Ascii(data_length, temp_buff, &buffer_size);
        Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    }
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    CM_Q03_Delay(50000);
    CM_Q03_buffer_clean();
    status = CM_Q03_Write(cmd_buff, cmd_buff_size, HTTP_TIMEOUT);
    do
    {
        status = CM_Q03_Read(rxbuff, &buffer_size, HTTP_TIMEOUT);
        count_rsptime++;
    }while(Success != status && 125 > count_rsptime);
    if(Timeout == status)
    {
        return status;
    }
    count_rsptime=0;
    if(NULL != data)
    {
        status = CM_Q03_Compare_short_response(rxbuff, buffer_size, http_connect_response,
                                         sizeof(http_connect_response), HTTP_TIMEOUT);
        if (0 != status)
        {
            status = CM_Q03_Get_Response(rxbuff, buffer_size);
            return status + HTTPS_CODES;
        }
        status = CM_Q03_Write(data, data_length, HTTP_TIMEOUT);
        status = CM_Q03_Read(rxbuff, &buffer_size, HTTP_TIMEOUT);
        if(Timeout == status)
        {
            return status;
        }
    }

    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), HTTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + HTTPS_CODES;
    }

    CM_Q03_AP_Ready(1, 500);

    while(new_urc != CM_Q03_HTTP_Get_GET_Request_Status() && (rsptime+10)> count_rsptime)
    {
        CM_Q03_Delay(1000000);
        count_rsptime++;

    }
    count_rsptime=0;
    CM_Q03_HTTP_Clear_GET_Request_Flag();

    CM_Q03_AP_Ready(0, 500);

    if(Success == (cm_q03_codes_t)(qhttp_get.u16_0))
    {
        (*content_length) = (cm_q03_codes_t)(qhttp_get.u16_2);
        return (cm_q03_codes_t)(qhttp_get.u16_1) + HTTPS_CODES;
    }
    else
    {
        return (cm_q03_codes_t)(qhttp_get.u16_0) + HTTPS_CODES;
    }

//    return status;
}

cm_q03_codes_t CM_Q03_HTTP_Send_POST(uint8_t *data, uint32_t data_length,
									uint16_t rsptime, uint16_t *content_length)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[30] = {0};
    uint8_t temp_buff[7] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint8_t i = 0;

    /* AT+QHTTPPOST=data_size(bytes),input_body(seconds),response_time(seconds)*/
    Concatenate_Command(cmd_buff, http_post, sizeof(http_post), &cmd_buff_size);
    /* data_size */
    Dec_to_Ascii(data_length, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    /* input_body */
    Dec_to_Ascii(60, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    /* response_time */
    Dec_to_Ascii(rsptime, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    /* write command to bg96 */
    CM_Q03_Delay(50000);
    CM_Q03_buffer_clean();
    status = CM_Q03_Write(cmd_buff, cmd_buff_size, HTTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, 60000000);
    if(Timeout == status)
    {
    	return status;
    }

	status = CM_Q03_Compare_short_response(rxbuff, buffer_size, http_connect_response,
									 sizeof(http_connect_response), HTTP_TIMEOUT);
	if (0 != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status + HTTPS_CODES;
	}
	status = CM_Q03_Write(data, data_length, HTTP_TIMEOUT);
	status = CM_Q03_Read(rxbuff, &buffer_size, HTTP_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}

    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), HTTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + HTTPS_CODES;
    }

    CM_Q03_AP_Ready(1, 500);

    while(new_urc != CM_Q03_HTTP_Get_POST_Request_Status())
    {
    	CM_Q03_Delay(1000000);
    	if(i > 80)
    	{
    	    CM_Q03_HTTP_Clear_POST_Request_Flag();

    	    CM_Q03_AP_Ready(0, 500);
    		return Timeout;
    	}
    	else
    {
    		i++;
    	}
    }
    CM_Q03_HTTP_Clear_POST_Request_Flag();

    CM_Q03_AP_Ready(0, 500);

	if(Success == (cm_q03_codes_t)(qhttp_post.u16_0))
	{
		(*content_length) = (cm_q03_codes_t)(qhttp_post.u16_2);
		return (cm_q03_codes_t)(qhttp_post.u16_1) + HTTPS_CODES;
	}
	else
	{
		return (cm_q03_codes_t)(qhttp_post.u16_0) + HTTPS_CODES;
	}

    return status;
}

cm_q03_codes_t CM_Q03_HTTP_Send_POST_via_File(uint8_t *file_name, uint32_t file_name_length,
												uint16_t rsptime, uint16_t *content_length)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t temp_buff[5] = {0};
    uint8_t cmd_buff[106] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint16_t count_rsptime=0;


    Concatenate_Command(cmd_buff, http_post_file, sizeof(http_post_file), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, file_name, file_name_length, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(rsptime, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    CM_Q03_Delay(50000);
    CM_Q03_buffer_clean();
    status = CM_Q03_Write(cmd_buff, cmd_buff_size, HTTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, HTTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), HTTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + HTTPS_CODES;
    }

    CM_Q03_AP_Ready(1, 500);

    while(new_urc != CM_Q03_HTTP_Get_POST_File_Request_Status() && (rsptime+10)> count_rsptime)
    {
    	CM_Q03_Delay(1000000);
        count_rsptime++;

    }

    count_rsptime=0;



    CM_Q03_HTTP_Clear_POST_File_Request_Flag();

    CM_Q03_AP_Ready(0, 500);

	if(Success == (cm_q03_codes_t)(qhttp_postfile.u16_0))
	{
		(*content_length) = (cm_q03_codes_t)(qhttp_postfile.u16_2);
		return (cm_q03_codes_t)(qhttp_postfile.u16_1) + HTTPS_CODES;
	}
	else
	{
		return (cm_q03_codes_t)(qhttp_postfile.u16_0) + HTTPS_CODES;
	}

    return status;
}

cm_q03_codes_t CM_Q03_HTTP_Read(uint16_t wait_time, uint8_t *data, uint16_t *data_length)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[19] = {0};
    uint8_t temp_buff[5] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint16_t count_rsptime=0;


    Concatenate_Command(cmd_buff, http_read, sizeof(http_read), &cmd_buff_size);
    Dec_to_Ascii(wait_time, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    CM_Q03_Delay(50000);
    CM_Q03_buffer_clean();
    status = CM_Q03_Write(cmd_buff, cmd_buff_size, HTTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, HTTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }

	status = CM_Q03_Compare_short_response(rxbuff, buffer_size, http_connect_response,
									 sizeof(http_connect_response), HTTP_TIMEOUT);
	if (0 != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status + HTTPS_CODES;
	}

	status = CM_Q03_HTTP_Read_Until_OK(data, data_length, HTTP_TIMEOUT);

	if(ERROR == status)
	{
		status = CM_Q03_Get_Response(data, *data_length);
		memset(data, '\0', (*data_length));

		if(Timeout == status)
		{
			return status;
		}

		else
		{
			return status + HTTPS_CODES;
		}
	}

    CM_Q03_AP_Ready(1, 500);

    while(new_urc != CM_Q03_HTTP_Get_Read_Status()&& (wait_time+10)> count_rsptime)
    {
        CM_Q03_Delay(1000000);
        count_rsptime++;

    }

    count_rsptime=0;
    CM_Q03_HTTP_Clear_Read_Flag();

    CM_Q03_AP_Ready(0, 500);

    if(Success == (cm_q03_codes_t)(qhttp_read))
    {
    	return Success;
    }else
    {
    	return (cm_q03_codes_t)(qhttp_read) + HTTPS_CODES;
    }

}

cm_q03_codes_t CM_Q03_HTTP_Read_via_File(uint16_t wait_time,
									uint8_t *file_name, uint32_t file_name_length)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[150] = {0};
    uint8_t temp_buff[5] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint16_t count_rsptime=0;


    Concatenate_Command(cmd_buff, http_read_file, sizeof(http_read_file), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, file_name, file_name_length, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(wait_time, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    CM_Q03_Delay(50000);
    CM_Q03_buffer_clean();
    status = CM_Q03_Write(cmd_buff, cmd_buff_size, HTTP_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, HTTP_TIMEOUT);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), HTTP_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + HTTPS_CODES;
    }

    CM_Q03_AP_Ready(1, 500);

    while(new_urc != CM_Q03_HTTP_Get_Read_File_Status() && (wait_time+10)> count_rsptime)
    {
        CM_Q03_Delay(1000000);
        count_rsptime++;

    }

    count_rsptime=0;

    CM_Q03_HTTP_Clear_Read_File_Flag();

    CM_Q03_AP_Ready(0, 500);

    if(Success == (cm_q03_codes_t)(qhttp_readfile))
    {
    	return Success;
    }else
    {
    	return (cm_q03_codes_t)(qhttp_readfile) + HTTPS_CODES;
    }

    return status;
}

urc_received_status_t CM_Q03_HTTP_Get_GET_Request_Status(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QHTTPGET_POS)]
											 >> (MODULE_8_SFT(QHTTPGET_POS))) & 0x01);
}

void CM_Q03_HTTP_Clear_GET_Request_Flag(void)
{
	urc_flags[DIV_BY_8(QHTTPGET_POS)] &= ~(1 << MODULE_8_SFT(QHTTPGET_POS));
}

urc_received_status_t CM_Q03_HTTP_Get_POST_Request_Status(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QHTTPPOST_POS)]
											 >> (MODULE_8_SFT(QHTTPPOST_POS))) & 0x01);
}

void CM_Q03_HTTP_Clear_POST_Request_Flag(void)
{
	urc_flags[DIV_BY_8(QHTTPPOST_POS)] &= ~(1 << MODULE_8_SFT(QHTTPPOST_POS));
}

urc_received_status_t CM_Q03_HTTP_Get_POST_File_Request_Status(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QHTTPPOSTFILE_POS)]
											 >> (MODULE_8_SFT(QHTTPPOSTFILE_POS))) & 0x01);
}

void CM_Q03_HTTP_Clear_POST_File_Request_Flag(void)
{
	urc_flags[DIV_BY_8(QHTTPPOSTFILE_POS)] &= ~(1 << MODULE_8_SFT(QHTTPPOSTFILE_POS));
}

urc_received_status_t CM_Q03_HTTP_Get_Read_Status(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QHTTPREAD_POS)]
											 >> (MODULE_8_SFT(QHTTPREAD_POS))) & 0x01);
}

void CM_Q03_HTTP_Clear_Read_Flag(void)
{
	urc_flags[DIV_BY_8(QHTTPREAD_POS)] &= ~(1 << MODULE_8_SFT(QHTTPREAD_POS));
}

urc_received_status_t CM_Q03_HTTP_Get_Read_File_Status(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QHTTPREADFILE_POS)]
											 >> (MODULE_8_SFT(QHTTPREADFILE_POS))) & 0x01);
}

void CM_Q03_HTTP_Clear_Read_File_Flag(void)
{
	urc_flags[DIV_BY_8(QHTTPREADFILE_POS)] &= ~(1 << MODULE_8_SFT(QHTTPREADFILE_POS));
}

