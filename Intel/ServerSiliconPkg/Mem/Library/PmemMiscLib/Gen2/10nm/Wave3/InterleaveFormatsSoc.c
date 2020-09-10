/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Library/BaseMemoryLib.h>
#include <Library/PmemMiscLib.h>
#include <Library/KtiApi.h>
#include <MemDecodeCommonIncludes.h>
#include <Memory/MemDefaults.h>
#include <Common/PmemMisc.h>

/**
   SPR LCC/XCC 8 Channel (E/W)
   AD or AD-WB + NoSNC|SNC0
**/
PM_INTERLEAVE mSprXccLccCh8IntlvSetList[] = {
//  InterleaveBitmap              Recommended             Reserved
  { ISET_CH8_WY8,                 ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY4_XIMC_CH0,        ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY4_XIMC_CH1,        ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY4_IMC0_IMC2,       ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY4_IMC1_IMC3,       ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY4_IMC0_IMC3,       ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY4_IMC1_IMC2,       ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY4_IMC0_IMC1,       ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY4_IMC2_IMC3,       ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC0_IMC2_CH0,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC0_IMC2_CH1,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC1_IMC3_CH0,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC1_IMC3_CH1,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC0_IMC3_CH0,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC0_IMC3_CH1,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC1_IMC2_CH0,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC1_IMC2_CH1,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC0_IMC1_CH0,   ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC0_IMC1_CH1,   ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC2_IMC3_CH0,   ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC2_IMC3_CH1,   ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC0,            ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC1,            ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC2,            ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC3,            ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY1_IMC0_CH0,        ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY1_IMC0_CH1,        ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY1_IMC1_CH0,        ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY1_IMC1_CH1,        ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY1_IMC2_CH0,        ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY1_IMC2_CH1,        ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY1_IMC3_CH0,        ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY1_IMC3_CH1,        ISET_RECOMMENDED,       0 }
};

/**
   SPR LCC/XCC 8 Channel (E/W)
   AD-WB SNC2
**/
PM_INTERLEAVE mSprXccLccCh8Snc2IntlvSetList[] = {
//  InterleaveBitmap              Recommended             Reserved
  { ISET_CH8_WY4_IMC0_IMC1,       ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY4_IMC2_IMC3,       ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC0_IMC1_CH0,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC0_IMC1_CH1,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC2_IMC3_CH0,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC2_IMC3_CH1,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC0,            ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC1,            ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC2,            ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC3,            ISET_NOT_RECOMMENDED,   0 }
};

/**
   SPR XCC/LCC/HCC 8 Channel
   AD-WB SNC4
**/
PM_INTERLEAVE mSprXccLccHccCh8Snc4IntlvSetList[] = {
//  InterleaveBitmap              Recommended             Reserved
  { ISET_CH8_WY2_IMC0,            ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC1,            ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC2,            ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC3,            ISET_RECOMMENDED,       0 },
};

/**
   SPR HCC 8 Channel (N/S)
   AD or AD-WB + NoSNC|SNC0
**/
PM_INTERLEAVE mSprHccCh8IntlvSetList[] = {
//  InterleaveBitmap              Recommended             Reserved
  { ISET_CH8_WY8,                 ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY4_XIMC_CH0,        ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY4_XIMC_CH1,        ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY4_IMC0_IMC2,       ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY4_IMC1_IMC3,       ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY4_IMC0_IMC3,       ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY4_IMC1_IMC2,       ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY4_IMC0_IMC1,       ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY4_IMC2_IMC3,       ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC0_IMC2_CH0,   ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC0_IMC2_CH1,   ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC1_IMC3_CH0,   ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC1_IMC3_CH1,   ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC0_IMC3_CH0,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC0_IMC3_CH1,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC1_IMC2_CH0,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC1_IMC2_CH1,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC0_IMC1_CH0,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC0_IMC1_CH1,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC2_IMC3_CH0,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC2_IMC3_CH1,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC0,            ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC1,            ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC2,            ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC3,            ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY1_IMC0_CH0,        ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY1_IMC0_CH1,        ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY1_IMC1_CH0,        ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY1_IMC1_CH1,        ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY1_IMC2_CH0,        ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY1_IMC2_CH1,        ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY1_IMC3_CH0,        ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY1_IMC3_CH1,        ISET_RECOMMENDED,       0 }
};

