/** @file
  SCS SD controller definition.

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

#ifndef _SCS_SD_CONTROLLER_H_
#define _SCS_SD_CONTROLLER_H_

#include <Iosf2OcpPort.h>
#include <ScsConfig.h>
#include <ScsInfoHob.h>

typedef struct _SCS_SDCARD_HANDLE  SCS_SDCARD_HANDLE;
typedef struct _SCS_EMMC_HANDLE  SCS_EMMC_HANDLE;

/**
  Called as a last step in SD card controller disable flow.

  @param[in] ScsSdCardHandle  Handle
**/
typedef
VOID
(*SCS_SDCARD_DISABLE) (
  IN SCS_SDCARD_HANDLE  *ScsSdCardHandle
  );

/**
  Called after disabling BAR1 in IOSF2OCP.

  @param[in] ScsSdCardHandle  Handle
**/
typedef
VOID
(*SCS_SDCARD_BAR1_DISABLE) (
  IN SCS_SDCARD_HANDLE  *ScsSdCardHandle
  );

/**
  Called after SD card MMIO init with the intention to enable SD card
  phy.

  @param[in] ScsSdCardHandle  Handle
**/
typedef
VOID
(*SCS_SDCARD_PHY_ENABLE) (
  IN SCS_SDCARD_HANDLE  *ScsSdCardHandle
  );

typedef struct {
  SCS_SDCARD_DISABLE       Disable;
  SCS_SDCARD_BAR1_DISABLE  Bar1Disable;
  SCS_SDCARD_PHY_ENABLE    PhyEnable;
} SCS_SDCARD_CALLBACKS;

/**
  Called as a last step in eMMC controller disable flow.

  @param[in] ScsEmmcHandle  Handle
**/
typedef
VOID
(*SCS_EMMC_DISABLE) (
  IN SCS_EMMC_HANDLE  *ScsEmmcHandle
  );

/**
  Called after disabling BAR1 in IOSF2OCP.

  @param[in] ScsEmmcHandle  Handle
**/
typedef
VOID
(*SCS_EMMC_BAR1_DISABLE) (
  IN SCS_EMMC_HANDLE  *ScsEmmcHandle
  );

/**
  Called after eMMC MMIO init with the intention to enable eMMC
  phy.

  @param[in] ScsEmmcHandle  Handle
**/
typedef
VOID
(*SCS_EMMC_PHY_ENABLE) (
  IN SCS_EMMC_HANDLE  *ScsEmmcHandle
  );

typedef struct {
  SCS_EMMC_DISABLE       Disable;
  SCS_EMMC_BAR1_DISABLE  Bar1Disable;
  SCS_EMMC_PHY_ENABLE    PhyEnable;
} SCS_EMMC_CALLBACKS;

typedef struct {
  UINT32  CapReg1;
  UINT32  CapReg2;
} SCS_SD_CAPS;

typedef struct {
  SCS_SD_CAPS  Caps;
  SCS_SD_DLL   DefaultDll;
  UINT8        IntPin;
  UINT8        Irq;
} SCS_SD_SOC_CONFIG;

typedef struct {
  IOSF2OCP_PORT  *Iosf2OcpPort; // Iosf2OcpPort to which SD controller belongs.
  UINT64         PciCfgBase; // PCI configuration base
  UINTN          MmioBase; // MMIO for usage in BAR access
} SCS_SD_CONTROLLER;

struct _SCS_SDCARD_HANDLE {
  SCS_SD_CONTROLLER     Controller; // Describes controller location
  SCS_SD_SOC_CONFIG     SocConfig; // Describes SoC specific configuration
  SCS_SDCARD_CONFIG     *Config; // Describes platform configuration
  SCS_SDCARD_CALLBACKS  Callbacks;  // SD card callbacks
  SCS_SDCARD_INFO       *SdCardInfo; // Contains initialization status
};

struct _SCS_EMMC_HANDLE {
  SCS_SD_CONTROLLER   Controller; // Describes controller location
  SCS_SD_SOC_CONFIG   SocConfig; // Describes SoC specific configuration
  SCS_EMMC_CONFIG     *Config; // Describes platform configuration
  SCS_EMMC_CALLBACKS  Callbacks; // eMMC callbacks
  SCS_EMMC_INFO       *EmmcInfo; // Contains initialization status
};

#endif
