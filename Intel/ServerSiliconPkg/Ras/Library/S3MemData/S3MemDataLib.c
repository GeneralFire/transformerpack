/** @file
  Implementation of generate RAS global data access for silicon library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#include <Library/RasGlobalDataLib.h>
#include <Library/S3MemDataLib.h>
#include <Library/DebugLib.h>
#include <Include/UncoreCommonIncludes.h>

MEMRAS_S3_PARAM    *mS3RasData;

/**
  Get imc S3 RAS data.

  @param   none.
  @retval  point of S3 RAS data.

**/
MEMRAS_S3_PARAM *
EFIAPI
GetS3RasData (
  VOID
  )
{
  return mS3RasData;
}

/**
  The constructor function caches the pointer to SMM Variable protocol.

  The constructor function locates SMM variable protocol from protocol database.
  It will ASSERT() if that operation fails and it will always return EFI_SUCCESS.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
InitializeSmmS3MemData (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{

  BOOLEAN             NewAllocation;

  mS3RasData = GetRasGlobalData (L"S3RasData", sizeof (MEMRAS_S3_PARAM), &NewAllocation);

  return EFI_SUCCESS;
}

/**
  Set full mirror channel failed flag

  @param  Node            - Node ID
  @param  ChOnMc          - Channel number per Mc

**/
VOID
EFIAPI
SetFullMirrorChFailed (
  IN  UINT8         Node,
  IN  UINT8         ChOnMc
  )
{
  MEMRAS_S3_PARAM       *MemrasS3Param;

  ASSERT (Node < MC_MAX_NODE);
  if (Node >= MC_MAX_NODE) {
    DEBUG ((EFI_D_ERROR, "\n  Invalid parameter!"));
    return;
  }
  ASSERT (ChOnMc < MAX_MC_CH);
  if (ChOnMc >= MAX_MC_CH) {
    DEBUG ((EFI_D_ERROR, "\n  Invalid parameter!"));
    return;
  }

  //
  // Get Memory RAS Runtime Data
  //
  MemrasS3Param = GetS3RasData ();
  MemrasS3Param->FullMirrorChFailed[Node][ChOnMc] = 0x01;
}

/**
  Check whether the full mirror channel is failed

  @param  Node            - Node ID
  @param  ChOnMc          - Channel number per Mc

  @retval TRUE -- Failed; FALSE -- Not failed full mirror channel.

**/
BOOLEAN
EFIAPI
IsFullMirrorChFailed (
  IN  UINT8         Node,
  IN  UINT8         ChOnMc
  )
{
  MEMRAS_S3_PARAM       *MemrasS3Param;

  ASSERT (Node < MC_MAX_NODE);
  if (Node >= MC_MAX_NODE) {
    DEBUG ((EFI_D_ERROR, "\n  Invalid parameter!"));
    return FALSE;
  }
  ASSERT (ChOnMc < MAX_MC_CH);
  if (ChOnMc >= MAX_MC_CH) {
    DEBUG ((EFI_D_ERROR, "\n  Invalid parameter!"));
    return FALSE;
  }

  //
  // Get Memory RAS Runtime Data
  //
  MemrasS3Param = GetS3RasData ();
  if (MemrasS3Param->FullMirrorChFailed[Node][ChOnMc]) {
    return TRUE;
  } else {
    return FALSE;
  }
}