/** @file Fis.h
  Implementation of NVDIMM Firmware Interface Specification.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

  @par Revision Reference
  NVDIMM Firmware Interface Specification version 1.6.
**/

#ifndef _FIS_H_
#define _FIS_H_

#include <Library/DdrtCoreLib.h>

//
// Timeout value 0 used with FisRequest() cause calculating default, command specific timeout value.
//
#define FIS_DEFTIMEOUT 0

//
// Status codes that can be set by NVDIMM firmware in response for FIS command.
//
typedef enum {
  FIS_STS_SUCCESS                    = 0x00,
  FIS_STS_INVALID_PARAM              = 0x01,
  FIS_STS_DATA_XFER_ERR              = 0x02,
  FIS_STS_INTERNAL_ERR               = 0x03,
  FIS_STS_UNSUPPORTED_CMD            = 0x04,
  FIS_STS_DEVICE_BUSY                = 0x05,
  FIS_STS_INCORRECT_NONCE            = 0x06,
  FIS_STS_SECURITY_CHK_FAIL          = 0x07,
  FIS_STS_INVALID_SEC_STATE          = 0x08,
  FIS_STS_SYS_TIME_NOT_SET           = 0x09,
  FIS_STS_DATA_NOT_SET               = 0x0A,
  FIS_STS_ABORTED                    = 0x0B,
  FIS_STS_NO_NEW_FW                  = 0x0C, // Deprecated
  FIS_STS_REVISION_FAIL              = 0x0D,
  FIS_STS_INJECT_NOT_ENABLED         = 0x0E,
  FIS_STS_CONFIG_LOCKED              = 0x0F,
  FIS_STS_INVALID_ALIGNMENT          = 0x10,
  FIS_STS_INCOMPATIBLE_DIMM          = 0x11,
  FIS_STS_TIMEOUT                    = 0x12,
  FIS_STS_MEDIA_DISABLED             = 0x14,
  FIS_STS_FW_UPDATE_ALREADY_OCCURRED = 0x15,
  FIS_STS_NO_RESOURCES_AVAILABLE     = 0x16,
  FIS_STS_INVALID_ACTIVATION_VERSION = 0x17,
  FIS_STS_UNDEFINED                  = 0xFF  // None of FIS defined values
} FIS_STATUS;

//
// Media status codes.
//
typedef enum {
  FIS_MEDIA_READY    = 0x1,
  FIS_MEDIA_NOTREADY = 0x0,
  FIS_MEDIA_INERROR  = 0x2,
  FIS_MEDIA_RESERVED = 0x3,
  FIS_MEDIA_DISABLED = 0x4,
  FIS_MEDIA_UNKNOWN  = 0xFF
} FIS_MEDIA;

typedef enum {
  FIS_DDRT,
  FIS_SMBUS,
  FIS_IFC_MAX
} FIS_INTERFACE;

#define FIS_OPCODE_GET_MAJOR(Opcode) ((Opcode) & 0xFF)
#define FIS_OPCODE_GET_MINOR(Opcode) ((Opcode) >> 8 & 0xFF)

//
// Boot Status Register - describes NVDIMM firmware state.
// This register describes state of NVDIMM firmware for host software.
//
#pragma pack(1)
typedef union {
  UINT64   QWord;
  struct {
    UINT32 L;
    UINT32 H;
  } DWord;
  struct {
    UINT32 MajorCheckpoint          : 8,  ///< [7:0] Major Checkpoint
           MinorCheckpoint          : 8,  ///< [15:8] Minor Checkpoint
           MediaReady               : 2,  ///< [17:16] MR (0:Not Ready, 1:Ready, 2:Error, 3:Rsvd)
           DdrtIoInitComplete       : 1,  ///< [18] DT (0:Not Ready, 1:Ready)
           PcrAccessLocked          : 1,  ///< [19] PCR (0:Unlocked, 1:Locked)
           MailboxInterfaceReady    : 1,  ///< [20] MBR (0:Not Ready, 1:Ready)
           WatchdogTimerStatus      : 1,  ///< [21] WTS (0:No Change, 1:WDT NMI Generated)
           FirstRefreshCompletedFlag: 1,  ///< [22] FRCF (0:No Change, 1:First Refresh Cycle Completed)
           CreditReady              : 1,  ///< [23] CR (0:WDB Not Flushed, 1:WDB Flushed)
           MediaDisabled            : 1,  ///< [24] MD (0:Media Normal, 1:Media Disabled)
           SvnDowngradeEnabled      : 1,  ///< [25] SVNDE (0:Not Enabled, 1:Enabled)
           SvnDowngradeOptInEnabled : 1,  ///< [26] SVNCOIS (0:Never Enabled, 1:Has Been Enabled)
           DramReadyAit             : 2,  ///< [28:27] DR (0:Not Trained/Not Loaded, 1:Trained/Not Loaded, 2:Error, 3:Trained/Loaded)
           RebootRequired           : 1,  ///< [29] The DIMM internal state requires a platform power cycle
           LinkFailOnPrevBoot       : 1,  ///< [30] This field is preserved across warm reset or cold reboot
           SvnOptInWinClosed        : 1;  ///< [31] This field shows the current state of Opt-In window for opt-into the SVN feature
    UINT32 Reserved0                : 2,  ///< [33:32] Reserved
           DdrtTrainingStatus       : 2,  ///< [35:34] DDRT training status in current boot, not preserved across cold reboot
           FwActivateComplete       : 1,  ///< [36] FAC (0:FW Activate has not completed, 1:FW Activate has completed)
           Reserved1                :27;  ///< [63:37] Reserved
  } Bits;
} FIS_BOOT_STATUS_REGISTER;
#pragma pack()

