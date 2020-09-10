/** @file
  Implementation IoMca RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2017 Intel Corporation. <BR>

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
#include <RcRegs.h>
#include <Library/MmPciExtLib.h>
#include <Library/IoLib.h>
#include <Library/PlatMemTopologyLib.h>

/**
  This function ennable UBOX IOMCA register

  @param[in] IioSev1Pcc        PCC equal 0 for severity 1 error.

  @retval    none.
**/
VOID
EFIAPI
UboxEnableIoMca (
  UINT8     IioSev1Pcc
  )
{
  UINT8    Skt;
  NCEVENTS_CR_UBOXERRCTL2_UBOX_CFG_STRUCT  UboxErrCtl2;

  for (Skt = 0; Skt < MAX_SOCKET; Skt ++) {
    if (!IsSocketPresent (Skt)) {
      return;
    }

    UboxErrCtl2.Data = ReadCpuCsr (Skt, 0, NCEVENTS_CR_UBOXERRCTL2_UBOX_CFG_REG);
    UboxErrCtl2.Bits.enable_io_mca = 1;
    //
    //When enable_pcc_eq0_sev1 = 0, pcc = 1;
    //When enable_pcc_eq0_sev1 = 1, pcc = 0;
    //
    if (IioSev1Pcc == 0) {
      UboxErrCtl2.Bits.enable_pcc_eq0_sev1 = 0;
    } else {
      UboxErrCtl2.Bits.enable_pcc_eq0_sev1 = 1;
    }
    WriteCpuCsr (Skt, 0, NCEVENTS_CR_UBOXERRCTL2_UBOX_CFG_REG, UboxErrCtl2.Data);
  }
}

/**
  This function Disables IOMCA logging

  @retval    none.
**/
VOID
EFIAPI
UboxDisableIoMca (
  VOID
  )
{
  UINT8    Skt;
  NCEVENTS_CR_UBOXERRCTL2_UBOX_CFG_STRUCT  UboxErrCtl2;

  for (Skt = 0; Skt < MAX_SOCKET; Skt ++) {
    if (!IsSocketPresent (Skt)) {
      return;
    }

    UboxErrCtl2.Data = ReadCpuCsr (Skt, 0, NCEVENTS_CR_UBOXERRCTL2_UBOX_CFG_REG);
    UboxErrCtl2.Bits.enable_io_mca = 0;
    WriteCpuCsr (Skt, 0, NCEVENTS_CR_UBOXERRCTL2_UBOX_CFG_REG, UboxErrCtl2.Data);
  }
}