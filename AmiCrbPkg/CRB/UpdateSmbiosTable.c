//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file UpdateSmbiosTable.c
    SMBIOS table is updated here
**/

#include <UpdateSmbiosTable.h>

EFI_SMBIOS_PROTOCOL      *gEfiSmbiosProtocol;
EFI_IIO_SYSTEM_PROTOCOL  *gIioSystemProtocol;
PCI_DEV_INFO             *dev = NULL;
SMBIOS_TABLE_TYPE41      *Type41Table;

SLOT_INFO gSlotInfo [] = {
//#if PLATFORM_TYPE == 0    
          //{ Socket,          Stack,          Slot Type,                 Data bus width,      Dev/Func number}
            { SLOT1_SOCKET_NO, SLOT1_STACK_NO, SlotTypePciExpressGen3X8,  SlotDataBusWidth8X,  SLOT1_DEV_FUN_NO},
            { SLOT2_SOCKET_NO, SLOT2_STACK_NO, SlotTypePciExpressGen3X16, SlotDataBusWidth16X, SLOT2_DEV_FUN_NO},
            { SLOT3_SOCKET_NO, SLOT3_STACK_NO, SlotTypePciExpressGen3X8,  SlotDataBusWidth8X,  SLOT3_DEV_FUN_NO},
            { SLOT4_SOCKET_NO, SLOT4_STACK_NO, SlotTypePciExpressGen3X16, SlotDataBusWidth16X, SLOT4_DEV_FUN_NO},
            { SLOT5_SOCKET_NO, SLOT5_STACK_NO, SlotTypePciExpressGen3X8,  SlotDataBusWidth8X,  SLOT5_DEV_FUN_NO},
            { SLOT6_SOCKET_NO, SLOT6_STACK_NO, SlotTypePciExpressGen3X8,  SlotDataBusWidth8X,  SLOT6_DEV_FUN_NO},
            { SLOT7_SOCKET_NO, SLOT7_STACK_NO, SlotTypePciExpressGen3X8,  SlotDataBusWidth8X,  SLOT7_DEV_FUN_NO},
            { SLOT8_SOCKET_NO, SLOT8_STACK_NO, SlotTypePciExpressGen3X16, SlotDataBusWidth16X, SLOT8_DEV_FUN_NO},
/*#else
            { SLOT1_SOCKET_NO, SLOT1_STACK_NO, SlotTypePciExpressGen3X16, SlotDataBusWidth16X, SLOT1_DEV_FUN_NO},
            { SLOT2_SOCKET_NO, SLOT2_STACK_NO, SlotTypePciExpressGen3X16, SlotDataBusWidth16X, SLOT2_DEV_FUN_NO},
            { SLOT3_SOCKET_NO, SLOT3_STACK_NO, SlotTypePciExpressGen3X16, SlotDataBusWidth16X, SLOT3_DEV_FUN_NO},
            { SLOT4_SOCKET_NO, SLOT4_STACK_NO, SlotTypePciExpressGen3X16, SlotDataBusWidth16X, SLOT4_DEV_FUN_NO},
            { SLOT5_SOCKET_NO, SLOT5_STACK_NO, SlotTypePciExpressGen3X16, SlotDataBusWidth16X, SLOT5_DEV_FUN_NO},
            { SLOT6_SOCKET_NO, SLOT6_STACK_NO, SlotTypePciExpressGen3X16, SlotDataBusWidth16X, SLOT6_DEV_FUN_NO},
            { SLOT7_SOCKET_NO, SLOT7_STACK_NO, SlotTypePciExpressGen3X16, SlotDataBusWidth16X, SLOT7_DEV_FUN_NO},
#endif*/
};

/**
  Add SMBIOS type 9 structures based on the count given.

  @param AddSlotCount       Slot Counts
  
  @retval EFI_STATUS        Based on ADD API of EFI SMBIOS protocol
**/

