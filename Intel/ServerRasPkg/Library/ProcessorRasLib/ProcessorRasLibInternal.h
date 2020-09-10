/** @file
  Master header file for processor RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2018 Intel Corporation. <BR>

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

#ifndef _PROCESSOR_RAS_LIB_H_
#define _PROCESSOR_RAS_LIB_H_

#include <Base.h>

#include <Register/Cpuid.h>
#include <Register/LocalApic.h>
#include <Register/Msr.h>
#include <Cpu/CpuCoreRegs.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/LocalApicLib.h>
#include <Library/MpExtensionLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/RasDebugLib.h>
#include <Library/ProcessorRasLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/RasSiliconLib.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/WorkaroundLib.h>
#include <RcRegs.h>

#define MSR_IA32_INVALID      0xFFFFFFFF

#pragma  pack(1)
typedef struct {
  UINT32 McBankBitFieldEn;
  UINT16 McScope;
} MC_ENABLE_INFO;
#pragma  pack()

BOOLEAN
IsLmceSignaled (
  VOID
  );


UINT32
EFIAPI
GetEmcaBankSupp (
  VOID
  );


/**

    Get Bank Index for Mci_Misc2 for McBank

    @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

    @retval Mci_Misc2 Index

**/
UINT32
GetAddrMcBankCtl2 (
  IN UINT32 McBankIdx
  );

#endif
