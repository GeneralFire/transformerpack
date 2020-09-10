/** @file
  Interface of Error Report library.

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

#ifndef __ERROR_REPORT_LIB_H__
#define __ERROR_REPORT_LIB_H__
#include <Pi/PiStatusCode.h>
#include <Protocol/ReportStatusCodeHandler.h>
#include <Guid/Cper.h>
#include <Guid/RasNonStandardErrorSection.h>
#include <Emca.h>

///
/// For optional input parameters of type UINT32. Indicates that the field
/// should be marked as invalid in the error record structure.
///
#define INVALID_U32   MAX_UINT32

RETURN_STATUS
EFIAPI
RegisterErrorLogListener (
  IN EFI_RSC_HANDLER_CALLBACK  Callback
  );

RETURN_STATUS
EFIAPI
UnregisterErrorLogListener (
  IN EFI_RSC_HANDLER_CALLBACK  Callback
  );

/**
  Report platform memory error log for MCA

  @param[in]  McBank                   - MC Bank the error was detected on
  @param[in]  Socket                   - The socket the MCE occured on
  @param[in]  Severity                 - The error severity
  @param[in]  PlatformMemoryErrorData  - Pointer to data structure EFI_PLATFORM_MEMORY_ERROR_DATA
  @param[in]  McErrSig                 - MC Error signature

  @retval EFI_STATUS
**/
RETURN_STATUS
EFIAPI
ReportPlatMemErrorLog (
  IN        UINT8                         McBank,
  IN        UINT8                         Socket,
  IN        UINT32                        Severity,
  IN        EFI_PLATFORM_MEMORY_ERROR_DATA  *PlatformMemoryErrorData,
  IN        EMCA_MC_SIGNATURE_ERR_RECORD  *McErrSig
  );

/**
  Report AP platform memory error log for MCA.

  BIOS Guard does not allow non-SMM BSP write EFI variable, so report AP memory error in BSP handler.

**/
VOID
EFIAPI
ReportApPlatMemErrorLog (
  VOID
  );

/**
  Report corrected memory error log for iMC.

  @param[in]  CorrectedMemoryErrorData  - Pointer to data structure EFI_PLATFORM_MEMORY_ERROR_DATA

  @retval RETURN_STATUS
**/
RETURN_STATUS
EFIAPI
ReportCorrectedMemErrorLog (
  IN  EFI_PLATFORM_MEMORY_ERROR_DATA  *CorrectedMemoryErrorData
  );

/**
  Report memory error log.

  @param[in]  Severity                  - Error Severity
  @param[in]  CorrectedMemoryErrorData  - Pointer to data structure EFI_PLATFORM_MEMORY_ERROR_DATA

  @retval RETURN_STATUS
**/
RETURN_STATUS
EFIAPI
ReportMemErrorLog (
  IN  UINT8                            Severity,
  IN  EFI_PLATFORM_MEMORY_ERROR_DATA  *MemoryErrorData
  );

/**
  This function reports the processor error record fields logs the appropriate type of error.

  @param[in] McBank     - The machine check bank
  @param[in] Socket           - The socket number
  @param[in] ErrorType  - The Error Type of the error
  @param[in] Severity          - The severity of the error
  @param[in] McErrSig    - MC Error signature

  @retval    EFI_STATUS
--*/
RETURN_STATUS
EFIAPI
ReportProcessorErrorLog (
  IN        UINT8                         McBank,
  IN        UINT8                         Socket,
  IN        UINT8                         ErrorType,
  IN        UINT32                        Severity,
  IN        EMCA_MC_SIGNATURE_ERR_RECORD  *McErrSig
  );

/**
  This function reports the PCIE error record fields logs the appropriate type of error.

  @param[in] Socket         The socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.
  @param[in] ErrorType      The type of error record

  @retval    EFI_STATUS
--*/
EFI_STATUS
EFIAPI
ReportPcieErrorLog (
  IN  UINT8             Socket,
  IN  UINT8             Bus,
  IN  UINT8             Device,
  IN  UINT8             Function,
  IN  VOID              *MmInfo,
  IN  UINT8             ErrorType
  );

/**
  This function reports the PCIE boot error record fields logs the appropriate type of error.

  @param[in] Socket                      The socket number.
  @param[in] Bus                         Device's bus number.
  @param[in] Device                      Device's device number.
  @param[in] Function                    Device's function number.
  @param[in] MmInfo                      Memory Map information of target device.
  @param[in] ErrorType                   The type of error record
  @param[in] BootErrorRegionAddress      Boot error region address for the BERT

  @retval    EFI_STATUS
--*/
EFI_STATUS
EFIAPI
ReportPcieBootErrorLog (
  IN  UINT8                   Socket,
  IN  UINT8                   Bus,
  IN  UINT8                   Device,
  IN  UINT8                   Function,
  IN  VOID                   *MmInfo,
  IN  UINT8                   ErrorType,
  IN  UINT64                  BootErrorRegionAddress
  );

