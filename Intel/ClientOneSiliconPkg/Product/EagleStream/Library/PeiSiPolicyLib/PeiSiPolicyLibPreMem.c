/** @file
  This file is PeiSiPolicyLib library creates default settings of RC
  Policy and installs RC Policy PPI.

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
#include "PeiSiPolicyLibrary.h"
#include <Base.h>
#include <PciePreMemConfig.h>
#include <Library/PeiPcieRpPolicyLib.h>
#include <Library/PeiSpsPreMemPolicyLib.h>
#include <Library/PeiFiaPolicyLib.h>


/**
  Get Si PreMem config block table total size.

  @retval                               Size of SI PreMem config block table
**/
UINT16
SiGetConfigBlockTotalSizePreMem (
  VOID
  )
{
  return (UINT16) sizeof (SI_PREMEM_CONFIG);
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
EFI_STATUS
LoadSiPreMemConfigBlockDefault (
  IN VOID              *ConfigBlockPointer
  )
{
  SI_PREMEM_CONFIG     *SiPreMemConfig;

  SiPreMemConfig     = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "SiPreMemConfig->Header.GuidHob.Name = %g\n", &SiPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "SiPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", SiPreMemConfig->Header.GuidHob.Header.HobLength));

  return EFI_SUCCESS;
}

/**
  Add the Si PreMem config block.

  @param[in] ConfigBlockTableAddress    The pointer to add config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
SiAddConfigBlocksPreMem (
  IN     VOID          *ConfigBlockTableAddress
  )
{
  VOID                 *ConfigBlockPointer;
  EFI_STATUS           Status;
  CONFIG_BLOCK_HEADER  SiPreMemBlock;

  //
  // Initalize SiPreMemBlock
  //
  CopyMem (&(SiPreMemBlock.GuidHob.Name), &gSiPreMemConfigGuid, sizeof (EFI_GUID));
  SiPreMemBlock.GuidHob.Header.HobLength = sizeof (SI_PREMEM_CONFIG);
  SiPreMemBlock.Revision                 = SI_PREMEM_CONFIG_REVISION;
  //
  // Initialize ConfigBlockPointer
  //
  ConfigBlockPointer = (VOID *)&SiPreMemBlock;
  //
  // Add SiPreMemBlock
  //
  DEBUG ((DEBUG_INFO, "gSiPreMemConfigGuid = %g\n", &gSiPreMemConfigGuid));
  DEBUG ((DEBUG_INFO, "SiPreMemBlock->Header.GuidHob.Name = %g\n", &(SiPreMemBlock.GuidHob.Name)));
  Status = AddConfigBlock (ConfigBlockTableAddress, (VOID *) &ConfigBlockPointer);
  ASSERT_EFI_ERROR (Status);

  LoadSiPreMemConfigBlockDefault ((VOID *) ConfigBlockPointer);

  return Status;
}

/**
  SiCreatePreMemConfigBlocks creates the config blocks of Silicon PREMEM Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] SiPreMemPolicyPpi   The pointer to get Silicon Policy PreMem PPI instance

  @retval EFI_SUCCESS             The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiCreatePreMemConfigBlocks (
  OUT  SI_PREMEM_POLICY_PPI **SiPreMemPolicyPpi
  )
{
  UINT16               TotalBlockSize;
  EFI_STATUS           Status;
  SI_PREMEM_POLICY_PPI *SiPreMemPolicy;
  UINT16               RequiredSize;

  SiPreMemPolicy = NULL;
  //
  // TotalBlockSize = SI, Pch , SA, ME, CPU + individual IPs' config block size
  //
  TotalBlockSize = SiGetConfigBlockTotalSizePreMem () +
                   PcieGetConfigBlockTotalSizePreMem () +
                   PchGetPreMemConfigBlockTotalSize () +
                   MeGetConfigBlockTotalSizePreMem () +
                   SpsPreMemGetConfigBlockTotalSize ();
  // IPs'
  TotalBlockSize += DciPreMemGetConfigBlockTotalSize ();
  TotalBlockSize += CpuTraceHubPreMemGetConfigBlockTotalSize ();
  TotalBlockSize += PchTraceHubPreMemGetConfigBlockTotalSize ();
  TotalBlockSize += SmbusPreMemGetConfigBlockTotalSize ();
  TotalBlockSize += WdtPreMemGetConfigBlockTotalSize ();
  TotalBlockSize += HdaPreMemGetConfigBlockTotalSize ();
  TotalBlockSize += FiaMuxPreMemGetConfigBlockTotalSize ();

  DEBUG ((DEBUG_INFO, "TotalBlockSize = 0x%x\n", TotalBlockSize));

  RequiredSize = sizeof (CONFIG_BLOCK_TABLE_HEADER) + TotalBlockSize;

  Status = CreateConfigBlockTable (RequiredSize, (VOID *)&SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);

  //
  // General initialization
  //
  SiPreMemPolicy->TableHeader.Header.Revision = SI_PREMEM_POLICY_REVISION;
  //
  // Add config blocks.
  //
  Status = SiAddConfigBlocksPreMem ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = PcieAddConfigBlocksPreMem ((VOID *)SiPreMemPolicy);
  ASSERT_EFI_ERROR(Status);
  Status = PchAddPreMemConfigBlocks ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = MeAddConfigBlocksPreMem ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  Status = SpsPreMemAddConfigBlocks ((VOID*)SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  // DCI
  Status = DciPreMemAddConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  // PCH Trace Hub
  Status = PchTraceHubPreMemAddConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  // CPU Trace Hub
  Status = CpuTraceHubPreMemAddConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  // SMBUS
  Status = SmbusPreMemAddConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  // WDT
  Status = WdtPreMemAddConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  // HDA
  Status = HdaPreMemAddConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  // FIA MUX
  Status = FiaMuxPreMemAddConfigBlock ((VOID *) SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);

  //
  // Assignment for returning SaInitPolicy config block base address
  //
  *SiPreMemPolicyPpi = SiPreMemPolicy;
  return Status;
}

/**
  Print SI PreMem Config block

  @param[in] SiPreMemPolicyPpi          The RC PREMEM Policy PPI instance
**/
VOID
SiPreMemPrintPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI          *SiPreMemPolicyPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                        Status;
  SI_PREMEM_CONFIG                  *SiPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSiPreMemConfigGuid, (VOID *) &SiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "---------------------- Si Pre-Mem Config Block Print Begin -----------------\n"));
  DEBUG ((DEBUG_INFO, "PlatformDebugConsent = %x\n", SiPreMemConfig->PlatformDebugConsent));
  DEBUG ((DEBUG_INFO, "---------------------- Si Pre-Mem Config Block Print End -------------------\n"));
  DEBUG_CODE_END ();
}

