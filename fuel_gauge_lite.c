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

#include "fuel_gauge_lite.h"
#include "fsl_gpio.h"

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))

#include "fsl_pit.h"

/** Defines the I2C as initialized*/
#define IS_INIT				(0)
/** Defines the I2C as not initialized*/
#define NOT_INIT			(1)
/** Address of the Fuel Gage device*/
#define FUEL_GAUGE_ADDR 	(0x70)

/** Length of the data to be sent*/
#define TX_DATA_LEN			(0x02)
/** Length of the data to be received*/
#define RX_DATA_LEN			(0x02)
/** Length of the address of a register*/
#define REG_ADDR_LEN		(1)

/** Timeout for the I2C*/
#define TIMEOUT				(3000000)
/** Masks the bit 13 for the current sign bit*/
#define BIT13_MASK			(0x02000)
/** Shift for the current sign bit*/
#define BIT13_SHIFT			(13)
/** Mask for the xor for cpl-2*/
#define XOR_MASK			(0x01FFF)
/** Defines the sign for the current*/
#define SIGN_VALUE			(-1)

enum actions{
    OFF,
    ON,
};

enum times
{
	first_time,
	second_time
};

struct FG_flags
{
	uint8_t fuel_gauge_status: 1;
	uint8_t voltage_divider_status :1;
	enum times first_call:1;
};

////////////////////////// VARIABLES /////////////////////////////////////
/** Variable to set whether the header's I2C are initialized or not */
extern uint8_t I2C_headers_isInit;
/** Variable to check whether the Fuel Gage's I2C has been initialized*/
uint8_t FuelGauge_isInit = NOT_INIT;
static volatile status_t g_master_completion_flag = 0xFFFF;
static i2c_master_handle_t i2c_handle;
extern bool pit_channel_1_flag;

static struct FG_flags FG_status =
{
		OFF,
		OFF,
		first_time
};

static port_pin_config_t config_port_I2C = {
	kPORT_PullUp,
	kPORT_FastSlewRate,
	kPORT_PassiveFilterDisable,
	kPORT_LowDriveStrength,
	kPORT_MuxAlt4
};
////////////////////////// VARIABLES /////////////////////////////////////

/** Waits for the I2C transfer to be completed*/
static uint32_t wait_for_transfer(void);
static void i2c_fuel_gauge_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData);
static general_codes_t FuelGauge_Initialization(void);
static general_codes_t FuelGauge_TurnOn(void);
static general_codes_t FuelGauge_TurnOff(void);
static general_codes_t BatteryTemperature_Initialize(void);

static general_codes_t FuelGauge_Initialization(void)
{
	general_codes_t retval = already_initialized;

	/** If the I2C pins are already initialized*/
	if (NOT_INIT == FuelGauge_isInit && ON == FG_status.fuel_gauge_status)
	{
		pit_config_t pitConfig;
		i2c_master_config_t i2c_master_config_struct;

		/** I2C pin configuration*/
		port_pin_config_t config_port_I2C = {
			kPORT_PullUp,
			kPORT_FastSlewRate,
			kPORT_PassiveFilterDisable,
			kPORT_LowDriveStrength,
			kPORT_MuxAlt4
		};

		/** If the header's I2C is initialized*/
		if(IS_INIT == I2C_headers_isInit)
		{
			/** Deinitializes the respective pins*/
			PORT_SetPinMux(PORTB, PIN0_IDX, kPORT_PinDisabledOrAnalog);
			PORT_SetPinMux(PORTB, PIN1_IDX, kPORT_PinDisabledOrAnalog);
		}

		/* pitConfig.enableRunInDebug = false; */
		PIT_GetDefaultConfig(&pitConfig);
		/* Init pit module */
		PIT_Init(PIT, &pitConfig);
		/* Enable timer interrupts for channel 1 */
		PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
		/* Enable at the NVIC */
		EnableIRQ(PIT_IRQn);

		CLOCK_EnableClock(kCLOCK_PortE);

		PORT_SetPinConfig(PORTE,PIN18_IDX, &config_port_I2C);
		PORT_SetPinConfig(PORTE,PIN19_IDX, &config_port_I2C);

		/** Initializes the I2C*/
		I2C_MasterGetDefaultConfig(&i2c_master_config_struct);
		// Configuration of I2C parameters
		I2C_MasterInit(I2C0, &i2c_master_config_struct, CLOCK_GetFreq(I2C0_CLK_SRC));

		memset(&i2c_handle, 0, sizeof(i2c_handle));

		I2C_MasterTransferCreateHandle(I2C0, &i2c_handle, i2c_fuel_gauge_callback, NULL);

		FuelGauge_isInit = IS_INIT;
		I2C_headers_isInit = NOT_INIT;

		Delay(5000);

		retval = successful_task;
	}

	else if(OFF == FG_status.fuel_gauge_status)
	{
		return FUEL_GAUGE_not_turned_on;
	}

	return retval;
}

