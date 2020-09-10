/** @file
  Sample to provide FSP wrapper related function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation. <BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <FspmUpd.h>
#include <Ppi/UpiPolicyPpi.h>

VOID
EFIAPI
UpdateFspmUpdData (
  IN OUT VOID         *FspUpdRgnPtr
  )
{
  FSPM_UPD                     *FspmUpd;
  EFI_STATUS                   Status;
  UPI_POLICY_PPI               *UpiPolicyPpi;
  KTI_HOST_IN                  *Upi;
  UINTN                        Index;
  VOID                         *FSPTempMem;

  Status = PeiServicesLocatePpi (&gUpiSiPolicyPpiGuid, 0, NULL, &UpiPolicyPpi);
  ASSERT_EFI_ERROR(Status);

  Upi = &UpiPolicyPpi->Upi;
  FspmUpd = (FSPM_UPD*)FspUpdRgnPtr;

  FSPTempMem = (VOID *)(UINTN)(PcdGet32(PcdTemporaryRamBase) + PcdGet32(PcdPeiTemporaryRamRcHeapSize));;
  if (FSPTempMem != NULL) {
    FspmUpd->FspmArchUpd.StackBase = (VOID*)(((UINTN)FSPTempMem) & (~0xFFF));
    FspmUpd->FspmArchUpd.StackSize = PcdGet32(PcdFspTemporaryRamSize);
  }

  for (Index = 0; Index < MAX_SOCKET; Index ++)  {
    FspmUpd->FspmConfig.BusRatio[Index] = Upi->BusRatio[Index];
    FspmUpd->FspmConfig.KtiFpgaEnable[Index] = Upi->KtiFpgaEnable[Index];
  }

  FspmUpd->FspmConfig.D2KCreditConfig = Upi->D2KCreditConfig;
  FspmUpd->FspmConfig.SnoopThrottleConfig = Upi->SnoopThrottleConfig;
  FspmUpd->FspmConfig.LegacyVgaSoc = Upi->LegacyVgaSoc;
  FspmUpd->FspmConfig.LegacyVgaStack = Upi->LegacyVgaStack;
  FspmUpd->FspmConfig.P2pRelaxedOrdering = Upi->P2pRelaxedOrdering;
  FspmUpd->FspmConfig.DebugPrintLevel = Upi->DebugPrintLevel;
  FspmUpd->FspmConfig.SncEn = Upi->SncEn;
  FspmUpd->FspmConfig.UmaClustering = Upi->UmaClustering;
  FspmUpd->FspmConfig.IoDcMode = Upi->IoDcMode;
  FspmUpd->FspmConfig.DegradePrecedence = Upi->DegradePrecedence;
  FspmUpd->FspmConfig.Degrade4SPreference = Upi->Degrade4SPreference;
  FspmUpd->FspmConfig.DirectoryModeEn = Upi->DirectoryModeEn;
  FspmUpd->FspmConfig.XptPrefetchEn = Upi->XptPrefetchEn;
  FspmUpd->FspmConfig.KtiPrefetchEn = Upi->KtiPrefetchEn;
  FspmUpd->FspmConfig.XptRemotePrefetchEn = Upi->XptRemotePrefetchEn;
  FspmUpd->FspmConfig.DdrtQosMode = Upi->DdrtQosMode;
  FspmUpd->FspmConfig.KtiLinkSpeedMode = Upi->KtiLinkSpeedMode;
  FspmUpd->FspmConfig.KtiLinkSpeed = Upi->KtiLinkSpeed;
  FspmUpd->FspmConfig.KtiLinkL0pEn = Upi->KtiLinkL0pEn;
  FspmUpd->FspmConfig.KtiLinkL1En = Upi->KtiLinkL1En;
  FspmUpd->FspmConfig.KtiFailoverEn = Upi->KtiFailoverEn;
  FspmUpd->FspmConfig.KtiLbEn = Upi->KtiLbEn;
  FspmUpd->FspmConfig.KtiCrcMode = Upi->KtiCrcMode;
  FspmUpd->FspmConfig.KtiCpuSktHotPlugEn = Upi->KtiCpuSktHotPlugEn;
  FspmUpd->FspmConfig.KtiCpuSktHotPlugTopology = Upi->KtiCpuSktHotPlugTopology;
  FspmUpd->FspmConfig.KtiSkuMismatchCheck = Upi->KtiSkuMismatchCheck;
  FspmUpd->FspmConfig.IrqThreshold = Upi->IrqThreshold;
  FspmUpd->FspmConfig.TorThresLoctoremNorm = Upi->TorThresLoctoremNorm;
  FspmUpd->FspmConfig.TorThresLoctoremEmpty = Upi->TorThresLoctoremEmpty;
  FspmUpd->FspmConfig.TscSyncEn = Upi->TscSyncEn;
  FspmUpd->FspmConfig.StaleAtoSOptEn = Upi->StaleAtoSOptEn;
  FspmUpd->FspmConfig.LLCDeadLineAlloc = Upi->LLCDeadLineAlloc;
  FspmUpd->FspmConfig.SplitLock = Upi->SplitLock;
  FspmUpd->FspmConfig.mmCfgBase = Upi->mmCfgBase;
  FspmUpd->FspmConfig.mmCfgSize = Upi->mmCfgSize;
  FspmUpd->FspmConfig.mmiohBase = Upi->mmiohBase;
  FspmUpd->FspmConfig.highGap = Upi->highGap;
  FspmUpd->FspmConfig.mmiohSize = Upi->mmiohSize;
  FspmUpd->FspmConfig.isocEn = Upi->isocEn;
  FspmUpd->FspmConfig.dcaEn = Upi->dcaEn;
  FspmUpd->FspmConfig.BoardTypeBitmask = Upi->BoardTypeBitmask;
  FspmUpd->FspmConfig.AllLanesPtr = Upi->AllLanesPtr;
  FspmUpd->FspmConfig.PerLanePtr = Upi->PerLanePtr;
  FspmUpd->FspmConfig.AllLanesSizeOfTable = Upi->AllLanesSizeOfTable;
  FspmUpd->FspmConfig.PerLaneSizeOfTable = Upi->PerLaneSizeOfTable;
  FspmUpd->FspmConfig.WaitTimeForPSBP = Upi->WaitTimeForPSBP;
  FspmUpd->FspmConfig.IsKtiNvramDataReady = Upi->IsKtiNvramDataReady;
  FspmUpd->FspmConfig.WaSerializationEn = Upi->WaSerializationEn;
  FspmUpd->FspmConfig.KtiInEnableMktme = Upi->KtiInEnableMktme;
}

/**
  This function overrides the default configurations in the FSP-S UPD data region.

  @param[in,out] FspUpdRgnPtr   A pointer to the UPD data region data strcture.

**/
VOID
EFIAPI
UpdateFspsUpdData (
  IN OUT VOID        *FspUpdRgnPtr
  )
{
}

/**
  Update TempRamExit parameter.

  @note At this point, memory is ready, PeiServices are available to use.

  @return TempRamExit parameter.
**/
VOID *
EFIAPI
UpdateTempRamExitParam (
  VOID
  )
{
  return NULL;
}

/**
  Get S3 PEI memory information.

  @note At this point, memory is ready, and PeiServices are available to use.
  Platform can get some data from SMRAM directly.

  @param[out] S3PeiMemSize  PEI memory size to be installed in S3 phase.
  @param[out] S3PeiMemBase  PEI memory base to be installed in S3 phase.

  @return If S3 PEI memory information is got successfully.
**/
EFI_STATUS
EFIAPI
GetS3MemoryInfo (
  OUT UINT64               *S3PeiMemSize,
  OUT EFI_PHYSICAL_ADDRESS *S3PeiMemBase
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Perform platform related reset in FSP wrapper.

  This function will reset the system with requested ResetType.

  @param[in] FspStatusResetType  The type of reset the platform has to perform.
**/
VOID
EFIAPI
CallFspWrapperResetSystem (
  IN UINT32    FspStatusResetType
  )
{
  //
  // Perform reset according to the type.
  //

  CpuDeadLoop();
}
