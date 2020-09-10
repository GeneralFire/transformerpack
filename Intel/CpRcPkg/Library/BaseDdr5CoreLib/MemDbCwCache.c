/** @file
  MemDbCwCache.c
  Functions needed to cache DDR5 Data Buffer (DB) Control Words

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
#include "Ddr5Core.h"
#include <Memory/JedecDefinitions.h>
#include <Memory/Ddr5DbRegs.h>
#include <Memory/Ddr5RcwRegs.h>

#if MAX_SOCKET <= 4
#include "MemDbCwCache.h"
#endif // MAX_SOCKET <= 4
/**
  Validate the DB data structure backing store

  @retval MRC_STATUS_FAILURE if not enough memory allocated for the buffer
          MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
          MRC_STATUS_SUCCESS otherwise
**/
MRC_STATUS
EFIAPI
ValidateDbDataStucture (
  VOID
  )
{
#if MAX_SOCKET <= 4
  PSYSHOST                  Host;
  UINT32                    RequiredSize;
  UINT32                    AllocatedSize;


  Host = GetSysHostPointer ();

  RequiredSize = sizeof (DDR5_DB_NVDATA);
  AllocatedSize = sizeof (Host->nvram.mem.DbNvdata);


  //
  // Verify sufficient memory has been allocated for the DDR5_DB_NVDATA structure
  //
  if (AllocatedSize < RequiredSize) {
    RcDebugPrint (SDBG_ERROR, "ERROR: Space allocated for DbNvdata(%d) < Space required by DDR5_DB_NVDATA(%d).\nDB_DATA_1S must be defined to be at least (%d)\n", AllocatedSize, RequiredSize, RequiredSize / MAX_SOCKET);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_189);
    return MRC_STATUS_FAILURE;
  }

  //
  // If there is more space allocated for DDR5_DB_NVDATA than is needed, NEM and NVRAM storage space are being wasted
  //
  if (AllocatedSize > RequiredSize) {
    RcDebugPrint (SDBG_MAX, "Warning - wasted memory: Space allocated for DbNvdata(%d) > Space required by DDR5_DB_NVDATA(%d)\n", AllocatedSize, RequiredSize);
  }

  return MRC_STATUS_SUCCESS;
#else
  return MRC_STATUS_FAILURE;
#endif // MAX_SOCKET <= 4
}

#if MAX_SOCKET <= 4
/**
   Get the pointer of SOCKET_DB_CW

  @param[in]      IsDirectCwList         - TRUE if Direct CWs List, FALSE if Paged CWs List
  @param[in]      PageNumber             - Page Number, if is IsDirectCwList, Don't care page number
  @param[in]      DbCwCache              - Pointer to the DB control word cache for the socket
  @param[in out]  PageListLength         - number of Cws in Page

  @retval If IsDirectCwList is true, return pointer to DirectCw
          if false, return pointer to PagedCw by page number
 **/
DB_CW_REG *
EFIAPI
GetCwListPointer (
  IN       BOOLEAN             IsDirectCwList,
  IN       UINT8               PageNumber,
  IN       SOCKET_DB_CW        *DbCwCache,
  IN OUT   UINT8               *PageListLength
)
{

  if (IsDirectCwList == TRUE) {
    *PageListLength = DB_CW_DIRECT_CACHE_LENGTH;
    return DbCwCache->DirectCw;
  }

  switch (PageNumber) {
    case LRDIMM_DB_PAGE0:
      *PageListLength = PAGE0_CW_LIST_LENGTH;
      return DbCwCache->Paged0Cw;
    case LRDIMM_DB_PAGE1:
      *PageListLength = PAGE1_CW_LIST_LENGTH;
      return DbCwCache->Paged1Cw;
    case LRDIMM_DB_PAGE2:
      *PageListLength = PAGE2_CW_LIST_LENGTH;
      return DbCwCache->Paged2Cw;
    case LRDIMM_DB_PAGE4:
      *PageListLength = PAGE4_CW_LIST_LENGTH;
      return DbCwCache->Paged4Cw;
    case LRDIMM_DB_PAGE5:
      *PageListLength = PAGE5_CW_LIST_LENGTH;
      return DbCwCache->Paged5Cw;
    case LRDIMM_DB_PAGE6:
      *PageListLength = PAGE6_CW_LIST_LENGTH;
      return DbCwCache->Paged6Cw;
    case LRDIMM_DB_PAGE8:
      *PageListLength = PAGE8_CW_LIST_LENGTH;
      return DbCwCache->Paged8Cw;
    case LRDIMM_DB_PAGE9:
      *PageListLength = PAGE9_CW_LIST_LENGTH;
      return DbCwCache->Paged9Cw;
    case LRDIMM_DB_PAGEB:
      *PageListLength = PAGEB_CW_LIST_LENGTH;
      return DbCwCache->PagedBCw;
    case LRDIMM_DB_PAGE83:
      *PageListLength = PAGE83_CW_LIST_LENGTH;
      return DbCwCache->Paged83Cw;
    case LRDIMM_DB_PAGECC:
      *PageListLength = PAGECC_CW_LIST_LENGTH;
      return DbCwCache->PagedCCCw;
    default:
      return NULL;
  }
}

