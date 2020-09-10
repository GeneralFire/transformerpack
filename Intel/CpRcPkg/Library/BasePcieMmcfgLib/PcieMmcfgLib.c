/** @file
  PCIE MMCFG Lib to get MMCFG base address

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Uefi.h>
#include <Library/SystemInfoLib.h>
#include <Library/PcieMmcfgLib.h>

/**
  Get the PCIE MMCFG Base address for a given segement contained in the
  Address structure. Return value will be the default until the Mmcfg registers
  are programmed by KTI, then they will be the programmed values.

  @param[in] - Address: Ptr to the address to get the base address for.

  @return    - The Mmcfg base address.

**/

UINTN
GetPcieSegMmcfgBaseAddress (
  IN USRA_ADDRESS    *Address
  )
{
  UINT32                BaseAddressLow; // Processor-relative Base Address (Lower 32-bit) for the Enhanced Configuration Access Mechanism
  UINT32                PciExpressBaseAddress;
  CPU_CSR_ACCESS_VAR    *CpuCsrAccessVarPtr;

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();
  PciExpressBaseAddress = CpuCsrAccessVarPtr->mmCfgBase;
// APTIOV_SERVER_OVERRIDE_RC_START : Validate mmCfgBase
  //PCI Express base address should be 256MB aligned and
  // not equal to 0/0xFFFFFFFF
  if ((PciExpressBaseAddress != 0) &&
      (PciExpressBaseAddress != 0xFFFFFFFF) &&
      !(PciExpressBaseAddress & 0xFFFFFFF)) {
// APTIOV_SERVER_OVERRIDE_RC_END : Validate mmCfgBase
      BaseAddressLow = PciExpressBaseAddress + ((Address->Pcie.Seg) << 28);
  } else {
  //
  // Check if we got valid values, if not use PcdPciExpressBaseAddress for the default value.
  //
    BaseAddressLow = (UINT32) PcdGet64 (PcdPciExpressBaseAddress);
  }

  return BaseAddressLow;
} // GetPcieSegMmcfgBaseAddress

