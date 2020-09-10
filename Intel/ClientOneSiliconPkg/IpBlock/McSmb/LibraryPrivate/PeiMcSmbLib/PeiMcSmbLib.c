/** @file
  Configures PCH McSmb device

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#include <Ppi/SiPolicy.h>
#include <Library/DebugLib.h>
#include <Library/PeiMcSmbLib.h>
#include <Register/McSmbRegs.h>
#include <Register/PchPcrRegs.h>
#include <Library/PchPcrLib.h>

/**
  Performs McSmb configuration

  @param[in]  McSmbHandle       Pointer to McSmb handle configuration structure

  @retval None
**/
VOID
ConfigureMcSmb (
  IN  MCSMB_HANDLE        *McSmbHandle
)
{
  PCH_SBI_PID             McSmbPid;
  UINT32                  Data32;
  MCSMB_PRIVATE_CONFIG    *PrivateConfig;

  PrivateConfig     = McSmbHandle->PrivateConfig;
  McSmbPid          = McSmbHandle->Controller->Pid;

  //
  // Configure SMBus PHY Global Timing Register
  //
  Data32 = 0;
  Data32 |= PrivateConfig->DeGlitchPulse << N_PCH_MCSMB_PCR_SPGT_DG;
  switch (PrivateConfig->SpeedMode) {
    case Standard0:
      Data32 |= V_PCH_MCSMB_PCR_SPGT_SPD_STANDARD0 << N_PCH_MCSMB_PCR_SPGT_SPD;
      break;
    case Standard1:
      Data32 |= V_PCH_MCSMB_PCR_SPGT_SPD_STANDARD1 << N_PCH_MCSMB_PCR_SPGT_SPD;
      break;
    case Fast:
      Data32 |= V_PCH_MCSMB_PCR_SPGT_SPD_FAST << N_PCH_MCSMB_PCR_SPGT_SPD;
      break;
    case FastPlus:
      Data32 |= V_PCH_MCSMB_PCR_SPGT_SPD_FASTPLUS << N_PCH_MCSMB_PCR_SPGT_SPD;
      break;
  }
  PchPcrAndThenOr32 (
    McSmbPid,
    R_PCH_MCSMB_PCR_SPGT,
    (UINT32)~(B_PCH_MCSMB_PCR_SPGT_DG | B_PCH_MCSMB_PCR_SPGT_SPD),
    Data32
  );

  //
  // Configure SMBus PHY Master Timing Register
  //
  Data32 = 0;
  Data32 |= PrivateConfig->TimingTsusto << N_PCH_MCSMB_PCR_SPMT_TSUSTO;
  Data32 |= PrivateConfig->TimingTsusta << N_PCH_MCSMB_PCR_SPMT_TSUSTA;
  Data32 |= PrivateConfig->TimingTbuf   << N_PCH_MCSMB_PCR_SPMT_TBUF;
  Data32 |= PrivateConfig->TimingThdsta << N_PCH_MCSMB_PCR_SPMT_THDSTA;
  PchPcrAndThenOr32 (
    McSmbPid,
    R_PCH_MCSMB_PCR_SPMT,
    (UINT32)~(B_PCH_MCSMB_PCR_SPMT_TSUSTO | B_PCH_MCSMB_PCR_SPMT_TBUF |
              B_PCH_MCSMB_PCR_SPMT_TSUSTA | B_PCH_MCSMB_PCR_SPMT_THDSTA),
    Data32
  );

}
