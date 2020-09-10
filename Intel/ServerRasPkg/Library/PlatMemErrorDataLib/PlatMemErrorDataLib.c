/** @file
  Implementation of platform memory error reporting library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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


#include <Library/BaseMemoryLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/RasDebugLib.h>
#include <Library/CrystalRidgeLib.h>
#include <Library/AddressDecodeLib.h>
#include <Library/PlatMemErrorDataLib.h>
#include <Library/ProcessorRasLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <RcRegs.h>
#include <Library/MemMcIpRasLib.h>
#include <SiliconSetting.h>
#include <Library/MemFuncLib.h>
#include <Protocol/RasPolicy.h>
#include <Library/PlatPolicyLib.h>
#include <Library/RasMcpLib.h>
#include <Library/ErrorReportLib.h>
#include <Library/RasGlobalDataLib.h>
#include <Library/RasRegisterClearLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/SiliconWorkaroundLib.h>

POISON_TRACK      *PoisonTrack;
BOOLEAN           mIs14nmCpu;
UINT32            gM2mBankIndexIcx [] =  {0x0c, //M2M index number in EMCA_MCBANK_DESC table
                                          0x10, //
                                          0x14, //
                                          0x18  //
                                         };
/**
  Double check for poison mscod and mcacod of IFU or DFU bank

  @param [in]   McErrSig   mc bank contains errors.
  @retval       TRUE   -- poison data in IFU or DFU.
                FALSE  -- Not poison data.

**/
BOOLEAN
IsCachePoisonError (
  IN  EMCA_MC_SIGNATURE_ERR_RECORD       *McErrSig
  )
{
  IA32_MCI_STATUS_MSR_STRUCT              McBankSts;
  UINT32                                  BankType;

  if (McErrSig == NULL) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: McErrSig is NULL\n"));
      RAS_ASSERT (FALSE);
      return FALSE;
  }

  BankType = GetMcBankUnitType (McErrSig->BankIndex);

  //
  //Only cover IFU and DCU poison detection case here.
  //
  if (!((BankType == MCA_UNIT_TYPE_IFU) || (BankType == MCA_UNIT_TYPE_DCU))) {
    return FALSE;
  }

  McBankSts.Data = McErrSig->Signature.McSts;

  if (BankType == MCA_UNIT_TYPE_IFU && McBankSts.Bits.ms_code == IFU_POISON_DETECTED) {
    return TRUE;
  }

  if (BankType == MCA_UNIT_TYPE_DCU && McBankSts.Bits.ms_code == DCU_POISON_DETECTED) {
    return TRUE;
  }

  return FALSE;
}

