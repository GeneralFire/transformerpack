//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file UpdateSmbiosTable.h
    Contents present here is used by UpdateSmbiosTable.c
**/
#ifndef __SMBIOSUPDATE__H__
#define __SMBIOSUPDATE__H__
#ifdef __cplusplus
extern "C" {
#endif

// Header Files
#include <Token.h>
#include <Library/PcdLib.h>
#include <AmiDxeLib.h>
#include <Pci.h>
#include <Uefi/UefiSpec.h>
#include <IndustryStandard/Smbios.h>
#include <IndustryStandard/Pci30.h>
#include <Protocol/Smbios.h>
#include <Protocol/IioSystem.h>
#include <Library/PciExpressLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/MmPciBaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <AmiPcieSegBusLib/AmiPcieSegBusLib.h>
#include <UsraAccessType.h>
#include <Library/UsraAccessApi.h>
#include <Library/PcieCommonInitLib.h>
#include <Library/PcieCommonInitLib/PcieCommonHelper.h>
#include <Include/PciBus.h>

#define SLOT1_SOCKET_NO   0
#define SLOT1_STACK_NO    1
#define SLOT1_DEV_FUN_NO  0x10

#define SLOT2_SOCKET_NO   0
#define SLOT2_STACK_NO    3
#define SLOT2_DEV_FUN_NO  0x00

#define SLOT3_SOCKET_NO   0
#define SLOT3_STACK_NO    0
#define SLOT3_DEV_FUN_NO  0xE0

#define SLOT4_SOCKET_NO   1
#define SLOT4_STACK_NO    1
#define SLOT4_DEV_FUN_NO  0x00

#define SLOT5_SOCKET_NO   1
#define SLOT5_STACK_NO    3
#define SLOT5_DEV_FUN_NO  0x00

#define SLOT6_SOCKET_NO   1
#define SLOT6_STACK_NO    0
#define SLOT6_DEV_FUN_NO  0x00

#define SLOT7_SOCKET_NO   1
#define SLOT7_STACK_NO    3
#define SLOT7_DEV_FUN_NO  0x10

#define SLOT8_SOCKET_NO   1
#define SLOT8_STACK_NO    2
#define SLOT8_DEV_FUN_NO  0x00

#define DeciveEnable  BIT7

#define MassStorage 0x01
#define Network     0x02
#define Video       0x03
#define MultiMedia  0x04

#define SCSIController 0x00
#define SataController 0x06
#define SasController  0x07

#define EthernetController  0x00
#define TokenRingController 0x01

#define VGAController 0x00
#define XGAController 0x01

#define AudioController 0x01
#define AudioDevice     0x03

#define VIDEO_CONTROLLER    0x03
#define SCSI_CONTROLLER     0x04
#define ETHERNET_CONTROLLER 0x05
#define TOKEN_RING          0x06
#define SOUND               0x07
#define SATA_CONTROLLER     0x09
#define SAS_CONTROLLER      0x0A

//Structure declarations
/**
    System Slot Socket, Stack, Type and Data bus width Information
**/
typedef struct {
    UINT8                     Socket;
    UINT8                     Stack;
    UINT8                     SlotType;
    UINT8                     SlotDataBusWidth;
    UINT8                     DevFun;
} SLOT_INFO; 

/**
    Onboard device information
**/
typedef struct {
    UINT8                     DevType;
    UINT8                     DevTypeInstance;
    UINT16                    SegGrpNum;
    UINT8                     BusNum;
    UINT8                     DevFunNum;
} TYPE41_DEV_INFO; 

//Function Declarations
VOID
EFIAPI
UpdateSmbiosTables (
  IN  EFI_EVENT Event,
  IN  VOID      *Context
);

EFI_STATUS
EFIAPI
UpdateSmbiosType9Table (
);

EFI_STATUS
EFIAPI
UpdateSmbiosType41Table (
);

EFI_STATUS
EFIAPI
UpdateSmbiosType41TableData (
    IN  UINT8           DevType,
    IN  CONST CHAR8*    Designation,
    IN  UINT8           DeviceInstance
);

EFI_STATUS
AddStructureForAdditionalSlots (
    UINT8 AddSlotCount
);

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif
