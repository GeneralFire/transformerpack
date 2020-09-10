/** @file
  Implementation of the PPR & VLS Error Log Listener.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2017 Intel Corporation. <BR>

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

#include <Library/SmmServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/SmmVariable.h>
#include <Protocol/VariableLock.h>
#include <Library/RasDebugLib.h>
#include <Library/ErrorReportLib.h>
#include <Library/PlatPolicyLib.h>
#include <Library/ImcRoutingLib.h>
#include <SiliconSetting.h>
#include "PprVlsErrorLogListener.h"
#include <Guid/PprVariable.h>
#include <Guid/VlsVariableGuid.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/RasMcpLib.h>
#include <Library/PprCmosLib.h>
#include <IndustryStandard/Acpi62.h>

// Module global data
RAS_POLICY *PolicyData =NULL;

//
// Global variable to cache pointer to SMM Variable protocol.
//
EFI_SMM_VARIABLE_PROTOCOL      *mSmmVariableProtocolPtr = NULL;

//
// Global variable to cache variable lock protocol.
//
EDKII_VARIABLE_LOCK_PROTOCOL   *mVariableLockPtr = NULL;


/**
  Entry point for the Core Handler initialization.

  Initialize the VIRTUAL_LOCKSTEP_VARIABLE_NAME variable to 0s at the boot time.


  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval Status.

**/
EFI_STATUS
EFIAPI
InitializPprVlsErrorLogListener (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS                          Status;
  VIRTUAL_LOCKSTEP_VARIABLE_DATA      VLSInfo;
  UINTN                               VLSVarSize;
  PPR_ADDR_VARIABLE                   PprData;
  UINTN                               PprVarSize;

  RAS_DEBUG((LEVEL_FUNC_FLOW,"InitializPprVlsErrorLogListener++\n"));

  //
  // Get PPR Policy
  //
  PolicyData = GetRasPolicy();
  RAS_DEBUG((LEVEL_FUNC_FLOW,"PprType: %d, PprErrInjTest: %d\n", \
             PolicyData->ImcPolicy.PprType, PolicyData->ImcPolicy.PprErrInjTest));

  Status = gSmst->SmmLocateProtocol (&gEfiSmmVariableProtocolGuid, NULL, (VOID **)&mSmmVariableProtocolPtr);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gEdkiiVariableLockProtocolGuid, NULL, (VOID **)&mVariableLockPtr);
  ASSERT_EFI_ERROR (Status);

  //
  // Clear VLS Variable from storage
  //
  VLSVarSize = sizeof(VIRTUAL_LOCKSTEP_VARIABLE_DATA);
  ZeroMem (&VLSInfo, VLSVarSize);

  Status = mSmmVariableProtocolPtr->SmmSetVariable (
                              VIRTUAL_LOCKSTEP_VARIABLE_NAME,
                              &gEfiVirtualLockstepGuid,
                              EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                              VLSVarSize,
                              &VLSInfo
                              );

  if(EFI_ERROR(Status)){
    RAS_DEBUG((LEVEL_FUNC_FLOW,"ERROR INITIALIZING - In Set Variable status:%d \n",Status));
  }

  //
  // Mark VLS_VARIABLE_NAME variable to read-only if the Variable Lock protocol exists
  // This will allow access only through smm and restrict 3rd party access
  //
  Status = mVariableLockPtr->RequestToLock (mVariableLockPtr, VIRTUAL_LOCKSTEP_VARIABLE_NAME, &gEfiVirtualLockstepGuid);
  ASSERT_EFI_ERROR (Status);

  //
  //Clear PPR Variable from storage
  //
  PprVarSize = sizeof(PPR_ADDR_VARIABLE);
  ZeroMem(&PprData, PprVarSize);

  Status = mSmmVariableProtocolPtr->SmmSetVariable (
                              PPR_VARIABLE_NAME,
                              &gEfiPprVariableGuid,
                              EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                              PprVarSize,
                              &PprData
                              );
  if(EFI_ERROR(Status)){
    RAS_DEBUG((LEVEL_FUNC_FLOW,"ERROR INITIALIZING on clearing PPR data- In Set Variable status:%d \n",Status));
  }

  //
  // Mark PPR_VARIABLE_NAME variable to read-only if the Variable Lock protocol exists
  // This will allow access only through smm and restrict 3rd party access
  //
  Status = mVariableLockPtr->RequestToLock (mVariableLockPtr, PPR_VARIABLE_NAME, &gEfiPprVariableGuid);
  ASSERT_EFI_ERROR (Status);

  RAS_DEBUG((LEVEL_FUNC_FLOW, "Clearing Ppr Data Consumed bit in CMOS\n"));
  ResetPprDataConsumed ();

  Status = RegisterErrorLogListener (LogErrorToPprVls);
  ASSERT_EFI_ERROR (Status);

  RAS_DEBUG((LEVEL_FUNC_FLOW,"InitializPprVlsErrorLogListener-- %r\n", Status));

  return Status;
}

