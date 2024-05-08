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
 * \file       init_config_functions.h
 * \brief      This is the initial configuration functions header file
 *             made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.
 * \version    1.3.3
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

////////////////////////////////Include Guard///////////////////////////////////
#ifndef INIT_CONFIG_FUNCTIONS_H_
#define INIT_CONFIG_FUNCTIONS_H_

/////////////////////////////End of Include Guard///////////////////////////////
/** Defines the board Athen Sense System*/
#define ATHEN_SENSE_SYSTEM 		(1)
/** Defines the board Athen Lite*/
#define ATHEN_LITE				(0)

/** Defines the shield as placed*/
#define SHIELD_IS_PLACED		(1)
/** Defines the shield as not placed*/
#define SHIELD_IS_NOT_PLACED	(0)

/** Defines which Athen board is being used*/
#define ATHEN_BOARD		 	ATHEN_LITE

/** Defines whether the shield is placed or not*/
#define ATHEN_SHIELD		SHIELD_IS_NOT_PLACED

////////////////////////////////Includes Area///////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <drivers/fsl_adc16.h>

#include <drivers/fsl_adc16.h>
#include <drivers/fsl_i2c.h>
#include <drivers/fsl_uart.h>


#include <drivers/fsl_lpuart.h>
#include <drivers/fsl_pit.h>
#include <drivers/fsl_port.h>
#include <drivers/fsl_spi.h>
#include <drivers/fsl_rtc.h>
///////////////////////////End of Includes Area/////////////////////////////////

#define SOPT5_LPUART0RXSRC_LPUART_RX 0x00u /*!<@brief LPUART0 Receive Data Source Select: LPUART_RX pin */
#define SOPT5_LPUART0TXSRC_LPUART_TX 0x00u /*!<@brief LPUART0 Transmit Data Source Select: LPUART0_TX pin */

/** Offset for I2C codes*/
#define I2C_STATUS_OFFSET		(10)
/** Offset for GPIO codes*/
#define GPIO_STATUS_OFFSET		(100)
/** Offset for ADC codes*/
#define ADC_STATUS_OFFSET		(200)
/** Offset for status codes*/
#define STATUS_OFFSET			(300)
/** Offset for timeout codes*/
#define TIMEOUT_OFFSET			(511)
/** Offset for UART codes*/
#define UART_STATUS_OFFSET		(1009)
/** Offset for I2C bus codes*/
#define I2C_BUSY_OFFSET			(1100)
/** Offset for LPUART codes*/
#define LPUART_STATUS_OFFSET	(1309)
/** Offset for LPU codes*/
#define LPU_STATUS_OFFSET		(1500)
/** Offset for fuel gauge codes*/
#define FG_STATUS_OFFSET		(2000)

#define PIN0_IDX		0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX        1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX        2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX        3u   /*!< Pin number for pin 3 in a port */
#define PIN4_IDX        4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX        5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX        6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX        7u   /*!< Pin number for pin 7 in a port */
#define PIN8_IDX        8u   /*!< Pin number for pin 8 in a port */
#define PIN9_IDX        9u   /*!< Pin number for pin 9 in a port */
#define PIN10_IDX       10u   /*!< Pin number for pin 10 in a port */
#define PIN11_IDX       11u   /*!< Pin number for pin 11 in a port */
#define PIN14_IDX       14u   /*!< Pin number for pin 14 in a port */
#define PIN15_IDX       15u   /*!< Pin number for pin 15 in a port */
#define PIN16_IDX       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX       17u   /*!< Pin number for pin 17 in a port */
#define PIN18_IDX       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX       19u   /*!< Pin number for pin 19 in a port */
#define PIN20_IDX       20u   /*!< Pin number for pin 20 in a port */
#define PIN21_IDX       21u   /*!< Pin number for pin 21 in a port */
#define PIN22_IDX       22u   /*!< Pin number for pin 22 in a port */
#define PIN23_IDX       23u   /*!< Pin number for pin 23 in a port */
#define PIN24_IDX       24u   /*!< Pin number for pin 23 in a port */
#define PIN25_IDX       25u   /*!< Pin number for pin 23 in a port */
#define PIN29_IDX       29u   /*!< Pin number for pin 29 in a port */
#define PIN30_IDX       30u   /*!< Pin number for pin 30 in a port */
#define PIN31_IDX       31u   /*!< Pin number for pin 31 in a port */
/////////////////////////////// Enumerators ////////////////////////////////////

/*!
 	 \name Codes enumerator
 	 \{
 */
/*!
 	 \brief Enumerator to indicate errors returned by functions.
 */
typedef enum
{
	successful_task,
	initialization_failed,
	already_initialized,
	parameter_error,

	I2C_bus_busy = I2C_STATUS_OFFSET,
	I2C_idle,
	I2C_NAK,
	I2C_arbitration_lost,
	I2C_timeout,
	I2C_addr_NAK,

	GPIO_pin_not_available = GPIO_STATUS_OFFSET,
	GPIO_pin_does_not_support_interruption,

	ADC_pin_not_available = ADC_STATUS_OFFSET,

	STATUS_read_only = STATUS_OFFSET,
	STATUS_out_of_range,
	STATUS_invalid_argument,
	STATUS_timeout,
	STATUS_no_transfer_in_progress,

	timeout_reached = TIMEOUT_OFFSET,

	UART_hardware_overrun = UART_STATUS_OFFSET,
	UART_nose_error,
	UART_framing_error,
	UART_parity_error,
	UART_RDFD_not_received,

	I2C_busy = I2C_BUSY_OFFSET,

	LPUART_hardware_overrun = LPUART_STATUS_OFFSET,
	LPUART_noise_error,
	LPUART_framing_error,
	LPUART_parity_error,

	LPU_not_enabled = LPU_STATUS_OFFSET,

	FUEL_GAUGE_not_turned_on = FG_STATUS_OFFSET,
	VOLTAGE_DIVIDER_not_turned_on
}general_codes_t;

/*!
 	 \{
 */

/*!
 	 \name ADC enumerators
 	 \{
 */

#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
/*!
 	 \brief Enumerator to define the different pin alternatives
 	 	 	 for the ADC
 */
