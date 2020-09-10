/** @file
  Mesh 2 Mem CMI Credit information
  This file is for SPR HBM

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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
#include <SysHost.h>
#include <MemHost.h>
#include "Include/Mesh2MemRegs.h"
#include <Library/Mesh2MemIpLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemRcLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemAccessLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/HbmCoreLib.h>
#include <Library/SystemInfoLib.h>
#include <CmiPerformance.h>
#include <MemCommon.h>
#include "Mesh2MemCmiCredits.h"
#include "Mesh2MemCmiPerformanceDataHbm.h"

//
// Convert HBM Channel Number to HBM MC ID.
//
#define GET_HBM_MC_ID(HbmCh) (HbmCh / 2) + 4

//
// Channel 0 credit config done bit mask for cmi misc config register
//
#define CHANNEL0_CREDIT_CONFIG_DONE_BIT_MASK BIT2
//
// Channel 1 credit config done bit mask for cmi misc config register
//
#define CHANNEL1_CREDIT_CONFIG_DONE_BIT_MASK BIT3

//
// Polling bit location mask for Mesh2Mem CMI Misc register
//
const UINT32 PollBitLocationHbm[] = { CHANNEL0_CREDIT_CONFIG_DONE_BIT_MASK, 
                                   CHANNEL1_CREDIT_CONFIG_DONE_BIT_MASK};

STATIC const UINT32 CmiRdCplCreditChnlConfig1RegAddr[2] = {
  CMIRDCPLCREDITCHNLCONFIG_0_M2MEM_MAIN_REG,
  CMIRDCPLCREDITCHNLCONFIG_1_M2MEM_MAIN_REG
};

STATIC const UINT32 CmiRdCplCreditChnlConfig2RegAddr[2] = {
  CMIRDCPLCREDITCHNLCONFIG_2_0_M2MEM_MAIN_REG,
  CMIRDCPLCREDITCHNLCONFIG_2_1_M2MEM_MAIN_REG
};

STATIC const UINT32 CmiRspCreditChnlConfigRegAddr[2] = {
  CMIRSPCREDITCHNLCONFIG_0_M2MEM_MAIN_REG,
  CMIRSPCREDITCHNLCONFIG_1_M2MEM_MAIN_REG
};

/**
  Get Read Completion Credit Channel Config Value
  Returns value for SPR

  @param[in]   Socket          -   Socket number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval None
**/
VOID
GetCmiRdCplCreditChnlConfig1ValueSprHbm (
  IN  UINT8 Socket,
  OUT CMI_DEFAULT_REG_VALUE_HBM **ConfigValue,
  OUT UINT8                 *Size
  )
{
  *ConfigValue = CmiRdCplCreditChnlConfig1ValueA0Hbm;
  *Size = sizeof(CmiRdCplCreditChnlConfig1ValueA0Hbm)/sizeof(CMI_DEFAULT_REG_VALUE_HBM);
}

/**
  Get Read Completion Credit Channel Config Value
  Returns value for SPR

  @param[in]   Socket          -   Socket number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval None
**/
VOID
GetCmiRdCplCreditChnlConfig2ValueSprHbm (
  IN  UINT8 Socket,
  OUT CMI_DEFAULT_REG_VALUE_HBM **ConfigValue,
  OUT UINT8                 *Size
  )
{
  *ConfigValue = CmiRdCplCreditChnlConfig2ValueA0Hbm;
  *Size = sizeof(CmiRdCplCreditChnlConfig2ValueA0Hbm)/sizeof(CMI_DEFAULT_REG_VALUE_HBM);
}

/**
  Get CMI Rsp Credit Channel Config Value
  Returns value of SPR

  @param[in]   Socket          -   Socket number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval None
**/
VOID
GetCmiRspCreditChnlConfigValueSprHbm (
  IN  UINT8 Socket,
  OUT CMI_DEFAULT_REG_VALUE_HBM **ConfigValue,
  OUT UINT8                 *Size
  )
{
  *ConfigValue = CmiRspCreditChnlConfigValueA0Hbm;
  *Size = sizeof(CmiRspCreditChnlConfigValueA0Hbm)/sizeof(CMI_DEFAULT_REG_VALUE_HBM);
}

/**
  Get CMI Sysfeatures0 Value
  Returns value of SPR

  @param[in]   Socket          -   Socket number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval None
**/
VOID
GetCmiSysfeatures0SprHbm (
  IN  UINT8 Socket,
  OUT CMI_DEFAULT_REG_VALUE_HBM **ConfigValue,
  OUT UINT8                 *Size
  )
{
  *ConfigValue = Sysfeatures0A0Hbm;
  *Size = sizeof(Sysfeatures0A0Hbm)/sizeof(CMI_DEFAULT_REG_VALUE_HBM);
}

