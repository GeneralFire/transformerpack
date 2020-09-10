/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef _DSM_H_
#define _DSM_H_

/*****************************************************************************
 * Macro definitions.
 *****************************************************************************/

// DSM root fields definitions
#define VOLATILE_SCRUB_SUPPORTED        BIT0
#define PMEM_SCRUB_SUPPORTED            BIT1

#define DSM_START_ARS_FLAG_OPTIMIZE_FOR_PERFORMANCE BIT0
#define DSM_START_ARS_FLAG_PREVIOUS_SCRUB           BIT1

#define DSM_ROOT_FLAG_OVERFLOW          BIT0

#define DSM_ARS_ERROR_INJECT_OPTION_UNCONSUMED_UNCORRECTABLE_NOTIFICATION BIT0

#define DSM_QUERY_ARS_CAPABILITIES_FLAG_UNCONSUMED_UNCORRECTABLE_NOTIFICATION BIT0
#define DSM_QUERY_ARS_CAPABILITIES_FLAG_ARS_STOPPED_NOTIFICATION BIT1

//
// DSM root function indexes defined in ACPI 6.2 specification
//
#define DSM_ROOT_QUERY_ARS_CAPABILITES         1
#define DSM_ROOT_START_ARS                     2
#define DSM_ROOT_QUERY_ARS_STATUS              3
#define DSM_ROOT_CLEAR_UNCORRECTABLE_ERROR     4
#define DSM_ROOT_TRANSLATE_SPA                 5
#define DSM_ROOT_RESERVED                      6
#define DSM_ROOT_ARS_ERROR_INJECT              7
#define DSM_ROOT_ARS_ERROR_INJECT_CLEAR        8
#define DSM_ROOT_ARS_ERROR_INJECT_STATUS_QUERY 9
#define DSM_ROOT_STOP_ARS_AND_UPDATE_STATUS    0xFFFFFFFD
#define DSM_ROOT_GET_ARS_STATE                 0xFFFFFFFE // Internal function, returns ARS_STATE from ArsState call

#define DSM_ROOT_INTEL_GET_DEVICES_FW_RUNTIME_ACTIVATION_INFO 1
#define DSM_ROOT_INTEL_ACTIVATE_FIRMWARE                      2

#define DSM_ROOT_DEVICE_ACPI_HANDLE       0xFFFFFFFF
#define DSM_ROOT_INTEL_DEVICE_ACPI_HANDLE 0xFFFFFFFE

//
// DSM leaf function indexes defined in NVDIMM DSM spec 1.6
//
#define DSM_FN_GET_SMART_INFO                    1
#define DSM_FN_GET_SMART_THRESHOLD               2
#define DSM_FN_GET_BLOCK_FLAGS                   3
#define DSM_FN_GET_LABEL_SIZE                    4
#define DSM_FN_GET_LABEL_DATA                    5
#define DSM_FN_SET_LABEL_DATA                    6
#define DSM_FN_GET_VENDOR_LOG_SIZE               7
#define DSM_FN_GET_VENDOR_LOG                    8
#define DSM_FN_VENDOR_COMMAND                    9
#define DSM_FN_ENABLE_LATCH_SYSTEM_SHUTDOWN_STATUS 10
#define DSM_FN_GET_SUPP_MODES                   11
#define DSM_FN_GET_FWUPDATE_INFO                12
#define DSM_FN_START_FWUPDATE                   13
#define DSM_FN_SEND_FWUPDATE_DATA               14
#define DSM_FN_FINISH_FWUPDATE                  15
#define DSM_FN_QUERY_FWUPDATE                   16
#define DSM_FN_SET_SMART_THRESHOLD              17
#define DSM_FN_INJECT_ERROR                     18
#define DSM_FN_GET_SECURITY_STATE               19
#define DSM_FN_SET_PASSPHRASE                   20
#define DSM_FN_DISABLE_PASSPHRASE               21
#define DSM_FN_UNLOCK_UNIT                      22
#define DSM_FN_FREEZE_LOCK                      23
#define DSM_FN_SECURE_ERASE_NVDIMM              24
#define DSM_FN_OVERWRITE_NVDIMM                 25
#define DSM_FN_QUERY_OVERVRITE_STATUS           26
#define DSM_FN_SET_MASTERPASSPHRASE             27
#define DSM_FN_SECURE_ERASE_NVDIMM_MASTERPASSPHRASE 28
#define DSM_FN_GET_DEVICE_RUNTIME_FW_ACTIVATION_INFO 29
#define DSM_FN_SET_DEVICE_RUNTIME_FW_ACTIVATION_ARM_STATE 30

