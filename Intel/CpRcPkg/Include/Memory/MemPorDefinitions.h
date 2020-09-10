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

#ifndef __MEM_POR_DEFINITIONS_H__
#define __MEM_POR_DEFINITIONS_H__

#include <PlatformInfoTypes.h>

#define MAX_POR_DIMM     2         // Number of DIMMs supported in the Memory POR tables

//
// DIMM Stack type
//
typedef enum {
  StackSdp = 0,       // Single stack
  Stack3ds,           // 3DS stack
  StackDdp,           // Multi load stack (DDP type)
  StackIgnore,        // Ignore stack type
  StackTypeMax        // Maximum stack type value
} DRAM_STACK_TYPE;

//
// Intel POR memory frequency table entry
//
typedef struct DimmPOREntryStruct {
  UINT32                          ProcType;               // Platform specific CPU family (e.g. CPU_FAMILY_SKX, CPU_FAMILY_ICX)
  UINT8                           PorSocketConfig;        // SOCKET_1S, SOCKET_2S, etc.
  UINT8                           Chop;                   // Processor chop (e.g. TypeLcc, TypeXcc)
  UINT8                           DramType;               // SPD_TYPE_DDR4 or SPD_TYPE_DDR5 (from keybyte in SPD)
  UINT8                           Spc;                    // Number of DIMM slots per channel
  UINT8                           Dpc;                    // DIMMs populated per channel
  UINT8                           DimmType[MAX_POR_DIMM]; // DIMM type (e.g. UDIMM, RDIMM, LRDIMM)
  DRAM_STACK_TYPE                 DimmStack;              // DIMM stack type (e.g. SDP, 3DS)
  EFI_MEMORY_DIMM_CONNECTOR_TYPE  DimmConnectorType;      // DIMM connector type (e.g. PTH, SMT)
  UINT8                           Vdd;                    // DDR voltage (eg. 1.2v)
  UINT8                           MaxChannelRanks;        // Maximum number of ranks allowed in the channel
  UINT8                           PorFreq;                // Highest POR frequency for the configuration
  UINT8                           StretchFreq;            // TEST ONLY - used for testing frequencies higher than POR
} DIMM_POR_ENTRY;

#endif // #ifndef __MEM_POR_DEFINITIONS_H__
