/** @file
  Header file for NVM tech Access protocol

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation. <BR>

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

#ifndef _CRYSTAL_RIDGE_PROTOCOL_H_
#define _CRYSTAL_RIDGE_PROTOCOL_H_

#define MAX_HOST_TRANSACTIONS    64
#define NUM_HOST_TRANSACTIONS    14

//
// Crystal Ridge Protocol Revisions
//
#define EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_1   1
#define EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_2   2
#define EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_3   3
#define EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_4   4
#define EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_5   5
#define EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_6   6
#define EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_7   7
#define EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_8   8
#define EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_9   9
#define EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_10  10
#define EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_CUR EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_10

//#define MAX_HOST_ALERTS      128 // 4 (num of bytes in a payload) * 32 ( num of payloads)
//#define NUM_HOST_ALERTS      16

typedef enum _FIS_VERSION {
  FIS_1_06 = 0x0106,
  FIS_1_12 = 0x0112,
  FIS_1_13 = 0x0113,
  FIS_2_02 = 0x0202,
  FIS_2_03 = 0x0203,
  FIS_2_04 = 0x0204,
  FIS_3_00 = 0x0300,
  FisMax = 0xFFFF
} FIS_VERSION;

#define MAX_PRIORITY            2
#define NO_LOG                  0
#define LOW_PRIORITY            1           // BIT0
#define HIGH_PRIORITY           2           // BIT1
#define NEW_ENTRIES_ONLY        ?


#define ENABLE_FLAG                  1
#define DISABLE_FLAG                 0

#define FNVERR_MASK                    0x00FFFFFF
#define FNVERR_NGNVM_ERROR_LOG         BIT0
#define FNVERR_THERMAL_SELF_THROTTLING BIT1
#define FNVERR_LONGOP_COMPLETE         BIT2
// FIS 1.6 alerts, kept for backward compatibility
#define FNVERR_FW_HANG                 BIT3
#define FNVERR_FATAL                   BIT4
// FIS 1.13 media and therm log alerts
#define FNVERR_MEDIA_ERROR_LOG          BIT0
#define FNVERR_THERMAL_ERROR_LOG        BIT1

//
// Signaling options on per priority (bucket) level. Strongly recommend to keep them mutually exclusive, aka set only one of them per bucket
//
#define SMI_FLAG                BIT0  // in-band SMI
#define ERROR0                  BIT1  // ERROR0# pin signaling
#define LOW_PRIORITY_EVENT      0
#define HIGH_PRIORITY_EVENT     1

#define CLEAR_ON_READ          TRUE
#define DO_NOT_CLEAR_ON_READ   FALSE

//
// FNV transaction types - delete these if these becomes available in FnvAccess.h
//
#define FNV_TT_2LM_READ             0
#define FNV_TT_2LM_WRITE            1
#define FNV_TT_PM_READ              2
#define FNV_TT_PM_WRITE             3
#define FNV_TT_BW_READ              4
#define FNV_TT_BW_WRITE             5
#define FNV_TT_AIT_READ             6
#define FNV_TT_AIT_WRITE            7
#define FNV_TT_WEAR_LEVEL_MOVE      8
#define FNV_TT_PATROL_SCRUB         9
#define FNV_TT_CSR_READ             10
#define FNV_TT_CSR_WRITE            11
#define FNV_TT_ADDRESS_RANGE_SCRUB  12
#define FNV_TT_UNAVAILABLE          13

#define LOW_LEVEL_LOG             0
#define HIGH_LEVEL_LOG            1
#define LOG_LEVEL_NUM             2
#define RETRIEVE_LOG_DATA         0
#define RETRIEVE_LOG_INFO         1
#define MEDIA_LOG_REQ             0
#define THERMAL_LOG_REQ           1
#define LOG_TYPE_NUM              2
#define MAX_MEDIA_LOGS            3
#define MAX_THERMAL_LOGS          7
#define MAX_MEDIA_PAYLOADS         (((MAX_MEDIA_LOG_ENTRIES) + (MAX_MEDIA_LOGS) - 1) / (MAX_MEDIA_LOGS) * 2)
#define MAX_THERMAL_PAYLOADS       (((MAX_THERMAL_LOG_ENTRIES) + (MAX_THERMAL_LOGS) - 1) / (MAX_THERMAL_LOGS) * 2)
#define MAX_MEDIA_LOG_ENTRIES     9
#define MAX_THERMAL_LOG_ENTRIES  14
#define NGN_ERROR_LOG_MIN_SEQ_NUM  1
#define NGN_ERROR_LOG_MAX_SEQ_NUM MAX_UINT16

#define ERROR_LOG_HEAD_SIZE 2

#define FNV_ET_UNCORRECTABLE               0
#define FNV_ET_DPA_MISMATCH                1
#define FNV_ET_AIT_ERROR                   2 // deprecated
#define FNV_ET_DATA_PATH_ERROR             3
#define FNV_ET_LOCKED_ILLEGAL_ACCESS       4
#define FNV_ET_USER_PERCENT_REM_ALARM_TRIP 5
#define FNV_ET_SMART_HEALTH_STATUS_CHANGED 6
#define FNV_ET_PERSISTENT_WRITE_ECC        7

#define FNV_HRTD_SIGN_POSITIVE 0
#define FNV_HRTD_SIGN_NEGATIVE 1

#define FNV_HRTD_REPORTED_USER_ALARM_TRIP   0
#define FNV_HRTD_REPORTED_ENTERED_LOW_ZONE  1
#define FNV_HRTD_REPORTED_ENTERED_HIGH_ZONE 2
#define FNV_HRTD_REPORTED_CRITICAL          4

#define FNV_HRTD_TEMP_TYPE_MEDIA 0
#define FNV_HRTD_TEMP_TYPE_CORE  1

#define NGN_MAX_MANUFACTURER_STRLEN 2
#define NGN_MAX_SERIALNUMBER_STRLEN 4
#ifdef DDR5_SUPPORT
#define NGN_MAX_PARTNUMBER_STRLEN   30
#else
#define NGN_MAX_PARTNUMBER_STRLEN   20
#endif //#ifdef DDR5_SUPPORT
#define NGN_MAX_UID_STRLEN          9

#define NGN_FW_VER_LEN              4
// FIS 1.12 Viral related macros
#define FIS_VIRAL_STANDALONE    0x0112

#define VALID_DPA               BIT1

#pragma pack(1)

typedef struct _FW_VERSION_ {
  UINT8  MajorVersion;
  UINT8  MinorVersion;
  UINT8  HotfixVersion;
  UINT16 BuildVersion;
} DCPMM_FW_VERSION;

typedef struct _MEDIA_ERR_LOG_ {
  UINT64      SysTimeStamp;        // Unix Epoch time of the log entry
  UINT64      Dpa;                 // Specifies the Dpa address of the error
  UINT64      Pda;                 // Specifies the PDA address of the failure
  UINT8       Range;               // Specifies the length in address space of the error
  UINT8       ErrorType;           // Indicates the kind of error that was logged.
  UINT8       ErrorFlags;          // Indicates the error flags for this entry
  UINT8       TransactionType;     // Indicates what transaction caused the error
  UINT16      SequenceNumber;      // Incremented for each log entry added to the selected log
  UINT16      Rsvd;
} MEDIA_ERR_LOG, *PMEDIA_ERR_LOG;

typedef struct _MEDIA_ERR_LOG_DATA_ {
  UINT16         ReturnCount;     // Total number of entries in the log
  MEDIA_ERR_LOG  MediaLogs[MAX_MEDIA_LOGS];
} MEDIA_ERR_LOG_DATA, *PMEDIA_ERR_LOG_DATA;

typedef struct _MEDIA_ERR_LOG_INFO_DATA_ {
  UINT16      MaxLogEntries;          // Total number of log entries FW has abitily to log before overflow
  UINT16      CurrentSequenceNumber;  // Last assigned Sequence Number
  UINT16      OldestSequenceNumber;   // Oldest Sequence Number currently stored
  UINT64      OldestEntryTimestamp;   // Unix Epoch time of the oldest log entry
  UINT64      NewestEntryTimestamp;   // Unix Epoch time of the newest log entry
} ERR_LOG_INFO_DATA, MEDIA_ERR_LOG_INFO;

typedef struct _THERMAL_ERR_LOG_ {
  UINT64      SysTimestamp;         // Unix Epoch time of the log entry
  UINT32      Temperature   : 15;   // Host reported temperature (0.0625 C resolution)
  UINT32      Sign          : 1;    // Temperature sign, 0 - positive, 1 - negative
  UINT32      Reported      : 3;    // Temperature being reported
  UINT32      TempType      : 2;    // This denotes which device the temperature is for
  UINT32      Rsvd1         : 11;
  UINT16      SequenceNumber;       // Incremented for each log entry added to the selected log
  UINT16      Rsvd2;
} THERMAL_ERR_LOG;

typedef struct _THERMAL_ERR_LOG_DATA_ {
  UINT16          ReturnCount;    // Total number of entries in the log
  THERMAL_ERR_LOG ThermalLogs[MAX_THERMAL_LOGS];
} THERMAL_ERR_LOG_DATA;

typedef union {
  MEDIA_ERR_LOG_DATA   MediaLogData;
  THERMAL_ERR_LOG_DATA ThermalLogData;
  ERR_LOG_INFO_DATA    LogInfoData;
} ERR_LOG_DATA;

typedef struct {
  UINT8               ViralPolicyEnable;  // Current Enable/Disable state of Viral Policies in the DCPMM
  UINT8               ViralClear;         // Current Viral/Not Viral status of the DCPMM Module
} VIRAL_POLICY;

typedef struct _NVDIMM_INFO_ {
  UINT16            VendorId;                                   // Vendor Identification
  UINT16            DeviceId;                                   // Device Identification
  UINT16            RevisionId;                                 // Revision Identification
  UINT16            FisVersion;                                 // Firmware Interface Specification version
  UINT16            Ifc;                                        // Interface Format Code
  UINT8             Sku;                                        // DIMM SKU
  UINT8             Manufacturer[NGN_MAX_MANUFACTURER_STRLEN];  // Manufacturer
  UINT8             SerialNumber[NGN_MAX_SERIALNUMBER_STRLEN];  // Serial Number
  UINT8             PartNumber[NGN_MAX_PARTNUMBER_STRLEN];      // Part Number
  DCPMM_FW_VERSION  FirmwareVersion;                            // Firmware Version
} NVDIMM_INFO;


#pragma pack()

//
// Fields in the Get/Set Feature - Optional Configuration Data Policy
// read and sent using NvmCtlrGetCfgDataPolicy() & NvmCtlrSetCfgDataPolicy()
// Common Data Policy field values
//
#define CFG_DATA_FIRST_FAST_REFRESH_ENABLE 0x01
#define CFG_DATA_VIRAL_POLICY_ENABLE       0x0100
//
// Set Feature/Optional Configuration Data Policy field value
//
#define CFG_DATA_VIRAL_STATUS_CLEAR        0x010000
//
// Get Feature/Optional Configuration Data Policy field value
//
#define CFG_DATA_VIRAL_STATUS              0x010000
//
// Set Viral Policy (Standalone) field value
//
#define CFG_VIRAL_POLICY_ENABLE_STANDALONE 0x01
#define CFG_VIRAL_CLEAR_STANDALONE         0x0100

// For poison error enable parameter
#define POISON_ENABLE_CLEAR             0x00
#define POISON_ENABLE_SET               0x01

// Memory types for poison error injection
#define POISON_MEMORY_TYPE_CLEAR        0x00
#define POISON_MEMORY_TYPE_2LM          0x01
#define POISON_MEMORY_TYPE_APP_DIRECT   0x02
#define POISON_MEMORY_TYPE_PATROL_SCRUB 0x04

/**

  Routine Description: Is this Dimm the NVMDIMM

  @param Socket      - Socket Number
  @param Ch          - DDR Channel ID
  @param Dimm        - DIMM number

  @return BOOLEAN     - TRUE if NVMDIMM otherwise false

**/
typedef
BOOLEAN
(EFIAPI *IS_NVM_DIMM) (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm
  );


