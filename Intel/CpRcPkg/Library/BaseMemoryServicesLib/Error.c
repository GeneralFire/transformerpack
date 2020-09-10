/** @file

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

#include <Library/MemoryCoreLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CteNetLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemRcLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/KtiApi.h>
#include <Library/UsraCsrLib.h>
#include <ScratchpadList.h>
#include <Library/MemMcIpLib.h>
#include <Library/BasicIpBlockServicesLib.h>
#include <Library/OemPlatformCheckpointLib.h>
#include <Library/SimulationServicesLib.h>

#include <Guid/MrcPlatformHooksGuid.h>

//
// Local Prototypes
//
VOID
Exit (
  UINT8 majorCode,
  UINT8 minorCode
  );

UINT32
SkipOverFlowUnderFlowCheckForCoSim(
IN  UINT8 WarningCode,
IN  UINT8 MinorWarningCode
);

/**

  Log warning to the warning log for simulation

  @param[in]  WarningCode       - Major warning code
  @param[in]  MinorWarningCode  - Minor warning code
  @param[in]  Socket            - socket number
  @param[in]  Ch                - channel number
  @param[in]  Dimm              - dimm number
  @param[in]  Rank              - rank number

  @retval SUCCESS
**/
UINT32
EFIAPI
OutputWarningSim (
  IN    UINT8    WarningCode,
  IN    UINT8    MinorWarningCode,
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Dimm,
  IN    UINT8    Rank
  )
{

  CteReportError ((WarningCode << 16) | MinorWarningCode, "Warning");

  return SUCCESS;
}

/**

  Log warning to the warning log

  @param warningCode       - Major warning code
  @param minorWarningCode  - Minor warning code
  @param socket              - socket number
  @param ch                - channel number
  @param dimm              - dimm number
  @param rank              - rank number

**/
VOID
EFIAPI
OutputWarning (
  UINT8    warningCode,
  UINT8    minorWarningCode,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank
  )
{
  UINT32 status = FAILURE;

  status = SkipOverFlowUnderFlowCheckForCoSim(warningCode, minorWarningCode);

  if (status == SUCCESS) {
    return;
  }
  //Create enhanced warning of type 1 and store in EWL structure
  EwlOutputType1 (warningCode, minorWarningCode, socket, ch, dimm, rank);

  OutputWarningSim (warningCode, minorWarningCode, socket, ch, dimm, rank);
}


/**

  Log a warning to the Host structure legacy warning log

  @param majorCode - Major error code to log
  @param minorCode - Minor error code to log
  @param logData   - 32 bit error specific data to log

  @retval N/A

**/
VOID
EFIAPI
LogWarning (
  UINT8    majorCode,
  UINT8    minorCode,
  UINT32   logData
  )
{

}


/**

  Log Error to the warning log

  @param ErrorCode         - Major warning code
  @param minorErrorCode    - Minor warning code
  @param socket            - socket number
  @param ch                - channel number
  @param dimm              - dimm number
  @param rank              - rank number

**/
VOID
EFIAPI
OutputError (
  UINT8    ErrorCode,
  UINT8    minorErrorCode,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank
  )
{
  EFI_STATUS Status;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformOutputError (ErrorCode, minorErrorCode, socket, ch, dimm, rank);
  }

  //
  // Log the error
  //

  LogError (ErrorCode, minorErrorCode, (socket << 24) | (ch << 16) | (dimm << 8) | rank);
}

/**

  Log an Error to the Host structure

  @param majorCode - Major error code to log
  @param minorCode - Minor error code to log
  @param Socket    - Socket error occurred on
  @param BistMask  - 64 bit BIST Mask results

  @retval N/A

**/
VOID
EFIAPI
LogCpuError (
  UINT8    majorCode,
  UINT8    minorCode,
  UINT8    Socket,
  UINT64   BistMask
  )
{
  EFI_STATUS Status;
  UINT32  SktBistMaskLo;
  UINT32  SktBistMaskHi;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  //
  // Provide Socket & BIST failure information for legacy and platform
  // implementations.
  // BITS[31]    - 0 for cores 0-27 or 1 for cores 28-55
  // BITS[30-28] - Socket Index of error
  // BITS[27-0]  - Bitmap of failed cores
  //
  SktBistMaskLo = ((UINT32) BistMask) & 0x0FFFFFFF;
  if (SktBistMaskLo != 0) {
    SktBistMaskLo |= ((Socket & 0x07) << 28);
  }
  SktBistMaskHi = ((UINT32) RShiftU64 (BistMask, 28)) & 0x0FFFFFFF;
  if (SktBistMaskHi != 0) {
    SktBistMaskHi |= BIT31; // inidicate high cores
    SktBistMaskHi |= ((Socket & 0x07) << 28);
  }

  //
  // Append errorCode to the status log
  //

  EwlOutputType7 (majorCode, minorCode, Socket, BistMask);

  //
  // Call platform hook to handle warnings
  //

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {

    if (SktBistMaskLo != 0) {
      MrcPlatformHooksInterface->PlatformLogWarning (majorCode, minorCode, SktBistMaskLo);
    }
    if (SktBistMaskHi != 0) {
      MrcPlatformHooksInterface->PlatformLogWarning (majorCode, minorCode, SktBistMaskHi);
    }

  }

  RcDebugPrint (
    SDBG_DEFAULT,
    "\nAn Error logged! Error Code = 0x%X, Minor Error Code = 0x%X, Socket = 0x%X, Data = 0x%X%08X\n",
    majorCode,
    minorCode,
    Socket,
    (UINT32) RShiftU64 (BistMask, 32),
    (UINT32) BistMask
    );
}

