/** @file
  MemDramAccess.c
  Functions needed to send commands to DDR5 DRAMs

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include "MemDramAccess.h"
#include "Ddr5Core.h"

//
// There are 256 mode registers defined by JEDEC
//
#define MAX_DRAM_MODE_REGISTERS   256

MRC_STATUS
WriteDramModeRegCachePerDeviceWorker (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubChMask,
  IN  UINT8       Dimm,
  IN  UINT8       Dram,
  IN  UINT8       Rank,
  IN  UINT8       Address,
  IN  UINT8       Data
  );

MRC_STATUS
ReadDramModeRegCachePerDeviceWorker (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubCh,
  IN  UINT8       Dimm,
  IN  UINT8       Dram,
  IN  UINT8       Rank,
  IN  UINT8       Address,
  OUT UINT8       *Data
  );

/**
  Initialize the DRAM MRW cache for a single socket
  Initialization consists of clearing all entries and valid bits to 0 and
  setting the MRW Address fields using the list of MRs that are to be cached.

  @param[in,out] MrwCache   - Pointer to the MRW cache for the socket

  @retval MRC_STATUS_SUCCESS if the cache is successfully initialized
          MRC_STATUS_FAILURE otherwise
**/
MRC_STATUS
InitMrwCache (
  IN OUT  SOCKET_MRW_CACHE  *MrwCache
  )
{
  UINT8                     Index;
  UINT8                     ListLength;
  UINT8                     AvailableSize;
  UINT8                     RequiredSize;
  UINT8                     MrwRankList[]           = {MRW_RANK_CACHE_LIST};
  UINT8                     MrwDevicex4x8x16List[]  = {MRW_DEVICE_CACHE_LIST_X4X8X16};
  UINT8                     MrwDevicex8x16List[]    = {MRW_DEVICE_CACHE_LIST_X8X16};
#if defined (X16_DEVICE_CACHE_SUPPORTED) && (X16_DEVICE_CACHE_SUPPORTED == 1)
  UINT8                     MrwDevicex16List[]      = {MRW_DEVICE_CACHE_LIST_X16};
#endif // X16_DEVICE_CACHE_SUPPORTED

  if (MrwCache == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_105);
    return MRC_STATUS_FAILURE;
  }

  ZeroMem (MrwCache, sizeof (SOCKET_MRW_CACHE));

  //
  // For each cache entry, initialize the Address field
  //
  // Prevent buffer overruns and display a warning if there is a mismatch
  // between the number of MRs that can be cached vs the number of MRs
  // that need to be cached. This is not fatal.
  // WriteDramModeRegCachePer* will call RC_FATAL_ERROR if it is called with an MR address
  // that does not have a slot in the cache.
  //
  RequiredSize  = ARRAY_SIZE (MrwRankList);
  AvailableSize = ARRAY_SIZE (MrwCache->ByRank);
  ListLength    = MIN (RequiredSize, AvailableSize);
  if (AvailableSize != RequiredSize) {
    //
    // Length of MRW_RANK_CACHE_LIST does not match MRW_RANK_CACHE_LENGTH
    //
    RcDebugPrint (SDBG_ERROR, "Warning - Per-rank MRW cache list mismatch - Available size: %d, Required size: %d\n",
      AvailableSize, RequiredSize);
  }
  for (Index = 0; Index < ListLength; Index++) {
    MrwCache->ByRank[Index].Address = MrwRankList[Index];
  } // Index

  RequiredSize  = ARRAY_SIZE (MrwDevicex4x8x16List);
  AvailableSize = ARRAY_SIZE (MrwCache->ByDevice.Devicesx4x8x16);
  ListLength    = MIN (RequiredSize, AvailableSize);
  if (AvailableSize != RequiredSize) {
    //
    // Length of MRW_DEVICE_CACHE_LIST_X4X8X16 does not match MRW_DEVICE_CACHE_X4X8X16_LENGTH
    //
    RcDebugPrint (SDBG_ERROR, "Warning - Per-device ALL MRW cache list mismatch - Available size: %d, Required size: %d\n",
      AvailableSize, RequiredSize);
  }
  for (Index = 0; Index < ListLength; Index++) {
    MrwCache->ByDevice.Devicesx4x8x16[Index].Address = MrwDevicex4x8x16List[Index];
  } // Index

  RequiredSize  = ARRAY_SIZE (MrwDevicex8x16List);
  AvailableSize = ARRAY_SIZE (MrwCache->ByDevice.Devicesx8x16);
  ListLength    = MIN (RequiredSize, AvailableSize);
  if (AvailableSize != RequiredSize) {
    //
    // Length of MRW_DEVICE_CACHE_LIST_X8X16 does not match MRW_DEVICE_CACHE_X8X16_LENGTH
    //
    RcDebugPrint (SDBG_ERROR, "Warning - Per-device x8x16 MRW cache list mismatch - Available size: %d, Required size: %d\n",
      AvailableSize, RequiredSize);
  }
  for (Index = 0; Index < ListLength; Index++) {
    MrwCache->ByDevice.Devicesx8x16[Index].Address = MrwDevicex8x16List[Index];
  } // Index

#if defined (X16_DEVICE_CACHE_SUPPORTED) && (X16_DEVICE_CACHE_SUPPORTED == 1)
  RequiredSize  = ARRAY_SIZE (MrwDevicex16List);
  AvailableSize = ARRAY_SIZE (MrwCache->ByDevice.Devicesx16);
  ListLength    = MIN (RequiredSize, AvailableSize);
  if (AvailableSize != RequiredSize) {
    //
    // Length of MRW_DEVICE_CACHE_LIST_X16 does not match MRW_DEVICE_CACHE_X16_LENGTH
    //
    RcDebugPrint (SDBG_ERROR, "Warning - Per-device x16 MRW cache list mismatch - Available size: %d, Required size: %d\n",
      AvailableSize, RequiredSize);
  }
  for (Index = 0; Index < ListLength; Index++) {
    MrwCache->ByDevice.Devicesx16[Index].Address = MrwDevicex16List[Index];
  } // Index
#endif  // X16_DEVICE_CACHE_SUPPORTED

  MrwCache->State.Bits.Initialized = 1;

  return MRC_STATUS_SUCCESS;
}

/**
  Obtain pointer to DRAM MRW cache for a socket

  @param[in]  Host    - Pointer to SysHost
  @param[in]  Socket  - Socket number

  @retval Pointer to DRAM_NVDATA.MrwCache[Socket]
**/
SOCKET_MRW_CACHE
*GetMrwCache (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  DRAM_NVDATA       *DramNvData;
  SOCKET_MRW_CACHE  *MrwCache;

  DramNvData = (VOID *)Host->nvram.mem.DramNvdata;
  MrwCache = &(DramNvData->MrwCache[Socket]);

  //
  // If the cache has not been initialized for this socket, do it now
  //
  if (MrwCache->State.Bits.Initialized == 0) {
    InitMrwCache (MrwCache);
  }
  return (MrwCache);
}

