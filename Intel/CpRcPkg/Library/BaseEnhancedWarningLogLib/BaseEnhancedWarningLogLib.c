/** @file Base implementation of the Enhanced Warning Log library class

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <SysHost.h>

#include <Library/EnhancedWarningLogLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/RcDebugLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BasicIpBlockServicesLib.h>

#include <Guid/MrcPlatformHooksGuid.h>
#include <Library/MemoryServicesLib.h>

#include <MaxCore.h>

//
// Calculate EWL Count
//

#define EWL_WARNING_LOG_COUNT (sizeof (EnhancedWarningLogCheck) / sizeof (EWL_SIZE_CHECK))

//
// Structure to check Type with Size match
//

EWL_SIZE_CHECK EnhancedWarningLogCheck[] = {
  {EwlType1, sizeof (EWL_ENTRY_TYPE1)},
  {EwlType2, sizeof (EWL_ENTRY_TYPE2)},
  {EwlType3, sizeof (EWL_ENTRY_TYPE3)},
  {EwlType4, sizeof (EWL_ENTRY_TYPE4)},
  {EwlType5, sizeof (EWL_ENTRY_TYPE5)},
  {EwlType6, sizeof (EWL_ENTRY_TYPE6)},
  {EwlType7, sizeof (EWL_ENTRY_TYPE7)},
  {EwlType8, sizeof (EWL_ENTRY_TYPE8)},
  {EwlType9, sizeof (EWL_ENTRY_TYPE9)},
  {EwlType10, sizeof (EWL_ENTRY_TYPE10)},
  {EwlType11, sizeof (EWL_ENTRY_TYPE11)},
  {EwlType12, sizeof (EWL_ENTRY_TYPE12)},
  {EwlType13, sizeof (EWL_ENTRY_TYPE13)},
  {EwlType14, sizeof (EWL_ENTRY_TYPE14)},
  {EwlType15, sizeof (EWL_ENTRY_TYPE15)},
  {EwlType16, sizeof (EWL_ENTRY_TYPE16)},
  {EwlType17, sizeof (EWL_ENTRY_TYPE17)},
  {EwlType18, sizeof (EWL_ENTRY_TYPE18)},
  {EwlType19, sizeof (EWL_ENTRY_TYPE19)},
  {EwlType20, sizeof (EWL_ENTRY_TYPE20)},
  {EwlType21, sizeof (EWL_ENTRY_TYPE21)},
  {EwlType22, sizeof (EWL_ENTRY_TYPE22)},
  {EwlType23, sizeof (EWL_ENTRY_TYPE23)},
  {EwlType24, sizeof (EWL_ENTRY_TYPE24)},
  {EwlType25, sizeof (EWL_ENTRY_TYPE25)},
  {EwlType26, sizeof (EWL_ENTRY_TYPE26)}
};

/**

  Called to see if the warning should be promoted to an exception

  @param[in]  Host        - Pointer to the system Host (root) structure
  @param[in]  EwlEntry    - Pointer to new EWL entry buffer

  @retval None

**/
VOID
PromoteWarningsToErrors (
  IN PSYSHOST          Host,         // Pointer to the system Host (root) structure
  IN EWL_ENTRY_HEADER  *EwlEntry     // Pointer to the EWL entry header
  )
{
  UINT8               i;
  UINT8               PromoteWarning = 0;
  EWL_ENTRY_CONTEXT   *ewlContext;
  SYS_SETUP           *Setup;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;
  EFI_STATUS          Status;
  EFI_STATUS          HookStatus;

  //
  // Check to see if the hook is present
  //

  HookStatus = FAILURE;

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    HookStatus = MrcPlatformHooksInterface->PlatformEwlLogEntry (EwlEntry);
  }

  //
  // If the hook was present, and it handled promoting
  // the warning, skip doing it ourselves.
  //

  if (HookStatus == MRC_STATUS_SUCCESS) {
    return;
  }

  Setup = GetSysSetupPointer ();

  //
  // Check if this is an Intel defined EWL entry
  //   The purpose is to determine if we have a valid context or not
  //   Any entry that has an EWL_ENTRY_CONTEXT following the EWL_ENTRY_HEADER
  //   may follow this path
  //
  if ((EwlEntry->Type < EwlTypeMax) &&
      (EwlEntry->Type != EwlType6) &&
      (EwlEntry->Type != EwlType8) &&
      (EwlEntry->Type != EwlType9) &&
      (EwlEntry->Type != EwlType21)) {
    //
    // EWL Context follows the EWL Header in all Intel defined entries
    //
    ewlContext = (EWL_ENTRY_CONTEXT*) (EwlEntry + 1); // Context follows Header, so + 1 adds size of header

    //
    // Caller assigned fatal error severity
    //
    if (EwlEntry->Severity == EwlSeverityFatal) {
      RcDebugPrint (SDBG_DEFAULT, "This is a Fatal Error!\n");
      FatalError (ewlContext->MajorWarningCode, ewlContext->MinorWarningCode);
    }

    //
    // Promote warning control is based on setup option
    //
    if (Setup->common.options & PROMOTE_WARN_EN) {
      PromoteWarning = 1;

      //
      // Search platform exception list
      //
      for (i = 0; i < Host->var.common.promoteWarnLimit; i++) {
        if ((Host->var.common.promoteWarnList[i] == (UINT16) ((ewlContext->MajorWarningCode << 8) | ewlContext->MinorWarningCode)) ||
            (Host->var.common.promoteWarnList[i] == (UINT16) ((ewlContext->MajorWarningCode << 8) | WARN_MINOR_WILDCARD))
            ) {
          PromoteWarning = 0;
          break;
        }
      }

      //
      // Upgrade FPT warnings according to MRC_WARN setup option
      //
      if ((ewlContext->MajorWarningCode >= WARN_FPT_CORRECTABLE_ERROR) && (ewlContext->MajorWarningCode <= WARN_FPT_UNCORRECTABLE_ERROR)) {
        if (Setup->common.options & PROMOTE_MRC_WARN_EN) {
          PromoteWarning = 1;
        } else {
          PromoteWarning = 0;
        }
      }

      //
      // Promote to fatal
      //
      if (PromoteWarning) {
        RcDebugPrint (SDBG_DEFAULT, "Warning upgraded to Fatal Error!\n");
        FatalError (ewlContext->MajorWarningCode, ewlContext->MinorWarningCode);
      }

    }

  } else {
    //
    // This path means we don't know the format of the entry and the context
    // So we use the current global context as the approximation
    //

    //
    // Caller assigned fatal error severity
    //
    if (EwlEntry->Severity == EwlSeverityFatal) {
      RcDebugPrint (SDBG_DEFAULT, "This is a Fatal Error!\n");
      FatalError (((GetCheckPoint()) >> 24) & 0xFF, ((GetCheckPoint()) >> 16) & 0xFF);
    }

  }
}

