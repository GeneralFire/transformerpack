/** @file
  SpsPeiLib.c

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

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MeTypeLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Ppi/HeciControlPpi.h>
#include <Library/PchInfoLib.h>
#include <CoreBiosMsg.h>
#include <IccMsgs.h>
#include <Library/SpsPeiLib.h>
#include <Library/MeUtilsLib.h>
#include <Sps.h>
#include <Guid/SpsInfoHobGuid.h>

/**
  Read FeatureSet information from SPS

  @param[in] SpsFeatureSet Pointer to FeatureSet structure

  @retval TRUE             Information has bee successfully read from SPS
                           and it is present in pointed structure
  @retval FALSE            Information is not available
**/
BOOLEAN
SpsGetFeatureSet (
  SPS_FEATURE_SET    *SpsFeatureSet
  )
{
  EFI_STATUS                        Status;
  HECI_CONTROL                      *HeciPpi;
  UINT32                            RspLen;
  SPS_MSG_GET_MEBIOS_INTERFACE_REQ  MeBiosVerReq;
  SPS_MSG_GET_MEBIOS_INTERFACE_RSP  MeBiosVerRsp;
  BOOLEAN                           ReturnStatus;

  if (SpsFeatureSet == NULL) {
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "[SPS] %a \n", __FUNCTION__));

  ZeroMem (&MeBiosVerReq, sizeof (MeBiosVerReq));
  ZeroMem (&MeBiosVerRsp, sizeof (MeBiosVerRsp));

  Status = PeiServicesLocatePpi (
             &gHeciControlPpiGuid,
             0,
             NULL,
             &HeciPpi
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] %a: Cannot locate HECI PPI\n", __FUNCTION__));
    return FALSE;
  }

  ReturnStatus = FALSE;
  MeBiosVerReq.Command.Data = SPS_CMD_GET_MEBIOS_INTERFACE_REQ;

  RspLen  = sizeof (SPS_MSG_GET_MEBIOS_INTERFACE_RSP);

  Status = HeciPpi->HeciSendAndReceive (
                      HeciPpi,
                      HECI_DEFAULT_PCH_ID,
                      NULL,
                      HECI_DEFAULT_RETRY_NUMBER,
                      SPS_CLIENTID_BIOS,
                      SPS_CLIENTID_ME_SPS,
                      (UINT32*) &MeBiosVerReq,
                      sizeof (MeBiosVerReq),
                      (UINT32*) &MeBiosVerRsp,
                      &RspLen,
                      NULL,
                      NULL
                      );

  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "[SPS] %a: Communication problem (status: %r)\n",
      __FUNCTION__,
      Status
      ));
  } else if (MeBiosVerRsp.Command.Data != SPS_CMD_GET_MEBIOS_INTERFACE_RSP ||
            (RspLen != sizeof (MeBiosVerRsp))) {
    DEBUG ((
      DEBUG_ERROR,
      "[SPS] %a: Invalid response (cmd: 0x%X, len %d)\n",
      __FUNCTION__,
      MeBiosVerRsp.Command,
      RspLen
      ));
  } else {
    SpsFeatureSet->Data.Set1 = MeBiosVerRsp.FeatureSet;
    SpsFeatureSet->Data.Set2 = MeBiosVerRsp.FeatureSet2;
    ReturnStatus = TRUE;
  }

  return ReturnStatus;
}

/**
  Return information if SPS supports ICC feature

  The function sends SPS_CMD_GET_MEBIOS_INTERFACE message to SPS FW and basing on
  the received message determines if ICC is supported or not.

  @retval TRUE   ICC is supported by SPS
  @retval FALSE  ICC isn't supported by SPS or a HECI communication error occurred

**/
BOOLEAN
SpsIsIccSupported (
  VOID
  )
{
  BOOLEAN                           IsIccSupported;
  SPS_FEATURE_SET                   SpsFeatureSet;

  DEBUG ((DEBUG_INFO, "[SPS] %a \n", __FUNCTION__));

  IsIccSupported = FALSE;

  if (SpsGetFeatureSet (&SpsFeatureSet)) {
    if (SpsFeatureSet.Bits.ICC) {
      IsIccSupported = TRUE;
    }
  }

  DEBUG ((DEBUG_INFO, "[SPS] %a, IccSupported: %d\n", __FUNCTION__, IsIccSupported));

  return IsIccSupported;
}

