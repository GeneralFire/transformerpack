//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file PciBoardDxeInit.c
  Pci Board Dxe Init driver
*/

#include <PciBoardDxeInit.h>

//Module global variables
BOOLEAN                         gDxeSmmReadyToLock = FALSE;
EFI_IIO_UDS_PROTOCOL            *gIioUds = NULL;
EFI_IIO_SYSTEM_PROTOCOL         *gIioSystemProtocol= NULL;
UINT8                           gNumberofStacksPerCpu = 0xFF;
BOOLEAN                         gFoundNoofSegments = FALSE;
UINT8                           gNumberOfAvailableSegments = 1;
SYSTEM_CONFIGURATION            *gSystemConfiguration = NULL;
SOCKET_MP_LINK_CONFIGURATION    gSocketMpLinkConfiguration;
BOOLEAN                         gFirstCallFlag = FALSE;
UINT8                           gNumOffboardVideoPresent;
BOOLEAN                         gMultiFunctionVga = FALSE;
UINT64                          *gAmiPcieBaseAddressArray=NULL;
UINTN                           gSizeOfBuffer = AMI_MAX_NUMBER_OF_PCI_SEGMENTS * sizeof(UINT64);
UINT64                          gBuffer[AMI_MAX_NUMBER_OF_PCI_SEGMENTS]; 
UINT8                           gBufferIndex = 0;
UINT8                           gTempBufferIndex = 0 ;
BOOLEAN                         gAtleastOneVideoFound = FALSE;
BOOLEAN                         gOnboardVideo = FALSE;
BOOLEAN                         gOffboardVideo = FALSE;
BOOLEAN                         gStackSocketMisMatch = FALSE;

EFI_GUID                        gSataGuidListCheckForRaid[] = { SATA_BUS_OVERRIDE_GUIDS_FOR_RAID {0} };
EFI_HANDLE                      gSataImageHandleArray[SATA_RAID_BUSOVERRIDE_MAX_HANDLES] = {NULL};
UINT8                           gSataGuidCount = 0;

// APTIOV_SERVER_OVERRIDE_START :   Revisit changes needed    
//EFI_GUID                        gsSataGuidListCheckForRaid[] = { SSATA_BUS_OVERRIDE_GUIDS_FOR_RAID {0} };
//EFI_HANDLE                      gsSataImageHandleArray[S_SATA_RAID_BUSOVERRIDE_MAX_HANDLES] = {NULL};
//UINT8                           gsSataGuidCount = 0;
// APTIOV_SERVER_OVERRIDE_END  :   Revisit changes needed    

// Instantiate AHCI_PLATFORM_POLICY_PROTOCOL with raid values 
// these values are taken from the gDefaultAhciPlatformPolicy in AhciBus.c
// The current Intel RAID driver can handle CD-ROM drives. So  AhciBusAtapiSupport
// is changed to FALSE to prevent the device from being installed twice.
AHCI_PLATFORM_POLICY_PROTOCOL        gRaidAhciPlatformPolicy = {
    FALSE,                              // Legacy Raid option selected 
    FALSE,                               // AhciBus driver handles the ATAPI devices
    FALSE,                              // Drive LED on ATAPI Enable (DLAE)
#ifdef POWERUP_IN_STANDBY_SUPPORT
    POWERUP_IN_STANDBY_SUPPORT,         // PowerUpInStandby feature is supported or not
#else
    FALSE,
#endif
#ifdef POWERUP_IN_STANDBY_MODE
    POWERUP_IN_STANDBY_MODE,            // PowerUpInStandby mode
#else
    FALSE,
#endif
#ifdef DiPM_SUPPORT
    DiPM_SUPPORT,                        // Device Initiated power management
#else
    FALSE,
#endif
#ifdef ENABLE_DIPM
    ENABLE_DIPM,
#else
    FALSE,
#endif
#ifdef DEVICE_SLEEP_SUPPORT
    DEVICE_SLEEP_SUPPORT,
#else
    FALSE,
#endif
#ifdef ENABLE_DEVICE_SLEEP
    ENABLE_DEVICE_SLEEP,
#else
    FALSE,
#endif
#ifdef USE_PCIIO_MAP_ADDRESS_FOR_DATA_TRANSFER
    USE_PCIIO_MAP_ADDRESS_FOR_DATA_TRANSFER
#else
    FALSE
#endif
};

#if CPU_SKX_ONLY_SUPPORT == 1
RB_INDEX_LOCATOR  gRbIndexLoc[] = {{"PC00",0}, {"PC01",1}, {"PC02",2}, {"PC03",3}, {"PC04",4}, {"PC05",5}, {"PC06",6}, 
                                    {"PC07",7}, {"PC08",8}, {"PC09",9}, {"PC0A",10}, {"PC0B",11}, {"PC0C",12}, {"PC0D",13}, 
                                    {"PC10",14}, {"PC11",15}, {"PC12",16}, {"PC13",17}, {"PC14",18}, {"PC15",19}, {"PC16",20}, 
                                    {"PC17",21}, {"PC18",22}, {"PC19",23}, {"PC2A",24}, {"PC2B",25}, {"PC2C",26}, {"PC2D",27}
#if MAX_SOCKET > 4      
                                    ,{"PC24",24}, {"PC25",25}, {"PC26",26}, {"PC27",27}, {"PC28",28}, {"PC29",29}
#endif
#if MAX_SOCKET > 5
                                    ,{"PC30",30}, {"PC31",31}, {"PC32",32}, {"PC33",33}, {"PC34",34}, {"PC35",35}
#endif
#if MAX_SOCKET > 6
                                    ,{"PC36",36}, {"PC37",37}, {"PC38",38}, {"PC39",39}, {"PC40",40}, {"PC41",41}
#endif
#if MAX_SOCKET > 7
                                    ,{"PC42",42}, {"PC43",43}, {"PC44",44}, {"PC45",45}, {"PC46",46}, {"PC47",47}
#endif
                                    };
#else
RB_INDEX_LOCATOR  gRbIndexLoc[] = {{"PC00",0}, {"PC01",1}, {"PC02",2}, {"PC03",3}, {"PC04",4}, {"PC05",5}, {"PC06",6}, 
                                   {"PC07",7}, {"PC08",8}, {"PC09",9}, {"PC0A",10}, {"PC0B",11}, {"UC0C",12},{"UC0D",13},
                                   {"PC10",14}, {"PC11",15}, {"PC12",16}, {"PC13",17}, {"PC14",18}, {"PC15",19}, {"PC16",20},
                                   {"PC17",21}, {"PC18",22}, {"PC19",23}, {"PC1A",24}, {"PC1B",25}, {"UC1C",26}, {"UC1D",27},
                                   {"PC20",28}, {"PC21",29}, {"PC22",30}, {"PC23",31}, {"PC24",32}, {"PC25",33}, {"PC26",34}, 
                                   {"PC27",35}, {"PC28",36}, {"PC29",37}, {"PC2A",38}, {"PC2B",39}, {"UC2C",40}, {"UC2D",41},
                                   {"PC30",42}, {"PC31",43}, {"PC32",44}, {"PC33",45}, {"PC34",46}, {"PC35",47}, {"PC36",48},
                                   {"PC37",49}, {"PC38",50}, {"PC39",51}, {"PC3A",52}, {"PC3B",53}, {"UC3C",54}, {"UC3D",55}
#if MAX_SOCKET > 4      
                                   ,{"PC40",56}, {"PC41",57}, {"PC42",58}, {"PC43",59}, {"PC44",60}, {"PC45",61}, {"PC46",62}, 
                                   {"PC47",63}, {"PC48",64}, {"PC49",65}, {"PC4A",66}, {"PC4B",67}, {"UC4C",68},{"UC4D",69},
                                   {"PC50",70}, {"PC51",71}, {"PC52",72}, {"PC53",73}, {"PC54",74}, {"PC55",75}, {"PC56",76},
                                   {"PC57",77}, {"PC58",78}, {"PC59",79}, {"PC5A",80}, {"PC5B",81}, {"UC5C",82}, {"UC5D",83},
                                   {"PC60",84}, {"PC61",85}, {"PC62",86}, {"PC63",87}, {"PC64",88}, {"PC65",89}, {"PC66",90}, 
                                   {"PC67",91}, {"PC68",92}, {"PC69",93}, {"PC6A",94}, {"PC6B",95}, {"UC6C",96}, {"UC6D",97},
                                   {"PC70",98}, {"PC71",99}, {"PC72",100}, {"PC73",101}, {"PC74",102}, {"PC75",103}, {"PC76",104},
                                   {"PC77",105}, {"PC78",106}, {"PC79",107}, {"PC7A",108}, {"PC7B",109}, {"UC7C",110}, {"UC7D",111}
#endif
                                    };
#endif

OFF_BOARD_VGA_DEVICES_TRIED gOffboardVideoDevices[MAX_NUMBER_OFFBOARD_VGA];

/**
    This function provide each initial routine in genericsio.c
    
    @param *This                Pointer to an instance of AMI_BOARD_INIT_PROTOCOL.
    @param *Function            Pointer to the function number from which it is called.
    @param *ParameterBlock      Contains a block of parameters of routine.

    @retval EFI_STATUS          EFI_SUCCESS       
                                EFI_UNSUPPORTED

**/
EFI_STATUS
EFIAPI
PciDevXXX_Init (
  IN      AMI_BOARD_INIT_PROTOCOL       *This,
  IN      UINTN                         *Function,
  IN OUT  VOID                          *ParameterBlock
)

{
    EFI_STATUS                           Status = EFI_UNSUPPORTED;
    AMI_BOARD_INIT_PARAMETER_BLOCK      *Args = (AMI_BOARD_INIT_PARAMETER_BLOCK*)ParameterBlock;
	PCI_INIT_STEP                       InitStep = (PCI_INIT_STEP)Args->InitStep;
    PCI_DEV_INFO                        *dev = (PCI_DEV_INFO*)Args->Param1;

    //Check if parameters passed are VALID and
    if(Args->Signature != AMI_PCI_PARAM_SIG) 
        return EFI_INVALID_PARAMETER;
    
    if(InitStep >= isPciMaxStep) 
        return EFI_INVALID_PARAMETER;
    
    if (dev == NULL) 
        return Status;
    
    switch (InitStep) {
        case isPciGetSetupConfig:
            DEBUG ((EFI_D_INFO,"%a:isPciGetSetupConfig\n", __FUNCTION__));
            if (dev->PciExpress != NULL) {
               if (dev->PciExpress->Pcie2 != NULL) {
// APTIOV_SERVER_OVERRIDE_RC_START : Coverity check for PCI_Porting_Template
                   AmiPciGetPcie2SetupDataOverrideHook (dev, &dev->PciExpress->Pcie2->Pcie2Setup, dev->AmiSdlPciDevData, dev->SdlDevIndex, FALSE);
               }
               AmiPciGetPcie1SetupDataOverrideHook (dev, &dev->PciExpress->Pcie1Setup, dev->AmiSdlPciDevData, dev->SdlDevIndex, FALSE);
            }
// APTIOV_SERVER_OVERRIDE_RC_END : Coverity check for PCI_Porting_Template			
            Status = AmiPciGetPciDevSetupDataOverrideHook (dev, &dev->DevSetup, dev->AmiSdlPciDevData, dev->SdlDevIndex, FALSE);
            break;

        case isPciSkipDevice:
            DEBUG ((EFI_D_INFO,"%a:isPciSkipDevice\n", __FUNCTION__));
            break;

        case isPciSetAttributes:
            DEBUG ((EFI_D_INFO,"%a:isPciSetAttributes\n", __FUNCTION__));
            break;

        case isPciProgramDevice:
            DEBUG ((EFI_D_INFO,"%a:isPciProgramDevice\n", __FUNCTION__));
            break;

        case isPcieSetAspm:
            DEBUG ((EFI_D_INFO,"%a:isPcieSetAspm\n", __FUNCTION__));
            break;

        case isPcieSetLinkSpeed:
            DEBUG ((EFI_D_INFO,"%a:isPcieSetLinkSpeed\n", __FUNCTION__));
            break;

        case isPciGetOptionRom:
            DEBUG ((EFI_D_INFO,"%a:isPciGetOptionRom\n", __FUNCTION__));
            break;

        case isPciOutOfResourcesCheck:
            DEBUG ((EFI_D_INFO,"%a:isPciOutOfResourcesCheck\n", __FUNCTION__));
            break;

        case isPciReadyToBoot:
            DEBUG ((EFI_D_INFO,"%a:isPciReadyToBoot\n", __FUNCTION__));
            break;

    }//switch

    return Status;
}

