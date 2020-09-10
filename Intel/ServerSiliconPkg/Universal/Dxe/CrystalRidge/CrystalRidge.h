/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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
#ifndef _CRYSTALRIDGE_H_
#define _CRYSTALRIDGE_H_

#include <PiDxe.h>
#include <Protocol/CrystalRidge.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/MpService.h>
#include <Protocol/CpuCsrAccess.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/SmmSxDispatch2.h>
#include <Protocol/EadrProtocol.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
// HSD:4930336_BEGIN
#include <Library/TimerLib.h>
// HSD:4930336_END
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/LocalApicLib.h>
#include <Library/CpuConfigLib.h>
#include <Library/MemFmcIpLib.h>
#include <Guid/MemoryMapData.h>
#include <Guid/HobList.h>
#include <Protocol/Smbios.h>
#include <Protocol/NvdimmSmbusSmmProtocol.h>
#include <RcRegs.h>
#include "PlatformCfgData.h"
#include <Upi/KtiHost.h>
#include "Nfit.h"
#include "Fis.h"
#include <UncoreCommonIncludes.h>
#include <Library/OemCrystalRidgeHooks.h>
#include <Library/NvdimmCommonLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/PmemMiscLib.h>
#include "NvdimmAcpiSmmInterface.h"

extern EFI_GUID  gEfiVolatileMemModeVariableGuid;
extern EFI_GUID  gNfitBindingProtocolGuid;
extern EFI_GUID  gEfiRasHostGuid;

#ifndef NELEMENTS
#define NELEMENTS(Array) (sizeof(Array)/sizeof((Array)[0]))
#endif

#ifdef SMM
#include <Library/SynchronizationLib.h>
extern SPIN_LOCK mSmmDebugLock;
#define CRDEBUGX(...) do { \
            AcquireSpinLock (&mSmmDebugLock); \
            DEBUG (__VA_ARGS__); \
            ReleaseSpinLock (&mSmmDebugLock); \
            } while (0)
#define CRDEBUG CRDEBUGX
#else
#define CRDEBUG DEBUG
#endif

#define CR_INFO_STR(module)  "[PMem](" module ") "
#define CR_WARN_STR(module)  "[PMem](" module ") WARNING: "
#define CR_ERROR_STR(module) "[PMem](" module ") ERROR: "
#define CR_EXT_ADR_STATE_UEFI_VARIABLE_NAME L"Extended ADR State"
#define CR_EXT_ADR_STATE_UEFI_VARIABLE_GUID {0x45801f08, 0xb1b3, 0x4255, {0xbd, 0x51, 0x07, 0xf7, 0x89, 0x73, 0xe8, 0x65}}

typedef enum _CR_EXT_ADR_TYPE {
  CR_EXT_ADR_ENHANCED_ADR,
  CR_EXT_ADR_FAST_ADR
} CR_EXT_ADR_TYPE;

typedef struct _EXT_ADR_DATA {
  UINT8 ExtAdrEnabled  :1,
        ExtAdrType     :1,
        Reserved       :6;
} EXT_ADR_DATA;

#define CR_EXT_ADR_FLOWS_UEFI_VARIABLE_NAME L"Extended ADR Flow"
#define CR_EXT_ADR_FLOWS_UEFI_VARIABLE_GUID {0xFBA6E806, 0xE1C8, 0x4791, {0x8A, 0xB9, 0xE7, 0xE9, 0xF5, 0x86, 0x2C, 0x46}}

typedef union _EXT_ADR_FLOWS {
  struct {
    UINT8 AcLossIioAndCpuCacheFlush             :1,
          ColdAndWarmResetCpuCacheFlush         :1,
          S3EntryCpuCacheFlush                  :1,
          S4EntryCpuCacheFlush                  :1,
          S5EntryCpuCacheFlush                  :1,
          Reserved0                             :3;
  } FlowsBits;

  UINT8 Flows;
} EXT_ADR_FLOWS;

//
// Delay unit in us when waiting for NVDIMM response
//
#define COMMON_DELAY_UNIT  10

//
// DIMM encoding is as follows:
// [0,1] - DIMMs 0,1 in socket 0, channel 0
// [2,3] - DIMMs 0,1 in socket 0, channel 1
// ...
// [10,11] - DIMMs 0,1 in socket 0, channel 5
// [12..15] - spare for future channels 6,7 if used
// [16,17] - DIMMs 0,1 in socket 1, channel 0
//
#define DIMM_TO_BITMAP(_Socket, _Ch, _Dimm) ((UINT64)0x1 << ((((_Socket) & 0x3)* 16) + ((_Ch) * 2) + (_Dimm)))
#define SOCKET_CHANNEL_DIMM_TO_NFIT_HANDLE(_SOCKET, _CHANNEL, _DIMM) (((((((UINT32) (_SOCKET) << 4) | \
                                                                          ((UINT32) (_CHANNEL) / MAX_MC_CH)) << 4) | \
                                                                          ((UINT32) (_CHANNEL) % MAX_MC_CH)) << 4) | \
                                                                           (UINT32) (_DIMM))
