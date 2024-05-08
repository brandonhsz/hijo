////////////////////////////COPYRIGHT NOTICES///////////////////////////////////
/*
 * Copyright 2016-2021 NXP Semiconductor, Inc.
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
 * \file       ubidots.c
 * \details    *For more information, please refer to the included documentation
 *             provided.
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2021, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////

#include "ubidots.h"

static json_error_codes_e _ParseInteger(uint32_t integer, uint8_t *str);
static json_error_codes_e _ParseTimestamp(uint32_t ts, uint8_t *str);

static json_error_codes_e _UbidotsGetStr(json_parameters_t parameter, uint8_t *str);
static inline void effstrcat(uint8_t *dst, uint8_t *src);
static uint16_t _GetStrLen(uint8_t *str);
static json_error_codes_e _JSONRemoveEnd(uint8_t *str);
static json_error_codes_e _JSONSetValue(json_data_t *data, uint8_t *str);
static json_error_codes_e _JSONSetContext(json_context_t *context, uint8_t *str);
static json_error_codes_e _JSONSetString(json_string_t *data, uint8_t *str);

static const uint8_t string_imei[] = {"\"IMEI\""};
static const uint8_t string_sv[]   = {"\"SV\""};
static const uint8_t string_sh[]   = {"\"SH\""};
static const uint8_t string_st[]   = {"\"ST\""};
static const uint8_t string_rssi[] = {"\"RSSI\""};
static const uint8_t string_ahb[]  = {"\"AHB\""};
static const uint8_t string_aha[]  = {"\"AHA\""};
static const uint8_t string_atb[]  = {"\"ATB\""};
static const uint8_t string_ata[]  = {"\"ATA\""};
static const uint8_t string_avb[]  = {"\"AVB\""};     // Alarma de voltaje bajo
static const uint8_t string_pahb[] = {"\"PAHB\""};     // Alarma de voltaje bajo
static const uint8_t string_paha[] = {"\"PAHA\""};     // Alarma de voltaje bajo
static const uint8_t string_patb[] = {"\"PATB\""};     // Alarma de voltaje bajo
static const uint8_t string_pata[] = {"\"PATA\""};     // Alarma de voltaje bajo
static const uint8_t string_pavb[] = {"\"PAVB\""};     // Alarma de voltaje bajo
static const uint8_t string_ptt[]  = {"\"PTT\""};     // Parametro de tiempos de transmision de datos
static const uint8_t string_ptc[]  = {"\"PTC\""};     // Parametros de tiempos de captura de datos
static const uint8_t string_ppm[]  = {"\"PPM\""};     // Parametros de promedio de muestras
static const uint8_t string_pcf[]  = {"\"PCF\""};     // Parametro de activacion de FOTA
static const uint8_t string_acttime[]  = {"\"ACTM\""};     // Parametro de activacion de FOTA

static const uint8_t string_context[]  = {"\"context\""};     // Parametro de activacion de FOTA
static const uint8_t string_value[] = {"\"value\""};
static const uint8_t string_timestamp[] = {"\"timestamp\""};
static const uint8_t string_lat[]  = {"\"lat\""};
static const uint8_t string_lng[]  = {"\"lng\""};
static const uint8_t string_mk[]  = {"\"my-key\""};

static const uint8_t string_start[] = {"{"};
static const uint8_t string_end[] = {"}"};
static const uint8_t string_start_buff[] = {"["};
static const uint8_t string_end_buff[] = {"]"};
static const uint8_t string_comma[] = {","};
static const uint8_t string_underscore[] = {"-"};
static const uint8_t string_two_points[] = {":"};
static const uint8_t string_space[] = {" "};

static bool message_initialized = false;
static bool message_finished = false;

static uint8_t ubidots_json_format[500];

static const ubidots_json_t kairos_json[KAIROS_JSON_SIZE] =
{
  {string_imei, ustr_imei},
  {string_sv, ustr_sv},
  {string_sh, ustr_sh},
  {string_st, ustr_st},
  {string_rssi, ustr_rssi},
  {string_ahb, ustr_ahb},
  {string_aha, ustr_aha},
  {string_atb, ustr_atb},
  {string_ata, ustr_ata},
  {string_avb, ustr_avb},
  {string_pahb, ustr_pahb},
  {string_paha, ustr_paha},
  {string_patb, ustr_patb},
  {string_pata, ustr_pata},
  {string_pavb, ustr_pavb},
  {string_ptt, ustr_ptt},
  {string_ptc, ustr_ptc},
  {string_ppm, ustr_ppm},
  {string_pcf, ustr_pcf},
  {string_value, ustr_value},
  {string_timestamp, ustr_timestamp},
  {string_context, ustr_context},
  {string_lat, ustr_lat},
  {string_lng, ustr_lng},
  {string_mk, ustr_mk},
  {string_acttime, ustr_acttm}
};

json_parameters_t last_parameter;

json_error_codes_e Ubidots_Parse(json_data_t *data, json_context_t *context)
{
    json_error_codes_e retval;

    if(NULL != data)
    {
    	if(data->timestamp > VALID_UNIX_TS)
    	{
			if(!message_initialized)
			{
				/* add key bracket { if message has not been initialized */
				memset(ubidots_json_format, '\0', sizeof(ubidots_json_format));
				strcat((char*) ubidots_json_format, (char*) string_start);
				message_initialized = true;
			}
			else
			{
				/* remove message end set and add a comma for next parameter */
				_JSONRemoveEnd(ubidots_json_format);
				strcat((char*) ubidots_json_format, (char*) string_comma);
			}

			if(last_parameter != data->parameter)
			{
				/* get parameter label and copy to ubidots_json_format */
				_UbidotsGetStr(data->parameter, ubidots_json_format);
				/* add equals ":" */
				strcat((char*) ubidots_json_format, (char*) string_two_points);
				/* start square bracket [ to append more than 1 value */
				strcat((char*) ubidots_json_format, (char*) string_start_buff);
				/* append data to the message */
				_JSONSetValue(data, ubidots_json_format);
				/* end square bracket ] to append more than 1 value */
				strcat((char*) ubidots_json_format, (char*) string_end_buff);
			}
			else
			{
				/* remove square bracket end */
				_JSONRemoveEnd(ubidots_json_format);
				/* remove square bracket end */
				_JSONRemoveEnd(ubidots_json_format);
				/* add a comma for next value */
				strcat((char*) ubidots_json_format, (char*) string_comma);
				/* append data to the message */
				_JSONSetValue(data, ubidots_json_format);
				/* end square bracket ] to append more than 1 value */
				strcat((char*) ubidots_json_format, (char*) string_end_buff);
			}


			if(NULL != context)
			{
				/* \note not yet tested */
				_JSONRemoveEnd(ubidots_json_format);
				_JSONSetContext(context, ubidots_json_format);
			}

			/* add message end key bracket } */
			strcat((char*) ubidots_json_format, (char*) string_end);
			last_parameter =  data->parameter;
			retval = json_succesful_task;
    	}
    	else
    	{
    		retval = json_invalid_parameter;
    	}
    }
    else
    {
    	retval = json_null_ptr;
    }

    return retval;
}

