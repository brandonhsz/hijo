////////////////////////////Module's Description////////////////////////////////
/*!
 * \file       CM_Q03_FTP.h
 * \brief      This is the CM_Q03 FTP driver made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.\n
 *             **to identify the meaning of a CME error, please refer to the
 *             provided Quectel FTP(S) AT commands manual.\n
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

////////////////////////////////Include Guard///////////////////////////////////
#ifndef CM_Q03_FTP_H_
#define CM_Q03_FTP_H_
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
 	 \brief Enumerator to define the file type.
 */
typedef enum
{
	binary,		/*!< Use binary format */
	ascii		/*!< Use ASCII format */
}file_t;

/*!
 	 \brief Enumerator to define the transfer mode.
 */
typedef enum
{
	active,		/*!< The module will listen on a port for data connection*/
	passive		/*!< The FTP(S) server will listen on a port for data connection */
}transmode_t;

/*!
 	 \brief Enumerator to define the SSL type.
 */
typedef enum
{
	FTP,			/*!< Do not use SSL connection */
	FTPS_implicit,	/*!< Use SSL connection on implicit mode */
	FTPS_explicit	/*!< Use SSL connection on explicit mode */
}sslmode_t;

/*!
 	 \brief Enumerator to define .
 */
typedef enum
{
	server_dispatched_address,	/*!< Do not use SSL connection */
	control_session_address		/*!< Use SSL connection on implicit mode */
}address_t;

/*!
 	 \brief Structure for the FTP configuration.
 */
typedef struct
{
	file_t file_type; 			/*!< The format of transferred data.*/
	transmode_t transmode; 		/*!< Whether the FTP(S) server or client listens on a port
										for data connection */
    uint8_t contexid;			/*!< PDP to be used by the FTP(S) client. The range is 1-3*/
    uint8_t rsptimeout;			/*!< Timeout value, in seconds, for most functions. The range is 20-180 */
    sslmode_t ssltype;			/*!< Whether the module will work as FTP client or FTPS client */
    uint8_t sslctxid; 			/*!< SSL context ID to be used by the FTPS client. The range is 0-5.*/
    address_t data_address;	 	/*!< FTP(S) data connection address selection */
}ftp_config_t;

//////////////////////End of Structures and Enumerators/////////////////////////

