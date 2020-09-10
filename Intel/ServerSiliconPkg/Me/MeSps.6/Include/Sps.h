/** @file
  SPS definitions common for various SPS modules.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2020 Intel Corporation. <BR>

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

#ifndef _SPS_H_
#define _SPS_H_

#include <Register/HeciRegs.h>
#include <MeiBusMsg.h>
#include <CoreBiosMsg.h>
#include <IccMsgs.h>
#include <Library/MeFiaMuxLib.h>

#ifndef MSR_PLATFORM_INFO
#define MSR_PLATFORM_INFO  0x000000CE
#endif

#define ME_FUNCTIONAL               0x1
#define ME_NONFUNCTIONAL            0x2

//
// e.g. HMRFPO message enable requires SPS to reinitialize
// for this larger timeout is required to wait SPS to wake up
//
#define SPS_INITIALIZATION_TIMEOUT  25000000   ///< 25sec timeout in microseconds

#define SPS_MAX_NUMBER_OF_RESEND_MESSAGE 3


/*****************************************************************************
 * SPS  HECI registers addresses
 */
#define SPS_REG_NMFS            R_ME_HFS   // R_ME_HFS on HECI-2

//
// SPS ME Firmware Status #2 Register
// This register is located in HECI-1 PCI config space at offset 48h.
//
typedef union
{
  UINT32   UInt32;
  struct
  {
    UINT32 NftpLoadFailure        : 1,  // 0     NFTP Load Failure Detection Field
           IccProgStatus          : 2,  // 1:2   ICC Programming Status
           RestrictedMode         : 1,  // 3     Intel ME FW entered the Restricted Mode of operation.
           CpuReplacedSts         : 1,  // 4     CPU Replaced STS
           RealFpf                : 1,  // 5     Set is CSME uses real FPFs
           MfsFailure             : 1,  // 6     ME detected error in its configuration
           WarmResetRequest       : 1,  // 7     ME informs that Host warm reset is necessary
           RecoveryCause          : 3,  // 8:10  If R_ME_HFS.r.CurrentState says recovery here is reason
           EopStatus              : 1,  // 11    ME notion of EOP status
           MeTargetImageBootFault : 1,  // 12    ME could not boot active image, backup is running
           FirmwareHeartbeat      : 3,  // 13:15 ME heartbeat, changes each second
           ExtendedStatusData     : 8,  // 16:23
           PmEvent                : 4,  // 24:27
           PhaseCode              : 4;  // 38:31
  } Bits;
} SPS_MEFS2;

//
// SPS_MEFS2::RecoveryCause Values
// If R_ME_HFS::CurrentState bits indicate that ME firmware is running in recovery mode
// these values provide the cause of this mode.
//
#define MEFS2_RCAUSE_RCVJMPR  0  // ME recovery jumper asserted
#define MEFS2_RCAUSE_MFGJMPR  1  // Security strap override jumper asserted (aka mfg jumper)
#define MEFS2_RCAUSE_IPMICMD  2  // Recovery forced with IPMI command
#define MEFS2_RCAUSE_FLASHCFG 3  // Invalid flash configuration (see ME-BIOS spec. for details)
#define MEFS2_RCAUSE_MEERROR  4  // ME internal error, check SEL
#define MEFS2_RCAUSE_UMAERROR 5  // UMA memory configuration error returned by HECI DID message