/**

  Report a new entry is available for reporting to
  the Enhanced Warning Log.

  This function will add it to the log or return an error if there is insufficient
  space remaining for the entry.

  @param[in] EwlEntry    - Pointer to new EWL entry buffer
  @retval                - SUCCESS if entry is added; FAILURE if entry is not added

**/
MRC_STATUS
EwlLogEntry (
  IN EWL_ENTRY_HEADER  *EwlEntry
  )
{
  PSYSHOST              Host;
  SYS_SETUP             *Setup;
  EWL_PRIVATE_DATA      *ewlPrivateData;
  EWL_PUBLIC_DATA       *log;
  UINT32                offset;
  UINT32                entrySize;
  UINT8                 Index;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;
  EFI_STATUS            Status;
  EFI_STATUS            HookStatus;

  //
  // Check to see if the hook is present
  //

  HookStatus = FAILURE;

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    HookStatus = MrcPlatformHooksInterface->PlatformEwlLogEntry (EwlEntry);
  }

  //
  // If the hook was present, and it handled adding
  // the entry, skip doing it ourselves.
  //

  if (HookStatus == MRC_STATUS_SUCCESS) {
    return MRC_STATUS_SUCCESS;
  }

  Host              = GetSysHostPointer ();
  Setup             = GetSysSetupPointer ();
  ewlPrivateData    = &Host->var.common.ewlPrivateData;
  log               = &Host->var.common.ewlPrivateData.status;
  offset            = log->Header.FreeOffset;
  entrySize         = EwlEntry->Size;
  Index             = 0;

  //
  // Assert if the type and size don't match expectations
  //
  for (Index = 0; Index < EWL_WARNING_LOG_COUNT; Index++) {
    if ((EwlEntry->Type == EnhancedWarningLogCheck[Index].Type) && (entrySize != EnhancedWarningLogCheck[Index].Size)) {
      RcDebugPrint (SDBG_DEFAULT, "\nEWL warning size mismatch");
      return FAILURE;
    }
  }

  //
  // Verify we have sufficient space for this entry.
  //
  if ((offset + entrySize) >= sizeof (log->Buffer)) {
    RcDebugPrint (SDBG_DEFAULT, "\nThis warning was omitted from the log, increase size of EWL buffer!");
    ewlPrivateData->bufSizeOverflow += entrySize;
    return FAILURE;
  }

  //
  // Add the entry
  //
  CopyMem (&log->Buffer[offset], (UINT8*) EwlEntry, entrySize); //dest, source, size

  //
  // Update the FreeOffset and internal tracking
  //
  log->Header.FreeOffset += entrySize;
  ewlPrivateData->numEntries += 1;

  //
  // Recompute crc
  //
  log->Header.Crc = CalculateCrc32 ((VOID *) log, sizeof (EWL_PUBLIC_DATA));

  //
  // Promote to fatal and handle fatal errors
  //

  PromoteWarningsToErrors (Host, EwlEntry);

  return MRC_STATUS_SUCCESS;

} // EwlLogEntry

/**

  Print the memory location.

  @param[in]  Header            Ptr to the EWL header.

  @retval None

**/

VOID
EwlPrintMemoryLocation (
  IN EWL_ENTRY_MEMORY_LOCATION MemoryLocation
  )
{
#ifdef DEBUG_CODE_BLOCK

  if (MemoryLocation.Socket != 0xFF) {
    RcDebugPrint (SDBG_DEFAULT, "\nSocket %d", MemoryLocation.Socket);
  }
  if (MemoryLocation.Channel != 0xFF) {
    RcDebugPrint (SDBG_DEFAULT, "\nChannel %d", MemoryLocation.Channel);
  }
  if (MemoryLocation.Dimm != 0xFF) {
    RcDebugPrint (SDBG_DEFAULT, "\nDimm %d", MemoryLocation.Dimm);
  }
  if (MemoryLocation.Rank != 0xFF) {
    RcDebugPrint (SDBG_DEFAULT, "\nRank %d", MemoryLocation.Rank);
  }

#endif // #ifdef DEBUG_CODE_BLOCK

  return;

} // EwlPrintMemoryLocation

/*
  Print the strobe with both non-encoded and encoded values.

  Printing is done at a debug level and in a format that matches other EWL output.

  @param[in]  Socket    The socket containing the strobe of interest
  @param[in]  Channel   The channel containing the strobe of interest
  @param[in]  Dimm      The DIMM containing the strobe of interest
  @param[in]  Strobe    The strobe of interest

  @return N/A
*/
VOID
EwlPrintStrobeWithEncoding (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  IN  UINT8 Dimm,
  IN  UINT8 Strobe
  )
{
  UINT8 EncodedStrobe;

  EncodedStrobe = StrobeEncoding (Socket, Channel, Dimm, Strobe);
  RcDebugPrint (SDBG_DEFAULT, "\nStrobe: %d (Encoded: ", Strobe);
  if (EncodedStrobe == NO_STROBE) {
    RcDebugPrint (SDBG_DEFAULT, "Unavailable)");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "%d)", EncodedStrobe);
  }
}

/**

  Print enhanced warning log basic info.

  @param[in]  Header            Ptr to the EWL header.

  @retval None

**/

VOID
EwlPrintBasicInfo (
  IN EWL_ENTRY_HEADER  *Header
  )
{
  //
  // Treat warning as type 1, collect basic information and print to serial log
  //

  EWL_ENTRY_TYPE1 *BasicWarning;

  BasicWarning = (EWL_ENTRY_TYPE1 *)Header;

  RcDebugPrint (SDBG_DEFAULT, "\nMajor Warning Code = 0x%02X, Minor Warning Code = 0x%02X,", BasicWarning->Context.MajorWarningCode, BasicWarning->Context.MinorWarningCode);
  RcDebugPrint (SDBG_DEFAULT, "\nMajor Checkpoint: 0x%02X", BasicWarning->Context.MajorCheckpoint);
  RcDebugPrint (SDBG_DEFAULT, "\nMinor Checkpoint: 0x%02X", BasicWarning->Context.MinorCheckpoint);

  return;

} // EwlPrintBasicInfo

/**

  Print enhanced warning log info.

  @param[in]  Header            Ptr to the EWL header.

  @retval None

**/