json_error_codes_e Ubidots_ParseString(json_string_t *data, json_context_t *context)
{
    json_error_codes_e retval;

    if(NULL != data)
    {
    	if(data->timestamp > VALID_UNIX_TS)
    	{
			if(!message_initialized)
			{
				/* add key bracket { if message has not been initialized */
				memset(ubidots_json_format, '\0', sizeof(ubidots_json_format));
				strcat((char*) ubidots_json_format, (char*) string_start);
				message_initialized = true;
			}
			else
			{
				/* remove message end set and add a comma for next parameter */
				_JSONRemoveEnd(ubidots_json_format);
				strcat((char*) ubidots_json_format, (char*) string_comma);
			}

			if(last_parameter != data->parameter)
			{
				/* get parameter label and copy to ubidots_json_format */
				_UbidotsGetStr(data->parameter, ubidots_json_format);
				/* add equals ":" */
				strcat((char*) ubidots_json_format, (char*) string_two_points);
				/* start square bracket [ to append more than 1 value */
				strcat((char*) ubidots_json_format, (char*) string_start_buff);
				/* append data to the message */
				_JSONSetString(data, ubidots_json_format);
				/* end square bracket ] to append more than 1 value */
				strcat((char*) ubidots_json_format, (char*) string_end_buff);
			}
			else
			{
				/* remove square bracket end */
				_JSONRemoveEnd(ubidots_json_format);
				/* remove square bracket end */
				_JSONRemoveEnd(ubidots_json_format);
				/* add a comma for next value */
				strcat((char*) ubidots_json_format, (char*) string_comma);
				/* append data to the message */
				_JSONSetString(data, ubidots_json_format);
				/* end square bracket ] to append more than 1 value */
				strcat((char*) ubidots_json_format, (char*) string_end_buff);
			}


			if(NULL != context)
			{
				/* \note not yet tested */
				_JSONRemoveEnd(ubidots_json_format);
				_JSONSetContext(context, ubidots_json_format);
			}

			/* add message end key bracket } */
			strcat((char*) ubidots_json_format, (char*) string_end);
			last_parameter =  data->parameter;
			retval = json_succesful_task;
    	}
    	else
    	{
    		retval = json_invalid_parameter;
    	}
    }
    else
    {
    	retval = json_null_ptr;
    }

    return retval;
}

void Ubidots_resetMessage(void)
{
	last_parameter = ustr_imei;
	message_initialized = 0;
}

static inline void effstrcat(uint8_t *dst, uint8_t *src)
{
	uint8_t dst_strlen;

	/* get the string length of the dest array */
	dst_strlen =_GetStrLen(dst);
	/* compensate offset */
	dst += dst_strlen;

	while(*src != '\0')
	{
		*dst = *src;
		src++;
		dst++;
	}
}

