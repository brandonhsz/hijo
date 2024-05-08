////////////////////////////Module's Description////////////////////////////////
/*!
 * \file         Watchdog.h
 * \brief        This is the Watchdog module driver developed by S4IoT
 * \details
 * \version      1.0.
 * \date         6/06/2018
 * \author       S4IoT
 * \copyright    (c) 2018, SOLUTIONS 4 IOT. All rights reserved.
 */
// reviewed by   Gabriel Santamaria
/////////////////////////End of Module Description//////////////////////////////
////////////////////////////////Include Guard///////////////////////////////////
#ifndef WATCHDOG_H_
#define WATCHDOG_H_
/////////////////////////////End of Include Guard///////////////////////////////

////////////////////////////////Includes Area///////////////////////////////////
#include "stdint.h"
///////////////////////////End of Includes Area/////////////////////////////////

//////////////////////////Structures and Enumerators////////////////////////////
/**this enumerator is used to define the time interval to feed the Watchdog*/
typedef enum {
    DogTime_32ms,    /*!< 0.032s */
    DogTime_256ms,   /*!< 0.256s */
    DogTime_1024ms,  /*!< 1.024s */
    DogTime_8192ms,  /*!< 8.192s */
    DogTime_65536ms, /*!< 1.092min */
    DogTime_262144ms /*!< 4.369min */
} DogTime_t;

/**this enumerator is used to define the time of operation to perform with the
   Watchdog */
typedef enum {
    normalFood, /*!< normal dog s refresh, i.e. module working correctly and
                     doesn't need reboot */
    resetFood /*!< manual system reboot can be done by feeding this to the
                   dog */
} DogFood_t;
//////////////////////End of Structures and Enumerators/////////////////////////

//////////////////////////////Functions Description/////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief This function is used to load the default configuration to the
 * 		  Watchdog module (i.e. dog refreshing interval of 4.369 minutes)
 *
 *	\note  This function DOES NOT start the Watchdog timer
 *
 * \param[in] void
 * \return void
 */
////////////////////////////////////////////////////////////////////////////////
void Watchdog_initModuleConfiguration();

////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief This function is used to configure another time interval for feeding
 * 		  the dog.
 *
 * \note  Please use the DogTime_t enumerator values declared in this
 * 		  same header file in order to assure proper configuration.
 *
 * \param[in] TimeSelection The selected new time interval for feeding the dog
 * \return void
 */
////////////////////////////////////////////////////////////////////////////////
void Watchdog_setDifferentTimer(DogTime_t TimeSelection);

////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief This function is used to enable the Watchdog module.
 *
 * \note  In order to use correctly this module, once this function has been
 * 		  called, the Watchdog_feedTheDog function with normalFood value as
 * 	      input parameter (from the DogFood_t enumerator) must be used within
 * 	   	  the defined time interval (whether the default one or another
 * 		  configured by the user), in order to avoid system reboot.
 *
 * \param[in] void
 * \return void
 */
////////////////////////////////////////////////////////////////////////////////
void Watchdog_dogAlert();

////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief This function deactivates the Watchdog module.
 *
 * \note  As calling this function deactivates the Watchdog module, there will
 * 		  be no need for continued Dog feeding in order to avoid system reboot;
 * 		  so the board will not be restarted by Watchdog operation.
 *
 * \param[in] void
 * \return void
 */
////////////////////////////////////////////////////////////////////////////////
void Watchdog_SleepTheDog();

////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief This function refreshes the Watchdog module. Normal use (in order to
 * 		  avoid a system reboot) would be by using as input parameter the
 * 		  normalFood value from the DogFood_t enumerator. Anytime a system reset
 * 		  can be achieved by using as input parameter the restartFood value.
 *
 * \param[in] typeOfFood the desired refresh operation for the Watchdog module
 * \return void
 */
////////////////////////////////////////////////////////////////////////////////
void Watchdog_feedTheDog(uint8_t typeOfFood);

////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief This function checks if the system was rebooted by the Watchdog
 * 		  previously, and returns as a boolean true in the Watchdog_test passed-
 * 	      by-reference-input-parameter if indeed the dog restarted the board.
 * 		  As an optional feature, the user can pass a reference to a function
 * 		  desired to be called if a system reboot was caused by the Watchdog.
 *
 *\note   In case that calling a function when a system reboot is caused by
 *        the Watchdog is not desired, the user can pass NULL to the function
 *        input parameter.
 *
 * \param[in] Watchdog_test Passed by reference variable where a true will be
 * 			  stored if, indeed the Watchdog rebooted the system.
 * \param[in] function      Optional input parameter; this parameter receives
 * 							ONLY the name of the function desired to be called
 * 						    in case a system reboot was caused by the Watchdog.
 * \return void
 */
////////////////////////////////////////////////////////////////////////////////
void Watchdog_checkIfReset(uint8_t * Watchdog_test, void * function);
//////////////////////////End of Functions Description//////////////////////////

/////////////////////////////Include Guard Closing//////////////////////////////
#endif /* WATCHDOG_H_ */
/////////////////////////End of Include Guard Closing///////////////////////////