//
// These are internal functions, not true DSM functions.
//
#define DSM_FN_CHECK_LONGOP_STATE       0xFFFFFFFF

// DSM status codes
#define DSM_STS_SUCCESS             0x00
#define DSM_STS_NOT_SUPPORTED       0x01  // Function not supported
#define DSM_STS_NO_MEM_DEVICE       0x02
#define DSM_STS_INVALID_PARAMS      0x03
#define DSM_STS_HW_ERROR            0x04
#define DSM_STS_RETRY_SUGGESTED     0x05  // Temporary cannot execute
#define DSM_STS_UNKNOWN_FAIL        0x06
#define DSM_STS_VEN_SPECIFIC_ERROR  0x07  // Extended status defines function specific error
#define DSM_STS_OUTOFRESOURCES      0x08
#define DSM_STS_NOTREADY            0x09
#define DSM_STS_INVSECSTATE         0x0A  // Invalid security state
#define DSM_STS_INVPASSPHRASE       0x0B  // Invalid passphrase supplied

#define CREATE_DSM_EXT_STATUS(sts, extSts) (((sts) & 0xFFFF) | (((extSts) & 0xFFFF)<<16))

// DSM status extended codes
#define DSM_STS_EXT_INVALID_MB_OFFSET           0x01
#define DSM_STS_EXT_INVALID_BYTES_TO_XFER       0x02
#define DSM_STS_EXT_ERROR_BUFFER_OVERRUN        0x03
//
// DSM extended status codes for GetNamespaceLabelSize
//
#define DSM_STS_EXT_PMEM_LOCKED     0x01
//
// DSM extened status codes for Error Injection functions.
//
#define DSM_STS_EXT_INJECT_NOTENABLED  CREATE_DSM_EXT_STATUS (DSM_STS_VEN_SPECIFIC_ERROR, 1)

// DSM status codes for root device
#define DSM_STS_ROOT_SUCCESS           0x00
#define DSM_STS_ROOT_NOT_SUPPORTED     0x01
#define DSM_STS_ROOT_INVALID_PARAMS    0x02
#define DSM_STS_ROOT_HW_ERROR          0x03
#define DSM_STS_ROOT_RETRY_SUGGESTED   0x04
#define DSM_STS_ROOT_UNKNOWN_FAIL      0x05
#define DSM_STS_ROOT_FUNCTION_SPECIFIC 0x06

// DSM status extended codes for root device
#define DSM_STS_EXT_ROOT_QUERY_ARS_COMPLETED     CREATE_DSM_EXT_STATUS (DSM_STS_ROOT_SUCCESS, 0)
#define DSM_STS_EXT_ROOT_ARS_IN_PROGRESS         CREATE_DSM_EXT_STATUS (DSM_STS_ROOT_SUCCESS, 1)
#define DSM_STS_EXT_ROOT_ARS_NOT_PERFORMED       CREATE_DSM_EXT_STATUS (DSM_STS_ROOT_SUCCESS, 2)
#define DSM_STS_EXT_ROOT_ARS_STOPPED_PREMATURELY CREATE_DSM_EXT_STATUS (DSM_STS_ROOT_SUCCESS, 3)

