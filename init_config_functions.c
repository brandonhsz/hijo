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


#include <drivers/fsl_common.h>
#include <drivers/fsl_gpio.h>
#include <drivers/fsl_llwu.h>
#include <drivers/fsl_lptmr.h>
#include <drivers/fsl_pmc.h>
#include <drivers/fsl_rcm.h>
#include <drivers/fsl_smc.h>
#include <state_machine.h>
#include "init_config_functions.h"

#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
#include "fuel_gauge_lite.h"
#endif

#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
#include "fuel_gauge_sense.h"
#endif

/////////////////////////////Definitions Area///////////////////////////////////
#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
#define RI_PORT_INTERRUPT 		(0x08)
#endif

#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
#define RI_PORT_INTERRUPT 		(0x02)//aqui fue cambio
#endif

#define ADC16_BASE 					ADC0
#define ADC16_CHANNEL_GROUP 		0U

#define PCR_PE_ENABLED              0x01u   /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
#define PCR_PS_UP                   0x01u   /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */

#define PCR_PULL_ENABLE_ENABLED     0x01u   /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
#define PCR_PULL_SELECT_UP          0x01u   /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */

#define IS_INIT						(0)
#define NOT_INIT					(1)

#define NOT_INTERRUPTED				(0)
#define INTERRUPTED					(1)

#define PTC0_MASK					(0x00000001)
#define PTC1_MASK					(0x00000002)
#define PTC2_MASK					(0x00000004)
#define PTC3_MASK					(0x00000008)
#define PTC4_MASK					(0x00000010)

#define PTD1_MASK                   (0x00000002)
#define PTD4_MASK					(0x00000010)
#define PTD5_MASK					(0x00000020)
#define PTD6_MASK					(0x00000040)

#define MIN_DELAY					(200)
//////////////////////////End of Definitions Area///////////////////////////////

struct
{
	lpuart_alternatives_t LPUART_alternative;
	LPUART_init_t lpuartConfigStruct;
	configuration_select_t default_configuration;
	bool flag;
}g_userInit_lpuart = {0};

struct
{
	bool flag;
}g_userInit_pit = {0};

#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
struct
{
	UART_init_t uartConfigStruct;
	configuration_select_t default_configuration;
	bool flag;
}g_userInit_uart = {0};

struct
{
	spi_alternatives_t SPI_alternative;
	SPI_Master_init_t masterConfigStruct;
	configuration_select_t default_configuration;
	bool flag;
}g_userInit_spiMaster = {0};

struct
{
	spi_alternatives_t SPI_alternative;
	SPI_Slave_init_t slaveConfigStruct;
	configuration_select_t default_configuration;
	bool flag;
}g_userInit_spiSlave = {0};
#endif

uint32_t g_userInit_shield_baudRate = 0;
bool g_userInit_shield_flag = false;

static void Enable_Clock_port(PORT_Type *port_base);
uint8_t Pins_UART(PORT_Type *port_base, uint32_t pin_idx);
uint32_t Init_Pins_UART(PORT_Type *port_base, uint32_t pin_idx);
uint32_t Init_Pins_LPUART(PORT_Type *port_base, uint32_t pin_idx);
uint8_t Pins_LPUART(PORT_Type *port_base, uint32_t pin_idx);
uint32_t Init_Pins_SPI(PORT_Type *port_base, uint32_t pin_idx);
uint8_t Pins_SPI(PORT_Type *port_base, uint32_t pin_idx);
uint32_t Init_Pins_ADC(PORT_Type *port_base, uint32_t pin_idx);
uint32_t Pins_ADC(PORT_Type *port_base, uint32_t pin_idx);
uint32_t Init_Pin_I2C(PORT_Type *port_base, uint32_t pin_idx);
uint8_t Pins_I2C(PORT_Type *port_base, uint32_t pin_idx);
uint32_t Init_Pins_GPIO(PORT_Type *port_base, uint32_t pin_idx);
uint32_t Pins_GPIO(PORT_Type *port_base, uint32_t pin_idx);
uint32_t Init_Pins_GPIO_llwu(PORT_Type *port_base, uint32_t pin_idx);
uint8_t Pins_GPIO_llwu(PORT_Type *port_base, uint32_t pin_idx);

// Function to enable clock port
static void Enable_Clock_port(PORT_Type *port_base)
{

		if(port_base==PORTA){
			CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */
		}
		else if(port_base==PORTB){
			CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
		}
		else if(port_base==PORTC){
			CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
		}
		else if(port_base==PORTD){
			CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */
		}
		else if(port_base==PORTE){
			CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */
		}

}


/********************************************************************************************************************************
 * 		PIT
 *********************************************************************************************************************************/
static bool pit_channel_0_flag = false;
bool pit_channel_1_flag = false;

//Interruption for PIT handler
void PIT_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    if (kPIT_TimerFlag & PIT_GetStatusFlags(PIT, kPIT_Chnl_0))
    {

        pit_channel_0_flag = true;
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
    }
    /* Clear interrupt flag.*/
    if (kPIT_TimerFlag & PIT_GetStatusFlags(PIT, kPIT_Chnl_1))
    {

        pit_channel_1_flag = true;
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);
    }
}

// Initialization of the PIT timer
void PIT_Initialization(void){

	pit_config_t pitConfig;

	/* pitConfig.enableRunInDebug = false; */
	PIT_GetDefaultConfig(&pitConfig);
	/* Init pit module */
	PIT_Init(PIT, &pitConfig);
	/* Enable timer interrupts for channel 0 */
	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);

	/* Enable at the NVIC */
	EnableIRQ(PIT_IRQn);

	g_userInit_pit.flag = true;

}

void PIT_Deinitialization(void)
{
	PIT_Deinit(PIT);
	g_userInit_pit.flag = false;
}

// Star the timer of the PIT
void PIT_Start_Timer(uint32_t count_usec){

	/* Set timer period for channel 0 */
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(count_usec, CLOCK_GetFreq(kCLOCK_BusClk)));
	/* Start channel 0 */
	PIT_StartTimer(PIT, kPIT_Chnl_0);

}

// Stop the timer of the PIT
void PIT_Stop_Timer(void){

	PIT_StopTimer(PIT, kPIT_Chnl_0);
}

pit_flag_state_t PIT_Get_Flag(void){

	return pit_channel_0_flag;

}

void PIT_Clean_Flag(void){

	pit_channel_0_flag = false;

}
/********************************************************************************************************************************
 * 		PIT
 *********************************************************************************************************************************/


/**********************************************************************************************************************************
 * 		UART
 *********************************************************************************************************************************/

#if(((defined(ATHEN_SHIELD) && (!ATHEN_SHIELD))) || (defined(ATHEN_BOARD)) && (!ATHEN_BOARD))
static uart_handle_t g_uartHandle;
static uart_transfer_t g_uart_sendXfer;
static uart_transfer_t g_uart_receiveXfer;
static uint32_t g_uart_timeout = 1000;
static bool g_uart_tx_finished = false;
static bool g_uart_rx_finished = false;
static uint8_t g_uart_read_byte = 0;
static uint8_t g_uart_data_read = 0;

///////////////////////////////////////////////////////////// UART ///////////////////////////////////////////////////////////////////////////////////////////////


//Function to validate pins of UART
uint8_t Pins_UART(PORT_Type *port_base, uint32_t pin_idx){
	 if(port_base==PORTE){
		switch(pin_idx){

			case 22:
				return kPORT_MuxAlt4;
			break;
			case 23:
				return kPORT_MuxAlt4;
			break;
			default:
				return kPORT_PinDisabledOrAnalog;
			break;
		}
	 }

	 else if(port_base==PORTD){
		switch(pin_idx){
			case 2:
				return kPORT_MuxAlt3;
			break;
			case 3:
				return kPORT_MuxAlt3;
			break;
			case 4:
				return kPORT_MuxAlt3;
			break;
			case 5:
				return kPORT_MuxAlt3;
			break;
			default:
				return kPORT_PinDisabledOrAnalog;
			break;
		}
	 }
	return kPORT_PinDisabledOrAnalog;

	if(PORTE == port_base)
	{
		switch(pin_idx)
		{
		case 22:
			return kPORT_MuxAlt4;
		break;

		case 23:
			return kPORT_MuxAlt4;

		break;

		default:
			return kPORT_PinDisabledOrAnalog;
		break;
		}
	}

	else if(PORTD == port_base)
	{
		if(5 == pin_idx)
		{
			return kPORT_MuxAlt3;
		}
	}

	return kPORT_PinDisabledOrAnalog;
}


//Function to enable pins of UART
uint32_t Init_Pins_UART(PORT_Type *port_base, uint32_t pin_idx){

	if(Pins_UART(port_base,pin_idx)){
		Enable_Clock_port(port_base);
		PORT_SetPinMux(port_base,pin_idx,Pins_UART(port_base,pin_idx));
		return successful_task;
	}
	else{
		return initialization_failed;
	}

}
///////////////////////////////////////////////////////////// UART ///////////////////////////////////////////////////////////////////////////////////////////////

bool uart_interrupt_flag;


void UART2_FLEXIO_IRQHandler(void)
{
    UART_TransferHandleIRQ(UART2, &g_uartHandle);
}



void UART_Callback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData)
{
    if(kStatus_UART_RxIdle == status)
    {
    	g_uart_data_read++;
    	g_uart_rx_finished = true;
    }else if(kStatus_UART_TxIdle == status)
    {
    	g_uart_tx_finished = true;
    }
}

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))
general_codes_t UART_Initialization(UART_init_t uartConfigStruct, configuration_select_t default_configuration)
{
    uint32_t status = 0;
	pit_config_t pitConfig;
	uart_config_t uart_config;

	Init_Pins_UART(PORTD, PIN5_IDX);
	Init_Pins_UART(PORTD, PIN4_IDX);

	UART_GetDefaultConfig(&uart_config);
	if(!default_configuration)
	{
		uart_config.baudRate_Bps = uartConfigStruct.baudRate_Bps;
		uart_config.parityMode = uartConfigStruct.parityMode;
	}
	uart_config.enableTx = true;
	uart_config.enableRx = true;

    if(UART_Init(UART2, &uart_config, CLOCK_GetFreq(kCLOCK_BusClk))==kStatus_Success){
    	g_userInit_uart.uartConfigStruct = uartConfigStruct;
    	g_userInit_uart.default_configuration = default_configuration;
    	g_userInit_uart.flag = true;
    	status =  successful_task;
    }
    else{
    	status = initialization_failed;
    }

	UART_TransferCreateHandle(UART2, &g_uartHandle, UART_Callback, NULL);

	/* pitConfig.enableRunInDebug = false; */
	PIT_GetDefaultConfig(&pitConfig);
	/* Init pit module */
	PIT_Init(PIT, &pitConfig);
	/* Enable timer interrupts for channel 1 */
	PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
	/* Enable at the NVIC */
	EnableIRQ(PIT_IRQn);

    return status;
}
#endif

#if (defined(ATHEN_BOARD) && ATHEN_BOARD)
general_codes_t UART_Initialization(uart_alternatives_t alternative, UART_init_t uartConfigStruct, configuration_select_t default_configuration)
{
	uint32_t status = 0;
	pit_config_t pitConfig;
	uart_config_t uart_config;

	switch(alternative)
	{
		case UART_Alt0:
			Init_Pins_UART(PORTE, PIN23_IDX);
			Init_Pins_UART(PORTE, PIN22_IDX);
		break;

		case UART_Alt1:
			Init_Pins_UART(PORTE, PIN23_IDX);
			Init_Pins_UART(PORTD, PIN5_IDX);
		break;

		default:

		break;
	}

	// Default parameters used in the UART configuration
	/*
	 * uartConfigStruct.baudRate_Bps = 9600u;
	 * uartConfigStruct.parityMode = kUART_ParityDisabled;
	 * uartConfigStruct.stopBitCount = kUART_OneStopBit;
	 * uartConfigStruct.txFifoWatermark = 0;
	 * uartConfigStruct.rxFifoWatermark = 1;
	 * uartConfigStruct.enableTx = true;
	 * uartConfigStruct.enableRx = true;
	 */
	UART_GetDefaultConfig(&uart_config);
	if(!default_configuration){
		// Configuration of UART parameters
		uart_config.baudRate_Bps = uartConfigStruct.baudRate_Bps;
		uart_config.parityMode = uartConfigStruct.parityMode;
//		uartConfigStruct.baudRate_Bps = 9600u;
	}
	uart_config.enableTx = true;
	uart_config.enableRx = true;

	if(UART_Init(UART2, &uart_config, CLOCK_GetFreq(kCLOCK_BusClk))==kStatus_Success){
		status =  false;
	}
	else{
		status = true;
	}

	UART_TransferCreateHandle(UART2, &g_uartHandle, UART_Callback, NULL);

	/* pitConfig.enableRunInDebug = false; */
	PIT_GetDefaultConfig(&pitConfig);
	/* Init pit module */
	PIT_Init(PIT, &pitConfig);
	/* Enable timer interrupts for channel 1 */
	PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
	/* Enable at the NVIC */
	EnableIRQ(PIT_IRQn);

	return status;
}
#endif

void UART_Interrupt(uart_transfer_callback_t callback)
{
	UART_TransferCreateHandle(UART2, &g_uartHandle, callback, NULL);
}

void UART_clean_interrupt_flag(void)
{
	uart_interrupt_flag = false;
}

void UART_set_interrupt_flag(void)
{
	uart_interrupt_flag = true;
}

bool UART_get_interrupt_flag(void)
{
	return uart_interrupt_flag;
}

general_codes_t UART_Write(uint8_t* data, uint32_t data_size)
{
	uint32_t status = 0;

	g_uart_sendXfer.data = data;
	g_uart_sendXfer.dataSize = data_size;

	/* Set timer period for channel 1 */
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, USEC_TO_COUNT(g_uart_timeout, CLOCK_GetFreq(kCLOCK_BusClk)));
	/* Start channel 1 */
	PIT_StartTimer(PIT, kPIT_Chnl_1);

	UART_TransferSendNonBlocking(UART2, &g_uartHandle, &g_uart_sendXfer);

	while(!g_uart_tx_finished && !pit_channel_1_flag){}

	if(pit_channel_1_flag)
	{
		status = timeout_reached;
	}

	g_uart_tx_finished = false;
	pit_channel_1_flag = false;
	PIT_StopTimer(PIT, kPIT_Chnl_1);

	return status;
}

