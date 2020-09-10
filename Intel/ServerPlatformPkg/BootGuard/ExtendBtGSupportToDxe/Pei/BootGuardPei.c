//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file BootGuardPei.c
    Chain of trust for Pei

**/
//*************************************************************************

#include "BootGuardPei.h"
//#include <CpuRegs.h>

#if (defined INTEL_BOOT_GUARD_CHAIN_OF_TRUST_SUPPORT) && (INTEL_BOOT_GUARD_CHAIN_OF_TRUST_SUPPORT == 1)
EFI_GUID ReserveBootGuardFvMainHashKeyGuid = RESERVE_BOOT_GUARD_FV_MAIN_HASH_KEY_FFS_FILE_RAW_GUID;



STATIC EFI_PEI_NOTIFY_DESCRIPTOR  BootGuardVerificationForPeiToDxeHandoffEndOfPeiNotifyDesc = {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    
    &gPeiSmmControlPpiGuid,

    BootGuardVerificationForPeiToDxeHandoffEndOfPei
};
#endif

#if (defined FV_BB_VERIFY_FV_AREA) && (FV_BB_VERIFY_FV_AREA == 1)

EFI_GUID ReserveBootGuardFvAreaHashKeyGuid = RESERVE_BOOT_GUARD_FV_AREA_HASH_KEY_FFS_FILE_RAW_GUID;

STATIC EFI_PEI_NOTIFY_DESCRIPTOR  BootGuardVerificationForPeiToOtherFVAreaNotifyDesc = {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiMemoryDiscoveredPpiGuid,
    BootGuardVerificationForPeiToOtherFVArea
};
#endif

CONST EFI_PEI_SERVICES  **ppPS;

/**
  Determine if Boot Guard is supported

  @retval TRUE  - Processor is Boot Guard capable.
  @retval FALSE - Processor is not Boot Guard capable.

**/
BOOLEAN
IsBootGuardSupported (
  VOID
  )
{
  UINT64          BootGuardBootStatus;
  UINT32          BootGuardAcmStatus;
  UINT64          BootGuardCapability;

  BootGuardBootStatus = *(UINT64 *)(UINTN)(TXT_PUBLIC_BASE + R_CPU_BOOT_GUARD_BOOTSTATUS);
  BootGuardAcmStatus  = *(UINT32 *)(UINTN)(TXT_PUBLIC_BASE + R_CPU_BOOT_GUARD_ACM_STATUS);
  BootGuardCapability = AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO) & B_BOOT_GUARD_SACM_INFO_CAPABILITY;

  if (BootGuardCapability != 0) {
    DEBUG ((DEBUG_INFO, "Processor supports Boot Guard.\n"));
    return TRUE;
  } else {
    DEBUG ((DEBUG_ERROR, "Processor does not support Boot Guard.\n"));
    return FALSE;
  }
}

//**********************************************************************
/**
    Loads binary from RAW section of X firwmare volume


    @retval Buffer returns a pointer to allocated memory. Caller
        must free it when done.
    @retval Size returns the size of the binary loaded into the
        buffer.

**/
//**********************************************************************
EFI_STATUS
LocateBootGuardHashKey (
    IN EFI_PEI_SERVICES  **PpSv,
    IN EFI_GUID          *Guid,
    IN OUT VOID          **Buffer
)
{
    EFI_STATUS                    Status;
    EFI_FIRMWARE_VOLUME_HEADER    *pFV;
    UINTN                         FvNum=0;
    EFI_FFS_FILE_HEADER           *ppFile=NULL;
    BOOLEAN                       Found = FALSE;

    Status = (*PpSv)->FfsFindNextVolume (PpSv, FvNum, &pFV);

    while ( TRUE ) {
        Status = (*PpSv)->FfsFindNextVolume( PpSv, FvNum, &pFV );
        if ( EFI_ERROR( Status ) ) {
            return Status;
        }

        ppFile = NULL;

        while ( TRUE ) {
            Status = (*PpSv)->FfsFindNextFile( PpSv,
                                               EFI_FV_FILETYPE_FREEFORM,
                                               pFV,
                                               &ppFile );

            if ( Status == EFI_NOT_FOUND ) {
                break;
            }

            if (CompareGuid( &ppFile->Name, Guid )) {
                Found = TRUE;
                break;
            }
        }

        if ( Found ) {
            break;
        } else {
            FvNum++;
        }
    }

    Status = (*PpSv)->FfsFindSectionData( PpSv,
                                          EFI_SECTION_RAW,
                                          ppFile,
                                          Buffer );

    if ( EFI_ERROR( Status ) ) {
        return EFI_NOT_FOUND;
    }

    return Status;
}