/**
  Validate the DRAM data structure backing store

  @retval MRC_STATUS_FAILURE if not enough memory allocated for the buffer
          MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
          MRC_STATUS_SUCCESS otherwise
**/
MRC_STATUS
EFIAPI
ValidateDramDataStucture (
  VOID
  )
{
  PSYSHOST                  Host;
  UINT32                    RequiredSize;
  UINT32                    AllocatedSize;

  Host = GetSysHostPointer ();

  RequiredSize = sizeof (DRAM_NVDATA);
  AllocatedSize = sizeof (Host->nvram.mem.DramNvdata);
  //
  // Verify sufficient memory has been allocated for the DDRIO_NVDATA structure
  //
  if (AllocatedSize < RequiredSize) {
    RcDebugPrint (SDBG_ERROR, "ERROR: Space allocated for DramNvdata(%d) < Space required by DRAM_NVDATA(%d)\n", AllocatedSize, RequiredSize);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_106);
    return MRC_STATUS_FAILURE;
  }

  //
  // If there is more space allocated for DRAM_NVDATA than is needed, NEM and NVRAM storage space are being wasted
  //
  if (AllocatedSize > RequiredSize) {
    RcDebugPrint (SDBG_MAX, "Warning - wasted memory: Space allocated for DramNvdata(%d) > Space required by DRAM_NVDATA(%d)\n", AllocatedSize, RequiredSize);
  }

  return MRC_STATUS_SUCCESS;
}

/**
  The byte and bit positions into the Valid array for Ch, SubCh, Dimm and Rank.

  @param[in]  Ch                - Channel number (80-bit channel)
  @param[in]  SubCh             - Subchannel number
  @param[in]  Dimm              - DIMM number within the channel
  @param[in]  Rank              - Rank number within the DIMM
  @param[out] BitInByte         - Bit mask for the Valid array byte
  @param[out] ByteOffsetInArray - Byte offset within the Valid array

  @retval MRC_STATUS_SUCCESS - Valid bit offset is found
  @retval MRC_STATUS_FAILURE - Array index is invalid
**/
MRC_STATUS
GetMrwCacheEntryValidBitAndBtyeOffsets (
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  OUT UINT8     *BitInByte,
  OUT UINT16    *ByteOffsetInArray
  )
{
  UINT16    BitOffsetInArray;

  //
  // Calculate the Valid bit position in the array for Ch, SubCh, Dimm and Rank
  //
  BitOffsetInArray = (Ch * VALID_BIT_CH_OFFSET) +
                     (SubCh * VALID_BIT_SUB_CH_OFFSET) +
                     (Dimm * VALID_BIT_DIMM_OFFSET) +
                     (Rank * VALID_BIT_RANK_OFFSET);

  //
  // Calculate the byte within the Valid array that contains the Valid bit for Ch, SubCh, Dimm and Rank
  //
  *ByteOffsetInArray = BitOffsetInArray / (MAX_BITS_IN_BYTE);

  if (*ByteOffsetInArray >= SIZE_VALID_ARRAY) {
    //
    // The array index is out of bounds
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_109);
    return MRC_STATUS_FAILURE;
  }
  *BitInByte = 1 << (BitOffsetInArray % (MAX_BITS_IN_BYTE));
  return MRC_STATUS_SUCCESS;
}

/**
  Return TRUE if the Valid bit is set for the mode register instance
  specified by Ch, SubCh, Dimm and Rank.
  This indicates the cache entry has been updated at least once.

  @param[in]  Valid     - Valid arrary
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - Subchannel number
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Rank      - Rank number within the DIMM

  @retval TRUE  - Valid bit is set
  @retval FALSE - Valide bit is not set
**/
BOOLEAN
IsMrwCacheEntryValid (
  IN  UINT8     Valid[SIZE_VALID_ARRAY],
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT8     Dimm,
  IN  UINT8     Rank
  )
{
  MRC_STATUS  Status;
  UINT16      ByteOffsetInArray;
  UINT8       BitInByte;

  Status = GetMrwCacheEntryValidBitAndBtyeOffsets (Ch, SubCh, Dimm, Rank, &BitInByte, &ByteOffsetInArray);

  if (Status == MRC_STATUS_SUCCESS) {

    if ((Valid[ByteOffsetInArray] & BitInByte) == 0) {
      return FALSE;
    }
    return TRUE;
  }

  return FALSE;

}

/**
  Set the Valid bit for the mode register instance
  specified by Ch, SubCh, Dimm and Rank

  @param[in]  Valid     - Valid arrary
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - Subchannel number
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Rank      - Rank number within the DIMM

  @retval N/A
**/
VOID
SetMrwCacheEntryValid (
  IN  UINT8     Valid[SIZE_VALID_ARRAY],
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT8     Dimm,
  IN  UINT8     Rank
  )
{
  MRC_STATUS  Status;
  UINT16      ByteOffsetInArray;
  UINT8       BitInByte;

  Status = GetMrwCacheEntryValidBitAndBtyeOffsets (Ch, SubCh, Dimm, Rank, &BitInByte, &ByteOffsetInArray);

  if (Status == MRC_STATUS_SUCCESS) {

    Valid[ByteOffsetInArray] |= BitInByte;
  }
}

/**
  Return the cached value of the per-rank MR specified by Address.
  If the MR is in the cache list, return the contents in the caller's Data buffer.
  If the MR is not in the cache list, do not modify the caller's Data buffer.

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - Subchannel number
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Rank      - Rank number within the DIMM
  @param[in]  Address   - The MRW address
  @param[out] Data      - Pointer to caller's buffer

  @retval MRC_STATUS_SUCCESS cached data is returned
  @retval MRC_STATUS_FAILURE if the mode register is not in the cache list
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
ReadDramModeRegCachePerRank (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubCh,
  IN  UINT8       Dimm,
  IN  UINT8       Rank,
  IN  UINT8       Address,
  OUT UINT8       *Data
  )
{
  PSYSHOST                    Host;
  MRC_STATUS                  Status;
  UINT8                       Index;
  SOCKET_MRW_CACHE            *MrwCache;

  Host = GetSysHostPointer ();
  MrwCache = GetMrwCache (Host, Socket);

  Status = MRC_STATUS_GROUP_NOT_SUPPORTED;
  //
  // Look for the mode register address in the per-rank cache
  // If is isn't there, try the per-device cache
  //
  for (Index = 0; Index < ARRAY_SIZE (MrwCache->ByRank); Index++) {
    if (MrwCache->ByRank[Index].Address != Address) {
      continue;
    }
    *Data = MrwCache->ByRank[Index].Cache[Ch][SubCh][Dimm][Rank].Data;
    Status = MRC_STATUS_SUCCESS;
    break;
  } // Index

  if (Status == MRC_STATUS_GROUP_NOT_SUPPORTED) {
    //
    // This MR is missing from the per-rank cache list. See if it is in the per-device list
    // If it is, use the value from Dram 0
    //
    Status = ReadDramModeRegCachePerDeviceWorker (Socket, Ch, SubCh, Dimm, 0, Rank, Address, Data);
  }

  if (Status == MRC_STATUS_GROUP_NOT_SUPPORTED) {
    //
    // This MR is missing from the cache
    // Update MRW_DEVICE_CACHE_LIST, MRW_DEVICE_CACHE_LENGTH and DRAM_DATA_1S to include the missing MR
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_110);
  }

  return Status;
}

/**
  Update the MRW cache for a per-rank MRW specified by Address

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubChMask - Mask of subchannel numbers to be updated
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Rank      - Rank number within the DIMM
  @param[in]  Address   - The MRW address
  @param[in]  Data      - New value to write to the cache

  @retval MRC_STATUS_SUCCESS if cache is updated
  @retval MRC_STATUS_FAILURE if the cache is not updated
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
**/
MRC_STATUS
EFIAPI
WriteDramModeRegCachePerRank (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubChMask,
  IN  UINT8       Dimm,
  IN  UINT8       Rank,
  IN  UINT8       Address,
  IN  UINT8       Data
  )
{
  PSYSHOST                    Host;
  MRC_STATUS                  Status;
  UINT8                       SubCh;
  UINT8                       Index;
  UINT8                       MaxChDdr;
  UINT8                       MaxRank;
  UINT8                       Strobe;
  UINT8                       MaxStrobeSubCh;
  SOCKET_MRW_CACHE            *MrwCache;

  Host        = GetSysHostPointer ();
  MaxChDdr    = GetMaxChDdr ();
  MaxRank     = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);

  if ((Socket >= MAX_SOCKET) ||
      (Ch     >= MaxChDdr)   ||
      (Dimm   >= MAX_DIMM)   ||
      (Rank   >= MAX_RANK_DIMM))
  {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "WARNING - WriteDramModeRegCachePerRank called with an invalid parameter\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_108);
    return MRC_STATUS_FAILURE;
  }

  MrwCache = GetMrwCache (Host, Socket);
  Status = MRC_STATUS_FAILURE;

  for (Index = 0; Index < ARRAY_SIZE (MrwCache->ByRank); Index++) {
    if (MrwCache->ByRank[Index].Address != Address) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      if (((1 << SubCh) & SubChMask) != 0) {
        MrwCache->ByRank[Index].Cache[Ch][SubCh][Dimm][Rank].Data = Data;
        SetMrwCacheEntryValid (MrwCache->ByRank[Index].Valid, Ch, SubCh, Dimm, Rank);
        Status = MRC_STATUS_SUCCESS;
      }
    } // SubCh
    break;
  } // Index

  if (Status == MRC_STATUS_FAILURE) {
    //
    // This MRW is missing from the per-rank cache, but it may be in the per-device cache
    //
    MaxStrobeSubCh = GetSubChMaxStrobeValid (Host);
    if (IsX4Dimm (Socket, Ch, Dimm) == FALSE) {
      MaxStrobeSubCh = MaxStrobeSubCh / 2;
    }
    for (Strobe = 0; Strobe < MaxStrobeSubCh; Strobe++) {
      Status = WriteDramModeRegCachePerDeviceWorker (Socket, Ch, SubChMask, Dimm, Strobe, Rank, Address, Data);
      if (Status != MRC_STATUS_SUCCESS) {
        break;
      }
    } // Strobe
  }
  if (Status == MRC_STATUS_FAILURE) {
    //
    // This MRW is missing from the cache
    // Update MRW_RANK_CACHE_LIST, MRW_RANK_CACHE_LENGTH and DRAM_DATA_1S to include the missing MRW
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_110);
  }
  return Status;
}

