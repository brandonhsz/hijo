////////////////////////////Module's Description////////////////////////////////
/*!
 * \file       CM_Q03_URC_Control.h
 * \brief      This is the initial configuration functions header file
 *             made by S4IoT
 * \details    *For more information, please refer to the included documentation
 *             provided.
 * \version    1.0.0
 * \author     S4IoT
 * \copyright  (c) 2018, Mobile IoT Latam S.A.P.I. de CV. All rights reserved.
 */
/////////////////////////End of Module Description//////////////////////////////


#ifndef CM_Q03_URC_CONTROL_H_
#define CM_Q03_URC_CONTROL_H_

#include "stdlib.h"
#include "stdint.h"

#define MODULE_8_SFT(X)		(X & (0x07))
#define DIV_BY_8(X)			(X >> 3)
#define MULT_BY_10(X)		((X << 3) + (X << 1))
#define URC_FLAG_SIZE		(10)

typedef struct
  {
    uint8_t lsb : 7;
    uint8_t msb : 1;
  }div_char_t;

typedef union
{
  uint8_t char_urc;
  div_char_t div_char;
}div_union_t;

typedef struct ptr_struct
{
  div_union_t data;
  struct ptr_struct const *down;
  struct ptr_struct const *right;
}urc_struct_t;


/********************************* ENUMERATORS *******************************/

#define QFTPMDTM_SIZE               (30)
#define QFTPPWD_SIZE                (255)
//TODO: Define size
#define QIND_GEOFENCE_SIZE          (71)
#define QMTRECV_SIZE                (500)

//TODO: Agregar + 1 en el cálculo del string
// Esto evita que coincidan TCP INCOMING y UDP SERVICE
typedef enum
{
    urc_me = 18,
    urc_mi = 22,
    urc_sm = 32,
    urc_sr = 37
}urc_mem_t;

typedef enum
{
    urc_tcp = 39,
    urc_udp = 41,
    urc_tcp_listener = 141,
    urc_tcp_incoming = 123,
    urc_udp_service = 122
}urc_service_type_t;

typedef enum
{
	no_urc_received,
	new_urc,
	invalid_ID
}urc_received_status_t;

/********************************* ENUMERATORS *******************************/

/********************************* MIX PARAMETERS ****************************/

typedef struct
{
    uint16_t u16_0;
    uint16_t u16_1;
}u16_2_t;

typedef struct
{
    uint16_t u16_0;
    uint16_t u16_1;
    uint16_t u16_2;
}u16_3_t;

typedef struct
{
    uint16_t u16_0;
    uint16_t u16_1;
    uint16_t u16_2;
    uint16_t u16_3;
    uint16_t u16_4;
}u16_5_t;

typedef struct
{
    uint16_t u16_0;
    uint16_t u16_1;
    uint16_t u16_2;
    uint16_t u16_3;
    uint16_t u16_4;
    uint16_t u16_5;
}u16_6_t;

typedef struct
{
    uint16_t u16_0;
    uint16_t u16_1;
    uint16_t u16_2;
    uint16_t u16_3;
    uint16_t u16_4;
    uint16_t u16_5;
    uint16_t u16_6;
    uint16_t u16_7;
    uint16_t u16_8;
    uint16_t u16_9;
    uint16_t u16_10;
    uint16_t u16_11;
}u16_12_t;

typedef union
{
    uint16_t all_flags;

    struct
    {
        uint8_t urc_rdy:1;
        uint8_t urc_powered_down:1;
        uint8_t urc_sms_done:1;
        uint8_t qiurc_closed_cid_0:1;
        uint8_t qiurc_closed_cid_1:1;
        uint8_t qiurc_closed_cid_2:1;
        uint8_t qiurc_incoming_full:1;
        uint8_t pdpdeact_cid_0:1;
        uint8_t pdpdeact_cid_1:1;
        uint8_t pdpdeact_cid_2:1;
        uint8_t qiurc_recv_cid_0:1;
        uint8_t qiurc_recv_cid_1:1;
        uint8_t qiurc_recv_cid_2:1;
    }flags_struct;
}no_params_flags_t;




/********************************* MIX PARAMETERS ****************************/

/********************************* NUM PARAMETERS ****************************/



#endif /* CM_Q03_URC_CONTROL_H_ */
