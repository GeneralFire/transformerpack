//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file PeiAmiSsifInterfaceLib.c
    SSIF Transport Pei phase Implementation library functions.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/AmiSsifInterfaceLib.h>
#include <Ppi/MemoryDiscovered.h>
#include <Ppi/Smbus2.h>

//---------------------------------------------------------------------------

/**
    After memory is discovered, update the SSIF Interface pointer.

    @param[in] PeiServices      Pointer to the PEI Core data Structure.
    @param[in] NotifyDescriptor Pointer to the instance of the
                                EFI_PEI_NOTIFY_DESCRIPTOR.
    @param[in] Ppi              Pointer to the instance of the interface.

    @retval EFI_NOT_FOUND   Ipmi Hob is not found.
    @retval Others          Return status of getting Ssif interface pointer.

**/
EFI_STATUS
EFIAPI
UpdateSmbusPeiApiPtr (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi )
{
    EFI_STATUS                    Status;
    AMI_IPMI_BMC_INSTANCE_DATA    *IpmiInstance;
    EFI_HOB_GUID_TYPE             *GuidHob;
    AMI_PEI_IPMI_DATA_HOB         *IpmiInstancePtrHob;

    GuidHob    = GetFirstGuidHob (&gAmiPeiIpmiHobGuid);

    ASSERT (GuidHob != NULL);
    if (GuidHob == NULL) {
        return EFI_NOT_FOUND;
    }

    IpmiInstancePtrHob = (AMI_PEI_IPMI_DATA_HOB *) GET_GUID_HOB_DATA(GuidHob);
    IpmiInstance = (AMI_IPMI_BMC_INSTANCE_DATA*)IpmiInstancePtrHob->IpmiInstance;
    Status = AmiIpmiGetSmbusApiPtr (&IpmiInstance->Interface);

    return Status;
}

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
  IN OUT VOID                       *Buffer )
{
    EFI_STATUS          Status = EFI_NOT_FOUND;
    EFI_PEI_SMBUS2_PPI  *EfiPeiSmbus2Ppi = NULL;
    
    EfiPeiSmbus2Ppi = (EFI_PEI_SMBUS2_PPI *)Interface->Ssif.SsifInterfaceApiPtr;

    if (EfiPeiSmbus2Ppi != NULL) {
        Status = EfiPeiSmbus2Ppi->Execute (
                                    EfiPeiSmbus2Ppi,
                                    SlaveAddress,
                                    Command,
                                    Operation,
                                    PecCheck,
                                    Length,
                                    Buffer );
    }
    
    DEBUG ((DEBUG_INFO, "%a EfiPeiSmbus2Ppi->Execute Status = %r\n", __FUNCTION__, Status)); 
    
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
    EFI_STATUS                          Status;
    UINTN                               Instance;
    CONST EFI_PEI_SERVICES              **PeiServices;
    EFI_PEI_SMBUS2_PPI                  *EfiPeiSmbus2Ppi = NULL;
    AMI_IPMI_BMC_INSTANCE_DATA          *IpmiInstance;
    AMI_BMC_STATUS                      BmcStatus;
    static EFI_PEI_NOTIFY_DESCRIPTOR    NotifyList[] = {
                                                        (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | \
                                                        EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
                                                        &gEfiPeiMemoryDiscoveredPpiGuid,
                                                        UpdateSmbusPeiApiPtr
                                                       };

    PeiServices = GetPeiServicesTablePointer ();

    Interface->Ssif.SsifInterfaceApiGuid = gEfiPeiSmbus2PpiGuid;

    IpmiInstance = AMI_IPMI_INSTANCE_FROM_INTERFACE_THIS (Interface);
    if (IpmiInstance->Signature != AMI_SM_IPMI_BMC_SIGNATURE) {
        return EFI_NOT_FOUND;
    }

    // Traverse all Smbus2 PPI instances and find the right instance for SSIF.
    for (Instance = 0; ; Instance++) {
        // Locate the Smbus Ppi.
        Status = (*PeiServices)->LocatePpi (
                               PeiServices,
                               &gEfiPeiSmbus2PpiGuid,
                               Instance,
                               NULL,
                               (VOID **)&EfiPeiSmbus2Ppi );
        if (EFI_ERROR (Status)) {
            break;
        }
        Interface->Ssif.InterfaceState = AmiIpmiInterfaceInitialized;
        Interface->Ssif.SsifInterfaceApiPtr = (UINTN)EfiPeiSmbus2Ppi;

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

    DEBUG ((DEBUG_INFO, "%a (*PeiServices)->LocatePpi gEfiPeiSmbus2PpiGuid Status = %r\n", __FUNCTION__, Status));

    if (!EFI_ERROR(Status)) {
        Interface->Ssif.SsifInterfaceApiPtr = (UINTN)EfiPeiSmbus2Ppi;
        Status = (*PeiServices)->LocatePpi (
                                  PeiServices,
                                  &gEfiPeiMemoryDiscoveredPpiGuid,
                                  0,
                                  NULL,
                                  NULL );
        if (EFI_ERROR(Status)) {
            Status = (*PeiServices)->NotifyPpi (
                                        PeiServices,
                                        &NotifyList[0] );
        }
    }

    return Status;
}