//
// SPS_MEFS2::PmEvent Values
// Power management transition status.
#define MEFS2_PMEVENT_CMOFF_CMX                0x00 // Clean CMoff->CMx wake
#define MEFS2_PMEVENT_CMOFF_CMX_AFTER_ERROR    0x01 // CMoff->CMx wake after an error
#define MEFS2_PMEVENT_CLEAN_GLOBAL_RESET       0x02 // Clean global reset
#define MEFS2_PMEVENT_GLOBAL_RESET_AFTER_ERROR 0x03 // Global reset after an error
#define MEFS2_PMEVENT_CLEAN_ME_RESET           0x04 // Clean ME reset
#define MEFS2_PMEVENT_RESET_DUE_EXCEPTION      0x05 // ME reset due to exception
#define MEFS2_PMEVENT_PSEUDO_GLOBAL_RESET      0x06 // Pseudo-global reset
#define MEFS2_PMEVENT_S0_SX                    0x07 // S0/CM0->Sx/CM3
#define MEFS2_PMEVENT_SX_S0                    0x08 // Sx/CM3->S0/CM0
#define MEFS2_PMEVENT_NON_POWER_CYCLE_RESET    0x09 // Non-power cycle reset
#define MEFS2_PMEVENT_POWER_CYCLE_RESET_CM3    0x0A // Power cycle reset through CM3
#define MEFS2_PMEVENT_POWER_CYCLE_RESET_CMOFF  0x0B // Power cycle reset through CMoff
#define MEFS2_PMEVENT_SX_SX                    0x0C // Sx/CMx->Sx/CMoff
#define MEFS2_PMEVENT_CM0_CM0_PG               0x0D // CM0->CM0_PG
#define MEFS2_PMEVENT_CM3_CM3_PG               0x0E // CM3->CM3_PG
#define MEFS2_PMEVENT_CM0_PG_CM0               0x0F // CM0_PG->CM0

//
// SPS NM Firmware Status Register.
// This register is located in HECI-2 PCI config space at offset 40h.
//
typedef union
{
  UINT32   UInt32;
  struct
  {
    UINT32 PerfOptBoot       : 1,  // 0     Power vs performance optimized boot mode
           Cores2Disable     : 7,  // 1:7   Number of processor cores to disable in each processor
           PwrLimiting       : 1,  // 8     Set to 1 if ME is actively limiting power consumption
           SmartClstUVolt    : 1,  // 9     Set to 1 if under-voltage was seen
           SmartClstOCurr    : 1,  // 10    Set to 1 if over-current was seen
           SmartClstOTemp    : 1,  // 11    Set to 1 if over-temperature was seen
           DirectPtuExecution: 1,  // 12    Set to 1 if PTU execution is requested
           Reserved1         :18,  // 13:30
           NmEnabled         : 1;  // 31    Set to 1 if Node Manager is supported in ME
  } Bits;
} SPS_NMFS;

/*****************************************************************************
 * SPS interface using HECI messaging
 */
#define SPS_CLIENTID_BIOS 0x00


/*****************************************************************************
 * ME Kernel Host Interface
 *****************************************************************************/
#define SPS_CLIENTID_ME_MKHI 0x07
//
// MKHI protocol supports several groups and several commands in each group
//
#define MKHI_GRP_GEN    0xFF  // Generic group
#define MKHI_GRP_HMRFPO 0x05  // Host ME Region Flash Protection Override
#define MKHI_GRP_NM     0x11  // Node Manager extension to MKHI
#define MKHI_GRP_DFUSE  0x13  // Dynamic Fusing

/******************************************************************************
 * ME Address of Fixed Address MEI Header -
 * This is the logical address of the Intel(R) ME client
 * of the message. This address is assigned during ME firmware initialization.
 */
#define HECI_ASF_MESSAGE_ADDR         0x02
#define HECI_FSC_MESSAGE_ADDR         0x03
#define HECI_POLICY_MANAGER_ADDR      0x05
#define HECI_ME_PASSWORD_SERVICE_ADDR 0x06
#define HECI_MKHI_MESSAGE_ADDR        0x07
#define HECI_ICC_MESSAGE_ADDR         0x08
#define HECI_TR_MESSAGE_ADDR          0x09
#define HECI_SPI_MESSAGE_ADDR         0x0A

/******************************************************************************
 * Generic MKHI messages group
 */
#define MKHI_CMD_GET_MKHI_VERSION   0x01
#define MKHI_CMD_GET_FW_VERSION     0x02
#define MKHI_CMD_END_OF_POST        0x0C
#define MKHI_CMD_HECI_STATE_CHANGE  0x0D

