/** @file
  ACPI Platform Driver Hooks

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2020 Intel Corporation. <BR>

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
// Statements that include other files
//
#include "AcpiPlatformLibLocal.h"
#include <Library/KtiApi.h>

extern SYSTEM_MEMORY_MAP_HOB        *mSystemMemoryMap;
extern EFI_IIO_UDS_PROTOCOL         *mIioUds;
extern EFI_ACPI_TABLE_PROTOCOL      *mAcpiTable;
extern CPU_CSR_ACCESS_VAR           *mCpuCsrAccessVarPtr;

EFI_ACPI_SYSTEM_LOCALITY_INFORMATION_TABL_PMEM_INFO mSlitPmemInfo[EFI_ACPI_SLIT_PMEM_INFO_CNT];


/**
  Displays System Locality Distance Information Table (SLIT)

  @param None

  @retval None
**/
VOID
DisplayEntries (
  IN ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *SlitPtr
  )
{
  UINT16                                     EntryIdx = 0;
  UINT16                                     MaxTableEntries;
  UINT8                                      NodeCount;

  if (SlitPtr == NULL) {

    ASSERT (SlitPtr != NULL);
    return;
  }
  NodeCount = (UINT8)SlitPtr->Header.NumberOfSystemLocalities;
  MaxTableEntries = NodeCount * NodeCount;

  DEBUG ((DEBUG_INFO, "SLIT: Dump table (size %d):\n", NodeCount));

  while (EntryIdx < MaxTableEntries) {

    DEBUG ((DEBUG_INFO, "%02X ", SlitPtr->NumSlit[EntryIdx].Entry));
    if ((EntryIdx % NodeCount) == (NodeCount - 1)) {
      DEBUG ((DEBUG_INFO, "\n"));
    }
    EntryIdx++;
  }
}


/**

  Stores correlation between AEP PMEM NUMA node and socket

  @param[in] Pmem   AEP PMEM NUMA node number
  @param[in] Socket Socket number

  @retval None

**/
VOID
SavePmemInfo (
  IN UINT8 Pmem,
  IN UINT8 Socket,
  MEM_TYPE Type
  )
{
  if (Pmem < EFI_ACPI_SLIT_PMEM_INFO_CNT) {

    DEBUG((DEBUG_INFO, "SLIT: Found PMem %d at socket %d (mem type 0x%X)\n", Pmem, Socket, Type));
    mSlitPmemInfo[Pmem].Pmem = Pmem;
    mSlitPmemInfo[Pmem].Socket = Socket;
    mSlitPmemInfo[Pmem].Valid = TRUE;
    return;
  }

  DEBUG((DEBUG_ERROR, "SLIT: Error - Number of PMem nodes (%d) exceed PMem info data table size (%d)\n",
      Pmem, EFI_ACPI_SLIT_PMEM_INFO_CNT));
  ASSERT(FALSE);
}

/**

  Retrieves socket correlated with given AEP PMEM NUMA node

  @param[in] Pmem AEP PMEM Numa node

  @retval Socket correlated with given AEP PMEM NUMA node

**/
UINT8
GetSocketForPmem (
  IN UINT8 Pmem
  )
{
  UINT8 PmemInfoEntry;

  for (PmemInfoEntry = 0; PmemInfoEntry < EFI_ACPI_SLIT_PMEM_INFO_CNT; PmemInfoEntry++) {
    if (mSlitPmemInfo[PmemInfoEntry].Valid && mSlitPmemInfo[PmemInfoEntry].Pmem == Pmem) {
      return mSlitPmemInfo[PmemInfoEntry].Socket;
    }
  }

  DEBUG((DEBUG_ERROR, "SLIT: Error - PMem node (%d) is not associated with any socket\n", Pmem));
  ASSERT(FALSE);
  return PMEM_INVALID_SOCKET;
}

