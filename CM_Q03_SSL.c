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
 * \file       CM_Q03_SSL.h
 * \brief      This is the CM_Q03 SSL driver made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.\n
 *             **to identify the meaning of a CME error, please refer to the
 *             provided Quectel SSL AT commands manual.\n
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

#include <CM_Q03_SSL.h>

#define ALL_SUITES                              "0xFFFF"
#define TLS_RSA_WITH_AES_256_CBC_SHA            "0x0035"
#define TLS_RSA_WITH_AES_128_CBC_SHA            "0x002F"
#define TLS_RSA_WITH_RC4_128_SHA                "0x0005"
#define TLS_RSA_WITH_RC4_128_MD5                "0x0004"
#define TLS_RSA_WITH_3DES_EDE_CBC_SHA           "0x000A"
#define TLS_RSA_WITH_AES_256_CBC_SHA256         "0x003D"
#define TLS_ECDHE_RSA_WITH_RC4_128_SHA          "0xC011"
#define TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA     "0xC012"
#define TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA      "0xC013"
#define TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA      "0xC014"
#define TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256   "0xC027"
#define TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384   "0xC028"
#define TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256   "0xC02F"

#define SSL_TIMEOUT 500000

extern uint8_t character_command_end;
extern uint8_t command_comma;
extern uint8_t command_quotes;
extern uint8_t response_ok[2];

uint8_t ssl_configurations[11] = "AT+QSSLCFG=";
uint8_t ssl_version[12] = "\"sslversion\"";
uint8_t ssl_ciphersuite[13] = "\"ciphersuite\"";
uint8_t ssl_seclevel[10] = "\"seclevel\"";
uint8_t ssl_ignorelocaltime[17] = "\"ignorelocaltime\"";
uint8_t ssl_negotiatetime[15] = "\"negotiatetime\"";

uint8_t ssl_ca_cert[8] = "\"cacert\"";
uint8_t ssl_client_cert[12] = "\"clientcert\"";
uint8_t ssl_client_key[11] = "\"clientkey\"";

uint8_t ssl_open[12] = "AT+QSSLOPEN=";
uint8_t ssl_open_response[11] = "+QSSLOPEN: ";
uint8_t ssl_open_connect[7] = "CONNECT";

uint8_t ssl_send[12] = "AT+QSSLSEND=";

uint8_t ssl_recv[12] = "AT+QSSLRECV=";

uint8_t ssl_close[13] = "AT+QSSLCLOSE=";

uint8_t ssl_state[13] = "AT+QSSLSTATE=";

ssl_config_parameters_struct_t ssl_config[6];


