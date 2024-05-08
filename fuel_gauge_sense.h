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
 * \file       fuel_gauge_sense.h
 * \brief      This is the header file for the fuel gauge driver made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.
 * \version    1.1.1
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

#ifndef FUEL_GAUGE_SENSE_H_
#define FUEL_GAUGE_SENSE_H_

#include "init_config_functions.h"

#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
#include "stdint.h"

/*!
 	 \brief This function reads the external battery temperature measured
 	 	 	 by the Fuel Gauge.

 	 \param[out] temperature Pointer to the variable where the temperature
 	 	 	 	 	 	 	 will be stored.

 	 \return This function indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t FuelGauge_ReadTemperature(uint16_t *temperature);

/*!
 	 \brief This function reads the voltage given by the battery.

 	 \param[out] voltage Pointer to the variable where the voltage
 	 	 	 	 	 	 will be stored.

 	 \return This function indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t FuelGauge_ReadVoltage(uint16_t *voltage);

/*!
 	 \brief This function reads the internal temperature of the battery.

 	 \param[out] int_temperature Pointer to the variable where the internal
 	 	 	 	 	 	 	 	 temperature will be stored.

 	 \return This function indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t FuelGauge_ReadInternalTemperature(uint16_t *int_temperature);

/*!
 	 \brief This function allows the gauge to enter the hibernate power mode
 	 	 	 after a transition to sleep power mode is detected.

 	 \return This function indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t FuelGauge_SetHibernate(void);

/*!
 	 \brief This function prevents the gauge from entering the hibernate power
 	 	 	 mode after a transition to the sleep power mode is detected.

 	 \return This function indicates if an error occurred.
 			 Returns 0 for success.
 */
general_codes_t FuelGauge_ClearHibernate(void);

/*!
 	 \brief This function allows the gauge to enter the sleep power mode
 	 	 	 when the current is lower than the sleep current (10 mA)

 	 \return This function indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t FuelGauge_SetSleep(void);

/*!
 	 \brief This function prevents the gauge to enter the sleep
 	 	 	 power mode when the current is lower than the sleep
 	 	 	 current (10 mA)

 	 \return This function indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t FuelGauge_ClearSleep(void);

/*!
 	 \brief This function performs a full reset to the fuel gauge.

 	 \return This function indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t FuelGauge_Reset(void);

#endif
#endif /* FUEL_GAUGE_SENSE_H_ */
