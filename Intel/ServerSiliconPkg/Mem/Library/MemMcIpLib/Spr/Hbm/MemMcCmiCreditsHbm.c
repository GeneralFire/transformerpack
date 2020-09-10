/** @file
  Cmi credit values programming for MemMc on HBM.

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
#include <SysHostChip.h>
#include <MemCommon.h>
#include <CmiPerformance.h>
#include "MemMcCmiPerformanceDataHbm.h"
#include <Library/SysHostPointerLib.h>
#include <Library/MemRcLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemAccessLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemMap2SgxInterface.h>
#include <Library/MemSmeeIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/HbmCoreLib.h>
#include <Library/KtiApi.h>
#include "Include/MemMcRegs.h"
#include "Include/MemMcIpLibInternal.h"

STATIC const UINT32 McMiscCmiCreditConfigRegAddrHbm[2] = {
  MCMISC_CMI_CREDIT_CONFIG_0_MC_GLOBAL_REG,
  MCMISC_CMI_CREDIT_CONFIG_1_MC_GLOBAL_REG,
};

/**
  Get MC CMI Credit Config N0 Value
  Returns value of SPR

  @param[in]   Socket          -   Socket number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
EFIAPI
GetMcCmiCreditCfgN0ValueHbm (
  IN  UINT8 Socket,
  OUT CMI_DEFAULT_REG_VALUE_HBM **ConfigValue,
  OUT UINT8                 *Size
  )
{
  *ConfigValue = McCmiCreditCfgN0ValueA0Hbm;
  *Size = sizeof(McCmiCreditCfgN0ValueA0Hbm)/sizeof(CMI_DEFAULT_REG_VALUE_HBM);
  return EFI_SUCCESS;
}

/**
  Get MC CMI Credit Config N1 Value
  Returns value of SPR

  @param[in]   Socket          -   Socket number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcCmiCreditCfgN1ValueHbm (
  IN  UINT8 Socket,
  OUT CMI_DEFAULT_REG_VALUE_HBM **ConfigValue,
  OUT UINT8                 *Size
  )
{
  *ConfigValue = McCmiCreditCfgN1ValueA0Hbm;
  *Size = sizeof(McCmiCreditCfgN1ValueA0Hbm)/sizeof(CMI_DEFAULT_REG_VALUE_HBM);
  return EFI_SUCCESS;
}

/**
  Get MC CMI Request Completion Config Value0
  Returns value of SPR

  @param[in]   Socket          -   Socket number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcCmiReqCpcfgConfigValue0Hbm (
  IN  UINT8 Socket,
  OUT CMI_DEFAULT_REG_VALUE_HBM **ConfigValue,
  OUT UINT8                 *Size
  )
{
  *ConfigValue = McCmiReqCpcfgConfigValue0A0Hbm;
  *Size = sizeof(McCmiReqCpcfgConfigValue0A0Hbm)/sizeof(CMI_DEFAULT_REG_VALUE_HBM);
  return EFI_SUCCESS;
}

/**
  Get MC CMI Request Completion Config Value1
  Returns value of SPR

  @param[in]   Socket          -   Socket number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcCmiReqCpcfgConfigValue1Hbm (
  IN  UINT8 Socket,
  OUT CMI_DEFAULT_REG_VALUE_HBM **ConfigValue,
  OUT UINT8                 *Size
  )
{
  *ConfigValue = McCmiReqCpcfgConfigValue1A0Hbm;
  *Size = sizeof(McCmiReqCpcfgConfigValue1A0Hbm)/sizeof(CMI_DEFAULT_REG_VALUE_HBM);
  return EFI_SUCCESS;
}

/**
  Get MC MISC CMI credit config Value
  Returns value of SPR

  @param[in]   Socket          -   Socket number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcMiscCmiCreditConfigValueHbm (
  IN  UINT8 Socket,
  OUT CMI_DEFAULT_REG_VALUE_HBM **ConfigValue,
  OUT UINT8                 *Size
  )
{
  *ConfigValue = McMiscCmiCreditConfigValueA0Hbm;
  *Size = sizeof(McMiscCmiCreditConfigValueA0Hbm)/sizeof(CMI_DEFAULT_REG_VALUE_HBM);
  return EFI_SUCCESS;
}

/**
  Get McDp CMI Qdepth credit config Value
  Returns value of SPR

  @param[in]   Socket          -   Socket number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcDpCmiQdepthCreditConfigValueHbm (
  IN  UINT8 Socket,
  OUT CMI_DEFAULT_REG_VALUE_HBM **ConfigValue,
  OUT UINT8                 *Size
  )
{
  *ConfigValue = McDpCmiQdepthCreditConfigValueA0Hbm;
  *Size = sizeof(McDpCmiQdepthCreditConfigValueA0Hbm)/sizeof(CMI_DEFAULT_REG_VALUE_HBM);
  return EFI_SUCCESS;
}

/**

  SetMcCmiConfigDoneChHbm.

  @param Socket       - Socket ID
  @param HbmCh        - Hbm Channel ID

  @retval N/A

**/
VOID
EFIAPI
SetMcCmiConfigDoneChHbm (
  IN UINT8    Socket,
  IN UINT8    HbmCh
  )
{
  MCMISC_CMI_CREDIT_CONFIG_0_MC_GLOBAL_STRUCT RegData1;
  MCCMIREQCPCFG_N1_MC_2LM_STRUCT RegData2;

  RegData1.Data = ChRegisterRead (MemTechHbm, Socket, HbmCh, McMiscCmiCreditConfigRegAddrHbm[HbmCh % 2]);
  RegData1.Bits.credit_init_done = 1;
  ChRegisterWrite (MemTechHbm, Socket, HbmCh, McMiscCmiCreditConfigRegAddrHbm[HbmCh % 2], RegData1.Data);

  RegData2.Data = ChRegisterRead (MemTechHbm, Socket, HbmCh, MCCMIREQCPCFG_N1_MC_2LM_REG);
  RegData2.Bits.req_credit_cfg_done = 1;
  ChRegisterWrite (MemTechHbm, Socket, HbmCh, MCCMIREQCPCFG_N1_MC_2LM_REG, RegData2.Data);
}

