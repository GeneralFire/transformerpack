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
#include <Library/BaseMemoryLib.h>
#include <Library/PipeSyncLib.h>
#include <Library/KtiApi.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemRcLib.h>
#include <Library/RcDebugLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/CxlMemLib.h>
#include <Library/HobLib.h>
#include <Guid/CxlNodeHob.h>
#include <MemDecode.h>
#include <Gen2/MemDecodeNode.h>

#define MIN_CXL_MEM_RANGE_SIZE     SIZE_256MB
#define MAX_CXL_MEM_RANGE_SIZE     SIZE_128GB

/**
  Add CXL memory devices to system.

  This function adds the newly discovered CXL memory device to the system.
  It judges the same CXL device based on the S/N and VID pair. If the same device is
  found, it just adds to the bit map for the installed socket and CXL stack bitmap.

  @param[in, out] MemMapData       Pointer to memory map private data structure.
  @param[in]      SocketId         The socket number.
  @param[in]      CxlStackId       The stack ID for the CXL device.
  @parpm[in]      CxlMemDeviceInfo The CXL memory device info to add.

**/
VOID
AddCxlMemDevice (
  IN OUT MEMORY_MAP_DATA      *MemMapData,
  IN     UINT8                SocketId,
  IN     UINT8                CxlStackId,
  IN     CXL_MEM_DEVICE_INFO  *CxlMemDeviceInfo
  )
{
  CXL_DATA                    *CxlData;
  CXL_MEM_DEVICE_DATA         *CxlMemDeviceData;
  CXL_HDM_DATA                *CxlHdmData;
  UINT8                       CxlDeviceId;
  UINT8                       CxlHdmId;
  UINT64                      CxlHdmSize;

  CxlData = &MemMapData->CxlData;
  for (CxlDeviceId = 0; CxlDeviceId < CxlData->CxlDeviceNum; CxlDeviceId++) {
    CxlMemDeviceData = &CxlData->CxlMemDeviceData[CxlDeviceId];
    if ((CxlMemDeviceData->SerialNumber == CxlMemDeviceInfo->SerialNumber) &&
        (CxlMemDeviceData->VendorId == CxlMemDeviceInfo->VendorId)) {
      break; // The CXL device is already found
    }
  }
  if (CxlDeviceId == CxlData->CxlDeviceNum) {
    //
    // Add the newly found CXL device
    //
    CxlMemDeviceData = &CxlData->CxlMemDeviceData[CxlData->CxlDeviceNum++];
    CxlMemDeviceData->SerialNumber = CxlMemDeviceInfo->SerialNumber;
    CxlMemDeviceData->VendorId = CxlMemDeviceInfo->VendorId;
    CxlMemDeviceData->HdmCount = CxlMemDeviceInfo->HdmCount;
    if (CxlMemDeviceData->HdmCount == 0) {
      CxlMemDeviceData->Status |= CXL_STATUS_UNSUPPORTED; // Ignore CXL device w/o memory.
    } else {
      CxlMemDeviceData->MemHwInitMode = CxlMemDeviceInfo->MemHwInitMode;
      for (CxlHdmId = 0; CxlHdmId < MAX_CXL_HDM_RANGES; CxlHdmId++) {
        CxlHdmData = &CxlMemDeviceData->HdmData[CxlHdmId];
        CxlHdmData->Size = CxlMemDeviceInfo->CxlMemInfo[CxlHdmId].MemSize;
        CxlHdmData->MemExpander = CxlMemDeviceInfo->MemExpander;
        CxlHdmSize = LShiftU64 (CxlHdmData->Size, SAD_UNIT);
        if (CxlHdmSize > MAX_CXL_MEM_RANGE_SIZE) {
          RcDebugPrint (SDBG_ERROR,
            "CXL device socket %d stack %d HDM %d unsupported: size %lx is larger than maximum %lx\n",
            SocketId,
            CxlStackId,
            CxlHdmId,
            CxlHdmSize,
            MAX_CXL_MEM_RANGE_SIZE
          );
          CxlMemDeviceData->Status |= CXL_STATUS_UNSUPPORTED;
        }

        CxlHdmData->MediaType = CxlMemDeviceInfo->CxlMemInfo[CxlHdmId].MediaType;
        if (CxlHdmData->MediaType >= MaxCxlMediaType) {
          RcDebugPrint (SDBG_ERROR,
            "CXL device socket %d stack %d HDM %d unsupported: Media Type %d is unsupported\n",
            SocketId,
            CxlStackId,
            CxlHdmId,
            CxlHdmData->MediaType
          );
          CxlMemDeviceData->Status |= CXL_STATUS_UNSUPPORTED;
        }

        CxlHdmData->DesiredInterleave = CxlMemDeviceInfo->CxlMemInfo[CxlHdmId].DesiredInterleave;
        if (CxlHdmData->DesiredInterleave >= MaxCxlInterleave) {
          RcDebugPrint (SDBG_ERROR,
            "CXL device socket %d stack %d HDM %d unsupported: Desired Interleave %d is unsupported\n",
            SocketId,
            CxlStackId,
            CxlHdmId,
            CxlHdmData->DesiredInterleave
          );
          CxlMemDeviceData->Status |= CXL_STATUS_UNSUPPORTED;
        }
      }
    }
  }

  CxlMemDeviceData->SocketBitmap |= BIT0 << SocketId;
  CxlMemDeviceData->StackBitmap[SocketId] |= BIT0 << CxlStackId;
}

