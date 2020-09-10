/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2020 Intel Corporation. <BR>

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

#include "FspInit.h"
#include <Ppi/ReadOnlyVariable2.h>

#define FIA_MUX_PCIE0_CLUSTER_DISABLE_MASK 0xFFFFFFFFFFFFAA00
#define FIA_MUX_PCIE1_CLUSTER_DISABLE_MASK 0xFFFFFFFFAAAAFFFF

EFI_STATUS
VerifyFspSiliconInitUpdDataPointer (
  IN EFI_PEI_SERVICES  **PeiServices,
  IN FSPS_UPD          *SiliconInitUpdPtr
  )
/*++

Routine Description:

  Validate FSP Silicon Init UPD Data Pointer.

Arguments:

  PeiServices:         PEI Services Table.
  MemoryInitUpdPtr: Pointer to structure to be validated.

Returns:

  !EFI_SUCCESS      - if any failure
  EFI_SUCCESS       - Successful

--*/
{
  EFI_STATUS   Status;
  Status = EFI_SUCCESS;

  if (SiliconInitUpdPtr == NULL) {
    Status = EFI_COMPROMISED_DATA;
  } else if (SiliconInitUpdPtr->FspUpdHeader.Signature != FSPS_UPD_SIGNATURE) {
    Status = EFI_COMPROMISED_DATA;
  }

  return Status;
};

EFI_STATUS
GetValidFspSiliconInitUpdDataPointer (
  IN     EFI_PEI_SERVICES  **PeiServices,
  IN OUT FSPS_UPD          **SiliconInitUpdPtr
  )
/*++

Routine Description:

  Get Valid FSP Silicon Init UPD Data Pointer.

Arguments:

  PeiServices:         PEI Services Table.
  MemoryInitUpdPtr: Pointer to structure to be updated.

Returns:

  !EFI_SUCCESS      - if any failure
  EFI_SUCCESS       - Successful

--*/
{
  EFI_STATUS   Status;
  Status = EFI_SUCCESS;

  //
  // Get and verify the UPD pointer.
  //
  *SiliconInitUpdPtr = (FSPS_UPD *) GetFspApiParameter();
  Status = VerifyFspSiliconInitUpdDataPointer(PeiServices,*SiliconInitUpdPtr);

  // In case of invalid UPD data pointer raise an error and halt system.
  if(Status != EFI_SUCCESS){
    DEBUG ((EFI_D_ERROR, "Invalid Fsp Silicon Init UPD data pointer! System halt!\n"));
    ASSERT_EFI_ERROR(Status);
  }
  SetFspSiliconInitUpdDataPointer(SiliconInitUpdPtr);

  return Status;
};

/**

  Do platform initialization for Silicon

  @param  FfsHeader    Not used.
  @param  PeiServices  General purpose services available to every PEIM.

  @return !EFI_SUCCESS      - if any failure
          EFI_SUCCESS       - Successful

**/
EFI_STATUS
PeimPlatformInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                       Status;
  FSPS_UPD                         *SiliconInitUpdPtr = NULL;
  VOID                             *HobPtr;
  FSP_GLOBAL_DATA                  *FspData;
  UINT32                           *PerfDataPtr;

  Status = EFI_SUCCESS;
  if (GetFspGlobalDataPointer()->FspMode == FSP_IN_API_MODE) {
    //
    // Get and validate the Silicon Init UPD pointer.
    //
    Status = GetValidFspSiliconInitUpdDataPointer((EFI_PEI_SERVICES**)PeiServices,&SiliconInitUpdPtr);
    if (SiliconInitUpdPtr == NULL) {
      ASSERT(SiliconInitUpdPtr != NULL);
      return EFI_NOT_FOUND;
    }

    if (Status != EFI_SUCCESS) {
      ASSERT_EFI_ERROR(Status);
    }

    //
    // Get PerfData
    //
    FspData = GetFspGlobalDataPointer();
    PerfDataPtr = (UINT32 *)FspData;
    PerfDataPtr = PerfDataPtr + ((sizeof (FSP_GLOBAL_DATA) - \
                                 sizeof (FspData->PerfData) - \
                                 sizeof (FspData->PerfIdx) - \
                                 sizeof (FspData->Reserved4) - \
                                 sizeof (FspData->PerfLen) - \
                                 sizeof (FspData->PerfSig)) / (sizeof (UINT32)));

    DEBUG((EFI_D_INFO, "FspData is 0x%08X \nPerfDataPtr is 0x%08X\n", FspData, PerfDataPtr));

    HobPtr = BuildGuidDataHob(&gPerformanceDataHobGuid, &PerfDataPtr, sizeof (UINT32 *));
    if (HobPtr != NULL) {
      DEBUG((EFI_D_INFO, "Build PERF DATA PTR HOB at 0x%08X(0x%x bytes) PerfDataPtr is 0x%08X\n", (UINT32)HobPtr, sizeof(UINT32 *), PerfDataPtr));
    } else {
      Status = EFI_NOT_FOUND;
      DEBUG((EFI_D_ERROR, "Build PERF DATA PTR HOB Failed"));
    }

    //
    // Update installed Policy with Silicon Init UPDs
    //
    DEBUG((EFI_D_INFO, "Updating Policies with Silicon Init UPD PCDs...\n"));

    //
    // Initialize PCH Policy for # of cores to PCD value (0:ALL, 1:1 Core, 2:2 Cores, 4:4 Cores, 8:8 Cores)
    //
    //PchPolicy->PmConfig.ActiveCoreCount = SiliconInitUpdPtr->FspsConfig.PcdActiveCoreCount;

    //
    // Initialize PCIE Policy for PCIe Bifurcation PCDs.
    //
    //PcieIpGlobalPolicy->PcieIpClusterPolicy[0].Bifurcation = SiliconInitUpdPtr->PcdBifurcationPcie0;
    //PcieIpGlobalPolicy->PcieIpClusterPolicy[1].Bifurcation = SiliconInitUpdPtr->PcdBifurcationPcie1;

    //
    // Set Policy for PCIe Static Power Gating, including FIA MUX overrides
    //
    if (SiliconInitUpdPtr->FspsConfig.EnablePcie0 == 0) {
      //FiaMuxGlobalPolicy->FiaMuxConfig.FiaMuxConfig.MeFiaMuxLaneConfig = FiaMuxGlobalPolicy->FiaMuxConfig.FiaMuxConfig.MeFiaMuxLaneConfig & FIA_MUX_PCIE0_CLUSTER_DISABLE_MASK;
    }
    if (SiliconInitUpdPtr->FspsConfig.EnablePcie1 == 0) {
      //FiaMuxGlobalPolicy->FiaMuxConfig.FiaMuxConfig.MeFiaMuxLaneConfig = FiaMuxGlobalPolicy->FiaMuxConfig.FiaMuxConfig.MeFiaMuxLaneConfig & FIA_MUX_PCIE1_CLUSTER_DISABLE_MASK;
    }
  }
  return Status;
}
