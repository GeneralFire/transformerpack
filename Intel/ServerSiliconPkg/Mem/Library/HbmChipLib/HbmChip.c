/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Library/PcuIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/HbmIoIpLib.h>
#include <Library/HbmCoreLib.h>
#include <Library/KtiApi.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemTurnaroundTimingLib.h>
#include <Library/MemThermManageLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/MemMap2SgxInterface.h>
#include <CmiPerformance.h>
#include "Include/HbmChipInternal.h"

/**
  Update the HBM IO and Channel number based on the CAP ID value

  @param[in]      Socket        - Socket number

  @retVal         SUCCESS
**/
EFI_STATUS
HbmUpdateIoAndChNumber (
  IN UINT8           Socket
  )
{
  UINT8                   HbmIoId;
  UINT8                   HbmMaxIoSocket;
  UINT8                   HbmChPerIo;
  CPU_CSR_ACCESS_VAR      *CpuCsrAccessVar;

  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();

  //
  // Find the highest order enabled IO to determine HbmMaxIoInst.
  //
  HbmMaxIoSocket = PcuGetHbmEnabledIoNum (Socket);

  //
  // Do not exceed MAX_HBM_IO.
  //
  if (HbmMaxIoSocket > MAX_HBM_IO) {
    HbmMaxIoSocket = MAX_HBM_IO;
  }

  //
  // Update the system-wide HbmMaxIoInst for Socket 0, or if we need to increase it.
  //
  if (Socket == 0 || HbmMaxIoSocket > CpuCsrAccessVar->HbmMaxIoInst) {
    CpuCsrAccessVar->HbmMaxIoInst = HbmMaxIoSocket;
    CpuCsrAccessVar->HbmMaxCh = HbmMaxIoSocket * CpuCsrAccessVar->HbmNumChPerIo;
  }

  //
  // Disable Hbm IO based on what was read.
  //
  for (HbmIoId = 0; HbmIoId < MAX_HBM_IO; HbmIoId++) {
    if (HbmIoId >= HbmMaxIoSocket) {
      SetHbmMemSsEnable (Socket, HbmIoId, 0);

      //
      // Disable corresponding channels if this HBM io is disable
      //
      for (HbmChPerIo = 0; HbmChPerIo < CpuCsrAccessVar->HbmNumChPerIo; HbmChPerIo++) {
        SetHbmChEnable (Socket, HbmIoId * CpuCsrAccessVar->HbmNumChPerIo + HbmChPerIo, 0);
      }
    }
  }

  return EFI_SUCCESS;
}

/**

  Initialize internal chip data structures

  @param[in]   Socket  Current Socket (0-Based)

  @retval EFI_SUCCESS   Init HBM data structures successful

**/
EFI_STATUS
HbmInit (
  IN  UINT8  Socket
  )
{
  UINT8     HbmIo = 0;
  UINT8     HbmCh = 0;

  if (!IsHbmFlowEnabled (HF_INIT)) {
    return EFI_SUCCESS;
  }

  //
  // Enable all the HBM IO and Ch by default
  //
  for (HbmIo = 0; HbmIo < GetMaxIoHbm (); HbmIo++) {
    SetHbmMemSsEnable (Socket, HbmIo, 1);
  }

  for (HbmCh = 0; HbmCh < GetMaxChHbm (); HbmCh++) {
    SetHbmChEnable (Socket, HbmCh, 1);
  }

  HbmUpdateIoAndChNumber ((UINT8) Socket);

  return EFI_SUCCESS;
} // HbmInit

/**

  HBM Early Configuration

  @param[in]  Socket  - Current Socket (0-Based)

  @retval EFI_SUCCESS  This function completed execution with no warnings/errors

**/
EFI_STATUS
HbmEarlyConfig (
  IN  UINT8  Socket
  )
{
  UINT8       HbmCh;
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  if (!IsHbmFlowEnabled (HF_EARLY_CONFIG)) {
    return Status;
  }

  //
  // Initial turnaround times
  //
  InitRelaxTurnaroundTimesHbm (Socket);

  //
  // Program Refresh timing and Scheduler timing
  // Can keep SchedulerConfigHbm before training, for these register will take some time to take effect
  //
  ProgramTimingsHbm (Socket);

  //
  // Program HBM DIMM MTR register
  //
  HbmDimmMtrConfig (Socket);

  for (HbmCh = 0; HbmCh < GetMaxChHbm (); HbmCh++) {
    if (!IsHbmChannelEnabled (Socket, HbmCh)) {
      continue;
    }

    HbmSetMcMode (Socket, HbmCh);

    HbmInitPagePolicy (Socket, HbmCh);

    ResetPmonUnitCtrlCounters (MemTechHbm, Socket, HbmCh);
  }

  SetDclkEnableMc (MemTechHbm, Socket);

  return Status;

}