/**
  SiPreMemInstallPolicyPpi installs SiPreMemPolicyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] SiPreMemPolicyPpi   The pointer to Silicon Policy PPI instance

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiPreMemInstallPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI *SiPolicyPreMemPpi
  )
{
  EFI_STATUS             Status;
  EFI_PEI_PPI_DESCRIPTOR *SiPolicyPreMemPpiDesc;

  SiPolicyPreMemPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (SiPolicyPreMemPpiDesc == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  SiPolicyPreMemPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  SiPolicyPreMemPpiDesc->Guid  = &gSiPreMemPolicyPpiGuid;
  SiPolicyPreMemPpiDesc->Ppi   = SiPolicyPreMemPpi;

  //
  // Install Silicon Policy PPI
  //
  Status = PeiServicesInstallPpi (SiPolicyPreMemPpiDesc);
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  SiPreMemInstallPolicyReadyPpi installs SiPreMemPolicyReadyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiPreMemInstallPolicyReadyPpi (
  VOID
  )
{
  EFI_STATUS             Status;
  SI_PREMEM_POLICY_PPI   *SiPolicyPreMemPpi;
  EFI_PEI_PPI_DESCRIPTOR *SiPolicyPreMemPpiDesc;

  SiPolicyPreMemPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (SiPolicyPreMemPpiDesc == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  SiPolicyPreMemPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  SiPolicyPreMemPpiDesc->Guid  = &gSiPreMemPolicyReadyPpiGuid;
  SiPolicyPreMemPpiDesc->Ppi   = NULL;

  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPolicyPreMemPpi);
  if (Status == EFI_SUCCESS) {
    //
    // Print SI PreMem config block
    //
    SiPreMemPrintPolicyPpi (SiPolicyPreMemPpi);
    //
    // Print PCIe PreMem config block
    //
    PciePreMemPrintPolicyPpi (SiPolicyPreMemPpi);
    //
    // Print PCH PreMem config blocks
    //
    PchPreMemPrintPolicyPpi (SiPolicyPreMemPpi);
    //
    // Print ME PreMem config blocks
    //
    MePrintPolicyPpiPreMem (SiPolicyPreMemPpi);
    SpsPreMemPrintPolicyPpi (SiPolicyPreMemPpi);

    DciPreMemPrintConfig (SiPolicyPreMemPpi);
    CpuTraceHubPreMemPrintConfig (SiPolicyPreMemPpi);
    PchTraceHubPreMemPrintConfig (SiPolicyPreMemPpi);
    SmbusPreMemPrintConfig (SiPolicyPreMemPpi);
    WdtPreMemPrintConfig (SiPolicyPreMemPpi);
    HdaPreMemPrintConfig (SiPolicyPreMemPpi);
    FiaMuxPreMemPrintConfig (SiPolicyPreMemPpi);

  }
  //
  // Install PreMem Silicon Policy PPI
  //
  Status = PeiServicesInstallPpi (SiPolicyPreMemPpiDesc);
  ASSERT_EFI_ERROR (Status);
  return Status;
}

