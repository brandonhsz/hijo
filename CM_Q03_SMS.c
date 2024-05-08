/*
 * CM_Q01_SMS.c
 *
 *  Created on: 24/07/2019
 *      Author: S4IoT Alex
 */

#include <CM_Q03_SMS.h>

#define INIT_VAL			(0)

#define STRING_EQUAL		(0)

#define SMS_READ_DELAY		(250000)

#define CDSI_POS			(71)
#define CMTI_POS			(72)
#define QIND_SMS_DONE_POS	(2)

#define SM					(18)
#define ME					(32)
#define MT					(33)
#define SR					(37)

extern uint8_t character_command_end;
extern uint8_t command_comma;
extern uint8_t command_quotes;
extern uint8_t response_ok[2];
extern uint8_t g_interrupt_ri;
extern uint16_t g_count_read;
extern uint16_t g_count_valid_data;

extern no_params_flags_t no_param_urc;
extern u16_2_t cdsi;
extern u16_2_t cmti;
extern uint8_t urc_flags[URC_FLAG_SIZE];

typedef enum
{
    sms_chset_GSM,
	sms_chset_IRA,
	sms_chset_UCS2

}sms_character_set_t;

uint8_t sms_ctrl_z[2] = "\x1A\r";


uint8_t sms_response_cpms[6] = {"+CPMS:"};
uint8_t sms_response_cmgr[6] = "+CMGR:";
uint8_t sms_response_cmgr_sto[12] = "+CMGR: \"STO ";

uint8_t sms_response_cmgw[6]={"+CMGW:"};

uint8_t sms_response_cmgs_ok[6]={"+CMGS:"};

uint8_t sms_response_cmss[6]={"+CMSS:"};

uint8_t sms_response_csca[6]={"+CSCA:"};

cm_q03_codes_t CM_Q01_SMS_Select_Read_Message_Storage(sms_mem_storage_t message_storage, uint8_t *message_in_storage, uint8_t *total_size_storage);

cm_q03_codes_t CM_Q01_SMS_Select_Write_Message_Storage(sms_mem_storage_t message_storage, uint8_t *message_in_storage, uint8_t *total_size_storage);


cm_q03_codes_t CM_Q03_SMS_Message_Format(sms_msg_format_t mode)
{
    uint8_t command_format[10] = "AT+CMGF=1";
    uint16_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t rxbuff[10] = {0};

    command_format[8] = mode + '0';
    command_format[9] = character_command_end;

    Delay(50000);
    CM_Q03_buffer_clean();


    status = CM_Q03_Write(command_format, sizeof(command_format), 5000000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);

        CM_Q03_buffer_clean();
        return status;
    }

    CM_Q03_buffer_clean();
    return status;
}


cm_q03_codes_t CM_Q03_SMS_Service_Center_Address(uint8_t *center_address, uint16_t *center_address_size, uint16_t *type_address)
{
    uint8_t command_format[9] = "AT+CSCA?\r";
    uint16_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t rxbuff[40] = {0};
    uint8_t rx_number[6] = {0};
    uint16_t rx_number_size = 0;


    command_format[8] = character_command_end;

    Delay(50000);
    CM_Q03_buffer_clean();


    status = CM_Q03_Write(command_format, sizeof(command_format), 5000000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, sms_response_csca,
                                     sizeof(sms_response_csca), 5000000);

    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);

        CM_Q03_buffer_clean();
        return status;
    }

    rxbuff[rxbuff_size]=',';
    rxbuff_size++;

    CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, rxbuff_size, center_address,
            center_address_size, '"', '"');

    CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, rxbuff_size,rx_number,
                                                       &rx_number_size, ',', ',');

    *type_address = (uint16_t)atoi(rx_number);


    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);

        CM_Q03_buffer_clean();
        return status;
    }

    CM_Q03_buffer_clean();
    return status;
}