//**********************************************************************
/**
    BootGuardVerificationForPeiToDxeHandoffEndOfPei at end of Pei
    handler.

    @retval PeiServices Pointer to PEI Services Table.
    @retval NotifyDesc Pointer to the descriptor for the Notification
        event that caused this function to execute.
    @retval Ppi Pointer to the PPI data associated with
        this function.

**/
//**********************************************************************

//**********************************************************************
/**
    BootGuardVerificationForPeiToOtherFVArea
    handler.

    @retval PeiServices Pointer to PEI Services Table.
    @retval NotifyDesc Pointer to the descriptor for the Notification
        event that caused this function to execute.
    @retval Ppi Pointer to the PPI data associated with
        this function.

**/
//**********************************************************************
#if (defined FV_BB_VERIFY_FV_AREA) && (FV_BB_VERIFY_FV_AREA == 1) 
STATIC
EFI_STATUS
BootGuardVerificationForPeiToOtherFVArea (
    IN      EFI_PEI_SERVICES          **PeiServices,
    IN      EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
    IN      VOID                      *Ppi
)
{
    EFI_STATUS                             Status;
    UINTN                                  BootGuardHashDataSize = 0;
    UINTN                                  i;
    UINT8                                  CurrentBootGuardFvAreaHash256Val[32];    
    VOID                                   *BootGuardSha256Context;
    UINT8                                  *BootGuardOrgFvAreaHash256;
    RESERVE_BOOT_GUARD_FV_AREA_HASH_KEY    *ReserveBootGuardFvAreaHashKey;
    
    BootGuardHashDataSize = Sha256GetContextSize ();
    Status = ((*PeiServices)->AllocatePool) (PeiServices, BootGuardHashDataSize, &BootGuardSha256Context);
    if ( EFI_ERROR( Status ) ) {
        DEBUG ((DEBUG_ERROR, "[BootGuardPei.c] VerificationForPeiToOtherFVArea :: AllocatePool is fail for BootGuardSha256Context\n"));
        return   Status;
    }

    BootGuardOrgFvAreaHash256 = AllocateZeroPool (sizeof(RESERVE_BOOT_GUARD_FV_AREA_HASH_KEY));
    if (BootGuardOrgFvAreaHash256 == NULL) {
        DEBUG ((DEBUG_ERROR, "[BootGuardPei.c] VerificationForPeiToOtherFVArea :: AllocateZeroPool is fail for RESERVE_BOOT_GUARD_FV_AREA_HASH_KEY\n"));
        return Status;
    }

    Status  = LocateBootGuardHashKey(PeiServices ,&ReserveBootGuardFvAreaHashKeyGuid, &BootGuardOrgFvAreaHash256);
    if ( EFI_ERROR( Status ) ) {
        DEBUG ((DEBUG_ERROR, "[BootGuardPei.c] VerificationForPeiToOtherFVArea :: LocateBootGuardHashKey is fail\n"));
		if(FeaturePcdGet (PcdPfrEnable))	{
        	Status = PfrSendStatusToCpld (0, (UINT8) PFR_STATUS_BLOCK_AUTH_ERROR);
		}
#if (defined INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_FUNCTION) && (INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_FUNCTION == 0)
        PeiServicesSetBootMode(BOOT_IN_RECOVERY_MODE);
        PeiServicesInstallPpi(&RecoveryModePpi);
        if(!PcdGetBool (AmiPcdForceShutdownOnFailedRecovery)){
            PcdSetBoolS(AmiPcdForceShutdownOnFailedRecovery,TRUE);            
        }
#endif
#if (defined INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_FUNCTION) && (INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_FUNCTION == 1)
        (*PeiServices)->ReportStatusCode (
                  PeiServices,
                  EFI_ERROR_CODE,                                          // Type
                  INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_POSTCODE,  // Value
                  0,
                  NULL,
                  NULL
        );
        PostCode (INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_POSTCODE);
        CpuDeadLoop ();
#endif      }
        return   Status;
    }

    ReserveBootGuardFvAreaHashKey = (RESERVE_BOOT_GUARD_FV_AREA_HASH_KEY*)BootGuardOrgFvAreaHash256;

    for ( i = 0; i < sizeof(ReserveBootGuardFvAreaHashKey->BootGuardFvAreaHashKey0); i++ ) {
        DEBUG ((DEBUG_INFO, "[BootGuardPei.c] VerificationForPeiToOtherFVArea :: BootGuardFVAreaHashKey0[%x]= %x.\n", i, ReserveBootGuardFvAreaHashKey->BootGuardFvAreaHashKey0[i]));
    }
    DEBUG ((DEBUG_INFO, "\n[BootGuardPei.c] VerificationForPeiToOtherFVArea :: BootGuardFVAreaSegmentBase0= %x.\n", ReserveBootGuardFvAreaHashKey->BootGuardFvAreaSegmentBase0));
    DEBUG ((DEBUG_INFO, "\n[BootGuardPei.c] VerificationForPeiToOtherFVArea :: BootGuardFVAreaSegmentSize0= %x.\n", ReserveBootGuardFvAreaHashKey->BootGuardFvAreaSegmentSize0));

    Sha256Init (BootGuardSha256Context);
    Sha256Update (BootGuardSha256Context, (UINT8 *)(ReserveBootGuardFvAreaHashKey->BootGuardFvAreaSegmentBase0), (UINTN)ReserveBootGuardFvAreaHashKey->BootGuardFvAreaSegmentSize0);
    Sha256Final (BootGuardSha256Context,  CurrentBootGuardFvAreaHash256Val);

    for ( i = 0; i < sizeof (CurrentBootGuardFvAreaHash256Val); i++ ) {
        DEBUG ((DEBUG_INFO, "[BootGuardPei.c] VerificationForPeiToOtherFVArea :: CurrentBootGuardFvAreaHash256Val[%x]= %x.\n", i, CurrentBootGuardFvAreaHash256Val[i]));
    }

    if ( CompareMem(ReserveBootGuardFvAreaHashKey->BootGuardFvAreaHashKey0, CurrentBootGuardFvAreaHash256Val, 32) ) {
        DEBUG ((DEBUG_INFO, "[BootGuardPei.c] FvArea Verify Fail.\n"));
		if(FeaturePcdGet (PcdPfrEnable))	{
       		Status = PfrSendStatusToCpld (0, (UINT8) PFR_STATUS_BLOCK_AUTH_ERROR);
		}
#if (defined INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_FUNCTION) && (INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_FUNCTION == 0)
        PeiServicesSetBootMode(BOOT_IN_RECOVERY_MODE);
        PeiServicesInstallPpi(&RecoveryModePpi);
        if(!PcdGetBool (AmiPcdForceShutdownOnFailedRecovery)){
            PcdSetBoolS(AmiPcdForceShutdownOnFailedRecovery,TRUE);            
        }
#endif
#if (defined INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_FUNCTION) && (INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_FUNCTION == 1)
        (*PeiServices)->ReportStatusCode (
                  PeiServices,
                  EFI_ERROR_CODE,                                          // Type
                  INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_POSTCODE,  // Value
                  0,
                  NULL,
                  NULL
        );
        PostCode (INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_POSTCODE);
        CpuDeadLoop ();
#endif      }
    }
	if(FeaturePcdGet (PcdPfrEnable))	{
    	Status = PfrSendStatusToCpld (0, (UINT8) PFR_STATUS_BLOCK_AUTH_SUCCESS);
	}
    
    return   Status;
}
#endif

