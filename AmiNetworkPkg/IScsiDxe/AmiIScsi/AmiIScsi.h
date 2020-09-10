//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AmiIScsi.h
    Header file to declare the AMI Override functions for the IScsi EDKII driver 

**/
#ifndef _AMI_ISCSI_LIB_H_
#define _AMI_ISCSI_LIB_H_

#include <Token.h>
#include <Uefi.h>
#include <Library/AmiNetworkLib.h>
#include "IScsiDxe/IScsiImpl.h"
#include "AmiLib.h"
#include <AmiIScsiElink.h>
#include <Protocol/Ip4.h>       
#include <Library/AmiNetworkPostManagerLib.h>       
#include <Library/iSCSICHAPEncrypt.h>               

#if(ENABLE_CHAP_SECRET_ENCRYPTION)
#include "IScsiDhcp.h"
#include "IScsiConfigNVDataStruc.h"
#endif

#define DYNAMIC_PRIORITY_LIST_QUESTION_ID    QUESTION_ID (PriorityOrderedList)
#define DYNAMIC_PRIORITY_LIST_VAR_OFFSET     VAR_OFFSET  (PriorityOrderedList)

#define ATTEMPT_NAME_MAX_SIZE      97
#define ATTEMPT_NAME_IFR_MAX_SIZE  96
#define SETUP_ATTEMPT              1
#define REDFISH_ATTEMPT            2
#define RSD_ATTEMPT                3
#define ISCSI_MAX_ATTEMPT_TYPE     3

#define LOCAL_ATTEMPT_START                0x252
#define LOCAL_ATTEMPT_END                  0x253

#define RSD_ATTEMPT_START                  0x250
#define RSD_ATTEMPT_END                    0x251
#define KEY_RSD_ISCSI_FORM                 0x200
#define KEY_RSD_UPDATE_ATTEMPT_LIST        0x203
#define KEY_RSD_UPDATE_ATTEMPT_BASE        0x400
#define RSD_ISCSI_NO_CHAP                  0x00
#define RSD_ISCSI_UNI_CHAP                 0x01
#define RSD_ISCSI_MUTUAL_CHAP              0x02

#define REDFISH_MAC_START                  0x255
#define REDFISH_MAC_END                    0x256
#define KEY_REDFISH_ISCSI_FORM             0x300
#define KEY_REDFISH_UPDATE_ATTEMPT_LIST    0x303
#define KEY_REDFISH_UPDATE_ATTEMPT_BASE    0x500
#define REDFISH_ISCSI_NO_CHAP              0x00
#define REDFISH_ISCSI_UNI_CHAP             0x01
#define REDFISH_ISCSI_MUTUAL_CHAP          0x02

#if (ENABLE_CHAP_SECRET_ENCRYPTION)
EFI_STATUS PrepareToEncrypt(ISCSI_ATTEMPT_CONFIG_NVDATA  *Attempt, UINT8 OpCode);
EFI_STATUS IscsiManageEncryptionList (CHAR8 *CHAPSecret,UINT8 *CHAPLen,CHAR8 *RevCHAPSecret,UINT8 *RevCHAPLen,UINT8 AttemptID,UINT8 OpCode);
BOOLEAN    IsValidChapLength(ISCSI_CHAP_AUTH_CONFIG_NVDATA  ChapData);
#endif

/**
  Callback function when user presses "Change Priority".

  @retval EFI_INVALID_PARAMETER  Any parameter is invalid.
  @retval EFI_OUT_OF_RESOURCES   Does not have sufficient resources to finish this
                                 operation.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
IScsiConfigDisplayPriorityList (
    ISCSI_CONFIG_IFR_NVDATA     *IfrNvData
);

/**
  Callback function when user presses "Commit Changes and Exit" in Change Priority Order.

  @param[in]  IfrNvData          The IFR nv data.

  @retval EFI_OUT_OF_RESOURCES   Does not have sufficient resources to finish this
                                 operation.
  @retval EFI_NOT_FOUND          Cannot find the corresponding variable.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
IScsiConfigChangePriority (
    IN ISCSI_CONFIG_IFR_NVDATA  *IfrNvData
);

/**
  Function to set default Priority when user havent set any priority.

  @retval EFI_OUT_OF_RESOURCES   Does not have sufficient resources to finish this
                                 operation.
  @retval EFI_NOT_FOUND          Cannot find the corresponding variable.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
IScsiSetDefaultPriority (
);

/**
  Function to allow multiple targets on a NIC without MPIO enabled

  @param[out] UINTN    Returns the valid Target count.

**/
UINTN
ConfigureMultipleTargets (
);