/*
  Routine Description: Does this system have at least one NVMDIMM

  @return TRUE if NVMDIMM present
**/
typedef
BOOLEAN
(EFIAPI *IS_SYSTEM_WITH_DCPMM)  (
  VOID
  );

/**

  Routine Description: This function returns the total number of Ngn Dimms
  installed in the system.

  @return UINT32 - Number of Ngn Dimms installed in the system.

**/
typedef
UINT32
(EFIAPI *GET_NUM_NVM_DIMMS) (
  VOID
  );

/**

  Routine Description: This function checks if the given address falls into
  NVMDIMM space or not.

  @param SystemAddress - System Physical Address

  @return BOOLEAN      - True if the address is NVMDIMM address else false

**/
typedef
BOOLEAN
(EFIAPI *IS_NVM_ADDRESS) (
  IN UINT64 Spa
  );

/**

  Routine Description: This function converts given Dpa
  to Spa and also populates the socket channel and dimm information with
  only system phy address as the input.
  CR protocol function.

  @param[in]  Spa        - System Physical address SPA to be translated
  @param[out] *Skt       - socket number of the given SPA
  @param[out] *Ch        - channel number of the given SPA
  @param[out] *Dimm      - dimm corresponding to the given SPA
  @param[out] *Dpa       - Device physical address

  @return EFI_STATUS - status of the conversion effort

**/
typedef
EFI_STATUS
(EFIAPI *SPA_TO_NVM_DPA) (
  IN UINT64  Spa,
  OUT UINT8  *Skt,
  OUT UINT8  *Ch,
  OUT UINT8  *Dimm,
  OUT UINT64 *Dpa
  );

