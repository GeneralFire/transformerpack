/** @file

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

#ifndef _MEM_DECODE_LIB_H_
#define _MEM_DECODE_LIB_H_

#include "MemHostChipCommon.h"
#include <MemDecodeCommonIncludes.h>

/**

Compute contents for memory map CSRs to allocate physical address space to sockets,
channels, and dimm ranks. Writes necessary memory map CSRs. Displays memory map
configuration for debug.

@param Host - Pointer to sysHost, the system Host (root) structure struct

@retval SUCCESS

**/
UINT32
EFIAPI
InitMemoryMap (
  IN PSYSHOST Host
  );

/**

  Find the MC index to use for calculating channel ways.
  imcInterBitmap must be a non-zero value in input. Routine assumes value has BIT0 and/or BIT1 set.

  @param imcInterBitmap : bitmap of IMCs in the interleave.
  @retval IMC number to use for calculating channel ways

**/
UINT8
EFIAPI
GetMcIndexFromBitmap (
  UINT8 imcInterBitmap
  );

/**

  Get the number of Channel Ways from the input SAD_TABLE Entry

  @param SadEntry - Pointer to the SAD Table entry to process

  @return The number of channel ways for the input SadEntry

**/
UINT8
EFIAPI
GetChannelWays (
  SAD_TABLE *SadEntry
  );

/**
  Get SGX memory map PRMRR data after memory map flow.

  This function is invoked by SGX module after memory map flow.
  It abstracts SGX requirement on system memory topology:
  1. If PrmrrSize field in CPUSetup is zero, memory map flow will calculate all supported PRMRR size based on existing
     configuration. SGX module can based on this info and CPU capability to configure the correct PRMRR size after reboot.
  2. If PrmrrSize field in CPUSetup is not zero, memory map flow will allocate and reserve the PRMRR size in system memory map.
     SGX module can depend on this output data structure to program the CPU registers to enable SGX CPU feature.

  Caller is responsible for allocating the output SGX_PRMRR_DATA data structure.

  @retval EFI_INVALID_PARAMETER    SgxMemMapData is NULL.
  @retval EFI_NOT_READY            This function is invoked before memory map flow.
  @retval EFI_NOT_READY            This function is not available for current memory decode SOC library.
  @retval EFI_UNSUPPORTED          Current memory topology is not supported SGX capable configuration.
  @retval EFI_INVALID_PARAMETER    Input PrmrrSize field in CPUSetup structure is not-zero and it is not a power of two.
                                   The supported PRMRR bit map is returned in ValidPrmrrBitMap field in SGX_PRMRR_DATA.
  @retval OUT_OF_RESOURCES         Memory map flow fails to reserve size indicated by PRMRR size, but the supported PRMRR
                                   bit map is returned in ValidPrmrrBitMap field in SGX_PRMRR_DATA.
  @retval EFI_SUCCESS              If the PrmrrSize field in CPUSetup is 0, the supported PRMRR bit map is returned in
                                   ValidPrmrrBitMap field in SGX_PRMRR_DATA.
  @retval EFI_SUCCESS              If the PrmrrSize field in CPUSetup is not 0, memory map flow has successfully reserved
                                   PrmrrSize SGX memory for SGX requirement.

**/
EFI_STATUS
EFIAPI
GetSgxPrmrrData (
  OUT SGX_PRMRR_DATA   *SgxMemMapData
  );


/**
  Get TDX memory map SEAMRR data after memory map flow.

  This function is invoked by TDX module after memory map flow. It abstracts TDX requirement on system memory topology.
  Memory map flow will assign the appropriate TDX SEAMRR region based on current memory topology if SecurityPolicy->EnableTdx
  is enabled.

  Caller is responsible for allocating the output TDX_SEAMRR_DATA data structure.

  @retval EFI_INVALID_PARAMETER    TdxMemMapData is NULL.
  @retval EFI_NOT_READY            This function is invoked before memory map flow.
  @retval OUT_OF_RESOURCES         Memory map flow fails to allocate TDX SEAMRR region.
  @retval EFI_SUCCESS              TDX SEAMRR region is allocated successfully in output TdxMemMapData.

**/
EFI_STATUS
EFIAPI
GetTdxSeamrrData (
  OUT TDX_SEAMRR_DATA   *TdxMemMapData
  );

