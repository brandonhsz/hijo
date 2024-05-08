////////////////////////////Module's Description////////////////////////////////
/*!
 * \file       CM_Q03_MQTT.h
 * \brief      This is the CM_Q03 MQTT driver made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.\n
 *             **to identify the meaning of a CME error, please refer to the
 *             provided Quectel MQTT AT commands manual.\n
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

////////////////////////////////Include Guard///////////////////////////////////

#ifndef CM_Q03_MQTT_H_
#define CM_Q03_MQTT_H_

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

/////////////////////////////Definitions Area///////////////////////////////////


//////////////////////////End of Definitions Area///////////////////////////////

//////////////////////////Structures and Enumerators////////////////////////////

/*!
 	 \brief Enumerator to define the will flag configuration.
 */
typedef enum
{
	Ignore_Will_flag, 	/*!< Ignores LWT*/
	Require_Will_flag	/*!< Requires LWT*/
}will_flag_t;

/*!
 	 \brief Enumerator to choose the MQTT version
 */
typedef enum
{
	MQTT_v3 = 3,	/*!< Version 3*/
	MQTT_v3_1 = 4		/*!< Version 4*/
}mqtt_protocol_version_t;

/*!
 	 \brief Enumerator to choose between the different QoS
 */
typedef enum
{
	at_most_once,	/*!< The message is delivered only once or is not delivered*/
	at_least_once,	/*!< The message is delivered at least one time*/
	exactly_once	/*!< The message should only be delivered one time*/
}quality_of_service_t;

/*!
 	 \brief Enumerator to define the timeout notice.
 */
typedef enum
{
	not_report,	/*!< Not report timeout.*/
	report		/*!< Report timeout*/
}timeout_notice_t;

/*!
 	 \brief Enumerator to define the session type
 */
typedef enum
{
	persistent_session, /*!< The server must store the subscriptions
							  of the client after it disconnects.*/
	clean_session		/*!< The server must discard any previously maintained
							  information about the client and treat the
							  connection as "clean"*/
}session_type_t;

/*!
 	 \brief Enumerator to define whether the TCP will have a normal connection,
 	 	 	 or a secured connection.
 */
typedef enum
{
	ssl_disabled,	/*!< Use normal TCP connection for MQTT*/
	ssl_enabled		/*!< Use SSL TCP secure connection for MQTT*/
}ssl_enable_t;

/*!
 	 \brief Enumerator to define whether the server will retain the message or not.
 */
typedef enum
{
	not_retain,	/*!< The server will not retain the message after it has been
						delivered to the current subscribers.*/
	retain		/*!< The server will retain the message after it has been
						delivered to the current subscribers*/
}retain_type_t;

/*!
 	 \brief Structure for the MQTT configuration.
 */
typedef struct
{
	mqtt_protocol_version_t version; 	/*!< MQTT protocol version.*/
    uint8_t pdpcid; 					/*!< PDP to be used by the MQTT client. The range is 1-16 */
    uint8_t pkt_timeout;				/*!< Timeout, in seconds, of the packet delivery. The range is 1-60*/
    uint8_t retry_times;				/*!< Retry times when the packet times out. The range is 0-10*/
    timeout_notice_t timeout_notice;	/*!< Whether a timeout message will be reported
     	 	 	 	 	 	 	 	 	 	 or not during transmission.*/
    session_type_t clean_session; 		/*!< Configure the session type.*/
    uint16_t keepalive;	 				/*!< Keep-alive time. The range is 0-3600 */
    ssl_enable_t ssl_enable; 			/*!< Configure the MQTT SSL Mode*/
    uint8_t ctx_index;					/*!< Index of the SSL context index. The range is 0-5.*/
} mqtt_config_t;
//////////////////////End of Structures and Enumerators/////////////////////////