typedef enum
{
	ADC_Alt0,	/*!< PTC 0 as ADC*/
	ADC_Alt1,	/*!< PTC 1 as ADC*/
	ADC_Alt2,	/*!< PTC 2 as ADC*/
	ADC_Alt3,	/*!< PTD 1 as ADC*/
	ADC_Alt4,	/*!< PTD 5 as ADC*/
	ADC_Alt5,	/*!< PTD 6 as ADC*/
	ADC_Alt6,	/*!< PTE 20 as ADC*/
	ADC_Alt7,	/*!< PTE 21 as ADC*/
	ADC_Alt8,	/*!< PTE 22 as ADC*/
	ADC_Alt9,	/*!< PTE 23 as ADC*/
    ADC_Alt10,  /*!< PTE 29 as ADC*/
    ADC_Alt11,  /*!< PTE 30 as ADC*/
}adc_alternatives_t;
#endif

/*!
 	 \brief Enumerator to set the ADC resolution
 */
typedef enum
{
    ADC_Resolution8 = 0U,		/*!< 8 bit resolution*/
    ADC_Resolution12 = 1U,		/*!< 12 bit resolution*/
    ADC_Resolution10 = 2U,		/*!< 10 bit resolution*/
    ADC_Resolution16 = 3U		/*!< 16 bit resolution*/
}adc_resolution_t;


/*!
 	 \brief Enumerator to define the continuous conversion mode.
 */
typedef enum
{
	continuous_conversion_disabled,	/*!< Enables the continuous conversion*/
	continuous_conversion_enabled	/*!< Disables the continuous conversion*/
}continuous_conversion_mode_t;

/*!
 	 \brief Enumerator to define the low power mode
 */
typedef enum
{
	low_power_mode_disabled,	/*!< Disables the low power mode*/
	low_power_mode_enabled		/*!< Enables the low power mode*/
}low_power_mode_t;

/*!
 	 \}
 */

/*!
 	 \name GPIO enumerators
 	 \{
 */

/*!
 	 \brief Enumerator to choose a GPIO port and a respective pin.
 */
typedef enum
{
#if (defined(ATHEN_BOARD) && ATHEN_BOARD)
	Port_C0,	/*!< Port C, pin 0*/
	Port_C1,	/*!< Port C, pin 1*/
	Port_C2,	/*!< Port C, pin 2*/
    Port_D1,    /*!< Port D, pin 1*/
	Port_D5,	/*!< Port D, pin 5*/
	Port_D6,	/*!< Port D, pin 6*/
	Port_E20,	/*!< Port E, pin 20*/
	Port_E21,	/*!< Port E, pin 21*/
	Port_E22,	/*!< Port E, pin 22*/
	Port_E23,	/*!< Port E, pin 23*/
	Port_E29,	/*!< Port E, pin 29*/
	Port_E30	/*!< Port E, pin 30*/
#endif

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))
    Port_E0,			/*!< Port E, pin 0*/
    Port_E1,			/*!< Port E, pin 1*/
    Port_C3,			/*!< Port C, pin 3*/
    Port_C4,			/*!< Port C, pin 4*/
    Port_D4,			/*!< Port D, pin 4*/
    Port_D5,			/*!< Port D, pin 5*/
#endif
}port_gpio_t;

/*!
 	 \brief enumerator to set the trigger for the GPIO interruption.
 */
typedef enum
{
	Logic0_interrupt = 0x08,	/*!< Interruption when a logic 0 occurs*/
	RisingEdge_interrupt,		/*!< Interruption when a rising edge occurs*/
	FallingEdge_interrupt,		/*!< Interruption when a falling edge occurs*/
	EitherEdge_interrupt,		/*!< Interruption when either edge occurs*/
	Logic1_interrupt			/*!< Interruption when a logic 1 occurs*/
}gpio_interrupt_type_t;

/*!
 	 \brief Enumerator to enable or disable pin's interruption,
 */
typedef enum
{
#if(defined(ATHEN_BOARD) && (ATHEN_BOARD))
	interrupt_PTC0,	/*!< Port C, pin 0*/
	interrupt_PTC1,	/*!< Port C, pin 1*/
	interrupt_PTC2,	/*!< Port C, pin 2*/
    interrupt_PTD1, /*!< Port D, pin 1*/
	interrupt_PTD5,	/*!< Port D, pin 5*/
	interrupt_PTD6	/*!< Port D, pin 6*/
#endif

#if(defined(ATHEN_BOARD) && !ATHEN_BOARD)
	interrupt_PTC3 = 2,	/*!< Port C, pin 3*/
	interrupt_PTC4,		/*!< Port C, pin 4*/
	interrupt_PTD4,		/*!< Port D, pin 4*/
	interrupt_PTD5		/*!< Port D, pin 5*/
#endif
}interrupt_pins_t;

/*!
 	 \brief Enumerator to define the interruption state.
 */
typedef enum
{
	pin_not_interrupted,	/*!< The pin has not interrupted yet*/
	pin_interrupted			/*!< The pin already interrupted*/
}interrupt_status_t;

/*!
 	 \brief Enumerator to set a pin output value.
 */
typedef enum
{
    Write_0,			/*!< Writes a logic 0*/
    Write_1,			/*!< Writes a logic 1*/
}write_output_t;

/*!
 	 \brief Enumerator to choose the pin direction.
 */
typedef enum
{
    Port_Output,		/*!< Pin is set as output*/
    Port_Input,			/*!< Pin is set as input*/
}port_mode_t;

/*!
 	 \}
 */

/*!
 	 \name I2C enumerators
 	 \{
 */
/*!
 	 \brief Enumerator to choose from different I2C pin combinations.
 */
typedef enum
{

#if (defined(ATHEN_BOARD) && ATHEN_BOARD)
	I2C_Alt0,	/*!< PTE24 as SCL, and PTE25 as SDA*/
	I2C_Alt1,	/*!< PTC1 as SCL, and PTC2 as SDA*/
#endif

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))
    I2C_Alt0,			/*!< PTB0 as SCL, and PTB1 as SDA*/
    I2C_Alt1,			/*!< PTE1 as SCL, and PTE0 as SDA*/
#endif
}i2c_alternatives_t;
/*!
 	 \}
 */

/*!
 	 \name LPU enumerators
 	 \{
 */

/*!
 	 \brief Enumerator the define the modules to interrupt the LLWU.
 */
typedef enum
{
    llwu_no_interrupt,	/*!< No module will wake up the LLWU*/
    llwu_timer,			/*!< A timer will wake up the LLWU*/
    llwu_port,			/*!< A port will wake up the LLWU*/
    llwu_timer_and_port	/*!< A timer and a port can wake up the LLWU*/
}llwu_interrupt_t;

