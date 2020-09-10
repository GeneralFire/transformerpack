//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file NbSetup.c
    Chipset and project related informations updated and displayed in Setup.

**/

#include <Token.h>
#include <Setup.h>
#include <AmiDxeLib.h>
#include <Guid/MemoryMapData.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PchInfoLib.h>
#include <Library/IoLib.h>
#include <Library/MmPciBaseLib.h>
#include <Protocol/PlatformType.h>
#include <Protocol/IioUds.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/MemoryAllocationLib.h>
#include <Nb.h>
#include <Library/MemMapDataLib.h>
#include <SysHostChipCommon.h>
#include <Register/Cpuid.h>
#include "UncoreCommonIncludes.h"
#include <Cpu/CpuIds.h>
#include <Library/MemTypeLib.h>

EFI_GUID gAmiLtsxSinitAcmGuid = AMI_LTSX_SINIT_GUID;

/**

  Converts an ascii string to unicode string 16 chars at a time.

  @param AsciiString    -  Address of Ascii string.
  @param UnicodeString  -  Address of Unicode String.

  @retval None.

**/
VOID
AsciiToUnicode (
  IN CHAR8     *AsciiString,
  IN CHAR16    *UnicodeString
  )
{

    UINT8 Index;

    Index = 0;
    while (AsciiString[Index] != 0) {
        UnicodeString[Index] = (CHAR16) AsciiString[Index];
        Index++;
    }
    UnicodeString[Index] = 0;
}

/**
    Display Board information.

    @param HiiHandle - Handle number

    @return VOID

**/

VOID
InitBoardInfoStrings (
  IN EFI_HII_HANDLE  HiiHandle
)
{
    EFI_STATUS                   Status;
    EFI_PLATFORM_TYPE_PROTOCOL   *PlatformType;
    UINT32                       RegEax;
    UINT32                       CpuidSignature;
    CHAR8                        Buffer1[32];
    CHAR16                       UniBuffer1[32];
    CHAR8                        Buffer2[32];
    CHAR16                       UniBuffer2[32];
    UINT32                       BufferSize;

    Status = gBS->LocateProtocol (
                    &gEfiPlatformTypeProtocolGuid,
                    NULL,
                    &PlatformType
                    );
    if (EFI_ERROR(Status)) {
        return;
    }

    //
    // CPU ID Information
    //
    AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
    CpuidSignature = RegEax;

    //
    // Update Platform type
    //
    InitString(
        HiiHandle,
        STRING_TOKEN(STR_PLATFORM_INFO_VALUE),
        L"%s",
        (CHAR16 *)PlatformType->TypeStringPtr
        );


    BufferSize = sizeof (Buffer1);
    PchGetSteppingStr (Buffer1, BufferSize);
    AsciiToUnicode (Buffer1, UniBuffer1);

    BufferSize = sizeof (Buffer2);
    AsciiStrCpyS (Buffer2, BufferSize, PchGetSkuStr ());
    AsciiToUnicode (Buffer2, UniBuffer2);

    //
    // Update PCH type value
    //
    InitString(
            HiiHandle,
            STRING_TOKEN(STR_PCH_INFO_VALUE),
            L"%s - %s",
            UniBuffer2,
            UniBuffer1
            );

    switch (CpuidSignature) {
        case ((CPU_FAMILY_SKX << 4) + SKX_A0_CPU_STEP):
          InitString(HiiHandle, STRING_TOKEN(STR_PROCESSOR_INFO_VALUE), L"%5X - SKX A0", RegEax );         
          break;
        case ((CPU_FAMILY_SKX << 4) + SKX_A2_CPU_STEP):
          InitString(HiiHandle, STRING_TOKEN(STR_PROCESSOR_INFO_VALUE), L"%5X - SKX A2", RegEax );
          break;
        case ((CPU_FAMILY_SKX << 4) + SKX_B0_CPU_STEP):
          InitString(HiiHandle, STRING_TOKEN(STR_PROCESSOR_INFO_VALUE), L"%5X - SKX B0", RegEax );
          break;
        case ((CPU_FAMILY_SKX << 4) + SKX_B1_CPU_STEP):
          InitString(HiiHandle, STRING_TOKEN(STR_PROCESSOR_INFO_VALUE), L"%5X - SKX B1", RegEax );
          break;
        case ((CPU_FAMILY_SKX << 4) + SKX_H0_CPU_STEP):
          InitString(HiiHandle, STRING_TOKEN(STR_PROCESSOR_INFO_VALUE), L"%5X - SKX H0", RegEax );
          break;
        case ((CPU_FAMILY_SKX << 4) + CLX_A0_CPU_STEP):
          InitString(HiiHandle, STRING_TOKEN(STR_PROCESSOR_INFO_VALUE), L"%5X - CLX A0", RegEax );
          break;
        case ((CPU_FAMILY_SKX << 4) + CLX_B0_CPU_STEP):
          InitString(HiiHandle, STRING_TOKEN(STR_PROCESSOR_INFO_VALUE), L"%5X - CLX B0", RegEax );
          break;
//        case ((CPU_FAMILY_ICX << 4) + ICXSP_A0_CPU_STEP):
//          InitString(HiiHandle, STRING_TOKEN(STR_PROCESSOR_INFO_VALUE), L"%5X - ICX A0", RegEax );
//          break;
        case ((CPU_FAMILY_ICXD << 4) + ICXD_A0_CPU_STEP):
          InitString(HiiHandle, STRING_TOKEN(STR_PROCESSOR_INFO_VALUE), L"%5X - ICX-D A0", RegEax );
          break;
        default:
          InitString(HiiHandle, STRING_TOKEN(STR_PROCESSOR_INFO_VALUE), L"%5X - Unknown", RegEax );
    }
    return;
}