/**

  SetM2mCmiMiscConfigDoneChHbm.

  @param Socket       - Socket ID
  @param HbmCh        - Hbm Channel ID
  @param ConfigDone   - ConfigDone

  @retval N/A

**/
VOID
EFIAPI
SetM2mCmiMiscConfigDoneChHbm (
  IN UINT8    Socket,
  IN UINT8    HbmCh,
  IN BOOLEAN  ConfigDone
  )
{
  CMIMISCCONFIG_M2MEM_MAIN_STRUCT RegData;
  RegData.Data = UsraCsrRead (Socket, GET_HBM_MC_ID(HbmCh), CMIMISCCONFIG_M2MEM_MAIN_REG);
  if (HbmCh % 2 == 0) {
    RegData.Bits.chnl0_credit_config_done = (ConfigDone ? 1 : 0);
  } else if (HbmCh % 2 == 1) {
    RegData.Bits.chnl1_credit_config_done = (ConfigDone ? 1 : 0);
  }
  UsraCsrWrite (Socket, GET_HBM_MC_ID(HbmCh), CMIMISCCONFIG_M2MEM_MAIN_REG, RegData.Data);
}

/**

  Poll CMI Misc register for Init done. This function generates ASM polling logic
  if FCE bios enabled

  @param[in] Socket   - Socket number
  @param[in] HbmCh    - Hbm Channel Id

  @retval  N/A

**/
VOID
PollM2mCmiMiscCfgChInitHbm (
  IN UINT8    Socket,
  IN UINT8    HbmCh
  )
{
  UINT32 Offset = CMIMISCCONFIG_M2MEM_MAIN_REG;
  PSYSHOST Host = (PSYSHOST) GetSysHostPointer();

  //Set Initialized bit and wait for done
  if (GetUbiosOutputMode () == ASM_OUTPUT_ENABLE) {
    M2mPollRegisterOffsetForEmulation (Host, Socket, GET_HBM_MC_ID(HbmCh), Offset, PollBitLocationHbm[HbmCh % 2], 1);
  }
}

/**

  Get Mesh2Mem field Value for CmiMiscCfg - Channel IDannle's Credit Init Mask.
  @param Socket   - Socket ID
  @param HbmCh    - Hbm Channel Id

  @retval UINT8   - CREDIT_INITED_CH_0 = 1 means ch0 is inited, and so on.

**/
UINT8
EFIAPI
GetM2mCmiMiscCfgChCreditInitedMaskHbm (
  IN UINT8    Socket,
  IN UINT8    HbmCh
  )
{
  CMIMISCCONFIG_M2MEM_MAIN_STRUCT RegData;
  UINT8                           ChInited;

  ChInited = 0;
  RegData.Data  = UsraCsrRead (Socket, GET_HBM_MC_ID(HbmCh), CMIMISCCONFIG_M2MEM_MAIN_REG);
  if (RegData.Bits.chnl0_credits_initialized){
    ChInited |= CREDIT_INITED_CH_0;
  }
  if (RegData.Bits.chnl1_credits_initialized){
    ChInited |= CREDIT_INITED_CH_1;
  }

  return ChInited;
}

