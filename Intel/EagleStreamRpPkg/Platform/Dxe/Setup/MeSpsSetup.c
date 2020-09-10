/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary    and
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
**/

#if defined(ME_SPS_SUPPORT) && ME_SPS_SUPPORT

#include "SetupPlatform.h"
#include <Library/PciLib.h>
#include <Library/HiiLib.h>
#include <Library/HobLib.h>
#include <Protocol/HeciControlProtocol.h>
#include <Library/DxeMeLib.h>
#include <Library/ServerDxeMeLib.h>
#include <Library/PchFiaLib.h>
#include <ConfigBlock/SpsGetDxeConfigBlock.h>
#include <Sps.h>
#include <Guid/SpsInfoHobGuid.h>
#include "Library/MeTypeLib.h"
#include "MeAccess.h"
#include "MeState.h"
#include <MeDefines.h>

#include <Library/SetupLib.h>
#include <Library/PttHeciLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/MeFiaMuxLib.h>
#include "MeSpsSetup.h"
#include <MeSpsFiaMux.h>

#include <PchSetupVariableCdfEbg.h>

//
// Variables required to Manage changes in Setup,
//

// original set, any change after After EndOfPost requires reset
extern ME_RC_CONFIGURATION MeRcConfigurationOrg;
// last processed set to determine is anything has been changed since last time
extern ME_RC_CONFIGURATION MeRcConfigurationProcessed;

EFI_GUID mFormSetGuid = SYSTEM_CONFIGURATION_GUID;

/**

  This function converts CPU related data as ASCII and put in provided string

  @param HiiAdvancedHandle  BIOS setup database handle
  @param String             pointer for string which will be displayed
  @param StartOpCodeHandle  pointer for StartOpCodeHandle

**/
VOID
UpdateTokenValue (
  IN EFI_HII_HANDLE      HiiHandle,
  IN CHAR16              *String,
  IN VOID                *StartOpCodeHandle
  )
{
  EFI_STRING_ID   TokenToUpdate;

  TokenToUpdate = 0;
  TokenToUpdate = HiiSetString (
    HiiHandle,
    0,
    String,
    NULL
  );

  if (TokenToUpdate == (EFI_STRING_ID)(0)) {
    DEBUG ((DEBUG_ERROR, "[ME] Error: The string could not be added.\n"));
    return;
  }

  HiiCreateTextOpCode (
    StartOpCodeHandle,
    TokenToUpdate,
    STRING_TOKEN (STR_NULL_STRING),
    STRING_TOKEN (STR_NULL_STRING)
  );
}

