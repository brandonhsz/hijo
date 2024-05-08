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
 *
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
 * \file       state_machine.c
 * \details    *For more information, please refer to the included documentation
 *             provided.
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

#include <CM_Q03.h>
#include <CM_Q03_File.h>
#include <CM_Q03_General.h>
#include <CM_Q03_MQTT.h>
#include <CM_Q03_SSL.h>
#include <CM_Q03_TCP_IP.h>
#include "state_machine.h"
#include "Watchdog.h"
#include "init_config_functions.h"
#include "fuel_gauge_lite.h"
#include "FOTA.h"
#include "SHT20.h"
#include "HX711.h"
#define SM_MAX_STATES       (51)
uint32_t counterssi = 0;
static uint8_t msg_cnt = 0;

#define created_filename_size   (14)
#define msg_cali_size           (8)

/*
 * NOTE: Enable or Disable modules inside the system.
  */
#define LPU_ENABLE              (1)
#define RTC_ENABLE              (0)
#define FOTA_ENABLE             (1)
#define BIDIRECCIONALITY_ENABLE (1)
#define ENABLE_DEBUG            (0)
#define FIRST_HTTP_ENABLE       (0)

typedef enum
{
/***** Initialization of the system  *****/
    sm_cmplify_init_wd_pos,                 //0
    sm_fota_pos,                            //1
/***** Initialization of the system  *****/

/***** Config cellular IoT ***************/
    sm_cm_q03_turn_on_initial_pos,          //3
    sm_general_get_imei_pos,                //4
    sm_tcp_ip_set_apn_pos,                  //5
    sm_general_conf_network_pos,            //6
    sm_general_network_reg_pos,             //7
    sm_general_reset_network_pos,           //8
//    sm_general_get_eps_reg_pos,               //9
//    sm_cm_q03_get_signal_qty_pos,         //10
/***** Config cellular IoT ***************/

/***** Config MQTT ***********************/
    sm_ssl_conf_pos,                        //11
    sm_file_extract_ptrs_pos,               //13
    sm_mqtt_conf_pos,                       //12
//    sm_tcp_ip_act_pdp_pos,                    //14
//    sm_tcp_ip_deact_pdp_pos,              //15
    sm_http_first_transmission_pos,         //16
    sm_mqtt_open_pos,                       //17
    sm_mqtt_conn_pos,                       //18
    sm_subscribe_mqtt_pos,                  //19
    sm_download_parameters_pos,             //20
/***** Config MQTT ***********************/

/***** Capture RTC ***********************/
    sm_capture_rtc_pos,                     //21
/***** Capture RTC ***********************/

/***** Capture data ***********************/
    sm_cmplify_init_peripherals_pos,        //2
    sm_cmplify_read_peripherals_pos,        //22
    sm_read_voltage_pos,
    sm_general_get_timestamp_pos,           //23
    sm_cmplify_deinit_peripherals_pos,      //25
    sm_alarm_event_pos,                     //26
/***** Capture data ***********************/

/***** Data Format ***********************/
    sm_data_format_pos,                     //27
/***** Data Format ***********************/

/***** Transmit latest messages **********/
    sm_file_query_pos,                      //28
    sm_file_download_pos,                   //29
    sm_mqtt_pub_old_data_pos,               //30
    sm_file_delete_pos,                     //31
    sm_file_update_read_ptr_file_pos,       //32
/***** Transmit latest messages **********/



/***** Transmit data ***********************/
    sm_mqtt_pub_new_data_pos,               //33
    sm_mqtt_conn_state_pos,                 //34
    sm_file_query_av_space_pos,             //35
    sm_file_free_space_pos,                 //36
    sm_file_overwrite_pos,                  //37
    sm_file_upload_pos,                     //38
    sm_file_update_both_ptr_file_pos,       //39
/***** Transmit data ***********************/
    sm_cm_q03_turn_off_initial_pos,         //40

/***** Wait event ***********************/
    sm_wait_event_pos,                      //41
/***** Wait event ***********************/

/***** FOTA event ***********************/
    sm_fota_download_pos,                   //42
    sm_fota_write_pos,                      //43
/***** FOTA event ***********************/
    sm_reset_pos                            //44
}sm_states_t;

typedef struct
{
    next_state_t (*stateFunc)();
    sm_states_t const *next;
}state_machine_t;

/************************ Initialization of the system  ************************************/
const sm_states_t sm_cmplify_init_wd_next  = sm_fota_pos;
const sm_states_t sm_fota_next = sm_cm_q03_turn_on_initial_pos;
/************************ Initialization of the system  ************************************/

/************************************ FOTA ****************************************/
const sm_states_t sm_fota_download_next[4] =
{
        sm_fota_write_pos,
        sm_fota_download_pos,
        sm_reset_pos,
        sm_capture_rtc_pos,
};
const sm_states_t sm_fota_write_next[3] = {

        sm_reset_pos,
        sm_capture_rtc_pos,
        sm_cm_q03_turn_on_initial_pos


};
/************************************ FOTA ****************************************/

/****************************** Config cellular IoT ****************************************/

const sm_states_t sm_cm_q03_turn_on_initial_next[3] =
{
    sm_general_get_imei_pos,
    sm_cm_q03_turn_on_initial_pos,
    sm_wait_event_pos
};
const sm_states_t sm_general_get_imei_next[2] =
{
    sm_tcp_ip_set_apn_pos,
    sm_general_get_imei_pos
};
const sm_states_t sm_tcp_ip_set_apn_next[2] =
{
    sm_general_conf_network_pos,
    sm_tcp_ip_set_apn_pos
};
const sm_states_t sm_general_conf_network_next[3] =
{
    sm_general_network_reg_pos,
    sm_general_conf_network_pos,
    sm_file_extract_ptrs_pos //Peko modification
};
const sm_states_t sm_general_network_reg_next[4] =
{
    sm_ssl_conf_pos,
    sm_general_network_reg_pos,
    sm_general_reset_network_pos,
    sm_fota_download_pos
};
const sm_states_t sm_general_reset_network_next = sm_general_network_reg_pos;

//const sm_states_t sm_general_get_eps_reg_next = sm_cm_q03_get_signal_qty_pos;
//
//const sm_states_t sm_cm_q03_get_signal_qty_next[2] =
//{
//    sm_ssl_conf_pos,
//    sm_cm_q03_get_signal_qty_pos
//};

/****************************** Config cellular IoT ****************************************/

/************************************ Config MQTT *****************************************/
const sm_states_t sm_ssl_conf_next[2] =
{
    sm_file_extract_ptrs_pos,
    sm_ssl_conf_pos
};

const sm_states_t sm_file_extract_ptrs_next [3]= {
    sm_mqtt_conf_pos,
    sm_data_format_pos,
    sm_cmplify_init_peripherals_pos
};

const sm_states_t sm_mqtt_conf_next[2] =
{
    sm_http_first_transmission_pos,
    sm_mqtt_conf_pos
};



const sm_states_t sm_tcp_ip_act_pdp_next[3] =
{
//  sm_http_first_transmission_pos,
    sm_general_reset_network_pos,
//    sm_tcp_ip_deact_pdp_pos,
};
const sm_states_t sm_tcp_ip_deact_pdp_next[2] =
{
//    sm_tcp_ip_act_pdp_pos,
    sm_general_reset_network_pos
};
const sm_states_t sm_http_first_next[3]={
    sm_mqtt_open_pos,
    sm_http_first_transmission_pos,
    sm_general_reset_network_pos
};
const sm_states_t sm_mqtt_open_next[5] =
{
    sm_mqtt_conn_pos,
    sm_general_network_reg_pos,
    sm_general_reset_network_pos,
    sm_capture_rtc_pos,
    sm_mqtt_open_pos
};
const sm_states_t sm_mqtt_conn_next[5] =
{
    sm_subscribe_mqtt_pos,
    sm_capture_rtc_pos,
    sm_general_reset_network_pos,
    sm_mqtt_conn_pos,
    sm_capture_rtc_pos
};

const sm_states_t sm_subscribe_mqtt_next[3] =
{
    sm_download_parameters_pos,
    sm_subscribe_mqtt_pos,
    sm_capture_rtc_pos
};

const sm_states_t sm_download_parameters_next[4] =
{
    sm_capture_rtc_pos,
    sm_subscribe_mqtt_pos,
    sm_wait_event_pos,
    sm_fota_download_pos
};

/************************************ Config MQTT *****************************************/

/**************************** Transmit latest messages ************************************/

const sm_states_t sm_file_query_next[4] =
{
    sm_file_download_pos,
    sm_mqtt_pub_new_data_pos,
    sm_wait_event_pos,
    sm_file_query_pos
};
const sm_states_t sm_file_download_next[2] =
{
    sm_mqtt_pub_old_data_pos,
    sm_file_query_pos,
};
const sm_states_t sm_mqtt_pub_old_data_next[3] =
{
    sm_file_delete_pos,
    sm_file_query_av_space_pos,
    sm_mqtt_conn_state_pos
};
const sm_states_t sm_file_delete_next[2] =
{
    sm_file_update_read_ptr_file_pos,
    sm_file_delete_pos
};
const sm_states_t sm_file_update_read_ptr_file_next[2] =
{
    sm_file_query_pos,
    sm_file_update_read_ptr_file_pos
};
/**************************** Transmit latest messages ************************************/

/**************************** Capture RTC ************************************/
const sm_states_t sm_capture_rtc_next [2]=
{
    sm_cmplify_init_peripherals_pos,
    sm_data_format_pos,
};
/**************************** Capture RTC ************************************/



/************************************ Capture data ****************************************/
const sm_states_t sm_cmplify_init_peripherals_next =sm_cmplify_read_peripherals_pos;


const sm_states_t sm_cmplify_read_peripherals_next[2] =
{
    sm_read_voltage_pos,
    sm_cmplify_read_peripherals_pos

};

const sm_states_t sm_cmplify_read_voltage_next[2] =
{
        sm_general_get_timestamp_pos,
        sm_read_voltage_pos
};

const sm_states_t sm_general_get_timestamp_next[2] =
{
    sm_cmplify_deinit_peripherals_pos,
    sm_general_get_timestamp_pos
};

const sm_states_t sm_cmplify_deinit_peripherals_next = sm_alarm_event_pos;

const sm_states_t sm_alarm_event_next[3] =
{
    sm_wait_event_pos,
    sm_data_format_pos,
    sm_cm_q03_turn_on_initial_pos
};
/************************************ Capture data ****************************************/

/************************************ Data Format ****************************************/
const sm_states_t sm_data_format_next[3] =
{
    sm_file_query_pos,
    sm_file_query_av_space_pos,
    sm_data_format_pos
};
/************************************ Data Format ****************************************/

/********************************** Transmit data *****************************************/

const sm_states_t sm_mqtt_pub_new_data_next[3] =
{
    sm_cm_q03_turn_off_initial_pos,
    sm_mqtt_conn_state_pos,
    sm_file_query_av_space_pos
};
const sm_states_t sm_mqtt_conn_state_next[4] =
{
    sm_file_query_pos,
    sm_mqtt_conf_pos,
    sm_file_query_av_space_pos,
    sm_mqtt_conn_state_pos,
};
const sm_states_t sm_file_query_av_space_next[4] =
{
    sm_file_upload_pos,
    sm_file_overwrite_pos,
    sm_file_update_both_ptr_file_pos,
    sm_file_free_space_pos,
};
const sm_states_t sm_file_free_space_next[2] =
{
    sm_file_query_av_space_pos,
    sm_file_free_space_pos
};
const sm_states_t sm_file_overwrite_next[2] =
{
    sm_file_upload_pos,
    sm_file_overwrite_pos
};
const sm_states_t sm_file_upload_next[2] =
{
    sm_file_update_both_ptr_file_pos,
    sm_file_upload_pos
};
const sm_states_t sm_file_update_both_ptr_file_next[2] =
{
    sm_cm_q03_turn_off_initial_pos,
    sm_file_update_both_ptr_file_pos
};

/********************************** Transmit data *****************************************/

const sm_states_t sm_turn_off_cm_q03_next = sm_wait_event_pos;

/************************************ Wait event ******************************************/
const sm_states_t sm_wait_event_next[4]=
{
    sm_wait_event_pos,
    sm_cm_q03_turn_on_initial_pos,
    sm_cmplify_init_peripherals_pos,
    sm_alarm_event_pos
};
/************************************ Wait event ******************************************/

/********************* STATE MACHINE TABLE *****************************/
const state_machine_t state_machine[SM_MAX_STATES] =
{
    /***** Initialization of the system  *****/
    {SM_Initialize_Watch_Dog, &sm_cmplify_init_wd_next},
    {SM_FOTA, &sm_fota_next},
    /***** Initialization of the system  *****/

    /***** Config cellular IoT ***************/
    {SM_Turn_On_CM_Q03, sm_cm_q03_turn_on_initial_next},
    {SM_Get_IMEI, sm_general_get_imei_next},
    {SM_Set_APN, sm_tcp_ip_set_apn_next},
    {SM_Configure_Network, sm_general_conf_network_next},
    {SM_Network_Registration, sm_general_network_reg_next},
    {SM_Reset_Network, &sm_general_reset_network_next},
//    {SM_Get_EPS_Reg_Status, &sm_general_get_eps_reg_next},
//    {SM_Get_Signal_Quality, sm_cm_q03_get_signal_qty_next},
    /***** Config cellular IoT ***************/

    /***** Config MQTT ***********************/
    {SM_Configure_SSL, sm_ssl_conf_next},
    {SM_File_Extract_Ptrs, sm_file_extract_ptrs_next},
    {SM_Configure_MQTT, sm_mqtt_conf_next},
//    {SM_Activate_PDP_Context, sm_tcp_ip_act_pdp_next},
//    {SM_Deactivate_PDP_Context, sm_tcp_ip_deact_pdp_next},
    {SM_HTTP_First_Transmission,sm_http_first_next},
    {SM_MQTT_Open, sm_mqtt_open_next},
    {SM_MQTT_Connect, sm_mqtt_conn_next},
    {SM_Subscribe_MQTT,sm_subscribe_mqtt_next},
    {SM_Download_Parameters,sm_download_parameters_next},
    /***** Config MQTT ***********************/

    /***** Capture RTC ***********************/
    {SM_Capture_RTC, sm_capture_rtc_next},
    /***** Capture RTC ***********************/

    {SM_Initialize_Peripherals, &sm_cmplify_init_peripherals_next},

    /***** Capture data ***********************/
    {SM_Read_Peripherals, sm_cmplify_read_peripherals_next},
    {SM_Read_Voltage,sm_cmplify_read_voltage_next},
    {SM_Get_Timestamp, sm_general_get_timestamp_next},
    {SM_Deinitialize_Peripherals, &sm_cmplify_deinit_peripherals_next},
    {SM_Alarm_Event,sm_alarm_event_next},
    /***** Capture data ***********************/

    /***** Data Format ***********************/
    {SM_Data_Format, sm_data_format_next},
    /***** Data Format ***********************/

    /***** Transmit latest messages **********/
    {SM_File_Query, sm_file_query_next},
    {SM_File_Download, sm_file_download_next},
    {SM_MQTT_Publish_Old_Data, sm_mqtt_pub_old_data_next},
    {SM_File_Delete, sm_file_delete_next},
    {SM_File_Update_Read_Ptr_File, sm_file_update_read_ptr_file_next},
    /***** Transmit latest messages **********/

    /***** Transmit data ***********************/
    {SM_MQTT_Publish_New_Data, sm_mqtt_pub_new_data_next},
    {SM_MQTT_Connection_State, sm_mqtt_conn_state_next},

    {SM_File_Query_Av_Space, sm_file_query_av_space_next},
    {SM_File_Free_Space, sm_file_free_space_next},
    {SM_File_Overwrite, sm_file_overwrite_next},
    {SM_File_Upload, sm_file_upload_next},
    {SM_File_Update_Both_File_Ptr, sm_file_update_both_ptr_file_next},
    /***** Transmit data ***********************/

    {SM_Turn_Off_CM_Q03, &sm_turn_off_cm_q03_next},

    /***** Wait event ***********************/
    {SM_Wait_Event,sm_wait_event_next},
    /***** Wait event ***********************/

    {SM_FOTA_Download, sm_fota_download_next},
    {SM_FOTA_Write, sm_fota_write_next},

    {SM_Reset_System, NULL},
};
/********************* STATE MACHINE TABLE *****************************/

/********************* STATE MACHINE ITERATOR **************************/
sm_states_t sm_iterator = sm_cmplify_init_wd_pos;
/********************* STATE MACHINE ITERATOR **************************/

/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/

/********************* GENERAL DEFINES *********************************/

/*state of reconnection, first transmission, lost connection, transmission fail */
typedef enum
{
    first_transmission,
    lost_connection,
    transmission_fail,
    transmission_success,
}reconnection_state_t;



typedef enum
{
    /** SRS0*/
    RCM_SRS0,
    RCM_SRS1
}RCM_state_t;

typedef enum
{
    /** SRS0*/
    RCM_POR = RCM_SRS0_POR_MASK,
    RCM_PIN = RCM_SRS0_PIN_MASK,
    RCM_WDOG = RCM_SRS0_WDOG_MASK,
    RCM_LVD = RCM_SRS0_LVD_MASK,
    RCM_LLWU = RCM_SRS0_WAKEUP_MASK
}RCM_SRS0_state_t;
typedef enum
{
    /** SRS1*/
    RCM_SACKERR = RCM_SRS1_SACKERR_MASK,
    RCM_MDM_AP = RCM_SRS1_MDM_AP_MASK,
    RCM_SW = RCM_SRS1_SW_MASK,
    RCM_LOCKUP = RCM_SRS1_LOCKUP_MASK
}RCM_SRS1_state_t;

alarm_flags_t alarm_flags_str;
//alarm_data_t alarm_data_str ;
uint16_t first_data_agr=0;
uint32_t first_value_ptc=0;

reconnection_state_t state_reconnection = first_transmission;

//alarm_SIGNAL_state_t alarm_signal_str = signal_not_alarm_send;
alarm_HUM_state_t alarm_hum_str= hum_not_alarm_send;
alarm_TEMP_state_t alarm_temp_str = temp_not_alarm_send;
alarm_BATERY_state_t alarm_batery_str = batery_not_alarm_send;
alarm_SHT20_state_t alarm_sht20_str = sht20_not_alarm_send;
alarm_HX711_state_t alarm_hx711_str = hx711_not_alarm_send;
//alarm_WHC_state_t alarm_whc_str = whc_not_alarm_send;
alarm_WHC_state_t alarm_whc_change_positive_str = whc_not_alarm_send;
alarm_WHC_state_t alarm_whc_change_negative_str = whc_not_alarm_send;
alarm_WHC_state_t alarm_whc_low_str = whc_not_alarm_send;
alarm_WHC_state_t alarm_whc_high_str = whc_not_alarm_send;

#define DECIMAL_BASE                    (10)
#define FILE_CREATE_TEMP_BUFF_SIZE      (6)

const int16_t days_since_beginning_of_year[12] = {0,31,59,90,120,151,181,212,243,273,304,334};

//antes static inline
static inline void SM_Create_File_Name(const uint8_t* prefix, uint16_t prefix_size,
        uint16_t file_number, const uint8_t* file_extension,
        uint16_t file_extension_size, uint8_t *new_file_name,
        uint16_t *new_file_name_size)
{
    uint8_t tmp_buff[FILE_CREATE_TEMP_BUFF_SIZE] = {0};

    strcat((char*)(new_file_name), (const char*)(prefix));
    (*new_file_name_size) = prefix_size;

    uitoa((file_number), (char*)(tmp_buff), DECIMAL_BASE);
    strcat((char *)(new_file_name), (const char*)(tmp_buff));
    (*new_file_name_size) += strlen((const char*)(tmp_buff));

    strcat((char *)(new_file_name), (const char*)(file_extension));
    (*new_file_name_size) += file_extension_size;
}

static inline int64_t Unix_Timestamp(uint16_t year, uint8_t month, uint8_t day,
        uint8_t hour, uint8_t min, uint8_t sec)
{
    int32_t leap_years = (((int32_t)year-1)-1968)/4 -
            (((int32_t)year-1)-1900)/100 +
            (((int32_t)year-1)-1600)/400;
    int32_t days_since_1970 = ((int32_t)year-1970)*365 +
            leap_years + days_since_beginning_of_year[(int32_t)month-1] +
            (int32_t)day-1;

    if( ((int32_t)month>2) && ((int32_t)year%4==0 && ((int32_t)year%100!=0 || (int32_t)year%400==0)) )
    {
      days_since_1970 += 1; /** +leap day, if year is a leap year */
    }

    return (int32_t)sec + 60 * ((int32_t)min + 60 * ((int32_t)hour + 24*days_since_1970) );
}

