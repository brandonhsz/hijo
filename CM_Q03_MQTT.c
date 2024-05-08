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
 * \file       CM_Q03_MQTT.h
 * \brief      This is the CM_Q03 MQTT driver made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.\n
 *             **to identify the meaning of a CME error, please refer to the
 *             provided Quectel MQTT AT commands manual.\n
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

#include <CM_Q03_MQTT.h>
#include "string.h"

#define MQTT_TIMEOUT 		(1000000)

/************** URC POS ************************/
#define QMTREADY_PUB_POS  (20)
#define QMTCLOSE_CID_0_POS	(42)
#define QMTCLOSE_CID_1_POS	(43)
#define QMTCLOSE_CID_2_POS	(44)
#define QMTCONN_CID_0_POS	(45)
#define QMTCONN_CID_1_POS	(46)
#define QMTCONN_CID_2_POS	(47)
#define QMTDISC_CID_0_POS	(48)
#define QMTDISC_CID_1_POS	(49)
#define QMTDISC_CID_2_POS	(50)
#define QMTOPEN_CID_0_POS	(51)
#define QMTOPEN_CID_1_POS	(52)
#define QMTOPEN_CID_2_POS	(53)
#define QMTPUB_CID_0_POS	(54)
#define QMTPUB_CID_1_POS	(55)
#define QMTPUB_CID_2_POS	(56)
#define QMTRECV_CID_0_POS	(16)
#define QMTRECV_CID_1_POS	(17)
#define QMTRECV_CID_2_POS	(18)
#define QMTSTAT_CID_0_POS	(57)
#define QMTSTAT_CID_1_POS	(58)
#define QMTSTAT_CID_2_POS	(59)
#define QMTSUB_CID_0_POS	(60)
#define QMTSUB_CID_1_POS	(61)
#define QMTSUB_CID_2_POS	(62)
#define QMTUNS_CID_0_POS	(63)
#define QMTUNS_CID_1_POS	(64)
#define QMTUNS_CID_2_POS	(65)
/************** URC POS ************************/
extern uint8_t character_response_end ;
extern uint8_t character_command_end;
extern uint8_t command_comma;
extern uint8_t command_quotes;
extern uint8_t response_ok[2];
extern uint8_t g_interrupt_ri;

extern int16_t qmtclose_cid_0;
extern int16_t qmtclose_cid_1;
extern int16_t qmtclose_cid_2;
extern u16_2_t qmtconn_cid_0;
extern u16_2_t qmtconn_cid_1;
extern u16_2_t qmtconn_cid_2;
extern int16_t qmtdisc_cid_0;
extern int16_t qmtdisc_cid_1;
extern int16_t qmtdisc_cid_2;
extern int16_t qmtopen_cid_0;
extern int16_t qmtopen_cid_1;
extern int16_t qmtopen_cid_2;
extern u16_3_t qmtpub_cid_0;
extern u16_3_t qmtpub_cid_1;
extern u16_3_t qmtpub_cid_2;
extern uint16_t qmtstat_cid_0;
extern uint16_t qmtstat_cid_1;
extern uint16_t qmtstat_cid_2;
extern u16_3_t qmtsub_cid_0;
extern u16_3_t qmtsub_cid_1;
extern u16_3_t qmtsub_cid_2;
extern u16_2_t qmtuns_cid_0;
extern u16_2_t qmtuns_cid_1;
extern u16_2_t qmtuns_cid_2;
extern uint8_t qmtrecv_cid_0[QMTRECV_SIZE];
extern uint8_t qmtrecv_cid_1[QMTRECV_SIZE];
extern uint8_t qmtrecv_cid_2[QMTRECV_SIZE];
extern uint8_t urc_flags[URC_FLAG_SIZE];

uint8_t mqtt_configurations[10] = "AT+QMTCFG=";
uint8_t mqtt_version[9] = "\"version\"";
uint8_t mqtt_pdp_cid[8] = "\"pdpcid\"";
uint8_t mqtt_will[6] = "\"will\"";
uint8_t mqtt_timeout[9] = "\"timeout\"";
uint8_t mqtt_session[9] = "\"session\"";
uint8_t mqtt_keepalive[11] = "\"keepalive\"";
uint8_t mqtt_ssl[5] = "\"ssl\"";
uint8_t mqtt_alibaba[9] = "\"aliauth\"";

uint8_t mqtt_open[11] = "AT+QMTOPEN=";
uint8_t mqtt_open_response[10] = "+QMTOPEN: ";

uint8_t mqtt_close[12] = "AT+QMTCLOSE=";
uint8_t mqtt_close_response[11] = "+QMTCLOSE: ";

uint8_t mqtt_connect[11] = "AT+QMTCONN=";
uint8_t mqtt_connect_response[10] = "+QMTCONN: ";

uint8_t mqtt_disconnect[11] = "AT+QMTDISC=";
uint8_t mqtt_disconnect_response[10] = "+QMTDISC: ";

uint8_t mqtt_subscribe[10] = "AT+QMTSUB=";
uint8_t mqtt_subscribe_response[9] = "+QMTSUB: ";

uint8_t mqtt_unsubscribe[10] = "AT+QMTUNS=";
uint8_t mqtt_unsubscribe_response[9] = "+QMTUNS: ";

uint8_t mqtt_publish[10] = "AT+QMTPUB=";
uint8_t mqtt_start_publish_response = '>';
uint8_t mqtt_publish_response[9] = "+QMTPUB: ";
uint8_t mqtt_receive_data[10] = "+QMTRECV: ";

uint8_t mqtt_read[11]={"AT+QMTRECV="};

uint8_t mqtt_store_read[11]={"AT+QMTRECV?"};

uint8_t mqtt_store_config[27]={"AT+QMTCFG=\"recv/mode\",0,1,1"};

uint8_t mqtt_ctrl_z[2] = "\x1A\r";

mqtt_config_t mqtt_config[3] = {{4,1,5,3,0,1,120,0,0},
								{4,1,5,3,0,1,120,0,0},
								{4,1,5,3,0,1,120,0,0}};

extern uint16_t g_count_read;
extern uint16_t g_count_valid_data;
extern uint16_t g_count_BUFFER_SIZE;
extern volatile bool pit_channel_1_flag;

extern void CM_Q03_PIT_Initialization(void);
extern void CM_Q03_PIT_Start_Timer(uint32_t count_usec);
extern void CM_Q03_PIT_Stop_Timer(void);
extern volatile bool active_urc;

enum MQTT_codes{
	_MQTT_Network_Opened_Successfully,
	_MQTT_Wrong_parameter,
	_MQTT_Identifier_Occupied,
	_MQTT_Failed_Active_PDP,
	_MQTT_Failed_Parse_Domain_Name,
	_MQTT_Network_Disconnection_Error,
	_MQTT_Failed_Open_Network,


	_MQTT_Packet_sent_successfully,
	_MQTT_Packet_retransmission,
	_MQTT_Failed_send_packet,

	_MQTT_notConnected,
	_MQTT_initial,
	_MQTT_connecting,
	_MQTT_connected,
	_MQTT_disconnecting,

	_MQTT_Connection_Accepted,
	_MQTT_Connection_Refused_Unacceptable_Protocol_Version,
	_MQTT_Connection_Refused_Identifier_Rejected,
	_MQTT_Connection_Refused_Server_Unavialable,
	_MQTT_Connection_Refused_Bad_UserName_or_Password,
	_MQTT_Connection_Refused_Not_Authorized,

	_MQTT_No_message_received,

	_MQTT_Failed_to_close_connection,
	_MQTT_Connection_closed_succesfully,
	_MQTT_Failed_to_close_network,
	_MQTT_Network_closed_succesfully
};

// TODO: Cambiar rxbuff a local. Validar todos los valores que reciban las funciones. Validar el status TIMEOUT