/**
    Display Reference code revision information. 

    @param HiiHandle - Handle number
    @param Class     - Form Class number

    @return VOID

**/

VOID
InitRefCodeVersionStrings (
  IN EFI_HII_HANDLE      HiiHandle
)
{

//    EFI_STATUS                     Status;
//    EFI_IIO_UDS_PROTOCOL           *IioUds;

/*    Status = gBS->LocateProtocol (
                    &gEfiIioUdsProtocolGuid,
                    NULL,
                    &IioUds
                    );
    if (EFI_ERROR(Status)) {
        return;
    }
*/
    InitString(
            HiiHandle,
            STRING_TOKEN(STR_REFCODE_REVISION_VALUE),
            RC_REVISION_VALUE // Hard coding SDL value need to check if any alternate available
/*            (IioUds->IioUdsPtr->SystemStatus.RcVersion & 0xFF000000) >> 24,
            (IioUds->IioUdsPtr->SystemStatus.RcVersion & 0xFF0000) >> 16,
            (IioUds->IioUdsPtr->SystemStatus.RcVersion & 0xFF00) >> 8*/
    );

    return;
}

//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   FindEntryInFit
//
// Description: FitTable address will be filled for the specified FIT type.
//
//
// Input:       IN     UINT8    FitType
//              IN OUT VOID     **FitTable
//
// Output:      EFI STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS 
FindEntryInFit (
    IN  UINT8    FitType,
    IN  VOID     **FitTable )
{
    FIT_TABLE_ENTRY    *FitEntry;
    UINT32             FitEntryCount;
    UINT32             Index;
// APTIOV_SERVER_OVERRIDE_START : Coverity Fix
    FitEntry = (FIT_TABLE_ENTRY*) (UINTN) MmioRead32 (FIT_TABLE_ENTRY_POINTER);
// APTIOV_SERVER_OVERRIDE_END : Coverity Fix
    if (((UINTN)FitEntry == 0xFFFFFFFF) || (FitEntry == NULL)) {
        DEBUG(( DEBUG_ERROR, "FitEntryPointer (%lx) is Invalid\n", FitEntry));
        return EFI_NOT_FOUND;
    }

    FitEntryCount = FitEntry->TblSize;
    
    for (Index = 1; Index <= FitEntryCount; Index++, FitEntry++) {
        if (FitEntry->TblType == FitType) {
            *FitTable = (VOID*)(UINTN)FitEntry->TblAddress;;
            return EFI_SUCCESS;
        }
    }

    DEBUG(( DEBUG_ERROR, "Requested FIT Table Not Found\n"));

    return EFI_NOT_FOUND;
}

