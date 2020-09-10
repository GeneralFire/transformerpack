/** @file
  Header file for CpuPcieRpLib.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation.

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

@par Specification Reference:
**/
#ifndef _CPU_PCIERP_LIB_H_
#define _CPU_PCIERP_LIB_H_

#include <Ppi/SiPolicy.h>
#include <Library/HobLib.h>

#pragma pack(1)

typedef struct {
  UINT8   Segment;
  UINT8   Bus;
  UINT8   Device;
  UINT8   Function;
  BOOLEAN Enable;
} CPU_PCIE_RP_INFO;

#pragma pack()

/**
  Determines whether L0s is supported on current stepping.

  @return TRUE if L0s is supported, FALSE otherwise
**/
//@TODO : Check if this API is required for SA - if not remove this API
BOOLEAN
IsCpuPcieL0sSupported (
  VOID
  );

/**
  Some early PCH steppings require Native ASPM to be disabled due to hardware issues:
   - RxL0s exit causes recovery
   - Disabling PCIe L0s capability disables L1
  Use this function to determine affected steppings.

  @return TRUE if Native ASPM is supported, FALSE otherwise
**/
//@TODO : Check if this API is required for CPU - if not remove this API
BOOLEAN
IsCpuPcieNativeAspmSupported (
  VOID
  );

/**
  Determines whether PCIe link is active

  @param[in] RpBase    Root Port base address
  @retval Link Active state
**/
BOOLEAN
CpuPcieIsLinkActive (
  UINT64  RpBase
  );

/**
  Get current PCIe link speed.

  @param[in] RpBase    Root Port base address
  @return    Link speed
**/
UINT32
CpuPcieGetLinkSpeed (
  UINT64  RpBase
  );

/**
  Get max PCIe link speed supported by the root port.

  @param[in] RpBase    Root Port pci segment base address
  @return    Max link speed
**/
UINT32
CpuPcieGetMaxLinkSpeed (
  UINT64 RpBase
  );

/**
  This function checks if Legacy Gen3 PCIe controller is supported.

  @retval TRUE  - Legacy Gen3 controller supported
  @retval FALSE - Legacy Gen3 controller not supported
**/
BOOLEAN
IsCpuPcieGen3 (
  VOID
);
#endif // _CPU_PCIERP_LIB_H_
