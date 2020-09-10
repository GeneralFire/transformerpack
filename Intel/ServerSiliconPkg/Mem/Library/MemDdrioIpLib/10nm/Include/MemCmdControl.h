/** @file
  MemCmdControl.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#ifndef  __MEM_CMD_CONTROL_H__
#define  __MEM_CMD_CONTROL_H__

#include <ReferenceCodeDataTypes.h>
#include "SysHost.h"
#include <Memory/MemCmdCtlClkCommon.h>
#include <Library/MemCmdControlTablesLib.h>

//
// Function prototypes that are private to this lib
//

/**

  Gets the CSR address and cache index for given IoGroup and side

  @param[in] Host       - Pointer to sysHost, the system Host (root) structure struct
  @param[in] ioGroup    - Pi group number
  @param[in] side       - Pi group side
  @param[out] cmdIndex  - Cache index for the CSR

  @retval CSR Address - CSR address for given Pi group number and side

**/

UINT32 
GetCmdGroupAddress10nm (
  IN PSYSHOST Host, 
  IN UINT8 ioGroup, 
  IN UINT8 side, 
  OUT UINT8 *cmdIndex
  );

/**
  Muli-use function to either get or set command tx eq

  @param[in]      Host    - Pointer to SYSHOST
  @param[in]      Socket  - Socket number
  @param[in]      Ch      - Channel number
  @param[in]      Mode    - GSM_READ_CSR - Read the data from hardware and not cache
                            GSM_READ_ONLY - Do not write
                            GSM_WRITE_OFFSET - Write offset and not value
                            GSM_FORCE_WRITE - Force the write
  @param[in,out]  Value   - Value or offset to program. Actual value returned to caller.

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetSetCmdTxEq (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  );

/**
  Evaluates the CMD margins and determines the appropriate offset

  @param [in    ] Host       - Pointer to Host structure
  @param [in    ] socket     - Processor socket within the system (0-based)
  @param [in    ] ch         - Memory channel number
  @param [in    ] ckEnabled  - Array of clocks enabled
  @param [in    ] ctlEnabled - Array of control indexes
  @param [in out] cmdLeft    - Array of command values for the left side
  @param [in out] cmdRight   - Array of command values for the right side
  @param [in    ] group      - Command group
  @param [in    ] cmdSignal  - Command signal
  @param [   out] cmdOffset  - Command offset

  @retval MRC_STATUS
**/
VOID
EvaluateCMDMargins(
  IN     PSYSHOST Host,
  IN     UINT8    socket,
  IN     UINT8    ch,
  IN     UINT8    ckEnabled[MAX_CLK],
  IN     UINT8    ctlIndex[MAX_CLK],
  IN OUT INT16    (*cmdLeft)[MAX_CLK],
  IN OUT INT16    (*cmdRight)[MAX_CLK],
  IN     MRC_GT   (*group)[MAX_CLK],
  IN     GSM_CSN  (*cmdSignal)[MAX_CLK],
  OUT    INT16    *cmdOffset
);

/**
  Gets the silicon pi group from the signal name

  @param [in    ] Host        - Pointer to SYSHOST
  @param [in    ] sig         - Enumerated signal name
  @param [   out] signalGroup - Signal group pointer

  @retval MRC_STATUS
**/
MRC_STATUS
SignalToPiGroup (
  IN     PSYSHOST             Host,
  IN     GSM_CSN              sig,
     OUT struct signalIOGroup *signalGroup
  );

/**
  Gets pointer to CmdCtl group list and number of elements in the list

  @param [in    ] Host       - Pointer to SYSHOST
  @param [in    ] group      - CmdCtl group to get
  @param [   out] maxIoGroup - Ponter to number of elements in the list
  @param [   out] iogPtr     - Pointer to the list

  @retval MRC_STATUS_SUCCESS or MRC_STATUS_GROUP_NOT_SUPPORTED
**/
MRC_STATUS
GetCmdCtlGroupList (
  IN     PSYSHOST       Host,
  IN     MRC_GT         group,
     OUT UINT8          *maxIoGroup,
     OUT struct ioGroup **iogPtr
  );
  
/*
  Function to obtain pico seconds per pi ticks for each supported frequency

  @param [in] RatioIndex - Frequency ratio index value

  @retval Pico seconds per pi tick
*/
UINT8
GetPicoSecondsPerPiTickForFreq (
  IN UINT8 RatioIndex
  );

/**
  Multi-use function to either get or set command delays based on the provided group number

  @param [in    ] Host    - Pointer to SYSHOST
  @param [in    ] socket  - Socket number
  @param [in    ] ch      - Channel number
  @param [in    ] ioGroup - Group number
  @param [in    ] side    - Side number
  @param [in    ] mode    - GSM_READ_CSR - Read the data from hardware and not cache
                            GSM_READ_ONLY - Do not write
                            GSM_WRITE_OFFSET - Write offset and not value
                            GSM_FORCE_WRITE - Force the write
  @param [out   ] value   - Value to program or offset

  @retval Pi delay value
**/
MRC_STATUS
GetSetCmdDelay (
  IN     PSYSHOST Host,
  IN     UINT8    socket,
  IN     UINT8    ch,
  IN     UINT8    ioGroup,
  IN     UINT8    side,
  IN     UINT8    mode,
  OUT    INT16    *value
  );

#endif   // __MEM_CMD_CONTROL_H__
