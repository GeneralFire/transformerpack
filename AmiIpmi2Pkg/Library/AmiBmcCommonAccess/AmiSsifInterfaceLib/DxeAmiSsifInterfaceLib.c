//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file DxeAmiSsifInterfaceLib.c
    SSIF Transport Dxe phase Implementation library functions.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/AmiSsifInterfaceLib.h>
#include <Protocol/SmbusHc.h>

//---------------------------------------------------------------------------

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
  IN      AMI_SYSTEM_INTERFACE      *Interface,
  IN      EFI_SMBUS_DEVICE_ADDRESS  SlaveAddress,
  IN      EFI_SMBUS_DEVICE_COMMAND  Command,
  IN      EFI_SMBUS_OPERATION       Operation,
  IN      BOOLEAN                   PecCheck,
  IN OUT  UINTN                     *Length,
  IN OUT  VOID                      *Buffer )
{
    EFI_STATUS              Status = EFI_NOT_FOUND;
    EFI_SMBUS_HC_PROTOCOL   *EfiSmbusHcProtocol = NULL;
    
    EfiSmbusHcProtocol = (EFI_SMBUS_HC_PROTOCOL *)Interface->Ssif.SsifInterfaceApiPtr;

    if (EfiSmbusHcProtocol != NULL) {
        Status = EfiSmbusHcProtocol->Execute (
                                        EfiSmbusHcProtocol,
                                        SlaveAddress,
                                        Command,
                                        Operation,
                                        PecCheck,
                                        Length,
                                        Buffer );
    }

    DEBUG ((DEBUG_INFO, "EfiSmbusHcProtocol->Execute Status = %r\n", Status));

    return Status;
}

/** @internal
    Locate Smbus instance and initialize interface pointer.

    @param[in, out] Interface   System interface pointer.

    @return EFI_STATUS  Status returned while locating smbus instance.

**/
EFI_STATUS
AmiIpmiGetSmbusApiPtr (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface )
{
    EFI_STATUS                  Status;
    EFI_SMBUS_HC_PROTOCOL       *EfiSmbusHcProtocol = NULL;
    AMI_IPMI_BMC_INSTANCE_DATA  *IpmiInstance;
    UINTN                       HandleCount;
    EFI_HANDLE                  *HandleBuffer;
    UINTN                       Index;
    AMI_BMC_STATUS              BmcStatus;

    IpmiInstance = AMI_IPMI_INSTANCE_FROM_INTERFACE_THIS (Interface);
    if (IpmiInstance->Signature != AMI_SM_IPMI_BMC_SIGNATURE) {
        return EFI_NOT_FOUND;
    }
    Status = gBS->LocateHandleBuffer(
                        ByProtocol,
                        &gEfiSmbusHcProtocolGuid,
                        NULL,
                        &HandleCount,
                        &HandleBuffer );
    if (EFI_ERROR (Status)) {
        return Status;
    }

    for (Index = 0; Index < HandleCount; Index++) {
         Status = gBS->HandleProtocol (
                            HandleBuffer[Index],
                            &gEfiSmbusHcProtocolGuid,
                            (VOID **)&EfiSmbusHcProtocol );
         DEBUG ((DEBUG_INFO, "%a Index: %d HandleProtocol Status %r\n", __FUNCTION__, Index, Status));
         if (!EFI_ERROR (Status)) {
             Interface->Ssif.InterfaceState = AmiIpmiInterfaceInitialized;
             Interface->Ssif.SsifInterfaceApiPtr = (UINTN)EfiSmbusHcProtocol;
             Status = SelfTestBySpecificInterface(
                                         IpmiInstance,
                                         AmiSysInterfaceSsif,
                                         &BmcStatus );

             if (EFI_ERROR (Status) || (BmcStatus == AmiBmcStatusHardFail)) {
                 Interface->Ssif.InterfaceState = AmiIpmiInterfaceInitError;
                 continue;
              }
             GetSystemInterfaceCapability (IpmiInstance);
             GetGlobalEnables (IpmiInstance);
             break;
            }
        }

    return Status;
}

