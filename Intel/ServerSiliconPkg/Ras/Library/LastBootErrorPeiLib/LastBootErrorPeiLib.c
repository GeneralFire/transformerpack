/** @file
  Implementation of Last Boot Error silicon interface

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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

#include <Library/PipeSyncLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/DebugLib.h>
#include <MaxSocket.h>
#include <Guid/LastBootErrorLogHob.h>
#include <Library/HobLib.h>
#include <SysRasData.h>
#include <Library/KtiApi.h>
#include <Library/PmcLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Register/ArchitecturalMsr.h>
#include <Library/MemFmcIpLib.h>
#include <Library/SpdAccessLib.h>
#include <Library/BasicIpBlockServicesLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Guid/MrcPlatformHooksGuid.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/RasRcPolicy.h>
#include <Library/EnhancedWarningLogLib.h>

#include "LastBootErrorPeiLibInternal.h"

//
// Bit 25 Processor Contect corrupt (Upper DWORD in MCA Status)
//
#define MCA_PROCESSOR_CONTEXT_CORRUPT   0x2000000

//
// Bit 29 Uncorrectable Error (Upper DWORD in MCA Status)
//
#define MCA_UNCORRECTABLE_ERROR       0x20000000

UINT8 mSkxM2mBanks[] = {MCBANK7, MCBANK8, MCBANK_END};
UINT8 mIcxM2mBanks[] = {MCBANK12, MCBANK16, MCBANK20, MCBANK24, MCBANK_END};
UINT8 mIcxdM2mBanks[] = {MCBANK12, MCBANK16, MCBANK_END};
UINT8 mSnrM2mBanks[] = {MCBANK12, MCBANK_END};

typedef struct {
  UINT8 Begin;
  UINT8 End;
} MCA_BANK_RANGE;

//SKX has 2 different range
MCA_BANK_RANGE mSkxMemErrorBankRanges[] = {
    //M2MEM Range
    {MCBANK7, MCBANK8},
    //MCCHAN Range
    {MCBANK13, MCBANK18}
  };

// ICX, ICXD, SNR, SPR has continuous range.
// Below is range include M2MEM and MCCHAN
MCA_BANK_RANGE mIcxMemErrorBankRanges[] = {
    {MCBANK12, MCBANK27}
  };
MCA_BANK_RANGE mIcxdMemErrorBankRanges[] = {
    {MCBANK12, MCBANK19}
  };
MCA_BANK_RANGE mSnrMemErrorBankRanges[] = {
    {MCBANK12, MCBANK15}
  };
MCA_BANK_RANGE mSprMemErrorBankRanges[] = {
    {MCBANK12, MCBANK20}
  };

/**
  Check given MC bank matches Memory Bank in current SOC

  @param[in] McBanknum - McBank to check

  @retval TRUE  - Is a Memory MC bank
  @retval FALSE - Not a Memory MC Bank
**/
BOOLEAN
IsMemoryMcBank (
  IN UINT8 McBankNum
  )
{
  MCA_BANK_RANGE  *Ranges   = NULL;
  UINT8           Index     = 0;
  UINT8           NumRanges = 0;
  BOOLEAN         IsMcBank  = FALSE;

  if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    Ranges = mIcxdMemErrorBankRanges;
    NumRanges = ARRAY_SIZE (mIcxdMemErrorBankRanges);
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    Ranges = mIcxMemErrorBankRanges;
    NumRanges = ARRAY_SIZE (mIcxMemErrorBankRanges);
  } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    Ranges = mSnrMemErrorBankRanges;
    NumRanges = ARRAY_SIZE (mSnrMemErrorBankRanges);
  } else if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    Ranges = mSkxMemErrorBankRanges;
    NumRanges = ARRAY_SIZE (mSkxMemErrorBankRanges);
  } else if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    Ranges = mSprMemErrorBankRanges;
    NumRanges = ARRAY_SIZE (mSprMemErrorBankRanges);
  } else {
    return IsMcBank;
  }

  for (Index = 0; Index < NumRanges; Index++) {
    if (McBankNum >= Ranges[Index].Begin &&
        McBankNum <= Ranges[Index].End) {
      IsMcBank = TRUE;
      break;
    }
  }
  return IsMcBank;
}