/**
  Populates CXL Memory devices by scanning all CXL stacks.

  This function scans all enabled sockets for CXL memory devices.

  @param[in, out] MemMapData     Pointer to memory map private data structure.

**/
VOID
PopulateCxlMemDevices (
  IN OUT MEMORY_MAP_DATA      *MemMapData
  )
{
  UINT8                   SocketId;
  UINT8                   CxlStackId;
  EFI_STATUS              Status;
  CXL_MEM_DEVICE_INFO     CxlMemDeviceInfo;

  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (!CheckSocketEnabled (SocketId)) {
      continue;
    }
    for (CxlStackId = 0; CxlStackId < MAX_CXL_PER_SOCKET; CxlStackId++) {
      ZeroMem (&CxlMemDeviceInfo, sizeof (CxlMemDeviceInfo));
      Status = GetCxlMemDeviceInfo (SocketId, CxlStackId, &CxlMemDeviceInfo);
      if (EFI_ERROR (Status)) {
        continue;
      }
      AddCxlMemDevice (MemMapData, SocketId, CxlStackId, &CxlMemDeviceInfo);
    }
  }
}

/**
  Calculates CXL memory interleave.

  This function calculates the final interleave for the HDM.
  If its desired interleave is 0, this memory range is not interleaved.
  If the stack bitmap is not a power of 2, we will downgrade the newest power of 2
  so that this range can be interleaved across multiple CXL stacks.

  @param[in, out] CxlHdmData     Pointer to logic HDM data structure.
  @param[in]      CxlStackBitmap Bitmap of CXL stacks on the sockets

**/
VOID
CalculateCxlHdmInterleave (
  IN OUT CXL_HDM_DATA         *CxlHdmData,
  IN     UINT8                CxlStackBitmap
  )
{
  UINT8                   CxlInterleaveWays;
  UINT8                   Index;
  UINT8                   CxlStack;
  UINT8                   Package;
  UINT8                   StartBit;

  CxlInterleaveWays = CalculateInterleaveWaysfromBitmap (CxlStackBitmap);
  if (CxlHdmData->DesiredInterleave == CxlNoInterleave)  {
    CxlHdmData->InterleaveWays = 1;
  } else {
    //
    // Downgrade to the nearest power of 2 links
    //
    CxlHdmData->InterleaveWays = (UINT8) MIN (MAX_TGT_LIST_ENTRY, (BIT0 << HighBitSet32 (CxlInterleaveWays)));
  }

  if (CxlHdmData->InterleaveWays != CxlInterleaveWays) {
    RcDebugPrint (SDBG_WARN, "Downgrade CXL interleave ways from %d to %d\n",
      CxlInterleaveWays,
      CxlHdmData->InterleaveWays
      );
  }
  //
  // Calculate package interleave list
  //
  Package = 0;
  do {
    Index = 0;
    for (CxlStack = 0; CxlStack < MAX_CXL_PER_SOCKET; CxlStack++) {
      if ((CxlStackBitmap & (BIT0 << CxlStack)) != 0) {
        Index += 1;
        StartBit = Package++ * 4;
        CxlHdmData->InterleaveList = BitFieldWrite32 (
                                       CxlHdmData->InterleaveList,
                                       StartBit,
                                       StartBit + 3,
                                       BIT3 | CxlStack
                                       );
        if (Index == CxlHdmData->InterleaveWays) {
          break;
        }
      }
    }
  } while (Package < MAX_TGT_LIST_ENTRY);
}

