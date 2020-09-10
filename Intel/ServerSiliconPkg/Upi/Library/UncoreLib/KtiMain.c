/** @file
  This file includes the entry point of KTI initialization and the main flow in KtiInit().

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include <RcRegs.h>
#include <KtiSetupDefinitions.h>
#include <Upi/KtiSi.h>
#include <KtiMisc.h>
#include <UncoreCommonIncludes.h>
#include <Library/MemMcIpLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemRcLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/KtiApi.h>
#include <Library/HobLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/ReferenceCodePolicyLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/IoAccessLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <ScratchpadList.h>
#include <Library/ChaLib.h>
#include <Guid/UboxIpInterface.h>
#include <Library/PcuIpLib.h>
#include <Guid/KtiIpInterface.h>
#include <Guid/IioIpInterface.h>
#include <Library/CsrAccessLib.h>
#include <Guid/FpgaIpInterface.h>
#include <Library/PeiServicesLib.h>
#include <Library/CxlLib.h>
#include <Guid/SocketCommonRcVariable.h>
#include "UncoreLibInternal.h"
#include <Library/KtiSimLib.h>
#include <Library/S3mLib.h>
#include <Library/SncPrefetchLib.h>
#include "KtiAdapt.h"
#include <Library/PreSiliconEnvDetectLib.h>
// APTIOV_SERVER_OVERRIDE_RC_START
#include <AmiMrcHooksWrapperLib/AmiMrcHooksWrapperLib.h>
// APTIOV_SERVER_OVERRIDE_RC_END

VOID
DataDumpKtiRcEntry (
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

VOID
DataDumpKtiRcExit (
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

KTI_STATUS
FillEarlySystemInfo (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

KTI_STATUS
PrimeHostStructure (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

KTI_STATUS
PrimeHostStructureIsoc (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

KTI_STATUS
PrimeHostStructureMisc (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

KTI_STATUS
CheckS3ResumePath (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

KTI_STATUS
SetupUncoreMisc (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

KTI_STATUS
KtiPcuMiscConfig (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

KTI_STATUS
ProgramMsrForWa (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

KTI_STATUS
KtiFinalCpuBusCfg (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

VOID
KtiNvramVerification (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

KTI_STATUS
KtiTriggerChaCreditChange (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

#ifdef IA32
static EFI_PEI_PPI_DESCRIPTOR mAfterKtircPpi[] =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiAfterKtiRcGuid,
  NULL
};
#endif //IA32

/**
  KTI entry point

  @param None

  @retval KTI STATUS

**/
KTI_STATUS
KtiMain (
  VOID
  )
{
  KTI_SOCKET_DATA            SocketData;
  KTI_HOST_INTERNAL_GLOBAL   KtiInternalGlobal;
  UINT8                      Index;
  UINT8                      CpuType;
  UINT8                      ResetRequired;
  OEM_HOOK_POST_TOPOLOGY_DISCOVERY  OemHookPostTopologyDiscoveryPtr;

  //
  // Initialize SocketData and KtiInternalGlobal
  //
  ZeroMem ((UINT8 *) &SocketData, sizeof (KTI_SOCKET_DATA));
  ZeroMem ((UINT8 *) &KtiInternalGlobal, sizeof (KTI_HOST_INTERNAL_GLOBAL));

  //
  // Retrieve setup, var and nvram pointers
  //
  KtiInternalGlobal.KtiSetup = (UINT32) (UINTN)KTI_HOST_IN_PTR;
  KtiInternalGlobal.KtiVar = (UINT32) (UINTN)KTI_HOST_OUT_PTR;
  KtiInternalGlobal.KtiNvram = (UINT32) (UINTN) KTI_HOST_NVRAM_PTR;
  KtiInternalGlobal.KtiPortCnt = GetKtiPortCnt ();

  //
  // Initialize CpuType in KtiInternalGlobal
  //
  KtiGetCpuTypeInfo (&KtiInternalGlobal);

  InitSocketData (&KtiInternalGlobal, &SocketData);

  //
  // Dump all the setup options and other input options that are expected to be initialized at this point
  //
  DataDumpKtiRcEntry (&KtiInternalGlobal);

  CopyMem ((UINT8 *)&KtiInternalGlobal.PhyLinkPerPortSetting, (UINT8 *)&((KTI_HOST_IN *) (UINTN) KtiInternalGlobal.KtiSetup)->PhyLinkPerPortSetting, sizeof (KTI_CPU_SETTING)* MAX_SOCKET);

  //
  // Fill in the system info that is known at this point
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Collecting Early System Information - %a *******\n", "\n\n\n", "START"));
  FillEarlySystemInfo (&SocketData, &KtiInternalGlobal);
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Collecting Early System Information - %a *******\n", "\n", "END  "));

  //
  // Setup MinPath from SBSP to all sockets and collect the LEP information
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Setting up Minimum Path - %a *******\n", "\n\n", "START"));
  SetupSbspPathToAllSockets (&SocketData, &KtiInternalGlobal);

  //
  // Call to OEM hook
  //
  OemHookPostTopologyDiscoveryPtr = (OEM_HOOK_POST_TOPOLOGY_DISCOVERY) (UINTN) (KTI_HOST_IN_PTR->OemHookPostTopologyDiscovery);
  if (OemHookPostTopologyDiscoveryPtr != NULL) {
    OemHookPostTopologyDiscoveryPtr ();
  }
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Setting up Minimum Path - %a *******\n", "\n", "END  "));

  //
  // Check if the system has valid KTI topology. Otherwise degrade it to supported topology
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Check for KTI Topology Degradation - %a *******\n", "\n\n", "START"));
  PreProcessKtiLinks (&SocketData, &KtiInternalGlobal);
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Check for KTI Topology Degradation - %a *******\n", "\n", "END"));

  CpuType     = KtiInternalGlobal.CpuType;

  //
  // Get the IIO count using the CAPID registers
  //

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Detecting IIO count - %a *******\n", "\n\n", "START"));
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if ((SocketData.Cpu[Index].Valid == TRUE) && (SocketData.Cpu[Index].SocType != SOCKET_TYPE_FPGA)) {
      GetIioAndM2PcieCount (Index, &KtiInternalGlobal);
    }
  }
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Detecting IIO count - %a *******\n", "\n", "END"));

  //
  // Update KTIRC output structure and verify Host if the input values comply with system topology; otherwise force them to default value
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Checking KTIRC Input Structure - %a *******\n", "\n\n", "START"));
  PrimeHostStructure (&SocketData, &KtiInternalGlobal);
  // APTIOV_SERVER_OVERRIDE_RC_START : OEM hook to change KTI parameters
  AmiOemKtiChangeParameterWrapper (&SocketData, &KtiInternalGlobal);
  // APTIOV_SERVER_OVERRIDE_RC_END : OEM hook to change KTI parameters
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Checking KTIRC Input Structure - %a *******\n", "\n", "END"));

  //
  // Verify the KTI NVRAM
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* KTI NVRAM Verification - %a *******\n", "\n\n", "START"));
  KtiNvramVerification (&KtiInternalGlobal);
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* KTI NVRAM Verification - %a *******\n", "\n", "END"));


  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Calculate Resource Allocation - %a *******\n", "\n\n", "START"));
  AllocateIioResources (&SocketData, &KtiInternalGlobal);
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Calculate Resource Allocation - %a *******\n", "\n", "END  "));

  //
  // If we are not in Cold Reset path, make sure the system topology is same as the one before reset
  //
  if (KtiInternalGlobal.CurrentReset != POST_RESET_POWERGOOD) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Check for KTI Topology change across reset - %a *******\n", "\n\n", "START"));
    CheckForTopologyChange (&SocketData, &KtiInternalGlobal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Check for KTI Topology change across reset - %a *******\n", "\n", "END"));
  }

  //
  // Sync up the PBSPs
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Sync Up PBSPs - %a *******\n", "\n\n", "START"));
  SyncUpPbspForReset (&SocketData, &KtiInternalGlobal, KtiInternalGlobal.CurrentReset);
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Sync Up PBSPs - %a *******\n", "\n", "END  "));

  //
  // Program bus numbers and enable mmcfg on PBSPs
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Program Mmcfg and bus numbers - %a *******\n", "\n\n", "START"));
  KtiFinalCpuBusCfg (&SocketData, &KtiInternalGlobal);
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Program Mmcfg and bus numbers - %a *******\n", "\n", "END  "));

  if (KtiInternalGlobal.CurrentReset == POST_RESET_POWERGOOD || KtiInternalGlobal.ForceColdResetFlow == TRUE) {
    //
    // Program the credits that require reset to take effect
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Programming Credits - %a *******\n", "\n\n", "START"));
    KtiCheckPoint (0xFF, 0xFF, 0xFF, STS_PROTOCOL_LAYER_SETTING, MINOR_STS_PROGRAM_RING_CRDT);
    ProgramMeshCredits (&SocketData, &KtiInternalGlobal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Programming Credits - %a *******\n", "\n", "END  "));

    //
    // Program MSR for w/a after all PBSPs are ready for subsequent warm reset
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Programming MSR for w/a - %a *******\n", "\n\n", "START"));
    ProgramMsrForWa (&SocketData, &KtiInternalGlobal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Programming MSR for w/a - %a *******\n", "\n", "END  "));

    //
    // Misc programming before WR
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Programming Misc CSRs before WR - %a *******\n", "\n\n", "START"));
    ProgramMiscBeforeWarmReset (&SocketData, &KtiInternalGlobal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Programming Misc CSRs before WR - %a *******\n", "\n", "END  "));

    //
    // setup MMIOL only during powergood. Since Mdfis and some other IP's registes are in MMIO space
    // so need to setup ubox mmio base, execute the function before mdfis training
    //
    EarlyIoSad (&SocketData, &KtiInternalGlobal);

    //
    // In the split die configuration, the sub dies are connected via Multi-die Fabric Interconnect.
    // At high frequencies, there is a risk that MDFIS autonomous training will not produce optimal signal quality.
    // Therefore, MDFIS allows for BIOS assisted advanced training flow.
    //
    if (KtiInternalGlobal.DfxMdfisAdvancedTrainingEn) {
      ConfigureMDFIAvanacedTraining (&KtiInternalGlobal);
    }
  }

  //
  // MDFI Advanced training requires core/mesh ratios to be at Pn value(Pn is default ratios after reset).
  // So BIOS should switch the core/mesh ratios to P1 after the MDFI training when MDFI training is needed.
  //
  SwitchCoreMeshRatio (CORE_RATIO_P1_ENCODING, MESH_RATIO_P1_ENCODING);

  //
  // SPR requires to execute the speed transition in every boot (warm and cold).
  //
  if (KtiInternalGlobal.CurrentReset == POST_RESET_POWERGOOD || IsSprFamilyCpu(CpuType)) {
    //
    // Program BGF Pointer Separation Overrides
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Programming BGF Overrides - %a *******\n", "\n\n", "START"));
    KtiProgramBGFOverrides (&SocketData, &KtiInternalGlobal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Programming BGF Overrides - %a *******\n", "\n", "END"));

    //
    // Set up Ubox MMIO space, and program BARs required for SPR UPI PHY recipe programming in warm reset.
    //
    if (KtiInternalGlobal.CurrentReset == POST_RESET_WARM) {
      EarlyIoSad (&SocketData, &KtiInternalGlobal);
    }

    //
    // Transition the links to full speed operation.
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Full Speed Transition - %a *******\n", "\n\n", "START"));
    KtiTransitionFullSpeed (&SocketData, &KtiInternalGlobal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Full Speed Transition - %a *******\n", "\n", "END"));
  }

  if (KtiInternalGlobal.CurrentReset == POST_RESET_POWERGOOD) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n******* Process Straps Config *******\n"));
    ProcessStrapsConfig (&SocketData, &KtiInternalGlobal);

    //
    // PCU Misc Configuration through pcode mail box command
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Pcu Misc Config - %a *******\n", "\n\n", "START"));
    KtiPcuMiscConfig (&SocketData, &KtiInternalGlobal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Pcu Misc Config - %a *******\n", "\n", "END"));
  }

  if ((KtiInternalGlobal.CurrentReset == POST_RESET_POWERGOOD) &&
      IsSiliconWorkaroundEnabled ("Warm Reset")) {
    KtiInternalGlobal.SkipWarmResetRequest = TRUE;
  }

  if (KtiInternalGlobal.CurrentReset == POST_RESET_WARM || KtiInternalGlobal.SkipWarmResetRequest) {
    //
    // Phy/LL work on warm reset
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Phy/Link Updates On Warm Reset - %a *******\n", "\n\n", "START"));
    KtiPhyLinkAfterWarmReset (&SocketData, &KtiInternalGlobal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Phy/Link Updates On Warm Reset - %a *******\n", "\n", "END"));
  }

  if (KtiInternalGlobal.SkipWarmResetRequest) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nResetRequired: %02x",GetSysResetRequired ()));
    ResetRequired = GetSysResetRequired () & (~POST_RESET_WARM);
    SetSysResetRequired (ResetRequired);
    if (ResetRequired == 0) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSkip the Kti Warm reset\n"));
      UBoxSetResetAllowedFlagInScratchpad (0, 0, 1);
      KtiTriggerChaCreditChange (&SocketData, &KtiInternalGlobal);
    }
  }

  if (GetSysResetRequired () == POST_RESET_NO_RESET) {
    if (UbiosGenerationEnabled ()) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nRST_CPL_SUPPORTED    EQU 0%xh\n", PcdGet8 (PcdDfxRstCplBitsEn)));
    }

    //
    // Do the topology discovery and optimum route calculation
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Topology Discovery and Optimum Route Calculation - %a *******\n", "\n\n", "START"));
    KtiTopologyDiscovery (&SocketData, &KtiInternalGlobal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Topology Discovery and Optimum Route Calculation - %a *******\n", "\n", "END  "));

    //
    // Setup the final IO SAD entries for all CPU sockets
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Program Final IO SAD Setting - %a *******\n", "\n\n", "START"));
    SetupSystemIoSadEntries (&SocketData, &KtiInternalGlobal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Program Final IO SAD Setting - %a *******\n", "\n", "END  "));

    //
    // Program the route table computed in the above step for each socket. Unlike the minimum path that we set before,
    // this is the complete, optimum route setting for the topology that is discovered dynamically.
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Program Optimum Route Table Settings - %a *******\n", "\n\n", "START"));
    ProgramSystemRoute (&SocketData, &KtiInternalGlobal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Program Optimum Route Table Settings - %a *******\n", "\n", "END  "));

    //
    // Setup KTI Protocol Layer miscellaneous registers
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Program Misc. KTI Parameters - %a *******\n", "\n\n", "START"));
    SetupKtiMisc (&SocketData, &KtiInternalGlobal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Program Misc. KTI Parameters - %a *******\n", "\n", "END  "));

    //
    // Program System Coherency registers
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Program System Coherency Registers - %a *******\n", "\n\n", "START"));
    SetupSystemCoherency (&SocketData, &KtiInternalGlobal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Program System Coherency Registers - %a *******\n", "\n", "END  "));

    //
    // S3 resume check to make sure the system configuration didn't change across S3
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Check for S3 Resume - %a *******\n", "\n\n", "START"));
    CheckS3ResumePath (&SocketData, &KtiInternalGlobal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Check for S3 Resume - %a *******\n", "\n", "END  "));

    //
    // SNC Misc and Recovery if failure on previous warm reset boot
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* SNC Misc and Recovery - %a *******\n", "\n\n", "START"));
    SncMisc (&SocketData, &KtiInternalGlobal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* SNC Misc and Recovery - %a *******\n", "\n", "END  "));

    //
    // TSC Sync for multi-sockets
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* TSC Sync - %a *******\n", "\n\n", "START"));
    Tsc_Sync (&SocketData, 0xFF, &KtiInternalGlobal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* TSC Sync - %a *******\n", "\n", "END  "));

    //
    // Collect the errors that might have occurred in previous boot and clear the error registers
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Collect Previous Boot Error - %a *******\n", "\n\n", "START"));
    CollectAndClearErrors (&SocketData, &KtiInternalGlobal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Collect Previous Boot Error - %a *******\n", "\n", "END  "));
  }

  SetupUncoreMisc (&SocketData, &KtiInternalGlobal);

  if (!(PcdGetBool (PcdEmuBiosSkipS3MAccess))) {
    //
    // Only runs S3M softstrap provision flow if the system is
    // running Warm-Reset Elimination flow
    // Note: S3M is accessed via SCF_MMIO, need to call it after
    // set system IO SAD
    //
    CollectS3mCpuStrapContents (&SocketData, &KtiInternalGlobal);
  }

  if (KtiInternalGlobal.CurrentReset == POST_RESET_POWERGOOD) {
    ProgramNumOfChaPerCluster (&SocketData, &KtiInternalGlobal);
  }
  //
  // Program SNC in CHA before memory is ready
  // Cold reset without skip warmreset, no need to do it
  //
  if (KtiInternalGlobal.CurrentReset != POST_RESET_POWERGOOD ||
      KtiInternalGlobal.SkipWarmResetRequest) {
    ProgramSncConfigureInChaBeforeMemoryReady (&SocketData, &KtiInternalGlobal);
  }

  //
  // KTI RC is almost complete, install AfterKtiRcPpi to trigger notify call back function.
  //
#ifdef IA32
  PeiServicesInstallPpi (mAfterKtircPpi);

  if (GetSysResetRequired () == POST_RESET_NO_RESET) {
    //
    // Initialize the Compute Express Link
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Initialize CXL - %a *******\n", "\n\n", "START"));
    InitializeComputeExpressLink (&SocketData, &KtiInternalGlobal);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Initialize CXL - %a *******\n", "\n", "END  "));
  }
#endif //IA32

  //
  // Dump output structure
  //
  DataDumpKtiRcExit (&KtiInternalGlobal);

  KtiCheckPoint (0xFF, 0xFF, 0xFF, STS_KTI_COMPLETE, MINOR_STS_KTI_COMPLETE);
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n******* KTIRC Exit  *******\n\n"));
  return KTI_SUCCESS;
}

/**
  Dump KTI RC variables on KTI RC entry

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval None

**/
VOID
DataDumpKtiRcEntry (
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  UINT8 soc;          //Target socket # (loop index)
  UINT8 port;         //Target port #   (loop ind)

  //
  // Dump KTIRC input structure
  //
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n\n******* KTI Setup Structure *******"));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nBus Ratio (per socket):"));
  for (soc=0; soc < MAX_SOCKET; soc++) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "  S%u: %u", soc, KTISETUP->BusRatio[soc]));
  }

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nD2KCreditConfig: %u [1:Low,2:Med,3:High]", KTISETUP->D2KCreditConfig));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nSnoopThrottleConfig: %u [0:Dis,1:Low,2:Med,3:High,4:Auto]", KTISETUP->SnoopThrottleConfig));

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nLegacyVgaSoc: %u", KTISETUP->LegacyVgaSoc));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nLegacyVgaStack: %u", KTISETUP->LegacyVgaStack));

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nP2pRelaxedOrdering: %u", KTISETUP->P2pRelaxedOrdering));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDebugPrintLevel: %u", KTISETUP->DebugPrintLevel));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDegradePrecedence: %u", KTISETUP->DegradePrecedence));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDegrade4SPreference: %u (%a)", KTISETUP->Degrade4SPreference, ((KTISETUP->Degrade4SPreference == 0) ? "4SFullyConnect" : "4S2LRing")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiLinkSpeedMode: %u (%a)", KTISETUP->KtiLinkSpeedMode, ((KTISETUP->KtiLinkSpeedMode == 0) ? "SLOW" : "FAST")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxWarmResetEliminationEn: %u", PcdGetBool (PcdWarmResetEliminationEn)));

  if (!IsSprFamilyCpu(KtiInternalGlobal->CpuType)) {
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiLinkSpeed: %u (%a)", KTISETUP->KtiLinkSpeed, ((KTISETUP->KtiLinkSpeed == SPEED_REC_96GT) ? "96GT" :
                                                                                            (KTISETUP->KtiLinkSpeed == SPEED_REC_104GT) ? "104GT" :
                                                                                            (KTISETUP->KtiLinkSpeed == SPEED_REC_112GT) ? "112GT" :
                                                                                            (KTISETUP->KtiLinkSpeed == MAX_KTI_LINK_SPEED) ? "MAX_SUPPORTED" :
                                                                                              (KTISETUP->KtiLinkSpeed == FREQ_PER_LINK) ? "PER_LINK" : "UNKNOWN")));
  } else {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiLinkSpeed: %u (%a)", KTISETUP->KtiLinkSpeed, ((KTISETUP->KtiLinkSpeed == SPEED_REC_128GT) ? "128GT" :
                                                                                              (KTISETUP->KtiLinkSpeed == SPEED_REC_144GT) ? "144GT" :
                                                                                              (KTISETUP->KtiLinkSpeed == SPEED_REC_160GT) ? "160GT" :
                                                                                              (KTISETUP->KtiLinkSpeed == MAX_KTI_LINK_SPEED) ? "MAX_SUPPORTED" :
                                                                                              (KTISETUP->KtiLinkSpeed == FREQ_PER_LINK) ? "PER_LINK" : "UNKNOWN")));
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiAdaptationEn: %u (%a)", KTISETUP->KtiAdaptationEn, ((KTISETUP->KtiAdaptationEn == KTI_DISABLE) ? "DISABLED" :
                                                                                                    (KTISETUP->KtiAdaptationEn == KTI_ENABLE) ? "ENABLED" : "UNKNOWN")));
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiAdaptationSpeed: %u (%a)", KTISETUP->KtiAdaptationSpeed, ((KTISETUP->KtiAdaptationSpeed == SPEED_REC_128GT) ? "128GT" :
                                                                                                          (KTISETUP->KtiAdaptationSpeed == SPEED_REC_144GT) ? "144GT" :
                                                                                                          (KTISETUP->KtiAdaptationSpeed == SPEED_REC_160GT) ? "160GT" :
                                                                                                          (KTISETUP->KtiAdaptationSpeed == MAX_KTI_LINK_SPEED) ? "MAX_SUPPORTED":"UNKNOWN")));
  }
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiLinkL0pEn: %u (%a)", KTISETUP->KtiLinkL0pEn, ((KTISETUP->KtiLinkL0pEn == KTI_DISABLE) ? "DISABLED" :
                                                                                            (KTISETUP->KtiLinkL0pEn == KTI_ENABLE) ? "ENABLED" :
                                                                                            (KTISETUP->KtiLinkL0pEn == KTI_AUTO) ? "AUTO" : "UNKNOWN")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiLinkL1En: %u (%a)", KTISETUP->KtiLinkL1En, ((KTISETUP->KtiLinkL1En == KTI_DISABLE) ? "DISABLED" :
                                                                                          (KTISETUP->KtiLinkL1En == KTI_ENABLE) ? "ENABLED" :
                                                                                          (KTISETUP->KtiLinkL1En == KTI_AUTO) ? "AUTO" : "UNKNOWN")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiFailoverEn: %u (%a)", KTISETUP->KtiFailoverEn, ((KTISETUP->KtiFailoverEn == KTI_DISABLE) ? "DISABLED" :
                                                                                              (KTISETUP->KtiFailoverEn == KTI_ENABLE) ? "ENABLED" :
                                                                                              (KTISETUP->KtiFailoverEn == KTI_AUTO) ? "AUTO" : "UNKNOWN")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiLbEn: %u", KTISETUP->KtiLbEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nSncEn: %u", KTISETUP->SncEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nIoDcMode: %u (%a)", KTISETUP->IoDcMode, ((KTISETUP->IoDcMode == KTI_DISABLE) ? "DISABLED" :
                                                                                    (KTISETUP->IoDcMode == IODC_AUTO) ? "AUTO" :
                                                                                    (KTISETUP->IoDcMode == IODC_EN_REM_INVITOM_PUSH) ? "IODC_EN_REM_INVITOM_PUSH" :
                                                                                    (KTISETUP->IoDcMode == IODC_EN_REM_INVITOM_ALLOCFLOW) ? "IODC_EN_REM_INVITOM_ALLOCFLOW" :
                                                                                    (KTISETUP->IoDcMode == IODC_EN_REM_INVITOM_ALLOC_NONALLOC) ? "IODC_EN_REM_INVITOM_ALLOC_NONALLOC" :
                                                                                    (KTISETUP->IoDcMode == IODC_EN_REM_INVITOM_AND_WCILF) ? "IODC_EN_REM_INVITOM_AND_WCILF" : "UNKNOWN")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDirectoryModeEn: %u", KTISETUP->DirectoryModeEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nXptPrefetchEn: %u", KTISETUP->XptPrefetchEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiPrefetchEn: %u", KTISETUP->KtiPrefetchEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nXptRemotePrefetchEn:  %u", KTISETUP->XptRemotePrefetchEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nRdCurForXptPrefetchEn: %u", KTISETUP->RdCurForXptPrefetchEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nStaleAtoSOptEn: %u", KTISETUP->StaleAtoSOptEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nLLCDeadLineAlloc: %u", KTISETUP->LLCDeadLineAlloc));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nSplitLock: %u", KTISETUP->SplitLock));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDdrtQosMode: %u", KTISETUP->DdrtQosMode));

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiFpgaEnable (per socket):"));
  for (soc=0; soc < MAX_SOCKET; soc++) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "  S%u: %u", soc, KTISETUP->KtiFpgaEnable[soc]));
  }

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiCrcMode: %u (%a)", KTISETUP->KtiCrcMode, ((KTISETUP->KtiCrcMode == CRC_MODE_16BIT) ? "16BIT" :
                                                                                        (KTISETUP->KtiCrcMode == CRC_MODE_ROLLING_32BIT) ? "ROLLING_32BIT" :
                                                                                        (KTISETUP->KtiCrcMode == CRC_MODE_AUTO) ? "AUTO" : "UNKNOWN")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiCpuSktHotPlugEn: %u", KTISETUP->KtiCpuSktHotPlugEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiCpuSktHotPlugTopology: %u (%a)", KTISETUP->KtiCpuSktHotPlugTopology, ((KTISETUP->KtiCpuSktHotPlugTopology == HOT_PLUG_TOPOLOGY_4S) ? "4S" : "8S")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiSkuMismatchCheck: %u", KTISETUP->KtiSkuMismatchCheck));

  //
  // Phy/Link Layer Options (per Port)
  //
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiPortDisable (per port):"));
  for (soc=0; soc < MAX_SOCKET; soc++) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  S%1u:", soc));
    for (port=0; port < KtiInternalGlobal->KtiPortCnt; port++) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "%1u ", KTISETUP->PhyLinkPerPortSetting[soc].Link[port].KtiPortDisable));
    }
  }

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiLinkVnaOverride (per port):"));
  for (soc=0; soc < MAX_SOCKET; soc++) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  S%1u:", soc));
      for (port=0; port < KtiInternalGlobal->KtiPortCnt; port++) {
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "%1u ", KTISETUP->PhyLinkPerPortSetting[soc].Link[port].KtiLinkVnaOverride));
    }
  }

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiLinkSpeed (per port):"));
  for (soc=0; soc < MAX_SOCKET; soc++) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  S%1u:", soc));
      for (port=0; port < KtiInternalGlobal->KtiPortCnt; port++) {
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "%1u ", KTISETUP->PhyLinkPerPortSetting[soc].Phy[port].KtiLinkSpeed));
    }
  }
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nRsvd (per port):"));
  for (soc=0; soc < MAX_SOCKET; soc++) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  S%1u:", soc));
      for (port=0; port < KtiInternalGlobal->KtiPortCnt; port++) {
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "%1u ", KTISETUP->PhyLinkPerPortSetting[soc].Phy[port].Rsvd));
    }
  }

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n\n**KTI Setup Structure DfxParms **"));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxHaltLinkFailReset: %u (%a)", KTISETUP->DfxParm.DfxHaltLinkFailReset, ((KTISETUP->DfxParm.DfxHaltLinkFailReset == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxKtiMaxInitAbort: %u (%a)", KTISETUP->DfxParm.DfxKtiMaxInitAbort, ((KTISETUP->DfxParm.DfxKtiMaxInitAbort == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxLlcShareDrdCrd %u (%a)", KTISETUP->DfxParm.DfxLlcShareDrdCrd, ((KTISETUP->DfxParm.DfxLlcShareDrdCrd == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxBiasFwdMode: %u (%a)", KTISETUP->DfxParm.DfxBiasFwdMode, ((KTISETUP->DfxParm.DfxBiasFwdMode == BIAS_FWD_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxSnoopFanoutEn: %u (%a)", KTISETUP->DfxParm.DfxSnoopFanoutEn, ((KTISETUP->DfxParm.DfxSnoopFanoutEn == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxHitMeEn: %u (%a)", KTISETUP->DfxParm.DfxHitMeEn, ((KTISETUP->DfxParm.DfxHitMeEn == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxFrcfwdinv %u (%a)", KTISETUP->DfxParm.DfxFrcfwdinv, ((KTISETUP->DfxParm.DfxFrcfwdinv == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxDbpEnable %u (%a)", KTISETUP->DfxParm.DfxDbpEnable, ((KTISETUP->DfxParm.DfxDbpEnable == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxCleanEvictAlwaysLive: %u (%a)", KTISETUP->DfxParm.DfxCleanEvictAlwaysLive, ((KTISETUP->DfxParm.DfxCleanEvictAlwaysLive == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxModifiedEvictAlwaysLive: %u (%a)", KTISETUP->DfxParm.DfxModifiedEvictAlwaysLive, ((KTISETUP->DfxParm.DfxModifiedEvictAlwaysLive == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxOsbEn %u (%a)", KTISETUP->DfxParm.DfxOsbEn, ((KTISETUP->DfxParm.DfxOsbEn == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxHitMeRfoDirsEn %u (%a)", KTISETUP->DfxParm.DfxHitMeRfoDirsEn, ((KTISETUP->DfxParm.DfxHitMeRfoDirsEn == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxGateOsbIodcAllocEn %u (%a)", KTISETUP->DfxParm.DfxGateOsbIodcAllocEn, ((KTISETUP->DfxParm.DfxGateOsbIodcAllocEn == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxDualLinksInterleavingMode %u (%a)", KTISETUP->DfxParm.DfxDualLinksInterleavingMode, ((KTISETUP->DfxParm.DfxDualLinksInterleavingMode == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxSystemDegradeMode: %u", KTISETUP->DfxParm.DfxSystemDegradeMode));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxVn1En: %u (%a)", KTISETUP->DfxParm.DfxVn1En, ((KTISETUP->DfxParm.DfxVn1En == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxD2cEn: %u (%a)", KTISETUP->DfxParm.DfxD2cEn, ((KTISETUP->DfxParm.DfxD2cEn == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxD2kEn: %u (%a)", KTISETUP->DfxParm.DfxD2kEn, ((KTISETUP->DfxParm.DfxD2kEn == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxLockMaster: %u (%a)", KTISETUP->DfxParm.DfxLockMaster, ((KTISETUP->DfxParm.DfxLockMaster == LOCK_MASTER_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxOsbLocRd: %u (%a)", KTISETUP->DfxParm.DfxOsbLocRd, ((KTISETUP->DfxParm.DfxOsbLocRd == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxOsbLocRdCur: %u (%a)", KTISETUP->DfxParm.DfxOsbLocRdCur, ((KTISETUP->DfxParm.DfxOsbLocRdCur == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxOsbRmtRd: %u (%a)", KTISETUP->DfxParm.DfxOsbRmtRd, ((KTISETUP->DfxParm.DfxOsbRmtRd == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxM3KtiCountMismatchEn: %u (%a)", KTISETUP->DfxParm.DfxM3KtiCountMismatchEn, ((KTISETUP->DfxParm.DfxM3KtiCountMismatchEn == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxSnoopFanoutChaInterleaveEn: %u (%a)", KTISETUP->DfxParm.DfxSnoopFanoutChaInterleaveEn, ((KTISETUP->DfxParm.DfxSnoopFanoutChaInterleaveEn == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxXptFifoEnabledCredit: 0x%x ", KTISETUP->DfxParm.DfxXptFifoEnabledCredit));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxClockModulationEn: %u (%a)", KTISETUP->DfxParm.DfxClockModulationEn, ((KTISETUP->DfxParm.DfxClockModulationEn == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxMdfisAdvancedTrainingEn: %u (%a)", KTISETUP->DfxParm.DfxMdfisAdvancedTrainingEn, ((KTISETUP->DfxParm.DfxMdfisAdvancedTrainingEn == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxCxlSecLvl: %u (%a)", KTISETUP->DfxParm.DfxCxlSecLvl, ((KTISETUP->DfxParm.DfxCxlSecLvl == CXL_SECURITY_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxCxlStcge: %u (%a)", KTISETUP->DfxParm.DfxCxlStcge, ((KTISETUP->DfxParm.DfxCxlStcge == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxCxlSdcge: %u (%a)", KTISETUP->DfxParm.DfxCxlSdcge, ((KTISETUP->DfxParm.DfxCxlSdcge == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxCxlDlcge: %u (%a)", KTISETUP->DfxParm.DfxCxlDlcge, ((KTISETUP->DfxParm.DfxCxlDlcge == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxCxlLtcge: %u (%a)", KTISETUP->DfxParm.DfxCxlLtcge, ((KTISETUP->DfxParm.DfxCxlLtcge == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxCxlVid: %u (%a)", KTISETUP->DfxParm.DfxCxlVid, ((KTISETUP->DfxParm.DfxCxlVid == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxIioDfxEnabled: %u (%a)", KTISETUP->DfxParm.DfxIioDfxEnabled, ((KTISETUP->DfxParm.DfxIioDfxEnabled == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxIioStackEn: 0x%08X (%a)", KTISETUP->DfxParm.DfxIioStackEn, ((KTISETUP->DfxParm.DfxIioStackEn == DFX_IIO_STACK_EN_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxHqmEn: %u (%a)", KTISETUP->DfxParm.DfxHqmEn, ((KTISETUP->DfxParm.DfxHqmEn == KTI_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxPmonConfig: %u (%a)", KTISETUP->DfxParm.DfxPmonConfig, ((KTISETUP->DfxParm.DfxPmonConfig == KTI_PMON_CONFIG_AUTO) ? "AUTO" :
                                                                                                      (KTISETUP->DfxParm.DfxPmonConfig == KTI_PMON_CONFIG_DISABLE) ? "DISABLED" :
                                                                                                      (KTISETUP->DfxParm.DfxPmonConfig == KTI_PMON_CONFIG_REDUCED) ? "REDUCED" : "FULL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxM2IosfPmonAccessControl: %u (%a)", KTISETUP->DfxParm.DfxM2IosfPmonAccessControl,
                                                                                                      ((KTISETUP->DfxParm.DfxM2IosfPmonAccessControl == KTI_PMON_ACC_CTRL_AUTO) ? "AUTO" :
                                                                                                      (KTISETUP->DfxParm.DfxM2IosfPmonAccessControl == KTI_PMON_ACC_CTRL_DISABLE) ? "DISABLED" : "FULL")));

  //
  // DFX Phy/Link layer options (per port):
  //
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxCrcMode (per port):"));
  for (soc=0; soc < MAX_SOCKET; soc++) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  S%1u:", soc));
      for (port=0; port < KtiInternalGlobal->KtiPortCnt; port++) {
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "%1u (%a) ", KTISETUP->DfxCpuCfg[soc].Link[port].DfxCrcMode, ((KTISETUP->DfxCpuCfg[soc].Link[port].DfxCrcMode == CRC_MODE_AUTO) ? "AUTO" : "MANUAL")));
    }
  }
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxL0pEnable (per port):"));
  for (soc=0; soc < MAX_SOCKET; soc++) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  S%1u:", soc));
      for (port=0; port < KtiInternalGlobal->KtiPortCnt; port++) {
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "%1u (%a) ", KTISETUP->DfxCpuCfg[soc].Link[port].DfxL0pEnable, ((KTISETUP->DfxCpuCfg[soc].Link[port].DfxL0pEnable == KTI_AUTO) ? "AUTO" : "MANUAL")));
    }
  }
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxL1Enable (per port):"));
  for (soc=0; soc < MAX_SOCKET; soc++) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  S%1u:", soc));
      for (port=0; port < KtiInternalGlobal->KtiPortCnt; port++) {
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "%1u (%a) ", KTISETUP->DfxCpuCfg[soc].Link[port].DfxL1Enable, ((KTISETUP->DfxCpuCfg[soc].Link[port].DfxL1Enable == KTI_AUTO) ? "AUTO" : "MANUAL")));
    }
  }
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nDfxKtiFailoverEn (per port):"));
  for (soc=0; soc < MAX_SOCKET; soc++) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  S%1u:", soc));
      for (port=0; port < KtiInternalGlobal->KtiPortCnt; port++) {
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "%1u (%a) ", KTISETUP->DfxCpuCfg[soc].Phy[port].DfxKtiFailoverEn, ((KTISETUP->DfxCpuCfg[soc].Phy[port].DfxKtiFailoverEn == KTI_AUTO) ? "AUTO" : "MANUAL")));
    }
  }

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n\n**Common Setup Structure**"));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nmmCfgBase: %u (%a)", KTISETUP->mmCfgBase, ((KTISETUP->mmCfgBase == MMCFG_BASE_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nmmCfgSize: %u (%a)", KTISETUP->mmCfgSize, ((KTISETUP->mmCfgSize == MMCFG_SIZE_AUTO) ? "AUTO" : "MANUAL")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nmmiohBase: 0x%08X ", KTISETUP->mmiohBase));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nmmiohSize: %u (%uGB per stack) ", KTISETUP->mmiohSize, (1 << (2 * KTISETUP->mmiohSize))));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nnumaEn: %u ", GetNumaEn ()));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nUmaClustering: %u ", KTISETUP->UmaClustering));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nisocEn: %u ", KTISETUP->isocEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\ndcaEn: %u ", KTISETUP->dcaEn));

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n\n**Common Var Structure **"));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nresetRequired: %u ", GetSysResetRequired ()));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nstate: %u ", GetSysBootMode ()));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nnumCpus: %u ", KTI_HOST_OUT_PTR->numCpus));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nsocketPresentBitMap: 0x%02X ", GetSocketPresentBitMap ()));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nSlaveDieBitMap: 0x%02X ", GetSlaveDieBitMap ()));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nmmCfgBase: 0x%08X ", KTI_HOST_OUT_PTR->mmCfgBase));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nmeRequestedSize: %u ", GetMeRequestedSize ()));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nieRequestedSize: %u ", GetIeRequestedSize ()));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n"));

  return;
}

/**
  Populate CpuCsrAccessVar structure and display current bus assignments

  @param KtiInternalGlobal    - pointer to the KTI RC internal global structure
  @param CpuCsrAccessVar      - pointer to CpuCsrAccessVar structure to be populated
  @param SocketData           - Pointer to socket specific data
  @param PrintMissingSockets  - TRUE/FALSE

  @retval None

**/
VOID
UpdateAndDisplayBusInformation (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  CPU_CSR_ACCESS_VAR         *CsrAccessVarPtr,
  KTI_SOCKET_DATA            *SocketData,
  BOOLEAN                    PrintMissingSockets
  )
{
  UpdateCpuCsrAccessVar_PEI (CsrAccessVarPtr);
  DumpCurrentBusInfo (KtiInternalGlobal, SocketData, PrintMissingSockets);
  return;
}

/**
  Dump KTI RC variables on KTI RC exit.

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure

  @retval None

**/
VOID
DataDumpKtiRcExit (
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  )
{
  UINT8        Index,Port;
  CONST CHAR8  *RoutingModeStr;

  //
  // Dump KTI output structure
  //
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n\n**KTI Output Structure **"));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutD2KCreditConfig: %u [1:Low,2:Med,3:High]", KTIVAR->OutD2KCreditConfig));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nSnoopThrottleConfig: %u [0:Dis,1:Low,2:Med,3:High,4:Auto]", KTIVAR->OutSnoopThrottleConfig));

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nUpiAffinityEn: %u", KTIVAR->UpiAffinityEn));

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutPmonConfig: %u [1:Reduced,2:Full,3:Auto,0:Disabled]", KTIVAR->OutPmonConfig));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutM2IosfPmonAccessControl: %u [0:Restricted,1:Full,2:Auto]", KTIVAR->OutM2IosfPmonAccessControl));

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutD2kEn: %u", KTIVAR->OutD2kEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutLegacyVgaSoc: %u", KTIVAR->OutLegacyVgaSoc));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutLegacyVgaStack: %u", KTIVAR->OutLegacyVgaStack));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutIsocEn: %u", KTIVAR->OutIsocEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutHitMeEn: %u", KTIVAR->OutHitMeEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutSncEn: %u", KTIVAR->OutSncEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutUmaClustering: %u", KTIVAR->OutUmaClustering));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutSysDirectoryModeEn: %u", KTIVAR->OutSysDirectoryModeEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutKtiPrefetch: %u", KTIVAR->OutKtiPrefetch));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutXptPrefetch: %u", KTIVAR->OutXptPrefetch));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutXptRemotePrefetch: %u", KTIVAR->OutXptRemotePrefetch));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutSncPrefetchEn: %u", KTIVAR->OutSncPrefetchEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutSncGbAlignRequired: %u", KTIVAR->OutSncGbAlignRequired));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutIsRouteThrough: %u", KTIVAR->OutIsRouteThrough));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutStaleAtoSOptEn: %u", KTIVAR->OutStaleAtoSOptEn));

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutSystemRasType: %u (%a)", KTIVAR->OutSystemRasType,
                                                                     ((KTIVAR->OutSystemRasType == HEDT_RAS) ? "HEDT" :
                                                                      (KTIVAR->OutSystemRasType == BASIC_RAS_AX) ? "BASIC" :
                                                                      (KTIVAR->OutSystemRasType == S1WS_RAS) ? "S1WS" :
                                                                      (KTIVAR->OutSystemRasType == CORE_RAS) ? "CORE" :
                                                                      (KTIVAR->OutSystemRasType == FPGA_RAS) ? "FPGA" :
                                                                      (KTIVAR->OutSystemRasType == STANDARD_RAS) ? "STANDARD" :
                                                                      (KTIVAR->OutSystemRasType == ADVANCED_RAS) ? "ADVANCED" :
                                                                      (KTIVAR->OutSystemRasType == HPC_RAS) ? "HPC" :
                                                                      (KTIVAR->OutSystemRasType == STANDARD_PLUS_RAS) ? "STANDARD_PLUS" :
                                                                      (KTIVAR->OutSystemRasType == CIRRUS_RAS) ? "CIRRUS" : "UNKNOWN")));

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutIoDcMode: %u (%a)", KTIVAR->OutIoDcMode, ((KTIVAR->OutIoDcMode == KTI_DISABLE) ? "DISABLED" :
                                                                                        (KTIVAR->OutIoDcMode == IODC_AUTO) ? "AUTO" :
                                                                                        (KTIVAR->OutIoDcMode == IODC_EN_REM_INVITOM_PUSH) ? "IODC_EN_REM_INVITOM_PUSH" :
                                                                                        (KTIVAR->OutIoDcMode == IODC_EN_REM_INVITOM_ALLOCFLOW) ? "IODC_EN_REM_INVITOM_ALLOCFLOW" :
                                                                                        (KTIVAR->OutIoDcMode == IODC_EN_REM_INVITOM_ALLOC_NONALLOC) ? "IODC_EN_REM_INVITOM_ALLOC_NONALLOC" :
                                                                                        (KTIVAR->OutIoDcMode == IODC_EN_REM_INVITOM_AND_WCILF) ? "IODC_EN_REM_INVITOM_AND_WCILF" : "UNKNOWN")));
 KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiCurrentLinkSpeedMode: %u (%a)", KTIVAR->KtiCurrentLinkSpeedMode, ((KTIVAR->KtiCurrentLinkSpeedMode == 0) ? "SLOW" : "FAST")));
  if (!IsSprFamilyCpu(KtiInternalGlobal->CpuType)) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutKtiLinkSpeed: %u (%a)", KTIVAR->OutKtiLinkSpeed, ((KTIVAR->OutKtiLinkSpeed == SPEED_REC_96GT) ? "96GT" :
                                                                                                  (KTIVAR->OutKtiLinkSpeed == SPEED_REC_104GT) ? "104GT" :
                                                                                                  (KTIVAR->OutKtiLinkSpeed == SPEED_REC_112GT) ? "112GT" :
                                                                                                  (KTIVAR->OutKtiLinkSpeed == MAX_KTI_LINK_SPEED) ? "MAX_SUPPORTED" :
                                                                                                  (KTIVAR->OutKtiLinkSpeed == FREQ_PER_LINK) ? "PER_LINK" : "UNKNOWN")));
  } else {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutKtiLinkSpeed: %u (%a)", KTIVAR->OutKtiLinkSpeed, ((KTIVAR->OutKtiLinkSpeed == SPEED_REC_128GT) ? "128GT" :
                                                                                                  (KTIVAR->OutKtiLinkSpeed == SPEED_REC_144GT) ? "144GT" :
                                                                                                  (KTIVAR->OutKtiLinkSpeed == SPEED_REC_160GT) ? "160GT" :
                                                                                                  (KTIVAR->OutKtiLinkSpeed == MAX_KTI_LINK_SPEED) ? "MAX_SUPPORTED" :
                                                                                                  (KTIVAR->OutKtiLinkSpeed == FREQ_PER_LINK) ? "PER_LINK" : "UNKNOWN")));
  }
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutKtiLinkL0pEn: %u (%a)", KTIVAR->OutKtiLinkL0pEn, ((KTIVAR->OutKtiLinkL0pEn == KTI_DISABLE) ? "DISABLED" :
                                                                                                (KTIVAR->OutKtiLinkL0pEn == KTI_ENABLE) ? "ENABLED" :
                                                                                                (KTIVAR->OutKtiLinkL0pEn == KTI_AUTO) ? "AUTO" : "UNKNOWN")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutKtiLinkL1En: %u (%a)", KTIVAR->OutKtiLinkL1En, ((KTIVAR->OutKtiLinkL1En == KTI_DISABLE) ? "DISABLED" :
                                                                                              (KTIVAR->OutKtiLinkL1En == KTI_ENABLE) ? "ENABLED" :
                                                                                              (KTIVAR->OutKtiLinkL1En == KTI_AUTO) ? "AUTO" : "UNKNOWN")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutKtiFailoverEn: %u (%a)", KTIVAR->OutKtiFailoverEn, ((KTIVAR->OutKtiFailoverEn == KTI_DISABLE) ? "DISABLED" :
                                                                                                  (KTIVAR->OutKtiFailoverEn == KTI_ENABLE) ? "ENABLED" :
                                                                                                  (KTIVAR->OutKtiFailoverEn == KTI_AUTO) ? "AUTO" : "UNKNOWN")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutKtiCrcMode: %u (%a)", KTIVAR->OutKtiCrcMode, ((KTIVAR->OutKtiCrcMode == CRC_MODE_16BIT) ? "16BIT" :
                                                                                            (KTIVAR->OutKtiCrcMode == CRC_MODE_ROLLING_32BIT) ? "ROLLING_32BIT" :
                                                                                            (KTIVAR->OutKtiCrcMode == CRC_MODE_AUTO) ? "AUTO" : "UNKNOWN")));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutBoardVsCpuConflict: 0x%x", KTIVAR->OutBoardVsCpuConflict));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutKtiAdaptationEnable: 0x%x", KTIVAR->OutKtiAdaptationEnable));

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutKtiPerLinkL1En (per port):"));
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  S%1u:", Index));
    for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "%1u ", KTIVAR->OutKtiPerLinkL1En[Index][Port]));
    }
  }

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutKtiFpgaPresent (per socket):"));
  for (Index=0; Index < MAX_SOCKET; Index++) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "  S%u: %u", Index, KTIVAR->OutKtiFpgaPresent[Index]));
  }

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutKtiFpgaEnable  (per socket):"));
  for (Index=0; Index < MAX_SOCKET; Index++) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "  S%u: %u", Index, KTIVAR->OutKtiFpgaEnable[Index]));
  }

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutFpgaHomeAgent (per socket):"));
  for (Index=0; Index < MAX_SOCKET; Index++) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "  S%u: %u", Index, KTIVAR->OutFpgaHomeAgent[Index]));
  }

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nOutFpgaCacheAgent (per socket):"));
  for (Index=0; Index < MAX_SOCKET; Index++) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "  S%u: %u", Index, KTIVAR->OutFpgaCacheAgent[Index]));
  }

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nmmCfgBase: 0x%08X", KTIVAR->mmCfgBase));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nmmCfgSize: 0x%08X", KTIVAR->mmCfgSize));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nmmiolBase: 0x%08X", KTIVAR->mmiolBase));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nmmiolSize: 0x%08X", KTIVAR->mmiolSize));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nlowGap   : 0x%02X", KTIVAR->lowGap));

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nKtiInternalGlobal:"));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\t->SnoopFanoutEn: %u", KtiInternalGlobal->SnoopFanoutEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\t->SysOsbEn: %u", KtiInternalGlobal->SysOsbEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\t->D2cEn: %u", KtiInternalGlobal->D2cEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\t->D2kEn: %u", KtiInternalGlobal->D2kEn));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\t->SnoopFilter: %u", KtiInternalGlobal->SnoopFilter));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\t->DualLinksInterleavingMode: %u", KtiInternalGlobal->DualLinksInterleavingMode));
  RoutingModeStr = (KtiInternalGlobal->UpiRoutingMode < UpiRoutingModeMax)? \
                    UpiRoutingModeStr[KtiInternalGlobal->UpiRoutingMode]: UpiRoutingModeStr[UpiRoutingModeMax];
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\t->UpiRoutingMode: (%a) %u", RoutingModeStr, KtiInternalGlobal->UpiRoutingMode));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\t->EightSocketTopology: %u", KtiInternalGlobal->EightSocketTopology));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\t->SnoopFanoutChaInterleaveEn: %u", KtiInternalGlobal->SnoopFanoutChaInterleaveEn));
}

