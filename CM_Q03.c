////////////////////////////COPYRIGHT NOTICES///////////////////////////////////
/*
 * Copyright 2016-2018 NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
//////////////////////END OF COPYRIGHT NOTICES//////////////////////////////////

////////////////////////////Module's Description////////////////////////////////
/*!
 * \file       CM_Q03.h
 * \brief      This is the CM_Q03 driver made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.
 * \version    1.0.2
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

#include <CM_Q03.h>
#include <CM_Q03_URC_Control.h>
#include "init_config_functions.h"
#include "fsl_uart.h"
#include "fsl_pmc.h"
#include "fsl_lpuart.h"
#include "fsl_smc.h"

#define CHARACTER_RESPONSE_END 	'\n'	// ATS4
#define CHARACTER_COMMAND_END 	'\r'	// ATS3
#define BUFFER_SIZE 			(1500)
#define UCR_BUFFER_SIZE 		(1)

#define SIMILAR_BUT_NOT_EQUAL		(1)
#define NOT_SIMILAR_AT_ALL			(2)

#define MIN_DELAY				(200)

#define READING_DATA            (0)
#define OK_FOUND                (1)


#define NO_PARAMS       (0)     // No parametros 1 variable
#define NOTDEF_PARAMS   (13)    // Parametros no definidos vector de caracteres
#define NUM_PARAMS      (19)    // Parametros numericos estructura
#define MIX_PARAMS      (71)    // Parametros combinados estructura

#define CM_Q03_TIMEOUT        (1000000)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/********************************* VARIABLES *********************************/
/******************** MIX PARAMETERS ************************/
u16_2_t cdsi = {0};
u16_2_t cmti = {0};
u16_12_t qistate_cid_0 = {0};
u16_12_t qistate_cid_1 = {0};
u16_12_t qistate_cid_2 = {0};
/******************** MIX PARAMETERS ************************/

/******************** NUM PARAMETERS ************************/
uint16_t cfun = 0;
uint16_t pub_st = 0;
uint16_t cpin = 0;
uint16_t creg = 0;

u16_2_t qftpclose = {0};
u16_2_t qftpcwd = {0};
u16_2_t qftpdel = {0};
u16_2_t qftpget = {0};
u16_2_t qftplen = {0};
u16_2_t qftpmkdir = {0};
u16_2_t qftpopen = {0};
u16_2_t qftpput = {0};
u16_2_t qftprename = {0};
u16_2_t qftprmdir = {0};
u16_2_t qftpsize = {0};
u16_2_t qftpstat = {0};

u16_2_t qigeterror = {0};
int16_t qiopen_cid_0 = 0;
int16_t qiopen_cid_1 = 0;
int16_t qiopen_cid_2 = 0;

u16_3_t qisend = {0};
u16_6_t qiurc_incoming_cid_0 = {0};
u16_6_t qiurc_incoming_cid_1 = {0};
u16_6_t qiurc_incoming_cid_2 = {0};

int16_t qmtclose_cid_0 = 0;
int16_t qmtclose_cid_1 = 0;
int16_t qmtclose_cid_2 = 0;

u16_2_t qmtconn_cid_0 = {0};
u16_2_t qmtconn_cid_1 = {0};
u16_2_t qmtconn_cid_2 = {0};

int16_t qmtdisc_cid_0 = 0;
int16_t qmtdisc_cid_1 = 0;
int16_t qmtdisc_cid_2 = 0;

int16_t qmtopen_cid_0 = 0;
int16_t qmtopen_cid_1 = 0;
int16_t qmtopen_cid_2 = 0;

 /* TODO: After reading values from the structures in functions,
check if the values must be set to 0. Eg value in +QMTPUB won't always
be present */
u16_3_t qmtpub_cid_0 = {0};
u16_3_t qmtpub_cid_1 = {0};
u16_3_t qmtpub_cid_2 = {0};

uint16_t qmtstat_cid_0 = 0;
uint16_t qmtstat_cid_1 = 0;
uint16_t qmtstat_cid_2 = 0;

u16_3_t qmtsub_cid_0 = {0};
u16_3_t qmtsub_cid_1 = {0};
u16_3_t qmtsub_cid_2 = {0};

u16_2_t qmtuns_cid_0 = {0};
u16_2_t qmtuns_cid_1 = {0};
u16_2_t qmtuns_cid_2 = {0};

u16_3_t qhttp_get = {0};
int16_t qhttp_read = {0};
int16_t qhttp_readfile = {0};
u16_3_t qhttp_post = {0};
u16_3_t qhttp_postfile = {0};

/******************** NUM PARAMETERS ************************/

/******************** UNDEF PARAMETERS **********************/

uint8_t qftpmdtm[QFTPMDTM_SIZE] = {0};
uint8_t qftppwd[QFTPPWD_SIZE] = {0};
uint8_t qind_geofence[QIND_GEOFENCE_SIZE] = {0};

uint8_t qmtrecv_cid_0[QMTRECV_SIZE] = {0};
uint8_t qmtrecv_cid_1[QMTRECV_SIZE] = {0};
uint8_t qmtrecv_cid_2[QMTRECV_SIZE] = {0};

/******************** UNDEF PARAMETERS **********************/

/******************** NO PARAMETERS *************************/
no_params_flags_t no_param_urc = {0};
/******************** NO PARAMETERS *************************/

/********************************* VARIABLES *********************************/


/********************************* VECTOR ************************************/



enum Pos{
    POS_0 =0,
    POS_1,
    POS_2,
    POS_3,
    POS_4,
    POS_5,
    POS_6,
    POS_7,
    POS_8,
    POS_9,
    POS_10,
    POS_11,
    POS_12,
    POS_13,
    POS_14,
    POS_15,
    POS_16,
    POS_17,
    POS_18,
    POS_19,
    POS_20,
    POS_21,
    POS_22,
    POS_23,
    POS_24,
    POS_25,
    POS_26,
    POS_27,
    POS_28,
    POS_29,
    POS_30,
    POS_31,
    POS_32,
    POS_33,
    POS_34,
    POS_35,
    POS_36,
    POS_37,
    POS_38,
    POS_39,
    POS_40,
    POS_41,
    POS_42,
    POS_43,
    POS_44,
    POS_45,
    POS_46,
    POS_47,
    POS_48,
    POS_49,
    POS_50,
    POS_51,
    POS_52,
    POS_53,
    POS_54,
    POS_55,
    POS_56,
    POS_57,
    POS_58,
    POS_59,
    POS_60,
    POS_61,
    POS_62,
    POS_63,
    POS_64,
    POS_65,
    POS_66,
    POS_67,
    POS_68,
    POS_69,
    POS_70,
    POS_71,
	POS_72,
	POS_73,
	POS_74,
	POS_75,
	POS_76
};

const void* buff_vector[77] =
{
    /** No parameters buffers*/
    (void*)(&no_param_urc.all_flags),		//Pos 0 RDY
    (void*)(&no_param_urc.all_flags),		//Pos 1 POWERED DOWN
    (void*)(&no_param_urc.all_flags),		//Pos 2 SMS DONE
    (void*)(&no_param_urc.all_flags),		//Pos 3 QIURC CLOSED CID 0
    (void*)(&no_param_urc.all_flags),		//Pos 4 QIURC CLOSED CID 1
    (void*)(&no_param_urc.all_flags),		//Pos 5 QIURC CLOSED CID 2
    (void*)(&no_param_urc.all_flags),		//Pos 6 QIURC INCOMING FULL
    (void*)(&no_param_urc.all_flags),		//Pos 7 PDPDEACT CID 0
    (void*)(&no_param_urc.all_flags),		//Pos 8 PDPDEACT CID 1
    (void*)(&no_param_urc.all_flags),		//Pos 9 PDPDEACT CID 2
    (void*)(&no_param_urc.all_flags), 		//Pos 10 QIURC RECV CID 0
    (void*)(&no_param_urc.all_flags),		//Pos 11 QIURC RECV CID 1
    (void*)(&no_param_urc.all_flags),		//Pos 12 QIURC RECV CID 2

    /** Undefined parameters buffers*/
    (void*)(qftpmdtm),						//Pos 13
    (void*)(qftppwd),						//Pos 14
    (void*)(qind_geofence),					//Pos 15
    (void*)(qmtrecv_cid_0),					//Pos 16
    (void*)(qmtrecv_cid_1),					//Pos 17
    (void*)(qmtrecv_cid_2),					//Pos 18

    /** Defined numerical parameters buffers*/
    (void*)(&cfun),							//Pos 19
    (void*)(&pub_st),						//Pos 20
    (void*)(&creg),							//Pos 21

    (void*)(&qftpclose.u16_0),				//Pos 22
    (void*)(&qftpcwd.u16_0),				//Pos 23
    (void*)(&qftpdel.u16_0),				//Pos 24
    (void*)(&qftpget.u16_0),				//Pos 25
    (void*)(&qftplen.u16_0),				//Pos 26
    (void*)(&qftpmkdir.u16_0),				//Pos 27
    (void*)(&qftpopen.u16_0),				//Pos 28
    (void*)(&qftpput.u16_0), 				//Pos 29
    (void*)(&qftprename.u16_0),				//Pos 30
    (void*)(&qftprmdir.u16_0), 				//Pos 31
    (void*)(&qftpsize.u16_0), 				//Pos 32
    (void*)(&qftpstat.u16_0),				//Pos 33

    (void*)(&qigeterror.u16_0),				//Pos 34
    (void*)(&qiopen_cid_0),					//Pos 35
    (void*)(&qiopen_cid_1),					//Pos 36
    (void*)(&qiopen_cid_2),					//Pos 37

    (void*)(&qisend.u16_0),					//Pos 38
    (void*)(&qiurc_incoming_cid_0.u16_0),	//Pos 39
    (void*)(&qiurc_incoming_cid_1.u16_0),	//Pos 40
    (void*)(&qiurc_incoming_cid_2.u16_0),	//Pos 41

    (void*)(&qmtclose_cid_0),				//Pos 42
    (void*)(&qmtclose_cid_1),				//Pos 43
    (void*)(&qmtclose_cid_2),				//Pos 44
    (void*)(&qmtconn_cid_0.u16_0),			//Pos 45
    (void*)(&qmtconn_cid_1.u16_0),			//Pos 46
    (void*)(&qmtconn_cid_2.u16_0),			//Pos 47

    (void*)(&qmtdisc_cid_0),				//Pos 48
    (void*)(&qmtdisc_cid_1),				//Pos 49
    (void*)(&qmtdisc_cid_2),				//Pos 50

    (void*)(&qmtopen_cid_0),				//Pos 51
    (void*)(&qmtopen_cid_1),				//Pos 52
    (void*)(&qmtopen_cid_2),				//Pos 53

    (void*)(&qmtpub_cid_0.u16_0),			//Pos 54
    (void*)(&qmtpub_cid_1.u16_0),			//Pos 55
    (void*)(&qmtpub_cid_2.u16_0),			//Pos 56

    (void*)(&qmtstat_cid_0),				//Pos 57
    (void*)(&qmtstat_cid_1),				//Pos 58
    (void*)(&qmtstat_cid_2),				//Pos 59

    (void*)(&qmtsub_cid_0.u16_0),			//Pos 60
    (void*)(&qmtsub_cid_1.u16_0), 			//Pos 61
    (void*)(&qmtsub_cid_2.u16_0),			//Pos 62

    (void*)(&qmtuns_cid_0.u16_0),			//Pos 63
    (void*)(&qmtuns_cid_1.u16_0),			//Pos 64
    (void*)(&qmtuns_cid_2.u16_0),			//Pos 65

    (void*)(&qhttp_get.u16_0),				//Pos 66
	(void*)(&qhttp_read),					//Pos 67
	(void*)(&qhttp_readfile),				//Pos 68
    (void*)(&qhttp_post.u16_0),				//Pos 68
    (void*)(&qhttp_postfile.u16_0),			//Pos 69

    /** Defined mixed parameters buffers*/
    (void*)(&cdsi.u16_0),					//Pos 70
    (void*)(&cmti.u16_0),					//Pos 71
    (void*)(&qistate_cid_0.u16_0),			//Pos 72
    (void*)(&qistate_cid_1.u16_0),			//Pos 73
    (void*)(&qistate_cid_2.u16_0),			//Pos 74
    (void*)(&cpin)							//Pos 75
};

