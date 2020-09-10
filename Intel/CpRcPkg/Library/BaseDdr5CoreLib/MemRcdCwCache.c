/** @file
  MemRcdCwCache.c
  Functions needed to cache DDR5 Registering Clock Driver (RCD) Control Words

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

#include "MemRcdCwCache.h"
#include "Ddr5Core.h"
#include <Memory/JedecDefinitions.h>

/**
  Initialize the RCD CW cache for a single socket
  Initialization consists of clearing all entries and valid bits to 0 and
  setting the CW Address fields using the list of CWs that are to be cached.

  @param[in,out] RcdCwCache   - Pointer to the RCD control word cache for the socket

  @retval MRC_STATUS_SUCCESS if the cache is successfully initialized
          MRC_STATUS_FAILURE otherwise
**/
MRC_STATUS
InitRcdCwCache (
  IN  SOCKET_RCD_CW_CACHE  *RcdCwCache
  )
{
  UINT8             Index;
  UINT8             RcdCwPageList[]  = {RCD_CW_PAGE_CACHE_LIST};

  if (RcdCwCache == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_188);
    return MRC_STATUS_FAILURE;
  }

  //
  // Clear all Valid bits
  //
  ZeroMem (RcdCwCache, sizeof (SOCKET_RCD_CW_CACHE));

  //
  // For each paged cache entry, initialize the Page field
  //
  for (Index = 0; Index < ARRAY_SIZE (RcdCwPageList); Index++) {
    RcdCwCache->PagedCw[Index].Page = RcdCwPageList[Index];
  } // Index

  RcdCwCache->State.Bits.Initialized = 1;
  return MRC_STATUS_SUCCESS;
}

/**
  Obtain pointer to RCD CW cache for a socket

  @param[in]  Host    - Pointer to SysHost
  @param[in]  Socket  - Socket number

  @retval Pointer to DDR5_RCD_NVDATA.RcdCwCache[Socket]
**/
SOCKET_RCD_CW_CACHE
*GetRcdCwCache (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  DDR5_RCD_NVDATA       *RcdNvData;
  SOCKET_RCD_CW_CACHE   *RcdCwCache;

  RcdNvData = (VOID *)Host->nvram.mem.RcdNvdata;
  RcdCwCache = &(RcdNvData->RcdCwCache[Socket]);

  //
  // If the cache has not been initialized for this socket, do it now
  //
  if (RcdCwCache->State.Bits.Initialized == 0) {
    InitRcdCwCache (RcdCwCache);
  }
  return (RcdCwCache);
}