//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   GetRawImage
//
// Description: Loads binary from RAW section of main firmware volume
//
//
// Input:       IN     EFI_GUID   *NameGuid,
//              IN OUT VOID       **Buffer,
//              IN OUT UINTN      *Size
//
// Output:      EFI STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS 
GetRawImage (
    IN     EFI_GUID  *NameGuid,
    IN OUT VOID      **Buffer,
    IN OUT UINTN     *Size )
{
    EFI_STATUS                      Status;
    EFI_HANDLE                      *HandleBuffer = 0;
    UINTN                           HandleCount   = 0;
    UINTN                           Index;
    EFI_FIRMWARE_VOLUME2_PROTOCOL   *Fv;
    UINT32                          AuthenticationStatus;
 
    Status = gBS->LocateHandleBuffer(
                    ByProtocol,
                    &gEfiFirmwareVolume2ProtocolGuid,
                    NULL,
                    &HandleCount,
                    &HandleBuffer);
    
    if (EFI_ERROR(Status) || (HandleCount == 0)) {
        return EFI_NOT_FOUND;
    }

    // Find desired image in all Fv's
    for (Index = 0; Index < HandleCount; Index++) {

        Status = gBS->HandleProtocol(
                        HandleBuffer[Index],
                        &gEfiFirmwareVolume2ProtocolGuid,
                        &Fv);

        if (EFI_ERROR(Status)) {
            FreePool( HandleBuffer );
            return EFI_LOAD_ERROR;
        }

        // Try a raw file
        *Buffer = NULL;
        *Size = 0;
        Status  = Fv->ReadSection(
                        Fv,
                        NameGuid,
                        EFI_SECTION_RAW,
                        0,
                        Buffer,
                        Size,
                        &AuthenticationStatus);

        if (!EFI_ERROR( Status)) {
            break;
        } 
    }

    FreePool( HandleBuffer );

    if ( Index >= HandleCount ) {
        return EFI_NOT_FOUND;
    }

    return EFI_SUCCESS;
}

//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   InitAcmRevision
//
// Description: Display BIOS and SINIT ACM version in main page.
//
//
// Input:       IN     EFI_HII_HANDLE      HiiHandle
//
// Output:      None
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************

