/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2018 Intel Corporation. <BR>

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

#include <Library/MemoryServicesLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <RcRegs.h>
#include <ScratchpadList.h>
#include <Library/UsraCsrLib.h>

/**

  Hook to override debug levels

  @param[in] Host   - Pointer to syshost structure
  @param[in] Socket - Socket number

**/
VOID
OverrideDebugLevels (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  BIOS_SCRATCHPAD0_STRUCT Sp0;
  SYS_SETUP *Setup = GetSysSetupPointer ();
  BOOLEAN ChangedSettings = TRUE;
  UINT32 MrcDebugLevel = SDBG_DEFAULT;
  UINT32 SystemDebugLevel = SDBG_ERROR;

  Sp0.Data = UsraCsrRead (Socket, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG);

  switch (Sp0.Bits.DebugOverride) {
  case 7:
    MrcDebugLevel = SDBG_BSSA | SDBG_DIMM_INFO;
    SystemDebugLevel = 0x00000000;
    break;
  case 6: // MRC/System 'normal' + TRACE + FORCE_ALL
    MrcDebugLevel |= SDBG_TRACE | SDBG_FORCE_ALL;
  case 5: // MRC 'normal' + MARGINS, System 'normal'
    MrcDebugLevel |= SDBG_MARGINS;
    SystemDebugLevel |= PcdGet32(PcdDebugPrintErrorLevel);
  case 4: // MRC 'normal', System 'minimum'
    MrcDebugLevel |= SDBG_MAX | SDBG_MIN;
    break;
  case 3: // MRC/System 'miminum' + debug jumper
    Setup->common.debugJumper = 1;
  case 2: // MRC/System 'minimum'
    MrcDebugLevel |= SDBG_MIN;
    break;
  case 1: // No messages
    MrcDebugLevel = SDBG_NONE;
    SystemDebugLevel = 0x00000000;
    break;
  default:
    ChangedSettings = FALSE;
    break;
  }

  if (ChangedSettings) {
    SetDebugPrintErrorLevel (SystemDebugLevel);
    SetDebugLevel (MrcDebugLevel);
  }
} // OverrideDebugLevel