/**
    updates IIO UDS protocol in gIioUds.
    
    @param VOID

    @retval EFI_STATUS       

**/
EFI_STATUS
GetIioUdsStrcuture ()
{
    EFI_STATUS         Status = EFI_SUCCESS;

    Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid,
                                  NULL,
                                  &gIioUds);

    DEBUG ((DEBUG_INFO,"%a: Status = %x\n", __FUNCTION__ ,Status));
    ASSERT_EFI_ERROR (Status);

    return  Status;

}

#if CRB_OUT_OF_RESOURCE_SUPPORT
/**
    Check to see if this device uses this resource type
    
    @param *Device              Pointer to Device Data to check
    @param Type                 PCI BAR Type

    @retval BOOLEAN       

**/
BOOLEAN 
DoesDeviceUseMmio (
  IN  PCI_DEV_INFO          *Device,
  IN  PCI_BAR_TYPE          Type
)
{
    UINT8                   BarIndex;

    for (BarIndex = 0; BarIndex <= PCI_MAX_BAR_NO; BarIndex++) {
        if (Device->Bar[BarIndex].Type == Type) 
            return TRUE;
    }
    
    return FALSE;
}

/**
    Disable this device's Resources of the given type
    
    @param *Device              Pointer to Device Data to check
    @param Type                 PCI BAR Type

    @retval VOID       

**/
VOID 
DisableDevResourceType (
  IN  PCI_DEV_INFO              *Device,
  IN  PCI_BAR_TYPE              Type
)
{
    UINT8   BarIndex;

    for (BarIndex = 0; BarIndex <= PCI_MAX_BAR_NO; BarIndex++) {
        if (Device->Bar[BarIndex].Type == Type) {
            Device->Bar[BarIndex].Type = tBarUnused;
            Device->Bar[BarIndex].DiscoveredType = tBarUnused;
            Device->Bar[BarIndex].Base = 0;
            Device->Bar[BarIndex].Length = 0;
        }
    }
}

/**
    Disable this device's OpROM BAR
    
    @param *Device              Pointer to Device Data to check

    @retval VOID       

**/
VOID 
DisableDevOprom (
  IN  PCI_DEV_INFO          *Device
)
{
    UINT8   BarIndex;

    for (BarIndex = 0; BarIndex <= PCI_MAX_BAR_NO; BarIndex++) {
        if (Device->Bar[BarIndex].Offset == PCI_EXPANSION_ROM_BASE) {
            Device->Bar[BarIndex].Type = tBarUnused;
            Device->Bar[BarIndex].DiscoveredType = tBarUnused;
            Device->Bar[BarIndex].Base = 0;
            Device->Bar[BarIndex].Length = 0;
        }
    }
}

/**
    Execute passed in function on all devices behind this bridge.
    
    @param *Bridge              Pointer to Bridge Data
    @param Parser               Parsing Function
    @param *Context             Any data the parsing function needs

    @retval VOID       

**/
VOID 
ParseAllDevices(
  IN  PCI_BRG_INFO              *Bridge,
  IN  BOOLEAN		            *PciOutOfResHit,
  IN  DEVICE_PARSER             Parser
    )
{
    UINTN   Index;

    for (Index = 0; Index < Bridge->Bridge.ChildCount; Index++) {
        switch (Bridge->Bridge.ChildList[Index]->Type) {
            case  tPci2PciBrg:
            case  tPci2CrdBrg:
                //call recursively on next-level bridges
                ParseAllDevices ((PCI_BRG_INFO*)(Bridge->Bridge.ChildList[Index]),
                                PciOutOfResHit,
                                Parser);
            default:
                //call parse function on this device
                Parser (Bridge->Bridge.ChildList[Index],PciOutOfResHit);
        }
    }
}

/**
    Find Root Bridge of this device
    
    @param *Device              Pointer to device data

    @retval PCI_DEV_INFO        Pointer to Root bridge data  

**/
PCI_DEV_INFO *
GetRootBridge (
  IN  PCI_DEV_INFO      *Device
)
{
    if (Device->Type == tPciRootBrg) return Device;

    return GetRootBridge (Device->ParentBrg);

}

/**
    Returns the Number of Offboard video devices
    
    @param *Device              Pointer to device data
    @param *PciOutOfResHit      Pointer to out of resource condition

    @retval VOID  

**/
VOID 
GetTotalOffBoardVgaDevices (
  IN  PCI_DEV_INFO              *Device,
  IN  BOOLEAN	                *PciOutOfResHit
)
{
      EFI_STATUS                Status = EFI_SUCCESS;
      PCI_DEV_INFO              *ChildFunc;
      UINT8                     ChildFuncCnt;
      static UINT8              Count = 0 ;
      
      if ((Device->Class.BaseClassCode == PCI_CLASS_DISPLAY && Device->Class.SubClassCode == PCI_CLASS_DISPLAY_VGA) ||
          (Device->Class.BaseClassCode == PCI_CLASS_DISPLAY && Device->Class.SubClassCode == PCI_CLASS_DISPLAY_OTHER) ||
          (Device->Class.BaseClassCode == PCI_CLASS_MEDIA && Device->Class.SubClassCode == PCI_CLASS_DISPLAY_VGA)) {

          // Offboard Video devices
          if (!(Device->Attrib & EFI_PCI_IO_ATTRIBUTE_EMBEDDED_DEVICE)) {
              
              gOffboardVideoDevices[Count].VenId = Device->DevVenId.VenId;
              gOffboardVideoDevices[Count].DevId = Device->DevVenId.DevId;
              
              DEBUG ((EFI_D_INFO, "Offboard Video device information : VENDDORID = %x DEVICEID = %x \n", gOffboardVideoDevices[Count].VenId, gOffboardVideoDevices[Count].DevId));
              
              gBS->CopyMem(&gOffboardVideoDevices[Count].VgaDevice, Device, sizeof(PCI_DEV_INFO));

              // If This is a Multi-Function Device. Need to Disable all functions of this device.
              if ((Device->Func0 != NULL) && ((Device->FuncCount != 0) || (Device->FuncInitCnt != 0))) {
                  
                  gMultiFunctionVga = TRUE;
                  gOffboardVideoDevices[Count].MultiFunc = TRUE; 
                  gOffboardVideoDevices[Count].ChildFuncCount = Device->FuncCount;
                  
                  DEBUG ((EFI_D_INFO, "A Multi-Function Device with FuncCount = %xh \n", Device->FuncCount));
      
                  for (ChildFuncCnt = 0; ChildFuncCnt < Device->FuncCount; ChildFuncCnt++){
                      ChildFunc = Device->DevFunc[ChildFuncCnt];
                      gBS->CopyMem(&gOffboardVideoDevices[Count].ChildFunc[ChildFuncCnt], ChildFunc, sizeof(PCI_DEV_INFO));
                  }// Child Functions of VGA Device
              }// Multi-Function Device
            
              Count = Count + 1;
      
          }
      }
      
      gNumOffboardVideoPresent = Count;

}


/**
    Find VideoDevice on the corresponding IOH
    
    @param *Device              Pointer to device data

    @retval EFI_STATUS  

**/
EFI_STATUS
CheckVideoDevice (
  IN  PCI_DEV_INFO              *Device
)
{
    EFI_STATUS                  Status = EFI_SUCCESS;
    UINTN                       SocketNum;
    UINTN                       StackNum;
  
    SocketNum = gSocketMpLinkConfiguration.LegacyVgaSoc;
    StackNum = gSocketMpLinkConfiguration.LegacyVgaStack;

    if ((Device->Address.Addr.Bus >= gIioUds->IioUdsPtr->PlatformData.IIO_resource[SocketNum].StackRes[StackNum].BusBase) && 
        (Device->Address.Addr.Bus <= gIioUds->IioUdsPtr->PlatformData.IIO_resource[SocketNum].StackRes[StackNum].BusLimit)) {
        
        if (Device->Attrib & EFI_PCI_IO_ATTRIBUTE_EMBEDDED_DEVICE) {
            DEBUG ((EFI_D_INFO, "%a: Onboard Device\n", __FUNCTION__));
            gOnboardVideo = TRUE;
        }else {
            DEBUG ((EFI_D_INFO, "%a: Offboard Device\n", __FUNCTION__));
            gOffboardVideo = TRUE;
        }
      
        if (((gSystemConfiguration->VideoSelect == 1) && gOnboardVideo) || ((gSystemConfiguration->VideoSelect == AUTO) && (gNumOffboardVideoPresent == 0))) {    // Auto/Onboard
            gAtleastOneVideoFound = TRUE;
            DEBUG ((EFI_D_INFO, "VGA Selection in Setup is :: Onboard\n"));
            return EFI_UNSUPPORTED; // Do not skip the device
        }

        if ((gSystemConfiguration->VideoSelect == AUTO || gSystemConfiguration->VideoSelect == 2) && gOffboardVideo) {         // Offboard
            gAtleastOneVideoFound = TRUE;
            DEBUG ((EFI_D_INFO,"VGA Selection in Setup :: Offboard\n"));
            return EFI_UNSUPPORTED; // Do not skip the device
        }
    }else {
        gStackSocketMisMatch = TRUE ;
        DEBUG ((EFI_D_INFO, "VGA is not on the Selected Stack. Disabling it\n"));
        return EFI_SUCCESS; // SKIP the device
    }


    return Status;
}

