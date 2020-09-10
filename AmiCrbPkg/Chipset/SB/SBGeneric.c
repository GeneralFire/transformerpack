//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file SBGeneric.c
    This file contains south bridge related code that is needed
    for both PEI & DXE stage.  To avoid code duplication this
    file is made as a library and linked both in PEI & DXE
    south bridge FFS.
	
    @note MAKE SURE NO PEI OR DXE SPECIFIC CODE IS NEEDED

**/

// Module specific Includes
#include <Efi.h>
#include <Token.h>
#include <AmiPeiLib.h>
#include <AmiDxeLib.h>
#include <AmiCspLib.h>
#include <Protocol/BootScriptSave.h>
#include "Cpu/CpuCoreRegs.h"
#include <PchAccess.h>
#include "Register/PchRegsLpc.h"
#include <Library/PciLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/PchRegsPmc.h>
#include <Library/PchPcrLib.h>
#include <Include/Register/PchRegsPcrCdf.h>

// EISS bit can't be set before NVRAMSMI
#ifndef MSR_SPCL_CHIPSET_USAGE_ADDR
#define  MSR_SPCL_CHIPSET_USAGE_ADDR 0x1FE
#endif

#define  MSR_PLATFORM_INFO SPR_MSR_PLATFORM_INFO

#define PCI_CONFIG_INDEX                0xCF8
#define DEFAULT_PCI_SEGMENT_NUMBER_PCH  0


#define   B_MSR_PLATFORM_INFO_BIOSGUARD_AVAIL   BIT35
#define   B_MSR_PLAT_FRMW_PROT_CTRL_EN          BIT1


#if CSM_SUPPORT                         // [EIP33943]
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   SBGen_GetPIRQIndex
//
// Description: This function returns the 0 based PIRQ index (PIRQ0, 1 etc)
//              based on the PIRQ register number specified in the routing
//              table
//
// Input:       PIRQRegister  Register number of the PIR
//
// Output:      PIRQIndex 0 - (MAX_PIRQ -1)
//                        0xFF if the register is invalid
//
// Notes:       Here is the control flow of this function:
//                1. If Invalid PirqNumber, return EFI_INVALID_PARAMETER.
//                2. Read into *PriqData from PIRQ register for Pirq requested.
//                3. Return EFI_SUCCESS.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

UINT8
SBGen_GetPIRQIndex(IN  UINT8 PIRQRegister)
{
	//
	// In Lewisburg PCH, PIRQ registers are no longer reside at 60h, 61h and etc in LPC device.
	// So LNKA to LNKH reg values in SB.sdl are changed to 01h -08h as PCI bus uses 0h to 
	// differentiate IRQ requirement of PCI device.
	// Csm driver expects zero based Pirq register to pass as argument to Legacy Interrupt protocol.
	// Convert one based index value to zero based index and return.
    // #PIRQ
	//
    return --PIRQRegister;
}