// Large Payload Region size
#define CR_LARGE_PAYLOAD_SIZE      0x100000
// Offset from the start of the CTRL region to the start of the BIOS mailbox
#define CR_BIOS_MB_OFFSET          0x101000
// Offset to BIOS MB Large InPayload region
#define CR_BIOS_MB_LARGE_INPAYLOAD 0x400000
// Offset to BIOS MB Large OutPayload region
#define CR_BIOS_MB_LARGE_OUTPAYLOAD 0x500000
// Boot Status Reg Offset
#define CR_MB_BOOT_STATUS_OFFSET   0x20000
// OS Mailbox Region Offsets below:
#define CR_OS_MB_OFFSET            0x100000
// Offset to OS Mailbox Large InPayload Region
#define CR_OS_MB_LARGE_INPAYLOAD   0x200000
// Offset to OS Mailbox Large OutPayload Region
#define CR_OS_MB_LARGE_OUTPAYLOAD  0x300000
// OS Mailbox length
#define CR_OS_MB_LENGTH            0x1000
// Offset to SMBUS Mailbox
#define CR_SMBUS_MB_OFFSET         0x24000

#define CR_REG_SIZE               8          // Size of a NVMDIMM Mailbox Register Bytes
#define CR_NONCE_SIZE             4          // Size of a NVMDIMM Nonce Register Bytes
#define CR_IN_PAYLOAD_SIZE        128        // Total size of the input payload registers
#define CR_IN_SINGLE_PAYLOAD_SIZE 4          // Size of the input payload register
#define CR_IN_PAYLOAD_NUM         32         // Total number of the input payload registers
#define CR_OUT_PAYLOAD_SIZE       128        // Total size of the output payload registers
#define CR_OUT_SINGLE_PAYLOAD_SIZE 4         // Size of the output payload register
#define CR_OUT_PAYLOAD_NUM        32         // Total number of the output payload registers
#define CR_REG_ALIGNMENT_SIZE     64         // NVMDIMM Registers are to be 64-byte aligned

//
// Offsets for NVMDIMM control region
//
#define CR_MB_COMMAND_OFFSET      0
#define CR_MB_NONCE0_OFFSET       0x40
#define CR_MB_NONCE1_OFFSET       0x80
#define CR_MB_IN_PAYLOAD0_OFFSET  0xC0
#define CR_MB_STATUS_OFFSET       0x4C0
#define CR_MB_OUT_PAYLOAD0_OFFSET 0x500

#define CONTROL_REGION_TRIM_SIZE                  (1 * 1024 * 1024)
#define FLUSH_HINT_ADDRESS_OFFSET                 0x7FFFC00
#define CTRL_RGN_GRANULARITY                      256

#define USE_SMALL_PAYLOAD              0
#define USE_LARGE_PAYLOAD              1
#define ERROR_UNCORRECTABLE            0

#define LARGE_PAYLOAD_INPUT     1
#define LARGE_PAYLOAD_OUTPUT    0

#define BIOS_PARTITION          0x00
#define OS_PARTITION            0x01
#define OS_PARTITION_OTHER      0x02
#define FNV_SIZE                0x01
#define FNV_DATA                0x00
#define LARGE_PAYLOAD           0
#define SMALL_PAYLOAD           1
#define LARGE_PAYLOAD_MGMT      1
#define USE_BIOS_MAILBOX        FALSE
#define USE_OS_MAILBOX          TRUE

#define ADMIN_FUNCTIONS     0x02
#define BIOS_EMULATED       BIT0
#define NON_EXISTING_MEM_DEV 0x02
#define ONE_IMC_WAY         1
#define TWO_IMC_WAY         2
#define FOUR_IMC_WAY        4
#define EIGHT_IMC_WAY       8
#define SIXTEEN_IMC_WAY     16

#define SET_PLATFORM_CONFIG_DATA_LENGTH 64

#define UNCORRECTABLE_UNIT_SIZE_SHIFT 8
#define UNCORRECTABLE_UNIT_SIZE       (1 << (UNCORRECTABLE_UNIT_SIZE_SHIFT))

/*
 * The macros defined below establish operations for Control Region Block Decoder addresses.
 */
#define CR_CRBD_LINESIZE                              0x1000 // Fixed at 4KB
#define CR_CRBD_MAXSKT                                8
#define CR_CRBD_MAXIMC                                4
#define CR_CRBD_MAXCH                                 4
#define CR_CRBD_INTWAYS                               (CR_CRBD_MAXSKT * CR_CRBD_MAXCH * CR_CRBD_MAXIMC) // 128 way
#define CR_CRBD_DIMM_INDEX(Skt, Imc, Ch)              (((Skt) << 4) | (Imc) | ((Ch) << 2))
#define CR_CRBD_DIMM_OFFSET(Skt, Imc, Ch)             (((UINT64)(Skt) << 16) | ((UINT64)(Imc) << 12) | ((UINT64)(Ch) << 14))
#define CRBD_SPA_TO_SKT(Spa)                          ((UINT8)((Spa) >> 16) & 7)
#define CRBD_SPA_TO_IMC(Spa)                          ((UINT8)((Spa) >> 12) & 3)
#define CRBD_SPA_TO_CH(Spa)                           ((UINT8)((Spa) >> 14) & 3)
#define CR_CRBD_DPA_TO_SPA_OFFSET(Dpa, Skt, Imc, Ch)  (((Dpa) & ~0xFFF) << 7 | CR_CRBD_DIMM_INDEX(Skt, Imc , Ch) << 12 | ((Dpa) & 0xFFF))
#define CR_CRBD_MAX_ASL_REGION_LENGTH                 CR_CRBD_DPA_TO_SPA_OFFSET(CR_OS_MB_OFFSET, CR_CRBD_MAXSKT - 1, CR_CRBD_MAXIMC - 1, CR_CRBD_MAXCH - 1) + CR_OS_MB_LENGTH

