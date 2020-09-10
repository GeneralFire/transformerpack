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
#include <Library/PeiFiaMuxConfigInitLib.h>

#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>

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
CONST CHAR8  *SpsPrefix = "[SPS][PostMem]";

/*****************************************************************************
 * Local functions prototypes
 */
EFI_STATUS SpsNonS3Path(IN CONST EFI_PEI_SERVICES**, IN EFI_PEI_NOTIFY_DESCRIPTOR*, IN VOID*);
EFI_STATUS SpsS3Path(IN CONST EFI_PEI_SERVICES**, IN EFI_PEI_NOTIFY_DESCRIPTOR*, IN VOID*);
EFI_STATUS SpsHwChangeSetStatus(SPS_HW_CHANGE_PPI*, BOOLEAN, BOOLEAN);

/*****************************************************************************
 * Variables
 */
static EFI_PEI_NOTIFY_DESCRIPTOR  mSpsPeiNonS3NotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gDramInitDoneSentPpiGuid,
    SpsNonS3Path
  }
};
static EFI_PEI_NOTIFY_DESCRIPTOR  mSpsPeiS3NotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gDramInitDoneSentPpiGuid,
    SpsS3Path
  }
};

static SPS_HW_CHANGE_PPI mSpsHwChangePpi = {
  SpsHwChangeSetStatus
};

static EFI_PEI_PPI_DESCRIPTOR mInstallSpsHwChangePpi = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gSpsHwChangePpiGuid,
  &mSpsHwChangePpi
};


/**
  Waits For SPS ME FW initialization

  @param[in] pSpsPolicy    SPS configuration

  @retval EFI_SUCCESS      Success
  @retval EFI_UNSUPPORTED  Failure, go to non-functional flow
**/
EFI_STATUS
WaitMeFwInitialization (
  SPS_PEI_CONFIG  *pSpsPolicy
  )
{
  UINT32                        SpsInitTimeout;
  HECI_FWS_REGISTER             MeFs1;
  SPS_MEFS2                     MeFs2;

  SpsInitTimeout = SPS_INIT_TIMEOUT / STALL_1MS;
#if ME_TESTMENU_FLAG
  if (pSpsPolicy != NULL) {
    SpsInitTimeout = pSpsPolicy->MeTimeout * (STALL_1S / STALL_1MS);
  }
#endif

  //
  // Wait for ME init complete. Timeout is 2 seconds.
  //
  DEBUG ((DEBUG_INFO, "%a Waiting for ME firmware init complete\n", SpsPrefix));
  MeFs1.ul = HeciPciRead32 (R_ME_HFS);
  while (!MeFs1.r.FwInitComplete) {
    if (!SpsInitTimeout) {
      DEBUG ((DEBUG_ERROR, "%a ERROR: Timeout when waiting for ME init complete\n", SpsPrefix));
      MeEwlStateFailures (
        MeEwlSeverityFatal,
        ME_EWL_CHKP_MAJOR_PREINIT,
        ME_EWL_CHKP_PREINIT_MESTATE,
        ME_EWL_WARN_MAJOR_MEFS1,
        ME_EWL_WARN_MEFS1_INCOMPLETE
        );
      return EFI_UNSUPPORTED;
    }
    MicroSecondDelay (STALL_1MS);
    MeFs1.ul = HeciPciRead32 (R_ME_HFS);
    SpsInitTimeout--;
  }

  switch (MeFs1.r.CurrentState) {
    case ME_STATE_RECOVERY:
      MeFs2.UInt32 = HeciPciRead32 (R_ME_HFS_2);
      DEBUG ((DEBUG_WARN, "%a WARNING: ME is in recovery mode (cause: %d)\n",
                         SpsPrefix, MeFs2.Bits.RecoveryCause));
      MeEwlStateFailures (
        MeEwlSeverityWarning,
        ME_EWL_CHKP_MAJOR_POSTINIT,
        ME_EWL_CHKP_PREMEM_MESTATE,
        ME_EWL_WARN_MAJOR_MEFS1,
        ME_EWL_WARN_MEFS1_CURSTATE
        );
      // Fall through to normal case
    case ME_STATE_NORMAL:
      break;

    default:
      return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;

} // WaitMeFwInitialization ()

/**
  Set boot mode requested by NM in ME.

  @param[in] PowerOptBoot - Power vs performance optimized boot is requested

  @retval EFI_SUCCESS   Always success
**/
EFI_STATUS
NmSetBootMode (
  BOOLEAN PerfOptBoot
  )
{
  CPUID_VERSION_INFO_ECX  Ecx;
  union
  {
    UINT64 QWord;
    struct
    {
      UINT32 DWordL;
      UINT32 DWordH;
    } Bits;
  } PlatformInfo, PerfCtrl;

  //
  // Check if P-states control (EIST) is supported in processor.
  //
  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &Ecx.Uint32, NULL);
  if (!(Ecx.Bits.EIST)) {
    return EFI_UNSUPPORTED;
  }

  //
  // In Platform Info MSR we got minimum and maximum ratio for processor.
  //
  PlatformInfo.QWord = AsmReadMsr64 (MSR_PLATFORM_INFO);
  //
  // Read current Performance Control MSR, clear ratio bits
  //
  PerfCtrl.QWord = AsmReadMsr64 (MSR_IA32_PERF_CTL) & ~0x7F00;

  if (PerfOptBoot) {
    //
    // Set max ratio in Performance Control MSR
    //
    PerfCtrl.Bits.DWordL |= PlatformInfo.Bits.DWordL & 0x7F00;
    DEBUG ((DEBUG_INFO, "%a NM Boot Mode: Performance Optimized\n", SpsPrefix));
  } else {
    //
    // Set min ratio in Performance Control MSR
    //
    PerfCtrl.Bits.DWordL |= PlatformInfo.Bits.DWordH & 0x7F00;
    DEBUG ((DEBUG_INFO, "%a NM Boot Mode: Power Optimized\n", SpsPrefix));
  }
  AsmWriteMsr64 (MSR_IA32_PERF_CTL, PerfCtrl.QWord);

  return EFI_SUCCESS;
} // NmSetBootMode ()

