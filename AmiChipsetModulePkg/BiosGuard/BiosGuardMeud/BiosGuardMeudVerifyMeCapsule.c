//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2015, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file BiosGuardMeudVerifyMeCapsule.c
   

**/

#include <PiPei.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Setup.h>
#include "Token.h"
#include "AmiCertificate.h"
#include <FlashUpd.h>
#include <Guid/SetupVariable.h>

#pragma pack (1)
typedef struct {
    //
    // AmiBiosGuardRecoveryAddress for Tool Interface
    //
    UINT64                      Signature;                  // _AMIBSGD
    UINT16                      Version;                    // 0
    UINT16                      ReoveryMode;                // 1: Recovery, 2: Capsule
    UINT32                      ReoveryAddress;             // AP pointer;
} AMI_BIOS_GUARD_RECOVERY;
#pragma pack ()

EFI_STATUS
EFIAPI
VerifyMeImage(
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PHYSICAL_ADDRESS       MeImageAddress,
  IN UINT32                     MeImageSize
)
{
    EFI_STATUS  Status;
    UINT32      FailedStage;
    
    Status = VerifyFwImage(PeiServices, (VOID**)&MeImageAddress, (UINT32*)&MeImageSize,(UINT32*)&FailedStage ); 
    
    return Status;
}

EFI_STATUS
EFIAPI
VerifyMe(
  IN CONST EFI_PEI_SERVICES       **PeiServices
  )
{
    EFI_STATUS              Status;    
    EFI_PEI_HOB_POINTERS    HobPointer;
    UINT8                   *RecoveryRomAddress;
    UINTN                   i;
    UINT8*                  MePtr;
    EFI_PHYSICAL_ADDRESS    TempAddress = 0;
    UINTN                   HeaderSize;
#if defined (BIOS_GUARD_ECFW_UPDATE_SUPPORT) && (BIOS_GUARD_ECFW_UPDATE_SUPPORT == 1)
    UINT8*                  EcPtr;
    EFI_PHYSICAL_ADDRESS    EcTempAddress = 0;
#endif
    
    HobPointer.Raw = GetHobList ();
    while ( (HobPointer.Raw = GetNextHob (EFI_HOB_TYPE_UEFI_CAPSULE, HobPointer.Raw)) != NULL ) {
        RecoveryRomAddress = (UINT8 *)HobPointer.Capsule->BaseAddress;
        
        DEBUG((DEBUG_INFO, "Capsule Length = %x\n", HobPointer.Capsule->Length));

#if defined (FWCAPSULE_FILE_FORMAT) && (FWCAPSULE_FILE_FORMAT == 1)
        if (HobPointer.Capsule->Length == (FWCAPSULE_MAX_HDR_SIZE + FLASH_SIZE + BIOSGUARD_ME_ALIGNMENT_SIZE)) {
            HeaderSize = ((APTIO_FW_CAPSULE_HEADER*)RecoveryRomAddress)->CapHdr.HeaderSize;
            DEBUG((DEBUG_INFO, "HeaderSize = %x\n", HeaderSize));
            MePtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FLASH_SIZE + HeaderSize);
            Status = VerifyMeImage(PeiServices, (EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FLASH_SIZE + HeaderSize, BIOSGUARD_ME_ALIGNMENT_SIZE);
            DEBUG ((DEBUG_INFO, "Verify ME Image Status(%r)\n", Status));
            if (EFI_ERROR(Status)) {            
                for (i = 0; i < 0x100; i++) {
                    *MePtr = 0xFF;
                    MePtr++;
                }
                return Status;
            }
            
            Status = PeiServicesAllocatePages (EfiBootServicesData, EFI_SIZE_TO_PAGES (FV_MEFW_CAPSULE_SIZE), &TempAddress);
            if ( EFI_ERROR( Status ) ) {
                ASSERT_EFI_ERROR (Status);
                return Status;
            }
            
            MePtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FLASH_SIZE + HeaderSize + ((APTIO_FW_CAPSULE_HEADER *)(RecoveryRomAddress + FLASH_SIZE + HeaderSize))->RomImageOffset);
            CopyMem((UINT8*)TempAddress, MePtr, FV_MEFW_CAPSULE_SIZE);
            MePtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FLASH_SIZE + HeaderSize);
            CopyMem(MePtr, (UINT8*)TempAddress, FV_MEFW_CAPSULE_SIZE);
        } else if (HobPointer.Capsule->Length == (FWCAPSULE_MAX_HDR_SIZE + FWCAPSULE_IMAGE_ALLIGN + FLASH_SIZE + BIOSGUARD_ME_ALIGNMENT_SIZE)) {
            HeaderSize = ((APTIO_FW_CAPSULE_HEADER*)RecoveryRomAddress)->CapHdr.HeaderSize;
            DEBUG((DEBUG_INFO, "HeaderSize = %x\n", HeaderSize));
            MePtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FWCAPSULE_IMAGE_ALLIGN + FLASH_SIZE + HeaderSize);
            Status = VerifyMeImage(PeiServices, (EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FWCAPSULE_IMAGE_ALLIGN + FLASH_SIZE + HeaderSize, BIOSGUARD_ME_ALIGNMENT_SIZE);
            DEBUG ((DEBUG_INFO, "Verify ME Image Status(%r)\n", Status));
            if (EFI_ERROR(Status)) {            
                for (i = 0; i < 0x100; i++) {
                    *MePtr = 0xFF;
                    MePtr++;
                }
                return Status;
            }
            
            Status = PeiServicesAllocatePages (EfiBootServicesData, EFI_SIZE_TO_PAGES (FV_MEFW_CAPSULE_SIZE), &TempAddress);
            if ( EFI_ERROR( Status ) ) {
                ASSERT_EFI_ERROR (Status);
                return Status;
            }
            
            MePtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FWCAPSULE_IMAGE_ALLIGN + FLASH_SIZE + HeaderSize + ((APTIO_FW_CAPSULE_HEADER *)(RecoveryRomAddress + FWCAPSULE_IMAGE_ALLIGN + FLASH_SIZE + HeaderSize))->RomImageOffset);
            CopyMem((UINT8*)TempAddress, MePtr, FV_MEFW_CAPSULE_SIZE);
            MePtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FWCAPSULE_IMAGE_ALLIGN + FLASH_SIZE + HeaderSize);
            CopyMem(MePtr, (UINT8*)TempAddress, FV_MEFW_CAPSULE_SIZE);
        }