/**
  Check for Memory MCA Error

  @param[in]  Socket       - Socket Number
  @param[in]  McBankNum    - BankNumber to check for Error Status
  @param[in]  McBankStatus - Status information obtained from MCA Bank register

  @retval  TRUE  - Memory Error Found
  @retval  FALSE - Memory Error Not Found
**/
BOOLEAN
CheckMemoryBootError (
  IN UINT8         Socket,
  IN UINT8         McBankNum,
  IN UINT64        McBankStatus
  )
{
  // Moved variable usage from IA32_MCI_STATUS_MSR_STRUCT to UINT64_STRUCT because of
  // build error thrown when using /Od option during arthimentic operation
  // performed between UINT64 variables in PEI 32 bit mode.
  UINT64_STRUCT                           MsrData;

  if (IsMemoryMcBank (McBankNum)) {

    MsrData = *((UINT64_STRUCT *) &McBankStatus);

    //
    // check for fatal error.
    //
    if (((MsrData.hi & MCA_PROCESSOR_CONTEXT_CORRUPT) != 0) &&
      ((MsrData.hi & MCA_UNCORRECTABLE_ERROR) != 0)) {
      //Return Previous Memory Boot Error.
      DEBUG ((DEBUG_ERROR, "CheckMemoryBootError: Memory MCA Error in Socket = %d, Bank = %d\n", Socket, McBankNum));
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Check if the given MC index belongs to M2M. If so, returns the M2M# it belongs to.

  @param[in]  Index - MC banks index

  @return     M2M number the index belongs to. Otherwise -1 if not found.
**/
INT8
GetM2mInstance (
  IN UINT8        McIndex
  )
{
  UINT8 *M2mBanks;
  INT8   i;

  if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    M2mBanks = &mIcxdM2mBanks[0];
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    M2mBanks = &mIcxM2mBanks[0];
  } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    M2mBanks = &mSnrM2mBanks[0];
  } else if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    M2mBanks = &mSkxM2mBanks[0];
  } else {
    return -1;
  }

  for (i = 0; M2mBanks[i] != MCBANK_END; i++) {
    if (M2mBanks[i] == McIndex) {
      return i;
    }
  }

  return -1;
}

/**
  Build RAS Data Structure and save it into a HOB.

  @param[in]  RasData - pointer to RasData structure on stack.
**/
VOID
SaveBootErrorRecordToHob (
  IN struct SysRasData  *RasData
  )
{
  PREV_BOOT_ERR_SRC_HOB                 PrevBootErrorHob;
  MCBANK_ERR_INFO                       *McBankErrInfo;
  UINT8                                 i;

  ZeroMem ((UINT8 *) &PrevBootErrorHob, sizeof (PrevBootErrorHob));

  PrevBootErrorHob.Length = (UINT16) RasData->PrevBootErr.ValidEntries * sizeof (MCBANK_ERR_INFO) + 2;  // 2 is for the length field in the hob
  McBankErrInfo = (MCBANK_ERR_INFO*) ((UINTN) &PrevBootErrorHob + 2);

  for (i = 0; i < RasData->PrevBootErr.ValidEntries; i++) {
    McBankErrInfo->Type         = McBankType;
    McBankErrInfo->Socket       = RasData->PrevBootErr.McRegs[i].SocketId;
    McBankErrInfo->McBankNum    = RasData->PrevBootErr.McRegs[i].McBankNum;
    *(UINT64_STRUCT*) (UINTN) &McBankErrInfo->McBankStatus = RasData->PrevBootErr.McRegs[i].McStatus;
    *(UINT64_STRUCT*) (UINTN) &McBankErrInfo->McbankAddr   = RasData->PrevBootErr.McRegs[i].McAddress;
    *(UINT64_STRUCT*) (UINTN) &McBankErrInfo->McBankMisc   = RasData->PrevBootErr.McRegs[i].McMisc;
    McBankErrInfo++; // Move to next entry
  }
  //
  // Build HOB for prev boot errors
  //
  BuildGuidDataHob (
    &gPrevBootErrSrcHobGuid,
    &PrevBootErrorHob,
    sizeof(PREV_BOOT_ERR_SRC_HOB)
    );

  return;
}

