/** @file
  Memory map data library instance

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include <PiPei.h>
#include <Guid/MemoryMapData.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/KtiApi.h>
#include <Library/MemRcLib.h>
#include <Library/MemTypeLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/ChaLib.h>
#include <Library/HbmCoreLib.h>
#include <Guid/IioIpInterface.h>

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
  )
{
  MEMORY_MAP_HOST *MemMapHost;

  MemMapHost = GetMemMapHostPointer ();
  //
  // Only one DDRT DIMM is supported per channel.
  //
  return &(MemMapHost->SocketInfo[Sckt].ChannelInfo[Ch].DdrtDimmList[0]);
}

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
  )
{
  SYSHOST *Host;
  Host = GetSysHostPointer ();
  //
  // Only one DDRT DIMM is supported per channel.
  //
  return &(Host->var.mem.socket[Sckt].channelList[Ch].DdrtDimmList[0]);
}

/**
  Appends a protected NXM range to the current top of mapped cacheable memory for better MTRR alignment

  @param [in]  NxmLimitAddr     Limit of requested NXM range (byte address)

  @retval EFI_SUCCESS           Requested address range is cacheable
  @retval EFI_OUT_OF_RESOURCES  Requested address range is NOT cacheable
  @retval EFI_INVALID_PARAMETER Parameter is invalid
  @retval EFI_UNSUPPORTED       API is not supported
  @retval EFI_DEVICE_ERROR      SystemMemoryMap returned NULL pointer

**/
EFI_STATUS
EFIAPI
AppendCacheableNxmRange (
  IN UINT64    NxmLimitAddr
  )
{
  UINT32                          NxmBase;
  UINT32                          NxmLimit;
  UINT8                           SocketIndex;
  UINT8                           SadIndex;
  SAD_TABLE                       *SadTable;
  UINT8                           *SadIntListEntry;
  SAD_DATA                        SadData;
  UINT8                           SadIndexData[MAX_SOCKET][MAX_DRAM_CLUSTERS];
  SYSTEM_MEMORY_MAP_HOB           *SystemMemoryMap;
  UINT8                           ClusterId;
  UINT8                           ClusterSadBegin;
  UINT8                           ClusterSadEnd;
  UINT8                           DramRegIndex;
  EFI_STATUS                      Status;

  //
  // MTRR alignment support is not implemented in 14nm cache init code.
  //
  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    return EFI_UNSUPPORTED;
  }

  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((EFI_D_INFO, "*******Start NXM range mapping for MTRR*******\n"));

  //
  // Check if NxmLimitAddr is aligned to a 64MB boundary
  //
  if ((NxmLimitAddr & (SIZE_64MB - 1)) != 0) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Initialize local NxmLimit (in 64MB unit) from NxmLimitAddr (64-bit byte address)
  //
  NxmLimit = (UINT32) RShiftU64 (NxmLimitAddr, CONVERT_B_TO_64MB);

  //
  // Check NxmLimitAddr boundary conditions
  //
  if (NxmLimit > SystemMemoryMap->MmiohBase) {
    return EFI_INVALID_PARAMETER;
  }

  if (SystemMemoryMap->BlockDecoderData.BlockSocketEnable != 0 && NxmLimit > SystemMemoryMap->BlockDecoderData.BlockDecoderBase) {
    return EFI_INVALID_PARAMETER;
  }

  NxmBase = 0;
  ZeroMem (SadIndexData, sizeof (SadIndexData));

  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
    //
    // If socket table entry is disabled, skip it
    //
    if (SystemMemoryMap->Socket[SocketIndex].SocketEnabled == 0) {
      continue;
    }

    SadTable = SystemMemoryMap->Socket[SocketIndex].SAD;

    for (ClusterId = 0; ClusterId < SystemMemoryMap->NumberofChaDramClusters; ClusterId++) {
      ClusterSadBegin = ClusterId * SystemMemoryMap->MaxSadRules;
      ClusterSadEnd = ClusterSadBegin + SystemMemoryMap->MaxSadRules;

      for (SadIndex = ClusterSadBegin; SadIndex < ClusterSadEnd; SadIndex++) {
        //
        // If SAD table entry is disabled, store socket's SadIndex per cluster and break
        //
        if (SadTable[SadIndex].Enable == 0) {
          SadIndexData[SocketIndex][ClusterId] = SadIndex;
          break;
        }
      }// SadIndex Loop

       //
       // Check if SadIndex is out of bounds
       //
      if (SadIndex >= ClusterSadEnd) {
        return EFI_OUT_OF_RESOURCES;
      }

      //
      // Store maximum Sad Limit across the socket
      //
      if ((SadIndex > 0) && (NxmBase < SadTable[SadIndex - 1].Limit)) {
        NxmBase = SadTable[SadIndex - 1].Limit;
      }

    }// ClusterId Loop
  }// Socket Loop

  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
    //
    // If socket table entry is disabled, skip it
    //
    if (SystemMemoryMap->Socket[SocketIndex].SocketEnabled == 0) {
      continue;
    }

    SadTable = SystemMemoryMap->Socket[SocketIndex].SAD;

    Status = SetGenprotRange (SocketIndex, NxmBase, NxmLimit);
    if (Status != SUCCESS) {
      if (Status == EFI_INVALID_PARAMETER) {
        DEBUG ((EFI_D_INFO, "Invalid Input Parameter\n"));
        return EFI_INVALID_PARAMETER;
      } else if (Status == EFI_OUT_OF_RESOURCES) {
        DEBUG ((EFI_D_INFO, "Requested address range is NOT protected\n"));
        return EFI_OUT_OF_RESOURCES;
      } else if (Status == EFI_UNSUPPORTED) {
        DEBUG ((EFI_D_INFO, "API is not supported\n"));
        return EFI_UNSUPPORTED;
      }
    }

    ZeroMem (&SadData, sizeof (SadData));

    SadData.Enabled = TRUE;
    SadData.Limit = NxmLimit;
    SadData.InterleaveList = 0xFFFFFFFF;
    SadData.Nxm = TRUE;

    for (ClusterId = 0; ClusterId < SystemMemoryMap->NumberofChaDramClusters; ClusterId++) {
      //
      // Retrieve stored SadIndex for socket per cluster
      //
      SadIndex = SadIndexData[SocketIndex][ClusterId];
      SadIntListEntry = SystemMemoryMap->Socket[SocketIndex].SadIntList[SadIndex];

      //
      // Before writing multicast SNC DRAM CSR's, unlock ClusterId and lock other Clusters
      //
      DEBUG ((EFI_D_INFO, "CHA unlock DRAM cluster %d\n", ClusterId));
      ChaUnlockDramCluster (SocketIndex, ClusterId);

      //
      // Before programming CHA DRAM decoder CSR's calculate the DRAM register index w.r.t each DRAM cluster
      //
      DramRegIndex = (SadIndex % SystemMemoryMap->MaxSadRules);

      //
      // Display enabled SAD rules in the serial log
      //
      DEBUG ((EFI_D_INFO, "Program CSRs for Socket %d SAD Rule %d (NXM) \n", SocketIndex, DramRegIndex));

      DEBUG ((EFI_D_INFO, "    En  Base     Limit    Interleave List\n"));
      DEBUG ((EFI_D_INFO, "    %d  0x%05x   0x%05x   0x%08x\n",
        SadData.Enabled,
        NxmBase,
        SadData.Limit,
        SadData.InterleaveList
        ));

      ChaSetSadData (SocketIndex, DramRegIndex, &SadData);

      ZeroMem ((UINT8 *) &SadTable[SadIndex], sizeof (SadTable[SadIndex]));
      ZeroMem ((UINT8 *) &SadIntListEntry, sizeof (SadIntListEntry));

      //
      // Create new SAD table entry for the NXM for MTRR for each cluster
      //
      SadTable[SadIndex].Enable = 1;
      SadTable[SadIndex].type   = MemTypeNxm;
      SadTable[SadIndex].Base   = NxmBase;
      SadTable[SadIndex].Limit  = NxmLimit;
      SadTable[SadIndex].Cluster = ClusterId;
    } // Cluster Loop

    //
    // Ensure DRAM cluster(s) lock after write and before exit.
    //
    ChaLockDramClusters (SocketIndex);
  } // Socket Loop

  DEBUG ((EFI_D_INFO, "*******End NXM range mapping for MTRR*******\n"));
  return EFI_SUCCESS;
}

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
  )
{
  MEMORY_MAP_HOST                 *MemMapHost;
  PSYSHOST                        Host;
  SYSTEM_MEMORY_MAP_HOB           *SystemMemoryMap;
  UINT32                          SadBase;
  UINT32                          SadSize;
  UINT8                           Sad;
  UINT8                           Socket;
  UINT8                           Index;
  UINT8                           Mc;
  UINT8                           Ch;
  UINT8                           Dimm;
  UINT8                           Rank;
  UINT16                          IntDimm;
  UINT16                          MemInfoMemFreq;
  UINT8                           MemInfoDdrVoltage;
  MEM_VAR_STRUCT                  *Mem;
  UINT32                          MemInfoTotalMem;
  UINT8                           GuidIdx;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *PeiVariable;
  UINTN                           VariableSize;
  EFI_STATUS                      Status;
  BOOLEAN                         DimmEnabled;
  KTI_HOST_OUT                    *KtiHostOutPtr;
  EFI_PEI_SERVICES                **PeiServices;
  EFI_PHYSICAL_ADDRESS            MemMapDataAddr;
  UINT8                           Node;
  UINT8                           RasMode;
  SYS_SETUP                       *Setup;
  DDRT_DIMM_DEVICE_INFO_STRUCT    *DdrtDimmInfo;
  UINT8                           TempString[4];
  UINT8                           ClusterId;
  UINT8                           ClusterSadBegin;
  UINT8                           ClusterSadEnd;
  UINT8                           LogTypeIndex;
  UINT8                           LogLevelIndex;
  UINT8                           MaxImc;
  VOID                            *MemMapHob = NULL;
  UINT64                          NxmLimitAddr;

  Setup = GetSysSetupPointer ();

  Host = GetSysHostPointer ();
  MemMapHost = GetMemMapHostPointer ();
  MaxImc = GetMaxImc ();
  RasMode = GetRasMode ();
  KtiHostOutPtr = KTI_HOST_OUT_PTR;
  PeiServices = (EFI_PEI_SERVICES **) (UINTN) Host->var.common.oemVariable;

  SystemMemoryMap = AllocatePages (EFI_SIZE_TO_PAGES (sizeof (SYSTEM_MEMORY_MAP_HOB)));
  if (SystemMemoryMap == NULL) {
    DEBUG ((EFI_D_ERROR, "Failed to allocate SYSTEM_MEMORY_MAP_HOB\n"));
    return;
  }
  ZeroMem ((UINT8 *) SystemMemoryMap, sizeof (SYSTEM_MEMORY_MAP_HOB));

  MemInfoMemFreq = 0;
  MemInfoDdrVoltage = 0;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent (Socket) == FALSE) {
      continue;
    }
    if ((CheckSocketEnabled (Socket) == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
      continue;
    }

    MemInfoMemFreq = Host->nvram.mem.socket[Socket].ddrFreqMHz*2;
    MemInfoDdrVoltage = Host->nvram.mem.socket[Socket].ddrVoltage;

    break;

  } // node loop

  MemInfoTotalMem = (Host->var.mem.memSize << 6) ;  // Memory size in 1MB units

  Mem = &(Host->var.mem);
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

    for (ClusterId = 0; ClusterId < MemMapHost->NumberofChaDramClusters; ClusterId++) {

      ClusterSadBegin = (ClusterId * MAX_SAD_RULES);
      ClusterSadEnd = (ClusterSadBegin + MAX_SAD_RULES);
      if (Mem->socket[Socket].SAD[ClusterSadBegin].Enable == 0) {
        break;
      }
      for (Sad = ClusterSadBegin; Sad < ClusterSadEnd; Sad++) {
        //
        // If SAD table entry is active, populate information into Memory Map HOB.
        // Ensure that IOT is not reported to SMBIOS, SRAT, other tables.
        //
        if (!Mem->socket[Socket].SAD[Sad].Enable ||
          !Mem->socket[Socket].SAD[Sad].local ||
          Mem->socket[Socket].SAD[Sad].IotEnabled) {
          continue;
        }
        if (Mem->socket[Socket].SAD[Sad].type == MemType1lmCtrl ||
          Mem->socket[Socket].SAD[Sad].type == MemType1lmAppDirectReserved) {
          //
          // We are interested in memory only, skip DCPMM ctrl registers region and PMEM alignment gap.
          //
          continue;
        }
        SadBase = Mem->socket[Socket].SAD[Sad].Base;
        SadSize = Mem->socket[Socket].SAD[Sad].Limit - SadBase;
        if (Sad == 0 && Socket == 0) {
          SadSize -= KTI_HOST_OUT_PTR->lowGap; // Adjust for MMIO-L
        }
        //
        // Verify whether this SAD was already reported from other socket in socket interleaving.
        //
        for (Index = 0; Index < SystemMemoryMap->numberEntries; Index++) {

          if (SystemMemoryMap->Element[Index].BaseAddress == SadBase) {

            SystemMemoryMap->Element[Index].SktInterBitmap |= (1 << Socket); // this is not new element, just add this socket to bitmap
            break;
          }
        }
        if (Index == SystemMemoryMap->numberEntries) {
          //
          // It is new memory range element, add it to elements table.
          //
          SystemMemoryMap->numberEntries++;
          SystemMemoryMap->Element[Index].Type = Mem->socket[Socket].SAD[Sad].type;
          for (Node = 0; Node < MC_MAX_NODE; Node++) {
            if (MemMapHost->SocketInfo[Socket].SadIntList[Sad][Node] == 1) {
              SystemMemoryMap->Element[Index].NodeId = Node;
              break;
            }
          }
          SystemMemoryMap->Element[Index].SocketId = Socket;
          SystemMemoryMap->Element[Index].SktInterBitmap |= (1 << Socket);
          SystemMemoryMap->Element[Index].ImcInterBitmap = Mem->socket[Socket].SAD[Sad].imcInterBitmap;
#if MemTypeLibConstructorFixed // TODO: Enable this section and remove the other after fixing MemTypeLib
          CopyMem (&SystemMemoryMap->Element[Index].ChInterBitmap[0],
            IsMemType1lm (Mem->socket[Socket].SAD[Sad].type) ? &Mem->socket[Socket].SAD[Sad].channelInterBitmap[0] :
            &Mem->socket[Socket].SAD[Sad].FMchannelInterBitmap[0],
            sizeof (SystemMemoryMap->Element[Index].ChInterBitmap));
#else
          CopyMem (&SystemMemoryMap->Element[Index].ChInterBitmap[0],
            (Mem->socket[Socket].SAD[Sad].type == MemType1lmDdr || Mem->socket[Socket].SAD[Sad].type == MemType1lmDdr) ?
            &Mem->socket[Socket].SAD[Sad].channelInterBitmap[0] :
            &Mem->socket[Socket].SAD[Sad].FMchannelInterBitmap[0],
            sizeof (SystemMemoryMap->Element[Index].ChInterBitmap));
#endif
          SystemMemoryMap->Element[Index].BaseAddress = SadBase;
          SystemMemoryMap->Element[Index].ElementSize = SadSize;
          DEBUG ((DEBUG_INFO, "[MemElement] SocketId:%d NodeId:%d SktInterBitmap:%02X ImcInterBitmap:%02X BaseAddress:%08x ElementSize:%08x Type:%d\n",
            SystemMemoryMap->Element[Index].SocketId, SystemMemoryMap->Element[Index].NodeId, SystemMemoryMap->Element[Index].SktInterBitmap |= (1 << Socket),
            SystemMemoryMap->Element[Index].ImcInterBitmap, SystemMemoryMap->Element[Index].BaseAddress, SystemMemoryMap->Element[Index].ElementSize,
            SystemMemoryMap->Element[Index].Type));
        }
      }
    }
    //
    // Copy SAD information to HOB.
    // NOTE: In Purley the SAD & SadIntList is in Socket loop of the HOB.
    //
    CopyMem ((VOID *) (&SystemMemoryMap->Socket[Socket].SadIntList), (VOID *) (&(MemMapHost->SocketInfo[Socket].SadIntList)), sizeof (MemMapHost->SocketInfo[Socket].SadIntList));
    CopyMem ((VOID *) (&SystemMemoryMap->Socket[Socket].SAD), (VOID *) (&(Host->var.mem.socket[Socket].SAD)), sizeof (Host->var.mem.socket[Socket].SAD));
    //
    // copy SKT SKU info
    //
    SystemMemoryMap->Socket[Socket].SktSkuValid = Host->var.mem.socket[Socket].SktSkuValid;
    SystemMemoryMap->Socket[Socket].SktSkuLimit = Host->var.mem.socket[Socket].SktSkuLimit;
    SystemMemoryMap->Socket[Socket].SktTotMemMapSPA = Host->var.mem.socket[Socket].SktTotMemMapSPA;
    SystemMemoryMap->Socket[Socket].SktMemSize2LM = Host->var.mem.socket[Socket].SktMemSize2LM;

    if (IsHbmSku ()) {
      for (Index = 0; Index < GetMaxIoHbm (); Index++) {
        SystemMemoryMap->Socket[Socket].HbmIoEn[Index] = IsHbmMemSsEnabled (Socket, Index);
      }
    }
  }

  // run loop for 14 - remote dram loop limit
  if (MemMapHost->FpgaMemCnt) {
    UINT8 Rindex;
    for (Rindex = 0; Rindex < MemMapHost->FpgaMemCnt; Rindex++) {
      if (MemMapHost->FpgaRsadRules[Rindex].Attr != SAD_FPGA_ATTR) {
        continue;
      }
      // Adding the Element in Hob
      Index++;
      SystemMemoryMap->Element[Index].BaseAddress = MemMapHost->FpgaRsadRules[Rindex].Base;
      SystemMemoryMap->Element[Index].ElementSize = MemMapHost->FpgaRsadRules[Rindex].Limit - MemMapHost->FpgaRsadRules[Rindex].Base;
      SystemMemoryMap->Element[Index].SocketId = MemMapHost->FpgaRsadRules[Rindex].NodeId;
      SystemMemoryMap->Element[Index].SktInterBitmap |= (1 << MemMapHost->FpgaRsadRules[Rindex].NodeId);
      SystemMemoryMap->Element[Index].Type = MemTypeFpga;
      SystemMemoryMap->numberEntries++;
    }
	//mem-copy
  }

  //
  // Copy system level block decoder data structure from host
  //
  CopyMem (&SystemMemoryMap->BlockDecoderData, &MemMapHost->BlockDecoderData, sizeof (MemMapHost->BlockDecoderData));

  SystemMemoryMap->lowMemBase = Host->var.mem.lowMemBase;
  SystemMemoryMap->lowMemSize = Host->var.mem.lowMemSize;
  SystemMemoryMap->highMemBase = Host->var.mem.highMemBase;
  SystemMemoryMap->highMemSize = Host->var.mem.highMemSize;
  SystemMemoryMap->memSize = Host->var.mem.memSize;
  // Do we have at least one Aep Dimm Installed?
  SystemMemoryMap->DcpmmPresent = Host->nvram.mem.DcpmmPresent;
  SystemMemoryMap->EkvPresent = Host->var.mem.EkvPresent;
  SystemMemoryMap->BwvPresent = Host->var.mem.BwvPresent;
  SystemMemoryMap->volMemMode = Host->var.mem.volMemMode;
  SystemMemoryMap->CacheMemType = Host->var.mem.CacheMemType;
  SystemMemoryMap->LatchSystemShutdownState = Setup->mem.LatchSystemShutdownState;
  SystemMemoryMap->memFreq = MemInfoMemFreq;
  SystemMemoryMap->DdrVoltage = MemInfoDdrVoltage;
  SystemMemoryMap->IsWpqFlushSupported = MemMapHost->IsWpqFlushSupported;
  SystemMemoryMap->EadrSupport = Host->var.mem.EadrSupport;
  SystemMemoryMap->EadrCacheFlushMode = Host->var.mem.EadrCacheFlushMode;
  SystemMemoryMap->DdrtIntlvGranularity = MemMapHost->DdrtIntlvGranularity;
  SystemMemoryMap->MmiohBase = Host->var.mem.mmiohBase;
  SystemMemoryMap->MaxSadRules = MemMapHost->MaxSadRules;
  SystemMemoryMap->NumberofChaDramClusters = MemMapHost->NumberofChaDramClusters;

  //
  // Store maximum value of Average Power limit, Mbb Power Limit, Mbb Average Power Time Constant
  // obtained from Host
  //
  SystemMemoryMap->MaxAveragePowerLimit = Host->var.mem.MaxAveragePowerLimit;
  SystemMemoryMap->MaxMbbPowerLimit = Host->var.mem.MaxMbbPowerLimit;
  SystemMemoryMap->MaxMbbAveragePowerTimeConstant = Host->var.mem.MaxMbbAveragePowerTimeConstant;

  //
  // Save FIS revision currently used by BIOS
  //
  SystemMemoryMap->BiosFisVersion = Host->var.mem.BiosFisRevision;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!SocketPresent (Socket)) {
      SystemMemoryMap->Socket[Socket].SocketEnabled = 0;
      SystemMemoryMap->Socket[Socket].IioStackBitmap = 0;
    } else {
      SystemMemoryMap->Socket[Socket].SocketEnabled = 1;
      SystemMemoryMap->Socket[Socket].IioStackBitmap = GetStackPresentBitMap (Socket);
    }
    for (Index = 0; Index < MAX_SMB_INSTANCE; Index++) {
      SystemMemoryMap->SmbMode[Socket][Index] = Host->var.mem.SmbMode[Socket][Index];
    }
  }

  SystemMemoryMap->memMode = 0;
  if (RasMode & STAT_VIRT_LOCKSTEP) {
    SystemMemoryMap->memMode = 1;
  }
  if (RasMode & CH_ALL_MIRROR) {
    SystemMemoryMap->memMode = 2;
  }

  if (PcdGetBool (PcdMrcXmpSupport)) {
    SystemMemoryMap->XMPProfilesSup = Host->nvram.mem.XMPProfilesSup;
  }

  SystemMemoryMap->DramType = Host->nvram.mem.dramType;
  SystemMemoryMap->NumChPerMC = GetNumChannelPerMc ();
  SystemMemoryMap->SystemRasType = KtiHostOutPtr->SystemRasType;
  SystemMemoryMap->RasModesEnabled = RasMode;
  SystemMemoryMap->ExRasModesEnabled = Host->nvram.mem.RASmodeEx;
  SystemMemoryMap->sncEnabled = IfFeatureEnabled (TypeSnc);
  SystemMemoryMap->NumOfCluster = GetNumOfClusterPerSystem ();

  SystemMemoryMap->bssaNumberDistinctGuids = Host->var.mem.bssaNumberDistinctGuids;
  DEBUG ((DEBUG_INFO, "BSSA: Disctinct result GUIDs: %d\n",SystemMemoryMap->bssaNumberDistinctGuids));
  SystemMemoryMap->bssaBdatSize = Host->var.mem.bssaBdatSize;
  DEBUG ((DEBUG_INFO, "BSSA: Total BDAT size: %d\n",SystemMemoryMap->bssaBdatSize));
  for (GuidIdx = 0; GuidIdx < MAX_NUMBER_SSA_BIOS_RESULTS; GuidIdx++) {
    SystemMemoryMap->bssaBdatGuid[GuidIdx] = Host->var.mem.bssaBdatGuid[GuidIdx];
    SystemMemoryMap->bssaNumberHobs[GuidIdx] = Host->var.mem.bssaNumberHobs[GuidIdx];
    DEBUG ((DEBUG_INFO, "BSSA: GuidIdx %d has %d HOBs\n", GuidIdx, SystemMemoryMap->bssaNumberHobs[GuidIdx]));
  }

  if (PcdGetBool (PcdMrcXmpSupport)) {

    UINT8 profile;

    for (profile = 0; profile < 2; profile++) {

      if (Host->nvram.mem.XMPProfilesSup & (1 << profile)) {
        SystemMemoryMap->profileMemTime[profile] = Host->nvram.mem.profileMemTime[profile];
      }
    }
  }

  // Now fill Channel/DIMM info/TAD info
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Mc = 0; Mc < MaxImc; Mc++) {
      SystemMemoryMap->Socket[Socket].imcEnabled[Mc] = Host->var.mem.socket[Socket].imcEnabled[Mc];
      SystemMemoryMap->Socket[Socket].imc[Mc].MemSize = Host->var.mem.socket[Socket].imc[Mc].memSize;
    }
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].Enabled = Host->nvram.mem.socket[Socket].channelList[Ch].enabled;
      SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].Features = Host->nvram.mem.socket[Socket].channelList[Ch].features;
      SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].MaxDimm = Host->nvram.mem.socket[Socket].channelList[Ch].maxDimm;
      SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].NumRanks = Host->nvram.mem.socket[Socket].channelList[Ch].numRanks;
      SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].chFailed = Host->nvram.mem.socket[Socket].channelList[Ch].chFailed;
      SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].ngnChFailed = Host->nvram.mem.socket[Socket].channelList[Ch].ngnChFailed;
      for (Rank = 0; Rank < MAX_SPARE_RANK; Rank++) {
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].SpareLogicalRank[Rank] = Host->nvram.mem.socket[Socket].channelList[Ch].spareLogicalRank[Rank];
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].SpareRankSize[Rank] = Host->nvram.mem.socket[Socket].channelList[Ch].spareRankSize[Rank];
      }
      SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].EnabledLogicalRanks = 0;
      for (LogTypeIndex = 0; LogTypeIndex < NGN_LOG_TYPE_NUM; LogTypeIndex++) {
        for (LogLevelIndex = 0; LogLevelIndex < NGN_LOG_LEVEL_NUM; LogLevelIndex++) {
          SystemMemoryMap->HostDdrtDimmInfo[Socket][Ch].NgnMaxLogEntries[LogTypeIndex][LogLevelIndex] = Host->var.mem.socket[Socket].channelList[Ch].DdrtDimmList[0].NgnMaxLogEntries[LogTypeIndex][LogLevelIndex];
          SystemMemoryMap->HostDdrtDimmInfo[Socket][Ch].NgnLogSeqNum[LogTypeIndex][LogLevelIndex] = Host->var.mem.socket[Socket].channelList[Ch].DdrtDimmList[0].NgnLogSeqNum[LogTypeIndex][LogLevelIndex];
        }
      }
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].Present = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].dimmPresent;
        DimmEnabled = FALSE;
        if (Host->nvram.mem.socket[Socket].channelList[Ch].enabled &&
            Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].dimmPresent) {

          for (Rank = 0;  Rank < Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].numDramRanks; Rank++) {
            if (Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].mapOut[Rank] == 0) {
              DimmEnabled = TRUE;
            }
          }
        }
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].Enabled = DimmEnabled;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].DcpmmPresent = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].DcpmmPresent;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].VendorID = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].VendorID;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].DeviceID = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].DeviceID;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].RevisionID = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].RevisionID;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].InterfaceFormatCode = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].InterfaceFormatCode;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].SubsystemVendorID = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].SubsystemVendorID;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].SubsystemDeviceID = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].SubsystemDeviceID;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].SubsystemRevisionID = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].SubsystemRevisionID;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].FisVersion = Host->var.mem.socket[Socket].channelList[Ch].FisVersion[Dimm];
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].DimmSku = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].dimmSku;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].manufacturingLocation = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].SPDMMfLoc;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].manufacturingDate = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].SPDModDate;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].perRegionDPA = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].perRegionDPA;

        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].NvDimmNPresent = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].NvDimmNPresent;

        DdrtDimmInfo = GetDdrtDimmInfo (Socket, Ch, Dimm);

        if (DdrtDimmInfo->NgnCfgCurPresent && DdrtDimmInfo->NgnCfgCurValid) {
          for (Index = 0; Index < MAX_UNIQUE_NGN_DIMM_INTERLEAVE; Index++) {
            for (IntDimm = 0; IntDimm < DdrtDimmInfo->NgnCfgCur.interleave[Index].NumOfDimmsInInterleaveSet; IntDimm++) {
              if ((DdrtDimmInfo->NgnCfgCur.interleaveId[Index][IntDimm].Socket == Socket) &&
                  (DdrtDimmInfo->NgnCfgCur.interleaveId[Index][IntDimm].Channel == Ch) &&
                  (DdrtDimmInfo->NgnCfgCur.interleaveId[Index][IntDimm].Dimm == Dimm)) {

                SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].perDPAMap[Index].perRegionDPAOffset = DdrtDimmInfo->NgnCfgCur.interleaveId[Index][IntDimm].PartitionOffset;

                DEBUG ((DEBUG_INFO, "\nsoc:%d; Ch:%d; Dimm:%d; Interleave:%d; IntDimm:%d; perRegionDPAOffset: 0x%08X ",
                  Socket, Ch, Dimm, Index, IntDimm, SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].perDPAMap[Index].perRegionDPAOffset));

                break;
              }
            }
            SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].perDPAMap[Index].SPALimit = DdrtDimmInfo->NgnCfgCur.interleave[Index].SadLimit;

            DEBUG ((DEBUG_INFO, "\nsoc:%d; Ch:%d; Dimm:%d; Interleave:%d; IntDimm:%d; SPALimit: 0x%08X ",
              Socket, Ch, Dimm, Index, IntDimm, SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].perDPAMap[Index].SPALimit));
          }
          DEBUG ((DEBUG_INFO, "\n"));
        }
        // If running on a Simics platform with enabled DCPMM DIMM, then we want to get the serial number and manufacturer from the IdentifyDIMM command.  This is because it is easy to override these values in Simcis without
        // having to provide a new SPD image
