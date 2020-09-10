/** @file Hardware implementation of the MemMcIpTargetLib library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include "Include/MemMcRegs.h"
#include <Library/MemoryCoreLib.h>
#include <Chip/Include/FnvAccessCommon.h>
#include <Library/MemMcIpTargetLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/UsraCsrLib.h>
#include <RcRegs.h>
#include <Chip/Include/MemHostChip.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/PreSiliconEnvDetectLib.h>

/**
  Build target function to set the Fnv Ecc mode.

  @param[out] DdrtEccMode  Ptr to to the DdrtEccMode register structure.

  @retval EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Error.
**/

EFI_STATUS
EFIAPI
SetFnvEccModeTarget (
  OUT VOID *DdrtEccMode
  )
{
  DDRT_ECC_MODE_MC_2LM_STRUCT *DdrtEccModePtr;

  DdrtEccModePtr = (DDRT_ECC_MODE_MC_2LM_STRUCT *)DdrtEccMode;

  DdrtEccModePtr->Bits.em = 0x1F;

  return EFI_SUCCESS;

} // SetFnvEccModeTarget

/**
  Build Target function to poll FNV for credit ready.

  @param[in]   Host             Ptr to SysHost.
  @param[in]   Socket           Socket Number.
  @param[in]   SysBootMode      Boot mode.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
FnvPollCreditReadyTarget (
  IN PSYSHOST  Host,
  IN UINT8 Socket,
  IN BootMode SysBootMode
  )
{

  // Wait for NVMCTLR to indicate that Media interface is ready
  if (SysBootMode == NormalBoot) {
    FnvPollingBootStatusRegister (Host, Socket, FNV_MEDIA_DONE);
  }

  // Poll for Credit Ready
  if ((SysBootMode == NormalBoot) && ((Host->var.mem.subBootMode == WarmBoot) || (Host->var.mem.subBootMode == WarmBootFast))) {
    FnvPollingBootStatusRegister (Host, Socket, FNV_CREDIT_READY);
  }

  return EFI_SUCCESS;

} // FnvPollCreditReadyTarget

/**
  Build Target function to poll FNV for credit ready.

  @param[in]  EnabledChannel  Channel.
  @param[out] McCreditCount   Ptr to the count.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
GetMcCreditCountEnabledTarget (
  IN UINT8 EnabledChannel,
  OUT UINT8  *McCreditCount
  )
{

  switch (EnabledChannel) {

    case (1):
      *McCreditCount = MC_CREDIT_COUNT_1_CHANNEL_ENABLED;
      break;

    case (2):

      *McCreditCount = MC_CREDIT_COUNT_2_CHANNELS_ENABLED;
      break;

    case (3):

      *McCreditCount = MC_CREDIT_COUNT_3_CHANNELS_ENABLED;
      break;

    default:

      //Automated add of default case. Please review.
      break;
  }

  return EFI_SUCCESS;

} // GetMcCreditCountEnabledTarget

/**
  Build Target function to set the BIOS reset Cpl.

  @param[in]   Host             Ptr to SysHost.
  @param[in]   Socket           Socket Number.
  @param[in]   Phase            Phase.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
SetBiosResetCplTarget (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Phase
  )
{
  BIOS_RESET_CPL_PCU_FUN1_STRUCT                BiosResetCpl;
  UINT32                                        TimeoutCounter;
  UINT8                                         PcodeInitDone = 0;
  UINT32                                        StartCount = 100;

  if ((PcdGet8 (PcdDfxRstCplBitsEn) & (1 << (Phase - 1))) == 0) {
    return EFI_DEVICE_ERROR;
  }

  BiosResetCpl.Data = UsraCsrRead (Socket, 0, BIOS_RESET_CPL_PCU_FUN1_REG);

  switch (Phase) {

    case SET_CPL_1:
      //
      // Set CPL1: NID MAP, package C-state and S-state BIOS configurations are completed.
      //
      BiosResetCpl.Bits.rst_cpl1 = 1;
      break;

    case SET_CPL_2:
      //
      // Set CPL2: Ready for Periodic RCOMP Starts
      //
      BiosResetCpl.Bits.rst_cpl2 = 1;
      break;

    case SET_CPL_3:
      //
      // Set CPL3: BIOS Power and Thermal Management Configure Completed
      //
      BiosResetCpl.Bits.rst_cpl3 = 1;
      break;

    case SET_CPL_4:
      //
      // Set CPL4: Memory Calibration Completed
      //
      BiosResetCpl.Bits.rst_cpl4 = 1;
      break;

    default:
      break;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "\n:InitMEM - BIOS_RESET_CPL: Set CPL%d on #S%d\n", Phase, Socket);
  UsraCsrWrite (Socket, 0, BIOS_RESET_CPL_PCU_FUN1_REG, BiosResetCpl.Data);

  //
  // wait for PCU acknowledgement for 2ms (20us * 100)
  //
  if (HybridSystemLevelEmulationEnabled() || IsSimicsEnvironment ()) {
    //Change the time out value to 20s for SPR HSLE and Simics (WB model)
    StartCount = 1000000;
  }

  for (TimeoutCounter = StartCount; TimeoutCounter > 0; TimeoutCounter--) {

    BiosResetCpl.Data = UsraCsrRead (Socket, 0, BIOS_RESET_CPL_PCU_FUN1_REG);

    switch (Phase) {

      case SET_CPL_1:
        PcodeInitDone = (UINT8) BiosResetCpl.Bits.pcode_init_done1;
        break;

      case SET_CPL_2:
        PcodeInitDone = (UINT8) BiosResetCpl.Bits.pcode_init_done2;
        break;

      case SET_CPL_3:
        PcodeInitDone = (UINT8) BiosResetCpl.Bits.pcode_init_done3;
        break;

      case SET_CPL_4:
        PcodeInitDone = (UINT8) BiosResetCpl.Bits.pcode_init_done4;
        break;

      default:
        break;
    }

    if (PcodeInitDone == 1) {
      break;
    }

    FixedDelayMicroSecond (20);  //20us
  }

  if ((TimeoutCounter == 0) || (PcodeInitDone == 0)) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "\n:InitMEM - Pcode fails to ACK CPL%d on #S%d\n", Phase, Socket);
    RC_FATAL_ERROR(FALSE, ERR_PCU, PCU_NOT_RESPONDING);
  }

  return EFI_SUCCESS;

}  // SetBiosResetCplTarget

/**
  Build Target function to display Pcode mailbox read Mc freq error.

  @param[in]   Socket           Socket Number.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
DisplayPcodeMailboxReadMcFreqErrorTarget (
  IN UINT8 Socket
  )
{

  RcDebugPrint (SDBG_MINMAX,
                    "Could not able to get Current DCLK and reqType from Pcode mailbox!\n");

  OutputError (ERR_READ_MC_FREQ, ERR_NOT_ABLE_READ_MC_FREQ, Socket, 0xFF, 0xFF, 0xFF);

  return EFI_SUCCESS;

} // DisplayPcodeMailboxReadMcFreqErrorTarget

/**
  Build Target function to display Pcode mailbox Set Mc freq error.

  @param[in]   Host             Ptr to SysHost.
  @param[in]   Socket           Socket Number.
  @param[in]   ReqType          Request type.
  @param[in]   ReqData          Request data.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
DisplayPcodeMailboxSetMcFreqErrorTarget (
  IN PSYSHOST  Host,
  IN UINT8 Socket,
  IN UINT8     ReqType,
  IN UINT8     ReqData
  )
{

  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "PCODE mailbox to Set MC Frequency failed: Boot with previous values!\n");

  return EFI_SUCCESS;

} // DisplayPcodeMailboxSetMcFreqErrorTarget

/**
  Limit the round trip values as needed for a given socket.

  @param[in]  Socket  The target socket

  @retval   EFI_SUCCESS   Round trip values were successfully limited, or they did not need limiting
  @retval   !EFI_SUCCESS  Round trip values needed to be limited, but limiting was not successful
**/
EFI_STATUS
EFIAPI
LimitRoundTripAsNeeded (
  IN  UINT8 Socket
  )
{
  return EFI_SUCCESS;
}