/**
  Verify the KTI setup knobs are modified during the warm boot and S3, KTI requests cold boot if there is
  any knob changed

  In cold boot: update the nvram structure
  In Warm Boot: Check the BIT1 of SP0
  BIT1=0, the system is not finish the MRC, keep updating nvram structure.
  BIT1=1, the system is boot from OS warm boot that means the nvram was saved in variable in previous boot.
          compare the nvram and setup variable, if they are not matched, request the cold boot.
  In S3 resume: Compare the NvRam structure in memory with setup directly

  @param  KtiInternalGlobal - pointer to the KTI RC internal global structure

  @retval None

**/
VOID
KtiNvramVerification (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  UINT32                  KtiNvramSize;
  BOOLEAN                 bWarmBootFlagSet;

  KtiNvramSize = (UINT32) (&KTINVRAM->SaveSetupData.ColdResetRequestEnd - &KTINVRAM->SaveSetupData.ColdResetRequestStart);
  if (GetSysBootMode () == NormalBoot) {
    if (KtiInternalGlobal->CurrentReset == POST_RESET_POWERGOOD) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n----------Update Kti Nvram in COLD BOOT ----------\n"));
      //
      // Update KTI Nvram
      //
      CopyMem ((UINT8*)&(KTINVRAM->SaveSetupData.ColdResetRequestStart), (UINT8*)&(KTISETUP->ColdResetRequestStart), KtiNvramSize);
    } else if (KtiInternalGlobal->CurrentReset == POST_RESET_WARM) {
      UBoxCheckWarmBootFlagSetInScratchpad (0, 0, &bWarmBootFlagSet);
      if (!bWarmBootFlagSet) {
        //
        // if BIOSSCRATCHPAD0_UBOX_MISC_REG[1] is 0, don't compare the nvram variable
        //
        CopyMem ((UINT8*)&(KTINVRAM->SaveSetupData.ColdResetRequestStart), (UINT8*)&(KTISETUP->ColdResetRequestStart), KtiNvramSize);
      } else {
        if (KTISETUP->IsKtiNvramDataReady){
          if (CompareMem ((UINT8 *)&KTINVRAM->SaveSetupData.ColdResetRequestStart, (UINT8 *)&KTISETUP->ColdResetRequestStart, KtiNvramSize)) {
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Kti setup has been changed in warm boot, request a cold boot!!!\n"));
            SetNewResetRequired (POST_RESET_POWERGOOD);
            return;
          }
        } else {
          //
          // If there is no kti nvram in variable, update the NVRAM data structure
          //
          CopyMem ((UINT8*)&(KTINVRAM->SaveSetupData.ColdResetRequestStart), (UINT8*)&(KTISETUP->ColdResetRequestStart), KtiNvramSize);
        }
      }
    }
  } else {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n---------- Verify Kti Nvram in S3 Resume ---------\n"));
    //
    // Resume from S3
    //
    if (CompareMem ((UINT8 *)&KTINVRAM->SaveSetupData.ColdResetRequestStart, (UINT8 *)&KTISETUP->ColdResetRequestStart, KtiNvramSize)) {
      KtiCheckPoint (0xFF, 0xFF, 0xFF, ERR_S3_RESUME, MINOR_ERR_S3_RESUME_SETUP_MISMATCH);
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Kti Setup Doesn't Match - S3 Resume Failed."));
      KTI_ASSERT (FALSE, ERR_S3_RESUME, MINOR_ERR_S3_RESUME_SETUP_MISMATCH);
    }
  }
}

