/** @file
  Routine for lookup table scan/search related.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#include <Library/RasDebugLib.h>
#include <Library/LookupTable.h>

BOOLEAN mIsSouthIehInLookupTable = FALSE;

/**
  Check Ieh lookup table entry validation

  @param   LookupTblEntry  -- pointer to a LookupTblEntry

  @retval  TRUE/FALSE

**/
BOOLEAN
EFIAPI
IsLookupTableEntryValid (
  IN  LOOKUP_TBL_ENTRY  *LookupTblEntry
  )
{
  if (LookupTblEntry->IehDevType > NullDevice
    && LookupTblEntry->IehDevType < IehDevTypeEnd ) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  get Ieh lookup table entry pointer per stack, bitmap and SharedIndex

  @param   Stack  -- stack index
  @param   BitIdx -- bitmap index
  @param   SharedIndex -- The device index that share the same BitIdx, if not shared it should be 0
  @retval  bitmap table pointer

**/
LOOKUP_TBL_ENTRY *
EFIAPI
GetIehLookupTableEntry (
  IN  UINT8           Stack,
  IN  UINT8           BitIdx,
  IN  UINT8           SharedIndex
  )
{
  LOOKUP_TBL_ENTRY  *LookupTblEntry;
  UINT32            Index;

  LookupTblEntry = GetLookupTable ();

  if ( LookupTblEntry == NULL) {
    return NULL;
  }

  for (Index = 0; Index < MAX_BITMAP; Index ++){

    if ( LookupTblEntry[Index].IehDevType == IehDevTypeEnd) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW,"[IEH]Lookup table entry not found for Stack 0x%x, BitIdx 0x%x, SharedIndex 0x%x \n",
        Stack, BitIdx, SharedIndex));
      return NULL;
    }
    //
    // Find the first entry of this bitIdx
    //
    if ( LookupTblEntry[Index].Stack == Stack && LookupTblEntry[Index].BitIndex == BitIdx) {
      //
      // Make sure SharedIndex is valid
      //
      if (LookupTblEntry[Index + SharedIndex].BitIndex == BitIdx) {
        //
        // further check if this entry is valid
        //
        if (IsLookupTableEntryValid (LookupTblEntry)) {
          return &LookupTblEntry[Index + SharedIndex];
        }
      }
      //
      // if execution came here, means wrong SharedIndex
      //
      RAS_DEBUG ((LEVEL_FUNC_FLOW,"[IEH] Wrong Shared Index for Stack 0x%x, BitIdx 0x%x, SharedIndex 0x%x ,\n",
        Stack, BitIdx, SharedIndex));
      return NULL;
    }
  }

  RAS_DEBUG ((LEVEL_FUNC_FLOW,"[IEH] Lookup table entry not found - Stack 0x%x, BitIdx 0x%x, SharedIndex 0x%x \n",
    Stack, BitIdx, SharedIndex));
  return NULL;
}

/**
  Check if south Ieh in lookup table

  @param   None

  @retval  TRUE/FALSE

**/
BOOLEAN
EFIAPI
IsSouthIehInLookupTable (
  VOID
  )
{
  LOOKUP_TBL_ENTRY  *LookupTblEntry;
  UINT32            Index;

  LookupTblEntry = GetLookupTable ();

  if ( LookupTblEntry == NULL) {
    return FALSE;
  }

  for (Index = 0; Index < MAX_BITMAP; Index ++){
    if (LookupTblEntry[Index].IehDevType == IehDevTypeEnd) {
      return FALSE;
    } else if (LookupTblEntry[Index].IehDevType == SatelliteIehSouth) {
      return TRUE;
    } else {
      continue;
    }
  }
  return FALSE;
}