//
// MKHI Get Version message structure
// This request provides MKHI protocol definition version.
//
#pragma pack(1)
typedef struct
{
  MKHI_MESSAGE_HEADER  Mkhi;
} MKHI_MSG_GET_MKHI_VERSION_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MESSAGE_HEADER  Mkhi;
  UINT16               Minor;
  UINT16               Major;
} MKHI_MSG_GET_MKHI_VERSION_RSP;
#pragma pack()


//
// MKHI Get Firmware Version message structure
// Backup firmware version is optional, response may not contain this field
// if ME does not implement dual-image configuration.
//
//
// ME firmware version numbers structure
//
typedef struct
{
  UINT16  Minor;
  UINT16  Major;
  UINT16  Build;
  UINT16  Patch;
} MKHI_FW_VERSION;

#pragma pack(1)
typedef struct
{
  MKHI_MESSAGE_HEADER  Mkhi;
} MKHI_MSG_GET_FW_VERSION_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MESSAGE_HEADER  Mkhi;
  MKHI_FW_VERSION      Act;  // Active operational firmware
  MKHI_FW_VERSION      Rcv;  // Recovery firmware
  MKHI_FW_VERSION      Bkp;  // Backup operational firmware (optional)
} MKHI_MSG_GET_FW_VERSION_RSP;
#pragma pack()

//
// MKHI End Of POST message structure
//
#pragma pack(1)
typedef struct
{
  MKHI_MESSAGE_HEADER  Mkhi;
} MKHI_MSG_END_OF_POST_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MESSAGE_HEADER  Mkhi;
  UINT32               Action;
} MKHI_MSG_END_OF_POST_RSP;
#pragma pack()


//
// MKHI HECI State Change message structure
//
#pragma pack(1)
typedef struct
{
  MKHI_MESSAGE_HEADER  Mkhi;
  UINT64               Nonce;
  UINT8                HeciId;  // HECI ID 1,2,3 - this is not function number
  UINT8                State;   // 1 for hide, or 0 for disable
  UINT16               Reserved;
} MKHI_MSG_HECI_STATE_CHANGE_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MESSAGE_HEADER  Mkhi;
  UINT32               Response;
} MKHI_MSG_HECI_STATE_CHANGE_RSP;
#pragma pack()


/******************************************************************************
 * Host ME Region Flash Protection Override MKHI messages group
 */
#define HMRFPO_CMD_MERESET           0x00
#define HMRFPO_CMD_ENABLE            0x01
#define HMRFPO_CMD_LOCK              0x02
#define HMRFPO_CMD_GETSTATUS         0x03
#define HMRFPO_CMD_LOCK_IMMEDIATELY  0x04

#pragma pack(1)
typedef struct
{
  MKHI_MESSAGE_HEADER Mkhi;
  UINT64              Nonce;
  UINT32              RegionSelect;
} MKHI_MSG_HMRFPO_ENABLE_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MESSAGE_HEADER Mkhi;
  UINT32              FactoryBase;
  UINT32              FactoryLimit;
  UINT8               Status;
  UINT8               Reserved[3];
} MKHI_MSG_HMRFPO_ENABLE_RSP;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MESSAGE_HEADER Mkhi;
  UINT32              RegionSelect;
} MKHI_MSG_HMRFPO_LOCK_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MESSAGE_HEADER Mkhi;
  UINT64              Nonce;
  UINT32              FactoryBase;
  UINT32              FactoryLimit;
  UINT8               Status;
  UINT8               Reserved[3];
} MKHI_MSG_HMRFPO_LOCK_RSP;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MESSAGE_HEADER Mkhi;
  UINT32              RegionSelect;
} MKHI_MSG_HMRFPO_GETSTATUS_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MESSAGE_HEADER Mkhi;
  UINT8               Status;
  UINT8               Reserved[3];
} MKHI_MSG_HMRFPO_GETSTATUS_RSP;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MESSAGE_HEADER Mkhi;
  UINT64              Nonce;
} MKHI_MSG_HMRFPO_MERESET_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{
  MKHI_MESSAGE_HEADER Mkhi;
  UINT8               Status;
  UINT8               Reserved[3];
} MKHI_MSG_HMRFPO_MERESET_RSP;
#pragma pack()


