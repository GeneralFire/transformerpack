//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file CspMeud.c

    Meud related functions.
*/
#include <Uefi.h>
#include <Token.h>
#include <Protocol/SmmSwDispatch.h>
// APTIOV_SERVER_OVERRIDE_START 
#include <Protocol/HeciSmm.h>
#include <Protocol/HeciProtocol.h>
// APTIOV_SERVER_OVERRIDE_END
#include <Protocol/SpsSmmHmrfpoProtocol.h>
#if PFR_AFU_UPDATE_SUPPORT
#if defined(Pfr_SUPPORT) && (Pfr_SUPPORT == 1)
#include <Library/PfrLib.h>
#include <Library/AmiPfrLib.h>
#endif
#endif
#if defined(SPS_MODULE_SUPPORT_ME) && (SPS_MODULE_SUPPORT_ME == 1)
#if defined(SecSMIFlash_SUPPORT) && (SecSMIFlash_SUPPORT == 1)
#include <Protocol/SecSmiFlash.h>
#endif
#endif
#include <Library/PciLib.h>
#include <Library/PchPcrLib.h>
#include <Library/AmiBufferValidationLib.h>
#include <Register/PchRegsPcr.h>
#include <Register/PchRegsPsf.h>
// APTIOV_SERVER_OVERRIDE_START 
//#include <Register/PchRegsPsfCnl.h>
#include <MeAccess.h>
#include <IeHeciRegs.h>
// APTIOV_SERVER_OVERRIDE_END
#include <Sps.h>
#include <AmiLib.h>
#include <AmiDxeLib.h>
#include <Library\AmiFlashLib.h>
#include <Meud.h>
#include "CspMeud.h"

#if defined(SPS_MODULE_SUPPORT_ME) && (SPS_MODULE_SUPPORT_ME == 1)
#if defined(SecSMIFlash_SUPPORT) && (SecSMIFlash_SUPPORT == 1)
static EFI_GUID gEfiSecSmiFlashProtocolGuid = EFI_SEC_SMI_FLASH_GUID;
UINT32          *gFwCapsuleAddress = NULL;
#endif
#endif

#if defined(NonBIOS_Validation) && (NonBIOS_Validation == 1)
#include <Protocol/AmiDigitalSignature.h>
#include "AmiCertificate.h"
#include <Library/AmiBufferValidationLib.h>
#endif

extern volatile UINT8           *gSpiBase;
// APTIOV_SERVER_OVERRIDE_START : Issue with ME downgrade
//SMM_HECI_PROTOCOL       *gSmmHeci = NULL;
HECI_PROTOCOL               *gSmmHeci = NULL;
// APTIOV_SERVER_OVERRIDE_END : Issue with ME downgrade
SPS_SMM_HMRFPO_PROTOCOL *gSpsSmmHmrfpo = NULL;

UINT8                           MacAddr[6];
UINT8                           SsidAddr[6];
UINT8                           SvidAddr[6];
static EFI_PHYSICAL_ADDRESS     gImageBuffer = 0;
static EFI_PHYSICAL_ADDRESS     RomBuffer = 0;
static EFI_PHYSICAL_ADDRESS     gOprBuffer;
OFBD_TC_55_ME_PROCESS_STRUCT    *StructPtr;
UINT32                          gCspMeudBiosLimit;
#if PFR_AFU_UPDATE_SUPPORT
static UINT32                   gImageBufferSize = 0;
UINT8                           gPfrCommand = 0;
BOOLEAN                         gPfrSupport = FALSE;
UINT32                          gStagingAreaOffset = 0;
UINT32                          gStagingAreaSize   = 0;
#endif
//In general we pass the mapping address to flash library. 
//If some region is behind to BIOS, we will pass the offset directly and set to FALSE
BOOLEAN                         gMappingAddress = TRUE;

#if defined(NonBIOS_Validation) && (NonBIOS_Validation == 1)
typedef struct {
    EFI_FFS_FILE_HEADER         FfsHdr;
    EFI_COMMON_SECTION_HEADER   SecHdr;
    EFI_GUID                    SectionGuid;
    UINT8                       FwCapHdr[0];
} AMI_FFS_COMMON_SECTION_HEADER;
#endif

#if defined(NonBIOS_Validation) && (NonBIOS_Validation == 1)
#define AMI_ME_FW_CAPSULE_FFS_GUID \
    {0x05CA020A,0x0FC1,0x11DC,0x90,0x11,0x00,0x17,0x31,0x53,0xEB,0xA8}

#define AMI_ME_FW_CAPSULE_SECTION_GUID \
    {0x87454886,0xF057,0x4032,0xA4,0x86,0x60,0x5A,0x35,0x43,0x5E,0xDD}

EFI_GUID gFwCapsuleGuid           = APTIO_FW_CAPSULE_GUID;
EFI_GUID gPRKeyGuid               = PR_KEY_GUID;
EFI_GUID gMeFwCapFfsGuid          = AMI_ME_FW_CAPSULE_FFS_GUID;

AMI_DIGITAL_SIGNATURE_PROTOCOL    *gAmiSig;
CRYPT_HANDLE                      gpPubKeyHndl;
EFI_SHA256_HASH                   *gHashTbl = NULL;
static UINT8                      gHashDB[SHA512_DIGEST_SIZE];
const UINT8                       *FwBadKey = "$BAD";
EFI_PHYSICAL_ADDRESS              WholeImage = 0;
UINTN                             HashTblBlockCount;
BOOLEAN                           NonBiosValidation = FALSE; //Flag for backward compatible. If use old tool with Signed ME, return error.
#endif

// APTIOV_SERVER_OVERRIDE_START : Me update changes.
UINT64  gNonce = 0;
UINT8   gMeStateBeforeFlashUpdate = 0xFF;

#define SPS_INIT_TIMEOUT        2000000 // [us] Timeout when waiting for InitComplete
#define SPS_RESET_TIMEOUT       5000000
#define SPS_NORMAL_TIMEOUT		2000000
#define STALL_1US               1
#define STALL_1MS               1000
#define STALL_1S                1000000

#define R_PCH_H_PCR_PSF1_T0_SHDW_HECI1_REG_BASE         0x0600                  ///< SPT-H D22F0 PSF base address (CSME: HECI1)
#define R_PCH_PSFX_PCR_T0_SHDW_PCIEN                    0x1C                    ///< PCI configuration space enable bits

UINTN
EFIAPI
MicroSecondDelay (
  IN      UINTN                     MicroSeconds
  );
// APTIOV_SERVER_OVERRIDE_END : Me update changes

FLASH_REGIONS_DESCRIPTOR
FlashRegionsDescriptor[] =
{
    { FDT_BLK, "FDR" },
    { BIOS_BLK, "BIOS" },
    { GBE_BLK, "GBER" },
    { PDR_BLK, "PDR" },
    { DER_BLK, "DER" },
    { BIOS_2ND_BLK, "BIOS" },
    { REG7_BLK, "REG7" },
    { REG8_BLK, "BMC" },
    { REG9_BLK, "DER2" },
    { REGA_BLK, "IE" },
    { REGB_BLK, "GBEA" },
    { REGC_BLK, "REGC" },
    { REGD_BLK, "REGD" },
#if PFR_AFU_UPDATE_SUPPORT
    { PFR_BLK, "PFR" },
#else
    { REGE_BLK, "REGE" },
#endif
    { REGF_BLK, "PTT" },
    { ME_BLK, "MER" },
    { MAX_BLK, "" }
};

BOOLEAN
IsRetryStep(
    IN  OFBD_TC_55_ME_PROCESS_STRUCT  *MeProcessStruct
)
{
    if( (MeProcessStruct->bHandleRequest & BIT06) == BIT06 )
        return TRUE;
    else
        return FALSE;
}
/**
    Check Heci PCI device

    @param VOID

    @retval BOOLEAN
**/
BOOLEAN
IsHeciHidden(
    IN  VOID
)
{
//APTIOV_SERVER_OVERRIDE_START
//    UINT32  D22F0FunctionDisable;

    PchPcrRead32(
        PID_PSF1,

        R_PCH_H_PCR_PSF1_T0_SHDW_HECI1_REG_BASE + R_PCH_PSFX_PCR_T0_SHDW_PCIEN );
        
  //      ,
//APTIOV_SERVER_OVERRIDE_END
     //   &D22F0FunctionDisable );
//APTIOV_SERVER_OVERRIDE_START
   // return ((D22F0FunctionDisable & B_PCH_PSFX_PCR_T0_SHDW_PCIEN_FUNDIS) >> 8);
        return (( B_PCH_PSFX_PCR_T0_SHDW_PCIEN_FUNDIS) >> 8);
//APTIOV_SERVER_OVERRIDE_END
}
/**
    UnHide Heci PCI device

    @param VOID

    @retval BOOLEAN
**/
BOOLEAN
UnHideHeci(
    IN  VOID 
)
{
    if( IsHeciHidden() )
    {
        PchPcrAndThenOr32(
            PID_PSF1,
//APTIOV_SERVER_OVERRIDE_START
            R_PCH_H_PCR_PSF1_T0_SHDW_HECI1_REG_BASE + R_PCH_PSFX_PCR_T0_SHDW_PCIEN,
            (UINT32)~B_PCH_PSFX_PCR_T0_SHDW_PCIEN_FUNDIS,
//APTIOV_SERVER_OVERRIDE_END
            0 );
        return TRUE;
    }
    else
        return FALSE;
}
/**
    Hide Heci PCI device

    @param HeciHide

    @retval VOID
**/
VOID
HideHeci(
    IN  BOOLEAN HeciHide
)
{
    if( HeciHide )
    {
        PchPcrAndThenOr32(
            PID_PSF1,
//APTIOV_SERVER_OVERRIDE_START
            R_PCH_H_PCR_PSF1_T0_SHDW_HECI1_REG_BASE + R_PCH_PSFX_PCR_T0_SHDW_PCIEN,
            (UINT8)~0,
            B_PCH_PSFX_PCR_T0_SHDW_PCIEN_FUNDIS );
//APTIOV_SERVER_OVERRIDE_END
    }
}
// APTIOV_SERVER_OVERRIDE_START: Get Nonce Value for ME flash operations
/**
    Get Nonce Value for ME flash operations
    @param  VOID
    @retval EFI_STATUS    - Status of Locate Protocol
            EFI_NOT_FOUND - Nonce is zero
    
**/
EFI_STATUS
GetNonceFromSpsSmm ( 
   VOID ) {

    if ( gNonce == 0 ) {
         gNonce = gSpsSmmHmrfpo->GetNonce();
         if ( gNonce == 0 ) {
              DEBUG ((EFI_D_ERROR, "Nonce is zero! Returning from GetNonceFromSpsSmm()!\n"));
              return EFI_NOT_FOUND;
         }
    }
    return EFI_SUCCESS;
}
// APTIOV_SERVER_OVERRIDE_END: Get Nonce Value for ME flash operations
/**
    Get Host Firmware Status pass to MEUD

    @param VOID

    @retval UINT32
**/
UINT32
GetHfs(
    IN  VOID
)
{
    UINT32  Hfs = 0;
    BOOLEAN HeciIsHide;

    HeciIsHide = UnHideHeci();
    // APTIOV_SERVER_OVERRIDE_START
    Hfs = HeciPciRead32( HECI_R_HFS );
	// APTIOV_SERVER_OVERRIDE_END
    if( Hfs == 0xFFFFFFFF )
        Hfs = 0;

    HideHeci( HeciIsHide );

    return Hfs;
}


