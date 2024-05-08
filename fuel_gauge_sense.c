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

#include "fuel_gauge_sense.h"

#if(defined(ATHEN_BOARD) && ATHEN_BOARD)

#define IS_INIT					(0)
#define NOT_INIT				(1)

/** Timeout for the I2C*/
#define TIMEOUT					(300000)

#define FUEL_GAUGE_ADDR			(0x55)
#define REG_ADDR_LEN			(0x01)
#define COMMAND_ADDR_LEN		(0x02)
#define READ_DATA_LEN			(0x02)

#define TEMP_REG_ADDR			(0x06)
#define VOLT_REG_ADDR			(0x08)
#define AVRG_CURRENT_REG_ADDR	(0x14)
#define INST_CURRENT_REG_ADDR	(0x22)
#define INT_TEMP_REG_ADDR		(0x28)
#define SET_HIBERNATE_ADDR		(0x11)
#define CLEAR_HIBERNATE_ADDR	(0x12)
#define SET_SLEEP_ADDR			(0x13)
#define CLEAR_SLEEP_ADDR		(0x14)
#define RESET_REG_ADDR			(0x41)
#define CONTROL_REG_ADDR		(0x00)

uint8_t I2C0_Fuel_init = NOT_INIT;
extern i2c_master_handle_t i2c_handle;
extern bool pit_channel_1_flag;
extern volatile status_t g_master_completion_flag;

static uint32_t wait_for_transfer(void);
extern void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData);

general_codes_t FuelGauge_ReadTemperature(uint16_t *temperature)
{
	uint8_t rx_data[2] = {0};
	uint8_t tx_data = TEMP_REG_ADDR;
	uint32_t retval = 0;
	i2c_master_transfer_t tx_struct = {0};

	tx_struct.slaveAddress = FUEL_GAUGE_ADDR;
	tx_struct.direction = kI2C_Write;
	tx_struct.data = &tx_data;
	tx_struct.dataSize = REG_ADDR_LEN;
	tx_struct.subaddress = 0;
	tx_struct.subaddressSize = 0;
	tx_struct.flags = kI2C_TransferDefaultFlag;

	retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &tx_struct);

	if(0 != retval)
	{
		return retval;
	}

	retval = wait_for_transfer();

	if(0 != retval)
	{
		return retval;
	}

	Delay(50000);

	tx_struct.direction = kI2C_Read;
	tx_struct.data = rx_data;
	tx_struct.dataSize = READ_DATA_LEN;

	retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &tx_struct);

	if(0 != retval)
	{
		return retval;
	}

	retval = wait_for_transfer();

	if(0 != retval)
	{
		return retval;
	}

	Delay(50000);

	*temperature = ((rx_data[1] << 8) | rx_data[0]);
	*temperature = ((*temperature) - 2731.5f) * 10.0f;

	return retval;
}

general_codes_t FuelGauge_ReadVoltage(uint16_t *voltage)
{
	uint8_t rx_data[2] = {0};
	uint8_t tx_data = VOLT_REG_ADDR;
	uint32_t retval = 0;
	i2c_master_transfer_t tx_struct = {0};

	tx_struct.slaveAddress = FUEL_GAUGE_ADDR;
	tx_struct.direction = kI2C_Write;
	tx_struct.data = &tx_data;
	tx_struct.dataSize = REG_ADDR_LEN;
	tx_struct.subaddress = 0;
	tx_struct.subaddressSize = 0;
	tx_struct.flags = kI2C_TransferDefaultFlag;

	retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &tx_struct);

	if(0 != retval)
	{
		return retval;
	}

	retval = wait_for_transfer();

	if(0 != retval)
	{
		return retval;
	}

	Delay(50000);

	tx_struct.direction = kI2C_Read;
	tx_struct.data = rx_data;
	tx_struct.dataSize = READ_DATA_LEN;

	retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &tx_struct);

	if(0 != retval)
	{
		return retval;
	}

	retval = wait_for_transfer();

	if(0 != retval)
	{
		return retval;
	}

	Delay(50000);

	(*voltage) = ((rx_data[1] << 8) | rx_data[0]);

	return retval;
}