#define CR_ACPI_SMM_INTERFACE_UEFI_VARIABLE_GUID  { 0x05C422E0, 0xAE15, 0x452E, {0xAD, 0x75, 0xF9, 0xA1, 0xF3, 0x00, 0x24, 0x46 } }
#define CR_ACPI_SMM_INTERFACE_UEFI_VARIABLE_NAME  L"CR ACPI SMM VARIABLE"


#define MAX_CONTROL_REGIONS            8

#define WBCACHED_PMEM_PRESENT  BIT0
#define PMEM_PRESENT           BIT1

#define PM_REGION_LOCKED_UPON_S3_RESUME        0x00  // Secure S3 enabled
#define PM_REGION_STATE_RESTORED_FROM_S3_ENTRY 0x01  // Secure S3 disabled
#define S3_RESUME_OPT_IN_CODE                  0x03

#define FW_ACTIVATE_OPTED_IN                   0x01
#define FW_ACTIVATE_NOT_OPTED_IN               0x00
#define FW_ACTIVATE_OPT_IN_CODE                0x04

#define SMM_MP_DEF_TIMEOUT_US                  1000000

#define CR_EADR_LSS_STATE_UPDATE_DATA_MASK     0x0000000F
#define DSM_INPUT_BUFFER_MAGIC                 0xBADDCAFE

//
// SMI handler for DSM CR actions
//
#define DSM_SW_SMI 0xFC
/*
 * Struct format extracted from XML file FNV\0.0.0.FNV.xml.
 * This register contains the Host Mailbox Command Register Bits definitions. This is a
 * 64-bit Register.
 */
#pragma pack(1)
typedef union {
  struct {
    UINT32 reserved : 32; // Bits[31:0] are reserved bits
    UINT32 opcode : 16;
    /* opcode - Bits[47:32], RW, default = 16'h0
       FW MB OPCODE
     */
    UINT32 doorbell : 1;
    /* doorbell - Bits[48:48], RW, default = 1'b0
       FW MB Doorbell
     */
    UINT32 spare : 14;
    /* spare - Bits[62:49], RW, default = 15'b0
       Spare for future use
     */
    UINT32 sequence_bit : 1;
    /* Sequence Bit*/
  } Bits;
  UINT64 Data;
} MB_SMM_CMD_FNV_SPA_MAPPED_0_STRUCT;
#pragma pack()

/*
 * Struct format extracted from XML file NVMCTLR\0.0.0.NVMCTLR.xml.
 * This register contains the FW Mailbox Status converted to 64-bit to be compatible with
 * 64-bit SPA addressing.
 */
#pragma pack(1)
typedef union {
  struct {
    UINT32 comp : 1;
    /* comp - Bits[0:0], RWV, default = 1'h1
       FW Mailbox Completion:[br]
                       0 = Not Complete[br]
                       1 = Complete[br]
                       HW will clear this bit when doorbell bit is set.
     */
    UINT32 nonce_fail : 1;
    /* fail - Bits[1:1], RW, default = 1'h0
       FW Mailbox Nonce Fail:[br]
                       0 = Success[br]
                       1 = Failure
     */
    UINT32 bos : 1;
    /*
    * Background Operation Status
    */
    UINT32 MbDelayed : 1;
    /* MbDelayed - Bits[3:3], RW, default = 1'h0
       MB Delayed:[br]
                       0 = Rate Limiting disabled[br]
                       1 = Mailbox is rate limited
     */
    UINT32 spare0 : 4;
    /* spare0 - Bits[7:4], RW, default = 4'h0
       Spare status bits reserved for future use.
     */
    UINT32 stat : 8;
    /* stat - Bits[15:8], RW, default = 8'h0
       FW Mailbox Status Code
     */
    UINT32 rsvd_16 : 16;
    UINT32 rsvd_32 : 32;
    /*
    Adding 32-bit of reserved field to be compatible with 64-bit
    SPA Addressing
    */
  } Bits;
  UINT64 Data;
} MB_SMM_STATUS_FNV_SPA_MAPPED_0_STRUCT;
#pragma pack()

//
// This struct is used by processing code to prevent redundant SADs
// being accounted for the computation of DCPMM Control Regions for CrystalRidge thru SAD interface.
typedef struct {
  UINT8        NumOfSADCntrlReg;
  UINT32       Limits[MAX_SOCKET * MAX_SAD_RULES];
} SAD_CONTROL_REGION, *PSAD_CONTROL_REGION;

//
// This struct is used by processing code to prevent redundant SADs
// being accounted for the computation of DCPMM PMEM Regions for CrystalRidge thru SAD interface.
typedef struct {
  UINT8        NumOfSADPMemReg;
  UINT32       Limits[MAX_SOCKET * MAX_SAD_RULES];
} SAD_PMEM_REGION, *PSAD_PMEM_REGION;

