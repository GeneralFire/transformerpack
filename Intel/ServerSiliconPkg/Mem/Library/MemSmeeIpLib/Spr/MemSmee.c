/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include "Include/MemSmeeRegs.h"
#include <Library/MemSmeeIpLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/RcDebugLib.h>
#include <Library/MemRcLib.h>
#include <CmiPerformance.h>
#include "MemSmeeCmiCredits.h"

//
// Indicate AppDirect or AppDirect-WB region access for MK-TME.
//
#define PERSISTENT1_ID    0x01
#define PERSISTENT2_ID    0x02

/**
  This function programs the MK-TME related CSRs with the range of AppDirect and/or AppDirect-WB regions.

  @param[in] ScktId             Socket index.
  @param[in] McId               Memory controller index.
  @param[in] ChId               Channel index on Memory controller.
  @param[in] MktmePmemData      The base and limit of the AppDirect and/or AppDirect-WB regions.

  @retval N/A
**/
VOID
SmeeSetMktmePmemData (
  IN UINT8            ScktId,
  IN UINT8            McId,
  IN UINT8            ChId,
  IN MKTME_PMEM_DATA  *MktmePmemData
  )
{
  UINT8                                     MaxChPerMc;
  TME_PERSISTENT1_BASE_SMEE_TME_STRUCT      Persistent1BaseReg;
  TME_PERSISTENT1_LIMIT_SMEE_TME_STRUCT     Persistent1LimitReg;
  TME_PERSISTENT2_BASE_SMEE_TME_STRUCT      Persistent2BaseReg;
  TME_PERSISTENT2_LIMIT_SMEE_TME_STRUCT     Persistent2LimitReg;
  TME_PERSISTENT_ADDR_IDS_SMEE_TME_STRUCT   AddrIdsReg;

  MaxChPerMc = GetNumChannelPerMc ();

  //
  // Append socket ID, MC ID and channel ID to make the input of AES tweak unique for each socket/MC/channel.
  //
  AddrIdsReg.Data = 0;
  AddrIdsReg.Bits.ad_id = PERSISTENT1_ID;
  AddrIdsReg.Bits.adwb_id = PERSISTENT2_ID;
  AddrIdsReg.Bits.apped_id = ((UINT32) ScktId << 8) | ((UINT32) McId << 4) | ChId;
  MemWritePciCfgEp (ScktId, ChId + McId * MaxChPerMc, TME_PERSISTENT_ADDR_IDS_SMEE_TME_REG, AddrIdsReg.Data);

  //
  // Program the related CSRs with the range of AppDirect and/or AppDirect-WB regions.
  //
  Persistent1BaseReg.Data = 0;
  Persistent2BaseReg.Data = 0;
  Persistent1BaseReg.Bits.base = MktmePmemData->Pmem1Base;
  Persistent2BaseReg.Bits.base = MktmePmemData->Pmem2Base;

  Persistent1LimitReg.Data = 0;
  Persistent2LimitReg.Data = 0;
  Persistent1LimitReg.Bits.limit = MktmePmemData->Pmem1Limit;
  Persistent2LimitReg.Bits.limit = MktmePmemData->Pmem2Limit;

  //
  // Enable base or mask check.
  //
  if (MktmePmemData->Pmem1Limit != 0) {
    Persistent1LimitReg.Bits.range_enable = 1;
  }
  if (MktmePmemData->Pmem2Limit != 0) {
    Persistent2LimitReg.Bits.range_enable = 1;
  }

  MemWritePciCfgEp (ScktId, ChId + McId * MaxChPerMc, TME_PERSISTENT1_BASE_SMEE_TME_REG, Persistent1BaseReg.Data);
  MemWritePciCfgEp (ScktId, ChId + McId * MaxChPerMc, TME_PERSISTENT1_LIMIT_SMEE_TME_REG, Persistent1LimitReg.Data);
  MemWritePciCfgEp (ScktId, ChId + McId * MaxChPerMc, TME_PERSISTENT2_BASE_SMEE_TME_REG, Persistent2BaseReg.Data);
  MemWritePciCfgEp (ScktId, ChId + McId * MaxChPerMc, TME_PERSISTENT2_LIMIT_SMEE_TME_REG, Persistent2LimitReg.Data);

  RcDebugPrint (SDBG_MAX,
    "MK-TME: Socket%d Mc%d Channel%d, Pmem1 base: 0x%x limit: 0x%x, Pmem2 base: 0x%x limit: 0x%x, AppendIds: 0x%x\n",
    ScktId, McId, ChId,
    Persistent1BaseReg.Bits.base,
    Persistent1LimitReg.Bits.limit,
    Persistent2BaseReg.Bits.base,
    Persistent2LimitReg.Bits.limit,
    AddrIdsReg.Data
    );
}

