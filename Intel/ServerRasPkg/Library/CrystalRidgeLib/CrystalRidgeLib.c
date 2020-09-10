/** @file
  Implementation of memory SSR library.

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

#include <PiSmm.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/RasDebugLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Protocol/CrystalRidge.h>
#include <SiliconSetting.h>
#include "CrystalRidgeLibInternal.h"

CRYSTAL_RIDGE_LIB_INTERNAL_DATA mCrystalRidgeLibData;

/**
  Internal protocol notify function.

  This function is registered to be called when the Crystal Ridge SMM protocol
  is installed. It records the protocol pointer in the library's private data
  to be used by the library interface functions.

  This function is NOT part of the library interface.

  @param[in]  Protocol    Points to the protocol's unique identifier.
  @param[in]  Interface   Points to the interface instance.
  @param[in]  Handle      The handle on which the interface was installed.

  @retval EFI_SUCCESS           The notify function executed successfully.
**/
EFI_STATUS
EFIAPI
CrystalRidgeLibProtocolNotifyFunction (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  )
{
  EFI_STATUS  Status;
  UINTN       Index;

  ASSERT (mCrystalRidgeLibData.Protocol == NULL);
  if (mCrystalRidgeLibData.Protocol == NULL) {
    mCrystalRidgeLibData.Protocol = (EFI_CRYSTAL_RIDGE_PROTOCOL *) Interface;
    RAS_DEBUG ((
      LEVEL_BASIC_FLOW,
      "[%a][CrystalRidgeLib] Notified of Crystal Ridge protocol @ 0x%p\n",
      gEfiCallerBaseName,
      mCrystalRidgeLibData.Protocol
      ));

    //
    // If there are callback functions, call them.
    //
    for (Index = 0; Index < MAX_CRYSTAL_RIDGE_LIB_CALLBACK_FUNCTIONS; Index++) {
      //
      // The first null entry indicates the end of the list.
      //
      if (mCrystalRidgeLibData.CallbackFunctions[Index] == NULL) {
        break;
      }

      RAS_DEBUG ((
        LEVEL_FUNC_FLOW,
        "[%a][CrystalRidgeLib] Calling notify function @ 0x%p\n",
        gEfiCallerBaseName,
        mCrystalRidgeLibData.CallbackFunctions[Index]
        ));
      mCrystalRidgeLibData.CallbackFunctions[Index] ();
      mCrystalRidgeLibData.CallbackFunctions[Index] = NULL;
    }
  }

  ASSERT (mCrystalRidgeLibData.Protocol != NULL);
  ASSERT (mCrystalRidgeLibData.Registration != NULL);
  if (mCrystalRidgeLibData.Protocol != NULL &&
      mCrystalRidgeLibData.Registration != NULL) {
    //
    // Un-register by passing Function = NULL and the same Registration.
    //
    Status = gSmst->SmmRegisterProtocolNotify (
      &gEfiCrystalRidgeSmmGuid,
      NULL,
      &mCrystalRidgeLibData.Registration
      );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      RAS_DEBUG ((
        LEVEL_BASIC_FLOW,
        "[%a][CrystalRidgeLib] Failed to un-register protocol notify: %r\n",
        gEfiCallerBaseName,
        Status
        ));
    } else {
      mCrystalRidgeLibData.Registration = NULL;
    }
  }

  return EFI_SUCCESS;
} // CrystalRidgeLibProtocolNotifyFunction