bool UbidotsMessageisInit(void)
{
	return message_initialized;
}

bool UbidotsMessageisFinished(void)
{
	return message_initialized;
}

json_error_codes_e UbidotsGetMessage(uint8_t *message)
{
	json_error_codes_e retval;

	if(NULL != message)
	{
		retval = json_succesful_task;
		memset(message, '\0', sizeof(message));
		strcat((char*) message, (char*) ubidots_json_format);
	}
	else
	{
		retval = json_null_ptr;
	}

	return retval;
}

static uint16_t _GetStrLen(uint8_t *str)
{
	return strlen(str);
}

static json_error_codes_e _JSONSetValue(json_data_t *data, uint8_t *str)
{
	strcat((char*) ubidots_json_format, (char*) string_start);
	_UbidotsGetStr(ustr_value, str);
	strcat((char*) str, (char*) string_two_points);
    _ParseInteger(data->data, str);
    strcat((char*) ubidots_json_format, (char*) string_comma);
	_UbidotsGetStr(ustr_timestamp, str);
	strcat((char*) str, (char*) string_two_points);
	_ParseTimestamp(data->timestamp, str);
    strcat((char*) ubidots_json_format, (char*) string_end);
    return json_succesful_task;
}

static json_error_codes_e _JSONSetString(json_string_t *data, uint8_t *str)
{
	strcat((char*) ubidots_json_format, (char*) string_start);
	_UbidotsGetStr(ustr_value, str);
	strcat((char*) str, (char*) string_two_points);
	strcat((char*) str, (char*) data->data);
    strcat((char*) ubidots_json_format, (char*) string_comma);
	_UbidotsGetStr(ustr_timestamp, str);
	strcat((char*) str, (char*) string_two_points);
	_ParseTimestamp(data->timestamp, str);
    strcat((char*) ubidots_json_format, (char*) string_end);
    return json_succesful_task;
}

static json_error_codes_e _JSONSetContext(json_context_t *context, uint8_t *str)
{
	strcat((char*) ubidots_json_format, (char*) string_comma);
	_UbidotsGetStr(ustr_context, str);
	strcat((char*) str, (char*) string_two_points);
    strcat((char*) ubidots_json_format, (char*) string_start);

    _UbidotsGetStr(ustr_lat, str);
	strcat((char*) str, (char*) string_two_points);
    _ParseInteger(context->latitude, str);

	strcat((char*) ubidots_json_format, (char*) string_comma);

    _UbidotsGetStr(ustr_lng, str);
	strcat((char*) str, (char*) string_two_points);
    _ParseInteger(context->longitude, str);

    strcat((char*) ubidots_json_format, (char*) string_end);

    return json_succesful_task;
}

static json_error_codes_e _JSONRemoveEnd(uint8_t *str)
{
	json_error_codes_e retval;
	uint16_t strlen;
	uint16_t i;

	strlen = _GetStrLen(str);

	if(strlen > 0)
	{
		retval = json_succesful_task;

		for(i = 0; i < (strlen-1); i++)
		{
			str++;
		}

		*str = '\0';
	}
	else
	{
		retval = json_null_ptr;
	}

	return retval;
}

static json_error_codes_e _ParseInteger(uint32_t integer, uint8_t *str)
{
	json_error_codes_e retval;
    uint8_t temp_buff[11];

    memset(temp_buff, '\0', 11);

    if(integer > 0)
    {
		retval = json_succesful_task;

		uitoa(integer,(char*) temp_buff, 10);
	    strcat((char*) str, (char*) temp_buff);
    }
    else if(integer == 0)
    {
		retval = json_succesful_task;
	    strcat((char*) str, (char*) '0');

    }
    else
    {
		retval = json_invalid_parameter;
    }

	return retval;
}

static json_error_codes_e _ParseTimestamp(uint32_t ts, uint8_t *str)
{
	json_error_codes_e retval;
    uint8_t temp_buff[11];

    memset(temp_buff, '\0', 11);

    if(ts > 0)
    {
		retval = json_succesful_task;

		uitoa(ts,(char*) temp_buff, 10);
	    strcat((char*) str, (char*) temp_buff);
	    strcat((char*) str, (char*) "000");
    }
    else if(ts == 0)
    {
		retval = json_succesful_task;
	    strcat((char*) str, (char*) '0');

    }
    else
    {
		retval = json_invalid_parameter;
    }

	return retval;
}

static json_error_codes_e _UbidotsGetStr(json_parameters_t parameter, uint8_t *str)
{
	json_error_codes_e retval;
	uint8_t i;

    retval = json_invalid_parameter;

    for(i = 0; i < KAIROS_JSON_SIZE; i++)
    {
    	if(kairos_json[i].parameter == parameter)
    	{
    		strcat((char*) str, (char*) kairos_json[i].strPtr);
    	    retval = json_succesful_task;
    		break;
    	}
    }

    return retval;
}


