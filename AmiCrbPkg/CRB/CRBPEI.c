//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file CRBPEI.c

    This file contains code for Chipset Reference Board Template 
    initialization in the PEI stage
*/

//----------------------------------------------------------------------------
// Include(s)
//----------------------------------------------------------------------------

#include <Efi.h>
#include <Pei.h>
#include <Token.h>
#include <AmiLib.h>
#include <AmiPeiLib.h>
#include <Hob.h>
#include <Setup.h>
#include <AmiCspLib.h>
#include <AmiCrbInc.h>//Build directory
#include "UncoreCommonIncludes.h"
#include <AmiPcieSegBusLib/AmiPcieSegBusLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <CrbPeiInitElink.h>
#include <../IncludePrivate/PchConfigHob.h>

#if defined(PI_SPECIFICATION_VERSION)&&(PI_SPECIFICATION_VERSION>=0x00010000)
#include <Ppi/Smbus2.h>
#else
#include <Ppi/Smbus.h>
#endif

#ifndef DISABLE_CRB_CHANGE_BOOT_MODE_ON_END_OF_PEI
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/PchPolicy.h>
#endif
// Dynamic mmcfg base address Support change
#ifdef DYNAMIC_MMCFG_BASE_FLAG 
#include "IioRegs.h"
#endif  

// APTIOV_SERVER_OVERRIDE_RC_START : DYNAMIC_MMCFG_BASE_SUPPORT Changes
#define R_IIO_MMCFG_B0                         0x90              // MMCFG Base Address
#define CORE05_BUS_NUM                            0 
#define CORE00_BUS_NUM                            0            
#define CORE00_DEV_NUM                            0            
#define CORE00_FUNC_NUM                           0            
// APTIOV_SERVER_OVERRIDE_RC_END : DYNAMIC_MMCFG_BASE_SUPPORT Changes

#if defined(PI_SPECIFICATION_VERSION)&&(PI_SPECIFICATION_VERSION>=0x00010000)
EFI_GUID gEfiPeiSmbus2PpiGuid = EFI_PEI_SMBUS2_PPI_GUID;
#else
EFI_GUID gPeiSmBusPpiGuid = EFI_PEI_SMBUS_PPI_GUID;
#endif

EFI_STATUS 
AmiUpdateSegBusPcd (
    IN EFI_PEI_SERVICES          **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
    IN VOID                      *NullPpi
);

EFI_STATUS 
CrbInitOnEndOfPei (
    IN EFI_PEI_SERVICES                   **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDesc,
    IN VOID                               *Ppi
);

static EFI_PEI_NOTIFY_DESCRIPTOR mNotifyList[] = 
{
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK,
    &gEfiSiliconRcHobsReadyPpi,
    AmiUpdateSegBusPcd
  },
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiEndOfPeiSignalPpiGuid,
    CrbInitOnEndOfPei
  }
};