//
// DSM extened status codes for DCPMM FW Update functions. Common for Start, Send, Finish and Query.
//
#define DSM_STS_EXT_UPDATE_CONTEXTINV    CREATE_DSM_EXT_STATUS (DSM_STS_VEN_SPECIFIC_ERROR, 1)
#define DSM_STS_EXT_UPDATE_INPROGRESS    CREATE_DSM_EXT_STATUS (DSM_STS_VEN_SPECIFIC_ERROR, 2)
#define DSM_STS_EXT_UPDATE_IMAGEAUTHFAIL CREATE_DSM_EXT_STATUS (DSM_STS_VEN_SPECIFIC_ERROR, 3)
#define DSM_STS_EXT_UPDATE_ABORTDONE     CREATE_DSM_EXT_STATUS (DSM_STS_VEN_SPECIFIC_ERROR, 4) // Finish only
#define DSM_STS_EXT_UPDATE_OUTOFSEQUENCE CREATE_DSM_EXT_STATUS (DSM_STS_VEN_SPECIFIC_ERROR, 4) // Query only

//
// DSM extened status codes for Overwrite NVDIMM function.
//
#define DSM_STS_EXT_OVERWRITE_NOTALLOWED    CREATE_DSM_EXT_STATUS (DSM_STS_VEN_SPECIFIC_ERROR, 1) // Start only
#define DSM_STS_EXT_OVERWRITE_INPROGRESS    CREATE_DSM_EXT_STATUS (DSM_STS_VEN_SPECIFIC_ERROR, 1) // Query only
#define DSM_STS_EXT_OVERWRITE_OUTOFSEQUENCE CREATE_DSM_EXT_STATUS (DSM_STS_VEN_SPECIFIC_ERROR, 2) // Query only

//
// Since FIS 1.10 NVDIMM FW image size is provided in FIS message Firmware Image Info.
// The size defined below is used with older FW for compatybility.
//
#define DSM_FIS10X_IMGSIZE CR_LARGE_PAYLOAD_SIZE // 256k is actually needed for 1.0x, but just use size of large payload

//
// The interval how often to query for firmware update status, when NVDIMM is
// updating its firmware. In units of microseconds.
//
#define DSM_FIS1XX_FWUPDATE_QUERY_INTERVAL   250000 // 250 ms
#define DSM_FIS1XX_FWUPDATE_QUERY_TIMEOUT   3000000 // 5 s
#define DSM_FIS2XX_FWUPDATE_QUERY_TIMEOUT  12000000 // 12 s TODO: verify in FIS 1.6 when available
#define DSM_FIS2XX_FWUPDATE_QUERY_INTERVAL   500000 // 500 ms TODO: verify in FIS 1.6 when available

#define DSM_FISVER2QUERYINTERVAL(v) (((v) < 0x0200) ? DSM_FIS1XX_FWUPDATE_QUERY_INTERVAL: DSM_FIS2XX_FWUPDATE_QUERY_INTERVAL)
#define DSM_FISVER2QUERYTIMEOUT(v)  (((v) < 0x0200) ? DSM_FIS1XX_FWUPDATE_QUERY_TIMEOUT : DSM_FIS2XX_FWUPDATE_QUERY_TIMEOUT)

//
// Definitions for Vendor Pass Through DSM that handles few simulated commands
// that are not sent to NVDIMM, but are used to read/write large payload.
//
#define DSM_EMULATED_COMMAND 0xFD
#define DSM_GET_LP_MB_INFO      0
#define DSM_WRITE_LP_INPUT_MB   1
#define DSM_READ_LP_OUTPUT_MB   2
#define DSM_GET_BOOT_STATUS     3
#define DSM_EXTENDED_VEN_SPEC   5

/*****************************************************************************
 * Type definitions.
 *****************************************************************************/
typedef enum {
  ACPI_DSM_ROOT,
  ACPI_DSM_DEVICE
} ACPI_DSM_TYPE;

