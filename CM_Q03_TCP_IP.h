////////////////////////////Module's Description////////////////////////////////
/*!
 * \file       CM_Q03_TCP_IP.h
 * \brief      This is the CM_Q03 TCP/IP drover made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.\n
 *             **to identify the meaning of a CME error, please refer to the
 *             provided Quectel TCP(IP) AT commands manual.\n
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

////////////////////////////////Include Guard///////////////////////////////////

#ifndef CM_Q03_TCP_IP_H_
#define CM_Q03_TCP_IP_H_

/////////////////////////////End of Include Guard///////////////////////////////

////////////////////////////////Includes Area///////////////////////////////////
#include <CM_Q03.h>
#include <CM_Q03_URC_Control.h>
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
 	 \brief Enumerator to define the IP protocol used.
 */
typedef enum
{
	protocol_IPv4 = 1,	/*!< Protocol IPv4 used*/
	protocol_IPv6,		/*!< Protocol IPv6 used*/
	protocol_IPv4v6
}context_type_t;

/*!
 	 \brief Enumerator to define the authentication used
 */
typedef enum
{
	no_authentication,			/*!< No authentication*/
	PAP_authentication,			/*!< Password Authentication Protocol*/
	CHAP_authentication,		/*!< Challenge Handshake Authentication Protocol*/
	PAP_or_CHAP_authentication	/*!< Either PAP or CHAP can be used*/
}authentication_t;

/*!
 	 \brief Enumerator to define the service type of a socket.
 */
typedef enum
{
	TCP,			/*!< TCP client socket*/
	UDP,			/*!< UDP client socket*/
	TCP_listener,	/*!< TCP server socket*/
	UDP_service,	/*!< UDP server socket*/
	TCP_incoming
}service_type_t;

/*!
 	 \brief Structure to configure the TCP/IP APN parameters
 */
typedef struct
{
	uint8_t context_id;					/*!< ID of the context. Value between 1 and 16*/
	context_type_t context_type;		/*!< IP protocol to be used*/
    authentication_t authentication;	/*!< Authentication protocol to be used*/
}tcp_ip_apn_parameters_struct_t;

/*!
 	 \brief Structure for the socket configuration.
 */
typedef struct
{
    uint8_t context_id;			/*!< ID of the context. Value between 1 and 3*/
    uint8_t connect_id;			/*!< ID of the connection. Value between 0 and 2*/
    uint16_t remote_port;		/*!< Port of the remote server*/
    uint16_t local_port;		/*!< Local port (Device's port to be used)*/
}tcp_ip_open_socket_parameters_struct_t;

typedef struct
{
	uint8_t octet_1;
	uint8_t octet_2;
	uint8_t octet_3;
	uint8_t octet_4;
	uint8_t IP_addr[15];
	uint8_t IP_addr_size;
}IPv4_t;

//////////////////////End of Structures and Enumerators/////////////////////////