uint8_t urc_flags[URC_FLAG_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
/********************************* VECTOR ************************************/

const urc_struct_t pos_70={{.div_char ={POS_70, 1}}, NULL, NULL};          //**pos70
const urc_struct_t BZ11_F ={{.char_urc = 'F'}, &pos_70, NULL};
const urc_struct_t pos_69 ={{.div_char ={POS_69, 1}}, NULL, NULL};          //**pos69
const urc_struct_t BY11_COLON ={{.char_urc = ':'}, &pos_69, &BZ11_F};
const urc_struct_t BY10_T ={{.char_urc = 'T'}, &BY11_COLON, NULL};
const urc_struct_t BY9_S ={{.char_urc = 'S'}, &BY10_T, NULL};
const urc_struct_t BY8_O ={{.char_urc = 'O'}, &BY9_S, NULL};
const urc_struct_t BY7_P ={{.char_urc = 'P'}, &BY8_O, NULL};

const urc_struct_t pos_68 ={{.div_char ={POS_68, 1}}, NULL, NULL};          //**pos68
const urc_struct_t BX11_F ={{.char_urc = 'F'}, &pos_68, NULL};
const urc_struct_t pos_67 ={{.div_char ={POS_67, 1}}, NULL, NULL};          //**pos67
const urc_struct_t BW11_COLON ={{.char_urc = ':'}, &pos_67, &BX11_F};
const urc_struct_t BW10_D ={{.char_urc = 'D'}, &BW11_COLON, NULL};
const urc_struct_t BW9_A ={{.char_urc = 'A'}, &BW10_D, NULL};
const urc_struct_t BW8_E ={{.char_urc = 'E'}, &BW9_A, NULL};
const urc_struct_t BW7_R ={{.char_urc = 'R'}, &BW8_E, &BY7_P};

const urc_struct_t pos_66 ={{.div_char ={POS_66, 1}}, NULL, NULL};          //**pos66
const urc_struct_t BV7_G ={{.char_urc = 'G'}, &pos_66, &BW7_R};
const urc_struct_t BV6_P ={{.char_urc = 'P'}, &BV7_G, NULL};
const urc_struct_t BV5_T ={{.char_urc = 'T'}, &BV6_P, NULL};
const urc_struct_t BV4_T ={{.char_urc = 'T'}, &BV5_T, NULL};
const urc_struct_t BV3_H ={{.char_urc = 'H'}, &BV4_T, NULL};

const urc_struct_t pos_65={{.div_char ={POS_65, 1}}, NULL, NULL};          //**pos65
const urc_struct_t BU11_COMA ={{.char_urc = ','}, &pos_65, NULL};
const urc_struct_t BU10_2 ={{.char_urc = '2'}, &BU11_COMA, NULL};
const urc_struct_t pos_64 ={{.div_char ={POS_64, 1}}, NULL, NULL};          //**pos64
const urc_struct_t BT11_COMA ={{.char_urc = ','}, &pos_64, NULL};
const urc_struct_t BT10_1 ={{.char_urc = '1'}, &BT11_COMA, &BU10_2};
const urc_struct_t pos_63 ={{.div_char ={POS_63, 1}}, NULL, NULL};          //**pos63
const urc_struct_t BS11_COMA ={{.char_urc = ','}, &pos_63, NULL};
const urc_struct_t BS10_0 ={{.char_urc = '0'}, &BS11_COMA, &BT10_1};

const urc_struct_t BS9_SPACE ={{.char_urc = ' '}, &BS10_0, NULL};
const urc_struct_t BS8_COLON ={{.char_urc = ':'}, &BS9_SPACE, NULL};
const urc_struct_t BS7_S ={{.char_urc = 'S'}, &BS8_COLON, NULL};
const urc_struct_t BS6_N ={{.char_urc = 'N'}, &BS7_S, NULL};

const urc_struct_t BS5_U ={{.char_urc = 'U'}, &BS6_N, NULL};


const urc_struct_t pos_62 ={{.div_char ={POS_62, 1}}, NULL, NULL};          //**pos62
const urc_struct_t BR11_COMA ={{.char_urc = ','}, &pos_62, NULL};
const urc_struct_t BR10_2 ={{.char_urc = '2'}, &BR11_COMA, NULL};
const urc_struct_t pos_61 ={{.div_char ={POS_61, 1}}, NULL, NULL};          //**pos61
const urc_struct_t BQ11_COMA ={{.char_urc = ','}, &pos_61, NULL};
const urc_struct_t BQ10_1 ={{.char_urc = '1'}, &BQ11_COMA, &BR10_2};
const urc_struct_t pos_60 ={{.div_char ={POS_60, 1}}, NULL, NULL};          //**pos60
const urc_struct_t BP11_COMA ={{.char_urc = ','}, &pos_60, NULL};
const urc_struct_t BP10_0 ={{.char_urc = '0'}, &BP11_COMA, &BQ10_1};

const urc_struct_t BP9_SPACE ={{.char_urc = ' '}, &BP10_0, NULL};
const urc_struct_t BP8_COLON ={{.char_urc = ':'}, &BP9_SPACE, NULL};
const urc_struct_t BP7_B ={{.char_urc = 'B'}, &BP8_COLON, NULL};
const urc_struct_t BP6_U ={{.char_urc = 'U'}, &BP7_B, NULL};

const urc_struct_t pos_59 ={{.div_char ={POS_59, 1}}, NULL, NULL};          //**pos59
const urc_struct_t BO12_COMA ={{.char_urc = ','}, &pos_59, NULL};
const urc_struct_t BO11_2 ={{.char_urc = '2'}, &BO12_COMA, NULL};
const urc_struct_t pos_58 ={{.div_char ={POS_58, 1}}, NULL, NULL};          //**pos58
const urc_struct_t BN12_COMA ={{.char_urc = ','}, &pos_58, NULL};
const urc_struct_t BN11_1 ={{.char_urc = '1'}, &BN12_COMA, &BO11_2};
const urc_struct_t pos_57 ={{.div_char ={POS_57, 1}}, NULL, NULL};          //**pos57
const urc_struct_t BM12_COMA ={{.char_urc = ','}, &pos_57, NULL};
const urc_struct_t BM11_0 ={{.char_urc = '0'}, &BM12_COMA, &BN11_1};

const urc_struct_t BM10_SPACE ={{.char_urc = ' '}, &BM11_0, NULL};
const urc_struct_t BM9_COLON ={{.char_urc = ':'}, &BM10_SPACE, NULL};
const urc_struct_t BM8_T ={{.char_urc = 'T'}, &BM9_COLON, NULL};
const urc_struct_t BM7_A ={{.char_urc = 'A'}, &BM8_T, NULL};
const urc_struct_t BM6_T ={{.char_urc = 'T'}, &BM7_A, &BP6_U};

const urc_struct_t BM5_S ={{.char_urc = 'S'}, &BM6_T, &BS5_U};


const urc_struct_t pos_18 ={{.div_char ={POS_18, 1}}, NULL, NULL};          //**pos18
const urc_struct_t BL12_COMA ={{.char_urc = ','}, &pos_18, NULL};
const urc_struct_t BL11_2 ={{.char_urc = '2'}, &BL12_COMA, NULL};
const urc_struct_t pos_17 ={{.div_char ={POS_17, 1}}, NULL, NULL};          //**pos17
const urc_struct_t BK12_COMA ={{.char_urc = ','}, &pos_17, NULL};
const urc_struct_t BK11_1 ={{.char_urc = '1'}, &BK12_COMA, &BL11_2};
const urc_struct_t pos_16 ={{.div_char ={POS_16, 1}}, NULL, NULL};          //**pos16
const urc_struct_t BJ12_COMA ={{.char_urc = ','}, &pos_16, NULL};
const urc_struct_t BJ11_0 ={{.char_urc = '0'}, &BJ12_COMA, &BK11_1};

const urc_struct_t BJ10_SPACE ={{.char_urc = ' '}, &BJ11_0, NULL};
const urc_struct_t BJ9_COLON ={{.char_urc = ':'}, &BJ10_SPACE, NULL};
const urc_struct_t BJ8_V ={{.char_urc = 'V'}, &BJ9_COLON, NULL};
const urc_struct_t BJ7_C ={{.char_urc = 'C'}, &BJ8_V, NULL};
const urc_struct_t BJ6_E ={{.char_urc = 'E'}, &BJ7_C, NULL};

const urc_struct_t BJ6_R ={{.char_urc = 'R'}, &BJ6_E, &BM5_S};


const urc_struct_t pos_56 ={{.div_char ={POS_56, 1}}, NULL, NULL};          //**pos56
const urc_struct_t BI11_COMA ={{.char_urc = ','}, &pos_56, NULL};
const urc_struct_t BI10_2 ={{.char_urc = '2'}, &BI11_COMA, NULL};

const urc_struct_t pos_55 ={{.div_char ={POS_55, 1}}, NULL, NULL};          //**pos55
const urc_struct_t BH11_COMA ={{.char_urc = ','}, &pos_55, NULL};
const urc_struct_t BH10_1 ={{.char_urc = '1'}, &BH11_COMA, &BI10_2};

const urc_struct_t pos_54 ={{.div_char ={POS_54, 1}}, NULL, NULL};          //**pos54
const urc_struct_t BG11_COMA ={{.char_urc = ','}, &pos_54, NULL};
const urc_struct_t BG10_0 ={{.char_urc = '0'}, &BG11_COMA, &BH10_1};

const urc_struct_t BG9_SPACE ={{.char_urc = ' '}, &BG10_0, NULL};
const urc_struct_t BG8_COLON ={{.char_urc = ':'}, &BG9_SPACE, NULL};
const urc_struct_t BG7_B ={{.char_urc = 'B'}, &BG8_COLON, NULL};
const urc_struct_t BG6_U ={{.char_urc = 'U'}, &BG7_B, NULL};

const urc_struct_t BG5_P ={{.char_urc = 'P'}, &BG6_U, &BJ6_R};


const urc_struct_t pos_53={{.div_char ={POS_53, 1}}, NULL, NULL};          //**pos53
const urc_struct_t BF12_COMA ={{.char_urc = ','}, &pos_53, NULL};
const urc_struct_t BF11_2 ={{.char_urc = '2'}, &BF12_COMA, NULL};
const urc_struct_t pos_52={{.div_char ={POS_52, 1}}, NULL, NULL};          //**pos52
const urc_struct_t BE12_COMA ={{.char_urc = ','}, &pos_52, NULL};
const urc_struct_t BE11_1 ={{.char_urc = '1'}, &BE12_COMA, &BF11_2};
const urc_struct_t pos_51={{.div_char ={POS_51, 1}}, NULL, NULL};          //**pos51
const urc_struct_t BD12_COMA ={{.char_urc = ','}, &pos_51, NULL};
const urc_struct_t BD11_0 ={{.char_urc = '0'}, &BD12_COMA, &BE11_1};

const urc_struct_t BD10_SPACE ={{.char_urc = ' '}, &BD11_0, NULL};
const urc_struct_t BD9_COLON ={{.char_urc = ':'}, &BD10_SPACE, NULL};
const urc_struct_t BD8_N ={{.char_urc = 'N'}, &BD9_COLON, NULL};
const urc_struct_t BD7_E ={{.char_urc = 'E'}, &BD8_N, NULL};
const urc_struct_t BD6_P ={{.char_urc = 'P'}, &BD7_E, NULL};

const urc_struct_t BD5_O ={{.char_urc = 'O'}, &BD6_P, &BG5_P};


const urc_struct_t pos_50={{.div_char ={POS_50, 1}}, NULL, NULL};          //**pos50
const urc_struct_t BC12_COMA ={{.char_urc = ','}, &pos_50, NULL};
const urc_struct_t BC11_2 ={{.char_urc = '2'}, &BC12_COMA, NULL};
const urc_struct_t pos_49={{.div_char ={POS_49, 1}}, NULL, NULL};          //**pos49
const urc_struct_t BB12_COMA ={{.char_urc = ','}, &pos_49, NULL};
const urc_struct_t BB11_1 ={{.char_urc = '1'}, &BB12_COMA, &BC11_2};
const urc_struct_t pos_48={{.div_char ={POS_48, 1}}, NULL, NULL};          //**pos48
const urc_struct_t BA12_COMA ={{.char_urc = ','}, &pos_48, NULL};
const urc_struct_t BA11_0 ={{.char_urc = '0'}, &BA12_COMA, &BB11_1};

const urc_struct_t BA10_SPACE ={{.char_urc = ' '}, &BA11_0, NULL};
const urc_struct_t BA9_COLON ={{.char_urc = ':'}, &BA10_SPACE, NULL};
const urc_struct_t BA8_C ={{.char_urc = 'C'}, &BA9_COLON, NULL};
const urc_struct_t BA7_S ={{.char_urc = 'S'}, &BA8_C, NULL};
const urc_struct_t BA6_I ={{.char_urc = 'I'}, &BA7_S, NULL};

const urc_struct_t BA5_D ={{.char_urc = 'D'}, &BA6_I, &BD5_O};


const urc_struct_t pos_47={{.div_char ={POS_47, 1}}, NULL, NULL};          //**pos47
const urc_struct_t AZ12_COMA ={{.char_urc = ','}, &pos_47, NULL};
const urc_struct_t AZ11_2 ={{.char_urc = '2'}, &AZ12_COMA, NULL};
const urc_struct_t pos_46={{.div_char ={POS_46, 1}}, NULL, NULL};          //**pos46
const urc_struct_t AY12_COMA ={{.char_urc = ','}, &pos_46, NULL};
const urc_struct_t AY11_1 ={{.char_urc = '1'}, &AY12_COMA, &AZ11_2};
const urc_struct_t pos_45={{.div_char ={POS_45, 1}}, NULL, NULL};          //**pos45
const urc_struct_t AX12_COMA ={{.char_urc = ','}, &pos_45, NULL};
const urc_struct_t AX11_0 ={{.char_urc = '0'}, &AX12_COMA, &AY11_1};

const urc_struct_t AX10_SPACE ={{.char_urc = ' '}, &AX11_0, NULL};
const urc_struct_t AX9_COLON ={{.char_urc = ':'}, &AX10_SPACE, NULL};
const urc_struct_t AX8_N ={{.char_urc = 'N'}, &AX9_COLON, NULL};
const urc_struct_t AX7_N ={{.char_urc = 'N'}, &AX8_N, NULL};
const urc_struct_t AX6_O ={{.char_urc = 'O'}, &AX7_N, NULL};

const urc_struct_t pos_44 ={{.div_char ={POS_44, 1}}, NULL, NULL};          //**pos44
const urc_struct_t AW13_COMA ={{.char_urc = ','}, &pos_44, NULL};
const urc_struct_t AW12_2 ={{.char_urc = '2'}, &AW13_COMA, NULL};
const urc_struct_t pos_43 ={{.div_char ={POS_43, 1}}, NULL, NULL};          //**pos43
const urc_struct_t AV13_COMA ={{.char_urc = ','}, &pos_43, NULL};
const urc_struct_t AV12_1 ={{.char_urc = '1'}, &AV13_COMA, &AW12_2};
const urc_struct_t pos_42 ={{.div_char ={POS_42, 1}}, NULL, NULL};          //**pos42
const urc_struct_t AU13_COMA ={{.char_urc = ','}, &pos_42, NULL};
const urc_struct_t AU12_0 ={{.char_urc = '0'}, &AU13_COMA, &AV12_1};

const urc_struct_t AU11_SPACE ={{.char_urc = ' '}, &AU12_0, NULL};
const urc_struct_t AU10_COLON ={{.char_urc = ':'}, &AU11_SPACE, NULL};
const urc_struct_t AU9_E ={{.char_urc = 'E'}, &AU10_COLON, NULL};
const urc_struct_t AU8_S ={{.char_urc = 'S'}, &AU9_E, NULL};
const urc_struct_t AU7_O ={{.char_urc = 'O'}, &AU8_S, NULL};
const urc_struct_t AU6_L ={{.char_urc = 'L'}, &AU7_O, &AX6_O};

const urc_struct_t AU5_C ={{.char_urc = 'C'}, &AU6_L, &BA5_D};


const urc_struct_t AU4_T ={{.char_urc = 'T'}, &AU5_C, NULL};
const urc_struct_t AU3_M ={{.char_urc = 'M'}, &AU4_T, &BV3_H};



const urc_struct_t pos_12 ={{.div_char ={POS_12, 1}}, NULL, NULL};          //**pos12
const urc_struct_t AT16_2 ={{.char_urc = '2'}, &pos_12, NULL};
const urc_struct_t pos_11 ={{.div_char ={POS_11, 1}}, NULL, NULL};          //**pos11
const urc_struct_t AS16_1 ={{.char_urc = '1'}, &pos_11, &AT16_2};
const urc_struct_t pos_10 ={{.div_char ={POS_10, 1}}, NULL, NULL};          //**pos10
const urc_struct_t AR16_0 ={{.char_urc = '0'}, &pos_10, &AS16_1};

const urc_struct_t AR15_COMA ={{.char_urc = ','}, &AR16_0, NULL};
const urc_struct_t AR14_QUOTES ={{.char_urc = '"'}, &AR15_COMA, NULL};
const urc_struct_t AR13_v ={{.char_urc = 'v'}, &AR14_QUOTES, NULL};
const urc_struct_t AR12_c ={{.char_urc = 'c'}, &AR13_v, NULL};
const urc_struct_t AR11_e ={{.char_urc = 'e'}, &AR12_c, NULL};

const urc_struct_t AR10_r ={{.char_urc = 'r'}, &AR11_e, NULL};

const urc_struct_t pos_5 ={{.div_char ={POS_5, 1}}, NULL, NULL};          //**pos5
const urc_struct_t AQ18_2 ={{.char_urc = '2'}, &pos_5, NULL};
const urc_struct_t pos_4 ={{.div_char ={POS_4, 1}}, NULL, NULL};          //**pos4
const urc_struct_t AP18_1 ={{.char_urc = '1'}, &pos_4, &AQ18_2};
const urc_struct_t pos_3 ={{.div_char ={POS_3, 1}}, NULL, NULL};          //**pos3
const urc_struct_t A018_0 ={{.char_urc = '0'}, &pos_3, &AP18_1};

const urc_struct_t A017_COMA ={{.char_urc = ','}, &A018_0, NULL};
const urc_struct_t A016_QUOTES ={{.char_urc = '"'}, &A017_COMA, NULL};
const urc_struct_t A015_d ={{.char_urc = 'd'}, &A016_QUOTES, NULL};
const urc_struct_t A014_e ={{.char_urc = 'e'}, &A015_d, NULL};
const urc_struct_t A013_s ={{.char_urc = 's'}, &A014_e, NULL};
const urc_struct_t A012_o ={{.char_urc = 'o'}, &A013_s, NULL};
const urc_struct_t A011_l ={{.char_urc = 'l'}, &A012_o, NULL};

const urc_struct_t A010_c ={{.char_urc = 'c'}, &A011_l, &AR10_r};

const urc_struct_t pos_9={{.div_char ={POS_9, 1}}, NULL, NULL};          //**pos9
const urc_struct_t AN20_2 ={{.char_urc = '2'}, &pos_9, NULL};
const urc_struct_t pos_8={{.div_char ={POS_8, 1}}, NULL, NULL};          //**pos8
const urc_struct_t AM20_1 ={{.char_urc = '1'}, &pos_8, &AN20_2};
const urc_struct_t pos_7={{.div_char ={POS_7, 1}}, NULL, NULL};          //**pos7
const urc_struct_t AL20_0 ={{.char_urc = '0'}, &pos_7, &AM20_1};

const urc_struct_t AL19_COMA ={{.char_urc = ','}, &AL20_0, NULL};
const urc_struct_t AL18_QUOTES ={{.char_urc = '"'}, &AL19_COMA, NULL};
const urc_struct_t AL17_t ={{.char_urc = 't'}, &AL18_QUOTES, NULL};
const urc_struct_t AL16_c ={{.char_urc = 'c'}, &AL17_t, NULL};
const urc_struct_t AL15_a ={{.char_urc = 'a'}, &AL16_c, NULL};
const urc_struct_t AL14_e ={{.char_urc = 'e'}, &AL15_a, NULL};
const urc_struct_t AL13_d ={{.char_urc = 'd'}, &AL14_e, NULL};
const urc_struct_t AL12_p ={{.char_urc = 'p'}, &AL13_d, NULL};
const urc_struct_t AL11_d ={{.char_urc = 'd'}, &AL12_p, NULL};

const urc_struct_t AL10_p ={{.char_urc = 'p'}, &AL11_d, &A010_c};

const urc_struct_t pos_6={{.div_char ={POS_6,1}}, NULL, NULL};          //**pos6
const urc_struct_t AK18_SPACE ={{.char_urc = ' '}, &pos_6, NULL};

const urc_struct_t pos_41 ={{.div_char ={POS_41, 1}}, NULL, NULL};          //**pos41
const urc_struct_t AJ21_COMA ={{.char_urc = ','}, &pos_41, NULL};
const urc_struct_t AJ20_2 ={{.char_urc = '2'}, &AJ21_COMA, NULL};
const urc_struct_t pos_40 ={{.div_char ={POS_40, 1}}, NULL, NULL};          //**pos40
const urc_struct_t AI21_COMA ={{.char_urc = ','}, &pos_40, NULL};
const urc_struct_t AI20_1 ={{.char_urc = '1'}, &AI21_COMA, &AJ20_2};
const urc_struct_t pos_39 ={{.div_char ={POS_39, 1}}, NULL, NULL};          //**pos39
const urc_struct_t AH21_COMA ={{.char_urc = ','}, &pos_39, NULL};
const urc_struct_t AH20_0 ={{.char_urc = '0'}, &AH21_COMA, &AI20_1};

const urc_struct_t AH19_COMA ={{.char_urc = ','}, &AH20_0, NULL};
const urc_struct_t AH18_QUOTES ={{.char_urc = '"'}, &AH19_COMA, &AK18_SPACE};

const urc_struct_t AH17_g ={{.char_urc = 'g'}, &AH18_QUOTES, NULL};
const urc_struct_t AH16_n ={{.char_urc = 'n'}, &AH17_g, NULL};
const urc_struct_t AH15_i ={{.char_urc = 'i'}, &AH16_n, NULL};
const urc_struct_t AH14_m ={{.char_urc = 'm'}, &AH15_i, NULL};
const urc_struct_t AH13_o ={{.char_urc = 'o'}, &AH14_m, NULL};
const urc_struct_t AH12_c ={{.char_urc = 'c'}, &AH13_o, NULL};
const urc_struct_t AH11_n ={{.char_urc = 'n'}, &AH12_c, NULL};

const urc_struct_t AH10_i ={{.char_urc = 'i'}, &AH11_n, &AL10_p};

const urc_struct_t AH9_QUOTES ={{.char_urc = '"'}, &AH10_i, NULL};
const urc_struct_t AR8_SPACE ={{.char_urc = ' '}, &AH9_QUOTES, NULL};
const urc_struct_t AR7_COLON ={{.char_urc = ':'}, &AR8_SPACE, NULL};
const urc_struct_t AR6_C ={{.char_urc = 'C'}, &AR7_COLON, NULL};
const urc_struct_t AR5_R ={{.char_urc = 'R'}, &AR6_C, NULL};

const urc_struct_t AR4_U ={{.char_urc = 'U'}, &AR5_R, NULL};


const urc_struct_t pos_75 ={{.div_char ={POS_75, 1}}, NULL, NULL};          //**pos75
const urc_struct_t AG12_COMA ={{.char_urc = ','}, &pos_75, NULL};
const urc_struct_t AG11_2 ={{.char_urc = '2'}, &AG12_COMA, NULL};
const urc_struct_t pos_74 ={{.div_char ={POS_74, 1}}, NULL, NULL};          //**pos74
const urc_struct_t AF12_COMA ={{.char_urc = ','}, &pos_74, NULL};
const urc_struct_t AF11_1 ={{.char_urc = '1'}, &AF12_COMA, &AG11_2};
const urc_struct_t pos_73 ={{.div_char ={POS_73, 1}}, NULL, NULL};          //**pos73
const urc_struct_t AE12_COMA ={{.char_urc = ','}, &pos_73, NULL};
const urc_struct_t AE11_0 ={{.char_urc = '0'}, &AE12_COMA, &AF11_1};

const urc_struct_t AE10_SPACE ={{.char_urc = ' '}, &AE11_0, NULL};
const urc_struct_t AE9_COLON ={{.char_urc = ':'}, &AE10_SPACE, NULL};

const urc_struct_t AE8_E ={{.char_urc = 'E'}, &AE9_COLON, NULL};
const urc_struct_t AE7_T ={{.char_urc = 'T'}, &AE8_E, NULL};
const urc_struct_t AE6_A ={{.char_urc = 'A'}, &AE7_T, NULL};
const urc_struct_t AE5_T ={{.char_urc = 'T'}, &AE6_A, NULL};

const urc_struct_t pos_38={{.div_char ={POS_38, 1}}, NULL, NULL};          //**pos38
const urc_struct_t AD5_E ={{.char_urc = 'E'}, &pos_38, &AE5_T};

const urc_struct_t AD4_S ={{.char_urc = 'S'}, &AD5_E, &AR4_U};


const urc_struct_t AC7_SPACE ={{.char_urc = ' '}, NULL, NULL};
const urc_struct_t AC6_COLON ={{.char_urc = ':'}, &AC7_SPACE, NULL};
const urc_struct_t AC5_D ={{.char_urc = 'D'}, &AC6_COLON, NULL};

const urc_struct_t AC4_R ={{.char_urc = 'R'}, &AC5_D, &AD4_S};


const urc_struct_t pos_37={{.div_char ={POS_37, 1}}, NULL, NULL};          //**pos37
const urc_struct_t AB11_COMA ={{.char_urc = ','}, &pos_37, NULL};
const urc_struct_t AB10_2 ={{.char_urc = '2'}, &AB11_COMA, NULL};
const urc_struct_t pos_36={{.div_char ={POS_36, 1}}, NULL, NULL};          //**pos36
const urc_struct_t AA11_COMA ={{.char_urc = ','}, &pos_36, NULL};
const urc_struct_t AA10_1 ={{.char_urc = '1'}, &AA11_COMA, &AB10_2};
const urc_struct_t pos_35={{.div_char ={POS_35, 1}}, NULL, NULL};          //**pos35
const urc_struct_t Z11_COMA ={{.char_urc = ','}, &pos_35, NULL};
const urc_struct_t Z10_0 ={{.char_urc = '0'}, &Z11_COMA, &AA10_1};

const urc_struct_t Z9_SPACE ={{.char_urc = ' '}, &Z10_0, NULL};
const urc_struct_t Z8_COLON ={{.char_urc = ':'}, &Z9_SPACE, NULL};
const urc_struct_t Z7_N ={{.char_urc = 'N'}, &Z8_COLON, NULL};
const urc_struct_t Z6_E ={{.char_urc = 'E'}, &Z7_N, NULL};
const urc_struct_t Z5_P ={{.char_urc = 'P'}, &Z6_E, NULL};

const urc_struct_t Z4_O ={{.char_urc = 'O'}, &Z5_P, &AC4_R};

const urc_struct_t pos_2 ={{.div_char ={POS_2,1}}, NULL, NULL};          //**pos2
const urc_struct_t Y8_S ={{.char_urc = 'S'}, &pos_2, NULL};

const urc_struct_t pos_15 ={{.div_char ={POS_15, 1}}, NULL, NULL};          //**pos15
const urc_struct_t X18_COMA ={{.char_urc = ','}, &pos_15, NULL};
const urc_struct_t X17_QUOTES ={{.char_urc = '"'}, &X18_COMA, NULL};
const urc_struct_t X16_E ={{.char_urc = 'E'}, &X17_QUOTES, NULL};
const urc_struct_t X15_C ={{.char_urc = 'C'}, &X16_E, NULL};
const urc_struct_t X14_N ={{.char_urc = 'N'}, &X15_C, NULL};
const urc_struct_t X13_E ={{.char_urc = 'E'}, &X14_N, NULL};
const urc_struct_t X12_F = {{.char_urc = 'F'}, &X13_E, NULL};
const urc_struct_t X11_O = {{.char_urc = 'O'}, &X12_F, NULL};
const urc_struct_t X10_E = {{.char_urc = 'E'}, &X11_O, NULL};
const urc_struct_t X9_G = {{.char_urc = 'G'}, &X10_E, NULL};

const urc_struct_t X8_QUOTES = {{.char_urc = '"'}, &X9_G, &Y8_S};

const urc_struct_t X7_SPACE = {{.char_urc = ' '}, &X8_QUOTES, NULL};
const urc_struct_t X6_COLON = {{.char_urc = ':'}, &X7_SPACE, NULL};
const urc_struct_t X5_D = {{.char_urc = 'D'}, &X6_COLON, NULL};

const urc_struct_t X4_N = {{.char_urc = 'N'}, &X5_D, &Z4_O};



const urc_struct_t pos_34 = {{.div_char ={POS_34, 1}}, NULL, NULL};          //**pos34
const urc_struct_t W4_G = {{.char_urc = 'G'}, &pos_34, &X4_N};


const urc_struct_t W3_I = {{.char_urc = 'I'}, &W4_G, &AU3_M}; //I


const urc_struct_t pos_33 = {{.div_char ={POS_33, 1}}, NULL, NULL};          //**pos33
const urc_struct_t V7_T = {{.char_urc = 'T'}, &pos_33, NULL};
const urc_struct_t pos_32 = {{.div_char ={POS_32, 1}}, NULL, NULL};          //**pos32
const urc_struct_t U7_I ={{.char_urc = 'I'}, &pos_32, &V7_T};

const urc_struct_t U6_S ={{.char_urc = 'S'}, &U7_I, NULL};


const urc_struct_t pos_31 ={{.div_char ={POS_31, 1}}, NULL, NULL};          //**pos31
const urc_struct_t T7_M = {{.char_urc = 'M'}, &pos_31, NULL};
const urc_struct_t pos_30 ={{.div_char ={POS_30, 1}}, NULL, NULL};          //**pos30
const urc_struct_t S7_E = {{.char_urc = 'E'}, &pos_30, &T7_M};

const urc_struct_t S6_R ={{.char_urc = 'R'}, &S7_E, &U6_S};


const urc_struct_t pos_14 ={{.div_char ={POS_14, 1}}, NULL, NULL};          //**pos14
const urc_struct_t R10_SPACE ={{.char_urc = ' '}, &pos_14, NULL};
const urc_struct_t R9_COLON ={{.char_urc = ':'}, &R10_SPACE, NULL};
const urc_struct_t R8_D ={{.char_urc = 'D'}, &R9_COLON, NULL};
const urc_struct_t R7_W ={{.char_urc = 'W'}, &R8_D, NULL};

const urc_struct_t pos_29 ={{.div_char ={POS_29, 1}}, NULL, NULL};          //**pos29
const urc_struct_t Q7_U ={{.char_urc = 'U'}, &pos_29, &R7_W};

const urc_struct_t Q6_P ={{.char_urc = 'P'}, &Q7_U, &S6_R};


const urc_struct_t pos_28 ={{.div_char ={POS_28, 1}}, NULL, NULL};          //**pos28
const urc_struct_t P6_O ={{.char_urc = 'O'}, &pos_28, &Q6_P};


const urc_struct_t pos_27 ={{.div_char ={POS_27, 1}}, NULL, NULL};          //**pos27
const urc_struct_t O7_K ={{.char_urc = 'K'}, &pos_27, NULL};

const urc_struct_t pos_13 ={{.div_char ={POS_13, 1}}, NULL, NULL};          //**pos13
const urc_struct_t N11_SPACE ={{.char_urc = ' '}, &pos_13, NULL};
const urc_struct_t N10_COLON ={{.char_urc = ':'}, &N11_SPACE, NULL};
const urc_struct_t N9_M ={{.char_urc = 'M'}, &N10_COLON, NULL};
const urc_struct_t N8_T ={{.char_urc = 'T'}, &N9_M, NULL};
const urc_struct_t N7_D ={{.char_urc = 'D'}, &N8_T, &O7_K};

const urc_struct_t N6_M ={{.char_urc = 'M'}, &N7_D, &P6_O};

const urc_struct_t pos_26 ={{.div_char ={POS_26, 1}}, NULL, NULL};          //**pos26
const urc_struct_t M6_L ={{.char_urc = 'L'}, &pos_26, &N6_M};

const urc_struct_t pos_25 ={{.div_char ={POS_25, 1}}, NULL, NULL};          //**pos25
const urc_struct_t L6_G ={{.char_urc = 'G'}, &pos_25, &M6_L};

const urc_struct_t pos_24 ={{.div_char ={POS_24, 1}}, NULL, NULL};          //**pos24
const urc_struct_t K6_D ={{.char_urc = 'D'}, &pos_24, &L6_G};


const urc_struct_t pos_23 ={{.div_char ={POS_23, 1}}, NULL, NULL};          //**pos23
const urc_struct_t J7_W ={{.char_urc = 'W'}, &pos_23, NULL};
const urc_struct_t pos_22 ={{.div_char ={POS_22, 1}}, NULL, NULL};          //**pos22
const urc_struct_t I7_L ={{.char_urc = 'L'}, &pos_22, &J7_W};

const urc_struct_t I6_C ={{.char_urc = 'C'}, &I7_L, &K6_D};


const urc_struct_t I5_P ={{.char_urc = 'P'}, &I6_C, NULL};
const urc_struct_t I4_T ={{.char_urc = 'T'}, &I5_P, NULL};
const urc_struct_t I3_F ={{.char_urc = 'F'}, &I4_T, &W3_I}; //F



const urc_struct_t I2_Q ={{.char_urc = 'Q'}, &I3_F, NULL}; //*Q





const urc_struct_t pos_21 ={{.div_char ={POS_21, 1}}, NULL, NULL};          //**pos21
const urc_struct_t H3_R ={{.char_urc = 'R'}, &pos_21, NULL};


const urc_struct_t pos_76 ={{.div_char ={POS_76, 1}}, NULL, NULL};          //**pos76

const urc_struct_t G5_N ={{.char_urc = 'N'}, &pos_76, NULL};
const urc_struct_t G4_I ={{.char_urc = 'I'}, &G5_N, NULL};

const urc_struct_t G3_P ={{.char_urc = 'P'}, &G4_I, &H3_R};


const urc_struct_t pos_72 ={{.div_char ={POS_72, 1}}, NULL, NULL};          //**pos72
const urc_struct_t F5_I ={{.char_urc = 'I'}, &pos_72, NULL};
const urc_struct_t F4_T ={{.char_urc = 'T'}, &F5_I, NULL};

const urc_struct_t F3_M ={{.char_urc = 'M'}, &F4_T, &G3_P};


const urc_struct_t pos_19 ={{.div_char ={POS_19, 1}}, NULL, NULL};          //**pos19
const urc_struct_t E3_F ={{.char_urc = 'F'}, &pos_19, &F3_M};


const urc_struct_t pos_71 ={{.div_char ={POS_71, 1}}, NULL, NULL};          //**pos71
const urc_struct_t D5_I ={{.char_urc = 'I'}, &pos_71, NULL};
const urc_struct_t D4_S ={{.char_urc = 'S'}, &D5_I, NULL};

const urc_struct_t D3_D ={{.char_urc = 'D'}, &D4_S, &E3_F};


const urc_struct_t D2_C ={{.char_urc = 'C'}, &D3_D, &I2_Q}; //*C



//const urc_struct_t pos_20 ={{.div_char ={POS_20, 1}}, NULL, NULL};          //**pos20
//const urc_struct_t BV_Less={{.char_urc = '>'}, &pos_20, NULL};
//
//
//
//
//const urc_struct_t D1_PLUS={{.char_urc = '+'}, &D2_C, &BV_Less};

const urc_struct_t D1_PLUS={{.char_urc = '+'}, &D2_C, NULL};


const urc_struct_t pos_1={{.div_char ={POS_1, 1}}, NULL, NULL};       //**pos1
const urc_struct_t C3_W={{.char_urc = 'W'}, &pos_1, NULL};
const urc_struct_t C2_O={{.char_urc = 'O'}, &C3_W, NULL};
const urc_struct_t C1_P={{.char_urc = 'P'}, &C2_O, &D1_PLUS};


const urc_struct_t pos_0={{.div_char ={ POS_0, 1}}, NULL, NULL};       //**pos0
const urc_struct_t B3_Y={{.char_urc = 'Y'}, &pos_0, NULL};
const urc_struct_t B2_D={{.char_urc = 'D'}, &B3_Y, NULL};
const urc_struct_t B1_R={{.char_urc = 'R'}, &B2_D, &C1_P};



urc_struct_t const *curr_char = &B1_R;


void *vec_ptr[] = {};

int16_t temp = 0;

uint16_t *temp_ptr_u16_t = NULL;
uint8_t *temp_ptr_u8_t = NULL;
uint8_t first_r = 0;

volatile bool active_urc=false;

extern urc_received_status_t CM_Q03_General_Get_Ready_Status_Flag(void);
extern void CM_Q03_General_Clear_Ready_Status_Flag(void);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


uint8_t character_response_end = CHARACTER_RESPONSE_END;
uint8_t character_command_end = CHARACTER_COMMAND_END;
 uint8_t command_comma = ',';
 uint8_t command_quotes = '"';
 uint8_t response_ok[2] = "OK";
 uint8_t general_set_baudrate[7]={"AT+IPR="};

extern volatile bool pit_channel_1_flag;

typedef enum{
    response_buff,
    urc_buff
}buffer_id_t;

typedef enum{
    disable,
    enable
}ap_ready_t;





typedef enum{
    Port_Output_cm_q03,        /*!< Pin is set as output*/
    Port_Input_cm_q03	       /*!< Pin is set as input*/
}port_mode_cm_q03;

typedef enum{
    Write_0_cm_q03,            /*!< Writes a logic 0*/
    Write_1_cm_q03             /*!< Writes a logic 1*/
}write_output_cm_q03;
//                  find start read    compare vector  save data  shift buffer clean buffer                        not results
enum read_urc_vector_machine{ find_end_read, find_start_read, compare_vector, save_data, shift_buffer, clean_buffer, end_urc_vector, not_results};


volatile uint8_t g_response_buff[BUFFER_SIZE];
volatile uint8_t g_urc_buff[UCR_BUFFER_SIZE];
volatile uint16_t g_count_BUFFER_SIZE = BUFFER_SIZE;
volatile uint16_t g_count_interrupt = 0;
volatile uint16_t g_count_read = 0;
volatile uint16_t g_count_valid_data = 0;
volatile uint16_t g_count_urc_interrupt = 0;
volatile uint16_t g_count_urc_read = 0;
volatile uint16_t g_count_urc_valid_data = 0;
volatile bool g_tx_finished = false;
volatile bool g_rx_finished = false;
volatile uint8_t g_transition_buff;
volatile buffer_id_t g_current_selected_buff = response_buff;
static ap_ready_t g_ap_ready_status = disable;
volatile uint8_t g_vector_ri = 0;
extern volatile uint8_t g_interrupt_ri;
extern volatile bool g_interrupt_ri_flag;


uint8_t g_cm_q03_cms[15] = "+CMS ERROR: XXX";
uint8_t g_cm_q03_cme[14] = "+CME ERROR: XX";
uint8_t g_cm_q03_error[16] = "+QIGETERROR: XXX";

static cm_baud_rate_t config_baud_rate = baud_rate_115200;

extern volatile uint32_t g_userInit_shield_baudRate;
extern volatile  bool g_userInit_shield_flag;

#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
static uart_handle_t g_uartHandle;
static uart_transfer_t g_uart_sendXfer;
static uart_transfer_t g_uart_receiveXfer;
#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
lpuart_handle_t g_cm_q01_Handle;
lpuart_transfer_t g_cm_q01_sendxfer;
lpuart_transfer_t g_cm_q01_receiveXfer;
#endif

static void CM_Q03_Init(void);
static void CM_Q03_UART_Init(void);
static cm_q03_codes_t CM_Q03_Ready(void);

static uint32_t CM_Q03_GPIO_Initialization(port_gpio_cm_q03 Port_select, bool select_input_output);
static uint32_t CM_Q03_GPIO_Write(port_gpio_cm_q03 Port_select, bool output );


static cm_q03_codes_t CM_Q03_Set_Command_Terminator_Character_ATS3(uint8_t character);
static cm_q03_codes_t CM_Q03_Set_Response_Character_ATS4(uint8_t character);
static cm_q03_codes_t CM_Q03_Echo_Control(uint8_t echo);
static cm_q03_codes_t CM_Q03_AT_Command(void);
static void Enable_Clock_port(PORT_Type *port_base);

#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
void ShielID_Control(bool state);
#endif

#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
bool Init_Lp_UART(LPUART_Type *base, lpuart_config_t LpuartConfigStruct, bool get_default_config);
uint8_t LPUART_Read_Response_Stop_read_character(uint8_t *lprxbuff, uint8_t *lprxbuff_size, uint8_t stop_character );
#endif

void CM_Q03_PIT_Initialization(void);
void CM_Q03_PIT_Start_Timer(uint32_t count_usec);
void CM_Q03_PIT_Stop_Timer(void);

void Enable_Clock_port(PORT_Type *port_base)
{

		if(port_base==PORTA){
			CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */
		}
		else if(port_base==PORTB){
			CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
		}
		else if(port_base==PORTC){
			CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
		}
		else if(port_base==PORTD){
			CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */
		}
		else if(port_base==PORTE){
			CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */
		}

}

#if(defined(ATHEN_BOARD) && (ATHEN_BOARD))
#if(defined(ATHEN_SHIELD) && ATHEN_SHIELD)
void UART2_FLEXIO_IRQHandler(void)
{
	UART_TransferHandleIRQ(UART2, &g_uartHandle);
}
#endif
#endif


struct ptr_t{

    uint8_t value;
    struct ptr_t *ptr_next;
    struct ptr_t *ptr_last;

} ptr_t;

uint16_t ncount=5;
uint8_t n_data=0;
bool n_int=false;
urc_struct_t const *ptr_urc;
int16_t sign_temp = 0;
uint16_t xmult=0;

#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
void CM_Q03_Callback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData)
{
    userData = userData;
    // Checks if the UART finish receiving
    if (kStatus_UART_RxIdle == status)
    {
        // Check whether the response buffer is selected
        if(response_buff == g_current_selected_buff){
            // Save the byte received to the buffer
            g_response_buff[g_count_interrupt] = g_transition_buff;
            // Go to the next byte of the buffer of go to the beginning
            if(BUFFER_SIZE-1 <= g_count_interrupt){
                g_count_interrupt = 0;
            }else{
                g_count_interrupt++;
            }
            if(BUFFER_SIZE == g_count_valid_data){
                g_count_read = g_count_interrupt;
            }else{
                g_count_valid_data++;
            }
        // Check whether the urc buffer is selected
        }else if(urc_buff == g_current_selected_buff){
            // Save the byte received to the buffer
            g_urc_buff[g_count_urc_interrupt] = g_transition_buff;
            // Go to the next byte of the buffer of go to the beginning
            if(BUFFER_SIZE-1 <= g_count_urc_interrupt){
                g_count_urc_interrupt = 0;
            }else{
                g_count_urc_interrupt++;
            }
            if(BUFFER_SIZE == g_count_urc_valid_data){
                g_count_urc_read = g_count_urc_interrupt;
            }else{
                g_count_urc_valid_data++;
            }
        }



        //TODO: Adapt comment for use in Athen
//        if(active_urc)
//        {
            // Callback
            if(!(curr_char->data.div_char.msb))
            {
                while((curr_char->data.char_urc != g_transition_buff) && (curr_char->right != NULL))
                {
                    curr_char = curr_char->right;
                }


                if(curr_char->data.char_urc == g_transition_buff)
                {
                    curr_char = curr_char->down;
                }

                else if(curr_char->right == NULL)
                {
                    curr_char = &B1_R;
                }

                if((curr_char->data.div_char.msb))
                {
                    temp_ptr_u16_t = (uint16_t*)buff_vector[(curr_char->data.div_char.lsb)];
                    temp_ptr_u8_t = (uint8_t*)temp_ptr_u16_t;
                    n_int=true;


                }
            }
            else
            {
                if((curr_char->data.div_char.lsb) >= MIX_PARAMS)
                {

                    if((g_transition_buff >= '0') && (g_transition_buff <= '9'))
                    {
                        temp = MULT_BY_10(temp) + (g_transition_buff - '0');
                    }
                    //vec_ptr[curr_char.data.div_char.lsb]
                    else if((g_transition_buff >= 'A') && (g_transition_buff <= 'Z'))
                    {
                        temp += ((g_transition_buff - 'A') + 1);
                    }

                    else if((g_transition_buff == '.') || (g_transition_buff == ','))
                    {
                        *temp_ptr_u16_t = temp;
                        temp = 0;
                        temp_ptr_u16_t++;
                    }
                    else if(g_transition_buff == '\r')
                    {
                        *temp_ptr_u16_t = temp;
                        temp = 0;
                        urc_flags[DIV_BY_8(curr_char->data.div_char.lsb)] |= (1 <<
                                MODULE_8_SFT(curr_char->data.div_char.lsb));
                        curr_char = &B1_R;
                    }
                }
                else if((curr_char->data.div_char.lsb) >= NUM_PARAMS)
                {
                    //vec_ptr[curr_char.data.div_char.lsb]
                    if((g_transition_buff >= '0') && (g_transition_buff <= '9'))
                    {
                        temp = MULT_BY_10(temp) + (g_transition_buff - '0');
                    }
                    else if((g_transition_buff == '.') || (g_transition_buff == ','))
                    {
                        *temp_ptr_u16_t = (temp ^ sign_temp) + (sign_temp & 0x01);
                        temp = 0;
                        temp_ptr_u16_t++;
                        sign_temp = 0;
                    }
                    else if(g_transition_buff == '\r')
                    {
                        *temp_ptr_u16_t = (temp ^ sign_temp) + (sign_temp & 0x0001);
                        temp = 0;
                        urc_flags[DIV_BY_8(curr_char->data.div_char.lsb)] |= (1 <<
                                MODULE_8_SFT(curr_char->data.div_char.lsb));
                        curr_char = &B1_R;
                        sign_temp = 0;
                    }

                    else if(g_transition_buff == '-')
                    {
                        sign_temp = 0xFFFF;
                    }
                }
                else if((curr_char->data.div_char.lsb) >= NOTDEF_PARAMS)
                {
                    if((g_transition_buff == character_response_end)  && (first_r == character_command_end))                    {
                        first_r = 0;
                        urc_flags[DIV_BY_8(curr_char->data.div_char.lsb)] |= (1 <<
                                MODULE_8_SFT(curr_char->data.div_char.lsb));
                        curr_char = &B1_R;
                    }

                    else
                    {
                        first_r = g_transition_buff;
                        *temp_ptr_u8_t = g_transition_buff;
                        temp_ptr_u8_t ++;
                    }
                }

                else
                {
                    *temp_ptr_u16_t |=  (1 << curr_char->data.div_char.lsb);
                    urc_flags[DIV_BY_8(curr_char->data.div_char.lsb)] |= (1 <<
                            MODULE_8_SFT(curr_char->data.div_char.lsb));
                    curr_char = &B1_R;
                }
            }
//        }

//        n_data++;
//        n_int=true;

//        for(uint8_t xcount=0; xcount < ncount; xcount++){
//
//            xmult=(xmult+1)*666666;
//            ptr_urc= &B1_R;
//            ptr_urc= ptr_urc->down;
//        }
            g_rx_finished = true;
            g_uart_receiveXfer.data = &g_transition_buff;
            g_uart_receiveXfer.dataSize = 1;
            UART_TransferReceiveNonBlocking(UART2, &g_uartHandle, &g_uart_receiveXfer, NULL);



    }else if(kStatus_UART_TxIdle == status)
    {
        g_tx_finished = true;
    }

}
#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
void CM_Q03_Callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    userData = userData;
    // Checks if the UART finish receiving
    if (kStatus_LPUART_RxIdle == status)
    {
    	// Check whether the response buffer is selected
    	if(response_buff == g_current_selected_buff){
    		// Save the byte received to the buffer
    		g_response_buff[g_count_interrupt] = g_transition_buff;
    		// Go to the next byte of the buffer of go to the beginning
        	if(BUFFER_SIZE-1 <= g_count_interrupt){
        		g_count_interrupt = 0;
        	}else{
        		g_count_interrupt++;
        	}
        	if(BUFFER_SIZE == g_count_valid_data){
        	    g_count_read = g_count_interrupt;
        	}else{
        	    g_count_valid_data++;
        	}
        // Check whether the urc buffer is selected
    	}else if(urc_buff == g_current_selected_buff){
    		// Save the byte received to the buffer
    		g_urc_buff[g_count_urc_interrupt] = g_transition_buff;
    		// Go to the next byte of the buffer of go to the beginning
        	if(BUFFER_SIZE-1 <= g_count_urc_interrupt){
        		g_count_urc_interrupt = 0;
        	}else{
        		g_count_urc_interrupt++;
        	}
            if(BUFFER_SIZE == g_count_urc_valid_data){
                g_count_urc_read = g_count_urc_interrupt;
            }else{
                g_count_urc_valid_data++;
            }
    	}




        //TODO: Adapt comment for use in Athen
//        if(active_urc)
//        {
            // Callback
            if(!(curr_char->data.div_char.msb))
            {
                while((curr_char->data.char_urc != g_transition_buff) && (curr_char->right != NULL))
                {
                    curr_char = curr_char->right;
                }


                if(curr_char->data.char_urc == g_transition_buff)
                {
                    curr_char = curr_char->down;
                }

                else if(curr_char->right == NULL)
                {
                    curr_char = &B1_R;
                }

                if((curr_char->data.div_char.msb))
                {
                    temp_ptr_u16_t = (uint16_t*)buff_vector[(curr_char->data.div_char.lsb)];
                    temp_ptr_u8_t = (uint8_t*)temp_ptr_u16_t;
                    n_int=true;


                }
            }
            else
            {
                if((curr_char->data.div_char.lsb) >= MIX_PARAMS)
                {

                    if((g_transition_buff >= '0') && (g_transition_buff <= '9'))
                    {
                        temp = MULT_BY_10(temp) + (g_transition_buff - '0');
                    }
                    //vec_ptr[curr_char.data.div_char.lsb]
                    else if((g_transition_buff >= 'A') && (g_transition_buff <= 'Z'))
                    {
                        temp += ((g_transition_buff - 'A') + 1);
                    }

                    else if((g_transition_buff == '.') || (g_transition_buff == ','))
                    {
                        *temp_ptr_u16_t = temp;
                        temp = 0;
                        temp_ptr_u16_t++;
                    }
                    else if(g_transition_buff == '\r')
                    {
                        *temp_ptr_u16_t = temp;
                        temp = 0;
                        urc_flags[DIV_BY_8(curr_char->data.div_char.lsb)] |= (1 <<
                                MODULE_8_SFT(curr_char->data.div_char.lsb));
                        curr_char = &B1_R;
                    }
                }
                else if((curr_char->data.div_char.lsb) >= NUM_PARAMS)
                {
                    //vec_ptr[curr_char.data.div_char.lsb]
                    if((g_transition_buff >= '0') && (g_transition_buff <= '9'))
                    {
                        temp = MULT_BY_10(temp) + (g_transition_buff - '0');
                    }
                    else if((g_transition_buff == '.') || (g_transition_buff == ','))
                    {
                        *temp_ptr_u16_t = (temp ^ sign_temp) + (sign_temp & 0x01);
                        temp = 0;
                        temp_ptr_u16_t++;
                        sign_temp = 0;
                    }
                    else if(g_transition_buff == '\r')
                    {
                        *temp_ptr_u16_t = (temp ^ sign_temp) + (sign_temp & 0x0001);
                        temp = 0;
                        urc_flags[DIV_BY_8(curr_char->data.div_char.lsb)] |= (1 <<
                                MODULE_8_SFT(curr_char->data.div_char.lsb));
                        curr_char = &B1_R;
                        sign_temp = 0;
                    }

                    else if(g_transition_buff == '-')
                    {
                        sign_temp = 0xFFFF;
                    }
                }
                else if((curr_char->data.div_char.lsb) >= NOTDEF_PARAMS)
                {
                    if((g_transition_buff == character_response_end)  && (first_r == character_command_end))
                    {
                        first_r = 0;
                        urc_flags[DIV_BY_8(curr_char->data.div_char.lsb)] |= (1 <<
                                MODULE_8_SFT(curr_char->data.div_char.lsb));
                        curr_char = &B1_R;
                    }

                    else
                    {
                        first_r = g_transition_buff;
                        *temp_ptr_u8_t = g_transition_buff;
                        temp_ptr_u8_t ++;
                    }
                }

                else
                {
                    *temp_ptr_u16_t |=  (1 << curr_char->data.div_char.lsb);
                    urc_flags[DIV_BY_8(curr_char->data.div_char.lsb)] |= (1 <<
                            MODULE_8_SFT(curr_char->data.div_char.lsb));
                    curr_char = &B1_R;
                }
            }


    	g_rx_finished = true;
        g_cm_q01_receiveXfer.data = &g_transition_buff;
        g_cm_q01_receiveXfer.dataSize = 1;
        LPUART_TransferReceiveNonBlocking(LPUART0, &g_cm_q01_Handle, &g_cm_q01_receiveXfer, NULL);
    }else if(kStatus_LPUART_TxIdle == status)
    {
    	g_tx_finished = true;
    }

}
#endif