/**
  Return the cached value of the per-device MR specified by Address.
  If the MR is in the cache list, return the contents in the caller's Data buffer.
  If the MR is not in the cache list, do not modify the caller's Data buffer.
  Do not ASSERT if the MR is not in the cache list.

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - Subchannel number
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Dram      - Dram device number (sub-channel-based)
  @param[in]  Rank      - Rank number within the DIMM
  @param[in]  Address   - The MRW address
  @param[out] Data      - Pointer to caller's buffer

  @retval MRC_STATUS_SUCCESS if cached data is returned
  @retval MRC_STATUS_FAILURE if the MR is not in the cache list

**/
MRC_STATUS
EFIAPI
ReadDramModeRegCachePerDeviceWorker (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubCh,
  IN  UINT8       Dimm,
  IN  UINT8       Dram,
  IN  UINT8       Rank,
  IN  UINT8       Address,
  OUT UINT8       *Data
  )
{
  PSYSHOST                    Host;
  MRC_STATUS                  Status;
  UINT8                       Index;
  UINT8                       MaxChDdr;
  UINT8                       MaxRank;
  UINT8                       SubChMSVx4;
  UINT8                       SubChMSVx8;
  SOCKET_MRW_CACHE            *MrwCache;

  Host        = GetSysHostPointer ();
  MaxChDdr    = GetMaxChDdr ();
  MaxRank     = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);
  SubChMSVx4  = GetSubChMaxStrobeValid (Host);
  SubChMSVx8  = SubChMSVx4 / 2;

  if ((Socket >= MAX_SOCKET) ||
      (Ch     >= MaxChDdr)   ||
      (Dimm   >= MAX_DIMM)   ||
      (Dram   >= SubChMSVx4) ||
      (Rank   >= MaxRank))
  {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, Dram, NO_BIT,
      "WARNING - ReadDramModeRegCachePerDeviceWorker called with an invalid parameter\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_108);
    return MRC_STATUS_FAILURE;
  }

  MrwCache = GetMrwCache (Host, Socket);
  Status = MRC_STATUS_GROUP_NOT_SUPPORTED;
  //
  // Check for registers that apply to all devices
  //
  for (Index = 0; Index < ARRAY_SIZE (MrwCache->ByDevice.Devicesx4x8x16); Index++) {
    if (MrwCache->ByDevice.Devicesx4x8x16[Index].Address != Address) {
      continue;
    }
    *Data = MrwCache->ByDevice.Devicesx4x8x16[Index].Cache[Ch][SubCh][Dimm][Rank][Dram].Data;
    Status = MRC_STATUS_SUCCESS;
    break;
  } // Index

  if ((Status != MRC_STATUS_SUCCESS) && (Dram < SubChMSVx8)) {
    //
    // Check for registers that are not cached for x4 devices
    //
    for (Index = 0; Index < ARRAY_SIZE (MrwCache->ByDevice.Devicesx8x16); Index++) {
      if (MrwCache->ByDevice.Devicesx8x16[Index].Address != Address) {
        continue;
      }
      *Data = MrwCache->ByDevice.Devicesx8x16[Index].Cache[Ch][SubCh][Dimm][Rank][Dram].Data;
      Status = MRC_STATUS_SUCCESS;
      break;
    } // Index
  }

#if defined (X16_DEVICE_CACHE_SUPPORTED) && (X16_DEVICE_CACHE_SUPPORTED == 1)
  if ((Status != MRC_STATUS_SUCCESS) && (Dram < SubChMSVx8)) {
    //
    // Check for registers that are only cached for x16 devices
    //
    for (Index = 0; Index < ARRAY_SIZE (MrwCache->ByDevice.Devicesx16); Index++) {
      if (MrwCache->ByDevice.Devicesx16[Index].Address != Address) {
        continue;
      }
      *Data = MrwCache->ByDevice.Devicesx16[Index].Cache[Ch][SubCh][Dimm][Rank][Dram].Data;
      Status = MRC_STATUS_SUCCESS;
      break;
    } // Index
  }
#endif  // X16_DEVICE_CACHE_SUPPORTED

  return Status;
} // ReadDramModeRegCachePerDeviceWorker

