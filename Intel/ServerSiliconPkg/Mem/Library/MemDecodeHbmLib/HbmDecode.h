/** @file
  Interface header file for the HBM library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#ifndef _HBM_DECODE_H_
#define _HBM_DECODE_H_

#define MIN_STACK_COUNT      1
#define ALL_STACKS_SPR       0xF
#define HBM_IO_EDC_START_TGT 4
#define HBM_CHAN_CAP_RANGE_BASE 0x8

//
// For HBM flat memory, always use H0 route tables.
//
#define HBM_FLAT_INTERLEAVE_LIST      0x88888888

typedef struct _HBM_MEMORY_MAP_MC_DATA {
  INT32          TadOffset[MAX_TAD_RULES_10NM];   // Corresponding TAD offsets (per channel)
  TAD_TABLE      TadTable[MAX_TAD_RULES_10NM];    // TAD structure
  UINT32         RemainingChannelCapacity;        // Remaining capacity of each HBM channel in units of 64MB.
} HBM_MEMORY_MAP_MC_DATA;

//
// Memory map private structure for per-socket HBM data.
//
typedef struct _HBM_MEMORY_MAP_SOCKET_CONFIG {
  HBM_STACK_INFO           HbmStackInfo;
  UINT8                    StackCount;         //  Number of enabled stacks. Equal to the number set bits in StackEnableMask.
  UINT32                   SizePerStack;       //  Size of each stack expressed in units of 64MB.
  CONST UINT8              *StackRouting;      //  Pointer to the array of MAX_CLUSTERS for routing the stacks to clusters.
  UINT32                   ChannelCapacity;    //  Total capacity of each HBM channel in units of 64MB.
  HBM_MEMORY_MAP_MC_DATA   McHbm[MAX_HBM_IO];  //  HBM TAD data per MC
} HBM_MEMORY_MAP_SOCKET_CONFIG;

//
// Memory map private structure for per-socket HBM data.
//
typedef struct _HBM_MEMORY_MAP_CONFIG {
  UINT8                        NumberOfSockets;          // Number of sockets in the system.
  HBM_MEMORY_MAP_SOCKET_CONFIG HbmSocketData[MAX_SOCKET];
} HBM_MEMORY_MAP_CONFIG;


/**
 *
 * MemDecode lib API routines
 *
 **/

/**

  Called by lib external to MemDecodeLib to create SAD entries.

  @param[in]      MemMapData            Pointer to the memory map private data structure.
  @param[in]      SocketNum             Socket id.
  @param[in]      MemType               Type of memory that the interleave request corresponds to.
  @param[in]      McWays                Number of IMC's within socket participating in the interleave.
  @param[in]      ChWays                Number of channels between IMC's within socket participating in the interleave.
  @param[in]      TgtInterBitmap        Bit map of stacks participating in the interleave.
  @param[in]      NmTgtInterBitmap      Bit map of stacks participating in the interleave as near memory.
  @param[in]      ChannelInterBitmap    Per IMC Bit map of channels participating in the interleave.
  @param[in]      FmChannelInterBitmap  Per IMC Bit map of channels participating in the interleave.
  @param[in]      SadIndex              Index of the available SAD table entry to fill with data.
  @param[in,out]  InterleaveLimit       Total interleave address limit.
  @param[in,out]  Cluster               SNC cluster this SAD applies to (ignored if SNC is disabled).

  @retval     0 SUCCESS
  @retval     1 FAILURE

**/
UINT32
EFIAPI
AddSadEntries10nmHl (
  IN     MEMORY_MAP_DATA *MemMapData,
  IN     UINT8           SocketNum,
  IN     UINT16          MemType,
  IN     UINT8           McWays,
  IN     UINT8           ChWays,
  IN     UINT8           TgtInterBitmap,
  IN     UINT8           NmTgtInterBitmap,
  IN     UINT8           ChannelInterBitmap[MAX_IMC],
  IN     UINT8           FmChannelInterBitmap[MAX_IMC],
  IN     UINT8           SadIndex,
  IN OUT UINT32          *InterleaveLimit,
  IN OUT UINT8           Cluster
  );

/**

  Called by lib external to MemDecodeLib to get SAD base.

  @param[in]  MemMapData  Pointer to the memory map private data structure.
  @param[in]  SocketNum   Socket id.
  @param[in]  SadIndex    Index of the available SAD table entry to fill data in.

  @return    The limit of the previous SAD

**/
UINT32
EFIAPI
GetSadBase10nmHl (
  IN     MEMORY_MAP_DATA    *MemMapData,
  IN        UINT8           SocketNum,
  IN        UINT8           SadIndex
  );

/**
  Deliver Route table data to the CHAs

  @param[in]  RtType     - Route table type.
  @param[in]  Rt         - Route table data.
  @param[in]  Socket     - Socket Id.
  @param[in]  Cluster    - Cluster ID in socket, only 0 if socket is not clustered.
  @param[in]  TotCluster - Total number of clusters enabled in a socket.
  @param[in]  HalfBitmap - Map of the halfs route table in use.

  @retval None

**/
VOID
EFIAPI
WriteRouteTables (
  IN ROUTE_TABLE_TYPE  RtType,
  IN UINT32            Rt[][MaxRtHalves],
  IN UINT8             Socket,
  IN UINT8             Cluster,
  IN UINT8             TotCluster,
  IN UINT8             HalfBitmap
  );

/**
  Print out a formatted representation of the input Route Table data.

  @param[in] ChRt        - Channel Route Table Data
  @param[in] TgtRt       - Target Route Table Data
  @param[in] RtNumString - String to append to Route Table name
  @param[in] Cluster     - The cluster that the route table belongs to

  @retval None

**/
VOID
EFIAPI
PrintRouteTable (
  IN UINT32  ChRt[MaxRtHalves],  OPTIONAL
  IN UINT32  TgtRt[MaxRtHalves], OPTIONAL
  IN UINT8   *RtNumString,
  IN UINT8   Cluster
  );
#endif // _HBM_DECODE_H_