/**

  Initialize channel specific registers after training and before normal Mode

  @param[in]                Socket              Current Socket (0-Based)

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
EFI_STATUS
HbmLateConfig (
  IN        UINT8           Socket
  )
{
  UINT8                   Channel;
  UINT32                  Status;
  BOOLEAN                 DbiEnable;

  Status = EFI_SUCCESS;

  if (!IsHbmFlowEnabled (HF_LATE_CONFIG)) {
    return Status;
  }

  if (!IsSocketEnabled (Socket)) {
    return Status;
  }

  //
  // Restore HbmIO values
  //
  if (IsFastBootPath ()) {
    // TODO for warm reset resume path, HSD

  }

  DbiEnable = (BOOLEAN) (GetHbmSetupOption (HbmOptionMiscOptions) & HBM_DBI);

  for (Channel = 0; Channel < GetMaxChHbm (); Channel++) {
    if (!IsHbmChannelEnabled (Socket, Channel)) {
      continue;
    }

    HbmInitDbi (Socket, Channel, DbiEnable);

    HbmMcLateConfig (Socket, Channel);
  }

  return Status;
}

/**

  Using boot Dimm configuration, find which option of cmi values to program

  @param[in]  Socket                         -  Socket Id
  @param[out] ProgramableValue[MAX_HBM_CH]   -  Array containing programmable value type for each Channel (From 0 to Max Channel in Socket)
                                                CMI_1LM_VAL       = Value for 1LM DDR
                                                CMI_XTILE_VAL     = Value for XTile Mode

  @retval N/A

**/
VOID
GetCmiProgramValueHbm (
  IN UINT8 Socket,
  OUT UINT8 ProgramableValue[MAX_HBM_CH]
)
{
  UINT8        HbmCh;
  UINT8        MaxChHbm;

  MEMORY_MODE_ATTRIBUTE MemMode;
  GetMemMode (Socket, &MemMode);

  ASSERT_EFI_ERROR (ProgramableValue != NULL);
  MaxChHbm = GetMaxChHbm ();

  for (HbmCh = 0; HbmCh < MaxChHbm; HbmCh++) {
    if (!IsHbmChannelEnabled (Socket, HbmCh)) {
      continue;
    }
    ProgramableValue[HbmCh] = CMI_1LM_VAL;  //default 1LM
    if ((MemMode & MEMORY_MODE_ATTRIBUTE_2LM_XTILE) != 0) {
	  ProgramableValue[HbmCh] = CMI_XTILE_VAL;
    }
  }
}

/**
  Initializes CMI Credit registers for Hbm

  @param[in]                Socket              Current Socket (0-Based)

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
HbmInitCmiCreditProgramming (
  IN UINT8   Socket
  )
{
  EFI_STATUS                      Status = EFI_SUCCESS;
  PSYSHOST                        Host;
  UINT8                           HbmCh;
  UINT8                           MaxChHbm;
  UINT8                           ProgramableValue[MAX_HBM_CH];
  UINT8                           CmiSetupValue;

  Host = (PSYSHOST) GetSysHostPointer ();
  MaxChHbm = GetMaxChHbm ();

  if (!IsHbmFlowEnabled (HF_CMI_CREDIT_PROGRAMMING)) {
    return Status;
  }

  if (Host->nvram.mem.socket[Socket].enabled == 0) {
    return Status;
  }

#ifdef DEBUG_CODE_BLOCK
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "Hbm Cmi Credit Programming\n");
#endif // DEBUG_CODE_BLOCK

  ZeroMem (ProgramableValue, sizeof (ProgramableValue));
  CmiSetupValue = GetCmiSetupProgramValue ();

  if (CmiSetupValue == CMI_AUTO_OPTION)  {
    GetCmiProgramValueHbm (Socket, ProgramableValue);
  } else {
    //If Auto, then program the value we calculated, else program default value
    SetMem (ProgramableValue, sizeof (ProgramableValue), CMI_REGISTER_DEFAULTS);
  }

  Status = ProgramMemMcCmiRegistersHbm (ProgramableValue, Socket);

  for (HbmCh = 0; HbmCh < MaxChHbm; HbmCh++) {
    if (!IsHbmChannelEnabled (Socket, HbmCh)) {
      continue;
    }

    ProgramMesh2MemCmiRegistersHbm (ProgramableValue, Socket);
  }

  //set done bits here
  for (HbmCh = 0; HbmCh < MaxChHbm; HbmCh++) {
    if (!IsHbmChannelEnabled (Socket, HbmCh)) {
      continue;
    }
    SetM2mCmiMiscConfigDoneChHbm (Socket, HbmCh, TRUE);
    SetMcCmiConfigDoneChHbm (Socket, HbmCh);
  }

  //Start Polling here
  for (HbmCh = 0; HbmCh < MaxChHbm; HbmCh++) {
    if (!IsHbmChannelEnabled (Socket, HbmCh)) {
      continue;
    }
    Status = WaitForM2mCmiMiscConfigChannelsCompleteTargetHbm (Socket, HbmCh);
    Status = WaitForMcMiscCmiCreditConfigCompleteTargetHbm (Socket, HbmCh);
    Status = WaitForMcCmiReqCpcfgCompleteTargetHbm (Socket, HbmCh);
  }

  return Status;
}

/**
  This funcition resets the HBM IO modules.

  @param[in] SocketId       Socket index.

**/
VOID
HbmIoReset (
  IN UINT8  SocketId
  )
{
  //
  // HBMIO Mailbox Physical Initialization Start
  //
  HbmioMboxPhyInitStart (SocketId);

  //
  // HBM MC reset_io toggling
  //
  HbmMcSetResetIo (SocketId);
}