cm_q03_codes_t CM_Q03_SMS_Select_Received_Message_Storage(sms_mem_storage_t message_storage, uint8_t *message_in_storage, uint8_t *total_size_storage)
{
    uint8_t command_storage[19] ={"AT+CPMS=\"\",\"\",\"ME\"\r"};

    uint16_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t rxbuff[50] = {0};
    uint8_t rx_number[4] = {0};
    uint16_t rx_number_size = 0;

    if(sms_sim == message_storage)
    {
    	command_storage[15] = 'S';
    	command_storage[16] = 'M';

    }
    else if(sms_device != message_storage)
    {
        CM_Q03_buffer_clean();
    	return Invalid_parameter;
    }

    command_storage[18] =character_command_end;

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(command_storage, sizeof(command_storage), 5000000);

    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, sms_response_cpms,
                                     sizeof(sms_response_cpms), 5000000);
    if (0 != status)
    {

        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }

    rxbuff[rxbuff_size]=',';
    rxbuff_size++;

    memset(rx_number, '\0', sizeof(rx_number));
    CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff,
             rxbuff_size, rx_number, &rx_number_size, ',', 4, ',');
    *message_in_storage = (uint8_t)atoi(rx_number);

    memset(rx_number, '\0', sizeof(rx_number));
    CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff,
             rxbuff_size, rx_number, &rx_number_size, ',', 5, ',');
    *total_size_storage = (uint8_t)atoi(rx_number);

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }


    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {

        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }

    CM_Q03_buffer_clean();
    return status;
}


cm_q03_codes_t CM_Q01_SMS_Select_Write_Message_Storage(sms_mem_storage_t message_storage, uint8_t *message_in_storage, uint8_t *total_size_storage)
{
    uint8_t command_storage[19] ={"AT+CPMS=\"\",\"ME\",\"\"\r"};

    uint16_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t rxbuff[50] = {0};
    uint8_t rx_number[4] = {0};
    uint16_t rx_number_size = 0;

    if(sms_sim == message_storage)
    {
        command_storage[12] = 'S';
        command_storage[13] = 'M';

    }
    else if(sms_device != message_storage)
    {
        CM_Q03_buffer_clean();
        return Invalid_parameter;
    }

    command_storage[18] =character_command_end;

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(command_storage, sizeof(command_storage), 5000000);

    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, sms_response_cpms,
                                     sizeof(sms_response_cpms), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }

    memset(rx_number, '\0', sizeof(rx_number));
    CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff,
             rxbuff_size, rx_number, &rx_number_size, ',', 2, ',');
    *message_in_storage = (uint8_t)atoi(rx_number);

    memset(rx_number, '\0', sizeof(rx_number));
    CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff,
             rxbuff_size, rx_number, &rx_number_size, ',', 3, ',');
    *total_size_storage = (uint8_t)atoi(rx_number);

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }


    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }

    CM_Q03_buffer_clean();
    return status;
}

cm_q03_codes_t CM_Q01_SMS_Select_Read_Message_Storage(sms_mem_storage_t message_storage, uint8_t *message_in_storage, uint8_t *total_size_storage)
{
    uint8_t command_storage[19] ={"AT+CPMS=\"ME\",\"\",\"\"\r"};

    uint16_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t rxbuff[50] = {0};
    uint8_t rx_number[4] = {0};
    uint16_t rx_number_size = 0;

    if(sms_sim == message_storage)
    {
        command_storage[9] = 'S';
        command_storage[10] = 'M';

    }
    else if(sms_device != message_storage)
    {
        CM_Q03_buffer_clean();
        return Invalid_parameter;
    }

    command_storage[18] =character_command_end;

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(command_storage, sizeof(command_storage), 5000000);

    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, sms_response_cpms,
                                     sizeof(sms_response_cpms), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }

    memset(rx_number, '\0', sizeof(rx_number));
    CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, rxbuff_size, rx_number,
            &rx_number_size, ' ', ',');
    *message_in_storage = (uint8_t)atoi(rx_number);


    memset(rx_number, '\0', sizeof(rx_number));
    CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, rxbuff_size, rx_number,
            &rx_number_size, ',', ',');
    *total_size_storage = (uint8_t)atoi(rx_number);


    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }


    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }

    CM_Q03_buffer_clean();
    return status;
}


