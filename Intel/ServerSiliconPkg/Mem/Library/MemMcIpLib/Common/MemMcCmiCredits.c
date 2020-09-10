/** @file
  Mem Mc CMI Credit information

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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
#include <SysHostChip.h>
#include <MemCommon.h>
#include <CmiPerformance.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemRcLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include "Include/MemMcRegs.h"
#include "Include/MemMcIpLibInternal.h"
#include "Include/MemMcCmiCredits.h"
#include <Library/MemTypeLib.h>
#include <Library/MemoryServicesLib.h>

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0ValueA0[] = {
  { 0, 31, 23, 24, 24, 0x1f},
  { 5, 31, 23, 24, 24, 0x1f},
  { 10, 31, 22, 22, 23, 0x3f},
  { 16, 31, 22, 22, 23, 0x3f},
  { 22, 35, 48, 48, 35, 0x3f},
  { 28, 3, 0, 0, 3, 0xf},
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1ValueA0[] = {
  { 0 ,0, 1, 1, 0, 0x3},
  { 2, 58, 54, 55, 50, 0x3f},
  { 8, 116, 108, 110, 100, 0xff},
  { 16, 0, 1, 1, 1, 0xf},
  { 20, 0, 3, 0, 0, 0xf},
  { 24, 1, 0, 0, 1, 0xf},
  { 28, 0, 0, 0, 0, 0xf},
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0A0[] = {
  { 0, 0, 1, 1, 1, 0x3},
  { 2, 0, 1, 0, 0, 0x3},
  { 4, 1, 0, 0, 1, 0x3},
  { 6, 0, 0, 0, 0, 0x3},
  { 8, 0, 1, 1, 1, 0x3},
  { 10, 0, 1, 0, 0, 0x3},
  { 12, 1, 0, 0, 1, 0x3},
  { 14, 0, 0, 0, 0, 0x3},
  { 16, 0, 8, 8, 8, 0xf},
  { 20, 0, 1, 0, 0, 0xf},
  { 24, 5, 0, 0, 5, 0xf},
  { 28, 0, 0, 0, 0, 0xf},
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1A0[] = {
  { 0, 0, 8, 8, 8, 0xf},
  { 4, 0, 1, 0, 0, 0xf},
  { 8, 5, 0, 0, 5, 0xf},
  { 12, 0, 0, 0, 0, 0xf},
  { 16, 4, 4, 4, 4, 0x1f},
};

STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValueA0[] = {
  { 0, 7, 7, 7, 7, 0x1f},
  { 5, 12, 12, 12, 12, 0x1f},
  { 10, 12, 12, 12, 12, 0x1f},
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0ValueB0[] = {
  { 0, 31, 23, 24, 24, 0x1f},
  { 5, 31, 23, 24, 24, 0x1f},
  { 10, 31, 22, 23, 23, 0x3f},
  { 16, 31, 22, 23, 23, 0x3f},
  { 22, 43, 48, 48, 43, 0x3f},
  { 28, 11, 0, 0, 11, 0xf},
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1ValueB0[] = {
  { 0 ,0, 1, 1, 0, 0x3},
  { 2, 58, 54, 55, 50, 0x3f},
  { 8, 148, 140, 142, 132, 0xff},
  { 16, 0, 1, 1, 1, 0xf},
  { 20, 0, 3, 0, 0, 0xf},
  { 24, 1, 0, 0, 1, 0xf},
  { 28, 0, 0, 0, 0, 0xf},
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0B0[] = {
  { 0, 0, 1, 1, 1, 0x3},
  { 2, 0, 1, 0, 0, 0x3},
  { 4, 1, 0, 0, 1, 0x3},
  { 6, 0, 0, 0, 0, 0x3},
  { 8, 0, 1, 1, 1, 0x3},
  { 10, 0, 1, 0, 0, 0x3},
  { 12, 1, 0, 0, 1, 0x3},
  { 14, 0, 0, 0, 0, 0x3},
  { 16, 0, 8, 8, 8, 0xf},
  { 20, 0, 1, 0, 0, 0xf},
  { 24, 5, 0, 0, 5, 0xf},
  { 28, 0, 0, 0, 0, 0xf},
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1B0[] = {
  { 0, 0, 8, 8, 8, 0xf},
  { 4, 0, 1, 0, 0, 0xf},
  { 8, 5, 0, 0, 5, 0xf},
  { 12, 0, 0, 0, 0, 0xf},
  { 16, 4, 4, 4, 4, 0x1f},
};

STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValueB0[] = {
  { 0, 7, 7, 7, 7, 0x1f},
  { 5, 12, 12, 12, 12, 0x1f},
  { 10, 12, 12, 12, 12, 0x1f},
};

/**
  Get MC CMI Credit Config N0 Value
  Returns value of ICX

  @param[in]   Socket          -   Socket number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcCmiCreditCfgN0Value (
  IN  UINT8 Socket,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  if (IsSiliconWorkaroundEnabled ("S1406925100")) {
    *ConfigValue = McCmiCreditCfgN0ValueA0;
    *Size = sizeof(McCmiCreditCfgN0ValueA0)/sizeof(CMI_DEFAULT_REG_VALUE);
  } else {
    *ConfigValue = McCmiCreditCfgN0ValueB0;
    *Size = sizeof(McCmiCreditCfgN0ValueB0)/sizeof(CMI_DEFAULT_REG_VALUE);
  }

  return EFI_SUCCESS;
}

/**
  Get MC CMI Credit Config N1 Value
  Returns value of ICX

  @param[in]   Socket          -   Socket number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcCmiCreditCfgN1Value (
  IN  UINT8 Socket,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  if (IsSiliconWorkaroundEnabled ("S1406925100")) {
    *ConfigValue = McCmiCreditCfgN1ValueA0;
    *Size = sizeof(McCmiCreditCfgN1ValueA0)/sizeof(CMI_DEFAULT_REG_VALUE);
  } else {
    *ConfigValue = McCmiCreditCfgN1ValueB0;
    *Size = sizeof(McCmiCreditCfgN1ValueB0)/sizeof(CMI_DEFAULT_REG_VALUE);
  }

  return EFI_SUCCESS;
}

/**
  Get MC CMI Request Completion Config Value0
  Returns value of ICX

  @param[in]   Socket          -   Socket number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcCmiReqCpcfgConfigValue0 (
  IN  UINT8 Socket,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  if (IsSiliconWorkaroundEnabled ("S1406925100")) {
    *ConfigValue = McCmiReqCpcfgConfigValue0A0;
    *Size = sizeof(McCmiReqCpcfgConfigValue0A0)/sizeof(CMI_DEFAULT_REG_VALUE);
  } else {
    *ConfigValue = McCmiReqCpcfgConfigValue0B0;
    *Size = sizeof(McCmiReqCpcfgConfigValue0B0)/sizeof(CMI_DEFAULT_REG_VALUE);
  }

  return EFI_SUCCESS;
}

/**
  Get MC CMI Request Completion Config Value1
  Returns value of ICX

  @param[in]   Socket          -   Socket number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcCmiReqCpcfgConfigValue1 (
  IN  UINT8 Socket,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  if (IsSiliconWorkaroundEnabled ("S1406925100")) {
    *ConfigValue = McCmiReqCpcfgConfigValue1A0;
    *Size = sizeof(McCmiReqCpcfgConfigValue1A0)/sizeof(CMI_DEFAULT_REG_VALUE);
  } else {
    *ConfigValue = McCmiReqCpcfgConfigValue1B0;
    *Size = sizeof(McCmiReqCpcfgConfigValue1B0)/sizeof(CMI_DEFAULT_REG_VALUE);
  }

  return EFI_SUCCESS;
}

/**
  Get MC MISC CMI credit config Value
  Returns value of ICX

  @param[in]   Socket          -   Socket number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcMiscCmiCreditConfigValue (
  IN  UINT8 Socket,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  )
{
  if (IsSiliconWorkaroundEnabled ("S1406925100")) {
    *ConfigValue = McMiscCmiCreditConfigValueA0;
    *Size = sizeof(McMiscCmiCreditConfigValueA0)/sizeof(CMI_DEFAULT_REG_VALUE);
  } else {
    *ConfigValue = McMiscCmiCreditConfigValueB0;
    *Size = sizeof(McMiscCmiCreditConfigValueB0)/sizeof(CMI_DEFAULT_REG_VALUE);
  }

  return EFI_SUCCESS;
}

/**
  Program McDp CMI Credit Registers

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number
  @param[in] ProgramableValue  - Programmable CMI credit value

  @retval N/A
**/
VOID
EFIAPI
ProgramMcDpCmiRegisters (
  IN UINT8 ProgramableValue[MAX_CH],
  IN UINT8 Socket,
  IN UINT8 McId
  )
{
}