general_codes_t FuelGauge_Voltage_Control(voltage_state_t voltage_state)
{
	gpio_pin_config_t config_PE30;
	gpio_pin_config_t config;

	general_codes_t retval;

	if(first_time == FG_status.first_call)
	{
		/** Enables clock gating*/
		CLOCK_EnableClock(kCLOCK_PortD);
		CLOCK_EnableClock(kCLOCK_PortB);
		CLOCK_EnableClock(kCLOCK_PortE);

		/** Sets the state on the pin*/
		config.pinDirection = kGPIO_DigitalOutput;
		config.outputLogic = OFF;

		config_PE30.pinDirection= kGPIO_DigitalOutput;
		config_PE30.outputLogic = OFF;

		PORT_SetPinMux(PORTE, PIN30_IDX, kPORT_MuxAsGpio);
		GPIO_PinInit(GPIOE, PIN30_IDX, &config_PE30);

		PORT_SetPinMux(PORTD, PIN7_IDX, kPORT_MuxAsGpio);
		GPIO_PinInit(GPIOD, PIN7_IDX, &config);

		/** This is to only initialize the GPIO once*/
		FG_status.first_call = second_time;
	}

	switch(voltage_state)
	{
		case fuel_gauge_on:
			/** Turns on the voltage for the I2C*/
		    GPIO_PinWrite(GPIOE, PIN30_IDX, ON);

			FG_status.fuel_gauge_status = ON;

		    retval = FuelGauge_TurnOn();

			if((successful_task != retval) && (already_initialized != retval))
			{
				return retval;
			}
		break;

		case voltage_divider_on:
		    GPIO_PinWrite(GPIOD, PIN7_IDX, ON);

			FG_status.voltage_divider_status = ON;

			retval = BatteryTemperature_Initialize();
		break;

		case both_voltage_on:
			/** Turns on the voltage for the I2C*/
		    GPIO_PinWrite(GPIOE, PIN30_IDX, ON);
		    GPIO_PinWrite(GPIOD, PIN7_IDX, ON);

			FG_status.fuel_gauge_status = ON;
			FG_status.voltage_divider_status = ON;

		    retval = FuelGauge_TurnOn();

			if((successful_task != retval) && (already_initialized != retval))
			{
				return retval;
			}
		break;

		case fuel_gauge_off:

		    GPIO_PinWrite(GPIOE, PIN30_IDX, OFF);

		    retval = FuelGauge_TurnOff();

			if((successful_task != retval) && (already_initialized != retval))
			{
				return retval;
			}

			FG_status.fuel_gauge_status = OFF;
		break;

		case voltage_divider_off:
		    GPIO_PinWrite(GPIOD, PIN7_IDX, OFF);

			FG_status.voltage_divider_status = OFF;
		break;

		case both_voltage_off:

		    retval = FuelGauge_TurnOff();

		    GPIO_PinWrite(GPIOE, PIN30_IDX, OFF);
		    GPIO_PinWrite(GPIOD, PIN7_IDX, OFF);

			if((successful_task != retval) && (already_initialized != retval))
			{
				return retval;
			}

			FG_status.fuel_gauge_status = OFF;
			FG_status.voltage_divider_status = OFF;
		break;

		default:
			return parameter_error;
		break;
	}

	return successful_task;
}

