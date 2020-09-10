/** @file
  MeChipsetLib.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <PiDxe.h>
#include <Library/IoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/PchRegsPsf.h>
#include <Register/HeciRegs.h>
#include <Library/PmcLib.h>
#include <Library/PsfLib.h>
#include <Register/PchRegs.h>
#include <Register/MeRegs.h>
#include <Library/PsfSocLib.h>

/**
  Initialize Me devices
  For Server this should not be required as HeciControl should do this

  @param[in] WhichDevice          Select of Me device
  @param[in] MmioAddrL32          MMIO address for 32-bit low dword
  @param[in] MmioAddrH32          MMIO address for 32-bit high dword

**/
VOID
MeDeviceInit (
  IN  ME_DEVICE                   WhichDevice,
  IN  UINT32                      MmioAddrL32,
  IN  UINT32                      MmioAddrH32
  )
{
  // For Server this should not be required as HeciControl should do this
}

/**

  @brief
  Enable/Disable Me devices

  @param[in] WhichDevice          Select of Me device
  @param[in] DeviceFuncCtrl       Function control


**/
VOID
MeDeviceControl (
  IN  ME_DEVICE             WhichDevice,
  IN  ME_DEVICE_FUNC_CTRL   DeviceFuncCtrl
  )
{
  UINT64                    PciBaseAdd;

  switch (WhichDevice) {
    case HECI1:
      if (DeviceFuncCtrl == Enabled) {
        PsfEnableHeciDevice (1);
        ClearD0I3Bit (HECI1);
      } else {
        SetD0I3Bit (HECI1);
        PsfDisableHeciDevice (1);
      }
      break;
    case HECI2:
      if (DeviceFuncCtrl == Enabled) {
        PsfEnableHeciDevice (2);
        ClearD0I3Bit (HECI2);
      } else {
        SetD0I3Bit (HECI2);
        PsfDisableHeciDevice (2);
      }
      break;
    case HECI3:
      if (DeviceFuncCtrl == Enabled) {
        PsfEnableHeciDevice (3);
        ClearD0I3Bit (HECI3);
      } else {
        SetD0I3Bit (HECI3);
        PsfDisableHeciDevice (3);
      }
      break;
    case HECI4:
      if (DeviceFuncCtrl == Enabled) {
        PsfEnableHeciDevice (4);
        ClearD0I3Bit (HECI4);
      } else {
        SetD0I3Bit (HECI4);
        PsfDisableHeciDevice (4);
      }
      break;
    case IDER:
      PciBaseAdd = PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, DEFAULT_PCI_BUS_NUMBER_PCH, ME_DEVICE_NUMBER, IDER_FUNCTION_NUMBER, 0);
      PciSegmentWrite32 (PciBaseAdd + R_ME_PMCSR, (UINT32) V_ME_PMCSR);
      PsfDisableIderDevice ();
      DeviceFuncCtrl = Disabled;
      break;
    case SOL:
      if (DeviceFuncCtrl == Enabled) {
        PsfEnableSolDevice ();
      } else {
        PciBaseAdd = PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, DEFAULT_PCI_BUS_NUMBER_PCH, ME_DEVICE_NUMBER, SOL_FUNCTION_NUMBER, 0);
        PciSegmentWrite32 (PciBaseAdd + R_ME_PMCSR, (UINT32) V_ME_PMCSR);
        PsfDisableSolDevice ();
      }
      break;
    default:
      DEBUG((DEBUG_ERROR, "[ME] ERROR: Unknown device %d operation (%d)\n", (UINT8)WhichDevice, DeviceFuncCtrl));
      ASSERT (FALSE);
      return;
  }
  if (DeviceFuncCtrl == Enabled) {
    DEBUG ((DEBUG_INFO, "Enabling"));
  } else {
    DEBUG ((DEBUG_INFO, "Disabling"));
  }
  DEBUG ((
    DEBUG_INFO,
    " CSME device %d:%d:%d\n",
    HECI_BUS,
    HECI_DEV,
    (UINT8) WhichDevice
  ));
}
