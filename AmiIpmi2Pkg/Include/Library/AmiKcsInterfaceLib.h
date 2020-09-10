//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file AmiKcsInterfaceLib.h
    KCS interface common function declarations and structures.

*/

#ifndef _AMI_KCS_INTERFACE_LIB_H_
#define _AMI_KCS_INTERFACE_LIB_H_

//---------------------------------------------------------------------------

#include <AmiIpmiBmcCommon.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define AMI_IPMI_MAX_KCS_CMD_DATA_SIZE   0xFF
#define AMI_IPMI_KCS_WRITE_START         0x61
#define AMI_IPMI_KCS_WRITE_END           0x62
#define AMI_IPMI_KCS_READ                0x68
#define AMI_IPMI_KCS_GET_STATUS          0x60
#define AMI_IPMI_KCS_ABORT               0x60
#define AMI_IPMI_KCS_ABORT_RETRY_COUNT   10

/**
    KCS state.
*/
typedef enum {
    AmiIpmiKcsIdleState,    ///< Kcs Idle State.
    AmiIpmiKcsReadState,    ///< Kcs Read State.
    AmiIpmiKcsWriteState,   ///< Kcs Write State.
    AmiIpmiKcsErrorState    ///< Kcs Error State.
} AMI_IPMI_KCS_STATE;

/**
    KCS status.
*/
typedef union {
    /// 8 bit raw data.
    UINT8   RawData;
    struct {
        UINT8   Obf:1;    ///< Input buffer full.
        UINT8   Ibf:1;    ///< Output buffer full.
        UINT8   SmAtn:1;  ///< SMS attention bit.
        UINT8   CD:1;     ///< Command/Data bit.
        UINT8   Oem1:1;   ///< Reserved for OEM.
        UINT8   Oem2:1;   ///< Reserved for OEM.
        UINT8   State:2;  ///< KCS state bits S0 and S1.
    } Status;
} AMI_IPMI_KCS_STATUS;

/** @internal
    OEM hook to initialize HOST BMC KCS interface data.

    @param[in] MmioBaseAddress      Pointer to the MMIO base address.
    @param[in] BaseAddressRange     Pointer to the base address range.

    @return EFI_STATUS  Return status of the Hook.

**/
typedef EFI_STATUS (AMI_OEM_INIT_KCS_INTERFACE_DATA_HOOK) (
  IN UINTN  *MmioBaseAddress,
  IN UINTN  *BaseAddressRange
);

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

/** @internal
    Allocate memory for KCS interface and initialize the variables.

    @param[in, out] Interface   System interface pointer.

    @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
    @retval EFI_SUCCESS             Interface initialized successfully.

**/
EFI_STATUS
AmiIpmiGetKcsApiPtr (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface
);

//---------------------------------------------------------------------------

#endif  // #ifndef _AMI_KCS_INTERFACE_LIB_H_

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