general_codes_t FuelGauge_Voltage_Get_State(voltage_source_t source, voltage_state_t *voltage_state)
{
	if(fuel_gauge_vdd == source)
	{
		(*voltage_state) = FG_status.fuel_gauge_status;
	}

	else if(voltage_divider == source)
	{
		(*voltage_state) = FG_status.voltage_divider_status;
	}

	else
	{
		return parameter_error;
	}

	return successful_task;
}

/** Turns on the Fuel Gauge module*/
static general_codes_t FuelGauge_TurnOn(void)
{
	general_codes_t retval = successful_task;
    uint8_t tx_data[2];
    i2c_master_transfer_t i2c_master_transfer_struct = {0};

	/** Initializes the I2C in case it has been deinitialized*/
	retval = FuelGauge_Initialization();

	if((successful_task != retval) && (already_initialized != retval))
	{
		return retval;
	}

    Delay(50000);

	/** Data to reset the Fuel Gauge*/
    tx_data[0] = 1;
    tx_data[1] = 0x02;

    i2c_master_transfer_struct.slaveAddress = FUEL_GAUGE_ADDR;
    i2c_master_transfer_struct.direction = kI2C_Write;
    i2c_master_transfer_struct.data = tx_data;
    i2c_master_transfer_struct.dataSize = TX_DATA_LEN;
    i2c_master_transfer_struct.subaddress = 0;
    i2c_master_transfer_struct.subaddressSize = 0;
    i2c_master_transfer_struct.flags = kI2C_TransferDefaultFlag;


    retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &i2c_master_transfer_struct);
	g_master_completion_flag = 0xFFFF;
    if(0 != retval)
    {
    	return retval;
    }

    retval = wait_for_transfer();

    if(0 != retval)
    {
    	return retval;
    }


    Delay(5000);

	/** Data to start the Fuel Gauge counter*/
    tx_data[0] = 0;
    tx_data[1] = 0x10;

    retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &i2c_master_transfer_struct);
	g_master_completion_flag = 0xFFFF;
    if(0 != retval)
    {
    	return retval;
    }

    retval = wait_for_transfer();

    if(0 != retval)
    {
    	return retval;
    }


    Delay(5000);

    return retval;
}

/** Reads the foltage from the Fuel Gauge*/
general_codes_t FuelGauge_ReadVoltage(uint32_t* FuelGauge_voltage)
{
    uint8_t tx_data;
    uint8_t rx_data[2];
	uint16_t return_data = 0;
    i2c_master_transfer_t i2c_master_transfer_struct = {0};
    general_codes_t retval = successful_task;

	PORT_SetPinConfig(PORTE, PIN18_IDX, &config_port_I2C);
	PORT_SetPinConfig(PORTE, PIN19_IDX, &config_port_I2C);
	PORT_SetPinMux(PORTB, PIN0_IDX, kPORT_PinDisabledOrAnalog);
	PORT_SetPinMux(PORTB, PIN1_IDX, kPORT_PinDisabledOrAnalog);
	memset(&i2c_handle, 0, sizeof(i2c_handle));
	I2C_MasterTransferCreateHandle(I2C0, &i2c_handle, i2c_fuel_gauge_callback, NULL);

    Delay(50000);

	/** Address of the voltage register*/
    tx_data = 0x08;

    i2c_master_transfer_struct.slaveAddress = FUEL_GAUGE_ADDR;
    i2c_master_transfer_struct.direction = kI2C_Write;
    i2c_master_transfer_struct.data = &tx_data;
    i2c_master_transfer_struct.dataSize = REG_ADDR_LEN;
    i2c_master_transfer_struct.subaddress = 0;
    i2c_master_transfer_struct.subaddressSize = 0;
    i2c_master_transfer_struct.flags = kI2C_TransferDefaultFlag;

    retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &i2c_master_transfer_struct);
	g_master_completion_flag = 0xFFFF;
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

	/** Reads the register*/
    i2c_master_transfer_struct.direction = kI2C_Read;
    i2c_master_transfer_struct.data = rx_data;
    i2c_master_transfer_struct.dataSize = RX_DATA_LEN;

    retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &i2c_master_transfer_struct);
	g_master_completion_flag = 0xFFFF;
    if(0 != retval)
    {
    	return retval;
    }

    retval = wait_for_transfer();

    if(0 != retval)
    {
    	return retval;
    }


    Delay(5000);

	/** Turns the data into mV*/
    return_data = ((rx_data[1] << 8) | rx_data[0]);
    return_data &= 0x0FFF;
    *(FuelGauge_voltage) = return_data * 244; // return_data/100 -> mV
    *(FuelGauge_voltage) /= 100;

    return retval;
}

