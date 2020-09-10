/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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
#include <ScratchpadList.h>
#include <RcRegs.h>

#include <Library/DebugLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/IoLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/PmcLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BasicIpBlockServicesLib.h>
#include <Library/HobLib.h>
#include <Library/ReportStatusCodeLib.h>

#include <Guid/PlatformInfo.h>

//
// This is required due to PmcLib having multiple API headers
//

#ifdef CDF_SC_FLAG
#include <Private/Library/PmcPrivateLib.h>
#endif // #ifdef CDF_SC_FLAG

//
// This is required due to PmcLib having multiple API headers
//

#ifdef CDF_SC_FLAG
#include <Private/Library/PmcPrivateLib.h>
#endif // #ifdef CDF_SC_FLAG

/**

  Force setup default. TODO - This function does
  not appear to force defaults, need to determine 
  what is does and rename.

  @param[in] Host      - Pointer to sysHost

  @retval None

**/

VOID
ForceSetupDefault (
  IN PSYSHOST  Host
  )
{
  UINT8                           SocketId = 0;
  BIOS_SCRATCHPAD5_STRUCT         Sp5;

  Sp5.Data = UsraCsrRead (SocketId, 0, BIOSSCRATCHPAD5_UBOX_MISC_REG);
  DEBUG((EFI_D_INFO, "Scratchpad_Debug ForceSetupDefault, Scratchpad5 = %d\n", Sp5.Data));
  if (Sp5.Bits.ClearCmos == 1) {


    DEBUG((EFI_D_INFO, "Advanced Debug Jumper set - Force Setup Defaults\n"));


    //
    // Clear Cmos Bad bit here.
    //

    Sp5.Bits.ClearCmos = 0;
    UsraCsrWrite (SocketId, 0, BIOSSCRATCHPAD5_UBOX_MISC_REG, Sp5.Data);


    DEBUG((EFI_D_INFO, "Scratchpad_Debug ForceSetupDefault, ForceToCmos Path: Scratchpad5 %d\n", Sp5.Data));


    PmcClearRtcPowerFailureStatus();

  }

} // ForceSetupDefault

/**

  Get the platform type from the PlatformInfo HOB.

  @param[in] Host - pointer to sysHost

  @retval UINT8 platformtype.

**/
UINT8
LocalGetPlatformType (
  IN PSYSHOST  Host
  )
{

  EFI_PLATFORM_INFO *PlatformInfo;
  EFI_HOB_GUID_TYPE *GuidHob;

  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfo);
  return PlatformInfo->BoardId;


} // LocalGetPlatformType

/**

  Initialize Memory data

  @param[in] Host - Pointer to SysHost structure

**/
VOID
InitializeMemoryData (
  IN PSYSHOST  Host
  )
{
  EFI_STATUS        Status;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  DEBUG ((EFI_D_INFO, "InitializeMemoryData entered\n"));

  Host->var.common.pmBase = PmcGetAcpiBase ();

  Host->nvram.common.platformType = LocalGetPlatformType (Host);


  {
    EFI_BOOT_MODE     BootMode;

    Status = PcdSet8S (PcdPlatformType, Host->nvram.common.platformType);
    ASSERT_EFI_ERROR (Status);
    //
    // Report Status Code EFI_SW_PS_PC_INSTALL_PEI_MEMORY
    //
    REPORT_STATUS_CODE (
      EFI_PROGRESS_CODE,
      (EFI_SOFTWARE_PEI_SERVICE  | EFI_SW_PS_PC_INSTALL_PEI_MEMORY)
      );

    //
    // Determine boot mode
    //

    Status = PeiServicesGetBootMode (&BootMode);
    ASSERT_EFI_ERROR(Status);

    if (BootMode == BOOT_ON_S3_RESUME) {
      SetSysBootMode (S3Resume);
    } else {
      SetSysBootMode (NormalBoot);
    }

    if (PcdGetBool (PcdDfxAdvDebugJumper))  {
      ForceSetupDefault (Host);
    }

  }


  //
  // Call platform hook for platform-specific init. Note that
  // failing to locate the platform hooks interface may be 
  // expected, such as in the case of FSP.
  //

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformInitializeMemoryData (Host);
  }

  DEBUG ((EFI_D_INFO, "InitializeMemoryData exiting\n"));

} // InitializeMemoryData