/**
  Error data to poison record conversion.

  @param[in]  ErrorData  -- Error data should record socket/channel field.
  @param[in out]  pRecord  -- Poison record.

  @retval EFI_SUCCESS - Error data to poison record successful
  @retval EFI_INVALID_PARAMETER - At least one of the pointers is NULL or invalid
**/
EFI_STATUS
ErrorDataToPoisonRecord (
  IN EFI_PLATFORM_MEMORY_ERROR_DATA    *ErrorData,
  IN OUT POISON_RECORD * pRecord
  )
{
  if (ErrorData == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: ErrorData is NULL.Invalid param.\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (pRecord == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: pRecord is NULL.Invalid param.\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  pRecord->PhysicalAddress = ErrorData->PhysicalAddress;
  pRecord->Node = ErrorData->Node;
  pRecord->Card = ErrorData->Card;
  pRecord->ModuleRank = ErrorData->ModuleRank;
  pRecord->RankNum = ErrorData->RankNum;
  pRecord->Bank = ErrorData->Bank;
  pRecord->Row = ErrorData->Row;
  pRecord->Device = ErrorData->Device;
  pRecord->ErrorType = ErrorData->ErrorType;
  pRecord->Extended = ErrorData->Extended;
  pRecord->ValidFields = ErrorData->ValidFields;
  pRecord->RecordValid = TRUE;

  return EFI_SUCCESS;
}

/**
  Poison record to error data conversion.

  @param[in]  pRecord  -- Poison record.
  @param[in out]  ErrorData  -- Error data should record socket/channel field.

  @retval EFI_SUCCESS - Poison record to error data successful
  @retval EFI_UNSUPPORTED - not supported
  @retval EFI_INVALID_PARAMETER - At least one of the pointers is NULL or invalid
**/
EFI_STATUS
PoisonRecordToErrorData (
  IN POISON_RECORD * pRecord,
  IN OUT EFI_PLATFORM_MEMORY_ERROR_DATA    *ErrorData
  )
{
  if (pRecord == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: pRecord is NULL.Invalid param.\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (ErrorData == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: ErrorData is NULL.Invalid param.\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (!pRecord->RecordValid) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: Poison is not valid.\n"));
    return EFI_UNSUPPORTED;
  }

  ErrorData->PhysicalAddress = pRecord->PhysicalAddress;
  ErrorData->Node = pRecord->Node;
  ErrorData->Card = pRecord->Card;
  ErrorData->ModuleRank = pRecord->ModuleRank;
  ErrorData->RankNum = pRecord->RankNum;
  ErrorData->Bank = pRecord->Bank;
  ErrorData->Row  = pRecord->Row;
  ErrorData->Device  = pRecord->Device;
  ErrorData->ErrorType = pRecord->ErrorType;
  ErrorData->Extended = pRecord->Extended;
  ErrorData->ValidFields = pRecord->ValidFields;

  return EFI_SUCCESS;
}

/**
  This implementation is to look up poison record. If matched, convert poison record to error data.

  @param[in out]  ErrorData  -- Error data should record socket/channel field.

  @retval EFI_SUCCESS - poison record to error data successful
  @retval EFI_UNSUPPORTED - not supported
  @retval EFI_INVALID_PARAMETER - At least one of the pointers is NULL or invalid
**/
EFI_STATUS
EFIAPI
LookUpPoisonRecord (
  IN OUT EFI_PLATFORM_MEMORY_ERROR_DATA     *ErrorData
  )
{
  UINT32           i;
  POISON_RECORD   *pPoisonRec = NULL;

  RAS_DEBUG((LEVEL_BASIC_FLOW, "LookUpPoisonRecord\n"));

  if (ErrorData == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: ErrorData is NULL.Invalid param.\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  for (i = 0; i < MAX_POISON_RECORD; i++) {
    pPoisonRec = PoisonTrack->Overflow ? &(PoisonTrack->Records[(i + PoisonTrack->Pivot) % MAX_POISON_RECORD]) : &(PoisonTrack->Records[i]);

    if (!pPoisonRec->RecordValid) {
      return EFI_NOT_FOUND;
    }

    if ((pPoisonRec->PhysicalAddress & pPoisonRec->PhysicalAddressMask) == (ErrorData->PhysicalAddress & ErrorData->PhysicalAddressMask)) {
      PoisonRecordToErrorData(pPoisonRec, ErrorData);
      return EFI_SUCCESS;
    }

  }
  return EFI_NOT_FOUND;
}

/**
  This implementation is to set error data to poison record.

  @param[in]  ErrorData  -- Error data should record socket/channel field.

  @retval EFI_SUCCESS - Set error data to poison record successful
  @retval EFI_UNSUPPORTED - not supported
  @retval EFI_INVALID_PARAMETER - At least one of the pointers is NULL or invalid
**/
EFI_STATUS
EFIAPI
SetPoisonRecord (
  IN EFI_PLATFORM_MEMORY_ERROR_DATA     *ErrorData
  )
{
  UINT32           i;
  POISON_RECORD   *pPoisonRec = NULL;

  RAS_DEBUG((LEVEL_BASIC_FLOW, "SetPoisonRecord\n"));

  if (ErrorData == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: ErrorData is NULL. Invalid param.\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  for (i = 0; i < MAX_POISON_RECORD; i++) {
    pPoisonRec = PoisonTrack->Overflow ? &(PoisonTrack->Records[(i + PoisonTrack->Pivot) % MAX_POISON_RECORD]) : &(PoisonTrack->Records[i]);
    if (!pPoisonRec->RecordValid) { // First non-occupied
      ErrorDataToPoisonRecord (ErrorData, pPoisonRec);
      PoisonTrack->Pivot++;
      PoisonTrack->Pivot = PoisonTrack->Pivot % MAX_POISON_RECORD;
      RAS_DEBUG((LEVEL_BASIC_FLOW, "SetPoisonRecord: %d \n",i));
      RAS_DEBUG((LEVEL_BASIC_FLOW, "SetPoisonRecord: pPoisonRec->PhysicalAddress : 0x%lx\n", pPoisonRec->PhysicalAddress));
      return EFI_SUCCESS;
    }

    if ((pPoisonRec->PhysicalAddress & pPoisonRec->PhysicalAddressMask) == (ErrorData->PhysicalAddress & ErrorData->PhysicalAddressMask)) {
      //
      // Poison addr already logged
      //
      return EFI_SUCCESS;
    }
  }

  if (i == MAX_POISON_RECORD) { // Record FIFO overflow
    PoisonTrack->Overflow = TRUE;
    ErrorDataToPoisonRecord (ErrorData, pPoisonRec);
    PoisonTrack->Pivot++;
    PoisonTrack->Pivot = PoisonTrack->Pivot % MAX_POISON_RECORD;
  }

  return EFI_UNSUPPORTED;
}

/**
  This implementation is to report CAP or WrCrc error.

  @param[in]      -- Mc bank info
  @param[in out]  -- Error report data

  @retval  BitField will be set in ErrorData
**/
EFI_STATUS
EFIAPI
CheckCAPError (
  IN     EMCA_MC_SIGNATURE_ERR_RECORD       *McErrSig,
  IN OUT EFI_PLATFORM_MEMORY_ERROR_DATA     *ErrorData
  )
{
  UINT8                               Socket;
  UINT8                               Ch;
  UINT64                              ValidFlag;
  IA32_MCI_STATUS_MSR_STRUCT          McBankSts;
  UINT32                              DimmAlert;

  if (McErrSig == NULL) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: McErrSig is NULL\n"));
      RAS_ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
  }
  if (ErrorData == NULL) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: ErrorData is NULL\n"));
      RAS_ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
  }

    ValidFlag = (EFI_PLATFORM_MEMORY_NODE_VALID | EFI_PLATFORM_MEMORY_CARD_VALID);

  if ((ErrorData->ValidFields & ValidFlag ) != ValidFlag) {
     RAS_DEBUG ((LEVEL_BASIC_FLOW,"ERROR: ErrorData->ValidFields & ValidFlag != ValidFlag.\n"));
     RAS_ASSERT (FALSE);
     return EFI_INVALID_PARAMETER;
  }
  Socket = (UINT8) ErrorData->Node;
  Ch = (UINT8) ErrorData->Card;

  //
  // Update Memory Error Data for MCP
  //
  UpdateMemErrRecordForMcp (ErrorData);

  McBankSts.Data = McErrSig->Signature.McSts;

  if (McBankSts.Bits.ms_code == DDR4_CAP_ERROR  && ((McBankSts.Bits.mca_code & BIT7))) { //DDR4 CAP or WrCRC
    RAS_DEBUG((LEVEL_FUNC_FLOW, "DDR4 CAP Or Wcrc error detected!\n"));

    DimmAlert = GetDimmAlert (Socket, Ch);

    //
    // clear the CAP errors
    //
    ClearCAPErrorPerCh (Socket, Ch);

    RAS_DEBUG ((LEVEL_FUNC_FLOW, "S:0x%x  C:0x%x  Alert reg:0x%x\n", Socket,Ch, DimmAlert));
    //
    // No longer check WrCRC, since WrCRC and CAP share the same isolation logic it will all reported as
    // Parity error type, and for parity error, no longer have DIMM slot information available. Channel
    // number is enough.
    //
      if (DimmAlert & BIT0) {
        ErrorData->ModuleRank = 0;
        ErrorData->ValidFields |= EFI_PLATFORM_MEMORY_MODULE_VALID;
      } else if (DimmAlert & BIT1) {
        ErrorData->ModuleRank = 1;
        ErrorData->ValidFields |= EFI_PLATFORM_MEMORY_MODULE_VALID;
      }
      ErrorData->ErrorType = EFI_PLATFORM_MEMORY_ERROR_PARITY;
      return EFI_SUCCESS;
    // }
  } else if (McBankSts.Bits.ms_code == DDRT_CAP_ERROR && (McBankSts.Bits.mca_code & BIT7)) { //DDRT CAP/WrCRC
    RAS_DEBUG((LEVEL_FUNC_FLOW, "DDRT CAP Or WECC error detected!\n"));
    ErrorData->ModuleRank = IsDcpmm(Socket, Ch, 0) ? 0 : 1; //DCPMM should be either on DIMM 0 or DIMM 1
    ErrorData->ValidFields |= EFI_PLATFORM_MEMORY_MODULE_VALID;
    ErrorData->ErrorType = EFI_PLATFORM_MEMORY_ERROR_PARITY;
    return EFI_SUCCESS;
  } else if (McBankSts.Bits.ms_code == DDR_LINK_FAILURE && (McBankSts.Bits.mca_code & BIT7)) { //DDRT CAP/WrCRC
    RAS_DEBUG((LEVEL_FUNC_FLOW, "DDR link failure detected!\n"));
    //
    // If too many CAP error happened. It will go into link failure state.
    // So report it as a EFI_PLATFORM_MEMORY_ERROR_PARITY error
    //
    ErrorData->ErrorType = EFI_PLATFORM_MEMORY_ERROR_PARITY;
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_FOUND;
  }

}

/**
  Check if Mc bank system address is equal to retry read error log address 3 register

  @param [in]     McErrSig                 Mc bank error structure.
  @param [in out] ErrorData                Error data getting updated

  @retval     TRUE            Retry-read register is valid.
  @retval     FALSE           Retry-read register is not valid.
**/
BOOLEAN
EFIAPI
CheckMcBankAddrEqualtoRetryAddr3 (
  IN     EMCA_MC_SIGNATURE_ERR_RECORD       *McErrSig,
  IN OUT EFI_PLATFORM_MEMORY_ERROR_DATA     *ErrorData
  )
{
  IA32_MCI_STATUS_MSR_STRUCT                        McBankSts;
  UINT8                                             Mc;
  UINT8                                             ChOnMc;
  UINT8                                             Socket;
  UINT8                                             ChannelOnSocket;
  UINT64                                            SystemAddress;
  BOOLEAN                                           Valid;

  if (mIs14nmCpu == TRUE) {
    //
    //Retry_rd Address 3 register is not valid in 14nm
    //
    return FALSE;
  }

  McBankSts.Data = McErrSig->Signature.McSts;
  ChOnMc = McBankSts.Bits.mca_code & IMC_MCACOD_CHANNEL_MSK;
  Mc = GetMcBankInstance (McErrSig->BankIndex);
  ErrorData->Card = NODECHA_TO_SKTCHA (Mc, ChOnMc); // Socket Channel
  ErrorData->ValidFields |= EFI_PLATFORM_MEMORY_CARD_VALID;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "CheckMcBankAddrEqualtoRetryAddr3 entry\n"));
  Socket = (UINT8) ErrorData->Node;
  ChannelOnSocket = (UINT8) ErrorData->Card;
  SystemAddress   =  ErrorData->PhysicalAddress;
  if (SystemAddress == (UINT64)-1) {
    return FALSE;
  }
  Valid = GetRetryReadInfo (
              Socket,
              ChannelOnSocket,
              SystemAddress,
              NULL,
              NULL,
              NULL,
              NULL,
              NULL,
              NULL,
              NULL,
              NULL,
              NULL,
              NULL,
              NULL
              );
  if (!Valid) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "RETRY_RD_ERR_LOG_MCDDC_DP_REG is not valid\n"));
    return FALSE;
  }
  return TRUE;
}