general_codes_t UART_Write_Data(uart_transaction_mode_t mode, uint8_t* data, uint32_t data_size)
{
	uint32_t status = successful_task;
	if(mode == UART_NonBlocking){
		while(data_size--)
		{
			UART_WriteByte(UART2, *data++);
		}
	}else if(mode == UART_Blocking)
	{
		UART_WriteBlocking(UART2, data, data_size);
	}
	return status;
}

general_codes_t UART_Read(uint8_t* data, uint32_t data_size){
	uint32_t status = successful_task;

	g_uart_receiveXfer.data = &g_uart_read_byte;
	g_uart_receiveXfer.dataSize = 1;

	/* Set timer period for channel 1 */
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, USEC_TO_COUNT(g_uart_timeout, CLOCK_GetFreq(kCLOCK_BusClk)));
	/* Start channel 1 */
	PIT_StartTimer(PIT, kPIT_Chnl_1);
	UART_TransferReceiveNonBlocking(UART2, &g_uartHandle, &g_uart_receiveXfer, NULL);

	while(g_uart_data_read <= data_size)
	{
		while(!g_uart_rx_finished && !pit_channel_1_flag){}
		if(pit_channel_1_flag)
		{
			status = timeout_reached;
			break;
		}
		(*data) = g_uart_read_byte;
		data ++;
		g_uart_rx_finished = false;
		UART_TransferReceiveNonBlocking(UART2, &g_uartHandle, &g_uart_receiveXfer, NULL);
		PIT_StopTimer(PIT, kPIT_Chnl_1);
		PIT_StartTimer(PIT, kPIT_Chnl_1);
	}
	g_uart_data_read = 0;
	g_uart_rx_finished = false;
	pit_channel_1_flag = false;
	PIT_StopTimer(PIT, kPIT_Chnl_1);

	return status;
}

general_codes_t UART_Read_Data(uart_transaction_mode_t mode, uint8_t* data, uint32_t data_size)
{
	uint32_t status = successful_task;
	if(mode == UART_NonBlocking){
			while(data_size--)
			{
				*data++ = UART_ReadByte(UART2);
			}

	}else if(mode == UART_Blocking)
	{
		status = UART_ReadBlocking(UART2, data, data_size);
	}
	return status;
}

void UART_Deinitialization(void)
{
	UART_Deinit(UART2);
	g_userInit_uart.flag = false;
}

void UART_Set_Timeout(uint32_t timeout)
{
	g_uart_timeout = timeout;
}

uint32_t UART_Get_Timeout(void)
{
	return g_uart_timeout;
}
#endif
/********************************************************************************************************************************
 * 		UART
 *********************************************************************************************************************************/


/**********************************************************************************************************************************
 * 		LPUART
 *********************************************************************************************************************************/
static lpuart_handle_t g_lpuartHandle;
static lpuart_transfer_t g_lpuart_sendXfer;
static lpuart_transfer_t g_lpuart_receiveXfer;
static uint32_t g_lpuart_timeout = 1000;
static bool g_lpuart_tx_finished = false;
static bool g_lpuart_rx_finished = false;
static uint8_t g_lpuart_read_byte = 0;
static uint8_t g_lpuart_data_read = 0;
static lpuart_handle_t g_lpuartUserHandle;

#if (defined(ATHEN_BOARD) && ATHEN_BOARD)
LPUART_Type* LPUART_USED = LPUART0;
#endif

#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
LPUART_Type* LPUART_USED = LPUART1;
#endif

//Function to enable pins of UART
uint32_t Init_Pins_LPUART(PORT_Type *port_base, uint32_t pin_idx){

    if(Pins_LPUART(port_base,pin_idx)){
        Enable_Clock_port(port_base);
        PORT_SetPinMux(port_base,pin_idx,Pins_LPUART(port_base,pin_idx));

        SIM->SOPT5 = ((SIM->SOPT5 &
        (~(SIM_SOPT5_LPUART1TXSRC_MASK | SIM_SOPT5_LPUART1RXSRC_MASK))) // Mask bits to zero which are setting
          | SIM_SOPT5_LPUART1TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX) // LPUART0 Transmit Data Source Select: LPUART0_TX pin
          | SIM_SOPT5_LPUART1RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX) // LPUART0 Receive Data Source Select: LPUART_RX pin
        );
        return successful_task;
    }
    else{
        return initialization_failed;
    }

}

//Function to validate pins of UART
uint8_t Pins_LPUART(PORT_Type *port_base, uint32_t pin_idx){

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))
     if(port_base==PORTC){
        switch(pin_idx){

            case 3:
                return kPORT_MuxAlt3;
            break;

            case 4:
                return kPORT_MuxAlt3;
            break;

            default:
                return kPORT_PinDisabledOrAnalog;
            break;
        }
     }
     if(port_base==PORTE){
        switch(pin_idx){

            case 0:
                return kPORT_MuxAlt3;
            break;

            case 1:
                return kPORT_MuxAlt3;
            break;

            default:
                return kPORT_PinDisabledOrAnalog;
            break;
        }
     }

    return kPORT_PinDisabledOrAnalog;
#endif

#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
    if(port_base == PORTD)
    {
    	if(pin_idx == PIN6_IDX)
    	{
    		return kPORT_MuxAlt3;
    	}
    }

    else if(port_base == PORTE)
    {
    	if(pin_idx == PIN20_IDX)
    	{
    		return kPORT_MuxAlt4;
    	}

    	else if(pin_idx == PIN21_IDX)
    	{
    		return kPORT_MuxAlt4;
    	}
    }

    return kPORT_PinDisabledOrAnalog;
#endif
}

void LPUART_Callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    // if a character is received through the LPUART,
    if(kStatus_LPUART_RxIdle == status)
    {
    	g_lpuart_data_read++;
    	g_lpuart_rx_finished = true;
    }else if(kStatus_LPUART_TxIdle == status)
    {
    	g_lpuart_tx_finished = true;
    }
}

general_codes_t LPUART_Initialization(lpuart_alternatives_t LPUART_alternative, LPUART_init_t lpuartConfigStruct,
		configuration_select_t default_configuration)
{
    uint32_t status = successful_task;
    pit_config_t pitConfig;
    lpuart_config_t lpuart_config;

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))
    switch(LPUART_alternative)
    {
		case LPUART_Alt0:
			Init_Pins_LPUART(PORTC, PIN4_IDX);
			Init_Pins_LPUART(PORTC, PIN3_IDX);
			break;

        case LPUART_Alt1:
            Init_Pins_LPUART(PORTE, PIN0_IDX);
            Init_Pins_LPUART(PORTE, PIN1_IDX);
            break;

        case LPUART_Alt2:
        	Init_Pins_LPUART(PORTE, PIN0_IDX);
        	Init_Pins_LPUART(PORTC, PIN3_IDX);
            break;

        case LPUART_Alt3:
        	Init_Pins_LPUART(PORTC, PIN4_IDX);
        	Init_Pins_LPUART(PORTE, PIN1_IDX);
            break;

        default:
        	Init_Pins_LPUART(PORTC, PIN4_IDX);
        	Init_Pins_LPUART(PORTC, PIN3_IDX);
            break;
    }
    CLOCK_SetLpuart1Clock(0x1U);
#endif

#if (defined(ATHEN_BOARD) && ATHEN_BOARD)
    switch(LPUART_alternative)
    {
		case LPUART_Alt0:
			Init_Pins_LPUART(PORTE, PIN20_IDX);
			Init_Pins_LPUART(PORTE, PIN21_IDX);
		break;

        case LPUART_Alt1:
            Init_Pins_LPUART(PORTE, PIN20_IDX);
            Init_Pins_LPUART(PORTD, PIN6_IDX);
		break;

        default:
        	Init_Pins_LPUART(PORTE, PIN20_IDX);
        	Init_Pins_LPUART(PORTE, PIN21_IDX);
		break;
    }
    CLOCK_SetLpuart0Clock(0x1U);
#endif



    LPUART_GetDefaultConfig(&lpuart_config);

    if(!default_configuration)
    {
    	lpuart_config.baudRate_Bps = lpuartConfigStruct.baudRate_Bps;
		lpuart_config.parityMode = lpuartConfigStruct.parityMode;
    }

    lpuart_config.enableTx = true;
    lpuart_config.enableRx = true;


    if(LPUART_Init(LPUART_USED, &lpuart_config, CLOCK_GetFreq(kCLOCK_CoreSysClk))==kStatus_Success){
    	status = successful_task;
    	g_userInit_lpuart.LPUART_alternative = LPUART_alternative;
    	g_userInit_lpuart.lpuartConfigStruct = lpuartConfigStruct;
    	g_userInit_lpuart.default_configuration = default_configuration;
    	g_userInit_lpuart.flag = true;
    }
    else{
    	status = initialization_failed;
    }

	LPUART_TransferCreateHandle(LPUART_USED, &g_lpuartHandle, LPUART_Callback, NULL);

	/* pitConfig.enableRunInDebug = false; */
	PIT_GetDefaultConfig(&pitConfig);
	/* Init pit module */
	PIT_Init(PIT, &pitConfig);
	/* Enable timer interrupts for channel 1 */
	PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
	/* Enable at the NVIC */
	EnableIRQ(PIT_IRQn);

    return status;
}


void LPUART_Interrupt(lpuart_transfer_callback_t callback)
{
	LPUART_TransferCreateHandle(LPUART_USED, &g_lpuartUserHandle, callback, NULL);
}

void LPUART_Read_Interrupt(uint8_t *data, uint32_t data_size)
{
	lpuart_transfer_t lpuart_receiveXfer;
	lpuart_receiveXfer.data = data;
	lpuart_receiveXfer.dataSize = data_size;
	LPUART_TransferReceiveNonBlocking(LPUART_USED, &g_lpuartUserHandle, &lpuart_receiveXfer, NULL);
}

void LPUART_Write_Interrupt(uint8_t *data, uint32_t data_size)
{
	lpuart_transfer_t lpuart_sendXfer;
	lpuart_sendXfer.data = data;
	lpuart_sendXfer.dataSize = data_size;
	LPUART_TransferSendNonBlocking(LPUART_USED, &g_lpuartUserHandle, &lpuart_sendXfer);
}

general_codes_t LPUART_Write(uint8_t* data, uint32_t data_size){
	uint32_t status = successful_task;

	g_lpuart_sendXfer.data = data;
	g_lpuart_sendXfer.dataSize = data_size;

	/* Set timer period for channel 1 */
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, USEC_TO_COUNT(g_lpuart_timeout, CLOCK_GetFreq(kCLOCK_BusClk)));
	/* Start channel 1 */
	PIT_StartTimer(PIT, kPIT_Chnl_1);

	LPUART_TransferSendNonBlocking(LPUART_USED, &g_lpuartHandle, &g_lpuart_sendXfer);

	while(!g_lpuart_tx_finished && !pit_channel_1_flag){}

	if(pit_channel_1_flag)
	{
		status = timeout_reached;
	}

	g_lpuart_tx_finished = false;
	pit_channel_1_flag = false;
	PIT_StopTimer(PIT, kPIT_Chnl_1);

	return status;
}

general_codes_t LPUART_Write_Data(lpuart_transaction_mode_t mode, uint8_t* data, uint32_t data_size)
{
	uint32_t status = successful_task;
	if(mode == LPUART_NonBlocking){
		while(data_size--)
		{
			LPUART_WriteByte(LPUART_USED, *data++);
		}

	}else if(mode == LPUART_Blocking)
	{
		LPUART_WriteBlocking(LPUART_USED, data, data_size);
	}
	return status;
}

general_codes_t LPUART_Read(uint8_t* data, uint32_t data_size){
	uint32_t status = successful_task;

	g_lpuart_receiveXfer.data = &g_lpuart_read_byte;
	g_lpuart_receiveXfer.dataSize = 1;

	/* Set timer period for channel 1 */
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, USEC_TO_COUNT(g_lpuart_timeout, CLOCK_GetFreq(kCLOCK_BusClk)));
	/* Start channel 1 */
	PIT_StartTimer(PIT, kPIT_Chnl_1);
	LPUART_TransferReceiveNonBlocking(LPUART_USED, &g_lpuartHandle, &g_lpuart_receiveXfer, NULL);

	while(data_size--)
	{
		while(!g_lpuart_rx_finished && !pit_channel_1_flag){}
		if(pit_channel_1_flag)
		{
			status = timeout_reached;
			break;
		}
		*(data++) = g_lpuart_read_byte;
		g_lpuart_data_read--;
		g_lpuart_rx_finished = false;
		g_lpuart_receiveXfer.data = &g_lpuart_read_byte;
		g_lpuart_receiveXfer.dataSize = 1;
		LPUART_TransferReceiveNonBlocking(LPUART_USED, &g_lpuartHandle, &g_lpuart_receiveXfer, NULL);
		PIT_StopTimer(PIT, kPIT_Chnl_1);
		PIT_StartTimer(PIT, kPIT_Chnl_1);
	}
	g_lpuart_rx_finished = false;
	pit_channel_1_flag = false;
	PIT_StopTimer(PIT, kPIT_Chnl_1);

	return status;
}

general_codes_t LPUART_Read_Data(lpuart_transaction_mode_t mode, uint8_t* data, uint32_t data_size)
{
	uint32_t status = successful_task;
	if(mode == LPUART_NonBlocking){
		while(data_size--)
		{
			*(data++) = LPUART_ReadByte(LPUART_USED);
            Delay(1000);

		}
	}else if(mode == LPUART_Blocking)
	{
		status = LPUART_ReadBlocking(LPUART_USED, data, data_size);
	}
	return status;
}

void LPUART_Deinitialization(void)
{
	LPUART_Deinit(LPUART_USED);
	g_userInit_lpuart.flag = false;
}

void LPUART_Set_Timeout(uint32_t timeout)
{
	g_lpuart_timeout = timeout;
}

