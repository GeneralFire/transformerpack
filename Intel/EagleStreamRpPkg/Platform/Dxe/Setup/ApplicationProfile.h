//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/**

Copyright (c) 1999 - 2018 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


  @file ApplicationProfile.h

  Header file for Platform Initialization Driver.

Revision History:

**/

#ifndef _WORKLOAD_PROFILE_H_
#define _WORKLOAD_PROFILE_H_

#include <PiDxe.h>

#include <Protocol/IioUds.h>

#include "Platform.h"
#include "Configuration.h"
#include "PlatformStrDefs.h"
#include "FlashMap.h"

#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HiiLib.h>
#include <Library/HobLib.h>
#include <Library/PciLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/S3IoLib.h>
#include <Library/SetupLib.h>
#include <Library/MmPciBaseLib.h>
#include <Protocol/HiiString.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiConfigRouting.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/PlatformType.h>
#include <Protocol/TcgService.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/MpService.h>
#include <Protocol/SimpleFileSystem.h>

#include <Guid/CpuHtCapable.h>
#include <Guid/HobList.h>
#include <Guid/MemorySetupData.h>
#include <Guid/MemoryConfigData.h>
#include <Guid/SetupVariable.h>


typedef struct {
  UINT8 CorePrefetch;
  UINT8 HyperThreading;
  UINT8 Turbo;
  UINT8 SubNUMAClustering;
  UINT8 UPIPrefetch;
} EFI_APPLICATION_PROFILE;

typedef struct {
  UINT8 EETurboDisable;
  UINT8 PagePolicy;
  UINT8 ProcessorHWPMEnable;
  UINT8 DimmInterleaving;
  UINT8 PatrolScrub;
  UINT8 PerfPLimitEn;
  UINT8 ProcessorC1eEnable;
  UINT8 ProcessorVmxEnable;
  UINT8 VTdSupport;
} EFI_APPLICATION_PROFILE_EXT;

/**

    Setup Application Profile

    @param Default - Application: which Application that you selected

    @retval None

**/
VOID
SetupApplicationProfile (
   IN   UINT8                                      Application
   );

#endif