/**
  Retrieves number of FPGA Presents

  @retval Number of FPGA Presents in the system

**/
UINT8
GetFpgaCount(
  VOID)
{
  UINT8 FpgaCount = 0;
  UINT32 FpgaPresentBitMap = mCpuCsrAccessVarPtr->FpgaPresentBitMap;

  while (FpgaPresentBitMap) {
    if (FpgaPresentBitMap & BIT0) {
      FpgaCount++;
    }
    FpgaPresentBitMap >>= 1;
  }
  return FpgaCount;
}
/**

  Retrieves number of AEP PMEM NUMA nodes

  @param[in]      NumCpus       Number of CPUs

  @retval Number of AEP PMEM NUMA nodes

**/
UINT8
GetPmemNodeCount (
  IN UINT8 NumCpus
  )
{
  UINT8 Socket;
  UINT8 SadRule;
  UINT8 PmemNodeCount = 0;

  for (Socket = 0; Socket < NumCpus; Socket++) {
    for (SadRule = 0; SadRule < MAX_SAD_RULES; SadRule++) {
      //
      // Only local SADs of PMEM type should be taken into consideration.
      // Skip any memory region marked reserved.
      //
      if (mSystemMemoryMap->Socket[Socket].SAD[SadRule].local &&
          IsMemTypeAppDirect (mSystemMemoryMap->Socket[Socket].SAD[SadRule].type) &&
          !IsMemTypeReserved (mSystemMemoryMap->Socket[Socket].SAD[SadRule].type)) {

        SavePmemInfo (PmemNodeCount++, Socket, mSystemMemoryMap->Socket[Socket].SAD[SadRule].type);
      }
    }
  }

  return PmemNodeCount;
}

/**

  Retrieves number of clusters in the system if system is in SNC mode. If system is not
  in SNC mode, the return 1.

  @return Number of clusters in the system in SNC mode or 1 if system is not in SNC mode.

**/
UINT8
GetNumClusters (
  VOID
  )
{
  UINT8 NumClusters;

  NumClusters = mIioUds->IioUdsPtr->SystemStatus.OutNumOfCluster;
  if ((NumClusters == 0) || (mIioUds->IioUdsPtr->SystemStatus.OutSncEn == 0)) {
    NumClusters = 1; // For non-SNC mode, we should also return 1.
  }

  return NumClusters;
}

/**

  Retrieves number of CPUs in the system

  @param None

  @retval Number of CPUs in the system

**/
UINT8
GetNumCpus (
  VOID
  )
{
  return mIioUds->IioUdsPtr->SystemStatus.numCpus;
}

/**

  Calculates total number of nodes

  @param[in]      NumCpus       Number of CPUs
  @param[in]      NumClusters   Number of clusters
  @param[in]      PmemNodeCount Number of AEP PMEM NUMA nodes

  @retval Total number of nodes

**/
UINT8
GetNodeCount (
  IN UINT8  NumCpus,
  IN UINT8  NumClusters,
  IN UINT8  PmemNodeCount,
  IN UINT8 FpgaCount
  )
{
  UINT8 NodeCount;

  if (mSystemMemoryMap->volMemMode == VOL_MEM_MODE_MIX_1LM2LM) {
    NodeCount = ((NumCpus * NumClusters * EFI_ACPI_SLIT_DOMAINS_NODES_MAX_CNT) + PmemNodeCount + FpgaCount);
  } else {
    NodeCount = ((NumCpus * NumClusters) + PmemNodeCount + FpgaCount);
  }
  if ((NodeCount * NodeCount) < EFI_ACPI_SYSTEM_LOCALITIES_ENTRY_COUNT) {
    return NodeCount;
  }

  DEBUG((DEBUG_ERROR, "SLIT: Error - Nodes distances info data size (%d) exceed SLIT distances info table size (%d)\n",
    (NodeCount * NodeCount), EFI_ACPI_SYSTEM_LOCALITIES_ENTRY_COUNT));
  ASSERT(FALSE);
  return 0;
}

/**

  Verifies whether sockets are linked

  @param[in]      SourceSocket  Source Socket ID
  @param[in]      TargetSocket  Targer Socket ID

  @retval TRUE link between source socket and target socket was found
          FALSE otherwise

**/
BOOLEAN
SocketsLinked (
  IN UINT8  SourceSocket,
  IN UINT8  TargetSocket
  )
{
  UINT8 PeerSocket;
  UINT8 LinkValid;
  UINT8 PeerSocId;

  // Validate sockets ids
  if ((SourceSocket < MAX_SOCKET) && (TargetSocket < MAX_SOCKET)) {
    // Do not process when source socket is the same as target socket
    if (SourceSocket != TargetSocket) {
      for (PeerSocket = 0; PeerSocket < (GetKtiPortCnt()); PeerSocket++) {
        LinkValid = mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[SourceSocket].PeerInfo[PeerSocket].Valid;
        PeerSocId = mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[SourceSocket].PeerInfo[PeerSocket].PeerSocId;
        if (LinkValid && (PeerSocId == TargetSocket)) {
          // Link found
          return TRUE;
        }
      }
    }
    // Link not found
    return FALSE;
  }

  DEBUG((DEBUG_ERROR, "SLIT: Error when checking if sockets are linked (source socket id %d, target socket id %d)\n", SourceSocket, TargetSocket));
  ASSERT(FALSE);
  return FALSE;
}

