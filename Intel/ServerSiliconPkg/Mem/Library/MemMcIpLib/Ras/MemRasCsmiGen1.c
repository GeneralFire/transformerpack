/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2018 Intel Corporation. <BR>

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
#include "Include/MemMcRegs.h"

/**
  Enable eMCA gen 1 CSMI morphing.

  The setting applies to each channel on the MC.

  @param[in]  Socket          Socket index.
  @param[in]  Mc              Memory controller on socket.
**/
VOID
EFIAPI
EnableMcChanCsmiGen1 (
  IN  UINT8   Socket,
  IN  UINT8   Mc
  )
{
  EMCALTCTL_MC_MAIN_STRUCT       EmcaLtCtrl;
  UINT8                          Ch;

  for (Ch = Mc * MAX_MC_CH; Ch < (Mc + 1) * MAX_MC_CH; Ch ++ ){

    EmcaLtCtrl.Data = (UINT8) ReadCpuCsr (Socket, Ch, EMCALTCTL_MC_MAIN_REG);
    EmcaLtCtrl.Bits.emcalt_sel_smi = 1;
    WriteCpuCsr (Socket, Ch, EMCALTCTL_MC_MAIN_REG, EmcaLtCtrl.Data);
  }
}