/**
    Disable the current device's MMIO/IO if it is present and
    on a ROotBridge .
    
    @param *Device             Pointer to device data
    @param *PciOutOfResHit     Out of resource condition

    @retval VOID       

**/
VOID 
EnableDisableResources (
  IN  PCI_DEV_INFO              *Device,
  IN  BOOLEAN                   *PciOutOfResHit
)
{

      EFI_STATUS                Status = EFI_SUCCESS;
      EFI_STATUS                PciReadStatus;
      UINT8		                OffboardVideoIndex;
      PCI_DEV_INFO              *ChildFunc;
      UINT8                     ChildFuncCnt;
      UINT8                     ChildFns;
      UINT8                     BarIndex=0;
   
      PCI_BAR                   *VfBar;
      PCI_CFG_ADDR              addr;
      PCIE_EXT_CAP_HDR          extcap;
      BOOLEAN                   IsDeviceVga = FALSE;
      BOOLEAN                   SRIOVDevice = FALSE; 

      if ((Device->Class.BaseClassCode == PCI_CLASS_DISPLAY && Device->Class.SubClassCode == PCI_CLASS_DISPLAY_VGA) ||
          (Device->Class.BaseClassCode == PCI_CLASS_DISPLAY && Device->Class.SubClassCode == PCI_CLASS_DISPLAY_OTHER) ||
          (Device->Class.BaseClassCode == PCI_CLASS_MEDIA && Device->Class.SubClassCode == PCI_CLASS_DISPLAY_VGA)) {
          IsDeviceVga = TRUE;
       
      }
      
      if ((Device->Address.Addr.Bus == gIioUds->IioUdsPtr->PlatformData.IIO_resource[0].StackRes[0].BusBase) && 
          (!IsDeviceVga)) {
          DEBUG ((EFI_D_INFO, "\nCPU0 Stack0 onboard Devices won't be SKIPPED\n"));
          return;   // Do not skip the device
      }else {
          if (Device->Class.BaseClassCode == PCI_CLASS_BRIDGE ) {       // this is the bridge device
              DEBUG ((EFI_D_INFO, "\nBridge Device won't be SKIPPED\n"));
              return ;    // Do not skip the device
          }

          
          if (gAtleastOneVideoFound == TRUE) {
              if ((Device->Class.BaseClassCode == PCI_CLASS_DISPLAY && Device->Class.SubClassCode == PCI_CLASS_DISPLAY_VGA) ||
                  (Device->Class.BaseClassCode == PCI_CLASS_DISPLAY && Device->Class.SubClassCode == PCI_CLASS_DISPLAY_OTHER) ||
                  (Device->Class.BaseClassCode == PCI_CLASS_MEDIA && Device->Class.SubClassCode == PCI_CLASS_DISPLAY_VGA)) {
                  
                  Status = EFI_SUCCESS; // FLAG VGA Device for Disabling and SKIP THE VIDEO
                  
                
                  for (OffboardVideoIndex = 0; OffboardVideoIndex < gNumOffboardVideoPresent; OffboardVideoIndex++) {
                      if ((Device->DevVenId.VenId == gOffboardVideoDevices[OffboardVideoIndex].VenId) && 
                          (Device->DevVenId.DevId == gOffboardVideoDevices[OffboardVideoIndex].DevId && 
                          (gOffboardVideoDevices[OffboardVideoIndex].BarDisabled == FALSE))) {
                          
                          gOffboardVideoDevices[OffboardVideoIndex].BarDisabled = TRUE;
                         
                        
                          DEBUG ((EFI_D_INFO, "Disabling the Video device : VENDDORID = %x DEVICEID = %x BarDisabled:%d\n", Device->DevVenId.VenId, \
                                 Device->DevVenId.DevId, gOffboardVideoDevices[OffboardVideoIndex].BarDisabled));
                      }
                  }
              }
          }else {
              
              if ((Device->Class.BaseClassCode == PCI_CLASS_DISPLAY && Device->Class.SubClassCode == PCI_CLASS_DISPLAY_VGA) ||
                  (Device->Class.BaseClassCode == PCI_CLASS_DISPLAY && Device->Class.SubClassCode == PCI_CLASS_DISPLAY_OTHER) ||
                  (Device->Class.BaseClassCode == PCI_CLASS_MEDIA && Device->Class.SubClassCode == PCI_CLASS_DISPLAY_VGA)) {
                  
                  Status = CheckVideoDevice(Device);
                  
                  // If VGA selection is Offboard and even after enabling one offboard VGA card and disabled the other Offboard VGA cards
                  // System may not boot because the Enabled VGA card requests more resource.
                  // Skip VGAs if they are not from Chosen Stack
                  if (!gStackSocketMisMatch) {
                      
                      if (gOffboardVideo && (gNumOffboardVideoPresent > 1)) {
                          
                          for (OffboardVideoIndex = 0 ; OffboardVideoIndex < gNumOffboardVideoPresent ; OffboardVideoIndex++) {
                              
                              if ((Device->DevVenId.VenId == gOffboardVideoDevices[OffboardVideoIndex].VenId) && 
                                  (Device->DevVenId.DevId == gOffboardVideoDevices[OffboardVideoIndex].DevId)) {
                                  
                                  DEBUG ((EFI_D_INFO, "\nVideo device : VENDDORID = %x DEVICEID = %x\n", Device->DevVenId.VenId, Device->DevVenId.DevId));
                                  
                                  if (gOffboardVideoDevices[OffboardVideoIndex].Enabled == FALSE) {

                                      gOffboardVideoDevices[OffboardVideoIndex].Enabled = TRUE;

                                      if (gOffboardVideoDevices[OffboardVideoIndex].BarDisabled == TRUE) {
                                          gBS->CopyMem(Device, &gOffboardVideoDevices[OffboardVideoIndex].VgaDevice, sizeof(PCI_DEV_INFO));
                                          gOffboardVideoDevices[OffboardVideoIndex].BarDisabled = FALSE;
                                          
                                          if (gOffboardVideoDevices[OffboardVideoIndex].MultiFunc == TRUE) {
                                              // If Multi-Function: Need to enable Child Functions as well. For now only 2 child functions are considered.
                                              DEBUG ((EFI_D_INFO, "A Multi-Function Device with FuncCount = %xh\n", Device->FuncCount));
                                              for (ChildFuncCnt=0; ChildFuncCnt < Device->FuncCount; ChildFuncCnt++) {
                                                  ChildFunc = Device->DevFunc[ChildFuncCnt];
                                                  DEBUG ((EFI_D_INFO, "Child Fn: OffboardVideoDevice: Address:%x DevFunc:%x\n", gOffboardVideoDevices[OffboardVideoIndex].VgaDevice.DevFunc[ChildFuncCnt], ChildFunc));
                                                  gBS->CopyMem (Device->DevFunc[ChildFuncCnt], &gOffboardVideoDevices[OffboardVideoIndex].ChildFunc[ChildFuncCnt], sizeof(PCI_DEV_INFO));   
                                              }
                                          }
                                          
                                          break;
                                      }
                                      DEBUG ((EFI_D_INFO, "Exit 1 : VGA[%d] VISIT 1", OffboardVideoIndex));
                                      break;
                                  }
                                  
                                  if ((gOffboardVideoDevices[OffboardVideoIndex].Enabled == TRUE) && (!gOffboardVideoDevices[OffboardVideoIndex].VgaTriedFlag)){
                                      
                                      gAtleastOneVideoFound = FALSE;
                                      gOffboardVideoDevices[OffboardVideoIndex].VgaTriedFlag = TRUE;
                                      Status = EFI_SUCCESS; 
                                      DEBUG ((EFI_D_INFO, "Exit 2: VGA[%d] Tried and Disabled gAtleastOneVideoFound:%d\n", OffboardVideoIndex, gAtleastOneVideoFound));
                                      break;
                                  }
                                  DEBUG ((EFI_D_INFO, "Exit 3: VGA[%d] Tried and Disabled VISIT 3 gAtleastOneVideoFound:%d\n", OffboardVideoIndex, gAtleastOneVideoFound));
                              }
                          }
                          
                          if (OffboardVideoIndex >= gNumOffboardVideoPresent) {
                     
                          #if (defined(AMI_CRB_OOR_FORCE_DISABLE_VGA) && (AMI_CRB_OOR_FORCE_DISABLE_VGA == 1))
                              DEBUG ((EFI_D_INFO, "Multiple VGAs: AMI_CRB_OOR_FORCE_DISABLE_VGA is Enabled. Force Disabling VGA to continue Booting.\n"));
                              Status = EFI_SUCCESS;
                              goto  DeviceDisable;
                        #endif
                              DEBUG ((EFI_D_INFO, "Multiple VGAs: Not Able to Disable VIDEO CARDs which requested more resources VENDDORID = %x DEVICEID = %x\n", Device->DevVenId.VenId, Device->DevVenId.DevId));
                              ERROR_CODE (DXE_PCI_BUS_OUT_OF_RESOURCES,EFI_ERROR_MAJOR);
                              EFI_DEADLOOP ();// NOT ABLE TO DISABLE VIDEO CARDs which requested more IO. 
                          }

                      }else{
                          if(gOffboardVideoDevices[0].Enabled == FALSE){
                              DEBUG ((EFI_D_INFO, "ONE VGA: VENDDORID = %x DEVICEID = %x; Enabling  and Trying  \n", Device->DevVenId.VenId, Device->DevVenId.DevId));
                              gOffboardVideoDevices[0].Enabled = TRUE; // Enable this VGA once and see if System Boots
                          }else {

                        #if (defined(AMI_CRB_OOR_FORCE_DISABLE_VGA) && (AMI_CRB_OOR_FORCE_DISABLE_VGA == 1))
                              DEBUG ((EFI_D_INFO, "ONE VGA: AMI_CRB_OOR_FORCE_DISABLE_VGA is Enabled. Force Disabling VGA to continue Booting.\n"));
                              Status = EFI_SUCCESS;
                              goto  DeviceDisable;
                       #endif
                              DEBUG ((EFI_D_INFO, "ONE VGA: Not Able to Disable VIDEO CARD which requested more resources VENDDORID = %x DEVICEID = %x\n", Device->DevVenId.VenId, Device->DevVenId.DevId));
                              ERROR_CODE (DXE_PCI_BUS_OUT_OF_RESOURCES,EFI_ERROR_MAJOR);
                              EFI_DEADLOOP ();// NOT ABLE TO DISABLE VIDEO CARDs which requested more IO.
                          }
                      }
                  }
                  
              }
              
          }  //else {
    }  //else {

    #if (defined(AMI_CRB_OOR_FORCE_DISABLE_VGA) && (AMI_CRB_OOR_FORCE_DISABLE_VGA == 1))
    DeviceDisable:
    #endif
    if (Status != EFI_SUCCESS) {
        DEBUG ((EFI_D_INFO, "Enabling the PCI device : VENDDORID = %x DEVICEID = %x\n", Device->DevVenId.VenId, Device->DevVenId.DevId));
        return;
      }
      
      // In Case of Multi Function VGA, Once VGA is enabled, all its child function devices should be 
      // skipped from Disabling.
      if (gMultiFunctionVga) {
          for (OffboardVideoIndex = 0; OffboardVideoIndex < gNumOffboardVideoPresent; OffboardVideoIndex++) {
              if (gOffboardVideoDevices[OffboardVideoIndex].MultiFunc) {
                  for (ChildFns = 0 ; ChildFns < gOffboardVideoDevices[OffboardVideoIndex].ChildFuncCount; ChildFns++) {
                      if ((Device->DevVenId.VenId == gOffboardVideoDevices[OffboardVideoIndex].ChildFunc[ChildFns].DevVenId.VenId) && 
                          (Device->DevVenId.DevId == gOffboardVideoDevices[OffboardVideoIndex].ChildFunc[ChildFns].DevVenId.DevId)) {
                          if (!gOffboardVideoDevices[OffboardVideoIndex].BarDisabled) {
                              DEBUG ((EFI_D_INFO, "Skipping the PCI device :\nBUS = %x DEVICE = %x FUNCTION = %x", Device->Address.Addr.Bus, Device->Address.Addr.Device, Device->Address.Addr.Function));
                              return ;
                          }
                          
                      }
                  }
              }
          }
      }
      
      // Does this device use  MMIO/IO?
      if (DoesDeviceUseMmio(Device, tBarMmio32) || DoesDeviceUseMmio(Device, tBarMmio32pf)||
          DoesDeviceUseMmio(Device, tBarIo16)|| DoesDeviceUseMmio(Device, tBarIo32)) {
          
          // yes, disable its  MMIO/IO
          DEBUG ((EFI_D_INFO, "Disabling the PCI device : BUS = %x DEVICE = %x FUNCTION = %x", Device->Address.Addr.Bus, Device->Address.Addr.Device, Device->Address.Addr.Function));
          DEBUG ((EFI_D_INFO, "VENDDORID = %x DEVICEID = %x\n", Device->DevVenId.VenId, Device->DevVenId.DevId));
          for(BarIndex=0; BarIndex<=PCI_MAX_BAR_NO; BarIndex++){
              if((Device->Bar[BarIndex].Type>0) && (Device->Bar[BarIndex].Length>0)){
                  DEBUG((EFI_D_INFO,"BarTypes Fn0: BAR Index=%d;\tType=%d;\tGRA=0x%lX;\tLEN=0x%lX;\tOffset=0x%X;\n", 
                      BarIndex, Device->Bar[BarIndex].Type, Device->Bar[BarIndex].Gran, 
                      Device->Bar[BarIndex].Length, Device->Bar[BarIndex].Offset));
              }
          }
          DisableDevResourceType (Device ,tBarIo16);
          DisableDevResourceType (Device ,tBarIo32);
          DisableDevResourceType (Device ,tBarMmio32);
          DisableDevResourceType (Device ,tBarMmio32pf);
          if(Device->Bar[PCI_MAX_BAR_NO].Offset){
              Device->Bar[PCI_MAX_BAR_NO].Offset = 0;
              if ( Device->Attrib & EFI_PCI_IO_ATTRIBUTE_EMBEDDED_DEVICE ) {
                  // Set OutOfResRemove only for OnBoard Video Device
                  Device->OutOfResRemove = TRUE;
              }
          }
          //
          // handling SRIOV Devices.
          //
            if( gSystemConfiguration->SRIOVEnable ){
                addr.ADDR=Device->Address.ADDR;
                //Check Pci Express Extended Capability header
                addr.Addr.ExtendedRegister=0x100;
                
                while(addr.Addr.ExtendedRegister){
                    PciReadStatus=PciCfg32(Device->RbIo,addr,FALSE,&extcap.EXT_CAP_HDR);
                    ASSERT_EFI_ERROR(PciReadStatus);
                    
                    if (extcap.ExtCapId==PCIE_CAP_ID_SRIOV ){
                        SRIOVDevice = TRUE;
                        DEBUG((EFI_D_INFO,"It is SRIOV Device \n"));
                        break;
                    }
                    addr.Addr.ExtendedRegister=extcap.NextItemPtr;
                }
                
                if(SRIOVDevice){
                    
                    VfBar=&Device->PciExpress->SriovData->Bar[0];
                    
                    for( BarIndex=0; BarIndex<PCI_MAX_BAR_NO; BarIndex++){
                       
                        if ((VfBar[BarIndex].Type >= tBarIo16 ) && VfBar[BarIndex].Length){
                          DEBUG((EFI_D_INFO,"SRIOVHandling BarType:%d\n",BarIndex));
                          Device->Bar[BarIndex].Type = tBarUnused;
                          Device->Bar[BarIndex].DiscoveredType = tBarUnused;
                          Device->Bar[BarIndex].Base = 0;
                          Device->Bar[BarIndex].Length = 0;
                          
                          }
                        }// bar Loop
                        
                        pBS->SetMem(&Device->PciExpress->SriovData->Bar[0], (PCI_MAX_BAR_NO * sizeof(PCI_BAR)),0);

                        for(BarIndex=0; BarIndex<=PCI_MAX_BAR_NO; BarIndex++){
                        DEBUG((EFI_D_INFO," BarTypes After SRIOVHandling BAR Index=%d;\tType=%d;\tGRA=0x%lX;\tLEN=0x%lX;\tOffset=0x%X;\n", 
                        BarIndex, Device->Bar[BarIndex].Type, Device->Bar[BarIndex].Gran, 
                        Device->Bar[BarIndex].Length, Device->Bar[BarIndex].Offset));
                        }
                        
                    }
                SRIOVDevice = FALSE;
            }

          // If This is a Multi-Function Device. Need to Disable all functions of this device.
          if ((Device->Func0 != NULL) && ((Device->FuncCount != 0) || (Device->FuncInitCnt != 0))) {
              
              DEBUG ((EFI_D_INFO, "A Multi-Function Device with FuncCount = %xh\n", Device->FuncCount));
              
              for(ChildFuncCnt = 0; ChildFuncCnt < Device->FuncCount; ChildFuncCnt++) {
                  ChildFunc = Device->DevFunc[ChildFuncCnt];
                  
                  for(BarIndex=0; BarIndex<=PCI_MAX_BAR_NO; BarIndex++){
                      if((Device->Bar[BarIndex].Type >= tBarIo16) && (Device->Bar[BarIndex].Length>0)){
                          DEBUG((EFI_D_INFO,"BarTypes: BAR Index=%d;\tType=%d;\tGRA=0x%lX;\tLEN=0x%lX;\tOffset=0x%X;\n", 
                             BarIndex, Device->Bar[BarIndex].Type, Device->Bar[BarIndex].Gran, 
                              Device->Bar[BarIndex].Length, Device->Bar[BarIndex].Offset));
                      }
                  }
                  
                  // Does this device use  MMIO/IO?
                  if (DoesDeviceUseMmio(ChildFunc, tBarMmio32) || DoesDeviceUseMmio(ChildFunc, tBarMmio32pf)||
                      DoesDeviceUseMmio(ChildFunc, tBarIo16)|| DoesDeviceUseMmio(ChildFunc, tBarIo32)) {
                      
                      // yes, disable its  MMIO/IO
                      DEBUG ((EFI_D_INFO, "Disabling the Child Function : BUS = %x DEVICE = %x FUNCTION = %x \n", ChildFunc->Address.Addr.Bus, ChildFunc->Address.Addr.Device, ChildFunc->Address.Addr.Function));
                      DEBUG ((EFI_D_INFO, "VENDDORID = %x DEVICEID = %x\n", ChildFunc->DevVenId.VenId, ChildFunc->DevVenId.DevId));
                      DisableDevResourceType (ChildFunc,tBarIo16);
                      DisableDevResourceType (ChildFunc,tBarIo32);
                      DisableDevResourceType (ChildFunc,tBarMmio32);
                      DisableDevResourceType (ChildFunc,tBarMmio32pf);
                     // handling SRIOV Devices.
                      if( gSystemConfiguration->SRIOVEnable ){
                          
                          addr.ADDR=Device->Address.ADDR;
                          //Check Pci Express Extended Capability header
                          addr.Addr.ExtendedRegister=0x100;
                          
                          while(addr.Addr.ExtendedRegister){
                              PciReadStatus=PciCfg32(Device->RbIo,addr,FALSE,&extcap.EXT_CAP_HDR);
                              ASSERT_EFI_ERROR(PciReadStatus);
                              
                              if (extcap.ExtCapId==PCIE_CAP_ID_SRIOV ){
                                  SRIOVDevice = TRUE;
                                  DEBUG((EFI_D_INFO,"It is SRIOV Child Devices \n"));
                                  break;
                              }
                              addr.Addr.ExtendedRegister=extcap.NextItemPtr;
                          }
                          if(SRIOVDevice){
                              
                              VfBar=&ChildFunc->PciExpress->SriovData->Bar[0];
                              for( BarIndex=0; BarIndex<PCI_MAX_BAR_NO; BarIndex++){
                                  if ((VfBar[BarIndex].Type >= tBarIo16 ) && VfBar[BarIndex].Length){
                                      DEBUG((EFI_D_INFO,"SRIOVHandling BarType:%d\n",BarIndex));
                                      ChildFunc->Bar[BarIndex].Type = tBarUnused;
                                      ChildFunc->Bar[BarIndex].DiscoveredType = tBarUnused;
                                      ChildFunc->Bar[BarIndex].Base = 0;
                                      ChildFunc->Bar[BarIndex].Length = 0;
                                  }
                              }// bar Loop
                              pBS->SetMem(&ChildFunc->PciExpress->SriovData->Bar[0], (PCI_MAX_BAR_NO * sizeof(PCI_BAR)),0);
                          }
                          SRIOVDevice = FALSE;
                      }
                  }
              }
          }
          *PciOutOfResHit = TRUE;
      }

}

