/** @file
  HECI Access driver Initialization information for use in PEI, DXE, and SMM.

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 - 2020 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

**/
#include <PiPei.h>
#include <Uefi.h>
#include <Register/MeRegs.h>
#include <Library/HeciAccessCoreLib.h>

#include "HeciAccessDeviceConfig.h"

/**
  ME device buffers default locations

  ME has one PCI device in PCH bus 0 with multiple functions.
  For each function MBAR address for communication buffer
  is required. Before PCI Enumerator MBAR is not set and HECI driver for sending
  and receiving uses temporary MBAR. One for PEI/DXE and second for SMM.
  (defined MBAR enables HECI_ACCESS_PRIVATE interface for given function PEI/DXE/SMM).
    Segment,
    Bus,
    Device,
    Function,
    HeciInterruptMode,
    MemoryBar (PEI),
    MemoryBar (DXE),
    MemoryBar (SMM)
**/


CONST HECI_ACCESS_DEVICE_INITIALIZATION_DEFINITION  mHeciDeviceInitInfo[] = {
                                                      { // HECI 1
                                                        ME_DEFAULT_SEGMENT,
                                                        ME_DEFAULT_BUS,
                                                        ME_DEFAULT_DEV,
                                                        HECI1_DEVICE,
                                                        V_ME_HIDM_MSI+B_ME_HIDM_L,
                                                        HECI_DEFAULT_MBAR_PEI_DXE,
                                                        HECI_DEFAULT_MBAR_PEI_DXE,
                                                        HECI_DEFAULT_MBAR_SMM
                                                      },
                                                      { // HECI 2
                                                        ME_DEFAULT_SEGMENT,
                                                        ME_DEFAULT_BUS,
                                                        ME_DEFAULT_DEV,
                                                        HECI2_DEVICE,
                                                        V_ME_HIDM_SCI+B_ME_HIDM_L,
                                                        HECI_DEFAULT_MBAR_PEI_DXE,
                                                        HECI_DEFAULT_MBAR_PEI_DXE,
                                                        HECI_DISABLED_MBAR
                                                      },
                                                      { // HECI 3
                                                        ME_DEFAULT_SEGMENT,
                                                        ME_DEFAULT_BUS,
                                                        ME_DEFAULT_DEV,
                                                        HECI3_DEVICE,
                                                        V_ME_HIDM_MSI+B_ME_HIDM_L,
                                                        HECI_DISABLED_MBAR,
                                                        HECI_DISABLED_MBAR,
                                                        HECI_DEFAULT_MBAR_SMM
                                                      },
                                                      { // HECI 4
                                                        ME_DEFAULT_SEGMENT,
                                                        ME_DEFAULT_BUS,
                                                        ME_DEFAULT_DEV,
                                                        HECI4_DEVICE,
                                                        V_ME_HIDM_MSI+B_ME_HIDM_L,
                                                        HECI_DISABLED_MBAR,
                                                        HECI_DISABLED_MBAR,
                                                        HECI_DISABLED_MBAR
                                                      }
                                                    };

/**
  Function returns initialization table information

  @param [out] HeciDeviceInitInfoReturn pointer to configuration table

  @retval Number of HECI interfaces definition stored in configuration table
**/
UINT8
GetHeciInitializationInfo (
  OUT HECI_ACCESS_DEVICE_INITIALIZATION_DEFINITION  **HeciDeviceInitInfoReturn
)
{
  if (HeciDeviceInitInfoReturn != NULL) {
    *HeciDeviceInitInfoReturn = (HECI_ACCESS_DEVICE_INITIALIZATION_DEFINITION*)mHeciDeviceInitInfo;
  }
  return sizeof (mHeciDeviceInitInfo)/sizeof (HECI_ACCESS_DEVICE_INITIALIZATION_DEFINITION);
}
