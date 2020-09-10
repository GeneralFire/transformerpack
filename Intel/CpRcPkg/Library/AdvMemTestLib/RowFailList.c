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

#include "MemTestFunc.h"
#include <Library/MemRcLib.h>
#include <ReferenceCodeFatalErrors.h>

//
// Code section
//

/**

  Compare most significant channel address bits excluding row bits

  @param CurAddr          - First address to compare
  @param RowAddr          - Second address to compare

  @retval   TRUE for match, otherwise FALSE

**/
BOOLEAN
CurrentAddrMatch (
  ROW_ADDR CurAddr,
  ROW_ADDR RowAddr
  )
{
  //
  // Mask fields that we don't care to match
  //
  RowAddr.Bits.row = 0;
  CurAddr.Bits.row = 0;
  RowAddr.Bits.upperBgMask = 0;
  CurAddr.Bits.upperBgMask = 0;

  if (CurAddr.Data == RowAddr.Data) {
    return TRUE;
  } else {
    return FALSE;
  }
} //CurrentAddrMatch


/**

  Shifts row fail list to right by number of entries starting at given index

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param Socket             - Socket Id
  @param Ch                 - Channel number
  @param Index              - List index to begin the shift operation
  @param NumEntries         - Number of entries to shift

  @retval status - SUCCESS/FAILURE

**/
UINT32
ShiftFailRangeListRight (
  PSYSHOST Host,
  UINT8 Socket,
  UINT8 Ch,
  UINT32 Index,
  UINT32 NumEntries
  )
{
  INT32 CurEntry;
  UINT32 FailMax;
  UINT32 Status = SUCCESS;

  FailMax = GetRowFailMax (Host, Ch);

  //For last entry in list down to index, move entry up by NumEntries amount
  if (FailMax + NumEntries < MAX_FAIL_RANGE) {
    if (FailMax != 0) {
      for (CurEntry = FailMax - 1; CurEntry >= (INT32) Index; CurEntry--) {
        *GetRowFailRange (Host, Ch, CurEntry + NumEntries) = *GetRowFailRange (Host, Ch, CurEntry);
      }
    }
    SetRowFailMax (Host, Ch, FailMax + NumEntries);

  } else {
    Status = FAILURE;
  }
  return Status;

} //ShiftFailRangeListRight


/**

  Shifts row fail list to left by number of entries starting at given index

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param Socket             - Socket Id
  @param Ch                 - Channel number
  @param Index              - List index to begin the shift operation
  @param NumEntries         - Number of entries to shift

  @retval status - SUCCESS/FAILURE

**/
UINT32
ShiftFailRangeListLeft (
  PSYSHOST Host,
  UINT8 Socket,
  UINT8 Ch,
  UINT32 Index,
  UINT32 NumEntries
  )
{
  UINT32 CurEntry;
  UINT32 FailMax;
  UINT32 Status = SUCCESS;

  FailMax = GetRowFailMax (Host, Ch);

  if (Index >= NumEntries) {

    //For index up to last entry in list, move entry down by NumEntries amount
    for (CurEntry = Index; CurEntry < FailMax; CurEntry++) {
      *GetRowFailRange (Host, Ch, CurEntry - NumEntries) = *GetRowFailRange (Host, Ch, CurEntry);
    }
    SetRowFailMax (Host, Ch, FailMax - NumEntries);

  } else {

    Status = FAILURE;
  }
  return Status;
} // ShiftFailRangeListLeft


UINT32
CombineFailRangeList (
  PSYSHOST Host,
  UINT8 Socket,
  UINT8 Ch,
  UINT32 Index
  )
{
  ROW_FAIL_RANGE *FailRangePtr1;
  ROW_FAIL_RANGE *FailRangePtr2;

  FailRangePtr1 = GetRowFailRange (Host, Ch, Index);
  FailRangePtr2 = GetRowFailRange (Host, Ch, Index + 1);

  //
  // Combine list entries if possible
  //
  if ((Index + 1 < MAX_FAIL_RANGE) &&
      CurrentAddrMatch (FailRangePtr1->addr, FailRangePtr2->addr) &&
      (FailRangePtr2->addr.Bits.row == FailRangePtr1->addr.Bits.row + FailRangePtr1->size) &&
      (FailRangePtr2->mask[0] == FailRangePtr1->mask[0]) &&
      (FailRangePtr2->mask[1] == FailRangePtr1->mask[1]) &&
      (FailRangePtr2->mask[2] == FailRangePtr1->mask[2])) {

    FailRangePtr2->addr.Bits.row = FailRangePtr1->addr.Bits.row;
    FailRangePtr2->size += FailRangePtr1->size;

    ShiftFailRangeListLeft (Host, Socket, Ch, Index + 1, 1);
    SetRowFailIndex (Host, Ch, Index);

  } else {
    return FAILURE;
  }
  return SUCCESS;

} // CombineFailRangeList


