/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include "UncoreMiscDxe.h"
#include <Protocol/PlatformType.h>
#include <Protocol/Decompress.h>
#include <Library/CompressLib.h>
#include <Protocol/VariableLock.h>
#include <Guid/CpuNvramData.h>
#include <Include/SystemInfoVar.h>
#include <Library/SystemInfoLib.h>

/**

  This function will save the KTI Output Adaptation table to Variable.

  @param VOID

  @retval VOID

**/
VOID
SaveAdaptationTableToNvram (
  VOID
  )
{
  EFI_STATUS            Status;
  KTI_ADAPTATION_TABLE  AdaptationTable[KTI_SPEEDS_SUPPORTED];

  Status = GetUpiTxeqAdaptationTable (AdaptationTable);
  if (!EFI_ERROR(Status)) {
    DEBUG((DEBUG_INFO, "\n Kti Adaptation Table is found!\n"));

    Status = gRT->SetVariable (
                    KTI_ADAPTATION_TABLE_DATA_NAME,
                    &gKtiAdaptationTableGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    sizeof (AdaptationTable),
                    (VOID *)&AdaptationTable
                    );
    if (EFI_ERROR(Status)) {
      DEBUG((EFI_D_ERROR, "\n Status of saving Adaptation table to Variable: %r \n", Status));
    }
  }

  return;
}

/**

  This is the main entry point of the Uncore Misc Dxe module.
  Note: This driver is only used to save the adaptation table to SPI for SPR, so it is only included in SPR platform.
        If there is any other additional programming for other projects, need to add this driver to the project.

  @param ImageHandle  -  Handle for the image of this driver.
  @param SystemTable  -  Pointer to the EFI System Table.

  @retval EFI_SUCCESS  -  Module launched successfully.

**/
EFI_STATUS
EFIAPI
UncoreMiscDxeEntry (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  //
  // Save the adaptation table to the SPI ROM
  //
  SaveAdaptationTableToNvram ();

  return EFI_SUCCESS;
}