bool CM_Q03_RI_Get_State(void)
{

    return g_interrupt_ri_flag;
}

void CM_Q03_RI_Clean_flag(void)
{

    g_interrupt_ri_flag=false;
}

cm_q03_codes_t CM_Q03_Set_Baud_Rate(cm_baud_rate_t Baud_Rate)
{

    uint16_t status = 0;
    uint8_t temp_buff[6] = {0};
    uint16_t temp_buffer_size = 0;
    uint8_t cmd_buff[30] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint16_t rxbuffer_size ;

    if( 9600 == Baud_Rate || 19200 == Baud_Rate
        || 38400 == Baud_Rate  || 57600 == Baud_Rate
        || 115200 == Baud_Rate || 230400 == Baud_Rate
        || 460800 == Baud_Rate || 921600 == Baud_Rate)
     {
    	config_baud_rate = Baud_Rate;
    	g_userInit_shield_baudRate = Baud_Rate;

        if(9600 == Baud_Rate)
        {
            temp_buff[0]='9';
            temp_buff[1]='6';
            temp_buff[2]='0';
            temp_buff[3]='0';
            temp_buffer_size=4;
        }
        else if(19200 == Baud_Rate)
        {
            temp_buff[0]='1';
            temp_buff[1]='9';
            temp_buff[2]='2';
            temp_buff[3]='0';
            temp_buff[4]='0';
            temp_buffer_size=5;
        }
        else if(38400 == Baud_Rate)
        {
            temp_buff[0]='3';
            temp_buff[1]='8';
            temp_buff[2]='4';
            temp_buff[3]='0';
            temp_buff[4]='0';
            temp_buffer_size=5;
        }
        else if(57600 == Baud_Rate)
        {
            temp_buff[0]='5';
            temp_buff[1]='7';
            temp_buff[2]='6';
            temp_buff[3]='0';
            temp_buff[4]='0';
            temp_buffer_size=5;
        }
        else if(115200 == Baud_Rate)
        {
            temp_buff[0]='1';
            temp_buff[1]='1';
            temp_buff[2]='5';
            temp_buff[3]='2';
            temp_buff[4]='0';
            temp_buff[5]='0';
            temp_buffer_size=6;
        }
        else if(230400 == Baud_Rate)
        {
            temp_buff[0]='2';
            temp_buff[1]='3';
            temp_buff[2]='0';
            temp_buff[3]='4';
            temp_buff[4]='0';
            temp_buff[5]='0';
            temp_buffer_size=6;
        }
        else if(460800 == Baud_Rate)
        {
            temp_buff[0]='4';
            temp_buff[1]='6';
            temp_buff[2]='0';
            temp_buff[3]='8';
            temp_buff[4]='0';
            temp_buff[5]='0';
            temp_buffer_size=6;
        }
        else if(921600 == Baud_Rate)
        {
            temp_buff[0]='9';
            temp_buff[1]='2';
            temp_buff[2]='1';
            temp_buff[3]='6';
            temp_buff[4]='0';
            temp_buff[5]='0';
            temp_buffer_size=6;
        }
      }

    Concatenate_Command(cmd_buff, general_set_baudrate, sizeof(general_set_baudrate), &cmd_buff_size);
    Concatenate_Command(cmd_buff, temp_buff, temp_buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    status = CM_Q03_Write(cmd_buff, cmd_buff_size, CM_Q03_TIMEOUT);
    status = CM_Q03_Read(rxbuff, &rxbuffer_size, 600000);

    if(Timeout == status)
    {
        return status;
    }

    status = CM_Q03_Compare_short_response(rxbuff, rxbuffer_size, response_ok,
                                     sizeof(response_ok), CM_Q03_TIMEOUT);

    if(Timeout == status)
    {
        return status;
    }
    else if(Success != status)
    {
        status = CM_Q03_Get_Response(rxbuff, temp_buffer_size);
        return status+FILE_CODES;
    }

#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
    CM_Q03_Delay(100000);

//    UART_Deinit(UART2);

//    CM_Q01_Delay(100000);

//    uart_config_t uartConfigStruct;
//    uartConfigStruct.baudRate_Bps = (uint32_t)Baud_Rate;
//    uartConfigStruct.parityMode = kUART_ParityDisabled;
//    uartConfigStruct.enableTx = true;
//    uartConfigStruct.enableRx = true;
    g_userInit_shield_baudRate = Baud_Rate;

    UART_SetBaudRate(UART2, (uint32_t)Baud_Rate, CLOCK_GetFreq(kCLOCK_BusClk));
//    UART_Init(UART2, &uartConfigStruct,  CLOCK_GetFreq(kCLOCK_BusClk));
//
//    UART_TransferCreateHandle(UART2, &g_uartHandle, CM_Q01_Callback, NULL);
//
//    g_uart_receiveXfer.data = &g_transition_buff;
//    g_uart_receiveXfer.dataSize = 1;
//
//    UART_TransferReceiveNonBlocking(UART2, &g_uartHandle, &g_uart_receiveXfer, NULL);

#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))

    g_userInit_shield_baudRate = Baud_Rate;

	CLOCK_SetLpuart0Clock(0x03U);
    LPUART_SetBaudRate(LPUART0, (uint32_t)Baud_Rate, CLOCK_GetFreq(kCLOCK_McgInternalRefClk));
#endif

    CM_Q03_Delay(100000);
    return Success;
}

cm_baud_rate_t CM_Q03_Get_Baud_Rate(void)
{
	return config_baud_rate;
}

static void CM_Q03_Init(void)
{
#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
    gpio_pin_config_t pin_config;
    CLOCK_EnableClock(kCLOCK_PortC);
    PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAsGpio);
    pin_config.pinDirection= kGPIO_DigitalInput;
    pin_config.outputLogic= true;

    GPIO_PinInit(GPIOC, PIN3_IDX, &pin_config);

    const port_pin_config_t portc_config = {
        kPORT_PullDown,
        kPORT_FastSlewRate,
        kPORT_PassiveFilterEnable,
        kPORT_LowDriveStrength,
        kPORT_MuxAsGpio
    };

    PORT_SetPinInterruptConfig(PORTC, PIN3_IDX, kPORT_InterruptRisingEdge );
    PORT_SetPinConfig(PORTC, PIN3_IDX, &portc_config);
    EnableIRQ(PORTC_PORTD_IRQn);
#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))


    CM_Q03_GPIO_Initialization(Port_AP_Ready,Port_Output_cm_q03);
    CM_Q03_GPIO_Write(Port_AP_Ready, Write_1_cm_q03);
//
//    CM_Q03_GPIO_Initialization(Port_NW_Disable,Port_Output_cm_q03);
//    CM_Q03_GPIO_Write(Port_NW_Disable, Write_0_cm_q03);
//
//    CM_Q03_GPIO_Initialization(Port_RI_EG915,Port_Input_cm_q03);
//    CM_Q03_GPIO_Initialization(Port_DTR,Port_Input_cm_q03);
//    CM_Q03_GPIO_Initialization(Port_PSM_IND,Port_Input_cm_q03);
//    CM_Q03_GPIO_Initialization(Port_Status,Port_Input_cm_q03);



#endif
}

