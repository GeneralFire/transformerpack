/** @file
  Unified Silicon Register Access (USRA) Ppi

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2017 Intel Corporation. <BR>

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

#ifndef __SILICON_REG_ACCESS_PPI_H__
#define __SILICON_REG_ACCESS_PPI_H__

#include <UsraAccessType.h>

extern EFI_GUID gUsraPpiGuid;

/**
  This API Perform 8-bit, 16-bit, 32-bit or 64-bit silicon register read operations.
  It transfers data from a register into a naturally aligned data buffer.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out
  @param[in] Buffer               A pointer of buffer for the value read from the register

  @retval RETURN_SUCCESS          The function completed successfully.
  @retval Others                  Return Error
**/
typedef
RETURN_STATUS
(EFIAPI *USRA_PPI_REG_READ) (
  IN USRA_ADDRESS             *Address,
  OUT VOID                    *Buffer
  );

/**
  This API Perform 8-bit, 16-bit, 32-bit or 64-bit silicon register write operations.
  It transfers data from a naturally aligned data buffer into a silicon register.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be written
  @param[in] Buffer               A pointer of buffer for the value write to the register

  @retval RETURN_SUCCESS          The function completed successfully.
  @retval Others                  Return Error
**/
typedef
RETURN_STATUS
(EFIAPI *USRA_PPI_REG_WRITE) (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *Buffer
  );

/**
  This API Perform 8-bit, 16-bit, 32-bit or 64-bit silicon register AND then OR operations. It read data from a
  register, And it with the AndBuffer, then Or it with the OrBuffer, and write the result back to the register

  @param[in] Address              A pointer of the address of the silicon register to be written
  @param[in] AndBuffer            A pointer of buffer for the value used for AND operation
                                  A NULL pointer means no AND operation. RegisterModify() equivalents to RegisterOr()
  @param[in] OrBuffer             A pointer of buffer for the value used for OR operation
                                  A NULL pointer means no OR operation. RegisterModify() equivalents to RegisterAnd()

  @retval RETURN_SUCCESS          The function completed successfully.
  @retval Others                  Return Error
**/
typedef
RETURN_STATUS
(EFIAPI *USRA_PPI_REG_MODIFY) (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *AndBuffer,
  IN VOID                     *OrBuffer
  );

/**
  This API get the flat address from the given USRA Address.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out

  @retval                         The flat address
**/
typedef
UINTN
(EFIAPI *USRA_PPI_GET_ADDR) (
  IN USRA_ADDRESS              *Address
  );

///
/// This service abstracts the ability to read/write silicon register.
///
typedef struct {
  USRA_PPI_REG_READ            RegRead;
  USRA_PPI_REG_WRITE           RegWrite;

  USRA_PPI_REG_MODIFY          RegModify;
  USRA_PPI_GET_ADDR            GetRegAddr;
} USRA_PPI;

/**
  This API Perform 8-bit, 16-bit, 32-bit or 64-bit silicon register read operations.
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
  );

/**
  This API Perform 8-bit, 16-bit, 32-bit or 64-bit silicon register write operations.
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
  );

/**
  This API Perform 8-bit, 16-bit, 32-bit or 64-bit Pcie silicon register AND then OR operations. It read data from a
  register, And it with the AndBuffer, then Or it with the OrBuffer, and write the result back to the register

  @param[in] Address              A pointer of the address of the silicon register to be written
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
  );

/**
  This API get the flat address from the given USRA Address.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out

  @retval                         The flat address
**/
UINTN
EFIAPI
PeiGetRegAddr (
  IN USRA_ADDRESS             *Address
  );

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
  );

#endif
