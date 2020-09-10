/** @file
  Mesh2Mem Emulation related functions

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

//
// Include files
//
#include <Library/SysHostPointerLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <MemHost.h>
#include "Include/Mesh2MemRegs.h"
#include <Library/EmulationConfigurationLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/KtiApi.h>
#include <Library/CsrAccessLib.h>
#include <Library/UsraCsrLib.h>
//
// Channel 0 credit config done bit mask for cmi misc config register
//
#define CHANNEL0_CREDIT_CONFIG_DONE_BIT_MASK BIT2
//
// Channel 1 credit config done bit mask for cmi misc config register
//
#define CHANNEL1_CREDIT_CONFIG_DONE_BIT_MASK BIT3

//
// Polling bit location mask for Mesh2Mem CMI Misc register
//
const UINT32 PollBitLocation[] = { CHANNEL0_CREDIT_CONFIG_DONE_BIT_MASK, 
                                   CHANNEL1_CREDIT_CONFIG_DONE_BIT_MASK};

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
M2mPollRegisterOffsetForEmulation (
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
    RcDebugPrint (SDBG_DEFAULT, "mov eax, DWORD PTR ds:[0%8xh]\n", RegAddr);
    if (Logic == 0) {
      RcDebugPrint (SDBG_DEFAULT, "not eax\n");
    }
    RcDebugPrint (SDBG_DEFAULT, "and eax, 0%08xh\n", Mask);
    RcDebugPrint (SDBG_DEFAULT, "cmp eax, 0%08xh\n", Mask);
    if (CheckCurrentReset (POST_RESET_POWERGOOD))  {
      RcDebugPrint (SDBG_DEFAULT, "jnz LABEL_c_mbox%d\n", LabelNumber);
    } else {
      RcDebugPrint (SDBG_DEFAULT, "jnz LABEL_w_mbox%d\n", LabelNumber);
    }
  }
}

/**

  Poll CMI Misc register for Init done. This function generates ASM polling logic
  if FCE bios enabled

  @param[in] Socket   - Socket number
  @param[in] McId     - Memory controller Id

  @retval  N/A

**/
VOID
PollM2mCmiMiscCfgChInit (
  IN UINT8    Socket,
  IN UINT8    McId
  )
{
  PSYSHOST Host;
  struct channelNvram(*ChannelNvList)[MAX_CH];
  UINT32 Offset;
  UINT8 Ch = 0;
  UINT8 ChId = 0;
  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  Offset = CMIMISCCONFIG_M2MEM_MAIN_REG;

  //Set Initialized bit and wait for done
  if (GetUbiosOutputMode () == ASM_OUTPUT_ENABLE) {
    for (ChId = 0; ChId < MAX_MC_CH; ChId++) {
      Ch = (McId * MAX_MC_CH) + ChId;
      if ((*ChannelNvList) [Ch].enabled == 1) {
        M2mPollRegisterOffsetForEmulation (Host, Socket, McId, Offset, PollBitLocation[ChId], 1);
      }
    }
  }
}