/**
  Initialize the DB CW cache for a single socket
  Initialization consists of clearing all entries and valid bits to 0 and
  setting the CW Address fields using the list of CWs that are to be cached.

  @param[in,out] DbCwCache   - Pointer to the DB control word cache for the socket

  @retval MRC_STATUS_SUCCESS if the cache is successfully initialized
          MRC_STATUS_FAILURE otherwise
**/
MRC_STATUS
InitDbCwCache (
  IN  SOCKET_DB_CW  *DbCwCache
  )
{
  UINT8              Page = 0;
  BOOLEAN            IsDirectCwList;
  UINT8              CwListLength = 0;
  UINT8              PageIndex;
  UINT8              ListCwIndex;
  UINT32             CwIndexCount = 0;
  DB_CW_REG          *CwListPtr;
  UINT8              DbCwOffsetInit[] = { DB_CW_INIT_OFFSET_LIST };
  UINT8              DbCwPageList[]   = { DB_CW_PAGE_LIST };

  if (DbCwCache == NULL) {
    RcDebugPrint (SDBG_ERROR, "ERROR: Cannot initialize the DDR5 DB control word cache due to an invalid cache pointer\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_188);
    return MRC_STATUS_FAILURE;
  }

  //
  // Clear all Valid bits
  //
  ZeroMem (DbCwCache, sizeof (SOCKET_DB_CW));
  //
  // Get Cw list length and pointer
  //
  for (PageIndex = 0; PageIndex < sizeof (DbCwPageList); PageIndex++) {
    //
    // {DB_CW_PAGE_LIST}[0] is used to map DirectCw list {DB_CW_INIT_OFFSET_LIST}[0]
    //
    IsDirectCwList = (PageIndex == 0);
    Page = DbCwPageList[PageIndex];
    //
    // Get Control word list llength and poniter
    //
    CwListPtr = GetCwListPointer (IsDirectCwList, Page, DbCwCache, &CwListLength);
    if (CwListPtr == NULL) {
      RcDebugPrint (SDBG_MAX, "WARNING - GetCwListPointer called with an unsupported page number (Page = 0x%x)\n", Page);
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_191);
      return MRC_STATUS_FAILURE;
    }
    //
    // Init Offset for each Control Word Register
    //
    for (ListCwIndex = 0; ListCwIndex < CwListLength; ListCwIndex++) {
      CwListPtr[ListCwIndex].Offset = DbCwOffsetInit[CwIndexCount];
      //
      // Init RW93 with broadcast value ALL_DATABUFFERS
      // Init RwDF with default value LRDIMM_DB_PAGE0
      //
      if (CwListPtr[ListCwIndex].Offset == DDR5_DATA_BUFFER_RW93_REG) {
        SetMem (CwListPtr[ListCwIndex].Cache, sizeof (CwListPtr[ListCwIndex].Cache), ALL_DATABUFFERS);
        SetMem (CwListPtr[ListCwIndex].Valid, sizeof (CwListPtr[ListCwIndex].Valid), 1);
      } else if (CwListPtr[ListCwIndex].Offset == RDIMM_RWDF ) {
        ZeroMem (CwListPtr[ListCwIndex].Cache, sizeof (CwListPtr[ListCwIndex].Cache));
        SetMem(CwListPtr[ListCwIndex].Valid, sizeof(CwListPtr[ListCwIndex].Valid), 1);
      }

      CwIndexCount += 1;
    } // ListCwIndex
  } // PageIndex

  DbCwCache->State.Bits.Initialized = 1;

  return MRC_STATUS_SUCCESS;
}