uint32_t LPUART_Get_Timeout(void)
{
	return g_lpuart_timeout;
}

/********************************************************************************************************************************
 * 		LPUART
 *********************************************************************************************************************************/



/********************************************************************************************************************************
 * 		SPI
 *********************************************************************************************************************************/


/////////////////////////////////////////////////////////////GPIO/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))
uint8_t PTC3_intr_status = NOT_INTERRUPTED;
uint8_t PTC4_intr_status = NOT_INTERRUPTED;
uint8_t PTD4_intr_status = NOT_INTERRUPTED;
#endif

#if (defined(ATHEN_BOARD) && (ATHEN_BOARD))
uint8_t PTC0_intr_status = NOT_INTERRUPTED;
uint8_t PTC1_intr_status = NOT_INTERRUPTED;
uint8_t PTC2_intr_status = NOT_INTERRUPTED;
uint8_t PTD1_intr_status = NOT_INTERRUPTED;
uint8_t PTD6_intr_status = NOT_INTERRUPTED;
#endif

uint8_t PTD5_intr_status = NOT_INTERRUPTED;

uint8_t g_interrupt_ri = 0;
volatile bool g_interrupt_ri_flag = 0;

void PORTC_PORTD_IRQHandler(void)
{
    uint32_t portC_interrupt = 0;
    uint32_t portD_interrupt = 0;

    portC_interrupt = GPIO_PortGetInterruptFlags(GPIOC);
    portD_interrupt = GPIO_PortGetInterruptFlags(GPIOD);

#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
    if(portC_interrupt)
    {
    	switch(portC_interrupt)
    	{
    		case RI_PORT_INTERRUPT:
    			g_interrupt_ri++;
    			g_interrupt_ri_flag=true;
    		break;

			case PTC0_MASK:
				PTC0_intr_status = INTERRUPTED;
			break;

			case PTC1_MASK:
				PTC1_intr_status = INTERRUPTED;
			break;

			case PTC2_MASK:
				PTC2_intr_status = INTERRUPTED;
			break;

    	}

    	GPIO_PortClearInterruptFlags(GPIOC, portC_interrupt);
    }

    else if(portD_interrupt)
    {
    	switch(portD_interrupt)
    	{
            case PTD1_MASK:
                PTD1_intr_status = INTERRUPTED;
            break;

    		case PTD5_MASK:
    			PTD5_intr_status = INTERRUPTED;
    		break;

    		case PTD6_MASK:
    			PTD6_intr_status = INTERRUPTED;
			break;
    	}

    	GPIO_PortClearInterruptFlags(GPIOD, portD_interrupt);
    }
#endif

#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
    if(portC_interrupt)
    {
    	switch(portC_interrupt)
    	{
    		case RI_PORT_INTERRUPT:
    			g_interrupt_ri++;
    			g_interrupt_ri_flag=true;
    		break;

			case PTC3_MASK:
				PTC3_intr_status = INTERRUPTED;
			break;

			case PTC4_MASK:
				PTC4_intr_status = INTERRUPTED;
			break;
    	}

    	GPIO_PortClearInterruptFlags(GPIOC, portC_interrupt);
    }

    else if(portD_interrupt)
    {
    	switch(portD_interrupt)
    	{
			case PTD4_MASK:
				PTD4_intr_status = INTERRUPTED;
			break;

    		case PTD5_MASK:
    			PTD5_intr_status = INTERRUPTED;
    		break;

    	}

    	GPIO_PortClearInterruptFlags(GPIOD, portD_interrupt);
    }
#endif

}

//Function to enable pins of GPIO
uint32_t Init_Pins_GPIO(PORT_Type *port_base, uint32_t pin_idx){

	if(Pins_GPIO(port_base,pin_idx)){

	    Enable_Clock_port(port_base);
		PORT_SetPinMux(port_base, pin_idx, kPORT_MuxAsGpio);

		return successful_task;
	}
	else{
		return initialization_failed;
	}
}

//Function to validate pins of GPIO
uint32_t Pins_GPIO(PORT_Type *port_base, uint32_t pin_idx){

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))
    if(port_base==PORTA ){

        switch(pin_idx){

            case 4:
                return kPORT_MuxAsGpio;
            break;

            default:
                return kPORT_PinDisabledOrAnalog;
            break;
        }
    }

    else if(port_base==PORTB ){

		switch(pin_idx){

			default:
				return kPORT_PinDisabledOrAnalog;
			break;
		}
	}

	else if(port_base==PORTC){

		switch(pin_idx){
			case 1:
				return kPORT_MuxAsGpio;
			break;
            case 2:
                return kPORT_MuxAsGpio;
            break;
            case 3:
                return kPORT_MuxAsGpio;
            break;
            case 4:
                return kPORT_MuxAsGpio;
            break;
            case 5:
                return kPORT_MuxAsGpio;
            break;
            case 6:
                return kPORT_MuxAsGpio;
            break;
            case 7:
                return kPORT_MuxAsGpio;
            break;
			default:
				return kPORT_PinDisabledOrAnalog;
			break;

		}

	}

	else if(port_base==PORTD){

		switch(pin_idx){
            case 4:
                return kPORT_MuxAsGpio;
            break;
		    case 5:
                return kPORT_MuxAsGpio;
            break;
		    case 6:
				return kPORT_MuxAsGpio;
			break;
            case 7:
                return kPORT_MuxAsGpio;
            break;
			default:
				return kPORT_PinDisabledOrAnalog;
			break;
		}
	}

	else if(port_base==PORTE ){

		switch(pin_idx){
			case 0:
				return kPORT_MuxAsGpio;
			break;
            case 1:
                return kPORT_MuxAsGpio;
            break;
            case 17:
                return kPORT_MuxAsGpio;
            break;
            case 30:
                return kPORT_MuxAsGpio;
            break;
			default:
				return kPORT_PinDisabledOrAnalog;
			break;
		}
	}

	else{
		return kPORT_PinDisabledOrAnalog;
	}
#endif

#if (defined(ATHEN_BOARD) && ATHEN_BOARD)
    if(port_base == PORTC)
    {
    	switch(pin_idx)
    	{
			case 0:
				return kPORT_MuxAsGpio;
			break;

			case 1:
				return kPORT_MuxAsGpio;
			break;

			case 2:
				return kPORT_MuxAsGpio;
			break;

			default:
				return kPORT_PinDisabledOrAnalog;
			break;
    	}
    }

    else if(port_base == PORTD)
    {
    	switch(pin_idx)
    	{
            case 1:
                return kPORT_MuxAsGpio;
            break;

			case 5:
				return kPORT_MuxAsGpio;
			break;

			case 6:
				return kPORT_MuxAsGpio;
			break;

			default:
				return kPORT_PinDisabledOrAnalog;
			break;
    	}
    }

    else if(port_base == PORTE)
    {
    	switch(pin_idx)
    	{
    		case 20:
				return kPORT_MuxAsGpio;
    		break;

    		case 21:
				return kPORT_MuxAsGpio;
			break;

    		case 22:
				return kPORT_MuxAsGpio;
			break;

    		case 23:
				return kPORT_MuxAsGpio;
			break;

    		case 29:
				return kPORT_MuxAsGpio;
			break;

    		case 30:
				return kPORT_MuxAsGpio;
			break;

			default:
				return kPORT_PinDisabledOrAnalog;
			break;
    	}
    }

    return kPORT_PinDisabledOrAnalog;
#endif
}

general_codes_t GPIO_Initialization(port_gpio_t Port_select, port_mode_t select_input_output){

    PORT_Type *port_base;
    GPIO_Type *gpio_base;
    uint8_t pin_idx;

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))
    switch(Port_select){

        case Port_E0:

            CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */
            port_base= PORTE;
            gpio_base= GPIOE;
            pin_idx= PIN0_IDX;
        break;

        case Port_E1:

            CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */
            port_base= PORTE;
            gpio_base= GPIOE;
            pin_idx= PIN1_IDX;
        break;

        case Port_C3:

            CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
            port_base= PORTC;
            gpio_base= GPIOC;
            pin_idx= PIN3_IDX;
        break;

        case Port_C4:

            CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
            port_base= PORTC;
            gpio_base= GPIOC;
            pin_idx= PIN4_IDX;
        break;

        case Port_D4:

            CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */
            port_base= PORTD;
            gpio_base= GPIOD;
            pin_idx= PIN4_IDX;
        break;

        case Port_D5:

            CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */
            port_base= PORTD;
            gpio_base= GPIOD;
            pin_idx= PIN5_IDX;
        break;

        default:
            return GPIO_pin_not_available;
        break;
    }
#endif

#if (defined(ATHEN_BOARD) && ATHEN_BOARD)
    switch(Port_select)
    {
		case Port_C0:
            CLOCK_EnableClock(kCLOCK_PortC);                           /* Port D Clock Gate Control: Clock enabled */
            port_base= PORTC;
            gpio_base= GPIOC;
            pin_idx= PIN0_IDX;
		break;

		case Port_C1:
            CLOCK_EnableClock(kCLOCK_PortC);                           /* Port D Clock Gate Control: Clock enabled */
            port_base= PORTC;
            gpio_base= GPIOC;
            pin_idx= PIN1_IDX;
		break;

		case Port_C2:
            CLOCK_EnableClock(kCLOCK_PortC);                           /* Port D Clock Gate Control: Clock enabled */
            port_base= PORTC;
            gpio_base= GPIOC;
            pin_idx= PIN2_IDX;
		break;

        case Port_D1:
            CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */
            port_base= PORTD;
            gpio_base= GPIOD;
            pin_idx= PIN1_IDX;
        break;

		case Port_D5:
            CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */
            port_base= PORTD;
            gpio_base= GPIOD;
            pin_idx= PIN5_IDX;
		break;

		case Port_D6:
            CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */
            port_base= PORTD;
            gpio_base= GPIOD;
            pin_idx= PIN6_IDX;
		break;

		case Port_E20:
            CLOCK_EnableClock(kCLOCK_PortE);                           /* Port D Clock Gate Control: Clock enabled */
            port_base= PORTE;
            gpio_base= GPIOE;
            pin_idx= PIN20_IDX;
		break;

		case Port_E21:
            CLOCK_EnableClock(kCLOCK_PortE);                           /* Port D Clock Gate Control: Clock enabled */
            port_base= PORTE;
            gpio_base= GPIOE;
            pin_idx= PIN21_IDX;
		break;

		case Port_E22:
            CLOCK_EnableClock(kCLOCK_PortE);                           /* Port D Clock Gate Control: Clock enabled */
            port_base= PORTE;
            gpio_base= GPIOE;
            pin_idx= PIN22_IDX;
		break;

		case Port_E23:
            CLOCK_EnableClock(kCLOCK_PortE);                           /* Port D Clock Gate Control: Clock enabled */
            port_base= PORTE;
            gpio_base= GPIOE;
            pin_idx= PIN23_IDX;
		break;

		case Port_E29:
            CLOCK_EnableClock(kCLOCK_PortE);                           /* Port D Clock Gate Control: Clock enabled */
            port_base= PORTE;
            gpio_base= GPIOE;
            pin_idx= PIN29_IDX;
		break;

		case Port_E30:
            CLOCK_EnableClock(kCLOCK_PortE);                           /* Port D Clock Gate Control: Clock enabled */
            port_base= PORTE;
            gpio_base= GPIOE;
            pin_idx= PIN30_IDX;
		break;

		default:
			return GPIO_pin_not_available;
		break;
    }
#endif

    gpio_pin_config_t config;

    if(select_input_output){

        config.pinDirection= kGPIO_DigitalInput;
        config.outputLogic= 0;
    }
    else{

        config.pinDirection= kGPIO_DigitalOutput;
        config.outputLogic= 0;
    }

    PORT_SetPinMux(port_base, pin_idx, kPORT_MuxAsGpio);

    GPIO_PinInit(gpio_base,pin_idx,&config);

	return successful_task;
}

general_codes_t GPIO_Write(port_gpio_t Port_select, write_output_t output ){
    GPIO_Type *gpio_base;
    uint8_t pin_idx;

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))
    switch(Port_select){

        case Port_E0:

            gpio_base= GPIOE;
            pin_idx= PIN0_IDX;
        break;

        case Port_E1:

            gpio_base=GPIOE;
            pin_idx= PIN1_IDX;
        break;

        case Port_C3:

            gpio_base=GPIOC;
            pin_idx= PIN3_IDX;
        break;

        case Port_C4:

            gpio_base=GPIOC;
            pin_idx= PIN4_IDX;
        break;

        case Port_D4:

            gpio_base=GPIOD;
            pin_idx= PIN4_IDX;
        break;

        case Port_D5:

            gpio_base=GPIOD;
            pin_idx= PIN5_IDX;
        break;

        default:
            return GPIO_pin_not_available;
        break;
    }

#endif

#if (defined(ATHEN_BOARD) && ATHEN_BOARD)

    switch(Port_select)
    {
		case Port_C0:
			gpio_base = GPIOC;
			pin_idx = PIN0_IDX;
		break;

		case Port_C1:
			gpio_base = GPIOC;
			pin_idx = PIN1_IDX;
		break;

		case Port_C2:
			gpio_base = GPIOC;
			pin_idx = PIN2_IDX;
		break;

        case Port_D1:
            gpio_base = GPIOD;
            pin_idx = PIN1_IDX;
        break;

		case Port_D5:
			gpio_base = GPIOD;
			pin_idx = PIN5_IDX;
		break;

		case Port_D6:
			gpio_base = GPIOD;
			pin_idx = PIN6_IDX;
		break;

		case Port_E20:
			gpio_base = GPIOE;
			pin_idx = PIN20_IDX;
		break;

		case Port_E21:
			gpio_base = GPIOE;
			pin_idx = PIN21_IDX;
		break;

		case Port_E22:
			gpio_base = GPIOE;
			pin_idx = PIN22_IDX;
		break;

		case Port_E23:
			gpio_base = GPIOE;
			pin_idx = PIN23_IDX;
		break;

		case Port_E29:
			gpio_base = GPIOE;
			pin_idx = PIN29_IDX;
		break;

		case Port_E30:
			gpio_base = GPIOE;
			pin_idx = PIN30_IDX;
		break;

		default:
			return GPIO_pin_not_available;
		break;
    }

