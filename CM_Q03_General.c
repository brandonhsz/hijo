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
 * \file       CM_Q03_General.h
 * \brief      This is the CM_Q03 General functions made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.\n
 *             **to identify the meaning of a CME error, please refer to the
 *             provided Quectel AT commands manual.\n
 * \version    1.0.1
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////


#include <CM_Q03_General.h>

/************** URC POS ************************/
#define CFUN_POS					(19)
#define CPIN_POS					(76)
#define CREG_POS					(21)
#define POWERED_DOWN_POS			(1)
#define RDY_POS						(0)
/************** URC POS ************************/

#define CLEAR_URC			(0)

#define SIM_CODES			(10)
#define PIN_LENGTH			(4)
#define PUK_LENGTH			(8)

#define URC_READY			(53)
#define URC_SIM_PIN			(80)
#define URC_SIM_PUK			(89)
#define URC_SIM_PIN2		(802)
#define URC_SIM_PUK2		(892)
#define URC_PH_NET_PIN		(102)
#define URC_PH_NET_PUK		(111)
#define URC_PH_NETSUB_PIN	(144)
#define URC_PH_NETSUB_PUK	(153)
#define URC_PH_SP_PIN		(98)
#define URC_PH_SP_PUK		(107)
#define URC_PH_CORP_PIN		(115)
#define URC_PH_CORP_PUK		(124)

#define SIM_CODES			(10)
#define PIN_LENGTH			(4)
#define PUK_LENGTH			(8)

extern uint8_t response_ok[2];
extern uint8_t character_command_end;
extern uint8_t command_quotes;
extern uint8_t command_comma;

extern uint16_t cfun;
extern uint16_t cpin;
extern uint16_t creg;
extern no_params_flags_t no_param_urc;
extern uint8_t urc_flags[URC_FLAG_SIZE];

uint8_t general_config_rat[8]={"AT+QCFG="};
uint8_t general_nwscanseq[12]={"\"nwscanseq\","};
uint8_t general_nwscan[13]={"\"nwscanmode\","};
uint8_t general_iotop_mode[12]={"\"iotopmode\","};
uint8_t general_roamservice[13]={"\"roamservice\""};
uint8_t general_take_effect[2]={",1"};
uint8_t general_csq[5]={"+CSQ:"};
uint8_t general_qcsq[6]={"+QCSQ:"};
uint8_t general_qnwinfo[9]={"+QNWINFO:"};
uint8_t general_iccid[7] = {"+ICCID:"};
uint8_t general_qping[7] = {"+QPING:"};
uint8_t general_force_plmn[8] = {"AT+COPS="};
uint8_t automatic_plmn[1] = {"0"};

enum General_codes{
	_Not_Searching,
	_Registered,
	_Searching,
	_Registration_Denied,
	_Unknown,
	_Registered_Roaming,
	_SIM_READY,
	_SIM_PIN,
 	_SIM_PUK,
	_SIM_PIN2,
	_SIM_PUK2,
	_PH_SP_PIN,
	_PH_SP_PUK,
	_PH_NET_PIN,
	_PH_NET_PUK,
	_PH_CORP_PIN,
	_PH_CORP_PUK,
	_PH_NETSUB_PIN,
	_PH_NETSUB_PUK,
	_No_Operator_Selected
};


