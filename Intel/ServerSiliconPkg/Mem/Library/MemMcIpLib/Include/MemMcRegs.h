/** @file
  McRegs.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#ifndef _MC_REGS_H_
#define _MC_REGS_H_

#include <Mem/Include/MemIpVer.h>

#include <RegPath.h>

#if defined(SPR_HOST)
#if defined(DNT_HOST)
#include "Registers/Dnt/McIpRegisters.h"
#else // DNT_HOST
#include REG_PATH_PRIVATE_MDFIS(McIpRegisters.h)
#endif // DNT_HOST
#elif defined(DDR5_SUPPORT) && defined(ICX_HOST)
//
// Will chgange to use REG_PATH_PRIVATE when BRS SOC CRIF is released.
//
#include "Registers/Brs/McIpRegisters.h"
#else
#include REG_PATH_PRIVATE(McIpRegisters.h)
#endif

//
// CSR name aliasing
//

//
// Additional alias's for structs and registers that do not have ICX_KNH or SKX in the name
//

#define ALIAS_CHN_TEMP_CFG_MCDDC_CTL_STRUCT                 CHN_TEMP_CFG_MCDDC_CTL_STRUCT
#define ALIAS_CHN_TEMP_CFG_MCDDC_CTL_REG                    CHN_TEMP_CFG_MCDDC_CTL_REG
#define ALIAS_MC_NM_ROUTE_TABLE_MC_MAIN_STRUCT              MC_NM_ROUTE_TABLE_CFG_MC_MAIN_STRUCT
#define ALIAS_MC_NM_ROUTE_TABLE_MC_MAIN_REG                 MC_NM_ROUTE_TABLE_CFG_MC_MAIN_REG
#define ALIAS_CPGC_GLOBALCTRL_MC_MAIN_STRUCT                CPGC_GLOBALCTRL_MC_GLOBAL_STRUCT
#define ALIAS_CPGC_GLOBALCTRL_MC_MAIN_REG                   CPGC_GLOBALCTRL_MC_GLOBAL_REG
#define ALIAS_CPGC_GLOBALERR_MC_MAIN_STRUCT                 CPGC_GLOBALERR_MC_GLOBAL_STRUCT
#define ALIAS_CPGC_GLOBALERR_MC_MAIN_REG                    CPGC_GLOBALERR_MC_GLOBAL_REG

#endif //_MC_REGS_H_