/**
  Function to forcefully set / get initiator name.

  @param[in, out]  BufferSize    Size of the buffer in bytes pointed to by Buffer /
                                 Actual size of the variable data buffer.
  @param[out]      Buffer        Pointer to the buffer for data to be read.
                                 The data is a null-terminated UTF-8 encoded string.
                                 The maximum length is 223 characters, including the null-terminator.
  @param[in]       BOOLEAN       BOOLEAN to verify whether it is Form init or get/set.

**/
VOID 
ForcefullySetInitiatorName (
    IN OUT  UINTN     *BufferSize,
    IN OUT  VOID      *Buffer,
    IN      BOOLEAN   FormInit
);

/**
  Function to change Attempt Order with Priority.

  @param[in]  IfrNvData          The IFR nv data.

  @retval EFI_OUT_OF_RESOURCES   Does not have sufficient resources to finish this
                                 operation.
  @retval EFI_NOT_FOUND          Cannot find the corresponding variable.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
OrderAttemptsWithPriority (
    IN ISCSI_CONFIG_IFR_NVDATA  *IfrNvData
);

/**
  Get the various configuration data.

  @param[in]  Private   The iSCSI driver data.
  @param[in]  SameNicIndex   Boolean for checking same NIC

  @retval EFI_SUCCESS            The configuration data is retrieved.
  @retval EFI_NOT_FOUND          This iSCSI driver is not configured yet.
  @retval EFI_OUT_OF_RESOURCES   Failed to allocate memory.

**/
EFI_STATUS
AmiIScsiGetConfigData (
    IN ISCSI_DRIVER_DATA  *Private
);

/**
  This function is called to provide results data to the driver.
  This data consists of a unique key that is used to identify
  which data is either being passed back or being asked for.

  @param[in]       This          Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]       Action        Specifies the type of action taken by the browser.
  @param[in]       QuestionId    A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect. The format of the data tends to
                                 vary based on the opcode that generated the callback.
  @param[in]       Type          The type of value for the question.
  @param[in, out]  Value         A pointer to the data being sent to the original
                                 exporting driver.
  @param[out]      ActionRequest On return, points to the action requested by the
                                 callback function.

  @retval EFI_SUCCESS            The callback successfully handled the action.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the
                                 variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be saved.
  @retval EFI_UNSUPPORTED        The specified Action is not supported by the
                                 callback.
**/
EFI_STATUS
EFIAPI
AmiIScsiFormCallback (
    IN CONST  EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
    IN        EFI_BROWSER_ACTION               Action,
    IN        EFI_QUESTION_ID                  QuestionId,
    IN        UINT8                            Type,
    IN OUT    EFI_IFR_TYPE_VALUE               *Value,
    OUT       EFI_BROWSER_ACTION_REQUEST       *ActionRequest
);

/**
  Callback function when user presses "Add an Attempt".

  @retval EFI_OUT_OF_RESOURCES   Does not have sufficient resources to finish this
                                 operation.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
AmiIScsiConfigAddAttempt (
    VOID
);

/**
  Callback function when user presses "Delete Attempts".

  @param[in]  IfrNvData          The IFR nv data.

  @retval EFI_INVALID_PARAMETER  Any parameter is invalid.
  @retval EFI_BUFFER_TOO_SMALL   The buffer in UpdateData is too small.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
IScsiConfigDisplayDeleteAttempts (
    IN ISCSI_CONFIG_IFR_NVDATA  *IfrNvData
);

/**
  Callback function when a user presses "Attempt *" or when a user selects a NIC to
  create the new attempt.

  @param[in]  KeyValue           A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param[in]  IfrNvData          The IFR nv data.

  @retval EFI_OUT_OF_RESOURCES   Does not have sufficient resources to finish this
                                 operation.
  @retval EFI_NOT_FOUND          Cannot find the corresponding variable.
  @retval EFI_UNSUPPORTED        Can not create more attempts.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
AmiIScsiConfigProcessDefault (
    IN  EFI_QUESTION_ID              KeyValue,
    IN  ISCSI_CONFIG_IFR_NVDATA      *IfrNvData
);

/**
  Get the existing attempt config data from global structure by the NicIndex.

  @param[in]  NewAttempt         The created new attempt
  @param[in]  IScsiMode          The IScsi Mode of the new attempt, Enabled or
                                 Enabled for MPIO.

  @return                        Pointer to the existing attempt config data which
                                 has the same NICIndex as the new created attempt.
  @retval     NULL               The attempt with NicIndex does not exist.

**/
ISCSI_ATTEMPT_CONFIG_NVDATA *
IScsiConfigGetAttemptByNic (
    IN ISCSI_ATTEMPT_CONFIG_NVDATA *NewAttempt,
    IN UINT8                       IScsiMode
);

