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
 * \file       CM_Q03_GNSS.h
 * \brief      This is the CM_Q03 GNSS driver made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.\n
 *             **to identify the meaning of a CME error, please refer to the
 *             provided Quectel GNSS AT commands manual.\n
 * \version    1.1.1
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

#include <CM_Q03_GNSS.h>

#define QIND_GEOFENCE_POS	(15)

extern uint8_t urc_flags[URC_FLAG_SIZE];
extern uint8_t qind_geofence[QIND_GEOFENCE_SIZE];
extern uint8_t character_response_end ;
extern uint8_t character_command_end;
extern uint8_t command_comma;
extern uint8_t command_quotes;
extern uint8_t response_ok[2];



uint8_t gnss_configurations[11] = "AT+QGPSCFG=";

uint8_t gnss_constellation[12] = "\"gnssconfig\"";
uint8_t gnss_nmeafmt_source[9] = "\"nmeafmt\"";
uint8_t gnss_gps_type[13] = "\"gpsnmeatype\"";
uint8_t gnss_glonass_type[17] = "\"glonassnmeatype\"";
uint8_t gnss_galileo_type[17] = "\"galileonmeatype\"";
uint8_t gnss_beidoun_type[16] = "\"beidounmeatype\"";
uint8_t gnss_nmea_source[9] = "\"nmeasrc\"";
uint8_t gnss_auto_gps[9] = "\"autogps\"";
uint8_t gnss_priority_gps[10] = "\"priority\"";


//uint8_t gnss_delete_assist[11] = "AT+QGPSDEL=";

uint8_t gnss_turn_on[8] = "AT+QGPS=";

uint8_t gnss_turn_off[10] = "AT+QGPSEND";

uint8_t gnss_position_info[11] = "AT+QGPSLOC=";
uint8_t gnss_position_get[12] = "AT+QGPSLOC=2";
uint8_t gnss_position_response[10] = "+QGPSLOC: ";

uint8_t gnss_nmea_sentences[13] = "AT+QGPSGNMEA=";
uint8_t gnss_nmea_gga[5] = "\"GGA\"";
uint8_t gnss_nmea_rmc[5] = "\"RMC\"";
uint8_t gnss_nmea_gsv[5] = "\"GSV\"";
uint8_t gnss_nmea_gsa[5] = "\"GSA\"";
uint8_t gnss_nmea_vtg[5] = "\"VTG\"";
uint8_t gnss_nmea_gns[5] = "\"GNS\"";
uint8_t gnss_nmea_response[12] = "+QGPSGNMEA: ";

//uint8_t gnss_gpsOneXtra[12] = "AT+QGPSXTRA=";
//
//uint8_t gnss_gpsOneXtra_time[16] = "AT+QGPSXTRATIME=";
//
//uint8_t gnss_gpsOneXtra_data[16] = "AT+QGPSXTRADATA=";
//
//uint8_t gnss_ext_configurations[11] = "AT+QCFGEXT=";
//uint8_t gnss_add_geofence[8] = "\"addgeo\"";
//uint8_t gnss_delete_geo[11] = "\"deletegeo\"";
//uint8_t gnss_query_geo[10] = "\"querygeo\"";
//uint8_t gnss_geo_response[21] = "+QCFGEXT: \"querygeo\",";

enum GNSS_codes
{
	_GNSS_Invalid_parameter = 501,
	_GNSS_Operation_not_supported,
	_GNSS_subsystem_busy,
	_GNSS_Session_ir_ongoing,
	_GNSS_Session_not_active,
	_GNSS_Operation_timeout,
	_GNSS_Function_not_enabled,
	_GNSS_Time_information_error,
	_GNSS_XTRA_not_enabled,
	_GNSS_Validity_time_out_of_range = 512,
	_GNSS_Internal_resource_error,
	_GNSS_locked,
	_GNSS_End_by_E911,
	_GNSS_Not_fixed_now,
	_GNSS_GeoFence_ID_not_existed,
	_GNSS_Unknown_error = 549
};