cm_q03_codes_t CM_Q03_MQTT_Configure_Parameters(uint8_t tcpconnectID,
                                          mqtt_config_t mqtt_config_struct,
										  configuration_select_t get_default_config){
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[30] = {0};
    uint8_t temp_buff[5] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};


    if (Default_Configuration == get_default_config)
    {
        mqtt_config_struct.version = MQTT_v3_1;
        mqtt_config_struct.pdpcid = 1;
        mqtt_config_struct.pkt_timeout = 5;
        mqtt_config_struct.retry_times = 3;
        mqtt_config_struct.timeout_notice = not_report;
        mqtt_config_struct.clean_session = clean_session;
        mqtt_config_struct.keepalive = 120;
        mqtt_config_struct.ssl_enable = ssl_disabled;
        mqtt_config_struct.ctx_index = 0;
    }else
    {
        if (3 > mqtt_config_struct.version || 4 < mqtt_config_struct.version)
        {
            return User_Error;
        }
        if(1 > mqtt_config_struct.pdpcid || 16 < mqtt_config_struct.pdpcid)
        {
            return User_Error;
        }
        if(1 > mqtt_config_struct.pkt_timeout || 60 < mqtt_config_struct.pkt_timeout)
        {
            return User_Error;
        }
        if(0 > mqtt_config_struct.retry_times || 10 < mqtt_config_struct.retry_times)
        {
            return User_Error;
        }
        if(0 > mqtt_config_struct.timeout_notice || 1 < mqtt_config_struct.timeout_notice)
        {
            return User_Error;
        }
        if(0 > mqtt_config_struct.clean_session || 1 < mqtt_config_struct.clean_session)
        {
            return User_Error;
        }
        if(0 > mqtt_config_struct.keepalive || 3600 < mqtt_config_struct.keepalive)
        {
            return User_Error;
        }
        if(0 > mqtt_config_struct.ssl_enable || 1 < mqtt_config_struct.ssl_enable)
        {
            return User_Error;
        }
        if(0 > mqtt_config_struct.ctx_index || 5 < mqtt_config_struct.ctx_index)
        {
            return User_Error;
        }
    }

    if (0 > tcpconnectID || 2 < tcpconnectID)
    {
        return User_Error;
    } else
    {
        mqtt_config[tcpconnectID] = mqtt_config_struct;
    }

    if(1== tcpconnectID){

        mqtt_store_config[22]='1';

    }
    else if(2==tcpconnectID){

        mqtt_store_config[22]='2';

    }

    Concatenate_Command(cmd_buff, mqtt_configurations, sizeof(mqtt_configurations), &cmd_buff_size);
    Concatenate_Command(cmd_buff, mqtt_version, sizeof(mqtt_version), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(tcpconnectID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(mqtt_config_struct.version, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }


    memset(&cmd_buff[sizeof(mqtt_configurations)], '\0', sizeof(cmd_buff)-sizeof(mqtt_configurations));
    cmd_buff_size = sizeof(mqtt_configurations);
    Concatenate_Command(cmd_buff, mqtt_pdp_cid, sizeof(mqtt_pdp_cid), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(tcpconnectID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(mqtt_config_struct.pdpcid, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }


    memset(&cmd_buff[sizeof(mqtt_configurations)], '\0', sizeof(cmd_buff)-sizeof(mqtt_configurations));
    cmd_buff_size = sizeof(mqtt_configurations);
    Concatenate_Command(cmd_buff, mqtt_timeout, sizeof(mqtt_timeout), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(tcpconnectID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(mqtt_config_struct.pkt_timeout, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(mqtt_config_struct.retry_times, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(mqtt_config_struct.timeout_notice, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }


    memset(&cmd_buff[sizeof(mqtt_configurations)], '\0', sizeof(cmd_buff)-sizeof(mqtt_configurations));
    cmd_buff_size = sizeof(mqtt_configurations);
    Concatenate_Command(cmd_buff, mqtt_session, sizeof(mqtt_session), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(tcpconnectID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(mqtt_config_struct.clean_session, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }


    memset(&cmd_buff[sizeof(mqtt_configurations)], '\0', sizeof(cmd_buff)-sizeof(mqtt_configurations));
    cmd_buff_size = sizeof(mqtt_configurations);
    Concatenate_Command(cmd_buff, mqtt_keepalive, sizeof(mqtt_keepalive), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(tcpconnectID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(mqtt_config_struct.keepalive, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }


    memset(&cmd_buff[sizeof(mqtt_configurations)], '\0', sizeof(cmd_buff)-sizeof(mqtt_configurations));
    cmd_buff_size = sizeof(mqtt_configurations);
    Concatenate_Command(cmd_buff, mqtt_ssl, sizeof(mqtt_ssl), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(tcpconnectID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(mqtt_config_struct.ssl_enable, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(mqtt_config_struct.ctx_index, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }

    memset(cmd_buff, '\0', sizeof(cmd_buff));
    cmd_buff_size=0;
    Concatenate_Command(cmd_buff, mqtt_store_config, sizeof(mqtt_store_config), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }


    return status;
}

cm_q03_codes_t CM_Q03_MQTT_Configure_Will_Info(uint8_t tcpconnectID,
											   will_flag_t flag,quality_of_service_t qos,
											   retain_type_t retain,
											   uint8_t *topic, uint8_t size_topic,
											   uint8_t *msg, uint16_t size_msg)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[630] = {0};
    uint8_t temp_buff = 0;
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};

    if (0 > tcpconnectID || 2 < tcpconnectID)
    {
        return User_Error;
    }
    if (0 > flag || 1 < flag)
    {
        return User_Error;
    }
    if (0 > qos || 2 < qos)
    {
        return User_Error;
    }
    if (0 > retain || 1 < retain)
    {
        return User_Error;
    }
    if (1 > size_topic || 100 < size_topic)
    {
        return User_Error;
    }
    if (1 > size_msg || 500 < size_msg)
    {
        return User_Error;
    }


    Concatenate_Command(cmd_buff, mqtt_configurations, sizeof(mqtt_configurations), &cmd_buff_size);
    Concatenate_Command(cmd_buff, mqtt_will, sizeof(mqtt_will), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(tcpconnectID, &temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, &temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(flag, &temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, &temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(qos, &temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, &temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(retain, &temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, &temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, topic, size_topic, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, msg, size_msg, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }

    return status;
}

cm_q03_codes_t CM_Q03_MQTT_Open_Network(uint8_t tcpconnectID,
                                  uint8_t *host, uint16_t host_size,
                                  uint16_t port){
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[122] = {0};
    uint8_t temp_buff[2];
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    cm_q03_codes_t code_return = MQTT_Failed_Open_Network;
    uint8_t count_timeout=0;
    urc_received_status_t status_flag=no_urc_received;

    if (0 > tcpconnectID || 2 < tcpconnectID)
    {
        return User_Error;
    }
    if (1 > host_size || 100 < host_size)
    {
        return User_Error;
    }

//    CM_Q01_Flush_URC();


    Concatenate_Command(cmd_buff, mqtt_open, sizeof(mqtt_open), &cmd_buff_size);
    Dec_to_Ascii(tcpconnectID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, host, host_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(port, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_MQTT_Clear_Open_Network_Status_Flag(tcpconnectID);
    CM_Q03_buffer_clean();

//    bool state_ri=false;
//    CM_Q03_RI_Clean_flag();
//
//    state_ri=CM_Q03_RI_Get_State();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
	else if(0 != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}

    CM_Q03_AP_Ready(1, 100);



//    GPIO_PinWrite(GPIOE, PIN17_IDX, 0);

    // Clean the PIT_1 flag
    pit_channel_1_flag = false;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer(60000000);

    do{
        status_flag= CM_Q03_MQTT_Open_Network_Status_Flag(tcpconnectID);

        if(pit_channel_1_flag && count_timeout==0){
            CM_Q03_PIT_Stop_Timer();
            pit_channel_1_flag = false;
            count_timeout++;
            CM_Q03_PIT_Start_Timer(15000000+1000000);
        }
        else if (pit_channel_1_flag && count_timeout == 1 ){
            count_timeout++;
        }

    }while(count_timeout <= 1 && status_flag == no_urc_received);

    // Stop the count of the PIT_1
    CM_Q03_PIT_Stop_Timer();
    // Checks if the PIT_1 flag is active
    if(pit_channel_1_flag){
        // Deactivate the PIT_1 flag
        pit_channel_1_flag = false;
        // Return 1 to indicate an error
        return Timeout;
    }

    CM_Q03_MQTT_Open_Network_Status(tcpconnectID, &code_return);
    CM_Q03_AP_Ready(0, 100);

//    GPIO_PinWrite(GPIOE, PIN17_IDX, 1);
//    state_ri=CM_Q03_RI_Get_State();
//    CM_Q03_RI_Clean_flag();
//    state_ri=CM_Q03_RI_Get_State();

    g_interrupt_ri = 0;
    CM_Q03_buffer_clean();

    return code_return;

//    status = CM_Q01_Read(rxbuff, &buffer_size, 60000000);
//
//    if(Timeout == status)
//    {
//        status = CM_Q01_Read(rxbuff, &buffer_size, 15000000);
//        if(Timeout == status)
//        {
//        	CM_Q01_AP_Ready(0, 500);
//            return status;
//        }
//    }
//    status = CM_Q01_Compare_short_response(rxbuff, buffer_size, mqtt_open_response,
//                                     sizeof(mqtt_open_response), MQTT_TIMEOUT);
//    if (0 != status)
//    {
//        status = CM_Q01_Get_Response(rxbuff, buffer_size);
//        return status;
//    }
//    status = rxbuff[sizeof(mqtt_open_response)+2] - ASCII_0;
//    if(0xfffd == status)
//    {
//    	code_return = _MQTT_Failed_Open_Network;
//    }
//    else
//    {
//    	code_return += status;
//    }
//    return code_return + MQTT_CODES;

}

cm_q03_codes_t CM_Q03_MQTT_Close_Network(uint8_t tcpconnectID)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[14] = {0};
    uint8_t temp_buff[2] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    cm_q03_codes_t code_return = MQTT_Failed_to_close_network;
    urc_received_status_t status_flag=no_urc_received;

    if (0 > tcpconnectID || 2 < tcpconnectID)
    {
        return User_Error;
    }

//	CM_Q01_Flush_URC();

    Concatenate_Command(cmd_buff, mqtt_close, sizeof(mqtt_close), &cmd_buff_size);
    Dec_to_Ascii(tcpconnectID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_MQTT_Close_Network_Status_Flag(tcpconnectID);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }


    CM_Q03_AP_Ready(1, 100);
//    GPIO_PinWrite(GPIOE, PIN17_IDX, 0);

    // Clean the PIT_1 flag
    pit_channel_1_flag = false;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer(60000000);

    do{
        status_flag=CM_Q03_MQTT_Close_Network_Status_Flag(tcpconnectID);


    }while( !pit_channel_1_flag && status_flag==no_urc_received);

    // Stop the count of the PIT_1
    CM_Q03_PIT_Stop_Timer();
    // Checks if the PIT_1 flag is active
    if(pit_channel_1_flag){
        // Deactivate the PIT_1 flag
        pit_channel_1_flag = false;
        // Return 1 to indicate an error
        return Timeout;
    }

    CM_Q03_MQTT_Close_Network_Status(tcpconnectID, &code_return);

    CM_Q03_AP_Ready(0, 100);
//    GPIO_PinWrite(GPIOE, PIN17_IDX, 1);

    g_interrupt_ri = 0;
    CM_Q03_buffer_clean();

    return code_return;


//    CM_Q01_AP_Ready(1, 500);
//    status = CM_Q01_Read(rxbuff, &buffer_size, 60000000);
//    if(Timeout == status)
//    {
//        status = CM_Q01_Read(rxbuff, &buffer_size, 15000000);
//        if(Timeout == status)
//        {
//        	CM_Q01_AP_Ready(0, 500);
//            return status;
//        }
//    }
//    CM_Q01_AP_Ready(0, 500);
//	CM_Q01_buffer_clean();
//
//    status = CM_Q01_Compare_short_response(rxbuff, buffer_size, mqtt_close_response,
//                                     sizeof(mqtt_close_response), MQTT_TIMEOUT);
//    if (0 != status)
//    {
//        status = CM_Q01_Get_Response(rxbuff, buffer_size);
//        return status;
//    }
//    if('0' == rxbuff[sizeof(mqtt_close_response)+2])
//    {
//    	status = _MQTT_Connection_closed_succesfully;
//    }
//    else
//    {
//    	status = _MQTT_Failed_to_close_connection;
//    }
//
//    return status + MQTT_CODES;
}

cm_q03_codes_t CM_Q03_MQTT_Connect(uint8_t tcpconnectID,
                          uint8_t *clientID, uint8_t size_clientID,
                          uint8_t *username, uint8_t size_username,
                          uint8_t *password, uint8_t size_password)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[322] = {0};
    uint8_t temp_buff = 0;
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    cm_q03_codes_t code_return =MQTT_notConnected ;
    urc_received_status_t status_flag=no_urc_received;

    if (0 > tcpconnectID || 2 < tcpconnectID)
    {
        return User_Error;
    }
    if (1 > size_clientID || 100 < size_clientID)
    {
        return User_Error;
    }
    if (0 > size_username || 100 < size_username)
    {
        return User_Error;
    }
    if (0 > size_password || 200 < size_password)
    {
        return User_Error;
    }

//	CM_Q01_Flush_URC();


    Concatenate_Command(cmd_buff, mqtt_connect, sizeof(mqtt_connect), &cmd_buff_size);
    Dec_to_Ascii(tcpconnectID, &temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, &temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, clientID, size_clientID, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    if(NULL != username)
    {
        Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
        Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
        Concatenate_Command(cmd_buff, username, size_username, &cmd_buff_size);
        Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
        if(NULL != password)
        {
            Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
            Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
            Concatenate_Command(cmd_buff, password, size_password, &cmd_buff_size);
            Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
        }
    }
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_MQTT_Clear_Connect_Status_Flag(tcpconnectID);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }

//    Delay(50000);

    CM_Q03_AP_Ready(1, 100);

//    GPIO_PinWrite(GPIOE, PIN17_IDX, 0);

    // Clean the PIT_1 flag
    pit_channel_1_flag = false;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer(((mqtt_config[tcpconnectID].pkt_timeout)*1000000)+500000);

    do{

        status_flag= CM_Q03_MQTT_Connect_Status_Flag(tcpconnectID);

    }while( !pit_channel_1_flag && status_flag==no_urc_received);

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
    CM_Q03_MQTT_Connect_Status(tcpconnectID, &code_return, &protocol_status);

    CM_Q03_AP_Ready(0, 100);
//    GPIO_PinWrite(GPIOE, PIN17_IDX, 1);

    g_interrupt_ri = 0;
    CM_Q03_buffer_clean();

    if(MQTT_Packet_sent_successfully == code_return)
    {
        return protocol_status;
    }

    else
    {
        return code_return;
    }

//    CM_Q01_AP_Ready(1, 500);
//
//
//    status = CM_Q01_Read(rxbuff, &buffer_size, (mqtt_config[tcpconnectID].pkt_timeout)*1000000);
//    if(Timeout == status)
//    {
//    	CM_Q01_AP_Ready(0, 500);
//        return status;
//    }
//    CM_Q01_AP_Ready(0, 500);
//	g_interrupt_ri = 0;
//	CM_Q01_buffer_clean();
//    status = CM_Q01_Compare_short_response(rxbuff, buffer_size, mqtt_connect_response,
//                                     sizeof(mqtt_connect_response), MQTT_TIMEOUT);
//    if(Timeout == status)
//    {
//        return status;
//    }
//    else if (0 != status)
//    {
//        status = CM_Q01_Get_Response(rxbuff, buffer_size);
//        return status;
//    }
//
//    status = rxbuff[sizeof(mqtt_connect_response)+2] - ASCII_0;
//    if(0 == status)
//    {
//    	code_return = _MQTT_Connection_Accepted + (rxbuff[sizeof(mqtt_connect_response)+4] - ASCII_0);
//    }
//    else
//    {
//    	code_return += status;
//    }
//    return code_return + MQTT_CODES;
//
//    //
}

cm_q03_codes_t CM_Q03_MQTT_Connection_Status(uint8_t tcpconnectID)
{
	uint8_t mqtt_connection_status[11]={"AT+QMTCONN?"};
//	uint8_t mqtt_connection_status_redy[11]={"+QMTCONN: X"};
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[12] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
//    uint16_t temp_index = 0;
    cm_q03_codes_t code_return =MQTT_notConnected ;
    urc_received_status_t status_flag=no_urc_received;

    if (0 > tcpconnectID || 2 < tcpconnectID)
    {
        return User_Error;
    }

    active_urc=true;

    Concatenate_Command(cmd_buff, mqtt_connection_status, sizeof(mqtt_connection_status), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_MQTT_Clear_Connect_Status_Flag(tcpconnectID);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }


    CM_Q03_AP_Ready(1, 100);


    // Clean the PIT_1 flag
    pit_channel_1_flag = false;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer(((mqtt_config[tcpconnectID].pkt_timeout)*1000000)+100000);

    do{

        status_flag= CM_Q03_MQTT_Connect_Status_Flag(tcpconnectID);

    }while( !pit_channel_1_flag && status_flag==no_urc_received);

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
    CM_Q03_MQTT_Connect_Status(tcpconnectID, &code_return, &protocol_status);

    code_return-=MQTT_Packet_sent_successfully;
    code_return+=MQTT_notConnected;

    CM_Q03_AP_Ready(0, 100);
    g_interrupt_ri = 0;
    CM_Q03_buffer_clean();

    return code_return;


//		while(CM_Q01_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), 500000))
//		{
//			if(0 == g_count_read){
//				g_count_read= g_count_BUFFER_SIZE-2;
//			}
//			else if(1 == g_count_read){
//				g_count_read= g_count_BUFFER_SIZE-1;
//			}
//			else{
//				g_count_read -= 2;
//			}
//			g_count_valid_data += 2;
//			status = CM_Q01_Compare_response(rxbuff, buffer_size, mqtt_connection_status_redy, sizeof(mqtt_connection_status_redy), sizeof(mqtt_connection_status_redy)-1, &temp_index, 500000);
//			if((rxbuff[temp_index]-ASCII_0) == tcpconnectID)
//			{
//				temp_index += 2;
//				code_return += rxbuff[temp_index]-ASCII_0;
//			}
//			status = CM_Q01_Read(rxbuff, &buffer_size, 600000);
//			if(Timeout == status)
//			{
//				return status;
//			}
//		}
//		if(Timeout == status)
//		{
//
//			return status;
//		}else if(0 != status)
//		{
//
//			status = CM_Q01_Get_Response(rxbuff, buffer_size);
//			return status;
//		}
//    }else
//    {
//        return User_Error;
//    }
//    return code_return + MQTT_CODES;
}

cm_q03_codes_t CM_Q03_MQTT_Disconnect(uint8_t tcpconnectID)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[13] = {0};
    uint8_t temp_buff[2] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    cm_q03_codes_t code_return =MQTT_Failed_to_close_connection ;
    urc_received_status_t status_flag=no_urc_received;

    if (0 > tcpconnectID || 2 < tcpconnectID)
    {
        return User_Error;
    }

//	CM_Q01_Flush_URC();


    Concatenate_Command(cmd_buff, mqtt_disconnect, sizeof(mqtt_disconnect), &cmd_buff_size);
    Dec_to_Ascii(tcpconnectID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_MQTT_Clear_Disconnect_Status_Flag(tcpconnectID);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }


    CM_Q03_AP_Ready(1, 100);

    // Clean the PIT_1 flag
    pit_channel_1_flag = false;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer(60000000+100000);

    do{
        status_flag=CM_Q03_MQTT_Disconnect_Status_Flag(tcpconnectID);


    }while( !pit_channel_1_flag && status_flag==no_urc_received);

    // Stop the count of the PIT_1
    CM_Q03_PIT_Stop_Timer();
    // Checks if the PIT_1 flag is active
    if(pit_channel_1_flag){
        // Deactivate the PIT_1 flag
        pit_channel_1_flag = false;
        // Return 1 to indicate an error
        return Timeout;
    }

    CM_Q03_MQTT_Disconnect_Status(tcpconnectID, &code_return);

    CM_Q03_AP_Ready(0, 100);
    g_interrupt_ri = 0;
    CM_Q03_buffer_clean();

    return code_return;



//    CM_Q01_AP_Ready(1, 500);
//    status = CM_Q01_Read(rxbuff, &buffer_size, 2*MQTT_TIMEOUT);
//	if(Timeout == status)
//	{
//	    CM_Q01_AP_Ready(0, 500);
//		return status;
//	}
//    CM_Q01_AP_Ready(0, 500);
//	CM_Q01_buffer_clean();
//    status = CM_Q01_Compare_short_response(rxbuff, buffer_size, mqtt_disconnect_response,
//                                     sizeof(mqtt_disconnect_response), MQTT_TIMEOUT);
//    if (0 != status)
//    {
//        status = CM_Q01_Get_Response(rxbuff, buffer_size);
//        return status;
//    }
//    if('0' == rxbuff[sizeof(mqtt_disconnect_response)+2])
//    {
//    	status = _MQTT_Connection_closed_succesfully;
//    }
//    else
//    {
//    	status = _MQTT_Failed_to_close_connection;
//    }
//
//    return status + MQTT_CODES;
}

cm_q03_codes_t CM_Q03_MQTT_Subscribe(uint8_t tcpconnectID,
                               uint16_t msgID,
                               uint8_t *topic, uint8_t size_topic,
							   quality_of_service_t QoS)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[123] = {0};
    uint8_t temp_buff[5] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[20] = {0};
//    uint8_t retry_times = 0;
    cm_q03_codes_t code_return = MQTT_Failed_send_packet;
    uint8_t count_timeout=0;
    urc_received_status_t status_flag=no_urc_received;

    if (0 > tcpconnectID || 2 < tcpconnectID)
    {
        return User_Error;
    }
    if (1 > size_topic || 100 < size_topic)
    {
        return User_Error;
    }
    if (0 > QoS || 2 < QoS)
    {
        return User_Error;
    }

//    CM_Q01_Flush_URC();


    Concatenate_Command(cmd_buff, mqtt_subscribe, sizeof(mqtt_subscribe), &cmd_buff_size);
    Dec_to_Ascii(tcpconnectID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(msgID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, topic, size_topic, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(QoS, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_MQTT_Clear_Subscribe_Status_Flag(tcpconnectID);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }

    CM_Q03_AP_Ready(1, 100);

    // Clean the PIT_1 flag
    pit_channel_1_flag = false;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer((mqtt_config[tcpconnectID].pkt_timeout)*1000000);

    do{
        status_flag=CM_Q03_MQTT_Subscribe_Status_Flag(tcpconnectID);

        if(pit_channel_1_flag && count_timeout < mqtt_config[tcpconnectID].retry_times){
            CM_Q03_PIT_Stop_Timer();
            pit_channel_1_flag = false;
            count_timeout++;
            CM_Q03_PIT_Start_Timer(((mqtt_config[tcpconnectID].pkt_timeout)*1000000)+100000);
        }
        else if (pit_channel_1_flag && count_timeout == mqtt_config[tcpconnectID].retry_times){
            count_timeout++;
        }

    }while(count_timeout <= mqtt_config[tcpconnectID].retry_times && status_flag==no_urc_received);


    // Stop the count of the PIT_1
    CM_Q03_PIT_Stop_Timer();
    // Checks if the PIT_1 flag is active
    if(pit_channel_1_flag){
        // Deactivate the PIT_1 flag
        pit_channel_1_flag = false;
        // Return 1 to indicate an error
        return Timeout;
    }
    uint16_t msgIDsub;
    uint16_t valuesub;
    CM_Q03_MQTT_Subscribe_Status(tcpconnectID, &code_return, &msgIDsub, &valuesub);

    CM_Q03_AP_Ready(0, 100);
    g_interrupt_ri = 0;
    CM_Q03_buffer_clean();

    return code_return;


//    CM_Q01_AP_Ready(1, 500);
//    do
//    {
//        status = CM_Q01_Read(rxbuff, &buffer_size, (mqtt_config[tcpconnectID].pkt_timeout)*1000000);
//        retry_times++;
//    }while(status && retry_times <= mqtt_config[tcpconnectID].retry_times);
//    CM_Q01_AP_Ready(0, 500);
//	g_interrupt_ri = 0;
//	CM_Q01_buffer_clean();
//	if(Timeout == status)
//	{
//		return status;
//	}
//
//	status = CM_Q01_Compare_short_response(rxbuff, buffer_size, mqtt_subscribe_response,
//                                     sizeof(mqtt_subscribe_response), MQTT_TIMEOUT);
//    if (0 != status)
//    {
//        status = CM_Q01_Get_Response(rxbuff, buffer_size);
//        return status;
//    }
//
//    code_return += rxbuff[sizeof(mqtt_subscribe_response)+4] - ASCII_0;
//    return code_return + MQTT_CODES;
}

cm_q03_codes_t CM_Q03_MQTT_Unsubscribe(uint8_t tcpconnectID,
                                 uint16_t msgID,
                                 uint8_t *topic,
                                 uint8_t size_topic)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[122] = {0};
    uint8_t temp_buff[5] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[18] = {0};
//    uint8_t retry_times = 0;
    cm_q03_codes_t code_return = MQTT_Failed_send_packet;
    uint8_t count_timeout=0;
    urc_received_status_t status_flag=no_urc_received;

    if (0 > tcpconnectID || 2 < tcpconnectID)
    {
        return User_Error;
    }
    if (1 > size_topic || 100 < size_topic)
    {
        return User_Error;
    }

//    CM_Q01_Flush_URC();


    Concatenate_Command(cmd_buff, mqtt_unsubscribe, sizeof(mqtt_unsubscribe), &cmd_buff_size);
    Dec_to_Ascii(tcpconnectID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(msgID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, topic, size_topic, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_MQTT_Clear_Unsubscribe_Status_Flag(tcpconnectID);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }



    CM_Q03_AP_Ready(1, 100);

    // Clean the PIT_1 flag
    pit_channel_1_flag = false;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer((mqtt_config[tcpconnectID].pkt_timeout)*1000000);

    do{
        status_flag=CM_Q03_MQTT_Unsubscribe_Status_Flag(tcpconnectID);

        if(pit_channel_1_flag && count_timeout < mqtt_config[tcpconnectID].retry_times){
            CM_Q03_PIT_Stop_Timer();
            pit_channel_1_flag = false;
            count_timeout++;
            CM_Q03_PIT_Start_Timer(((mqtt_config[tcpconnectID].pkt_timeout)*1000000)+100000);
        }
        else if (pit_channel_1_flag && count_timeout == mqtt_config[tcpconnectID].retry_times){
            count_timeout++;
        }

    }while(count_timeout <= mqtt_config[tcpconnectID].retry_times && status_flag==no_urc_received);


    // Stop the count of the PIT_1
    CM_Q03_PIT_Stop_Timer();
    // Checks if the PIT_1 flag is active
    if(pit_channel_1_flag){
        // Deactivate the PIT_1 flag
        pit_channel_1_flag = false;
        // Return 1 to indicate an error
        return Timeout;
    }
    uint16_t msgIDsub;
    CM_Q03_MQTT_Unsubscribe_Status(tcpconnectID, &code_return, &msgIDsub);

    CM_Q03_AP_Ready(0, 100);
    g_interrupt_ri = 0;
    CM_Q03_buffer_clean();

    return code_return;


//    CM_Q01_AP_Ready(1, 500);
//    do
//    {
//        status = CM_Q01_Read(rxbuff, &buffer_size, (mqtt_config[tcpconnectID].pkt_timeout)*1000000);
//        retry_times++;
//    }while(status && retry_times <= mqtt_config[tcpconnectID].retry_times);
//    CM_Q01_AP_Ready(0, 500);
//	g_interrupt_ri = 0;
//	CM_Q01_buffer_clean();
//	if(Timeout == status)
//	{
//		return status;
//	}
//
//	status = CM_Q01_Compare_short_response(rxbuff, buffer_size, mqtt_unsubscribe_response,
//                                     sizeof(mqtt_unsubscribe_response), MQTT_TIMEOUT);
//    if (0 != status)
//    {
//        status = CM_Q01_Get_Response(rxbuff, buffer_size);
//        return status;
//    }
//    code_return += rxbuff[sizeof(mqtt_unsubscribe_response)+4] - ASCII_0;
//    return status;
}

cm_q03_codes_t CM_Q03_MQTT_Publish(uint8_t tcpconnectID,
                             uint16_t msgID,
                             uint8_t *topic, uint8_t size_topic,
                             uint8_t *payload, uint16_t size_payload,
							 quality_of_service_t QoS, retain_type_t retain)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[125] = {0};
    uint8_t temp_buff[5] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[20] = {0};
    cm_q03_codes_t code_return = MQTT_Failed_send_packet;
    uint8_t count_timeout=0;
    urc_received_status_t status_flag=no_urc_received;

    if (0 > tcpconnectID || 2 < tcpconnectID)
    {
        return User_Error;
    }
    if (1 > size_topic || 100 < size_topic)
    {
        return User_Error;
    }
//    if (1 > size_payload || 1500 < size_payload)
//    {
//        return User_Error;
//    }
    if (0 > QoS || 2 < QoS)
    {
        return User_Error;
    }
    if (0 > retain || 1 < retain)
    {
        return User_Error;
    }

    if ((0 == QoS && msgID) || (0 != QoS && 0 == msgID))
    {
    	return User_Error;
    }

//	CM_Q01_Flush_URC();


    Concatenate_Command(cmd_buff, mqtt_publish, sizeof(mqtt_publish), &cmd_buff_size);
    Dec_to_Ascii(tcpconnectID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(msgID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(QoS, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(retain, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, topic, size_topic, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_MQTT_Clear_Ready_Publish_Status_Flag();
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);


    // Clean the PIT_1 flag
    pit_channel_1_flag = false;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer(200000);

    do{

    }while(!pit_channel_1_flag && CM_Q03_MQTT_Ready_Publish_Status_Flag()==no_urc_received && 4 != g_count_valid_data);

    // Stop the count of the PIT_1
    CM_Q03_PIT_Stop_Timer();
    // Checks if the PIT_1 flag is active
    if(pit_channel_1_flag){
        // Deactivate the PIT_1 flag
        pit_channel_1_flag = false;
        // Return 1 to indicate an error
        return Timeout;
    }

    Delay(1000);
    CM_Q03_buffer_clean();
    status = CM_Q03_Write(payload, size_payload, MQTT_TIMEOUT);
    status = CM_Q03_Write(mqtt_ctrl_z, sizeof(mqtt_ctrl_z), MQTT_TIMEOUT);

//    Delay(100000);
////    CM_Q01_MQTT_Get_Ready_Publish_Status_Flag();
//    if(4 == g_count_valid_data)
//    {
//		CM_Q01_buffer_clean();
//		status = CM_Q01_Write(payload, size_payload, MQTT_TIMEOUT);
//		status = CM_Q01_Write(mqtt_ctrl_z, sizeof(mqtt_ctrl_z), MQTT_TIMEOUT);
//    }

    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }


    CM_Q03_AP_Ready(1, 100);

    // Clean the PIT_1 flag
    pit_channel_1_flag = false;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer((mqtt_config[tcpconnectID].pkt_timeout)*1000000);

    do{
        status_flag=CM_Q03_MQTT_Publish_Status_Flag(tcpconnectID);

        if(pit_channel_1_flag && count_timeout < mqtt_config[tcpconnectID].retry_times){
            CM_Q03_PIT_Stop_Timer();
            pit_channel_1_flag = false;
            count_timeout++;
            CM_Q03_PIT_Start_Timer(((mqtt_config[tcpconnectID].pkt_timeout)*1000000)+100000);
        }
        else if (pit_channel_1_flag && count_timeout == mqtt_config[tcpconnectID].retry_times){
            count_timeout++;
        }

    }while(count_timeout <= mqtt_config[tcpconnectID].retry_times && status_flag==no_urc_received);


    // Stop the count of the PIT_1
    CM_Q03_PIT_Stop_Timer();
    // Checks if the PIT_1 flag is active
    if(pit_channel_1_flag){
        // Deactivate the PIT_1 flag
        pit_channel_1_flag = false;
        // Return 1 to indicate an error
        return Timeout;
    }
    uint16_t msgIDpub;
    uint16_t retransmissions;
    CM_Q03_MQTT_Publish_Status( tcpconnectID, &code_return, &msgIDpub, &retransmissions);

    CM_Q03_AP_Ready(0, 100);
    g_interrupt_ri = 0;
    CM_Q03_buffer_clean();

    return code_return;



//    do
//    {
//    	CM_Q01_Flush_URC();
//    	status = CM_Q01_read_URC_vector(rxbuff, &buffer_size, mqtt_publish_response, sizeof(mqtt_publish_response),
//    										(mqtt_config[tcpconnectID].pkt_timeout)*1000000);
//        retry_times++;
//    }while(status && (retry_times <= mqtt_config[tcpconnectID].retry_times));
//
//	if(Timeout == status)
//	{
//		return status;
//	}
//
//    status = CM_Q01_Compare_short_response(rxbuff, buffer_size, mqtt_publish_response,
//                                     sizeof(mqtt_publish_response), MQTT_TIMEOUT);
//    if (0 != status)
//    {
//        status = CM_Q01_Get_Response(rxbuff, buffer_size);
//        return status;
//    }
//
//    status = CM_Q01_Read_Response_count_Start_and_Stop_read_character(rxbuff, sizeof(rxbuff), &result, &size_result,
//    																	',', 2, '\0');
//
//    if (0 != status)
//    {
//        status = CM_Q01_Get_Response(rxbuff, buffer_size);
//        return status;
//    }
//
//    code_return += result - ASCII_0;
//    return code_return + MQTT_CODES;
}

cm_q03_codes_t CM_Q03_MQTT_Start_Publish(uint8_t tcpconnectID,
                                   uint16_t msgID,
                                   uint8_t *topic, uint8_t size_topic,
								   quality_of_service_t QoS,
								   retain_type_t retain)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[125] = {0};
    uint8_t temp_buff[5] = {0};
    uint16_t cmd_buff_size = 0;
//    uint8_t rxbuff[15] = {0};

    if (0 > tcpconnectID || 2 < tcpconnectID)
    {
        return User_Error;
    }
    if (1 > size_topic || 100 < size_topic)
    {
        return User_Error;
    }
    if (0 > QoS || 2 < QoS)
    {
        return User_Error;
    }
    if (0 > retain || 1 < retain)
    {
        return User_Error;
    }

//	CM_Q01_Flush_URC();


    Concatenate_Command(cmd_buff, mqtt_publish, sizeof(mqtt_publish), &cmd_buff_size);
    Dec_to_Ascii(tcpconnectID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(msgID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(QoS, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(retain, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, topic, size_topic, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_MQTT_Clear_Ready_Publish_Status_Flag();
    CM_Q03_buffer_clean();


    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);

    // Clean the PIT_1 flag
    pit_channel_1_flag = false;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer(200000);

    do{

    }while(!pit_channel_1_flag && CM_Q03_MQTT_Ready_Publish_Status_Flag()==no_urc_received && 4 != g_count_valid_data);

    // Stop the count of the PIT_1
    CM_Q03_PIT_Stop_Timer();
    // Checks if the PIT_1 flag is active
    if(pit_channel_1_flag){
        // Deactivate the PIT_1 flag
        pit_channel_1_flag = false;
        // Return 1 to indicate an error
        return Timeout;
    }

//    Delay(1000);
//    CM_Q01_buffer_clean();
//    status = CM_Q01_Write(payload, size_payload, MQTT_TIMEOUT);
//    status = CM_Q01_Write(mqtt_ctrl_z, sizeof(mqtt_ctrl_z), MQTT_TIMEOUT);
//
//
//    Delay(100000);
//    if(4 == g_count_valid_data)
//    {
//		CM_Q01_buffer_clean();
//		return Success;
//    }
//

//    status = CM_Q01_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
//	if(Timeout == status)
//	{
//		return status;
//	}
//	CM_Q01_buffer_clean();
//	status = CM_Q01_Get_Response(rxbuff, buffer_size);
	return status;
}

cm_q03_codes_t CM_Q03_MQTT_Write_Publish(uint8_t *payload, uint16_t size_payload)
{
    uint16_t status = 0;
    if (1 > size_payload || 1500 < size_payload)
    {
        return User_Error;
    }
    status = CM_Q03_Write(payload, size_payload, MQTT_TIMEOUT);
    return status;
}

cm_q03_codes_t CM_Q03_MQTT_End_Publish(uint8_t tcpconnectID)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t rxbuff[20] = {0};
    cm_q03_codes_t code_return = MQTT_Failed_send_packet;
    uint8_t count_timeout=0;
    urc_received_status_t status_flag=no_urc_received;

    if (0 > tcpconnectID || 2 < tcpconnectID)
    {
        return User_Error;
    }

    Delay(50000);
    CM_Q03_MQTT_Clear_Ready_Publish_Status_Flag();
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(mqtt_ctrl_z, sizeof(mqtt_ctrl_z), 5000000);

    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
	if(Timeout == status)
	{
		return status;
	}
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }


    CM_Q03_AP_Ready(1, 100);

    // Clean the PIT_1 flag
    pit_channel_1_flag = false;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer((mqtt_config[tcpconnectID].pkt_timeout)*1000000);

    do{
        status_flag=CM_Q03_MQTT_Publish_Status_Flag(tcpconnectID);

        if(pit_channel_1_flag && count_timeout < mqtt_config[tcpconnectID].retry_times){
            CM_Q03_PIT_Stop_Timer();
            pit_channel_1_flag = false;
            count_timeout++;
            CM_Q03_PIT_Start_Timer(((mqtt_config[tcpconnectID].pkt_timeout)*1000000)+100000);
        }
        else if (pit_channel_1_flag && count_timeout == mqtt_config[tcpconnectID].retry_times){
            count_timeout++;
        }

    }while(count_timeout <= mqtt_config[tcpconnectID].retry_times && status_flag==no_urc_received);


    // Stop the count of the PIT_1
    CM_Q03_PIT_Stop_Timer();
    // Checks if the PIT_1 flag is active
    if(pit_channel_1_flag){
        // Deactivate the PIT_1 flag
        pit_channel_1_flag = false;
        // Return 1 to indicate an error
        return Timeout;
    }
    uint16_t msgIDpub;
    uint16_t retransmissions;
    CM_Q03_MQTT_Publish_Status( tcpconnectID, &code_return, &msgIDpub, &retransmissions);

    CM_Q03_AP_Ready(0, 100);
    g_interrupt_ri = 0;
    CM_Q03_buffer_clean();

    return code_return;

//
//    do
//    {
//    	CM_Q01_Flush_URC();
//    	status = CM_Q01_read_URC_vector(rxbuff, &buffer_size, mqtt_publish_response, sizeof(mqtt_publish_response),
//    										(mqtt_config[tcpconnectID].pkt_timeout)*1000000);
//        retry_times++;
//    }while(status && (retry_times <= mqtt_config[tcpconnectID].retry_times));
//
//	if(Timeout == status)
//	{
//		return status;
//	}
//
//    status = CM_Q01_Compare_short_response(rxbuff, buffer_size, mqtt_publish_response,
//                                     sizeof(mqtt_publish_response), MQTT_TIMEOUT);
//    if (0 != status)
//    {
//        status = CM_Q01_Get_Response(rxbuff, buffer_size);
//        return status;
//    }
//
//    status = CM_Q01_Read_Response_count_Start_and_Stop_read_character(rxbuff, sizeof(rxbuff), &result, &size_result,
//    																	',', 2, '\0');
//
//    if (0 != status)
//    {
//        status = CM_Q01_Get_Response(rxbuff, buffer_size);
//        return status;
//    }
//
//    code_return += result - ASCII_0;
//    return code_return + MQTT_CODES;
}



cm_q03_codes_t CM_Q03_MQTT_Read_Message(uint8_t* tcpconnectID, uint16_t* msgID, uint8_t* topic,
		uint16_t* size_of_topic, uint8_t* msg, uint16_t* size_of_msg)
{
	uint32_t retval = Success;
	uint8_t msg_read[101] = {0};
	uint16_t size_msg_read = 0;

	uint8_t tcpID;
	uint16_t size_of_msgID = 0;
	uint8_t msg_ID[5];
	uint16_t temp_var;

	uint16_t read_character = sizeof(mqtt_receive_data);

	(*size_of_topic) = 0;
	(*size_of_msg) = 0;

//	CM_Q01_Flush_URC();
    Delay(50000);
    CM_Q03_buffer_clean();


	if(0 == CM_Q03_MQTT_Receive_Msg_Status_Flag(1))
	{
		return 	_MQTT_No_message_received + MQTT_CODES;
	}

	retval = CM_Q03_read_URC_vector(msg_read, &size_msg_read, mqtt_receive_data, 10, 50000);

	if(1 == retval)
	{
		return _MQTT_No_message_received + MQTT_CODES;
	}

	else if(Success != retval)
	{
		return retval;
	}

	retval = CM_Q03_Compare_short_response(msg_read, size_msg_read, mqtt_receive_data,
			sizeof(mqtt_receive_data), 500000);

	if(Success != retval)
	{
		return _MQTT_No_message_received + MQTT_CODES;
	}

	retval = CM_Q03_Read_Response_Start_and_Stop_read_character(msg_read, size_msg_read, &tcpID, &size_of_msgID, ' ', ',');
	if(Success != retval)
	{
		return retval;
	}

	//Adds size of TCP id
	read_character += (size_of_msgID);

	size_of_msgID = 0;

	retval = CM_Q03_Read_Response_Start_and_Stop_read_character(msg_read, size_msg_read, msg_ID, &size_of_msgID, ',', ',');
	if(Success != retval)
	{
		return retval;
	}

	//Adds size of msg ID
	read_character += size_of_msgID;

	retval = CM_Q03_Read_Response_Start_and_Stop_read_character(msg_read, size_msg_read, topic, size_of_topic, '"', '"');
	if(Success != retval)
	{
		return retval;
	}

	//Adds the size of the topic
	read_character += (*size_of_topic);

	//Adds the extra characters (quotes and commas)
	read_character += 7;

	//Subtracts the size of the URC minus the characters that do
	//correspond to the message
	read_character = size_msg_read - read_character;

	retval = CM_Q03_Read_Response_count_Start_N_characters(msg_read, size_msg_read, msg, size_of_msg, '"', 3, read_character);
	if(Success != retval)
	{
		return retval;
	}

	if(tcpID < '0' || tcpID > '2')
	{
		return ERROR;
	}
	else
	{
		(*tcpconnectID) = tcpID - 0x30;
	}

	(*msgID) = 0;

	switch(size_of_msgID)
	{
	case 5:
		temp_var = msg_ID[size_of_msgID - 5];
		temp_var -= 0x30;
		temp_var *= 10000;
		(*msgID) += temp_var;
	case 4:
		temp_var = msg_ID[size_of_msgID - 4];
		temp_var -= 0x30;
		temp_var *= 1000;
		(*msgID) += temp_var;
	case 3:
		temp_var = msg_ID[size_of_msgID - 3];
		temp_var -= 0x30;
		temp_var *= 100;
		(*msgID) += temp_var;
	case 2:
		temp_var = msg_ID[size_of_msgID - 2];
		temp_var -= 0x30;
		temp_var *= 10;
		(*msgID) += temp_var;
	case 1:
		temp_var = msg_ID[size_of_msgID - 1];
		temp_var -= 0x30;
		(*msgID) += temp_var;
		break;

	default:
		return ERROR;
		break;
	}

	return Success;
}

// store request

uint8_t store_request[3][5]={{0,0,0,0,0},
                             {0,0,0,0,0},
                             {0,0,0,0,0}
                            };
uint8_t store_count_msg[3]={1,1,1};
uint8_t store_read[3]={0,0,0};
//uint8_t store_

cm_q03_codes_t CM_Q03_MQTT_Store_Messages(uint8_t Client_ID, uint8_t* Store_msg){


    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[12] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[30] = {0};

    uint8_t store_temp_count=0;
    *Store_msg=0;
    Concatenate_Command(cmd_buff, mqtt_store_read, sizeof(mqtt_store_read), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);


    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);
    if(Timeout == status)
    {
      return status;
    }
    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
    if(Timeout == status)
    {
      return status;
    }

    rxbuff[buffer_size]=',';
    cmd_buff_size++;

    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, mqtt_receive_data,
                                     sizeof(mqtt_receive_data), MQTT_TIMEOUT);

    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status+FILE_CODES;
    }

    uint8_t output_buff[3];
    uint16_t size_output_buff;
    uint8_t start_characte=',';

//    CM_Q03_Read_Response_Start_and_Stop_read_character_end_ptr(rxbuff,buffer_size, output_buff, &size_output_buff, start_characte, start_characte, &ptr_end);
    status = CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff,buffer_size, output_buff, &size_output_buff, start_characte, start_characte);
    if(Success != status)
    {
        return status;
    }

    if(output_buff[0]-48){
        if( 0 == store_request[Client_ID][0]){
            store_request[Client_ID][0]= store_count_msg[Client_ID];
            store_count_msg[Client_ID]++;
        }

        store_read[Client_ID]=store_request[Client_ID][0]==1? 0:store_read[Client_ID];
    }
    else if(store_request[Client_ID][0]){

        store_temp_count++;
    }

    status = CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff,buffer_size, output_buff, &size_output_buff, start_characte, 2,start_characte);
    if(Success != status)
    {
        return status;
    }

    if(output_buff[0]-48){
        if( 0 == store_request[Client_ID][1]){
            store_request[Client_ID][1]= store_count_msg[Client_ID];
            store_count_msg[Client_ID]++;
        }
        store_read[Client_ID]=store_request[Client_ID][1]==1? 1:store_read[Client_ID];
    }
    else if(store_request[Client_ID][1]){

        store_temp_count++;
    }

    status = CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff,buffer_size, output_buff, &size_output_buff, start_characte, 3,start_characte);
    if(Success != status)
    {
        return status;
    }

    if(output_buff[0]-48){
        if( 0 == store_request[Client_ID][2]){
            store_request[Client_ID][2]= store_count_msg[Client_ID];
            store_count_msg[Client_ID]++;
        }
        store_read[Client_ID]=store_request[Client_ID][2]==1? 2:store_read[Client_ID];
    }
    else if(store_request[Client_ID][2]){

        store_temp_count++;
    }

    status = CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff,buffer_size, output_buff, &size_output_buff, start_characte, 4,start_characte);
    if(Success != status)
    {
        return status;
    }

    if(output_buff[0]-48){
        if( 0 == store_request[Client_ID][3]){
            store_request[Client_ID][3]= store_count_msg[Client_ID];
            store_count_msg[Client_ID]++;
        }
        store_read[Client_ID]=store_request[Client_ID][3]==1? 3:store_read[Client_ID];
    }
    else if(store_request[Client_ID][3]){

        store_temp_count++;
    }

    status = CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff,buffer_size, output_buff, &size_output_buff, start_characte, 5,start_characte);
    if(Success != status)
    {
        return status;
    }
    if(output_buff[0]-48){
        if( 0 == store_request[Client_ID][4]){
            store_request[Client_ID][4]= store_count_msg[Client_ID];
            store_count_msg[Client_ID]++;
        }
        store_read[Client_ID]=store_request[Client_ID][4]==1? 4:store_read[Client_ID];
    }
    else if(store_request[Client_ID][4]){

        store_temp_count++;
    }

//    if(store_temp_count){
//        store_request[Client_ID][0]=store_request[Client_ID][0]<1? 0:store_request[Client_ID][0]-1;
//        store_request[Client_ID][1]=store_request[Client_ID][1]<1? 0:store_request[Client_ID][1]-1;
//        store_request[Client_ID][2]=store_request[Client_ID][2]<1? 0:store_request[Client_ID][2]-1;
//        store_request[Client_ID][3]=store_request[Client_ID][3]<1? 0:store_request[Client_ID][3]-1;
//        store_request[Client_ID][4]=store_request[Client_ID][4]<1? 0:store_request[Client_ID][4]-1;
//
//        store_count_msg[Client_ID]=store_count_msg[Client_ID]<1? 0:store_count_msg[Client_ID]-1;
//
//    }

    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }
//    store_count_msg[Client_ID]=store_temp_count;
    *Store_msg=store_count_msg[Client_ID]-1;
    return status;
}


cm_q03_codes_t CM_Q03_MQTT_Read_Store_Message(uint8_t Client_ID, uint8_t* store_status, uint16_t*msgID, uint8_t* topic,
        uint16_t* size_of_topic, uint8_t* msg, uint16_t* size_of_msg)
{

//    uint16_t size_of_msgID = 0;
//
//    uint8_t start_characte=',';
//    uint8_t end_characte=',';


//    uint8_t output_buff[500];
//    uint16_t size_output_buff;

    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[30] = {0};
    uint8_t temp_buff[2] = {0};
    uint16_t temp_buff_size=0;
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[500] = {0};

//    Delay(5000000);

    status = CM_Q03_MQTT_Store_Messages(Client_ID, store_status);

    if(Success != status)
    {
        return status;
    }
    else if(0 == *store_status){

        return MQTT_No_message_received;
    }

    //CHECAR HASTA ESTE PUNTO SI CAMBIA EL AT+QMTRECV? CON RESPUESTA 1,1,0,0,0,0

    Concatenate_Command(cmd_buff, mqtt_read, sizeof(mqtt_read), &cmd_buff_size);
    Dec_to_Ascii(Client_ID, temp_buff, &temp_buff_size);
    Concatenate_Command(cmd_buff, temp_buff, temp_buff_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(store_read[Client_ID], temp_buff, &temp_buff_size);
    Concatenate_Command(cmd_buff, temp_buff, temp_buff_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    CM_Q03_MQTT_Clear_Receive_Msg_Status_Flag(Client_ID);

    Delay(50000);
    CM_Q03_buffer_clean();


    status = CM_Q03_Write(cmd_buff, cmd_buff_size, MQTT_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }

    Delay(100000);
    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }

//    Delay(5000000);

    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), MQTT_TIMEOUT);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
            if(Timeout == status)
            {
                return status;
            }
        status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
                                             sizeof(response_ok), MQTT_TIMEOUT);
            if(Timeout == status)
            {
                return status;
            }
            else if(Success != status)
            {
                status = CM_Q03_Get_Response(rxbuff, buffer_size);
                return status+FILE_CODES;
            }

    }


    CM_Q03_PIT_Stop_Timer();
    // Clean the PIT_1 flag
    pit_channel_1_flag = false;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer(400000);


    do{

    }while(!pit_channel_1_flag && !CM_Q03_MQTT_Receive_Msg_Status_Flag(Client_ID));

    // Stop the count of the PIT_1
    CM_Q03_PIT_Stop_Timer();
    // Checks if the PIT_1 flag is active
    if(pit_channel_1_flag){
        Delay(5000000);
        status=CM_Q03_MQTT_Receive_Msg_Status_Flag(Client_ID);
        // Deactivate the PIT_1 flag
        pit_channel_1_flag = false;
        // Return 1 to indicate an error
        return Timeout;
    }

    CM_Q03_MQTT_Clear_Receive_Msg_Status_Flag(Client_ID);

//    printf(" %s \n\r",qmtrecv_cid_1);

//    character_response_end='\n';
//    status = CM_Q03_Read(rxbuff, &buffer_size, MQTT_TIMEOUT);
//    character_response_end='\r';




    (*store_status)--;
    store_count_msg[Client_ID]--;
    store_request[Client_ID][0]=store_request[Client_ID][0]<1? 0:store_request[Client_ID][0]-1;
    store_request[Client_ID][1]=store_request[Client_ID][1]<1? 0:store_request[Client_ID][1]-1;
    store_request[Client_ID][2]=store_request[Client_ID][2]<1? 0:store_request[Client_ID][2]-1;
    store_request[Client_ID][3]=store_request[Client_ID][3]<1? 0:store_request[Client_ID][3]-1;
    store_request[Client_ID][4]=store_request[Client_ID][4]<1? 0:store_request[Client_ID][4]-1;



    status= CM_Q03_MQTT_Receive_Msg_Status(Client_ID,msgID,topic,size_of_topic,msg,size_of_msg);
    if(Success != status)
    {
        return status;
    }
//    status = CM_Q03_Read_Response_Start_and_Stop_read_character(qmtrecv_cid_1,sizeof(qmtrecv_cid_1)-1, msgID, &size_of_msgID, start_characte,start_characte);
//    msgID=msgID-48;
//    if(Success != status)
//    {
//        return status;
//    }
//
//    start_characte='"';
//    status=CM_Q03_Read_Response_Start_and_Stop_read_character(qmtrecv_cid_1,sizeof(qmtrecv_cid_1)-1, topic, size_of_topic, start_characte, start_characte);
//    if(Success != status)
//    {
//        return status;
//    }
//
//    status = CM_Q03_Read_Response_count_Start_and_Stop_read_character(qmtrecv_cid_1,sizeof(qmtrecv_cid_1)-1, msg, size_of_msg, start_characte, 3,start_characte);
//    if(Success != status)
//    {
//        return status;
//    }


    return status;
}


urc_received_status_t CM_Q03_MQTT_Close_Network_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTCLOSE_CID_0_POS)]
							>> (MODULE_8_SFT(QMTCLOSE_CID_0_POS))) & 0x01);
		break;

		case 1:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTCLOSE_CID_1_POS)]
							>> (MODULE_8_SFT(QMTCLOSE_CID_1_POS))) & 0x01);
		break;

		case 2:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTCLOSE_CID_2_POS)]
							>> (MODULE_8_SFT(QMTCLOSE_CID_2_POS))) & 0x01);
		break;

		default:
			return invalid_ID;
		break;
	}
}