VOID
ReportMachineCheckException (
  PSYSHOST      Host,
  UINT8         Socket,
  UINT8         Core,
  UINT8         Bank,
  UINT64_STRUCT McStatus,
  UINT64_STRUCT McAddress,
  UINT64_STRUCT McMisc
  )
{
  EWL_ENTRY_TYPE22  MceEntry;
  UINT32            CpuId = 0;

  ZeroMem(&MceEntry, sizeof(EWL_ENTRY_TYPE22));
  AsmCpuid (1, &CpuId, NULL, NULL, NULL);//EFI_CPUID_VERSION_INFO

  MceEntry.Header.Size     = sizeof(EWL_ENTRY_TYPE22);
  MceEntry.Header.Type     = EwlType22;
  MceEntry.Header.Severity = EwlSeverityInfo;
  MceEntry.Context.MajorWarningCode = WARN_MCA_UNCORRECTABLE_ERROR;
  MceEntry.Context.MinorWarningCode = WARN_PREVIOUS_BOOT_MCA_MINOR_CODE;

  MceEntry.Socket       = Socket;
  MceEntry.Core         = Core;
  MceEntry.CpuId        = CpuId;
  MceEntry.McBankNum    = Bank;
  MceEntry.McBankStatus = (LShiftU64(McStatus.hi, 32) & (UINT64)McStatus.lo);
  MceEntry.McBankAddr   = (LShiftU64(McAddress.hi, 32) & (UINT64)McAddress.lo);
  MceEntry.McBankMisc   = (LShiftU64(McMisc.hi, 32) & (UINT64)McMisc.lo);


  //log the warning in the Enhanced Warning Log structure

  EwlLogEntry (&MceEntry.Header);
}

/**
  Apply the WA for invalid crediting MCA.
**/
VOID
ClearM2mMciStatus (
  VOID
  )
{
  UINT8                       Socket;
  UINT8                       McId;
  UINT64_STRUCT               MsrData;
  IA32_MCI_STATUS_MSR_STRUCT  MciStatusMsr;

  CONST UINT32 M2mMciStatus[] = {
    MSR_IA32_MC12_STATUS,   // M2M0
    MSR_IA32_MC16_STATUS,   // M2M1
    MSR_IA32_MC20_STATUS,   // M2M2
    MSR_IA32_MC24_STATUS    // M2M3
  };

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!SocketPresent (Socket)) {
      continue;
    }

    for (McId = 0; McId < MAX_IMC; McId++) {
      MsrData = ReadMsrPipeHL (Socket, M2mMciStatus[McId]);
      MciStatusMsr.Data = *((UINT64 *) &MsrData);
      MciStatusMsr.Bits.val = 0;
      MciStatusMsr.Bits.over = 0;
      MciStatusMsr.Bits.uc = 0;
      MciStatusMsr.Bits.pcc = 0;
      MciStatusMsr.Bits.ms_code = 0;
      MciStatusMsr.Bits.mca_code = 0;
      MsrData = *((UINT64_STRUCT *) &MciStatusMsr.Data);
      WriteMsrPipeHL (Socket, M2mMciStatus[McId], MsrData);
    }
  }
}