cm_q03_codes_t CM_Q03_GNSS_Configuration(gnss_config_t gnss_config_struct,
										configuration_select_t get_default_config)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[31] = {0};
    uint8_t temp_buff[2] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};

    if(Default_Configuration == get_default_config)
    {
        gnss_config_struct.nmea_src = nmea_src_enable;
        gnss_config_struct.gps_type = gps_all_sentences_outputted;
        gnss_config_struct.glonass_type = glonass_disable_sentences;
        gnss_config_struct.galileo_type = galileo_disable_sentences;
        gnss_config_struct.beidou_type = beidou_disable_sentences;
        gnss_config_struct.nmeafmt_config=sentence_qualcomm_standards;
        gnss_config_struct.gnss_priority=gnss_high_priority;
//        gnss_config_struct.gsvext_type = gsvext_disable;
        gnss_config_struct.gnss_config = gps_on_glonass_on_beidou_off_galileo_off_qzss_off;
        gnss_config_struct.auto_gps = auto_disable;
    }

    else
    {
        if (0 > gnss_config_struct.nmea_src || 1 < gnss_config_struct.nmea_src)
        {
            return User_Error;
        }
        if (0 > gnss_config_struct.gps_type || 31 < gnss_config_struct.gps_type)
        {
            return User_Error;
        }
        if (0 > gnss_config_struct.glonass_type || 4 < gnss_config_struct.glonass_type)
        {
            return User_Error;
        }
        if (0 > gnss_config_struct.galileo_type || 1 < gnss_config_struct.galileo_type)
        {
            return User_Error;
        }
        if (0 > gnss_config_struct.beidou_type || 2 < gnss_config_struct.beidou_type)
        {
            return User_Error;
        }

        if (0 > gnss_config_struct.beidou_type || 2 < gnss_config_struct.beidou_type)
        {
            return User_Error;
        }

        if (0 > gnss_config_struct.nmeafmt_config || 1 < gnss_config_struct.nmeafmt_config)
        {
            return User_Error;
        }

        if (0 > gnss_config_struct.gnss_priority || 1 < gnss_config_struct.gnss_priority)
        {
            return User_Error;
        }

//        if (0 > gnss_config_struct.gsvext_type || 1 < gnss_config_struct.gsvext_type)
//        {
//            return User_Error;
//        }
        if (1 > gnss_config_struct.gnss_config || 6 < gnss_config_struct.gnss_config)
        {
            return User_Error;
        }
        if (0 > gnss_config_struct.auto_gps || 1 < gnss_config_struct.auto_gps)
        {
            return User_Error;
        }
    }

    Concatenate_Command(cmd_buff, gnss_configurations, sizeof(gnss_configurations), &cmd_buff_size);
    Concatenate_Command(cmd_buff, gnss_nmea_source, sizeof(gnss_nmea_source), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(gnss_config_struct.nmea_src, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Read(rxbuff, &buffer_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), 5000000);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + GNSS_CODES;
    }

    memset(&cmd_buff[sizeof(gnss_configurations)], '\0', sizeof(cmd_buff)-sizeof(gnss_configurations));
    cmd_buff_size = sizeof(gnss_configurations);
    Concatenate_Command(cmd_buff, gnss_gps_type, sizeof(gnss_gps_type), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(gnss_config_struct.gps_type, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Read(rxbuff, &buffer_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), 5000000);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + GNSS_CODES;
    }

    memset(&cmd_buff[sizeof(gnss_configurations)], '\0', sizeof(cmd_buff)-sizeof(gnss_configurations));
    cmd_buff_size = sizeof(gnss_configurations);
    Concatenate_Command(cmd_buff, gnss_glonass_type, sizeof(gnss_glonass_type), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(gnss_config_struct.glonass_type, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Read(rxbuff, &buffer_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), 5000000);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + GNSS_CODES;
    }

    memset(&cmd_buff[sizeof(gnss_configurations)], '\0', sizeof(cmd_buff)-sizeof(gnss_configurations));
    cmd_buff_size = sizeof(gnss_configurations);
    Concatenate_Command(cmd_buff, gnss_galileo_type, sizeof(gnss_galileo_type), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(gnss_config_struct.galileo_type, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Read(rxbuff, &buffer_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), 5000000);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + GNSS_CODES;
    }

    memset(&cmd_buff[sizeof(gnss_configurations)], '\0', sizeof(cmd_buff)-sizeof(gnss_configurations));
    cmd_buff_size = sizeof(gnss_configurations);
    Concatenate_Command(cmd_buff, gnss_beidoun_type, sizeof(gnss_beidoun_type), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(gnss_config_struct.beidou_type, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Read(rxbuff, &buffer_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), 5000000);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + GNSS_CODES;
    }



    memset(&cmd_buff[sizeof(gnss_configurations)], '\0', sizeof(cmd_buff)-sizeof(gnss_configurations));
    cmd_buff_size = sizeof(gnss_configurations);
    Concatenate_Command(cmd_buff, gnss_nmeafmt_source, sizeof(gnss_nmeafmt_source), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(gnss_config_struct.beidou_type, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Read(rxbuff, &buffer_size, 5000000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), 5000000);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + GNSS_CODES;
    }

    memset(&cmd_buff[sizeof(gnss_configurations)], '\0', sizeof(cmd_buff)-sizeof(gnss_configurations));
    cmd_buff_size = sizeof(gnss_configurations);
    Concatenate_Command(cmd_buff, gnss_priority_gps, sizeof(gnss_priority_gps), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(gnss_config_struct.beidou_type, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Read(rxbuff, &buffer_size, 5000000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), 5000000);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + GNSS_CODES;
    }