/**
    If there was a resource overflow, implement the following
    
    @param *Device             Current device (not necessarily problematic device)
    @param *PciOutOfResHit     Out of resource condition

    @retval EFI_STATUS          EFI_SUCCESS       
                                EFI_UNSUPPORTED

**/
EFI_STATUS 
EnableDisableDevices (
  IN  PCI_DEV_INFO              *Device,
  IN  BOOLEAN                   *PciOutOfResHit
)
{
    EFI_STATUS                  Status;
    PCI_DEV_INFO                *RootBridge;
    EFI_HANDLE                  *Handle = NULL;
    UINTN                       SocketVariableSize = sizeof(SOCKET_MP_LINK_CONFIGURATION);
    UINT32                      SocketVariableAttr;

    RootBridge = GetRootBridge(Device);
    
    Status = gRT->GetVariable (SOCKET_MP_LINK_CONFIGURATION_NAME,
                               &gEfiSocketMpLinkVariableGuid,
                               &SocketVariableAttr,
                               &SocketVariableSize,
                               &gSocketMpLinkConfiguration);
    
    if (EFI_ERROR(Status)) {
        DEBUG ((EFI_D_ERROR,"PciBoardDxeInit.c SOCKET_MP_LINK_CONFIGURATION_NAME : Getvariable failed\n" ));
        EFI_DEADLOOP ();
    }

    ASSERT_EFI_ERROR (Status);

    // execute MMIO/IO function on all devices behind this root bridge.
    // if more than One Offboard VGA cards . Try One VGA Card at a time and see if System Boots.
    // Halt the System if Enabled VGA is requesting more resources.
    if(!gFirstCallFlag){
        // Number of VGA Devices wont change over different calls of CrbDefinedTable. So, Checking Number of VGAs only on First Call to CrbDefinedTable
        DEBUG ((EFI_D_INFO,"GetTotalOffBoardVgaDevices: START\n" ));
        ParseAllDevices ((PCI_BRG_INFO*)RootBridge, PciOutOfResHit, GetTotalOffBoardVgaDevices);
        gFirstCallFlag = TRUE;
        DEBUG ((EFI_D_INFO,"GetTotalOffBoardVgaDevices: gNumOffboardVideoPresent:%d END\n", gNumOffboardVideoPresent));
    }
    
    ParseAllDevices ((PCI_BRG_INFO*)RootBridge, PciOutOfResHit, EnableDisableResources);
    
    return EFI_UNSUPPORTED;

}

