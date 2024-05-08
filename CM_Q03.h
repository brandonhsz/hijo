////////////////////////////Module's Description////////////////////////////////
/*!
 * \file       CM_Q03.h
 * \brief      This is the CM_Q03 driver made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

////////////////////////////////Include Guard///////////////////////////////////
#ifndef SOURCE_CM_Q03_H_
#define SOURCE_CM_Q03_H_
/////////////////////////////End of Include Guard///////////////////////////////

////////////////////////////////Includes Area///////////////////////////////////
#include <CM_Q03_codes.h>
#include <CM_Q03_URC_Control.h>
#include "init_config_functions.h"
#include "fsl_uart.h"
#include <stdio.h>
#include <stdlib.h>
#include  "stdint.h"
#include "stdbool.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_lpuart.h"
///////////////////////////End of Includes Area/////////////////////////////////

///////////////////////

/** Defines the hexadecimal value of '0'*/
#define ASCII_0 		0x30
//////////////////////////End of Definitions Area///////////////////////////////

//////////////////////////Structures and Enumerators////////////////////////////
/*!
 	 \brief Enumerator to control the shield's state.
 */
typedef enum{
	cm_q03_turn_on,		/*!< Turns on the CM*/
	cm_q03_turn_off,	/*!< Turns off the CM*/
	cm_q03_reset		/*!< Resets the CM*/
}cm_q03_control_t;

/*!
 	 \brief Enumerator to define the baud rates of the CM.
 */
typedef enum
{
	baud_rate_9600 = 9600,		/*!< Sets a baud rate of 9600 bps*/
	baud_rate_19200 = 19200,	/*!< Sets a baud rate of 19200 bps*/
	baud_rate_38400 = 38400,	/*!< Sets a baud rate of 38400 bps*/
	baud_rate_57600 = 57600,	/*!< Sets a baud rate of 57600 bps*/
	baud_rate_115200 = 115200,	/*!< Sets a baud rate of 115200 bps*/
	baud_rate_230400 = 230400,	/*!< Sets a baud rate of 230400 bps*/
	baud_rate_460800 = 460800,	/*!< Sets a baud rate of 460800 bps*/
	baud_rate_921600 = 921600	/*!< Sets a baud rate of 921600 bps*/
}cm_baud_rate_t;

typedef enum
{
	Immediately_PowerDown,
	Normal_PowerDown
}cm_q03_powd_t;
//////////////////////End of Structures and Enumerators/////////////////////////

//////////////////////////////Functions Description/////////////////////////////

/*!
 	 \brief This function controls the state of the cellular communication module.

 	 \param[in] CM_command_control New state of the CM. See cm_q03_control_t.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
cm_q03_codes_t CM_Q03_Control(cm_q03_control_t CM_command_control);


/*!
 	 \brief This function sets the baud rate of the CM. If this function is not
 	 	 	 	 	 	 	 called, the default baud rate of the CM will be
 	 	 	 	 	 	 	 115200 bps.

 	 \param[in] Baud_Rate The new baud rate of the CM.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
cm_q03_codes_t CM_Q03_Set_Baud_Rate(cm_baud_rate_t Baud_Rate);

/*!
 	 \brief This function gets the current configured baud rate.

 	 \return This function returns the current configured baud rate.
 */
cm_baud_rate_t CM_Q03_Get_Baud_Rate(void);


cm_q03_codes_t CM_Q03_Power_Down(cm_q03_powd_t type);
//////////////////////////////Functions Description/////////////////////////////

/*!
 	 NOTE: The following function set is NOT intended to be used by the
 	 	 	 client.
 */

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*!
 	 \private
 */
uint32_t CM_Q03_Write(uint8_t *txbuff, uint16_t size, uint32_t timeout);

/*!
 	 \private
 */
uint32_t CM_Q03_Read_Until_OK(uint8_t *rx_buff, uint16_t *size, uint32_t timeout);

