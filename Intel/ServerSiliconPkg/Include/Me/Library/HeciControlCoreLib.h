/** @file
  HECI Control driver core operations for use in PEI, DXE, and SMM.

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
#ifndef _HECI_CONTROL_CORE_LIB_H_
#define _HECI_CONTROL_CORE_LIB_H_

/**
  PCI defaults heci communication
**/
#define HECI_DEFAULT_COMMUNICATION_FUNCTION                  0
#define HECI_DEFAULT_COMMUNICATION_PCH_ID_RESOLVED           0 ///< PCH to use for default

/**
  HECI Control data instance definition.

  This protocol defines operations for HECI (Host Embedded Controller Interface)
  control access.
**/
typedef struct _HECI_CONTROL_PRIVATE_ {
  HECI_CONTROL                  HeciControl;         ///< HeciControl Protocol/Ppi

  // Control data
  HECI_TRANSPORT                *LastUsedHeciTransport;

  BOOLEAN                       PeiReinitialize;     /// < PPI reinitialization is required on memory discovered signal
} HECI_CONTROL_PRIVATE;

/***************************************************************************
  Prototypes of functions used by HeciControlCoreLib delivered by modules
****************************************************************************/

/**
  Initialize defined HECI communication interface (excluding ICH interface).

  @param[in]     This           HeciControl instance
  @param[in]     HeciTransport  HeciTransport instance

  @retval EFI_SUCCESS           Initialization succeed
  @retval EFI_INVALID_PARAMETER At least one of input parameter is NULL
**/
EFI_STATUS
EFIAPI
HeciControlInitalizeCommunicationDevice (
  IN      HECI_CONTROL_PRIVATE *This,
  IN      HECI_TRANSPORT       *HeciTransport
  );

/**
  Initialize all defined HECI communication interfaces (excluding ICH interfaces).

  @param[in]     This           HeciControl instance
**/
VOID
HeciControlInitalizeAllCommunicationDevices (
  IN      HECI_CONTROL_PRIVATE *This
  );

/**
  Initialize HECI_CONTROL protocol functions

  @param[out]     HeciControl HECI control pointer the be initialized
**/
VOID
SetHeciControlFunctions (
  OUT HECI_CONTROL_PRIVATE *HeciControl
  );

/**
  Initialize HECI_CONTROL_PRIVATE protocol instance

  @param[in]     HeciControl HECI control pointer the be initialized

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_SUCCESS           Heci control initialized
**/
EFI_STATUS
HeciInitializeNewControlInstance (
  IN OUT HECI_CONTROL_PRIVATE *HeciControl
  );

#endif // _HECI_CONTROL_CORE_LIB_H_
