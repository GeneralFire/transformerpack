//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file PlatformPeiSystemInfoCpu.c
     

**/
#include "PlatformPeiSystemInformation.h"

static VALUE_TO_STRING_MAPPING gFamilyIdNameMapping[] = {
                                        {0x06, "Intel Nehalem Family"}
                                        };

static VALUE_TO_STRING_MAPPING gModelIdNameMapping[] = {
                                        {0x8F, "Sappirerapids Server Model"}
                                        };

static VALUE_TO_STRING_MAPPING gSteppingIdNameMappingSpr[] = {
                                        {REV_A0, "SPR A0"},
                                        {REV_A1, "SPR A1"}
                                        };

/**
   Get Brand string

    @param[in] PeiServices   - Pointer to Pei service table
    @param[in] ProcessorNumber

    @retval  Pointer to Brand string
**/
VOID
GetBrandString (
  IN     CONST EFI_PEI_SERVICES        **PeiServices ,
  IN     UINT8                         ProcessorNumber,
  IN OUT CHAR8                         *BrandString
)
{
    EFI_CPUID_REGISTER              CpuidRegisters;
    CHAR8                           *TempStringPtr;
    EFI_STATUS                      Status=EFI_SUCCESS;
       
    TempStringPtr = BrandString;
    
    // Get Brand string
    AsmCpuid (CPUID_BRAND_STRING1, &CpuidRegisters.RegEax, &CpuidRegisters.RegEbx, &CpuidRegisters.RegEcx, &CpuidRegisters.RegEdx);
    if (CpuidRegisters.RegEax != 0) {
        *(UINT32*)TempStringPtr = CpuidRegisters.RegEax; 
        TempStringPtr += 4;
        *(UINT32*)TempStringPtr = CpuidRegisters.RegEbx; 
        TempStringPtr += 4;
        *(UINT32*)TempStringPtr = CpuidRegisters.RegEcx; 
        TempStringPtr += 4;
        *(UINT32*)TempStringPtr = CpuidRegisters.RegEdx; 
        TempStringPtr += 4;
       
        AsmCpuid(CPUID_BRAND_STRING2, &CpuidRegisters.RegEax, &CpuidRegisters.RegEbx, &CpuidRegisters.RegEcx, &CpuidRegisters.RegEdx);
        *(UINT32*)TempStringPtr = CpuidRegisters.RegEax; 
        TempStringPtr += 4;
        *(UINT32*)TempStringPtr = CpuidRegisters.RegEbx; 
        TempStringPtr += 4;
        *(UINT32*)TempStringPtr = CpuidRegisters.RegEcx; 
        TempStringPtr += 4;
        *(UINT32*)TempStringPtr = CpuidRegisters.RegEdx; 
        TempStringPtr += 4;

        AsmCpuid(CPUID_BRAND_STRING3, &CpuidRegisters.RegEax, &CpuidRegisters.RegEbx, &CpuidRegisters.RegEcx, &CpuidRegisters.RegEdx);
        *(UINT32*)TempStringPtr = CpuidRegisters.RegEax; 
        TempStringPtr += 4;
        *(UINT32*)TempStringPtr = CpuidRegisters.RegEbx; 
        TempStringPtr += 4;
        *(UINT32*)TempStringPtr = CpuidRegisters.RegEcx; 
        TempStringPtr += 4;
        *(UINT32*)TempStringPtr = CpuidRegisters.RegEdx; 
        TempStringPtr += 4;
    
        *TempStringPtr = '\0';
 
    // Remove Initial spaces.
        TempStringPtr = BrandString;
    
        while (*TempStringPtr == ' ' && *TempStringPtr !=0) 
            ++TempStringPtr;
    
        if (TempStringPtr != BrandString) {
            INT32  NumToCopy = 49 - (UINT32)(TempStringPtr - BrandString);
            CHAR8 *TempStringPtr2 = BrandString;
            while (NumToCopy--) *TempStringPtr2++ = *TempStringPtr++;
        }
    }
    else {
         AsciiSPrint(BrandString,49,"Unknown");
    }
}

