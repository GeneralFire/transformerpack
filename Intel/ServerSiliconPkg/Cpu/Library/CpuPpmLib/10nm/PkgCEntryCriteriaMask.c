/** @file
  Code File for CPU Power Management

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

#include "Common/CpuPpmIncludes.h"
#include <Library/PcuMailBoxLib.h>

/**

  This routine is called to program PKG_CST_ENTRY_CRITERIA_MASK_PCU_FUN2_REG.
  It will be call multiple time passing any the SocketNumber to be programmed.

  @param SocketNumber          Socket index that is executing.
  @param PpmPolicy             Pointer to PPM Policy protocol instance.

  @retval                      None

**/
VOID
EFIAPI
InitializePkgCstEntryCriteriaMask (
  UINT8                    SocketNumber,
  PPM_POLICY_CONFIGURATION *PpmPolicy
  )
{

}


/*

  This function porgram PCIE_IN_Lx fields in PKG_CST_CRITERIA_MASK CSR.
  It will be call multiple time passing any the SocketNumber to be programmed.

  @param SocketNumber          Socket index that is executing.
  @param PpmPolicy             Pointer to PPM Policy protocol instance.

  @retval                      None

**/
VOID
EFIAPI
SetupPCIEPkgCstEntryCriteria (
  UINT8                    SocketNumber,
  PPM_POLICY_CONFIGURATION *PpmPolicy
  )
{

}

/**

  This routine is called to program PCU_MISC_CONFIG. It will be call multiple time passing any the SocketNumber to be programmed.

  @param SocketNumber          Socket index that is executing.
  @param PpmPolicy             Pointer to PPM Policy protocol instance.

  @retval                      None

**/
VOID
EFIAPI
InitializeDynamicL1 (
  UINT8                    SocketNumber,
  PPM_POLICY_CONFIGURATION *PpmPolicy
  )
{
  UINT32                           Tempdata = 0;
  B2P_PCU_MISC_CONFIG_DATA_STRUCT  PcuMiscConfigB2pOrData;

  if (PpmPolicy->AdvPwrMgtCtl.DynamicL1Disable) {
    PcuMiscConfigB2pOrData.Data = 0;
    PcuMiscConfigB2pOrData.Bits.DisIioDynamicL1 = 1;
    ReadModifyWritePcuMailbox (
      SocketNumber,
      MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG,
      MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG,
      &Tempdata,
      (UINT32)-1,
      PcuMiscConfigB2pOrData.Data
      );
  }
}