/**
    Reading the Setup Data
    
    @param VOID

    @retval EFI_STATUS       

**/
EFI_STATUS
GetSetupData ()
{
    UINTN               SetupDataVarSize = sizeof(SYSTEM_CONFIGURATION);
    EFI_STATUS          Status = EFI_OUT_OF_RESOURCES;

    gSystemConfiguration = AllocateZeroPool (SetupDataVarSize);
    
    if (gSystemConfiguration != NULL) {
        Status = gRT->GetVariable (L"IntelSetup",
                                   &gEfiSetupVariableGuid,
                                   NULL,
                                   &SetupDataVarSize,
                                   gSystemConfiguration);

        ASSERT_EFI_ERROR (Status);
    }

    return Status;

}

/**
    If there was a resource overflow, implement the following
    
    @param *Device             Current device (not necessarily problematic device)
    @param Count               Count of already disabled devices
    @param LowResType          MRES_TYPE - type of resource causing overflow
    @param *PciOutOfResHit     Out of resource condition

    @retval EFI_STATUS          EFI_SUCCESS       
                                EFI_UNSUPPORTED

**/
EFI_STATUS 
CrbDefinedPciDeviceTable (
  IN  PCI_DEV_INFO              *Device,
  IN  UINTN                     Count,
  IN  UINTN                     LowResType,
  IN  BOOLEAN                   **PciOutOfResHit
    )
{
    EFI_STATUS                  Status = EFI_UNSUPPORTED;
    
    if (gSystemConfiguration == NULL) {
        Status  = GetSetupData ();
        if (EFI_ERROR(Status))
            return Status;
    }

    if (gIioUds == NULL) {
        Status = GetIioUdsStrcuture ();
        if (EFI_ERROR(Status))
            return Status;
    }
    
    DEBUG ((EFI_D_INFO,"In CrbDefinedPciDeviceTable(): LowResType : %lx \n", LowResType));
    
    Status =  EnableDisableDevices (Device,*PciOutOfResHit);
    gAtleastOneVideoFound = FALSE;
    
    return Status;
}
#endif

/**
    Get Iio Root Port Index
    
    @param *Dev                 Pointer to an instance of PCI_DEV_INFO.
    @param *SocketIndex         Pointer to the SocketIndex Variable.
    @param *RootPortIndex       Pointer to Root Port Index Variable.

    @retval EFI_STATUS          EFI_SUCCESS       
                                EFI_UNSUPPORTED

**/

EFI_STATUS
EFIAPI
GetIioRootPortIndex (
  IN PCI_DEV_INFO   *Dev,
  IN OUT UINT8      *SocketIndex,
  IN OUT UINTN      *RootPortIndex
  )
{
    PCI_ROOT_BRG_DATA       *RootBridge=(PCI_ROOT_BRG_DATA*)(Dev->ParentBrg);
    UINTN                   PortIndex = 0;
    UINT8                   Socket = 0;

    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
        if (!gIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Valid) 
        {
            continue;
        }

        if (RootBridge->RbIoProtocol.SegmentNumber != gIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].PcieSegment) {
            continue;
            
        }
        for (PortIndex = 0; PortIndex < NUMBER_PORTS_PER_SOCKET; PortIndex++) {
            if ((Dev->Address.Addr.Bus == gIioSystemProtocol->IioGlobalData->IioVar.IioVData.SocketPortBusNumber[Socket][PortIndex]) &&
                    (Dev->Address.Addr.Device == gIioSystemProtocol->IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Device) &&
                    (Dev->Address.Addr.Function == gIioSystemProtocol->IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Function)) {
                *SocketIndex = Socket;
				*RootPortIndex = PortIndex;
                return EFI_SUCCESS;
            }
        }
    }

    return EFI_NOT_FOUND;
}


/**
    Initialize PCIe root ports
    
    @param *This                Pointer to an instance of AMI_BOARD_INIT_PROTOCOL.
    @param *Function            Pointer to the function number from which it is called.
    @param *ParameterBlock      Contains a block of parameters of routine.

    @retval EFI_STATUS          EFI_SUCCESS       
                                EFI_UNSUPPORTED

**/
EFI_STATUS 
EFIAPI
PcieRootPortInitCallback (
  IN      AMI_BOARD_INIT_PROTOCOL       *This,
  IN      UINTN                         *Function,
  IN OUT  VOID                          *ParameterBlock
)
{
    EFI_STATUS                          Status;
    AMI_BOARD_INIT_PARAMETER_BLOCK      *Args = (AMI_BOARD_INIT_PARAMETER_BLOCK*)ParameterBlock;
    PCI_INIT_STEP                       InitStep = (PCI_INIT_STEP)Args->InitStep;
    PCI_DEV_INFO                        *dev = (PCI_DEV_INFO*)Args->Param1;
    EFI_PCI_IO_PROTOCOL                 *PciIo = &dev->PciIo;       //PCI IO Protocol Instance
    UINT8                               SocketIndex;
    UINTN                               IioRootPortIndex;
#if CRB_OUT_OF_RESOURCE_SUPPORT		
    UINTN                               *Count;
    UINTN                               *LowResType;
    BOOLEAN                             *PciOutOfResHit;
    SOCKET_COMMONRC_CONFIGURATION       SocketCommonRcConfiguration;
    UINT32                              CrcAttributes;
    UINTN                               VarSize;
#endif

    if (Args->Signature != AMI_PCI_PARAM_SIG) 
        return EFI_INVALID_PARAMETER;
    
    if (InitStep>=isPciMaxStep) 
        return EFI_INVALID_PARAMETER;
    if ( gIioSystemProtocol == NULL ) {
        Status = pBS->LocateProtocol( \
                &gEfiIioSystemProtocolGuid, \
                NULL, \
                &gIioSystemProtocol);
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR(Status))
            return Status;
    }

    switch (InitStep) {
        case isPciOutOfResourcesCheck:
            
            DEBUG ((EFI_D_INFO,"isPciOutOfResourcesCheck \n"));
            
#if CRB_OUT_OF_RESOURCE_SUPPORT	
            Count = (UINTN*)Args->Param2;
            LowResType = (UINTN*)Args->Param3;
            PciOutOfResHit = (BOOLEAN*)Args->Param4;
            
            if( *LowResType > 4 ){
                if (0) {
                    DEBUG ((EFI_D_INFO,"\nMMIOH OOR happened and is not Handled in CRB \n"));
                    DEBUG ((EFI_D_INFO,"\n SYSTEM is HALTED \n"));
                    ERROR_CODE (DXE_PCI_BUS_OUT_OF_RESOURCES,EFI_ERROR_MAJOR);
                    EFI_DEADLOOP ();
                } else {
                    Status = gRT->GetVariable (
                            SOCKET_COMMONRC_CONFIGURATION_NAME,
                            &gEfiSocketCommonRcVariableGuid,
                            &CrcAttributes,
                            &VarSize,
                            &SocketCommonRcConfiguration
                            );
                    ASSERT_EFI_ERROR (Status);
                    
                    DEBUG ((EFI_D_INFO,"\nMMIOH OOR happened!! MmiohSize = %d GB \n", (1 << (2 * SocketCommonRcConfiguration.MmiohSize))));

                    if (SocketCommonRcConfiguration.MmiohSize == 5) {
                        DEBUG ((EFI_D_INFO,"\n The MAX MmiohSize is 1024GB, cannot be enlarged anymore!!\n"));
                        DEBUG ((EFI_D_INFO," SYSTEM is HALTED \n"));
                        ERROR_CODE (DXE_PCI_BUS_OUT_OF_RESOURCES,EFI_ERROR_MAJOR);
                        EFI_DEADLOOP ();
                    }
                    
                    SocketCommonRcConfiguration.MmiohSize++;
                    
                    Status = gRT->SetVariable (
                            SOCKET_COMMONRC_CONFIGURATION_NAME,
                            &gEfiSocketCommonRcVariableGuid,
                            CrcAttributes,
                            sizeof (SOCKET_COMMONRC_CONFIGURATION),
                            &SocketCommonRcConfiguration
                            );
                    ASSERT_EFI_ERROR (Status);
                    DEBUG ((EFI_D_INFO,"\nMMIOH is not enough, enlarging MmiohSize to %d GB!!\n", (1 << (2 * SocketCommonRcConfiguration.MmiohSize))));
                    DEBUG ((EFI_D_INFO,"Issuing a cold reset.\n"));
                    gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
                    EFI_DEADLOOP ();
                }
            }
                
            Status = CrbDefinedPciDeviceTable (dev,*Count,*LowResType,&PciOutOfResHit);
# else	
            Status = EFI_UNSUPPORTED;
#endif						
            break;  //case isPciOutOfResourcesCheck:
            
            case isPciGetSetupConfig:
                DEBUG((EFI_D_INFO," (isPciGetSetupConfig); " ));

                Status = GetIioRootPortIndex (dev, &SocketIndex, &IioRootPortIndex);
                if (!EFI_ERROR(Status) && (dev->PciExpress != NULL) ) {
                    UINT8   MaxPayload = gIioSystemProtocol->IioGlobalData->SetupData.IioPcieConfig.PcieMaxPayload[(SocketIndex * NUMBER_PORTS_PER_SOCKET) + IioRootPortIndex];

                    // Map RC MaxPayload to AMI MaxPayload Setup question
                    // IIO RC Options: 0 - 128, 1 - 256, 2 - Auto
                    // AMI RC Options: 0 - 128, 1 - 256, 55 - Auto
                    if ( MaxPayload == 2 ) {
                        MaxPayload = PCI_SETUP_AUTO_VALUE;
                    }
                    dev->PciExpress->Pcie1Setup.MaxPayload = MaxPayload;
                    Status = EFI_SUCCESS;
                } else {
                    Status=EFI_UNSUPPORTED;
                }
                break;
        default:
            Status = EFI_UNSUPPORTED;
            DEBUG ((EFI_D_INFO," (!!!isPciMaxStep!!!); " ));
    }//switch
    
    return Status;
}

/**
    updates available segment in gNumberOfAvailableSegments.
    
    @param VOID

    @retval VOID       

**/
VOID 
GetNumberofSegements()
{

    UINT8 SocketIndex;

    for (SocketIndex = 1 ; SocketIndex < gIioUds->IioUdsPtr->SystemStatus.numCpus ; SocketIndex++) {
        if (gIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[SocketIndex].PcieSegment != gIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[SocketIndex -1].PcieSegment) {
            gNumberOfAvailableSegments = gNumberOfAvailableSegments + 1;
        }
    }
    
    DEBUG ((EFI_D_INFO,"%a() gNumberOfAvailableSegments:0x%X\n", __FUNCTION__, gNumberOfAvailableSegments));

}


