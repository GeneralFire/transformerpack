/** @file
  CSR Pseudo Offset Convert Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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
#include <PiPei.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <MaxSocket.h>
#include <MaxCore.h>
#include <Library/CsrPseudoOffsetConvertLib.h>
#include <CsrPseudoOffsetDefine.h>
/**
  Identifies the real hardware register for given BoxType, functional Block and pseudo table offset.

  @param[in] RegOffset: Bits <31:24>  Bits <23:20>  Bit 19  Bits <18:16>  Bits <15:0>
                         Box Number   Func Number   Pseudo  Size          pseudo table Offset

  @retval Bits <31:24>  Bits <23:20>  Bit 19  Bits <18:16>  Bits <15:0>
  @retval Box Number    Func Number   0       Size          PCICfg Offset

**/

UINT32
EFIAPI
GetPseudoRegisterOffset (
  IN  CSR_OFFSET  RegOffset
  )
{
#if ((TOTAL_PSEUDO_OFFSET_TABLES == 0) || (defined ICXDE_HOST && defined L0_HOST))
  ASSERT (FALSE);
  return RegOffset.Data;
#else
  UINT32                                 PseudoOffset;
  UINT8                                  BoxType;
  UINT8                                  FuncBlk;
  PSEUDO_OFFSET_TABLE_HEADER             *TableHeader;
  EFI_HOB_GUID_TYPE                      *GuidHob;
  UINT32                                 Reg;

  GuidHob = GetFirstGuidHob (&gCsrPseudoOffsetTableGuid);
  if (GuidHob == NULL) {
    ASSERT (GuidHob != NULL);
    return 0;
  }
  TableHeader = (PSEUDO_OFFSET_TABLE_HEADER*) GET_GUID_HOB_DATA (GuidHob);

  PseudoOffset = RegOffset.Bits.offset;
  BoxType      = (UINT8)RegOffset.Bits.boxtype;
  FuncBlk      = (UINT8)RegOffset.Bits.funcblk;
  Reg          = *((UINT32*)((UINTN)TableHeader + (UINTN)TableHeader->SubTableLocation[BoxType][FuncBlk] + (UINTN)PseudoOffset * sizeof(UINT32)));

  return Reg;
#endif // #if (TOTAL_PSEUDO_OFFSET_TABLES == 0)
}