VOID
EFIAPI
EwlPrintWarning (
  IN EWL_ENTRY_HEADER *WarningHeader
  )
{
#ifdef DEBUG_CODE_BLOCK

  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  RcDebugPrint (SDBG_DEFAULT, "\nEnhanced warning of type %d logged:", WarningHeader->Type);

  //
  // Print warning codes and current checkpoint (Context)
  //

  EwlPrintBasicInfo (WarningHeader);

  //
  // Print type specific details
  //

  switch (WarningHeader->Type) {
    case EwlType0:
      RcDebugPrint (SDBG_DEFAULT, "\nEwl spec does not define type 0 warning");
      break;

    case EwlType1:

      {
        EWL_ENTRY_TYPE1* Type1;

        Type1 = (EWL_ENTRY_TYPE1 *)WarningHeader;

        EwlPrintMemoryLocation (Type1->MemoryLocation);
        break;

      }

    case EwlType2:
      {
        EWL_ENTRY_TYPE2* Type2;
        CHAR8 *GroupString;

        Type2 = (EWL_ENTRY_TYPE2 *)WarningHeader;

        EwlPrintMemoryLocation (Type2->MemoryLocation);
        if (Type2->Strobe != 0xFF) {
          EwlPrintStrobeWithEncoding (Type2->MemoryLocation.Socket, Type2->MemoryLocation.Channel,
            Type2->MemoryLocation.Dimm, Type2->Strobe);
        }
        if (Type2->Bit != 0xFF) {
          RcDebugPrint (SDBG_DEFAULT, "\nBit:    %d", Type2->Bit);
        }
        if (Type2->Level != MrcLtDelim) {
          RcDebugPrint (SDBG_DEFAULT, "\nLevel:  %a(%d)", GetLevelStr (Type2->Level), Type2->Level);
        }
        if (Type2->Group != MrcGtDelim) {
          if (EFI_ERROR (GetGroupString (Type2->Group, &GroupString))) {
            GroupString = "Unknown group";
          }
          RcDebugPrint (SDBG_DEFAULT, "\nGroup:  %a(%d)", GroupString, Type2->Group);
        }
        if (Type2->EyeSize != 0xFF) {
          RcDebugPrint (SDBG_DEFAULT, "\nEyesize %d", Type2->EyeSize);
        }
        break;
      }

    case EwlType3:
      {
        EWL_ENTRY_TYPE3* Type3;
        CHAR8 *GroupString;

        Type3 = (EWL_ENTRY_TYPE3 *)WarningHeader;

        EwlPrintMemoryLocation (Type3->MemoryLocation);

        if (Type3->Level != MrcLtDelim) {
          RcDebugPrint (SDBG_DEFAULT, "\nLevel:   %a(%d)", GetLevelStr (Type3->Level), Type3->Level);
        }
        if (Type3->Group != MrcGtDelim) {
          if (EFI_ERROR (GetGroupString (Type3->Group, &GroupString))) {
            GroupString = "Unknown group";
          }
          RcDebugPrint (SDBG_DEFAULT, "\nGroup:   %a(%d)", GroupString, Type3->Group);
        }
        if (Type3->Signal != gsmCsnDelim) {
          RcDebugPrint (SDBG_DEFAULT, "\nSignal:  %a(%d)", GetSignalStr (Type3->Signal), Type3->Signal);
        }
        if (Type3->EyeSize != 0xFF) {
          RcDebugPrint (SDBG_DEFAULT, "\nEyesize: %d", Type3->EyeSize);
        }
        break;
      }

    case EwlType4:

      {
        EWL_ENTRY_TYPE4* Type4;

        Type4 = (EWL_ENTRY_TYPE4 *)WarningHeader;

        EwlPrintMemoryLocation (Type4->MemoryLocation);

        if (Type4->MemtestType != AdvMtDelim) {
          RcDebugPrint (SDBG_DEFAULT, "\nMemtest of type %d", Type4->MemtestType);
        }
        RcDebugPrint (SDBG_DEFAULT,"\nDat0S:      %08x", Type4->AdvMemtestErrorInfo.Dat0S);
        RcDebugPrint (SDBG_DEFAULT,"\nDat1S:      %08x", Type4->AdvMemtestErrorInfo.Dat1S);
        RcDebugPrint (SDBG_DEFAULT,"\nDat2S:      %08x", Type4->AdvMemtestErrorInfo.Dat2S);
        RcDebugPrint (SDBG_DEFAULT,"\nDat3S:      %08x", Type4->AdvMemtestErrorInfo.Dat3S);
        RcDebugPrint (SDBG_DEFAULT,"\nEccS:       %08x", Type4->AdvMemtestErrorInfo.EccS);
        RcDebugPrint (SDBG_DEFAULT,"\nChunk:      %08x", Type4->AdvMemtestErrorInfo.Chunk);
        RcDebugPrint (SDBG_DEFAULT,"\nColumn:     %08x", Type4->AdvMemtestErrorInfo.Column);
        RcDebugPrint (SDBG_DEFAULT,"\nColumnExt:  %08x", Type4->AdvMemtestErrorInfo.ColumnExt);
        RcDebugPrint (SDBG_DEFAULT,"\nRow:        %08x", Type4->AdvMemtestErrorInfo.Row);
        RcDebugPrint (SDBG_DEFAULT,"\nRowExt:     %08x", Type4->AdvMemtestErrorInfo.RowExt);
        RcDebugPrint (SDBG_DEFAULT,"\nBank:       %08x", Type4->AdvMemtestErrorInfo.Bank);
        RcDebugPrint (SDBG_DEFAULT,"\nRank:       %08x", Type4->AdvMemtestErrorInfo.Rank);
        RcDebugPrint (SDBG_DEFAULT,"\nSubrank:    %08x", Type4->AdvMemtestErrorInfo.Subrank);
        RcDebugPrint (SDBG_DEFAULT,"\nCount:      %d", Type4->Count);
        break;

      }

    case EwlType5:

      {
        EWL_ENTRY_TYPE5* Type5;

        Type5 = (EWL_ENTRY_TYPE5 *)WarningHeader;

        EwlPrintMemoryLocation (Type5->MemoryLocation);
        if (Type5->SubRank != 0xFF) {
          RcDebugPrint (SDBG_DEFAULT, "\nSubRank %d", Type5->SubRank);
        }

        if (Type5->BankAddress != 0xFF) {
          RcDebugPrint (SDBG_DEFAULT, "\nBank Address %d", Type5->BankAddress);
        }
        RcDebugPrint (SDBG_DEFAULT, "\nDq bytes 8 - 0 : 0x%02X %02X %02X %02X %02X %02X %02X %02X %02X",
                   Type5->DqBytes[8],
                   Type5->DqBytes[7],
                   Type5->DqBytes[6],
                   Type5->DqBytes[5],
                   Type5->DqBytes[4],
                   Type5->DqBytes[3],
                   Type5->DqBytes[2],
                   Type5->DqBytes[1],
                   Type5->DqBytes[0]);
        break;
      }

    case EwlType7:

      {
        EWL_ENTRY_TYPE7* Type7;

        Type7 = (EWL_ENTRY_TYPE7 *)WarningHeader;
        RcDebugPrint (SDBG_DEFAULT, "\nSocket: %X", Type7->Socket);
        RcDebugPrint (SDBG_DEFAULT, "\nCore: %d", Type7->Core);
        break;
      }

    case EwlType8:

      {
        EWL_ENTRY_TYPE8* Type8;

        Type8 = (EWL_ENTRY_TYPE8 *)WarningHeader;
        RcDebugPrint (SDBG_DEFAULT, "\nSocket: %d", Type8->LinkDescription.Socket);
        RcDebugPrint (SDBG_DEFAULT, "\nStack: %d", Type8->LinkDescription.Stack);
        RcDebugPrint (SDBG_DEFAULT, "\nPort: %d", Type8->LinkDescription.Port);
        RcDebugPrint (SDBG_DEFAULT, "\nExpectedLinkWidth: %d", Type8->ExpectedLinkWidth);
        RcDebugPrint (SDBG_DEFAULT, "\nActualLinkWidth: %d", Type8->ActualLinkWidth);
        break;
      }

    case EwlType9:

      {
        EWL_ENTRY_TYPE9* Type9;

        Type9 = (EWL_ENTRY_TYPE9 *)WarningHeader;
        RcDebugPrint (SDBG_DEFAULT, "\nSocket: %d", Type9->LinkDescription.Socket);
        RcDebugPrint (SDBG_DEFAULT, "\nStack: %d", Type9->LinkDescription.Stack);
        RcDebugPrint (SDBG_DEFAULT, "\nPort: %d", Type9->LinkDescription.Port);
        RcDebugPrint (SDBG_DEFAULT, "\nExpectedLinkSpeed: %d", Type9->ExpectedLinkSpeed);
        RcDebugPrint (SDBG_DEFAULT, "\nActualLinkSpeed: %d", Type9->ActualLinkSpeed);
        break;
      }

    case EwlType10:

      {
        EWL_ENTRY_TYPE10* Type10;

        Type10 = (EWL_ENTRY_TYPE10 *)WarningHeader;

        EwlPrintMemoryLocation (Type10->MemoryLocation);
        RcDebugPrint (SDBG_DEFAULT, "\nSwizzle Pattern: 0x%X", Type10->SwizzlePattern);
        RcDebugPrint (SDBG_DEFAULT, "\nSwizzled Lanes: 0x%X", Type10->SwizzledDqLanes);
        RcDebugPrint (SDBG_DEFAULT, "\nLanes Per Strobe: %d", Type10->LanesPerStrobe);
        EwlPrintStrobeWithEncoding (Type10->MemoryLocation.Socket, Type10->MemoryLocation.Channel,
          Type10->MemoryLocation.Dimm, Type10->Strobe);
        break;
      }

    case EwlType11:

      {
        EWL_ENTRY_TYPE11* Type11;

        Type11 = (EWL_ENTRY_TYPE11 *)WarningHeader;

        EwlPrintMemoryLocation (Type11->MemoryLocation);
        RcDebugPrint (SDBG_DEFAULT, "\nBoot Status Register: 0x%x", Type11->BootStatusRegister);
        break;

      }

    case EwlType12:

      {
        EWL_ENTRY_TYPE12* Type12;

        Type12 = (EWL_ENTRY_TYPE12 *)WarningHeader;

        EwlPrintMemoryLocation (Type12->MemoryLocation);
        RcDebugPrint (SDBG_DEFAULT, "\nPMem Mailbox Command: 0x%x", Type12->Command);
        RcDebugPrint (SDBG_DEFAULT, "\nPMem Mailbox Status: 0x%x", Type12->Status);
        break;
      }

    case EwlType13:

      {
        EWL_ENTRY_TYPE13* Type13;

        Type13 = (EWL_ENTRY_TYPE13 *)WarningHeader;

        EwlPrintMemoryLocation (Type13->MemoryLocation);
        RcDebugPrint (SDBG_DEFAULT, "\nPMem FW revision: 0x%x", Type13->RevisionNvmdimmFw);
        RcDebugPrint (SDBG_DEFAULT, "\nPMem SPD revision: 0x%x", Type13->RevisionNvmdimmSpd);
        break;
      }
    default:
      //expand this print function as warning types get implemented
      //We shouldn't reach this default case
      RcDebugPrint (SDBG_DEFAULT, "\nPrint function has not been implemented for this type of warning");
      break;
  }

  RcDebugPrint (SDBG_DEFAULT, "\n");
#endif // #ifdef DEBUG_CODE_BLOCK
  return;

} // EwlPrintWarning