/**
  This function is called on S3 resume path. It sends END_OF_POST message to
  SPS firmware.

  @param[in] PeiServices      unused
  @param[in] NotifyDescriptor unused
  @param[in] Ppi              unused

  @retval EFI_SUCCESS             MeMode copied
  @retval EFI_INVALID_PARAMETER   Pointer of MeMode is invalid
**/
EFI_STATUS
SpsS3Path (
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  )
{
  EFI_STATUS                    Status;
  HECI_CONTROL                  *pHeciPpi;
  UINT32                        Len;
  SPS_PEI_CONFIG                *SpsPolicy;
  ME_PEI_CONFIG                 *MePolicy;
  UINT64                        Request;
  UINT32                        Response;
  EFI_STATUS                    SendStatus;
  EFI_STATUS                    ReceiveStatus;
  VOID                          *TestPpi;
  UINT32 MbarL = 0;
  UINT32 MbarH = 0;
  UINT8  Cmd = 0;
  UINT8  Irq = 0;

  union {
    MKHI_MSG_END_OF_POST_REQ EopReq;
    MKHI_MSG_END_OF_POST_RSP EopRsp;
  } HeciMsg;

  DEBUG ((DEBUG_INFO, "%a %a called.\n",SpsPrefix, __FUNCTION__));

  Status = PeiServicesLocatePpi (&gDramInitDoneSentPpiGuid, 0, NULL, &TestPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a %a DramInitDone has not been sent yet - waiting for next notification\n",SpsPrefix, __FUNCTION__));
    return EFI_SUCCESS;
  }

  Status = WaitMeFwInitialization (GetSpsPreMemPeiConfigBlock (NULL));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: ME not initialized correctly (%r)\n", SpsPrefix, Status));
    SpsHobCreate (SpsPrefix, ME_NONFUNCTIONAL, 0, 0, FALSE, 0);
    return Status;
  }

  Status = PeiServicesLocatePpi (&gHeciControlPpiGuid, 0, NULL, &pHeciPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot locate HECI PPI (%r)\n",SpsPrefix, Status));
    return Status;
  }

  MbarL = HeciPciRead32 (PCI_BASE_ADDRESSREG_OFFSET);
  MbarH = HeciPciRead32 (PCI_BASE_ADDRESSREG_OFFSET + 4);
  Cmd = HeciPciRead8 (PCI_COMMAND_OFFSET);
  Irq = HeciPciRead8 (PCI_INT_LINE_OFFSET);

  //
  // If HECI is already configured to 64-bit address force its reinitialization to 32-bit.
  // PEI works in 32-bit.
  //
  if (MbarH != 0 && (MbarL & 0x4)) {
    HeciPciWrite32 (PCI_BASE_ADDRESSREG_OFFSET, 0);
  }

  //
  // Send END_OF_POST notification if not desibled in ME debug options
  //