#if (defined INTEL_BOOT_GUARD_CHAIN_OF_TRUST_SUPPORT) && (INTEL_BOOT_GUARD_CHAIN_OF_TRUST_SUPPORT == 1)
STATIC
EFI_STATUS
BootGuardVerificationForPeiToDxeHandoffEndOfPei (
    IN      EFI_PEI_SERVICES          **PeiServices,
    IN      EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
    IN      VOID                      *Ppi
)
{
    EFI_STATUS                                  Status;
    UINTN                                       BootGuardHashDataSize = 0, i;
    UINT8                                       CurrentBootGuardFvMainHash256Val[32];  
    VOID                                        *BootGuardSha256Context;
    UINT8                                       *BootGuardOrgFvMainHash256;
    EFI_BOOT_MODE                               BootMode;
    RESERVE_BOOT_GUARD_FV_MAIN_HASH_KEY     *ReserveBootGuardFvMainHashKey;

    Status = PeiServicesGetBootMode (&BootMode);
    if ( EFI_ERROR( Status ) ) {
        DEBUG ((DEBUG_ERROR, "[BootGuardPei.c] Get Boot Mode is fail\n"));
        return   Status;
    }

    if ( BootMode == BOOT_ON_S3_RESUME ) {
        DEBUG ((DEBUG_INFO, "[BootGuardPei.c] In the BOOT_ON_S3_RESUME\n"));
        return   Status;
    }
   
    BootGuardHashDataSize = Sha256GetContextSize ();
    Status = ((*PeiServices)->AllocatePool) (PeiServices, BootGuardHashDataSize, &BootGuardSha256Context);
    if ( EFI_ERROR( Status ) ) {
        DEBUG ((DEBUG_ERROR, "[BootGuardPei.c] AllocatePool is fail for BootGuardSha256Context\n"));
        return   Status;
    }

    BootGuardOrgFvMainHash256 = AllocateZeroPool (sizeof(RESERVE_BOOT_GUARD_FV_MAIN_HASH_KEY));
    if (BootGuardOrgFvMainHash256 == NULL) {
        DEBUG ((DEBUG_ERROR, "[BootGuardPei.c] AllocateZeroPool is fail for RESERVE_BOOT_GUARD_FV_MAIN_HASH_KEY\n"));
        return Status;
    }

    Status  = LocateBootGuardHashKey(PeiServices ,&ReserveBootGuardFvMainHashKeyGuid, &BootGuardOrgFvMainHash256);
    if ( EFI_ERROR( Status ) ) {
        DEBUG ((DEBUG_ERROR, "[BootGuardPei.c] LocateBootGuardHashKey is fail\n"));
		if(FeaturePcdGet (PcdPfrEnable))	{
	        Status = PfrSendStatusToCpld (0, (UINT8) PFR_STATUS_BLOCK_AUTH_ERROR);
		}
#if (defined INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_FUNCTION) && (INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_FUNCTION == 0)
        PeiServicesSetBootMode(BOOT_IN_RECOVERY_MODE);
        PeiServicesInstallPpi(&RecoveryModePpi);
        if(!PcdGetBool (AmiPcdForceShutdownOnFailedRecovery)){
            PcdSetBoolS(AmiPcdForceShutdownOnFailedRecovery,TRUE);            
        }
#endif
#if (defined INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_FUNCTION) && (INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_FUNCTION == 1)
        (*PeiServices)->ReportStatusCode (
                  PeiServices,
                  EFI_ERROR_CODE,                                          // Type
                  INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_POSTCODE,  // Value
                  0,
                  NULL,
                  NULL
        );
        PostCode (INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_POSTCODE);
        CpuDeadLoop ();
#endif          
        return   Status;
    }

    ReserveBootGuardFvMainHashKey = (RESERVE_BOOT_GUARD_FV_MAIN_HASH_KEY*)BootGuardOrgFvMainHash256;

    for ( i = 0; i < sizeof(ReserveBootGuardFvMainHashKey->BootGuardFvMainHashKey); i++ ) {
        DEBUG ((DEBUG_INFO, "[BootGuardPei.c] BootGuardFvMainHashKey[%x]= %x.\n", i, ReserveBootGuardFvMainHashKey->BootGuardFvMainHashKey[i]));
    }
    Sha256Init (BootGuardSha256Context);
    for( i = 0; i < NUMBER_FVMAIN_SEGMAMENT; i++) {
        DEBUG ((DEBUG_INFO, "\n[BootGuardPei.c] BootGuardFvMainSegmentBase[%x]= %x.\n", i, ReserveBootGuardFvMainHashKey->BootGuardFvMainSegmentBase[i]));
        DEBUG ((DEBUG_INFO, "\n[BootGuardPei.c] BootGuardFvMainSegmentSize[%x]= %x.\n", i, ReserveBootGuardFvMainHashKey->BootGuardFvMainSegmentSize[i]));
        if ((ReserveBootGuardFvMainHashKey->BootGuardFvMainSegmentSize[i] != 0) && (ReserveBootGuardFvMainHashKey->BootGuardFvMainSegmentBase[i] != 0xFFFFFFFF)) {
            Sha256Update (BootGuardSha256Context, (UINT8 *)(ReserveBootGuardFvMainHashKey->BootGuardFvMainSegmentBase[i]), (UINTN)ReserveBootGuardFvMainHashKey->BootGuardFvMainSegmentSize[i]);
        }
    }
    Sha256Final (BootGuardSha256Context,  CurrentBootGuardFvMainHash256Val);

    for ( i = 0; i < sizeof (CurrentBootGuardFvMainHash256Val); i++ ) {
        DEBUG ((DEBUG_INFO, "[BootGuardPei.c] CurrentBootGuardFvMainHash256Val[%x]= %x.\n", i, CurrentBootGuardFvMainHash256Val[i]));
    }

    if ( CompareMem(ReserveBootGuardFvMainHashKey->BootGuardFvMainHashKey, CurrentBootGuardFvMainHash256Val, 32) ) {
        DEBUG ((DEBUG_INFO, "[BootGuardPei.c] FvMainHashKey Verify Fail.\n"));
		if(FeaturePcdGet (PcdPfrEnable))	{
		    Status = PfrSendStatusToCpld (0, (UINT8) PFR_STATUS_BLOCK_AUTH_ERROR);
		}
#if (defined INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_FUNCTION) && (INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_FUNCTION == 0)
        PeiServicesSetBootMode(BOOT_IN_RECOVERY_MODE);
        PeiServicesInstallPpi(&RecoveryModePpi);
        if(!PcdGetBool (AmiPcdForceShutdownOnFailedRecovery)){
            PcdSetBoolS(AmiPcdForceShutdownOnFailedRecovery,TRUE);            
        }
#endif
#if (defined INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_FUNCTION) && (INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_FUNCTION == 1)
        (*PeiServices)->ReportStatusCode (
                  PeiServices,
                  EFI_ERROR_CODE,                                          // Type
                  INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_POSTCODE,  // Value
                  0,
                  NULL,
                  NULL
        );
        PostCode (INTEL_BOOT_GUARD_CHAIN_OF_TRUST_VERIFY_FAILED_POSTCODE);
        CpuDeadLoop ();
#endif  
        return   EFI_SUCCESS;
    }
           
	if(FeaturePcdGet (PcdPfrEnable))	{
    	Status = PfrSendStatusToCpld (0, (UINT8) PFR_STATUS_BLOCK_AUTH_SUCCESS);
	}
    return   Status;
}
#endif

