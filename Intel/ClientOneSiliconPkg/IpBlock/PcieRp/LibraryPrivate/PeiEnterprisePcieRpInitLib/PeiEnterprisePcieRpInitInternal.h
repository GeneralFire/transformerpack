/** @file
  Internal header for PeiEnterprisePcieRpInit library.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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

#ifndef _PEI_ENTERPRISE_PCIE_RP_INIT_INTERNAL_H_
#define _PEI_ENTERPRISE_PCIE_RP_INIT_INTERNAL_H_

#include <Library/PcieRpHandleLib.h>

/**
  This function creates Capability and Extended Capability List

  @param[in] RDevice                  Pointer to PCIE RP device
  @param[in] PcieRpCommonConfig       Pointer to a PCIE_ROOT_PORT_COMMON_CONFIG that provides the platform settings
**/
VOID
InitCapabilityList (
  IN       PCH_RP_DEVICE                 *RpDevice,
  IN CONST PCIE_ROOT_PORT_COMMON_CONFIG  *PcieRpCommonConfig
  );

/**
  Returns the PCIe controller bifurcation configuration

  @param[in] RpHandle                  Pointer to PCIE RP handle

  @retval PCIe controller configuration
**/
PCIE_BIFURCATION_CONFIG
PchPcieGetBifurcationConfig (
  IN  PCH_RP_HANDLE           *RpHandle
  );

/**
  Get max link width.

  @param[in] RpBase    Root Port base address
  @retval Max link width
**/
UINT8
GetMaxLinkWidth (
  UINT64  RpBase
  );

/**
  Transition root port link to link active state. This function
  is also responsible for setting up equalization for link speeds
  that require it.

  @param[in] RpHandle            Pointer to PCIE RP handle
  @param[in] PcieLinkEqSettings  Structure containing equalization settings for root port
**/
VOID
PcieRpTrainToLinkActive (
  IN PCH_RP_HANDLE                   *RpHandle,
  IN ENTERPRISE_PCIE_LINK_EQ_CONFIG  *PcieLinkEqSettings
  );

/**
  This function reads Pci Config register via SBI Access

  @param[in]  RpHandle            Pointer to PCIE RP handle
  @param[in]  Offset              Offset of Config register
  @param[out] *Data32             Value of Config register

  @retval EFI_SUCCESS             SBI Read successful.
**/
EFI_STATUS
PchSbiRpPciRead32 (
  IN    PCH_RP_HANDLE *RpHandle,
  IN    UINT32  Offset,
  OUT   UINT32  *Data32
  );

/**
  This function And then Or Pci Config register via SBI Access

  @param[in]  RpHandle            Pointer to PCIE RP handle
  @param[in]  Offset              Offset of Config register
  @param[in]  Data32And           Value of Config register to be And-ed
  @param[in]  Data32AOr           Value of Config register to be Or-ed

  @retval EFI_SUCCESS             SBI Read and Write successful.
**/
EFI_STATUS
PchSbiRpPciAndThenOr32 (
  IN  PCH_RP_HANDLE *RpHandle,
  IN  UINT32  Offset,
  IN  UINT32  Data32And,
  IN  UINT32  Data32Or
  );

/**
  This function And then Or Pci Memory register via SBI Access

  @param[in]  RpHandle            Pointer to PCIE RP handle
  @param[in]  Offset              Offset of Config register
  @param[in]  Data32And           Value of Config register to be And-ed
  @param[in]  Data32AOr           Value of Config register to be Or-ed

  @retval EFI_SUCCESS             SBI Read and Write successful.
**/
EFI_STATUS
PchSbiRpPciMemAndThenOr32 (
  IN  PCH_RP_HANDLE *RpHandle,
  IN  UINT64  Offset,
  IN  UINT32  Data32And,
  IN  UINT32  Data32Or
  );

#endif