/**

  Generate ASM polling loop for given register offset. Takes mask and logic as input
  for polling.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket Id
  @param[in] BoxInst - Box Instance, 0 based
  @param[in] Offset  - Register offset; values come from the auto generated header file
  @param[in] Mask    - Bit mask of bits we are polling
  @param[in] Logic   - Polling for 0 or 1

  @retval None

**/
VOID
PollRegisterOffsetForEmulationHbm (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    BoxInst,
  IN UINT32   Offset,
  IN UINT32   Mask,
  IN UINT8    Logic
  )
{
  UINT8      *RegAddr;
  UINT32     LabelNumber;

  RegAddr = (UINT8 *) GetChRegisterAddress (MemTechHbm, Socket, BoxInst, Offset);
  if (GetUbiosLoopbackLabelNumber (&LabelNumber) == EFI_SUCCESS) {
    LabelPrint (LabelNumber);
    SetUbiosLoopbackLabelNumber (LabelNumber + 1);
    RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "mov eax, DWORD PTR ds:[0%8xh]\n", RegAddr);
    if (Logic == 0) {
      RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "not eax\n");
    }
    RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "and eax, 0%08xh\n", Mask);
    RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "cmp eax, 0%08xh\n", Mask);
    if (CheckCurrentReset (POST_RESET_POWERGOOD))  {
      RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "jnz LABEL_c_mbox%d\n", LabelNumber);
    } else {
      RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "jnz LABEL_w_mbox%d\n", LabelNumber);
    }
  }
}


/**
  Build Target function to wait for Cmi misc channels to complete.

  @param[in]   Socket                 Socket Number.
  @param[in]   HbmCh                  HbmCh

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.

**/
EFI_STATUS
EFIAPI
WaitForMcMiscCmiCreditConfigCompleteTargetHbm (
  IN UINT8 Socket,
  IN UINT8 HbmCh
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT64 StartCount;
  MCMISC_CMI_CREDIT_CONFIG_0_MC_GLOBAL_STRUCT McMiscCmiCreditConfig;
  PSYSHOST Host = (PSYSHOST) GetSysHostPointer();

  if(GetEmulation () == SIMICS_FLAG) {
    if (GetUbiosOutputMode () == ASM_OUTPUT_ENABLE) {
      PollRegisterOffsetForEmulationHbm (Host, Socket, HbmCh, McMiscCmiCreditConfigRegAddrHbm[HbmCh % 2], BIT29 | BIT30, 1);
    }
    return Status;
  }

  StartCount = GetCountTsc ();
  do {
    if (GetDelayTsc (StartCount) > CATCHALL_TIMEOUT) {
      Status = 1; //Indicate function failure
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n InitCmiCreditProgrammingHbm: McMiscCmiCreditConfig[%d] initialize failed \n", HbmCh);
      RC_FATAL_ERROR (FALSE, ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);
      FatalError (ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);
      break;
    }
    McMiscCmiCreditConfig.Data = ChRegisterRead (MemTechHbm, Socket, HbmCh, McMiscCmiCreditConfigRegAddrHbm[HbmCh % 2]);
  } while (!((McMiscCmiCreditConfig.Bits.rsp_ism_credits_initialized == 1) &&
             (McMiscCmiCreditConfig.Bits.req_ism_credits_initialized == 1)));
  return Status;
}