void create_file_weight(uint8_t *filename ,uint32_t adc){

//    cm_q03_codes_t retval = Success;
    uint8_t valor_len=0;
    uint8_t buff[msg_cali_size] = {0};
    uint8_t msg_send[msg_cali_size] = {0};
    uint8_t msg_send_cero[msg_cali_size] = {0};
    memset(msg_send_cero, 0x30, sizeof(msg_send_cero));
    memset(buff, '\0', msg_cali_size);
    memset(msg_send, '\0', sizeof(msg_send_cero));

    uitoa(adc, (char *)(buff), DECIMAL_BASE);//pasamos de entero a char
    valor_len = strlen((const char *)(buff));

    strncat((char *)(msg_send), (const char *)(msg_send_cero),(8-valor_len));
    strcat((char *)(msg_send), (const char *)(buff));

    CM_Q03_File_Upload_File((uint8_t*)filename, created_filename_size, (uint8_t*)msg_send, msg_cali_size);

}


 void download_files_adc(uint8_t *filename, uint32_t*data_file){
//    cm_q03_codes_t retval = Success;
    uint8_t buff_download_adc[msg_cali_size] = {0};
    uint16_t buff_download_adc_size = 0;
//    uint32_t data_file=0;

        CM_Q03_File_Download_File((uint8_t*)filename, created_filename_size,
                                  buff_download_adc, &buff_download_adc_size);

            *data_file = atoi((char*)buff_download_adc); //checar esto


}

 void create_CRC(int16_t data_st ,int16_t data_sh, int32_t data_sp, int32_t* CRC){

     *CRC=((((data_st*3)+(data_sp*2))+data_sh)*2);

}

#if (FOTA_ENABLE)
static uint8_t Reset_Control_Module(RCM_state_t rcm_state)
{
    uint8_t dummy_read = 0;

    switch(rcm_state)
    {
    /** SRS1*/
    case RCM_SRS0:
        dummy_read = RCM->SRS0;

        switch(dummy_read)
        {
        case RCM_POR:
            return RCM_POR;
            break;
        case RCM_PIN:
            return RCM_PIN;
            break;
        case RCM_WDOG:
            return RCM_WDOG;
            break;
        case RCM_LVD:
            return RCM_LVD;
            break;
        case RCM_LLWU:
            return RCM_LLWU;
            break;
        default:
            return 0;
        }

        break;
    case RCM_SRS1:
        dummy_read = RCM->SRS1;

        switch(dummy_read)
        {
        case RCM_SACKERR:
            return RCM_SACKERR;
            break;
        case RCM_MDM_AP:
            return RCM_MDM_AP;
            break;
        case RCM_SW:
            return RCM_SW;
            break;
        case RCM_LOCKUP:
            return RCM_LOCKUP;
            break;
        default:
            return 0;
        }

        break;
    default:
        return 0;
    }
    return dummy_read;
}
#endif
/********************* GENERAL DEFINES *********************************/

/********************* CONTEXTS ****************************************/
 const uint8_t connID = 1;
 const uint8_t ctxID = 1;
 const uint8_t ctxID2 = 2;
 const uint8_t sslID = 1;
/********************* CONTEXTS ****************************************/


/********************* SYSTEM  **********************************/
static uint32_t SW_V=495; //swv 495 AGREGAMOS CARACTERISTICAS DE RED proximanente integrar esto a la 485 analogica normal para unir los cambios //485 apready fix valor de crc//475 sera para identificar los analogicos version final
/* Ubidots structure to store all variables */
ubidots_t ubidots_str =
{
        "000000000000000",                      /* IMEI */
        "\0\0\0\0\0\0",                                     /* SWV */
        "beewaze",         /* TOPIC */
        "a17woz8ddp2nss-ats.iot.us-east-2.amazonaws.com",           /* MQTT_HOST */
        8883,                                   /* MQTT_PORT */
//      "BBFF-QCk4YjmxuXnUsadiQMt6PfqV9PKfQC",  /* USER_NAME */
        "Daniel-Prueba",  /* USER_NAME */

        "pap/867730051923062"   /* PAP: Parameter */
};

uint32_t uinx_12am=0;
uint32_t uinx_aux=0;
uint32_t alarm_rtc_actual=0;
uint32_t alarm_rtc_next=0;
uint32_t alarm_rtc_unix_next=0;

/* Structure of buffer to be sent  */
system_buff2tx_t system_buff2tx_str;
/* Structure to store alarms events */
event_flags_t event_flags;
event_time_seconds_str event_time_seconds_str_t;

/** Event times set*/
/** */
unix_t unix_str;
#if (BIDIRECCIONALITY_ENABLE)
/* String structure to identify the values when obtained via bidireccionality*/
parameter_download_json_t parameter_download_json_str =
{
        "\"value\":",
        "\"PCC\":",
        "\"PTH\":",
        "\"PTC\":",
        "\"PTP\":",
        "\"PCF\":",

        //los mios

        "\"PAPE\":",
        "\"PAPP\":",
        "\"PATA\":",
        "\"PATB\":",
        "\"PAHA\":",
        "\"PAHB\":",
        "\"PAPB\":",

}; //debe coinsidir la posicion con la que va
#endif
/* Structure that stores the decimal value of the download parameters */
parameter_download_value_t parameter_download_value_str; //parameter values of alarms
/* Structure to store alarms events */

#if (FOTA_ENABLE)
/* Structure to store events for FOTA*/
fota_t fota_str;
#endif

static uint8_t rsrp[6] = {0};
uint16_t rsrp_size=0;
int16_t rsrp_number=0;

static uint8_t rsrq[6] = {0};
uint16_t rsrq_size=0;
int16_t rsrq_number=0;

uint8_t sysmode[12]= {0};
uint16_t sysmode_size;

uint8_t rssi[6]= {0};
uint16_t rssi_size;

uint8_t  sinr[6]= {0};
uint16_t sinr_size;

static uint8_t signal_qty_cnt = 0;

#define NETWORK_REG_DELAY           (3000000)
#define NET_REG_MAX_RETRIES         (3)
#define NOT_REGISTERED_TO_NET       (0)
#define REGISTERED_TO_NET           (1)
static uint8_t net_reg_retries = 0;
static uint8_t configure_nw_count = 0; // Peko modification
static uint8_t registered = REGISTERED_TO_NET;

uint32_t attemp_count_register=0;

/********************* SYSTEM  **********************************/

interrupt_status_t interrupt_flag = pin_not_interrupted;

bool flag_fota_parameter=false;

/********************* LOCAL FUNCTIONS ****************************/

/********************* LOCAL FUNCTIONS ****************************/

/********************* INITIALIZE WATCH DOG ****************************/
#define WATCH_DOG_TIME      (DogTime_262144ms)
#define CNT_LPU_SLEEP_TIME 60
#define OFFSET_UNIX_24 86400
uint32_t uinx_actual=0;
uint32_t CNT_DOWNLOAD_PARAMETERS=(1440*60);

uint32_t countToTransmit = 0;

next_state_t SM_Initialize_Watch_Dog(void)
{

#if( LPU_ENABLE )
    /** Initialize LPU*/
    LPU_Enable_Power_Modes();
    LPU_Init_Power_Mode(llwu_timer,llwu_not_port);
    LPU_Select_Power_Mode(run_mode, 0);

#endif

    if(first_transmission == state_reconnection)
    {
        /** Preset parameters*/
        parameter_download_value_str.PCC_value = 0;//Parametro calibracion completa para pesos 5 y 10 kg
        parameter_download_value_str.PTH_value = 1;
        parameter_download_value_str.PTC_value = 5;//va a tener 1 dato cada hora o hasta juntar los 14
        parameter_download_value_str.PCF_value= 0;
        parameter_download_value_str.PTP_value= 1; //parametro de veces de descarga de parametro
        //LOS MIOS//
        parameter_download_value_str.PAPE_value=10000;//10000 gramos CAMBIO REPENTINO
        parameter_download_value_str.PAPP_value=120000;//120KG O 120000 g
        parameter_download_value_str.PATA_value=400; //40 GRADOS
        parameter_download_value_str.PATB_value=50; //5 GRADOS
        parameter_download_value_str.PAHA_value=850; // 85% DE HUMEDAD
        parameter_download_value_str.PAHB_value =100; //parametro alarma humedad baja

        parameter_download_value_str.PAPB_value=100; // alarma peso bajo en gramos

//        event_time_seconds_str_t.LPU_cycle_send=parameter_download_value_str.PTT_value*60;
//        event_time_seconds_str_t.LPU_cycle_read=parameter_download_value_str.PTC_value/CNT_LPU_SLEEP_TIME;
//        event_time_seconds_str_t.LPU_sleep_time = CNT_LPU_SLEEP_TIME; //para que se mantenga siempre en cada  min

        event_time_seconds_str_t.LPU_sleep_time=parameter_download_value_str.PTC_value*60; //sec

        countToTransmit =(60/parameter_download_value_str.PTC_value)*parameter_download_value_str.PTH_value; //mio

//        alarm_rtc_unix_next=1440;
//                alarm_rtc_unix_next=1440*60; //adaptado a segundos
                alarm_rtc_unix_next = (parameter_download_value_str.PTH_value*3600) + parameter_download_value_str.PTC_value*60;//10 minutos extra
        uitoa(SW_V, (char *)(ubidots_str.swv), DECIMAL_BASE);

        event_flags.event_alarm_flag=false;
    }

    /** GPIO Initialization*/
//  GPIO_Initialization(Port_C4, Port_Input);
//  GPIO_InterruptEnable(Port_C4, FallingEdge_interrupt);
//
//    Watchdog_initModuleConfiguration();
//    Watchdog_setDifferentTimer(WATCH_DOG_TIME);
//    Watchdog_dogAlert();

    /** RTC Initialization*/
    RTC_Initialization();

//    //printf("\n\nWatchdog");
//    Watchdog_feedTheDog(normalFood);
//
//

    /** GOTO: */
    return next_state_0;
}
/********************* INITIALIZE WATCH DOG ****************************/
#define FOTA_DOWNLOAD_MAX_RETRIES         (2)
#if (FOTA_ENABLE)
static uint8_t fota_download_retries = 0;

uint8_t status_reset[2]={0};
uint8_t status_reset_size=0;

#endif
bool reset_fota=true;
/********************* FOTA  **********************************/
next_state_t SM_FOTA(void)
{
#if (FOTA_ENABLE)
    RCM_SRS0_state_t retval_SRS0 = 0;
    RCM_SRS1_state_t retval_SRS1 = 0;
    retval_SRS0 = Reset_Control_Module(RCM_SRS0);
    retval_SRS1 = Reset_Control_Module(RCM_SRS1);


    fota_str.b_FOTA_first_download_flag = false;
    fota_str.b_FOTA_first_write_file_flag = false;
    fota_str.b_FOTA_download_flag = false;
    fota_str.b_FOTA_write_file_flag = false;

//  retval_SRS0=64;
//  retval_SRS1=0;


    if( (0 != retval_SRS0) || (0 != retval_SRS1) )
    {
        /** If the rst button was pressed*/
        if( RCM_PIN == retval_SRS0 )
        {
//            flag_fota_parameter=false;
//            fota_str.b_FOTA_first_download_flag = true;
//            fota_str.b_FOTA_first_write_file_flag = true;
        }
        /** If the rst happened after a fota restart*/
        if( (RCM_SW == retval_SRS1) || (RCM_WDOG == retval_SRS0) || (RCM_LLWU == retval_SRS0))
        {
            fota_str.b_FOTA_first_download_flag = false;
            fota_str.b_FOTA_first_write_file_flag = false;
            if( (RCM_SW == retval_SRS1) ){
                reset_fota=false;
            }
        }

    }

    if((RCM_SW == retval_SRS1)&&(RCM_WDOG == retval_SRS0)){
        status_reset[0]='W';
        status_reset[1]='F';
        status_reset_size=2;
    }
    else if(RCM_WDOG == retval_SRS0){
        status_reset[0]='W';
        status_reset_size=1;
    }
    else if( (RCM_SW == retval_SRS1) ){
        status_reset[0]='F';
        status_reset_size=1;
    }

#endif
    //printf("\n\ninitfota");

    Watchdog_feedTheDog(normalFood);
    /** GOTO: SM_Initialize_Peripherals*/
    return next_state_0;
}
/********************* FOTA  **********************************/

/********************* FOTA DOWNLOAD  **********************************/
next_state_t SM_FOTA_Download(void)
{
#if (FOTA_ENABLE)
    fota_codes_t retval = FOTA_Download_Success;
    //printf("\n\nfotadow");

    if((true == fota_str.b_FOTA_download_flag) || (true == fota_str.b_FOTA_first_download_flag))
    {
        retval = FOTA_Download_File();

        if(FOTA_Download_Success == retval)
        {
            fota_download_retries = 0;

                Watchdog_feedTheDog(normalFood);
                /** GOTO: SM_FOTA_Write*/
                return next_state_0;
        }
        else if(FOTA_DOWNLOAD_MAX_RETRIES < fota_download_retries)
        {
            fota_download_retries = 0;

            if(flag_fota_parameter){
                Watchdog_feedTheDog(normalFood);
                /** GOTO: SM_FOTA_Write*/
                return next_state_3;
            }
            else{


           Watchdog_feedTheDog(normalFood);

                FOTA_Software_Reset();

        //        reset_fota=false;
        //        fota_str.b_FOTA_first_download_flag = false;
        //        fota_str.b_FOTA_first_write_file_flag = false;

                /** GOTO: SM_FOTA_Write*/
                return next_state_2;
            }
        }
        else
        {
            fota_download_retries++;

            Watchdog_feedTheDog(normalFood);
            /** GOTO: SM_FOTA_Download*/
            return next_state_1;
        }
    }

#endif
    if(flag_fota_parameter){
        Watchdog_feedTheDog(normalFood);
        /** GOTO: SM_FOTA_Write*/
        return next_state_3;
    }
    else{

                Watchdog_feedTheDog(normalFood);

        FOTA_Software_Reset();

//        reset_fota=false;
//        fota_str.b_FOTA_first_download_flag = false;
//        fota_str.b_FOTA_first_write_file_flag = false;

        /** GOTO: SM_FOTA_Write*/
        return next_state_2;
    }
}
/********************* FOTA DOWNLOAD  **********************************/
/********************* FOTA WRITE  **********************************/
next_state_t SM_FOTA_Write(void)
{

    //printf("\n\nfotawrite");

#if (FOTA_ENABLE)
    fota_codes_t retval_f = FOTA_Write_File_Success;

    retval_f = FOTA_Write_File_to_Memory(SW_V); 

    if(FOTA_Write_File_Success == retval_f)
    {

        Watchdog_feedTheDog(normalFood);

        /** Reset device*/
        FOTA_Software_Reset();

        /** GOTO: SM_Reset_System*/
        return next_state_0;
    }

#endif
    if(flag_fota_parameter){
        Watchdog_feedTheDog(normalFood);
        /** GOTO: SM_FOTA_Write*/
        return next_state_1;
    }
    else{


         Watchdog_feedTheDog(normalFood);

        FOTA_Software_Reset();

//        reset_fota=false;
//        fota_str.b_FOTA_first_download_flag = false;
//        fota_str.b_FOTA_first_write_file_flag = false;

        /** GOTO: SM_FOTA_Write*/
        return next_state_0;
    }
}
/********************* FOTA WRITE  **********************************/





/********************* TURN ON CM Q03  **********************************/
volatile uint32_t count_time_on_cmq0=0;
bool cm_q03_mode=0;
bool cm_q03_state=false;
uint8_t cm_q03_attempt =0;
//uint32_t count_time_on_cmq03=0;
//uint32_t count_time_on_cmq03_start=0;

next_state_t SM_Turn_On_CM_Q03(void)
{
    cm_q03_codes_t retval = Success;

    //printf("\n\nCQ03_pre_on");

    LPU_Select_Power_Mode(run_mode, 60);

    RTC_Reset_Alarm_Seconds();

    retval = CM_Q03_Control(cm_q03_turn_on);
    Delay(3000000);
    //printf("\n\nCQ03_pre_next_on");

    if(CM_Q03_is_ready == retval)
    {

        //printf("\n\nCQ03_on_ready");

        cm_q03_attempt=0;
        cm_q03_state=true;
        Watchdog_feedTheDog(normalFood);
        return next_state_0;
    }
    else if(cm_q03_attempt >= 3){
        //printf("\n\nCQ03_attempt3");

        cm_q03_attempt=0;
        cm_q03_state=false;

        system_buff2tx_str.sample_counter=0;
//        alarm_agr_str=agr_not_alarm_send;
//        alarm_agb_str=agb_not_alarm_send;
        event_flags.event_alarm_flag=0;
        event_time_seconds_str_t.sleep_cnt_send = 0;
        event_time_seconds_str_t.sleep_cnt_read = 0;
//        GPIO_Write(BUZ,1);

        Watchdog_feedTheDog(normalFood);
        return next_state_2;
    }
    else
    {
        //printf("\n\nCQ03_++");

        cm_q03_attempt++;
        return next_state_1;
    }
}
/********************* TURN ON CM Q03 **********************************/


/********************* GET IMEI ****************************************/
#define IMEI_SIZE       (15)

next_state_t SM_Get_IMEI(void)
{
    cm_q03_codes_t retval = Success;
//    uint8_t count_imei=0;
    retval = CM_Q03_Get_IMEI(ubidots_str.imei);
    //printf("\n\nCQ03_IMEI");

    if(Success == retval)
    {
//        do{
//            ubidots_str.topic[14+count_imei] = ubidots_str.imei[count_imei];
//            count_imei++;
//        }while(IMEI_SIZE>count_imei);

        if(first_transmission == state_reconnection){
            ubidots_str.top_pap[4]= ubidots_str.imei[0];
            ubidots_str.top_pap[5]= ubidots_str.imei[1];
            ubidots_str.top_pap[6]= ubidots_str.imei[2];
            ubidots_str.top_pap[7]= ubidots_str.imei[3];
            ubidots_str.top_pap[8]= ubidots_str.imei[4];
            ubidots_str.top_pap[9]= ubidots_str.imei[5];
            ubidots_str.top_pap[10]= ubidots_str.imei[6];
            ubidots_str.top_pap[11]= ubidots_str.imei[7];
            ubidots_str.top_pap[12]= ubidots_str.imei[8];
            ubidots_str.top_pap[13]= ubidots_str.imei[9];
            ubidots_str.top_pap[14]= ubidots_str.imei[10];
            ubidots_str.top_pap[15]= ubidots_str.imei[11];
            ubidots_str.top_pap[16]= ubidots_str.imei[12];
            ubidots_str.top_pap[17]= ubidots_str.imei[13];
            ubidots_str.top_pap[18]= ubidots_str.imei[14];


        }
        Watchdog_feedTheDog(normalFood);
        return next_state_0;
    }

    else
    {
        return next_state_1;
    }
}
/********************* GET IMEI ****************************************/



/********************* SET APN *****************************************/
#define APN_FAIL_DELAY          (100000)

#define APN_PROTOCOL_VERSION    (protocol_IPv4v6)//aqui va el ipv4v6?
#define APN_AUTHENTICATION      (no_authentication)

uint8_t APN[30] = {0};
//static uint8_t APN2[] = "iot.altan.test";
uint32_t plmn = 0;
uint8_t iccid[21] = {0};