/**
  Validate the RCD data structure backing store

  @retval MRC_STATUS_FAILURE if not enough memory allocated for the buffer
          MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
          MRC_STATUS_SUCCESS otherwise
**/
MRC_STATUS
EFIAPI
ValidateRcdDataStucture (
  VOID
  )
{
  PSYSHOST                  Host;
  UINT32                    RequiredSize;
  UINT32                    AllocatedSize;

  Host = GetSysHostPointer ();

  RequiredSize = sizeof (DDR5_RCD_NVDATA);
  AllocatedSize = sizeof (Host->nvram.mem.RcdNvdata);
  //
  // Verify sufficient memory has been allocated for the DDR5_RCD_NVDATA structure
  //
  if (AllocatedSize < RequiredSize) {
    RcDebugPrint (SDBG_ERROR, "ERROR: Space allocated for RcdNvdata(%d) < Space required by DDR5_RCD_NVDATA(%d).\nRCD_DATA_1S=139 must be defined to be at least (%d)\n", AllocatedSize, RequiredSize, RequiredSize / MAX_SOCKET);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_189);
    return MRC_STATUS_FAILURE;
  }

  //
  // If there is more space allocated for DDR5_RCD_NVDATA than is needed, NEM and NVRAM storage space are being wasted
  //
  if (AllocatedSize > RequiredSize) {
    RcDebugPrint (SDBG_MAX, "Warning - wasted memory: Space allocated for RcdNvdata(%d) > Space required by DDR5_RCD_NVDATA(%d)\n", AllocatedSize, RequiredSize);
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Determine if a control word is global

  @param[in]  Address           - Control word address

  @retval TRUE if CW is Global
  @retval FALSE otherwise
**/
BOOLEAN
IsGlobalCw (
  IN  UINT8   Address
  )
{
  UINT8     Index;
  UINT8     GlobalCwList[] = {RCD_CW_GLOBAL_CW_LIST};

  for (Index = 0; Index < ARRAY_SIZE (GlobalCwList); Index++) {
    if (Address == GlobalCwList[Index]) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Determine if a control word is in the exception list
  These control words must not be restored to HW from cache

  @param[in]  Page              - Control word page
  @param[in]  Address           - Control word address

  @retval TRUE if CW is in the exception list
  @retval FALSE otherwise
**/
BOOLEAN
IsInRcdCwExceptionList (
  IN  UINT8   Page,
  IN  UINT8   Address
  )
{
  UINT8         Index;
  RCD_PAGED_CW  CwExceptionList[] = {{RDIMM_CW_PAGE0, DDR5_RCD_COMMAND_SPACE_REG}};

  for (Index = 0; Index < ARRAY_SIZE (CwExceptionList); Index++) {
    if ((Page == CwExceptionList[Index].Page) && (Address == CwExceptionList[Index].Address)) {
      return TRUE;
    }
  }

  return FALSE;
}


/**
  The byte and bit positions into the Valid array for Ch, SubCh and Dimm.

  @param[in]  Ch                - Channel number (80-bit channel)
  @param[in]  SubCh             - Subchannel number
  @param[in]  Dimm              - DIMM number within the channel
  @param[out] BitMask           - Bit mask for the Valid array byte
  @param[out] ByteOffsetInArray - Byte offset within the Valid array

  @retval MRC_STATUS_SUCCESS - Valid bit offset is found
  @retval MRC_STATUS_FAILURE - Array index is invalid
**/
MRC_STATUS
GetRcdCwCacheEntryValidBitMaskAndBtyeOffsets (
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT8     Dimm,
  OUT UINT8     *BitMask,
  OUT UINT16    *ByteOffsetInArray
  )
{
  UINT16    BitOffsetInArray;

  //
  // Calculate the Valid bit position in the array for Ch, SubCh and Dimm
  //
  BitOffsetInArray = (Ch * VALID_BIT_CH_OFFSET_RCD_CW) +
                     (SubCh * VALID_BIT_SUB_CH_OFFSET_RCD_CW) +
                     (Dimm * VALID_BIT_DIMM_OFFSET_RCD_CW);

  //
  // Calculate the byte within the Valid array that contains the Valid bit for Ch, SubCh, Dimm and Rank
  //
  *ByteOffsetInArray = BitOffsetInArray / (MAX_BITS_IN_BYTE);

  if (*ByteOffsetInArray >= SIZE_VALID_ARRAY_RCD_CW) {
    //
    // The array index is out of bounds
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_190);
    return MRC_STATUS_FAILURE;
  }
  *BitMask = 1 << (BitOffsetInArray % (MAX_BITS_IN_BYTE));
  return MRC_STATUS_SUCCESS;
}

/**
  Return TRUE if the Valid bit is set for the mode register instance
  specified by Ch, SubCh and Dimm.
  This indicates the cache entry has been updated at least once.

  @param[in]  Valid     - Valid arrary
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - Subchannel number
  @param[in]  Dimm      - DIMM number within the channel

  @retval TRUE  - Valid bit is set
  @retval FALSE - Valid bit is not set
**/
BOOLEAN
IsRcdCwCacheEntryValid (
  IN  UINT8     Valid[SIZE_VALID_ARRAY_RCD_CW],
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT8     Dimm
  )
{
  MRC_STATUS  Status;
  UINT16      ByteOffsetInArray;
  UINT8       BitMask;

  Status = GetRcdCwCacheEntryValidBitMaskAndBtyeOffsets (Ch, SubCh, Dimm, &BitMask, &ByteOffsetInArray);

  if (Status == MRC_STATUS_SUCCESS) {

    if ((Valid[ByteOffsetInArray] & BitMask) == 0) {
      return FALSE;
    }
    return TRUE;
  }

  return FALSE;

}

/**
  Set the Valid bit for the mode register instance
  specified by Ch, SubCh and Dimm

  @param[in]  Valid     - Valid arrary
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - Subchannel number
  @param[in]  Dimm      - DIMM number within the channel

  @retval N/A
**/
VOID
SetRcdCwCacheEntryValid (
  IN  UINT8     Valid[SIZE_VALID_ARRAY_RCD_CW],
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT8     Dimm
  )
{
  MRC_STATUS  Status;
  UINT16      ByteOffsetInArray;
  UINT8       BitMask;

  Status = GetRcdCwCacheEntryValidBitMaskAndBtyeOffsets (Ch, SubCh, Dimm, &BitMask, &ByteOffsetInArray);

  if (Status == MRC_STATUS_SUCCESS) {

    Valid[ByteOffsetInArray] |= BitMask;
  }
}

/**
  Read or write the cached value of the Global RCD CW specified by Address.
  If the CW is in the cache list, update or return the cache contents in the
  caller's Data buffer.
  If the CW is not in the cache list, do not modify the caller's Data buffer.

  @param[in]     Socket     - Socket number
  @param[in]     Ch         - Channel number (80-bit channel)
  @param[in]     SubCh      - Subchannel within the channel
  @param[in]     Dimm       - DIMM number within the channel
  @param[in]     AccessType - Read or Write
  @param[in]     Address    - The CW address
  @param[in,out] Data       - Pointer to caller's buffer

  @retval MRC_STATUS_SUCCESS cached data is returned
  @retval MRC_STATUS_FAILURE if the mode register is not in the cache list
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
AccessRcdCwCacheDirect (
  IN     UINT8               Socket,
  IN     UINT8               Ch,
  IN     UINT8               SubCh,
  IN     UINT8               Dimm,
  IN     RCD_CW_ACCESS_TYPE  AccessType,
  IN     UINT8               Address,
  IN OUT UINT8               *Data
  )
{
  PSYSHOST                    Host;
  UINT8                       TempSubCh;
  SOCKET_RCD_CW_CACHE         *RcdCwCache;

  //
  // Direct address CWs are at addresses 0 -> 0x5F
  //
  if (Address >= MAX_RCD_DIRECT_CONTROL_WORDS) {
    return MRC_STATUS_GROUP_NOT_SUPPORTED;
  }

  Host = GetSysHostPointer ();
  RcdCwCache = GetRcdCwCache (Host, Socket);

  //
  // Global control words apply to both subchannels but are written and read from
  // subchannel A only. Any attempt to write to subchannel B will do nothing.
  //
  // Look for the control word address in the Global CW list and force SubCh = A if found
  //
  TempSubCh = SubCh;
  if (IsGlobalCw (Address)) {
    TempSubCh = SUB_CH_A;
  }

  if (AccessType == RcdCwRead) {
    *Data = RcdCwCache->DirectCw[Address].Cache[Ch][TempSubCh][Dimm].Data;
  } else {
    RcdCwCache->DirectCw[Address].Cache[Ch][TempSubCh][Dimm].Data = *Data;
    if (!IsInRcdCwExceptionList (RDIMM_CW_PAGE0, Address)) {
      //
      // Don't set the Valid bit for registers in the exception list
      //
      SetRcdCwCacheEntryValid (RcdCwCache->DirectCw[Address].Valid, Ch, TempSubCh, Dimm);
    }
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Read or write the cached value of the Paged RCD CW specified by Page and Address.
  If the CW is in the cache list, update or return the cache contents in the
  caller's Data buffer.
  If the CW is not in the cache list, do not modify the caller's Data buffer.

  @param[in]     Socket     - Socket number
  @param[in]     Ch         - Channel number (80-bit channel)
  @param[in]     SubCh      - Subchannel within the channel
  @param[in]     Dimm       - DIMM number within the channel
  @param[in]     AccessType - Read or Write
  @param[in]     Page       - The CW page number
  @param[in]     Address    - The CW address
  @param[in,out] Data       - Pointer to caller's buffer

  @retval MRC_STATUS_SUCCESS cached data is returned
  @retval MRC_STATUS_FAILURE if the mode register is not in the cache list
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
AccessRcdCwCachePaged (
  IN     UINT8               Socket,
  IN     UINT8               Ch,
  IN     UINT8               SubCh,
  IN     UINT8               Dimm,
  IN     RCD_CW_ACCESS_TYPE  AccessType,
  IN     UINT8               Page,
  IN     UINT8               Address,
  IN OUT UINT8               *Data
  )
{
  PSYSHOST                    Host;
  MRC_STATUS                  Status;
  UINT8                       PageIndex;
  UINT8                       AddressIndex;
  SOCKET_RCD_CW_CACHE         *RcdCwCache;

  Status = MRC_STATUS_GROUP_NOT_SUPPORTED;
  //
  // Paged CW addresses are at addresses 0x60 -> 0x7F
  //
  if ((Address < PAGED_CONTROL_WORD_INITIAL_OFFSET) || (Address >= (PAGED_CONTROL_WORD_INITIAL_OFFSET + MAX_RCD_CONTROL_WORDS_PER_PAGE))) {
    return Status;
  }

  Host = GetSysHostPointer ();
  RcdCwCache = GetRcdCwCache (Host, Socket);

  //
  // Address is the actual CW address within the page
  // AddressIndex is the index into the PageCache array, derived from Address
  //
  AddressIndex = Address - PAGED_CONTROL_WORD_INITIAL_OFFSET;
  //
  // Look for matching control word address and page number in the Paged CW list
  //
  for (PageIndex = 0; PageIndex < ARRAY_SIZE (RcdCwCache->PagedCw); PageIndex++) {
    if (RcdCwCache->PagedCw[PageIndex].Page == Page) {
      if (AccessType == RcdCwRead) {
        *Data = RcdCwCache->PagedCw[PageIndex].PageCache[AddressIndex].Cache[Ch][SubCh][Dimm].Data;
      } else {
        RcdCwCache->PagedCw[PageIndex].PageCache[AddressIndex].Cache[Ch][SubCh][Dimm].Data = *Data;
        if (!IsInRcdCwExceptionList (Page, Address)) {
          //
          // Don't set the Valid bit for registers in the exception list
          //
          SetRcdCwCacheEntryValid (RcdCwCache->PagedCw[PageIndex].PageCache[AddressIndex].Valid, Ch, SubCh, Dimm);
        }
      }
      Status = MRC_STATUS_SUCCESS;
      break;
    }
  } // Index

  return Status;
}

/**
  Return the cached value of the Global RCD CW specified by Address.
  If the CW is in the cache list, return the contents in the caller's Data buffer.
  If the CW is not in the cache list, do not modify the caller's Data buffer.

  @param[in]     Socket     - Socket number
  @param[in]     Ch         - Channel number (80-bit channel)
  @param[in]     SubCh      - Subchannel within the channel
  @param[in]     Dimm       - DIMM number within the channel
  @param[in]     AccessType - Read or Write
  @param[in]     Page       - The CW page number
  @param[in]     Address    - The CW address
  @param[in]     Data       - Pointer to caller's buffer

  @retval MRC_STATUS_SUCCESS cached is written
  @retval MRC_STATUS_FAILURE if the control word is not in the cache list
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
AccessRcdCwCache (
  IN     UINT8               Socket,
  IN     UINT8               Ch,
  IN     UINT8               SubCh,
  IN     UINT8               Dimm,
  IN     RCD_CW_ACCESS_TYPE  AccessType,
  IN     UINT8               Page,
  IN     UINT8               Address,
  IN OUT UINT8               *Data
  )
{
  PSYSHOST                    Host;
  MRC_STATUS                  Status;
  SOCKET_RCD_CW_CACHE         *RcdCwCache;

  Host = GetSysHostPointer ();
  RcdCwCache = GetRcdCwCache (Host, Socket);

  Status = MRC_STATUS_GROUP_NOT_SUPPORTED;

  if (Address >= (PAGED_CONTROL_WORD_INITIAL_OFFSET + MAX_RCD_CONTROL_WORDS_PER_PAGE)) {
    //
    // Address is out of RCD CW range
    //
    return Status;
  }

  //
  // Direct address CWs are at addresses 0 -> 0x5F
  //
  if (Address < MAX_RCD_DIRECT_CONTROL_WORDS) {
    Status = AccessRcdCwCacheDirect (Socket, Ch, SubCh, Dimm, AccessType, Address, Data);
  } else {
    Status = AccessRcdCwCachePaged (Socket, Ch, SubCh, Dimm, AccessType, Page, Address, Data);
  }

  if (Status == MRC_STATUS_GROUP_NOT_SUPPORTED) {
    //
    // This CW is missing from the cache list
    // Update RCD_GLOBAL_CW_CACHE_LIST, RCD_GLOBAL_CW_CACHE_LENGTH and RCD_DATA_1S to include the missing CW
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_191);
  }

  return Status;
}

/**
  Write an entry in the RCD control word cache

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubChMask - Subchannel mask within the channel
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Page      - The CW page number
  @param[in]  Address   - The MRW address
  @param[in]  Data      - New value to write to the cache

  @retval MRC_STATUS_SUCCESS if cache is updated
  @retval MRC_STATUS_FAILURE if the cache is not updated
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
**/
MRC_STATUS
EFIAPI
WriteRcdCwCache (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubChMask,
  IN  UINT8       Dimm,
  IN  UINT8       Page,
  IN  UINT8       Address,
  IN  UINT8       Data
  )
{
  MRC_STATUS                  Status;
  UINT8                       MaxChDdr;
  UINT8                       SubCh;

  MaxChDdr    = GetMaxChDdr ();
  Status      = MRC_STATUS_FAILURE;

  if ((Socket    >= MAX_SOCKET)       ||
      (Ch        >= MaxChDdr)         ||
      (SubChMask > TWO_SUBCH_MASK) ||
      (Dimm      >= MAX_DIMM))
  {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "WARNING - WriteRcdCwCache called with an invalid parameter (SubChMask = 0x%x)\n", SubChMask);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_192);
    return MRC_STATUS_FAILURE;
  }

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & SubChMask) == 0) {
      continue;
    }
    Status = AccessRcdCwCache (Socket, Ch, SubCh, Dimm, RcdCwWrite, Page, Address, &Data);
  }

  return Status;
}