/**
  Build Target function to wait for Mc Cmi Req Cpcfg to complete.

  @param[in]   Socket           Socket Number.
  @param[in]   HbmCh            Hbm Channel Id.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.

**/
EFI_STATUS
EFIAPI
WaitForMcCmiReqCpcfgCompleteTargetHbm (
  IN UINT8 Socket,
  IN UINT8 HbmCh
  )
{
  PSYSHOST Host;
  UINT64 StartCount;
  MCCMIREQCPCFG_N1_MC_2LM_STRUCT McCmiReqCpcfg;
  UINT32 PollRegMask = BIT14;

  Host = (PSYSHOST) GetSysHostPointer();
  if (GetEmulation () == SIMICS_FLAG) {
    if (GetUbiosOutputMode () == ASM_OUTPUT_ENABLE) {
      PollRegisterOffsetForEmulationHbm (Host, Socket, HbmCh, MCCMIREQCPCFG_N1_MC_2LM_REG, PollRegMask, 1);
    }
    return EFI_SUCCESS;
  }

  StartCount = GetCountTsc ();
  do {
    if (GetDelayTsc (StartCount) > CATCHALL_TIMEOUT) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n InitCmiCreditProgrammingHbm: McCmiReqCpcfg initialize failed \n");
      RC_FATAL_ERROR (FALSE, ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);
      FatalError (ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);
      return EFI_TIMEOUT;
    }

    McCmiReqCpcfg.Data = ChRegisterRead (MemTechHbm, Socket, HbmCh, MCCMIREQCPCFG_N1_MC_2LM_REG);
  } while (McCmiReqCpcfg.Bits.req_credits_initialized == 0);
  return EFI_SUCCESS;
}


/**
  Program McCmiCreditCfg CMI Credit Registers for Hbm

  @param[in] ProgramableValue  - Programmable CMI credit value
  @param[in] Socket            - Socket number
  @param[in] HbmCh             - Hbm Channel number

  @retval N/A
**/
VOID
EFIAPI
ProgramMcCmiCreditCfgRegistersHbm (
  IN UINT8 ProgramableValue[MAX_HBM_CH],
  IN UINT8 Socket,
  IN UINT8 HbmCh
  )
{
  UINT8                                         IndexVal;
  UINT32                                        RegData;
  MEM_TECH_TYPE                                 MemTechType                  = MemTechHbm;
  CMI_DEFAULT_REG_VALUE_HBM                         *McCmiCreditCfgN0Value       = NULL;
  CMI_DEFAULT_REG_VALUE_HBM                         *McCmiCreditCfgN1Value       = NULL;
  UINT8                                         McCmiCreditCfgN0Len          = 0;
  UINT8                                         McCmiCreditCfgN1Len          = 0;
  MCSTRUCTCMICREDITCFG_N0_MC_2LM_STRUCT         McCmiCreditCfgN0;
  MCSTRUCTCMICREDITCFG_N1_MC_2LM_STRUCT         McCmiCreditCfgN1;

  GetMcCmiCreditCfgN0ValueHbm (Socket, &McCmiCreditCfgN0Value ,&McCmiCreditCfgN0Len);
  GetMcCmiCreditCfgN1ValueHbm (Socket, &McCmiCreditCfgN1Value ,&McCmiCreditCfgN1Len);

  if (McCmiCreditCfgN0Value != NULL) {
    McCmiCreditCfgN0.Data = ChRegisterRead (MemTechType,  Socket, HbmCh, MCSTRUCTCMICREDITCFG_N0_MC_2LM_REG);
    for (IndexVal = 0; IndexVal < McCmiCreditCfgN0Len; IndexVal++) {
      RegData = (McCmiCreditCfgN0.Data >> McCmiCreditCfgN0Value[IndexVal].BitStart) & McCmiCreditCfgN0Value[IndexVal].AndVal;
      McCmiCreditCfgN0.Data &= ~(McCmiCreditCfgN0Value[IndexVal].AndVal << McCmiCreditCfgN0Value[IndexVal].BitStart);
      switch (ProgramableValue[HbmCh]) {
        case CMI_1LM_VAL:
          RegData = McCmiCreditCfgN0Value[IndexVal].OneLMVal;
          break;
	    case CMI_XTILE_VAL:
          RegData = McCmiCreditCfgN0Value[IndexVal].XtileVal;
          break;
        default:
          break;
      }
      McCmiCreditCfgN0.Data |= (RegData << McCmiCreditCfgN0Value[IndexVal].BitStart);
    }
    ChRegisterWrite (MemTechType,  Socket, HbmCh, MCSTRUCTCMICREDITCFG_N0_MC_2LM_REG, McCmiCreditCfgN0.Data);
  }
  if (McCmiCreditCfgN1Value != NULL) {
    McCmiCreditCfgN1.Data = ChRegisterRead (MemTechType,  Socket, HbmCh, MCSTRUCTCMICREDITCFG_N1_MC_2LM_REG);
    for (IndexVal = 0; IndexVal < McCmiCreditCfgN1Len; IndexVal++) {
      RegData = (McCmiCreditCfgN1.Data >> McCmiCreditCfgN1Value[IndexVal].BitStart) & McCmiCreditCfgN1Value[IndexVal].AndVal;
      McCmiCreditCfgN1.Data &= ~(McCmiCreditCfgN1Value[IndexVal].AndVal << McCmiCreditCfgN1Value[IndexVal].BitStart);
      switch (ProgramableValue[HbmCh]) {
        case CMI_1LM_VAL:
          RegData = McCmiCreditCfgN1Value[IndexVal].OneLMVal;
          break;
  	  case CMI_XTILE_VAL:
          RegData = McCmiCreditCfgN1Value[IndexVal].XtileVal;
          break;
        default:
          break;
      }
      McCmiCreditCfgN1.Data |= (RegData << McCmiCreditCfgN1Value[IndexVal].BitStart);
    }
    ChRegisterWrite (MemTechType,  Socket, HbmCh, MCSTRUCTCMICREDITCFG_N1_MC_2LM_REG, McCmiCreditCfgN1.Data);
  }
}

