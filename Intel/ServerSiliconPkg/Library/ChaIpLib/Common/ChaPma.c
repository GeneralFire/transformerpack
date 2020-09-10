/** @file
  Common functions for configuring CHA_PMA* registers.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

//
// Include files
//
#include "Include/ChaRegs.h"
#include <Library/UsraCsrLib.h>
#include <Library/ChaLib.h>

/**
  CHA: This routine determines the route table settings for Odd CHAs and Even CHAs
  in 2-socket-duallink topology when CHA interleave mode is enabled in the setup
  knob.

  @param[in] RootSocId                  Root Node Socket ID (0 or 1 since this routine is specific to 2S2L)
  @param[in] SbspSktId                  SBSP Socket Node ID
  @param[in] SocketData                 Socket specific data structure
  @param[in] UpiRoutingMode             UPI routing mode, see UPI_ROUTING_MODE enum
  @param[out] OddChaData                Route table setting for Odd CHAs
  @param[out] EvenChaData               Route table setting for Even CHAs
  @param[out] FieldMask                 Clear Destination Node entry(s) in KTI_ROUTING_TABLE registers

  @retval  EFI_SUCCESS            Route table settings for odd and even CHAs, and field mask were 
                                  populated and returned.

**/
EFI_STATUS
EFIAPI
ChaInterleaveKtiRouteTableSettingFor2S2L (
  IN  UINT8              RootSocId,
  IN  UINT8              SbspSktId,
  IN  KTI_SOCKET_DATA    *SocketData,
  IN  UINT8              UpiRoutingMode,
  OUT UINT32             *OddChaData,
  OUT UINT32             *EvenChaData,
  OUT UINT32             *FieldMask
  )
{
  UINT8                             Port0;
  UINT8                             Port1;
  UINT8                             Port0PeerPort;
  UINT8                             Port1PeerPort;

  Port0 = Port1 = 0;
  Port0PeerPort = Port1PeerPort = 0;

  //
  // Using SBSP's ChaKtiRt table, determine UPI ports numbers on SBSP that are routed to Node ID 1
  //
  if (UpiRoutingMode == UpiRoutingModeB2) {
    //
    // 16-entry table
    //
    Port0 = (UINT8) ((SocketData->Route[SbspSktId].ChaKtiRt >> 6) & 7);
    Port1 = (UINT8) ((SocketData->Route[SbspSktId].ChaKtiRt >> 9) & 7);
    *FieldMask = 0xFFFFF000;  // Clear NodeID 0 and Node 1 entries; each node has 2 entries; each entry is 3-bit wide.
  } else {
    //
    // UpiRoutingModeB0 (8-entry table)
    //
    Port0 = (UINT8) (SocketData->Route[SbspSktId].ChaKtiRt & 3);
    Port1 = (UINT8) ((SocketData->Route[SbspSktId].ChaKtiRt >> 2) & 3);
    *FieldMask = 0xFFFFFFF0;  // Clear NodeID 0 and Node 1 entries; each node has 1 entry; each entry is 2-bit wide.
  }

  Port0PeerPort = (UINT8) (SocketData->Cpu[SbspSktId].LinkData[Port0].PeerPort);
  Port1PeerPort = (UINT8) (SocketData->Cpu[SbspSktId].LinkData[Port1].PeerPort);

  //
  // Construct the port settings for Even and Odd CHAs
  //

  if (UpiRoutingMode == UpiRoutingModeB2) {
    if (RootSocId == 1) {
      *EvenChaData = (UINT32) (Port0PeerPort | (Port0PeerPort << 3));
      *OddChaData = (UINT32) (Port1PeerPort | (Port1PeerPort << 3));
    } else {
      *EvenChaData = (UINT32) (Port0 | (Port0 << 3));
      *EvenChaData <<= 6;
      *OddChaData = (UINT32) (Port1 | (Port1 << 3));
      *OddChaData <<= 6;
    }
  } else {
    if (RootSocId == 1) {
      *EvenChaData = (UINT32) Port0PeerPort;
      *OddChaData = (UINT32) Port1PeerPort;
    } else {
      *EvenChaData = (UINT32) (Port0 << 2);
      *OddChaData = (UINT32) (Port1 << 2);
    }
  }

  return EFI_SUCCESS;
}