cm_q03_codes_t CM_Q03_SMS_Message_and_Total_size_in_Storage(sms_mem_storage_t message_storage, uint8_t *message_in_storage, uint8_t *total_size_storage)
{
    uint8_t command_storage[19] ={"AT+CPMS=\"ME\",\"\",\"\"\r"};

    uint16_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t rxbuff[50] = {0};
    uint8_t rx_number[4] = {0};
    uint16_t rx_number_size = 0;

    if(sms_sim == message_storage)
    {
        command_storage[9] = 'S';
        command_storage[10] = 'M';

    }
    else if(sms_device != message_storage)
    {
        CM_Q03_buffer_clean();
        return Invalid_parameter;
    }

    command_storage[18] =character_command_end;

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(command_storage, sizeof(command_storage), 5000000);

    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, sms_response_cpms,
                                     sizeof(sms_response_cpms), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }

    memset(rx_number, '\0', sizeof(rx_number));
    CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, rxbuff_size, rx_number,
            &rx_number_size, ' ', ',');
    *message_in_storage = (uint8_t)atoi(rx_number);


    memset(rx_number, '\0', sizeof(rx_number));
    CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, rxbuff_size, rx_number,
            &rx_number_size, ',', ',');
    *total_size_storage = (uint8_t)atoi(rx_number);


    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }


    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }

    CM_Q03_buffer_clean();
    return status;
}


//number of message received
cm_q03_codes_t CM_Q03_SMS_Number_of_Message_Received(uint8_t *message_received, uint8_t *total_size_storage)
{
    uint8_t command_number_message[9] = "AT+CPMS?\r";
    uint16_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t rxbuff[50] = {0};
    uint8_t rx_number[4] = {0};
    uint16_t rx_number_size = 0;

    command_number_message[8]=character_command_end;

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(command_number_message, sizeof(command_number_message), 5000000);

    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, sms_response_cpms,
                                     sizeof(sms_response_cpms), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }


    rxbuff[rxbuff_size]=',';
    rxbuff_size++;

    memset(rx_number, '\0', sizeof(rx_number));
    CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff,
             rxbuff_size, rx_number, &rx_number_size, ',', 7, ',');
    *message_received = (uint8_t)atoi(rx_number);

    memset(rx_number, '\0', sizeof(rx_number));
    CM_Q03_Read_Response_count_Start_and_Stop_read_character(rxbuff,
             rxbuff_size, rx_number, &rx_number_size, ',', 8, ',');
    *total_size_storage = (uint8_t)atoi(rx_number);

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }


    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }

    CM_Q03_buffer_clean();
    return status;
}

cm_q03_codes_t CM_Q03_SMS_Delete_Message(sms_mem_storage_t message_storage, uint16_t msg_index, sms_delete_msg_t msgs_to_delete)
{
    uint8_t command_delete[8] = "AT+CMGD=";
    uint8_t command_delete_all[4] = "1,4\r";
    uint8_t command_delete_read[4] = "1,1\r";
    uint16_t rxbuff_size = INIT_VAL;
    uint8_t rxbuff[30] = {INIT_VAL};

    uint8_t cmd_buff[20] = {INIT_VAL};
    uint16_t cmd_buff_size = INIT_VAL;
    uint8_t temp_buff[3] = {INIT_VAL};
    uint16_t temp_buff_size=INIT_VAL;

    uint8_t message_in_storage;
    uint8_t total_size_storage;

    cm_q03_codes_t status = INIT_VAL;

    status = CM_Q01_SMS_Select_Read_Message_Storage( message_storage, &message_in_storage, &total_size_storage);

    if(Success != status)
    {
        CM_Q03_buffer_clean();
        return (status);
    }


    if(sms_delete_all_msgs == msgs_to_delete)
    {
        command_delete_all[3]=character_command_end;
    	Concatenate_Command(cmd_buff, command_delete, sizeof(command_delete), &cmd_buff_size);
    	Concatenate_Command(cmd_buff, command_delete_all, sizeof(command_delete_all), &cmd_buff_size);
    }

    else if(sms_delete_all_read_msgs == msgs_to_delete)
    {
        command_delete_read[3]=character_command_end;
    	Concatenate_Command(cmd_buff, command_delete, sizeof(command_delete), &cmd_buff_size);
    	Concatenate_Command(cmd_buff, command_delete_read, sizeof(command_delete_read), &cmd_buff_size);
    }

    else if(sms_delete_index_msg == msgs_to_delete)
    {
    	Concatenate_Command(cmd_buff, command_delete, sizeof(command_delete), &cmd_buff_size);
    	Dec_to_Ascii(msg_index, temp_buff, &temp_buff_size);
    	Concatenate_Command(cmd_buff, temp_buff, temp_buff_size, &cmd_buff_size);
    	Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    }

    else
    {
        CM_Q03_buffer_clean();
    	return Invalid_parameter;
    }


    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff,cmd_buff_size , 5000000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }
    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
    }

    CM_Q03_buffer_clean();
    return status;
}

