////////////////////////////Module's Description////////////////////////////////
/*!
 * \file       CM_Q03_General.h
 * \brief      This is the CM_Q03 General functions made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.\n
 *             **to identify the meaning of a CME error, please refer to the
 *             provided Quectel AT commands manual.\n
 * \version    1.0.1
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

////////////////////////////////Include Guard///////////////////////////////////

#ifndef CM_Q03_GENERAL_H_
#define CM_Q03_GENERAL_H_

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
 	 \brief Enumerator to define the order of the network
 	 	 	 scan sequence.
 */
typedef enum
{
	GSM_CatM1_CatNB1 = 123,	/*!< First GSM, second LTE Cat M1, third LTE Cat NB1*/
	GSM_CatNB1_CatM1 = 132,	/*!< First GSM, second LTE Cat NB1, third LTE Cat M1*/
	CatM1_GSM_CatNB1 = 213,	/*!< First LTE Cat M1, second GSM, third LTE Cat NB1*/
	CatM1_CatNB1_GSM = 231,	/*!< First LTE Cat M1, second LTE Cat NB1, third GSM*/
	CatNB1_GSM_CatM1 = 312,	/*!< First LTE Cat NB1, second GSM, third LTE Cat M1*/
	CatNB1_CatM1_GSM = 321	/*!< First LTE Cat NB1, second LTE Cat M1, third GSM*/
}network_scan_sequency_t;

/*!
 	 \brief Enumerator to define the networks to be searched.
 */
typedef enum
{
	scan_automatic,		/*!< The search will be automatic*/
	scan_GSM_only,		/*!< The search will be only for GSM*/
	scan_LTE_only = 3	/*!< The search will be only for LTE*/
}network_scan_mode_t;

/*!
 	 \brief Enumerator to define the network category to
 	 	 	 be searched under LTE
 */
typedef enum
{
	scan_CatM1_for_LTE,				/*!< Only LTE Cat M1 will be searched*/
	scan_CatNB1_for_LTE,			/*!< Only LTE Cat NB1 will be searched*/
	scan_CatM1_and_CatNB1_for_LTE	/*!< Both LTE Cat M1 and Cat NB1 will be searched*/
}IoT_option_mode_t;

/*!
 	 \brief Enumerator to enable or disable the roam service.
 */
typedef enum
{
	disable_roam_service = 1,		/*!< Disables roam service*/
	enable_roam_service,			/*!< Enables roam service*/
	automatic_roam_service = 255	/*!< Roam service is automatic*/
}roam_service_t;

/*!
 	 \brief Enumerator to define the module functionality mode.
 */
typedef enum
{
	Minimum_functionality,		/*!< Minimum functionality*/
	Full_functionality,			/*!< Full functionality (Default)*/
	Airplane_functionality = 4	/*!< Disable both transmitting and receiving RF signals*/
}module_functionality_t;

/*!
 	 \brief Enumerator to define the module access technology selected.
 */
typedef enum
{
	GSM_technology, 		/*!< GSM */
	Cat_M1_technology = 8,	/*!< LTE Cat M1 */
	NB_IoT_technology = 9	/*!< LTE Cat NB1 */
}access_technology_t;

typedef enum
{
	ME_awake,
	powered_down
}power_down_status_t;

typedef enum
{
	ME_not_ready,
	ME_ready
}ready_status_t;


/*!
     \brief Enumerator to configure bands per country
 */
typedef enum
{
    mexico,
    usa,
    colombia,
    linksfield,
    EG915LA
}bands_struct_t;

/*!
 	 \brief Structure to configure the network.
 */
typedef struct{
	network_scan_sequency_t nwscanseq;	/*!< Network scan sequence*/
	network_scan_mode_t nwscanmode;		/*!< Network to be scanned */
    IoT_option_mode_t iotopmode;		/*!< LTE network category to be scanned*/
    roam_service_t roamservice;			/*!< Enable/Disable roam service*/
    uint32_t plmn;                      /*!< PEKO To force attach to PLMN set to zero for automatic mode*/
    bands_struct_t frequency_bands;     /*!< Configure frequency bands to be scanned*/
}general_network_paramenters_struct_t;

