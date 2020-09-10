//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file PlatformPeiSystemInfoDimm.c
     Gathers DIMM information

**/

#include "PlatformPeiSystemInformation.h"

/**
    Gets the DIMM's data from the system memory HOB

    @param[in] PeiServices   - Pointer to Pei service table
    @param[in] EarlyVideoPpi - Pointer to PEI text out PPI

    @retval  VOID
**/
VOID
InitializeDimmData (
  IN  CONST EFI_PEI_SERVICES        **PeiServices,
  IN  AMI_PEI_TEXT_OUT              *EarlyVideoPpi
)
{
    EFI_STATUS                      Status;
    struct SystemMemoryMapHob       *SystemMemoryMap;
    UINT32                          MemInfoTotalMem;
    UINT16                          MemInfoMemFreq;
    CHAR8                           RasMode[12]; 
    CHAR8                           TextString[160];
    
    DEBUG((EFI_D_INFO,"%a Entry... \n", __FUNCTION__));
    
    SystemMemoryMap = GetSystemMemoryMapData();
    if (SystemMemoryMap == NULL) {
        ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
        return;
      }
 
    MemInfoTotalMem = (UINT32)(SystemMemoryMap->memSize << 6);
    
    if (SystemMemoryMap->memFreq == CALCULATED_2133HZ) {
        MemInfoMemFreq = ACTUAL_2133HZ;
    } else if (SystemMemoryMap->memFreq == CALCULATED_2933HZ) {
        MemInfoMemFreq = ACTUAL_2933HZ;
    } else {
        MemInfoMemFreq = SystemMemoryMap->memFreq;
    }
    
    if (SystemMemoryMap->RasModesEnabled == 0) {
        AsciiSPrint(RasMode, sizeof (RasMode), "Indep");
    } else if (SystemMemoryMap->RasModesEnabled == FULL_MIRROR_1LM) {
        AsciiSPrint(RasMode, sizeof (RasMode), "1LM Mirror");
    } else if (SystemMemoryMap->RasModesEnabled == FULL_MIRROR_2LM) {
        AsciiSPrint(RasMode, sizeof (RasMode), "2LM Mirror");
    } else if (SystemMemoryMap->RasModesEnabled == STAT_VIRT_LOCKSTEP) {
        AsciiSPrint(RasMode, sizeof (RasMode), "Lockstep");
    } else if (SystemMemoryMap->RasModesEnabled == RK_SPARE) {
        AsciiSPrint(RasMode, sizeof (RasMode), "Rank Spare");
    } else if (SystemMemoryMap->RasModesEnabled == ADDDC_EN) {
        AsciiSPrint(RasMode, sizeof (RasMode), "ADDDC Spare");
    } else if (SystemMemoryMap->RasModesEnabled == SDDC_EN) {
        AsciiSPrint(RasMode, sizeof (RasMode), "SDDC Spare");
    } else if (SystemMemoryMap->RasModesEnabled == CH_SL) {
        AsciiSPrint(RasMode, sizeof (RasMode), "SP/LK");
    } else if (SystemMemoryMap->RasModesEnabled == CH_MS) {
        AsciiSPrint(RasMode, sizeof (RasMode), "MR/SP");
    }
    
    Status = EarlyVideoPpi->SetAttribute(EarlyVideoPpi, 0xF, 0, 0);
    
    AsciiSPrint (
            TextString, 
            160, 
            "Memory Info: Memory Size: %dGB  Memory Speed: %dMHz RAS Mode: %a\n", 
            MemInfoTotalMem >> 10, 
            MemInfoMemFreq, 
            RasMode);
    
    Status = EarlyVideoPpi->WriteString(EarlyVideoPpi, 0, TextString);
    Status = EarlyVideoPpi->SetAttribute(EarlyVideoPpi, 0x7, 0, 0);
      
}