EFI_STATUS
AddStructureForAdditionalSlots (
    UINT8 AddSlotCount
)
{
    SMBIOS_TABLE_TYPE9       *Type9Table;
    EFI_STATUS               Status = EFI_SUCCESS;
    EFI_SMBIOS_HANDLE        SmbiosHandle;
    UINT8                    Index;

    Type9Table = AllocateZeroPool( sizeof(SMBIOS_TABLE_TYPE9));

    Type9Table->Hdr.Type = EFI_SMBIOS_TYPE_SYSTEM_SLOTS;
    Type9Table->Hdr.Length = sizeof(SMBIOS_TABLE_TYPE9);
    Type9Table->SlotDesignation = 1;
    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    
    for (Index=0; Index < AddSlotCount; Index++) {
        Status = gEfiSmbiosProtocol->Add( gEfiSmbiosProtocol, NULL, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER*) &(Type9Table->Hdr));
        DEBUG(( DEBUG_INFO,  "%a : Adding Slot %d structure status  - %r \n",__FUNCTION__,Index,Status));
    }
    
    FreePool(Type9Table);
    return Status;
}

/**
  Update the SMBIOS TYPE 9 table with proper values.
  
  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
UpdateSmbiosType9Table (
    VOID
)
{
    EFI_STATUS               Status;
    UINT8                    Index;
    UINT8                    SecBus;
    UINT32                   VendorIDDeviceID;
    UINTN                    StringNumber;
    SMBIOS_TABLE_TYPE9       *Type9Table;
    EFI_SMBIOS_HANDLE        SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    EFI_SMBIOS_TYPE          SmbiosType = EFI_SMBIOS_TYPE_SYSTEM_SLOTS;
    CHAR8                    *SlotDesignator[] = {"Slot 1", "Slot 2", "Slot 3", "Slot 4", "Slot 5", "Slot 6", "Slot 7", "Slot 8"};
/** #if PPO_FLAG == 0
    CHAR8                           *SlotDesignator[] = {"Slot C", "Slot B", "Slot D", "Slot 3", "Slot 1", "Slot 6", "Slot 4"};
#else
    CHAR8                           *SlotDesignator[] = {"Slot 1", "Slot 2", "Slot 3", "Slot 4", "Slot 5", "Slot 6"};
#endif
#elif PROJECT_TYPE == 0 // CooperCity
    CHAR8                           *SlotDesignator[] = {"Slot 1", "Slot 2", "Slot 3", "Slot 4", "Slot 5", "Slot 6", "Slot 7", "Slot 8"};
#endif */

    USRA_ADDRESS                    Address;
    UINT8                           CapPtr;
    PCI_REG_PCIE_SLOT_CAPABILITY    SlotCap;
    
    DEBUG(( DEBUG_INFO, __FUNCTION__ " Updating Type 9\n"));

    for (Index = 0; Index < (sizeof(gSlotInfo)/ sizeof(SLOT_INFO)); Index++) {

        // Read Type 9 structure
        Status = gEfiSmbiosProtocol->GetNext( gEfiSmbiosProtocol, &SmbiosHandle, &SmbiosType, (EFI_SMBIOS_TABLE_HEADER **) &Type9Table, NULL);
        if (EFI_ERROR(Status))
            return Status;

        if (SmbiosHandle == SMBIOS_HANDLE_PI_RESERVED) 
            break;

        //Updating common data of all slots
        Type9Table->SlotLength = SlotLengthLong;
        Type9Table->SlotDesignation = 1;
        Type9Table->SlotCharacteristics1.Provides33Volts = 0x1;
        Type9Table->SlotCharacteristics1.SharedSlot = 0x0;
        Type9Table->SlotCharacteristics2.PmeSignalSupported = 0x1;


        Type9Table->DevFuncNum = gSlotInfo[Index].DevFun;
        Type9Table->SlotType = gSlotInfo[Index].SlotType;
        Type9Table->SlotDataBusWidth = gSlotInfo[Index].SlotDataBusWidth;

        //Updating slot's dynamic data
        Status = AmiGetPcieSegmentBus ( gSlotInfo[Index].Socket, gSlotInfo[Index].Stack, (UINT8 *) &Type9Table->SegmentGroupNum, &Type9Table->BusNum);
        if (EFI_ERROR(Status))
            return Status;
        SecBus = PciExpressRead8 (PCI_EXPRESS_LIB_ADDRESS( Type9Table->BusNum, Type9Table->DevFuncNum >> 3, Type9Table->DevFuncNum & 0x7 ,PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));

        if (SecBus == 0xFF){
            Type9Table->CurrentUsage = SlotUsageUnknown;
        } else {

            //Checking for Slot device availability
            VendorIDDeviceID = PciExpressRead32 (PCI_EXPRESS_LIB_ADDRESS(SecBus,0,0,0));
            if (VendorIDDeviceID == 0xFFFFFFFF) {
                Type9Table->CurrentUsage  = SlotUsageAvailable;
            } else {
                Type9Table->CurrentUsage = SlotUsageInUse;
            }
        }

        if((PciExpressRead32 (PCI_EXPRESS_LIB_ADDRESS(Type9Table->BusNum, Type9Table->DevFuncNum >> 3, Type9Table->DevFuncNum & 0x7,0)))!=0xFFFFFFFF){
            USRA_PCIE_SEG_ADDRESS (Address, UsraWidth32, Type9Table->SegmentGroupNum, Type9Table->BusNum, Type9Table->DevFuncNum >> 3, Type9Table->DevFuncNum & 0x7, 0);
            CapPtr = PciFindCapId (&Address, EFI_PCI_CAPABILITY_ID_PCIEXP);

            Address.Pcie.Offset = CapPtr + OFFSET_OF(PCI_CAPABILITY_PCIEXP, SlotCapability);
            RegisterRead (&Address, &SlotCap.Uint32);

            Type9Table->SlotID = (UINT16)SlotCap.Bits.PhysicalSlotNumber ;
            
                if (SlotCap.Bits.HotPlugCapable == 1){
                     Type9Table->SlotCharacteristics2.HotPlugDevicesSupported = 1;
                }
        } else {
            Type9Table->SlotCharacteristics2.HotPlugDevicesSupported = 0;
        }

        // Updating Slot designator
        StringNumber = (UINTN) Type9Table->SlotDesignation;
        Status = gEfiSmbiosProtocol->UpdateString ( gEfiSmbiosProtocol, &SmbiosHandle, &StringNumber, SlotDesignator[Index]);
    }
    DEBUG(( DEBUG_INFO, "%a() Updating Type 9 EXIT\n", __FUNCTION__ ));
    return Status;

}