typedef struct {
  UINT64 Command;
  UINT64 Status;
  UINT32 InPayload[NUM_PAYLOAD_REG];
  UINT32 OutPayload[NUM_PAYLOAD_REG];
  UINT64 Bsr;
} CR_DUMMY_CONTROL_REGION;

typedef struct {
  UINT64 FlushReg[32];
} CR_DUMMY_FLUSH_REGION;

typedef struct {
  UINT64 Base;
  UINT64 Size;
} CONTROL_REGION_INFO;

typedef struct {
  UINT64 Base;
  UINT64 End;
} FLUSH_REGION_INFO;

typedef struct _CR_INFO
{
  NVDIMM_COMMON_INFO      NvdimmInfo;
  UINT64                  CrbdAddrBase;       // Base address of control region block decoder
  UINT64                  CrbdAddrLimit;      // A limit of control region block decoder addresses
  CONTROL_REGION_INFO     ControlRegion[MAX_CONTROL_REGIONS + 1];
  FLUSH_REGION_INFO       FlushRegion[MAX_CONTROL_REGIONS + 1];
  UINT8                   NumControlRegions;
  CR_DUMMY_CONTROL_REGION ControlRegionDummy;
  CR_DUMMY_FLUSH_REGION   FlushRegionDummy;
  UINT8                   DdrtAlertSmiEn;
  UINT16                  PlatformActivationStatus;
  EXT_ADR_DATA            ExtAdrData;
  EXT_ADR_FLOWS           ExtAdrFlows;
} CR_INFO;

typedef struct _ARS_OP_STATUS {
  BOOLEAN   InProgress;         // is operation in progress
  BOOLEAN   PrematurelyEnded;   // is operation ended prematurely
  BOOLEAN   Overflow;           // GetLongOpStatus buffer overflow
  UINT8     StatusCode;         // FW status code of operation
  UINT16    PercentComplete;    // percent complete
  UINT32    EstimatedTime;      // estimated time in seconds to finish the operation
  UINT8     ErrorsFound;        // how many erroneous addresses has been found
  UINT64    DpaErrorAddress[MAX_REPORTED_ERRORS_PER_DIMM]; // list of erroneous addresses found during the scrub
} ARS_OP_STATUS;

typedef struct _ARS_ERROR_RECORD {
  UINT32    NfitHandle;       // Nfit handle that defines the dimm that is part of the error record
  UINT32    Reserved;
  UINT64    SpaOfErrLoc;      // start SPA of the error location
  UINT64    Length;           // length of the error location region
} ARS_ERROR_RECORD;

/******************************************************************************
 * Module global variables.
 ******************************************************************************/
extern EFI_CPU_CSR_ACCESS_PROTOCOL  *mCpuCsrAccess;
extern EFI_SMM_SYSTEM_TABLE2        *mSmst;
extern struct SystemMemoryMapHob    *mSystemMemoryMap;
extern CR_INFO                      mCrInfo;
extern NVDIMM_ACPI_SMM_INTERFACE    *mNvdimmAcpiSmmInterface;
extern NVDIMM_SMBUS_SMM_INTERFACE   *mNvdimmSmbusSmmInterface;
extern BOOLEAN                      mUseWpqFlush;
extern BOOLEAN                      mUseOSMailbox;
extern UINT8                        *mLargePayloadBuffer;
extern BOOLEAN                      mInSmm;
extern EFI_EADR_PROTOCOL            *mEadrProtocol;
extern MEMORY_MAP_HOST              *mMemMapHost;
extern UINT8                        *mNvdimmAcpiSmmInterfaceInputBuffer;


/******************************************************************************
 * Function prototypes
 ******************************************************************************/

EFI_STATUS
CrystalRidgeProtocolInstall (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  );

EFI_STATUS
DcpmmProtocolInstall (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  );

/**
  @brief Return position of next bit set in a bitmap.

  @param[in] Bitmap   Bitmap of to look for bit
  @param[in] StartAt  Bit position to start at

  @return The position of bit found in Bitmap, or FFh if not found.
**/
UINT8
BitmapGetBit (
  IN UINT32 Bitmap,
  IN UINT8  StartAt
  );

EFI_STATUS
InjectErrorRange (
  IN  UINT64  StartAddress,
  IN  UINT64  NumBlks,
  IN  UINT8   Enable,
  IN  UINT8   MemType,
  OUT UINT64  *InjectedBlks
  );

/**

  Routine Description: Performs a durable Cache Line Write

  @param CsrAddress   - Mailbox CSR Address
  @param Data         - Data to be written
  @param DataSize     - Size of Data to be written

  DurableCacheLineWrite( addr, data ) performs the following:

  (1) UC Write of cache line of write data using CSR addr
  (2) UC Read of same cache line as step b, using CSR addr (See Note* below)

  Note*: IMC will block new requests to the same address as a pending request in the buffer.
         Read forces previous write data to be flushed from WPQ to SXPs Durability Domain

**/
VOID
DurableCacheLineWrite (
  UINTN CsrAddress,
  UINT64 Data,
  UINT8 DataSize
  );

/**

  Returns mailbox for given Dimm.

  @param NvmDimmPtr   - NVDIMM structure, describing current Dimm
  @param UseOSMailbox - If TRUE, use the OS mailbox. BIOS mailbox otherwise.

  @return Pointer to CR Mailbox for given NVDIMM

**/
CR_MAILBOX *
GetMailbox (
  IN NVDIMM  *NvmDimmPtr,
  IN BOOLEAN UseOSMailbox
  );

