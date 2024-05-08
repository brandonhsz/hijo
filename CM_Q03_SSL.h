////////////////////////////Module's Description////////////////////////////////
/*!
 * \file       CM_Q03_SSL.h
 * \brief      This is the CM_Q03 SSL driver made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.\n
 *             **to identify the meaning of a CME error, please refer to the
 *             provided Quectel SSL AT commands manual.\n
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

////////////////////////////////Include Guard///////////////////////////////////

#ifndef CM_Q03_SSL_H_
#define CM_Q03_SSL_H_

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

/////////////////////////////Definitions Area///////////////////////////////////

//////////////////////////End of Definitions Area///////////////////////////////

//////////////////////////Structures and Enumerators////////////////////////////

/*!
 	 \brief Enumerator to set the SSL version to be used.
 */
typedef enum
{
	ssl_version_ssl_3_0,	/*!< SSL version 3.0*/
	ssl_versiont_tls_1_0,	/*!< TLS version 1.0*/
	ssl_version_tls_1_1,	/*!< TLS version 1.1*/
	ssl_version_tls_1_2,	/*!< TLS version 1.2*/
	ssl_version_all			/*!< Support all version*/
}ssl_version_t;

/*!
 	 \brief Enumerator to set the supported SSL cipher suites.
 */
typedef enum{
    all,								/*!< All cipher suites are supported*/
    rsa_with_aes_256_sha,         		/*!< TLS_RSA_WITH_AES_256_CBC_SHA*/
    rsa_with_aes_128_sha,         		/*!< TLS_RSA_WITH_AES_128_CBC_SHA*/
    rsa_with_rc4_128_sha,         		/*!< TLS_RSA_WITH_RC4_128_SHA*/
    rsa_with_rc4_128_md5,        		/*!< TLS_RSA_WITH_RC4_128_MD5*/
    rsa_with_3des_ede,            		/*!< TLS_RSA_WITH_3DES_EDE_CBC_SHA*/
    rsa_with_aes_256_sha256,      		/*!< TLS_RSA_WITH_AES_256_CBC_SHA256*/
    ecdhe_rsa_with_rc4_128,       		/*!< TLS_ECDHE_RSA_WITH_RC4_128_SHA*/
    ecdhe_rsa_with_3des_ede,      		/*!< TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA*/
    ecdhe_rsa_with_aes_128,      	 	/*!< TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA*/
    ecdhe_rsa_with_aes_256,       		/*!< TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA*/
    ecdhe_rsa_with_aes_128_cbc_sha256,	/*!< TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256*/
    ecdhe_rsa_with_aes_256_sha384,		/*!< TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384*/
    ecdhe_rsa_with_aes_128_gcm_sha256 	/*!< TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256*/
}ssl_suites_t;

/*!
 	 \brief Enumerator to set the security level
 */
typedef enum
{
	no_authentication_mode,				/*!< No authentication mode is selected*/
	manage_server_authentication,		/*!< Manage server authentication*/
	manage_authentication_if_requested	/*!< Manage server and client authentication
	 	 	 	 	 	 	 	 	 	 	 if requested by the remote server*/
}security_level_t;

/*!
 	 \brief Enumerator to define how to deal with an expired
 	 	 	 certificate
 */
typedef enum
{
	care_validity_check,	/*!< Care about validity check for certification*/
	ignore_validity_check	/*!< Ignore validity check for certification*/
}ignore_ltime_t;

/*!
     \brief  Structure for the SLL configuration.
 */
typedef struct
{
	ssl_version_t version; 				/*!< SSL Version for the SSL Context*/
	ssl_suites_t ciphersuite; 			/*!< SSL Cipher Suite for the SSL Context */
	security_level_t seclevel;			/*!< Authentication mode for the SSL Context */
	ignore_ltime_t ignorelocaltime; 	/*!< How to deal with expired certificate.*/
    uint16_t negotiatetime; 			/*!< Indicates maximum timeout used in SSL
     	 	 	 	 	 	 	 	 	 negotiation stage. The range is 10-300 */
} ssl_config_parameters_struct_t;

//////////////////////End of Structures and Enumerators/////////////////////////

//////////////////////////////Functions Description/////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Configure optional parameters.

   \param[in] sslctxID			 Identification number of the context.
   \param[in] ssl_config_struct  Structure with the configuration parameters of
   	                             SLL Protocol.
   \param[in] get_default_config Indicates whether or not to use the default
                                 configuration.

   \return	This function indicates if the task was successful, or if an error
 	 	 	occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_SSL_Configuration(uint8_t sslctxID,
                                  ssl_config_parameters_struct_t ssl_config_struct,
								  configuration_select_t get_default_config);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Configure the path of the trusted CA certificate.

   \param[in] sslctxID	   Identification number of the context.
   \param[in] ca_path      Path of the CA certificate.
   \param[in] size_ca_path Number of characters in CA_path vector.

   \return 	This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_SSL_CA_Certificate(uint8_t sslctxID,
                                   uint8_t *ca_path,
                                   uint16_t size_ca_path);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Configure the path of the client certificate.

   \param[in] sslctxID		Identification number of the context.
   \param[in] cc_path      Path of the client certificate.
   \param[in] size_cc_path Number of characters in CC_path vector.

   \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_SSL_Client_Certificate(uint8_t sslctxID,
                                       uint8_t *cc_path,
                                       uint16_t size_cc_path);

////////////////////////////////////////////////////////////////////////////////
/*!
   \brief Configure the path of the client private key.

   \param[in] sslctxID		Identification number of the context.
   \param[in] key_path      Path of the private key.
   \param[in] size_key_path Number of characters in key_path vector

   \return This function indicates if the task was successful, or if an error
 	 	 	 occurred.
 */
////////////////////////////////////////////////////////////////////////////////
cm_q03_codes_t CM_Q03_SSL_Client_Private_Key(uint8_t sslctxID,
                                       uint8_t *key_path,
                                       uint16_t size_key_path);


//////////////////////////End of Functions Description//////////////////////////

/////////////////////////////Include Guard Closing//////////////////////////////
#endif /* CM_Q03_SSL_H_ */
/////////////////////////End of Include Guard Closing///////////////////////////