// control of the state USB
cm_q03_codes_t CM_Q03_Control_USB(bool state)
{
    uint8_t disable_usb[21] = "AT+QCFGEXT=\"disusb\",1";
    uint16_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t cmd_buff[22] = {0};
    uint8_t rxbuff[15] = {0};
    uint16_t cmd_buff_size = 0;

    if(state){
        disable_usb[20]='0';
    }

    Concatenate_Command(cmd_buff, disable_usb, sizeof(disable_usb), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Read(rxbuff, &rxbuff_size, 5000000);

    if(Timeout == status)
    {
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        return status;
    }
    return status;
}

cm_q03_codes_t CM_Q03_Synchronize_Local_Time()
{
    uint8_t sync_time[28] = "AT+QNTP=1,\"132.163.96.6\",123";
    uint16_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t cmd_buff[28] = {0};
    uint8_t rxbuff[15] = {0};
    uint16_t cmd_buff_size = 0;

    Concatenate_Command(cmd_buff, sync_time, sizeof(sync_time), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Read(rxbuff, &rxbuff_size, 5000000);

    if(Timeout == status)
    {
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        return status;
    }
    return status;
}


cm_q03_codes_t CM_Q03_Automatic_Time_Zone_Update(void)  //esta es la que resuelve el problema del +00
{
    uint8_t update_time[9] = "AT+CTZU=1";
    uint16_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t cmd_buff[10] = {0};
    uint8_t rxbuff[15] = {0};
    uint16_t cmd_buff_size = 0;

    Concatenate_Command(cmd_buff, update_time, sizeof(update_time), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    status = CM_Q03_Read(rxbuff, &rxbuff_size, 5000000);
    if(Timeout == status)
    {
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                         sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        return status;
    }

    return status;

}


cm_q03_codes_t CM_Q03_Time_Zone_Reporting(void)
{

    uint8_t update_time[9] = "AT+CTZR=1";
    uint16_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t cmd_buff[10] = {0};
    uint8_t rxbuff[15] = {0};
    uint16_t cmd_buff_size = 0;

    Concatenate_Command(cmd_buff, update_time, sizeof(update_time), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    status = CM_Q03_Read(rxbuff, &rxbuff_size, 5000000);
    if(Timeout == status)
    {
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                         sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        return status;
    }

    return status;
}

cm_q03_codes_t CM_Q03_General_Set_PLMN(general_network_paramenters_struct_t parameters_network,
                                       configuration_select_t default_parameters)
{
    uint16_t status = 0;
    uint16_t temp_index = 0;
    uint8_t temp_buff[6] = {0};
    uint16_t temp_buffer_size = 0;
    uint8_t cmd_buff[30] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint16_t rxbuffer_size ;

    if(default_parameters)
    {
        parameters_network.plmn = 0;
    }
    if( 0 < parameters_network.plmn && 999999 >= parameters_network.plmn)
    {
        memset(cmd_buff, '\0', 30);
        cmd_buff_size=0;
        Concatenate_Command(cmd_buff, general_force_plmn, sizeof(general_force_plmn), &cmd_buff_size);
        Concatenate_Command(cmd_buff, "1,2,\"", 5, &cmd_buff_size);
        Dec_to_Ascii(parameters_network.plmn, temp_buff, &temp_buffer_size);
        Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
        Concatenate_Command(cmd_buff, "\"",1, &cmd_buff_size);
        Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

        Delay(50000);
        CM_Q03_buffer_clean();

        status = CM_Q03_Write(cmd_buff, cmd_buff_size, 10000000);
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

                status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
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
            return status;
        }
    }
    else if( 0 == parameters_network.plmn)
        {
            memset(cmd_buff, '\0', sizeof(general_config_rat)-1);
            cmd_buff_size=0;
            Concatenate_Command(cmd_buff, general_force_plmn, sizeof(general_force_plmn), &cmd_buff_size);
            Concatenate_Command(cmd_buff, automatic_plmn, sizeof(automatic_plmn), &cmd_buff_size);
            Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

            Delay(50000);
            CM_Q03_buffer_clean();

            status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
            if(Timeout == status)
            {
                return status;
            }
            status = CM_Q03_Read(rxbuff, &rxbuffer_size, 6000000);
            if(Timeout == status)
            {

                status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
                if(Timeout == status)
                {

                    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
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
                return status;
            }
        }
    else
    {
        return User_Error;
    }

}

cm_q03_codes_t CM_Q03_General_Config_Network(general_network_paramenters_struct_t parameters_network,
		configuration_select_t default_parameters)
{
    uint16_t status = 0;
    uint16_t temp_index = 0;
    uint8_t temp_buff[6] = {0};
    uint16_t temp_buffer_size = 0;
    uint8_t cmd_buff[30] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint16_t rxbuffer_size ;

    //POSIBILIDAD DE RETIRAR ESTO
    if(default_parameters)
    {
        parameters_network.nwscanseq = 213;
        parameters_network.nwscanmode = 0;
        parameters_network.iotopmode = 0;
        parameters_network.roamservice = 255;
        parameters_network.frequency_bands = 4;
    }

    if( 123 == parameters_network.nwscanseq || 132 == parameters_network.nwscanseq
            || 213 == parameters_network.nwscanseq || 231 == parameters_network.nwscanseq
            || 312 == parameters_network.nwscanseq || 321 == parameters_network.nwscanseq)
    {
        if(123 == parameters_network.nwscanseq)
        {
            temp_buff[0]='0';
            temp_buff[1]='1';
            temp_buff[2]='0';
            temp_buff[3]='2';
            temp_buff[4]='0';
            temp_buff[5]='3';
        }
        else if(132 == parameters_network.nwscanseq)
        {
            temp_buff[0]='0';
            temp_buff[1]='1';
            temp_buff[2]='0';
            temp_buff[3]='3';
            temp_buff[4]='0';
            temp_buff[5]='2';
        }
        else if(213 == parameters_network.nwscanseq)
        {
            temp_buff[0]='0';
            temp_buff[1]='2';
            temp_buff[2]='0';
            temp_buff[3]='1';
            temp_buff[4]='0';
            temp_buff[5]='3';
        }
        else if(231 == parameters_network.nwscanseq)
        {
            temp_buff[0]='0';
            temp_buff[1]='2';
            temp_buff[2]='0';
            temp_buff[3]='3';
            temp_buff[4]='0';
            temp_buff[5]='1';
        }
        else if(312 == parameters_network.nwscanseq)
        {
            temp_buff[0]='0';
            temp_buff[1]='3';
            temp_buff[2]='0';
            temp_buff[3]='1';
            temp_buff[4]='0';
            temp_buff[5]='2';
        }
        else
        {
            temp_buff[0]='0';
            temp_buff[1]='3';
            temp_buff[2]='0';
            temp_buff[3]='2';
            temp_buff[4]='0';
            temp_buff[5]='1';
        }
        temp_buffer_size=6;

        Concatenate_Command(cmd_buff, general_config_rat, sizeof(general_config_rat), &cmd_buff_size);
//        Concatenate_Command(cmd_buff, general_nwscanseq, sizeof(general_nwscanseq), &cmd_buff_size);
//        Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
//        Concatenate_Command(cmd_buff, general_take_effect, sizeof(general_take_effect), &cmd_buff_size);
//        Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
//
//        Delay(50000);
//        CM_Q03_buffer_clean();
//
//        status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
//        if(Timeout == status)
//        {
//            return status;
//        }
//        status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
//        if(Timeout == status)
//        {
//            return status;
//        }
//        status = CM_Q03_Compare_response(rxbuff, rxbuffer_size, response_ok, sizeof(response_ok), sizeof(response_ok), &temp_index, 500000);
//        if(Timeout == status)
//        {
//            return status;
//        }
//        else if(0 != status)
//        {
//            status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
//            return status;
//        }
    } else
    {
        return User_Error;
    }

//    if( 0 <= parameters_network.nwscanmode && 3 >= parameters_network.nwscanmode)
//    {
//        memset(cmd_buff, '\0', sizeof(general_config_rat)-1);
//        cmd_buff_size=0;
//        Concatenate_Command(cmd_buff, general_config_rat, sizeof(general_config_rat), &cmd_buff_size);
//        Concatenate_Command(cmd_buff, general_nwscan, sizeof(general_nwscan), &cmd_buff_size);
//        Dec_to_Ascii(parameters_network.nwscanmode, temp_buff, &temp_buffer_size);
//        Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
//        Concatenate_Command(cmd_buff, general_take_effect, sizeof(general_take_effect), &cmd_buff_size);
//        Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
//
//        Delay(50000);
//        CM_Q03_buffer_clean();
//
//        status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
//        if(Timeout == status)
//        {
//            return status;
//        }
//        status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
//        if(Timeout == status)
//        {
//            return status;
//        }
//        status = CM_Q03_Compare_response(rxbuff, rxbuffer_size, response_ok, sizeof(response_ok), sizeof(response_ok), &temp_index, 500000);
//        if(Timeout == status)
//        {
//            return status;
//        }
//        else if(0 != status)
//        {
//            status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
//            return status;
//        }
//    }
//    else
//    {
//        return User_Error;
//    }
//
//    if( 0 <= parameters_network.iotopmode && 2 >= parameters_network.iotopmode)
//    {
//        memset(cmd_buff, '\0', sizeof(general_config_rat)-1);
//        cmd_buff_size=0;
//        Concatenate_Command(cmd_buff, general_config_rat, sizeof(general_config_rat), &cmd_buff_size);
//        Concatenate_Command(cmd_buff, general_iotop_mode, sizeof(general_iotop_mode), &cmd_buff_size);
//        Dec_to_Ascii(parameters_network.iotopmode, temp_buff, &temp_buffer_size);
//        Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
//        Concatenate_Command(cmd_buff, general_take_effect, sizeof(general_take_effect), &cmd_buff_size);
//        Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
//
//        Delay(50000);
//        CM_Q03_buffer_clean();
//
//        status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
//        if(Timeout == status)
//        {
//            return status;
//        }
//        status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
//        if(Timeout == status)
//        {
//            return status;
//        }
//        status = CM_Q03_Compare_response(rxbuff, rxbuffer_size, response_ok, sizeof(response_ok), sizeof(response_ok), &temp_index, 500000);
//        if(Timeout == status)
//        {
//            return status;
//        }
//        else if(0 != status)
//        {
//            status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
//            return status;
//        }
//    }
//    else
//    {
//        return User_Error;
//    }

    // Peko modification To force a PLMN, choose "0" for automatic mode

    if( 0 < parameters_network.plmn && 999999 >= parameters_network.plmn)
    {
        memset(cmd_buff, '\0', 30);
        cmd_buff_size=0;
        Concatenate_Command(cmd_buff, general_force_plmn, sizeof(general_force_plmn), &cmd_buff_size);
        Concatenate_Command(cmd_buff, "1,2,\"", 5, &cmd_buff_size);
        Dec_to_Ascii(parameters_network.plmn, temp_buff, &temp_buffer_size);
        Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
        Concatenate_Command(cmd_buff, "\"",1, &cmd_buff_size);
        Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

        Delay(50000);
        CM_Q03_buffer_clean();

        status = CM_Q03_Write(cmd_buff, cmd_buff_size, 10000000);
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

                status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
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
            return status;
        }
    }
    else if( 0 == parameters_network.plmn)
        {
            memset(cmd_buff, '\0', sizeof(general_config_rat)-1);
            cmd_buff_size=0;
            Concatenate_Command(cmd_buff, general_force_plmn, sizeof(general_force_plmn), &cmd_buff_size);
            Concatenate_Command(cmd_buff, automatic_plmn, sizeof(automatic_plmn), &cmd_buff_size);
            Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

            Delay(50000);
            CM_Q03_buffer_clean();

            status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
            if(Timeout == status)
            {
                return status;
            }
            status = CM_Q03_Read(rxbuff, &rxbuffer_size, 6000000);
            if(Timeout == status)
            {

                status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
                if(Timeout == status)
                {

                    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 60000000);
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
                return status;
            }
        }
    else
    {
        return User_Error;
    }
//“*” means under development BG95
//    if( 1 == parameters_network.roamservice || 2 == parameters_network.roamservice ||  255 == parameters_network.roamservice)
//    {
//        memset(cmd_buff, '\0', sizeof(general_config_rat)-1);
//        cmd_buff_size=0;
//        Concatenate_Command(cmd_buff, general_config_rat, sizeof(general_config_rat), &cmd_buff_size);
//        Concatenate_Command(cmd_buff, general_roamservice, sizeof(general_roamservice), &cmd_buff_size);
//        Dec_to_Ascii(parameters_network.roamservice, temp_buff, &temp_buffer_size);
//        Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
//        Concatenate_Command(cmd_buff, general_take_effect, sizeof(general_take_effect), &cmd_buff_size);
//
//        Delay(50000);
//        CM_Q03_buffer_clean();
//
//        Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
//        status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
//        if(Timeout == status)
//        {
//            return status;
//        }
//        status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
//        if(Timeout == status)
//        {
//            return status;
//        }
//        status = CM_Q03_Compare_response(rxbuff, rxbuffer_size, response_ok, sizeof(response_ok), sizeof(response_ok), &temp_index, 500000);
//        if(Timeout == status)
//        {
//            return status;
//        }
//        else if(0 != status)
//        {
//            status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
//            return status;
//        }
//    }
//    else
//    {
//        return User_Error;
//    }
    return 0;
}

cm_q03_codes_t CM_Q03_General_Registration_Status()
{
	uint8_t general_network_registration_status[8]={"AT+CREG?"};
	uint8_t general_network_status_correct[7]={"+CREG: "};
    uint16_t status = 0;
    uint8_t cmd_buff[9] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[30] = {0};
    uint16_t rxbuffer_size ;
    uint8_t return_code = 0;

    Concatenate_Command(cmd_buff, general_network_registration_status, sizeof(general_network_registration_status), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
    if(Timeout == status){
        return status;
    }

    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
    if(Timeout == status){
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, general_network_status_correct, sizeof(general_network_status_correct), 500000);
    if(Timeout == status){
        return status;
    }else if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        return status;
    }
    return_code = rxbuff[sizeof(general_network_status_correct)+2] - ASCII_0;

    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
    if(Timeout == status){
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, response_ok, sizeof(response_ok), 500000);
    if(Timeout == status){
        return status;
    }
    else if(0 != status){
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        return status;
    }

    return return_code + GENERAL_CODES;
}

cm_q03_codes_t CM_Q03_General_EPS_Registration_Status()
{
	uint8_t network_status[9]="AT+CEREG?";
	uint8_t network_status_response[8]="+CEREG: ";
    uint16_t status = 0;
    uint8_t cmd_buff[10] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[38] = {0};
    uint16_t rxbuffer_size ;
    uint8_t return_code = 0;

    Concatenate_Command(cmd_buff, network_status, sizeof(network_status), &cmd_buff_size);
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

    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, network_status_response, sizeof(network_status_response), 500000);
    if(Timeout == status)
    {
        return status;
    }else if(0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        return status;
    }
    return_code = rxbuff[sizeof(network_status_response)+2] - ASCII_0;

    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
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
        return status;
    }

    return return_code + GENERAL_CODES;
}

cm_q03_codes_t CM_Q03_General_GPRS_Registration_Status()
{
	uint8_t network_status[9]="AT+CGREG?";
	uint8_t network_status_response[8]="+CGREG: ";
    uint16_t status = 0;
    uint8_t cmd_buff[10] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[32] = {0};
    uint16_t rxbuffer_size ;
    uint8_t return_code = 0;

    Concatenate_Command(cmd_buff, network_status, sizeof(network_status), &cmd_buff_size);
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

    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, network_status_response, sizeof(network_status_response), 500000);
    if(Timeout == status)
    {
        return status;
    }else if(0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        return status;
    }
    return_code = rxbuff[sizeof(network_status_response)+2] - ASCII_0;

    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
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
        return status;
    }
    return return_code + GENERAL_CODES;
}

