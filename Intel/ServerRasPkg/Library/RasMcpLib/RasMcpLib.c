/** @file
  Implementation of mapping/remapping of Skt/SktCh for Mcp Pkg.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#include <Library/RasDebugLib.h>
#include <Library/RasGlobalDataLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <UncoreCommonIncludes.h>
#include <Library/KtiApi.h>
#include <Library/PlatMemTopologyLib.h>
#include <Guid/Cper.h>

/**
  This function is to update the Node and Card info in PlatformErrorData for MCP.
  For the CPX 2DieInSocket, Update the channels belong to SlaveNode as master Node,
  and Slave NodeId as Master NodeId

  @param [in out] ErrorData               Error data should record socket/channel field.
  @retval    none

**/

VOID
EFIAPI
UpdateMemErrRecordForMcp (
  IN OUT EFI_PLATFORM_MEMORY_ERROR_DATA     *ErrorData
  )
{
  MEM_TOPOLOGY                          *MemTopology;
  UINT8                                  DieCount = 0;

  MemTopology = GetMemTopology ();

  if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {

    DieCount = MemTopology->DieCount;

    if (DieCount == TWO_DIE_IN_SOCKET) {
      RAS_DEBUG ((LEVEL_REG, "MCP Socket identified. Update Node and Card Info.."));

      //
      // Each Pkg in MCP looks like each individual Socket to BIOS.
      // For Error Error reporting,  Slave Socket shouldn't be considered,
      // and all the memory channels in slave socket to be reported as part of Master Socket.
      //
      if (ErrorData->Node & 1) {                        // Identifies if the socket is master or slave
        ErrorData->Card = ErrorData->Card + 4;          // 4 is the Maximum number of channels per Chip in CPX MCP
      }
      ErrorData->Node = ErrorData->Node / DieCount;     // Skipping the Slave SocketId
    }
    RAS_DEBUG ((LEVEL_REG, "Update Node: 0x%x, Updated Card: 0x%x\n", ErrorData->Node, ErrorData->Card));
  }

  return;
}


/**
  This function is to get the Socket and SocketCh per Chip in the MCP from PLATFORM_MEMORY_ERROR_DATA.
  The following Listners need to have the Socet, SocketCh per Chip

  1) PprVlsErrorLogLisnter

**/


VOID
EFIAPI
ReMapSktAndSktChForMcp (
  IN OUT  UINT8     *Socket,
  IN OUT  UINT8     *SocketCh
  )
{
  MEM_TOPOLOGY                          *MemTopology;
  UINT8                                  DieCount = 0;
  UINT8                                  Skt;
  UINT8                                  SktCh;


  MemTopology = GetMemTopology ();

  if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    DieCount = MemTopology->DieCount;

    Skt = *Socket;
    SktCh = *SocketCh;
    //
    // Get the original Skt Id for MCP
    //
    if (DieCount == TWO_DIE_IN_SOCKET) {
      Skt = Skt * DieCount;
      if (SktCh >= 4) {                        // If th SktCh >= 4 (max. number of channels in CPX MCP), then it is slave die
        SktCh = SktCh - 4;
        Skt = Skt + 1;
      }

      *Socket = Skt;
      *SocketCh = SktCh;
    }
  }

  return;
}
