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
 * \file       fuel_gauge_lite.h
 * \brief      This is the header file for the fuel gauge driver made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.
 * \version    1.1.1
 * \author     S4IoT
 * \copyright  (c) 2018, SOLUTIONS 4 IOT. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

////////////////////////////////Include Guard///////////////////////////////////
#ifndef FUEL_GAUGE_LITE_H_
#define FUEL_GAUGE_LITE_H_
/////////////////////////////End of Include Guard///////////////////////////////

////////////////////////////////Includes Area///////////////////////////////////
#include "init_config_functions.h"
///////////////////////////End of Includes Area/////////////////////////////////

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))
#include "stdint.h"

/*!
 	 \brief Enumerator to set the state of the voltage divider,
 	 	 	 and the fuel gauge.
 */
typedef enum
{
	fuel_gauge_on,			/*!< Fuel gauge supply voltage on*/
	voltage_divider_on,		/*!< Voltage divider on*/
	both_voltage_on,		/*!< Both fuel gauge supply voltage and voltage divider on*/
	fuel_gauge_off,			/*!< Fuel gauge supply voltage off*/
	voltage_divider_off,	/*!< Voltage divider off*/
	both_voltage_off		/*!< Both fuel gauge supply voltage and voltage divider off*/
}voltage_state_t;

/*!
 	 \brief Enumerator to get the state of either the voltage divider or
 	 	 	 	the fuel gauge.
 */
typedef enum
{
	fuel_gauge_vdd,	/*!< Gets the state of the fuel gauge*/
	voltage_divider	/*!< Gets the state of the voltage divider*/
}voltage_source_t;

/*!
 	 \brief This function turns on either the fuel gauge or the voltage divider
 	 	 	 for the battery temperature.

 	 \param[in] voltage_state State that the fuel gauge and/or the voltage divider for the
 	 	 	 	 	 	 	 	 battery temperature will have.

 	 \note The states of the fuel gauge and the voltage divider are not excluding.
 	 	 	 i.e. using both_voltage_on has the same effect as calling the function
 	 	 	 first with fuel_gauge_on and then with voltage_divider_on.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t FuelGauge_Voltage_Control(voltage_state_t voltage_state);

/*!
 	 \brief This function gets the state of the voltage divider or the fuel
			gauge.

 	 \param[in] source Whether the state fetched will be the voltage divider state
 	 	 	 	 	 	 or the fuel gauge state.
 	 \param[out] voltage_state The state of source.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t FuelGauge_Voltage_Get_State(voltage_source_t source, voltage_state_t *voltage_state);

/*!
 	 \brief This function reads the voltage given by the battery.

 	 \param[out] FuelGauge_voltage Pointer to the variable where the read
 	 	 	 	 voltage will be stored. The value given is the voltage in mV.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t FuelGauge_ReadVoltage(uint32_t* FuelGauge_voltage);

/*!
 	 \brief This function reads the current given by the battery.

 	 \param[out] FuelGauge_Current Pointer to the variable where the read
 	 	 	 	 current will be stored. The value given is the current in uA.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t FuelGauge_ReadCurrent(int32_t* FuelGauge_Current);

/*!
 	 \brief This function reads the temperature of the battery.

 	 \param[in] battery_temperature Pointer to the variable where the read value will
 	 	 	 	 be stored. The value received must be divided by 100
 	 	 	 	 to be in Celsius.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t BatteryTemperature_Read(uint16_t* battery_temperature);

#endif
#endif /* FUEL_GAUGE_LITE_H_ */
