//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file
  @brief The file contains definition of the AMI Bus Resource Map Information data structures.

**/
#ifndef __AMI_BUS_RESOURCE_MAP_H__
#define __AMI_BUS_RESOURCE_MAP_H__

#define AMI_BUS_RESOURCE_MAP_NAME               L"AmiBusResourceMap"
#define SOCKET_BUS_GRANULARITY_PER_SEGMENT      32
#define STACK_MINIMUM_BUS_LENGTH                2
#define MAX_BUS_PER_SEGMENT                     0x100
//Create SegmentMap for MAX SOCKET
#define MAX_SEGMENT_RESOURCE_MAP                MAX_SOCKET

#define AMI_BUS_RESOURCE_MAP_GUID \
    { 0x0a813062, 0xfad5, 0x414d, { 0x83, 0xb5, 0xc3, 0xc8, 0x37, 0x93, 0x9c, 0xba} }

#pragma pack (1)
typedef struct {
    BOOLEAN                 Valid;
    BOOLEAN                 Failed;
    BOOLEAN                 UnDetermined;
    UINT8                   BusBase;
    UINT16                  BusLength;
    UINT8                   NewBusBase;
    UINT16                  NewBusLength;
    UINT16                  BusRequired;
} BUS_STACK_RESOURCE;

typedef struct { 
    BOOLEAN                 Valid;
    BOOLEAN                 Failed;
    UINT8                   SocketIndex;
    UINT8                   BusBase;
    UINT16                  BusLength;
    UINT8                   NewBusBase;
    UINT16                  NewBusLength;
    UINT16                  BusRequired;
    UINT8                   FailedStackCount;
    UINT8                   UnDeterminedStackCount;
    UINT8                   ValidStackCount;
    BUS_STACK_RESOURCE      StackRes[MAX_LOGIC_IIO_STACK];
} BUS_SOCKET_RESOURCE;

typedef struct { 
    BOOLEAN                 Initialized;
    BUS_SOCKET_RESOURCE     SocketRes[MAX_SOCKET];
} BUS_RESOURCE;

typedef struct { 
    UINT8                   BusOORResetCount;
    BUS_RESOURCE            BusResource;
} AMI_BUS_RESOURCE_MAP;

typedef struct {
    //Granularity
    UINT8                   SocketBusGranularity;
    UINT8                   StackMinBusLength;
} BUS_RESOURCE_GRANULARITY;

typedef struct {
    //To make it look Like _T_ITEM_LST
    UINTN                   InitCount;
    UINTN                   SocketCount;
    BUS_SOCKET_RESOURCE     **SocketRes;  //this is array of Pointers 
    UINT8                   FailedSocketCount;

    //Stack details
    UINT8                   FailedStackCount;
    UINT8                   UnDeterminedStackCount;
    UINT8                   ValidStackCount;
} PCIE_SEGMENT_SOCKET_MAP;
#pragma pack ()

extern EFI_GUID gAmiBusResourceMapGuid;

#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
