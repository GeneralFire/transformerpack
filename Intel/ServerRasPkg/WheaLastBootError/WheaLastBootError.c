/** @file
  Implementation of the WHEA BERT table.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#include <PiSmm.h>
#include <Library/RasDebugLib.h>
#include <Library/PlatPolicyLib.h>
#include <Library/WheaSupportLib.h>
#include <Library/HobLib.h>
#include <Library/ErrorReportLib.h>
#include <Library/PlatMemErrorDataLib.h>
#include <Library/LastBootErrorDxeLib.h>
#include <Library/ProcessorRasLib.h>
#include <Guid/LastBootErrorLogHob.h>
#include <Cpu/CpuCoreRegs.h>
#include <PCIeErrorTypes.h>

UINT64 mBERTAddress = 0;

/**
  Checks and updates the previous boot fatal CBO errors.

  @param[in] McBankErrInfo          McBankErrInfo contains the Prev boot fatal QPI/PCU error type.

  @retval Status.

**/
EFI_STATUS
UpdateChaError (
  MCBANK_ERR_INFO     *McBankErrInfo
  )
{
  EFI_STATUS                    Status;
  EMCA_MC_SIGNATURE_ERR_RECORD  McErrSig;

  McErrSig.ApicId               = McBankErrInfo->ApicId;
  McErrSig.BankIndex            = McBankErrInfo->McBankNum;
  McErrSig.Signature.McSts      = McBankErrInfo->McBankStatus;
  McErrSig.Signature.McAddr     = McBankErrInfo->McbankAddr;
  McErrSig.Signature.McMisc     = McBankErrInfo->McBankMisc;

  Status = ReportProcessorBootErrorLog ((UINT8) McBankErrInfo->McBankNum, McBankErrInfo->Socket, EFI_GENERIC_ERROR_PROC_ERROR_TYPE_CACHE, EFI_ACPI_6_2_ERROR_SEVERITY_FATAL, &McErrSig, mBERTAddress);

  return Status;
}

/**
Checks and updates the previous boot fatal PCI-EX errors.

@param[in] McBankErrInfo          McBankErrInfo contains the Prev boot fatal QPI/PCU error type.

@retval Status.

**/
EFI_STATUS
UpdatePciError (
  MCBANK_ERR_INFO     *McBankErrInfo
)
{
  EFI_STATUS              Status;
  UINT8                   Bus;
  UINT8                   Device;
  UINT8                   Function;
  UINT8                   Severity;
  IA32_MCI_STATUS_MSR_STRUCT             McBankSts;

  McBankSts.Data = McBankErrInfo->McBankStatus;
  Status = EFI_NOT_FOUND;

  Bus = (UINT8) GET_BUS_LOG_MC6_MISC (McBankErrInfo->McBankMisc);
  Device = (UINT8) GET_DEVICE_LOG_MC6_MISC (McBankErrInfo->McBankMisc);
  Function = (UINT8) GET_FUNCTION_LOG_MC6_MISC (McBankErrInfo->McBankMisc);

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "[WheaBERT] UpdatePciError: Skt: %x, Bus: %x, Dev: %x, Func: %x\n", McBankErrInfo->Socket, Bus, Device, Function));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "[WheaBERT] UpdatePciError: McMisc: %lx, McSts: %lx \n", McBankErrInfo->McBankMisc, McBankErrInfo->McBankStatus));
  //
  // IOMCA bank Uncorrected error check
  //
  if ((McBankSts.Bits.mca_code == IOMCA_MCACOD_ERR_MSK) && (McBankSts.Bits.ms_code == 0)) {
    if (McBankSts.Bits.pcc == 1) {
      Severity = FATAL_ERROR_TYPE;
    } else {
      //
      // uncorrected non fatal check (pcc=0 and s=1 ) is done before calling this function.
      //
      Severity = NONFATAL_ERROR_TYPE;
    }
    Status = ReportPcieBootErrorLog (McBankErrInfo->Socket, Bus, Device, Function, NULL, Severity, mBERTAddress);
  }
  return Status;
}