/**
  Check MCA Bank Status register mcacode

  @param [in]     McErrSig                 Mc bank error structure.
  @param [in]     BankType                 MCA Bank Type
  @param [in]     ErrorData                Error report data

  @retval     TRUE
  @retval     FALSE
**/
BOOLEAN
EFIAPI
Wa_1707183940 (
  IN     EMCA_MC_SIGNATURE_ERR_RECORD       *McErrSig,
  IN     UINT32                             BankType,
  IN     EFI_PLATFORM_MEMORY_ERROR_DATA     *ErrorData
  )
{
  IA32_MCI_STATUS_MSR_STRUCT                        McBankSts;
  UINT16                                            MemErrMode;
  UINT8                                             MemErrType;
  UINT8                                             ImcOnSocket;
  PHYSICAL_ADDRESS                                  PhysicalAddress_M2m;
  UINT8                                             Socket;
  UINT8                                             Node;
  EMCA_MC_SIGNATURE_ERR_RECORD                      McSig_M2m;


  RAS_DEBUG ((LEVEL_FUNC_FLOW, "Wa_1707183940 entry\n"));

  if (!IsSiliconWorkaroundEnabled ("S1707183940")) {
    return FALSE;
  }
  if (BankType == MCA_UNIT_TYPE_M2MEM) {
    McBankSts.Data = McErrSig->Signature.McSts;
  } else {
    //When Poison enabled and if injected mirror_uce error, SRAR will be logged in DCU or IFU.
    //In some cases, CE will be logged in IMC.
    Socket = (UINT8) ErrorData->Node;
    Node = SKT_CH_TO_NODE(Socket, (UINT8) ErrorData->Card);
    ImcOnSocket = NODE_TO_MC(Node);

    RAS_DEBUG ((LEVEL_FUNC_FLOW, "Failed DIMM location info : 0x%x, 0x%x, 0x%x\n", Socket, Node, ImcOnSocket));
    if (ImcOnSocket < ARRAY_SIZE (gM2mBankIndexIcx)) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "gM2mBankIndexIcx[ImcOnSocket] : 0x%x\n", gM2mBankIndexIcx[ImcOnSocket]));
      ReadM2MemMcBankSignature (gM2mBankIndexIcx[ImcOnSocket], Socket, ImcOnSocket, &McSig_M2m);
      PhysicalAddress_M2m = GetMcAddress (&McSig_M2m);
      if (PhysicalAddress_M2m != ErrorData->PhysicalAddress) {
        RAS_DEBUG ((LEVEL_FUNC_FLOW, "The seperate physical address is : 0x%x, 0x%x\n", ErrorData->PhysicalAddress, PhysicalAddress_M2m));
        return FALSE;
      }
      McBankSts.Data = McSig_M2m.Signature.McSts;
    }
  }
  if (McBankSts.Bits.val == 1) {
   MemErrMode = (UINT16) (McBankSts.Bits.mca_code & IMC_MCACOD_ERRMODE_MSK) >> 7; // bits[15:7]
   MemErrType = (UINT8) (McBankSts.Bits.mca_code & IMC_MCACOD_ERRTYPE_MSK) >> 4; // bits[6:4]
   //MCACOD:
     //bits[15:7] = 0000 0010 0: Near-Memory Cache controller error. Follow below encodings -> for 2lm
     //bits[15:7] = 0000 0000 1: Last level memory controller error. Follow below encodings -> for mirroring
     //bits[6:4]  =         001: Memory Read Error (MemRd*, MemSpecRd*, MemInv*).
     //bits[3:0]  =              Physical Channel Number (0-2). So bits[3:2] always 0 in this implementation. => for 2lm should tell NM channle and for mirroring should tell 1LM mirroring channel
   RAS_DEBUG ((LEVEL_FUNC_FLOW, "Memory Error : 0x%x, 0x%x\n", MemErrType, MemErrMode));
   if ((MemErrType == 0x01) && (MemErrMode == 0x04 || MemErrMode == 0x01)) {
     return TRUE;
   }
  }
  return FALSE;
}