/**

  This function updates SPS Feature Set in BIOS Setup

  @param[in] HiiHandle      BIOS setup database handle
  @param[in] SpsFeatureSet  pointer to variable with Sps Feature Set

**/
VOID
MeInitSpsFeatureSetStrings (
  IN EFI_HII_HANDLE   HiiHandle,
  IN SPS_FEATURE_SET  *SpsFeatureSet
  )
{
  CHAR16                StringBuffer[ME_SPS_INFO_MAX_STR_SIZE];
  VOID                  *StartOpCodeHandle;
  EFI_IFR_GUID_LABEL    *StartLabel;
  VOID                  *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL    *EndLabel;

  if (HiiHandle == NULL || SpsFeatureSet == NULL) {
    return;
  }

  //
  // Initialize the container for dynamic opcodes
  //
  StartOpCodeHandle = HiiAllocateOpCodeHandle ();

  if (StartOpCodeHandle == NULL) {
    return;
  }

  EndOpCodeHandle = HiiAllocateOpCodeHandle ();

  if (EndOpCodeHandle == NULL) {
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    return;
  }

  //
  // Create Hii Extend Label OpCode as the start opcode
  //
  StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = FEATURE_SET_LABEL_BEGIN;

  //
  // Create Hii Extend Label OpCode as the end opcode
  //
  EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number       = FEATURE_SET_LABEL_END;

  UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  SiEn");
  UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);

  if (SpsFeatureSet->Bits.NodeManager) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  NodeManager");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.PeciProxy) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  PECIProxy");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.ICC) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  ICC");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.MeStorageServices) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  MeStorageServices");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.BootGuard) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  BootGuard");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.PTT) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  PTT");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.OemDefinedCpuDebugPolicy) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  OemDefinedCpuDebugPolicy");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.ResetSuppression) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  ResetSuppression");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.PmBusProxy) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  PmBusProxy");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.CpuHotPlug) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  CpuHotPlug");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.MicProxy) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  MicProxy");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.MctpProxy) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  MctpProxy");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.ThermalReportVolumeAirflow) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  ThermalReport");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.SocThermalReporting) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  SocThermalReporting");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.DualBIOSSupport) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  DualBiosImage");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.MPHYSurvivabilityProgramming) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  HSIO");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.PECIOverDMI) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  PECIOverDMI");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.PCHDebug) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  PCHDebug");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.PowerThermalUtilitySupport) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  PowerThermalUtility");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.FiaMuxConfiguration) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  FiaMuxConfiguration");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.PchThermalSensorInit) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  PCHThermalSensorInit");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.DeepSx) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  DeepSx");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.DualMeFwImage) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  DualMeImage");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.DirectFwUpdate) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  DirectMeUpdate");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.MctpInfrastructure) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  MctpInfrastructure");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.CUPS) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  CUPS");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.FlashDescRegionVerification) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  FlashDescRegionVerification");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.Hotham) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  Hotham");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.TurboStateLimiting) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  TurboStateLimiting");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.TelemetryHub) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  TelemetryHub");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.MeShutdown) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  MeShutdown");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.ASA) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  ASA");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  if (SpsFeatureSet->Bits.WarmResetNotificationSubFlow) {
    UnicodeSPrint (StringBuffer, ME_SPS_INFO_MAX_STR_SIZE, L"  WarmResetNotificationSubFlow");
    UpdateTokenValue (HiiHandle, StringBuffer, StartOpCodeHandle);
  }

  //
  // Update Sps Feature Set
  //
  HiiUpdateForm (
    HiiHandle,          // HII handle
    &mFormSetGuid,      // Formset GUID
    VFR_FORMID_SPS_ME,  // Form ID
    StartOpCodeHandle,  // Label for where to insert opcodes
    EndOpCodeHandle     // Replace data
    );

  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
}