#define FIS_BSR_MEDIA_NOTREADY 0
#define FIS_BSR_MEDIA_READY    1
#define FIS_BSR_MEDIA_ERROR    2

#define FIS_BSR_DRAM_NOTTRAINED_NOTLOADED 0
#define FIS_BSR_DRAM_TRAINED_NOTLOADED    1
#define FIS_BSR_DRAM_TRAINED_LOADED       3
#define FIS_BSR_DRAM_ERROR                2

#define FIS_BSR_TRAINING_NOT_CPLT 0
#define FIS_BSR_TRAINING_CPLT     1
#define FIS_BSR_TRAINING_FAILURE  2
#define FIS_BSR_S3_CPLT           3


/*****************************************************************************
 * Identify DIMM group
 *****************************************************************************/
//
// FIS request Identify DIMM :: Identify (IDENTIFY_DIMM)
// No request payload, response only.
//
#pragma pack(1)
typedef union {
  UINT32     DWord[NUM_PAYLOAD_REG];
  struct {
    UINT16   VendorId;
    UINT16   DeviceId;
    UINT16   RevisionId;  // Stepping
    UINT16   Ifc;         // Interface Format Code: 0x301
    struct {
      UINT16 BuildNo;
      UINT8  SecurityNo;
      UINT8  RevisionNo;
      UINT8  ProductNo;
    } FwRev;              // All numers in FwRev are BCD encoded
    UINT8    Reserved0;
    struct {
      UINT8  Ibbro    : 1,// Invalidate Before Block Read (Optional)
             Rbwr     : 1,// Readback of BW Address Register required before use
             Reserved1: 6;
    } Fswr;               // Feature SW Required Mask
    UINT8    Reserved2;
    UINT16   NumOfBlocWins;
    UINT8    Reserved3[10];
    UINT32   BlockCtrlDpa;
    UINT32   RawCapacity;
    UINT16   ManufId;
    UINT32   SerialNo;
    UINT8    PartNumer[20];
    struct {
      UINT32 MemoryModeEn : 1,
             StorageModeEn: 1,
             DirectModeEn : 1,
             PkgSparingEn : 1,
             Reserved4    :13,
             EncriptionEn : 1,
             Reserved5    :14;
    } Sku;
    UINT16   Ifce;
    UINT16   FisVer;      // Firmwaer Infterface Version BCD encoded
  } Bits;
} FIS_DIMM_IDENTIFY_RSP;
#pragma pack()

//
// FIS request Identify DIMM :: Device Characteristic (DEVICE_CHARACTERISTICS)
// No request payload, response only.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[NUM_PAYLOAD_REG];
  struct {
    UINT16 CoreShutdownThr;
    UINT16 MediaShutdownThr;
    UINT16 MediaThrottleStartThr;
    UINT16 MediaThrottleStopThr;
  } Bits;
} FIS_DIMM_CHARACTERISTICS_RSP;
#pragma pack()


/*****************************************************************************
 * Get Security Info group
 *****************************************************************************/
//
// FIS request Get Security Info :: Get Security State (GET_SECURITY_STATE)
// No request payload, response only.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[NUM_PAYLOAD_REG];
  struct {
    UINT32 Reserved0           : 1,
           SecurityEnabled     : 1,
           SecurityLocked      : 1,
           SecurityFrozen      : 1,
           UserPassLimitReached: 1,
           SecurityNotSupported: 1,
           SecurityNonceSet    : 1,
           Reserved1           : 1,
           MasterPassEnabled   : 1,
           MasterPassLimitReached:1,
           Reserved2           :22;
    UINT32 SecErasePolicyEn    : 1,
           Reserved3           :31;
  } Bits;
} FIS_GET_SECURITY_STATE_RSP;
#pragma pack()

//
// FIS request Get Security Info :: Get Security Opt-In (GET_SECURITY_OPT_IN)
// No request payload, response only.
//

#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT16  OptInCode;
  } Bits;
} FIS_GET_SECURITY_OPT_IN_REQ;

typedef union {
  UINT32   DWord[NUM_PAYLOAD_REG];
  struct {
    UINT16  OptInCode;
    UINT16  Reserved1;
    UINT32  OptInValue;
    UINT8   OptInModify;
    UINT8   Reserved2[3];
    UINT8   OptInWindow;
    UINT8   Reserved3[51];
    UINT64  OptInSpecificData[8];
  } Bits;
} FIS_GET_SECURITY_OPT_IN_RSP;
#pragma pack()


/*****************************************************************************
 * Set Security Info group
 *****************************************************************************/
//
// FIS request Set Security Info :: Set Security Nonce (SET_SECURITY_NONCE)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32 DWord[0];
  struct {
    UINT64  SecurityNonce;
  } Bits;
} FIS_SET_SECURITY_NONCE_REQ;
#pragma pack()

//
// FIS request Set Security Info :: Overvrite DIMM (OVERWRITE_DIMM)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT8  Passphrase[32];
  } Bits;
} FIS_OVERWRITE_DIMM_REQ;
#pragma pack()