/**
  Program McCmiReqCpcfg CMI Credit Registers for Hbm

  @param[in] ProgramableValue  - Programmable CMI credit value
  @param[in] Socket            - Socket number
  @param[in] HbmCh             - Hbm Channel number

  @retval N/A
**/
VOID
EFIAPI
ProgramMcCmiReqCpcfgRegistersHbm (
  IN UINT8 ProgramableValue[MAX_HBM_CH],
  IN UINT8 Socket,
  IN UINT8 HbmCh
  )
{
  UINT8                                         IndexVal;
  UINT32                                        RegData;
  MEM_TECH_TYPE                                 MemTechType                  = MemTechHbm;
  CMI_DEFAULT_REG_VALUE_HBM                         *McCmiReqCpcfgConfigValue0   = NULL;
  CMI_DEFAULT_REG_VALUE_HBM                         *McCmiReqCpcfgConfigValue1   = NULL;
  UINT8                                         McCmiReqCpcfgConfigValue0Len = 0;
  UINT8                                         McCmiReqCpcfgConfigValue1Len = 0;
  MCCMIREQCPCFG_N0_MC_2LM_STRUCT                McCmiReqCpcfgN0;
  MCCMIREQCPCFG_N1_MC_2LM_STRUCT                McCmiReqCpcfgN1;

  GetMcCmiReqCpcfgConfigValue0Hbm (Socket, &McCmiReqCpcfgConfigValue0 ,&McCmiReqCpcfgConfigValue0Len);
  GetMcCmiReqCpcfgConfigValue1Hbm (Socket, &McCmiReqCpcfgConfigValue1 ,&McCmiReqCpcfgConfigValue1Len);

  if (McCmiReqCpcfgConfigValue0 != NULL) {
    McCmiReqCpcfgN0.Data = ChRegisterRead (MemTechType, Socket, HbmCh, MCCMIREQCPCFG_N0_MC_2LM_REG);
    for (IndexVal = 0; IndexVal < McCmiReqCpcfgConfigValue0Len; IndexVal++) {
      RegData = (McCmiReqCpcfgN0.Data >> McCmiReqCpcfgConfigValue0[IndexVal].BitStart) & McCmiReqCpcfgConfigValue0[IndexVal].AndVal;
      McCmiReqCpcfgN0.Data &= ~(McCmiReqCpcfgConfigValue0[IndexVal].AndVal << McCmiReqCpcfgConfigValue0[IndexVal].BitStart);
      switch (ProgramableValue[HbmCh]) {
        case CMI_1LM_VAL:
          RegData = McCmiReqCpcfgConfigValue0[IndexVal].OneLMVal;
          break;
        case CMI_XTILE_VAL:
          RegData = McCmiReqCpcfgConfigValue0[IndexVal].XtileVal;
          break;
        default:
          break;
      }
      McCmiReqCpcfgN0.Data |= (RegData << McCmiReqCpcfgConfigValue0[IndexVal].BitStart);
    }
    ChRegisterWrite (MemTechType, Socket, HbmCh, MCCMIREQCPCFG_N0_MC_2LM_REG, McCmiReqCpcfgN0.Data);
  }
  if (McCmiReqCpcfgConfigValue1 != NULL) {
    McCmiReqCpcfgN1.Data = ChRegisterRead (MemTechType,  Socket, HbmCh, MCCMIREQCPCFG_N1_MC_2LM_REG);
    for (IndexVal = 0; IndexVal < McCmiReqCpcfgConfigValue1Len; IndexVal++) {
      RegData = (McCmiReqCpcfgN1.Data >> McCmiReqCpcfgConfigValue1[IndexVal].BitStart) & McCmiReqCpcfgConfigValue1[IndexVal].AndVal;
      McCmiReqCpcfgN1.Data &= ~(McCmiReqCpcfgConfigValue1[IndexVal].AndVal << McCmiReqCpcfgConfigValue1[IndexVal].BitStart);
      switch (ProgramableValue[HbmCh]) {
        case CMI_1LM_VAL:
          RegData = McCmiReqCpcfgConfigValue1[IndexVal].OneLMVal;
          break;
		case CMI_XTILE_VAL:
          RegData = McCmiReqCpcfgConfigValue1[IndexVal].XtileVal;
          break;
        default:
          break;
      }
      McCmiReqCpcfgN1.Data |= (RegData << McCmiReqCpcfgConfigValue1[IndexVal].BitStart);
    }
    ChRegisterWrite (MemTechType,  Socket, HbmCh, MCCMIREQCPCFG_N1_MC_2LM_REG, McCmiReqCpcfgN1.Data); 
  }
}

