//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file AmiBmcCommonInterfaceLib.h
    Bmc Common interface library functions.

*/

#ifndef _AMI_BMC_COMMON_INTERFACE_LIB_H_
#define _AMI_BMC_COMMON_INTERFACE_LIB_H_

//---------------------------------------------------------------------------

#include <AmiIpmiBmcCommon.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

/** @internal
    Interface Wrapper for send ipmi command Implementation.

    @param[in]      Interface           Pointer to System interface.
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
    @param[in]      Context             NULL here.

    @return EFI_STATUS  Status returned from BMC while executing the command.

**/
typedef 
EFI_STATUS
(AMI_IPMI_SEND_COMMAND_WRAPPER) (
  IN     AMI_SYSTEM_INTERFACE       *Interface,
  IN     UINT8                      NetFunction,
  IN     UINT8                      Lun,
  IN     UINT8                      Command,
  IN     UINT8                      *CommandData,
  IN     UINT32                     CommandDataSize,
  OUT    UINT8                      *ResponseData,
  IN OUT UINT32                     *ResponseDataSize,
  OUT    UINT8                      *CompletionCode,
  IN     AMI_SYSTEM_INTERFACE_TYPE  InterfaceType,
  IN     VOID                       *Context
);

/** @internal
    Initialize interface specific data.

    @param[in, out]  Interface  System interface pointer.

    @retval EFI_SUCCESS Interfaces are successfully initialized.
    @retval Others      Return status of the specific interface hook.

*/
typedef EFI_STATUS (AMI_IPMI_INIT_INTERFACE_DATA_HOOK) (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface
);

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

/** @internal
    Read 8 bit data from BMC port based on access type.

    @param[in] AccessType   Specifies MMIO or IO access.
    @param[in] Address      Specifies Address to read.

    @return UINT8   Data read.

**/
UINT8
AmiIpmiBmcRead8 (
  IN UINT8  AccessType,
  IN UINTN  Address
);

/** @internal
    Write 8 bit data to BMC port based on access type.

    @param[in] AccessType   Specifies MMIO or IO access.
    @param[in] Address      Specifies Address to write.
    @param[in] Data         Specifies data to be written.

    @return Data written.

**/
UINT8
AmiIpmiBmcWrite8 (
  IN UINT8  AccessType,
  IN UINTN  Address,
  IN UINT8  Data
);

/** @internal
    Acquire the lock to use the IPMI transport.

    @param[out] Lock    Pointer to Lock.

    @return VOID    Returns nothing.

**/
VOID
AmiIpmiTransportAcquireLock (
  OUT BOOLEAN   *Lock
);

/** @internal
    Release the lock of IPMI transport.

    @param[out] Lock    Pointer to Lock.

    @return VOID    Returns nothing.

**/
VOID
AmiIpmiTransportReleaseLock (
  OUT BOOLEAN   *Lock
);

/** @internal
    Returns the Lock state of IPMI transport.

    @param[in]  Lock      Pointer to Lock.

    @retval TRUE    IPMI transport is in lock state.
    @retval FALSE   IPMI transport is in release state.

*/
BOOLEAN
AmiIpmiIsIpmiTransportlocked (
  IN BOOLEAN    *Lock
);

/** @internal
    Updates the SoftErrorCount of specific interface based on the BMC Error input.

    @param[in]      BmcError        - BMC Error.
    @param[in, out] Interface       - Interface pointer to update soft error count.
    @param[in]      InterfaceType   - Interface type to communicate.

    @retval EFI_SUCCESS             Updated SoftErrorCount of specific interface.
    @retval EFI_INVALID_PARAMETER   Invalid Interface pointer or Interface type.

**/
EFI_STATUS
AmiIpmiUpdateSoftErrorCount (
  IN     UINT8                      BmcError,
  IN OUT AMI_SYSTEM_INTERFACE       *Interface,
  IN     AMI_SYSTEM_INTERFACE_TYPE  InterfaceType
);

/** @internal
    Initialize interface specific data.

    @param[in, out]  Interface  System interface pointer.

    @retval EFI_SUCCESS Interfaces are successfully initialized.
    @retval Others      Return status of the specific interface hook.

**/
EFI_STATUS
AmiIpmiInitInterfaceData (
  IN OUT AMI_IPMI_BMC_INSTANCE_DATA   *IpmiInstance
);

/** @internal
    Updates Command specific retry counter based on the availability.
    If not available, updates default retry counter value.

    @param[in] NetFunction  Net function of the command.
    @param[in] Command      Command to be sent to BMC.

    @return UINT32  Retry count value. Returns 0 if none of the delay
                    is provided.

    @note   This function is only for KCS and BT interface.

**/
UINT32
AmiIpmiUpdateCommandSpecificDelay (
  IN UINT8  NetFunction,
  IN UINT8  Command
);

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
EFI_STATUS
EFIAPI
EfiIpmiSendCommand (
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
EFI_STATUS
EFIAPI
EfiIpmiGetBmcStatus (
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
EFI_STATUS
EFIAPI
EfiIpmiSendCommandEx (
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
    Execute the Self Test command with specific interface 

    @param IpmiInstance       IpmiInstance Data structure pointer.
     @param InterfaceType     Interface Type

    @return EFI_STATUS       Return Status
**/
EFI_STATUS
SelfTestBySpecificInterface (
  IN  AMI_IPMI_BMC_INSTANCE_DATA      *IpmiInstance,
  IN  AMI_SYSTEM_INTERFACE_TYPE       InterfaceType,
  OUT AMI_BMC_STATUS                  *BmcStatus
);

#endif