general_codes_t FuelGauge_ReadInternalTemperature(uint16_t *int_temperature)
{
	uint8_t rx_data[2] = {0};
	uint8_t tx_data = INT_TEMP_REG_ADDR;
	uint32_t retval = 0;
	i2c_master_transfer_t tx_struct = {0};

	tx_struct.slaveAddress = FUEL_GAUGE_ADDR;
	tx_struct.direction = kI2C_Write;
	tx_struct.data = &tx_data;
	tx_struct.dataSize = REG_ADDR_LEN;
	tx_struct.subaddress = 0;
	tx_struct.subaddressSize = 0;
	tx_struct.flags = kI2C_TransferDefaultFlag;

	retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &tx_struct);

	if(0 != retval)
	{
		return retval;
	}

	retval = wait_for_transfer();

	if(0 != retval)
	{
		return retval;
	}

	Delay(50000);

	tx_struct.direction = kI2C_Read;
	tx_struct.data = rx_data;
	tx_struct.dataSize = READ_DATA_LEN;

	retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &tx_struct);

	if(0 != retval)
	{
		return retval;
	}

	retval = wait_for_transfer();

	if(0 != retval)
	{
		return retval;
	}

	Delay(50000);

	(*int_temperature) = ((rx_data[1] << 8) | rx_data[0]);

	return retval;
}

general_codes_t FuelGauge_SetHibernate(void)
{
	uint32_t retval = 0;
	uint8_t tx_data[2] = {CONTROL_REG_ADDR};
	i2c_master_transfer_t tx_struct = {0};

	tx_struct.slaveAddress = FUEL_GAUGE_ADDR;
	tx_struct.direction = kI2C_Write;
	tx_struct.data = tx_data;
	tx_struct.dataSize = REG_ADDR_LEN;
	tx_struct.subaddress = 0;
	tx_struct.subaddressSize = 0;
	tx_struct.flags = kI2C_TransferDefaultFlag;

	retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &tx_struct);

	if(0 != retval)
	{
		return retval;
	}

	retval = wait_for_transfer();

	if(0 != retval)
	{
		return retval;
	}

	Delay(50000);

	tx_data[0] = SET_HIBERNATE_ADDR;

	tx_struct.data = tx_data;
	tx_struct.dataSize = COMMAND_ADDR_LEN;

	retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &tx_struct);

	if(0 != retval)
	{
		return retval;
	}

	retval = wait_for_transfer();

	if(0 != retval)
	{
		return retval;
	}

	Delay(50000);

	return retval;
}

general_codes_t FuelGauge_ClearHibernate(void)
{
	uint32_t retval = 0;
	uint8_t tx_data[2] = {CONTROL_REG_ADDR};
	i2c_master_transfer_t tx_struct = {0};

	tx_struct.slaveAddress = FUEL_GAUGE_ADDR;
	tx_struct.direction = kI2C_Write;
	tx_struct.data = tx_data;
	tx_struct.dataSize = REG_ADDR_LEN;
	tx_struct.subaddress = 0;
	tx_struct.subaddressSize = 0;
	tx_struct.flags = kI2C_TransferDefaultFlag;

	retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &tx_struct);

	if(0 != retval)
	{
		return retval;
	}

	retval = wait_for_transfer();

	if(0 != retval)
	{
		return retval;
	}

	Delay(50000);

	tx_data[0] = CLEAR_HIBERNATE_ADDR;

	tx_struct.data = tx_data;
	tx_struct.dataSize = COMMAND_ADDR_LEN;

	retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &tx_struct);

	if(0 != retval)
	{
		return retval;
	}

	retval = wait_for_transfer();

	if(0 != retval)
	{
		return retval;
	}

	Delay(50000);

	return retval;
}