/**

  Routine Description: Update Nvm Tech ACPI tables

  @param *CrystalRidgeTablePtr  - pointer to CR ACPI tables

  @retval EFI_SUCCESS           - operation completed successfully
  @retval EFI_LOAD_ERROR        - error during ACPI table update

**/
typedef
EFI_STATUS
(EFIAPI *UPDATE_NVM_ACPI_TABLES) (
  IN OUT UINT64 *CrystalRidgeTablePtr
  );

/**

  Routine Description: Update Nvm Tech ACPI PCAT table

  @param CrystalRidgeTablePtr * - pointer to CR ACPI tables

  @return EFI_STATUS            - Status of operation

**/
typedef
EFI_STATUS
(EFIAPI *UPDATE_NVM_ACPI_PCAT_TABLE) (
  IN UINT64 *CrystalRidgeTablePtr
  );

/**

  Routine description: Verifies what kind of LongOp was running, processes results.
  This is CR protocol function.

  @param[in] Socket   Socket ID for given DIMM (0 based)
  @param[in] Channel  Channel ID (0 based, socket based)
  @param[in] Dimm     Slot ID

  @retval EFI_INVALID_PARAMETER NvDimm was not found
  @retval EFI_NOT_READY         ARS on Dimm still in progress
  @retval EFI_SUCCESS           State updated
  @retval other                 Other HW failure

**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_LONGOP_COMPLETE) (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm
  );

/**
  @brief Get the state of long operation for given DIMM.

  @param[in]  Skt    Socket ID for given DIMM (0 based)
  @param[in]  Ch     Channel ID (0 based, socket based)
  @param[in]  Dimm   DIMM slot ID
  @param[out] FisRsp LongOp Fis response
  @param[out] SeqNum Fis sequence number

  @return EFI_STATUS is returned.
**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_LONGOP_COMPLETE_GET) (
  IN     UINT8 Skt,
  IN     UINT8 Ch,
  IN     UINT8 Dimm,
  OUT    UINT32 *FisRsp,
  OUT    UINT32 *SeqNum
  );

/**
  @brief Process the state of long operation for given DIMM.

  @param[in] Skt    Socket ID for given DIMM (0 based)
  @param[in] Ch     Channel ID (0 based, socket based)
  @param[in] Dimm   DIMM slot ID
  @param[in] FisRsp LongOp Fis response
  @param[in] SeqNum Fis sequence number
**/

typedef
VOID
(EFIAPI *NVMCTLR_LONGOP_COMPLETE_PROCESS) (
  IN UINT8 Skt,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN UINT32 *FisRsp,
  IN UINT32 *SeqNum
  );

// Support for NvmController's config mem access

