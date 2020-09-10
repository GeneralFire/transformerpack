/** @file
  Code for Selecting Least-Feature Processor as BSP

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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

#include "SiCpuInitDxe.h"

//
// Common set of processors' feature flags.
//
UINT32    CommonFeatureFlagSet[6];
//
// Flags of whether corresponding processor matches common set.
//
BOOLEAN   *mMatchingCommonSet;

/**
  Get the common set of processors' feature flags.

  This function calculates the common set of processors' feature flags.
  Processor's feature flags include 6 segments: CPUID(1).ECX, CPUID(1).EDX,
  CPUID(0x80000001).EAX, CPUID(0x80000001).EBX, CPUID(0x80000001).ECX,
  and CPUID(0x80000001).EDX. "AND" them together and result is the common set.

**/
VOID
GetCommonFeatureFlagSet (
  VOID
  )
{
  EFI_CPUID_REGISTER  *CpuidRegisters;
  UINTN               ProcessorNumber;

  //
  // Initialize common set of feature flag with all "1"s.
  //
  SetMem32 (CommonFeatureFlagSet, sizeof (CommonFeatureFlagSet), 0xffffffff);

  //
  // Traverse all processors' feature flags
  //
  for (ProcessorNumber = 0; ProcessorNumber < mCpuConfigLibConfigContextBuffer->NumberOfProcessors; ProcessorNumber++) {

    //
    // AND CPUID(1).ECX and CPUID(1).EDX with common set
    //
    CpuidRegisters = GetProcessorCpuid (ProcessorNumber, CPUID_VERSION_INFO);
    ASSERT (CpuidRegisters != NULL);
    if (CpuidRegisters == NULL) {
      return;
    }
    CommonFeatureFlagSet[0] &= CpuidRegisters->RegEcx;
    CommonFeatureFlagSet[1] &= CpuidRegisters->RegEdx;

    //
    // AND CPUID(0x80000001).EAX, CPUID(0x80000001).EBX, CPUID(0x80000001).ECX,
    // and CPUID(0x80000001).EDX with common set.
    //
    CpuidRegisters = GetProcessorCpuid (ProcessorNumber, CPUID_EXTENDED_CPU_SIG);
    ASSERT (CpuidRegisters != NULL);
    if (CpuidRegisters == NULL) {
      return;
    }
    CommonFeatureFlagSet[2] &= CpuidRegisters->RegEax;
    CommonFeatureFlagSet[3] &= CpuidRegisters->RegEbx;
    CommonFeatureFlagSet[4] &= CpuidRegisters->RegEcx;
    CommonFeatureFlagSet[5] &= CpuidRegisters->RegEdx;
  }
}

/**
  Checks which processors match the common set of feature flag

  This function Checks which processors match the common set of feature flag.

  @retval TRUE    At least one processor matches the common set of feature flag.
  @retval FALSE   No processor matches the common set of feature flag.

**/
BOOLEAN
CompareProcessorsWithCommonSet (
  VOID
  )
{
  EFI_CPUID_REGISTER  *CpuidRegisters;
  UINTN               ProcessorNumber;
  BOOLEAN             ReturnValue;

  mMatchingCommonSet = (BOOLEAN *)AllocatePool (
                                    sizeof (BOOLEAN) * mCpuConfigLibConfigContextBuffer->NumberOfProcessors
                                    );
  ASSERT (mMatchingCommonSet != NULL);
  if (mMatchingCommonSet == NULL) {
    return FALSE;
  }
  ReturnValue = FALSE;
  for (ProcessorNumber = 0; ProcessorNumber < mCpuConfigLibConfigContextBuffer->NumberOfProcessors; ProcessorNumber++) {
    mMatchingCommonSet[ProcessorNumber] = FALSE;

    //
    // Compare CPUID(1).ECX and CPUID(1).EDX with common set
    //
    CpuidRegisters = GetProcessorCpuid (0, CPUID_VERSION_INFO);
    ASSERT (CpuidRegisters != NULL);
    if (CpuidRegisters == NULL) {
      return FALSE;
    }
    if (CommonFeatureFlagSet[0] != CpuidRegisters->RegEcx || CommonFeatureFlagSet[1] != CpuidRegisters->RegEdx) {
      continue;
    }

    //
    // Compare CPUID(0x80000001).EAX, CPUID(0x80000001).EBX, CPUID(0x80000001).ECX,
    // and CPUID(0x80000001).EDX with common set.
    //
    CpuidRegisters = GetProcessorCpuid (ProcessorNumber, CPUID_EXTENDED_CPU_SIG);
    ASSERT (CpuidRegisters != NULL);
    if (CpuidRegisters == NULL) {
      return FALSE;
    }
    if (CommonFeatureFlagSet[2] != CpuidRegisters->RegEax || CommonFeatureFlagSet[3] != CpuidRegisters->RegEbx ||
        CommonFeatureFlagSet[4] != CpuidRegisters->RegEcx || CommonFeatureFlagSet[5] != CpuidRegisters->RegEdx) {
      continue;
    }

    //
    // If the processor matches common set, flag it, and set return value as TRUE.
    //
    mMatchingCommonSet[ProcessorNumber] = TRUE;
    ReturnValue = TRUE;
  }

  return ReturnValue;
}

