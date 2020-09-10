/** @file
  Support functions for PPM library, PEI version

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


#include "Common/CpuPpmIncludes.h"
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/MpServices2.h>



UINT32 *mThreadSocketMapping;


/**
  Create thread->socket mapping which is needed in InitialzeConfigurePbf()

  Only PEI needs this. DXE can use GetProcessorLocation()
**/
EFI_STATUS
FillThreadSocketMapping (
  PPM_POLICY_CONFIGURATION *PpmPolicy
)
{
  EFI_STATUS                  Status;
  EDKII_PEI_MP_SERVICES2_PPI  *Mp;
  UINT32                      NumProcessor = 0;
  UINT32                      i;
  EFI_PROCESSOR_INFORMATION   ProcessorInfo;

  if (!PpmPolicy->Hwpm.ConfigurePbf) {
    return EFI_SUCCESS;
  }

  Status = PeiServicesLocatePpi (&gEdkiiPeiMpServices2PpiGuid, 0, NULL, &Mp);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }

  Status = Mp->GetNumberOfProcessors (Mp, &NumProcessor, NULL);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  mThreadSocketMapping = AllocateZeroPool (NumProcessor * sizeof (UINT32));
  if (mThreadSocketMapping == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  for (i = 0; i < NumProcessor; i++) {
    Status = Mp->GetProcessorInfo (Mp, i, &ProcessorInfo);
    if (EFI_ERROR (Status)) {
      ASSERT (FALSE);
      return EFI_UNSUPPORTED;
    }
    mThreadSocketMapping[i] = ProcessorInfo.Location.Package;
    DEBUG((EFI_D_ERROR, "Location - ProcNumber:%d - Pkg:%d - Core:%d - Thread:%d\n", 
      i, ProcessorInfo.Location.Package,  ProcessorInfo.Location.Core, ProcessorInfo.Location.Thread));
  }

  return EFI_SUCCESS;
}



UINT32
PpmGetSocketFromProcessNumber (
  UINTN ProcessorNumber
)
{
  return mThreadSocketMapping[ProcessorNumber];
}