/**
    Send HMRFPO MERESET by SpsSmmHmrfpoProtocol

    @param VOID

    @retval EFI_STATUS
**/
// APTIOV_SERVER_OVERRIDE_START :
/**

EFI_STATUS
HmrfpoMeReset(
    IN  VOID
)
{
    EFI_STATUS  Status;
    BOOLEAN     HeciIsHide;

// APTIOV_SERVER_OVERRIDE_START : ME Update Changes
    SPS_MEFS1   MeFs1;
    UINT32      Timeout;
    MKHI_MSG_HMRFPO_MERESET_REQ	HmrfpoMeResetReq;
// APTIOV_SERVER_OVERRIDE_END : ME Update Changes
// APTIOV_SERVER_OVERRIDE_START : Issue with ME downgrade
//    UINT32      RecLength;
// APTIOV_SERVER_OVERRIDE_END : Issue with ME downgrade
    if( gSpsSmmHmrfpo == NULL )
        return EFI_DEVICE_ERROR;

    HeciIsHide = UnHideHeci();

// APTIOV_SERVER_OVERRIDE_START : ME Update Changes
#if 0
    Status = gSpsSmmHmrfpo->MeReset();
#endif

	//
	// Get nonce from Sps Smm Hmrfpo protocol
	//
    Status = GetNonceFromSpsSmm();
    if ( EFI_ERROR(Status) ) {
    	DEBUG ((EFI_D_ERROR, "Unable to get Nonce. Returning from HMRFPO_ENABLE_MSG!\n")); 
    	return Status;
    }

    HmrfpoMeResetReq.Mkhi.Data = 0x00000005;
    HmrfpoMeResetReq.Nonce = gNonce;

// APTIOV_SERVER_OVERRIDE_START : Issue with ME downgrade
//    Status = SmmHeci->SendMsg(HECI1_DEVICE, (UINT32*)&HmrfpoMeResetReq, sizeof(HmrfpoMeResetReq),
//                                          SPS_CLIENTID_BIOS, SPS_CLIENTID_ME_MKHI);
//    RecLength = sizeof(MKHI_MSG_HMRFPO_MERESET_RSP);
//    Status = SmmHeci->SendwACK (HECI1_DEVICE, (UINT32*)&HmrfpoMeResetReq, sizeof(HmrfpoMeResetReq), &RecLength,
//                                          SPS_CLIENTID_BIOS, SPS_CLIENTID_ME_MKHI);
// APTIOV_SERVER_OVERRIDE_END : Issue with ME downgrade

    //
    // If Intel ME was not running in operational mode before 
    // flash update, return success.
    //        
    MeFs1.UInt32 = GetHfs();
    if ( gMeStateBeforeFlashUpdate == MeFs1.Bits.CurrentState ) {
        Status = EFI_SUCCESS;
        goto MERESET_END;
    }

    //
    // From ME-BIOS specification:
    // HMRFPO_MERESET message triggers Intel ME-only reset. The reset occurs immediately after the
    // HMRFPO_MERESET response is written to HECI, so if there was no error and the 
    // request has been accepted it is possible that BIOS will not have enough time to read 
    // the response. It is crucial to not repeat the request in such a case. MEFS1 register 
    // should be observed to confirm Intel ME is re-initializing.
    // So check for MEFS1.CurrentState to confirm that ME is re-initializing.
    // ME state transition: RECOVERY -> RESET -> INIT -> OPERATIONAL
    Timeout = SPS_RESET_TIMEOUT / STALL_1MS;
    MeFs1.UInt32 = GetHfs();
        
    while ( MeFs1.Bits.CurrentState != MEFS1_CURSTATE_INIT ) {
        if (!Timeout) {
            Status = EFI_DEVICE_ERROR;
            goto MERESET_END;
        } else if ( gMeStateBeforeFlashUpdate == MeFs1.Bits.CurrentState ) {
        	Status = EFI_SUCCESS;
        	goto MERESET_END;
        }

        MicroSecondDelay(STALL_1MS);
        MeFs1.UInt32 = GetHfs();
        Timeout--;
    }
        
    //
    // Wait for ME init complete. Timeout is 2 seconds.
    //
    Timeout = SPS_INIT_TIMEOUT / STALL_1MS;
    MeFs1.UInt32 = GetHfs();
    while ( !MeFs1.Bits.InitComplete ) {
        if (!Timeout) {
            Status = EFI_DEVICE_ERROR;
            goto MERESET_END;
        } else if ( gMeStateBeforeFlashUpdate == MeFs1.Bits.CurrentState ) {
        	Status = EFI_SUCCESS;
        	goto MERESET_END;
        }

        MicroSecondDelay(STALL_1MS);
        MeFs1.UInt32 = GetHfs();
        Timeout--;
    }
    //
    // Now, ME should be in normal operational State.
    //
        
    Timeout = SPS_NORMAL_TIMEOUT / STALL_1MS;
    MeFs1.UInt32 = GetHfs();
    while ( MeFs1.Bits.CurrentState != MEFS1_CURSTATE_NORMAL ) {
    	if (!Timeout) {
    		Status = EFI_DEVICE_ERROR;
    		goto MERESET_END;
    	} 
        	
    	MicroSecondDelay(STALL_1MS);
    	MeFs1.UInt32 = GetHfs();
    	Timeout--;
    }

MERESET_END:  
// APTIOV_SERVER_OVERRIDE_END : Me update changes.      

    HideHeci( HeciIsHide );

    return Status;
}
**/
// APTIOV_SERVER_OVERRIDE_END :