/**
   SPR HCC 8 Channel (N/S)
   AD-WB + SNC2
**/
PM_INTERLEAVE mSprHccCh8Snc2IntlvSetList[] = {
//  InterleaveBitmap              Recommended             Reserved
  { ISET_CH8_WY4_IMC0_IMC2,       ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY4_IMC1_IMC3,       ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC0_IMC2_CH0,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC0_IMC2_CH1,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC1_IMC3_CH0,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC1_IMC3_CH1,   ISET_RECOMMENDED,       0 },
  { ISET_CH8_WY2_IMC0,            ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC1,            ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC2,            ISET_NOT_RECOMMENDED,   0 },
  { ISET_CH8_WY2_IMC3,            ISET_NOT_RECOMMENDED,   0 },
};

/**

  Fetch the number of supported interleave formats for the AppDirect write back platform configuration.

  @param[in]  SncEnabled           0 - SNC disabled for this configuration, 1 - SNC enabled for this configuration
  @param[in]  NumOfCluster         SNC level 2 / 4

  @retval    The number of supported interleave formats.

**/
UINT16
EFIAPI
GetAppDirectWbNumIntFormats (
  IN  UINT8              SncEnabled,
  IN  UINT8              NumOfCluster
  )
{
  UINT16 NumIntFormats;

  NumIntFormats = 0;

  if (GetChopType (0) == TypeHcc) {
    if (SncEnabled) {
      switch (NumOfCluster) {
      case 2:
        NumIntFormats = ARRAY_SIZE (mSprHccCh8Snc2IntlvSetList);
        break;
      case 4:
        NumIntFormats = ARRAY_SIZE (mSprXccLccHccCh8Snc4IntlvSetList);
        break;
      default:
        break;
      }
    } else {
        NumIntFormats = ARRAY_SIZE (mSprHccCh8IntlvSetList);
    }
  } else {
    if (SncEnabled) {
      switch (NumOfCluster) {
      case 2:
        NumIntFormats = ARRAY_SIZE (mSprXccLccCh8Snc2IntlvSetList);
        break;
      case 4:
        NumIntFormats = ARRAY_SIZE (mSprXccLccHccCh8Snc4IntlvSetList);
        break;
      default:
        break;
      }
    } else {
      NumIntFormats = ARRAY_SIZE (mSprXccLccCh8IntlvSetList);
    }
  }
  return NumIntFormats;
}

/**

  Fetch a copy of the supported interleave format list for the AppDirect write back platform configuration.

  @param[in]  SncEnabled           0 - SNC disabled for this configuration, 1 - SNC enabled for this configuration
  @param[in]  NumOfCluster         SNC level 2 / 4
  @param[out] IntFormatListBuffer  A pointer to the data buffer where the interleave format list will be copied to.

**/
VOID
EFIAPI
GetAppDirectWbIntFormatList (
  IN     UINT8              SncEnabled,
  IN     UINT8              NumOfCluster,
     OUT PM_INTERLEAVE      *IntFormatListBuffer
  )
{
  if (IntFormatListBuffer == NULL) {
    return;
  }

  if (GetChopType (0) == TypeHcc) {
    if (SncEnabled) {
      switch (NumOfCluster) {
      case 2:
        CopyMem (IntFormatListBuffer, mSprHccCh8Snc2IntlvSetList, sizeof(mSprHccCh8Snc2IntlvSetList));
        break;
      case 4:
        CopyMem (IntFormatListBuffer, mSprXccLccHccCh8Snc4IntlvSetList, sizeof(mSprXccLccHccCh8Snc4IntlvSetList));
        break;
      default:
        break;
      }
    } else {
      CopyMem (IntFormatListBuffer, mSprHccCh8IntlvSetList, sizeof(mSprHccCh8IntlvSetList));
    }
  } else {
    if (SncEnabled) {
      switch (NumOfCluster) {
      case 2:
        CopyMem (IntFormatListBuffer, mSprXccLccCh8Snc2IntlvSetList, sizeof(mSprXccLccCh8Snc2IntlvSetList));
        break;
      case 4:
        CopyMem (IntFormatListBuffer, mSprXccLccHccCh8Snc4IntlvSetList, sizeof(mSprXccLccHccCh8Snc4IntlvSetList));
        break;
      default:
        break;
      }
    } else {
      CopyMem (IntFormatListBuffer, mSprXccLccCh8IntlvSetList, sizeof(mSprXccLccCh8IntlvSetList));
    }
  }
}

