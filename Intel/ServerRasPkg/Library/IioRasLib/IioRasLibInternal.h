/** @file
  Internal header file for IIO RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2017 Intel Corporation. <BR>

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

#ifndef _IIO_RAS_LIB_H_
#define _IIO_RAS_LIB_H_

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/MpExtensionLib.h>
#include <Library/IioRasLib.h>
#include <Library/PcieRasLib.h>
#include <Library/MmPciExtLib.h>
#include <Library/RasDebugLib.h>
#include <IndustryStandard/Pci22.h>
#include <Library/IoLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/RasMailBoxLib.h>
#include <Library/PcieStdLib.h>
#include <Library/IioSiliconLib.h>
#include <Library/BaseMemoryLib.h>
#include <IioRegs.h>
#include <Register/IioRegDef.h>
#include <Register/PcieRegDef.h>
#include <UncoreCommonIncludes.h>
#include <IndustryStandard/PciExpress21.h>
#include <Register/ArchitecturalMsr.h>
#include <RcRegs.h>
#include <PCIeErrorTypes.h>
#include <Cpu/CpuCoreRegs.h>

// Used to override the AER mask registers
// Set bits that need to be masked due to known issues
#define PCH_AER_UEM_OVERRIDE            0x00008000    // BIT15 must be set to prevent a reset during legacy Windows boot


/**
  Handler for standard PCI to PCI bridge device.

  @param[in] Bus              Device's bus number
  @param[in] Device          Device's device number
  @param[in] Function        Device's function number

  @retval ErrorDetected      Return BOOLEAN as Error is detected

**/
BOOLEAN
EFIAPI
PchP2PElogHandler (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

#endif
