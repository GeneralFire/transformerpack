/** @file
  Function prototype for GetCpuInfo driver

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

#ifndef _ALL_CPU_INFO_H
#define _ALL_CPU_INFO_H

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/MdeModuleHii.h>
#include <Library/UefiLib.h>
#include <IioUniversalData.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Protocol/IioUds.h>
#include <Guid/PlatformInfo.h>
#include <Library/MemoryCoreLib.h>
#include <Protocol/MpService.h>
#include <Library/HiiLib.h>
#include <Register/ArchitecturalMsr.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/TimerLib.h>
#include <Register/Cpuid.h>
#include <Library/MicrocodeLib.h>


#define UPDATE_DATA_SIZE  0x1000
//
// This is the generated header file which includes whatever needs to be exported (strings + IFR)
//

extern UINT8  VfrBin[];

extern UINT8  GetCpuInfoStrings[];

typedef struct {
  EFI_HII_HANDLE      RegisteredHandle;
} GET_CPU_INFO;

EFI_STATUS
EFIAPI
RegisterAllCpuInfo (
  IN GET_CPU_INFO       *Private
  );

VOID
GetCpuInformation (
  VOID
  );

VOID
UpdateTokenValue (
  CHAR16              *String,
  CHAR8               *Lang,
  VOID                *StartOpCodeHandle,
  GET_CPU_INFO        *Private
  );

#endif
