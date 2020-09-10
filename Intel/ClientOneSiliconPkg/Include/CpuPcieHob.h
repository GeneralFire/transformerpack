/** @file
  The GUID definition for CpuPcieHob

@copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation.

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
#ifndef _CPU_PCIE_HOB_H_
#define _CPU_PCIE_HOB_H_

#include <Base.h>
#include <CpuPcieInfo.h>
#include <CpuPcieConfig.h>

extern EFI_GUID gCpuPcieHobGuid;
#pragma pack (push,1)

/**
  The CPU_PCIE_HOB block describes the expected configuration of the CpuPcie controllers
**/
typedef struct {
  ///
  /// These members describe the configuration of each CPU PCIe root port.
  ///
  EFI_HOB_GUID_TYPE           EfiHobGuidType;                           ///< Offset 0 - 23: GUID Hob type structure for gCpuPcieHobGuid
  CPU_PCIE_ROOT_PORT_CONFIG   RootPort[CPU_PCIE_MAX_ROOT_PORTS];
  UINT8                       L1SubStates[CPU_PCIE_MAX_ROOT_PORTS];  ///< The L1 Substates configuration of the root port

  UINT32                      DekelFwVersionMinor;                      ///< Dekel Firmware Minor Version
  UINT32                      DekelFwVersionMajor;                      ///< Dekel Firmware Major Version
  BOOLEAN                     InitPcieAspmAfterOprom;                   ///< 1=initialize PCIe ASPM after Oprom; 0=before (This will be set basing on policy)
  UINT32                      RpEnabledMask;                            ///< Rootport enabled mask based on DEVEN register
  UINT32                      RpEnMaskFromDevEn;                        ///< Rootport enabled mask based on Device Id
  UINT8                       DisableClkReqMsg[CPU_PCIE_MAX_ROOT_PORTS];     ///< 1=ClkReqMsg disabled, 0=ClkReqMsg enabled
  UINT8                       SlotSelection;                            ///< 1=M2 slot, 0=CEMx4 slot
  BOOLEAN                     ComplianceTest;                           ///< Compliance Test based on policy
  UINT32                      HsPhyRecipeVersionMajor;                  ///< HS-Phy Recipe Major Version
  UINT32                      HsPhyRecipeVersionMinor;                  ///< HS-Phy Recipe Minor Version
  UINT32                      HsPhyFwProdMajor;                         ///< HS-Phy Firmware Product Major Verison
  UINT32                      HsPhyFwProdMinor;                         ///< HS-Phy Firmware Product Minor Verison
  UINT32                      HsPhyFwHotFix;                            ///< HS-Phy Firmware Hot Fix Version
  UINT32                      HsPhyFwBuild;                             ///< HS-Phy Firmware Build version
  UINT32                      HsPhyFwEvBitProgMajor;                    ///< HS-Phy Firmware EV Bit Prog Major
  UINT32                      HsPhyFwEvBitProgMinor;                    ///< HS-Phy Firmware EV Bit Prog Minor
} CPU_PCIE_HOB;
#pragma pack (pop)
#endif
