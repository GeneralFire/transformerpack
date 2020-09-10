/** @file
  Stubs of DXE specific functions not used in SMM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Protocol/Dcpmm.h>

#include "Nfit.h"
#include "NvdimmAcpiConfig.h"


/*****************************************************************************
 * Definitions.
 *****************************************************************************/


/*****************************************************************************
 * Global variables
 *****************************************************************************/


/*****************************************************************************
 * Functions
 *****************************************************************************/

/**
  @brief This function installs DCPMM FIS protocol in DXE, not used in SMM.

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
DcpmmProtocolInstall (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  ASSERT (FALSE);
  return EFI_SUCCESS;
}


/**
  @brief This function builds NFIT structures in DXE, not used in SMM.

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
NfitBuildTable (
     OUT NVDIMM_FW_INTERFACE_TABLE *NfitPtr,
  IN     UINTN                      NfitMaxSize
  )
{
  ASSERT (FALSE);
  return EFI_SUCCESS;
}


/**
  @brief This function updates NFIT checksum in DXE, not used in SMM.

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
NfitUpdateTableChecksum (
  IN OUT NVDIMM_FW_INTERFACE_TABLE *NfitPtr
  )
{
  ASSERT (FALSE);
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
DcpmmGetPlatformInterpretedLss (
  IN  UINT32                        NfitHandle,
  OUT DCPMM_INTERPRETED_LSS        *InterpretedLssPtr,
  OUT DCPMM_INTERPRETED_LSS_REASON *InterpretedLssReasonPtr
  )
{
  ASSERT (FALSE);
  return EFI_UNSUPPORTED;
}

EFI_STATUS
LoadAcpiSsdtNvdrTable (
  VOID
)
{
  ASSERT (FALSE);
  return EFI_SUCCESS;
}

EFI_STATUS
InstallNfitTableUpdateProtocol (
  IN EFI_HANDLE        ImageHandle
)
{
  ASSERT (FALSE);
  return EFI_SUCCESS;
}

EFI_STATUS
InstallNvdimmAcpiConfigProtocol (
  IN EFI_HANDLE  *Handle
  )
{
  ASSERT (FALSE);
  return EFI_UNSUPPORTED;
}
