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
#ifndef _EVAUTORECIPE_LIB_H_
#define _EVAUTORECIPE_LIB_H_

#define EV_RECIPE_INVALID_VALUE     0xFF

//EV recipe revision            EvRevisionBit
#define EV_REV_BIT_00         0
#define EV_REV_BIT_01         1
#define EV_REV_BIT_02         2
#define EV_REV_BIT_03         3
#define EV_REV_BIT_04         4
#define EV_REV_BIT_05         5
#define EV_REV_BIT_06         6
#define EV_REV_BIT_07         7
#define EV_REV_BIT_08         8
#define EV_REV_BIT_09         9
#define EV_REV_BIT_10        10
#define EV_REV_BIT_11        11
#define EV_REV_BIT_12        12
#define EV_REV_BIT_13        13
#define EV_REV_BIT_14        14
#define EV_REV_BIT_15        15
#define EV_REV_BIT_16        16
#define EV_REV_BIT_17        17
#define EV_REV_BIT_18        18
#define EV_REV_BIT_19        19
#define EV_REV_BIT_20        20

/**
  Get the EV recipe revision bit

  @retval EV recipe revision bit

**/
UINT8
EvAutoRecipeGetRevBit (
  VOID
  );

/**

  Check if the CPU SKU in EV recipe row is valid.

  @param SkuTypeMask - Supported processor SKU bitmask from EV recipe row

  @retval TRUE/FALSE

**/
BOOLEAN
EvAutoRecipeiIsValidSkuType (
  IN UINT16 SkuTypeMask
  );

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
  );

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
  );

#endif // _EVAUTORECIPE_LIB_H_