//    memset(&cmd_buff[sizeof(gnss_configurations)], '\0', sizeof(cmd_buff)-sizeof(gnss_configurations));
//    cmd_buff_size = sizeof(gnss_configurations);
//    Concatenate_Command(cmd_buff, gnss_gsvext_type, sizeof(gnss_gsvext_type), &cmd_buff_size);
//    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
//    Dec_to_Ascii(gnss_config_struct.gsvext_type, temp_buff, &buffer_size);
//    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
//    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
//
//    Delay(50000);
//    CM_Q03_buffer_clean();
//
//    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
//    if(Timeout == status)
//    {
//    	return status;
//    }
//    status = CM_Q03_Read(rxbuff, &buffer_size, 5000000);
//    if(Timeout == status)
//    {
//    	return status;
//    }
//    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), 5000000);
//    if(0 != status){
//        status = CM_Q03_Get_Response(rxbuff, buffer_size);
//        return status + GNSS_CODES;
//    }

    memset(&cmd_buff[sizeof(gnss_configurations)], '\0', sizeof(cmd_buff)-sizeof(gnss_configurations));
    cmd_buff_size = sizeof(gnss_configurations);
    Concatenate_Command(cmd_buff, gnss_constellation, sizeof(gnss_constellation), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(gnss_config_struct.gnss_config, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Read(rxbuff, &buffer_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), 5000000);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + GNSS_CODES;
    }

    memset(&cmd_buff[sizeof(gnss_configurations)], '\0', sizeof(cmd_buff)-sizeof(gnss_configurations));
    cmd_buff_size = sizeof(gnss_configurations);
    Concatenate_Command(cmd_buff, gnss_auto_gps, sizeof(gnss_auto_gps), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(gnss_config_struct.auto_gps, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Read(rxbuff, &buffer_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), 5000000);
    if(0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + GNSS_CODES;
    }
    return status;
}

//TODO: Update the structure GNSS_operation_modes_t when Quectel updates the working modes.
cm_q03_codes_t CM_Q03_GNSS_Turn_On(GNSS_operation_modes_t mode, max_positioning_accuracy_t max_dist,
                                   uint16_t count, uint16_t rate)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint16_t temp_index = 0;
    uint8_t cmd_buff[30] = {0};
    uint8_t temp_buff[5] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};

    if (1 != mode)
    {
        return User_Error;
    }

    if (1 > max_dist || 3 < max_dist)
    {
        return User_Error;
    }
    if (0 > count || 1000 < count)
    {
        return User_Error;
    }
    if (1 > rate)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, gnss_turn_on, sizeof(gnss_turn_on), &cmd_buff_size);
    Dec_to_Ascii(mode, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(max_dist, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(count, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Dec_to_Ascii(rate, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Read(rxbuff, &buffer_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_response(rxbuff, buffer_size, response_ok,
                                     sizeof(response_ok), sizeof(response_ok),
                                     &temp_index, 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + GNSS_CODES;
    }
    return status;
}

cm_q03_codes_t CM_Q03_GNSS_Turn_Off(void)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[11] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};

    Concatenate_Command(cmd_buff, gnss_turn_off, sizeof(gnss_turn_off), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Read(rxbuff, &buffer_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), 5000000);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + GNSS_CODES;
    }
    return status;
}

cm_q03_codes_t CM_Q03_GNSS_Location(location_format_t mode, uint8_t *response, uint16_t *response_size)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint16_t temp_index = 0;
    uint16_t temp_size = 1;
    uint8_t cmd_buff[13] = {0};
    uint8_t temp_buff = 0;
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[95] = {0};

    if (0 > mode || 2 < mode)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, gnss_position_info, sizeof(gnss_position_info), &cmd_buff_size);
    Dec_to_Ascii(mode, &temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, &temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    character_response_end='\n';
    status = CM_Q03_Read(rxbuff, &buffer_size, 5000000);
    character_response_end='\r';
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_response(rxbuff, buffer_size, gnss_position_response, sizeof(gnss_position_response), sizeof(gnss_position_response), &temp_index, 5000000);
    if(2 == status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + GNSS_CODES;
    }
    while(0 < (buffer_size - temp_index)){
        response[temp_size-1] = rxbuff[temp_index];
        temp_index++;
        temp_size++;
    }
    *response_size = temp_size;
    status = CM_Q03_Read(rxbuff, &buffer_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), 5000000);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + GNSS_CODES;
    }
    return status;
}