/**
    Initialize root bridge.
    
    @param *This                Pointer to an instance of AMI_BOARD_INIT_PROTOCOL.
    @param *Function            Pointer to the function number from which it is called.
    @param *ParameterBlock      Contains a block of parameters of routine.

    @retval EFI_STATUS          EFI_SUCCESS       
                                EFI_UNSUPPORTED

**/
EFI_STATUS
EFIAPI
RootBrgInit (
  IN      AMI_BOARD_INIT_PROTOCOL       *This,
  IN      UINTN                         *Function,
  IN OUT  VOID                          *ParameterBlock
)
{
    EFI_STATUS                          Status = EFI_UNSUPPORTED;    
    AMI_BOARD_INIT_PARAMETER_BLOCK      *Args = (AMI_BOARD_INIT_PARAMETER_BLOCK*)ParameterBlock;
    PCI_INIT_STEP                       InitStep = (PCI_INIT_STEP)Args->InitStep;
    PCI_ROOT_BRG_DATA                   *rb = (PCI_ROOT_BRG_DATA*)Args->Param1;
    ASLR_QWORD_ASD                      *res = (ASLR_QWORD_ASD*)Args->Param2;
    UINTN                               StackIndex = 0, SocketIndex = 0;
    UINTN                               StackResIndex = 0, SocketResIndex = 0;
    BOOLEAN                             RbAslNameMatchFound = FALSE;
    UINTN                               Index;
    static UINTN                        NumberofRb = 0;
    UINT8                               NumberOfSegments = AMI_MAX_NUMBER_OF_PCI_SEGMENTS;
    
    if (gIioUds == NULL) {
        Status = GetIioUdsStrcuture ();
        if (EFI_ERROR(Status))
            return EFI_UNSUPPORTED;
    }
                
    DEBUG((DEBUG_INFO,"\n%a() ENTRY (InitStep:0x%X)\n",__FUNCTION__, InitStep));
    if (NumberofRb == 0) {
        for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
            if (!gIioUds->IioUdsPtr->PlatformData.IIO_resource[SocketIndex].Valid) {
                continue;
            }
            for (StackIndex = 0; StackIndex < MAX_LOGIC_IIO_STACK; StackIndex++) {
                if (!((gIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[SocketIndex].stackPresentBitmap) & (1 << StackIndex))) {
                    continue;
                }
                NumberofRb++;
            } // for loop for Stack
        } // for loop for Socket

        gNumberofStacksPerCpu = (UINT8)(NumberofRb / gIioUds->IioUdsPtr->SystemStatus.numCpus);
        DEBUG((DEBUG_INFO,"NumberofRb:%d; gNumberofStacksPerCpu:%d\n", NumberofRb, gNumberofStacksPerCpu));
    }
    if (!gFoundNoofSegments) {
        GetNumberofSegements();
        gFoundNoofSegments = TRUE;
    }
    for (Index =0; Index < MAX_SOCKET*MAX_LOGIC_IIO_STACK; Index++) { //EGS has 14 root bridges under each Socket/IIO
        if (Strcmp(rb->RbAslName, gRbIndexLoc[Index].RbAslDevName) == 0 ) {
            StackResIndex = gRbIndexLoc[Index].Index % MAX_LOGIC_IIO_STACK; //within each IIO socket ID, the stack resources are indexed 0 to 5.
            SocketResIndex = gRbIndexLoc[Index].Index / MAX_LOGIC_IIO_STACK;
            RbAslNameMatchFound = TRUE;
            
            DEBUG((DEBUG_INFO,"SocketResIndex:%d; StackResIndex:%d of Current Root Bridge\n", SocketResIndex, StackResIndex));
            break;
        }
    }

    switch (InitStep) {
        case isRbCheckPresence:
            DEBUG ((DEBUG_INFO,"isRbCheckPresence\n"));

            if ( RbAslNameMatchFound && (gIioUds->IioUdsPtr->PlatformData.IIO_resource[SocketResIndex].Valid) && 
                 ((gIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[SocketResIndex].stackPresentBitmap) & (1 << StackResIndex)) ) {

                DEBUG ((DEBUG_INFO,"stackPresentBitmap:0x%X\n", gIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[SocketResIndex].stackPresentBitmap));
                DEBUG ((DEBUG_INFO,"gIioUds->IioUdsPtr->PlatformData.PciExpressSize:0x%X\n", gIioUds->IioUdsPtr->PlatformData.PciExpressSize));
                rb->NotPresent = FALSE;
                      
                if (((gIioUds->IioUdsPtr->PlatformData.PciExpressSize/1024/1024)) > 256 ) {
                    gBufferIndex = gTempBufferIndex / gNumberofStacksPerCpu;
                    gBufferIndex = gBufferIndex / (gIioUds->IioUdsPtr->SystemStatus.numCpus / gNumberOfAvailableSegments);
                    // APTIOV_SERVER_OVERRIDE_RC_START : Coverity check for PCI_Porting_Template
					gBuffer[gBufferIndex] = (UINTN)PcdGet64 (PcdPciExpressBaseAddress) + ((UINTN)(gIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[SocketResIndex].PcieSegment * PCIEX_LENGTH));
                	// APTIOV_SERVER_OVERRIDE_RC_END : Coverity check for PCI_Porting_Template
	                DEBUG ((DEBUG_INFO,"gBuffer[gBufferIndex]:0x%X\n", gBuffer[gBufferIndex]));
				} else {
                    gBuffer[0] = (UINTN)PcdGet64 (PcdPciExpressBaseAddress);
                }
                
                gTempBufferIndex ++ ;
                DEBUG ((DEBUG_INFO,"NumberOfSegments:0x%X\n",NumberOfSegments));
                if (gTempBufferIndex == NumberofRb ) {
                    NumberOfSegments = gNumberOfAvailableSegments;
                    DEBUG ((DEBUG_INFO,"NumberOfSegments:0x%X\n",NumberOfSegments));
                    PcdSet32 (AmiPcdMaxSegmentSupported, NumberOfSegments);
                    gSizeOfBuffer = NumberOfSegments * sizeof(UINT64);
                    PcdSetPtr (AmiPcdPcieBaseAddressBuffer, &gSizeOfBuffer, (VOID*)&gBuffer);
                    gAmiPcieBaseAddressArray = (UINT64*)PcdGetPtr(AmiPcdPcieBaseAddressBuffer);
                }
                      
            } else {
                DEBUG ((DEBUG_INFO,"RB not present\n "));
                rb->NotPresent = TRUE;
            }
            
            break; //case isRbCheckPresence:

        case isRbBusUpdate:
            DEBUG ((DEBUG_INFO,"isRbBusUpdate \n"));
            res->_MIN = gIioUds->IioUdsPtr->PlatformData.IIO_resource[SocketResIndex].StackRes[StackResIndex].BusBase;
            res->_MAX = gIioUds->IioUdsPtr->PlatformData.IIO_resource[SocketResIndex].StackRes[StackResIndex].BusLimit;
            res->_LEN = (res->_MAX - res->_MIN) + 1;				
            rb->RbIoProtocol.SegmentNumber = gIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[SocketResIndex].PcieSegment;
            DEBUG ((DEBUG_INFO,"SegmentNumber = %d\n",rb->RbIoProtocol.SegmentNumber ));

            break;  //case isRbBusUpdate:
    }//switch

    DEBUG((DEBUG_INFO,"%a() EXIT\n",__FUNCTION__));
    return EFI_SUCCESS;
}

/**
    Skips the specified device from PCI enumeration. Bus, device and 
    function numbers of the device are used to skip device.
    
    @param *This                Pointer to an instance of AMI_BOARD_INIT_PROTOCOL.
    @param *Function            Pointer to the function number from which it is called.
    @param *ParameterBlock      Contains a block of parameters of routine.

    @retval EFI_STATUS          EFI_SUCCESS        When device is called with skip device and matches
                                                   specific bus, device and function numbers of device.
                                EFI_UNSUPPORTED    When device is not called with skip device or is
                                                   not matched with specific bus, device and function numbers of device.

**/
EFI_STATUS 
EFIAPI
PciPortSkipDeviceInit (
  IN      AMI_BOARD_INIT_PROTOCOL     *This,
  IN      UINTN                       *Function,
  IN OUT  VOID                        *ParameterBlock
)
{
    AMI_BOARD_INIT_PARAMETER_BLOCK  *Args = (AMI_BOARD_INIT_PARAMETER_BLOCK*)ParameterBlock;
    PCI_DEV_INFO                    *Dev = (PCI_DEV_INFO*)Args->Param1;
    
    if(Args->InitStep == isPciSkipDevice) {

        // Skip SMBUS device from PCI Bus enumeration.
        if ((Dev->Address.Addr.Bus == DEFAULT_PCI_BUS_NUMBER_PCH) && \
            (Dev->Address.Addr.Device == PCI_DEVICE_NUMBER_PCH_SMBUS) && \
            (Dev->Address.Addr.Function == PCI_FUNCTION_NUMBER_PCH_SMBUS)) {
            return EFI_SUCCESS;
        }
        
        // Skip SPI device from PCI Bus enumeration.
        if ((Dev->Address.Addr.Bus == DEFAULT_PCI_BUS_NUMBER_PCH) && \
            (Dev->Address.Addr.Device == PCI_DEVICE_NUMBER_PCH_SPI) && \
            (Dev->Address.Addr.Function == PCI_FUNCTION_NUMBER_PCH_SPI)) {
            return EFI_SUCCESS;
        }

        // Skip P2SB device from PCI Bus enumeration.
        if ((Dev->Address.Addr.Bus == DEFAULT_PCI_BUS_NUMBER_PCH) && \
            (Dev->Address.Addr.Device == PCI_DEVICE_NUMBER_PCH_P2SB) && \
            (Dev->Address.Addr.Function == PCI_FUNCTION_NUMBER_PCH_P2SB)) {
            return EFI_SUCCESS;
        }
    }
    
    return EFI_UNSUPPORTED;
}