cm_q03_codes_t CM_Q03_MQTT_Clear_Close_Network_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			urc_flags[DIV_BY_8(QMTCLOSE_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QMTCLOSE_CID_0_POS));

			return Success;
		break;

		case 1:
			urc_flags[DIV_BY_8(QMTCLOSE_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QMTCLOSE_CID_1_POS));

			return Success;
		break;

		case 2:
			urc_flags[DIV_BY_8(QMTCLOSE_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QMTCLOSE_CID_2_POS));

			return Success;
		break;

		default:
			return MQTT_Invalid_Connect_ID;
		break;
	}
}

void CM_Q03_MQTT_Close_Network_Status(uint8_t connectID,
		cm_q03_codes_t *status)
{
	switch(connectID)
	{
		case 0:
			(*status) = (cm_q03_codes_t)(qmtclose_cid_0 + MQTT_Network_closed_succesfully);

			urc_flags[DIV_BY_8(QMTCLOSE_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QMTCLOSE_CID_0_POS));
		break;

		case 1:
			(*status) = (cm_q03_codes_t)(qmtclose_cid_1 + MQTT_Network_closed_succesfully);

			urc_flags[DIV_BY_8(QMTCLOSE_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QMTCLOSE_CID_1_POS));
		break;

		case 2:
			(*status) = (cm_q03_codes_t)(qmtclose_cid_2 + MQTT_Network_closed_succesfully);

			urc_flags[DIV_BY_8(QMTCLOSE_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QMTCLOSE_CID_2_POS));
		break;

		default:
			(*status) = MQTT_Invalid_Connect_ID;
		break;
	}
}