#endif

    GPIO_PinWrite(gpio_base, pin_idx, output);

    return successful_task;
}

general_codes_t GPIO_Read(port_gpio_t Port_select, uint8_t* input)
{
	GPIO_Type *gpio_base;
	uint8_t pin_idx;

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))

	switch(Port_select){

		case Port_E0:

			gpio_base= GPIOE;
			pin_idx= PIN0_IDX;
		break;

		case Port_E1:

			gpio_base=GPIOE;
			pin_idx= PIN1_IDX;
		break;

		case Port_C3:

			gpio_base=GPIOC;
			pin_idx= PIN3_IDX;
		break;

		case Port_C4:

			gpio_base=GPIOC;
			pin_idx= PIN4_IDX;
		break;

		case Port_D4:

			gpio_base=GPIOD;
			pin_idx= PIN4_IDX;
		break;

		case Port_D5:

			gpio_base=GPIOD;
			pin_idx= PIN5_IDX;
		break;

		default:
			return GPIO_pin_not_available;
		break;
	}

#endif

#if (defined(ATHEN_BOARD) && ATHEN_BOARD)

    switch(Port_select)
    {
		case Port_C0:
			gpio_base = GPIOC;
			pin_idx = PIN0_IDX;
		break;

		case Port_C1:
			gpio_base = GPIOC;
			pin_idx = PIN1_IDX;
		break;

		case Port_C2:
			gpio_base = GPIOC;
			pin_idx = PIN2_IDX;
		break;

        case Port_D1:
            gpio_base = GPIOD;
            pin_idx = PIN1_IDX;
        break;

		case Port_D5:
			gpio_base = GPIOD;
			pin_idx = PIN5_IDX;
		break;

		case Port_D6:
			gpio_base = GPIOD;
			pin_idx = PIN6_IDX;
		break;

		case Port_E20:
			gpio_base = GPIOE;
			pin_idx = PIN20_IDX;
		break;

		case Port_E21:
			gpio_base = GPIOE;
			pin_idx = PIN21_IDX;
		break;

		case Port_E22:
			gpio_base = GPIOE;
			pin_idx = PIN22_IDX;
		break;

		case Port_E23:
			gpio_base = GPIOE;
			pin_idx = PIN23_IDX;
		break;

		case Port_E29:
			gpio_base = GPIOE;
			pin_idx = PIN29_IDX;
		break;

		case Port_E30:
			gpio_base = GPIOE;
			pin_idx = PIN30_IDX;
		break;

		default:
			return GPIO_pin_not_available;
		break;
    }

#endif

	*input = GPIO_PinRead(gpio_base, pin_idx);

	return successful_task;
}

general_codes_t GPIO_InterruptEnable(interrupt_pins_t pin_select, gpio_interrupt_type_t trigger)
{
    const port_pin_config_t port_config = {
            kPORT_PullUp,
            kPORT_FastSlewRate,
            kPORT_PassiveFilterEnable,
            kPORT_LowDriveStrength,
            kPORT_MuxAsGpio
    };

    CLOCK_EnableClock(kCLOCK_PortC);
    CLOCK_EnableClock(kCLOCK_PortD);
    EnableIRQ(PORTC_PORTD_IRQn);

#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
    switch(pin_select)
    {
		case interrupt_PTC0:
		     PORT_SetPinConfig(PORTC, PIN0_IDX, &port_config);
		     PORT_SetPinInterruptConfig(PORTC, PIN0_IDX, trigger);
		break;

		case interrupt_PTC1:
		     PORT_SetPinConfig(PORTC, PIN1_IDX, &port_config);
		     PORT_SetPinInterruptConfig(PORTC, PIN1_IDX, trigger);
		break;

		case interrupt_PTC2:
		     PORT_SetPinConfig(PORTC, PIN2_IDX, &port_config);
		     PORT_SetPinInterruptConfig(PORTC, PIN2_IDX, trigger);
		break;

        case interrupt_PTD1:
             PORT_SetPinConfig(PORTD, PIN1_IDX, &port_config);
             PORT_SetPinInterruptConfig(PORTD, PIN1_IDX, trigger);
        break;

		case interrupt_PTD5:
		     PORT_SetPinConfig(PORTD, PIN5_IDX, &port_config);
		     PORT_SetPinInterruptConfig(PORTD, PIN5_IDX, trigger);
		break;

		case interrupt_PTD6:
		     PORT_SetPinConfig(PORTD, PIN6_IDX, &port_config);
		     PORT_SetPinInterruptConfig(PORTD, PIN6_IDX, trigger);
		break;

		default:
			if(pin_select >= 6 && pin_select <= 14)
			{
				return GPIO_pin_does_not_support_interruption;
			}

			return GPIO_pin_not_available;
		break;
    }
#endif

#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
	switch(pin_select)
	{
		case interrupt_PTC3:
		     PORT_SetPinConfig(PORTC, PIN3_IDX, &port_config);
		     PORT_SetPinInterruptConfig(PORTC, PIN3_IDX, trigger);
		break;

		case interrupt_PTC4:
		     PORT_SetPinConfig(PORTC, PIN4_IDX, &port_config);
		     PORT_SetPinInterruptConfig(PORTC, PIN4_IDX, trigger);
		break;

		case interrupt_PTD4:
		     PORT_SetPinConfig(PORTD, PIN4_IDX, &port_config);
		     PORT_SetPinInterruptConfig(PORTD, PIN4_IDX, trigger);
		break;

		case interrupt_PTD5:
		     PORT_SetPinConfig(PORTD, PIN5_IDX, &port_config);
		     PORT_SetPinInterruptConfig(PORTD, PIN5_IDX, trigger);
		break;

		default:
			if(pin_select == 0 || pin_select == 1)
			{
				return GPIO_pin_does_not_support_interruption;
			}
			return GPIO_pin_not_available;
		break;
	}

#endif

	return successful_task;
}

general_codes_t GPIO_InterruptDisable(interrupt_pins_t pin_select)
{
    const port_pin_config_t port_config = {
            kPORT_PullUp,
            kPORT_FastSlewRate,
            kPORT_PassiveFilterEnable,
            kPORT_LowDriveStrength,
            kPORT_MuxAsGpio
    };

#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
    switch(pin_select)
    {
		case interrupt_PTC0:
		     PORT_SetPinConfig(PORTC, PIN0_IDX, &port_config);
		     PORT_SetPinInterruptConfig(PORTC, PIN0_IDX, kPORT_InterruptOrDMADisabled);
		break;

		case interrupt_PTC1:
		     PORT_SetPinConfig(PORTC, PIN1_IDX, &port_config);
		     PORT_SetPinInterruptConfig(PORTC, PIN1_IDX, kPORT_InterruptOrDMADisabled);
		break;

		case interrupt_PTC2:
		     PORT_SetPinConfig(PORTC, PIN2_IDX, &port_config);
		     PORT_SetPinInterruptConfig(PORTC, PIN2_IDX, kPORT_InterruptOrDMADisabled);
		break;

        case interrupt_PTD1:
             PORT_SetPinConfig(PORTD, PIN1_IDX, &port_config);
             PORT_SetPinInterruptConfig(PORTD, PIN1_IDX, kPORT_InterruptOrDMADisabled);
        break;

        case interrupt_PTD5:
		     PORT_SetPinConfig(PORTD, PIN5_IDX, &port_config);
		     PORT_SetPinInterruptConfig(PORTD, PIN5_IDX, kPORT_InterruptOrDMADisabled);
		break;

		case interrupt_PTD6:
		     PORT_SetPinConfig(PORTD, PIN6_IDX, &port_config);
		     PORT_SetPinInterruptConfig(PORTD, PIN6_IDX, kPORT_InterruptOrDMADisabled);
		break;

		default:
			if(pin_select >= 6 && pin_select <= 14)
			{
				return GPIO_pin_does_not_support_interruption;
			}

			return GPIO_pin_not_available;
		break;
    }
#endif

#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
	switch(pin_select)
	{
		case interrupt_PTC3:
		     PORT_SetPinConfig(PORTC, PIN3_IDX, &port_config);
		     PORT_SetPinInterruptConfig(PORTC, PIN3_IDX, kPORT_InterruptOrDMADisabled);
		break;

		case interrupt_PTC4:
		     PORT_SetPinConfig(PORTC, PIN4_IDX, &port_config);
		     PORT_SetPinInterruptConfig(PORTC, PIN4_IDX, kPORT_InterruptOrDMADisabled);
		break;

		case interrupt_PTD4:
		     PORT_SetPinConfig(PORTD, PIN4_IDX, &port_config);
		     PORT_SetPinInterruptConfig(PORTD, PIN4_IDX, kPORT_InterruptOrDMADisabled);
		break;

		case interrupt_PTD5:
		     PORT_SetPinConfig(PORTD, PIN5_IDX, &port_config);
		     PORT_SetPinInterruptConfig(PORTD, PIN5_IDX, kPORT_InterruptOrDMADisabled);
		break;

		default:
			if(pin_select == 0 || pin_select == 1)
			{
				return GPIO_pin_does_not_support_interruption;
			}
			return GPIO_pin_not_available;
		break;
	}

#endif

	return successful_task;
}

general_codes_t GPIO_GetInterruptionFlag(interrupt_pins_t pin_select, interrupt_status_t* interrupt_flag)
{

#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
	switch(pin_select)
	{
		case interrupt_PTC3:
			(*interrupt_flag) = PTC3_intr_status;
		break;

		case interrupt_PTC4:
			(*interrupt_flag) = PTC4_intr_status;
		break;

		case interrupt_PTD4:
			(*interrupt_flag) = PTD4_intr_status;
		break;

		case interrupt_PTD5:
			(*interrupt_flag) = PTD5_intr_status;
		break;

		default:
			if(pin_select == 0 || pin_select == 1)
			{
				return GPIO_pin_does_not_support_interruption;
			}
			return GPIO_pin_not_available;
		break;
	}

#endif

#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
	switch(pin_select)
	{
		case interrupt_PTC0:
			(*interrupt_flag) = PTC0_intr_status;
		break;

		case interrupt_PTC1:
			(*interrupt_flag) = PTC1_intr_status;
		break;

		case interrupt_PTC2:
			(*interrupt_flag) = PTC2_intr_status;
		break;

        case interrupt_PTD1:
            (*interrupt_flag) = PTD1_intr_status;
        break;

		case interrupt_PTD5:
			(*interrupt_flag) = PTD5_intr_status;
		break;

		case interrupt_PTD6:
			(*interrupt_flag) = PTD6_intr_status;
		break;

		default:
			if(pin_select >= 6 && pin_select <= 14)
			{
				return GPIO_pin_does_not_support_interruption;
			}

			return GPIO_pin_not_available;
		break;
	}
#endif

	return successful_task;
}

general_codes_t GPIO_ClearInterruptionFlag(interrupt_pins_t pin_select)
{
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
	switch(pin_select)
	{
		case interrupt_PTC3:
			PTC3_intr_status = NOT_INTERRUPTED;
		break;

		case interrupt_PTC4:
			PTC4_intr_status = NOT_INTERRUPTED;
		break;

		case interrupt_PTD4:
			PTD4_intr_status = NOT_INTERRUPTED;
		break;

		case interrupt_PTD5:
			PTD5_intr_status = NOT_INTERRUPTED;
		break;

		default:
			if(pin_select == 0 || pin_select == 1)
			{
				return GPIO_pin_does_not_support_interruption;
			}
			return GPIO_pin_not_available;
		break;
	}

#endif

#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
	switch(pin_select)
	{
		case interrupt_PTC0:
			PTC0_intr_status = NOT_INTERRUPTED;
		break;

		case interrupt_PTC1:
			PTC1_intr_status = NOT_INTERRUPTED;
		break;

		case interrupt_PTC2:
			PTC2_intr_status = NOT_INTERRUPTED;
		break;

        case interrupt_PTD1:
            PTD1_intr_status = NOT_INTERRUPTED;
        break;

		case interrupt_PTD5:
			PTD5_intr_status = NOT_INTERRUPTED;
		break;

		case interrupt_PTD6:
			PTD6_intr_status = NOT_INTERRUPTED;
		break;

		default:
			if(pin_select >= 6 && pin_select <= 14)
			{
				return GPIO_pin_does_not_support_interruption;
			}

			return GPIO_pin_not_available;
		break;
	}
#endif

	return successful_task;
}
/////////////////////////////////////////////////////////////GPIO/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////ADC variables///////////////////////
static volatile uint32_t g_adc_timeout = 30000;

#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
uint8_t ADC_headers_is_init = NOT_INIT;

extern uint8_t BatteryTemperature_isInit;

static ADC_init_t ADC_config = {0};
#endif

///////////////ADC variables///////////////////////
//Function to enable pins of ADC
uint32_t Init_Pins_ADC(PORT_Type *port_base, uint32_t pin_idx) {

	if(Pins_ADC(port_base,pin_idx))
	{
		Enable_Clock_port(port_base);
		PORT_SetPinMux(port_base, pin_idx, kPORT_PinDisabledOrAnalog);
		return successful_task;
	}

	else
	{
		return initialization_failed;
	}

	return initialization_failed;
}

//Function to validate pins of ADC
uint32_t Pins_ADC(PORT_Type *port_base, uint32_t pin_idx){

	if(port_base == PORTC)
	{

		switch(pin_idx){

			case 0:
				return true;
			break;

			case 1:
				return true;
			break;

			case 2:
				return true;
			break;

			default:
				return false;
			break;
		}
	}

	else if(port_base == PORTD)
	{

		switch(pin_idx){
			case 1:
				return true;
			break;

			case 5:
				return true;
			break;

			case 6:
				return true;
			break;

			default:
				return false;
			break;
		}
	}

	else if(port_base==PORTE)
	{

		switch(pin_idx){
			case 20:
				return true;
			break;

			case 21:
				return true;
			break;

			case 22:
				return true;
			break;

			case 23:
				return true;
			break;

            case 29:
                return true;
            break;

            case 30:
                return true;
            break;

			default:
				return false;
			break;
		}
	}

	else
	{
		return false;
	}
}