//////////////////////////////Functions Description/////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function configures the FTP parameters.

 	 \param[in] ftp_config_struct Structure for the configuration of the FTP.
 	 \param[in] get_default_config Whether the configuration used will be a default
 	 	 	 	 	 	 	 	 	 one or a personal configuration.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_FTP_Configure_Parameters(ftp_config_t ftp_config_struct,
										  	   configuration_select_t get_default_config);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function configures an account for the FTP server.

 	 \param[in] username Pointer to the variable where the user name is stored.
 	 	 	 	 	 	 Typically an array. The maximum size of the username
 	 	 	 	 	 	 is 255 characters.
 	 \param[in] username_size Length of the user name. The range is 1-255.
 	 \param[in] password Pointer to the variable where the password is stored.
 	 	 	 	 	 	 Typically an array. The maximum size of the password
 	 	 	 	 	 	 is 255 characters.
 	 \param[in] password_size Length of the password. The range is 1-255.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_FTP_Configure_Account_Info(uint8_t *username, uint16_t username_size,
												 uint8_t *password, uint16_t password_size);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function is used to login to an FTP server.

 	 \param[in] hostname Pointer to the variable where the host name is stored.
 	 	 	 	 	 	 Typically an array. The maximum size of the hostname
 	 	 	 	 	 	 is 200 characters.
 	 \param[in] hostname_size Length of the host name. The range is 1-200.
 	 \param[in] port The port of the FTP server.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_FTP_Login_to_Server(uint8_t *hostname, uint16_t hostname_size, uint16_t port);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function configures the current directory on the FTP server.

 	 \param[in] pathname Pointer to the variable where the path of the directory
 	 	 	 	 	 	 is stored. Typically an array. The maximum size of the
 	 	 	 	 	 	 pathname is 255 characters.
 	 \param[in] pathname_size Length of the path. The range is 1-255.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_FTP_Configure_Current_Directory(uint8_t *pathname, uint16_t pathname_size);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function gets the path of the current directory on the
 	 	 	 FTP server.

 	 \param[out] pathname Pointer to the variable where the path of the directory
 	 	 	 	 	 	 	 will be stored. Typically an array.
 	 \param[out] pathname_size Pointer to the variable where the length of the
 	 	 	 	 	 	 	 	 directory path will be stored.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_FTP_Get_Current_Directory(uint8_t *pathname, uint16_t *pathname_size);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function uploads a file to the current directory on the FTP
 	 	 	 server. The file must be in the UFS. (See CM_Q03_File).

 	 \param[in] filename Pointer to the variable where the name of the file is.
 	 	 	 	 	 	 The filename is the name the file will have in the FTP
 	 	 	 	 	 	 server. Typically an array.The maximum size of the
 	 	 	 	 	 	 filename is 255 characters.
 	 \param[in] filename_size Length of the name of the file. The range is 1-255.
 	 \param[in] localname Pointer to the variable where the local name of the
 	 	 	 	 	 	 	file is, the localname is the name the file has
 	 	 	 	 	 	 	in the UFS. Typically an array. The maximum size of
							the localname is 56 characters.
 	 \param[in] localname_size Length of the localname. The range is 1-56

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_FTP_Upload_File(uint8_t *filename, uint16_t filename_size,
										uint8_t *localname, uint16_t localname_size);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function creates a file and uploads it to the current directory
 	 	 	 on the FTP server.

 	 \param[in] filename Pointer to the variable where the name of the file is.
 	 	 	 	 	 	 Typically an array.The maximum size of the filename is
 	 	 	 	 	 	 255 characters.
 	 \param[in] filename_size Length of the name of the file. The range is 1-255.
 	 \param[in] payload Pointer to the variable where the content of the file is.
 	 	 	 	 	 	 Typically an array.
 	 \param[in] payload_size Length of the payload.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_FTP_Create_File(uint8_t *filename, uint16_t filename_size,
										uint8_t *payload, uint16_t payload_size);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function downloads a file from the FTP server to the UFS.

 	 \param[in] filename Pointer to the variable where the name of the file is.
 	 	 	 	 	 	 The filename is the name the file has in the FTP server.
 	 	 	 	 	 	 Typically an array.The maximum size of the filename is
 	 	 	 	 	 	 255 characters.
 	 \param[in] filename_size Length of the name of the file. The range is 1-255.
 	 \param[in] localname Pointer to the variable where the local name of the
 	 	 	 	 	 	 	file is, the localname is the name the file will have
 	 	 	 	 	 	 	when downloaded. Typically an array. The maximum size
 	 	 	 	 	 	 	of the localname is 56 characters.
 	 \param[in] localname_size Length of the localname. The range is 1-56

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_FTP_Download_File(uint8_t *filename, uint16_t filename_size,
										uint8_t *localname, uint16_t localname_size);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function reads a file from the FTP server.

	 \warning This function can only read a file of up to 490 characters.

 	 \param[in] filename Pointer to the variable where the name of the file is.
 	 	 	 	 	 	 Typically an array. The maximum size of the filename is
 	 	 	 	 	 	 255 characters.
 	 \param[in] filename_size Length of the name of the file. The range is 1-255.
 	 \param[in] payload Pointer to the variable where the content of the file
 	 	 	 	 	 	 will be stored.
 	 \param[in] size_payload Pointer to the variable where the length of the
 	 	 	 	 	 	 	 content of the file will be stored.

	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