/**
  Return an entry from the RCD control word cache

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - Subchannel within the channel
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Page      - The CW page number
  @param[in]  Address   - The MRW address
  @param[out] Data      - Poitner to caller's data buffer

  @retval MRC_STATUS_SUCCESS if cache entry is returned
  @retval MRC_STATUS_FAILURE if the cache entry is not supported
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
**/
MRC_STATUS
EFIAPI
ReadRcdCwCache (
  IN      UINT8       Socket,
  IN      UINT8       Ch,
  IN      UINT8       SubCh,
  IN      UINT8       Dimm,
  IN      UINT8       Page,
  IN      UINT8       Address,
     OUT  UINT8       *Data
  )
{
  MRC_STATUS                  Status;
  UINT8                       MaxChDdr;

  MaxChDdr    = GetMaxChDdr ();

  if ((Socket >= MAX_SOCKET) ||
      (Ch     >= MaxChDdr)   ||
      (SubCh  >= SUB_CH)     ||
      (Dimm   >= MAX_DIMM))
  {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "WARNING - ReadRcdCwCache called with an invalid parameter\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_193);
    return MRC_STATUS_FAILURE;
  }

  Status = AccessRcdCwCache (Socket, Ch, SubCh, Dimm, RcdCwRead, Page, Address, Data);

  return Status;
}

