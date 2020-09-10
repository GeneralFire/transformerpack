/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef _SOCKET_SETUP_H_
#define _SOCKET_SETUP_H_

#include <Uefi.h>
#include <Uefi/UefiInternalFormRepresentation.h>
#include <Protocol/IioUds.h>
#include "SocketSetupStrDefs.h"
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HiiLib.h>
#include <Library/UefiHiiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/S3IoLib.h>

#include <Protocol/HiiString.h>
#include <Protocol/HiiDatabase.h>
// APTIOV_SERVER_OVERRIDE_RC_START
#include <Protocol/HiiPackageList.h>
// APTIOV_SERVER_OVERRIDE_RC_END
#include <Protocol/HiiConfigRouting.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/IdeControllerInit.h>
#include <Protocol/Smbios.h>
#include <Protocol/DevicePath.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/MpService.h>
#include <Protocol/CpuPpmProtocol.h>
#include <Cpu/CpuCoreRegs.h>
#include <Guid/HobList.h>

#include <Guid/MemoryConfigData.h>
#include <Guid/MemoryMapData.h>
#include <Guid/MdeModuleHii.h>
#include <Guid/SocketVariable.h>
#include "SocketConfiguration.h"
#include <Library/MemoryCoreLib.h>
#include <KtiSetupDefinitions.h>
#include <IndustryStandard/SmBios.h>
#include <Library/MmPciBaseLib.h>
#include <PchAccess.h>
#include <Guid/MktmeInitData.h>
#include <Register/Cpuid.h>
#include <Guid/TdxInitData.h>

#define NEWSTRING_SIZE             0x200
#define IA32_TME_CAPABILITY_MSR    0x982
extern EFI_GUID                    gSocketPkgListGuid;
extern EFI_GUID                    gSignalBeforeEnterSetupGuid;

typedef struct {
  UINT8   Type;
  UINT8   Length;
  UINT16  Handle;
} SMBIOS_HEADER;

typedef struct {
  UINT32  Id;
  UINT32  Freq;
  UINT32  MaxRatio;
  UINT32  MinRatio;
  UINT32  Ucode;
  UINT32  L1Cache;
  UINT32  L2Cache;
  UINT32  L3Cache;
  CHAR16  *Version;
  UINT64  FusedCores;
  UINT64  ActiveCores;
} CPU_SETUP_INFO;


typedef enum {
    ProcVersionVal,
    ProcIdVal,
    ProcFreqVal,
    ProcMaxRatioVal,
    ProcMinRatioVal,
    ProcUcodeVal,
    ProcL1Val,
    ProcL2Val,
    ProcL3Val,
    LastItemVal
} SOCKET_UPDATE_INFO_FIELDS;


#define MAX_SOCKETS_UPDATE_INFO 8
#define MAX_SOCKETS_UPDATE_FIELDS LastItemVal

#endif