void ADC_Set_Timeout(uint32_t timeout){

    g_adc_timeout = timeout;
}

uint32_t ADC_Get_Timeout(void){

    return g_adc_timeout;
}

#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
// Initialization and configuration of the ADC_16bit
general_codes_t ADC_Initialization(ADC_init_t init_adc_struct, configuration_select_t get_default_config){

    pit_config_t pitConfig;
    adc16_config_t adc_config;

    // Default parameters used in the ADC configuration
    /*
     * init_adc_struct.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
     * init_adc_struct.clockSource = kADC16_ClockSourceAsynchronousClock;
     * init_adc_struct.enableAsynchronousClock = true;
     * init_adc_struct.clockDivider = kADC16_ClockDivider8;
     * init_adc_struct.resolution = kADC16_ResolutionSE12Bit;
     * init_adc_struct.longSampleMode = kADC16_LongSampleDisabled;
     * init_adc_struct.enableHighSpeed = false;
     * init_adc_struct.enableLowPower = false;
     * init_adc_struct.enableContinuousConversion = false;
     */

	if (NOT_INIT == ADC_headers_is_init)
	{

		if(IS_INIT == BatteryTemperature_isInit)
		{
			ADC16_Deinit(ADC0);
		}

		ADC_config = init_adc_struct;

		/* pitConfig.enableRunInDebug = false; */
		PIT_GetDefaultConfig(&pitConfig);
		/* Init pit module */
		PIT_Init(PIT, &pitConfig);
		/* Enable timer interrupts for channel 1 */
		PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
		/* Enable at the NVIC */
		EnableIRQ(PIT_IRQn);


		Enable_Clock_port(PORTD);
		PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_PinDisabledOrAnalog);


	    ADC16_GetDefaultConfig(&adc_config);
	    if(!get_default_config)
	    {
	    	adc_config.resolution = init_adc_struct.resolution;
	    	adc_config.enableLowPower = init_adc_struct.enableLowPower;
	    	adc_config.enableContinuousConversion = init_adc_struct.enableContinuousConversion;
	    }

		ADC16_Init(ADC16_BASE, &adc_config);

		ADC16_EnableHardwareTrigger(ADC16_BASE, false); /* Make sure the software trigger is used. */

		ADC_headers_is_init = IS_INIT;
		BatteryTemperature_isInit = NOT_INIT;


		#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
		   if (kStatus_Success == ADC16_DoAutoCalibration(ADC16_BASE))
		   {
			   return successful_task;
		   }
		#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */
	}

    return initialization_failed;
}

// Function to change the slot to read
general_codes_t ADC_Read( uint16_t *adc_value_read)
{
    adc16_channel_config_t adc_channel_struct;

    adc_channel_struct.enableInterruptOnConversionCompleted = false;

    #if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
        adc_channel_struct.enableDifferentialConversion = false;
    #endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */


    adc_channel_struct.channelNumber = 6U;
    ADC16_SetChannelMuxMode(ADC16_BASE,kADC16_ChannelMuxB);
    ADC16_SetChannelConfig(ADC16_BASE, ADC16_CHANNEL_GROUP, &adc_channel_struct);

    pit_channel_1_flag=false;

	/* Set timer period for channel 1 */
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, USEC_TO_COUNT(g_adc_timeout, CLOCK_GetFreq(kCLOCK_BusClk)));
	/* Start channel 1 */
	PIT_StartTimer(PIT, kPIT_Chnl_1);

    while ( !(kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(ADC16_BASE, ADC16_CHANNEL_GROUP)) && !(pit_channel_1_flag)){
    }


	pit_channel_1_flag = false;
	PIT_StopTimer(PIT, kPIT_Chnl_1);

    if( !(pit_channel_1_flag)){

        *adc_value_read= ADC16_GetChannelConversionValue(ADC16_BASE, ADC16_CHANNEL_GROUP);
        pit_channel_1_flag=false;
        return successful_task;
    }

    pit_channel_1_flag=false;

    return timeout_reached;
}
#endif

#if(defined(ATHEN_BOARD) && (ATHEN_BOARD))
general_codes_t ADC_Initialization(adc_alternatives_t alternative, ADC_init_t init_adc_struct,
							configuration_select_t get_default_config)
{

    pit_config_t pitConfig;
    adc16_config_t adc_config;

	PORT_Type *port_base;
	uint32_t pin_idx;


	/* pitConfig.enableRunInDebug = false; */
	PIT_GetDefaultConfig(&pitConfig);
	/* Init pit module */
	PIT_Init(PIT, &pitConfig);
	/* Enable timer interrupts for channel 1 */
	PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
	/* Enable at the NVIC */
	EnableIRQ(PIT_IRQn);

	switch(alternative)
	{
		case ADC_Alt0: /** PTC0*/
			port_base = PORTC;
			pin_idx = PIN0_IDX;
		break;

		case ADC_Alt1: /** PTC1*/
			port_base = PORTC;
			pin_idx = PIN1_IDX;
		break;

		case ADC_Alt2: /** PTC2*/
			port_base = PORTC;
			pin_idx = PIN2_IDX;
		break;

		case ADC_Alt3: /** PTD1*/
			port_base = PORTD;
			pin_idx = PIN1_IDX;
		break;

		case ADC_Alt4: /** PTD5*/
			port_base = PORTD;
			pin_idx = PIN5_IDX;
		break;

		case ADC_Alt5: /** PTD6*/
			port_base = PORTD;
			pin_idx = PIN6_IDX;
		break;

		case ADC_Alt6: /** PTE20*/
			port_base = PORTE;
			pin_idx = PIN20_IDX;
		break;

		case ADC_Alt7: /**PTE21*/
			port_base = PORTE;
			pin_idx = PIN21_IDX;
		break;

		case ADC_Alt8: /** PTE22*/
			port_base = PORTE;
			pin_idx = PIN22_IDX;
		break;

		case ADC_Alt9: /** PTE23*/
			port_base = PORTE;
			pin_idx = PIN23_IDX;
		break;

        case ADC_Alt10: /** PTE29*/
            port_base = PORTE;
            pin_idx = PIN29_IDX;
        break;

        case ADC_Alt11: /** PTE30*/
            port_base = PORTE;
            pin_idx = PIN30_IDX;
        break;

		default:
			return ADC_pin_not_available;
		break;
	}

//	Enable_Clock_port(PORTD);
//	PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_PinDisabledOrAnalog);

	Init_Pins_ADC(port_base, pin_idx);


	ADC16_GetDefaultConfig(&adc_config);

	if(!get_default_config)
	{
		adc_config.resolution = init_adc_struct.resolution;
		adc_config.enableLowPower = init_adc_struct.enableLowPower;
		adc_config.enableContinuousConversion = init_adc_struct.enableContinuousConversion;
	}

	ADC16_Init(ADC16_BASE, &adc_config);


	ADC16_EnableHardwareTrigger(ADC16_BASE, false); /* Make sure the software trigger is used. */

    if (kStatus_Success == ADC16_DoAutoCalibration(ADC16_BASE))
    {
 	    return successful_task;
    }

    return initialization_failed;
}

void ADC_Deinitialization(void)
{
	ADC16_Deinit(ADC16_BASE);
}

general_codes_t ADC_Read(adc_alternatives_t alternative, uint16_t *adc_value_read)
{

    adc16_channel_config_t adc_channel_struct;
    adc16_channel_mux_mode_t mux_mode;

	switch(alternative)
	{
		case ADC_Alt0: /** PTC0*/
		    adc_channel_struct.channelNumber = 14U;
		    mux_mode = kADC16_ChannelMuxA;
		break;

		case ADC_Alt1: /** PTC1*/
		    adc_channel_struct.channelNumber = 15U;
		    mux_mode = kADC16_ChannelMuxA;
		break;

		case ADC_Alt2: /** PTC2*/
		    adc_channel_struct.channelNumber = 11U;
		    mux_mode = kADC16_ChannelMuxA;
		break;

		case ADC_Alt3: /** PTD1*/
		    adc_channel_struct.channelNumber = 5U;
		    mux_mode = kADC16_ChannelMuxB;
		break;

		case ADC_Alt4: /** PTD5*/
		    adc_channel_struct.channelNumber = 6U;
		    mux_mode = kADC16_ChannelMuxB;
		break;

		case ADC_Alt5: /** PTD6*/
		    adc_channel_struct.channelNumber = 7U;
		    mux_mode = kADC16_ChannelMuxB;
		break;

		case ADC_Alt6: /** PTE20*/
		    adc_channel_struct.channelNumber = 0U;
		    mux_mode = kADC16_ChannelMuxA;
		break;

		case ADC_Alt7: /**PTE21*/
		    adc_channel_struct.channelNumber = 4U;
		    mux_mode = kADC16_ChannelMuxA;
		break;

		case ADC_Alt8: /** PTE22*/
		    adc_channel_struct.channelNumber = 3U;
		    mux_mode = kADC16_ChannelMuxA;
		break;

		case ADC_Alt9: /** PTE23*/
		    adc_channel_struct.channelNumber = 7U;
		    mux_mode = kADC16_ChannelMuxA;
		break;

        case ADC_Alt10: /** PTE29*/
            adc_channel_struct.channelNumber = 4U;
            mux_mode = kADC16_ChannelMuxB;
        break;

        case ADC_Alt11: /** PTE30*/
            adc_channel_struct.channelNumber = 23U;
            mux_mode = kADC16_ChannelMuxA;
        break;

		default:
			return ADC_pin_not_available;
		break;
	}


    adc_channel_struct.enableInterruptOnConversionCompleted = false;
	adc_channel_struct.enableDifferentialConversion = false;

    ADC16_SetChannelMuxMode(ADC16_BASE,mux_mode);
    ADC16_SetChannelConfig(ADC16_BASE, ADC16_CHANNEL_GROUP, &adc_channel_struct);

    pit_channel_1_flag=false;

	/* Set timer period for channel 1 */
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, USEC_TO_COUNT(g_adc_timeout, CLOCK_GetFreq(kCLOCK_BusClk)));
	/* Start channel 1 */
	PIT_StartTimer(PIT, kPIT_Chnl_1);

    while ( !(kADC16_ChannelConversionDoneFlag
    		& ADC16_GetChannelStatusFlags(ADC16_BASE, ADC16_CHANNEL_GROUP))
    		&& !(pit_channel_1_flag)){}


	pit_channel_1_flag = false;
	PIT_StopTimer(PIT, kPIT_Chnl_1);

    if( !(pit_channel_1_flag)){

        *adc_value_read= ADC16_GetChannelConversionValue(ADC16_BASE, ADC16_CHANNEL_GROUP);
        pit_channel_1_flag=false;
        return successful_task;
    }

    pit_channel_1_flag=false;

    return timeout_reached;
}
#endif
/////////////////////////////////////////////////////////////ADC///////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////I2C/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////I2C variables///////////////////////

#define I2C_MASTER_IRQ I2C0_IRQn

volatile status_t g_master_completion_flag = 0xFFFF;

static int g_status=0;

static volatile uint32_t g_i2c_timeout=3000000;

/** NO HACER STATIC!!!!*/
i2c_master_handle_t i2c_handle;

#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
uint8_t I2C_headers_isInit = NOT_INIT;

extern uint8_t FuelGauge_isInit;

static I2C_init_t I2C_config = {0};

static i2c_alternatives_t I2C_Alt = I2C_Alt0;

static port_pin_config_t config_port_I2C = {
	kPORT_PullDisable,
	kPORT_FastSlewRate,
	kPORT_PassiveFilterDisable,
	kPORT_LowDriveStrength,
	kPORT_MuxAlt2
};
#endif

#if (defined(ATHEN_BOARD) && (ATHEN_BOARD))
extern uint8_t I2C0_Fuel_init;
#endif

//////////////I2C variables///////////////////////

void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData);

//Function to enable pins of I2C
uint32_t Init_Pin_I2C(PORT_Type *port_base, uint32_t pin_idx) {

	if(Pins_I2C(port_base,pin_idx)){
		Enable_Clock_port(port_base);
		/////////Config the I2C port SCL//////////
		PORT_SetPinMux(port_base, pin_idx, Pins_I2C(port_base,pin_idx));                  /*  configured as I2C SCL_SDA */

		return successful_task;
	}
	else{
		return initialization_failed;
	}
}

//Function to validate pins of I2C
uint8_t Pins_I2C(PORT_Type *port_base, uint32_t pin_idx){

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))
	if(port_base==PORTB ){
		switch(pin_idx){
			case 0:
				return kPORT_MuxAlt2;
			break;
			case 1:
				return kPORT_MuxAlt2;
			default:
				return kPORT_PinDisabledOrAnalog;
			break;
		}
	}
	else if(port_base==PORTE){
		switch(pin_idx){
            case 0:
                return kPORT_MuxAlt6;
            break;
            case 1:
                return kPORT_MuxAlt6;
            break;
            case 18:
                return kPORT_MuxAlt4;
            break;
            case 19:
                return kPORT_MuxAlt4;
            break;
            default:
                return kPORT_PinDisabledOrAnalog;
            break;
		}
	}
	else{
		return kPORT_PinDisabledOrAnalog;
	}
#endif

#if (defined(ATHEN_BOARD) && (ATHEN_BOARD))
	if(PORTC == port_base)
	{
		switch(pin_idx)
		{
			case 1:
				return kPORT_MuxAlt2;
			break;

			case 2:
				return kPORT_MuxAlt2;
			break;

			default:
				return kPORT_PinDisabledOrAnalog;
			break;
		}
		return kPORT_MuxAlt2;
	}

	else if(port_base)
	{
		switch(pin_idx)
		{
			case 24:
				return kPORT_MuxAlt5;
			break;

			case 25:
				return kPORT_MuxAlt5;
			break;

			default:
				return kPORT_PinDisabledOrAnalog;
			break;
		}
	}

	return kPORT_PinDisabledOrAnalog;
#endif
}


void I2C_Set_Timeout(uint32_t timeout){

    g_i2c_timeout = timeout;
}

