/** @file
  This library exposes the SPD read functions for use during SMM, PEI and DXE.

  This implementation works as an interface between an agent that needs
  to read a DIMM SPD.  The rea functions accept DIMM location information as well
  as the SPD byte offset.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2018 Intel Corporation. <BR>

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

#ifndef _ON_BOARD_SPD_LIB_H_
#define _ON_BOARD_SPD_LIB_H_

#include <MemCommon.h>

//
// Definitions
//
#define FFS_RAW_HEADER_SIZE 0x04 // Size of FFS header
#define SPD_BYTE0_UNDEFINED 0x00 // Undefined due to SPD JEDEC Standard

//
// Ensure proper structure formats
//
#pragma pack(1)
//
// SPD location structure
//
typedef struct {
  UINT8 ChannelLocation;
  UINT8 DimmLocation;
} ON_BOARD_SPD_LOCATION_STRUCTURE;

//
// Memory location and SPD data structure
//
typedef struct {
  ON_BOARD_SPD_LOCATION_STRUCTURE Location;
  UINT8                           SpdData[MAX_SPD_BYTE_DDR];
} ON_BOARD_SPD_TABLE_STRUCTURE;
#pragma pack()

#endif // #ifndef _ON_BOARD_SPD_LIB_H_
