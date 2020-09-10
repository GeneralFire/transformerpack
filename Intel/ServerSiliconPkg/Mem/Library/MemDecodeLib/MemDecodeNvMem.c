/** @file

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

#include <MemDecode.h>
#include <Library/SysHostPointerLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemTypeLib.h>
#include <Library/MemRcLib.h>

/** Finds the current memory limit for the requested socket

  This routine scans the SAD table for the most recent limit. For SKX, this is sufficient since both local
  and remote SAD are in this table. For 10nm, however, the remote SAD table must also be scanned for any
  limits higher than the one found in the SAD table.

  @param[in] Host         Pointer to sysHost.
  @param[in] MemMapData   Pointer to the memory map private data structure.
  @param[in] SocketNum    Socket requested for memory limit.
  @param[in] ClusterId    Cluster Id for the multicast DRAM rules access

  @retval    SadLimit     Current memory limit for the socket.

**/
UINT32
GetSadMemLimit (
  IN SYSHOST          *Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            SocketNum,
  IN UINT8            ClusterId
  )
{
  UINT8  SadIndex;
  UINT32 SadLimit = 0;

  SadIndex = FindSadIndex (Host, MemMapData, SocketNum, ClusterId);

  if (SadIndex > 0 && SadIndex < MAX_SAD_INDEX) {
    SadLimit = Host->var.mem.socket[SocketNum].SAD[SadIndex - 1].Limit;
  }

  return SadLimit;
}

/** Interleaves normal DDR4 memory (Non NVMem) on the channel level across sockets.

  This routine figures out if there is any normal DDR4 memory present in the system and calls SAD interleave function.
  This function works for cases where normal dimms are mixed with Type 01 NVDIMMs and also when AEP dimms are present.
  This function does not intereleave memory if the only memory presetn in the system is NVMem.

  @param[in]  Host       - Pointer to sysHost
  @param[in]  MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in]  MemType    - Type of memory to interleave

  @retval     SUCCESS    - Interleave completed
  @retval     FAILURE    - Interleave could not be supported for input memory type

**/
UINT32
SocketInterleaveNormalMem (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT16           MemType
  )
{
  UINT8   SadIndex;
  UINT32  Status = SUCCESS;
  UINT32  RegionLimit = 0;
  UINT32  RegionBase = 0;

  Host->var.mem.TotalInterleavedMemSize = 0;
  if (MemType == MemType1lmDdr) {
    if (Host->var.mem.memSize == Host->var.mem.NVmemSize) {// If only NVMem is present in the system do not interleave here.
      return SUCCESS;
    }
  }

  SadIndex = FindSadIndex (Host, MemMapData, 0, 0);
  RegionBase = GetSadBase (Host, MemMapData, 0, SadIndex);
  if (RegionBase == 0) {
    //
    // No SADs have been created, need to adjust for SAD0
    //
    RegionBase = FOUR_GB_MEM;
  }

  Status = SadUma (Host, MemMapData, MemType); // Interleave all volatile memory (or 2LM memory).

  RegionLimit = GetSadMemLimit (Host, MemMapData, 0, 0);

  Host->var.mem.TotalInterleavedMemSize = RegionLimit - RegionBase;

  return Status;
}