/**

  Initializes SLIT Table entries

  @param[in,out] Table Pointer to SLIT ACPI tables

  @retval EFI_SUCCESS  operation completed successfully

**/
EFI_STATUS
InitEntries (
  IN OUT ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *SlitPtr
  )
{
  if (SlitPtr == NULL) {

    ASSERT (SlitPtr != NULL);
    return EFI_INVALID_PARAMETER;
  }
  SetMem (&SlitPtr->NumSlit[0].Entry, EFI_ACPI_SYSTEM_LOCALITIES_ENTRY_COUNT, 0xFF);

  return EFI_SUCCESS;
}

/**

  Processes socket nodes

  @param[in,out]  Table         Pointer to SLIT ACPI tables
  @param[in]      NumCpus       Number of CPUs
  @param[in]      NumClusters   Number of clusters
  @param[in]      PmemNodeCount Number of AEP PMEM NUMA nodes
  @param[in]      NodeCount     Number of all nodes

  @retval EFI_SUCCESS  operation completed successfully

**/
EFI_STATUS
ProcessSockets (
  IN OUT EFI_ACPI_COMMON_HEADER *Table,
  IN UINT8                      NumCpus,
  IN UINT8                      NumClusters,
  IN UINT8                      PmemNodeCount,
  IN UINT8                      NodeCount
  )
{
  ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE  *SLitAcpiTable;
  UINT16                                  EntryIdx;
  UINT8                                   SourceNode;
  UINT8                                   SourceSocket;
  UINT8                                   SourceCluster;
  UINT8                                   TargetSocket;
  UINT8                                   TargetCluster;

  if (NULL == Table) {
    DEBUG((DEBUG_ERROR, "SLIT: Error in SLIT update with data about nodes on same socket\n"));
    return EFI_INVALID_PARAMETER;
  }

  DEBUG((DEBUG_INFO, "SLIT: Update with data about nodes on same socket\n"));

  SLitAcpiTable = (ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *)Table;

  for (SourceNode = 0; SourceNode < (NodeCount - PmemNodeCount) ; SourceNode++) {
    SourceSocket = SourceNode / NumClusters;
    SourceCluster = SourceNode % NumClusters;
    for (TargetSocket = 0; TargetSocket < NumCpus; TargetSocket++) {
      for (TargetCluster = 0; TargetCluster < NumClusters; TargetCluster++) {
        if (SourceSocket == TargetSocket) {
          EntryIdx = (SourceNode * (NodeCount)) + (TargetSocket * NumClusters) + TargetCluster;
          //Source and target are nodes on same socket
          if (SourceCluster == TargetCluster) {
            //a) Same socket same cluster
            SLitAcpiTable->NumSlit[EntryIdx].Entry = ZERO_HOP;
          } else {
            //b) Same socket different cluster
            SLitAcpiTable->NumSlit[EntryIdx].Entry = ZERO_ONE;
          }
        }
      }
    }
  }

  return EFI_SUCCESS;
}