/*!
 	 \brief Enumerator the define the port and pin that will cause
 	 	 	 the LLWU to wake up.
 */
typedef enum
{
    llwu_not_port,	/*!< No port will wake up the LLWU*/
#if (defined(ATHEN_BOARD) && (ATHEN_BOARD))

    llwu_port_C1,	/*!< Port C, pin 1*/
    llwu_port_C3,	/*!< Port C, pin 3*/
    llwu_port_D6,	/*!< Port D, pin 6*/
#endif

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))

    llwu_port_C3,	/*!< Port C, pin 3*/
    llwu_port_C4,	/*!< Port C, pin 4*/
    llwu_port_D4,	/*!< Port D, pin 4*/
#endif

}llwu_ports_t;

/*!
 	 \brief Enumerator to define the power modes for the LLWU.

 	 \note Configure the low-leakage wake up unit interruptions
 	 	 	 before entering LLS, VLLS3, VLLS1 or VLLS0 modes.
 	 	 	 Use the function LPU_Init_Power_Mode.

 	 \note All power modes can be exited with a reset.
 */
typedef enum
{
    run_mode,	/*!< The device runs at full speed.*/
    vlpr_mode,	/*!< The core, system, bus and flash clocks
     	 	 	 	 speed is restricted. When a reset occurs
     	 	 	 	 the device will exit the vlpr mode*/
    wait_mode,	/*!< The core clock is gated off, system
     	 	 	 	 and bus clocks are regulated. When an
     	 	 	 	 interrupt request occurs, the device will
     	 	 	 	 exit wait mode*/
    stop_mode,	/*!< The core, system and bus clocks are
     	 	 	 	 gated off after all stop acknowledge
     	 	 	 	 signals are valid. When an interrupt
     	 	 	 	 request occurs, the device will exit
     	 	 	 	 the stop mode.*/
    vlpw_mode,	/*!< The core clock is gated off, the system
     	 	 	 	 bus, and flash clocks' speeds are
     	 	 	 	 restricted*/
    vlps_mode,	/*!< The core, system and bus clocks are
     	 	 	 	 gated off after all stop acknowledge
     	 	 	 	 signals are valid.*/
    lls_mode,	/*!< The core clock is gated off. System and
     	 	 	 	 bus clocks are gated off after all stop
     	 	 	 	 acknowledge signals are valid. The device
     	 	 	 	 is placed in a low leakage mode. All
     	 	 	 	 system RAM contents, internal logic and I/O
     	 	 	 	 states are retained*/
    vlls3_mode,	/*!< The core clock is gated off. System and
     	 	 	 	 bus clocks are gated off after all stop
     	 	 	 	 acknowledge signals are valid. The device
     	 	 	 	 is placed in a low leakage mode. All
     	 	 	 	 system RAM contents and I/O states are
     	 	 	 	 retained, internal logic states are not
     	 	 	 	 retained.*/
    vlls1_mode,	/*!< The core clock is gated off. System and
     	 	 	 	 bus clocks are gated off after all stop
     	 	 	 	 acknowledge signals are valid. The device
     	 	 	 	 is placed in a low leakage mode. All
     	 	 	 	 I/O states are retained, system RAM and
     	 	 	 	 internal logic states are not retained.*/
    vlls0_mode	/*!< The core clock is gated off. System and
     	 	 	 	 bus clocks are gated off after all stop
     	 	 	 	 acknowledge signals are valid. The device
     	 	 	 	 is placed in a low leakage mode. All
     	 	 	 	 I/O states are retained, system RAM and
     	 	 	 	 internal logic states are not retained.*/
}power_mode_t;

/*!
 	 \}
 */

/*!
 	 \name LPUART enumerators
 	 \{
 */

/*!
 	 \brief Enumerator to choose from different Low Power UART pin combinations.
 */
typedef enum
{

#if (defined(ATHEN_BOARD) && ATHEN_BOARD)
	LPUART_Alt0,	/*!< PTE21 as RX, and PTE20 as TX*/
	LPUART_Alt1		/*!< PTD6 as RX, and PTE20 as TX*/
#endif

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))
	LPUART_Alt0,	/*!< PTC3 as RX, and PTC4 as TX*/
	LPUART_Alt1,	/*!< PTE1 as RX, and PTE0 as TX*/
	LPUART_Alt2,	/*!< PTC3 as RX, and PTE0 as TX*/
	LPUART_Alt3		/*!< PTE1 as RX, and PTC4 as TX*/
#endif
}lpuart_alternatives_t;

/*!
 	 \brief Enumerator to choose from a blocking or a
 	 	 	 non-blocking Low Power UART reading
 */
typedef enum
{
	LPUART_NonBlocking,	/*!< Non-blocking Low Power UART reading*/
	LPUART_Blocking		/*!< Blocking Low Power UART reading*/
}lpuart_transaction_mode_t;

/*!
 	 \brief Enumerator to choose the parity check of the LPUART
 */
typedef enum
{
	LPUART_ParityDisabled = 0x00,	/*!< No parity bits*/
	LPUART_ParityEven = 0x02,		/*!< Check for even parity bits*/
	LPUART_ParityOdd = 0x03			/*!< Check for odd parity bits*/
}lpuart_parity_t;

/*!
 	 \}
 */

/*!
 	 \name PIT enumerators
 	 \{
 */
/*!
 	 \brief Enumerator to define the PIT flag state.
 */
typedef enum
{
	PIT_not_overflow,	/*!< PIT has not overflowed yet*/
	PIT_overflow		/*!< PIT has overflowed*/
}pit_flag_state_t;
/*!
 	 \}
 */

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))
/*!
 	 \name SPI enumerators
 	 \{
 */
/*!
 	 \brief Enumerator to choose from different SPI SCK pins.
 */
typedef enum
{
	SPI_Alt0,	/*!< PTD5 as SCK*/
	SPI_Alt1	/*!< PTC3 as SCK*/
}spi_alternatives_t;

/*!
 	 \brief Enumerator to choose from a blocking or a non-blocking
 	 	 	 SPI read/write.
 */
typedef enum
{
	SPI_NonBlocking,	/*!< Non-blocking SPI read/write*/
	SPI_Blocking		/*!< Blocking SPI read/write*/
}spi_transaction_mode_t;

/*!
 	 \brief Enumerator to set the Athen as master or slave.
 */