static void CM_Q03_UART_Init(void)
{
#if(defined(ATHEN_BOARD) && ATHEN_BOARD)

	uart_config_t uartConfigStruct;

     uartConfigStruct.baudRate_Bps = 115200U;
     uartConfigStruct.parityMode = kUART_ParityDisabled;
     uartConfigStruct.enableTx = true;
     uartConfigStruct.enableRx = true;

     Enable_Clock_port(PORTD);

     PORT_SetPinMux(PORTD, 2, kPORT_MuxAlt3);
     PORT_SetPinMux(PORTD, 3, kPORT_MuxAlt3);
     UART_Init(UART2, &uartConfigStruct,  CLOCK_GetFreq(kCLOCK_BusClk));

     UART_TransferCreateHandle(UART2, &g_uartHandle, CM_Q03_Callback, NULL);

     g_uart_receiveXfer.data = &g_transition_buff;
     g_uart_receiveXfer.dataSize = 1;

     UART_TransferReceiveNonBlocking(UART2, &g_uartHandle, &g_uart_receiveXfer, NULL);

     g_userInit_shield_baudRate = uartConfigStruct.baudRate_Bps = 115200U;
     g_userInit_shield_flag = true;
#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))

 	 Enable_Clock_port(PORTA);
 	 PORT_SetPinMux(PORTA,PIN1_IDX,kPORT_MuxAlt2);
 	 PORT_SetPinMux(PORTA,PIN2_IDX,kPORT_MuxAlt2);

	 CLOCK_SetLpuart0Clock(0x03);

	 lpuart_config_t uartConfigStruct1;
	 LPUART_GetDefaultConfig(&uartConfigStruct1);
	 g_userInit_shield_baudRate=uartConfigStruct1.baudRate_Bps;
	 uartConfigStruct1.enableRx=true;
	 uartConfigStruct1.enableTx=true;

	 LPUART_Init(LPUART0, &uartConfigStruct1, CLOCK_GetFreq(kCLOCK_McgInternalRefClk));

	 LPUART_TransferCreateHandle(LPUART0, &g_cm_q01_Handle, CM_Q03_Callback, NULL);

	 g_cm_q01_receiveXfer.data = &g_transition_buff;
	 g_cm_q01_receiveXfer.dataSize = 1;
	 LPUART_TransferReceiveNonBlocking(LPUART0, &g_cm_q01_Handle, &g_cm_q01_receiveXfer, NULL);
     g_userInit_shield_flag = true;
#endif
}