/**
  This implementation is to check memory controller retry.

  @param[in out] ErrorData  -- Error data should record socket/channel field.
  @retval  none

**/
VOID
EFIAPI
CheckDDR4Error (
  IN OUT EFI_PLATFORM_MEMORY_ERROR_DATA     *ErrorData
  )
{
  UINT8                                       Socket;
  UINT8                                       Ch;
  UINT8                                       Node;
  UINT8                                       NodeCh;
  UINT64                                      ValidFlag;
  FAILED_DIMM_INFO                            FailedDimmInfo;
  EFI_STATUS                                  Status;

  if (ErrorData == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW,"ERROR: ErrorData is NULL.\n"));
    RAS_ASSERT (FALSE);
    return;
  }

  ValidFlag = (EFI_PLATFORM_MEMORY_NODE_VALID | EFI_PLATFORM_MEMORY_CARD_VALID);

  if ((ErrorData->ValidFields & ValidFlag) != ValidFlag) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW,"ERROR: ErrorData->ValidFields & ValidFlag != ValidFlag.\n"));
    RAS_ASSERT (FALSE);
    return;
  }

  Socket = (UINT8) ErrorData->Node;
  Ch = (UINT8) ErrorData->Card;
  Node = SKT_CH_TO_NODE (Socket, Ch);
  NodeCh = SKTCH_TO_NODECH (Ch);

  Status = GetChannelErrorInfo (Node, NodeCh, ErrorData->PhysicalAddress, &FailedDimmInfo);

  if (EFI_ERROR (Status)) {
    ErrorData->ValidFields  &= ~(UINT64)((EFI_PLATFORM_MEMORY_CARD_VALID | EFI_PLATFORM_MEMORY_MODULE_VALID | EFI_PLATFORM_MEMORY_ERROR_RANK_NUM_VALID |
                        EFI_PLATFORM_MEMORY_BANK_VALID | EFI_PLATFORM_MEMORY_ROW_VALID | EFI_PLATFORM_MEMORY_COLUMN_VALID | EFI_PLATFORM_MEMORY_DEVICE_VALID)); // Clear out valid flag.
    return;
  }

  ErrorData->ModuleRank   = FailedDimmInfo.FailedDimm;
  ErrorData->RankNum = FailedDimmInfo.FailedRank;
  ErrorData->Column = (UINT16) FailedDimmInfo.FailedCol;
  ErrorData->Bank = FailedDimmInfo.FailedBank + (FailedDimmInfo.FailedBG << 2);  //retry_rd_err_log_address1.bank : Bank ID for last retry. Bottom two bits are Bank Group, top two bits are Bank Address
  ErrorData->Row  = (UINT16) FailedDimmInfo.FailedRow;
  ErrorData->Device  = (UINT16) FailedDimmInfo.FailedDev;

  ErrorData->ValidFields |= (EFI_PLATFORM_MEMORY_MODULE_VALID | EFI_PLATFORM_MEMORY_ERROR_RANK_NUM_VALID |
                      EFI_PLATFORM_MEMORY_BANK_VALID | EFI_PLATFORM_MEMORY_ROW_VALID | EFI_PLATFORM_MEMORY_COLUMN_VALID | EFI_PLATFORM_MEMORY_DEVICE_VALID);

  if (FailedDimmInfo.FailedRow & (BIT16 | BIT17)) {
    ErrorData->Extended |= (FailedDimmInfo.FailedRow & (BIT16 | BIT17)) >> 16;
    ErrorData->ValidFields |= EFI_PLATFORM_MEMORY_ERROR_EXTENDED_ROW_BIT_16_17_VALID;
  }

  SetPoisonRecord (ErrorData);
  RecordPositionToClearRetryRegister(Socket, Ch);

  return;
}


/**
  This implementation is to check DDRT error through mailbox.

  @param [in]     McErrSig                Machine check error status
  @param [in]     FailedDimmInfo          Failed Dimm data structure.
  @param [in]     ErrorDpa                Error Dpa get from address translation.
  @param [in out] ErrorData               Error data should record socket/channel field.
  @retval    none

**/
VOID
EFIAPI
CheckDDRTError (
  IN EMCA_MC_SIGNATURE_ERR_RECORD           *McErrSig,
  IN FAILED_DIMM_INFO                       *FailedDimmInfo,
  IN UINT64                                 ErrorDpa,
  IN OUT EFI_PLATFORM_MEMORY_ERROR_DATA     *ErrorData
  )
{
  EFI_STATUS                      Status;
  UINT8                           Socket;
  UINT8                           Ch;
  UINT8                           Dimm;
  UINT64                          ValidFlag;
  RAS_POLICY                     *PolicyData;

  MEDIA_ERR_LOG                   *MediaLogPtr;
  ERR_LOG_DATA                    ErrLogData;
  UINT8                           i;

  PolicyData = GetRasPolicy ();

  if (FailedDimmInfo == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW,"ERROR: FailedDimmInfo is NULL.\n"));
    RAS_ASSERT (FALSE);
    return;
  }

  if (ErrorData == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW,"ERROR: ErrorData is NULL.\n"));
    RAS_ASSERT (FALSE);
    return;
  }

  ValidFlag = EFI_PLATFORM_MEMORY_NODE_VALID | EFI_PLATFORM_MEMORY_PHY_ADDRESS_VALID;

  if ((ErrorData->ValidFields & ValidFlag) != ValidFlag) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW,"ERROR: ErrorData->ValidFields & ValidFlag != ValidFlag.\n"));
    return;
  }

  MediaLogPtr = NULL;

  Socket = FailedDimmInfo->Node;
  Ch = FailedDimmInfo->NodeCh;
  Dimm = FailedDimmInfo->FailedDimm;

  //
  // Get High Priority Log
  //
  Status = FnvGetLatestErrorLog (Socket, Ch, Dimm, HIGH_LEVEL_LOG, MEDIA_LOG_REQ, MAX_MEDIA_LOGS, &ErrLogData);
  if (Status == EFI_NOT_READY) {
    //
    // If NVDIMM was not ready retry once
    //
    Status = FnvGetLatestErrorLog (Socket, Ch, Dimm, HIGH_LEVEL_LOG, MEDIA_LOG_REQ, MAX_MEDIA_LOGS, &ErrLogData);
  }
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "CheckDDRTError: Failed to GetLatestErrorLog. Socket = 0x%x, Ch = 0x%x, Dimm = 0x%x, LogLevel = 0x%x (%r)\n",
                   Socket, Ch, Dimm, HIGH_LEVEL_LOG, Status));
    return;
  }

  for (i = 0; i < ErrLogData.MediaLogData.ReturnCount; i++) {
    if (ErrLogData.MediaLogData.MediaLogs[i].Dpa == ErrorDpa) {
      MediaLogPtr = &ErrLogData.MediaLogData.MediaLogs[i];
      break;
    }
  }

  if (!MediaLogPtr) { //No match in high priority. Get low priority log
    Status = FnvGetLatestErrorLog (Socket, Ch, Dimm, LOW_LEVEL_LOG, MEDIA_LOG_REQ, MAX_MEDIA_LOGS, &ErrLogData);
    if (EFI_ERROR (Status)) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "CheckDDRTError: Failed to GetLatestErrorLog. Socket = 0x%x, Ch = 0x%x, Dimm = 0x%x, LogLevel = 0x%x\n",
                    Socket, Ch, Dimm, LOW_LEVEL_LOG));
      return;
    }
    for (i = 0; i < ErrLogData.MediaLogData.ReturnCount; i++) {
      if (ErrLogData.MediaLogData.MediaLogs[i].Dpa == ErrorDpa) {
        MediaLogPtr = &ErrLogData.MediaLogData.MediaLogs[i];
        break;
      }
    }
  }

  if (MediaLogPtr) { //Insert DDRT Error Log here

    //
    // Do UNC Poison explicitly for Read Transactions
    //
    if ((PolicyData->ImcPolicy.DcpmmUncPoison) && (MediaLogPtr->ErrorType == FNV_ET_UNCORRECTABLE)
        && ((MediaLogPtr->TransactionType == FNV_TT_2LM_READ) ||(MediaLogPtr->TransactionType == FNV_TT_PM_READ))) {

      RAS_DEBUG ((LEVEL_FUNC_FLOW, "CheckDDRTError: MediaLogPtr->ErrorType = type 0x%X\n", MediaLogPtr->ErrorType));
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "CheckDDRTError: MediaLogPtr->TransactionType = type 0x%X\n", MediaLogPtr->TransactionType));
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "CheckDDRTError: Calling AsmWriteCacheLine on SPA 0x%lx\n", McErrSig->Signature.McAddr));

      MemoryFence ();
      *(VOLATILE UINT64 *)((UINTN)McErrSig->Signature.McAddr) = 0;
      MemoryFence ();
      AsmFlushCacheLine ((VOID*)(UINTN)McErrSig->Signature.McAddr);
    }

    if (MediaLogPtr->TransactionType == FNV_TT_PATROL_SCRUB) {
      ErrorData->ErrorType = EFI_PLATFORM_MEMORY_ERROR_SCRUB_UNCORRECTED;
    }
    ErrorData->Node          = FailedDimmInfo->Node;
    ErrorData->Card          = Ch;
    ErrorData->ModuleRank    = Dimm;
    ErrorData->RankNum       = FailedDimmInfo->FailedRank;
    ErrorData->ValidFields  |= (UINT64)(EFI_PLATFORM_MEMORY_CARD_VALID | EFI_PLATFORM_MEMORY_MODULE_VALID | EFI_PLATFORM_MEMORY_ERROR_RANK_NUM_VALID);
  }

  return;
}