/**

  Routine Description: this function will read a NVMCTLR register via
  SADs interface.

  @param Socket      - Socket Number
  @param Ch          - DDR Channel ID
  @param Dimm        - DIMM number
  @param Reg         - NVMCTLR register address
  @param *Data       - pointer containing the data after reading the register

  @retval EFI_SUCCESS - operation completed successfully
  @retval EFI_INVALID_PARAMETER - invalid DIMM

**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_READ_CFG_MEM) (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT32   Reg,
  OUT UINT32   *Data
  );

/**

  Routine Description: this function will write 32 bits of data
  into a NVMCTLR register using SAD interface.

  @param Socket      - Socket Number
  @param Ch          - DDR Channel ID
  @param Dimm        - DIMM number
  @param Reg         - NVMCTLR register Address
  @param Data        - Data to be written to NVMCTLR register

  @retval EFI_SUCCESS - operation completed successfully
  @retval EFI_INVALID_PARAMETER - invalid DIMM

**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_WRITE_CFG_MEM) (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT32   Reg,
  IN UINT32   Data
  );

// Support for Error Logging

/**
  Routine Description: this function allows the user to setup host alerts
  This function sends NVMCTLR Command to acoomplish this.

  @param Socket       - Socket Number
  @param Ch           - DDR Channel ID
  @param Dimm         - DIMM Number
  @param Alerts       - New Host Alert Settings

  @return EFI_STATUS  - Status of the command sent

**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_SET_HOST_ALERT) (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT16    Alerts[MAX_HOST_TRANSACTIONS]
  );

/**
  Routine Description: this function allows the user to retrieve
  host alerts. This function accomplishes this by sending this
  command to the NVMCTLR Controller.

  @param Socket       - Socket Number
  @param Ch           - DDR Channel ID
  @param Dimm         - DIMM Number
  @param Alerts       - structure containing host alerts from NVMCTLR

  @return EFI_STATUS  - Status of the command sent
**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_GET_HOST_ALERT) (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  OUT UINT16   Alerts[MAX_HOST_TRANSACTIONS]
  );

/**

  Routine Description: this function will send
  Set Address Range Scrub command to NVMCTLR.
  CR protocol function.

  @param[in] Socket             - Socket Number
  @param[in] Ch                 - DDR Channel ID
  @param[in] Dimm               - DIMM number
  @param[in] DpaStartAddress    - Start address for scrubbing
  @param[in] DpaEndAddress      - End address for scrubbing
  @param[in] Enable             - Enable\Disable scrubbing

  @return EFI_STATUS            - Status of the command sent

**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_SET_ADDRESS_RANGE_SCRUB) (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT64    DpaStartAddress,
  IN UINT64    DpaEndAddress,
  IN BOOLEAN   Enable
  );

/**

  Routine Description: this function will send
  Get Address Range Scrub command to NVMCTLR.
  CR protocol function.

  @param[in]  Socket                 - Socket Number
  @param[in]  Ch                     - DDR Channel ID
  @param[in]  Dimm                   - DIMM number
  @param[out] *DpaStartAddress       - Start address for scrubbing
  @param[out] *DpaCurrentAddress     - Current address of scrub
  @param[out] *DpaEndAddress         - End address for scrubbing
  @param[out] *Enabled               - TRUE if scrub is enabled

  @return EFI_STATUS            - Status of the command sent

**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_GET_ADDRESS_RANGE_SCRUB) (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  OUT UINT64    *DpaStartAddress,
  OUT UINT64    *DpaEndAddress,
  OUT UINT64    *DpaCurrentAddress,
  OUT BOOLEAN   *Enabled
  );

/**

  Routine Description: this function gets latest error logs form NVMCTLR.

  @param Socket            - Socket Number
  @param Ch                - DDR Channel ID
  @param Dimm              - DIMM Number
  @param LogLevel          - Log Level 0 - low priority 1 - high priority
  @param LogType           - Log type
  @param Count             - Max number of log entries requested
  @param *ErrLog           - Pointer to error log data buffer

  @return EFI_STATUS       - Status of the Command Sent

**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_GET_LATEST_ERROR_LOG) (
  IN  UINT8    Socket,
  IN  UINT8    Ch,
  IN  UINT8    Dimm,
  IN  UINT8    LogLevel,
  IN  UINT8    LogType,
  IN  UINT8    Count,
  OUT ERR_LOG_DATA *ErrLog
  );

/**

  Retrieves specified error log starting at specified sequence number.

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
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_GET_ERROR_LOG) (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
  IN     UINT8        LogType,
  IN     UINT8        LogLevel,
  IN     BOOLEAN      CurrentBoot,
  IN OUT UINT16       *SequenceNumber,
     OUT ERR_LOG_DATA *ErrLog
  );

// Support for Error Injection

/**

  Routine Description: this function will enable
  injection within the part

  @param socket      - Socket Number
  @param ch          - DDR Channel ID
  @param dimm        - DIMM number

  @return EFI_STATUS - success if command sent

**/
typedef
EFI_STATUS
(EFIAPI  *NVMCTLR_ENABLE_ERR_INJECTION) (
  IN UINT8     socket,
  IN UINT8     ch,
  IN UINT8     dimm
  );

/**

  Routine Description: this function will disable
  injection within the part

  @param socket      - Socket Number
  @param ch          - DDR Channel ID
  @param dimm        - DIMM number

  @return EFI_STATUS - success if command sent

**/
typedef
EFI_STATUS
(EFIAPI  *NVMCTLR_DISABLE_ERR_INJECTION) (
  IN UINT8     socket,
  IN UINT8     ch,
  IN UINT8     dimm
  );