/**

  Calculate the 16 bit CRC

  @param ptr    - Pointer to the beginning of the data to work on
  @param count  - number of bytes to check

  @retval INT16

**/
INT16
Crc16 (
  CHAR8 *Ptr,
  INTN  Count
  )
{
  INT16 Crc, I;
  Crc = 0;
  while (--Count >= 0)
  {
    Crc = Crc ^ (INT16)(INT32)*Ptr++ << 8;
    for (I = 0; I < 8; ++I)
    {
      if ((Crc & 0x8000) != 0)
      {
        Crc = Crc << 1 ^ 0x1021;
      }
      else
      {
        Crc = Crc << 1;
      }
    }
  }
  return (Crc & 0xFFFF);
}

EFI_STATUS
UpdatePprAddress(
    UINT8   NodeId,
    UINT8   ChannelId,
    UINT8   DimmSlot,
    UINT8   DimmRank,
    UINT8   Bank,
    UINT32  Row,
    UINT8   SubRank,
    UINT32  DramMask
  )
{
  UINT8             I;
  EFI_STATUS        Status;
  UINTN             VarSize;
  PPR_ADDR_VARIABLE PprData;
  UINT16            OldCrc;
  UINT16            CurCrc;

  VarSize = sizeof(PPR_ADDR_VARIABLE);
  Status = mSmmVariableProtocolPtr->SmmGetVariable (
                              PPR_VARIABLE_NAME,
                              &gEfiPprVariableGuid,
                              NULL,
                              &VarSize,
                              &PprData
                              );
  OldCrc = PprData.crc16;
  PprData.crc16 = 0;
  CurCrc = Crc16 ((CHAR8*)&PprData, (INTN)VarSize);
  if (EFI_ERROR(Status) || (OldCrc != CurCrc)) {
    //
    // Variable is not present. Initialize the data structure to all 0
    //
    ZeroMem (&PprData, VarSize);
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "[PPR] Var not found or crc mismatch! Zero-out struct - oldcrc:0x%0x, curCrc:0x%0x\n", OldCrc, CurCrc));
  }

  for (I = 0; I < MAX_PPR_ADDR_ENTRIES; I++) {
    if (PprData.pprAddrSetup[I].pprAddrStatus & PPR_STS_ADDR_VALID) {
      if ( (PprData.pprAddrSetup[I].socket == NODE_TO_SKT(NodeId)) &&
            (PprData.pprAddrSetup[I].mc == NODE_TO_MC(NodeId)) &&
            (PprData.pprAddrSetup[I].ch == ChannelId) &&
            (PprData.pprAddrSetup[I].rank == DimmRank) &&
            (PprData.pprAddrSetup[I].bank == Bank) &&
            (PprData.pprAddrSetup[I].row == Row) &&
            (PprData.pprAddrSetup[I].subRank == SubRank) &&
            (PprData.pprAddrSetup[I].nibbleMask == DramMask) ) {

        RAS_DEBUG ((LEVEL_FUNC_FLOW, "[PPR] Entry already existed for \n"));
        RAS_DEBUG ((LEVEL_FUNC_FLOW, "[PPR] Address-Socket[%d].MC[%d].CH[%d].D[%d].R[%d].CID[%d].Bank[%d].Row[%d].FailDevMask = 0x%0x\n",
          PprData.pprAddrSetup[I].socket,
          PprData.pprAddrSetup[I].mc,
          PprData.pprAddrSetup[I].ch,
          PprData.pprAddrSetup[I].dimm,
          PprData.pprAddrSetup[I].rank,
          PprData.pprAddrSetup[I].subRank,
          PprData.pprAddrSetup[I].bank,
          PprData.pprAddrSetup[I].row,
          PprData.pprAddrSetup[I].nibbleMask));

        break;
      }
    } else {
      PprData.pprAddrSetup[I].pprAddrStatus |= PPR_STS_ADDR_VALID;
      PprData.pprAddrSetup[I].socket  = NODE_TO_SKT(NodeId);
      PprData.pprAddrSetup[I].mc      = NODE_TO_MC(NodeId);
      PprData.pprAddrSetup[I].ch      = ChannelId;
      PprData.pprAddrSetup[I].dimm  = DimmSlot;
      PprData.pprAddrSetup[I].rank  = DimmRank;
      PprData.pprAddrSetup[I].bank  = Bank;
      PprData.pprAddrSetup[I].row   = Row;
      PprData.pprAddrSetup[I].subRank = SubRank;
      PprData.pprAddrSetup[I].nibbleMask = DramMask;

      RAS_DEBUG ((LEVEL_FUNC_FLOW, "[PPR] Error logged - Entry: %d \n", I));
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "[PPR] Address-Socket[%d].MC[%d].CH[%d].D[%d].R[%d].CID[%d].Bank[%d].Row[%d].FailDevMask = 0x%0x\n",
          PprData.pprAddrSetup[I].socket,
          PprData.pprAddrSetup[I].mc,
          PprData.pprAddrSetup[I].ch,
          PprData.pprAddrSetup[I].dimm,
          PprData.pprAddrSetup[I].rank,
          PprData.pprAddrSetup[I].subRank,
          PprData.pprAddrSetup[I].bank,
          PprData.pprAddrSetup[I].row,
          PprData.pprAddrSetup[I].nibbleMask));

      //
      // Update CRC and update the variable
      //
      PprData.crc16 = 0;
      PprData.crc16 = Crc16 ((CHAR8*)&PprData, VarSize);

      Status = mSmmVariableProtocolPtr->SmmSetVariable (
                              PPR_VARIABLE_NAME,
                              &gEfiPprVariableGuid,
                              EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                              VarSize,
                              &PprData
                              );
      if (EFI_ERROR(Status)) {
        RAS_DEBUG ((LEVEL_FUNC_FLOW, "PPR: Var Update for PPR new entry failed - status: %r\n", Status)
        );
      }
      break;
    }
  } // for loop
  return Status;
}