cm_q03_codes_t CM_Q01_SMS_Select_Character_Set(sms_character_set_t character_mode)
{
    uint8_t command_format_gsm[14] = "AT+CSCS=\"GSM\"\r";
    uint8_t command_format_ira[14] = "AT+CSCS=\"IRA\"\r";
    uint8_t command_format_ucs2[15] = "AT+CSCS=\"UCS2\"\r";

    uint16_t status = INIT_VAL;
    uint16_t rxbuff_size = INIT_VAL;
    uint8_t rxbuff[10] = {INIT_VAL};

    command_format_gsm[13]=character_command_end;
    command_format_ira[13]=character_command_end;
    command_format_ucs2[14]=character_command_end;


    Delay(50000);
    CM_Q03_buffer_clean();

    if(sms_chset_GSM == character_mode){

        status = CM_Q03_Write(command_format_gsm, sizeof(command_format_gsm), 5000000);
    }
    else if(sms_chset_IRA == character_mode){

        status = CM_Q03_Write(command_format_ira, sizeof(command_format_ira), 5000000);
    }
    else if(sms_chset_UCS2 == character_mode){

        status = CM_Q03_Write(command_format_ucs2, sizeof(command_format_ucs2), 5000000);
    }
    else{

        return User_Error;
    }

    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }
    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }

    CM_Q03_buffer_clean();
    return status;
}

cm_q03_codes_t CM_Q03_SMS_Read_Message(sms_mem_storage_t message_storage, uint16_t msg_index, uint8_t *orig_addr,
		uint16_t *orig_addr_size, uint8_t *time_stamp, uint16_t *time_stamp_size,
		uint8_t *payload, uint16_t *payload_size)
{
	uint8_t read_cmd[8] = "AT+CMGR=";

	uint8_t cmd_buff[14] = {INIT_VAL};
	uint16_t cmd_buff_size = INIT_VAL;
	uint8_t ascii_buff[3] = {INIT_VAL};
	uint16_t ascii_buff_size = INIT_VAL;
	uint8_t read_buff[500] = {INIT_VAL};
	uint16_t read_buff_size = INIT_VAL;

    uint8_t message_in_storage;
    uint8_t total_size_storage;

    cm_q03_codes_t status = INIT_VAL;

    status = CM_Q01_SMS_Select_Read_Message_Storage( message_storage, &message_in_storage, &total_size_storage);

    if(Success != status)
    {
        CM_Q03_buffer_clean();
        return (status);
    }


    status = (uint16_t)CM_Q03_SMS_Message_Format(sms_text_mode);

    if(Success != status)
    {
        CM_Q03_buffer_clean();
    	return (status);
    }



    (*orig_addr_size) = INIT_VAL;
    (*time_stamp_size) = INIT_VAL;

    Concatenate_Command(cmd_buff, read_cmd, sizeof(read_cmd), &cmd_buff_size);
	Dec_to_Ascii(msg_index, ascii_buff, &ascii_buff_size);
	Concatenate_Command(cmd_buff, ascii_buff, ascii_buff_size, &cmd_buff_size);
	Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end),
			&cmd_buff_size);

	status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
	Delay(SMS_READ_DELAY);

	if(Success != status)
	{
        CM_Q03_buffer_clean();
		return (status);
	}

	status = CM_Q03_Read(read_buff, &read_buff_size, 5000000);
	if(Success != status)
	{
        CM_Q03_buffer_clean();
		return (status);
	}

	status = CM_Q03_Compare_short_response(read_buff, read_buff_size, sms_response_cmgr,
			sizeof(sms_response_cmgr), 500000);

	if(STRING_EQUAL != status)
	{
		status = CM_Q03_Get_Response(read_buff, read_buff_size);
        CM_Q03_buffer_clean();
		return (status);
	}

	status = CM_Q03_Read_Response_count_Start_and_Stop_read_character(read_buff,
			read_buff_size, orig_addr, orig_addr_size, '\"', 3, '\"');
	if(Success != status)
	{
		CM_Q03_buffer_clean();
		return (status);
	}

    status = CM_Q03_Compare_short_response(read_buff, read_buff_size, sms_response_cmgr_sto,
            sizeof(sms_response_cmgr_sto), 500000);

	if( Success !=status){

        status = CM_Q03_Read_Response_count_Start_and_Stop_read_character(read_buff,
                read_buff_size, time_stamp, time_stamp_size, '\"', 5, '\"');
        if(Success != status)
        {
            CM_Q03_buffer_clean();
            return (status);
        }

	}else{

	    *time_stamp='0';
	    time_stamp_size++;
	}

	status = CM_Q03_Read_Until_OK(payload, payload_size, 500000);

	if(Success != status)
    {
        CM_Q03_buffer_clean();
        return (status);
    }

	CM_Q03_buffer_clean();
	return (status);
}