/**
  Obtain pointer to DB CW cache for a socket

  @param[in]  Host    - Pointer to SysHost
  @param[in]  Socket  - Socket number

  @retval Pointer to DDR5_DB_NVDATA.DbCwCache[Socket]
**/
SOCKET_DB_CW
*GetDbCwCache (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  DDR5_DB_NVDATA       *DbNvData;
  SOCKET_DB_CW         *DbCwCache;

  DbNvData = (VOID *)Host->nvram.mem.DbNvdata;
  DbCwCache = &(DbNvData->DbCwCache[Socket]);

  //
  // If the cache has not been initialized for this socket, do it now
  //
  if (DbCwCache->State.Bits.Initialized == 0) {
    InitDbCwCache (DbCwCache);
  }
  return (DbCwCache);
}

/**
  The byte and bit positions into the Valid array for Ch, SubCh and Dimm.

  @param[in]  Ch                - Channel number (80-bit channel)
  @param[in]  SubCh             - Subchannel number
  @param[in]  Dimm              - DIMM number within the channel
  @param[in]  Strobe            - Data buffer number per sub channel
  @param[out] BitMask           - Bit mask for the Valid array byte
  @param[out] ByteOffsetInArray - Byte offset within the Valid array

  @retval MRC_STATUS_SUCCESS - Valid bit offset is found
  @retval MRC_STATUS_FAILURE - Array index is invalid
**/
MRC_STATUS
GetDbCwCacheEntryValidBitMaskAndBtyeOffsets (
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT8     Dimm,
  IN  UINT8     Strobe,
  OUT UINT8     *BitMask,
  OUT UINT16    *ByteOffsetInArray
  )
{
  UINT16    BitOffsetInArray;

  //
  // Calculate the Valid bit position in the arrayby calculate buffer <<0 + Dimm <<3 + SubCh << 4 + Ch <<5;
  //
  BitOffsetInArray = (Ch * VALID_BIT_CH_OFFSET_DB_CW) +
                     (SubCh * VALID_BIT_SUB_CH_OFFSET_DB_CW) +
                     (Dimm * VALID_BIT_DIMM_OFFSET_DB_CW) +
                     (Strobe * VALID_BIT_STROBE_OFFSET_DB_CW) ;

  //
  // Calculate the byte within the Valid array that contains the Valid bit for Ch, SubCh, Dimm Strobe
  //
  *ByteOffsetInArray = BitOffsetInArray / (MAX_BITS_IN_BYTE);

  if (*ByteOffsetInArray >= SIZE_VALID_ARRAY_DB_CW) {
    //
    // The array index is out of bounds
    //
    RcDebugPrintWithDevice (SDBG_ERROR, NO_SOCKET, Ch, Dimm, SubCh, NO_RANK, Strobe, NO_BIT,
    "WARNING - DB Control Word cache Valid array index is out of bounds\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_190);
    return MRC_STATUS_FAILURE;
  }
  *BitMask = 1 << (BitOffsetInArray % (MAX_BITS_IN_BYTE));
  return MRC_STATUS_SUCCESS;
}

/**
  Return TRUE if the Valid bit is set for the mode register instance
  specified by Ch, SubCh and Dimm Rank.
  This indicates the cache entry has been updated at least once.

  @param[in]  Valid     - Valid arrary
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - Subchannel number
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Buffer    - Data buffer number per sub channel


  @retval TRUE  - Valid bit is set
  @retval FALSE - Valid bit is not set
**/
BOOLEAN
IsDbCwCacheEntryValid (
  IN  UINT8     Valid[SIZE_VALID_ARRAY_DB_CW],
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT8     Dimm,
  IN  UINT8     Buffer
  )
{
  MRC_STATUS  Status;
  UINT16      ByteOffsetInArray;
  UINT8       BitMask;

  Status = GetDbCwCacheEntryValidBitMaskAndBtyeOffsets (Ch, SubCh, Dimm, Buffer, &BitMask, &ByteOffsetInArray);

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
  @param[in]  Buffer    - Data buffer number per sub channel

  @retval N/A
**/
VOID
SetDbCwCacheEntryValid (
  IN  UINT8     Valid[SIZE_VALID_ARRAY_DB_CW],
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT8     Dimm,
  IN  UINT8     Buffer
  )
{
  MRC_STATUS  Status;
  UINT16      ByteOffsetInArray;
  UINT8       BitMask;

  Status = GetDbCwCacheEntryValidBitMaskAndBtyeOffsets (Ch, SubCh, Dimm, Buffer, &BitMask, &ByteOffsetInArray);

  if (Status == MRC_STATUS_SUCCESS) {

    Valid[ByteOffsetInArray] |= BitMask;
  }
}

/**
  Return the cached value of the DB CW specified by Address.
  If the CW is in the cache list, return the contents in the caller's Data buffer.
  If the CW is not in the cache list, do not modify the caller's Data buffer.

  @param[in]     Socket     - Socket number
  @param[in]     Ch         - Channel number (80-bit channel)
  @param[in]     SubCh      - Subchannel within the channel
  @param[in]     Dimm       - DIMM number within the channel
  @param[in]     Strobe     - Data buffer number per sub channel
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
AccessDbCwCache (
  IN     UINT8               Socket,
  IN     UINT8               Ch,
  IN     UINT8               SubCh,
  IN     UINT8               Dimm,
  IN     UINT8               Strobe,
  IN     DB_CW_ACCESS_TYPE   AccessType,
  IN     UINT8               Page,
  IN     UINT8               Address,
  IN OUT UINT8               *Data
  )
{
  MRC_STATUS                  Status;
  PSYSHOST                    Host;
  UINT8                       CwIndex;
  SOCKET_DB_CW               *DbCwCache;
  DB_CW_REG                  *CwListPtr;
  UINT8                      CwListLength = 0;
  BOOLEAN                     IsDirectCw;

  Status = MRC_STATUS_GROUP_NOT_SUPPORTED;

  if (Address < DB_DIRECT_CONTROL_WORD_INITIAL_OFFSET) {
    //
    // Address is out of DB CW range
    //
    return Status;
  }

  Host = GetSysHostPointer ();
  DbCwCache = GetDbCwCache (Host, Socket);
  //
  // Direct control words: 0x80~0xDF
  // Paged COntrol words: 0xE0~0xFF
  //
  if ( Address < DB_PAGED_CONTROL_WORD_INITIAL_OFFSET ) {
    IsDirectCw = TRUE;
  } else {
    IsDirectCw = FALSE;
  }
  // Get Cw list Length and pointer
  //
  CwListPtr = GetCwListPointer(IsDirectCw, Page, DbCwCache, &CwListLength);
  if (CwListPtr == NULL) {
    RcDebugPrint (SDBG_MAX, "WARNING - GetCwListPointer called with an unsupported page number (Page = 0x%x)\n", Page);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_191);
    return MRC_STATUS_FAILURE;
  }
  //
  // Look for matching control word address in the CW list
  //
  for (CwIndex = 0; CwIndex < CwListLength; CwIndex++) {
    if (CwListPtr[CwIndex].Offset == Address) {
      if (AccessType == DbCwRead) {
        if (!IsDbCwCacheEntryValid (CwListPtr[CwIndex].Valid, Ch, SubCh, Dimm, Strobe)) {
          RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, SubCh, NO_RANK, Strobe, NO_BIT,
           "WARNING - DB PG[%x]RW%02x: invalid. DbCwRead was performed before DbCwWrite.\n", Page, Address);
        }
        *Data = CwListPtr[CwIndex].Cache[Ch][SubCh][Dimm][Strobe].Data;
      } else {
        CwListPtr[CwIndex].Cache[Ch][SubCh][Dimm][Strobe].Data = *Data;
        SetDbCwCacheEntryValid (CwListPtr[CwIndex].Valid, Ch, SubCh, Dimm, Strobe);
      }
      Status = MRC_STATUS_SUCCESS;
      break;
    }
  } // CwIndex

  if (Status == MRC_STATUS_GROUP_NOT_SUPPORTED) {
    //
    // This CW is missing from the cache
    // Update DB_CW_DIRECT_CACHE_LIST DB_CW_PAGE_CACHE_LIST and DB_DATA_1S to include the missing CW
    //
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, SubCh, NO_RANK, Strobe, NO_BIT,
    "WARNING - DB PG[%x]RW%02x: not found in the cache list\n", Page, Address);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_191);

    return MRC_STATUS_FAILURE;
  }

  return Status;
}
/**
  Determine if a control word is in the exception list
  These control words must not be restored to HW from cache

  @param[in]  Address           - Control word address

  @retval TRUE if CW is in the exception list
  @retval FALSE otherwise
**/
BOOLEAN
IsInDbCwExceptionList (
  IN  UINT8   Address
  )
{
  UINT8         Index;
  UINT8         CwExceptionList[] = {DDR5_DATA_BUFFER_RW81_REG, DDR5_DATA_BUFFER_RW92_REG, DDR5_DATA_BUFFER_RW93_REG};

  for (Index = 0; Index < ARRAY_SIZE (CwExceptionList); Index++) {
    if ((Address == CwExceptionList[Index])) {
      return TRUE;
    }
  }
  return FALSE;
}
#endif // MAX_SOCKET <= 4

/**
  Write an entry in the DB control word cache

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - SubCh number
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Strobe    - Data buffer number per sub channel
  @param[in]  Page      - Page number
  @param[in]  Address   - The MRW address
  @param[in]  Data      - New value to write to the cache

  @retval MRC_STATUS_SUCCESS if cache is updated
  @retval MRC_STATUS_FAILURE if the cache is not updated
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
**/
MRC_STATUS
EFIAPI
WriteDbCwCache (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubCh,
  IN  UINT8       Dimm,
  IN  UINT8       Strobe,
  IN  UINT8       Page,
  IN  UINT8       Address,
  IN  UINT8       Data
  )
{
#if MAX_SOCKET <= 4
  MRC_STATUS                  Status;
  UINT8                       MaxChDdr;


  MaxChDdr    = GetMaxChDdr ();
  Status      = MRC_STATUS_FAILURE;

  if ((Socket    >= MAX_SOCKET)       ||
      (Ch        >= MaxChDdr)         ||
      (SubCh     >= SUB_CH)           ||
      (Dimm      >= MAX_DIMM))
  {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, SubCh, NO_RANK, Strobe, NO_BIT,
      "WARNING - WriteDbCwCache called with an invalid parameter (SubCh = 0x%x)\n", SubCh);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_193);
    return MRC_STATUS_FAILURE;
  }

  Status = AccessDbCwCache (Socket, Ch, SubCh, Dimm, Strobe, DbCwWrite, Page, Address, &Data);
  return Status;
#else
  return MRC_STATUS_FAILURE;
#endif // MAX_SOCKET <= 4
}

