 /** @file
  HECI Transport Protocol DXE SMM driver

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

#include <PiDxe.h>
#include <Base.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HeciTransportCoreLib.h>
#include <Library/HeciCommonLib.h>
#include <Library/MeUtilsLib.h>
#include <Protocol/HeciTransportProtocol.h>
#include <Protocol/HeciAccessProtocol.h>


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
  HECI_TRANSPORT  *ReturnHeciTransport;

  if (This == NULL) {
    return NULL;
  }

  if (HeciTransport == NULL) {
    ReturnHeciTransport = ((HECI_TRANSPORT_PRIVATE*)This)->HeciTransportRoot;
  } else {
    ReturnHeciTransport = ((HECI_TRANSPORT_PRIVATE*)HeciTransport)->HeciTransportNext;
  }

  return ReturnHeciTransport;
}

/**
  Function returns HeciAccess for given HeciTransport
  Function implementation is deliver by parent module to HeciTransportCore library

  @param[in] HeciTransport        HEciTransport for getting HeciAccess


  @retval HECI_ACCESS*            On success valid pointer to linked HeciAccess is returned
  @retval NULL                    If HEciAccess can't be finded NULL is returned
**/
HECI_ACCESS*
GetHeciAccess (
  HECI_TRANSPORT_PRIVATE         *HeciTransport
 )
{
  HECI_ACCESS  *ReturnHeciAccess;

  ASSERT (HeciTransport != NULL);

  ReturnHeciAccess = NULL;
  if (HeciTransport != NULL) {
    ReturnHeciAccess = HeciTransport->HeciAccess;
  }

  ASSERT (ReturnHeciAccess != NULL);

  return ReturnHeciAccess;
}