cm_q03_codes_t CM_Q03_Control(cm_q03_control_t shield_command_control)
{
	cm_q03_codes_t retval;
	uint8_t retries = 0;

	#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
    uart_config_t uartConfigStruct =
    {
    		115200U,
			kUART_ParityDisabled,
			true,
			true
    };
	#endif
	#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
	 lpuart_config_t lpuartConfigStruct;
	 LPUART_GetDefaultConfig(&lpuartConfigStruct);
	 lpuartConfigStruct.enableRx=true;
	 lpuartConfigStruct.enableTx=true;
	#endif

	switch(shield_command_control)
    {
        case cm_q03_turn_on:

			#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
                ShielID_Control(false);
			#endif

            CM_Q03_GPIO_Initialization(Port_VDD_EG915,Port_Output_cm_q03);
            CM_Q03_GPIO_Initialization(Port_PWR_KEY_EG915,Port_Output_cm_q03);

            CM_Q03_GPIO_Initialization(Port_RESET,Port_Output_cm_q03);

			#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
                CM_Q03_UART_Init();
                UART_Deinit(UART2);
			#endif
			#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
                CM_Q03_UART_Init();
                LPUART_Deinit(LPUART0);
			#endif
            CM_Q03_Delay(250000);
            CM_Q03_GPIO_Write(Port_VDD_EG915, Write_1_cm_q03);
            CM_Q03_GPIO_Write(Port_RESET, Write_0_cm_q03);
            CM_Q03_GPIO_Write(Port_RESET, Write_1_cm_q03);

            CM_Q03_GPIO_Write(Port_PWR_KEY_EG915, Write_0_cm_q03);

            CM_Q03_Delay(30000);
            CM_Q03_GPIO_Write(Port_PWR_KEY_EG915, Write_1_cm_q03);
            CM_Q03_Delay(1500000);
            CM_Q03_GPIO_Write(Port_PWR_KEY_EG915, Write_0_cm_q03);
            CM_Q03_Delay(3400000);
        	CM_Q03_Init();
        	CM_Q03_UART_Init();
        	CM_Q03_Delay(1000000);

            break;

        case cm_q03_turn_off:
            CM_Q03_Delay(200);
            CM_Q03_GPIO_Write(Port_PWR_KEY_EG915, Write_1_cm_q03);
            CM_Q03_Delay(650000);
            CM_Q03_GPIO_Write(Port_PWR_KEY_EG915, Write_0_cm_q03);
            CM_Q03_Delay(2000000);
            CM_Q03_GPIO_Write(Port_VDD_EG915, Write_0_cm_q03);
//            CM_Q01_Delay(200);

            retval = CM_Q03_AT_Command();
            if (Success != retval)
            {

                #if(defined(ATHEN_BOARD) && ATHEN_BOARD)
                    CM_Q03_UART_Init();
                    UART_Deinit(UART2);
                #endif
                #if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
                    CM_Q03_UART_Init();
                    LPUART_Deinit(LPUART0);

                    gpio_pin_config_t config;

                    config.pinDirection= kGPIO_DigitalOutput;
                    config.outputLogic= 0;

                    CLOCK_EnableClock(kCLOCK_PortA);

                    PORT_SetPinMux(PORTA, PIN1_IDX, kPORT_MuxAsGpio);
                    GPIO_PinInit(GPIOA,PIN1_IDX,&config);
                    PORT_SetPinMux(PORTA, PIN2_IDX, kPORT_MuxAsGpio);
                    GPIO_PinInit(GPIOA,PIN2_IDX,&config);

                    CM_Q03_GPIO_Write(Port_AP_Ready, Write_0_cm_q03); //change test to 0
//                    CM_Q03_GPIO_Write(Port_NW_Disable, Write_0_cm_q03);
//
//                    CM_Q03_GPIO_Initialization(Port_RI_EG915,Port_Output_cm_q03);
//                    CM_Q03_GPIO_Initialization(Port_DTR,Port_Output_cm_q03);
//                    CM_Q03_GPIO_Initialization(Port_PSM_IND,Port_Output_cm_q03);
//                    CM_Q03_GPIO_Initialization(Port_Status,Port_Output_cm_q03);
//
//                    CM_Q03_GPIO_Write(Port_RI_EG915, Write_0_cm_q03);
//                    CM_Q03_GPIO_Write(Port_DTR, Write_0_cm_q03);
//                    CM_Q03_GPIO_Write(Port_PSM_IND, Write_0_cm_q03);
//                    CM_Q03_GPIO_Write(Port_Status, Write_0_cm_q03);


                #endif

                g_userInit_shield_flag = false;
				character_response_end = CHARACTER_RESPONSE_END;
				character_command_end = CHARACTER_COMMAND_END;
				CM_Q03_buffer_reset();
				CM_Q03_URC_buffer_reset();

                return CM_Q03_not_ready;
            }

            break;

        case cm_q03_reset:
			CM_Q03_buffer_reset();
			CM_Q03_URC_buffer_reset();
            CM_Q03_GPIO_Write(Port_PWR_KEY_EG915, Write_1_cm_q03);
            CM_Q03_Delay(310000);
            CM_Q03_GPIO_Write(Port_PWR_KEY_EG915, Write_0_cm_q03);
            CM_Q03_Delay(5000000);
            CM_Q03_Init();
            CM_Q03_UART_Init();
            character_response_end = CHARACTER_RESPONSE_END;
        	character_command_end = CHARACTER_COMMAND_END;

            break;

        default:
            return User_Error;
    }

	do
	{
		retval = CM_Q03_Ready();
		if(CM_Q03_not_ready == retval)
		{
			#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
			UART_Deinit(UART2);
			#endif
			#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
			LPUART_Deinit(LPUART0);
			CM_Q03_Delay(500000);
			#endif
			CM_Q03_Delay(100000);
			CM_Q03_UART_Init();
			CM_Q03_Delay(500000);
			CM_Q03_buffer_reset();
			CM_Q03_URC_buffer_reset();
		}
		retries++;
	}while((3 > retries) && (CM_Q03_is_ready != retval));

	if(CM_Q03_is_ready == retval)
	{
		if(CM_Q03_Set_Response_Character_ATS4(CHARACTER_COMMAND_END)!=Success)
		{
            CM_Q03_Delay(500000);

			if(CM_Q03_Set_Response_Character_ATS4(CHARACTER_COMMAND_END)!=Success)
			{
                CM_Q03_Delay(500000);
                if(CM_Q03_Set_Response_Character_ATS4(CHARACTER_COMMAND_END)!=Success)
                {
				CM_Q03_GPIO_Write(Port_VDD_EG915, Write_0_cm_q03);
				CM_Q03_Delay(500000);
				g_userInit_shield_flag = false;
				character_response_end = CHARACTER_RESPONSE_END;
				character_command_end = CHARACTER_COMMAND_END;
				return CM_Q03_not_ready;
                }
			}
		}

		if(CM_Q03_Set_Command_Terminator_Character_ATS3(CHARACTER_COMMAND_END)!=Success)
		{
            CM_Q03_Delay(500000);

			if(CM_Q03_Set_Command_Terminator_Character_ATS3(CHARACTER_COMMAND_END)!=Success)
			{
                CM_Q03_Delay(500000);
                if(CM_Q03_Set_Command_Terminator_Character_ATS3(CHARACTER_COMMAND_END)!=Success)
                {
				CM_Q03_GPIO_Write(Port_VDD_EG915, Write_0_cm_q03);
				CM_Q03_Delay(500000);
				g_userInit_shield_flag = false;
				character_response_end = CHARACTER_RESPONSE_END;
				character_command_end = CHARACTER_COMMAND_END;
				return CM_Q03_not_ready;
			}
		}
		}
	}
	else
	{
		CM_Q03_GPIO_Write(Port_VDD_EG915, Write_0_cm_q03);
		CM_Q03_Delay(500000);
		g_userInit_shield_flag = false;
		character_response_end = CHARACTER_RESPONSE_END;
		character_command_end = CHARACTER_COMMAND_END;
		return CM_Q03_not_ready;
	}
    return retval;
}

static cm_q03_codes_t CM_Q03_Ready(void)
{
	cm_q03_codes_t status = 0;
//    uint16_t buffer_size = 0;
//    uint8_t rxbuff[15] = {0};

//    status = CM_Q01_Read(rxbuff, &buffer_size, 1000000);
    // Clean the PIT_1 flag
    active_urc=true;

	pit_channel_1_flag = false;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer(1000000);
    do{

    }while( !pit_channel_1_flag && 0 == CM_Q03_General_Get_Ready_Status_Flag());

    // Stop the count of the PIT_1
    CM_Q03_PIT_Stop_Timer();
    // Checks if the PIT_1 flag is active
    if(pit_channel_1_flag){
        // Deactivate the PIT_1 flag
        pit_channel_1_flag = false;
        // Return 1 to indicate an error
        status= Timeout;
    }

    if(Timeout == status)
    {
		status = CM_Q03_AT_Command();
		if (Success != status)
		{
			status = CM_Q03_AT_Command();
			if (Success != status)
			{
                character_response_end='\r';
                status = CM_Q03_AT_Command();
                if (Success != status)
                {
                    status = CM_Q03_AT_Command();
                    if (Success != status)
                    {
                        return CM_Q03_not_ready;
                    }
                }
//				return CM_Q03_not_ready;
			}
		}
    }else
    {
    	CM_Q03_Delay(100000);
    	CM_Q03_buffer_clean();
    }
    CM_Q03_General_Clear_Ready_Status_Flag();

	status = CM_Q03_AP_Ready(0, 500);
    if(Success != status)
    {
    	return status;
    }
    status = CM_Q03_Echo_Control(false);
    if(Success != status)
    {
    	return status;
    }
    CM_Q03_buffer_clean();
    return CM_Q03_is_ready;
}

#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
void ShielID_Control(bool state)
{

    gpio_pin_config_t config;

    CLOCK_EnableClock(kCLOCK_PortE);

    PORT_SetPinMux(PORTE, PIN31_IDX, kPORT_MuxAsGpio);

    config.pinDirection= kGPIO_DigitalOutput;
    config.outputLogic= state;

    GPIO_PinInit(GPIOE, PIN31_IDX, &config);
}
#endif

uint32_t CM_Q03_Write(uint8_t *txbuff, uint16_t size, uint32_t timeout)
{
    // Clean PIT_1 flag
    pit_channel_1_flag = false;
    // Clean transmission flag
    g_tx_finished = false;
    // Star the count for the PIT_1 flag
    CM_Q03_PIT_Start_Timer(timeout);
    // Start the transmission of data


#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
    g_uart_sendXfer.data = txbuff;
    g_uart_sendXfer.dataSize = size;
    UART_TransferSendNonBlocking(UART2, &g_uartHandle, &g_uart_sendXfer);
#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
    g_cm_q01_sendxfer.data = txbuff;
    g_cm_q01_sendxfer.dataSize = size;
	LPUART_TransferSendNonBlocking(LPUART0, &g_cm_q01_Handle, &g_cm_q01_sendxfer);
#endif

    // Waits until transmission is finish or the PIT_1 flag is active
    while(!g_tx_finished && !(pit_channel_1_flag)){}
    // Stop the count of PIT_1 flag
    CM_Q03_PIT_Stop_Timer();
    // Clean transmission flag
    g_tx_finished = false;
    // Checks if PIT_1 flag is active
    if(pit_channel_1_flag){
        // Clean PIT_1 flag
        pit_channel_1_flag = false;
        // Stop the transmission of data
#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
        UART_TransferAbortSend(UART2, &g_uartHandle);
#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
		LPUART_TransferAbortSend(LPUART0, &g_cm_q01_Handle);
#endif
        // Return 1 to indicate an error
        return Timeout;
    }
    // Clean PIT_1 flag
    pit_channel_1_flag = false;
    // Return 0 to indicates success
    return Success;
}

uint32_t CM_Q03_Read_Until_OK(uint8_t *rx_buff, uint16_t *size, uint32_t timeout)
{
	uint32_t result = 0;
	uint8_t read_status = READING_DATA;

	uint16_t it_1 = g_count_read;
	uint16_t it_2 = g_count_read + 1;
	uint16_t it_3 = g_count_read + 2;
	uint16_t it_4 = g_count_read + 3;
	uint16_t it_5 = g_count_read + 4;
	uint16_t it_6 = g_count_read + 5;
    uint16_t it_7 = g_count_read + 6;
    uint16_t it_8 = g_count_read + 7;

	uint8_t *search_buff = g_response_buff;

	it_2 = (BUFFER_SIZE <= it_2) ? (it_2 - BUFFER_SIZE) : (it_2);
	it_3 = (BUFFER_SIZE <= it_3) ? (it_3 - BUFFER_SIZE) : (it_3);
	it_4 = (BUFFER_SIZE <= it_4) ? (it_4 - BUFFER_SIZE) : (it_4);
	it_5 = (BUFFER_SIZE <= it_5) ? (it_5 - BUFFER_SIZE) : (it_5);
	it_6 = (BUFFER_SIZE <= it_6) ? (it_6 - BUFFER_SIZE) : (it_6);
    it_7 = (BUFFER_SIZE <= it_7) ? (it_7 - BUFFER_SIZE) : (it_7);
    it_8 = (BUFFER_SIZE <= it_8) ? (it_8 - BUFFER_SIZE) : (it_8);


	pit_channel_1_flag = false;
	CM_Q03_PIT_Start_Timer(timeout);

	(*size) = 0;

	while((!pit_channel_1_flag) && (OK_FOUND != read_status))
	{
		if(     ('\r' == search_buff[it_1])
                && ('\r' == search_buff[it_2])
                && ('\r' == search_buff[it_3])
				&& ('\r' == search_buff[it_4])
				&& ('O' == search_buff[it_5])
				&& ('K' == search_buff[it_6])
				&& ('\r' == search_buff[it_7])
				&& ('\r' == search_buff[it_8]))
		{
			read_status = OK_FOUND;
		}

		else
		{
			(*rx_buff) = search_buff[it_1];
			(*size) ++;
			rx_buff ++;
		}

		it_1 = ((BUFFER_SIZE - 1) == it_1) ? (0) : (it_1 + 1);
		it_2 = ((BUFFER_SIZE - 1) == it_2) ? (0) : (it_2 + 1);
		it_3 = ((BUFFER_SIZE - 1) == it_3) ? (0) : (it_3 + 1);
		it_4 = ((BUFFER_SIZE - 1) == it_4) ? (0) : (it_4 + 1);
		it_5 = ((BUFFER_SIZE - 1) == it_5) ? (0) : (it_5 + 1);
		it_6 = ((BUFFER_SIZE - 1) == it_6) ? (0) : (it_6 + 1);
        it_7 = ((BUFFER_SIZE - 1) == it_7) ? (0) : (it_7 + 1);
        it_8 = ((BUFFER_SIZE - 1) == it_8) ? (0) : (it_8 + 1);

	}

	CM_Q03_PIT_Stop_Timer();

	if(pit_channel_1_flag)
	{
		pit_channel_1_flag = false;
		(*size) = 0;
		result = ERROR;
	}

	else
	{
		result = Success;
	}

	return result;
}

uint32_t CM_Q03_HTTP_Read_Until_OK(uint8_t *rx_buff, uint16_t *size, uint32_t timeout)
{
	uint32_t result = 0;
	uint8_t read_status = READING_DATA;

	uint16_t it_1 = g_count_read;
	uint16_t it_2 = g_count_read + 1;
	uint16_t it_3 = g_count_read + 2;
	uint16_t it_4 = g_count_read + 3;
	uint16_t it_5 = g_count_read + 4;
	uint16_t it_6 = g_count_read + 5;

	uint8_t *search_buff = g_response_buff;

	it_2 = (BUFFER_SIZE <= it_2) ? (it_2 - BUFFER_SIZE) : (it_2);
	it_3 = (BUFFER_SIZE <= it_3) ? (it_3 - BUFFER_SIZE) : (it_3);
	it_4 = (BUFFER_SIZE <= it_4) ? (it_4 - BUFFER_SIZE) : (it_4);
	it_5 = (BUFFER_SIZE <= it_5) ? (it_5 - BUFFER_SIZE) : (it_5);
	it_6 = (BUFFER_SIZE <= it_6) ? (it_6 - BUFFER_SIZE) : (it_6);


	pit_channel_1_flag = false;
	CM_Q03_PIT_Start_Timer(timeout);

	(*size) = 0;

	while((!pit_channel_1_flag) && (OK_FOUND != read_status))
	{
		if(     ('\r' == search_buff[it_1])
                && ('\r' == search_buff[it_2])
                && ('O' == search_buff[it_3])
				&& ('K' == search_buff[it_4])
				&& ('\r' == search_buff[it_5])
				&& ('\r' == search_buff[it_6]))
		{
			read_status = OK_FOUND;
		}

		else
		{
			(*rx_buff) = search_buff[it_1];
			(*size) ++;
			rx_buff ++;

			g_count_valid_data --;
		}

		it_1 = ((BUFFER_SIZE - 1) == it_1) ? (0) : (it_1 + 1);
		it_2 = ((BUFFER_SIZE - 1) == it_2) ? (0) : (it_2 + 1);
		it_3 = ((BUFFER_SIZE - 1) == it_3) ? (0) : (it_3 + 1);
		it_4 = ((BUFFER_SIZE - 1) == it_4) ? (0) : (it_4 + 1);
		it_5 = ((BUFFER_SIZE - 1) == it_5) ? (0) : (it_5 + 1);
		it_6 = ((BUFFER_SIZE - 1) == it_6) ? (0) : (it_6 + 1);

		while((!pit_channel_1_flag) && (6 > g_count_valid_data) && (OK_FOUND != read_status));
	}

	CM_Q03_PIT_Stop_Timer();

	if(pit_channel_1_flag)
	{
		(*rx_buff) = search_buff[it_1];
		rx_buff ++;
		(*rx_buff) = search_buff[it_2];
		rx_buff ++;
		(*rx_buff) = search_buff[it_3];
		rx_buff ++;
		(*rx_buff) = search_buff[it_4];
		rx_buff ++;
		(*rx_buff) = search_buff[it_5];
		rx_buff ++;
		(*rx_buff) = search_buff[it_6];
		rx_buff ++;
		(*size) += 6;

		pit_channel_1_flag = false;
		(*size) = 0;
		result = ERROR;
	}

	else
	{
		result = Success;
	}

	return result;
}