/**
  Routine to collect information that is available at this stage of booting and needed to initialize KTI.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0 - Successful
  @retval Other - failure

**/
KTI_STATUS
FillEarlySystemInfo (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  UINT32                              BusBase, BusSize;
  UINT8                               sbspSktId, Idx1, Idx2, i;
  UINT8                               BusBaseIdx;
  UINT8                               MaxKtiPort = 1;
  UINT8                               CpuType;
  UINT8                               SteppingInScratchPad;
  BOOLEAN                             bCmpColdReset;
  CONST CHAR8                         *RoutingModeStr;

  KtiCheckPoint (0xFF, 0xFF, 0xFF, STS_COLLECT_EARLY_SYSTEM_INFO, MINOR_STS_COLLECT_INFO);

  CpuType     = KtiInternalGlobal->CpuType;

  //
  // Initialize SkipWarmResetRequest default value to FALSE.
  // For SOC that supports warm-reset elimination feature (PcdWarmResetEliminationEn = TRUE),
  // SkipWarmResetRequest will be reconfigured in CheckSetStrapRegsConfiguration() based on
  // whether softstraps match the existing registers configuration.
  //
  KtiInternalGlobal->SkipWarmResetRequest = FALSE;
  KTIVAR->WarmResetEliminationEn = PcdGetBool (PcdWarmResetEliminationEn);

  //
  // SBSP socket id & Stepping
  //
  sbspSktId = KtiInternalGlobal->SbspSoc = GetSysSbspSocketIdNv ();

  //
  // Get SBSP's ChopType
  //
  KtiInternalGlobal->ChopType[sbspSktId] = PcuGetPhysicalChop (sbspSktId);
  KTIVAR->CpuInfo[sbspSktId].ChopType = KtiInternalGlobal->ChopType[sbspSktId];
  KTIVAR->CpuInfo->ChopTypeValid = TRUE;

  //
  // Extract the current system reset type from Sticky Scratch (updated by CPURC)
  //
  UBoxCheckCompletedColdResetFlowFlagSetInScratchpad (sbspSktId, 0, &bCmpColdReset);
  if (bCmpColdReset) {
    //
    // Indicates the successful completion of Cold Reset flow.
    //
    KtiInternalGlobal->CurrentReset = POST_RESET_WARM;
  } else {
    KtiInternalGlobal->CurrentReset = POST_RESET_POWERGOOD;
  }

  UBoxGetCpuSteppingInScratchpad (sbspSktId, 0, &SteppingInScratchPad);
  KtiInternalGlobal->PackageBspStepping[sbspSktId] = SteppingInScratchPad;

  KTIVAR->CurrentReset = KtiInternalGlobal->CurrentReset;
  KTIVAR->OutSystemRasType = UNDEFINED_RAS;
  KTIVAR->MaxSupportedM2m  = GetMaxM2mSupported (KtiInternalGlobal);
  UpdateCpuInfoInKtiInternal (KtiInternalGlobal, sbspSktId);

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  SBSP Socket: %u   Ways: 0x%02X   Ras: 0x%02X   Stepping: 0x%02X   Chop: %a   DieCount:  %d",
                       sbspSktId, KtiInternalGlobal->SocketWays[sbspSktId], KtiInternalGlobal->RasType[sbspSktId],
                       KtiInternalGlobal->PackageBspStepping[sbspSktId],
                       ((KtiInternalGlobal->ChopType[sbspSktId] == TypeXcc) ? "XCC" :
                        ((KtiInternalGlobal->ChopType[sbspSktId] == TypeHcc) && (!IsCpuAndRevision (CPU_SPRSP, REV_UX))) ? "HCC" :
                        (KtiInternalGlobal->ChopType[sbspSktId] == TypeMcc) ? "MCC" :
                        (KtiInternalGlobal->ChopType[sbspSktId] == TypeLcc) ? "LCC" :
                        ((KtiInternalGlobal->ChopType[sbspSktId] == TypeUcc) && (IsCpuAndRevision (CPU_SPRSP, REV_UX))) ? "UCC" : " "),
                        KtiInternalGlobal->DieCount[sbspSktId]
                     ));

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Total Chas: %02u   Cha List Map:\n", KtiInternalGlobal->TotCha[sbspSktId]));
  for (i = 0; i < MAX_CHA_MAP; i++) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "   Cha List[%d]: 0x%08X\n", i, KtiInternalGlobal->ChaList[sbspSktId][i]));
  }
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Total M3Kti: %02u   Total KtiAgent: %02u   Total M2M: %02u M2M list map: 0x%08X\n",
                       KTIVAR->CpuInfo[sbspSktId].TotM3Kti, KtiInternalGlobal->TotKtiAgent[sbspSktId], KTIVAR->CpuInfo[sbspSktId].TotM2m, KTIVAR->CpuInfo[sbspSktId].M2mBitmap));
  //
  // Fill the Host common structure with the stack bus numbers assigned to each
  // populated socket so that the CSR access routines become usable for all sockets.
  // Note that the bus number for SBSP is filled by CPURC; so the CSR access routines can be
  // used for SBSP CSR space before we do this step.
  //
  // In reset flow, assume maximal configuration and initialize the output structure accordingly
  // If S0 is not the legacy socket, then assign the bus number such that Legacy Socket (i.e SBSP) gets bus range starting from 0
  //
  KTI_HOST_OUT_PTR->numCpus = MAX_SOCKET;
  SetSocketPresentBitMap (FLD_MASK (MAX_SOCKET, 0));
  BusBase = 0;
  BusSize = 256 / SI_MAX_CPU_SOCKETS;

  SetSocketFirstBus (sbspSktId, (UINT8)BusBase);
  BusBaseIdx = 0;
  for (Idx2 = 0; Idx2 < MAX_LOGIC_IIO_STACK; Idx2++) {

    //
    // Sync the SBSP BUSNO to LEGACY_BUS_NO in ProcessorStartupUncore.inc
    //
    if (Idx2 != 0) {
      BusBaseIdx = Idx2 | 0x10;
    }
    SetStackBus (sbspSktId, Idx2, (UINT8)BusBase + BusBaseIdx);
  }
  BusBase = BusBase + BusSize;
  SetSocketLastBus (sbspSktId, (UINT8)(BusBase - 1));

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (Idx1 != sbspSktId) {
      SetSocketFirstBus (Idx1, (UINT8)BusBase);
      for (Idx2 = 0; Idx2 < MAX_LOGIC_IIO_STACK; Idx2++) {
        SetStackBus (Idx1, Idx2, (UINT8)BusBase + Idx2);
      }
      BusBase = BusBase + BusSize;
      SetSocketLastBus (Idx1, (UINT8)(BusBase - 1));
    }
  }

  UpdateAndDisplayBusInformation (KtiInternalGlobal, GetSysCpuCsrAccessVar (), SocketData, TRUE);

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Assuming maximal config: TotCpus: %u  CpuList: 0x%02X ", KTI_HOST_OUT_PTR->numCpus, GetSocketPresentBitMap ()));

  //
  // Fpga enable?
  //
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (KTISETUP->KtiFpgaEnable[Idx1] != KTI_DISABLE) {
      if (KTISETUP->PhyLinkPerPortSetting[Idx1].Link[XEON_FPGA_PORT].KtiPortDisable == FALSE) {
        KtiInternalGlobal->FpgaEnable[Idx1] = KTI_ENABLE;
      }
    }
  }

  //
  // if SBSP has no KTI Agents, set KtiPortCnt to zero. Do no programming of KTI port registers
  //
  if (KtiInternalGlobal->TotKtiAgent[sbspSktId] == 0) {
    KtiInternalGlobal->KtiPortCnt = 0;
    SetKtiPortCnt (0);
  }

  MaxKtiPort = GetKtiPortCnt ();

  KtiInternalGlobal->EvMode = PcdGet8 (PcdEvMode);

  if (HybridSystemLevelEmulationEnabled ()) {
    if (GetKtiPortCnt () != 0) {
      MaxKtiPort = 1;
    }
  }

  if (Is16EntryUpiRoutingTableSupported ()) {
    KtiInternalGlobal->UpiRoutingMode = UpiRoutingModeB2;
  } else {
    KtiInternalGlobal->UpiRoutingMode = UpiRoutingModeA;
  }
  RoutingModeStr = (KtiInternalGlobal->UpiRoutingMode < UpiRoutingModeMax)? \
                    UpiRoutingModeStr[KtiInternalGlobal->UpiRoutingMode]: UpiRoutingModeStr[UpiRoutingModeMax];

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Default UpiRoutingMode: (%a) %d", RoutingModeStr, KtiInternalGlobal->UpiRoutingMode));

  //
  // Logic to detect the system KTI link speed mode (slow or fast). Note that all the valid links
  // must be in same speed mode. i.e some can't be in full speed mode while the rest are in slow mode.
  // Find a valid link of SBSP to read the KtiMisc and PhCis registers.
  //
  KTIVAR->KtiCurrentLinkSpeedMode = GetKtiCurrentLinkSpeedMode (KtiInternalGlobal, sbspSktId, MaxKtiPort);

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Reset Type: %a   Link Speed: %a", (KtiInternalGlobal->CurrentReset == POST_RESET_POWERGOOD) ? "Cold Reset" : "Warm Reset",
     ((KTIVAR->KtiCurrentLinkSpeedMode == KTI_LINK_SLOW_SPEED_MODE) ? "Slow Speed" : "Fast Speed")));

  return KTI_SUCCESS;
}

