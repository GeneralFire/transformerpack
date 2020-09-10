/** @file
  This implements filling out the HECI Message responsible of passing
  CPU Info data.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2010 - 2020 Intel Corporation. <BR>

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
#include <Uefi.h>
#include <PiDxe.h>
#include <Register/Cpuid.h>
#include <Register/ArchitecturalMsr.h>
#include <Register/ArchMsr.h>
#include <CpuRegs.h>
#include <RcRegs.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/CpuConfigLib.h>
#include <Protocol/CpuCsrAccess.h>
#include <Library/SpsDxeLib.h>
#include <Library/CpuPreMemPolicyLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Cpu/CpuIds.h>
#include <Protocol/CpuPolicyProtocol.h>
#include <Protocol/PpmPolicyProtocol.h>
#include <Guid/SocketPowermanagementVariable.h>
#include <Guid/SocketProcessorCoreVariable.h>
#include <Library/SpsDxeLib.h>
#include <SpsDxeDependencyCheck.h>

/*****************************************************************************
 * Local definitions
 *****************************************************************************/
#define CPU_TSTATES_NUM_STD    8
#define CPU_TSTATES_NUM_FG    15  // fine grained T-states

SPS_DEPENDENCY_CHECK CheckTable[] = {
  {&gCpuConfigContextReadyProtocolGuid,"gCpuConfigContextReadyProtocolGuid", EFI_NOT_FOUND},
  {&gEfiCpuPolicyProtocolGuid,         "gEfiCpuPolicyProtocolGuid",          EFI_NOT_FOUND},
  {&gPpmPolicyProtocolGuid,            "gPpmPolicyProtocolGuid",             EFI_NOT_FOUND},
  {NULL,                               NULL,                                 EFI_NOT_FOUND}
};

/*****************************************************************************

/**
  Get Turbo information

  @retval TRUE    Turbo bit is enabled
  @retval FALSE   Turbo bit is disabled
**/
BOOLEAN
SpsGetTurboBit (
  VOID
  )
{
  CPU_POLICY_CONFIGURATION      *CpuPolicy;
  EFI_STATUS                    Status;

  Status = gBS->LocateProtocol (&gEfiCpuPolicyProtocolGuid, NULL, &CpuPolicy);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot locate CPU Policy protocol (%r)\n", Status));
    return FALSE;
  }

  return CpuPolicy->Policy.CpuTurboModeEnable;
}

/**
  Get TStates information
  When TStates information is not available input data is returned

  @param[in] TStatesNumber    TStates number to return in error case

  @retval TStatesNumber       Number of TStatesNumber configured in the system
**/
UINT8
SpsGetTStatesNumber (
  IN UINT8 TStatesNumber
  )
{
  CPU_COLLECTED_DATA             *CpuData;
  CPU_POLICY_CONFIGURATION       *CpuPolicy;
  EFI_STATUS                     Status;

  Status = gBS->LocateProtocol (&gEfiCpuPolicyProtocolGuid, NULL, &CpuPolicy);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot locate CPU Policy protocol (%r)\n", Status));
    return TStatesNumber;
  }

  CpuData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[0];
  ASSERT (CpuData != NULL);
  if (CpuData == NULL) {
    return TStatesNumber;
  }

  for (CpuData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[0];
       CpuData < &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[mCpuConfigLibConfigContextBuffer->NumberOfProcessors];
       CpuData++) {
    ASSERT (CpuData->ProcessorLocation.Package < MAX_SOCKET);
    if (CpuData->PackageBsp) {
      if (CpuPolicy->Policy.CpuTStateEnable) {
        TStatesNumber = CPU_TSTATES_NUM_STD;
        if (CpuData->CpuidData.CpuIdLeaf != NULL &&
            CpuData->CpuidData.NumberOfBasicCpuidLeafs > CPUID_THERMAL_POWER_MANAGEMENT &&
            (((CPUID_THERMAL_POWER_MANAGEMENT_EAX*) &CpuData->CpuidData.CpuIdLeaf[CPUID_THERMAL_POWER_MANAGEMENT].RegEax)->Bits.ECMD == 1)) {
          TStatesNumber = CPU_TSTATES_NUM_FG;
        }
      }
    }
  }

  return TStatesNumber;
}

