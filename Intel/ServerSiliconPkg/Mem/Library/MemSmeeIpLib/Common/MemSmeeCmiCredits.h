/** @file
  Mem SMEE CMI Credit information internal header

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

#ifndef _MEM_SMEE_CMI_CREDITS_H_
#define _MEM_SMEE_CMI_CREDITS_H_

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
  );

/**
  Get MC TME CMI Qdepth Value0
  Returns value for SPR

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval None
**/
VOID
GetMcTmeCmiQdepthValue0ChannelSpr (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  );

/**
  Get MC TME CMI Qdepth Value1
  Returns value for SPR

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval None
**/
VOID
GetMcTmeCmiQdepthValue1ChannelSpr (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  );

/**
  Get MC TME Configuration structures

  @param[in    ] Socket              - Socket number
  @param[in    ] Channel             - Channel number
  @param[   out] **McTmeCmiCfgValue  - Pointer to Register Configuration value
  @param[   out] *McTmeCmiCfgLen     - Pointer to Structure Size
  @param[   out] **McTmeRdbCfgValue  - Pointer to Register Configuration value
  @param[   out] *McTmeRdbCfgLen     - Pointer to Structure Size
  @param[   out] **McTmeRdbCfgValue1 - Pointer to Register Configuration value
  @param[   out] *McTmeRdbCfgValue1  - Pointer to Structure Size
  @param[   out] **McTmeCfgValue     - Pointer to Register Configuration value
  @param[   out] *McTmeCfgLen        - Pointer to Structure Size

  @retval N/A
**/
VOID
EFIAPI
GetMcTmeStructures (
  IN     UINT8                 Socket,
  IN     UINT8                 Channel,
     OUT CMI_DEFAULT_REG_VALUE **McTmeCmiCfgValue,
     OUT UINT8                 *McTmeCmiCfgLen,
     OUT CMI_DEFAULT_REG_VALUE **McTmeRdbCfgValue,
     OUT UINT8                 *McTmeRdbCfgLen,
     OUT CMI_DEFAULT_REG_VALUE **McTmeRdbCfgValue1,
     OUT UINT8                 *McTmeRdbCfgValue1Len,
     OUT CMI_DEFAULT_REG_VALUE **McTmeCfgValue,
     OUT UINT8                 *McTmeCfgLen
  );

/**
  Get TME structure values based on programable parameter

  @param[in] ProgramableValue    - Programmable CMI credit value
  @param[in] McTmeStructureValue - Structure containing the fields for the programable value
  @param[in] *RegData            - Register data to be returned
  
  @retval N/A
**/
VOID
EFIAPI
GetMcTmeStructureField (
  IN     UINT8                 ProgramableValue,
  IN     CMI_DEFAULT_REG_VALUE McTmeStructureValue,
     OUT UINT32                *RegData
  );

/**

  Print McTme CMI SpreadSheet Data Version

  @param[in] None

  @retVal    None
**/
VOID
EFIAPI
PrintMcTmeCmiDataVersion (
  VOID
  );
#endif //_MEM_SMEE_CMI_CREDITS_H_