/**

  Count the number of warnings in the enhanced warning log.

  @param  None

  @retval Number of warnings in the log

**/

UINT32
EFIAPI
CountWarningsInLog (
  VOID
  )
{
  PSYSHOST              Host;
  UINT8                 *Buffer;
  UINT32                NumEntries = 0;
  EWL_ENTRY_HEADER      *CurrentWarning;
  VOID                  *FreeSpace;

  Host = GetSysHostPointer ();

  Buffer = &Host->var.common.ewlPrivateData.status.Buffer[0];
  CurrentWarning = (EWL_ENTRY_HEADER*) &Buffer[0];

  FreeSpace = &Buffer[Host->var.common.ewlPrivateData.status.Header.FreeOffset];

  while ((VOID*) CurrentWarning < FreeSpace) {
    CurrentWarning = (EWL_ENTRY_HEADER*) ((UINT8*)CurrentWarning + CurrentWarning->Size);
    NumEntries++;
  }

  return NumEntries;

} // CountWarningsInLog

/**

  Construct a EWL type 1 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Rank                  Rank.

  @retval None

**/

VOID
EFIAPI
EwlOutputType1 (
  IN UINT8    WarningCode,
  IN UINT8    MinorWarningCode,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  EFI_STATUS Status;
  PSYSHOST Host;
  EWL_ENTRY_TYPE1   LegacyWarning;
  UINT8 SocketId;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  Host = GetSysHostPointer ();
  SocketId = GetSysSocketId ();

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformOutputWarning (WarningCode, MinorWarningCode, Socket, Channel, Dimm, Rank);
  }

  //
  // A LogWarning call results in a legacy type 1 warning being added to the enhanced warning log
  //

  LegacyWarning.Header.Size               = sizeof(EWL_ENTRY_TYPE1);
  LegacyWarning.Header.Type               = EwlType1;
  LegacyWarning.Header.Severity           = EwlSeverityWarning;
  LegacyWarning.Context.MajorCheckpoint   = GetLastMajorCheckPoint (SocketId); // shift to get Major Checkpoint
  LegacyWarning.Context.MinorCheckpoint   = GetLastMinorCheckPoint (SocketId); // shift to get Minor
  LegacyWarning.Context.MajorWarningCode  = WarningCode;
  LegacyWarning.Context.MinorWarningCode  = MinorWarningCode;
  LegacyWarning.MemoryLocation.Socket     = Socket;
  LegacyWarning.MemoryLocation.Channel    = Channel;
  LegacyWarning.MemoryLocation.Dimm       = Dimm;
  LegacyWarning.MemoryLocation.Rank       = Rank;

  //
  // Output warning details to the Serial Log
  //
  EwlPrintWarning (&(LegacyWarning.Header));

  //
  // Log the warning
  //

  EwlLogEntry (&LegacyWarning.Header);

  return;

} // EwlOutputType1

/**

  Construct a EWL type 2 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Rank                  Rank.
  @param[in]  Severity              Severity.
  @param[in]  Strobe                Strobe.
  @param[in]  Bit                   Bit.
  @param[in]  Group                 Group.
  @param[in]  Level                 Level.
  @param[in]  EyeSize               EyeSize.

  @retval None

**/

VOID
EFIAPI
EwlOutputType2 (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN EWL_SEVERITY Severity,
  IN UINT8        Strobe,
  IN UINT8        Bit,
  IN MRC_GT       Group,
  IN MRC_LT       Level,
  IN UINT8        EyeSize
  )
{
  EFI_STATUS Status;
  PSYSHOST Host;
  EWL_ENTRY_TYPE2   Type2;
  UINT8 SocketId;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  Host = GetSysHostPointer ();
  SocketId = GetSysSocketId ();

  SaveDdrtTrainingFailureCode (WarningCode, MinorWarningCode, Socket, Channel, Dimm);

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformOutputWarning (WarningCode, MinorWarningCode, Socket, Channel, Dimm, Rank);
  }

  Type2.Header.Size               = sizeof(EWL_ENTRY_TYPE2);
  Type2.Header.Type               = EwlType2;
  Type2.Header.Severity           = Severity;

  Type2.Context.MajorCheckpoint   = GetLastMajorCheckPoint (SocketId); //shift to get Major Checkpoint
  Type2.Context.MinorCheckpoint   = GetLastMinorCheckPoint (SocketId); //shift to get Minor
  Type2.Context.MajorWarningCode  = WarningCode;
  Type2.Context.MinorWarningCode  = MinorWarningCode;

  Type2.MemoryLocation.Socket     = Socket;
  Type2.MemoryLocation.Channel    = Channel;
  Type2.MemoryLocation.Dimm       = Dimm;
  Type2.MemoryLocation.Rank       = Rank;

  Type2.Level                     = Level;
  Type2.Group                     = Group;
  Type2.Strobe                    = Strobe;
  Type2.Bit                       = Bit;
  Type2.EyeSize                   = EyeSize;

  //
  // Output warning details to the Serial Log
  //
  EwlPrintWarning (&Type2.Header);

  //
  // Log the warning
  //

  EwlLogEntry (&Type2.Header);

  return;

} // EwlOutputType2

/**

  Construct a EWL type 3 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Rank                  Rank.
  @param[in]  Severity              Severity.
  @param[in]  Group                 Group.
  @param[in]  Level                 Level.
  @param[in]  Level                 Signal.
  @param[in]  EyeSize               EyeSize.

  @retval None

**/