#if defined (BIOS_GUARD_ECFW_UPDATE_SUPPORT) && (BIOS_GUARD_ECFW_UPDATE_SUPPORT == 1)
        else if (HobPointer.Capsule->Length == (FWCAPSULE_MAX_HDR_SIZE + FLASH_SIZE + BIOSGUARD_ME_ALIGNMENT_SIZE + EC_FIRMWARE_BIN_ALIGNMENT_SIZE)) {
            HeaderSize = ((APTIO_FW_CAPSULE_HEADER*)RecoveryRomAddress)->CapHdr.HeaderSize;
            DEBUG((EFI_D_INFO, "HeaderSize = %x\n", HeaderSize));
            MePtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FLASH_SIZE + HeaderSize);
            Status = VerifyMeImage(PeiServices, (EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FLASH_SIZE + HeaderSize, BIOSGUARD_ME_ALIGNMENT_SIZE);
            DEBUG ((EFI_D_INFO, "Verify ME Image Status(%r)\n", Status));
            if (EFI_ERROR(Status)) {            
                for (i = 0; i < 0x100; i++) {
                    *MePtr = 0xFF;
                    MePtr++;
                }
                return Status;
            }
            
            Status = PeiServicesAllocatePages (EfiBootServicesData, EFI_SIZE_TO_PAGES (FV_MEFW_CAPSULE_SIZE), &TempAddress);
            if ( EFI_ERROR( Status ) ) {
                ASSERT_EFI_ERROR (Status);
                return Status;
            }

            Status = PeiServicesAllocatePages (EfiBootServicesData, EFI_SIZE_TO_PAGES (EC_FIRMWARE_BIN_SIZE), &EcTempAddress);
            if ( EFI_ERROR( Status ) ) {
                ASSERT_EFI_ERROR (Status);
                return Status;
            }

            
            MePtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FLASH_SIZE + HeaderSize + ((APTIO_FW_CAPSULE_HEADER *)(RecoveryRomAddress + FLASH_SIZE + HeaderSize))->RomImageOffset);
            CopyMem((UINT8*)TempAddress, MePtr, FV_MEFW_CAPSULE_SIZE);
            EcPtr = (UINT8 *)(MePtr + FV_MEFW_CAPSULE_SIZE);
            CopyMem((UINT8*)EcTempAddress, EcPtr, EC_FIRMWARE_BIN_SIZE);
            MePtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FLASH_SIZE + HeaderSize);
            EcPtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FLASH_SIZE + HeaderSize + FV_MEFW_CAPSULE_SIZE);
            CopyMem(MePtr, (UINT8*)TempAddress, FV_MEFW_CAPSULE_SIZE);
            CopyMem(EcPtr, (UINT8*)TempAddress, EC_FIRMWARE_BIN_SIZE);         
        } else if (HobPointer.Capsule->Length == (FWCAPSULE_MAX_HDR_SIZE + FWCAPSULE_IMAGE_ALLIGN + FLASH_SIZE + BIOSGUARD_ME_ALIGNMENT_SIZE + EC_FIRMWARE_BIN_ALIGNMENT_SIZE)) {
            HeaderSize = ((APTIO_FW_CAPSULE_HEADER*)RecoveryRomAddress)->CapHdr.HeaderSize;
            DEBUG((EFI_D_INFO, "HeaderSize = %x\n", HeaderSize));
            MePtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FWCAPSULE_IMAGE_ALLIGN + FLASH_SIZE + HeaderSize);
            Status = VerifyMeImage(PeiServices, (EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FWCAPSULE_IMAGE_ALLIGN + FLASH_SIZE + HeaderSize, BIOSGUARD_ME_ALIGNMENT_SIZE);
            DEBUG ((EFI_D_INFO, "Verify ME Image Status(%r)\n", Status));
            if (EFI_ERROR(Status)) {            
                for (i = 0; i < 0x100; i++) {
                    *MePtr = 0xFF;
                    MePtr++;
                }
                return Status;
            }
            
            Status = PeiServicesAllocatePages (EfiBootServicesData, EFI_SIZE_TO_PAGES (FV_MEFW_CAPSULE_SIZE), &TempAddress);
            if ( EFI_ERROR( Status ) ) {
                ASSERT_EFI_ERROR (Status);
                return Status;
            }

            Status = PeiServicesAllocatePages (EfiBootServicesData, EFI_SIZE_TO_PAGES (EC_FIRMWARE_BIN_SIZE), &EcTempAddress);
            if ( EFI_ERROR( Status ) ) {
                ASSERT_EFI_ERROR (Status);
                return Status;
            }

            
            MePtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FWCAPSULE_IMAGE_ALLIGN + FLASH_SIZE + HeaderSize + ((APTIO_FW_CAPSULE_HEADER *)(RecoveryRomAddress + FWCAPSULE_IMAGE_ALLIGN + FLASH_SIZE + HeaderSize))->RomImageOffset);
            CopyMem((UINT8*)TempAddress, MePtr, FV_MEFW_CAPSULE_SIZE);
            EcPtr = (UINT8 *)(MePtr + FV_MEFW_CAPSULE_SIZE);
            CopyMem((UINT8*)EcTempAddress, EcPtr, EC_FIRMWARE_BIN_SIZE);
            MePtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FWCAPSULE_IMAGE_ALLIGN + FLASH_SIZE + HeaderSize);
            EcPtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FWCAPSULE_IMAGE_ALLIGN + FLASH_SIZE + HeaderSize + FV_MEFW_CAPSULE_SIZE);
            CopyMem(MePtr, (UINT8*)TempAddress, FV_MEFW_CAPSULE_SIZE);
            CopyMem(EcPtr, (UINT8*)TempAddress, EC_FIRMWARE_BIN_SIZE);         
        }