#ifdef DDRT_SUPPORT
        if ((IsSimicsEnvironment () == TRUE) && (Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].DcpmmPresent) && DimmEnabled)  {
          DEBUG ((DEBUG_INFO, "SerialNumber: "));
          for (Index = 0; Index < NGN_MAX_SERIALNUMBER_STRLEN; Index++)  {
            SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].serialNumber[Index] = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].serialNumber[Index];
            DEBUG ((DEBUG_INFO, "%02X", Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].serialNumber[Index] ));
          }
          DEBUG ((DEBUG_INFO, "\n"));
          DEBUG ((DEBUG_INFO, "PartNumber: "));
          for (Index = 0; Index < NGN_MAX_PARTNUMBER_STRLEN; Index++)  {
            SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].PartNumber[Index] = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].PartNumber[Index];
            DEBUG ((DEBUG_INFO, "%02X", Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].PartNumber[Index] ));
          }
          DEBUG ((DEBUG_INFO, "\n"));
        }
#endif // DDRT_SUPPORT

        // Otherwise the serial number and manufacturer should be obtained from the SPD
        else  {
          // Update SerialNumber
          DEBUG ((DEBUG_INFO, "SPD SerialNumber: "));
          for (Index = 0; Index < SPD_MODULE_SERIAL; Index++) {
            SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].serialNumber[Index] =
              Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].SPDModSN[Index];
            DEBUG ((DEBUG_INFO, "%02X", SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].serialNumber[Index]));
          }
          DEBUG ((DEBUG_INFO, "\n"));
          // Update PartNumber
          DEBUG ((DEBUG_INFO, "SPD PartNumber: "));