/**

  Log a warning to the Host structure

  @param majorCode - Major error code to log
  @param minorCode - Minor error code to log
  @param socket    - Socket

  @retval VOID

**/
VOID
EFIAPI
LogCpuWarning (
  UINT8    majorCode,
  UINT8    minorCode,
  UINT8    Socket
  )
{
  LogWarning (majorCode, minorCode, Socket);
}

/**

  Log an Error to the Host structure

  @param majorCode - Major error code to log
  @param minorCode - Minor error code to log
  @param logData   - 32 bit error specific data to log

  @retval N/A

**/
VOID
EFIAPI
LogError (
  UINT8    majorCode,
  UINT8    minorCode,
  UINT32   logData
  )
{
  EFI_STATUS Status;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  //
  // Append errorCode to the status log
  //

  EwlOutputType1 (majorCode, minorCode, (UINT8) (logData >> 24), (UINT8) (logData >> 16), (UINT8) (logData >> 8), (UINT8) logData);

  //
  // Call platform hook to handle warnings
  //

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformLogWarning (majorCode, minorCode, logData);
  }

  RcDebugPrint (
    SDBG_DEFAULT,
    "\nAn Error logged! Error Code = 0x%X, Minor Error Code = 0x%X, Data = 0x%X\n",
    majorCode,
    minorCode,
    logData
    );

  if (((logData >> 24) & 0xFF) != 0xFF) {
    RcDebugPrint (SDBG_DEFAULT, "S%d", (logData >> 24) & 0xFF);
  }

  if (((logData >> 16) & 0xFF) != 0xFF) {
    RcDebugPrint (SDBG_DEFAULT, " Ch%d", (logData >> 16) & 0xFF);
  }

  if (((logData >> 8) & 0xFF) != 0xFF) {
    RcDebugPrint (SDBG_DEFAULT, " DIMM%d", (logData >> 8) & 0xFF);
  }

  if ((logData & 0xFF) != 0xFF) {
    RcDebugPrint (SDBG_DEFAULT, " Rank%d", logData & 0xFF);
  }

  RcDebugPrint (SDBG_DEFAULT, "\n\n");

  //
  // Promote fatal error
  //
  FatalError (majorCode, minorCode);
}

/**

  Halts the platform

  @param majorCode - Major error code to log
  @param minorCode - Minor error code to log

  @retval N/A

**/
VOID
EFIAPI
HaltOnError (
  UINT8    majorCode,
  UINT8    minorCode
  )
{
  UINT8   haltOnError;
  SYS_SETUP *Setup;
  PSYSHOST Host;

  Host = GetSysHostPointer ();
  Setup = GetSysSetupPointer ();

  //
  // Report error if this is a CTE build, otherwise do nothing.
  //

  CteReportError ((majorCode << 8) | minorCode, "SIM_BUILD/HMRC error!");

  //
  // Default halt on error based on setup option
  //
  if (Setup->common.options & HALT_ON_ERROR_EN) {
    haltOnError = 1;
  } else {
    haltOnError = 0;
  }

#ifdef DEBUG_CODE_BLOCK
  //
  // Check Scratchpad0 BIT5 - ResetFatalErr for next reset or halt-on
  //
  if (Setup->common.options & HALT_ON_ERROR_AUTO) {
    if ((FatalErrSetMaxSDBGSelect (Host, MAX_SDBG_FATAL_ERR_CHECK)) || (IsDcpmmPresentOnPlatform ())) {
      haltOnError = 1;
    } else {
      //
      // Request and do warm reset
      //
      SetSysResetRequired (POST_RESET_WARM);
      CheckAndHandleResetRequests (Host);
    }
  }
#endif //DEBUG_CODE_BLOCK

  //
  // Loop forever
  //
  RcDeadLoop (haltOnError);

  //
  // Exit now!
  //
  Exit (majorCode, minorCode);

} // HaltOnError