cm_q03_codes_t CM_Q03_SMS_Read_Message_PDU(sms_mem_storage_t message_storage, uint16_t msg_index, uint16_t *length,
		uint8_t *payload, uint16_t *payload_size)
{
	uint8_t read_cmd[8] = "AT+CMGR=";

	uint8_t cmd_buff[14] = {INIT_VAL};
	uint16_t cmd_buff_size = INIT_VAL;
	uint8_t ascii_buff[5] = {INIT_VAL};
	uint16_t ascii_buff_size = INIT_VAL;
	uint8_t read_buff[500] = {INIT_VAL};
	uint16_t read_buff_size = INIT_VAL;

    uint8_t message_in_storage;
    uint8_t total_size_storage;

    cm_q03_codes_t status = INIT_VAL;

    status = CM_Q01_SMS_Select_Read_Message_Storage( message_storage, &message_in_storage, &total_size_storage);

    if(Success != status)
    {
        CM_Q03_buffer_clean();
        return (status);
    }

    status = (uint16_t)CM_Q03_SMS_Message_Format(sms_pdu_mode);

    if(Success != status)
    {
        CM_Q03_buffer_clean();
    	return (status);
    }

    Concatenate_Command(cmd_buff, read_cmd, sizeof(read_cmd), &cmd_buff_size);
	Dec_to_Ascii(msg_index, ascii_buff, &ascii_buff_size);
	Concatenate_Command(cmd_buff, ascii_buff, ascii_buff_size, &cmd_buff_size);
	Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end),
			&cmd_buff_size);


    Delay(50000);
    CM_Q03_buffer_clean();

	status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
	Delay(SMS_READ_DELAY);

	if(Success != status)
	{
        CM_Q03_buffer_clean();
		return (status);
	}

	status = CM_Q03_Read(read_buff, &read_buff_size, 5000000);
	if(Success != status)
	{
        CM_Q03_buffer_clean();
		return (status);
	}

	status = CM_Q03_Compare_short_response(read_buff, read_buff_size, sms_response_cmgr,
			sizeof(sms_response_cmgr), 500000);

	if(STRING_EQUAL != status)
	{
		status = CM_Q03_Get_Response(read_buff, read_buff_size);
        CM_Q03_buffer_clean();
		return (status);
	}

	ascii_buff_size = 0;

//    status = CM_Q01_Read_Until_OK(payload, payload_size, 1000);
	read_buff[read_buff_size]=',';
	read_buff_size++;
	status = CM_Q03_Read_Response_count_Start_and_Stop_read_character(read_buff,
			read_buff_size, ascii_buff, &ascii_buff_size, ',', 2, ',');

	if(Success != status)
	{
		CM_Q03_buffer_clean();
		return (status);
	}

	(*length) = Ascii_to_Number(ascii_buff, ascii_buff_size);

	status = CM_Q03_Read_Until_OK(payload, payload_size, 500000);

    if(Success != status)
    {
        CM_Q03_buffer_clean();
        return (status);
    }

	CM_Q03_buffer_clean();
    return (status);
}