typedef enum
{
	SPI_Master,			/*!< Athen set as master*/
	SPI_Slave			/*!< Athen set as slave*/
}spi_init_mode_t;

/*!
 	 \brief Enumerator to set the SPI polarity
 */
typedef enum
{
    SPI_PolarityActiveHigh = 0x0U,	/*!< Active-high SPI clock*/
    SPI_PolarityActiveLow			/*!< Active-low SPI clock*/
} spi_polarity_t;

/*!
 	 \brief Enumerator to set the SPI phase
 */
typedef enum
{
    SPI_PhaseFirstEdge = 0x0U,		/*!< First clock edge occurs at the middle
     	 	 	 	 	 	 	 	 	 of the transmission*/
    SPI_PhaseSecondEdge				/*!< First clock edge occurs at the start
     	 	 	 	 	 	 	 	 	 of the transmission*/
} spi_phase_t;
/*!
 	 \}
 */
#endif

/*!
 	 \name UART enumerators
 	 \{
 */
#if(((defined(ATHEN_SHIELD) && (!ATHEN_SHIELD))) || (defined(ATHEN_BOARD)) && (!ATHEN_BOARD))
/*!
 	 \brief Enumerator to choose from a blocking or a
 	 	 	 non-blocking UART reading
 */
typedef enum{
	UART_NonBlocking,	/*!< Non-blocking UART reading*/
	UART_Blocking		/*!< Blocking UART reading*/
}uart_transaction_mode_t;
#endif

#if ((defined(ATHEN_BOARD) && ATHEN_BOARD))
#if(defined(ATHEN_SHIELD) && (!ATHEN_SHIELD))
/*!
 	 \brief Enumerator to choose from different UART alternatives.
 */
typedef enum
{
	UART_Alt0,	/*!< PTE23 as RX, and PTE22 as TX*/
	UART_Alt1	/*!< PTE23 as RX, and PTD5 as TX*/
}uart_alternatives_t;
#endif
#endif

#if(((defined(ATHEN_SHIELD) && (!ATHEN_SHIELD))) || (defined(ATHEN_BOARD)) && (!ATHEN_BOARD))
/*!
 	 \brief Enumerator to choose the parity check of the UART
 */
typedef enum
{
	UART_ParityDisabled = 0x00,	/*!< No parity bits*/
	UART_ParityEven = 0x02,		/*!< Check for even parity bits*/
	UART_ParityOdd = 0x03		/*!< Check for odd parity bits*/
}uart_parity_t;
#endif

/*!
 	 \}
 */

/*!
 	 \name General enumerators
 	 \{
 */

/*!
 	 \brief Enumerator to choose between default and own configuration
 	 for the peripherals
 */
typedef enum
{
    Personal_Configuration,	/*!< Set the user defined configuration*/
    Default_Configuration,	/*!< Set the default configuration*/
}configuration_select_t;

/*!
 	 \brief Enumerator to control the state of the LED
 */
typedef enum
{
	LED_off,	/*!< LED is turned off*/
	LED_on		/*!< LED is turned on*/
}led_state_t;

#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
/*!
 	 \brief Enumerator to control the state of the GPS antenna.
 */
typedef enum
{
	GPS_off,	/*!< The antenna is turned off*/
	GPS_on		/*!< The antenna is turned on*/
}antenna_state_t;
#endif

/*!
 	 \brief Enumerator to define the state of the voltage
 	 	 	 of the sensor's headers.
 */
typedef enum
{
	VDD_off,	/*!< Header's voltage is off*/
	VDD_on		/*!< Header's voltage is on*/
}VDD_state_t;


typedef enum
{
	RTC_Alarm_only,
	RTC_Seconds_only,
	RTC_Both_interrupt
}RTC_interrupt_t;
/*!
 	 \}
 */

/////////////////////////////// Enumerators ////////////////////////////////////

//////////////////////////////// Structures ////////////////////////////////////

#if(((defined(ATHEN_SHIELD) && (!ATHEN_SHIELD))) || (defined(ATHEN_BOARD)) && (!ATHEN_BOARD))
/*!
 	 \brief Structure for the UART configuration.
 */
typedef struct
{
	uint32_t baudRate_Bps;		/*!< Sets the baud rate in bits per second (bps)*/
	uart_parity_t parityMode;	/*!< Sets the parity mode*/
}UART_init_t;
#endif

/*!
 	 \brief Structure for the LPUART configuration.
 */
typedef struct
{
	uint32_t baudRate_Bps;		/*!< Sets the baud rate in bits per seconds (bps)*/
	lpuart_parity_t parityMode;	/*!< Sets the parity mode*/
}LPUART_init_t;

/*!
 	 \brief Structure for the ADC configuration.
 */
typedef struct
{
	adc_resolution_t resolution;								/*!< Sets the resolution of the ADC*/
	low_power_mode_t enableLowPower;							/*!< Enables or disables the low power mode*/
    continuous_conversion_mode_t enableContinuousConversion;	/*!< Enables or disables the continuous conversion*/
}ADC_init_t;

/*!
 	 \brief Structure for the I2C configuration
 */
typedef struct
{
	uint32_t baudRate_Bps;	/*!< Sets the baud rate in bits per second (bps)*/
}I2C_init_t;

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))
/*!
 	 \brief Structure for the SPI master configuration
 */
typedef struct
{
	spi_polarity_t polarity;	/*!< Sets the clock polarity*/
	spi_phase_t phase;			/*!< Sets the clock phase*/
	uint32_t baudRate_Bps;		/*!< Sets the baud rate in bits per second (bps)*/
}SPI_Master_init_t;

/*!
 	 \brief Structure for the SPI slave configuration
 */
typedef struct
{
	spi_polarity_t polarity;	/*!< Sets the clock polarity*/
	spi_phase_t phase;			/*!< Sets the clock phase*/
}SPI_Slave_init_t;
#endif
//////////////////////////////// Structures ////////////////////////////////////

//////////////////////////////// Functions /////////////////////////////////////

/********************************************************************************************************************************
 * 		PIT
 *********************************************************************************************************************************/
/*!
 	 \name PIT functions
 	 \{
 */

/*!
 	 \brief This function initializes the PIT timer.

 	 \return void.
 */
void PIT_Initialization(void);

/*!
 	 \brief This function deinitializes the PIT timer.

 	 \return void.
 */
void PIT_Deinitialization(void);