VOID
EFIAPI
EwlOutputType3 (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN EWL_SEVERITY Severity,
  IN MRC_GT       Group,
  IN MRC_LT       Level,
  IN GSM_CSN      Signal,
  IN UINT8        EyeSize
  )
{
  EFI_STATUS Status;
  PSYSHOST     Host;
  EWL_ENTRY_TYPE3   Type3;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;
  UINT8 SocketId = GetSysSocketId ();

  Host = GetSysHostPointer ();

  RcDebugPrintWithDevice (
    SDBG_MAX,
    Socket, Channel,
    NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Eye width is too small: %d\n", EyeSize
    );

  SaveDdrtTrainingFailureCode (WarningCode, MinorWarningCode, Socket, Channel, Dimm);

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformOutputWarning (WarningCode, MinorWarningCode, Socket, Channel, Dimm, Rank); //OEM specific
  }

  Type3.Header.Size               = sizeof(EWL_ENTRY_TYPE3);
  Type3.Header.Type               = EwlType3;
  Type3.Header.Severity           = Severity;

  Type3.Context.MajorCheckpoint   = GetLastMajorCheckPoint (SocketId);//shift to get Major Checkpoint
  Type3.Context.MinorCheckpoint   = GetLastMinorCheckPoint (SocketId);//shift to get Minor
  Type3.Context.MajorWarningCode  = WarningCode;
  Type3.Context.MinorWarningCode  = MinorWarningCode;

  Type3.MemoryLocation.Socket     = Socket;
  Type3.MemoryLocation.Channel    = Channel;
  Type3.MemoryLocation.Dimm       = Dimm;
  Type3.MemoryLocation.Rank       = Rank;

  Type3.Level                     = Level;
  Type3.Group                     = Group;
  Type3.Signal                    = Signal;
  Type3.EyeSize                   = EyeSize;

  //
  // Output warning details to the Serial Log
  //
  EwlPrintWarning (&Type3.Header);

  //
  // Log the warning
  //

  EwlLogEntry (&Type3.Header);

  return;

} // EwlOutputType3

/**

  Construct a EWL type 4 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Rank                  Rank.
  @param[in]  Severity              Severity.
  @param[in]  MemtestType           MemtestType.
  @param[in]  AdvMemtestErrorInfo   AdvMemtestErrorInfo.
  @param[in]  Count                 Count.

  @retval None

**/

VOID
EFIAPI
EwlOutputType4 (
  IN UINT8             WarningCode,
  IN UINT8             MinorWarningCode,
  IN UINT8             Socket,
  IN UINT8             Channel,
  IN UINT8             Dimm,
  IN UINT8             Rank,
  IN EWL_SEVERITY      Severity,
  IN ADV_MT_TYPE       MemtestType,
  IN EWL_ADV_MT_STATUS AdvMemtestErrorInfo,
  IN UINT32            Count
  )
{
  EFI_STATUS Status;
  PSYSHOST Host;
  EWL_ENTRY_TYPE4 Type4;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;
  UINT8 SocketId = GetSysSocketId ();

  Host = GetSysHostPointer ();

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformOutputWarning (WarningCode, MinorWarningCode, Socket, Channel, Dimm, Rank); //OEM specific
  }

  Type4.Header.Size              = sizeof(EWL_ENTRY_TYPE4);
  Type4.Header.Type              = EwlType4;
  Type4.Header.Severity          = Severity;

  Type4.Context.MajorCheckpoint  = GetLastMajorCheckPoint (SocketId);//shift to get Major Checkpoint
  Type4.Context.MinorCheckpoint  = GetLastMinorCheckPoint (SocketId);//shift to get Major Checkpoint
  Type4.Context.MajorWarningCode = WarningCode;
  Type4.Context.MinorWarningCode = MinorWarningCode;

  Type4.MemoryLocation.Socket    = Socket;
  Type4.MemoryLocation.Channel   = Channel;
  Type4.MemoryLocation.Dimm      = Dimm;
  Type4.MemoryLocation.Rank      = Rank;

  Type4.MemtestType              = MemtestType;
  Type4.AdvMemtestErrorInfo      = AdvMemtestErrorInfo;
  Type4.Count                    = Count;

  //
  // Output warning details to the Serial Log
  //
  EwlPrintWarning (&Type4.Header);

  //
  // Log the warning
  //

  EwlLogEntry (&Type4.Header);

  return;

} // EwlOutputType4

/**

  Construct a EWL type 5 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Rank                  Rank.
  @param[in]  Severity              Severity.
  @param[in]  MemtestType           MemtestType.
  @param[in]  AdvMemtestErrorInfo   AdvMemtestErrorInfo.
  @param[in]  Count                 Count.

  @retval None

**/

VOID
EFIAPI
EwlOutputType5 (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN EWL_SEVERITY Severity,
  IN UINT8        SubRank,
  IN UINT8        BankAddress,
  IN UINT8        *DqBytes
  )
{
  EFI_STATUS Status;
  PSYSHOST Host;
  EWL_ENTRY_TYPE5   Type5;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;
  UINT8 SocketId = GetSysSocketId ();

  Host = GetSysHostPointer ();

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformOutputWarning (WarningCode, MinorWarningCode, Socket, Channel, Dimm, Rank); //OEM specific
  }

  Type5.Header.Size                 = sizeof(EWL_ENTRY_TYPE5);
  Type5.Header.Type                 = EwlType5;
  Type5.Header.Severity             = Severity;

  Type5.Context.MajorCheckpoint     = GetLastMajorCheckPoint (SocketId);//shift to get Major Checkpoint;
  Type5.Context.MinorCheckpoint     = GetLastMinorCheckPoint (SocketId);//shift to get Minor
  Type5.Context.MajorWarningCode    = WarningCode;
  Type5.Context.MinorWarningCode    = MinorWarningCode;

  Type5.MemoryLocation.Socket       = Socket;
  Type5.MemoryLocation.Channel      = Channel;
  Type5.MemoryLocation.Dimm         = Dimm;
  Type5.MemoryLocation.Rank         = Rank;

  Type5.SubRank                     = SubRank;
  Type5.BankAddress                 = BankAddress;

  CopyMem (Type5.DqBytes, DqBytes, 9); // 9 bytes = 72 dq bits

  //
  // Output warning details to the Serial Log
  //
  EwlPrintWarning (&Type5.Header);

  //
  // Log the warning
  //

  EwlLogEntry (&Type5.Header);

  return;

} // EwlOutputType5

/**

  Construct a EWL type 7 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  BistMask              BistMask.

  @retval None

**/

VOID
EFIAPI
EwlOutputType7 (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT64       BistMask
  )
{
  EFI_STATUS Status;
  PSYSHOST Host;
  EWL_ENTRY_TYPE7   Type7;
  UINT8             CpuIndex;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;
  UINT8             SocketId = GetSysSocketId ();

  Host = GetSysHostPointer ();

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformOutputWarning (WarningCode, MinorWarningCode, Socket, 0xFF, 0xFF, 0xFF); //OEM specific
  }

  Type7.Header.Size                 = sizeof(EWL_ENTRY_TYPE7);
  Type7.Header.Type                 = EwlType7;
  Type7.Header.Severity             = EwlSeverityWarning;

  Type7.Context.MajorCheckpoint     = GetLastMajorCheckPoint (SocketId);//shift to get Major Checkpoint;
  Type7.Context.MinorCheckpoint     = GetLastMinorCheckPoint (SocketId);//shift to get Minor
  Type7.Context.MajorWarningCode    = WarningCode;
  Type7.Context.MinorWarningCode    = MinorWarningCode;

  Type7.Socket       = Socket;

  //
  // Find the BIST failing cores - each core failure will produce log entry
  //
  for(CpuIndex = 0; CpuIndex < MAX_CORE; CpuIndex++){
    if ((BistMask & BIT0) != 0) {
      Type7.Core = CpuIndex;
      //
      // Output warning details to the Serial Log
      //
      EwlPrintWarning (&Type7.Header);

      //
      // Log the warning
      //

      EwlLogEntry (&Type7.Header);

    }
    BistMask = RShiftU64 (BistMask, 1);
  }

  return;

} // EwlOutputType7

