/** @file

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

#ifndef _ON_BOARD_SPD_LIB_PRIVATE_H_
#define _ON_BOARD_SPD_LIB_PRIVATE_H_

//
// Statements that include other files
//
#include <Library/SpdAccessLibOnBoard.h>
#include <IndustryStandard/Acpi.h>

//
// Definitions
//
#define EFI_SPD_TABLE_SIGNATURE SIGNATURE_32('S','P','D','_')
#define NUM_DIMMS_IN_FFS        16

//
// Ensure proper structure formats
//
#pragma pack(1)
//
// EFI BIOS structure for SPD data
//
typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER  Header;
  ON_BOARD_SPD_TABLE_STRUCTURE Info[NUM_DIMMS_IN_FFS];
} ON_BOARD_SPD_INJECTION_STRUCTURE;
#pragma pack()

#endif // _ON_BOARD_SPD_LIB_PRIVATE_H_