/**
  This function is the entry point for this PEI. This function
  initializes the chipset CRB

    @param[in]       FileHandle  Pointer to the File Handle
    @param[in]       PeiServices Pointer to the PEI services table

    @return         EFI_STATUS
    @retval         Status            EFI_SUCCESS         - Success will be returned.
**/
EFI_STATUS 
EFIAPI 
CrbPeiInit (
    IN       EFI_PEI_FILE_HANDLE   FileHandle,
    IN CONST EFI_PEI_SERVICES     **PeiServices )
{
    EFI_STATUS Status = EFI_SUCCESS;
#if PI_SPECIFICATION_VERSION < 0x00010000
    EFI_PEI_PCI_CFG_PPI *PciCfg;   
#else
    EFI_PEI_PCI_CFG2_PPI *PciCfg;
#endif
    EFI_PEI_CPU_IO_PPI          *CpuIo;

// Dynamic mmcfg base address Support change
    #ifdef DYNAMIC_MMCFG_BASE_FLAG 
    UINT32                SecpciExbase;   
#if CPU_SKX_ONLY_SUPPORT
    // Read MMCFG_BASE Register (@ B:D:F;Offset :: 0:5:0;0x90) and Update PcdPciExpressBaseAddress with MMCFG_BASE Value
    IoWrite32 (0xCF8, 0x80000000 | (CORE05_BUS_NUM << 16) + (CORE05_DEV_NUM << 11) + (CORE05_FUNC_NUM << 8) + R_IIO_MMCFG_B0);
#else
    // Read MMCFG_BASE Register (@ B:D:F;Offset :: 0:0:0;0x90) and Update PcdPciExpressBaseAddress with MMCFG_BASE Value
    IoWrite32 (0xCF8, 0x80000000 | (CORE00_BUS_NUM << 16) + (CORE00_DEV_NUM << 11) + (CORE00_FUNC_NUM << 8) + R_IIO_MMCFG_B0);
#endif 
    SecpciExbase = IoRead32 (0xCFC) & 0xFFFFFFF0; 
    DEBUG((EFI_D_INFO, "SecpciExbase:%x\n", SecpciExbase));
    PcdSet64 (PcdPciExpressBaseAddress, (UINT64) SecpciExbase);
    DEBUG((EFI_D_INFO, "PcdPciExpressBaseAddress:%x\n", PcdGet64 (PcdPciExpressBaseAddress)));    
    #endif  


    // Get pointer to the PCI config PPI
    PciCfg = (*PeiServices)->PciCfg;
    CpuIo = (*PeiServices)->CpuIo;

    // Set the CRB Notify PPI
    Status = (*PeiServices)->NotifyPpi(PeiServices, mNotifyList);
    ASSERT_PEI_ERROR (PeiServices, Status);

    return EFI_SUCCESS;
}

EFI_STATUS 
AmiUpdateSegBusPcd (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *NullPpi
)
{
  EFI_STATUS              Status;
  PCIE_SEG_BUS_TABLE      *PcieSegBusTable;
      
  //Get the PcdPcieSegBusTablePtr
  PcieSegBusTable = (PCIE_SEG_BUS_TABLE *) PcdGetPtr (PcdPcieSegBusTablePtr);
      
  if (!PcieSegBusTable->ValidFlag) {
    Status = AmiUpdatePcieSegmentBusTable (PcieSegBusTable);
    if (EFI_ERROR(Status))
      return Status;  
  }
    
  DEBUG((EFI_D_INFO, "%a\n",__FUNCTION__)); 
  return EFI_SUCCESS;
}

#ifndef DISABLE_CRB_CHANGE_BOOT_MODE_ON_END_OF_PEI
EFI_STATUS 
CrbChangeBootModeOnEndOfPei ( 
  VOID 
)
{
  EFI_BOOT_MODE         BootMode;
  PCH_CONFIG_HOB        *PchConfigHob;
  EFI_PEI_HOB_POINTERS  Hob;

  BootMode = GetBootModeHob ();
  //
  // Get PCH Config HOB.
  //
  Hob.Guid = GetFirstGuidHob (&gPchConfigHobGuid);
  DEBUG((EFI_D_ERROR, "Hob.Guid :%x\n", Hob.Guid));
  ASSERT (Hob.Guid != NULL);
  if (Hob.Guid == NULL) {
    return EFI_NOT_FOUND;
  }
  PchConfigHob = GET_GUID_HOB_DATA(Hob.Guid);
  
  if (BootMode == BOOT_ON_FLASH_UPDATE || BootMode == BOOT_IN_RECOVERY_MODE) {
      PchConfigHob->LockDown.BiosLock = 0;
  }
  return EFI_SUCCESS;
}
#endif


EFI_STATUS 
CrbInitOnEndOfPei (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDesc,
  IN VOID                               *Ppi
)
{
  EFI_STATUS            Status = EFI_SUCCESS;

#ifndef DISABLE_CRB_CHANGE_BOOT_MODE_ON_END_OF_PEI
  Status = CrbChangeBootModeOnEndOfPei();
#endif

  return Status;
}