/**
  Fill PStates information in the message table

  @param[out] PStatesNumber      Pointer to PStatesNumber
  @param[out] NPStatesTable      Pointer to NPStatesTable table
  @param[out] GPSstatesNumber    Pointer to GPSstatesNumber
  @param[out] GPStatesTable      Pointer to GPStatesTable table
  @param[in] Turbo               Turbo information

  @retval EFI_SUCCESS            PStates Tables are prepared
  @retval EFI_INVALID_PARAMETER  Passed pointers are invalid
  @retval EFI_NOT_FOUND          Information has been not found in the system
**/
EFI_STATUS
SpsFillStatesTable (
     OUT UINT8   *PStatesNumber,
     OUT UINT8   *NPStatesTable,
     OUT UINT8   *GPSstatesNumber,
     OUT UINT8   *GPStatesTable,
  IN     UINT8   Turbo
  )
{
  CPU_COLLECTED_DATA                   *CpuData;
  UINT32                               Index;

  if (PStatesNumber == NULL ||
      NPStatesTable == NULL ||
      GPSstatesNumber == NULL ||
      GPStatesTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  CpuData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[0];
  ASSERT (CpuData != NULL);
  if (CpuData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (CpuData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[0];
       CpuData < &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[mCpuConfigLibConfigContextBuffer->NumberOfProcessors];
       CpuData++) {
    ASSERT (CpuData->ProcessorLocation.Package < MAX_SOCKET);
    if (CpuData->PackageBsp) {
      *PStatesNumber = (UINT8) CpuData->CpuMiscData.NumberOfPStates;
      if (CpuData->CpuMiscData.FvidTable != NULL) {
        for (Index = 0; Index < CpuData->CpuMiscData.NumberOfPStates; Index++) {
          NPStatesTable[Index] = (UINT8) CpuData->CpuMiscData.FvidTable[Index].Ratio;
        }
      }

      *GPSstatesNumber = (UINT8) CpuData->CpuMiscData.GreaterNumberOfPStates;
      if (CpuData->CpuMiscData.GreaterFvidTable != NULL) {
        for (Index = 0; Index < CpuData->CpuMiscData.GreaterNumberOfPStates; Index++) {
          GPStatesTable[Index] = (UINT8) CpuData->CpuMiscData.GreaterFvidTable[Index].Ratio;
        }
      }

    }
  } // for (pProcData...)

  return EFI_SUCCESS;
}

/**
  Get Cores and threads information

  @param[out] ProcCoresEnabled   Pointer to return cores information
  @param[out] ProcThreadsEnabled Pointer to return threads information

  @retval EFI_SUCCESS            Cores and threads information has been prepared
  @retval EFI_INVALID_PARAMETER  Passed pointers are invalid
  @retval EFI_NOT_FOUND          Information has been not found in the system
**/
EFI_STATUS
SpsGetCoresAndTreads (
  OUT UINT16    *ProcCoresEnabled,
  OUT UINT16    *ProcThreadsEnabled
  )
{
  UINT32                               tCoreMask[MAX_SOCKET];
  CPU_COLLECTED_DATA                   *CpuData;
  CPU_PRE_MEM_POLICY                   *CpuPolicy;

  if (ProcCoresEnabled == NULL ||
      ProcThreadsEnabled == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  CpuData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[0];
  ASSERT (CpuData != NULL);
  if (CpuData == NULL) {
    return EFI_NOT_FOUND;
  }

  CpuPolicy = GetCpuPreMemPolicy ();
  if (CpuPolicy == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot locate CPU Pre Mem Policy\n"));
    return EFI_NOT_FOUND;
  }

  SetMem (&tCoreMask[0], sizeof (tCoreMask), 0);
  for (CpuData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[0];
       CpuData < &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[mCpuConfigLibConfigContextBuffer->NumberOfProcessors];
       CpuData++) {
    ASSERT (CpuData->ProcessorLocation.Package < sizeof (tCoreMask) / sizeof (tCoreMask[0]));
    if (!(tCoreMask[CpuData->ProcessorLocation.Package] & (1 << CpuData->ProcessorLocation.Core))) {
      tCoreMask[CpuData->ProcessorLocation.Package] |= 1 << CpuData->ProcessorLocation.Core;
      *ProcCoresEnabled++;
      *ProcThreadsEnabled++;
    } else {
      //
      // For some reason hyper-threads are listed in this library regardless whether
      // HT is enabled or not. Let's filter them.
      //
      if (CpuPolicy->smtEnable != 0) {
        *ProcThreadsEnabled++;
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Check if Hardware Power Management may be configured
  in OOB or Native w/o Legacy

  @param[out] ProcNumber   Pointer to processorNumber information

  @retval TRUE             HWP is configured in OOB or Native w/o Legacy
  @retval FALSE            HWP is not configured in OOB or Native w/o Legacy
**/
BOOLEAN
SpsIsLegacyOob (
  OUT UINT16    *ProcNumber
  )
{
  BOOLEAN                      HwpCapable = TRUE;
  CPU_COLLECTED_DATA           *CpuData;
  EFI_CPU_CSR_ACCESS_PROTOCOL  *CpuCsrAccess;
  CAPID5_PCU_FUN3_STRUCT       CsrCapid5;
  PPM_POLICY_CONFIGURATION     *PpmPolicy;
  UINT32                       ProcMask;
  EFI_STATUS                   Status;

  if (ProcNumber != NULL) {
    *ProcNumber = 0;
  }
  CpuData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[0];
  ASSERT (CpuData != NULL);
  if (CpuData == NULL) {
    return FALSE;
  }

  Status = gBS->LocateProtocol (&gPpmPolicyProtocolGuid, NULL, &PpmPolicy);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot locate PPM Policy protocol (%r)\n", Status));
    return FALSE;
  }

  Status = gBS->LocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &CpuCsrAccess);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot locate processor CSR access protocol (%r)\n", Status));
    CpuCsrAccess = NULL;
  }

  ProcMask = 0;
  for (CpuData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[0];
       CpuData < &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[mCpuConfigLibConfigContextBuffer->NumberOfProcessors];
       CpuData++) {
    ASSERT (CpuData->ProcessorLocation.Package < MAX_SOCKET);
    if (!(ProcMask & (1 << CpuData->ProcessorLocation.Package))) {
      ProcMask |= 1 << CpuData->ProcessorLocation.Package;
      if (ProcNumber != NULL) {
        (*ProcNumber)++;
      }
      if (CpuCsrAccess != NULL) {
        CsrCapid5.Data = CpuCsrAccess->ReadCpuCsr (
          (UINT8)CpuData->ProcessorLocation.Package,
          0,
          CAPID5_PCU_FUN3_REG
          );
        HwpCapable &= CsrCapid5.Bits.hwp_en;
      }
    }
  }

  if ((HwpCapable) &&
     (PpmPolicy != NULL) && (PpmPolicy->Hwpm.HWPMEnable > HWP_MODE_NATIVE)) { // if HWP OOB or Native w/o Legacy
    return TRUE;
  }

  return FALSE;
}