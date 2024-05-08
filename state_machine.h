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
 * \file       state_machine.h
 * \brief      This is the header file for the state machine for the
 *              appricola project.
 * \details    *For more information, please refer to the included documentation
 *             provided.
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "init_config_functions.h"

typedef enum
{
    next_state_0,
    next_state_1,
    next_state_2,
    next_state_3,
    next_state_4,
    next_state_5,
    next_state_6,
    next_state_7,
    next_state_8,
    next_state_9,
    next_state_10,
}next_state_t;


//INICIO BATERIAS BEEWAZE

typedef enum
{
    normal_weight   =10,
    fast_weight_change_p, //sobre_peso positivo
    fast_weight_change_n,//sobrepeso_negativo
    over_weight,
    low_weight,

    hx711_conect    =20,
    hx711_disconect,
//    hx711_synchronization_trouble,
    hx711_conection_trouble,

    sht20_conect    =30,    //succes
    sht20_disconect,        //timeuout
    sht20_conection_trouble,    //else uknown

    normal_batery   =40,
    low_batery,
    batery_read_trouble,

    normal_temperature=50,
    high_temperature,
    low_temperature,

    normal_humidity=60,
    high_humidity,
    low_humidity,

    normal_signal=70,
    bad_signal

}estados_sensors_alarms;


typedef enum
{
    temp_not_alarm_send,
    temp_first_alarm_send,
    temp_seconds_alarm_send,
    temp_all_alarm_send,
}alarm_TEMP_state_t;

typedef enum
{
    hum_not_alarm_send,
    hum_first_alarm_send,
    hum_seconds_alarm_send,
    hum_all_alarm_send,
}alarm_HUM_state_t;

//typedef enum
//{
//    signal_not_alarm_send,
//    signal_first_alarm_send,
//    signal_seconds_alarm_send,
//    signal_all_alarm_send,
//}alarm_SIGNAL_state_t;

typedef enum
{
    batery_not_alarm_send,
    batery_first_alarm_send,
    batery_seconds_alarm_send,
    batery_all_alarm_send,
}alarm_BATERY_state_t;

typedef enum
{
    hx711_not_alarm_send,
    hx711_first_alarm_send,
    hx711_seconds_alarm_send,
    hx711_all_alarm_send,
}alarm_HX711_state_t;

typedef enum
{
    sht20_not_alarm_send,
    sht20_first_alarm_send,
    sht20_seconds_alarm_send,
    sht20_all_alarm_send,
}alarm_SHT20_state_t;

typedef enum
{
    whc_not_alarm_send,
    whc_first_alarm_send,
    whc_seconds_alarm_send,
    whc_all_alarm_send,
}alarm_WHC_state_t; //cambios en pesos

typedef struct {
    port_gpio_t input_Data;
    port_gpio_t output_signal;

}Comunication_pins;


//fin de estado de alarmas

/** structure to save and store the values of the event counts*/
typedef struct
{
    /** Variables to set the timers*/
    uint32_t LPU_cycle_download_parameters;
    uint32_t LPU_cycle_send;
    uint32_t LPU_cycle_read;
    uint32_t LPU_sleep_time;
    /** Variables to keep count of time*/
    uint32_t sleep_cnt_send;
    uint32_t sleep_cnt_read;
    uint32_t sleep_cnt_download_parameters;
}event_time_seconds_str;


/** Structure to calibrate and modify the alarms*/
//typedef struct
//{
//    uint16_t AGB;   //Alarma de humedad baja
//    uint16_t AGR;   //Alarma de humedad alta
//    uint16_t LGR;   //Alarma de humedad alta
//
//    uint32_t AVB;   //Alarma de voltaje bajo
//}alarm_data_t;

/** Structure to store the alarms flags*/
typedef struct
{


//    bool WEG_bit; //bit para englobar all
    bool WEG_bit_positive_change; //bit para englobar all
    bool WEG_bit_negative_change; //bit para englobar all
    bool WEG_bit_high; //bit para englobar all
    bool WEG_bit_low;

    bool AHX7_bit; //alarma Hx7
    bool ASHT_bit; //alarma sht20
    bool ABAT_bit; //alarma bateria
    bool ATEM_bit; //alarma  temp
    bool AHUM_bit; //alarma humedad

    bool ASIG_bit; //alarma señal

}alarm_flags_t;


/* Structure to store all the necessary values to connect to ubidots*/
typedef struct
{
    uint8_t imei[15];
    uint8_t swv[6];

    uint8_t topic[7];
    uint8_t mqtt_host[46];
    uint16_t mqtt_port;

    uint8_t user_name[13];

    uint8_t top_pap[19];
}ubidots_t;



/** structure to save and store the values of the temperature and humidity*/
typedef struct
{
    uint16_t n_samples;
    uint16_t sample_counter;
    int16_t temp_buff[65];
    int16_t hum_buff[65];
    int32_t voltage_buff[65];
    int32_t peso_buff[65];
    uint32_t timestamp_tch_buff[65];

}system_buff2tx_t;



