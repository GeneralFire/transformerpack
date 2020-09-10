/** @file

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

#include "CxlDxe.h"

/**

  This function constructs CXL UEFI memory map by promoting normal memory to CXL memory

  @param[in] CxlNodeHob               Pointer to CXL node HOB.

**/
VOID
ConstructCxlUefiMemoryMap (
  IN CXL_NODE_SOCKET                  *CxlNodeHob
  )
{
  EFI_STATUS                          Status;
  CXL_NODE_INFO                       *CxlNodeInfo;
  UINT8                               CxlNodeId;
  UINT8                               SocketId;
  EFI_PHYSICAL_ADDRESS                Address;
  UINT64                              Length;
  UINT64                              Capabilities;
  EFI_GCD_MEMORY_TYPE                 GcdMemType;

  DEBUG ((DEBUG_INFO, "[CXL] Construct CXL UEFI memory map\n"));
  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    for (CxlNodeId = 0; CxlNodeId < CxlNodeHob[SocketId].CxlNodeCount; CxlNodeId++) {
      CxlNodeInfo = &CxlNodeHob[SocketId].CxlNodeInfo[CxlNodeId];
      if ((CxlNodeInfo->Attr & CXL_NODE_ATTR_MEM) != CXL_NODE_ATTR_MEM) {
        continue; // Skip CXL device without HDM.
      }
      Address = LShiftU64 (CxlNodeInfo->Address, SAD_UNIT); // Convert 64MB to byte
      Length = LShiftU64 (CxlNodeInfo->Size, SAD_UNIT);   // Convert 64MB to byte
      //
      // Remove CXL memory range if it has been added as normal memory
      //
      Status = gDS->RemoveMemorySpace(Address, Length);
      DEBUG ((DEBUG_INFO, "[CXL] Remove %lx-%lx memory space Status:%r\n", Address, Address + Length, Status));
      if (!EFI_ERROR (Status) || (Status == EFI_NOT_FOUND)) {
        if ((CxlNodeInfo->Attr & CXL_NODE_ATTR_MEM_HW_INIT) == 0) {
          //
          // Reserve the memory if HW initialization flag has not been set
          //
          GcdMemType = EfiGcdMemoryTypeReserved;
        } else if ((CxlNodeInfo->Attr & CXL_NODE_ATTR_PERSISTENT) != 0) {
          GcdMemType = EfiGcdMemoryTypePersistent;
        } else {
          GcdMemType = EfiGcdMemoryTypeSystemMemory;
        }

        Capabilities = EFI_MEMORY_UC | EFI_MEMORY_WC | EFI_MEMORY_WT | EFI_MEMORY_WB | EFI_MEMORY_SP;
        if ((CxlNodeInfo->Attr & CXL_NODE_ATTR_PERSISTENT) != 0) {
          Capabilities |= EFI_MEMORY_NV;
        }

        //
        // Add CXL memory range with the proper capabilities based on CXL node attributes
        //
        Status = gDS->AddMemorySpace (GcdMemType, Address, Length, Capabilities);
        DEBUG ((DEBUG_INFO, "[CXL] Add memory space type %x cap %x Status: %r\n", GcdMemType, Capabilities, Status));
        if (!EFI_ERROR (Status)) {
          //
          // Set CXL memory range with specific-purpose memory (SPM)
          //
          Status = gDS->SetMemorySpaceAttributes (Address, Length, EFI_MEMORY_SP);
          DEBUG ((DEBUG_INFO, "[CXL] Set memory space as SPM Status: %r\n", Status));
        }
      }
    }
  }
}