/**
   This function gets the version information of processor,
   including family ID, model ID, stepping ID and processor type.

    @param[in] PeiServices   - Pointer to Pei service table
    @param[in] DisplayedFamily
    @param[in] DisplayedModel
    @param[in] SteppingId
    @param[in] ProcessorType

    @retval  VOID
**/
VOID
EFIAPI
PeiGetProcessorVersionInfo (
  IN  CONST EFI_PEI_SERVICES        **PeiServices ,
  OUT UINT32                        *DisplayedFamily OPTIONAL,
  OUT UINT32                        *DisplayedModel  OPTIONAL,       
  OUT UINT32                        *SteppingId      OPTIONAL,
  OUT UINT32                        *ProcessorType   OPTIONAL
  )
{
  EFI_CPUID_REGISTER                VersionInfo;
  UINT32                            RegEax;
  UINT32                            FamilyId;
  UINT32                            ExtendedFamilyId;
  UINT32                            ExtendedModelId;

  // Get CPUID(1).EAX
  AsmCpuid(CPUID_VERSION_INFO, &VersionInfo.RegEax, &VersionInfo.RegEbx, &VersionInfo.RegEcx, &VersionInfo.RegEdx);
  RegEax      = VersionInfo.RegEax;

  // Processor Type is CPUID(1).EAX[12:13]
  if (ProcessorType != NULL) {
    *ProcessorType = BitFieldRead32 (RegEax, 12, 13);
  }

  // Stepping ID is CPUID(1).EAX[0:3]
  if (SteppingId != NULL) {
    *SteppingId = BitFieldRead32 (RegEax, 0, 3);
  }

  // The Extended Family ID needs to be examined only when the Family ID is 0FH.
  // If Family ID is 0FH, Displayed Family ID = Family ID + Extended Family ID.
  // Otherwise, Displayed Family ID is Family ID
  FamilyId = BitFieldRead32 (RegEax, 8, 11);
  if (DisplayedFamily != NULL) {
    *DisplayedFamily = FamilyId;
    if (FamilyId == 0x0F) {
      ExtendedFamilyId = BitFieldRead32 (RegEax, 20, 27);
      *DisplayedFamily += ExtendedFamilyId;
    }
  }

  // The Extended Model ID needs to be examined only when the Family ID is 06H or 0FH.
  // If Family ID is 06H or 0FH, Displayed Model ID = Model ID + (Extended Model ID << 4).
  // Otherwise, Displayed Model ID is Model ID.
  if (DisplayedModel != NULL) {
    *DisplayedModel = BitFieldRead32 (RegEax, 4, 7);
    if (FamilyId == 0x06 || FamilyId == 0x0F) {
      ExtendedModelId = BitFieldRead32 (RegEax, 16, 19);
      *DisplayedModel += (ExtendedModelId << 4);
    }
  }
}