/**
  Build Target function to wait for Cmi misc channels to complete.

  @param[in]   Socket           Socket Number.
  @param[in]   HbmCh            Hbm Channel Id

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
WaitForM2mCmiMiscConfigChannelsCompleteTargetHbm (
  IN UINT8 Socket,
  IN UINT8 HbmCh
  )
{
  UINT32 Status = EFI_SUCCESS;
  UINT64 StartCount;
  UINT32 Channel0Initialized = 1;
  UINT32 Channel1Initialized = 1;
  UINT8  ChCreditInited = 0;

  if(GetEmulation() == SIMICS_FLAG) {
    PollM2mCmiMiscCfgChInitHbm (Socket, HbmCh);
    return EFI_SUCCESS;
  }

  StartCount = GetCountTsc ();
  do {
    if (GetDelayTsc (StartCount) > CATCHALL_TIMEOUT) {
      if (Channel0Initialized == 0) {
        Channel0Initialized = 1;  //Reset Variable for next loop
        Status = 1; //Indicate function failure
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n InitCmiCreditProgrammingHbm: M2MCmiMiscConfig Channel %d not set \n", HbmCh);
        RC_FATAL_ERROR (FALSE, ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);
        FatalError (ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);
      }
      if (Channel1Initialized == 0) {
        Channel1Initialized = 1; //Reset Variable for next loop
        Status = 1; //Indicate function failure
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n InitCmiCreditProgrammingHbm: M2MCmiMiscConfig Channel %d not set \n", HbmCh);
        RC_FATAL_ERROR (FALSE, ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);
        FatalError (ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);
      }
      break;
    }
    ChCreditInited = GetM2mCmiMiscCfgChCreditInitedMaskHbm (Socket, HbmCh);
    Channel0Initialized = ((ChCreditInited & CREDIT_INITED_CH_0) != 0);
    Channel1Initialized = ((ChCreditInited & CREDIT_INITED_CH_1) != 0);
  } while (!((Channel0Initialized == 1) && (Channel1Initialized == 1)));
  return Status;
} // WaitForCmiMiscConfigChannelsCompleteTarget

/**

  Program M2mem CMI registers for HBM

  @param[in] ProgramableValue[MAX_HBM_CH]    - Array containing type of value to program per channel.
  @param[in] Socket                      - Socket number

  @retval N/A

**/
VOID
EFIAPI
ProgramMesh2MemCmiRegistersHbm (
  IN UINT8 ProgramableValue[MAX_HBM_CH],
  IN UINT8 Socket
  )
{
  UINT8                 IndexVal                        = 0;
  UINT32                CmiRdCplCredit                  = 0;
  UINT32                CmiRspCredit                    = 0;
  UINT32                M2MemSysFeatures0               = 0;
  UINT32                RegData                         = 0;
  CMI_DEFAULT_REG_VALUE_HBM *CmiRdCplCreditChnlConfig1Value = NULL;
  CMI_DEFAULT_REG_VALUE_HBM *CmiRdCplCreditChnlConfig2Value = NULL;
  CMI_DEFAULT_REG_VALUE_HBM *CmiRspCreditChnlConfigValue    = NULL;
  CMI_DEFAULT_REG_VALUE_HBM *CmiSysfeatures0Value           = NULL;
  UINT8                 CmiRdCplCreditChnlConfig1Len    = 0;
  UINT8                 CmiRdCplCreditChnlConfig2Len    = 0;
  UINT8                 CmiRspCreditChnlConfigLen       = 0;
  UINT8                 CmiSysfeatures0Len              = 0;
  UINT8                 HbmCh;
  UINT8                 MaxChHbm                        = GetMaxChHbm ();

  GetCmiRdCplCreditChnlConfig1ValueSprHbm (Socket, &CmiRdCplCreditChnlConfig1Value, &CmiRdCplCreditChnlConfig1Len);
  GetCmiRdCplCreditChnlConfig2ValueSprHbm (Socket, &CmiRdCplCreditChnlConfig2Value, &CmiRdCplCreditChnlConfig2Len);
  GetCmiRspCreditChnlConfigValueSprHbm (Socket, &CmiRspCreditChnlConfigValue, &CmiRspCreditChnlConfigLen);
  GetCmiSysfeatures0SprHbm (Socket, &CmiSysfeatures0Value, &CmiSysfeatures0Len);

  for (HbmCh = 0; HbmCh < MaxChHbm; HbmCh++) {
    if (!IsHbmChannelEnabled (Socket, HbmCh)) {
      continue;
    }
    if (CmiRdCplCreditChnlConfig1Value != NULL) {
      CmiRdCplCredit = UsraCsrRead (Socket, GET_HBM_MC_ID(HbmCh), CmiRdCplCreditChnlConfig1RegAddr[HbmCh % 2]);
      for (IndexVal = 0; IndexVal < CmiRdCplCreditChnlConfig1Len; IndexVal++) {
        RegData = (CmiRdCplCredit >> CmiRdCplCreditChnlConfig1Value[IndexVal].BitStart) & CmiRdCplCreditChnlConfig1Value[IndexVal].AndVal;
        CmiRdCplCredit &= ~(CmiRdCplCreditChnlConfig1Value[IndexVal].AndVal << CmiRdCplCreditChnlConfig1Value[IndexVal].BitStart);
          switch (ProgramableValue[HbmCh]) {
            case CMI_1LM_VAL:
              RegData = CmiRdCplCreditChnlConfig1Value[IndexVal].OneLMVal;
              break;
            case CMI_XTILE_VAL:
              RegData = CmiRdCplCreditChnlConfig1Value[IndexVal].XtileVal;
              break;
            default:  //Do nothing for default...So that register defaults will be left alone
              break;
          }
          CmiRdCplCredit |=  (RegData << CmiRdCplCreditChnlConfig1Value[IndexVal].BitStart);
        }
      UsraCsrWrite (Socket, GET_HBM_MC_ID(HbmCh), CmiRdCplCreditChnlConfig1RegAddr[HbmCh % 2], CmiRdCplCredit);
    }
    if (CmiRdCplCreditChnlConfig2Value != NULL) {
      CmiRdCplCredit = UsraCsrRead (Socket, GET_HBM_MC_ID(HbmCh), CmiRdCplCreditChnlConfig2RegAddr[HbmCh % 2]);
      for (IndexVal = 0; IndexVal < CmiRdCplCreditChnlConfig2Len; IndexVal++) {
        RegData = (CmiRdCplCredit >> CmiRdCplCreditChnlConfig2Value[IndexVal].BitStart) & CmiRdCplCreditChnlConfig2Value[IndexVal].AndVal;
        CmiRdCplCredit &= ~(CmiRdCplCreditChnlConfig2Value[IndexVal].AndVal << CmiRdCplCreditChnlConfig2Value[IndexVal].BitStart);
        switch (ProgramableValue[HbmCh]) {
          case CMI_1LM_VAL:
            RegData = CmiRdCplCreditChnlConfig2Value[IndexVal].OneLMVal;
            break;
          case CMI_XTILE_VAL:
            RegData = CmiRdCplCreditChnlConfig2Value[IndexVal].XtileVal;
            break;
          default:  //Do nothing for default...So that register defaults will be left alone
            break;
        }
        CmiRdCplCredit |=  (RegData << CmiRdCplCreditChnlConfig2Value[IndexVal].BitStart);
      }
      UsraCsrWrite (Socket, GET_HBM_MC_ID(HbmCh), CmiRdCplCreditChnlConfig2RegAddr[HbmCh % 2], CmiRdCplCredit);
    }
    if (CmiRspCreditChnlConfigValue != NULL) {
	  CmiRspCredit = UsraCsrRead (Socket, GET_HBM_MC_ID(HbmCh), CmiRspCreditChnlConfigRegAddr[HbmCh % 2]);
      for (IndexVal = 0; IndexVal < CmiRspCreditChnlConfigLen; IndexVal++) {
        RegData = (CmiRspCredit >> CmiRspCreditChnlConfigValue[IndexVal].BitStart) & CmiRspCreditChnlConfigValue[IndexVal].AndVal;
        CmiRspCredit &= ~(CmiRspCreditChnlConfigValue[IndexVal].AndVal << CmiRspCreditChnlConfigValue[IndexVal].BitStart);
        switch (ProgramableValue[HbmCh]) {
          case CMI_1LM_VAL:
            RegData = CmiRspCreditChnlConfigValue[IndexVal].OneLMVal;
            break;
          case CMI_XTILE_VAL:
            RegData = CmiRspCreditChnlConfigValue[IndexVal].XtileVal;
            break;
          default:  //Do nothing for default...So that register defaults will be left alone
            break;
        }
        CmiRspCredit |= (RegData << CmiRspCreditChnlConfigValue[IndexVal].BitStart);
      }
	  UsraCsrWrite (Socket, GET_HBM_MC_ID(HbmCh), CmiRspCreditChnlConfigRegAddr[HbmCh % 2], CmiRspCredit);
	}
    if (CmiSysfeatures0Value != NULL) {
      M2MemSysFeatures0 = UsraCsrRead (Socket, GET_HBM_MC_ID(HbmCh), SYSFEATURES0_M2MEM_MAIN_REG);
      for (IndexVal = 0; IndexVal < CmiSysfeatures0Len; IndexVal++) {
        RegData = (M2MemSysFeatures0 >> CmiSysfeatures0Value[IndexVal].BitStart) & CmiSysfeatures0Value[IndexVal].AndVal;
        M2MemSysFeatures0 &= ~(CmiSysfeatures0Value[IndexVal].AndVal << CmiSysfeatures0Value[IndexVal].BitStart);
        switch (ProgramableValue[HbmCh]) {
          case CMI_1LM_VAL:
            RegData = CmiSysfeatures0Value[IndexVal].OneLMVal;
            break;
          case CMI_XTILE_VAL:
            RegData = CmiSysfeatures0Value[IndexVal].XtileVal;
            break;
          default:  //Do nothing for default...So that register defaults will be left alone
            break;
        }
        M2MemSysFeatures0 |= (RegData << CmiSysfeatures0Value[IndexVal].BitStart);
      }
      UsraCsrWrite (Socket, GET_HBM_MC_ID(HbmCh), SYSFEATURES0_M2MEM_MAIN_REG, M2MemSysFeatures0);
    }
  }
}