/**
  Checks CXL memory device data structure.

  This function checks the installed CXL memory devices and it will do some necessary downgrade
  so that these devices can be mapped to system memory.

  @param[in, out] MemMapData  Pointer to memory map private data structure.

**/
VOID
CheckCxlMemDevices (
  IN OUT MEMORY_MAP_DATA      *MemMapData
  )
{
  CXL_DATA                    *CxlData;
  CXL_MEM_DEVICE_DATA         *CxlMemDeviceData;
  UINT8                       CxlDeviceId;
  UINT8                       SocketWays;
  UINT8                       SocketId;
  CXL_HDM_DATA                *CxlHdmData;
  UINT8                       CxlHdmId;

  CxlData = &MemMapData->CxlData;
  for (CxlDeviceId = 0; CxlDeviceId < CxlData->CxlDeviceNum; CxlDeviceId++) {
    CxlMemDeviceData = &CxlData->CxlMemDeviceData[CxlDeviceId];
    SocketWays = CalculateInterleaveWaysfromBitmap (CxlMemDeviceData->SocketBitmap);
    SocketId = (UINT8) HighBitSet32 (CxlMemDeviceData->SocketBitmap);
    if (SocketWays > ONE_WAY) {
      //
      // Pick up one socket if one CXL device is connect to more than one socket.
      //
      RcDebugPrint (SDBG_WARN,
        "CXL device VID %x S/N %lx is connect to multiple sockets: ignore other socket connection than %d\n",
        CxlMemDeviceData->VendorId,
        CxlMemDeviceData->SerialNumber,
        SocketId
        );
    }
    for (CxlHdmId = 0; CxlHdmId < MAX_CXL_HDM_RANGES; CxlHdmId++) {
      CxlHdmData = &CxlMemDeviceData->HdmData[CxlHdmId];
      if (CxlMemDeviceData->HdmData[CxlHdmId].Size == 0) {
        continue;
      }
      CalculateCxlHdmInterleave (CxlHdmData, CxlMemDeviceData->StackBitmap[SocketId]);
    }
  }
}

