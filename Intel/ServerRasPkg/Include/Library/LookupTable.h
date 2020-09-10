/** @file
  lookup table for IEHs.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#ifndef _LOOKUP_TABLE_H_
#define _LOOKUP_TABLE_H_

#define MAX_BITMAP          256
#define CDF_IEH_STACK       0x10
#define EBG_IEH_STACK       0x10

#include <IehTypes.h>

typedef struct{
  IEH_DEVICE_TYPE IehDevType;
  UINT8           Stack;
  INT8            BusOffset;
  UINT8           Device;
  UINT8           Function;
  UINT32          SbPortId;
  UINT8           BitIndex;
  UINT64          DevSpecInfo;
} LOOKUP_TBL_ENTRY;

/**
  get lookup table per CPU type
  wave1/2 need lookup table.

  @param   none

  @retval  Lookup table pointer.

**/
LOOKUP_TBL_ENTRY  *
EFIAPI
GetLookupTable (
  VOID
  );
#endif
