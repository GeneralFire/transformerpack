/** @file
  This has platform-specific code for LT.  It contains hooks used by
  ServerCommon code that must be run during PEI for enabling/disabling/examining
  LT/TXT functions and status.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2018 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/Stall.h>
#include <Ppi/MpServices.h>
#include <Guid/SocketVariable.h>
#include <Guid/SocketProcessorCoreVariable.h>
#include <Guid/PlatformTxt.h>
#include <Universal/GetSec/Pei/TxtPeiLib.h>
#include <Library/SetupLib.h>

// External variables
//
extern EFI_GUID gEfiPeiReadOnlyVariable2PpiGuid;

// External PROCs
//


EFI_STATUS
GetPeiMpServices (
  IN      CONST EFI_PEI_SERVICES  **PeiServices,
  OUT     EFI_PEI_MP_SERVICES_PPI **PeiMpServices

  )
{
  //
  // Find PeiMpServices PPI, only need success locating at end of S3 resume
  //
  return ((*PeiServices)->LocatePpi(
                      PeiServices,
                      &gEfiPeiMpServicesPpiGuid,
                      0,
                      NULL,
                      PeiMpServices
                      ));
}