/**
  Constructor for this library.

  Register a notification function to call when the Crystal Ridge SMM
  protocol is installed.

  This function is NOT part of the library interface.

  @param[in]  ImageHandle   The firmware allocated handle for the EFI image.
  @param[in]  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The constructor executed successfully.
  @retval EFI_OUT_OF_RESOURCES  There is not enough memory to use the library.
**/
EFI_STATUS
EFIAPI
CrystalRidgeLibConstructor (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  EFI_STATUS  Status;
  UINTN       Index;

  //
  // Initialize internal data.
  //
  mCrystalRidgeLibData.Protocol = NULL;
  mCrystalRidgeLibData.Registration = NULL;
  for (Index = 0; Index < MAX_CRYSTAL_RIDGE_LIB_CALLBACK_FUNCTIONS; Index++) {
    mCrystalRidgeLibData.CallbackFunctions[Index] = NULL;
  }

  Status = gSmst->SmmLocateProtocol (
    &gEfiCrystalRidgeSmmGuid,
    NULL,
    (VOID **) &mCrystalRidgeLibData.Protocol
    );
  if (EFI_ERROR (Status)) {
    ASSERT (mCrystalRidgeLibData.Protocol == NULL);
    mCrystalRidgeLibData.Protocol = NULL;
    RAS_DEBUG ((
      LEVEL_BASIC_FLOW,
      "[%a][CrystalRidgeLib] Failed to locate protocol: %r\n",
      gEfiCallerBaseName,
      Status
      ));
  } else {
    ASSERT (mCrystalRidgeLibData.Protocol != NULL);
    RAS_DEBUG ((
      LEVEL_BASIC_FLOW,
      "[%a][CrystalRidgeLib] Located Crystal Ridge protocol @ 0x%p\n",
      gEfiCallerBaseName,
      mCrystalRidgeLibData.Protocol
      ));
  }

  if (mCrystalRidgeLibData.Protocol == NULL) {
    //
    // Register the notification function.
    //
    Status = gSmst->SmmRegisterProtocolNotify (
      &gEfiCrystalRidgeSmmGuid,
      CrystalRidgeLibProtocolNotifyFunction,
      &mCrystalRidgeLibData.Registration
      );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      RAS_DEBUG ((
        LEVEL_BASIC_FLOW,
        "[%a][CrystalRidgeLib] Failed to register protocol notify: %r\n",
        gEfiCallerBaseName,
        Status
        ));
    }
  }

  return Status;
} // CrystalRidgeLibConstructor

/**
  Destructor for this library.

  Un-register the protocol notification function that was registered by the
  constructor.

  This function is NOT part of the library interface.

  @param[in]  ImageHandle   The firmware allocated handle for the EFI image.
  @param[in]  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The destructor executed successfully.
**/
EFI_STATUS
EFIAPI
CrystalRidgeLibDestructor (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;       // assume success

  if (mCrystalRidgeLibData.Registration != NULL) {
    //
    // Un-register by passing Function = NULL and the same Registration.
    //
    Status = gSmst->SmmRegisterProtocolNotify (
      &gEfiCrystalRidgeSmmGuid,
      NULL,
      &mCrystalRidgeLibData.Registration
      );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      RAS_DEBUG ((
        LEVEL_BASIC_FLOW,
        "[%a][CrystalRidgeLib] Failed to un-register protocol notify: %r\n",
        gEfiCallerBaseName,
        Status
        ));
    }
  }

  return Status;
} // CrystalRidgeLibDestructor

/**
  Register a callback function for when the library is ready.

  If the library is already ready when this function is called, then this
  function calls the callback immediately, before returning EFI_SUCCESS.
  Otherwise, the callback function pointer is saved to be called when the
  library becomes ready.

  Consumers of this library that need to access the library interface for
  initialization code should register a callback here. For code running in SMI
  handlers, this is not required.

  @param[in]  Function      The callback function pointer to register.

  @retval EFI_SUCCESS             The registration was successful.
  @retval EFI_INVALID_PARAMETER   The callback function pointer is null.
  @retval EFI_OUT_OF_RESOURCES    There are not enough resources to register.
**/
EFI_STATUS
EFIAPI
RegisterCrystalRidgeReadyCallback (
  IN CRYSTAL_RIDGE_LIB_CALLBACK_FUNCTION Function
  )
{
  UINTN       Index;

  ASSERT (Function != NULL);
  if (Function == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // If the protocol was already found, run the function now. Otherwise, save
  // the function pointer to call later.
  //
  if (mCrystalRidgeLibData.Protocol != NULL) {
    RAS_DEBUG ((
      LEVEL_FUNC_FLOW,
      "[%a][CrystalRidgeLib] Calling notify function @ 0x%p\n",
      gEfiCallerBaseName,
      Function
      ));
    Function ();
  } else {
    //
    // Find a free array entry to add the callback function.
    //
    for (Index = 0; Index < MAX_CRYSTAL_RIDGE_LIB_CALLBACK_FUNCTIONS; Index++) {
      if (mCrystalRidgeLibData.CallbackFunctions[Index] == NULL) {
        mCrystalRidgeLibData.CallbackFunctions[Index] = Function;
        break;
      }
    }
    //
    // If we ever run out of resources, the array size should be increased.
    //
    ASSERT (Index < MAX_CRYSTAL_RIDGE_LIB_CALLBACK_FUNCTIONS);
    if (Index >= MAX_CRYSTAL_RIDGE_LIB_CALLBACK_FUNCTIONS) {
      return EFI_OUT_OF_RESOURCES;
    }
  }

  return EFI_SUCCESS;
} // RegisterCrystalRidgeReadyCallback

BOOLEAN
IsDcpmm (
  IN   UINT8    Skt,
  IN   UINT8    ChOnSkt,
  IN   UINT8    DIMM
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return FALSE;             // not ready
  }

  return mCrystalRidgeLibData.Protocol->IsNvmDimm (Skt,ChOnSkt,DIMM);
}