/**
  Checks MSRs with UC set and logs the register data for further use.
**/
VOID
EFIAPI
CollectPrevBootFatalErrors (
  PSYSHOST    Host
  )
{
  UINT16                     NoOfMcBanks;
  UINT8                      McIndex;
  UINT8                      Socket;
  UINT8                      Ch;
  UINT8                      Dimm;
  UINT64_STRUCT              McStatus;
  UINT64_STRUCT              McAddress;
  UINT64_STRUCT              McMisc;
  UINT64_STRUCT              MsrReg;
  UINT8                      Entry;
  SYSRASDATA                 RasData;
  BOOLEAN                    InDwr;
  UINT8                      SpdReg;
  BOOT_STATUS_REGISTER       Bsr;
  BOOT_STATUS_HIGH_REGISTER  BsrHi;
  INT8                       M2mBank;
  BOOLEAN                    LogMemError = FALSE;
  DDR_TECHNOLOGY_TYPE        DdrType;
  RAS_RC_POLICY_PPI          *RasRcPolicyPpi;
  EFI_STATUS                 Status;
  BOOLEAN                    McStsUcErrorCheck;

  Status = PeiServicesLocatePpi (&gRasRcPolicyPpiGuid, 0, NULL, (VOID **)&RasRcPolicyPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  InDwr =  PmcIsDwrBootMode() ;
  DEBUG ((DEBUG_INFO, "CollectPrevBootFatalErrors: inDWR: %d\n", InDwr));

  NoOfMcBanks = (UINT16) AsmMsrBitFieldRead64 (MSR_MCG_CAP, 0, 7);
  Entry = 0;
  RasData.PrevBootErr.ValidEntries = 0;

  DEBUG ((DEBUG_INFO, "CollectPrevBootFatalErrors - start\n"));
  ZeroMem ((UINT8 *) &RasData, sizeof (RasData));

  MsrReg.hi = 0;
  MsrReg.lo = 0;
  McStsUcErrorCheck = 0;

  if (IsSiliconWorkaroundEnabled ("S1407456470")) {
    //
    // Workaround an invalid crediting MCA by clearing the relevant MCi_STATUS
    // fields on warm reset, before creating a previous boot error entry for it.
    //
    if (CheckCurrentReset (POST_RESET_WARM)) {
      ClearM2mMciStatus ();
    }
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (McIndex = 0; McIndex < NoOfMcBanks; McIndex++) {
      //
      // Skip skts that are not present
      //
      if (SocketPresent (Socket) == FALSE) {
        continue;
      }

      if (RasData.PrevBootErr.ValidEntries >= MAX_PREV_BOOT_ERR_ENTRIES) {
        DEBUG ((DEBUG_INFO, "Warning! Num of MC BANKS with UC error exceeds %d - max entries!\n", MAX_PREV_BOOT_ERR_ENTRIES));
        return;
      }
      DEBUG ((DEBUG_INFO, "CollectPrevBootFatalErrorsReadMsrPipe - start\n"));
      McStatus = ReadMsrPipeHL (Socket, MCI_REG_BASE + McIndex * 4 + MC_BANK_STATUS_REG);
      McAddress = ReadMsrPipeHL (Socket, MCI_REG_BASE + McIndex * 4 + MC_BANK_ADDRESS_REG);
      McMisc = ReadMsrPipeHL (Socket, MCI_REG_BASE +  McIndex * 4 + MC_BANK_MISC_REG);
      DEBUG ((DEBUG_INFO, "CollectPrevBootFatalErrorsReadMsrPipe - end\n"));

      if (RasRcPolicyPpi->McBankWarmBootClearError == TRUE) {
        WriteMsrPipeHL (Socket, MCI_REG_BASE + McIndex * 4 + MC_BANK_STATUS_REG, MsrReg);
      }

      if (InDwr) {
        DEBUG ((DEBUG_INFO, "Socket: %d, Machine Check Bank %d: Status: 0x%08x%08x, Address: 0x%08x%08x, Misc: 0x%08x%08x\n",
                 Socket, McIndex, McStatus.hi, McStatus.lo, McAddress.hi, McAddress.lo, McMisc.hi, McMisc.lo));

        if (McStatus.hi & BIT31) {
          ReportMachineCheckException(Host, Socket, 0, McIndex, McStatus, McAddress, McMisc);
          M2mBank = GetM2mInstance (McIndex);
          if (M2mBank >= 0) {
            if (McStatus.lo & BIT21) {
              DdrType = Ddr4Type;
#ifdef DDR5_SUPPORT
              DdrType = Ddr5Type;
#endif // DDR5_SUPPORT
              InitializeSpd (Socket, DdrType);
              Ch = (McStatus.lo & 0xF) + (M2mBank * MAX_DIMM);
              for (Dimm = 0; Dimm < 2; Dimm++) {
                SpdReadByte (Socket, Ch, Dimm, SPD_KEY_BYTE, &SpdReg);
                DEBUG ((DEBUG_INFO, "Dimm: %d, SPD Key Byte = %d\n", Dimm, SpdReg));
                if (SPD_TYPE_AEP == SpdReg) {
                  ReadBootStatusRegisters (Host, Socket, Ch, Dimm, &Bsr, &BsrHi);
                  DEBUG ((DEBUG_INFO, "Socket: %d, Channel: %d, Dimm: %d, BSR: 0x%x - 0x%x\n", Socket, Ch, Dimm, Bsr.Data, BsrHi.Data));
                  EwlOutputType23 (WARN_NVMCTRL_MEDIA_STATUS, WARN_NVMCTRL_MEDIA_DWR, Socket, Ch, Dimm, Bsr.Data, BsrHi.Data);
                }
              }
            }
          }
        }
      }
      //
      // Check previous boot Uncorrected errors.
      //
      McStsUcErrorCheck = ((McStatus.hi & (MSR_LOG_VALID | MSR_LOG_UC | MSR_LOG_EN)) != (MSR_LOG_VALID | MSR_LOG_UC | MSR_LOG_EN)) ? 00 : 01;
      //
      // Skip if there is no fatal error/log not valid
      //
      if (McStsUcErrorCheck == 00) {
       continue;
      }

      DEBUG ((DEBUG_ERROR, "Socket: %d, Machine Check Bank %d: Status: 0x%08x%08x, Address: 0x%08x%08x, Misc: 0x%08x%08x\n",
                 Socket, McIndex, McStatus.hi, McStatus.lo, McAddress.hi, McAddress.lo, McMisc.hi, McMisc.lo));
      //
      // Capture log information
      //
      RasData.PrevBootErr.McRegs[Entry].SocketId  = Socket;
      RasData.PrevBootErr.McRegs[Entry].McBankNum = McIndex;
      RasData.PrevBootErr.McRegs[Entry].McStatus  = McStatus;
      RasData.PrevBootErr.McRegs[Entry].McAddress = McAddress;
      RasData.PrevBootErr.McRegs[Entry].McMisc    = McMisc;

      if (!LogMemError) {
        //No Need to check again, once Error is logged
        LogMemError = CheckMemoryBootError (Socket, McIndex, *((UINT64 *) &McStatus));
      }
      Entry++;
      RasData.PrevBootErr.ValidEntries = Entry;
    } // mcbank loop
  } // skt loop

  if (LogMemError) {
     DEBUG ((DEBUG_ERROR, "PrevBootErrors: Memory MCA Error found on Previous Boot\n"));
     DEBUG ((DEBUG_ERROR, "PrevBootErrors: Setting PreviousErrorBoot Flag to skip Cold Fast Boot\n"));
     SetPreviousBootError (ERROR_ON_PREVIOUS_BOOT);
  } else {
     DEBUG ((DEBUG_ERROR, "PrevBootErrors: No Memory MCA Error Found\n"));
  }
  DEBUG ((DEBUG_ERROR, "PrevBootErrors - Valid MCA UC entries: %d\n", RasData.PrevBootErr.ValidEntries));

  SaveBootErrorRecordToHob (&RasData);
}