/**
  This function updates PSUs Status Info in BIOS Setup

  @param[out] HiiAdvancedHandle BIOS setup database handle
  @param[in] HeciProtocolPtr HECI protocol pointer

  @return Void
**/
VOID
MeUpdateSpsPsuStatusInfoStrings (
  OUT EFI_HII_HANDLE        HiiAdvancedHandle,
  IN HECI_CONTROL           *HeciProtocolPtr
  )
{
  EFI_STATUS                        Status;
  CHAR16                            *StrBufPtr = NULL;
  UINT32                            RequestLength;
  UINT32                            ResponseLengthReceived;
  UINT32                            ResponseLengthExpected;
  UINT8                             PsuIdx;
  PMBUS_STATUS_WORD_MESSAGE_CONTENT PmbusStatusWordContent;
  ME_RC_CONFIGURATION               MeRcConfiguration;
  struct {
    SPS_MSG_PMBUS_CMD_SEND_RAW_REQ  PmbusCmdRawReq;
    SPS_MSG_PMBUS_CMD_SEND_RAW_RSP  PmbusCmdRawRsp;
  } HeciMsg;
  EFI_STRING_ID                     PsuStringId[ME_SPS_PSU_NUM_MAX] = {
    STR_SPS_PSU1_STATUS_VALUE,
    STR_SPS_PSU2_STATUS_VALUE,
    STR_SPS_PSU3_STATUS_VALUE,
    STR_SPS_PSU4_STATUS_VALUE
  };

  DEBUG ((
    DEBUG_INFO,
    "[ME] MeUpdateSpsPsuStatusInfoStrings() called\n"
    ));

  if ((NULL == HiiAdvancedHandle) || (NULL == HeciProtocolPtr)) {
    DEBUG ((
      DEBUG_ERROR,
      "[ME] ERROR: MeUpdateSpsPsuStatusInfoStrings() input parameter invalid\n"
      ));
    return;
  }

  Status = GetSpecificConfigGuid (
    &gEfiMeRcVariableGuid,
    (VOID*)&MeRcConfiguration
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "[ME] ERROR: Failed to read ME_RC_CONFIGURATION!  Status = %r\n",
      Status
      ));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  if (MeRcConfiguration.SpsPmbusProxySupported) {
    StrBufPtr = AllocateZeroPool (ME_SPS_INFO_MAX_STR_SIZE);
    ASSERT (NULL != StrBufPtr);

    if (NULL != StrBufPtr) {
      for (PsuIdx = 0; PsuIdx < ME_SPS_PSU_NUM_MAX; PsuIdx++) {
        //
        // Check if query should be sent to PSU
        //
        if (ME_SPS_PMBUS_ADDR_NONE != MeRcConfiguration.SpsPsuAddress[PsuIdx]) {
          DEBUG ((
            DEBUG_INFO,
            "[ME] Sending SPS_CMD_PMBUS_CMD_SEND_RAW_REQ request to ME for PSU #%d, PMbus (7-bit) address: 0x%X\n",
            PsuIdx,
            MeRcConfiguration.SpsPsuAddress[PsuIdx]
            ));

          gBS->SetMem (&HeciMsg.PmbusCmdRawReq, sizeof (HeciMsg.PmbusCmdRawReq), 0);
          gBS->SetMem (&HeciMsg.PmbusCmdRawReq, sizeof (HeciMsg.PmbusCmdRawRsp), 0);
          HeciMsg.PmbusCmdRawReq.Command.Data = SPS_CMD_PMBUS_CMD_SEND_RAW_REQ;
          HeciMsg.PmbusCmdRawReq.PmbusParameters.PsuAddress.Fields.SmbusAddress = MeRcConfiguration.SpsPsuAddress[PsuIdx];
          HeciMsg.PmbusCmdRawReq.PmbusParameters.Flags.Flags.SmbusMessageTransactionType = PMBUS_CMD_READ_WORD;
          HeciMsg.PmbusCmdRawReq.PmbusParameters.WriteLength = PMBUS_CMD_WRITE_LENGTH_READ_WORD;
          HeciMsg.PmbusCmdRawReq.PmbusParameters.ReadLength = PMBUS_CMD_READ_LENGTH_READ_WORD;
          HeciMsg.PmbusCmdRawReq.PmbusCommandData.RequestData[0] = PMBUS_CMD_STATUS_WORD;

          RequestLength = SpsMsgPmbusCmdSendRawGetReqLength (HeciMsg.PmbusCmdRawReq.PmbusParameters.WriteLength);
          ResponseLengthExpected = SpsMsgPmbusCmdSendRawGetRspLength (HeciMsg.PmbusCmdRawReq.PmbusParameters.ReadLength);

          ResponseLengthReceived = ResponseLengthExpected;
          Status = HeciProtocolPtr->HeciSendAndReceive (
                                      HeciProtocolPtr,
                                      HECI_DEFAULT_PCH_ID,
                                      NULL,
                                      HECI_DEFAULT_RETRY_NUMBER,
                                      SPS_CLIENTID_BIOS,
                                      SPS_CLIENTID_ME_SPS,
                                      (UINT32*) &HeciMsg.PmbusCmdRawReq,
                                      RequestLength,
                                      (UINT32*) &HeciMsg.PmbusCmdRawRsp,
                                      &ResponseLengthReceived,
                                      NULL,
                                      NULL
                                     );
          if (!EFI_ERROR (Status)) {
            if (SpsMsgPmbusCmdSendRawRspLengthVerify (ResponseLengthReceived, ResponseLengthExpected)) {
              if (ResponseLengthReceived != SpsMsgPmbusCmdSendRawNoPmbusDataRspLength ()) {
                if (PMBUS_CMD_RESULT_SUCCESS == HeciMsg.PmbusCmdRawRsp.PmbusParameters.Result) {
                  if (ResponseLengthReceived != SpsMsgPmbusCmdSendRawNoPmbusDataRspLength ()) {
                    gBS->CopyMem (&PmbusStatusWordContent, &HeciMsg.PmbusCmdRawRsp.PmbusCommandData.ResponseData, sizeof (PmbusStatusWordContent));
                    if (PMBUS_STATUS_WORD_NO_ERRORS == PmbusStatusWordContent.Content) {
                      UnicodeSPrint (StrBufPtr, ME_SPS_INFO_MAX_STR_SIZE, L"%s", L"OK");
                    } else {
                      UnicodeSPrint (StrBufPtr, ME_SPS_INFO_MAX_STR_SIZE, L"ERROR: 0x%X", PmbusStatusWordContent.Content);
                    }
                    HiiSetString (HiiAdvancedHandle, STRING_TOKEN(PsuStringId[PsuIdx]), StrBufPtr, NULL);
                  }
                }
              } else {
                //
                // No PMBus Command data received, only Send Raw PMBus Command Completion Code
                //
                DEBUG ((DEBUG_WARN, "[ME]: No PMBus Command data, only SPS_CMD_PMBUS_CMD_SEND_RAW_REQ Completion Code (0x%X) received for PSU #%d\n",
                  HeciMsg.PmbusCmdRawRsp.PmbusParameters.Result, PsuIdx));
              }
            } else {
              DEBUG ((DEBUG_ERROR, "[ME] ERROR: Invalid SPS_CMD_PMBUS_CMD_SEND_RAW_REQ response length for PSU #%d (length %d)\n",
                PsuIdx, ResponseLengthReceived));
            }
          } else {
            DEBUG ((DEBUG_ERROR, "[ME] ERROR: Cannot send SPS_CMD_PMBUS_CMD_SEND_RAW_REQ for PSU #%d (status: %r)\n",
              PsuIdx, Status));
            FreePool (StrBufPtr);
            return;
          }
        }
      }

      FreePool (StrBufPtr);
      StrBufPtr = NULL;
    }
  }
}