/**
  Shows CXL memory device data structure.

  This function prints the platform CXL memory topology.

  @param[in] MemMapData  Pointer to memory map private data structure.

**/
VOID
PrintCxlMemDevices (
  IN MEMORY_MAP_DATA          *MemMapData
  )
{
  CXL_DATA                    *CxlData;
  CXL_MEM_DEVICE_DATA         *CxlMemDeviceData;
  UINT8                       CxlHdmId;
  UINT8                       CxlDeviceId;
  UINT8                       SocketId;

  CxlData = &MemMapData->CxlData;
  if (CxlData->CxlDeviceNum == 0) {
    return;
  }

  RcDebugPrint (SDBG_MAX, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  RcDebugPrint (SDBG_MAX, "          Platform CXL Memory Configuration\n");
  RcDebugPrint (SDBG_MAX, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  RcDebugPrint (SDBG_MAX, "VID  S/N              Status SocketBitmap");
  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    RcDebugPrint (SDBG_MAX, " StackBitmap[%d]", SocketId);
  }
  RcDebugPrint (SDBG_MAX, " Size1 MT1 DI1 Size2 MT2 DI2\n");
  for (CxlDeviceId = 0; CxlDeviceId < CxlData->CxlDeviceNum; CxlDeviceId++) {
    CxlMemDeviceData = &CxlData->CxlMemDeviceData[CxlDeviceId];
    RcDebugPrint (SDBG_MAX, "%04x %-16lx %6x %12x",
      CxlMemDeviceData->VendorId,
      CxlMemDeviceData->SerialNumber,
      CxlMemDeviceData->Status,
      CxlMemDeviceData->SocketBitmap
      );
    for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
      RcDebugPrint (SDBG_MAX, " %14x", CxlMemDeviceData->StackBitmap[SocketId]);
    }
    for (CxlHdmId = 0; CxlHdmId < MAX_CXL_HDM_RANGES; CxlHdmId++) {
      RcDebugPrint (SDBG_MAX, " %5x %3d %3d",
        CxlMemDeviceData->HdmData[CxlHdmId].Size,
        CxlMemDeviceData->HdmData[CxlHdmId].MediaType,
        CxlMemDeviceData->HdmData[CxlHdmId].DesiredInterleave
      );
    }
    RcDebugPrint (SDBG_MAX, "\n");
  }
}

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
  if (MemMapData->MemMapState != MEM_MAP_STATE_RESOURCE_CALCULATION_FAILURE) {
    PopulateCxlMemDevices (MemMapData);
    CheckCxlMemDevices (MemMapData);
  }
  if (MemMapData->MemMapState == MEM_MAP_STATE_RESOURCE_CALCULATION) {
    PrintCxlMemDevices (MemMapData);
  }
}