//
// Parameters for _DSM function Get SMART Thresholds.
// Output only, no input.
//
#pragma pack(1)
typedef struct {
  UINT16 SpareBlocksThrEn : 1,
         MediaTempThrEn   : 1,
         CoreTempThrEn    : 1,
         Reserved         :13;
  UINT8  SpareBlocksThr;    // Spare blocks remaining percentage 1..99 %
  UINT16 MediaTempThr;
  UINT16 CoreTempThr;
} DSM_FNOUT_SMART_THRESHOLDS;
#pragma pack()

//
// Parameters for _DSM function Get Block NVDIMM Flags DSM_FN_GET_BLOCK_FLAGS.
// Output only, no input.
//
#pragma pack(1)
typedef struct {
  UINT32 BlockDataWinInvRequired: 1, // Block Data Window Invalidation Required
         ComRegBlockCtrlWinLatch: 1, // Command Register in Block Control Window Latch
         Reserved               :30;
} DSM_FNOUT_BLOCK_NVDIMM_FLAGS;
#pragma pack()

//
// Parameters for _DSM function Get Namespace Label Size DSM_FN_GET_LABEL_SIZE.
// Output only, no input.
//
#pragma pack(1)
typedef struct {
  UINT32 LabelAreaSize;
  UINT32 MaxLabelDataChunkSize;
} DSM_FNOUT_GET_NAMESPACE_LABEL_SIZE;
#pragma pack()

//
// Parameters for _DSM function Get Namespace Label Data DSM_FN_GET_LABEL_DATA.
//
#pragma pack(1)
typedef struct {
  UINT32 ChunkOffset;
  UINT32 ChunkSize;
} DSM_FNINP_GET_NAMESPACE_LABEL_DATA;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT8  ChunkData[0];
} DSM_FNOUT_GET_NAMESPACE_LABEL_DATA;
#pragma pack()

//
// Parameters for _DSM function Set Namespace Label Data DSM_FN_SET_LABEL_DATA.
// Input only, no output.
//
#pragma pack(1)
typedef struct {
  UINT32 ChunkOffset;
  UINT32 ChunkSize;
  UINT8  ChunkData[0];
} DSM_FNINP_SET_NAMESPACE_LABEL_DATA;
#pragma pack()

//
// Parameters for _DSM function Pass-Through Command DSM_FN_VENDOR_COMMAND.
// This DSM can send commands to the NVDIMM, but there are also emulated commands
// to read or write large payload.
//
#pragma pack(1)
typedef struct {
  struct {
    UINT16   OpCode;
    UINT16   Reserved;
    UINT32   DataSize;
  } Head;
  union {
    struct {
      UINT32 Payload[0];
    } Fis;
    struct {
      UINT32 Size;
      UINT32 Offset;
    } LpRead;
    struct {
      UINT32 Size;
      UINT32 Offset;
      UINT32 Payload[0];
    } LpWrite;
    struct {
      UINT16 FisCmd;
      UINT16 TransportInterface : 2,    ///< 0: DDRT, 1: SMBUS, 2: Reserved, 3: Reserved
             Reserved1          : 14;
      UINT32 Reserved2;
      UINT32 Timeout;                   ///< Timeout in milisecond, 0: use default timeout
      UINT32 Payload[0];                ///< Data buffer to be transferred to input payload registers
    } ExtVenSpec;
  } Data;
} DSM_FNINP_VENDOR_PASSTHROUGH;

typedef struct {
  struct {
    UINT32   DataSize;
  } Head;
  union {
    struct {
      UINT32 Payload[0];
    } Fis;
    struct {
      UINT32 InpPayloadSize;
      UINT32 OutPayloadSize;
      UINT32 DataChunkSize;
    } LpInfo;
    struct {
      UINT32 Payload[0];
    } LpData;
    struct {
      UINT32 Payload[0];       ///< NVMDIMM Command Output Data
    } ExtVenSpecData;
    UINT64   Bsr;
  } Data;
} DSM_FNOUT_VENDOR_PASSTHROUGH;
#pragma pack()

