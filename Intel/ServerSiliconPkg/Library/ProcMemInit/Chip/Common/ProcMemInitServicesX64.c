/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2020 Intel Corporation. <BR>

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
#include <Library/MemoryServicesLib.h>
#include <SysHostChip.h>
#include "Chip10nm/Include/MemHostChip10nm.h"
#include <Library/UncoreLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include "BiosSsaChipFunc.h"
#include "SysHostChipCommon.h"
#include <Library/MemMcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CpuInitLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/KtiApi.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Iio/Include/Library/IioInitLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemDdrioIpLib.h>

#ifdef CDF_SC_FLAG
#include <Library/PmcLib.h>
#endif //CDF_SC_FLAG

#include <Library/IoLib.h>
#include <Ppi/Dwr.h>
#include <Library/PeiServicesLib.h>

#include <Library/PcieMmcfgLib.h>
#include <ScratchpadList.h>
#include <Library/LastBootErrorPeiLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/UpiHostDataLib.h>

#include <Library/PeiServicesLib.h>
#include <ReferenceCodePolicy.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/UsraCsrLib.h>

/**

Executed by System BSP only.
Check global reset reqeust flag and trigger the proper type of reset.

@param Host - pointer to sysHost structure on stack

@retval VOID

**/
VOID
CheckAndHandleResetRequests(
  IN PSYSHOST  Host
  )
{
  return;
}


/**
Function for check Scratchpad0 BIT5 =  ResetFatalErr select

@param[in]      Host    Pointer to the system Host (root) structure
@param[in]      MaxSDBGFunction  - Function select:
MAX_SDBG_FATAL_ERR_CHECK  = 0
MAX_SDBG_FATAL_ERR_SET    = 1
MAX_SDBG_FATAL_ERR_CLEAR  = 2
Returns:

TRUE / FALSE
TRUE  -  Force to Halt on, or Fatal error bit already setted.
FALSE -  BIOS_STICKY_SCRATCHPAD_0 Set BIT5 = ResetFatalErr for Maximum serial debug message with next boot.
**/
BOOLEAN
FatalErrSetMaxSDBGSelect(
PSYSHOST  Host,
UINT8   MaxSDBGFunction
)
{
  BOOLEAN                    Result = TRUE;

#ifdef DEBUG_CODE_BLOCK
  BIOS_SCRATCHPAD0_STRUCT    Spad0;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  Spad0.Data = UsraCsrRead (0, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG);
  switch (MaxSDBGFunction) {
  case MAX_SDBG_FATAL_ERR_CHECK:
    if (Spad0.Bits.ResetFatalErr == 1) {
      //
      // Clear it and Halt on fatal error if bit already set.
      //
      Spad0.Bits.ResetFatalErr = 0;
      UsraCsrWrite (0, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG, Spad0.Data);
      Result = TRUE;
    }
    else {
      //
      // BIOS_STICKY_SCRATCHPAD_0 Set BIT5 = ResetFatalErr with setting to Maximum
      //
      Spad0.Bits.ResetFatalErr = 1;
      UsraCsrWrite (0, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG, Spad0.Data);
      //
      // Request and do warm reset
      //
      Result = FALSE;
    }
    break;

  case MAX_SDBG_FATAL_ERR_SET:
    if (Spad0.Bits.ResetFatalErr == 1) {
      //
      // Set seial debug message level to SDBG_MAX
      //
      SetDebugLevel (GetDebugLevel () | SDBG_INFO | SDBG_DEFAULT);
      //
      // and Disable multi-threading MRC
      //
      RcDebugPrint (SDBG_DEFAULT, "Disable multi-threading MRC.\n");
      Setup->mem.options &= ~MULTI_THREAD_MRC_EN;
    }
    break;

  case MAX_SDBG_FATAL_ERR_CLEAR:
    if (Spad0.Bits.ResetFatalErr == 1) {
      //
      // if Fatal error reset pass MRC, clear Scratchpad0 BIT5 - ResetFatalErr
      // The debug level to max for this boot cycle only, and need clear BIOSSCRATCHPAD0 BIT4 for Debug level  return to the level set by Setup.
      //
      Spad0.Bits.ResetFatalErr = 0;
      UsraCsrWrite (0, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG, Spad0.Data);
      RcDebugPrint (SDBG_DEFAULT, "Debug level should return to the level set by Setup on the next boot!\n");
    }
    break;

  default:
    Result = TRUE;
    break;
  }
#endif // DEBUG_CODE_BLOCK

  return Result;
}
