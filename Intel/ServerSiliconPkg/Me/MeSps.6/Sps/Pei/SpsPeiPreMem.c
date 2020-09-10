/** @file
  This driver manages the initial phase of SPS ME firmware support specified in
  SPS ME-BIOS Interface Specification.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2020 Intel Corporation. <BR>

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

#include <Base.h>
#include <PiPei.h>
#include <Uefi.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>

#include <Ppi/HeciControlPpi.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Ppi/SpsHwChangePpi.h>

#include <Guid/SpsInfoHobGuid.h>

#include <ConfigBlock/MeGetConfigBlock.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/MeUtilsLib.h>

#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>

#include <Library/MeEwlLib.h>

#include <Register/HeciRegs.h>
#include <MeAccess.h>
#include <Sps.h>
#include <Library/MeTypeLib.h>
#include <Library/SpsPeiLib.h>
#include <Ppi/MeUmaPpi.h>
#include <SpsPei.h>
#include <IndustryStandard/Pci22.h>
#include <DualBiosMsgs.h>
#include <SpsPeiCommon.h>
#include <CpuRegs.h>
#include <Register/Intel/Cpuid.h>
#include <Register/Intel/ArchitecturalMsr.h>
#include <Register/PchRegsPsf.h>

/*****************************************************************************
 * Local definitions
 */
CONST CHAR8  *SpsPrefix = "[SPS][PreMem]";

/*****************************************************************************
 * Local functions prototypes
 */
EFI_STATUS
SpsPeiStartPreMemTasks (
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  );

/*****************************************************************************
 * Variables
 */
static EFI_PEI_NOTIFY_DESCRIPTOR mSpsPeiStartPreMemTasksNofifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gSiPreMemPolicyPpiGuid,
    SpsPeiStartPreMemTasks
  }
};

#if ME_TESTMENU_FLAG
/**
  Procedure to send BiosUpdateWatchdogControl HECI Request

  @param[in]    Heci    Pointer to HECI driver
  @param[in]    Action  Action to send in request

  @return EFI_INVALID_PARAMETER Invalid parameter has been passed to the function
  @return EFI_UNSUPPORTED       Unexpected HECI response
  @return EFI_SUCCESS           Message has been sent successfully
**/
EFI_STATUS
HeciReq_WatchdogControl (
  IN HECI_CONTROL          *Heci,
  IN UINT8                 Action
  )
{
  UINT32     BufLen;
  EFI_STATUS Status;

  union {
    HECI_MSG_BIOS_UPDATE_WATCHDOG_CONTROL_REQ Req;
    HECI_MSG_BIOS_UPDATE_WATCHDOG_CONTROL_RSP Rsp;
  } HeciMsg;

  if (Heci == NULL) {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  // Initializing the HeciMsg
  ZeroMem (&HeciMsg, sizeof (HeciMsg));

  DEBUG ((DEBUG_INFO, "%a Sending SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_REQ\n", SpsPrefix));

  //construct Heci request
  HeciMsg.Req.Command = SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_REQ;
  HeciMsg.Req.ActionFlag = Action;

  BufLen = sizeof (HeciMsg.Rsp);

  Status = Heci->HeciSendAndReceive (
                   Heci,
                   HECI_DEFAULT_PCH_ID,
                   NULL,
                   HECI_DEFAULT_RETRY_NUMBER,
                   SPS_CLIENTID_BIOS,
                   SPS_CLIENTID_ME_SPS,
                   (UINT32*) &HeciMsg.Req,
                   sizeof (HeciMsg.Req),
                   (UINT32*) &HeciMsg.Rsp,
                   &BufLen,
                   NULL,
                   NULL
                   );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a ERROR: Cannot send SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_REQ (%r)\n",
      SpsPrefix,
      Status
      ));
  } else if (HeciMsg.Rsp.Command != SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_RSP) {
    DEBUG ((
      DEBUG_ERROR,
      "%a ERROR: SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL response failed\n",
      SpsPrefix
      ));
    Status = EFI_UNSUPPORTED;
  } else if (HeciMsg.Rsp.Status != HECI_RSP_STATUS_SUCCESS) {
    DEBUG ((
      DEBUG_ERROR,
      "%s ERROR: BIOS_UPDATE_WATCHDOG_CONTROL failed (status: %d)\n",
      SpsPrefix,
      HeciMsg.Rsp.Status
      ));
    Status = EFI_UNSUPPORTED;
  } else {
    //no error, status OK
    DEBUG ((DEBUG_INFO, "%a BIOS_UPDATE_WATCHDOG_CONTROL reports success\n", SpsPrefix));
  }

  return Status;
} // HeciReq_WatchdogControl ()
#endif // ME_TESTMENU_FLAG