/**
  Update Kti Internal Global data.

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param SocktId           - Socket ID

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
UpdateCpuInfoInKtiInternal (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocktId
  )
{
  //
  // Get the CPU's Die Count
  //
  KtiInternalGlobal->DieCount[SocktId] = PcuGetDieCount (SocktId);
  KTIVAR->CpuInfo[SocktId].DieCount = KtiInternalGlobal->DieCount[SocktId];

  //
  // SPR UCC detection
  //
  if (IsCpuAndRevision (CPU_SPRSP, REV_UX)) {
    if (KTIVAR->CpuInfo[SocktId].DieCount == 0) {
      KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n WARNING:  Unexpected Die Count %x on Soc %x in SPR UCC! ", KtiInternalGlobal->DieCount[SocktId], SocktId));
    }
    KTIVAR->UpimdfPortBitmap[SocktId] |= (1 << UPIMDF_PORT_NUMBER_SPRUCC);
  }

  //
  // Get Max Kti Agent and Max M3kti which is based on CPU type and stepping
  //
  UpdateMaxKtiAgentAndM3KtiCount (KtiInternalGlobal, SocktId);

  //
  // Socket Ways
  //
  GetSocketWays (SocktId, &KtiInternalGlobal->SocketWays[SocktId]);

  //
  // Get the number of and list of Chas present
  //
  GetChaCountAndList (KtiInternalGlobal, SocktId);
  KTI_ASSERT ((KtiInternalGlobal->TotCha[SocktId] != 0), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_89);

  //
  // Get the number of M2Mem present
  //
  GetM2MemCount (KtiInternalGlobal, SocktId, &(KTIVAR->CpuInfo[SocktId].TotM2m));

  //
  // Update SharedKtiPort0And1 information
  //
  KTIVAR->SharedKtiPort0And1 = FALSE;
  if (IsSkxFamilyCpu (KtiInternalGlobal->CpuType)) {
    KTIVAR->SharedKtiPort0And1 = TRUE;          //only SKX M3KTI share port 0 and port 1
  }

  //
  // Get the number of M3KTI present
  //
  KTIVAR->CpuInfo[SocktId].TotM3Kti = GetM3KtiCount (SocktId, KTIVAR->MaxM3Kti[SocktId]);

  //
  // Get the number of KtiAgents present
  //
  GetKtiAgentCount (KtiInternalGlobal, SocktId, &KtiInternalGlobal->TotKtiAgent[SocktId]);
  KTIVAR->KtiPortPresentBitmap[SocktId] = KtiInternalGlobal->KtiPortPresentBitmap[SocktId];

  //
  // Get the Ras capability
  //
  GetRasType (SocktId, KtiInternalGlobal);

  //
  // Only if all parts have Advanced Ras features do we have online/offline capability
  //
  if ((KTIVAR->OutSystemRasType == ADVANCED_RAS) && (KTISETUP->KtiCpuSktHotPlugEn == TRUE)){
    KtiInternalGlobal->KtiCpuSktHotPlugEn = TRUE;
  } else {
    KtiInternalGlobal->KtiCpuSktHotPlugEn = FALSE;
  }

  //
  // Collect the CPU segment type based on presence of MCP stack(s)
  //
  GetCpuSegmentType (KtiInternalGlobal, SocktId);

  return KTI_SUCCESS;

}

/**
  Prime the KTIRC data structure based on input option and system topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
PrimeHostStructure (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8      Ctr;
  UINT8      CpuType;
  UINT32     SlaveDieBitMap;

  CpuType = KtiInternalGlobal->CpuType;
  //
  // Update the sysHost common structure with the topology info
  //
  KTI_HOST_OUT_PTR->numCpus = KtiInternalGlobal->TotCpu;
  KTI_ASSERT (( KTI_HOST_OUT_PTR->numCpus <= MAX_SOCKET), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY); // Defensive check for data structure overflows
  SetSocketPresentBitMap (KtiInternalGlobal->CpuList);

  SlaveDieBitMap = 0;
  if (GetDieCount (KtiInternalGlobal->SbspSoc) == TWO_DIE_IN_SOCKET) {
    for (Ctr = 0; Ctr < MAX_SOCKET; Ctr++) {
      if ((Ctr == KtiInternalGlobal->SbspSoc) || ((KtiInternalGlobal->CpuList & (1 << Ctr)) == 0)) {
        continue;
      }
      //
      // Socket is present. Slave Die on dual-die package has odd Socket ID.
      //
      if ((Ctr & BIT0) != 0) {
        SlaveDieBitMap |= (1 << Ctr);
      }
    }
  }
  SetSlaveDieBitMap (SlaveDieBitMap);

  for (Ctr = 0; Ctr < MAX_SOCKET; Ctr++) {
    if ((KtiInternalGlobal->CpuList & (1 << Ctr)) == 0) {
      SetSocketFirstBus (Ctr, 0);
      SetSocketLastBus (Ctr, 0);
    }
    CopyMem ((UINT8*)KTIVAR->CpuInfo[Ctr].ChaList,
             (UINT8*)KtiInternalGlobal->ChaList[Ctr],
             sizeof (KtiInternalGlobal->ChaList[Ctr]));
    KTIVAR->CpuInfo[Ctr].TotCha = KtiInternalGlobal->TotCha[Ctr];
  }

  //
  // Update KTIRC output structure
  //

  //
  // The Host setup structure is a const structure. If the options provided in setup structure is
  // invalid for the given topology KTIRC will force supported value for that option. To facilitate this
  // we copy those kind of options into Host kti structure and modify it to match the topology.
  //
  KTIVAR->OutLegacyVgaSoc= KTISETUP->LegacyVgaSoc;
  KTIVAR->OutLegacyVgaStack= KTISETUP->LegacyVgaStack;
  KTIVAR->OutKtiLinkSpeed = KTISETUP->KtiLinkSpeed;
  KTIVAR->OutKtiLinkL0pEn = KTISETUP->KtiLinkL0pEn;
  if ((KTISETUP->KtiLinkL0pEn == KTI_AUTO) && IsCpuAndRevision (CPU_SPRSP, REV_AX)) {
    KTIVAR->OutKtiLinkL0pEn = KTI_DISABLE;
  }
  KTIVAR->OutKtiLinkL1En = KTISETUP->KtiLinkL1En;
  KTIVAR->OutIsocEn = KTISETUP->isocEn;
  KTIVAR->OutKtiCrcMode = KTISETUP->KtiCrcMode;
  KTIVAR->OutKtiFailoverEn = KTISETUP->KtiFailoverEn;
  KTIVAR->OutSysDirectoryModeEn = KTI_ENABLE;       // The Directory Mode is enabled by default (AUTO) Mode.
                                                    // In AUTO mode, DirectoryMode feature enabled\disabled [in PrimeHostStructureMisc ()] based on system configuration

  //
  // Initialize the KTIRC global variables and check the Host input structure to make sure it is
  // consist with the system topology.
  //
  KTIVAR->OutKtiCpuSktHotPlugEn = KtiInternalGlobal->KtiCpuSktHotPlugEn;

  //
  // Make sure the legacy VGA range target is valid socket
  //
  if ((KTIVAR->OutLegacyVgaSoc >= MAX_SOCKET) || (SocketData->Cpu[KTIVAR->OutLegacyVgaSoc].Valid != TRUE)) {
    KtiLogWarning (0xFF, 0xFF, 0xFF, WARN_VGA_TARGET_SOC_NOT_PRESENT);
    KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n WARNING:  Legacy VGA range target not populated; assigning the range to Legacy CPU socket. "));
    KTIVAR->OutLegacyVgaSoc = KtiInternalGlobal->SbspSoc; // Make Legacy socket as the VGA range target
  }

  //
  // Only SPR support the txeq adaptation.
  // For the KtiAdaptationEn to FALSE if it is other silicon
  //
  if (KTISETUP->KtiAdaptationEn == KTI_AUTO) {
    KTIVAR->OutKtiAdaptationEnable = KTI_DISABLE;
  } else {
    KTIVAR->OutKtiAdaptationEnable = KTISETUP->KtiAdaptationEn;
  }
  if (!IsSprFamilyCpu(CpuType)) {
    KTIVAR->OutKtiAdaptationEnable = KTI_DISABLE;
  }

  //
  // Currently, only simulation the flow for SPR in KtiRcSim, as the HSPHY registers was not released.
  // Only to check if we need to do adaptation:
  //   when the setup option is enabled & not S3 resume & not single socket & in skip warm reset flow.
  //
  if ((KTIVAR->OutKtiAdaptationEnable == KTI_ENABLE) &&
      (GetSysBootMode () != S3Resume) &&
      (KtiInternalGlobal->TotCpu != 1)) {
    CheckIfAdaptationNeeded (KtiInternalGlobal);
  }

  //
  // Make sure the legacy VGA range target is valid stack
  //
  if ((KTIVAR->OutLegacyVgaStack >= MAX_IIO_STACK) || (!StackPresent (KTIVAR->OutLegacyVgaSoc, KTIVAR->OutLegacyVgaStack))) {
    KtiLogWarning (0xFF, 0xFF, 0xFF, WARN_VGA_TARGET_STACK_NOT_PRESENT);
    KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n WARNING:  Legacy VGA range target stack not present; assigning the range to Legacy Cstack. "));
    KTIVAR->OutLegacyVgaStack = IIO_CSTACK; // Make Legacy stack as the VGA range target
  }


  //
  // Process Phy/Link?
  //
  KtiInternalGlobal->ProgramPhyLink = FALSE;

  //
  // Process if Multisocket or Hotplug enabled
  //
  if ((KtiInternalGlobal->TotCpu != 1) || (KTIVAR->OutKtiCpuSktHotPlugEn == TRUE)) {
    KtiInternalGlobal->ProgramPhyLink = TRUE;
  }

  UpdateMmcfgMmiolBaseSize (KtiInternalGlobal);

  PrimeHostStructureIsoc (SocketData, KtiInternalGlobal);

  PrimeHostStructureStack (SocketData, KtiInternalGlobal);

  if (IsSkxFamilyCpu (CpuType) || IsIcxFamilyCpu (CpuType) || IsSprFamilyCpu (CpuType) || IsGnrSrfFamilyCpu (CpuType)) {
      PrimeHostStructureMisc (SocketData, KtiInternalGlobal);
  } else {
    KtiCheckPoint (0xFF, 0xFF, 0xFF, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Unknown Cpu Type"));
    KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
  }

  PrimeHostStructureFpga (SocketData, KtiInternalGlobal);

  PrimeHostStructurePhyLinkLayer (SocketData, KtiInternalGlobal);

  KTIVAR->OutIsRouteThrough = KtiInternalGlobal->IsRouteThrough;

  //
  // Interleave size = 2**(size*2) GB.
  //
  KtiInternalGlobal->MmiohTargetSize = KTISETUP->mmiohSize;
  if (KtiInternalGlobal->MmiohTargetSize > MMIOH_MAX_GRANTY) {
    KtiLogWarning (0xFF, 0xFF, 0xFF, WARN_MMIOH_SIZE_UNSUPPORTED);
    KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n WARNING:  MMIOH size requested is invalid. Forcing it to 1GB.\n"));
    KtiInternalGlobal->MmiohTargetSize = 0;
  }

  Init_IRQ_Threshold (KtiInternalGlobal);

  Init_TOR_Threshold_Loctorem (KtiInternalGlobal);

  KtiInternalGlobal->TSCSyncEn = KTISETUP->TscSyncEn;

  DetectMaxAddress (SocketData, KtiInternalGlobal);

  //
  // Process Mdfis training enabling
  //
  if (KTISETUP->DfxParm.DfxMdfisAdvancedTrainingEn < KTI_AUTO) {
    KtiInternalGlobal->DfxMdfisAdvancedTrainingEn = KTISETUP->DfxParm.DfxMdfisAdvancedTrainingEn;
  } else {
    if (IsCpuAndRevision (CPU_SPRSP, REV_AX)) {
      //SPR A0 did not support Mdfis Advanced Training yet, disable for now.
      KtiInternalGlobal->DfxMdfisAdvancedTrainingEn = KTI_DISABLE;
    } else {
      KtiInternalGlobal->DfxMdfisAdvancedTrainingEn = KTI_ENABLE;
    }
  }

  if (KTISETUP->DfxParm.DfxCxlVid < KTI_AUTO) {
    KTIVAR->OutDfxCxlVid = KTISETUP->DfxParm.DfxCxlVid;
  } else {
    KTIVAR->OutDfxCxlVid = 1;
  }

  return KTI_SUCCESS;
}

/**
  Prime the KTIRC data structure based on input option and system topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
PrimeHostStructureIsoc (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  BOOLEAN     bAepDimmPresent;


  if (KtiInternalGlobal->CpuType == CPU_SNR ||
      KtiInternalGlobal->CpuType == CPU_TNR ||
      KtiInternalGlobal->CpuType == CPU_ICXD) {
    KTIVAR->OutIsocEn = FALSE;
    return KTI_SUCCESS;
  }

  //
  // Allows Isoc in 1S and 2S2Link
  //
  if ((KTIVAR->SysConfig != SYS_CONFIG_1S && KTIVAR->SysConfig != SYS_CONFIG_2S2L)) {
    KTIVAR->OutIsocEn = FALSE;
  }

  //
  // Isoc not allowed in hot-pluggable systems
  //
  if (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE) {
    KTIVAR->OutIsocEn = FALSE;
  }

  //
  // If AEP DIMM present, disable Isoc
  //
  UBoxCheckAepDimmPresentFlagSetInScratchpad (KtiInternalGlobal->SbspSoc, 0, &bAepDimmPresent);
  if (bAepDimmPresent) {
    KTIVAR->OutIsocEn = FALSE;
  }

  return KTI_SUCCESS;
}

/**
  Prime the KTIRC data structure based on input option and system topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
PrimeHostStructureMisc (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  BOOLEAN                     NumaAware;
  UINT8                       CpuType;
  BOOLEAN                     SncModeFailReccovery;
  BOOLEAN                     PrefetchModeFailRecovery;
  UINT8                       SbspSocketId;

  NumaAware   = IsNumaAware ();
  CpuType     = KtiInternalGlobal->CpuType;
  SbspSocketId = KtiInternalGlobal->SbspSoc;

  //
  // Initialize to setup input
  //
  //
  // Directory mode can be enabled based on setup option if multi-socket topology
  // OSB should be enabled only if multi-socket and Directory mode enabled
  //

  if (KTISETUP->KtiPrefetchEn < KTI_AUTO) {
    KTIVAR->OutKtiPrefetch = KTISETUP->KtiPrefetchEn;
  } else {
    KTIVAR->OutKtiPrefetch = KTI_ENABLE;
  }
  if (KTISETUP->XptRemotePrefetchEn < KTI_AUTO) {
    KTIVAR->OutXptRemotePrefetch = KTISETUP->XptRemotePrefetchEn;
  } else {
    if (IsCpuAndRevisionOrLater (CPU_ICXSP, REV_L0)) {
      KTIVAR->OutXptRemotePrefetch = KTI_ENABLE;
    } else {
      KTIVAR->OutXptRemotePrefetch = KTI_DISABLE;
    }
  }

  if (KTIVAR->SysConfig == SYS_CONFIG_1S) {
    KTIVAR->OutKtiPrefetch = KTI_DISABLE;
    KTIVAR->OutXptRemotePrefetch = KTI_DISABLE;
    KtiInternalGlobal->SysOsbEn = KTI_DISABLE;
    KTIVAR->OutSysDirectoryModeEn = KTI_DISABLE;
  } else {
    //
    // DirectoryModeEn = User Setup knob selection
    // SPR_C0 revision and later version silicon does not need below [Override]
    // [Override] if DirectoryModeEn is Auto, DirectoryModeEn = !(MkTmeEn)
    //
    if (KTISETUP->DirectoryModeEn < KTI_AUTO) {
      KTIVAR->OutSysDirectoryModeEn = KTISETUP->DirectoryModeEn;
    } else if (IsSkxFamilyCpu (CpuType) || IsIcxFamilyCpu (CpuType) || IsCpuAndRevision (CPU_SPRSP, REV_AX) || \
               IsCpuAndRevision (CPU_SPRSP, REV_BX)) {
      KTIVAR->OutSysDirectoryModeEn = (KTISETUP->KtiInEnableMktme == KTI_ENABLE)? KTI_DISABLE: KTI_ENABLE;
    }
#ifdef DDR5_SUPPORT
    //
    // WA for BRS: force directory mode to disabled.
    //
    if (IsIcxFamilyCpu (CpuType)) {
      KTIVAR->OutSysDirectoryModeEn = KTI_DISABLE;
    }
#endif
    if (KTIVAR->OutSysDirectoryModeEn == KTI_ENABLE) {
      KtiInternalGlobal->SysOsbEn = KTI_ENABLE;
    } else {
      KtiInternalGlobal->SysOsbEn = KTI_DISABLE;
    }
  }

  if (KTISETUP->XptPrefetchEn == KTI_AUTO) {
    KTIVAR->OutXptPrefetch = KTI_ENABLE;
  } else {
    KTIVAR->OutXptPrefetch = KTISETUP->XptPrefetchEn;
  }

  if (KTIVAR->HbmSku) {
    KTIVAR->OutXptPrefetch = KTI_DISABLE;
    KTIVAR->OutKtiPrefetch = KTI_DISABLE;
    KTIVAR->OutXptRemotePrefetch = KTI_DISABLE;
  }

  if (CpuType == CPU_CPX) {
    KTIVAR->OutD2KCreditConfig     = KTISETUP->D2KCreditConfig;
  }

  //
  // Snoop Throttle config (or snoop threshold)
  //
  KTIVAR->OutSnoopThrottleConfig = KTI_STC_DIS;
  if (KTISETUP->SnoopThrottleConfig == KTI_STC_AUTO) {
    if ((KTIVAR->SysConfig == SYS_CONFIG_8S) && (KtiInternalGlobal->EightSocketTopology == Topo8SDoublePinwheel)) {
      //
      // Set Snnop throttle default setting for 8S DPW topology to medium
      //
      KTIVAR->OutSnoopThrottleConfig = KTI_STC_MED;
    }
  } else {
    if (CpuType == CPU_CPX || IsCpuAndRevisionOrLater (CPU_ICXSP, REV_L0)) {
      KTIVAR->OutSnoopThrottleConfig = KTISETUP->SnoopThrottleConfig;
    }
  }

  //
  // ICX-SP has PMON discovery disabled by default
  //
  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    KTIVAR->OutPmonConfig = KTI_DISABLE;
  }

  //
  //SPR is still defining the register values to be programmed in PMON discovery units.
  //Only enabled if explicitly done and will program using preeliminary values.
  //
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL) || IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    KTIVAR->OutPmonConfig = KTI_DISABLE;
  }

  if (IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    //
    // SNR has PMON discovery disabled by default
    //
    KTIVAR->OutPmonConfig         = KTI_PMON_CONFIG_DISABLE;
    //
    // SNR has PMON access control restricted by default
    //
    KTIVAR->OutM2IosfPmonAccessControl  = KTI_PMON_ACC_CTRL_DISABLE;
  }

  if (IsCpuAndRevision (CPU_SNR, REV_ALL) ||
      IsCpuAndRevision (CPU_TNR, REV_ALL) ||
      IsCpuAndRevision(CPU_ICXD, REV_ALL)
      ) {
    KTIVAR->OutDfxIioStackEn = KTISETUP->DfxParm.DfxIioStackEn;
    KTIVAR->OutDfxHqmEn = KTISETUP->DfxParm.DfxHqmEn;
  } else {
    KTIVAR->OutDfxIioStackEn = DFX_IIO_STACK_EN_AUTO;
    KTIVAR->OutDfxHqmEn = KTI_AUTO;
  }

  if (KTISETUP->DfxParm.DfxPmonConfig < KTI_PMON_CONFIG_AUTO) {
    KTIVAR->OutPmonConfig = KTISETUP->DfxParm.DfxPmonConfig;
  }

  if (KTISETUP->DfxParm.DfxM2IosfPmonAccessControl < KTI_PMON_ACC_CTRL_AUTO) {
    KTIVAR->OutM2IosfPmonAccessControl = KTISETUP->DfxParm.DfxM2IosfPmonAccessControl;
  }

  if (KTISETUP->DfxParm.DfxOsbEn < KTI_AUTO) {
    KtiInternalGlobal->SysOsbEn = KTISETUP->DfxParm.DfxOsbEn;
  }

  //
  // IODC only in Directory enabled configurations
  //
  if (KTIVAR->OutSysDirectoryModeEn == KTI_DISABLE) {
    KTIVAR->OutIoDcMode = IODC_DISABLE;
  } else {
    KTIVAR->OutIoDcMode = KTISETUP->IoDcMode;
    if (KTIVAR->OutIoDcMode == IODC_AUTO) {
      if (IsIcxFamilyCpu (KtiInternalGlobal->CpuType) ||
          IsSprFamilyCpu (KtiInternalGlobal->CpuType) ||
          ((CpuType == CPU_CPX) && (KTIVAR->SysConfig == SYS_CONFIG_4S) && !KtiInternalGlobal->Is4SRing && KtiInternalGlobal->IsDoubleLinks)) {
        KTIVAR->OutIoDcMode = IODC_EN_REM_INVITOM_AND_WCILF;
      }
    }
  }

  if ((IsIcxFamilyCpu (KtiInternalGlobal->CpuType) ||
      IsSprFamilyCpu (KtiInternalGlobal->CpuType)) &&
      (KTISETUP->StaleAtoSOptEn == KTI_AUTO)) {
    KTIVAR->OutStaleAtoSOptEn = KTI_DISABLE;
  } else {
    KTIVAR->OutStaleAtoSOptEn = KTISETUP->StaleAtoSOptEn;
  }

  if ((CpuType == CPU_CPX) && (KTISETUP->StaleAtoSOptEn == KTI_AUTO)) {
    if (((KTIVAR->SysConfig == SYS_CONFIG_4S) && !KTIVAR->Is4SRing && KtiInternalGlobal->IsDoubleLinks && (GetDieCount (KtiInternalGlobal->SbspSoc) == ONE_DIE_IN_SOCKET)) ||
        ((KTIVAR->SysConfig == SYS_CONFIG_8S) && (KtiInternalGlobal->EightSocketTopology == Topo8SDoublePinwheel))) {
      //
      // StaleAtoS setting is default to enable in 8S DPW and 4S6L fully connect topologies
      //
      KTIVAR->OutStaleAtoSOptEn = KTI_ENABLE;
    }
  }

  KtiInternalGlobal->DualLinksInterleavingMode = KTI_AUTO;
  //
  // DfxDualLinksInterleavingMode is only valid in 2S2L
  //
  if (KTIVAR->SysConfig == SYS_CONFIG_2S2L && KTISETUP->DfxParm.DfxDualLinksInterleavingMode < KTI_AUTO) {
    KtiInternalGlobal->DualLinksInterleavingMode = KTISETUP->DfxParm.DfxDualLinksInterleavingMode;
  }

  KtiInternalGlobal->SnoopFanoutEn = FALSE;

  //
  // Enable  4SRing and beyond > 4S
  //
  if (((KTIVAR->SysConfig == SYS_CONFIG_4S) && (KtiInternalGlobal->Is4SRing)) || (KTIVAR->SysConfig > SYS_CONFIG_4S)) {
    KtiInternalGlobal->SnoopFanoutEn = TRUE;
  }

  //
  // If DFX snoop fanout is not "Auto", set it to the requested value
  //
  if (KTISETUP->DfxParm.DfxSnoopFanoutEn < KTI_AUTO) {
    KtiInternalGlobal->SnoopFanoutEn = KTISETUP->DfxParm.DfxSnoopFanoutEn;
  }

  KtiInternalGlobal->DualLinksEn = TRUE;

  KtiInternalGlobal->SnoopFanoutChaInterleaveEn = FALSE;

  if ((KTIVAR->SysConfig == SYS_CONFIG_8S) && (KtiInternalGlobal->EightSocketTopology == Topo8SDoublePinwheel)) {
    if (KtiInternalGlobal->TotCpu == 6) {
      //
      // 6S duallink config degraded from 8S DPW - force snoop fanout to disable due to limitation in
      // current BIOS implementation of snoop fanout route through for 3-hop away destination node
      //
      KtiInternalGlobal->SnoopFanoutEn = FALSE;
    }

    if (KtiInternalGlobal->SnoopFanoutEn == TRUE) {
      //
      // SnoopFanoutChaInterleaveEn knob is a subset of SnoopFanoutEn knob. It is a don't care if SnoopFanoutEn = FALSE.
      //
      KtiInternalGlobal->SnoopFanoutChaInterleaveEn = TRUE;

      if (KTISETUP->DfxParm.DfxSnoopFanoutChaInterleaveEn < KTI_AUTO) {
        KtiInternalGlobal->SnoopFanoutChaInterleaveEn = KTISETUP->DfxParm.DfxSnoopFanoutChaInterleaveEn;
      }
    }
  }

  //
  // VN1 is not supported in EP
  //
  KTIVAR->OutVn1En = FALSE;
  if ((KTIVAR->SysConfig == SYS_CONFIG_8S) && (KtiInternalGlobal->EightSocketTopology != Topo8SAlmostFullyConnect)) {
    KTIVAR->OutVn1En = TRUE;
  }

  //
  // If DFX VN1 is not "Auto", set it to DFX requested value.
  //
  if (KTISETUP->DfxParm.DfxVn1En < KTI_AUTO) {
    KTIVAR->OutVn1En = KTISETUP->DfxParm.DfxVn1En;
  }

  //
  // Try to enable SNC according to knob setting and hardware support.
  //
  if (NumaAware) {
    if (GetMaxImc () > 2) {
      KTIVAR->OutSncEn = KTISETUP->SncEn;
    } else {
      if (KTISETUP->SncEn == KTI_AUTO) {
        KTIVAR->OutSncEn = 0;
      } else if (KTISETUP->SncEn == KTI_DISABLE) {
        KTIVAR->OutSncEn = 0;
      } else if (KtiInternalGlobal->CpuType == CPU_ICXD) {
        if (GetMaxImc () == 1) {
          //
          // ICXD-LCC only has 1 MC so SNC enable is not possible.
          //
          KTIVAR->OutSncEn = 0;
        } else {
          //
          // ICXD-HCC has 2 MC so SNC enable is possible.
          //
          KTIVAR->OutSncEn = KTI_SNC2;
        }
      } else {
        KTIVAR->OutSncEn = KTISETUP->SncEn;
      }
    }
    if (KTIVAR->HbmSku && KTIVAR->OutSncEn == KTI_SNC2) {
      KTIVAR->OutSncEn = 0; // HBM Sku, SNC2 not supported
    }
  } else {
    // UMA enabled, need to disable prefetch
    KTIVAR->OutXptPrefetch = KTI_DISABLE;
    KTIVAR->OutKtiPrefetch = KTI_DISABLE;
    KTIVAR->OutXptRemotePrefetch = KTI_DISABLE;
  }

  KTIVAR->OutUmaClustering = KTISETUP->UmaClustering;
  if (IsCpuAndRevision (CPU_ICXSP, REV_R0)) {
    KTIVAR->OutSncEn = 0;
    KTIVAR->OutUmaClustering = 0;
    KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n WARNING:  ICX-SP R0 does not support SNC or UMA. Forcing both to disabled.\n"));
  }
  if ((IsCpuAndRevision (CPU_ICXSP, REV_ALL)) && (KtiInternalGlobal->TotCha[KtiInternalGlobal->SbspSoc] == ICXSP_UNSUPPORTED_CHA_COUNT)) {
    KTIVAR->OutSncEn = 0;
    KTIVAR->OutUmaClustering = 0;
    KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n WARNING:  ICX-SP does not support 38CHA + hemisphere/SNC. Forcing both to disabled.\n"));
  }

  if (KTIVAR->OutSncEn > 0 || !IsClusterXorEnable ()) {
    KTIVAR->OutUmaClustering = 0;
  }

  //
  // Need NUMA enabled and MMCFG Base aligned on 1GB to enable SNC
  //
  if (KTIVAR->OutSncEn) {
    if (NumaAware && ((KTIVAR->mmCfgBase % 0x40000000) == 0) &&
        (KtiInternalGlobal->DualLinksInterleavingMode != DFX_DUAL_LINK_INTLV_MODE_CHA_INTLV)) {
        KTIVAR->OutSncPrefetchEn = (KTIVAR->OutKtiPrefetch | KTIVAR->OutXptPrefetch | KTIVAR->OutSncEn | KTIVAR->OutXptRemotePrefetch);
    } else {
      if ((KTIVAR->mmCfgBase % 0x40000000) != 0) {
        KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n WARNING:  MMCFG Base is not aligned on 1G boundary. Forcing SNC to disabled.\n"));
      }
      KTIVAR->OutSncEn = 0;
      KTIVAR->OutSncPrefetchEn = 0;
    }
  } else {
    if (((KTIVAR->mmCfgBase % 0x40000000) == 0) &&
        (KtiInternalGlobal->DualLinksInterleavingMode != DFX_DUAL_LINK_INTLV_MODE_CHA_INTLV)) {
      KTIVAR->OutSncPrefetchEn = (KTIVAR->OutKtiPrefetch | KTIVAR->OutXptPrefetch | KTIVAR->OutUmaClustering | KTIVAR->OutXptRemotePrefetch);
    } else {
      KTIVAR->OutSncPrefetchEn = 0;
    }
  }

  //
  // In SNR TNT bits for SNC ind and SNC cluster are hardcoded, and pcode doesn't program any snc clustering related registers.
  // BIOS should program ms2idi snc_config/snc_base register, even when xpt prefetch is disabled.
  //
  if (KtiInternalGlobal->CpuType == CPU_SNR || KtiInternalGlobal->CpuType == CPU_TNR) {
    KTIVAR->OutSncPrefetchEn |= 1;
  }

  //
  // s1504535040: SNC couldn't be enabled if enabled slices are less than 12.
  //              SNC enable will have reduced number of LLCs. To determine the NEM size,
  //              only half of the LLCs of the sbsp should be considered.
  //              5MB NEM should have at least 6 LLCs to provide enough size of cache
  //
  if (GetMaxImc () > 2) {
    //
    // SNC2 and SNC4
    //
    if (KTIVAR->OutSncEn != KTI_DISABLE) {
      if ((KtiInternalGlobal->TotCha[KtiInternalGlobal->SbspSoc] / KTIVAR->OutSncEn) < 6) {
        KTIVAR->OutSncEn = 0;
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Disable SNC because the number of Core is not enough!!!\n"));
      }
    }
    //
    // SNC requires the total number of CHA could be divided evenly
    //
    if ( KTIVAR->OutSncEn == 4) {
      if ((KtiInternalGlobal->TotCha[KtiInternalGlobal->SbspSoc] % KTIVAR->OutSncEn) != 0) {
        KTIVAR->OutSncEn = 0;
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Disable SNC because the number of CHA cannot be divided evenly!!!\n"));
      }
    }
  } else if (GetMaxImc () == 2) {
    if (KtiInternalGlobal->TotCha[KtiInternalGlobal->SbspSoc] < 12) {
      if (KTIVAR->OutSncEn == KTI_ENABLE || KTIVAR->OutSncEn == KTI_SNC2){
        KTIVAR->OutSncEn = 0;
        KTIVAR->OutSncPrefetchEn = 0;
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Disable SNC because the number of Core is not enough!!!\n"));
      }
    }
  } else if (GetMaxImc () == 1) {
    KTIVAR->OutSncEn = 0;
    KTIVAR->OutUmaClustering = 0;
  }

  CheckUmaBasedClusteringDowngrade (KtiInternalGlobal);

  //
  // SP7 bit 30 indicates snc failure during previous warm reset boot, then need to recovery back to SNC disabled mode
  // SP7 bit 20 indicates prefetch failure during previous warm reset boot, then need to recovery back to Prefetch disabled mode
  //
  UBoxCheckSncModeFailRecoveryFlagSetInScratchpad (KtiInternalGlobal->SbspSoc, 0, &SncModeFailReccovery);
  if (SncModeFailReccovery) {
    // recovery back to SNC disabled mode
    KTIVAR->OutSncEn = 0;         // disable OutSncEn for MRC usage
    KTIVAR->OutSncPrefetchEn = (KTIVAR->OutKtiPrefetch | KTIVAR->OutXptPrefetch | KTIVAR->OutUmaClustering | KTIVAR->OutXptRemotePrefetch);
  }

  UBoxCheckPrefetchModeFailRecoveryFlagSetInScratchpad (KtiInternalGlobal->SbspSoc, 0, &PrefetchModeFailRecovery);
  if (PrefetchModeFailRecovery) {
    // recovery back to prefetch disabled mode
    KTIVAR->OutKtiPrefetch = 0;
    KTIVAR->OutXptPrefetch = 0;
    KTIVAR->OutXptRemotePrefetch = 0;
    KTIVAR->OutSncPrefetchEn = (KTIVAR->OutSncEn | KTIVAR->OutUmaClustering);
  }

  if (NumaAware && (KTIVAR->OutSncPrefetchEn == 0)) {
    KTIVAR->OutKtiPrefetch = KTI_DISABLE;
    KTIVAR->OutXptPrefetch = KTI_DISABLE;
    KTIVAR->OutXptRemotePrefetch = KTI_DISABLE;
  }

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n   OutSncPrefetchEn=%x, OutSncEn=%x\n", KTIVAR->OutSncPrefetchEn, KTIVAR->OutSncEn));

  //
  //If Rbt is enabled, d2c or d2k can be enabled by default
  //If Rbt is disabled, d2c and d2k must be disabled
  //It is illegal to enable D2C in multi-socket systems with dir-disabled. (from SysDefature0 definition)
  //It is illegal to enable D2K in multi-socket systems with dir-disabled.
  //
  if (KTIVAR->OutSysDirectoryModeEn == KTI_ENABLE) {
    KTIVAR->OutHitMeEn = 1;
    KtiInternalGlobal->D2cEn = 1;
    KtiInternalGlobal->D2kEn = 1;
  } else {
    KTIVAR->OutHitMeEn = 0;
    KtiInternalGlobal->D2cEn = 0;
    KtiInternalGlobal->D2kEn = 0;
  }
  //
  // If Validation is requesting non-Auto for Hit Me, override it for all topologies
  //
  if (KTISETUP->DfxParm.DfxHitMeEn < KTI_AUTO) {
    KTIVAR->OutHitMeEn = KTISETUP->DfxParm.DfxHitMeEn;
  }

  if ((KTIVAR->OutVn1En == TRUE) && (CpuType == CPU_SKX || CpuType == CPU_CLX)) {
    KtiInternalGlobal->D2kEn = 0;
  }

  if (KTIVAR->SysConfig == SYS_CONFIG_1S) {
    KtiInternalGlobal->D2cEn = 1;
  }

  //
  // 5330853: M3KTI Egress zeroing out SNID for route-through BL SR-D packets
  //
  if (KtiInternalGlobal->DualLinksInterleavingMode == DFX_DUAL_LINK_INTLV_MODE_DISABLE_D2C) {
    KtiInternalGlobal->D2cEn = 0;
  }

  if (KTIVAR->SysConfig == SYS_CONFIG_8S) {
    if ((KtiInternalGlobal->EightSocketTopology == Topo8SDoublePinwheel) ||
        (KtiInternalGlobal->Topo8S2ClumpsInfo.LinkStatus == Topology8S2ClumpsDoubleLinked)) {
      //
      // Disable remote D2C
      //
      KtiInternalGlobal->D2cEn = 0;
    }
  }

  if ((KTIVAR->SysConfig == SYS_CONFIG_4S) && KtiInternalGlobal->Is4SRing && KtiInternalGlobal->IsDoubleLinks) {
    //
    // Disable remote D2c
    //
    KtiInternalGlobal->D2cEn = 0;
  }

  if (KtiInternalGlobal->DualLinksInterleavingMode == DFX_DUAL_LINK_INTLV_MODE_CHA_INTLV &&
      KTIVAR->SysConfig == SYS_CONFIG_2S2L) {
    KtiInternalGlobal->D2kEn = 0;
  }

  if (KTIVAR->SysConfig == SYS_CONFIG_1S || IsCpuAndRevision (CPU_SKX, REV_ALL)) {
    KtiInternalGlobal->D2kEn = 0;
  }

  //
  // If it is requesting "Auto" for D2C, enable it for all topologies
  //
  if (KTISETUP->DfxParm.DfxD2cEn < KTI_AUTO) {
    KtiInternalGlobal->D2cEn = KTISETUP->DfxParm.DfxD2cEn;
  }
  if (KTISETUP->DfxParm.DfxD2kEn < KTI_AUTO) {
    KtiInternalGlobal->D2kEn = KTISETUP->DfxParm.DfxD2kEn;
  }

  //
  // Not enough UPI buffering for HBM controllers, need to disable D2K for HBM SKU
  //
  if (KTIVAR->HbmSku) {
    KtiInternalGlobal->D2kEn = FALSE;
  }

  //
  // Check whether UPI Affinity is supported
  //
  InitUpiAffinityStatus (KtiInternalGlobal);

  KTIVAR->OutD2kEn = KtiInternalGlobal->D2kEn;

  KTIVAR->DdrtQosMode = KTISETUP->DdrtQosMode;

  return KTI_SUCCESS;
}

/**
  Routine to program the final IOPORT SAD for the given socket

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuIoEntries (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  )
{
  UINT8     Stack, Socket, TargetIndex, NumberTargets;
  UINT32    TmpTarget = 0;
  UINT32    IoTgtLst[MAX_SAD_TARGETS];

  ZeroMem (IoTgtLst, sizeof(UINT32) * MAX_SAD_TARGETS);
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Socket present?
    //
    if (SocketData->Cpu[Socket].Valid == FALSE) {
      continue;
    }

    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      //
      // Stack present?
      //
      if (!StackPresent (Socket, Stack)) {
        continue;
      }

      //
      // Build up the 4 target register contents
      //
      if (KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].IoBase < KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].IoLimit) {
        TargetIndex = (UINT8)(KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].IoBase / SAD_LEG_IO_GRANTY);
        NumberTargets = (UINT8)((KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].IoLimit + 1 - KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].IoBase) / SAD_LEG_IO_GRANTY);
        while ((TargetIndex < MAX_SAD_TARGETS) && (NumberTargets > 0)) {
          if (Socket == SocId) {
            TmpTarget = (Stack | SOCKET_LOCAL_FLAG);
          } else {
            TmpTarget = (Socket | SOCKET_REMOTE_FLAG);
          }
          IoTgtLst[TargetIndex] = TmpTarget;
          TargetIndex++;
          NumberTargets--;
        }
      }
    }
  }

  //
  // Configure the IOPORT_TARGET list for this socket
  //
  ChaConfigureIoPortTargetList (SocId, IoTgtLst);

  ProgramCpuIoEntriesKti (SocketData, KtiInternalGlobal, SocId);

  return KTI_SUCCESS;
}

/**
  Routine to program the final IoApic SAD for given socket

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuIoApicEntries (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  )
{
  UINT8     Stack, Socket, TargetIndex = 0, NumberTargets;
  UINT32    TmpTarget = 0;
  UINT32    IoApicTgtLst[MAX_SAD_TARGETS];

  ZeroMem (IoApicTgtLst, sizeof(UINT32) * MAX_SAD_TARGETS);
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Socket present?
    //
    if (SocketData->Cpu[Socket].Valid == FALSE) {
      continue;
    }

    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      //
      // Stack present?
      //
      if (!StackPresent (Socket, Stack)) {
        continue;
      }

      //
      // Build up the 4 target register contents
      //
      if (KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].IoApicBase < KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].IoApicLimit) {
        TargetIndex = (UINT8)((KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].IoApicBase - IOAPIC_BASE) / SAD_IOAPIC_GRANTY);
        NumberTargets = (UINT8)((KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].IoApicLimit + 1 - KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].IoApicBase) / SAD_IOAPIC_GRANTY);
        while ((TargetIndex < MAX_SAD_TARGETS) && (NumberTargets > 0)) {
          if (Socket == SocId) {
            TmpTarget = (Stack | SOCKET_LOCAL_FLAG);
          } else {
            TmpTarget = (Socket | SOCKET_REMOTE_FLAG);
          }
          IoApicTgtLst[TargetIndex] = TmpTarget;
          TargetIndex++;
          NumberTargets--;
        }
      }
    }
  }

  //
  // Configure the IOPORT_TARGET list for this socket
  //
  ChaConfigureIoApicTargetList (SocId, IoApicTgtLst);

  ProgramCpuIoApicEntriesKti (SocketData, KtiInternalGlobal, SocId);

  return KTI_SUCCESS;
}

/**
  Routine to program the TCSKTSAD register on Cstack of Legacy Socket.
  This configures MCTP Broadcast traffic routing.

  @param SocketData        - CPU socket info
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramMctpBroadcastSettings (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  )
{
  UINT8  SocketsPresentBitmap;
  UINT8  Index;
  UINT16 StackPresentBitMap;

  SocketsPresentBitmap = 0;
  StackPresentBitMap = GetStackPresentBitMap (KtiInternalGlobal->SbspSoc) & (~(1 << IIO_CSTACK));

  //
  // vector of the remote node-ids in the system
  // excluding the nodeid of the socket being programmed. This indicates
  // which remote sockets MCTP broadcast messages should be programmed.
  //
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if ((SocketData->Cpu[Index].Valid == TRUE) &&
        (Index != KtiInternalGlobal->SbspSoc)  &&
        (SocketData->Cpu[Index].SocType == SOCKET_TYPE_CPU)) {
      SocketsPresentBitmap |= 1 << Index;
    }
  }
  IioSetMctpBroadcastSettings (KtiInternalGlobal->SbspSoc, IIO_CSTACK,
                               StackPresentBitMap,
                               SocketsPresentBitmap);

  return KTI_SUCCESS;
}

/**
  Routine to program MCTP Segment ID Size field.
  This is required to enable MCTP traffic routing.
  Needs to be programmed on each present stack.

  @param SocId     - processor socket ID
  @param IioStack  - IIO stack ID
  @param MmcfgRule - contents of BUSID_GLOB_RULE

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramMctpSegmentIdSize (
  UINT8          SocId,
  UINT8          IioStack,
  UINT32         MmcfgRule
  )
{
  IioSetMctpSegIdSize (SocId, IioStack, KTI_HOST_OUT_PTR->segmentSocket[SocId], MmcfgRule);
  return KTI_SUCCESS;
}

/**
  S3 Resume check and data structure update. If we are not in S3 resume path, update the NVRAM
  structure with the system topology info. If we are in S3 resume path, verify that the system topology
  matches the pre-S3 resume path topology info stored previously in NVRAM. We have to do the check
  only when no resets are pending; this is to make sure any transient system configuration is not
  interpreted as system configuration change in S3 resume path.

  @param SocketData        - pointer to the socket data structure
  @param KtiInternalGlobal - pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
CheckS3ResumePath (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8      Ctr, Port;
  BOOLEAN    S3ResumeFail;

  //
  // When we are not in S3 Resume path copy the system configuration info to the output structure
  //
  if (GetSysBootMode () == NormalBoot) {
    KTINVRAM->Cpus = KtiInternalGlobal->CpuList;
    CopyMem ((UINT8 *)KTINVRAM->Chas, (UINT8 *)KtiInternalGlobal->ChaList[KtiInternalGlobal->SbspSoc], sizeof (KTINVRAM->Chas));
    for (Ctr = 0; Ctr < MAX_SOCKET; Ctr++) {
      if (SocketData->Cpu[Ctr].Valid == TRUE) {
        CopyMem ((UINT8 *)KTINVRAM->LepInfo[Ctr], (UINT8 *)SocketData->Cpu[Ctr].LinkData, sizeof (KTINVRAM->LepInfo[Ctr]));
      }
    }
    return KTI_SUCCESS;
  }

  //
  // We are in S3 resume path; check to make sure config didn't change
  //
  S3ResumeFail = FALSE;
  if (GetSysResetRequired () == POST_RESET_NO_RESET) {
      if (KTINVRAM->Cpus != KtiInternalGlobal->CpuList ||
          KTINVRAM->Chas[0] != KtiInternalGlobal->ChaList[KtiInternalGlobal->SbspSoc][0] ||
          KTINVRAM->Chas[1] != KtiInternalGlobal->ChaList[KtiInternalGlobal->SbspSoc][1] ||
          KTINVRAM->Chas[2] != KtiInternalGlobal->ChaList[KtiInternalGlobal->SbspSoc][2] ||
          KTINVRAM->Chas[3] != KtiInternalGlobal->ChaList[KtiInternalGlobal->SbspSoc][3]) {
      S3ResumeFail = TRUE;
    }
    for (Ctr = 0; Ctr < MAX_SOCKET; Ctr++) {
      if (S3ResumeFail == TRUE) {
        break;
      }
      if (SocketData->Cpu[Ctr].Valid == TRUE) {
        for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
          if (KTINVRAM->LepInfo[Ctr][Port].Valid != SocketData->Cpu[Ctr].LinkData[Port].Valid) {
            S3ResumeFail = TRUE;
            break;
          }
        }
      }
    }
  }

  if (S3ResumeFail == TRUE) {
    KtiCheckPoint (0xFF, 0xFF, 0xFF, ERR_S3_RESUME, MINOR_ERR_S3_RESUME_TOPOLOGY_MISMATCH);
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Topology Doesn't Match - S3 Resume Failed."));
    KTI_ASSERT (FALSE, ERR_S3_RESUME, MINOR_ERR_S3_RESUME_TOPOLOGY_MISMATCH);
  }

  return KTI_SUCCESS;
}

/**
  Program MSR as workarounds for KTI RC.
  SBSP writs the value into UBOX Sticky Scratch 07 in each cpu socket, BSP itself as well as all PBSPs
  At subsequent warm resets, each cpu socket will program MSRs as instructed by the value in its UBOX Sticky Scratch 07.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

**/
KTI_STATUS
ProgramMsrForWa (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  return KTI_SUCCESS;
}

