//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file
    Contains the functions definitions for
    1. To decode BMC KCS base address of BMC
    2. Check Trickle current Power failure status
*/

#include <Token.h>
#include <PiPei.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/MmPciBaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Register/PchRegsPmc.h>
#include <PchAccess.h>

#define KCS_BASE_ADDRESS_MASK   	0xFFF0
#define NUMBER_OF_BYTES_TO_DECODE	0x10

/**
    Initializes LPC/eSPI Generic IO decode range registers to decode BMC IO ranges. This is platform specific Hook
    need to initialize chipset to decode and enable BMC IO ranges.

    @param PeiServices Pointer to the PEI Core data Structure

    @return EFI_STATUS
    @retval Status returned

**/
EFI_STATUS
DecodeBmcBaseAddress (
  IN  CONST EFI_PEI_SERVICES	**PeiServices )
{
	EFI_STATUS          Status;
	
    DEBUG ((DEBUG_INFO, "%a: Decoding 0x%X bytes from 0x%X base address \n", __FUNCTION__,
    		NUMBER_OF_BYTES_TO_DECODE, (IPMI_KCS_BASE_ADDRESS & KCS_BASE_ADDRESS_MASK)));

    // Decode KCS BMC IO Ranges
	Status = PchLpcGenIoRangeSet ((IPMI_KCS_BASE_ADDRESS & KCS_BASE_ADDRESS_MASK), NUMBER_OF_BYTES_TO_DECODE);
	
	return Status;
}

/**
    Checks the Power Failure PWR_FLR bit status in Power Management Control register.

    @param PeiServices Pointer to the PEI Core data Structure

    @return BOOLEAN
    @retval TRUE  - If PWR_FLR bit is set.
    @retval FALSE - If PWR_FLR bit is not set.

**/
BOOLEAN
CheckPowerFailureStatus (
  IN CONST EFI_PEI_SERVICES          **PeiServices )
{

  UINTN                       PmcGenPmConfigBAddress;
  UINT32                      PmConfigBData;
    
  PmcGenPmConfigBAddress = MmPciAddress (0,                           //Seg
                                          DEFAULT_PCI_BUS_NUMBER_PCH,
                                          PCI_DEVICE_NUMBER_PCH_PMC,
                                          PCI_FUNCTION_NUMBER_PCH_PMC,
                                          R_PMC_PWRM_GEN_PMCON_A);
    
  PmConfigBData = MmioRead32(PmcGenPmConfigBAddress);
    
  DEBUG ((DEBUG_INFO, "Power Failure PWR_FLR bit: %x\n",(PmConfigBData & B_PMC_PWRM_GEN_PMCON_A_PWR_FLR) >> 1 ));    

  // Check Power Failure PWR_FLR bit for AC power loss.
  if (PmConfigBData & B_PMC_PWRM_GEN_PMCON_A_PWR_FLR) {
      DEBUG ((DEBUG_INFO, "Power Failure PWR_FLR bit is set due to AC power loss.\n"));
      return TRUE;
  }
  return FALSE;
}
