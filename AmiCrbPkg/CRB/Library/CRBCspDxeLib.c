//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <Token.h>
#include <Protocol/ExitPmAuth.h>
#include <Protocol/LegacyBiosPlatform.h>
#include <Pci.h>
#include <Protocol/PciIo.h>
#include <Protocol/IioUds.h>
#include <Guid/SetupVariable.h>
#include <Include/SouthCluster/Register/PchRegsEva.h>
#include <Include/SouthCluster/Register/PchRegsSata.h>
#include "BootOptions.h"
#include "BootOptioneLinks.h"
#include "Protocol/IdeControllerInit.h"
#include "Protocol/PciIo.h"
#include "Protocol/DiskInfo.h"
#include "Protocol/AmiAhciBus.h"
#include "Protocol/LegacyRegion2.h"
#include "Guid/SocketMpLinkVariable.h"
#include <Library/S3BootScriptLib.h>
#include <AmiCspLib.h>

#define SOCKET_MP_LINK_CONFIGURATION_NAME L"SocketMpLinkConfig"

extern EFI_GUID gSignalBeforeEnterSetupGuid;
EFI_GUID  EfiLegacyRegion2ProtocolGuid = EFI_LEGACY_REGION2_PROTOCOL_GUID;

// Controller info Structure
typedef struct _CONTROLLER_INFO_STRUC {
    UINT16      BusDevFun;      // Bus, Device and Function number for a controller
    UINTN       ControllerNo;   // Controler number for a controller
} CONTROLLER_INFO_STRUC;

CONTROLLER_INFO_STRUC ControllerInfo[30];  // need to use macro
UINTN           MaxControllerFound;

#define SATACONTROLLER0 0
#define SATACONTROLLER1 1
#define SSATACONTROLLER 2


/**
    The elink will signal gExitPmAuthProtocolGuid Event.
    @param[in]       VOID  

    @return     VOID

**/
VOID 
SignalExitPmAuthProtocolEvent (
  VOID
)
{
  EFI_HANDLE  Handle = NULL;

  //
  // Signaling gExitPmAuthProtocolGuid Event
    //
  pBS->InstallProtocolInterface (
        &Handle,
        &gExitPmAuthProtocolGuid,
        EFI_NATIVE_INTERFACE,
        NULL
        );
}

