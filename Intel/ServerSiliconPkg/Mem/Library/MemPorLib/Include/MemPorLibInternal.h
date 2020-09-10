/** @file
  MemPor.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef  __MEM_POR_LIB_INTERNAL_H__
#define  __MEM_POR_LIB_INTERNAL_H__

#include <PlatformInfoTypes.h>

#define CHOP_IGNORE       0xFF      // Indication to ignore chop in POR table
#define DIMM_TYPE_IGNORE  0xFF      // Indication to ignore DIMM type in POR table
#define SERVER_1S_PLUS    0x1A      // Indicates this configuration is valid for 1Socket, 2Socket, 4Socket, 8Socket server systems
#define SOCKET_2S_PLUS    0x2A      // Indicates this configuration is valid for 2Socket, 4Socket, 8Socket systems
#define SOCKET_4S_PLUS    0x4A      // Indicates this configuration is valid for 4Socket, 8Socket systems
#define NO_MIN_ALLOWED    0xFF      // Indicates there is no minimum socket installed configuration
#define ONE_SOCKET        1
#define TWO_SOCKETS       2
#define FOUR_SOCKETS      4


#pragma pack (push,1)
//
// DIMM Info structure to keep track of DIMM info for POR purposes
//
typedef struct {
  UINT8                           MaxDimm;                // Total number of DIMMs installed in channel
  UINT8                           TotalRanks;             // Total number of ranks present in channel
  UINT8                           DimmType[MAX_POR_DIMM]; // Type of DIMMs installed
  BOOLEAN                         EccPresent;             // TRUE if ECC is present
  BOOLEAN                         DimmX16Present;         // TRUE if x16 DIMM is present
  DRAM_STACK_TYPE                 DimmStack;              // DRAM stack type (SDP, 3DS, DDP)
  EFI_MEMORY_DIMM_CONNECTOR_TYPE  DimmConnectorType;      // DIMM connector type
} POR_CHANNEL_DIMM_INFO;
#pragma pack (pop)


/** Get the system's socket configuration in a format that corresponds with POR table entries.

  @param[out]   PorSocketConfig   The system's socket configuration, corresponding to one of the POR table's SOCKET_*
                                  macros.

  @retval EFI_SUCCESS   The configuration was successfully determined.
  @retval !EFI_SUCCESS  The configuration was not successfully determined.

**/
EFI_STATUS
GetPorSocketConfig (
  OUT UINT8 *PorSocketConfig
  );

EFI_STATUS
GetMemoryPorTable (
  OUT DIMM_POR_ENTRY  **MemPorTable,
  OUT UINT16          *TableLength
  );

#endif // __MEM_POR_LIB_INTERNAL_H__