/*!
 	 \brief Structure to use as clock.
 */
typedef struct
{
	uint8_t string[20];
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minutes;
	uint8_t seconds;
	int8_t time_zone;		/*!< Indicates the differences,
								 expressed in quarters of an hour,
								 between the local time and GMT*/
}clock_struct_t;



//////////////////////End of Structures and Enumerators/////////////////////////

//////////////////////////////Functions Description/////////////////////////////

cm_q03_codes_t CM_Q03_Control_USB(bool state);

///////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function configures the radio access technology (RAT). Allowing the
 	 	 	 user to choose between a default configuration or a user-defined one.

 	 \param[in] parameters_network Configuration values for the network.
 	 	 	 	 See general_network_parameters_struct_t.
 	 \param[in] default_parameters Whether the configuration will be the default one,
 	 	 	 	 or the user-defined one.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.

 */


///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_Synchronize_Local_Time(void);

///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_Automatic_Time_Zone_Update(void);//nueva sebas

///////////////////////////////////////////////////////////////////////////////
/*!
   \brief This function update the time zone of the RTC, used for Modem EG915

   \return This function indicates if the task was successful, or if an error
        occurred.
 */
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_Time_Zone_Reporting(void);//nueva sebas

///////////////////////////////////////////////////////////////////////////////
/*!
     \brief This function controls the time zone reporting of changed event.only for eg915


     \return This function indicates if the task was successful, or if an error
             occurred.
 */
///////////////////////////////////////////////////////////////////////////////

cm_q03_codes_t CM_Q03_General_Set_PLMN(general_network_paramenters_struct_t parameters_network,
                                       configuration_select_t default_parameters);
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_General_Config_Network(general_network_paramenters_struct_t parameters_network,
										configuration_select_t default_parameters);

///////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function returns the International Mobile Equipment Identity (IMEI).

 	 \param[out] imei Pointer to the variable where the IMEI will be stored.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_Get_IMEI(uint8_t *imei);

///////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function checks if the device is registered in a network.

 	 \return Returns the connection status.
 */
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_General_Registration_Status(void);

///////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function returns the EPS network registration status.

 	 \return Returns the connection status.
 */
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_General_EPS_Registration_Status(void);

///////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function returns the GPRS network registration status.

 	 \return Returns the connection status.
 */
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_General_GPRS_Registration_Status(void);

///////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function returns the SIM status.

 	 \return Returns the SIM PIN status
 */
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_General_PIN_status(void);


///////////////////////////////////////////////////////////////////////////////
/*!
	\brief This function allows the user to enter the PIN for the SIM.

	\param[in] pin PIN to be entered. Must be a 4-digit number and the digits
					must be in ASCII format.

	\return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_General_enter_PIN(uint8_t* pin);

///////////////////////////////////////////////////////////////////////////////
/*!
	\brief This function allows the user to change the PIN of the SIM.

	\param[in] puk	PUK of the SIM. Must be an 8-digit number and the digits
					must be in ASCII format.
	\param[in] new_pin New PIN for the SIM. Must be a 4-digit number and the
						digits must be in ASCII format.

	\return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_General_change_PIN(uint8_t* puk, uint8_t* new_pin);

///////////////////////////////////////////////////////////////////////////////
/*!
	\brief This function allows the user to configure the functionality level.

	\param[in] functionality Functionality level of the module.

	\return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_General_Set_Functionality(module_functionality_t functionality);


///////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function returns the access technology selected.

	 \param[out] act Pointer to the variable where the access technology
				will be stored.

 	 \return Returns the connection status.
 */
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_General_Access_Technology(access_technology_t *act);

