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
#include <Library/MemDdrioIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemRcLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Memory/MemDefaults.h>

/**
  Power Training hook router for MemDdrioIpLib
  General purpose API to handle special cases during power training

  @param[in]  Socket          - Socket number
  @param[in]  PowerTrainHook  - The hook identifier

  @retval  MRC_STATUS_SUCCESS if no failure
  @retval  MRC_STATUS_SIGNAL_NOT_SUPPORTED if hook is not supported
  @retval  !MRC_STATUS_SUCCESS otherwise
**/
MRC_STATUS
EFIAPI
MemPowerTrainingDdrioHook (
  IN  UINT8             Socket,
  IN  POWER_TRAIN_HOOK  PowerTrainHook
  )
{

  //
  // There are no hooks for this platform
  //
  return MRC_STATUS_SIGNAL_NOT_SUPPORTED;
}

/**

  Clean up any chipset bugs related to ReadDqDqsPerBit training

  @param[in] Host   - Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket - Socket Id

  @retval  MRC_STATUS_SUCCESS if no failure
  @retval  !MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
EFIAPI
ReadDqDqsCleanup (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket
)
{
  //
  // Placeholder to satisfy the API definition
  //
  return MRC_STATUS_SUCCESS;
} //ReadDqDqsCleanup

/**

  Return Override Read Preamble timing setting

  @param[in] Socket         - Socket number
  @param[in] PreambleType   - READ_PREAMBLE

  @retval PREAMBLE_1TCLK, PREAMBLE_2TCLK, PREAMBLE_3TCLK, PREAMBLE_4TCLK

**/
UINT8
EFIAPI
GetReadPreambleOverride (
  IN      UINT8   Socket,
  IN      UINT8   ReadPreamble
  )
{
  UINT8             ReadPreambleNew;
  UINT8             ReadPostamble;

  //
  // Return caller’s ReadPreamble by default
  //
  ReadPreambleNew = ReadPreamble;

  if (IsSiliconWorkaroundEnabled ("S1707159028")) {
    //
    // Don't allow 3tck read preamble and 0.5tck read postamble
    //
    ReadPostamble = GetPostambleState (Socket, READ_POSTAMBLE);
    if ((ReadPreamble == PREAMBLE_3TCLK) && (ReadPostamble == POSTAMBLE_0P5TCLK)) {
      ReadPreambleNew = PREAMBLE_2TCLK;
      RcDebugPrint (SDBG_ERROR, "Warning - invalid Read Preamble (0x%x) with Read Postamble (0x%x) - Applying override to Read Preamble (0x%x)\n", ReadPreamble, ReadPostamble, ReadPreambleNew);
    }
  }
  return ReadPreambleNew;
}