urc_received_status_t CM_Q03_MQTT_Connect_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTCONN_CID_0_POS)]
							>> (MODULE_8_SFT(QMTCONN_CID_0_POS))) & 0x01);
		break;

		case 1:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTCONN_CID_1_POS)]
							>> (MODULE_8_SFT(QMTCONN_CID_1_POS))) & 0x01);
		break;

		case 2:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTCONN_CID_2_POS)]
							>> (MODULE_8_SFT(QMTCONN_CID_2_POS))) & 0x01);
		break;

		default:
			return invalid_ID;
		break;
	}
}

cm_q03_codes_t CM_Q03_MQTT_Clear_Connect_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			urc_flags[DIV_BY_8(QMTCONN_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QMTCONN_CID_0_POS));

			return Success;
		break;

		case 1:
			urc_flags[DIV_BY_8(QMTCONN_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QMTCONN_CID_1_POS));

			return Success;
		break;

		case 2:
			urc_flags[DIV_BY_8(QMTCONN_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QMTCONN_CID_2_POS));

			return Success;
		break;

		default:
			return MQTT_Invalid_Connect_ID;
		break;
	}
}

void CM_Q03_MQTT_Connect_Status(uint8_t connectID, cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status)
{
	switch(connectID)
	{
		case 0:
			(*status) = (qmtconn_cid_0.u16_0 + MQTT_Packet_sent_successfully);
			(*protocol_status) = (qmtconn_cid_0.u16_1 + MQTT_Connection_Accepted);

			urc_flags[DIV_BY_8(QMTCONN_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QMTCONN_CID_0_POS));
		break;

		case 1:
			(*status) = (qmtconn_cid_1.u16_0 + MQTT_Packet_sent_successfully);
			(*protocol_status) = (qmtconn_cid_1.u16_1 + MQTT_Connection_Accepted);

			urc_flags[DIV_BY_8(QMTCONN_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QMTCONN_CID_1_POS));
		break;

		case 2:
			(*status) = (qmtconn_cid_2.u16_0 + MQTT_Packet_sent_successfully);
			(*protocol_status) = (qmtconn_cid_2.u16_1 + MQTT_Connection_Accepted);

			urc_flags[DIV_BY_8(QMTCONN_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QMTCONN_CID_2_POS));
		break;

		default:
			(*status) = MQTT_Invalid_Connect_ID;
			(*protocol_status) = MQTT_Invalid_Connect_ID;
		break;
	}
}