/**
  Generate ASM code to put each sockets Pipe interface bus number in xmm6/xmm7
  Consumed in emulation

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

**/
KTI_STATUS
KtiGeneratePipeBusNumberAsm (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8   SocId;
  UINT32  Ecx_Register;
  UINT32  Edx_Register;
  UINTN   RegVal;

  if (UbiosGenerationEnabled()) {
    Ecx_Register = 0;
    Edx_Register = 0;

    for (SocId = 0; SocId < MAX_SOCKET; SocId++) {
      if ((SocketData->Cpu[SocId].Valid == FALSE) || (SocketData->Cpu[SocId].SocType != SOCKET_TYPE_CPU)) {
        continue;
      }

      UBoxGetBiosNonStickyScratchPadRegisterFlatAddr(SocId, 0, 13, &RegVal);
      if (SocId < 4) {
        Ecx_Register |= ((RegVal >> 20) & 0xff) << (8 * SocId);
      }
      else {
        Edx_Register |= ((RegVal >> 20) & 0xff) << (8 * SocId);
      }
    }

    RcDebugPrint (SDBG_DEFAULT, "\nmov   ecx, 0%08Xh", Ecx_Register);
    RcDebugPrint (SDBG_DEFAULT, "\nmov   edx, 0%08Xh", Edx_Register);
    RcDebugPrint (SDBG_DEFAULT, "\nmovd  xmm6, ecx");
    RcDebugPrint (SDBG_DEFAULT, "\nmovd  xmm7, edx");
  }

  return KTI_SUCCESS;
}