//////////////////////////////Functions Description/////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Configure optional parameters.

   \param[in] tcpconnectID       MQTT socket identifier. The range is 0-2.
   \param[in] mqtt_config_struct   Structure with the configuration
                                parameters of MQTT Protocol.
   \param[in] get_default_config Indicates whether or not to use the default
                                configuration.
   \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Configure_Parameters(uint8_t tcpconnectID,
                                          mqtt_config_t mqtt_config_struct,
										  configuration_select_t get_default_config);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function sets the Last Will and Testament (LWT) of the device.
 	 	 	 The LWT is a message that will be sent to all devices subscribed to
 	 	 	 the broker if a device disconnects "ungracefully".

 	 \param[in] tcpconnectID MQTT socket identifier. The range is 0-2.
 	 \param[in] flag Whether the flag configuration will be ignored or not.
 	 \param[in] qos Quality of Service level at which the client
                    wants to publish the messages.
 	 \param[in] retain Whether or not the server will retain the
                  	  message after it has been delivered to the
                       current subscribers.
 	 \param[in] topic Pointer to the variable where the topic name is. Typically an array.
 	 \param[in] size_topic Length of the topic name. The range is 1-100
 	 \param[in] msg Pointer to the variable where the message to be sent is.
 	 	 	 	 	 Typically an array.
 	 \param[in] size_msg Length of the message to be sent. The range is 1-500

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Configure_Will_Info(uint8_t tcpconnectID,
										 will_flag_t flag, quality_of_service_t qos,retain_type_t retain,
                                         uint8_t *topic, uint8_t size_topic,
                                         uint8_t *msg, uint16_t size_msg);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Open a network for MQTT client.

   \param[in] tcpconnectID           MQTT socket identifier. The range is 0-2.
   \param[in] host                   Address of the MQTT Broker.
                                     It could be an IP or a domain name
   \param[in] host_size				 Number of characters in host name vector.
   	   	   	   	   	   	   	   	   	 The range is 1-100
   \param[in] port                   Port of the MQTT Broker.
                                     The range is 1-65535
   \return Returns the state of the network.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Open_Network(uint8_t tcpconnectID,
                                  uint8_t *host, uint16_t host_size,
                                  uint16_t port);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Close a network for MQTT client.

   \param[in] tcpconnectID    MQTT socket identifier. The range is 0-2.
   \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Close_Network(uint8_t tcpconnectID);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Create a client request connection to MQTT Broker.

   \param[in] tcpconnectID               MQTT socket identifier. The range is 0-2.
   \param[in] clientID                   Client identifier string
   \param[in] size_clientID 			 Number of characters in clientID vector
   \param[in] username                   User name of the client
   \param[in] size_username				 Number of characters in user name vector
   \param[in] password                   Password corresponding to the user name
                                         of the client
   \param[in] size_password				 Number of characters in clientID vector

   \return Indicates the state of the connection.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Connect(uint8_t tcpconnectID,
                          uint8_t *clientID, uint8_t size_clientID,
                          uint8_t *username, uint8_t size_username,
                          uint8_t *password, uint8_t size_password
                          );

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function returns the MQTT connection status.

 	 \param[in] tcpconnectID MQTT socket identifier. The range is 0-2.

 	 \return Returns the state of the connection.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Connection_Status(uint8_t tcpconnectID);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Request a disconnection from MQTT Broker.

   \param[in] tcpconnectID    MQTT socket identifier. The range is 0-2.

   \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Disconnect(uint8_t tcpconnectID);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Subscribes to a topic.

   \param[in] tcpconnectID          MQTT socket identifier. The range is 0-2.
   \param[in] msgID					Message identifier.
   \param[in] topic                  Topic that the client want to
                                      subscribe to from
   \param[in] size_topic Number of characters in name vector. The range is 1-100
   \param[in] QoS                   Quality of Service level at which the client
                                      wants to publish the messages.
   \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Subscribe(uint8_t tcpconnectID,
                               uint16_t msgID,
                               uint8_t *topic, uint8_t size_topic,
							   quality_of_service_t QoS);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Unsubscribes from a topic.

   \param[in] tcpconnectID          MQTT socket identifier. The range is 0-2.
   \param[in] msgID					Message identifier.
   \param[in] topic                  Topic that the client want to unsubscribe from
   \param[in] size_topic Number of characters in name vector

   \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Unsubscribe(uint8_t tcpconnectID,
                                 uint16_t msgID,
                                 uint8_t *topic,
                                 uint8_t size_topic);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Publish messages to the MQTT Broker for distribution to
          interested subscribers.

   \param[in] tcpconnectID              MQTT socket identifier. The range is 0-2.
   \param[in] msgID						Message identifier. The range is 1-65535
   \param[in] topic                     Topic that needs to be published
   \param[in] size_topic			  	Number of characters in topic vector
   \param[in] payload                   Message to be published
   \param[in] size_payload				Number of characters in payload vector
   \param[in] QoS                       Quality of Service level at which the
                                         client wants to publish the messages.
   \param retain                    Whether or not the server will retain the
                                    message after it has been delivered to the
                                    current subscribers.

   \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Publish(uint8_t tcpconnectID,
                             uint16_t msgID,
                             uint8_t *topic, uint8_t size_topic,
                             uint8_t *payload, uint16_t size_payload,
							 quality_of_service_t QoS, retain_type_t retain);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief This function start a publish process.

   \param[in] tcpconnectID              MQTT socket identifier. The range is 0-2.
   \param[in] msgID						Identification number of the message.
   \param[in] topic                     Topic that needs to be published
   \param[in] size_topic   				Number of characters in topic vector
   \param[in] QoS                       Quality of Service level at which the
                                        client wants to publish the messages.
   \param[in] retain                    Whether or not the server will retain the
                                        message after it has been delivered to the
                                          current subscribers.

   \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Start_Publish(uint8_t tcpconnectID,
                                   uint16_t msgID,
                                   uint8_t *topic, uint8_t size_topic,
								   quality_of_service_t QoS, retain_type_t retain);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function saves data into a buffer buffer, this data will be published
 	 	 	 when the publish process ends.

 	 \param[in] payload Pointer to the variable where the data to be sent is.
 	 	 	 	 Typically an array.
 	 \param[in] size_payload Length of the data to be sent.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Write_Publish(uint8_t *payload, uint16_t size_payload);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief This function finish a publish process.

   \param[in] tcpconnectID MQTT socket identifier. The range is 0-2.

   \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_End_Publish(uint8_t tcpconnectID);