urc_received_status_t CM_Q03_MQTT_Disconnect_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTDISC_CID_0_POS)]
							>> (MODULE_8_SFT(QMTDISC_CID_0_POS))) & 0x01);
		break;

		case 1:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTDISC_CID_1_POS)]
							>> (MODULE_8_SFT(QMTDISC_CID_1_POS))) & 0x01);
		break;

		case 2:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTDISC_CID_2_POS)]
							>> (MODULE_8_SFT(QMTDISC_CID_2_POS))) & 0x01);
		break;

		default:
			return invalid_ID;
		break;
	}
}

cm_q03_codes_t CM_Q03_MQTT_Clear_Disconnect_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			urc_flags[DIV_BY_8(QMTDISC_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QMTDISC_CID_0_POS));

			return Success;
		break;

		case 1:
			urc_flags[DIV_BY_8(QMTDISC_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QMTDISC_CID_1_POS));

			return Success;
		break;

		case 2:
			urc_flags[DIV_BY_8(QMTDISC_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QMTDISC_CID_2_POS));

			return Success;
		break;

		default:
			return MQTT_Invalid_Connect_ID;
		break;
	}
}

void CM_Q03_MQTT_Disconnect_Status(uint8_t connectID, cm_q03_codes_t *status)
{
	switch(connectID)
	{
		case 0:
			(*status) = (cm_q03_codes_t)(qmtdisc_cid_0 + MQTT_Network_closed_succesfully);

			urc_flags[DIV_BY_8(QMTDISC_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QMTDISC_CID_0_POS));
		break;

		case 1:
			(*status) = (cm_q03_codes_t)(qmtdisc_cid_1 + MQTT_Network_closed_succesfully);

			urc_flags[DIV_BY_8(QMTDISC_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QMTDISC_CID_1_POS));
		break;

		case 2:
			(*status) = (cm_q03_codes_t)(qmtdisc_cid_2 + MQTT_Network_closed_succesfully);

			urc_flags[DIV_BY_8(QMTDISC_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QMTDISC_CID_2_POS));
		break;

		default:
			(*status) = MQTT_Invalid_Connect_ID;
		break;
	}
}

