/** @file
  Header file for private PEI PMC Lib.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef _PEI_PMC_SOC_LIB_H_
#define _PEI_PMC_SOC_LIB_H_

#include <PmcSocConfig.h>

/**
  This function enables GBE ModPHY SPD gating.
**/
VOID
PmcGbeModPhyPowerGating (
  VOID
  );

/**
  This function checks if GbE is function disabled
  by static power gating

  @retval GbE device state
**/
BOOLEAN
PmcIsGbeFunctionDisabled (
  VOID
  );

/**
  This function disables GbE device by static power gating
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.
**/
VOID
PmcStaticDisableGbe (
  VOID
  );

/**
  This function enables GbE device by disabling static power gating
  Static power gating disabling takes place after Global Reset, G3 or DeepSx transition.
**/
VOID
PmcEnableGbe (
  VOID
  );

/**
  This function checks if GbE device is supported (not disabled by fuse)

  @retval GbE support state
**/
BOOLEAN
PmcIsGbeSupported (
  VOID
  );

/**
  This function enables all SerailIo devices
  Static power gating disabling takes place after Global Reset, G3 or DeepSx transition.
**/
VOID
PmcEnableSerialIo (
  VOID
  );

/**
  This function disables (static power gating) all SerailIo devices.
  For SerialIo controllers they can be power gated only if all of them are to be disabled.
  They cannot be statically power gated separately.
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.
**/
VOID
PmcStaticDisableSerialIo (
  VOID
  );

/**
  This function checks if all SerialIo devices are statically disabled (static power gating)

  @retval SerialIo disable state
**/
BOOLEAN
PmcIsSerialIoStaticallyDisabled (
  VOID
  );

/**
  This function checks if SerialIo device is supported (not disabled by fuse)

  @retval SerialIo support state
**/
BOOLEAN
PmcIsSerialIoSupported (
  VOID
  );

/**
  This function disables (non-static power gating) HDA device
**/
VOID
PmcDisableHda (
  VOID
  );

/**
  This function disables (non-static power gating) PCIe Root Port

  @param[in] RpIndex        PCIe Root Port Index (0 based)
**/
VOID
PmcDisablePcieRootPort (
  IN UINT32  RpIndex
  );

/**
  This function disables (non-static power gating) SATA

  @param[in]  SataCtrlIndex     SATA controller index
**/
VOID
PmcDisableSata (
  IN UINT32     SataCtrlIndex
  );

/**
  This function checks if SATA device is supported (not disabled by fuse)

  @param[in] SataCtrlIndex SATA controller index

  @retval SATA support state
**/
BOOLEAN
PmcIsSataSupported (
  IN UINT32  SataCtrlIndex
  );

/**
  This function disables (non-static power gating) xHCI
**/
VOID
PmcDisableXhci (
  VOID
  );

/**
  This function disables (non-static power gating) XDCI
**/
VOID
PmcDisableXdci (
  VOID
  );

/**
  This function checks if XDCI device is supported (not disabled by fuse)

  @retval XDCI support state
**/
BOOLEAN
PmcIsXdciSupported (
  VOID
  );

/**
  Check if PMC Extended straps are supported

  @retval  Status of Extended Straps support
**/
BOOLEAN
PmcIsExtendedStrapsSupported (
  VOID
  );

#endif
