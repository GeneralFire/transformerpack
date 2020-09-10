/** @file
  Mem Mc CMI Credit information internal header

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019-2020 Intel Corporation. <BR>

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

#ifndef _MEM_MC_CMI_CREDITS_H_
#define _MEM_MC_CMI_CREDITS_H_

/**
  Get MC CMI Credit Config N0 Value

  @param[in]   Socket          -   Socket number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
EFIAPI
GetMcCmiCreditCfgN0Value (
  IN  UINT8 Socket,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  );

/**
  Get MC CMI Credit Config N0 Value per Channel

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
EFIAPI
GetMcCmiCreditCfgN0ValueChannel (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  );


/**
  Get MC CMI Credit Config N1 Value

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
  );

/**
  Get MC CMI Credit Config N1 Value per Channel

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcCmiCreditCfgN1ValueChannel (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  );

/**
  Get MC CMI Request Completion Config Value0

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
  );

/**
  Get MC CMI Request Completion Config Value0 per Channel

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcCmiReqCpcfgConfigValue0Channel (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  );

/**
  Get MC CMI Request Completion Config Value1

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
  );

/**
  Get MC CMI Request Completion Config Value1 per Channel

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcCmiReqCpcfgConfigValue1Channel (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  );

/**
  Get MC MISC CMI credit config Value

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
  );

/**
  Get MC MISC CMI credit config Value per Channel

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
GetMcMiscCmiCreditConfigValueChannel (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  );

/**
  Modify Mc CMI credits registers, chip specific

  @param[   out] McCmiCreditCfgN0Value        - McCmiCreditCfgN0 values pointer
  @param[   out] McCmiCreditCfgN1Value        - McCmiCreditCfgN1 values pointer
  @param[   out] McCmiReqCpcfgConfigValue0    - McCmiReqCpcfgConfig0 values pointer
  @param[   out] McCmiReqCpcfgConfigValue1    - McCmiReqCpcfgConfig1 values pointer
  @param[   out] McMiscCmiCreditConfigValue   - McMiscCmiCreditConfig values pointer
  @param[   out] McCmiCreditCfgN0Len          - McCmiCreditCfgN0 length pointer
  @param[   out] McCmiCreditCfgN1Len          - McCmiCreditCfgN1 length pointer
  @param[   out] McCmiReqCpcfgConfigValue0Len - McCmiReqCpcfgConfig0 length pointer
  @param[   out] McCmiReqCpcfgConfigValue1Len - McCmiReqCpcfgConfig1 length pointer
  @param[   out] McMiscCmiCreditConfigLen     - McMiscCmiCreditConfig length pointer

  @retval EFI_UNSUPPORTED if the table is not supported
          EFI_SUCCESS otherwise
**/
EFI_STATUS
EFIAPI
ModifyMcCmiCreditProgrammingChip (
     OUT CMI_DEFAULT_REG_VALUE **McCmiCreditCfgN0Value,
     OUT CMI_DEFAULT_REG_VALUE **McCmiCreditCfgN1Value,
     OUT CMI_DEFAULT_REG_VALUE **McCmiReqCpcfgConfigValue0,
     OUT CMI_DEFAULT_REG_VALUE **McCmiReqCpcfgConfigValue1,
     OUT CMI_DEFAULT_REG_VALUE **McMiscCmiCreditConfigValue,
     OUT UINT8                 *McCmiCreditCfgN0Len,
     OUT UINT8                 *McCmiCreditCfgN1Len,
     OUT UINT8                 *McCmiReqCpcfgConfigValue0Len,
     OUT UINT8                 *McCmiReqCpcfgConfigValue1Len,
     OUT UINT8                 *McMiscCmiCreditConfigLen
  );

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
  );

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
  );

#endif //MESH_2_MEM_CMI_CREDITS_H_