uint32_t CM_Q03_Read(uint8_t *rxbuff, uint16_t *size, uint32_t timeout)
{
    uint16_t finder_status = 0;
    // Counter to know the number of character sequences found
    uint16_t temp_character_counter = 0;
    // Pointer to the buffer where the character will be search
    uint8_t *search_buffer;
    // Counter for the number of byte that are read
    uint16_t size_counter = 0;
    // Select to use the response buffer
    search_buffer = g_response_buff;
    // Clean the PIT_1 flag
    pit_channel_1_flag = false;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer(timeout);
    // Read the buffer until 2 sequences are found or until the PIT_1 flag is active
    while(!(pit_channel_1_flag) && (2 != temp_character_counter)){
        // Waits until 2 characters are receive in the buffer or until the PIT_1 flag is active
        while((2 > g_count_valid_data) && !(pit_channel_1_flag)){}
        // If the PIT_1 flag is active, stop reading the buffer
        if(pit_channel_1_flag){
            break;
        }
        finder_status = Character_finder(search_buffer, g_count_read, character_response_end,BUFFER_SIZE);
        // Look for the character sequence
        if(0 == finder_status){
            // Increase the counter
            temp_character_counter++;
            if(BUFFER_SIZE-1 == g_count_read){
                g_count_read = 1;
            }else if(BUFFER_SIZE-2 == g_count_read){
                g_count_read = 0;
            }else{
                g_count_read += 2;
            }
            g_count_valid_data -=2 ;
        // Check if only one sequence has been found
        }else if(1 == temp_character_counter){
            // Copy the byte to the buffer
            *rxbuff = search_buffer[g_count_read];
            // Go to the next position of the buffer
            rxbuff++;
            // Increase the read counter
            if(BUFFER_SIZE-1 == g_count_read){
                g_count_read = 0;
            }else{
                g_count_read++;
            }
            // Increase the size counter
            size_counter++;
            g_count_valid_data--;
        }else
        {
            // Increase the read counter
            if(BUFFER_SIZE-1 == g_count_read){
                g_count_read = 0;
            }else{
                g_count_read++;
            }
            g_count_valid_data--;
        }
    }
    // Stop the count of the PIT_1
    CM_Q03_PIT_Stop_Timer();
    // Checks if the PIT_1 flag is active
    if(pit_channel_1_flag){
        // Deactivate the PIT_1 flag
        pit_channel_1_flag = false;
        //  Assign the number of byte read to the parameter
        *size = size_counter;
        // Return 1 to indicate an error
        return Timeout;
    }else{
        // Assign the number of byte read to the parameter
        *size = size_counter;
        // Return 0 to indicate successful execution
        return Success;
    }
}

uint32_t Character_finder(uint8_t *ptr_buffer,
                      uint16_t index_of_buffer,
                      uint8_t character_start_end,
                      uint16_t buffer_size
                      )
{
    // Checks if the index is in a correct range
    if(buffer_size > index_of_buffer){
        // Checks if the character is the one you are looking for
        if(character_command_end == ptr_buffer[index_of_buffer]){
            // Go to the next byte of the buffer
                // if there is the last byte of the buffer, go to the beginning of the buffer
            if(buffer_size-1 == index_of_buffer){
                index_of_buffer = 0;
            }else{
                index_of_buffer++;
            }
            // Checks if the character is the next character of the sequence
            if(character_start_end == ptr_buffer[index_of_buffer]){
                // Return 0 to indicate that the sequence were found successfully
                return Success;
            }else{
                // Return 1 to indicate that the sequence were not found
                return 1;
            }
        }else{
            return 1;
        }
    }else{
        // Return 2 to indicate an error
        return 2;
    }
}

void CM_Q03_buffer_clean()
{

    g_count_read = g_count_interrupt;
    g_count_valid_data = 0;

}

void CM_Q03_URC_buffer_clean()
{
    g_count_urc_read = g_count_urc_interrupt;
    g_count_urc_valid_data = 0;

}

void CM_Q03_buffer_reset()
{

	g_count_interrupt = 0;
	g_count_read = 0;
	g_count_valid_data = 0;
}

void CM_Q03_URC_buffer_reset()
{
	g_count_urc_interrupt = 0;
	g_count_urc_read = 0;
	g_count_urc_valid_data = 0;
}

//This function is used to compare the similarity between two arrays. There are
//two operation modes:
//the first one is by entering the full buffer size in the valide character count parameter, which
//will compare the entirity of the buffers, and the response will be equal, not equal, or timeout.
//The second one, is performed by passing a value smaller than the buffer size in the valide character
//count parameter, which will initially evaluate the similarity between the first valide_character_count parameter,
//if they are not equal, the response will be not equal, if they are equal, it will continue evaluating the similarity;
//from this point, if there is any difference between the buffers, it will response similar as response, otherwise,
//the function will respond equal; it can respond timeout, too. In this second mode, considering that the first
//valide_character_buffer characters are equal, the position of the last character where such comparison was done
//is returned in the finish_count output parameter.
uint32_t CM_Q03_Compare_response(uint8_t *buffer_to_be_compare,
                              uint16_t size_buffer_to_be_compare,
                              uint8_t *compare_buffer,
                              uint16_t size_compare_buffer,
                              uint16_t valide_character_count,
                              uint16_t *finish_count,
                              uint32_t time_out)
{
    uint16_t count_size=0;
    uint16_t count_to_valide_character=0;
    uint8_t *principal_compare_ptr;

    principal_compare_ptr=compare_buffer;

    CM_Q03_PIT_Start_Timer(time_out);

    while(((size_buffer_to_be_compare-count_size)>=size_compare_buffer || count_to_valide_character) && !(pit_channel_1_flag) ){
        if(*buffer_to_be_compare == *compare_buffer){
            count_to_valide_character++;
            if(count_to_valide_character == valide_character_count){
                *finish_count = count_size+1;
            }
            if(count_to_valide_character==size_compare_buffer){
                CM_Q03_PIT_Stop_Timer();
                pit_channel_1_flag=false;
                return Success;
            }
            count_size++;
            buffer_to_be_compare++;
            compare_buffer++;
        }else if(*principal_compare_ptr == *compare_buffer){
            count_size++;
            buffer_to_be_compare++;
        }else{
            if(count_to_valide_character >= valide_character_count){
                CM_Q03_PIT_Stop_Timer();
                pit_channel_1_flag=false;
                return Timeout;
            }
            count_to_valide_character=0;
            compare_buffer=principal_compare_ptr;
        }
    }

    CM_Q03_PIT_Stop_Timer();

    if(pit_channel_1_flag){
        pit_channel_1_flag=false;
        return Timeout;
    }
    else{
        pit_channel_1_flag=false;
        return NOT_SIMILAR_AT_ALL;
    }
}



uint32_t CM_Q03_Compare_short_response(uint8_t *buffer_to_be_compare,
                              uint16_t size_buffer_to_be_compare,
                              uint8_t *compare_buffer,
                              uint16_t size_compare_buffer,
                              uint32_t time_out)
{
    uint16_t count_size=0;
    uint16_t count_to_valide_character=0;
    uint8_t *principal_compare_ptr;

    principal_compare_ptr=compare_buffer;

    CM_Q03_PIT_Start_Timer(time_out);

    while(((size_buffer_to_be_compare-count_size)>=size_compare_buffer || count_to_valide_character) && !(pit_channel_1_flag) ){
        if(*buffer_to_be_compare == *compare_buffer){
            count_to_valide_character++;

            if(count_to_valide_character==size_compare_buffer){
                CM_Q03_PIT_Stop_Timer();
                pit_channel_1_flag=false;
                return Success;
            }
            count_size++;
            buffer_to_be_compare++;
            compare_buffer++;
        }else if(*principal_compare_ptr == *compare_buffer){
            count_size++;
            buffer_to_be_compare++;
        }else{

            count_to_valide_character=0;
            compare_buffer=principal_compare_ptr;
        }
    }

    CM_Q03_PIT_Stop_Timer();

    if(pit_channel_1_flag){
        pit_channel_1_flag=false;
        return Timeout;
    }
    else{
        pit_channel_1_flag=false;
        return NOT_SIMILAR_AT_ALL;
    }
}


cm_q03_codes_t CM_Q03_Delay(uint32_t delay)
{
	if(MIN_DELAY > delay)
	{
		return User_Error;
	}
    pit_channel_1_flag=false;
    CM_Q03_PIT_Initialization();
    CM_Q03_PIT_Start_Timer(delay);
    // Wait the PIT interruption
    while(!pit_channel_1_flag){}
    // Stop PIT count
    CM_Q03_PIT_Stop_Timer();
    pit_channel_1_flag=false;
    return Success;
}


void CM_Q03_PIT_Initialization(void)
{

    pit_config_t pitConfig;

    /* pitConfig.enableRunInDebug = false; */
    PIT_GetDefaultConfig(&pitConfig);
    /* Init pit module */
    PIT_Init(PIT, &pitConfig);
    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(PIT_IRQn);

}

// Star the timer of the PIT
void CM_Q03_PIT_Start_Timer(uint32_t count_usec)
{

    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, USEC_TO_COUNT(count_usec, CLOCK_GetFreq(kCLOCK_BusClk)));
    /* Start channel 0 */
    PIT_StartTimer(PIT, kPIT_Chnl_1);

}

// Stop the timer of the PIT
void CM_Q03_PIT_Stop_Timer(void)
{

    PIT_StopTimer(PIT, kPIT_Chnl_1);
}


cm_q03_codes_t CM_Q03_Get_Response(uint8_t *rxbuff, uint16_t size)
{
    uint8_t status;
    uint16_t index_of_command = 0;
    cm_q03_codes_t return_code = 0;

    status = CM_Q03_Compare_response(rxbuff, size, g_cm_q03_cms, sizeof(g_cm_q03_cms)-3, sizeof(g_cm_q03_cms)-3, &index_of_command, 5000000);
    if(0 == status || 1 == status)
    {
        return_code = Ascii_to_Code(&rxbuff[index_of_command], size-index_of_command);
    }else
    {
        status = CM_Q03_Compare_response(rxbuff, size, g_cm_q03_cme, sizeof(g_cm_q03_cme)-2, sizeof(g_cm_q03_cme)-2, &index_of_command, 5000000);
        if(0 == status || 1 == status)
        {
            return_code = Ascii_to_Code(&rxbuff[index_of_command], size-index_of_command);
            if(!return_code)
            {
                return_code = Phone_failure;
            }
        }else
        {
            return_code = ERROR;
        }
    }
    return return_code;
}

cm_q03_codes_t Ascii_to_Code(uint8_t *buffer, uint16_t size_buff)
{
    cm_q03_codes_t code = 0;
    uint8_t ascii_number[3] = "\0\0\0";
    uint8_t i = 0;
    uint16_t temp = 1;

    while(size_buff--)
    {
        ascii_number[i] = *buffer;
        buffer++;
        i++;
    }
    while(i--)
    {
        code += (ascii_number[i] - 0x30)*(temp);
        temp = temp*10;
    }

        // Return the code found
        return code;

}

uint64_t Ascii_to_Number(uint8_t *buffer, uint16_t size_buff)
{
    uint64_t code = 0;
    uint64_t temp = 1;

    while(size_buff--)
    {
        code += (buffer[size_buff] - 0x30)*(temp);
        temp = temp*10;
    }
    return code;

}

// compare response error offset compare
uint32_t CM_Q03_Compare_response_Error(uint8_t *buffer_to_be_compare, uint16_t size_buffer_to_be_compare,
					uint8_t *compare_buffer,uint16_t size_compare_buffer,
					uint16_t offset_error ,uint8_t max_num_compare,uint32_t time_out )
{

    uint16_t count_size=0;
    uint16_t count_to_valide_character=0;
    uint8_t *principal_compare_ptr;
    uint8_t *principal_buffer_ptr;

    uint16_t count_max_compare=0;

    principal_compare_ptr=compare_buffer;
    principal_buffer_ptr=buffer_to_be_compare;

    CM_Q03_PIT_Start_Timer(time_out);

    while(count_max_compare < max_num_compare &&!(pit_channel_1_flag)){

        while(((size_buffer_to_be_compare-count_size)>=size_compare_buffer || count_to_valide_character) && !(pit_channel_1_flag) ){
            if(*buffer_to_be_compare == *compare_buffer){
                count_to_valide_character++;
                if(count_to_valide_character==size_compare_buffer){
                    CM_Q03_PIT_Stop_Timer();
                    pit_channel_1_flag=false;
                    return (offset_error + count_max_compare);
                }
                count_size++;
                buffer_to_be_compare++;
                compare_buffer++;
            }else if(*principal_compare_ptr == *compare_buffer){
                count_size++;
                buffer_to_be_compare++;
            }else{
                count_to_valide_character=0;

            }
        }

        if(pit_channel_1_flag){
            CM_Q03_PIT_Stop_Timer();
            pit_channel_1_flag=false;
            return 3;
        }
        compare_buffer=principal_compare_ptr;
        compare_buffer+=size_compare_buffer;
        principal_compare_ptr=compare_buffer;
        buffer_to_be_compare = principal_buffer_ptr;
        count_to_valide_character=0;
        count_size=0;
        count_max_compare++;

    }

    if(pit_channel_1_flag){
        CM_Q03_PIT_Stop_Timer();
        pit_channel_1_flag=false;
        return Timeout;
    }
    else{
        CM_Q03_PIT_Stop_Timer();
        pit_channel_1_flag=false;
        return Not_found;
    }

    return Success;
}

//This function is based in a big vector which contains both valid responses, and erroneous codes alike.
//The offset_error value is very important to select from which command error range will the comparison be done.
//For example, consider that creg errors goes from 0x10 to 0x1F, and the user wants to look for a creg error present
//in the passed buffer, the user must pass as offset 0x10, and if the third creg error is encountered, then the function
//will respond with the value 0x13.
uint32_t CM_Q03_Compare_vector_response(uint8_t *buffer_to_be_compare, uint16_t size_buffer_to_be_compare,
				uint8_t *compare_buffer,uint16_t size_compare_buffer, uint16_t offset_error ,
				uint8_t max_num_compare, uint8_t count_valid_response, uint32_t time_out )
{

    uint16_t count_size=0;
    uint16_t count_to_valide_character=0;
    uint8_t *principal_compare_ptr;
    uint8_t *principal_buffer_ptr;

    uint16_t count_max_compare=0;

    principal_compare_ptr=compare_buffer;
    principal_buffer_ptr=buffer_to_be_compare;

    CM_Q03_PIT_Start_Timer(time_out);

    while(count_max_compare < max_num_compare &&!(pit_channel_1_flag)){

        while(((size_buffer_to_be_compare-count_size)>=size_compare_buffer || count_to_valide_character) && !(pit_channel_1_flag) ){
            if(*buffer_to_be_compare == *compare_buffer){
                count_to_valide_character++;
                if(count_to_valide_character==size_compare_buffer){
                    CM_Q03_PIT_Stop_Timer();
                    pit_channel_1_flag=false;
                    if(count_valid_response < count_max_compare){
                        return (offset_error + count_max_compare);
                    }else{
                        return count_max_compare;
                    }
                }
                count_size++;
                buffer_to_be_compare++;
                compare_buffer++;
            }else if(*principal_compare_ptr == *compare_buffer){
                count_size++;
                buffer_to_be_compare++;
            }else{
                count_to_valide_character=0;

            }
        }

        if(pit_channel_1_flag){
            CM_Q03_PIT_Stop_Timer();
            pit_channel_1_flag=false;
            return Timeout;
        }
        compare_buffer=principal_compare_ptr;
        compare_buffer+=size_compare_buffer;
        principal_compare_ptr=compare_buffer;
        buffer_to_be_compare = principal_buffer_ptr;
        count_to_valide_character=0;
        count_size=0;
        count_max_compare++;

    }

    if(pit_channel_1_flag){
        CM_Q03_PIT_Stop_Timer();
        pit_channel_1_flag=false;
        return Timeout;
    }
    else{
        CM_Q03_PIT_Stop_Timer();
        pit_channel_1_flag=false;
        return Not_found;
    }

    return Success;
}

void CM_Q03_Flush_URC()
{

    g_ap_ready_status = enable;
    CM_Q03_AP_Ready(1, 500);
    g_current_selected_buff = urc_buff;
    g_vector_ri += g_interrupt_ri;
    g_interrupt_ri = 0;
    // Clean the PIT_1 flag
    pit_channel_1_flag = false;
    CM_Q03_PIT_Start_Timer(3200);
    while(!pit_channel_1_flag){
        if(g_rx_finished){
            CM_Q03_PIT_Stop_Timer();
            pit_channel_1_flag = false;
            CM_Q03_PIT_Start_Timer(1600);
            g_rx_finished = false;
        }
    }
    CM_Q03_PIT_Stop_Timer();
    pit_channel_1_flag = false;
    CM_Q03_Delay(500000);
    g_current_selected_buff = response_buff;
    CM_Q03_AP_Ready(0, 500);
    CM_Q03_Delay(500000);
}

uint32_t CM_Q03_Read_URC(uint8_t *rxbuff, uint16_t *size, uint8_t *urc_compare, uint16_t size_urc, uint32_t timeout)
{
    uint8_t finder_status = 0;
    // Counter to know the number of character sequences found
    uint8_t temp_character_counter = 0;
    // Pointer to the buffer where the character will be search
    uint8_t *search_buffer;
    // Counter for the number of byte that are read
    uint16_t size_counter = 0;

    uint16_t count_search_buffer = 0;
    // Select to use the response buffer
    search_buffer = g_urc_buff;
    // Clean the PIT_1 flag
    pit_channel_1_flag = false;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer( timeout);
    // Read the buffer until 2 sequences are found or until the PIT_1 flag is active
    while(!(pit_channel_1_flag) && (count_search_buffer == g_count_urc_interrupt)){
        // Waits until 2 characters are receive in the buffer or until the PIT_1 flag is active
        while((2 > g_count_valid_data) && !(pit_channel_1_flag)){}
        // If the PIT_1 flag is active, stop reading the buffer
        if(pit_channel_1_flag){
            break;
        }
        finder_status = Character_finder(search_buffer, g_count_read, character_response_end, BUFFER_SIZE );
        // Look for the character sequence
        if(0 == finder_status){
            // Increase the counter
            temp_character_counter++;
            if(BUFFER_SIZE-1 == g_count_read){
                g_count_read = 1;
            }else if(BUFFER_SIZE-2 == g_count_read){
                g_count_read = 0;
            }else{
                g_count_read += 2;
            }
            g_count_valid_data -=2 ;
        // Check if only one sequence has been found
        }else if(1 == temp_character_counter){
            // Copy the byte to the buffer
            *rxbuff = search_buffer[g_count_read];
            // Go to the next position of the buffer
            rxbuff++;
            // Increase the read counter
            if(BUFFER_SIZE-1 == g_count_read){
                g_count_read = 0;
            }else{
                g_count_read++;
            }
            // Increase the size counter
            size_counter++;
            g_count_valid_data--;
        }
    }
    // Stop the count of the PIT_1
    CM_Q03_PIT_Stop_Timer();
    // Checks if the PIT_1 flag is active
    if(pit_channel_1_flag){
        // Deactivate the PIT_1 flag
        pit_channel_1_flag = false;
        //  Assign the number of byte read to the parameter
        *size = size_counter;
        // Return 1 to indicate an error
        return Timeout;
    }else{
        // Assign the number of byte read to the parameter
        *size = size_counter;
        // Return 0 to indicate successful execution
        return Success;
    }
}

void Dec_to_Ascii(uint32_t num_to_convert, uint8_t *ascii_buff, uint16_t *size_buffer) //Peko modification
//Aumenta el tama�o de las variables a uint32_t
{
    uint16_t temp_size = 1;
    uint32_t temp_num = num_to_convert;
    uint8_t index = 0;

    ascii_buff[0] = (temp_num%10) + 0x30;
    temp_num = temp_num/10;
    while(0 != temp_num){
        index = temp_size;
        while(0 < index){
            ascii_buff[index] = ascii_buff[index-1];
            index--;
        }
        temp_size++;
        ascii_buff[0] = (temp_num%10) + 0x30;
        temp_num = temp_num/10;
    }
    *size_buffer = temp_size;
}


