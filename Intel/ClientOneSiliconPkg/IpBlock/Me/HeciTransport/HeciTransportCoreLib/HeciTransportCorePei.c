/** @file
  HECI Transport protocol driver (PEI)

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2020 Intel Corporation.

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
#include <PiPei.h>
#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/HeciTransportCoreLib.h>
#include <Library/HeciCommonLib.h>
#include <Ppi/HeciTransportPpi.h>
#include <Ppi/HeciAccessPpi.h>


/**
  Get Next HECI Transport

  @param[in]     This          Pointer to HECI Transport device context structure
  @param[in]     HeciTransport Pointer to HECI Transport device context structure
                               the next element should be relative to
                               Passing NULL requests first element

  @retval HECI_TRANSPORT Next HeciTransport object
**/
HECI_TRANSPORT*
GetNextHeciTransport (
  IN      HECI_TRANSPORT   *This,
  IN      HECI_TRANSPORT   *HeciTransport
  )
{
  return GetNextPpi (HeciTransport, &gHeciTransportPpiGuid);
}

/**
  Get HECI_TRANSPORT instance for given PchIndex and HeciFunction in HeciAccess.
  (PEI only)

  @param[in]     HeciAccess           Heci Access that parameters HeciAccess Has to match
  @param[in,out] HeciCurrentInstance  Pointer to found instance id

  @retval HECI_TRANSPORT pointer
  @retval NULL pointer if no HECI_TRANSPORT_PRIVATE is found
**/
HECI_TRANSPORT*
GetHeciTransportForHeciAccess (
  IN     HECI_ACCESS      *HeciAccess,
  IN OUT UINTN            *HeciCurrentInstance
  )
{
  HECI_TRANSPORT_PRIVATE *HeciTransportPrivate;
  UINTN                  HeciAccessPciSegment;
  UINTN                  HeciAccessPciBus;
  UINTN                  HeciAccessPciDevice;
  UINTN                  HeciAccessHeciFunction;
  UINTN                  HeciInstance;
  EFI_STATUS             Status;

  Status = HeciAccess->PciIo.GetLocation (
                                &(HeciAccess->PciIo),
                                &HeciAccessPciSegment,
                                &HeciAccessPciBus,
                                &HeciAccessPciDevice,
                                &HeciAccessHeciFunction
                                );
  ASSERT_EFI_ERROR (Status);
  HeciInstance = 0;
  HeciTransportPrivate = (HECI_TRANSPORT_PRIVATE *)GetFirstHeciTransport ();
  while ((HeciTransportPrivate != NULL) &&
         ((HeciAccessPciSegment != HeciTransportPrivate->PciSegment) ||
          (HeciAccessPciBus != HeciTransportPrivate->PciBus) ||
          (HeciAccessPciDevice != HeciTransportPrivate->PciDevice) ||
          (HeciAccessHeciFunction != HeciTransportPrivate->PciFunction))) {
    HeciInstance++;
    HeciTransportPrivate = (HECI_TRANSPORT_PRIVATE *)GetNextHeciTransport (
                                                       (HECI_TRANSPORT *)HeciTransportPrivate,
                                                       (HECI_TRANSPORT *)HeciTransportPrivate
                                                       );

    if (HeciTransportPrivate == NULL) {
      break;
    }
  }

  if (HeciCurrentInstance != NULL) {
    *HeciCurrentInstance = HeciInstance;
  }

  return (HECI_TRANSPORT *)HeciTransportPrivate;
}

/**
  Function returns HeciAccess for given HeciTransport
  Function implementation is deliver by parent module to HeciTransportCore library

  @param[in] HeciTransport        HEciTransport for getting HeciAccess


  @retval HECI_ACCESS*            On success valid pointer to linked HeciAccess is returned
  @retval NULL                    If HeciAccess can't be found, NULL is returned
**/
HECI_ACCESS*
GetHeciAccess (
  IN     HECI_TRANSPORT_PRIVATE         *HeciTransport
 )
{
  HECI_ACCESS  *ReturnHeciAccess;
  VOID         *MemoryDiscoveredPpi;
  EFI_STATUS   Status;

  ReturnHeciAccess = NULL;
  if (HeciTransport != NULL) {
    if (HeciTransport->HeciAccess == NULL) {
      ASSERT (HeciTransport->HeciAccessIndex != MAX_INT32);
      if (HeciTransport->HeciAccessIndex != MAX_INT32) {
        Status = PeiServicesLocatePpi (
                   &gHeciAccessPpiGuid,
                   HeciTransport->HeciAccessIndex,
                   NULL,
                   &ReturnHeciAccess
                   );
        ASSERT_EFI_ERROR (Status);
        Status = PeiServicesLocatePpi (
                   &gEfiPeiMemoryDiscoveredPpiGuid,
                   0,
                   NULL,
                   &MemoryDiscoveredPpi
                   );
        if (!EFI_ERROR (Status)) {
          // After memory is discovered,
          // link HeciTransport and HeciAccess,
          // so lookup in the future is not required
          HeciTransport->HeciAccess = ReturnHeciAccess;
        }
      }
    } else {
      ReturnHeciAccess = HeciTransport->HeciAccess;
    }
  }

  ASSERT (ReturnHeciAccess != NULL);

  return ReturnHeciAccess;
}