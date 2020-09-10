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
#include <Mem/Library/MemSmeeIpLib/Include/MemSmeeRegs.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemMcIpTargetLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/IoAccessLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/KtiApi.h>
#include <Library/CsrAccessLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/MemRcLib.h>
#include <Library/CpuAndRevisionLib.h>
//
// Workaround for SMEE CFG programming for Model 0p8.
//

#define SMEE_CMI_N0_MC0_CH0  0xFB908040
#define SMEE_CMI_N0_MC0_CH1  0xFB90A040
#define SMEE_CMI_N0_MC0_CH2  0xFB90C040
#define SMEE_CMI_N0_MC1_CH0  0xFB988040
#define SMEE_CMI_N0_MC1_CH1  0xFB98A040
#define SMEE_CMI_N0_MC1_CH2  0xFB98C040

/**

  Generate ASM polling loop for given register offset. Takes mask and logic as input
  for polling.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket Id
  @param[in] BoxInst - Box Instance, 0 based
  @param[in] Offset  - Register offset; values come from the auto generated header file
  @param[in] Mask    - Bit mask of bits we are polling
  @param[in] Logic   - Polling for 0 or 1

  @retval None

**/
VOID
PollRegisterOffsetForEmulation (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    BoxInst,
  IN UINT32   Offset,
  IN UINT32   Mask,
  IN UINT8    Logic
  )
{
  CSR_OFFSET RegOffset;
  UINT8      *RegAddr;
  UINT8      Size;
  UINT32     LabelNumber;

  RegOffset.Data = Offset;
  Size = (UINT8)RegOffset.Bits.size;
  RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (Socket, BoxInst, Offset);
  if (GetUbiosLoopbackLabelNumber (&LabelNumber) == EFI_SUCCESS) {
    LabelPrint (LabelNumber);
    SetUbiosLoopbackLabelNumber (LabelNumber + 1);
    RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "mov eax, DWORD PTR ds:[0%8xh]\n", RegAddr);
    if (Logic == 0) {
      RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "not eax\n");
    }
    RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "and eax, 0%08xh\n", Mask);
    RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "cmp eax, 0%08xh\n", Mask);
    if (CheckCurrentReset (POST_RESET_POWERGOOD))  {
      RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "jnz LABEL_c_mbox%d\n", LabelNumber);
    } else {
      RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "jnz LABEL_w_mbox%d\n", LabelNumber);
    }
  }
}

