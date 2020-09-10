//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file BmcElogCommon.h
    Baseboard Event Log common headers, macros and structures.

*/

#ifndef _BMC_ELOG_COMMON_H_
#define _BMC_ELOG_COMMON_H_

//---------------------------------------------------------------------------

#include <Token.h>
#include <IndustryStandard/IpmiNetFnSensorEvent.h>
#include <AmiIpmiNetFnStorageDefinitions.h>
#include <AmiServerMgmtSetupVariable.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/AmiSelLib.h>
#include <Library/AmiIpmiLib.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Protocol/RedirElogProtocol.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define MAX_ELOG_TEMP_DATA              100

#define INSTANCE_FROM_EFI_SM_ELOG_REDIR_THIS(a) \
  CR(a, BMC_ELOG_INSTANCE_DATA, BmcElog, EFI_SM_ELOG_REDIR_SIGNATURE)

/**
    BMC Elog instance data structure.
*/
typedef struct {
    /// Bmc signature.
    UINTN                         Signature;
    EFI_SM_ELOG_REDIR_TYPE        DataType;                           ///< Error logging type.
    UINT8                         TempData[MAX_ELOG_TEMP_DATA + 1];   ///< Buffer to store temp Elog data.
    EFI_SM_ELOG_REDIR_PROTOCOL    BmcElog;                            ///< Bmc Elog protocol.
} BMC_ELOG_INSTANCE_DATA;

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern BMC_ELOG_INSTANCE_DATA   *gRedirProtoPrivate;
extern EFI_IPMI_TRANSPORT       *gIpmiTransport;

//---------------------------------------------------------------------------

/**
    Sends the Event-Log data to the destination. Destination can be a remote
    target like LAN, ICMB, local targets BMC, IPMB or a Firmware-Volume.

    @param[in]  This        Instance of EFI_SM_ELOG_PROTOCOL.
    @param[in]  ElogData    Pointer to the Event-Log data that needs to be
                            recorded.
    @param[in]  DataType    Type of Elog Data that is being recorded.
                            Elog is redirected based upon this information.
    @param[in]  Size        Size of the data to be logged.
    @param[out] RecordId    This is the array of record IDs sent by the
                            target. This can be used to retrieve the records
                            or erase the records.

    @retval EFI_SUCCESS             Event Log was recorded successfully.
    @retval EFI_NOT_FOUND           Event Log target not found.
    @retval EFI_INVALID_PARAMETER   Invalid Parameter.
    @retval EFI_OUT_OF_RESOURCES    Not enough resources to record data.
    @retval Others                  Status returned from function calls used.

**/
EFI_STATUS
EFIAPI
EfiSetBmcElogData (
  IN  EFI_SM_ELOG_REDIR_PROTOCOL    *This,
  IN  UINT8                         *ElogData,
  IN  EFI_SM_ELOG_REDIR_TYPE        DataType,
  IN  BOOLEAN                       AlertEvent,
  IN  UINTN                         Size,
  OUT UINT64                        *RecordId
);

/**
    Gets the Event-Log data from the destination. Destination can be a
    remote target like LAN, ICMB, local targets BMC, IPMB or a
    Firmware-Volume.

    @param[in]      This        Instance of EFI_SM_ELOG_PROTOCOL.
    @param[out]     ElogData    Pointer to the Event-Log data that needs to
                                be retrieved.
    @param[in]      DataType    Type of Elog Data. Elog is redirected based
                                upon this information.
    @param[in, out] Size        Size of the data to be retrieved. This size
                                should be equal to or more than the size of
                                data to be retrieved. On return, it contains
                                the actual data size retrieved.
    @param[in, out] RecordId    This is record id to retrieve the specific
                                record.

    @retval EFI_SUCCESS             Event Log was retrieved successfully.
    @retval EFI_NOT_FOUND           Event Log target not found.
    @retval EFI_INVALID_PARAMETER   Invalid Parameter.
    @retval EFI_OUT_OF_RESOURCES    Not enough resources to record data.
    @retval EFI_BUFFER_TOO_SMALL    Target buffer is too small to retrieve
                                    the data.
    @retval Others                  Status returned from function calls used.

**/
EFI_STATUS
EFIAPI
EfiGetBmcElogData (
  IN     EFI_SM_ELOG_REDIR_PROTOCOL     *This,
  OUT    UINT8                          *ElogData,
  IN     EFI_SM_ELOG_REDIR_TYPE         DataType,
  IN OUT UINTN                          *Size,
  IN OUT UINT64                         *RecordId
);

/**
    Erases the Event-Log data from the destination. Destination can be a
    remote target like LAN, ICMB, local targets BMC, IPMB or a
    Firmware-Volume.

    @param[in]      This        Instance of EFI_SM_ELOG_PROTOCOL.
    @param[in]      DataType    Type of Elog Data that is being Erased. Elog
                                is redirected based upon this information.
    @param[in, out] RecordId    This is the RecordId of the data to be
                                erased. If RecordId is NULL, all the records
                                on the database are erased if permitted by
                                the target. Contains the deleted RecordId on
                                return. Record Id value is assumed as Hexa
                                Decimal with 2 bytes of data.

    @retval EFI_SUCCESS         Event Log was Erased successfully.
    @retval EFI_NOT_FOUND       Event Log target not found.
    @retval EFI_UNSUPPORTED     Delete sel entry command is not supported.
    @retval EFI_NOT_READY       Erasing event log is in progress, so cannot
                                erase.
    @retval Others              Status returned from function calls used.

**/
EFI_STATUS
EFIAPI
EfiEraseBmcElogData (
  IN     EFI_SM_ELOG_REDIR_PROTOCOL     *This,
  IN     EFI_SM_ELOG_REDIR_TYPE         DataType,
  IN OUT UINT64                         *RecordId
);

/**
    This API enables/disables Event Log.

    @param[in]      This        Instance of EFI_SM_ELOG_PROTOCOL.
    @param[in]      DataType    Type of Elog Data that is being Activated.
                                Activation (Enable/Disable) is redirected
                                based upon this information.
    @param[in]      EnableElog  Enables (TRUE)/Disables(FALSE) Event Log.
                                If NULL, just return the Current ElogStatus.
    @param[out]     ElogStatus  Current (New) Status of Event Log.
                                Enabled (TRUE) or Disabled (FALSE).

    @retval EFI_SUCCESS     Event Log is enabled/disabled successfully.
    @retval EFI_NOT_FOUND   Event Log target not found.
    @retval Others          Status returned from send ipmi command.

**/
EFI_STATUS
EFIAPI
EfiActivateBmcElog (
  IN  EFI_SM_ELOG_REDIR_PROTOCOL    *This,
  IN  EFI_SM_ELOG_REDIR_TYPE        DataType,
  IN  BOOLEAN                       *EnableElog,
  OUT BOOLEAN                       *ElogStatus
);

#endif


