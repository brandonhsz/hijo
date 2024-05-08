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
 * \file       CM_Q03_TCP_IP.h
 * \brief      This is the CM_Q03 TCP/IP drover made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.\n
 *             **to identify the meaning of a CME error, please refer to the
 *             provided Quectel TCP(IP) AT commands manual.\n
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

#include <CM_Q03.h>
#include <CM_Q03_TCP_IP.h>
#include <stdlib.h>

/************** URC POS ************************/
#define QIGETERROR_POS				(34)
#define QIOPEN_CID_0_POS			(35)
#define QIOPEN_CID_1_POS			(36)
#define QIOPEN_CID_2_POS			(37)
#define QISEND_POS					(38)
#define QISTATE_CID_0_POS			(73)
#define QISTATE_CID_1_POS			(74)
#define QISTATE_CID_2_POS			(75)
#define QIURC_INCOMING_FULL_POS		(6)
#define QIURC_INCOMINC_CID_0_POS	(39)
#define QIURC_INCOMINC_CID_1_POS	(40)
#define QIURC_INCOMINC_CID_2_POS	(41)
#define QIURC_PDPDEACT_CID_0_POS	(7)
#define QIURC_PDPDEACT_CID_1_POS	(8)
#define QIURC_PDPDEACT_CID_2_POS	(9)
#define QIURC_RECV_CID_0_POS		(10)
#define QIURC_RECV_CID_1_POS		(11)
#define QIURC_RECV_CID_2_POS		(12)
#define QIURC_CLOSED_CID_0_POS		(3)
#define QIURC_CLOSED_CID_1_POS		(4)
#define QIURC_CLOSED_CID_2_POS		(5)
/************** URC POS ************************/

#define TCP_IP_SOCKET_STATE_OFFSET	(4)

extern uint8_t response_ok[2]; //se puso const en la libreria de aqui
extern uint8_t character_response_end;
extern uint8_t character_command_end;
extern uint8_t command_comma;
extern uint8_t command_quotes;

extern u16_2_t qigeterror;
extern int16_t qiopen_cid_0;
extern int16_t qiopen_cid_1;
extern int16_t qiopen_cid_2;
extern u16_3_t qisend;
extern u16_12_t qistate_cid_0;
extern u16_12_t qistate_cid_1;
extern u16_12_t qistate_cid_2;
extern no_params_flags_t no_param_urc;
extern u16_6_t qiurc_incoming_cid_0;
extern u16_6_t qiurc_incoming_cid_1;
extern u16_6_t qiurc_incoming_cid_2;
extern uint8_t urc_flags[URC_FLAG_SIZE];


uint8_t tcp_ip_config_parameters[10]={"AT+QICSGP="};//modificacion mia
uint8_t tcp_ip_activate_pdp_context[9]={"AT+QIACT="};
uint8_t tcp_ip_deactivate_pdp_context[11]={"AT+QIDEACT="};
uint8_t tcp_ip_current_pdp_context[9]={"AT+QIACT?"};

uint8_t tcp_ip_current_pdp_response[9]={"+QIACT: X"};


uint8_t tcp_ip_open_socket[10]={"AT+QIOPEN="};


uint8_t tcp_service[5] = "\"TCP\"";
uint8_t udp_service[5] = "\"UDP\"";
uint8_t tcp_listener_service[14] = "\"TCP LISTENER\"";
uint8_t udp_service_service[13] = "\"UDP SERVICE\"";

 uint8_t tcp_ip_close_socket[12]={"AT+QICLOSE=?"};

 uint8_t tcp_ip_query_socket_status[13]={"AT+QISTATE=1,"};
 uint8_t tcp_ip_query_socket_return[10]={"+QISTATE: "};

uint8_t tcp_ip_send_hex_string[12]={"AT+QISENDEX="};

//response
uint8_t tcp_ip_send_response_ok[7]={"SEND OK"};
 uint8_t tcp_ip_send_response_fail[9]={"SEND FAIL"};

 uint8_t tcp_ip_send_string[10]={"AT+QISEND="};
uint8_t tcp_ip_send_string_response = '>';

uint8_t tcp_ip_retrive_received[8]={"AT+QIRD="};
uint8_t tcp_ip_retrive_received_response[8]={"+QIRD: 0"};


 uint8_t tcp_ip_open_successful[9]={"+QIOPEN: "};

cm_q03_codes_t CM_Q03_TCP_IP_Get_Error();

enum TCP_IP_codes
{
	_TCP_IP_Service_opened_successfully,
	_TCP_IP_PDP_Deactivated,
	_TCP_IP_PDP_Activated,
	_TCP_IP_No_data_to_read,
	_TCP_IP_Socket_Initial,
	_TCP_IP_Socket_Opening,
	_TCP_IP_Socket_Connected,
	_TCP_IP_Socket_Listening,
	_TCP_IP_Socket_Closing,
	_TCP_IP_Send_Fail,
	_TCP_IP_Unknown_error = 550,
	_TCP_IP_Operation_blocked,
	_TCP_IP_Invalid_parameters,
	_TCP_IP_Memory_not_enough,
	_TCP_IP_Create_socket_failed,
	_TCP_IP_Operation_not_supported,
	_TCP_IP_Socket_bind_failed,
	_TCP_IP_Socket_listen_failed,
	_TCP_IP_Socket_write_failed,
	_TCP_IP_Socket_read_failed,
	_TCP_IP_Socket_accept_failed,
	_TCP_IP_Open_PDP_context_failed,
	_TCP_IP_Close_PDP_context_failed,
	_TCP_IP_Socket_identify_has_been_used,
	_TCP_IP_DNS_busy,
	_TCP_IP_DNS_parse_failed,
	_TCP_IP_Socket_connect_failed,
	_TCP_IP_Socket_has_been_closed,
	_TCP_IP_Operation_busy,
	_TCP_IP_Operation_timeout,
	_TCP_IP_PDP_context_broken_down,
	_TCP_IP_Cancel_Send,
	_TCP_IP_Operation_not_allowed,
	_TCP_IP_APN_not_configured,
	_TCP_IP_Port_busy

};

extern uint16_t g_count_read;
extern uint16_t g_count_valid_data;

