/** @file
  This file is PeiCpuLatePolicyLib library implementation creates default settings
  of CPU Late Policy and installs CPU Late Policy PPI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiCpuLatePolicyLib.h>
#include <Ppi/CpuLatePolicyPpi.h>

/**
  CpuCreateLatePolicyDefaults creates default settings of CPU Late Policy.

  @param[in, out] CpuLatePolicyPpi  The pointer to CPU Late Policy PPI instance.

  @retval EFI_SUCCESS               The policy defaults are created.

**/
EFI_STATUS
EFIAPI
CpuCreateLatePolicyDefaults (
  IN OUT CPU_LATE_POLICY_PPI    *CpuLatePolicyPpi
  )
{
  CpuLatePolicyPpi->Revision = CPU_LATE_POLICY_PPI_REVISION;
  CpuLatePolicyPpi->Policy.CpuTStateEnable = FALSE;
  CpuLatePolicyPpi->Policy.CpuClockModulationDutyCycle = 0;
  CpuLatePolicyPpi->Policy.CpuAesEnable = TRUE;
  CpuLatePolicyPpi->Policy.CpuFastStringEnable = TRUE;
  CpuLatePolicyPpi->Policy.CpuMaxCpuidValueLimitEnable = FALSE;
  CpuLatePolicyPpi->Policy.CpuMachineCheckEnable = TRUE;
  CpuLatePolicyPpi->Policy.CpuMonitorMwaitEnable = TRUE;
  CpuLatePolicyPpi->Policy.CpuVtEnable = TRUE;
  CpuLatePolicyPpi->Policy.CpuLtEnable = FALSE;
  CpuLatePolicyPpi->Policy.CpuX2ApicEnable = FALSE;
  CpuLatePolicyPpi->Policy.CpuEistEnable = TRUE;
  CpuLatePolicyPpi->Policy.CpuTurboModeEnable = TRUE;
  CpuLatePolicyPpi->Policy.CpuHwCoordinationEnable = TRUE;
  CpuLatePolicyPpi->Policy.CpuBootPState = 0;
  CpuLatePolicyPpi->Policy.CpuPpinControlEnable = TRUE;
  CpuLatePolicyPpi->Policy.CpuPeciDownstreamWriteEnable = FALSE;
  CpuLatePolicyPpi->Policy.CpuL1NextPagePrefetcherDisable = FALSE;
  CpuLatePolicyPpi->Policy.CpuDcuPrefetcherEnable = TRUE;
  CpuLatePolicyPpi->Policy.CpuIpPrefetcherEnable = TRUE;
  CpuLatePolicyPpi->Policy.CpuMlcStreamerPrefetecherEnable = TRUE;
  CpuLatePolicyPpi->Policy.CpuMlcSpatialPrefetcherEnable = TRUE;
  CpuLatePolicyPpi->Policy.CpuAmpPrefetchEnable = FALSE;
  CpuLatePolicyPpi->Policy.CpuThreeStrikeCounterEnable = TRUE;
  CpuLatePolicyPpi->Policy.CpuCStateEnable = TRUE;
  CpuLatePolicyPpi->Policy.CpuPackageCStateLimit = 0;
  CpuLatePolicyPpi->Policy.CpuC1AutoDemotionEnable = FALSE;
  CpuLatePolicyPpi->Policy.CpuC1AutoUndemotionEnable = FALSE;
  CpuLatePolicyPpi->Policy.CpuCoreCStateValue = 0;
  CpuLatePolicyPpi->Policy.CpuAcpiLvl2Addr = 0;
  CpuLatePolicyPpi->Policy.CpuThermalManagementEnable = TRUE;
  CpuLatePolicyPpi->Policy.CpuTccActivationOffset = 0;
  CpuLatePolicyPpi->Policy.CpuDbpfEnable = FALSE;
  CpuLatePolicyPpi->Policy.CpuEnergyPerformanceBiasEnable = TRUE;
  CpuLatePolicyPpi->Policy.CpuIioLlcWaysBitMask = 0;
  CpuLatePolicyPpi->Policy.CpuExpandedIioLlcWaysBitMask = 0;
  CpuLatePolicyPpi->Policy.CpuRemoteWaysBitMask = 0;
  CpuLatePolicyPpi->Policy.CpuRrqCountThreshold = 0;
  CpuLatePolicyPpi->Policy.CpuMtoIWa = 0;
  CpuLatePolicyPpi->Policy.RunCpuPpmInPei = FALSE;
  CpuLatePolicyPpi->Policy.AcExceptionOnSplitLockEnable = FALSE;

  return EFI_SUCCESS;
}

/**
  CpuInstallLatePolicyPpi installs CPU Late Policy PPI.
  While installed, RC assumes the Policy is ready and finalized.
  So please update and override any setting before calling this function.

  @param[in] CpuLatePolicyPpi       The pointer to CPU Late Policy PPI instance.

  @retval EFI_SUCCESS               The policy PPI is installed.
  @retval EFI_OUT_OF_RESOURCES      Insufficient resources to create buffer.

**/
EFI_STATUS
EFIAPI
CpuInstallLatePolicyPpi (
  IN CPU_LATE_POLICY_PPI    *CpuLatePolicyPpi
  )
{
  EFI_STATUS                Status;
  EFI_PEI_PPI_DESCRIPTOR    *PpiDesc;

  PpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (PpiDesc == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  PpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  PpiDesc->Guid  = &gPeiCpuLatePolicyPpiGuid;
  PpiDesc->Ppi   = CpuLatePolicyPpi;

  DEBUG ((DEBUG_INFO, "Installing CpuLatePolicyPpi\n"));
  Status = PeiServicesInstallPpi (PpiDesc);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to install CpuLatePolicyPpi, Error = %r\n", Status));
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  return Status;
}