/**
  Return information if SPS supports FiaMux feature

  The function sends SPS_CMD_GET_MEBIOS_INTERFACE message to SPS FW and basing on
  the received message determines if FiaMux is supported or not.

  @retval TRUE   FiaMux is supported by SPS
  @retval FALSE  FiaMux isn't supported by SPS or a HECI communication error occurred

**/
BOOLEAN
SpsIsFiaMuxSupported (
  VOID
  )
{
  BOOLEAN                           IsFiaMuxSupported;
  SPS_FEATURE_SET                   SpsFeatureSet;

  DEBUG ((DEBUG_INFO, "[SPS] %a \n", __FUNCTION__));

  IsFiaMuxSupported = FALSE;

  if (SpsGetFeatureSet (&SpsFeatureSet)) {
    if (SpsFeatureSet.Bits.FiaMuxConfiguration) {
      IsFiaMuxSupported = TRUE;
    }
  }

  DEBUG ((DEBUG_INFO, "[SPS] %a, FiaMuxSupported: %d\n", __FUNCTION__, IsFiaMuxSupported));

  return IsFiaMuxSupported;
}

EFI_STATUS
SpsSetGetCurrenClockingMode (
  IN     UINT8                             Command,
  IN     ICC_GETSET_CLOCK_SETTINGS_REQRSP  *Request,
  IN     ICC_GETSET_CLOCK_SETTINGS_REQRSP  *Response
  )
{
  EFI_STATUS                    Status = EFI_UNSUPPORTED;
#if ME_SPS_SUPPORT
  HECI_CONTROL                  *HeciPpi;
  UINT32                        CommandSize;
  UINT32                        ResponseSize;

  DEBUG ((DEBUG_INFO, "[ICC] %a \n", __FUNCTION__));

  if ((Request == NULL) || (Response == NULL)) {
    Status = EFI_INVALID_PARAMETER;
  } else if (MeTypeIsSps () && !MeSpsInRecovery () && SpsIsIccSupported ()) {
    Status = PeiServicesLocatePpi (
               &gHeciControlPpiGuid,     // GUID
               0,                        // INSTANCE
               NULL,                     // EFI_PEI_PPI_DESCRIPTOR
               (VOID **) &HeciPpi        // PPI
               );
    if (!EFI_ERROR (Status)) {
      CommandSize                        = sizeof (ICC_GETSET_CLOCK_SETTINGS_REQRSP);
      ResponseSize                       = sizeof (ICC_GETSET_CLOCK_SETTINGS_REQRSP);

      Request->Header.ApiVersion         = ICC_API_VERSION;
      Request->Header.IccCommand         = Command;
      Request->Header.IccResponse        = 0;
      Request->Header.BufferLength       = CommandSize - sizeof (ICC_HEADER);
      Request->Header.Reserved           = 0;
      Request->ClockSettings.ReqClock    = REQUESTED_CLOCK_GEN2_PCIE;
      Request->ClockSettings.SettingType = 0;

      Status = HeciPpi->HeciSendAndReceive (
                          HeciPpi,
                          HECI_DEFAULT_PCH_ID,
                          NULL,
                          HECI_DEFAULT_RETRY_NUMBER,
                          BIOS_FIXED_HOST_ADDR,
                          HECI_ICC_MESSAGE_ADDR,
                          (UINT32*) Request,
                          CommandSize,
                          (UINT32*) Response,
                          &ResponseSize,
                          NULL,
                          NULL
                          );

      if (!EFI_ERROR (Status)) {
        if (Response->Header.IccCommand != Command) {
          Status = EFI_DEVICE_ERROR;
          DEBUG ((
            DEBUG_ERROR,
            "(ICC) %a: Wrong response! Header.IccCommand = 0x%x\n",
            __FUNCTION__,
            Response->Header.IccCommand
            ));
        }
        if (Response->Header.IccResponse != ICC_STATUS_SUCCESS) {
          DEBUG ((
            DEBUG_ERROR,
            "(ICC) %a: Wrong response! Header.IccResponse = 0x%x\n",
            __FUNCTION__,
            Response->Header.IccResponse
            ));
          Status = EFI_DEVICE_ERROR;
        }
      }
    } else {
        DEBUG ((
          DEBUG_ERROR,
          "(ICC) %a: Can't locate HECI PPI\n",
          __FUNCTION__
          ));
      Status = EFI_DEVICE_ERROR;
    }
  }
#endif // ME_SPS_SUPPORT
  DEBUG ((
    DEBUG_INFO,
    "[ICC] %a exit status = %r \n",
    __FUNCTION__,
    Status
    ));

  return Status;
}