/**
  Update the SMBIOS TYPE 41 table with onboard device informations

  @param DevType            Onboard Device Types field
  @param Designation        Designation
  @param DeviceInstance     Unique number for each Device Type
                            
  @retval EFI_STATUS
**/

EFI_STATUS
EFIAPI
UpdateSmbiosType41TableData (
    IN  UINT8           DevType,
    IN  CONST CHAR8*    Designation,
    IN  UINT8           DeviceInstance
)
{
    UINT8                    DevFuncNum;
    EFI_SMBIOS_HANDLE        SmbiosHandle;
    UINT8                    *Type41StrPtr;
    CHAR8                    TempStr[30];
    EFI_STATUS               Status;
    
    Type41Table = AllocateZeroPool( sizeof(SMBIOS_TABLE_TYPE41) + sizeof(UINT16) + SMBIOS_STRING_MAX_LENGTH );
   
    // Updating the Smbios Type41 Structure  with Onboard Device information.
    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    Type41Table->Hdr.Type = EFI_SMBIOS_TYPE_ONBOARD_DEVICES_EXTENDED_INFORMATION;
    Type41Table->Hdr.Length = sizeof(SMBIOS_TABLE_TYPE41);
    Type41Table->ReferenceDesignation = 1;
    Type41Table->DeviceType = DevType;
    
    if (MmioRead32(MmPciBase(dev->AmiSdlPciDevData->Bus, dev->AmiSdlPciDevData->Device, dev->AmiSdlPciDevData->Function)) != 0xFFFFFFFF) {
        Type41Table->DeviceType |= DeciveEnable;
    }
    Type41Table->SegmentGroupNum = 0;
    Type41Table->DeviceTypeInstance = DeviceInstance;
    Type41Table->BusNum = dev->AmiSdlPciDevData->Bus;
    DevFuncNum = ((dev->AmiSdlPciDevData->Device << 3) | (dev->AmiSdlPciDevData->Function) );
    Type41Table->DevFuncNum = DevFuncNum;
    
    // Add the Strings to SMBIOS data
    AsciiSPrint (TempStr, sizeof (TempStr), Designation);
    Type41StrPtr = (UINT8 *)(Type41Table + 1);
    //copy string with null terminator
    CopyMem (Type41StrPtr, TempStr, AsciiStrLen(TempStr) + 1);
      
    
    Status = gEfiSmbiosProtocol->Add( gEfiSmbiosProtocol, NULL, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER*)&(Type41Table->Hdr));
    if(!EFI_ERROR(Status))
        FreePool(Type41Table);
             
    
    return Status;               
}