/**
  Checks and updates the previous boot fatal QPI/PCU errors.

  @param[in] McBankErrInfo          McBankErrInfo contains the Prev boot fatal QPI/PCU error type.

  @retval Status.

**/
EFI_STATUS
UpdateQpiPcuError (
  MCBANK_ERR_INFO     *McBankErrInfo
  )
{
  EFI_STATUS                    Status;
  EMCA_MC_SIGNATURE_ERR_RECORD  McErrSig;

  McErrSig.ApicId               = McBankErrInfo->ApicId;
  McErrSig.BankIndex            = McBankErrInfo->McBankNum;
  McErrSig.Signature.McSts      = McBankErrInfo->McBankStatus;
  McErrSig.Signature.McAddr     = McBankErrInfo->McbankAddr;
  McErrSig.Signature.McMisc     = McBankErrInfo->McBankMisc;

  Status = ReportProcessorBootErrorLog ((UINT8) McBankErrInfo->McBankNum, McBankErrInfo->Socket, EFI_GENERIC_ERROR_PROC_ERROR_TYPE_UNKNOWN, EFI_ACPI_6_2_ERROR_SEVERITY_FATAL, &McErrSig, mBERTAddress);

  return Status;
}

/**
  Checks and updates the previous boot fatal Core errors.

  @param[in] McBankErrInfo          McBankErrInfo contains the Prev boot fatal Core error type.

  @retval Status.

**/
EFI_STATUS
UpdateCoreError (
  MCBANK_ERR_INFO     *McBankErrInfo
  )
{
  EFI_STATUS                    Status;
  EMCA_MC_SIGNATURE_ERR_RECORD  McErrSig;

  McErrSig.ApicId               = McBankErrInfo->ApicId;
  McErrSig.BankIndex            = McBankErrInfo->McBankNum;
  McErrSig.Signature.McSts      = McBankErrInfo->McBankStatus;
  McErrSig.Signature.McAddr     = McBankErrInfo->McbankAddr;
  McErrSig.Signature.McMisc     = McBankErrInfo->McBankMisc;

  Status = ReportProcessorBootErrorLog ((UINT8) McBankErrInfo->McBankNum, McBankErrInfo->Socket, EFI_GENERIC_ERROR_PROC_ERROR_TYPE_MICRO_ARCH, EFI_ACPI_6_2_ERROR_SEVERITY_FATAL, &McErrSig, mBERTAddress);

  return Status;
}

/**
  Checks and updates the previous boot fatal memory errors.

  @param[in] McBankErrInfo          McBankErrInfo contains the Prev boot fatal memory error type.

  @retval Status.

**/
EFI_STATUS
UpdateMemoryError (
  MCBANK_ERR_INFO     *McBankErrInfo
  )
{
  EFI_STATUS                        Status;
  EMCA_MC_SIGNATURE_ERR_RECORD      McErrSig;
  EFI_PLATFORM_MEMORY_ERROR_DATA    MemoryErrorData;


  McErrSig.ApicId               = McBankErrInfo->ApicId;
  McErrSig.BankIndex            = McBankErrInfo->McBankNum;
  McErrSig.Signature.McSts      = McBankErrInfo->McBankStatus;
  McErrSig.Signature.McAddr     = McBankErrInfo->McbankAddr;
  McErrSig.Signature.McMisc     = McBankErrInfo->McBankMisc;

  Status = FillPlatformMemoryErrorData (McBankErrInfo->Socket, &McErrSig, TRUE, &MemoryErrorData);
  if (!EFI_ERROR (Status)) {
    Status = ReportPlatMemBootErrorLog ((UINT8) McBankErrInfo->McBankNum, McBankErrInfo->Socket, EFI_ACPI_6_2_ERROR_SEVERITY_FATAL, &MemoryErrorData, &McErrSig, mBERTAddress);
  }

  return Status;
}