/**
  Callback function when user presses "Commit Changes and Exit" in Delete Attempts or Delete Attempts by Keyword.

  @param[in]  IfrNvData          The IFR NV data.

  @retval EFI_NOT_FOUND          Cannot find the corresponding variable.
  @retval EFI_SUCCESS            The operation is completed successfully.
  @retval EFI_ABOTRED            This operation is aborted cause of error
                                 configuration.
  @retval EFI_OUT_OF_RESOURCES   Fail to finish the operation due to lack of
                                 resources.

**/
EFI_STATUS
IScsiConfigDeleteAttempts (
    IN ISCSI_CONFIG_IFR_NVDATA  *IfrNvData
);

/**
  Convert the IP address into a dotted string.

  @param[in]  Ip        The IP address.
  @param[in]  Ipv6Flag  Indicates whether the IP address is version 4 or version 6.
  @param[out] Str       The formatted IP string.

**/
VOID
IScsiIpToStr (
    IN  EFI_IP_ADDRESS    *Ip,
    IN  BOOLEAN           Ipv6Flag,
    OUT CHAR16            *Str
);

/**
  Check whether the input IP address is valid.

  @param[in]  Ip        The IP address.
  @param[in]  IpMode    Indicates iSCSI running on IP4 or IP6 stack.

  @retval     TRUE      The input IP address is valid.
  @retval     FALSE     Otherwise

**/
BOOLEAN
IpIsUnicast (
    IN EFI_IP_ADDRESS *Ip,
    IN  UINT8          IpMode
);

/**
  Start to manage the controller. This is the worker function for
  IScsiIp4(6)DriverBindingStart.

  @param[in]  Image                Handle of the image.
  @param[in]  ControllerHandle     Handle of the controller.
  @param[in]  IpVersion            IP_VERSION_4 or IP_VERSION_6.

  @retval EFI_SUCCES            This driver was started.
  @retval EFI_ALREADY_STARTED   This driver is already running on this device.
  @retval EFI_INVALID_PARAMETER Any input parameter is invalid.
  @retval EFI_NOT_FOUND         There is no sufficient information to establish
                                the iScsi session.
  @retval EFI_OUT_OF_RESOURCES  Failed to allocate memory.
  @retval EFI_DEVICE_ERROR      Failed to get TCP connection device path.
  @retval EFI_ACCESS_DENIED     The protocol could not be removed from the Handle
                                because its interfaces are being used.

**/
EFI_STATUS
AmiIScsiStart (
    IN EFI_HANDLE                   Image,
    IN EFI_HANDLE                   ControllerHandle,
    IN UINT8                        IpVersion
);

/**
  Check whether an iSCSI HBA adapter already installs an AIP instance with
  network boot policy matching the value specified in PcdIScsiAIPNetworkBootPolicy.
  If yes, return EFI_SUCCESS.

  @retval EFI_SUCCESS              Found an AIP with matching network boot policy.
  @retval EFI_NOT_FOUND            AIP is unavailable or the network boot policy
                                   not matched.
**/
EFI_STATUS
IScsiCheckAip (
    VOID
);