/**
  Program McDp CMI Credit Registers for Hbm

  @param[in] ProgramableValue  - Programmable CMI credit value
  @param[in] Socket            - Socket number
  @param[in] HbmCh             - Hbm Channel number

  @retval N/A
**/
VOID
EFIAPI
ProgramMcMiscCmiCreditConfigRegistersHbm (
  IN UINT8 ProgramableValue[MAX_HBM_CH],
  IN UINT8 Socket,
  IN UINT8 HbmCh
  )
{
  UINT8                                         IndexVal;
  UINT32                                        RegData;
  MEM_TECH_TYPE                                 MemTechType                  = MemTechHbm;
  CMI_DEFAULT_REG_VALUE_HBM                     *McMiscCmiCreditConfigValue  = NULL;
  UINT8                                         McMiscCmiCreditConfigLen     = 0;
  MCMISC_CMI_CREDIT_CONFIG_0_MC_GLOBAL_STRUCT   McMiscCmiCreditConfig;
                           
  GetMcMiscCmiCreditConfigValueHbm (Socket, &McMiscCmiCreditConfigValue ,&McMiscCmiCreditConfigLen);

  if (McMiscCmiCreditConfigValue != NULL) {
    McMiscCmiCreditConfig.Data = ChRegisterRead (MemTechType,  Socket, HbmCh, McMiscCmiCreditConfigRegAddrHbm[HbmCh % 2]);
    for (IndexVal = 0; IndexVal < McMiscCmiCreditConfigLen; IndexVal++) {
      RegData = (McMiscCmiCreditConfig.Data >> McMiscCmiCreditConfigValue[IndexVal].BitStart) & McMiscCmiCreditConfigValue[IndexVal].AndVal;
      McMiscCmiCreditConfig.Data &= ~(McMiscCmiCreditConfigValue[IndexVal].AndVal << McMiscCmiCreditConfigValue[IndexVal].BitStart);
      switch (ProgramableValue[HbmCh]) {
        case CMI_1LM_VAL:
          RegData = McMiscCmiCreditConfigValue[IndexVal].OneLMVal;
          break;
		case CMI_XTILE_VAL:
          RegData = McMiscCmiCreditConfigValue[IndexVal].XtileVal;
          break;
        default:
          break;
      }
      McMiscCmiCreditConfig.Data |= (RegData << McMiscCmiCreditConfigValue[IndexVal].BitStart);
    }
    ChRegisterWrite (MemTechType,  Socket, HbmCh, McMiscCmiCreditConfigRegAddrHbm[HbmCh % 2], McMiscCmiCreditConfig.Data);
  }
}