/**

  Construct a EWL type 10 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Rank                  Rank.
  @param[in]  Severity              Severity.
  @param[in]  SwizzlePattern        SwizzlePattern.
  @param[in]  SwizzledDqLanes       SwizzledDqLanes.
  @param[in]  LanesPerStrobe        LanesPerStrobe.
  @param[in]  Strobe                Strobe.

  @retval None

**/

VOID
EFIAPI
EwlOutputType10 (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN EWL_SEVERITY Severity,
  IN UINT8        SwizzlePattern,
  IN UINT8        SwizzledDqLanes,
  IN UINT8        LanesPerStrobe,
  IN UINT8        Strobe
  )
{
  EFI_STATUS Status;
  PSYSHOST Host;
  EWL_ENTRY_TYPE10  Type10;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;
  UINT8 SocketId = GetSysSocketId ();

  Host = GetSysHostPointer ();

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformOutputWarning (WarningCode, MinorWarningCode, Socket, Channel, Dimm, Rank);
  }

  Type10.Header.Size                = sizeof(EWL_ENTRY_TYPE10);
  Type10.Header.Type                = EwlType10;
  Type10.Header.Severity            = Severity;

  Type10.Context.MajorCheckpoint    = GetLastMajorCheckPoint (SocketId); // shift to get Major Checkpoint;
  Type10.Context.MinorCheckpoint    = GetLastMinorCheckPoint (SocketId); // shift to get Minor
  Type10.Context.MajorWarningCode   = WarningCode;
  Type10.Context.MinorWarningCode   = MinorWarningCode;

  Type10.MemoryLocation.Socket      = Socket;
  Type10.MemoryLocation.Channel     = Channel;
  Type10.MemoryLocation.Dimm        = Dimm;
  Type10.MemoryLocation.Rank        = Rank;

  Type10.SwizzlePattern             = SwizzlePattern;
  Type10.SwizzledDqLanes            = SwizzledDqLanes;
  Type10.LanesPerStrobe             = LanesPerStrobe;
  Type10.Strobe                     = Strobe;

  //
  // Output warning details to the Serial Log
  //
  EwlPrintWarning (&Type10.Header);

  //
  // Log the warning
  //

  EwlLogEntry (&Type10.Header);

  return;

} // EwlOutputType10

/**

  Construct a EWL type 11 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Rank                  Rank.
  @param[in]  Severity              Severity.
  @param[in]  Data                  Data.

  @retval None

**/

VOID
EFIAPI
EwlOutputType11 (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN EWL_SEVERITY Severity,
  IN UINT32       Data
  )
{
  EFI_STATUS Status;
  PSYSHOST Host;
  EWL_ENTRY_TYPE11  Type11;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;
  UINT8 SocketId = GetSysSocketId ();

  Host = GetSysHostPointer ();

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformOutputWarning (WarningCode, MinorWarningCode, Socket, Channel, Dimm, Rank);
  }

  Type11.Header.Size                = sizeof(EWL_ENTRY_TYPE11);
  Type11.Header.Type                = EwlType11;
  Type11.Header.Severity            = Severity;

  Type11.Context.MajorCheckpoint    = GetLastMajorCheckPoint (SocketId); // shift to get Major Checkpoint;
  Type11.Context.MinorCheckpoint    = GetLastMinorCheckPoint (SocketId); // shift to get Minor
  Type11.Context.MajorWarningCode   = WarningCode;
  Type11.Context.MinorWarningCode   = MinorWarningCode;

  Type11.MemoryLocation.Socket      = Socket;
  Type11.MemoryLocation.Channel     = Channel;
  Type11.MemoryLocation.Dimm        = Dimm;
  Type11.MemoryLocation.Rank        = Rank;

  Type11.BootStatusRegister         = Data;


  //
  // Output warning details to the Serial Log
  //
  EwlPrintWarning (&Type11.Header);

  //
  // Log the warning
  //

  EwlLogEntry (&Type11.Header);

  return;

} // EwlOutputType11

/**

  Construct a EWL type 12 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Rank                  Rank.
  @param[in]  Severity              Severity.
  @param[in]  Command               Command.
  @param[in]  Status                Status.

  @retval None

**/

VOID
EFIAPI
EwlOutputType12 (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN EWL_SEVERITY Severity,
  IN UINT16       Command,
  IN UINT8        Status
  )
{
  EFI_STATUS PpiStatus;
  PSYSHOST Host;
  EWL_ENTRY_TYPE12  Type12;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;
  UINT8 SocketId = GetSysSocketId ();

  Host = GetSysHostPointer ();

  PpiStatus = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (PpiStatus)) {
    MrcPlatformHooksInterface->PlatformOutputWarning (WarningCode, MinorWarningCode, Socket, Channel, Dimm, Rank);
  }

  Type12.Header.Size                = sizeof(EWL_ENTRY_TYPE12);
  Type12.Header.Type                = EwlType12;
  Type12.Header.Severity            = Severity;

  Type12.Context.MajorCheckpoint    = GetLastMajorCheckPoint (SocketId); // shift to get Major Checkpoint;
  Type12.Context.MinorCheckpoint    = GetLastMinorCheckPoint (SocketId); // shift to get Minor
  Type12.Context.MajorWarningCode   = WarningCode;
  Type12.Context.MinorWarningCode   = MinorWarningCode;

  Type12.MemoryLocation.Socket      = Socket;
  Type12.MemoryLocation.Channel     = Channel;
  Type12.MemoryLocation.Dimm        = Dimm;
  Type12.MemoryLocation.Rank        = Rank;

  Type12.Revision                   = 0x0104;
  Type12.Command                    = Command;
  Type12.Status                     = Status;

  //
  // Output warning details to the Serial Log
  //
  EwlPrintWarning (&Type12.Header);

  //
  // Log the warning
  //

  EwlLogEntry (&Type12.Header);

  return;

} // EwlOutputType12

/**

  Construct a EWL type 13 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Rank                  Rank.
  @param[in]  Severity              Severity.
  @param[in]  RevisionNvmdimmFw     RevisionNvmdimmFw.
  @param[in]  RevisionNvmdimmSpd    RevisionNvmdimmSpd.

  @retval None

**/