/**
  SBSP programs bus numbers and mmcfg rules to each socket.
  SBSP has PBSP activate changes

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

**/
KTI_STATUS
KtiFinalCpuBusCfg (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8  Index, Targets = 0, Idx2, NumBuses = 0;
  UINT32 WaitTime, MmcfgTgtlist, Data32;
  UINT8  TmpTotCpu;
  UINT32 MmcfgGranularity;
  UINT8  Size;
  UINT8  *RegAddr;
  UINT32 LabelNumber;
  UINT8  CpuType;
  BOOLEAN IsPostResetPowerGood;
  UINTN   RegVal;

  CpuType     = KtiInternalGlobal->CpuType;

  //
  // Create list of valid targets
  // Create MmmcfgTargetList
  // Write new value for MMCFG_RULE.
  //
  if (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE) {
    if (KTISETUP->KtiCpuSktHotPlugTopology == 0) {
      TmpTotCpu =  4;         //online/offline based on 4S topology
    } else {
      TmpTotCpu =  8;         //online/offline based on 8S topology
    }
  } else {
    TmpTotCpu =  MAX_SOCKET;
  }

  KTI_ASSERT ((TmpTotCpu <= MAX_SOCKET), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_7B);

  if (KTIVAR->mmCfgSize <= 0x10000000) {
    MmcfgGranularity = SAD_BUS_GRANTY;
  } else {
    MmcfgGranularity = (KTIVAR->mmCfgSize / 1024 / 1024 / SAD_BUS_TARGETS);
  }

  MmcfgTgtlist = 0;
  Idx2 = 0;
  for (Index = 0; Index < TmpTotCpu; Index++) {
    if ((SocketData->Cpu[Index].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) {
      NumBuses = (UINT8)(((((KTIVAR->CpuInfo[Index].CpuRes.BusLimit - KTIVAR->CpuInfo[Index].CpuRes.BusBase + 1) / MmcfgGranularity) - 1)) + 1);
      //
      //Idx2 = (UINT8)(KTIVAR->CpuInfo[Index].CpuRes.BusBase / MmcfgGranularity);
      //
      while (NumBuses) {
        //
        // If hotplug system, preprogram the target list.
        // But only program valid_target for the currently present sockets.
        // During ONL flow this field will be updated
        //
        if (SocketData->Cpu[Index].Valid == TRUE) {
          if ((SocketData->Cpu[Index].SocType == SOCKET_TYPE_CPU) || (SocketFpgasInitialized (KtiInternalGlobal))) {
            Targets = Targets | (1 << Idx2);
          }
        }
        if ((SocketData->Cpu[Index].Valid == FALSE) && (KtiInternalGlobal->KtiCpuSktHotPlugEn == FALSE)) {
          MmcfgTgtlist |= SOCKET_LOCAL_4BITS_NID_MSB << (Idx2*4);
        } else {
          MmcfgTgtlist |= Index << (Idx2*4);
        }

        NumBuses--;
        Idx2++;
      }
    }
  }

  //
  // Clear bus numbers assigned to FPGA during topology discovery, FPGA buses will be re-assigned
  //
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if ((SocketData->Cpu[Index].Valid == TRUE) && (SocketData->Cpu[Index].SocType == SOCKET_TYPE_FPGA)) {
      FpgaIpTearDownBusNumbers (Index);
    }
  }

  //
  // Loop through all PBSPs and update bus programming
  //
  IsPostResetPowerGood = CheckCurrentReset (POST_RESET_POWERGOOD);

  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if (((KTIVAR->RasInProgress == FALSE && (KtiInternalGlobal->CpuList & (1 << Index))) ||
        (KTIVAR->RasInProgress == TRUE && KTIVAR->RasSocId == Index))) {

      if (KtiInternalGlobal->SbspSoc != Index) {
        //
        // Check for PBSP_READY
        //
        WaitTime = 5000;
        UBoxGetBiosNonStickyScratchPadRegisterValue (Index, 0, 13, &Data32);
        while ((Data32 != BUSCFGSYNC_PBSP_READY) && (WaitTime > 0)) {
          KtiFixedDelay (1000); // 1000 microsecs
          WaitTime = WaitTime - 1;
          UBoxGetBiosNonStickyScratchPadRegisterValue (Index, 0, 13, &Data32);
        }
        if (GetUbiosLoopbackLabelNumber (&LabelNumber) == EFI_SUCCESS) {
          Size = 4;
          UBoxGetBiosNonStickyScratchPadRegisterFlatAddr (Index, 0, 13, &RegVal);
          RegAddr = (UINT8 *)RegVal;
          if (IsPostResetPowerGood) {
            RcDebugPrint (SDBG_DEFAULT, "\nLABEL_c_mbox%d:", LabelNumber);
          } else {
            RcDebugPrint (SDBG_DEFAULT, "\nLABEL_w_mbox%d:", LabelNumber);
          }
          RcDebugPrint (SDBG_DEFAULT, "\nmov eax, DWORD PTR ds:[0%8xh]", RegAddr);
          RcDebugPrint (SDBG_DEFAULT, "\nxor eax, BUSCFGSYNC_PBSP_READY");
          if (IsPostResetPowerGood) {
            RcDebugPrint (SDBG_DEFAULT, "\njnz LABEL_c_mbox%d\n", LabelNumber);
          } else {
            RcDebugPrint (SDBG_DEFAULT, "\njnz LABEL_w_mbox%d\n", LabelNumber);
          }
          LabelNumber++;
          SetUbiosLoopbackLabelNumber (LabelNumber);
        }
        KTI_ASSERT (WaitTime, ERR_BOOT_MODE, MINOR_ERR_PBSP_S0_BUS_PROGRAM_FAILURE | Index);
        KtiFinalPbspBusCfg (SocketData, KtiInternalGlobal, Targets, MmcfgTgtlist, Index);
      }
    }
  }
  //
  // Update bus numbers in FPGAs
  //
  KtiFinalFpgaBusCfg (SocketData, KtiInternalGlobal);

  //
  // Now update SBSP.
  //
  KtiFinalSbspBusCfg (SocketData, KtiInternalGlobal, Targets, MmcfgTgtlist, KtiInternalGlobal->SbspSoc);

  if (IsIcxFamilyCpu(CpuType)|| IsSprFamilyCpu(CpuType)) {
    UpdateCpuBusOnIio (KtiInternalGlobal, KtiInternalGlobal->SbspSoc);
  }

  KtiFinalBusCfgKti (SocketData, KtiInternalGlobal, KtiInternalGlobal->SbspSoc);

  //
  // Check that all sockets completed before continuing
  //
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if (((KTIVAR->RasInProgress == FALSE && (KtiInternalGlobal->CpuList & (1 << Index))) ||
        (KTIVAR->RasInProgress == TRUE && KTIVAR->RasSocId == Index))) {

      if (KtiInternalGlobal->SbspSoc != Index) {
        //
        // Check for PBSP_DONE
        //
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Wait for S%u to update\n", Index));
        WaitTime = 5000;
        UBoxGetBiosNonStickyScratchPadRegisterValue(Index, 0, 13, &Data32);
        while ((Data32 != BUSCFGSYNC_PBSP_DONE) && (WaitTime > 0)) {
          KtiFixedDelay (1000); // 1000 microsecs
          WaitTime = WaitTime - 1;
          UBoxGetBiosNonStickyScratchPadRegisterValue (Index, 0, 13, &Data32);
        }
        if (GetUbiosLoopbackLabelNumber (&LabelNumber) == EFI_SUCCESS) {
          Size = 4;
          UBoxGetBiosNonStickyScratchPadRegisterFlatAddr (Index, 0, 13, &RegVal);
          RegAddr = (UINT8 *)RegVal;
          if (IsPostResetPowerGood) {
           RcDebugPrint (SDBG_DEFAULT, "\nLABEL_c_mbox%d:", LabelNumber);
          } else {
           RcDebugPrint (SDBG_DEFAULT, "\nLABEL_w_mbox%d:", LabelNumber);
          }
          RcDebugPrint (SDBG_DEFAULT, "\nmov eax, DWORD PTR ds:[0%8xh]", RegAddr);
          RcDebugPrint (SDBG_DEFAULT, "\nxor eax, BUSCFGSYNC_PBSP_DONE");
          if (IsPostResetPowerGood) {
            RcDebugPrint (SDBG_DEFAULT, "\njnz LABEL_c_mbox%d\n", LabelNumber);
          } else {
            RcDebugPrint (SDBG_DEFAULT, "\njnz LABEL_w_mbox%d\n", LabelNumber);
          }
          LabelNumber++;
          SetUbiosLoopbackLabelNumber (LabelNumber);
        }
        KTI_ASSERT (WaitTime, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_6E);

        UBoxSetBiosNonStickyScratchPadRegisterValue (Index, 0, 13, 0);
      }
    }
  }

  //
  // Update KTI bus TADs in PBSPs
  //
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if ((KtiInternalGlobal->CpuList & (1 << Index)) && (KtiInternalGlobal->SbspSoc != Index)) {
      KtiFinalBusCfgKti (SocketData, KtiInternalGlobal, Index);

      //
      // At this point, ubox and iio stack can be accessed now
      // So we update Iio here.
      //
      if (!IsSkxFamilyCpu(CpuType)) {
        UpdateCpuBusOnIio (KtiInternalGlobal, Index);
      }
    }
  }

  //
  // Implement Si_WAs, just after MMCFG are programmed for all the sockets
  //
  ProgramMiscSiWa (KtiInternalGlobal);

  //
  // Set Bus number on WHR and FPGA to enable MMCFG access on specific bus
  //
  SetFpgaBusNumber (SocketData, KtiInternalGlobal);
  KtiGeneratePipeBusNumberAsm(SocketData, KtiInternalGlobal);
  return KTI_SUCCESS;
}