urc_received_status_t CM_Q03_MQTT_Open_Network_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTOPEN_CID_0_POS)]
							>> (MODULE_8_SFT(QMTOPEN_CID_0_POS))) & 0x01);
		break;

		case 1:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTOPEN_CID_1_POS)]
							>> (MODULE_8_SFT(QMTOPEN_CID_1_POS))) & 0x01);
		break;

		case 2:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTOPEN_CID_2_POS)]
							>> (MODULE_8_SFT(QMTOPEN_CID_2_POS))) & 0x01);
		break;

		default:
			return invalid_ID;
		break;
	}
}

cm_q03_codes_t CM_Q03_MQTT_Clear_Open_Network_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			urc_flags[DIV_BY_8(QMTOPEN_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QMTOPEN_CID_0_POS));

			return Success;
		break;

		case 1:
			urc_flags[DIV_BY_8(QMTOPEN_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QMTOPEN_CID_1_POS));

			return Success;
		break;

		case 2:
			urc_flags[DIV_BY_8(QMTOPEN_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QMTOPEN_CID_2_POS));

			return Success;
		break;

		default:
			return MQTT_Invalid_Connect_ID;
		break;
	}
}

void CM_Q03_MQTT_Open_Network_Status(uint8_t connectID, cm_q03_codes_t *status)
{
	switch(connectID)
	{
		case 0:
			if((-1) == qmtopen_cid_0)
			{
				(*status) = MQTT_Failed_Open_Network;
			}

			else
			{
				(*status) = (cm_q03_codes_t)(qmtopen_cid_0 + MQTT_Network_Opened_Successfully);
			}

			urc_flags[DIV_BY_8(QMTOPEN_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QMTOPEN_CID_0_POS));
		break;

		case 1:
			if((-1) == qmtopen_cid_1)
			{
				(*status) = MQTT_Failed_Open_Network;
			}

			else
			{
				(*status) = (cm_q03_codes_t)(qmtopen_cid_1 + MQTT_Network_Opened_Successfully);
			}

			urc_flags[DIV_BY_8(QMTOPEN_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QMTOPEN_CID_1_POS));
		break;

		case 2:
			if((-1) == qmtopen_cid_2)
			{
				(*status) = MQTT_Failed_Open_Network;
			}

			else
			{
				(*status) = (cm_q03_codes_t)(qmtopen_cid_2 + MQTT_Network_Opened_Successfully);
			}

			urc_flags[DIV_BY_8(QMTOPEN_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QMTOPEN_CID_2_POS));
		break;

		default:
			(*status) = MQTT_Invalid_Connect_ID;
		break;
	}
}