/**
  This function updates ME related strings in BIOS setup database.

  Some of the strings contain dummy values to be update with run-time data.
  These strings are updated by this function before any setup screen is
  printed. For some reason the strings from various screens,
  although identified with unique ids, are stored in separate databases.
  Class identifies the database. To avoid multiple requests to ME
  and multiple traces parts of this function are executed for particular
  forms.

  @param[out] HiiAdvancedHandle BIOS setup database handle
  @param[out] HiiMainHandle     BIOS setup database handle
**/
VOID
MeInitSpsSetupStrings (
  OUT EFI_HII_HANDLE HiiAdvancedHandle,
  OUT EFI_HII_HANDLE HiiMainHandle
  )
{
  EFI_STATUS            Status;
  HECI_CONTROL          *pHeciProtocol;
  HECI_FWS_REGISTER     MeFs1;
  SPS_MEFS2             MeFs2;
  UINT32                RspLen;
  CHAR16                StrBuf[ME_SPS_INFO_MAX_STR_SIZE];
  CHAR16*               MeTypeString;
  ME_RC_CONFIGURATION   MeRcConfiguration;
  EFI_HOB_GUID_TYPE     *GuidHob;
  SPS_INFO_HOB          *SpsInfo;

  struct {
    MKHI_MSG_GET_FW_VERSION_REQ      FwVerReq;
    MKHI_MSG_GET_FW_VERSION_RSP      FwVerRsp;
  } HeciMsg;

  DEBUG ((DEBUG_INFO, "[ME] MeInitSpsInfo called...\n"));
  // APTIOV_SERVER_OVERRIDE_RC_START
  if(NULL != HiiAdvancedHandle) { // && (NULL != HiiMainHandle)) 
  // APTIOV_SERVER_OVERRIDE_RC_END
    //
    // Init MEFS#1 info in setup string database
    //
    MeFs1.ul = HeciPciRead32 (R_ME_HFS);
    if (MeFs1.ul == 0xFFFFFFFF) {
      MeFs1.ul = GetMeFwsts1FromHob (MeFs1.ul);
    }
    DEBUG ((DEBUG_INFO, "[ME] MEFS1: 0x%08X\n", MeFs1.ul));
    UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"0x%08X", MeFs1.ul);
    HiiSetString (HiiAdvancedHandle, STRING_TOKEN (STR_ME_FWSTS1_VALUE), StrBuf, NULL);

    //
    // Init MEFS#2 info in setup string database
    //
    MeFs2.UInt32 = HeciPciRead32 (R_ME_HFS_2);
    DEBUG ((DEBUG_INFO, "[ME] MEFS2: 0x%08X\n", MeFs2.UInt32));
    UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"0x%08X", MeFs2.UInt32);
    HiiSetString (HiiAdvancedHandle, STRING_TOKEN (STR_ME_FWSTS2_VALUE), StrBuf, NULL);

    //
    // Update ME Current State in setup strings database
    //
    switch (MeFs1.r.CurrentState) {
      case ME_STATE_RESET:
        UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"Reset");
        break;

      case ME_STATE_INIT:
        UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"Initializing");
        break;

      case ME_STATE_RECOVERY:
        UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"Recovery");
        break;

      case ME_STATE_DISABLED:
        UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"Disabled");
        break;

      case ME_STATE_NORMAL:
        UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"Operational");
        break;

      case ME_STATE_TRANSITION:
        UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"Transitioning");
        break;


      default:
        UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"Unknown(%d)", MeFs1.r.CurrentState);
        break;
    }
    HiiSetString (HiiAdvancedHandle, STRING_TOKEN (STR_ME_FWCURSTE_VALUE), StrBuf, NULL);

    //
    // Update ME Error Code in setup strings database
    //
    switch (MeFs1.r.ErrorCode) {
      case ME_ERROR_CODE_NO_ERROR:
        UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"No Error");
        break;

      case ME_ERROR_CODE_UNKNOWN:
        UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"Uncategorized Failure");
        break;

      case ME_ERROR_CODE_DISABLED:
        UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"Disabled");
        break;

      case ME_ERROR_CODE_IMAGE_FAILURE:
        UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"Image Failure");
        break;

      default:
        UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"Unknown(%d)", MeFs1.r.ErrorCode);
        break;
    }
    HiiSetString (HiiAdvancedHandle, STRING_TOKEN (STR_ME_FWERRCODE_VALUE), StrBuf, NULL);

    //
    // Update ME FW Type in setup strings database
    //
    MeTypeString = HiiGetString (HiiAdvancedHandle, STRING_TOKEN (STR_ME_FW_TYPE_SPS), NULL);
    if (MeTypeString == NULL) {
      DEBUG ((EFI_D_ERROR, "[ME] ERROR: HiiGetString has returned NULL pointer\n"));
      ASSERT (MeTypeString != NULL);
      return;
    }
    HiiSetString (HiiAdvancedHandle, STRING_TOKEN (STR_ME_FW_TYPE_VALUE), MeTypeString, NULL);
    FreePool (MeTypeString);

    switch (MeFs1.r.CurrentState) {
      case ME_STATE_RECOVERY:
        //
        // Update Recovery Cause in setup strings database
        //
        switch (MeFs2.Bits.RecoveryCause) {
          case MEFS2_RCAUSE_RCVJMPR:
            UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"ME Recovery Jumper");
            break;
          case MEFS2_RCAUSE_MFGJMPR:
            UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"Manufacturing Jumper");
            break;
          case MEFS2_RCAUSE_IPMICMD:
            UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"IPMI Command");
            break;
          case MEFS2_RCAUSE_FLASHCFG:
            UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"Flash Conf. Error");
            break;
          case MEFS2_RCAUSE_MEERROR:
            UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"ME Internal Error");
            break;
          case MEFS2_RCAUSE_UMAERROR:
            UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"UMA Memory Error");
            break;
          default:
            UnicodeSPrint (StrBuf, ME_SPS_INFO_MAX_STR_SIZE, L"Unknown(%d)", MeFs2.Bits.RecoveryCause);
            break;
        }
        HiiSetString (HiiAdvancedHandle, STRING_TOKEN (STR_ME_RCVCAUSE_VALUE), StrBuf, NULL);
        // Fall Through
      case ME_STATE_NORMAL:     // Fall Through
      case ME_STATE_TRANSITION:
        break;

      default:
        //
        // Do not send any HECI requests, no chance ME responds.
        //
        DEBUG((DEBUG_INFO, "[ME] Do not send any HECI requests, no chance ME responds\n"));
        return;
    }

    //
    // Update the ME firmware version info in BIOS Setup strings database
    //
    Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, &pHeciProtocol);
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "[ME] ERROR: Cannot locate HECI protocol (status: %r)\n", Status));
      return;
    }

    //
    // Send MKHI_GET_FW_VERSION request to ME
    //
    DEBUG((DEBUG_INFO, "[ME] Sending MKHI_GET_FW_VERSION request to ME\n"));

    gBS->SetMem (&HeciMsg.FwVerReq, sizeof (HeciMsg.FwVerReq), 0);
    gBS->SetMem (&HeciMsg.FwVerRsp, sizeof (HeciMsg.FwVerRsp), 0);
    HeciMsg.FwVerReq.Mkhi.Fields.GroupId = MKHI_GRP_GEN;
    HeciMsg.FwVerReq.Mkhi.Fields.Command = MKHI_CMD_GET_FW_VERSION;
    RspLen = sizeof (HeciMsg.FwVerRsp);

    Status = pHeciProtocol->HeciSendAndReceive (
                              pHeciProtocol,
                              HECI_DEFAULT_PCH_ID,
                              NULL,
                              HECI_DEFAULT_RETRY_NUMBER,
                              SPS_CLIENTID_BIOS,
                              SPS_CLIENTID_ME_MKHI,
                              (UINT32*) &HeciMsg.FwVerReq,
                              sizeof (HeciMsg.FwVerReq),
                              (UINT32*) &HeciMsg.FwVerRsp,
                              &RspLen,
                              NULL,
                              NULL
                             );
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "[ME] ERROR: Cannot send MKHI_GET_FW_VERSION (status: %r)\n", Status));
    } else if (!((HeciMsg.FwVerRsp.Mkhi.Fields.IsResponse) &&
               (MKHI_GRP_GEN == HeciMsg.FwVerReq.Mkhi.Fields.GroupId) &&
               (MKHI_CMD_GET_FW_VERSION == HeciMsg.FwVerReq.Mkhi.Fields.Command))) {
      DEBUG ((EFI_D_ERROR, "[ME] ERROR: Invalid MKHI_GET_FW_VERSION response (MKHI: 0x%X)\n", HeciMsg.FwVerRsp.Mkhi.Data));
    } else {
      DEBUG ((
        DEBUG_INFO,
        "[ME] Firmware version is:\n"
        "\tOperational: %02X:%d.%d.%d.%d\n"
        "\tRecovery:    %02X:%d.%d.%d.%d\n",
        (HeciMsg.FwVerRsp.Act.Major >> 8),
        (HeciMsg.FwVerRsp.Act.Major & 0x00FF),
        HeciMsg.FwVerRsp.Act.Minor,
        HeciMsg.FwVerRsp.Act.Patch,
        HeciMsg.FwVerRsp.Act.Build,
        (HeciMsg.FwVerRsp.Rcv.Major >> 8),
        (HeciMsg.FwVerRsp.Rcv.Major & 0x00FF),
        HeciMsg.FwVerRsp.Rcv.Minor,
        HeciMsg.FwVerRsp.Rcv.Patch,
        HeciMsg.FwVerRsp.Rcv.Build
        ));

      UnicodeSPrint (
        StrBuf,
        ME_SPS_INFO_MAX_STR_SIZE,
        L"%02X:%d.%d.%d.%d",
        (HeciMsg.FwVerRsp.Act.Major >> 8),
        (HeciMsg.FwVerRsp.Act.Major & 0x00FF),
        HeciMsg.FwVerRsp.Act.Minor,
        HeciMsg.FwVerRsp.Act.Patch,
        HeciMsg.FwVerRsp.Act.Build
        );

      HiiSetString (HiiAdvancedHandle, STRING_TOKEN (STR_ME_FWVER_OPERATIONAL_VALUE), StrBuf, NULL);

      //
      // Update ME firmware info in platform information menu
      //
      // APTIOV_SERVER_OVERRIDE_RC_START : Form not provided in AptioV
      //HiiSetString(HiiMainHandle, STRING_TOKEN(STR_ME_REVISION_VALUE), StrBuf, NULL);
      // APTIOV_SERVER_OVERRIDE_RC_END : Form not provided in AptioV

      UnicodeSPrint  (
        StrBuf,
        ME_SPS_INFO_MAX_STR_SIZE,
        L"%02X:%d.%d.%d.%d",
        (HeciMsg.FwVerRsp.Rcv.Major >> 8),
        (HeciMsg.FwVerRsp.Rcv.Major & 0x00FF),
        HeciMsg.FwVerRsp.Rcv.Minor,
        HeciMsg.FwVerRsp.Rcv.Patch,
        HeciMsg.FwVerRsp.Rcv.Build
        );

      HiiSetString (HiiAdvancedHandle, STRING_TOKEN (STR_ME_FWVER_RECOVERY_VALUE), StrBuf, NULL);

      //
      // ME in dual-image configuration provides the version of the backup image yet
      //
      if (RspLen > sizeof (HeciMsg.FwVerRsp) - sizeof (HeciMsg.FwVerRsp.Bkp)) {
        DEBUG ((
          EFI_D_ERROR,
          "\tBackup:      %02X:%d.%d.%d.%d\n",
          (HeciMsg.FwVerRsp.Bkp.Major >> 8),
          (HeciMsg.FwVerRsp.Bkp.Major & 0x00FF),
          HeciMsg.FwVerRsp.Bkp.Minor,
          HeciMsg.FwVerRsp.Bkp.Patch,
          HeciMsg.FwVerRsp.Bkp.Build
          ));

        UnicodeSPrint (
          StrBuf,
          ME_SPS_INFO_MAX_STR_SIZE,
          L"%02X:%d.%d.%d.%d",
          (HeciMsg.FwVerRsp.Bkp.Major >> 8),
          (HeciMsg.FwVerRsp.Bkp.Major & 0x00FF),
          HeciMsg.FwVerRsp.Bkp.Minor,
          HeciMsg.FwVerRsp.Bkp.Patch,
          HeciMsg.FwVerRsp.Bkp.Build
          );

        HiiSetString (HiiAdvancedHandle, STRING_TOKEN (STR_ME_FWVER_BACKUP_VALUE), StrBuf, NULL);
      }
    }

    GuidHob = GetFirstGuidHob (&gSpsInfoHobGuid);

    if (GuidHob == NULL) {
      DEBUG ((DEBUG_ERROR, "[ME] ERROR: Cannot locate SpsInfo HOB\n"));
    } else {
      SpsInfo = GET_GUID_HOB_DATA (GuidHob);

      if (SpsInfo != NULL) {
        //
        // Fill the ME firmware SKU info in setup
        //
        MeInitSpsFeatureSetStrings (HiiAdvancedHandle, &(SpsInfo->FeatureSet));

        Status = GetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID*)&MeRcConfiguration);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "[ME] ERROR: Failed to read ME_RC_CONFIGURATION!  Status = %r\n", Status));
          ASSERT_EFI_ERROR (Status);
          return;
        }

        MeRcConfiguration.SpsPmbusProxySupported = (UINT8)SpsInfo->FeatureSet.Bits.PmBusProxy;

        Status = SetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID*)&MeRcConfiguration);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "[ME] ERROR: Failed to update ME_RC_CONFIGURATION!  Status = %r\n", Status));
          ASSERT_EFI_ERROR (Status);
          return;
        }

        //
        // Update the ME Fia Mux information in setup
        //
        FiaMuxUpdateSetupInformation (HiiAdvancedHandle, SpsInfo);
      }
    }
    //
    // Update the ME PSUs Status in setup
    //
    MeUpdateSpsPsuStatusInfoStrings (HiiAdvancedHandle, pHeciProtocol);
  }
}

