/** @file
  Interface of SMM Crystal Ridge Library

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

#ifndef __CRYSTAL_RIDGE_LIB_H__
#define __CRYSTAL_RIDGE_LIB_H__

#include <Protocol/CrystalRidge.h>

typedef struct _HOST_ALERT_POLICY {
  UINT8   LogLevel:2;                       ///< 0 - do not log, 1 - low priority, 2 - high priority
  UINT8   Rsvd:1;
  UINT8   Rsvd0:1;
  UINT8   DpaErrViralEn:1;
  UINT8   DpaErrInterruptEn:1;
  UINT8   UncErrViralEn:1;
  UINT8   UncErrInterruptEn:1;
  UINT8   DataPathErrInterruptEn:1;
  UINT8   DataPathErrViralEn:1;
  UINT8   IllegalAccessInterruptEn:1;
  UINT8   IllegalAccessViralEn:1;
  UINT8   Rsvd1:1;
  UINT8   Rsvd2:1;
  UINT8   Rsvd3:1;
  UINT8   Rsvd4:1;
} HOST_ALERT_POLICY;

/**
  Function prototype for library callbacks.
**/
typedef
VOID
(EFIAPI *CRYSTAL_RIDGE_LIB_CALLBACK_FUNCTION) (
  VOID
  );

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
  );

BOOLEAN
IsDcpmm (
  IN   UINT8    Skt,
  IN   UINT8    ChOnSkt,
  IN   UINT8    Dimm
  );

RETURN_STATUS
EFIAPI
GetCfgDataPolicy (
  IN      UINT8     Socket,
  IN      UINT8     ChOnSkt,
  IN      UINT8     Dimm,
  OUT     UINT32    *Data
  );



RETURN_STATUS
EFIAPI
SetCfgDataPolicy (
  IN    UINT8     Socket,
  IN    UINT8     ChOnSkt,
  IN    UINT8     Dimm,
  IN    UINT32    Data
  );

RETURN_STATUS
EFIAPI
GetViralPolicy (
  IN    UINT8     Socket,
  IN    UINT8     ChOnSkt,
  IN    UINT8     Dimm,
  OUT   UINT32    *Data
  );

RETURN_STATUS
EFIAPI
SetViralPolicy (
  IN    UINT8     Socket,
  IN    UINT8     ChOnSkt,
  IN    UINT8     Dimm,
  IN    UINT32    Data
  );

RETURN_STATUS
EFIAPI
GetIdentifyDimm (
  IN    UINT8        Socket,
  IN    UINT8        ChOnSkt,
  IN    UINT8        Dimm,
  OUT   NVDIMM_INFO  *DimmInfo
  );

EFI_STATUS
EFIAPI
SendFnvSetHostAlert (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT16   *HostAlerts
  );

EFI_STATUS
EFIAPI
SendFnvSetAddressRangeScrub (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT64    DpaStartAddress,
  UINT64    DpaEndAddress,
  UINT8     Enable
  );

EFI_STATUS
EFIAPI
FnvLongOpComplete (
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm
  );

EFI_STATUS
EFIAPI
FnvLongOpCompleteGet (
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm,
  UINT32   *FisRspPtr,
  UINT32   *SeqNum
  );

VOID
EFIAPI
FnvLongOpCompleteProcess (
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm,
  UINT32   *FisRspPtr,
  UINT32   *SeqNum
  );

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
  );

VOID
EFIAPI
FnvAcpiAddLeafNotification (
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm
  );

VOID
EFIAPI
FnvAcpiClearLeafNotifications (
  VOID
  );

VOID
EFIAPI
FnvAcpiLeafNotify (
  VOID
  );

VOID
EFIAPI
FnvAcpiClearRootNotification (
  VOID
  );

VOID
EFIAPI
FnvAcpiAddRootNotification (
  UINT8  Socket,
  UINT8  Ch,
  UINT8  Dimm,
  UINT64 Dpa,
  UINT8  RangeExponent
  );

VOID
EFIAPI
FnvAcpiRootNotify (
  VOID
  );

VOID
EFIAPI
FnvAcpiAddToErrorList (
  UINT8  Socket,
  UINT8  Ch,
  UINT8  Dimm,
  UINT64 Dpa,
  UINT8  RangeExponent
  );

BOOLEAN
IsDDRTAddress (
  UINT64 Spa
  );

EFI_STATUS
SpaToDpa (
  UINT64       Spa,
  UINT8        *Skt,
  UINT8        *Ch,
  UINT8        *Dimm,
  UINT64       *Dpa
  );


EFI_STATUS
EFIAPI
NvmCtlrEnableErrInjection (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  );

EFI_STATUS
EFIAPI
NvmCtlrDisableErrInjection (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  );

EFI_STATUS
EFIAPI
NvmCtlrPlatformEnableErrInjection (
  VOID
  );

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
  );

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
  );

/*

  Routine Description: Does this system have at least one NVMDIMM

  @return TRUE if NVMDIMM present

**/
BOOLEAN
EFIAPI
IsSystemWithDcpmm (
  VOID
  );

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
  );

/**
  Notify Crystal Ridge driver of Host Alert Configuration

  @param[in]  HighPrioritySmi  TRUE: High Priority Alert SMI enabled
  @param[in]  LowPrioritySmi   TRUE: Low Priority Alert SMI enabled

**/
VOID
FnvNotifyHostAlertConfiguration (
  IN BOOLEAN HighPrioritySmi,
  IN BOOLEAN LowPrioritySmi
  );

#endif // __CRYSTAL_RIDGE_LIB_H__