//
// Parameters for _DSM function Enable Latch System Shutdown Status DSM_FN_ENABLE_LATCH_SYSTEM_SHUTDOWN_STATUS.
// Input only, no output.
//
#pragma pack(1)
typedef struct {
  UINT8  LatchSystemShutdownStateEn: 1,
         Reserved                  : 7;
} DSM_FNINP_ENABLE_LATCH_SYSTEM_SHUTDOWN_STATUS;
#pragma pack()

//
// Parameters for _DSM function Get FW Info DSM_FN_GET_FWUPDATE_INFO.
// Output only, no input.
//
#pragma pack(1)
typedef struct {
  UINT32 FwStorageSize;
  UINT32 FwChunkSize;
  UINT32 QueryInterval;   // Suggested interval between query calls
  UINT32 QueryTimeout;
  UINT32 ResetColdAfterUpdate                         : 1,
         RuntimeFwActivationWithIoAndProcessorQuiesce : 1,
         Reserved0                                    : 30;
  UINT32 RunningFisVer;
  UINT64 RunningFwVer;
  UINT64 UpdatedFwVer;
} DSM_FNOUT_GET_FWUPDATE_INFO;
#pragma pack()

//
// Parameters for _DSM function Start FW Update DSM_FN_START_FWUPDATE.
// Output only, no input.
//
#pragma pack(1)
typedef struct {
  UINT32 Context;
} DSM_FNOUT_START_FWUPDATE;
#pragma pack()

//
// Parameters for _DSM function Send FW Update Data DSM_FN_SEND_FWUPDATE_DATA.
// Input only, no output apart of status.
//
#pragma pack(1)
typedef struct {
  UINT32 Context;
  UINT32 Offset;    // Offset of this fragment of FW image
  UINT32 Length;    // Length of this fragment of FW image
  UINT8  Data[0];
} DSM_FNINP_FWUPDATE_DATA;
#pragma pack()

//
// Parameters for _DSM function Finish FW Update.
// Input only, no output apart of status is provided.
//
#pragma pack(1)
typedef struct {
  struct {
    UINT8 Abort    : 1,
          Reserved0: 7;
  } Flag;
  UINT8   Reserved1[3];
  UINT32  Context;
} DSM_FNINP_FINISH_FWUPDATE;
#pragma pack()

//
// Parameters for _DSM function Query Finish FW Update DSM_FN_QUERY_FWUPDATE.
//
#pragma pack(1)
typedef struct {
  UINT32  Context;
} DSM_FNINP_QUERY_FWUPDATE;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT64 UpdatedFwVer;
} DSM_FNOUT_QUERY_FWUPDATE;
#pragma pack()

//
// Parameters for _DSM function Set SMART Thresholds DSM_FN_SET_SMART_THRESHOLD.
// Input only, no output apart of status is provided.
//
#pragma pack(1)
typedef struct {
  UINT16 SpareBlocksThrEn : 1,
         MediaTempThrEn   : 1,
         CoreTempThrEn    : 1,
         Reserved         :13;
  UINT8  SpareBlocksThr;       // Spare blocks remaining percentage 1..99 %
  UINT16 MediaTempThr;
  UINT16 CoreTempThr;
} DSM_FNINP_SMART_THRESHOLDS;
#pragma pack()

