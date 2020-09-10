/** @file
  RAS non-standard error section GUID header file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#ifndef _RAS_NON_STANDARD_ERROR_SECTION_H_
#define _RAS_NON_STANDARD_ERROR_SECTION_H_

#include <Guid/Cper.h>

#pragma pack(push, 1)

///
/// Common fields for every section structure when SectionType is
/// gRasNonStandardErrorSectionGuid.
///
typedef struct {
  UINT32                    ErrorType;          ///< refer to RAS_ERROR_TYPE
  UINT32                    Revision;           ///< type-specific revision
  UINT64                    ValidFields;        ///< type-specific bitmask of fields after Common
} RAS_ERROR_SECTION_COMMON;

///
/// Valid values for Common.ErrorType.
///
typedef enum {
  RasErrorTypeSparing = 0,                      ///< identifies RAS_SPARING_ERROR_SECTION
  RasErrorTypeUpiFailover,                      ///< identifies RAS_UPI_FAILOVER_ERROR_SECTION
  RasErrorTypeIeh,                              ///< identifies RAS_IEH_ERROR_SECTION
  RasErrorTypeIioInternal,                      ///< identifies RAS_IIO_INTERNAL_ERROR_SECTION
  RasErrorTypeIioXpError,                       ///< identifies RAS_IIO_XP_ERROR_SECTION
  RasErrorTypeSpdSmbus,                         ///< identifies RAS_SPD_SMBUS_ERROR_SECTION
  RasErrorTypeDdrtEcc,                          ///< identifies RAS_DDRT_ECC_ERROR_SECTION
  RasErrorTypeDdrtAlert,                        ///< identifies RAS_DDRT_ALERT_ERROR_SECTION
  RasErrorTypeSpareCore,                        ///< identifies RAS_SPARE_CORE_ERROR_SECTION
  RasErrorTypeUboxOrder,                        ///< identifies RAS_UBOX_ORDER_ERROR_SECTION
  RasErrorTypeLeakyBucket,                      ///< identifies RAS_LEAKY_BUCKET_SECTION
  RasErrorTypeUpiError,                         ///< identifies RAS_UPI_ERROR_SECTION
  RasErrorTypeCxlError,                         ///< identifies RAS_CXL_ERROR_SECTION
  RasErrorTypeMax                               ///< must be the last value in the enum
} RAS_ERROR_TYPE;

///
/// Section structure for memory sparing events.
///
/// Used when Common.ErrorType is RasErrorTypeSparing.
///
typedef struct {
  RAS_ERROR_SECTION_COMMON  Common;             ///< Common.ErrorType must be RasErrorTypeSparing
  UINT32                    EventType;          ///< refer to RAS_MEMORY_SPARE_EVENT_TYPE
  UINT32                    Direction;          ///< refer to RAS_MEMORY_SPARE_DIRECTION
  UINT32                    CopyStatus;         ///< refer to RAS_MEMORY_SPARE_COPY_STATUS
  UINT32                    Socket;             ///< socket index
  UINT32                    Channel;            ///< channel index on socket
  UINT32                    EventIndex;         ///< incremented per event from 0 to NumEvents-1
  UINT32                    NumEvents;          ///< number of events expected for this EventType
  UINT32                    SourceRank;         ///< source rank index
  UINT32                    SourceBank;         ///< source bank index
  UINT32                    DestinationRank;    ///< destination rank index
  UINT32                    DestinationBank;    ///< destination bank index
} RAS_SPARING_ERROR_SECTION;

#define RAS_SPARING_ERROR_SECTION_REVISION 1

///
/// Memory sparing bit fields for Common.ValidFields.
///
/// Used when Common.ErrorType is RasErrorTypeSparing.
///
typedef union {
  struct {
    UINT64                  EventType:1;        ///< 1: Sparing.EventType is valid; 0: invalid
    UINT64                  Direction:1;        ///< 1: Sparing.Direction is valid; 0: invalid
    UINT64                  CopyStatus:1;       ///< 1: Sparing.CopyStatus is valid; 0: invalid
    UINT64                  Socket:1;           ///< 1: Sparing.Socket is valid; 0: invalid
    UINT64                  Channel:1;          ///< 1: Sparing.Channel is valid; 0: invalid
    UINT64                  EventIndex:1;       ///< 1: Sparing.EventIndex is valid; 0: invalid
    UINT64                  NumEvents:1;        ///< 1: Sparing.NumEvents is valid; 0: invalid
    UINT64                  SourceRank:1;       ///< 1: Sparing.SourceRank is valid; 0: invalid
    UINT64                  SourceBank:1;       ///< 1: Sparing.SourceBank is valid; 0: invalid
    UINT64                  DestinationRank:1;  ///< 1: Sparing.DestinationRank is valid; 0: invalid
    UINT64                  DestinationBank:1;  ///< 1: Sparing.DestinationBank is valid; 0: invalid
  }                         Bits;               ///< one bit per field in RAS_SPARING_ERROR_SECTION
  UINT64                    Data;               ///< full 64-bit data of ValidFields
} RAS_SPARING_ERROR_SECTION_VALID_FIELDS;

///
/// Valid values for Sparing.EventType.
///
typedef enum {
  RasSpareEventAdddc = 0,                       ///< ADDDC event
  RasSpareEventRankSparing,                     ///< Rank sparing event
  RasSpareEventPlusOne,                         ///< (SDDC/ADDDC) +1 event
  RasSpareEventPcls,                            ///< PCLS event
  RasSpareEventLaneSparing,                     ///< Lane sparing event
  RasSpareEventMax                              ///< must be the last value in the enum
} RAS_MEMORY_SPARE_EVENT_TYPE;

///
/// Valid values for Sparing.Direction.
///
typedef enum {
  RasSparingForward = 0,                        ///< Sparing operation is being applied
  RasSparingReverse,                            ///< Sparing operation is being reversed
  RasSpareDirectionMax                          ///< must be the last value in the enum
} RAS_MEMORY_SPARE_DIRECTION;

///
/// Valid values for Sparing.CopyStatus.
///
typedef enum {
  RasSpareCopyInProgress = 0,                   ///< Sparing copy is in progress
  RasSpareCopyDone,                             ///< Sparing copy is done
  RasSpareCopyStatusMax                         ///< must be the last value in the enum
} RAS_MEMORY_SPARE_COPY_STATUS;

///
/// Section structure for UPI failover events.
///
/// Used when Common.ErrorType is RasErrorTypeUpiFailover.
///
typedef struct {
  RAS_ERROR_SECTION_COMMON  Common;             ///< Common.ErrorType must be RasErrorTypeUpiFailover
  UINT32                    Socket;             ///< socket index
  UINT32                    LaneIndex;          ///< UPI lane index
  UINT32                    FailedLanes;        ///< refer to RAS_UPI_FAILED_LANES
} RAS_UPI_FAILOVER_ERROR_SECTION;

#define RAS_UPI_FAILOVER_ERROR_SECTION_REVISION 1

///
/// UPI failover bit fields for Common.ValidFields.
///
/// Used when Common.ErrorType is RasErrorTypeUpiFailover.
///
typedef union {
  struct {
    UINT64                  Socket:1;           ///< 1: UpiFailover.Socket is valid; 0: invalid
    UINT64                  LaneIndex:1;        ///< 1: UpiFailover.LaneIndex is valid; 0: invalid
    UINT64                  FailedLanes:1;      ///< 1: UpiFailover.FailedLanes is valid; 0: invalid
  }                         Bits;               ///< one bit per field in RAS_UPI_FAILOVER_ERROR_SECTION
  UINT64                    Data;               ///< full 64-bit data of ValidFields
} RAS_UPI_FAILOVER_ERROR_SECTION_VALID_FIELDS;

///
/// Valid values for UpiFailover.FailedLanes.
///
typedef enum {
  RasUpiFailLanesLower8 = 0,                    ///< Lanes 0-7
  RasUpiFailLanesUpper8,                        ///< Lanes 12-19
  RasUpiFailLanesMax                            ///< must be the last value in the enum
} RAS_UPI_FAILED_LANES;

///
/// Section structure for IEH error events.
///
/// Used when Common.ErrorType is RasErrorTypeIeh.
///
typedef struct {
  RAS_ERROR_SECTION_COMMON  Common;             ///< Common.ErrorType must be RasErrorTypeIeh
  UINT32                    Socket;             ///< socket index
  UINT32                    IehType;            ///< refer to RAS_IEH_TYPE
  UINT32                    IehIndex;           ///< individual index if the IEH type has multiple instances
  UINT32                    IehErrorStatus;     ///< the error status value of local error source
  UINT32                    SourceIndex;        ///< individual index if the error type has multiple sources
} RAS_IEH_ERROR_SECTION;

#define RAS_IEH_ERROR_SECTION_REVISION 1

///
/// IEH error bit fields for Common.ValidFields.
///
/// Used when Common.ErrorType is RasErrorTypeIeh.
///
typedef union {
  struct {
    UINT64                  Socket:1;           ///< 1: Ieh.Socket is valid; 0: invalid
    UINT64                  IehType:1;          ///< 1: Ieh.IehType is valid; 0: invalid
    UINT64                  IehIndex:1;         ///< 1: Ieh.IehIndex is valid; 0: invalid
    UINT64                  IehErrorStatus:1;   ///< 1: Ieh.IehErrorStatus is valid; 0: invalid
    UINT64                  SourceIndex:1;      ///< 1: Ieh.SourceIndex is valid; 0: invalid
  }                         Bits;               ///< one bit per field in RAS_IEH_ERROR_SECTION
  UINT64                    Data;               ///< full 64-bit data of ValidFields
} RAS_IEH_ERROR_SECTION_VALID_FIELDS;

///
/// Section structure for IIO internal error events.
///
/// Used when Common.ErrorType is RasErrorTypeIioInternal.
///
typedef struct {
  RAS_ERROR_SECTION_COMMON  Common;             ///< Common.ErrorType must be RasErrorTypeIioInternal
  UINT32                    Socket;             ///< socket index
  UINT32                    IioStack;           ///< IIO stack index on socket
  UINT32                    IioErrorType;       ///< refer to RAS_IIO_INTERNAL_ERROR_TYPE
} RAS_IIO_INTERNAL_ERROR_SECTION;

#define RAS_IIO_INTERNAL_ERROR_SECTION_REVISION 1

///
/// IIO internal error bit fields for Common.ValidFields.
///
/// Used when Common.ErrorType is RasErrorTypeIioInternal.
///
typedef union {
  struct {
    UINT64                  Socket:1;           ///< 1: IioInternal.Socket is valid; 0: invalid
    UINT64                  IioStack:1;         ///< 1: IioInternal.IioStack is valid; 0: invalid
    UINT64                  IioErrorType:1;     ///< 1: IioInternal.ErrorSource is valid; 0: invalid
  }                         Bits;               ///< one bit per field in RAS_IIO_INTERNAL_ERROR_SECTION
  UINT64                    Data;               ///< full 64-bit data of ValidFields
} RAS_IIO_INTERNAL_ERROR_SECTION_VALID_FIELDS;

///
/// Valid values for IioInternal.IioErrorType.
///
typedef enum {
  RasIioInternalErrorVtd = 0,                   ///< VTD error
  RasIioInternalErrorCBDma,                     ///< CBDMA error
  RasIioInternalErrorM2PCIE,                    ///< M2PCIE error
  RasIioInternalErrorIrp,                       ///< IRP error
  RasIioInternalErrorRing,                      ///< Ring error
  RasIioInternalErrorItc,                       ///< ITC error
  RasIioInternalErrorOtc,                       ///< OTC error
  RasIioInternalErrorMax                        ///< must be the last value in the enum
} RAS_IIO_INTERNAL_ERROR_TYPE;

///
/// Section structure for IIO Root Port XP error events.
///
/// Used when Common.ErrorType is RasErrorTypeIioXpError.
///
typedef struct {
  RAS_ERROR_SECTION_COMMON  Common;             ///< Common.ErrorType must be RasErrorTypeIioXpError
  UINT32                    Socket;             ///< socket index
  UINT8                     Bus;                ///< Bus Number of the root port
  UINT8                     Device;             ///< Device Number of the root port
  UINT8                     Function;           ///< Function Number of the root port
  UINT32                    XpCorErrSts;        ///< XP correctable error status
  UINT32                    XpUncErrSts;        ///< XP uncorrectable error status
} RAS_IIO_XP_ERROR_SECTION;

#define RAS_IIO_XP_ERROR_SECTION_REVISION 1

///
/// IIO XP error bit fields for Common.ValidFields.
///
/// Used when Common.ErrorType is RasErrorTypeIioXpError.
///
typedef union {
  struct {
    UINT64                  Socket:1;           ///< 1: IioXpError.Socket is valid; 0: invalid
    UINT64                  Bus:1;              ///< 1: IioXpError.Bus is valid; 0: invalid
    UINT64                  Device:1;           ///< 1: IioXpError.Device is valid; 0: invalid
    UINT64                  Function:1;         ///< 1: IioXpError.Function is valid; 0: invalid
    UINT64                  XpErrSts:1;         ///< 1: IioXpError.ErrorSource is valid; 0: invalid
  }                         Bits;               ///< one bit per field in RAS_IIO_XP_ERROR_SECTION
  UINT64                    Data;               ///< full 64-bit data of ValidFields
} RAS_IIO_XP_ERROR_SECTION_VALID_FIELDS;

///
/// Section structure for SPD SMBUS error events.
///
/// Used when Common.ErrorType is RasErrorTypeSpdSmbus.
///
typedef struct {
  RAS_ERROR_SECTION_COMMON  Common;             ///< Common.ErrorType must be RasErrorTypeSpdSmbus
  UINT32                    Socket;             ///< socket index
  UINT32                    SmbusSegment;       ///< SMBUS segment index on socket
  UINT32                    Flags;              ///< refer to RAS_SPD_SMBUS_FLAGS
} RAS_SPD_SMBUS_ERROR_SECTION;

#define RAS_SPD_SMBUS_ERROR_SECTION_REVISION 1

///
/// SPD SMBUS bit fields for Common.ValidFields.
///
/// Used when Common.ErrorType is RasErrorTypeSpdSmbus.
///
typedef union {
  struct {
    UINT64                  Socket:1;           ///< 1: SpdSmbus.Socket is valid; 0: invalid
    UINT64                  SmbusSegment:1;     ///< 1: SpdSmbus.SmbusSegment is valid; 0: invalid
    UINT64                  Flags:1;            ///< 1: SpdSmbus.Flags is valid; 0: invalid
  }                         Bits;               ///< one bit per field in RAS_SPD_SMBUS_ERROR_SECTION
  UINT64                    Data;               ///< full 64-bit data of ValidFields
} RAS_SPD_SMBUS_ERROR_SECTION_VALID_FIELDS;

///
/// Bit fields for SpdSmbus.Flags.
///
typedef union {
  struct {
    UINT32                  Recovered:1;        ///< 1: SMBUS recovery succeeded; 0: failed or not attempted
  }                         Bits;               ///< flag bits for RAS_SPD_SMBUS_ERROR_SECTION
  UINT32                    Data;               ///< full 32-bit data of Flags
} RAS_SPD_SMBUS_FLAGS;

typedef struct {
  RAS_ERROR_SECTION_COMMON  Common;             ///< Common.ErrorType must be RasErrorTypeDdrtEcc
  UINT32                    Socket;             ///< socket index
  UINT32                    ChOnSkt;            ///< Chanel index on socket
  UINT32                    Rank;               ///< Rank
  UINT32                    Nibble;             ///< Nibble
} RAS_DDRT_ECC_ERROR_SECTION;

#define RAS_DDRT_ECC_ERROR_SECTION_REVISION 1

typedef union {
  struct {
    UINT64                  Socket : 1;           ///< 1: DdrtEcc.Socket is valid; 0: invalid
    UINT64                  ChOnSkt : 1;          ///< 1: DdrtEcc.ChOnSkt is valid; 0: invalid
    UINT64                  Rank : 1;             ///< 1: DdrtEcc.Rank is valid; 0: invalid
    UINT64                  Nibble : 1;           ///< 1: DdrtEcc.Nibble is valid; 0: invalid
  }                         Bits;                 ///< one bit per field in RAS_DDRT_ECC_ERROR_SECTION
  UINT64                    Data;                 ///< full 64-bit data of ValidFields
} RAS_DDRT_ECC_ERROR_SECTION_VALID_FIELDS;

///
/// Structure for DDRT alert error log.
///
typedef union {
  struct {
    UINT8                 ErrorType;         ///< Indicates the kind of media error that was logged.
                                             ///< Refer to RAS_DDRT_ALERT_Media_Log_Error_TYPE.
    UINT8                 TransactionType;   ///< Indicates what transaction caused the media error.
                                             ///< Refer to RAS_DDRT_ALERT_Media_Log_Transaction_TYPE.
    UINT64                Dpa;               ///< Dpa address of the error
  } MediaLog;                                ///< Media log
  struct {
    UINT8                 Reported;          ///< Indicates temperature being reported
                                             ///< Refer to RAS_DDRT_ALERT_THERMAL_LOG_REPORTED.
    UINT8                 TempType;          ///< Indicates which device the temperature is for
                                             ///< Refer to RAS_DDRT_ALERT_THERMAL_LOG_TEMP_TYPE.
  } ThermalLog;                              ///< Thermal Log
} RAS_DDRT_ALERT_ERROR_LOG;

///
/// Section structure for DDRT alert error events.
///
/// Used when Common.ErrorType is RasErrorTypeDdrtAlert.
///
typedef struct {
  RAS_ERROR_SECTION_COMMON  Common;                  ///< Common.ErrorType must be RasErrorTypeDdrtAlert
  UINT32                    Socket;                  ///< socket index
  UINT32                    Channel;                 ///< channel index on socket
  UINT32                    Dimm;                    ///< DIMM index on channel
  UINT32                    DdrtAlertErrorType;      ///< refer to RAS_DDRT_ALERT_ERROR_TYPE
  RAS_DDRT_ALERT_ERROR_LOG  DdrtAlertErrorLog;       ///< DDRT alert error log
} RAS_DDRT_ALERT_ERROR_SECTION;

#define RAS_DDRT_ALERT_ERROR_SECTION_REVISION 1

///
/// DDRT alert bit fields for Common.ValidFields.
///
/// Used when Common.ErrorType is RasErrorTypeDdrtAlert.
///
typedef union {
  struct {
    UINT64                  Socket:1;                ///< 1: DdrtAlert.Socket is valid; 0: invalid
    UINT64                  Channel:1;               ///< 1: DdrtAlert.Channel is valid; 0: invalid
    UINT64                  Dimm:1;                  ///< 1: DdrtAlert.Dimm is valid; 0: invalid
    UINT64                  DdrtAlertErrorType:1;    ///< 1: DdrtAlert.DdrtAlertErrorType is valid; 0: invalid
    UINT64                  DdrtAlertErrorLog:1;     ///< 1: DdrtAlert.DdrtAlertErrorLog is valid; 0: invalid
  }                         Bits;                    ///< one bit per field in RAS_DDRT_ALERT_ERROR_SECTION
  UINT64                    Data;                    ///< full 64-bit data of ValidFields
} RAS_DDRT_ALERT_ERROR_SECTION_VALID_FIELDS;

///
/// Valid values for DDRT alert error type.
///
typedef enum {
  DdrtAlertErrotTypeMediaErrorLog = 0,                     ///< Media error log
  DdrtAlertErrotTypeThermalErrorLog,                       ///< Thermal error log
  DdrtAlertErrotTypeLongOperationComplete,                 ///< Long operation complete
  DdrtAlertErrotTypeFwHang,                                ///< Fw hang
  DdrtAlertErrotTypeFatal,                                 ///< Fatal
  DdrtAlertErrorTypeMax                                    ///< must be the last value in the enum
} RAS_DDRT_ALERT_ERROR_TYPE;

///
/// Valid values for media log ErrorType.
///
typedef enum {
  MediaLogErrorTypeUncorrectable = 0,                         ///< Uncorrectable error
  MediaLogErrorTypeDpaMismatch,                               ///< Dpa mismatch error
  MediaLogErrorTypeAitError,                                  ///< Ait error. Deprecated since FIS 2.0
  MediaLogErrorTypeDataPathError,                             ///< Data path error
  MediaLogErrorTypeLockedIllegalAccess,                       ///< Locked/Illegal access error
  MediaLogErrorTypeUserPercentageRemainingAlarmTrip,          ///< User percentage remaining alarm trip error
  MediaLogErrorTypeSmartHealthStatusChange,                   ///< Smart health status change error
  MediaLogErrorTypePersistentWriteEcc,                        ///< Persistent write ecc error
  MediaLogErrorTypeMax                                        ///< must be the last value in the enum
} RAS_DDRT_ALERT_MEDIA_LOG_ERROR_TYPE;

///
/// Valid values for media log TransactionType.
///
typedef enum {
  MediaLogTransactionType2lmRead = 0,                    ///< 2LM read
  MediaLogTransactionType2lmWrite,                       ///< 2LM write
  MediaLogTransactionTypePmRead,                         ///< PM read
  MediaLogTransactionTypePmWrite,                        ///< PM write
  MediaLogTransactionTypeBwRead,                         ///< BW read
  MediaLogTransactionTypeBwWrite,                        ///< BW write
  MediaLogTransactionTypeAitRead,                        ///< Ait read
  MediaLogTransactionTypeAitWrite,                       ///< Ait write
  MediaLogTransactionTypeWearLevelMove,                  ///< Wear level move
  MediaLogTransactionTypePatrolScrub,                    ///< Patrol scrub
  MediaLogTransactionTypeCsrRead,                        ///< CSR read
  MediaLogTransactionTypeCsrWrite,                       ///< CSR write
  MediaLogTransactionTypeAddressRangeScrub,              ///< Address range scrub
  MediaLogTransactionTypeUnavailable,                    ///< Unavailable
  MediaLogTransactionTypeMax                             ///< must be the last value in the enum
} RAS_DDRT_ALERT_MEDIA_LOG_TRANSACTION_TYPE;

///
/// Valid values for thermal log Reported.
///
typedef enum {
  ThermalLogReportedUserAlarmTrip = 0,                   ///< User Alarm Trip
  ThermalLogReportedLow,                                 ///< Low
  ThermalLogReportedHigh,                                ///< High
  ThermalLogReportedCritical,                            ///< Critical
  ThermalLogReportedMax                                 ///< must be the last value in the enum
} RAS_DDRT_ALERT_THERMAL_LOG_REPORTED;

///
/// Valid values for thermal log TempType.
///
typedef enum {
  ThermalLogTempTypeMedia = 0,                     ///< Media
  ThermalLogTempTypeCore,                          ///< Core
  ThermalLogTempTypeMax                            ///< must be the last value in the enum
} RAS_DDRT_ALERT_THERMAL_LOG_TEMP_TYPE;

///
/// Section structure for opportunistic spare core events.
///
/// Used when Common.ErrorType is RasErrorTypeSpareCore.
///
typedef struct {
  RAS_ERROR_SECTION_COMMON  Common;             ///< Common.ErrorType must be RasErrorTypeSpareCore
  UINT32                    Socket;             ///< socket index
  UINT32                    FailedCore;         ///< failed core index on socket
} RAS_SPARE_CORE_ERROR_SECTION;

#define RAS_SPARE_CORE_ERROR_SECTION_REVISION 1

///
/// Opportunistic spare core bit fields for Common.ValidFields.
///
/// Used when Common.ErrorType is RasErrorTypeSpareCore.
///
typedef union {
  struct {
    UINT64                  Socket:1;           ///< 1: SpareCore.Socket is valid; 0: invalid
    UINT64                  FailedCore:1;       ///< 1: SpareCore.FailedCore is valid; 0: invalid
  }                         Bits;               ///< one bit per field in RAS_SPARE_CORE_ERROR_SECTION
  UINT64                    Data;               ///< full 64-bit data of ValidFields
} RAS_SPARE_CORE_ERROR_SECTION_VALID_FIELDS;

///
/// Section structure for UBOX order error events.
///
/// Used when Common.ErrorType is RasErrorTypeUboxOrder.
///
typedef struct {
  RAS_ERROR_SECTION_COMMON  Common;             ///< Common.ErrorType must be RasErrorTypeUboxOrder
  UINT32                    Socket;             ///< socket index
  UINT32                    FirstMcerrPort;     ///< source port of the first MCERR
  UINT32                    SecondMcerrPort;    ///< source port of the second MCERR
  UINT32                    FirstIerrPort;      ///< source port of the first IERR
  UINT32                    SecondIerrPort;     ///< source port of the second IERR
} RAS_UBOX_ORDER_ERROR_SECTION;

#define RAS_UBOX_ORDER_ERROR_SECTION_REVISION 1

///
/// UBOX order bit fields for Common.ValidFields.
///
/// Used when Common.ErrorType is RasErrorTypeUboxOrder.
///
typedef union {
  struct {
    UINT64                  Socket:1;           ///< 1: UboxOrder.Socket is valid; 0: invalid
    UINT64                  FirstMcerrPort:1;   ///< 1: UboxOrder.FirstMcerrPort is valid; 0: invalid
    UINT64                  SecondMcerrPort:1;  ///< 1: UboxOrder.SecondMcerrPort is valid; 0: invalid
    UINT64                  FirstIerrPort:1;    ///< 1: UboxOrder.FirstIerrPort is valid; 0: invalid
    UINT64                  SecondIerrPort:1;   ///< 1: UboxOrder.SecondIerrPort is valid; 0: invalid
  }                         Bits;               ///< one bit per field in RAS_UBOX_ORDER_ERROR_SECTION
  UINT64                    Data;               ///< full 64-bit data of ValidFields
} RAS_UBOX_ORDER_ERROR_SECTION_VALID_FIELDS;

///
/// Section structure for PCIe leaky bucket events.
///
/// Used when Common.ErrorType is RasErrorTypeLeakyBucket.
///
typedef struct {
  RAS_ERROR_SECTION_COMMON  Common;             ///< Common.ErrorType must be RasErrorTypeLeakyBucket
  UINT16                    Segment;            ///< root port PCI segment number
  UINT8                     Bus;                ///< root port PCI primary bus number
  UINT8                     Device;             ///< root port PCI device number
  UINT8                     Function;           ///< root port PCI function number
  UINT8                     Reserved;           ///< reserved for alignment
  UINT16                    LaneErrorMask;      ///< bit mask of which lane(s) exceeded the leaky bucket threshold
  UINT8                     LaneErrorCount[16]; ///< error count for each lane
  UINT32                    ActionStatus;       ///< refer to RAS_LEAKY_BUCKET_ACTION_STATUS
} RAS_LEAKY_BUCKET_ERROR_SECTION;

#define RAS_LEAKY_BUCKET_ERROR_SECTION_REVISION 1

///
/// PCIe leaky bucket bit fields for Common.ValidFields.
///
/// Used when Common.ErrorType is RasErrorTypeLeakyBucket.
///
typedef union {
  struct {
    UINT64                  Segment:1;          ///< 1: LeakyBucket.Segment is valid; 0: invalid
    UINT64                  Bus:1;              ///< 1: LeakyBucket.PrimaryBus is valid; 0: invalid
    UINT64                  Device:1;           ///< 1: LeakyBucket.Device is valid; 0: invalid
    UINT64                  Function:1;         ///< 1: LeakyBucket.Function is valid; 0: invalid
    UINT64                  LaneErrorMask:1;    ///< 1: LeakyBucket.LaneMask is valid; 0: invalid
    UINT64                  LaneErrorCount:1;   ///< 1: LeakyBucket.LaneErrorCount is valid; 0: invalid
    UINT64                  ActionStatus:1;     ///< 1: LeakyBucket.ActionStatus is valid; 0: invalid
  }                         Bits;               ///< one bit per field in RAS_LEAKY_BUCKET_SECTION
  UINT64                    Data;               ///< full 64-bit data of ValidFields
} RAS_LEAKY_BUCKET_ERROR_SECTION_VALID_FIELDS;

///
/// Bit fields for LeakyBucket.
///
typedef union {
  struct {
    UINT32                  Gen34ReEqualize:1;  ///< 1: Gen3/4 re-equalization was requested; 0: was not requested
    UINT32                  Gen2LinkDegrade:1;  ///< 1: Gen2 link degradation has occurred; 0: has not occured
    UINT32                  Gen3LinkDegrade:1;  ///< 1: Gen3 link degradation has occurred; 0: has not occured
    UINT32                  Gen4LinkDegrade:1;  ///< 1: Gen4 link degradation has occurred; 0: has not occured
  }                         Bits;               ///< status bits for RAS_LEAKY_BUCKET_SECTION
  UINT32                    Data;               ///< full 32-bit data of ActionStatus
} RAS_LEAKY_BUCKET_ACTION_STATUS;


///
/// Section structure for UPI error events.
///
/// Used when Common.ErrorType is RasErrorTypeLeakyBucket.
///
typedef struct {
  RAS_ERROR_SECTION_COMMON  Common;             ///< Common.ErrorType must be RasErrorTypeUpiError
  UINT32                    Socket;             ///< socket index
  UINT32                    LaneIndex;          ///< UPI lane index
  UINT32                    Mscod;              ///< mscod value
} RAS_UPI_ERROR_SECTION;

#define RAS_UPI_ERROR_SECTION_REVISION 1

///
/// UPI error bit fields for Common.ValidFields.
///
/// Used when Common.ErrorType is RasErrorTypeUpiFailover.
///
typedef union {
  struct {
    UINT64                  Socket:1;           ///< 1: UpiError.Socket is valid; 0: invalid
    UINT64                  LaneIndex:1;        ///< 1: UpiError.LaneIndex is valid; 0: invalid
    UINT64                  Mscod:1;            ///< 1: UpiError.FailedLanes is valid; 0: invalid
  }                         Bits;               ///< one bit per field in RAS_UPI_ERROR_SECTION
  UINT64                    Data;               ///< full 64-bit data of ValidFields
} RAS_UPI_ERROR_SECTION_VALID_FIELDS;


///
/// Section structure for CXL error events.
///
/// Used when Common.ErrorType is RasErrorTypeCxlError.
///
typedef struct {
  RAS_ERROR_SECTION_COMMON  Common;             ///< Common.ErrorType must be RasErrorTypeCxlError
  UINT32                    Socket;             ///< socket index
  UINTN                     Address;            ///< Physical address
} RAS_CXL_ERROR_SECTION;

#define RAS_CXL_ERROR_SECTION_REVISION 1

///
/// CXL error bit fields for Common.ValidFields.
///
/// Used when Common.ErrorType is RasErrorTypeCxlError.
///
typedef union {
  struct {
    UINT64                  Socket:1;           ///< 1: CxlError.Socket is valid; 0: invalid
    UINT64                  Address:1;          ///< 1: CxlError.Address is valid; 0: invalid
  }Bits;                                        ///< one bit per field in RAS_CXL_ERROR_SECTION
  UINT64                    Data;               ///< full 64-bit data of ValidFields
} RAS_CXL_ERROR_SECTION_VALID_FIELDS;

///
/// Error section for a non-standard RAS error.
///
/// The actual section structure depends on the value of Common.ErrorType.
///
typedef union {
  RAS_ERROR_SECTION_COMMON        Common;       ///< check Common.ErrorType to identify section structure
  RAS_SPARING_ERROR_SECTION       Sparing;      ///< if Common.ErrorType is RasErrorTypeSparing
  RAS_UPI_FAILOVER_ERROR_SECTION  UpiFailover;  ///< if Common.ErrorType is RasErrorTypeUpiFailover
  RAS_IEH_ERROR_SECTION           Ieh;          ///< if Common.ErrorType is RasErrorTypeIeh
  RAS_IIO_INTERNAL_ERROR_SECTION  IioInternal;  ///< if Common.ErrorType is RasErrorTypeIioInternal
  RAS_IIO_XP_ERROR_SECTION        IioXpError;   ///< if Common.ErrorType is RasErrorTypeIioXpError
  RAS_SPD_SMBUS_ERROR_SECTION     SpdSmbus;     ///< if Common.ErrorType is RasErrorTypeSpdSmbus
  RAS_DDRT_ECC_ERROR_SECTION      DdrtEcc;      ///< if Common.ErrorType is RasErrorTypeDdrtEcc
  RAS_DDRT_ALERT_ERROR_SECTION    DdrtAlert;    ///< if Common.ErrorType is RasErrorTypeDdrtAlert
  RAS_SPARE_CORE_ERROR_SECTION    SpareCore;    ///< if Common.ErrorType is RasErrorTypeSpareCore
  RAS_UBOX_ORDER_ERROR_SECTION    UboxOrder;    ///< if Common.ErrorType is RasErrorTypeUboxOrder
  RAS_LEAKY_BUCKET_ERROR_SECTION  LeakyBucket;  ///< if Common.ErrorType is RasErrorTypeLeakyBucket
  RAS_UPI_ERROR_SECTION           UpiError;     ///< if Common.ErrorType is RasErrorTypeUpiError
  RAS_CXL_ERROR_SECTION           CxlError;     ///< if Common.ErrorType is RasErrorTypeCxlError
} RAS_NON_STANDARD_ERROR_SECTION;

///
/// Full error record for a non-standard RAS error.
///
/// Used when SectionType is gRasNonStandardErrorSectionGuid.
///
typedef struct {
  EFI_COMMON_ERROR_RECORD_HEADER  Header;       ///< standard error record header
  EFI_ERROR_SECTION_DESCRIPTOR    Descriptor;   ///< Descriptor.SectionType must be gRasNonStandardErrorSectionGuid
  RAS_NON_STANDARD_ERROR_SECTION  Section;      ///< non-standard section data
} RAS_NON_STANDARD_ERROR_RECORD;

extern EFI_GUID gRasNonStandardErrorSectionGuid;

#pragma pack(pop)

#endif // _RAS_NON_STANDARD_ERROR_SECTION_H_