#if ME_TESTMENU_FLAG
  SpsPolicy = GetSpsPreMemPeiConfigBlock (NULL);
  MePolicy = GetMePeiConfigBlock (NULL);
#else
  SpsPolicy = NULL;
  MePolicy = NULL;
#endif
  if ((SpsPolicy == NULL) || (MePolicy == NULL) ||
      (MePolicy->EndOfPostMessage)) {
    DEBUG ((DEBUG_INFO, "%a Sending END_OF_POST to ME\n", SpsPrefix));
    HeciMsg.EopReq.Mkhi.Data = 0x00000CFF;
    Len = sizeof (HeciMsg.EopRsp);
    Request = *(UINT64*)&HeciMsg.EopReq;
    Status = pHeciPpi->HeciSendAndReceive (
                         pHeciPpi,
                         HECI_DEFAULT_PCH_ID,
                         NULL,
                         HECI_DEFAULT_RETRY_NUMBER,
                         SPS_CLIENTID_BIOS,
                         SPS_CLIENTID_ME_MKHI,
                         (UINT32*) &HeciMsg.EopReq,
                         sizeof (HeciMsg.EopReq),
                         (UINT32*) &HeciMsg.EopRsp,
                         &Len,
                         &SendStatus,
                         &ReceiveStatus
                         );
    Response = *(UINT32*)&HeciMsg.EopReq;
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot send END_OF_POST (%r)\n",SpsPrefix, Status));
      MeEwlCommunicationFailures (
        MeEwlSeverityFatal,
        ME_EWL_CHKP_MAJOR_POSTINIT,
        ME_EWL_CHKP_POSTINIT_EOP,
        ME_EWL_WARN_MAJOR_HECI,
        ME_EWL_WARN_HECI_MESSAGE,
        HECI1_DEVICE,           // HECI device
        SPS_CLIENTID_ME_MKHI,   // MeAddress
        SendStatus,             // Send Status
        ReceiveStatus,          // Receive Status
        Request,                // Request
        Response                // Response
        );
    } else if (HeciMsg.EopRsp.Mkhi.Data != 0x00008CFF) {
      DEBUG ((DEBUG_ERROR, "%a ERROR: Invalid END_OF_POST response (MKHI: 0x%X)\n",
                          SpsPrefix,
                          HeciMsg.EopRsp.Mkhi.Data));
      MeEwlCommunicationFailures (
        MeEwlSeverityFatal,
        ME_EWL_CHKP_MAJOR_POSTINIT,
        ME_EWL_CHKP_POSTINIT_EOP,
        ME_EWL_WARN_MAJOR_HECI,
        ME_EWL_WARN_HECI_MESSAGE,
        HECI1_DEVICE,           // HECI device
        SPS_CLIENTID_ME_MKHI,   // MeAddress
        SendStatus,             // Send Status
        ReceiveStatus,          // Receive Status
        Request,                // Request
        Response                // Response
        );
    }
  } // if (MeEndOfPostEnabled)
  //
  // Restore the registers that may have changed
  //
  HeciPciWrite32 (PCI_BASE_ADDRESSREG_OFFSET + 4, MbarH);
  HeciPciWrite32 (PCI_BASE_ADDRESSREG_OFFSET, MbarL);
  HeciPciWrite8 (PCI_COMMAND_OFFSET, Irq);
  HeciPciWrite8 (PCI_COMMAND_OFFSET, Cmd);

  return EFI_SUCCESS;
} // SpsS3Path ()

/**
  This Function will be called after MRC and HECI Protocol launched
  on non-S3 resume path.

  @param[in] PeiServices      unused
  @param[in] NotifyDescriptor unused
  @param[in] Ppi              unused

  @retval EFI_SUCCESS         SPS support is enabled
  @retval EFI_ABORTED         SPS is in non-functional flow
**/