//
// FIS request Set Security Info :: Set Security Opt-In (SET_SECURITY_OPT_IN)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT16  OptInCode;
    UINT16  Reserved;
    UINT32  OptInValue;
  } Bits;
} FIS_SET_SECURITY_OPT_IN_REQ;
#pragma pack()

//
// FIS request Set Security Info :: Set Master Passphrase (SET_MASTERPASSPHRASE)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT8  CurPassphrase[32]; // Currently used one for request authentication
    UINT8  Reserved[32];
    UINT8  NewPassphrase[32];
  } Bits;
} FIS_SET_MASTERPASSPHRASE_REQ;
#pragma pack()

//
// FIS request Set Security Info :: Set Passphrase (SET_PASSPHRASE)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT8  CurPassphrase[32]; // Currently used one for request authentication
    UINT8  Reserved[32];
    UINT8  NewPassphrase[32];
  } Bits;
} FIS_SET_PASSPHRASE_REQ;
#pragma pack()

//
// FIS request Set Security Info :: Disable Passphrase (DISABLE_PASSPHRASE)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT8  Passphrase[32];
  } Bits;
} FIS_DISABLE_PASSPHRASE_REQ;
#pragma pack()

//
// FIS request Set Security Info :: Unlock Unit (UNLOCK_UNIT)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT8  Passphrase[32];
  } Bits;
} FIS_UNLOCK_UNIT_REQ;
#pragma pack()

//
// FIS request Set Security Info :: Secure Erase Unit (SECURE_ERASE_UNIT)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT8  Passphrase[32];
  } Bits;
} FIS_SECURE_ERASE_UNIT_REQ;
#pragma pack()

//
// FIS request Set Security Info :: Freeze Lock (FREEZE_LOCK)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT8  Passphrase[32];
  } Bits;
} FIS_FREEZE_LOCK_REQ;
#pragma pack()

//
// FIS request Set Security Info :: S3 Authenticate (S3_AUTHENTICATE)
// Request payload only, no response payload defined.
//
#pragma pack(1)
#define FIS_S3_PASSPHRASE_SIZE 32
typedef union {
  UINT32   DWord[0];
  struct {
    UINT8  S3Passphrase[FIS_S3_PASSPHRASE_SIZE];
  } Bits;
} FIS_S3_AUTHENTICATE_REQ;

//
// FIS request Set Security Info :: S3 Passphrase (S3_PASSPHRASE)
// Request payload only, no response payload defined.
//
typedef union {
  UINT32   DWord[0];
  struct {
    UINT8  CurPassphrase[FIS_S3_PASSPHRASE_SIZE]; // Currently used one
    UINT8  Reserved[FIS_S3_PASSPHRASE_SIZE];
    UINT8  NewPassphrase[FIS_S3_PASSPHRASE_SIZE];
  } Bits;
} FIS_S3_PASSPHRASE_REQ;
#pragma pack()

/*****************************************************************************
 * Get Features group
 *****************************************************************************/

//
// FIS request Get Features :: Power Managemnt Policy (GET_POWER_MANAGEMENT)
// No request payload, response payload only.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[NUM_PAYLOAD_REG];
  struct {
    UINT8  Reserved[3];
    UINT16 AvgPwrLimit;
    UINT8  AvgPwrTimeConst;
    UINT8  TurboModeState;
    UINT16 TurboPwrLimit;
  } Bits;
} FIS_GET_POWER_MANAGEMENT_RSP;
#pragma pack()


//
// FIS request Get Features :: Alarm Thresholds (GET_ALARM_THRESHOLDS)
// No request payload, response payload only.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[NUM_PAYLOAD_REG];
  struct {
    UINT16 SpareBlocksThrEn : 1,
           MediaTempThrEn   : 1,
           CoreTempThrEn    : 1,
           Reserved         :13;
    UINT8  SpareBlocksThr;      // Per cent, 0 and 100 are not valid values
    UINT16 MediaTempThr;
    UINT16 CoreTempThr;
  } Bits;
} FIS_GET_ALARM_THRESHOLDS_RSP;
#pragma pack()

//
// FIS request Get Features :: Address Range Scrub (GET_ADDRESS_RANGE_SCRUB)
// No request payload, response payload only.
//
#pragma pack(1)
typedef union {
  UINT32 DWord[NUM_PAYLOAD_REG];
  struct {
    UINT8   Enabled;             // 1 if ARS running, 0 otherwise
    UINT8   Reserved[3];
    UINT64  DpaStartAddress;     // start DPA for the ARS
    UINT64  DpaEndAddress;       // end DPA for the ARS
    UINT64  DpaCurrentAddress;   // last DPA, that was scrubbed
  } Bits;
} FIS_GET_ARS_RSP;
#pragma pack()

//
// FIS request Get Features :: DDRT Alerts (GET_DDRT_ALERTS)
// No request payload, response payload only.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[NUM_PAYLOAD_REG];
  struct {
    UINT16 Level      : 2,         // 0 - do not log, 1 - low priority, 2 - high priority
           Reserved0  : 1,
           AitErrIntEn: 1,
           Reserved1  : 1,
           DpaErrIntEn: 1,
           Reserved2  : 1,
           UncErrIntEn: 1,
           DataPathErrIntEn: 1,
           Reserved3  : 1,
           IllegalAccessErrIntEn: 1,
           Reserved4  : 5;
  } TransactionType[NUM_PAYLOAD_REG / 2];
} FIS_GET_DDRT_ALERTS_RSP;
#pragma pack()

