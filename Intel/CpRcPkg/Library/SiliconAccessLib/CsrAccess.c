/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2019 Intel Corporation. <BR>

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

#include "SiliconAccessLibInternal.h"
#include <Library/DebugLib.h>


/**
  This API gets the CSR address from the given USRA Address.

  @param[in] Address              A pointer of the address of the USRA Address Structure
  @param[out] AlignedAddress      A pointer of aligned address converted from USRA address

**/
VOID
EFIAPI
GetCsrAccessAddress (
  IN USRA_ADDRESS             *Address,
  OUT UINTN                   *AlignedAddress
  )
{
  if (Address->Attribute.AddrType == AddrTypeCSR) {
    CsrGetPhysicalAddress (Address, AlignedAddress); // Silicon translation
  } else if (Address->Attribute.AddrType == AddrTypeCSRMEM) {
    GetCsrMemAccessAddress (Address, AlignedAddress); // Independent math
  } else if (Address->Attribute.AddrType == AddrTypeCSRCFG) {
    GetCsrCfgAccessAddress (Address, AlignedAddress); // Independent math
  }
}

/**
  This API gets the CSR Mem address from the given USRA Address.

  @param[in] Address              A pointer of the address of the USRA Address Structure
  @param[out] AlignedAddress      A pointer of aligned address converted from USRA address

**/
VOID
EFIAPI
GetCsrMemAccessAddress (
  IN USRA_ADDRESS             *Address,
  OUT UINTN                   *AlignedAddress
  )
{
  CPU_CSR_ACCESS_VAR          *CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar();

  ASSERT (CpuCsrAccessVarPtr->DataPopulated == TRUE);

  *AlignedAddress = CpuCsrAccessVarPtr->MmioBar[Address->CsrMem.SocketId][Address->CsrMem.MemBarId] + Address->CsrMem.Offset;

  if (Address->CsrMem.High64Split == 1) {
    *AlignedAddress = *AlignedAddress + 4; // Read higher part of register
  }
}

/**
  This API gets the CSR Cfg address from the given USRA Address.

  @param[in] Address              A pointer of the address of the USRA Address Structure
  @param[out] AlignedAddress      A pointer of aligned address converted from USRA address

**/
VOID
EFIAPI
GetCsrCfgAccessAddress (
  IN USRA_ADDRESS             *Address,
  OUT UINTN                   *AlignedAddress
  )
{
  CPU_CSR_ACCESS_VAR          *CpuCsrAccessVarPtr;
  UINT8                       Bus;

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar();

  ASSERT (CpuCsrAccessVarPtr->DataPopulated == TRUE);

  Bus = (UINT8)Address->CsrCfg.Bus;
  if (Bus == 30) {
    Bus = CpuCsrAccessVarPtr->Socket10nmUboxBus0[Address->CsrCfg.SocketId];
  } else if (Bus == 31) {
    Bus = CpuCsrAccessVarPtr->Socket10nmUboxBus0[Address->CsrCfg.SocketId] + 1;
  } else if (Bus < MAX_LOGIC_IIO_STACK) {
    Bus = CpuCsrAccessVarPtr->StackBus[Address->CsrCfg.SocketId][Bus];
  } else {
    Bus = Bus + CpuCsrAccessVarPtr->SocketFirstBus[Address->CsrCfg.SocketId]; // Add socket bus offset
  }

  *AlignedAddress = CpuCsrAccessVarPtr->mmCfgBaseL[Address->CsrCfg.SocketId] |
                      (Bus << 20) |
                      (Address->CsrCfg.Device << 15) |
                      (Address->CsrCfg.Function << 12) |
                      Address->CsrCfg.Offset;

  if (Address->CsrCfg.High64Split == 1) {
    *AlignedAddress = *AlignedAddress + 4; // Read higher part of register
  }
}