cm_q03_codes_t CM_Q03_GNSS_Get_Location(uint8_t *latitude, uint16_t *latitude_size,
										uint8_t *longitude, uint16_t *longitude_size)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint16_t temp_index = 0;
    uint16_t temp_size = 0;
    uint8_t cmd_buff[12] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[95] = {0};

    Concatenate_Command(cmd_buff, gnss_position_get, sizeof(gnss_position_get), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    character_response_end='\n';
    status = CM_Q03_Read(rxbuff, &buffer_size, 5000000);
    character_response_end='\r';
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_response(rxbuff, buffer_size, gnss_position_response,
    								 sizeof(gnss_position_response), sizeof(gnss_position_response), &temp_index, 5000000);
    if(2 == status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + GNSS_CODES;
    }
    while(',' != rxbuff[temp_index++]);
    while(',' != rxbuff[temp_index])
    {
    	latitude[temp_size] = rxbuff[temp_index];
        temp_index++;
        temp_size++;
    }
    *latitude_size = temp_size;
    temp_index++;
    temp_size = 0;
    while(',' != rxbuff[temp_index])
    {
    	longitude[temp_size] = rxbuff[temp_index];
        temp_index++;
        temp_size++;
    }
    *longitude_size = temp_size;

    status = CM_Q03_Read(rxbuff, &buffer_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), 5000000);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + GNSS_CODES;
    }
    return status;
}

cm_q03_codes_t CM_Q03_GNSS_NMEA(NMEA_sentences_types_t sentence_type, uint8_t *sentence, uint16_t *sentence_size)
{
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[20] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[100] = {0};
    uint8_t *temp_ptr = NULL;

    if (0 > sentence_type || 5 < sentence_type)
    {
        return User_Error;
    }

    Concatenate_Command(cmd_buff, gnss_nmea_sentences, sizeof(gnss_nmea_sentences), &cmd_buff_size);
    switch(sentence_type){
        case GGA:
            Concatenate_Command(cmd_buff, gnss_nmea_gga, sizeof(gnss_nmea_gga), &cmd_buff_size);
            break;
        case RMC:
            Concatenate_Command(cmd_buff, gnss_nmea_rmc, sizeof(gnss_nmea_rmc), &cmd_buff_size);
            break;
        case GSV:
            Concatenate_Command(cmd_buff, gnss_nmea_gsv, sizeof(gnss_nmea_gsv), &cmd_buff_size);
            break;
        case GSA:
            Concatenate_Command(cmd_buff, gnss_nmea_gsa, sizeof(gnss_nmea_gsa), &cmd_buff_size);
            break;
        case VTG:
            Concatenate_Command(cmd_buff, gnss_nmea_vtg, sizeof(gnss_nmea_vtg), &cmd_buff_size);
            break;
        default:
            Concatenate_Command(cmd_buff, gnss_nmea_gga, sizeof(gnss_nmea_gga), &cmd_buff_size);
            break;
    }
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    character_response_end='\n';
    status = CM_Q03_Read(rxbuff, &buffer_size, 5000000);
    character_response_end='\r';
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, gnss_nmea_response, sizeof(gnss_nmea_response), 5000000);
    if(2 == status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + GNSS_CODES;
    }
    temp_ptr = (uint8_t*)strchr((const char*)rxbuff, '$');
    strcpy((char*)sentence, (const char*)temp_ptr);
    *sentence_size = buffer_size - sizeof(gnss_nmea_response);
    status = CM_Q03_Read(rxbuff, &buffer_size, 5000000);
    status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok, sizeof(response_ok), 5000000);
    if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, buffer_size);
        return status + GNSS_CODES;
    }
    return status;
}
