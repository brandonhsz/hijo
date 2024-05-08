////////////////////////////Module's Description////////////////////////////////
/*!
 * \file       CM_Q03_GNSS.h
 * \brief      This is the CM_Q03 GNSS driver made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.\n
 *             **to identify the meaning of a CME error, please refer to the
 *             provided Quectel GNSS AT commands manual.\n
 * \version    1.1.1
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

////////////////////////////////Include Guard///////////////////////////////////

#ifndef CM_Q03_GNSS_H_
#define CM_Q03_GNSS_H_

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
 	 \brief Enumerator to acquire a NMEA sentence
 */
typedef enum{
    GGA,   /*!< Global positioning system fix data.*/
    RMC,   /*!< Recommended minimum data*/
    GSV,   /*!< Detailed satellite data*/
    GSA,   /*!< Overall satellite data*/
    VTG,   /*!< Vector track and speed over the ground*/
}NMEA_sentences_types_t;

/*!
 	 \brief Enumerator to set the type of the GPS
 	 	 	 NMEA output sentences
 */
typedef enum
{
	gps_disable_sentences,				/*!< Disable the GPS NMEA sentences*/
	gps_GGA,							/*!< Global positioning system fix data.*/
	gps_RMC,							/*!< Recommended minimum data*/
	gps_GSV = 4,						/*!< Detailed satellite data*/
	gps_GSA = 8,						/*!< Overall satellite data*/
	gps_VTG = 16,						/*!< Vector track and speed over the ground*/
	gps_all_sentences_outputted = 31	/*!< All sentences are outputted*/
}GPS_sentences_types_t;

/*!
 	 \brief Enumerator to set the type of the GLONASS
 	 	 	 NMEA output sentences
 */
typedef enum
{
	glonass_disable_sentences,	/*!< Disable the GLONASS NMEA sentences*/
	glonass_GSV,				/*!< Detailed satellite data*/
	glonass_GSA,				/*!< Overall satellite data*/
}GLONASS_sentences_types_t;

/*!
 	 \brief Enumerator to set the type of the Galileo
 	 	 	 NMEA output sentences
 */
typedef enum
{
	galileo_disable_sentences,	/*!< Disables the Galileo NMEA sentences*/
	galileo_GSV					/*!< Detailed satellite data*/
}Galileo_sentences_types_t;

/*!
 	 \brief Enumerator to set the type of BeiDou
 	 NMEA output sentence
 */
typedef enum
{
	beidou_disable_sentences,	/*!< Disables the BeiDou NMEA sentences*/
	beidou_GSA,					/*!< Overall satellite data*/
	beidou_GSV					/*!< Detailed satellite data*/
}BeiDou_sentences_types_t;

typedef enum
{
    sentence_qualcomm_standards,
    sentence_nmea_0183

}NMEAfmt_config_t;

typedef enum
{
    gnss_high_priority,
    lte_high_priority

}GNSS_priority_t ;



/*!
 	 \brief Enumerator to set the state of the GNSS constellations.
 	 	 	 NOTE: GPS is always on
 */
typedef enum
{
    gps_on_glonass_on_beidou_off_galileo_off_qzss_off=1,
    gps_on_glonass_off_beidou_on_galileo_off_qzss_off,
    gps_on_glonass_off_beidou_off_galileo_on_qzss_off,
    gps_on_glonass_off_beidou_off_galileo_off_qzss_on,
    automatic_based_on_mcc_camped_network,
}supported_gnss_constelations_t;


/*!
 	 \brief Enumerator to set the GNSS working mode
 */
typedef enum
{
	stand_alone = 1	/*!< The position is asked directly to the satellite constellation*/
}GNSS_operation_modes_t;

/*!
 	 \brief Enumerator to  define the maximum positioning time
 	 	 	 for the GNSS.
 */


/*!
 	 \brief Enumerator to define the position accuracy.
 */
typedef enum
{
	pos_accuracy_low=1,
	pos_accuracy_medium,
	pos_accuracy_high,
}max_positioning_accuracy_t;

/*!
 	 \brief Enumerator to define the shape of the geofence.
 */