RETURN_STATUS
EFIAPI
GetCfgDataPolicy (
  IN      UINT8     Socket,
  IN      UINT8     ChOnSkt,
  IN      UINT8     Dimm,
  OUT     UINT32    *Data
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return RETURN_NOT_READY;
  }

  return mCrystalRidgeLibData.Protocol->NvmCtlrGetCfgDataPolicy (Socket, ChOnSkt, Dimm, Data);
}

RETURN_STATUS
EFIAPI
SetCfgDataPolicy (
  IN    UINT8       Socket,
  IN    UINT8       ChOnSkt,
  IN    UINT8       Dimm,
  IN    UINT32      Data
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return RETURN_NOT_READY;
  }

  return mCrystalRidgeLibData.Protocol->NvmCtlrSetCfgDataPolicy (Socket, ChOnSkt, Dimm, Data);
}

RETURN_STATUS
EFIAPI
GetViralPolicy(
  IN     UINT8      Socket,
  IN     UINT8      ChOnSkt,
  IN     UINT8      Dimm,
  OUT    UINT32     *Data
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return RETURN_NOT_READY;
  }
  return mCrystalRidgeLibData.Protocol->NvmCtlrGetViralPolicy (Socket, ChOnSkt, Dimm, Data);
}

RETURN_STATUS
EFIAPI
SetViralPolicy(
  IN    UINT8       Socket,
  IN    UINT8       ChOnSkt,
  IN    UINT8       Dimm,
  IN    UINT32      Data
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return RETURN_NOT_READY;
  }
  return mCrystalRidgeLibData.Protocol->NvmCtlrSetViralPolicy(Socket, ChOnSkt, Dimm, Data);
}

RETURN_STATUS
EFIAPI
GetIdentifyDimm(
  IN    UINT8        Socket,
  IN    UINT8        ChOnSkt,
  IN    UINT8        Dimm,
  OUT   NVDIMM_INFO  *DimmInfo
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return RETURN_NOT_READY;
  }

  return mCrystalRidgeLibData.Protocol->NvmCtlrGetDimmInfo(Socket, ChOnSkt, Dimm, DimmInfo);
}

EFI_STATUS
EFIAPI
SendFnvSetHostAlert (
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16   *hostAlerts
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return EFI_NOT_READY;
  }

  return mCrystalRidgeLibData.Protocol->NvmCtlrSetHostAlert (socket,ch,dimm,hostAlerts);
}


EFI_STATUS
EFIAPI
SendFnvSetAddressRangeScrub (
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT64    DpaStartAddress,
  UINT64    DpaEndAddress,
  UINT8     Enable
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return EFI_NOT_READY;
  }

  return mCrystalRidgeLibData.Protocol->NvmCtlrSetAddressRangeScrub (socket,ch,dimm,DpaStartAddress,DpaEndAddress,Enable);
}

EFI_STATUS
EFIAPI
FnvLongOpComplete (
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return EFI_NOT_READY;
  }

  return  mCrystalRidgeLibData.Protocol->NvmCtlrLongOpComplete (Socket, Ch, Dimm);
}

EFI_STATUS
EFIAPI
FnvLongOpCompleteGet (
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm,
  UINT32   *FisRspPtr,
  UINT32   *SeqNum
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return EFI_NOT_READY;
  }

  return  mCrystalRidgeLibData.Protocol->NvmCtlrLongOpCompleteGet (Socket, Ch, Dimm, FisRspPtr, SeqNum);
}