cm_q03_codes_t CM_Q03_Get_IMEI(uint8_t *imei)
{
    uint8_t command_imei[6] = "AT+GSN";
    uint16_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t cmd_buff[7] = {0};
    uint8_t rxbuff[15] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t imei_size = 15;

    Concatenate_Command(cmd_buff, command_imei, sizeof(command_imei), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Read(rxbuff, &rxbuff_size, 5000000);
    if(Timeout == status)
    {
        return status;
    }
    while(imei_size--)
    {
        imei[imei_size] = rxbuff[imei_size];
    }
    status = CM_Q03_Read(rxbuff, &rxbuff_size, 5000000);
    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        return status;
    }
    return status;
}

cm_q03_codes_t CM_Q03_General_PIN_status()
{
    uint8_t general_sim_status[8] = "AT+CPIN?";
    uint8_t general_sim_response[7] = "+CPIN: ";
    uint8_t general_sim_ready[5] =    	 "READY";
    uint8_t general_sim_pin[2][7] = { 	 "SIM PIN",
    								  	 "SIM PUK"};
    uint8_t general_sim_pin2[2][8] = {	 "SIM PIN2",
			 	 	 	 	 	 	  	 "SIM PUK2"};
    uint8_t general_sim_sp[2][9] = {	 "PH-SP PIN",
    									 "PH-SP PUK"};
    uint8_t general_sim_net[2][10] = {   "PH-NET PIN",
    									 "PH-NET PUK"};
    uint8_t general_sim_corp[2][11] = {  "PH-CORP PIN",
    									 "PH-CORP PUK"};
    uint8_t general_sim_netsub[2][13] = {"PH-NETSUB PIN",
    									 "PH-NETSUB PUK"};
	uint16_t status = 0;
    uint16_t temp_index = 0;
    uint8_t cmd_buff[9] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[22] = {0};
    uint16_t rxbuffer_size;
    cm_q03_codes_t code_return;

    Concatenate_Command(cmd_buff, general_sim_status, sizeof(general_sim_status), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 5000000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_response(rxbuff, rxbuffer_size, general_sim_response, sizeof(general_sim_response), sizeof(general_sim_response), &temp_index, 500000);

    if(!status)
    {
    	switch(rxbuffer_size - temp_index)
    	{
    	case sizeof(general_sim_ready):
			code_return = _SIM_READY;
			break;
    	case sizeof(general_sim_pin[0]):
    		if(!(CM_Q03_Compare_short_response(&rxbuff[temp_index], rxbuffer_size-temp_index, general_sim_pin[0], sizeof(general_sim_pin[0]), 500000)))
    		{
    			code_return = _SIM_PIN;
    		}else
    		{
    			code_return = _SIM_PUK;
    		}
			break;
    	case sizeof(general_sim_pin2[0]):
			if(!(CM_Q03_Compare_short_response(&rxbuff[temp_index], rxbuffer_size-temp_index, general_sim_pin2[0], sizeof(general_sim_pin2[0]), 500000)))
			{
				code_return = _SIM_PIN2;
			}else
			{
				code_return = _SIM_PUK2;
			}
			break;
    	case sizeof(general_sim_net[0]):
			if(!(CM_Q03_Compare_short_response(&rxbuff[temp_index], rxbuffer_size-temp_index, general_sim_net[0], sizeof(general_sim_net[0]), 500000)))
			{
				code_return = _PH_NET_PIN;
			}else
			{
				code_return = _PH_NET_PUK;
			}
			break;
    	case sizeof(general_sim_netsub[0]):
			if(!(CM_Q03_Compare_short_response(&rxbuff[temp_index], rxbuffer_size-temp_index, general_sim_netsub[0], sizeof(general_sim_netsub[0]), 500000)))
			{
				code_return = _PH_NETSUB_PIN;
			}else
			{
				code_return = _PH_NETSUB_PUK;
			}
			break;
    	case sizeof(general_sim_sp[0]):
			if(!(CM_Q03_Compare_short_response(&rxbuff[temp_index], rxbuffer_size-temp_index, general_sim_sp[0], sizeof(general_sim_sp[0]), 500000)))
			{
				code_return = _PH_SP_PIN;
			}else
			{
				code_return = _PH_SP_PUK;
			}
			break;
    	case sizeof(general_sim_corp[0]):
			if(!(CM_Q03_Compare_short_response(&rxbuff[temp_index], rxbuffer_size-temp_index, general_sim_corp[0], sizeof(general_sim_corp[0]), 500000)))
			{
				code_return = _PH_CORP_PIN;
			}else
			{
				code_return = _PH_CORP_PUK;
			}
			break;
    	default:
    		code_return = ERROR;
    		break;
    	}
    }else
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        return status;
    }

    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 5000000);
    status = CM_Q03_Compare_response(rxbuff, rxbuffer_size, response_ok, sizeof(response_ok), sizeof(response_ok), &temp_index, 500000);


    return code_return + GENERAL_CODES;
}