cm_q03_codes_t CM_Q03_SMS_Send_Messages(uint8_t *destination_number, uint8_t size_destination_number,
uint8_t *message, uint8_t size_message)
{
    uint8_t command_send[9] = "AT+CMGS=\"";
    cm_q03_codes_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t rxbuff[30] = {0};
    uint8_t cmd_buff[40] = {0};
    uint16_t cmd_buff_size = 0;

    status = (uint16_t)CM_Q03_SMS_Message_Format(sms_text_mode);

    if(Success != status)
    {
        CM_Q03_buffer_clean();
    	return (status);
    }

    Concatenate_Command(cmd_buff, command_send, sizeof(command_send), &cmd_buff_size);
    Concatenate_Command(cmd_buff, destination_number, size_destination_number, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    Delay(100000);
    if(4 == g_count_valid_data)
    {
        CM_Q03_buffer_clean();
        status = CM_Q03_Write(message, size_message, 5000000);
        status = CM_Q03_Write(sms_ctrl_z, sizeof(sms_ctrl_z), 5000000);
    }
    else{
        Delay(200000);
        CM_Q03_buffer_clean();
        status = CM_Q03_Write(sms_ctrl_z, sizeof(sms_ctrl_z), 5000000);
        CM_Q03_buffer_clean();
        return Timeout;
    }

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 60000000);
    if(Timeout == status)
    {
        status = CM_Q03_Read(rxbuff, &rxbuff_size, 60000000);
        if(Timeout == status)
        {
            CM_Q03_buffer_clean();
            return status;
        }
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, sms_response_cmgs_ok,
                                     sizeof(sms_response_cmgs_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }


    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }

    CM_Q03_buffer_clean();
    return status;
}

cm_q03_codes_t CM_Q03_SMS_Send_Messages_PDU(uint8_t length, uint8_t *message_pdu,
		uint8_t size_message_pdu)
{
    uint8_t command_send[9] = "AT+CMGS=";
    cm_q03_codes_t status = 0;
    uint16_t rxbuff_size = 0;
    uint8_t rxbuff[30] = {0};
    uint8_t cmd_buff[40] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t length_buff[3]={0};
    uint16_t length_buff_size=0;

    status = (uint16_t)CM_Q03_SMS_Message_Format(sms_pdu_mode);

    if(Success != status)
    {
        CM_Q03_buffer_clean();
    	return (status);
    }

    Concatenate_Command(cmd_buff, command_send, sizeof(command_send), &cmd_buff_size);
    Dec_to_Ascii(length,length_buff,&length_buff_size);
    Concatenate_Command(cmd_buff, length_buff, length_buff_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();


    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    Delay(100000);
    if(4 == g_count_valid_data)
    {
        CM_Q03_buffer_clean();
        status = CM_Q03_Write(message_pdu, size_message_pdu, 5000000);
        status = CM_Q03_Write(sms_ctrl_z, sizeof(sms_ctrl_z), 5000000);
    }
    else{
        Delay(200000);
        CM_Q03_buffer_clean();
        status = CM_Q03_Write(sms_ctrl_z, sizeof(sms_ctrl_z), 5000000);
        CM_Q03_buffer_clean();
        return Timeout;
    }

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 60000000);
    if(Timeout == status)
    {
        status = CM_Q03_Read(rxbuff, &rxbuff_size, 60000000);
        if(Timeout == status)
        {
            CM_Q03_buffer_clean();
            return status;
        }
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, sms_response_cmgs_ok,
                                     sizeof(sms_response_cmgs_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }


    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }

    CM_Q03_buffer_clean();
    return status;
}