//
// FIS request Get Features :: Optional Configuration Data Policy (GET_CONFIG_DATA_POLICY)
// No request payload, response payload only.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[NUM_PAYLOAD_REG];
  struct {
    UINT32 FirstFastRefreshEn: 1,
           ViralPolicyEn     : 1,
           ViralStatus       : 1,
           Reserved          :29;
  } Bits;
} FIS_GET_CONFIG_DATA_POLICY_RSP;
#pragma pack()


/*****************************************************************************
 * Set Features group
 *****************************************************************************/

//
// FIS request Set Features :: Power Managemnt Policy (SET_POWER_MANAGEMENT)
// request payload only, no response payload.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT8  Reserved[3];
    UINT16 AvgPwrLimit;
    UINT8  AvgPwrTimeConst;
    UINT8  TurboModeState;
    UINT16 TurboPwrLimit;
  } Bits;
} FIS_SET_POWER_MANAGEMENT_REQ;
#pragma pack()

//
// FIS request Set Features :: Alarm Thresholds (SET_ALARM_THRESHOLDS)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32 DWord[0];
  struct {
    UINT16  SpareBlocksThrEn : 1,
            MediaTempThrEn   : 1,
            CoreTempThrEn    : 1,
            Reserved         :12;
    UINT8   SpareBlocksThr;
    UINT16  MediaTempThr;
    UINT16  CoreTempThr;
  } Bits;
} FIS_SET_ALARM_THRESHOLDS_REQ;
#pragma pack()


//
// FIS request Set Features :: Address Range Scrub (SET_ADDRESS_RANGE_SCRUB)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32 DWord[0];
  struct {
    UINT8   Enable;              // 1 to start the ARS, 0 to stop (if running)
    UINT8   Reserved[3];
    UINT64  DpaStartAddress;     // start DPA for the ARS
    UINT64  DpaEndAddress;       // end DPA for the ARS
  } Bits;
} FIS_SET_ARS_REQ;
#pragma pack()

//
// FIS request Set Features :: DDRT Alerts (SET_DDRT_ALERTS)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT16 Level      : 2,         // 0 - do not log, 1 - low priority, 2 - high priority
           Reserved0  : 1,
           AitErrIntEn: 1,
           Reserved1  : 1,
           DpaErrIntEn: 1,
           Reserved2  : 1,
           UncErrIntEn: 1,
           DataPathErrIntEn: 1,
           Reserved3  : 1,
           IllegalAccessErrIntEn: 1,
           Reserved4  : 5;
  } TransactionType[NUM_PAYLOAD_REG / 2];
} FIS_SET_DDRT_ALERTS_REQ;
#pragma pack()

//
// FIS request Set Features :: Optional Configuration Data Policy (SET_CONFIG_DATA_POLICY)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT32 FirstFastRefreshEn: 1,
           ViralPolicyEn     : 1,
           ViralStatusClear  : 1,
           Reserved          :29;
  } Bits;
} FIS_SET_CONFIG_DATA_POLICY_REQ;
#pragma pack()


/*****************************************************************************
 * Get Admin Features group
 *****************************************************************************/
//
// FIS request Get Admin Features :: System Time (GET_SYSTEM_TIME)
// No request payload, response payload only.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[NUM_PAYLOAD_REG];
  struct {
    UINT64 UnixTime;           // The number of seconds since Jun 1'th 1970
  } Bits;
} FIS_GET_SYSTEM_TIME_RSP;
#pragma pack()

//
// FIS request Get Admin Features :: Platform Config Data (GET_PLATFORM_CONFIG)
// Request used for all requests, response format depends on request:
// 1. JustSize == 1 && InSmallPayload == ANY
//    Partition size is retuned in FIS_GET_PLATFORM_CONFIG_RSP.Bits.PartitionSize
// 2. JustSize == 0 && InSmallPayload == 1
//    Response is in DWord[] table, contains 128 bytes (32 dwords) of raw data
//    read from PCD startin at requested offset.
// 3. JustSize == 0 && InSmallPayload == 0
//    Response is in large payload buffer, contains 128 kilobytes of raw data
//    i.e. whole PCD.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT8  PartitionId;        // Which Partition to access
    UINT8  InSmallPayload: 1,  // 0 - large payload, 1 - small payload
           JustSize      : 1,  // 0 - partition data, 1 - partition size
           Reserved      : 6;
    UINT32 Offset;             // Offset of the partition to start reading from
  } Bits;
} FIS_GET_PLATFORM_CONFIG_REQ;

typedef union {
  UINT32   DWord[NUM_PAYLOAD_REG];
  struct {
    UINT32 PartitionSize;      // Size of the partition
  } Bits;
} FIS_GET_PLATFORM_CONFIG_RSP;
#pragma pack()

//
// FIS request Get Admin Features :: Latch System Shutdown State (GET_LATCH_SYSTEM_SHUTDOWN_STATUS)
// No request payload, response payload only.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[NUM_PAYLOAD_REG];
  struct {
    UINT8  LatchSystemShutdownStateEn    : 1,
           Reserved0                     : 7;
    UINT8  PrevLatchSystemShutdownStateEn: 1,
           Reserved1                     : 7;
  } Bits;
} FIS_GET_LATCH_SYSTEM_SHUTDOWN_STATE_RSP;
#pragma pack()