/** Interleaves NVMem (Type01) on the channel level across sockets

  This routine figures out if there is any NVMem and then calls SAD interleave function.
  If there no Type01 NVMEM in the system this function does nothing.

  @param[in]  Host       - Pointer to sysHost
  @param[in]  MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in]  MemType    - Type of memory to interleave

  @retval     SUCCESS    - Interleave completed
  @retval     FAILURE    - Interleave could not be supported for input memory type

**/
UINT32
SocketInterleaveNVMem (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT16           MemType
  )
{
  UINT8     i = 0;
  UINT8     j = 0;
  UINT8     Ch = 0;
  UINT8     MaxCh;
  UINT8     SocketNum;
  UINT8     OrigchInter = 0;
  UINT8     OrigimcInter = 0;
  UINT8     NvMemSadIndexStart = 0;
  UINT8     NvMemSadIndexEnd = 0;
  UINT32    Status = SUCCESS;
  UINT32    RegionBase = 0;
  UINT32    RegionLimit = 0;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  Host->var.mem.TotalInterleavedNVMemSize = 0;
  if (Host->var.mem.NVmemSize == 0) { // No NVMem in the system. Nothing to interleave.
    return SUCCESS;
  }

  // Load NVMem into remSize on each channel
  MaxCh = GetMaxImc () * GetNumChannelPerMc ();
  for (SocketNum = 0; SocketNum<MAX_SOCKET; SocketNum++) {
    for (Ch = 0; Ch < MaxCh; Ch++) {
      Host->var.mem.socket[SocketNum].channelList[Ch].remSize = Host->var.mem.socket[SocketNum].channelList[Ch].NVmemSize;
    }
  }
  OrigchInter = Host->var.mem.chInter;
  OrigimcInter = Host->var.mem.imcInter;
  if (Setup->mem.interNVDIMMS == 0 && Setup->mem.ADRDataSaveMode == ADR_NVDIMM) {
    Host->var.mem.imcInter = 1;
    Host->var.mem.chInter = 1;
  }

  NvMemSadIndexStart = FindSadIndex (Host, MemMapData, 0, 0);
  RegionBase = GetSadBase (Host, MemMapData, SocketNum, NvMemSadIndexStart);

  Status = SadUma (Host, MemMapData, MemType);

  Host->var.mem.chInter = OrigchInter;
  Host->var.mem.imcInter = OrigimcInter;

  NvMemSadIndexEnd = FindSadIndex (Host, MemMapData, 0, 0);
  for (i = NvMemSadIndexStart; i<NvMemSadIndexEnd; i++) {
    for (j = 0; j < MAX_SOCKET; j++) {
      MemMapData->MemMapSocket[j].NvMemSadIdxBitMap[0] |= (1 << i);
    }
  }

  RegionLimit = GetSadMemLimit (Host, MemMapData, 0, 0);
  Host->var.mem.TotalInterleavedNVMemSize = RegionLimit - RegionBase;

  return Status;
}

/** Interleaves normal DDR4 memory if present on the channel level but not across sockets

  This routine figures out the mapping and fills in the structures for the SAD, TAD, tables.
  It does not program any registers.

  @param[in]  Host       - Pointer to sysHost
  @param[in]  MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in]  SocketNum  - Socket ID
  @param[in]  MemType    - Type of memory to interleave
  @param[in]  Cluster    - Cluster in socket for SNC, or 0 if not SNC.

  @retval     SUCCESS    - Interleave completed.
  @retval     FAILURE    - Interleave could not be supported for input memory type.

**/
UINT32
SocketNonInterleaveNormalMem (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8            SocketNum,
  IN UINT16           MemType,
  IN UINT8            Cluster
  )
{
  UINT8               SadIndex;
  UINT32              Status = SUCCESS;
  UINT32              RegionLimit = 0;
  UINT32              RegionBase = 0;
  SOCKET_INFO_STRUCT  *Socket;

  Socket = &Host->var.mem.socket[SocketNum];
  Socket->TotalInterleavedMemSize = 0;
  if (SocketNum == 0) {
    Host->var.mem.TotalInterleavedMemSize = 0;
  }

  if (MemType == MemType1lmDdr) {
    if (GetSocketMemSize (SocketNum) == Host->var.mem.socket[SocketNum].NVmemSize) { //Only NVMEM is in this socket, do not interleave here.
      return SUCCESS;
    }
  }

  SadIndex = FindSadIndex (Host, MemMapData, SocketNum, Cluster);
  RegionBase = GetSadBase (Host, MemMapData, SocketNum, SadIndex);
  if (RegionBase == 0) {
    //
    // No SADs have been created, need to adjust for SAD0
    //
    RegionBase = FOUR_GB_MEM;
  }

  Status = SadNuma (Host, MemMapData, SocketNum, MemType, Cluster);

  RegionLimit = GetSadMemLimit (Host, MemMapData, SocketNum, Cluster);

  Socket->TotalInterleavedMemSize = RegionLimit - RegionBase;
  Host->var.mem.TotalInterleavedMemSize += Socket->TotalInterleavedMemSize;

  return Status;
}