/*!
 	 \private
 */
uint32_t CM_Q03_HTTP_Read_Until_OK(uint8_t *rx_buff, uint16_t *size, uint32_t timeout);

/*!
 	 \private
 */
uint32_t CM_Q03_Read(uint8_t *rxbuff, uint16_t *size, uint32_t timeout);

/*!
 	 \private
 */
uint32_t Character_finder(uint8_t *ptr_buffer,
					  uint16_t index_of_buffer,
					  uint8_t character_start_end,
					  uint16_t buffer_size
					  );


/*!
 	 \private
 */
uint32_t CM_Q03_Compare_response(uint8_t *buffer_to_be_compare,
							  uint16_t size_buffer_to_be_compare,
							  uint8_t *compare_buffer,
							  uint16_t size_compare_buffer,
							  uint16_t valide_character_count,
							  uint16_t *finish_count,
							  uint32_t time_out);

/*!
 	 \private
 */
uint32_t CM_Q03_Compare_short_response(uint8_t *buffer_to_be_compare,
							  uint16_t size_buffer_to_be_compare,
							  uint8_t *compare_buffer,
							  uint16_t size_compare_buffer,
							  uint32_t time_out);

/*!
 	 \private
 */
cm_q03_codes_t CM_Q03_Delay(uint32_t delay);

/*!
 	 \private
 */
cm_q03_codes_t CM_Q03_Get_Response(uint8_t *rxbuff, uint16_t size);

/*!
 	 \private
 */
cm_q03_codes_t Ascii_to_Code(uint8_t *buffer, uint16_t size_buff);

/*!
 	 \private
 */
uint64_t Ascii_to_Number(uint8_t *buffer, uint16_t size_buff);

/*!
 	 \private
 */
uint32_t CM_Q03_Compare_response_Error(uint8_t *buffer_to_be_compare,
									 uint16_t size_buffer_to_be_compare,
									 uint8_t *compare_buffer,
									 uint16_t size_compare_buffer,
									 uint16_t offset_error,
									 uint8_t max_num_compare,
									 uint32_t time_out);

/*!
 	 \private
 */
uint32_t CM_Q03_Compare_vector_response(uint8_t *buffer_to_be_compare,
									 uint16_t size_buffer_to_be_compare,
									 uint8_t *compare_buffer,
									 uint16_t size_compare_buffer,
									 uint16_t offset_error,
									 uint8_t max_num_compare,
									 uint8_t count_valid_response,
									 uint32_t time_out);

/*!
 	 \private
 */
void CM_Q03_Flush_URC();

/*!
 	 \private
 */
void Dec_to_Ascii(uint32_t num_to_convert, uint8_t *ascii_buff, uint16_t *size_buffer); // Peko modification

/*!
 	 \private
 */
uint32_t CM_Q03_read_URC_vector(uint8_t *rx_urc_buff, uint16_t *size_rx_urc_buff,
                               uint8_t *compare_buffer,
                               uint16_t size_compare_buffer, uint16_t time_out);

/*!
 	 \private
 */
uint32_t CM_Q03_Read_Wait_URC(uint8_t *rxbuff, uint16_t *size, uint32_t timeout);

/*!
 	 \private
 */
void Concatenate_Command(uint8_t *dest, uint8_t *src, uint16_t size_src, uint16_t *size_dest);

/*!
 	 \private
 */
void CM_Q03_buffer_clean();

/*!
 	 \private
 */
void CM_Q03_URC_buffer_clean();

/*!
 	 \private
 */
void CM_Q03_buffer_reset();

/*!
 	 \private
 */
void CM_Q03_URC_buffer_reset();

/*!
 	 \private
 */
uint32_t CM_Q03_Read_Response_Start_and_Stop_read_character(
        uint8_t *rxbuff, uint16_t size_rxbuff, uint8_t *output_buff,
        uint16_t *size_output_buff, uint8_t start_character,
        uint8_t stop_character);