/**
    Returns Vga handle for the Platform. Onboard/Off-board Vga
    is decided based upon user setting for Vga priority.
    
    @param[in]  This            Indicates the EFI_LEGACY_BIOS_PLATFORM_PROTOCOL instance.
    @param[in]  Mode            Specifies what handle to return.
                                GetVgaHandle
    @param[in]  Type            Handle Modifier - Mode specific
    @param[in]  HandleBuffer    Pointer to buffer containing all Handles matching the
                                specified criteria. Handles are sorted in priority order.
                                Type EFI_HANDLE is defined in InstallProtocolInterface()
                                in the EFI 1.10 Specification.
    @param[in]  HandleCount     Number of handles in HandleBuffer.
    @param[in]  AdditionalData  Pointer to additional data returned - mode specific..

    @return     EFI_STATUS
    @retval     EFI_SUCCESS         - The handle is valid.
    @retval     EFI_UNSUPPORTED Mode is not supported on this platform.
    @retval     EFI_NOT_FOUND   The handle is not known.
    
**/
EFI_STATUS
CRBGetPlatformVgaHandle (
  IN  EFI_LEGACY_BIOS_PLATFORM_PROTOCOL     *This,
  IN  EFI_GET_PLATFORM_HANDLE_MODE          Mode,
  IN  UINT16                                Type,
  OUT EFI_HANDLE                            **HandleBuffer,
  OUT UINTN                                 *HandleCount,
  OUT VOID                                  OPTIONAL **AdditionalData
  )
{
    EFI_HANDLE              *hBuffer = NULL;
    EFI_HANDLE              hVga = NULL;
    EFI_HANDLE              hOffboardVideo = NULL;
    EFI_STATUS              Status;
    UINT8                   dData[4];
    UINT32                  BARAddrReg;
    UINT8                   BarIndex;
    UINT8                   Offset=0x10;
    BOOLEAN                 IsBARNonZero=FALSE;
    EFI_PCI_IO_PROTOCOL     *PciIo;
    EFI_STATUS              VgaStatus = EFI_NOT_FOUND;
    UINTN                   VariableSize = sizeof(SYSTEM_CONFIGURATION);
    UINTN                   SocketVariableSize = sizeof(SOCKET_MP_LINK_CONFIGURATION);
    UINTN                   Index, Count, Seg, Bus, Dev, Fun;
    UINT8                   SocketNum, StackNum;
    BOOLEAN                 SocketStackNumFound = FALSE;
    UINT64                  PciAttributes;
    EFI_IIO_UDS_PROTOCOL    *IioUds;
    SYSTEM_CONFIGURATION    SystemConfiguration;
    SOCKET_MP_LINK_CONFIGURATION  SocketMpLinkConfiguration;
    UINT32                  VariableAttr, SocketVariableAttr;

    if (Mode == EfiGetPlatformVgaHandle) {
        Status = pRS->GetVariable(  L"IntelSetup",
                        &gEfiSetupVariableGuid,
                        &VariableAttr,
                        &VariableSize,
                        &SystemConfiguration
                        );
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR (Status)) {
            return Status;
        }
        Status = pRS->GetVariable( SOCKET_MP_LINK_CONFIGURATION_NAME,
                                &gEfiSocketMpLinkVariableGuid,
                                &SocketVariableAttr,
                                &SocketVariableSize,
                                &SocketMpLinkConfiguration
                                );
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR (Status)) {
            return Status;
        }
    
        //
        // Locate all PciIo handles
        //
        Status = pBS->LocateHandleBuffer (
                        ByProtocol,
                        &gEfiPciIoProtocolGuid,
                        NULL,
                        &Count,
                        &hBuffer);
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR (Status)) {
            return Status;
        }
        
        for (Index = 0; Index < Count; Index++) {
            Status = pBS->HandleProtocol (
                            hBuffer[Index],
                            &gEfiPciIoProtocolGuid,
                            &PciIo);           // Get PciIo protocol
            ASSERT_EFI_ERROR (Status);
            if (EFI_ERROR(Status)) break;
            Status = PciIo->Pci.Read(
                                PciIo,
                                EfiPciIoWidthUint32,
                                8,  // offset
                                1,  // width
                                dData);
            
            ASSERT_EFI_ERROR(Status);
            if (EFI_ERROR(Status)) break;
            //
            // Check for VGA-compatible Display controller
            // Base Class: 03h (Display Controller), Sub-Class: 00h (VGA-compatible controller)
            //
            if (dData[3] == PCI_CL_DISPLAY && dData[2] == PCI_CL_DISPLAY_SCL_VGA) {
                for(BarIndex=0, IsBARNonZero=FALSE, Offset=0x10; BarIndex<6; BarIndex++,Offset+=4) {
                    //Get Device BAR Address Values
                    Status = PciIo->Pci.Read(
                                        PciIo,
                                        EfiPciIoWidthUint32,
                                        Offset,  // offset
                                        1,  // width
                                        &BARAddrReg);
                    if (!EFI_ERROR(Status)) {
                        if ( BARAddrReg & BIT0 ) {
                            // Io Space: Mask BIT1:BIT0 and check BAR is Zero
                            if(BARAddrReg & 0xFFFFFFFC) {
                                IsBARNonZero = TRUE;
                            }
                        } else {
                            //Memory Space: Mask BIT3:BIT0 and Check if BAR is ZERO
                            if(BARAddrReg & 0xFFFFFFF0) {
                                IsBARNonZero = TRUE;
                            }
                            //Memory Space: Mask BIT2:BIT1 and Check if 64-Bit BAR
                            if((BARAddrReg & (BIT2 | BIT1)) == BIT2){
                                BarIndex++;
                                Offset+=4;
                                //Get Device BAR Address Values
                                Status = PciIo->Pci.Read(
                                                    PciIo,
                                                    EfiPciIoWidthUint32,
                                                    Offset,  // offset
                                                    1,  // width
                                                    &BARAddrReg);
                                //Memory Space: Check if upper 32-bit of BAR is not ZERO
                                if(!EFI_ERROR(Status) && BARAddrReg) {
                                    IsBARNonZero = TRUE;
                                }
                            }
                        }
                    }
                }
                if((BarIndex == 6) && !IsBARNonZero){
                    DEBUG((DEBUG_INFO, "VGA Detected with No Resources allocated\n"));
                    continue;
                }

                Status = PciIo->Attributes(PciIo, EfiPciIoAttributeOperationGet, 0, &PciAttributes);
                VgaStatus = EFI_SUCCESS;
                if (EFI_ERROR(Status))
                    return Status;

                //
                // Check for on-board device(EFI_PCI_IO_ATTRIBUTE_EMBEDDED_DEVICE is set).
                // If no onboard video is installed, and offboard video detected, then offboard VGA handle is returned.
                //
                if ((PciAttributes & EFI_PCI_IO_ATTRIBUTE_EMBEDDED_DEVICE)) {
                    hVga = hBuffer[Index];                  
                    if (SystemConfiguration.VideoSelect == 1) {// If user priority set to Onboard video, break the loop
                        break;
                    }
                }
                //
                // Handle only the 1st Offboard Video device identified.
                // If no offboard video is installed, then onboard VGA handle is returned when present.
                //
                if(hOffboardVideo == NULL && !(PciAttributes & EFI_PCI_IO_ATTRIBUTE_EMBEDDED_DEVICE)) { //Offboard Video
                    hOffboardVideo = hBuffer[Index];
                    if(SystemConfiguration.VideoSelect == 2) {
                        hVga = hOffboardVideo;
                        break; // If user priority set to Offboard Video, break the loop
                    }
                }
                
            } // if PCI_CL_DISPLAY
        } // end of for loop
        
        pBS->FreePool(hBuffer);
        
        if(VgaStatus != EFI_SUCCESS){
            **HandleBuffer = 0;
            *HandleCount = 0;
            return VgaStatus;
        }
        
        //
        // Set priority to offboard if Video Select is Auto
        //
        //
        if( (SystemConfiguration.VideoSelect == 0) && (hOffboardVideo != NULL) ) {
            hVga = hOffboardVideo;
        }
            
        // Check for offboard VGA handle when there is no onboard video and user sets priority to onboard video
        //
        if(hOffboardVideo != NULL && hVga == NULL) {
            hVga = hOffboardVideo;
        }
        
        Status = pBS->HandleProtocol (
                        hVga,
                        &gEfiPciIoProtocolGuid,
                        &PciIo);           // Get PciIo protocol
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR(Status))
            return Status;

        Status = PciIo->GetLocation(PciIo, &Seg, &Bus, &Dev, &Fun);
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR(Status))
            return Status;

        //
        // Get the Cpu No. of the assigned Vga controller
        //
        Status = pBS->LocateProtocol(
                        &gEfiIioUdsProtocolGuid,
                        NULL,
                        &IioUds
                        );
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR(Status))
            return Status;
        
        //
        // Loop through all IIO stacks of all sockets that are present
        //
        for (SocketNum = 0; SocketNum < MAX_SOCKET; SocketNum++) {
            if (!IioUds->IioUdsPtr->PlatformData.IIO_resource[SocketNum].Valid) {
              continue;
            }
            //
            // #MultiSegment: Need to have condition check for Pci Segment if Multi segment is supported.
            // Currently, Multi segment is not supported so modify the code after the support is available.
            //
            //if( Seg != ) {
            //    continue;
            //}
            if( (Bus < IioUds->IioUdsPtr->PlatformData.IIO_resource[SocketNum].BusBase) || \
                (Bus > IioUds->IioUdsPtr->PlatformData.IIO_resource[SocketNum].BusLimit) ) {
                continue;
            }
            for (StackNum = 0; StackNum < MAX_LOGIC_IIO_STACK; StackNum++) {
                if (!((IioUds->IioUdsPtr->PlatformData.CpuQpiInfo[SocketNum].stackPresentBitmap) & (1 << StackNum))) {
                    continue;
                }
                //
                // #MultiSegment: Need to have condition check for Pci Segment if Multi segment is supported.
                // Currently, Multi segment is not supported so modify the code after the support is available.
                //
                //if( Seg != ) {
                //    continue;
                //}
                if( (Bus >= IioUds->IioUdsPtr->PlatformData.IIO_resource[SocketNum].StackRes[StackNum].BusBase) && \
                    (Bus <= IioUds->IioUdsPtr->PlatformData.IIO_resource[SocketNum].StackRes[StackNum].BusLimit) ) {
                    SocketStackNumFound = TRUE;
                    break;
                }
            }
            //
            // Break Socket loop if SocketStackNumFound is TRUE
            //
            if( SocketStackNumFound ) {
                break;
            }
        }
        if( SocketStackNumFound == FALSE ) {
            return EFI_NOT_FOUND;
        }
        
        Status = pBS->AllocatePool (
                EfiBootServicesData,
                sizeof (EFI_HANDLE),
                (VOID **) HandleBuffer);
        if (!EFI_ERROR (Status)){
            *HandleBuffer[0] = hVga;
            *HandleCount = 1; 
        }        
        
        //
        // Issue Cold Reset if the Socket or Stack number for Vga changes from previous boot
        //
        if( (SocketMpLinkConfiguration.LegacyVgaSoc != SocketNum) || \
            (SocketMpLinkConfiguration.LegacyVgaStack != StackNum) ) { 
            DEBUG((DEBUG_INFO, "Current: LegacyVgaSoc: %x LegacyVgaStack: %x\n", SocketMpLinkConfiguration.LegacyVgaSoc, SocketMpLinkConfiguration.LegacyVgaStack));
            SocketMpLinkConfiguration.LegacyVgaSoc = SocketNum;
            SocketMpLinkConfiguration.LegacyVgaStack = StackNum;
            DEBUG((DEBUG_INFO, "Updated: LegacyVgaSoc: %x LegacyVgaStack: %x\n", SocketMpLinkConfiguration.LegacyVgaSoc, SocketMpLinkConfiguration.LegacyVgaStack));
            Status = pRS->SetVariable (
                            SOCKET_MP_LINK_CONFIGURATION_NAME,
                            &gEfiSocketMpLinkVariableGuid,
                            SocketVariableAttr,
                            SocketVariableSize,
                            &SocketMpLinkConfiguration
                            );
            ASSERT_EFI_ERROR(Status);
            DEBUG((DEBUG_INFO, "Issue Cold Reset for change in VGA range target \n"));
            DEBUG((DEBUG_INFO, "SYSTEM is going to COLD RESET; VGA Target Changed.\n"));
            
            pRS->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);
        }

        return VgaStatus;
   }
    else {
        return EFI_UNSUPPORTED;
    }
    
}

