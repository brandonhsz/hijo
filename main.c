/*
 * Copyright 2016-2021 NXP
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
 
/**
 * @file    Plantica_Maqueta_v1.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "pin_mux.h"
#include "clock_config.h"
#include "init_config_functions.h"
#include "Watchdog.h"
//#include "HX711.h"
//#include <math.h>

//#include "CM_Q03_File.h"//quitar

//#include "MKL17Z4.h"
/* TODO: insert other include files here. */
#include "state_machine.h"

/* TODO: insert other definitions and declarations here. */
/*
 * @brief   Application entry point.
 */
int main(void) {
    BOARD_InitBootClocks();
	InitConfigBoard();

//    GPIO_Initialization(Port_E0,Port_Output);
    LED_Control(1);
//    GPIO_Write(Port_E0,0);

    Watchdog_initModuleConfiguration();
    Watchdog_setDifferentTimer(DogTime_262144ms);
    Watchdog_dogAlert();
    Watchdog_feedTheDog(normalFood);

//    Comunication_pins data_pin={
//        Port_C3,//input
//        Port_C4//output
//    };
//
//    uint32_t data_info;
//
//    while(1){
//        hx711_init(data_pin);
//        VDD_Sensors_Control(VDD_on);
//        Delay(3000000);
//        set_Gain(gain_128);
//        data_info = hx711_read();
//        printf("adc: %d\n", data_info);
//        Delay(400000);
//        VDD_Sensors_Control(VDD_off);
//        Delay(3000000);
//    }

//    VDD_Sensors_Control(VDD_on);
//    Delay(5000000);
//    ADC_init_t adc_config;
//    adc_config.resolution=ADC_Resolution16;
//    adc_config.enableLowPower=low_power_mode_disabled;
//    adc_config.enableContinuousConversion=continuous_conversion_disabled;
//    ADC_Initialization(adc_config, Personal_Configuration);
//    uint16_t adc_temperature=0;
//    float voltaje_temperatura=0;
//    float temperatura_atlas=0;
////    float voltaje_res2;
//    float corriente;
//    float resistencia_sensor;
//
//    while(1){
//    //lectura ADC_ sensor temperatura2
//    ADC_Read(&adc_temperature);
//    voltaje_temperatura=((3.3f * adc_temperature)/65536.0f);
//   corriente =(3.3f-voltaje_temperatura)/986.0f;
////   corriente=voltaje_res2/986;
//   resistencia_sensor = voltaje_temperatura/corriente;
//   temperatura_atlas =-1*((sqrt((-0.00232f*resistencia_sensor)+17.59246f)-3.908)/0.00116f);
//    temperatura_atlas =0;
//    voltaje_temperatura=0;
//    }

//#define DECIMAL_BASE                    (10)
//#define FILE_CREATE_TEMP_BUFF_SIZE      (6)
//#define FILE_PREFIX_SIZE    (10)
//#define FILE_EXT_SIZE   (5)
//#define MAX_FILES_N_CHARACTERS (6)
//
//void SM_Create_File_Name(const uint8_t* prefix, uint16_t prefix_size,
//            uint16_t file_number, const uint8_t* file_extension,
//            uint16_t file_extension_size, uint8_t *new_file_name,
//            uint16_t *new_file_name_size)
//    {
//        uint8_t tmp_buff[FILE_CREATE_TEMP_BUFF_SIZE] = {0};
//
//        strcat((char*)(new_file_name), (const char*)(prefix));
//        (*new_file_name_size) = prefix_size;
//
//        itoa((int)(file_number), (char*)(tmp_buff), DECIMAL_BASE);
//        strcat((char *)(new_file_name), (const char*)(tmp_buff));
//        (*new_file_name_size) += strlen((const char*)(tmp_buff));
//
//        strcat((char *)(new_file_name), (const char*)(file_extension));
//        (*new_file_name_size) += file_extension_size;
//    }
//
//    uint8_t calibracion_file[MAX_FILES_N_CHARACTERS + FILE_PREFIX_SIZE + FILE_EXT_SIZE] = {0};
//    uint16_t calibracion_file_size = 0;
//   uint8_t  msg_cali_to_be_sent[]="cero_4355666\0";
//   uint8_t msg_cali_size=13;
//   uint8_t file_ext[FILE_EXT_SIZE] = ".txt\0";
//    uint8_t file_prefix[FILE_PREFIX_SIZE] = "cali_cero\0";
//    uint16_t file_write_cali_ptr = 0;
//
//    SM_Create_File_Name(file_prefix, FILE_PREFIX_SIZE, file_write_cali_ptr, file_ext,
//       FILE_EXT_SIZE, calibracion_file, &calibracion_file_size);
//
//  CM_Q03_File_Upload_File(calibracion_file, calibracion_file_size,
//            msg_cali_to_be_sent, msg_cali_size);


    Delay(3000000);

	SM_start_state_machine();
    while(1) {

    }
    return 0 ;
}