/**
    Send HMRFPO ENABLE by SpsSmmHmrfpoProtocol

    @param VOID

    @retval EFI_STATUS
**/
EFI_STATUS
HmrfpoEnableMsg(
    IN  VOID
)
{
    EFI_STATUS  Status;
    BOOLEAN     HeciIsHide;

    //If FDOPSS bit in SPI address is not set, the access is accepted
    if( ((*(UINT32*)(gSpiBase + 0x04)) & BIT13) == 0 )
        return EFI_SUCCESS;

    if( gSpsSmmHmrfpo == NULL )
        return EFI_DEVICE_ERROR;

    HeciIsHide = UnHideHeci();

// APTIOV_SERVER_OVERRIDE_START: Build issue resolved
    Status = gSpsSmmHmrfpo->Enable(HMRFPO_EN_REGION_INTEL_ME);
// APTIOV_SERVER_OVERRIDE_END: Build issue resolved

    HideHeci( HeciIsHide );

    return Status;
}
/**
    Send HMRFPO LOCK by SpsSmmHmrfpoProtocol

    @param VOID

    @retval EFI_STATUS
**/
EFI_STATUS
HmrfpoLockMsg(
    IN  VOID
)
{
    EFI_STATUS  Status;
    BOOLEAN     HeciIsHide;

    if( gSpsSmmHmrfpo == NULL )
        return EFI_DEVICE_ERROR;

    HeciIsHide = UnHideHeci();

    Status = gSpsSmmHmrfpo->Lock();

    HideHeci( HeciIsHide );

    return Status;
}
/**
    Mapping AMI defined region type to EDS defined

    @param AmiRegDef

    @retval UINT8

**/
UINT8
RegionMapping(
    IN  UINT8   AmiRegDef
)
{
    UINT8 EdsRegion;

    switch( AmiRegDef )
    {
        //Mapping old definition to region number
        case PDR_BLK:
            EdsRegion = 4;
        break;

        case GBE_BLK:
            EdsRegion = 3;
        break;

        case ME_BLK:
            EdsRegion = 2;
        break;

        case BIOS_BLK:
            EdsRegion = 1;
        break;

        case DER_BLK:
            EdsRegion = 5;
        break;

        case BIOS_2ND_BLK:
            EdsRegion = 6;
        break;

        default:
            EdsRegion = AmiRegDef;
        break;
    }

    return EdsRegion;
}
/**
    Find index of FlashRegionsDescriptor

    @param AmiRegDef AMI defined region type

    @retval UINT8 Return MAX_BLK if not found match index

**/
UINT8
FindDescriptorIndex(
    IN  UINT8   AmiRegDef
)
{
    UINT8   Index;
    UINT8   Regions;

    Regions = sizeof(FlashRegionsDescriptor) / sizeof(FLASH_REGIONS_DESCRIPTOR);
    for( Index = 0; Index < Regions; Index++ )
    {
        if( FlashRegionsDescriptor[Index].FlashRegion == AmiRegDef )
            return Index;
    }

    return MAX_BLK;
}
/**
    Read the FLMSTR1 in descriptor for checking write access

    @param Region Region type

    @retval BOOLEAN TRUE region can be written, FALSE region can't be written

**/
BOOLEAN
CheckRegionWriteAccess(
    IN  UINT8   Region
)
{
    static UINT32   Flmstr1 = 0;
    UINT32          Value32;
    UINT8           EdsRegion;
    UINT8           Shift;

    //If FDOPSS bit in SPI address is not set, the access is accepted
    if( ((*(UINT32*)(gSpiBase + 0x04)) & BIT13) == 0 )
        return TRUE;

    if( Flmstr1 == 0 )
    {
        UINT8   *Address;

        //Read FLMAP1 for Fmba
        Address = (UINT8*)FLASH_BASE_ADDRESS(0x18);
        AmiFlashReadEx( Address, (UINT8*)&Value32, sizeof(UINT32), TRUE );

        //Read FLMSTR1 for write access
        Address = (UINT8*)FLASH_BASE_ADDRESS(((Value32 & (0x000000FF)) << 0x04));
        AmiFlashReadEx( Address, (UINT8*)&Value32, sizeof(UINT32), TRUE );

        Flmstr1 = Value32;
    }
    else
    {
        //Since the descriptor region may be changed in the update process,
        //it should read onboard FLMSTR1 only once.
        Value32 = Flmstr1;
    }

#if defined(SPS_MODULE_SUPPORT_ME) && (SPS_MODULE_SUPPORT_ME == 0)
    //Me region register should be locked, but we will call HMRFPO to unlock
    //just retur true here.
    if( Region == ME_BLK )
        return TRUE;
#else
    //If onboard is ME firmware, check the HFSTS
    if( Region == ME_BLK )
    {
        //Get HFSTS
        Value32 = GetHfs();
        //bit [16:19]
        Value32 &= (BIT16 | BIT17 | BIT18 | BIT19);
        Value32 = Value32 >> 0x10; 
        switch( Value32 )
        {
            case 0x03: //Soft Temporary Disable
            case 0x04: //SECOVER_JMPR
            case 0x05: //SECOVER_MEI_MSG
                return TRUE;
            break;

            default:
                return FALSE;
            break;
        }
    }
#endif

    EdsRegion = RegionMapping( Region );

    if( (EdsRegion == 0xFF) || (EdsRegion == MAX_BLK) )
        return TRUE;

    if( EdsRegion >= 0x0C )
        Shift = EdsRegion - 0x08; //Region 12 to 15 is bit[4:7]
    else
        Shift = EdsRegion + 0x14; //Region 0 to 11 is bit[20:31]

    return (((Value32 >> Shift) & 0x01) == 0x01) ? TRUE : FALSE;
}
/**
    Compare the FREG register from SPI base and FLREG register
    in update image to determin wheter region is changed.
    If this function checks descriptor region, it will check all
    FREG and FLREG registers in order to avoiding region layout
    change.

    @param Region Region type

    @retval BOOLEAN TRUE region range is different, FALSE region range is same
**/
BOOLEAN
RegionRangeChanged(
    IN  UINT8   Region
)
{
    UINT8   EdsRegion;
    UINT8   Limit;
    UINT32  Offset;

    EdsRegion = RegionMapping( Region );

    //Undefined region or OPR just assume not changed
    if( (EdsRegion == 0xFF) || (EdsRegion == MAX_BLK) )
        return FALSE;

    if( Region == FDT_BLK )
        Limit = 0x10;  //Check all registers if it's descriptor
    else
        Limit = EdsRegion + 0x01;  //Only check single region register

    for( ; EdsRegion < Limit; EdsRegion++ )
    {
        UINT32  Freg;
        UINT32  Flreg;

        //Get on-board FREG from SPI base
        if( EdsRegion >= 0x0C )
            //Region 12 to 15 is start from offset 0xE0
            Offset = 0xE0 + ((EdsRegion - 0x0C) * sizeof(UINT32));
        else
            //Region 0 to 11 is start from offset 0x54
            Offset = 0x54 + EdsRegion * sizeof(UINT32);

        Freg = *(UINT32*)(gSpiBase + Offset);

        //Get FLREG from image's descriptor region
        Flreg = *(UINT32*)(gImageBuffer + 0x40 + (EdsRegion * sizeof(UINT32)));

        if( Freg != Flreg )
            return TRUE;
    }

    return FALSE;
}
/**
    Compare the on-board FPT partition in ME region with update image's.

    @param MeRegion ME region in memory buffer
    @param IsIdentical Return ture if content is identical

    @retval EFI_STATUS
**/
EFI_STATUS
CheckFptPartition(
    IN  VOID    *MeRegion,
    OUT BOOLEAN *IsIdentical
)
{
    UINT8       Counter;
    EFI_STATUS  Status;
    UINTN       BufferSize;
    VOID        *SmmBuffer;
    UINT32      Offset;
    UINT32      Length;
    UINT8       *Address;
    VOID        *ImageFpt;
    VOID        *RomFpt;

    //Send hmrfpo enable to get read access to ME region
    for( Counter = 0; Counter < 0x03; Counter++ )
    {
        Status = HmrfpoEnableMsg();
        if( !EFI_ERROR(Status) )
        {
            break;
        }
        else
        {
            DEBUG((EFI_D_ERROR, "HmrfpoEnableMsg: %r\n", Status));
        }
    }

    BufferSize = FPT_LENGTH;
    Status = pSmst->SmmAllocatePool(
                        EfiRuntimeServicesData,
                        BufferSize,
                        &SmmBuffer );
    if( EFI_ERROR(Status) )
    {
        HmrfpoLockMsg();
        return Status;
    }

    Status = GetRegionOffset( ME_BLK, &Offset, &Length );
    if( EFI_ERROR(Status) )
    {
        HmrfpoLockMsg();
        return Status;
    }

    //Read on-board FPT partition
    Address = (UINT8*)FLASH_BASE_ADDRESS(FPT_START);
    AmiFlashReadEx( Address, SmmBuffer, (UINT32)BufferSize, TRUE );

    //First 48 bytes seems version, skip compare it
    ImageFpt = (VOID*)((UINT8*)MeRegion + (FPT_START - Offset) + 0x30);
    RomFpt = (VOID*)((UINT8*)SmmBuffer + 0x30);
    Length = (UINT32)MemCmp( ImageFpt, RomFpt, (BufferSize - 0x30) );

    Status = pSmst->SmmFreePool( SmmBuffer );

    if( Length != 0 )
        *IsIdentical = FALSE;
    else
        *IsIdentical = TRUE;

    Status = HmrfpoLockMsg();
    if( EFI_ERROR(Status) )
    {
        DEBUG((EFI_D_ERROR, "HmrfpoLockMsg: %r\n", Status));
    }

    return EFI_SUCCESS;
}
/**
    Get regions' offset and length by FDOC register

    @param Region
    @param Offset
    @param Length

    @retval EFI_STATUS

**/
EFI_STATUS
GetRegionOffset(
    UINT8   Region,
    UINT32  *Offset,
    UINT32  *Length
)
{
    volatile UINT32 *FDOC;
    volatile UINT32 *FDOD;
    UINT8           EdsRegion;
    UINT32          Buffer32;
    UINT32          RegionStart;
    UINT32          RegionEnd;

    InitializeSpiBase();

    FDOC = (UINT32*)(gSpiBase + 0xB4); 
    FDOD = (UINT32*)(gSpiBase + 0xB8);
    *FDOC = 0;

    if( *FDOD != FLASH_VALID_SIGNATURE )
        return EFI_UNSUPPORTED;

    EdsRegion = RegionMapping( Region );

    if( EdsRegion != 0xFF )
        *FDOC = (0x2000 + (EdsRegion * 0x04));
    else
        return EFI_UNSUPPORTED;

    Buffer32 = *FDOD;

    //If the region is unsued
    if( Buffer32 == 0x00007FFF )
        return EFI_UNSUPPORTED;

    RegionEnd = Buffer32 >> 16;
    RegionStart = Buffer32 & 0xFFFF;

    *Offset = RegionStart << 12;
    *Length = (RegionEnd - RegionStart + 1) << 12;

    return EFI_SUCCESS;
}
/**
    Get flash parts' capacity by FDOC register

    @param VOID

    @retval UINT32

**/
UINT32
GetFlashCapacity(
    IN  VOID
)
{
    volatile UINT32 *FDOC;
    volatile UINT32 *FDOD;
    UINT32          FlashDensity = 0;
    UINT16          Components;
    UINT8           i;
    UINT32          j;

    InitializeSpiBase();

    FDOC = (UINT32*)(gSpiBase + 0xB4);
    FDOD = (UINT32*)(gSpiBase + 0xB8);

    *FDOC = 0;
    // Invalid !! Not Support ME
    if( *FDOD != FLASH_VALID_SIGNATURE )
        return 0;

    //Get flash part numbers
    *FDOC = 0x04;
    Components = (*FDOD >> 8) & 0x03;

    *FDOC = 0x1000;
    j = *FDOD;

    for( i = 0; i < ( Components + 1 ); i++ )
    {
        switch( j & 0x07 )
        {
            case 0:
                FlashDensity += 0x80000;
            break;
            case 1:
                FlashDensity += 0x100000;
            break;
            case 2:
                FlashDensity += 0x200000;
            break;
            case 3:
                FlashDensity += 0x400000;
            break;
            case 4:
                FlashDensity += 0x800000;
            break;
            case 5:
                FlashDensity += 0x1000000;
            break;
            case 6:
                FlashDensity += 0x2000000;
            break;
            case 7:
                FlashDensity += 0x4000000;
            break;
        }
        j = j >> 4;
    }

    return FlashDensity;
}
/**
    Report ME Base address and Length to AFU

    @param Func_Num 0 for Command /MEUF , 3 for Command /ME
    @param BASE_Address address of ME region to be updated
    @param Length Length of ME region to be updated

    @retval EFI_STATUS Return the EFI Stauts

**/
EFI_STATUS
CspReportMeInfo(
    IN      UINT8   Func_Num,
    IN  OUT UINT32  *BASE_Address,
    IN  OUT UINT32  *Length
)
{
    // Fill these 2 item for Afu can update BIOS with whole image input
    *BASE_Address = 0;
    *Length = GetFlashCapacity() - FLASH_SIZE;

    switch( Func_Num )
    {
        case 0:
            // Command 0 is for Ignition FW, only ME6 support this.
            return EFI_UNSUPPORTED;
        break;
        case 3:
            // Always rteturn SUCCESS here !!
        break;
        default:
            return EFI_UNSUPPORTED;
        break;
    }

    return EFI_SUCCESS;
}
/**
    Send Enable and Global reset MSG to ME FW.

    @param DispatchHandle
    @param DispatchContext

    @retval VOID

**/
VOID
EFIAPI
MeudSmiHandler(
    IN  EFI_HANDLE                  DispatchHandle,
    IN  EFI_SMM_SW_DISPATCH_CONTEXT *DispatchContext
)
{
    // This is not needed anymore, SPS can be disabled in runtime.
/*
    HmrfpoEnableMsg();
    SEND_MEI_GLOBAL_RESET_MSG();
*/
}
/**
    Locate Smm Heci protocol

    @param Protocol
    @param Interface
    @param Handle

    @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
LocateSmmHeciProtocol(
    CONST EFI_GUID  *Protocol,
    VOID            *Interface,
    EFI_HANDLE      Handle
)
{
    EFI_STATUS  Status;

    Status = pSmst->SmmLocateProtocol(
                        &gSmmHeciProtocolGuid,
                        NULL,
                        (VOID **)&gSmmHeci );
    if( EFI_ERROR(Status) )
    {
        DEBUG((EFI_D_ERROR, "Smm Heci Protocol locate Status: %r\n", Status));
    }

    return Status;
}
/**
    Locate Smm Hmrfpo protocol

    @param Protocol
    @param Interface
    @param Handle

    @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
LocateSmmHmrfpoProtocol(
    CONST EFI_GUID  *Protocol,
    VOID            *Interface,
    EFI_HANDLE      Handle
)
{
    EFI_STATUS  Status;

    Status = pSmst->SmmLocateProtocol(
                        &gSpsSmmHmrfpoProtocolGuid,
                        NULL,
                        (VOID **)&gSpsSmmHmrfpo );
    if( EFI_ERROR(Status) )
    {
        DEBUG((EFI_D_ERROR, "Smm Hmrfpo locate Status: %r\n", Status));
    }

    return Status;
}
#if defined(SPS_MODULE_SUPPORT_ME) && (SPS_MODULE_SUPPORT_ME == 1)
#if defined(SecSMIFlash_SUPPORT) && (SecSMIFlash_SUPPORT == 1)
/**
    Callback function on Secure SmiFlash Protocol for saving FW Capsule Buffer address.

    @param Protocol
    @param Interface
    @param Handle

    @retval EFI_SUCCESS Secure SmiFlash Protocol installed.
    @retval EFI_NOT_FOUND Secure SmiFlash Protocol not install yet.

**/
static
EFI_STATUS
EFIAPI
SecSmiFlashProtocolCallback(
    IN  const EFI_GUID  *Protocol,
    IN  VOID            *Interface,
    IN  EFI_HANDLE      Handle
)
{
    EFI_STATUS                  Status;
    EFI_SEC_SMI_FLASH_PROTOCOL  *SecSmiFlash;

    Status = pSmst->SmmLocateProtocol(
                        &gEfiSecSmiFlashProtocolGuid,
                        NULL,
                        (VOID **)&SecSmiFlash );
    if( EFI_ERROR(Status) )
        return EFI_NOT_FOUND;

    gFwCapsuleAddress = SecSmiFlash->pFwCapsule;

    return EFI_SUCCESS;
}
#endif
#endif
/**
    Elink in OFBD entry

    @param  VOID

    @retval VOID
**/
VOID
CspMeudInSmm(
    IN  VOID
)
{
    EFI_STATUS  Status;
    UINT32      Offset;
    UINT32      Length;
    VOID        *ProtocolNotifyRegistration;
#if defined(NonBIOS_Validation) && (NonBIOS_Validation == 1)
    UINTN       Size;
#endif

    Status = GetRegionOffset( BIOS_BLK, &Offset, &Length );
    if( !EFI_ERROR(Status) )
        gCspMeudBiosLimit = Offset + Length;
    else
        gCspMeudBiosLimit = GetFlashCapacity();
#if PFR_AFU_UPDATE_SUPPORT
//Get the PFR state during POST
#if defined(Pfr_SUPPORT) && (Pfr_SUPPORT == 1)
    Status = PfrSupported();
    if (!EFI_ERROR (Status) && IsPfrProvisioned())
    {
        gPfrSupport     = TRUE;
        gStagingAreaOffset = PFR_STAGING_AREA_BASE;
        gStagingAreaSize   = PFR_STAGING_AREA_SIZE;
    }
#endif
#endif
    //Locate the buffer for saving non-BIOS region image from AFU
    RomBuffer = 0;
#if defined(OEM_REGION_ORDER) && (OEM_REGION_ORDER == 1)                
    Status = pBS->AllocatePages(
                    AllocateAnyPages,
                    EfiRuntimeServicesData,
                    EFI_SIZE_TO_PAGES(GetFlashCapacity()),
                    &RomBuffer );
#else  
    Status = pBS->AllocatePages(
                    AllocateAnyPages,
                    EfiRuntimeServicesData,
                    EFI_SIZE_TO_PAGES(GetFlashCapacity() - FLASH_SIZE),
                    &RomBuffer );
#endif    
    if( EFI_ERROR(Status) )
        return;

    //Locate the buffer for saving OPR partiton in ME region
    gOprBuffer = 0;
    Status = pBS->AllocatePages(
                    AllocateAnyPages,
                    EfiRuntimeServicesData,
                    EFI_SIZE_TO_PAGES(OPR_BUFFER_LENGTH),
                    &gOprBuffer );
    if( EFI_ERROR(Status) )
        return;

    Status = pSmst->SmmLocateProtocol(
                        &gSmmHeciProtocolGuid,
                        NULL,
                        (VOID **)&gSmmHeci );
    if( EFI_ERROR(Status) )
    {
        DEBUG((EFI_D_ERROR, "Smm Heci Protocol locate Status: %r\n", Status));
        pSmst->SmmRegisterProtocolNotify(
                    &gSmmHeciProtocolGuid,
                    LocateSmmHeciProtocol,
                    &ProtocolNotifyRegistration );
    }

    Status = pSmst->SmmLocateProtocol(
                        &gSpsSmmHmrfpoProtocolGuid,
                        NULL,
                        (VOID **)&gSpsSmmHmrfpo );
    if( EFI_ERROR(Status) )
    {
        ProtocolNotifyRegistration = NULL;
        pSmst->SmmRegisterProtocolNotify(
                    &gSpsSmmHmrfpoProtocolGuid,
                    LocateSmmHmrfpoProtocol,
                    &ProtocolNotifyRegistration );
    }

#if defined(NonBIOS_Validation) && (NonBIOS_Validation == 1)
    Status = pSmst->SmmLocateProtocol(&gAmiSmmDigitalSignatureProtocolGuid, NULL, (VOID **)&gAmiSig);
    if( !EFI_ERROR(Status) )
    {
        Status = pBS->AllocatePages(
                        AllocateAnyPages,
                        EfiReservedMemoryType,
                        EFI_SIZE_TO_PAGES(GetFlashCapacity()),
                        &WholeImage );
        if( EFI_ERROR(Status) )
            WholeImage = 0;

        //
        // Allocate space to hold a Hash table for Me blocks in SMM
        //
        HashTblBlockCount = ((GetFlashCapacity() - FLASH_SIZE)/FLASH_BLOCK_SIZE);
        Size = (sizeof(EFI_SHA256_HASH)*HashTblBlockCount);
        pSmst->SmmAllocatePool(EfiRuntimeServicesData, Size, (void**)&gHashTbl);

    }
#endif

#if defined(SPS_MODULE_SUPPORT_ME) && (SPS_MODULE_SUPPORT_ME == 1)
#if defined(SecSMIFlash_SUPPORT) && (SecSMIFlash_SUPPORT == 1)
    {
        EFI_SEC_SMI_FLASH_PROTOCOL  *SecSmiFlash;

        Status = pSmst->SmmLocateProtocol(
                            &gEfiSecSmiFlashProtocolGuid,
                            NULL,
                            (VOID **)&SecSmiFlash );
        if( !EFI_ERROR(Status) )
        {
            gFwCapsuleAddress = SecSmiFlash->pFwCapsule;
        }
        else
        {
            VOID    *Registration;

            pSmst->SmmRegisterProtocolNotify(
                    &gEfiSecSmiFlashProtocolGuid,
                    SecSmiFlashProtocolCallback,
                    &Registration );
        }
    }
#endif
#endif

    return;
}
/**
    Handle ME Process

    @param Result
    @param Message

    @retval VOID

**/
VOID
MeProcessHandleResult(
    IN  UINT16  Result,
    IN  CHAR8   *Message
)
{
    EFI_STATUS   Status;

    StructPtr->UpdateResult = Result;

    Status = AmiValidateMemoryBuffer( (VOID*)(UINTN)(StructPtr->ddMessageBuffer), Strlen(Message) );
    if( EFI_ERROR(Status) )
    {
        DEBUG((DEBUG_ERROR, "Status of AmiValidateMemoryBuffer : %r\n", Status));
        return;
    }
    MemCpy( (UINT8*)(UINTN)(StructPtr->ddMessageBuffer), Message, Strlen( Message ) );

    *(CHAR8*)(StructPtr->ddMessageBuffer + Strlen( Message )) = 0;
}
/**
    Send Hmrfpo enable message

    @param InSmm

    @retval EFI_STATUS
**/
EFI_STATUS
SendEnableMessage(
    IN  BOOLEAN InSmm
)
{
    UINT8       Counter;
    EFI_STATUS  Status;

    Counter = 0;
    do
    {
        Status = HmrfpoEnableMsg();
        if( !EFI_ERROR(Status) )
            break;
        Counter++;
        // APTIOV_SERVER_OVERRIDE_START 
        // gSmmHeci->ReInitHeci();
        // APTIOV_SERVER_OVERRIDE_START 
    }while( Counter < 3 );

    GetHfs();

    return Status;
}
/**
    Init the Length and Offset need to be updated. If needed, send ENABLE MESSAGE

    @param ProgramOffset
    @param ProgramLength
    @param Step
    @param InSmm

    @retval EFI_STATUS

**/
EFI_STATUS
HandleBuffer(
    IN  OUT UINT32  *ProgramOffset,
    IN  OUT UINT32  *ProgramLength,
    IN  OUT UINT8   *Step,
    IN      BOOLEAN InSmm
)
{
    EFI_STATUS  Status;
    UINT32      Offset = 0;
    UINT32      Length = 0;
    UINT32      Hfs = GetHfs();

    switch( StructPtr->bBlockType )
    {
        case ME_BLK:
#if defined(SPS_MODULE_SUPPORT_ME) && (SPS_MODULE_SUPPORT_ME == 0)
            if( !(Hfs & BIT09) )
            {
                MeProcessHandleResult(
                    BIT02,
                    "ME FW not Initial done" );

                return EFI_NOT_READY;
            }
#endif
            Status = GetRegionOffset( StructPtr->bBlockType, &Offset, &Length );
            //If Fpt bad or recovery load fault
            if((Hfs & BIT05) || (Hfs & BIT10))
                *Step = UpdateMfsb;
            else
                *Step = UpdateFtprToOpr;
        break;

        case GBE_BLK:
            Status = GetRegionOffset( StructPtr->bBlockType, &Offset, &Length );
            if( Status == EFI_NOT_FOUND )
                return EFI_UNSUPPORTED;

            if( Length )
            {
                UINT8   *Address = (UINT8*)FLASH_BASE_ADDRESS(Offset);
                UINT8   SharedInitControlOffset = 0x27;  //Shared Init Control Word (Word 0x13)
                UINT8   SsidOffset = 0x16;               //Subsystem ID (Word 0x0B)
                UINT8   SvidOffset = 0x18;               //Subsystem Vendor ID (Word 0x0C)
                UINT8   SharedInitHigh;

#if defined(OEM_REGION_ORDER) && (OEM_REGION_ORDER == 1)    
                if(((UINT32)Address & BIT31) == 0)  //In this case, the region behind BIOS won't map to 0xFE000000~0xFFFFFFFF, so use the offset directly
                {
                    Address = (UINT8*)Offset;
                    gMappingAddress = FALSE;
                }
#endif                           

                //Store Ssid and Svid
                AmiFlashReadEx( (Address + SsidOffset), SsidAddr, 0x02, gMappingAddress );                
                AmiFlashReadEx( (Address + SvidOffset), SvidAddr, 0x02, gMappingAddress );
                
                // Store Mac address
                AmiFlashReadEx( (Address + SharedInitControlOffset), &SharedInitHigh, 1, gMappingAddress );

                //Bit 15:14, if the valid field doesn't equal 10b the NVM data is not valid
                if( (SharedInitHigh & 0xC0) != 0x80 )
                {
                    //Other MAC will be in next 0x1000 
                    Address = (UINT8*)FLASH_BASE_ADDRESS(Offset +0x1000);
#if defined(OEM_REGION_ORDER) && (OEM_REGION_ORDER == 1)    
                    if(((UINT32)Address & BIT31) == 0)  //In this case, the region behind BIOS won't map to 0xFE000000~0xFFFFFFFF, so use the offset directly
                    {
                        Address = (UINT8*)Offset;
                        gMappingAddress = FALSE;
                    }
#endif                         
                }

                AmiFlashReadEx( Address, MacAddr, 0x06, gMappingAddress );
            }
            *Step = UpdateNonMeRegion;
        break;

        case REGB_BLK:
            Status = GetRegionOffset( StructPtr->bBlockType, &Offset, &Length );
            if( Status == EFI_NOT_FOUND )
                return EFI_UNSUPPORTED;

            if( Length )
            {
                UINT8   *Address = (UINT8*)FLASH_BASE_ADDRESS(Offset);
#if defined(OEM_REGION_ORDER) && (OEM_REGION_ORDER == 1)    
                if(((UINT32)Address & BIT31) == 0)  //In this case, the region behind BIOS won't map to 0xFE000000~0xFFFFFFFF, so use the offset directly in the CspMeud10Gbe.c
                {
                    Address = (UINT8*)Offset;
                    gMappingAddress = FALSE;
                }
#endif                     

                //This function is a sample code for Intel Ethernet Connection X722.
                //Please following the 10GBE spec and porting these functions if needed.
                Process10GbeMac( Address, Save10GbeMac );
            }
            *Step = UpdateNonMeRegion;
        break;
#if PFR_AFU_UPDATE_SUPPORT
        case PFR_BLK:
            //Since the PFR update can use PFR binary and full binary, so we need to assign correct offset before updating.
            if(gPfrSupport)
            {
                Status = GetRegionOffset( StructPtr->bBlockType, &Offset, &Length );
                if( Status == EFI_NOT_FOUND )
                    return EFI_UNSUPPORTED;

                //Image buffer size is less than (SPI size - BIOS region size), we assume the input file is PFR only.
                if(gImageBufferSize < (GetFlashCapacity() - FLASH_SIZE))
                {
                    //Return staging area offset from the start of the image buffer.
                    //Use the image buffer size as length since the input file size might be small than PFR_STAGING_AREA_SIZE
                    Offset = gStagingAreaOffset - Offset;
                    Length = gImageBufferSize;
                }
                //Input file is full binary, so return staging area offset directly.
                else
                {
                    Offset = gStagingAreaOffset;
                    Length = gStagingAreaSize;
                }
            }
        break;
#endif
        default:
            Status = GetRegionOffset( StructPtr->bBlockType, &Offset, &Length );
            if( EFI_ERROR(Status) )
                return EFI_UNSUPPORTED;
            *Step = UpdateNonMeRegion;
        break;
    }

    *ProgramOffset = Offset;
    *ProgramLength = Length;

    return EFI_SUCCESS;
}
/**
    Handle Program Length and Offset , Steps and Program Buffer

    @param ProgramOffset
    @param ProgramLength
    @param Step
    @param InSmm
    @param ProgramBuffer

    @retval EFI_STATUS
**/
EFI_STATUS
HandleMeUpdateSteps(
    IN  OUT UINT32  *ProgramOffset,
    IN  OUT UINT32  *ProgramLength,
    IN      UINT8   Step,
    IN      BOOLEAN InSmm,
    IN  OUT UINT8   **ProgramBuffer
)
{
    EFI_STATUS  Status;
    UINT32      MeOffset;
    UINT32      MeLength;
    UINT32      Offset = 0;
    UINT32      Length = 0;

    Status = GetRegionOffset( ME_BLK, &MeOffset, &MeLength );
    if( EFI_ERROR(Status) )
    {
        //If get ME region base error, use token
        MeOffset = FPT_START - FPT_OFFSET;
    }

    if( Step == UpdateFtprToOpr )
    {
        //Program FTPR to OPR1 and OPR2, steps 3 ~ 6 in spec

        //Send Enable to ME
        //Retry 3 times for Enable message.
        Status = SendEnableMessage( InSmm );
        if( EFI_ERROR(Status) )
        {
            MeProcessHandleResult(
                (BIT00 | BIT02),
                "Error when sending Enable Message to ME !!" );
            return Status;
        }

        //Initialze Opr buffer
        MemSet( (UINT8*)(gOprBuffer), OPR_BUFFER_LENGTH, 0xFF );

        //Copy image FTPR partition to OPR1 in OPR buffer
        MemCpy(
            (UINT8*)(gOprBuffer),
            (UINT8*)(gImageBuffer + FTPR_START),
            FTPR_LENGTH );

        //Copy image FTPR partition to OPR2 in OPR buffer
        if( OPR2_LENGTH )
        {
            MemCpy(
                (UINT8*)(gOprBuffer + OPR1_LENGTH),
                (UINT8*)(gImageBuffer + FTPR_START),
                FTPR_LENGTH );
        }

        //Using prepared OPR buffer for updating
        Offset = MeOffset + OPR1_OFFSET;
        Length = OPR_BUFFER_LENGTH;
        *ProgramBuffer = (UINT8*)gOprBuffer;
    }

    if( Step == UpdateMfsb )
    {
        //Program MFSB, steps 7 ~ 9 in spec

        //HMRFPO_LOCK will be send after program FTPR to OPR,
        //here needs to send HMRFPO_ENABLE again.
        Status = SendEnableMessage( InSmm );
        if( EFI_ERROR(Status) )
        {
            MeProcessHandleResult(
                (BIT00 | BIT02),
                "Error when sending Enable Message to ME !!" );
            return Status;
        }

        //Update Mfsb
        Offset = MeOffset + MFSB_OFFSET;
        Length = MFSB_LENGTH;
        *ProgramBuffer = (UINT8*)(gImageBuffer + Offset);
    }

    if( Step == UpdateFactoryData )
    {
        //Update factory data, steps 10 ~ 11 in spec
        Offset = MeOffset + MFS_OFFSET;

        //Just take care of length
        Length = MFS_LENGTH;
        Length += IVB1_LENGTH;
        Length += IVB2_LENGTH;
        Length += BIS_LENGTH;
        Length += FLOG_LENGTH;
        Length += UTOK_LENGTH;
        Length += UEP_LENGTH;
        Length += FTPR_LENGTH;

        *ProgramBuffer = (UINT8*)(gImageBuffer + Offset);
    }
    
    if( Step == UpdateOpr )
    {
        //Update OPR1 and OPR2, steps 12 ~ 13 in spec
        Offset = MeOffset + OPR1_OFFSET;
        Length = OPR1_LENGTH + OPR2_LENGTH;
        *ProgramBuffer = (UINT8*)(gImageBuffer + Offset);
    }

    *ProgramOffset = Offset;
    *ProgramLength = Length;

    return EFI_SUCCESS;

}
/**
    Verifies if the device has been erased properly or if the current
    byte is the same as the byte to be written at this location

    @param Destination Starting address of where the data will be written
    @param Source Starting address of the data that is supposed to be written 
                    or "NULL" for checking erase completed
    @param Length Length of the data set to check

    @retval BOOLEAN

**/
static
BOOLEAN
IsEqualOrClean(
    IN  UINT8   *Destination,
    IN  UINT8   *Source,
    IN  UINT32  Length
)
{
    UINT8   Buffer[256];
    UINT8   Data;
    UINT32  TempLength = Length;
    UINT32  RemainBytes = Length;
    UINT32  i, j;

    for( i = 0; i < Length; TempLength = RemainBytes, i = (Length - RemainBytes) )
    {
        // Initial local read buffer.
        MemSet( Buffer, 256, 0 );
        // Read flash through SPI Cycle if SpiCycle is requested.
        AmiFlashReadEx( Destination + i, Buffer, 256, gMappingAddress);
        RemainBytes -= 256;
        // Verify whether flash contents is the same as written Data or is clean.
        for( j = 0; j < (TempLength - RemainBytes); j++ )
        {
            Data = (Source != NULL) ? *(Source + i + j) : 0xFF;
            if( Buffer[j] != Data )
                return FALSE;
        }
    }

    return TRUE;
}
/**
    Update region

    @param Buffer
    @param InSmm

    @retval EFI_STATUS
**/
EFI_STATUS
UpdateRegions(
    IN  UINT8   *Buffer,
    IN  BOOLEAN InSmm
)
{
    static UINT32   Offset;
    static UINT32   Length;
    UINT8           *Address;
    EFI_STATUS      Status;
    BOOLEAN         NeedToVerify = FALSE;
    static UINT8    Step = UpdateNonMeRegion;
    static BOOLEAN  NewRegion;
    UINTN           Counter = 0;
    static UINT8    RetryTimes = RETRY_OPR_COUNT;
    static UINT8    *ProgramBuffer;
    static BOOLEAN  FptIsIdentical;
    static BOOLEAN  WriteAccess;
    static BOOLEAN  RegionChanged;
    static BOOLEAN  HmrfpoLockStatus = TRUE;
    UINT8           Index;
    CHAR8           Messages[100];
    UINT16          Result;
#if defined(NonBIOS_Validation) && (NonBIOS_Validation == 1)
    UINT8       *BuffAddr;
    UINT8       *PageAddr;
    UINTN       PageSize;
    UINTN       PageNum;
#endif

#if defined(NonBIOS_Validation) && (NonBIOS_Validation == 1)
    if(NonBiosValidation == FALSE){
        MeProcessHandleResult( (BIT00 | BIT02), "AFU don't support whole signed Image update or can't find capsule Hdr. Stop process" );
        return EFI_SUCCESS;
    }
#endif        

    //Set to TRUE when every FLASH_BLOCK_SIZE updated
    gMappingAddress = TRUE;

    if( IsRetryStep( StructPtr ) == TRUE )
    {
        //Once HMRFPO_LOCK status is not equal to 0, always return error to AFU
        if(!HmrfpoLockStatus)
        {
            MeProcessHandleResult((BIT02 | BIT00), "HMRFPO_LOCK message fails, abort update procedure, please update again" );
            return EFI_SUCCESS;
        }
    }

    // Prepare Offset and Length to be updated
    // If BIT02 , update buffer
    if( (StructPtr->bHandleRequest & BIT02) )
    {
        Status = HandleBuffer( &Offset, &Length, &Step, InSmm );
        if( EFI_ERROR(Status) )
        {
            MeProcessHandleResult( BIT02, "UN SUPPORT TYPE" );
            return Status;
        }
        // Frist In
        NewRegion = TRUE;
        WriteAccess = FALSE;
        RegionChanged = FALSE;
        ProgramBuffer = (UINT8*)(gImageBuffer + Offset);
    }

    // Set MAC address to buffer
    if( ((StructPtr->bBlockType) == GBE_BLK) && NewRegion )
    {
        UINT16  *GBEPtr;
        UINT16  CheckSum16;
        UINT32  i;
        UINT8   SsidOffset = 0x16;               //Subsystem ID (Word 0x0B)
        UINT8   SvidOffset = 0x18;               //Subsystem Vendor ID (Word 0x0C)

        // Set MAC address to buffer
        MemCpy( (Buffer + Offset), MacAddr, 0x06 );

        // Set SSID and SVID address to buffer
        MemCpy( (Buffer + Offset + SsidOffset), SsidAddr, 0x02 );
        MemCpy( (Buffer + Offset + SvidOffset), SvidAddr, 0x02 );

        //Calculate the checksum
        GBEPtr = (UINT16*)(Buffer + Offset);
        CheckSum16 = 0;
        // GBE checksum is from word offset 0x00 to 0x3F
        for( i = 0; i < 0x3F; i++ )
            CheckSum16 += GBEPtr[i];
        // Checksum should be 0xBABA, also include itself.
        CheckSum16 = 0xBABA - CheckSum16;
        // Checksum word is at offset 0x3F;
        MemCpy( &GBEPtr[0x3F], &CheckSum16, sizeof(UINT16) );
    }

    if(((StructPtr->bBlockType) == REGB_BLK) && NewRegion)
    {
        VOID    *Region10Gbe;
        Region10Gbe = (VOID*)(Buffer + Offset);
        //This function is a sample code for Intel Ethernet Connection X722.
        //Please following the 10GBE spec and porting these functions if needed.        
        Process10GbeMac( Region10Gbe, Restore10GbeMac );
#if PFR_AFU_UPDATE_SUPPORT
    }

    //If the input file is PFR only, the offset is not corresponding to the map file.
    //So we need to get the PFR offset again to flash to the correct address.
    if(((StructPtr->bBlockType) == PFR_BLK) && NewRegion)
    {
#endif
    }
//APTIOV_SERVER_OVERRIDE_START: Issue in afu flash /shutdown command with ME.
		if ( gMeStateBeforeFlashUpdate == 0xFF )
    	{
       		gMeStateBeforeFlashUpdate = (0x0F & GetHfs());
    	}
//APTIOV_SERVER_OVERRIDE_END: Issue in afu flash /shutdown command with ME.
    // Handle Step
    if( (StructPtr->bBlockType == ME_BLK) && NewRegion )
    {
#if defined(SPS_MODULE_SUPPORT_ME) && (SPS_MODULE_SUPPORT_ME == 0)
        //Compare FPT partition only one time before update ME region
        if( (StructPtr->bHandleRequest & BIT02) )
        {
            VOID *RegionMe;

            RegionMe = (VOID*)(Buffer + Offset);
            Status = CheckFptPartition( RegionMe, &FptIsIdentical );
            if( EFI_ERROR(Status) )
                return Status;
        }
   
        if( FptIsIdentical == TRUE )
        {
            //FPT partion is identical with update image's, process safe udpate
            Status = HandleMeUpdateSteps(
                        &Offset,
                        &Length,
                        Step,
                        InSmm,
                        &ProgramBuffer );
            if( EFI_ERROR(Status) )
                return Status;
        }
        else
        {
            //FPT partition is changed from update image, process simple update
            //Just take ME regions as normal region so send HmrfpoEnable here
            Status = SendEnableMessage( InSmm );
            if( EFI_ERROR(Status) )
            {
                MeProcessHandleResult(
                    (BIT00 | BIT02),
                    "Error when sending Enable Message to ME !!" );
                return Status;
            }
            //Set step to UpdateNonMeRegion in order to avoid extra steps
            Step = UpdateNonMeRegion;
        }
#else
        Step = UpdateNonMeRegion;
#endif
    }

    if( NewRegion )
    {
        NewRegion = FALSE;
        WriteAccess = CheckRegionWriteAccess( StructPtr->bBlockType );
        RegionChanged = RegionRangeChanged( StructPtr->bBlockType );
    }

    Address = (UINT8*)FLASH_BASE_ADDRESS(Offset);
    
#if defined(OEM_REGION_ORDER) && (OEM_REGION_ORDER == 1)    
    if(((UINT32)Address & BIT31) == 0)  //In this case, the region behind BIOS won't map to 0xFE000000~0xFFFFFFFF, so use the offset directly
    {
        Address = (UINT8*)Offset;
        gMappingAddress = FALSE;
    }
#endif            
    
#if defined(NonBIOS_Validation) && (NonBIOS_Validation == 1)
    PageSize = FLASH_BLOCK_SIZE;
    PageNum  = ((UINTN)Address - (0xFFFFFFFF - gCspMeudBiosLimit + 1 )) / PageSize;
    BuffAddr = (UINT8*)gImageBuffer;
    PageAddr = (UINT8*)((UINTN)BuffAddr + (PageSize *PageNum));
#endif

    if( WriteAccess == TRUE )
    {
        AmiFlashWriteEnable( Address, FLASH_BLOCK_SIZE, gMappingAddress );
        if( IsEqualOrClean( Address, NULL, FLASH_BLOCK_SIZE ) )
        {
#if defined(NonBIOS_Validation) && (NonBIOS_Validation == 1)
         // Mac address and SSID and SVID will be preserved and checksum word is filled during flashing,
         // so GBE region need to skip the hash compare. It's a limitation.
        if(((StructPtr->bBlockType) != GBE_BLK) || ((StructPtr->bBlockType) != REGB_BLK))
        {
            if(PageNum >= HashTblBlockCount)
            {
                MeProcessHandleResult( (BIT00 | BIT02), "Hash Compare Error before flashing." );
                return EFI_SUCCESS;
            }
            // Compare new data digest with the corresponding digest from the Good Hash Table
            // skip the Flash update Op if there is no match
            Status = gAmiSig->Hash(gAmiSig, &gEfiHashAlgorithmSha256Guid, 1,
                                   (const UINT8**)&PageAddr, (const UINTN*)&PageSize, gHashDB);

            if(EFI_ERROR(Status) || MemCmp(gHashDB, gHashTbl[PageNum], SHA256_DIGEST_SIZE) )
            {
                MeProcessHandleResult( (BIT00 | BIT02), "Hash Compare Error before flashing." );
                return EFI_SUCCESS;
            }
        }
#endif
            //The flash content is clean
        AmiFlashProgramEx( Address, ProgramBuffer, FLASH_BLOCK_SIZE, gMappingAddress );
            if( IsEqualOrClean( Address, ProgramBuffer, FLASH_BLOCK_SIZE ) )
                Status = EFI_SUCCESS;
            else
                Status = EFI_DEVICE_ERROR;
        }
        else
        {
            //The flash content is not clean
            if( IsEqualOrClean( Address, ProgramBuffer, FLASH_BLOCK_SIZE ) )
            {
                //The flash content is same
                Status = EFI_SUCCESS;
            }
            else
            {
                //The flash content is different
#if defined(NonBIOS_Validation) && (NonBIOS_Validation == 1)
                // Mac address and SSID and SVID will be preserved and checksum word is filled during flashing,
                // so GBE region need to skip the hash compare. It's a limitation.
               if(((StructPtr->bBlockType) != GBE_BLK) || ((StructPtr->bBlockType) != REGB_BLK))
               {
                   if(PageNum >= HashTblBlockCount)
                   {
                       MeProcessHandleResult( (BIT00 | BIT02), "Hash Compare Error before flashing." );
                       return EFI_SUCCESS;
                   }
                   // Compare new data digest with the corresponding digest from the Good Hash Table
                   // skip the Flash update Op if there is no match
                   Status = gAmiSig->Hash(gAmiSig, &gEfiHashAlgorithmSha256Guid, 1,
                                          (const UINT8**)&PageAddr, (const UINTN*)&PageSize, gHashDB);
                   
                   if(EFI_ERROR(Status) || MemCmp(gHashDB, gHashTbl[PageNum], SHA256_DIGEST_SIZE) )
                   {               
                       MeProcessHandleResult( (BIT00 | BIT02), "Hash Compare Error before flashing." );
                       return EFI_SUCCESS;
                   }
               }
#endif                    
                AmiFlashErase( Address, FLASH_BLOCK_SIZE, gMappingAddress );
                AmiFlashProgramEx( Address, ProgramBuffer, FLASH_BLOCK_SIZE, gMappingAddress );
                if( IsEqualOrClean( Address, ProgramBuffer, FLASH_BLOCK_SIZE ) )
                    Status = EFI_SUCCESS;
                else
                    Status = EFI_DEVICE_ERROR;
            }
        }
        AmiFlashWriteDisable( Address, FLASH_BLOCK_SIZE, gMappingAddress );
    }
    else
    {
        //The region is locked.
        Index = FindDescriptorIndex( StructPtr->bBlockType );

        if( RegionChanged == TRUE )
        {
            //The region range changed, there should be layout changed.
            //Force to stop whole update process.
            if( Index != MAX_BLK )
            {
                Sprintf(
                    Messages,
                    "%s is locked and layout changed, block updating. Or trying assert HDA_SDO pin.",
                    FlashRegionsDescriptor[Index].Command );
            }
            else
                Sprintf( Messages, "There is locked region and layout changed, block updating. Or trying assert HDA_SDO pin.");

            //Show the message and block whole updating process
            MeProcessHandleResult( (BIT00 | BIT02), Messages );
        }
        else
        {
            if( Index != MAX_BLK )
            {
                Sprintf(
                    Messages,
                    "%s is locked, skip updating. Or trying assert HDA_SDO pin.",
                    FlashRegionsDescriptor[Index].Command );
            }
            else
            {
                Sprintf(
                    Messages,
                    "There is locked region, skip updating. Or trying assert HDA_SDO pin." );
            }

            //Show the message and just return the success bit to AFU in
            //order to updating other regions. If needs to block whole updating
            //process, change to set (BIT00 | BIT02)
            MeProcessHandleResult( BIT02, Messages );
        }
        return EFI_SUCCESS;
    }

    if( !EFI_ERROR(Status) )
    {
        //Only increase update process when there is no error
        ProgramBuffer = ProgramBuffer + FLASH_BLOCK_SIZE;
        Length -= FLASH_BLOCK_SIZE;
        Offset += FLASH_BLOCK_SIZE;
    }


    // End of Region Update
    if( !EFI_ERROR(Status) )
    {
        if( Length == 0 )
        {
            NewRegion = TRUE;
#if defined(SPS_MODULE_SUPPORT_ME) && (SPS_MODULE_SUPPORT_ME == 0)
            if( (Step == UpdateFtprToOpr) ||
                (Step == UpdateOpr) )
            {
                NeedToVerify = TRUE;
            }

            //It's simple update process
            if( (Step == UpdateNonMeRegion) && (StructPtr->bBlockType == ME_BLK) )
                NeedToVerify = TRUE;
#endif
        }
        else
        {
            // OEM can output message here in every block updated.
            // Remember to Set BIT02
            MeProcessHandleResult( (BIT01), " " );
            return EFI_SUCCESS;
        }

        if( NeedToVerify )
        {
            Counter = 0;
            do
            {
                Status = HmrfpoLockMsg();
                if( !EFI_ERROR(Status) )
                    break;
                Counter++;
            }while( Counter < 5 );

            // If success.
            if( !EFI_ERROR(Status) )
            {
                if( (Step == UpdateOpr) ||
                    ((Step == UpdateNonMeRegion) && (StructPtr->bBlockType == ME_BLK)) )
                {
                    //Send MERESET message after ME region is updated.
                    //Step UpdateOpr is for safe update, another condition is simple update
                    HmrfpoLockStatus = TRUE;
		    // APTIOV_SERVER_OVERRIDE_START :
                   // Status = HmrfpoMeReset();
		    // APTIOV_SERVER_OVERRIDE_END :
                }
            }
            //Once HMRFPO_LOCK status is not equal to 0, abort update procedure.
            else
            {            
                HmrfpoLockStatus = FALSE;
                MeProcessHandleResult((BIT02 | BIT00), "HMRFPO_LOCK message fails, abort update procedure, please update again" );
                return EFI_SUCCESS;
            }
        }
        else
            Status = EFI_SUCCESS;
    }

    // Show Strings
    if( !EFI_ERROR(Status) )
    {
        switch( Step )
        {
            case UpdateFtprToOpr:
                Result = (BIT01 | BIT02);
                Sprintf( Messages, "Successful Update Recovery Loader to OPRx!!" );
            break;

            case UpdateMfsb:
                Result = (BIT01 | BIT02);
                Sprintf( Messages, "Successful Update MFSB" );
            break;

            case UpdateFactoryData:
                Result = (BIT01 | BIT02);
                Sprintf( Messages, "Successful update factory data" );
            break;

            case UpdateOpr:
                Result = (BIT02);
                Sprintf( Messages, "ME Entire Image update success !!" );
            break;

            default:
            {
                Result = (BIT02);
                Index = FindDescriptorIndex( StructPtr->bBlockType );
                if( Index != MAX_BLK )
                    Sprintf( Messages, "Update success for %s", FlashRegionsDescriptor[Index].Command);
                else
                    Sprintf( Messages, "Update success");
            }
        }
        MeProcessHandleResult( Result, Messages );
        if( Step != UpdateNonMeRegion )
            Step += 1;
    }
    else
    {
        // check Flash verify result first.
        if( Status == EFI_DEVICE_ERROR )
        {
            Sprintf( Messages, "Flash Update Error at address 0x%08X !!", Address );
            MeProcessHandleResult( (BIT00 | BIT02), Messages );
            return EFI_SUCCESS;
        }
        switch( Step )
        {
            case UpdateFtprToOpr:
                Result = (BIT00 | BIT02);
                Sprintf( Messages, "Error when Update Recovery Loader to OPRx!!" );
            break;

            case UpdateMfsb:
                Result = (BIT00 | BIT02);
                Sprintf( Messages, "Error when update MFSB!!" );
            break;

            case UpdateFactoryData:
                Result = (BIT00 | BIT02);
                Sprintf( Messages, "Error when update factory data!!" );
            break;

            case UpdateOpr:
                Result = (BIT00 | BIT02);
                Sprintf( Messages, "Error on ME Entire Image update!!" );
            break;

            default:
                Result = (BIT00 | BIT02);
            break;
        }
        MeProcessHandleResult( Result, Messages );
        if( Step != UpdateNonMeRegion )
            Step += 1;
    }

    return EFI_SUCCESS;
}

