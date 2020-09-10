/** @file
  Header file for MemorySubClass Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef _MEMORY_SUBCLASS_H_
#define _MEMORY_SUBCLASS_H_

#include <IndustryStandard/SmBios.h>
#include <Protocol/Smbios.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiLib.h>
#include <Library/HiiLib.h>
#include <Library/HobLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/BaseMemoryLib.h>

#include <Guid/MemoryMapData.h>

#define MAX_APACHEPASS_DIMM_SIZE   512    // in GiB
#define MAX_APACHEPASS_DIMM_NUM    6
#define MAXIMUM_CHANNEL_PER_SOCKET 8    //up to 8 channels per socket, for SMBIOS type17 string table
#define MAXIMUM_IMC_PER_SOCKET     4    //up to 4 IMCs per socket, for SMBIOS type17 string table

//
// Currently the only voltage supported by DDR4
// Module operable and endurant 1.20V
//
#define SPD_VDD_120 3
//
// Currently the only voltage supported by DDR5
// module Nominal, Operable and Endurant 1.10V
//
#define SPD_VDD_110 0x0 // DDR5 SPD spec proposed

//
// Legacy CPU socket ID
//
#define LEGACY_CPU      0

#define MemoryTotalWidth_DDR5 80
#define MemoryTotalWidth_DDR4 72
#define MemoryDataWidth_DDR5  64

//
// GUID for Memory Subclass Driver
//
#define EFI_MEMORY_SUBCLASS_DRIVER_GUID \
  { \
    0xef17cee7, 0x267d, 0x4bfd, { 0xa2, 0x57, 0x4a, 0x6a, 0xb3, 0xee, 0x85, 0x91 }  \
  }

extern UINT8 MemorySubClassStrings[];

#endif // _MEMORY_SUBCLASS_H_