/**
  Parse IsId in string format and convert it to binary.

  @param[in]        String  The buffer of the string to be parsed.
  @param[in, out]   IsId    The buffer to store IsId.

  @retval EFI_SUCCESS              The operation finished successfully.
  @retval EFI_INVALID_PARAMETER    Any input parameter is invalid.

**/
EFI_STATUS
AmiIScsiParseIsIdFromString (
    IN CONST CHAR16                    *String,
    IN OUT   UINT8                     *IsId
);
/**
  Convert the IFR data to iSCSI configuration data.

  @param[in]       IfrNvData              Point to ISCSI_CONFIG_IFR_NVDATA.
  @param[in, out]  Attempt                The iSCSI attempt config data.

  @retval EFI_INVALID_PARAMETER  Any input or configured parameter is invalid.
  @retval EFI_NOT_FOUND          Cannot find the corresponding variable.
  @retval EFI_OUT_OF_RESOURCES   The operation is failed due to lack of resources.
  @retval EFI_ABORTED            The operation is aborted.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
AmiIScsiConvertIfrNvDataToAttemptConfigData (
    IN ISCSI_CONFIG_IFR_NVDATA          *IfrNvData,
    IN OUT ISCSI_ATTEMPT_CONFIG_NVDATA  *Attempt
);

/**
  Update the component name for the iSCSI instance.

  @param[in]  IScsiExtScsiPassThru  A pointer to the EFI_EXT_SCSI_PASS_THRU_PROTOCOL instance.
  @param[in]  Ipv6Flag              TRUE if IP6 network stack is used.

  @retval EFI_SUCCESS               Update the ControllerNameTable of this instance successfully.
  @retval EFI_INVALID_PARAMETER     The input parameter is invalid.
  @retval EFI_UNSUPPORTED           Can't get the corresponding NIC info from the Controller handle.

**/
EFI_STATUS
AmiUpdateName (
    IN   EFI_EXT_SCSI_PASS_THRU_PROTOCOL *IScsiExtScsiPassThru,
    IN   BOOLEAN     Ipv6Flag
);

/**
  Callback function when user presses "Change Attempt Order".

  @retval EFI_INVALID_PARAMETER  Any parameter is invalid.
  @retval EFI_OUT_OF_RESOURCES   Does not have sufficient resources to finish this
                                 operation.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
AmiIScsiConfigDisplayOrderAttempts (
    ISCSI_CONFIG_IFR_NVDATA     *IfrNvData
);

/**
  Convert the iSCSI configuration data into the IFR data.

  @param[in]       Attempt                The iSCSI attempt config data.
  @param[in, out]  IfrNvData              The IFR nv data.

**/
VOID
IScsiConvertAttemptConfigDataToIfrNvData (
    IN ISCSI_ATTEMPT_CONFIG_NVDATA  *Attempt,
    IN OUT ISCSI_CONFIG_IFR_NVDATA  *IfrNvData
);

/**

  This function allows the caller to request the current
  configuration for one or more named elements. The resulting
  string is in <ConfigAltResp> format. Also, any and all alternative
  configuration strings shall be appended to the end of the
  current configuration string. If they are, they must appear
  after the current configuration. They must contain the same
  routing (GUID, NAME, PATH) as the current configuration string.
  They must have an additional description indicating the type of
  alternative configuration the string represents,
  "ALTCFG=<StringToken>". That <StringToken> (when
  converted from Hex UNICODE to binary) is a reference to a
  string in the associated string pack.

  @param[in]  This       Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.

  @param[in]  Request    A null-terminated Unicode string in
                         <ConfigRequest> format. Note that this
                         includes the routing information as well as
                         the configurable name / value pairs. It is
                         invalid for this string to be in
                         <MultiConfigRequest> format.

  @param[out] Progress   On return, points to a character in the
                         Request string. Points to the string's null
                         terminator if request was successful. Points
                         to the most recent "&" before the first
                         failing name / value pair (or the beginning
                         of the string if the failure is in the first
                         name / value pair) if the request was not successful.

  @param[out] Results    A null-terminated Unicode string in
                         <ConfigAltResp> format which has all values
                         filled in for the names in the Request string.
                         String to be allocated by the called function.

  @retval EFI_SUCCESS             The Results string is filled with the
                                  values corresponding to all requested
                                  names.

  @retval EFI_OUT_OF_RESOURCES    Not enough memory to store the
                                  parts of the results that must be
                                  stored awaiting possible future
                                  protocols.

  @retval EFI_INVALID_PARAMETER   For example, passing in a NULL
                                  for the Request parameter
                                  would result in this type of
                                  error. In this case, the
                                  Progress parameter would be
                                  set to NULL.

  @retval EFI_NOT_FOUND           Routing data doesn't match any
                                  known driver. Progress set to the
                                  first character in the routing header.
                                  Note: There is no requirement that the
                                  driver validate the routing data. It
                                  must skip the <ConfigHdr> in order to
                                  process the names.

  @retval EFI_INVALID_PARAMETER   Illegal syntax. Progress set
                                  to most recent "&" before the
                                  error or the beginning of the
                                  string.

  @retval EFI_INVALID_PARAMETER   Unknown name. Progress points
                                  to the & before the name in
                                  question.

**/
EFI_STATUS
EFIAPI
AmiIScsiFormExtractConfig (
    IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
    IN  CONST EFI_STRING                       Request,
    OUT EFI_STRING                             *Progress,
    OUT EFI_STRING                             *Results
);