VOID
EFIAPI
FillSmbiosItems (
  IN OUT EFI_PLATFORM_MEMORY_ERROR_DATA     *ErrorData
  )
{
  MEM_TOPOLOGY                          *MemTopology;

  //module SMBIOS
  if ((ErrorData->ValidFields & EFI_PLATFORM_MEMORY_NODE_VALID) != 0 &&
      (ErrorData->ValidFields & EFI_PLATFORM_MEMORY_CARD_VALID) != 0 &&
      (ErrorData->ValidFields & EFI_PLATFORM_MEMORY_MODULE_VALID) != 0) {

    if (ErrorData->Node >= MAX_SOCKET) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "ERROR: ErrorData->Node >= MAX_SOCKET\n"));
      RAS_ASSERT (FALSE);
      return;
    }
    if (ErrorData->Card >= IMC_MAX_CH) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "ERROR: ErrorData->Card >= IMC_MAX_CH\n"));
      RAS_ASSERT (FALSE);
      return;
    }
    if (ErrorData->ModuleRank >= MAX_DIMM) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "ERROR: ErrorData->ModuleRank >= MAX_DIMM\n"));
      RAS_ASSERT (FALSE);
      return;
    }

    MemTopology = GetMemTopology ();
    ErrorData->CardHandle = MemTopology->Socket[ErrorData->Node].SmbiosType16Handle[((ErrorData->Card) / MAX_MC_CH)]; // Type 16 handle
    ErrorData->ModuleHandle =
      MemTopology->Socket[ErrorData->Node].ChannelInfo[ErrorData->Card].DimmInfo[ErrorData->ModuleRank].SmbiosType17Handle; // Type 17 handle

    ErrorData->ValidFields |= EFI_PLATFORM_MEMORY_ERROR_CARD_HANDLE_VALID | EFI_PLATFORM_MEMORY_ERROR_MODULE_HANDLE_VALID;
  }
}

/**
  Checks for NM Hit/Miss clean case. In such cases, we just log an error and don't need to report address to OS.

  @param [in]     McErrSig                 Mc bank error structure.
  @param [in out] ErrorData                Error data getting updated

**/
VOID
CheckNMCleanCase (
  IN     EMCA_MC_SIGNATURE_ERR_RECORD           *McErrSig,
  IN OUT EFI_PLATFORM_MEMORY_ERROR_DATA         *ErrorData
  )
{
  UINT32         McType;
  MEM_TOPOLOGY   *MemTopology;

  McType = GetMcBankUnitType (McErrSig->BankIndex);
  MemTopology = GetMemTopology ();

  if (McType != MCA_UNIT_TYPE_M2MEM) { //Only look for M2M MC Bank
    return;
  }
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "CheckNMCleanCase: McErrSig->Signature.McSts:0x%lx;\n", McErrSig->Signature.McSts));
  if (!(McErrSig->Signature.McSts & BIT59)) { //BIT59 is misc_v
    return;
  }
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "CheckNMCleanCase: McErrSig->Signature.McMisc:0x%lx;\n", McErrSig->Signature.McMisc));

  if (MemTopology == NULL) {
    return;
  }
  //
  // Look for Hit/Miss - Clean signature. ErrorType(Misc[20:11]) = 0x82; BIT8 & BIT9 are reserved
  //
  if (((McErrSig->Signature.McMisc >> 11) & 0xff) == 0x82) {
    //
    // set BIT37 to update the MC status
    // Advanced RAS part supports writable(with 1) MC bank
    //
    if (MemTopology->SystemRasType == ADVANCED_RAS) {
      McErrSig->Signature.McSts |= (UINT64)(MCA_FIRMWARE_UPDATED_BIT);
      WriteMcBank (McErrSig->BankIndex, McErrSig);
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "CheckNMCleanCase: McSts:0x%lx\n", McErrSig->Signature.McSts));
    }
    ErrorData->ValidFields &= (UINT64)~(EFI_PLATFORM_MEMORY_PHY_ADDRESS_VALID);  //Unset Physical address valid bit
    ErrorData->PhysicalAddress = 0;

    if (MemTopology->SystemRasType == ADVANCED_RAS) {
      McErrSig->Signature.McSts &= (UINT64)~(MCA_STATUS_ADDRV_BIT); // clear ADDRV bit, so that OS does not report the address
      WriteMcBank (McErrSig->BankIndex, McErrSig);
    }
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "CheckNMCleanCase: ErrorData->ValidFields:0x%lx; ErrorData->PhysicalAddress:0x%lx; McSts:0x%lx\n", ErrorData->ValidFields, ErrorData->PhysicalAddress, McErrSig->Signature.McSts));
  }
}