cm_q03_codes_t CM_Q03_TCP_IP_Config_Context_APN(uint8_t *manual_apn, uint16_t size_of_apn,
                                         uint8_t *username, uint16_t size_of_username,
                                         uint8_t *password, uint16_t size_of_password,
                                         tcp_ip_apn_parameters_struct_t parameters_apn_tcp_ip)
{
    uint16_t status = 0;
    uint16_t temp_index = 0;
    uint8_t temp_buff = 0;
    uint16_t temp_buffer_size = 0;
    uint8_t cmd_buff[790] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15];
    uint16_t rxbuffer_size ;

    if (1 > parameters_apn_tcp_ip.context_id || 3 < parameters_apn_tcp_ip.context_id)
    {
        return User_Error;
    }
    if (1 > parameters_apn_tcp_ip.context_type || 3 < parameters_apn_tcp_ip.context_type)
    {
        return User_Error;
    }
    if (0 > parameters_apn_tcp_ip.authentication || 3 < parameters_apn_tcp_ip.authentication)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, tcp_ip_config_parameters, sizeof(tcp_ip_config_parameters), &cmd_buff_size);
    Dec_to_Ascii(parameters_apn_tcp_ip.context_id , &temp_buff, &temp_buffer_size);
    Concatenate_Command(cmd_buff, &temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(parameters_apn_tcp_ip.context_type  , &temp_buff, &temp_buffer_size);
    Concatenate_Command(cmd_buff, &temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, manual_apn, size_of_apn, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, username, size_of_username, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, password, size_of_password, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(parameters_apn_tcp_ip.authentication, &temp_buff, &temp_buffer_size);
    Concatenate_Command(cmd_buff, &temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_response(rxbuff, rxbuffer_size, response_ok, sizeof(response_ok), sizeof(response_ok), &temp_index, 500000);

    if(Timeout == status)
    {
        return status;
    }
    else if(0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        if(ERROR == status)
        {
        	status = CM_Q03_TCP_IP_Get_Error();
        }
        return status;
    }

    return Success;
}

cm_q03_codes_t CM_Q03_TCP_IP_Activate_PDP(uint8_t contextid)
{
    uint16_t status = 0;
    uint16_t temp_index = 0;
    uint8_t temp_buff = 0;
    uint16_t temp_buffer_size = 0;
    uint8_t cmd_buff[11] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint16_t rxbuffer_size ;

    if( 1 <= contextid && 3 >= contextid)
    {
        Concatenate_Command(cmd_buff, tcp_ip_activate_pdp_context, sizeof(tcp_ip_activate_pdp_context), &cmd_buff_size);
        Dec_to_Ascii(contextid , &temp_buff, &temp_buffer_size);
        Concatenate_Command(cmd_buff, &temp_buff, temp_buffer_size, &cmd_buff_size);

        Delay(50000);
        CM_Q03_buffer_clean();

        Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
        status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
        if(Timeout == status)
        {
            return status;
        }
        status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
        if(Timeout == status)
        {
            status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
            if(Timeout == status)
            {
                status = CM_Q03_Read(rxbuff, &rxbuffer_size, 30000000);
                if(Timeout == status)
                {
                    return status;
                }
            }
        }
        status = CM_Q03_Compare_response(rxbuff, rxbuffer_size, response_ok, sizeof(response_ok), sizeof(response_ok), &temp_index, 500000);
        if(Timeout == status)
        {
            return status;
        }
        else if(0 != status)
        {
            status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
            if(ERROR == status)
            {
            	status = CM_Q03_TCP_IP_Get_Error();
            }
            return status;
        }
    }
    else
    {
        return User_Error;
    }
    return status;
}

cm_q03_codes_t CM_Q03_TCP_IP_Deactivate_PDP(uint8_t contextid)
{
    uint16_t status = 0;
    uint16_t temp_index = 0;
    uint8_t temp_buff[2] = {0};
    uint16_t temp_buffer_size = 0;
    uint8_t cmd_buff[13] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint16_t rxbuffer_size;

    if( 1 <= contextid && 3 >= contextid)
    {
        Concatenate_Command(cmd_buff, tcp_ip_deactivate_pdp_context, sizeof(tcp_ip_deactivate_pdp_context), &cmd_buff_size);
        Dec_to_Ascii(contextid , temp_buff, &temp_buffer_size);
        Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
        Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

        Delay(50000);
        CM_Q03_buffer_clean();

        status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
        if(Timeout == status)
        {
            return status;
        }
        status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
        if(Timeout == status)
        {
            return status;
        }
        status = CM_Q03_Compare_response(rxbuff, rxbuffer_size, response_ok, sizeof(response_ok), sizeof(response_ok), &temp_index, 500000);
        if(Timeout == status)
        {
            return status;
        }
        else if(0 != status)
        {
            status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
            if(ERROR == status)
            {
            	status = CM_Q03_TCP_IP_Get_Error();
            }
            return status;
        }

    }
    else
    {
        return User_Error;
    }
    return status;
}

cm_q03_codes_t CM_Q03_TCP_IP_Current_Activated_PDP(uint8_t contextid)
{
    uint16_t status = 0;
    uint16_t temp_index = 0;
    uint8_t cmd_buff[10] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[30] = {0};
    uint16_t rxbuffer_size;
    uint16_t code_return = _TCP_IP_PDP_Deactivated;

    if( 1 <= contextid && 3 >= contextid)
    {
		Concatenate_Command(cmd_buff, tcp_ip_current_pdp_context, sizeof(tcp_ip_current_pdp_context), &cmd_buff_size);
		Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

		Delay(50000);
	    CM_Q03_buffer_clean();

		status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
		if(Timeout == status)
		{
			return status;
		}
		status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
		if(Timeout == status)
		{
			return status;
		}
		while(CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, response_ok, sizeof(response_ok), 500000))
		{
			status = CM_Q03_Compare_response(rxbuff, rxbuffer_size, tcp_ip_current_pdp_response, sizeof(tcp_ip_current_pdp_response), sizeof(tcp_ip_current_pdp_response)-1, &temp_index, 500000);
			if((rxbuff[temp_index]-ASCII_0) == contextid)
			{
				temp_index += 2;
				code_return += rxbuff[temp_index]-ASCII_0;
			}
			status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
			if(Timeout == status)
			{
				return status;
			}
		}
		if(Timeout == status)
		{
			return status;
		}
    }
    else{
        return User_Error;
    }
    return code_return + TCP_IP_CODES;
}

cm_q03_codes_t CM_Q03_TCP_Open_Socket(service_type_t service_type,
								uint8_t *ip_address, uint16_t size_ip_address,
								tcp_ip_open_socket_parameters_struct_t open_parameters)
{
    uint16_t status = 0;
    uint16_t temp_index = 0;
    uint8_t temp_buff[5] = {0};
    uint16_t temp_buffer_size = 0;
    uint8_t cmd_buff[292] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint16_t rxbuffer_size;
    uint16_t code_return = _TCP_IP_Service_opened_successfully;

    uint8_t* service = NULL;
    uint16_t service_size = 0;

    if (1 > open_parameters.context_id || 3 < open_parameters.context_id)
    {
        return User_Error;
    }
    if (0 > open_parameters.connect_id || 2 < open_parameters.connect_id)
    {
        return User_Error;
    }
    if (1 > size_ip_address || 255 < size_ip_address)
    {
        return User_Error;
    }

    if(TCP == service_type)
    {
    	service = tcp_service;
    	service_size = sizeof(tcp_service);
    }

    else if(UDP == service_type)
    {
    	service = udp_service;
    	service_size = sizeof(udp_service);
    }

    else if(TCP_listener == service_type)
    {
    	service = tcp_listener_service;
    	service_size = sizeof(tcp_listener_service);
    }

    else if(UDP_service == service_type)
    {
    	service = udp_service_service;
    	service_size = sizeof(udp_service_service);
    }

    else
    {
    	return User_Error;
    }

    CM_Q03_Flush_URC();

    Concatenate_Command(cmd_buff, tcp_ip_open_socket, sizeof(tcp_ip_open_socket), &cmd_buff_size);
    Dec_to_Ascii(open_parameters.context_id , temp_buff, &temp_buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(open_parameters.connect_id , temp_buff, &temp_buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, service, service_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, ip_address,size_ip_address, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(open_parameters.remote_port , temp_buff, &temp_buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(open_parameters.local_port , temp_buff, &temp_buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
	Dec_to_Ascii(0 , temp_buff, &temp_buffer_size);
	Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_response(rxbuff, rxbuffer_size, response_ok, sizeof(response_ok), sizeof(response_ok), &temp_index, 500000);
    if(Timeout == status)
    {
        return status;
    }
    else if(0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        if(ERROR == status)
        {
        	status = CM_Q03_TCP_IP_Get_Error();
        }
        return status;
    }
	CM_Q03_AP_Ready(1, 500);
    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
    if(Timeout == status)
    {
        status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
        if(Timeout == status)
        {
            status = CM_Q03_Read(rxbuff, &rxbuffer_size, 3000000);
            if(Timeout == status)
            {
            	CM_Q03_AP_Ready(0, 500);
                return status;
            }
        }
    }
	CM_Q03_AP_Ready(0, 500);
	CM_Q03_buffer_clean();
    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, tcp_ip_open_successful, sizeof(tcp_ip_open_successful), 500000);
    if(Timeout == status)
    {
        return status;
    }
    else if(0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        if(ERROR == status)
        {
        	status = CM_Q03_TCP_IP_Get_Error();
        }
        return status;
    }
    code_return += Ascii_to_Code(&rxbuff[sizeof(tcp_ip_open_successful)+2], rxbuffer_size-(sizeof(tcp_ip_open_successful)+2));
    return code_return + TCP_IP_CODES;
}

cm_q03_codes_t CM_Q03_TCP_IP_Close_Socket(uint8_t connectid, uint16_t timeout)
{
    uint16_t status = 0;
    uint8_t temp_buff[5] = {0};
    uint16_t temp_buffer_size = 0;
    uint8_t cmd_buff[20] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint16_t rxbuffer_size;
    uint8_t i_timeout = 1;

    if (1 > timeout)
    {
        return User_Error;
    }
    if (0 > connectid || 2 < connectid)
    {
        return User_Error;
    }

    CM_Q03_buffer_clean();

    Concatenate_Command(cmd_buff, tcp_ip_close_socket, sizeof(tcp_ip_close_socket), &cmd_buff_size);
    Dec_to_Ascii(connectid, temp_buff, &temp_buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(timeout, temp_buff, &temp_buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
    if(Timeout == status)
    {
        return status;
    }
    do
    {
		status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
    }while((i_timeout++ < timeout) && Timeout == status);
    if(Timeout == status)
    {
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, response_ok, sizeof(response_ok), 500000);

    if(Timeout == status)
    {
        return status;
    }
    else if(0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        if(ERROR == status)
        {
        	status = CM_Q03_TCP_IP_Get_Error();
        }
        return status;
    }
    return Success;
}

cm_q03_codes_t CM_Q03_TCP_IP_Query_Socket_Status(uint8_t connectid)
{

    uint16_t status = 0;
    uint8_t cmd_buff[15] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[64] = {0};
    uint16_t rxbuffer_size ;
    uint16_t temp_buffer_size = 0;
    uint8_t temp_buff = 0;
    uint16_t code_return = _TCP_IP_Socket_Initial;

    if (0 > connectid || 2 < connectid)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, tcp_ip_query_socket_status, sizeof(tcp_ip_query_socket_status), &cmd_buff_size);
    Dec_to_Ascii(connectid, &temp_buff, &temp_buffer_size);
    Concatenate_Command(cmd_buff, &temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, tcp_ip_query_socket_return, sizeof(tcp_ip_query_socket_return), 500000);
    if(Timeout == status)
    {
        return status;
    }
    else if(0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        if(ERROR == status)
        {
        	status = CM_Q03_TCP_IP_Get_Error();
        }
        return status;
    }
    temp_buffer_size = 0;
    status = CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff, rxbuffer_size,
    																  cmd_buff, &temp_buffer_size,
																	  ',', 5, ',');
    code_return += Ascii_to_Code(cmd_buff, temp_buffer_size);
    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, response_ok, sizeof(response_ok), 500000);
    if(Timeout == status){

        return status;
    }
    else if(0 != status){

        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        if(ERROR == status)
        {
        	status = CM_Q03_TCP_IP_Get_Error();
        }
        return status;
    }
    return code_return + TCP_IP_CODES;
}

cm_q03_codes_t CM_Q03_TCP_IP_UDP_Send(uint8_t connectid, uint8_t *data, uint16_t size_data,
										 uint8_t *remote_ip, uint16_t size_remote_ip, uint16_t remote_port)
{
    uint16_t status = 0;
    uint8_t temp_buff[5] = {0};
    uint16_t temp_buffer_size = 0;
    uint8_t cmd_buff[41] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint16_t rxbuffer_size;

    if (0 > connectid || 2 < connectid)
    {
        return User_Error;
    }

    if (0 > size_data || 1460 < size_data)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, tcp_ip_send_string, sizeof(tcp_ip_send_string), &cmd_buff_size);
    Dec_to_Ascii(connectid, temp_buff, &temp_buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(size_data, temp_buff, &temp_buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, remote_ip, size_remote_ip, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(remote_port, temp_buff, &temp_buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
    if(Timeout == status)
    {
        return status;
    }
    Delay(1000000);
    if(4 == g_count_valid_data)
    {
    	status = Success;
    }
/*    status = CM_Q01_Read(rxbuff, &rxbuffer_size, 600000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q01_Compare_short_response(rxbuff, rxbuffer_size, &tcp_ip_send_string_response,
                                     	   sizeof(tcp_ip_send_string_response), 500000);
    if(Success != status)
    {
		status = CM_Q01_Get_Response(rxbuff, rxbuffer_size);
        if(ERROR == status)
        {
        	status = CM_Q01_TCP_IP_Get_Error();
        }
		return status;
    }*/
    CM_Q03_buffer_clean();
    status = CM_Q03_Write(data, size_data, 500000);
    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
    if(Timeout == status)
    {
        return status;
    }
	status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, tcp_ip_send_response_ok,
										   sizeof(tcp_ip_send_response_ok), 500000);
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, tcp_ip_send_response_fail,
											   sizeof(tcp_ip_send_response_fail), 500000);
		if(Success == status)
		{
			return _TCP_IP_Send_Fail;
		}
		status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        if(ERROR == status)
        {
        	status = CM_Q03_TCP_IP_Get_Error();
        }
		return status;
	}
     return Success;
}

cm_q03_codes_t CM_Q03_TCP_IP_Send_String(uint8_t connectid, uint8_t *string, uint16_t size_string)
{
    uint16_t status = 0;
    uint8_t temp_buff[4] = {0};
    uint16_t temp_buffer_size = 0;
    uint8_t cmd_buff[17] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint16_t rxbuffer_size;

    if (0 > connectid || 2 < connectid)
    {
        return User_Error;
    }

    if (0 >= size_string || 1460 < size_string)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, tcp_ip_send_string, sizeof(tcp_ip_send_string), &cmd_buff_size);
    Dec_to_Ascii(connectid, temp_buff, &temp_buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(size_string, temp_buff, &temp_buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
    if(Timeout == status)
    {
        return status;
    }
    Delay(1000000);
     if(4 == g_count_valid_data)
     {
         status = Success;
     }
 /*    status = CM_Q01_Read(rxbuff, &rxbuffer_size, 600000);
     if(Timeout == status)
     {
         return status;
     }
     status = CM_Q01_Compare_short_response(rxbuff, rxbuffer_size, &tcp_ip_send_string_response,
                                            sizeof(tcp_ip_send_string_response), 500000);
     if(Success != status)
     {
         status = CM_Q01_Get_Response(rxbuff, rxbuffer_size);
         if(ERROR == status)
         {
             status = CM_Q01_TCP_IP_Get_Error();
         }
         return status;
     }*/
    CM_Q03_buffer_clean();
    status = CM_Q03_Write(string, size_string, 500000);
    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
    if(Timeout == status)
    {
        return status;
    }
	status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, tcp_ip_send_response_ok,
										   sizeof(tcp_ip_send_response_ok), 500000);
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, tcp_ip_send_response_fail,
											   sizeof(tcp_ip_send_response_fail), 500000);
		if(Success == status)
		{
			return _TCP_IP_Send_Fail;
		}
		status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        if(ERROR == status)
        {
        	status = CM_Q03_TCP_IP_Get_Error();
        }
		return status;
	}
     return Success;
}

