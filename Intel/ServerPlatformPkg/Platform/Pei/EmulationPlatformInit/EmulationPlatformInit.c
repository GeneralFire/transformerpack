/** @file
  EFI PEIM for Emulation Platform Initial

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2018 Intel Corporation. <BR>

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
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/SystemInfoLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/EmulationConfigurationLib.h>
#include <EmulationConfiguration.h>
#include <Guid/EmulationDfxVariable.h>
#include <Cpu/CpuCoreRegs.h>
#include <RcRegs.h>

EFI_PEI_PPI_DESCRIPTOR     mPpiListEmulation = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEmulationHobGuid,
  NULL
};

EFI_STATUS
EFIAPI
EmulationPlatformInitEntry (
  IN EFI_PEI_FILE_HANDLE             FileHandle,
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
{

  EFI_STATUS                           Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI      *PeiVariable;
  EMULATION_DFX_CONFIGURATION          EmulationVariable;
  UINTN                                BufferSize = sizeof(EMULATION_DFX_CONFIGURATION);
  EMULATION_SETTING                    *EmulationSetting;
  UINT32                               RegEax;
  UINT16                               CpuFamily;
  UINTN                                PciLibAddress;
  UINT32                               EmulationType;

  //
  // Build the Emulation Hob
  //
  EmulationSetting = BuildGuidHob (&gEmulationHobGuid, sizeof (EMULATION_SETTING));
  if (EmulationSetting == NULL) {
    DEBUG((EFI_D_ERROR, "Emulation BuildGuidDataHob fail!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  EmulationSetting->UbiosGenerationSetting = EMULATION_DISABLE;
  EmulationSetting->HybridSystemLevelEmulationSetting = EMULATION_DISABLE;
  EmulationSetting->UbiosOutputMode = ASM_OUTPUT_ENABLE;
  EmulationSetting->LoopBackLabelNumber = 0;
  EmulationSetting->FnvAccessValue = FNV_ACCESS_DISABLE;
  EmulationSetting->MsrTraceEnable = MSR_OUTPUT_DISABLE;

  //
  // Store variable into the hob.
  //
  (*PeiServices)->LocatePpi (
                  PeiServices,
                  &gEfiPeiReadOnlyVariable2PpiGuid,
                  0,
                  NULL,
                  &PeiVariable
                  );

  Status = PeiVariable->GetVariable (PeiVariable, EMULATION_DFX_CONFIGURATION_NAME, &gEmulationDfxVariableGuid, NULL, &BufferSize, &EmulationVariable);
  DEBUG ((EFI_D_INFO, "Emulation GetVariable status = %r !\n", Status));
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  EmulationSetting->UbiosGenerationSetting = EmulationVariable.DfxUbiosGeneration;
  EmulationSetting->HybridSystemLevelEmulationSetting = EmulationVariable.DfxHybridSystemLevelEmulation;
  EmulationSetting->MsrTraceEnable = EmulationVariable.DfxPmMsrTrace;

  //
  // Check the override value
  //
  AsmCpuid (1, &RegEax, NULL, NULL, NULL);
  CpuFamily = (UINT16) (RegEax >> 4);

  if (CpuFamily == CPU_FAMILY_SKX) {
    //
    // Simics flag is at B0:D0:F0 offset 0xFC for SKX.
    //
    PciLibAddress = PCI_LIB_ADDRESS(0, 0, 0, CSR_EMULATION_FLAG_OFFSET);
  } else if (CpuFamily == CPU_FAMILY_SPRSP)  {
    //
    // Simics flag is at UBOX B30:D0:F1 offset 0xF00 for SPR.
    //
    PciLibAddress = PCI_LIB_ADDRESS(30, 0, 1, CSR_EMULATION_FLAG_OFFSET);
  } else {
    //
    // Simics flag is at B0:D3:F0 offset 0xFC for 10nm.
    //
    PciLibAddress = PCI_LIB_ADDRESS(0, 3, 0, CSR_EMULATION_FLAG_OFFSET);
  }

  EmulationType = PciRead32 (PciLibAddress);

  if (EmulationType != 0xFFFFFFFF) {
    if ((EmulationType & UBIOS_GENERATION_EN) != 0) {
      EmulationSetting->UbiosGenerationSetting = EMULATION_ENABLE;
      DEBUG ((EFI_D_INFO, "EmulationVariable.DfxUbiosGeneration = %d\n", EmulationSetting->UbiosGenerationSetting));
    }

    if ((EmulationType & HYBRID_SYSTEM_LEVEL_EMULATION_EN) != 0) {
      EmulationSetting->HybridSystemLevelEmulationSetting = EMULATION_ENABLE;
      DEBUG ((EFI_D_INFO, "EmulationSetting->HybridSystemLevelEmulationSetting = %d\n", EmulationSetting->HybridSystemLevelEmulationSetting));
    }
  }
  //
  // Install gEmulationHobGuid PPI to inform the Emulation Hob is ready.
  //
  Status = PeiServicesInstallPpi (&mPpiListEmulation);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