#if defined(NonBIOS_Validation) && (NonBIOS_Validation == 1)
/**
    Find Me Cap sig data

    @param Capsule
    @param FfsData

    @retval EFI_STATUS
**/
EFI_STATUS FindMeCapHdrFfs(
    IN  VOID    *Capsule,
    OUT UINT8 **FfsData
)
{
    UINT32 Signature;
    UINT32 *SearchPointer;
    AMI_FFS_COMMON_SECTION_HEADER *FileSection;
    APTIO_FW_CAPSULE_HEADER *FwCapHdr;
    static EFI_GUID MEFwCapSectionGuid = AMI_ME_FW_CAPSULE_SECTION_GUID;

    SearchPointer = (UINT32 *)((UINT8 *)Capsule - sizeof(AMI_FFS_COMMON_SECTION_HEADER) + GetFlashCapacity()/*FWCAPSULE_MAX_PAYLOAD_SIZE*/);
    Signature = gMeFwCapFfsGuid.Data1;
    do {
        if(*SearchPointer == Signature) {
            FileSection = (AMI_FFS_COMMON_SECTION_HEADER *)SearchPointer;
            if(!guidcmp(&gMeFwCapFfsGuid, &(FileSection->FfsHdr.Name))
               && !guidcmp(&MEFwCapSectionGuid, &(FileSection->SectionGuid))
            ){
                FwCapHdr = (APTIO_FW_CAPSULE_HEADER*)(FileSection->FwCapHdr);
                // just a sanity check - Cap Size must match the Section size
                if(((*(UINT32 *)FileSection->FfsHdr.Size) & 0xffffff) >=
                        FwCapHdr->CapHdr.HeaderSize + sizeof(AMI_FFS_COMMON_SECTION_HEADER) &&
                    !guidcmp((EFI_GUID*)&FwCapHdr->CapHdr.CapsuleGuid, &gFwCapsuleGuid)
                ){
                    *FfsData = (UINT8*)FwCapHdr;
                        return EFI_SUCCESS;
                }
            }
        }
    } while(SearchPointer-- != Capsule);

    return EFI_NOT_FOUND;
}