VOID
InitAcmRevision (
  IN EFI_HII_HANDLE      HiiHandle
)
{
    EFI_STATUS   Status = EFI_SUCCESS;
    UINT8        *AcmTable;
    UINT8        *ScratchSize;
    UINT8        *UserArea;
    UINT8        *AcmVersion;
    UINT8        *SinitAcmVersion;
    UINT8        *SinitAddress;
    UINT16        AcmHeader;
    UINTN        SinitSize;

    // Procedure to get BIOS ACM version
    DEBUG(( DEBUG_INFO, "%a Entry point \n", __FUNCTION__));

    Status = FindEntryInFit (FIT_ACM_TYPE, &AcmTable);

    if (EFI_ERROR(Status)) {
        DEBUG(( DEBUG_ERROR, "FIT entry not found %r \n", Status ));
        return;
    }
    
    // Getting the ScratchSize address of the BIOS ACM
    ScratchSize = AcmTable + SCRATCHSIZE_OFFSET;
    
    //Acm Architectural Header is different in Purley and Whitley
        AcmHeader = 0x380;
    
    // User Area presents in offset (644 + ScratchSize*4)
    UserArea = AcmTable + AcmHeader + ((*ScratchSize)*4);
    
    // ACM version found in the three bytes at offset 37.
    // 3 offsets represents <major>:<minor>.<build>
    AcmVersion = UserArea + 0x25;
    
    DEBUG(( DEBUG_INFO, "BIOS ACM version %x.%x.%x \n", *AcmVersion, *(AcmVersion+1), *(AcmVersion+2) ));
    
    // Update BIOS ACM version
    InitString(
            HiiHandle,
            STRING_TOKEN(STR_BIOS_ACM_VALUE),
            L"%x.%x.%x",
            *AcmVersion,
            *(AcmVersion+1),
            *(AcmVersion+2)
            );

    // Procedure to get SINIT ACM version
    Status = GetRawImage (&gAmiLtsxSinitAcmGuid, &SinitAddress, &SinitSize);
    
    if (EFI_ERROR(Status)) {
        DEBUG(( DEBUG_ERROR, "SINIT Address not found %r \n", Status ));
        return;
    }
    
    // Getting the ScratchSize address of the SINIT ACM
    ScratchSize = SinitAddress + SCRATCHSIZE_OFFSET;
    
    // User Area presents in offset (644 + ScratchSize*4)
    UserArea = SinitAddress + AcmHeader + ((*ScratchSize)*4);
    
    // ACM version found in the three bytes at offset 37.
    // 3 offsets represents <major>:<minor>.<build>
    SinitAcmVersion = UserArea + 0x25;
    
    DEBUG(( DEBUG_INFO, "SINIT ACM version %x.%x.%x \n", *SinitAcmVersion, *(SinitAcmVersion+1), *(SinitAcmVersion+2) ));

    // Update SINIT ACM version
    InitString(
            HiiHandle,
            STRING_TOKEN(STR_SINIT_ACM_VALUE),
            L"%x.%x.%x",
            *SinitAcmVersion,
            *(SinitAcmVersion+1),
            *(SinitAcmVersion+2)
            );
    
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   InitNbStrings
//
// Description: Chipset and project related informations updated
//
// Input:       HiiHandle - Handle to HII database
//              Class - Indicates the setup class
//
// Output:      None
//
// Notes:       PORTING REQUIRED
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID
InitNbStrings(
    IN EFI_HII_HANDLE      HiiHandle,
    IN UINT16              Class
)
{
    struct SystemMemoryMapHob       *SystemMemoryMap;
    UINT64                          MemorySize = 0;
    UINT32                          Index;

    if(Class == MAIN_FORM_SET_CLASS) {

        SystemMemoryMap = GetSystemMemoryMapData ();

        for (Index = 0; Index < SystemMemoryMap->numberEntries; Index++) {
            if (IsMemTypeReserved (SystemMemoryMap->Element[Index].Type)) {
              continue;
            }
            /*
            DEBUG(( DEBUG_INFO, "InitNbStrings:: Type = %x; NodeId = %x; SocketId = %x; SktInterBitmap = %x; ImcInterBitmap= %x; BaseAddress[%d] = %d; ElementSize[%d] = %d\n", \
                    SystemMemoryMap->Element[Index].Type, \
                    SystemMemoryMap->Element[Index].NodeId, \
                    SystemMemoryMap->Element[Index].SocketId, \
                    SystemMemoryMap->Element[Index].SktInterBitmap, \
                    SystemMemoryMap->Element[Index].ImcInterBitmap, \
                    Index, \
                    LShiftU64 (SystemMemoryMap->Element[Index].BaseAddress, 6),\
                    Index, \
                    LShiftU64 (SystemMemoryMap->Element[Index].ElementSize, 6)));
			*/
            MemorySize += LShiftU64 (SystemMemoryMap->Element[Index].ElementSize, 6);
        }
        //DEBUG(( DEBUG_ERROR, "InitNbStrings:: MemorySize = %d\n", MemorySize));

        InitString(
                HiiHandle,
                STRING_TOKEN(STR_MEMORY_SIZE_VALUE),
                L"%4d MB",
                MemorySize
                );
        
        InitBoardInfoStrings (HiiHandle);
        
        InitRefCodeVersionStrings (HiiHandle);
        
        InitAcmRevision (HiiHandle);

        }
}