/**
  Create CXL SAD entry for the logic HDM data structure.

  This function creates SAD entry CXL memory type based on CXL HDM data structure.
  If the input HdmData is NULL, it will create NXM rule for padding purpose.

  @param[in, out] MemMapData  Pointer to memory map private data structure.
  @param[in, out] HdmData     Pointer to CXL HDM data structure.
  @param[in]      SocketId    Socket number.
  @param[in]      Size        Memory size to create HDM

  @retval SUCCESS SAD entry is successfully created for the CXL device.
  @retval others  Some error happens when creating SASD entry to the socket.

**/
UINT32
CreateCxlSadEntry (
  IN OUT MEMORY_MAP_DATA      *MemMapData,
  IN OUT CXL_HDM_DATA         *HdmData OPTIONAL,
  IN     UINT8                SocketId,
  IN     UINT32               Size
  )
{
  UINT8                       SadIndex;
  SYSHOST                     *Host;
  SAD_TABLE                   *SadEntry;
  UINT32                      SadLimit;

  Host = GetSysHostPointer ();
  SadLimit = GetSadLimit (Host, MemMapData);
  SadIndex = FindSadIndex (Host, MemMapData, SocketId, 0);
  if (SadIndex >= MemMapData->MaxSadRules) {
    RcDebugPrintWithDevice (SDBG_MINMAX, SocketId, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "SAD rules exceed %d. Cannot create SAD for Socket %d\n",
        MemMapData->MaxSadRules,
        SocketId
        );
    OutputWarning (
      WARN_INTERLEAVE_FAILURE,
      WARN_SAD_RULES_EXCEEDED,
      SocketId,
      0xFF,
      0xFF,
      0xFF
      );
    return ERROR_RESOURCE_CALCULATION_FAILURE;
  }

  SadEntry = &Host->var.mem.socket[SocketId].SAD[SadIndex];
  SadEntry->Enable = 1;
  SadEntry->local = 1;
  SadEntry->Base = SadLimit;
  SadEntry->Limit = SadEntry->Base + Size;
  SadEntry->Attr = SAD_CXL_ATTR;
  if (HdmData != NULL) {
    SadEntry->ways = HdmData->InterleaveWays;
    if (HdmData->DesiredInterleave == Cxl4KBInterleave) {
      SadEntry->tgtGranularity = INTERLEAVE_GRANULARITY_4KB;
    } else {
      SadEntry->tgtGranularity = INTERLEAVE_GRANULARITY_256B;
    }
    if (HdmData->MediaType == CxlNonVolatileMem) {
      SadEntry->type = HdmData->MemExpander ? MemTypeCxlExpPersistentMem : MemTypeCxlAccPersistentMem;
    } else {
      SadEntry->type = HdmData->MemExpander ? MemTypeCxlExpVolatileMem : MemTypeCxlAccVolatileMem;
    }
    HdmData->SadEntry = SadEntry;
  } else {
    SadEntry->type = MemTypeNxm;
  }
  //
  // Add remote Dram rule for newly created CXL SAD entries.
  //
  return AddRemoteSadEntry (
           Host,
           MemMapData,
           SocketId,
           SadEntry->type,
           0
           );
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
  UINT32                      Status;
  UINT8                       CxlDeviceId;
  UINT8                       CxlHdmId;
  CXL_HDM_DATA                *CxlHdmData;
  CXL_DATA                    *CxlData;
  CXL_MEM_DEVICE_DATA         *CxlMemDeviceData;
  UINT32                      SadLimit;
  SYSHOST                     *Host;
  UINT32                      CxlNxmPaddingSize;

  CxlData = &MemMapData->CxlData;
  for (CxlDeviceId = 0; CxlDeviceId < CxlData->CxlDeviceNum; CxlDeviceId++) {
    CxlMemDeviceData = &CxlData->CxlMemDeviceData[CxlDeviceId];
    if ((CxlMemDeviceData->Status & CXL_STATUS_UNSUPPORTED) != 0) {
      continue; // Skip unsupported
    }
    if (HighBitSet32 (CxlMemDeviceData->SocketBitmap) != SocketId) {
      continue;
    }
    for (CxlHdmId = 0; CxlHdmId < MAX_CXL_HDM_RANGES; CxlHdmId++) {
      CxlHdmData = &CxlMemDeviceData->HdmData[CxlHdmId];
      if (CxlHdmData->Size == 0) {
        continue;
      }
      Status = CreateCxlSadEntry (MemMapData, CxlHdmData, SocketId, CxlHdmData->Size);
      if (Status != SUCCESS) {
        return Status;
      }
    }
  }

  Status = SUCCESS;
  if (SncGbAlignRequired ()) {
    //
    // Align socket number with GB by inserting NXM SAD rule.
    //
    Host = GetSysHostPointer ();
    SadLimit = GetSadLimit (Host, MemMapData);
    CxlNxmPaddingSize = ((SadLimit + (~GB_BOUNDARY_ALIGN)) & GB_BOUNDARY_ALIGN) - SadLimit;
    if (CxlNxmPaddingSize != 0) {
      Status = CreateCxlSadEntry (MemMapData, NULL, SocketId, CxlNxmPaddingSize);
    }
  }

  return Status;
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
  UINT32                      InterleaveList;
  UINT8                       CxlDeviceId;
  UINT8                       CxlHdmId;
  CXL_DATA                    *CxlData;
  CXL_MEM_DEVICE_DATA         *CxlMemDeviceData;
  CXL_HDM_DATA                *CxlHdmData;

  InterleaveList = 0;
  CxlData = &MemMapData->CxlData;
  for (CxlDeviceId = 0; CxlDeviceId < CxlData->CxlDeviceNum; CxlDeviceId++) {
    CxlMemDeviceData = &CxlData->CxlMemDeviceData[CxlDeviceId];
    if ((CxlMemDeviceData->Status & CXL_STATUS_UNSUPPORTED) != 0) {
      continue; // Skip unsupported
    }
    for (CxlHdmId = 0; CxlHdmId < MAX_CXL_HDM_RANGES; CxlHdmId++) {
      CxlHdmData = &CxlMemDeviceData->HdmData[CxlHdmId];
      if (CxlHdmData->SadEntry == SadEntry) {
        InterleaveList = CxlHdmData->InterleaveList;
        break;
      }
    }
  }

  return InterleaveList;
}