/*!
 	 \brief This function sets the timer count and starts the timer.

 	 \param[in] count_usec Time in microseconds for the PIT to wait.

 	 \return void.
 */
void PIT_Start_Timer(uint32_t count_usec);

/*!
 	 \brief This function stops the PIT timer.

 	 \return void.
 */
void PIT_Stop_Timer(void);

/*!
 	 \brief This function returns if the PIT has finished its count or not.

 	 \return The state of the PIT overflow flag.
 */
pit_flag_state_t PIT_Get_Flag(void);

/*!
 	 \brief This function set the overflow PIT flag to 0.

 	 \return void.
 */
void PIT_Clean_Flag(void);
/*!
 	 \}
 */
/********************************************************************************************************************************
 * 		PIT
 *********************************************************************************************************************************/

/**********************************************************************************************************************************
 * 		UART
 *********************************************************************************************************************************/

/*!
 	 \name UART functions
 	 \{
 */

#if(((defined(ATHEN_SHIELD) && (!ATHEN_SHIELD))) || (defined(ATHEN_BOARD)) && (!ATHEN_BOARD))
#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))
/*!
 	 \brief This function initializes the UART, with PTD4 as RX and PTD5 as TX.

 	 \param[in] uartConfigStruct Configuration values for the UART. See uart_config_t.
 	 \param[in] default_configuration Whether the configuration used will be the default
 	 	 	 	 	 	 	 	 	 one, or the user-defined one.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success
 */
general_codes_t UART_Initialization(UART_init_t uartConfigStruct, configuration_select_t default_configuration);
#endif

#if (defined(ATHEN_BOARD) && ATHEN_BOARD)
/*!
	\brief This function initializes the UART, allowing the user to choose between different
			pin alternatives.

	\param[in] alternative Combination of TX and RX pins, see uart_alternatives_t
 	\param[in] uartConfigStruct Configuration values for the UART. See uart_config_t.
 	\param[in] default_configuration Whether the configuration used will be the default
 	 	 	 	 	 	 	 	 	 one, or the user-defined one.

 	\return Indicates if an error occurred.
 			Returns 0 for success
 */
general_codes_t UART_Initialization(uart_alternatives_t alternative, UART_init_t uartConfigStruct,
		configuration_select_t default_configuration);
#endif

/*!
 	 \brief This function sets the desired function as callback.

 	 \param[in] callback Pointer to the desired callback function. See uart_transfer_callback_t.

 	 \return void.
 */
void UART_Interrupt(uart_transfer_callback_t callback);

/*!
 	 \brief This function sends data from UART.

 	 \param[in] data Pointer to the data to be sent.
 	 \param[in] data_size Length of the data to be sent.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t UART_Write(uint8_t* data, uint32_t data_size);

/*!
 	 \brief This function makes a UART transfer, allowing the user to choose between
 	 	 	 blocking and non blocking transmission.

 	 \param[in] mode Whether the transaction will be blocking or non-blocking.
 	 \param[in] data Pointer to the data to be sent.
 	 \param[in] data_size Length of the data to be sent.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t UART_Write_Data(uart_transaction_mode_t mode, uint8_t* data, uint32_t data_size);

/*!
 	 \brief This function receives data from the UART.

 	 \param[in] data Pointer to the variable where the data will be received.
 	 \param[in] data_size Length of the data to be received.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t UART_Read(uint8_t* data, uint32_t data_size);

/*!
 	 \brief This function receives data from the UART, allowing the user to choose between
 	 	 	 blocking and non-blocking reception. See uart_transactuon_mode.

 	 \param[in] mode Whether the reception will be blocking or non-blocking.
 	 \param[in] data Pointer to the variable where the data will be received.
 	 \param[in] data_size Length of the data to be received.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t UART_Read_Data(uart_transaction_mode_t mode, uint8_t* data, uint32_t data_size);

/*!
 	 \brief This function deinitializes the UART and releases the pins used.

 	 \return void.
 */
void UART_Deinitialization(void);

/*!
 	 \brief This function sets the maximum time for the UART to receive. After this time,
 	 	 	 the receive function will be exited.

 	 \param[in] timeout Time limit in microseconds.

 	 \return void.
 */
void UART_Set_Timeout(uint32_t timeout);

/*!
 	 \brief This function returns the maximum time for the UART to receive. After this time,
 	 	 	 the receive function will be exited.

 	 \return The time limit in microseconds.
 */
uint32_t UART_Get_Timeout(void);
#endif

/*!
 	 \}
 */
/********************************************************************************************************************************
 * 		UART
 *********************************************************************************************************************************/

/**********************************************************************************************************************************
 * 		LPUART
 *********************************************************************************************************************************/

/*!
 	 \name LPUART functions
 	 \{
 */

/*!
 	 \brief This function initializes the Low Power UART.

 	 \param[in] LPUART_alternative Combination of RX and TX pins. See lpuart_alternatives.
 	 \param[in] lpuartConfigStruct Configuration values for the LPUART. See lpuart_config_t.
 	 \param[in] default_configuration Whether the configuration used will be the default one
 	 	 	 	 	 	 	 	 	 	 or the user-defined one.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t LPUART_Initialization(lpuart_alternatives_t LPUART_alternative, LPUART_init_t lpuartConfigStruct,
								configuration_select_t default_configuration);

/*!
 	 \brief This function sets the desired function as callback.

 	 \param[in] callback Pointer to the desired function.

 	 \return void.
 */
void LPUART_Interrupt(lpuart_transfer_callback_t callback);

/*!
 	 \brief This function received data using interrupt.

 	 \param[in] data Pointer to the variable where the data is received.
 	 \param[in] data_size Number of bytes to be received.

 	 \return void.
 */
void LPUART_Read_Interrupt(uint8_t *data, uint32_t data_size);

/*!
 	 \brief This function send data using interrupt.

 	 \param[in] data Pointer to the variable to be send.
 	 \param[in] data_size Number of bytes to be send.

 	 \return void.
 */
void LPUART_Write_Interrupt(uint8_t *data, uint32_t data_size);