cm_q03_codes_t CM_Q03_SSL_Configuration(uint8_t sslctxID,
                                  ssl_config_parameters_struct_t ssl_config_struct,
								  configuration_select_t get_default_config)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[33] = {0};
    uint8_t temp_buff[3] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};

    if (Default_Configuration == get_default_config)
    {
        // Configuration of SSL parameters
        ssl_config_struct.version = 4;
        ssl_config_struct.ciphersuite = 0;
        ssl_config_struct.seclevel = 2;
        ssl_config_struct.ignorelocaltime = 1;
        ssl_config_struct.negotiatetime = 300;
    }

    else
    {
        if(4 < ssl_config_struct.version || 0 > ssl_config_struct.version)
        {
            return User_Error;
        }
        if(13 < ssl_config_struct.ciphersuite || 0 > ssl_config_struct.ciphersuite)
        {
            return User_Error;
        }
        if(2 < ssl_config_struct.seclevel || 0 > ssl_config_struct.seclevel)
        {
            return User_Error;
        }
        if(1 < ssl_config_struct.ignorelocaltime || 0 > ssl_config_struct.ignorelocaltime)
        {
            return User_Error;
        }
        if(300 < ssl_config_struct.negotiatetime || 10 > ssl_config_struct.negotiatetime)
        {
            return User_Error;
        }
    }

    if(5 < sslctxID || 0 > sslctxID)
    {
        return User_Error;
    } else
    {
        ssl_config[sslctxID] = ssl_config_struct;
    }

    Concatenate_Command(cmd_buff, ssl_configurations, sizeof(ssl_configurations), &cmd_buff_size);
    Concatenate_Command(cmd_buff, ssl_version, sizeof(ssl_version), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(sslctxID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(ssl_config_struct.version, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, SSL_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, SSL_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), SSL_TIMEOUT);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }

    memset(&cmd_buff[sizeof(ssl_configurations)], '\0', sizeof(cmd_buff)-sizeof(ssl_configurations));
    cmd_buff_size = sizeof(ssl_configurations);
    Concatenate_Command(cmd_buff, ssl_ciphersuite, sizeof(ssl_ciphersuite), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(sslctxID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    switch(ssl_config_struct.ciphersuite){
        case all:
            Concatenate_Command(cmd_buff, (uint8_t*)ALL_SUITES, 6, &cmd_buff_size);
            break;
        case rsa_with_aes_256_sha:
            Concatenate_Command(cmd_buff, (uint8_t*)TLS_RSA_WITH_AES_256_CBC_SHA, 6, &cmd_buff_size);
            break;
        case rsa_with_aes_128_sha:
            Concatenate_Command(cmd_buff, (uint8_t*)TLS_RSA_WITH_AES_128_CBC_SHA, 6, &cmd_buff_size);
            break;
        case rsa_with_rc4_128_sha:
            Concatenate_Command(cmd_buff, (uint8_t*)TLS_RSA_WITH_RC4_128_SHA, 6, &cmd_buff_size);
            break;
        case rsa_with_rc4_128_md5:
            Concatenate_Command(cmd_buff, (uint8_t*)TLS_RSA_WITH_RC4_128_MD5, 6, &cmd_buff_size);
            break;
        case rsa_with_3des_ede:
            Concatenate_Command(cmd_buff, (uint8_t*)TLS_RSA_WITH_3DES_EDE_CBC_SHA, 6, &cmd_buff_size);
            break;
        case rsa_with_aes_256_sha256:
            Concatenate_Command(cmd_buff, (uint8_t*)TLS_RSA_WITH_AES_256_CBC_SHA256, 6, &cmd_buff_size);
            break;
        case ecdhe_rsa_with_rc4_128:
            Concatenate_Command(cmd_buff, (uint8_t*)TLS_ECDHE_RSA_WITH_RC4_128_SHA, 6, &cmd_buff_size);
            break;
        case ecdhe_rsa_with_3des_ede:
            Concatenate_Command(cmd_buff, (uint8_t*)TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA, 6, &cmd_buff_size);
            break;
        case ecdhe_rsa_with_aes_128:
            Concatenate_Command(cmd_buff, (uint8_t*)TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA, 6, &cmd_buff_size);
            break;
        case ecdhe_rsa_with_aes_256:
            Concatenate_Command(cmd_buff, (uint8_t*)TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA, 6, &cmd_buff_size);
            break;
        case ecdhe_rsa_with_aes_128_cbc_sha256:
            Concatenate_Command(cmd_buff, (uint8_t*)TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256, 6, &cmd_buff_size);
            break;
        case ecdhe_rsa_with_aes_256_sha384:
            Concatenate_Command(cmd_buff, (uint8_t*)TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384, 6, &cmd_buff_size);
            break;
        case ecdhe_rsa_with_aes_128_gcm_sha256:
            Concatenate_Command(cmd_buff, (uint8_t*)TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256, 6, &cmd_buff_size);
            break;
        default:
            Concatenate_Command(cmd_buff, (uint8_t*)ALL_SUITES, 6, &cmd_buff_size);
            break;
    }
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, SSL_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, SSL_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), SSL_TIMEOUT);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }

    memset(&cmd_buff[sizeof(ssl_configurations)], '\0', sizeof(cmd_buff)-sizeof(ssl_configurations));
    cmd_buff_size = sizeof(ssl_configurations);
    Concatenate_Command(cmd_buff, ssl_seclevel, sizeof(ssl_seclevel), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(sslctxID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(ssl_config_struct.seclevel, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, SSL_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, SSL_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), SSL_TIMEOUT);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }

    memset(&cmd_buff[sizeof(ssl_configurations)], '\0', sizeof(cmd_buff)-sizeof(ssl_configurations));
    cmd_buff_size = sizeof(ssl_configurations);
    Concatenate_Command(cmd_buff, ssl_ignorelocaltime, sizeof(ssl_ignorelocaltime), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(sslctxID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(ssl_config_struct.ignorelocaltime, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, SSL_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, SSL_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), SSL_TIMEOUT);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }

    memset(&cmd_buff[sizeof(ssl_configurations)], '\0', sizeof(cmd_buff)-sizeof(ssl_configurations));
    cmd_buff_size = sizeof(ssl_configurations);
    Concatenate_Command(cmd_buff, ssl_negotiatetime, sizeof(ssl_negotiatetime), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(sslctxID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(ssl_config_struct.negotiatetime, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, SSL_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, SSL_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), SSL_TIMEOUT);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }

    return status;
}