//////////////////////////////Functions Description/////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function configures the TCP/IP APN parameters.

 	 \param[in] manual_apn Pointer to the variable with the Access Point Name. Typically an array.
 	 \param[in] size_of_apn Length of manual_apn.
 	 \param[in] username Pointer to a variable with the user name. Typically an array.
 	 \param[in] size_of_username Length of user name.
 	 \param[in] password Pointer to the variable with the password. Typically an array.
 	 \param[in] size_of_password Length of password.
 	 \param[in] parameters_apn_tcp_ip Configuration values of the APN.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_IP_Config_Context_APN(uint8_t *manual_apn, uint16_t size_of_apn,
                                         uint8_t *username, uint16_t size_of_username,
                                         uint8_t *password, uint16_t size_of_password,
                                         tcp_ip_apn_parameters_struct_t parameters_apn_tcp_ip);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function activates a PDP context. The TCP/IP APN must be configured first.

 	 \param[in] contextid  ID of the context. The range is 1 and 3.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_IP_Activate_PDP(uint8_t contextid);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function deactivates a PDP context.

 	 \param[in] contextid ID of the context. The range 1 and 3.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_IP_Deactivate_PDP(uint8_t contextid);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function gets the state of a certain context.

 	 \param[in] contextid Identification number of the context to be
 	 	 	 	 checked.

 	 \return Indicates if an error occurred during communication.
			 Returns the state of the context given context.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_IP_Current_Activated_PDP(uint8_t contextid);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function opens a socket service.

 	 \param[in] service_type Socket service type. See service_type_t.
 	 \param[in] ip_address Pointer to the variable where the IP address is stored.
 	 	 	 	 Typically an array.
 	 \param[in] size_ip_address Length of the IP address.
 	 \param[in] open_parameters Configuration values for the socket to be open.
 	 	 	 	 See tcp_ip_open_socket_parameters_struct_t.

 	 \note When using TCP_listener or UDP_service please use 127.0.0.1 as ip_address.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_Open_Socket(service_type_t service_type,  uint8_t *ip_address,
								uint16_t size_ip_address,tcp_ip_open_socket_parameters_struct_t open_parameters);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function closes a socket service.

 	 \param[in] connectid ID of the socket service to close. Value between 0 and 2.
 	 \param[in] timeout Maximum time, in seconds, this function has to complete its task.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_IP_Close_Socket(uint8_t connectid, uint16_t timeout );

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function returns the status the connection.

 	 \param[out] connectid ID of the socket service to close. Value between 0 and 2.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_IP_Query_Socket_Status(uint8_t connectid);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function sends data using UDP protocol. The data is sent in ASCII.

 	 \warning This function can only be used for UDP service sockets.

 	 \param[in] connectid ID of the socket service. Value between 0 and 2.
 	 \param[in] data Pointer to the data to be sent. Typically an array.
 	 	 	 	 Must be in ASCII format
 	 \param[in] size_data Length of the data to be sent,in bytes.
 	 	 	 	 Must be between 0 and 1460
	 \param[in] remote_ip Pointer to the variable where the remote IP address is stored.
	 	 	 	 Typically an array.
 	 \param[in] size_remote_ip Length of the remote IP address.
	 \param[in] remote_port Port of the remote host.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_IP_UDP_Send(uint8_t connectid, uint8_t *data, uint16_t size_data,
										 uint8_t *remote_ip, uint16_t size_remote_ip, uint16_t remote_port);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function sends data using TCP/IP protocol. The data is sent in ASCII.

 	 \warning This function cannot be used for UDP service sockets.

 	 \param[in] connectid ID of the socket service. Value between 0 and 2.
 	 \param[in] string Pointer to the data to be sent. Typically an array.
 	 \param[in] size_string Length of the data to be sent. Must be between 0 and 1460

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_IP_Send_String(uint8_t connectid, uint8_t *string, uint16_t size_string );

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function sends data using TCP/IP protocol. The data is sent in hex format.

 	 \warning This function cannot be used for TCP listener or UDP service sockets.

 	 \param[in] connectid ID of the socket service. Value between 0 and 2.
 	 \param[in] hex_string Pointer to the data to be sent. Typically an array.
 	 \param[in] size_hex_string Length of the data to be sent.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_IP_Send_hex_String(uint8_t connectid, uint8_t *hex_string, uint16_t size_hex_string );

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function returns the data received by TCP.

 	 \warning This function cannot be used to retrieve UDP service messages.

 	 \param[in] connectid ID of the socket service. Value between 0 and 2.
 	 \param[out] return_string Pointer to the variable where the data will be stored.
 	 	 	 	 	 	 	 	 Typically an array.
 	 \param[out] size_return_string Pointer to the variable where the length of the
 	  	  	  	  	  	  	  	  	  data received will
 	 	 	 	 be stored.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_IP_Retrieve_Received(uint8_t connectid, uint8_t *return_string,
												uint16_t *size_return_string );

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function returns the data received by UDP service sockets.

 	 \warning This function can only be used to retrieve UDP service messages.

 	 \param[in] connectid ID of the socket service. Value between 0 and 2.
 	 \param[out] return_string Pointer to the variable where the data will be stored.
 	 	 	 	 	 	 	 	 Typically an array.
 	 \param[out] size_return_string Pointer to the variable where the length of the
 	 	 	 	 	 	 	 	 	 data received will be stored.
	 \param[out] remote_ip Pointer to the variable where the remote IP will be stored.
	 	 	 	 	 	 	 Typically an array.
 	 \param[out] size_remote_ip Pointer to the variable where the length of the remote
 	 	 	 	 	 	 	 	 IP received will be stored.
	 \param[out] remote_port Pointer to the variable where the remote port will be stored.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_IP_UDP_Service_Received(uint8_t connectid,
												  uint8_t *return_string, uint16_t *size_return_string,
												  uint8_t *remote_ip, uint16_t *size_remote_ip,
												  uint16_t *remote_port);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function returns the IP address of the device.

 	 \param[in] contextid ID of the socket service. The range is 0-2.
 	 \param[out] ip_address Pointer to the variable where the data will be stored.
 	 \param[out] ip_address_size Length of the IP address.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_IP_Address(uint8_t contextid, uint8_t *ip_address,
		uint16_t *ip_address_size);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_TCP_IP_Get_Last_Error_Code_Status_Flag(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_TCP_IP_Clear_Last_Error_Code_Status_Flag(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_TCP_IP_Get_Last_Error_Code_Status(cm_q03_codes_t *error_code);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_TCP_IP_Open_Socket_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_IP_Clear_Open_Socket_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_TCP_IP_Open_Socket_Status(uint8_t connectID, cm_q03_codes_t *status);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_TCP_IP_Send_Data_Status_Flag(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_TCP_IP_Clear_Send_Data_Status_Flag(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_TCP_IP_Send_Data_Status(uint16_t *total_sent_len,
		uint16_t *acked_bytes, uint16_t *nacked_bytes);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_TCP_IP_Socket_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_IP_Clear_Socket_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_TCP_IP_Socket_Status(uint8_t connectID,
		cm_q03_codes_t *socket_state, service_type_t *service_type,
		IPv4_t *IP_addr, uint16_t *remote_port, uint16_t *local_port,
		uint8_t *context_id, uint8_t *server_id);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_TCP_IP_Incoming_Full_Flag(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_TCP_IP_Clear_Incoming_Full_Flag(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_TCP_IP_Incoming_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_IP_Clear_Incoming_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_TCP_IP_Incoming_Status(uint8_t connectID,
		cm_q03_codes_t *socket_status, uint8_t *server_ID, IPv4_t *IP_addr,
		uint16_t *remote_IP);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_TCP_IP_PDP_Deactivated_Flag(uint8_t contextID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_IP_Clear_PDP_Deactivated_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_TCP_IP_Received_Data_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_IP_Clear_Received_Data_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_TCP_IP_Socket_Closed_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_TCP_IP_Clear_Socket_Closed_Flag(uint8_t connectID);

//////////////////////////End of Functions Description//////////////////////////

/////////////////////////////Include Guard Closing//////////////////////////////
#endif /* CM_Q03_TCP_IP_H_ */
/////////////////////////End of Include Guard Closing///////////////////////////
