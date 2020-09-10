/** @file
  NumaAcpiTableLib internal header file for the library common code consumption.

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

#ifndef _NUMA_ACPI_TABLE_INTERNAL_H_
#define _NUMA_ACPI_TABLE_INTERNAL_H_

#include <Library/NumaAcpiTableLib.h>

/**
  This function sets the NUMA ACPI table data buffer pointer.

  @retval EFI_SUCCESS              This function is executed successfully.
  @retval EFI_BAD_BUFFER_SIZE      Failed to allocate memory for NUMA ACPI data.
**/
EFI_STATUS
SetNumaAcpiDataPointer (
  VOID
  );

/**
  This function retrieves the pointer of NUMA ACPI table data buffer.
  If the buffer pointer has not been set yet, this function will allocate
  a buffer and set the buffer pointer. NULL will be returned if the pointer
  is failed to be set.

  @retval Pointer to the NUMA ACPI table data buffer or NULL.
**/
VOID*
GetNumaAcpiDataPointer (
  VOID
  );

/**
  This function adds the system resource information into the proximity domain.
  A new proximity domain will be created if the specified one is uninitialized.
  Otherwise, the specified proximity domain will be updated to include the new
  resource information.

  @param[in] DomainId            Proximity domain index.
  @param[in] SocketId            Socket index.
  @param[in] ClusterId           Cluster index within socket.
  @param[in] DomainAttr          Proximity domain attribute.
  @param[in] DomainData          Pointer to the proximity domain data to be added.

  @retval EFI_SUCCESS            This function is executed successfully.
  @retval EFI_LOAD_ERROR         Failed to get the proximity domain data buffer pointer.
  @retval EFI_ACCESS_DENIED      Updating is denied due to socket and/or cluster mismatched.
  @retval EFI_INVALID_PARAMETER  Some of the input parameters are invalid.
**/
EFI_STATUS
AddProximityDomain (
  IN UINT32                    DomainId,
  IN UINT8                     SocketId,
  IN UINT8                     ClusterId,
  IN PROXIMITY_DOMAIN_ATTR     DomainAttr,
  IN VOID                      *DomainData
  );

#endif  // _NUMA_ACPI_TABLE_INTERNAL_H_
