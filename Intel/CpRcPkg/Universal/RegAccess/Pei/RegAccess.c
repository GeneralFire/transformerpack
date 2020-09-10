/** @file
  All Pcd Ppi services are implemented here.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2018 Intel Corporation. <BR>

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

#include <Ppi/SiliconRegAccess.h>
#include <Library/SiliconAccessLib.h>
#include <Library/DebugLib.h>
#include <Library/SystemInfoLib.h>

///
/// Instance of PCD_PPI protocol is EDKII native implementation.
/// This protocol instance supports dynamic and dynamicEx type PCDs.
///
USRA_PPI mUsraPpiInstance = {
  PeiRegRead,
  PeiRegWrite,
  PeiRegModify,
  PeiGetRegAddr
};


EFI_PEI_PPI_DESCRIPTOR  mPpiList = {
    EFI_PEI_PPI_DESCRIPTOR_PPI  | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gUsraPpiGuid,
    &mUsraPpiInstance
};

static EFI_PEI_NOTIFY_DESCRIPTOR mMemDiscoveredNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  FixUpPpi
};

/**
  Main entry for USRA PEIM driver.
  This routine installs USRA access ppi.

  @param[in] FileHandle           A pointer of the address of the USRA Address Structure to be read out
  @param[in] PeiServices          Describes the list of possible PEI Services

  @retval Status                  Status of install USRA ppi
**/
EFI_STATUS
EFIAPI
UsarPeimInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS Status;

  //
  // Install Unified Register Access Ppi
  //
  Status = PeiServicesInstallPpi (&mPpiList);
  ASSERT_EFI_ERROR (Status);

  SetUsraPpiPtr ((UINTN)&mUsraPpiInstance);

  Status = PeiServicesNotifyPpi (&mMemDiscoveredNotifyList);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Fix up Ppi once PEI core has migrated NEM data to permanent memory.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
FixUpPpi (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS Status;
  EFI_PEI_PPI_DESCRIPTOR*  UsraPpi;
  UINTN tempPpi;

  DEBUG ((DEBUG_INFO, "FixUpPpi ENTER\n"));

  Status = PeiServicesLocatePpi (
          &gUsraPpiGuid,
          0,
          (EFI_PEI_PPI_DESCRIPTOR **)&UsraPpi,
          (VOID **)&tempPpi
          );

  ASSERT_EFI_ERROR (Status);

  //
  // Install Unified Register Access Ppi
  //
  Status = PeiServicesReInstallPpi (UsraPpi, &mPpiList);
  ASSERT_EFI_ERROR (Status);

  SetUsraPpiPtr ((UINTN)&mUsraPpiInstance);

  DEBUG ((DEBUG_INFO, "FixUpPpi EXIT\n"));

  return EFI_SUCCESS;
}

/**
  This API performs 8-bit, 16-bit, 32-bit or 64-bit silicon register read operations.
  It transfers data from a register into a naturally aligned data buffer.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out
  @param[in] Buffer               A pointer of buffer for the value read from the register

  @retval RETURN_SUCCESS          The function completed successfully.
  @retval Others                  Return Error
**/
RETURN_STATUS
EFIAPI
PeiRegRead (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *Buffer
  )
{
  return InternalRegisterRead(Address, Buffer);
}

/**
  This API performs 8-bit, 16-bit, 32-bit or 64-bit silicon register write operations.
  It transfers data from a naturally aligned data buffer into a silicon register.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be written
  @param[in] Buffer               A pointer of buffer for the value write to the register

  @retval RETURN_SUCCESS          The function completed successfully.
  @retval Others                  Return Error
**/
RETURN_STATUS
EFIAPI
PeiRegWrite (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *Buffer
  )
{
  return InternalRegisterWrite(Address, Buffer);
}

/**
  This API performs 8-bit, 16-bit, 32-bit or 64-bit Pcie silicon register AND then OR operations. It read data from a
  register, And it with the AndBuffer, then Or it with the OrBuffer, and write the result back to the register

  @param[in] Address              A pointer of the address of the silicon register to be modified
  @param[in] AndBuffer            A pointer of buffer for the value used for AND operation
                                  A NULL pointer means no AND operation. RegisterModify() equivalents to RegisterOr()
  @param[in] OrBuffer             A pointer of buffer for the value used for OR operation
                                  A NULL pointer means no OR operation. RegisterModify() equivalents to RegisterAnd()

  @retval RETURN_SUCCESS          The function completed successfully.
  @retval Others                  Return Error
**/
RETURN_STATUS
EFIAPI
PeiRegModify (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *AndBuffer,
  IN VOID                     *OrBuffer
  )
{
  return InternalRegisterModify(Address, AndBuffer, OrBuffer);
}

/**
  This API gets the flat address from the given USRA Address.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out

  @retval                         The flat address
**/
UINTN
EFIAPI
PeiGetRegAddr (
  IN USRA_ADDRESS               *Address
  )
{
  return InternalGetRegisterAddress(Address);
};