cm_q03_codes_t CM_Q03_General_enter_PIN(uint8_t* pin)
{
	uint8_t pin_status[8] = "AT+CPIN=";
	uint8_t cmd_buff[13] = {0};
	uint16_t cmd_buff_size = 0;
	uint8_t rx_buff[15] = {0};
	uint16_t rx_buff_size = 0;
    cm_q03_codes_t status = Success;
    uint16_t temp_index = 0;
    uint8_t timeout_count = 0;

    Concatenate_Command(cmd_buff, pin_status, sizeof(pin_status), &cmd_buff_size);
    Concatenate_Command(cmd_buff, pin, PIN_LENGTH, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
    if(status == Timeout)
    {
        return status;
    }
    do
    {
        status = CM_Q03_Read(rx_buff, &rx_buff_size, 1000000);
    }while(5 > (timeout_count++) && Timeout == status);
    if(status == Timeout)
    {
    	return status;
    }
    status = CM_Q03_Compare_response(rx_buff, rx_buff_size, response_ok, sizeof(response_ok), sizeof(response_ok), &temp_index, 500000);
    if(Timeout == status)
    {
    	return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rx_buff, rx_buff_size);
        return status;
    }
    return Success;
}

cm_q03_codes_t CM_Q03_General_change_PIN(uint8_t* puk, uint8_t* new_pin)
{
	uint8_t pin_status[8] = "AT+CPIN=";
	uint8_t cmd_buff[22] = {0};
	uint16_t cmd_buff_size = 0;
	uint8_t rx_buff[15] = {0};
	uint16_t rx_buff_size = 0;
    cm_q03_codes_t status = Success;
    uint16_t temp_index = 0;
    uint8_t timeout_count = 0;

    Concatenate_Command(cmd_buff, pin_status, sizeof(pin_status), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, puk, PUK_LENGTH, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, new_pin, PIN_LENGTH, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
    if(status == Timeout)
    {
        return status;
    }
    do
    {
        status = CM_Q03_Read(rx_buff, &rx_buff_size, 1000000);
    }while(5 > (timeout_count++) && Timeout == status);
    if(status == Timeout)
    {
    	return status;
    }
    status = CM_Q03_Compare_response(rx_buff, rx_buff_size, response_ok, sizeof(response_ok), sizeof(response_ok), &temp_index, 500000);
    if(Timeout == status)
    {
    	return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rx_buff, rx_buff_size);
        return status + SIM_CODES;
    }
    return status;
}