VOID
EFIAPI
FnvLongOpCompleteProcess (
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm,
  UINT32   *FisRspPtr,
  UINT32   *SeqNum
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return;
  }

  mCrystalRidgeLibData.Protocol->NvmCtlrLongOpCompleteProcess (Socket, Ch, Dimm, FisRspPtr, SeqNum);
}

EFI_STATUS
EFIAPI
FnvGetLatestErrorLog (
  UINT8        Socket,
  UINT8        Ch,
  UINT8        Dimm,
  UINT8        LogLevel,
  UINT8        LogType,
  UINT8        Count,
  ERR_LOG_DATA *LogBuffer
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return EFI_NOT_READY;
  }

  return mCrystalRidgeLibData.Protocol->NvmCtlrGetLatestErrorLog (Socket, Ch, Dimm, LogLevel, LogType, Count, LogBuffer);
}

VOID
EFIAPI
FnvAcpiAddLeafNotification (
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return;                   // not ready
  }

  mCrystalRidgeLibData.Protocol->AcpiAddLeafNotification (Socket, Ch, Dimm);
}


VOID
EFIAPI
FnvAcpiClearLeafNotifications (
  VOID
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return;                   // not ready
  }

  mCrystalRidgeLibData.Protocol->AcpiClearLeafNotifications ();
}

VOID
EFIAPI
FnvAcpiLeafNotify (
  VOID
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return;                   // not ready
  }

  mCrystalRidgeLibData.Protocol->AcpiLeafNotify ();
}

VOID
EFIAPI
FnvAcpiClearRootNotification (
  VOID
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return;                   // not ready
  }

  mCrystalRidgeLibData.Protocol->AcpiClearRootNotification ();
}

VOID
EFIAPI
FnvAcpiAddRootNotification (
  UINT8  Socket,
  UINT8  Ch,
  UINT8  Dimm,
  UINT64 Dpa,
  UINT8  RangeExponent)
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return;                   // not ready
  }

  mCrystalRidgeLibData.Protocol->AcpiAddRootNotification (Socket, Ch, Dimm, Dpa, RangeExponent);
}

VOID
EFIAPI
FnvAcpiRootNotify (
  VOID
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return;                   // not ready
  }

  mCrystalRidgeLibData.Protocol->AcpiRootNotify ();
}

VOID
EFIAPI
FnvAcpiAddToErrorList (
  UINT8  Socket,
  UINT8  Ch,
  UINT8  Dimm,
  UINT64 Dpa,
  UINT8  RangeExponent)
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return;                   // not ready
  }

  mCrystalRidgeLibData.Protocol->AcpiAddToErrorList (Socket, Ch, Dimm, Dpa, RangeExponent);
}

BOOLEAN
IsDDRTAddress (
  UINT64 Spa
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return FALSE;             // not ready
  }

  return mCrystalRidgeLibData.Protocol->IsNvmAddress (Spa);
}

EFI_STATUS
SpaToDpa (
  UINT64       Spa,
  UINT8        *Skt,
  UINT8        *Ch,
  UINT8        *Dimm,
  UINT64       *pDpa
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return EFI_NOT_READY;
  }

  return mCrystalRidgeLibData.Protocol->SpaToNvmDpa (Spa,Skt,Ch,Dimm,pDpa);
}


EFI_STATUS
EFIAPI
NvmCtlrEnableErrInjection (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return EFI_NOT_READY;
  }

  return mCrystalRidgeLibData.Protocol->NvmCtlrEnableErrInjection (Socket, Ch, Dimm);
}

EFI_STATUS
EFIAPI
NvmCtlrDisableErrInjection (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return EFI_NOT_READY;
  }

  return mCrystalRidgeLibData.Protocol->NvmCtlrDisableErrInjection (Socket, Ch, Dimm);
}

EFI_STATUS
EFIAPI
NvmCtlrPlatformEnableErrInjection (
  VOID
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return EFI_NOT_READY;
  }

  return mCrystalRidgeLibData.Protocol->NvmCtlrPlatformEnableErrInjection ();
}


