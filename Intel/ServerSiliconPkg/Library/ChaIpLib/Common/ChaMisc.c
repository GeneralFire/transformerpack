/** @file
  Common functions for configuring CHA_PMA* registers.

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

//
// Include files
//
#include "Include/ChaRegs.h"
#include <Library/UsraCsrLib.h>
#include <Library/ChaLib.h>

/**
  CHA: Replicate ME Seg registers from SBSP to to be online PBSP

  @param SbspSocId           - SBSP Node number (Socket ID)
  @param BoxInst             - Box Instance, 0 based
  @param TobeOnlinePbspSocId - to be online PBSP Node number

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaRasReplicateMeSeg (
  UINT8    SbspSocId,
  UINT8    BoxInst,
  UINT8    TobeOnlinePbspSocId
  )
{
  UINT32 Data32;
  //
  // Program ME SEG Base/Limit in CHA
  //
  Data32 = UsraCsrRead ((UINT8)SbspSocId, BoxInst, MESEG_BASE_N0_CHABC_SAD_REG);
  UsraCsrWrite ((UINT8)TobeOnlinePbspSocId, BoxInst, MESEG_BASE_N0_CHABC_SAD_REG, Data32);
  Data32 = UsraCsrRead ((UINT8)SbspSocId, BoxInst, MESEG_BASE_N1_CHABC_SAD_REG);
  UsraCsrWrite ((UINT8)TobeOnlinePbspSocId, BoxInst, MESEG_BASE_N1_CHABC_SAD_REG, Data32);
  Data32 = UsraCsrRead ((UINT8)SbspSocId, BoxInst, MESEG_LIMIT_N0_CHABC_SAD_REG);
  UsraCsrWrite ((UINT8)TobeOnlinePbspSocId, BoxInst, MESEG_LIMIT_N0_CHABC_SAD_REG, Data32);
  Data32 = UsraCsrRead ((UINT8)SbspSocId, BoxInst, MESEG_LIMIT_N1_CHABC_SAD_REG);
  UsraCsrWrite ((UINT8)TobeOnlinePbspSocId, BoxInst, MESEG_LIMIT_N1_CHABC_SAD_REG, Data32);

  return EFI_SUCCESS;
}

/**
  CHA: Configure KTI TOR Timeout

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param EntryCtrIncCtl   - Indication of which bit of upper base counter
                            increments entry counter.
  @param Level0Offset     - Encoded Intel UPI Level 0 Timeout offset.
  @param Level1Offset     - Encoded Intel UPI Level 1 Timeout offset.
  @param Level2Offset     - Encoded Intel UPI Level 2 Timeout offset.
  @param Level3Offset     - Encoded Intel UPI Level 3 Timeout offset.
  @param Level4Offset     - Encoded Intel UPI Level 4 Timeout offset.
  @param Level6Offset     - Encoded Intel UPI Level 6 Timeout offset.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaConfigureKtiTorTimeout (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT8    EntryCtrIncCtl,
  UINT8    Level0Offset,
  UINT8    Level1Offset,
  UINT8    Level2Offset,
  UINT8    Level3Offset,
  UINT8    Level4Offset,
  UINT8    Level6Offset
  )
{
  QPI_TIMEOUT_CTRL_CHABC_SAD_STRUCT  KtiTorTimeout;
  QPI_TIMEOUT_CTRL2_CHABC_SAD_STRUCT KtiTorTimeout2;

  KtiTorTimeout.Data = UsraCsrRead (SocId, BoxInst, QPI_TIMEOUT_CTRL_CHABC_SAD_REG);
  KtiTorTimeout2.Data = UsraCsrRead (SocId, BoxInst, QPI_TIMEOUT_CTRL2_CHABC_SAD_REG);
  KtiTorTimeout.Bits.enabletortimeout  = 0x1;
  KtiTorTimeout.Bits.entry_ctr_inc_ctl = EntryCtrIncCtl;
  KtiTorTimeout.Bits.level0_offset     = Level0Offset;
  KtiTorTimeout.Bits.level1_offset     = Level1Offset;
  KtiTorTimeout.Bits.level2_offset     = Level2Offset;
  KtiTorTimeout.Bits.level3_offset     = Level3Offset;
  KtiTorTimeout.Bits.level4_offset     = Level4Offset;
  KtiTorTimeout2.Bits.level6_offset    = Level6Offset;
  UsraCsrWrite (SocId, BoxInst, QPI_TIMEOUT_CTRL_CHABC_SAD_REG, KtiTorTimeout.Data);
  UsraCsrWrite (SocId, BoxInst, QPI_TIMEOUT_CTRL2_CHABC_SAD_REG, KtiTorTimeout2.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Fixup PAM Attributes

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function is not executed successfully due to some illegal input.
  @retval EFI_UNSUPPORTED        The function is not supported by the library instance.

**/
EFI_STATUS
EFIAPI
ChaFixupPamAttributes (
  UINT8    SocId,
  UINT8    BoxInst
  )
{
  PAM0123_CHABC_SAD_STRUCT                Pam0123Reg;
  PAM456_CHABC_SAD_STRUCT                 Pam456Reg;

  Pam0123Reg.Data = UsraCsrRead (SocId, BoxInst, PAM0123_CHABC_SAD_REG);
  Pam0123Reg.Bits.pam0_hienable = 3;
  UsraCsrWrite (SocId, BoxInst, PAM0123_CHABC_SAD_REG, Pam0123Reg.Data);
  Pam456Reg.Data = UsraCsrRead (SocId, BoxInst, PAM456_CHABC_SAD_REG);
  Pam456Reg.Bits.pam5_loenable = 3;
  Pam456Reg.Bits.pam5_hienable = 3;
  Pam456Reg.Bits.pam6_loenable = 3;
  Pam456Reg.Bits.pam6_hienable = 3;
  UsraCsrWrite (SocId, BoxInst, PAM456_CHABC_SAD_REG, Pam456Reg.Data);

  return EFI_SUCCESS;
}