next_state_t SM_Set_APN(void)
{
    cm_q03_codes_t retval = Success;
    uint8_t temp_buff[7] = {0};
    uint8_t APN[30] = {0};
    uint32_t sim_identifier = 0;
    uint16_t iccid_size = 0;


    memset(temp_buff, 0, sizeof(temp_buff));
    memset(iccid, 0, sizeof(iccid));
    memset(APN, 0, sizeof(APN));


  /** Variable for the APN configuration value*/
  tcp_ip_apn_parameters_struct_t apn_parameters = {
          ctxID,                  /** Context ID of 1*/
          APN_PROTOCOL_VERSION,       /** IP version 4 is used*/
          APN_AUTHENTICATION  /** No authentication is required*/
  };

  // Peko modification START
  //printf("\n\nCQ03_APN_ICCID");

 retval = CM_Q03_Get_SIM_ICCID(iccid, &iccid_size);
  if (retval != Success)
  {
      Delay(APN_FAIL_DELAY);
      return next_state_1;
  }

  temp_buff[0] = iccid[2];
  temp_buff[1] = iccid[3];
  temp_buff[2] = iccid[4];
  temp_buff[3] = iccid[5];
  temp_buff[4] = iccid[6];

  sim_identifier = (uint32_t)atoi((char*)temp_buff); //modificacion de (char*)
  strcpy((char *)(APN), (const char *)("bam.clarochile.cl")); //APN de chile
  plmn = 0;

//  switch (sim_identifier) //dummy para programa final, descomentar despues
//  {
//      case 52020:
//          strcpy((char *)(APN), (const char *)("internet.itelcel.com"));
//          plmn = 334020;
//          break;
//      case 52070:
//          strcpy((char *)(APN), (const char *)("ott.iot.attmex.mx"));
//          plmn = 334050;
//          break;
//      case 88228:
//          strcpy((char *)(APN), (const char *)("iot.1nce.net"));
//          plmn = 0;//334020; //superdummy
//          break;
//      case 43010:
//            strcpy((char *)(APN), (const char *)("linksnet"));
//            plmn = 334020;
//            break;
//      case 52140: // Agrega APN altan
//          strcpy((char *)(APN), (const char *)("iot.altan.vasanta")); //aqui va el de vasanta?
//          plmn = 334140;
//          break;
//      default: //CASO DE SIM DE CHILE
//          strcpy((char *)(APN), (const char *)("bam.clarochile.cl")); //aqui va el de vasanta?
//          plmn = 0;
//          break;
//  }



  retval = CM_Q03_TCP_IP_Config_Context_APN(APN, sizeof(APN) -1, NULL, 0,
          NULL, 0, apn_parameters);

  //printf("\n\nCQ03_APN_contx");

//  tcp_ip_apn_parameters_struct_t apn_parameters2 = {
//          ctxID2,                  /** Context ID of 1*/
//          APN_PROTOCOL_VERSION,       /** IP version 4 is used*/
//          APN_AUTHENTICATION  /** No authentication is required*/
//  };
//
//  retval = CM_Q03_TCP_IP_Config_Context_APN(APN2, sizeof(APN2) -1, NULL, 0,
//          NULL, 0, apn_parameters2);

    if(Success == retval)
    {

        Watchdog_feedTheDog(normalFood);
        return next_state_0;
    }

    else
    {
        //printf("\n\nCQ03_APN_retry");

        Delay(APN_FAIL_DELAY);
        return next_state_1;
    }

}
/********************* SET APN *****************************************/



/********************* CONFIGURE NETWORK *******************************/

#define NETWORK_SCAN_SEQUENCY   (CatM1_GSM_CatNB1)
#define NETWORK_SCAN_MODE       (scan_automatic)
#define IOT_OPTION_MODE         (scan_CatM1_for_LTE)
#define ROAMING_MODE            (enable_roam_service)
#define NETWORK_CONFIG_OP       (Personal_Configuration)
#define FREQUENCY_BANDS         (EG915LA)

next_state_t SM_Configure_Network(void)
{
    cm_q03_codes_t retval = Success;

    general_network_paramenters_struct_t net_params =
    {
            NETWORK_SCAN_SEQUENCY,
            NETWORK_SCAN_MODE,
            IOT_OPTION_MODE,
            ROAMING_MODE,
            plmn,
            FREQUENCY_BANDS
    };
    CM_Q03_General_Set_Functionality(Airplane_functionality);

//    CM_Q03_General_Config_CEREG();
    // Peko modification
    // Add function to configure RF bands
    //retval = CM_Q03_General_Config_Network(net_params, NETWORK_CONFIG_OP);
    retval = CM_Q03_EG915_Configure_Bands(EG915LA);//QUITADO POR BANDAS //mover dentro de general_config

    //printf("\n\nCQ03_ConfN_bands");

    if ((retval != Success) &&configure_nw_count >= 3) // Peko modification
    {
        //printf("\n\nCQ03_ConfN_count3");

        configure_nw_count = 0;
        registered = NOT_REGISTERED_TO_NET;
        return next_state_2;
    }
    else if (retval != Success)
    {
        //printf("\n\nCQ03_ConfN_bands_++++");

        configure_nw_count++;
        Delay(NETWORK_REG_DELAY);
        return next_state_1;
    }
    CM_Q03_General_Set_Functionality(Full_functionality);

    //retval = CM_Q03_General_Config_Network(net_params, NETWORK_CONFIG_OP);
    //printf("\n\nCQ03_ConfN_netw");
    retval = CM_Q03_General_Set_PLMN(net_params, NETWORK_CONFIG_OP); //configura la conexion a la red si esta una plmn mala, toma la que esta por default en caos de que falle

    if(Success == retval)
    {
        //printf("\n\nCQ03_ConfN_netw_ready");

        Delay(5000000);
        configure_nw_count = 0;
        Watchdog_feedTheDog(normalFood);
        return next_state_0;
    }
    else if (configure_nw_count >= 3) // Peko modification
    {
        //printf("\n\nCQ03_ConfN_netw_3");

        configure_nw_count = 0;
        registered = NOT_REGISTERED_TO_NET;
        return next_state_2;
    }
    else
    {
        //printf("\n\nCQ03_ConfN_netw_++");

        configure_nw_count++;
        Delay(NETWORK_REG_DELAY);
        return next_state_1;
    }
}
/********************* CONFIGURE NETWORK *******************************/


/********************* NETWORK REGISTRATION ****************************/
uint8_t coun_serch=0;
uint16_t rat_size =0;
uint16_t band_size =0;
uint16_t canal_size =0;

uint8_t rat[10]={};
uint8_t plmn_str[8]={};
uint8_t banda_str[15]={};
uint8_t canal_str[5]={};

next_state_t SM_Network_Registration(void)
{
    cm_q03_codes_t retval = Success;
    cm_q03_codes_t special_retval = Success;

    retval = CM_Q03_General_Registration_Status();
    //printf("\n\nCQ03_Reg");

    memset(rat,'\0', sizeof(rat));
    memset(plmn_str,'\0', sizeof(plmn_str));
    memset(banda_str, '\0', sizeof(banda_str));
    memset(canal_str, '\0', sizeof(canal_str));


    attemp_count_register+=1;
    /** If registered or roaming, go to next state*/
    if((Registered == retval)
            || (Registered_Roaming == retval))
    {
        net_reg_retries = 0;
        coun_serch=0;
        //printf("\n\nCQ03_Reg_ready");

        registered = REGISTERED_TO_NET;
        special_retval=CM_Q03_GET_NW_INFO(rat, &rat_size,plmn_str, banda_str, &band_size,canal_str,&canal_size); // funcion para extraer la info de plmn canal y banda
        if(special_retval!= Success){ //si la funcion no es exitosa que ponga valores por default
            memset(plmn_str,'\0', sizeof(plmn_str));
            memset(banda_str, '\0', sizeof(banda_str));
            memset(canal_str, '\0', sizeof(canal_str));
            plmn_str[0]='0';
            banda_str[0]='0';
            canal_str[0]='0';
        }

#if (FOTA_ENABLE)
        /*
         * FOTA check
         */
        if((first_transmission == state_reconnection) && (true == fota_str.b_FOTA_first_download_flag))
        {
            retval = CM_Q03_TCP_IP_Activate_PDP(ctxID);
            Watchdog_feedTheDog(normalFood);
            //printf("\n\nCQ03_Reg_ready_fisrt");

            /** GOTO: SM_FOTA_Download(void)*/
            return next_state_3;
        }
#endif
        //printf("\n\nCQ03_Reg_ready_norm");

        Delay(1000000);
        Watchdog_feedTheDog(normalFood);
        return next_state_0;
    }

    /** When unknown or maximum retries, reset system*/
    else if( (NET_REG_MAX_RETRIES <= net_reg_retries))
    {
        net_reg_retries = 0;
        coun_serch=0;
        //printf("\n\nCQ03_Reg_3");

        registered = NOT_REGISTERED_TO_NET;
        Watchdog_feedTheDog(normalFood);

        return next_state_0;
    }
    /** If searching, wait until is not searching*/
    else if((Searching == retval)|| (Registration_Denied == retval))
    {
        //printf("\n\nCQ03_Reg_serch");

        if(60 <= coun_serch){
            net_reg_retries ++;
            coun_serch=0;
        }

        coun_serch++;
        Delay(NETWORK_REG_DELAY);
        Watchdog_feedTheDog(normalFood);
        return next_state_1;
    }

    else if((Not_Searching == retval)
            ||(Unknown == retval))
    {
        //printf("\n\nCQ03_Reg_not_sert");

        retval = CM_Q03_General_EPS_Registration_Status();
        Delay(5000000);
        retval = CM_Q03_General_EPS_Registration_Status();
        /** If registered or roaming, go to next state*/
        if((Registered == retval)
                || (Registered_Roaming == retval))
        {
            net_reg_retries = 0;
            coun_serch=0;
            //printf("\n\nCQ03_Reg_not_sert_ready");

            registered = REGISTERED_TO_NET;

            special_retval = CM_Q03_GET_NW_INFO(rat, &rat_size, plmn_str, banda_str, &band_size, canal_str, &canal_size); //funcion para extraer la info de plmn canal y banda
            if(special_retval != Success){ //si la funcion no es exitosa que ponga valores por default
                memset(plmn_str,'\0', sizeof(plmn_str));
                memset(banda_str, '\0', sizeof(banda_str));
                memset(canal_str, '\0', sizeof(canal_str));
                plmn_str[0]='0';
                banda_str[0]='0';
                canal_str[0]='0';
            }
#if (FOTA_ENABLE)
        /*
         * FOTA check
         */
        if((first_transmission == state_reconnection) && (true == fota_str.b_FOTA_first_download_flag))
        {
            retval = CM_Q03_TCP_IP_Activate_PDP(ctxID);
            Watchdog_feedTheDog(normalFood);
            //printf("\n\nCQ03_Reg_not_sert_ready_firts");

            /** GOTO: SM_FOTA_Download(void)*/
            return next_state_3;
        }
#endif
        //printf("\n\nCQ03_Reg_not_sert_ready_norm");

            Delay(1000000);
            Watchdog_feedTheDog(normalFood);
            return next_state_0;
        }

        net_reg_retries ++;
        Watchdog_feedTheDog(normalFood);
        return next_state_2;
    }

    /** For timeout or ERROR, increment counter and retry*/
    else
    {
        //printf("\n\nCQ03_Reg_noting");

        net_reg_retries ++;
        Delay(NETWORK_REG_DELAY);
        Watchdog_feedTheDog(normalFood);

        return next_state_1;
    }
}
/********************* NETWORK REGISTRATION ****************************/



/********************* RESET REGISTRATION ******************************/
#define RESET_NETWORK_DELAY         (3000000)

next_state_t SM_Reset_Network(void)
{

    CM_Q03_TCP_IP_Deactivate_PDP(connID); //estas tres lineas se agregaron por skyalert
    Delay(RESET_NETWORK_DELAY);//checar si el comando lo soporta eg915

    CM_Q03_General_Set_Functionality(Minimum_functionality);
    //printf("\n\nCQ03_minimal_funnction");
    Watchdog_feedTheDog(normalFood);

    Delay(RESET_NETWORK_DELAY);
    CM_Q03_General_Set_Functionality(Full_functionality);
    //printf("\n\nCQ03_full_funnction");

    Delay(6000000);

    return next_state_0;
}
/********************* RESET REGISTRATION ******************************/



/********************* GET EPS REG STATUS ******************************/
static uint8_t cel_tec = '0';

next_state_t SM_Get_EPS_Reg_Status(void)
{
    cm_q03_codes_t retval = Success;

    retval = CM_Q03_General_EPS_Registration_Status();
    //printf("\n\nCQ03_general_reg");

    if(Registered == retval)
    {
        cel_tec = '8';
    }

    else
    {
        cel_tec = '0';
    }

    return next_state_0;
}
/********************* GET EPS REG STATUS ******************************/



/********************* GET SIGNAL QUALITY ******************************/
#define SIGNAL_QTY_DELAY    (1000000)
#define MAX_RETRIES         (3)

next_state_t SM_Get_Signal_Quality(void)
{

//    CM_Q03_4G_Signal_Quality_Report( sysmode, &sysmode_size, rssi,
//            &rssi_size, rsrp, &rsrp_size, sinr, &sinr_size,
//            rsrq, &rsrq_size);
    CM_Q03_Signal_Quality_Report(&rsrp, &rsrq);
    //printf("\n\nCQ03_signal_qlaty");

    if(MAX_RETRIES < signal_qty_cnt)
    {
        //printf("\n\nCQ03_signal_qlaty_3");

        signal_qty_cnt = 0;
        return next_state_0;
    }

    else if(99 == rsrp)
    {
        //printf("\n\nCQ03_signal_qlaty++");

        signal_qty_cnt ++;
        Delay(SIGNAL_QTY_DELAY);
        return next_state_1;
    }

    else
    {
        //printf("\n\nCQ03_signal_qlaty_ready");

        signal_qty_cnt = 0;
        return next_state_0;
    }

}
/********************* GET SIGNAL QUALITY ******************************/



/********************* CONFIGURE SSL ***********************************/
#define TURN_ON_SSL 1


#define SSL_VERSION         (ssl_version_tls_1_2)
#define SSL_CIPHERSUITE     (all)
#define SSL_AUTHENTICATION  (manage_authentication_if_requested)
#define SSL_VALIDITY_CHECK  (ignore_validity_check)
#define SSL_TIMEOUT         (60)
#define SSL_CONFIG_OP       (Personal_Configuration)

#if (TURN_ON_SSL)
static uint8_t CA_File_Name[]={"aws_ca.pem"};
static uint8_t CC_File_Name[]={"aws_ce.pem.crt"};
static uint8_t CK_File_Name[]={"aws_ck.key"};
#endif

next_state_t SM_Configure_SSL(void)
{
    cm_q03_codes_t retval = Success;
#if (TURN_ON_SSL)
    ssl_config_parameters_struct_t ssl_config =
    {
            SSL_VERSION,
            SSL_CIPHERSUITE,
            SSL_AUTHENTICATION,
            SSL_VALIDITY_CHECK,
            SSL_TIMEOUT
    };

    retval = CM_Q03_SSL_Configuration(sslID, ssl_config, SSL_CONFIG_OP);
    //printf("\n\nCQ03_SSL");

    if(Success != retval)
    {

        //printf("\n\nCQ03_SSL_error");
        FOTA_Software_Reset();
        return next_state_1;
    }

    retval = CM_Q03_SSL_CA_Certificate(sslID, CA_File_Name, (sizeof(CA_File_Name) - 1));
    retval = CM_Q03_SSL_Client_Certificate(sslID, CC_File_Name, (sizeof(CC_File_Name) - 1));
    retval = CM_Q03_SSL_Client_Private_Key(sslID, CK_File_Name, (sizeof(CK_File_Name) - 1));
#endif
    if(Success == retval)
    {
        //printf("\n\nCQ03_SSL_ready");

        Watchdog_feedTheDog(normalFood);
        return next_state_0;
    }

    else
    {
        //printf("\n\nCQ03_SSLerror");
        FOTA_Software_Reset();
        return next_state_1;
    }
}
/********************* CONFIGURE SSL ***********************************/



/********************* CONFIGURE MQTT **********************************/
#define MQTT_VERSION        (MQTT_v3_1)
#define PKT_TIMEOUT         (5)
#define RETRY_TIMES         (3)
#define REPORT_TIMEOUT      (not_report)
#define SESSION_TYPE        (clean_session)
#define KEEPALIVE_TIME      (30)
#define ENABLE_SSL          (ssl_enabled)
#define MQTT_CONFIG_OP      (Personal_Configuration)
static uint16_t msgID = 1;
uint8_t configure_mqtt_count=0;
next_state_t SM_Configure_MQTT(void)
{
    cm_q03_codes_t retval = Success;
    mqtt_config_t mqtt_config =
    {
        MQTT_VERSION,
        ctxID,
        PKT_TIMEOUT,
        RETRY_TIMES,
        REPORT_TIMEOUT,
        SESSION_TYPE,
        KEEPALIVE_TIME,
        ENABLE_SSL,
        sslID
    };

    retval = CM_Q03_MQTT_Configure_Parameters(connID, mqtt_config, MQTT_CONFIG_OP);
    //printf("\n\nCQ03_MQTT");
//    retval=1;

    if(Success == retval)
    {
        //printf("\n\nCQ03_MQTT_Ready");
        configure_mqtt_count=0;
        Watchdog_feedTheDog(normalFood);
        return next_state_0;
    }
    else if(5 < configure_mqtt_count){
        configure_mqtt_count=0;
        Watchdog_feedTheDog(normalFood);
        return next_state_0;
    }
    else
    {
        Delay(5000000);
        configure_mqtt_count++;
        //printf("\n\nCQ03_MQTT_error");
//        GPIO_Write(BUZ,1);

        return next_state_1;
    }
}
/********************* CONFIGURE MQTT **********************************/

/********************* EXTRACT PTRS ************************************/
#define FILE_PTRS_MAX_SIZE      (6)


static uint16_t file_write_ptr = 0;
static uint16_t file_read_ptr = 0;

const uint8_t write_ptr_file[] = "write_ptr_file.txt";
const uint8_t read_ptr_file[] = "read_ptr_file.txt";

next_state_t SM_File_Extract_Ptrs(void)
{
    cm_q03_codes_t retval = Success;
    uint8_t file_buff[FILE_PTRS_MAX_SIZE] = {0};
    uint16_t file_buff_size = 0;


    retval = CM_Q03_Set_Command_Terminator_Character_ATS_Reconnect();


    retval = CM_Q03_File_Download_File((uint8_t*)write_ptr_file, sizeof(write_ptr_file) -1,
            file_buff, &file_buff_size);

    if(Success == retval)
    {    //printf("\n\nCQ03_File_dow_wri");

        file_write_ptr = (uint16_t)atoi((char*)file_buff);
    }

    memset(file_buff, '\0', sizeof(file_buff));

    retval|= CM_Q03_File_Download_File((uint8_t*)read_ptr_file, sizeof(read_ptr_file) -1,
            file_buff, &file_buff_size);

    if(Success == retval)
    {    //printf("\n\nCQ03_File_dow_red");

        file_read_ptr = (uint16_t)atoi((char*)file_buff);
            Watchdog_feedTheDog(normalFood);

    }



    Delay(6000000);
    cm_q03_mode=1;
    if(REGISTERED_TO_NET == registered)
    {
        return next_state_0;
        //printf("\n\nCQ03_File_dow_net_ready");

    }

    else
    {
        if(first_transmission == state_reconnection)
        {
            return next_state_2;
            //printf("\n\nCQ03_File_dow_first_t");

        }
        //printf("\n\nCQ03_File_dow_not_net");

        return next_state_1;
    }
}
/********************* EXTRACT PTRS ************************************/


/******************** HTTP FIRS TRANSMISSION ************************* */
#define HTTP_MAX_RETRIES        (3)