/**
  Return the cached value of the per-device MR specified by Address.
  If the MR is in the cache list, return the contents in the caller's Data buffer.
  If the MR is not in the cache list, do not modify the caller's Data buffer.
  ASSERT if the MR is not in the cache list.


  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - Subchannel number
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Dram      - Dram device number (sub-channel-based)
  @param[in]  Rank      - Rank number within the DIMM
  @param[in]  Address   - The MRW address
  @param[out] Data      - Pointer to caller's buffer

  @retval MRC_STATUS_SUCCESS if cached data is returned
  @retval MRC_STATUS_FAILURE if the MR is not in the cache list
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
ReadDramModeRegCachePerDevice (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubCh,
  IN  UINT8       Dimm,
  IN  UINT8       Dram,
  IN  UINT8       Rank,
  IN  UINT8       Address,
  OUT UINT8       *Data
  )
{
  MRC_STATUS                  Status;

  Status = ReadDramModeRegCachePerDeviceWorker (Socket, Ch, SubCh, Dimm, Dram, Rank, Address, Data);

  if (Status == MRC_STATUS_GROUP_NOT_SUPPORTED) {
    //
    // This MR is missing from the cache
    // Update MRW_DEVICE_CACHE_LIST, MRW_DEVICE_CACHE_LENGTH and DRAM_DATA_1S to include the missing MR
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_110);
  }

  return Status;
}

/**
  Update the MRW cache for a per-device MR specified by Address
  Do not ASSERT if the MR is not in the cache list

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubChMask - Mask of subchannel numbers to be updated
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Dram      - Dram device number (sub-channel-based)
  @param[in]  Rank      - Rank number within the DIMM
  @param[in]  Address   - The MRW address
  @param[in]  Data      - New value to write to the cache

  @retval MRC_STATUS_SUCCESS if cache is updated
  @retval MRC_STATUS_FAILURE if the cache is not updated

**/
MRC_STATUS
WriteDramModeRegCachePerDeviceWorker (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubChMask,
  IN  UINT8       Dimm,
  IN  UINT8       Dram,
  IN  UINT8       Rank,
  IN  UINT8       Address,
  IN  UINT8       Data
  )
{
  PSYSHOST                    Host;
  MRC_STATUS                  Status;
  UINT8                       SubCh;
  UINT8                       Index;
  UINT8                       MaxChDdr;
  UINT8                       MaxRank;
  UINT8                       SubChMSVx4;
  UINT8                       SubChMSVx8;
  SOCKET_MRW_CACHE            *MrwCache;

  Host        = GetSysHostPointer ();
  MaxChDdr    = GetMaxChDdr ();
  MaxRank     = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);
  SubChMSVx4  = GetSubChMaxStrobeValid (Host);
  SubChMSVx8  = SubChMSVx4 / 2;

  if ((Socket >= MAX_SOCKET) ||
      (Ch     >= MaxChDdr)   ||
      (Dimm   >= MAX_DIMM)   ||
      (Dram   >= SubChMSVx4) ||
      (Rank   >= MaxRank))
  {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, Dram, NO_BIT,
      "WARNING - WriteDramModeRegCachePerDeviceWorker called with an invalid parameter\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_107);
    return MRC_STATUS_FAILURE;
  }

  MrwCache = GetMrwCache (Host, Socket);
  Status = MRC_STATUS_FAILURE;

  //
  // Check for registers that apply to all devices
  //
  for (Index = 0; Index < ARRAY_SIZE (MrwCache->ByDevice.Devicesx4x8x16); Index++) {
    if (MrwCache->ByDevice.Devicesx4x8x16[Index].Address != Address) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      if (((1 << SubCh) & SubChMask) != 0) {
        MrwCache->ByDevice.Devicesx4x8x16[Index].Cache[Ch][SubCh][Dimm][Rank][Dram].Data = Data;
        SetMrwCacheEntryValid (MrwCache->ByDevice.Devicesx4x8x16[Index].Valid, Ch, SubCh, Dimm, Rank);
        Status = MRC_STATUS_SUCCESS;
      }
    } // SubCh
    break;
  } // Index

  if ((Status != MRC_STATUS_SUCCESS) && (Dram < SubChMSVx8)) {
    //
    // Check for registers that are not cached for x4 devices
    //
    for (Index = 0; Index < ARRAY_SIZE (MrwCache->ByDevice.Devicesx8x16); Index++) {
      if (MrwCache->ByDevice.Devicesx8x16[Index].Address != Address) {
        continue;
      }
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        if (((1 << SubCh) & SubChMask) != 0) {
          MrwCache->ByDevice.Devicesx8x16[Index].Cache[Ch][SubCh][Dimm][Rank][Dram].Data = Data;
          SetMrwCacheEntryValid (MrwCache->ByDevice.Devicesx8x16[Index].Valid, Ch, SubCh, Dimm, Rank);
          Status = MRC_STATUS_SUCCESS;
        }
      } // SubCh
      break;
    } // Index
  }

#if defined (X16_DEVICE_CACHE_SUPPORTED) && (X16_DEVICE_CACHE_SUPPORTED == 1)
  if ((Status != MRC_STATUS_SUCCESS) && (Dram < SubChMSVx8)) {
    //
    // Check for registers that are only cached for x16 devices
    //
    for (Index = 0; Index < ARRAY_SIZE (MrwCache->ByDevice.Devicesx16); Index++) {
      if (MrwCache->ByDevice.Devicesx16[Index].Address != Address) {
        continue;
      }
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        if (((1 << SubCh) & SubChMask) != 0) {
          MrwCache->ByDevice.Devicesx16[Index].Cache[Ch][SubCh][Dimm][Rank][Dram].Data = Data;
          SetMrwCacheEntryValid (MrwCache->ByDevice.Devicesx16[Index].Valid, Ch, SubCh, Dimm, Rank);
          Status = MRC_STATUS_SUCCESS;
        }
      } // SubCh
      break;
    } // Index
  }
#endif  // X16_DEVICE_CACHE_SUPPORTED

  return Status;
} // WriteDramModeRegCachePerDeviceWorker

/**
  Update the MRW cache for a per-device MRW specified by Address
  ASSERT if the MR is not in the cache list

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubChMask - Mask of subchannel numbers to be updated
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Dram      - Dram device number (sub-channel-based)
  @param[in]  Rank      - Rank number within the DIMM
  @param[in]  Address   - The MRW address
  @param[in]  Data      - New value to write to the cache

  @retval MRC_STATUS_SUCCESS if cache is updated
  @retval MRC_STATUS_FAILURE if the cache is not updated
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
WriteDramModeRegCachePerDevice (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubChMask,
  IN  UINT8       Dimm,
  IN  UINT8       Dram,
  IN  UINT8       Rank,
  IN  UINT8       Address,
  IN  UINT8       Data
  )
{
  MRC_STATUS                  Status;

  Status = WriteDramModeRegCachePerDeviceWorker (Socket, Ch, SubChMask, Dimm, Dram, Rank, Address, Data);

  if (Status == MRC_STATUS_FAILURE) {
    //
    // This MRW is missing from the cache
    // Update MRW_DEVICE_CACHE_LIST, MRW_DEVICE_CACHE_LENGTH and DRAM_DATA_1S to include the missing MRW
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_110);
  }
  return Status;
}

/**

  Send MRW Command to DRAM and update the MRW cache

  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (socket-based)
  @param[in] SubChMask  - Mask of subchannel numbers to be updated
  @param[in] Dimm       - DIMM number (Channel-based)
  @param[in] Rank       - Rank number (DIMM-based)
  @param[in] Address    - Address of the mode register
  @param[in] Data       - Data to write

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
DramModeRegWrite (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Address,
  IN UINT8    Data
  )
{
  PSYSHOST          Host;
  UINT32            Status;
  UINT8             MaxChDdr;
  UINT8             MaxRank;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];

  MaxChDdr    = GetMaxChDdr ();

  if ((Socket >= MAX_SOCKET) ||
      (Ch     >= MaxChDdr)   ||
      (Dimm   >= MAX_DIMM))
  {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "WARNING - DramModeRegWrite called with an invalid parameter\n");
    return MRC_STATUS_FAILURE;
  }

  Host        = GetSysHostPointer ();
  DimmNvList  = GetDimmNvList (Host, Socket, Ch);
  MaxRank     = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);

  if (Rank   >= MaxRank) {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "WARNING - DramModeRegWrite called with an invalid parameter\n");
    return MRC_STATUS_FAILURE;
  }

  Status = MRC_STATUS_FAILURE;

  if (SendMrw (Host, Socket, Ch, SubChMask, Dimm, Rank, Address, CW_DRAM, Data) == SUCCESS) {

    //
    // DDRT 2.0: Program direct snoop settings to DQ buffer
    //
    if ((*DimmNvList)[Dimm].fmcType == FMC_CWV_TYPE) {
      ProgramSnoopSettingsToDb (Host, Socket, Ch, SubChMask, Dimm, Rank, Address, Data);
    }

    WriteDramModeRegCachePerRank (Socket, Ch, SubChMask, Dimm, Rank, Address, Data);
    Status = MRC_STATUS_SUCCESS;
  }

  return Status;
}

/**

  This function sends MRR command and read MR value back from DIMM instead of from software caches,
  and update software caches if the reads is successful.
  The reason it only accesses hardware instead of software caches is to reflect to the current use scenario:
  a. dump dram mode registers
  b. read MR42 for supported DCA training assist mode
  c. issue MRR on MR46 and MR47 and DDRIO will snoop MRR value and record in Tx periodic retraining

  @param[in]  Socket          - Socket Id
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  SubChMask       - Sub-channel bit mask
  @param[in]  Dimm            - DIMM number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  Address         - Mode register Address
  @param[out] Data            - Data array to store the returned result for all dram devices

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise

  **/
MRC_STATUS
EFIAPI
DramModeRegRead (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        SubChMask,
  IN     UINT8        Dimm,
  IN     UINT8        Rank,
  IN     UINT8        Address,
     OUT UINT8        (*Data)[SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  PSYSHOST          Host;
  UINT8             MaxChDdr;
  UINT8             MaxRank;
  UINT8             SubCh;
  MRC_STATUS        Status = MRC_STATUS_FAILURE;
  UINT8             StrobeIndex;
  UINT8             SubChMSV;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];

  MaxChDdr    = GetMaxChDdr ();

  if ((Socket >= MAX_SOCKET) ||
      (Ch     >= MaxChDdr)   ||
      (Dimm   >= MAX_DIMM)   ||
      (Data   == NULL)) {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "Error - DramModeRegRead called with an invalid parameter\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_234);
    return MRC_STATUS_FAILURE;
  }

  Host        = GetSysHostPointer ();
  DimmNvList  = GetDimmNvList (Host, Socket, Ch);
  MaxRank     = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);

  if (Rank   >= MaxRank) {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "Error - DramModeRegRead called with an invalid parameter\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_234);
    return MRC_STATUS_FAILURE;
  }

  //
  // Read mode register value for all devices
  //
  if (ReadDramMr (Socket, Ch, SubChMask, Dimm, Rank, Address, Data) != MRC_STATUS_SUCCESS) {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "Error - DramModeRegRead runs into failure.\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_234);
    return MRC_STATUS_FAILURE;
  }

  //
  // Update cache
  //
  if (IsX4Dimm (Socket, Ch, Dimm)) {
    SubChMSV    = GetSubChMaxStrobeValid (Host);
  } else {
    SubChMSV    = GetSubChMaxStrobeValid (Host) / 2;
  }

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & SubChMask) == 0) {
      continue;
    }
    //
    // Check whether or not it's supported by per device cache
    //
    for (StrobeIndex = 0; StrobeIndex < SubChMSV; StrobeIndex++) {
      Status = WriteDramModeRegCachePerDeviceWorker(
                   Socket, Ch, 1 << SubCh, Dimm, StrobeIndex,
                   Rank, Address, (*Data)[SubCh][StrobeIndex]);
      if (Status != MRC_STATUS_SUCCESS) {
        //
        // This MR doesn't support per device cache. Use broadcast cache instead below.
        //
        break;
      }
    }
    //
    // Per rank based broadcast cache. Use dram 0 MR data for all drams.
    //
    if (Status != MRC_STATUS_SUCCESS) {
      Status = WriteDramModeRegCachePerRank (Socket, Ch, 1 << SubCh, Dimm, Rank, Address, (*Data)[SubCh][LOGICAL_STROBE_0]);
    }
  }
  return Status;
}