EFI_STATUS
SpsNonS3Path (
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  )
{
  EFI_STATUS                    Status;
  HECI_FWS_REGISTER             MeFs1;
  SPS_NMFS                      NmFs;
  SPS_FEATURE_SET               SpsFeatureSet;
  HECI_CONTROL                  *pHeciPpi;
  UINTN                         Attempts;
  UINT32                        RspLen;
  SPS_PEI_CONFIG                *pSpsPolicy;
  UINT64                        Request;
  UINT32                        Response = 0;
  EFI_STATUS                    SendStatus = EFI_DEVICE_ERROR;
  EFI_STATUS                    ReceiveStatus = EFI_DEVICE_ERROR;
  VOID                          *TestPpi;

  union {
    SPS_MSG_GET_MEBIOS_INTERFACE_REQ MeBiosVerReq;
    SPS_MSG_GET_MEBIOS_INTERFACE_RSP MeBiosVerRsp;
  } HeciMsg;

  DEBUG ((DEBUG_INFO, "%a %a called.\n", SpsPrefix, __FUNCTION__));

  Status = PeiServicesLocatePpi (&gDramInitDoneSentPpiGuid, 0, NULL, &TestPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a %a DramInitDone has not been sent yet - waiting for next notification\n",SpsPrefix, __FUNCTION__));
    return EFI_SUCCESS;
  }

  pSpsPolicy = GetSpsPreMemPeiConfigBlock (NULL);
  if (pSpsPolicy == NULL) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot locate SPS Policy PPI, defaults will be used\n", SpsPrefix));
  }
  Status = WaitMeFwInitialization (pSpsPolicy);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: ME not initialized correctly.\n", SpsPrefix));
    SpsHobCreate (SpsPrefix, ME_NONFUNCTIONAL, 0, 0, FALSE, 0);

    return Status;
  }

  MeFs1.ul = HeciPciRead32 (R_ME_HFS);
  if (MeFs1.r.CurrentState == ME_STATE_NORMAL) {
    PeiFiaMuxConfigInit ();
  }

  Status = PeiServicesLocatePpi (&gHeciControlPpiGuid, 0, NULL, &pHeciPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot locate HECI PPI\n", SpsPrefix));
    return Status;
  }

  ZeroMem (&HeciMsg, sizeof(HeciMsg));
  DEBUG ((DEBUG_INFO, "%a Sending ME-BIOS Interface Version request\n", SpsPrefix));
  Attempts = 0;
  do {
    HeciMsg.MeBiosVerReq.Command.Data = SPS_CMD_GET_MEBIOS_INTERFACE_REQ;
    Request = *(UINT64*)&HeciMsg.MeBiosVerReq;
    Status = pHeciPpi->HeciSend (
                         pHeciPpi,
                         HECI_DEFAULT_PCH_ID,
                         NULL,
                         SPS_CLIENTID_BIOS,
                         SPS_CLIENTID_ME_SPS,
                         (UINT32*)&HeciMsg.MeBiosVerReq,
                         sizeof (HeciMsg.MeBiosVerReq)
                         );
    SendStatus = Status;
    Response = 0;
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a WARNING: Cannot send SPS_GET_MEBIOS_INTERFACE (status %r)\n",
                          SpsPrefix,
                          Status));
    } else {
      RspLen = sizeof (HeciMsg.MeBiosVerRsp);
      Status = pHeciPpi->HeciReceive (
                           pHeciPpi,
                           HECI_DEFAULT_PCH_ID,
                           NULL,
                           NULL,
                           NULL,
                           (UINT32*)&HeciMsg.MeBiosVerRsp,
                           &RspLen
                           );
      ReceiveStatus = Status;
      Response = *(UINT32*)&HeciMsg.MeBiosVerRsp;
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_WARN, "%a WARNING: Cannot get SPS_GET_MEBIOS_INTERFACE response "
                             "(status: %r)\n",SpsPrefix, Status));
      } else if (HeciMsg.MeBiosVerRsp.Command.Data != SPS_CMD_GET_MEBIOS_INTERFACE_RSP ||
                (RspLen != sizeof(HeciMsg.MeBiosVerRsp))) {
        DEBUG ((DEBUG_ERROR, "%a ERROR: Invalid GET_MEBIOS_INTERFACE response "
                            "(cmd: 0x%X, len %d)\n", SpsPrefix, HeciMsg.MeBiosVerRsp.Command, RspLen));
        Status = EFI_ABORTED;
      }
    }
    Attempts++;
  } while (EFI_ERROR (Status) && Attempts < SPS_MAX_NUMBER_OF_RESEND_MESSAGE);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Error sending SPS_GET_MEBIOS_INTERFACE message (Status = %r)\n",
                        SpsPrefix, Status));
    MeEwlCommunicationFailures (
      MeEwlSeverityFatal,
      ME_EWL_CHKP_MAJOR_POSTINIT,
      ME_EWL_CHKP_POSTINIT_COMPATIB,
      ME_EWL_WARN_MAJOR_HECI,
      ME_EWL_WARN_HECI_MESSAGE,
      HECI1_DEVICE,           // HECI device
      SPS_CLIENTID_ME_SPS,    // MeAddress
      SendStatus,             // Send Status
      ReceiveStatus,          // Receive Status
      Request,                // Request
      Response                // Response
      );
    SpsHobCreate(SpsPrefix, ME_NONFUNCTIONAL, 0, 0, FALSE, 0);
    return EFI_ABORTED;

  } else if ((MeFs1.r.CurrentState == ME_STATE_RECOVERY) ?
              !SpsMeBiosRcvVersionVerify(HeciMsg.MeBiosVerRsp.VerMajor, HeciMsg.MeBiosVerRsp.VerMinor) :
              !SpsMeBiosOprVersionVerify(HeciMsg.MeBiosVerRsp.VerMajor, HeciMsg.MeBiosVerRsp.VerMinor)) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Incompatible SPS ME-BIOS interface definition %d.%d\n",
                        SpsPrefix, HeciMsg.MeBiosVerRsp.VerMajor, HeciMsg.MeBiosVerRsp.VerMinor));
    SpsHobCreate (SpsPrefix, ME_NONFUNCTIONAL, 0, 0, FALSE, 0);
    return EFI_ABORTED;
  }
  DEBUG ((DEBUG_INFO, "%a SPS ME-BIOS interface version is %d.%d\n",
         SpsPrefix,
         HeciMsg.MeBiosVerRsp.VerMajor, HeciMsg.MeBiosVerRsp.VerMinor));
  DEBUG ((DEBUG_INFO, "      Feature set is   0x%08X\n", HeciMsg.MeBiosVerRsp.FeatureSet));
  DEBUG ((DEBUG_INFO, "      Feature set 2 is 0x%08X\n", HeciMsg.MeBiosVerRsp.FeatureSet2));

  SpsFeatureSet.Data.Set1 = HeciMsg.MeBiosVerRsp.FeatureSet;
  SpsFeatureSet.Data.Set2 = HeciMsg.MeBiosVerRsp.FeatureSet2;
  if (SpsFeatureSet.Bits.NodeManager) {
    DEBUG ((DEBUG_INFO, "%a Node Manager enabled\n", SpsPrefix));
    NmFs.UInt32 = Heci2PciRead32 (SPS_REG_NMFS);
    if ((NmFs.UInt32 != 0xFFFFFFFF) &&
         NmFs.Bits.NmEnabled) {
#if ME_TESTMENU_FLAG
      if (pSpsPolicy != NULL) {
        if (pSpsPolicy->Common.NmPwrOptBootOverride) {
          NmFs.Bits.PerfOptBoot = !pSpsPolicy->Common.NmPwrOptBoot;
        }
        if (pSpsPolicy->Common.NmCores2DisableOverride) {
          NmFs.Bits.Cores2Disable = pSpsPolicy->Common.NmCores2Disable;
        }
      }
#endif
      NmSetBootMode ((BOOLEAN)NmFs.Bits.PerfOptBoot);
    } else {
      DEBUG ((DEBUG_ERROR, "%a ERROR: NMFS not configured while NM enabled "
                          "(feature set: 0x%08X, feature set 2: 0x%08X, NMFS: 0x%08X)\n",
                          SpsPrefix,
                          SpsFeatureSet.Data.Set1, SpsFeatureSet.Data.Set2, NmFs.UInt32));
    }

    //
    // Register SPS_HW_CHANGE_PPI for PTU support
    //
    Status = PeiServicesInstallPpi (&mInstallSpsHwChangePpi);
    ASSERT_EFI_ERROR (Status);
  } else {
    NmFs.UInt32 = 1; // no cores to disable, performance optimized boot
  }

  SpsHobCreate (SpsPrefix, ME_FUNCTIONAL, HeciMsg.MeBiosVerRsp.FeatureSet, HeciMsg.MeBiosVerRsp.FeatureSet2,
               (BOOLEAN)!NmFs.Bits.PerfOptBoot, (UINT8)NmFs.Bits.Cores2Disable);

