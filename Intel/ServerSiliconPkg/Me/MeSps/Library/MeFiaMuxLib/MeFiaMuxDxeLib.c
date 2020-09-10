/** @file
  MeFiaMuxLib.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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
#include <Library/PciLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MeFiaMuxLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PchInfoLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/PchFiaLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Ppi/HeciControlPpi.h>
#include <Sps.h>


/**
  Translate PCIe Port/Lane pair to 0-based PCIe lane number.

  @param[in] RpIndex    Root Port index
  @param[in] RpLane     Root Port Lane

  @retval PCIe lane number (0-based)
**/
UINT32
PchPciePhysicalLane (
  UINT32  RpIndex,
  UINT32  RpLane
  )
{
  DEBUG ((DEBUG_ERROR, "%a () : ERROR: (%r)\n", __FUNCTION__, EFI_UNSUPPORTED));
  DEBUG ((DEBUG_ERROR, "File: %a\n", __FILE__));
  ASSERT_EFI_ERROR (EFI_UNSUPPORTED);

  return 0;
}