/**

  Send MRW PDA Command to DRAM and update the MRW cache

  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (socket-based)
  @param[in] SubChMask  - Mask of subchannel numbers to be updated
  @param[in] Dimm       - DIMM number (Channel-based)
  @param[in] Rank       - Rank number (DIMM-based)
  @param[in] Dram       - Dram device number (sub-channel-based)
  @param[in] Address    - Address of the mode register
  @param[in] Data       - Data to write

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
DramModeRegWritePda (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT8    Address,
  IN UINT8    Data
  )
{
  PSYSHOST    Host;
  UINT8       MaxChDdr;
  UINT8       MaxRank;
  UINT8       SubChMSVx4;

  Host        = GetSysHostPointer ();
  MaxChDdr    = GetMaxChDdr ();
  MaxRank     = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);
  SubChMSVx4  = GetSubChMaxStrobeValid (Host);

  if ((Socket >= MAX_SOCKET) ||
      (Ch     >= MaxChDdr)   ||
      (Dimm   >= MAX_DIMM)   ||
      (Dram   >= SubChMSVx4) ||
      (Rank   >= MaxRank))
  {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, Dram, NO_BIT,
      "WARNING - DramModeRegWritePda called with an invalid parameter\n");
    return MRC_STATUS_FAILURE;
  }
  SendMrwPda (Host, Socket, Ch, SubChMask, Dimm, Rank, Dram, Address, Data);
  WriteDramModeRegCachePerDevice (Socket, Ch, SubChMask, Dimm, Dram, Rank, Address, Data);
  return MRC_STATUS_SUCCESS;
}

/**

  Save or get NVRAM value for DRAM CS VREF or CA VREF

  @param[in]     Socket     - Socket Id
  @param[in]     Ch         - Channel number (socket-based)
  @param[in]     SubChMask  - Mask of subchannel numbers to be updated
  @param[in]     Dimm       - DIMM number (Channel-based)
  @param[in]     Rank       - Rank number (DIMM-based)
  @param[in]     Dram       - Dram number in the subchannel
  @param[in]     Group      - DramCsVref or DramCaVref
  @param[in,out] *Vref      - Data to save or get

  @retval MRC_STATUS_SUCCESS if operation is successful
  @retval MRC_STATUS_FAILURE otherwise
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
AccessSavedDramVref (
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    SubChMask,
  IN      UINT8    Dimm,
  IN      UINT8    Rank,
  IN      UINT8    Dram,
  IN      MRC_GT   Group,
  IN      UINT8    Operation,
  IN OUT  UINT16   *Vref
  )
{
  PSYSHOST          Host;
  UINT8             MaxChDdr;
  UINT8             SubCh;
  UINT8             MaxRank;
  UINT8             SubChMSV;
  UINT8             DramStart;
  UINT8             DramMax;
  UINT8             DramIndex;
  DRAM_NVDATA       *DramNvData;
  SOCKET_DRAM_VREF  *DramVref;

  Host        = GetSysHostPointer ();
  MaxChDdr    = GetMaxChDdr ();
  MaxRank     = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);
  SubChMSV    = GetSubChMaxStrobeValid (Host);

  if ((Socket >= MAX_SOCKET) ||
      (Ch     >= MaxChDdr)   ||
      (Dimm   >= MAX_DIMM)   ||
      (Rank   >= MaxRank)    ||
      ((Dram  >= SubChMSV) && (Dram != ALL_STROBES)) ||
      (Vref   == NULL))
  {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, Dram, NO_BIT,
      "WARNING - AccessSavedDramVref called with an invalid parameter\n");
    return MRC_STATUS_FAILURE;
  }

  DramNvData = (VOID *)Host->nvram.mem.DramNvdata;
  DramVref = &(DramNvData->DramVref[Socket]);

  if (Dram == ALL_STROBES) {
    DramStart = 0;
    DramMax = SubChMSV;
  } else {
    DramStart = Dram;
    DramMax = Dram + 1;
  }

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & SubChMask) == 0) {
      continue;
    }

    for (DramIndex = DramStart; DramIndex < DramMax; DramIndex++) {

      if (Operation == SAVE_DRAM_VREF) {
        switch (Group) {
          case DramCsVref:
            DramVref->CsVref[Ch][SubCh][Dimm][Rank][DramIndex] = *Vref;
            break;
          case DramCaVref:
            DramVref->CaVref[Ch][SubCh][Dimm][Rank][DramIndex] = *Vref;
            break;
          default:
            return MRC_STATUS_FAILURE;
            break;
        }
      } else {
        switch (Group) {
          case DramCsVref:
            *Vref = DramVref->CsVref[Ch][SubCh][Dimm][Rank][DramIndex];
            break;
          case DramCaVref:
            *Vref = DramVref->CaVref[Ch][SubCh][Dimm][Rank][DramIndex];
            break;
          default:
            return MRC_STATUS_FAILURE;
            break;
        }
      }
    } // DramIndex
  } // SubCh
  return MRC_STATUS_SUCCESS;
}

/**

  Save DRAM CS VREF setting to NVRAM

  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (socket-based)
  @param[in] SubChMask  - Mask of subchannel numbers to be updated
  @param[in] Dimm       - DIMM number (Channel-based)
  @param[in] Rank       - Rank number (DIMM-based)
  @param[in] Dram       - Dram number in the subchannel
  @param[in] CsVref     - Data to save

  @retval MRC_STATUS_SUCCESS if operation is successful
  @retval MRC_STATUS_FAILURE otherwise
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
SaveDramVrefCs (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT16   CsVref
  )
{
  return AccessSavedDramVref (Socket, Ch, SubChMask, Dimm, Rank, Dram, DramCsVref, SAVE_DRAM_VREF, &CsVref);
}

/**

  Return the saved DRAM CS VREF setting to NVRAM

  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (socket-based)
  @param[in] SubChMask  - Mask of subchannel numbers to be updated
  @param[in] Dimm       - DIMM number (Channel-based)
  @param[in] Rank       - Rank number (DIMM-based)
  @param[in] Dram       - Dram number in the subchannel
  @param[in] CsVref     - Pointer to caller's byte

  @retval MRC_STATUS_SUCCESS if operation is successful
  @retval MRC_STATUS_FAILURE otherwise
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
GetSavedDramVrefCs (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT16   *CsVref
  )
{
  return AccessSavedDramVref (Socket, Ch, SubChMask, Dimm, Rank, Dram, DramCsVref, GET_DRAM_VREF, CsVref);
}

/**

  Save DRAM CA VREF setting to NVRAM

  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (socket-based)
  @param[in] SubChMask  - Mask of subchannel numbers to be updated
  @param[in] Dimm       - DIMM number (Channel-based)
  @param[in] Rank       - Rank number (DIMM-based)
  @param[in] Dram       - Dram number in the subchannel
  @param[in] CaVref     - Data to save

  @retval MRC_STATUS_SUCCESS if operation is successful
  @retval MRC_STATUS_FAILURE otherwise
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
SaveDramVrefCa (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT16   CaVref
  )
{
  return AccessSavedDramVref (Socket, Ch, SubChMask, Dimm, Rank, Dram, DramCaVref, SAVE_DRAM_VREF, &CaVref);
}

/**

  Return the saved DRAM CA VREF setting to NVRAM

  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (socket-based)
  @param[in] SubChMask  - Mask of subchannel numbers to be updated
  @param[in] Dimm       - DIMM number (Channel-based)
  @param[in] Rank       - Rank number (DIMM-based)
  @param[in] Dram       - Dram number in the subchannel
  @param[in] CaVref     - Pointer to caller's byte

  @retval MRC_STATUS_SUCCESS if operation is successful
  @retval MRC_STATUS_FAILURE otherwise
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
GetSavedDramVrefCa (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT16   *CaVref
  )
{
  return AccessSavedDramVref (Socket, Ch, SubChMask, Dimm, Rank, Dram, DramCaVref, GET_DRAM_VREF, CaVref);
}

/**
  Restore all cached DRAM mode registers for the channel, dimm and rank

  @param[in]  Socket     - Socket number
  @param[in]  Ch         - Channel number (socket-based)
  @param[in]  Dimm       - DIMM number (Channel-based)
  @param[in]  Rank       - Rank number (DIMM-based)

  @retval   MRC_STATUS_SUCCESS if registers are restored
  @retval   MRC_STATUS_FAILURE otherwise
  @retval   MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
RestoreDramModeRegistersFromCache (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     Rank
  )
{
  PSYSHOST                    Host;
  UINT8                       SubCh;
  UINT8                       Strobe;
  UINT8                       Index;
  UINT8                       MaxChDdr;
  UINT8                       MaxRank;
  UINT8                       Address;
  UINT8                       Data;
  UINT8                       SubChMSVx4;
  UINT8                       SubChMSVx8;
  UINT8                       PrevChannelModeOperation;
  SOCKET_MRW_CACHE            *MrwCache;

  Host       = GetSysHostPointer ();
  MrwCache   = GetMrwCache (Host, Socket);
  MaxChDdr   = GetMaxChDdr ();
  MaxRank    = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);

  if ((Socket >= MAX_SOCKET) ||
      (Ch     >= MaxChDdr)   ||
      (Dimm   >= MAX_DIMM)   ||
      (Rank   >= MaxRank))
  {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "WARNING - RestoreDramModeRegistersFromCache called with an invalid parameter\n");
    return MRC_STATUS_FAILURE;
  }

  if ((IsChannelEnabled (Socket, Ch) == FALSE) ||
      (IsDimmPresent (Socket, Ch, Dimm) == FALSE) ||
      (IsRankValid (Socket, Ch, Dimm, Rank, FALSE) == FALSE))
  {
    return MRC_STATUS_FAILURE;
  }

  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  SubChMSVx8 = SubChMSVx4 / 2;

  PrevChannelModeOperation = ModeDdr5SubChannel;
  if (IsBrsPresent (Host, Socket)) {
    PrevChannelModeOperation = Host->ChannelModeOperation;
    Host->ChannelModeOperation= ModeDdr5FullChannel;
  }

  //
  // Per-rank mode registers
  //
  for (Index = 0; Index < ARRAY_SIZE (MrwCache->ByRank); Index++) {
    Address = MrwCache->ByRank[Index].Address;
    //
    // It may be more efficient to send both subchannels at once
    // Do that if the data is the same and both subchannel entires are valid
    //
    Data = MrwCache->ByRank[Index].Cache[Ch][SUB_CH_A][Dimm][Rank].Data;
    if ((Data == MrwCache->ByRank[Index].Cache[Ch][SUB_CH_B][Dimm][Rank].Data) &&
        (IsMrwCacheEntryValid (MrwCache->ByRank[Index].Valid, Ch, SUB_CH_A, Dimm, Rank)) &&
        (IsMrwCacheEntryValid (MrwCache->ByRank[Index].Valid, Ch, SUB_CH_B, Dimm, Rank)))
    {
      SendMrw (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, Address, CW_DRAM, Data);
    } else {
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        if (IsMrwCacheEntryValid (MrwCache->ByRank[Index].Valid, Ch, SubCh, Dimm, Rank)) {
          Data = MrwCache->ByRank[Index].Cache[Ch][SubCh][Dimm][Rank].Data;
          SendMrw (Host, Socket, Ch, 1 << SubCh, Dimm, Rank, Address, CW_DRAM, Data);
        } // Valid
      } // SubCh
    }
  } // Index

  //
  // Per-device mode registers - All device types cache these registers
  //
  for (Index = 0; Index < ARRAY_SIZE (MrwCache->ByDevice.Devicesx4x8x16); Index++) {
    Address = MrwCache->ByDevice.Devicesx4x8x16[Index].Address;
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {

        if ((((Strobe >= SubChMSVx8) && (IsX4Dimm (Socket, Ch, Dimm)) == FALSE)) ||
            (!IsMrwCacheEntryValid (MrwCache->ByDevice.Devicesx4x8x16[Index].Valid, Ch, SubCh, Dimm, Rank)))
        {
          continue;
        } else {
          Data = MrwCache->ByDevice.Devicesx4x8x16[Index].Cache[Ch][SubCh][Dimm][Rank][Strobe].Data;
          SendMrwPda (Host, Socket, Ch, 1 << SubCh, Dimm, Rank, Strobe, Address, Data);
        }
      } // Strobe
    } // SubCh
  } // Index

  if (IsX4Dimm (Socket, Ch, Dimm) == FALSE) {
    //
    // Per-device mode registers - x4 devices do not cache these registers
    //
    for (Index = 0; Index < ARRAY_SIZE (MrwCache->ByDevice.Devicesx8x16); Index++) {
      Address = MrwCache->ByDevice.Devicesx8x16[Index].Address;
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        for (Strobe = 0; Strobe < SubChMSVx8; Strobe++) {
          if (IsMrwCacheEntryValid (MrwCache->ByDevice.Devicesx8x16[Index].Valid, Ch, SubCh, Dimm, Rank)) {
            Data = MrwCache->ByDevice.Devicesx8x16[Index].Cache[Ch][SubCh][Dimm][Rank][Strobe].Data;
            SendMrwPda (Host, Socket, Ch, 1 << SubCh, Dimm, Rank, Strobe, Address, Data);
          } // Valid
        } // Strobe
      } // SubCh
    } // Index

#if defined (X16_DEVICE_CACHE_SUPPORTED) && (X16_DEVICE_CACHE_SUPPORTED == 1)
    //
    // Per-device mode registers - only x16 devices cache these registers
    //
    for (Index = 0; Index < ARRAY_SIZE (MrwCache->ByDevice.Devicesx16); Index++) {
      Address = MrwCache->ByDevice.Devicesx16[Index].Address;
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        for (Strobe = 0; Strobe < SubChMSVx8; Strobe++) {
          if (IsMrwCacheEntryValid (MrwCache->ByDevice.Devicesx16[Index].Valid, Ch, SubCh, Dimm, Rank)) {
            Data = MrwCache->ByDevice.Devicesx16[Index].Cache[Ch][SubCh][Dimm][Rank][Strobe].Data;
            SendMrwPda (Host, Socket, Ch, 1 << SubCh, Dimm, Rank, Strobe, Address, Data);
          } // Valid
        } // Strobe
      } // SubCh
    } // Index
#endif  // X16_DEVICE_CACHE_SUPPORTED

  } // !IsX4Dimm
  if (IsBrsPresent (Host, Socket)) {
    Host->ChannelModeOperation = PrevChannelModeOperation;
  }
  return MRC_STATUS_SUCCESS;
}

/**
  Restore DRAM mode registers from cache

  @param[in]  Host       - Pointer to SysHost
  @param[in]  Socket     - Socket number
  @param[in]  Channel    - Channel number (socket-based)

  @retval N/A

**/
VOID
RestoreDramModeRegisters (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               NumRanks;

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    if ((IsDimmPresent (Socket, Channel, Dimm) == FALSE) || (IsDcpmmPresentDimm (Socket, Channel, Dimm) == TRUE)) {
      continue;
    }
    if (CheckS3Jedec (Host, Socket, Channel, Dimm) == 0) {
      continue;
    }

    NumRanks = GetNumberOfRanksOnDimm (Socket, Channel, Dimm);
    for (Rank = 0; Rank < NumRanks; Rank++) {
      if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }
      RestoreDramModeRegistersFromCache (Socket, Channel, Dimm, Rank);
    } // Rank
  } // Dimm
}