/**
  Checks and updates the previous boot errors. This function is currently implemented for McBank errors.

  @retval Status.

**/
EFI_STATUS
CheckAndUpdatePrevBootErrors (
  VOID
  )
{
  EFI_STATUS                              Status;
  EFI_STATUS                              ReturnStatus;
  UINTN                                   Length;
  MCBANK_ERR_INFO                         *McBankErrInfo;
  IA32_MCI_STATUS_MSR_STRUCT              MsrIa32MCiStatusReg;
  EFI_HOB_GUID_TYPE                       *GuidHob;
  PREV_BOOT_ERR_SRC_HOB                   *PrevBootErrSrcHob = NULL;
  UINT16                                  Scope;
  UINT32                                  UnitType;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "[WheaBERT] CheckAndUpdatePrevBootErrors: Inside the function\n"));

  ReturnStatus = EFI_SUCCESS; // assume success

  //
  // Get the HOB Pointer to update the new error source
  //
  GuidHob = GetFirstGuidHob (&gPrevBootErrSrcHobGuid);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  PrevBootErrSrcHob = (PREV_BOOT_ERR_SRC_HOB *) GET_GUID_HOB_DATA (GuidHob);

  //
  // No error record come out of the function.
  //
  if (PrevBootErrSrcHob->Length <= 2) {
    return EFI_SUCCESS;
  }

  for (Length = 2; Length < PrevBootErrSrcHob->Length; Length += sizeof (MCBANK_ERR_INFO)) {
    McBankErrInfo = (MCBANK_ERR_INFO *) ((UINTN) PrevBootErrSrcHob + Length);

    if (McBankErrInfo->Type != McBankType) {
      continue;
    }

    //
    // Check for Fatal/Non-Fatal error.
    //
    MsrIa32MCiStatusReg.Data = McBankErrInfo->McBankStatus;
    if (MsrIa32MCiStatusReg.Bits.uc == 0 || ((MsrIa32MCiStatusReg.Bits.pcc == 0) && (MsrIa32MCiStatusReg.Bits.s == 0))){
      //
      // UCNA and Corrected errors are not logged to BERT
      //
      continue;
    }

    Scope = GetMcBankScope (McBankErrInfo->McBankNum);
    UnitType = GetMcBankUnitType (McBankErrInfo->McBankNum);

    RAS_DEBUG ((
      LEVEL_FUNC_FLOW,
      "[WheaBERT] CheckAndUpdatePrevBootErrors: McBankNum = %2d, Scope = 0x%04x, UnitType = 0x%08x\n",
      McBankErrInfo->McBankNum,
      Scope,
      UnitType
      ));

    if (UnitType == MCA_UNIT_TYPE_M2MEM || UnitType == MCA_UNIT_TYPE_IMC) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "[WheaBERT] CheckAndUpdatePrevBootErrors: Memory MC bank errors detect\n"));
      Status = UpdateMemoryError (McBankErrInfo);
    } else if (Scope == MC_SCOPE_CORE || Scope == MC_SCOPE_TD) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "[WheaBERT] CheckAndUpdatePrevBootErrors: Processor core errors detect\n"));
      Status = UpdateCoreError (McBankErrInfo);
    } else if (UnitType == MCA_UNIT_TYPE_CHA) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "[WheaBERT] CheckAndUpdatePrevBootErrors: CBO errors detect\n"));
      Status = UpdateChaError (McBankErrInfo);
    } else if (UnitType == MCA_UNIT_TYPE_PCU || UnitType == MCA_UNIT_TYPE_KTI) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "[WheaBERT] CheckAndUpdatePrevBootErrors: QPI errors detect\n"));
      Status = UpdateQpiPcuError (McBankErrInfo);
    } else if (UnitType == MCA_UNIT_TYPE_IIO) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "[WheaBERT] CheckAndUpdatePrevBootErrors: PCI-EX errors detect\n"));
      Status = UpdatePciError (McBankErrInfo);
    } else {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "[WheaBERT] CheckAndUpdatePrevBootErrors: unknown/unsupported MC bank, skip\n"));
      Status = EFI_SUCCESS; // skip, assume success
    }

    //
    // Keep the most recent error, if any, but keep trying to report more.
    //
    if (EFI_ERROR (Status)) {
      ReturnStatus = Status;
    }
  }

  //
  // Return the most recent error, if any.
  //
  return ReturnStatus;
}

/**
  Entry point for the WHEA BERT initialization.

  This function initializes the WHEA last boot error.


  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval Status.

**/
EFI_STATUS
EFIAPI
InitializWheaLastBootError (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS                      Status = EFI_SUCCESS;
  RAS_POLICY                      *PolicyData =NULL;

  RAS_DEBUG((LEVEL_FUNC_FLOW, "[WheaBERT]InitializWheaLastBootError\n"));
  PolicyData = GetRasPolicy ();

  RAS_DEBUG((LEVEL_FUNC_FLOW, "[WheaBERT]WheaSupportEn: %d\n", PolicyData->WheaPolicy.WheaSupportEn));

  if (PolicyData->WheaPolicy.WheaSupportEn == 0) {
    return EFI_SUCCESS;
  }

  GetBERTErrorAddress (&mBERTAddress);

  //
  // Collects All APs mcbank error.
  //
  CollectApMcBankInfo ();

  //
  // Collect if any previous boot fatal errors and update them in whea error boot record table that arew supported.
  //
  CheckAndUpdatePrevBootErrors ();

  return Status;
}