/**
  Function reads ME Reset Counter from ME FS1

  @retval UINT       Read ME Reset Counter
**/
UINTN
GetMeResetCounter (
  VOID
  )
{
  HECI_FWS_REGISTER MeFs1;

  MeFs1.ul = HeciPciRead32 (R_ME_HFS);

  return MeFs1.r.MeRstCnt;
} // GetMeResetCounter ()

/**
  Function waits for ME Reset Counter change.

  @param[in] StartResetCounter Reset counter we wait to be changed
  @param[in] pSpsInitTimeout   Reset counter we wait to be changed

  @retval EFI_SUCCESS       The function completed successfully.
  @retval EFI_TIMEOUT       Timeout on ME transition
**/
EFI_STATUS
WaitMeResetCounterChange(
  IN     UINTN   StartResetCounter,
  IN     UINT32 *pSpsInitTimeout
  )
{
  UINT32 InitTimeout = SPS_INIT_TIMEOUT / STALL_1MS;
  if (pSpsInitTimeout == NULL) {
    pSpsInitTimeout = &InitTimeout;
  }

  while (StartResetCounter == GetMeResetCounter ()) {
    if (!*pSpsInitTimeout) {
      DEBUG ((DEBUG_ERROR, "%a ERROR: Timeout when waiting for ME Reset Counter Change\n", SpsPrefix));
      MeEwlCommunicationFailures (
        MeEwlSeverityWarning,
        ME_EWL_CHKP_MAJOR_PREMEM,
        ME_EWL_CHKP_PREMEM_MESTATE,
        ME_EWL_WARN_MAJOR_HECI,
        ME_EWL_WARN_HECI_MESSAGE,
        HECI1_DEVICE,           // HECI device
        0,                      // MeAddress
        EFI_TIMEOUT,            // Send Status
        EFI_TIMEOUT,            // Receive Status
        0,                      // Request
        0                       // Response
        );
      return EFI_TIMEOUT;
    }
    MicroSecondDelay(STALL_1MS);
    (*pSpsInitTimeout)--;
  }
  return EFI_SUCCESS;
} // WaitMeResetCounterChange ()

/**
  Function SPS meaningful state (Normal or Recovery)

  @param[in] WaitTimeout   Pointer to wait timeout

  @retval EFI_SUCCESS  ME is in Normal in Recovery state
  @retval EFI_TIMEOUT  ME is not in Normal or Recovery state
                       within defined timeout
**/

EFI_STATUS
WaitForMeMeaningfulState (
  IN UINT32        *SpsMeaningfulStateTimeout
  )
{
  HECI_FWS_REGISTER MeFs1;
  UINT32            SpsTimeout = SPS_INIT_TIMEOUT / STALL_1MS;

  DEBUG ((DEBUG_INFO, "%a Wait for ME Normal or Recovery state\n", SpsPrefix));
  if (SpsMeaningfulStateTimeout == NULL) {
    SpsMeaningfulStateTimeout = &SpsTimeout;
  }

  // Wait for ME meaningful state
  MeFs1.ul = HeciPciRead32 (R_ME_HFS);
  while ((MeFs1.r.CurrentState != ME_STATE_RECOVERY) &&
         (MeFs1.r.CurrentState != ME_STATE_NORMAL)) {
    if (!*SpsMeaningfulStateTimeout) {
      DEBUG ((DEBUG_ERROR, "%a ERROR: Timeout when waiting for ME meaningful state\n", SpsPrefix));
      MeEwlStateFailures (
        MeEwlSeverityFatal,
        ME_EWL_CHKP_MAJOR_PREMEM,
        ME_EWL_CHKP_PREMEM_MESTATE,
        ME_EWL_WARN_MAJOR_MEFS1,
        ME_EWL_WARN_MEFS1_CURSTATE
        );
      return EFI_TIMEOUT;
    }
    MicroSecondDelay(STALL_1MS);
    MeFs1.ul = HeciPciRead32 (R_ME_HFS);
    (*SpsMeaningfulStateTimeout)--;
  }

  DEBUG ((DEBUG_INFO, "%a Wait for ME state reaches ME State = %d\n",
         SpsPrefix,
         MeFs1.r.CurrentState));

  return EFI_SUCCESS;
} // WaitForMeMeaningfulState ()

/**
  Execute SPS pre DID reset

  @param[in] VOID

  @retval EFI_SUCCESS       The function completed successfully.
  @retval EFI_DEVICE_ERROR  No Heci device
  @retval EFI_TIMEOUT       Timeout on ME transition
**/
EFI_STATUS
SpsExecutePreDidReset (
  VOID
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  UINT32                        SpsInitTimeout = SPS_RESET_TIMEOUT / STALL_1MS;
  HECI_CONTROL                  *pHeciPpi;
  UINT32                        RspLen;
  UINTN                         ResetCounter;
  union {
    MKHI_MSG_HMRFPO_MERESET_REQ Req;
    MKHI_MSG_HMRFPO_MERESET_RSP Rsp;
  } HeciMsg;

  DEBUG ((DEBUG_WARN, "%a WARNING: Execute ME pre-DID reset\n", SpsPrefix));

  Status = PeiServicesLocatePpi (&gHeciControlPpiGuid, 0, NULL, (VOID**)&pHeciPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot locate HECI PPI (%r)\n", SpsPrefix, Status));
    ASSERT_EFI_ERROR (Status);
    return EFI_DEVICE_ERROR;
  }

  ResetCounter = GetMeResetCounter ();

  HeciMsg.Req.Mkhi.Data               = 0;
  HeciMsg.Req.Mkhi.Fields.Command     = HMRFPO_CMD_MERESET;
  HeciMsg.Req.Mkhi.Fields.IsResponse  = 0;
  HeciMsg.Req.Mkhi.Fields.GroupId     = MKHI_GRP_HMRFPO;
  HeciMsg.Req.Mkhi.Fields.Reserved    = 0;
  HeciMsg.Req.Mkhi.Fields.Result      = 0;
  HeciMsg.Req.Nonce                   = 0;

  RspLen = sizeof (HeciMsg.Rsp);
  Status = pHeciPpi->HeciSendAndReceive (
                       pHeciPpi,
                       HECI_DEFAULT_PCH_ID,
                       NULL,
                       HECI_DEFAULT_RETRY_NUMBER,
                       BIOS_FIXED_HOST_ADDR,
                       HECI_MKHI_MESSAGE_ADDR,
                       (UINT32*) &HeciMsg.Req,
                       sizeof (HeciMsg.Req),
                       (UINT32*) &HeciMsg.Rsp,
                       &RspLen,
                       NULL,
                       NULL
                       );
  if (!EFI_ERROR (Status)) {
    // check if reset has been accepted
    if ((HeciMsg.Rsp.Mkhi.Fields.Command == HMRFPO_CMD_MERESET) &&
        (HeciMsg.Rsp.Mkhi.Fields.GroupId == MKHI_GRP_HMRFPO) &&
        (HeciMsg.Rsp.Mkhi.Fields.IsResponse == 1) &&
        (HeciMsg.Rsp.Status == 0)) {
      // Wait for reset counter change
      Status = WaitMeResetCounterChange (ResetCounter, &SpsInitTimeout);
      if (!EFI_ERROR (Status)) {
        Status = WaitForMeMeaningfulState (&SpsInitTimeout);
        if (!EFI_ERROR (Status)) {
          DEBUG ((DEBUG_INFO, "%a Pre-DID reset has been executed successfully\n", SpsPrefix));
        } else {
          Status = EFI_TIMEOUT;
        }
      } else {
        DEBUG ((DEBUG_ERROR, "%a ERROR: Pre-DID reset, ME Reset Counter not changed (%r)\n", SpsPrefix, Status));
        MeEwlCommunicationFailures (
          MeEwlSeverityWarning,
          ME_EWL_CHKP_MAJOR_PREMEM,
          ME_EWL_CHKP_PREMEM_MESTATE,
          ME_EWL_WARN_MAJOR_HECI,
          ME_EWL_WARN_HECI_MESSAGE,
          HECI1_DEVICE,           // HECI device
          0,                      // MeAddress
          EFI_TIMEOUT,            // Send Status
          EFI_TIMEOUT,            // Receive Status
          0,                      // Request
          0                       // Response
          );
      }
    } else {
      DEBUG ((DEBUG_ERROR, "%a Pre-DID reset is not accepted by ME. Continue without reset (%r)\n", SpsPrefix, Status));
    }
  } else {
    DEBUG ((DEBUG_ERROR, "%a Pre-DID reset, no ACK. Continue without reset (%r)\n", SpsPrefix, Status));
    Status = EFI_SUCCESS;
  }

  return Status;
} // SpsExecutePreDidReset ()