cm_q03_codes_t CM_Q03_SSL_CA_Certificate(uint8_t sslctxID,
                                   uint8_t *ca_path,
                                   uint16_t size_ca_path)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[280] = {0};
    uint8_t temp_buff = 0;
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};

    if(5 < sslctxID || 0 > sslctxID)
    {
        return User_Error;
    }
    if(1 > size_ca_path || 255 < size_ca_path)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, ssl_configurations, sizeof(ssl_configurations), &cmd_buff_size);
    Concatenate_Command(cmd_buff, ssl_ca_cert, sizeof(ssl_ca_cert), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(sslctxID, &temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, &temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, ca_path, size_ca_path, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, SSL_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, SSL_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), SSL_TIMEOUT);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }
    return status;
}

cm_q03_codes_t CM_Q03_SSL_Client_Certificate(uint8_t sslctxID,
                                       uint8_t *cc_path,
                                       uint16_t size_cc_path)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[280] = {0};
    uint8_t temp_buff = 0;
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};

    if(5 < sslctxID || 0 > sslctxID)
    {
        return User_Error;
    }
    if(1 > size_cc_path || 255 < size_cc_path)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, ssl_configurations, sizeof(ssl_configurations), &cmd_buff_size);
    Concatenate_Command(cmd_buff, ssl_client_cert, sizeof(ssl_client_cert), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(sslctxID, &temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, &temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, cc_path, size_cc_path, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, SSL_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, SSL_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), SSL_TIMEOUT);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }
    return status;
}

cm_q03_codes_t CM_Q03_SSL_Client_Private_Key(uint8_t sslctxID,
                                       uint8_t *key_path,
                                       uint16_t size_key_path)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[100] = {0};
    uint8_t temp_buff[2] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};

    if(5 < sslctxID || 0 > sslctxID)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, ssl_configurations, sizeof(ssl_configurations), &cmd_buff_size);
    Concatenate_Command(cmd_buff, ssl_client_key, sizeof(ssl_client_key), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(sslctxID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, key_path, size_key_path, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, SSL_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &buffer_size, SSL_TIMEOUT);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), SSL_TIMEOUT);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }
    return status;
}

cm_q03_codes_t CM_Q03_SSL_Open_Socket(uint8_t pdpctxID, uint8_t sslctxID, uint8_t clientID,
                                uint8_t *server_addr, uint16_t server_addr_size,
                                uint16_t server_port, uint8_t acces_mode)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[100] = {0};
    uint8_t temp_buff[2] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};

    if(1 > pdpctxID || 16 < pdpctxID)
    {
        return User_Error;
    }
    if(0 > sslctxID || 5 < sslctxID)
    {
        return User_Error;
    }
    if(0 > clientID || 11 < clientID)
    {
        return User_Error;
    }
    if(0 > acces_mode || 2 < acces_mode)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, ssl_open, sizeof(ssl_open), &cmd_buff_size);
    Dec_to_Ascii(pdpctxID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(sslctxID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(clientID, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, server_addr, server_addr_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(server_port, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(acces_mode, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
    status = CM_Q03_Read(rxbuff, &buffer_size, (150+ssl_config[sslctxID].negotiatetime)*1000000);
    if(acces_mode == 2)
    {
        status = CM_Q03_Compare_short_response(rxbuff, buffer_size, ssl_open_connect, sizeof(ssl_open_connect), 500000);
    }else
    {
        status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), 500000);
    }
    if(2 == status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }

    status = CM_Q03_Read(rxbuff, &buffer_size, (150+ssl_config[sslctxID].negotiatetime)*1000000);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, ssl_open_response, sizeof(ssl_open_response), 500000);
    if(0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status;
    }
    return status;
}
