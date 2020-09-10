/** @file
  EvItpDownloadAgent.h

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

#ifndef __EV_ITP_DOWNLOAD_AGENT_H__
#define __EV_ITP_DOWNLOAD_AGENT_H__


#include <Library/DebugLib.h>

#include "SysHost.h"
#include <Ppi/SsaBiosServicesPpi.h>

//
// EV agent command list
//
#define MRC_EVAGENT_CMD_PING                        0x0
#define MRC_EVAGENT_CMD_GET_CALLBACK_REV            0x1
#define MRC_EVAGENT_CMD_GET_TOOL_BUFFER             0x2
#define MRC_EVAGENT_CMD_LOAD_TOOL                   0x3
#define MRC_EVAGENT_CMD_UNLOAD_TOOL                 0x4
#define MRC_EVAGENT_CMD_GET_INPUT_BUFFER            0x5
#define MRC_EVAGENT_CMD_FREE_MEMORY                 0x6
#define MRC_EVAGENT_CMD_RUN_EV_TOOL                 0x7
#define MRC_EVAGENT_CMD_EXIT                        0x8
#define MRC_EVAGENT_CMD_GET_AGENT_VERSION           0x9
#define MRC_EVAGENT_CMD_GET_DLL_VERSION             0xA
#define MRC_EVAGENT_CMD_FLUSH_CACHE                 0xB
#define MRC_EVAGENT_CMD_CHANGE_PRINT_MASK           0xC
#define MRC_EVAGENT_CMD_SAVE_PRINT_MASK             0xD
#define MRC_EVAGENT_CMD_RESTORE_PRINT_MASK          0xE
#define MRC_EVAGENT_CMD_START_AGENT                 0xF
#define MRC_EVAGENT_CMD_START_TARGET_ONLY           0x10
#define MRC_EVAGENT_CMD_FREE_RESULTS                0x11

//
// EV agent status and error code
//
#define MRC_EVAGENT_PENDING_CMD                     1
#define MRC_EVAGENT_NO_PENDING_CMD                  0
#define MRC_EVAGENT_STATUS_READY                    0
#define MRC_EVAGENT_STATUS_BUSY                     1

#define MRC_EVAGENT_CMD_NO_ERROR                    0x0
#define MRC_EVAGENT_CMD_ERROR_UNKNOWN_CMD           0x1
#define MRC_EVAGENT_CMD_ERROR_MALLOC_FAIL           0x2
#define MRC_EVAGENT_CMD_ERROR_INVALID_TOOL_HANDLE   0x3
#define MRC_EVAGENT_CMD_ERROR_INVALID_MEMORY_HANDLE 0x4
#define MRC_EVAGENT_CMD_ERROR_MISSING_CONFIG_DATA   0x5
#define MRC_EVAGENT_CMD_ERROR_INVALID_CONFIG_DATA   0x6
#define MRC_EVAGENT_CMD_ERROR_PARSE_CONFIG_DATA     0x7
#define MRC_EVAGENT_CMD_ERROR_CHECKSUM_FAIL         0x8

//
// Definition of the EV agent command register
//
typedef union {
  struct{
    UINT32 status                    :1;
    UINT32 error_code                :6;
    UINT32 command_pending_execution :1;
    UINT32 command_opcode            :8;
    UINT32 reserved                  :16;
  } Bits;
  UINT32 Data;
} MRC_EV_AGENT_CMD_STRUCT;

typedef EFI_STATUS (EvToolEntryPoint) (SSA_BIOS_SERVICES_PPI* SsaBiosServicesPpi, VOID * Configuration);

#define TOOL_MAJOR_VERSION 1
#define TOOL_MINOR_VERSION 0
#define TOOL_REVISION 0

//
// BSSA EV test type
//
#define BSSA_RMT                0
#define BSSA_MARGIN_1D          1
#define BSSA_MARGIN_2D          2
#define BSSA_POINT_TEST         3
#define BSSA_SET_MARGIN_PARAM   4

/**
  This function runs the EV test based on the test configuration.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       TestType           EV test type which was defined by the BSSA loader interface spec. 
                                      0 - RMT, 1 - Margin1D, 2 - Margin2, 3 - PointTest, 4 - SetMarginParam
  @param[in]       Config             Pointer to test configuration structure.

  @retval EFI_SUCCESS
          EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
RunEvTest (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN UINT8 TestType,
  IN VOID *Config
);
#endif //__EV_ITP_DOWNLOAD_AGENT_H__