#endif
#else
        if (HobPointer.Capsule->Length == (FWCAPSULE_MAX_HDR_SIZE + FLASH_SIZE + BIOSGUARD_ME_ALIGNMENT_SIZE)) {
            HeaderSize = ((APTIO_FW_CAPSULE_HEADER*)RecoveryRomAddress)->CapHdr.HeaderSize;
            DEBUG((DEBUG_INFO, "HeaderSize = %x\n", HeaderSize));
            MePtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FLASH_SIZE + HeaderSize);
            Status = VerifyMeImage(PeiServices, (EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FLASH_SIZE + HeaderSize, BIOSGUARD_ME_ALIGNMENT_SIZE);
            DEBUG ((DEBUG_INFO, "Verify ME Image Status(%r)\n", Status));
            if (EFI_ERROR(Status)) {            
                for (i = 0; i < 0x100; i++) {
                    *MePtr = 0xFF;
                    MePtr++;
                }
                return Status;
            }
            
            Status = PeiServicesAllocatePages (EfiBootServicesData, EFI_SIZE_TO_PAGES (FV_MEFW_CAPSULE_SIZE), &TempAddress);
            if ( EFI_ERROR( Status ) ) {
                ASSERT_EFI_ERROR (Status);
                return Status;
            }
            
            MePtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FLASH_SIZE + HeaderSize + ((APTIO_FW_CAPSULE_HEADER *)(RecoveryRomAddress + FLASH_SIZE + HeaderSize))->RomImageOffset);
            CopyMem((UINT8*)TempAddress, MePtr, FV_MEFW_CAPSULE_SIZE);
            MePtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FLASH_SIZE + HeaderSize);
            CopyMem(MePtr, (UINT8*)TempAddress, FV_MEFW_CAPSULE_SIZE);
        } 