/******************************************************************************
 * SPS Node Manager messages
 */
#define NM_CMD_HOSTCFG            0x00
#define MAX_ACPI_PSTATES_LO_LIMIT 16
#define MAX_ACPI_PSTATES          256

#define MAX_ACPI_NPSS_PSTATES     16
#define MAX_ACPI_GPSS_PSTATES     255

#pragma pack(1)
typedef struct
{
  MKHI_MESSAGE_HEADER Mkhi;
  union
  {
    UINT16            Uint16;
    struct
    {
      UINT16          TurboEn   : 1,
                      SmiOptim  : 1,
                      PowerMsmt : 1,
                      HwChange  : 1,
                      Resrerve0 :10,
                      MsgVer    : 2;
    } Bits;
  } Capabilities;
  UINT8               PStatesNumber;
  UINT8               TStatesNumber;
  UINT16              MaxPower;
  UINT16              MinPower;
  UINT16              ProcNumber;
  UINT16              ProcCoresNumber;
  UINT16              ProcCoresEnabled;
  UINT16              ProcThreadsEnabled;
  UINT64              PlatformInfo;
  INT16               Altitude;
  UINT8               NPSSPStatesRatio[MAX_ACPI_NPSS_PSTATES];
  UINT8               GPSSPstatesNumber;
  UINT8               GPSSPStatesRatio[MAX_ACPI_GPSS_PSTATES];
} MKHI_MSG_NM_HOST_CFG_REQ;

#define MKHI_MSG_NM_HOST_CFG_VER                    2

#define STATUS_NM_HOST_CFG_RSP_OK                   0
#define STATUS_NM_HOST_CFG_RSP_WRONG_MSG_FORMAT     1
#define STATUS_NM_HOST_CFG_RSP_ALTITUDE_MISSING     2
#define STATUS_NM_HOST_CFG_RSP_PSTATESRATIO_MISSING 3

typedef struct
{
  MKHI_MESSAGE_HEADER Mkhi;
  UINT8               Status;
  UINT8               Reserved1;
  UINT16              Reserved2;
} MKHI_MSG_NM_HOST_CFG_RSP;

typedef union {
  MKHI_MSG_NM_HOST_CFG_REQ        NMHostCfgReq;
  MKHI_MSG_NM_HOST_CFG_RSP        NMHostCfgRsp;
} MKHI_MSG_NM_HOST_CFG_BUFFER;
#pragma pack()

/*****************************************************************************
 * SPS messages for ME-BIOS interface definition version
 *****************************************************************************/
#define SPS_CLIENTID_ME_SPS   0x20

#define SPS_CMD_GET_MEBIOS_INTERFACE_REQ 1
#define SPS_CMD_GET_MEBIOS_INTERFACE_RSP (0x80 | SPS_CMD_GET_MEBIOS_INTERFACE_REQ)

#pragma pack(1)
typedef struct {
  HBM_COMMAND       Command;
} SPS_MSG_GET_MEBIOS_INTERFACE_REQ;

typedef struct {
    HBM_COMMAND     Command;
    UINT8           VerMajor;
    UINT8           VerMinor;
    UINT32          FeatureSet;
    UINT32          FeatureSet2;
} SPS_MSG_GET_MEBIOS_INTERFACE_RSP;
#pragma pack()

//
// BIOS must verify the SPS ME-BIOS Interface Specification version to make
// sure BIOS and ME firmware will talk the same language.
// There can be different versions for recovery and operational ME firmware
// after update of operational firmware.
//
#define SPS_MEBIOS_OPR_VERSION_MIN ((0 << 8) | 0)
#define SPS_MEBIOS_OPR_VERSION_MAX ((0xFF << 8) | 0xFF)
#define SPS_MEBIOS_RCV_VERSION_MIN ((0 << 8) | 0)
#define SPS_MEBIOS_RCV_VERSION_MAX ((0xFF << 8) | 0xFF)
#define SpsMeBiosOprVersionVerify(Major, Minor)                      \
        ((((Major) << 8) | (Minor)) >= SPS_MEBIOS_OPR_VERSION_MIN && \
         (((Major) << 8) | (Minor)) <= SPS_MEBIOS_OPR_VERSION_MAX)
