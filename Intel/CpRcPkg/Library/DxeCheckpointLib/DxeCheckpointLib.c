/** @file Dxe implementation of the Checkpoint library

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
#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Protocol/CpuCsrAccess.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SystemInfoLib.h>


EFI_CPU_CSR_ACCESS_PROTOCOL *mCpuCsrAccess = NULL;


/**
  Writes the checkpoint code to the checkpoint CSR and breaks if match with debug breakpoint

  @param[in] MajorCode - Major Checkpoint code to write
  @param[in] MinorCode - Minor Checkpoint code to write
  @param[in] Data      - Data specific to the minor checkpoint is written to
                         low word of the checkpoint CSR
**/
VOID
EFIAPI
OutputCheckpoint (
  IN UINT8            MajorCode,
  IN UINT8            MinorCode,
  IN UINT16           Data
  )
{
  UINT8               SocketId;
  EFI_STATUS          Status;

  if (mCpuCsrAccess == NULL) {
    Status = gBS->LocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess);
    ASSERT_EFI_ERROR (Status);
  }

  SocketId = GetSysSocketId ();

  mCpuCsrAccess->BreakAtCheckpoint (SocketId, MajorCode, MinorCode, Data);

} // OutputCheckpoint

/**
  Writes the checkpoint code to the checkpoint CSR and breaks if match with debug breakpoint

  @param[in] Socket    - Socket to write Checkpoint
  @param[in] MajorCode - Major Checkpoint code to write
  @param[in] MinorCode - Minor Checkpoint code to write
  @param[in] Data      - Data specific to the minor checkpoint is written to
                         low word of the checkpoint CSR
**/
VOID
EFIAPI
OutputCheckpointSocket (
  IN UINT8    Socket,
  IN UINT8    MajorCode,
  IN UINT8    MinorCode,
  IN UINT16   Data
  )
{
  UINT8    SocketIdBak;

  SocketIdBak = GetSysSocketId ();

  SetSysSocketId (Socket);

  OutputCheckpoint (MajorCode,  MinorCode, Data);

  SetSysSocketId (SocketIdBak);

} // OutputCheckpointSocket
