/** @file
# Instance of MmPciExtLib to return PCIE physical address.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/RasDebugLib.h>
#include <Library/PciLib.h>
#include <Library/IoLib.h>
#include <Library/UsraAccessApi.h>
#include <Library/SystemInfoLib.h>
#include <Library/IioTopologyLib.h>
#include <Library/MmPciExtLib.h>
#include <IndustryStandard/Pci.h>

/**
  This procedure will get PCIE address

  @param[in] Socket               Socket Number
  @param[in] Bus                  Pcie Bus Number
  @param[in] Device               Pcie Device Number
  @param[in] Function             Pcie Function Number
  @param[in] MmInfo               Memory Map information of target device

  @retval PCIE address
**/
UINTN
EFIAPI
MmPciAddressExt (
  IN UINT32                       Socket,
  IN UINT32                       Bus,
  IN UINT32                       Device,
  IN UINT32                       Function,
  IN UINT32                       Register,
  IN PCI_EXT_MM_INFO              *MmInfo
  )
{
  USRA_ADDRESS                Address;
//UINTN                       VmdCfgBar = 0;
  UINTN                       RcrbBase = 0;
  UINTN                       Base = 0;
  UINT64                      MemBar0 = 0;
  UINT32                      Data32;
  BOOLEAN                     CxlValid;

  if (MmInfo == NULL) {
    USRA_PCIE_SEG_ADDRESS (Address, UsraWidth8, GetPcieSegment ((UINT8) Socket), \
              Bus, Device, Function, Register);

    return GetRegisterAddress (&Address);
  }

  if (MmInfo->PciExtMmType == PCI_EXT_MM_TYPE_VMD) {
    //VmdCfgBar = GetVmdCfgBar (Socket, MmInfo->Stack);
    //return VmdCfgBar + PCI_DEVICE_MM_OFFSET (Bus, Device, Function, Register);
    RAS_DEBUG((LEVEL_BASIC_FLOW, "MmPciAddressExt - PCI_EXT_MM_TYPE_VMD is not supported yet!!!"));
    RAS_ASSERT (FALSE);
    return 0;
  } else if (MmInfo->PciExtMmType == PCI_EXT_MM_TYPE_RCRB) {
    if (!GetStackCxlInfo (Socket, (UINT8)MmInfo->Stack, &RcrbBase, &CxlValid)) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "MmPciAddressExt - Get Socket %x, stack %x RCRB Address fail!!!", Socket, MmInfo->Stack));
      RAS_ASSERT (FALSE);
      return 0;
    }
    if ((CxlValid == FALSE) && (MmInfo->PciExtMmSubType != PCI_EXT_MM_SUB_TYPE_RCRB_DMI)) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "MmPciAddressExt - Socket %x, stack %x CXL is invalid !!!", Socket, MmInfo->Stack));
      RAS_ASSERT (FALSE);
      return 0;
    }
    switch (MmInfo->PciExtMmSubType) {
      case PCI_EXT_MM_SUB_TYPE_RCRB_DMI:
        Base = RcrbBase;
        break;
      case PCI_EXT_MM_SUB_TYPE_RCRB_CXLDP:
        Base = RcrbBase;
        break;
      case PCI_EXT_MM_SUB_TYPE_RCRB_CXLUP:
        Base = RcrbBase + CXL_DP_UP_RCRB_MEM_SIZE;
        break;
      case PCI_EXT_MM_SUB_TYPE_RCRB_CXLIO_DP:
        //
        // Get High Address
        //
        Data32 = MmioRead32 (RcrbBase + PCI_BASE_ADDRESSREG_OFFSET + 4);
        MemBar0 = Data32;
        //
        // Get Low Address
        //
        Data32 = MmioRead32 (RcrbBase + PCI_BASE_ADDRESSREG_OFFSET);
        Data32 &= 0xFFFFFFF0;
        MemBar0 = LShiftU64 (MemBar0, 32) | Data32;
        Base = (UINTN) (MemBar0 + CXL_DP_UP_MEMBAR0_IO_OFFSET);
        break;
      case PCI_EXT_MM_SUB_TYPE_RCRB_CXLIO_UP:
        //
        // Get High Address
        //
        Data32 = MmioRead32 (RcrbBase + CXL_DP_UP_RCRB_MEM_SIZE + PCI_BASE_ADDRESSREG_OFFSET + 4);
        MemBar0 = Data32;
        //
        // Get Low Address
        //
        Data32 = MmioRead32 (RcrbBase + CXL_DP_UP_RCRB_MEM_SIZE + PCI_BASE_ADDRESSREG_OFFSET);
        Data32 &= 0xFFFFFFF0;
        MemBar0 = LShiftU64 (MemBar0, 32) | Data32;
        Base = (UINTN) (MemBar0 + CXL_DP_UP_MEMBAR0_IO_OFFSET);
        break;
      case PCI_EXT_MM_SUB_TYPE_RCRB_CXLCM_DP:
        //
        // Get High Address
        //
        Data32 = MmioRead32 (RcrbBase + PCI_BASE_ADDRESSREG_OFFSET + 4);
        MemBar0 = Data32;
        //
        // Get Low Address
        //
        Data32 = MmioRead32 (RcrbBase + PCI_BASE_ADDRESSREG_OFFSET);
        Data32 &= 0xFFFFFFF0;
        MemBar0 = LShiftU64 (MemBar0, 32) | Data32;
        Base = (UINTN) (MemBar0 + CXL_DP_UP_MEMBAR0_CM_OFFSET);
        break;
      case PCI_EXT_MM_SUB_TYPE_RCRB_CXLCM_UP:
        //
        // Get High Address
        //
        Data32 = MmioRead32 (RcrbBase + CXL_DP_UP_RCRB_MEM_SIZE + PCI_BASE_ADDRESSREG_OFFSET + 4);
        MemBar0 = Data32;
        //
        // Get Low Address
        //
        Data32 = MmioRead32 (RcrbBase + CXL_DP_UP_RCRB_MEM_SIZE + PCI_BASE_ADDRESSREG_OFFSET);
        Data32 &= 0xFFFFFFF0;
        MemBar0 = LShiftU64 (MemBar0, 32) | Data32;
        Base = (UINTN) (MemBar0 + CXL_DP_UP_MEMBAR0_CM_OFFSET);
        break;
      default:
        RAS_DEBUG((LEVEL_BASIC_FLOW, "MmPciAddressExt - Socket %x, stack %x PciExtMmSubType is Unknown !!!", Socket, MmInfo->Stack));
        RAS_ASSERT (FALSE);
        return 0;
        break;
    }
    return Base + Register;
  } else {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "MmPciAddressExt - Socket %x, stack %x PciExtMmType is Unknown !!!", Socket, MmInfo->Stack));
    RAS_ASSERT (FALSE);
    return 0;
  }
}