/**
  This implementation is to decode system address, then check the address is valid or not.

  @param [in out] FailedDimmInfo                Failed Dimm data structure.
  @param [in out] ErrorData                     Error data structure.
  @param [in out] ErrorDpa                      Error Dpa get from address translation.

  @retval  -- DIMM_TYPE_DDR4  DDR4 address exist,
           -- DIMM_TYPE_DDRT  DDRT address exist,
           -- DIMM_TYPE_UNKNOWN  address not valid.

**/
UINT32
CheckValidAddress (
  IN OUT FAILED_DIMM_INFO                   *FailedDimmInfo,
  IN OUT EFI_PLATFORM_MEMORY_ERROR_DATA     *ErrorData,
  IN OUT UINT64                             *ErrorDpa
  )
{
  EFI_STATUS                        Status;
  TRANSLATED_ADDRESS                XlatedAddr;

  if (FailedDimmInfo == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW,"ERROR: FailedDimmInfo is NULL.\n"));
    RAS_ASSERT (FALSE);
    return DIMM_TYPE_UNKNOWN;
  }

  if (ErrorData == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW,"ERROR: ErrorData is NULL.\n"));
    RAS_ASSERT (FALSE);
    return DIMM_TYPE_UNKNOWN;
  }

  if ((ErrorData->ValidFields & EFI_PLATFORM_MEMORY_PHY_ADDRESS_VALID ) != EFI_PLATFORM_MEMORY_PHY_ADDRESS_VALID) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW,"ERROR: (ErrorData->ValidFields & EFI_PLATFORM_MEMORY_PHY_ADDRESS_VALID) is not equal to  EFI_PLATFORM_MEMORY_PHY_ADDRESS_VALID.\n"));
    RAS_ASSERT (FALSE);
    return DIMM_TYPE_UNKNOWN;
  }


  ZeroMem (&XlatedAddr, sizeof (TRANSLATED_ADDRESS));

  //
  // Do FAT
  //
  Status = SystemAddressToDimmAddress ((UINTN) ErrorData->PhysicalAddress, &XlatedAddr);
  if (EFI_ERROR (Status)) {
    return DIMM_TYPE_UNKNOWN;
  }

  switch (XlatedAddr.MemType) {
    case MemType1lmDdr:
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "ValidMcAddress - XlatedAddr.MemType == MemType1lmDdr!\n"));
      ErrorData->Node = XlatedAddr.SocketId;
      ErrorData->Card = NODECHA_TO_SKTCHA (XlatedAddr.MemoryControllerId, XlatedAddr.ChannelId);
      ErrorData->PhysicalAddressMask = CACHELINE_MASK;
      ErrorData->ValidFields |=  EFI_PLATFORM_MEMORY_CARD_VALID;
      return DIMM_TYPE_DDR4;
    case MemType1lmAppDirect:
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "ValidMcAddress - XlatedAddr.MemType == MemType1lmAppDirect!\n"));
      FailedDimmInfo->Node = XlatedAddr.SocketId;
      FailedDimmInfo->Node = (UINT8) ErrorData->Node;
      FailedDimmInfo->NodeCh = NODECHA_TO_SKTCHA (XlatedAddr.MemoryControllerId, XlatedAddr.ChannelId);
      FailedDimmInfo->FailedDimm = XlatedAddr.DimmSlot;
      FailedDimmInfo->FailedRank = XlatedAddr.PhysicalRankId;
      *ErrorDpa = XlatedAddr.DPA;
      return DIMM_TYPE_DDRT;
    case MemType2lmDdrCacheMemoryMode:
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "ValidMcAddress - XlatedAddr.MemType == MemType2lmDdrCacheMemoryMode!\n"));
      ErrorData->Node = XlatedAddr.SocketId;
      ErrorData->Card = NODECHA_TO_SKTCHA (XlatedAddr.MemoryControllerId, XlatedAddr.NmChannelId);
      ErrorData->PhysicalAddressMask = CACHELINE_MASK_CR;
      ErrorData->ValidFields |=  EFI_PLATFORM_MEMORY_CARD_VALID;
      FailedDimmInfo->Node = XlatedAddr.SocketId;
      FailedDimmInfo->NodeCh = NODECHA_TO_SKTCHA (XlatedAddr.MemoryControllerId, XlatedAddr.ChannelId);
      FailedDimmInfo->FailedDimm = XlatedAddr.DimmSlot;
      FailedDimmInfo->FailedRank = XlatedAddr.PhysicalRankId;
      *ErrorDpa = XlatedAddr.DPA;
      return DIMM_TYPE_DDR4 | DIMM_TYPE_DDRT;
    default:
      return DIMM_TYPE_UNKNOWN;
  }
}