/**

  This function returns Platform Config Data Area size.
  Currently this is not implemented in NVMCTLR F/W and hence this
  call is not implemented. However, we will need to revisit this
  when the F/W start to support this call.

  @param Socket          - Socket index at system level
  @param Ch              - Channel index at socket level
  @param Dimm            - DIMM slot index within DDR channel
  @param PartitionId     - Partition ID for which the PCD size is needed
  @param *SizePtr        - Pointer to return the size data.

  @return Status - Status of the operation

**/
EFI_STATUS
GetPlatformConfigDataAreaSize (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     PartitionId,
  OUT UINT32    *SizePtr
  );

/**

  This function returns Platform Config Data.

  @param[in]     Socket          - Socket index at system level
  @param[in]     Ch              - Channel index at socket level
  @param[in]     Dimm            - DIMM slot index within DDR channel
  @param[out]    OutputBuffer    - Buffer for PCD data
  @param[in,out] OutputSize      - On entry size of OutputBuffer, on exit size of data in OutputBuffer
  @param[in]     Offset          - Offset to read
  @param[in]     PartitionId     - Partition ID for which the PCD
  @param[in]     UseLargePayload - Whether large peyload should be used

  @return EFI status of the operation is returned.

**/
EFI_STATUS
EFIAPI
GetPlatformConfigDataAreaData (
  IN     UINT8   Socket,
  IN     UINT8   Ch,
  IN     UINT8   Dimm,
  OUT    UINT8  *OutputBuffer,
  IN OUT UINT32 *OutputSize,
  IN     UINT32  Offset,
  IN     UINT8   PartitionId,
  IN     BOOLEAN UseLargePayload
  );

/**

  Routine Description: Returns dimm number for
    channel which has a Ngn Dimm.

  @param[in]  Socket      - Socket Number
  @param[in]  Ch          - DDR Channel ID
  @param[out] Dimm        - Dimm number of the channel

  @retval EFI_SUCCESS           Dimm found
  @retval EFI_INVALID_PARAMETER Dimm is NULL
  @retval EFI_NOT_FOUND         Dimm was not found

**/
EFI_STATUS
GetChannelDimm (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  OUT UINT8     *Dimm
  );

INT16
GetRegIndex(
  UINT64    Reg
  );

INT16
GetSPARegIndex(
  NVDIMM    *DimmPtr,
  UINT64    RegSPA
  );

EFI_STATUS
GetDpaFromRegIndex(
  NVDIMM   *DimmPtr,
  INT16     Index,
  UINT64   *RDpaPtr
  );

VOID
WaWpqFlush (
  NVDIMM *Dimm
  );

/**

  This function writes Platform Config Data.

  @param[in]  Socket          - Socket index at system level
  @param[in]  Ch              - Channel index at socket level
  @param[in]  Dimm            - DIMM slot index within DDR channel
  @param[in]  IutputBuffer    - Buffer for PCD data
  @param[in]  IutputSize      - Size of buffer for PCD data
  @param[in]  Offset          - Offset to write
  @param[in]  PartitionId     - Partition ID for which the PCD
  @param[in]  UseLargePayload - Whether large peyload should be used

  @return EFI status of the operation is returned.

**/
EFI_STATUS
EFIAPI
SetPlatformConfigDataAreaData (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     *InputBuffer,
  IN UINT32    InputSize,
  IN UINT32    Offset,
  IN UINT8     PartitionId,
  IN BOOLEAN   UseLargePayload
  );

/**

  Routine Description: Does this system have at least one NVMDIMM

  @return TRUE if NVMDIMM present

**/
BOOLEAN
EFIAPI
IsSystemWithDcpmm (
  VOID
  );

/**
  Issues a WPQ Flush for specific channel.

  @param[in] SocketId       Socket number
  @param[in] ChId           Channel number

  @retval TRUE    Issued a WPQ Flush successfully for specific channel.
  @retval FALSE   Failed to issue a WPQ Flush for specific channel.
**/
BOOLEAN
IssueWpqFlush (
  IN NVDIMM   *Dimm,
  IN BOOLEAN  CsrAccess
  );

/**

  Issue WPQ Flush for all iMCs

  @param None

  @return Standard EFI_STATUS is returned

**/
EFI_STATUS
IssueWpqFlushForAllImc (
  VOID
  );

/**
  Routine Description: Is this Dimm the NVMDIMM.
    CR interface protocol function.

  @param[in] Socket      - Socket Number
  @param[in] Ch          - DDR Channel ID
  @param[in] Dimm        - DIMM number

  @return TRUE if NVMDIMM otherwise FALSE

**/
BOOLEAN
EFIAPI
IsNvmDimm (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm
  );

/**

  Routine Description: Is the given NVMDIMM Enabled

  @param[in] Socket   - Socket ID
  @param[in] Ch       - Channel ID on given Socket
  @param[in] Dimm     - Dimm ID on given channel

  @return TRUE if NVMDIMM is Enabled, FALSE otherwise

**/
BOOLEAN
IsNvmDimmEnabled (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm
  );


