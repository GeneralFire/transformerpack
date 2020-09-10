/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include "ssabios.h"
#include <Library/MemoryEvContentLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemMcIpLib.h>
#include "RmtCore.h"
#include <Library/SiliconWorkaroundLib.h>

/**
  @brief
  This function get the RMT test config ERID setting.

  @param[in]  pTestCfg   Pointer to RMT test configuration structure.

  @retval TRUE/FALSE - ERID enable or disable
**/
BOOLEAN
GetRmtTestCfgErid (
  IN RMT_CONFIG *pTestCfg
  )
{
  return FALSE;
}

/**

  @brief
  This function initializes the MarginGroups array with the project specific groups.

  @param[in]       IoLevel        I/O level.
  @param[in, out]  MarginGroups   An array structure with the margin groups.

  @retval N/A
**/
VOID
InitMarginGroups (
  IN     MRC_LT IoLevel,
  IN OUT MRC_GT (*MarginGroups)[MARGIN_GROUP_ARRAY_CNT][MARGIN_GROUP_ARRAY_ELE_CNT]
  )
{
  SYS_SETUP       *Setup;
  UINT32          MarginGroupsSize;
  MRC_GT          Group;

  if (IsSiliconWorkaroundEnabled ("S22010823763")) {
    Group = RxVref;
  } else {
    Group = RxSamplerEvenOdd;
  }
  //
  // Arrays of margin parameter groups
  // NOTE: Any modification to the values in these arrays needs to be reflected
  // in the LogRankmarginResults function's code.
  // MrcGtMax is used to indicate that it is an invalid/skip margin group
  //

  //
  // Margin groups for the frontside (DdrLevel)
  //
  MRC_GT MarginGroupsDdrLevelLocal[MARGIN_GROUP_ARRAY_CNT][MARGIN_GROUP_ARRAY_ELE_CNT] = \
  {
    // DQ signal groups
    {
      RxDqsDelay,
      TxDqDelay,
      RxSamplerEvenOdd,
      TxVref
    },
    // Cmd/Ctl signal groups
    {
      DcaDelay,
      RcdDcaVref,
      DcsDelay,
      SKIP_TEST   // Placeholder for DcsVref
    },
#ifdef DDR5_SUPPORT
    // Placeholder to match backside table
    {
      SKIP_TEST,  // Placeholder
      SKIP_TEST,  // Placeholder
      SKIP_TEST,  // Placeholder
      SKIP_TEST   // Placeholder
    },
#endif
#if SUPPORT_FOR_DDRT
    // DDRT signal groups
    {
      SKIP_TEST,  // Placeholder for Req timing
      SKIP_TEST,  // Placeholder for Req Vref
      SKIP_TEST,  // Placeholder
      SKIP_TEST   // Placeholder
    },
#endif
  };

    //
    // Margin groups for the backside (LrbufLevel)
    //
    MRC_GT MarginGroupsLrbufLevelLocal[MARGIN_GROUP_ARRAY_CNT][MARGIN_GROUP_ARRAY_ELE_CNT] = \
  {
    // backside DQ signal groups
    {
      RxDqsDelay,
      TxDqDelay,
      RxVref,
      TxVref
    },
    // backside Cmd/Ctl signal groups
    {
      QcaDelay,
      DramCaVref,
      QcsDelay,
      DramCsVref
    },
#ifdef DDR5_SUPPORT
    // BCOM signal groups
    {
      BcomDelay,
      BcomVref,
      SKIP_TEST,  // Placeholder
      SKIP_TEST   // Placeholder
    },
#endif
#if SUPPORT_FOR_DDRT
    // DDRT signal groups
    {
      SKIP_TEST,  // Placeholder
      SKIP_TEST,  // Placeholder
      SKIP_TEST,  // Placeholder
      SKIP_TEST   // Placeholder
    },
#endif
  };

  Setup  = GetSysSetupPointer ();

  MarginGroupsSize = MARGIN_GROUP_ARRAY_CNT * MARGIN_GROUP_ARRAY_ELE_CNT * sizeof (MRC_GT);

  if (IoLevel == DdrLevel) {
    CopyMem (MarginGroups, &MarginGroupsDdrLevelLocal, MarginGroupsSize);

    //
    // Update RxSampler parameter from BIOS setup knobs
    //
    switch (Setup->mem.dfxMemSetup.DfxDdr5SsaRxSamplerSelection) {
    case DDR5_SSA_RXSAMPLER_EVEN_ONLY:
      (*MarginGroups)[0][2] = RxSamplerEven;
      break;
    case DDR5_SSA_RXSAMPLER_ODD_ONLY:
      (*MarginGroups)[0][2] = RxSamplerOdd;
      break;
    case DDR5_SSA_RXVREF:
      (*MarginGroups)[0][2] = RxVref;
      break;
    default:
      (*MarginGroups)[0][2] = Group;
      break;
    }

    //
    // Use DCA DFE Vref monitor to margin the Cmd Vref when DCA DFE training is enabled
    //
    if (IsMemFlowEnabled (RcdDcaDfeDdr5) && GetTrainingStepEnableChip (GetCurrentSocketId (), CHECKPOINT_MINOR_DCA_DFE)) {
      (*MarginGroups)[1][1] = DcaDfeVref;
    }

    //
    // Use data buffer DFE Vref monitor to margin the Tx Vref when DB DFE training is enabled
    //
    if (IsMemFlowEnabled (DbDfeDdr5Training) && (IsNonLrdimmPresentOnSocket (GetCurrentSocketId ()) == FALSE)) {
      (*MarginGroups)[0][3] = DbDfeVrefGroup;
    }

  } else {
     CopyMem (MarginGroups, &MarginGroupsLrbufLevelLocal, MarginGroupsSize);
  }

  return;
}

/**
  @brief
  This function returns the number of SubChannel and the width of a SubChannel.
  It is used to help display test results. BSSA test treats DDR5 channel as 80bits bus internally.
  The 80bits bus is broken into SubChannel when displaying test results.

          NumSubCh  SubChMaxBit
  DDR4      1          64/72
  DDR5      2          40

  @param[in    ] BusWidth    - DDR Bus width
  @param[   out] NumSubCh    - Number of SubChannel
  @param[   out] SubChMaxBit - Number of bits in a SubChannel

**/
VOID
GetSubChInfo (
  IN     UINT8 BusWidth,
     OUT UINT8 *NumSubCh,
     OUT UINT8 *SubChMaxBit
  )
{
  *NumSubCh = 2;
  *SubChMaxBit = BusWidth / 2;
}