////////////////////////////////////////////////////////////////////////////////
/*!
	\brief This function reads a message published in a topic to which the device
			is subscribed to.

	\note This function has to be called for each message published in the topic.
			i.e. If three messages have been published, this function has to be
			called three times.

	\param[out] tcpconnectID Pointer to the variable where the MQTT socket
								identifier will be stored.
	\param[out] msgID Pointer to the variable where the message identifier will
						be stored.
	\param[out] topic Pointer to the variable where the topic will be stored.
						Typically an array.
	\param[out] size_of_topic Pointer to the variable where the length of the
								topic will be stored.
	\param[out] msg Pointer to the variable where the message will be stored.
					Typically an array.
	\param[out] size_of_msg Pointer to the variable where the length of the
							message will be stored.

	\return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Read_Message(uint8_t* tcpconnectID, uint16_t* msgID, uint8_t* topic,
								  uint16_t* size_of_topic, uint8_t* msg, uint16_t* size_of_msg);

////////////////////////////////////////////////////////////////////////////////


cm_q03_codes_t CM_Q03_MQTT_Store_Messages(uint8_t Client_ID, uint8_t* Store_msg);


cm_q03_codes_t CM_Q03_MQTT_Read_Store_Message(uint8_t Client_ID, uint8_t* store_status, uint16_t*msgID, uint8_t* topic,
        uint16_t* size_of_topic, uint8_t* msg, uint16_t* size_of_msg);

////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_MQTT_Close_Network_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Clear_Close_Network_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_MQTT_Close_Network_Status(uint8_t connectID,
		cm_q03_codes_t *status);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_MQTT_Connect_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Clear_Connect_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_MQTT_Connect_Status(uint8_t connectID, cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_MQTT_Disconnect_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Clear_Disconnect_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_MQTT_Disconnect_Status(uint8_t connectID, cm_q03_codes_t *status);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_MQTT_Open_Network_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Clear_Open_Network_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_MQTT_Open_Network_Status(uint8_t connectID, cm_q03_codes_t *status);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_MQTT_Publish_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Clear_Publish_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_MQTT_Publish_Status(uint8_t connectID, cm_q03_codes_t *status,
		uint16_t *msgID, uint16_t *retransmissions);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_MQTT_Receive_Msg_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Clear_Receive_Msg_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Receive_Msg_Status(uint8_t connectID,
		uint16_t *msgID, uint8_t *topic, uint16_t *topic_size, uint8_t *msg,
		uint16_t *msg_size);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_MQTT_Get_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Clear_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_MQTT_Get_Status(uint8_t connectID, cm_q03_codes_t *status);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_MQTT_Subscribe_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Clear_Subscribe_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_MQTT_Subscribe_Status(uint8_t connectID,
		cm_q03_codes_t *status, uint16_t *msgID, uint16_t *value);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_MQTT_Unsubscribe_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_MQTT_Clear_Unsubscribe_Status_Flag(uint8_t connectID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_MQTT_Unsubscribe_Status(uint8_t connectID, cm_q03_codes_t *status,
		uint16_t *msgID);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

urc_received_status_t CM_Q03_MQTT_Ready_Publish_Status_Flag(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void CM_Q03_MQTT_Clear_Ready_Publish_Status_Flag(void);
//////////////////////////End of Functions Description//////////////////////////

/////////////////////////////Include Guard Closing//////////////////////////////
#endif /* CM_Q03_MQTT_H_ */
/////////////////////////End of Include Guard Closing///////////////////////////