uint16_t CM_Q03_Emm_Cause()
{
	uint8_t emmcause_status[18] = "AT+QCFG=\"emmcause\"";
	uint8_t emmcause_response[18]="+QCFG: \"emmcause\",";
    uint16_t status = 0;
    uint8_t cmd_buff[19] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint16_t rxbuffer_size ;
    cm_q03_codes_t return_code = Success;

    Concatenate_Command(cmd_buff, emmcause_status, sizeof(emmcause_status), &cmd_buff_size);
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
    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, emmcause_response, sizeof(emmcause_response), 500000);
    if(Timeout == status)
    {
        return status;
    }else if(0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        return status;
    }
    return_code = Ascii_to_Code(&rxbuff[sizeof(emmcause_response)], rxbuffer_size-sizeof(emmcause_response));
    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
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
        return status;
    }

    return return_code;
}

cm_q03_codes_t CM_Q03_General_Set_Functionality(module_functionality_t functionality)
{
	uint8_t command_cfun[8] = "AT+CFUN=";
	uint8_t cmd_buff[10] = {0};
	uint16_t cmd_buff_size = 0;
	uint8_t rx_buff[15] = {0};
	uint16_t rx_buff_size = 0;
    cm_q03_codes_t status = Success;
    uint8_t temp_cmd = 0;
    uint16_t temp_cmd_size = 0;
    uint8_t timeout_count = 0;

    Concatenate_Command(cmd_buff, command_cfun, sizeof(command_cfun), &cmd_buff_size);
    Dec_to_Ascii(functionality, &temp_cmd, &temp_cmd_size);
    Concatenate_Command(cmd_buff, &temp_cmd, temp_cmd_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
    if(status == Timeout)
    {
        return status;
    }
    do
    {
        status = CM_Q03_Read(rx_buff, &rx_buff_size, 1000000);
    }while(15 > (timeout_count++) && Timeout == status);
    if(status == Timeout)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rx_buff, rx_buff_size, response_ok, sizeof(response_ok), 500000);
    if(Timeout == status)
    {
    	return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rx_buff, rx_buff_size);
        return status;
    }
    return Success;
}

cm_q03_codes_t CM_Q03_General_Access_Technology(access_technology_t *act)
{
	uint8_t general_operator_selector[8]={"AT+COPS?"};
	uint8_t general_operator_selector_response[7]={"+COPS: "};
    uint16_t status = 0;
    uint8_t cmd_buff[9] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[35] = {0};
    uint16_t rxbuffer_size ;
    uint16_t return_code = 0;

    Concatenate_Command(cmd_buff, general_operator_selector, sizeof(general_operator_selector), &cmd_buff_size);
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

    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, general_operator_selector_response,
    									   sizeof(general_operator_selector_response), 500000);
    if(Timeout == status)
    {
        return status;
    }else if(0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        return status;
    }

    if(9 == rxbuffer_size)
    {
    	return_code = _No_Operator_Selected + GENERAL_CODES;
    }else
    {
    	*act = rxbuff[rxbuffer_size-1] - ASCII_0;
    }

    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
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
        return status;
    }

	return status + return_code;

}


cm_q03_codes_t CM_Q03_General_Get_Clock(clock_struct_t *clk)
{
    uint8_t command_clock[8] = "AT+CCLK?";
    uint8_t response_clock[8] = "+CCLK: \"";
    cm_q03_codes_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[9] = {0};
    uint8_t temp_buff[30] = {0};
    uint16_t cmd_buff_size = 0;
    char temp_clk[20] = {0};
    char token[3] = {0};
    uint8_t token_index = 0;
    uint8_t data_struct[6] = {0};
    uint8_t i = 0;
//+CCLK: yy/MM/dd,hh:mm:ss±zz

    Concatenate_Command(cmd_buff, command_clock, sizeof(command_clock), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Read(temp_buff, &buffer_size, 5000000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_short_response(temp_buff, buffer_size, response_clock,
                                     sizeof(response_clock), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(temp_buff, buffer_size);
        return status;
    }
    strncpy((char*)temp_clk, (char*)&temp_buff[sizeof(response_clock)], 20);
    strcpy((char*)clk->string, temp_clk);

    while(i < sizeof(data_struct))
    {
		strncpy(token, &temp_clk[token_index], 2);
		token_index += 3;
		data_struct[i++] = (uint8_t)atoi(token);
    }
    i = 0;
    clk->year = data_struct[i++];
    clk->month = data_struct[i++];
    clk->day = data_struct[i++];
    clk->hour = data_struct[i++];
    clk->minutes = data_struct[i++];
    clk->seconds = data_struct[i++];

    if('+' == temp_clk[token_index-1])
    {
    	strncpy(token, &temp_clk[token_index], 2);
    }
    else
    {
    	token_index--;
    	strncpy(token, &temp_clk[token_index], 3);
    }
	clk->time_zone = (int8_t)atoi(token);

    status = CM_Q03_Read(temp_buff, &buffer_size, 5000000);
    if(Timeout == status)
    {
        return status;
    }
	status = CM_Q03_Compare_short_response(temp_buff, buffer_size, response_ok,
									 sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(temp_buff, buffer_size);
        return status;
    }
    return status;
}


cm_q03_codes_t CM_Q03_Signal_Quality_Report(uint8_t *rssi, uint8_t *ber)
{
    uint8_t command[7] = "AT+CSQ\r";
    uint16_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint8_t rx_number[4] = {0};
    uint16_t rx_number_size = 0;

    command[6]=character_command_end;
    status = CM_Q03_Write(command, sizeof(command), 5000000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Read(rxbuff, &rxbuff_size, 5000000);
    if(Timeout == status)
    {
        return status;
    }


    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, general_csq,
                                     sizeof(general_csq), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        return status;
    }

    rxbuff[rxbuff_size]=',';
    rxbuff_size++;

    memset(rx_number, '\0', sizeof(rx_number));

    CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, rxbuff_size, rx_number,
            &rx_number_size, ' ', ',');

    *rssi=atoi(rx_number);

    memset(rx_number, '\0', sizeof(rx_number));

    CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, rxbuff_size, rx_number,
            &rx_number_size, ',', ',');

    *ber=atoi(rx_number);

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 5000000);
    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        return status;
    }
    return status;
}