urc_received_status_t CM_Q03_MQTT_Publish_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTPUB_CID_0_POS)]
							>> (MODULE_8_SFT(QMTPUB_CID_0_POS))) & 0x01);
		break;

		case 1:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTPUB_CID_1_POS)]
							>> (MODULE_8_SFT(QMTPUB_CID_1_POS))) & 0x01);
		break;

		case 2:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTPUB_CID_2_POS)]
							>> (MODULE_8_SFT(QMTPUB_CID_2_POS))) & 0x01);
		break;

		default:
			return invalid_ID;
		break;
	}
}

cm_q03_codes_t CM_Q03_MQTT_Clear_Publish_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			urc_flags[DIV_BY_8(QMTPUB_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QMTPUB_CID_0_POS));

			return Success;
		break;

		case 1:
			urc_flags[DIV_BY_8(QMTPUB_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QMTPUB_CID_1_POS));

			return Success;
		break;

		case 2:
			urc_flags[DIV_BY_8(QMTPUB_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QMTPUB_CID_2_POS));

			return Success;
		break;

		default:
			return MQTT_Invalid_Connect_ID;
		break;
	}
}

void CM_Q03_MQTT_Publish_Status(uint8_t connectID, cm_q03_codes_t *status,
		uint16_t *msgID, uint16_t *retransmissions)
{
	switch(connectID)
	{
		case 0:
			(*msgID) = qmtpub_cid_0.u16_0;
			(*status) = (cm_q03_codes_t)(qmtpub_cid_0.u16_1 + MQTT_Packet_sent_successfully);


			if(MQTT_Packet_retransmission == (*status))
			{
				(*retransmissions) = qmtpub_cid_0.u16_2;
			}

			else
			{
				(*retransmissions) = 0;
			}

			urc_flags[DIV_BY_8(QMTPUB_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QMTPUB_CID_0_POS));
		break;

		case 1:
			(*msgID) = qmtpub_cid_1.u16_0;
			(*status) = (cm_q03_codes_t)(qmtpub_cid_1.u16_1 + MQTT_Packet_sent_successfully);


			if(MQTT_Packet_retransmission == (*status))
			{
				(*retransmissions) = qmtpub_cid_1.u16_2;
			}

			else
			{
				(*retransmissions) = 0;
			}

			urc_flags[DIV_BY_8(QMTPUB_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QMTPUB_CID_1_POS));
		break;

		case 2:
			(*msgID) = qmtpub_cid_2.u16_0;
			(*status) = (cm_q03_codes_t)(qmtpub_cid_2.u16_1 + MQTT_Packet_sent_successfully);


			if(MQTT_Packet_retransmission == (*status))
			{
				(*retransmissions) = qmtpub_cid_2.u16_2;
			}

			else
			{
				(*retransmissions) = 0;
			}

			urc_flags[DIV_BY_8(QMTPUB_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QMTPUB_CID_2_POS));
		break;

		default:
			(*msgID) = 0;
			(*status) = MQTT_Invalid_Connect_ID;
			(*retransmissions) = 0;
		break;
	}
}

urc_received_status_t CM_Q03_MQTT_Receive_Msg_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTRECV_CID_0_POS)]
							>> (MODULE_8_SFT(QMTRECV_CID_0_POS))) & 0x01);
		break;

		case 1:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTRECV_CID_1_POS)]
							>> (MODULE_8_SFT(QMTRECV_CID_1_POS))) & 0x01);
		break;

		case 2:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTRECV_CID_2_POS)]
							>> (MODULE_8_SFT(QMTRECV_CID_2_POS))) & 0x01);
		break;

		default:
			return invalid_ID;
		break;
	}
}

cm_q03_codes_t CM_Q03_MQTT_Clear_Receive_Msg_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			urc_flags[DIV_BY_8(QMTRECV_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QMTRECV_CID_0_POS));

			return Success;
		break;

		case 1:
			urc_flags[DIV_BY_8(QMTRECV_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QMTRECV_CID_1_POS));

			return Success;
		break;

		case 2:
			urc_flags[DIV_BY_8(QMTRECV_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QMTRECV_CID_2_POS));

			return Success;
		break;

		default:
			return MQTT_Invalid_Connect_ID;
		break;
	}
}

cm_q03_codes_t CM_Q03_MQTT_Receive_Msg_Status(uint8_t connectID,
		uint16_t *msgID, uint8_t *topic, uint16_t *topic_size, uint8_t *msg,
		uint16_t *msg_size)
{
	uint8_t flag_quotes = 0;
	uint8_t flag_r = 0;
	uint8_t flag_n_or_r = 0;
	uint8_t flag_0 = 0;
	uint16_t cnt = 0;

	(*msgID) = 0;
	(*topic_size) = 0;
	(*msg_size) = 0;

	switch(connectID)
	{
		case 0:
            while(',' != qmtrecv_cid_0[cnt])
            {
                (*msgID) = MULT_BY_10(*msgID) + (qmtrecv_cid_0[cnt] - '0');

                cnt ++;
            }

            cnt ++;

            while((',' != qmtrecv_cid_0[cnt]) && (QMTRECV_SIZE > cnt))
            {
                if('"' != qmtrecv_cid_0[cnt])
                {
                    (*topic) = qmtrecv_cid_0[cnt];
                    topic ++;
                    (*topic_size) ++;
                }

                cnt ++;
            }

            do{
                cnt ++;
            }while('"'!= qmtrecv_cid_0[cnt]);
            cnt ++;

            while((!flag_quotes) && (!flag_r) && (!flag_n_or_r)
                    && (!flag_0) && (QMTRECV_SIZE > cnt))
            {
                (*msg) = qmtrecv_cid_0[cnt];
                msg ++;
                (*msg_size) ++;
                cnt ++;

//              flag_quotes =   (cnt >= QMTRECV_SIZE)       ? (1) :
//                              ('"' == qmtrecv_cid_0[cnt]) ? (1) : (0);
                flag_quotes=0;
                flag_r =    ((cnt + 1) >= QMTRECV_SIZE)         ? (1) :
                            ('\r' == qmtrecv_cid_0[cnt + 1])    ? (1) : (0);

//              flag_n_or_r =   ((cnt + 2) >= QMTRECV_SIZE)         ? (1) :
//                              ('\n' == qmtrecv_cid_0[cnt + 2])    ? (1) :
//                              ('\r'== qmtrecv_cid_0[cnt + 2])     ? (1) : (0);
                flag_n_or_r=0;

                flag_0 =    ((cnt + 2) >= QMTRECV_SIZE)         ? (1) :
                            ('\0' == qmtrecv_cid_0[cnt + 2])    ? (1) : (0);


            }


			urc_flags[DIV_BY_8(QMTRECV_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QMTRECV_CID_0_POS));
			memset(qmtrecv_cid_0, '\0', QMTRECV_SIZE);
		break;

		case 1:
			while(',' != qmtrecv_cid_1[cnt])
			{
				(*msgID) = MULT_BY_10(*msgID) + (qmtrecv_cid_1[cnt] - '0');

				cnt ++;
			}

			cnt ++;

			while((',' != qmtrecv_cid_1[cnt]) && (QMTRECV_SIZE > cnt))
			{
				if('"' != qmtrecv_cid_1[cnt])
				{
					(*topic) = qmtrecv_cid_1[cnt];
					topic ++;
					(*topic_size) ++;
				}

				cnt ++;
			}

			do{
			    cnt ++;
			}while('"'!= qmtrecv_cid_1[cnt]);
            cnt ++;

			while((!flag_quotes) && (!flag_r) && (!flag_n_or_r)
					&& (!flag_0) && (QMTRECV_SIZE > cnt))
			{
				(*msg) = qmtrecv_cid_1[cnt];
				msg ++;
				(*msg_size) ++;
				cnt ++;

//				flag_quotes = 	(cnt >= QMTRECV_SIZE) 		? (1) :
//								('"' == qmtrecv_cid_1[cnt])	? (1) : (0);
				flag_quotes=0;
//				flag_r = 	((cnt + 1) >= QMTRECV_SIZE) 		? (1) :
//							('\r' == qmtrecv_cid_1[cnt + 1])	? (1) : (0);
				flag_r=0;

				if(((cnt + 2) >= QMTRECV_SIZE) && (character_command_end == qmtrecv_cid_1[cnt + 3]) && (character_response_end== qmtrecv_cid_1[cnt + 4]) ){
				    flag_n_or_r =1;
				}
//				flag_n_or_r=0;

				flag_0 = 	((cnt + 2) >= QMTRECV_SIZE) 		? (1) :
							('\0' == qmtrecv_cid_1[cnt + 2])	? (1) : (0);


			}

			urc_flags[DIV_BY_8(QMTRECV_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QMTRECV_CID_1_POS));
			memset(qmtrecv_cid_1, '\0', QMTRECV_SIZE);
		break;

		case 2:
            while(',' != qmtrecv_cid_2[cnt])
            {
                (*msgID) = MULT_BY_10(*msgID) + (qmtrecv_cid_2[cnt] - '0');

                cnt ++;
            }

            cnt ++;

            while((',' != qmtrecv_cid_2[cnt]) && (QMTRECV_SIZE > cnt))
            {
                if('"' != qmtrecv_cid_2[cnt])
                {
                    (*topic) = qmtrecv_cid_2[cnt];
                    topic ++;
                    (*topic_size) ++;
                }

                cnt ++;
            }

            do{
                cnt ++;
            }while('"'!= qmtrecv_cid_2[cnt]);
            cnt ++;

            while((!flag_quotes) && (!flag_r) && (!flag_n_or_r)
                    && (!flag_0) && (QMTRECV_SIZE > cnt))
            {
                (*msg) = qmtrecv_cid_2[cnt];
                msg ++;
                (*msg_size) ++;
                cnt ++;

//              flag_quotes =   (cnt >= QMTRECV_SIZE)       ? (1) :
//                              ('"' == qmtrecv_cid_2[cnt]) ? (1) : (0);
                flag_quotes=0;
                flag_r =    ((cnt + 1) >= QMTRECV_SIZE)         ? (1) :
                            ('\r' == qmtrecv_cid_2[cnt + 1])    ? (1) : (0);

//              flag_n_or_r =   ((cnt + 2) >= QMTRECV_SIZE)         ? (1) :
//                              ('\n' == qmtrecv_cid_2[cnt + 2])    ? (1) :
//                              ('\r'== qmtrecv_cid_2[cnt + 2])     ? (1) : (0);
                flag_n_or_r=0;

                flag_0 =    ((cnt + 2) >= QMTRECV_SIZE)         ? (1) :
                            ('\0' == qmtrecv_cid_2[cnt + 2])    ? (1) : (0);


            }

			urc_flags[DIV_BY_8(QMTRECV_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QMTRECV_CID_2_POS));
			memset(qmtrecv_cid_2, '\0', QMTRECV_SIZE);
		break;

		default:
			return MQTT_Invalid_Connect_ID;
		break;
	}

	return Success;
}