bool http_first_send=true;
uint8_t http_count_send=0;
next_state_t SM_HTTP_First_Transmission(void)
{

#if FIRST_HTTP_ENABLE
    uint8_t url[] = "https://industrial.api.ubidots.com/api/v1.6/devices/867730051852188?type=gas-metering";

    uint8_t hdr[] = {
                        "POST /api/v1.6/devices/867730051852188?type=gas-metering HTTP/1.1\r\nHost: industrial.api.ubidots.com\r\n"
                        "X-Auth-Token: BBFF-umsbNa5Ha9COz6W4KxTNDRcB4ei8KG\r\nContent-Type: application/json\r\n"
                        "Content-Length: 25\r\n\r\n{\"IMEI\": 867730051852188}\r\n"
                     };
    http_config_t http_config_struct;
    cm_q03_codes_t retval;


    /*
     * Add imei of current device into the http protocol
     */
    url[52] = ubidots_str.imei[0];
    url[53] = ubidots_str.imei[1];
    url[54] = ubidots_str.imei[2];
    url[55] = ubidots_str.imei[3];
    url[56] = ubidots_str.imei[4];
    url[57] = ubidots_str.imei[5];
    url[58] = ubidots_str.imei[6];
    url[59] = ubidots_str.imei[7];
    url[60] = ubidots_str.imei[8];
    url[61] = ubidots_str.imei[9];
    url[62] = ubidots_str.imei[10];
    url[63] = ubidots_str.imei[11];
    url[64] = ubidots_str.imei[12];
    url[65] = ubidots_str.imei[13];
    url[66] = ubidots_str.imei[14];



    hdr[23] = ubidots_str.imei[0];
    hdr[24] = ubidots_str.imei[1];
    hdr[25] = ubidots_str.imei[2];
    hdr[26] = ubidots_str.imei[3];
    hdr[27] = ubidots_str.imei[4];
    hdr[28] = ubidots_str.imei[5];
    hdr[29] = ubidots_str.imei[6];
    hdr[30] = ubidots_str.imei[7];
    hdr[31] = ubidots_str.imei[8];
    hdr[32] = ubidots_str.imei[9];
    hdr[33] = ubidots_str.imei[10];
    hdr[34] = ubidots_str.imei[11];
    hdr[35] = ubidots_str.imei[12];
    hdr[36] = ubidots_str.imei[13];
    hdr[37] = ubidots_str.imei[14];

    hdr[215] = ubidots_str.imei[0];
    hdr[216] = ubidots_str.imei[1];
    hdr[217] = ubidots_str.imei[2];
    hdr[218] = ubidots_str.imei[3];
    hdr[219] = ubidots_str.imei[4];
    hdr[220] = ubidots_str.imei[5];
    hdr[221] = ubidots_str.imei[6];
    hdr[222] = ubidots_str.imei[7];
    hdr[223] = ubidots_str.imei[8];
    hdr[224] = ubidots_str.imei[9];
    hdr[225] = ubidots_str.imei[10];
    hdr[226] = ubidots_str.imei[11];
    hdr[227] = ubidots_str.imei[12];
    hdr[228] = ubidots_str.imei[13];
    hdr[229] = ubidots_str.imei[14];



//    http_first_send=false;

    if(http_first_send)
    {

        http_config_struct.context_id = 1;
        http_config_struct.request_header = request_enable;
        http_config_struct.response_header = response_enable;
        http_config_struct.sslctxID = 1;
        http_config_struct.data_type = x_www_form_urlencoded;

        retval = CM_Q03_HTTP_Configure_Parameters(http_config_struct, Personal_Configuration);
        Delay(500);

        uint16_t content_length;
        retval = CM_Q03_HTTP_Set_URL(url, sizeof(url)-1);
        Delay(500);
        retval = CM_Q03_HTTP_Send_POST(hdr, sizeof(hdr)-1, 80, &content_length);
        retval=HTTPS_OK;
        if(HTTPS_OK == retval)
        {
            http_first_send=false;
            http_count_send=0;
            Watchdog_feedTheDog(normalFood);
            return next_state_0;
        }
        else if(HTTP_MAX_RETRIES < http_count_send){
            http_count_send=0;
            Watchdog_feedTheDog(normalFood);
            return next_state_0;
        }
        else{
            http_count_send++;
            Delay(5000000);
            return next_state_1;
        }

    }

#endif
    return next_state_0;

}
/******************** HTTP FIRS TRANSMISSION ************************* */

/********************* OPEN MQTT NETWORK *******************************/
#define MQTT_OPEN_MAX_RETRIES        (2)

static uint8_t mqtt_open_retries = 0;

next_state_t SM_MQTT_Open(void)
{
    cm_q03_codes_t retval = Success;

    retval = CM_Q03_MQTT_Open_Network(connID, ubidots_str.mqtt_host, sizeof(ubidots_str.mqtt_host), ubidots_str.mqtt_port);
    attemp_count_register+=100;
    //printf("\n\nCQ03_MQTT_open");

    /** If network is opened, or network was already opened,
     * go to next state*/
    if((MQTT_Network_Opened_Successfully == retval)
            || (MQTT_Identifier_Occupied == retval))
    {
        //printf("\n\nCQ03_MQTT_readyy");

        registered = REGISTERED_TO_NET;
        mqtt_open_retries = 0;
        Watchdog_feedTheDog(normalFood);

        return next_state_0;
    }

    /** If maximum retries reached, reset the module*/
    else if(MQTT_OPEN_MAX_RETRIES < mqtt_open_retries)
    {
        //printf("\n\nCQ03_MQTT_open_3");

        registered = NOT_REGISTERED_TO_NET;
        mqtt_open_retries = 0;
        Watchdog_feedTheDog(normalFood);

        return next_state_3;
    }

    /** If fails to activate PDP or open network, go to activate PDP*/
    else if(MQTT_Failed_Parse_Domain_Name == retval||MQTT_Failed_Active_PDP == retval)
    {
        //printf("\n\nCQ03_MQTT_dailed_domian");

        mqtt_open_retries ++;
        Watchdog_feedTheDog(normalFood);

        return next_state_2;
    }

    /** For a network disconnection error go to network registration*/
    else if(MQTT_Network_Disconnection_Error == retval)
    {
        //printf("\n\nCQ03_MQTT_Disconect");

        mqtt_open_retries ++;
        Watchdog_feedTheDog(normalFood);

        return next_state_1;
    }

    /** If failed to parse domain name, or wrong parameters,
         cycle this state*/
    else if( (MQTT_Wrong_parameter == retval))
    {
        //printf("\n\nCQ03_MQTT_wrong");

        mqtt_open_retries ++;
        return next_state_4;
    }


    /** For timeout and ERROR, increment retries and try again*/
    else
    {
        //printf("\n\nCQ03_MQTT_error");
        Watchdog_feedTheDog(normalFood);

        mqtt_open_retries ++;
        return next_state_4;
    }
}
/********************* OPEN MQTT NETWORK *******************************/
uint8_t mqtt_conn_state_retries = 0;
uint8_t mqtt_conn_retries = 0;
uint8_t mqtt_count_retries=0;
#define MQTT_CONN_STATE_MAX_RETRIES             (3)

/********************* CONNECT TO MQTT NETWORK *************************/
next_state_t SM_MQTT_Connect(void)
{
    cm_q03_codes_t retval = Success;

    retval = CM_Q03_MQTT_Connect(connID, ubidots_str.imei, IMEI_SIZE, ubidots_str.user_name,
            sizeof(ubidots_str.user_name), NULL, 0);
    attemp_count_register+=10000;
    //printf("\n\nCQ03_MQTT_conects\n");

    /** When connection is accepted, or was already connected,
     * go to next state*/
    if((MQTT_Connection_Accepted == retval) ||
            (MQTT_Connection_Refused_Identifier_Rejected == retval))
    {
        /** If connected to MQTT, resets the retries for
             initial, Timeout and ERROR from MQTT connection
             State*/

        registered = REGISTERED_TO_NET;
        mqtt_conn_retries = 0;
        mqtt_count_retries = 0;

        //printf("\nCQ03_MQTT_conect_ready\n");

        Watchdog_feedTheDog(normalFood);
        if( (event_flags.event_subscribe_flag) || (first_transmission == state_reconnection) )
        {
            /** GOTO: SM_Subscribe_MQTT(void)*/
            return next_state_0;
        }
        else
        {
            /** GOTO: SM_Capture_RTC(void)*/
            return next_state_4;
        }

    }
    else if(MQTT_OPEN_MAX_RETRIES < mqtt_count_retries)
    {
        //printf("\nCQ03_MQTT_conect_open_3\n");

        mqtt_count_retries = 0;
        mqtt_conn_retries = 0;
        registered = NOT_REGISTERED_TO_NET;

        Watchdog_feedTheDog(normalFood);
        return next_state_1;
    }
    else if(MQTT_CONN_STATE_MAX_RETRIES < mqtt_conn_retries)
    {
        //printf("\nCQ03_MQTT_conect_3\n");

        mqtt_conn_retries = 0;
        mqtt_count_retries++;

        Watchdog_feedTheDog(normalFood);
        return next_state_2;
    }

    /** If server unavailable, store the data and sleep the module*/
    else if((MQTT_Connection_Refused_Server_Unavialable == retval) ||
            (MQTT_Connection_Refused_Not_Authorized == retval))
    {
        //printf("\nCQ03_MQTT_conect_refused\n");

        mqtt_count_retries = 0;
        mqtt_conn_retries = 0;
        registered = NOT_REGISTERED_TO_NET;
        Watchdog_feedTheDog(normalFood);
        return next_state_1;
    }

    else if(ERROR == retval)
    {
        //printf("\nCQ03_MQTT_conect_error\n");

        Delay(2000000);
        retval = CM_Q03_MQTT_Connection_Status(connID);
        if(MQTT_connected == retval)
        {
            registered = REGISTERED_TO_NET;
            mqtt_conn_retries = 0;
            mqtt_count_retries = 0;
            //printf("\nCQ03_MQTT_conect_error_ready\n");

            Watchdog_feedTheDog(normalFood);

            if( (event_flags.event_subscribe_flag) || (first_transmission == state_reconnection) )
            {
                /** GOTO: SM_Subscribe_MQTT(void)*/
                return next_state_0;
            }
            else
            {
                /** GOTO: SM_Capture_RTC(void)*/
                return next_state_4;
            }

        }

        mqtt_conn_retries++;
        return next_state_3;
    }

    /** For bad username/password, unacceptable protocol version,
     * or other responses, cycle in this state*/
    else
    {
        //printf("\nCQ03_MQTT_conect_attmempt\n");

        Delay(2000000);
        retval = CM_Q03_MQTT_Connection_Status(connID);
        if(MQTT_connected == retval)
        {
            //printf("\nCQ03_MQTT_conect_attmempt_ready\n");

            registered = REGISTERED_TO_NET;
            mqtt_conn_retries = 0;
            mqtt_count_retries = 0;
            Watchdog_feedTheDog(normalFood);

            if( (event_flags.event_subscribe_flag) || (first_transmission == state_reconnection) )
            {
                /** GOTO: SM_Subscribe_MQTT(void)*/
                return next_state_0;
            }
            else
            {
                /** GOTO: SM_Capture_RTC(void)*/
                return next_state_4;
            }

        }
        mqtt_conn_retries++;
        return next_state_3;
    }
}
/********************* CONNECT TO MQTT NETWORK *************************/

#define MQTT_SUB_MAX_RETRIES   (3)

bool first_download_parameters=true;
bool first_download_control=true;
uint8_t sub_cnt=0;

/********************* Subscribe MQTT *************************/

next_state_t SM_Subscribe_MQTT(void)
{

    //printf("\nCQ03_MQTT_subscribe\n");

    cm_q03_codes_t retval = MQTT_Packet_sent_successfully;
    Delay(1000000);
#if(BIDIRECCIONALITY_ENABLE)
    if(MQTT_SUB_MAX_RETRIES <= sub_cnt)
    {
        sub_cnt = 0;
        first_download_parameters=false;
        return next_state_2;
    }
    else if(false==first_download_parameters){
        return next_state_2;

    }
#else
    sub_cnt = 0;

    /* 10 Min*/
    /* 8 Hrs*/
    parameter_download_value_str.PTC_value = 3600*8; //Ciclos establecidos para la captura de datos [(segundos * minutos_deseados) * horas_deseados];
    parameter_download_value_str.PTT_value = parameter_download_value_str.PTC_value; //Ciclos establecidos para la transmisión de datos [Ciclos_lectura * minutos_deseados];

    /* get this values from subscribe */
    /* get this values from subscribe */
    event_time_seconds_str_t.LPU_cycle_send = parameter_download_value_str.PTT_value;
    /* Download parameters every 720 hours */
    event_time_seconds_str_t.LPU_cycle_download_parameters = (43200); // 30 dias o 720 hrs o 43200 min

    return next_state_2;
#endif

#if(BIDIRECCIONALITY_ENABLE)
    retval = CM_Q03_MQTT_Subscribe(ctxID, 1, ubidots_str.top_pap, sizeof(ubidots_str.top_pap), at_least_once);
    attemp_count_register+=1000000;

#endif
    Watchdog_feedTheDog(normalFood);
    if ((MQTT_Packet_sent_successfully == retval)
            || (MQTT_Packet_retransmission == retval))
    {
        Delay(5000000);
        sub_cnt++;
        return next_state_0;
    }

    sub_cnt++;
    return next_state_1;

}
/********************* Subscribe MQTT *************************/

/********************* Download parameters  *********************************/

uint8_t store_status=0;
uint8_t topic_r[100]={};
uint16_t size_of_topic=0;
uint8_t msg_mqtt[1000]={};
uint16_t size_of_msg=0;

typedef enum{

    PCC_recibe  =     1, //parametro calibracion completa
    PTC_recibe  =     2,
    PTP_recibe  =     4,
    PCF_recibe  =     8,
    PTH_recibe  =     16,
    //INICIO MIOS
    PAPE_recibe  =    32,
    PAPP_recibe  =    64,
    PATA_recibe  =    128,
    PATB_recibe  =    256,
    PAPB_recibe  =    512,
    PAHA_recibe  =    1024,//este ver por humedad
    PAHB_recibe  =    2048,//este ver por humedad

}Parameters_to_recibe;

/***actual **/

uint8_t string_pcea[5]={'0','0','0','0','0'};
uint16_t string_pcea_sizeof=0;


uint32_t alarm_rtc_aux=0;
uint8_t flagfirstdata = 0;

uint32_t seed_unix=0;
int32_t time_zone_unix=0;

#define OFFSET_UNIX 5

next_state_t SM_Download_Parameters (void)
{

    //printf("\nCQ03_MQTT_subscribe_down\n");

    cm_q03_codes_t retval = Success;
    int32_t parameter_aux=0;

#if (BIDIRECCIONALITY_ENABLE)
    retval =CM_Q03_MQTT_Read_Store_Message( connID,  &store_status, &msgID,  topic_r, &size_of_topic,  msg_mqtt,  &size_of_msg);

//    retval =CM_Q03_Compare_short_response( ubidots_str.top_pap, sizeof(ubidots_str.top_pap),topic_r,size_of_topic, 500000);

#endif
    if(Success==retval)
    {
        sub_cnt = 0;

        parameter_download_value_str.pap_value = 0;

#if (BIDIRECCIONALITY_ENABLE)
        CM_Q03_Get_parameter_JSON(msg_mqtt, sizeof(msg_mqtt)-1, parameter_download_json_str.parameter_pap,sizeof(parameter_download_json_str.parameter_pap),'.', &parameter_download_value_str.pap_value);
        parameter_download_value_str.pap_value=65535;
#endif

#if (BIDIRECCIONALITY_ENABLE)

        /** Parametro Tiempos Transmision*/
        if(PCC_recibe & parameter_download_value_str.pap_value){
//            parameter_aux=parameter_download_value_str.PCC_value;//parametro por default
            retval = CM_Q03_Get_parameter_JSON(msg_mqtt, sizeof(msg_mqtt)-1, parameter_download_json_str.parameter_pcc,sizeof(parameter_download_json_str.parameter_pcc),',', &parameter_aux);
            if(parameter_aux>=0 && parameter_aux<=1 && retval == Success){
                parameter_download_value_str.PCC_value=parameter_aux;
            }
        }

        /** Parametro Tiempos de Captura (datos)*/
        if(PTC_recibe & parameter_download_value_str.pap_value){

//            parameter_aux=parameter_download_value_str.PTC_value;
            retval = CM_Q03_Get_parameter_JSON(msg_mqtt, sizeof(msg_mqtt)-1, parameter_download_json_str.parameter_ptc,sizeof(parameter_download_json_str.parameter_ptc),',', &parameter_aux);
            if(parameter_aux>=1 && parameter_aux<=60 && retval == Success){
                parameter_download_value_str.PTC_value=parameter_aux;
            }
        }

        if(PTP_recibe & parameter_download_value_str.pap_value){
//            parameter_aux=parameter_download_value_str.PTP_value;

            retval = CM_Q03_Get_parameter_JSON(msg_mqtt, sizeof(msg_mqtt)-1, parameter_download_json_str.parameter_ptp,sizeof(parameter_download_json_str.parameter_ptp),',', &parameter_aux);
            if(parameter_aux>=1 && parameter_aux<=30 && retval == Success){
                parameter_download_value_str.PTP_value=parameter_aux;
            }
        }


        if(PTH_recibe & parameter_download_value_str.pap_value){
//            parameter_aux=parameter_download_value_str.PTH_value;

            retval = CM_Q03_Get_parameter_JSON(msg_mqtt, sizeof(msg_mqtt)-1, parameter_download_json_str.parameter_pth,sizeof(parameter_download_json_str.parameter_pth),',', &parameter_aux);
            if(parameter_aux>=0 && parameter_aux<=24 && retval == Success){
                    parameter_download_value_str.PTH_value=parameter_aux;
            }
        }

//MIS PARAMETROS
        if(PAPE_recibe & parameter_download_value_str.pap_value){//cambio de peso repentino
//            parameter_aux=parameter_download_value_str.PAPE_value;
            retval = CM_Q03_Get_parameter_JSON(msg_mqtt, sizeof(msg_mqtt)-1, parameter_download_json_str.parameter_pape,sizeof(parameter_download_json_str.parameter_pape),',', &parameter_aux);
            if(parameter_aux>=0 && parameter_aux<=200000 && retval == Success){
                parameter_download_value_str.PAPE_value=parameter_aux;
            }
        }
        if(PAPP_recibe & parameter_download_value_str.pap_value){
//            parameter_aux=parameter_download_value_str.PAPP_value;

            retval = CM_Q03_Get_parameter_JSON(msg_mqtt, sizeof(msg_mqtt)-1, parameter_download_json_str.parameter_papp,sizeof(parameter_download_json_str.parameter_papp),',', &parameter_aux);
            if(parameter_aux>=0 && parameter_aux<=200000 && retval == Success){
                parameter_download_value_str.PAPP_value=parameter_aux;
            }
        }
        if(PATA_recibe & parameter_download_value_str.pap_value){
            retval = CM_Q03_Get_parameter_JSON(msg_mqtt, sizeof(msg_mqtt)-1, parameter_download_json_str.parameter_pata,sizeof(parameter_download_json_str.parameter_pata),',', &parameter_aux);
            if(retval == Success){
                parameter_download_value_str.PATA_value=parameter_aux;
            }
        }
        if(PATB_recibe & parameter_download_value_str.pap_value){
            retval = CM_Q03_Get_parameter_JSON(msg_mqtt, sizeof(msg_mqtt)-1, parameter_download_json_str.parameter_patb,sizeof(parameter_download_json_str.parameter_patb),',', &parameter_aux);
            if(retval == Success){
                parameter_download_value_str.PATB_value=parameter_aux;
            }
        }
        if(PAPB_recibe & parameter_download_value_str.pap_value){
//            parameter_aux=parameter_download_value_str.PAPB_value;

            retval = CM_Q03_Get_parameter_JSON(msg_mqtt, sizeof(msg_mqtt)-1, parameter_download_json_str.parameter_papb,sizeof(parameter_download_json_str.parameter_papb),',', &parameter_aux);
            if(parameter_aux>=0 && parameter_aux<=200000 && retval == Success){
                parameter_download_value_str.PAPB_value=parameter_aux;
            }
        }
        if(PAHA_recibe & parameter_download_value_str.pap_value){
//            parameter_aux=parameter_download_value_str.PAHA_value;

            retval = CM_Q03_Get_parameter_JSON(msg_mqtt, sizeof(msg_mqtt)-1, parameter_download_json_str.parameter_paha,sizeof(parameter_download_json_str.parameter_paha),'}', &parameter_aux);
            if(parameter_aux>=0 && parameter_aux<=1000 && retval == Success){
                parameter_download_value_str.PAHA_value=parameter_aux;
            }
        }
        if(PAHB_recibe & parameter_download_value_str.pap_value){
//            parameter_aux=parameter_download_value_str.PAHB_value;
            retval = CM_Q03_Get_parameter_JSON(msg_mqtt, sizeof(msg_mqtt)-1, parameter_download_json_str.parameter_pahb,sizeof(parameter_download_json_str.parameter_pahb),',', &parameter_aux);
            if(parameter_aux>=0 && parameter_aux<=1000 && retval == Success){
                parameter_download_value_str.PAHB_value=parameter_aux;
            }
        }

        /** Parametro Activacion Fota*/
        if(PCF_recibe & parameter_download_value_str.pap_value){
//            parameter_aux=parameter_download_value_str.PCF_value;
            retval = CM_Q03_Get_parameter_JSON(msg_mqtt, sizeof(msg_mqtt)-1, parameter_download_json_str.parameter_pcf,sizeof(parameter_download_json_str.parameter_pcf),',', &parameter_aux);
            if(parameter_aux>=0 && parameter_aux<=1 && retval == Success){
                parameter_download_value_str.PCF_value=parameter_aux;
            }
            if(!reset_fota){
                parameter_download_value_str.PCF_value=0;
                reset_fota=true;
            }
        }


        first_download_parameters = false;
        event_flags.event_subscribe_flag = false;
//        event_time_seconds_str_t.LPU_cycle_send=parameter_download_value_str.PTT_value*60;
//        event_time_seconds_str_t.LPU_cycle_read=parameter_download_value_str.PTC_value/CNT_LPU_SLEEP_TIME; //parametro inutil?
        event_time_seconds_str_t.LPU_sleep_time = parameter_download_value_str.PTC_value*60;

         countToTransmit =(60/parameter_download_value_str.PTC_value)*parameter_download_value_str.PTH_value; //mio

//        uinx_actual=3600*parameter_download_value_str.PTT_value;
        CNT_DOWNLOAD_PARAMETERS=86400*parameter_download_value_str.PTP_value; //para que este el tiempo en segundos


        if((first_transmission != state_reconnection))
        {
            RTC_Clean_Alarm_Flag();

            RTC_Get_Time_Seconds(&alarm_rtc_actual);
//            uinx_12am=alarm_rtc_actual%OFFSET_UNIX_24;
//            uinx_aux=(alarm_rtc_actual-uinx_12am+time_zone_unix)+uinx_actual+seed_unix;
//
//
//            if((alarm_rtc_actual + 3600) > uinx_aux){
//                alarm_rtc_next= uinx_aux + OFFSET_UNIX_24;
////                alarm_rtc_unix_next= (uint32_t)(((uinx_aux + OFFSET_UNIX_24)-alarm_rtc_actual)/60.0f)+30;
////                alarm_rtc_unix_next= (uint32_t)(((uinx_aux + OFFSET_UNIX_24)-alarm_rtc_actual)+(30*60));//para aplicarlo en segundos
//
//            }
//            else{
//                alarm_rtc_next= uinx_aux;
////                alarm_rtc_unix_next= (uint32_t)(((uinx_aux-alarm_rtc_actual)/60.0f)+30);
////                alarm_rtc_unix_next= (uint32_t)((uinx_aux-alarm_rtc_actual)+(30*60));
//
//            }

            alarm_rtc_next = alarm_rtc_actual + (parameter_download_value_str.PTH_value*3600) + (parameter_download_value_str.PTC_value*60);
            alarm_rtc_unix_next = (parameter_download_value_str.PTH_value*3600) + parameter_download_value_str.PTC_value*60;// 10 min extra
//            alarm_rtc_next=alarm_rtc_actual+121;
            RTC_Set_Alarm_Seconds(alarm_rtc_next);
            RTC_Enable_Interrupt(RTC_Both_interrupt);
        }

#endif

        /*
         * DOWNLOAD DATA CHANGE FORMAT TO CORRECT ONE
         */
#if (BIDIRECCIONALITY_ENABLE)
//        parameter_download_value_str.PTT_value = (parameter_download_value_str.PTT_value - 77) / 10;
#endif
//        RCM_SRS1_state_t retval_SRS1_l = 0;
//
//        retval_SRS1_l = Reset_Control_Module(RCM_SRS1);

        /** If the value obtaines is less than 1027 then we download the latest program*/
        if( parameter_download_value_str.PCF_value)
        {
#if (FOTA_ENABLE)
            /** */
            flag_fota_parameter=true;
            fota_str.b_FOTA_download_flag = true;
            fota_str.b_FOTA_write_file_flag = true;
#endif
            Watchdog_feedTheDog(normalFood);
            /** GOTO: SM_FOTA_Download(void)*/
            return next_state_3;
        }
        else
        {
            Watchdog_feedTheDog(normalFood);
            /** GOTO: SM_Capture_RTC(void)*/
            return next_state_0;
        }

    }
    else
    {
        Watchdog_feedTheDog(normalFood);
        /** GOTO: SM_Subscribe_MQTT(void)*/
        return next_state_1;
    }
}

