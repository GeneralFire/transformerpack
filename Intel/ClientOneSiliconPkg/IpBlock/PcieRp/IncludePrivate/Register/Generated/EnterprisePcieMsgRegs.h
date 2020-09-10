
/** @file
  pcie_0_9_0_msg_instCommon.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2019 Intel Corporation. <BR>

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

  This file contains Silicon register definitions.

  This is a generated file; please do not modify it directly.

**/

#ifndef _PCH_REGS_PCIE_0_9_0_MSG_INSTCOMMON_MSG_H_
#define _PCH_REGS_PCIE_0_9_0_MSG_INSTCOMMON_MSG_H_

#define R_PCH_PCIE_MSG_PPBIFCTL_PRIV  0x000000C0
#define S_PCH_PCIE_MSG_PPBIFCTL_PRIV 4

#define N_PCH_PCIE_MSG_PPBIFCTL_PRIV_BIFCTL0 0
#define B_PCH_PCIE_MSG_PPBIFCTL_PRIV_BIFCTL0 (BIT2 | BIT1 | BIT0)

#define N_PCH_PCIE_MSG_PPBIFCTL_PRIV_CL 31
#define B_PCH_PCIE_MSG_PPBIFCTL_PRIV_CL BIT31



#define R_PCH_PCIE_MSG_ACRDPR 0x000007C0
#define S_PCH_PCIE_MSG_ACRDPR 8

#define N_PCH_PCIE_MSG_ACRDPR_ACPV 0
#define B_PCH_PCIE_MSG_ACRDPR_ACPV (0xFFFFFFFFFFFFFFFFULL)



#define R_PCH_PCIE_MSG_ACWRPR 0x000007C8
#define S_PCH_PCIE_MSG_ACWRPR 8

#define N_PCH_PCIE_MSG_ACWRPR_ACPV 0
#define B_PCH_PCIE_MSG_ACWRPR_ACPV (0xFFFFFFFFFFFFFFFFULL)



#define R_PCH_PCIE_MSG_ACCRPR 0x000007D0
#define S_PCH_PCIE_MSG_ACCRPR 8

#define N_PCH_PCIE_MSG_ACCRPR_ACPV 0
#define B_PCH_PCIE_MSG_ACCRPR_ACPV (0xFFFFFFFFFFFFFFFFULL)



#define R_PCH_PCIE_MSG_DEVHIDE 0x000007D8
#define S_PCH_PCIE_MSG_DEVHIDE 4

#define N_PCH_PCIE_MSG_DEVHIDE_P0CFGH 0
#define B_PCH_PCIE_MSG_DEVHIDE_P0CFGH BIT0

#define N_PCH_PCIE_MSG_DEVHIDE_P1CFGH 1
#define B_PCH_PCIE_MSG_DEVHIDE_P1CFGH BIT1

#define N_PCH_PCIE_MSG_DEVHIDE_P2CFGH 2
#define B_PCH_PCIE_MSG_DEVHIDE_P2CFGH BIT2

#define N_PCH_PCIE_MSG_DEVHIDE_P3CFGH 3
#define B_PCH_PCIE_MSG_DEVHIDE_P3CFGH BIT3

#define N_PCH_PCIE_MSG_DEVHIDE_PCFGHP 4
#define B_PCH_PCIE_MSG_DEVHIDE_PCFGHP BIT4

#define N_PCH_PCIE_MSG_DEVHIDE_P0GBE 5
#define B_PCH_PCIE_MSG_DEVHIDE_P0GBE BIT5

#define N_PCH_PCIE_MSG_DEVHIDE_P1GBE 6
#define B_PCH_PCIE_MSG_DEVHIDE_P1GBE BIT6

#define N_PCH_PCIE_MSG_DEVHIDE_P2GBE 7
#define B_PCH_PCIE_MSG_DEVHIDE_P2GBE BIT7

#define N_PCH_PCIE_MSG_DEVHIDE_P3GBE 8
#define B_PCH_PCIE_MSG_DEVHIDE_P3GBE BIT8

#define N_PCH_PCIE_MSG_DEVHIDE_DHCL 31
#define B_PCH_PCIE_MSG_DEVHIDE_DHCL BIT31



#define R_PCH_PCIE_MSG_SAIECSR_PRIV 0x000007DC
#define S_PCH_PCIE_MSG_SAIECSR_PRIV 4



#define N_PCH_PCIE_MSG_SAIECSR_PRIV_SSAIESID 0
#define B_PCH_PCIE_MSG_SAIECSR_PRIV_SSAIESID (0x000000FF)



#define N_PCH_PCIE_MSG_SAIECSR_PRIV_AVDSBI 8
#define B_PCH_PCIE_MSG_SAIECSR_PRIV_AVDSBI BIT8

#define N_PCH_PCIE_MSG_SAIECSR_PRIV_FSIDAV 9
#define B_PCH_PCIE_MSG_SAIECSR_PRIV_FSIDAV (BIT12 | BIT11 | BIT10 | BIT9)



#define R_PCH_PCIE_MSG_FUSECSR1 0x000007F0
#define S_PCH_PCIE_MSG_FUSECSR1 4

#define N_PCH_PCIE_MSG_FUSECSR1_FI 0
#define B_PCH_PCIE_MSG_FUSECSR1_FI (0xFFFFFFFF)



#define R_PCH_PCIE_MSG_FUSECSR2 0x000007F4
#define S_PCH_PCIE_MSG_FUSECSR2 4

#define N_PCH_PCIE_MSG_FUSECSR2_FI 0
#define B_PCH_PCIE_MSG_FUSECSR2_FI (0xFFFFFFFF)



#define R_PCH_PCIE_MSG_STRAPCSR1 0x000007F8
#define S_PCH_PCIE_MSG_STRAPCSR1 4

#define N_PCH_PCIE_MSG_STRAPCSR1_SI 0
#define B_PCH_PCIE_MSG_STRAPCSR1_SI (0xFFFFFFFF)



#define R_PCH_PCIE_MSG_STRAPCSR2 0x000007FC
#define S_PCH_PCIE_MSG_STRAPCSR2 4

#define N_PCH_PCIE_MSG_STRAPCSR2_SI 0
#define B_PCH_PCIE_MSG_STRAPCSR2_SI (0xFFFFFFFF)
#endif /* _PCIE_0_9_0_MSG_INSTCOMMON_h */
