/** @file
  Implementation of MP related function for processor RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#include "ProcessorRasLibInternal.h"


/*
  Get core and thread shift bits.

  @param[out] ThreadBits   Output for thread shift bits
  @param[out] CoreBits     Output for core shift bits

*/
VOID
EFIAPI
GetCoreAndSmtBits (
  OUT UINTN *ThreadBits,
  OUT UINTN *CoreBits
  )
{
  UINT32  RegEax;
  UINT32  RegEbx;
  UINT32  RegEcx;
  UINT32  RegEdx;
  UINT32  MaxCpuIdIndex;
  UINT32  SubIndex;
  UINTN   LevelType;
  UINT32  MaxLogicProcessorsPerPackage;
  UINT32  MaxCoresPerPackage;

  if (CoreBits == NULL) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: CoreBits is NULL\n"));
      RAS_ASSERT (FALSE);
      return;
  }
  if (ThreadBits == NULL) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: ThreadBits is NULL\n"));
      RAS_ASSERT (FALSE);
      return;
  }
 


  //
  // Check if the processor is capable of supporting more than one logical processor.
  //
  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, NULL, &RegEdx);
  RAS_ASSERT ((RegEdx & BIT28) != 0);

  //
  // Assume three-level mapping of APIC ID: Package:Core:SMT.
  //

  //
  // Get the max index of basic CPUID
  //
  AsmCpuid (CPUID_SIGNATURE, &MaxCpuIdIndex, NULL, NULL, NULL);

  if (MaxCpuIdIndex >= CPUID_EXTENDED_TOPOLOGY) {
    AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, 0, &RegEax, &RegEbx, NULL, NULL);
    if ((RegEbx & 0xffff) != 0) {
      //
      // x2APIC ID
      //

      *ThreadBits = RegEax & 0x1f;

      *CoreBits = 0;
      SubIndex = 1;
      do {
        AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, SubIndex, &RegEax, NULL, &RegEcx, NULL);
        LevelType = (RegEcx >> 8) & 0xff;
        if (LevelType == CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_CORE) {
          *CoreBits = (RegEax & 0x1f) - *ThreadBits;
          break;
        }
        SubIndex++;
      } while (LevelType != CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_INVALID);
    }
  } else {

    //
    // xAPIC ID
    //

    AsmCpuid (CPUID_VERSION_INFO, NULL, &RegEbx, NULL, NULL);
    MaxLogicProcessorsPerPackage = (RegEbx >> 16) & 0xff;
    if (MaxCpuIdIndex >= CPUID_CACHE_PARAMS) {
      AsmCpuidEx (CPUID_CACHE_PARAMS, 0, &RegEax, NULL, NULL, NULL);
      MaxCoresPerPackage = (RegEax >> 26) + 1;
    } else {
      MaxCoresPerPackage = 1;
    }

    *ThreadBits = (UINTN) (HighBitSet32 (MaxLogicProcessorsPerPackage / MaxCoresPerPackage - 1) + 1);
    *CoreBits = (UINTN) (HighBitSet32 (MaxCoresPerPackage - 1) + 1);
  }

  RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "GetCoreAndSmtBits: CoreBits %x, ThreadBits %x\n", (UINT32) *CoreBits, (UINT32) *ThreadBits));
}

/**
  Enable the feature to clear pending SMI on SIPI on the executing AP.

  An AP responds with a spurious SMI when it is woken from Wait-For-SIPI (WFS)
  by a Startup-IPI (SIPI) if an SMI has occurred while it was in WFS. This
  feature clears any pending SMI for an AP when a SIPI is issued to it.

  @param[in, out] Buffer  The pointer to private data buffer.
**/
VOID
EFIAPI
ApEnableClearSmiOnSipi (
  IN OUT  VOID  *Buffer
  )
{
  MSR_SMM_CFG_OPTIONS_REGISTER  MsrSmmCfgOptions;

  MsrSmmCfgOptions.Uint64 = AsmReadMsr64 (MSR_SMM_CFG_OPTIONS);
  MsrSmmCfgOptions.Bits.CfgOption = 1;
  AsmWriteMsr64 (MSR_SMM_CFG_OPTIONS, MsrSmmCfgOptions.Uint64);

  RAS_DEBUG ((
    MP_PRINT | LEVEL_REG,
    "[CpuRas] MSR(%x) = %lx\n",
    MSR_SMM_CFG_OPTIONS,
    MsrSmmCfgOptions.Uint64
    ));
}

/**
  Enable the feature to clear pending SMI on SIPI on all APs.

  An AP responds with a spurious SMI when it is woken from Wait-For-SIPI (WFS)
  by a Startup-IPI (SIPI) if an SMI has occurred while it was in WFS. This
  feature clears any pending SMI for an AP when a SIPI is issued to it.
**/
VOID
EFIAPI
EnableClearPendingSmiOnSipi (
  VOID
  )
{
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "[CpuRas]EnableClearSmiOnSipi\n"));

  MultiProcessorExecute (
    AllAps,
    ALL_PACKAGE,
    ALL_CORE,
    FALSE,
    ApEnableClearSmiOnSipi,
    NULL
    );
}