uint32_t I2C_Get_Timeout(void){

    return g_i2c_timeout;
}


void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
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

// Initialization of I2C
general_codes_t I2C_Initialization(i2c_alternatives_t i2c_alternatives, I2C_init_t i2c_master_config_struct, configuration_select_t get_default_config )
{

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))
	i2c_master_config_t i2c_config;

	if (NOT_INIT == I2C_headers_isInit)
	{

		I2C_Type *i2c_base;
		pit_config_t pitConfig;

		if(IS_INIT == FuelGauge_isInit)
		{
			PORT_SetPinMux(PORTE, PIN18_IDX, kPORT_PinDisabledOrAnalog);
			PORT_SetPinMux(PORTE, PIN19_IDX, kPORT_PinDisabledOrAnalog);
		}

		/* pitConfig.enableRunInDebug = false; */
		PIT_GetDefaultConfig(&pitConfig);
		/* Init pit module */
		PIT_Init(PIT, &pitConfig);
		/* Enable timer interrupts for channel 1 */
		PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
		/* Enable at the NVIC */
		EnableIRQ(PIT_IRQn);


		if(I2C_Alt0 == i2c_alternatives)
		{

			Init_Pin_I2C(PORTB,PIN0_IDX);
			Init_Pin_I2C(PORTB,PIN1_IDX);
			i2c_base=I2C0;
			I2C_headers_isInit = IS_INIT;
		}
		else if(I2C_Alt1 == i2c_alternatives)
		{

			Init_Pin_I2C(PORTE,PIN0_IDX);
			Init_Pin_I2C(PORTE,PIN1_IDX);
			i2c_base=I2C1;
		}
		else{

			return parameter_error;
		}

		// Default parameters used in the I2C configuration
		/*
		 * i2c_master_config_struct.baudRate_Bps = 100000U;
		 * i2c_master_config_struct.enableHighDrive = false;
		 * i2c_master_config_struct.enableStopHold = false;
		 * i2c_master_config_struct.glitchFilterWidth = 0U;
		 * i2c_master_config_struct.enableMaster = true;
		 */
		I2C_MasterGetDefaultConfig(&i2c_config);
		if(!get_default_config)
		{
			i2c_config.baudRate_Bps = i2c_master_config_struct.baudRate_Bps;
		}

		// Configuration of I2C parameters
		I2C_MasterInit(i2c_base, &i2c_config, CLOCK_GetFreq(I2C0_CLK_SRC));

		memset(&i2c_handle, 0, sizeof(i2c_handle));

		I2C_MasterTransferCreateHandle(i2c_base, &i2c_handle, i2c_master_callback, NULL);

		FuelGauge_isInit = NOT_INIT;

		I2C_config.baudRate_Bps = i2c_config.baudRate_Bps;
		I2C_Alt = i2c_alternatives;
	}

    return successful_task;
#endif

#if (defined(ATHEN_BOARD) && ATHEN_BOARD)
	i2c_master_config_t i2c_config;
	I2C_Type *i2c_base;
	pit_config_t pitConfig;

	/* pitConfig.enableRunInDebug = false; */
	PIT_GetDefaultConfig(&pitConfig);
	/* Init pit module */
	PIT_Init(PIT, &pitConfig);
	/* Enable timer interrupts for channel 1 */
	PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
	/* Enable at the NVIC */
	EnableIRQ(PIT_IRQn);


	if(I2C_Alt0 == i2c_alternatives)
	{

		Init_Pin_I2C(PORTE,PIN24_IDX);
		Init_Pin_I2C(PORTE,PIN25_IDX);
		i2c_base = I2C0;
		I2C0_Fuel_init = IS_INIT;
	}
	else if(I2C_Alt1 == i2c_alternatives)
	{

		Init_Pin_I2C(PORTC,PIN1_IDX);
		Init_Pin_I2C(PORTC,PIN2_IDX);
		i2c_base=I2C1;
	}
	else
	{

		return parameter_error;
	}

	// Default parameters used in the I2C configuration
	/*
	 * i2c_master_config_struct.baudRate_Bps = 100000U;
	 * i2c_master_config_struct.enableHighDrive = false;
	 * i2c_master_config_struct.enableStopHold = false;
	 * i2c_master_config_struct.glitchFilterWidth = 0U;
	 * i2c_master_config_struct.enableMaster = true;
	 */
	I2C_MasterGetDefaultConfig(&i2c_config);

	if(!get_default_config)
	{
		i2c_config.baudRate_Bps = i2c_master_config_struct.baudRate_Bps;
	}

	// Configuration of I2C parameters
	I2C_MasterInit(i2c_base, &i2c_config, CLOCK_GetFreq(I2C0_CLK_SRC));

	memset(&i2c_handle, 0, sizeof(i2c_handle));

	I2C_MasterTransferCreateHandle(i2c_base, &i2c_handle, i2c_master_callback, NULL);

    return successful_task;
#endif
}

general_codes_t I2C_Deinitialization(i2c_alternatives_t i2c_alternatives)
{
	if(I2C_Alt0 == i2c_alternatives)
	{
		I2C_MasterDeinit(I2C0);
	}
	else if(I2C_Alt1 == i2c_alternatives)
	{
		I2C_MasterDeinit(I2C1);
	}
	else
	{
		return parameter_error;
	}
    I2C_headers_isInit = NOT_INIT;

	return successful_task;
}

general_codes_t I2C_Write(i2c_alternatives_t i2c_alternatives, uint8_t slave_address, uint8_t *i2c_master_tx_buff, uint8_t i2c_data_length)
{

	status_t status_temp = successful_task;
    i2c_master_transfer_t i2c_master_transfer_struct = {0};
    I2C_Type *i2c_base;



    if(I2C_Alt0 == i2c_alternatives)
    {
        i2c_base=I2C0;
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
		PORT_SetPinMux(PORTE, PIN18_IDX, kPORT_PinDisabledOrAnalog);
		PORT_SetPinMux(PORTE, PIN19_IDX, kPORT_PinDisabledOrAnalog);
		PORT_SetPinConfig(PORTB, PIN0_IDX, &config_port_I2C);
		PORT_SetPinConfig(PORTB, PIN1_IDX, &config_port_I2C);
		memset(&i2c_handle, 0, sizeof(i2c_handle));
		I2C_MasterTransferCreateHandle(i2c_base, &i2c_handle, i2c_master_callback, NULL);
#endif
    }
    else if(I2C_Alt1 == i2c_alternatives){

        i2c_base=I2C1;
    }
    else{

        return parameter_error;
    }

	// Configuration of I2C transmission parameters
    i2c_master_transfer_struct.slaveAddress = slave_address;
    i2c_master_transfer_struct.direction = kI2C_Write;
    i2c_master_transfer_struct.data = i2c_master_tx_buff;
    i2c_master_transfer_struct.dataSize = i2c_data_length;
    i2c_master_transfer_struct.subaddress = 0;
    i2c_master_transfer_struct.subaddressSize = 0;
    i2c_master_transfer_struct.flags = kI2C_TransferDefaultFlag;

    g_status=I2C_MasterTransferNonBlocking(i2c_base, &i2c_handle, &i2c_master_transfer_struct);

	g_master_completion_flag =0xFFFF;

    pit_channel_1_flag = false;

	/* Set timer period for channel 1 */
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, USEC_TO_COUNT(g_i2c_timeout, CLOCK_GetFreq(kCLOCK_BusClk)));
	/* Start channel 1 */
	PIT_StartTimer(PIT, kPIT_Chnl_1);

	// Wait the PIT interruption
	//  Wait for transfer completed.
	while (g_master_completion_flag ==0xFFFF && !(pit_channel_1_flag) ){
	}

    // Stop PIT count
	PIT_StopTimer(PIT, kPIT_Chnl_1);

	if(pit_channel_1_flag){

		pit_channel_1_flag=false;
	    status_temp=timeout_reached;
	}
	else{

		pit_channel_1_flag=false;
		status_temp=g_master_completion_flag;
	}

    g_master_completion_flag =0xFFFF;
    return status_temp;
}

general_codes_t I2C_Read(i2c_alternatives_t i2c_alternatives, uint8_t slave_address, uint8_t *i2c_master_rx_buff, uint8_t i2c_data_length){

	status_t status_temp = successful_task;
    i2c_master_transfer_t i2c_master_transfer_struct = {0};
    I2C_Type *i2c_base;

    if(I2C_Alt0 == i2c_alternatives)
    {
        i2c_base=I2C0;
#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))
		PORT_SetPinMux(PORTE, PIN18_IDX, kPORT_PinDisabledOrAnalog);
		PORT_SetPinMux(PORTE, PIN19_IDX, kPORT_PinDisabledOrAnalog);
		PORT_SetPinConfig(PORTB, PIN0_IDX, &config_port_I2C);
		PORT_SetPinConfig(PORTB, PIN1_IDX, &config_port_I2C);
		memset(&i2c_handle, 0, sizeof(i2c_handle));
		I2C_MasterTransferCreateHandle(i2c_base, &i2c_handle, i2c_master_callback, NULL);
#endif
    }
    else if(I2C_Alt1 == i2c_alternatives){

        i2c_base=I2C1;
    }
    else{

        return parameter_error;
    }

    // Configuration of I2C reception parameters
    i2c_master_transfer_struct.slaveAddress = slave_address;
    i2c_master_transfer_struct.direction = kI2C_Read;
    i2c_master_transfer_struct.data = i2c_master_rx_buff;
    i2c_master_transfer_struct.dataSize = i2c_data_length;
    i2c_master_transfer_struct.subaddress = 0;
    i2c_master_transfer_struct.subaddressSize = 0;
    i2c_master_transfer_struct.flags = kI2C_TransferDefaultFlag;

    g_status=I2C_MasterTransferNonBlocking(i2c_base, &i2c_handle, &i2c_master_transfer_struct);

    g_master_completion_flag =0xFFFF;

    pit_channel_1_flag=false;
	/* Set timer period for channel 1 */
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, USEC_TO_COUNT(g_i2c_timeout, CLOCK_GetFreq(kCLOCK_BusClk)));
	/* Start channel 1 */
	PIT_StartTimer(PIT, kPIT_Chnl_1);

    // Wait the PIT interruption
    //  Wait for transfer completed
    while (g_master_completion_flag ==0xFFFF && !(pit_channel_1_flag) ){
    }

    // Stop PIT count
	PIT_StopTimer(PIT, kPIT_Chnl_1);

    if(pit_channel_1_flag){

    	pit_channel_1_flag=false;
        status_temp=timeout_reached;
    }
    else{

    	pit_channel_1_flag=false;
        status_temp=g_master_completion_flag;
    }

    g_master_completion_flag =0xFFFF;
    return status_temp;
}
/////////////////////////////////////////////////////////////I2C OP/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////Low power/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define LLWU_LPTMR_IDX 0U      /* LLWU_M0IF */

volatile uint16_t count_time_to_sleep=0;
volatile bool flag_sw_maintenance =false;
volatile bool llwu_timer_is_ready=false;
static uint8_t LPU_state = NOT_INIT;

static void Set_ClockRun_FromVlpr(void);
static void Set_Clock_Vlpr(void);
static uint8_t Interrupt_LLWU_port(PORT_Type *base, uint32_t pin_idx);
static uint8_t Pins_IDX_llwu(PORT_Type *port_base, uint32_t pin_idx);
static void Interrupt_LLWU_lptmr0( lptmr_config_t lptmrConfig, configuration_select_t get_default_config);

void LLWU_IRQHandler(void)
{
    /* If wakeup by LPTMR. */
    if (LLWU_GetInternalWakeupModuleFlag(LLWU, LLWU_LPTMR_IDX))
    {
        LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(LPTMR0);

    }

    #if(defined(ATHEN_BOARD) && (ATHEN_BOARD))

        /* If wakeup by external pin. */
        if(LLWU_GetExternalWakeupPinFlag(LLWU, PIN6_IDX)){

            PORT_ClearPinsInterruptFlags(PORTC, (1U << PIN1_IDX));
            LLWU_ClearExternalWakeupPinFlag(LLWU, PIN6_IDX);
        }

        if(LLWU_GetExternalWakeupPinFlag(LLWU, PIN7_IDX))
        {
            flag_sw_maintenance=true;
            PORT_ClearPinsInterruptFlags(PORTC, (1U << PIN3_IDX));
            LLWU_ClearExternalWakeupPinFlag(LLWU, PIN7_IDX);
        }

        if(LLWU_GetExternalWakeupPinFlag(LLWU, PIN15_IDX)){

            PORT_ClearPinsInterruptFlags(PORTD, (1U << PIN6_IDX));
            LLWU_ClearExternalWakeupPinFlag(LLWU, PIN15_IDX);
        }
    #endif

    #if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))

        /* If wakeup by external pin. */
        if(LLWU_GetExternalWakeupPinFlag(LLWU, PIN7_IDX))
        {
            PORT_ClearPinsInterruptFlags(PORTC, (1U << PIN3_IDX));
            LLWU_ClearExternalWakeupPinFlag(LLWU, PIN7_IDX);
        }

        if(LLWU_GetExternalWakeupPinFlag(LLWU, PIN8_IDX)){

            PORT_ClearPinsInterruptFlags(PORTC, (1U << PIN4_IDX));
            LLWU_ClearExternalWakeupPinFlag(LLWU, PIN8_IDX);
        }

        if(LLWU_GetExternalWakeupPinFlag(LLWU, PIN1_IDX)){

            PORT_ClearPinsInterruptFlags(PORTD, (1U << PIN4_IDX));
            LLWU_ClearExternalWakeupPinFlag(LLWU, PIN1_IDX);
        }

    #endif

}

void LPTMR0_IRQHandler(void)
{
    if (kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(LPTMR0))
    {
        LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(LPTMR0);
    }
}