/**
  This function extracts ME RC Configuration data on Setup enter

  @param[in out] MeRcConfiguration - ME FW RC Configuration Data

  @return EFI Status code
  @retval EFI_SUCCESS Operation completed successfully
**/
EFI_STATUS EFIAPI MeExtractSpsConfigOnSetupEnter (
  IN OUT ME_RC_CONFIGURATION *MeRcConfiguration
  )
{
  BOOLEAN             MePttState;
  UINT32              MeMode;
  EFI_HOB_GUID_TYPE   *GuidHob;
  SPS_INFO_HOB        *SpsInfo;

  DEBUG((DEBUG_INFO, "[ME] MeExtractSpsConfigOnSetupEnter called...\n"));

  if (NULL == MeRcConfiguration)
  {
    ASSERT(MeRcConfiguration != NULL);
    return EFI_INVALID_PARAMETER;
  }

  // Update MeFirmwareMode in setup to correct show/hide operations
  MeMode = ME_MODE_FAILED;
  MeBiosGetMeMode (&MeMode);
  MeRcConfiguration->MeFirmwareMode = (UINT8)MeMode;
  DEBUG((DEBUG_INFO, "[ME] MeFirmwareMode is set to %d\n", MeRcConfiguration->MeFirmwareMode));

  GuidHob = GetFirstGuidHob (&gSpsInfoHobGuid);

  MeRcConfiguration->MePttSupported = 0;
  MeRcConfiguration->MePttEnabled = 0;
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_ERROR, "[ME] ERROR: Cannot locate SpsInfo HOB\n"));
  } else {
    SpsInfo = GET_GUID_HOB_DATA (GuidHob);

    if ((SpsInfo != NULL) && (SpsInfo->FeatureSet.Bits.PTT)) {
      MePttState = FALSE;

      // Before EndOfPost changes in PTT are allowed only
      // check if enabled
      if (MeIsAfterEndOfPost ()) {
        DEBUG ((DEBUG_INFO, "[ME] PTT change is not supported after EndOfPost\n"));
      } else {
        PttHeciGetCapability (&MePttState);
      }
      MeRcConfiguration->MePttSupported = (UINT8)MePttState;

      MePttState = FALSE;
      if (MeRcConfiguration->MePttSupported)
      {
        PttHeciGetState(&MePttState);
      }
      MeRcConfiguration->MePttEnabled = (UINT8)MePttState;
    }
  }

  DEBUG((DEBUG_INFO, "[ME] PTT supported/enabled: %d/%d\n",
                     MeRcConfiguration->MePttSupported, MeRcConfiguration->MePttEnabled));

  return EFI_SUCCESS;
}


