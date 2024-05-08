/*
 * SHT20.h
 *
 *  Created on: 7 abr. 2023
 *      Author: S4IOT
 */

#ifndef SHT20_H_
#define SHT20_H_
#include "init_config_functions.h"

typedef enum
{
    temperature,
    humidity
}read_var;

general_codes_t SHT20Read(read_var op, int16_t* var);
//general_codes_t SHT20ReadTemperature(i2c_alternatives_t I2C, int16_t* temp);
//general_codes_t SHT20ReadHumedad(i2c_alternatives_t I2C, uint16_t* hum);

#endif /* SHT20_H_ */