cm_q03_codes_t CM_Q03_Get_parameter_JSON(uint8_t *buffer_to_be_compare,
                                  uint16_t size_buffer_to_be_compare,
                                  uint8_t *compare_parameter,
                                  uint16_t size_compare_parameter,
                                  uint8_t end_compare_parameter,
                                  int32_t *return_parameter){

      uint16_t count_size=0;
      uint16_t count_to_valide_character=0;
      uint8_t *principal_compare_ptr;
      uint32_t time_out=1000000;
      uint8_t negativo=0;
//      uint8_t char_json_end='\"';
      principal_compare_ptr=compare_parameter;
      bool signo_value=false;
      CM_Q03_PIT_Start_Timer(time_out);

      while(((size_buffer_to_be_compare-count_size)>=size_compare_parameter ) && !(pit_channel_1_flag) ){
          if(*buffer_to_be_compare == *compare_parameter){
              count_to_valide_character++;

              if(count_to_valide_character==size_compare_parameter){
                  (*return_parameter)=0;

                  while(((size_buffer_to_be_compare-count_size)>=size_compare_parameter ) && !(pit_channel_1_flag) ){

                      count_size++;
                      buffer_to_be_compare++;

                      if('-'==(*buffer_to_be_compare) && signo_value ==false){//mio
                          negativo=1;//mio
                      }//mio

                      if(((*buffer_to_be_compare) < '0' || (*buffer_to_be_compare) > '9') && negativo == 0){

                          if(signo_value){
                              (*return_parameter)=(*return_parameter)*-1;
                          }
                          CM_Q03_PIT_Stop_Timer();
                          pit_channel_1_flag=false;
                          return Success;

                      }

                      if('-'==(*buffer_to_be_compare)){
                          signo_value=true;
                      }
                      else{
                          (*return_parameter)= (*return_parameter)*10+((*buffer_to_be_compare)-48);
                          negativo = 0; //mio
                      }
                  }

                  CM_Q03_PIT_Stop_Timer();
                  pit_channel_1_flag=false;
                  return Not_found;
              }
              count_size++;
              buffer_to_be_compare++;
              compare_parameter++;
          }else if(*principal_compare_ptr == *compare_parameter){
              count_size++;
              buffer_to_be_compare++;
          }else{

              count_to_valide_character=0;
              compare_parameter=principal_compare_ptr;
          }
      }

      CM_Q03_PIT_Stop_Timer();

      if(pit_channel_1_flag){
          pit_channel_1_flag=false;
          return Timeout;
      }
      else{
          pit_channel_1_flag=false;
          return NOT_SIMILAR_AT_ALL;
      }

}


cm_q03_codes_t CM_Q03_Get_string_JSON(uint8_t *buffer_to_be_compare,
                                  uint16_t size_buffer_to_be_compare,
                                  uint8_t *compare_parameter,
                                  uint16_t size_compare_parameter,
                                  uint8_t end_compare_parameter,
                                  uint8_t *return_string,
                                  uint16_t *size_return_string){

      uint16_t count_size=0;
      uint16_t count_to_valide_character=0;
      uint8_t *principal_compare_ptr;
      uint32_t time_out=1000000;
//      uint8_t char_json_end='\"';
      principal_compare_ptr=compare_parameter;
      CM_Q03_PIT_Start_Timer(time_out);
      (*size_return_string)=0;

      while(((size_buffer_to_be_compare-count_size)>=size_compare_parameter ) && !(pit_channel_1_flag) ){
          if(*buffer_to_be_compare == *compare_parameter){
              count_to_valide_character++;

              if(count_to_valide_character==size_compare_parameter){

                  while(((size_buffer_to_be_compare-count_size)>=size_compare_parameter ) && !(pit_channel_1_flag) ){

                      count_size++;
                      buffer_to_be_compare++;

                      if((*buffer_to_be_compare) == end_compare_parameter){

                          CM_Q03_PIT_Stop_Timer();
                          pit_channel_1_flag=false;
                          return Success;

                      }
                      *return_string=*buffer_to_be_compare;
                      return_string++;
                      (*size_return_string)+=1;
//                      (*return_parameter)= (*return_parameter)*10+((*buffer_to_be_compare)-48);
                  }

                  CM_Q03_PIT_Stop_Timer();
                  pit_channel_1_flag=false;
                  return Not_found;
              }
              count_size++;
              buffer_to_be_compare++;
              compare_parameter++;
          }else if(*principal_compare_ptr == *compare_parameter){
              count_size++;
              buffer_to_be_compare++;
          }else{

              count_to_valide_character=0;
              compare_parameter=principal_compare_ptr;
          }
      }

      CM_Q03_PIT_Stop_Timer();

      if(pit_channel_1_flag){
          pit_channel_1_flag=false;
          return Timeout;
      }
      else{
          pit_channel_1_flag=false;
          return NOT_SIMILAR_AT_ALL;
      }

}


uint32_t  CM_Q03_read_URC_vector(uint8_t *rx_urc_buff, uint16_t *size_rx_urc_buff, uint8_t *compare_buffer,
				uint16_t size_compare_buffer, uint16_t time_out)
{


	uint8_t read_urc_vector_switch=0;

	uint8_t *search_buffer;
	// Counter for the number of byte that are read
	uint16_t size_counter = 0;

	uint16_t aux_count_vector=0;

	uint16_t shift_count_vector=g_count_urc_interrupt;

	uint16_t end_vector=0;
	//Response
	uint8_t response=0;

    uint16_t finish_ptr=0;


	*size_rx_urc_buff=0;
	// Select whether to use the response buffer or the URC buffer, depending of the state of current_selected_buff
	search_buffer = g_urc_buff;


	end_vector=g_count_urc_interrupt;

	// Decrease the counter
    if( shift_count_vector == 0){
        shift_count_vector = UCR_BUFFER_SIZE-1;
      //  end_vector=shift_count_vector;

    }else{
        shift_count_vector -= 1;
      //  end_vector=shift_count_vector;
    }

	while(1){
		switch(read_urc_vector_switch){

			case find_end_read:

				while(( g_count_urc_valid_data - size_counter) >= 0){

					response=Character_finder(search_buffer, shift_count_vector, character_response_end, UCR_BUFFER_SIZE);

					if(response == 0){

						// Increase the counter
						if( shift_count_vector == 1){
							shift_count_vector = UCR_BUFFER_SIZE-1;

						}else if( shift_count_vector == 0){
							shift_count_vector = UCR_BUFFER_SIZE-2;

						}else{
							shift_count_vector -= 2;
						}

						break;
					}
					else if(response == 1){
						size_counter++;

                        // Increase the counter
                        if( shift_count_vector == 0){
                            shift_count_vector = UCR_BUFFER_SIZE-1;

                        }else{
                            shift_count_vector --;
                        }

					}
				}
				if(( g_count_urc_valid_data - size_counter) < 0){
				    read_urc_vector_switch=not_results;
				}else{
                    //end_count_vector=size_counter;
                    read_urc_vector_switch=find_start_read;
				}


			break;

			case find_start_read:

				while(( g_count_urc_valid_data - size_counter) >= 0){

					response=Character_finder(search_buffer, shift_count_vector, character_response_end, UCR_BUFFER_SIZE);

					if(response == 0){

						if(UCR_BUFFER_SIZE-1 == shift_count_vector){
							aux_count_vector = 1;

						}else if(UCR_BUFFER_SIZE-2 == shift_count_vector){
							aux_count_vector = 0;

						}else{
							aux_count_vector = shift_count_vector+2;

						}


						// Increase the counter
						if( shift_count_vector == 0){
							shift_count_vector = UCR_BUFFER_SIZE-1;

						}else{
							shift_count_vector --;
							size_counter++;

						}

						break;

					}
					else if(response ==1){

                        // Increase the counter
                        if( shift_count_vector == 0){
                            shift_count_vector = UCR_BUFFER_SIZE-1;

                        }else{
                            shift_count_vector --;
                        }

						size_counter ++;

					}

				}

                if(( g_count_urc_valid_data - size_counter) < 0){
                    read_urc_vector_switch=not_results;
                }else{
                    //end_count_vector=size_counter;
                    read_urc_vector_switch=compare_vector;
                }


			break;

			case compare_vector:

				response = CM_Q03_Compare_response((&search_buffer[aux_count_vector]), size_compare_buffer, compare_buffer, size_compare_buffer, size_compare_buffer, &finish_ptr, time_out);

				if(response == 0){

					read_urc_vector_switch=save_data;

				}
				else {

					read_urc_vector_switch=find_start_read;

				}

			break;

			case save_data:

				response = Character_finder(search_buffer, aux_count_vector, character_response_end, UCR_BUFFER_SIZE);

				if(response == 0){
					// Increase the counter
					if(UCR_BUFFER_SIZE-1 == aux_count_vector){
						aux_count_vector = 1;

					}else if(UCR_BUFFER_SIZE-2 == aux_count_vector){
						aux_count_vector = 0;

					}else{
						aux_count_vector += 2;

					}

					read_urc_vector_switch=shift_buffer;
					break;

				}
				else if(response == 1){

					*rx_urc_buff = search_buffer[aux_count_vector];

                    // Increase the counter
                    if(UCR_BUFFER_SIZE-1 == aux_count_vector){
                        aux_count_vector = 0;
                    }
                    else{
                        aux_count_vector++;
                    }

					rx_urc_buff++;
					(*size_rx_urc_buff)++;

				}

			break;

			case shift_buffer:

				size_counter=aux_count_vector;

				while(aux_count_vector != end_vector ){


					// Increase the counter
					if(UCR_BUFFER_SIZE-1 == shift_count_vector){
						shift_count_vector = 0;
					}
					else{
						shift_count_vector ++;
					}

					search_buffer[shift_count_vector] = search_buffer[aux_count_vector];

					// Increase the counter
					if(UCR_BUFFER_SIZE-1 == aux_count_vector){
						aux_count_vector = 0;
					}
					else{
						aux_count_vector++;
					}

				}

				read_urc_vector_switch=clean_buffer;

			break;

			case clean_buffer:

                // Increase the counter
                if(UCR_BUFFER_SIZE-1 == shift_count_vector){
                    shift_count_vector = 0;
                }
                else{
                    shift_count_vector ++;
                }

                g_count_urc_interrupt=shift_count_vector;

				while(shift_count_vector != end_vector ){

					search_buffer[shift_count_vector]='\0';

                    // Increase the counter
                    if(UCR_BUFFER_SIZE-1 == shift_count_vector){
                        shift_count_vector = 0;
                    }
                    else{
                        shift_count_vector ++;
                    }

				}
				g_count_urc_valid_data-=(*size_rx_urc_buff+4);
				read_urc_vector_switch=end_urc_vector;

			break;

			case end_urc_vector:

				read_urc_vector_switch=find_start_read;
				return 0;
			break;

			case not_results:

				read_urc_vector_switch=find_start_read;
				return 1;
			break;

			default:

			break;
		}
	}

	return 1;
}

uint32_t CM_Q03_Read_Wait_URC(uint8_t *rxbuff, uint16_t *size, uint32_t timeout)
{
    uint16_t finder_status = 0;
    // Counter to know the number of character sequences found
    uint16_t temp_character_counter = 0;
    // Pointer to the buffer where the character will be search
    uint8_t *search_buffer;
    // Counter for the number of byte that are read
    uint16_t size_counter = 0;
    // Select to use the response buffer
    search_buffer = g_urc_buff;
    // Clean the PIT_1 flag
    pit_channel_1_flag = false;

    g_ap_ready_status = enable;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer( timeout);

    while((0 == g_interrupt_ri) && !(pit_channel_1_flag)){}
    // Read the buffer until 2 sequences are found or until the PIT_1 flag is active
    while(!(pit_channel_1_flag) && (2 != temp_character_counter)){
        // Waits until 2 characters are receive in the buffer or until the PIT_1 flag is active
        while((2 > g_count_urc_valid_data) && !(pit_channel_1_flag)){}
        // If the PIT_1 flag is active, stop reading the buffer
        if(pit_channel_1_flag){
            g_ap_ready_status = disable;
            break;
        }
        finder_status = Character_finder(search_buffer, g_count_urc_read, character_response_end,BUFFER_SIZE);
        // Look for the character sequence
        if(0 == finder_status){
            // Increase the counter
            temp_character_counter++;
            if(BUFFER_SIZE-1 == g_count_urc_read){
                g_count_urc_read = 1;
            }else if(BUFFER_SIZE-2 == g_count_urc_read){
                g_count_urc_read = 0;
            }else{
                g_count_urc_read += 2;
            }
            g_count_urc_valid_data -=2 ;
        // Check if only one sequence has been found
        }else if(1 == temp_character_counter){
            // Copy the byte to the buffer
            *rxbuff = search_buffer[g_count_urc_read];
            // Go to the next position of the buffer
            rxbuff++;
            // Increase the read counter
            if(BUFFER_SIZE-1 == g_count_urc_read){
                g_count_urc_read = 0;
            }else{
                g_count_urc_read++;
            }
            // Increase the size counter
            size_counter++;
            g_count_urc_valid_data--;
        }
    }
    g_ap_ready_status = disable;
    // Stop the count of the PIT_1
    CM_Q03_PIT_Stop_Timer();
    // Checks if the PIT_1 flag is active
    if(pit_channel_1_flag){
        // Deactivate the PIT_1 flag
        pit_channel_1_flag = false;
        //  Assign the number of byte read to the parameter
        *size = size_counter;
        // Return 1 to indicate an error
        return Timeout;
    }else{
        // Assign the number of byte read to the parameter
        *size = size_counter;
        // Return 0 to indicate successful execution
        return Success;
    }
}

void Concatenate_Command(uint8_t *dest, uint8_t *src, uint16_t size_src, uint16_t *size_dest)
{
    strncat((char*)dest, (const char*)src, size_src);
    (*size_dest) += size_src;
}



//output buffer
uint32_t CM_Q03_Read_Response_Start_and_Stop_read_character(
        uint8_t *rxbuff, uint16_t size_rxbuff, uint8_t *output_buff,
        uint16_t *size_output_buff, uint8_t start_character,
        uint8_t stop_character)
{

    uint16_t capture_count = 0;
    uint16_t flag_capture = false;

    while (capture_count <= size_rxbuff)
    {

        if (*rxbuff == start_character || flag_capture == true)
        {

            rxbuff++;
            if (*rxbuff == stop_character)
            {
                return Success;
            }

            flag_capture = true;
            *output_buff = *rxbuff;

            output_buff++;
            (*size_output_buff)++;
            capture_count++;
        } else
        {
            rxbuff++;
            capture_count++;
        }
    }

    return ERROR;
}
//search count
uint32_t CM_Q03_Read_Response_count_Start_and_Stop_read_character(
        uint8_t *rxbuff, uint16_t size_rxbuff, uint8_t *output_buff,
        uint16_t *size_output_buff, uint8_t search_character,
        uint8_t count_search_character, uint8_t stop_character)
{

    uint16_t capture_count = 0;
    uint16_t flag_capture_count = false;

    while (capture_count <= size_rxbuff)
    {

        if (*rxbuff == search_character)
        {

            flag_capture_count++;
        }

        if (flag_capture_count >= count_search_character)
        {

            rxbuff++;
            if (*rxbuff == stop_character)
            {
                return Success;
            }

            *output_buff = *rxbuff;

            output_buff++;
            (*size_output_buff)++;
            capture_count++;

        }else{
            rxbuff++;
            capture_count++;
        }
    }

    return ERROR;
}

uint8_t CM_Q03_Read_response_signal_type(uint8_t *rxbuff, uint16_t size_rxbuff){
    uint16_t capture_count = 0;

    while(capture_count <= size_rxbuff){

        if (*rxbuff == 'L'){
            return 1;
        }
        if (*rxbuff == 'G'){
            return 2;
        }

        capture_count++;
        rxbuff++;
    }
    return 0;
}

uint32_t CM_Q03_Read_Response_count_Start_N_characters(
		uint8_t *rxbuff, uint16_t size_rxbuff, uint8_t *output_buff,
		uint16_t *size_output_buff, uint8_t search_character,
		uint8_t count_search_character, uint16_t N_characters)
{

    uint16_t capture_count = 0;
    uint16_t flag_capture_count = false;
	uint16_t counter = 0;

	while (capture_count <= size_rxbuff)
	{

		if (*rxbuff == search_character)
		{

			flag_capture_count++;
		}

		if (flag_capture_count >= count_search_character)
		{

			rxbuff++;
			if (counter >= N_characters)
			{
				return Success;
			}

			*output_buff = *rxbuff;

			output_buff++;
			(*size_output_buff)++;
			capture_count++;
			counter ++;
		} else
		{
			rxbuff++;
			capture_count++;
		}
	}

	return ERROR;
}

//output buffer
uint32_t CM_Q03_Read_Response_Start_and_Stop_read_character_end_ptr(
        uint8_t *rxbuff, uint16_t size_rxbuff, uint8_t *output_buff,
        uint16_t *size_output_buff, uint8_t start_character,
        uint8_t stop_character, uint8_t **ptr_end)
{

    uint16_t capture_count = 0;
    uint8_t flag_capture = false;

    while (capture_count <= size_rxbuff)
    {

        if (*rxbuff == start_character || flag_capture == true)
        {

            rxbuff++;
            if (*rxbuff == stop_character)
            {

                rxbuff++;
                *ptr_end = rxbuff;
                return Success;
            }

            flag_capture = true;
            *output_buff = *rxbuff;

            output_buff++;
            (*size_output_buff)++;
            capture_count++;
        } else
        {
            rxbuff++;
            capture_count++;
        }
    }

    return Success;
}
//search count
uint32_t CM_Q03_Read_Response_count_Start_and_Stop_read_character_end_ptr(
        uint8_t *rxbuff, uint16_t size_rxbuff, uint8_t *output_buff,
        uint16_t *size_output_buff, uint8_t search_character,
        uint8_t count_search_character, uint8_t stop_character,
        uint8_t **ptr_end)
{

    uint16_t capture_count = 0;
    uint8_t flag_capture_count = false;

    while (capture_count <= size_rxbuff)
    {

        if (*rxbuff == search_character)
        {

            flag_capture_count++;
        }

        if (flag_capture_count >= count_search_character)
        {

            rxbuff++;
            if (*rxbuff == stop_character)
            {

                rxbuff++;
                *ptr_end = rxbuff;
                return Success;
            }

            *output_buff = *rxbuff;

            output_buff++;
            (*size_output_buff)++;
            capture_count++;
        } else
        {
            rxbuff++;
            capture_count++;
        }
    }

    return Success;
}

