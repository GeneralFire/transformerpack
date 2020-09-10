/** @file

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

#include <MemDecode.h>
#include <MemDecodeCommonIncludes.h>


/**
  Discover CXL memory devices.

  This function scans through all enabled sockets for CXL memory devices and constructs CXL
  memory topology in memory map private data structure.

  @param[in, out] MemMapData  Pointer to memory map private data structure.

**/
VOID
DiscoverCxlMemDevices (
  IN OUT MEMORY_MAP_DATA      *MemMapData
  )
{
  return;
}

/**
  Process CXL memory devices on the socket.

  This function will map CXL memory for a given socket

  @param[in, out] MemMapData  Pointer to memory map private data structure.
  @param[in]      SocketId    Socket number

  @retval SUCCESS The function executes successfully to map all CXL memory attached to the socket.
  @retval others  Some error happens when mapping CXL memory attached the socket.

**/
UINT32
ProcessCxlMemDevices (
  IN OUT MEMORY_MAP_DATA      *MemMapData,
  IN     UINT8                SocketId
  )
{
  return SUCCESS;
}

/**
  Finds the interleave list for the SAD entry.

  @param[in] MemMapData       Pointer to the memory map private data structure
  @param[in] SadEntry         The SAD entry to match.

  @return The encoded package list for the input SAD entry.

**/
UINT32
GetCxlMemInterleaveList (
  IN MEMORY_MAP_DATA          *MemMapData,
  IN SAD_TABLE                *SadEntry
  )
{
  return 0;
}

