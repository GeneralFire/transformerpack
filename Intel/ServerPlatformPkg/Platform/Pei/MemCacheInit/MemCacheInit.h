/** @file
  Header file for setting up Memory Type Range Registers for the platform

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

#include <Base.h>
#include <PiPei.h>
#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/SetupLib.h>
#include <Library/MtrrLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/EmulationConfigurationLib.h>

#include <Guid/PlatformInfo.h>
#include <Guid/SetupVariable.h>
#include <Guid/MemoryMapData.h>

#include <Ppi/MpServices.h>

#include <Protocol/IioUds.h>
#include <Register/ArchitecturalMsr.h>
#include <Cpu/CpuCoreRegs.h>
#include <Register/Cpuid.h>

//   EDK2_TODO
#define EFI_MAX_ADDRESS   0xFFFFFFFF        /* Definition in EfiBind.h */

/**

  End of PEI Callback to set up the MTRRs for system boot.

  @param PeiServices       -  General purpose services available to every PEIM.
  @param NotifyDescriptor  -  Not used.
  @param Ppi               -  Not used.

  @retval EFI_SUCCESS   - MTRRs successfully programmed
          EFI_NOT_FOUND - Required HOB not found

**/
EFI_STATUS
MemCacheEndOfPeiCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );
