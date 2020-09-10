/** @file
  MemDecodeRas.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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
#include "MemHostChipCommon.h"
#include "SysHost.h"
#include <Library/MemDecodeLib.h>
#include <Library/RasRcMirrorLib.h>
#include <MemDecodeCommonIncludes.h>

#ifndef _MEM_DECODE_RAS_H_
#define _MEM_DECODE_RAS_H_

#define RAS_SKIP_CHANNEL                          0
#define SAD0INDEX                                 0
#define SAD_NOT_ENABLED                           0
#define SAD_NOT_ELIGIBLE_MIRROR                   1
#define SAD_ELIGIBLE_MIRROR                       2
#define MIRROR_STATUS_MIRROR_INCAPABLE            1
#define MIRROR_STATUS_INVALID_REQUEST             3
#define MIRROR_STATUS_UNSUPPORTED_CONFIG          4

//SAD with memory hole and limit of 4GB. 4 << 4 is actually (4 << 30) >> 26. i.e., 4GB >> 26.
#define SAD_4GB   (4 << 4)

/**

Check if two channels in a IMC is capable for mirroring by checking if DIMMs
populated in all of those channels are good for mirroring mode

@param Host  - Pointer to sysHost
@param socket  - Socket number
@param imc   - IMC number

@retval RAS_IS_CAPABLE
@retval RAS_NOT_CAPABLE

**/
UINT8
CheckMirrorPopulation2 (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Imc,
  UINT8    Ch0,        // at Imc level
  UINT8    Ch1         // at Imc level
  );

/**

Routine figures out if a SAD can be mirrored

@param Host      - Pointer to sysHost
@param SAD       - Pointer to the Sad Table
@param SadIndex  - Index into the Sad Table

@retval 0 Sad not local or not enabled
@retval 1 Sad can not be mirrored
@retval 2 Sad can be mirrored
**/
UINT8
CheckSadForMirroring (
  PSYSHOST  Host,
  SAD_TABLE *SadTablePtr,
  UINT8     SadIndex
  );

/**
  Calculate if enough memory is available to create
  the mirror region completely.

  @param[in] Host             Pointer to sysHost.
  @param[in] MemMapData       Pointer to the memory map private data structure.
  @param[in] MirrorSize       Mirror size to verify.

  @retval SUCCESS

**/
BOOLEAN
CalculateMirrorMem (
  IN  PSYSHOST          Host,
  IN  MEMORY_MAP_DATA   *MemMapData,
  IN  UINT32            MirrorSize
  );


/**
  Check if a IMC is capable for mirroring by checking if DIMMs populated in all of its
  channels is good for mirroring mode.

  @param[in] Host             Pointer to sysHost.
  @param[in] MemMapData       Pointer to the memory map private data structure.
  @param[in] Socket           Socket number.

  @retval RAS_IS_CAPABLE
  @retval RAS_NOT_CAPABLE

**/
UINT8
CheckMirrorPopulation (
  IN PSYSHOST          Host,
  IN MEMORY_MAP_DATA   *MemMapData,
  IN UINT8             Socket
  );

/**

Adjust memory address map for mirroring
Routine figures out the adjustment required in SAD, SAD2TAD, TAD tables.

@param Host       - Pointer to sysHost, the system Host (root) structure struct
@param MemMapData - Pointer to memory map private data structure

@retval SUCCESS

**/

UINT32
AdjustMemAddrMapForMirror (
  PSYSHOST        Host,
  MEMORY_MAP_DATA *MemMapData,
  UINT8           SizeAdjustmentForSnc[MAX_SOCKET][MAX_DRAM_CLUSTERS * MAX_SAD_RULES]
  );

/**
Gets memory below 4GB

@param[in]   Host   Pointer to sysHost

@retval Tolm

**/
UINT32
GetMemBlocksBelow4GB (
  PSYSHOST  Host
  );

/**
  After mirroring, this call will adjust the memory size fields to accurately reflect the
  value of the reduced memory in system.

  @param[in] Host                  Pointer to sysHost.
  @param[in] MemMapData            Pointer to the memory map private data structure.
  @param[in] SizeAdjustmentForSnc  Memory size need to adjust for SNC.

  @retval SUCCESS

**/
UINT32
AdjustMemorySizeFieldsforMirror (
  IN PSYSHOST          Host,
  IN MEMORY_MAP_DATA   *MemMapData,
  IN UINT8             SizeAdjustmentForSnc[MAX_SOCKET][MAX_DRAM_CLUSTERS * MAX_SAD_RULES]
  );
#endif //_MEM_DECODE_RAS_H_