/** Reads the current of the Fuel Gauge module*/
general_codes_t FuelGauge_ReadCurrent(int32_t* FuelGauge_Current)
{
    uint8_t tx_data;
    uint8_t rx_data[2];
	int32_t return_data = 0;
    i2c_master_transfer_t i2c_master_transfer_struct = {0};
    general_codes_t retval = successful_task;
    uint8_t sign_bit = 0;

	PORT_SetPinConfig(PORTE, PIN18_IDX, &config_port_I2C);
	PORT_SetPinConfig(PORTE, PIN19_IDX, &config_port_I2C);
	PORT_SetPinMux(PORTB, PIN0_IDX, kPORT_PinDisabledOrAnalog);
	PORT_SetPinMux(PORTB, PIN1_IDX, kPORT_PinDisabledOrAnalog);
	memset(&i2c_handle, 0, sizeof(i2c_handle));
	I2C_MasterTransferCreateHandle(I2C0, &i2c_handle, i2c_fuel_gauge_callback, NULL);

	if((successful_task != retval) && (already_initialized != retval))
	{
		return retval;
	}

    Delay(50000);

	/** Address of the current register*/
	tx_data = 0x06;

    i2c_master_transfer_struct.slaveAddress = FUEL_GAUGE_ADDR;
    i2c_master_transfer_struct.direction = kI2C_Write;
    i2c_master_transfer_struct.data = &tx_data;
    i2c_master_transfer_struct.dataSize = REG_ADDR_LEN;
    i2c_master_transfer_struct.subaddress = 0;
    i2c_master_transfer_struct.subaddressSize = 0;
    i2c_master_transfer_struct.flags = kI2C_TransferDefaultFlag;

    Delay(500000);

    retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &i2c_master_transfer_struct);

    if(0 != retval)
    {
    	return retval;
    }

    retval = wait_for_transfer();

    if(0 != retval)
    {
    	return retval;
    }


    Delay(5000);

	/** Reads the register*/
    i2c_master_transfer_struct.direction = kI2C_Read;
    i2c_master_transfer_struct.data = rx_data;
    i2c_master_transfer_struct.dataSize = RX_DATA_LEN;

    retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &i2c_master_transfer_struct);
	g_master_completion_flag = 0xFFFF;
    if(0 != retval)
    {
    	return retval;
    }

    retval = wait_for_transfer();

    if(0 != retval)
    {
    	return retval;
    }


    Delay(500000);

	/** Turns the current to uA*/
	return_data = ((rx_data[1] << 8) | rx_data[0]);
	return_data &= 0x3FFF;

	sign_bit = ((return_data & BIT13_MASK) >> BIT13_SHIFT);

	return_data &= (~BIT13_MASK);
	return_data = ((return_data ^ (XOR_MASK * sign_bit)) + sign_bit);
	return_data = (0 == sign_bit) ? (return_data) : ((-1) * return_data);

	*(FuelGauge_Current) = return_data * 0.5885; // return_data/100 -> mA  58.85 = (11.77/Rsense)*100

	return retval;
}

