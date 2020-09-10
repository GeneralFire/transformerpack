/** @file
  Contains Register Aliases

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#ifndef _RC_REGS_10NM_H_
#define _RC_REGS_10NM_H_

#include <RcRegs.h>

//
// Temporary workarounds to handle the case where the combo headers
// have different struct or register names than the Purley headers
// due to tools issues. Apply to both SKX and CNX / KNH
//

// ICX_TODO

#define   vmse_wrtrn_enb                                    link_wrtrn_enb
#define   tpar_recov_ch0                                    tpar_recov

// BWV header aliases
#define   DDRCRCMDCONTROLS_SA_FNV_DDRIO_DAT7_CH_STRUCT  DDRCRCMDCONTROLS_3_FNV_DDRIO_DAT7_CH_STRUCT
#define   DDRCRCMDCONTROLS2_SA_FNV_DDRIO_DAT7_CH_STRUCT DDRCRCMDCONTROLS2_3_FNV_DDRIO_DAT7_CH_STRUCT
#define   DDRCRCMDCONTROLS3_SA_FNV_DDRIO_DAT7_CH_STRUCT DDRCRCMDCONTROLS3_3_FNV_DDRIO_DAT7_CH_STRUCT
#define   DDRCRCMDTRAINING_SA_FNV_DDRIO_DAT7_CH_STRUCT  DDRCRCMDTRAINING_3_FNV_DDRIO_DAT7_CH_STRUCT
#define   DDRCRCTLRANKSUSED_SA_FNV_DDRIO_DAT7_CH_STRUCT DDRCRCTLRANKSUSED_3_FNV_DDRIO_DAT7_CH_STRUCT
#define   DDRCRCMDPICODING_SA_FNV_DDRIO_DAT7_CH_STRUCT  DDRCRCMDPICODING_3_FNV_DDRIO_DAT7_CH_STRUCT
#define   DDRCRCMDPICODING2_SA_FNV_DDRIO_DAT7_CH_STRUCT DDRCRCMDPICODING2_3_FNV_DDRIO_DAT7_CH_STRUCT

#define   DDRCRCMDCONTROLS_SA_FNV_DDRIO_DAT7_CH_REG     DDRCRCMDCONTROLS_3_FNV_DDRIO_DAT7_CH_REG
#define   DDRCRCMDCONTROLS2_SA_FNV_DDRIO_DAT7_CH_REG    DDRCRCMDCONTROLS2_3_FNV_DDRIO_DAT7_CH_REG
#define   DDRCRCMDCONTROLS3_SA_FNV_DDRIO_DAT7_CH_REG    DDRCRCMDCONTROLS3_3_FNV_DDRIO_DAT7_CH_REG
#define   DDRCRCMDTRAINING_SA_FNV_DDRIO_DAT7_CH_REG     DDRCRCMDTRAINING_3_FNV_DDRIO_DAT7_CH_REG
#define   DDRCRCTLRANKSUSED_SA_FNV_DDRIO_DAT7_CH_REG    DDRCRCTLRANKSUSED_3_FNV_DDRIO_DAT7_CH_REG
#define   DDRCRCMDPICODING_SA_FNV_DDRIO_DAT7_CH_REG     DDRCRCMDPICODING_3_FNV_DDRIO_DAT7_CH_REG
#define   DDRCRCMDPICODING2_SA_FNV_DDRIO_DAT7_CH_REG    DDRCRCMDPICODING2_3_FNV_DDRIO_DAT7_CH_REG
#define   DDRCRCMDCONTROLS_NA_FNV_DDRIO_DAT7_CH_REG     DDRCRCMDCONTROLS_1_FNV_DDRIO_DAT7_CH_REG
#define   DDRCRCMDCONTROLS_NB_FNV_DDRIO_DAT7_CH_REG     DDRCRCMDCONTROLS_0_FNV_DDRIO_DAT7_CH_REG
#define   DDRCRCMDCONTROLS_SB_FNV_DDRIO_DAT7_CH_REG     DDRCRCMDCONTROLS_2_FNV_DDRIO_DAT7_CH_REG

#endif // _RC_REGS_H_