/**
  Enables error injection in all NVDIMMs in the platform

  @return EFI_STATUS - success if all NVDIMMs enabled error injection
**/
typedef
EFI_STATUS
(EFIAPI  *NVMCTLR_PLATFORM_ENABLE_ERR_INJECTION) (
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
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_INJECT_POISON_ERROR) (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT64    Dpa,
  IN  UINT8     MemoryType,
  IN  UINT8     Enable
  );

/**

  Routine Description: this routine sets the config data policy

  @param[in]  Socket  The socket
  @param[in]  Ch      The channel
  @param[in]  Dimm    The dimm number
  @param[in]  Data    The data to write

  @return Status

**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_SET_CFG_DATA_POLICY) (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT32    Data
  );

/**

  Routine Description: this routine gets the config data policy

  @param[in]    Socket  The socket
  @param[in]    Ch      The channel
  @param[in]    Dimm    The dimm number
  @param[out]   *Data   The data that is read

  @return Status

**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_GET_CFG_DATA_POLICY) (
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     Dimm,
  OUT   UINT32    *Data
  );

/**

  Routine Description: This function Sends Set Platform Config Data Area
  size to the NVMCTLR Controller. This function also copies the Input Buffer
  to the Dimm's Large Payload Input area. The mailbox used for communication
  with NVDIMM is defined by module location. In SMM it is SMM mailbox, in DXE
  it is OS mailbox used.

  @param[in] Socket           - Socket index at system level
  @param[in] Ch               - Channel index at socket level
  @param[in] Dimm             - DIMM slot index within DDR channel
  @param[in] InputBuffer      - Pointer to the Input Buffer.
  @param[in] InputSize        - Input length of the Config Data buffer
  @param[in] Offset           - PCD offset to which to write the Config data
  @param[in] PartitionID      - Partition ID to use for this command
  @param[in] UseLargePayload  - Boolean to tell whether to use
                                LargePayload or SmallPayload registers
                                for this PCD command.

  @return Status - Status of the operation

**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_SET_PCD) (
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     Dimm,
  IN    UINT8     *InputBuffer,
  IN    UINT32    InputSize,
  IN    UINT32    Offset,
  IN    UINT8     PartitionId,
  IN    BOOLEAN   UseLargePayload
  );

/**

  Routine Description: This function sends NVMCTLR Sets command access policy to FNV/EKV FW
  FIS 1.6 compatible.
  @param[in]  Socket    - Socket Number
  @param[in]  Ch        - DDR Channel ID
  @param[in]  Dimm      - DIMM Number
  @param[in]  Command   - The command to be modified (subopcode | opcode)
  @param[in]  SmmOnly   - Availability of the command. 0:BIOS mailbox only(SmmOnly), 1:No restriction

  @return Standard EFI_STATUS is returned.

**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_SET_COMMAND_ACCESS_POLICY) (
  IN UINT8  Socket,
  IN UINT8  Ch,
  IN UINT8  Dimm,
  IN UINT16 Command,
  IN UINT8  SmmOnly
  );

typedef
VOID
(EFIAPI *ACPI_CLEAR_LEAF_NOTIFICATIONS) (
  VOID
  );

typedef
VOID
(EFIAPI *ACPI_ADD_LEAF_NOTIFICATION) (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm
  );

typedef
VOID
(EFIAPI *ACPI_LEAF_NOTIFY) (
  VOID
  );

/**
  Clears NVDIMM root device notification status
**/
typedef
VOID
(EFIAPI *ACPI_CLEAR_ROOT_NOTIFICATION) (
  VOID
  );

/**
  Adds NVDIMM root device notification for specified DIMM and DPA

  @param[in]   Socket         Socket index
  @param[in]   Ch             Channel index
  @param[in]   Dimm           Dimm index
  @param[in]   Dpa            DPA
  @param[in]   RangeExponent  DPA range exponent (base 2)

**/
typedef
VOID
(EFIAPI *ACPI_ADD_ROOT_NOTIFICATION) (
  IN UINT8  Socket,
  IN UINT8  Ch,
  IN UINT8  Dimm,
  IN UINT64 Dpa,
  IN UINT8  RangeExponent
  );

/**
  Performs ACPI Notification of NVDIMM Root Device
**/
typedef
VOID
(EFIAPI *ACPI_ROOT_NOTIFY) (
  VOID
  );

/**
  Adds to Patrol Scrubber error list

  @param[in]   Socket         Socket index
  @param[in]   Ch             Channel index
  @param[in]   Dimm           Dimm index
  @param[in]   Dpa            DPA
  @param[in]   RangeExponent  DPA range exponent (base 2)

**/
typedef
VOID
(EFIAPI *ACPI_ADD_TO_ERROR_LIST) (
  IN UINT8  Socket,
  IN UINT8  Ch,
  IN UINT8  Dimm,
  IN UINT64 Dpa,
  IN UINT8  RangeExponent
  );