/**
  Configure CXL memory devices.

  This function configures CXL memory devices after finalizing memory map.

  @param[in] MemMapData       Pointer to memory map private data structure.

**/
VOID
ConfigureCxlMemDevices (
  IN MEMORY_MAP_DATA          *MemMapData
  )
{
  UINT8                       CxlDeviceId;
  UINT8                       CxlHdmId;
  CXL_DATA                    *CxlData;
  CXL_MEM_DEVICE_DATA         *CxlMemDeviceData;
  CXL_HDM_DATA                *CxlHdmData;
  CXL_MEM_DEVICE_CONFIG       CxlMemDeviceConfig;
  UINT8                       CxlStackId;
  UINT8                       SocketId;

  CxlData = &MemMapData->CxlData;
  for (CxlDeviceId = 0; CxlDeviceId < CxlData->CxlDeviceNum; CxlDeviceId++) {
    CxlMemDeviceData = &CxlData->CxlMemDeviceData[CxlDeviceId];
    if ((CxlMemDeviceData->Status & CXL_STATUS_UNSUPPORTED) != 0) {
      continue; // Skip unsupported
    }
    ZeroMem (&CxlMemDeviceConfig, sizeof (CxlMemDeviceConfig));
    for (CxlHdmId = 0; CxlHdmId < MAX_CXL_HDM_RANGES; CxlHdmId++) {
      CxlHdmData = &CxlMemDeviceData->HdmData[CxlHdmId];
      if (CxlHdmData->SadEntry != NULL) {
        CxlMemDeviceConfig.CxlMemConfig[CxlHdmId].BaseAddress = LShiftU64 (CxlHdmData->SadEntry->Base, SAD_UNIT);
        CxlMemDeviceConfig.CxlMemConfig[CxlHdmId].LimitAddress = LShiftU64 (CxlHdmData->SadEntry->Limit, SAD_UNIT);
      }
    }
    SocketId = (UINT8) HighBitSet32 (CxlMemDeviceData->SocketBitmap);
    CxlStackId = (UINT8) LowBitSet32 (CxlMemDeviceData->StackBitmap[SocketId]);
    ConfigureCxlMemDeviceInfo (SocketId, CxlStackId, &CxlMemDeviceConfig);
  }
}


/**
  Adds CXL node in CXL node HOB and initializes partial data.

  @param[in, out] CxlNodeHob        Point to CXL node HOB.
  @param[in]      SocketId          Socket number.
  @param[in]      CXlMemDeviceData  Point to CXL memory device data.

  @return Pointer to the newly added CXL node.

**/
CXL_NODE_INFO *
AddCxlNode (
  IN OUT CXL_NODE_SOCKET      *CxlNodeHob,
  IN     UINT8                SocketId,
  IN     CXL_MEM_DEVICE_DATA  *CxlMemDeviceData
  )
{
  CXL_NODE_INFO               *CxlNodeInfo;

  CxlNodeInfo = &CxlNodeHob[SocketId].CxlNodeInfo[CxlNodeHob[SocketId].CxlNodeCount];
  CxlNodeInfo->VendorId = CxlMemDeviceData->VendorId;
  CxlNodeInfo->SerialNumber = CxlMemDeviceData->SerialNumber;
  RcDebugPrint (SDBG_MAX, "%4d %4d %4x %8x ",
    SocketId, CxlNodeHob[SocketId].CxlNodeCount, CxlNodeInfo->VendorId, CxlNodeInfo->SerialNumber);

  CxlNodeHob[SocketId].CxlNodeCount++;
  return CxlNodeInfo;
}


