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

#include <Library/MemoryCoreLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemoryServicesLib.h>

#include <ReferenceCodeFatalErrors.h>


/**

  Check if Current setup Freq limit same as DIMM init freq during warm reboot, and log an error if not same.

  @param[in] Host     - Point to sysHost
  @param[in] Socket   - CPU socket ID

  @retval None

**/
VOID
CheckDdrtFreqChange (
  IN PSYSHOST       Host,
  IN UINT8          Socket
  )
{

  if ((GetSysBootMode () != S3Resume) && (Host->var.mem.subBootMode == ColdBoot)) {
    if (FmcCheckDdrtFreqChanged (Host, Socket) == TRUE) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "DDRT Power cycle is required after frequency changed.\n");
      RC_FATAL_ERROR(FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_182);
    }
  }

}

