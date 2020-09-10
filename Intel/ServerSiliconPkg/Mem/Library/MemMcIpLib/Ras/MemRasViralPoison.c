/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include <Library/CpuCsrAccessLib.h>
#include "Include/MemMcRegs.h"
#include <SiliconSetting.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/PcuMailBoxRasLib.h>

/**
  Configure DDRT viral.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
**/
VOID
EFIAPI
DdrtViralConfig (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt
  )
{
  DDRT_DEFEATURE_MC_2LM_STRUCT     DdrtDefeatureStruct;
  UINT32                           Command;
  UINT8                            Node;

  DdrtDefeatureStruct.Data = ReadCpuCsr (Skt, ChOnSkt, DDRT_DEFEATURE_MC_2LM_REG);
  DdrtDefeatureStruct.Bits.ignore_viral = 0;

  if (IsSiliconWorkaroundEnabled ("S14010263993")) {
    Node = SKT_CH_TO_NODE(Skt,ChOnSkt);
    Command = MAILBOX_BIOS_CR_WRITE_CMD (NODE_TO_MC(Node), SKTCH_TO_NODECH (ChOnSkt), MAILBOX_BIOS_CMD_CR_PROXY_DDRT_DEFEATURE_INDEX);
    ReadWritePcuMailbox (Skt, MAILBOX_BIOS_CMD_ZERO, Command, &DdrtDefeatureStruct.Data, 0, 0);
  } else {
    WriteCpuCsr (Skt, ChOnSkt, DDRT_DEFEATURE_MC_2LM_REG, DdrtDefeatureStruct.Data);
  }
}