/**

  Updates Row failure list with last failure, coalescing failure ranges where possible

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param socket             - Socket Id
  @param ddrChEnMap         - List of channels with rank that was tested
  @param cpgcErrorStatus[MAX_CH] - the failure information per channel
  @param TestStatus[MAX_CH] - Pass/fail status for the test per channel

  @retval status - SUCCESS/FAILURE

**/
UINT32
RemoveRowFailRange (
  PSYSHOST          Host,
  UINT8             Socket,
  UINT8             Ch,
  UINT32            FailIndex
)
{
  UINT32 Status = SUCCESS;

  //
  // Shift list left by 1 at next entry
  //
  ShiftFailRangeListLeft (Host, Socket, Ch, FailIndex + 1, 1);
  SetRowFailIndex (Host, Ch, FailIndex);

  //
  // Combine list entries (index - 1, index) if possible
  //
  if (FailIndex) {
    CombineFailRangeList (Host, Socket, Ch, FailIndex - 1);
  }
  return Status;

} // RemoveRowFailRange


/**

  Updates Row failure list with last failure, coalescing failure ranges where possible

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param socket             - Socket Id
  @param ch                 - Channel of the new failure
  @param CheckMaskError     - Check for previously masked range
  @param NewFail            - New failure information

  @retval status - SUCCESS/FAILURE

**/
UINT32
UpdateRowFailList (
  PSYSHOST          Host,
  UINT8             socket,
  UINT8             ch,
  BOOLEAN           CheckMaskError,
  ROW_FAIL_RANGE    NewFail
  )
{
  UINT32 index;
  UINT32 status = SUCCESS;
  ROW_FAIL_RANGE *FailRangePtr;
  ROW_FAIL_RANGE *FailRangePtrNext1;
  ROW_FAIL_RANGE *FailRangePtrNext2;
  ROW_ADDR SearchFail;
  UINT32 SearchSize;
  UINT32 SearchMask[3];
  UINT8 done;

  //
  // Search FailRange list for position to insert new faiure data
  //
  index = 0;
  done = 0;
  while (!done && (index < GetRowFailMax (Host, ch)) && (status == SUCCESS)) {

    FailRangePtr = GetRowFailRange (Host, ch, index);
    FailRangePtrNext1 = GetRowFailRange (Host, ch, index + 1);
    FailRangePtrNext2 = GetRowFailRange (Host, ch, index + 2);

    SearchFail = FailRangePtr->addr;
    SearchSize = FailRangePtr->size;

    //
    // Check search criteria to move to next entry
    // 1. Non-matching bank, subrank, rank
    // 2. New failure is greater than current entry row range
    // 3. New failure is equal to next entry row
    //
    if (!CurrentAddrMatch (SearchFail, NewFail.addr) || (SearchFail.Bits.row + SearchSize < NewFail.addr.Bits.row) ||
        ((index + 1 < GetRowFailMax (Host, ch)) && (CurrentAddrMatch (FailRangePtrNext1->addr, NewFail.addr)) && (FailRangePtrNext1->addr.Bits.row == NewFail.addr.Bits.row))) {
      index++;
      continue;
    }

    SearchMask[0] = FailRangePtr->mask[0];
    SearchMask[1] = FailRangePtr->mask[1];
    SearchMask[2] = FailRangePtr->mask[2];

    //
    // Append new failure to end of current entry
    // Row address + 1
    // Same DQ mask
    //
    if ((NewFail.addr.Bits.row >= SearchFail.Bits.row) &&
        (NewFail.addr.Bits.row < SearchFail.Bits.row + SearchSize) &&
        ((SearchMask[0] | NewFail.mask[0]) == SearchMask[0]) &&
        ((SearchMask[1] | NewFail.mask[1]) == SearchMask[1]) &&
        ((SearchMask[2] | NewFail.mask[2]) == SearchMask[2])) {

      if (CheckMaskError) {

        RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "UpdateRowFailures: New failure occured in previously masked range:  fail addr = 0x%08x, error status = 0x%02x%08x%08x, search addr = 0x%08x, size = 0x%08x, mask = 0x%02x%08x%08x\n",
          NewFail.addr.Data, NewFail.mask[2], NewFail.mask[1], NewFail.mask[0], SearchFail.Data, SearchSize, SearchMask[2], SearchMask[1], SearchMask[0]);

        // Something went wrong if we got failure on test address that was previously masked
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_99);
      } else {

        SetRowFailIndex (Host, ch, index);

        //
        // Combine list entries (index, index + 1), then (index-1, index) if possible
        //
        CombineFailRangeList (Host, socket, ch, index);
        if (index > 0) {
          CombineFailRangeList (Host, socket, ch, index - 1);
        }
        done = 1;
      }
    }

    //
    // Add 1 to current entry size
    //
    else if ((NewFail.addr.Bits.row == SearchFail.Bits.row + SearchSize) &&
             (NewFail.mask[0] == SearchMask[0]) &&
             (NewFail.mask[1] == SearchMask[1]) &&
             (NewFail.mask[2] == SearchMask[2])) {

        FailRangePtr->size++;
        SetRowFailIndex (Host, ch, index);
        done = 1;

        //
        // Combine list entries (index, index + 1) if possible
        //
        CombineFailRangeList (Host, socket, ch, index);
    }

    //
    // Split existing entry, so shift by 2 at next entry
    // New DQ mask
    //
    else if ((NewFail.addr.Bits.row > SearchFail.Bits.row) &&
      (NewFail.addr.Bits.row < SearchFail.Bits.row + SearchSize - 1)) {

      // Shift list right by 2 at next entry
      if (ShiftFailRangeListRight (Host, socket, ch, index + 1, 2) == SUCCESS) {

        // Truncate current entry size;
        FailRangePtr->size = NewFail.addr.Bits.row - SearchFail.Bits.row;

        // Add new entry + 1 to report new failure added to previous mask;
        FailRangePtrNext1->size = 1;
        FailRangePtrNext1->addr = NewFail.addr;
        FailRangePtrNext1->mask[0] = SearchMask[0] | NewFail.mask[0];
        FailRangePtrNext1->mask[1] = SearchMask[1] | NewFail.mask[1];
        FailRangePtrNext1->mask[2] = SearchMask[2] | NewFail.mask[2];
        SetRowFailIndex (Host, ch, index);

        // Add new entry + 2 to report previous extra size
        FailRangePtrNext2->size = SearchSize - 1 - (NewFail.addr.Bits.row - SearchFail.Bits.row);
        FailRangePtrNext2->addr = NewFail.addr;
        FailRangePtrNext2->addr.Bits.row = NewFail.addr.Bits.row + 1;
        FailRangePtrNext2->mask[0] = SearchMask[0];
        FailRangePtrNext2->mask[1] = SearchMask[1];
        FailRangePtrNext2->mask[2] = SearchMask[2];
        done = 1;
      } else {
        status = FAILURE;
      }
    }

    //
    // Add new entry overlapping end of current entry
    // Same Row address as end of range (or range size = 1)
    // New DQ mask
    //
    else if (NewFail.addr.Bits.row == SearchFail.Bits.row + SearchSize - 1) {

      if (SearchSize > 1) {
        //
        // Shift list right by 1 at next entry
        //
        if (ShiftFailRangeListRight (Host, socket, ch, index + 1, 1) == SUCCESS) {

          FailRangePtr->size--;

          FailRangePtrNext1->size = 1;
          FailRangePtrNext1->addr = NewFail.addr;
          FailRangePtrNext1->mask[0] = SearchMask[0] | NewFail.mask[0];
          FailRangePtrNext1->mask[1] = SearchMask[1] | NewFail.mask[1];
          FailRangePtrNext1->mask[2] = SearchMask[2] | NewFail.mask[2];
          SetRowFailIndex (Host, ch, index + 1);
          done = 1;

          //
          // Combine list entries (index + 1, index + 2) if possible
          //
          CombineFailRangeList (Host, socket, ch, index + 1);
        } else {
          status = FAILURE;
        }
      } else {
        //
        // Replace current entry
        //
        FailRangePtr->mask[0] = SearchMask[0] | NewFail.mask[0];
        FailRangePtr->mask[1] = SearchMask[1] | NewFail.mask[1];
        FailRangePtr->mask[2] = SearchMask[2] | NewFail.mask[2];
        SetRowFailIndex (Host, ch, index);
        done = 1;
        //
        // Combine list entries (index, index + 1), then (index-1, index) if possible
        //
        CombineFailRangeList (Host, socket, ch, index);
        if (index) {
          CombineFailRangeList (Host, socket, ch, index - 1);
        }
      }
    }

    //
    // Update start of current entry to include fail row
    // Row address - 1
    // Same DQ mask
    //
    else if ((NewFail.addr.Bits.row == SearchFail.Bits.row - 1) &&
             (NewFail.mask[0] == SearchMask[0]) &&
             (NewFail.mask[1] == SearchMask[1]) &&
             (NewFail.mask[2] == SearchMask[2])) {

      FailRangePtr->size++;
      FailRangePtr->addr = NewFail.addr;
      SetRowFailIndex (Host, ch, index);
      done = 1;
      //
      // Combine list entries (index-1, index) if possible
      //
      if (index) {
        CombineFailRangeList (Host, socket, ch, index - 1);
      }
    }

    //
    // Add new entry overlapping the start of current entry
    // Same Row address as start of range
    // New DQ mask
    //
    else if (NewFail.addr.Bits.row == SearchFail.Bits.row) {

      if (SearchSize > 1) {
        //
        // Shift list right by 1 at current entry
        //
        if (ShiftFailRangeListRight (Host, socket, ch, index, 1) == SUCCESS) {

          FailRangePtr->size = 1;
          FailRangePtr->addr = NewFail.addr;
          FailRangePtr->mask[0] = SearchMask[0] | NewFail.mask[0];
          FailRangePtr->mask[1] = SearchMask[1] | NewFail.mask[1];
          FailRangePtr->mask[2] = SearchMask[2] | NewFail.mask[2];
          SetRowFailIndex (Host, ch, index);

          FailRangePtrNext1->size--;
          FailRangePtrNext1->addr.Bits.row++;
          done = 1;

          //
          // Combine list entries (index-1, index) if possible
          //
          CombineFailRangeList (Host, socket, ch, index - 1);
        } else {
          status = FAILURE;
        }
      }
    }

    //
    // Add new entry for higher row address with no overlap
    //
    else if (NewFail.addr.Bits.row >= SearchFail.Bits.row + SearchSize) {

      // Shift list right by 1 at next entry
      if (ShiftFailRangeListRight (Host, socket, ch, index + 1, 1) == SUCCESS) {
        FailRangePtrNext1->size = 1;
        FailRangePtrNext1->addr = NewFail.addr;
        FailRangePtrNext1->mask[0] = NewFail.mask[0];
        FailRangePtrNext1->mask[1] = NewFail.mask[1];
        FailRangePtrNext1->mask[2] = NewFail.mask[2];
        SetRowFailIndex (Host, ch, index + 1);
        done = 1;
      } else {
        status = FAILURE;
      }
    }
    //
    // Add new entry for lower row address with no overlap
    //
    else {

      // Shift list right by 1 at current entry
      if (ShiftFailRangeListRight (Host, socket, ch, index, 1) == SUCCESS) {
        FailRangePtr->size = 1;
        FailRangePtr->addr = NewFail.addr;
        FailRangePtr->mask[0] = NewFail.mask[0];
        FailRangePtr->mask[1] = NewFail.mask[1];
        FailRangePtr->mask[2] = NewFail.mask[2];
        SetRowFailIndex (Host, ch, index);
        done = 1;
      } else {
        status = FAILURE;
      }
    }
  } //while
  //
  // Append entry to end of list
  // Shift list right by 1 at end of list
  //
  if (!done) {
    if (ShiftFailRangeListRight (Host, socket, ch, index, 1) == SUCCESS) {
      FailRangePtr = GetRowFailRange (Host, ch, index);
      FailRangePtr->size = 1;
      FailRangePtr->addr = NewFail.addr;
      FailRangePtr->mask[0] = NewFail.mask[0];
      FailRangePtr->mask[1] = NewFail.mask[1];
      FailRangePtr->mask[2] = NewFail.mask[2];
      SetRowFailIndex (Host, ch, index);
    } else {
      status = FAILURE;
    }
  } // if !done

  return status;
}  //UpdateRowFailList