// Fucntion to get Controller Number
#ifdef CSM_SUPPORT
VOID 
GetControllerNumber (
    IN OUT UINTN                *gControllerNoAddr,
    IN UINT16                   wBusDevFunc
)
{
    // APTIOV_SERVER_OVERRIDE_START :   Revisit changes needed  
    /**
    UINT8       i;
    UINT16     Func;

    Func = wBusDevFunc & 0x07;

    for(i=0 ; i<MaxControllerFound ; i++) {
        if(ControllerInfo[i].BusDevFun == wBusDevFunc) {

            // Updatng the respective controller number.
            *gControllerNoAddr = ControllerInfo[i].ControllerNo;
            return;
        }
    }

    //
    // A new controller is found so BusDevFunc and ControllerNo is added
    // into the array of structure.
    //
    ControllerInfo[MaxControllerFound].BusDevFun    = wBusDevFunc;
    switch( Func ) {
        case PCI_FUNCTION_NUMBER_PCH_SATA:
            ControllerInfo[MaxControllerFound].ControllerNo = SATACONTROLLER0;
            break;

        case PCI_FUNCTION_NUMBER_EVA_SSATA:
            ControllerInfo[MaxControllerFound].ControllerNo = SSATACONTROLLER;
            break;
    }
    *gControllerNoAddr= ControllerInfo[MaxControllerFound].ControllerNo;
    MaxControllerFound++;
    **/
    // APTIOV_SERVER_OVERRIDE_END :   Revisit changes needed  
    return;
}

