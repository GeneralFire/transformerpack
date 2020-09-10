/** @file
  Implements Memory TSOD feature

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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
#include <SysHost.h>
#include <SysHostChip.h>
#include <Library/SysHostPointerLib.h>
#include <SysRegs.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/KtiApi.h>
#include <Library/SystemInfoLib.h>
#include <Include/MemTsod.h>
#include <Library/MemThermManageTargetLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/UsraCsrLib.h>

/**

  This function Initializes registers for Memory TSOD feature

  @param[in]  Socket               Socket Number, 0 based
  @param[in]  Channel              Channel Number, 0 based
  @param[in]  Dimm                 Dimm Number, 0 based

  @retval N/A

**/
VOID
InitializeTsod (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm
  )
{
  InitializeTsodTarget (Socket, Channel, Dimm);
  return;
} // InitializeTsod

/**

  Check if dimm is TSOD complaint, if not disable Closed Loop Thermal Throttling

  @param[in]  Socket               Socket Number

  @retval N/A

**/
VOID
CheckForTsodDimm (
  IN UINT8 Socket
  )
{
  UINT8     Dimm;
  UINT8     Channel;
  UINT8     DimmTsMask;
  PSYSHOST  Host = NULL;
  struct dimmNvram(*DimmNvList)[MAX_DIMM];
  struct channelNvram(*ChannelNvList)[MAX_CH];
  SYS_SETUP *Setup;
  UINT8     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 1) {

      DimmNvList = GetDimmNvList (Host, Socket, Channel);

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }
        //
        // set TSOD present bit if a TSOD is present on the DIMM
        //
#ifdef DDR5_SUPPORT
        if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
          DimmTsMask = SPD_DIMM_TS_MASK_DDR5;
        } else
#endif  // #ifdef DDR5_SUPPORT
        {
          DimmTsMask = BIT7;
        }
        if ((((*DimmNvList)[Dimm].dimmTs & DimmTsMask) == 0) && Setup->mem.thermalThrottlingOptions & (CLTT_EN | CLTT_PECI_EN)) {
          //
          // here CLTT is enabled on input but a dimm is present with no thermal sensor...so
          // disable CLTT and print a warning message
          //

          Setup->mem.thermalThrottlingOptions &= ~(CLTT_EN | CLTT_PECI_EN);
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "\n:MPM: CLTT is enabled on input but a dimm is present with no thermal sensor. So, disable CLTT.\n");
        }
      }
    }
  }
  return;
}  // CheckForTsodDimm

/**

  This function configures registers for Memory TSOD feature

  @param[in]  Socket               Socket Number

  @retval N/A

**/
VOID
EFIAPI
EnableTsod (
  IN UINT8    Socket
  )
{
#ifndef GNRSRF_HOST // GNR_TODO HSD 14011798074
  UINT8     McId;
  UINT8     FPGALoadAttempted;
  UINT32    TsodPollRate = DEFAULT_TSOD_POLL_RATE;
  PSYSHOST  Host = NULL;
  SYS_SETUP *Setup;
  UINT8     MaxImc;
  UINT8     OriginalUbiosOutputMode;

  Setup = GetSysSetupPointer ();

  Host   = GetSysHostPointer ();
  MaxImc = GetMaxImc ();

  if ((Setup->mem.thermalThrottlingOptions & CLTT_PECI_EN) != 0) {
    //Set poll rate to 0 as per whitley white paper for CLTT_PECI_EN
    TsodPollRate = 0;
  }

  SetTsodPollRateSmb (Socket, 0, TsodPollRate);

  if (Host->nvram.mem.socket[Socket].mcpPresent == 1) {
    SetTsodPollRateSmb (Socket, 1, TsodPollRate);
  }

  FPGALoadAttempted = (UINT8) (BIT5 & UsraCsrRead (GetSysSbspSocketIdNv (), 0, BIOSSCRATCHPAD1_UBOX_MISC_REG));

  OriginalUbiosOutputMode = EnableI3cAsmOutput (TRUE);

  for (McId = 0; McId < MaxImc; McId++) {
    if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
      continue;
    }

    UpdateTsodPopulation (Socket, McId);
    if ((Setup->mem.thermalThrottlingOptions & CLTT_EN) != 0) {
      if (IfFpgaActive (Socket) && FPGALoadAttempted == 0) {
        SetTsodPollEnSmb (Socket, GetProcSmbInstanceFromMcId (McId), 0);
      } else {
        SetTsodPollEnSmb (Socket, GetProcSmbInstanceFromMcId (McId), 1);
      }
    } // CLTT_EN
  } // McId

  RestoreI3cAsmOutput (OriginalUbiosOutputMode, TRUE);
  return;
#endif // GNRSRF_HOST
} // EnableTsod
