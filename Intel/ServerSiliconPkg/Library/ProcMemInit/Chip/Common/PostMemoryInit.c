/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <PiPei.h>

#include <SysHost.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/KtiApi.h>
#include <Library/UsraCsrLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemoryInstallLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/BasicIpBlockServicesLib.h>
#include <Library/MeUmaLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Guid/MrcPlatformHooksGuid.h>

#include <RcRegs.h>
#include <ScratchpadList.h>

/**

  Perform post memory init functions, install memory, etc.

  @param[in] Host   - Pointer to syshost structure

  @retval EFI_SUCCESS

**/

EFI_STATUS
EFIAPI
PostMemoryInit (
  IN PSYSHOST Host
  )
{
  EFI_STATUS               Status;
  EFI_STATUS               PpiStatus;
  BIOS_SCRATCHPAD0_STRUCT  Sp0;
  BIOS_SCRATCHPAD1_STRUCT  Sp1;
  BOOLEAN FpgaExist = FALSE;
  UMA_MEMORY_INFORMATION MeMemoryInformation;
  UMA_MEMORY_INFORMATION IeMemoryInformation;
  UINT8      socket;
  BootMode   SysBootMode;
  SYS_SETUP  *Setup;
  MEMORY_POLICY_PPI *MemSetupPpi;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  Status = PeiServicesLocatePpi (
             &gMemoryPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &MemSetupPpi
             );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return Status;
  }

  Setup = MemSetupPpi->SysSetup;

  Status = EFI_SUCCESS;
  SysBootMode = GetSysBootMode ();

  if (HybridSystemLevelEmulationEnabled () && !IsSiliconWorkaroundEnabled ("S14010451750")) {
    // SKIP_WARM_RESET
    SetSysResetRequired(0);     //force to don't do a warm reset again if we try to skip warm reset. debug later why resetRequired is not 0
  }

  //
  // Post-MRC Housekeeping
  //
  if (Status == EFI_SUCCESS && !GetSysResetRequired()) {

    if (SysBootMode == S3Resume && Host->var.mem.subBootMode != AdrResume) {
      //
      // Install S3 memory
      //
      Status = InstallS3Memory (Host);
      ASSERT_EFI_ERROR (Status);
    } else {
        //
        // Set BIT1 for SSPAD0 to indicate the next reset is a warm reset.  If this bit
        // goes back to 0, it means the power was fully cycled and the next boot will be
        // a cold reset.
        //
        for (socket = 0; socket < MAX_SOCKET; socket++) {
          if (IfFpgaActive (socket)) {
            FpgaExist = TRUE;
            break;
          }
        }
        for (socket = 0; socket < MAX_SOCKET; socket++) {
          if (SocketPresent (socket)) {
            Sp1.Data = UsraCsrRead (GetSysSbspSocketIdNv (), 0, BIOSSCRATCHPAD1_UBOX_MISC_REG);
            if ((FpgaExist && (Sp1.Bits.FpgaBitStreamLoaded == 1)) || (!FpgaExist)) {
              Sp0.Data = UsraCsrRead (socket, 0, Host->var.mem.memChipPolicy.BiosStickyScratchPad0);
              DEBUG((EFI_D_INFO, "Scratchpad_Debug PlatformPostMemoryInit , set scratchpad0 bit 1\n"));
              //
              // Set BIT1, indicating MRC is done and the next reset is a warm reset.
              //
              Sp0.Bits.WarmBoot = 1;
              UsraCsrWrite (socket, 0, Host->var.mem.memChipPolicy.BiosStickyScratchPad0, Sp0.Data);
            }
            // else is for a debug statement only
            else {
              DEBUG ((EFI_D_INFO, "Scratchpad_Debug PlatformPostMemoryInit, not setting scratchpad0 bit 1\n"));
            }
          }
        }

        //
        // Save socket bit map to NVRAM for cold fast boot
        //
        if (SysBootMode == NormalBoot && (
              Host->var.mem.subBootMode == ColdBoot ||
              Host->var.mem.subBootMode == NvDimmResume)) {
          Host->nvram.mem.socketBitMap = GetSocketPresentBitMap ();
        }

        //
        // Install EFI memory HOBs
        //
        DEBUG ((EFI_D_INFO, "Install EFI Memory\n"));
        Status = InstallEfiMemory (Host);
        ASSERT_EFI_ERROR (Status);
      }
    }
    DEBUG ((EFI_D_INFO, "MRC status = %r\n", Status));

  MeMemoryInformation.NcMemLowBaseAddr = Host->nvram.common.MeNcMemLowBaseAddr.Data;
  MeMemoryInformation.NcMemHighBaseAddr = Host->nvram.common.MeNcMemHighBaseAddr.Data;
  MeMemoryInformation.NcMemLowLimit = Host->nvram.common.MeNcMemLowLimit.Data;
  MeMemoryInformation.NcMemHighLimit = Host->nvram.common.MeNcMemHighLimit.Data;
  IeMemoryInformation.NcMemLowBaseAddr = Host->nvram.common.IeNcMemLowBaseAddr.Data;
  IeMemoryInformation.NcMemHighBaseAddr = Host->nvram.common.IeNcMemHighBaseAddr.Data;
  IeMemoryInformation.NcMemLowLimit = Host->nvram.common.IeNcMemLowLimit.Data;
  IeMemoryInformation.NcMemHighLimit = Host->nvram.common.IeNcMemHighLimit.Data;
  MeUmaSendDramInitDone (
    Status,
    Host->var.mem.memSize,
    Host->var.mem.previousBootError,
    Host->var.mem.subBootMode,
    &MeMemoryInformation,
    &IeMemoryInformation
    );


  PpiStatus = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (PpiStatus)) {
    MrcPlatformHooksInterface->PlatformPostMemoryInit (Host);
  }

  return Status;

} // PostMemoryInit
