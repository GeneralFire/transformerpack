//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
#ifndef __SBLIB_H__
#define __SBLIB_H__

#include <Efi.h>
#include <Token.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/BootScriptSave.h>
#include <Protocol/PciIo.h>
#include <Protocol/AmiSio.h>
//#include <Include\CmosAccess.h>
#if CMOS_MANAGER_SUPPORT
#include <CmosAccess.h>
#endif

#if CSM_SUPPORT
#include <Protocol/LegacyInterrupt.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if CSM_SUPPORT
UINT8
SBGen_GetPIRQIndex (
  UINT8 PIRQRegister
);
#endif

BOOLEAN 
SbLib_GetSmiState(
  VOID
);

VOID 
SbLib_SmiDisable(
  VOID
);

VOID 
SbLib_SmiEnable(
  VOID
);

VOID 
SbDelayUs(
  IN UINT16 wCount
);

#if SMM_SUPPORT
EFI_STATUS 
SbSmmSaveRestoreStates(
  IN BOOLEAN Save
);
#endif

EFI_STATUS 
SbLib_SetLpcDeviceDecoding (
  IN EFI_PCI_IO_PROTOCOL      *LpcPciIo,
  IN UINT16                   Base,
  IN UINT8                    DevUid,
  IN SIO_DEV_TYPE             Type
);

EFI_STATUS 
SbLib_SetLpcGenericDecoding (
    IN EFI_PCI_IO_PROTOCOL      *LpcPciIo,
    IN UINT16                   Base,
    IN UINT16                   Length,
    IN BOOLEAN                  Enable
);

#if CMOS_MANAGER_SUPPORT
EFI_STATUS 
ReadWriteCmosBank2 (
    IN CONST EFI_PEI_SERVICES   **PeiServices,  // NULL in DXE phase
    IN CMOS_ACCESS_TYPE         AccessType,
    IN UINT16                   CmosRegister,
    IN OUT UINT8                *CmosParameterValue
);
#endif

#ifdef __cplusplus
}
#endif
#endif

#ifndef _SB_ICSPLIB_H_
#define _SB_ICSPLIB_H_
#ifndef __CSP_LIB__
#define __CSP_LIB__

VOID    
CPULib_DisableInterrupt(
VOID
);

VOID    
CPULib_EnableInterrupt(
VOID
);

#endif //#ifndef __CSP_LIB__

BOOLEAN 
IsTopSwapOn(
  VOID
);

VOID SetTopSwap(
    IN BOOLEAN On
);
// ---------------
//#endif

#endif