/**

  Routine Description: Gets the PMEM DPA base address for given NVDIMM using the
  SPA base address for the relevant SAD of type PMEM.

  @param[in] SadSpaBase  - Base Address of the SAD Rule found
  @param[in] Socket      - Socket Number
  @param[in] Ch          - Channel number
  @param[in] Dimm        - Dimm number

  @return DPA base address

**/
UINT64
GetPmemDpaBase (
  IN UINT64  SadSpaBase,
  IN UINT8   Socket,
  IN UINT8   Ch,
  IN UINT8   Dimm
  );

/**

  Routine Description: This helper function returns interleave ways for the
  given PMEM region.

  @param[in] SpaBaseAddr  - Spa Start Address of the SAD Rule

  @return Interleave ways for PMEM region or 0 if error

**/
UINT8
GetInterleaveWaysForPmemRgn (
  IN UINT64 SadBase
  );

/**
  Routine Description: Returns channel interleave for given socket, Imc, and Sad index.

  @param[in] Socket   - socket number
  @param[in] Imc      - memory controller number
  @param[in] SadInx   - SAD index

  @return Ways of channel interleave for socket/sad
**/
UINT8
GetChInterleave (
  IN UINT8 Socket,
  IN UINT8 Imc,
  IN UINT8 SadInx
  );

/**
  Routine Description: Returns interleave channel bitmap for given socket and SAD.

  @param[in] Socket   - socket number
  @param[in] SadInx   - SAD number

  @return Channel bitmap for given socket/SAD. Counting from BIT0 - channel 1 of first iMC,
          BIT1 - channel 2 of first iMC, etc.
**/
UINT32
GetChInterleaveBitmap (
  IN UINT8 Socket,
  IN UINT8 SadInx
  );

/**
  @brief This function finds SAD rule for a SPA address and returns basic info about it.

  @param[in]  SpaAddress  - SPA address to find matching SAD rule
  @param[out] SktPtr      - Buffer for id of the socket where SAD rule was found
  @param[out] SpaBasePtr  - Buffer for start address of the SAD rule
  @param[out] SpaLimitPtr - Buffer for end address of the SAD rule
  @param[out] MemTypePtr  - Buffer for type of memory this rule provides.

  @retval EFI_SUCCESS     - Local SAD rule found, info updated.
  @retval EFI_NO_MAPPING  - No matching SAD rule found, output variables not changed.
**/
EFI_STATUS
CrGetSadInfo (
  IN  UINT64    SpaAddress,
  OUT UINT8    *SktPtr,
  OUT UINT64   *SpaBasePtr,
  OUT UINT64   *SpaLimitPtr,
  OUT MEM_TYPE *MemTypePtr
  );

/**
  Given SAD base address, function returns socket
  granularity based on the SAD Rule.

  @param[in]  SadBaseAddr  SAD base address

  @return Socket granularity

**/
UINT32
GetSktGranularityFromSadBaseAddr (
  UINT64 SadBaseAddr
  );

/**

  Routine Description: Returns the Index number of the NVMDIMM for a given
    Socket and Channel.

  @param[in]  Socket      - Socket Number
  @param[in]  Ch          - DDR Channel ID in socket
  @param[out] *Index      - Index in mCrInfo.Nvdimms array.

  @retval EFI_SUCCESS           Dimm index found
  @retval EFI_NOT_FOUND         Dimm index not found
  @retval EFI_INVALID_PARAMETER Index is NULL or parameters are invalid

**/
EFI_STATUS
GetDimmIndex (
  IN  UINT8 Socket,
  IN  UINT8 Ch,
  OUT UINT8 *Index
  );

/**

  Utility function to check whether given DIMM is using FNV or EKV controller.
  In the case Subsystem DID is not programmed, assuming older gen.

  @param NvmDimm - NVDIMM structure, describing current Dimm

  @return           - TRUE if FNV or EKV or SubsystemDeviceID == 0x0
**/
BOOLEAN
IsNvmCtrlFirstGen (
  NVDIMM  *NvmDimm
  );