/*!
 	 \brief This function writes data from the LPUART.

 	 \param[in] data Pointer to the data to be sent.
 	 \param[in] data_size Length of the data to be sent.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t LPUART_Write(uint8_t* data, uint32_t data_size);

/*!
 	 \brief This function makes an LPUART transfer allowing the user to choose blocking and
 	 	 	 non-blocking transmission.

 	 \param[in] mode Whether the transmission will be blocking and non-blocking.
 	 \param[in] data Pointer to the data to be sent.
 	 \param[in] data_size Length of the data to be sent.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t LPUART_Write_Data(lpuart_transaction_mode_t mode, uint8_t* data, uint32_t data_size);

/*!
 	 \brief This function receives data from the LPUART.

 	 \param[in] data Pointer to the variable where the data will be received.
 	 \param[in] data_size Length of the data to be received.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t LPUART_Read(uint8_t* data, uint32_t data_size);

/*!
 	 \brief This function receives data from the LPUART, allowing the user to choose between
 	 	 	 blocking and non-blocking reception. See lpuart_transaction_mode.

 	 \param[in] mode Whether the reception will be blocking or non-blocking.
 	 \param[in] data Pointer to the variable where the data will be received.
 	 \param[in] data_size Length of the data to be received.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t LPUART_Read_Data(lpuart_transaction_mode_t mode, uint8_t* data, uint32_t data_size);

/*!
 	 \brief This function deinitializes the LPUART and releases the pins used.

 	 \return void.
 */
void LPUART_Deinitialization(void);

/*!
 	 \brief This function sets the maximum time for the LPUART to send/receive. After this time,
 	 	 	 the send/receive function will be exited.

 	 \param[in] timeout Time limit in microseconds.

 	 \return void.
 */
void LPUART_Set_Timeout(uint32_t timeout);

/*!
 	 \brief This function returns the maximum time for the LPUART to send/receive. After this time,
 	 	 	 the send/receive function will be exited.

 	 \return The time limit in microseconds.
 */
uint32_t LPUART_Get_Timeout(void);

/*!
 	 \}
 */
/********************************************************************************************************************************
 * 		LPUART
 *********************************************************************************************************************************/
/********************************************************************************************************************************
 * 		LPU
 *********************************************************************************************************************************/

/*!
 	 \name LPU functions
 	 \{
 */

/*!
 	 \brief This function enables the Low Power Unit and its
 	 	 	 functions.

 	 \return void.
 */
void LPU_Enable_Power_Modes(void);

/*!
 	 \brief This function sets the board to a specified power mode

	 \note For this function to work, first call the function
	 	 	LPU_Enable_Low_Power_Modes

 	 \param[in] select_power_mode_switch Selects the power mode.
 	 \param[in] time_to_sleep The amount of time, in seconds, for the board
 	 	 	 	 	 	 	 	 to be in sleep mode.

 	 \return This function indicates if the task was successful, or if an error occurred.
 */
general_codes_t LPU_Select_Power_Mode(power_mode_t select_power_mode_switch, uint8_t time_to_sleep);

/*!
 	 \brief This function initializes the ability to use power saving modes

	 \note For this function to work, first call the function
	 	 	LPU_Enable_Low_Power_Modes

 	 \param[in] llwu_interrupt Chooses the interruption that will cause the
 	 	 	 	 	 	 	 	 LLWU to wake up.
 	 \param[in] llwu_ports Chooses the port that will cause the LLWU to wake
 	 	 	 	 	 	 	 up, in case the interruption will be caused by
 	 	 	 	 	 	 	 a port. For other interruptions, use llwu_not_port.

 	 \return This function indicates if the task was successful, or if an error occurred.
 */
general_codes_t LPU_Init_Power_Mode(llwu_interrupt_t llwu_interrupt, llwu_ports_t llwu_ports);

/*!
 	 \}
 */

/********************************************************************************************************************************
 * 		LPU
 *********************************************************************************************************************************/

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))
/********************************************************************************************************************************
 * 		SPI
 *********************************************************************************************************************************/

/*!
 	 \name SPI functions
 	 \{
 */

/*!
 	 \brief This function initializes the SPI as master.

 	 \param[in] SPI_alternative The SCK pin. See spi_alternatives.
 	 \param[in] masterConfigStruct Configuration values for the SPI. See spi_master_config_t.
 	 \param[in] default_configuration Whether the configuration will be the default one
 	 	 	 	 	 	 	 	 	 	 or the user-defined one.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t SPI_Master_Init(spi_alternatives_t SPI_alternative, SPI_Master_init_t masterConfigStruct,
						configuration_select_t default_configuration);

/*!
 	 \brief This function initializes the SPI as slave.

 	 \param[in] SPI_alternative The SCK pin. See spi_alternatives.
 	 \param[in] slaveConfigStruct Configuration values for the SPI. See spi_slave_config_t.
 	 \param[in] default_configuration Whether the configuration used will be the default one
 	 	 	 	 	 	 	 	 	 	 or the user-defined one.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t SPI_Slave_Init(spi_alternatives_t SPI_alternative, SPI_Slave_init_t slaveConfigStruct,
						configuration_select_t default_configuration);

/*!
	\brief This function sends data from the SPI.

	\param[in] data Pointer to the data to be sent.
	\param[in] data_size Length of the data to be sent.

	\return Indicates if an error occurred.
			Returns 0 for success.
 */
general_codes_t SPI_Write(uint8_t* data, uint32_t data_size);

/*!
 	 \brief This function makes an SPI transfer allowing the user to choose between blocking and
 	 	 	 non-blocking transmission.

 	 \param[in] mode Whether the transfer will be blocking or non-blocking.
 	 \param[in] data Pointer to the data to be sent.
 	 \param[in] data_size Length of the data to be sent.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t SPI_Write_Data(spi_transaction_mode_t mode, uint8_t* data, uint32_t data_size);

/*!
 	 \brief This function read data from the SPI.

 	 \param[out] data Pointer to the variable where the data will be received.
 	 \param[in] data_size Length of the data to be received.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t SPI_Read(uint8_t* data, uint32_t data_size);

/*!
 	 \brief This function reads data from the SPI allowing the user to choose between blocking and
 	 	 	 non-blocking reception.

 	 \param[in] mode Whether the reception will be blocking or non-blocking.
 	 \param[out] data Pointer to the variable where the data will be received.
 	 \param[in] data_size Length of the data to be received.

 	 \return void.
 */
void SPI_Read_Data(spi_transaction_mode_t mode, uint8_t* data, uint32_t data_size);

/*!
 	 \brief This function deinitializes the SPI and releases the pins used.

 	 \return void.
 */
void SPI_Deinitialization(void);

/*!
 	 \}
 */