void PORTCD_IRQHandler(void)
{

    #if(defined(ATHEN_BOARD) && (ATHEN_BOARD))
        if ((1U << PIN1_IDX) & PORT_GetPinsInterruptFlags(PORTC))
        {
            /* Disable interrupt. */
            PORT_ClearPinsInterruptFlags(PORTC, (1U << PIN1_IDX));
            PORT_SetPinInterruptConfig(PORTC, PIN1_IDX, kPORT_InterruptOrDMADisabled);
        }

        if ((1U << PIN3_IDX) & PORT_GetPinsInterruptFlags(PORTC))
        {
            /* Disable interrupt. */
            PORT_ClearPinsInterruptFlags(PORTC, (1U << PIN3_IDX));
            PORT_SetPinInterruptConfig(PORTC, PIN3_IDX, kPORT_InterruptOrDMADisabled);
        }

        if ((1U << PIN6_IDX) & PORT_GetPinsInterruptFlags(PORTD))
        {
            /* Disable interrupt. */
            PORT_ClearPinsInterruptFlags(PORTD, (1U << PIN6_IDX));
            PORT_SetPinInterruptConfig(PORTD, PIN6_IDX, kPORT_InterruptOrDMADisabled);
        }
    #endif


    #if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
        if ((1U << PIN3_IDX) & PORT_GetPinsInterruptFlags(PORTC))
        {
            /* Disable interrupt. */
            PORT_ClearPinsInterruptFlags(PORTC, (1U << PIN3_IDX));
            PORT_SetPinInterruptConfig(PORTC, PIN3_IDX, kPORT_InterruptOrDMADisabled);
        }

        if ((1U << PIN4_IDX) & PORT_GetPinsInterruptFlags(PORTC))
        {
            /* Disable interrupt. */
            PORT_ClearPinsInterruptFlags(PORTC, (1U << PIN4_IDX));
            PORT_SetPinInterruptConfig(PORTC, PIN4_IDX, kPORT_InterruptOrDMADisabled);
        }

        if ((1U << PIN4_IDX) & PORT_GetPinsInterruptFlags(PORTD))
        {
            /* Disable interrupt. */
            PORT_ClearPinsInterruptFlags(PORTD, (1U << PIN4_IDX));
            PORT_SetPinInterruptConfig(PORTD, PIN4_IDX, kPORT_InterruptOrDMADisabled);
        }
    #endif

}

static void Set_ClockRun_FromVlpr(void)
{
    const mcglite_config_t mcgliteConfig = {.outSrc = kMCGLITE_ClkSrcHirc,
                                            .irclkEnableMode = 0U,
                                            .ircs = kMCGLITE_Lirc8M,
                                            .fcrdiv = kMCGLITE_LircDivBy1,
                                            .lircDiv2 = kMCGLITE_LircDivBy1,
                                            .hircEnableInNotHircMode = true};

    const sim_clock_config_t simConfig =
    {
        .clkdiv1 = 0x00010000U, /* SIM_CLKDIV1. */
#if (defined(FSL_FEATURE_SIM_OPT_HAS_OSC32K_SELECTION) && FSL_FEATURE_SIM_OPT_HAS_OSC32K_SELECTION)
        .er32kSrc = 0U, /* SIM_SOPT1[OSC32KSEL]. */
#endif
    };

    CLOCK_SetSimSafeDivs();
    CLOCK_SetMcgliteConfig(&mcgliteConfig);
    CLOCK_SetSimConfig(&simConfig);
}

static void Set_Clock_Vlpr(void)
{
    const mcglite_config_t mcgliteConfig = {
        .outSrc = kMCGLITE_ClkSrcLirc,
        .irclkEnableMode = kMCGLITE_IrclkEnable,
        .ircs = kMCGLITE_Lirc2M,
        .fcrdiv = kMCGLITE_LircDivBy1,
        .lircDiv2 = kMCGLITE_LircDivBy1,
        .hircEnableInNotHircMode = false,
    };

    const sim_clock_config_t simConfig =
    {
        .clkdiv1 = 0x00010000U, /* SIM_CLKDIV1. */
#if (defined(FSL_FEATURE_SIM_OPT_HAS_OSC32K_SELECTION) && FSL_FEATURE_SIM_OPT_HAS_OSC32K_SELECTION)
        .er32kSrc = 0U, /* SIM_SOPT1[OSC32KSEL]. */
#endif
    };

    CLOCK_SetSimSafeDivs();
    CLOCK_SetMcgliteConfig(&mcgliteConfig);
    CLOCK_SetSimConfig(&simConfig);
}

static uint8_t before_state=0;

general_codes_t LPU_Select_Power_Mode(power_mode_t select_power_mode_switch, uint8_t time_to_sleep)
{
	general_codes_t retval = successful_task;
	if(LPU_state == IS_INIT)
	{

		smc_power_mode_vlls_config_t vlls_config;
		vlls_config.enablePorDetectInVlls0 = true;

		if(run_mode != select_power_mode_switch)
		{
			/* Power mode change. */
			before_state=SMC_GetPowerModeState(SMC);
			SMC_SetPowerModeRun(SMC);
			while (kSMC_PowerStateRun != SMC_GetPowerModeState(SMC))
			{
			}
		   /* If enter RUN from VLPR, change clock after the power mode change. */
			if (kSMC_PowerStateVlpr == before_state)
			{
				Set_ClockRun_FromVlpr();
			}
		}


		switch(select_power_mode_switch){

			case run_mode:
				/* Power mode change. */
				before_state=SMC_GetPowerModeState(SMC);
				SMC_SetPowerModeRun(SMC);
				while (kSMC_PowerStateRun != SMC_GetPowerModeState(SMC))
				{
				}
			   /* If enter RUN from VLPR, change clock after the power mode change. */
				if (kSMC_PowerStateVlpr == before_state)
				{
					Set_ClockRun_FromVlpr();
				}

				retval = successful_task;
			break;

			case vlpr_mode:
				Set_Clock_Vlpr();
				SMC_SetPowerModeVlpr(SMC);
				while (kSMC_PowerStateVlpr != SMC_GetPowerModeState(SMC))
				{
				}

				retval = successful_task;
			break;

			case wait_mode:
				if(time_to_sleep && llwu_timer_is_ready){
					LPTMR_SetTimerPeriod(LPTMR0, (LPO_CLK_FREQ * time_to_sleep)- 1u);
					LPTMR_StartTimer(LPTMR0);
				}
				SMC_PreEnterWaitModes();
				SMC_SetPowerModeWait(SMC);
				SMC_PostExitWaitModes();

				retval = successful_task;
			break;

			case stop_mode:
				if(time_to_sleep && llwu_timer_is_ready){
					LPTMR_SetTimerPeriod(LPTMR0, (LPO_CLK_FREQ * time_to_sleep)- 1u);
					LPTMR_StartTimer(LPTMR0);
				}
				SMC_PreEnterStopModes();
				SMC_SetPowerModeStop(SMC, kSMC_PartialStop);
				SMC_PostExitStopModes();

				retval = successful_task;
			break;

			case vlpw_mode:
				if(time_to_sleep && llwu_timer_is_ready){
					LPTMR_SetTimerPeriod(LPTMR0, (LPO_CLK_FREQ * time_to_sleep)- 1u);
					LPTMR_StartTimer(LPTMR0);
				}
				SMC_PreEnterWaitModes();
				SMC_SetPowerModeVlpw(SMC);
				SMC_PostExitWaitModes();

				retval = successful_task;
			break;

			case vlps_mode:
				if(time_to_sleep && llwu_timer_is_ready){
					LPTMR_SetTimerPeriod(LPTMR0, (LPO_CLK_FREQ * time_to_sleep)- 1u);
					LPTMR_StartTimer(LPTMR0);
				}
				SMC_PreEnterStopModes();
				SMC_SetPowerModeVlps(SMC);
				SMC_PostExitStopModes();

				retval = successful_task;
			break;

			case lls_mode:
				if(time_to_sleep && llwu_timer_is_ready){
					LPTMR_SetTimerPeriod(LPTMR0, (LPO_CLK_FREQ * time_to_sleep)- 1u);
					LPTMR_StartTimer(LPTMR0);
				}
				SMC_PreEnterStopModes();
				SMC_SetPowerModeLls(SMC);
				SMC_PostExitStopModes();

				retval = successful_task;
			break;

			case vlls3_mode:
				if(time_to_sleep && llwu_timer_is_ready){
					LPTMR_SetTimerPeriod(LPTMR0, (LPO_CLK_FREQ * time_to_sleep)- 1u);
					LPTMR_StartTimer(LPTMR0);
				}
				vlls_config.subMode = kSMC_StopSub3;
				SMC_PreEnterStopModes();
				SMC_SetPowerModeVlls(SMC, &vlls_config);
				SMC_PostExitStopModes();

				retval = successful_task;
			break;

			case vlls1_mode:
				if(time_to_sleep && llwu_timer_is_ready){
					LPTMR_SetTimerPeriod(LPTMR0, (LPO_CLK_FREQ * time_to_sleep)- 1u);
					LPTMR_StartTimer(LPTMR0);
				}
				vlls_config.subMode = kSMC_StopSub1;
				SMC_PreEnterStopModes();
				SMC_SetPowerModeVlls(SMC, &vlls_config);
				SMC_PostExitStopModes();

				retval = successful_task;
			break;

			case vlls0_mode:
				if(time_to_sleep && llwu_timer_is_ready){
					LPTMR_SetTimerPeriod(LPTMR0, (LPO_CLK_FREQ * time_to_sleep)- 1u);
					LPTMR_StartTimer(LPTMR0);
				}
				vlls_config.subMode = kSMC_StopSub0;
				SMC_PreEnterStopModes();
				SMC_SetPowerModeVlls(SMC, &vlls_config);
				SMC_PostExitStopModes();

				retval = successful_task;
			break;

			default:
				retval = parameter_error;
			break;
		}

		if((successful_task == retval) && ((run_mode == select_power_mode_switch) || (vlpr_mode == select_power_mode_switch)))
		{
			if(g_userInit_lpuart.flag)
			{
				LPUART_Initialization(g_userInit_lpuart.LPUART_alternative,
									  g_userInit_lpuart.lpuartConfigStruct,
									  g_userInit_lpuart.default_configuration);
			}
			if(g_userInit_pit.flag)
			{
				PIT_Initialization();
			}
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
			if(g_userInit_uart.flag)
			{
				UART_Deinitialization();
				UART_Initialization(g_userInit_uart.uartConfigStruct,
									g_userInit_uart.default_configuration);
			}
			if(g_userInit_spiMaster.flag)
			{

			}
			if(g_userInit_spiSlave.flag)
			{

			}
#endif
			if(g_userInit_shield_flag)
			{
#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
				UART_SetBaudRate(UART2, g_userInit_shield_baudRate, CLOCK_GetFreq(kCLOCK_BusClk));
#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
//				LPUART_SetBaudRate(LPUART0, g_userInit_shield_baudRate, CLOCK_GetFreq(kCLOCK_CoreSysClk));
#endif
			}
		}
		return retval;
	}else
	{
		return LPU_not_enabled;
	}



}


static uint8_t Interrupt_LLWU_port(PORT_Type *base, uint32_t pin_idx){

	if(Pins_IDX_llwu(base,pin_idx))
	{
		LLWU_SetExternalWakeupPinMode(LLWU, Pins_IDX_llwu(base,pin_idx), kLLWU_ExternalPinFallingEdge);
		NVIC_EnableIRQ(PORTC_PORTD_IRQn);
		return 1;
	}

	return 0;
}

void LPU_Enable_Power_Modes(void)
{
	LPU_state = IS_INIT;

	Delay(500000);
}

static uint8_t Pins_IDX_llwu(PORT_Type *port_base, uint32_t pin_idx){

#if (defined(ATHEN_BOARD) && (ATHEN_BOARD))

	if(port_base==PORTC){
		switch(pin_idx){
			case 1:
				return PIN6_IDX;
			break;
			case 3:
				return PIN7_IDX;
			break;
			default:
				return 0;
			break;
		}
	}
	else if(port_base==PORTD){
		switch(pin_idx){
			case 6:
				return PIN15_IDX;
			break;
			default:
				return 0;
			break;
		}
	}
	else{
		return 0;
	}
#endif

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))


    if(port_base==PORTC){
        switch(pin_idx){
            case 3:
                return PIN7_IDX;
            break;
            case 4:
                return PIN8_IDX;
            break;
            default:
                return 0;
            break;
        }
    }
    else if(port_base==PORTD){
        switch(pin_idx){
            case 4:
                return PIN1_IDX;
            break;
            default:
                return 0;
            break;
        }
    }
    else{
        return 0;
    }

#endif
}

static void Interrupt_LLWU_lptmr0( lptmr_config_t lptmrConfig, configuration_select_t get_default_config){

    /* Setup LPTMR. */
	if(get_default_config){
		LPTMR_GetDefaultConfig(&lptmrConfig);
		lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1; /* Use LPO as clock source. */
		lptmrConfig.bypassPrescaler = true;
	}

    LPTMR_Init(LPTMR0, &lptmrConfig);
    LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    LLWU_EnableInternalModuleInterruptWakup(LLWU, LLWU_LPTMR_IDX, true);
    NVIC_EnableIRQ(LPTMR0_IRQn);
    NVIC_EnableIRQ(LLWU_IRQn);

}