/**
  Return an entry from the DB control word cache

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - Subchannel within the channel
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Strobe    - Data buffer number per sub channel
  @param[in]  Page      - The CW page number
  @param[in]  Address   - The MRW address
  @param[out] Data      - Poitner to caller's data buffer

  @retval MRC_STATUS_SUCCESS if cache entry is returned
  @retval MRC_STATUS_FAILURE if the cache entry is not supported
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
**/
MRC_STATUS
EFIAPI
ReadDbCwCache (
  IN      UINT8       Socket,
  IN      UINT8       Ch,
  IN      UINT8       SubCh,
  IN      UINT8       Dimm,
  IN      UINT8       Strobe,
  IN      UINT8       Page,
  IN      UINT8       Address,
     OUT  UINT8       *Data
  )
{
#if MAX_SOCKET <= 4
  MRC_STATUS                  Status;
  UINT8                       MaxChDdr;

  MaxChDdr    = GetMaxChDdr ();

  if ((Socket >= MAX_SOCKET) ||
      (Ch     >= MaxChDdr)   ||
      (SubCh  >= SUB_CH)     ||
      (Dimm   >= MAX_DIMM))
  {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, SubCh, Strobe, NO_RANK, NO_BIT,
      "WARNING - ReadDbCwCache called with an invalid parameter\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_193);
    return MRC_STATUS_FAILURE;
  }

  Status = AccessDbCwCache (Socket, Ch, SubCh, Dimm, Strobe, DbCwRead, Page, Address, Data);
  return Status;
#else
  return MRC_STATUS_FAILURE;
#endif // MAX_SOCKET <= 4
}

