/** @file
  Interface header file for the HBM library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef _MEM_DECODE_HBM_LIB_H_
#define _MEM_DECODE_HBM_LIB_H_

#define ROUTE_TABLE_TARGET_ONLY 0

// ********************************************************************************************************************
//                                                                                                                    *
// Public HBM Memory Decode interfaces                                                                                         *
//                                                                                                                    *
// ********************************************************************************************************************

/**
  Intitialize HBM channel and MC bitmaps

  @param[in,out] ChBitmap   Array of channel bitmaps.
  @param[in,out] McBitMap   Memory controller bitmap.

**/
VOID
EFIAPI
LoadHbm2lmInterleaveData (
  IN OUT UINT8 *ChBitmap,
  IN OUT UINT8 *McBitMap
  );

/**
  Allocate private structure for HBM.

  @param[in,out] MemMapData   The private data structure for memory map.

**/
VOID
EFIAPI
InitHbmMemoryMap (
  IN OUT MEMORY_MAP_DATA *MemMapData
  );

/**
  Test if HBM is enabled.

  Test if HBM stacks are enabled.

  @param[in] MemMapData   Pointer to the memory map private data structure.

  @retval  TRUE           HBM is enabled.
  @retval  FALSE          HBM is not enabled.

**/
BOOLEAN
EFIAPI
IsHbmEnabled (
  IN MEMORY_MAP_DATA *MemMapData
  );

/**
  Check if we have HBM in quadrant mode.

  @param[in] MemMapData   The private data structure for memory map.
  @param[in] Socket       Current Socket (0-Based)

  @retval    True  HBM is in quadrant mode
  @retval    False HBM is not in quadrant mode

**/
BOOLEAN
IsHbmInQuadrantMode (
  IN MEMORY_MAP_DATA *MemMapData,
  IN  UINT8          Socket
  );

/**
  Retrieve HBM stack count.

  Return the number of enabled HBM stacks on socket.

  @param[in] MemMapData   Pointer to the memory map private data structure.
  @param[in] Socket       Current Socket (0-Based)

  @return                 Number of enabled HBM stacks.

**/
UINT8
EFIAPI
GetHbmStackCount (
  IN MEMORY_MAP_DATA *MemMapData,
  IN  UINT8          Socket
  );

/**
  Populate HBM size fields.

  Populate the HBM memory map private data structure with the size of available
  HBM in the system, by reading the output data from HBM IO training.

  @param[in] MemMapData   Pointer to the memory map private data structure.
  @param[in] Socket       Current Socket (0-Based)

  @return                 Total amount of HBM in the Socket expressed in
                          units of 64MB.
**/
UINT32
EFIAPI
PopulateHbmSizeFields (
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           Socket
  );

/**
  Get the HBM Cache Size in units of 64MB

  Assumes we are in Hbm cache mode

  @param[in] MemMapData The private data structure for memory map.
  @param[in] Socket       Socket number


  @return The HBM Cache Size in units of 64MB

**/
UINT32
EFIAPI
GetHbmCacheSize (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8            Socket
  );

/**
  Check if the Socket is using HBM memory.

  @param[in] MemMapData   The private data structure for memory map.
  @param[in] Socket       Current Socket (0-Based)

  @retval    TRUE    HBM is enabled in Socket.
  @retval    FALSE   HBM is disabled in Socket.
**/
BOOLEAN
EFIAPI
IsHbmEnabledInSkt (
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           Socket
  );

/**
  Get HBM Flat Interleave List


  @return  Flat interleave list for HBM
**/
UINT32
EFIAPI
GetHbmFlatInterleaveList (
  VOID
  );


/** Write Route table data for Hbm 1LM.

  @param[in]  MemMapData - Pointer to the memory map private data structure.
  @param[in]  Socket     - Socket Id
  @param[in]  Cluster    - Cluster ID in socket, only 0 if socket is not clustered.
  @param[in]  TotCluster - Total number of clusters enabled in a socket.

  @retval None

**/
VOID
EFIAPI
WriteHbmRouteTables (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Socket,
  IN UINT8           Cluster,
  IN UINT8           TotCluster
  );

/** Write Mesh to Memory CSRs for Hbm.

  This function writes mesh to memory CSR registers

  @param[in]  MemMapData - Pointer to the memory map private data structure.
  @param[in]  Socket     - Socket Id

**/
VOID
EFIAPI
WriteHbmMesh2MemCsrs (
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            Socket
  );

/**
  Create a SAD rule for HBM as flat memory.

  Create a SAD rule for a region of memory containing only HBM as flat memory.
  If SNC is enabled by policy, then this function will create a SAD
  rule for the cluster specified by Cluster. Otherwise, Cluster is ignored.

  @param[in] MemMapData   Pointer to the memory map private data structure.
  @param[in] Socket       Current Socket (0-Based)
  @param[in] SadIndex     Index of the SAD rule to create.
  @param[in] Cluster      If SNC is enabled, cluster for which to create a
                                             rule.

  @retval  EFI_SUCCESS            An HBM SAD rule was successfully created.
  @retval  EFI_INVALID_PARAMETER  An HBM SAD rule was not created.
**/
EFI_STATUS
EFIAPI
CreateHbmFlatSadRule (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Socket,
  IN UINT8           SadIndex,
  IN UINT8           Cluster
  );

/**
  Create TAD rules for HBM.

  Iterate through the existing SAD rules. For each one that includes HBM,
  create TAD rules in the HBM MCs.

  @param[in] MemMapData   Pointer to the memory map private data structure.

  @retval  EFI_SUCCESS             Any HBM TAD rules were successfully created.
  @retval  EFI_INVALID_PARAMETER   An invalid parameter was passed in.
  @retval  EFI_OUT_OF_RESOURCES    Max TAD rules was exceeded.
**/
EFI_STATUS
EFIAPI
CreateHbmTadRules (
  IN MEMORY_MAP_DATA *MemMapData
  );

/**
  Write TAD CSRs for HBM.

  For each HBM IO stack, write the TAD CSRs to its MCs.

  @param[in] Socket     Current Socket (0-Based)
  @param[in] MemMapData Pointer to the memory map private data structure

**/
VOID
WriteHbmTadCsrs (
  IN UINT8            Socket,
  IN MEMORY_MAP_DATA  *MemMapData
  );

/**
  Display TAD rule tables for HBM.

  Iterate through the existing TAD rules for each HBM IO stack and display
  their data.

  @param[in] Socket       Current Socket (0-Based)
  @param[in] MemMapData   Pointer to the memory map private data structure.

**/
VOID
DisplayHbmTadTables (
  IN  UINT8           Socket,
  IN MEMORY_MAP_DATA  *MemMapData
  );

/**

  This function writes M2M and MC CSRs to enable or disable the early completion.

  @param[in] SocketId          Socket index.

**/
VOID
HbmWriteEarlyCmpCsrs (
  IN UINT8   SocketId
  );
#endif // _MEM_DECODE_HBM_LIB_H_