EFI_STATUS
UpdateVlsVariable (
  VOID
  )
{
  EFI_STATUS  Status;
  VIRTUAL_LOCKSTEP_VARIABLE_DATA   VLSInfo;
  UINTN                            VLSVarSize;

  VLSVarSize = sizeof(VIRTUAL_LOCKSTEP_VARIABLE_DATA);

  Status = mSmmVariableProtocolPtr->SmmGetVariable (
                              VIRTUAL_LOCKSTEP_VARIABLE_NAME,
                              &gEfiVirtualLockstepGuid,
                              NULL,
                              &VLSVarSize,
                              &VLSInfo
                              );

  //Variable was already updated before
  if(!EFI_ERROR(Status)) {
    if (VLSInfo.VirtualLockstepEstablished) {
      RAS_DEBUG((LEVEL_FUNC_FLOW,"Previously updated Virtual Lock step variable structure :%d\n",VLSInfo.VirtualLockstepEstablished));
      //Update the structure only one time
      return EFI_SUCCESS;
    }
  }
  if (EFI_ERROR(Status) || (!VLSInfo.VirtualLockstepEstablished) ) {
    //
    // Variable is not present. Initialize the data structure to all 0
    //
    ZeroMem (&VLSInfo, VLSVarSize);
  }

  VLSInfo.VirtualLockstepEstablished = TRUE;

  Status = mSmmVariableProtocolPtr->SmmSetVariable (
                              VIRTUAL_LOCKSTEP_VARIABLE_NAME,
                              &gEfiVirtualLockstepGuid,
                              EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                              VLSVarSize,
                              &VLSInfo
                              );

  if (EFI_ERROR(Status)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "Creating VLS established variable failed - Status: %r\n", Status));
  }
  return Status;
}