/**
  Restore cached DB control words to the DB

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
RestoreLrbufferControlWordsFromCache (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm
 )
{
#if MAX_SOCKET <= 4
  PSYSHOST                    Host;
  UINT8                       SubCh;
  UINT8                       Rank;
  UINT8                       MaxChDdr;
  UINT8                       SubChMSVx8 = BUFFERS_PER_SUBCH;
  UINT8                       Strobe;
  UINT8                       PageIndex;
  UINT8                       Page;
  UINT8                       Address;
  UINT8                       Data;
  UINT8                       CachedPage;
  UINT8                       PrevChannelModeOperation = 0;
  UINT8                       CwIndex;
  SOCKET_DB_CW               *DbCwCache;
  DB_CW_REG                  *CwListPtr;
  UINT8                      CwListLength = 0;
  BOOLEAN                     IsDirectCw;;
  UINT8                      PageList[] = {DB_CW_PAGE_LIST};

  MaxChDdr   = GetMaxChDdr ();

  if ((Socket >= MAX_SOCKET) ||
      (Ch     >= MaxChDdr)   ||
      (Dimm   >= MAX_DIMM) )
  {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "WARNING - RestoreDbControlWordsFromCache called with an invalid parameter\n");
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_205);
    return MRC_STATUS_FAILURE;
  }

  //
  // Direct Cws are globally accessed for ranks, some paged Cws are accessed through RWDF to select Rank
  // Rank Paramter to SendMrw don't care here
  //
  Rank       = 0;

  Host       = GetSysHostPointer ();
  DbCwCache  = GetDbCwCache (Host, Socket);

  if ((IsChannelEnabled (Socket, Ch) == FALSE) ||
      (IsDimmPresent (Socket, Ch, Dimm) == FALSE))
  {
    return MRC_STATUS_FAILURE;
  }

  if (IsBrsPresent (Host, Socket)) {
    PrevChannelModeOperation = Host->ChannelModeOperation;
    Host->ChannelModeOperation= ModeDdr5FullChannel;
  }

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

    for (Strobe = 0; Strobe < SubChMSVx8; Strobe++) {

      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }

      //
      // Get Cws Data from Cws List
      //
      for (PageIndex = 0; PageIndex < ARRAY_SIZE (PageList); PageIndex++) {
        Page = PageList[PageIndex];
        //
        // {DB_CW_PAGE_LIST}[0] is used to map DirectCw list {DB_CW_INIT_OFFSET_LIST}[0]
        //
        IsDirectCw = (PageIndex == 0);
        //
        // Get Cw list Length and pointer
        //
        CwListPtr = GetCwListPointer(IsDirectCw, Page, DbCwCache, &CwListLength);
        if (CwListPtr == NULL) {
          RcDebugPrint (SDBG_MAX, "WARNING - GetCwListPointer called with an unsupported page number (Page = 0x%x)\n", Page);
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_191);
          return MRC_STATUS_FAILURE;
        }
        for (CwIndex = 0; CwIndex < CwListLength; CwIndex++) {
          if (IsDbCwCacheEntryValid (CwListPtr[CwIndex].Valid, Ch, SubCh, Dimm, Strobe)) {
            Data    = CwListPtr[CwIndex].Cache[Ch][SubCh][Dimm][Strobe].Data;
            Address = CwListPtr[CwIndex].Offset;
            if (!IsDirectCw) {
              //
              // Switch Pages if necessary for Paged Control words
              //
              CachedPage = LRDIMM_DB_PAGE0;
              ReadDbCwCache (Socket, Ch, SubCh, Dimm, Strobe, LRDIMM_DB_PAGE0, RDIMM_RWDF, &CachedPage);
              if (Page != CachedPage) {
                //
                // Set page number to Page Register and update cache
                //
                SendMrwPba (Host, Socket, Ch, 1 << SubCh, Dimm, Rank, Strobe, RDIMM_RWDF, Page);
                WriteDbCwCache (Socket, Ch, SubCh, Dimm, Strobe, LRDIMM_DB_PAGE0, RDIMM_RWDF, Page);
              }
            } // Paged control Words
            if (!IsInDbCwExceptionList (Address)) {
              SendMrwPba (Host, Socket, Ch, 1 << SubCh, Dimm, Rank, Strobe, Address, Data);
            }
          } // Valid
        } // CwIndex
      } // PageIndex

    } // Strobe
  } // Sub Ch

  if (IsBrsPresent (Host, Socket)) {
    Host->ChannelModeOperation = PrevChannelModeOperation;
  }
  return MRC_STATUS_SUCCESS;
#else
 return MRC_STATUS_FAILURE;
#endif // MAX_SOCKET <= 4
}

/**
  Display LrData Buffer control word contents from the cached values

  @param[in]  Socket      - Socket number

  @retval   N/A
**/
VOID
EFIAPI
DumpDbCwCache (
  IN  UINT8     Socket
  )
{
#if MAX_SOCKET <= 4
#ifdef DEBUG_CODE_BLOCK
  PSYSHOST                    Host;
  UINT8                       Ch;
  UINT8                       SubCh;
  UINT8                       Dimm;
  UINT8                       MaxChDdr;
  UINT8                       PageIndex;
  UINT8                       Page;
  UINT8                       Strobe;
  UINT8                       Address;
  UINT8                       DirectIndex;
  UINT8                       Data;
  UINT16                      ListLength;
  UINT8                       PrevChannelModeOperation = 0;
  UINT32                      DbgLevel;
  SOCKET_DB_CW               *DbCwCache;
  DB_CW_REG                  *CwListPtr;
  UINT8                       DirectAddress;
  UINT8                       SubChMSVx8 = BUFFERS_PER_SUBCH;
  UINT8                       CwIndex = 0;
  UINT8                       CwListLength = 0;
  UINT8                       PageList[] = {DB_CW_PAGE_LIST};

  Host = GetSysHostPointer ();
  DbCwCache = GetDbCwCache (Host, Socket);
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

  RcDebugPrint (DbgLevel, "\nSTART_DATA_DB_CW_CACHE\n");

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

          ListLength = ARRAY_SIZE (DbCwCache->DirectCw);

            for (Strobe = 0; Strobe < SubChMSVx8; Strobe++) {

              if (IsStrobeNotValidDdr5 (Host, Strobe)) {
                continue;
              }

              RcDebugPrint (DbgLevel, "\nDirect Addressed Control Words\n");

              for (DirectIndex = 0; DirectIndex < ListLength; DirectIndex++) {
                DirectAddress = DbCwCache->DirectCw[DirectIndex].Offset;
                RcDebugPrintWithDevice (DbgLevel, Socket, Ch, Dimm, SubCh, NO_RANK, Strobe, NO_BIT, "RW%02x: ", DirectAddress);
                if (IsDbCwCacheEntryValid (DbCwCache->DirectCw[DirectIndex].Valid, Ch, SubCh, Dimm, Strobe)) {
                  Data = DbCwCache->DirectCw[DirectIndex].Cache[Ch][SubCh][Dimm][Strobe].Data;
                  RcDebugPrint (DbgLevel, " 0x%02x", Data);
                } else {
                  RcDebugPrint (DbgLevel, "   - ");
                } // Valid
                RcDebugPrint (DbgLevel, "\n");
              } // DirectIndex

              RcDebugPrint (DbgLevel, "\nPaged Control Words\n");
              //
              // DB_CW_PAGE_LIST[0] is mapped to direct control words list
              // Paged Control word list starts from DB_CW_PAGE_LIST[1]
              //
              for (PageIndex = 1; PageIndex < ARRAY_SIZE (PageList); PageIndex++) {
                Page = PageList[PageIndex];
                CwListPtr = GetCwListPointer(FALSE, Page, DbCwCache, &CwListLength);
                if (CwListPtr == NULL) {
                  RcDebugPrint (SDBG_MAX, "WARNING - GetCwListPointer called with an unsupported page number (Page = 0x%x)\n", Page);
                  RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_191);
                  return;
                }
                for (CwIndex = 0; CwIndex < CwListLength; CwIndex++) {
                  Address = CwListPtr[CwIndex].Offset;
                  RcDebugPrintWithDevice (DbgLevel, Socket, Ch, Dimm, SubCh, NO_RANK, Strobe, NO_BIT, "PG[%x]RW%02x: ", Page, Address);
                  if (IsDbCwCacheEntryValid (CwListPtr[CwIndex].Valid, Ch, SubCh, Dimm, Strobe)) {
                    Data = CwListPtr[CwIndex].Cache[Ch][SubCh][Dimm][Strobe].Data;
                    RcDebugPrint (DbgLevel, " 0x%02x", Data);
                  } else {
                    RcDebugPrint (DbgLevel, "   - ");
                  } // Valid
                  RcDebugPrint (DbgLevel, "\n");
                } // CwIndex
              } // PageIndex

            } // Strobe
        } // Sub Ch
    } // Dimm
  } // Ch

  RcDebugPrint (DbgLevel, "\nSTOP_DATA_DB_CW_CACHE\n\n");

  if (IsBrsPresent (Host, Socket)) {
    Host->ChannelModeOperation= PrevChannelModeOperation;
  }
#endif // DEBUG_CODE_BLOCK
#endif // MAX_SOCKET <= 4
}