/**

  Exits reference code

  @param majorCode - Major error code to log
  @param minorCode - Minor error code to log

  @retval N/A

**/
VOID
Exit (
  UINT8    majorCode,
  UINT8    minorCode
  )
{
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  // TODO: change this flow to use gcc inline asm and take common exit path with MS build targets
  // For now replicate RcSim steps that CTE needs
  //INT32 status = (majorCode << 8) | minorCode;
  //DisconnectFromRemoteHost();
  //exit(-status);

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    return;
  }

  //
  // If this is RcSim, exit to the OS, otherwise 
  // do nothing. Since we don't really know the RcSim
  // specific code, return generic -1.
  //

  ExitRcSim (-1);

  LongJump ((BASE_LIBRARY_JUMP_BUFFER *)(UINTN) Host->var.common.JumpBuffer, (majorCode << 8) | minorCode);

}

/**

  Adds error/exception to the promoteWarningList in Host structure if possible

  @param majorCode - Major error code to log
  @param minorCode - Minor error code to log

  @retval 0 - Successfully added to log
  @retval 1 - Log full

**/
UINT32
EFIAPI
SetPromoteWarningException (
  UINT8    majorCode,
  UINT8    minorCode
  )
{
  UINT32    status;
  UINT32    i;
  PSYSHOST  Host;
  UINT16    WarningCode;

  Host = GetSysHostPointer ();

  status = SUCCESS;
  //
  // Search warning list for either duplicate or unused entry
  //
  for (i = 0; i < MAX_PROMOTE_WARN_LIMIT; i++) {
    WarningCode = (majorCode << 8) | minorCode;

    if (Host->var.common.promoteWarnList[i] == WarningCode) {
      // The requested warning code is already in the list.
      break;

    } else if (Host->var.common.promoteWarnList[i] == 0) {
      // Add the requested warning code to the list.
      Host->var.common.promoteWarnList[i] = WarningCode;
      if (i == Host->var.common.promoteWarnLimit) {
        Host->var.common.promoteWarnLimit++;
      }
      break;
    }
  }

  if (i == MAX_PROMOTE_WARN_LIMIT) {
    RcDebugPrint (SDBG_DEFAULT, "SetPromoteWarningException: Out of storage\n");
    FatalError (ERR_MRC_STRUCT, ERR_ARRAY_OUT_OF_BOUNDS);
    status = FAILURE;
  }

  return status;
}

/**

  Removes a warning from the promoteWarningList in Host structure if present

  @param majorCode - Major error code to log
  @param minorCode - Minor error code to log

  @retval 0 - Successfully cleared from log
  @retval 1 - Warning/Error not in log

**/
UINT32
EFIAPI
ClearPromoteWarningException (
  UINT8    majorCode,
  UINT8    minorCode
  )
{
  UINT32  status;
  UINT32  i;
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  status = SUCCESS;
  //
  // Search warning list for unused entry
  //
  for (i = 0; i < MAX_PROMOTE_WARN_LIMIT; i++) {
    if (Host->var.common.promoteWarnList[i] == (UINT16) ((majorCode << 8) | minorCode)) {
      Host->var.common.promoteWarnList[i] = 0;
      if (i == Host->var.common.promoteWarnLimit) {
        Host->var.common.promoteWarnLimit--;
      }
      break;
    }
  }

  if (i == MAX_PROMOTE_WARN_LIMIT) {
    status = FAILURE;
  }

  return status;
}

/*++

  Routine Description:

    Checks that index is less than arraySize to avoid array out of bounds access

  Arguments:

    index
    arraySize

  Returns:

    N/A


--*/

VOID
EFIAPI
MemCheckIndex (
  UINT32 index,
  UINT32 arraySize
  )

{
  if (index >= arraySize) {

#ifdef DEBUG_CODE_BLOCK

    RcDebugPrint (SDBG_MINMAX,
                  "\nERROR: array out of bounds\n");

#endif //DEBUG_CODE_BLOCK

    FatalError (ERR_MRC_STRUCT, ERR_ARRAY_OUT_OF_BOUNDS);

  }

} // MemCheckIndex

