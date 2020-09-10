/** @file
    MemTurnaroundTimingSocHbm.c - contains the Memory turnaround timing functions for a
     specific processor with HBM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <SysHost.h>
#include <MemCommon.h>
#include <Library/HbmCoreLib.h>
#include <Library/MemTurnaroundTimingLib.h>
#include "Include/MemTurnaroundTimingLibInternal.h"
#include <Library/MemMcIpLib.h>

#define HBM_TCCD_L   4
#define HBM_TCCD_S   2

//                                                      HbmFreq   t_rwsg  t_rwsr  t_rrdr  t_wwdr  t_rwdr  t_wrdr  t_rrdd  t_wwdd  t_rwdd  t_wrdd
CONST TURNAROUND_CONSTANTS  mTurnaroundHbm[]     =    {{HBM2E_2p8, 0x19,  0x19,   0,      0,      0,      0,      0,      0,      0,      0},
                                                       {HBM2E_3p2, 0x1E,  0x1E,   0,      0,      0,      0,      0,      0,      0,      0},
                                                      };

UINT8
HbmGetTccd_L (
  IN  UINT8  Socket
  )
{
  return HBM_TCCD_L;
} // GetTccd_L

UINT8
HbmGetTccd_S (
  IN  UINT8  Socket
  )
{
  return HBM_TCCD_S;
} // GetTccd_L

UINT8
HbmGetT_wrsg (
  IN  UINT8  Socket
  )
{
  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    return 0x1A;  // HBM2e_2.8_GTPerSec
  } else {
    return 0x1B;  // HBM2e_3.2_GTPerSec
  }
}

UINT8
HbmGetT_wrsr (
  IN  UINT8  Socket
  )
{
  return 0x13;  // HBM2e_2.8_GTPerSec; HBM2e_3.2_GTPerSec
}

/**

  Find and return a pointer to the turnaround contants structures
  for the current HBM frequency.

  @param[in]  Socket                - Socket number
  @param[out] TurnaroundConstants   - On return, points to a pointer to the turnaround constants

  @retval EFI_SUCCESS if table is found
          !EFI_SUCCESS otherwise

**/
EFI_STATUS
GetRelaxTurnaroundConstantsHbm (
  IN  UINT8                           Socket,
  OUT CONST TURNAROUND_CONSTANTS      **TurnaroundConstants
  )
{
  EFI_STATUS                      Status = EFI_UNSUPPORTED;
  UINT8                           TableIndex;
  HBM_FREQUENCY                   HbmFrequency;
  UINT16                          TableLength;
  CONST TURNAROUND_CONSTANTS      *TurnaroundTable;

  TurnaroundTable = mTurnaroundHbm;
  TableLength = sizeof (mTurnaroundHbm) / sizeof (mTurnaroundHbm[0]);

  HbmFrequency = HbmGetFrequency (Socket);

  //
  // Find the table entry based on frequency
  //
  for (TableIndex = 0; TableIndex < TableLength; TableIndex++) {
    if (TurnaroundTable[TableIndex].Frequency == (UINT8) HbmFrequency) {
      *TurnaroundConstants = &(TurnaroundTable[TableIndex]);
      Status = EFI_SUCCESS;
      break;
    }
  }

  return Status;
}