/**

  Program Mem Mc CMI registers

  @param[in] ProgramableValue[MAX_CH]    - Array containing type of value to program per channel (From 0 to Max Channel in Socket)
                                           1 = DDRT,2 = 1LM,3 = CR, 4 = 1LM ISOCH..CMI value
  @param[in] Socket                      - Socket number

  @retVal   1 CMI Registers initialized
            0 CMI Registers not initialized
**/
UINT32
EFIAPI
ProgramMemMcCmiRegisters (
  IN UINT8 ProgramableValue[MAX_CH],
  IN UINT8 Socket
  )
{
  UINT32                   Status                       = 0;
  CMI_DEFAULT_REG_VALUE    *McCmiCreditCfgN0Value       = NULL;
  CMI_DEFAULT_REG_VALUE    *McCmiCreditCfgN1Value       = NULL;
  CMI_DEFAULT_REG_VALUE    *McCmiReqCpcfgConfigValue0   = NULL;
  CMI_DEFAULT_REG_VALUE    *McCmiReqCpcfgConfigValue1   = NULL;
  CMI_DEFAULT_REG_VALUE    *McMiscCmiCreditConfigValue  = NULL;
  UINT8                    McCmiCreditCfgN0Len          = 0;
  UINT8                    McCmiCreditCfgN1Len          = 0;
  UINT8                    McCmiReqCpcfgConfigValue0Len = 0;
  UINT8                    McCmiReqCpcfgConfigValue1Len = 0;
  UINT8                    McMiscCmiCreditConfigLen     = 0;
  UINT32                   RegData                      = 0;
  UINT32                   VcFloor                      = 0;
  UINT32                   WpqFloor                     = 0;
  CMI_MAIN_REG_INFO        CmiMainRegInfo;
  UINT8                    McId;
  UINT8                    Ch;
  UINT8                    ChId;
  UINT8                    IndexVal;
  UINT8                    NumChPerMc;
  UINT8                    MaxImc;
  PSYSHOST                 Host;
  UINT32                   WpqThreshold = 0;
  UINT32                   ArrayValue[2];
  UINT32                   ArraySize = sizeof (ArrayValue)/sizeof (ArrayValue[0]);

  struct      channelNvram (*ChannelNvList)[MAX_CH];

  Host = GetSysHostPointer ();
  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();


  ChannelNvList = GetChannelNvList (Host, Socket);

  GetMcCmiCreditCfgN0Value (Socket, &McCmiCreditCfgN0Value ,&McCmiCreditCfgN0Len);
  GetMcCmiCreditCfgN1Value (Socket, &McCmiCreditCfgN1Value ,&McCmiCreditCfgN1Len);
  GetMcCmiReqCpcfgConfigValue0 (Socket, &McCmiReqCpcfgConfigValue0 ,&McCmiReqCpcfgConfigValue0Len);
  GetMcCmiReqCpcfgConfigValue1 (Socket, &McCmiReqCpcfgConfigValue1 ,&McCmiReqCpcfgConfigValue1Len);
  GetMcMiscCmiCreditConfigValue (Socket, &McMiscCmiCreditConfigValue ,&McMiscCmiCreditConfigLen);

  for (McId = 0; McId < MaxImc; McId++) {
    if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
      continue;
    }

    ModifyMcCmiCreditProgrammingChip (&McCmiCreditCfgN0Value,
                                      &McCmiCreditCfgN1Value,
                                      &McCmiReqCpcfgConfigValue0,
                                      &McCmiReqCpcfgConfigValue1,
                                      &McMiscCmiCreditConfigValue,
                                      &McCmiCreditCfgN0Len,
                                      &McCmiCreditCfgN1Len,
                                      &McCmiReqCpcfgConfigValue0Len,
                                      &McCmiReqCpcfgConfigValue1Len,
                                      &McMiscCmiCreditConfigLen);

    for (ChId = 0; ChId < NumChPerMc; ChId++) {
      Ch = (McId * NumChPerMc) + ChId;
      CmiMainRegInfo.McCmiReqCpcfg.Data = MemReadPciCfgEp (Socket, Ch, MCCMIREQCPCFG_N0_MC_2LM_REG);
      for (IndexVal = 0; IndexVal < McCmiReqCpcfgConfigValue0Len; IndexVal++) {
        RegData = (CmiMainRegInfo.McCmiReqCpcfg.Data >> McCmiReqCpcfgConfigValue0[IndexVal].BitStart) & McCmiReqCpcfgConfigValue0[IndexVal].AndVal;
        CmiMainRegInfo.McCmiReqCpcfg.Data &= ~(McCmiReqCpcfgConfigValue0[IndexVal].AndVal << McCmiReqCpcfgConfigValue0[IndexVal].BitStart);
        switch (ProgramableValue[Ch]) {
          case CMI_DDRT_VAL:
            RegData = McCmiReqCpcfgConfigValue0[IndexVal].DdrtVal;
            break;
          case CMI_1LM_VAL:
            RegData = McCmiReqCpcfgConfigValue0[IndexVal].OneLMVal;
              break;
          case CMI_CR_VAL:
            RegData = McCmiReqCpcfgConfigValue0[IndexVal].CRVal;
            break;
          case CMI_1LM_ISOCH_VAL:
            RegData = McCmiReqCpcfgConfigValue0[IndexVal].OneLMIsoChVal;
            break;
          default:
            break;
        }
        CmiMainRegInfo.McCmiReqCpcfg.Data |= (RegData << McCmiReqCpcfgConfigValue0[IndexVal].BitStart);
      }
      MemWritePciCfgEp (Socket, Ch, MCCMIREQCPCFG_N0_MC_2LM_REG, CmiMainRegInfo.McCmiReqCpcfg.Data);

      CmiMainRegInfo.McCmiReqCpcfg.Data = MemReadPciCfgEp (Socket, Ch, MCCMIREQCPCFG_N1_MC_2LM_REG);
      for (IndexVal = 0; IndexVal < McCmiReqCpcfgConfigValue1Len; IndexVal++) {
        RegData = (CmiMainRegInfo.McCmiReqCpcfg.Data >> McCmiReqCpcfgConfigValue1[IndexVal].BitStart) & McCmiReqCpcfgConfigValue1[IndexVal].AndVal;
        CmiMainRegInfo.McCmiReqCpcfg.Data &= ~(McCmiReqCpcfgConfigValue1[IndexVal].AndVal << McCmiReqCpcfgConfigValue1[IndexVal].BitStart);
        switch (ProgramableValue[Ch]) {
          case CMI_DDRT_VAL:
            RegData = McCmiReqCpcfgConfigValue1[IndexVal].DdrtVal;
            break;
          case CMI_1LM_VAL:
            RegData = McCmiReqCpcfgConfigValue1[IndexVal].OneLMVal;
              break;
          case CMI_CR_VAL:
            RegData = McCmiReqCpcfgConfigValue1[IndexVal].CRVal;
            break;
          case CMI_1LM_ISOCH_VAL:
            RegData = McCmiReqCpcfgConfigValue1[IndexVal].OneLMIsoChVal;
            break;
          default:
            break;
        }
        CmiMainRegInfo.McCmiReqCpcfg.Data |= (RegData << McCmiReqCpcfgConfigValue1[IndexVal].BitStart);
      }

      GetMinimumVcFloor (CmiMainRegInfo.McCmiReqCpcfg.Bits.wr_vc0_floor, CmiMainRegInfo.McCmiReqCpcfg.Bits.wr_vc1_floor, CmiMainRegInfo.McCmiReqCpcfg.Bits.wr_vc3_floor, &VcFloor);
      MemWritePciCfgEp (Socket, Ch, MCCMIREQCPCFG_N1_MC_2LM_REG, CmiMainRegInfo.McCmiReqCpcfg.Data);

      //
      //MCChan
      //
      CmiMainRegInfo.McCmiCreditCfgN0.Data = MemReadPciCfgEp (Socket, Ch, MCSTRUCTCMICREDITCFG_N0_MC_2LM_REG);
      for (IndexVal = 0; IndexVal < McCmiCreditCfgN0Len; IndexVal++) {
        RegData = (CmiMainRegInfo.McCmiCreditCfgN0.Data >> McCmiCreditCfgN0Value[IndexVal].BitStart) & McCmiCreditCfgN0Value[IndexVal].AndVal;
        CmiMainRegInfo.McCmiCreditCfgN0.Data &= ~(McCmiCreditCfgN0Value[IndexVal].AndVal << McCmiCreditCfgN0Value[IndexVal].BitStart);
        switch (ProgramableValue[Ch]) {
          case CMI_DDRT_VAL:
            RegData = McCmiCreditCfgN0Value[IndexVal].DdrtVal;
            break;
          case CMI_1LM_VAL:
            RegData = McCmiCreditCfgN0Value[IndexVal].OneLMVal;
            break;
          case CMI_CR_VAL:
            RegData = McCmiCreditCfgN0Value[IndexVal].CRVal;
            break;
          case CMI_1LM_ISOCH_VAL:
            RegData = McCmiCreditCfgN0Value[IndexVal].OneLMIsoChVal;
            break;
          default:
            break;
        }
        CmiMainRegInfo.McCmiCreditCfgN0.Data |= (RegData << McCmiCreditCfgN0Value[IndexVal].BitStart);
      }
      WpqFloor = MIN (CmiMainRegInfo.McCmiCreditCfgN0.Bits.wpq4_0_shared_cnt, CmiMainRegInfo.McCmiCreditCfgN0.Bits.wpq4_1_shared_cnt);
      MemWritePciCfgEp (Socket, Ch, MCSTRUCTCMICREDITCFG_N0_MC_2LM_REG, CmiMainRegInfo.McCmiCreditCfgN0.Data);

      CmiMainRegInfo.McCmiCreditCfgN1.Data = MemReadPciCfgEp (Socket, Ch, MCSTRUCTCMICREDITCFG_N1_MC_2LM_REG);
      for (IndexVal = 0; IndexVal < McCmiCreditCfgN1Len; IndexVal++) {
        RegData = (CmiMainRegInfo.McCmiCreditCfgN1.Data >> McCmiCreditCfgN1Value[IndexVal].BitStart) & McCmiCreditCfgN1Value[IndexVal].AndVal;
        CmiMainRegInfo.McCmiCreditCfgN1.Data &= ~(McCmiCreditCfgN1Value[IndexVal].AndVal << McCmiCreditCfgN1Value[IndexVal].BitStart);
        switch (ProgramableValue[Ch]) {
          case CMI_DDRT_VAL:
            RegData = McCmiCreditCfgN1Value[IndexVal].DdrtVal;
            break;
          case CMI_1LM_VAL:
            RegData = McCmiCreditCfgN1Value[IndexVal].OneLMVal;
            break;
          case CMI_CR_VAL:
            RegData = McCmiCreditCfgN1Value[IndexVal].CRVal;
            break;
          case CMI_1LM_ISOCH_VAL:
            RegData = McCmiCreditCfgN1Value[IndexVal].OneLMIsoChVal;
            break;
          default:
            break;
        }
        CmiMainRegInfo.McCmiCreditCfgN1.Data |= (RegData << McCmiCreditCfgN1Value[IndexVal].BitStart);
      }

      // MRC Shall reduce McStructCmiCreditCfg.sb_shared_cnt by 1,
      // from the desired CMI credit programming for memory channel
      // if it has DDRT populated or has DDR4 acting as NM for 2LM
      if (IsSiliconWorkaroundEnabled ("S14010199966")) {
        if (IsMemTypePresentMcFunction (Socket, McId, IsMemType2lm) || ((*ChannelNvList)[Ch].ddrtEnabled == 1)) {
          if (CmiMainRegInfo.McCmiCreditCfgN1.Bits.sb_shared_cnt > 0) {
            CmiMainRegInfo.McCmiCreditCfgN1.Bits.sb_shared_cnt--;
          }
        }
      }
      MemWritePciCfgEp (Socket, Ch, MCSTRUCTCMICREDITCFG_N1_MC_2LM_REG, CmiMainRegInfo.McCmiCreditCfgN1.Data);

      ArrayValue[0] = WpqFloor + VcFloor;
      ArrayValue[1] = (VcFloor + CmiMainRegInfo.McCmiCreditCfgN1.Bits.wdb_shared_cnt) / 2;

      WpqThreshold = GetMinimumOfNonZeroNumberN (ArrayValue, ArraySize);

      ProgramWPQThershold (Host, Socket, Ch, WpqThreshold);

      if (McMiscCmiCreditConfigValue != NULL) {
        CmiMainRegInfo.McMiscCmiCreditConfig[ChId].Data = MemReadPciCfgMC (Socket, McId, McMiscCmiCreditConfigRegAddr[ChId]);
        for (IndexVal = 0; IndexVal < McMiscCmiCreditConfigLen; IndexVal++) {
         RegData = (CmiMainRegInfo.McMiscCmiCreditConfig[ChId].Data >> McMiscCmiCreditConfigValue[IndexVal].BitStart) & McMiscCmiCreditConfigValue[IndexVal].AndVal;
         CmiMainRegInfo.McMiscCmiCreditConfig[ChId].Data &= ~(McMiscCmiCreditConfigValue[IndexVal].AndVal << McMiscCmiCreditConfigValue[IndexVal].BitStart);
         switch (ProgramableValue[Ch]) {
           case CMI_DDRT_VAL:
             RegData = McMiscCmiCreditConfigValue[IndexVal].DdrtVal;
             break;
           case CMI_1LM_VAL:
             RegData = McMiscCmiCreditConfigValue[IndexVal].OneLMVal;
             break;
           case CMI_CR_VAL:
             RegData = McMiscCmiCreditConfigValue[IndexVal].CRVal;
             break;
           case CMI_1LM_ISOCH_VAL:
             RegData = McMiscCmiCreditConfigValue[IndexVal].OneLMIsoChVal;
             break;
           default:
             break;
         }
         CmiMainRegInfo.McMiscCmiCreditConfig[ChId].Data |= (RegData << McMiscCmiCreditConfigValue[IndexVal].BitStart);
        }
        MemWritePciCfgMC (Socket, McId, McMiscCmiCreditConfigRegAddr[ChId], CmiMainRegInfo.McMiscCmiCreditConfig[ChId].Data);
      }
    }  
    ProgramMcDpCmiRegisters (ProgramableValue, Socket, McId);
  }

  return Status;
}