// Turns off the Fuel Gauge module
static general_codes_t FuelGauge_TurnOff(void)
{
	general_codes_t retval = successful_task;
	/** Writes a 0 in register 0 to turn off the Fuel Gauge*/
    uint8_t tx_data[2] = {0x00};
    i2c_master_transfer_t i2c_master_transfer_struct = {0};

	/** Initializes the I2C in case it has been deinitialized*/
	retval = FuelGauge_Initialization();

	if((successful_task != retval) && (already_initialized != retval))
	{
		return retval;
	}

    Delay(5000);

    i2c_master_transfer_struct.slaveAddress = FUEL_GAUGE_ADDR;
    i2c_master_transfer_struct.direction = kI2C_Write;
    i2c_master_transfer_struct.data = tx_data;
    i2c_master_transfer_struct.dataSize = TX_DATA_LEN;
    i2c_master_transfer_struct.subaddress = 0;
    i2c_master_transfer_struct.subaddressSize = 0;
    i2c_master_transfer_struct.flags = kI2C_TransferDefaultFlag;

    retval = I2C_MasterTransferNonBlocking(I2C0, &i2c_handle, &i2c_master_transfer_struct);

    if(0 != retval)
    {
    	return retval;
    }

    retval = wait_for_transfer();

    if(0 != retval)
    {
    	return retval;
    }


    Delay(5000);

    return retval;
}

static uint32_t wait_for_transfer(void)
{
	uint32_t retval;

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
		retval = timeout_reached;
	}

	else
	{

		pit_channel_1_flag = false;
		retval = g_master_completion_flag;
	}

    g_master_completion_flag = 0xFFFF;

    return retval;
}

////////////////////////// VARIABLES /////////////////////////////////////
/** Variable to set whether the ADC for the battery has been initialized*/
uint8_t BatteryTemperature_isInit = NOT_INIT;
/** Variable to set whether the header's ADC has been initialized*/
extern uint8_t ADC_headers_is_init;

static uint32_t g_adc_timeout = 30000;
////////////////////////// VARIABLES /////////////////////////////////////

static double log(int b,double n);
static double p(double x,int i);

/** This function initializes the ADC to read the battery temperature*/
static general_codes_t BatteryTemperature_Initialize(void)
{
    pit_config_t pitConfig;
    adc16_config_t init_adc_struct;

    /** Initializes only if is not initialized*/
	if (NOT_INIT == ADC_headers_is_init)
	{
	    // Default parameters used in the ADC configuration
		init_adc_struct.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
		init_adc_struct.clockSource = kADC16_ClockSourceAsynchronousClock;
		init_adc_struct.enableAsynchronousClock = true;
		init_adc_struct.clockDivider = kADC16_ClockDivider8;
		init_adc_struct.resolution = kADC16_ResolutionSE12Bit;
		init_adc_struct.longSampleMode = kADC16_LongSampleDisabled;
		init_adc_struct.enableHighSpeed = false;
		init_adc_struct.enableLowPower = false;
		init_adc_struct.enableContinuousConversion = false;

		/* pitConfig.enableRunInDebug = false; */
		PIT_GetDefaultConfig(&pitConfig);
		/* Init pit module */
		PIT_Init(PIT, &pitConfig);
		/* Enable timer interrupts for channel 1 */
		PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
		/* Enable at the NVIC */
		EnableIRQ(PIT_IRQn);

		CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */
		PORT_SetPinMux(PORTE, PIN16_IDX, kPORT_PinDisabledOrAnalog);


		ADC16_Init(ADC0, &init_adc_struct);

		ADC16_EnableHardwareTrigger(ADC0, false); /* Make sure the software trigger is used. */

		/** Sets the header's ADC as deinitialized*/
		ADC_headers_is_init = NOT_INIT;
		/** Sets the ADC for the battery as initialized*/
		BatteryTemperature_isInit = IS_INIT;

	   	#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
		   if (kStatus_Success == ADC16_DoAutoCalibration(ADC0))
		   {
			   return successful_task;
		   }
		#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */
	}

    return already_initialized;
}