/**

  Searches the failure tracking list for NewFail and returns bit mask of the DQ bits with
  number of row failures greater or equal to the given threshoold.

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param socket             - Socket Id
  @param ch                 - Channel of the new failure
  @param NewFail            - New failure information
  @param Threshold          - Number of errors per DQ where mask bit is set
  @param DqMaskOver         - Bitmask of DQ having number of row errors in bank greater or equal to threshold

  @retval status - TRUE if any DQ has number of row errors greater or equal to threshold

**/
BOOLEAN
GetDqMaskOverThreshold (
  PSYSHOST          Host,
  UINT8             socket,
  UINT8             ch,
  ROW_FAIL_RANGE    NewFail,
  UINT32            Threshold,
  UINT32            DqMaskOver[3]
  )
{
  UINT32 PerLaneCounters[MAX_BITS];
  ROW_FAIL_RANGE *FailRangePtr;
  ROW_ADDR SearchFail;
  UINT32 SearchSize;
  UINT32 index;
  UINT32 i;
  
  for (i = 0; i < MAX_BITS; i++) {
    PerLaneCounters[i] = 0;
  }
  //
  // Search FailRange list for position to insert new faiure data
  //
  for (index = 0; index < GetRowFailMax (Host, ch); index++) {

    FailRangePtr = GetRowFailRange (Host, ch, index);

    SearchFail = FailRangePtr->addr;
    SearchSize = FailRangePtr->size;

    //
    // Check search criteria to move to next entry
    // 1. Non-matching bank, subrank, rank, or
    //
    if (CurrentAddrMatch (SearchFail, NewFail.addr)) {
      //&& ((SearchFail.Bits.row <= NewFail.addr.Bits.row) && (NewFail.addr.Bits.row < SearchFail.Bits.row + SearchSize))) {
      //
      // Update counters
      //
      for (i = 0; i < MAX_BITS; i++) {
        if (FailRangePtr->mask[i / 32] & (1 << (i % 32))) {
          PerLaneCounters[i] += SearchSize;
        }
      }
    }
  }
  //
  // Form new DQ mask
  //
  for (i = 0; i < 3; i++) {
    DqMaskOver[i] = NewFail.mask[i];
  }
  for (i = 0; i < MAX_BITS; i++) {
    if (PerLaneCounters[i] < Threshold) {
      DqMaskOver[i / 32] &= ~(1 << (i % 32));
    }
  }
  //
  // Return status
  //
  for (i = 0; i < 3; i++) {
    if (DqMaskOver[i] != 0) {
      return TRUE;
    }
  }
  return FALSE;
}