/**
    Get Raid Driver image handle

    @param *This                 Pointer to an instance of EFI_BUS_SPECIFIC_DRIVER_OVERRIDE_PROTOCOL.
    @param *DriverImageHandle    Pointer to an Driver Image handle.

    @retval EFI_STATUS          EFI_SUCCESS             


**/
EFI_STATUS 
RaidGetDriver(
    IN     EFI_BUS_SPECIFIC_DRIVER_OVERRIDE_PROTOCOL *This,
    IN OUT EFI_HANDLE                                *DriverImageHandle
)
{

// APTIOV_SERVER_OVERRIDE_START  :   Revisit changes needed
 /**   
    EFI_STATUS              Status = EFI_UNSUPPORTED;
    UINTN                   Index, RaidBusOverrideMaxHandles;
    UINT8                   PciSubClassCode;
    BOOLEAN                 ArrayFound = FALSE;
    PCI_DEV_INFO            *Dev = ((PCI_BUS_OVERRIDE_DATA*)This)->Owner;
    EFI_HANDLE              *ImageHandleArray = NULL;

    
    DEBUG ((EFI_D_INFO,"%a: Start \n", __FUNCTION__));
    
    // Validate the input parameters
    if (DriverImageHandle == NULL) {
        return EFI_INVALID_PARAMETER;
    } 
    
    Status = Dev->PciIo.Pci.Read (&Dev->PciIo, EfiPciIoWidthUint8, PCI_CLASSCODE_OFFSET + 1, 1, &PciSubClassCode);	
    
    if (EFI_ERROR(Status)){
        DEBUG ((EFI_D_INFO,"%a: No PciIo \n", __FUNCTION__));
        return EFI_UNSUPPORTED;
    }
    
    if (PciSubClassCode != PCI_CLASS_MASS_STORAGE_RAID){
        DEBUG ((EFI_D_INFO,"%a: Not Mass Storage \n", __FUNCTION__));
        return EFI_UNSUPPORTED;
    }
    

    if ((Dev->Address.Addr.Bus == DEFAULT_PCI_BUS_NUMBER_PCH) && 
        (Dev->Address.Addr.Device == PCI_DEVICE_NUMBER_PCH_SATA) && 
        (Dev->Address.Addr.Function == PCI_FUNCTION_NUMBER_PCH_SATA)) {
        
            if (gSataGuidCount != 0) 
                ArrayFound = TRUE;
            
            ImageHandleArray = gSataImageHandleArray;
            RaidBusOverrideMaxHandles = gSataGuidCount;
    }
    
    if ((Dev->Address.Addr.Bus == DEFAULT_PCI_BUS_NUMBER_PCH) && 
        (Dev->Address.Addr.Device == PCI_DEVICE_NUMBER_PCH_SSATA) && 
        (Dev->Address.Addr.Function == PCI_FUNCTION_NUMBER_PCH_SSATA)) {
        
            if (gsSataGuidCount != 0) 
                ArrayFound = TRUE;
            
            ImageHandleArray = gsSataImageHandleArray;
            RaidBusOverrideMaxHandles = gsSataGuidCount;
    }
    
  
    if (ArrayFound) {
        // Image handle found
        //If the *DriverImageHandle is NULL , return the first Image handle
        if ( *DriverImageHandle == NULL ) {
            if(ImageHandleArray[0] != NULL) {
                *DriverImageHandle = ImageHandleArray[0];
                return EFI_SUCCESS;
            }
        } else {
            // If *DriverImageHandle not NULL , return the next Image handle 
            // from the available image handle list 
            for (Index = 0; Index < RaidBusOverrideMaxHandles; Index++) {
                if ( *DriverImageHandle == ImageHandleArray[Index] && (ImageHandleArray[Index+1] != NULL) ) {
                    *DriverImageHandle = ImageHandleArray[Index+1];
                    return EFI_SUCCESS;
                }
            }
        }
    }
    
    //Finished with checking handle array
    DEBUG ((EFI_D_INFO,"%a: Finish \n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
   **/  
    
    return EFI_SUCCESS;
// APTIOV_SERVER_OVERRIDE_END  :   Revisit changes needed  
 
}

/**
    Get SATA AHCI device handles

    @param *Dev                 Pointer to an instance of PCI_DEV_INFO.
    @param *GuidList            Pointer to array of guid to compare against image handle
    @param *ImageHandleArray    Pointer Image Handle buffer
    @param *GuidCount           Pointer to guids matched
    @param MaxGuidCount         Max guid can be copied to ImageHandleArray buffer
    
    @retval EFI_STATUS          EFI_SUCCESS             


**/
EFI_STATUS 
GetSataAhciHandles (
  IN      PCI_DEV_INFO             *dev,
  IN      EFI_GUID                 *GuidList,
  IN OUT  EFI_HANDLE               *ImageHandleArray,
  IN OUT  UINT8                    *GuidCount,
  IN      UINT8                    MaxGuidCount
)
{
    EFI_STATUS                      Status = EFI_UNSUPPORTED;
    UINTN                           HandleCount;
	// APTIOV_SERVER_OVERRIDE_RC_START : Coverity check for PCI_Porting_Template
    UINT8                           TotalGuidCount=0;
	// APTIOV_SERVER_OVERRIDE_RC_END : Coverity check for PCI_Porting_Template
    EFI_HANDLE                      *HandleBuffer = NULL;
    UINTN                           Index;
    EFI_DRIVER_BINDING_PROTOCOL     *DriverBindingProtocol = NULL;
    EFI_LOADED_IMAGE_PROTOCOL   	*LoadedImageProtocol;

    // Locate all the driver binding protocols
    Status = gBS->LocateHandleBuffer (ByProtocol,
                                      &gEfiDriverBindingProtocolGuid,
                                      NULL,
                                      &HandleCount,
                                      &HandleBuffer);
    
    if (EFI_ERROR(Status)) {
        return Status;
    }
	// APTIOV_SERVER_OVERRIDE_RC_START : Coverity check for PCI_Porting_Template
    // Get all the Guid's
    if (GuidList == gSataGuidListCheckForRaid) {
        TotalGuidCount=(sizeof(gSataGuidListCheckForRaid)/sizeof(EFI_GUID) -1 );
    } 
    // APTIOV_SERVER_OVERRIDE_START :   Revisit changes needed  
    /**
    else if (GuidList == gsSataGuidListCheckForRaid) {
        TotalGuidCount=(sizeof(gsSataGuidListCheckForRaid)/sizeof(EFI_GUID) -1 );
    }
    **/
    // APTIOV_SERVER_OVERRIDE_END :   Revisit changes needed
	// APTIOV_SERVER_OVERRIDE_RC_END : Coverity check for PCI_Porting_Template
    for (Index = 0; Index < HandleCount; Index++) {
        
        // Get the Driver Binding Protocol Interface
        Status = gBS->HandleProtocol(HandleBuffer[Index], 
                                     &gEfiDriverBindingProtocolGuid, 
                                     &DriverBindingProtocol );
        
        if (EFI_ERROR(Status) || DriverBindingProtocol == NULL){
            continue;
        }
        
        // Get the LoadedImage Protocol from ImageHandle
        Status = gBS->HandleProtocol(DriverBindingProtocol->ImageHandle, 
                                     &gEfiLoadedImageProtocolGuid, 
                                     &LoadedImageProtocol);
        
        if (EFI_ERROR(Status)){
            continue;
        }
        
        //Compare the File guid with driver's needs to launched first
        if (guidcmp(&(((MEDIA_FW_VOL_FILEPATH_DEVICE_PATH*)(LoadedImageProtocol->FilePath))->FvFileName), &GuidList[*GuidCount]) != 0) {
            continue;
        }
        
        //break the loop if guid count exceed sdl defined value
        if (*GuidCount >= MaxGuidCount) {
            break;
        }
        
        // Driver Image handle found. Add it in the Array
        ImageHandleArray[*GuidCount] = DriverBindingProtocol->ImageHandle;
        (*GuidCount)++;
        
        // Start from the beginning
        Index = -1;
        
        // Check All the Guid's are found. If found break the loop
		// APTIOV_SERVER_OVERRIDE_RC_START : Coverity check for PCI_Porting_Template
        if (*GuidCount >= TotalGuidCount) {
		// APTIOV_SERVER_OVERRIDE_RC_END : Coverity check for PCI_Porting_Template
            break;
        }   
    }
    
    //Finally add embedded ROM
    if (dev->EfiRomCount > 0){
        if (*GuidCount < MaxGuidCount) {
            ImageHandleArray[*GuidCount] = dev->EfiRomImages[0]->RomImgHandle;
            (*GuidCount)++;
        }
    }
        
    // Free the HandleBuffer Memory.
    if (HandleBuffer) {
        gBS->FreePool (HandleBuffer);
    }
    
    return EFI_SUCCESS;
}

#if defined SECURITY_SUPPORT && SECURITY_SUPPORT
/**
    Saves the boot scripts for AHCI devices

    @param *Dev                 Pointer to an instance of PCI_DEV_INFO.

    @retval EFI_STATUS          EFI_SUCCESS             


**/
EFI_STATUS 
SatasSataBootScriptSave (
  PCI_DEV_INFO                   *Dev
)
{
   
// APTIOV_SERVER_OVERRIDE_START : Revisit changes needed
/**
    UINT8             PciRead8 = 0;
    UINT16            PciRead16 = 0;
    UINT32            PciRead32 = 0;
    UINT8             Index;
    
    DEBUG ((EFI_D_INFO,"%a: Saving Boot Scripts on Device: %X \n", __FUNCTION__, Dev->Address.Addr.Device));
    
    // SATA/sSATA, Reg#74h PMCS
    S3PciRead16 (PCI_LIB_ADDRESS(Dev->Address.Addr.Bus, Dev->Address.Addr.Device, Dev->Address.Addr.Function, R_PCH_SATA_PMCS));
    
    // SATA/sSATA, Reg#90h MAP
    S3PciRead8 (PCI_LIB_ADDRESS(Dev->Address.Addr.Bus, Dev->Address.Addr.Device, Dev->Address.Addr.Function, R_PCH_H_SATA_MAP));
    
    // SATA/sSATA, Reg#94h PCS
    S3PciRead8 (PCI_LIB_ADDRESS(Dev->Address.Addr.Bus, Dev->Address.Addr.Device, Dev->Address.Addr.Function, R_PCH_H_SATA_PCS));
    
    // SATA/sSATA, Reg#10h~24h BARs
    for (Index = PCI_BASE_ADDRESSREG_OFFSET; Index < PCI_CARDBUS_CIS_OFFSET; Index += 4) {
	    S3PciRead32 (PCI_LIB_ADDRESS(Dev->Address.Addr.Bus, Dev->Address.Addr.Device, Dev->Address.Addr.Function, Index));
    }
    
    // SATA/sSATA, Reg#04h CMD
    S3PciOr8 (PCI_LIB_ADDRESS(Dev->Address.Addr.Bus, Dev->Address.Addr.Device, Dev->Address.Addr.Function, PCI_COMMAND_OFFSET), 
             (EFI_PCI_COMMAND_IO_SPACE | EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER));
**/
// APTIOV_SERVER_OVERRIDE_END : Revisit changes needed
    return EFI_SUCCESS;    
}
#endif		