/** Interleaves Type01 NVMEM if present on the channel level but not across sockets

  This routine figures out the mapping and fills in the structures for the SAD, TAD, tables.
  It does not program any registers.

  @param[in]  Host       - Pointer to sysHost
  @param[in]  MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in]  SocketNum  - Socket ID
  @param[in]  MemType    - Type of memory to interleave
  @param[in]  Cluster    - Cluster in socket for SNC, or 0 if not SNC.

  @retval     SUCCESS    - Interleave completed
  @retval     FAILURE    - Interleave could not be supported for input memory type

**/
UINT32
SocketNonInterleaveNVMem (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8            SocketNum,
  IN UINT16           MemType,
  IN UINT8            Cluster
  )
{
  UINT8               i = 0;
  UINT8               Ch = 0;
  UINT8               MaxCh;
  UINT8               OrigChInter = 0;
  UINT8               OrigImcInter = 0;
  UINT8               NvMemSadIndexEnd = 0;
  UINT8               NvMemSadIndexStart = 0;
  UINT32              Status = SUCCESS;
  UINT32              RegionLimit = 0;
  UINT32              RegionBase = 0;
  SYS_SETUP           *Setup;
  SOCKET_INFO_STRUCT  *Socket;

  Setup = GetSysSetupPointer ();
  Socket = &Host->var.mem.socket[SocketNum];
  Socket->TotalInterleavedNVMemSize = 0;
  if (SocketNum == 0) {
    Host->var.mem.TotalInterleavedNVMemSize = 0;
  }

  // No NVMem in the system. Nothing to interleave.
  if (Host->var.mem.socket[SocketNum].NVmemSize == 0) {
    return SUCCESS;
  }

  MaxCh = GetMaxImc () * GetNumChannelPerMc ();
  for (Ch = 0; Ch < MaxCh; Ch++) {
    Host->var.mem.socket[SocketNum].channelList[Ch].remSize = Host->var.mem.socket[SocketNum].channelList[Ch].NVmemSize;
  }
  OrigImcInter = Host->var.mem.imcInter;
  OrigChInter = Host->var.mem.chInter;
  if (Setup->mem.interNVDIMMS == 0 && Setup->mem.ADRDataSaveMode == ADR_NVDIMM) {
    Host->var.mem.imcInter = 1;
    Host->var.mem.chInter = 1;
  }

  NvMemSadIndexStart = FindSadIndex (Host, MemMapData, SocketNum, Cluster);
  RegionBase = GetSadBase (Host, MemMapData, SocketNum, NvMemSadIndexStart);

  Status = SadNuma (Host, MemMapData, SocketNum, MemType, Cluster);

  Host->var.mem.imcInter = OrigImcInter;
  Host->var.mem.chInter = OrigChInter;

  RegionLimit = GetSadMemLimit (Host, MemMapData, SocketNum, Cluster);
  Socket->TotalInterleavedNVMemSize = RegionLimit - RegionBase;
  Host->var.mem.TotalInterleavedNVMemSize += Socket->TotalInterleavedNVMemSize;

  NvMemSadIndexEnd = FindSadIndex (Host, MemMapData, SocketNum, Cluster);
  for (i = NvMemSadIndexStart; i<NvMemSadIndexEnd; i++) {
    MemMapData->MemMapSocket[SocketNum].NvMemSadIdxBitMap[MemMapData->NumberofChaDramClusters > 1 ? Cluster : 0] |= (1 << i);
  }
  RcDebugPrintWithDevice (SDBG_MAX, SocketNum, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "NvMemSadIndexStart = %d, NvMemSadIndexEnd = %d,NvMemSadIdxBitMap = 0x%x\n",
    NvMemSadIndexStart, NvMemSadIndexEnd,MemMapData->MemMapSocket[SocketNum].NvMemSadIdxBitMap[MemMapData->NumberofChaDramClusters > 1 ? Cluster : 0]);

  return Status;
}