EFI_STATUS HashFwRomMapImage (
    IN APTIO_FW_CAPSULE_HEADER  *FwCapsuleHdr,
    IN UINT8                    *Payload,
    IN UINTN                     RomSize,
    IN EFI_GUID                 *HashAlgorithm,
    IN OUT UINT8                *HashData
){
    EFI_STATUS  Status = EFI_SUCCESS;
    ROM_AREA    *RomAreaTbl;
    UINTN       *Addr = NULL;
    UINTN       *Len  = NULL;
    UINTN        i, RomMap_size, max_num_elem, num_elem, max_hash_elem;
    RomAreaTbl = (ROM_AREA *)((UINTN)FwCapsuleHdr+FwCapsuleHdr->RomLayoutOffset);

    RomMap_size = FwCapsuleHdr->RomImageOffset-FwCapsuleHdr->RomLayoutOffset;
    max_num_elem = RomMap_size/sizeof(ROM_AREA);

// assume max size of RomMap array = RomMap_size/sizeof(ROM_AREA);
// or better yet ...calculate exact number
    num_elem = 0;
    for (i=0; i < max_num_elem && RomAreaTbl[i].Size != 0; i++ )
    {
        if (RomAreaTbl[i].Attributes & ROM_AREA_FV_SIGNED)
            num_elem++;
    }

    max_num_elem  = i;
    max_hash_elem = num_elem+2; // add 2 extra entries

    Addr = (UINTN*)gHashTbl;
    Len = (UINTN*)((UINTN)gHashTbl + max_hash_elem*sizeof(UINTN));

    num_elem = 0;
    for(i=0; i < max_num_elem && num_elem < max_hash_elem && RomAreaTbl[i].Size != 0; i++)
    {
        if(!(RomAreaTbl[i].Attributes & ROM_AREA_FV_SIGNED)) 
            continue;
    // sanity check for buffer overruns
        if(RomAreaTbl[i].Offset > RomSize || 
           ((UINT64)RomAreaTbl[i].Offset + RomAreaTbl[i].Size) > RomSize )
            return EFI_SECURITY_VIOLATION;
    // RomArea only holds offsets within a payload
        Addr[num_elem] = (UINTN)Payload + RomAreaTbl[i].Offset;
        Len[num_elem] = RomAreaTbl[i].Size;

        num_elem++;

    }
    if(num_elem >= max_hash_elem) return EFI_SECURITY_VIOLATION;

//
//  Hash of Capsule Hdr + FW Certificate Hdr
//
    if(FwCapsuleHdr->CapHdr.Flags & CAPSULE_FLAGS_CAPHDR_IN_SIGNCERT) {
        Addr[num_elem] = (UINTN) FwCapsuleHdr;
        Len[num_elem] = (UINTN)&FwCapsuleHdr->FWCert.SignCert.CertData - (UINTN)FwCapsuleHdr;
        num_elem++;
        if(num_elem >= max_hash_elem) 
            return EFI_SECURITY_VIOLATION;
    }
//
//  Hash of the ROM_MAP table
//
    Addr[num_elem] = (UINTN)RomAreaTbl;
    Len[num_elem] = (i+1)*sizeof(ROM_AREA);
    num_elem++;
 
    Status = gAmiSig->Hash(gAmiSig, HashAlgorithm/*&gEfiHashAlgorithmSha256Guid*/,
                num_elem, (const UINT8**)Addr,  (const UINTN*)Len, HashData );

    return Status;
}