/**
  Report platform memory boot error log for MCA.

  @param[in] McBank                       MC Bank the error was detected on
  @param[in] Socket                       The socket the MCE occured on
  @param[in] Severity                     The error severity
  @param[in] PlatformMemoryErrorData      Pointer to data structure EFI_PLATFORM_MEMORY_ERROR_DATA
  @param[in] McErrSig                     MC Error signature
  @param[in] BootErrorRegionAddress       Boot error region address for the BERT

  @retval RETURN_STATUS
**/
RETURN_STATUS
EFIAPI
ReportPlatMemBootErrorLog (
  IN      UINT8                            McBank,
  IN      UINT8                            Socket,
  IN      UINT32                           Severity,
  IN      EFI_PLATFORM_MEMORY_ERROR_DATA  *PlatformMemoryErrorData,
  IN      EMCA_MC_SIGNATURE_ERR_RECORD    *McErrSig,
  IN      UINT64                           BootErrorRegionAddress
  );

/**
  This function reports the processor error record fields logs the appropriate type of error.

  @param[in] McBank                      The machine check bank
  @param[in] Socket                      The socket number
  @param[in] ErrorType                   The Error Type of the error
  @param[in] Severity                    The severity of the error
  @param[in] McErrSig                    MC Error signature
  @param[in] BootErrorRegionAddress      Boot error region address for the BERT

  @retval    RETURN_STATUS
--*/
RETURN_STATUS
EFIAPI
ReportProcessorBootErrorLog (
  IN      UINT8                            McBank,
  IN      UINT8                            Socket,
  IN      UINT8                            ErrorType,
  IN      UINT32                           Severity,
  IN      EMCA_MC_SIGNATURE_ERR_RECORD     *McErrSig,
  IN      UINT64                           BootErrorRegionAddress
  );

/**
  Report a firmware boot error to BERT.

  RecordIdGuid is optional. If null, the zero GUID will be used. Payload is
  optional and may be null only if PayloadLength is zero.

  @param[in]  ErrorType                 The firmware-specific error type.
  @param[in]  Severity                  The severity of the error.
  @param[in]  RecordId                  The record ID number.
  @param[in]  RecordIdGuid              The record ID GUID.
  @param[in]  Payload                   The payload to append to the record.
  @param[in]  PayloadLength             The payload length.
  @param[in]  BootErrorRegionAddress    The boot error region address of BERT.

  @retval     EFI_SUCCESS               The error was successfully logged.
  @retval     EFI_INVALID_PARAMETER     Payload is null when PayloadLength is
                                        not zero.
  @retval     EFI_OUT_OF_RESOURCES      The record could not be allocated.
  @retval     EFI_UNSUPPORTED           The boot error region is full.
**/
EFI_STATUS
EFIAPI
ReportFirmwareBootErrorLog (
  IN  UINT8                         ErrorType,
  IN  UINT32                        Severity,
  IN  UINT64                        RecordId,
  IN  EFI_GUID                      *RecordIdGuid,  OPTIONAL
  IN  UINT8                         *Payload,       OPTIONAL
  IN  UINT32                        PayloadLength,
  IN  UINT64                        BootErrorRegionAddress
  );

