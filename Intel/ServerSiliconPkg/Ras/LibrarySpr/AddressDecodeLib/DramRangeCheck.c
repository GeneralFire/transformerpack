/** @file
  Implementation of CHA mmio decode.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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
#include <Base.h>
#include <Library/MemMapDataLib.h>
#include "AddressDecodeInternal.h"


extern DRAM_SCOPE                  *mDramScope;
extern AD_SOCKET_INFO              *mSadTadScope;
extern struct SystemMemoryMapHob  *SystemMemoryMap;

/**
  This routine is to initialize existing system DRAM resource.

  capture all DRAM resource. Assume all valid item are continuous. And some range are overrapped. We must put InclusiveFlag = 0 at latter part.

  @param[in] DramScopePtr    A pointer to DRAM resource array.

  @retval none

**/
VOID
EFIAPI
InitDramScope (
  IN   DRAM_SCOPE   *DramScopePtr
  )
{
  //
  // DramLo
  //
  DramScopePtr[0].ValidFlag = 1;
  DramScopePtr[0].InclusiveFlag = 1;
  DramScopePtr[0].DramBase =  LShiftU64 (SystemMemoryMap->lowMemBase, SAD_UNIT);
  DramScopePtr[0].DramLimit = LShiftU64 (SystemMemoryMap->lowMemBase + SystemMemoryMap->lowMemSize, SAD_UNIT) -1;
  DEBUG ((EFI_D_INFO, " [AddressDecode Init] DRAM range 0x%x  ~  0x%lx\n", DramScopePtr[0].DramBase,DramScopePtr[0].DramLimit));

  //
  // DramHi
  //
  DramScopePtr[1].ValidFlag = 1;
  DramScopePtr[1].InclusiveFlag = 1;
  DramScopePtr[1].DramBase = LShiftU64 (SystemMemoryMap->highMemBase, SAD_UNIT);
  DramScopePtr[1].DramLimit = LShiftU64 (SystemMemoryMap->highMemBase + SystemMemoryMap->highMemSize, SAD_UNIT) -1;//LShiftU64 (UdsHobPtr->SystemStatus.tohmLimit + 1, SAD_UNIT) - 1;
  DEBUG ((EFI_D_INFO, " [AddressDecode Init] DRAM range 0x%lx  ~  0x%lx\n", DramScopePtr[1].DramBase,DramScopePtr[1].DramLimit));

  //
  //VGA range
  //
  DramScopePtr[2].ValidFlag = 1;
  DramScopePtr[2].InclusiveFlag = 0;
  DramScopePtr[2].DramBase = 0xA0000; //A, B seg
  DramScopePtr[2].DramLimit = 0xC0000 -1;
  DEBUG ((EFI_D_INFO, " [AddressDecode Init] DRAM range 0x%x  ~  0x%lx\n", DramScopePtr[2].DramBase,DramScopePtr[2].DramLimit));

  //
  //PAM
  //
  DramScopePtr[3].ValidFlag = 1;
  DramScopePtr[3].InclusiveFlag = 0;
  DramScopePtr[3].DramBase = 0xc0000; //C,D,E,F seg
  DramScopePtr[3].DramLimit = 0x100000 - 1;
  DEBUG ((EFI_D_INFO, " [AddressDecode Init] DRAM range 0x%x  ~  0x%lx\n", DramScopePtr[3].DramBase,DramScopePtr[3].DramLimit));


  //
  //block decode
  //
  if (IsBlockmodeEnable()) {
    DramScopePtr[4].ValidFlag = 1;
    DramScopePtr[4].InclusiveFlag = 1;
    DramScopePtr[4].DramBase = mSadTadScope[0].BlockMode.BlockBase;
    DramScopePtr[4].DramLimit =mSadTadScope[0].BlockMode.BlockLimit;
    DEBUG ((EFI_D_INFO, " [AddressDecode Init] DRAM range 0x%lx  ~  0x%lx\n", DramScopePtr[4].DramBase,DramScopePtr[4].DramLimit));
  }
}

/**
  This routine is to check whether system address belongs to DRAM space.

  @param[in] InputSystemAddress  system address to check
  @param[in] DramScopePtr        A pointer to DRAM resource array.

  @retval TRUE  -- DRAM address
          FALSE -- NON-DRAM address

**/
BOOLEAN
EFIAPI
IsAddressBelong2Dram (
  IN   UINT64       InputSystemAddress,
  IN   DRAM_SCOPE  *DramScopePtr
  )
{
  UINT32   Index;
  BOOLEAN  RetFlag = FALSE;

  for (Index = 0; Index < DRAM_SCOPE_NUMBER; Index ++) {

    if (DramScopePtr[Index].ValidFlag == 1) {

      if (InputSystemAddress >= DramScopePtr[Index].DramBase &&
            InputSystemAddress <= DramScopePtr[Index].DramLimit) {

        if (DramScopePtr[Index].InclusiveFlag == 1) {
          RetFlag = TRUE;
        } else {
          RetFlag = FALSE;
        }
      }

    } else {
      break;
    }
  }

  DEBUG ((EFI_D_INFO, "System address:%lx belong to DRAM or block decode? %d(YES-1; NO-0)  \n", InputSystemAddress, RetFlag));

  return  RetFlag;
}



/**
  This routine is to check whether this address belong to DRAM space.

  @param[in] Sa    --  System address

  @retval TRUE -- it is DRAM address; FALSE -- it is not DRAM address.

**/
BOOLEAN
EFIAPI
IsDramMemoryAddress (
  IN   UINT64    Sa
  )
{
  return IsAddressBelong2Dram (Sa, mDramScope);
}