/**
  Display Mode Register contents from the cached values

  @param[in]  Socket      - Socket number

  @retval   N/A
**/
VOID
EFIAPI
DumpDramMrCache (
  IN  UINT8     Socket
  )
{
#ifdef DEBUG_CODE_BLOCK
  PSYSHOST                    Host;
  UINT8                       Ch;
  UINT8                       SubCh;
  UINT8                       Dimm;
  UINT8                       Rank;
  UINT8                       NumRanks;
  UINT8                       Strobe;
  UINT8                       Index;
  UINT8                       MaxChDdr;
  UINT8                       SubChMSVx4;
  UINT8                       SubChMSVx8;
  UINT8                       Address;
  UINT8                       Data;
  UINT16                      ListLength;
  UINT8                       PrevChannelModeOperation;
  UINT32                      DbgLevel;
  SOCKET_MRW_CACHE            *MrwCache;

  Host = GetSysHostPointer ();
  MrwCache = GetMrwCache (Host, Socket);
  MaxChDdr = GetMaxChDdr ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  SubChMSVx8 = SubChMSVx4 / 2;

  //
  // Every print in this function should use the same level.
  //
  DbgLevel = SDBG_MAX;

  //
  // If this function can't print, don't bother to read anything, but
  // return early instead.
  //
  if (!CheckDebugPrintLevel (DbgLevel)) {
    return;
  }

  PrevChannelModeOperation = ModeDdr5SubChannel;
  if (IsBrsPresent (Host, Socket)) {
    PrevChannelModeOperation = Host->ChannelModeOperation;
    Host->ChannelModeOperation= ModeDdr5FullChannel;
  }

  RcDebugPrint (DbgLevel, "\nSTART_MR_CACHE\n");

  RcDebugPrint (DbgLevel, "Strobe:                     0    1    2    3    4    5    6    7    8    9\n");

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

        NumRanks = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);
        for (Rank = 0; Rank < NumRanks; Rank++) {
          if (IsRankValid (Socket, Ch, Dimm, Rank, FALSE) == FALSE) {
            continue;
          }

          //
          // Per-rank range for x4, x8 and x16 devices
          //
          ListLength = ARRAY_SIZE (MrwCache->ByRank);
          for (Index = 0; Index < ListLength; Index++) {
            Address = MrwCache->ByRank[Index].Address;
            RcDebugPrintWithDevice (DbgLevel, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT, "MR%3d: ", Address);

            for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {

              if (IsMrwCacheEntryValid (MrwCache->ByRank[Index].Valid, Ch, SubCh, Dimm, Rank)) {
                Data = MrwCache->ByRank[Index].Cache[Ch][SubCh][Dimm][Rank].Data;
                RcDebugPrint (DbgLevel, " 0x%02x", Data);
              } else {
                RcDebugPrint (DbgLevel, "   - ");
              } // Valid
            } // Strobe
            RcDebugPrint (DbgLevel, "\n");
          } // Index

          //
          // PDA range for x4, x8 and x16 devices
          //
          ListLength = ARRAY_SIZE (MrwCache->ByDevice.Devicesx4x8x16);
          for (Index = 0; Index < ListLength; Index++) {
            Address = MrwCache->ByDevice.Devicesx4x8x16[Index].Address;
            RcDebugPrintWithDevice (DbgLevel, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT, "MR%3d: ", Address);

            for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {

              if (IsMrwCacheEntryValid (MrwCache->ByDevice.Devicesx4x8x16[Index].Valid, Ch, SubCh, Dimm, Rank)) {
                Data = MrwCache->ByDevice.Devicesx4x8x16[Index].Cache[Ch][SubCh][Dimm][Rank][Strobe].Data;
                RcDebugPrint (DbgLevel, " 0x%02x", Data);
              } else {
                RcDebugPrint (DbgLevel, "   - ");
              } // Valid
            } // Strobe
            RcDebugPrint (DbgLevel, "\n");
          } // Index

          //
          // PDA range for x8 and x16 devices
          //
          ListLength = ARRAY_SIZE (MrwCache->ByDevice.Devicesx8x16);
          for (Index = 0; Index < ListLength; Index++) {
            Address = MrwCache->ByDevice.Devicesx8x16[Index].Address;
            RcDebugPrintWithDevice (DbgLevel, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT, "MR%3d: ", Address);

            for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {

              if ((Strobe < SubChMSVx8) &&
                  (IsMrwCacheEntryValid (MrwCache->ByDevice.Devicesx8x16[Index].Valid, Ch, SubCh, Dimm, Rank)))
              {
                Data = MrwCache->ByDevice.Devicesx8x16[Index].Cache[Ch][SubCh][Dimm][Rank][Strobe].Data;
                RcDebugPrint (DbgLevel, " 0x%02x", Data);
              } else {
                RcDebugPrint (DbgLevel, "   - ");
              } // Valid
            } // Strobe
            RcDebugPrint (DbgLevel, "\n");
          } // Index

#if defined (X16_DEVICE_CACHE_SUPPORTED) && (X16_DEVICE_CACHE_SUPPORTED == 1)
          //
          // PDA range for x16 devices
          //
          ListLength = ARRAY_SIZE (MrwCache->ByDevice.Devicesx16);
          for (Index = 0; Index < ListLength; Index++) {
            Address = MrwCache->ByDevice.Devicesx16[Index].Address;
            RcDebugPrintWithDevice (DbgLevel, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT, "MR%3d: ", Address);

            for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {

              if ((Strobe < SubChMSVx8) &&
                  (IsMrwCacheEntryValid (MrwCache->ByDevice.Devicesx16[Index].Valid, Ch, SubCh, Dimm, Rank)))
              {
                Data = MrwCache->ByDevice.Devicesx16[Index].Cache[Ch][SubCh][Dimm][Rank][Strobe].Data;
                RcDebugPrint (DbgLevel, " 0x%02x", Data);
              } else {
                RcDebugPrint (DbgLevel, "   - ");
              } // Valid
            } // Strobe
            RcDebugPrint (DbgLevel, "\n");
          } // Index
#endif // X16_DEVICE_CACHE_SUPPORTED
        } // Rank
        RcDebugPrint (DbgLevel, "\n");
      } // SubCh
    } // Dimm
  } // Ch

  RcDebugPrint (DbgLevel, "\nSTOP_MR_CACHE\n\n");

  if (IsBrsPresent (Host, Socket)) {
    Host->ChannelModeOperation= PrevChannelModeOperation;
  }
