/** @file
  API for the Memory Pkg Delay library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#ifndef _PKG_DELAY_LIB_H_
#define _PKG_DELAY_LIB_H_

#define PERCENT     100

typedef struct {
  UINT8   ch;                         ///< Channel
  UINT8   iog;                        ///< IO Group #
  UINT8   side;                       ///< Side A or B
} IoGroupStruct;


typedef struct {
  UINT8   ch;                              ///< Channel
  UINT8   clk;                             ///< Clock number (4 clocks per channel)
} IoGroupClkStruct;

/**
  Get Command Delay Value table and number of table entries

  @param[out]  *Size       Pointer to number of entries in the Table
  @param[out]  **CmdDelay  Pointer to Command Delay Table
  @param[out]  **CmdTable  Pointer to Command table signal

  @retval EFI_SUCCESS   Table Present
  @retval !EFI_SUCCESS  Table not Present

**/
EFI_STATUS
EFIAPI
GetCmdDelay (
  OUT  UINT8              *Size,
  OUT  UINT8              **CmdDelay,
  OUT  IoGroupStruct      **CmdTable
  );

/**
  Get Control Delay Value table and number of table entries

  @param[out]  *Size       Pointer to number of entries in the Table
  @param[out]  **CtlDelay  Pointer to Control Delay Table
  @param[out]  **CtlTable  Pointer to Control Table Signal

  @retval EFI_SUCCESS   Table Present
  @retval !EFI_SUCCESS  Table not Present

**/
EFI_STATUS
EFIAPI
GetCtlDelay (
  OUT  UINT8              *Size,
  OUT  UINT8              **CtlDelay,
  OUT  IoGroupStruct      **CtlTable
  );

/**
  Get Clock Delay Value table and  number of table entries

  @param[out]  *Size        Pointer to number of entries in the Table
  @param[out]  **ClkDelay   Pointer to Clock Delay Value Table
  @param[out]  **ClkTable   Pointer to Clock Table Signal

  @retval EFI_SUCCESS   Table Present
  @retval !EFI_SUCCESS  Table not Present

**/
EFI_STATUS
EFIAPI
GetClkDelay (
  OUT  UINT8                 *Size,
  OUT  UINT16                **ClkDelay,
  OUT  IoGroupClkStruct      **ClkTable
  );

#endif // _PKG_DELAY_LIB_H_