//
// FIS request Get Admin Features :: eADR
//
#define FIS_EADR_DISABLE      0x00
#define FIS_EADR_ENABLE       0x01

//
// FIS request Get Admin Features :: Viral Policy (GET_VIRAL_POLICY)
// No request payload, response payload only.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[NUM_PAYLOAD_REG];
  struct {
    UINT32 ViralPolicyEn : 1,
           ViralStatus   : 1,
           Reserved      :30;
  } Bits;
} FIS_GET_VIRAL_POLICY_RSP;
#pragma pack()

//
// FIS request Get Admin Features :: Command Access Policy
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT32 Command  :16,     // 7:0 - Opcode, 15:8 - Subopcode
           Reserved :16;
  } Bits;
} FIS_GET_CMD_ACCESS_POLICY_REQ;

typedef union {
  UINT32   DWord[NUM_PAYLOAD_REG];
  struct {
    UINT32 SmmOnly  : 1,     // If set command is available on SMM/BIOS mailbox only
           Reserved :31;
  } Bits;
} FIS_GET_CMD_ACCESS_POLICY_RSP;
#pragma pack()


/*****************************************************************************
 * Set Admin Features group
 *****************************************************************************/
//
// FIS request Set Admin Features :: System Time (SET_SYSTEM_TIME)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT64  UnixTime;    // The number of seconds since Jun 1'th 1970
  } Bits;
} FIS_SET_SYSTEM_TIME_REQ;
#pragma pack()

//
// FIS request Set Admin Features :: Platform Config Data (SET_PLATFORM_CONFIG)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT8   PartitionId;    // Which partition to access
    UINT8   InSmallPayload; // 0 - Large Payload, 1 - Small Payload
    UINT32  Offset;         // Offset of the partition to start writing to
    UINT8   Reserved[58];
    UINT8   Data[64];
  } Bits;
} FIS_SET_PLATFORM_CONFIG_REQ;
#pragma pack()

//
// FIS request Set Admin Features :: Latch System Shutdown State (SET_LATCH_SYSTEM_SHUTDOWN_STATUS)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT32 LatchSystemShutdownStateEn: 1,
           Reserved                  :31;
  } Bits;
} FIS_SET_LATCH_SYSTEM_SHUTDOWN_STATE_REQ;
#pragma pack()

//
// FIS request Set Admin Features :: Viral Policy (SET_VIRAL_POLICY)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT32 ViralPolicyEn    : 1,
           ViralStatusClear : 1,
           Reserved         :30;
  } Bits;
} FIS_SET_VIRAL_POLICY_REQ;
#pragma pack()

//
// FIS request Set Admin Features :: Command Access Policy
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT32 Command  :16,     // 7:0 - Opcode, 15:8 - Subopcode
           SmmOnly  : 1,    // If set restricts command to SMM/BIOS mailbox only
           Reserved :15;
  } Bits;
} FIS_SET_CMD_ACCESS_POLICY_REQ;
#pragma pack()


/*****************************************************************************
 * Get Log Page group
 *****************************************************************************/