/**

  Processes socket nodes

  @param[in,out]  Table         Pointer to SLIT ACPI tables
  @param[in]      NumCpus       Number of CPUs
  @param[in]      NumClusters   Number of clusters
  @param[in]      PmemNodeCount Number of AEP PMEM NUMA nodes
  @param[in]      NodeCount     Number of all nodes

  @retval EFI_SUCCESS  operation completed successfully

**/
EFI_STATUS
ProcessMixedModeSockets (
  IN OUT EFI_ACPI_COMMON_HEADER *Table,
  IN UINT8                      NumCpus,
  IN UINT8                      NumClusters,
  IN UINT8                      PmemNodeCount,
  IN UINT8                      NodeCount
  )
{
  ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE  *SLitAcpiTable;
  UINT16                                  EntryIdx1LM;
  UINT16                                  EntryIdx2LM;
  UINT8                                   SourceNode;
  UINT8                                   SourceSocket;
  UINT8                                   SourceCluster;
  UINT8                                   TargetSocket;
  UINT8                                   TargetCluster;
  BOOLEAN                                 Is2LM;

  if (NULL == Table) {
    DEBUG((DEBUG_ERROR, "SLIT: Error in SLIT update with data about nodes on same socket for Mixed Mode\n"));
    return EFI_INVALID_PARAMETER;
  }

  DEBUG((DEBUG_INFO, "SLIT: Update with data about nodes on same socket for Mixed Mode\n"));

  SLitAcpiTable = (ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *)Table;

  for (SourceNode = 0; SourceNode < (NodeCount - PmemNodeCount) ; SourceNode++) {
    Is2LM = (SourceNode >= NumCpus * NumClusters) ? TRUE : FALSE;
    SourceSocket = (SourceNode - NumCpus * NumClusters * (Is2LM ? 1 : 0)) / NumClusters;
    SourceCluster = (SourceNode - NumCpus * NumClusters * (Is2LM ? 1 : 0)) % NumClusters;

    TargetSocket = SourceSocket;
    for (TargetCluster = 0; TargetCluster < NumClusters; TargetCluster++) {
      EntryIdx1LM = (UINT16)((SourceNode * NodeCount) + (TargetSocket * NumClusters) + TargetCluster);
      EntryIdx2LM = EntryIdx1LM + NumCpus * NumClusters;

      if ((SourceCluster == TargetCluster) && (Is2LM == FALSE)) {
        //CPU -> 1LM at the same socket, the same cluster
        SLitAcpiTable->NumSlit[EntryIdx1LM].Entry = ZERO_HOP;
        //CPU -> 2LM at the same socket, the same cluster
        SLitAcpiTable->NumSlit[EntryIdx2LM].Entry = ZERO_ONE;
      } else if ((SourceCluster == TargetCluster) && (Is2LM == TRUE)) {
        //CPU -> 2LM at the same socket, the same cluster
        SLitAcpiTable->NumSlit[EntryIdx1LM].Entry = ZERO_ONE;
        //not effective
        SLitAcpiTable->NumSlit[EntryIdx2LM].Entry = ZERO_HOP;
      } else if ((SourceCluster != TargetCluster) && (Is2LM == FALSE)) {
        //CPU -> 1LM at the same socket, different cluster
        SLitAcpiTable->NumSlit[EntryIdx1LM].Entry = ZERO_ONE;
        //CPU -> 2LM at the same socket, different cluster
        SLitAcpiTable->NumSlit[EntryIdx2LM].Entry = ZERO_TWO;
      } else {
        //branch condition: ((SourceCluster != TargetCluster) && (Is2LM == TRUE))
        //CPU -> 2LM at the same socket, different cluster
        SLitAcpiTable->NumSlit[EntryIdx1LM].Entry = ZERO_TWO;
        //not effective
        SLitAcpiTable->NumSlit[EntryIdx2LM].Entry = ZERO_TWO;
      }
    }
  }

  return EFI_SUCCESS;
}

/**

  Processes connections between sockets to retrieve valid distances

  @param[in,out]  Table         Pointer to SLIT ACPI tables
  @param[in]      NumCpus       Number of CPUs
  @param[in]      NumClusters   Number of clusters
  @param[in]      PmemNodeCount Number of AEP PMEM NUMA nodes
  @param[in]      NodeCount     Number of all nodes

  @retval EFI_SUCCESS  operation completed successfully

**/
EFI_STATUS
ProcessSocketsLinks (
  IN OUT EFI_ACPI_COMMON_HEADER *Table,
  IN UINT8                      NumCpus,
  IN UINT8                      NumClusters,
  IN UINT8                      PmemNodeCount,
  IN UINT8                      NodeCount
  )
{
  ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE  *SLitAcpiTable;
  UINT8                                   SourceNode;
  UINT8                                   SourceSocket;
  UINT8                                   SourceCluster;
  UINT8                                   TargetSocket;

  if (NULL == Table) {
    DEBUG((DEBUG_ERROR, "SLIT: Error in processing links between sockets\n"));
    return EFI_INVALID_PARAMETER;
  }

  DEBUG((DEBUG_INFO, "SLIT: Update table with links between sockets\n"));

  SLitAcpiTable = (ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *)Table;

  for (SourceNode = 0; SourceNode < (NodeCount - PmemNodeCount); SourceNode++) {
    SourceSocket = SourceNode / NumClusters;
    SourceCluster = SourceNode % NumClusters;
    for (TargetSocket = 0; TargetSocket < NumCpus; TargetSocket++) {
      if (SocketsLinked(SourceSocket, TargetSocket)) {
        SetMem (&SLitAcpiTable->NumSlit[(SourceNode * NodeCount) + (TargetSocket * NumClusters)].Entry,
                NumClusters, ONE_HOP);
      }
    }
  }

  return EFI_SUCCESS;
}