/**
  Pcu Misc Config

  This routine performs the misc mail box commands

  @param SocketData        - pointer to the socket data structure
  @param KtiInternalGlobal - pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS - on successful completion

**/

KTI_STATUS
KtiPcuMiscConfig (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8      Index;
  BOOLEAN    SetE2EParity;

  SetE2EParity = FALSE;

  //CHECK_CPX need to check for CPX
  if (IsCpuAndRevision (CPU_SKX, REV_ALL) ||
      IsCpuAndRevision (CPU_CLX, REV_ALL) ||
      IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    SetE2EParity = TRUE;
  }

  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if ((SocketData->Cpu[Index].Valid == TRUE) && (SocketData->Cpu[Index].SocType == SOCKET_TYPE_CPU)) {
      //
      // Enable IDI Parity at all times
      //
      if (SetE2EParity) {
        PcuSetE2EParity (Index);
      }
    }
  }
  return KTI_SUCCESS;
}

/**
  Trigger CHA credit change without warm-reset.

  @param[in] SocketData           Pointer to the socket data structure
  @param[in] KtiInternalGlobal    Pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS   Successful completion

**/
KTI_STATUS
KtiTriggerChaCreditChange (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8    SocId;
  UINT8    Cha;

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nTrigger CHA credit change without warm-reset\n"));

  for (SocId = 0; SocId < MAX_SOCKET; SocId++) {
    if ((SocketData->Cpu[SocId].Valid == FALSE) || (SocketData->Cpu[SocId].SocType != SOCKET_TYPE_CPU)) {
      continue;
    }

    for (Cha = 0; Cha < KtiInternalGlobal->TotCha[SocId]; Cha++) {
      ChaEnableCreditChange (SocId, Cha);
    }
  }
  return KTI_SUCCESS;
}

