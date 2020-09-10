/** @file
  Implementation of patrol scrub of SPR

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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
#include <Library/BaseLib.h>
#include "Include/MemMcRegs.h"

/**
  Get MCMTR register values.

  This routine returns close_page, bank_xor_enable, channel_xor_enable, cluster_xor_enable bit field values

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[out]  ClosePg        close_page value return  OPTIONAL Pass NULL if not needed
  @param[out]  BankXorEn      bank_xor_enable value return   OPTIONAL Pass NULL if not needed
  @param[out]  DdrXorModeEn          channel_xor_enable value return   OPTIONAL Pass NULL if not needed
  @param[out]  DdrHalfXorModeEn      cluster_xor_enable value return   OPTIONAL Pass NULL if not needed
**/
VOID
EFIAPI
GetMcmtr (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  OUT  UINT32   *ClosePg,              OPTIONAL
  OUT  UINT32   *BankXorEn,            OPTIONAL
  OUT  UINT32   *ChXorModeEn,          OPTIONAL
  OUT  UINT32   *ClusterXorModeEn      OPTIONAL
  )
{
  MCMTR_MC_MAIN_STRUCT                    McmtrMain;

  McmtrMain.Data = ReadCpuCsr (Skt, ChOnSkt, MCMTR_MC_MAIN_REG);
  if (ClosePg != NULL) {
    *ClosePg   = McmtrMain.Bits.close_pg;
  }
  if (BankXorEn != NULL) {
    *BankXorEn = McmtrMain.Bits.bank_xor_enable;
  }
  if (ChXorModeEn != NULL) {
    *ChXorModeEn = McmtrMain.Bits.channel_xor_enable;
  }
  if (ClusterXorModeEn != NULL) {
    *ClusterXorModeEn = McmtrMain.Bits.cluster_xor_enable;
  }
}
