/** @file
  Mesh 2 Mem CMI Credit information internal header
  This file is for SPR

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

#ifndef MESH_2_MEM_CMI_CREDITS_SPR_H_
#define MESH_2_MEM_CMI_CREDITS_SPR_H_

/**
  Get Read Completion Credit Channel Config Value per Channel
  Returns value for SPR

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval None
**/
VOID
GetCmiRdCplCreditChnlConfig1ValueChannelSpr (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  );

/**
  Get Read Completion Credit Channel Config Value per Channel
  Returns value for SPR

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval None
**/
VOID
GetCmiRdCplCreditChnlConfig2ValueChannelSpr (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  );

/**
  Get CMI Rsp Credit Channel Config Value per Channel
  Returns value of SPR

  @param[in]   Socket          -   Socket number
  @param[in]   Channel         -   Channel number
  @param[out]  *ConfigValue    -   Pointer to Register Configuration value.
  @param[out]  *Size           -   Pointer to Structure Size

  @retval None
**/
VOID
GetCmiRspCreditChnlConfigValueChannelSpr (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  OUT CMI_DEFAULT_REG_VALUE **ConfigValue,
  OUT UINT8                 *Size
  );

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
M2mPollRegisterOffsetForEmulation (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    BoxInst,
  IN UINT32   Offset,
  IN UINT32   Mask,
  IN UINT8    Logic
  );

/**

Print Mesh2Mem CMI SpreadSheet Data Version

@param[in] None

@retVal    None
**/
VOID
EFIAPI
PrintMesh2MemCmiDataVersion(
  VOID
);
#endif //MESH_2_MEM_CMI_CREDITS_SPR_H_