cm_q03_codes_t CM_Q03_TCP_IP_Send_hex_String(uint8_t connectid, uint8_t *hex_string, uint16_t size_hex_string ){

    uint16_t status = 0;
    uint8_t temp_buff = 0;
    uint16_t temp_buffer_size = 0;
    uint8_t cmd_buff[529] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint16_t rxbuffer_size ;

    if (0 > connectid || 2 < connectid)
    {
        return User_Error;
    }
    if (0 > size_hex_string || 512 < size_hex_string)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, tcp_ip_send_hex_string, sizeof(tcp_ip_send_hex_string), &cmd_buff_size);
    Dec_to_Ascii(connectid, &temp_buff, &temp_buffer_size);
    Concatenate_Command(cmd_buff, &temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, hex_string, size_hex_string, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, tcp_ip_send_response_ok,
    									   sizeof(tcp_ip_send_response_ok), 500000);
	if(Timeout == status)
	{
		return status;
	}
	else if(Success != status)
	{
		status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, tcp_ip_send_response_fail,
											   sizeof(tcp_ip_send_response_fail), 500000);
		if(Success == status)
		{
			return _TCP_IP_Send_Fail;
		}
		status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        if(ERROR == status)
        {
        	status = CM_Q03_TCP_IP_Get_Error();
        }
		return status;
	}
     return Success;
}