EFI_STATUS VerifyFwCertRsa2048Sha256 (
    IN APTIO_FW_CAPSULE_HEADER  *FwCapsuleHdr,
    IN UINT8                    *Payload,
    IN UINTN                     RomSize
){
    EFI_STATUS              Status;
    CRYPT_HANDLE            HashHndl;
    CRYPT_HANDLE            PubKeyHndl;
    UINT8                   *Signature;
    UINT32                  Flags;
    UINT8                   *Addr;
    UINTN                   Size;
    EFI_CERT_BLOCK_RSA_2048_SHA256* pRootCert;

    Flags = (FwCapsuleHdr->CapHdr.Flags & CAPSULE_FLAGS_RSA_PSS_PADDING_SCHEME)?EFI_CRYPT_RSASSA_PSS:EFI_CRYPT_RSASSA_PKCS1V15;

    HashHndl.AlgGuid = gEfiHashAlgorithmSha256Guid;
    HashHndl.BlobSize =  SHA256_DIGEST_SIZE;
    HashHndl.Blob = gHashDB;

    PubKeyHndl.AlgGuid = gEfiCertRsa2048Guid;
    PubKeyHndl.BlobSize =  DEFAULT_RSA_KEY_MODULUS_LEN;
//
// 1. Compare Capsule's Sign Cert key with Platform Root Key
//
    PubKeyHndl.Blob = (UINT8*)FwCapsuleHdr->FWCert.SignCert.CertData.PublicKey;
    Status = gAmiSig->VerifyKey(gAmiSig, &gPRKeyGuid, &PubKeyHndl);
    //  Skip the RootCert key checking if SignCert Key and PR Key are matching
    DEBUG ((EFI_D_ERROR, "Compare SignCert Key == FW Key : %r\n", Status));
    if(EFI_ERROR(Status)) {
//
// 1.1 Compare Platform Root with Capsule's Key from a Root Key store
//
        for (pRootCert = &FwCapsuleHdr->FWCert.RootCert;
            (UINTN)pRootCert < ((UINT64)(UINTN)&FwCapsuleHdr->FWCert+FwCapsuleHdr->FWCert.SignCert.Hdr.Hdr.dwLength)
                 && pRootCert->PublicKey[0]!=0;
            pRootCert++)
        {
            PubKeyHndl.Blob = pRootCert->PublicKey;
            Status = gAmiSig->VerifyKey(gAmiSig, &gPRKeyGuid, &PubKeyHndl);
DEBUG ((EFI_D_ERROR, "Compare RootCert Key == FW Key : %r (0x%4X, ..)\n", Status, (UINT32)*PubKeyHndl.Blob));
            if (EFI_ERROR(Status)) continue;
//
// 2. Verify RootCert.Signature
//
// 2.1  Compute FWCert.SignCert.PublicKey Hash
            if(FwCapsuleHdr->CapHdr.Flags & CAPSULE_FLAGS_SIGNKEY_IN_ROOTCERT)
            {
                Addr = (UINT8*)&FwCapsuleHdr->FWCert.SignCert.CertData.PublicKey;
                Size = DEFAULT_RSA_KEY_MODULUS_LEN;
            } else
// 2.2  Compute FWCert.SignCert Hash
            {
                Addr = (UINT8*)&FwCapsuleHdr->FWCert.SignCert;
                Size = sizeof(AMI_CERTIFICATE_RSA2048_SHA256);
            }

            Status = gAmiSig->Hash(gAmiSig, &gEfiHashAlgorithmSha256Guid, 1,&Addr,(const UINTN*)&Size, gHashDB);
            if (EFI_ERROR(Status)) break;

            Signature = (void*)pRootCert->Signature;
            Status = gAmiSig->Pkcs1Verify(gAmiSig, &PubKeyHndl, &HashHndl, Signature, DEFAULT_RSA_SIG_LEN, Flags);
DEBUG ((EFI_D_ERROR, "Verify Root Cert : %r\n", Status));
            break;
        }
    }
    if (EFI_ERROR(Status)) return EFI_SECURITY_VIOLATION;
// 3. Verify Signing Cert Signature
//
// 3.1 The Rom image hash is calculated based on info from the Rom Area map
//
    Status = HashFwRomMapImage(FwCapsuleHdr, Payload, RomSize, &gEfiHashAlgorithmSha256Guid, gHashDB);
    if (EFI_ERROR(Status)) return Status;

    Signature = (void*)FwCapsuleHdr->FWCert.SignCert.CertData.Signature;
    PubKeyHndl.Blob = (UINT8*)FwCapsuleHdr->FWCert.SignCert.CertData.PublicKey;
    Status = gAmiSig->Pkcs1Verify(gAmiSig, &PubKeyHndl, &HashHndl, Signature, DEFAULT_RSA_SIG_LEN, Flags);
    DEBUG ((EFI_D_ERROR, "Verify Signing Cert Signature : %r\n", Status));
    return Status;
}

