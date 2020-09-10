/** @file
  Memory POR table for a specifc processor.

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

#include <Base.h>
#include <Cpu/CpuIds.h>
#include <Library/KtiApi.h>
#include <SysHost.h>
#include <PlatformInfoTypes.h>
#include "Include/MemPorLibInternal.h"

DIMM_POR_ENTRY mDimmPorTable[] = {
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 1,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 2,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 4,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, RDIMM,   DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 4,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, RDIMM,   DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 8,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, RDIMM,   DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 16, NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 4,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, LRDIMM,  DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 4,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, LRDIMM,  DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 8,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, LRDIMM,  DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 16, NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, NVMDIMM, DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, NVMDIMM, DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 1,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 2,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 4,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, RDIMM,   DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 4,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, RDIMM,   DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 8,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, RDIMM,   DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 16, NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 4,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, LRDIMM,  DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 4,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, LRDIMM,  DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 8,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, LRDIMM,  DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 16, NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, NVMDIMM, DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, NVMDIMM, DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 2,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 4,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 4,  DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 8,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, RDIMM,   DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 8,  DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, RDIMM,   DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 16, DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, RDIMM,   DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 32, NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 4,  DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 8,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, LRDIMM,  DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 8,  DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, LRDIMM,  DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 16, DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, LRDIMM,  DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 32, NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, NVMDIMM,          StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 2,  NOT_SUP,  NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, NVMDIMM,          StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 2,  NOT_SUP,  NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 3,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 3,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 5,  NOT_SUP,  NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 5,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 9,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 17, NOT_SUP,  NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 3,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 3,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 5,  NOT_SUP,  NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 5,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 9,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 17, NOT_SUP,  NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 3,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 5,  NOT_SUP,  NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 5,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 9,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 17, NOT_SUP,  NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 3,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 5,  NOT_SUP,  NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 5,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 9,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_2S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 17, NOT_SUP,  NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 1,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 2,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 4,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, RDIMM,   DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 4,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, RDIMM,   DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 8,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, RDIMM,   DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 16, NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 4,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, LRDIMM,  DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 4,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, LRDIMM,  DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 8,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, LRDIMM,  DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 16, NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, NVMDIMM, DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 1, 1, NVMDIMM, DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 1,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 2,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 4,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, RDIMM,   DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 4,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, RDIMM,   DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 8,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, RDIMM,   DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 16, NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 4,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, LRDIMM,  DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 4,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, LRDIMM,  DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 8,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, LRDIMM,  DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 16, NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, NVMDIMM, DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 1, NVMDIMM, DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 2,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 4,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 4,  DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 8,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, RDIMM,   DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 8,  DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, RDIMM,   DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 16, DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, RDIMM,   DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 32, NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 4,  DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 8,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, LRDIMM,  DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 8,  DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, LRDIMM,  DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 16, DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, LRDIMM,  DIMM_TYPE_IGNORE, Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 32, NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, NVMDIMM,          StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 2,  NOT_SUP,  NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, NVMDIMM,          StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 2,  NOT_SUP,  NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 3,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 3,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 5,  NOT_SUP,  NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 5,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 9,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 17, NOT_SUP,  NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 3,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 3,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 5,  NOT_SUP,  NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 5,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 9,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, RDIMM,            Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 17, NOT_SUP,  NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 3,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 5,  NOT_SUP,  NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 5,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 9,  DDR_4400, NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 17, NOT_SUP,  NOT_SUP },// for NVM-10D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 3,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 5,  NOT_SUP,  NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 5,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 9,  DDR_4400, NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_4S,   TypeXcc, SPD_TYPE_DDR5, 2, 2, NVMDIMM, LRDIMM,           Stack3ds,    DimmConnectorIgnore, SPD_VDD_110, 17, NOT_SUP,  NOT_SUP },// for NVM-11D
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 1, 1, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 1,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 1, 1, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 2,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 1, 1, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 1,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 1, 1, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 2,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 1, 1, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 1, 1, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 1, 1, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 4,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 1, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 1, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 1, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 4,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 1, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 8,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 1, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 16, NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 1, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 1,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 1, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 2,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 1, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 1,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 1, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 2,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 1, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 1, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 1, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 4,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 1,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 4,  DDR_4800, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 8,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 1, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 16, NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 2, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 2,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 2, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 4,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 2, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 2,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 2, UDIMM,   DIMM_TYPE_IGNORE, StackIgnore, DimmConnectorIgnore, SPD_VDD_110, 4,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 2, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 2, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 4,  DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 2, RDIMM,   DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 8,  NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 2, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 2,  DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 2, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 4,  DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 2, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 8,  DDR_4400, NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 2, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 16, NOT_SUP,  NOT_SUP },
  { CPU_FAMILY_SPRSP, SOCKET_1SWS, TypeHcc, SPD_TYPE_DDR5, 2, 2, LRDIMM,  DIMM_TYPE_IGNORE, StackSdp,    DimmConnectorIgnore, SPD_VDD_110, 32, NOT_SUP,  NOT_SUP },
};

UINT16 mDimmPorTableLength = ARRAY_SIZE (mDimmPorTable);