/**
  Build Target function to do an Io reset delay.

  @param[in]   Host             Ptr to SysHost.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
IoResetDelayTarget (
  IN PSYSHOST Host
  )
{

  //
  // Wait 20 QCLK with reset high.
  //
  FixedQClkDelay (Host, 20);

  return EFI_SUCCESS;

} // IoResetDelayTarget

/**
  Build Target function to wait for a Comp cycle to complete.

  @param[in]   Host             Ptr to SysHost.
  @param[in]   Socket           Socket Number.
  @param[in]   McId             Memory Controller.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
WaitForCompCycleTarget (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 McId
  )
{
  RCOMP_GLOBALCTRL1_MC_GLOBAL_STRUCT  RcompGlobalCtrl;

  //
  // Wait until comp cycle completes on all eanbled channels on the MC
  //
  do {
    //
    // RCOMP_GLOBALCTRL1 aggregates rcomp_in_progress and first_rcomp_done for all channels on the MC
    //
    RcompGlobalCtrl.Data  = MemReadPciCfgMC (Socket, McId, RCOMP_GLOBALCTRL1_MC_GLOBAL_REG);

    if (GetEmulation () & SIMICS_FLAG) {
      break;
    }
  } while ((RcompGlobalCtrl.Bits.rcomp_in_progress == 1) || (RcompGlobalCtrl.Bits.first_rcomp_done == 0));

  return EFI_SUCCESS;

} // WaitForCompCycleTarget

/**
  Build Target function to wait for Cmi misc channels to complete.

  @param[in]   Host             Ptr to SysHost.
  @param[in]   Socket           Socket Number.
  @param[in]   McId             Memory Controller.
  @param[out]  Status           Ptr to the status.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
WaitForCmiMiscConfigChannelsCompleteTarget (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 McId,
  OUT UINT32 *Status
  )
{
  UINT64 StartCount;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT32 Channel0Initialized;
  UINT32 Channel1Initialized;
  UINT32 Channel2Initialized;
  UINT8  ChCreditInited;
  UINT8 Ch = 0;
  UINT8 NumChPerMc;

  Channel0Initialized = 1;
  Channel1Initialized = 1;
  Channel2Initialized = 1;
  ChCreditInited = 0;
  NumChPerMc = GetNumChannelPerMc ();

  if(GetEmulation() == SIMICS_FLAG) {
    PollM2mCmiMiscCfgChInit (Socket, McId);
    return EFI_SUCCESS;
  }
  ChannelNvList = GetChannelNvList (Host, Socket);

  StartCount = GetCountTsc ();

  do {

    if (GetDelayTsc (StartCount) > CATCHALL_TIMEOUT) {

      if (Channel0Initialized == 0) {

        Channel0Initialized = 1;  //Reset Variable for next loop
        *Status = 1; //Indicate function failure
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n InitCmiCreditProgramming: CmiMiscConfig Channel %d not set \n", ((McId * NumChPerMc) + 0));
        RC_FATAL_ERROR (FALSE, ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);
        FatalError (ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);
      }

      if (Channel1Initialized == 0) {

        Channel1Initialized = 1; //Reset Variable for next loop
        *Status = 1; //Indicate function failure
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n InitCmiCreditProgramming: CmiMiscConfig Channel %d not set \n", ((McId * NumChPerMc) + 1));
        RC_FATAL_ERROR (FALSE, ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);
        FatalError (ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);
      }

      if (GetNumChannelPerMc() > 2) {
        if (Channel2Initialized == 0) {
          Channel2Initialized = 1; //Reset Variable for next loop
          *Status = 1; //Indicate function failure
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n InitCmiCreditProgramming: CmiMiscConfig Channel %d not set \n", ((McId * NumChPerMc) + 2));
          RC_FATAL_ERROR (FALSE, ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);
          FatalError (ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);
        }
      }
      break;
    }

    ChCreditInited = GetM2mCmiMiscCfgChCreditInitedMask (Socket, McId);
    Ch = (McId * GetNumChannelPerMc());
    Channel0Initialized = ((*ChannelNvList)[Ch].enabled) ? ((ChCreditInited & CREDIT_INITED_CH_0) != 0) : 1;
    if (GetNumChannelPerMc() > 1) {
      Ch++;
      Channel1Initialized = ((*ChannelNvList)[Ch].enabled) ? ((ChCreditInited & CREDIT_INITED_CH_1) != 0) : 1;
      if (GetNumChannelPerMc() > 2) {
        Ch++;
        Channel2Initialized = ((*ChannelNvList)[Ch].enabled) ? ((ChCreditInited & CREDIT_INITED_CH_2) != 0) : 1;
      } else {
        Channel2Initialized = 1;
      }
    } else {
      Channel2Initialized = 1;
      Channel1Initialized = 1;
    }
  } while (!((Channel0Initialized == 1) && (Channel1Initialized == 1) && (Channel2Initialized == 1)));

  return EFI_SUCCESS;

} // WaitForCmiMiscConfigChannelsCompleteTarget

/**
  Build Target function to wait for Cmi misc channels to complete.

  @param[in]   Host                   Ptr to SysHost.
  @param[in]   Socket                 Socket Number.
  @param[in]   McId                   Memory Controller.
  @param[in]   Config                 Config.
  @param[in]   McMiscCmiCreditConfig  Ptr to the Misc Cmi Credit config struct.
  @param[in]   Address                Address.
  @param[out]  Status                 Ptr to the status.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.

**/
EFI_STATUS
EFIAPI
WaitForMiscCmiCreditConfigCompleteTarget (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT8 Config,
  IN VOID *McMiscCmiCreditConfig,
  IN UINT32 Address,
  OUT UINT32 *Status
  )
{
  UINT64 StartCount;
  UINT8 Ch = 0;
  MCMISC_CMI_CREDIT_CONFIG_0_MC_GLOBAL_STRUCT *McMiscCmiCreditConfigPtr;

  McMiscCmiCreditConfigPtr = (MCMISC_CMI_CREDIT_CONFIG_0_MC_GLOBAL_STRUCT *)McMiscCmiCreditConfig;
  Ch = (McId * GetNumChannelPerMc()) + Config;
  if(GetEmulation () == SIMICS_FLAG) {
    if (GetUbiosOutputMode () == ASM_OUTPUT_ENABLE) {
      PollRegisterOffsetForEmulation (Host, Socket, Ch, Address, BIT29 | BIT30, 1);
    }
    return EFI_SUCCESS;
  }

  StartCount = GetCountTsc ();

  do {

    if (GetDelayTsc (StartCount) > CATCHALL_TIMEOUT) {

      *Status = 1; //Indicate function failure

      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n InitCmiCreditProgramming: McMiscCmiCreditConfig[%d] initialize failed \n", Config);
      RC_FATAL_ERROR (FALSE, ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);
      FatalError (ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);
      break;
    }

    McMiscCmiCreditConfigPtr->Data = MemReadPciCfgEp (Socket, Ch, Address);

  } while (!((McMiscCmiCreditConfigPtr->Bits.rsp_ism_credits_initialized == 1) &&
             (McMiscCmiCreditConfigPtr->Bits.req_ism_credits_initialized == 1)));

  return EFI_SUCCESS;

} // WaitForMiscCmiCreditConfigCompleteTarget

/**
  Build Target function to wait for Mc Cmi Req Cpcfg to complete.

  @param[in]   Host             Ptr to SysHost.
  @param[in]   Socket           Socket Number.
  @param[in]   Channel          Channel.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.

**/
EFI_STATUS
EFIAPI
WaitForMcCmiReqCpcfgCompleteTarget (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel
  )
{
  UINT64 StartCount;
  MCCMIREQCPCFG_N1_MC_2LM_STRUCT McCmiReqCpcfg;
  UINT32 PollRegMask;

  if (GetEmulation () == SIMICS_FLAG) {
    if (GetUbiosOutputMode () == ASM_OUTPUT_ENABLE) {
      if (IsCpuAndRevision (CPU_SPRSP, REV_ALL))  {
        PollRegMask = BIT14;
      } else {
        PollRegMask = BIT22;
      }
      PollRegisterOffsetForEmulation (Host, Socket, Channel, MCCMIREQCPCFG_N1_MC_2LM_REG, PollRegMask, 1);
    }
    return EFI_SUCCESS;
  }

  StartCount = GetCountTsc ();

  do {

    if (GetDelayTsc (StartCount) > CATCHALL_TIMEOUT) {

      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n InitCmiCreditProgramming: McCmiReqCpcfg initialize failed \n");
      RC_FATAL_ERROR (FALSE, ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);
      FatalError (ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);

      return EFI_TIMEOUT;
    }

    McCmiReqCpcfg.Data = MemReadPciCfgEp (Socket, Channel, MCCMIREQCPCFG_N1_MC_2LM_REG);

  } while (McCmiReqCpcfg.Bits.req_credits_initialized == 0);

  return EFI_SUCCESS;

} // WaitForMcCmiReqCpcfgCompleteTarget
