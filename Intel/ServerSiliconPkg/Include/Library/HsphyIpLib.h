/** @file

  This is the library class header file for Hsphy IP lib.

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

#ifndef __HSPHY_IP_LIB_H__
#define __HSPHY_IP_LIB_H__

#include <Uefi.h>
#include <Base.h>
#include <Upi/KtiSi.h>
#include <Library/UsraAccessApi.h>
#include <Library/CsrSbPortIdLib.h>
#include <Guid/UboxIpInterface.h>

typedef enum {
  RECIPE_UPI_TYPE   = 0,
  RECIPE_PCIE_TYPE
} HSPHY_RECIPE_TYPE;

#define NUMBER_OF_LANES_PER_HSPHY_REG    8     // each Hsphy_Reg contain 8 Lanes.
#define MAX_LANES_PER_PCIE_PORT          16    // each PCIE port supports 16 lanes maximum

#define MAX_HSPHY_REG_INDEX_PER_UPI      (MAX_LANES_PER_LINK_SPR / NUMBER_OF_LANES_PER_HSPHY_REG)     // UPI supports 24 lanes per link
#define MAX_HSPHY_REG_INDEX_PER_PCIE     (MAX_LANES_PER_PCIE_PORT / NUMBER_OF_LANES_PER_HSPHY_REG)    // PCIE supports 16 lanes per port

/**
  This function will do the hsphy recipe programming for UPI & PCIE.

  @param[in]     RecipeType            UPI recipe or PCIE recipe
  @param[in]     Socket                Ssocket number.
  @param[in]     Port                  UPI link or PCIE Port (IIO Stack)
  @param[in]     X8LaneIndex           The Index of x8 lanes (SPR: UPI has 3 set of x8 lanes; PCIE has 2 sets of x8 lanes)
  @param[in]     RegOffset             Register Offset.
  @param[in]     BitMask               The value to AND with the read value from register.
  @param[in]     OrData                The value to OR with the result of the AND operation.

  @retval        EFI_SUCCESS           The programming is successful.
  @retval        EFI_INVALID_PARAMETER One of the input parameter is invalid or out of range.
  @retval        EFI_NOT_FOUND         Cant't find the access way.

**/
EFI_STATUS
EFIAPI
HsphyRecipeProgramming (
  IN       UINT8              RecipeType,
  IN       UINT8              Socket,
  IN       UINT8              Port,
  IN       UINT8              X8LaneIndex,
  IN       UINT32             RegOffset,
  IN       UINT32             BitMask,
  IN       UINT32             OrData
);

/**
  This function will return the final upi hsphy speed.

  @param[in]     Socket                Ssocket number.
  @param[in]     Port                  UPI link or PCIE Port (IIO Stack)

  @retval        The final upi hsphy speed of the port.

**/
UINT8
EFIAPI
GetUpiHsphyPipeRate (
  IN       UINT8              Socket,
  IN       UINT8              Port
);

#endif // __HSPHY_IP_LIB_H__