/**
  Exposes CXL node info via HOB for UEFI memory map and ACPI tables

  This function expose CXL node info into MemMapHost so that we can construct UEFI memory
  map and ACPI tables for OS/EFI driver interaction.

  @param[in] MemMapData       Pointer to memory map private data structure.

**/
VOID
PublishCxlNodeHob (
  IN MEMORY_MAP_DATA          *MemMapData
  )
{
  UINT8                               SocketId;
  UINT8                               CxlDeviceId;
  UINT8                               CxlHdmId;
  CXL_DATA                            *CxlData;
  CXL_MEM_DEVICE_DATA                 *CxlMemDeviceData;
  CXL_NODE_INFO                       *CxlNodeInfo;
  CXL_NODE_SOCKET                     *CxlNodeHob;
  SAD_TABLE                           *SadEntry;

  CxlData = &MemMapData->CxlData;
  if (CxlData->CxlDeviceNum == 0) {
    return;
  }

  CxlNodeHob = BuildGuidHob (&gCxlNodeHobGuid, sizeof (CXL_NODE_SOCKET) * MAX_SOCKET);
  if (CxlNodeHob == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return;
  }
  CxlNodeHob = ZeroMem (CxlNodeHob, sizeof (CXL_NODE_SOCKET) * MAX_SOCKET);

  RcDebugPrint (SDBG_MAX, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  RcDebugPrint (SDBG_MAX, "          CXL NODE INFO\n");
  RcDebugPrint (SDBG_MAX, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

  RcDebugPrint (SDBG_MAX, "%4a %4a %4a %8a %4a %8a %8a\n", "Sckt", "Node", "VID", "S/N", "Attr", "Addr", "Size");
  for (CxlDeviceId = 0; CxlDeviceId < CxlData->CxlDeviceNum; CxlDeviceId++) {
    CxlMemDeviceData = &CxlData->CxlMemDeviceData[CxlDeviceId];
    SocketId = (UINT8) HighBitSet32 (CxlMemDeviceData->SocketBitmap);

    if ((CxlMemDeviceData->Status & CXL_STATUS_UNSUPPORTED) != 0) {
      CxlNodeInfo = AddCxlNode (CxlNodeHob, SocketId, CxlMemDeviceData);
      RcDebugPrint (SDBG_MAX, "%4x %8a %8a\n", CxlNodeInfo->Attr, "-", "-");
      continue; // CXL devices w/o memory
    }

    for (CxlHdmId = 0; CxlHdmId < MAX_CXL_HDM_RANGES; CxlHdmId++) {
      SadEntry = CxlMemDeviceData->HdmData[CxlHdmId].SadEntry;
      if (SadEntry != NULL) {
        CxlNodeInfo = AddCxlNode (CxlNodeHob, SocketId, CxlMemDeviceData);
        CxlNodeInfo->Attr |= CXL_NODE_ATTR_MEM;
        if (SadEntry->type == MemTypeCxlAccPersistentMem) {
          CxlNodeInfo->Attr |= CXL_NODE_ATTR_PERSISTENT;
        }
        if (CxlMemDeviceData->MemHwInitMode) {
          CxlNodeInfo->Attr |= CXL_NODE_ATTR_MEM_HW_INIT;
        }
        CxlNodeInfo->Address = SadEntry->Base;
        CxlNodeInfo->Size = SadEntry->Limit - SadEntry->Base;
        RcDebugPrint (SDBG_MAX, "%4x %8x %8x\n", CxlNodeInfo->Attr, CxlNodeInfo->Address, CxlNodeInfo->Size);
      }
    }
  }
}

