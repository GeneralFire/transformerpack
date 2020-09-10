//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file DxeIpmiInitialize.h
    Dxe Ipmi Transport driver macros, definitions and function declarations.

*/

#ifndef _DXE_IPMI_INITIALIZE_H_
#define _DXE_IPMI_INITIALIZE_H_

//---------------------------------------------------------------------------

#include <Token.h>
#include <AmiIpmiInitHooks.h>
#include <IndustryStandard/IpmiNetFnSensorEvent.h>
#include <AmiServerMgmtSetupVariable.h>
#include <AmiIpmiNetFnStorageDefinitions.h>
#include <AmiDxeLib.h>
#include <Library/UefiLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/AmiIpmiLib.h>
#include <Library/AmiBmcCommonInterfaceLib.h>
#include <Protocol/RealTimeClock.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Protocol/IpmiSelfTestLogProtocol.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern AMI_IPMI_BMC_INSTANCE_DATA   *gIpmiInstance;

/** @internal
    Initiates the Smbios Types structure update hooks of OEM.

    @param[in, out] SmbiosTypeRecord    Pointer to the Smbios types structure.

    @return EFI_STATUS  Return status of the hooks.

**/
typedef EFI_STATUS (IPMI_OEM_SMBIOS_TYPES_UPDATE_HOOK) (
  IN OUT VOID   **SmbiosTypeRecord
);

/** @internal
    Initiates the System info update hooks of OEM.

    @param[in, out] StringPtr           Pointer to the String.
    @param[in, out] ParameterSelector   Pointer to the Parameter Selector.
    @param[in, out] StringEncoding      Pointer to the String Encoding.

    @return EFI_STATUS                  Return status of the hooks.

**/
typedef EFI_STATUS (IPMI_OEM_SYSTEM_INFO_UPDATE_HOOK) (
  IN OUT   VOID                                 **StringPtr,
  IN OUT   AMI_SYSTEM_INFO_PARAMETER_SELECTOR   *ParameterSelector,
  IN OUT   AMI_SYSTEM_INFO_STRING_ENCODING      *StringEncoding
);

/** @internal
    Registers notification call back AcpiTable guid to publish SPMI table
    and update IPMI Specification Version ASL Object update if
    AcpiTable guid is already installed.

    @param VOID     Nothing.

    @return EFI_STATUS     Return status of function used.

**/
EFI_STATUS
SpmiSupport (
  VOID
);

/** @internal
    Registers notification call back on SMBIOS protocol to install Smbios
    type 38 and 42 structure.

    @param VOID     Nothing.

    @return VOID    Nothing.

**/
VOID
IpmiSmbiosTypesSupport (
  VOID
);

/**
    Method check for CMOS Clear flag bit is set. If set Create a NVRAM  variable.

    @param VOID

    @return EFI_STATUS Status of Execution

**/
EFI_STATUS 
IpmiCmosClear (
  VOID
);

/**

    Update System info to BMC ex:FIrmware version

    @param SmbiosProtocol    Pointer to SMBIOS protocol instance

    @return EFI_STATUS       Status returned by function calls used

**/
EFI_STATUS
IpmiUpdateSystemInfo (
  IN  EFI_SMBIOS_PROTOCOL             *SmbiosProtocol
);

/** @internal
    Install  Ipmi Info protocol.

    @param  VOID    Nothing.

    @return EFI_STATUS  Status returned from functions used.

**/
EFI_STATUS
IpmiInstallInfoProtocol (
   IN  EFI_HANDLE         NewHandle
  );

//---------------------------------------------------------------------------

#endif  // #ifndef _DXE_IPMI_INITIALIZE_H_