/********************* Download parameters  *********************************/

/********************* CAPTURE RTC *************************/
#define MAX_CLOCK_TRY 3
uint8_t count_tries = 0;
bool data_format_ready=false;
rtc_datetime_t datetime;

next_state_t SM_Capture_RTC(void)
{
   count_tries++;
    cm_q03_codes_t retval = Success;
    general_codes_t status = initialization_failed;
    uint8_t get_time_try = 0;
    clock_struct_t cm_q03_clock;
    //printf("\nCQ03_RTC\n");

    if(REGISTERED_TO_NET==registered){
        do{
               retval = CM_Q03_Synchronize_Local_Time();
//               retval = CM_Q03_Time_Zone_Reporting(); //solo para eg915
               retval = CM_Q03_Automatic_Time_Zone_Update(); //solo para eg915

            retval = CM_Q03_General_Get_Clock(&cm_q03_clock);
            if(Success == retval)
            {
                datetime.second = cm_q03_clock.seconds;
                datetime.minute = cm_q03_clock.minutes;
                datetime.hour = cm_q03_clock.hour;
                datetime.day = cm_q03_clock.day;
                datetime.month = cm_q03_clock.month;
                datetime.year = cm_q03_clock.year + 2000;
                time_zone_unix=-((cm_q03_clock.time_zone/4.0f)*3600.0f);

                   if(datetime.year > 2000)
                   {
                    status = RTC_Set_Time(&datetime);
                }
                   else
                   {
                    status=ERROR;
                    Delay(10000000);

                }
                   count_tries = 0;
            }
               else
               {

                Delay(10000000);
            }
            get_time_try++;
        }while(successful_task != status && MAX_CLOCK_TRY > get_time_try);

           if(count_tries >= 3)
           {
               count_tries = 0;

           }
           else if(successful_task != status && MAX_CLOCK_TRY <= get_time_try)
           {
               return next_state_2;
           }
           else
           {
               count_tries++;
               Delay(10000000);
           }

        if(first_transmission == state_reconnection && successful_task == status)
        {
            Delay(1000000);

        }

    }

    Watchdog_feedTheDog(normalFood);

    if( event_flags.event_alarm_flag || event_flags.event_send_flag){

            return next_state_1;
    }

    return next_state_0;
}
/********************* CAPTURE RTC *************************/

/********************* INITIALIZE PERIPHERALS **************************/

/********************* LPUART *************************/

#define MAX_LPUART_TIME         (600000) //antes 1 segundo
#define BAUDRATE                (9600)
#define PARITY                  (LPUART_ParityDisabled)
#define HX711_READ_MAX_RETRIES  (3)

#define I2C_BAUDRATE            (100000U) //100000U
#define SHT20_READ_MAX_RETRIES  (3)

//#define LED                     (Port_E1)
#define BUZ                     (Port_E0)
#define BUTTON                  (Port_D5)
#define CUENTA_TRIES_BATERY     (20)

/*********************VARIABLES TEMP/HUM******************/
int16_t temperatura_real =0;
int16_t humedad_real =0;

/*********************VARIABLES PESO**********************/
int32_t weight =0;

///*******************VARIABLE BATERIA**************/
//uint16_t batery_level=0;
uint8_t bit_No_Batery = 0;

/*******************VARIABLE para CRC**************/
int32_t crc_send=0;

/*******************BANDERA PARA CALIBRACION**************/
uint8_t estado_boton=0;
uint8_t contador_cal=0;
//uint8_t bit_calibracion=0;
uint8_t back_to_calibrate=0;
uint8_t read_adc_again=0;
uint8_t cambioPesoChange = 0;


/********************VARIABLE PANEL SOLAR BATERY**********/
uint16_t batery_solar_panel =0;


void Buz_to_sound(uint8_t veces, uint32_t tiempo_delay){

    uint8_t cuenta = 0;

    while(veces>cuenta){

        GPIO_Write(BUZ, 1);
        Delay(tiempo_delay);
        GPIO_Write(BUZ, 0);
        Delay(tiempo_delay);
        cuenta++;

    }

}

void blink_leds(uint8_t op){

uint8_t cuenta_ciclo=0;

      if(op==0){
          // que se puede calibrar 1er aviso
          GPIO_Write(BUZ, 1);

//          while(cuenta_ciclo<3){
////              GPIO_Write(LED, 0);
//              Delay(800000);
////              GPIO_Write(LED, 1);
//              Delay(800000);
//              cuenta_ciclo++;
//          }
          Delay(4800000); //esto se compenza por el while de arriba
//          cuenta_ciclo=0;
          GPIO_Write(BUZ, 0);
//          GPIO_Write(LED, 0);

      }else{

            while(cuenta_ciclo<5){

//                GPIO_Write(LED, 1);

                Buz_to_sound(2,300000);

//                GPIO_Write(LED, 0);
//                GPIO_Write(BUZ, 0);
                Delay(600000);
                cuenta_ciclo++;
            }
            cuenta_ciclo=0;
      }

}

uint8_t check_button(void){

    uint8_t cuenta_auxiliar=0;
    uint8_t cuenta_valida=0;

    while(cuenta_auxiliar<15){
        GPIO_Read(BUTTON, &estado_boton);
        if(estado_boton==1){
            cuenta_valida++;
        }
        Delay(70000);
        cuenta_auxiliar++;
    }
    if(cuenta_valida>=10){
        return 1;
    }
    cuenta_auxiliar=0;
    cuenta_valida=0;
    return 0;
}

bool OneTime_Calibrate = true;
bool set_the_weight =true;

Comunication_pins data_pin={
    Port_C3,//input DT va a RX
    Port_C4//output SCK va a TX // el C4 va abajo
};

next_state_t SM_Initialize_Peripherals(void)
{
    general_codes_t retval;
//    uint8_t cuenta_auxiliar=0;
    uint8_t bit_button=0;
    uint8_t cuenta_batery_fail=0;

    hx711_init(data_pin);

    /** Turn ON VDD control*/
    VDD_Sensors_Control(VDD_on);
    Delay(2000000);
    set_Gain(gain_128);

    /** I2C Initialization*/
    I2C_init_t i2c_config;
    i2c_config.baudRate_Bps=I2C_BAUDRATE;
    retval=I2C_Initialization(I2C_Alt0, i2c_config, Personal_Configuration);
    Delay(500000);


        do{
            cuenta_batery_fail++;
            retval=FuelGauge_Voltage_Control(fuel_gauge_on);
            Delay(100000);

        }while(successful_task!=retval && cuenta_batery_fail<=CUENTA_TRIES_BATERY);

        if(cuenta_batery_fail>=CUENTA_TRIES_BATERY){
            bit_No_Batery = 1;
        }

//     GPIO_Initialization(LED, Port_Output);
     GPIO_Initialization(BUZ, Port_Output);
     GPIO_Initialization(BUTTON, Port_Input); //boton para calibrar

     estado_boton=0;//inicializamos para viable del boton para que este sin presionar en pull down
     cuenta_batery_fail=0;
////////////////////////////////////////////esto se puede omitir para una sola vezz//////
     if(OneTime_Calibrate==true){

         OneTime_Calibrate=false;

         blink_leds(0);//inicio de calibracion
         Delay(2000000);

         do{
             bit_button=check_button();//esto es .8 segundos

//             Delay(200000);
             if(bit_button==1){//va a ser 1 cuando se presione por pulldown
                 back_to_calibrate=1;
             }else{
                 back_to_calibrate=0;
                 contador_cal++;
             }

         }while(back_to_calibrate == 0 && contador_cal<20);


         blink_leds(0);     //fin de calibracion
         bit_button=0;
         estado_boton=0;//volvemos a colocar el cero la variable del boton
     //segunda comprobacion de calibracion para borrar los pesos conocidos

         bit_button=check_button();
         Delay(3000000);
         bit_button=check_button();


         if(bit_button==1 && back_to_calibrate==1 && parameter_download_value_str.PCC_value == 1){ //aqui la segunda variable para el parametro

             read_adc_again=1;

             blink_leds(1);
         }
         bit_button=0;
         estado_boton=0;
}//fin del first transmition para indicar la calibracion

///fin de la calibracion
//     GPIO_Write(LED, 0);
     GPIO_Write(BUZ, 0);
     Delay(2000000);
     contador_cal=0;


    Watchdog_feedTheDog(normalFood);
    /** GOTO: */
    return next_state_0;
}
/********************* INITIALIZE PERIPHERALS **************************/



/********************* READ PERIPHERALS ********************************/

uint8_t read_attempts=0;
//
//uint16_t read_last_vol=0;     //para el voltaje
int32_t read_last_pso=0;
//uint16_t read_last_temp=0;
////uint16_t read_last_hum=0;
//uint8_t count_read_peripheral=0;
uint8_t bit_modulo_desconection_sht20=0;
uint8_t bit_modulo_desconection_hx711=0;

uint32_t adc_ps0=0;
uint32_t adc_ps1=0;
uint32_t adc_ps2=0;
//uint32_t adc_ps3=0;

///********ADC_VARIABLES************/
//
//uint16_t adc_temperature=0;
//float voltaje_temperatura=0.0f;
//float corriente=0.0f;
//float resistencia_sensor=0.0f;


///////////Estados de alarmas normales/////////////
estados_sensors_alarms batery_estado = normal_batery;
estados_sensors_alarms hx711_estado = hx711_conect;
estados_sensors_alarms sht20_estado =sht20_conect;
estados_sensors_alarms alarma_pesos=normal_weight;
estados_sensors_alarms alarma_temperatura =normal_temperature;
estados_sensors_alarms alarma_humedad=normal_humidity;
estados_sensors_alarms signal_status= normal_signal;

next_state_t SM_Read_Peripherals(void)
{
    general_codes_t retval = successful_task;
    cm_q03_codes_t return_value=Success;
//    cm_q03_codes_t return_value2=Success;
    bit_modulo_desconection_sht20=0;
    bit_modulo_desconection_hx711=0;

    uint8_t bit_check_error=0;

    uint8_t file_name0[created_filename_size]="cali_pes0.txt\0";
    uint8_t file_name1[created_filename_size]="cali_pes1.txt\0";
    uint8_t file_name2[created_filename_size]="cali_pes2.txt\0";
    uint8_t data_auxiliar[msg_cali_size]="22222222";


    if(set_the_weight == true){

        set_the_weight = false;

        return_value = CM_Q03_File_Upload_File((uint8_t*)file_name0, created_filename_size, (uint8_t*)data_auxiliar, msg_cali_size);

            if(return_value!=File_File_already_existed || back_to_calibrate==1){//si el archivo no existe

//                    GPIO_Write(LED, 1);
                    Buz_to_sound(1,700000);
                    Delay(10000000);
                    retval=Calibracion_pesos(peso_0, &adc_ps0);
                    Watchdog_feedTheDog(normalFood);
                    if(retval==timeout_reached){
                        hx711_estado = hx711_conection_trouble;
                        bit_check_error=1;
                    }

                    CM_Q03_File_Delete_File((uint8_t*)file_name0, created_filename_size);
                    Delay(1000000);
                    create_file_weight(file_name0 ,adc_ps0);

//                    if(read_adc_again==1 || return_value!=File_File_already_existed){
//                    }

                    if((read_adc_again==1 && back_to_calibrate==1) || return_value!=File_File_already_existed){

//                        GPIO_Write(LED, 0);
                        Buz_to_sound(2,700000);
                        Delay(10000000);
                        retval=Calibracion_pesos(peso_1,&adc_ps1);
                        Watchdog_feedTheDog(normalFood);

//                        GPIO_Write(LED, 1);
                        Buz_to_sound(3,700000);
                        Delay(10000000);
                        retval=Calibracion_pesos(peso_2,&adc_ps2);
                        Watchdog_feedTheDog(normalFood);

                        CM_Q03_File_Delete_File((uint8_t*)file_name1, created_filename_size);
                        Delay(1000000);
                        create_file_weight(file_name1 ,adc_ps1);
                        Delay(1000000);
                        CM_Q03_File_Delete_File((uint8_t*)file_name2, created_filename_size);
                        Delay(1000000);
                        create_file_weight(file_name2 ,adc_ps2);

                        if(retval==timeout_reached){
                            hx711_estado = hx711_conection_trouble;
                            bit_check_error=1;
                        }

//                        GPIO_Write(LED, 0);
                        GPIO_Write(BUZ, 0);
                    }
                        blink_leds(1);


        }

            download_files_adc(file_name0,&adc_ps0);
            download_files_adc(file_name1,&adc_ps1);
            download_files_adc(file_name2,&adc_ps2);


    download_parameters_cali(&adc_ps0, &adc_ps1, &adc_ps2); //pasamos los parametros a la libreria de hx711
    calibration_factor();//realiza la extrapolacion

//    GPIO_Write(LED, 0);
    GPIO_Write(BUZ, 0);
    Delay(2000000);

    back_to_calibrate = 0;//reseteamos banderas
//    bit_calibracion = 0;
    read_adc_again=0;

}//fistr transmition para la lectura de la calibracion


    /*****************READ_WEIGHT*****************/
    retval=Get_Weight(&weight);

    hx711_estado = hx711_conect;

    if(successful_task != retval || bit_check_error ==1){

        hx711_estado = hx711_conection_trouble;
        weight=0;
        bit_modulo_desconection_hx711=1;
    }
//    if(successful_task == retval){
//        hx711_estado = hx711_conect;
//    }
//    bit_check_error=0;
 /****************READ TEMP/HUM****************/
        retval=SHT20Read(temperature, &temperatura_real);
        retval=SHT20Read(humidity, &humedad_real);

        if(successful_task != retval){

            if(retval==I2C_NAK || retval==I2C_addr_NAK){

                sht20_estado = sht20_disconect;

            }else{

                sht20_estado = sht20_conection_trouble;
            }
                humedad_real=0;
                temperatura_real=0;
                bit_modulo_desconection_sht20=1;

         }else{

             if(temperatura_real>= parameter_download_value_str.PATA_value){
                 alarma_temperatura=high_temperature;
             }
             if(temperatura_real<= parameter_download_value_str.PATB_value){
                 alarma_temperatura=low_temperature;
             }

             if(humedad_real>= parameter_download_value_str.PAHA_value){
                 alarma_humedad=high_humidity;
             }
             if(humedad_real<= parameter_download_value_str.PAHB_value){
                 alarma_humedad=low_humidity;
             }

         }

            system_buff2tx_str.peso_buff[system_buff2tx_str.sample_counter] = weight;
            system_buff2tx_str.temp_buff[system_buff2tx_str.sample_counter] = temperatura_real;
            system_buff2tx_str.hum_buff[system_buff2tx_str.sample_counter] = humedad_real;

    if(successful_task == retval && bit_modulo_desconection_hx711==0)//aqui viene lo del buff para cada sensor
    {

        read_attempts = 0;
        /** return to next state*/
        Watchdog_feedTheDog(normalFood);
        return next_state_0;
    }
    else{
        if(read_attempts >= 1){

            read_attempts = 0;
            Watchdog_feedTheDog(normalFood);
            return next_state_0;
        }

        read_attempts++;
        Delay(100000);
        /** return to read again*/
        Watchdog_feedTheDog(normalFood);
        return next_state_1;
    }
}

/********************* READ PERIPHERALS ********************************/


uint32_t timestamp_alarm=0;
uint32_t voltage_value = 0;
uint8_t porcentaje_voltaje = 0;
//uint32_t low_batery_value=2900;
#define MAX_VOLTAGE         (3920)
#define MIN_VOLTAGE         (3410)
#define NONE_VOLTAGE        (3200)
#define READ_ATTEMPTS_G     (3)

/********************* READ VOLTAGE ********************************/
next_state_t SM_Read_Voltage(void)
{

        general_codes_t retval = successful_task;
        Delay(5000);
        retval = FuelGauge_ReadVoltage(&voltage_value);

        if(successful_task != retval || bit_No_Batery == 1){

                batery_estado = batery_read_trouble;
                porcentaje_voltaje = 0;
                voltage_value=0;
        }else{

            batery_estado = normal_batery;

            if(voltage_value <=MIN_VOLTAGE){
                batery_estado = low_batery;
            }
            if(voltage_value>=MAX_VOLTAGE){
                porcentaje_voltaje = 100;
            }else if(voltage_value < NONE_VOLTAGE){
                porcentaje_voltaje = 1;
            }
            else{
                porcentaje_voltaje = ((voltage_value-NONE_VOLTAGE)*100)/(MAX_VOLTAGE-NONE_VOLTAGE);
            }
        }


    if(successful_task == retval && voltage_value!=0)
    {

        system_buff2tx_str.voltage_buff[system_buff2tx_str.sample_counter] = porcentaje_voltaje;

//        read_last_vol=porcentaje_voltaje;
//        /** rst read_attempts*/
        read_attempts = 0;
        bit_No_Batery = 0;
        /** return to next state*/
        Watchdog_feedTheDog(normalFood);
        return next_state_0;

    }
    else
    {
        if(read_attempts >= READ_ATTEMPTS_G)
        {

            /** return to rst*/
            read_attempts = 0;
            bit_No_Batery = 0;
            Watchdog_feedTheDog(normalFood);
            return next_state_0;
        }
        read_attempts++;
        Delay(300000);
        /** return to read again*/
        return next_state_1;
    }


}
/********************* READ VOLTAGE ********************************/


