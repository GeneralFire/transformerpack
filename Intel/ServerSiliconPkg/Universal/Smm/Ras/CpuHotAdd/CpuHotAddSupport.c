/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2020 Intel Corporation. <BR>

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

#include "CpuHotAdd.h"
#include <Library/IoLib.h>
#include <Library/LocalApicLib.h>
#include <Register/PchRegsP2sb.h>
#include <Register/LocalApic.h>
#include <Library/CpuIpLib.h>

extern ACPI_CPU_DATA                              *mAcpiCpuData;
extern volatile HOT_ADD_CPU_EXCHANGE_INFO         *mExchangeInfo;
extern EFI_CPU_PM_STRUCT                          mLocalPpmStruct;
extern PPM_POLICY_CONFIGURATION                   mLocalPpmPolicy;

VOID
InitialProcessorInit (
  )
{
  MTRR_SETTINGS             *MtrrSettings;

  LoadMicrocodeEx();

  ProgramVirtualWireMode ();

  MtrrSettings = (MTRR_SETTINGS *) (UINTN)mAcpiCpuData->MtrrTable;
  MtrrSetAllMtrrs (MtrrSettings);

   CpuMiscMsrProgramming();

  //
  // TO-DO  -- Need to find out how to enable other Processor Features like
  // X2APIC, PPM, Reset CPL, TXT etc...
  //
}

VOID CpuMiscMsrProgramming (
  )
{
  UINT8   Index;
  UINT32  MsrOffset;
  UINT64  MsrValue;

  for (Index = 0; Index < SBSP_MSR_NUM; Index++) {
    MsrOffset = mExchangeInfo->SbspMsrInfo[Index].MsrOffset;
    MsrValue = mExchangeInfo->SbspMsrInfo[Index].MsrValue;

    AsmWriteMsr64 (MsrOffset, MsrValue);
  }
}



VOID PpmCsrProgramming (
  UINT8   SktId
  )
{

  InitializeRaplPowerLimitCsr (SktId, &mLocalPpmStruct, &mLocalPpmPolicy.TurboPowerLimit, mLocalPpmPolicy.C2C3TT);
  InitializePlatformPowerLimitCsr (SktId, &mLocalPpmStruct, &mLocalPpmPolicy.PsysConfig);
  InitializePkgCstEntryCriteriaMask (SktId, &mLocalPpmPolicy);
  InitializePerfPlimitControl (SktId, &mLocalPpmPolicy.PerPLimitCtl);
  InitializeDynamicPerfPowerCtl (SktId, &mLocalPpmPolicy.DynamicPerPowerCtl);
  InitializeSapmControl (SktId, &mLocalPpmPolicy.SapmCtl[SktId]);
  InitializeSwLtrOverride (SktId, &mLocalPpmPolicy);
  InitializePmaxConfiguration (SktId, &mLocalPpmStruct, &mLocalPpmPolicy.PmaxConfig);
  InitializeThermalReport (SktId, &mLocalPpmPolicy.ThermalReport, mLocalPpmPolicy.CpuThermalManagement);
  InitializeProchotResponseRatio (SktId, mLocalPpmPolicy.ProchotRatio);
  InitializeFastRaplDutyCycle (SktId, mLocalPpmPolicy.FastRaplDutyCycle);
  InitializeHWPMMiscConfig(SktId, &mLocalPpmPolicy.PowerCtl, &mLocalPpmPolicy.Hwpm);
  InitializeSstCpConfig(SktId, mLocalPpmPolicy.Hwpm.SstCpSystemStatus, mLocalPpmPolicy.Hwpm.SstCpSetting);
  InitializeDynamicL1 (SktId, &mLocalPpmPolicy);
  ForceUncoreAndMeshRatio (&mLocalPpmStruct);
  MiscWorkaroundEnable (SktId, &mLocalPpmPolicy.AdvPwrMgtCtl);
  InitializePkgcSaPsCriteria (SktId, &mLocalPpmPolicy.PkgcCriteria);
  InitializeAVXICCPPreGrantLevel (SktId, mLocalPpmPolicy.AvxLicensePreGrant, mLocalPpmPolicy.AvxIccpLevel);
  InitializeGpssTimer (SktId, mLocalPpmPolicy.GpssTimer);
  InitializeDcstLuts (SktId);

  return;
}