#if defined (BIOS_GUARD_ECFW_UPDATE_SUPPORT) && (BIOS_GUARD_ECFW_UPDATE_SUPPORT == 1)
        else if (HobPointer.Capsule->Length == (FWCAPSULE_MAX_HDR_SIZE + FLASH_SIZE + BIOSGUARD_ME_ALIGNMENT_SIZE + EC_FIRMWARE_BIN_ALIGNMENT_SIZE)) {
            HeaderSize = ((APTIO_FW_CAPSULE_HEADER*)RecoveryRomAddress)->CapHdr.HeaderSize;
            DEBUG((EFI_D_INFO, "HeaderSize = %x\n", HeaderSize));
            MePtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FLASH_SIZE + HeaderSize);
            Status = VerifyMeImage(PeiServices, (EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FLASH_SIZE + HeaderSize, BIOSGUARD_ME_ALIGNMENT_SIZE);
            DEBUG ((EFI_D_INFO, "Verify ME Image Status(%r)\n", Status));
            if (EFI_ERROR(Status)) {            
                for (i = 0; i < 0x100; i++) {
                    *MePtr = 0xFF;
                    MePtr++;
                }
                return Status;
            }
            
            Status = PeiServicesAllocatePages (EfiBootServicesData, EFI_SIZE_TO_PAGES (FV_MEFW_CAPSULE_SIZE), &TempAddress);
            if ( EFI_ERROR( Status ) ) {
                ASSERT_EFI_ERROR (Status);
                return Status;
            }

            Status = PeiServicesAllocatePages (EfiBootServicesData, EFI_SIZE_TO_PAGES (EC_FIRMWARE_BIN_SIZE), &EcTempAddress);
            if ( EFI_ERROR( Status ) ) {
                ASSERT_EFI_ERROR (Status);
                return Status;
            }

            
            MePtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FLASH_SIZE + HeaderSize + ((APTIO_FW_CAPSULE_HEADER *)(RecoveryRomAddress + FLASH_SIZE + HeaderSize))->RomImageOffset);
            CopyMem((UINT8*)TempAddress, MePtr, FV_MEFW_CAPSULE_SIZE);
            EcPtr = (UINT8 *)(MePtr + FV_MEFW_CAPSULE_SIZE);
            CopyMem((UINT8*)EcTempAddress, EcPtr, EC_FIRMWARE_BIN_SIZE);
            MePtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FLASH_SIZE + HeaderSize);
            EcPtr = (UINT8 *)((EFI_PHYSICAL_ADDRESS)RecoveryRomAddress + FLASH_SIZE + HeaderSize + FV_MEFW_CAPSULE_SIZE);
            CopyMem(MePtr, (UINT8*)TempAddress, FV_MEFW_CAPSULE_SIZE);
            CopyMem(EcPtr, (UINT8*)TempAddress, EC_FIRMWARE_BIN_SIZE);         
        }
#endif
#endif
        
        HobPointer.Raw = GET_NEXT_HOB (HobPointer);
    }  
    
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
BiosGuardMeudVerifyMeCapsuleEntry (
    IN       EFI_PEI_FILE_HANDLE  FileHandle,
    IN CONST EFI_PEI_SERVICES     **PeiServices
)
{
    EFI_STATUS                       Status;
    EFI_PEI_READ_ONLY_VARIABLE2_PPI  *ReadOnlyVariable;
    SYSTEM_CONFIGURATION             SetupData;
    UINTN                            VariableSize= sizeof (SETUP_DATA);
    EFI_BOOT_MODE                    BootMode;

    Status = PeiServicesLocatePpi (&gEfiPeiReadOnlyVariable2PpiGuid, 0, NULL, (VOID **) &ReadOnlyVariable);
    if ( EFI_ERROR( Status ) ) {
        return Status;
    }    

    Status = ReadOnlyVariable->GetVariable (
                 ReadOnlyVariable,
                 L"IntelSetup",
                 &gEfiSetupVariableGuid,
                 NULL,
                 &VariableSize,
                 &SetupData
             );

    if ( EFI_ERROR( Status ) ) {
        return Status;
    }
    
    Status = PeiServicesGetBootMode (&BootMode);
    if ( EFI_ERROR( Status ) ) {
        DEBUG ((DEBUG_ERROR, "[BootGuardPei.c] Get Boot Mode is fail\n"));
        return   Status;
    }
    
    if ( SetupData.BiosGuardEnabled == 1 && BootMode == BOOT_ON_FLASH_UPDATE) {
        VerifyMe(PeiServices);
    }

    return Status;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2015, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