typedef enum
{
	geofence_circularity_center_radius,		/*!< Circle with a center and a radius*/
	geofence_circularity_center_onepoint,	/*!< Circle with a center and a point
													in the circle*/
	geofence_triangle,						/*!< Triangle*/
	geofence_quadrangle						/*!< Quadrangle*/
}geofence_shape_t;

/*!
 	 \brief Enumerator to define the state of the NMEA sentences.
 */
typedef enum
{
	nmea_src_disable,	/*!< NMEA sentences disabled*/
	nmea_src_enable		/*!< NMEA sentences enabled*/
}nmea_src_t;

/*!
 	 \brief Enumerator to define the state of the extended
 	 	 	 GSV information.
 */
typedef enum
{
	gsvext_disable,	/*!< Extended GSV disabled*/
	gsvext_enable	/*!< Extended GSV enabled*/
}gsvext_t;

/*!
 	 \brief Enumerator to set the state of the GNSS
 	 	 	 automatic run.
 */
typedef enum
{
	auto_disable,	/*!< Automatic run disabled*/
	auto_enable		/*!< Automatic run enabled*/
}auto_gps_t;

/*!
 	 \brief Enumerator to delete the assistance data.
 */
typedef enum
{
	delete_all_assistance_except_gpsOneXTRA,	/*!< Delete all assistance data.
	 	 	 	 	 	 	 	 	 	 	 	 	 except for gpsOneXTRA.*/
	do_not_delete_any_data,						/*!< Do not delete any data*/
	delete_some_related_data,					/*!< Delete some related data*/
	delete_gpsOneXTRA_assistance_data			/*!< Delete the gpsOneXTRA
	 	 	 	 	 	 	 	 	 	 	 	 	 assistance data.*/
}delete_assistance_t;

/*!
 	 \brief Enumerator to define the location format.

 	 \note d stands for degrees\n
 	 	 	m stands for minutes.
 */
typedef enum
{
	degrees_minutes,			/*!< Format for latitude:
	 	 	 	 	 	 	 	 	 ddmm.mmmm N/S\n
	 	 	 	 	 	 	 	 	 Format for longitude:
	 	 	 	 	 	 	 	 	 dddmm.mmmm E/W*/
	degrees_minutes_precision,	/*!< Format for latitude:
	 	 	 	 	 	 	 	 	 ddmm.mmmmmm N/S\n
	 	 	 	 	 	 	 	 	 Format for longitude:
	 	 	 	 	 	 	 	 	 dddmm.mmmmmm E/W*/
	degrees						/*!< Format for latitude:
	 	 	 	 	 	 	 	 	 (-)dd.dddd\n
	 	 	 	 	 	 	 	 	 Format for longitude:
	 	 	 	 	 	 	 	 	 (-)ddd.dddd.*/
}location_format_t;

/*!
 	 \brief Enumerator to define the time of the gpsOneXTRA.
 */
typedef enum
{
	GPS_time,	/*!< Time will be the GPS time*/
	UTC_time	/*!< Time will be the UTC time*/
}utc_time_t;

/*!
 	 \brief Enumerator to define whether the GPS subsystem will be
 	 	 	 forced or allowed to accept the time injected.
 */
typedef enum
{
	allow_acceptance,	/*!< Allows the GPS to accept the time injected*/
	force_acceptance	/*!< Forces the GPS to accept the time injected*/
}force_t;

/*!
 	 \brief Structure to configure the GNSS.
 */
typedef struct
{
	nmea_src_t nmea_src;   						/*!< Used to enable/disable NMEA sentences acquisition */
    GPS_sentences_types_t gps_type;   			/*!< Used to define the type of GPS NMEA sentences to be delivered* */
    GLONASS_sentences_types_t glonass_type; 	/*!< Used to define the type of GLONASS NMEA sentences to be delivered */
    Galileo_sentences_types_t galileo_type; 	/*!< Used to define the type of Galileo NMEA sentences to be delivered */
    BeiDou_sentences_types_t beidou_type;  		/*!< Used to define the type of BeiDou NMEA sentences to be delivered */
    NMEAfmt_config_t nmeafmt_config;
    GNSS_priority_t gnss_priority;
    supported_gnss_constelations_t gnss_config;	/*!< Used to configure the supported GNSS constellations  */
    auto_gps_t auto_gps;     					/*!< Used to enable/disable the GNSS module to automatically run
    												after the module has reboot */
}gnss_config_t;