#endif // DEBUG_CODE_BLOCK
}

/**
  Return TRUE if the MR address is in the skip list

  @param[in]  Address      - Mode register address

  @retval   TRUE if address is in the skip list
  @retval   FALSE otherwise
**/
BOOLEAN
IsInMrDumpSkipList (
  IN  UINT8   Address
  )
{
  UINT8       Index;
  UINT8       SkipList[] = {DDR5_MODE_REGISTER_31_REG};

  for (Index = 0; Index < ARRAY_SIZE (SkipList); Index++) {
    if (Address == SkipList[Index]) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Display Mode Register contents by reading directly using MRR command

  @param[in]  Socket      - Socket number

  @retval   N/A
**/
VOID
EFIAPI
DumpDramModeRegisters (
  IN  UINT8     Socket
  )
{
#ifdef DEBUG_CODE_BLOCK
  PSYSHOST                    Host;
  UINT8                       Ch;
  UINT8                       SubCh;
  UINT8                       Dimm;
  UINT8                       Rank;
  UINT8                       NumRanks;
  UINT8                       Strobe;
  UINT16                      Index;
  UINT8                       MaxChDdr;
  UINT8                       SubChMSVx4;
  UINT8                       MaxStrobeSubCh;
  UINT8                       Address;
  UINT8                       PrevChannelModeOperation;
  UINT32                      DbgLevel;
  UINT8                       Data[SUB_CH][MAX_STROBE_DDR5 / 2];
  MRC_STATUS                  Status;
  BOOLEAN                     X4DimmPresent;
  UINT32                      PrevDebugLevel;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  //
  // Every print in this function should use the same level.
  //
  DbgLevel = SDBG_MAX;
  //
  // If this function can't print, don't bother to read anything, but
  // return early instead.
  //
  if (!CheckDebugPrintLevel (DbgLevel)) {
    return;
  }

  if (UseSmbusMrr (Socket)) {
    //
    // Can't send MRR commands yet - no way to read DRAM mode registers
    //
    RcDebugPrint (DbgLevel, "MRR command not allowed yet - skipping DRAM mode register dump\n");
    return;
  }

  PrevDebugLevel = GetDebugLevel ();

  PrevChannelModeOperation = ModeDdr5SubChannel;
  if (IsBrsPresent (Host, Socket)) {
    PrevChannelModeOperation = Host->ChannelModeOperation;
    Host->ChannelModeOperation= ModeDdr5FullChannel;
  }

  RcDebugPrint (DbgLevel, "\nDDR5 DRAM MR Data\n");

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      X4DimmPresent = IsX4Dimm (Socket, Ch, Dimm);
      if (X4DimmPresent) {
        MaxStrobeSubCh = SubChMSVx4;
        RcDebugPrint (DbgLevel, "Strobe:                     0    1    2    3    4    5    6    7    8    9\n");
      } else {
        MaxStrobeSubCh = SubChMSVx4 / 2;
        RcDebugPrint (DbgLevel, "Strobe:                     0    1    2    3    4 \n");
      }

      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

        NumRanks = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);
        for (Rank = 0; Rank < NumRanks; Rank++) {
          if (IsRankValid (Socket, Ch, Dimm, Rank, FALSE) == FALSE) {
            continue;
          }

          //
          // All registers
          //
          for (Index = 0; Index < MAX_DRAM_MODE_REGISTERS; Index++) {
            Address = (UINT8)Index;

            RcDebugPrintWithDevice (DbgLevel, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT, "MR%3d: ", Address);
            if (IsInMrDumpSkipList (Address)) {
              RcDebugPrint (DbgLevel, " Skipped\n");
              continue;
            }

            ZeroMem (Data, sizeof (Data));
            //
            // Prevent the MRR debug messages from messing up the pretty table
            //
            if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
              SetDebugLevel (SDBG_ERROR);
            }
            Status = ReadDramMr (Socket, Ch, 1 << SubCh, Dimm, Rank, Address, &Data);
            SetDebugLevel (PrevDebugLevel);
            for (Strobe = 0; Strobe < MaxStrobeSubCh; Strobe++) {

              if (Status == MRC_STATUS_SUCCESS) {
                RcDebugPrint (DbgLevel, " 0x%02x", Data[SubCh][Strobe]);
              } else {
                RcDebugPrint (DbgLevel, "   x ");
              } // Valid
            } // Strobe
            RcDebugPrint (DbgLevel, "\n");
          } // Index
        } // Rank
        RcDebugPrint (DbgLevel, "\n");
      } // SubCh
    } // Dimm
  } // Ch

  if (IsBrsPresent (Host, Socket)) {
    Host->ChannelModeOperation= PrevChannelModeOperation;
  }
#endif // DEBUG_CODE_BLOCK
}