#define SpsMeBiosRcvVersionVerify(Major, Minor)                      \
        ((((Major) << 8) | (Minor)) >= SPS_MEBIOS_RCV_VERSION_MIN && \
         (((Major) << 8) | (Minor)) <= SPS_MEBIOS_RCV_VERSION_MAX)

// SPS messages for PMBus over HECI communication

#define SPS_CMD_PMBUS_CMD_SEND_RAW_REQ        0xA
#define SPS_CMD_PMBUS_CMD_SEND_RAW_RSP        (0x80 | SPS_CMD_PMBUS_CMD_SEND_RAW_REQ)

#define SPS_CMD_PMBUS_CMD_REQ_PMBUS_DATA_SIZE 21
#define SPS_CMD_PMBUS_CMD_RSP_PMBUS_DATA_SIZE 26

#define PMBUS_CMD_STATUS_WORD 0x79

#define PMBUS_CMD_WRITE_LENGTH_READ_WORD  1
#define PMBUS_CMD_READ_LENGTH_READ_WORD   2

#define PMBUS_CMD_RESULT_SUCCESS                    0x00
#define PMBUS_CMD_RESULT_NO_DEVICE_TIMEOUT          0x80
#define PMBUS_CMD_RESULT_NOT_SERVICED               0x81
#define PMBUS_CMD_RESULT_ILLEGAL_SMBUS_ADDR_CMD     0x82
#define PMBUS_CMD_RESULT_ILLEGAL_SMBUS_ADDR_TARGET  0xA1
#define PMBUS_CMD_RESULT_PEC_ERROR                  0xA2
#define PMBUS_CMD_RESULT_UNSUPPORTED_WRITE_LENGTH   0xA5
#define PMBUS_CMD_RESULT_FRAME_LENGTH_NOT_SUPPORTED 0xA7
#define PMBUS_CMD_RESULT_MUX_COMMUNICATION_PROBLEM  0xA9
#define PMBUS_CMD_RESULT_SMBUS_TIMEOUT              0xAA

typedef enum {
  PMBUS_CMD_SEND_BYTE = 0,
  PMBUS_CMD_READ_BYTE,
  PMBUS_CMD_WRITE_BYTE,
  PMBUS_CMD_READ_WORD,
  PMBUS_CMD_WRITE_WORD,
  PMBUS_CMD_BLOCK_READ,
  PMBUS_CMD_BLOCK_WRITE,
  PMBUS_CMD_BLOCK_WRITE_READ_PROC_CALL
} PMBUS_CMD_SMBUS_TRANSACTION_TYPE;

#define PMBUS_STATUS_WORD_NO_ERRORS 0

#pragma pack(1)
typedef union {
  UINT16 Content;
  struct {
    UINT16 Unknown      :1,
           Other        :1,
           Fans         :1,
           PgStatus     :1,
           MfrSpecific  :1,
           Input        :1,
           IoutPout     :1,
           Vout         :1,
           NoneOfAbove  :1,
           Cml          :1,
           Temperature  :1,
           VinUvFault   :1,
           IoutOcFault  :1,
           VoutOvFault  :1,
           Off          :1,
           Busy         :1;
  } Bits;
} PMBUS_STATUS_WORD_MESSAGE_CONTENT;

typedef union {
  UINT8 Data;
  struct {
    UINT8 Reserved                      :1,
          SmbusMessageTransactionType   :3,
          DeviceAddressFormat           :2,
          DoNotReportPecInCc            :1,
          EnablePec                     :1;
  } Flags;
} PMBUS_CMD_FLAGS;