general_codes_t FuelGauge_SetSleep(void)
{
	uint32_t retval = 0;
	uint8_t tx_data[2] = {CONTROL_REG_ADDR};
	i2c_master_transfer_t tx_struct = {0};

	tx_struct.slaveAddress = FUEL_GAUGE_ADDR;
	tx_struct.direction = kI2C_Write;
	tx_struct.data = tx_data;
	tx_struct.dataSize = REG_ADDR_LEN;
	tx_struct.subaddress = 0;
	tx_struct.subaddressSize = 0;
	tx_struct.flags = kI2C_TransferDefaultFlag;

	retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &tx_struct);

	if(0 != retval)
	{
		return retval;
	}

	retval = wait_for_transfer();

	if(0 != retval)
	{
		return retval;
	}

	Delay(50000);

	tx_data[0] = SET_SLEEP_ADDR;

	tx_struct.data = tx_data;
	tx_struct.dataSize = COMMAND_ADDR_LEN;

	retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &tx_struct);

	if(0 != retval)
	{
		return retval;
	}

	retval = wait_for_transfer();

	if(0 != retval)
	{
		return retval;
	}

	Delay(50000);

	return retval;
}

general_codes_t FuelGauge_ClearSleep(void)
{
	uint32_t retval = 0;
	uint8_t tx_data[2] = {CONTROL_REG_ADDR};
	i2c_master_transfer_t tx_struct = {0};

	tx_struct.slaveAddress = FUEL_GAUGE_ADDR;
	tx_struct.direction = kI2C_Write;
	tx_struct.data = tx_data;
	tx_struct.dataSize = REG_ADDR_LEN;
	tx_struct.subaddress = 0;
	tx_struct.subaddressSize = 0;
	tx_struct.flags = kI2C_TransferDefaultFlag;

	retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &tx_struct);

	if(0 != retval)
	{
		return retval;
	}

	retval = wait_for_transfer();

	if(0 != retval)
	{
		return retval;
	}

	Delay(50000);

	tx_data[0] = CLEAR_SLEEP_ADDR;

	tx_struct.data = tx_data;
	tx_struct.dataSize = COMMAND_ADDR_LEN;

	retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &tx_struct);

	if(0 != retval)
	{
		return retval;
	}

	retval = wait_for_transfer();

	if(0 != retval)
	{
		return retval;
	}

	Delay(50000);

	return retval;
}

general_codes_t FuelGauge_Reset(void)
{
	uint32_t retval = 0;
	uint8_t tx_data[2] = {CONTROL_REG_ADDR};
	i2c_master_transfer_t tx_struct = {0};

	tx_struct.slaveAddress = FUEL_GAUGE_ADDR;
	tx_struct.direction = kI2C_Write;
	tx_struct.data = tx_data;
	tx_struct.dataSize = REG_ADDR_LEN;
	tx_struct.subaddress = 0;
	tx_struct.subaddressSize = 0;
	tx_struct.flags = kI2C_TransferDefaultFlag;

	retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &tx_struct);

	if(0 != retval)
	{
		return retval;
	}

	retval = wait_for_transfer();

	if(0 != retval)
	{
		return retval;
	}

	Delay(50000);

	tx_data[0] = RESET_REG_ADDR;

	tx_struct.data = tx_data;
	tx_struct.dataSize = COMMAND_ADDR_LEN;

	retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &tx_struct);

	if(0 != retval)
	{
		return retval;
	}

	retval = wait_for_transfer();

	if(0 != retval)
	{
		return retval;
	}

	Delay(50000);

	return retval;
}

static uint32_t wait_for_transfer(void)
{
	uint32_t retval;
	g_master_completion_flag = 0xFFFF;

    pit_channel_1_flag = false;

	/* Set timer period for channel 1 */
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, USEC_TO_COUNT(TIMEOUT, CLOCK_GetFreq(kCLOCK_BusClk)));
	/* Start channel 1 */
	PIT_StartTimer(PIT, kPIT_Chnl_1);


	// Wait the PIT interruption
	//  Wait for transfer completed.
	while (g_master_completion_flag == 0xFFFF && !(pit_channel_1_flag));

    // Stop PIT count
	PIT_StopTimer(PIT, kPIT_Chnl_1);

	if(pit_channel_1_flag)
	{

		pit_channel_1_flag = false;
		retval = 0x01ff;
	}

	else
	{

		pit_channel_1_flag = false;
		retval = g_master_completion_flag;
	}

    g_master_completion_flag = 0xFFFF;

    return retval;
}

#endif