/**
  Convert the iSCSI configuration data into the IFR data Which will be used
  to extract the iSCSI Keyword configuration in <ConfigAltResp> format.

  @param[in, out]  IfrNvData              The IFR nv data.

**/
VOID
EFIAPI
IScsiConvertAttemptConfigDataToIfrNvDataByKeyword (
    IN OUT ISCSI_CONFIG_IFR_NVDATA  *IfrNvData
);

/**
  Initialize the iSCSI configuration form.

  @param[in]  DriverBindingHandle The iSCSI driverbinding handle.

  @retval EFI_SUCCESS             The iSCSI configuration form is initialized.
  @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.

**/
EFI_STATUS
AmiIScsiConfigFormInit (
    IN EFI_HANDLE  DriverBindingHandle
);

/**

  This function applies changes in a driver's configuration.
  Input is a Configuration, which has the routing data for this
  driver followed by name / value configuration pairs. The driver
  must apply those pairs to its configurable storage. If the
  driver's configuration is stored in a linear block of data
  and the driver's name / value pairs are in <BlockConfig>
  format, it may use the ConfigToBlock helper function (above) to
  simplify the job.

  @param[in]  This           Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.

  @param[in]  Configuration  A null-terminated Unicode string in
                             <ConfigString> format.

  @param[out] Progress       A pointer to a string filled in with the
                             offset of the most recent '&' before the
                             first failing name / value pair (or the
                             beginning of the string if the failure
                             is in the first name / value pair) or
                             the terminating NULL if all was
                             successful.

  @retval EFI_SUCCESS             The results have been distributed or are
                                  awaiting distribution.

  @retval EFI_OUT_OF_RESOURCES    Not enough memory to store the
                                  parts of the results that must be
                                  stored awaiting possible future
                                  protocols.

  @retval EFI_INVALID_PARAMETERS  Passing in a NULL for the
                                  Results parameter would result
                                  in this type of error.

  @retval EFI_NOT_FOUND           Target for the specified routing data
                                  was not found.

**/
EFI_STATUS
EFIAPI
IScsiFormRouteConfig (
    IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
    IN  CONST EFI_STRING                       Configuration,
    OUT EFI_STRING                             *Progress
);

/**
  Convert IsId from binary to string format.

  @param[out]      String  The buffer to store the converted string.
  @param[in]       IsId    The buffer to store IsId.

  @retval EFI_SUCCESS              The string converted successfully.
  @retval EFI_INVALID_PARAMETER    Any input parameter is invalid.

**/
EFI_STATUS
IScsiConvertIsIdToString (
  OUT CHAR16                         *String,
  IN  UINT8                          *IsId
  );

/**
  Function to get Contoller Name corresponding to the NIC.

  @param[in]       MacAddress           MAC Address.
  @param[in, out]  ControllerName       The Controller name of the NIC.
  
  @retval EFI_SUCCESS            The operation is completed successfully.

**/  
EFI_STATUS 
GetControllerName (
  EFI_MAC_ADDRESS 					MacAddress, 
  CHAR16 							**ControllerName
  );  

/**
  This function determines whether the decrypted secret meets the required length.

  @param[in]    MacStringStr    MacString available in attempt data.

  @retval[out]  TRUE            If the MAC address matched.
  @retval[out]  FALSE           If the MAC address not matched.

**/
  
BOOLEAN CheckMacAvailability (
  CHAR16 MacStringStr[]
  );
  
#endif

