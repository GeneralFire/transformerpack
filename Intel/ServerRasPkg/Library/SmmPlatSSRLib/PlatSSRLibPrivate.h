/** @file
  Interface of SSR private library.

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

#ifndef __PLAT_SSR_PRIVATE_LIB_H__
#define __PLAT_SSR_PRIVATE_LIB_H__

#include <Guid/MemoryMapData.h>
#include <RcRegs.h>
#ifdef SKX_HOST
#include <Mem/Library/MemMcIpLib14nm/Include/MemMcRegs.h>
#else
#include <Mem/Library/MemMcIpLib/Include/MemMcRegs.h>
#endif

//
//This data should be max safe frequence.
//
#define POISON_SCRUB_INTERVAL         0x2
#define MAX_NANO_SECOND_DELAY        1000

#pragma pack(1)

typedef enum {
  QuiesceAvailable,
  QuiesceInProgress
} ADDDC_PCODE_STATUS;



typedef struct {
  SCRUBCTL_MC_MAIN_STRUCT             ScrubCtlReg;
  SCRUBADDRESSLO_MC_MAIN_STRUCT       ScrubAddrLoReg;
  SCRUBADDRESSHI_MC_MAIN_STRUCT       ScrubAddrHiReg;
  SCRUBADDRESS2LO_MC_MAIN_STRUCT      ScrubAddrLo2Reg;
  SCRUBADDRESS2HI_MC_MAIN_STRUCT      ScrubAddrHi2Reg;
  SSR_STATE                           McchanSsrState;
  UINT32                              ChAmapReg;
  UINT32                              ScrubInterval;
} PATROL_SCRUB_STRUC;

typedef struct{
  PATROL_SCRUB_STRUC         PatrolScrubStatus[MC_MAX_NODE][MAX_MC_CH];
  UINT32                     PkgC6Signal[MAX_SOCKET];
  UINT32                     GvSignal[MAX_SOCKET];
  UINT32                     QuiescePcodeCounter[MAX_SOCKET];
  ADDDC_PCODE_STATUS         AdddcPcodeSts[MAX_SOCKET];
  UINT32                     AdddcPcodeCmd[MAX_SOCKET];
  BOOLEAN                    PatrolScrubFlag;
} SSR_STRUCTURE;

#pragma pack()
#endif
