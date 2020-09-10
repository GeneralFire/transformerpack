/** @file

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
#include <Library/SysHostPointerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/EmulationConfigurationLib.h>

/**
  Return the preamble timing for DDR5

  @param[in] Socket         - Socket number
  @param[in] PreambleType   - READ_PREAMBLE, WRITE_PREAMBLE or READ_OR_WRITE_PREAMBLE

  @retval PREAMBLE_1TCLK, PREAMBLE_2TCLK, PREAMBLE_3TCLK, PREAMBLE_4TCLK
**/

UINT8
GetPreambleStateDdr5 (
  IN      UINT8   Socket,
  IN      UINT8   PreambleType
  )
{
  PSYSHOST        Host;
  UINT8           PreambleState = PREAMBLE_2TCLK; // for case PREAMBLE_AUTO
  SYS_SETUP       *Setup;

  Setup = GetSysSetupPointer ();
  Host  = GetSysHostPointer ();

  if (IsBrsPresent (Host, Socket)) {
    if ((PreambleType & READ_PREAMBLE) != 0) {
      return PREAMBLE_1TCLK;
    } else if ((PreambleType & WRITE_PREAMBLE) != 0) {
      return PREAMBLE_3TCLK;
    }
  }

  if ((PreambleType & WRITE_PREAMBLE) != 0) {
    if (Setup->mem.writePreamble != PREAMBLE_AUTO) {
      PreambleState = Setup->mem.writePreamble;
    }
  } else if ((PreambleType & READ_PREAMBLE) != 0) {
    if (Setup->mem.readPreamble != PREAMBLE_AUTO) {
      PreambleState = GetReadPreambleOverride (Socket, Setup->mem.readPreamble);
    }
  }
  return PreambleState;
}

/**
  Execute 1N workaround required for emulation only

  @param[in]  Host    - Pointer to SysHost
  @param[in]  Socket  - Socket number

  @retval N/A

**/
VOID
Emulation1nCmdTimingWorkaround (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT8       Channel;
  INT16       PiDelay;
  UINT8       MaxChDdr;

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    //
    // CTE 1N BFM WA.
    //
    MaxChDdr = GetMaxChDdr ();
    for (Channel = 0; Channel < MaxChDdr; Channel++) {
      if (!IsChannelEnabled (Socket, Channel)) {
        continue;
      }
      //
      // Copy CS0 delay to CS1
      //
      if (MRC_STATUS_SUCCESS == GetSetSignalDdr5 (Host, Socket, Channel, DdrLevel, CS0_A_N, GSM_READ_ONLY, &PiDelay)) {
        GetSetSignalDdr5 (Host, Socket, Channel, DdrLevel, CS1_A_N, GSM_FORCE_WRITE, &PiDelay);
      }

      if (MRC_STATUS_SUCCESS == GetSetSignalDdr5 (Host, Socket, Channel, DdrLevel, CS0_B_N, GSM_READ_ONLY, &PiDelay)) {
        GetSetSignalDdr5 (Host, Socket, Channel, DdrLevel, CS1_B_N, GSM_FORCE_WRITE, &PiDelay);
      }
    } // Channel
  } // PcdCteBuild
}

