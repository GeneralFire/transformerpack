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

/** @file AmiBtInterfaceLib.h
    BT interface common macros and declarations.

*/

#ifndef _AMI_BT_INTERFACE_LIB_H_
#define _AMI_BT_INTERFACE_LIB_H_

//---------------------------------------------------------------------------

#define AMI_IPMI_MAX_BT_CMD_DATA_SIZE    0xFF
#define AMI_IPMI_CLR_WR_PTR_BIT          0x01
#define AMI_IPMI_CLR_RD_PTR_BIT          0x02
#define AMI_IPMI_H2B_ATN_BIT             0x04
#define AMI_IPMI_B2H_ATN_BIT             0x08
#define AMI_IPMI_H_BUSY                  0x40
#define AMI_IPMI_B_BUSY_BIT              0x80

/** @internal
    OEM hook to initializes HOST BMC BT interface data.

    @param[in] MmioBaseAddress      Pointer to the MMIO base address.
    @param[in] BaseAddressRange     Pointer to the base address range.

    @return EFI_STATUS  Return status of the Hook.

**/
typedef EFI_STATUS (AMI_OEM_INIT_BT_INTERFACE_DATA_HOOK) (
  IN UINTN  *MmioBaseAddress,
  IN UINTN  *BaseAddressRange
);

//---------------------------------------------------------------------------

#endif  // #ifndef _AMI_BT_INTERFACE_LIB_H_

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