EFI_STATUS VerifyFwCertPkcs7 (
    IN APTIO_FW_CAPSULE_HEADER  *FwCapsuleHdr,
    IN UINT8                    *Payload,
    IN UINTN                     RomSize
){
    EFI_STATUS              Status;
    UINT8                  *Pkcs7Cert, *Digest, *TrustCert;
    UINTN                   Pkcs7Cert_len, DigestLen, CertSize;
    EFI_GUID               *HashAlgorithm;
    UINT8                   DigestAlgo;
    UINT8                  *DigestAlgoPoint;
    UINTN                   Size;

    EFI_CERT_X509_SHA256    HashCert;

//
// 1. Verify Platform Key algo matches x509 cert or Sha256 (possibly a hash of x509 cert)
//
    if(!guidcmp(&gpPubKeyHndl.AlgGuid, &gEfiCertX509Guid)) {
        TrustCert = gpPubKeyHndl.Blob;
        CertSize  = gpPubKeyHndl.BlobSize;
    } else
    if(!guidcmp(&gpPubKeyHndl.AlgGuid, &gEfiCertSha256Guid)) {
        MemSet((UINT8*)&HashCert, sizeof(HashCert), 0);
        MemCpy(HashCert.ToBeSignedHash, gpPubKeyHndl.Blob, SHA256_DIGEST_SIZE);
        TrustCert = (UINT8*)&HashCert;
        CertSize = sizeof(EFI_CERT_X509_SHA256);
    } else
        return EFI_UNSUPPORTED;

  Digest = &gHashDB[0];
  Pkcs7Cert = (UINT8*)&FwCapsuleHdr->FWCert.SignCert.CertData;
  Pkcs7Cert_len = FwCapsuleHdr->FWCert.SignCert.Hdr.Hdr.dwLength-sizeof(WIN_CERTIFICATE_UEFI_GUID_1);

  // get Digest Algorithm
  Size         = 0;
  DigestAlgoPoint  = (UINT8*)&DigestAlgo;
  Status = gAmiSig->Pkcs7Verify( gAmiSig,
                      Pkcs7Cert, Pkcs7Cert_len,
                      NULL,
                      0,
                      &DigestAlgoPoint,
                      &Size,
                      Pkcs7GetDigestAlgorithm,
                      LOCK
                          );
  if (EFI_ERROR(Status))
    return Status;

  switch(DigestAlgo) {
      case SHA256:
          HashAlgorithm = &gEfiHashAlgorithmSha256Guid;
          DigestLen = SHA256_DIGEST_SIZE;
      break;
      case SHA384:
          HashAlgorithm = &gEfiHashAlgorithmSha384Guid;
          DigestLen = SHA384_DIGEST_SIZE;
      break;
      case SHA512:
          HashAlgorithm = &gEfiHashAlgorithmSha512Guid;
          DigestLen = SHA512_DIGEST_SIZE;
      break;
      default:
          return EFI_UNSUPPORTED;
  }

// 2. Verify Signing Cert Signature
//
// 2.1 The Rom image hash is calculated based on info from the Rom Area map
//
    Status = HashFwRomMapImage(FwCapsuleHdr, Payload, RomSize, HashAlgorithm, gHashDB);
    if (EFI_ERROR(Status)) return Status;

// 2.2 Verify Fw Certificate
    Status = gAmiSig->Pkcs7Verify( gAmiSig,
                     Pkcs7Cert, Pkcs7Cert_len,           // Pkcs7Cert
                     TrustCert, CertSize,
                     &Digest, &DigestLen,               // In/OutData
                     Pkcs7CertValidate,
                     RELEASE                             // Flags, mutex
                     );

    return Status;
}
#endif
/**
    Handle ME Process

    @param MEProcessStructPtr

    @retval VOID

**/
VOID
MEProcessHandler(
    IN  OUT OFBD_TC_55_ME_PROCESS_STRUCT    **MEProcessStructPtr
)
{
    EFI_STATUS                  Status;
    static UINTN                NumberOfPages;
    static UINT32               SizeCopied;
    static BOOLEAN              UseSmmMem = FALSE;
    static EFI_PHYSICAL_ADDRESS SMM_Address;
    static BOOLEAN              HeciIsHide;
    static UINT8                LastRequest;
    UINT8                       Request;
    UINT16                      MeDataCheckSum = 0;
#if defined(NonBIOS_Validation) && (NonBIOS_Validation == 1) 
    APTIO_FW_CAPSULE_HEADER     *FwCapsuleHdr;
    UINT8                       *Payload;
    UINTN                       RomSize;
    UINTN                       BlockSize;
    UINT8                       *BlockAddr;
    static UINTN                HashBlock = 0;
#endif
#if defined(OEM_REGION_ORDER) && (OEM_REGION_ORDER == 1)                
            UINT8   Init;
            UINT8   *InitAddress;
#endif            

    StructPtr = *MEProcessStructPtr;

    if( IsRetryStep( StructPtr ) == FALSE )
    {
        //If this is not retry step, save the request
        Request = (StructPtr->bHandleRequest & (~(UINT8)BIT06));
        LastRequest = Request;
    }
    else
    {
        Request = LastRequest;
    }

    switch( Request )
    {
        // Allocate Buffer
        case 1:
#if PFR_AFU_UPDATE_SUPPORT
            //If PFR state is enable, return error for backward compatible.
            //We use 0x11 instead of 0x01 to locate buffer
            if(gPfrSupport)
            {
                MeProcessHandleResult( BIT02, "Failed when Memory Allocated!!, region can't be updated since PFR state is enable" );
            }
#endif
            HeciIsHide = UnHideHeci();

//If region is behind BIOS, we would use the SPI offset instead of mapping address.
//So we need to use mapping address to initial FlashLib than SPI offset will be workable in the FlashLib.
#if defined(OEM_REGION_ORDER) && (OEM_REGION_ORDER == 1)                
            InitAddress = (UINT8*)FLASH_BASE_ADDRESS(0x0);
            AmiFlashReadEx( InitAddress, &Init, 1, FALSE );
#endif  
            // If Non-BIOS(ME/GbE/DER/PDR) region can't be identified 
            // by FDOC, the TotalBlocks could be less than Non-BIOS Region,
            // then, the SMM memroy will get destroyed when AFU upload
            // Non-BIOS region data to SMM. So, below change to use Non-BIOS 
            // region size instead of TotalBlocks size for allocating memory
            // to avoid such situation.
            if(!gImageBuffer)   //Ensure the buffer is NULL or freed first to avoid SMI vulnerability
            {
#if defined(OEM_REGION_ORDER) && (OEM_REGION_ORDER == 1)                
                NumberOfPages = EFI_SIZE_TO_PAGES(GetFlashCapacity());
#else                     
               NumberOfPages = EFI_SIZE_TO_PAGES(GetFlashCapacity() - FLASH_SIZE);
#endif                      
               Status = pSmst->SmmAllocatePages(
                                   AllocateAnyPages,
                                   EfiRuntimeServicesData,
                                   NumberOfPages,
                                   &SMM_Address );
               if( !EFI_ERROR(Status) )
               {
                   UseSmmMem = TRUE;
                   gImageBuffer = SMM_Address;
               }
               else
               {
                   gImageBuffer = RomBuffer;
               }
            }
            // No memory allocated
            if( !gImageBuffer )
                MeProcessHandleResult(
                    (BIT00 | BIT02),
                    "Error : No Memory Allocated!!" );

            SizeCopied = 0;
#if PFR_AFU_UPDATE_SUPPORT
            gImageBufferSize = 0;
            gPfrCommand = 0;
#endif
        break;

#if defined(SPS_MODULE_SUPPORT_ME) && (SPS_MODULE_SUPPORT_ME == 1)
#if defined(SecSMIFlash_SUPPORT) && (SecSMIFlash_SUPPORT == 1)
        // ME FW Capsule Update functions.
        case 3: // ME FW Capsule Update. (ME FW only)
        case 5: // ME FW Capsule Update. (BIOS + ME FW)
            if( !gFwCapsuleAddress )
            {
                MeProcessHandleResult( (BIT00 | BIT02), "Error : Functon Not Supported!!" );
                break;
            }
            // Initial local variables.
            gImageBuffer = 0;
            SizeCopied = 0;
            if( (StructPtr->bHandleRequest == 3) && \
                (StructPtr->TotalBlocks <= EFI_SIZE_TO_PAGES(FWCAPSULE_IMAGE_SIZE)) )
            {
                // Function#3 : ME FW Update only, upload ME FW capsule from the beggining.
                gImageBuffer = (EFI_PHYSICAL_ADDRESS)gFwCapsuleAddress;
            }
            if( (StructPtr->bHandleRequest == 5) && \
                (StructPtr->TotalBlocks <= \
                        EFI_SIZE_TO_PAGES(FWCAPSULE_IMAGE_SIZE - FLASH_SIZE)) )
            {
                // Function#5 : BIOS + ME FW Update, upload capsule from the end of BIOS.
#if defined(FWCAPSULE_FILE_FORMAT) && (FWCAPSULE_FILE_FORMAT == 1)
                gImageBuffer = (EFI_PHYSICAL_ADDRESS)gFwCapsuleAddress + FWCAPSULE_MAX_HDR_SIZE + FLASH_SIZE;
#else
                gImageBuffer = (EFI_PHYSICAL_ADDRESS)gFwCapsuleAddress + FLASH_SIZE;
#endif
            }
            if( !gImageBuffer ) MeProcessHandleResult( (BIT00 | BIT02), "Error : Buffer Too Small!!" );
            break;
#endif
#endif

        // Recieve Data from AFU
        case 2:
            if( IsRetryStep( StructPtr ) == TRUE )
            {
                //If this is retry step, copy to the last offest
                SizeCopied -= StructPtr->ddMeDataSize;
#if defined(NonBIOS_Validation) && (NonBIOS_Validation == 1)
                HashBlock -= 1;
#endif
            }

            Status = EFI_OUT_OF_RESOURCES;

#if defined(OEM_REGION_ORDER) && (OEM_REGION_ORDER == 1)
            if( (SizeCopied + StructPtr->ddMeDataSize) <= GetFlashCapacity()) //Ensure SizeCopied + StructPtr->ddMeDataSize remains within the allocated buffer
#else                    
            if( (SizeCopied + StructPtr->ddMeDataSize) <= (GetFlashCapacity() - FLASH_SIZE) ) //Ensure SizeCopied + StructPtr->ddMeDataSize remains within the allocated buffer
#endif                    
            Status = AmiValidateMemoryBuffer( (VOID*)(gImageBuffer + SizeCopied), StructPtr->ddMeDataSize );

            if( EFI_ERROR(Status) )
            {
                MeProcessHandleResult( (BIT00 | BIT02), "Invalid memory usage" );
                break;
            }

            Status = AmiValidateMemoryBuffer( (VOID*)(UINTN)(StructPtr->ddMeDataBuffer), StructPtr->ddMeDataSize );
            if( EFI_ERROR(Status) )
            {
                MeProcessHandleResult( (BIT00 | BIT02), "Invalid memory usage" );
                break;
            }

            MeDataCheckSum = CalculateSum16((UINT16*)(UINTN)(StructPtr->ddMeDataBuffer), StructPtr->ddMeDataSize);
            if(MeDataCheckSum != StructPtr->ddMeDataChecksum)
            {
                MeProcessHandleResult( (BIT00 | BIT02), "Data CheckSum is invalid" );
                break;
            }

            MemCpy(
                (UINT8*)(gImageBuffer + SizeCopied),
                (UINT8*)(UINTN)StructPtr->ddMeDataBuffer,
                StructPtr->ddMeDataSize );

#if defined(NonBIOS_Validation) && (NonBIOS_Validation == 1)
            // Fill in Hash Table
            BlockSize = StructPtr->ddMeDataSize;
            BlockAddr = (UINT8*)(gImageBuffer + SizeCopied);
            Status = gAmiSig->Hash(gAmiSig, &gEfiHashAlgorithmSha256Guid,
                    1, &BlockAddr, (const UINTN*)&BlockSize, gHashTbl[HashBlock]);
            if (EFI_ERROR(Status))
            {
                MeProcessHandleResult( (BIT00 | BIT02), "Error: Fill Hash Table error" );
            }
            HashBlock += 1;
#endif
            SizeCopied += StructPtr->ddMeDataSize;
#if PFR_AFU_UPDATE_SUPPORT
            gImageBufferSize = SizeCopied;
#endif
        break;

        // Update
        case 4:
            UpdateRegions( (UINT8*)gImageBuffer, TRUE );
        break;

        // Continue....
        case 8:
            UpdateRegions( (UINT8*)gImageBuffer, TRUE );
        break;
#if PFR_AFU_UPDATE_SUPPORT        
//HadleRequests for the PFR feature
#if defined(Pfr_SUPPORT) && (Pfr_SUPPORT == 1)
                // Allocate Buffer
                case 0x11:
                    HeciIsHide = UnHideHeci();

                    if(!gImageBuffer)   //Ensure the buffer is NULL or freed first to avoid SMI vulnerability
                    {
                        gImageBuffer = RomBuffer;
                    }
                    // No memory allocated
                    if( !gImageBuffer )
                        MeProcessHandleResult(
                            (BIT00 | BIT02),
                            "Error : No Memory Allocated!!" );

                    SizeCopied = 0;
                    gImageBufferSize = 0;
                    gPfrCommand = 0;
                break;

                //Preserve 0x12 and 0x14 for future usage
                case 0x12:
                    break;

                case 0x14:
                    break;

                //Receive PFR command which use input
                case 0x18:
                    gPfrCommand = (UINT8)(StructPtr->ddMeDataBuffer);
                    //If Pfr command = 0, we don't need to signal CPLD
                    if(gPfrCommand != 0 )
                    {
                        Status = SignalCpldPfrUpdate(gPfrCommand);
                        if(EFI_ERROR(Status))
                            MeProcessHandleResult( (BIT02), "Error when Signaling CPLD\n");
                    }
                    break;
#endif
#endif
#if defined(NonBIOS_Validation) && (NonBIOS_Validation == 1) 
        case 0x80:
            if(!WholeImage)
                MeProcessHandleResult(
                    (BIT00 | BIT02),
                    "Error : No Memory Allocated!!" );

            SizeCopied = 0;

        break;

        // Recieve whole Data from AFU for non-BIOS regions validation
        case 0x81:
            if( IsRetryStep( StructPtr ) == TRUE )
            {
                //If this is retry step, copy to the last offest
                SizeCopied -= StructPtr->ddMeDataSize;
            }

            Status = EFI_OUT_OF_RESOURCES;

            if( SizeCopied < GetFlashCapacity() ) //Ensure SizeCopied remains within the allocated buffer
            Status = AmiValidateMemoryBuffer( (VOID*)(WholeImage + SizeCopied), StructPtr->ddMeDataSize );

            if( EFI_ERROR(Status) )
            {
                MeProcessHandleResult( (BIT00 | BIT02), "Invalid memory usage" );
                break;
            }

            Status = AmiValidateMemoryBuffer( (VOID*)(UINTN)(StructPtr->ddMeDataBuffer), StructPtr->ddMeDataSize );
            if( EFI_ERROR(Status) )
            {
                MeProcessHandleResult( (BIT00 | BIT02), "Invalid memory usage" );
                break;
            }

            MemCpy(
                (UINT8*)(WholeImage + SizeCopied),
                (UINT8*)(UINTN)StructPtr->ddMeDataBuffer,
                StructPtr->ddMeDataSize );
            SizeCopied += StructPtr->ddMeDataSize;
        break;

        case 0x82: //valid non-BIOS regions
            FwCapsuleHdr = (APTIO_FW_CAPSULE_HEADER*)WholeImage;
            Payload = (UINT8*)WholeImage;

            Status = FindMeCapHdrFfs(Payload, (UINT8**)&FwCapsuleHdr);
            if(EFI_ERROR(Status))
            {
                MeProcessHandleResult((BIT00 | BIT02),"Error : Can't find ME Hdr" );
                return;
            }

            RomSize = FwCapsuleHdr->CapHdr.CapsuleImageSize - FwCapsuleHdr->RomImageOffset;

            // Aptio FW Capsule only supporting WIN_CERT_TYPE_EFI_GUID
            // sanity check for buffer overruns
            if((RomSize > (GetFlashCapacity()+ 0x4000)) ||
               (FwCapsuleHdr->FWCert.SignCert.Hdr.Hdr.wCertificateType != WIN_CERT_TYPE_EFI_GUID) ||
               (FwCapsuleHdr->CapHdr.CapsuleImageSize > (GetFlashCapacity()+ 0x4000)) ||
               (FwCapsuleHdr->CapHdr.HeaderSize > FwCapsuleHdr->CapHdr.CapsuleImageSize) ||
               (FwCapsuleHdr->CapHdr.HeaderSize > FwCapsuleHdr->RomImageOffset) ||
               (FwCapsuleHdr->RomImageOffset > 0x4000) || // 16k is a MAX possible FwCap Hdr size
               (FwCapsuleHdr->RomLayoutOffset > FwCapsuleHdr->RomImageOffset) ||
               ((UINT64)FwCapsuleHdr->FWCert.SignCert.Hdr.Hdr.dwLength + offsetof(APTIO_FW_CAPSULE_HEADER, FWCert) >
                 FwCapsuleHdr->RomLayoutOffset)
              )
            {
                MeProcessHandleResult( (BIT00 | BIT02), "Error: CapsuleHdr have problem!!" );
                return;
            }

            if(gAmiSig == NULL)
            {
                MeProcessHandleResult( (BIT00 | BIT02), "Error: Can't Begin Authentication!!" );
                return;
            }

            gpPubKeyHndl.Blob = NULL;
            gpPubKeyHndl.BlobSize = 0;
            Status = gAmiSig->GetKey(gAmiSig, &gpPubKeyHndl, &gPRKeyGuid, gpPubKeyHndl.BlobSize, 0);
        DEBUG ((EFI_D_ERROR,"GetKey %r (%X, 0x%lx bytes)\n",Status, gpPubKeyHndl.Blob, gpPubKeyHndl.BlobSize));
            if ((EFI_ERROR(Status) && Status != EFI_BUFFER_TOO_SMALL)|| gpPubKeyHndl.Blob == NULL) 
            {
                MeProcessHandleResult( (BIT00 | BIT02), "Error: Can't Begin Authentication!!" );
                return;
            }

            if(EFI_ERROR(AmiValidateSmramBuffer(gpPubKeyHndl.Blob, gpPubKeyHndl.BlobSize)))
            {
                MeProcessHandleResult( (BIT00 | BIT02), "Error: Can't Begin Authentication!!" );
                return;
            }

            if(*((UINT32*)(gpPubKeyHndl.Blob)) == *(UINT32*)FwBadKey) {
                MeProcessHandleResult( (BIT00 | BIT02), "WARNING! Dummy FW Key detected\n");
                return;
            }

            // Begin Authentication
                if(!guidcmp((EFI_GUID*)&FwCapsuleHdr->FWCert.SignCert.Hdr.CertType, &gEfiCertPkcs7Guid))
                    Status = VerifyFwCertPkcs7(FwCapsuleHdr, Payload, RomSize);
                else
                    Status = VerifyFwCertRsa2048Sha256(FwCapsuleHdr, Payload, RomSize);
                DEBUG ((EFI_D_ERROR, "Signature validate %r\n", Status));
                if (EFI_ERROR(Status))
                {
                    MeProcessHandleResult( (BIT00 | BIT02), "Error: Signature validate failed" );
                }

                NonBiosValidation = TRUE;
        break;
#endif
        // Free Buffer
        case 0x10:
#if defined(NonBIOS_Validation) && (NonBIOS_Validation == 1)
            NonBiosValidation = FALSE;
#endif
            if( UseSmmMem )
            {
                pSmst->SmmFreePages( gImageBuffer, NumberOfPages );
                gImageBuffer = 0;
                SMM_Address = 0;
            }
            HideHeci( HeciIsHide );
        break;
    }
}
/**
    CspMeudReportFirmwareType reports AFU on-board firmware version, and trigger
    AFU new process flow by setting ReturnStatus with 1.

    @param MeVersionStructPtr

    @retval VOID

**/
VOID
CspMeudReportFirmwareType(
    IN  OFBD_TC_55_ME_VERSION_STRUCT    *MeVersionStructPtr
)
{
    MeVersionStructPtr->FwType = MEUD_FIRMWARE_SPS;
#if defined(OEM_REGION_ORDER) && (OEM_REGION_ORDER == 1)
    MeVersionStructPtr->FwType = MEUD_FIRMWARE_OEM; //Ask Afu tool to upload entire binary
#endif     
    MeVersionStructPtr->MeudVersion = Meud_Module_Version;
    MeVersionStructPtr->MeStructVersion = 1;
    //Return 0x01 to indicate AFU use new process
    MeVersionStructPtr->ReturnStatus = 0x01;
#if PFR_AFU_UPDATE_SUPPORT
    if(gPfrSupport)
    {
        MeVersionStructPtr->FwType = MEUD_FIRMWARE_SPS_PFR;
    }
    else
    {
#if defined(OEM_REGION_ORDER) && (OEM_REGION_ORDER == 1)
        MeVersionStructPtr->FwType = MEUD_FIRMWARE_OEM; //Once return this type, Afu tool will upload entire binary
#else
        MeVersionStructPtr->FwType = MEUD_FIRMWARE_SPS;
#endif
    }
#endif
    return;
}
