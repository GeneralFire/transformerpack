/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/KtiApi.h>
#include <Library/EmulationConfigurationLib.h>
#include <RcRegs.h>
#include <Upi/KtiSi.h>
#include <Library/UsraCsrLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/KtiSimLib.h>
#include <Library/UpimdfIpAccessLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/CpuAndRevisionLib.h>

/**
  Get the Maximum number of UPIMDF instances on one UPIMDF subsystem
  @param Socket        - The Socket number

  @return The Maximum number of UPIMDF instances on one UPIMDF subsystem.
**/
UINT8
EFIAPI
GetMaxUpimdfInsNum (
  IN UINT8    Socket
  )
{
  if (IsCpuAndRevision (CPU_SPRSP, REV_UX)) {
    if (GetDieCount(Socket) == TWO_DIE_IN_SOCKET) {
      return MAX_UPIMDF_INSTANCE_SPRUCC;
    } else if (GetDieCount(Socket) == ONE_DIE_IN_SOCKET) {
      return (MAX_UPIMDF_INSTANCE_SPRUCC / 2);
    }
  }
  return 0;
}

/**
  UPIMDF IP (UPI Prototol layer & link layer, UPIMDF MDFIS) Read access

  @param Socket        - The Socket number
  @param UpiPort       - The UPI Port number
  @param UpimdfInsId   - The specified UPIMDF instance ID (0 ~ (MAX_UPIMDF_INSTANCE-1))
  @param CsrOffset     - The UPIMDF Csr Offset

  @return Value.
**/
UINT32
EFIAPI
UpimdfIpReadAccess32 (
  IN UINT8    Socket,
  IN UINT8    UpiPort,
  IN UINT8    UpimdfInsId,
  IN UINT32   CsrOffset
  )
{
  UINT8     UpimdfBoxInstance;

  if (IsUpiPortUpimdf (Socket, UpiPort)) {
    UpimdfBoxInstance = UpimdfInsId + UPIMDF_INST_OFFSET;
    return UsraCsrRead (Socket, UpimdfBoxInstance, CsrOffset);
  } else {
    return UsraCsrRead (Socket, UpiPort, CsrOffset);
  }
}

/**
  UPIMDF IP (UPI Prototol layer & link layer, UPIMDF MDFIS) write access

  @param Socket        - The Socket number
  @param UpiPort       - The UPI Port number
  @param UpimdfInsId   - The specified UPIMDF instance ID (0 ~ (MAX_UPIMDF_INSTANCE-1)), 0xFF -- Broadcase write
  @param CsrOffset     - The UPIMDF Csr Offset
  @param Data32        - The Data write to the registers

  @return .
**/
VOID
EFIAPI
UpimdfIpWriteAccess32 (
  IN UINT8    Socket,
  IN UINT8    UpiPort,
  IN UINT8    UpimdfInsId,
  IN UINT32   CsrOffset,
  IN UINT32   Data32
  )
{
  UINT8     UpimdfBoxInstance;
  UINT8     UpimdfIndex, MaxUpimdfInsNum;

  if (IsUpiPortUpimdf (Socket, UpiPort)) {
    if (UpimdfInsId != UPIMDF_INSTANCE_BROADCAST) {
      //
      // Unicast a UPIMDF instance
      //
      UpimdfBoxInstance = UpimdfInsId + UPIMDF_INST_OFFSET;
      UsraCsrWrite (Socket, UpimdfBoxInstance, CsrOffset, Data32);
    } else {
      //
      // Broadcast all the UPIMDF instances per UPIMDF Subsystem
      //
      MaxUpimdfInsNum = GetMaxUpimdfInsNum (Socket);
      if (MaxUpimdfInsNum == 0) {
        DEBUG ((EFI_D_ERROR, "\n\nERROR: The MAX number of UPIMDF instance is 0!"));
        return;
      }
      for (UpimdfIndex = 0; UpimdfIndex < MaxUpimdfInsNum; UpimdfIndex++) {
        UpimdfBoxInstance = UpimdfIndex + UPIMDF_INST_OFFSET;
        UsraCsrWrite (Socket, UpimdfBoxInstance, CsrOffset, Data32);
      }
    }
  } else {
    UsraCsrWrite (Socket, UpiPort, CsrOffset, Data32);
  }

  return;
}