//
// FIS request Get Log Page :: SMART and Health Info (GET_SMART_HEALTH)
// No request payload, response payload only.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[NUM_PAYLOAD_REG];
  struct {
    UINT32 HealthStatusVal       : 1,// Byte 0:3
           PercentRemainingVal   : 1,
           PercentUsedVal        : 1,
           MediaTempVal          : 1,
           CoreTempVal           : 1,
           LDirtyShutdownCntVal  : 1,
           AitDramStatusVal      : 1,
           HealthStatusReasonVal : 1,
           Reserved0             : 1,
           AlarmTripsVal         : 1,
           LLastShutdownStatusVal: 1,
           VendorDataSizeVal     : 1,
           Reserved1             :20;
    UINT32 Reserved2;                // Byte 4:7
    UINT8  HealthStatus;             // Byte 8
    UINT8  PercentRemaining;         // Byte 9
    UINT8  PercentUsed;              // Byte 10
    UINT8  AlarmTrips;               // Byte 11
    UINT16 MediaTemp;                // Byte 12:13
    UINT16 CoreTemp;                 // Byte 14:15
    UINT32 LDirtyShutdownCnt;        // Byte 16:19
    UINT8  AitDramStatus;            // Byte 20
    UINT16 HealthStatusReason;       // Byte 22:21
    UINT8  Reserved3[8];             // Byte 23:30
    UINT8  LLastShutdownStatus;      // Byte 31
    UINT32 VendorDataSize;           // Byte 32:35
    struct {
      UINT64 PowerCycles;            // Byte 36:43
      UINT64 PowrOnTime;             // Byte 44:51
      UINT64 UpTime;                 // Byte 52:59
      UINT32 UDirtyShutdowns;        // Byte 60:63
      union {
        UINT8    Byte;
        struct {
          UINT8  PmAdrCmdRecv      : 1,
                 PmS3Recv          : 1,
                 PmS5REcv          : 1,
                 DdrtPwrFailCmdRecv: 1,
                 PmicDdrtPwrLoss   : 1,
                 PmWarmResetRecv   : 1,
                 ThermShutdownRecv : 1,
                 FwStateFlushCmpl  : 1;
        } Bits;
      } LLssDetails;                 // Byte 64
      UINT64 LastShutdownTime;       // Byte 65:72
      union {
        UINT8    Byte[3];
        struct {
          UINT8  ViralIntRecv      : 1,
                 SurpClkStopIntRecv: 1,
                 WrDataFlushCmpl   : 1,
                 PmS4Recv          : 1,
                 PmIdleRecv        : 1,
                 DdrtSurpResetRecv : 1,
                 Reserved4         : 2;
          UINT8  Reserved5         : 8;
          UINT8  Reserved6         : 8;
        } Bits;
      } LLssExtDetails;              // Byte 73:75
      UINT8  Reserved7[2];           // Byte 76:77
      union {
        UINT8    Byte;
        struct {
          UINT8  PmAdrCmdRecv      : 1,
                 PmS3Recv          : 1,
                 PmS5REcv          : 1,
                 DdrtPwrFailCmdRecv: 1,
                 PmicDdrtPwrLoss   : 1,
                 PmWarmResetRecv   : 1,
                 ThermShutdownRecv : 1,
                 FwStateFlushCmpl  : 1;
        } Bits;
      } ULssDetails;                 // Byte 78
      union {
        UINT8    Byte[3];
        struct {
          UINT8  ViralIntRecv      : 1,
                 SurpClkStopIntRecv: 1,
                 WrDataFlushCmpl   : 1,
                 PmS4Recv          : 1,
                 PmIdleRecv        : 1,
                 DdrtSurpResetRecv : 1,
                 Reserved4         : 2;
          UINT8  Reserved5         : 8;
          UINT8  Reserved6         : 8;
        } Bits;
      } ULssExtDetails;              // Byte 79:81
      UINT16 MaxMediaTemp;           // Byte 82:83
      UINT16 MaxCoreTemp;            // Byte 84:85
    } VendorData;
  } Bits;
} FIS_GET_SMART_HEALTH_RSP;
#pragma pack()

//
// FIS_GET_SMART_HEALTH_RSP::LastShutdownStatus
//
#define FIS_LSS_PLATFORM_CLEAN_SHUTDOWN 0


//
// FIS request Get Log Page :: Firmware Image Info (GET_FIRMWARE_IMAGE)
// No request payload, response payload only.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[NUM_PAYLOAD_REG];
  struct {
    struct {
      UINT16 BuildNo;
      UINT8  SecurityNo;
      UINT8  RevisionNo;
      UINT8  ProductNo;
    } FwRev;              // All numers in fw revision are BCD encoded
    UINT8    Reserved0;
    UINT16   MaxFwImageSize;  // MAX FW image size provided by DIMM FW
    UINT8    Reserved1[8];
    struct {
      UINT16 BuildNo;
      UINT8  SecurityNo;
      UINT8  RevisionNo;
      UINT8  ProductNo;
    } StgFwRev;           // All numers in staged fw revision are BCD encoded
    UINT8    Reserved2;
    UINT8    LastFwUpdateStatus;// See FIS_LFUSTATUS_* values
    UINT8    Reserved3;
  } Bits;
} FIS_GET_FIRMWARE_INFO_RSP;
#pragma pack()

#define FIS_LFUSTATUS_NOTRUN 1 // Image successfully staged but not yet run
#define FIS_LFUSTATUS_LOADED 2 // Last updated image loaded sucessfully
#define FIS_LFUSTATUS_FAILED 3 // Last updated image failed to loaded - fallback to prior image

//
// FIS request Get Log Page :: Long Operation Status (GET_LONG_OPERATION)
// No request payload, response payload only.
//
#pragma pack(1)
#define MAX_REPORTED_ERRORS_PER_DIMM  14
typedef union {
  UINT32   DWord[NUM_PAYLOAD_REG];
  struct {
    UINT16 Command;           // 7:0 - Opcode, 15:8 - Subopcode
    UINT16 PercentComplete;   // Percent complete, BCD format = XXX
    UINT32 Etc;               // Estimated Time to Completion in seconds
    UINT8  StatusCode;        // Mailbox status code of the last long operation
    union {                   // Union of data specific to the Command that was executed
      struct {
        UINT8  ErrorsFound;         // Number of errors found so far by ARS
        UINT8  ArsState;            // See FIS_LONGOP_ARSSTATE_* definitions
        UINT64 DpaErrorAddress[MAX_REPORTED_ERRORS_PER_DIMM]; // List of erroneous addresses found durning the ARS
      } Ars;
      // Currently only ARS has data defined for Long Operation Status response
    } CmdData;
  } Bits;
} FIS_GET_LONG_OPERATION_STATUS_RSP;
#pragma pack()

// Long Operation Status Info
#define FIS_LONGOP_ARSSTATE_NORMAL                         0x00
#define FIS_LONGOP_ARSSTATE_MAX_DPA_ERRORS_ADDRESS_REACHED 0x01
#define FIS_LONGOP_ARSSTATE_USER_ABORT_OR_POWER_CYCLE      0x02
#define FIS_LONGOP_ARSSTATE_WARM_RESET_ABORT               0x03