/********************************************************************************************************************************
 * 		SPI
 *********************************************************************************************************************************/

#endif

/********************************************************************************************************************************
 * 		GPIO
 *********************************************************************************************************************************/

/*!
 	 \name GPIO functions
 	 \{
 */

/*!
   \brief This function initializes and configures a pin as an input
          or output.

   \param[in] Port_select Port and pin to be configured. See port_gpio_t.
   \param[in] select_input_output Whether the pin will be an input or an output.

   \return Indicates if an error occurred.
   	   	   Returns 0 for success.
 */
general_codes_t GPIO_Initialization(port_gpio_t Port_select, port_mode_t select_input_output);

/*!
 	 \brief This function writes a logic 1 or 0 in a the desired pin.

 	 \param[in] Port_select Port and pin to be written. See port_gpio_t.
 	 \param[in] output Whether the output of the pin will be a logic 1
 	 	 	 	 	 	 or a logic 0.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t GPIO_Write(port_gpio_t Port_select, write_output_t output );

/*!
 	 \brief This function reads the input of a GPIO pin.

 	 \param[in] Port_select Port and pin to be read. See port_gpio_t.
 	 \param[out] input Pointer to a variable where the GPIO will be read.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t GPIO_Read(port_gpio_t Port_select, uint8_t* input);

/*!
 	 \brief This function enables an interruption for a pin. The pin must be
 	 	 	 initialized first.

 	 \param[in] pin_select Port and pin's interruption to be enabled.
 	 \param[in] trigger Trigger that will cause the interruption.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t GPIO_InterruptEnable(interrupt_pins_t pin_select, gpio_interrupt_type_t trigger);

/*!
	\brief This function disables an interruption for a pin. The pin must be
			initialized first.

	\param[in] pin_select Port and pin's interruption to be disabled.

	\return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t GPIO_InterruptDisable(interrupt_pins_t pin_select);

/*!
 	 \brief This function gets the interrupt status of a specific pin.
 	 	 	 The pin's interruption must be enabled first.

 	 \param[in] pin_select Port and pin's interruption to be read.
 	 	 	 	 See port_gpio_t.
 	 \param[out] interrupt_flag The state of the pin's interruption.

 	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t GPIO_GetInterruptionFlag(interrupt_pins_t pin_select,
											interrupt_status_t* interrupt_flag);

/*!
 	 \brief This function clears the interrupt flag of a specific pin.
 	 	 	 The pin's interruption must be enabled first.

 	 \param[in] pin_select Port and pin's interruption to be cleared.
 	 	 	 	 See port_gpio_t.

	 \return Indicates if an error occurred.
 	 	 	 Returns 0 for success.
 */
general_codes_t GPIO_ClearInterruptionFlag(interrupt_pins_t pin_select);

/*!
 	 \}
 */
/********************************************************************************************************************************
 * 		GPIO
 *********************************************************************************************************************************/

/********************************************************************************************************************************
 * 		ADC
 *********************************************************************************************************************************/

/*!
 	 \name ADC functions
 	 \{
 */

/*!
 	 \brief This function sets the maximum time for the ADC to read. After this time,
 			the read function will be exited.

 	 \param[in] timeout Time limit in microseconds.

 	 \return void.
 */
void ADC_Set_Timeout(uint32_t timeout);
/*!
 	 \brief This function returns the maximum time for the ADC to read. After this time,
 	 	 	 the read function will be exited.

 	 \return Time limit in microseconds.
 */
uint32_t ADC_Get_Timeout(void);

#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
/*!
   \brief This function initializes and configures the 16-bit ADC, using the PD5 as
   	   	   the input.

   \param[in] init_adc_struct    Configuration values for the ADC. See adc16_config_t.
   \param[in] get_default_config Whether the configuration used will be the default one
   	   	   	   	   	   	   	   	   or the user-defined one.
   \return Indicates if an error occurred.
   	   	   Returns 0 for success.
 */
general_codes_t ADC_Initialization(ADC_init_t init_adc_struct, configuration_select_t get_default_config);

/*!
   \brief This function reads a value from the ADC.

   \param[out] adc_value_read Pointer to the variable where the data will be read.

   \return Indicates if an error occurred.
   	   	   Returns 0 for success.
 */
general_codes_t ADC_Read(uint16_t *adc_value_read);
#endif

#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
/*!
   \brief This function initializes and configures the 16-bit ADC

   \param[in] alternative The pin to be used as ADC.
   \param[in] init_adc_struct    Configuration values for the ADC. See adc16_config_t.
   \param[in] get_default_config Whether the configuration used will be the default one
   	   	   	   	   	   	   	   	   or the user-defined one.

   \return Indicates if an error occurred.
   	   	   Returns 0 for success.
 */
general_codes_t ADC_Initialization(adc_alternatives_t alternative, ADC_init_t init_adc_struct,
							configuration_select_t get_default_config);

/*!
   \brief This function reads a value from the ADC.

   \param[in] alternative The pin to be used as ADC.
   \param[out] adc_value_read Pointer to the variable where the data will be read.

   \return Indicates if an error occurred.
   	   	   Returns 0 for success.
 */
general_codes_t ADC_Read(adc_alternatives_t alternative, uint16_t *adc_value_read);
#endif

/*!
 	 \}
 */

/********************************************************************************************************************************
 * 		ADC
 *********************************************************************************************************************************/

/********************************************************************************************************************************
 * 		I2C
 *********************************************************************************************************************************/

/*!
 	 \name I2C functions
 	 \{
 */

/*!
   \brief This function initializes the I2C.

   \param[in] i2c_alternatives Combination of I2C pins to be used.
   \param[in] i2c_master_config_struct Configuration values for the I2C. See i2c_master_config_t.
   \param[in] get_default_config       Whether the configuration used will be the default one,
   	   	   	   	   	   	   	   	   	   or the user-defined one.

   \return Indicates if an error occurred.
   	   	   Returns 0 for success.
 */
general_codes_t I2C_Initialization(i2c_alternatives_t i2c_alternatives,
							I2C_init_t i2c_master_config_struct,
							configuration_select_t get_default_config );

general_codes_t I2C_Deinitialization(i2c_alternatives_t i2c_alternatives);

/*!
   \brief This function writes data into the I2C.

   \param[in] i2c_alternatives 	 I2C where the data will be written.
   \param[in] slave_address      The slave address.
   \param[in] i2c_master_tx_buff Pointer to the data to be sent.
   \param[in] i2c_data_length    The length of the data to be written.

   \return Indicates if an error occurred.
   	   	   Returns 0 for success.
 */