/**
  Get current clocking mode via HECI.
  In case non SPS ME on board read it from soft straps

  @param[out] ClockingMode        Clocking mode read

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_INVALID_PARAMETER   Invalid pointer
  @retval EFI_NOT_READY           Function called before DID
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_UNSUPPORTED         Unsupported Clocking Mode has been returned
**/
EFI_STATUS
PeiGetCurrenClockingMode (
   OUT CLOCKING_MODES *ClockingMode
  )
{
  EFI_STATUS                        Status = EFI_INVALID_PARAMETER;
  ICC_GETSET_CLOCK_SETTINGS_REQRSP  Request;
  ICC_GETSET_CLOCK_SETTINGS_REQRSP  Response;
  CONST EFI_PEI_SERVICES            **PeiServices = GetPeiServicesTablePointer ();

  DEBUG ((DEBUG_INFO, "[ME] %a \n", __FUNCTION__));
  if (ClockingMode != NULL) {
    (*PeiServices)->SetMem (&Request, sizeof (ICC_GET_CLOCK_SETTINGS_BUFFER), 0);
    (*PeiServices)->SetMem (&Response, sizeof (ICC_GET_CLOCK_SETTINGS_BUFFER), 0);
#if ME_SPS_SUPPORT
    if (MeTypeIsSps () && !MeSpsInRecovery ()) {
      DEBUG ((DEBUG_INFO, "[ME] %a: For SPS ME use Heci message\n", __FUNCTION__));
      Status = SpsSetGetCurrenClockingMode (
                 ICC_GET_CLOCK_SETTINGS_CMD,
                 &Request,
                 &Response
                 );
    } else {
      DEBUG ((DEBUG_INFO, "[ME] %a: Non SPS ME detected\n", __FUNCTION__));
      Status = EFI_NOT_READY;
    }
#else
    Status = EFI_NOT_READY;
#endif // ME_SPS_SUPPORT

    // Msg.Response.ClockMode is not in use on Mehlow
    // return default
    *ClockingMode = InternalAlternate;

  }

  DEBUG ((DEBUG_INFO, "[ME] %a exit status = %r \n", __FUNCTION__, Status));

  return Status;
}

/**
  Function sets SSC on/off

  @param[in] UseAlternate         Set Alternate SSC
                                  FALSE: turn off SSC for BCLK PLL
                                  TRUE:  turn on SSC 0.5% downspread for BCLK PLL

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_INVALID_PARAMETER   Invalid pointer
  @retval EFI_NOT_READY           Function called before DID
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_UNSUPPORTED         Unsupported Clocking Mode has been returned
**/
EFI_STATUS
PeiHeciSetSSCAlternate (
   IN BOOLEAN UseAlternate
  )
{
  EFI_STATUS                        Status = EFI_UNSUPPORTED;
#if ME_SPS_SUPPORT
  ICC_GETSET_CLOCK_SETTINGS_REQRSP  Request;
  ICC_GETSET_CLOCK_SETTINGS_REQRSP  Response;
  CONST EFI_PEI_SERVICES            **PeiServices = GetPeiServicesTablePointer ();

  Status = EFI_INVALID_PARAMETER;
  DEBUG ((DEBUG_INFO, "[ME] PeiHeciSetSSCAlternate(%d)\n", UseAlternate));
  (*PeiServices)->SetMem (&Request, sizeof (ICC_GET_CLOCK_SETTINGS_BUFFER), 0);
  (*PeiServices)->SetMem (&Response, sizeof (ICC_GET_CLOCK_SETTINGS_BUFFER), 0);

  if (UseAlternate) {
    Request.ClockSettings.SscPercent = 50; // turn on SSC for BCLK PLL
  } else {
    Request.ClockSettings.SscPercent = 0;  // turn off SSC for BCLK PLL
  }

  Request.ClockSettings.UserFreq                = 100000000;
  Request.ClockSettings.CurrentFlags.Flags.Type = ICC_PROGRAMMING_TYPE_DYNAMIC;

  DEBUG ((DEBUG_INFO, "[ME] PeiHeciSetSSCAlternate sets SSC percentage to %d\n",
                       Request.ClockSettings.SscPercent));
  if (!IsSimicsEnvironment ()) {
    Status = SpsSetGetCurrenClockingMode (
               ICC_SET_CLOCK_SETTINGS_CMD,
               &Request,
               &Response
               );
  } else {
    Status = EFI_SUCCESS;
  }
#endif // ME_SPS_SUPPORT

  DEBUG ((DEBUG_INFO, "[ME] PeiHeciSetSSCAlternate exit status = %r \n",
                      Status));

  return Status;
}