/**
    Saves the boot scripts for AHCI devices and loads the Bus Override Protocol for RAID devices.

    @param *This                Pointer to an instance of AMI_BOARD_INIT_PROTOCOL.
    @param *Function            Pointer to the function number from which it is called.
    @param *ParameterBlock      Contains a block of parameters of routine.

    @retval EFI_STATUS          EFI_SUCCESS             
                                EFI_UNSUPPORTED When device is not called with set attribute device or is
                                                not matched with specific bus, device and function numbers of RAID device.

**/
EFI_STATUS 
EFIAPI
SataDeviceInit (
  IN      AMI_BOARD_INIT_PROTOCOL     *This,
  IN      UINTN                       *Function,
  IN OUT  VOID                        *ParameterBlock
)
{
    EFI_STATUS                           Status;
    AMI_BOARD_INIT_PARAMETER_BLOCK       *Args = (AMI_BOARD_INIT_PARAMETER_BLOCK*)ParameterBlock;
    PCI_DEV_INFO                         *Dev = (PCI_DEV_INFO*)Args->Param1;
    UINT8                                PciSubClassCode;
    AHCI_PLATFORM_POLICY_PROTOCOL        *AhciPlatformPolicy = NULL;
    EFI_HANDLE                           Handle = NULL;
        
    if(Args->InitStep == isPciProgramDevice) {

        if (((Dev->Address.Addr.Bus == DEFAULT_PCI_BUS_NUMBER_PCH) && 
             (Dev->Address.Addr.Device == PCI_DEVICE_NUMBER_PCH_SATA) && 
             (Dev->Address.Addr.Function == PCI_FUNCTION_NUMBER_PCH_SATA))
                // APTIOV_SERVER_OVERRIDE_START  :   Revisit changes needed
                /** ||
            ((Dev->Address.Addr.Bus == DEFAULT_PCI_BUS_NUMBER_PCH) &&
             (Dev->Address.Addr.Device == PCI_DEVICE_NUMBER_PCH_SSATA) &&
             (Dev->Address.Addr.Function == PCI_FUNCTION_NUMBER_PCH_SSATA))**/
                // APTIOV_SERVER_OVERRIDE_END  :   Revisit changes needed        
        ){
            
            Status = Dev->PciIo.Pci.Read(&Dev->PciIo, EfiPciIoWidthUint8, PCI_CLASSCODE_OFFSET + 1, 1, &PciSubClassCode);	
            if (EFI_ERROR(Status)){
                return EFI_UNSUPPORTED;
            } 
        
            //Check for AHCI and RAID modes.
#if A_S3 && (defined SECURITY_SUPPORT && SECURITY_SUPPORT)
            if (gDxeSmmReadyToLock == FALSE){
                SatasSataBootScriptSave(Dev);
            }
#endif	
        
            if (PciSubClassCode == PCI_CLASS_MASS_STORAGE_RAID) {
                
                DEBUG ((EFI_D_INFO,"%a: Installing OnboardRaidGuid on Device: %X \n", __FUNCTION__, Dev->Address.Addr.Device));
                
                Status = gBS->InstallProtocolInterface (&Dev->Handle,
                                                       &gAmiOnboardRaidControllerGuid,
                                                       EFI_NATIVE_INTERFACE,
                                                       NULL);
                ASSERT_EFI_ERROR (Status);
                
                //Check for UEFI
                if (Dev->BusOvrData.BusOverride.GetDriver!=NULL){
                    if ((Dev->Address.Addr.Device == PCI_DEVICE_NUMBER_PCH_SATA) && (Dev->Address.Addr.Function == PCI_FUNCTION_NUMBER_PCH_SATA)){
                        //Get SATA Handle Array
                        Status = GetSataAhciHandles(Dev, gSataGuidListCheckForRaid, gSataImageHandleArray, &gSataGuidCount, SATA_RAID_BUSOVERRIDE_MAX_HANDLES);
                    }
                    // APTIOV_SERVER_OVERRIDE_START  :   Revisit changes needed
                    /**      
                    if ((Dev->Address.Addr.Device == PCI_DEVICE_NUMBER_PCH_SSATA) && 
                            (Dev->Address.Addr.Function == PCI_FUNCTION_NUMBER_PCH_SSATA)){
                        //Get sSATA Handle Array
                        Status = GetSataAhciHandles(Dev, gsSataGuidListCheckForRaid, gsSataImageHandleArray, &gsSataGuidCount, S_SATA_RAID_BUSOVERRIDE_MAX_HANDLES);
                    }
                     **/ 
                    // APTIOV_SERVER_OVERRIDE_END  :   Revisit changes needed
                    // Get the AHCI Platform Policy Protocol
                    Status = gBS->LocateProtocol(&gAmiAhciPlatformPolicyProtocolGuid,
                                                 NULL,
                                                 (VOID **)&AhciPlatformPolicy );
    
                    if (EFI_ERROR(Status)) {
                        // If the Ahci Platform policy protocol not found, initialize with RAID value
                        Status = gBS->InstallProtocolInterface(&Handle,
                                                               &gAmiAhciPlatformPolicyProtocolGuid,
                                                               EFI_NATIVE_INTERFACE,
                                                               &gRaidAhciPlatformPolicy);
                    }
                    
                    //Replace Bus Specific Override Get Driver		    
                    Dev->BusOvrData.BusOverride.GetDriver = RaidGetDriver;
                }
            }
            return EFI_SUCCESS;                       
        }
    }
    
    return EFI_UNSUPPORTED;
}

/**
    Forces USB Bar into 32 bit resources.

    @param *This                Pointer to an instance of AMI_BOARD_INIT_PROTOCOL.
    @param *Function            Pointer to the function number from which it is called.
    @param *ParameterBlock      Contains a block of parameters of routine.

    @retval EFI_STATUS          EFI_SUCCESS             
                                EFI_UNSUPPORTED When device is not called with set attribute device, does not match 
                                                with specific bus, device and function numbers of USB device, or 
                                                Force10GbE32BitResource token is disabled.

**/
EFI_STATUS 
EFIAPI
PciPortDegradeUsbBar (
    IN AMI_BOARD_INIT_PROTOCOL              *This,
    IN UINTN                                *Function,
    IN OUT AMI_BOARD_INIT_PARAMETER_BLOCK   *ParameterBlock
)
{
    // APTIOV_SERVER_OVERRIDE_START  : Revisit changes needed (PchUsbDegradeBar field removed)
    /**
    EFI_STATUS                             Status;	
    AMI_BOARD_INIT_PARAMETER_BLOCK         *Args = ParameterBlock;
    PCI_INIT_STEP                          InitStep = (PCI_INIT_STEP)Args->InitStep;
    PCI_DEV_INFO                           *dev = (PCI_DEV_INFO*)Args->Param1;
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *RbIo = dev->RbIo;
    UINTN                                  BarCount;
    UINT8                                  DegradeUsbBar = 0;
    

    if (InitStep == isPciQueryDevice) {

        if ((RbIo->SegmentNumber == PCH_PCI_DEVICES_SEGMENT_NO) && (dev->Address.Addr.Bus == DEFAULT_PCI_BUS_NUMBER_PCH) &&
            (dev->Address.Addr.Device == PCI_DEVICE_NUMBER_PCH_XHCI) && (dev->Address.Addr.Function == PCI_FUNCTION_NUMBER_PCH_XHCI)) {
            
            Status = GetOptionData(&gPchSetupVariableGuid, OFFSET_OF (PCH_SETUP, PchUsbDegradeBar), &DegradeUsbBar, sizeof (DegradeUsbBar));
            if ( !EFI_ERROR(Status) && (DegradeUsbBar == 1) ) {
    
                for(BarCount = 0; BarCount < PCI_MAX_BAR_NO; BarCount++) {
                    if ( dev->Bar[BarCount].Type == tBarMmio64 ) { // Checking the allocate type.
                        DEBUG ((EFI_D_INFO,"\n Assigning resources below 4 GB for USB as per RC request \n"));
                        dev->Bar[BarCount].Type = tBarMmio32;
                    } else if ( dev->Bar[BarCount].Type == tBarMmio64pf) {
                        DEBUG ((EFI_D_INFO,"\n Assigning resources below 4 GB for USB as per RC request \n"));
                        dev->Bar[BarCount].Type = tBarMmio32pf;
                    }
                }
                
                return EFI_SUCCESS;
            }
        }
    } // isPciQueryDevice
     
     **/
    // APTIOV_SERVER_OVERRIDE_END : Revisit needed
    return EFI_UNSUPPORTED;
}

/**
    Forces 10 GbE Bar into 32 bit resources.
    To fix issue with issue in booting 10 GbE legacy PXE

    @param *This                Pointer to an instance of AMI_BOARD_INIT_PROTOCOL.
    @param *Function            Pointer to the function number from which it is called.
    @param *ParameterBlock      Contains a block of parameters of routine.

    @retval EFI_STATUS          EFI_SUCCESS             
                                EFI_UNSUPPORTED When device is not called with set attribute device, does not match 
                                                with specific bus, device and function numbers of 10 GbE device, or 
                                                Force10GbE32BitResource token is disabled.

**/
EFI_STATUS 
EFIAPI 
PciPortDegrade10GbEBar (
    IN AMI_BOARD_INIT_PROTOCOL              *This,
    IN UINTN                                *Function,
    IN OUT AMI_BOARD_INIT_PARAMETER_BLOCK   *ParameterBlock
)
{
    AMI_BOARD_INIT_PARAMETER_BLOCK        *Args = ParameterBlock;
    PCI_INIT_STEP                         InitStep = (PCI_INIT_STEP)Args->InitStep;
    PCI_DEV_INFO                          *dev = (PCI_DEV_INFO*)Args->Param1;
    
    DEBUG ((EFI_D_INFO,"%a: Start \n", __FUNCTION__));
    
    if (InitStep == isPciGetSetupConfig) {
        if (FORCE_32BIT_RESOURCES_TO_ONBOARD_10GBE == 1){
            DEBUG ((EFI_D_INFO,"%a: restrict MMIO resource to 32 bit \n", __FUNCTION__));
            dev->DevSetup.Decode4gDisable = TRUE;
            return EFI_SUCCESS;
        }
    } // isPciGetSetupConfig
    
    return EFI_UNSUPPORTED;
}

/**
    This is the Event call back function to notify the library functions that the 
    system is entering the SmmLocked phase.

    @param Event       Pointer to this event.
    @param *Context    Event handler private data.

    @retval VOID

**/
VOID
EFIAPI
DxeSmmReadyToLockEventCallBack (
  IN EFI_EVENT              Event,
  IN VOID                   *Context
  )
{
  EFI_STATUS                Status;
  VOID                      *Interface;

  // Try to locate gEfiDxeSmmReadyToLockProtocolGuid because EfiCreateProtocolNotifyEvent will trigger once during registration.
  // just return if it is not found.
  Status = gBS->LocateProtocol (&gEfiDxeSmmReadyToLockProtocolGuid,
                                NULL,
                                &Interface);
  
  if (EFI_ERROR (Status)) {
    return;
  }
  
  //Set global variable
  gDxeSmmReadyToLock = TRUE;
}

/**
    Constructor to register protocol notify event for  gEfiDxeSmmReadyToLockProtocolGuid

    @param ImageHandle    Image handle.
    @param SystemTable    Pointer to the EFI system table.

    @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
PciBoardDxeInitInitialize (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  VOID                           *Registration;
  EFI_EVENT                      Event;
  
  // Create event to notify the library system entered the SmmLocked phase.
  Event = EfiCreateProtocolNotifyEvent  (
		  &gEfiDxeSmmReadyToLockProtocolGuid,
		  TPL_CALLBACK,
		  DxeSmmReadyToLockEventCallBack,
		  NULL,
		  &Registration
		  );
  
  ASSERT (Event != NULL);
  
  return EFI_SUCCESS;
}
