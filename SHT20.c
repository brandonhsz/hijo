/*
 * SHT20.c
 *
 *  Created on: 7 abr. 2023
 *      Author: S4IOT
 */
#include "SHT20.h"
#define reg_address         (0x40)




general_codes_t SHT20Read(read_var variable,int16_t* var){

    uint8_t hum_reg=0xF5;
    uint8_t temp_reg=0xF3;
    general_codes_t status;
    uint8_t read_data_temp[2];
    uint8_t read_data_hum[2];
    float decimal=0;

    if(variable==humidity){                                      //op1 es para hum
        memset(read_data_hum,'\0',2);
        status=I2C_Write(I2C_Alt0,reg_address, &hum_reg,1);
        Delay(100000);
        status= I2C_Read(I2C_Alt0,reg_address,read_data_hum,2);
        if(successful_task!=status){
            return status;
        }
        Delay(500000);
        decimal = (125.0f * (((read_data_hum[0]<<8 | read_data_hum[1]))/65536.0f))-6.0f;
        memset(read_data_hum,'\0',2);

    }else{//op0 es para temp

        memset(read_data_temp,'\0',2);
        status=I2C_Write(I2C_Alt0,reg_address, &temp_reg,1);

        Delay(100000);

        status= I2C_Read(I2C_Alt0,reg_address,read_data_temp,2);
        if(successful_task!=status){
            return status;
        }
        Delay(500000);
        decimal = (175.72f * (((read_data_temp[0]<<8 | read_data_temp[1]))/65536.0f))-46.85f;
        memset(read_data_temp,'\0',2);

    }

    (*var)=decimal*10;
    decimal=0;
    return status;
}



//general_codes_t SHT20ReadTemperature(i2c_alternatives_t I2C, int16_t* temp){
//
//    uint8_t temp_reg=0xF3;
//    general_codes_t status;
//    uint8_t read_data_temp[2];
//    float decimal_temp=0;
//
//    status=I2C_Write(I2C,reg_address, &temp_reg,1);
//
//    Delay(100000);
//
//    status= I2C_Read(I2C,reg_address,read_data_temp,2);
//    if(successful_task!=status){
//        return status;
//    }
//    Delay(500000);
//    decimal_temp = (175.72f * (((read_data_temp[0]<<8 | read_data_temp[1])+1.0f)/65536.0f))-46.85f;
//
//    (*temp)=decimal_temp*10;
//    return status;
//}
//
//general_codes_t SHT20ReadHumedad(i2c_alternatives_t I2C, uint16_t* hum){
//
//    uint8_t hum_reg=0xF5;
//    general_codes_t status;
//    uint8_t read_data_hum[2];
//    float decimal_hum=0;
//
//    status=I2C_Write(I2C,reg_address, &hum_reg,1);
//
//    Delay(100000);
//
//    status= I2C_Read(I2C,reg_address,read_data_hum,2);
//    if(successful_task!=status){
//        return status;
//    }
//    Delay(500000);
//    decimal_hum = (125.0f * (((read_data_hum[0]<<8 | read_data_hum[1])+1.0f)/65536.0f))-6.0f;
//
//    (*hum)=decimal_hum*10;
//    return status;
//}
