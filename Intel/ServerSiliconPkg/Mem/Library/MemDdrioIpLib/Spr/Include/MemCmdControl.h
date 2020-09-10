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
#include <SysHost.h>

//
// SPR DDR4 CMD CTL signal number.
// Note: The same as length of array SignalToRegisterMappingSprDdr4
//
#define MAX_CMD_CTL_ARRAY_SPR_DDR4_SUB_CHA  21
#define MAX_CMD_CTL_ARRAY_SPR_DDR4_SUB_CHB  23
#define MAX_CMDCTL_ARRAY_SPR_DDR4     (MAX_CMD_CTL_ARRAY_SPR_DDR4_SUB_CHA + MAX_CMD_CTL_ARRAY_SPR_DDR4_SUB_CHB)

//
// SPR DDR5 CMD CTL signal number.
// Note: DDR5_TODO, DDR5 code needs cleanup. https://hsdes.intel.com/appstore/article/#/1507335428.
//
#define MAX_CMD_CTL_ARRAY_SPR_DDR5_SUB_CHA  22
#define MAX_CMD_CTL_ARRAY_SPR_DDR5_SUB_CHB  20
#define MAX_CMDCTL_ARRAY_SPR_DDR5     (MAX_CMD_CTL_ARRAY_SPR_DDR5_SUB_CHA + MAX_CMD_CTL_ARRAY_SPR_DDR5_SUB_CHB)

#define MAX_CMD_DELAY_DDR5 511
#define MIN_CMD_DELAY_DDR5 -511

typedef struct {
  UINT8   RegIndex;
  UINT8   SubCh;
} SIGNAL_DESCRIPTOR;

//
// Function prototypes that are private to this lib
//

/**
  Gets pointer to CMD/CTL signal group and number of elements in the group.

  @param Host:             Pointer to SYSHOST.
  @param Group:            CMD/CTL signal group to get.
  @param CmdCtlGroupSize:  Ponter to number of elements in the CMD/CTL signal group.
  @param CmdCtlGroupPtr:   Pointer to CMD/CTL signal group.

  @retval MRC_STATUS_SUCCESS or MRC_STATUS_GROUP_NOT_SUPPORTED

**/
MRC_STATUS
GetCmdCtlGroupList (
  PSYSHOST          Host,
  MRC_GT            Group,
  UINT8             *CmdCtlGroupSize,
  SIGNAL_DESCRIPTOR **CmdCtlGroupPtr
  );

/**
  Returns the desired clk CSR offset

  @param [IN] Clk: Clock number

  @retval: Clk CSR
**/
UINT32
GetClkRegOffset (
  IN UINT8 Clk
  );


/**
  Gets the CSR address for a CMD/CTL/CLK CSR index

  @param[in] Host       - Pointer to SYSHOST
  @param[in] RegIndex   - DDRCC_CTRL0_BIT register index

  @retval CSR address for given register index if the index is valid
  @retval 0xFFFFFFFF if register not found

**/
UINT32
GetCmdGroupAddress (
  IN  PSYSHOST Host,
  IN  UINT8    RegIndex
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
  Multi-use function to either get or set command delays based on the provided group number

  @param Host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param SubCh:    SubCh number
  @param RegIndex: Register Index
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program or offset

  @retval Pi delay value

**/
MRC_STATUS
GetSetCmdDelay (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    SubCh,
  UINT8    RegIndex,
  UINT8    mode,
  INT16    *value
  );

#endif   // __MEM_CMD_CONTROL_H__
