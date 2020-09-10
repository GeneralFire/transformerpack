/** @file
  Memory Initialization Module.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2018 Intel Corporation. <BR>

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

#include <Library/CrcLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/RcDebugLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/BasicIpBlockServicesLib.h>

#include <Guid/MrcPlatformHooksGuid.h>

#include "Include/BaseMemoryCoreInternal.h"

// APTIOV_SERVER_OVERRIDE_RC_START : AMI hook
#include  <Library/PeiServicesTablePointerLib.h>

#define AMI_PEI_AFTER_MRC_GUID \
 {0x64c96700, 0x6b4c, 0x480c, 0xa3, 0xe1, 0xb8, 0xbd, 0xe8, 0xf6, 0x2, 0xb2}
EFI_GUID gAmiPeiAfterMrcGuid            = AMI_PEI_AFTER_MRC_GUID;

static EFI_PEI_PPI_DESCRIPTOR mAmiPeiCompelteMrcDesc[] = {
    { (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST), \
      &gAmiPeiAfterMrcGuid, \
      NULL }
};
// APTIOV_SERVER_OVERRIDE_RC_END : AMI hook
/**

  MRC wrapper code.

  @param[in] Host  - Pointer to sysHost

  @retval N/A

**/
VOID
MemoryInit (
  IN PSYSHOST  Host
  )
{
  EFI_STATUS MrcStatus;
  EFI_STATUS PpiStatus;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;
  // APTIOV_SERVER_OVERRIDE_RC_START
  EFI_STATUS             Status;
  EFI_BOOT_MODE          BootMode;
  CONST EFI_PEI_SERVICES **PeiServices;
  // APTIOV_SERVER_OVERRIDE_RC_END

  //
  // PlatformHook Before Memory Init
  //
  PpiStatus = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (PpiStatus)) {
    MrcStatus = MrcPlatformHooksInterface->PlatformPreMemoryInit (Host);
  }

  //
  // Invoke MRC
  //
  MrcStatus = MemStart (Host);
  if (MrcStatus) {
    Host->var.common.rcStatus = (UINT32) MrcStatus;
  }

  // APTIOV_SERVER_OVERRIDE_RC_START
  // Install AMI hook guid
  PeiServices = GetPeiServicesTablePointer ();
  Status = (*PeiServices)->InstallPpi(PeiServices, &mAmiPeiCompelteMrcDesc[0]);
  ASSERT_EFI_ERROR (Status);

  // Determine if change boot mode
  Status = (*PeiServices)->GetBootMode(PeiServices, &BootMode);
  ASSERT_EFI_ERROR(Status);

  if (BootMode == BOOT_ON_S3_RESUME) {
      SetSysBootMode (S3Resume);
  } else {
      SetSysBootMode (NormalBoot);
  }
  // APTIOV_SERVER_OVERRIDE_RC_END
  //
  // Dump RC heap usage to the serial log
  //

  RcDebugPrint (SDBG_DEFAULT, "Final Rc Heap usage:\n");

  RcDumpHeapUsage ();

  //
  // Execute Late Uncore configuration. Final uncore init must occur
  // before PlatformPostMemoryInit is called.
  //

  LateUncoreInit (Host);


  MrcStatus = PostMemoryInit (Host);


  //
  // Finalize the memory NVRAM CRC
  //
  Host->nvram.mem.Crc16 = 0;
  MrcStatus = CalculateCrc16 (
                (VOID *) &Host->nvram.mem,
                sizeof (Host->nvram.mem),
                &Host->nvram.mem.Crc16
                );
  if (EFI_ERROR (MrcStatus)) {
    Host->nvram.mem.Crc16 = 0xFFFF;
  }
}
