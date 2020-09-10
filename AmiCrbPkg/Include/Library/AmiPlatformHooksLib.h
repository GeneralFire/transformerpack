//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
/** @file
    Contains the function declarations for Platform Hooks functions.
*/
#ifndef __AmiPlatformHooksLib__H__
#define __AmiPlatformHooksLib__H__

EFI_STATUS 
OemUpdatePlatformInfoHob (
   IN  CONST EFI_PEI_SERVICES   **PeiServices,
   OUT VOID         			*PlatformInfoHob
);

VOID 
OemPchPolicyOverride(
   IN OUT VOID                  *PchPolicy,
   IN VOID                      *SetupVariables,
   IN VOID                      *PchRcVariables
);

VOID 
OemPchSoftStrapFixupOverride(
   IN OUT UINT8                 *StrapRegion,
   IN VOID                      *SystemBoard
);

VOID 
OemInstallXhciAcpiTableOverride(
   IN VOID                      *PlatformInfo,
   IN OUT UINT64                *XhciAcpiTable
);
#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
