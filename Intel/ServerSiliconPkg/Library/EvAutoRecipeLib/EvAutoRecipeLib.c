/** @file

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

#include <Uefi.h>
#include <Chip/Include/EvAutoRecipe.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/EvAutoRecipeLib.h>

#pragma pack (1)
typedef struct CPU_RECIPE_TRANSLATION {
  UINT8   CpuType;
  UINT16  Revision;
  UINT8   EvRevisionBit;
} CPU_RECIPE_TRANSLATION;

//
// CPU_RECIPE_TRANSLATION Table is used to idenfity the recipe
// to be applied based on CPU type and revision.
//

CPU_RECIPE_TRANSLATION
CpuEvRecipeTranslation[] = {
  //       CpuType,   Revision,  EvRevisionBit
  {        CPU_SKX,   REV_A0 ,   EV_REV_BIT_00},
  {        CPU_SKX,   REV_A2,    EV_REV_BIT_02},
  {        CPU_SKX,   REV_B0,    EV_REV_BIT_03},
  {        CPU_SKX,   REV_L0,    EV_REV_BIT_04},
  {        CPU_SKX,   REV_B1,    EV_REV_BIT_05},
  {        CPU_SKX,   REV_H0,    EV_REV_BIT_06},
  {        CPU_SKX,   REV_M0,    EV_REV_BIT_07},
  {        CPU_SKX,   REV_U0,    EV_REV_BIT_08},
  {        CPU_CLX,   REV_A0,    EV_REV_BIT_00},
  {        CPU_CLX,   REV_B0,    EV_REV_BIT_03},
  {        CPU_CLX,   REV_N0,    EV_REV_BIT_11},
  {        CPU_CLX,   REV_V0,    EV_REV_BIT_12},
  {        CPU_CLX,   REV_B1,    EV_REV_BIT_05},
  {        CPU_CLX,   REV_N1,    EV_REV_BIT_07},
  {        CPU_CLX,   REV_V1,    EV_REV_BIT_07},
  {        CPU_CPX,   REV_A0,    EV_REV_BIT_00},
  {        CPU_CPX,   REV_A1,    EV_REV_BIT_00},
  {        CPU_SNR,   REV_A0,    EV_REV_BIT_00},
  {        CPU_SNR,   REV_A1,    EV_REV_BIT_00},
  {        CPU_SNR,   REV_A2,    EV_REV_BIT_02},
  {        CPU_SNR,   REV_A3,    EV_REV_BIT_02},
  {        CPU_SNR,   REV_B0,    EV_REV_BIT_03},
  {        CPU_SNR,   REV_B1,    EV_REV_BIT_05},
  {        CPU_TNR,   REV_A0,    EV_REV_BIT_00},
  {      CPU_ICXSP,   REV_R0,    EV_REV_BIT_00},
  {      CPU_ICXSP,   REV_L0,    EV_REV_BIT_04},
  {      CPU_ICXSP,   REV_C0,    EV_REV_BIT_05},
  {      CPU_ICXSP,   REV_D0,    EV_REV_BIT_06},
  {      CPU_ICXSP,   REV_M0,    EV_REV_BIT_07},
  {       CPU_ICXD,   REV_A0,    EV_REV_BIT_00},
  {       CPU_ICXD,   REV_B0,    EV_REV_BIT_03},
  {      CPU_SPRSP,   REV_A0,    EV_REV_BIT_00},
  {      CPU_SPRSP,   REV_B0,    EV_REV_BIT_03},
  {      CPU_SPRSP,   REV_C0,    EV_REV_BIT_05},
  {      CPU_SPRSP,   REV_U0,    EV_REV_BIT_06},
  {      CPU_GNRSP,   REV_A0,    EV_REV_BIT_00},
  {      CPU_SRFSP,   REV_A0,    EV_REV_BIT_00}
};
#pragma pack ()

/**
  Get the EV recipe revision bit

  @retval EV recipe revision bit

**/
UINT8
EvAutoRecipeGetRevBit (
  VOID
  )
{
  UINT8 Index, TableSize, RevisionBit, CpuType;

  //
  // Detect current CPU
  //
  if (IsCpuAndRevision (CPU_SKX, REV_ALL)) {
    CpuType = CPU_SKX;
  } else if (IsCpuAndRevision (CPU_CLX, REV_ALL)) {
    CpuType = CPU_CLX;
  } else if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    CpuType = CPU_CPX;
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    CpuType = CPU_ICXSP;
  } else if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    CpuType = CPU_SPRSP;
  } else if (IsCpuAndRevision (CPU_GNRSP, REV_ALL)) {
    CpuType = CPU_GNRSP;
  } else if (IsCpuAndRevision (CPU_SRFSP, REV_ALL)) {
    CpuType = CPU_SRFSP;
  } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    CpuType = CPU_ICXD;
  } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    CpuType = CPU_SNR;
  } else if (IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    CpuType = CPU_TNR;
  } else {
    return EV_RECIPE_INVALID_VALUE;
  }

  TableSize   = sizeof (CpuEvRecipeTranslation)/sizeof(CPU_RECIPE_TRANSLATION);
  RevisionBit = EV_RECIPE_INVALID_VALUE;

  for (Index = 0; Index < TableSize; Index++) {
    if (CpuEvRecipeTranslation[Index].CpuType == CpuType) {
      if (IsCpuAndRevision (CpuType, CpuEvRecipeTranslation[Index].Revision)) {
        RevisionBit      = CpuEvRecipeTranslation[Index].EvRevisionBit;
        break;
      }
    }
  }
  return RevisionBit;
}

/**

  Check if the CPU SKU in EV recipe row is valid.

  @param SkuTypeMask - Supported processor SKU bitmask from EV recipe row

  @retval TRUE/FALSE

**/
BOOLEAN
EvAutoRecipeiIsValidSkuType (
  IN UINT16 SkuTypeMask
  )
{
  //
  //  This function will always return TRUE for now, until (if at all) the SKU
  //  concept comes back, in which case the code will be updated.
  //
  return TRUE;
}

/**
  Check if the CPU revision in EV recipe row is valid.

  @param[in] EvRevisionBit    - Revision bit used in the recipe
  @param[in] EvRevMask        - Supported processor stepping bitmask from EV recipe row

  @retval TRUE/FALSE

**/
BOOLEAN
EvAutoRecipeIsValidRevision (
  IN UINT8   EvRevisionBit,
  IN UINT64  EvRevMask
  )
{
  BOOLEAN Result = FALSE;

  if ((EvRevMask & (UINT64)(1 << EvRevisionBit)) != 0) {
    Result = TRUE;
  }

  return Result;
}

/**
  Check if the Socket ID in EV recipe row is valid.

  @param[in] SocId      - Socket ID being programmed
  @param[in] SocketMask - Socket ID bitmask from EV recipe row

  @retval TRUE/FALSE

**/
BOOLEAN
EvAutoRecipeIsValidSocket (
  IN UINT8  SocId,
  IN UINT32 SocketMask
  )
{
  if (SocketMask == WILDCARD_32) {
    //
    // When mask is 0xFFFF_FFFF, return TRUE irrespective of current CPU
    //
    return TRUE;
  }

  if ((SocketMask & (1 << SocId)) != 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}