cm_q03_codes_t CM_Q03_FTP_Read_File(uint8_t *filename, uint16_t filename_size,
									uint8_t *payload, uint16_t *size_payload);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function returns the size of a file in the FTP server.

 	 \param[in] filename Pointer to the variable where the name of the file is.
 	 	 	 	 	 	 Typically an array. The maximum size of the filename is
 	 	 	 	 	 	 255 characters.
 	 \param[in] filename_size Length of the name of the file. The range is 1-255.
 	 \param[out] filesize Size of the file in bytes.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_FTP_Get_File_Size(uint8_t *filename, uint16_t filename_size,
										uint16_t *filesize);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function deletes a file in the FTP server.

 	 \warning Any file deleted CANNOT be restored.

 	 \param[in] filename Pointer to the variable where the name of the file is.
 	 	 	 	 	 	 Typically an array. The maximum size of the filename is
 	 	 	 	 	 	 255 characters.
 	 \param[in] filename_size Length of the name of the file. The range is 1-255.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_FTP_Delete_File(uint8_t *filename, uint16_t filename_size);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function creates a folder in the FTP server.

 	 \note If there is a folder with the same name in the FTP server, the
 	 	 	folder will not be created, and the function will return
 	 	 	FTP_Requested_action_not_taken.


 	 \param[in] foldername Pointer to the variable where the name of the folder is.
 	 	 	 	 	 	   Typically an array. The maximum size of the foldername
 	 	 	 	 	 	   is 255 characters.
 	 \param[in] foldername_size Length of the name of the folder. The range is 1-255.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_FTP_Create_Folder(uint8_t *foldername, uint16_t foldername_size);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function deletes a folder in the FTP server.

	 \warning This function also deletes all the files and folders inside the
	 	 	 	 deleted folder.
	 \warning Any file or folder deleted from the FTP server CANNOT be restored.
	 	 	 	 Be careful when testing this function.

 	 \param[in] foldername Pointer to the variable where the name of the folder is.
 	 	 	 	 	 	   Typically an array. The maximum size of the foldername
 	 	 	 	 	 	   is 255 characters.
 	 \param[in] foldername_size Length of the name of the folder. The range is 1-255.

	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_FTP_Delete_Folder(uint8_t *foldername, uint16_t foldername_size);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function gets the modification time of a file in the FTP server.

	 \param[in] filename Pointer to the variable where the name of the file is.
					 	 Typically an array. The maximum size of the filename
					 	 is 255 characters.
 	 \param[in] filename_size Length of the name of the file. The range is 1-255.
 	 \param[out] modifytime Pointer to the variable where the modification time
 	 	 	 	 	 	 	 of the file will be stored. Typically an array.
 	 	 	 	 	 	 	 The format is YYYYMMDDHHMMSS
 	 \param[out] modifytime_size Pointer to the variable where the modification
 	 	 	 	 	 	 	 	 time length will be stored.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_FTP_Get_File_Modification_Time(uint8_t *filename, uint16_t filename_size,
													 uint8_t *modifytime, uint16_t *modifytime_size);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function renames a file or a folder in the FTP server.

 	 \param[in] oldname Pointer to the variable where the old name of the file or
 	 	 	 	 	 	 the folder is stored. Typically an array. The maximum
 	 	 	 	 	 	 size of the oldname is 255 characters.
 	 \param[in] oldname_size Length of the old name of the file or the folder.
 	 	 	 	 	 	 	 The range is 1-255.
 	 \param[in] newname Pointer to the variable where the new name of the file or
 	 	 	 	 	 	 the folder is stored. Typically an array. The maximum
 	 	 	 	 	 	 size of the newname is 255 characters.
 	 \param[in] newname_size Length of the new name of the file or the folder.
 	 	 	 	 	 	 	 The range is 1-255.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_FTP_Rename_File_or_Folder(uint8_t *oldname, uint16_t oldname_size,
												uint8_t *newname, uint16_t newname_size);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function returns the length of the transferred data to the FTP
 	 	 	 server. (Either as an output or an input).

 	 \param[out] transfer_length Pointer to the variable where the size,
 	 	 	 	 	 	 	 	 in bytes, of the transferred data.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_FTP_Get_Lenght_Transferred_Data(uint16_t *transfer_length);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function returns the current status of the FTP server.

 	 \return The current status of the FTP server.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_FTP_Get_Server_Status(void);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function logs out from the FTP server.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_FTP_Close_Server(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_FTP_Get_Closed_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Clear_Closed_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Get_Closed_Status(cm_q03_codes_t *status, cm_q03_codes_t *protocol_status);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_FTP_Get_Configured_Directory_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Clear_Configured_Directory_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Get_Configured_Directory_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_FTP_Get_Deleted_File_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Clear_Deleted_File_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Get_Deleted_File_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_FTP_Get_Download_File_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Clear_Download_File_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Get_Download_File_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status, uint16_t *transfer_len);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_FTP_Get_Transfer_Length_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Clear_Transfer_Length_Status_Flag(void);


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Get_Transfer_Length_Status(cm_q03_codes_t *status,
		uint16_t *transferlength);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_FTP_Get_Modify_Time_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Clear_Modify_Time_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void _CM_Q03_FTP_Get_Modify_Time_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status, uint8_t* mod_time,
		uint16_t *mod_time_size);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_FTP_Get_Create_Folder_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Clear_Create_Folder_Status_Flag(void);


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Get_Create_Folder_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_FTP_Get_Login_to_Server_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Clear_Login_to_Server_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Get_Login_to_Server_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_FTP_Get_Upload_File_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Clear_Upload_File_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Get_Upload_File_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status, uint16_t *transferlen);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_FTP_Get_Current_Directory_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Clear_Current_Directory_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Get_Current_Directory_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status, uint8_t *curr_path,
		uint16_t *curr_path_size);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_FTP_Get_Rename_File_or_Folder_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Clear_Rename_File_or_Folder_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Get_Rename_File_or_Folder_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_FTP_Get_Delete_Folder_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Clear_Delete_Folder_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Get_Delete_Folder_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_FTP_Get_File_Size_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Clear_File_Size_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Get_File_Size_Status(cm_q03_codes_t *status,
		cm_q03_codes_t *protocol_status, uint16_t *file_size);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_FTP_Get_Server_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Clear_Server_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_FTP_Retreive_Server_Status(cm_q03_codes_t *status, uint16_t *ftp_stat);

//////////////////////////End of Functions Description//////////////////////////

/////////////////////////////Include Guard Closing//////////////////////////////
#endif /* CM_Q03_FTP_H_ */
/////////////////////////End of Include Guard Closing///////////////////////////