/**
  Program Tme CMI Qdepth Credit Registers

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number
  @param[in] ProgramableValue  - Programmable CMI credit value

  @retval N/A
**/
VOID
EFIAPI
ProgramMcTmeCmiQdepthRegisters (
  IN UINT8   Socket,
  IN UINT8   Ch,
  IN UINT8   ProgramableValue
  )
{
  UINT32 RegData = 0;
  UINT8 IndexVal = 0;
  TME_CMI_QDEPTH_N0_SMEE_TME_STRUCT TmeCmiQdepthRegValue0;
  CMI_DEFAULT_REG_VALUE             *McTmeCmiQdepthValue0 = NULL;
  UINT8                             McTmeCmiQdepthLen0 = 0;
  TME_CMI_QDEPTH_N0_SMEE_TME_STRUCT TmeCmiQdepthRegValue1;
  CMI_DEFAULT_REG_VALUE             *McTmeCmiQdepthValue1 = NULL;
  UINT8                             McTmeCmiQdepthLen1 = 0;

  GetMcTmeCmiQdepthValue0ChannelSpr (Socket, Ch, &McTmeCmiQdepthValue0, &McTmeCmiQdepthLen0);
  GetMcTmeCmiQdepthValue1ChannelSpr (Socket, Ch, &McTmeCmiQdepthValue1, &McTmeCmiQdepthLen1);

  //tme_cmi_qdepth starts
  if (McTmeCmiQdepthValue0 != NULL) {
    TmeCmiQdepthRegValue0.Data = MemReadPciCfgEp (Socket, Ch, TME_CMI_QDEPTH_N0_SMEE_TME_REG);
    for (IndexVal = 0; IndexVal < McTmeCmiQdepthLen0; IndexVal++) {
      RegData = (TmeCmiQdepthRegValue0.Data >> McTmeCmiQdepthValue0[IndexVal].BitStart) & McTmeCmiQdepthValue0[IndexVal].AndVal;
      TmeCmiQdepthRegValue0.Data &= ~(McTmeCmiQdepthValue0[IndexVal].AndVal << McTmeCmiQdepthValue0[IndexVal].BitStart);
      switch (ProgramableValue) {
        case CMI_DDRT_VAL:
          RegData = McTmeCmiQdepthValue0[IndexVal].DdrtVal;
          break;
        case CMI_1LM_VAL:
          RegData = McTmeCmiQdepthValue0[IndexVal].OneLMVal;
          break;
        case CMI_CR_VAL:
          RegData = McTmeCmiQdepthValue0[IndexVal].CRVal;
          break;
        case CMI_1LM_ISOCH_VAL:
          RegData = McTmeCmiQdepthValue0[IndexVal].OneLMIsoChVal;
          break;
        case CMI_1LM_2WAY_VAL:
          RegData = McTmeCmiQdepthValue0[IndexVal].OneLM2WAYVal;
          break;
        case CMI_CR_2WAY_VAL:
          RegData = McTmeCmiQdepthValue0[IndexVal].CR2WAYVal;
          break;
        default:
          break;
      }
      TmeCmiQdepthRegValue0.Data |= (RegData << McTmeCmiQdepthValue0[IndexVal].BitStart);
    }
    MemWritePciCfgEp (Socket, Ch, TME_CMI_QDEPTH_N0_SMEE_TME_REG, TmeCmiQdepthRegValue0.Data);
  }

  if (McTmeCmiQdepthValue1 != NULL) {
    TmeCmiQdepthRegValue1.Data = MemReadPciCfgEp (Socket, Ch, TME_CMI_QDEPTH_N1_SMEE_TME_REG);
    for (IndexVal = 0; IndexVal < McTmeCmiQdepthLen1; IndexVal++) {
      RegData = (TmeCmiQdepthRegValue1.Data >> McTmeCmiQdepthValue1[IndexVal].BitStart) & McTmeCmiQdepthValue1[IndexVal].AndVal;
      TmeCmiQdepthRegValue1.Data &= ~(McTmeCmiQdepthValue1[IndexVal].AndVal << McTmeCmiQdepthValue1[IndexVal].BitStart);
      switch (ProgramableValue) {
        case CMI_DDRT_VAL:
          RegData = McTmeCmiQdepthValue1[IndexVal].DdrtVal;
          break;
        case CMI_1LM_VAL:
          RegData = McTmeCmiQdepthValue1[IndexVal].OneLMVal;
          break;
        case CMI_CR_VAL:
          RegData = McTmeCmiQdepthValue1[IndexVal].CRVal;
          break;
        case CMI_1LM_ISOCH_VAL:
          RegData = McTmeCmiQdepthValue1[IndexVal].OneLMIsoChVal;
          break;
        case CMI_1LM_2WAY_VAL:
          RegData = McTmeCmiQdepthValue1[IndexVal].OneLM2WAYVal;
          break;
        case CMI_CR_2WAY_VAL:
          RegData = McTmeCmiQdepthValue1[IndexVal].CR2WAYVal;
          break;
        default:
          break;
      }
      TmeCmiQdepthRegValue1.Data |= (RegData << McTmeCmiQdepthValue1[IndexVal].BitStart);
    }
    MemWritePciCfgEp (Socket, Ch, TME_CMI_QDEPTH_N1_SMEE_TME_REG, TmeCmiQdepthRegValue1.Data);
  }
  //tme_cmi_qdepth ends
}