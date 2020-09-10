/** @file
  Interface of CPU and Revision library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 -2020 Intel Corporation. <BR>

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

#ifndef _CPU_AND_REVISION_LIB_H_// #ifndef _CPU_AND_REVISION_LIB_H_
#define _CPU_AND_REVISION_LIB_H_

#include <Uefi.h>
#include <CpuAndRevisionDefines.h>


/**
  Check if the current CPU executing is this type and revision.

  @param[in] CpuType      UINT8 of the CpuType define
  @param[in] Revision     UINT16 of the Revision to check against

  @retval  TRUE    Is the requested CPU and Revision
           FALSE   Is not the requested CPU and Revision

**/
BOOLEAN
EFIAPI
IsCpuAndRevision (
  IN UINT8  CpuType,
  IN UINT16 Revision
  );

/**
  Check if the current CPU executing is this Cpu type and this revision or
  a later revision.  This would be used if code needs to apply for one
  revision and all future revisions of the same CPU type.

  @param[in] CpuType      UINT8 of the CpuType define
  @param[in] Revision     UINT16 of the Revision to start with

  @retval  TRUE    Is the requested CPU and Revision
           FALSE   Is not the requested CPU and Revision

**/
BOOLEAN
EFIAPI
IsCpuAndRevisionOrLater (
  IN UINT8  CpuType,
  IN UINT16 Revision
  );

/**
  Function for a very specific case where mixed steppings are populated
  on the board.  This will align CPU data so that the lowest stepping Id
  is used for all CPU and revision checks.  This should only be called
  by KTI code.

  @param   UINT8       SteppingId - Physical Stepping Id to change to.

  @retval  EFI_STATUS  Status - EFI_SUCCESS: Revision updated
                                EFI_ALREADY_STARTED: Already called

**/
EFI_STATUS
EFIAPI
NormalizeCpuAndRevision (
  UINT8     SocketId
  );

/**
  Set CPU type and revision data. Only valid for simulation
  build targets, will assert on hardware targets.  Passing
  in REV_ALL as the Revision will select first Revision for
  the CpuType passed in.

  @param[in] CpuType      UINT8 of the CpuType define
  @param[in] Revision     UINT16 of the Revision to use

  @retval    None

**/
VOID
EFIAPI
SetCpuAndRevision (
  IN UINT8  CpuType,
  IN UINT16 Revision
  );
#endif // #ifndef _CPU_AND_REVISION_LIB_H_