VOID PpmMsrProgramming (
  EFI_CPU_PM_STRUCT *PpmStruct,
  PPM_POLICY_CONFIGURATION *PpmPolicy
  )
{
  InitializePowerControl (PpmStruct, &PpmPolicy->PowerCtl, 0);
  InitializeVrCurrentConfig (&PpmPolicy->AdvPwrMgtCtl, &PpmPolicy->PpoCurrentCfg, 0);
  InitializeRaplPowerLimitMsr (PpmStruct, &PpmPolicy->TurboPowerLimit, 0);
  InitializePlatformPowerLimitMsr (PpmStruct, &PpmPolicy->PsysConfig, 0);
  InitializeTurboRatioLimitCores (PpmStruct, &PpmPolicy->TurboRatioLimit, &PpmPolicy->PpmXe, 0);
  InitializeEnergyPerformanceBias (&PpmPolicy->PerfBiasConfig, 0);
  InitializeCStateLatency (PpmStruct, &PpmPolicy->PpmCst, 0);
  InitialzeHwpFeatures (PpmStruct, &PpmPolicy->Hwpm, 0);
  InitialzeConfigurePbf (PpmStruct, &PpmPolicy->Hwpm, 0);

  return;
}

VOID
SmmInitializeSmrrEmrr (
  VOID
  )
{
  UINT64                ValidMtrrAddressMask;
  EFI_CPUID_REGISTER    FeatureInfo;
  EFI_CPUID_REGISTER    FunctionInfo;
  UINT8                 PhysicalAddressBits;
  MSR_MTRRCAP_REGISTER    MtrrCap;

  //
  // Synchup SMRR/EMRR
  //
  MtrrCap.Uint32 = (UINT32) AsmReadMsr64 (MSR_IA32_MTRRCAP);
  if (MtrrCap.Bits.Smrr == 0) {
    return ;
  }

  //
  // Get physical CPU MTRR width in case of difference from BSP
  //
  AsmCpuid (CPUID_EXTENDED_FUNCTION, &FunctionInfo.RegEax, NULL, NULL, NULL);
  PhysicalAddressBits = 36;
  if (FunctionInfo.RegEax >= (UINT32)CPUID_VIR_PHY_ADDRESS_SIZE) {
    AsmCpuid (CPUID_VIR_PHY_ADDRESS_SIZE, &FeatureInfo.RegEax, NULL, NULL, NULL);
    PhysicalAddressBits = (UINT8) FeatureInfo.RegEax;
  }
  ValidMtrrAddressMask = (LShiftU64(1, PhysicalAddressBits) - 1) & 0xfffffffffffff000;

  AsmWriteMsr64 (MSR_SMRR_BASE, (mExchangeInfo->CpuSmmInitData.SmrrBase & 0xFFFFF000) | EFI_CACHE_WRITEBACK);
  AsmWriteMsr64 (MSR_SMRR_MASK, (~(mExchangeInfo->CpuSmmInitData.SmrrSize - 1)) & ValidMtrrAddressMask | B_EFI_CACHE_MTRR_VALID);
  if (MtrrCap.Bits.Prmrr == 1) {
    AsmWriteMsr64 (SKX_MSR_PRMRR_BASE, ((mExchangeInfo->CpuSmmInitData.SmrrBase + mExchangeInfo->CpuSmmInitData.SmrrSize - 0x100000)  & 0xFFFFF000) | EFI_CACHE_WRITEBACK);
    AsmWriteMsr64 (ICX_MSR_PRMRR_MASK, (~(0x100000 - 1)) & ValidMtrrAddressMask | B_EFI_CACHE_MTRR_VALID);
  }

  return;
}

#ifdef LT_FLAG

EFI_STATUS
SetApicBSPBit (
  IN  BOOLEAN   Enable
  )
{
  UINT64  ApicBaseReg;

  ApicBaseReg = AsmReadMsr64 (MSR_IA32_APIC_BASE);

  if (Enable) {
    ApicBaseReg |= 0x100;
  } else {
    ApicBaseReg &= 0xfffffffffffffe00;
  }

  AsmWriteMsr64 (MSR_IA32_APIC_BASE, ApicBaseReg);

  return EFI_SUCCESS;
}