//////////////////////End of Structures and Enumerators/////////////////////////

//////////////////////////////Functions Description/////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*!
	\brief This function configures the parameters of the GNSS module.

	\param[in] gnss_config_struct This parameter is the GNSS module configuration
                                structure.
	\param[in] get_default_config Whether the configuration used will be the
									default one, or a personal configuration.

	\return This function indicates if the task was successful, or if an error
 	 	 	 occurred.

 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_GNSS_Configuration(gnss_config_t gnss_config_struct,
								   configuration_select_t get_default_config);


////////////////////////////////////////////////////////////////////////////////
/*!
	\brief This function turns on the GNSS module's function.

	\warning Currently, only Stand-alone GNSS operation mode is supported.

	\param[in] mode     The GNSS operation mode.
	\param[in] max_dist The positioning accuracy.
	\param[in] count    The amount of desired positioning attempts (ranges from 0
                      to 1000). 0 is used to indicate continuous positioning.
	\param[in] rate     The time interval between positioning captures.
                      (ranges from 1 to 65535).

	\return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_GNSS_Turn_On(GNSS_operation_modes_t mode, max_positioning_accuracy_t max_dist,
                                   uint16_t count, uint16_t rate);

////////////////////////////////////////////////////////////////////////////////
/*!
	\brief This function is used to turn off the GNSS function.

	\note This function is required only when passing a value of zero to the
        "count" parameter in the CM_Q03_GNSS_Turn_On function, as when such
        parameter is different to zero, it will automatically turn off when
        that value is reached.

	\return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_GNSS_Turn_Off(void);

////////////////////////////////////////////////////////////////////////////////
/*!
	\brief This function is used to acquire the current position's information.

	\warning In order to execute this function successfully, the GNSS module must
           be turned on previously using the CM_Q03_GNSS_Turn_On function.

	\param[in] mode           The format of the position response.
	\param[out] response      The response to the command sent which includes
                            	information of the position.
	\param[out] response_size The size (in bytes) of the response obtained.

	\return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_GNSS_Location(location_format_t mode, uint8_t *response,
                              uint16_t *response_size);
////////////////////////////////////////////////////////////////////////////////
/*!
	\brief This function returns the location given by the GPS.

	\param[out] latitude Pointer to the variable where the latitude will be stored.
	\param[out] latitude_size Pointer to the data where the latitude length will
 	 	 	 	 	 	 	 	 be stored.
	\param[out] longitude Pointer to the variable where the longitude will be stored.
	\param[out] longitude_size Pointer to the data where the longitude length will
 	 	 	 	 	 	 	 	 be stored.

 	\return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
cm_q03_codes_t CM_Q03_GNSS_Get_Location(uint8_t *latitude, uint16_t *latitude_size,
										uint8_t *longitude, uint16_t *longitude_size);

////////////////////////////////////////////////////////////////////////////////
/*!
	\brief This function is used to retrieve NMEA sentences.

	\note In order to execute this function successfully, the GNSS module must
           be turned on previously using the CM_Q03_GNSS_Turn_On function.
           Also, the nmea_src parameter in the module's configuration, from the
           gnss_config_t structure, must be set to nmea_src_enable.

	\param[in]  sentence_type The NMEA type of sentence to be queried.
	\param[out] sentence      The NMEA sentence response.
	\param[out] sentence_size The size of the NMEA sentence response (in bytes).

	\return This function indicates if the task was successful, or if an error
 	 	 	 occurred..
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_GNSS_NMEA(NMEA_sentences_types_t sentence_type,
                          uint8_t *sentence, uint16_t *sentence_size);


/////////////////////////////Include Guard Closing//////////////////////////////
#endif /* CM_Q03_GNSS_H_ */
/////////////////////////End of Include Guard Closing///////////////////////////