/**
  Set rst_cpl1 bit in PCU BIOS_RESET_CPL register.

  @param[in] KtiInternalGlobal    Pointer to the KTI RC internal global structure
  @param[in] Socket               Node ID of the socket to program

  @retval KTI_SUCCESS   rst_cpl1 bit programming completed.
  @retval KTI_FAILURE   rst_cpl1 bit programming was skipped according to PcdDfxRstCplBitsEn.

**/
KTI_STATUS
SetRstCpl1 (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      Socket
  )
{
  EFI_STATUS                     Status;
  UINT8                          PcodeInitDone;
  UINT32                         TimeoutCounter;
  UINT32                         LabelNumber;
  UINT8                          *RegAddr;

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n:INIT - BIOS_RESET_CPL: Set CPL1 on #S%d\n", Socket));

  if (PcuSetRstCpl (Socket, SET_CPL_1) != EFI_SUCCESS) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "RST_CPL1 is not set\n"));
    return KTI_FAILURE;
  }

  //
  // wait for PCU acknowledgement for 2ms (20us * 100)
  //
  TimeoutCounter = 100;

  if (HybridSystemLevelEmulationEnabled() || IsSimicsEnvironment ()) {
    //Change the time out value to 20s for HSLE and Simics (WB model)
    TimeoutCounter = 1000000;
  }

  do {
    PcuGetPcodeInitDone (Socket, SET_CPL_1, &PcodeInitDone);
    KtiFixedDelay (20);  //20us
    TimeoutCounter--;
  } while ((PcodeInitDone == 0) && (TimeoutCounter > 0));


  if (TimeoutCounter == 0) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n:INIT - Pcode fails to ACK CPL1 on #S%d\n", Socket));
    KTI_ASSERT (TimeoutCounter, ERR_BOOT_MODE, MINOR_ERR_CPL1_TIMEOUT_FAILURE);
  }

  if (GetUbiosLoopbackLabelNumber (&LabelNumber) == EFI_SUCCESS) {
    //
    // Manually poll for rst_cpl1 done in FC
    //
    RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (Socket, 0, BIOS_RESET_CPL_PCU_FUN1_REG);

    if (KtiInternalGlobal->CurrentReset == POST_RESET_POWERGOOD) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nLABEL_c_mbox%d:", LabelNumber));
    } else {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nLABEL_w_mbox%d:", LabelNumber));
    }
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nmov eax, DWORD PTR ds:[0%8xh]", RegAddr));
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nxor eax, 0200h"));
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nbt  eax, 9"));
    if (KtiInternalGlobal->CurrentReset == POST_RESET_POWERGOOD) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\njc LABEL_c_mbox%d\n", LabelNumber));
    } else {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\njc LABEL_w_mbox%d\n", LabelNumber));
    }
    LabelNumber++;
    SetUbiosLoopbackLabelNumber (LabelNumber);
  }
  if (KTISETUP->DfxParm.DfxIioDfxEnabled) {
    //
    // Request PCU to allow configuring additional access rights for IOMT tool running in host.
    // PCU accepts this commant only between RESET_CPL1 and RESET_CPL2, then the configuration
    // changes must be done before RESET_CPL3.
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n[UPI][DFX] Sending PCU request to enable IOMT support on #S%d\n", Socket));
    Status = PcuSetMiscWorkaroundEnable (Socket, MAILBOX_BIOS_CMD_MISC_WORKAROUND_ENABLE_IOMT_WORKAROUND_ENABLE);
    if (EFI_ERROR (Status)) {

      KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n[UPI][DFX] WARNING: Failed to enable IOMT support on #S%d PCU (%r)\n",
                          Socket, Status));
    }
  }
  return KTI_SUCCESS;
}

/**
  Setup other Uncore registers. This includes programming of NID and am_i_master
  fields in Global_PKG_C_S_CONTROL register, rst_cpl1 bit and clearing DMICTRL
  auto_pm complete field.

  The programming of am_i_master and NID in GLOBAL_PKG_C_S_CONTROL register must be
  done before rst_cpl1 bit is set to 1.

  @param[in] SocketData           Pointer to the socket data structure
  @param[in] KtiInternalGlobal    Pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS   Successful completion

**/
KTI_STATUS
SetupUncoreMisc (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8      SocId, SbspSocId;
  UINT32     Data32;
  UINT32     SlaveDieBitMap;
  UINT32     DmiMemBarBase, DmiMemBarSize, DmiMemResBase, DmiRcrbBar;
  KTI_STATUS Status;

  SbspSocId = KtiInternalGlobal->SbspSoc;
  //
  // Program GLOBAL_PKG_C_S_CONTROL_REGISTER in PCU
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSetup Uncore Misc:\n"));

  for (SocId = 0; SocId < MAX_SOCKET; SocId++) {
    if ((SocketData->Cpu[SocId].Valid == FALSE) || (SocketData->Cpu[SocId].SocType != SOCKET_TYPE_CPU)) {
      continue;
    }

    Data32 = PcuSetNidAndAmIMaster (SocId, SbspSocId);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nWrite Socket %2d GLOBAL_PKG_C_S_CONTROL_REGISTER = %X\n", SocId, Data32));
  }

  //
  // Set "BIOS_RESET_CPL" flag bit of all PBSP sockets
  //

  SlaveDieBitMap = GetSlaveDieBitMap ();

  //
  // For dual-die package, Slave die need to be set prior to the master die
  //
  if (GetDieCount (SbspSocId) == TWO_DIE_IN_SOCKET) {
    for (SocId = 0; SocId < MAX_SOCKET; SocId++) {
      if ((SlaveDieBitMap & (1 << SocId)) == 0) {
        continue;
      }
      SetRstCpl1 (KtiInternalGlobal, SocId);
    }
  }

  //
  // Proceeds to PBSP sockets or master die
  //
  for (SocId = 0; SocId < MAX_SOCKET; SocId++) {
    if (SocId == SbspSocId) {
      continue;
    }

    if ((KtiInternalGlobal->CpuList & (1 << SocId)) == 0) {
      continue;
    }

    if ((SlaveDieBitMap & (1 << SocId)) != 0) {
      continue;
    }

    SetRstCpl1 (KtiInternalGlobal, SocId);
  }

  //
  //  Lastly the SBSP socket
  //
  SetRstCpl1 (KtiInternalGlobal, SbspSocId);

  //
  // Program DMICTRL...We only want to do this for SBSP... clear bits 0:1 of DMICTRL on Legacy socket only
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nClear DMICtrlAutoPmCmplt...\n"));
  IioClearDmictrlAutoCompletePm (SbspSocId);
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nDMICTRL.auto_complete_pm cleared on S%x\n", SbspSocId));

  //
  // Program the DMI RCRBBAR for DMI configuration region access
  // Sbsp stack0's first 8KB mmiol will be used as the RCRBBAR
  //
  if (IsSprFamilyCpu(KtiInternalGlobal->CpuType)) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nInitalize DMI RCRB region. \n"));
    DmiMemBarBase = DmiMemBarSize = 0;
    for (SocId = 0; SocId < MAX_SOCKET; SocId ++) {
      if ((SocketData->Cpu[SocId].Valid == FALSE) || (SocketData->Cpu[SocId].SocType != SOCKET_TYPE_CPU) ||
        (((BIT0 << SocId) & GetPchPresentBitMap()) == 0)) {
        continue;
      }
        DmiRcrbBar = KTIVAR->CpuInfo[SocId].CpuRes.StackRes[0].MmiolBase;
        KTIVAR->CpuInfo[SocId].CpuRes.StackRes[0].RcrbBase = DmiRcrbBar;
        DmiMemResBase = DmiRcrbBar + DMI_RCRB_BAR_SIZE;
      Status = DmiRcrbInitialize (SocId, DmiRcrbBar, DmiMemResBase, &DmiMemBarBase, &DmiMemBarSize);
      if (Status == KTI_SUCCESS) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket: %d;DMI MemBar locates on 0x%x, Size: 0x%x \n", SocId, DmiMemBarBase, DmiMemBarSize));
        KTIVAR->DmiMemBarBase[SocId] = DmiMemBarBase;
        KTIVAR->DmiMemBarSize[SocId] = DmiMemBarSize;
      }
    }
  }
  return KTI_SUCCESS;
}

/**
  Program BGF Pointer Separation Overrides as workarounds for KTI RC.
  SBSP writes these values via Mailbox Commands to all populated sockets.  Values will take effect after a warm reset.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

**/
KTI_STATUS
KtiProgramBGFOverrides (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8   Socket = 0;
  UINT8   BgfOverrideMask = 0;
  BOOLEAN ProgramBgfPtr = FALSE;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((KTIVAR->RasInProgress == FALSE && (KtiInternalGlobal->CpuList & (1 << Socket))) ||
      (KTIVAR->RasInProgress == TRUE && KTIVAR->RasSocId == Socket)) {
      BgfOverrideMask = BgfPtrOverrideSupported (KtiInternalGlobal, Socket);
      if (BgfOverrideMask != 0) {
        PcuBGFPointerOverride (Socket, BgfOverrideMask);
        ProgramBgfPtr = TRUE;
      }
    }
  }
  if (ProgramBgfPtr) {
    SetNewResetRequired (POST_RESET_WARM);
  }

  return KTI_SUCCESS;
}