//  Function to Create AHCI drive Boot Option
EFI_STATUS
CreateBootOptionNameForAhciDrive ( 
    UINT8  ControllerNo,
    UINT8  PortNumber,
    UINT8  *SerialNumber,
    UINT8  *DriveString
)
{
    EFI_STATUS Status;
    UINT8   s[32] = {0};
    UINT8   TempStr[12] = "xSATAx Px: ";   //"[Controller] P[PortNo]: "
    UINTN   Index;
    EFI_LEGACY_REGION2_PROTOCOL *LegacyRegion2Protocol;
    UINT32  Granularity;
    
    TempStr[5] = ControllerNo + 0x30;
    TempStr[8] = PortNumber + 0x30;
    TempStr[0] = 0x20;

    if( ControllerNo == SSATACONTROLLER ) {
        TempStr[0] = 's';
        TempStr[5] = 0x20;
        pBS->CopyMem(s,TempStr,11);
        Index = 11;
    } else {
        pBS->CopyMem(s,&TempStr[1],10);
        Index = 10;
    }
    pBS->CopyMem(&s[Index],SerialNumber,20);

    Status = pBS->LocateProtocol(&EfiLegacyRegion2ProtocolGuid,NULL,&LegacyRegion2Protocol);
          
    if(EFI_ERROR(Status)) {
        return Status;
    }
          
    LegacyRegion2Protocol->UnLock(LegacyRegion2Protocol,(UINT32)DriveString,32,&Granularity);

    pBS->CopyMem(DriveString,s,32);

    LegacyRegion2Protocol->Lock(LegacyRegion2Protocol,(UINT32)DriveString,32,&Granularity);

   return EFI_SUCCESS;
    
}

