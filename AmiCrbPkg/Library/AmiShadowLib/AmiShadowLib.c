//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AmiShadowLib.c
    Shadow Library implementation
*/

#include <Library/AmiShadowLib.h>

/**

  This function provides runtime interface to enable/disable
  writing in E000-F000 segmentPlatform Hook for resetting the platform.

  @param[in] BOOLEAN
             TRUE - Enable writing
             FALSE - Disable writing

  @retval VOID

**/

VOID 
NbRuntimeShadowRamWrite (
  IN BOOLEAN             Enable 
  )
{
  EFI_STATUS              Status;
  UINT8                   StackSeg;
  UINT8                   StackBus;
  UINT8                   SocId;
  UINTN                   Address;
  static UINT32           PAM0123Reg[MAX_SOCKET];
  static UINT32           PAM456Reg[MAX_SOCKET];
  static BOOLEAN          PamRwEnabled = FALSE;

  for (SocId = 0; SocId < MAX_SOCKET; SocId++) {
      
    //Check Socket/Stack valid bit. 
    if (!AmiIsStackValid (SocId, IIO_PSTACK0))
      continue;
      
    Status = AmiGetPcieSegmentBus (SocId, IIO_PSTACK0, &StackSeg, &StackBus);
    
    DEBUG((EFI_D_INFO, "AmiGetPcieSegmentBus - Status = %r StackSeg : %x, StackBus = %x\n", Status, StackSeg, StackBus));
    

    if (EFI_ERROR(Status))
      continue;
     
    Address = MmPciAddress (StackSeg, StackBus, PCI_SAD_ALL_DEVICE_NUMBER, PCI_SAD_ALL_FUNC_NUMBER, 0x00);
    
    DEBUG((EFI_D_INFO, "MmPciAddress - Address = %x\n", Address));
    
    //check PAM Lock bit. If the PAM is locked, we cannot modify the PAM attributes.
    //The lock is bypassed by SMM writes.
    //if (MmioRead32 (Address | R_PCI_SAD_ALL_PAM0123) & BIT0)
      //break;
    
    if (Enable && !PamRwEnabled) {   

      //Preserve PAM register value to restore.
      PAM0123Reg[SocId] = MmioRead32 (Address + R_PCI_SAD_ALL_PAM0123);
      PAM456Reg[SocId] = MmioRead32 (Address + R_PCI_SAD_ALL_PAM456);
      
      DEBUG((EFI_D_INFO, "MmioRead32 PAM0123Reg[%d] = %x PAM456Reg[%d] = %x\n",SocId, PAM0123Reg[SocId], SocId, PAM456Reg[SocId]));
      //0F0000-0FFFFF Attribute - Normal DRAM Operation: All reads and writes are serviced by DRAM
      MmioWrite32 ((Address + R_PCI_SAD_ALL_PAM0123), (PAM0123Reg[SocId] | PAM0123_DRAM_RW_ENABLE));
        
      //0D8000-0DBFFF / 0DC000-0DFFFF / 0E0000-0E3FFF / 0E4000-0E7FFF / 0E8000-0EBFFF / 0EC000-0EFFFF Attribute - Normal DRAM Operation: All reads and writes are serviced by DRAM
      MmioWrite32 ((Address + R_PCI_SAD_ALL_PAM456), (PAM456Reg[SocId] | PAM456_DRAM_RW_ENABLE));
        
    } else if (!Enable && PamRwEnabled){
      DEBUG((EFI_D_INFO, "MmioWrite32 PAM0123Reg[%d] = %x PAM456Reg[%d] = %x\n",SocId, PAM0123Reg[SocId], SocId, PAM456Reg[SocId]));
	  //Restore PAM register
      //0F0000-0FFFFF Attribute - DRAM Disabled: All accesses are directed to DMI
      MmioWrite32 ((Address + R_PCI_SAD_ALL_PAM0123), PAM0123Reg[SocId]);
        
      //0E0000-0E3FFF / 0E4000-0E7FFF / 0E8000-0EBFFF / 0EC000-0EFFFF Attribute - DRAM Disabled: All accesses are directed to DMI
      MmioWrite32 ((Address + R_PCI_SAD_ALL_PAM456), PAM456Reg[SocId]);
    } 
  }
  
  PamRwEnabled = Enable;
  
}

/**

  This function provides runtime interface to enable/disable
  writing in E000-F000 segmentPlatform Hook for resetting the platform.

  @param[in] BOOLEAN
             TRUE - Enable writing
             FALSE - Disable writing

  @retval VOID

**/

VOID OemRuntimeShadowRamWrite(
    IN BOOLEAN Enable
)
{
    NbRuntimeShadowRamWrite(Enable);
}
