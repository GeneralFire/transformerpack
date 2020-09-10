/** @file
  AutoGenDefinitions.h
  Internal and external data structures, Macros, and enumerations.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2016 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary    and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.
**/

#ifndef _AUTO_GEN_DEFINITIONS_H_
#define _AUTO_GEN_DEFINITIONS_H_

#define ASSIGNDONE            (0xFF)
//
// Macros for converting Little-Endian to Big-Endian.
//
#define CONV1(x)              (x&0xff)
#define CONV2(x)              ((x>>0)&0xff), ((x>>8)&0xff)
#define CONV3(x)              ((x>>0)&0xff), ((x>>8)&0xff), ((x>>16)&0xff)
#define CONV4(x)              ((x>>0)&0xff), ((x>>8)&0xff), ((x>>16)&0xff), ((x>>24)& 0xff)
#define CONV5(x)              ((x>>0)& 0xff), ((x>>8)& 0xff), ((x>>16)& 0xff), ((x>>24)& 0xff), ((x>>32)& 0xff)
#define CONV6(x)              ((x>>0)& 0xff), ((x>>8)& 0xff), ((x>>16)& 0xff), ((x>>24)& 0xff), ((x>>32)& 0xff), ((x>>40)& 0xff)
#define CONV7(x)              ((x>>0)& 0xff), ((x>>8)& 0xff), ((x>>16)& 0xff), ((x>>24)& 0xff), ((x>>32)& 0xff), ((x>>40)& 0xff), ((x>>48)& 0xff)
#define CONV8(x)              ((x>>0)& 0xff), ((x>>8)& 0xff), ((x>>16)& 0xff), ((x>>24)& 0xff), ((x>>32)& 0xff), ((x>>40)& 0xff), ((x>>48)& 0xff), ((x>>56)& 0xff)
//
// Defines for register initialization table.
//
#define C_(c)      (c)
#define _I(i)      (i)
#define CI(c,i)    (c), (i)
#define RB(r,b)    (((r & 0x0f) << 4) + ((b & 0x0f) << 0))
#define BB(bt,be)  (((bt & 0x0f) << 4) + ((be & 0x0f) << 0))
#define BB2(bt,be) (bt & 0xff), (be & 0xff)
#define AP(ac,pf)  (((ac & 0x07) << 5) + ((pf & 0x1f) << 0))
#define MASK1(m)   CONV1(m)
#define MASK2(m)   CONV2(m)
#define MASK3(m)   CONV3(m)
#define MASK4(m)   CONV4(m)
#define MASK5(m)   CONV5(m)
#define MASK6(m)   CONV6(m)
#define MASK7(m)   CONV7(m)
#define MASK8(m)   CONV8(m)
#define VAL1(m)    CONV1(m)
#define VAL2(m)    CONV2(m)
#define VAL3(m)    CONV3(m)
#define VAL4(m)    CONV4(m)
#define VAL5(m)    CONV5(m)
#define VAL6(m)    CONV6(m)
#define VAL7(m)    CONV7(m)
#define VAL8(m)    CONV8(m)
//
// Byte Enables supported in the CAPSULEINIT Sequences.
//
typedef enum {
  _00000000,
  _00000001,
  _00000010,
  _00000011,
  _00000100,
  _00000101,
  _00000110,
  _00000111,
  _00001000,
  _00001001,
  _00001010,
  _00001011,
  _00001100,
  _00001101,
  _00001110,
  _00001111,
  _00010000,
  _00010001,
  _00010010,
  _00010011,
  _00010100,
  _00010101,
  _00010110,
  _00010111,
  _00011000,
  _00011001,
  _00011010,
  _00011011,
  _00011100,
  _00011101,
  _00011110,
  _00011111,
  _00100000,
  _00100001,
  _00100010,
  _00100011,
  _00100100,
  _00100101,
  _00100110,
  _00100111,
  _00101000,
  _00101001,
  _00101010,
  _00101011,
  _00101100,
  _00101101,
  _00101110,
  _00101111,
  _00110000,
  _00110001,
  _00110010,
  _00110011,
  _00110100,
  _00110101,
  _00110110,
  _00110111,
  _00111000,
  _00111001,
  _00111010,
  _00111011,
  _00111100,
  _00111101,
  _00111110,
  _00111111,
  _01000000,
  _01000001,
  _01000010,
  _01000011,
  _01000100,
  _01000101,
  _01000110,
  _01000111,
  _01001000,
  _01001001,
  _01001010,
  _01001011,
  _01001100,
  _01001101,
  _01001110,
  _01001111,
  _01010000,
  _01010001,
  _01010010,
  _01010011,
  _01010100,
  _01010101,
  _01010110,
  _01010111,
  _01011000,
  _01011001,
  _01011010,
  _01011011,
  _01011100,
  _01011101,
  _01011110,
  _01011111,
  _01100000,
  _01100001,
  _01100010,
  _01100011,
  _01100100,
  _01100101,
  _01100110,
  _01100111,
  _01101000,
  _01101001,
  _01101010,
  _01101011,
  _01101100,
  _01101101,
  _01101110,
  _01101111,
  _01110000,
  _01110001,
  _01110010,
  _01110011,
  _01110100,
  _01110101,
  _01110110,
  _01110111,
  _01111000,
  _01111001,
  _01111010,
  _01111011,
  _01111100,
  _01111101,
  _01111110,
  _01111111,
  _10000000,
  _10000001,
  _10000010,
  _10000011,
  _10000100,
  _10000101,
  _10000110,
  _10000111,
  _10001000,
  _10001001,
  _10001010,
  _10001011,
  _10001100,
  _10001101,
  _10001110,
  _10001111,
  _10010000,
  _10010001,
  _10010010,
  _10010011,
  _10010100,
  _10010101,
  _10010110,
  _10010111,
  _10011000,
  _10011001,
  _10011010,
  _10011011,
  _10011100,
  _10011101,
  _10011110,
  _10011111,
  _10100000,
  _10100001,
  _10100010,
  _10100011,
  _10100100,
  _10100101,
  _10100110,
  _10100111,
  _10101000,
  _10101001,
  _10101010,
  _10101011,
  _10101100,
  _10101101,
  _10101110,
  _10101111,
  _10110000,
  _10110001,
  _10110010,
  _10110011,
  _10110100,
  _10110101,
  _10110110,
  _10110111,
  _10111000,
  _10111001,
  _10111010,
  _10111011,
  _10111100,
  _10111101,
  _10111110,
  _10111111,
  _11000000,
  _11000001,
  _11000010,
  _11000011,
  _11000100,
  _11000101,
  _11000110,
  _11000111,
  _11001000,
  _11001001,
  _11001010,
  _11001011,
  _11001100,
  _11001101,
  _11001110,
  _11001111,
  _11010000,
  _11010001,
  _11010010,
  _11010011,
  _11010100,
  _11010101,
  _11010110,
  _11010111,
  _11011000,
  _11011001,
  _11011010,
  _11011011,
  _11011100,
  _11011101,
  _11011110,
  _11011111,
  _11100000,
  _11100001,
  _11100010,
  _11100011,
  _11100100,
  _11100101,
  _11100110,
  _11100111,
  _11101000,
  _11101001,
  _11101010,
  _11101011,
  _11101100,
  _11101101,
  _11101110,
  _11101111,
  _11110000,
  _11110001,
  _11110010,
  _11110011,
  _11110100,
  _11110101,
  _11110110,
  _11110111,
  _11111000,
  _11111001,
  _11111010,
  _11111011,
  _11111100,
  _11111101,
  _11111110,
  _11111111,
}  BYTE_ENABLES;