#ifdef ECP_FLAG
EFI_STATUS
BootGuardPeiEntryPoint (
  IN  EFI_FFS_FILE_HEADER  *FfsHeader,
  IN  EFI_PEI_SERVICES     **PeiServices
  )
#else
EFI_STATUS
BootGuardPeiEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
#endif
{
    EFI_STATUS                      Status;
    ppPS = PeiServices; 
    
    if ( IsBootGuardSupported() == FALSE ) {
        return   EFI_SUCCESS;
    }

    if ( (UINT32)AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO) == 0 ) {
        DEBUG ((EFI_D_ERROR, "[BootGuardPei.c] Boot Guard is disabled by Boot Guard Profile Configuration in the Intel Fitc\n"));
        return   EFI_SUCCESS;
    }

    Status = PeiServicesNotifyPpi (&BootGuardVerificationForPeiToDxeHandoffEndOfPeiNotifyDesc);
    if ( EFI_ERROR( Status ) ) {
        return   Status;
    }

#if (defined FV_BB_VERIFY_FV_AREA) && (FV_BB_VERIFY_FV_AREA == 1) 
    Status = PeiServicesNotifyPpi (&BootGuardVerificationForPeiToOtherFVAreaNotifyDesc);
    if ( EFI_ERROR( Status ) ) {
        return   Status;
    }    
#endif
    return   Status;
}