/**

  Processes connections between sockets to retrieve valid distances

  @param[in,out]  Table         Pointer to SLIT ACPI tables
  @param[in]      NumCpus       Number of CPUs
  @param[in]      NumClusters   Number of clusters
  @param[in]      PmemNodeCount Number of AEP PMEM NUMA nodes
  @param[in]      NodeCount     Number of all nodes

  @retval EFI_SUCCESS  operation completed successfully

**/
EFI_STATUS
ProcessMixedModeSocketsLinks (
  IN OUT EFI_ACPI_COMMON_HEADER *Table,
  IN UINT8                      NumCpus,
  IN UINT8                      NumClusters,
  IN UINT8                      PmemNodeCount,
  IN UINT8                      NodeCount
  )
{
  ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE  *SLitAcpiTable;
  UINT16                                  EntryIdx1LM;
  UINT16                                  EntryIdx2LM;
  UINT8                                   SourceNode;
  UINT8                                   SourceSocket;
  UINT8                                   SourceCluster;
  UINT8                                   TargetSocket;
  UINT8                                   TargetCluster;
  BOOLEAN                                 Is2LM;

  if (NULL == Table) {
    DEBUG((DEBUG_ERROR, "SLIT: Error in processing links between sockets in Mixed Mode\n"));
    return EFI_INVALID_PARAMETER;
  }

  DEBUG((DEBUG_INFO, "SLIT: Update table with links between sockets in Mixed Mode\n"));

  SLitAcpiTable = (ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *)Table;

  for (SourceNode = 0; SourceNode < (NodeCount - PmemNodeCount); SourceNode++) {
    Is2LM = (SourceNode >= NumCpus * NumClusters) ? TRUE : FALSE;
    SourceSocket = (SourceNode - NumCpus * NumClusters * (Is2LM ? 1 : 0)) / NumClusters;
    SourceCluster = (SourceNode - NumCpus * NumClusters * (Is2LM ? 1 : 0)) % NumClusters;

    for (TargetSocket = 0; TargetSocket < NumCpus; TargetSocket++) {
      if (SocketsLinked(SourceSocket, TargetSocket)) {
        //
        // If both sockets are linked by KTI and not the same socket.
        //
        for (TargetCluster = 0; TargetCluster < NumClusters; TargetCluster++) {
          EntryIdx1LM = (UINT16)((SourceNode * NodeCount) + (TargetSocket * NumClusters) + TargetCluster);
          EntryIdx2LM = EntryIdx1LM + NumCpus * NumClusters;

          if (Is2LM == FALSE) {
            //CPU -> 1LM at different socket
            SLitAcpiTable->NumSlit[EntryIdx1LM].Entry = ONE_HOP;
            //CPU -> 2LM at different socket
            SLitAcpiTable->NumSlit[EntryIdx2LM].Entry = ONE_ONE;
          } else {
            //branch condition: (Is2LM == TRUE)
            //CPU -> 2LM at different socket
            SLitAcpiTable->NumSlit[EntryIdx1LM].Entry = ONE_ONE;
            //not effective
            SLitAcpiTable->NumSlit[EntryIdx2LM].Entry = ONE_TWO;
          }
        }
      }
    }
  }

  return EFI_SUCCESS;
}