cm_q03_codes_t CM_Q03_TCP_IP_Retrieve_Received(uint8_t connectid,
											uint8_t *return_string, uint16_t *size_return_string)
{
    uint16_t status = 0;
    uint8_t temp_buff = 0;
    uint16_t temp_buffer_size = 0;
    uint8_t cmd_buff[10] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[215] = {0};
    uint16_t rxbuffer_size;
    uint16_t i = 0;

    if (0 > connectid || 2 < connectid)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, tcp_ip_retrive_received, sizeof(tcp_ip_retrive_received), &cmd_buff_size);
    Dec_to_Ascii(connectid, &temp_buff, &temp_buffer_size);
    Concatenate_Command(cmd_buff, &temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
    character_response_end = '\n';
    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size,
    									   tcp_ip_retrive_received_response, sizeof(tcp_ip_retrive_received_response)-1,
										   500000);
    if(Timeout == status)
    {
        return status;
    }
    else if(0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        if(ERROR == status)
        {
        	status = CM_Q03_TCP_IP_Get_Error();
        }
        return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size,
    									   tcp_ip_retrive_received_response, sizeof(tcp_ip_retrive_received_response),
										   500000);
    if(0 == status)
    {
    	status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
        status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size,
        									   response_ok, sizeof(response_ok),
    										   500000);
        character_response_end = '\r';
        return _TCP_IP_No_data_to_read;
    }
	g_count_read -= 2;
	g_count_valid_data += 2;
	status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
	*size_return_string = rxbuffer_size;
	while(rxbuffer_size--)
	{
		*(return_string++) = rxbuff[i++];
	}
	character_response_end = '\r';
	status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
    return status;
}