///  Fucntion to create boot option string to all drives
EFI_STATUS 
CreateStringForAllDrives (
  BBS_TABLE *BbsTable
)
{
    EFI_STATUS                        Status;
    static EFI_GUID                   DiskInfoGuid      = EFI_DISK_INFO_PROTOCOL_GUID;
    UINT32                            Bus = BbsTable->Bus;
    UINT32                            Device = BbsTable->Device;
    UINT32                            Function = BbsTable->Function;    
    UINT8                             SubClass = BbsTable->SubClass;
    UINTN                             gControllerNo=0,i;
    VOID                              *Dev;
    EFI_HANDLE                        DeviceHandle = (EFI_HANDLE)*(UINTN*)&BbsTable->IBV1;

    EFI_DISK_INFO_PROTOCOL            *DiskInfo;
    UINT16                            IdentifyData[256];
    UINT8                             SerialNumber[20];
    UINT8                             data8;
    UINT32                            Size;

    // Call to get Controller Number of the Drive
    GetControllerNumber(&gControllerNo,((UINT16)Bus<<8) + ((UINT16)Device<<3) + ((UINT16)Function) );

    Status = pBS->HandleProtocol( DeviceHandle, &DiskInfoGuid, &DiskInfo );

    if (EFI_ERROR(Status)) return 0;

    Size = sizeof(IdentifyData);
    
    Status = DiskInfo->Identify ( DiskInfo, IdentifyData, &Size );

    pBS->CopyMem(SerialNumber,&IdentifyData[27],20);
      
    for (i=0; i< 20; i+=2) {
        data8 = SerialNumber[i];
        SerialNumber[i] = SerialNumber[i+1];
        SerialNumber[i+1] = data8;
    }

    // BAsed on SubClass Value calling AHCI drive Option
    if(SubClass == 6) {
        Dev = (SATA_DEVICE_INTERFACE *)((SATA_DISK_INFO*)DiskInfo)->SataDevInterface;

        CreateBootOptionNameForAhciDrive((UINT8)gControllerNo,
                                         ((SATA_DEVICE_INTERFACE *)Dev)->PortNumber,
                                         SerialNumber,
                                         (UINT8*)(UINTN)(((UINT32)BbsTable->DescStringSegment << 4)+((UINT32)BbsTable->DescStringOffset & 0x0FFFF)) );
    }

    return Status;
}


EFI_STATUS 
EFIAPI 
FastbootUpdateBootOptionNameByBbsDescription(
  VOID
)
{
    EFI_STATUS                  Status;
    UINT8                       i;
    UINT16                      HddCount;
    UINT16                      BbsCount;
    HDD_INFO                    *HddInfo;
    BBS_TABLE                   *BbsTable;
    EFI_LEGACY_BIOS_PROTOCOL    *LegacyBios;

    Status = pBS->LocateProtocol(&gEfiLegacyBiosProtocolGuid, NULL, (VOID**)&LegacyBios);
    if (EFI_ERROR(Status))
        return Status;

    LegacyBios->GetBbsInfo(LegacyBios, &HddCount, &HddInfo, &BbsCount, &BbsTable);

    for(i=0; i<BbsCount; i++)
    {
        if (BbsTable[i].DeviceType == BBS_HARDDISK) {
            CreateStringForAllDrives(&BbsTable[i]);
        }
    }
    return EFI_SUCCESS;
}

/// OEM hook function 
UINTN 
OEMConstructBootOptionNameByBbsDescription (
    BOOT_OPTION         *Option, 
    CHAR16              *Name, 
    UINTN               NameSize
)
{
    if (Option->BbsEntry == NULL) return 0;

    // Function to Change the Boot Option Name of the Drive
    CreateStringForAllDrives(Option->BbsEntry);

    return 0;
}

#endif