/**
  Program early turnaround times in accordance with Turnaround doc for this SOC.
  Display at the end of training for final results.

  @param[in] Socket              - Socket number
  @param[in] TurnaroundTable     - Pointer to table containing constants provided by EV
  @param[in] TurnaroundSettings  - Pointer to table containing settings to be programmed

  @retval None
**/
EFI_STATUS
EFIAPI
RelaxTurnaroundTimesSocHbm (
  IN UINT8                           Socket,
  IN CONST TURNAROUND_CONSTANTS      *TurnaroundTable,
  IN UINT32                          TurnaroundSettings[]
  )
{
  EFI_STATUS                      Status = EFI_SUCCESS;
  UINT8                           tCCD_L;
  UINT8                           tCCD_S;
  UINT8                           tCCDR;

  tCCD_L = HbmGetTccd_L (Socket);
  tCCD_S = HbmGetTccd_S (Socket);

  //
  // Same bank group, different bank
  //
  TurnaroundSettings[t_RRSG] = tCCD_L;
  TurnaroundSettings[t_WWSG] = tCCD_L;
  TurnaroundSettings[t_RWSG] = TurnaroundTable->t_rwsg;
  TurnaroundSettings[t_WRSG] = HbmGetT_wrsg (Socket);

  //
  // Different bank group
  //
  TurnaroundSettings[t_RRSR] = tCCD_S;
  TurnaroundSettings[t_WWSR] = tCCD_S;
  TurnaroundSettings[t_RWSR] = TurnaroundTable->t_rwsr;
  TurnaroundSettings[t_WRSR] = HbmGetT_wrsr (Socket);

  //
  // Different rank
  //
  TurnaroundSettings[t_RRDR] = TurnaroundTable->t_rrdr;
  TurnaroundSettings[t_WWDR] = TurnaroundTable->t_wwdr;
  TurnaroundSettings[t_RWDR] = TurnaroundTable->t_rwdr;
  TurnaroundSettings[t_WRDR] = TurnaroundTable->t_wrdr;

  //
  // Different DIMM
  //
  TurnaroundSettings[t_RRDD] = TurnaroundTable->t_rrdd;
  TurnaroundSettings[t_WWDD] = TurnaroundTable->t_wwdd;
  TurnaroundSettings[t_RWDD] = TurnaroundTable->t_rwdd;
  TurnaroundSettings[t_WRDD] = TurnaroundTable->t_wrdd;

  // Different subrank
  tCCDR = 0; // 1507661238 depend on different vendor

  if (tCCDR > tCCD_S) { // Increase only if HBM tCCDR > tCCD_S
    TurnaroundSettings[t_RRDS] = 0x3;
  } else {
    TurnaroundSettings[t_RRDS] = TurnaroundSettings[t_RRSR];
  }

  TurnaroundSettings[t_WWDS] = TurnaroundSettings[t_WWSR];
  TurnaroundSettings[t_RWDS] = TurnaroundSettings[t_RWSR];
  TurnaroundSettings[t_WRDS] = TurnaroundSettings[t_WRSR];

  //
  // t_rrd_dlr is 0 for HBM2
  //
  TurnaroundSettings[t_RRDLR] = 0;

  return Status;
}

/**
  Early turnaround times programed before training according to relax values given by TA doc.
  display at the end of training for final results.

  @param[in] Socket  - Socket number

  @retval EFI_SUCCESS       Turnaround CSRs read/write successful
  @retval EFI_UNSUPPORTED   Turnaround CSRs read/write successful
**/
EFI_STATUS
EFIAPI
InitRelaxTurnaroundTimesHbm (
  IN UINT8  Socket
  )
{
  EFI_STATUS                      Status = EFI_SUCCESS;
  UINT8                           HbmCh;
  UINT8                           MaxChHbm;
  UINT32                          TurnaroundSettings[TurnaroundTypeMax];
  CONST TURNAROUND_CONSTANTS      *TurnaroundConstants = NULL;

  MaxChHbm = GetMaxChHbm ();

  if (!IsHbmFlowEnabled (HF_BUS_TURNAROUND)) {
    return Status;
  }

  InitTurnaroundTable (TurnaroundSettings, TurnaroundTypeMax);

  //
  // Get pointer to the turnaround constants table for this configuration
  //
  if (EFI_SUCCESS != GetRelaxTurnaroundConstantsHbm (Socket, &TurnaroundConstants)) {
    RcDebugPrint (SDBG_DEFAULT, "GetRelaxTurnaroundConstantsHbm - Unsupported frequency\n");
    return EFI_UNSUPPORTED;
  }

  RelaxTurnaroundTimesSocHbm (Socket, TurnaroundConstants, TurnaroundSettings);

  for (HbmCh = 0; HbmCh < MaxChHbm; HbmCh++) {
    if (!IsHbmChannelEnabled (Socket, HbmCh)) {
      continue;
    }

    Status |= GetSetTurnaroundSettings (MemTechHbm, Socket, HbmCh, GSM_FORCE_WRITE, TurnaroundSettings);

    DisplayTurnaroundTimes (MemTechHbm, Socket, HbmCh);
  }

  return Status;
}
