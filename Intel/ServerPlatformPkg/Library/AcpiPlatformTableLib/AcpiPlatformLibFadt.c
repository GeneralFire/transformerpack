/** @file
  ACPI Platform Driver Hooks

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2018 Intel Corporation. <BR>

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

//
// Statements that include other files
//
#include "AcpiPlatformLibLocal.h"
#include <Library/PchPcieRpLib.h>

extern UINT8                 mKBPresent;
extern UINT8                 mMousePresent;
extern SOCKET_PROCESSORCORE_CONFIGURATION   mSocketProcessorCoreConfiguration;
extern SOCKET_POWERMANAGEMENT_CONFIGURATION mSocketPowermanagementConfiguration;


EFI_STATUS
PatchFadtTable(
   IN OUT   EFI_ACPI_COMMON_HEADER  *Table
   )
{
  UINT16                                    LegacyDevice;
  // APTIOV_SERVER_OVERRIDE_RC_START
//  EFI_LPC_POLICY_PROTOCOL                   *LpcPolicy;
  // APTIOV_SERVER_OVERRIDE_RC_END
  EFI_ACPI_6_2_FIXED_ACPI_DESCRIPTION_TABLE *FadtHeader;
  EFI_STATUS                                Status;
  UINT8                                     PcieGlobalAspm;
  Status = GetOptionData (&gEfiSocketIioVariableGuid, OFFSET_OF(SOCKET_IIO_CONFIGURATION, PcieGlobalAspm), &PcieGlobalAspm, sizeof(UINT8));
  if (EFI_ERROR (Status)) {
    PcieGlobalAspm = 0x2;
  }

  //
  // Patch FADT for legacy free
  //
  LegacyDevice  = 0;
  FadtHeader    = (EFI_ACPI_6_2_FIXED_ACPI_DESCRIPTION_TABLE *) Table;
  //
  // APTIOV_SERVER_OVERRIDE_RC_START : Remove LpcPlatform component. Pilot3 has a COM port, so setting the Legacy device
  // flag in FADT/FACP.
  //
  #if 0
  Status        = gBS->LocateProtocol (&gEfiLpcPolicyProtocolGuid, NULL, &LpcPolicy);

  if (!EFI_ERROR (Status)) {
    if (LpcPolicy->PolicyCom.Enable[0] ||
        LpcPolicy->PolicyCom.Enable[1] ||
        LpcPolicy->PolicyParallel.Enable[0] ||
        LpcPolicy->PolicyFloppy.Enable[0]
        ) {
  #endif
      LegacyDevice |= 1;      // set LEGACY_DEVICES FADT flag
  #if 0
    }

    if (LpcPolicy->PolicyKeyboard.Enable) {
      mKBPresent     = 1;      // copy later to ACPI NVS
      LegacyDevice |= 2;      // set 8042 FADT flag
    }

    if (LpcPolicy->PolicyMouse.Enable) {
      mMousePresent  = 1;      // copy later to ACPI NVS
      LegacyDevice |= 2;      // set 8042 FADT flag
    }
  }
  #endif

  //
  // APTIOV_SERVER_OVERRIDE_RC_END : Remove LpcPlatform component. Pilot3 has a COM port, so setting the Legacy device
  //

  //
  // Control of setting ASPM disabled bit in FADT
  //
  switch (mSocketPowermanagementConfiguration.NativeAspmEnable) {

  case 0:
    LegacyDevice |= (1 << 4);
    break;

  case 1:
    LegacyDevice &= ~(1 << 4);
    break;

  case 2:
    if (!PchIsPcieNativeAspmSupported () || (PcieGlobalAspm == 0)) {
      LegacyDevice |= (1 << 4);
    } else {
      LegacyDevice &= ~(1 << 4);
    }
    break;

  default:
    LegacyDevice &= ~(1 << 4);
    DEBUG ((EFI_D_ERROR, "\n Native ASPM = %d is not valid (expected values are 0, 1, 2). \n", mSocketPowermanagementConfiguration.NativeAspmEnable ));
    ASSERT (0);
    break;
  }
// APTIOV_SERVER_OVERRIDE_RC_START : Do not overwrite IaPcBootArch, append the flags.
  FadtHeader->IaPcBootArch |= LegacyDevice;
// APTIOV_SERVER_OVERRIDE_RC_END : Do not overwrite IaPcBootArch, append the flags.
  FadtHeader->Flags |= (mSocketProcessorCoreConfiguration.ForcePhysicalModeEnable) ? EFI_ACPI_6_2_FORCE_APIC_PHYSICAL_DESTINATION_MODE : 0;
  //FadtHeader->Flags |= EFI_ACPI_6_2_FORCE_APIC_CLUSTER_MODEL;

  return EFI_SUCCESS;
}
