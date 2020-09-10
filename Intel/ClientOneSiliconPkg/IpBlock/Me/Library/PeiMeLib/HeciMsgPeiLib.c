/** @file
  Implementation file for Heci Message functionality

@copyright
  INTEL CONFIDENTIAL
  Copyright 2010 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#include "MePeiLibInternals.h"
#include <Library/ChipsetInitLib.h>
#include <Library/MeEwlLib.h>
#include <MeBiosPayloadData.h>
#include <MeState.h>
#include <Register/HeciRegs.h>
#include <IccMsgs.h>
#include "Library/MeTypeLib.h"
#include <Library/MeUtilsLib.h>

//
// ME Client - MKHI
//

//
// MKHI_CBM_GROUP_ID = 0x00
//

/**
  Send Core BIOS Reset Request Message through HECI to reset the system.

  @param[in] ResetOrigin         Reset source
  @param[in] ResetType           Global or Host reset

  @retval EFI_UNSUPPORTED        Current ME mode doesn't support this function
  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_NOT_FOUND          No ME present
  @retval EFI_DEVICE_ERROR       HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT            HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciSendCbmResetRequest (
  IN  UINT8                      ResetOrigin,
  IN  UINT8                      ResetType
  )
{
  EFI_STATUS                     Status;
  UINT32                         HeciLength;
  UINT32                         AckLength;
  CBM_RESET_REQ                  CbmResetRequest;
  UINT32                         MeMode;
  UINT32                         Result;
  VOID                           *Heci;
  CHAR8                          *ResetString;

  switch (ResetType) {
    case CBM_HRR_GLOBAL_RESET:
      ResetString = "Global";
      break;

    case CBM_HRR_HOST_RESET:
      ResetString = "Host";
      break;

    case CBM_HRR_CSME_RESET:
      ResetString = "CSME";
      break;

    default:
      return EFI_UNSUPPORTED;
  }

  DEBUG((DEBUG_INFO, "ME-BIOS: ME: %a Reset Request Entry.\n", ResetString));
  if (!MeTypeIsClient ()) {
    return EFI_UNSUPPORTED;
  }

  PostCode (0xE06);

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) ||
      ((MeMode != ME_MODE_NORMAL) && (MeMode != ME_MODE_TEMP_DISABLED) && (MeMode != ME_MODE_E_DEBUG))) {
    return EFI_UNSUPPORTED;
  }

  Heci = GetHeci (NULL);
  if (Heci == NULL) {
    return EFI_NOT_FOUND;
  }

  CbmResetRequest.MkhiHeader.Data           = 0;
  CbmResetRequest.MkhiHeader.Fields.GroupId = MKHI_CBM_GROUP_ID;
  CbmResetRequest.MkhiHeader.Fields.Command = CBM_RESET_CMD;
  CbmResetRequest.Data.RequestOrigin        = ResetOrigin;
  CbmResetRequest.Data.ResetType            = ResetType;
  HeciLength                                = sizeof (CBM_RESET_REQ);
  AckLength                                 = sizeof (CBM_RESET_ACK);

  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &CbmResetRequest,
             HeciLength,
             (UINT32 *) &CbmResetRequest,
             &AckLength,
             NULL,
             NULL
             );
  if (EFI_ERROR (Status)) {
    if (Status == EFI_NOT_FOUND) {
      DEBUG ((DEBUG_ERROR, "ME-BIOS: ME: %a Reset Request Exit - Error by HECI service not found.\n", ResetString));
      PostCode (0xEA6);
    } else if (Status == EFI_UNSUPPORTED) {
      DEBUG ((DEBUG_ERROR, "ME-BIOS: ME: %a Reset Request Exit - Error by message is not allowed.\n", ResetString));
      PostCode (0xE86);
    } else {
      DEBUG ((DEBUG_ERROR, "PeiHeciSendCbmResetRequest() - Unable to Send Reset Request - %r\n", Status));
      DEBUG ((DEBUG_ERROR, "ME-BIOS: ME: %a Reset Request Exit - Error by message sent fail.\n", ResetString));
      PostCode (0xEC6);
    }
    return Status;
  }

  Result = ((CBM_RESET_ACK *)&CbmResetRequest)->MkhiHeader.Fields.Result;
  if (Result != 0) {
    DEBUG ((DEBUG_ERROR, "ME-BIOS: ME: %a Reset Request Exit - Error by message ack error. Result: %x\n", ResetString, Result));
    Status = EFI_DEVICE_ERROR;
    PostCode (0xEE6);
  } else {
    DEBUG((DEBUG_INFO, "ME-BIOS: ME: %a Reset Request Exit - Success.\n", ResetString));
    PostCode (0xE26);
  }

  return Status;
}

//
// MKHI_FWCAPS_GROUP_ID = 0x03
//

/**
  Send Get Rule Data Request to CSME

  @param[in]   RuleId               Identifies the rule which data is requested.
  @param[out]  *RuleData            Pointer to requested rule data.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_INVALID_PARAMETER     Parameter invalid.
**/
EFI_STATUS
PeiHeciFwCapsGetRuleData (
  IN  UINT32                      RuleId,
  OUT UINT32                      *RuleData
  )
{
  EFI_STATUS                      Status;
  GET_RULE_BUFFER                 GetRuleMsg;
  UINT32                          Length;
  UINT32                          RecvLength;
  UINT32                          MeMode;
  VOID                            *Heci;

  if (RuleData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  Heci = GetHeci (NULL);
  if (Heci == NULL) {
    return EFI_UNSUPPORTED;
  }

  GetRuleMsg.Request.MkhiHeader.Data           = 0;
  GetRuleMsg.Request.MkhiHeader.Fields.GroupId = MKHI_FWCAPS_GROUP_ID;
  GetRuleMsg.Request.MkhiHeader.Fields.Command = FWCAPS_GET_RULE_CMD;
  GetRuleMsg.Request.RuleId                    = RuleId;
  Length                                       = sizeof (GET_RULE);
  RecvLength                                   = sizeof (GET_RULE_ACK);

  ///
  /// Send Get Rule Data Request to ME
  ///
  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetRuleMsg,
             Length,
             (UINT32 *) &GetRuleMsg,
             &RecvLength,
             NULL,
             NULL
             );

  if (!EFI_ERROR (Status) &&
      (GetRuleMsg.Response.MkhiHeader.Fields.Command == FWCAPS_GET_RULE_CMD) &&
      (GetRuleMsg.Response.MkhiHeader.Fields.IsResponse == 1) &&
      (GetRuleMsg.Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
    *RuleData = GetRuleMsg.Response.RuleData;
  }

  return Status;
}

/**
  Send Get Firmware SKU Request to ME

  @param[out] FwCapsSku             ME Firmware Capability SKU

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetFwCapsSkuMsg (
  OUT MEFWCAPS_SKU             *RuleData
  )
{
  return PeiHeciFwCapsGetRuleData (FWCAPS_RULE_ID (KernelFeatureId, FwCapsRuleTypeId), (UINT32 *) RuleData);
}

/**
  This message is sent by the BIOS or IntelR MEBX. One of usages is to utilize
  this command to determine if the platform runs in Consumer or Corporate SKU
  size firmware.

  @param[out] RuleData              Including PlatformBrand, IntelMeFwImageType,
                                    SuperSku, and PlatformTargetUsageType

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetPlatformTypeMsg (
  OUT PLATFORM_TYPE_RULE_DATA     *RuleData
  )
{
  return PeiHeciFwCapsGetRuleData (FWCAPS_RULE_ID (KernelFeatureId, PlatTypeRuleTypeId), (UINT32 *) RuleData);
}

/**
  The Get FW Feature Status message is based on MKHI interface.
  This command is used by BIOS/IntelR MEBX to get firmware runtime status.
  The GET FW RUNTIME STATUS message doesn't need to check the HFS.FWInitComplete
  value before sending the command.
  It means this message can be sent regardless of HFS.FWInitComplete.

  @param[out] RuleData              ME Firmware Capability SKU

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetFwFeatureStateMsg (
  OUT MEFWCAPS_SKU                *RuleData
  )
{
  return PeiHeciFwCapsGetRuleData (FWCAPS_RULE_ID (KernelFeatureId, FeatureStateRuleTypeId), (UINT32 *) RuleData);
}

/**
  This message is sent by the BIOS in PEI phase to query
  ME Unconfig on RTC Clear Disable state.

  @param[out] RuleData            1 - Unconfig on RTC clear is disabled
                                  0 - Unconfig on RTC clear is enabled

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetUnconfigOnRtcClearDisableMsg (
  OUT UINT32                *RuleData
  )
{
  return PeiHeciFwCapsGetRuleData (FWCAPS_RULE_ID (KernelFeatureId, UnconfigOnRtcClarRuleTypeId), RuleData);
}

/**
  This message is sent by the BIOS on the boot where the host wants to get the firmware provisioning state.
  The firmware will respond to AMT BIOS SYNC INFO message even after the End of Post.

  @param[out] RuleData            @see AMT_BIOS_SYNC_INFO

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetAmtBiosSyncInfo (
  OUT AMT_BIOS_SYNC_INFO *RuleData
  )
{
  return PeiHeciFwCapsGetRuleData (FWCAPS_RULE_ID (AmtFeatureId, AmtBiosSyncInfoRuleTypeId), (UINT32 *) RuleData);
}

/**
  Send Set Rule Data Request to CSME

  @param[in]   RuleId             Identifies the rule which data needs to be changed.
  @param[in]   RuleDataLength     Rule Data Length.
  @param[in]   RuleData           Pointer to new rule data.

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciFwCapsSetRuleData (
  IN  UINT32                      RuleId,
  IN  UINT8                       RuleDataLength,
  IN  UINT32                      RuleData
  )
{
  EFI_STATUS                      Status;
  UINT32                          Length;
  UINT32                          RecvLength;
  SET_RULE_BUFFER                 SetRuleMsg;
  UINT32                          MeMode;
  VOID                            *Heci;

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  Heci = GetHeci (NULL);
  if (Heci == NULL) {
    return EFI_UNSUPPORTED;
  }

  SetRuleMsg.Request.MkhiHeader.Data           = 0;
  SetRuleMsg.Request.MkhiHeader.Fields.GroupId = MKHI_FWCAPS_GROUP_ID;
  SetRuleMsg.Request.MkhiHeader.Fields.Command = FWCAPS_SET_RULE_CMD;
  SetRuleMsg.Request.RuleId                    = RuleId;
  SetRuleMsg.Request.RuleDataLen               = RuleDataLength;
  SetRuleMsg.Request.RuleData                  = RuleData;
  Length                                       = sizeof (SET_RULE);
  RecvLength                                   = sizeof (SET_RULE_ACK);

  ///
  /// Send Set Rule Data Request to ME
  ///
  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &SetRuleMsg,
             Length,
             (UINT32 *) &SetRuleMsg,
             &RecvLength,
             NULL,
             NULL
             );

  return Status;
}

/**
  This message is sent by the BIOS in PEI phase to set
  ME Unconfig on RTC Clear Disable state.

  @param[in] RuleData             1 - Disable Unconfig on RTC clear
                                  0 - Enable Unconfig on RTC clear

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function or EOP was sent
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciSetUnconfigOnRtcClearDisableMsg (
  IN UINT32                RuleData
  )
{
  return PeiHeciFwCapsSetRuleData (FWCAPS_RULE_ID (KernelFeatureId, UnconfigOnRtcClarRuleTypeId), RULE_DATA_LENGTH, RuleData);
}

//
// MKHI_MCA_GROUP_ID = 0x0A
//

/**
  Sends the NVM request device ownership message.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciRequestDeviceOwnership (
  VOID
  )
{
  EFI_STATUS                     Status;
  REQUEST_OWNERSHIP_BUFFER       RequestOwnershipMsg;
  UINT32                         Length;
  UINT32                         RecvLength;
  VOID                            *Heci;

  Heci = GetHeci (NULL);
  if (Heci == NULL) {
    return EFI_UNSUPPORTED;
  }

  RequestOwnershipMsg.Request.MkhiHeader.Data           = 0;
  RequestOwnershipMsg.Request.MkhiHeader.Fields.GroupId = MKHI_MCA_GROUP_ID;
  RequestOwnershipMsg.Request.MkhiHeader.Fields.Command = MCA_REQUEST_DEVICE_OWNERSHIP;
  Length                                                = sizeof (REQUEST_OWNERSHIP);
  RecvLength                                            = sizeof (REQUEST_OWNERSHIP);

  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &RequestOwnershipMsg,
             Length,
             (UINT32 *) &RequestOwnershipMsg,
             &RecvLength,
             NULL,
             NULL
             );

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Heci Request Device Ownership Msg Fail, Status = %r\n", Status));
    return Status;
  }

  if (RequestOwnershipMsg.Response.MkhiHeader.Fields.IsResponse == 0) {
    DEBUG ((DEBUG_ERROR, "Heci Request Device Ownership unsuccessful - no response\n"));
    return EFI_DEVICE_ERROR;
  }

  if (RequestOwnershipMsg.Response.MkhiHeader.Fields.Result != MkhiStatusSuccess) {
    DEBUG ((DEBUG_WARN, "Heci Request Device Ownership processed with MKHI Error Code. Response: \n"));
    DEBUG ((DEBUG_WARN, "  MkhiResult = 0x%02x\n", RequestOwnershipMsg.Response.MkhiHeader.Fields.Result));
    DEBUG ((DEBUG_WARN, "  Group      = %x\n", RequestOwnershipMsg.Response.MkhiHeader.Fields.GroupId));
    DEBUG ((DEBUG_WARN, "  Command    = %x\n", RequestOwnershipMsg.Response.MkhiHeader.Fields.Command));
    DEBUG ((DEBUG_WARN, "  IsResponse = %x\n", RequestOwnershipMsg.Response.MkhiHeader.Fields.IsResponse));
    return EFI_DEVICE_ERROR;
  } else {
    DEBUG ((DEBUG_INFO, "Heci Request Device Ownership successful.\n"));
  }

  return EFI_SUCCESS;
}

//
// MKHI_DNX_GROUP_ID = 0x0D
//

/**
  Send DNX Request Set message.

  @param[out] BiosAction          Requested Bios Action:
                                    0 - Continue Post
                                    1 - Reset Required

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_INVALID_PARAMETER   Invalid input parameter
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciDnxReqSet (
  OUT UINT32         *BiosAction
  )
{
  EFI_STATUS         Status;
  UINT32             MeMode;
  DNX_REQ_SET_BUFFER DnxReqSet;
  UINT32             Length;
  UINT32             RecvLength;
  VOID               *Heci;

  if (BiosAction == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  Heci = GetHeci (NULL);
  if (Heci == NULL) {
    return EFI_UNSUPPORTED;
  }

  DnxReqSet.Request.MkhiHeader.Data           = 0;
  DnxReqSet.Request.MkhiHeader.Fields.GroupId = MKHI_DNX_GROUP_ID;
  DnxReqSet.Request.MkhiHeader.Fields.Command = DNX_REQ_SET_CMD;
  Length                                      = sizeof (DNX_REQ_SET);
  RecvLength                                  = sizeof (DNX_REQ_SET_ACK);

  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &DnxReqSet,
             Length,
             (UINT32 *) &DnxReqSet,
             &RecvLength,
             NULL,
             NULL
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a() failed - %r\n", __FUNCTION__, Status));
  }

  if (!EFI_ERROR (Status) &&
      (DnxReqSet.Response.MkhiHeader.Fields.Command == DNX_REQ_SET_CMD) &&
      (DnxReqSet.Response.MkhiHeader.Fields.IsResponse == 1) &&
      (DnxReqSet.Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
    *BiosAction = DnxReqSet.Response.BiosAction;
  }

  return Status;
}


//
// MKHI_GEN_GROUP_ID = 0xFF
//

/**
  Get ME Unconfiguration Status.

  @param[out] UnconfigStatus      Unconfiguration Status:
                                    0x01 - Unconfiguration in progress
                                    0x02 - No unconfigure w/o password to be processed
                                    0x03 - CSME finished processing unconfigure w/o password
                                    0x80 - Error encountered by CSME during unconfigure w/o password

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetUnconfigureStatus (
  OUT UINT8* UnconfigStatus
  )
{
  GEN_GET_ME_UNCFG_STATE UnconfigureStatus;
  UINT32                 Length;
  EFI_STATUS             Status;
  UINT32                 MeMode;
  VOID                   *Heci;

  Heci = GetHeci (NULL);
  if (Heci == NULL) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    *UnconfigStatus = ME_UNCONFIG_NOT_IN_PROGRESS;
    return EFI_UNSUPPORTED;
  }

  UnconfigureStatus.MkhiHeader.Data           = 0;
  UnconfigureStatus.MkhiHeader.Fields.GroupId = MKHI_GEN_GROUP_ID;
  UnconfigureStatus.MkhiHeader.Fields.Command = GEN_GET_ME_UNCFG_STATE_CMD;
  Length                                      = sizeof (GEN_GET_ME_UNCFG_STATE);

  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &UnconfigureStatus,
             Length,
             (UINT32 *) &UnconfigureStatus,
             &Length,
             NULL,
             NULL
             );

  if (!EFI_ERROR (Status) &&
      (UnconfigureStatus.MkhiHeader.Fields.Command == GEN_GET_ME_UNCFG_STATE_CMD) &&
      (UnconfigureStatus.MkhiHeader.Fields.IsResponse == 1)) {
    *UnconfigStatus = (UINT8) UnconfigureStatus.MkhiHeader.Fields.Result;
  }

  return Status;
}


//
// BUP_COMMON_GROUP_ID = 0xF0
//

/**
  Send DRAM init done message through HECI to inform ME of memory initialization done.

  @param[in]  ImrBaseLow                Base address for IMR region (Low DWORD)
  @param[in]  ImrBaseHigh               Base address for IMR region (High DWORD)
  @param[in]  MemStatus                 Memory init status
  @param[out] MkhiResult                MKHI Error Code
  @param[out] Flags                     Flags
  @param[out] BiosAction                ME response to DID
  @param[out] PciImrBaseLow             Base address for PCI IMR (Low DWORD)
  @param[out] PciImrBaseHig             Base address for PCI IMR (High DWORD)
  @retval EFI_SUCCESS                   Command succeeded
  @retval EFI_DEVICE_ERROR              HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                   HECI does not return the buffer before timeout
  @retval EFI_UNSUPPORTED               Message is not supported for current ME
**/
EFI_STATUS
PeiHeciSendDid (
  IN  UINT32  ImrBaseLow,
  IN  UINT32  ImrBaseHigh,
  IN  UINT8   MemStatus,
  OUT UINT8   *MkhiResult,
  OUT UINT8   *Flags,
  OUT UINT8   *BiosAction,
  OUT UINT32  *PciImrBaseLow,
  OUT UINT32  *PciImrBaseHigh
  )
{
  EFI_STATUS                 Status;
  DRAM_INIT_DONE_CMD_BUFFER  DidBuffer;
  UINT32                     ReqLength;
  UINT32                     RespLength;
  VOID                       *Heci;

  if (MeTypeIsDisabled ()) {
    return EFI_UNSUPPORTED;
  }

  Heci = GetHeci (NULL);
  if (Heci == NULL) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&DidBuffer, sizeof(DRAM_INIT_DONE_CMD_BUFFER));
  DidBuffer.Request.MkhiHeader.Data           = 0;
  DidBuffer.Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  DidBuffer.Request.MkhiHeader.Fields.Command = DRAM_INIT_DONE_CMD;
  DidBuffer.Request.ImrData.BiosImrsBaLow     = ImrBaseLow;
  DidBuffer.Request.ImrData.BiosImrsBaHigh    = ImrBaseHigh;
  DidBuffer.Request.MemStatus                 = MemStatus;
  ReqLength                                   = sizeof (DRAM_INIT_DONE_CMD_REQ);
  RespLength                                  = sizeof (DRAM_INIT_DONE_CMD_RESP);

  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &DidBuffer.Request,
             ReqLength,
             (UINT32 *) &DidBuffer.Response,
             &RespLength,
             NULL,
             NULL
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((DidBuffer.Response.MkhiHeader.Fields.GroupId != BUP_COMMON_GROUP_ID) ||
      (DidBuffer.Response.MkhiHeader.Fields.Command != DRAM_INIT_DONE_CMD) ||
      (DidBuffer.Response.MkhiHeader.Fields.IsResponse == 0) ||
      (DidBuffer.Response.MkhiHeader.Fields.Result != MkhiStatusSuccess)) {
    DEBUG ((DEBUG_ERROR, "HeciSend DRAM Init Done unsuccessful - no response\n"));
    return EFI_DEVICE_ERROR;
  }

  *MkhiResult     = (UINT8)DidBuffer.Response.MkhiHeader.Fields.Result;
  *Flags          = DidBuffer.Response.Flags;
  *BiosAction     = DidBuffer.Response.BiosAction;
  *PciImrBaseLow  = DidBuffer.Response.Pci2PrivBase.AdrLow;
  *PciImrBaseHigh = DidBuffer.Response.Pci2PrivBase.AdrHigh;

  if (*MkhiResult != MkhiStatusSuccess) {
    DEBUG ((DEBUG_WARN, "HeciSend DRAM Init Done processed with MKHI Error Code. Response: \n"));
    DEBUG ((DEBUG_WARN, "  MkhiResult = 0x%02x\n",  *MkhiResult));
  } else {
    DEBUG ((DEBUG_INFO, "HeciSend DRAM Init Done successful. Response:\n"));
  }
  DEBUG ((DEBUG_INFO, "  Pci2Priv Base Low = 0x%08x\n",  DidBuffer.Response.Pci2PrivBase.AdrLow));
  DEBUG ((DEBUG_INFO, "  Pci2Priv Base High = 0x%08x\n", DidBuffer.Response.Pci2PrivBase.AdrHigh));
  DEBUG ((DEBUG_INFO, "  Flags = 0x%02x\n",              DidBuffer.Response.Flags));
  DEBUG ((DEBUG_INFO, "  BIOS Action = 0x%02x\n",        DidBuffer.Response.BiosAction));

  return Status;
}