general_codes_t I2C_Write(i2c_alternatives_t i2c_alternatives, uint8_t slave_address,
                  uint8_t *i2c_master_tx_buff, uint8_t i2c_data_length);


/*!
   \brief This function reads data from the I2C. Must write the register address first
   	   	   using I2C_write.

   \param[in] i2c_alternatives	 I2C from where the data will be read.
   \param[in] slave_address      The slave-to-be-read-from address.
   \param[in] i2c_data_length    The length of the data to be read.
   \param[in] i2c_master_rx_buff Pointer to the variable where the data will be read.

   \return Indicates if an error occurred.
   	   	   Returns 0 for success.
 */
general_codes_t I2C_Read(i2c_alternatives_t i2c_alternatives, uint8_t slave_address,
                  uint8_t *i2c_master_rx_buff, uint8_t i2c_data_length);

/*!
 	 \brief This function sets the maximum time for the I2C to read/write. After this time,
 	 	 	 the read/write functions will exit.

 	 \param[in] timeout Time limit in microseconds.

 	 \return void.
 */
void I2C_Set_Timeout(uint32_t timeout);

/*!
 	 \brief This function returns the maximum time for the I2C to read/write. After this time,
 	 	 	 the read/write functions will exit.

 	 \return Time limit in microseconds.
 */
uint32_t I2C_Get_Timeout(void);

/*!
 	 \}
 */

/********************************************************************************************************************************
 * 		I2C
 *********************************************************************************************************************************/

/********************************************************************************************************************************
 * 		General functions
 *********************************************************************************************************************************/

/*!
 	 \name General functions
 	 \{
 */

/*!
  \brief This function stalls the microprocessor for the desired amount of time.

  \param[in] delay Time the microprocessor will be stalled in microseconds.

   \return Indicates if an error occurred.
   	   	   Returns 0 for success.
 */
general_codes_t Delay(uint32_t delay);

/*!
	\brief This function enables/disables the voltage in the sensors header.

	\param[in] state Whether the header's voltage is on or off.

 	\return void.
 */
void VDD_Sensors_Control(VDD_state_t state);

/*!
 	 \brief This function returns the voltage state in the sensors header.

 	 \return Whether the voltage is on or off.
 */
VDD_state_t VDD_Sensors_Get_State(void);

#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
/*!
 	 \brief This function enables/disables the active antenna mode in the GPS.

	 \warning Turn the Active Antenna Mode on ONLY when an active antenna is
	 	 	 	 connected. Otherwise, the device's components may burn.

 	 \param[in] state Whether the active antenna mode will be turned on or off.

 	 \return void.
 */
void GPS_Active_Antenna_Control(antenna_state_t state);

/*!
 	 \brief This function returns the Active Antenna Mode in the GPS.

 	 \return Whether the antenna is on or off.
 */
antenna_state_t GPS_Get_State(void);

/*!
     \brief This function turns on/off the general purpose LED.

     \param[in] state Whether the general purpose LED will be turned on
                         or turned off.

     \return void.
 */
void LED_Control(led_state_t state);

/*!
     \brief This function returns the general purpose LED state.

     \return Whether the LED is on or off.
 */
led_state_t LED_Get_State(void);
#endif

#if(defined(ATHEN_BOARD) && (ATHEN_BOARD))

/*!
     \brief This function controls the LED in the shield
             nearest to the GPS antenna.

     \param[in] state Whether the LED will be turned on or off.

     \return void.
 */
void LED_1_Control(led_state_t state);

/*!
     \brief This function controls the LED in the shield
             farthest from the GPS antenna.

     \param[in] state Whether the LED will be turned on or off.

     \return void.
 */
void LED_2_Control(led_state_t state);

/*!
     \brief This function controls the LED in Athen Sense System.

     \param[in] state Whether the LED will be turned on or off.

     \return void.
 */
void LED_Control(led_state_t state);

/*!
     \brief This function returns the state of the LED in
             the shield nearest to the GPS antenna.

     \return Whether the LED is on or off.
 */
led_state_t LED_1_Get_State(void);

/*!
     \brief This function returns the state of the LED in
             the shield farthest to the GPS antenna.

     \return Whether the LED is on or off.
 */
led_state_t LED_2_Get_State(void);

/*!
     \brief This function returns the state of the LED in
             Athen Sense System.

     \return Whether the LED is on or off.
 */
led_state_t LED_Get_State(void);

#endif

/*!
     \brief This function initializes the board.

     \return void.
 */
void InitConfigBoard(void);

/*!
 	 \}
 */

/********************************************************************************************************************************
 * 		General functions
 *********************************************************************************************************************************/

/********************************************************************************************************************************
 * 		RTC
 *********************************************************************************************************************************/

/*!
 	 \name RTC functions
 	 \{
 */

void RTC_Initialization();

void RTC_Start();

void RTC_Enable_Interrupt(RTC_interrupt_t interrupt);

general_codes_t RTC_Set_Time(rtc_datetime_t *datetime);

void RTC_Set_Time_Seconds(uint32_t seconds);

general_codes_t RTC_Set_Alarm(rtc_datetime_t *datetime);

void RTC_Set_Alarm_Seconds(uint32_t seconds);

void RTC_Get_Time(rtc_datetime_t *datetime);

void RTC_Get_Time_Seconds(uint32_t *datetime_seconds);

void RTC_Stop();

void RTC_Get_Alarm_Flag(bool *alarm_flag);

void RTC_Clean_Alarm_Flag();

void RTC_Get_Alarm_Seconds(uint32_t *alarm_seconds);

void RTC_Get_Alarm_Seconds_Flag(bool *seconds_flag);

void RTC_Clean_Alarm_Seconds_Flag();

void RTC_Reset_Alarm_Seconds();


bool UART_get_interrupt_flag(void);
void UART_clean_interrupt_flag(void);
void UART_set_interrupt_flag(void);

/*!
 	 \}
 */

/********************************************************************************************************************************
 * 		RTC
 *********************************************************************************************************************************/

//////////////////////////End of Functions Description//////////////////////////

/////////////////////////////Include Guard Closing//////////////////////////////
#endif /* INIT_CONFIG_FUNCTIONS_H_ */
/////////////////////////End of Include Guard Closing///////////////////////////
