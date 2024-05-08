////////////////////////////Module's Description////////////////////////////////
/*!
 * \file       CM_Q03_SMS.h
 * \brief      This is the CM_Q03 SMS driver made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.\n
 *             **to identify the meaning of a CME error, please refer to the
 *             provided Quectel AT commands manual.\n
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

////////////////////////////////Include Guard///////////////////////////////////
#ifndef CM_Q03_SMS_H_
#define CM_Q03_SMS_H_
/////////////////////////////End of Include Guard///////////////////////////////

////////////////////////////////Includes Area///////////////////////////////////
#include <CM_Q03.h>
#include <CM_Q03_URC_Control.h>
#include <CM_Q03_codes.h>
#include <CM_Q03_General.h>
///////////////////////////End of Includes Area/////////////////////////////////

/////////////////////////////Definitions Area///////////////////////////////////

//////////////////////////End of Definitions Area///////////////////////////////

//////////////////////////Structures and Enumerators////////////////////////////

/*!
 	 \brief Enumerator to set the SMS message format to be used.
 */
typedef enum
{
	sms_pdu_mode,	/*!< PDU mode */
	sms_text_mode	/*!< Text mode */
}sms_msg_format_t;

/*!
 	 \brief Enumerator to set the SMS memory storage to be used.
 */
typedef enum
{
	sms_sim,		/*!< Store message in SIM memory */
	sms_device		/*!< Store message in device memory */
}sms_mem_storage_t;

typedef enum
{
	sms_recv_in_sim,
	sms_recv_in_device,
	sms_recv_in_report_storage,
	sms_recv_unknown
}sms_recv_mem_storeage_t;

/*!
 	 \brief Enumerator to select which messages are going to be deleted.
 */
typedef enum
{
	sms_delete_index_msg,		/*!< Delete message on the index selected */
	sms_delete_all_read_msgs,	/*!< Delete all read messages */
	sms_delete_all_msgs = 4		/*!< Delete all messages */
}sms_delete_msg_t;

//////////////////////End of Structures and Enumerators/////////////////////////