/**
  Report a memory sparing event.

  Report the details of a memory sparing operation event. Direction applies to
  sparing event types which may need to be reversed after being applied, in
  order to apply a different operation (e.g. ADDDC, PCLS).

  If an optional input parameter is INVALID_U32 on entry, the corresponding
  field is marked invalid in the log data.

  @param[in]  EventType           The type of sparing event to log.
                                  Refer to RAS_MEMORY_SPARE_EVENT_TYPE.
  @param[in]  Direction           The direction of the operation.
                                  Refer to RAS_MEMORY_SPARE_DIRECTION.
  @param[in]  CopyStatus          The status of the sparing copy.
                                  Refer to RAS_MEMORY_SPARE_COPY_STATUS.
  @param[in]  Socket              The socket number of the error(s).
  @param[in]  Channel             The channel number on the socket.
  @param[in]  EventIndex          The event index, from 0 to NumEvents-1.
  @param[in]  NumEvents           The number of expected events for this type.
  @param[in]  SourceRank          The source rank index.
  @param[in]  SourceBank          The source bank index.
  @param[in]  DestinationRank     The destination rank index.
  @param[in]  DestinationBank     The destination bank index.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_INVALID_PARAMETER   EventType, Direction, or CopyStatus was an
                                  unknown value.
  @retval EFI_INVALID_PARAMETER   EventIndex was not less than NumEvents.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportSpareErrorLog (
  IN  UINT32    EventType,
  IN  UINT32    Direction,        OPTIONAL
  IN  UINT32    CopyStatus,
  IN  UINT32    Socket,
  IN  UINT32    Channel,
  IN  UINT32    EventIndex,
  IN  UINT32    NumEvents,
  IN  UINT32    SourceRank,       OPTIONAL
  IN  UINT32    SourceBank,       OPTIONAL
  IN  UINT32    DestinationRank,  OPTIONAL
  IN  UINT32    DestinationBank   OPTIONAL
  );

/**
  Report a UPI failover.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  LaneIndex           The UPI lane index on the socket.
  @param[in]  FailedLanes         The encoded active lanes for the failure.
                                  Refer to RAS_UPI_FAILED_LANES.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_INVALID_PARAMETER   FailedLanes was an unknown value.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportUpiFailoverErrorLog (
  IN  UINT32    Socket,
  IN  UINT32    LaneIndex,
  IN  UINT32    FailedLanes
  );


/**
  Report a UPI error.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  LaneIndex           The UPI lane index on the socket.
  @param[in]  Mscod               The encoded mscod of UPI bank.
                                  Refer to RAS_UPI_FAILED_LANES.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_INVALID_PARAMETER   FailedLanes was an unknown value.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportUpiErrorLog (
  IN  UINT32    Socket,
  IN  UINT32    LaneIndex,
  IN  UINT32    Mscod
  );

/**
  Report an IEH error.

  Report a "local" IEH error; i.e. one that cannot be reported via other
  standard formats.

  If an optional input parameter is INVALID_U32 on entry, the corresponding
  field is marked invalid in the log data.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  IehType             The type of IEH that had the error.
                                  Refer to RAS_IEH_TYPE.
  @param[in]  IehIndex            The index of the IEH that had the error, for
                                  types with multiple instances.
  @param[in]  IehErrorStatus      The status value of error that occurred.
  @param[in]  SourceIndex         The index of the error source, for error
                                  types with multiple sources.
  @param[in]  ErrSeverity         The error severity for the standard header.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_INVALID_PARAMETER   IehType was an unknown value.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportIehErrorLog (
  IN  UINT32    Socket,
  IN  UINT32    IehType,
  IN  UINT32    IehIndex,         OPTIONAL
  IN  UINT32    IehErrorStaus,
  IN  UINT32    SourceIndex,      OPTIONAL
  IN  UINT32    ErrSeverity
  );

/**
  Report an IIO internal error.

  Report a "internal" IIO error; i.e. one that cannot be reported via other
  standard formats.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  IioStack            The index of the IIO stack that had the error.
  @param[in]  IioErrorType        The type of error that occurred.
                                  Refer to RAS_IIO_INTERNAL_ERROR_TYPE.
  @param[in]  ErrSeverity         The error severity for the standard header.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_INVALID_PARAMETER   IioErrorType was an unknown value.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportIioInternalErrorLog (
  IN  UINT32    Socket,
  IN  UINT32    IioStack,
  IN  UINT32    IioErrorType,
  IN  UINT32    ErrSeverity
  );

/**
  Report an IIO Root Port XP error.

  Report a IIO Root Port XP error; i.e. one that cannot be reported via other
  standard formats.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  Bus                 The Bus number of the IIO Root port that had the error.
  @param[in]  Device              The Device number of the IIO Root port that had the error.
  @param[in]  Function            The Function number of the IIO Root port that had the error.
  @param[in]  XpCorErrSts         The XP Correctable error status of the IIO Root port that had the error.
  @param[in]  XpUncErrSts         The XP Uncorrectable error status of the IIO Root port that had the error.
  @param[in]  ErrSeverity         The error severity for the standard header.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_INVALID_PARAMETER   IioErrorType was an unknown value.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportIioXpErrorLog (
  IN  UINT32    Socket,
  IN  UINT8     Bus,
  IN  UINT8     Device,
  IN  UINT8     Function,
  IN  UINT32    XpCorErrSts,
  IN  UINT32    XpUncErrSts,
  IN  UINT32    ErrSeverity
  );

/**
  Report an SPD SMBUS error.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  SmbusSegment        The SMBUS segment index on the socket.
  @param[in]  Recovered           TRUE: recovery succeeded; FALSE: failed, or
                                  not attempted.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportSpdSmbusErrorLog (
  IN  UINT32    Socket,
  IN  UINT32    SmbusSegment,
  IN  BOOLEAN   Recovered
  );

/**
  Report DDRT ECC mode switch error

  @param[in]  Socket              The socket number of the error.
  @param[in]  ChOnSkt             The channal number on the socket.
  @param[in]  Rank                The Rank number on the channel.
  @param[in]  Nibble              The failed nibble.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportDdrtEccModeSwitchErrorLog (
  IN   UINT32   Socket,
  IN   UINT32   ChOnSkt,
  IN   UINT32   Rank,
  IN   UINT32   Nibble
);

/**
  Report a DDRT alert error.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  Channel             The channel number on the socket.
  @param[in]  Dimm                The DIMM number on the channel.
  @param[in]  DdrtErrorType       The type of error that occurred.
                                  Refer to RAS_DDRT_ALERT_ERROR_TYPE.
  @param[in]  DdrtAlertErrorLog   DDRT Alert Error Log.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_INVALID_PARAMETER   DdrtErrorType was an unknown value.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportDdrtAlertErrorLog (
  IN  UINT32                       Socket,
  IN  UINT32                       Channel,
  IN  UINT32                       Dimm,
  IN  UINT32                       DdrtAlertErrorType,
  IN  RAS_DDRT_ALERT_ERROR_LOG    *DdrtAlertErrorLog      OPTIONAL
  );

/**
  Report a spare core event.

  Report an event for the opportunistic spare core feature. Only one core is
  reported per log entry, so if multiple cores failed, this function should
  be invoked multiple times.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  FailedCore          The failed core index on the socket.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportSpareCoreErrorLog (
  IN  UINT32    Socket,
  IN  UINT32    FailedCore
  );

/**
  Report the UBOX order of errors.

  Report the first and second ports where an MCERR or IERR occurred on a given
  socket.

  If an optional input parameter is INVALID_U32 on entry, the corresponding
  field is marked invalid in the log data.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  FirstMcerrPort      The source port of the first MCERR.
  @param[in]  SecondMcerrPort     The source port of the second MCERR.
  @param[in]  FirstIerrPort       The source port of the first IERR.
  @param[in]  SecondIerrPort      The source port of the second IERR.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_INVALID_PARAMETER   SecondMcerrPort or SecondIerrPort was
                                  provided but FirstMcerrPort or FirstIerrPort
                                  (respectively) was not.
  @retval EFI_INVALID_PARAMETER   Neither FirstMcerrPort nor FirstIerrPort
                                  was provided.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportUboxOrderErrorLog (
  IN  UINT32    Socket,
  IN  UINT32    FirstMcerrPort,   OPTIONAL
  IN  UINT32    SecondMcerrPort,  OPTIONAL
  IN  UINT32    FirstIerrPort,    OPTIONAL
  IN  UINT32    SecondIerrPort    OPTIONAL
  );

/**
  Report a leaky bucket event.

  Report re-equalization and/or link degradation events triggered by the PCIe
  leaky bucket feature.

  @param[in]  Segment             The segment number of the PCIe root port.
  @param[in]  Bus                 The bus number of the PCIe root port.
  @param[in]  Device              The device number of the PCIe root port.
  @param[in]  Function            The function number of the PCIe root port.
  @param[in]  LaneErrorMask       The bitmask of lanes that were triggered.
  @param[in]  LaneErrorCount      An array of error counts for each lane.
  @param[in]  Gen34ReEqualize     TRUE if re-equalization was requested.
  @param[in]  Gen2LinkDegrade     TRUE if Gen2 link degradation occurred.
  @param[in]  Gen3LinkDegrade     TRUE if Gen3 link degradation occurred.
  @param[in]  Gen4LinkDegrade     TRUE if Gen4 link degradation occurred.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportLeakyBucketErrorLog (
  IN  UINT8     Segment,
  IN  UINT8     Bus,
  IN  UINT8     Device,
  IN  UINT8     Function,
  IN  UINT16    LaneErrorMask,
  IN  UINT8     LaneErrorCount[16],
  IN  BOOLEAN   Gen34ReEqualize,
  IN  BOOLEAN   Gen2LinkDegrade,
  IN  BOOLEAN   Gen3LinkDegrade,
  IN  BOOLEAN   Gen4LinkDegrade
  );

/**
  Report a CXL error.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  Address             Physical address.


  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_INVALID_PARAMETER   FailedLanes was an unknown value.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportCxlErrorLog (
  IN  UINT32    Socket,
  IN  UINTN     Address
  );
#endif // __ERROR_REPORT_LIB_H__