//
// Parameters for _DSM function Inject Error DSM_FN_INJECT_ERROR.
// Input only, no output apart of status is provided.
//
#pragma pack(1)
typedef struct {
  UINT64               MediaTempVal  : 1,
                       SpareBlocksVal: 1,
                       FatalErrorVal : 1,
                       DseFailureVal : 1, // Dirty Shutdown Error
                       LsaErrorVal   : 1,
                       Reserved0     :59;
  struct {
    UINT8              Enable        : 1,
                       Reserved0     : 7;
    UINT16             MediaTemp;
  }  MediaTempError;    // Media Temperature Error definition is consistant with FIS message
  struct {
    UINT8              Enable        : 1,
                       Reserved0     : 7;
    UINT8              Percent;           // Spare blocks remaining to inject 0..99 %
  } SpareBlocks;
  struct {
    UINT8              Enable        : 1,
                       Reserved0     : 7;
  } FatalError;
  struct {
    UINT8              Enable        : 1, // Enable Dirty Shutdown Error
                       Reserved0     : 7;
  } DseFailure;
  struct {
    UINT8             Enable         : 1,
                      Reserved0      : 7;
    UINT8             Reserved1[3];
    UINT32            ErrorInjectionOffset;  // Error Injection Offset in bytes
    UINT32            ErrorInjectionLength;  // Error Injection Length in bytes
  } LsaError;
} DSM_FNINP_INJECT_ERROR;
#pragma pack()

//
// Parameters for _DSM function Get Security State DSM_FN_GET_SECURITY_STATE.
// Output only, no input.
//
#pragma pack(1)
typedef struct {
  UINT32 MasterPassEnabled     : 1,
         MasterPassLimitReached: 1,
         Reserved0             :30;
  UINT8  Reserved1             : 1,
         SecurityEnabled       : 1,
         SecurityLocked        : 1,
         SecurityFrozen        : 1,
         UserPassLimitReached  : 1,
         SecurityNotSupported  : 1,
         SecurityNonceSet      : 1,
         Reserved2             : 1;
} DSM_FNOUT_GET_SECURITY_STATE;
#pragma pack()

//
// Parameters for _DSM function Set Passphrase DSM_FN_SET_PASSPHRASE.
// Input only, no output apart of status is provided.
//
#pragma pack(1)
typedef struct {
  UINT8 CurPassphrase[32];
  UINT8 NewPassphrase[32];
} DSM_FNINP_SET_PASSPHRASE;
#pragma pack()

//
// Parameters for _DSM function Disable Passphrase DSM_FN_DISABLE_PASSPHRASE.
// Input only, no output apart of status is provided.
//
#pragma pack(1)
typedef struct {
  UINT8 Passphrase[32];
} DSM_FNINP_DISABLE_PASSPHRASE;
#pragma pack()

//
// Parameters for _DSM function Disable Passphrase DSM_FN_UNLOCK_UNIT.
// Input only, no output apart of status is provided.
//
#pragma pack(1)
typedef struct {
  UINT8 Passphrase[32];
} DSM_FNINP_UNLOCK_UNIT;
#pragma pack()

//
// Parameters for _DSM function Secure Erase NVDIMM DSM_FN_SECURE_ERASE_NVDIMM.
// Input only, no output apart of status is provided.
//
#pragma pack(1)
typedef struct {
  UINT8 Passphrase[32];
} DSM_FNINP_SECURE_ERASE_NVDIMM;
#pragma pack()

//
// Parameters for _DSM function Overwrite NVDIMM DSM_FN_OVERWRITE_NVDIMM.
// Input only, no output apart of status is provided.
//
#pragma pack(1)
typedef struct {
  UINT8 Passphrase[32];
} DSM_FNINP_OVERWRITE_NVDIMM;
#pragma pack()

//
// Parameters for _DSM function Set Master Passphrase DSM_FN_SET_MASTERPASSPHRASE.
// Input only, no output apart of status is provided.
//
#pragma pack(1)
typedef struct {
  UINT8 CurPassphrase[32];
  UINT8 NewPassphrase[32];
} DSM_FNINP_SET_MASTERPASSPHRASE;
#pragma pack()