//////////////////////////////Functions Description/////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Configure the mode to be used in SMS.

   \param[in] mode	Message format.

   \return	This function indicates if the task was successful, or if an error
 	 	 	occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_SMS_Message_Format(sms_msg_format_t mode);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Read the SMSC address when SMS are transmitted.

   \param[out] center_address		Pointer to the variable where the service
   	   	   	   	   	   	   	   	   	   center address will be stored.
   \param[out] center_address_size	Pointer to the variable where the length of
   	   	   	   	   	   	   	   	   	   service center address will be stored.
   \param[out] type_address			Pointer to the variable where the type of
   	   	   	   	   	   	   	   	   	    service center address will be stored.

   \return	This function indicates if the task was successful, or if an error
 	 	 	occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_SMS_Service_Center_Address(uint8_t *center_address,
												uint16_t *center_address_size,
												uint16_t *type_address);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Select the memory storage to be used for received messages.

   \param[in] message_storage			Message storage.
   \param[out] message_in_storage		Number of current messages in storage
   	   	   	   	   	   	   	   	   	   	selected.
   \param[out] total_size_storage		Total number of messages which can be
   	   	   	   	   	   	   	   	   	   	stored in the storage selected.

   \return	This function indicates if the task was successful, or if an error
 	 	 	occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_SMS_Select_Received_Message_Storage(sms_mem_storage_t message_storage,
													uint8_t *message_in_storage,
													uint8_t *total_size_storage);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Get the number of current received messages and the total number of
   	   	   messages which can be stored.

   \param[out] message_in_storage		Number of current messages in storage
   	   	   	   	   	   	   	   	   	   	selected.
   \param[out] total_size_storage		Total number of messages which can be
   	   	   	   	   	   	   	   	   	   	stored in the storage selected.

   \return	This function indicates if the task was successful, or if an error
 	 	 	occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_SMS_Number_of_Message_Received(uint8_t *message_received,
												uint8_t *total_size_storage);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Get the current messages and total number of messages in the selected
   	   	   memory storage.

   \param[in] message_storage			Message storage.
   \param[out] message_in_storage		Number of current messages in storage
   	   	   	   	   	   	   	   	   	   	selected.
   \param[out] total_size_storage		Total number of messages which can be
   	   	   	   	   	   	   	   	   	   	stored in the storage selected.

   \return	This function indicates if the task was successful, or if an error
 	 	 	occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_SMS_Message_and_Total_size_in_Storage(sms_mem_storage_t message_storage,
														uint8_t *message_in_storage,
														uint8_t *total_size_storage);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Delete messages from select memory storage.

   \param[in] message_storage	Message storage.
   \param[in] msg_index			Index of message to be deleted.
   \param[in] msgs_to_delete	Select whether delete the message specified
   	   	   	   	   	   	   	   	in index, delete all read messages or delete
   	   	   	   	   	   	   	   	all messages from memory selected.

   \return	This function indicates if the task was successful, or if an error
 	 	 	occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_SMS_Delete_Message(sms_mem_storage_t message_storage,
									uint16_t msg_index,
									sms_delete_msg_t msgs_to_delete);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Read messages from select memory storage.

   \param[in]  message_storage	Message storage.
   \param[in]  msg_index		Index of message to be read.
   \param[out] orig_addr		Pointer to the variable where the Originating
   	   	   	   	   	   	   	   	   Address will be stored.
								   Typically an array.
   \param[out] orig_addr_size	Pointer to the variable where the length of the
   	   	   	   	   	   	   	   	   Originating Address will be stored.
   \param[out] time_stamp		Pointer to the variable where the Service Center
   	   	   	   	   	   	   	   	   Time Stamp will be stored.
								   Typically an array.
   \param[out] time_stamp_size	Pointer to the variable where the length of the
   	   	   	   	   	   	   	   	   Service Center Time Stamp will be stored.
   \param[out] payload			Pointer to the variable where the text of short
   	   	   	   	   	   	   	   	   message will be stored.
								   Typically an array.
   \param[out] payload_size		Pointer to the variable where the length of the
   	   	   	   	   	   	   	   	   text of short message will be stored.

   \return	This function indicates if the task was successful, or if an error
 	 	 	occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_SMS_Read_Message(sms_mem_storage_t message_storage,
									uint16_t msg_index, uint8_t *orig_addr,
									uint16_t *orig_addr_size, uint8_t *time_stamp,
									uint16_t *time_stamp_size, uint8_t *payload,
									uint16_t *payload_size);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Read messages from select memory storage in PDU mode.

   \param[in]  message_storage	Message storage.
   \param[in]  msg_index		Index of message to be read.
   \param[out] length			Pointer to the variable where the length of the
   	   	   	   	   	   	   	   	   actual data unit in octets will be stored.
   \param[out] payload			Pointer to the variable where the text of short
   	   	   	   	   	   	   	   	   message will be stored.
								   Typically an array.
   \param[out] payload_size		Pointer to the variable where the length of the
   	   	   	   	   	   	   	   	   text of short message will be stored.

   \return	This function indicates if the task was successful, or if an error
 	 	 	occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_SMS_Read_Message_PDU(sms_mem_storage_t message_storage,
									uint16_t msg_index, uint16_t *length,
									uint8_t *payload, uint16_t *payload_size);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Sends a short messages.

   \param[in] destination_number		Destination address. Typically an array.
   \param[in] size_destination_number	Number of characters in destination_number
   	   	   	   	   	   	   	   	   	   	   vector.
   \param[in] message					Short message to be send.
   \param[in] size_message				Number of characters in message vector.

   \return	This function indicates if the task was successful, or if an error
 	 	 	occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_SMS_Send_Messages(uint8_t *destination_number,
									uint8_t size_destination_number,
									uint8_t *message, uint8_t size_message);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Sends a short messages in PDU mode.

   \param[in] length			Length of the actual TP data unit in octets.
   \param[in] message_pdu		Short message to be send in PDU mode.
   \param[in] size_message_pdu	Number of characters in message_pdu vector.

   \return	This function indicates if the task was successful, or if an error
 	 	 	occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_SMS_Send_Messages_PDU(uint8_t length, uint8_t *message_pdu,
        								uint8_t size_message_pdu);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Write messages to select memory storage.

   \param[in] message_storage			Message storage.
   \param[in] destination_number		Destination address.
   \param[in] size_destination_number	Number of characters in destination_number
   	   	   	   	   	   	   	   	   	   	   vector.
   \param[in] message					Short message to be written.
   \param[in] size_message				Number of characters in message vector.
   \param[out] index_in_memory			Pointer to the variable where the index
   	   	   	   	   	   	   	   	   	   	   in memory will be stored.

   \return	This function indicates if the task was successful, or if an error
 	 	 	occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_SMS_Write_Messages_to_Memory(sms_mem_storage_t message_storage,
												uint8_t *destination_number,
												uint8_t size_destination_number,
												uint8_t *message, uint8_t size_message,
												uint8_t *index_in_memory);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Write messages to select memory storage in PDU mode.

   \param[in] message_storage	Message storage.
   \param[in] length			Length of the actual TP data unit in octets.
   \param[in] message			Short message to be written.
   \param[in] size_message		Number of characters in message vector.
   \param[out] index_in_memory	Pointer to the variable where the index in
   	   	   	   	   	   	   	   	   memory will be stored.

   \return	This function indicates if the task was successful, or if an error
 	 	 	occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_SMS_Write_Messages_PDU_to_Memory(sms_mem_storage_t message_storage,
												uint8_t length, uint8_t *message,
												uint8_t size_message,
												uint8_t *index_in_memory);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Send message previously written in select memory storage.

   \param[in] message_storage	Message storage.
   \param[in] index_in_memory	Index in memory where the message is located.

   \return	This function indicates if the task was successful, or if an error
 	 	 	occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_SMS_Send_Message_from_Storage(sms_mem_storage_t message_storage,
												uint8_t index);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_SMS_Status_Report_Received_Flag(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_SMS_Clear_Status_Report_Received_Flag(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_SMS_Status_Report_Received(sms_recv_mem_storeage_t *mem,
		uint8_t *index);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_SMS_Deliver_Received_Flag(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_SMS_Clear_Deliver_Received_Flag(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_SMS_Deliver_Received(sms_recv_mem_storeage_t *mem,
		uint8_t *index);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_SMS_Done_Initialization_Flag(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CM_Q03_SMS_Clear_Done_Initialization_Flag(void);

//////////////////////////End of Functions Description//////////////////////////

/////////////////////////////Include Guard Closing//////////////////////////////
#endif /* CM_Q03_SMS_H_ */
/////////////////////////End of Include Guard Closing///////////////////////////
