/** @file
  Implementation of Last Boot Error silicon interface

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017-2019 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/RasDebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/SmmServicesTableLib.h>
#include <MaxSocket.h>
#include <Guid/LastBootErrorLogHob.h>
#include <Library/MpExtensionLib.h>
#include <Library/CpuConfigLib.h>
#include <Guid/LastBootErrorLogHob.h>
#include <SysRasData.h>
#include <Library/PlatPolicyLib.h>
#include <Library/ProcessorRasLib.h>

typedef struct {
  struct McBankStruct McReg[4];
} MACHINE_CHECK_ATTRIBUTE;

MACHINE_CHECK_ATTRIBUTE mMachineCheckAttribute[FixedPcdGet32 (PcdCpuMaxLogicalProcessorNumber)];

/**
  Detect machine check error for specified processor.

  @param[in] Buffer    The pointer to private data buffer.

  @retval None

**/
VOID
EFIAPI
MachineCheckErrorDetect (
  IN UINT8 *Buffer
  )
{
  MP_CPU_INFO   CpuInfo;
  UINTN        ProcessorNumber;
  UINT8         McIndex;
  UINT16        BankScope;
  RAS_POLICY    *PolicyData =NULL;
  union {
    UINT64        Data64;
    UINT64_STRUCT DataHiLow;
  } TempData;

  MSR_MCG_CAP_REGISTER McgCapReg;
  McgCapReg.Uint64 = AsmReadMsr64 (MSR_MCG_CAP);

  ProcessorNumber = GetProcessorInfo (&CpuInfo);
  PolicyData = GetRasPolicy ();

  for (McIndex = 0; McIndex < McgCapReg.Bits.Count; McIndex++) {
    TempData.Data64 = AsmReadMsr64(MCI_REG_BASE + McIndex * 4 + MC_BANK_STATUS_REG);
    if (PolicyData->McaPolicy.McBankWarmBootClearEn) {
       //
       // Clear MC status MSR to ensure all status is cleared before ECMA enable
       //
       AsmWriteMsr64(MCI_REG_BASE + McIndex * 4 + MC_BANK_STATUS_REG, 0x00);
    }
    //
    // Uncore mcbank status is captured in PEI phase, skipping here.
    //
    BankScope = GetMcBankScope (McIndex);
    if (BankScope == MC_SCOPE_PKG) {
      break;
    }
    //
    // Skip if there is no fatal error / log not valid / not enabled
    //
    if ((TempData.DataHiLow.hi & (MSR_LOG_VALID | MSR_LOG_UC | MSR_LOG_EN)) != (MSR_LOG_VALID | MSR_LOG_UC | MSR_LOG_EN)) {
      continue;
    }
    //
    // Capture log information
    //
    mMachineCheckAttribute[ProcessorNumber].McReg[McIndex].SocketId   = (UINT8) CpuInfo.Package;
    mMachineCheckAttribute[ProcessorNumber].McReg[McIndex].McBankNum  = McIndex;
    mMachineCheckAttribute[ProcessorNumber].McReg[McIndex].McStatus   = TempData.DataHiLow;
    TempData.Data64 = AsmReadMsr64 (MCI_REG_BASE + McIndex * 4 + MC_BANK_ADDRESS_REG);
    mMachineCheckAttribute[ProcessorNumber].McReg[McIndex].McAddress  = TempData.DataHiLow;
    TempData.Data64 = AsmReadMsr64 (MCI_REG_BASE + McIndex * 4 + MC_BANK_MISC_REG);
    mMachineCheckAttribute[ProcessorNumber].McReg[McIndex].McMisc     = TempData.DataHiLow;
  } // mcbank loop
}