/********************* GET TIMESTAMP ***********************************/
#define MAX_GET_TIMESTAMP_CNT       (3)

next_state_t SM_Get_Timestamp(void)
{

    //printf("\nGETtimestamp\n");

    RTC_Get_Time_Seconds(&unix_str.rtc_seconds);

    unix_str.unix_timestamp = unix_str.rtc_seconds * 1000;

        Watchdog_feedTheDog(normalFood);
        return next_state_0;


}
/********************* GET TIMESTAMP ***********************************/


/********************* READ PARAMETERS ********************************/

next_state_t SM_Read_Parameters(void)
{

    Watchdog_feedTheDog(normalFood);
    return next_state_0;
}

/********************* READ PARAMETERS ********************************/


/********************* DEINITIALIZE PERIPHERALS ************************/
next_state_t SM_Deinitialize_Peripherals(void)
{

    FuelGauge_Voltage_Control(fuel_gauge_off);

//    GPIO_Write(LED, 0);
//    GPIO_Write(BUZ, 0);
    // Turn OFF VDD control
    VDD_Sensors_Control(VDD_off);

    /** I2C Deinitialization*/
    I2C_Deinitialization(I2C_Alt0);


    Watchdog_feedTheDog(normalFood);
    return next_state_0;
}
/********************* DEINITIALIZE PERIPHERALS ************************/

/********************* Alarm event *********************************/

//uint8_t bit_active_alarm=0;

next_state_t SM_Alarm_Event(void)
{
//    int32_t peso_anterior=0;
//    int32_t peso_anterior2=0;
    int32_t peso_actual=0;

    peso_actual = system_buff2tx_str.peso_buff[system_buff2tx_str.sample_counter];
//    peso_anterior = system_buff2tx_str.peso_buff[(system_buff2tx_str.sample_counter)-1];
//    peso_anterior2 = system_buff2tx_str.peso_buff[(system_buff2tx_str.sample_counter)-2];

    if(first_transmission == state_reconnection && cm_q03_state==true)
    {
        event_flags.event_send_flag =true;

        RTC_Clean_Alarm_Flag();

//        uinx_actual=3600*parameter_download_value_str.PTT_value;

//        seed_unix=(datetime.minute*60)+datetime.second;
        RTC_Get_Time_Seconds(&alarm_rtc_actual);
//        uinx_12am=alarm_rtc_actual%OFFSET_UNIX_24;
//        uinx_aux=(alarm_rtc_actual-uinx_12am+time_zone_unix)+uinx_actual+seed_unix;


//        if((alarm_rtc_actual + 3600) > uinx_aux){
//            alarm_rtc_next= uinx_aux + OFFSET_UNIX_24;
////            alarm_rtc_unix_next=(uint32_t)((((uinx_aux + OFFSET_UNIX_24)-alarm_rtc_actual)/60.0f)+30);
////            alarm_rtc_unix_next=(uint32_t)(((uinx_aux + OFFSET_UNIX_24)-alarm_rtc_actual)+(30*60));//caso para pasarlo a segundos + el offset
//
//        }
//        else{
//            alarm_rtc_next= uinx_aux;
////            alarm_rtc_unix_next= (uint32_t)(((uinx_aux-alarm_rtc_actual)/60.0f)+30);//modo normal esta en minutos
////            alarm_rtc_unix_next= (uint32_t)((uinx_aux-alarm_rtc_actual)+(30*60));//caso para usar en segundos
//        }
        alarm_rtc_next = alarm_rtc_actual + (parameter_download_value_str.PTH_value*3600) + (parameter_download_value_str.PTC_value*60);
        alarm_rtc_unix_next=(parameter_download_value_str.PTH_value*3600) + parameter_download_value_str.PTC_value*60;//sec
//        alarm_rtc_next=alarm_rtc_actual+121;
//        alarm_rtc_unix_next=alarm_rtc_actual+202;

        RTC_Set_Alarm_Seconds(alarm_rtc_next);
        RTC_Enable_Interrupt(RTC_Both_interrupt);

        Watchdog_feedTheDog(normalFood);
        read_last_pso=weight;
        /** GOTO: SM_Data_Format(void)*/
        return next_state_1;
    }

    else {

            system_buff2tx_str.timestamp_tch_buff[system_buff2tx_str.sample_counter] =unix_str.rtc_seconds; //por que esto aqui
            ///////////////ALARMA BATERIA///////////////
            if(batery_estado != normal_batery){
                if(alarm_batery_str == batery_not_alarm_send || alarm_batery_str == batery_first_alarm_send){
                    event_flags.event_alarm_flag = 1;
                    alarm_batery_str++;
                }
                alarm_flags_str.ABAT_bit = true;

            }else{
                alarm_flags_str.ABAT_bit = false;
                alarm_batery_str = batery_not_alarm_send;
            }
            ///////////////////////////////

//            ((peso_actual > (peso_anterior + (parameter_download_value_str.PAPE_value*1000)))
//                                || (peso_actual > (peso_anterior2 + (parameter_download_value_str.PAPE_value*1000)))
//                                || (peso_actual > (read_last_pso + (parameter_download_value_str.PAPE_value*1000))))
//                                    || ((peso_actual < (peso_anterior - (parameter_download_value_str.PAPE_value*1000)))
//                                        || (peso_actual < (peso_anterior2 -(parameter_download_value_str.PAPE_value*1000)))
//                                        ||(peso_actual < (read_last_pso - (parameter_download_value_str.PAPE_value*1000))) )

if(bit_modulo_desconection_hx711==0){

            if(peso_actual > (read_last_pso + (parameter_download_value_str.PAPE_value))){
                alarma_pesos=fast_weight_change_p;
            }

            if(peso_actual <= parameter_download_value_str.PAPB_value){//supero el peso bajo minimo que puede cargar
                alarma_pesos=low_weight;
            }

            if(peso_actual < (read_last_pso - (parameter_download_value_str.PAPE_value))){
                alarma_pesos=fast_weight_change_n;
            }
//|| (peso_actual < 0 && alarma_pesos == normal_weight) otra condicion que puse en esto de peso bajo pero no recuerdo por que

            if(peso_actual >= parameter_download_value_str.PAPP_value){//supero el peso maximo que puede cargar
                alarma_pesos=over_weight;
            }

}

//bit_modulo_desconection_hx711 = 0;
//##################################fin de lo QUE PASAMOS PARA ARRIBA###########################

            if(alarma_pesos== fast_weight_change_p){

                if(alarm_whc_change_positive_str == whc_not_alarm_send || alarm_whc_change_positive_str == whc_first_alarm_send){
                    event_flags.event_alarm_flag = 1;
                    alarm_whc_change_positive_str++;
                }
                alarm_flags_str.WEG_bit_positive_change = true;

            }else{
                alarm_whc_change_positive_str = whc_not_alarm_send;
                alarm_flags_str.WEG_bit_positive_change = false; //nuevo bit falta
            }


            if(alarma_pesos == fast_weight_change_n){

                if(alarm_whc_change_negative_str == whc_not_alarm_send || alarm_whc_change_negative_str == whc_first_alarm_send){
                    event_flags.event_alarm_flag = 1;
                    alarm_whc_change_negative_str++;
                }
                alarm_flags_str.WEG_bit_negative_change = true;

            }else{
                alarm_whc_change_negative_str = whc_not_alarm_send;
                alarm_flags_str.WEG_bit_negative_change = false;
            }


            if(alarma_pesos==over_weight){

                if(alarm_whc_high_str == whc_not_alarm_send || alarm_whc_high_str == whc_first_alarm_send){
                    event_flags.event_alarm_flag = 1;
                    alarm_whc_high_str++;
                }
                alarm_flags_str.WEG_bit_high = true;

            }else{
                alarm_whc_high_str = whc_not_alarm_send;
                alarm_flags_str.WEG_bit_high = false;
            }


            if(alarma_pesos == low_weight){

                if(alarm_whc_low_str == whc_not_alarm_send || alarm_whc_low_str == whc_first_alarm_send){
                    event_flags.event_alarm_flag = 1;
                    alarm_whc_low_str++;
                }
                alarm_flags_str.WEG_bit_low = true;

            }else{
                alarm_whc_low_str = whc_not_alarm_send;
                alarm_flags_str.WEG_bit_low = false;
            }



            ///////ALRMA TEMPERATURA//////////////////////
            if(alarma_temperatura !=normal_temperature){
                if(alarm_temp_str == temp_not_alarm_send || alarm_temp_str == temp_first_alarm_send){
                    event_flags.event_alarm_flag = 1;
                    alarm_temp_str++;
                }
                alarm_flags_str.ATEM_bit = true;

            }else{
                alarm_temp_str = temp_not_alarm_send;
                alarm_flags_str.ATEM_bit = false;
            }
            //////////////////////////////

            /////ALARMA HUMEDAD//////
            if(alarma_humedad !=normal_humidity){
                if(alarm_hum_str == hum_not_alarm_send || alarm_hum_str == hum_first_alarm_send){
                    event_flags.event_alarm_flag = 1;
                    alarm_hum_str++;
                }
                alarm_flags_str.AHUM_bit = true;

            }else{
                alarm_hum_str = hum_not_alarm_send;
                alarm_flags_str.AHUM_bit = false;
            }
            ////////////

            //////ALARMA SHT20//////////////
            if(sht20_estado != sht20_conect){
                if(alarm_sht20_str == sht20_not_alarm_send || alarm_sht20_str == sht20_first_alarm_send){
                    event_flags.event_alarm_flag = 1;
                    alarm_sht20_str++;
                }
                alarm_flags_str.ASHT_bit = true;

            }else{
                alarm_sht20_str = sht20_not_alarm_send;
                alarm_flags_str.ASHT_bit = false;
            }

            /////////////////////////////

            if(hx711_estado != hx711_conect){
                if(alarm_hx711_str == hx711_not_alarm_send || alarm_hx711_str == hx711_first_alarm_send){
                    event_flags.event_alarm_flag = 1;
                    alarm_hx711_str++;
                }
                alarm_flags_str.AHX7_bit = true;

            }else{
                alarm_hx711_str = hx711_not_alarm_send;
                alarm_flags_str.AHX7_bit = false;
            }


            if(event_flags.event_alarm_flag || event_flags.event_send_flag){

                timestamp_alarm = unix_str.rtc_seconds;
                system_buff2tx_str.sample_counter++;
                event_time_seconds_str_t.sleep_cnt_send = 0;
                event_time_seconds_str_t.sleep_cnt_read = 0;
                read_last_pso=weight;
                //count_read_peripheral=0;
                Watchdog_feedTheDog(normalFood);
                /** GOTO: SM_Turn_On_CM_Q03(void)*/
                return next_state_2;

            }else{//probando reiniciando esas cuentas
//                event_time_seconds_str_t.sleep_cnt_send = 0;//esto se quita
                event_time_seconds_str_t.sleep_cnt_read = 0;
                read_last_pso=weight;
                system_buff2tx_str.sample_counter++;
            }


        /** GOTO: SM_Wait_Event(void)*/
        Watchdog_feedTheDog(normalFood);
        return next_state_0;
    }

}

/********************* Alarm event *********************************/

/********************* DATA FORMAT *************************************/

//Mediciones
uint8_t string_imei[]={"\"IMEI\":"};    //
#if(FOTA_ENABLE)
uint8_t string_swv[]={"\"SWV\":"};      //SoftWare Version
#endif

uint8_t string_rsrp[]={"\"RSRP\":"};    //
uint8_t string_rsrq[]={"\"RSRQ\":"};    //

uint8_t string_end_buff[]={"}],"};

//uint8_t string_sgr[]={"\"SGR\":"};
//uint8_t string_lgr[]={"\"LGR\":"};

/*****************VARIABLE PESO*********************/
uint8_t string_weight[]={"{\"SP\":["};

/*****************VARIABLE TEMP/HUM*********************/
uint8_t string_temperature[]={"\"ST\":["};
uint8_t string_humedity[]={"\"SH\":["};

/*****************VOLTAJE PILA*********************/
uint8_t string_porcentaje_bat[]={"\"SB\":["};

//*******STRING PARA CRC********************//
uint8_t string_CRC[]={"\"CRC\":"};
uint8_t string_ARS[]={"\"ARS\":"};     // Alarma

//alarmas mias basicas
uint8_t string_ASP[]={"\"ASP\":"}; //peso excesivo
uint8_t string_AMH[]={"\"AMH\":"};//alarma modulo Hx711
uint8_t string_AMS[]={"\"AMS\":"};//alarma Modulo sht20
uint8_t string_AVB[]={"\"AVB\":"};// Alarma de voltaje bajo

uint8_t string_ACT[]={"\"ACT\":"};//alarma cambio temp
uint8_t string_AHA[]={"\"AHA\":"};//alarma alta humedad
uint8_t string_ABS[]={"\"ABS\":"}; //alarma bad signal
//alarmas mias basicas


uint8_t string_Time_on[]={"\"TON\":"};     // Alarma

uint8_t string_cel_id[]={"\"CID\":"};

uint8_t string_count_attempt[]={"\"CIT\":"};


uint8_t string_context[]={",\"context\":{\"tstamp\": \""};


uint8_t string_PTT[]={"\"PTT\":["};     // Parametro de tiempos de transmision de datos
uint8_t string_PTC[]={"\"PTC\":["};     // Parametros de tiempos de captura de datos
uint8_t string_PPM[]={"\"PPM\":["};     // Parametros de promedio de muestras
uint8_t string_PCF[]={"\"PCF\":["};     // Parametro de activacion de FOTA

uint8_t string_value[]={"{\"value\":"};
uint8_t string_value_context[]={"{\"context\": \""};

uint8_t string_timestamp[]={",\"timestamp\":"};

uint8_t string_rtc[]={"\"RTC\":"};
uint8_t string_end_coma[]={"},"};
uint8_t string_end[]={"}"};
uint8_t string_coma[]={","};
uint8_t string_underscore[]={"-"};
uint8_t string_two_points[]={":"};
uint8_t string_space[]={" "};
uint8_t string_marks[]={"\""};

uint8_t string_plmn[]={"\"PLMN\":"}; //extra 1
uint8_t string_band[]={"\"BAND\":"}; //extra 2
uint8_t string_canal[]={"\"CANAL\":"}; //extra 3
uint8_t string_ICCID[]={"\"ICCID\":"}; //extra 4

uint8_t string_add_mseconds[]={"000"};
uint8_t string_add_seconds_and_endComa[]={"000},"};


#define DATA_BUFF_MAX_SIZE          (11)
#define MSG_TO_BE_SENT_MAX_SIZE     (4496)

static uint8_t msg_to_be_sent[MSG_TO_BE_SENT_MAX_SIZE] = {0};
static uint16_t msg_size = 0;

uint8_t count_data_tch=0;

void Add_end_format(void){

    uint8_t temp_buff2[DATA_BUFF_MAX_SIZE] = {0};
    strcat((char *)(msg_to_be_sent), (const char *)(string_timestamp));
    uitoa(unix_str.rtc_seconds, (char *)(temp_buff2), DECIMAL_BASE);
    strncat((char *)(msg_to_be_sent), (const char *)(temp_buff2),strlen((const char *)temp_buff2));

    strcat((char *)(msg_to_be_sent), (const char *)(string_add_seconds_and_endComa));
//    strcat((char *)(msg_to_be_sent), (const char *)(string_end_coma));
    memset(temp_buff2, '\0', DATA_BUFF_MAX_SIZE);

}

void Buff_capturas(int32_t buff_value, uint32_t data_timestamp){

    uint8_t temp_buff2[DATA_BUFF_MAX_SIZE] = {0};
    strcat((char *)(msg_to_be_sent), (const char *)(string_value));
    itoa(buff_value, (char *)(temp_buff2), DECIMAL_BASE);
    strncat((char *)(msg_to_be_sent), (const char *)(temp_buff2),strlen((const char *)temp_buff2));
    memset(temp_buff2, '\0', DATA_BUFF_MAX_SIZE);
    strcat((char *)(msg_to_be_sent), (const char *)(string_timestamp));
    uitoa(data_timestamp, (char *)(temp_buff2), DECIMAL_BASE);
    strncat((char *)(msg_to_be_sent), (const char *)(temp_buff2),strlen((const char *)temp_buff2));
    strcat((char *)(msg_to_be_sent), (const char *)(string_add_mseconds));
    memset(temp_buff2, '\0', DATA_BUFF_MAX_SIZE);

}

