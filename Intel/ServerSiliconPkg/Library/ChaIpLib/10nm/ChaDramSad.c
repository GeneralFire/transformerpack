/** @file
  CHA Configuration file for IO related registers.

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

//
// Include files
//
#include "Include/ChaRegs.h"
#include <Library/UsraCsrLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/ChaLib.h>

/**
  CHA: Replicate DRAM Settings from SBSP to to be online PBSP

  @param SbspSocId           - SBSP Node number (Socket ID)
  @param BoxInst             - Box Instance, 0 based
  @param TobeOnlinePbspSocId - to be online PBSP Node number

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaRasReplicateDramSetting (
  UINT8    SbspSocId,
  UINT8    BoxInst,
  UINT8    TobeOnlinePbspSocId
  )
{
  DRAM_RULE_CFG_0_CHABC_SAD_STRUCT     ChaDramRule;
  UINT32 Data32;
  UINT8  i;

  UINT32 DramRuleCfgCha[] = {
    DRAM_RULE_CFG_0_CHABC_SAD_REG, DRAM_RULE_CFG_1_CHABC_SAD_REG, DRAM_RULE_CFG_2_CHABC_SAD_REG,
    DRAM_RULE_CFG_3_CHABC_SAD_REG, DRAM_RULE_CFG_4_CHABC_SAD_REG, DRAM_RULE_CFG_5_CHABC_SAD_REG,
    DRAM_RULE_CFG_6_CHABC_SAD_REG, DRAM_RULE_CFG_7_CHABC_SAD_REG, DRAM_RULE_CFG_8_CHABC_SAD_REG,
    DRAM_RULE_CFG_9_CHABC_SAD_REG, DRAM_RULE_CFG_10_CHABC_SAD_REG, DRAM_RULE_CFG_11_CHABC_SAD_REG,
    DRAM_RULE_CFG_12_CHABC_SAD_REG, DRAM_RULE_CFG_13_CHABC_SAD_REG, DRAM_RULE_CFG_14_CHABC_SAD_REG,
    DRAM_RULE_CFG_15_CHABC_SAD_REG
  };

  UINT32 InterleaveListCfgCha[] = {
    INTERLEAVE_LIST_CFG_0_CHABC_SAD_REG, INTERLEAVE_LIST_CFG_1_CHABC_SAD_REG, INTERLEAVE_LIST_CFG_2_CHABC_SAD_REG,
    INTERLEAVE_LIST_CFG_3_CHABC_SAD_REG, INTERLEAVE_LIST_CFG_4_CHABC_SAD_REG, INTERLEAVE_LIST_CFG_5_CHABC_SAD_REG,
    INTERLEAVE_LIST_CFG_6_CHABC_SAD_REG, INTERLEAVE_LIST_CFG_7_CHABC_SAD_REG, INTERLEAVE_LIST_CFG_8_CHABC_SAD_REG,
    INTERLEAVE_LIST_CFG_9_CHABC_SAD_REG, INTERLEAVE_LIST_CFG_10_CHABC_SAD_REG, INTERLEAVE_LIST_CFG_11_CHABC_SAD_REG,
    INTERLEAVE_LIST_CFG_12_CHABC_SAD_REG, INTERLEAVE_LIST_CFG_13_CHABC_SAD_REG, INTERLEAVE_LIST_CFG_14_CHABC_SAD_REG,
    INTERLEAVE_LIST_CFG_15_CHABC_SAD_REG
  };

  //
  // Replicate the SBSP's DRAM decoder setting into the onlined CPU socket
  // Replicate the DRAM decoders starting with entry 0.
  //
  for (i = 0; i < 16; i++) {
    // Read SBSP's DRAM decoder entries
    ChaDramRule.Data = UsraCsrRead (SbspSocId, BoxInst, DramRuleCfgCha[i]);
    UsraCsrWrite (TobeOnlinePbspSocId, BoxInst, DramRuleCfgCha[i], ChaDramRule.Data);

    Data32 = UsraCsrRead (SbspSocId, BoxInst, InterleaveListCfgCha[i]);
    Data32 &= 0x77777777;        // Change the local memory of SBSP to Remote for onlined socket
    UsraCsrWrite (TobeOnlinePbspSocId, BoxInst, InterleaveListCfgCha[i], Data32);
  }

  return EFI_SUCCESS;
}