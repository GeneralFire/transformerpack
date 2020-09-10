/** @file
  Routes to register includes

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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
#ifndef __RC_REGS_H__
#define __RC_REGS_H__

#include <RegPath.h>
#include REG_PATH(Registers.h)

//
// These specials were in RegisterDefinitions.h
//

#if defined(SPR_HOST) || defined(GNRSRF_HOST)
// register at B:D:F: 30:0:1:F00 with SoftSim flag for SPR
// Scratch register usage
//
#define CSR_EMULATION_FLAG_OFFSET  0xF00
#define CSR_EMULATION_FLAG_UBOX_CFG_REG  ((BOX_UBOX << 24) | (BOX_FUNC_UBOX_DFX << 20) | (2 << 16) | CSR_EMULATION_FLAG_OFFSET)
#else
// register at B:D:F: 0:0:0:FC with SoftSim flag
// Scratch register usage
//
#define CSR_EMULATION_FLAG_OFFSET  0xFC
#define CSR_EMULATION_FLAG_UBOX_CFG_REG (BIT31 | BIT30 | (BOX_IIO_PCIE_DMI << 24) | (2 << 16) | CSR_EMULATION_FLAG_OFFSET) //Bit31/Bit30 indicate 10nm/Skx support in CSR Access routine
#define   EMULATION_INFO_UBOX_CFG_REG   ((BOX_IIO_PCIE_DMI << 24) | (2 << 16) | CSR_EMULATION_FLAG_OFFSET)
#endif // SPR_HOST

#define   EMULATION_FLAG_CSR_BOXINSTANCE  0

//
// These specials were included by the old RcRegs.h
//

#if defined(SNR_HOST) || defined(ICX_HOST) || defined(ICXDE_HOST) || defined(SPR_HOST) || defined(GNRSRF_HOST)

#include "Chip10nm/Include/RcRegs10nm.h"

#else // #if defined(SNR_HOST) || defined(ICX_HOST) || defined(ICXDE_HOST) || defined(SPR_HOST)

#include "ChipSkx/Include/RcRegsSkx.h"

#endif // SKX_HOST

#endif // __RC_REGS_H__