urc_received_status_t CM_Q03_General_Get_ME_Functionality_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(CFUN_POS)]
											 >> (MODULE_8_SFT(CFUN_POS))) & 0x01);
}

void CM_Q03_General_Clear_ME_Functionality_Flag(void)

{

	urc_flags[DIV_BY_8(CFUN_POS)] &= ~(1 << MODULE_8_SFT(CFUN_POS));
}

void CM_Q03_General_Get_ME_Functionality(module_functionality_t *func)
{
	(*func) = (module_functionality_t)cfun;

	urc_flags[DIV_BY_8(CFUN_POS)] &= ~(1 << MODULE_8_SFT(CFUN_POS));
}

urc_received_status_t CM_Q03_General_Get_Pin_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(CPIN_POS)]
											 >> (MODULE_8_SFT(CPIN_POS))) & 0x01);
}

void CM_Q03_General_Clear_Pin_Status_Flag(void)
{
	urc_flags[DIV_BY_8(CPIN_POS)] &= ~(1 << MODULE_8_SFT(CPIN_POS));
}

void CM_Q03_General_Get_Pin_Status(cm_q03_codes_t *pin_status)
{
	switch(cpin)
	{
		case URC_READY:
			(*pin_status) = SIM_READY;
		break;

		case URC_SIM_PIN:
			(*pin_status) = SIM_PIN;
		break;

		case URC_SIM_PUK:
			(*pin_status) = SIM_PUK;
		break;

		case URC_SIM_PIN2:
			(*pin_status) = SIM_PIN2;
		break;

		case URC_SIM_PUK2:
			(*pin_status) = SIM_PUK2;
		break;

		case URC_PH_NET_PIN:
			(*pin_status) = PH_SP_PIN;
		break;

		case URC_PH_NET_PUK:
			(*pin_status) = PH_SP_PUK;
		break;

		case URC_PH_NETSUB_PIN:
			(*pin_status) = PH_SP_PUK;
		break;

		case URC_PH_NETSUB_PUK:
			(*pin_status) = PH_NET_PUK;
		break;

		case URC_PH_SP_PIN:
			(*pin_status) = PH_NET_PUK;
		break;

		case URC_PH_SP_PUK:
			(*pin_status) = PH_CORP_PUK;
		break;

		case URC_PH_CORP_PIN:
			(*pin_status) = PH_NETSUB_PIN;
		break;

		case URC_PH_CORP_PUK:
			(*pin_status) = PH_NETSUB_PUK;
		break;

		default:
			(*pin_status) = ERROR;
		break;
	}

	urc_flags[DIV_BY_8(CPIN_POS)] &= ~(1 << MODULE_8_SFT(CPIN_POS));
}



urc_received_status_t CM_Q03_General_Get_Registration_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(CREG_POS)]
											 >> (MODULE_8_SFT(CREG_POS))) & 0x01);
}

void CM_Q03_General_Clear_Registration_Status_Flag(void)
{
	urc_flags[DIV_BY_8(CREG_POS)] &= ~(1 << MODULE_8_SFT(CREG_POS));
}

void CM_Q03_General_Get_Registration_Status(cm_q03_codes_t *reg_stat)
{
	(*reg_stat) = (cm_q03_codes_t)(creg + GENERAL_CODES);

	urc_flags[DIV_BY_8(CREG_POS)] &= ~(1 << MODULE_8_SFT(CREG_POS));
}

urc_received_status_t CM_Q03_General_Get_Powered_Down_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(POWERED_DOWN_POS)]
											 >> (MODULE_8_SFT(POWERED_DOWN_POS))) & 0x01);
}

void CM_Q03_General_Clear_Powered_Down_Status_Flag(void)
{
	urc_flags[DIV_BY_8(POWERED_DOWN_POS)] &= ~(1 << MODULE_8_SFT(POWERED_DOWN_POS));
}

void CM_Q03_General_Powered_Down_Status(power_down_status_t *pow_dwn)
{
	(*pow_dwn) = (power_down_status_t)(no_param_urc.flags_struct.urc_powered_down);

	urc_flags[DIV_BY_8(POWERED_DOWN_POS)] &= ~(1 << MODULE_8_SFT(POWERED_DOWN_POS));
}

urc_received_status_t CM_Q03_General_Get_Ready_Status_Flag(void)
{
	return(urc_received_status_t)((urc_flags[DIV_BY_8(RDY_POS)]
											 >> (MODULE_8_SFT(RDY_POS))) & 0x01);
}

void CM_Q03_General_Clear_Ready_Status_Flag(void)
{
	urc_flags[DIV_BY_8(RDY_POS)] &= ~(1 << MODULE_8_SFT(RDY_POS));
}

void CM_Q03_General_Ready_Status(ready_status_t *rdy)
{
	(*rdy) = (ready_status_t)(no_param_urc.flags_struct.urc_rdy);

	urc_flags[DIV_BY_8(RDY_POS)] &= ~(1 << MODULE_8_SFT(RDY_POS));
}

//TODO: Validar con Homi esta funcion
cm_q03_codes_t CM_Q03_4G_Signal_Quality_Report(uint8_t *sysmode, uint16_t *sysmode_size, uint8_t *rssi,
        uint16_t *rssi_size, uint8_t *rsrp, uint16_t *rsrp_size, uint8_t *sinr, uint16_t *sinr_size,
        uint8_t *rsrq, uint16_t *rsrq_size)
{
    uint8_t command[8] = "AT+QCSQ\r";
    cm_q03_codes_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t rxbuff[40] = {0};
    uint8_t rx_number[8] = {0};
    uint8_t rx_rat[8] = {0};
    uint16_t rx_rat_size = 0;
    uint8_t rat_gsm[3] = "GSM";
    uint16_t rx_number_size = 0;
    volatile int16_t value = 0;
    uint8_t response_signal_type = 0; //mio

    command[7]=character_command_end;

//AT+COPS
    //PLMN 334020 Telcel
    //altan 334140
    //ATyT 334050 334070 334090
    //linksfield no conecta con altan provar linksfield con altan
    //entrar a 2G con Telcel y 4G y ver los parametros

    //EN 2G -110 MINIMO a -51 es buena señal en 2G

    //en LTE 40 a 85 con AT6T lo minimo por solo unas veces y luego desaparecen es de 25


    CM_Q03_buffer_clean();
    Delay(500);

    status = CM_Q03_Write(command, sizeof(command), 5000000);
    if(Timeout == status)
    {
        return status;
    }

    Delay(500000);

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 5000000);
    if(Timeout == status)
    {
        return status;
    }


    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, general_qcsq,
                                     sizeof(general_qcsq), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        return status;
    }

    rxbuff[rxbuff_size]=',';
    rxbuff_size++;