cm_q03_codes_t CM_Q03_TCP_IP_UDP_Service_Received(uint8_t connectid,
												  uint8_t *return_string, uint16_t *size_return_string,
												  uint8_t *remote_ip, uint16_t *size_remote_ip,
												  uint16_t *remote_port)
{
    uint16_t status = 0;
    uint8_t temp_buff = 0;
    uint16_t temp_buffer_size = 0;
    uint8_t cmd_buff[10] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[215] = {0};
    uint16_t rxbuffer_size;
    uint16_t i = 0;
    uint8_t *temp_ptr;

    if (0 > connectid || 2 < connectid)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, tcp_ip_retrive_received, sizeof(tcp_ip_retrive_received), &cmd_buff_size);
    Dec_to_Ascii(connectid, &temp_buff, &temp_buffer_size);
    Concatenate_Command(cmd_buff, &temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
    Delay(1000000);
    character_response_end = '\n';
    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size,
    									   tcp_ip_retrive_received_response, sizeof(tcp_ip_retrive_received_response)-1,
										   500000);
    if(Timeout == status)
    {
        return status;
    }
    else if(0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        if(ERROR == status)
        {
        	status = CM_Q03_TCP_IP_Get_Error();
        }
        return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size,
    									   tcp_ip_retrive_received_response, sizeof(tcp_ip_retrive_received_response),
										   500000);
    if(0 == status)
    {
    	status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
        status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size,
        									   response_ok, sizeof(response_ok),
    										   500000);
        character_response_end = '\r';
        return _TCP_IP_No_data_to_read;
    }
    temp_ptr = (uint8_t*)strchr((char*)rxbuff, '"');
    temp_ptr++;
	while('"' != *temp_ptr)
	{
		*(remote_ip++) = *(temp_ptr++);
	}
	i = 0;
    temp_ptr = (uint8_t*)strrchr((char*)rxbuff, ',');
    temp_ptr++;
    *remote_port = (uint16_t)atoi((char*)temp_ptr);
	g_count_read -= 2;
	g_count_valid_data += 2;
	status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
	*size_return_string = rxbuffer_size;
	while(rxbuffer_size--)
	{
		*(return_string++) = rxbuff[i++];
	}
	character_response_end = '\r';
	status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
    return status;
}

cm_q03_codes_t CM_Q03_TCP_IP_Address(uint8_t contextid, uint8_t *ip_address, uint16_t *ip_address_size)
{
    uint16_t status = 0;
    uint16_t temp_index = 0;
    uint8_t cmd_buff[10] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[35] = {0};
    uint16_t rxbuffer_size;

    if( 1 <= contextid && 3 >= contextid)
    {
		Concatenate_Command(cmd_buff, tcp_ip_current_pdp_context, sizeof(tcp_ip_current_pdp_context), &cmd_buff_size);
		Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

	    Delay(50000);
	    CM_Q03_buffer_clean();

		status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
		if(Timeout == status)
		{
			return status;
		}
		status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
		if(Timeout == status)
		{
			return status;
		}
		while(CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, response_ok, sizeof(response_ok), 500000))
		{
			g_count_read -= 2;
			g_count_valid_data += 2;
			status = CM_Q03_Compare_response(rxbuff, rxbuffer_size, tcp_ip_current_pdp_response, sizeof(tcp_ip_current_pdp_response), sizeof(tcp_ip_current_pdp_response)-1, &temp_index, 500000);
			if((rxbuff[temp_index]-ASCII_0) == contextid)
			{
				temp_index += 7;
				*ip_address_size = 0;
				while(temp_index < (rxbuffer_size - 1))
				{
					*(ip_address++) = rxbuff[temp_index++];
					(*ip_address_size)++;
				}


			}
			status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
			if(Timeout == status)
			{
				return status;
			}
		}
		if(Timeout == status)
		{
			return status;
		}
    }
    else{
        return User_Error;
    }
    return Success;
}

cm_q03_codes_t CM_Q03_TCP_IP_Get_Error()
{
	uint8_t tcp_ip_get_error[13]={"AT+QIGETERROR"};
	uint8_t tcp_ip_get_error_response[13]={"+QIGETERROR: "};
    uint16_t status = 0;
    uint8_t cmd_buff[14] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint16_t rxbuffer_size;
    cm_q03_codes_t return_code;

	Concatenate_Command(cmd_buff, tcp_ip_get_error, sizeof(tcp_ip_get_error), &cmd_buff_size);
	Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

	status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
	if(Timeout == status)
	{
		return status;
	}
	status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
	if(Timeout == status)
	{
		return status;
	}
	status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, tcp_ip_get_error_response,
											sizeof(tcp_ip_get_error_response), 500000);
	if(Success != status)
	{
		return ERROR;
	}
	return_code = Ascii_to_Code(&rxbuff[sizeof(tcp_ip_get_error_response)], 3);
	status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
	if(Timeout == status)
	{
		return status;
	}
    return return_code;
}

urc_received_status_t CM_Q03_TCP_IP_Get_Last_Error_Code_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QIGETERROR_POS)]
										 >> (MODULE_8_SFT(QIGETERROR_POS))) & 0x01);
}

void CM_Q03_TCP_IP_Clear_Last_Error_Code_Status_Flag(void)
{
	urc_flags[DIV_BY_8(QIGETERROR_POS)] &= ~(1 << MODULE_8_SFT(QIGETERROR_POS));
}

void CM_Q03_TCP_IP_Get_Last_Error_Code_Status(cm_q03_codes_t *error_code)
{
	if(Success == (cm_q03_codes_t)(qigeterror.u16_0))
	{
		(*error_code) = Success;
	}

	else
	{
		(*error_code) = (cm_q03_codes_t)(qigeterror.u16_0 + TCP_IP_CODES);
	}

	urc_flags[DIV_BY_8(QIGETERROR_POS)] &= ~(1 << MODULE_8_SFT(QIGETERROR_POS));
}

urc_received_status_t CM_Q03_TCP_IP_Open_Socket_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			return(urc_received_status_t)((urc_flags[DIV_BY_8(QIOPEN_CID_0_POS)]
												 >> (MODULE_8_SFT(QIOPEN_CID_0_POS))) & 0x01);
		break;

		case 1:
			return(urc_received_status_t)((urc_flags[DIV_BY_8(QIOPEN_CID_1_POS)]
												 >> (MODULE_8_SFT(QIOPEN_CID_1_POS))) & 0x01);
		break;

		case 2:
			return(urc_received_status_t)((urc_flags[DIV_BY_8(QIOPEN_CID_2_POS)]
												 >> (MODULE_8_SFT(QIOPEN_CID_2_POS))) & 0x01);
		break;

		default:
			return invalid_ID;
		break;
	}
}

cm_q03_codes_t CM_Q03_TCP_IP_Clear_Open_Socket_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			urc_flags[DIV_BY_8(QIOPEN_CID_0_POS)] &= ~(1 << MODULE_8_SFT(QIOPEN_CID_0_POS));

			return Success;
		break;

		case 1:
			urc_flags[DIV_BY_8(QIOPEN_CID_1_POS)] &= ~(1 << MODULE_8_SFT(QIOPEN_CID_1_POS));

			return Success;
		break;

		case 2:
			urc_flags[DIV_BY_8(QIOPEN_CID_2_POS)] &= ~(1 << MODULE_8_SFT(QIOPEN_CID_2_POS));

			return Success;
		break;

		default:
			return TCP_IP_Invalid_Connect_ID;
		break;
	}
}