/**
  This function routes ME RC Configuration data

  @param[in out] MeRcConfiguration - ME FW RC Configuration Data
  @param[in out] pMeResetRequested - Indicates whether reset was requested or not

  @return EFI Status code
  @retval EFI_SUCCESS Operation completed successfully
**/
EFI_STATUS EFIAPI MeRouteSpsAdvancedConfig (
  IN OUT ME_RC_CONFIGURATION *MeRcConfiguration,
  IN OUT BOOLEAN             *pMeResetRequested
  )
{
  EFI_STATUS              Status = EFI_SUCCESS;
  SPS_DXE_CONFIG          *SpsPolicy = NULL;
  HECI_FWS_REGISTER       MeFs1;

  DEBUG ((DEBUG_INFO, "[ME] MeRouteSpsAdvancedConfig called...\n"));

  if ((MeRcConfiguration != NULL) && (pMeResetRequested != NULL)) {
    if (CompareMem (&MeRcConfigurationProcessed, MeRcConfiguration, sizeof (ME_RC_CONFIGURATION)) == 0) {
      // This set has been processed or there is no changes.
      return Status;
    }

    SpsPolicy = GetSpsDxeConfigBlock ();
    ASSERT (SpsPolicy != NULL);

    if (SpsPolicy != NULL) {
      // Update Policy for setting that can have matter in current boot
      // or have been stored in ME

      // Take carry about reset request - if necessary
      // *pMeResetRequested = TRUE;
    }

    if (MeRcConfiguration->MePttSupported) {
      DEBUG ((DEBUG_INFO, "[ME] Save to ME MeRcConfiguration->MePttEnabled: %d\n", MeRcConfiguration->MePttEnabled));
      PttHeciUpdateState (MeRcConfiguration->MePttEnabled);
    }

    MeFs1.ul = HeciPciRead32 (R_ME_HFS);
    switch (MeFs1.r.CurrentState) {
      case ME_STATE_RECOVERY:
      case ME_STATE_NORMAL:
      case ME_STATE_TRANSITION:
        break;

      default:
        //
        // Do not send any HECI requests, no chance ME responds.
        //
        DEBUG ((DEBUG_INFO, "[ME] Do not send any HECI requests, no chance ME responds\n"));
        *pMeResetRequested = TRUE;
    }

    if (MeRcConfiguration->DelayedAuthenticationMode != MeRcConfigurationProcessed.DelayedAuthenticationMode) {
      *pMeResetRequested = TRUE;
      ServerHeciSetDamState (MeRcConfiguration->DelayedAuthenticationMode);
    }

  } else {
    DEBUG ((DEBUG_ERROR, "[ME] Invalid parameters (MeRouteSpsAdvancedConfig)\n"));
    Status = EFI_INVALID_PARAMETER;
  }

  return Status;
}

#endif //ME_SPS_SUPPORT