/**

  Encode TAD channel "ways" for TAD rule CSR in imc (memory controller)

  @param[in] ways  - ways to encode

  @retval Encoded ways

**/
UINT8
EFIAPI
ImcTadEncodeChWays (
  IN UINT8 ways
  );

/**

  Encode TAD socket "ways" for TAD rule CSR.

  @param[in] ways  - ways to encode

  @retval Encoded ways

**/
UINT8
EFIAPI
TadEncodeSkWays (
  IN UINT8 ways
  );

/** Encode channel interleave granularity.

  @param[in]  Granularity - Channel granularity

  @retval     -      Encoded channel granularity.

**/
UINT8
EFIAPI
EncodeChannelInterleaveGranularity (
  IN UINT8  Granularity
  );

/**

  Encode MC/TAD target interleave granularity

  @param[in] TgtGranularity  Specifies the MC granularity

  @retval Encoded granularity

**/
UINT8
EFIAPI
EncodeTadTargetInterleaveGranularity (
  IN UINT8 TgtGranularity
  );

/**

  Calculate the TAD Offset value which will ultimately be used to program the
  MC TADCHNILVOFFSET "tad_offset" and "tad_offset_sign" register fields.
  The calculated TAD Offset value may be a negative 2'complement integer value.

  @param[in] Mirrored       - Indicates if Mirroring is enabled
  @param[in] SocketWays     - Socket Interleave ways for the TAD entry
  @param[in] ChannelWays    - The Channel Interleave ways for the corresponding SAD
  @param[in] ChannelMemUsed - The total channel size of each channel participating
  in the interleave
  @param[in] TadLimit       - Limit of the TAD entry

  @return The TAD Offset

**/
INT32
EFIAPI
CalculateTadOffset (
  IN BOOLEAN Mirrored,
  IN UINT8   SocketWays,
  IN UINT8   ChannelWays,
  IN UINT32  ChannelMemUsed,
  IN UINT32  TadLimit
  );
/**

  Encode Limit field for all DRAM rules.
  Limit is encoded such that valid address range <= Limit.

  @param Limit - Limit to encode

  @retval Encoded Limit

**/
UINT32
EFIAPI
SadTadEncodeLimit (
  UINT32 Limit
  );

/**

  Encode RIR "ways" for RIR rule CSR.

  @param ways  - ways to encode

  @retval Encoded ways

**/
UINT8
EFIAPI
RirEncodeRirWays (
  UINT8 ways
  );

/**
  Get the TAD Rule Target Granularity for the input SadIndex.

  @param[in]  MemMapData   Pointer to the memory map private data structure.
  @param[in]  Sckt         The socket index for the input SadIndex.
  @param[in]  SadIndex     The SadIndex corresponding to the TAD rule.
  @param[out] TargetGran   Pointer to a variable where the Target Granularity will be retuned.

  @retval SUCCESS          The Target Granularity was successfully calculated.
  @retval FAILURE          The operation failed due to an error.
**/
UINT32
EFIAPI
GetTadRuleTargetGranularity (
  IN     MEMORY_MAP_DATA    *MemMapData,
  IN     UINT8              Sckt,
  IN     UINT8              SadIndex,
     OUT UINT8              *TargetGran
  );

/**
  Get near memory patrol sparing range.

  @param[in]  Host        Pointer to the sysHost structure.
  @param[in]  MemMapData  Pointer to memory map private data structure.
  @param[in]  SocketId    Socket number.
  @param[in]  McIndex     Memory controller index.
  @param[in]  SadIndex    Sad table index
  @param[out] PtrPsBase   Pointer to output UINT32 buffer for patrol sparing range.
  @param[out] PtrPsLimit  Pointer to output UINT32 buffer for patrol sparing limit.

**/
VOID
GetNmPsRange (
  IN     SYSHOST          *Host,
  IN     MEMORY_MAP_DATA  *MemMapData,
  IN     UINT8            SocketId,
  IN     UINT8            McIndex,
  IN     UINT8            SadIndex,
     OUT UINT32           *PtrPsBase,
     OUT UINT32           *PtrPsLimit
  );
#endif //_MEM_DECODE_LIB_H_
