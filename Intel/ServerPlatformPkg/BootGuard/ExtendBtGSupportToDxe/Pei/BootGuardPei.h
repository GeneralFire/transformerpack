//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file BootGuardPei.h
    Header file for BootGuardPei

**/
//*************************************************************************

#ifndef _BOOT_GUARD_PEI_H_
#define _BOOT_GUARD_PEI_H_

#include <Ppi/EndOfPeiPhase.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Pi/PiFirmwareVolume.h>
#include <Pi/PiFirmwareFile.h>
#include <BaseCryptLib.h>

#include <Token.h>

#include <Library/PostCodeLib.h>
#include <Library/PcdLib.h>
#include <Library/PfrLib.h>

#define NUMBER_FVMAIN_SEGMAMENT 3

#if (defined FV_BB_VERIFY_FV_AREA) && (FV_BB_VERIFY_FV_AREA == 1)
extern EFI_GUID gEfiPeiMemoryDiscoveredPpiGuid;
#define RESERVE_BOOT_GUARD_FV_AREA_HASH_KEY_FFS_FILE_RAW_GUID \
    {0xe869e15d, 0x9a95, 0x433e, 0x88, 0xaa, 0xec, 0xf0, 0x96, 0x2d, 0x21, 0xd2}
#endif

#define RESERVE_BOOT_GUARD_FV_MAIN_HASH_KEY_FFS_FILE_RAW_GUID \
    {0xcbc91f44, 0xa4bc, 0x4a5b, 0x86, 0x96, 0x70, 0x34, 0x51, 0xd0, 0xb0, 0x53}

//
// BtG related definitions
//
#define MSR_BOOT_GUARD_SACM_INFO                                      0x13A
#define B_BOOT_GUARD_SACM_INFO_NEM_ENABLED                            BIT0
#define V_TPM_PRESENT_MASK                                            0x06
#define V_TPM_PRESENT_NO_TPM                                          0
#define V_TPM_PRESENT_DTPM_12                                         1
#define V_TPM_PRESENT_DTPM_20                                         2
#define V_TPM_PRESENT_PTT                                             3
#define B_BOOT_GUARD_SACM_INFO_TPM_SUCCESS                            BIT3
#define B_BOOT_GUARD_SACM_INFO_MEASURED_BOOT                          BIT5
#define B_BOOT_GUARD_SACM_INFO_VERIFIED_BOOT                          BIT6
#define B_BOOT_GUARD_SACM_INFO_CAPABILITY                             BIT32
#define TXT_PUBLIC_BASE                                               0xFED30000
#define R_CPU_BOOT_GUARD_ERRORCODE                                    0x30
#define R_CPU_BOOT_GUARD_BOOTSTATUS                                   0xA0
#define R_CPU_BOOT_GUARD_ACM_STATUS                                   0x328
#define V_CPU_BOOT_GUARD_LOAD_ACM_SUCCESS                             0x8000000000000000
#define B_BOOT_GUARD_ACM_ERRORCODE_MASK                               0x00007FF0


#if (defined INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_FUNCTION) && (INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_FUNCTION == 0)

static EFI_PEI_PPI_DESCRIPTOR RecoveryModePpi = {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiPeiBootInRecoveryModePpiGuid, NULL
	};
	#endif

#if defined(BUILD_WITH_GLUELIB)
#undef SetMem
VOID *
SetMem (
    OUT VOID *Buffer,
    IN UINTN Length,
    IN UINT8 Value
)
{
    return GlueSetMem (Buffer, Length, Value);
}

#undef CopyMem
VOID *
EFIAPI
CopyMem (
    OUT VOID       *DestinationBuffer,
    IN CONST VOID  *SourceBuffer,
    IN UINTN       Length
)
{
    return GlueCopyMem (DestinationBuffer, SourceBuffer, Length);
}
#endif

#ifdef ECP_FLAG
//
// Hash functions definitionS
//
UINTN
EFIAPI
Sha256GetContextSize (
    VOID
);

BOOLEAN
EFIAPI
Sha256Init (
    IN OUT  VOID  *Sha256Context
);

BOOLEAN
EFIAPI
Sha256Update (
    IN OUT  VOID        *Sha256Context,
    IN      CONST VOID  *Data,
    IN      UINTN       DataLength
);

BOOLEAN
EFIAPI
Sha256Final (
    IN OUT  VOID   *Sha256Context,
    OUT     UINT8  *HashValue
);
#endif

#if (defined FV_BB_VERIFY_FV_AREA) && (FV_BB_VERIFY_FV_AREA == 1) 
STATIC
EFI_STATUS
BootGuardVerificationForPeiToOtherFVArea (
    IN      EFI_PEI_SERVICES          **PeiServices,
    IN      EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
    IN      VOID                      *Ppi
);
#endif

STATIC
EFI_STATUS
BootGuardVerificationForPeiToDxeHandoffEndOfPei (
    IN      EFI_PEI_SERVICES          **PeiServices,
    IN      EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
    IN      VOID                      *Ppi
);



#pragma pack (1)

#if (defined FV_BB_VERIFY_FV_AREA) && (FV_BB_VERIFY_FV_AREA == 1) 
typedef struct {
    UINT8             BootGuardFvAreaHashKey0[32];
    UINT32            BootGuardFvAreaSegmentBase0;
    UINT32            BootGuardFvAreaSegmentSize0;
} RESERVE_BOOT_GUARD_FV_AREA_HASH_KEY;
#endif

#if (defined INTEL_BOOT_GUARD_CHAIN_OF_TRUST_SUPPORT) && (INTEL_BOOT_GUARD_CHAIN_OF_TRUST_SUPPORT == 1)
typedef struct {
    UINT8             BootGuardFvMainHashKey[32];
    UINT32            BootGuardFvMainSegmentBase[NUMBER_FVMAIN_SEGMAMENT];
    UINT32            BootGuardFvMainSegmentSize[NUMBER_FVMAIN_SEGMAMENT];
} RESERVE_BOOT_GUARD_FV_MAIN_HASH_KEY;
#endif

#pragma pack ()

#endif