/** This function reads the battery temperature*/
general_codes_t BatteryTemperature_Read(uint16_t *battery_temperature)
{
	/** Channel configurations*/
    adc16_channel_config_t adc_channel_struct;
    /** Bypass variable*/
    float R;

    if(OFF == FG_status.voltage_divider_status)
    {
    	return VOLTAGE_DIVIDER_not_turned_on;
    }

    /** Initializes the ADC (If is not initialized)*/
    BatteryTemperature_Initialize();

    adc_channel_struct.enableInterruptOnConversionCompleted = false;

    #if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
        adc_channel_struct.enableDifferentialConversion = false;
    #endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */


    /** Battery's ADC channel*/
    adc_channel_struct.channelNumber = 1U;
    ADC16_SetChannelMuxMode(ADC0,kADC16_ChannelMuxB);
    ADC16_SetChannelConfig(ADC0, 0U, &adc_channel_struct);

    pit_channel_1_flag=false;

	/* Set timer period for channel 1 */
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, USEC_TO_COUNT(g_adc_timeout, CLOCK_GetFreq(kCLOCK_BusClk)));
	/* Start channel 1 */
	PIT_StartTimer(PIT, kPIT_Chnl_1);

    while ( !(kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(ADC0, 0U)) && !(pit_channel_1_flag))
    {

    }

	PIT_StopTimer(PIT, kPIT_Chnl_1);

    if( !(pit_channel_1_flag)){

        *battery_temperature = ADC16_GetChannelConversionValue(ADC0, 0U);

        /** Regino's formulas*/
        R = (10000) / ( (4095/(float)(*battery_temperature)) - 1);
        *battery_temperature = (uint16_t)(((-24.45)*(log(2,R)/log(2,2.71828)) + 251.91) * 100);

        pit_channel_1_flag=false;
        return successful_task;
    }

    pit_channel_1_flag=false;

    return timeout_reached;
}

static double log(int b,double n) {
    double val = 0;
    int i,accurate = 10,reps=0;
    while(n != 1 && accurate>=0) {
        for(i=0;n>=b;i++) n /= b;
            n = p(n,10);
            val = 10*(val+i);
            accurate--; reps++;
    }
    return (double)val/p(10,reps);
}

static double p(double x,int i) {
    double r = 1.0;
    for( ; i > 0 ; i --)
    {
    	r *= x;
    }
    return r;
}


static void i2c_fuel_gauge_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData){
   /*
   if (status == kStatus_Success)
   {
       g_master_completion_flag = true;
   }
   if (status == kStatus_Fail)
   {
       g_master_completion_flag = true;
   }
   if (status == kStatus_ReadOnly)
   {
       g_master_completion_flag = true;
   }
   if (status == kStatus_OutOfRange)
   {
       g_master_completion_flag = true;
   }
   if (status == kStatus_InvalidArgument)
   {
       g_master_completion_flag = true;
   }
   if (status == kStatus_Timeout)
   {
       g_master_completion_flag = true;
   }
   if (status == kStatus_NoTransferInProgress)
   {
       g_master_completion_flag = true;
   }


   if (status == kStatus_I2C_Busy)
   {
       g_master_completion_flag = true;
   }
   if (status == kStatus_I2C_Idle)
   {
       g_master_completion_flag = true;
   }
   if (status == kStatus_I2C_Nak)
   {
       g_master_completion_flag = true;
   }
   if (status == kStatus_I2C_ArbitrationLost)
   {
       g_master_completion_flag = true;
   }
   if (status == kStatus_I2C_Timeout)
   {
       g_master_completion_flag = true;
   }
   if (status == kStatus_I2C_Addr_Nak)
   {
       g_master_completion_flag = true;
   }
   */
    if(status > 6){
        g_master_completion_flag=(status-1100)+10;
    }
    else if(0 == status || 1 == status)
    {
    	g_master_completion_flag = 0;
    }
    else{
        g_master_completion_flag = status + (STATUS_OFFSET - 2);
    }

}


#endif
