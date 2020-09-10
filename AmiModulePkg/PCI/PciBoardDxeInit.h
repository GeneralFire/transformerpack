//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
/** @file PciBoardDxeInit.h
  Pci Board Dxe Init driver
*/

#ifndef _PCI_BOARD_DXE_INIT_H_
#define _PCI_BOARD_DXE_INIT_H_

#include <Token.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <AmiDxeLib.h>
#include <AmiLib.h>
#include <Include/PchAccess.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/PciIo.h>
#include <Protocol/IioUds.h>
#include <Protocol/AmiBoardInitPolicy.h>
#include <Protocol/AmiBdsConnectPolicy.h> 
#include <Protocol/BusSpecificDriverOverride.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/BlockIo.h>
#include <Protocol/AmiAhciBus.h>
#include <Library/PciAccessCspLib.h>
#include <Library/AmiPciBusLib.h>
#include <Library/PcdLib.h>
#include <Library/AmiPciBusSetupOverrideLib.h>
#include <Library/SetupLib.h>
#include <PciBus.h>
#include <PciHostBridge.h>
#include <Setup.h>
// APTIOV_SERVER_OVERRIDE_START 
#include <Include/Register/PchRegsSata.h> 


//#include <Include/Register/PchRegsEva.h>

#include <PchPolicyCommon.h> 
//#include <Include/PchRcVariable.h>
#include <Include/PchSetupVariableCdfEbg.h>
//#include <Include/PchSetupVariableLbg.h>
// APTIOV_SERVER_OVERRIDE_END 
#include <Protocol/IioSystem.h>

#if CRB_OUT_OF_RESOURCE_SUPPORT
#include <Guid/SetupVariable.h>
#include <Guid/SocketCommonRcVariable.h>
#endif

#if defined SECURITY_SUPPORT && SECURITY_SUPPORT
#include <Library/PciLib.h>
#include <Library/S3PciLib.h>
#endif

#include <SataRaidOverrideInitElink.h>

//defines
#define MAX_NUMBER_OFFBOARD_VGA         4  
#define PCH_PCI_DEVICES_SEGMENT_NO      0x00


//Structure defines
typedef struct _RB_INDEX_LOCATOR {
    CHAR8           *RbAslDevName;
    UINT8           Index;
}RB_INDEX_LOCATOR;

typedef struct _OFF_BOARD_VGA_DEVICES_TRIED {
    UINT16          VenId;
    UINT16          DevId;
    BOOLEAN         Enabled;
    BOOLEAN         BarDisabled;
    BOOLEAN         VgaTriedFlag;
    BOOLEAN         MultiFunc;
    UINTN           ChildFuncCount;
    PCI_DEV_INFO    VgaDevice;
    PCI_DEV_INFO    ChildFunc[8];
//    UINT64    Length;
} OFF_BOARD_VGA_DEVICES_TRIED;

EFI_STATUS
EnableDisableDevices (
  IN  PCI_DEV_INFO          *dev,
  IN  BOOLEAN               *PciOutOfResHit
);

typedef VOID (*DEVICE_PARSER)(
  IN  PCI_DEV_INFO    *Device,
  IN  BOOLEAN     *PciOutOfResHit
    
);


#endif  //_PCI_BOARD_DXE_INIT_H_
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