/**

  Fetch the number of supported interleave formats for the AppDirect platform configuration.

  @retval    The number of supported interleave formats.

**/
UINT16
EFIAPI
GetAppDirectNumIntFormats (
  VOID
  )
{
  if (GetChopType (0) == TypeHcc) {
    return ARRAY_SIZE (mSprHccCh8IntlvSetList);
  } else {
    return ARRAY_SIZE (mSprXccLccCh8IntlvSetList);
  }
}

/**

  Fetch a copy of the supported interleave format list for the AppDirect platform configuration.

  @param[out] IntFormatListBuffer  A pointer to the data buffer where the interleave format list will be copied to.

**/
VOID
EFIAPI
GetAppDirectIntFormatList (
  OUT PM_INTERLEAVE      *IntFormatListBuffer
  )
{
  if (IntFormatListBuffer == NULL) {
    return;
  }

  if (GetChopType (0) == TypeHcc) {
    CopyMem (IntFormatListBuffer, mSprHccCh8IntlvSetList, sizeof(mSprHccCh8IntlvSetList));
  } else {
    CopyMem (IntFormatListBuffer, mSprXccLccCh8IntlvSetList, sizeof(mSprXccLccCh8IntlvSetList));
  }
}

/**

  Fetch the supported Max Interleave Sets for the current AppDirect write back platform configuration.

  @param[in] VolMemMode  Current volatile memory mode (1LM / 2LM / 1LM + 2LM)

  @retval The encoded Max Interleave Sets for the current AppDirect write back platform config.

**/
MAX_INT_SET
EFIAPI
GetAppDirectWbMaxInterleaveSet (
  IN UINT8    VolMemMode
  )
{
  MAX_INT_SET MaxInterleaveSet;
  MaxInterleaveSet.Data = 0;

  switch (VolMemMode) {
  //
  // 1LM + AD-WBx1: Supported
  //
  case VOL_MEM_MODE_1LM:
    MaxInterleaveSet.Bits.PerCpuDie = PLAT_CAP_MAX_INTERLEAVE_PER_CPU_DIE;
    MaxInterleaveSet.Bits.PerPmem   = PLAT_CAP_MAX_INTERLEAVE_PER_PMEM;
    break;

  //
  // MM + AD-WBx1: Not Supported
  // 1LM + 2LM + AD-WBx1: Not Supported
  //
  case VOL_MEM_MODE_2LM:
  case VOL_MEM_MODE_MIX_1LM2LM:
    MaxInterleaveSet.Bits.PerCpuDie = PLAT_CAP_MAX_INTERLEAVE_PER_PMEM;
    MaxInterleaveSet.Bits.PerPmem   = PLAT_CAP_MAX_INTERLEAVE_PER_PMEM;
    break;

  default:
    break;
  }

  return MaxInterleaveSet;
}