/**
  Send Get MBP from FW

  @param[out]     MbpEntriesNumbe   Number of MBP entries
  @param[in, out] MbpItems          MBP items of the response
  @paran[in]      SkipMbp           Skip MBP

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetMbpMsg (
  OUT UINT32                   *MbpEntriesNumber,
  IN OUT UINT32                *MbpItems,
  IN BOOLEAN                   SkipMbp
  )
{
  EFI_STATUS     Status;
  GET_MBP_BUFFER GetMbp;
  UINT32         Length;
  UINT32         RecvLength;
  VOID           *Heci;

  Heci = GetHeci (NULL);
  if (Heci == NULL) {
    return EFI_UNSUPPORTED;
  }

  GetMbp.Request.MkhiHeader.Data           = 0;
  GetMbp.Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  GetMbp.Request.MkhiHeader.Fields.Command = MBP_REQUEST_CMD;
  GetMbp.Request.SkipMbp                   = SkipMbp;
  Length                                   = sizeof (GET_MBP_REQ);
  RecvLength                               = sizeof (GET_MBP_RESP);

  ///
  /// Send Get MBP Request to ME
  ///
  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetMbp.Request,
             Length,
             (UINT32 *) &GetMbp.Response,
             &RecvLength,
             NULL,
             NULL
             );

  DEBUG ((DEBUG_INFO, "ReadMsg returned %r\n", Status));
  DEBUG ((DEBUG_INFO, "MsgGetMbpAck->MkhiHeader.Fields.Command = %d\n", GetMbp.Response.MkhiHeader.Fields.Command));
  DEBUG ((DEBUG_INFO, "MsgGetMbpAck->MkhiHeader.Fields.IsResponse = %d\n", GetMbp.Response.MkhiHeader.Fields.IsResponse));
  DEBUG ((DEBUG_INFO, "MsgGetMbpAck->MkhiHeader.Fields.Result = %d\n", GetMbp.Response.MkhiHeader.Fields.Result));

  if (!EFI_ERROR (Status) &&
      !SkipMbp &&
      (GetMbp.Response.MkhiHeader.Fields.Command == MBP_REQUEST_CMD) &&
      (GetMbp.Response.MkhiHeader.Fields.IsResponse == 1) &&
      (GetMbp.Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
    *MbpEntriesNumber = GetMbp.Response.MbpHeader.NumEntries;
    CopyMem (MbpItems, &GetMbp.Response.MbpItems, (GetMbp.Response.MbpHeader.MbpSize * sizeof(UINT32) - sizeof (MBP_HEADER)));
  }

  DEBUG ((DEBUG_INFO, "No. of Mbp Entries: %d\n", GetMbp.Response.MbpHeader.NumEntries));

  //
  // Clear buffers from sensitive data
  //
  ZeroMem (&GetMbp.Response, sizeof (GET_MBP_RESP));
  return Status;
}

/**
  This message is sent by the BIOS to retrieve from CSME total size of IMRs.
  BIOS needs to provide mask of disabled IMRs and requested size for PCIe IMR.

  @param[in]  BiosImrDisableMask0  Low DWORD of BIOS IMR Disable mask
  @param[in]  BiosImrDisableMask1  High DWORD of BIOS IMR Disable mask
  @param[in]  PciImrSize           Requested IMR size for PCI
  @param[out] MkhiResult           MKHI Error Code
  @param[out] ImrsSize             Total IMR size
  @param[out] Alignment            Required address alignment

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciGetImrSizeMsg (
  IN  UINT32     BiosImrDisableMask0,
  IN  UINT32     BiosImrDisableMask1,
  IN  UINT32     PciImrSize,
  OUT UINT8      *MkhiResult,
  OUT UINT32     *ImrsSize,
  OUT UINT32     *Alignment
  )
{
  EFI_STATUS              Status;
  GET_IMR_CMD_BUFFER      GetImrCmd;
  UINT32                  ReqLength;
  UINT32                  RespLength;
  VOID                    *Heci;

  DEBUG ((DEBUG_INFO, "Heci Get IMR Size Msg\n"));

  Heci = GetHeci (NULL);
  if (Heci == NULL) {
    return EFI_UNSUPPORTED;
  }

  *ImrsSize  = 0;
  *Alignment = 0;

  GetImrCmd.Request.MkhiHeader.Data           = 0;
  GetImrCmd.Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  GetImrCmd.Request.MkhiHeader.Fields.Command = GET_IMR_SIZE_CMD;
  GetImrCmd.Request.Data.BiosImrDisableMask0  = BiosImrDisableMask0;
  GetImrCmd.Request.Data.BiosImrDisableMask1  = BiosImrDisableMask1;
  GetImrCmd.Request.Data.Pci2PrivSize         = (UINT32) PciImrSize;
  ReqLength                                   = sizeof (GET_IMR_CMD_REQ);
  RespLength                                  = sizeof (GET_IMR_CMD_RESP);

  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &GetImrCmd.Request,
             ReqLength,
             (UINT32 *) &GetImrCmd.Response,
             &RespLength,
             NULL,
             NULL
             );

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Heci Get IMR Size Msg Fail, Status = %r\n", Status));
    return Status;
  }

  if ((GetImrCmd.Response.MkhiHeader.Fields.GroupId != BUP_COMMON_GROUP_ID) ||
      (GetImrCmd.Response.MkhiHeader.Fields.Command != GET_IMR_SIZE_CMD) ||
      (GetImrCmd.Response.MkhiHeader.Fields.IsResponse == 0) ||
      (GetImrCmd.Response.MkhiHeader.Fields.Result != MkhiStatusSuccess)) {
    DEBUG ((DEBUG_ERROR, "Heci Get IMR Size Msg Fail - no response\n"));
    return EFI_DEVICE_ERROR;
  }

  *MkhiResult = (UINT8)GetImrCmd.Response.MkhiHeader.Fields.Result;
  *ImrsSize   = GetImrCmd.Response.Data.ImrsSize;
  *Alignment  = GetImrCmd.Response.Data.Alignment;

  if (*MkhiResult != MkhiStatusSuccess) {
    DEBUG ((DEBUG_WARN, "Heci Get IMR Size processed with MKHI Error Code. Response: \n"));
    DEBUG ((DEBUG_WARN, "  MkhiResult = 0x%02x\n", *MkhiResult));
  } else {
    DEBUG ((DEBUG_INFO, "Heci Get IMR Size Msg successful. Response:\n"));
  }

  DEBUG ((DEBUG_INFO, "  IMR Size = 0x%08x\n",          *ImrsSize));
  DEBUG ((DEBUG_INFO, "  Largest IMR Size = 0x%08x\n",  *Alignment));
  DEBUG ((DEBUG_INFO, "  Flags = 0x%08x\n",             GetImrCmd.Response.Data.Flags));

  PostCodeWithDescription (*ImrsSize, "Heci Get IMR Size Msg");

  return Status;
}
/**
  Send Manufacturing Reset and Halt

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciBupManufacturingResetAndHalt (
  VOID
  )
{
  EFI_STATUS                  Status;
  MANUF_RESET_AND_HALT_BUFFER ManufResetAndHalt;
  UINT32                      Length;
  UINT32                      RecvLength;
  VOID                        *Heci;

  Heci = GetHeci (NULL);
  if (Heci == NULL) {
    return EFI_UNSUPPORTED;
  }

  ManufResetAndHalt.Request.MkhiHeader.Data           = 0;
  ManufResetAndHalt.Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  ManufResetAndHalt.Request.MkhiHeader.Fields.Command = MANUF_RESET_AND_HALT_CMD;
  Length                                              = sizeof (MANUF_RESET_AND_HALT_REQ);
  RecvLength                                          = sizeof (MANUF_RESET_AND_HALT_RESP);

  ///
  /// Send Manufacturing Reset and Halt Request to ME
  ///
  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &ManufResetAndHalt.Request,
             Length,
             (UINT32 *) &ManufResetAndHalt.Response,
             &RecvLength,
             NULL,
             NULL
             );

  if (ManufResetAndHalt.Response.MkhiHeader.Fields.Result != MkhiStatusSuccess) {
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Send Set eDebug Mode State.

  @param[in]  State                 EDebug Mode state to be set:
                                      TRUE  - Enable  eDebug State
                                      FALSE - Disable eDebug State

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_NOT_FOUND             Couldn't locate HeciPpi.
**/
EFI_STATUS
PeiHeciSetEDebugModeState (
  IN BOOLEAN                   State
  )
{
  EFI_STATUS                   Status;
  SET_EDEBUG_MODE_STATE_BUFFER SetEDebugState;
  UINT32                       Length;
  UINT32                       RecvLength;
  HECI_PPI                     *HeciPpi;

  Status = PeiServicesLocatePpi (
             &gHeciPpiGuid,
             0,
             NULL,
             (VOID **) &HeciPpi
             );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  SetEDebugState.Request.MkhiHeader.Data           = 0;
  SetEDebugState.Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  SetEDebugState.Request.MkhiHeader.Fields.Command = SET_EDEBUG_STATE_CMD;
  SetEDebugState.Request.State                     = State;
  Length                                           = sizeof (SET_EDEBUG_MODE_STATE_REQ);
  RecvLength                                       = sizeof (SET_EDEBUG_MODE_STATE_RESP);

  Status = HeciPpi->SendwAck (
                      HECI1_DEVICE,
                      (UINT32 *) &SetEDebugState,
                      Length,
                      &RecvLength,
                      BIOS_FIXED_HOST_ADDR,
                      HECI_MKHI_MESSAGE_ADDR
                      );

  if (!EFI_ERROR (Status) &&
     ((SetEDebugState.Response.MkhiHeader.Fields.Command != SET_EDEBUG_STATE_CMD) ||
      (SetEDebugState.Response.MkhiHeader.Fields.IsResponse != 1) ||
      (SetEDebugState.Response.MkhiHeader.Fields.Result != MkhiStatusSuccess))) {
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  This command is sent by the BIOS to switch clear CSME data to image creation state.
  After the command success, platform reset is expected.

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_UNSUPPORTED         Get ME mode fail.
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout or transition failed
                                  within allowed time limit
**/
EFI_STATUS
PeiHeciDataClear (
  VOID
  )
{
  EFI_STATUS         Status;
  DATA_CLEAR_BUFFER  DataClear;
  UINT32             Length;
  UINT32             RespLength;
  VOID               *Heci;
  UINT32             MeMode;

  Heci = GetHeci (NULL);
  if (Heci == NULL) {
    return EFI_NOT_FOUND;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status)) {
    //
    // CSME should support this command in all modes.
    //
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&DataClear, sizeof (DATA_CLEAR_BUFFER));
  DataClear.Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  DataClear.Request.MkhiHeader.Fields.Command = DATA_CLEAR_CMD;
  Length                                      = sizeof (DATA_CLEAR_REQ);
  RespLength                                  = sizeof (DATA_CLEAR_RESP);

  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &DataClear,
             Length,
             (UINT32 *) &DataClear,
             &RespLength,
             NULL,
             NULL
             );
  if (!EFI_ERROR (Status) &&
      ((DataClear.Response.MkhiHeader.Fields.Command != DATA_CLEAR_CMD) ||
      (DataClear.Response.MkhiHeader.Fields.IsResponse != 1) ||
      (DataClear.Response.MkhiHeader.Fields.Result != MkhiStatusSuccess))) {
    Status = EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "%a() exit, Status = %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  This message is sent by the BIOS to tell CSME that Buffer has been allocated for payload to be copied.

  @param[in]      LoadRequest            IP information for which firmware has to be loaded
  @param[in]      BufferAddressLow       Lower DRAM address of the buffer
  @param[in]      BufferAddressHigh      Upper DRAM address of the buffer
  @param[in, out] BufferSize             On input, BufferSize is size of Buffer allocated by Bios.On output, actual PayloadSize copied by ME
  @param[out]     IpLoaderStatus         IP Loading status from CSME
  @param[out]     HashType               Hash algorithm used for message digest
  @param[out]     Hash                   Hash Value

  @retval EFI_SUCCESS                    Command succeeded
  @retval EFI_DEVICE_ERROR               HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                    HECI does not return the buffer before timeout
  @retval EFI_UNSUPPORTED                Current ME mode doesn't support sending this message through this HECI or Input parameters are not valid.
  @retval EFI_BUFFER_TOO_SMALL           Message Buffer is too small for the Acknowledge
  @retval EFI_INVALID_PARAMETER          Parameter invalid
**/
EFI_STATUS
PeiHeciGetFwPayload (
  IN     UINT32        LoadRequest,
  IN     UINT32        BufferAddressLow,
  IN     UINT32        BufferAddressHigh,
  IN OUT UINT32        *BufferSize,
  OUT    UINT32        *IpLoaderStatus,
  OUT    UINT8         *HashType,
  OUT    UINT8         *Hash
  )
{
  EFI_STATUS                 Status;
  IP_LOADING_CMD_BUFFER      *IpLoad;
  UINT32                     ReqLength;
  UINT32                     RespLength;
  UINT32                     MeMode;
  VOID                       *Heci;

  DEBUG ((DEBUG_INFO, "%a () enter\n", __FUNCTION__));

  if (((BufferAddressLow == 0) && (BufferAddressHigh == 0)) ||
      (BufferSize == NULL) ||
      (IpLoaderStatus == NULL) ||
      (HashType == NULL) ||
      (Hash == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) ||
    (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  Heci = GetHeci (NULL);
  if (Heci == NULL) {
    return EFI_UNSUPPORTED;
  }

  ReqLength = sizeof (IP_LOADING_CMD_REQ);
  //(3 * sizeof (UINT8)) is needed for DWORD alignment
  RespLength = sizeof (IP_LOADING_CMD_RESP) + MAX_DIGEST_SIZE + (3 * sizeof (UINT8));
  IpLoad = AllocateZeroPool (RespLength);
  if (IpLoad == NULL) {
    DEBUG ((DEBUG_ERROR, "%a(): Could not allocate Memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  IpLoad->Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
  IpLoad->Request.MkhiHeader.Fields.Command = GET_IP_FIRMWARE_CMD;
  IpLoad->Request.Version                   = IP_LOADING_CMD_VERSION;
  IpLoad->Request.Operation                 = LoadRequest;
  IpLoad->Request.DramBaseLow               = (UINT32) BufferAddressLow;
  IpLoad->Request.DramBaseHigh              = (UINT32) BufferAddressHigh;
  IpLoad->Request.MemorySize                = (UINT32) BufferSize;

  DEBUG ((DEBUG_INFO, "BufferAddressHigh %x, BufferAddressLow %x\n", BufferAddressHigh, BufferAddressLow));
  ///
  /// Send Ip Load Request to ME
  ///
  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) IpLoad,
             ReqLength,
             (UINT32 *) IpLoad,
             &RespLength,
             NULL,
             NULL
             );
  if (EFI_ERROR (Status)) {
    FreePool (IpLoad);
    return Status;
  }

  if ((IpLoad->Response.MkhiHeader.Fields.GroupId != BUP_COMMON_GROUP_ID) ||
      (IpLoad->Response.MkhiHeader.Fields.Command != GET_IP_FIRMWARE_CMD) ||
      (IpLoad->Response.MkhiHeader.Fields.IsResponse == 0) ||
      (IpLoad->Response.MkhiHeader.Fields.Result != MkhiStatusSuccess)) {
    DEBUG ((DEBUG_ERROR, "Heci Get IP Load Msg Failed\n"));
    FreePool (IpLoad);
    return EFI_DEVICE_ERROR;
  }

  *BufferSize     = IpLoad->Response.PayloadSize;
  *IpLoaderStatus = IpLoad->Response.Status;
  *HashType       = IpLoad->Response.HashType;

  DEBUG ((DEBUG_INFO, "Heci Ip Load Request Msg successful. Response:\n"));
  DEBUG ((DEBUG_INFO, "  PayloadSize = 0x%08x\n", *BufferSize));
  DEBUG ((DEBUG_INFO, "  Hash Type used for Signing Payload = 0x%08x\n", IpLoad->Response.HashType));
  CopyMem (Hash, IpLoad->Response.Hash, MAX_DIGEST_SIZE);

  FreePool (IpLoad);
  DEBUG ((DEBUG_INFO, "%a () exit\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  This message is sent by the BIOS to retrieve debut token data from CSME.

  @param[out] KnobData              Debug Token Knob Data

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_NOT_FOUND             Couldn't locate HeciPpi or debug token data
  @retval EFI_INVALID_PARAMETER     Parameter invalid
**/
EFI_STATUS
PeiHeciGetDebugTokenData (
  OUT DEBUG_TOKEN_KNOB_DATA    *KnobData
  )
{
//  EFI_STATUS                   Status;
//  GET_DEBUG_TOKEN_DATA_BUFFER  GetDebugTokenData;
//  UINT32                       Length;
//  UINT32                       RespLength;
//  VOID                         *Heci;
//  UINT32                       MeMode;
//
//  if (KnobData == NULL) {
//    return EFI_INVALID_PARAMETER;
//  }
//
//  Heci = GetHeci (NULL);
//  if (Heci == NULL) {
//    return EFI_NOT_FOUND;
//  }
//
//  Status = MeBiosGetMeMode (&MeMode);
//  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
//    return EFI_UNSUPPORTED;
//  }
//
//  ZeroMem (&GetDebugTokenData, sizeof (GET_DEBUG_TOKEN_DATA_BUFFER));
//  GetDebugTokenData.Request.MkhiHeader.Fields.GroupId = BUP_COMMON_GROUP_ID;
//  GetDebugTokenData.Request.MkhiHeader.Fields.Command = GET_DEBUG_TOKEN_DATA_CMD;
//  Length                                              = sizeof (GET_DEBUG_TOKEN_DATA_REQ);
//  RespLength                                          = sizeof (GET_DEBUG_TOKEN_DATA_RESP);
//
//  Status = HeciSendWithAck (
//             Heci,
//             HECI_DEFAULT_PCH_ID,
//             NULL,
//             HECI_DEFAULT_RETRY_NUMBER,
//             BIOS_FIXED_HOST_ADDR,
//             HECI_MKHI_MESSAGE_ADDR,
//             (UINT32 *) &GetDebugTokenData,
//             Length,
//             (UINT32 *) &GetDebugTokenData,
//             &RespLength,
//             NULL,
//             NULL
//             );
//  if (EFI_ERROR (Status)) {
//    return Status;
//  }
//
//  if ((GetDebugTokenData.Response.MkhiHeader.Fields.Command == GET_DEBUG_TOKEN_DATA_CMD) &&
//      (GetDebugTokenData.Response.MkhiHeader.Fields.IsResponse == 1) &&
//      (GetDebugTokenData.Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
//    CopyMem (KnobData, &GetDebugTokenData.Response.KnobData, sizeof (DEBUG_TOKEN_KNOB_DATA));
//  } else {
//    Status = EFI_DEVICE_ERROR;
//  }
//
//  return Status;
  return EFI_UNSUPPORTED;
}


//
// BUP_ICC_GROUP_ID = 0xF1
//

/**
  Send ICC request through HECI to query if CSME FW requires the warm reset flow from a previous boot.

  @param[out] WarmResetRequired   1 - CSME requires a warm reset to complete BCLK ramp en flow

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciIccBclkMsg (
  OUT UINT8    *WarmResetRequired
  )
{
  EFI_STATUS        Status;
  ICC_CMD_BUFFER    IccCmdBuffer;
  UINT32            Length;
  UINT32            RespLength;
  VOID              *Heci;

  DEBUG ((DEBUG_INFO, "(ICC) PeiHeciIccBclkMsg\n"));

  Heci = GetHeci (NULL);
  if (Heci == NULL) {
    return EFI_UNSUPPORTED;
  }

  IccCmdBuffer.Response.Data.FwNeedsWarmResetFlag = 0; //@Todo: remove this after CSME fixes response structure issue.
  IccCmdBuffer.Request.MkhiHeader.Data            = 0;
  IccCmdBuffer.Request.MkhiHeader.Fields.GroupId  = BUP_ICC_GROUP_ID;
  IccCmdBuffer.Request.MkhiHeader.Fields.Command  = ICC_CMD;
  Length                                          = sizeof (ICC_CMD_REQ);
  RespLength                                      = sizeof (ICC_CMD_RESP);

  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &IccCmdBuffer.Request,
             Length,
             (UINT32 *) &IccCmdBuffer.Response,
             &RespLength,
             NULL,
             NULL
             );

  if (EFI_ERROR(Status) ||
      (IccCmdBuffer.Response.MkhiHeader.Fields.GroupId != BUP_ICC_GROUP_ID) ||
      (IccCmdBuffer.Response.MkhiHeader.Fields.Command != ICC_CMD) ||
      (IccCmdBuffer.Response.MkhiHeader.Fields.IsResponse != 1) ||
      (IccCmdBuffer.Response.MkhiHeader.Fields.Result != MkhiStatusSuccess)) {
    Status = EFI_DEVICE_ERROR;
  } else {
    *WarmResetRequired = (UINT8) IccCmdBuffer.Response.Data.FwNeedsWarmResetFlag;
  }

  return Status;
}


//
// BUP_MPHY_GROUP_ID = 0xF2
//

/**
  Get the Mphy data from CSME

  @param[out] Data                Pointer to the data buffer.
  @param[in]  DataSize            Pointer to the length of the table in bytes

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES    HECI Could not allocate Memory
  @retval EFI_NOT_FOUND           Couldn't locate HeciPpi.
**/
EFI_STATUS
PeiHeciReadFromMphy (
  OUT UINT8  *Data,
  IN  UINT32 DataSize
  )
{
  EFI_STATUS                     Status;
  UINT32                         Length;
  UINT32                         RecvLength;
  BUP_MPHY_READ_FROM_MPHY        *HsioReadMphyReq;
  BUP_MPHY_READ_FROM_MPHY_ACK    *HsioReadMphyRsp;
  VOID                           *Heci;
  EFI_STATUS                     SendStatus;
  EFI_STATUS                     ReceiveStatus;
  HECI_API_VERSION               HeciApiVersion;

  if ((Data == NULL) || (DataSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "%a(): Start\n", __FUNCTION__));

  Length     = sizeof (BUP_MPHY_READ_FROM_MPHY);
  RecvLength = sizeof (BUP_MPHY_READ_FROM_MPHY_ACK) + DataSize;

  HsioReadMphyRsp = AllocateZeroPool (RecvLength);
  if (HsioReadMphyRsp == NULL) {
    DEBUG ((DEBUG_ERROR, "%a(): Could not allocate Memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  Heci = GetHeci (&HeciApiVersion);
  if (Heci == NULL) {
    FreePool (HsioReadMphyRsp);
    return EFI_UNSUPPORTED;
  }

  if (HeciApiVersion == HeciApiHeciControl) {
    HsioReadMphyReq = AllocateZeroPool (Length);
    if (HsioReadMphyReq == NULL) {
      DEBUG ((DEBUG_ERROR, "(HSIO) %a: Could not allocate Memory\n", __FUNCTION__));
      FreePool (HsioReadMphyRsp);
      return EFI_OUT_OF_RESOURCES;
    }
  } else {
    HsioReadMphyReq = (BUP_MPHY_READ_FROM_MPHY *) HsioReadMphyRsp;
  }

  ZeroMem (HsioReadMphyReq, sizeof (BUP_MPHY_READ_FROM_MPHY));

  HsioReadMphyReq->MkhiHeader.Data           = 0;
  HsioReadMphyReq->MkhiHeader.Fields.GroupId = BUP_MPHY_GROUP_ID;
  HsioReadMphyReq->MkhiHeader.Fields.Command = MPHY_READ_FROM_MPHY_CMD;
  HsioReadMphyReq->NumOfBytes                = DataSize;

  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) HsioReadMphyReq,
             Length,
             (UINT32 *) HsioReadMphyRsp,
             &RecvLength,
             &SendStatus,
             &ReceiveStatus
             );

  if (!EFI_ERROR (Status) &&
      (HsioReadMphyRsp->MkhiHeader.Fields.Command == MPHY_READ_FROM_MPHY_CMD) &&
      (HsioReadMphyRsp->MkhiHeader.Fields.IsResponse == 1) &&
      (HsioReadMphyRsp->MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
    CopyMem (Data, HsioReadMphyRsp->Data, DataSize);
  }

  if (HsioReadMphyReq != (BUP_MPHY_READ_FROM_MPHY *) HsioReadMphyRsp) {
    FreePool (HsioReadMphyReq);
  }
  FreePool (HsioReadMphyRsp);

  DEBUG ((DEBUG_INFO, "%a(): End\n", __FUNCTION__));

  return Status;
}


//
// BUP_PM_GROUP_ID = 0xF3
//

/**
  Send Host Reset Notification Message to determine if warm reset is required.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciSendHostResetNotificationMsg (
  VOID
  )
{
  EFI_STATUS             Status;
  UINT32                 Length;
  UINT32                 RecvLength;
  HR_NOTIFICATION_BUFFER HrNotification;
  VOID                   *Heci;

  if (MeTypeIsDisabled ()) {
    return EFI_UNSUPPORTED;
  }

  Heci = GetHeci (NULL);
  if (Heci == NULL) {
    return EFI_UNSUPPORTED;
  }

  HrNotification.Request.MkhiHeader.Fields.IsResponse  = 0;
  HrNotification.Request.MkhiHeader.Fields.Result      = 0;
  HrNotification.Request.MkhiHeader.Data               = 0;
  HrNotification.Request.MkhiHeader.Fields.GroupId     = BUP_PM_GROUP_ID;
  HrNotification.Request.MkhiHeader.Fields.Command     = NPCR_NOTIFICATION_CMD;
  Length                                               = sizeof (HR_NOTIFICATION_CMD);
  RecvLength                                           = sizeof (HR_NOTIFICATION_CMD_RESP);

  ///
  /// Send Host Reset Notification Request to ME
  ///
  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &HrNotification.Request,
             Length,
             (UINT32 *) &HrNotification.Response,
             &RecvLength,
             NULL,
             NULL
             );

  if (!EFI_ERROR (Status) &&
      (HrNotification.Response.MkhiHeader.Fields.GroupId == BUP_PM_GROUP_ID) &&
      (HrNotification.Response.MkhiHeader.Fields.Command == NPCR_NOTIFICATION_CMD) &&
      (HrNotification.Response.MkhiHeader.Fields.IsResponse == 1) &&
      (HrNotification.Response.MkhiHeader.Fields.Result == MkhiStatusSuccess) &&
      (HrNotification.Response.ResetRequested == 0x1)) {
    (*GetPeiServicesTablePointer ())->ResetSystem2 (EfiResetWarm, EFI_SUCCESS, 0, NULL);
  } else {
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

//
// MKHI_GEN_GROUP_ID = 0xFF
//

/**
  Get MKHI (Management Engine Kernel Host Interface) Version.

  @param[out] MkhiVersion           MKHI Version

  @retval EFI_UNSUPPORTED           Can't localize HECI protocol
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetMkhiVersion (
  OUT MKHI_VERSION        *MkhiVersion
  )
{
  GET_MKHI_VERSION        Request;
  GET_MKHI_VERSION_ACK    Response;
  UINT32                  Length;
  UINT32                  RecvLength;
  EFI_STATUS              Status;
  VOID                    *Heci;
  HECI_API_VERSION        HeciVersion;

  Heci = GetHeci (&HeciVersion);
  if (Heci == NULL) {
    return EFI_UNSUPPORTED;
  }

  Request.MkhiHeader.Data           = 0;
  Request.MkhiHeader.Fields.GroupId = MKHI_GEN_GROUP_ID;
  Request.MkhiHeader.Fields.Command = GEN_GET_MKHI_VERSION_CMD;
  Length                            = sizeof (GET_MKHI_VERSION);
  RecvLength                        = sizeof (GET_MKHI_VERSION_ACK);

  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32*) &Request,
             Length,
             (UINT32*) &Response,
             &RecvLength,
             NULL,
             NULL
             );

  if (!EFI_ERROR (Status) &&
      (Response.MkhiHeader.Fields.Command == GEN_GET_MKHI_VERSION_CMD) &&
      (Response.MkhiHeader.Fields.IsResponse == 1) &&
      (Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
    *MkhiVersion = Response.MkhiVersion;
  }

  return Status;
}

//
// Interface functions of HeciMsgLib
//

//
// ME Client - Integrated Clock Chip (ICC)
//

/**
  Read the ChipsetInit table from CSME

  @param[out]     ChipsetInitTable     Pointer to the required system ChipsetInit Table.
  @param[in, out] ChipsetInitTableLen  Pointer to the length of the table in bytes

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciReadChipsetInitMsg (
  OUT UINT8             *ChipsetInitTable,
  IN OUT UINT32         *ChipsetInitTableLen
  )
{
  EFI_STATUS                     Status;
  UINT32                         Length;
  UINT32                         RecvLength;
  ICC_READ_FROM_MPHY_REQ         *HsioReadMphy;
  EFI_STATUS                     SendStatus;
  EFI_STATUS                     ReceiveStatus;
  ICC_READ_FROM_MPHY_ACK         *HsioReadMphyRsp;
  VOID                           *Heci;
  HECI_API_VERSION               HeciApiVersion;

  if ((ChipsetInitTable == NULL) || (ChipsetInitTableLen == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "%a(): Start\n", __FUNCTION__));

  if (!IsMeAvailableForHsio ()) {
    return EFI_UNSUPPORTED;
  }

  Length     = sizeof (ICC_READ_FROM_MPHY_REQ);
  RecvLength = sizeof (ICC_READ_FROM_MPHY_ACK) + *ChipsetInitTableLen;

  HsioReadMphyRsp = AllocateZeroPool (RecvLength);
  if (HsioReadMphyRsp == NULL) {
    DEBUG ((DEBUG_ERROR, "(HSIO) PeiHeciReadChipsetInitMsg: Could not allocate Memory\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  Heci = GetHeci (&HeciApiVersion);
  if (Heci == NULL) {
    FreePool (HsioReadMphyRsp);
    return EFI_UNSUPPORTED;
  }

  if (HeciApiVersion == HeciApiHeciControl) {
    HsioReadMphy = AllocateZeroPool (Length);
    if (HsioReadMphy == NULL) {
      DEBUG ((DEBUG_ERROR, "(HSIO) PeiHeciReadChipsetInitMsg: Could not allocate Memory\n"));
      FreePool (HsioReadMphyRsp);
      return EFI_OUT_OF_RESOURCES;
    }
  } else {
    HsioReadMphy = (ICC_READ_FROM_MPHY_REQ*) HsioReadMphyRsp;
  }

  HsioReadMphy->Header.ApiVersion   = ICC_API_VERSION;
  HsioReadMphy->Header.IccCommand   = ICC_READ_FROM_MPHY_CMD;
  HsioReadMphy->Header.BufferLength = ICC_MESSAGE_BUFFER_LENGTH (Length);
  HsioReadMphy->Offset              = 0;
  HsioReadMphy->NumOfBytes          = *ChipsetInitTableLen;

  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_ICC_MESSAGE_ADDR,
             (UINT32 *) HsioReadMphy,
             Length,
             (UINT32 *) HsioReadMphyRsp,
             &RecvLength,
             &SendStatus,
             &ReceiveStatus
             );

  if (!EFI_ERROR (Status)) {
    if (HsioReadMphyRsp->Header.IccResponse == ICC_STATUS_SUCCESS) {
      if (HsioReadMphyRsp->ReadSize > PCH_HSIO_CHIPSETINIT_TBL_MAX_SIZE) {
        *ChipsetInitTableLen = 0;
        ASSERT (FALSE);  // ChipsetInit table should not get too large
        Status = EFI_DEVICE_ERROR;
      } else {
        if (*ChipsetInitTableLen < HsioReadMphyRsp->ReadSize) {
          *ChipsetInitTableLen = 0;
          ASSERT (FALSE);  // ChipsetInit buffer size is too small to copy full data
          Status = EFI_BUFFER_TOO_SMALL;
        } else {
          CopyMem (ChipsetInitTable, HsioReadMphyRsp->Payload, HsioReadMphyRsp->ReadSize);
          *ChipsetInitTableLen = HsioReadMphyRsp->ReadSize;
        }
      }
    } else {
      *ChipsetInitTableLen = 0;
      DEBUG ((DEBUG_ERROR, "(HSIO) ERROR: Get HSIO Settings failed!: FW Response=0x%x\n", HsioReadMphyRsp->Header.IccResponse));
      Status = EFI_DEVICE_ERROR;
    }
    if (EFI_ERROR (Status) && MeTypeIsSps () && (HeciApiVersion == HeciApiHeciControl)) {
      MeEwlCommunicationFailures (
        MeEwlSeverityWarning,
        ME_EWL_CHKP_MAJOR_POSTINIT,
        ME_EWL_CHKP_POSTINIT_MPHY,
        ME_EWL_WARN_MAJOR_HECI,
        ME_EWL_WARN_HECI_MESSAGE,
        HECI1_DEVICE,            // HECI device
        HECI_ICC_MESSAGE_ADDR,   // MeAddress
        SendStatus,              // Send Status
        ReceiveStatus,           // Receive Status
        *(UINT64 *) HsioReadMphy,  // Request
        *(UINT32 *) HsioReadMphyRsp// Response
        );
    }
  }
  if (HsioReadMphy != (ICC_READ_FROM_MPHY_REQ*) HsioReadMphyRsp) {
    FreePool (HsioReadMphy);
  }
  FreePool (HsioReadMphyRsp);

  DEBUG ((DEBUG_INFO, "%a(): End\n", __FUNCTION__));

  return Status;
}

/**
  Send the required system ChipsetInit Table to ME FW.

  @param[in] ChipsetInitTable     Pointer to the required system ChipsetInit Table
  @param[in] ChipsetInitTableLen  Length of the table in bytes

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES    HECI Could not allocate Memory
**/
EFI_STATUS
PeiHeciWriteChipsetInitMsg (
  IN  UINT8                       *ChipsetInitTable,
  IN  UINT32                      ChipsetInitTableLen
  )
{
  EFI_STATUS                     Status;
  UINT32                         ReqSize;
  ICC_SET_MPHY_SETTINGS_REQ      *SetMphySettings;
  ICC_SET_MPHY_SETTINGS_RSP      *SetMphySettingsRsp;
  EFI_STATUS                     SendStatus;
  EFI_STATUS                     ReceiveStatus;
  UINT32                         RespSize;
  VOID                           *Heci;
  HECI_API_VERSION               HeciApiVersion;

  DEBUG ((DEBUG_INFO, "%a(): Start\n", __FUNCTION__));
  if (ChipsetInitTableLen > PCH_HSIO_CHIPSETINIT_TBL_MAX_SIZE) {
    return EFI_DEVICE_ERROR;
  }

  if (!IsMeAvailableForHsio ()) {
    return EFI_UNSUPPORTED;
  }

  Heci = GetHeci (&HeciApiVersion);
  if (Heci == NULL) {
    return EFI_UNSUPPORTED;
  }

  ReqSize = sizeof (ICC_SET_MPHY_SETTINGS_REQ) + ChipsetInitTableLen;
  SetMphySettings = AllocateZeroPool (ReqSize);
  if (SetMphySettings == NULL) {
    DEBUG ((DEBUG_ERROR, "%a() : Could not allocate Memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  SetMphySettings->Header.ApiVersion   = ICC_API_VERSION;
  SetMphySettings->Header.IccCommand   = ICC_SET_MPHY_SETTINGS_CMD;
  SetMphySettings->Header.BufferLength = ICC_MESSAGE_BUFFER_LENGTH (ReqSize);
  CopyMem (&SetMphySettings->MphyData, ChipsetInitTable, ChipsetInitTableLen);
  RespSize = sizeof (ICC_SET_MPHY_SETTINGS_REQ);

  if (HeciApiVersion == HeciApiHeciControl) {
    SetMphySettingsRsp = AllocateZeroPool (RespSize);
    if (SetMphySettingsRsp == NULL) {
      DEBUG ((DEBUG_ERROR, "(HSIO) PeiHeciChipsetInitSyncMsg: Could not allocate Memory\n"));
      FreePool (SetMphySettings);
      return EFI_OUT_OF_RESOURCES;
    }
  } else {
    SetMphySettingsRsp = (ICC_SET_MPHY_SETTINGS_RSP*)SetMphySettings;
  }

  //
  // Send ChipsetInit Table to ME
  //
  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_ICC_MESSAGE_ADDR,
             (UINT32 *) SetMphySettings,
             ReqSize,
             (UINT32 *) SetMphySettingsRsp,
             &RespSize,
             &SendStatus,
             &ReceiveStatus
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "(HSIO) ERROR: Write HSIO Settings Message failed! EFI_STATUS = %r\n", Status));
  } else if (SetMphySettingsRsp->Header.IccResponse != ICC_STATUS_SUCCESS ||
             SetMphySettingsRsp->Header.IccCommand != ICC_SET_MPHY_SETTINGS_CMD) {
    DEBUG ((DEBUG_ERROR, "(HSIO) ERROR: Write HSIO Settings failed!: FW Response = 0x%x\n", SetMphySettingsRsp->Header.IccResponse));
    Status = EFI_DEVICE_ERROR;
  }
  if (EFI_ERROR (Status)) {
    MeEwlCommunicationFailures (
      MeEwlSeverityWarning,
      ME_EWL_CHKP_MAJOR_POSTINIT,
      ME_EWL_CHKP_POSTINIT_MPHY,
      ME_EWL_WARN_MAJOR_HECI,
      ME_EWL_WARN_HECI_MESSAGE,
      HECI1_DEVICE,               // HECI device
      HECI_ICC_MESSAGE_ADDR,      // MeAddress
      SendStatus,                 // Send Status
      ReceiveStatus,              // Receive Status
      *(UINT64 *) SetMphySettings,  // Request
      *(UINT32 *) SetMphySettingsRsp// Response
      );
  }

  if (SetMphySettingsRsp != (ICC_SET_MPHY_SETTINGS_RSP *) SetMphySettings) {
    FreePool (SetMphySettingsRsp);
  }
  FreePool (SetMphySettings);

  DEBUG ((DEBUG_INFO, "%a(): End\n", __FUNCTION__));
  return Status;
}

/**
  Retrieves information on whether Pcie root port configuration SoftStrap override has been done or not
  This command retrieves information from CSME if it performed SoftStrap override in the current boot

  @param[in]      NumberOfControllers    On input, it is the number of controllers caller expects.
  @param[in, out] ControllerConfig       Pointer to the controller config message.

  @retval EFI_SUCCESS                   Command succeeded
  @retval EFI_DEVICE_ERROR              HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                   HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL          Message Buffer is too small for the Acknowledge
  @retval EFI_UNSUPPORTED               Current ME mode doesn't support send this message through this HECI
  @retval EFI_OUT_OF_RESOURCES          Unable to allocate required resources
**/
EFI_STATUS
PeiHeciGetSoftStrpConfigMsg (
  IN      UINT16     NumberOfControllers,
  IN OUT  UINT8      *ControllerConfig
  )
{
  EFI_STATUS                             Status;
  UINT32                                 CommandSize;
  UINT32                                 ResponseSize;
  ICC_GET_SOFT_STRAP_CONFIG_CMD_BUFFER   *GetSoftStrapBuffer;
  VOID                                   *Heci;

  DEBUG ((DEBUG_INFO, "(ICC) PeiHeciGetSoftStrpConfigMsg Message\n"));

  Heci = GetHeci (NULL);
  if (Heci == NULL) {
    return EFI_UNSUPPORTED;
  }

  //(4 * sizeof (UINT32)) is needed for reserved field
  ResponseSize         = NumberOfControllers * sizeof (UINT8) + sizeof (ICC_GET_SOFT_STRAP_CONFIG_CMD_RESP) + (4 * sizeof (UINT32));
  GetSoftStrapBuffer   = AllocateZeroPool (ResponseSize);
  if (GetSoftStrapBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CommandSize                                     = sizeof (ICC_GET_SOFT_STRAP_CONFIG_CMD_REQ);
  GetSoftStrapBuffer->Message.Header.ApiVersion   = ICC_API_VERSION;
  GetSoftStrapBuffer->Message.Header.IccCommand   = ICC_GET_SOFT_STRAP_CONFIG_CMD;
  GetSoftStrapBuffer->Message.Header.BufferLength = ICC_MESSAGE_BUFFER_LENGTH (CommandSize);

  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_ICC_MESSAGE_ADDR,
             (UINT32 *) GetSoftStrapBuffer,
             CommandSize,
             (UINT32 *) GetSoftStrapBuffer,
             &ResponseSize,
             NULL,
             NULL
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Message failed! Status = %r\n", Status));
  } else if (GetSoftStrapBuffer->Response.Header.IccResponse != ICC_STATUS_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Wrong response! IccHeader.Response = 0x%x\n", GetSoftStrapBuffer->Response.Header.IccResponse));
    Status = EFI_DEVICE_ERROR;
  } else if (GetSoftStrapBuffer->Response.NumOfControllers != NumberOfControllers) {
    DEBUG ((DEBUG_ERROR, "Controller number does not match! Number Of Controllers = 0x%x\n", GetSoftStrapBuffer->Response.NumOfControllers));
    Status = EFI_DEVICE_ERROR;
  } else {
    CopyMem (ControllerConfig, GetSoftStrapBuffer->Response.ControllerConfig, GetSoftStrapBuffer->Response.NumOfControllers);
  }

  FreePool (GetSoftStrapBuffer);
  DEBUG ((DEBUG_INFO, "(ICC) PeiHeciGetSoftStrpConfigMsg Status = %r\n", Status));
  return Status;
}

/**
  Set Pcie Root port configuration SoftStrap override for the specified Pcie Root Port controller

  @param[in] NumberOfControllers         On input, it is the number of controllers caller expects.
  @param[in] OverrideMask                Mask with RpController where Hybrid Storage is present and softstraps need to be overridden

  @retval EFI_SUCCESS                    Command succeeded
  @retval EFI_DEVICE_ERROR               HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                    HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL           Message Buffer is too small for the Acknowledge
  @retval EFI_UNSUPPORTED                Current ME mode doesn't support send this message through this HECI
  @retval EFI_OUT_OF_RESOURCES           Unable to allocate required resources
**/
EFI_STATUS
PeiHeciOverrideSoftStrapMsg (
  IN UINT16                                NumberOfControllers,
  IN UINT8                                 OverrideMask
  )
{
  EFI_STATUS                                  Status;
  UINT32                                      CommandSize;
  UINT32                                      ResponseSize;
  ICC_OVERRIDE_PCIE_SOFT_STRAP_CMD_BUFFER     *OverrideSoftStrapBuffer;
  UINT8                                       *ConfigBuffer;
  UINT8                                       Index;
  VOID                                        *Heci;

  DEBUG ((DEBUG_INFO, "(ICC) PeiHeciOverrideSoftStrapMsg Message\n"));

  Heci = GetHeci (NULL);
  if (Heci == NULL) {
    return EFI_UNSUPPORTED;
  }

  CommandSize = NumberOfControllers * sizeof (UINT8) + sizeof (ICC_OVERRIDE_SOFT_STRAP_CMD_REQ) + (4 * sizeof (UINT32));//(4 * sizeof (UINT32)) is needed for reserved field
  OverrideSoftStrapBuffer = AllocateZeroPool (CommandSize);
  if (OverrideSoftStrapBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ResponseSize                                         = sizeof (ICC_OVERRIDE_SOFT_STRAP_CMD_RESP);
  OverrideSoftStrapBuffer->Message.Header.ApiVersion   = ICC_API_VERSION;
  OverrideSoftStrapBuffer->Message.Header.IccCommand   = ICC_OVERRIDE_SOFT_STRAP_CMD;
  OverrideSoftStrapBuffer->Message.Header.BufferLength = ICC_MESSAGE_BUFFER_LENGTH (CommandSize);
  OverrideSoftStrapBuffer->Message.NumOfControllers    = NumberOfControllers;
  ConfigBuffer                                         = OverrideSoftStrapBuffer->Message.ControllerConfig;
  for (Index = 0; Index < NumberOfControllers; Index++) {
    if ((OverrideMask & (BIT0 << Index)) != 0) {
      ConfigBuffer[Index] = PcieOverride2x2;
    } else {
      ConfigBuffer[Index] = PcieOverrideNone;
    }
  }

  Status = HeciSendWithAck (
             Heci,
             HECI_DEFAULT_PCH_ID,
             NULL,
             HECI_DEFAULT_RETRY_NUMBER,
             BIOS_FIXED_HOST_ADDR,
             HECI_ICC_MESSAGE_ADDR,
             (UINT32 *) OverrideSoftStrapBuffer,
             CommandSize,
             (UINT32 *) OverrideSoftStrapBuffer,
             &ResponseSize,
             NULL,
             NULL
             );
  if (EFI_ERROR (Status) || (OverrideSoftStrapBuffer->Response.Header.IccResponse != ICC_STATUS_SUCCESS)) {
    DEBUG ((DEBUG_ERROR, "(ICC) PeiHeciOverrideSoftStrapMsg: Message failed! Status = %r\n", Status));
    Status = EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "(ICC) PeiHeciOverrideSoftStrapMsg: Message Status = %r\n", Status));
  FreePool (OverrideSoftStrapBuffer);
  return Status;
}