/**

  Routine Description: This function returns Platform Config Data by sending
  this command to the NVMCTLR Controller. The mailbox used for communication
  with NVDIMM is defined by module location. In SMM it is SMM mailbox, in DXE
  it is OS mailbox used.

  @param[in]  Socket          - Socket index at system level
  @param[in]  Ch              - Channel index at socket level
  @param[in]  Dimm            - DIMM slot index within DDR channel
  @param[out] OutputBuffer    - pointer to the buffer to which PCD needs to be
                                copied after successful command completion
  @param[in,out] OutputSize   - As input: size of the buffer, output:
                                size of the PCD read.
  @param[in]  Offset          - Offset from which the PCD needs to be read
  @param[in]  PartitionID     - ID of the partition to be read
  @param[in]  UseLargePayload - Boolean to tell whether to use
                                LargePayload or SmallPayload registers
                                for this PCD command.

  @return Status - Status of the operation

**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_GET_PCD) (
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN OUT  UINT8     *OutputBuffer,
  IN      UINT32    *OutputSize,
  IN      UINT32    Offset,
  IN      UINT8     PartitionId,
  IN      BOOLEAN   UseLargePayload
  );

/**
 * Platform Interpreted Last Shutdown Status
 * Takes in to account whether LSS was latched on previous shutdown, alters final LSS state based on system configuration.
 * See Intel Optane DC Persistent Memory Module Software RAS and Error Recovery
 * Specifiction for details. In particular:
 * Table 6-5 Power Down Signals and LSS Clean Shutdown Determination
 * Table 6-6 Power Down Signals and LSS Dirty Shutdown Determination
 **/
typedef enum {
  NvmLssClean = 0,       // Last platform adjusted shutdown was clean – LSS Reason enum is valid
  NvmLssDirty,           // Last platform adjusted shutdown was dirty – LSS Reason enum is valid
  NvmLssCleanNotLatched, // Current LSS is reporting a clean shutdown but the latch was not enabled – LSS Reason enum is valid
                         // The LSS state returned is reporting a clean shutdown for the first shutdown that occurred after the latch was enabled.
                         // You cannot imply ANYTHING about the last shutdown that just occurred.
  NvmLssDirtyNotLatched, // DIRTY_NOT_LATCHED – Current LSS is reporting a dirty shutdown but the latch was not enabled – LSS Reason enum is valid
                         // A previous system shutdown was detected by the Intel Optane DC Module as a dirty shutdown and the OS/sw/apps have not
                         // yet acknowledged or handled the dirty shutdown case.  Previous write data sent to the Intel Optane DCM… may or may not
                         // be lost and it is left to other SW components or applications to determine if their data is affected by the dirty shutdown.
                         // NOTE: that the returned enumeration values are valid in this case and represent the specific state of those bits at the time latch
                         // was enabled.  To be clear, LSS, DSC, Vendor Specific LSS Details, Vendor Specific LSS Extended Details update on shutdown after
                         // latch is enabled and remain frozen in previous state while latch is not enabled.
  NvmLssMax
} NVM_INTERPRETED_LSS;

typedef enum {
  //
  // Clean LSS reasons – matches row decode in Table 6-5
  //
  NvmLssReasonCleanAdrPwrFlow = 0,
  NvmLssReasonCleanS3PwrFlow,
  NvmLssReasonCleanS4PwrFlow,
  NvmLssReasonCleanS5PwrFlow,
  NvmLssReasonCleanIdlePwrFlow,
  NvmLssReasonCleanWarmResetPwrFlow,
  NvmLssReasonCleanIgnoredDueToCfg,
  NvmLssReasonCleanOther,            // Used if none of above clean shutdown reasons matched
  //
  // Dirty LSS reasons – matches row decode in Table 6-6
  //
  NvmLssReasonDirtyAdrStartedNoDdrtPwrFailCmdRecv,
  NvmLssReasonDirtyDdrtPwrFailCmdRecvAdrNotStarted,
  NvmLssReasonDirtySuprisePwrLossAtNvdimm,
  NvmLssReasonDirtyNvdimmThermalTrip,
  NvmLssReasonDirtyViralTriggeredBeforPwrDown,
  NvmLssReasonDirtyDdrtSupriseClkStop,
  NvmLssReasonDirtyWriteDataFlushNotCompleted,
  NvmLssReasonDirtyInjectedDirtyShutdown,
  NvmLssReasonDirtyNvdimmInternalCtxFlushFailed,
  NvmLssReasonDirtyPersistentCachingFailure,
  NvmLssReasonDirtyDimmFatal,        // The DCPMM has gone FATAL. The media error log or BIOS EWL may contain further details on why.
  NvmLssReasonDirtyDimmFatalFwHang,  // The DCPMM detected a DIMM Fatal, and the WTS FW watch dog interrupt fired. No additional Media Error Log information will be reported
  NvmLssReasonDirtyTransactionFatal, // Restarting should allow normal operations to continue. The media error log or BIOS EWL may contain further details on why.
  NvmLssReasonDirtyTempFatal,        // The DCPMM Controller detected a shutdown media temperature has been reached. No additional Media Error Log information will be reported

  NvmLssReasonDirtyOther,            // Used if none of above dirty shutdown reasons matched
  NvmLssReasonMax
} NVM_INTERPRETED_LSS_REASON;