uint32_t CM_Q03_Read_Response_Vector_Until(uint8_t *rxbuff, uint16_t *size,
											uint8_t stop_character, uint32_t timeout)
{
    // Counter to know the number of character sequences found
    uint16_t temp_character_counter = 0;
    // Pointer to the buffer where the character will be search
    uint8_t *search_buffer;
    // Counter for the number of byte that are read
    uint16_t size_counter = 0;

    // Select to use the response buffer
    search_buffer = g_response_buff;
    // Clean the PIT_1 flag
    pit_channel_1_flag = false;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer(timeout);

    // Read the buffer until 2 sequences are found or until the PIT_1 flag is active
    while(!(pit_channel_1_flag) && ( 1> temp_character_counter))
    {
        // Waits until 2 characters are receive in the buffer or until the PIT_1 flag is active
        while((2 > g_count_valid_data) && !(pit_channel_1_flag)){}

        // If the PIT_1 flag is active, stop reading the buffer
        if(pit_channel_1_flag)
        {
            break;
        }

        // Look for the character sequence
        if('\r' == search_buffer[g_count_read] && '\r' == search_buffer[g_count_read+1])
        {
            temp_character_counter ++;

            if(0 == g_count_read)
            {
                g_count_read = BUFFER_SIZE-1;
            }
        }
        else
        {


            // Copy the byte to the buffer
            *rxbuff = search_buffer[g_count_read];
            // Go to the next position of the buffer
            rxbuff++;
            // Increase the read counter
            if(BUFFER_SIZE-1 == g_count_read)
            {
                g_count_read = 0;
            }

            else
            {
                g_count_read++;
            }

            // Increase the size counter
            size_counter++;
            g_count_valid_data--;
        }
    }

    // Stop the count of the PIT_1
    CM_Q03_PIT_Stop_Timer();
    // Checks if the PIT_1 flag is active
    if(pit_channel_1_flag)
    {
        // Deactivate the PIT_1 flag
        pit_channel_1_flag = false;
        //  Assign the number of byte read to the parameter
        *size = size_counter;
        // Return 1 to indicate an error
        return Timeout;
    }

    else
    {
        // Assign the number of byte read to the parameter
        *size = size_counter;
        // Return 0 to indicate successful execution
        return Success;
    }
}

uint32_t CM_Q03_Read_Response_Vector_N_Characters(uint16_t length_to_read, uint8_t* rxbuff,
													uint16_t* size, uint32_t timeout)
{
    // Counter to know the number of character sequences found
    uint16_t temp_character_counter = 0;
    // Pointer to the buffer where the character will be search
    uint8_t *search_buffer;
    // Counter for the number of byte that are read
    uint16_t size_counter = 0;

    // Select to use the response buffer
    search_buffer = g_response_buff;
    // Clean the PIT_1 flag
    pit_channel_1_flag = false;
    // Start the count of PIT_1
    CM_Q03_PIT_Start_Timer(timeout);

    // Read the buffer until 2 sequences are found or until the PIT_1 flag is active
    while(!(pit_channel_1_flag) && (1 != temp_character_counter))
    {
        // Waits until 2 characters are receive in the buffer or until the PIT_1 flag is active
        while((2 > g_count_valid_data) && !(pit_channel_1_flag)){}

        // If the PIT_1 flag is active, stop reading the buffer
        if(pit_channel_1_flag)
        {
            break;
        }

        if(size_counter < length_to_read)
        {
            // Copy the byte to the buffer
            *rxbuff = search_buffer[g_count_read];
            // Go to the next position of the buffer
            rxbuff++;
            // Increase the read counter
            if(BUFFER_SIZE-1 == g_count_read)
            {
                g_count_read = 0;
            }

            else
            {
                g_count_read++;

                if(0 == g_count_read)
                {
                    g_count_read = BUFFER_SIZE-1;
                }
            }

            // Increase the size counter
            size_counter++;
            g_count_valid_data--;
        }

        else
        {
        	temp_character_counter ++;
        }
    }

    // Stop the count of the PIT_1
    CM_Q03_PIT_Stop_Timer();
    // Checks if the PIT_1 flag is active
    if(pit_channel_1_flag)
    {
        // Deactivate the PIT_1 flag
        pit_channel_1_flag = false;
        //  Assign the number of byte read to the parameter
        *size = size_counter;
        // Return 1 to indicate an error
        return Timeout;
    }

    else
    {
        // Assign the number of byte read to the parameter
        *size = size_counter;
        // Return 0 to indicate successful execution
        return Success;
    }
}

uint32_t CM_Q03_AP_Ready(uint8_t level, uint16_t interval)
{
    uint8_t response_ok[2] = "OK";
    uint8_t command_apready[19] = "AT+QCFG=\"apready\",1";
    uint16_t status = 0;
    uint8_t temp_character = '\r';
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[50] = {0};
    uint8_t temp_buff[5] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t command_comma = ',';

    Delay(50000);
    CM_Q03_buffer_clean();

    if(level){
        active_urc=true;
    }
    else{
        active_urc=false;
    }

    Concatenate_Command(cmd_buff, command_apready, sizeof(command_apready), &cmd_buff_size);
    Dec_to_Ascii(level, temp_buff, &buffer_size);
//    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &command_comma, sizeof(command_comma), &cmd_buff_size);
//    Dec_to_Ascii(interval, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &temp_character, sizeof(temp_character), &cmd_buff_size);
    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
    status = CM_Q03_Read(temp_buff, &buffer_size, 500000);
    status = CM_Q03_Compare_short_response(temp_buff, buffer_size, response_ok,
                                     sizeof(response_ok), 500000);
    Delay(50000);

    if (0 != status)
    {
        status = CM_Q03_Get_Response(temp_buff, buffer_size);
        return status;
    }
    return status;
}

volatile uint8_t *ptr_ri = &g_interrupt_ri;
volatile bool *ptr_ri_flag= &g_interrupt_ri_flag;
uint8_t *Get_RI_Flag()
{
    return ptr_ri;
}

void CM_Q03_Return_To_Last_Character_End()
{
	g_count_read -= 2;
	g_count_valid_data += 2;
}


/////////////////////////////////////////////////////////////GPIO/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static uint32_t CM_Q03_GPIO_Initialization(port_gpio_cm_q03 Port_select, bool select_input_output)
{

    PORT_Type *port_base;
    GPIO_Type *gpio_base;
    uint8_t pin_idx;

    const port_pin_config_t portc_config = {
            kPORT_PullUp,
            kPORT_SlowSlewRate,
            kPORT_PassiveFilterDisable,
            kPORT_LowDriveStrength,
            kPORT_MuxAsGpio
    };

    switch(Port_select)
    {
        case Port_VDD_EG915:
#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
            CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
            port_base= PORTB;
            gpio_base= GPIOB;
            pin_idx= PIN16_IDX;
#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
            CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */
            port_base= PORTA;
            gpio_base= GPIOA;
            pin_idx= PIN4_IDX;
#endif
        break;

        case Port_RI_EG915:


#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
            CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
            port_base= PORTC;
            gpio_base= GPIOC;
            pin_idx= PIN3_IDX;

#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
            CLOCK_EnableClock(kCLOCK_PortD);                           /* Port C Clock Gate Control: Clock enabled */
            port_base= PORTD;
            gpio_base= GPIOD;
            pin_idx= PIN7_IDX;
#endif
        break;

        case Port_PWR_KEY_EG915:
#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
            CLOCK_EnableClock(kCLOCK_PortB);                           /* Port C Clock Gate Control: Clock enabled */
            port_base= PORTB;
            gpio_base= GPIOB;
            pin_idx= PIN0_IDX;
#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
            CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
            port_base= PORTC;
            gpio_base= GPIOC;
            pin_idx= PIN6_IDX;
#endif
        break;
        case Port_AP_Ready:
#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))

            CLOCK_EnableClock(kCLOCK_PortE);                           /* Port C Clock Gate Control: Clock enabled */
            port_base= PORTE;
            gpio_base= GPIOE;
            pin_idx= PIN17_IDX;
#endif
        break;
        case Port_NW_Disable:
#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))

            CLOCK_EnableClock(kCLOCK_PortE);                           /* Port C Clock Gate Control: Clock enabled */
            port_base= PORTE;
            gpio_base= GPIOE;
            pin_idx= PIN18_IDX;
#endif
        break;
        case Port_DTR:
#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))

            CLOCK_EnableClock(kCLOCK_PortE);                           /* Port C Clock Gate Control: Clock enabled */
            port_base= PORTE;
            gpio_base= GPIOE;
            pin_idx= PIN19_IDX;
#endif
        break;

        case Port_PSM_IND:
#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))

            CLOCK_EnableClock(kCLOCK_PortD);                           /* Port C Clock Gate Control: Clock enabled */
            port_base= PORTD;
            gpio_base= GPIOD;
            pin_idx= PIN6_IDX;
#endif
        break;

        case Port_Status:
#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))

            CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
            port_base= PORTC;
            gpio_base= GPIOC;
            pin_idx= PIN7_IDX;
#endif
        break;

        case Port_RESET:
#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
            CLOCK_EnableClock(kCLOCK_PortB);                           /* Port C Clock Gate Control: Clock enabled */
            port_base= PORTB;
            gpio_base= GPIOB;
            pin_idx= PIN1_IDX;
#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))

#endif
        break;

        default:
            return GPIO_pin_not_available;
        break;
    }

    gpio_pin_config_t config;

    if(select_input_output){

        config.pinDirection= kGPIO_DigitalInput;
        config.outputLogic= 0;
    }
    else{

        config.pinDirection= kGPIO_DigitalOutput;
        config.outputLogic= 0;
    }

    PORT_SetPinMux(port_base, pin_idx, kPORT_MuxAsGpio);

    GPIO_PinInit(gpio_base,pin_idx,&config);

    if(Port_RI_EG915==Port_select)
    {
//        PORT_SetPinInterruptConfig(PORTC, PIN3_IDX, kPORT_InterruptRisingEdge );
//        PORT_SetPinConfig(PORTC, PIN3_IDX, &portc_config);
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
//        PORT_SetPinInterruptConfig(PORTC, PIN1_IDX, kPORT_InterruptRisingEdge );
//        PORT_SetPinConfig(PORTC, PIN1_IDX, &portc_config);

        PORT_SetPinInterruptConfig(PORTD, PIN7_IDX, kPORT_InterruptFallingEdge );
        PORT_SetPinConfig(PORTD, PIN7_IDX, &portc_config);

#endif
        EnableIRQ(PORTC_PORTD_IRQn);
    }
    return Success;
}

static uint32_t CM_Q03_GPIO_Write(port_gpio_cm_q03 Port_select, bool output )
{
    GPIO_Type *gpio_base;
    uint8_t pin_idx;

    switch(Port_select)
    {
        case Port_VDD_EG915:
#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
            gpio_base= GPIOB;
            pin_idx= PIN16_IDX;
#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
            gpio_base= GPIOA;
            pin_idx= PIN4_IDX;
#endif
        break;

        case Port_PWR_KEY_EG915:
#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
            gpio_base=GPIOB;
            pin_idx= PIN0_IDX;
#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
            gpio_base=GPIOC;
            pin_idx= PIN6_IDX;
#endif
        break;

        case Port_AP_Ready:
#if(defined(ATHEN_BOARD) && ATHEN_BOARD)

#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
            gpio_base=GPIOE;
            pin_idx= PIN17_IDX;
#endif
        break;

        case Port_NW_Disable:
#if(defined(ATHEN_BOARD) && ATHEN_BOARD)

#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
            gpio_base=GPIOE;
            pin_idx= PIN18_IDX;
#endif
        break;

        case Port_RESET:
#if(defined(ATHEN_BOARD) && ATHEN_BOARD)
            gpio_base=GPIOB;
            pin_idx= PIN1_IDX;
#endif
#if(defined(ATHEN_BOARD) && (!ATHEN_BOARD))
            gpio_base=GPIOC;
            pin_idx= PIN7_IDX;
#endif
        break;

        default:
            return GPIO_pin_not_available;
        break;
    }
    GPIO_PinWrite(gpio_base, pin_idx, output);

    return Success;
}


general_codes_t CM_Q03_GPIO_Read(port_gpio_cm_q03 Port_select, uint8_t* input)
{
    GPIO_Type *gpio_base;
    uint8_t pin_idx;

#if (defined(ATHEN_BOARD) && (!ATHEN_BOARD))

    switch(Port_select){

        case Port_DTR:

            gpio_base= GPIOE;
            pin_idx= PIN19_IDX;
        break;

        case Port_PSM_IND:

            gpio_base=GPIOD;
            pin_idx= PIN6_IDX;
        break;

        case Port_Status:

            gpio_base=GPIOC;
            pin_idx= PIN7_IDX;
        break;

        default:
            return GPIO_pin_not_available;
        break;
    }

#endif


    *input = GPIO_PinRead(gpio_base, pin_idx);

    return successful_task;
}

 cm_q03_codes_t CM_Q03_Set_Command_Terminator_Character_ATS_Reconnect(void)
{

    cm_q03_codes_t status = Success;

    uint8_t count_ats=0;

    if( CM_Q03_AT_Command()==Success ){

        return status;
    }

    do{
        Delay(100000);
        status=CM_Q03_Set_Command_Terminator_Character_ATS3(character_command_end);
        Delay(100000);
        status|=CM_Q03_Set_Response_Character_ATS4(character_command_end);

        Delay(1000000);

        status= CM_Q03_AT_Command();
        count_ats++;
    }while(status!=Success && count_ats < 5);

    if (0 != status)
    {
        status = Unknown_error;
        return status;
    }
    return status;
}


 static cm_q03_codes_t CM_Q03_Set_Command_Terminator_Character_ATS3(uint8_t character)
{
    uint8_t command_response_character[5] = "ATS3=";
    cm_q03_codes_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[10] = {0};
    uint8_t temp_buff[3] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rx_buff[15] = {0};

    Concatenate_Command(cmd_buff, command_response_character, sizeof(command_response_character), &cmd_buff_size);
    Dec_to_Ascii(character, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 5000000);
    character_command_end = character;
    status = CM_Q03_Read(rx_buff, &buffer_size, 5000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rx_buff, buffer_size, response_ok,
                                     sizeof(response_ok), 5000000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rx_buff, buffer_size);
        return status;
    }
    return status;
}

static cm_q03_codes_t CM_Q03_Set_Response_Character_ATS4(uint8_t character)
{
    uint8_t command_response_character[5] = "ATS4=";
    cm_q03_codes_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[10] = {0};
    uint8_t temp_buff[3] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rx_buff[15] = {0};

    Concatenate_Command(cmd_buff, command_response_character, sizeof(command_response_character), &cmd_buff_size);
    Dec_to_Ascii(character, temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, temp_buff, buffer_size, &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 1000000);
    character_response_end = character;
    status = CM_Q03_Read(rx_buff, &buffer_size, 1000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rx_buff, buffer_size, response_ok,
                                     sizeof(response_ok), 100000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rx_buff, buffer_size);
        return status;
    }
    return status;
}

static cm_q03_codes_t CM_Q03_Echo_Control(uint8_t echo)
{
    uint8_t turn_off_echo[6] = "ATE0&W";
    cm_q03_codes_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[10] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rx_buff[15] = {0};

    turn_off_echo[3] = echo + '0';
    Concatenate_Command(cmd_buff, turn_off_echo, sizeof(turn_off_echo), &cmd_buff_size);
    Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
    status = CM_Q03_Write(cmd_buff, cmd_buff_size, 1000000);
    status = CM_Q03_Read(rx_buff, &buffer_size, 1000000);
    if(Timeout == status)
    {
    	return status;
    }
    status = CM_Q03_Compare_short_response(rx_buff, buffer_size, response_ok,
                                     sizeof(response_ok), 100000);
    if (0 != status)
    {
        status = CM_Q03_Get_Response(rx_buff, buffer_size);
        return status;
    }
    return status;
}

static cm_q03_codes_t CM_Q03_AT_Command(void)
{
	uint8_t AT_command[2] = "AT";
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[3] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};

	Concatenate_Command(cmd_buff, AT_command, sizeof(AT_command), &cmd_buff_size);
	Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);

    Delay(50000);
    CM_Q03_buffer_clean();
    Delay(50000);

	status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);//ANTES 500000
	status = CM_Q03_Read(rxbuff, &buffer_size, 3000000); //3000000 ANTES
	if(Timeout == status)
	{
		return status;
	}
	status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
									 sizeof(response_ok), 500000);
	if (Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}

    return status;
}

cm_q03_codes_t CM_Q03_Power_Down(cm_q03_powd_t type)
{
	uint8_t Power_Down[9] = "AT+QPOWD=";
	uint8_t power_down_response[12] = "POWERED DOWN";
    uint16_t status = 0;
    uint16_t buffer_size = 0;
    uint8_t cmd_buff[11] = {0};
    uint16_t cmd_buff_size = 0;
    uint8_t rxbuff[15] = {0};
    uint16_t temp_buff = 0;
    uint8_t i = 0;

    CM_Q03_Flush_URC();

	Concatenate_Command(cmd_buff, Power_Down, sizeof(Power_Down), &cmd_buff_size);
    Dec_to_Ascii(type, &temp_buff, &buffer_size);
    Concatenate_Command(cmd_buff, &temp_buff, buffer_size, &cmd_buff_size);
	Concatenate_Command(cmd_buff, &character_command_end, sizeof(character_command_end), &cmd_buff_size);
	status = CM_Q03_Write(cmd_buff, cmd_buff_size, 500000);
	if(Timeout == status)
	{
		return status;
	}
	status = CM_Q03_Read(rxbuff, &buffer_size, 300000);
	if(Timeout == status)
	{
		return status;
	}
	status = CM_Q03_Compare_short_response(rxbuff, buffer_size, response_ok,
									 sizeof(response_ok), 500000);
	if (Success != status)
	{
		status = CM_Q03_Get_Response(rxbuff, buffer_size);
		return status;
	}
    CM_Q03_AP_Ready(1, 500);
    do
    {
		status = CM_Q03_Read(rxbuff, &buffer_size, 1000000);
		if(Success == status)
		{
			status = CM_Q03_Compare_short_response(rxbuff, buffer_size, power_down_response,
											 sizeof(power_down_response), 500000);
		}
		i++;
    }while((Success != status) && (i <= 60));
    if(Success != status)
    {
    	CM_Q03_AP_Ready(0, 500);
        return Timeout;
    }
	g_interrupt_ri = 0;
	CM_Q03_buffer_clean();
	CM_Q03_GPIO_Write(Port_RESET, Write_0_cm_q03);
    CM_Q03_GPIO_Write(Port_PWR_KEY_EG915, Write_0_cm_q03);
    CM_Q03_GPIO_Write(Port_VDD_EG915, Write_0_cm_q03);
	g_userInit_shield_flag = false;
	character_response_end = CHARACTER_RESPONSE_END;
	character_command_end = CHARACTER_COMMAND_END;
	CM_Q03_buffer_reset();
	CM_Q03_URC_buffer_reset();
	CM_Q03_Delay(1000);
    return status;
}