/**
  Update PrevBootErrHob to conbine Ap and NBSP error.

  This HOB is used to create ACPI BERT table information to pass to the OS.

  @retval None

**/
VOID
EFIAPI
UpdatePrevBootErrHob (
  VOID
  )
{
  EFI_HOB_GUID_TYPE                     *GuidHob;
  PREV_BOOT_ERR_SRC_HOB                 *PrevBootErrSrcHob;
  MCBANK_ERR_INFO                       *McBankErrInfo;
  UINT8                                 McIndex;
  UINT16                                ProcessorNumber;
  union {
    UINT64        Data64;
    UINT64_STRUCT DataHiLow;
  } TempData;

  MSR_MCG_CAP_REGISTER McgCapReg;
  McgCapReg.Uint64 = AsmReadMsr64 (MSR_MCG_CAP);

  //
  // Get the HOB Pointer to update the possible new error source
  //
  GuidHob = GetFirstGuidHob (&gPrevBootErrSrcHobGuid);

  if (GuidHob != NULL) {
    PrevBootErrSrcHob = (PREV_BOOT_ERR_SRC_HOB *)GET_GUID_HOB_DATA (GuidHob);
    //
    // Move to next available entry
    //
    McBankErrInfo = (MCBANK_ERR_INFO *) ((UINTN) PrevBootErrSrcHob + (PrevBootErrSrcHob->Length));
    for (ProcessorNumber = 0; ProcessorNumber < gSmst->NumberOfCpus; ProcessorNumber++) {
      for (McIndex = 0; McIndex < McgCapReg.Bits.Count; McIndex++) {
        //
        // Skip if there is no fatal error / log not valid / not enabled
        //
        if ((mMachineCheckAttribute[ProcessorNumber].McReg[McIndex].McStatus.hi & (MSR_LOG_VALID | MSR_LOG_UC | MSR_LOG_EN)) != (MSR_LOG_VALID | MSR_LOG_UC | MSR_LOG_EN)) {
          continue;
        }
        //
        // Check availability of Hob space before updating Hob with record
        //
        if ((sizeof (PREV_BOOT_ERR_SRC_HOB) - PrevBootErrSrcHob->Length) < sizeof (MCBANK_ERR_INFO)) {
          RAS_DEBUG((LEVEL_FUNC_FLOW, "Prev Boot Err HOB out of space!!\n"));
          return;
        }
        //
        // Capture log information
        //
        McBankErrInfo->Type         = McBankType;
        McBankErrInfo->Socket       = mMachineCheckAttribute[ProcessorNumber].McReg[McIndex].SocketId;
        McBankErrInfo->ApicId       = (UINT16)mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber].CpuMiscData.ApicID;
        McBankErrInfo->McBankNum    = mMachineCheckAttribute[ProcessorNumber].McReg[McIndex].McBankNum;
        TempData.DataHiLow          = mMachineCheckAttribute[ProcessorNumber].McReg[McIndex].McStatus;
        McBankErrInfo->McBankStatus = TempData.Data64;
        TempData.DataHiLow          = mMachineCheckAttribute[ProcessorNumber].McReg[McIndex].McAddress;
        McBankErrInfo->McbankAddr   = TempData.Data64;
        TempData.DataHiLow          = mMachineCheckAttribute[ProcessorNumber].McReg[McIndex].McMisc;
        McBankErrInfo->McBankMisc   = TempData.Data64;

        RAS_DEBUG((LEVEL_FUNC_FLOW, "PrevBootErrLog - ApicId: 0x%x, MCBANK: 0x%x, Status = 0x%08x%08x, Addr = 0x%08x%08x, Misc = 0x%08x%08x\n",
          McBankErrInfo->ApicId, McBankErrInfo->McBankNum,
          *(UINT32*) ((UINTN) &McBankErrInfo->McBankStatus + 4), *(UINT32*) ((UINTN) &McBankErrInfo->McBankStatus),
          *(UINT32*) ((UINTN) &McBankErrInfo->McbankAddr + 4), *(UINT32*) ((UINTN) &McBankErrInfo->McbankAddr),
          *(UINT32*) ((UINTN) &McBankErrInfo->McBankMisc + 4), *(UINT32*) ((UINTN) &McBankErrInfo->McBankMisc)
        ));
        McBankErrInfo++; // Move to next entry
        PrevBootErrSrcHob -> Length += sizeof (MCBANK_ERR_INFO); // Add to length of Hob
      } // mcbank loop
    } // processor loop
  } // prevlog data Hob

}

/**
  Collects All APs mcbank error data for building a HOB.
  This HOB is used to create ACPI BERT table information to pass to the OS.

  @retval None

**/
VOID
EFIAPI
CollectApMcBankInfo (
  VOID
  )
{
  MultiProcessorExecute (OneApPerCore, ALL_PACKAGE, ALL_CORE, TRUE, MachineCheckErrorDetect, NULL);
  UpdatePrevBootErrHob ();
}