/**
  HBM pre-training entry

  This function is the HBM pre-training entry where the HBM pre-training
  steps are executed sequentially.

  @param[in] Host      Pointer to sysHost.

  @retval SUCCESS      HBM pre-training steps completed successfully.
**/
UINT32
EFIAPI
HbmPreTraining (
  IN PSYSHOST  Host
  )
{
  EFI_STATUS      Status;
  UINT8           Socket;

  if (!IsHbmSku()) {
    return SUCCESS;
  }

  Socket = Host->var.mem.currentSocket;

  //
  // Enable all HBM flows.
  //
  Status = EnableAllHbmFlows ();

  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = HbmInit (Socket);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Detect HBM frequency via B2P commands
  //
  HbmDetectFrequency (Socket);

  //
  // Reset HBM IO modules.
  //
  HbmIoReset (Socket);

  //
  // Discover HBM device information.
  //
  Status = HbmDiscovery (Socket);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Skip below steps if there is a reset request
  //
  if (GetSysResetRequired () == 0) {
    Status = HbmEarlyConfig (Socket);
  }

  return (UINT32) Status;
}

/**
  HBM training entry

  This function is the HBM training entry where the HBM training steps
  are executed sequentially.

  @param[in] Host      Pointer to sysHost.

  @retval SUCCESS      HBM training steps completed successfully.
**/
UINT32
EFIAPI
HbmTraining (
  IN PSYSHOST  Host
  )
{

  EFI_STATUS      Status = EFI_SUCCESS;
  UINT8           Socket;

  if (!IsHbmSku()) {
    return SUCCESS;
  }

  Socket = Host->var.mem.currentSocket;
  Status = HbmioMboxInit (Socket, FALSE);

  return (UINT32)Status;
}

/**
  HBM post-training entry

  This function is the HBM post-training entry where the HBM post-training
  steps are executed sequentially.

  @param[in] Host      Pointer to sysHost.

  @retval SUCCESS      HBM post-training steps completed successfully.
**/
UINT32
EFIAPI
HbmPostTraining (
  IN PSYSHOST  Host
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINT8           Socket;

  //
  // HBM_TODO: Needs to add HBM post-training steps here.
  //

  if (!IsHbmSku()) {
    return SUCCESS;
  }

  Socket = Host->var.mem.currentSocket;

  SchedulerConfigLateHbm (Socket);

  //
  // Program HBM MC AMAP register
  //
  PostTrainingInitMc (MemTechHbm, Socket);

  Status = HbmLateConfig (Socket);

  Status |= HbmInitThrottling (Socket);

  return Status;
}

/**
  HBM switch to normal mode entry

  This function is the HBM switch to normal mode entry where the HBM CSRs
  required by switching HBM MC to normal mode are programmed sequentially.

  @param[in] Host      Pointer to sysHost.

  @retval SUCCESS      HBM SwitchToNormalMode steps completed successfully.
**/
UINT32
EFIAPI
HbmSwitchToNormalMode (
  IN PSYSHOST  Host
  )
{
  UINT8        SocketId;
  UINT8        HbmChId;
  UINT8        HbmMaxCh;
  EFI_STATUS   Status;

  if (!IsHbmSku()) {
    return SUCCESS;
  }

  SocketId = Host->var.mem.currentSocket;
  HbmMaxCh = GetMaxChHbm ();

  for (HbmChId = 0; HbmChId < HbmMaxCh; HbmChId++) {
    if (!IsHbmChannelEnabled (SocketId, HbmChId)) {
      continue;
    }
    HbmMcSetNormalMode (SocketId, HbmChId);
  }

  Status = HbmInitCmiCreditProgramming (SocketId);
  if (EFI_ERROR (Status)) {
    return (UINT32) Status;
  }

  return SUCCESS;
}

