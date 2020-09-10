/** @file
  Header file for BasicIpBlockServicesLib

  This library exists to abstract phase specific (PEI, DXE, SMM) most commonly used
  functionality to streamline developing phase agnostic Si IP block libraries.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#ifndef _BASIC_IP_BLOCK_SERVICES_LIB_H_
#define _BASIC_IP_BLOCK_SERVICES_LIB_H_

//
// Require PEI descriptor definition.  As this is counter-intuitive, it is explicitly included
//
#include <PiPei.h>

/**
  This service registers an interface into the phase specific database using
  the phase specific service from the UEFI PI Arch spec or UEFI Spec.
  The input follows PPI style descriptors to take a list of GUID/Pointer pairs.

  @param  InterfaceList         A pointer to the list of interfaces that the caller shall install.

  @retval EFI_SUCCESS           The interface was successfully installed.
  @retval EFI_INVALID_PARAMETER The InterfaceList pointer is NULL.
  @retval EFI_INVALID_PARAMETER Any of the PEI PPI descriptors in the list do not have the
                                EFI_PEI_PPI_DESCRIPTOR_PPI bit set in the Flags field.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space in the database.

**/
EFI_STATUS
EFIAPI
InstallInterface (
  IN CONST EFI_PEI_PPI_DESCRIPTOR     *InterfaceList
  );

/**
  This service locates an interface in the phase specific database using the
  phase specific service from the UEFI PI Arch spec or UEFI Spec.

  @param  Guid                  A pointer to the GUID whose corresponding interface needs to be
                                found.
  @param  Instance              The N-th instance of the interface that is required.
  @param  Interface             A pointer to the instance of the interface.

  @retval EFI_SUCCESS           The interface was successfully returned.
  @retval EFI_NOT_FOUND         The instance of the interface was not found in the database.

**/
EFI_STATUS
EFIAPI
LocateInterface (
  IN EFI_GUID                     *Guid,
  IN UINTN                        Instance,
  IN OUT VOID                     **Interface
  );

#endif // _BASIC_IP_BLOCK_SERVICES_LIB_H_