//
// FIS request Get Log Page :: Error Log (GET_ERROR_LOG)
//
#pragma pack(1)
typedef union {
  UINT32 DWord[0];
  struct {
    UINT8   LogLevel    : 1;    // 0 for LOW and 1 for HIGH level log
    UINT8   LogType     : 1;    // 0 for Media log, 1 for thermal
    UINT8   LogInfo     : 1;    // 0 for retrieving data and 1 for retrieving info
    UINT8   LogPayload  : 1;    // 0 for small payload, 1 for large payload
    UINT8   Rsvd        : 4;
    UINT16  SequenceNumber;     // sequence number from which to get logs, 0 for oldest available
    UINT16  RequestCount;       // how many logs to retrieve
  } Bits;
} FIS_GET_ERROR_LOG_REQ;

typedef union {
  UINT32   DWord[NUM_PAYLOAD_REG];
  struct {
    UINT16 MaxLogEntries;         // Total number of log entries FW has abitily to log before overflow
    UINT16 CurrentSequenceNumber; // Last assigned Sequence Number
    UINT16 OldestSequenceNumber;  // Oldest Sequence Number currently stored
    UINT64 OldestEntryTimestamp;  // Unix Epoch time of the oldest log entry
    UINT64 NewestEntryTimestamp;  // Unix Epoch time of the newest log entry
  } Bits;
} FIS_GET_ERROR_LOG_INFO_RSP;
#pragma pack()

//
// FIS request FW Debug Log :: Debug Log (GET_FIRMWARE_DEBUG)
//
#pragma pack(1)
typedef union {
  UINT32 DWord[0];
  struct {
    UINT8   LogAction;            // 0x00 - Retrieve Log Size
                                  // 0x01 - Get Log Page
                                  // 0x02 - Get SRAM Log
                                  // 0x03 - Get SPI Log Page
                                  // 0x80 - Clear Log Data
                                  // 0x81 - Store Log Data to SPI
    UINT32  LogPageOffset;
    UINT8   LogPayloadSize  : 1;  // 0x00 - Large Payload
                                  // 0x01 - Small Payload
    UINT16  Rsvd;
    UINT8   ForceCoreDump   : 1;  // FW will force Core Dump before retrieving the log data when set to 1
  } Bits;
} FIS_FW_DEBUG_LOG_REQ;
#pragma pack()

#define FIS_FW_DEBUG_LOG_RETRIEVE_LOG_SIZE      0x00
#define FIS_FW_DEBUG_LOG_GET_LOG_PAGE           0x01
#define FIS_FW_DEBUG_LOG_GET_SRAM_LOG           0x02
#define FIS_FW_DEBUG_LOG_GET_SPI_LOG_PAGE       0x03
#define FIS_FW_DEBUG_LOG_CLEAR_LOG_DATA         0x80
#define FIS_FW_DEBUG_LOG_STORE_LOG_DATA_TO_SPI  0x81

/*****************************************************************************
 * Update Firmware group
 *****************************************************************************/
//
// Parameters for FIS request Update Firmware :: Update FW (UPDATE_FNV_FW)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    struct {
      UINT32 Phase         : 2,
             PacketNo      :14,
             InSmallPayload: 1,
             Reserved      :15;
    } Header;
    UINT8  Data[64];
  } Bits;
} FIS_UPDATE_FW_REQ;
#pragma pack()

#define FIS_UPDATEFW_PHASE_INIT 0 // When first chunk is sent
#define FIS_UPDATEFW_PHASE_CONT 1
#define FIS_UPDATEFW_PHASE_END  2 // When last chunk is sent


/*****************************************************************************
 * Update Firmware group
 *****************************************************************************/
//
// Parameters for FIS request Inject Error :: Enable Injection (ENABLE_ERR_INJECTION)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32 DWord[0];
  struct {
    UINT8   Enable;         // 0 - Disable error injection, 1 - Enable error injection
  } Bits;
} FIS_ENABLE_ERR_INJECTION_REQ;
#pragma pack()

//
// Parameters for FIS request Inject Error :: Poison Error (INJECT_POISON_ERROR)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32 DWord[0];
  struct {
    UINT8   Enable;             ///< 0 for clear poison, 1 for set poison
    UINT8   Reserved;
    UINT8   MemoryType;         ///< 1 for 2LM, 2 for AppDirect, 3 for Storage, 4 for Patrol Scrub
    UINT8   Reserved2;
    UINT64  DpaPoisonAddress;   ///< Device physical address to set/clear poison
  } Bits;
} FIS_INJECT_POISON_ERROR_REQ;
#pragma pack()

//
// Parameters for FIS request Inject Error :: Media Temperature Error (INJECT_TEMP_ERROR)
// Request payload only, no response payload defined.
//
#pragma pack(1)
typedef union {
  UINT32   DWord[0];
  struct {
    UINT32 Enable   : 1,
           Reserved0: 7,
           MediaTemp:16,
           Reserved1: 8;
  } Bits;
} FIS_INJECT_MEDIA_TEMP_ERROR_REQ;
#pragma pack()

