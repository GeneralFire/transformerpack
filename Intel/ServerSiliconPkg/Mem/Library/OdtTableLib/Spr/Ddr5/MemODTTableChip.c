/** @file

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

#include <SysHost.h>
#include "DdrOdtPrivate.h"
#include "Ddr5OdtActivationTable.h"
#include "Ddr5OdtValueTable.h"
#include <Library/SysHostPointerLib.h>

/**
  Return pointer to the ODT activation table.
  The table is an array of ODT activation structures.
  Also returns the size of the array.

  @param [OUT] OdtActTablePtr  - Pointer to caller's ODT table pointer
  @param [OUT] OdtActTableSize - Pointer to caller's ODT table size variable.

  @retval N/A
**/
VOID
GetActivationTableOdt (
  OUT struct ddrOdtActivationTableEntry **OdtActTablePtr,
  OUT UINT32                             *OdtActTableSize
  )
{
  *OdtActTablePtr = &Ddr5OdtActTable[0];
  *OdtActTableSize = ARRAY_SIZE (Ddr5OdtActTable);
} // GetActivationTableOdt

/**
  Return pointer to the ODT value table.
  The table is an array of ODT value structures.
  Also returns the size of the array.

  @param [OUT] OdtValueTablePtr  - Pointer to caller's ODT table pointer
  @param [OUT] OdtValueTableSize - Pointer to caller's ODT table size variable.

  @retval N/A
**/
VOID
GetValuesTableOdt (
  OUT struct ddrOdtValueTableEntry **OdtValueTablePtr,
  OUT UINT16                        *OdtValueTableSize
  )
{
  *OdtValueTablePtr = &Ddr5OdtValueTable[0];
  *OdtValueTableSize = ARRAY_SIZE (Ddr5OdtValueTable);
} // GetValuesTableOdt

/**
  Return the correct slot designator for LRDIMM and AEP DIMM when walking
  the ODT activation table

  @param [IN] AepIsPresent  - 1 if AEP DIMM is present, 0 otherwise

  @retval LR_DIMM or AEP_DIMM
**/
UINT8
GetSlotTypeForActivationLookupOdt (
  IN UINT8 AepIsPresent
  )
{
  // 10nm ODT table makes no distinction between AEP and LDRIMM
  return LR_DIMM;
} // GetSlotTypeForActivationLookupOdt

/**
  Get the current DIMM frequency formatted as an "index" for the ODT value table
  (the DIMM slot population fields are set to default values)

  @param [IN] Socket  - Socket number

  @retval The current DIMM frequency formatted as an "index" for the ODT value table
  (the DIMM slot population fields are set to default values)
**/
UINT16
GetCurrentDimmFrequencyOdt (
  IN UINT8 Socket
  )
{
  PSYSHOST                    Host;
  ODT_VALUE_TABLE_ENTRY_INDEX CurrentDimmFrequencyWithDefaultPopulation;

  Host = GetSysHostPointer ();
  // Construct config index
  CurrentDimmFrequencyWithDefaultPopulation.Data = 0;
  CurrentDimmFrequencyWithDefaultPopulation.Bits.slot2 = EMPTY_DIMM;
  CurrentDimmFrequencyWithDefaultPopulation.Bits.freq = GetClosestFreq (Host, Socket);

  // Cap frequencies above DDR_4800
  if (CurrentDimmFrequencyWithDefaultPopulation.Bits.freq > DDR_4800) {
    CurrentDimmFrequencyWithDefaultPopulation.Bits.freq = DDR_4800;
  }

  // Adjust frequencies below DDR_3200
  if (CurrentDimmFrequencyWithDefaultPopulation.Bits.freq < DDR_3200) {
    CurrentDimmFrequencyWithDefaultPopulation.Bits.freq = DDR_3200;
  }

  return CurrentDimmFrequencyWithDefaultPopulation.Data;
} // GetCurrentDimmFrequencyOdt
