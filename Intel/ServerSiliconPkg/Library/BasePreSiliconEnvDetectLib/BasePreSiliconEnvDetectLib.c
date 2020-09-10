/** @file
  This file is BasePreSiliconEnvDetectLib library is used to detect Simics environment.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation.

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
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Base.h>
#include <Platform.h>
#include <Library/PciLib.h>
#include <Library/BaseMemoryLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Include/SioRegs.h>
#include <RcRegs.h>
#include <Library/CpuAccessLib.h>
#include <Library/SystemInfoLib.h>


/**
  Check if currently run in Simics environment or not.

  @retval TRUE                  Run in Simics environment
  @retval FALSE                 Not run in Simics environment
**/
BOOLEAN
IsSimicsEnvironment (
  VOID
  )
{
  UINT32  RegEax;
  UINT16  CpuFamily;
  UINTN   PciLibAddress;
  UINT8   EmulationType;
  UINT64_STRUCT  MsrData64;
  UINT8   BusNo;

  AsmCpuid (1, &RegEax, NULL, NULL, NULL);
  CpuFamily = (UINT16) (RegEax >> 4);

  if (CpuFamily == CPU_FAMILY_SKX) {
    //
    // Simics flag is at B0:D0:F0 offset 0xFC for SKX.
    //
    PciLibAddress = PCI_LIB_ADDRESS(0, 0, 0, CSR_EMULATION_FLAG_OFFSET);
  } else if (CpuFamily == CPU_FAMILY_SPRSP)  {
    //
    // Simics flag is at UBOX U0:D0:F1 offset 0xF00 for SPR.
    //
    MsrData64 = ReadMsr (MSR_CPU_BUSNUMBER);
    BusNo = MsrData64.lo & 0x000000FF;
    PciLibAddress = PCI_LIB_ADDRESS(BusNo, 0, 1, CSR_EMULATION_FLAG_OFFSET);
  } else {
    //
    // Simics flag is at B0:D3:F0 offset 0xFC for 10nm.
    //
    PciLibAddress = PCI_LIB_ADDRESS(0, 3, 0, CSR_EMULATION_FLAG_OFFSET);
  }

  EmulationType = PciRead8 (PciLibAddress);

  if (((EmulationType & SIMICS_FLAG) != 0) && (EmulationType != 0xFF)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Check if currently run in HFPGA environment or not.

  @retval TRUE                  Run in HFPGA environment
  @retval FALSE                 Not run in HFPGA environment
**/
BOOLEAN
IsHfpgaEnvironment (
  VOID
  )
{
  UINT32  RegEax;
  UINT16  CpuFamily;
  UINTN   PciLibAddress;
  UINT8   EmulationType;
  UINT32  EmulationType32;
  UINT64_STRUCT  MsrData64;
  UINT8   BusNo;

  AsmCpuid (1, &RegEax, NULL, NULL, NULL);
  CpuFamily = (UINT16) (RegEax >> 4);

  if (CpuFamily == CPU_FAMILY_SKX) {
    //
    // Simics flag is at B0:D0:F0 offset 0xFC for SKX.
    //
    PciLibAddress = PCI_LIB_ADDRESS(0, 0, 0, CSR_EMULATION_FLAG_OFFSET);
  } else if (CpuFamily == CPU_FAMILY_SPRSP)  {
    //
    // Simics flag is at UBOX U0:D0:F1 offset 0xF00 for SPR.
    //
    MsrData64 = ReadMsr (MSR_CPU_BUSNUMBER);
    BusNo = MsrData64.lo & 0x000000FF;
    PciLibAddress = PCI_LIB_ADDRESS(BusNo, 0, 1, CSR_EMULATION_FLAG_OFFSET);
  } else {
    //
    // Simics flag is at B0:D3:F0 offset 0xFC for 10nm.
    //
    PciLibAddress = PCI_LIB_ADDRESS(0, 3, 0, CSR_EMULATION_FLAG_OFFSET);
  }

  EmulationType = PciRead8 (PciLibAddress);
  EmulationType32 = PciRead32 (PciLibAddress);

  DEBUG ((DEBUG_INFO, "IsHfpgaEnvironment () EmulationType: 0x%02x, EmulationType32: 0x%08x\n", EmulationType, EmulationType32));

  if (EmulationType == (SIMICS_FLAG | SOFT_SDV_FLAG)) {
    return TRUE;
  } else {
    return FALSE;
  }
}