/**
  @brief Provide platform interpreted LSS reason enum.

  @param[in]  Skt                     - Socket index at system level
  @param[in]  Ch                      - Channel index at socket level
  @param[in]  Dimm                    - DIMM slot index within DDR channel
  @param[out] InterpretedLssPtr       - Buffer for Last Shutdown State.
  @param[out] InterpretedLssReasonPtr - Buffer for Last Shutdown State Reason.

  @return Standard EFI_STATUS is returned.
**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_GET_PLATFORM_INTERPRETED_LSS) (
  IN  UINT8       Skt,
  IN  UINT8       Ch,
  IN  UINT8       Dimm,
  OUT NVM_INTERPRETED_LSS        *InterpretedLssPtr,
  OUT NVM_INTERPRETED_LSS_REASON *InterpretedLssReasonPtr
  );

/**
  @brief Provide platform interpreted LSS reason enum.

  @param[in]  Skt                     - Socket index at system level
  @param[in]  Ch                      - Channel index at socket level
  @param[in]  Dimm                    - DIMM slot index within DDR channel
  @param[out] InterpretedLssPtr       - Buffer for .
  @param[out] InterpretedLssReasonPtr - Buffer for .

  @return Standard EFI_STATUS is returned.
**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_GET_PLATFORM_INTERPRETED_UNLATCHED_DSC) (
  IN  UINT8       Skt,
  IN  UINT8       Ch,
  IN  UINT8       Dimm,
  OUT NVM_INTERPRETED_LSS        *InterpretedLssPtr,
  OUT NVM_INTERPRETED_LSS_REASON *InterpretedLssReasonPtr
  );

/**

  Routine Description: This function returns Error Log Data by sending
  this command to the NVMCTLR Controller.

  @param[in]  Socket          - Socket index at system level
  @param[in]  Ch              - Channel index at socket level
  @param[in]  Dimm            - DIMM slot index within DDR channel
  @param[in]  LogLevel        - Log levels Low & High Priority logs
  @param[in]  LogType         - Log Type Media & Thermal logs
  @param[in]  Count           - Number of log entries to read
  @param[out] ErrLogPtr       - pointer to the buffer to which Error log entries
                                copied after successful command completion

  @return Status - Status of the operation

**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_GET_ERROR_LOG_SMB) (
  IN  UINT8         Socket,
  IN  UINT8         Ch,
  IN  UINT8         Dimm,
  IN  UINT8         LogLevel,
  IN  UINT8         LogType,
  IN  UINT8         Count,
  OUT ERR_LOG_DATA *ErrLogPtr
  );

/**

  Routine Description: This function Enable/Disable Polling after sending
  this command to the NVMCTLR Controller.

  @param[in]  Socket          - Socket index at system level
  @param[in]  Imc              - Imc index at socket level
  @param[in]  PollState      - Enable/Disable polling
  @param[out] SaveState      - Preserve polling state if provided.

  @return Status - Status of the operation

**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_ENABLE_DISABLE_POLLING) (
 IN  UINT8          Socket,
 IN  UINT8          Imc,
 IN  UINT8          PollState,
 OUT UINT8 * CONST  SaveState
 );

/**

  Routine Description: This standalone routine sets the viral policy

  @param[in]  Socket  The socket
  @param[in]  Ch      The channel
  @param[in]  Dimm    The dimm number
  @param[in]  Data    The data to write

  @return Status

**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_SET_VIRAL_POLICY) (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT32    Data
  );

/**

  Routine Description: This standalone routine gets the viral policy

  @param[in]    Socket  The socket
  @param[in]    Ch      The channel
  @param[in]    Dimm    The dimm number
  @param[out]   *Data   The data that is read

  @return Status

**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_GET_VIRAL_POLICY) (
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     Dimm,
  OUT   UINT32    *Data
  );

/**

  Routine Description: This routine returns device specific information and various
  device characteristics

  @param[in]    Socket      The socket
  @param[in]    Ch          The channel
  @param[in]    Dimm        The dimm number
  @param[out]   *DimmInfoPtr  The device information structure

  @return Status

**/
typedef
EFI_STATUS
(EFIAPI *NVMCTLR_GET_DIMM_INFO) (
  IN    UINT8       Socket,
  IN    UINT8       Ch,
  IN    UINT8       Dimm,
  OUT   NVDIMM_INFO *DimmInfoPtr
  );

/**
  Notify Crystal Ridge driver of Host Alert Configuration

  @param[in]  HighPrioritySmi  TRUE: High Priority Alert SMI enabled
  @param[in]  LowPrioritySmi   TRUE: Low Priority Alert SMI enabled

**/
typedef
VOID
(EFIAPI *NOTIFY_HOST_ALERT_CONFIGURATION) (
  IN BOOLEAN HighPrioritySmi,
  IN BOOLEAN LowPrioritySmi
  );

/**

  Updates Extended ADR (eADR/fADR) LSS state in the PMEM Modules

  @param[in]  LssState  - Extended ADR LSS state

  @return Standard EFI_STATUS is returned

**/
typedef
EFI_STATUS
(EFIAPI *EXT_ADR_LSS_STATE_UPDATE) (
  IN UINT32 LssState
  );

typedef enum _EXT_ADR_STATE {
  EXT_ADR_DISABLED,
  EXT_ADR_ENABLED
} EXT_ADR_STATE;

typedef enum _EXT_ADR_TYPE {
  EXT_ADR_ENHANCED_ADR,
  EXT_ADR_FAST_ADR
} EXT_ADR_TYPE;

/**

  Enables/Disables Extended ADR (eADR/fADR) on PMEM Modules

  @param[in]  Type  - Extended ADR type (eADR/fADR)
  @param[in]  State - Extended ADR state (enabled/disabled)

  @return Standard EFI_STATUS is returned

**/
typedef
EFI_STATUS
(EFIAPI *EXT_ADR_STATE_UPDATE) (
  IN EXT_ADR_TYPE   Type,
  IN EXT_ADR_STATE  State
  );

/**

  Issues WPQ Flush for all iMCs

  @param None

  @return Standard EFI_STATUS is returned

**/
typedef
EFI_STATUS
(EFIAPI *IMC_WPQ_FLUSH) (
  VOID
  );

/**

  Updates information about Extended ADR (eADR/fADR) enabled flows

  @param[in] Flows - Extended ADR type (eADR/fADR) map of enabled flows

  @return Standard EFI_STATUS is returned

**/
typedef
EFI_STATUS
(EFIAPI *EXT_ADR_FLOWS_UPDATE) (
  IN UINT8 Flows
  );