/**
  SPS PEI PreMemory entry point.

  @param[in] FileHandle  PEIM file handle
  @param[in] PeiServices General purpose services available to every PEIM

  @retval EFI_SUCCESS     The function completed successfully.
  @retval EFI_UNSUPPORTED SPS is non functional
**/
EFI_STATUS
SpsPeiPreMemEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                    Status;
  SI_PREMEM_POLICY_PPI          *SiPreMemPolicy;

  DEBUG ((DEBUG_INFO, "%a %a called.\n", SpsPrefix, __FUNCTION__));

   //
   // Check if there is other than SPS ME on-board.
   //
  if (!MeTypeIsSps ()) {
    DEBUG ((DEBUG_INFO, "%a Other ME FW detected.\n", SpsPrefix));
    MeIsNonFunctional (SpsPrefix);
    return EFI_UNSUPPORTED;
  }

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID**) &SiPreMemPolicy
             );
  if (EFI_ERROR (Status)) {
    Status = PeiServicesNotifyPpi (&mSpsPeiStartPreMemTasksNofifyList[0]);
    DEBUG ((DEBUG_INFO, "%a Set pre-mem notification status %r\n", SpsPrefix, Status));
    ASSERT_EFI_ERROR (Status);
    Status = EFI_SUCCESS;
  } else {
    Status = SpsPeiStartPreMemTasks (PeiServices, NULL, NULL);
  }
  return Status;
} // SpsPeiPreMemEntryPoint ()