next_state_t SM_Data_Format(void)
{
    cm_q03_codes_t retval = Success;

    uint8_t lac[8]={0};
    uint16_t lac_size=0;
    uint8_t cid[15]={0};
    uint16_t cid_size=0;
    bool bit_2G_signal =true;

    //printf("\nData_format\n");


    if(data_format_ready){
        Watchdog_feedTheDog(normalFood);
        if(REGISTERED_TO_NET == registered)
        {
            return next_state_0;
        }

        else
        {
//            GPIO_Write(BUZ,1);

            return next_state_1;
        }
    }


    memset(rsrp, 0, sizeof(rsrp));
    memset(rsrq, 0, sizeof(rsrq));
    memset(sysmode, 0, sizeof(sysmode));
    if (NOT_REGISTERED_TO_NET == registered)
    {
        rsrp[0] = '0';
        rsrp_size = 1;
        rsrq[0] = '0';
        rsrq_size = 1;
        cid[0] = '0';
        cid_size = 1;
    }else
    {
    retval=CM_Q03_4G_Signal_Quality_Report( sysmode, &sysmode_size, rssi,
            &rssi_size, rsrp, &rsrp_size, sinr, &sinr_size,
            rsrq, &rsrq_size);

        if(retval!=Success) // If error in previous function then send X to signal an error in the funciton and distinguise from "No network" state
    {
            memset(rsrp, 0, sizeof(rsrp));
            rsrp[0] = '0';
        rsrp_size=1;

            memset(rsrq, 0, sizeof(rsrq));
            rsrq[0] = '0';
            rsrq_size=1;

            cid[0] = 'E';
            cid[1] = 'R';
            cid[2] = 'R';
            cid[3] = 'O';
            cid[4] = 'R';
            cid_size = 5;

        }else
        {
            if(sysmode[0] == 'G')
            {
                // Use RSRP variable to transmit the RSSI value when in 2G
                rsrp[0] = rssi[0];
                rsrp[1] = rssi[1];
                rsrp[2] = rssi[2];
                rsrp[3] = rssi[3];
                rsrp[4] = rssi[4];
                rsrp[5] = rssi[5];
                rsrp_size = rssi_size;

                memset(rsrq, 0, sizeof(rsrq)); //when RSRQ is equal to 0 and RSRP have a value then it means device is in 2G
                rsrq[0] = '0';
        rsrq_size=1;

                // Fill CID value with GSM indication
                cid[0] = 'G';
                cid[1] = 'S';
                cid[2] = 'M';
                cid_size = 3;
                bit_2G_signal =true;
            }else // If device is in CAT-M1 or NB
            {
                bit_2G_signal =false;
                retval=CM_Q03_General_Get_LAC_CID(lac, &lac_size,cid, &cid_size);
                if(retval!=Success)
                {
                    memset(lac, '0', 1);
                    lac_size=1;

                    memset(cid, '0', 1);
                    cid_size=1;

                }
            }
        }
    }

    //**PASAMOS A INT LOS PARAMETROS DE RSRP y RSRQ
    rsrp_number = (uint16_t)atoi((char*)rsrp);//menor a -105 es malo
    rsrq_number = (uint16_t)atoi((char*)rsrq);//menor a -19 es malo
    Delay(200000);

    if(rsrp_number<=-105 && rsrq_number<=-19){

//        if(alarm_signal_str == signal_not_alarm_send || alarm_signal_str == signal_first_alarm_send){
//            event_flags.event_alarm_flag=1;
//            alarm_signal_str++;
//        }
        alarm_flags_str.ASIG_bit = true;
        signal_status = bad_signal;
    }else{
        alarm_flags_str.ASIG_bit = false;
        signal_status = normal_signal;
    }
    if(bit_2G_signal == true){
        alarm_flags_str.ASIG_bit = true;
        signal_status = bad_signal;
    }
    bit_2G_signal =false;
//    retval=CM_Q03_General_Get_LAC_CID(lac, &lac_size,cid, &cid_size);
//    if(retval!=Success)
//    {
//        memset(lac, '0', 1);
//        lac_size=1;
//
//        memset(cid, '0', 1);
//        cid_size=1;
//
//    }

    uint8_t temp_buff[DATA_BUFF_MAX_SIZE] = {0};

    memset(msg_to_be_sent, '\0', sizeof(msg_to_be_sent));
    memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
    count_data_tch=0;


    //system_buff2tx_str.peso_buff[0]=120000; //super dummy
    create_CRC(system_buff2tx_str.temp_buff[0], system_buff2tx_str.hum_buff[0], system_buff2tx_str.peso_buff[0], &crc_send);

    if(first_transmission==state_reconnection){

        RTC_Get_Time_Seconds(&unix_str.rtc_seconds);
        system_buff2tx_str.timestamp_tch_buff[count_data_tch]=unix_str.rtc_seconds;
        timestamp_alarm=unix_str.rtc_seconds;
        system_buff2tx_str.sample_counter++;

    }

    strcpy((char *)(msg_to_be_sent), (const char *)(string_weight));
    do{

//        strcat((char *)(msg_to_be_sent), (const char *)(string_value));
//        itoa(system_buff2tx_str.peso_buff[count_data_tch], (char *)(temp_buff), DECIMAL_BASE);
        Buff_capturas(system_buff2tx_str.peso_buff[count_data_tch], system_buff2tx_str.timestamp_tch_buff[count_data_tch]);
        system_buff2tx_str.peso_buff[count_data_tch]=0;
//        strncat((char *)(msg_to_be_sent), (const char *)(temp_buff),strlen((const char *)temp_buff));
//        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
//        strcat((char *)(msg_to_be_sent), (const char *)(string_timestamp));
//        uitoa(system_buff2tx_str.timestamp_tch_buff[count_data_tch], (char *)(temp_buff), DECIMAL_BASE);
//        strncat((char *)(msg_to_be_sent), (const char *)(temp_buff),strlen((const char *)temp_buff));
//        strcat((char *)(msg_to_be_sent), (const char *)(string_add_mseconds));
//        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
        count_data_tch++;
        if(system_buff2tx_str.sample_counter > count_data_tch){
            strcat((char *)(msg_to_be_sent), (const char *)(string_end_coma));
        }
    }while(system_buff2tx_str.sample_counter > count_data_tch);
    strcat((char *)(msg_to_be_sent), (const char *)(string_end_buff));
    count_data_tch=0;


//************************estructura para buff de sensor de temp

    strcat((char *)(msg_to_be_sent), (const char *)(string_temperature));
    do{

        Buff_capturas(system_buff2tx_str.temp_buff[count_data_tch], system_buff2tx_str.timestamp_tch_buff[count_data_tch]);
        system_buff2tx_str.temp_buff[count_data_tch]=0;
        count_data_tch++;
        if(system_buff2tx_str.sample_counter > count_data_tch){
            strcat((char *)(msg_to_be_sent), (const char *)(string_end_coma));
        }
    }while(system_buff2tx_str.sample_counter > count_data_tch);
    strcat((char *)(msg_to_be_sent), (const char *)(string_end_buff));
    count_data_tch=0;


//************************estructura para buff de sensor de hum
        strcat((char *)(msg_to_be_sent), (const char *)(string_humedity));
        do{

            Buff_capturas(system_buff2tx_str.hum_buff[count_data_tch], system_buff2tx_str.timestamp_tch_buff[count_data_tch]);
            system_buff2tx_str.hum_buff[count_data_tch]=0;
            count_data_tch++;
            if(system_buff2tx_str.sample_counter > count_data_tch){
                strcat((char *)(msg_to_be_sent), (const char *)(string_end_coma));
            }
        }while(system_buff2tx_str.sample_counter > count_data_tch);
        strcat((char *)(msg_to_be_sent), (const char *)(string_end_buff));
        count_data_tch=0;


//************************estructura para buff de sensor de voltaje

        strcat((char *)(msg_to_be_sent), (const char *)(string_porcentaje_bat));
            do{

                Buff_capturas(system_buff2tx_str.voltage_buff[count_data_tch], system_buff2tx_str.timestamp_tch_buff[count_data_tch]);
                system_buff2tx_str.voltage_buff[count_data_tch]=0;
                count_data_tch++;
                if(system_buff2tx_str.sample_counter > count_data_tch){
                    strcat((char *)(msg_to_be_sent), (const char *)(string_end_coma));
                }
            }while(system_buff2tx_str.sample_counter > count_data_tch);
            strcat((char *)(msg_to_be_sent), (const char *)(string_end_buff));


         count_data_tch--;//esto debe de ir al ultimo de los ciclos y

//***************************CADENA DE CRC*********************************//
         memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
         strcat((char *)(msg_to_be_sent), (const char *)(string_CRC));
         strcat((char *)(msg_to_be_sent), (const char *)(string_value));

         itoa(crc_send, (char *)(temp_buff), DECIMAL_BASE);
         strncat((char *)(msg_to_be_sent), (const char *)(temp_buff), strlen((const char *)temp_buff));
         memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);

//         strcat((char *)(msg_to_be_sent), (const char *)(string_timestamp));
//         uitoa(timestamp_alarm, (char *)(temp_buff), DECIMAL_BASE);
//         strncat((char *)(msg_to_be_sent), (const char *)(temp_buff),strlen((const char *)temp_buff));
//         strcat((char *)(msg_to_be_sent), (const char *)(string_add_mseconds));
//         strcat((char *)(msg_to_be_sent), (const char *)(string_end_coma));
//         memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
         Add_end_format();



    if(status_reset_size!=0){


        strcat((char *)(msg_to_be_sent), (const char *)(string_ARS));
        strcat((char *)(msg_to_be_sent), (const char *)(string_value));
        strcat((char *)(msg_to_be_sent), (const char *)(string_marks));

        strncat((char *)(msg_to_be_sent), (const char *)(status_reset),
                   status_reset_size);
        strcat((char *)(msg_to_be_sent), (const char *)(string_marks));
        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);

//        strcat((char *)(msg_to_be_sent), (const char *)(string_timestamp));
//        uitoa(timestamp_alarm, (char *)(temp_buff), DECIMAL_BASE);
//        strncat((char *)(msg_to_be_sent), (const char *)(temp_buff),strlen((const char *)temp_buff));
//        strcat((char *)(msg_to_be_sent), (const char *)(string_add_mseconds));
//        strcat((char *)(msg_to_be_sent), (const char *)(string_end_coma));
//        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
        Add_end_format();

        status_reset_size=0;
    }
    else{
        strcat((char *)(msg_to_be_sent), (const char *)(string_ARS));
        strcat((char *)(msg_to_be_sent), (const char *)(string_value));
        strcat((char *)(msg_to_be_sent), (const char *)(string_marks));
        temp_buff[0]='0';
        strncat((char *)(msg_to_be_sent), (const char *)(temp_buff),1);
        strcat((char *)(msg_to_be_sent), (const char *)(string_marks));
        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);

//        strcat((char *)(msg_to_be_sent), (const char *)(string_timestamp));
//        uitoa(timestamp_alarm, (char *)(temp_buff), DECIMAL_BASE);
//        strncat((char *)(msg_to_be_sent), (const char *)(temp_buff),strlen((const char *)temp_buff));
//        strcat((char *)(msg_to_be_sent), (const char *)(string_add_mseconds));
//        strcat((char *)(msg_to_be_sent), (const char *)(string_end_coma));
//        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
        Add_end_format();

        status_reset_size=0;
    }


    memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);

    strcat((char *)(msg_to_be_sent), (const char *)(string_Time_on));
    strcat((char *)(msg_to_be_sent), (const char *)(string_value));
    uitoa( count_time_on_cmq0, (char *)(temp_buff), DECIMAL_BASE);
    strncat((char *)(msg_to_be_sent), (const char *)(temp_buff),
            strlen((const char *)temp_buff));
    memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
    Add_end_format();


    strcat((char *)(msg_to_be_sent), (const char *)(string_cel_id));
    strcat((char *)(msg_to_be_sent), (const char *)(string_value_context));
    strncat((char *)(msg_to_be_sent), (const char *)(cid),cid_size);
    strcat((char *)(msg_to_be_sent), (const char *)(string_marks));
    Add_end_format();


    strcat((char *)(msg_to_be_sent), (const char *)(string_count_attempt));
    strcat((char *)(msg_to_be_sent), (const char *)(string_value));
    uitoa( attemp_count_register, (char *)(temp_buff), DECIMAL_BASE);
    strncat((char *)(msg_to_be_sent), (const char *)(temp_buff),
            strlen((const char *)temp_buff));
    memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
    Add_end_format();


    /* IMEI */
    strcat((char *)(msg_to_be_sent), (const char *)(string_imei));
    strcat((char *)(msg_to_be_sent), (const char *)(string_value));

    strncat((char *)(msg_to_be_sent), (const char *)(ubidots_str.imei),sizeof(ubidots_str.imei));
    memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
    Add_end_format();

#if(FOTA_ENABLE)
    /* SW Version */
    strcat((char *)(msg_to_be_sent), (const char *)(string_swv));
    strcat((char *)(msg_to_be_sent), (const char *)(string_value));

    strncat((char *)(msg_to_be_sent), (const char *)(ubidots_str.swv),sizeof(ubidots_str.swv));
    memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
    Add_end_format();

#endif
    //PLMN
    strcat((char *)(msg_to_be_sent), (const char *)(string_plmn));
    strcat((char *)(msg_to_be_sent), (const char *)(string_value));
    strcat((char *)(msg_to_be_sent), (const char *)(plmn_str));
    Add_end_format();


    //banda
    strcat((char *)(msg_to_be_sent), (const char *)(string_band));
    strcat((char *)(msg_to_be_sent), (const char *)(string_value));
    strcat((char *)(msg_to_be_sent), (const char *)(string_marks));
    strcat((char *)(msg_to_be_sent), (const char *)(banda_str));
    strcat((char *)(msg_to_be_sent), (const char *)(string_marks));
    Add_end_format();

    //canal
    strcat((char *)(msg_to_be_sent), (const char *)(string_canal));
    strcat((char *)(msg_to_be_sent), (const char *)(string_value));
    strcat((char *)(msg_to_be_sent), (const char *)(canal_str));
    Add_end_format();

    //iccid
    strcat((char *)(msg_to_be_sent), (const char *)(string_ICCID));
    strcat((char *)(msg_to_be_sent), (const char *)(string_value));
    strcat((char *)(msg_to_be_sent), (const char *)(iccid));
    Add_end_format();

    //////////////////////FORMATO DE alarmas Mias/////////////////////////////

    if(alarm_flags_str.WEG_bit_high || alarm_flags_str.WEG_bit_low ||
            alarm_flags_str.WEG_bit_negative_change || alarm_flags_str.WEG_bit_positive_change){// antes alarma_pesos!=normal_weight

        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
        strcat((char *)(msg_to_be_sent), (const char *)(string_ASP));
        strcat((char *)(msg_to_be_sent), (const char *)(string_value));

        uitoa(alarma_pesos, (char *)(temp_buff), DECIMAL_BASE);
        strncat((char *)(msg_to_be_sent), (const char *)(temp_buff),strlen((const char *)temp_buff));
        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
        Add_end_format();

        alarma_pesos=normal_weight;
        alarm_flags_str.WEG_bit_high=false;
        alarm_flags_str.WEG_bit_low=false;
        alarm_flags_str.WEG_bit_negative_change=false;
        alarm_flags_str.WEG_bit_positive_change=false;

    }

    if(alarm_flags_str.AHX7_bit){
        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
        strcat((char *)(msg_to_be_sent), (const char *)(string_AMH));
        strcat((char *)(msg_to_be_sent), (const char *)(string_value));

        uitoa(hx711_estado, (char *)(temp_buff), DECIMAL_BASE);
        strncat((char *)(msg_to_be_sent), (const char *)(temp_buff),strlen((const char *)temp_buff));
        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);

        Add_end_format();

        hx711_estado = hx711_conect;
        alarm_flags_str.AHX7_bit = false;
    }

    if(alarm_flags_str.ASHT_bit){
        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
        strcat((char *)(msg_to_be_sent), (const char *)(string_AMS));
        strcat((char *)(msg_to_be_sent), (const char *)(string_value));

        uitoa(sht20_estado, (char *)(temp_buff), DECIMAL_BASE);
        strncat((char *)(msg_to_be_sent), (const char *)(temp_buff),strlen((const char *)temp_buff));
        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);

        Add_end_format();

        sht20_estado =sht20_conect;
        alarm_flags_str.ASHT_bit = false;

    }

    if(alarm_flags_str.ABAT_bit){
        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
        strcat((char *)(msg_to_be_sent), (const char *)(string_AVB));
        strcat((char *)(msg_to_be_sent), (const char *)(string_value));

        uitoa(batery_estado, (char *)(temp_buff), DECIMAL_BASE);
        strncat((char *)(msg_to_be_sent), (const char *)(temp_buff),strlen((const char *)temp_buff));
        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
        Add_end_format();

        batery_estado = normal_batery;
        alarm_flags_str.ABAT_bit = false;

    }

    if(alarm_flags_str.ATEM_bit){//temp
        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
        strcat((char *)(msg_to_be_sent), (const char *)(string_ACT));
        strcat((char *)(msg_to_be_sent), (const char *)(string_value));

        uitoa(alarma_temperatura, (char *)(temp_buff), DECIMAL_BASE);
        strncat((char *)(msg_to_be_sent), (const char *)(temp_buff),strlen((const char *)temp_buff));
        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);

        Add_end_format();

        alarma_temperatura=normal_temperature;
        alarm_flags_str.ATEM_bit=false;

    }

    if(alarm_flags_str.AHUM_bit){//hum
        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
        strcat((char *)(msg_to_be_sent), (const char *)(string_AHA));
        strcat((char *)(msg_to_be_sent), (const char *)(string_value));

        uitoa(alarma_humedad, (char *)(temp_buff), DECIMAL_BASE);
        strncat((char *)(msg_to_be_sent), (const char *)(temp_buff),strlen((const char *)temp_buff));
        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
        Add_end_format();

        alarma_humedad=normal_humidity;
        alarm_flags_str.AHUM_bit=false;
    }

    if(alarm_flags_str.ASIG_bit){

        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
        strcat((char *)(msg_to_be_sent), (const char *)(string_ABS));
        strcat((char *)(msg_to_be_sent), (const char *)(string_value));

        uitoa(signal_status, (char *)(temp_buff), DECIMAL_BASE);
        strncat((char *)(msg_to_be_sent), (const char *)(temp_buff),strlen((const char *)temp_buff));
        memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);

        Add_end_format();

        signal_status = normal_signal;
        alarm_flags_str.ASIG_bit=false;

    }


    /* RSSI */
    strcat((char *)(msg_to_be_sent), (const char *)(string_rsrp));
    strcat((char *)(msg_to_be_sent), (const char *)(string_value));
    strcat((char *)(msg_to_be_sent), (const char *)(rsrp));
    memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);
    Add_end_format();

    /* RSSI */
    strcat((char *)(msg_to_be_sent), (const char *)(string_rsrq));
    strcat((char *)(msg_to_be_sent), (const char *)(string_value));
    strcat((char *)(msg_to_be_sent), (const char *)(rsrq));
    memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);

    strcat((char *)(msg_to_be_sent), (const char *)(string_timestamp));
    uitoa( unix_str.rtc_seconds, (char *)(temp_buff), DECIMAL_BASE);
    strncat((char *)(msg_to_be_sent), (const char *)(temp_buff),
            strlen((const char *)temp_buff));
    strcat((char *)(msg_to_be_sent), (const char *)(string_add_mseconds));

    strcat((char *)(msg_to_be_sent), (const char *)(string_end));
    strcat((char *)(msg_to_be_sent), (const char *)(string_end));

    memset(temp_buff, '\0', DATA_BUFF_MAX_SIZE);


    msg_size = strlen((const char *)(msg_to_be_sent));
    counterssi=0;
    attemp_count_register=0;

//#if (1)
//    do{
//    printf("%c",msg_to_be_sent[counterssi]);
//        counterssi++;
//    }while(msg_size!=counterssi);
//    printf("\n\n");
//#endif

    system_buff2tx_str.sample_counter=0;

    counterssi=0;
    data_format_ready=true;

    Watchdog_feedTheDog(normalFood);
    if(REGISTERED_TO_NET == registered)
    {
        return next_state_0;
    }

    else
    {
//        GPIO_Write(BUZ,1);
        return next_state_1;
    }
}
/********************* DATA FORMAT *************************************/


/********************* FILE QUERY **************************************/
next_state_t SM_File_Query(void)
{
    cm_q03_codes_t retval = Success;


    //printf("\nQery_file\n");
   CM_Q03_Set_Command_Terminator_Character_ATS_Reconnect();

    if(file_read_ptr > file_write_ptr){

       Delay(2000000);
       retval = CM_Q03_File_Delete_File((uint8_t*)read_ptr_file, sizeof(read_ptr_file) - 1);

       if((Success != retval) && ((File_File_not_found != retval) && File_Fail_to_delete_the_file!= retval))
       {
           return next_state_3;
       }
       Watchdog_feedTheDog(normalFood);

       Delay(2000000);
       retval= CM_Q03_File_Delete_File((uint8_t*)write_ptr_file, sizeof(write_ptr_file) - 1);

       if((Success != retval) && ((File_File_not_found != retval)&&File_Fail_to_delete_the_file!= retval))
       {
           return next_state_3;
       }
       Watchdog_feedTheDog(normalFood);

       file_read_ptr=0;
       file_write_ptr=0;
    }

    if(file_read_ptr == file_write_ptr)
    {

        if(file_read_ptr>=32500){
            Delay(2000000);
            retval = CM_Q03_File_Delete_File((uint8_t*)read_ptr_file, sizeof(read_ptr_file) - 1);

            if((Success != retval) && ((File_File_not_found != retval)&&File_Fail_to_delete_the_file!= retval))
            {
                return next_state_3;
            }
            Watchdog_feedTheDog(normalFood);

            Delay(2000000);
            retval= CM_Q03_File_Delete_File((uint8_t*)write_ptr_file, sizeof(write_ptr_file) - 1);

            if((Success != retval) && ((File_File_not_found != retval)&&File_Fail_to_delete_the_file!= retval))
            {
                return next_state_3;
            }
            Watchdog_feedTheDog(normalFood);

            file_read_ptr=0;
            file_write_ptr=0;
        }

            return next_state_1;
    }

    Watchdog_feedTheDog(normalFood);
    return next_state_0;
}
/********************* FILE QUERY **************************************/

/********************* FILE DOWNLOAD ***********************************/
#define MAX_FILES                   (65535)
#define MAX_FILES_N_CHARACTERS      (6)
#define FILE_PREFIX_SIZE            (9)
#define FILE_EXT_SIZE               (5)

const uint8_t file_prefix[FILE_PREFIX_SIZE] = "old_data\0";
const uint8_t file_ext[FILE_EXT_SIZE] = ".txt\0";

static uint8_t download_file_name[MAX_FILES_N_CHARACTERS
                  + FILE_PREFIX_SIZE
                  + FILE_EXT_SIZE] = {0};
static uint16_t download_file_name_size = 0;

static uint8_t old_msg_to_be_sent[MSG_TO_BE_SENT_MAX_SIZE] = {0};
static uint16_t old_msg_to_be_sent_size = 0;

next_state_t SM_File_Download(void)
{
    cm_q03_codes_t retval = Success;

    //printf("\nFile_dowload\n");

    memset(download_file_name, '\0', sizeof(download_file_name));
    download_file_name_size = 0;

    retval = CM_Q03_Set_Command_Terminator_Character_ATS_Reconnect();



    SM_Create_File_Name(file_prefix, FILE_PREFIX_SIZE, file_read_ptr, file_ext,
            FILE_EXT_SIZE, download_file_name, &download_file_name_size);

    retval = CM_Q03_File_Download_File(download_file_name, download_file_name_size,
            old_msg_to_be_sent, &old_msg_to_be_sent_size);


    if(Success == retval)
    {
        Watchdog_feedTheDog(normalFood);
        return next_state_0;
    }

    else
    {
        file_read_ptr ++;

        return next_state_1;
    }
}
/********************* FILE DOWNLOAD ***********************************/

/********************* PUBLISH OLD DATA ********************************/
#define MQTT_PUB_OLD_DATA_MAX_RETRIES   (3)
#define OLD_DATA_QoS                    (at_least_once)
#define OLD_DATA_RETAIN                 (not_retain)

static uint8_t pub_old_data_cnt = 0;

