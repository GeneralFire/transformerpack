/** @file
  MemXoverCalibOffsets.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include "Include/MemXoverCalibOffsets.h"

XOVER_OFFSETS_CHANNEL_SET mXoverOffsets[] = {{XOVER_OFFSETS_CH0},
                                             {XOVER_OFFSETS_CH1},
                                             {XOVER_OFFSETS_CH2},
                                             {XOVER_OFFSETS_CH3},
                                             {XOVER_OFFSETS_CH4},
                                             {XOVER_OFFSETS_CH5},
                                             {XOVER_OFFSETS_CH6},
                                             {XOVER_OFFSETS_CH7}
                                            };

/**
  Return Xover offsets for the current DDR frequency and channel

  @param[in]   Socket        - Socket number
  @param[in]   Ch            - Channel number
  @param[out]  XoverOffsets  - Pointer to caller's offset structure pointer

  @retval  EFI_SUCCESS if offsets found
           EFI_NOT_FOUND or EFI_INVALID_PARAMETER otherwise
**/
EFI_STATUS
GetXoverOffsets (
  IN    UINT8           Socket,
  IN    UINT8           Ch,
  OUT   XOVER_OFFSETS   **XoverOffsets
  )
{
  UINT8                   DdrFreq;
  UINT8                   FreqIndex;
  UINT8                   NumFreq;
  UINT8                   MaxChSupported;
  XOVER_OFFSET_FREQ_SET   *OffsetFreqSet;

  MaxChSupported = sizeof (mXoverOffsets) / sizeof (mXoverOffsets[0]);

  if ((XoverOffsets == NULL) || (Ch >= MaxChSupported)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Obtain current DDR frequency
  //
  GetEncodedDdrFreq (Socket, &DdrFreq);

  //
  // Point to frequency table for the channel
  //
  OffsetFreqSet = &(mXoverOffsets[Ch].OffsetFreqSet[0]);
  NumFreq = sizeof (mXoverOffsets[Ch]) / sizeof (mXoverOffsets[Ch].OffsetFreqSet[0]);

  //
  // Return pointer to Xover Offsets for the frequency
  //
  // Find the "best" frequency match if an exact match is not available.
  // "best" frequncy => The nearest frequency in the offset table that is greater than the current DDR frequency
  //                 => Use the last table entry if current DDR frequency is greather than the last table entry's frequency.
  //
  // This algorithm assumes the table is sorted in ascending frequency order.
  //
  for (FreqIndex = 0; FreqIndex < NumFreq; FreqIndex++) {
    *XoverOffsets = &(OffsetFreqSet[FreqIndex].Offset);
    if (OffsetFreqSet[FreqIndex].DdrFreq >= DdrFreq) {
      break;
    }
  }
  return EFI_SUCCESS;
}

