/** @file

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

#ifndef _SPD_DECODE_COMMON_H_
#define _SPD_DECODE_COMMON_H_

#include <MemCommon.h>
#include <SysHost.h>
#include <Library/BaseMemoryLib.h>
//
// Using when there is no implemention in DDR4 or DDR5.
//
#define NO_SPD_BYTE_NUMBER  0xFFFF
#define NO_SPD_START_BIT    0xF
#define NO_SPD_END_BIT      0xF
#define NO_SPD_FIELD_INDEX  0xFFFF

//
// The defination of the items in the SpdFieldMappingTableDdr4 and SpdFieldMappingTableDdr5
//
#pragma pack (push,1)

typedef struct {
  UINT16 StartByte;
  UINT8  StartByteStartBit;  // StartByteStartBit is in Byte level, should be in range 0-7
  UINT16 EndByte;
  UINT8  EndByteEndBit;      // EndByteEndBit is in Byte level, should be in range 0-7
  UINT16 FieldIndex;
} SPD_FIELD_MAPPING;

#pragma pack (pop)

/**
  GetSpdFieldMappingTable.

  @param[in]  FieldIndex      - SPD Field Index to be read
  @param[out] SpdFieldMapping - Pointer to SpdFieldMapping

  @retval EFI_SUCCESS           The read completed successfully
  @retval EFI_INVALID_PARAMETER  Input parameter out of bounds
**/
EFI_STATUS
EFIAPI
GetSpdFieldMappingTable (
  IN     UINT16            FieldIndex,
  IN OUT SPD_FIELD_MAPPING *SpdFieldMapping
  );

#endif  // #ifndef _SPD_DECODE_COMMON_H_
