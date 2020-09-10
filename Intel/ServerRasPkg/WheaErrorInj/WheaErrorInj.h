/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2019 Intel Corporation. <BR>

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

#ifndef _WHEA_CORE_EINJ_
#define _WHEA_CORE_EINJ_


#include <Protocol/SmmSwDispatch2.h>
#include <IndustryStandard/Acpi.h>
#include <RasSwSmi.h>

#define EFI_IA32_MC0_CTL                      0x400
//
// Data & structure definition
//
#define WHEA_EINJ_SET_ERROR_TYPE_WITH_ADDRSS                    0x08
#define WHEA_INSTRUCTION_CFLUSH                                 0x05

// SSG requested to validate the plug-in with error type 0x1000(bit12 set) to inject memory error
#define WHEA_TEMP_MEM_ERROR_TYPE                                0x1000  // for IFU , DCU error injections
#define WHEA_VENDOR_EXT_TYPE                                    0x80000000  // Intel specified OEM defined errro types
  #define WHEA_MEM_ERROR_REC_TYPE                               BIT0
  #define WHEA_PATROL_SCRUB_ERROR                               BIT1
  #define WHEA_UMA_ERROR_TYPE                                   BIT2

  #define WHEA_MESEG_COR_ERR_INJ_CONSUME_NOW                    BIT0
  #define WHEA_MESEG_COR_ERR_INJ_CONSUME_1MS                    BIT1
  #define WHEA_MESEG_COR_ERR_INJ_CONSUME_G0_S1_RW               BIT2
  #define WHEA_MESEG_COR_ERR_INJ_CONSUME_RESET_WARN             BIT3
  #define WHEA_MESEG_UNCOR_ERR_INJ_CONSUME_NOW                  BIT4
  #define WHEA_MESEG_UNCOR_ERR_INJ_CONSUME_1MS                  BIT5
  #define WHEA_MESEG_UNCOR_ERR_INJ_CONSUME_G0_S1_RW             BIT6
  #define WHEA_MESEG_UNCOR_ERR_INJ_CONSUME_RESET_WARN           BIT7
  #define WHEA_MESEG_ICV_ERR_INJ_CONSUME_NOW                    BIT8
  #define WHEA_MESEG_ICV_ERR_INJ_CONSUME_1MS                    BIT9
  #define WHEA_MESEG_ICV_ERR_INJ_CONSUME_G0_S1_RW               BIT10
  #define WHEA_MESEG_ICV_ERR_INJ_CONSUME_RESET_WARN             BIT11

// Error consume types
#define WHEA_NO_CONSUME                 0x00
#define WHEA_REC_PATROLSCRUB_ERROR      0x01
#define WHEA_CORE_IFU_DCU_ERR           0x02
#define WHEA_MEM_COR_ERROR              0x03

// EINJ - SET_ERROR_TYPE_WITH_ADDRESS Data Structure flags
#define WHEA_SET_ERROR_TYPE_WITH_PROC_APIC_VALID      0x01
#define WHEA_SET_ERROR_TYPE_WITH_MEMORY_ADDRESS_VALID 0x02
#define WHEA_SET_ERROR_TYPE_WITH_SBDF_VALID           0x04

// BIOS runtime Address range used for error injection
#define WHEA_EINJ_ADDR_RANGE                            0x4000
// Error injection operation definitions
#define EINJ_BEGIN_OPERATION                            0x55AA55AA
#define EINJ_END_OPERATION                              0x00000000

// Error injection operation status
#define WHEA_EINJ_OPERATION_BUSY                        0x01
#define WHEA_EINJ_CMD_SUCCESS                           0x00
#define WHEA_EINJ_CMD_UNKNOWN_FAILURE                   0x01
#define WHEA_EINJ_CMD_INVALID_ACCESS                    0x02
#define WHEA_EINJ_CMD_FAILED                            0x03

// Port for triggering SMI
#define R_APM_CNT                                         0xB2

#define MC_ERR_INJ_LCK          0x790
#define MC_PS_INT_DOWNGRADE     0x17f
#define V_PS_INT_DOWNGRADE     0x4  // BIT2

#define ENTRY_INVALID   0xFF

//
// WHEA error injection
//
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_8        7
#define PCH_PCIE_RROT_PORT_AER_UES                      0x104
#define PREV_BOOT_ERR_SRC_HOB_SIZE                      1000
#define WHEA_EINJ_TRIGGER_ERROR_ACTION                  0xFF
#define INJECTION_ADDRESS_VALUE                         0xDEADBEEF;
#define MAX_SCRUB_RETRY_COUNT                           20000

#pragma pack (1)
typedef struct {
  EFI_ACPI_6_2_EINJ_TRIGGER_ACTION_TABLE                   Header;
  EFI_ACPI_6_2_EINJ_INJECTION_INSTRUCTION_ENTRY            Trigger0;
  EFI_ACPI_6_2_EINJ_INJECTION_INSTRUCTION_ENTRY            Trigger1;
  EFI_ACPI_6_2_EINJ_INJECTION_INSTRUCTION_ENTRY            Trigger2;
  EFI_ACPI_6_2_EINJ_INJECTION_INSTRUCTION_ENTRY            Trigger3;
} WHEA_EINJ_TRIGGER_ACTION_TABLE;


typedef struct {
  UINT64                Busy:1;
  UINT64                CmdSts:8;
  UINT64                Resv1:55;
} WHEA_EINJ_OP_STATUS;

typedef struct {
  UINT32    ErrType;
  UINT32    VendorErrTypeExtOffset;
  UINT32    Flags;
  UINT32    ApicId;
  UINT64    MemAddress;
  UINT64    MemAddressRange;
  UINT32    PcieSBDF;
} WHEA_EINJ_SET_ERROR_TYPE_WITH_ADDRESS;