//
// PFCT Variations supported in the CAPSULEINIT Sequences.
//
typedef enum {
  _____ = 0,
  ____T,
  ___C_,
  ___CT,
  __F__,
  __F_T,
  __FC_,
  __FCT,
  _P___,
  _P__T,
  _P_C_,
  _P_CT,
  _PF__,
  _PF_T,
  _PFC_,
  _PFCT,
  D____,
  D___T,
  D__C_,
  D__CT,
  D_F__,
  D_F_T,
  D_FC_,
  D_FCT,
  DP___,
  DP__T,
  DP_C_,
  DP_CT,
  DPF__,
  DPF_T,
  DPFC_,
  DPFCT,
} DPFCT_TYPES;

//*************************Auto generated from the Tool**********************//
//
// All possible actions for the init phase of MSP.
//
#define MSP_SET                                                0
#define MSP_DELAY                                              1
#define MSP_SET_DELAY                                          2
#define MSP_FORCE_SET                                          3
#define MSP_POLL                                               4
#define MSP_GET                                                5
#define MSP_SUBTASK                                            6
#define MSP_ACTION_ERROR                                       7

//
// Register access methods (sideband, PCI, etc)
//
#define eSB                                                0
#define ePCI                                               1
#define eBAR                                               2
#define eACCESS_METHOD_ERROR                               3