next_state_t SM_MQTT_Publish_Old_Data(void)
{
    cm_q03_codes_t retval = Success;
    msgID=1;

    Delay(1000000);
    retval = CM_Q03_MQTT_Publish(connID, msgID, ubidots_str.topic, sizeof(ubidots_str.topic),
            old_msg_to_be_sent, old_msg_to_be_sent_size,
            OLD_DATA_QoS, OLD_DATA_RETAIN);
    //printf("\nCM_mqttPublish_old_data\n");
    Delay(1000000);

    Watchdog_feedTheDog(normalFood);

    if((MQTT_Packet_sent_successfully == retval) ||
            (MQTT_Packet_retransmission == retval))
    {
        pub_old_data_cnt = 0;

        return next_state_0;
    }

    else if(MQTT_PUB_OLD_DATA_MAX_RETRIES <= pub_old_data_cnt)
    {
        pub_old_data_cnt = 0;

        return next_state_1;
    }

    else
    {
        pub_old_data_cnt ++;

        return next_state_2;
    }
}
/********************* PUBLISH OLD DATA ********************************/

/********************* FILE DELETE *************************************/
next_state_t SM_File_Delete(void)
{
    cm_q03_codes_t retval = Success;
    //printf("\nFile_Delete\n");

    retval = CM_Q03_Set_Command_Terminator_Character_ATS_Reconnect();

    retval = CM_Q03_File_Delete_File(download_file_name, download_file_name_size);

    if((Success == retval) || ((File_File_not_found == retval)&&File_Fail_to_delete_the_file== retval))
    {
        Watchdog_feedTheDog(normalFood);
        return next_state_0;
    }

    else
    {
        return next_state_1;
    }
}
/********************* FILE DELETE *************************************/

/********************* UPDATE READ PTR FILE ****************************/
next_state_t SM_File_Update_Read_Ptr_File(void)
{
    cm_q03_codes_t retval = Success;
    //printf("\nUpdate_files\n");

    uint8_t file_buff[FILE_PTRS_MAX_SIZE] = {0};
    uint8_t file_buff_size = 0;


    retval = CM_Q03_Set_Command_Terminator_Character_ATS_Reconnect();


    uitoa((file_read_ptr), (char *)(file_buff), DECIMAL_BASE);
    file_buff_size = (uint8_t)strlen((const char*)(file_buff));

    retval = CM_Q03_File_Delete_File((uint8_t*)read_ptr_file, sizeof(read_ptr_file) - 1);

    if((Success != retval) && ((File_File_not_found != retval)&&File_Fail_to_delete_the_file!= retval)) //extra
    {
        return next_state_1;
    }

    retval = CM_Q03_File_Upload_File((uint8_t*)read_ptr_file, sizeof(read_ptr_file),
            file_buff, file_buff_size);

    if(Success != retval)
    {
        return next_state_1;
    }

    else
    {

        Watchdog_feedTheDog(normalFood);
        return next_state_0;
    }
}
/********************* UPDATE READ PTR FILE ****************************/

/********************* PUBLISH NEW DATA ********************************/
#define MQTT_PUB_NEW_DATA_MAX_RETRIES   (3)
#define NEW_DATA_QoS                    (at_least_once)
#define NEW_DATA_RETAIN                 (not_retain)

uint8_t pub_new_data_cnt = 0;

static uint16_t curr_max_msg_size = MSG_TO_BE_SENT_MAX_SIZE;

//peko debug
//uint8_t sensor_counter = 0;
//end

next_state_t SM_MQTT_Publish_New_Data(void)
{
    cm_q03_codes_t retval = MQTT_Packet_sent_successfully;
    msgID = 1;
    //printf("\nMQTT_publish_new_data\n");

    retval = CM_Q03_MQTT_Publish(connID, msgID, ubidots_str.topic, sizeof(ubidots_str.topic), msg_to_be_sent,
            msg_size, NEW_DATA_QoS, NEW_DATA_RETAIN);
    attemp_count_register+=100000000;

     Watchdog_feedTheDog(normalFood);

    if((MQTT_Packet_sent_successfully == retval) ||
            (MQTT_Packet_retransmission == retval))
    {
        data_format_ready=false;
        LED_Control(0);
        //peko debug
//        sensor_counter = 0;
        //end
        state_reconnection = transmission_success;
        pub_new_data_cnt = 0;
        return next_state_0;
    }

    else if(MQTT_PUB_NEW_DATA_MAX_RETRIES <= pub_new_data_cnt)
    {
        pub_new_data_cnt = 0;
//        GPIO_Write(BUZ,1);
        state_reconnection=transmission_fail;
        return next_state_2;
    }

    else
    {
        pub_new_data_cnt ++;
        state_reconnection=transmission_fail;
        return next_state_1;
    }
}
/********************* PUBLISH NEW DATA ********************************/

/********************* CHECK MQTT CONNECTION STATE *********************/

next_state_t SM_MQTT_Connection_State(void)
{
    cm_q03_codes_t retval = Success;

    retval = CM_Q03_MQTT_Connection_Status(connID);
    //printf("\nMQTT_Conenctios_states\n");

    if(MQTT_connected == retval)
    {
        mqtt_conn_state_retries = 0;
        Watchdog_feedTheDog(normalFood);

        return next_state_0;
    }

    else if(MQTT_CONN_STATE_MAX_RETRIES <= mqtt_conn_state_retries)
    {
        mqtt_conn_state_retries = 0;
        Watchdog_feedTheDog(normalFood);
        return next_state_2;
    }
    else if((MQTT_connecting == retval)
            || (MQTT_disconnecting == retval))
    {
        mqtt_conn_state_retries ++;
        Delay(20000000);
        Watchdog_feedTheDog(normalFood);
        return next_state_3;
    }

    else
    {
        mqtt_conn_state_retries ++;
        Watchdog_feedTheDog(normalFood);
        return next_state_1;
    }

}
/********************* CHECK MQTT CONNECTION STATE *********************/

/********************* QUERY AVAILABLE SPACE ***************************/
#define AVAILABLE_SPACE_OFFSET      (1472)

uint32_t available_space = 0;

next_state_t SM_File_Query_Av_Space(void)
{
    uint32_t total_space = 0;
    //printf("\n\ Query_state_av_space\n");


    CM_Q03_Set_Command_Terminator_Character_ATS_Reconnect();


    CM_Q03_File_Get_Space_Information(&available_space, &total_space);

    Watchdog_feedTheDog(normalFood);

    if((file_write_ptr == file_read_ptr)
            && (available_space < (curr_max_msg_size + AVAILABLE_SPACE_OFFSET)))
    {
        return next_state_2;
    }

    else if((file_write_ptr != file_read_ptr)
            && (available_space < (curr_max_msg_size + AVAILABLE_SPACE_OFFSET)))
    {
        return next_state_3;
    }

    else if(file_read_ptr == (uint16_t)(file_write_ptr + 1))
    {
        return next_state_1;
    }

    else
    {
        return next_state_0;
    }
}
/********************* QUERY AVAILABLE SPACE ***************************/

/********************* FREE SPACE **************************************/
next_state_t SM_File_Free_Space(void)
{
    cm_q03_codes_t retval = Success;
    uint8_t free_file_name[MAX_FILES_N_CHARACTERS
                                  + FILE_PREFIX_SIZE
                                  + FILE_EXT_SIZE] = {0};

    uint16_t free_file_name_size = 0;

    //printf("\nFile_freee_space\n");


    retval = CM_Q03_Set_Command_Terminator_Character_ATS_Reconnect();

    SM_Create_File_Name(file_prefix, FILE_PREFIX_SIZE, file_read_ptr, file_ext,
            FILE_EXT_SIZE, free_file_name, &free_file_name_size);

    retval = CM_Q03_File_Delete_File(free_file_name, free_file_name_size);

    if((Success == retval) || ((File_File_not_found == retval)||File_Fail_to_delete_the_file== retval))
    {
        file_read_ptr ++;

        Watchdog_feedTheDog(normalFood);
        return next_state_0;
    }

    else
    {
        return next_state_1;
    }
}
/********************* FREE SPACE **************************************/

/********************* OVERWRITE FILE **********************************/
next_state_t SM_File_Overwrite(void)
{
    cm_q03_codes_t retval = Success;

    uint8_t overwrite_file[MAX_FILES_N_CHARACTERS
                                  + FILE_PREFIX_SIZE
                                  + FILE_EXT_SIZE] = {0};
    uint16_t overwrite_file_size = 0;
    //printf("\nFile_overwrite\n");


    retval = CM_Q03_Set_Command_Terminator_Character_ATS_Reconnect();


    SM_Create_File_Name(file_prefix, FILE_PREFIX_SIZE, file_read_ptr, file_ext,
            FILE_EXT_SIZE, overwrite_file, &overwrite_file_size);

    retval = CM_Q03_File_Delete_File(overwrite_file, overwrite_file_size);

    if((Success == retval) || ((File_File_not_found == retval)||File_Fail_to_delete_the_file== retval))
    {
        file_read_ptr ++;

        Watchdog_feedTheDog(normalFood);
        return next_state_0;
    }

    else
    {
        return next_state_1;
    }
}
/********************* OVERWRITE FILE **********************************/

/********************* FILE UPLOAD *************************************/
next_state_t SM_File_Upload(void)
{
    cm_q03_codes_t retval = Success;

    uint8_t upload_file[MAX_FILES_N_CHARACTERS
                                  + FILE_PREFIX_SIZE
                                  + FILE_EXT_SIZE] = {0};
    uint16_t upload_file_size = 0;
    //printf("\nFile_Upload\n");

    retval = CM_Q03_Set_Command_Terminator_Character_ATS_Reconnect();


    SM_Create_File_Name(file_prefix, FILE_PREFIX_SIZE, file_write_ptr, file_ext,
            FILE_EXT_SIZE, upload_file, &upload_file_size);

    retval = CM_Q03_File_Upload_File(upload_file, upload_file_size,
            msg_to_be_sent, msg_size);
//    GPIO_Write(BUZ,1);

    if((Success == retval) || (File_File_already_existed == retval))
    {
        file_write_ptr ++;
        data_format_ready=false;
        state_reconnection=transmission_fail;
        Watchdog_feedTheDog(normalFood);
        return next_state_0;
    }

    else
    {
        return next_state_1;
    }
}
/********************* FILE UPLOAD *************************************/

/********************* UPDATE BOTH FILE POINTERS ***********************/
next_state_t SM_File_Update_Both_File_Ptr(void)
{
    cm_q03_codes_t retval = Success;

    uint8_t file_buff[FILE_PTRS_MAX_SIZE] = {0};
    uint8_t file_buff_size = 0;
    //printf("\nFile_update_ptr\n");

    retval = CM_Q03_Set_Command_Terminator_Character_ATS_Reconnect();


    uitoa((file_read_ptr), (char *)(file_buff), DECIMAL_BASE);
    file_buff_size = (uint8_t)strlen((const char*)(file_buff));

    retval = CM_Q03_File_Delete_File((uint8_t*)read_ptr_file, sizeof(read_ptr_file) - 1);

    //aqui force y cree el archivo nuevo con el nombre

    if((Success != retval) && ((File_File_not_found != retval) && File_Fail_to_delete_the_file!= retval))
    {
        return next_state_1;
    }

    retval = CM_Q03_File_Upload_File((uint8_t*)read_ptr_file, sizeof(read_ptr_file),
            file_buff, file_buff_size);

    if(Success != retval)
    {
        return next_state_1;
    }


    memset(file_buff, '\0', sizeof(file_buff));

    uitoa((file_write_ptr), (char *)(file_buff), DECIMAL_BASE);
    file_buff_size = (uint8_t)strlen((const char*)(file_buff));

    retval = CM_Q03_File_Delete_File((uint8_t*)write_ptr_file, sizeof(write_ptr_file) - 1);

    if((Success != retval) && ((File_File_not_found != retval)&&File_Fail_to_delete_the_file!= retval))
    {
        return next_state_1;
    }

    retval = CM_Q03_File_Upload_File((uint8_t*)write_ptr_file, sizeof(write_ptr_file),
            file_buff, file_buff_size);

    if(Success != retval)
    {
        return next_state_1;
    }

    else
    {
        Watchdog_feedTheDog(normalFood);

        return next_state_0;
    }
}
/********************* UPDATE BOTH FILE POINTERS ***********************/

/********************* TURN OFF CM Q03 *********************************/
next_state_t SM_Turn_Off_CM_Q03(void)
{
    msg_cnt ++;
    //printf("\nCM_Q03_trun_Off\n");

    Delay(1500000);
    CM_Q03_Control(cm_q03_turn_off);

    RTC_Get_Alarm_Seconds(&count_time_on_cmq0);


    Watchdog_feedTheDog(normalFood);
    return next_state_0;
}
/********************* TURN OFF CM Q03 *********************************/

/********************* Wait event *********************************/


//uint8_t const value_max_mem[6000]={1};
uint8_t count_wait=0;


//Peko Debug
//uint8_t wakeup_counter = 0;
//end
bool alarm_seconds=false;

//uint8_t count_test_lpu=0;
uint8_t count_down_parameters=0;
uint8_t counter_pth=0;
next_state_t SM_Wait_Event(void)
{

//    if(count_test_lpu > 3){
//  /** Set the count in terms of minutes*/
//    event_time_seconds_str_t.LPU_sleep_time = CNT_LPU_SLEEP_TIME;
//    }
//    else{
//        event_time_seconds_str_t.LPU_sleep_time = 10;
//        count_test_lpu++;
//
//    }

    /** Set event flags to false again*/
    event_flags.event_read_flag = false;
    event_flags.event_alarm_flag = false;
    event_flags.event_send_flag = false;

//    first_download_parameters=false;


    /** Led Debug
     *  !keed Alive*/
//    led_ctrl = !led_ctrl;
//    LED_Control(led_ctrl);
//        count_wait+=value_max_mem[count_wait];
//    count_wait++;
    //printf("\n\nWait %d",count_wait);
    while( (event_time_seconds_str_t.LPU_sleep_time > event_time_seconds_str_t.sleep_cnt_read) && !alarm_seconds && event_time_seconds_str_t.sleep_cnt_send < alarm_rtc_unix_next) // Peko Debug && (wakeup_counter <= 3)
    {

        LPU_Select_Power_Mode(run_mode, 60); //ANTES 60 prueba solo con 1 a ver que pasa
        LPU_Init_Power_Mode(llwu_timer,llwu_not_port);
//        GPIO_Write(LED, 0);
        LPU_Select_Power_Mode(lls_mode, 1);//duerme un minuto //antes 60 en 1
        //Delay(1000);
//        GPIO_Write(LED, 1);
        RTC_Get_Alarm_Flag(&alarm_seconds); //si borramos esta condicion de alarm_seconds del ciclo while? que pasara
        Delay(500);
        Watchdog_feedTheDog(normalFood);
        Delay(500);

        LED_Control(0);
//        Watchdog_feedTheDog(normalFood);
        event_time_seconds_str_t.sleep_cnt_download_parameters++;
        event_time_seconds_str_t.sleep_cnt_send++;
        event_time_seconds_str_t.sleep_cnt_read++;
//        counterssi = counterssi + 1;

        //Peko debug
//        wakeup_counter++;
        //end

    }

    counter_pth++;

//    GPIO_Write(BUZ,1);

    //printf("\n\nWait_out_minut");

    //LED_Control(0);
    //Delay(1000000);
//    LPU_Select_Power_Mode(vlpr_mode, 60); //comentado por errores

    //Peko Debug
//    wakeup_counter = 0;
    //end


//    if(event_time_seconds_str_t.LPU_cycle_send <= event_time_seconds_str_t.sleep_cnt_send)
    if(alarm_seconds || event_time_seconds_str_t.sleep_cnt_send >= alarm_rtc_unix_next || system_buff2tx_str.sample_counter >= 63 || countToTransmit <= counter_pth ) //Peko debug || sensor_counter >= 23
    {
//        count_wait=0;

        counter_pth=0;

//        if(alarm_seconds){
//            count_down_parameters++;
//        }

        if((CNT_DOWNLOAD_PARAMETERS - 7200) <= event_time_seconds_str_t.sleep_cnt_download_parameters){ // Peko debug || sensor_counter >= 23
            count_down_parameters=0;
            event_time_seconds_str_t.sleep_cnt_download_parameters=0;

            first_download_parameters=true;
            event_flags.event_subscribe_flag=true;

        }
//        else if(count_down_parameters >= parameter_download_value_str.PTP_value){
//            count_down_parameters=0;
//            event_time_seconds_str_t.sleep_cnt_download_parameters=0;
//
//            first_download_parameters=true;
//            event_flags.event_subscribe_flag=true;
//        }



        RTC_Clean_Alarm_Flag();
        alarm_seconds=false;

        RTC_Get_Time_Seconds(&alarm_rtc_actual);
//        uinx_12am=alarm_rtc_actual%OFFSET_UNIX_24;
//        uinx_aux=(alarm_rtc_actual-uinx_12am+time_zone_unix)+uinx_actual+seed_unix;

//
//        if((alarm_rtc_actual + 3600) > uinx_aux){
//            alarm_rtc_next= uinx_aux + OFFSET_UNIX_24;
////            alarm_rtc_unix_next= (uint32_t)(((uinx_aux + OFFSET_UNIX_24)-alarm_rtc_actual)/60.0f)+30;
////            alarm_rtc_unix_next=(uint32_t)(((uinx_aux + OFFSET_UNIX_24)-alarm_rtc_actual)+(30*60));//caso para pasarlo a segundos + el offset
//
//        }
//        else{
//            alarm_rtc_next= uinx_aux;
////            alarm_rtc_unix_next= (uint32_t)(((uinx_aux-alarm_rtc_actual)/60.0f)+30);
////            alarm_rtc_unix_next= (uint32_t)((uinx_aux-alarm_rtc_actual)+(30*60));
//        }

        alarm_rtc_next = alarm_rtc_actual + (parameter_download_value_str.PTH_value*3600) + (parameter_download_value_str.PTC_value*60);
        alarm_rtc_unix_next = (parameter_download_value_str.PTH_value*3600) + parameter_download_value_str.PTC_value*60;

//
//        alarm_rtc_next=alarm_rtc_actual+121;
//        alarm_rtc_unix_next=alarm_rtc_actual+3602;

        RTC_Set_Alarm_Seconds(alarm_rtc_next);
        RTC_Enable_Interrupt(RTC_Both_interrupt);


//        count_test_lpu=0;
        //event_time_seconds_str_t.sleep_cnt_capture = 0;
        event_flags.event_send_flag = true;
        event_time_seconds_str_t.sleep_cnt_read = 0;

        // Peko debug
//        sensor_counter = 0;
        //end
        //printf("\n\nWait_out_send");

        Watchdog_feedTheDog(normalFood);
        /** Event to send values*/
        return next_state_2;
    }
    else
    {
        //Peko debug
//        sensor_counter++;
        //end
        event_time_seconds_str_t.sleep_cnt_read = 0;
        event_flags.event_read_flag = true;
        //printf("\n\nWait_read");

        Watchdog_feedTheDog(normalFood);
        /** Event to read again*/
        return next_state_2;

    }

    Watchdog_feedTheDog(normalFood);
    /** Return to sleep*/
    return next_state_0;

}

/********************* Wait event *********************************/

/********************* Message MQTT Received *********************************/
next_state_t SM_Message_MQTT_Received(void)
{
    Watchdog_feedTheDog(normalFood);
    return next_state_0;
}

/********************* Message MQTT Received *********************************/

/********************* Connection status  *********************************/
next_state_t SM_Connection_Status (void)
{


    cm_q03_codes_t retval = Success;

    retval = CM_Q03_MQTT_Connection_Status(connID);

    if((MQTT_connecting == retval)
            || (MQTT_disconnecting == retval))
    {
        return next_state_3;
    }

    else if(MQTT_connected == retval)
    {
        mqtt_conn_state_retries = 0;
        Watchdog_feedTheDog(normalFood);

        return next_state_0;
    }

    else if(MQTT_CONN_STATE_MAX_RETRIES <= mqtt_conn_state_retries)
    {
        Watchdog_feedTheDog(normalFood);
        mqtt_conn_state_retries = 0;
        state_reconnection=lost_connection;

        return next_state_2;
    }

    else
    {
        mqtt_conn_state_retries ++;
        Watchdog_feedTheDog(normalFood);
        state_reconnection=lost_connection;
        return next_state_1;
    }

}

/********************* Connection status  *********************************/

/********************* SET SLEEP MODE **********************************/
next_state_t SM_Reset_System(void)
{
    Watchdog_feedTheDog(resetFood);

    return next_state_0;
}

/********************* SET SLEEP MODE **********************************/

/********************* RUN STATE MACHINE *******************************/
void SM_start_state_machine(void)
{
    while(1)
    {
        sm_iterator = *(state_machine[sm_iterator].next
                + state_machine[sm_iterator].stateFunc());
    }
}
/********************* RUN STATE MACHINE *******************************/