#ifdef FSP_FLAG
  Status = SpsFspInitComplete (pHeciPpi, pSpsPolicy, ME_FUNCTIONAL, HeciMsg.MeBiosVerRsp.FeatureSet,
            HeciMsg.MeBiosVerRsp.FeatureSet2, !NmFs.Bits.PerfOptBoot, (UINT8)NmFs.Bits.Cores2Disable);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a ERROR: Unable to complete ME FSP initialization (%r)\n", SpsPrefix, Status));
  }
#endif // FSP_FLAG

  return EFI_SUCCESS;
} // SpsNonS3Path ()

/**
  SPS PEI entry point.

  @param[in] FileHandle  PEIM file handle
  @param[in] PeiServices General purpose services available to every PEIM

  @retval EFI_SUCCESS    The function completed successfully.
**/
EFI_STATUS
SpsPeiPostMemEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                    Status;
  EFI_BOOT_MODE                 BootMode;
  EFI_PEI_NOTIFY_DESCRIPTOR     *pNotifyList;

  DEBUG ((DEBUG_INFO, "%a %a called.\n", SpsPrefix, __FUNCTION__));

  //
  // Check if there is other than SPS ME on-board.
  //
  if (!MeTypeIsSps ()) {
    DEBUG ((DEBUG_INFO, "%a Other ME FW detected.\n", SpsPrefix));
    goto MeNonFunctional;
  }

  //
  // Get Boot mode
  //
  BootMode = BOOT_WITH_DEFAULT_SETTINGS;
  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot read boot mode (%r)\n", SpsPrefix, Status));
    BootMode = BOOT_WITH_DEFAULT_SETTINGS;
  }

  //
  // Install Protocols To be Called once HECI Initialization for PEI is performed
  //
  if (BootMode == BOOT_ON_S3_RESUME) {
    DEBUG ((DEBUG_INFO, "%a S3 resume path\n", SpsPrefix));
    pNotifyList = &mSpsPeiS3NotifyList[0];
  } else {
    DEBUG ((DEBUG_INFO, "%a Non S3 boot path\n", SpsPrefix));
    pNotifyList = &mSpsPeiNonS3NotifyList[0];

  }
  Status = (*PeiServices)->NotifyPpi(PeiServices, pNotifyList);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a ERROR: Cannot register PPI notify handler (%r)\n", SpsPrefix, Status));
  }

  return EFI_SUCCESS;