//
// Definitions for register initialization sequence that tell the common MSP libraries
// how to program a register, whether the offset is 8 or 16 bits, etc.
//
#define xxxxx1xx                                           0x00 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define xxxxx1xR                                           0x01 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define xxxxx1Sx                                           0x02 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define xxxxx1SR                                           0x03 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define xxxxx2xx                                           0x04 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define xxxxx2xR                                           0x05 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define xxxxx2Sx                                           0x06 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define xxxxx2SR                                           0x07 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define xxxxE1xx                                           0x08 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define xxxxE1xR                                           0x09 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define xxxxE1Sx                                           0x0A //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define xxxxE1SR                                           0x0B //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define xxxxE2xx                                           0x0C //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define xxxxE2xR                                           0x0D //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define xxxxE2Sx                                           0x0E //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define xxxxE2SR                                           0x0F //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define xxxMx1xx                                           0x10 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define xxxMx1xR                                           0x11 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define xxxMx1Sx                                           0x12 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define xxxMx1SR                                           0x13 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define xxxMx2xx                                           0x14 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define xxxMx2xR                                           0x15 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define xxxMx2Sx                                           0x16 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define xxxMx2SR                                           0x17 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define xxxME1xx                                           0x18 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define xxxME1xR                                           0x19 //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define xxxME1Sx                                           0x1A //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define xxxME1SR                                           0x1B //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define xxxME2xx                                           0x1C //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define xxxME2xR                                           0x1D //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define xxxME2Sx                                           0x1E //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define xxxME2SR                                           0x1F //  Done=FALSE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define xxNxx1xx                                           0x20 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define xxNxx1xR                                           0x21 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define xxNxx1Sx                                           0x22 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define xxNxx1SR                                           0x23 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define xxNxx2xx                                           0x24 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define xxNxx2xR                                           0x25 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define xxNxx2Sx                                           0x26 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define xxNxx2SR                                           0x27 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define xxNxE1xx                                           0x28 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define xxNxE1xR                                           0x29 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define xxNxE1Sx                                           0x2A //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define xxNxE1SR                                           0x2B //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define xxNxE2xx                                           0x2C //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define xxNxE2xR                                           0x2D //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define xxNxE2Sx                                           0x2E //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define xxNxE2SR                                           0x2F //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define xxNMx1xx                                           0x30 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define xxNMx1xR                                           0x31 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define xxNMx1Sx                                           0x32 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define xxNMx1SR                                           0x33 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define xxNMx2xx                                           0x34 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define xxNMx2xR                                           0x35 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define xxNMx2Sx                                           0x36 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define xxNMx2SR                                           0x37 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define xxNME1xx                                           0x38 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define xxNME1xR                                           0x39 //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define xxNME1Sx                                           0x3A //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define xxNME1SR                                           0x3B //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define xxNME2xx                                           0x3C //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define xxNME2xR                                           0x3D //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define xxNME2Sx                                           0x3E //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define xxNME2SR                                           0x3F //  Done=FALSE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define xBxxx1xx                                           0x40 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define xBxxx1xR                                           0x41 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define xBxxx1Sx                                           0x42 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define xBxxx1SR                                           0x43 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define xBxxx2xx                                           0x44 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define xBxxx2xR                                           0x45 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define xBxxx2Sx                                           0x46 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define xBxxx2SR                                           0x47 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define xBxxE1xx                                           0x48 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define xBxxE1xR                                           0x49 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define xBxxE1Sx                                           0x4A //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define xBxxE1SR                                           0x4B //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define xBxxE2xx                                           0x4C //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define xBxxE2xR                                           0x4D //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define xBxxE2Sx                                           0x4E //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define xBxxE2SR                                           0x4F //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define xBxMx1xx                                           0x50 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define xBxMx1xR                                           0x51 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define xBxMx1Sx                                           0x52 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define xBxMx1SR                                           0x53 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define xBxMx2xx                                           0x54 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define xBxMx2xR                                           0x55 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define xBxMx2Sx                                           0x56 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define xBxMx2SR                                           0x57 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define xBxME1xx                                           0x58 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define xBxME1xR                                           0x59 //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define xBxME1Sx                                           0x5A //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define xBxME1SR                                           0x5B //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define xBxME2xx                                           0x5C //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define xBxME2xR                                           0x5D //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define xBxME2Sx                                           0x5E //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define xBxME2SR                                           0x5F //  Done=FALSE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define xBNxx1xx                                           0x60 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define xBNxx1xR                                           0x61 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define xBNxx1Sx                                           0x62 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define xBNxx1SR                                           0x63 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define xBNxx2xx                                           0x64 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define xBNxx2xR                                           0x65 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define xBNxx2Sx                                           0x66 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define xBNxx2SR                                           0x67 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define xBNxE1xx                                           0x68 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define xBNxE1xR                                           0x69 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define xBNxE1Sx                                           0x6A //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define xBNxE1SR                                           0x6B //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define xBNxE2xx                                           0x6C //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define xBNxE2xR                                           0x6D //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define xBNxE2Sx                                           0x6E //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define xBNxE2SR                                           0x6F //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define xBNMx1xx                                           0x70 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define xBNMx1xR                                           0x71 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define xBNMx1Sx                                           0x72 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define xBNMx1SR                                           0x73 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define xBNMx2xx                                           0x74 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define xBNMx2xR                                           0x75 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define xBNMx2Sx                                           0x76 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define xBNMx2SR                                           0x77 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define xBNME1xx                                           0x78 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define xBNME1xR                                           0x79 //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define xBNME1Sx                                           0x7A //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define xBNME1SR                                           0x7B //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define xBNME2xx                                           0x7C //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define xBNME2xR                                           0x7D //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define xBNME2Sx                                           0x7E //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define xBNME2SR                                           0x7F //  Done=FALSE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define Dxxxx1xx                                           0x80 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define Dxxxx1xR                                           0x81 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define Dxxxx1Sx                                           0x82 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define Dxxxx1SR                                           0x83 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define Dxxxx2xx                                           0x84 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define Dxxxx2xR                                           0x85 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define Dxxxx2Sx                                           0x86 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define Dxxxx2SR                                           0x87 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define DxxxE1xx                                           0x88 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define DxxxE1xR                                           0x89 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define DxxxE1Sx                                           0x8A //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define DxxxE1SR                                           0x8B //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define DxxxE2xx                                           0x8C //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define DxxxE2xR                                           0x8D //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define DxxxE2Sx                                           0x8E //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define DxxxE2SR                                           0x8F //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define DxxMx1xx                                           0x90 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define DxxMx1xR                                           0x91 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define DxxMx1Sx                                           0x92 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define DxxMx1SR                                           0x93 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define DxxMx2xx                                           0x94 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define DxxMx2xR                                           0x95 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define DxxMx2Sx                                           0x96 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define DxxMx2SR                                           0x97 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define DxxME1xx                                           0x98 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define DxxME1xR                                           0x99 //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define DxxME1Sx                                           0x9A //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define DxxME1SR                                           0x9B //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define DxxME2xx                                           0x9C //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define DxxME2xR                                           0x9D //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define DxxME2Sx                                           0x9E //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define DxxME2SR                                           0x9F //  Done=TRUE UseBB2=FALSE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define DxNxx1xx                                           0xA0 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define DxNxx1xR                                           0xA1 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define DxNxx1Sx                                           0xA2 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define DxNxx1SR                                           0xA3 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define DxNxx2xx                                           0xA4 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define DxNxx2xR                                           0xA5 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define DxNxx2Sx                                           0xA6 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define DxNxx2SR                                           0xA7 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define DxNxE1xx                                           0xA8 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define DxNxE1xR                                           0xA9 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define DxNxE1Sx                                           0xAA //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define DxNxE1SR                                           0xAB //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define DxNxE2xx                                           0xAC //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define DxNxE2xR                                           0xAD //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define DxNxE2Sx                                           0xAE //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define DxNxE2SR                                           0xAF //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define DxNMx1xx                                           0xB0 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define DxNMx1xR                                           0xB1 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define DxNMx1Sx                                           0xB2 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define DxNMx1SR                                           0xB3 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define DxNMx2xx                                           0xB4 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define DxNMx2xR                                           0xB5 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define DxNMx2Sx                                           0xB6 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define DxNMx2SR                                           0xB7 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define DxNME1xx                                           0xB8 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define DxNME1xR                                           0xB9 //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define DxNME1Sx                                           0xBA //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define DxNME1SR                                           0xBB //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define DxNME2xx                                           0xBC //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define DxNME2xR                                           0xBD //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define DxNME2Sx                                           0xBE //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define DxNME2SR                                           0xBF //  Done=TRUE UseBB2=FALSE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define DBxxx1xx                                           0xC0 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define DBxxx1xR                                           0xC1 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define DBxxx1Sx                                           0xC2 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define DBxxx1SR                                           0xC3 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define DBxxx2xx                                           0xC4 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define DBxxx2xR                                           0xC5 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define DBxxx2Sx                                           0xC6 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define DBxxx2SR                                           0xC7 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define DBxxE1xx                                           0xC8 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define DBxxE1xR                                           0xC9 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define DBxxE1Sx                                           0xCA //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define DBxxE1SR                                           0xCB //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define DBxxE2xx                                           0xCC //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define DBxxE2xR                                           0xCD //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define DBxxE2Sx                                           0xCE //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define DBxxE2SR                                           0xCF //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define DBxMx1xx                                           0xD0 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define DBxMx1xR                                           0xD1 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define DBxMx1Sx                                           0xD2 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define DBxMx1SR                                           0xD3 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define DBxMx2xx                                           0xD4 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define DBxMx2xR                                           0xD5 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define DBxMx2Sx                                           0xD6 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define DBxMx2SR                                           0xD7 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define DBxME1xx                                           0xD8 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define DBxME1xR                                           0xD9 //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define DBxME1Sx                                           0xDA //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define DBxME1SR                                           0xDB //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define DBxME2xx                                           0xDC //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define DBxME2xR                                           0xDD //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define DBxME2Sx                                           0xDE //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define DBxME2SR                                           0xDF //  Done=TRUE UseBB2=TRUE NeedSecondByte=FALSE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define DBNxx1xx                                           0xE0 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define DBNxx1xR                                           0xE1 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define DBNxx1Sx                                           0xE2 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define DBNxx1SR                                           0xE3 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define DBNxx2xx                                           0xE4 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define DBNxx2xR                                           0xE5 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define DBNxx2Sx                                           0xE6 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define DBNxx2SR                                           0xE7 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define DBNxE1xx                                           0xE8 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define DBNxE1xR                                           0xE9 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define DBNxE1Sx                                           0xEA //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define DBNxE1SR                                           0xEB //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define DBNxE2xx                                           0xEC //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define DBNxE2xR                                           0xED //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define DBNxE2Sx                                           0xEE //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define DBNxE2SR                                           0xEF //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=FALSE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define DBNMx1xx                                           0xF0 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define DBNMx1xR                                           0xF1 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define DBNMx1Sx                                           0xF2 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define DBNMx1SR                                           0xF3 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define DBNMx2xx                                           0xF4 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define DBNMx2xR                                           0xF5 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define DBNMx2Sx                                           0xF6 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define DBNMx2SR                                           0xF7 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=FALSE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define DBNME1xx                                           0xF8 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=FALSE
#define DBNME1xR                                           0xF9 //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=FALSE RankLoop=TRUE
#define DBNME1Sx                                           0xFA //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=FALSE
#define DBNME1SR                                           0xFB //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=8b StrobeLoop=TRUE RankLoop=TRUE
#define DBNME2xx                                           0xFC //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=FALSE
#define DBNME2xR                                           0xFD //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=FALSE RankLoop=TRUE
#define DBNME2Sx                                           0xFE //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=FALSE
#define DBNME2SR                                           0xFF //  Done=TRUE UseBB2=TRUE NeedSecondByte=TRUE MaskPresent=TRUE ConditionalExecution=TRUE Offset=16b StrobeLoop=TRUE RankLoop=TRUE
#define xxxxxxxx                                           0x00 //  Reserved=xx Reserved=xx Reserved=xx Reserved=xx Reserved=xx ChannelDependent=FALSE InstanceDependent=FALSE StringPresent=FALSE
#define xxxxxxxS                                           0x01 //  Reserved=xx Reserved=xx Reserved=xx Reserved=xx Reserved=xx ChannelDependent=FALSE InstanceDependent=FALSE StringPresent=TRUE
#define xxxxxxIx                                           0x02 //  Reserved=xx Reserved=xx Reserved=xx Reserved=xx Reserved=xx ChannelDependent=FALSE InstanceDependent=TRUE StringPresent=FALSE
#define xxxxxxIS                                           0x03 //  Reserved=xx Reserved=xx Reserved=xx Reserved=xx Reserved=xx ChannelDependent=FALSE InstanceDependent=TRUE StringPresent=TRUE
#define xxxxxCxx                                           0x04 //  Reserved=xx Reserved=xx Reserved=xx Reserved=xx Reserved=xx ChannelDependent=TRUE InstanceDependent=FALSE StringPresent=FALSE
#define xxxxxCxS                                           0x05 //  Reserved=xx Reserved=xx Reserved=xx Reserved=xx Reserved=xx ChannelDependent=TRUE InstanceDependent=FALSE StringPresent=TRUE
#define xxxxxCIx                                           0x06 //  Reserved=xx Reserved=xx Reserved=xx Reserved=xx Reserved=xx ChannelDependent=TRUE InstanceDependent=TRUE StringPresent=FALSE
#define xxxxxCIS                                           0x07 //  Reserved=xx Reserved=xx Reserved=xx Reserved=xx Reserved=xx ChannelDependent=TRUE InstanceDependent=TRUE StringPresent=TRUE

#endif // _AUTO_GEN_DEFINITIONS_H_