typedef struct {
  UINT32                Length;
  UINT32                SBDF;// This provides a PCIe Segment, Bus, Device and Function number which can be used to read the Vendor ID, Device ID and Rev ID,

  UINT16                VendorId;
  UINT16                DeviceId;
  UINT8                 RevId;
  UINT8                 Resvd[3];
  UINT64                OemDefType1;
  UINT64                OemDefType2;
  UINT64                OemDefType3;
} WHEA_EINJ_VENDOR_EXT;

typedef struct {
  UINT64                OpState;            // opeartion state, begin/end/etc
  UINT64                ErrorToInject[5];
  UINT64                TriggerActionTable;
  UINT64                ErrorInjectCap;    // Qowrd0 -  Eroor injection cap.
  UINT64                OpStatus;
  UINT64                CmdStatus;
  UINT64                EinjAddr;
  UINT64                AddrMask;
  WHEA_EINJ_SET_ERROR_TYPE_WITH_ADDRESS  SetAddressTable;
  WHEA_EINJ_VENDOR_EXT  VendorExt;
  BOOLEAN               PcieErrInjActionTable;
  UINT8                 Skt;
} WHEA_EINJ_PARAM_BUFFER;

#pragma pack ()

#define EINJ_REGISTER_EXECUTE {   \
  EFI_ACPI_6_2_SYSTEM_IO,         \
  0x08,                           \
  0x00,                           \
  EFI_ACPI_6_2_BYTE,              \
  R_APM_CNT                       \
  }

#define EINJ_REGISTER_FILLER {    \
  EFI_ACPI_6_2_SYSTEM_MEMORY,     \
  64,                             \
  0,                              \
  EFI_ACPI_6_2_QWORD,             \
  0                               \
  }

/**
  Do the pre-setup for ME Error Injection

  @param NONE

  @retval NONE
**/
VOID
PreMESegErrInjSetup (
  VOID
  );


/**
  Do the post-setup for ME Error Injection

  @param NONE

  @retval NONE
**/
VOID
PostMESegErrInjSetup (
  VOID
  );

  /**
  Corrupt the data the ME Seg Injection address offset

  @param NONE

  @retval NONE
**/
VOID
CorruptMESegInjOffset (
  VOID
  );


/**

  Call ME interface to get a page offset for ME Error Injection

  @param NONE

  @retval EFI_SUCCESS:              ME Page allocated Successfully
  @retval EFI_OUT_OF_RESOURCES:     ME Page cannot be allocated
  @retval EFI_UNSUPPORTED           Unsupported, No SPS support
**/
EFI_STATUS
GetMEPageForErrorInj (
  VOID
  );

/**

  Call ME interface to initiate the Error consumption

  @retval EFI_SUCCESS:            ME EINJ_CONSUME command successful.
  @retval EFI_ABORTED             ME EINJ_CONSUME command Failed

**/
EFI_STATUS
InitiateUmaErrorConsume(
  VOID
);


/**
  Periodic SMI handler to get the status from ME Seg Error consumption.
  ME will provide the status in around 5 ms after the error consumption is initiated

  @param Handle             - The unique handle assigned to this handler by SmiHandlerRegister().Type
                                            EFI_HANDLE is defined in InstallProtocolInterface() in the UEFI 2.1 Specification.
  @param mPeriodicTimer     -  Pointer to the DispatchContext

  @param CommBuffer         - A pointer to a collection of data in memory that will be conveyed from a non-SMM
                                           environment into an SMM environment. The buffer must be contiguous, physically mapped, and be a physical address.
  @param CommBufferSize     - The size of the CommBuffer.

  @retval EFI_SUCCESS       - The interrupt was handled and quiesced. No other handlers should still be called.

**/
EFI_STATUS
GetMeEInjConsumeStatus (
  IN  EFI_HANDLE     Handle,
  IN  CONST VOID     *PeriodicTimer,
  IN  OUT VOID      *CommBuffer  OPTIONAL,
  IN  OUT UINTN     *CommBufferSize OPTIONAL
  );

BOOLEAN
EFIAPI
ValidateSBDFForInjection (
  IN   UINT32      PcieSBDF
  );

EFI_STATUS
EFIAPI
InjectPciExError (
  IN   UINT32      PcieSBDF,
  IN   UINT32      ErrorToInject,
  IN   WHEA_EINJ_TRIGGER_ACTION_TABLE  *EinjAction,
  IN   WHEA_EINJ_PARAM_BUFFER          *EinjParam
  );

UINT32
EFIAPI
GetRootPortForInjection (
  VOID
  );

EFI_STATUS
EFIAPI
IioErrorInjectionLockHandler (
  IN      EFI_HANDLE                          DispatchHandle,
  IN      CONST EFI_SMM_SW_REGISTER_CONTEXT   *DispatchContext, OPTIONAL
  IN OUT  VOID                                *CommBuffer,      OPTIONAL
  IN OUT  UINTN                               *CommBufferSize   OPTIONAL
  );


VOID
EFIAPI
UnLockInjLogic(
  UINT8 *Node
  );

VOID
EFIAPI
LockInjLogic(
  UINT8 *Node
  );

VOID
EFIAPI
InjectMemoryError (
  UINT32   ErrorToInject
  );

/**

  This function enables the error injection on AEP DIMMS
  
  @retval None
  
**/
VOID
EFIAPI
EnableAepErrorInjection (
  VOID
  );


/**

  This function disables the error injection on AEP DIMMS
  
  @retval None
  
**/
VOID
EFIAPI
DisableAepErrorInjection (
  VOID
  );

VOID
InjectLLCError(
  UINT8 SocketId
  );

#endif  //_WHEA_CORE_EINJ_
