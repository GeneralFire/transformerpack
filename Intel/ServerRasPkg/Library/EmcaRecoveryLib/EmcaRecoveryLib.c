/** @file
  Implementation of generate RAS global data access for silicon library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#include <Base.h>
#include <Library/EmcaRecoveryLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/WorkaroundLib.h>


/*
  Attempts to recovery from correctable error.

  This function tries to recovery from a correctable error from a specific machine check bank.
  If the recovery attempt succeeds, it will return TRUE and updated the status register accordingly.

  @param[in]     McBankIdx              The machine check bank index.
  @param[in]     Socket                 The socket number of the machine check bank.
  @param[in,out] McSig                  The signature of the machine check registers.

*/
BOOLEAN
EFIAPI
AttemptRecovery (
  IN UINT32                           McBankIdx,
  IN UINT8                            Socket,
  IN BOOLEAN                          EmcaSetFwUpdate,
  IN OUT EMCA_MC_SIGNATURE_ERR_RECORD *McSig
  )
{
  IA32_MCI_STATUS_MSR_STRUCT         McSts;
  
  if (SkipFwUpdSettingWa1606892119 ()) {
    return FALSE;
  }

  if (EmcaSetFwUpdate) {
    McSts.Data = McSig->Signature.McSts;

    if (McSts.Bits.val == 1) {
      McSts.Data |= MCA_FIRMWARE_UPDATED_BIT;
      McSig->Signature.McSts = McSts.Data;
      return TRUE;
    }
  }

  return FALSE;
}