//
// Parameters for root _DSM function Query ARS Capabilities
//
#pragma pack(1)
typedef struct {
  UINT64 ArsStartAddress;
  UINT64 ArsLength;
} ROOT_DSM_FNINP_QUERY_ARS_CAPABILITIES;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT32 MaxQueryArsStatusOutputBufferSize;
  UINT32 ClearUncorrectableUnitSize;
  UINT16 Flags;
  UINT8  Reserved[2];
} ROOT_DSM_FNOUT_QUERY_ARS_CAPABILITIES;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT64 ArsStartAddress;
  UINT64 ArsLength;
  UINT16 ArsType;
  UINT8  ArsFlags;
  UINT8  Reserved[5];
} ROOT_DSM_FNINP_START_ARS;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT32 EstimatedTime;
} ROOT_DSM_FNOUT_START_ARS;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT32           OutputLength;    // Output size of the ARS Output Header
  UINT64           StartSpa;        // Start System Physical Address
  UINT64           Length;          // ARS Length
  UINT64           RestartSpa;      // Restart start address of SPA address of ARS
  UINT64           RestartLength;   // Length of region that need to scrubed after restart
  UINT16           Type;            // if Bit0 is set, it is Volatile & if Bit1 is set it is Non-Volatile, both if both the bits are set
  UINT16           Flags;           // Bit[0] - If set to 1, indicates an overflow condition has occurred
  UINT32           NumErrorRecords; // Number of error records defined in this output
  ARS_ERROR_RECORD ErrorRecords[0];
} ROOT_DSM_FNOUT_QUERY_ARS_STATUS;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT64 SpaRangeBase;
  UINT64 SpaRangeLength;
} ROOT_DSM_FNINP_CLEAR_UNCORRECTABLE_ERROR;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT8  Reserved[4];
  UINT64 ErrorRangeLength; // The range of errors actually cleared by the platform
} ROOT_DSM_FNOUT_CLEAR_UNCORRECTABLE_ERROR;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT64 SpaRangeBase;
  UINT64 SpaRangeLength;
  UINT8  Options;
} ROOT_DSM_FNINP_ARS_ERROR_INJECT;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT64 SpaRangeBase;
  UINT64 SpaRangeLength;
} ROOT_DSM_FNINP_ARS_ERROR_INJECT_CLEAR;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT64 SpaRangeBase;
  UINT64 SpaRangeLength;
} ARS_ERROR_INJECT_STATUS_QUERY_ERROR_RECORD;
#pragma pack()

#pragma pack(1)
typedef struct {
  // Status is relayed via dedicated field
  UINT32                                     ErrorRecordCount;
  ARS_ERROR_INJECT_STATUS_QUERY_ERROR_RECORD ErrorRecords[0];
} ROOT_DSM_FNOUT_ARS_ERROR_INJECT_STATUS_QUERY;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT16 Status;
  UINT16 ExtendedStatus;
  UINT16 Reserved;
  UINT8  ActivationState;
  UINT8  ActivationCapability;
  UINT64 EstimatedFwActivationTime;
  UINT64 EstimatedProcessorQuiesceTime;
  UINT64 EstimatedIoAccessToMemoryQuiesceTime;
  UINT64 PlatformFwSupportedMaxIoAccessToMemoryQuiesceTime;
} ROOT_DSM_INTEL_FNOUT_GET_DEVICES_FW_RUNTIME_ACTIVATION_INFO;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT8 IoDeviceState;
} ROOT_DSM_INTEL_FNINP_ACTIVATE_FIRMWARE;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT16 Status;
  UINT16 ExtendedStatus;
} ROOT_DSM_INTEL_FNOUT_ACTIVATE_FIRMWARE;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT16 LastFwActivationStatus;
  UINT8  ActivationState;
  UINT8  Reserved;
} DSM_FNOUT_GET_DEVICE_RUNTIME_FW_ACTIVATION_INFO;
#pragma pack()

#pragma pack(1)
typedef struct {
  UINT8  ArmState;
} DSM_FNINP_SET_DEVICE_RUNTIME_FW_ACTIVATION_ARM_STATE;
#pragma pack()


