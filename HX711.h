/*
 * HX711.h
 *
 *  Created on: 7 abr. 2023
 *      Author: S4IOT
 */

#ifndef HX711_H_
#define HX711_H_

#include "init_config_functions.h"
#include "state_machine.h"

typedef enum{
    gain_128=1,
    gain_64,
    gain_32

}gain_options;

typedef enum
{
    peso_0,
    peso_1,
    peso_2,
    peso_3

}select_weight;

void set_Gain(gain_options gain);
void hx711_init(Comunication_pins pin_set);
void hx711_start(void);
void hx711_deinit(void);
uint8_t read_input_state(void);
general_codes_t hx711_read(uint32_t* data_out);


general_codes_t Read_Data(uint32_t* data_ciclo);
general_codes_t Get_Weight(int32_t* weight);
general_codes_t Calibracion_pesos(select_weight peso,uint32_t *var_peso);
void calibration_factor(void);
void download_parameters_cali(uint32_t *p0,uint32_t *p1, uint32_t* p2);

#endif /* HX711_H_ */