#ifdef DDR5_SUPPORT
          if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
            for (Index = 0; Index < SPD_MODULE_PART_DDR5; Index++) {
              SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].PartNumber[Index] =
                Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].SPDModPartDDR5[Index];
              DEBUG ((DEBUG_INFO, "%02X", SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].PartNumber[Index]));
            }
            DEBUG ((DEBUG_INFO, "\n"));
          } else
#endif // DDR5_SUPPORT
          {
            for (Index = 0; Index < SPD_MODULE_PART_DDR4; Index++) {
              SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].PartNumber[Index] =
                Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].SPDModPartDDR4[Index];
              DEBUG ((DEBUG_INFO, "%02X", SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].PartNumber[Index]));
            }
            DEBUG ((DEBUG_INFO, "\n"));
          }
        }

        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].FirmwareVersion.majorVersion = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].firmwareRevision.majorVersion;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].FirmwareVersion.minorVersion = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].firmwareRevision.minorVersion;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].FirmwareVersion.hotfixVersion = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].firmwareRevision.hotfixVersion;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].FirmwareVersion.buildVersion = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].firmwareRevision.buildVersion;

        TempString[0] = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].firmwareRevision.buildVersion  >> 12;
        TempString[1] = (Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].firmwareRevision.buildVersion & 0x0F00) >> 8;
        TempString[2] = (Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].firmwareRevision.buildVersion & 0x00F0) >> 4;
        TempString[3] = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].firmwareRevision.buildVersion  & 0x000F;
        for (Index = 0; Index < 4; Index++) {
          if (TempString[Index] < 0x0a) {
            TempString[Index] += 0x30;
          } else {
            TempString[Index] += 0x36;
          }
          SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].FirmwareVersionStr[Index] = TempString[Index];
        }
        DEBUG ((DEBUG_INFO, "Manufacturer: "));
        for (Index = 0; Index < NGN_MAX_MANUFACTURER_STRLEN; Index++)  {
          DEBUG ((DEBUG_INFO, "%02X", Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].manufacturer[Index] ));
        }
        DEBUG ((DEBUG_INFO, "\n"));

        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].NumRanks = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].numRanks;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].X4Present = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].x4Present;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].SPDMMfgId = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].SPDMMfgId;
        if (Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].DcpmmPresent) {
          SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].DimmSize = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].rawCap;
        } else{
          //
          // Total DIMM size includes mapped and unmapped memory (disabled ranks are not included)
          //
          SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].DimmSize =
              (Host->var.mem.socket[Socket].channelList[Ch].dimmList[Dimm].memSize + Host->var.mem.socket[Socket].channelList[Ch].dimmList[Dimm].UnmappedMemSize);
        }
        if (PcdGetBool (PcdNvDimmEn)) {
          if (Setup->mem.ADREn && Setup->mem.ADRDataSaveMode == ADR_NVDIMM) {
            SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].NVmemSize = Host->var.mem.socket[Socket].channelList[Ch].dimmList[Dimm].NVmemSize;
          } else {
            SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].NVmemSize = 0;
          }
        } else {
          SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].NVmemSize = 0;
        }
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].actSPDModuleOrg = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].actSPDModuleOrg;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].keyByte = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].keyByte;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].actKeyByte2 = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].actKeyByte2;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].nonVolCap = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].nonVolCap;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].VolCap = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].volCap;
        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].dimmTs = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].dimmTs;

        for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
          if (Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].rankList[Rank].enabled == 0) {
            continue;
          }
          if (Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].mapOut[Rank]) {
            continue;
          }
          SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].EnabledLogicalRanks |=
            1 <<  Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].rankList[Rank].logicalRank;

          SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].rankList[Rank].enabled =
            Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].rankList[Rank].enabled;

          SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].rankList[Rank].logicalRank =
            Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].rankList[Rank].logicalRank;

          SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].rankList[Rank].rankSize =
            Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].rankList[Rank].rankSize;
        } // Rank loop
        //fill Dimm minTCK, serial number and part number for smbios type17

        SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].commonTck = Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].minTCK;
        SystemMemoryMap->HostDdrtDimmInfo[Socket][Ch].NvdimmLinkFailOnPrevBoot = Host->var.mem.socket[Socket].channelList[Ch].DdrtDimmList[0].NvdimmLinkFailOnPrevBoot;
        SystemMemoryMap->HostDdrtDimmInfo[Socket][Ch].NvdimmMediaErrLogged = Host->var.mem.socket[Socket].channelList[Ch].DdrtDimmList[0].NvdimmMediaErrLogged;
        SystemMemoryMap->HostDdrtDimmInfo[Socket][Ch].NvdimmTempErrLogged = Host->var.mem.socket[Socket].channelList[Ch].DdrtDimmList[0].NvdimmTempErrLogged;
        SystemMemoryMap->HostDdrtDimmInfo[Socket][Ch].NvdimmUnmapped = Host->var.mem.socket[Socket].channelList[Ch].DdrtDimmList[0].NvdimmUnmapped;
        SystemMemoryMap->HostDdrtDimmInfo[Socket][Ch].NvdimmUnmappedReason = Host->var.mem.socket[Socket].channelList[Ch].DdrtDimmList[0].NvdimmUnmappedReason;
        SystemMemoryMap->HostDdrtDimmInfo[Socket][Ch].Reserved = Host->var.mem.socket[Socket].channelList[Ch].DdrtDimmList[0].Reserved;
        SystemMemoryMap->HostDdrtDimmInfo[Socket][Ch].NgnBsr = Host->var.mem.socket[Socket].channelList[Ch].DdrtDimmList[0].NgnBsr;
      } // Dimm loop

      //Copy Address Range Partial Mirroring Information
      Status = (*PeiServices)->LocatePpi (
                                 PeiServices,
                                 &gEfiPeiReadOnlyVariable2PpiGuid,
                                 0,
                                 NULL,
                                 &PeiVariable
                                 );
      if (Status == EFI_SUCCESS) {
        VariableSize = sizeof (SystemMemoryMap->RasMeminfo.MirrorRequestType);
        Status = PeiVariable->GetVariable (
                                PeiVariable,
                                ADDRESS_RANGE_MIRROR_VARIABLE_REQUEST,
                                &gAddressBasedMirrorGuid,
                                NULL,
                                &VariableSize,
                                &SystemMemoryMap->RasMeminfo.MirrorRequestType
                                );
        if (Status != EFI_SUCCESS) {
          ZeroMem ((UINT8 *) &SystemMemoryMap->RasMeminfo.MirrorRequestType, VariableSize);
        }
        SystemMemoryMap->RasMeminfo.MirrorCurrentType.MirrorVersion = ADDRESS_RANGE_MIRROR_VARIABLE_VERSION;
        if (Setup->mem.partialMirrorUEFI == 1) {
          SystemMemoryMap->RasMeminfo.MirrorCurrentType.MirrorMemoryBelow4GB = Setup->mem.partialmirrorsad0;
          SystemMemoryMap->RasMeminfo.MirrorCurrentType.MirroredAmountAbove4GB = (UINT16) Setup->mem.partialmirrorpercent;
        } else {
          SystemMemoryMap->RasMeminfo.MirrorCurrentType.MirrorMemoryBelow4GB = 0;
          SystemMemoryMap->RasMeminfo.MirrorCurrentType.MirroredAmountAbove4GB = 0;
        }
        SystemMemoryMap->RasMeminfo.MirrorCurrentType.MirrorStatus = Setup->mem.partialmirrorsts;
      }

    } // Ch loop

  } // Socket loop
  CopyMem ((UINT8 *)&(SystemMemoryMap->MemSetup), (UINT8 *)&Setup->mem, sizeof (struct memSetup));
  CopyMem (&SystemMemoryMap->DfxMemVar, &Host->var.mem.dfxMemVars, sizeof (SystemMemoryMap->DfxMemVar));

  //
  // Copy default SecureErase Knob Settings to System Memory HOB
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      SystemMemoryMap->SetSecureEraseSktChHob[Socket][Ch] = Host->var.mem.SetSecureEraseSktChHob[Socket][Ch];
      DEBUG ((EFI_D_INFO, "HOB SystemMemoryMap->SetSecureEraseSktChHob[%d][%d] = %d\n", Socket, Ch, SystemMemoryMap->SetSecureEraseSktChHob[Socket][Ch]));

      //
      // Fill DdrCacheSize for Each Channel
      //
      SystemMemoryMap->DdrCacheSize[Socket][Ch] = Host->var.mem.socket[Socket].channelList[Ch].DdrCacheSize;

      //
      // Fill ADR status for Each Channel(Intel DCPMM populated channels)
      //
