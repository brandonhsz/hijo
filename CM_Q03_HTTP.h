////////////////////////////Module's Description////////////////////////////////
/*!
 * \file       CM_Q03_HTTP.h
 * \brief      This is the CM_Q03 HTTP driver made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.\n
 *             **to identify the meaning of a CME error, please refer to the
 *             provided Quectel HTTP(S) AT commands manual.\n
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

////////////////////////////////Include Guard///////////////////////////////////
#ifndef CM_Q03_HTTP_H_
#define CM_Q03_HTTP_H_
/////////////////////////////End of Include Guard///////////////////////////////


////////////////////////////////Includes Area///////////////////////////////////
#include <CM_Q03.h>
#include "init_config_functions.h"
#include "fsl_uart.h"
#include <stdio.h>
#include <stdlib.h>

#include  "stdint.h"
#include "stdbool.h"
#include "fsl_gpio.h"
///////////////////////////End of Includes Area/////////////////////////////////

//////////////////////////Structures and Enumerators////////////////////////////

/*!
 	 \brief Enumerator to define the use of customize HTTP(S) request header
 */
typedef enum request_header
{
	request_disable,	/*!< Disable customize request header */
	request_enable		/*!< Enable customize request header */
}request_header_t;

/*!
 	 \brief Enumerator to define the use of HTTP(S) response header
 */
typedef enum response_header
{
	response_disable,		/*!< Disable output response header */
	response_enable			/*!< Enable output response header */
}response_header_t;

/*!
 	 \brief Enumerator to define the data type of HTTP(S) body
 */
typedef enum content_type
{
	x_www_form_urlencoded,	/*!< application/x-www-form-urlencoded */
	plain,					/*!< text/plain */
	octet_stream,			/*!< application/octet-stream */
	form_data				/*!< multipart/form-data */
}content_type_t;

/*!
 	 \brief Structure for the HTTP(S) configuration
 */
typedef struct http_config
{
	uint8_t context_id;					/*!< PDP context ID. Value between 1 and 16 */
	request_header_t request_header;		/*!< Disable or enable to customize request header */
	response_header_t response_header;	/*!< Disable or enable to output response header */
	uint8_t sslctxID;					/*!< SSL context ID used for HTTP(S). Range is 0 - 5 */
	content_type_t data_type;			/*!< Data type of HTTP(S) body */
}http_config_t;


//////////////////////End of Structures and Enumerators/////////////////////////

//////////////////////////////Functions Description/////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function configures the HTTP parameters.

 	 \param[in] http_config_struct Structure for the configuration of HTTP.
 	 \param[in] get_default_config Whether the configuration used will be a default
 	 	 	 	 	 	 	 	 	 one or a personal configuration.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_HTTP_Configure_Parameters(http_config_t http_config_struct,
												configuration_select_t get_default_config);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function sets the URL of HTTP(S) Server

 	 \param[in] url  Pointer to the variable where the URL is stored.
 	 	 	 	 	 Typically an array
	 \param[in] url_lenght  The length of URL. The range is 1-100.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_HTTP_Set_URL(uint8_t *url, uint16_t url_lenght);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function sends a GET Request.

 	 \param[in] rsptime Timeout for the GET response. The range is 1-65535.
 	 \param[in] data Pointer to the variable where the request information is stored.
 	 	 	 	 	 Typically an array.
 	 \param[in] data_length The length of HTTP(S) request information.
 	 	 	 	 	 	 	 The range is 1-2048.
 	 \param[out] content_length Pointer to the variable where the length of the
 	 	 	 	 	 	 	 	 response body is going to be stored.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_HTTP_Send_GET(uint16_t rsptime, uint8_t *data, uint16_t data_lenght,  uint16_t *content_length);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function sends a POST Request.

 	 \param[in] data Pointer to the variable where the request information is stored.
 	 	 	 	 	 Typically an array.
 	 \param[in] data_length If request header is disable, it indicates the length of
 	 	 	 	 	 	 	 post body, and if request header is enable, it indicates
 	 	 	 	 	 	 	 the length of HTTP(S) request information.
 	 	 	 	 	 	 	 The range is 1-10240000.
 	 \param[in] rsptime Timeout for the GET response. The range is 1-65535.
 	 \param[out] content_length Pointer to the variable where the length of the
 	 	 	 	 	 	 	 	 response body is going to be stored.

 	 \return Indicates if an error occurred during communication.
			 Returns the state of the context given context.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_HTTP_Send_POST(uint8_t *data, uint32_t data_length,
									uint16_t rsptime, uint16_t *content_length);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function sends a POST Request via file.

 	 \param[in] file_name Pointer to the variable where the file name is stored.
 	 	 	 	 	 Typically an array.
 	 \param[in] file_name_length The length of the file name. The range is 1-80.
 	 \param[in] rsptime Timeout for the GET response. The range is 1-65535.
	 \param[out] content_length Pointer to the variable where the length of the
							 response body is going to be stored.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_HTTP_Send_POST_via_File(uint8_t *file_name, uint32_t file_name_length,
												uint16_t rsptime, uint16_t *content_length);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function reads the response from HTTP(S) Server.

 	 \param[in] wait_time Maximum interval time between receiving two packets
 	 	 	 	 	 	 	 of data. The range is 1-65535.
	 \param[out] data Pointer to the variable where the data of the
							 response body is going to be stored.
	 \param[out] data_length Pointer to the variable where the length of the
							 response body is going to be stored.
 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_HTTP_Read(uint16_t wait_time, uint8_t *data, uint16_t *data_length);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function returns the status the connection.

 	 \param[out] wait_time ID of the socket service to close. Value between 0 and 2.
 	 \param[in] file_name Pointer to the variable where the file name is stored.
 	 	 	 	 	 Typically an array.
 	 \param[in] file_name_length The length of the file name. The range is 1-80.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_HTTP_Read_via_File(uint16_t wait_time,
									uint8_t *file_name, uint32_t file_name_length);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_HTTP_Get_GET_Request_Status(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_HTTP_Clear_GET_Request_Flag(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_HTTP_Get_POST_Request_Status(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_HTTP_Clear_POST_Request_Flag(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_HTTP_Get_POST_File_Request_Status(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_HTTP_Clear_POST_File_Request_Flag(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_HTTP_Get_Read_Status(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_HTTP_Clear_Read_Flag(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_HTTP_Get_Read_File_Status(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_HTTP_Clear_Read_File_Flag(void);

//////////////////////////End of Functions Description//////////////////////////

/////////////////////////////Include Guard Closing//////////////////////////////
#endif /* CM_Q03_HTTP_H_ */
/////////////////////////End of Include Guard Closing///////////////////////////
