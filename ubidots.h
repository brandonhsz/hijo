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
 * \file       ubidots.h
 * \details    *For more information, please refer to the included documentation
 *             provided.
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2021, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

#ifndef UBIDOTS_H_
#define UBIDOTS_H_

#include "init_config_functions.h"

#define VALID_UNIX_TS 100000000U
#define KAIROS_JSON_SIZE 26

typedef enum
{
	ustr_imei,
	ustr_sv,
	ustr_sh,
	ustr_st,
	ustr_rssi,
	ustr_ahb,
	ustr_aha,
	ustr_atb,
	ustr_ata,
	ustr_avb,
	ustr_pahb,
	ustr_paha,
	ustr_patb,
	ustr_pata,
	ustr_pavb,
	ustr_ptt,
	ustr_ptc,
	ustr_ppm,
	ustr_pcf,
	ustr_value,
	ustr_timestamp,
	ustr_context,
	ustr_lat,
	ustr_lng,
	ustr_mk,
	ustr_acttm
} json_parameters_t;

typedef struct
{
	const uint8_t *strPtr;
	json_parameters_t parameter;
} ubidots_json_t;

typedef struct
{
	json_parameters_t parameter;
	uint32_t data;
	uint32_t timestamp;
} json_data_t;

typedef struct
{
	json_parameters_t parameter;
	uint8_t *data;
	uint32_t timestamp;
} json_string_t;

typedef struct
{
	int8_t latitude;
	int8_t longitude;
} json_context_t;

typedef enum
{
	json_succesful_task,
	json_invalid_parameter,
	json_null_ptr
} json_error_codes_e;

json_error_codes_e Ubidots_Parse(json_data_t *data, json_context_t *context);

json_error_codes_e Ubidots_ParseString(json_string_t *data, json_context_t *context);

json_error_codes_e Ubidots_Batch_Parse(json_data_t *data, json_context_t *context);

bool UbidotsMessageisInit(void);

bool UbidotsMessageisFinished(void);

void Ubidots_resetMessage(void);

json_error_codes_e UbidotsGetMessage(uint8_t *message);
#endif /* UBIDOTS_H_ */