#ifdef DDRT_SUPPORT
      SystemMemoryMap->AdrStateForPmemModule[Socket][Ch] = Host->var.mem.AdrStateForPmemModule[Socket][Ch];
#else
      SystemMemoryMap->AdrStateForPmemModule[Socket][Ch] = 0;
#endif
    }
  }

  //
  // Build HOB for system memory map
  //
  MemMapDataAddr = (EFI_PHYSICAL_ADDRESS) (UINTN) SystemMemoryMap;
  MemMapHob = BuildGuidDataHob (
                &gEfiMemoryMapGuid,
                &MemMapDataAddr,
                sizeof (EFI_PHYSICAL_ADDRESS)
                );
  if (MemMapHob == NULL) {
    ASSERT (FALSE);
    DEBUG ((EFI_D_ERROR, "Failed to build HOB for system memory map"));
    return;
  }

  //
  // Determine NxmLimitAddr to map the NXM rule
  //
  if (SystemMemoryMap->BlockDecoderData.BlockSocketEnable == 0) {
    NxmLimitAddr = LShiftU64 (SystemMemoryMap->MmiohBase, CONVERT_64MB_TO_B);
  } else {
    NxmLimitAddr = LShiftU64 (SystemMemoryMap->BlockDecoderData.BlockDecoderBase, CONVERT_64MB_TO_B);
  }

  Status = AppendCacheableNxmRange (NxmLimitAddr);
  if (Status != SUCCESS) {
    if (Status == EFI_INVALID_PARAMETER) {
      DEBUG ((EFI_D_INFO, "Invalid Input Parameter to map NXM for MTRR\n"));
    } else if (Status == EFI_OUT_OF_RESOURCES) {
      DEBUG ((EFI_D_INFO, "Out of resources to map NXM for MTRR\n"));
    } else if (Status == EFI_UNSUPPORTED) {
      DEBUG ((EFI_D_INFO, "Platform does not support mapping NXM for MTRR\n"));
    } else if (Status == EFI_DEVICE_ERROR) {
      DEBUG ((EFI_D_INFO, "SystemMemoryMap returned NULL pointer\n"));
    }
  }

  return;
}

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
  )
{
  EFI_HOB_GUID_TYPE             *GuidHob;
  EFI_PHYSICAL_ADDRESS          *MemMapDataAddr;

  GuidHob = GetFirstGuidHob (&gEfiMemoryMapGuid);
  if (GuidHob == NULL) {
    return NULL;
  }

  MemMapDataAddr = GET_GUID_HOB_DATA (GuidHob);
  return (SYSTEM_MEMORY_MAP_HOB *) (UINTN) (*MemMapDataAddr);
}