VOID
EFIAPI
EwlOutputType13 (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN EWL_SEVERITY Severity,
  IN UINT16       RevisionNvmdimmFw,
  IN UINT8        RevisionNvmdimmSpd
  )
{
  EFI_STATUS Status;
  PSYSHOST Host;
  EWL_ENTRY_TYPE13  Type13;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;
  UINT8 SocketId = GetSysSocketId ();

  Host = GetSysHostPointer ();

  SaveDdrtTrainingFailureCode (WarningCode, MinorWarningCode, Socket, Channel, Dimm);

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformOutputWarning (WarningCode, MinorWarningCode, Socket, Channel, Dimm, Rank);
  }

  Type13.Header.Size                = sizeof(EWL_ENTRY_TYPE13);
  Type13.Header.Type                = EwlType13;
  Type13.Header.Severity            = Severity;

  Type13.Context.MajorCheckpoint    = GetLastMajorCheckPoint (SocketId); // shift to get Major Checkpoint;
  Type13.Context.MinorCheckpoint    = GetLastMinorCheckPoint (SocketId); // shift to get Minor
  Type13.Context.MajorWarningCode   = WarningCode;
  Type13.Context.MinorWarningCode   = MinorWarningCode;

  Type13.MemoryLocation.Socket      = Socket;
  Type13.MemoryLocation.Channel     = Channel;
  Type13.MemoryLocation.Dimm        = Dimm;
  Type13.MemoryLocation.Rank        = Rank;

  Type13.RevisionNvmdimmFw          = RevisionNvmdimmFw;
  Type13.RevisionNvmdimmSpd         = RevisionNvmdimmSpd;

  //
  // Output warning details to the Serial Log
  //
  EwlPrintWarning (&Type13.Header);

  //
  // Log the warning
  //

  EwlLogEntry (&Type13.Header);

  return;

} // EwlOutputType13

/**

  Construct a EWL type 23 warning for report DCPM BSR(Boot Status Register)

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Severity.
  @param[in]  Ch                    Channel.
  @param[in]  Dimm                  Dimm.


  @retval None

**/
VOID
EwlOutputType23 (
  UINT8     WarningCode,
  UINT8     MinorWarningCode,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT32    Bsr,
  UINT32    BsrHi
  )
{
  EWL_ENTRY_TYPE23  Type23;
  PSYSHOST          Host;

  Host = GetSysHostPointer ();

  Type23.Header.Size     = sizeof(EWL_ENTRY_TYPE23);
  Type23.Header.Type     = EwlType23;
  Type23.Header.Severity = EwlSeverityInfo;
  Type23.Context.MajorWarningCode = WarningCode;
  Type23.Context.MinorWarningCode = MinorWarningCode;

  Type23.MemoryLocation.Socket  = Socket;
  Type23.MemoryLocation.Channel = Ch;
  Type23.MemoryLocation.Dimm    = Dimm;
  Type23.MemoryLocation.Rank    = 0;
  Type23.BootStatusRegister     = Bsr;
  Type23.BootStatusRegisterHi   = BsrHi;

  EwlLogEntry (&Type23.Header);

}


/**

  Construct a EWL type 24 warning.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Severity              Severity.

  @retval None

**/

VOID
EFIAPI
EwlOutputType24 (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN EWL_SEVERITY Severity
  )
{
  EWL_ENTRY_TYPE24  Type24;
  PSYSHOST          Host;
  UINT8             Socket = GetSysSocketId ();

  Host = GetSysHostPointer ();

  Type24.Header.Size = sizeof (EWL_ENTRY_TYPE24);
  Type24.Header.Type = EwlType24;
  Type24.Header.Severity = Severity;

  Type24.Context.MajorCheckpoint = GetLastMajorCheckPoint (Socket); // shift to get Major Checkpoint;
  Type24.Context.MinorCheckpoint = GetLastMinorCheckPoint (Socket); // shift to get Minor
  Type24.Context.MajorWarningCode = WarningCode;
  Type24.Context.MinorWarningCode = MinorWarningCode;

  Type24.Socket = Socket;

  RcDebugPrint (SDBG_DEFAULT, "\nMKTME warning major code: 0x%x", Type24.Context.MajorWarningCode);
  RcDebugPrint (SDBG_DEFAULT, "\nMKTME warning minor code: 0x%x", Type24.Context.MinorWarningCode);

  //
  // Log the warning
  //

  EwlLogEntry (&Type24.Header);

  return;

} // EwlOutputType24

/**
  Print Health Check warning to serial port

  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  MemHealthCheckWarning        Health check warning strucutre.

  @retval None

**/
VOID
PrintBootHealthCheckWarning (
  IN UINT8             MinorWarningCode,
  IN EWL_ENTRY_TYPE25 *MemHealthCheckWarning
  )
{
  CHAR8 *GroupString;

  RcDebugPrint (SDBG_DEFAULT, "\nMemory Boot Health Check Log -- Start\n");
  if (MinorWarningCode == WARN_MEMORY_BOOT_HEALTH_CHECK_MASK_FAIL) {
    EwlPrintMemoryLocation (MemHealthCheckWarning->MemoryLocation);
    RcDebugPrint (SDBG_DEFAULT, "\nGroup =");
    if (EFI_ERROR (GetGroupString (MemHealthCheckWarning->Group, &GroupString))) {
      GroupString = "Unknown group";
    }
    RcDebugPrint (SDBG_DEFAULT, GroupString);
    RcDebugPrint (SDBG_DEFAULT, "Offset = %d\n", MemHealthCheckWarning->Offset);
  } else if (MinorWarningCode == WARN_MEMORY_BOOT_HEALTH_CHECK_CHANNEL_MAP_OUT) {
    RcDebugPrint (SDBG_DEFAULT, "Socket:%d Channel:%d was mapped out.\n",
      MemHealthCheckWarning->MemoryLocation.Socket,
      MemHealthCheckWarning->MemoryLocation.Channel);
  }

  RcDebugPrint (SDBG_DEFAULT, "Memory Boot Health Check Log -- End\n");
}

/**

  Construct a EWL type 25 warning. This warning logs BOOT Health check errors.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  MemoryLocation        Memory location where the error occured.
  @param[in]  Severity              Severity.
  @param[in]  Group                 Group.
  @param[in]  Offset                Margin Offset.

  @retval None

**/
VOID
EFIAPI
EwlOutputType25 (
  IN UINT8                      WarningCode,
  IN UINT8                      MinorWarningCode,
  IN EWL_ENTRY_MEMORY_LOCATION  MemoryLocation,
  IN EWL_SEVERITY               Severity,
  IN MRC_GT                     Group,
  IN INT16                      Offset
  )
{
  EFI_STATUS Status;
  PSYSHOST Host;
  UINT8 SocketId;
  EWL_ENTRY_TYPE25    MemHealthCheckWarning;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  Host = GetSysHostPointer ();
  SocketId = GetSysSocketId ();

  MemHealthCheckWarning.Header.Size                = sizeof(EWL_ENTRY_TYPE25);
  MemHealthCheckWarning.Header.Type                = EwlType25;
  MemHealthCheckWarning.Header.Severity            = Severity;

  MemHealthCheckWarning.Context.MajorCheckpoint    = GetLastMajorCheckPoint (SocketId); // shift to get Major Checkpoint;
  MemHealthCheckWarning.Context.MinorCheckpoint    = GetLastMinorCheckPoint (SocketId); // shift to get Minor
  MemHealthCheckWarning.Context.MajorWarningCode   = WarningCode;
  MemHealthCheckWarning.Context.MinorWarningCode   = MinorWarningCode;

  MemHealthCheckWarning.MemoryLocation.Socket  = MemoryLocation.Socket;
  MemHealthCheckWarning.MemoryLocation.Channel = MemoryLocation.Channel;
  MemHealthCheckWarning.MemoryLocation.Dimm    = MemoryLocation.Dimm;
  MemHealthCheckWarning.MemoryLocation.Rank    = MemoryLocation.Rank;
  MemHealthCheckWarning.Group                  = Group;
  MemHealthCheckWarning.Offset                 = Offset;

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformOutputWarning (WarningCode, MinorWarningCode, MemoryLocation.Socket, MemoryLocation.Channel, MemoryLocation.Dimm, MemoryLocation.Rank);
  }

  //
  // Output warning details to the Serial Log
  //
  PrintBootHealthCheckWarning (MinorWarningCode, &MemHealthCheckWarning);

  //
  // Log the warning
  //

  EwlLogEntry (&MemHealthCheckWarning.Header);

  return;

} // EwlOutputType 25