MeNonFunctional:
  MeIsNonFunctional (SpsPrefix);

  return EFI_UNSUPPORTED;
} // SpsPeiPostMemEntryPoint ()

/**
  This function sets power measurement suport status and hardware configuration
  change status to be sent to ME. It is needed when platform power measurement
  using Power/Thermal Utility (PTU) Option ROM is supported.

  NOTE: This function can be used only when Node Manager is enabled in ME.

  @param[in] pThis            Pointer to this PPI
  @param[in] PwrMsmtSupport   True if power measurement using PTU is supported,
                              otherwise false.
  @param[in] HwCfgChanged     True if hardware configuration changed since
                              last boot, otherwise false.

  @return EFI_STATUS is returned.
**/
EFI_STATUS EFIAPI
SpsHwChangeSetStatus (
  IN     SPS_HW_CHANGE_PPI *pThis,
  IN     BOOLEAN            PwrMsmtSupport,
  IN     BOOLEAN            HwCfgChanged)
{
  EFI_HOB_GUID_TYPE        *pGuidHob;
  SPS_INFO_HOB             *pSpsHob;

  pGuidHob = GetFirstGuidHob (&gSpsInfoHobGuid);
  pSpsHob = GET_GUID_HOB_DATA (pGuidHob);
  if (pGuidHob == NULL || !pSpsHob->FeatureSet.Bits.NodeManager) {
    return EFI_UNSUPPORTED;
  }
  pSpsHob->NmPowerMsmtSupport = PwrMsmtSupport;
  pSpsHob->NmHwChangeStatus = HwCfgChanged;
  return EFI_SUCCESS;
}