urc_received_status_t CM_Q03_MQTT_Get_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTSTAT_CID_0_POS)]
							>> (MODULE_8_SFT(QMTSTAT_CID_0_POS))) & 0x01);
		break;

		case 1:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTSTAT_CID_1_POS)]
							>> (MODULE_8_SFT(QMTSTAT_CID_1_POS))) & 0x01);
		break;

		case 2:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTSTAT_CID_2_POS)]
							>> (MODULE_8_SFT(QMTSTAT_CID_2_POS))) & 0x01);
		break;

		default:
			return invalid_ID;
		break;
	}
}

cm_q03_codes_t CM_Q03_MQTT_Clear_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			urc_flags[DIV_BY_8(QMTSTAT_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QMTSTAT_CID_0_POS));

			return Success;
		break;

		case 1:
			urc_flags[DIV_BY_8(QMTSTAT_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QMTSTAT_CID_1_POS));

			return Success;
		break;

		case 2:
			urc_flags[DIV_BY_8(QMTSTAT_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QMTSTAT_CID_2_POS));

			return Success;
		break;

		default:
			return MQTT_Invalid_Connect_ID;
		break;
	}
}

void CM_Q03_MQTT_Get_Status(uint8_t connectID, cm_q03_codes_t *status)
{
	switch(connectID)
	{
		case 0:
			(*status) = (cm_q03_codes_t)(qmtstat_cid_0
					+ MQTT_Connection_Closed_Or_Reset_By_Peer);

			urc_flags[DIV_BY_8(QMTSTAT_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QMTSTAT_CID_0_POS));
		break;

		case 1:
			(*status) = (cm_q03_codes_t)(qmtstat_cid_1
					+ MQTT_Connection_Closed_Or_Reset_By_Peer);

			urc_flags[DIV_BY_8(QMTSTAT_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QMTSTAT_CID_1_POS));
		break;

		case 2:
			(*status) = (cm_q03_codes_t)(qmtstat_cid_2
					+ MQTT_Connection_Closed_Or_Reset_By_Peer);

			urc_flags[DIV_BY_8(QMTSTAT_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QMTSTAT_CID_2_POS));
		break;

		default:
			(*status) = MQTT_Invalid_Connect_ID;
		break;
	}
}

urc_received_status_t CM_Q03_MQTT_Subscribe_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTSUB_CID_0_POS)]
							>> (MODULE_8_SFT(QMTSUB_CID_0_POS))) & 0x01);
		break;

		case 1:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTSUB_CID_1_POS)]
							>> (MODULE_8_SFT(QMTSUB_CID_1_POS))) & 0x01);
		break;

		case 2:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTSUB_CID_2_POS)]
							>> (MODULE_8_SFT(QMTSUB_CID_2_POS))) & 0x01);
		break;

		default:
			return invalid_ID;
		break;
	}
}

cm_q03_codes_t CM_Q03_MQTT_Clear_Subscribe_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			urc_flags[DIV_BY_8(QMTSUB_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QMTSUB_CID_0_POS));

			return Success;
		break;

		case 1:
			urc_flags[DIV_BY_8(QMTSUB_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QMTSUB_CID_1_POS));

			return Success;
		break;

		case 2:
			urc_flags[DIV_BY_8(QMTSUB_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QMTSUB_CID_2_POS));

			return Success;
		break;

		default:
			return MQTT_Invalid_Connect_ID;
		break;
	}
}

void CM_Q03_MQTT_Subscribe_Status(uint8_t connectID,
		cm_q03_codes_t *status, uint16_t *msgID, uint16_t *value)
{
	switch(connectID)
	{
		case 0:
			(*msgID) = qmtsub_cid_0.u16_0;
			(*status) = (cm_q03_codes_t)(qmtsub_cid_0.u16_1
					+ MQTT_Packet_sent_successfully);

			if(MQTT_Failed_send_packet == (*status))
			{
				(*value) = 0;
			}

			else
			{
				(*value) = qmtsub_cid_0.u16_2;
			}

			urc_flags[DIV_BY_8(QMTSUB_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QMTSUB_CID_0_POS));
		break;

		case 1:
			(*msgID) = qmtsub_cid_1.u16_0;
			(*status) = (cm_q03_codes_t)(qmtsub_cid_1.u16_1
					+ MQTT_Packet_sent_successfully);

			if(MQTT_Failed_send_packet == (*status))
			{
				(*value) = 0;
			}

			else
			{
				(*value) = qmtsub_cid_1.u16_2;
			}

			urc_flags[DIV_BY_8(QMTSUB_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QMTSUB_CID_1_POS));
		break;

		case 2:
			(*msgID) = qmtsub_cid_2.u16_0;
			(*status) = (cm_q03_codes_t)(qmtsub_cid_2.u16_1
					+ MQTT_Packet_sent_successfully);

			if(MQTT_Failed_send_packet == (*status))
			{
				(*value) = 0;
			}

			else
			{
				(*value) = qmtsub_cid_2.u16_2;
			}

			urc_flags[DIV_BY_8(QMTSUB_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QMTSUB_CID_2_POS));
		break;

		default:
			(*msgID) = 0;
			(*status) = MQTT_Invalid_Connect_ID;
			(*value) = 0;
		break;
	}
}

urc_received_status_t CM_Q03_MQTT_Unsubscribe_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTUNS_CID_0_POS)]
							>> (MODULE_8_SFT(QMTUNS_CID_0_POS))) & 0x01);
		break;

		case 1:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTUNS_CID_1_POS)]
							>> (MODULE_8_SFT(QMTUNS_CID_1_POS))) & 0x01);
		break;

		case 2:
			return (urc_received_status_t)((urc_flags[DIV_BY_8(QMTUNS_CID_2_POS)]
							>> (MODULE_8_SFT(QMTUNS_CID_2_POS))) & 0x01);
		break;

		default:
			return invalid_ID;
		break;
	}
}

cm_q03_codes_t CM_Q03_MQTT_Clear_Unsubscribe_Status_Flag(uint8_t connectID)
{
	switch(connectID)
	{
		case 0:
			urc_flags[DIV_BY_8(QMTUNS_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QMTUNS_CID_0_POS));

			return Success;
		break;

		case 1:
			urc_flags[DIV_BY_8(QMTUNS_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QMTUNS_CID_1_POS));

			return Success;
		break;

		case 2:
			urc_flags[DIV_BY_8(QMTUNS_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QMTUNS_CID_2_POS));

			return Success;
		break;

		default:
			return MQTT_Invalid_Connect_ID;
		break;
	}
}

void CM_Q03_MQTT_Unsubscribe_Status(uint8_t connectID, cm_q03_codes_t *status,
		uint16_t *msgID)
{
	switch(connectID)
	{
		case 0:
			(*msgID) = qmtuns_cid_0.u16_0;
			(*status) = (cm_q03_codes_t)(qmtuns_cid_0.u16_1
					+ MQTT_Packet_sent_successfully);

			urc_flags[DIV_BY_8(QMTSUB_CID_0_POS)] &= ~(1
					<< MODULE_8_SFT(QMTSUB_CID_0_POS));
		break;

		case 1:
			(*msgID) = qmtuns_cid_1.u16_0;
			(*status) = (cm_q03_codes_t)(qmtuns_cid_1.u16_1
					+ MQTT_Packet_sent_successfully);

			urc_flags[DIV_BY_8(QMTSUB_CID_1_POS)] &= ~(1
					<< MODULE_8_SFT(QMTSUB_CID_1_POS));
		break;

		case 2:
			(*msgID) = qmtuns_cid_2.u16_0;
			(*status) = (cm_q03_codes_t)(qmtuns_cid_2.u16_1
					+ MQTT_Packet_sent_successfully);

			urc_flags[DIV_BY_8(QMTSUB_CID_2_POS)] &= ~(1
					<< MODULE_8_SFT(QMTSUB_CID_2_POS));
		break;

		default:
			(*msgID) = 0;
			(*status) = MQTT_Invalid_Connect_ID;
		break;
	}
}

urc_received_status_t CM_Q03_MQTT_Ready_Publish_Status_Flag(void)
{
    return(urc_received_status_t)((urc_flags[DIV_BY_8(QMTREADY_PUB_POS)]
                                             >> (MODULE_8_SFT(QMTREADY_PUB_POS))) & 0x01);
}

void CM_Q03_MQTT_Clear_Ready_Publish_Status_Flag(void)
{
    urc_flags[DIV_BY_8(QMTREADY_PUB_POS)] &= ~(1 << MODULE_8_SFT(QMTREADY_PUB_POS));
}


