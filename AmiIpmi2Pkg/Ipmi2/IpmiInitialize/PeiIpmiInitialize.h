//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file PeiIpmiInitialize.c
    Pei Ipmi Transport module definitions and declarations.

*/

#ifndef _PEI_IPMI_INITIALIZE_H_
#define _PEI_IPMI_INITIALIZE_H_

//---------------------------------------------------------------------------

#include <Token.h>
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <AmiIpmiInitHooks.h>
#include <Library/BaseMemoryLib.h>
#include <AmiServerMgmtSetupVariable.h>
#include <Library/HobLib.h>
#include <Library/TimerLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AmiIpmiLib.h>
#include <Library/AmiBmcCommonInterfaceLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/IpmiTransportPpi.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

/** @internal
    Initializes Lpc to decode BMC IO ranges. This is platform specific Hook
    need to initialize chipset to decode and enable BMC IO ranges.

    @param[in] PeiServices  Describes the list of possible PEI Services.

    @return EFI_STATUS  Status returned while initialization.

**/
typedef EFI_STATUS (INIT_HOST_BMC_INTERFACE) (
  IN CONST EFI_PEI_SERVICES     **PeiServices
);

/** @internal
    Checks the Power Failure PWR_FLR bit in Power Management Control
    register and Proper delay is provided to get response from BMC when
    PWR_FLR bit is set and Wait for BMC Setup option is enabled.

    @param[in] PeiServices  Describes the list of possible PEI Services.

    @retval TRUE    If PWR_FLR bit is set.
    @retval FALSE   If PWR_FLR bit is not set.

**/
typedef BOOLEAN (BMC_INIT_DELAY_HOOK) (
  IN CONST  EFI_PEI_SERVICES          **PeiServices
);

/**
    After memory is discovered, update the Ipmi Instance pointer in Hob.

    @param[in] PeiServices      Describes the list of possible PEI Services.
    @param[in] NotifyDescriptor Pointer to notify descriptor.
    @param[in] Ppi              Pointer to Ppi.

    @retval EFI_NOT_FOUND   Ipmi hob is not found.
    @retval EFI_ABORTED     IpmiInstance pointer is null in Hob.
    @retval EFI_SUCCESS     Updated the Ipmi instance pointer successfully.

**/
EFI_STATUS
EFIAPI
UpdateIpmiInstancePtr (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
);

//---------------------------------------------------------------------------

#endif  // #ifndef _PEI_IPMI_INITIALIZE_H_

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
