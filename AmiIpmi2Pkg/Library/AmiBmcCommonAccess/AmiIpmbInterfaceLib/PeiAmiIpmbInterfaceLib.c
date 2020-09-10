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

/** @file PeiAmiIpmbInterfaceLib.c
    IPMB Transport Pei phase implementation library functions.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/AmiIpmbInterfaceLib.h>
#include <Ppi/MemoryDiscovered.h>
#include <Ppi/I2cMaster.h>

//---------------------------------------------------------------------------

/**
    After memory is discovered, update the Ipmb Interface pointer

    @param[in] PeiServices      Pointer to the PEI Core data Structure.
    @param[in] NotifyDescriptor Pointer to the instance of the
                                EFI_PEI_NOTIFY_DESCRIPTOR.
    @param[in] Ppi              Pointer to the instance of the interface.

    @retval EFI_NOT_FOUND   Ipmi Hob is not found.
    @retval Others          Return status of getting Ssif interface pointer.

**/
EFI_STATUS
EFIAPI
UpdateI2CMasterPeiApiPtr (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi )
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
    Status = AmiIpmiGetI2cApiPtr (&IpmiInstance->Interface);

    return Status;
}

/** @internal
    Send Ipmi command through Ipmb interface.

    @param[in] Interface        Pointer to System interface.
    @param[in] SlaveAddress     I2C device slave address.
    @param[in] RequestPacket    Pointer to an EFI_I2C_REQUEST_PACKET
                                structure describing the I2C transaction.

    @return EFI_STATUS  Status of the Send I2c command.

**/
EFI_STATUS
AmiIpmiI2cSendCommand (
  IN AMI_SYSTEM_INTERFACE       *Interface,
  IN UINTN                      SlaveAddress,
  IN EFI_I2C_REQUEST_PACKET     *RequestPacket )
{
    EFI_STATUS              Status = EFI_NOT_FOUND;
    EFI_PEI_I2C_MASTER_PPI  *I2cMasterTransmit = NULL;
    
    I2cMasterTransmit = (EFI_PEI_I2C_MASTER_PPI *)Interface->Ipmb.IpmbInterfaceApiPtr;

    if (I2cMasterTransmit != NULL) {
        Status = I2cMasterTransmit->StartRequest (
                                        I2cMasterTransmit,
                                        SlaveAddress,
                                        RequestPacket );
    }

    DEBUG ((DEBUG_INFO, "%a I2cMasterTransmit->StartRequest Status = %r\n", __FUNCTION__, Status)); 
    
    return Status;
}

/** @internal
    Locate I2c Ppi/Protocol instance and initialize interface pointer.

    @param[in, out] Interface   System interface pointer.

    @return EFI_STATUS  Status returned from functions used.

**/
EFI_STATUS
AmiIpmiGetI2cApiPtr (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface )
{
    EFI_STATUS                          Status;
    CONST EFI_PEI_SERVICES              **PeiServices;
    EFI_PEI_I2C_MASTER_PPI              *I2cMasterTransmit = NULL;
    static EFI_PEI_NOTIFY_DESCRIPTOR    NotifyList[] = {
                                                        (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | \
                                                        EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
                                                        &gEfiPeiMemoryDiscoveredPpiGuid,
                                                        UpdateI2CMasterPeiApiPtr
                                                       };

    PeiServices = GetPeiServicesTablePointer ();
    
    Interface->Ipmb.IpmbInterfaceApiGuid = gEfiPeiI2cMasterPpiGuid;

    // Locate the I2C PPI for Communication.
    Status = (*PeiServices)->LocatePpi (
                               PeiServices,
                               &gEfiPeiI2cMasterPpiGuid,
                               0,
                               NULL,
                               (VOID **)&I2cMasterTransmit );
    
    DEBUG ((DEBUG_INFO, "%a (*PeiServices)->LocatePpi gEfiPeiI2cMasterPpiGuid Status = %r\n", __FUNCTION__, Status)); 
    
    if (!EFI_ERROR(Status)) {
        Interface->Ipmb.IpmbInterfaceApiPtr = (UINTN)I2cMasterTransmit;
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