/**

  Processes all AEP PMEM NUMA nodes

  @param[in,out]  Table         Pointer to SLIT ACPI tables
  @param[in]      NumCpus       Number of CPUs
  @param[in]      NumClusters   Number of clusters
  @param[in]      PmemNodeCount Number of AEP PMEM NUMA nodes
  @param[in]      NodeCount     Number of all nodes

  @retval EFI_SUCCESS  operation completed successfully

**/
EFI_STATUS
ProcessPmems (
  IN OUT EFI_ACPI_COMMON_HEADER *Table,
  IN UINT8                      NumCpus,
  IN UINT8                      NumClusters,
  IN UINT8                      PmemNodeCount,
  IN UINT8                      NodeCount
  )
{
  ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE  *SLitAcpiTable;
  UINT16                                  EntryIdx;
  UINT8                                   SourceNode;
  UINT8                                   TargetNode;
  UINT8                                   SourceSocket;
  UINT8                                   TargetCluster;
  UINT8                                   TargetSocket;
  UINT8                                   SourcePmem;
  UINT8                                   TargetPmem;
  UINT8                                   SourcePmemSocket;
  UINT8                                   TargetPmemSocket;
  UINT8                                   TotalVolMemNodes;

  if (NULL == Table) {
    DEBUG((DEBUG_ERROR, "SLIT: Error in processing PMems\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (mSystemMemoryMap->volMemMode == VOL_MEM_MODE_MIX_1LM2LM) {
    TotalVolMemNodes = NumCpus * NumClusters * EFI_ACPI_SLIT_DOMAINS_NODES_MAX_CNT;
  } else {
    TotalVolMemNodes = NumCpus * NumClusters;
  }

  DEBUG((DEBUG_INFO, "SLIT: Include PMem NUMA nodes\n"));

  if (PmemNodeCount > 0) {

    SLitAcpiTable = (ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *)Table;

    //1) AEP PMEM nodes to AEP PMEM nodes distances
    for (SourceNode = (NodeCount - PmemNodeCount); SourceNode < NodeCount; SourceNode++) {
      SourcePmem = SourceNode - TotalVolMemNodes;
      for (TargetPmem = 0; TargetPmem < PmemNodeCount; TargetPmem++) {
        TargetNode = TargetPmem + TotalVolMemNodes;
        EntryIdx = (SourceNode * NodeCount) + TargetNode;
        if (SourcePmem == TargetPmem) {
          SLitAcpiTable->NumSlit[EntryIdx].Entry = PMEM_ZERO_HOP;
        } else {
          // Retrieve sockets associated with PMEMs
          SourcePmemSocket = GetSocketForPmem (SourcePmem);
          TargetPmemSocket = GetSocketForPmem (TargetPmem);

          if (SourcePmemSocket == TargetPmemSocket) {
            // PMEMs are on the same socket
            SLitAcpiTable->NumSlit[EntryIdx].Entry = PMEM_ONE_ONE;
          } else {
            // Assign 2 hop and process with PeerInfo checking
            SLitAcpiTable->NumSlit[EntryIdx].Entry = PMEM_TWO_HOP;

            // Examine PeerInfo to look for link between AEP PMEM source socket and AEP PMEM target socket
            if (SocketsLinked (SourcePmemSocket, TargetPmemSocket)) {
              // Link found assign 1 hop
              SLitAcpiTable->NumSlit[EntryIdx].Entry = PMEM_ONE_HOP;
            }
          }
        }
      }
    }

    //2) Sockets to AEP PMEM nodes distances
    for (SourceNode = 0; SourceNode < (NodeCount - PmemNodeCount); SourceNode++) {
      SourceSocket = SourceNode / NumClusters;
      for (TargetPmem = 0; TargetPmem < PmemNodeCount; TargetPmem++) {
        TargetPmemSocket = GetSocketForPmem (TargetPmem);
        TargetNode = TargetPmem + TotalVolMemNodes;
        EntryIdx = (SourceNode * NodeCount) + TargetNode;
        if(SourceSocket == TargetPmemSocket) {
          SLitAcpiTable->NumSlit[EntryIdx].Entry = PMEM_ONE_ONE;
        } else {
          // Assign 2 hop and process with PeerInfo checking
          SLitAcpiTable->NumSlit[EntryIdx].Entry = PMEM_TWO_HOP;

          // Examine PeerInfo to look for link between source socket and AEP PMEM socket
          if (SocketsLinked (SourceSocket, TargetPmemSocket)) {
            // Link found assign 1 hop
            SLitAcpiTable->NumSlit[EntryIdx].Entry = PMEM_ONE_HOP;
          }
        }
      }
    }

    //3) AEP PMEM nodes to sockets distances
    for (SourceNode = (NodeCount - PmemNodeCount); SourceNode < NodeCount; SourceNode++) {
      SourcePmem = SourceNode - TotalVolMemNodes;
      SourcePmemSocket = GetSocketForPmem (SourcePmem);
      for (TargetSocket = 0; TargetSocket < NumCpus; TargetSocket++) {
        for (TargetCluster = 0; TargetCluster < NumClusters; TargetCluster++) {
          EntryIdx = (SourceNode * NodeCount) + (TargetSocket * NumClusters) + TargetCluster;
          if(SourcePmemSocket == TargetSocket) {
            SLitAcpiTable->NumSlit[EntryIdx].Entry = PMEM_ONE_ONE;
          } else {
            // Assign 2 hop and process with PeerInfo checking
            SLitAcpiTable->NumSlit[EntryIdx].Entry = PMEM_TWO_HOP;

            // Examine PeerInfo to look for link between source socket and AEP PMEM socket
            if (SocketsLinked (SourcePmemSocket, TargetSocket)) {
              // Link found assign 1 hop
              SLitAcpiTable->NumSlit[EntryIdx].Entry = PMEM_ONE_HOP;
            }
          }
        }
      }
    }
  } else {
    DEBUG((DEBUG_INFO, "SLIT: PMem NUMA nodes not present\n"));
  }

  return EFI_SUCCESS;
}

/**

  Processes all remaining valid SLIT nodes

  @param[in,out] Table  pointer to SLIT ACPI tables

  @retval EFI_SUCCESS  operation completed successfully

**/
EFI_STATUS
ProcessRemainingNodes (
  IN OUT EFI_ACPI_COMMON_HEADER *Table
  )
{
  ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE  *SLitAcpiTable;
  UINT16                                  EntryIdx = 0;
  UINT16                                  MaxTableEntries;
  UINT8                                   NodeCount;

  if (NULL == Table) {
    DEBUG((DEBUG_ERROR, "SLIT: Error while processing remaining valid nodes\n"));
    return EFI_INVALID_PARAMETER;
  }

  DEBUG((DEBUG_INFO, "SLIT: Fill in the rest of the SLIT table\n"));

  SLitAcpiTable = (ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *)Table;

  NodeCount = (UINT8)SLitAcpiTable->Header.NumberOfSystemLocalities;
  MaxTableEntries = NodeCount * NodeCount;

  while (EntryIdx < MaxTableEntries) {
    if (SLitAcpiTable->NumSlit[EntryIdx].Entry == 0xFF) {
      //This entry has not been filled yet, assign 2 hop to this table entry
      SLitAcpiTable->NumSlit[EntryIdx].Entry = TWO_HOP;
    }

    if ((EntryIdx % NodeCount) == 0) {
      DEBUG((EFI_D_INFO, "[%2d - %2d] ", EntryIdx/NodeCount, EntryIdx%NodeCount));
    }
    EntryIdx++;
  }

  return EFI_SUCCESS;
}

/**

  Processes unused SLIT nodes

  @param[in,out] Table  pointer to SLIT ACPI tables

  @retval EFI_SUCCESS  operation completed successfully

**/
EFI_STATUS
ProcessUnusedNodes (
  IN OUT EFI_ACPI_COMMON_HEADER *Table
  )
{
  ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE  *SLitAcpiTable;
  UINT16                                  MaxTableEntries;
  UINT8                                   NodeCount;

  if (NULL == Table) {
    DEBUG((DEBUG_ERROR, "SLIT: Error while processing unused nodes\n"));
    return EFI_INVALID_PARAMETER;

  }

  DEBUG((DEBUG_INFO, "SLIT: Zero out the unused nodes\n"));

  SLitAcpiTable = (ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *)Table;

  NodeCount = (UINT8)SLitAcpiTable->Header.NumberOfSystemLocalities;
  MaxTableEntries = NodeCount * NodeCount;

  SetMem (&SLitAcpiTable->NumSlit[MaxTableEntries],
         (UINTN)&SLitAcpiTable->NumSlit[EFI_ACPI_SYSTEM_LOCALITIES_ENTRY_COUNT] - (UINTN)&SLitAcpiTable->NumSlit[MaxTableEntries], 0);

  return EFI_SUCCESS;
}

/**

  Updates System Locality Distance Information Table (SLIT)

  @param[in,out] Table  pointer to SLIT ACPI tables

  @retval EFI_SUCCESS  operation completed successfully
  @retval EFI_ABORTED  operation not completed due to processing error

**/
EFI_STATUS
PatchSLitTable (
   IN OUT EFI_ACPI_COMMON_HEADER  *Table
   )
{
  ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE  *SLitAcpiTable;
  EFI_STATUS                              Status;
  UINT8                                   NodeCount;
  UINT8                                   NumCpus;
  UINT8                                   NumClusters;
  UINT8                                   PmemNodeCount;
  UINT8                                   FpgaCount;

  NumCpus = GetNumCpus ();
  NumClusters = GetNumClusters ();
  PmemNodeCount = GetPmemNodeCount (NumCpus);
  FpgaCount = GetFpgaCount();
  NodeCount = GetNodeCount (NumCpus, NumClusters, PmemNodeCount, FpgaCount);

  DEBUG ((DEBUG_INFO, "SLIT: NumCpus %d, NumClusters %d, PmemNodeCount %d -> NodeCount %d FpgaCount %d\n",
                             NumCpus, NumClusters, PmemNodeCount, NodeCount,FpgaCount));

  SLitAcpiTable = (ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *)Table;
  SLitAcpiTable->Header.NumberOfSystemLocalities = NodeCount;

  //1) Initialize all entries to 0xFF
  Status = InitEntries (SLitAcpiTable);

  //2) Update SLIT table with data about nodes on same socket
  if (!EFI_ERROR(Status)) {
    if (mSystemMemoryMap->volMemMode == VOL_MEM_MODE_MIX_1LM2LM) {
      Status = ProcessMixedModeSockets (Table, NumCpus, NumClusters, PmemNodeCount, NodeCount);
    } else {
      Status = ProcessSockets (Table, NumCpus + FpgaCount, NumClusters, PmemNodeCount, NodeCount);
    }
  }

  //3) Update table with links between sockets by examining PeerInfo structure
  if (!EFI_ERROR (Status)) {
    if (mSystemMemoryMap->volMemMode == VOL_MEM_MODE_MIX_1LM2LM) {
      Status = ProcessMixedModeSocketsLinks (Table, NumCpus, NumClusters, PmemNodeCount, NodeCount);
    } else {
      Status = ProcessSocketsLinks (Table, NumCpus, NumClusters, PmemNodeCount, NodeCount);
    }
  }

  //4) Update table with PMEMs
  if (!EFI_ERROR (Status)) {
    Status = ProcessPmems (Table, NumCpus, NumClusters, PmemNodeCount, NodeCount);
  }

  //5) Fill in the rest of the Slit table, 2 hops between any remaining valid nodes
  if (!EFI_ERROR (Status)) {
    Status = ProcessRemainingNodes (Table);
  }

  //6) Zero out the unused nodes
  if (!EFI_ERROR (Status)) {
    Status = ProcessUnusedNodes (Table);
  }

  //7) Print the entire SLIT table
  if (!EFI_ERROR (Status)) {
    DisplayEntries (SLitAcpiTable);
  }

  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  Allocate memory and fill SLIT information to this buffer, then
  install this table to ACPI table.

  @retval EFI_SUCCESS           Install table success.
  @retval EFI_OUT_OF_RESOURCES  Out of memory.

**/
EFI_STATUS
InstallSlitTable(
   VOID
   )
{
  ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *Table;
  UINTN                                  TableSize;
  UINT8                                  NumClusters;
  UINTN                                  TableHandle = 0;
  EFI_STATUS                             Status;

  NumClusters = GetNumClusters ();

  TableSize = sizeof (EFI_ACPI_6_2_SYSTEM_LOCALITY_DISTANCE_INFORMATION_TABLE_HEADER) +
              sizeof (ACPI_SYSTEM_LOCALITIES_STRUCTURE) * EFI_ACPI_SYSTEM_LOCALITIES_ENTRY_COUNT;

  Table = (ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *) AllocateZeroPool (TableSize);
  if (Table == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Table->Header.Header.Signature = EFI_ACPI_6_2_SYSTEM_LOCALITY_INFORMATION_TABLE_SIGNATURE;
  Table->Header.Header.Length = (UINT32) TableSize;
  Table->Header.Header.Revision = EFI_ACPI_6_2_SYSTEM_LOCALITY_DISTANCE_INFORMATION_TABLE_REVISION;
  Table->Header.Header.OemRevision = EFI_ACPI_OEM_SLIT_REVISION;
  CopyMem (Table->Header.Header.OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (Table->Header.Header.OemId));
  Table->Header.Header.OemTableId = PcdGet64 (PcdAcpiDefaultOemTableId);
  Table->Header.Header.CreatorId = PcdGet32 (PcdAcpiDefaultCreatorId);
  Table->Header.Header.CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);

  //
  // All node init with 0 before pass to patcher
  //
  PatchSLitTable ((EFI_ACPI_COMMON_HEADER *)&Table->Header.Header);

  //
  // Publish SLIT Structure to ACPI
  //

  Status = mAcpiTable->InstallAcpiTable (
                          mAcpiTable,
                          Table,
                          Table->Header.Header.Length,
                          &TableHandle
                          );

  //
  // Free memory
  //
  if (Table != NULL) {
    FreePool (Table);
  }

  return Status;
}

