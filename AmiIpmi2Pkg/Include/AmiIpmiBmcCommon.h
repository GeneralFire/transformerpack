//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file AmiIpmiBmcCommon.h
    IPMI Host to Bmc Common Structures and Function Prototypes.

*/

#ifndef _AMI_IPMI_BMC_COMMON_H_
#define _AMI_IPMI_BMC_COMMON_H_

//---------------------------------------------------------------------------

#include "AmiIpmiNetFnAppDefinitions.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define AMI_SM_IPMI_BMC_SIGNATURE SIGNATURE_32 ('I', 'P', 'M', 'I')

#define AMI_IPMI_INSTANCE_FROM_EFI_SM_IPMI_BMC_THIS(a) CR ( \
                                                            a, \
                                                            AMI_IPMI_BMC_INSTANCE_DATA, \
                                                            IpmiTransport, \
                                                            AMI_SM_IPMI_BMC_SIGNATURE \
                                                          )

#define AMI_IPMI_INSTANCE_FROM_INTERFACE_THIS(a) CR ( \
                                                       a, \
                                                       AMI_IPMI_BMC_INSTANCE_DATA, \
                                                       Interface, \
                                                       AMI_SM_IPMI_BMC_SIGNATURE \
                                                    )

#define AMI_BMC_LUN                                         0x00
#define AMI_IPMI_GET_SET_IN_PROGRESS_RETRY_COUNT            10
#define AMI_IPMI_BIT_CLEAR                                  0
#define AMI_IPMI_SELECTOR_NONE                              0
#define AMI_IPMI_CLEAR_FLAG                                 0
#define AMI_IPMI_SET_FLAG                                   1
#define AMI_IPMI_STALL                                      1000
#define AMI_MIN_TO_100MS                                    60 * 10

// Completion code macros.
#define AMI_IPMI_COMPLETION_CODE_SUCCESS                     0x00
#define AMI_IPMI_COMPLETION_CODE_DEVICE_SPECIFIC_START       0x01
#define AMI_IPMI_COMPLETION_CODE_DEVICE_SPECIFIC_END         0x7E
#define AMI_IPMI_COMPLETION_CODE_COMMAND_SPECIFIC_START      0x80
#define AMI_IPMI_COMPLETION_CODE_COMMAND_SPECIFIC_END        0xBE
#define AMI_IPMI_MAX_SOFT_COUNT                              10

#define AMI_IPMI_ERROR_COMPLETION_CODE(a)    !((a == AMI_IPMI_COMPLETION_CODE_SUCCESS) || \
                                             ((a >= AMI_IPMI_COMPLETION_CODE_DEVICE_SPECIFIC_START) &&\
                                              (a <= AMI_IPMI_COMPLETION_CODE_DEVICE_SPECIFIC_END)) ||\
                                             ((a >= AMI_IPMI_COMPLETION_CODE_COMMAND_SPECIFIC_START) &&\
                                              (a <= AMI_IPMI_COMPLETION_CODE_COMMAND_SPECIFIC_END))\
                                             )

#define EFI_IPMI_CMOS_CLEAR_VARIABLE_GUID \
    { \
        0x675b8766, 0x7f03, 0x4181, { 0x9b, 0xe1, 0x73, 0xbc, 0x45, 0xdf, 0xf7, 0x99 } \
    }

#define AMI_PEI_IPMI_HOB_GUID \
    { \
        0xcb4d3e13, 0x1e34, 0x4373, { 0x8a, 0x81, 0xe9, 0x0, 0x10, 0xf1, 0xdb, 0xa4 } \
    }

/** @internal
    BMC Interface status.
*/
typedef enum {
    AmiBmcStatusOk,                 ///< Bmc status Ok.
    AmiBmcStatusSoftFail,           ///< Bmc status Soft fail.
    AmiBmcStatusHardFail,           ///< Bmc status Hard fail.
    AmiBmcStatusUpdateInProgress    ///< Bmc status Update in progress.
} AMI_BMC_STATUS;