void CM_Q03_TCP_IP_Open_Socket_Status(uint8_t connectID, cm_q03_codes_t *status)
{
	switch(connectID)
	{
		case 0:
			if(Success == (cm_q03_codes_t)(qiopen_cid_0))
			{
				(*status) = (cm_q03_codes_t)(qiopen_cid_0);
			}

			else
			{
				(*status) = (cm_q03_codes_t)(qiopen_cid_0 + TCP_IP_CODES);
			}

			urc_flags[DIV_BY_8(QIOPEN_CID_0_POS)] &= ~(1 << MODULE_8_SFT(QIOPEN_CID_0_POS));
		break;

		case 1:
			if(Success == (cm_q03_codes_t)(qiopen_cid_1))
			{
				(*status) = (cm_q03_codes_t)(qiopen_cid_1);
			}

			else
			{
				(*status) = (cm_q03_codes_t)(qiopen_cid_1 + TCP_IP_CODES);
			}

			urc_flags[DIV_BY_8(QIOPEN_CID_1_POS)] &= ~(1 << MODULE_8_SFT(QIOPEN_CID_1_POS));
		break;

		case 2:
			if(Success == (cm_q03_codes_t)(qiopen_cid_2))
			{
				(*status) = (cm_q03_codes_t)(qiopen_cid_2);
			}

			else
			{
				(*status) = (cm_q03_codes_t)(qiopen_cid_2 + TCP_IP_CODES);
			}

			urc_flags[DIV_BY_8(QIOPEN_CID_2_POS)] &= ~(1 << MODULE_8_SFT(QIOPEN_CID_2_POS));
		break;

		default:
			(*status) = TCP_IP_Invalid_Connect_ID;
		break;
	}
}

urc_received_status_t CM_Q03_TCP_IP_Send_Data_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(QISEND_POS)]
										 >> (MODULE_8_SFT(QISEND_POS))) & 0x01);
}

void CM_Q03_TCP_IP_Clear_Send_Data_Status_Flag(void)
{
	urc_flags[DIV_BY_8(QISEND_POS)] &= ~(1 << MODULE_8_SFT(QISEND_POS));
}

void CM_Q03_TCP_IP_Send_Data_Status(uint16_t *total_sent_len,
		uint16_t *acked_bytes, uint16_t *nacked_bytes)
{
	(*total_sent_len) = qisend.u16_0;
	(*acked_bytes) = qisend.u16_1;
	(*nacked_bytes) = qisend.u16_2;

	urc_flags[DIV_BY_8(QISEND_POS)] &= ~(1 << MODULE_8_SFT(QISEND_POS));
}

urc_received_status_t CM_Q03_TCP_IP_Socket_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			return(urc_received_status_t)((urc_flags[DIV_BY_8(QISTATE_CID_0_POS)]
												 >> (MODULE_8_SFT(QISTATE_CID_0_POS))) & 0x01);
		break;

		case 1:
			return(urc_received_status_t)((urc_flags[DIV_BY_8(QISTATE_CID_1_POS)]
												 >> (MODULE_8_SFT(QISTATE_CID_1_POS))) & 0x01);
		break;

		case 2:
			return(urc_received_status_t)((urc_flags[DIV_BY_8(QISTATE_CID_2_POS)]
												 >> (MODULE_8_SFT(QISTATE_CID_2_POS))) & 0x01);
		break;

		default:
			return invalid_ID;
		break;
	}
}

cm_q03_codes_t CM_Q03_TCP_IP_Clear_Socket_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			urc_flags[DIV_BY_8(QISTATE_CID_0_POS)] &= ~(1 << MODULE_8_SFT(QISTATE_CID_0_POS));

			return Success;
		break;

		case 1:
			urc_flags[DIV_BY_8(QISTATE_CID_1_POS)] &= ~(1 << MODULE_8_SFT(QISTATE_CID_1_POS));

			return Success;
		break;

		case 2:
			urc_flags[DIV_BY_8(QISTATE_CID_2_POS)] &= ~(1 << MODULE_8_SFT(QISTATE_CID_2_POS));

			return Success;
		break;

		default:
			return TCP_IP_Invalid_Connect_ID;
		break;
	}
}