//----------------------------------------------------------------------------
// END OF CSM Related Porting Hooks
//----------------------------------------------------------------------------
#endif  // END OF CSM_SUPPORT
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SbLib_GetSmiState
//
// Description: This function returns SMI state
//
// Input:  None
//
// Output:  TRUE - SMI enabled, FALSE - SMI disabled
//
// Note:        This function must work at runtime. Do not use boot time services/protocols
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
BOOLEAN 
SbLib_GetSmiState(
        VOID
)
{
#if SMM_SUPPORT
                                        // [EIP25382]>
    static BOOLEAN    OriginalSmiState = FALSE;
    static BOOLEAN    HadBeenCame = FALSE;
    volatile BOOLEAN  SmiState = (IoRead32(PM_BASE_ADDRESS + R_ACPI_IO_SMI_EN) & BIT00) ? \
                                                                 TRUE : FALSE;

    if (HadBeenCame && SmiState) HadBeenCame = FALSE;

    if (HadBeenCame) {
        SmiState = OriginalSmiState;
    } else {
        OriginalSmiState = SmiState;
        HadBeenCame = TRUE;
    }

    return SmiState;
                                        // <[EIP25382]
#else
    return FALSE;
#endif
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SbLib_SmiDisable
//
// Description: This function disables SMI
//
// Input:  None
//
// Output:  None
//
// Note:        This function should be used ONLY in critical parts of code.
//              This function must work at runtime. Do not use boot time
//              services/protocols
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID SbLib_SmiDisable(VOID)
{
#if SMM_SUPPORT
    UINT32  SmiCtl = IoRead32(PM_BASE_ADDRESS + R_ACPI_IO_SMI_EN) ;
//----------------------------------
    SmiCtl &= (~BIT01); //Mask OFF EOS bit
    IoWrite32(PM_BASE_ADDRESS + R_ACPI_IO_SMI_EN, SmiCtl & (~BIT00));
#endif
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SbLib_SmiEnable
//
// Description: This function enables SMI
//
// Input:  None
//
// Output:  None
//
// Note: This function must work at runtime. Do not use boot time services/protocols
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID SbLib_SmiEnable(VOID)
{
#if SMM_SUPPORT
    UINT32  SmiCtl = IoRead32(PM_BASE_ADDRESS + R_ACPI_IO_SMI_EN);
//----------------------------------
    SmiCtl &= (~BIT01); //Mask OFF EOS bit
    IoWrite32(PM_BASE_ADDRESS + R_ACPI_IO_SMI_EN, SmiCtl | BIT00);
#endif
}

#if SMM_SUPPORT
/**
  This function can be called to enable/disable Alternate Access Mode
  @param[in] AmeCtrl              If TRUE, enable Alternate Access Mode.
                                  If FALSE, disable Alternate Access Mode.
**/
VOID
EFIAPI
SbGeneric_PchAlternateAccessMode (
  IN  BOOLEAN       AmeCtrl
  )
{
  UINT32  	Data32Or;
  UINT32  	Data32And;
  UINT32	Data32;

  Data32Or  = 0;
  Data32And = 0xFFFFFFFF;

  if (AmeCtrl == TRUE) {
    ///
    /// Enable Alternate Access Mode
    ///
    Data32Or  = (UINT32) (B_ITSS_PCR_GIC_AME);
  }

  if (AmeCtrl == FALSE) {
    ///
    /// Disable Alternate Access Mode
    ///
    Data32And = (UINT32) ~(B_ITSS_PCR_GIC_AME);
  }

  //
  // Pch PCR Read 
  //
  Data32 = MmioRead32 (PCH_PCR_ADDRESS (PID_ITSS, R_ITSS_PCR_GIC));

  Data32 &= Data32And;
  Data32 |= Data32Or;
  
  //
  // Write the PCR register with provided data
  // Then read back PCR register to prevent from back to back write.
  //
  MmioWrite32 (PCH_PCR_ADDRESS (PID_ITSS, R_ITSS_PCR_GIC), (UINT32)Data32);
  MmioRead32  (PCH_PCR_ADDRESS (PID_LPC, R_LPC_PCR_GCFD));
}

/**
    This hook is called in the every SMI entry and exit.
    Save/Restore chipset data if needed.
    
    @param[in]       Save       -   Flag to Save the Content.

    @return     EFI_STATUS
    @retval     Status            EFI_SUCCESS         - Success will be returned.
**/


EFI_STATUS 
SbSmmSaveRestoreStates(
  IN BOOLEAN Save
)
{
    static UINT8    bStoreCMOS;
    static UINT8    bStoreExtCMOS;
    static UINT32   dStoreCF8;
    static UINT32   rSpiBar0 = 0;
    UINT8           i;
    UINT32          SpiRegBase ,SpiBar0;

    SpiRegBase = PCI_SEGMENT_LIB_ADDRESS (
                     DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_SPI,
                     PCI_FUNCTION_NUMBER_PCH_SPI,
                     0
                     );

    if (Save) {
        dStoreCF8 = IoRead32(PCI_CONFIG_INDEX);      // Store CF8 (PCI index)

        // Enable Alternate access mode, let port 70h can be readable.
        SbGeneric_PchAlternateAccessMode (TRUE);

        // Read until value other than 0xFF returned or 100 reads performed
        for (i = 0; i < 100; i++) {
            bStoreCMOS = IoRead8(CMOS_ADDR_PORT);
            if (bStoreCMOS != 0xFF)
                break;
        }

        // Disable Alternate access mode.
        SbGeneric_PchAlternateAccessMode (FALSE);

        bStoreCMOS &= 0x80;               // Combine CMOS index with NMI bit
        bStoreCMOS |= IoRead8(CMOS_IO_STD_INDEX);      // Store 0x74 standard CMOS index
        bStoreExtCMOS = IoRead8(CMOS_IO_EXT_INDEX);    // Store 0x72 extended CMOS index

        SpiBar0 = PciSegmentRead32 (SpiRegBase + R_SPI_CFG_BAR0) & ~(B_SPI_CFG_BAR0_MASK);

        if (SpiBar0 != SPI_BASE_ADDRESS) {
            rSpiBar0 = SpiBar0;
            // Temporary disable MSE, and override with SPI reserved MMIO address, then enable MSE.
            SpiBar0 = SPI_BASE_ADDRESS;
            PciSegmentAnd8 (SpiRegBase + PCI_COMMAND_OFFSET, (UINT8) ~EFI_PCI_COMMAND_MEMORY_SPACE);
            PciSegmentWrite32 (SpiRegBase + R_SPI_CFG_BAR0, SpiBar0);
            PciSegmentOr8 (SpiRegBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

            DEBUG((-1, "SPI Bar in SPI COnfig %x", PciSegmentRead32 (SpiRegBase + R_SPI_CFG_BAR0) & ~(B_SPI_CFG_BAR0_MASK)));

        }
        else {
            rSpiBar0 = 0;
        }
    } else {
        if (rSpiBar0) {
            // restore original SPI bar back
            PciSegmentAnd8 (SpiRegBase + PCI_COMMAND_OFFSET, (UINT8) ~EFI_PCI_COMMAND_MEMORY_SPACE);
            PciSegmentWrite32 (SpiRegBase + R_SPI_CFG_BAR0, rSpiBar0);
            PciSegmentOr8 (SpiRegBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
        }
        IoWrite32(PCI_CONFIG_INDEX, dStoreCF8);      // Restore 0xCF8 (PCI index)
        IoWrite8(CMOS_ADDR_PORT, bStoreCMOS);       // Restore 0x70(CMOS index)
        IoWrite8(CMOS_IO_EXT_INDEX, bStoreExtCMOS); // Restore 0x72(EXT-CMOS index)
    }

    return EFI_SUCCESS;
}

#endif

//----------------------------------------------------------------------------
// Start of SPI Flash Related Porting Hooks
//----------------------------------------------------------------------------
#if SPIFlash_SUPPORT

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: ChipsetFlashDeviceWriteEnable
//
// Description: This function is invoked to do any chipset specific operations
//              that are necessary when enabling the Flash Part for writing
//
// Input:  None
//
// Output:  None
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID ChipsetFlashDeviceWriteEnable(VOID)
{
  if ( PciRead8( PCI_LIB_ADDRESS(0, 0x1F, 0x05, R_SPI_CFG_BC) ) & B_SPI_CFG_BC_LE ) {
      if ( (AsmReadMsr64 (MSR_PLATFORM_INFO) & B_MSR_PLATFORM_INFO_BIOSGUARD_AVAIL) && 
           (AsmReadMsr64 (MSR_PLAT_FRMW_PROT_CTRL) & B_MSR_PLAT_FRMW_PROT_CTRL_EN)) {
          return;
      }
  }  
  
  ///
  /// Write clear BC_SYNC_SS prior to change WPD from 0 to 1.
  ///
  PciOr8(PCI_LIB_ADDRESS(0, 0x1F, 0x05, R_SPI_CFG_BC + 1), (B_SPI_CFG_BC_SYNC_SS >> 8));
  
  ///
  /// Set BIOSWE bit (SPI PCI Offset DCh [0]) = 1b
  /// Enable the access to the BIOS space for both read and write cycles
  ///
  PciOr8( PCI_LIB_ADDRESS( 0, 0x1F, 0x05, R_SPI_CFG_BC ), B_SPI_CFG_BC_WPD );
// EISS bit can't be set before NVRAMSMI
  if ((PciRead8( PCI_LIB_ADDRESS(0, 0x1F, 0x05, R_SPI_CFG_BC) ) & B_SPI_CFG_BC_EISS) == 0) return;
  if ( !((AsmReadMsr64 (MSR_PLATFORM_INFO) & B_MSR_PLATFORM_INFO_BIOSGUARD_AVAIL) &&
         (AsmReadMsr64 (MSR_PLAT_FRMW_PROT_CTRL) & B_MSR_PLAT_FRMW_PROT_CTRL_EN))) {
	   UINT32 Data32;
       Data32 = MmioRead32 ((UINTN) (0xFED30880)) | (UINT32) (BIT0);
       AsmWriteMsr64(MSR_SPCL_CHIPSET_USAGE_ADDR, Data32);
  }

}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: ChipsetFlashDeviceWriteDisable
//
// Description: This function is invoked to do any chipset specific operations
//              that are necessary when disabling the Flash Part for writing
//
// Input:  None
//
// Output:  None
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID ChipsetFlashDeviceWriteDisable(VOID)
{
  if (PciRead8( PCI_LIB_ADDRESS(0, 0x1F, 0x05, R_SPI_CFG_BC) ) & B_SPI_CFG_BC_LE) {
      if ( (AsmReadMsr64 (MSR_PLATFORM_INFO) & B_MSR_PLATFORM_INFO_BIOSGUARD_AVAIL) && 
           (AsmReadMsr64 (MSR_PLAT_FRMW_PROT_CTRL) & B_MSR_PLAT_FRMW_PROT_CTRL_EN)) {
          return;
      }
  }  
  
  ///
  /// Clear BIOSWE bit (SPI PCI Offset DCh [0]) = 0b
  /// Disable the access to the BIOS space for write cycles  
  PciAnd8( PCI_LIB_ADDRESS(0, 0x1F, 0x05, R_SPI_CFG_BC), ~(B_SPI_CFG_BC_WPD) );
  
// EISS bit can't be set before NVRAMSMI
  if ((PciRead8( PCI_LIB_ADDRESS(0, 0x1F, 0x05, R_SPI_CFG_BC) ) & B_SPI_CFG_BC_EISS) == 0) return;
  if ( !((AsmReadMsr64 (MSR_PLATFORM_INFO) & B_MSR_PLATFORM_INFO_BIOSGUARD_AVAIL) &&
         (AsmReadMsr64 (MSR_PLAT_FRMW_PROT_CTRL) & B_MSR_PLAT_FRMW_PROT_CTRL_EN))) {
        UINT32 Data32;
        Data32 = MmioRead32 ((UINTN) (0xFED30880)) & (UINT32) (~BIT0);
        AsmWriteMsr64 (MSR_SPCL_CHIPSET_USAGE_ADDR, Data32);
  }
}
#endif // #if SPIFlash_SUPPORT

//----------------------------------------------------------------------------
// END of SPI Flash related porting hooks
//----------------------------------------------------------------------------

#if defined(LOAD_DEFAULTS_ON_CMOS_CLEAR) && (LOAD_DEFAULTS_ON_CMOS_CLEAR == 1)
/**
 * 
  This function determines if the system should boot with the default configuration.
  This function is called by NVRAM PEI code.
  If boot with default configuration is detected, default values for NVRAM variables are used.

    @param[in] **PeiServices - pointer to a pointer to the PEI Services Table.
    @param[in] *ReadVariablePpi - pointer to EFI_PEI_READ_ONLY_VARIABLE2_PPI PPI. 
               The pointer can be used to read and enumerate existing NVRAM variables.


    @return     BOOLEAN
    @retval     TRUE    -   
**/

BOOLEAN 
IsPchRtcReset(
    IN EFI_PEI_SERVICES **PeiServices,
    IN VOID *ReadVariablePpi
)
{
   if(PciRead32( PCI_LIB_ADDRESS (DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_PMC, PCI_FUNCTION_NUMBER_PCH_PMC, R_PMC_PWRM_GEN_PMCON_B) ) & B_PMC_PWRM_GEN_PMCON_B_RTC_PWR_STS ) //Check RTC Power Status is set to see whether  RTCRST# is asserted. Clearing CMOS can be done by using a jumper on RTCRST
      return TRUE;                                                                                                                                                           //RTC_PWR_STS  bit will remain set until the software clears it by writing a 0 back to this bit position. The PCH code will take care of  this.
   
    return FALSE;
}
#endif

/**
  This function checks if TopSwap (A16 address line inversion)
  is on

    @param[in]       VOID

    @return     BOOLEAN
    @retval     TRUE    -   Top Swap Enabled.
                FALSE   -   Top Swap Disabled.
**/
BOOLEAN 
IsTopSwapOn(
  VOID
)
{
  UINT8             PchPcrRtcBuc=0;

  PchPcrRtcBuc = MmioRead8 (PCH_PCR_ADDRESS (PID_RTC_HOST, R_RTC_PCR_BUC));

  return ( PchPcrRtcBuc & B_RTC_PCR_BUC_TS) ? TRUE : FALSE;

}

/**
  This function sets TopSwap (A16 address line inversion) ON or OFF
  
  @param[in]       On -   Flag to configure Top Swap   

  @return     VOID
**/

VOID  
SetTopSwap(
  IN BOOLEAN On
)
{
  UINT8	Data8;
    
    if ( On ) {
        //
        // Enable TopSwap bit in RTC RAM
        //
        Data8 = MmioRead8 (PCH_PCR_ADDRESS (PID_RTC_HOST, R_RTC_PCR_BUC));

        Data8 &= (UINT8)~0;
        Data8 |= B_RTC_PCR_BUC_TS;
        
        //
        // Write the PCR register with provided data
        // Then read back PCR register to prevent from back to back write.
        //
        MmioWrite8 (PCH_PCR_ADDRESS (PID_RTC_HOST, R_RTC_PCR_BUC), (UINT32)Data8);
        MmioRead8  (PCH_PCR_ADDRESS (PID_LPC, R_LPC_PCR_GCFD));
        
    } else {
        //
        // Disable TopSwap bit in RTC RAM
        //
        //
        // Enable TopSwap bit in RTC RAM
        //
        Data8 = MmioRead8 (PCH_PCR_ADDRESS (PID_RTC_HOST, R_RTC_PCR_BUC));

        Data8 &= (UINT8)~B_RTC_PCR_BUC_TS;
        Data8 |= 0;
        
        //
        // Write the PCR register with provided data
        // Then read back PCR register to prevent from back to back write.
        //
        MmioWrite8 (PCH_PCR_ADDRESS (PID_RTC_HOST, R_RTC_PCR_BUC), (UINT32)Data8);
        MmioRead8  (PCH_PCR_ADDRESS (PID_LPC, R_LPC_PCR_GCFD));
    }
  if ( On ) {
      
      //
      // Enable TopSwap bit in RTC RAM
      //
      Data8 = MmioRead8 (PCH_PCR_ADDRESS (PID_RTC_HOST, R_RTC_PCR_BUC));

      Data8 &= (UINT8)~0;
      Data8 |= B_RTC_PCR_BUC_TS;
    
      //
      // Write the PCR register with provided data
      // Then read back PCR register to prevent from back to back write.
      //
      MmioWrite8 (PCH_PCR_ADDRESS (PID_RTC_HOST, R_RTC_PCR_BUC), (UINT32)Data8);
      MmioRead8  (PCH_PCR_ADDRESS (PID_LPC, R_LPC_PCR_GCFD));
    
      } else {
          //
          // Disable TopSwap bit in RTC RAM
          //
          //
          // Enable TopSwap bit in RTC RAM
          //
          Data8 = MmioRead8 (PCH_PCR_ADDRESS (PID_RTC_HOST, R_RTC_PCR_BUC));
    
          Data8 &= (UINT8)~B_RTC_PCR_BUC_TS;
          Data8 |= 0;
        
          //
          // Write the PCR register with provided data
          // Then read back PCR register to prevent from back to back write.
          //
          MmioWrite8 (PCH_PCR_ADDRESS (PID_RTC_HOST, R_RTC_PCR_BUC), (UINT32)Data8);
          MmioRead8  (PCH_PCR_ADDRESS (PID_LPC, R_LPC_PCR_GCFD));
      }
}