//
// Parameters for _DSM function Translate SPA
// Output and input.
//
#pragma pack(1)
typedef struct _TRANSLATED_NVDIMM_DEVICE {
  NFIT_DEVICE_HANDLE      NfitHandle;          // Nfit handle to NVDIMM that the SPA maps to
  UINT32                  Reserved;            // 4 bytes reserved
  UINT64                  Dpa;                 // DIMM Physical Address that the SPA translates to
} TRANSLATED_NVDIMM_DEVICE;
#pragma pack()

#pragma pack(1)
typedef struct _ROOT_DSM_FNOUT_TRANSLATE_SPA {
  UINT8                    Flags;                            // Flags, bit[0]-if set to 1, indicates the SPA location maps to one or more NVDIMMs that are mirrored together
  UINT8                    Reserved[3];                      // 3 bytes reserved
  UINT64                   TranslatedLength;                 // The number of bytes the returned SPA translation applies to
  UINT32                   NumberOfNvdimms;                  // The number of NVDIMM devices beiing returned in the list of Translated NVDIMM Devices
  TRANSLATED_NVDIMM_DEVICE TranslatedNvdimmDeviceList[0];    // List of one or more Translated NVDIMM Devices
} ROOT_DSM_FNOUT_TRANSLATE_SPA;
#pragma pack()

#pragma pack(1)
typedef struct _ROOT_DSM_FNINP_TRANSLATE_SPA {
  UINT64 Spa;
} ROOT_DSM_FNINP_TRANSLATE_SPA;
#pragma pack()

/*****************************************************************************
 * Function prototypes.
 *****************************************************************************/
EFI_STATUS
DsmProcessUpdateResults (
  IN UINT8 Skt,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN FIS_GET_LONG_OPERATION_STATUS_RSP *FisOpStatusPtr
  );
EFI_STATUS
DsmProcessOverwriteDimmResults (
  IN UINT8 Skt,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN FIS_GET_LONG_OPERATION_STATUS_RSP *FisOpStatusPtr
  );

/**
  Populates appropriate ACPI buffer with updated Query ARS Status data.
**/
VOID
DsmUpdateArsStatusData (
  IN BOOLEAN UpdateState
  );

VOID
DsmReadFisCmdAccessPolicy (
  VOID
  );

VOID
DsmUpdateFisCmdAccessPolicy (
  IN UINT8   Skt,
  IN UINT8   Ch,
  IN UINT16  Command,
  IN BOOLEAN IsSmmOnly
  );

EFI_STATUS
EFIAPI
DsmServeNgnCommandsFromOS (
  IN       EFI_HANDLE                   DispatchHandle,
  IN CONST EFI_SMM_SW_REGISTER_CONTEXT  *DispatchContext,
  IN OUT   VOID                         *CommBuffer,     OPTIONAL
  IN OUT   UINTN                        *CommBufferSize  OPTIONAL
  );

/**
  Determines if ARS stop should produce overflow condition.

  @return TRUE  ARS stop should always produce overflow condition.
  @return FALSE ARS stop should produce overflow condition only if indicated in FIS response
                by FIS_LONGOP_ARSSTATE_MAX_DPA_ERRORS_ADDRESS_REACHED

**/
BOOLEAN
DsmStopArsWithOverflow (
  VOID
  );


VOID
GetDimmInfoFromDevHandle (
  OUT UINT8       *Socket,
  OUT UINT8       *Ch,
  OUT UINT8       *Dimm,
  IN  UINT32      DevHandle
  );


EFI_STATUS
DsmFisRequest (
  IN       UINT8  Skt,
  IN       UINT8  Ch,
  IN       UINT8  Dimm,
  IN CONST UINT32 *InputPayload,
  IN       UINT32 InputLength,
  OUT      UINT32 OutputPayload[NUM_PAYLOAD_REG],
  IN       UINT16 Opcode,
  IN       UINT64 Timeout,
  OUT FIS_STATUS *FisStsPtr
  );

#endif  // _DSM_H_