/** Structure to calibrate and modify the alarms*/
/** Structure to store alarms events*/
typedef struct
{
    uint8_t event_read_flag;
    uint8_t event_alarm_flag;
    uint8_t event_send_flag;
    uint8_t event_subscribe_flag;
}event_flags_t;
typedef struct
{

    /************* Parametros *************/
    uint32_t pap_value;
    uint32_t PCC_value;// parametro calbracion completa
    uint32_t PTH_value;//
    uint32_t PTC_value;//
    uint32_t PTP_value;//
    uint32_t PCF_value;//
    //nuevas alarmas para beewze
    int32_t PAPE_value;//parametro alarma peso excesico
    int32_t PAPP_value;//parametro alarma peso maximo
    int32_t PATA_value;//parameto alarma temperatura alta
    int32_t PATB_value;//parametro alarma tem baja
    int32_t PAHA_value;//este ver por humedad
    int32_t PAHB_value;//PArametro alarma de humedad baja
    int32_t PAPB_value;//parametro alarma peso bajo

}parameter_download_value_t;

/** */
typedef struct
{
    uint8_t parameter_pap[8];
    uint8_t parameter_pcc[6];
    uint8_t parameter_pth[6];
    uint8_t parameter_ptc[6];
    uint8_t parameter_ptp[6];
    uint8_t parameter_pcf[6];

//
    uint8_t parameter_pape[7];//esperado 10 en kilos
    uint8_t parameter_papp[7];//esperado 200000 en gramos
    uint8_t parameter_pata[7];//450 en centigrados
    uint8_t parameter_patb[7];// -50 en centigrados
    uint8_t parameter_paha[7];//900 en humedad
    uint8_t parameter_pahb[7];//parametro alarma humedad baja
    uint8_t parameter_papb[7];//parametro alarma peso bajo

}parameter_download_json_t;

/** */
typedef struct
{
    uint64_t unix_timestamp;
    uint32_t rtc_seconds;
}unix_t;

/** */
typedef struct
{
    bool b_FOTA_first_download_flag;
    bool b_FOTA_download_flag;
    bool b_FOTA_first_write_file_flag;
    bool b_FOTA_write_file_flag;
}fota_t;

/*!
     \brief This function initializes the system variables that are going to be used

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Initialize_System(void);
/*!
     \brief This function initializes the watchdog to recover the system in case of a failure happen.

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */

void create_file_weight(uint8_t *filename ,uint32_t adc);

void download_files_adc(uint8_t *filename,uint32_t*data_file);
void create_CRC(int16_t data_st ,int16_t data_sh, int32_t data_sp, int32_t* CRC);
void Add_end_format(void);
uint8_t check_button(void);
void Buff_capturas(int32_t buff_value, uint32_t data_timestamp);

next_state_t SM_Initialize_Watch_Dog(void);
/*!
     \brief This function checks for any changes into the broker to update alarms.

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_FOTA(void);
/*!
     \brief This function checks for any changes into the broker to update alarms.

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_FOTA_Download(void);
/*!
     \brief This function checks for any changes into the broker to update alarms.

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_FOTA_Write(void);
/*!
     \brief This function initializes and turns on the celullar module.

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Turn_On_CM_Q03(void);
/*!
     \brief This function gets the IMEI

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Get_IMEI(void);
/*!
     \brief This function sets an APN

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Set_APN(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Configure_Network(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Configure_SSL(void);
/*!
     \brief This function configures MQTT

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Configure_MQTT(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_File_Extract_Ptrs(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
void Buz_to_sound(uint8_t veces, uint32_t tiempo_delay);
void blink_leds(uint8_t op);

next_state_t SM_Initialize_Peripherals(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Read_Peripherals(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Deinitialize_Peripherals(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Network_Registration(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Reset_Network(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Get_Timestamp(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Get_EPS_Reg_Status(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Get_Signal_Quality(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Data_Format(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Activate_PDP_Context(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Deactivate_PDP_Context(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_HTTP_First_Transmission(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_MQTT_Open(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_MQTT_Connect(void);
/*!
     \brief This function reads the RTC timestamp

     \param[out] void Structure to the variable where timestamp will be stored.

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Capture_RTC(void);
/*!
     \brief This function reads the voltage from the battery

     \param[out] void Structure to the variable where the temperature will be
                             stored. The voltage is given in hundredth.

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Read_Voltage(void);
/*!
     \brief This function reads the temperature from the sensor

     \param[out] void Structure to the variable where the temperature will be
                             stored. The temperature is given in hundredth.

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Read_Temperature(void);
/*!
     \brief This function reads the humidity from the sensor

     \param[out] void Structure to the variable where the humidity will be
                             stored. The humidity is given in hundredth of
                             percentage.

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Read_Humidity(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Read_CO2(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Read_Voltage(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Read_Parameters(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_File_Query(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_File_Download(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_MQTT_Publish_Old_Data(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_File_Delete(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_File_Update_Read_Ptr_File(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_MQTT_Publish_New_Data(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_MQTT_Connection_State(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Subscribe_MQTT(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_File_Query_Av_Space(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_File_Free_Space(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_File_Overwrite(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_File_Upload(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_File_Update_Both_File_Ptr(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Control_On_Off(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Control_Dimming(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Wait_Event(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Alarm_Event(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Message_MQTT_Received(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Download_Parameters (void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Connection_Status(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Turn_Off_CM_Q03(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Sleep_Mode(void);
/*!
     \brief This function

     \param[out] void

     \return This function indicates if the task was successful, or if an error
             occurred.
 */
next_state_t SM_Reset_System(void);
/*!
     \brief This function sets the control of the system via pointers from one function to another

     \param[out] void

     \return void
 */
void SM_start_state_machine(void);

#endif /* STATE_MACHINE_H_ */
