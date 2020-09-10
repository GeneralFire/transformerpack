/** @file
 Definitions for HECI Transport driver

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

**/
#ifndef _HECI_COMMON_LIB_H_
#define _HECI_COMMON_LIB_H_

#include <Library/SmmServicesTableLib.h>
#include <Protocol/HeciAccessProtocol.h>
#include <Protocol/HeciTransportProtocol.h>

#define DEBUG_SMM_DRIVER_STRING         "SMM"
#define DEBUG_DXE_DRIVER_STRING         "DXE"
#define DEBUG_PEI_DRIVER_STRING         "PEI"

/**
  Function returns CHAR16 string pointer (DXE/SMM/PEI)
  for module identification in debug output

  @retval Pointer to CHAR16 module identification string
  @retval NULL pointer indicates error
**/
CHAR16 *
GetModuleIdentificationString (
  VOID
  );

/**
  Allocate memory for Heci (Dxe/Smm)

  @param[in]      AllocationSize   Amount memory to allocate

  @retval   Allocated memory pointer, NULL on failure
**/
VOID*
HeciAllocateMemory (
  IN UINTN   AllocationSize
  );

/**
  Free memory for Heci (Dxe/Smm)

  @param[in]      Memory   Memory pointer to free`
**/
VOID
HeciFreeMemory (
  IN VOID   *Memory
  );

/**
  Function installs DXE or SMM protocol depending on mode we are running in

  @param[in]     HeciGuidProtocol    Pointer to Dxe/Smm protocol guid to install
  @param[in]     HeciProtocol        Pointer to HECI protocol to be installed

  @retval Protocol installation status
  @retval EFI_UNSUPPORTED if called in PEI
**/
EFI_STATUS
InstallHeciProtocol (
  EFI_GUID                *HeciGuidProtocol,
  VOID                    *HeciProtocol
  );


/**
  Return HECI number based on HECI Handle

  @param[in]      PciFunction    Pci function to be converted to HECI number
  @param[in,out]  HeciNumber     Pointer to returned Heci number
                                 Returned HECI communication number 1..4
                                 0 is returned for non communication devices
                                 (SOL and IDEr)

  @retval EFI_INVALID_PARAMETER Invalid input parameters
  @retval EFI_DEVICE_ERROR      Passed function not supported
  @retval EFI_SUCCESS           Returned valid HECI number
**/
EFI_STATUS
GetHeciNumber (
  IN     UINTN        PciFunction,
  IN OUT UINTN        *HeciNumber
  );

/**
  Get instance index of given PPI for given GUID.

  @param[in]     InputPpi         PPi instance pointer to look for index
  @param[in]     InputPpiPpiGuid  PPi Guid to search for


  @retval PPI instance index
  @retval MAX_UINT32 if found no instance
**/
UINTN
GetPpiInstanceIndex (
  IN     VOID              *InputPpi,
  IN     EFI_GUID          *InputPpiPpiGuid
  );

/**
  Get next instance of given PPI for given GUID.

  @param[in]     InputPpi         PPi instance pointer to look for next instance
  @param[in]     InputPpiPpiGuid  PPi Guid to search for

  @retval PPI pointer Next to input PPI object
  @retval NULL in error case
**/
VOID*
GetNextPpi (
  IN     VOID              *InputPpi,
  IN     EFI_GUID          *InputPpiPpiGuid
  );

/**
  Get first HeciAccess protocol pointer

  @retval Pointer to first HeciAccess protocol
  @retval NULL pointer indicates error
**/
HECI_ACCESS*
GetFirstHeciAccess (
  VOID
  );

/**
  Get first HeciTransport protocol pointer

  @retval Pointer to first HeciTransport protocol
  @retval NULL pointer indicates error
**/
HECI_TRANSPORT*
GetFirstHeciTransport (
  VOID
  );
#endif // _HECI_COMMON_LIB_H_