/**
  Execute SPS start pre-mem tasks

  @param[in] PeiServices      General purpose services available to every PEIM
  @param[in] NotifyDescriptor Notification descriptor
  @param[in] Ppi              Notification source PPI

  @retval EFI_SUCCESS     The function completed successfully.
  @retval EFI_UNSUPPORTED SPS is not supported
**/
EFI_STATUS
SpsPeiStartPreMemTasks (
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  )
{
  EFI_STATUS                    Status;
  SPS_PEI_CONFIG                *pSpsPolicy;
  HECI_FWS_REGISTER             MeFs1;
  SPS_MEFS2                     MeFs2;
  EFI_BOOT_MODE                 BootMode;
  BOOLEAN                       EnableSSC;

  DEBUG ((DEBUG_INFO, "%a %a called.\n", SpsPrefix, __FUNCTION__));

  //
  // Get Boot mode
  //
  BootMode = BOOT_WITH_DEFAULT_SETTINGS;
  Status = (*PeiServices)->GetBootMode(PeiServices, &BootMode);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot read boot mode (%r)\n", SpsPrefix, Status));
    BootMode = BOOT_WITH_DEFAULT_SETTINGS;
  }
  pSpsPolicy = GetSpsPreMemPeiConfigBlock (NULL);
  //
  // Verify if it is SPS firmware running in Recovery.
  // If yes, try reset SPS
  //
  DEBUG ((DEBUG_INFO, "%a Pre-DID reset ", SpsPrefix));
  if (BootMode == BOOT_ON_S3_RESUME ||
      pSpsPolicy == NULL ||
      !pSpsPolicy->PreDidMeResetEnabled) {
    DEBUG ((DEBUG_INFO, "is disabled\n"));
  } else {
    // [SPS] Pre-DID reset
    DEBUG ((DEBUG_INFO, "execution\n"));
    Status = WaitForMeMeaningfulState (NULL);
    if (EFI_ERROR (Status)) {
      goto MeNonFunctional;
    }
    MeFs1.ul = HeciPciRead32 (R_ME_HFS);
    if (MeFs1.r.CurrentState == ME_STATE_RECOVERY) {
      MeFs2.UInt32 = HeciPciRead32 (R_ME_HFS_2);
      if (MeFs2.Bits.RecoveryCause == MEFS2_RCAUSE_MEERROR) {
        DEBUG ((DEBUG_WARN, "%a WARNING: ME is in recovery mode (cause: %d)\n", SpsPrefix, MeFs2.Bits.RecoveryCause));
        Status = SpsExecutePreDidReset ();
        MeFs1.ul = HeciPciRead32 (R_ME_HFS);
        switch (Status) {
          case EFI_TIMEOUT:
            DEBUG ((DEBUG_ERROR, "%a ERROR: Pre-DID reset timeout failure causes ME non-functional flow\n", SpsPrefix));
            MeEwlCommunicationFailures (
              MeEwlSeverityWarning,
              ME_EWL_CHKP_MAJOR_PREMEM,
              ME_EWL_CHKP_PREMEM_MESTATE,
              ME_EWL_WARN_MAJOR_HECI,
              ME_EWL_WARN_HECI_MESSAGE,
              HECI1_DEVICE,           // HECI device
              0,                      // MeAddress
              EFI_TIMEOUT,            // Send Status
              EFI_TIMEOUT,            // Receive Status
              0,                      // Request
              0                       // Response
              );
            goto MeNonFunctional;
          case EFI_SUCCESS:
            if (MeFs1.r.CurrentState == ME_STATE_NORMAL) {
              DEBUG ((DEBUG_INFO, "%a Pre-DID reset finished successfully, ME in Normal state\n", SpsPrefix));
            }
          break;
          default:
            DEBUG ((DEBUG_ERROR, "%a ERROR: Pre-DID reset failure. Continue according to ME state\n", SpsPrefix));
            MeEwlCommunicationFailures (
              MeEwlSeverityWarning,
              ME_EWL_CHKP_MAJOR_PREMEM,
              ME_EWL_CHKP_PREMEM_MESTATE,
              ME_EWL_WARN_MAJOR_HECI,
              ME_EWL_WARN_HECI_MESSAGE,
              HECI1_DEVICE,           // HECI device
              0,                      // MeAddress
              EFI_TIMEOUT,            // Send Status
              EFI_TIMEOUT,            // Receive Status
              0,                      // Request
              0                       // Response
              );
          break;
        }
      }
    }
  }

#if ME_TESTMENU_FLAG
  // Disable Dual BIOS watchdog
  if ((pSpsPolicy != NULL) && (pSpsPolicy->Common.MeWatchdogDisable != 0)) {
    HECI_CONTROL    *Heci;

    Status = PeiServicesLocatePpi (&gHeciControlPpiGuid, 0, NULL, &Heci);
    if (!EFI_ERROR (Status)) {
      // Send Disable Watchdog message
      Status = HeciReq_WatchdogControl (Heci, BIOS_UPDATE_WATCHDOG_CTRL_FLAG_STOP);
    }
    ASSERT_EFI_ERROR (Status);
  }
#endif // ME_TESTMENU_FLAG

  if (BootMode != BOOT_ON_S3_RESUME) {
    if (!IsEbgPch ()) {
      if (SpsIsIccSupported ()) {
        DEBUG ((DEBUG_INFO, "%a[ICC] Sending IccSetClockSettings request\n", SpsPrefix));

        if (pSpsPolicy != NULL) {
          EnableSSC = (BOOLEAN)pSpsPolicy->SpsIccClkSscSetting;
        } else {
          EnableSSC = TRUE;
        }

        Status = PeiHeciSetSSCAlternate (EnableSSC);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "%a ERROR: Unable to set clock settings (%r)\n", SpsPrefix, Status));
        } else {
          DEBUG ((DEBUG_INFO, "%a[ICC] IccSetClockSettings: status: Success\n", SpsPrefix));
        }
      } else {
        DEBUG ((DEBUG_INFO, "%a[ICC] IccSetClockSettings request wasn't sent\n", SpsPrefix));
      }
    }
  }

  return EFI_SUCCESS;

MeNonFunctional:
  MeIsNonFunctional (SpsPrefix);

  return EFI_UNSUPPORTED;
} // SpsPeiStartPreMemTasks ()