typedef union {
  UINT8 Data;
  struct {
    UINT8 Reserved      :1,
          SmbusAddress  :7;
  } Fields;
} PMBUS_CMD_PSU_ADDRESS;

typedef union {
  UINT8 Data;
  struct {
    UINT8 MuxMgpioIdx :6,
          Reserved    :2;
  } Fields;
} PMBUS_CMD_MUX_ADDRESS;

typedef struct {
  PMBUS_CMD_FLAGS       Flags;
  PMBUS_CMD_PSU_ADDRESS PsuAddress;
  PMBUS_CMD_MUX_ADDRESS MuxAddress;
  UINT8                 Reserved;
  UINT8                 WriteLength;
  UINT8                 ReadLength;
} PMBUS_CMD_REQUEST_PARAMETERS;

typedef struct {
  UINT8                 RequestData[SPS_CMD_PMBUS_CMD_REQ_PMBUS_DATA_SIZE];
} PMBUS_CMD_REQUEST_DATA;

typedef struct {
  UINT8                 Result;
} PMBUS_CMD_RESPONSE_PARAMETERS;

typedef struct {
  UINT8                 ResponseData[SPS_CMD_PMBUS_CMD_RSP_PMBUS_DATA_SIZE];
} PMBUS_CMD_RESPONSE_DATA;

typedef struct {
  HBM_COMMAND                     Command;
  PMBUS_CMD_REQUEST_PARAMETERS    PmbusParameters;
  PMBUS_CMD_REQUEST_DATA          PmbusCommandData;
} SPS_MSG_PMBUS_CMD_SEND_RAW_REQ;

typedef struct {
    HBM_COMMAND                   Command;
    PMBUS_CMD_RESPONSE_PARAMETERS PmbusParameters;
    PMBUS_CMD_RESPONSE_DATA       PmbusCommandData;
} SPS_MSG_PMBUS_CMD_SEND_RAW_RSP;
#pragma pack()

#define SpsMsgPmbusCmdSendRawGetReqLength(WriteLength) \
  (sizeof(SPS_MSG_PMBUS_CMD_SEND_RAW_REQ) - sizeof(PMBUS_CMD_REQUEST_DATA) + (sizeof(UINT8) * WriteLength))

#define SpsMsgPmbusCmdSendRawGetRspLength(ReadLength) \
  (sizeof(SPS_MSG_PMBUS_CMD_SEND_RAW_RSP) - sizeof(PMBUS_CMD_RESPONSE_DATA) + (sizeof(UINT8) * ReadLength))

#define SpsMsgPmbusCmdSendRawNoPmbusDataRspLength() \
  (sizeof(SPS_MSG_PMBUS_CMD_SEND_RAW_RSP) - sizeof(PMBUS_CMD_RESPONSE_DATA))

#define SpsMsgPmbusCmdSendRawRspLengthVerify(ResponseLengthReceived, ResponseLengthExpected) \
  ((ResponseLengthReceived == SpsMsgPmbusCmdSendRawNoPmbusDataRspLength()) || (ResponseLengthReceived == ResponseLengthExpected))

//
// Thermal throttling messages
//
#define SPS_CMD_SET_PCH_TEMP_REPORTING_CFG_REQ 0x0B
#define SPS_CMD_SET_PCH_TEMP_REPORTING_CFG_RSP (0x80 | SPS_CMD_SET_PCH_TEMP_REPORTING_CFG_REQ)

#pragma pack(1)
typedef struct
{
  UINT8           Command;
  UINT8           Reserved;
  UINT16          PchTempReportingInterval;
  UINT16          PchTempMaximumLowPowerInterval;
} HECI_MSG_SET_PCH_TEMP_REPORTING_CFG_REQ;
#pragma pack()

#pragma pack(1)
typedef struct
{
  UINT8           Command;
  UINT16          Reserved;
  UINT8           Result;
} HECI_MSG_SET_PCH_TEMP_REPORTING_CFG_RSP;
#pragma pack()