general_codes_t LPU_Init_Power_Mode(llwu_interrupt_t llwu_interrupt, llwu_ports_t llwu_ports){

	if(LPU_state == IS_INIT)
	{
		/* Power related. */
		SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
		if (kRCM_SourceWakeup & RCM_GetPreviousResetSources(RCM)) /* Wakeup from VLLS. */
		{
			PMC_ClearPeriphIOIsolationFlag(PMC);
			NVIC_ClearPendingIRQ(LLWU_IRQn);
		}

		if( llwu_no_interrupt != llwu_interrupt )
		{

			if( llwu_timer == llwu_interrupt || llwu_timer_and_port == llwu_interrupt )
			{
				//Struct of parameter used in the LPTMR configuration
				lptmr_config_t lptmrConfig;
				//initialization of LPTMR
				//Return status    struct parameter      get default configuration
				Interrupt_LLWU_lptmr0(lptmrConfig    ,       true);
				llwu_timer_is_ready=true;
			}

			if( llwu_port != llwu_interrupt || llwu_timer_and_port != llwu_interrupt){
				#if (defined(ATHEN_BOARD) && (ATHEN_BOARD))

					if(llwu_port_C1 == llwu_ports)
					{
						//LLWU pin initialization
						Init_Pins_GPIO_llwu(PORTC,PIN1_IDX);

						//interruption LLWU port initialization
						Interrupt_LLWU_port(PORTC,PIN1_IDX);
					}
					else if(  llwu_port_C3 == llwu_ports )
					{
						//LLWU pin initialization
						Init_Pins_GPIO_llwu(PORTC,PIN3_IDX);

						//interruption LLWU port initialization
						Interrupt_LLWU_port(PORTC,PIN3_IDX);
					}
					else if( llwu_port_D6 == llwu_ports )
					{
						//LLWU pin initialization
						Init_Pins_GPIO_llwu(PORTD,PIN6_IDX);

						//interruption LLWU port initialization
						Interrupt_LLWU_port(PORTD,PIN6_IDX);
					}
					else
					{
						return parameter_error;
					}

					return successful_task;
				#endif

				#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))

					if( llwu_port_C3 == llwu_ports )
					{
						//LLWU pin initialization
						Init_Pins_GPIO_llwu(PORTC,PIN3_IDX);

						//interruption LLWU port initialization
						Interrupt_LLWU_port(PORTC,PIN3_IDX);
					}
					else if( llwu_port_C4 == llwu_ports )
					{
						//LLWU pin initialization
						Init_Pins_GPIO_llwu(PORTC,PIN4_IDX);

						//interruption LLWU port initialization
						Interrupt_LLWU_port(PORTC,PIN4_IDX);
					}
					else if( llwu_port_D4 == llwu_ports )
					{
						//LLWU pin initialization
						Init_Pins_GPIO_llwu(PORTD,PIN4_IDX);

						//interruption LLWU port initialization
						Interrupt_LLWU_port(PORTD,PIN4_IDX);
					}
					else
					{
						return parameter_error;
					}

					return successful_task;

				#endif

			}
		}

		else if(llwu_no_interrupt == llwu_interrupt)
		{
			return successful_task;
		}

		else
		{
			return parameter_error;
		}
	}

	return LPU_not_enabled;
}

//Function to enable pins of GPIO_llwu
uint32_t Init_Pins_GPIO_llwu(PORT_Type *port_base, uint32_t pin_idx){

	if(Pins_GPIO_llwu(port_base,pin_idx)){
		Enable_Clock_port(port_base);
		PORT_SetPinMux(port_base, pin_idx, kPORT_MuxAsGpio);
		port_base->PCR[pin_idx] = ((port_base->PCR[pin_idx] &
		(~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
		  | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
		  | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
		);

		return true;
	}
	else{
		return false;
	}

}

//Function to validate pins of GPIO_llwu
uint8_t Pins_GPIO_llwu(PORT_Type *port_base, uint32_t pin_idx){


	if(port_base==PORTC){
		switch(pin_idx){
			case 1:
				return kPORT_MuxAsGpio;
			break;
			case 3:
				return kPORT_MuxAsGpio;
			break;
			default:
				return 0;
			break;
		}
	}

	else if(port_base==PORTD){
		switch(pin_idx){
			case 4:
				return kPORT_MuxAsGpio;
			break;
			case 6:
				return kPORT_MuxAsGpio;
			break;
			default:
				return 0;
			break;
		}
	}
	else{
		return false;
	}

}

/////////////////////////////////////////////////////////////Low power/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////Enable_extra_functions/////////////////////////////////////////////////////////////////////////////////////////////////////////

static volatile bool g_vdd_sensors_state=false;
static uint8_t PIT_is_init = NOT_INIT;

general_codes_t Delay(uint32_t delay)
{
	if(MIN_DELAY > delay)
	{
		return parameter_error;
	}
	if(true)
	{

		pit_config_t pitConfig;

		/* pitConfig.enableRunInDebug = false; */
		PIT_GetDefaultConfig(&pitConfig);
		/* Init pit module */
		PIT_Init(PIT, &pitConfig);
		/* Enable timer interrupts for channel 1 */
		PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
		/* Enable at the NVIC */
		EnableIRQ(PIT_IRQn);

		PIT_is_init = IS_INIT;

	}

	pit_channel_1_flag=false;
	/* Set timer period for channel 1 */
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, USEC_TO_COUNT(delay, CLOCK_GetFreq(kCLOCK_BusClk)));
	/* Start channel 1 */
	PIT_StartTimer(PIT, kPIT_Chnl_1);

	while(!pit_channel_1_flag){}

	// Stop PIT count
	PIT_StopTimer(PIT, kPIT_Chnl_1);

	pit_channel_1_flag=false;

	return successful_task;
}

void VDD_Sensors_Control(VDD_state_t state){

    gpio_pin_config_t config;

	g_vdd_sensors_state = state;

    CLOCK_EnableClock(kCLOCK_PortC);

    config.pinDirection= kGPIO_DigitalOutput;
    config.outputLogic= g_vdd_sensors_state;

#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
    PORT_SetPinMux(PORTC, PIN5_IDX, kPORT_MuxAsGpio);

    GPIO_PinInit(GPIOC, PIN5_IDX, &config);
#endif

#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
    PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAsGpio);

    GPIO_PinInit(GPIOC, PIN4_IDX, &config);
#endif
}

VDD_state_t VDD_Sensors_Get_State(void){

    return g_vdd_sensors_state;
}

#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))

static volatile bool g_gps_active_antenna_state = GPS_off;

void GPS_Active_Antenna_Control(antenna_state_t state){

    gpio_pin_config_t config;

    CLOCK_EnableClock(kCLOCK_PortE);

    PORT_SetPinMux(PORTE, PIN17_IDX, kPORT_MuxAsGpio);

    if(state)
    {
        g_gps_active_antenna_state = true;

    }
    else
    {
        g_gps_active_antenna_state = false;
    }

    config.pinDirection = kGPIO_DigitalOutput;
    config.outputLogic = g_gps_active_antenna_state;

    GPIO_PinInit(GPIOE, PIN17_IDX, &config);
}

antenna_state_t GPS_Get_State(void){

    return g_gps_active_antenna_state;
}

static volatile bool g_led_state=false;

void LED_Control(led_state_t state)
{

    gpio_pin_config_t config;

    CLOCK_EnableClock(kCLOCK_PortD);

    PORT_SetPinMux(PORTD, PIN6_IDX, kPORT_MuxAsGpio);

    if(state){
        g_led_state = true;

    }
    else{
        g_led_state = false;

    }
    config.pinDirection= kGPIO_DigitalOutput;
    config.outputLogic= g_led_state;

    GPIO_PinInit(GPIOD, PIN6_IDX, &config);
}


led_state_t LED_Get_State(void){

    return g_led_state;
}
#endif


#if(defined(ATHEN_BOARD) && (ATHEN_BOARD))

static volatile bool g_led1_state;
static volatile bool g_led2_state;
static volatile bool g_led_state;

void LED_1_Control(led_state_t state)
{
    gpio_pin_config_t config;

    CLOCK_EnableClock(kCLOCK_PortC);

    PORT_SetPinMux(PORTC, PIN8_IDX, kPORT_MuxAsGpio);

    g_led1_state = state;

    config.pinDirection= kGPIO_DigitalOutput;
    config.outputLogic= g_led1_state;

    GPIO_PinInit(GPIOC, PIN8_IDX, &config);
}

void LED_2_Control(led_state_t state)
{
    gpio_pin_config_t config;

    CLOCK_EnableClock(kCLOCK_PortB);

    PORT_SetPinMux(PORTB, PIN2_IDX, kPORT_MuxAsGpio);

    g_led2_state = state;

    config.pinDirection= kGPIO_DigitalOutput;
    config.outputLogic= g_led2_state;

    GPIO_PinInit(GPIOB, PIN2_IDX, &config);
}

void LED_Control(led_state_t state)
{
    gpio_pin_config_t config;

    CLOCK_EnableClock(kCLOCK_PortB);

    PORT_SetPinMux(PORTB, PIN17_IDX, kPORT_MuxAsGpio);

    g_led_state = state;

    config.pinDirection= kGPIO_DigitalOutput;
    config.outputLogic= g_led_state;

    GPIO_PinInit(GPIOB, PIN17_IDX, &config);
}

led_state_t LED_1_Get_State(void)
{
    return g_led1_state;
}

led_state_t LED_2_Get_State(void)
{
    return g_led2_state;
}

led_state_t LED_Get_State(void)
{
    return g_led_state;
}
#endif

void InitConfigBoard(void)
{
    PORT_Type *port_base;
    GPIO_Type *gpio_base;
    uint8_t pin_idx;
    gpio_pin_config_t config;

    config.pinDirection = kGPIO_DigitalOutput;
    config.outputLogic = false;

	VDD_Sensors_Control(VDD_off);

#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
	GPS_Active_Antenna_Control(GPS_off);
	LED_Control(LED_off);

	CLOCK_EnableClock(kCLOCK_PortE);
	PORT_SetPinMux(PORTE, PIN30_IDX, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOE, PIN30_IDX, &config);

	CLOCK_EnableClock(kCLOCK_PortD);
	PORT_SetPinMux(PORTD, PIN7_IDX, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOD, PIN7_IDX, &config);

    CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */
    port_base= PORTA;
    gpio_base= GPIOA;
    pin_idx= PIN4_IDX;
#endif
#if(defined(ATHEN_BOARD) && (ATHEN_BOARD))
	LED_1_Control(LED_off);
	LED_2_Control(LED_off);
	LED_Control(LED_off);
	CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
	port_base= PORTB;
	gpio_base= GPIOB;
	pin_idx= PIN16_IDX;
#endif


    PORT_SetPinMux(port_base, pin_idx, kPORT_MuxAsGpio);
    GPIO_PinInit(gpio_base,pin_idx,&config);
    GPIO_PinWrite(gpio_base, pin_idx, false);
}

//////////////////////////////////////////////////////Enable_extra_functions/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////RTC///////////////////////////////////////////////////////////////////////////////////////////////

bool rtc_alarm = false;
bool rtc_seconds_flag = false;
uint32_t rtc_seconds = 0;

void RTC_IRQHandler(void)
{
    uint32_t status = RTC_GetStatusFlags(RTC);

    if (status & kRTC_AlarmFlag)
    {
        /* Clear alarm flag */
        RTC_ClearStatusFlags(RTC, kRTC_AlarmInterruptEnable);
        rtc_alarm = true;
    }
    else if (status & kRTC_TimeInvalidFlag)
    {
        /* Clear timer invalid flag */
        RTC_ClearStatusFlags(RTC, kRTC_TimeInvalidFlag);
    }
    else
    {
    }
}

void RTC_Seconds_IRQHandler(void)
{
	rtc_seconds++;
	rtc_seconds_flag = true;
}


void RTC_Initialization()
{
	rtc_config_t rtcConfig;

    RTC_GetDefaultConfig(&rtcConfig);
    RTC_Init(RTC, &rtcConfig);
    RTC_SetClockSource(RTC);
}

void RTC_Start()
{
	RTC_StartTimer(RTC);
}

void RTC_Enable_Interrupt(RTC_interrupt_t interrupt)
{
	if(RTC_Alarm_only == interrupt)
	{
	    RTC_EnableInterrupts(RTC, kRTC_AlarmInterruptEnable);
	    EnableIRQ(RTC_IRQn);
	}else if(RTC_Seconds_only == interrupt)
	{
	    RTC_EnableInterrupts(RTC, kRTC_SecondsInterruptEnable);
	    EnableIRQ(RTC_Seconds_IRQn);
	}else if(RTC_Both_interrupt == interrupt)
	{
	    RTC_EnableInterrupts(RTC, kRTC_AlarmInterruptEnable);
	    RTC_EnableInterrupts(RTC, kRTC_SecondsInterruptEnable);
	    EnableIRQ(RTC_IRQn);
	    EnableIRQ(RTC_Seconds_IRQn);
	}
}

general_codes_t RTC_Set_Time(rtc_datetime_t *datetime)
{
	status_t status = kStatus_Success;
    RTC_StopTimer(RTC);

    status = RTC_SetDatetime(RTC, datetime);

    RTC_StartTimer(RTC);

    if(kStatus_Success == status)
    {
    	return successful_task;
    }else
    {
    	return parameter_error;
    }

}

void RTC_Set_Time_Seconds(uint32_t seconds)
{
    RTC_StopTimer(RTC);

    RTC->TSR = seconds;

    RTC_StartTimer(RTC);
}

general_codes_t RTC_Set_Alarm(rtc_datetime_t *datetime)
{
	status_t status;
	status = RTC_SetAlarm(RTC, datetime);

	if(kStatus_InvalidArgument == status)
	{
		return parameter_error;
	}else if(kStatus_Fail == status)
	{
		return initialization_failed;
	}

    return successful_task;
}

void RTC_Set_Alarm_Seconds(uint32_t seconds)
{
	//uint32_t currSeconds;

//    currSeconds = RTC->TSR;
//    currSeconds += seconds;
    RTC->TAR = seconds;
}

void RTC_Get_Time(rtc_datetime_t *datetime)
{
	RTC_GetDatetime(RTC, datetime);
}

void RTC_Get_Time_Seconds(uint32_t *datetime_seconds)
{
	*datetime_seconds = RTC->TSR;
}

void RTC_Stop()
{
	RTC_StopTimer(RTC);
}

void RTC_Get_Alarm_Flag(bool *alarm_flag)
{
	*alarm_flag = rtc_alarm;
}

void RTC_Clean_Alarm_Flag()
{
	rtc_alarm = false;
}

void RTC_Get_Alarm_Seconds(uint32_t *alarm_seconds)
{
	*alarm_seconds = rtc_seconds;
}

void RTC_Get_Alarm_Seconds_Flag(bool *seconds_flag)
{
	*seconds_flag = rtc_seconds_flag;
}

void RTC_Clean_Alarm_Seconds_Flag()
{
	rtc_seconds_flag = false;
}

void RTC_Reset_Alarm_Seconds()
{
	rtc_seconds = 0;
}


/////////////////////////////////////////////////////////////RTC///////////////////////////////////////////////////////////////////////////////////////////////