/**

  Outputs error code hand halts if halt on fatal error, else exits reference code

  @param majorCode - Major error code to log
  @param minorCode - Minor error code to log
  @retval N/A


**/
VOID
EFIAPI
FatalError (
  UINT8    majorCode,
  UINT8    minorCode
  )
{
  EFI_STATUS                          Status;
  UINT8                               haltOnError;
  BIOS_NON_STICKY_SCRATCHPAD8_STRUCT  ErrorCodeCsr;
  UINT8                               sbspSktId;
  UINT8                               SocketId;
  SYS_SETUP                           *Setup;
  PSYSHOST                            Host;
  MRC_PLATFORM_HOOKS_INTERFACE        *MrcPlatformHooksInterface = NULL;
  MRC_STATUS                          HookStatus;

  Host = GetSysHostPointer ();

  Setup = GetSysSetupPointer ();

  CteReportError (majorCode << 16 | minorCode, "Error");

  sbspSktId = GetSysSbspSocketIdNv ();
  SocketId = GetSysSocketId ();

  //
  // Read the last checkpoint
  //
  ErrorCodeCsr.Data = GetCheckPoint ();
  //
  // Write the major and minor error codes to the scratchpad register
  //
  ErrorCodeCsr.Bits.MajorErrCode = majorCode;
  ErrorCodeCsr.Bits.SubErrCode = minorCode;
  UsraCsrWrite (sbspSktId, 0, SR_ERROR_CODE_CSR_ADDR (Host), ErrorCodeCsr.Data);

  //
  // Default halt on error based on setup option
  //
  if (Setup->common.options & HALT_ON_ERROR_EN) {
    haltOnError = 1;
  } else {
    haltOnError = 0;

  }

#ifdef DEBUG_CODE_BLOCK
  //
  // Default halt on error with Auto cold reset and set Maximum seial debug message based on setup option
  //
  if (Setup->common.options & HALT_ON_ERROR_AUTO) {
    haltOnError = 1;
  }
#endif //DEBUG_CODE_BLOCK

  //
  // Call platform hook to handle fatal error
  //

  HookStatus = MRC_FUNCTION_NOT_IMPLEMENTED;

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    HookStatus = MrcPlatformHooksInterface->PlatformFatalError (majorCode, minorCode, &haltOnError);
  }

  //
  // If Hooks PlatformCheckpoint returns not implemented, or
  // if the hooks are not present, perform the generic EWL init.
  //

  if (HookStatus == MRC_FUNCTION_NOT_IMPLEMENTED) {

#ifdef IA32

    OemPlatformFatalError (Host, majorCode, minorCode);

    OutPort8 (Setup->common.debugPort,majorCode);

#endif // #ifdef IA32

  }

  if ((Setup->mem.options & MULTI_THREAD_MRC_EN) && (GetSocketPresentBitMap () > 1)) {
    Host->var.mem.socket[SocketId].fatalError = 1;
    Host->var.mem.socket[SocketId].majorCode = majorCode;
    Host->var.mem.socket[SocketId].minorCode = minorCode;
    if (SocketId == GetSysSbspSocketIdNv ()) {
      //
      // Let all the non-BSP's know the BSP has a fatal error
      //
      RcDebugPrint (SDBG_DEFAULT, "\nFatalError: BSP - SocketId = %d registered Major Code = 0x%2x, Minor Code = 0x%2x \n",
                SocketId, majorCode, minorCode);

      //
      // Sync with non-BSP sockets
      //
      SyncErrors();

    } else {
      //
      // Let the BSP know this non-BSP had a fatal error
      //
      RcDebugPrint (SDBG_DEFAULT, "\nFatalError: nonBSP -  SocketId = %d registered Major Code = 0x%2x, Minor Code = 0x%2x \n",
                SocketId, majorCode, minorCode);

      SyncErrors();
    }
  } else {
    RcDebugPrint (SDBG_DEFAULT, "\nFatalError: SocketId = %d registered Major Code = 0x%2x, Minor Code = 0x%2x \n",
              SocketId, majorCode, minorCode);
  }

  //
  // Flush buffer
  //
  FlushDebugBuffer ();

  HaltOnError (majorCode, minorCode);
}

/**

Skips logging a warning for CoSim

@param majorCode - Major error code
@param minorCode - Minor error code
@retval SUCCESS/FAILURE


**/
UINT32
SkipOverFlowUnderFlowCheckForCoSim(
IN  UINT8 WarningCode,
IN  UINT8 MinorWarningCode
)
{
  if (FeaturePcdGet(PcdCosimBuild) == TRUE) {
    if (WarningCode == WARN_REGISTER_OVERFLOW && (MinorWarningCode == WARN_MINOR_REGISTER_OVERFLOW || MinorWarningCode == WARN_MINOR_REGISTER_UNDERFLOW)) {
      return SUCCESS;
    }
  }
  return FAILURE;
}
