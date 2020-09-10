/** @file

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

#include <RcRegs.h>
#include <Library/UsraCsrLib.h>
#include <Guid/IioIpInterface.h>
#include <Library/KtiApi.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>

/**
  Sets IIO TOLM and TOHM base registers

  @param[in] SocId     Socket ID
  @param[in] CtrId     Controller ID
  @param[in] Tolm      TOLM in 64MB granularity
  @param[in] Tohm      TOHM in 64MB granularity

  @retval EFI_SUCCESS  IIO TOLM and TOHM is successfully set.

**/
EFI_STATUS
EFIAPI
IioSetTolmTohm (
  IN UINT8           SocId,
  IN UINT8           CtrlId,
  IN UINT32          Tolm,
  IN UINT32          Tohm
  )
{
  TOLM_IIO_VTD_STRUCT     Tolm_5_0;
  TOHM_N0_IIO_VTD_STRUCT  Tohm_0_5_0;
  TOHM_N1_IIO_VTD_STRUCT  Tohm_1_5_0;

  Tolm_5_0.Data = 0;
  Tohm_0_5_0.Data = 0;
  Tohm_1_5_0.Data = 0;

  Tolm_5_0.Bits.addr = Tolm;
  Tohm_0_5_0.Bits.addr = Tohm;
  Tohm_1_5_0.Bits.addr = Tohm >> 6;

  UsraCsrWrite (SocId, CtrlId, TOLM_IIO_VTD_REG, Tolm_5_0.Data);
  UsraCsrWrite (SocId, CtrlId, TOHM_N0_IIO_VTD_REG, Tohm_0_5_0.Data);
  UsraCsrWrite (SocId, CtrlId, TOHM_N1_IIO_VTD_REG, Tohm_1_5_0.Data);

  return EFI_SUCCESS;
}

/**
  Protects a specified address range via the GENPROT mechanism.
  Long term : The routine should be implemented in IioIpLib with general
              search for available GENPROT registers.
  Short term: Implement API temporarily with a hardcoded GENPROT allocation
              until a full implementation in IioIpLib is available

  @param[in] SocId - Socket Index
  @param[in] Base  - Base of requested address range (in 64MB units)
  @param[in] Limit - Limit of requested address range (in 64MB units)

  @retval EFI_SUCCESS           Requested address range is protected
  @retval EFI_OUT_OF_RESOURCES  Requested address range is NOT protected
  @retval EFI_INVALID_PARAMETER Parameter is invalid
  @retval EFI_UNSUPPORTED       API is not supported

**/
EFI_STATUS
EFIAPI
SetGenprotRange (
  IN UINT8  SocId,
  IN UINT32 Base,
  IN UINT32 Limit
  )
{
  UINT8                                 Stack;
  UINT64                                IioData;
  GENPROTRANGE5_BASE_N1_IIO_VTD_STRUCT  BaseN1IioReg;
  GENPROTRANGE5_BASE_N0_IIO_VTD_STRUCT  BaseN0IioReg;
  GENPROTRANGE5_LIMIT_N1_IIO_VTD_STRUCT LimitN1IioReg;
  GENPROTRANGE5_LIMIT_N0_IIO_VTD_STRUCT LimitN0IioReg;
  UINT32                                BaseN1Data;
  UINT32                                LimitN1Data;

  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    if (!(GetStackPresentBitMap (SocId) & (BIT0 << Stack))) {
      continue;
    }

    DEBUG ((EFI_D_INFO, "Protect NXM Memory Range for MTRR NxmBase = 0x%x\n", Base));
    IioData = LShiftU64 (Base, CONVERT_64MB_TO_B);

    BaseN1Data = (UINT32) RShiftU64 (IioData, 32);
    BaseN1IioReg.Data = 0;
    BaseN1IioReg.Bits.addr = BaseN1Data & 0x000FFFFF;
    UsraCsrWrite (SocId, Stack, GENPROTRANGE5_BASE_N1_IIO_VTD_REG, BaseN1IioReg.Data);

    BaseN0IioReg.Data = 0;
    BaseN0IioReg.Bits.addr = (UINT16) RShiftU64 (IioData, 16);
    UsraCsrWrite (SocId, Stack, GENPROTRANGE5_BASE_N0_IIO_VTD_REG, BaseN0IioReg.Data);

    DEBUG ((EFI_D_INFO, "Protect NXM Memory Range for MTRR NxmLimit = 0x%x\n", Limit));
    IioData = LShiftU64 (Limit, CONVERT_64MB_TO_B);

    LimitN1Data = (UINT32) RShiftU64 (IioData, 32);
    LimitN1IioReg.Data = 0;
    LimitN1IioReg.Bits.addr = LimitN1Data & 0x000FFFFF;
    UsraCsrWrite (SocId, Stack, GENPROTRANGE5_LIMIT_N1_IIO_VTD_REG, LimitN1IioReg.Data);

    LimitN0IioReg.Data = 0;
    LimitN0IioReg.Bits.addr = (UINT16) RShiftU64 (IioData, 16);
    UsraCsrWrite (SocId, Stack, GENPROTRANGE5_LIMIT_N0_IIO_VTD_REG, LimitN0IioReg.Data);
  }

  return EFI_SUCCESS;
}