///////////////////////////////////////////////////////////////////////////////
/*!
 	 \brief This function returns the real time clock of the module.

	 \param[out] act Pointer to the variable where the RTC will be stored.

 	 \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_General_Get_Clock(clock_struct_t *clock);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_General_Get_ME_Functionality_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_General_Clear_ME_Functionality_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_General_Get_ME_Functionality(module_functionality_t *func);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_General_Get_Pin_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_General_Clear_Pin_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_General_Get_Pin_Status(cm_q03_codes_t *pin_status);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_General_Get_Registration_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_General_Clear_Registration_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_General_Get_Registration_Status(cm_q03_codes_t *reg_stat);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_General_Get_Powered_Down_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_General_Clear_Powered_Down_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_General_Powered_Down_Status(power_down_status_t *pow_dwn);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
urc_received_status_t CM_Q03_General_Get_Ready_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_General_Clear_Ready_Status_Flag(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CM_Q03_General_Ready_Status(ready_status_t *rdy);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


cm_q03_codes_t CM_Q03_Signal_Quality_Report(uint8_t *rssi, uint8_t *ber);


///////////////////////////////////////////////////////////////////////////////
/*!
    \brief This function returns the status of signal strenght and signal quality

    \param[in] void.

    \return This function indicates if the task was successful, or if an error
             occurred.
 */
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_4G_Signal_Quality_Report(uint8_t *sysmode, uint16_t *sysmode_size, uint8_t *rssi,
        uint16_t *rssi_size, uint8_t *rsrp, uint16_t *rsrp_size, uint8_t *sinr, uint16_t *sinr_size,
        uint8_t *rsrq, uint16_t *rsrq_size);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!
    \brief This function returns the operator PLMN which the SIM card belong

    \param[in] void.

    \return This function indicates if the task was successful, or if an error
             occurred.
 */
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_Get_SIM_PLMN(uint32_t *plmn);

///////////////////////////////////////////////////////////////////////////////
/*!
    \brief This function returns the unique identification number of the SIM card
    named ICCID

    \param[in] void.

    \return This function indicates if the task was successful, or if an error
             occurred.
 */
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_Get_SIM_ICCID(uint8_t *iccid, uint16_t *iccid_size);

///////////////////////////////////////////////////////////////////////////////
/*!
    \brief This function returns parameters related to the network such as PLMN,
    Frequency band, EARFCN (channel) and RAT (Radio Access Network)

    \param[in] void.

    \return This function indicates if the task was successful, or if an error
             occurred.
 */
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_GET_NW_INFO(uint8_t *rat, uint16_t *rat_size, uint8_t *plmn, uint8_t *band,
                              uint16_t *band_size, uint8_t *channel, uint16_t *channel_size);

///////////////////////////////////////////////////////////////////////////////
/*!
    \brief This function makes a ping to the defined server and returns the Ping TTL (time to live)

    \param[in] void.

    \return This function indicates if the task was successful, or if an error
             occurred.
 */
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_Ping(uint8_t *server, uint16_t *ttl);

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/*!
    \brief This function configures CEREG=2 in order to get the LAC and CID
    (Location Area Code), (Cell ID) with CM_Q03_General_Get_LAC_CID(void)

    \param[in] void.

    \return This function indicates if the task was successful, or if an error
             occurred.
 */
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_General_Config_CEREG(void);

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/*!
    \brief This function gets the LAC and CID
    (Location Area Code), (Cell ID) with CM_Q03_General_Get_LAC_CID(void)
    This function requires to call CM_Q03_General_Config_CEREG(void) function first

    \param[in] void.

    \return This function indicates if the task was successful, or if an error
             occurred.
 */
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_General_Get_LAC_CID(uint8_t *lac, uint16_t *lac_size, uint8_t *cid, uint16_t *cid_size);

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/*!
    \brief This function gets the LAC and CID
    (Location Area Code), (Cell ID) with CM_Q03_General_Get_LAC_CID(void)
    This function requires to call CM_Q03_General_Config_CEREG(void) function first

    \param[in] void.

    \return This function indicates if the task was successful, or if an error
             occurred.
 */
///////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_EG915_Configure_Bands(bands_struct_t seleccion);

///////////////////////////////////////////////////////////////////////////////
//////////////////////////End of Functions Description//////////////////////////


/////////////////////////////Include Guard Closing//////////////////////////////
#endif /* CM_Q03_GENERAL_H_ */
/////////////////////////End of Include Guard Closing///////////////////////////