///////////////////////////// GET SYSMODE ///////////////////////////////////////////
    CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, rxbuff_size, sysmode,
                                                       &rx_rat_size, '"', '"');

    strncpy(rx_rat,sysmode, rx_rat_size);
    *sysmode_size = rx_rat_size;


///////////////////////////// GET RSSI ///////////////////////////////////////////
    CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff, rxbuff_size, rssi,
                                                             &rx_number_size, ',', 1,',');

    strncpy(rx_number,rssi,rx_number_size);
    if( rx_number_size>4 || (rx_number_size == 4 && rx_number[1]>49) )
    {
        return 550; //unknown_error
    }


    response_signal_type = CM_Q03_Read_response_signal_type(rxbuff, rxbuff_size); //funcion mia sebas
    value = atoi (rx_number);

    if(response_signal_type ==1){ //si responde con LTE
        if ((value < 20) || (value > 95)){
            return ERROR;
        }
    }else if(response_signal_type ==2){//si responde con GSM
       if ((value < -145) || (value > -20)){ //por ahora esos valores son los registrados
           return ERROR;
       }
    }else{
        return ERROR;
    }

//    value = atoi (rx_number);
////    if ((value < -113) || (value > -48))esto es lo que debemos modificar
////        return ERROR;

    *rssi_size = rx_number_size;
    memset(rx_number, '\0', sizeof(rx_number));
    rx_number_size=0;

    status = CM_Q03_Compare_short_response(rx_rat, rxbuff_size, rat_gsm,
                                         sizeof(rat_gsm), 5000000);

    if (status == 0)
    {
        *rsrp = '0';
        *rsrp_size = 1;
        *rsrq = '0';
        *rsrq_size = 1;
        *sinr = '0';
        *sinr_size = 1;

        return status;
    }

///////////////////////////// GET RSRP ///////////////////////////////////////////
    CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff, rxbuff_size, rsrp,
                                                             &rx_number_size, ',',2,',');

    strncpy(rx_number,rsrp,rx_number_size);
    if( rx_number_size>4 || (rx_number_size == 4 && rx_number[1]>49) )
    {
        return 550; //unknown_error
    }

    value = atoi (rx_number);
    if ((value < -150) || (value > -20))
        return ERROR;

    *rsrp_size = rx_number_size;
    memset(rx_number, '\0', sizeof(rx_number));
    rx_number_size=0;

///////////////////////////// GET SINR ///////////////////////////////////////////
    CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff, rxbuff_size, sinr,
                                                             &rx_number_size, ',',3,',');

    strncpy(rx_number,sinr,rx_number_size);
    if( rx_number_size>3 || (rx_number_size == 3 && rx_number[0]>50) )
    {
        return 550; //unknown_error
    }

    value = atoi (rx_number);
    if ((value < 1) || (value > 400))
        return ERROR;

    *sinr_size = rx_number_size;
    memset(rx_number, '\0', sizeof(rx_number));
    rx_number_size=0;

///////////////////////////// GET RSRQ ///////////////////////////////////////////
    CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff, rxbuff_size, rsrq,
                                                             &rx_number_size, ',', 4,',');

    strncpy(rx_number,rsrq,rx_number_size);
    if( rx_number_size>3 || (rx_number_size == 3 && rx_number[1]>50) )
    {
        return 550; //unknown_error
    }

    value = atoi (rx_number);
    if ((value < -23) || (value > -1))
        return ERROR;

    *rsrq_size = rx_number_size;


    status = CM_Q03_Read(rxbuff, &rxbuff_size, 5000000);
    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        return status;
    }
    return status;
}

cm_q03_codes_t CM_Q03_Get_SIM_PLMN(uint32_t *plmn)
{
    uint8_t command[8] = "AT+CIMI\r";
    cm_q03_codes_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint8_t buff[6] = {0};
    uint8_t i=0;

    command[7]=character_command_end;

    CM_Q03_buffer_clean();
    Delay(500);

    status = CM_Q03_Write(command, sizeof(command), 5000000);
    if(Timeout == status)
    {
        return status;
    }
    Delay(500000);

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 5000000);
    if(Timeout == status)
    {
        return status;
    }

    for (i=0;i<6;i++)
    {
        buff[i] = rxbuff[i];
    }

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 5000000);
    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        return status;
    }
    *plmn = atoi (buff);
    return status;
}

cm_q03_codes_t CM_Q03_GET_NW_INFO(uint8_t *rat, uint16_t *rat_size, uint8_t *plmn, uint8_t *band,
                              uint16_t *band_size, uint8_t *channel, uint16_t *channel_size)
{
    uint8_t command[11] = "AT+QNWINFO\r";
    cm_q03_codes_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t rxbuff[53] = {0};
    uint16_t plmn_size=0;

    command[10]=character_command_end;

    CM_Q03_buffer_clean();
    Delay(500);

    status = CM_Q03_Write(command, sizeof(command), 5000000);
        if(Timeout == status)
        {
            return status;
        }
        Delay(500000);

        status = CM_Q03_Read(rxbuff, &rxbuff_size, 5000000);
        if(Timeout == status)
        {
            return status;
        }

        status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, general_qnwinfo,
                                             sizeof(general_qnwinfo), 5000000);
        if (0 != status)
        {
            status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
            return status;
        }

        rxbuff[rxbuff_size]=',';
        rxbuff_size++;

        CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff, rxbuff_size, rat,
                                                                 rat_size, '"', 1,'"');

        CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff, rxbuff_size, plmn,
                                                                 &plmn_size, '"', 3,'"');

        CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff, rxbuff_size, band,
                                                                 band_size, '"', 5,'"');

        CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff, rxbuff_size, channel,
                                                                 channel_size, ',', 3,',');

        status = CM_Q03_Read(rxbuff, &rxbuff_size, 5000000);
            status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                             sizeof(response_ok), 5000000);
            if (0 != status)
            {
                status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
                return status;
            }
            return status;
}