typedef struct _EFI_CRYSTAL_RIDGE_PROTOCOL {
  UINT16                                 NvmProtocolVersion;
  UINT8                                  Reserved[6];
  IS_NVM_DIMM                            IsNvmDimm;
  IS_SYSTEM_WITH_DCPMM                   IsSystemWithDcpmm;
  GET_NUM_NVM_DIMMS                      GetNumNvmDimms;
  IS_NVM_ADDRESS                         IsNvmAddress;
  SPA_TO_NVM_DPA                         SpaToNvmDpa;
  UPDATE_NVM_ACPI_TABLES                 UpdateNvmAcpiTables;
  UPDATE_NVM_ACPI_PCAT_TABLE             UpdateNvmAcpiPcatTable;
  NVMCTLR_LONGOP_COMPLETE                NvmCtlrLongOpComplete;
  NVMCTLR_READ_CFG_MEM                   NvmCtlrReadCfgMem;
  NVMCTLR_WRITE_CFG_MEM                  NvmCtlrWriteCfgMem;
  NVMCTLR_SET_HOST_ALERT                 NvmCtlrSetHostAlert;
  NVMCTLR_GET_HOST_ALERT                 NvmCtlrGetHostAlert;
  NVMCTLR_SET_ADDRESS_RANGE_SCRUB        NvmCtlrSetAddressRangeScrub;
  NVMCTLR_GET_ADDRESS_RANGE_SCRUB        NvmCtlrGetAddressRangeScrub;
  NVMCTLR_GET_LATEST_ERROR_LOG           NvmCtlrGetLatestErrorLog;
  NVMCTLR_GET_ERROR_LOG                  NvmCtlrGetErrorLog;
  NVMCTLR_ENABLE_ERR_INJECTION           NvmCtlrEnableErrInjection;
  NVMCTLR_DISABLE_ERR_INJECTION          NvmCtlrDisableErrInjection;
  NVMCTLR_PLATFORM_ENABLE_ERR_INJECTION  NvmCtlrPlatformEnableErrInjection;
  NVMCTLR_INJECT_POISON_ERROR            NvmCtlrInjectPoisonError;
  NVMCTLR_SET_CFG_DATA_POLICY            NvmCtlrSetCfgDataPolicy;
  NVMCTLR_GET_CFG_DATA_POLICY            NvmCtlrGetCfgDataPolicy;
  NVMCTLR_SET_PCD                        NvmCtlrSetPcd;
  NVMCTLR_GET_PCD                        NvmCtlrGetPcd;
  NVMCTLR_SET_COMMAND_ACCESS_POLICY      NvmCtlrSetCommandAccessPolicy;
  ACPI_CLEAR_LEAF_NOTIFICATIONS          AcpiClearLeafNotifications;
  ACPI_ADD_LEAF_NOTIFICATION             AcpiAddLeafNotification;
  ACPI_LEAF_NOTIFY                       AcpiLeafNotify;
  ACPI_CLEAR_ROOT_NOTIFICATION           AcpiClearRootNotification;
  ACPI_ADD_ROOT_NOTIFICATION             AcpiAddRootNotification;
  ACPI_ROOT_NOTIFY                       AcpiRootNotify;
  NVMCTLR_GET_ERROR_LOG_SMB              NvmCtlrGetErrorLogSmb;
  NVMCTLR_ENABLE_DISABLE_POLLING         NvmCtlrEnableDisablePolling;
  NVMCTLR_SET_VIRAL_POLICY               NvmCtlrSetViralPolicy;
  NVMCTLR_GET_VIRAL_POLICY               NvmCtlrGetViralPolicy;
  NVMCTLR_GET_DIMM_INFO                  NvmCtlrGetDimmInfo;
  //
  // Functions added in EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_3 start here.
  //
  NVMCTLR_GET_PLATFORM_INTERPRETED_LSS           NvmCtlrGetPlatformInterpretedLss;
  NVMCTLR_GET_PLATFORM_INTERPRETED_UNLATCHED_DSC NvmCtlrGetPlatformInterpretedUnlatchedDsc;
  //
  // Functions added in EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_4 start here
  //
  ACPI_ADD_TO_ERROR_LIST                 AcpiAddToErrorList;
  //
  // Functions added in EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_5 start here
  //
  NOTIFY_HOST_ALERT_CONFIGURATION        NotifyHostAlertConfiguration;
  //
  // Functions added in EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_6 start here
  //
  NVMCTLR_LONGOP_COMPLETE_GET            NvmCtlrLongOpCompleteGet;
  NVMCTLR_LONGOP_COMPLETE_PROCESS        NvmCtlrLongOpCompleteProcess;
  //
  // Functions added in EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_7 start here
  //
  //
  // Functions added in EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_8 start here
  //
  EXT_ADR_LSS_STATE_UPDATE               ExtAdrLssStateUpdate;
  EXT_ADR_STATE_UPDATE                   ExtAdrStateUpdate;
  //
  // Functions added in EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_9 start here
  //
  IMC_WPQ_FLUSH                          ImcWpqFlush;

  //
  // Functions added in EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_10 start here
  //
  EXT_ADR_FLOWS_UPDATE                   ExtAdrFlowsUpdate;

} EFI_CRYSTAL_RIDGE_PROTOCOL;

extern EFI_GUID         gEfiCrystalRidgeGuid;
extern EFI_GUID         gEfiCrystalRidgeSmmGuid;

#endif // _CRYSTAL_RIDGE_PROTOCOL_H_