/**
  The PPR & VLS Error Logger.

  This function is responsible for logging errors for PPR & VLS.

  @param[in]  CodeType      Indicates the type of status code being reported.
  @param[in[  Value            Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.
  @param[in]  Instance         The enumeration of a hardware or software entity within
                           the system. A system may contain multiple entities that match a class/subclass
                           pairing. The instance differentiates between them. An instance of 0 indicates
                           that instance information is unavailable, not meaningful, or not relevant.
                           Valid instance numbers start with 1.
  @param[in]  CallerId         This optional parameter may be used to identify the caller.
                           This parameter allows the status code driver to apply different rules to
                           different callers.
  @param[in]  Data             This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS      Status code reported to serial I/O successfully.

**/
EFI_STATUS
LogErrorToPprVls (
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN EFI_GUID                 *CallerId,
  IN EFI_STATUS_CODE_DATA     *Data
  )
{
  EFI_COMMON_ERROR_RECORD_HEADER  *ErrorRecordHeader;
  EFI_ERROR_SECTION_DESCRIPTOR    *ErrorRecordDescription;
  EFI_PLATFORM_MEMORY_ERROR_DATA  *MemoryErrorData;
  EFI_STATUS                       Status = EFI_SUCCESS;

  UINT8                            NodeId;
  UINT8                            SubRank;
  UINT8                            ChOnNode;
  UINT8                            Socket;
  UINT8                            SktCh;
  UINT8                            Mc;
  UINT32                           RowAddress;
  UINT32                           DramMask = 0;

  //
  // Check whether status code is what we are interested in.
  //
  if (((CodeType & EFI_STATUS_CODE_TYPE_MASK) != EFI_ERROR_CODE) || \
      (Value != SMM_RAS_ENHANCED_ERROR_LOG) || \
      (Data == NULL)) {
    return EFI_UNSUPPORTED;
  }

  RAS_DEBUG((LEVEL_FUNC_FLOW, "LogErrorToPprVls++, CodeType: %X\n", CodeType));

  ErrorRecordHeader = (EFI_COMMON_ERROR_RECORD_HEADER *)((UINT8 *)Data + sizeof (EFI_STATUS_CODE_DATA));
  ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *)((UINT8 *)ErrorRecordHeader + sizeof(EFI_COMMON_ERROR_RECORD_HEADER));

  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->SectionType: %g\n", ErrorRecordDescription->SectionType));

  if (!CompareGuid (&ErrorRecordDescription->SectionType, &gEfiPlatformMemoryErrorSectionGuid)) {
    return EFI_UNSUPPORTED;
  }

  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordHeader->RecordLength: %lX\n", ErrorRecordHeader->RecordLength));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "sizeof(MEM_ERROR_RECORD): %lX\n", sizeof(MEM_ERROR_RECORD)));



  MemoryErrorData = (EFI_PLATFORM_MEMORY_ERROR_DATA *)((UINT8 *)ErrorRecordDescription + sizeof(EFI_ERROR_SECTION_DESCRIPTOR));

  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryErrorData->ValidFields: %lX\n", MemoryErrorData->ValidFields));

  //
  // Check whether memory row data is valid for PPR & VLS.
  //
  if ((MemoryErrorData->ValidFields & (EFI_PLATFORM_MEMORY_ROW_VALID | EFI_PLATFORM_MEMORY_ERROR_EXTENDED_ROW_BIT_16_17_VALID)) == 0) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "MemoryErrorData->ValidFields: %X\n", MemoryErrorData->ValidFields));
    return EFI_UNSUPPORTED;
  }

  Socket = (UINT8) (MemoryErrorData->Node);
  SktCh = (UINT8)(MemoryErrorData->Card);

  ReMapSktAndSktChForMcp (&Socket, &SktCh);

  RowAddress = (MemoryErrorData->Row)  + ((MemoryErrorData->ValidFields & EFI_PLATFORM_MEMORY_ERROR_EXTENDED_ROW_BIT_16_17_VALID ) ? (MemoryErrorData->Extended << 16) : 0);
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryErrorData->Node: %X\n", Socket));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryErrorData->Card: %X\n", SktCh));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryErrorData->ModuleRank: %X\n", MemoryErrorData->ModuleRank));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryErrorData->Bank: %X\n", MemoryErrorData->Bank));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryErrorData->Device: %X\n", MemoryErrorData->Device));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryErrorData->Row: %X\n", RowAddress));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "MemoryErrorData->Row[17:16]: %X\n", MemoryErrorData->Extended));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryErrorData->Column: %X\n", MemoryErrorData->Column));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryErrorData->RankNum: %X\n", MemoryErrorData->RankNum));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryErrorData->CardHandle: %X\n", MemoryErrorData->CardHandle));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryErrorData->ModuleHandle: %X\n", MemoryErrorData->ModuleHandle));

  Mc = SktCh/MAX_MC_CH;
  NodeId = SKTMC_TO_NODE(Socket, Mc);  

  SubRank = GetSubRank (NodeId, (UINT8)SktCh);
  ChOnNode = SKTCH_TO_NODECH((UINT8)SktCh);

  if (PolicyData->ImcPolicy.PprType) {
    if (ErrorRecordDescription->Severity == EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED) {
      if ((MemoryErrorData->ErrorType == EFI_PLATFORM_MEMORY_ERROR_SINGLESYMBOLS_CHIPKILL) ||
          (MemoryErrorData->ErrorType == EFI_PLATFORM_MEMORY_ERROR_SCRUB_UNCORRECTED)) {
        DramMask = 0;
      } else {
        DramMask = (UINT32) (1 << MemoryErrorData->Device);
      }
    } else {
      DramMask = 0;
    }

    Status = UpdatePprAddress(NodeId, \
                              ChOnNode, \
                              (UINT8)MemoryErrorData->ModuleRank, \
                              (UINT8)MemoryErrorData->RankNum, \
                              (UINT8)MemoryErrorData->Bank, \
                              RowAddress, \
                              SubRank, \
                              DramMask);
    RAS_DEBUG((LEVEL_FUNC_FLOW, "UpdatePprAddress: NodeId: %X, ChannelId: %X, DimmSlot: %X, DimmRank: %X, Bank: %X, Row: %X, SubRank: %X, DramMask: %X, %r\n", \
                                 NodeId, \
                                 ChOnNode, \
                                 MemoryErrorData->ModuleRank, \
                                 MemoryErrorData->RankNum, \
                                 MemoryErrorData->Bank, \
                                 RowAddress, \
                                 SubRank, \
                                 DramMask, \
                                 Status));
  }

  if(CheckVlsTriggered (Socket, Mc)) {
    Status = UpdateVlsVariable();
    RAS_DEBUG((LEVEL_FUNC_FLOW, "UpdateVlsVariable: %r\n", Status));
  }

  RAS_DEBUG((LEVEL_FUNC_FLOW, "LogErrorToPprVls--, %r\n", Status));
  return Status;
}