//
// Parameters for FIS request Inject Error :: Software Triggers (INJECT_SW_TRIGGER)
//
#pragma pack(1)
typedef union {
  UINT32 DWord[0];
  struct {
    UINT64  PackageSparingTriggerVal   : 1,
            SpareBlockAlarmTriggerVal  : 1,
            FatalErrorTriggerVal       : 1,
            SpareBlockPercentTriggerVal: 1,
            UnsafeShutdownTriggerVal   : 1,
            Reserved0                  :59;
    struct {
      UINT8 Enable    : 1,
            Reserved1 : 7;
    } PackageSparingTrigger;
    UINT8   Reserved2;
    struct {
      UINT8 Enable    : 1,
            Reserved  : 7;
    } SpareBlockAlarmTrigger;
    struct {
      UINT8 Enable    : 1,
            Reserved3 : 7;
    } FatalErrorTrigger;
    struct {
      UINT8 Enable    : 1,
            Percent   : 7;
    } SpareBlockPercentTrigger;
    struct {
      UINT8 Enable    : 1,
            Reserved4 : 7;
    } UnsafeShutdownTrigger;
  } Bits;
} FIS_INJECT_SOFTWARE_TRIGGER_REQ;
#pragma pack()


/******************************************************************************
 * Variables.
 ******************************************************************************/
extern UINT16 FisLongOpRunning[MAX_SOCKET][MAX_CH];
extern UINT32 FisLongOpSeqNum[MAX_SOCKET][MAX_CH];

/*****************************************************************************
 * Function prototypes.
 *****************************************************************************/

VOID
FisInit (VOID);

/**
  @brief Get the Boot Status Register value for given DIMM.

  @param[in]  FisIface  - FIS interface to use for the request.
  @param[in]  Skt       - Skt Number
  @param[in]  Ch        - DDR Channel ID
  @param[in]  Dimm      - DIMM Number
  @param[out] BsrPtr    - Pointer to buffer for BSR value

  @return EFI_STATUS is returned.
**/
EFI_STATUS
FisGetBootStatusRegister (
  IN  FIS_INTERFACE             FisIface,
  IN  UINT8                     Skt,
  IN  UINT8                     Ch,
  IN  UINT8                     Dimm,
  OUT FIS_BOOT_STATUS_REGISTER  *BsrPtr
  );

/**
 @brief Sends a command to NVDIMM via Small Payload registers as defined in FIS.

 Note that this function actively waits for NVDIMM FW response.

 @param[in]  Socket        - Socket Number
 @param[in]  Ch            - DDR Channel ID
 @param[in]  Dimm          - DIMM Number
 @param[in]  InputPayload  - Pointer to buffer containing Input Payload data (NULL if not needed)
 @param[in]  InputLength   - Length of input payload data (may be 0 if no input)
 @param[out] OutputPayload - Pointer to buffer for Output Payload data (NULL if not needed)
 @param[in]  Opcode        - Opcode of the command to be sent
 @param[in]  Timeout       - Optional timeout in us, use zero to assign default timeout
 @param[out] FisStsPtr     - Optional pointer to buffer for FIS status value (NULL if unused)

 @return NVDIMM status translated to EFI_STATUS is returned.

**/
EFI_STATUS
FisRequest (
  IN       UINT8  Socket,
  IN       UINT8  Ch,
  IN       UINT8  Dimm,
  IN CONST UINT32 *InputPayload,
  IN       UINT32 InputLength,
  OUT      UINT32 OutputPayload[NUM_PAYLOAD_REG],
  IN       UINT16 Opcode,
  IN       UINT64 Timeout,
  OUT FIS_STATUS *StsPtr
  );

/**
 @brief Sends a command to NVDIMM via Small Payload registers using SMBUS mailbox as defined in FIS.

 Note that this function actively waits for NVDIMM FW response.

 @param[in]  Socket        - Socket index
 @param[in]  Ch            - DDR Channel ID
 @param[in]  Dimm          - DIMM Number
 @param[in]  InputPayload  - Pointer to buffer containing Input Payload data (NULL if not needed)
 @param[in]  InputLength   - Length of input payload data (may be 0 if no input)
 @param[out] OutputPayload - Pointer to buffer for Output Payload data (NULL if not needed)
 @param[in]  Opcode        - Opcode of the command to be sent
 @param[in]  Timeout       - Optional timeout in us, use zero to assign default timeout
 @param[out] FisStsPtr     - Optional pointer to buffer for FIS status value (NULL if unused)

 @return NVDIMM status translated to EFI_STATUS is returned.

**/
EFI_STATUS
FisSmbusRequest (
  IN  UINT8       Skt,
  IN  UINT8       Ch,
  IN  UINT8       Dimm,
  IN  UINT32      *InputPayload,
  IN  UINT32      InputLength,
  OUT UINT32      OutputPayload[NUM_PAYLOAD_REG],
  IN  UINT16      Opcode,
  IN  UINT64      Timeout,
  OUT FIS_STATUS *FisStsPtr
  );

/**
  @brief Converts mailbox status to EFI status and message describing the status.

  @param[in]  MboxStatus   - FIS status value
  @param[in]  CmdOpcode    - The command that is being executed
  @param[in]  InputPayload  - Pointer to buffer containing Input Payload data (NULL if not needed)
  @param[out] ErrStrPtr    - Messsage describing the status

  @return EFI status value corresponding to mailbox status code is returned.
**/
EFI_STATUS
MailboxStatusDecode (
  IN       UINT8   MboxStatus,
  IN       UINT16  CmdOpcode,
  IN CONST UINT32  *InputPayload,
  OUT      CHAR8   **ErrStrPtr
  );

#endif // _FIS_H_