/**
  Construct a EWL type 26 warning. This warning logs Memory Power Management errors.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  MemoryLocation        Memory location where the error occured.
  @param[in]  Severity              Severity.

  @retval None

**/
VOID
EFIAPI
EwlOutputType26  (
  IN UINT8                      WarningCode,
  IN UINT8                      MinorWarningCode,
  IN EWL_ENTRY_MEMORY_LOCATION  MemoryLocation,
  IN EWL_SEVERITY               Severity
  )
{
  EWL_ENTRY_TYPE26  Type26;
  PSYSHOST          Host;
  UINT8             Socket = GetSysSocketId ();

  Host = GetSysHostPointer ();

  Type26.Header.Size = sizeof (EWL_ENTRY_TYPE26);
  Type26.Header.Type = EwlType26;
  Type26.Header.Severity = Severity;

  Type26.Context.MajorCheckpoint = GetLastMajorCheckPoint (Socket); // shift to get Major Checkpoint;
  Type26.Context.MinorCheckpoint = GetLastMinorCheckPoint (Socket); // shift to get Minor
  Type26.Context.MajorWarningCode = WarningCode;
  Type26.Context.MinorWarningCode = MinorWarningCode;

  Type26.MemoryLocation.Socket  = MemoryLocation.Socket;
  Type26.MemoryLocation.Channel = MemoryLocation.Channel;
  Type26.MemoryLocation.Dimm    = MemoryLocation.Dimm;
  Type26.MemoryLocation.Rank    = MemoryLocation.Rank;

  //
  // Output warning details to the Serial Log
  //
  RcDebugPrint (SDBG_DEFAULT, "\nMemory Power Management - Warning major code: 0x%x", Type26.Context.MajorWarningCode);
  RcDebugPrint (SDBG_DEFAULT, "\nMemory Power Management - Warning minor code: 0x%x", Type26.Context.MinorWarningCode);
  RcDebugPrint (SDBG_DEFAULT, "\nMemory Power Management - Error Location Socket = %d, Channel = %d, Dimm = %d, Rank = %d\n",
                  Type26.MemoryLocation.Socket,
                  Type26.MemoryLocation.Channel,
                  Type26.MemoryLocation.Dimm,
                  Type26.MemoryLocation.Rank
                );

  EwlLogEntry (&Type26.Header);

}

/**

  Construct a EWL type 27 warning. This warning logs NVMDIMM Media errors.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Log                   Log.

  @retval None

**/
void
EwlOutputType27 (
  UINT8               WarningCode,
  UINT8               MinorWarningCode,
  UINT8               Socket,
  UINT8               Ch,
  UINT8               Dimm,
  UINT8               Log[32]
  )
{
  EFI_STATUS Status;
  EWL_ENTRY_TYPE27  type27;
  PSYSHOST          Host;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  Host = GetSysHostPointer ();

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformOutputWarning (WarningCode, MinorWarningCode, Socket, Ch, Dimm, 0);
  }

  type27.Header.Size     = sizeof(EWL_ENTRY_TYPE27);
  type27.Header.Type     = EwlType27;
  type27.Header.Severity = EwlSeverityInfo;
  type27.Context.MajorWarningCode = WarningCode;
  type27.Context.MinorWarningCode = MinorWarningCode;

  type27.MemoryLocation.Socket  = Socket;
  type27.MemoryLocation.Channel = Ch;
  type27.MemoryLocation.Dimm    = Dimm;
  type27.MemoryLocation.Rank    = 0;

  type27.TimeStamp              = *((UINT64*)&Log[0]);
  type27.DPA                    = *((UINT64*)&Log[8]);
  type27.PDA                    = *((UINT64*)&Log[16]);
  type27.Range                  = *((UINT8*)&Log[24]);
  type27.ErrorType              = *((UINT8*)&Log[25]);
  type27.ErrorFlag              = *((UINT8*)&Log[26]);
  type27.TransacationType       = *((UINT8*)&Log[27]);
  type27.SequenceNumber         = *((UINT16*)&Log[28]);
  type27.Rsvd                   = *((UINT16*)&Log[30]);

  //
  // Log the warning
  //

  EwlLogEntry (&type27.Header);

  return;

} // EwlOutputType27

/**

  Construct a EWL type 28 warning. This warning logs NVMDIMM Thermal errors.

  @param[in]  WarningCode           Major warning code.
  @param[in]  MinorWarningCode      Minor warning code.
  @param[in]  Socket                Socket.
  @param[in]  Channel               Channel.
  @param[in]  Dimm                  Dimm.
  @param[in]  Log                   Log.

  @retval None

**/
void
EwlOutputType28 (
  UINT8               WarningCode,
  UINT8               MinorWarningCode,
  UINT8               Socket,
  UINT8               Ch,
  UINT8               Dimm,
  UINT8               Log[16]
  )
{
  EFI_STATUS Status;
  EWL_ENTRY_TYPE28  type28;
  PSYSHOST          Host;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  Host = GetSysHostPointer ();

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformOutputWarning (WarningCode, MinorWarningCode, Socket, Ch, Dimm, 0);
  }

  type28.Header.Size     = sizeof(EWL_ENTRY_TYPE28);
  type28.Header.Type     = EwlType28;
  type28.Header.Severity = EwlSeverityInfo;
  type28.Context.MajorWarningCode = WarningCode;
  type28.Context.MinorWarningCode = MinorWarningCode;

  type28.MemoryLocation.Socket  = Socket;
  type28.MemoryLocation.Channel = Ch;
  type28.MemoryLocation.Dimm    = Dimm;
  type28.MemoryLocation.Rank    = 0;

  type28.TimeStamp              = *((UINT64*)&Log[0]);
  type28.HostReportedTempData   = *((UINT32*)&Log[8]);
  type28.SequenceNumber         = *((UINT16*)&Log[10]);
  type28.Rsvd                   = *((UINT16*)&Log[12]);

  //
  // Log the warning
  //

  EwlLogEntry (&type28.Header);

  return;

} // EwlOutputType28

/**

  Initialize the enhanced warning log.

  This function will initialize the enhanced warning log public
  and private data structures.

  @param  - None

  @retval - SUCCESS

**/

UINT32
EFIAPI
EwlInit (
  VOID
  )
{
  PSYSHOST Host;
  EWL_PRIVATE_DATA      *ewlPrivateData;
  EWL_HEADER            *Header;
  EFI_GUID              EwlGuid = EWL_REVISION1_GUID;

  Host           = GetSysHostPointer ();
  ewlPrivateData = &Host->var.common.ewlPrivateData;
  Header         = &ewlPrivateData->status.Header;

  //
  // Initialize the EWL Header
  //
  CopyMem ((UINT8*) &Header->EwlGuid, (UINT8*) &EwlGuid, sizeof (EFI_GUID));
  Header->Size = sizeof (EWL_PUBLIC_DATA);
  Header->FreeOffset = 0;
  Header->Reserved = 0;  // Required by spec to be initialized to 0

  //
  // It is not strictly necessary to initialize the Buffer to a known state
  //

  //
  // Calculate Crc
  //
  Header->Crc = CalculateCrc32 ((VOID *) Header, Header->Size);

  //
  // Initialize some implementation private data helpers
  //
  ewlPrivateData->bufSizeOverflow = 0;
  ewlPrivateData->numEntries = 0;

  return SUCCESS;

} // EwlInit