/**
  Routine Description: This function is responsible for getting Socket,
  Channel and Dimm information based on the given System Physical Address.
  That is, to figure out the Nvm Dimm that is mapped to the passed in Spa.

  @param[in]  Spa          - SPA address
  @param[out] *SpaBasePtr  - Base address of the region where SPA belongs
  @param[out] *SktPtr      - Socket that this function will return
  @param[out] *ChPtr       - Channel that this function will return
  @param[out] *DimmPtr     - Dimm that this function will return
  @param[out] *MemTypePtr  - Memory type of the SPA address

  @return EFI status is returnd.
**/
EFI_STATUS
GetDimmInfoFromSpa (
  IN  UINT64       Spa,
  OUT UINT64      *SpaBasePtr,
  OUT UINT8       *SktPtr,
  OUT UINT8       *ChPtr,
  OUT UINT8       *DimmPtr,
  OUT UINT16      *MemTypePtr
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
EFI_STATUS
EFIAPI
SpaToNvmDpa (
  IN  UINT64       Spa,
  OUT UINT8        *Skt,
  OUT UINT8        *Ch,
  OUT UINT8        *Dimm,
  OUT UINT64       *Dpa
  );

/**
  Routine Description: Get/Retrieve the Media Status via Boot status register value.

  @param[in] NvmDimmPtr - Pointer to Dimm structure

  @retval MEDIA_READY                 - media is ready
  @retval WARN_NVMCTRL_MEDIA_NOTREADY - media not ready
  @retval WARN_NVMCTRL_MEDIA_INERROR  - media is in error state or disabled
  @retval WARN_NVMCTRL_MEDIA_RESERVED - media status is reserved
  @retval MEDIA_UNKNOWN               - can't get media status
**/
UINT8
GetMediaStatus (
  IN NVDIMM *NvmDimmPtr
  );

/**

  Routine Description: This function will send Log Page Get Long Operation
  Command to the NVMCTLR Controller and return Status of the command sent.

  @param[in]  Socket       - Socket Number
  @param[in]  Ch           - DDR Channel ID
  @param[in]  Dimm         - DIMM number
  @param[out] Data         - Data returned by the Log Page Get Long Operation
                             Command

  @return Status of command sent.

**/
EFI_STATUS
SendFnvGetLongOperationStatus (
  IN  UINT8   Socket,
  IN  UINT8   Ch,
  IN  UINT8   Dimm,
  OUT VOID    *Data
  );

/**
  @brief Updates the state of long operation for given DIMM.

  @param[in] Skt   Socket ID for given DIMM (0 based)
  @param[in] Ch    Channel ID (0 based, socket based)
  @param[in] Dimm  DIMM slot ID

  @return EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
NvmCtlrLongOpComplete (
  IN UINT8 Skt,
  IN UINT8 Ch,
  IN UINT8 Dimm
  );

/**

  Routine Description: Gets error logs from NVMCTLR. Uses small payload, so
  request count shouldn't be more than 3 for media logs and 7 for thermal logs.
  FIS 1.3 compatible.

  @param[in] Socket            - Socket Number
  @param[in] Ch                - DDR Channel ID
  @param[in] Dimm              - DIMM Number
  @param[in] LogLevel          - Log Level 0 - low priority 1 - high priority
  @param[in] LogType           - 0 - for media logs, 1 for thermal
  @param[in] LogInfo           - 0 - retrieve log data, 1 - retrieve log info
  @param[in] Sequence          - Reads log from specified sequence number
  @param[in] RequestCount      - Max number of log entries requested for this access
  @param[out] *LogData         - pointer to LogData structure.
                                 Caller must assure this data struct is big enough.

  @retval EFI_INVALID_PARAMETER  - one of parameters is not valid
  @retval EFI_OUT_OF_RESOURCES   - requested number of log entries does not fit in small payload
  @retval EFI_SUCCESS            - success
  @retval other                  - see return codes of FisRequest

**/
EFI_STATUS
SendFnvGetErrorLog (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     LogLevel,
  IN  UINT8     LogType,
  IN  UINT8     LogInfo,
  IN  UINT16    Sequence,
  IN  UINT16    RequestCount,
  OUT VOID      *ErrLog,
  IN  UINT32    ErrLogSize
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
  @param[in] UseOsMailbox       - TRUE to use OS mailbox

  @return EFI_STATUS            - Status of the command sent

**/
EFI_STATUS
EFIAPI
NvmCtlrSetAddressRangeScrub (
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
  @param[in]  UseOsMailbox           - TRUE to use OS mailbox

  @return EFI_STATUS            - Status of the command sent

**/
EFI_STATUS
EFIAPI
NvmCtlrGetAddressRangeScrub (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  OUT UINT64    *DpaStartAddress,
  OUT UINT64    *DpaEndAddress,
  OUT UINT64    *DpaCurrentAddress,
  OUT BOOLEAN   *Enabled
  );

EFI_STATUS EFIAPI NvmCtlrReadCfgMem (UINT8 socket, UINT8 ch, UINT8 dimm, UINT32 reg, UINT32 *data);
EFI_STATUS EFIAPI NvmCtlrWriteCfgMem (UINT8 socket, UINT8 ch, UINT8 dimm, UINT32 reg, UINT32 data);
EFI_STATUS EFIAPI NvmCtlrGetHostAlert (UINT8 socket, UINT8 ch, UINT8 dimm, UINT16 hostAlerts[MAX_HOST_TRANSACTIONS]);
EFI_STATUS EFIAPI NvmCtlrSetHostAlert (UINT8 socket, UINT8 ch, UINT8 dimm, UINT16 hostAlerts[MAX_HOST_TRANSACTIONS]);

/**

  Routine Description: This function gets latest error logs form NVMCTLR.
  FIS 1.3 compatible.

  @param[in]  Socket    - Socket Number
  @param[in]  Ch        - DDR Channel ID
  @param[in]  Dimm      - DIMM Number
  @param[in]  LogLevel  - Log Level: 0 - Low Priority, 1 - High Priority
  @param[in]  LogType   - Log Type: 0 - Media, 1 - Thermal
  @param[in]  Count     - Max number of log entries requested
  @param[out] LogBuffer - Pointer to data buffer

  @return EFI_STATUS    - Status of the Command Sent

**/
EFI_STATUS
EFIAPI
NvmCtlrGetLatestErrorLog (
  IN  UINT8        Socket,
  IN  UINT8        Ch,
  IN  UINT8        Dimm,
  IN  UINT8        LogLevel,
  IN  UINT8        LogType,
  IN  UINT8        Count,
  OUT ERR_LOG_DATA *ErrLog
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

EFI_STATUS
EFIAPI
NvmCtlrInjectPoisonError (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT64    Dpa,
  IN  UINT8     MemoryType,
  IN  UINT8     Enable
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
EFI_STATUS
EFIAPI
NvmCtlrGetPlatformInterpretedLss (
  IN  UINT8       Skt,
  IN  UINT8       Ch,
  IN  UINT8       Dimm,
  OUT NVM_INTERPRETED_LSS        *InterpretedLssPtr,
  OUT NVM_INTERPRETED_LSS_REASON *InterpretedLssReasonPtr
  );

/**
  @brief Provide platform interpreted unlatched LSS reason enum.

  NOTE: Although this function shares NVM_INTERPRETED_LSS as the type of
        interpreted LSS value, it never returns the not latched values.
        Only NvmLssClean or NvmLssDirty can be returned.

  @param[in]  Skt                     - Socket index at system level
  @param[in]  Ch                      - Channel index at socket level
  @param[in]  Dimm                    - DIMM slot index within DDR channel
  @param[out] InterpretedLssPtr       - Buffer for interpreted LSS type.
  @param[out] InterpretedLssReasonPtr - Buffer for interpreted LSS reason.

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
NvmCtlrGetPlatformInterpretedUnlatchedDsc (
  IN  UINT8       Skt,
  IN  UINT8       Ch,
  IN  UINT8       Dimm,
  OUT NVM_INTERPRETED_LSS        *InterpretedLssPtr,
  OUT NVM_INTERPRETED_LSS_REASON *InterpretedLssReasonPtr
  );

/**

  Generates and sets NVMCTLR Security Nonce

  @param[in]  NvmDimm     NVDIMM to operate on
  @retval     EFI_STATUS  success if command sent

**/
EFI_STATUS
SendFnvSecurityNonce (
  IN  NVDIMM  *NvmDimm
  );

EFI_STATUS
SendFnvSetConfigLockdown (
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm
  );

EFI_STATUS
GetSmartHealthData (
  UINT8 Socket,
  UINT8 Ch,
  UINT8 Dimm,
  UINT8 *SmartBuffer
  );

EFI_STATUS
UpdateNvmAcpiTable (
  UINT64 *CrystalRidgeTablePtr
  );

EFI_STATUS
UpdateCrystalRidgeSmBiosTable (
  VOID
  );

/**
Routine Description: This function send LSS CMD to All NVDIMMs.
@retval VOID       - None
**/
VOID
LatchSystemShutdownStateInAllDimms (
  VOID
  );

/**
  Returns platform PMEM configuration.

  Returns a bit-wise OR of zero or more of the following.

  @retval  WBCACHED_PMEM_PRESENT  AD-WB interleave sets present in the system.
  @retval  PMEM_PRESENT           App-Direct interleave sets present in the system.

**/
UINT8
PlatformPmemConfiguration (
  VOID
  );

/**
This routine checks if all cpus have arrived in SMM.

The routine queries save state register Processor ID on every cpu. It uses
the return status of the query to decide if a CPU has arrived in SMM or not.

@retval TRUE All Cpus checked in to SMM
@retval FALSE Some Cpu not checked in to SMM
**/
BOOLEAN
CheckAllApCheckedIn (
  OUT UINT16 *NumCheckedInPtr
  );

EFI_STATUS
GetPmemSpaFromDpa (
  IN  NVDIMM   *NvmDimm,
  IN  UINT64   Dpa,
  OUT UINT64   *Spa
  );

EFI_STATUS
EFIAPI
SmmTsodActionsHandler(
   IN       EFI_HANDLE                   DispatchHandle,
   IN CONST EFI_SMM_SW_REGISTER_CONTEXT  *DispatchContext,
   IN OUT   VOID                         *CommBuffer,     OPTIONAL
   IN OUT   UINTN                        *CommBufferSize  OPTIONAL
   );

/**
  Routine Description: Setting S3 Passphrase for all CR dimms to lock them. This passphrase is used to unlock dimms during platform S3 Resume.

  @retval VOID

**/
VOID
S3SetPassphraseForAllDimms (
  VOID
  );

/**
  Routine Description: Unlock all dimms during platform S3 Resume

  @retval VOID

**/
VOID
S3UnlockAllDimms (
  VOID
  );


EFI_STATUS
AepFwActivateHandler(
  IN  EFI_HANDLE                          DispatchHandle,
  IN  CONST EFI_SMM_SW_REGISTER_CONTEXT* DispatchContext, OPTIONAL
  IN OUT VOID* CommBuffer, OPTIONAL
  IN OUT UINTN* CommBufferSize  OPTIONAL
);

/**
  Copies Length of bytes of DSM input data to buffer allocated in SMM.
  Asserts that memory was't overwritten out of bounds of allocated buffer
  by checking the magic that was placed immediately after the buffer
  in additional allocated 4 bytes.

  @param[in,out]  DestBuffer    Pointer to buffer allocated in SMM
  @param[in]      SourceBuffer  Pointer to DSM input buffer that will be copied to SMM
  @param[in]      Length        Size of DSM input buffer in bytes

  @retval VOID

**/
VOID
CopyDsmInputBuffer(
  IN OUT UINT8    *DestBuffer,
  IN CONST VOID   *SourceBuffer,
  IN UINTN        Length
);
#endif // _CRYSTALRIDGE_H_