/**

  Updates Row failure list with last failure, coalescing failure ranges where possible

  @param[in]  Host                    - Pointer to sysHost, the system Host (root) structure
  @param[in]  socket                  - Socket Id
  @param[in]  ddrChEnMap              - List of channels with rank that was tested
  @param[in]  cpgcErrorStatus[MAX_CH] - the failure information per channel
  @param[in]  rowBits                 - Number of row bits supported by current logical rank
  @param[in]  baseBits                - Number of bank bits in SW loop
  @param[out] TestStatus[MAX_CH]      - Pass/fail status for the test per channel
  @param[in]  Direction               - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN

  @retval status - SUCCESS/FAILURE

**/
UINT32
UpdateRowFailures (
  IN     PSYSHOST          Host,
  IN     UINT8             socket,
  IN     UINT32            ddrChEnMap,
  IN     CPGC_ERROR_STATUS_MATS cpgcErrorStatus[MAX_CH],
  IN     UINT8             rowBits[MAX_CH],
  IN     UINT8             baseBits,
     OUT UINT8             TestStatus[MAX_CH],
  IN     UINT8             Direction
  )
{
  UINT32 errorStatus0;
  UINT32 errorStatus1;
  UINT32 errorStatus2;
  UINT32 Row;
  UINT32 OrgRow;
  UINT8 ch;
  UINT32 status = SUCCESS;
  ROW_FAIL_RANGE NewFail;
  ROW_FAIL_RANGE LastFail;
  ROW_FAIL_RANGE TempFail;
  UINT8 RetryState;
  UINT8 MaxChDdr;
  UINT32 DqMaskOver[3];
  UINT32 Index;
#ifdef DDR5_SUPPORT
  UINT8  ErrorStatusEccSubChA = 0;
  UINT8  ErrorStatusEccSubChB = 0;
#endif

  MaxChDdr = GetMaxChDdr ();
  RetryState = GetAmtRetryState (Host);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if (ddrChEnMap & (1 << ch)) {

      errorStatus0 = cpgcErrorStatus[ch].cpgcErrDat0S | cpgcErrorStatus[ch].cpgcErrDat2S;
      errorStatus1 = cpgcErrorStatus[ch].cpgcErrDat1S | cpgcErrorStatus[ch].cpgcErrDat3S;

      if (Host->nvram.mem.eccEn) {

        if (IsDdr5Present (Host, socket)) {
#ifdef DDR5_SUPPORT
          ErrorStatusEccSubChA = (cpgcErrorStatus[ch].cpgcErrEccS & BYTE_MASK) | ((cpgcErrorStatus[ch].cpgcErrEccS >> BYTE_WIDTH) & BYTE_MASK);
          ErrorStatusEccSubChB = ((cpgcErrorStatus[ch].cpgcErrEccS >> WORD_WIDTH) & BYTE_MASK) | (((cpgcErrorStatus[ch].cpgcErrEccS >> (WORD_WIDTH + BYTE_WIDTH))) & BYTE_MASK);
          errorStatus2 = (ErrorStatusEccSubChB << BYTE_WIDTH) | ErrorStatusEccSubChA;
#endif
        } else {
          errorStatus2 = (cpgcErrorStatus[ch].cpgcErrEccS & 0xFF) | ((cpgcErrorStatus[ch].cpgcErrEccS >> BYTE_WIDTH) & BYTE_MASK);
        }


      } else {
        errorStatus2 = 0;
      }

      TestStatus[ch] = 0;
      if (!(errorStatus0 | errorStatus1 | errorStatus2)) {
        //
        // retry 0, no fail - set last fail to default, test status = 0
        // retry 1, no fail - no last fail update, test status = 0
        // retry 2, no fail - no last fail update, test status = 1
        //
        switch (RetryState) {
        case 0:
          NewFail.addr.Data = 0;     // Set to Invalid state
          NewFail.mask[0] = 0;
          NewFail.mask[1] = 0;
          NewFail.mask[2] = 0;
          NewFail.size = 0;
          SetAmtLastFail (Host, ch, NewFail);
          break;
        case 2:
          GetAmtLastFail (Host, ch, &NewFail);
          TestStatus[ch] = 1;
          break;
        }

      } else {

        //
        // New fail address
        //
        NewFail.addr.Data = 0;
        if (IsDdr5Present (Host, socket)) {
#ifdef DDR5_SUPPORT
          if (((errorStatus0 != 0) || (ErrorStatusEccSubChA != 0)) &&
             ((errorStatus1 != 0) || (ErrorStatusEccSubChB != 0))) {
            //
            // Error happens on both sub-channels, then retry from the row which is failed early
            //
            if (Direction == MT_ADDR_DIR_UP) {
              NewFail.addr.Bits.row = MIN (cpgcErrorStatus[ch].cpgcErrRow, cpgcErrorStatus[ch].cpgcErrRowExt);
            } else {
              NewFail.addr.Bits.row = MAX (cpgcErrorStatus[ch].cpgcErrRow, cpgcErrorStatus[ch].cpgcErrRowExt);
            }
          } else if ((errorStatus0 != 0) || (ErrorStatusEccSubChA != 0)) {
            //
            // Error happens on sub-channel A only
            //
            NewFail.addr.Bits.row = cpgcErrorStatus[ch].cpgcErrRow;
          } else {
            //
            // Error happens on sub-channel B only
            //
            NewFail.addr.Bits.row = cpgcErrorStatus[ch].cpgcErrRowExt;
          }
#endif
        } else {
          NewFail.addr.Bits.row = cpgcErrorStatus[ch].cpgcErrRow;
        }

        NewFail.addr.Bits.bankPair = cpgcErrorStatus[ch].cpgcErrBank & ((1 << baseBits) - 1);
        NewFail.addr.Bits.logicalSubRank = cpgcErrorStatus[ch].cpgcSubrank;
        NewFail.addr.Bits.logicalRank = cpgcErrorStatus[ch].cpgcErrRank;
        NewFail.addr.Bits.valid = 1;
        NewFail.mask[0] = 0;
        NewFail.mask[1] = 0;
        NewFail.mask[2] = 0;
        NewFail.size = 1;

        //
        // retry 0, fail - save last fail, upperBgMask 0, test status = 1
        // retry 1, fail - update last fail with upperBgMask bit-0=1, test status = 1
        // retry 2, fail - update last fail with upperBgMask bit-1=1, test status = 1
        //
        if (RetryState) {
          GetAmtLastFail (Host, ch, &LastFail);
          TempFail = LastFail;
          TempFail.addr.Bits.upperBgMask = 0;
          if (TempFail.addr.Data == NewFail.addr.Data) {
            NewFail.addr.Bits.upperBgMask = LastFail.addr.Bits.upperBgMask | (1 << (cpgcErrorStatus[ch].cpgcErrBank >> baseBits));
            NewFail.mask[0] = LastFail.mask[0] | errorStatus0;
            NewFail.mask[1] = LastFail.mask[1] | errorStatus1;
            NewFail.mask[2] = LastFail.mask[2] | errorStatus2;

            //RcDebugPrint (SDBG_MAX,
            //  "NewFail.mask 0x%04x 0x%08x 0x%08x\n", NewFail.mask[2], NewFail.mask[1], NewFail.mask[0]);
          }
          else {
            // RC_ASSERT
          }
        }
        SetAmtLastFail (Host, ch, NewFail);
        TestStatus[ch] = 1;
      }
      //
      // Only update list state after all retries are done
      //
      if ((RetryState == 2) && TestStatus[ch] && NewFail.addr.Bits.upperBgMask) {

        if (UpdateRowFailList (Host, socket, ch, TRUE, NewFail) != SUCCESS) {
          status = FAILURE;
        } else {
          
          //
          // Call function passing in NewFail and max row threshold; output is a dq mask over threshold.
          // Function will search the current bank and create a DQ mask of the new DQ failures that occur on more than N number of rows in the bank. 
          //
          if (GetDqMaskOverThreshold (Host, socket, ch, NewFail, MAX_BAD_ROW_LIMIT_PER_BANK, DqMaskOver)) {
            //
            // Update list to apply DQ mask to whole bank
            //
            OrgRow = NewFail.addr.Bits.row;
            for (Index = 0; Index < 3; Index++) {
              NewFail.mask[Index] = DqMaskOver[Index];
            }
            for (Row = 0; Row < (UINT32)(1 << rowBits[ch]); Row++) {
              NewFail.addr.Bits.row = Row;
              if (UpdateRowFailList (Host, socket, ch, FALSE, NewFail) != SUCCESS) {
                status = FAILURE;
                break;
              }
            }
            //
            // Final update to restore RowFailIndex
            //
            NewFail.addr.Bits.row = OrgRow;
            if (UpdateRowFailList (Host, socket, ch, FALSE, NewFail) != SUCCESS) {
              status = FAILURE;
            }
          }
        }
      } // RetryState
    } // if ch mask
  } // for ch

  return status;

}  //UpdateRowFailures

/**

  Initializes data structures for the AMT feature

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param Socket             - Socket Id

  @retval   None

**/
VOID
ResetRowFailureList (
  PSYSHOST  Host,
  UINT8     Socket
)
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  //
  // Reset row fail list to test effectiveness of multiple Advanced Memtest options
  //
  if (Setup->mem.AdvMemTestResetList) {
    Host->var.mem.FailRangeInitDone[Socket] = 0;
    InitAmtDataStructures (Host, Socket);
  }
}
