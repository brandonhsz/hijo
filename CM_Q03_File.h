////////////////////////////Module's Description////////////////////////////////
/*!
 * \file       CM_Q03_File.h
 * \brief      This is the CM_Q03 File driver made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.\n
 *             **to identify the meaning of a CME error, please refer to the
 *             provided Quectel FILE AT commands manual.\n
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

////////////////////////////////Include Guard///////////////////////////////////

#ifndef CM_Q03_FILE_H_
#define CM_Q03_FILE_H_

////////////////////////////////Includes Area///////////////////////////////////
#include <CM_Q03.h>
#include "init_config_functions.h"
#include "fsl_uart.h"
#include <stdio.h>
#include <stdlib.h>

#include "stdint.h"
#include "stdbool.h"
#include "fsl_gpio.h"
///////////////////////////End of Includes Area/////////////////////////////////

//////////////////////////Structures and Enumerators////////////////////////////
/*!
 	 \brief Enumerator to define the mode in which the file will be opened
 */
typedef enum
{
	open_or_create,			/*!< If the file doesn't exists, it will be created.
	 	 	 	 	 	 	 	 If the file exits, it will be opened.
	 	 	 	 	 	 	 	 The file can be read and written*/
	overwrite_or_create,	/*!< If the file doesn't exists, it will be created.
	 	 	 	 	 	 	 	 If the file exists, it will be overwritten.
	 	 	 	 	 	 	 	 The file can be read and written*/
	open_read_only			/*!< If the file doesn't exists, an error will be
								 returned.
	 	 	 	 	 	 	 	 If the file exists, it will be opened.
	 	 	 	 	 	 	 	 The file can only be read (If it is opened).*/
}open_file_mode_t;

/*!
 	 \brief Enumerator the set the position from which the file pointer
 	 	 	 will be set.
 */
typedef enum
{
	beginning_of_the_file,	/*!< The pointer will move from the beginning
	 	 	 	 	 	 	 	 of the file forward*/
	current_position,		/*!< The pointer will move from its current
	 	 	 	 	 	 	 	 position forward*/
	end_of_the_file			/*!< The pointer will move from the end of the
	 	 	 	 	 	 	 	 file backwards*/
}start_position_t;
//////////////////////End of Structures and Enumerators/////////////////////////