/**
  Program McDp CMI Credit Registers for Hbm

  @param[in] ProgramableValue  - Programmable CMI credit value
  @param[in] Socket            - Socket number
  @param[in] HbmCh             - Hbm Channel number

  @retval N/A
**/
VOID
EFIAPI
ProgramMcDpCmiRegistersHbm (
  IN UINT8 ProgramableValue[MAX_HBM_CH],
  IN UINT8 Socket,
  IN UINT8 HbmCh
  )
{
  UINT8                                         IndexVal;
  UINT32                                        RegData;
  MEM_TECH_TYPE                                 MemTechType                     = MemTechHbm;
  CMI_DEFAULT_REG_VALUE_HBM                     *McDpCmiQdepthCreditConfigValue = NULL;
  UINT8                                         McDpCmiQdepthCreditConfigLen    = 0;
  MCDP_CMI_QDEPTH_CFG_MCDDC_DP_STRUCT           McDpCmiQdepthCreditConfig;

  GetMcDpCmiQdepthCreditConfigValueHbm (Socket, &McDpCmiQdepthCreditConfigValue ,&McDpCmiQdepthCreditConfigLen);

  if (McDpCmiQdepthCreditConfigValue != NULL) {
    McDpCmiQdepthCreditConfig.Data = ChRegisterRead (MemTechType, Socket, HbmCh, MCDP_CMI_QDEPTH_CFG_MCDDC_DP_REG);
    for (IndexVal = 0; IndexVal < McDpCmiQdepthCreditConfigLen; IndexVal++) {
      RegData = (McDpCmiQdepthCreditConfig.Data >> McDpCmiQdepthCreditConfigValue[IndexVal].BitStart) & McDpCmiQdepthCreditConfigValue[IndexVal].AndVal;
      McDpCmiQdepthCreditConfig.Data &= ~(McDpCmiQdepthCreditConfigValue[IndexVal].AndVal << McDpCmiQdepthCreditConfigValue[IndexVal].BitStart);
      switch (ProgramableValue[HbmCh]) {
        case CMI_1LM_VAL:
          RegData = McDpCmiQdepthCreditConfigValue[IndexVal].OneLMVal;
          break;
        case CMI_XTILE_VAL:
          RegData = McDpCmiQdepthCreditConfigValue[IndexVal].XtileVal;
          break;
        default:
          break;
      }
      McDpCmiQdepthCreditConfig.Data |= (RegData << McDpCmiQdepthCreditConfigValue[IndexVal].BitStart);
    }
    ChRegisterWrite (MemTechType, Socket, HbmCh, MCDP_CMI_QDEPTH_CFG_MCDDC_DP_REG, McDpCmiQdepthCreditConfig.Data);
  }
}

/**

  Program Mem Mc CMI registers for Hbm

  @param[in] ProgramableValue[MAX_HBM_CH]    - Array containing type of value to program per channel (From 0 to Max Channel in Socket)
  @param[in] Socket                          - Socket number

  @retVal   1 CMI Registers initialized
            0 CMI Registers not initialized
**/
UINT32
EFIAPI
ProgramMemMcCmiRegistersHbm (
  IN UINT8 ProgramableValue[MAX_HBM_CH],
  IN UINT8 Socket
  )
{
  UINT32                   Status                       = 0;
  UINT8                    HbmCh;
  UINT8                    MaxChHbm                     = GetMaxChHbm ();

  for (HbmCh = 0; HbmCh < MaxChHbm; HbmCh++) {
    if (!IsHbmChannelEnabled (Socket, HbmCh)) {
      continue;
    }
    ProgramMcCmiReqCpcfgRegistersHbm (ProgramableValue, Socket, HbmCh);
    ProgramMcCmiCreditCfgRegistersHbm (ProgramableValue, Socket, HbmCh);
    ProgramMcMiscCmiCreditConfigRegistersHbm (ProgramableValue, Socket, HbmCh);
    ProgramMcDpCmiRegistersHbm (ProgramableValue, Socket, HbmCh);
  }
  return Status;
}
