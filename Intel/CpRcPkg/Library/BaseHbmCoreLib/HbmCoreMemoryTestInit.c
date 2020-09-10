/** @file
  Interface source file for the HBM library class.

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

#include "HbmCore.h"
#include <Library/EmulationConfigurationLib.h>

/**

  Memory test wrapper

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                AdvMemTestOptions   Advance memory test options

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
HbmAdvancedMemTestWorker (
  IN        UINT8           Socket,
  IN        UINT32          AdvMemTestOptions
  )
{
  UINT8               Ch;
  UINT32              ChEnabled;
  UINT32              MaxRepeatTest;
  UINT32              RepeatTest;
  UINT32              Status = EFI_SUCCESS;
  UINT32              ScrambleConfigOrg[MAX_HBM_CH];
  UINT8               MaxHbmChPerSocket;
  SetMem ((UINT8 *)ScrambleConfigOrg, sizeof (ScrambleConfigOrg), 0);

  SetUbiosOutputMode (ASM_OUTPUT_DISABLE);
  MaxHbmChPerSocket = GetMaxChHbm ();

  ChEnabled = 0;

  MaxRepeatTest = 1;

  for (RepeatTest = 0; RepeatTest < MaxRepeatTest; RepeatTest++) {
    for (Ch = 0; Ch < MaxHbmChPerSocket; Ch++) {
      if (!IsHbmChannelEnabled (Socket, Ch)) {
        continue;
      }

      ChEnabled |= 1 << Ch;

      ScrambleConfigOrg[Ch] = HbmPreAdvancedMemTest (Socket, Ch);
    }

    //
    // Chip hook to re-enable scrambling
    //
    for (Ch = 0; Ch < MaxHbmChPerSocket; Ch++) {
      if (!IsHbmChannelEnabled (Socket, Ch)) {
        continue;
      }

      HbmPostAdvancedMemTestRestoreScrambler (Socket, Ch, ScrambleConfigOrg[Ch]);
    }

    if (AdvMemTestOptions & HBM_ADV_MT_LTEST_SCRAM) {
      Status = HbmMemTestScram (Socket, ChEnabled, HBM_ADV_MT_TYPE_LTEST_SCRAM, 64, 0, RepeatTest);
    }

    //
    // Chip hook to re-enable scrambling and other stuff that was modified for Adv memtest
    //
    for (Ch = 0; Ch < MaxHbmChPerSocket; Ch++) {
      if (!IsHbmChannelEnabled (Socket, Ch)) {
        continue;
      }

      HbmPostAdvancedMemTest (Socket, Ch, ScrambleConfigOrg[Ch]);
    }
  }

  SetUbiosOutputMode (ASM_OUTPUT_ENABLE);

  return Status;
} // AdvancedMemTestWorker

/**

  Memory Test - Modified Scan algorithm

  @param[in]                Socket              Current Socket (0-Based)

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
HbmMemoryTest (
  IN        UINT8           Socket
  )
{
  MRC_MST   TmpMemSsType;

  if (!IsSocketEnabled (Socket)) {
    return EFI_SUCCESS;
  }

  if (!IsHbmFlowEnabled (HF_MEM_TEST)) {
    return EFI_SUCCESS;
  }

  TmpMemSsType = GetMemSsType (Socket);;
  SetMemSsType (Socket, Hbm2MemSs);

  HbmAdvancedMemTestWorker (Socket, HBM_ADV_MT_LTEST_SCRAM);

  SetMemSsType (Socket, TmpMemSsType);
  return EFI_SUCCESS;
} // HbmMemoryTest

/**

  Memory Init - initialize all the HBM Memory to '0'

  @param[in]                Socket              Current Socket (0-Based)

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
HbmMemoryInit (
  IN        UINT8           Socket
  )
{
  UINT8     Ch;
  MRC_MST   TmpMemSsType;
  UINT8     NumCl;
  UINT32    Status  = 0;
  UINT32    ChEnabled;
  UINT8     MaxHbmChPerSocket;

  NumCl         = 1;
  ChEnabled     = 0;

  MaxHbmChPerSocket = GetMaxChHbm ();

  if (!IsSocketEnabled (Socket)) {
    return EFI_SUCCESS;
  }

  if (!IsHbmFlowEnabled (HF_MEM_INIT)) {
    return EFI_SUCCESS;
  }

  //
  // Skip MemInit if the skip meminit flag is set and memory does not need to be wiped
  //
  if (IsSkipEn () && !IsWipeEn ()) {
    return EFI_SUCCESS;
  }

  //
  // Execute MemInit only when ECC is enabled or memory needs to be wiped
  //
  if (!IsEccEn () && !IsWipeEn ()) {
    return EFI_SUCCESS;
  }

  for (Ch = 0; Ch < MaxHbmChPerSocket; Ch++) {
    if (!IsHbmChannelEnabled (Socket, Ch)) {
      continue;
    }

    ChEnabled |= 1 << Ch;
  }

  //1. Write pattern to all of memory
  TmpMemSsType = GetMemSsType (Socket);;
  SetMemSsType (Socket, Hbm2MemSs);

  Status = HbmCpgcMemTestWithScramble (Socket, ChEnabled, NumCl, 1, HBM_MT_ADDR_DIR_UP);

  SetMemSsType (Socket, TmpMemSsType);

  return Status;
} // HbmMemoryInit