/**
  Update the SMBIOS TYPE 41 table with onboard device informations

  @retval EFI_STATUS
**/

EFI_STATUS
EFIAPI
UpdateSmbiosType41Table (
    VOID
)
{
    EFI_STATUS               Status;
    UINTN                    Index;
    EFI_HANDLE               *HandleBuffer;
    UINTN                    NumberofHandles;
    EFI_PCI_IO_PROTOCOL      *PciIo;       
    UINT8                    ScsiDevInstance = 1;
    UINT8                    SataDevInstance = 1;
    UINT8                    SasDevInstance = 1;
    UINT8                    EthernetDevInstance = 1;
    UINT8                    TokenRingDevInstance = 1;
    UINT8                    VgaDevInstance = 1;
    UINT8                    XgaDevInstance = 1;
    UINT8                    MMAudioDevInstance = 1;
    UINT8                    AudioDevInstance = 1;
    
    DEBUG(( DEBUG_INFO, "%a Updating Type 41 Entry...\n", __FUNCTION__));
    
    Status = pBS->LocateHandleBuffer(
                        ByProtocol,
                        &gEfiPciIoProtocolGuid,
                        NULL,
                        &NumberofHandles,
                        &HandleBuffer
                        );
    
    if(EFI_ERROR(Status)){
        return Status;
    }
    
    for (Index = 0; Index < NumberofHandles; Index++){
        
      Status = pBS->HandleProtocol(HandleBuffer[Index], &gEfiPciIoProtocolGuid, (VOID**)&PciIo);
      if (EFI_ERROR(Status)){
          return Status;
      }
      dev =  (PCI_DEV_INFO*)PciIo;
      
      // Check if it is  an onboard PCI device.

      if ((dev->AmiSdlPciDevData!= NULL)&&(dev->AmiSdlPciDevData->PciDevFlags.Bits.OnBoard)){
          switch(dev->Class.BaseClassCode){
          
          // Check whether Onboard device is SCSI/SATA/SAS Controller. 
          case MassStorage:
              if(dev->Class.SubClassCode == SCSIController ){
                  UpdateSmbiosType41TableData(SCSI_CONTROLLER, "Onboard SCSI Controller",ScsiDevInstance);
                  ScsiDevInstance++;
              }
              if(dev->Class.SubClassCode == SataController ){
                  UpdateSmbiosType41TableData(SATA_CONTROLLER, "Onboard SATA Controller",SataDevInstance);
                  SataDevInstance++;
              }
              if(dev->Class.SubClassCode == SasController){
                  UpdateSmbiosType41TableData(SAS_CONTROLLER, "Onboard SAS Controller",SasDevInstance);
                  SasDevInstance++;
              }
              break;
          // Check whether Onboard device is Ethernet/TokenRing Controller.    
          case Network:
              if(dev->Class.SubClassCode == EthernetController ){
                  UpdateSmbiosType41TableData(ETHERNET_CONTROLLER, "Onboard ETHERNET Controller",EthernetDevInstance);
                  EthernetDevInstance++;
              }
              if(dev->Class.SubClassCode == TokenRingController ){
                  UpdateSmbiosType41TableData(TOKEN_RING, "Onboard TOKEN RING",TokenRingDevInstance);
                  TokenRingDevInstance++;
              }
              break;
          // Check whether Onboard device is VGA/XGA Controller.    
          case Video:
              if(dev->Class.SubClassCode == VGAController ){
                  UpdateSmbiosType41TableData(VIDEO_CONTROLLER, "Onboard VGA",VgaDevInstance);
                  VgaDevInstance++;
              }
              if(dev->Class.SubClassCode == XGAController ){
                  UpdateSmbiosType41TableData(VIDEO_CONTROLLER, "Onboard XGA",XgaDevInstance);
                  XgaDevInstance++;
              }
              break;
          // Check whether Onboard device is AudioController/AudioDecive.
          case MultiMedia:
              if(dev->Class.SubClassCode == AudioController ){
                  UpdateSmbiosType41TableData( SOUND , "Onboard MultiMediaAudioController",MMAudioDevInstance);
                  MMAudioDevInstance++;
              }
              if(dev->Class.SubClassCode == AudioDevice ){
                  UpdateSmbiosType41TableData(SOUND, "Onboard AudioDevice",AudioDevInstance);
                  AudioDevInstance++;
              }
              break;
             
          }
      }   
    }
    DEBUG(( DEBUG_INFO, "%a Updating Type 41 Status %r Exiting...\n", __FUNCTION__, Status));
    
    return Status;
}

