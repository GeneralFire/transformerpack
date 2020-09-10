//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file AmiSsifInterfaceLib.h
    SSIF interface common function declarations and macros.

*/

#ifndef _AMI_SSIF_INTERFACE_LIB_H
#define _AMI_SSIF_INTERFACE_LIB_H

//---------------------------------------------------------------------------

#include <IndustryStandard/SmBus.h>
#include <AmiIpmiBmcCommon.h>
#include <Library/AmiBmcCommonInterfaceLib.h>

//---------------------------------------------------------------------------

#define AMI_IPMI_MAX_SSIF_CMD_DATA_SIZE          0xFF
#define AMI_IPMI_SMBUS_BLOCK_LENGTH              0x20

// Smbus Write Commands.
#define AMI_IPMI_SMBUS_SINGLE_WRITE_CMD          0x2
#define AMI_IPMI_SMBUS_MULTI_WRITE_START_CMD     0x6
#define AMI_IPMI_SMBUS_MULTI_WRITE_MIDDLE_CMD    0x7
#define AMI_IPMI_SMBUS_MULTI_WRITE_END_CMD       0x8

// Smbus Read Commands.
#define AMI_IPMI_SMBUS_SINGLE_READ_CMD           0x3
#define AMI_IPMI_SMBUS_MULTI_READ_START_CMD      SMBUS_SINGLE_READ_CMD
#define AMI_IPMI_SMBUS_MULTI_READ_MIDDLE_CMD     0x9
#define AMI_IPMI_SMBUS_MULTI_READ_END_CMD        0x9
#define AMI_IPMI_SMBUS_MULTI_READ_RETRY_CMD      0xA

#define AMI_IPMI_MULTI_READ_ZEROTH_STRT_BIT      0x0
#define AMI_IPMI_MULTI_READ_FIRST_STRT_BIT       0x1

/** @internal
    Check the SMBUS alert pin status function

    @param  VOID    Nothing.

    @retval TRUE    Alert pin status is set.
    @retval FALSE   Alert pin status is not set.

**/
typedef BOOLEAN (AMI_SSIF_ALERT_PIN_CHECK) (
  VOID
);

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/**
    Execute the Get System Interface Capability command and update the RwSupport
    and PecSupport of Ipmi Instance.

    @param IpmiInstance     Ipmi Instance Data structure pointer.

    @return EFI_STATUS       Return Status

**/
VOID
GetSystemInterfaceCapability (
  IN  AMI_IPMI_BMC_INSTANCE_DATA  *IpmiInstance
);

/**
    Execute the Get Global Enable command to get receive message queue interrupt.

    @return VOID

**/

VOID
GetGlobalEnables (
  IN OUT AMI_IPMI_BMC_INSTANCE_DATA  *IpmiInstance
);

/** @internal
    Locate Smbus instance and initialize interface pointer.

    @param[in, out] Interface   System interface pointer.

    @return EFI_STATUS  Status returned while locating smbus instance.

**/
EFI_STATUS
AmiIpmiGetSmbusApiPtr (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface
);

/** @internal
    Send Ipmi command through Smbus instance.

    @param[in]      Interface       Pointer to System interface.
    @param[in]      SlaveAddress    The SMBUS hardware address.
    @param[in]      Command         This command is transmitted by the SMBus
                                    host controller to the SMBus slave device.
    @param[in]      Operation       Operation to be performed.
    @param[in]      PecCheck        Defines if Packet Error Code (PEC)
                                    checking is required for this operation.
    @param[in, out] Length          Signifies the number of bytes that this
                                    operation will do. 
    @param[in, out] Buffer          Contains the value of data to execute to
                                    the SMBus slave device. The length of
                                    this buffer is identified by Length.

    @retval EFI_NOT_FOUND   Smbus instance is not found.
    @retval Others          Return status of the Smbus Execute operation.

**/
EFI_STATUS
AmiIpmiSmbusSendCommand (
  IN     AMI_SYSTEM_INTERFACE       *Interface,
  IN     EFI_SMBUS_DEVICE_ADDRESS   SlaveAddress,
  IN     EFI_SMBUS_DEVICE_COMMAND   Command,
  IN     EFI_SMBUS_OPERATION        Operation,
  IN     BOOLEAN                    PecCheck,
  IN OUT UINTN                      *Length,
  IN OUT VOID                       *Buffer
);

//---------------------------------------------------------------------------

#endif  // #ifndef _AMI_SSIF_INTERFACE_LIB_H


