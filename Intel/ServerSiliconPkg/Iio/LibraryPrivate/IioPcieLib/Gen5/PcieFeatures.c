/** @file
  Miscellaneous PCIE features implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <PcieRegs.h>
#include <Chip/Include/SysHostChip.h>

#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <Library/IioSysInfoLib.h>

#include <LibraryPrivate/IioPcieLib.h>

/**
  This function is to hide unused root ports.

  @param[in] IioIndex         Socket/IIO complex index
  @param[in] Stack            Stack Index
  @param[in] HideData         Pointer to data to write into register
**/
VOID
IioHideUnusedRootPorts (
  IN  UINT8                   IioIndex,
  IN  UINT8                   Stack,
  IN  UINT32                  *HideData
  )
{
  //
  // Currently it's being done in SecureMethodSpr.c file
  //
  return;
}