void CM_Q03_TCP_IP_Socket_Status(uint8_t connectID,
		cm_q03_codes_t *socket_state, service_type_t *service_type,
		IPv4_t *IP_addr, uint16_t *remote_port, uint16_t *local_port,
		uint8_t *context_id, uint8_t *server_id)
{
	uint8_t temp_octet[3] = {0};

	switch(connectID)
	{
		case 0:
			(*service_type) = (urc_tcp == qistate_cid_0.u16_0) ? (TCP) :
						(urc_udp == qistate_cid_0.u16_0) ? (UDP) :
						(urc_tcp_listener == qistate_cid_0.u16_0) ? (TCP_listener) :
						(urc_tcp_incoming == qistate_cid_0.u16_0) ? (TCP_incoming) :
																	(UDP_service);

			IP_addr->octet_1 = qistate_cid_0.u16_1;
			IP_addr->octet_2 = qistate_cid_0.u16_2;
			IP_addr->octet_3 = qistate_cid_0.u16_3;
			IP_addr->octet_4 = qistate_cid_0.u16_4;

			uitoa(qistate_cid_0.u16_1, (char*)temp_octet, 10);
			strcat((char*)&(IP_addr->IP_addr), (const char*)temp_octet);
			temp_octet[0] = '\0';
			temp_octet[1] = '\0';
			temp_octet[2] = '\0';
			uitoa(qistate_cid_0.u16_2, (char*)temp_octet, 10);
			strcat((char*)&(IP_addr->IP_addr), (const char*)temp_octet);
			temp_octet[0] = '\0';
			temp_octet[1] = '\0';
			temp_octet[2] = '\0';
			uitoa(qistate_cid_0.u16_3, (char*)temp_octet, 10);
			strcat((char*)&(IP_addr->IP_addr), (const char*)temp_octet);
			temp_octet[0] = '\0';
			temp_octet[1] = '\0';
			temp_octet[2] = '\0';
			uitoa(qistate_cid_0.u16_4, (char*)temp_octet, 10);
			strcat((char*)&(IP_addr->IP_addr), (const char*)temp_octet);
			temp_octet[0] = '\0';
			temp_octet[1] = '\0';
			temp_octet[2] = '\0';
			//TODO: Validate the following line to be correct
			IP_addr->IP_addr_size = strlen((const char*)(IP_addr->IP_addr));

			(*remote_port) = qistate_cid_0.u16_5;
			(*local_port) = qistate_cid_0.u16_6;
			(*socket_state) = (cm_q03_codes_t)(qistate_cid_0.u16_7 + TCP_IP_CODES
					+ TCP_IP_SOCKET_STATE_OFFSET);
			(*context_id) = qistate_cid_0.u16_8;
			(*server_id) = qistate_cid_0.u16_9;

			urc_flags[DIV_BY_8(QISTATE_CID_0_POS)] &= ~(1 << MODULE_8_SFT(QISTATE_CID_0_POS));
		break;

		case 1:
			(*service_type) = (urc_tcp == qistate_cid_1.u16_0) ? (TCP) :
						(urc_udp == qistate_cid_1.u16_0) ? (UDP) :
						(urc_tcp_listener == qistate_cid_1.u16_0) ? (TCP_listener) :
						(urc_tcp_incoming == qistate_cid_1.u16_0) ? (TCP_incoming) :
																	(UDP_service);

			IP_addr->octet_1 = qistate_cid_1.u16_1;
			IP_addr->octet_2 = qistate_cid_1.u16_2;
			IP_addr->octet_3 = qistate_cid_1.u16_3;
			IP_addr->octet_4 = qistate_cid_1.u16_4;

			uitoa(qistate_cid_1.u16_1, (char*)temp_octet, 10);
			strcat((char*)&(IP_addr->IP_addr), (const char*)temp_octet);
			temp_octet[0] = '\0';
			temp_octet[1] = '\0';
			temp_octet[2] = '\0';
			uitoa(qistate_cid_1.u16_2, (char*)temp_octet, 10);
			strcat((char*)&(IP_addr->IP_addr), (const char*)temp_octet);
			temp_octet[0] = '\0';
			temp_octet[1] = '\0';
			temp_octet[2] = '\0';
			uitoa(qistate_cid_1.u16_3, (char*)temp_octet, 10);
			strcat((char*)&(IP_addr->IP_addr), (const char*)temp_octet);
			temp_octet[0] = '\0';
			temp_octet[1] = '\0';
			temp_octet[2] = '\0';
			uitoa(qistate_cid_1.u16_4, (char*)temp_octet, 10);
			strcat((char*)&(IP_addr->IP_addr), (const char*)temp_octet);
			temp_octet[0] = '\0';
			temp_octet[1] = '\0';
			temp_octet[2] = '\0';
			//TODO: Validate the following line to be correct
			IP_addr->IP_addr_size = strlen((const char*)(IP_addr->IP_addr));

			(*remote_port) = qistate_cid_1.u16_5;
			(*local_port) = qistate_cid_1.u16_6;
			(*socket_state) = (cm_q03_codes_t)(qistate_cid_1.u16_7 + TCP_IP_CODES
					+ TCP_IP_SOCKET_STATE_OFFSET);
			(*context_id) = qistate_cid_1.u16_8;
			(*server_id) = qistate_cid_1.u16_9;

			urc_flags[DIV_BY_8(QISTATE_CID_1_POS)] &= ~(1 << MODULE_8_SFT(QISTATE_CID_1_POS));
		break;

		case 2:
			(*service_type) = (urc_tcp == qistate_cid_2.u16_0) ? (TCP) :
						(urc_udp == qistate_cid_2.u16_0) ? (UDP) :
						(urc_tcp_listener == qistate_cid_2.u16_0) ? (TCP_listener) :
						(urc_tcp_incoming == qistate_cid_2.u16_0) ? (TCP_incoming) :
																	(UDP_service);

			IP_addr->octet_1 = qistate_cid_2.u16_1;
			IP_addr->octet_2 = qistate_cid_2.u16_2;
			IP_addr->octet_3 = qistate_cid_2.u16_3;
			IP_addr->octet_4 = qistate_cid_2.u16_4;

			uitoa(qistate_cid_2.u16_1, (char*)temp_octet, 10);
			strcat((char*)&(IP_addr->IP_addr), (const char*)temp_octet);
			temp_octet[0] = '\0';
			temp_octet[1] = '\0';
			temp_octet[2] = '\0';
			uitoa(qistate_cid_2.u16_2, (char*)temp_octet, 10);
			strcat((char*)&(IP_addr->IP_addr), (const char*)temp_octet);
			temp_octet[0] = '\0';
			temp_octet[1] = '\0';
			temp_octet[2] = '\0';
			uitoa(qistate_cid_2.u16_3, (char*)temp_octet, 10);
			strcat((char*)&(IP_addr->IP_addr), (const char*)temp_octet);
			temp_octet[0] = '\0';
			temp_octet[1] = '\0';
			temp_octet[2] = '\0';
			uitoa(qistate_cid_2.u16_4, (char*)temp_octet, 10);
			strcat((char*)&(IP_addr->IP_addr), (const char*)temp_octet);
			temp_octet[0] = '\0';
			temp_octet[1] = '\0';
			temp_octet[2] = '\0';
			//TODO: Validate the following line to be correct
			IP_addr->IP_addr_size = strlen((const char*)(IP_addr->IP_addr));

			(*remote_port) = qistate_cid_2.u16_5;
			(*local_port) = qistate_cid_2.u16_6;
			(*socket_state) = (cm_q03_codes_t)(qistate_cid_2.u16_7 + TCP_IP_CODES
					+ TCP_IP_SOCKET_STATE_OFFSET);
			(*context_id) = qistate_cid_2.u16_8;
			(*server_id) = qistate_cid_2.u16_9;

			urc_flags[DIV_BY_8(QISTATE_CID_2_POS)] &= ~(1 << MODULE_8_SFT(QISTATE_CID_2_POS));
		break;

		default:
			(*socket_state) = TCP_IP_Invalid_Connect_ID;
			IP_addr->octet_1 = 0;
			IP_addr->octet_2 = 0;
			IP_addr->octet_3 = 0;
			IP_addr->octet_4 = 0;

			memset((IP_addr->IP_addr), '\0', sizeof(IP_addr->IP_addr));
			IP_addr->IP_addr_size = 0;

			remote_port = 0;
			local_port = 0;
			context_id = 0;
			server_id = 0;
		break;
	}
}

urc_received_status_t CM_Q03_TCP_IP_Incoming_Full_Flag(void)
{
	return((urc_received_status_t)(no_param_urc.flags_struct.qiurc_incoming_full));
}

void CM_Q03_TCP_IP_Clear_Incoming_Full_Flag(void)
{
	no_param_urc.flags_struct.qiurc_incoming_full = 0;
}

urc_received_status_t CM_Q03_TCP_IP_Incoming_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QIURC_INCOMINC_CID_0_POS)]
							>> (MODULE_8_SFT(QIURC_INCOMINC_CID_0_POS))) & 0x01);
		break;

		case 1:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QIURC_INCOMINC_CID_1_POS)]
							>> (MODULE_8_SFT(QIURC_INCOMINC_CID_1_POS))) & 0x01);
		break;

		case 2:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QIURC_INCOMINC_CID_2_POS)]
							>> (MODULE_8_SFT(QIURC_INCOMINC_CID_2_POS))) & 0x01);
		break;

		default:
			return invalid_ID;
		break;
	}
}