/**
    Outputs CPU information to early video.

    @param[in] PeiServices   - Pointer to Pei service table
    @param[in] EarlyVideoPpi - Pointer to PEI text out PPI

    @retval  VOID
**/
VOID
InitializeCpuData (
  IN  CONST EFI_PEI_SERVICES        **PeiServices,
  IN  AMI_PEI_TEXT_OUT              *EarlyVideoPpi
)
{
    EFI_STATUS                      Status;
    CHAR8                           BrandString[49];
    UINT8                           Index;
    UINT8		                    SocketIndex; 
    CHAR8                           Stepping[30] = "Unknown";  
    CHAR8                           CpuFamilyName[30];
    CHAR8                           CpuModelIdName[30];
    UINT8                           CoreCount;
    IIO_UDS                         *IioUds;
    EFI_GUID                        UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID; 
    EFI_HOB_GUID_TYPE               *GuidHob;
    UINT32                          ProcessorNumber = 0;
    UINT32                          FamilyId;
    UINT32                          ModelId;
    UINT32                          SteppingId;
    CHAR8                           TextString[160];
    CPU_VAR_DATA                    *mCpuVarDataPtr;
    
    DEBUG((EFI_D_INFO,"%a Entry... \n", __FUNCTION__));
    
    GuidHob    = GetFirstGuidHob (&UniversalDataGuid);
    IioUds = GET_GUID_HOB_DATA (GuidHob);
    
    if (IioUds == NULL) {
        return;
    }
    //
    // Fill the CPU details for the sockets populated
    PeiGetProcessorVersionInfo (PeiServices, &FamilyId, &ModelId, &SteppingId, NULL);
    
    // Fill Family ID string based on value
    for (Index = 0; Index < (sizeof(gFamilyIdNameMapping) / sizeof(VALUE_TO_STRING_MAPPING)); Index++) {
        if(gFamilyIdNameMapping[Index].Value == FamilyId) {
            AsciiSPrint(CpuFamilyName, sizeof(CpuFamilyName), gFamilyIdNameMapping[Index].String);
            break;
        }
    }
    
    // Fill Model ID string based on value
    for (Index = 0; Index < (sizeof(gModelIdNameMapping) / sizeof(VALUE_TO_STRING_MAPPING)); Index++) {
        if(gModelIdNameMapping[Index].Value == ModelId) {
            AsciiSPrint(CpuModelIdName, sizeof (CpuModelIdName), gModelIdNameMapping[Index].String);
            break;
        }
    }
    
    // Fill Stepping ID string based on value and ModelId
    if(ModelId == 0x8F) {
        for (Index = 0; Index < (sizeof(gSteppingIdNameMappingSpr) / sizeof(VALUE_TO_STRING_MAPPING)); Index++) {
            if (IsCpuAndRevision (CPU_SPRSP, gSteppingIdNameMappingSpr[Index].Value)) {
                AsciiSPrint(Stepping, sizeof (Stepping), gSteppingIdNameMappingSpr[Index].String);
                break;
            }
        }
    }
        
    // Fill BrandName String
    GetBrandString(PeiServices, ProcessorNumber, BrandString);
    
    mCpuVarDataPtr = GetCpuVarData ();
     // Determine the Core Count from the FusedCores mask.  Perform this check on each socket present.
    for (SocketIndex = 0, CoreCount = 0; (SocketIndex < MAX_SOCKET) && (SocketIndex < IioUds->SystemStatus.numCpus); SocketIndex++) {
        // Use Index to track the number of detected Cores in the mask.  Take the size in bytes of the mask and multiply by 8 to get total lower bits.
// EIP559224 :  To reslove system hang
        for (Index = 0; Index < (sizeof(mCpuVarDataPtr->FusedCores[SocketIndex])*8); Index++) {
            // Count number of set bits in the mask.  Assume core 0 -> core x are enabled bits 0 -> x.
            if ((mCpuVarDataPtr->FusedCores[SocketIndex] >> Index) & BIT0) {
                // Add fused Core to CoreCount.
                CoreCount++;
            }
        }
        /*
        for (Index = 0; Index < (sizeof(mCpuVarDataPtr->FusedCoresHi[SocketIndex])*8); Index++) {
            // Use Index to track the number of detected Cores in the mask.  Take the size in bytes of the mask and multiply by 8 to get total higher bits.
             if ((mCpuVarDataPtr->FusedCoresHi[SocketIndex] >> Index) & BIT0) {
                 // Add fused Core to CoreCount.
                 CoreCount++;
              }
        }
        */
// EIP559224 :  To reslove system hang
    }
       
    Status = EarlyVideoPpi->SetAttribute(EarlyVideoPpi, 0xF, 0, 0);
    
    AsciiSPrint (TextString, 160, "Intel RC Version: %s \n", RC_REVISION_VALUE);
    Status = EarlyVideoPpi->WriteString(EarlyVideoPpi, 0, TextString);
    
    AsciiSPrint (TextString, 160, " CPU Info:%a \n", BrandString);
    Status = EarlyVideoPpi->WriteString(EarlyVideoPpi, 0, TextString);
    
    AsciiSPrint (
            TextString, 160, "    Processors: %d Cores: %d Stepping: %a\n", 
            IioUds->SystemStatus.numCpus,
            CoreCount,
            Stepping);  //Possibly set to 0 in Simics
    
    Status = EarlyVideoPpi->WriteString(EarlyVideoPpi, 0, TextString);
    Status = EarlyVideoPpi->SetAttribute(EarlyVideoPpi, 0x7, 0, 0);
}