/*****************************************************************************
* MCTP Bus Owner Proxy Configuration
*****************************************************************************/
#define SPS_CLIENTID_ME_MCTP   0x21

#define MCPT_CMD_SET_BUS_OWNER_REQ   0x01
#define MCPT_CMD_SET_BUS_OWNER_RSP (0x80 | MCPT_CMD_SET_BUS_OWNER_REQ)
#define MCPT_CMD_SET_BUS_OWNER_RSP_SUCCESS (0x00)

#pragma pack(1)
typedef struct {
  UINT8           Command;
  UINT8           Reserved0[3];
  UINT16          PCIeAddress;
  UINT8           Location;
  UINT8           Reserved1;
} MCTP_SET_BUS_OWNER_REQ;

typedef struct {
  UINT8           Command;
  UINT8           Reserved0[2];
  UINT8           Result;
} MCTP_SET_BUS_OWNER_RSP;
#pragma pack()

/*****************************************************************************
* RAS Configuration
*****************************************************************************/
#define SPS_CLIENTID_ME_RAS                         0x22 // RAS Notification handler in ME

#define RAS_NOTIFY_REQ                              0x03
#define RAS_NOTIFY_RSP                              0x83

#pragma pack(1)
typedef struct {
  MKHI_MESSAGE_HEADER Heci;
  UINT8               Cmd;
  UINT64              Nonce;
  UINT8               NotificationType;
} HECI_RAS_NOTIFY_REQ;

typedef struct {
  MKHI_MESSAGE_HEADER Heci;
  UINT8               Cmd;
  UINT64              Nonce;
  UINT8               Status;
} HECI_RAS_NOTIFY_RSP;
#pragma pack()


/*****************************************************************************
 * SPS FIA CONFIG messages
 *****************************************************************************/
// Size of tables is changed upon send operation according to count fields
#define FIA_MUX_LENGTH        38
#pragma pack(1)
typedef struct {
  ICC_HEADER                  Header;
  UINT8                       FiaMuxCfgInvalidate;
  UINT8                       FiaMuxRecordsCount;
  UINT8                       SataRecordsCount;
  UINT8                       PcieRootPortRecordsCount;
  FIA_MUX_RECORD              FiaMux[FIA_MUX_LENGTH];
  SATA_RECORD                 Sata[FIA_MUX_LENGTH];
  PCIE_ROOT_PORT_RECORD       PcieRootPort[FIA_MUX_LENGTH];
} ICC_MSG_SET_FIA_MUX_CONFIG_REQ;

typedef struct {
  ICC_HEADER Header;
} ICC_MSG_SET_FIA_MUX_CONFIG_RSP;

typedef struct {
  ICC_HEADER Header;
} ICC_MSG_GET_FIA_MUX_CONFIG_REQ;

typedef struct {
  ICC_HEADER                  Header;
  UINT8                       LanesAllowed;
  UINT8                       FiaMuxRecordsCount;
  UINT8                       SataRecordsCount;
  UINT8                       PcieRootPortRecordsCount;
  FIA_MUX_RECORD              FiaMux[FIA_MUX_LENGTH];
  SATA_RECORD                 Sata[FIA_MUX_LENGTH];
  PCIE_ROOT_PORT_RECORD       PcieRootPort[FIA_MUX_LENGTH];
} ICC_MSG_GET_FIA_MUX_CONFIG_RSP;
#pragma pack()

/*****************************************************************************
 * SPS ME SHUTDOWN message
 *****************************************************************************/
#define SPS_CMD_ME_SHUTDOWN_REQ 3
#define SPS_CMD_ME_SHUTDOWN_RSP (0x80 | SPS_CMD_ME_SHUTDOWN_REQ)

#pragma pack(1)
typedef struct {
  HBM_COMMAND       Command;
} SPS_MSG_ME_SHUTDOWN_REQ;

typedef struct {
  HBM_COMMAND     Command;
  UINT8           Result;
} SPS_MSG_ME_SHUTDOWN_RSP;
#pragma pack()

#endif // _SPS_H_

