/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2017 Intel Corporation. <BR>

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

#include <Library/MemFmcIpLib.h>
#include "Include/MemFmcRegs.h"
#include "Include/MemFmcIpLibInternal.h"


/**

Routine Description: GetRegIndex will return the index of the NVMDIMM
Mailbox Register that are #defined in the NVMCTLR Register header.

Arguments.

  @param pDimm       - Pointer to AEP_DIMM structure, this will
                     have the Dimm struct.
  @param RegSPA      - Register that is in MBSpa.
  @retval Index of the register if found, else -1.

**/
INT16
GetRegIndex (
   UINT64    Reg
   )
{
  INT16      Index;

  switch (Reg) {
  case MB_SMM_CMD_FNV_DPA_MAPPED_0_REG:
    // Command Register
    Index = MB_REG_COMMAND;
    break;
  case MB_SMM_NONCE_0_FNV_DPA_MAPPED_0_REG:
    Index = MB_REG_NONCE0;
    break;
  case MB_SMM_NONCE_1_FNV_DPA_MAPPED_0_REG:
    Index = MB_REG_NONCE1;
    break;
  case MB_SMM_STATUS_FNV_DPA_MAPPED_0_REG:
    Index = MB_REG_STATUS;
    break;
  default:
    Index = -1;
    break;
  }
  return (Index);
}

