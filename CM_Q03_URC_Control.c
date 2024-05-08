/*
 * CM_Q03_URC_Control.c
 *
 *  Created on: 19/09/2019
 *      Author: S4IoT Alex
 */

#include <CM_Q03_URC_Control.h>


// TEST COMMENT



//
//
//
//const urc_struct_t BU10_2 ={{.char_urc = '2'}, NULL, NULL};
//const urc_struct_t BT10_1 ={{.char_urc = '1'}, NULL, &BU10_2};
//const urc_struct_t BS10_0 ={{.char_urc = '0'}, NULL, &BT10_1};
//
//const urc_struct_t BS9_SPACE ={{.char_urc = ' '}, &BS10_0, NULL};
//const urc_struct_t BS8_COLON ={{.char_urc = ':'}, &BS9_SPACE, NULL};
//const urc_struct_t BS7_S ={{.char_urc = 'S'}, &BS8_COLON, NULL};
//const urc_struct_t BS6_N ={{.char_urc = 'N'}, &BS7_S, NULL};
//
//const urc_struct_t BS5_U ={{.char_urc = 'U'}, &BS6_N, NULL};
//
//
//const urc_struct_t BR10_2 ={{.char_urc = '2'}, NULL, NULL};
//const urc_struct_t BQ10_1 ={{.char_urc = '1'}, NULL, &BR10_2};
//const urc_struct_t BP10_0 ={{.char_urc = '0'}, NULL, &BQ10_1};
//
//const urc_struct_t BP9_SPACE ={{.char_urc = ' '}, &BP10_0, NULL};
//const urc_struct_t BP8_COLON ={{.char_urc = ':'}, &BP9_SPACE, NULL};
//const urc_struct_t BP7_B ={{.char_urc = 'B'}, &BP8_COLON, NULL};
//const urc_struct_t BP6_U ={{.char_urc = 'U'}, &BP7_B, NULL};
//
//const urc_struct_t BO11_2 ={{.char_urc = '2'}, NULL, NULL};
//const urc_struct_t BN11_1 ={{.char_urc = '1'}, NULL, &BO11_2};
//const urc_struct_t BM11_0 ={{.char_urc = '0'}, NULL, &BN11_1};
//
//const urc_struct_t BM10_SPACE ={{.char_urc = ' '}, &BM11_0, NULL};
//const urc_struct_t BM9_COLON ={{.char_urc = ':'}, &BM10_SPACE, NULL};
//const urc_struct_t BM8_T ={{.char_urc = 'T'}, &BM9_COLON, NULL};
//const urc_struct_t BM7_A ={{.char_urc = 'A'}, &BM8_T, NULL};
//const urc_struct_t BM6_T ={{.char_urc = 'T'}, &BM7_A, &BP6_U};
//
//const urc_struct_t BM5_S ={{.char_urc = 'S'}, &BM6_T, &BS5_U};
//
//
//const urc_struct_t BL11_2 ={{.char_urc = '2'}, NULL, NULL};
//const urc_struct_t BK11_1 ={{.char_urc = '1'}, NULL, &BL11_2};
//const urc_struct_t BJ11_0 ={{.char_urc = '0'}, NULL, &BK11_1};
//
//const urc_struct_t BJ10_SPACE ={{.char_urc = ' '}, &BJ11_0, NULL};
//const urc_struct_t BJ9_COLON ={{.char_urc = ':'}, &BJ10_SPACE, NULL};
//const urc_struct_t BJ8_V ={{.char_urc = 'V'}, &BJ9_COLON, NULL};
//const urc_struct_t BJ7_C ={{.char_urc = 'C'}, &BJ8_V, NULL};
//const urc_struct_t BJ6_E={{.char_urc = 'E'}, &BJ7_C, NULL};
//
//const urc_struct_t BJ6_R={{.char_urc = 'R'}, &BJ6_E, &BM5_S};
//
//
//const urc_struct_t BI10_2 ={{.char_urc = '2'}, NULL, NULL};
//const urc_struct_t BH10_1 ={{.char_urc = '1'}, NULL, &BI10_2};
//const urc_struct_t BG10_0 ={{.char_urc = '0'}, NULL, &BH10_1};
//
//const urc_struct_t BG9_SPACE ={{.char_urc = ' '}, &BG10_0, NULL};
//const urc_struct_t BG8_COLON ={{.char_urc = ':'}, &BG9_SPACE, NULL};
//const urc_struct_t BG7_B ={{.char_urc = 'B'}, &BG8_COLON, NULL};
//const urc_struct_t BG6_U ={{.char_urc = 'U'}, &BG7_B, NULL};
//
//const urc_struct_t BG5_P ={{.char_urc = 'P'}, &BG6_U, &BJ6_R};
//
//
//const urc_struct_t BF11_2 ={{.char_urc = '2'}, NULL, NULL};
//const urc_struct_t BE11_1 ={{.char_urc = '1'}, NULL, &BF11_2};
//const urc_struct_t BD11_0 ={{.char_urc = '0'}, NULL, &BE11_1};
//
//const urc_struct_t BD10_SPACE ={{.char_urc = ' '}, &BD11_0, NULL};
//const urc_struct_t BD9_COLON ={{.char_urc = ':'}, &BD10_SPACE, NULL};
//const urc_struct_t BD8_N ={{.char_urc = 'N'}, &BD9_COLON, NULL};
//const urc_struct_t BD7_E ={{.char_urc = 'E'}, &BD8_N, NULL};
//const urc_struct_t BD6_P ={{.char_urc = 'P'}, &BD7_E, NULL};
//
//const urc_struct_t BD5_O ={{.char_urc = 'O'}, &BD6_P, &BG5_P};
//
//
//const urc_struct_t BC11_2 ={{.char_urc = '2'}, NULL, NULL};
//const urc_struct_t BB11_1 ={{.char_urc = '1'}, NULL, &BC11_2};
//const urc_struct_t BA11_0 ={{.char_urc = '0'}, NULL, &BB11_1};
//
//const urc_struct_t BA10_SPACE ={{.char_urc = ' '}, &BA11_0, NULL};
//const urc_struct_t BA9_COLON ={{.char_urc = ':'}, &BA10_SPACE, NULL};
//const urc_struct_t BA8_C ={{.char_urc = 'C'}, &BA9_COLON, NULL};
//const urc_struct_t BA7_S ={{.char_urc = 'S'}, &BA8_C, NULL};
//const urc_struct_t BA6_I ={{.char_urc = 'I'}, &BA7_S, NULL};
//
//const urc_struct_t BA5_D ={{.char_urc = 'D'}, &BA6_I, &BD5_O};
//
//
//const urc_struct_t AZ11_2 ={{.char_urc = '2'}, NULL, NULL};
//const urc_struct_t AY11_1 ={{.char_urc = '1'}, NULL, &BC11_2};
//const urc_struct_t AX11_0 ={{.char_urc = '0'}, NULL, &BB11_1};
//
//const urc_struct_t AX10_SPACE ={{.char_urc = ' '}, &BA11_0, NULL};
//const urc_struct_t AX9_COLON ={{.char_urc = ':'}, &BA10_SPACE, NULL};
//const urc_struct_t AX7_={{.char_urc = 'N'}, NULL, NULL};
//const urc_struct_t AX6_={{.char_urc = 'N'}, NULL, NULL};
//const urc_struct_t AX5_={{.char_urc = 'O'}, NULL, NULL};
//
//const urc_struct_t AW12_2 ={{.char_urc = '2'}, NULL, NULL};
//const urc_struct_t AV12_1 ={{.char_urc = '1'}, NULL, &AW12_2};
//const urc_struct_t AU12_0 ={{.char_urc = '0'}, NULL, &AV12_1};
//
//const urc_struct_t AU11_SPACE ={{.char_urc = ' '}, &AU12_0, NULL};
//const urc_struct_t AU10_COLON ={{.char_urc = ':'}, &AU11_SPACE, NULL};
//const urc_struct_t AU9_E={{.char_urc = 'E'}, &AU10_COLON, NULL};
//const urc_struct_t AU8_S={{.char_urc = 'S'}, &AU9_E, NULL};
//const urc_struct_t AU7_O={{.char_urc = 'O'}, &AU8_S, NULL};
//const urc_struct_t AU6_L={{.char_urc = 'L'}, &AU7_O, NULL};
//
//const urc_struct_t AU5_C={{.char_urc = 'C'}, &AU6_L, &BA5_D};
//
//
//const urc_struct_t AU4_T={{.char_urc = 'T'}, &AU5_C, NULL};
//const urc_struct_t AU3_M={{.char_urc = 'M'}, &AU4_T, NULL}; //FINAL M
//
//
//
//const urc_struct_t AT16_2 ={{.char_urc = '2'}, NULL, NULL};
//const urc_struct_t AS16_1 ={{.char_urc = '1'}, NULL, &AT16_2};
//const urc_struct_t AR16_0 ={{.char_urc = '0'}, NULL, &AS16_1};
//
//const urc_struct_t AR15_COMA ={{.char_urc = ','}, &AR16_0, NULL};
//const urc_struct_t AR14_QUOTES ={{.char_urc = '"'}, &AR15_COMA, NULL};
//const urc_struct_t AR13_v={{.char_urc = 'v'}, &AR14_QUOTES, NULL};
//const urc_struct_t AR12_c={{.char_urc = 'c'}, &AR13_v, NULL};
//const urc_struct_t AR11_e={{.char_urc = 'e'}, &AR12_c, NULL};
//
//const urc_struct_t AR10_r={{.char_urc = 'r'}, &AR11_e, NULL};
//
//const urc_struct_t pos_5={{.div_char ={5,1}}, NULL, NULL};          //**pos5
//const urc_struct_t AQ18_2 ={{.char_urc = '2'}, &pos_5, NULL};
//
//const urc_struct_t pos_4={{.div_char ={4,1}}, NULL, NULL};          //**pos4
//const urc_struct_t AP18_1 ={{.char_urc = '1'}, &pos_4, &AQ18_2};
//
//const urc_struct_t pos_3={{.div_char ={3,1}}, NULL, NULL};          //**pos3
//const urc_struct_t A018_0 ={{.char_urc = '0'}, &pos_3, &AP18_1};
//
//const urc_struct_t A017_COMA ={{.char_urc = ','}, &A018_0, NULL};
//const urc_struct_t A016_QUOTES ={{.char_urc = '"'}, &A017_COMA, NULL};
//const urc_struct_t A015_d={{.char_urc = 'd'}, &A016_QUOTES, NULL};
//const urc_struct_t A014_e={{.char_urc = 'e'}, &A015_d, NULL};
//const urc_struct_t A013_s={{.char_urc = 's'}, &A014_e, NULL};
//const urc_struct_t A012_o={{.char_urc = 'o'}, &A013_s, NULL};
//const urc_struct_t A011_l={{.char_urc = 'l'}, &A012_o, NULL};
//
//const urc_struct_t A010_c={{.char_urc = 'c'}, &A011_l, &AR10_r};
//
//const urc_struct_t AN20_2 ={{.char_urc = '2'}, NULL, NULL};
//const urc_struct_t AM20_1 ={{.char_urc = '1'}, NULL, &AN20_2};
//const urc_struct_t AL20_0 ={{.char_urc = '0'}, NULL, &AM20_1};
//
//const urc_struct_t AL19_COMA ={{.char_urc = ','}, &AL20_0, NULL};
//const urc_struct_t AL18_QUOTES ={{.char_urc = '"'}, &AL19_COMA, NULL};
//const urc_struct_t AL17_t={{.char_urc = 't'}, &AL18_QUOTES, NULL};
//const urc_struct_t AL16_c={{.char_urc = 'c'}, &AL17_t, NULL};
//const urc_struct_t AL15_a={{.char_urc = 'a'}, &AL16_c, NULL};
//const urc_struct_t AL14_e={{.char_urc = 'e'}, &AL15_a, NULL};
//const urc_struct_t AL13_d={{.char_urc = 'd'}, &AL14_e, NULL};
//const urc_struct_t AL12_p={{.char_urc = 'p'}, &AL13_d, NULL};
//const urc_struct_t AL11_d={{.char_urc = 'd'}, &AL12_p, NULL};
//
//const urc_struct_t AL10_p={{.char_urc = 'p'}, &AL11_d, &A010_c};
//
//const urc_struct_t pos_6={{.div_char ={6,1}}, NULL, NULL};          //**pos6
//const urc_struct_t AK18_SPACE={{.char_urc = ' '}, &pos_6, NULL};
//
//const urc_struct_t AJ20_2 ={{.char_urc = '2'}, NULL, NULL};
//const urc_struct_t AI20_1 ={{.char_urc = '1'}, NULL, &AJ20_2};
//const urc_struct_t AH20_0 ={{.char_urc = '0'}, NULL, &AI20_1};
//
//const urc_struct_t AH19_COMA ={{.char_urc = ','}, &AH20_0, NULL};
//const urc_struct_t AH18_QUOTES ={{.char_urc = '"'}, &AH19_COMA, &AK18_SPACE};
//
//const urc_struct_t AH17_g={{.char_urc = 'g'}, &AH18_QUOTES, NULL};
//const urc_struct_t AH16_n={{.char_urc = 'n'}, &AH17_g, NULL};
//const urc_struct_t AH15_i={{.char_urc = 'i'}, &AH16_n, NULL};
//const urc_struct_t AH14_m={{.char_urc = 'm'}, &AH15_i, NULL};
//const urc_struct_t AH13_o={{.char_urc = 'o'}, &AH14_m, NULL};
//const urc_struct_t AH12_c={{.char_urc = 'c'}, &AH13_o, NULL};
//const urc_struct_t AH11_n={{.char_urc = 'n'}, &AH12_c, NULL};
//
//const urc_struct_t AH10_i={{.char_urc = 'i'}, &AH11_n, &AL10_p};
//
//const urc_struct_t AH9_QUOTES ={{.char_urc = '"'}, &AH10_i, NULL};
//const urc_struct_t AR8_SPACE ={{.char_urc = ' '}, &AH9_QUOTES, NULL};
//const urc_struct_t AR7_COLON ={{.char_urc = ':'}, &AR8_SPACE, NULL};
//const urc_struct_t AR6_C ={{.char_urc = 'C'}, &AR7_COLON, NULL};
//const urc_struct_t AR5_R ={{.char_urc = 'R'}, &AR6_C, NULL};
//
//const urc_struct_t AR4_U ={{.char_urc = 'U'}, &AR5_R, NULL};
//
//
//const urc_struct_t AG11_2 ={{.char_urc = '2'}, NULL, NULL};
//const urc_struct_t AF11_1 ={{.char_urc = '1'}, NULL, &AG11_2};
//const urc_struct_t AE11_0 ={{.char_urc = '0'}, NULL, &AF11_1};
//
//const urc_struct_t AE10_SPACE ={{.char_urc = ' '}, &AE11_0, NULL};
//const urc_struct_t AE9_COLON ={{.char_urc = ':'}, &AE10_SPACE, NULL};
//
//const urc_struct_t AE8_E ={{.char_urc = 'E'}, &AE9_COLON, NULL};
//const urc_struct_t AE7_T ={{.char_urc = 'T'}, &AE8_E, NULL};
//const urc_struct_t AE6_A ={{.char_urc = 'A'}, &AE7_T, NULL};
//const urc_struct_t AE5_T ={{.char_urc = 'T'}, &AE6_A, NULL};
//
//const urc_struct_t AD5_E ={{.char_urc = 'E'}, NULL, &AE5_T};
//
//const urc_struct_t AD4_S ={{.char_urc = 'S'}, &AD5_E, &AR4_U};
//
//
//const urc_struct_t AC7_SPACE ={{.char_urc = ' '}, NULL, NULL};
//const urc_struct_t AC6_COLON ={{.char_urc = ':'}, &AC7_SPACE, NULL};
//const urc_struct_t AC5_D ={{.char_urc = 'D'}, &AC6_COLON, NULL};
//
//const urc_struct_t AC4_R ={{.char_urc = 'R'}, &AC5_D, &AD4_S};
//
//
//const urc_struct_t AB10_2 ={{.char_urc = '2'}, NULL, NULL};
//const urc_struct_t AA10_1 ={{.char_urc = '1'}, NULL, &AB10_2};
//const urc_struct_t Z10_0 ={{.char_urc = '0'}, NULL, &AA10_1};
//
//const urc_struct_t Z9_SPACE ={{.char_urc = ' '}, &Z10_0, NULL};
//const urc_struct_t Z8_COLON ={{.char_urc = ':'}, &Z9_SPACE, NULL};
//const urc_struct_t Z7_N ={{.char_urc = 'N'}, &Z8_COLON, NULL};
//const urc_struct_t Z6_E ={{.char_urc = 'E'}, &Z7_N, NULL};
//const urc_struct_t Z5_P ={{.char_urc = 'P'}, &Z6_E, NULL};
//
//const urc_struct_t Z4_O ={{.char_urc = 'O'}, &Z5_P, &AC4_R};
//
//const urc_struct_t pos_2={{.div_char ={2,1}}, NULL, NULL};          //**pos2
//const urc_struct_t Y8_S ={{.char_urc = 'S'}, &pos_2, NULL};
//
//const urc_struct_t X18_COMA ={{.char_urc = ','}, NULL, NULL};
//const urc_struct_t X17_QUOTES ={{.char_urc = '"'}, &X18_COMA, NULL};
//const urc_struct_t X16_E ={{.char_urc = 'E'}, &X17_QUOTES, NULL};
//const urc_struct_t X15_C ={{.char_urc = 'C'}, &X16_E, NULL};
//const urc_struct_t X14_N ={{.char_urc = 'N'}, &X15_C, NULL};
//const urc_struct_t X13_E ={{.char_urc = 'E'}, &X14_N, NULL};
//const urc_struct_t X12_F ={{.char_urc = 'F'}, &X13_E, NULL};
//const urc_struct_t X11_O ={{.char_urc = 'O'}, &X12_F, NULL};
//const urc_struct_t X10_E ={{.char_urc = 'E'}, &X11_O, NULL};
//const urc_struct_t X9_G ={{.char_urc = 'G'}, &X10_E, NULL};
//
//const urc_struct_t X8_QUOTES ={{.char_urc = '"'}, &X9_G, &Y8_S};
//
//const urc_struct_t X7_SPACE ={{.char_urc = ' '}, &X8_QUOTES, NULL};
//const urc_struct_t X6_COLON ={{.char_urc = ':'}, &X7_SPACE, NULL};
//const urc_struct_t X5_D ={{.char_urc = 'D'}, &X6_COLON, NULL};
//
//const urc_struct_t X4_N ={{.char_urc = 'N'}, &X5_D, &Z4_O};
//
//
//
//const urc_struct_t W4_G ={{.char_urc = 'G'}, NULL, &X4_N};
//
//
//const urc_struct_t W3_I ={{.char_urc = 'I'}, &W4_G, &AU3_M}; //I
//
//
//const urc_struct_t V7_T={{.char_urc = 'T'}, NULL, NULL};
//const urc_struct_t U7_I={{.char_urc = 'I'}, NULL, &V7_T};
//
//const urc_struct_t U6_S={{.char_urc = 'S'}, &U7_I, NULL};
//
//
//const urc_struct_t T7_M={{.char_urc = 'I'}, NULL, NULL};
//const urc_struct_t S7_E={{.char_urc = 'I'}, NULL, &T7_M};
//
//const urc_struct_t S6_R={{.char_urc = 'R'}, &S7_E, &U6_S};
//
//
//const urc_struct_t R10_SPACE ={{.char_urc = ' '}, NULL, NULL};
//const urc_struct_t R9_COLON ={{.char_urc = ':'}, &R10_SPACE, NULL};
//const urc_struct_t R8_D={{.char_urc = 'D'}, &R9_COLON, NULL};
//const urc_struct_t R7_W={{.char_urc = 'W'}, &R8_D, NULL};
//
//const urc_struct_t Q7_U={{.char_urc = 'U'}, NULL, &R7_W};
//
//const urc_struct_t Q6_P={{.char_urc = 'P'}, &Q7_U, NULL};
//
//
//const urc_struct_t P6_O={{.char_urc = 'O'}, NULL, &Q6_P};
//
//
//const urc_struct_t O7_K={{.char_urc = 'K'}, NULL, NULL};
//
//const urc_struct_t N11_SPACE ={{.char_urc = ' '}, NULL, NULL};
//const urc_struct_t N10_COLON ={{.char_urc = ':'}, &N11_SPACE, NULL};
//const urc_struct_t N9_M={{.char_urc = 'M'}, &N10_COLON, NULL};
//const urc_struct_t N8_T={{.char_urc = 'T'}, &N9_M, NULL};
//const urc_struct_t N7_D={{.char_urc = 'D'}, &N8_T, &O7_K};
//
//const urc_struct_t N6_M={{.char_urc = 'M'}, &N7_D, &P6_O};
//
//const urc_struct_t M6_L={{.char_urc = 'L'}, NULL, &N6_M};
//
//const urc_struct_t L6_G={{.char_urc = 'G'}, NULL, &M6_L};
//
//const urc_struct_t K6_D={{.char_urc = 'D'}, NULL, &L6_G};
//
//
//const urc_struct_t J7_W={{.char_urc = 'W'}, NULL, NULL};
//const urc_struct_t L7_L={{.char_urc = 'L'}, NULL, &J7_W};
//
//const urc_struct_t I6_C={{.char_urc = 'C'}, &L7_L, NULL};
//
//
//const urc_struct_t I5_P ={{.char_urc = 'P'}, &I6_C, NULL};
//const urc_struct_t I4_T ={{.char_urc = 'T'}, &I5_P, NULL};
//const urc_struct_t I3_F ={{.char_urc = 'F'}, &I4_T, &W3_I}; //F
//
//
//
//const urc_struct_t I2_q ={{.char_urc = 'Q'}, &I3_F, NULL}; //*Q
//
//
//
//
//
//const urc_struct_t H3_R={{.char_urc = 'R'}, NULL, NULL};
//
//
//const urc_struct_t G3_P={{.char_urc = 'P'}, NULL, &H3_R};
//
//
//const urc_struct_t F5_I={{.char_urc = 'I'}, NULL, NULL};
//const urc_struct_t F4_T={{.char_urc = 'T'}, &F5_I, NULL};
//
//const urc_struct_t F3_M={{.char_urc = 'M'}, &F4_T, &G3_P};
//
//
//const urc_struct_t E3_F={{.char_urc = 'F'}, NULL, &F3_M};
//
//
//const urc_struct_t D5_I={{.char_urc = 'I'}, NULL, NULL};
//const urc_struct_t D4_S={{.char_urc = 'S'}, &D5_I, NULL};
//
//const urc_struct_t D3_D={{.char_urc = 'D'}, &D4_S, &E3_F};
//
//
//const urc_struct_t D2_C={{.char_urc = 'C'}, &D3_D, NULL}; //*C
//
//
//
//
//const urc_struct_t D1_PLUS={{.char_urc = '+'}, &D2_C, NULL};
//
//
//
//const urc_struct_t pos_1={{.div_char ={1,1}}, NULL, NULL};       //**pos1
//const urc_struct_t C3_W={{.char_urc = 'W'}, &pos_1, NULL};
//const urc_struct_t C2_O={{.char_urc = 'O'}, &C3_W, NULL};
//const urc_struct_t C1_P={{.char_urc = 'P'}, &C2_O, &D1_PLUS};
//
//
//const urc_struct_t pos_0={{.div_char ={0,1}}, NULL, NULL};       //**pos0
//const urc_struct_t B3_Y={{.char_urc = 'Y'}, &pos_0, NULL};
//const urc_struct_t B2_D={{.char_urc = 'D'}, &B3_Y, NULL};
//const urc_struct_t B1_R={{.char_urc = 'R'}, &B2_D, &C1_P};
//
//
//