//////////////////////////////Functions Description/////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function returns the space used in the UFS and the number of
 	 	 	 files stored.

 	 \param[out] size_used Pointer to the variable where the space used, in bytes,
 	 	 	 	 	 	 	 will be stored.
 	 \param[out] file_number Pointer to the variable where the number of files
 	 	 	 	 	 	 	 in the UFS will be stored.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_File_Get_Space_Used(uint32_t *size_used, uint32_t *file_number);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function returns the free space and the total space in the UFS.

 	 \param[out] freesize Pointer to the variable where the free space size will be
 	 	 	 	 	 	 	 stored, in bytes.
 	 \param[out] total_size Pointer to the variable where the total space size will
 	 	 	 	 	 	 	 be stored, in bytes.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_File_Get_Space_Information(uint32_t *freesize, uint32_t *total_size);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function indicates if a file exits in the UFS, and the size of
 	 	 	 the file (If it exists)

 	 \param[in] filename Pointer to the variable where the name of the file is.
 	 	 	 	 	 	 Typically an array. The maximum length is 80 characters.
 	 \param[in] filename_size Length of the name of the file. The range is 1-80.
 	 \param[out] file_size Pointer to the variable where the length of the file,
 	 	 	 	 	 	 	 in bytes, will be stored.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_File_List_File(uint8_t *filename, uint8_t filename_size,
										uint32_t *file_size);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function stores a file in the UFS. If there is a file with the
 	 	 	 same name as the file to be stored, an error will be reported.

 	 \param[in] filename Pointer to the variable where the name of the file
 	 	 	 	 	 	 is stored. Typically an array. The maximum length
 	  	  	  	  	  	  is 80 characters.
 	 \param[in] filename_size Length of the name of the file. The range is 1-80.
 	 \param[in] buffer_write Pointer to the variable where the payload to be
 	 	 	 	 	 	 	 stored in the file is stored. Typically an array.
 	 \param[in] buffer_write_size Length of the payload.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_File_Upload_File(uint8_t *filename, uint8_t filename_size,
										uint8_t *buffer_write,
										uint16_t buffer_write_size);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function downloads a file from the UFS to a buffer.

 	 \param[in] filename Pointer to the variable where the name of the file to
 	 	 	 	 	 	 be downloaded is. Typically an array. The maximum length
 	  	  	  	  	  	  is 80 characters.
 	 \param[in] filename_size Length of the name of the file. The range is 1-80.
 	 \param[in] buffer_read Pointer to the variable where the payload of the
 	 	 	 	 	 	 	 file will be stored. Typically an array.
 	 \param[in] buffer_read_size Pointer to the variable where the length of the
 	 	 	 	 	 	 	 	 payload will be stored.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_File_Download_File(uint8_t  *filename, uint8_t filename_size,
											uint8_t *buffer_read,
											uint16_t *buffer_read_size);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function opens a file if it exists, or creates it if it doesn't
 	 	 	 exists (In read_only_mode, the function returns an error message if
 	 	 	 the file doesn't exists).

 	 \param[in] filename Pointer to the variable where the name of the file to
 	  	  	  	  	  	  be opened is. Typically an array. The maximum length
 	  	  	  	  	  	  is 80 characters.
 	 \param[in] filename_size Length of the name of the file to be opened.
 	 	 	 	 	 	 	 	 The range is 1-80.
 	 \param[in] mode Mode in which the file will be opened or created.
 	 \param[out] filehandle Returns the ID of the opened file. This ID is
 	 	 	 	 	 	 	 necessary to read, write, change the position
 	 	 	 	 	 	 	 and close the file.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_File_Open_File(uint8_t  *filename, uint8_t filename_size,
										open_file_mode_t mode, uint32_t *filehandle);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function sets the file pointer to a specific position on the file.
 	 	 	 After a file is opened, the initial position of the file pointer is
 	 	 	 0 (Beginning of the file).

	 \note The file must be opened before using this function.

 	 \param[in] filehandle ID of the file of which the file pointer will be set.
 	 \param[in] offset Number of bytes that the file pointer is going to move.
 	 \param[in] start_position The position from which the pointer is going to
 	 	 	 	 	 	 	 	 be moved. If the position chosen is end_of_the_file,
 	 	 	 	 	 	 	 	 the pointer is going to move from the end backwards.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_File_Set_Pointer_Position(uint32_t filehandle, uint16_t offset,
												start_position_t start_position);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function returns the current position of the pointer.

 	 \note The file must be opened before using this function.

 	 \param[in] filehandle  ID of the file of which the file pointer will be queried.
 	 \param[in] position Pointer to the variable in which the position will be
 	 	 	 	 	 	 stored.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
cm_q03_codes_t CM_Q03_File_Get_Pointer_Position(uint32_t filehandle,
												uint16_t *position);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function reads data from a file starting from the current pointer
 	 	 	 position.

 	 \note The file must be opened before using this function.

 	 \param[in] filehandle ID of the file to be read from.
 	 \param[in] length_read Number of bytes to be read.
 	 \param[out] buffer_read Pointer to the variable where the read data will be
 	 	 	 	 	 	 	 stored. Typically an array. This function can read a
 	 	 	 	 	 	 	 maximum of 490 characters.
 	 \param[out] buffer_read_size Pointer to the variable where the length of the
 	 	 	 	 	 	 	 	 	 data read will be stored.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_File_Read_File(uint32_t filehandle, uint16_t length_read,
										uint8_t *buffer_read,
										uint16_t *buffer_read_size);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function writes data into a file, starting from the current pointer
 	 	 	 position.

 	 \note The file must be opened before using this function.
 	 \warning If the current position of the pointer is before the end of the file
 	 	 	 	 the data from the current position of the file pointer on will be
 	 	 	 	 overwritten.

 	 \param[in] filehandle ID of the file to be written into.
 	 \param[in] buffer_write Pointer to the variable where the data to be written
 	 	 	 	 	 	 	 is. Typically an array.
 	 \param[in] buffer_write_size Length of the data to be written.

 	 \note If the data written exceeds the length of the file, the file's length
 	 	 	 will increase the number of bytes exceeded.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_File_Write_File(uint32_t filehandle, uint8_t *buffer_write,
										uint16_t buffer_write_size);

////////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function closes an opened file.

 	 \param[in] filehandle ID of the file to be closed.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_File_Close_File(uint32_t filehandle);

////////////////////////////////////////////////////////////////////////////////
/*!
	\brief This function deletes a file from the UFS.

	\warning Be careful when testing this function, a deleted file CANNOT be
	restored.

	\param[in] filename Pointer to the variable where the name of the file is
						stored. Typically an array. The maximum length is 80
						characters.
	\param[in] filename_size Length of the name of the file. The range is 1-80.

	\return This function indicates if the task was successful, or if an error
 	 	 	occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_File_Delete_File(uint8_t *filename, uint16_t filename_size);

//////////////////////////////Functions Description/////////////////////////////

#endif /* CM_Q03_FILE_H_ */
/////////////////////////End of Include Guard Closing///////////////////////////
