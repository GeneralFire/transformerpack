/** @file
  This file is SampleCode of the library for Intel PCH PEI Policy initialization.

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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

#include <Library/DebugLib.h>
#include <Ppi/UpiPolicyPpi.h>
#include <Library/FspCommonLib.h>
#include <Library/ConfigBlockLib.h>
#include <FspmUpd.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

/**
  This function performs UPI PEI Policy update in pre-memory.

  @param[in, out] UpiPolicyPpi    The UPI PreMem Policy PPI instance
  @param[in]      FspmUpd         The pointer of MemoryInitUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiUpiPolicyPreMem (
  IN OUT UPI_POLICY_PPI           *UpiPolicyPpi,
  IN     FSPM_UPD                 *FspmUpd
  )
{
  KTI_HOST_IN           *Upi;
  UINTN                 Index;

  Upi = &UpiPolicyPpi->Upi;

  for (Index = 0; Index < MAX_SOCKET; Index ++)  {
    Upi->BusRatio[Index]      = FspmUpd->FspmConfig.BusRatio[Index];
    Upi->KtiFpgaEnable[Index] = FspmUpd->FspmConfig.KtiFpgaEnable[Index];
  }

  Upi->D2KCreditConfig              = FspmUpd->FspmConfig.D2KCreditConfig;
  Upi->SnoopThrottleConfig          = FspmUpd->FspmConfig.SnoopThrottleConfig;
  Upi->LegacyVgaSoc                 = FspmUpd->FspmConfig.LegacyVgaSoc;
  Upi->LegacyVgaStack               = FspmUpd->FspmConfig.LegacyVgaStack;
  Upi->P2pRelaxedOrdering           = FspmUpd->FspmConfig.P2pRelaxedOrdering;
  Upi->DebugPrintLevel              = FspmUpd->FspmConfig.DebugPrintLevel;
  Upi->SncEn                        = FspmUpd->FspmConfig.SncEn;
  Upi->UmaClustering                = FspmUpd->FspmConfig.UmaClustering;
  Upi->IoDcMode                     = FspmUpd->FspmConfig.IoDcMode;
  Upi->DegradePrecedence            = FspmUpd->FspmConfig.DegradePrecedence;
  Upi->Degrade4SPreference          = FspmUpd->FspmConfig.Degrade4SPreference;
  Upi->DirectoryModeEn              = FspmUpd->FspmConfig.DirectoryModeEn;
  Upi->XptPrefetchEn                = FspmUpd->FspmConfig.XptPrefetchEn;
  Upi->KtiPrefetchEn                = FspmUpd->FspmConfig.KtiPrefetchEn;
  Upi->XptRemotePrefetchEn          = FspmUpd->FspmConfig.XptRemotePrefetchEn;
  Upi->DdrtQosMode                  = FspmUpd->FspmConfig.DdrtQosMode;
  Upi->KtiLinkSpeedMode             = FspmUpd->FspmConfig.KtiLinkSpeedMode;
  Upi->KtiLinkSpeed                 = FspmUpd->FspmConfig.KtiLinkSpeed;
  Upi->KtiLinkL0pEn                 = FspmUpd->FspmConfig.KtiLinkL0pEn;
  Upi->KtiLinkL1En                  = FspmUpd->FspmConfig.KtiLinkL1En;
  Upi->KtiFailoverEn                = FspmUpd->FspmConfig.KtiFailoverEn;
  Upi->KtiLbEn                      = FspmUpd->FspmConfig.KtiLbEn;
  Upi->KtiCrcMode                   = FspmUpd->FspmConfig.KtiCrcMode;
  Upi->KtiCpuSktHotPlugEn           = FspmUpd->FspmConfig.KtiCpuSktHotPlugEn;
  Upi->KtiCpuSktHotPlugTopology     = FspmUpd->FspmConfig.KtiCpuSktHotPlugTopology;
  Upi->KtiSkuMismatchCheck          = FspmUpd->FspmConfig.KtiSkuMismatchCheck;
  Upi->IrqThreshold                 = FspmUpd->FspmConfig.IrqThreshold;
  Upi->TorThresLoctoremNorm         = FspmUpd->FspmConfig.TorThresLoctoremNorm;
  Upi->TorThresLoctoremEmpty        = FspmUpd->FspmConfig.TorThresLoctoremEmpty;
  Upi->TscSyncEn                    = FspmUpd->FspmConfig.TscSyncEn;
  Upi->StaleAtoSOptEn               = FspmUpd->FspmConfig.StaleAtoSOptEn;
  Upi->LLCDeadLineAlloc             = FspmUpd->FspmConfig.LLCDeadLineAlloc;
  Upi->SplitLock                    = FspmUpd->FspmConfig.SplitLock;
  Upi->mmCfgBase                    = FspmUpd->FspmConfig.mmCfgBase;
  Upi->mmCfgSize                    = FspmUpd->FspmConfig.mmCfgSize;
  Upi->mmiolBase                    = 0; // KTI calculates based on mmCfgBase and mmCfgSize
  Upi->mmiolSize                    = 0; // KTI calculates
  Upi->mmiohBase                    = FspmUpd->FspmConfig.mmiohBase;
  Upi->highGap                      = FspmUpd->FspmConfig.highGap;
  Upi->mmiohSize                    = FspmUpd->FspmConfig.mmiohSize;
  Upi->isocEn                       = FspmUpd->FspmConfig.isocEn;
  Upi->dcaEn                        = FspmUpd->FspmConfig.dcaEn;
  Upi->BoardTypeBitmask             = FspmUpd->FspmConfig.BoardTypeBitmask;
  Upi->AllLanesPtr                  = FspmUpd->FspmConfig.AllLanesPtr;
  Upi->PerLanePtr                   = FspmUpd->FspmConfig.PerLanePtr;
  Upi->AllLanesSizeOfTable          = FspmUpd->FspmConfig.AllLanesSizeOfTable;
  Upi->PerLaneSizeOfTable           = FspmUpd->FspmConfig.PerLaneSizeOfTable;
  Upi->WaitTimeForPSBP              = FspmUpd->FspmConfig.WaitTimeForPSBP;
  Upi->IsKtiNvramDataReady          = FspmUpd->FspmConfig.IsKtiNvramDataReady;
  Upi->OemHookPostTopologyDiscovery = 0; // FSP should not expose hooks
  Upi->OemGetResourceMapUpdate      = 0; // FSP should not expose hooks
  Upi->WaSerializationEn            = FspmUpd->FspmConfig.WaSerializationEn;
  Upi->KtiInEnableMktme             = FspmUpd->FspmConfig.KtiInEnableMktme;

  return EFI_SUCCESS;
}