/** @internal
    Host to BMC Interface Type.
*/
typedef enum {
    AmiSysInterfaceUnknown,    ///< Unknown interface type.
    AmiSysInterfaceKcs,        ///< Kcs interface.
    AmiSysInterfaceSmic,       ///< Smic interface.
    AmiSysInterfaceBt,         ///< Bt interface.
    AmiSysInterfaceSsif,       ///< Ssif interface.
    AmiSysInterfaceIpmb,       ///< Ipmb interface.
    AmiSysInterfaceUsb,        ///< Usb interface.
    AmiSysInterfaceOem1,       ///< Oem1 interface.
    AmiSysInterfaceOem2,       ///< Oem2 interface.
    AmiSysInterfaceMax         ///< Maximum interface type.
} AMI_SYSTEM_INTERFACE_TYPE;

/** @inrernal
    Ipmi Interface Access Type.
*/
typedef enum {
    AmiIpmiMmioAccess,  ///< Mmio Access.
    AmiIpmiIoAccess     ///< Io Access.
} AMI_IPMI_ACCESS_TYPE;

/** @internal
    Ipmi Interface state.
*/
typedef enum {
    AmiIpmiInterfaceNotReady,       ///< Interface Not Ready.
    AmiIpmiInterfaceInitialized,    ///< Interface Initialized.
    AmiIpmiInterfaceInitError,      ///< Interface Initialization Error.
} AMI_IPMI_INTERFACE_STATE;

/** @internal
    Ipmi Pei data HOB.
*/
typedef struct _AMI_PEI_IPMI_DATA_HOB AMI_PEI_IPMI_DATA_HOB;

struct _AMI_PEI_IPMI_DATA_HOB {
    /// IpmiInstance pointer.
    UINTN                     IpmiInstance;
    AMI_PEI_IPMI_DATA_HOB    *PreMemIpmiDataHobPtr;   ///< HOB Data pointer before Memory discovered
};

/** @internal
    KCS Interface.
*/
typedef struct {
    /// Interface state.
    UINT8       InterfaceState;
    UINT16      DataPort;           ///< Data port.
    UINT16      CommandPort;        ///< Command port.
    UINTN       MmioBaseAddress;    ///< Mmio base address.
    UINTN       BaseAddressRange;   ///< Mmio base address range to
                                    ///< differentiate port address.
    UINT8       AccessType;         ///< Access type - IO or MMIO.
    UINT16      KcsRetryCounter;    ///< Retry counter.
    UINT8       KcsSoftErrorCount;  ///< Soft error count.
    BOOLEAN     KcsTransportLocked; ///< Interface lock.
} AMI_KCS_SYSTEM_INTERFACE;

/** @internal
    BT Interface.
*/
typedef struct {
    /// Interface state.
    UINT8       InterfaceState;
    UINT16      CtrlPort;            ///< Control port.
    UINT16      ComBuffer;           ///< Communication buffer port.
    UINT16      IntMaskPort;         ///< Interrupt mask port.
    UINTN       MmioBaseAddress;     ///< Mmio base address.
    UINTN       BaseAddressRange;    ///< Mmio base address range to
                                     ///< differentiate port address.
    UINT8       AccessType;          ///< Access type - IO or MMIO.
    UINT32      BtRetryCount;        ///< Delay counter for retry.
    UINT8       BtSoftErrorCount;    ///< Soft error count.
    BOOLEAN     BtTransportLocked;   ///< Interface lock.
    UINT8       HosttoBmcBufferSize; ///< Host to Bmc Buffer Size.
    UINT8       BmctoHostBufferSize; ///< Bmc to Host Buffer Size.
} AMI_BT_SYSTEM_INTERFACE;

/** @internal
    SSIF Interface.
*/
typedef struct {
    /// Interface state.
    UINT8       InterfaceState;
    EFI_GUID    SsifInterfaceApiGuid; ///< Smbus instance guid.
    UINTN       SsifInterfaceApiPtr;  ///< Smbus instance pointer.
    UINT8       RwSupport;            ///< Read-write support.
    UINT16      SsifRetryCounter;     ///< Retry counter.
    BOOLEAN     PecSupport;           ///< Packet Error Check support.
    BOOLEAN     SmbAlertSupport;      ///< Smbus alert support.
    UINT8       SsifSoftErrorCount;   ///< Soft error count.
    BOOLEAN     SsifTransportLocked;  ///< Interface lock.
} AMI_SSIF_SYSTEM_INTERFACE;