/**
  Restore cached RCD control words to the RCD

  @param[in]  Socket     - Socket number
  @param[in]  Ch         - Channel number within the socket
  @param[in]  SubCh      - Subchannel within the channel
  @param[in]  Dimm       - DIMM number within the channel

  @retval   MRC_STATUS_SUCCESS if registers are restored
  @retval   MRC_STATUS_FAILURE otherwise
  @retval   MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
RestoreRcdControlWordsFromCache (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT8     Dimm
  )
{
  PSYSHOST                    Host;
  UINT8                       Rank;
  UINT8                       MaxChDdr;
  UINT8                       PageIndex;
  UINT8                       Page;
  UINT8                       CachedPage;
  UINT8                       AddressIndex;
  UINT8                       Address;
  UINT8                       Data;
  UINT32                      SmbData;
  UINT8                       PrevChannelModeOperation;
  SOCKET_RCD_CW_CACHE         *RcdCwCache;

  MaxChDdr   = GetMaxChDdr ();

  if ((Socket >= MAX_SOCKET) ||
      (Ch     >= MaxChDdr)   ||
      (SubCh  >= SUB_CH)     ||
      (Dimm   >= MAX_DIMM))
  {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "WARNING - RestoreRcdControlWordsFromCache called with an invalid parameter\n");
    return MRC_STATUS_FAILURE;
  }

  Host       = GetSysHostPointer ();
  RcdCwCache = GetRcdCwCache (Host, Socket);

  if ((IsChannelEnabled (Socket, Ch) == FALSE) ||
      (IsDimmPresent (Socket, Ch, Dimm) == FALSE))
  {
    return MRC_STATUS_FAILURE;
  }

  if (IsBrsPresent (Host, Socket)) {
    PrevChannelModeOperation = Host->ChannelModeOperation;
    Host->ChannelModeOperation= ModeDdr5FullChannel;
  }
  //
  // RCD writes go to rank 0 except when exiting a pass-through mode
  //
  Rank = 0;

  //
  // Direct address control words
  //
  for (Address = 0; Address < ARRAY_SIZE (RcdCwCache->DirectCw); Address++) {
    if (IsGlobalCw (Address) && (SubCh != SUB_CH_A)) {
      //
      // Global CWs can be written to and read from subchannel A only
      //
      continue;
    }
    if (IsRcdCwCacheEntryValid (RcdCwCache->DirectCw[Address].Valid, Ch, SubCh, Dimm)) {
      Data = RcdCwCache->DirectCw[Address].Cache[Ch][SubCh][Dimm].Data;
      if (Address <= DDR5_RCD_BCK_OUTPUT_DELAY_REG) {
        //
        // Use sideband accesses until backside delays are restored to avoid sending invalid commands to DRAM
        //
        SmbData = Data;
        WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, 1 << SubCh, BYTE_WRITE, RDIMM_CW_PAGE0, Address, &SmbData);
      } else {
        SendMrw (Host, Socket, Ch, 1 << SubCh, Dimm, Rank, Address, CW_RCD_DB, Data);
      }

    } // Valid
  } // Index

  //
  // Paged control words
  //
  for (PageIndex = 0; PageIndex < ARRAY_SIZE (RcdCwCache->PagedCw); PageIndex++) {
    Page = RcdCwCache->PagedCw[PageIndex].Page;
    //
    // AddressIndex is the index into the PageCache array
    // Address is the actual CW address within the page, derived from AddressIndex
    //
    for (AddressIndex = 0; AddressIndex < ARRAY_SIZE (RcdCwCache->PagedCw[PageIndex].PageCache); AddressIndex++) {
      if (IsRcdCwCacheEntryValid (RcdCwCache->PagedCw[PageIndex].PageCache[AddressIndex].Valid, Ch, SubCh, Dimm)) {
        Data = RcdCwCache->PagedCw[PageIndex].PageCache[AddressIndex].Cache[Ch][SubCh][Dimm].Data;
        //
        // Switch pages if necessary
        //
        ReadRcdCwCache (Socket, Ch, SubCh, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_CW_PAGE_REG, &CachedPage);
        if (Page != CachedPage) {
          SendMrw (Host, Socket, Ch, 1 << SubCh, Dimm, Rank, DDR5_RCD_CW_PAGE_REG, CW_RCD_DB, Page);
          WriteRcdCwCache (Socket, Ch, 1 << SubCh, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_CW_PAGE_REG, Page);
        }
        Address = AddressIndex + MAX_RCD_DIRECT_CONTROL_WORDS;
        SendMrw (Host, Socket, Ch, 1 << SubCh, Dimm, Rank, Address, CW_RCD_DB, Data);
      } // Valid
    } // AddressIndex
  } // PageIndex
  if (IsBrsPresent (Host, Socket)) {
    Host->ChannelModeOperation = PrevChannelModeOperation;
  }
  return MRC_STATUS_SUCCESS;
}

/**
  Display RCD control word contents from the cached values

  @param[in]  Socket      - Socket number

  @retval   N/A
**/
VOID
EFIAPI
DumpRcdCwCache (
  IN  UINT8     Socket
  )
{
#ifdef DEBUG_CODE_BLOCK
  PSYSHOST                    Host;
  UINT8                       Ch;
  UINT8                       SubCh;
  UINT8                       Dimm;
  UINT8                       MaxChDdr;
  UINT8                       PageIndex;
  UINT8                       Page;
  UINT8                       PageCount;
  UINT8                       PageLength;
  UINT8                       AddressIndex;
  UINT8                       Address;
  UINT8                       Data;
  UINT16                      ListLength;
  UINT8                       PrevChannelModeOperation;
  UINT32                      DbgLevel;
  SOCKET_RCD_CW_CACHE         *RcdCwCache;

  Host = GetSysHostPointer ();
  RcdCwCache = GetRcdCwCache (Host, Socket);
  MaxChDdr = GetMaxChDdr ();

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

  if (IsBrsPresent (Host, Socket)) {
    PrevChannelModeOperation = Host->ChannelModeOperation;
    Host->ChannelModeOperation= ModeDdr5FullChannel;
  }

  RcDebugPrint (DbgLevel, "\nSTART_RCD_CW_CACHE\n");

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

        ListLength = ARRAY_SIZE (RcdCwCache->DirectCw);
        for (Address = 0; Address < ListLength; Address++) {
          if ((Address % 16) == 0) {
            RcDebugPrint (DbgLevel, "\n");
            RcDebugPrintWithDevice (DbgLevel, Socket, Ch, Dimm, SubCh, NO_RANK, NO_STROBE, NO_BIT, "      RW%02x: ", Address);
          }
          if (IsRcdCwCacheEntryValid (RcdCwCache->DirectCw[Address].Valid, Ch, SubCh, Dimm)) {
            Data = RcdCwCache->DirectCw[Address].Cache[Ch][SubCh][Dimm].Data;
            RcDebugPrint (DbgLevel, " 0x%02x", Data);
          } else {
            RcDebugPrint (DbgLevel, "   - ");
          } // Valid
        } // Index

        PageCount = ARRAY_SIZE (RcdCwCache->PagedCw);
        for (PageIndex = 0; PageIndex < PageCount; PageIndex++) {

          Page = RcdCwCache->PagedCw[PageIndex].Page;
          PageLength = ARRAY_SIZE (RcdCwCache->PagedCw[PageIndex].PageCache);

          for (AddressIndex = 0; AddressIndex < PageLength; AddressIndex++) {
            Address = AddressIndex + MAX_RCD_DIRECT_CONTROL_WORDS;
            if ((Address % 16) == 0) {
              RcDebugPrint (DbgLevel, "\n");
              RcDebugPrintWithDevice (DbgLevel, Socket, Ch, Dimm, SubCh, NO_RANK, NO_STROBE, NO_BIT, "PG[%02x]RW%02x: ", Page, Address);
            }

            if (IsRcdCwCacheEntryValid (RcdCwCache->PagedCw[PageIndex].PageCache[AddressIndex].Valid, Ch, SubCh, Dimm)) {
              Data = RcdCwCache->PagedCw[PageIndex].PageCache[AddressIndex].Cache[Ch][SubCh][Dimm].Data;
              RcDebugPrint (DbgLevel, " 0x%02x", Data);
            } else {
              RcDebugPrint (DbgLevel, "   - ");
            } // Valid
          } // AddressIndex
        } // PageIndex
        RcDebugPrint (DbgLevel, "\n");
      } // SubCh

    } // Dimm
  } // Ch

  RcDebugPrint (DbgLevel, "\nSTOP_RCD_CW_CACHE\n\n");

  if (IsBrsPresent (Host, Socket)) {
    Host->ChannelModeOperation= PrevChannelModeOperation;
  }
#endif // DEBUG_CODE_BLOCK
}