/*!
 	 \private
 */
uint32_t CM_Q03_Read_Response_count_Start_and_Stop_read_character(
        uint8_t *rxbuff, uint16_t size_rxbuff, uint8_t *output_buff,
        uint16_t *size_output_buff, uint8_t search_character,
        uint8_t count_search_character, uint8_t stop_character);

/*!
 	 \private

 */

uint8_t CM_Q03_Read_response_signal_type(uint8_t *rxbuff, uint16_t size_rxbuff); //mio
/*!
     \private

 */
uint32_t CM_Q03_Read_Response_Start_and_Stop_read_character_end_ptr(
        uint8_t *rxbuff, uint16_t size_rxbuff, uint8_t *output_buff,
        uint16_t *size_output_buff, uint8_t start_character,
        uint8_t stop_character, uint8_t **ptr_end);

/*!
 	 \private
 */
uint32_t CM_Q03_Read_Response_count_Start_N_characters(
		uint8_t *rxbuff, uint16_t size_rxbuff, uint8_t *output_buff,
		uint16_t *size_output_buff, uint8_t search_character,
		uint8_t count_search_character, uint16_t N_characters);

/*!
 	 \private
 */
uint32_t CM_Q03_Read_Response_count_Start_and_Stop_read_character_end_ptr(
        uint8_t *rxbuff, uint16_t size_rxbuff, uint8_t *output_buff,
        uint16_t *size_output_buff, uint8_t search_character,
        uint8_t count_search_character, uint8_t stop_character,
        uint8_t **ptr_end);

/*!
 	 \private
 */
uint32_t CM_Q03_Read_Response_Vector_Until(uint8_t *rxbuff, uint16_t *size,
											uint8_t stop_character, uint32_t timeout);

/*!
 	 \private
 */
uint32_t CM_Q03_Read_Response_Vector_N_Characters(uint16_t length_to_read, uint8_t* rxbuff,
													uint16_t* size, uint32_t timeout);

/*!
 	 \private
 */
uint32_t CM_Q03_AP_Ready(uint8_t level, uint16_t interval);

/*!
 	 \private
 */
uint8_t *Get_RI_Flag();

/*!
 	 \private
 */
void CM_Q03_Return_To_Last_Character_End();

/*!
 	 \private
 */
void CM_Q03_RI_Clean_flag(void);

/*!
 	 \private
 */
bool CM_Q03_RI_Get_State(void);

cm_q03_codes_t CM_Q03_Set_Command_Terminator_Character_ATS_Reconnect(void);

cm_q03_codes_t CM_Q03_Get_parameter_JSON(uint8_t *buffer_to_be_compare,
                                  uint16_t size_buffer_to_be_compare,
                                  uint8_t *compare_parameter,
                                  uint16_t size_compare_parameter,
                                  uint8_t end_compare_parameter,
                                  int32_t *return_parameter);


cm_q03_codes_t CM_Q03_Get_string_JSON(uint8_t *buffer_to_be_compare,
                                  uint16_t size_buffer_to_be_compare,
                                  uint8_t *compare_parameter,
                                  uint16_t size_compare_parameter,
                                  uint8_t end_compare_parameter,
                                  uint8_t *return_string,
                                  uint16_t *size_return_string);
//////////////////////////End of Functions Description//////////////////////////
typedef enum{
    Port_VDD_EG915,
    Port_RI_EG915,
    Port_PWR_KEY_EG915,
    Port_RESET,
    Port_AP_Ready,
    Port_NW_Disable,
    Port_DTR,
    Port_PSM_IND,
    Port_Status,
}port_gpio_cm_q03;
 general_codes_t CM_Q03_GPIO_Read(port_gpio_cm_q03 Port_select, uint8_t* input);

/////////////////////////////Include Guard Closing//////////////////////////////
#endif /* SOURCE_CM_Q03_H_ */
/////////////////////////End of Include Guard Closing///////////////////////////
