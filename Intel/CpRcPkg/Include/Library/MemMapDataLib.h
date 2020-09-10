/** @file
  Interface of memory map access data

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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

#ifndef __MEM_MAP_DATA_LIB_H__
#define __MEM_MAP_DATA_LIB_H__

#include <Guid/MemoryMapData.h>
#include <Include/MemMapHost.h>

/**
  Saves system memory map data.

  This library API saves memory map data so that it can be accesses in later DXE phase.
  It can only be invoked in PEI phase after memory map has been established and pernament
  memory is ready.

**/
VOID
EFIAPI
SaveSystemMemoryMapData (
  VOID
  );


/**
  Get system memory map data.

  This library API retrieves memory map data structure if it is available. Otherwise,
  NULL pointer is returned.

  @return Pointer to system memory map data or NULL if the memory map data is not available

**/
SYSTEM_MEMORY_MAP_HOB *
EFIAPI
GetSystemMemoryMapData (
  VOID
  );

/**

  This routine points the input pointer to the DdrtDimmList entry that corresponds
  to the input Sckt, Ch, and Dimm indexes.

  The pointer is fetched from the Host structure.

  @param[in]     Sckt             - Socket index
  @param[in]     Ch               - Channel index
  @param[in]     Dimm             - Dimm index

  @return Pointer to the DdrtDimmList entry

**/
DDRT_DIMM_DEVICE_INFO_STRUCT *
GetDdrtDimmInfo (
  IN     UINT8                        Sckt,
  IN     UINT8                        Ch,
  IN     UINT8                        Dimm
  );

/**

  This routine points the input pointer to the DdrtDimmInfo entry that corresponds
  to the input Sckt, Ch, and Dimm indexes.

  The pointer is fetched from the global Host pointer.

  @param[in]     Sckt            - Socket index
  @param[in]     Ch              - Channel index
  @param[in]     Dimm            - Dimm index

  @return Pointer to the DdrtDimmInfo entry

**/
HOST_DDRT_DIMM_DEVICE_INFO_STRUCT *
GetHostDdrtDimmInfo (
  IN     UINT8                            Sckt,
  IN     UINT8                            Ch,
  IN     UINT8                            Dimm
  );
#endif
