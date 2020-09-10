/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef _MEM_POPULATION_POR_LIB_H_
#define _MEM_POPULATION_POR_LIB_H_

#include "SysHost.h"

#define MAX_MPOR_POP    500 // This should be set appropriately to cap the table size
#define MPOR_BITMAPS_8  (((MAX_CH * MAX_DIMM) / 8) + (((MAX_CH * MAX_DIMM) % 8) != 0))

//
// Define MAX_DIE here until it is defined at the system level
//
#define MAX_DIE 4

#define MPOR_SIGNATURE  0x77807982 // "MPOR"
#define MPOR_REVISION   0x0102

#define NOT_VALIDATED {0, 0}
#define VALIDATED     {1, 0}
#define NO_DIMMS      0
#define D0            BIT0
#define D1            BIT1
#define D2            BIT2
#define D3            BIT3
#define D4            BIT4
#define D5            BIT5
#define D6            BIT6
#define D7            BIT7

#pragma pack(1)
typedef struct {
  UINT8 Validated:1; // Set if the population is validated, otherwise it is only supported
  UINT8 Reserved:7;
} MPOR_FLG;
#pragma pack()

typedef struct {
  MPOR_FLG Flags;
  UINT8    DdrBitmap8[MPOR_BITMAPS_8];   // Array of 8-slot DDR population bitmaps
  UINT8    DcpmmBitmap8[MPOR_BITMAPS_8]; // Array of 8-slot DCPMM population bitmaps
} MPOR_POP;

typedef struct {
  UINT32   Signature;    // "MPOR" - Memory Population POR Table
  UINT16   Revision;     // Major/Minor Table revision (0x0102)
  UINT8    DieCount;     // Number of dies per socket
  UINT8    Channels;     // Number of channels per socket
  UINT8    Slots;        // Number of slots per channel
  UINT16   Populations;  // Number of POR memory populations
  MPOR_POP Population[]; // Unbounded array of POR populations
} MPOR_TBL;


/**

Get the pointer to the POR table.

  @param Host  - Pointer to sysHost

  @retval      - Pointer to the POR table

**/
MPOR_TBL*
GetPorTablePtr (
  PSYSHOST Host
  );

/**

Check current memory population against the Memory POR table

  @param Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
EnforcePopulationPor (
  PSYSHOST Host
);

#endif //_MEM_POPULATION_POR_LIB_H_