cm_q03_codes_t CM_Q03_Get_SIM_ICCID(uint8_t *iccid, uint16_t *iccid_size)
{
    uint8_t command[9] = "AT+ICCID\r";
    cm_q03_codes_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t rxbuff[30] = {0};

    command[8]=character_command_end;

    CM_Q03_buffer_clean();
    Delay(500);

    status = CM_Q03_Write(command, sizeof(command), 5000000);
    if(Timeout == status)
    {
        return status;
    }
    Delay(500000);

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 5000000);
    if(Timeout == status)
    {
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, general_iccid,
                                                 sizeof(general_iccid), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        return status;
    }

    rxbuff[rxbuff_size]=',';
    rxbuff_size++;

    CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff, rxbuff_size, iccid,
                                                                     iccid_size, ' ', 1,',');

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 5000000);
    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        return status;
    }

    return status;
}

cm_q03_codes_t CM_Q03_Ping(uint8_t *server, uint16_t *ttl)
{
    uint8_t command[80] = {0}; //"AT+QPING=1,\"www.google.com\",5,1\r";
    cm_q03_codes_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t rxbuff[53] = {0};
    uint8_t buff [6] = {0};
    uint16_t buff_size = 0;

    strcpy((char *)(command), (char *)("AT+QPING=1,\""));
    strcat((char *)(command), (char *)(server));
    strcat((char *)(command), (char *)("\",5,1\r"));
//    command[31]=character_command_end;

    CM_Q03_AP_Ready(1,500);
    Delay(300000);

    CM_Q03_buffer_clean();
    Delay(500);

    status = CM_Q03_Write(command, sizeof(command), 5000000);
    if(Timeout == status)
    {
        CM_Q03_AP_Ready(0,500);
        return status;
    }
    Delay(5000000);

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 5000000);
    if(Timeout == status)
    {
        CM_Q03_AP_Ready(0,500);
        return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                         sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_AP_Ready(0,500);
        return status;
    }

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 5000000);
        if(Timeout == status)
        {
            CM_Q03_AP_Ready(0,500);
            return status;
        }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, general_qping,
                                                 sizeof(general_qping), 5000000);
            if (0 != status)
            {
                status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
                return status;
            }

    CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff, rxbuff_size, buff,
                                                             &buff_size, ',', 3,',');

    *ttl = atoi (buff);

    CM_Q03_AP_Ready(0,500);

    return status;

}

cm_q03_codes_t CM_Q03_General_Config_CEREG(void)
{
    uint8_t network_status[10]="AT+CEREG=2";
    uint8_t network_status_response[8]="+CEREG: ";
    uint16_t status = 0;
    uint8_t cmd_buff[11] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[38] = {0};
    uint16_t rxbuffer_size ;
    uint8_t return_code = 0;

    Concatenate_Command(cmd_buff, network_status, sizeof(network_status), &cmd_buff_size);
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
    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, response_ok, sizeof(response_ok), 500000);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        return status;
    }
    return Success;
}

cm_q03_codes_t CM_Q03_General_Get_LAC_CID(uint8_t *lac, uint16_t *lac_size, uint8_t *cid, uint16_t *cid_size)
{
    uint8_t cereg_config[10]="AT+CEREG=2";
    uint8_t cereg_deconfig[11]="\rAT+CEREG=0";
    uint8_t network_status[9]="AT+CEREG?";
    uint8_t network_status_response[8]="+CEREG: ";
    uint16_t status = 0;
    uint8_t cmd_buff[11] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[38] = {0};
    uint16_t rxbuffer_size ;
//    uint8_t return_code = 0;

    Concatenate_Command(cmd_buff, cereg_config, sizeof(cereg_config), &cmd_buff_size);
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
    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, response_ok, sizeof(response_ok), 500000);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        return status;
    } // If no error present then continue execution

// Send LAC CID Query with AT+CEREG?

    memset(cmd_buff, '\0', 11);

    Concatenate_Command(cmd_buff, network_status, sizeof(network_status), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);//aqui empieza
    if(Timeout == status)
    {
        return status;
    }

    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);
    if(Timeout == status)
    {
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, network_status_response, sizeof(network_status_response), 500000);
    if(Timeout == status)
    {
        return status;
    }else if(0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        return status;
    }

   //TODO: Agregar procesamiento del dato para extraerlo a funcion principal
    CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff, rxbuffer_size, lac,
                                                             lac_size, '"', 1,'"');

    CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff, rxbuffer_size, cid,
                                                                 cid_size, '"', 3,'"');

    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);//aqui termina
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
        return status;
    }

    // Deconfigure the CEREG function to not receive URC anymore
//****************cambios realizados para EG915U-LA********************//
    memset(cmd_buff, '\0', 11);
    memset(rxbuff, '\0', sizeof(rxbuff));
    cmd_buff_size=0;
    Concatenate_Command(cmd_buff, cereg_deconfig, sizeof(cereg_deconfig), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(1000000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
    if(Timeout == status)
    {
        return status;
    }

    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 1000000);//AQUI OCURRE TIMEOUT CON CEREG=0

    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, response_ok, sizeof(response_ok), 500000);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        return status;
    }

    // If process is OK then send result Success

    return Success;
}

cm_q03_codes_t CM_Q03_EG915_Configure_Bands(bands_struct_t seleccion)
{
    uint8_t qcfg_band[15] = {"AT+QCFG=\"band\","};
    uint8_t band_selection_string[50] = {0};
    uint16_t status = 0;
    uint8_t cmd_buff[80] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[32] = {0};
    uint16_t rxbuffer_size ;

    Concatenate_Command(cmd_buff, qcfg_band, sizeof(qcfg_band), &cmd_buff_size);

    switch (seleccion)
    {
        case linksfield:
            strcpy((char *)(band_selection_string), (char *)("c,8,10,1"));
            break;
        case mexico:
            strcpy((char *)(band_selection_string), (char *)("c,8000008,8000010,1"));
            break;
        case usa:
            strcpy((char *)(band_selection_string), (char *)("f,2000000000800081A,2000000000800081A,1"));
            break;
        case EG915LA:
            strcpy((char *)(band_selection_string), (char *)("FFFF,7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF,1"));
            break;
        case colombia:
            return User_Error;
            break;
        default:
            return User_Error;
            break;

    }

    Concatenate_Command(cmd_buff, band_selection_string, sizeof(band_selection_string), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 1000000);
    if(Timeout == status)
    {
        return status;
    }

    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 1000000);
    if(Timeout == status)
    {
        return status;
    }
    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, response_ok, sizeof(response_ok), 500000);
    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuffer_size);
        return status;
    }
    return Success;
}