/** @internal
    IPMB Interface.
*/
typedef struct {
    /// Interface state.
    UINT8       InterfaceState;
    EFI_GUID    IpmbInterfaceApiGuid; ///< Ipmb instance guid.
    UINTN       IpmbInterfaceApiPtr;  ///< Ipmb instance pointer.
    UINT8       IpmbSoftErrorCount;   ///< Soft error count.
    BOOLEAN     IpmbTransportLocked;  ///< Interface lock.
} AMI_IPMB_SYSTEM_INTERFACE;

/** @internal
    USB Interface.
*/
typedef struct {
    /// Interface state.
    UINT8       InterfaceState;
    EFI_GUID    UsbInterfaceApiGuid;  ///< Usb instance guid.
    UINTN       UsbInterfaceApiPtr;   ///< Usb instance pointer.
    UINT8       BulkInEndpointAddr;   ///< Bulk in endpoint address.
    UINT8       BulkOutEndpointAddr;  ///< Bulk out endpoint address.
    UINT32      BOTCommandTag;        ///< Bulk Of Transfer command tag.
    VOID        *Context;             ///< In PEI phase, it contains **PeiServices.
    UINT16      BulkTransferTimeOut;  ///< Timeout variable to get timeout provided in PCD.
    UINTN       UsbCommBufferPtr;     ///< Usb communication buffer pointer.
    BOOLEAN     UsbTransportLocked;   ///< Interface lock.
    EFI_HANDLE  UsbIoHandle;          ///< UsbIo Handle.
} AMI_USB_SYSTEM_INTERFACE;

/** @internal
    OEM1 Interface.
*/
typedef struct {
    /// Interface state.
    UINT8       InterfaceState;
    EFI_GUID    Oem1InterfaceApiGuid; ///< Oem1 instance guid.
    UINTN       Oem1InterfaceApiPtr;  ///< Oem1 instance pointer.
    UINT8       Oem1SoftErrorCount;   ///< Soft error count.
    BOOLEAN     Oem1TransportLocked;  ///< Interface lock.
} AMI_OEM1_SYSTEM_INTERFACE;

/** @internal
    OEM2 Interface.
*/
typedef struct {
    /// Interface state.
    UINT8       InterfaceState;
    EFI_GUID    Oem2InterfaceApiGuid; ///< Oem2 instance guid.
    UINTN       Oem2InterfaceApiPtr;  ///< Oem2 instance pointer.
    UINT8       Oem2SoftErrorCount;   ///< Soft error count.
    BOOLEAN     Oem2TransportLocked;  ///< Interface lock.
} AMI_OEM2_SYSTEM_INTERFACE;

/** @internal
    System Interface.
*/
typedef struct {
    /// Kcs interface pointer.
    AMI_KCS_SYSTEM_INTERFACE      *Kcs;
    AMI_BT_SYSTEM_INTERFACE       Bt;      ///< Bt interface.
    AMI_SSIF_SYSTEM_INTERFACE     Ssif;    ///< Ssif interface.
    AMI_IPMB_SYSTEM_INTERFACE     Ipmb;    ///< Ipmb interface.
    AMI_USB_SYSTEM_INTERFACE      *Usb;    ///< Usb interface pointer.
    AMI_OEM1_SYSTEM_INTERFACE     Oem1;    ///< Oem1 interface.
    AMI_OEM2_SYSTEM_INTERFACE     Oem2;    ///< Oem2 interface.
} AMI_SYSTEM_INTERFACE;

typedef struct _AMI_IPMI_TRANSPORT_INSTANCE AMI_IPMI_TRANSPORT_INSTANCE;

/**
    Ipmi send command API for default Host to BMC interface.

    @param[in]      This                Pointer to Ipmi Protocol/Ppi.
    @param[in]      NetFunction         Net function of the command.
    @param[in]      Lun                 Logical Unit Number of the command.
    @param[in]      Command             Command to be sent to BMC.
    @param[in]      CommandData         Command data to be sent along with
                                        Command.
    @param[in]      CommandDataSize     Command Data size.
    @param[out]     ResponseData        Pointer to the response data buffer.
    @param[in, out] ResponseDataSize    Pointer to the response data size.

    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval Others                  Status returned from BMC while executing
                                    the command.

**/
typedef
EFI_STATUS
(EFIAPI *AMI_IPMI_SEND_COMMAND) (
  IN     AMI_IPMI_TRANSPORT_INSTANCE    *This,
  IN     UINT8                          NetFunction,
  IN     UINT8                          Lun,
  IN     UINT8                          Command,
  IN     UINT8                          *CommandData,
  IN     UINT8                          CommandDataSize,
  OUT    UINT8                          *ResponseData,
  IN OUT UINT8                          *ResponseDataSize
);