cm_q03_codes_t CM_Q03_TCP_IP_Clear_Incoming_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			urc_flags[DIV_BY_8(QIURC_INCOMINC_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QIURC_INCOMINC_CID_0_POS));

			return Success;
		break;

		case 1:
			urc_flags[DIV_BY_8(QIURC_INCOMINC_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QIURC_INCOMINC_CID_1_POS));

			return Success;
		break;

		case 2:
			urc_flags[DIV_BY_8(QIURC_INCOMINC_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QIURC_INCOMINC_CID_2_POS));

			return Success;
		break;

		default:
			return TCP_IP_Invalid_Connect_ID;
		break;
	}
}

void CM_Q03_TCP_IP_Incoming_Status(uint8_t connectID,
		cm_q03_codes_t *socket_status, uint8_t *server_ID, IPv4_t *IP_addr,
		uint16_t *remote_IP)
{
	switch(connectID)
	{
		case 0:
			(*socket_status) = Success;

			(*server_ID) = qiurc_incoming_cid_0.u16_0;

			IP_addr->octet_1 = qiurc_incoming_cid_0.u16_1;
			IP_addr->octet_2 = qiurc_incoming_cid_0.u16_2;
			IP_addr->octet_3 = qiurc_incoming_cid_0.u16_3;
			IP_addr->octet_4 = qiurc_incoming_cid_0.u16_4;

			(*remote_IP) = qiurc_incoming_cid_0.u16_5;

			urc_flags[DIV_BY_8(QIURC_INCOMINC_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QIURC_INCOMINC_CID_0_POS));

		break;

		case 1:
			(*socket_status) = Success;

			(*server_ID) = qiurc_incoming_cid_1.u16_0;

			IP_addr->octet_1 = qiurc_incoming_cid_1.u16_1;
			IP_addr->octet_2 = qiurc_incoming_cid_1.u16_2;
			IP_addr->octet_3 = qiurc_incoming_cid_1.u16_3;
			IP_addr->octet_4 = qiurc_incoming_cid_1.u16_4;

			(*remote_IP) = qiurc_incoming_cid_1.u16_5;

			urc_flags[DIV_BY_8(QIURC_INCOMINC_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QIURC_INCOMINC_CID_1_POS));
		break;

		case 2:
			(*socket_status) = Success;

			(*server_ID) = qiurc_incoming_cid_2.u16_0;

			IP_addr->octet_1 = qiurc_incoming_cid_2.u16_1;
			IP_addr->octet_2 = qiurc_incoming_cid_2.u16_2;
			IP_addr->octet_3 = qiurc_incoming_cid_2.u16_3;
			IP_addr->octet_4 = qiurc_incoming_cid_2.u16_4;

			(*remote_IP) = qiurc_incoming_cid_2.u16_5;

			urc_flags[DIV_BY_8(QIURC_INCOMINC_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QIURC_INCOMINC_CID_2_POS));


		break;

		default:
			(*socket_status) = TCP_IP_Invalid_Connect_ID;

			(*server_ID) = 0;

			IP_addr->octet_1 = 0;
			IP_addr->octet_2 = 0;
			IP_addr->octet_3 = 0;
			IP_addr->octet_4 = 0;
			IP_addr->IP_addr_size = 0;
			memset(IP_addr->IP_addr, '\0', sizeof(IP_addr->IP_addr));

			(*remote_IP) = 0;
		break;
	}
}

urc_received_status_t CM_Q03_TCP_IP_PDP_Deactivated_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			return (urc_received_status_t)(no_param_urc.flags_struct.pdpdeact_cid_0);
		break;

		case 1:
			return (urc_received_status_t)(no_param_urc.flags_struct.pdpdeact_cid_1);
		break;

		case 2:
			return (urc_received_status_t)(no_param_urc.flags_struct.pdpdeact_cid_2);
		break;

		default:
			return invalid_ID;
		break;
	}
}

cm_q03_codes_t CM_Q03_TCP_IP_Clear_PDP_Deactivated_Flag(uint8_t contextID)
{
	switch(contextID)
	{
		case 0:
			no_param_urc.flags_struct.pdpdeact_cid_0 = 0;
			return Success;
		break;

		case 1:
			no_param_urc.flags_struct.pdpdeact_cid_1 = 0;
			return Success;
		break;

		case 2:
			no_param_urc.flags_struct.pdpdeact_cid_2 = 0;
			return Success;
		break;

		default:
			return TCP_IP_Invalid_Context_ID;
		break;
	}
}

urc_received_status_t CM_Q03_TCP_IP_Received_Data_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			return (urc_received_status_t)(no_param_urc.flags_struct.qiurc_recv_cid_0);
		break;

		case 1:
			return (urc_received_status_t)(no_param_urc.flags_struct.qiurc_recv_cid_1);
		break;

		case 2:
			return (urc_received_status_t)(no_param_urc.flags_struct.qiurc_recv_cid_2);
		break;

		default:
			return invalid_ID;
		break;
	}
}

cm_q03_codes_t CM_Q03_TCP_IP_Clear_Received_Data_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			no_param_urc.flags_struct.qiurc_recv_cid_0 = 0;
			return Success;
		break;

		case 1:
			no_param_urc.flags_struct.qiurc_recv_cid_1 = 0;
			return Success;
		break;

		case 2:
			no_param_urc.flags_struct.qiurc_recv_cid_2 = 0;
			return Success;
		break;

		default:
			return TCP_IP_Invalid_Connect_ID;
		break;
	}
}

urc_received_status_t CM_Q03_TCP_IP_Socket_Closed_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			return (urc_received_status_t)(no_param_urc.flags_struct.qiurc_closed_cid_0);
		break;

		case 1:
			return (urc_received_status_t)(no_param_urc.flags_struct.qiurc_closed_cid_1);
		break;

		case 2:
			return (urc_received_status_t)(no_param_urc.flags_struct.qiurc_closed_cid_2);
		break;

		default:
			return invalid_ID;
		break;
	}
}

cm_q03_codes_t CM_Q03_TCP_IP_Clear_Socket_Closed_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			no_param_urc.flags_struct.qiurc_closed_cid_0 = 0;
			return Success;
		break;

		case 1:
			no_param_urc.flags_struct.qiurc_closed_cid_1 = 0;
			return Success;
		break;

		case 2:
			no_param_urc.flags_struct.qiurc_closed_cid_2 = 0;
			return Success;
		break;

		default:
			return TCP_IP_Invalid_Connect_ID;
		break;
	}
}