/**

  Routine Description: this function injects an error for a specified
  memory type at a particular device physical address.

  @param[in]  Socket        - Socket Number
  @param[in]  Ch            - DDR Channel ID
  @param[in]  Dimm          - DIMM number
  @param[in]  Dpa           - DPA of the device
  @param[in]  MemoryType    - Type of memory mapped to DPA
  @param[in]  Enable        - Enable/disable poison for this address

  @return EFI_STATUS - success if command sent

**/
EFI_STATUS
NvmCtlrInjectPoisonError (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT64    Dpa,
  IN  UINT8     MemoryType,
  IN  UINT8     Enable
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return EFI_NOT_READY;
  }

  return mCrystalRidgeLibData.Protocol->NvmCtlrInjectPoisonError (Socket, Ch, Dimm, Dpa, MemoryType, Enable);
}

/**
  @brief Retrieves specified error log starting at specified sequence number.

  Stores new sequence number (to be used with subsequent calls) in SequenceNumber.

  @param[in]      Socket                 Socket index
  @param[in]      Ch                     DDR channel index
  @param[in]      Dimm                   DIMM index
  @param[in]      LogType                Log type
                                         MEDIA_LOG_REQ for media log
                                         THERMAL_LOG_REQ for thermal log
  @param[in]      LogLevel               Log level, LOW_LEVEL_LOG or HIGH_LEVEL_LOG
  @param[in,out]  SequenceNumber         Pointer to sequence number to start retrieving log entries from.
                                         When function returns EFI_SUCCESS or EFI_NOT_FOUND, sequence number of next
                                         event is stored at location pointed by SequenceNumber.
  @param[out]     ErrLog                 Pointer to ERR_LOG_DATA union to store retrieved log entries in.

  @retval         EFI_SUCCESS            Success.
  @retval         EFI_INVALID_PARAMETER  Invalid Log Type or Log Level.
                                         SequenceNumber or ErrLog is NULL.
                                         Specified DIMM is not found.
  @retval         EFI_NOT_FOUND          No log matching *SequenceNumber found.
  @retval         MailboxStatusDecode    Error returned by MailboxStatusDecode.
**/
EFI_STATUS
EFIAPI
NvmCtlrGetErrorLog (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
  IN     UINT8        LogType,
  IN     UINT8        LogLevel,
  IN     BOOLEAN      CurrentBoot,
  IN OUT UINT16       *SequenceNumber,
     OUT ERR_LOG_DATA *ErrLog
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return EFI_NOT_READY;
  }

  return mCrystalRidgeLibData.Protocol->NvmCtlrGetErrorLog (Socket, Ch, Dimm, LogType, LogLevel, CurrentBoot, SequenceNumber, ErrLog);
}

/*

  Routine Description: Does this system have at least one NVMDIMM
  CR interface protocol function.

  @return TRUE if NVMDIMM present

**/
BOOLEAN
EFIAPI
IsSystemWithDcpmm (
  VOID
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return FALSE;
  }
  return mCrystalRidgeLibData.Protocol->IsSystemWithDcpmm();
}

/**

  This function returns True if the channel has a DDRT dimm present

  @param[in] Socket    - Socket ID
  @param[in] Ch       - Channel on Socket

  @retval TRUE/FALSE

**/
BOOLEAN
IsDcpmmOnCh (
  IN UINT8 Socket,
  IN UINT8 ChOnSkt
  )
{
  UINT8 Dimm;

  if (mCrystalRidgeLibData.Protocol == NULL) {
    return FALSE;
  }

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if (mCrystalRidgeLibData.Protocol->IsNvmDimm (Socket, ChOnSkt, Dimm))
      return TRUE;
  }
  return FALSE;
}

/**
  Notify Crystal Ridge driver of Host Alert Configuration

  @param[in]  HighPrioritySmi  TRUE: High Priority Alert SMI enabled
  @param[in]  LowPrioritySmi   TRUE: Low Priority Alert SMI enabled

**/
VOID
FnvNotifyHostAlertConfiguration (
  IN BOOLEAN HighPrioritySmi,
  IN BOOLEAN LowPrioritySmi
  )
{
  if (mCrystalRidgeLibData.Protocol == NULL) {
    return;
  }

  mCrystalRidgeLibData.Protocol->NotifyHostAlertConfiguration (HighPrioritySmi, LowPrioritySmi);
}
