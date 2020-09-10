/** @file
  Board Init Post Mem PEIM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018-2020 Intel Corporation. <BR>

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

#include <Library/DebugLib.h> // for DEBUG()
#include <Library/PeiServicesLib.h> // for PeiServicesInstallPpi()


EFI_PEI_PPI_DESCRIPTOR     mPpiListBoardInit = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gBoardInitPostMemGuid,
  NULL
};

/**
  Entry point function for the PEIM

  @param FileHandle      Handle of the file being invoked.
  @param PeiServices     Describes the list of possible PEI Services.

  @return EFI_SUCCESS    If we installed our PPI

**/
EFI_STATUS
EFIAPI
BoardInitPostMemPeimEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
)
{
  EFI_STATUS                    Status;
  DEBUG((DEBUG_INFO,"UBA PEI Post Memory: %a\n",__FUNCTION__));

  //
  // Inform board init ready, then peims which will use board init data such as
  // pcd data in uba database can take gBoardInitGuid as dependency.
  //
  Status = PeiServicesInstallPpi (&mPpiListBoardInit);
  if (Status != EFI_SUCCESS) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return EFI_SUCCESS;
}