/**
  Create the processor error record
  following bank invoke this procedure:
    IFU/DFU bank with poison enable
    M2M bank
    IMC bank

  @param[in] Socket     The socket that has the error
  @param[in] McErrSig   Machine check error status
  @param[in] PostInvoke     TRUE for POST phase. FALSE for SMM phase.
  @param[in out] ErrorData  Standard Memory error data

**/
EFI_STATUS
EFIAPI
FillPlatformMemoryErrorData (
  IN     UINT8                              Socket,
  IN     EMCA_MC_SIGNATURE_ERR_RECORD       *McErrSig,
  IN     BOOLEAN                            PostInvoke,
  IN OUT EFI_PLATFORM_MEMORY_ERROR_DATA     *ErrorData
  )
{
  PHYSICAL_ADDRESS                       PhysicalAddress;
  IA32_MCI_STATUS_MSR_STRUCT             McBankSts;
  UINT32                                 BankType;
  UINT8                                  Mc;
  UINT8                                  ChOnMc;
  MCE_CLASS                              MceClass;
  FAILED_DIMM_INFO                       FailedDimmInfo;
  UINT32                                 ValidAddrDimmType;
  UINT64                                 ErrorDpa = 0;
  UINT8                                  Uc;
  EFI_STATUS                             Status;

  ZeroMem (ErrorData, sizeof (EFI_PLATFORM_MEMORY_ERROR_DATA));
  ZeroMem (&FailedDimmInfo, sizeof (FAILED_DIMM_INFO));

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "FillPlatformMemoryErrorData start!\n"));

  ErrorData->Node = Socket;
  ErrorData->ValidFields = EFI_PLATFORM_MEMORY_ERROR_STATUS_VALID;
  ErrorData->ValidFields |= EFI_PLATFORM_MEMORY_NODE_VALID;

  McBankSts.Data = McErrSig->Signature.McSts;
  Uc = (UINT8) McBankSts.Bits.uc;

  MceClass = GetMceClass (McErrSig);
  //
  // CORRECTED and UCNA error are mapped to same error severity(EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED),
  // In order to differentiate CORRECTED and UCNA error for PPR usage. Different ErrorType is reported here.
  //
  if (MceClass == CORRECTED) {
    ErrorData->ErrorType = EFI_PLATFORM_MEMORY_ERROR_SINGLEBIT_ECC;
  } else if (MceClass == UCNA) {
    ErrorData->ErrorType = EFI_PLATFORM_MEMORY_ERROR_SINGLESYMBOLS_CHIPKILL;
  } else { // SRAO, SRAR, FATAL case
    ErrorData->ErrorType = EFI_PLATFORM_MEMORY_ERROR_MLTIBIT_ECC;
  }
  ErrorData->ValidFields |= EFI_PLATFORM_MEMORY_ERROR_TYPE_VALID;

  PhysicalAddress = GetMcAddress (McErrSig);
  if (PhysicalAddress != MAX_ADDRESS) {
    ErrorData->PhysicalAddress = PhysicalAddress;
    ErrorData->ValidFields |= EFI_PLATFORM_MEMORY_PHY_ADDRESS_VALID;
    RAS_DEBUG ((LEVEL_REG, "Address:0x%lx\n", ErrorData->PhysicalAddress));
  }

  if (IsCxlAddress ((UINTN) PhysicalAddress, (UINT8 *) &ErrorData->Node)) {
    RAS_DEBUG ((LEVEL_REG, "UC error is come from IAL device.\n"));
    RAS_DEBUG ((LEVEL_REG, "Report RAS Non-standard errors. Error type: RasErrorTypeCxlError\n"));
    ReportCxlErrorLog (ErrorData->Node, (UINTN) PhysicalAddress);
    return EFI_SUCCESS;
  }

  BankType = GetMcBankUnitType (McErrSig->BankIndex);
  if (BankType == MCA_UNIT_TYPE_ERROR) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "BankType == MCA_UNIT_TYPE_ERROR\n"));
    RAS_ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "FillPlatformMemoryErrorData: BankType:0x%x\n", BankType));
  if ((BankType == MCA_UNIT_TYPE_IFU) || (BankType == MCA_UNIT_TYPE_DCU)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "Error Source is IFU or DCU.\n"));
    if ((ErrorData->ValidFields & EFI_PLATFORM_MEMORY_PHY_ADDRESS_VALID) && IsCachePoisonError (McErrSig)) { //Only process poison detected case for cache bank
      ValidAddrDimmType = CheckValidAddress (&FailedDimmInfo, ErrorData, &ErrorDpa);

      if (!PostInvoke) { // For POST phase, we don't do the further check, just report error based on address translation result.
        Status = LookUpPoisonRecord (ErrorData);
        if (Status == EFI_SUCCESS) {
          RAS_DEBUG((LEVEL_BASIC_FLOW, "Found poison record\n"));
        } else {
          RAS_DEBUG((LEVEL_BASIC_FLOW, "Poison record not found\n"));

          if ((ValidAddrDimmType & DIMM_TYPE_DDR4) != 0) {
            if (!Wa_1707183940 (McErrSig, BankType, ErrorData)) {
              CheckDDR4Error (ErrorData);
            }
          }
          if ((ValidAddrDimmType & DIMM_TYPE_DDRT) != 0) {
            CheckDDRTError (McErrSig, &FailedDimmInfo, ErrorDpa, ErrorData);
          }
        }
      }
    } else {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "Address not valid or poison not detectd.\n"));
      return EFI_NOT_FOUND;
    }
  } else if ((BankType == MCA_UNIT_TYPE_M2MEM) || (BankType == MCA_UNIT_TYPE_IMC)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "Error Source is M2M or IMC(MCCHAN)BankType:0x%x\n", BankType));
    if (ErrorData->ValidFields & EFI_PLATFORM_MEMORY_PHY_ADDRESS_VALID) {
      if (BankType == MCA_UNIT_TYPE_IMC) {
        if (McBankSts.Bits.ms_code == MC_UC_PATROL_SCRUB) {
          ErrorData->ErrorType = EFI_PLATFORM_MEMORY_ERROR_SCRUB_UNCORRECTED;
        } else if (McBankSts.Bits.ms_code == MC_CE_PATROL_SCRUB) {
          ErrorData->ErrorType = EFI_PLATFORM_MEMORY_ERROR_SCRUB_CORRECTED;
        }
      }
    if (!CheckMcBankAddrEqualtoRetryAddr3 (McErrSig, ErrorData)) {
      ValidAddrDimmType = CheckValidAddress (&FailedDimmInfo, ErrorData, &ErrorDpa);
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "FillPlatformMemoryErrorData: ValidAddrDimmType:0x%x; PostInvoke:0x%x:\n", ValidAddrDimmType, PostInvoke));
      if (!PostInvoke) {
        if ((ValidAddrDimmType & DIMM_TYPE_DDR4) != 0) {
          CheckNMCleanCase (McErrSig, ErrorData);
          if (!Wa_1707183940 (McErrSig, BankType, ErrorData)) {
            CheckDDR4Error (ErrorData);
          }
          if (Wa_1707183940 (McErrSig, BankType, ErrorData)) {
           RecordPositionToClearRetryRegister(Socket, (UINT8) ErrorData->Card);
          }
        }
        if ((ValidAddrDimmType & DIMM_TYPE_DDRT) != 0) {
          CheckDDRTError (McErrSig, &FailedDimmInfo, ErrorDpa, ErrorData);
        }
      }
    }else{
        ErrorData->PhysicalAddressMask = CACHELINE_MASK;
        if (!PostInvoke) {
          RAS_DEBUG ((LEVEL_FUNC_FLOW, "McBank system address is equal to retry read error log address 3 \n"));
          CheckNMCleanCase (McErrSig, ErrorData);
          CheckDDR4Error (ErrorData);
        }
      }
    } else {// No valid address. Check for CAP or WrCRC error.
      ChOnMc = McBankSts.Bits.mca_code & IMC_MCACOD_CHANNEL_MSK;

      Mc = GetMcBankInstance (McErrSig->BankIndex);

      ErrorData->Card = NODECHA_TO_SKTCHA (Mc, ChOnMc); // Socket Channel
      ErrorData->ValidFields |= EFI_PLATFORM_MEMORY_CARD_VALID;

      return CheckCAPError (McErrSig, ErrorData);
    }
  } else {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "BankType unsupported\n"));
    return EFI_UNSUPPORTED;
  }

  if ((ErrorData->ValidFields) & EFI_PLATFORM_MEMORY_ERROR_RANK_NUM_VALID) {
    ErrorData->RankNum = CHRANK_TO_DIMMRANK (ErrorData->RankNum);
  }
  FillSmbiosItems (ErrorData);

  //
  // Update Memory Error Data for MCP
  //
  UpdateMemErrRecordForMcp (ErrorData);

  return EFI_SUCCESS;
}