/**

   Get New SBSP processor number

    @param SocketNumber - CPU SocketID

    @retval ProcessorNumber - Processor Number for new SBSP

**/
UINTN
GetNewSbspProcessNumber (
  UINT32 SocketNumber
  )
{
  UINTN       i;
  UINTN       ProcessorNumber = (UINTN)-1;
  UINT32      CoreNumber = (UINT32)-1;
  UINT32      ThreadNumber = (UINT32)-1;

  for (i = 0; i < mCpuConfigLibConfigContextBuffer->NumberOfProcessors; i++) {

    if(mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[i].ProcessorLocation.Package == SocketNumber) {
      if(mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[i].ProcessorLocation.Core >= CoreNumber) {
        continue;
      } else {
        CoreNumber = mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[i].ProcessorLocation.Core;
        if(mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[i].ProcessorLocation.Thread > ThreadNumber) {
          continue;
        } else {
          ThreadNumber = mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[i].ProcessorLocation.Thread;
          ProcessorNumber = i;
        }
      } //end of if CoreNumber
    } //end if SocketNumber
  }

  if(ProcessorNumber >= mCpuConfigLibConfigContextBuffer->NumberOfProcessors) {  //don't find one
    ProcessorNumber = mCpuConfigLibConfigContextBuffer->BspNumber;     //deafult to current Sbsp
  }
  return ProcessorNumber;
}

/**
  Select least-feature processor as BSP

  This function selects least-feature processor as BSP.

**/
VOID
SelectLfpAsBsp (
  VOID
  )
{
  UINTN               ProcessorNumber;
  UINTN               OldestProcessorNumber;
  UINT32              OldestProcessorFms[3];
  UINT32              FamilyId;
  UINT32              ModelId;
  UINT32              SteppingId;
  BOOLEAN             MatchingProcessorExist;
//ESS Override start
  UINT32              SbspSkt = 0xFF;
//ESS Override end

  //
  // Calculate the common set of processors' feature flags
  //
  GetCommonFeatureFlagSet ();

  //
  // Compare all logical processors with common set.
  //
  MatchingProcessorExist = CompareProcessorsWithCommonSet ();

//OVERRIDE_HSD_5370421_BEGIN
  OldestProcessorNumber = mCpuConfigLibConfigContextBuffer->BspNumber;
  GetProcessorVersionInfo (OldestProcessorNumber, &FamilyId, &ModelId, &SteppingId, NULL);
  if (!MatchingProcessorExist || mMatchingCommonSet[OldestProcessorNumber]) {
    //
    // If no processor matches common set or current BSP matches the common set,
    // set current BSP firstly. So if more than 1 processor match common set and with the
    // same CPUID, the current one is BSP.
    //
    OldestProcessorFms[2] = FamilyId;
    OldestProcessorFms[1] = ModelId;
    OldestProcessorFms[0] = SteppingId;
  } else {
    OldestProcessorFms[2] = 0xFFFFFFFF;
    OldestProcessorFms[1] = 0;
    OldestProcessorFms[0] = 0;
  }
//OVERRIDE_HSD_5370421_END
  for (ProcessorNumber = 0; ProcessorNumber < mCpuConfigLibConfigContextBuffer->NumberOfProcessors; ProcessorNumber++) {

    //
    // If only 1 processor matches common set, it should be BSP
    // If more than 1 processors match common set, the one with lowest CPUID should be BSP.
    // If no processor matches common set, the processor with lowest CPUID should be BSP.
    //
    if (!MatchingProcessorExist || mMatchingCommonSet[ProcessorNumber]) {
      GetProcessorVersionInfo (ProcessorNumber, &FamilyId, &ModelId, &SteppingId, NULL);

      if (FamilyId > OldestProcessorFms[2]) {
        continue;
      }

      if (FamilyId == OldestProcessorFms[2]) {
        if (ModelId > OldestProcessorFms[1]) {
          continue;
        }
        if (ModelId == OldestProcessorFms[1]) {
          if (SteppingId >= OldestProcessorFms[0]) {
            continue;
          }
        }
      }

      OldestProcessorFms[2] = FamilyId;
      OldestProcessorFms[1] = ModelId;
      OldestProcessorFms[0] = SteppingId;
      OldestProcessorNumber = ProcessorNumber;
    }
  }

  FreePool (mMatchingCommonSet);
  //ESS Override start
  //check if user want to select SBSP
  //if yes, override
  SbspSkt = mCpuPolicyConfiguration->SbspSelection;
  if(SbspSkt != 0xFF) {          //if select SBSP
    OldestProcessorNumber = GetNewSbspProcessNumber(SbspSkt);
  }
  //ESS Override end

  //
  // If current BSP is not the least-feature processor, then switch BSP.
  //
  if (OldestProcessorNumber != mCpuConfigLibConfigContextBuffer->BspNumber) {
    mMpServices->SwitchBSP (mMpServices, OldestProcessorNumber, TRUE);
    DEBUG ((EFI_D_ERROR, "BSP switched to processor: %8d\r\n", OldestProcessorNumber));
  }
}