cm_q03_codes_t CM_Q03_SMS_Write_Messages_to_Memory(sms_mem_storage_t message_storage, uint8_t *destination_number,
		uint8_t size_destination_number, uint8_t *message, uint8_t size_message,
		uint8_t *index_in_memory)
{
    uint8_t command_write[9] = "AT+CMGW=\"";
    uint16_t rxbuff_size = 0;
    uint8_t rxbuff[30] = {0};
    uint8_t cmd_buff[40] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rx_number[4] = {0};
    uint16_t rx_number_size = 0;

    uint8_t message_in_storage;
    uint8_t total_size_storage;

    cm_q03_codes_t status = INIT_VAL;

    status = CM_Q01_SMS_Select_Write_Message_Storage( message_storage, &message_in_storage, &total_size_storage);

    if(Success != status)
    {
        CM_Q03_buffer_clean();
        return (status);
    }

    status = (uint16_t)CM_Q03_SMS_Message_Format(sms_text_mode);

    if(Success != status)
    {
        CM_Q03_buffer_clean();
    	return (status);
    }

    CM_Q03_buffer_clean();

    Concatenate_Command(cmd_buff, command_write, sizeof(command_write), &cmd_buff_size);
    Concatenate_Command(cmd_buff, destination_number, size_destination_number, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_quotes, sizeof(command_quotes), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();

    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    Delay(100000);
    if(4 == g_count_valid_data)
    {
        CM_Q03_buffer_clean();
        status = CM_Q03_Write(message, size_message, 5000000);
        status = CM_Q03_Write(sms_ctrl_z, sizeof(sms_ctrl_z), 5000000);
    }
    else{
        Delay(200000);
        CM_Q03_buffer_clean();
        status = CM_Q03_Write(sms_ctrl_z, sizeof(sms_ctrl_z), 5000000);
        CM_Q03_buffer_clean();
        return Timeout;
    }

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, sms_response_cmgw,
                                     sizeof(sms_response_cmgw), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }

    rxbuff[rxbuff_size]=',';
    rxbuff_size++;

    CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, rxbuff_size, rx_number,
            &rx_number_size, ' ', ',');
    *index_in_memory=atoi(rx_number);

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }

    CM_Q03_buffer_clean();
    return status;
}

cm_q03_codes_t CM_Q03_SMS_Write_Messages_PDU_to_Memory(sms_mem_storage_t message_storage, uint8_t length, uint8_t *message,
		uint8_t size_message, uint8_t *index_in_memory)
{
    uint8_t command_write[8] = "AT+CMGW=";
    uint16_t rxbuff_size = 0;
    uint8_t rxbuff[30] = {0};
    uint8_t cmd_buff[40] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rx_number[4] = {0};
    uint16_t rx_number_size = 0;

    uint8_t length_buff[3]={0};
    uint16_t length_buff_size=0;

    uint8_t message_in_storage;
    uint8_t total_size_storage;

    cm_q03_codes_t status = INIT_VAL;

    status = CM_Q01_SMS_Select_Write_Message_Storage( message_storage, &message_in_storage, &total_size_storage);

    if(Success != status)
    {
        CM_Q03_buffer_clean();
        return (status);
    }

    status = (uint16_t)CM_Q03_SMS_Message_Format(sms_pdu_mode);

    if(Success != status)
    {
        CM_Q03_buffer_clean();
    	return ((cm_q03_codes_t)status);
    }

    Concatenate_Command(cmd_buff, command_write, sizeof(command_write), &cmd_buff_size);
    Dec_to_Ascii(length,length_buff,&length_buff_size);
    Concatenate_Command(cmd_buff, length_buff, length_buff_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();


    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    Delay(100000);
    if(4 == g_count_valid_data)
    {
        CM_Q03_buffer_clean();
        status = CM_Q03_Write(message, size_message, 5000000);
        status = CM_Q03_Write(sms_ctrl_z, sizeof(sms_ctrl_z), 5000000);
    }
    else{
        Delay(200000);
        CM_Q03_buffer_clean();
        status = CM_Q03_Write(sms_ctrl_z, sizeof(sms_ctrl_z), 5000000);

        return Timeout;
    }

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, sms_response_cmgw,
                                     sizeof(sms_response_cmgw), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }

    rxbuff[rxbuff_size]=',';
    rxbuff_size++;

    CM_Q03_Read_Response_Start_and_Stop_read_character(rxbuff, rxbuff_size, rx_number,
            &rx_number_size, ' ', ',');
    *index_in_memory=atoi(rx_number);

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }

    CM_Q03_buffer_clean();
    return status;
}