/**
  This implementation is to compose correctable memory data for legacy memory driver.

  @param [in]  SubType  -- memory error type
  @param [in]  DimmErrorInfo  -- Dimm Error information from memory driver
  @param [in,out] ErrorData -- Error data produced for error report.

  @retval none
**/
VOID
EFIAPI
FillCorrectablMemoryErrorData (
  IN        UINT8                               SubType,
  IN        DIMM_ERROR_INFO                    *DimmErrorInfo,
  IN OUT    EFI_PLATFORM_MEMORY_ERROR_DATA     *ErrorData
  )
{
  UINT8               Socket;
  UINT8               Mc;
  //MCI_STATUS_SHADOW_N1_M2MEM_MAIN_STRUCT  Mc5ShadowStatus;
  //MCI_ADDR_SHADOW0_M2MEM_MAIN_STRUCT      Mc5Shadow0Addr;
  //MCI_ADDR_SHADOW1_M2MEM_MAIN_STRUCT      Mc5Shadow1Addr;

  ZeroMem (ErrorData, sizeof (EFI_PLATFORM_MEMORY_ERROR_DATA));

  Socket = DimmErrorInfo->Socket;
  Mc = DimmErrorInfo->SktCh / MAX_MC_CH ;

  RAS_DEBUG ((LEVEL_REG, "LogCorrectedMemError : SubType = 0x%x, socket = 0x%x, channel = 0x%x, Rank = 0x%x Dimm = 0x%x\n",
              SubType, Socket, DimmErrorInfo->SktCh, DimmErrorInfo->Rank, DimmErrorInfo->Dimm));

  ErrorData->ValidFields = EFI_PLATFORM_MEMORY_ERROR_STATUS_VALID;
  ErrorData->Node = Socket;
  ErrorData->Card = DimmErrorInfo->SktCh;

  ErrorData->ValidFields |= EFI_PLATFORM_MEMORY_NODE_VALID | EFI_PLATFORM_MEMORY_CARD_VALID;

  switch (SubType) {
    case MEM_ECC_ERROR:
      ErrorData->ModuleRank = DimmErrorInfo->Dimm;
      ErrorData->RankNum = DimmErrorInfo->Rank;
      ErrorData->Bank = DimmErrorInfo->Bank;
      ErrorData->Device = DimmErrorInfo->Dev;
      ErrorData->Row = (UINT16) DimmErrorInfo->Row;
      ErrorData->Column = (UINT16) DimmErrorInfo->Col;
      ErrorData->ErrorType = EFI_PLATFORM_MEMORY_ERROR_SINGLEBIT_ECC;
      ErrorData->ValidFields |= EFI_PLATFORM_MEMORY_MODULE_VALID | EFI_PLATFORM_MEMORY_ERROR_RANK_NUM_VALID
        | EFI_PLATFORM_MEMORY_BANK_VALID | EFI_PLATFORM_MEMORY_DEVICE_VALID
        | EFI_PLATFORM_MEMORY_ROW_VALID | EFI_PLATFORM_MEMORY_COLUMN_VALID
        | EFI_PLATFORM_MEMORY_ERROR_TYPE_VALID;

      if (DimmErrorInfo->Row & (BIT16 | BIT17)) {
        ErrorData->Extended |= (DimmErrorInfo->Row & (BIT16 | BIT17)) >> 16;
        ErrorData->ValidFields |= EFI_PLATFORM_MEMORY_ERROR_EXTENDED_ROW_BIT_16_17_VALID;
      }
      break;

    case MEM_MIRROR_FAILOVER:
      ErrorData->ErrorType = EFI_PLATFORM_MEMORY_ERROR_MIRROR_FAILED;
      ErrorData->ModuleRank = DimmErrorInfo->Dimm;
      ErrorData->RankNum = DimmErrorInfo->Rank;
      ErrorData->ValidFields |= EFI_PLATFORM_MEMORY_MODULE_VALID | EFI_PLATFORM_MEMORY_ERROR_RANK_NUM_VALID | EFI_PLATFORM_MEMORY_ERROR_TYPE_VALID;
      break;

      // case CAP_ERROR:
    default:
      break;
  }

/*
  if (SubType == MEM_MIRROR_FAILOVER) {
      Mc5ShadowStatus.Data = ReadCpuCsr (Socket, Mc, MCI_STATUS_SHADOW_N1_M2MEM_MAIN_REG);
      if (Mc5ShadowStatus.Bits.valid && Mc5ShadowStatus.Bits.addrv) {
        Mc5Shadow0Addr.Data = ReadCpuCsr (Socket, Mc, MCI_ADDR_SHADOW0_M2MEM_MAIN_REG);
        Mc5Shadow1Addr.Data = ReadCpuCsr (Socket, Mc, MCI_ADDR_SHADOW1_M2MEM_MAIN_REG);
        ErrorData->PhysicalAddress = ((UINT64)Mc5Shadow1Addr.Data << 32) | (Mc5Shadow0Addr.Data);
        ErrorData->PhysicalAddress &= (0x3fffffffffc0); // 46bit address space, bits 45:6.
        ErrorData->ValidFields |= EFI_PLATFORM_MEMORY_PHY_ADDRESS_VALID;

        ValidMcAddress(ErrorData->PhysicalAddress ,ErrorData);
      }
  }
*/

  if ((ErrorData->ValidFields) & EFI_PLATFORM_MEMORY_ERROR_RANK_NUM_VALID) {
    ErrorData->RankNum = CHRANK_TO_DIMMRANK (ErrorData->RankNum);
  }

  FillSmbiosItems (ErrorData);

  //
  // Update Memory Error Data for MCP
  //
  UpdateMemErrRecordForMcp (ErrorData);

}

/**
  Creates the memory error record based on the inputs.

  This function will only handle correctable errors.

  @param[in]  Type      - The error type
  @param[in]  SubType   - The error sub type
  @param[in]  Node      - The node the error was detected on
  @param[in]  Channel   - The channel the error was detected on
  @param[in]  Rank      - The rank the error was detected on
  @param[in]  Dimm      - The dimm the error was detected on
  @param[in]  McaCode   - The MCA code for the error

  @retval None
**/
VOID
FillFnvErrorData (
  IN  UINT8                                           Socket,
  IN  UINT8                                           Ch,
  IN  UINT8                                           Dimm,
  IN  UINT8                                           ErrorType,
  IN  UINT64                         CONST  *  CONST  Dpa,
  IN  BOOLEAN                                         ReportSocketOnly,
  OUT EFI_PLATFORM_MEMORY_ERROR_DATA        *  CONST  ErrorData
  )
{
  EFI_STATUS          Status;
  TRANSLATED_ADDRESS  TranslatedAddress;

  ZeroMem (ErrorData, sizeof (EFI_PLATFORM_MEMORY_ERROR_DATA));

  ErrorData->ValidFields = EFI_PLATFORM_MEMORY_ERROR_STATUS_VALID;
  ErrorData->Node = Socket;
  ErrorData->Card = Ch;
  ErrorData->ModuleRank = Dimm;
  ErrorData->ErrorType = ErrorType;
  ErrorData->ValidFields = EFI_PLATFORM_MEMORY_NODE_VALID;
  if (ReportSocketOnly == FALSE) {
    ErrorData->ValidFields = EFI_PLATFORM_MEMORY_ERROR_STATUS_VALID | EFI_PLATFORM_MEMORY_NODE_VALID | EFI_PLATFORM_MEMORY_MODULE_VALID  |
                             EFI_PLATFORM_MEMORY_CARD_VALID | EFI_PLATFORM_MEMORY_ERROR_TYPE_VALID;
  }

  if (Dpa == NULL) {
    return;
  }

  ZeroMem (&TranslatedAddress, sizeof(TRANSLATED_ADDRESS));
  TranslatedAddress.SocketId           = Socket;
  TranslatedAddress.MemoryControllerId = Ch / MAX_MC_CH;
  TranslatedAddress.ChannelId          = Ch % MAX_MC_CH;
  TranslatedAddress.DimmSlot           = Dimm;
  TranslatedAddress.PhysicalRankId     = 0;
  TranslatedAddress.DPA                = *Dpa;

  Status = DimmAddressToSystemAddress(&TranslatedAddress);

  if (EFI_ERROR(Status)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "RAS: HandleNvDimmHighPriorityError: S:0x%x.C:0x%x.D:0x%x: DPA:0x%xl failed to perform DPA to SPA %r\n", Socket, Ch, Dimm, *Dpa, Status));
    return;
  }


  ErrorData->PhysicalAddress = TranslatedAddress.SystemAddress;
  ErrorData->PhysicalAddressMask = 0xFFFFFFFFFFFFFF00; //DCPMM poison granularity is 256B.
  ErrorData->ValidFields |= (UINT64)(EFI_PLATFORM_MEMORY_PHY_ADDRESS_VALID | EFI_PLATFORM_MEMORY_PHY_ADDRESS_MASK_VALID);
}

/**
  The constructor function for the library

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
InitializePlatMemErrorDataLib (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  BOOLEAN        NewAllocation;

  PoisonTrack = GetRasGlobalData (L"PoisonTrack",
                 sizeof (POISON_TRACK),
                 &NewAllocation
                 );
  RAS_ASSERT (PoisonTrack != NULL);

  if (NewAllocation) {
    ZeroMem (PoisonTrack, sizeof (POISON_TRACK));
  }

  mIs14nmCpu = IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL);
  return EFI_SUCCESS;
}