/**
    Ipmi get bmc status API which returns BMC status, channel type etc.

    @param[in]  This        Pointer to Ipmi Protocol/Ppi.
    @param[out] BmcStatus   Pointer to Bmc status.
    @param[out] ComAddress  Pointer to Bmc communication address.

    @return EFI_STATUS  Status returned while getting the Bmc status.

**/
typedef
EFI_STATUS
(EFIAPI *AMI_IPMI_GET_BMC_STATUS) (
  IN  AMI_IPMI_TRANSPORT_INSTANCE   *This,
  OUT AMI_BMC_STATUS                *BmcStatus,
  OUT AMI_SM_COM_ADDRESS            *ComAddress
);

/**
    Ipmi send command API for specific Host to BMC interface.
    It updates completion code.

    @param[in]      This                Pointer to Ipmi Protocol/Ppi.
    @param[in]      NetFunction         Net function of the command.
    @param[in]      Lun                 Logical Unit Number of the command.
    @param[in]      Command             Command to be sent to BMC.
    @param[in]      CommandData         Command data to be sent along with
                                        Command.
    @param[in]      CommandDataSize     Command Data size.
    @param[out]     ResponseData        Pointer to the response data buffer.
    @param[in, out] ResponseDataSize    Pointer to the response data size.
    @param[out]     CompletionCode      Pointer to completion code.
    @param[in]      InterfaceType       Interface type.

    @retval EFI_UNSUPPORTED     Interface type is invalid or CommandDataSize
                                or ResponseDataSize is invalid for given
                                InterfaceType.
    @retval Others              Status returned from BMC while executing
                                the command.

**/
typedef
EFI_STATUS
(EFIAPI *AMI_IPMI_SEND_COMMAND_EX) (
  IN     AMI_IPMI_TRANSPORT_INSTANCE    *This,
  IN     UINT8                          NetFunction,
  IN     UINT8                          Lun,
  IN     UINT8                          Command,
  IN     UINT8                          *CommandData,
  IN     UINT32                         CommandDataSize,
  OUT    UINT8                          *ResponseData,
  IN OUT UINT32                         *ResponseDataSize,
  OUT    UINT8                          *CompletionCode,
  IN     AMI_SYSTEM_INTERFACE_TYPE      InterfaceType
);

/**
    Ipmi transport Ppi/Protocol common instance.
*/
struct _AMI_IPMI_TRANSPORT_INSTANCE {
    /// Ipmi Ppi/Protocol revision.
    UINT64                      Revision;
    /// Send Ipmi Command for default interface.
    AMI_IPMI_SEND_COMMAND       SendIpmiCommand;
    /// Get Bmc status.
    AMI_IPMI_GET_BMC_STATUS     GetBmcStatus;
    /// Send Ipmi Command with completion code and specific interface type.
    AMI_IPMI_SEND_COMMAND_EX    SendIpmiCommandEx;
};

/** @internal
    Ipmi Bmc instance data.
*/
typedef struct {
    /// Bmc signature.
    UINTN                       Signature;
    UINT8                       SlaveAddress;   ///< Bmc slave address.
    AMI_BMC_STATUS              BmcStatus;      ///< Bmc status.
    AMI_IPMI_TRANSPORT_INSTANCE IpmiTransport;  ///< Ipmi transport instance.
    AMI_SYSTEM_INTERFACE        Interface;      ///< System interface.
} AMI_IPMI_BMC_INSTANCE_DATA;

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern EFI_GUID gEfiIpmiCmosClearVariableGuid;
extern EFI_GUID gAmiPeiIpmiHobGuid;

//---------------------------------------------------------------------------

#endif  // #ifndef _AMI_IPMI_BMC_COMMON_H_