/**
  EFI call back for EFI SMBIOS protocol. Update SMBIOS Structures

  @param Event              Event created for the callback.
  @param Context            Any arguments passed.
**/


VOID
EFIAPI
UpdateSmbiosTables (
    IN  EFI_EVENT Event,
    IN  VOID      *Context
)
{
    EFI_STATUS       Status;
/*
#if PLATFORM_TYPE == 1
    EFI_EVENT        UpdateSmbiosType9TableEvent;
    VOID             *UpdateSmbiosType9TableRegistration;
#endif*/
    DEBUG(( DEBUG_INFO, "%a: Updating SMBIOS structures \n", __FUNCTION__ ));

    Status = pBS->LocateProtocol( &gEfiSmbiosProtocolGuid, NULL, &gEfiSmbiosProtocol);
    if (EFI_ERROR(Status))
        return;

    pBS->CloseEvent(Event);
/**
#if PLATFORM_TYPE == 1
    Status = pBS->LocateProtocol( &gEfiIioSystemProtocolGuid, NULL, &gIioSystemProtocol);
    if (EFI_ERROR(Status)) {
    //EFI callback for IIO system Protocol is created if it is not located
    Status = pBS->CreateEvent( EVT_NOTIFY_SIGNAL, TPL_CALLBACK, UpdateSmbiosType9Table, NULL, &UpdateSmbiosType9TableEvent);
        if (!EFI_ERROR(Status)) {
            Status = pBS->RegisterProtocolNotify( &gEfiIioSystemProtocolGuid, UpdateSmbiosType9TableEvent, &UpdateSmbiosType9TableRegistration);
            if (EFI_ERROR(Status))
                return;
        }
    return;
    }
#endif
*/
    //Update SMBIOS Type 9 and type 41 Structures
    Status = UpdateSmbiosType9Table();
    Status = UpdateSmbiosType41Table();
    return;
}