cm_q03_codes_t CM_Q03_SMS_Send_Message_from_Storage(sms_mem_storage_t message_storage, uint8_t index)
{
    uint8_t command_send_storage[8] = "AT+CMSS=";
    uint16_t rxbuff_size = 0;
    uint8_t rxbuff[10] = {0};
    uint8_t index_buff[3]={0};
    uint16_t index_buff_size=0;
    uint8_t cmd_buff[20] = {0};
    uint16_t cmd_buff_size = 0;

    uint8_t message_in_storage;
    uint8_t total_size_storage;

    cm_q03_codes_t status = INIT_VAL;

    status = CM_Q01_SMS_Select_Write_Message_Storage( message_storage, &message_in_storage, &total_size_storage);

    if(Success != status)
    {
        CM_Q03_buffer_clean();
        return (status);
    }


    memset(index_buff,'\0',sizeof(index_buff));

    Concatenate_Command(cmd_buff, command_send_storage, sizeof(command_send_storage), &cmd_buff_size);
    Dec_to_Ascii(index,index_buff,&index_buff_size);
    Concatenate_Command(cmd_buff, index_buff, index_buff_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();


    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }


    status = CM_Q03_Read(rxbuff, &rxbuff_size, 60000000);
    if(Timeout == status)
    {
        status = CM_Q03_Read(rxbuff, &rxbuff_size, 60000000);
        if(Timeout == status)
        {
            CM_Q03_buffer_clean();
            return status;
        }
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, sms_response_cmss,
                                     sizeof(sms_response_cmss), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Read(rxbuff, &rxbuff_size, 500000);
    if(Timeout == status)
    {
        CM_Q03_buffer_clean();
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuff_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rxbuff, rxbuff_size);
        CM_Q03_buffer_clean();
        return status;
    }

    CM_Q03_buffer_clean();
    return status;
}

urc_received_status_t CM_Q03_SMS_Status_Report_Received_Flag(void)
{
	return (urc_received_status_t)((urc_flags[DIV_BY_8(CDSI_POS)]
					>> (MODULE_8_SFT(CDSI_POS))) & 0x01);
}

void CM_Q03_SMS_Clear_Status_Report_Received_Flag(void)
{
	urc_flags[DIV_BY_8(CDSI_POS)] &= ~(1 << MODULE_8_SFT(CDSI_POS));
}

void CM_Q03_SMS_Status_Report_Received(sms_recv_mem_storeage_t *mem,
		uint8_t *index)
{
	switch(cdsi.u16_0)
	{
		case SM:
			(*mem) = sms_recv_in_sim;
		break;

		case ME:
			(*mem) = sms_recv_in_device;
		break;

		case MT:
			(*mem) = sms_recv_in_device;
		break;

		case SR:
			(*mem) = sms_recv_in_report_storage;
		break;

		default:
			(*mem) = sms_recv_unknown;
		break;
	}

	(*index) = cdsi.u16_1;
	urc_flags[DIV_BY_8(CDSI_POS)] &= ~(1 << MODULE_8_SFT(CDSI_POS));
}

urc_received_status_t CM_Q03_SMS_Deliver_Received_Flag(void)
{
	return (urc_received_status_t)((urc_flags[DIV_BY_8(CMTI_POS)]
					>> (MODULE_8_SFT(CMTI_POS))) & 0x01);
}

void CM_Q03_SMS_Clear_Deliver_Received_Flag(void)
{
	urc_flags[DIV_BY_8(CMTI_POS)] &= ~(1 << MODULE_8_SFT(CMTI_POS));
}

void CM_Q03_SMS_Deliver_Received(sms_recv_mem_storeage_t *mem,
		uint8_t *index)
{
	switch(cmti.u16_0)
	{
		case SM:
			(*mem) = sms_recv_in_sim;
		break;

		case ME:
			(*mem) = sms_recv_in_device;
		break;

		case MT:
			(*mem) = sms_recv_in_device;
		break;

		case SR:
			(*mem) = sms_recv_in_report_storage;
		break;

		default:
			(*mem) = sms_recv_unknown;
		break;
	}

	(*index) = cmti.u16_1;
	urc_flags[DIV_BY_8(CMTI_POS)] &= ~(1 << MODULE_8_SFT(CMTI_POS));
}

urc_received_status_t CM_Q03_SMS_Done_Initialization_Flag(void)
{
	return (urc_received_status_t)(no_param_urc.flags_struct.urc_sms_done);
}

void CM_Q03_SMS_Clear_Done_Initialization_Flag(void)
{
	no_param_urc.flags_struct.urc_sms_done = 0;
}





