/**
    Program Virtual Wire Mode.

    @param  BSP             Is this the Processor BSP.
    @param  VirtualWireMode Virtual Wire mode.

    @return None
**/
VOID
ProgramVirtualWireMode_RAS (
  BOOLEAN                       BSP,
  UINT32                        VirtualWireMode
  )
{
  UINT64                ApicBaseReg;
  EFI_PHYSICAL_ADDRESS  ApicBase;
  volatile UINT32       *EntryAddress;
  UINT32                EntryValue;

  ApicBaseReg = AsmReadMsr64 (MSR_IA32_APIC_BASE);
  ApicBase    = ApicBaseReg & 0xffffff000;

  //
  // Program the Spurious Vectore entry
  //
  EntryAddress  = (UINT32 *) (UINTN) (ApicBase + XAPIC_SPURIOUS_VECTOR_OFFSET);
  EntryValue    = *EntryAddress;
  EntryValue &= 0xFFFFFD0F;
  EntryValue |= 0x10F;
  *EntryAddress = EntryValue;

  //
  // Program the LINT1 vector entry as extINT
  //
  EntryAddress  = (UINT32 *) (UINTN) (ApicBase + XAPIC_LVT_LINT0_OFFSET);
  EntryValue    = *EntryAddress;
  if (VirtualWireMode != 0) {
    EntryValue |= 0x10000;
  } else {
    if (BSP) {
      EntryValue &= 0xFFFE00FF;
      EntryValue |= 0x700;
    } else {
      EntryValue |= 0x10000;
    }
  }
  *EntryAddress = EntryValue;

  //
  // Program the LINT1 vector entry as NMI
  //
  EntryAddress  = (UINT32 *) (UINTN) (ApicBase + XAPIC_LVT_LINT1_OFFSET);
  EntryValue    = *EntryAddress;
  EntryValue &= 0xFFFE00FF;
  if (BSP) {
    EntryValue |= 0x400;
  } else {
    EntryValue |= 0x10400;
  }
  *EntryAddress = EntryValue;

  if (VirtualWireMode && BSP) {
    //
    // Initialize the I0XApic RT table for virtual wire B
    //
    *(volatile UINT8   *)(UINTN)R_PCH_IO_APIC_INDEX = 0x10;
    *(volatile UINT32  *)(UINTN)R_PCH_IO_APIC_DATA= 0x0700;
    *(volatile UINT8   *)(UINTN)R_PCH_IO_APIC_INDEX = 0x11;
    *(volatile UINT32  *)(UINTN)R_PCH_IO_APIC_DATA  = (GetApicId () << 24);
  }
}

/**
    Send Interrupts to the Processor

    @param  BroadcastMode             Broadcast mode.
    @param  ApicID                    Apic ID of the processor for which interrupt to be sent.
    @param  VectorNumber              Vector Number.
    @param  DeliveryMode              Delivery mode.
    @param  TriggerMode               Trigger mode
    @param  Assert                    Assert


  @return EFI_SUCCESS                 If the interrupt sent successfully.
  @return EFI_NOT_READY
  @return EFI_INVALID_PARAMETER
**/
EFI_STATUS
SendInterrupt (
  IN  UINT32                               BroadcastMode,
  IN  UINT32                               ApicID,
  IN  UINT32                               VectorNumber,
  IN  UINT32                               DeliveryMode,
  IN  UINT32                               TriggerMode,
  IN  BOOLEAN                              Assert
  )
{
  UINT64                ApicBaseReg;
  EFI_PHYSICAL_ADDRESS  ApicBase;
  UINT32                ICRLow;
  UINT32                ICRHigh;

  //
  // Initialze ICR high dword, since XEON family processor needs
  // the destination field to be 0xFF when it is a broadcast
  //
  ICRHigh = 0xff000000;
  ICRLow  = VectorNumber | (DeliveryMode << 8);

  if (TriggerMode == TRIGGER_MODE_LEVEL) {
    ICRLow |= 0x8000;
  }

  if (Assert) {
    ICRLow |= 0x4000;
  }

  switch (BroadcastMode) {
  case BROADCAST_MODE_SPECIFY_CPU:
    ICRHigh = ApicID << 24;
    break;

  case BROADCAST_MODE_ALL_INCLUDING_SELF:
    ICRLow |= 0x80000;
    break;

  case BROADCAST_MODE_ALL_EXCLUDING_SELF:
    ICRLow |= 0xC0000;
    break;

  default:
    return EFI_INVALID_PARAMETER;
  }

  ApicBaseReg = AsmReadMsr64 (MSR_IA32_APIC_BASE);
  ApicBase    = ApicBaseReg & 0xffffff000;

  *(volatile UINT32 *) (UINTN) (ApicBase + XAPIC_ICR_HIGH_OFFSET)  = ICRHigh;
  *(volatile UINT32 *) (UINTN) (ApicBase + XAPIC_ICR_LOW_OFFSET)   = ICRLow;

  //MicroSecondDelay  (200);

  ICRLow = *(volatile UINT32 *) (UINTN) (ApicBase + XAPIC_ICR_LOW_OFFSET);
  if (ICRLow & 0x1000) {
    return EFI_NOT_READY;
  }

  return EFI_SUCCESS;
}

#endif //LT_FLAG
