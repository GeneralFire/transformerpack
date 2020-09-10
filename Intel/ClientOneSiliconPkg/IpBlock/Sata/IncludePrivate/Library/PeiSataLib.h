/** @file
  SATA routines

@copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation.

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

@par Specification
**/
#ifndef _PEI_SATA_LIB_H_
#define _PEI_SATA_LIB_H_

#include <SataCtrl.h>
#include <SataConfig.h>

/**
  Internal SATA policy options
**/
typedef struct {
  /**
    Description: Indicate if remapping is supported
    Possible values: TRUE or FALSE
  **/
  BOOLEAN               RemappingSupported;
  /**
    Description: RST mode supported
    Possible values: RST_MODE options
  **/
  RST_MODE              SataSupportedRstMode;
  /**
    Description: Indicate if storage remapping is enabled on SATA controller
    Possible values: TRUE or FALSE
  **/
  BOOLEAN               StorageRemappingEnabled;
  /**
    Description: Disables SATA SRAM parity check
    Possible values: TRUE or FALSE
  **/
  BOOLEAN               SataSramParityCheckDisable;
  /**
    Possible values: TRUE or FALSE
  **/
  BOOLEAN               SirC4Programming;
  /**
    Description: Indicate if SATA controller supports power gating features
    Possible values: TRUE or FALSE
  **/
  BOOLEAN               SataPowerGatingSupported;
  /**
    Description: Skips SATA port clock disable during SATA controller disable flow.
    Possible values: TRUE or FALSE
  **/
  BOOLEAN               SataSkipPortClockDisable;
  /**
    Description: Indicates whether PsOn feature is enabled on the system.
    PsOn is a feature that will cut the primary ATX rail
    and transition all SATA ports into D3Cold.
    Possible values: TRUE or FALSE
  **/
  BOOLEAN               SataPsOnSupported;
  /**
    Description: SATA controller oscylator frequency
    Possible values: SATA_SOSC_CLK_FREQ options
  **/
  SATA_SOSC_CLK_FREQ    SataOscClkFreq;
  /**
    Description: SATA controller interrupt pin
    Possible values: 0 - 255
  **/
  UINT8                 InterruptPin;
} SATA_PRIVATE_CONFIG;

/**
  Checks if SATA controller has lanes connected

  @param[in]  SataController    Pointer to SATA Controller structure
  @param[in]  Port              Port number (0 based) to be examined

  @retval TRUE if SATA controller has lanes assigned, otherwise FALSE
**/
typedef
BOOLEAN
(*SATA_PHY_CONNECTED) (
  IN  SATA_CONTROLLER   *SataController,
  IN  UINT8             Port
  );

/**
  Configures GPIO pins for SATA DevSlp

  @param[in]  SataController    Pointer to SATA Controller structure
  @param[in]  Port              Port number (0 based) for Dev Slp enable
  @param[in]  ResetType         GPIO reset type (see GPIO_RESET_CONFIG)

  @retval EFI_STATUS      Status returned by worker function
**/
typedef
EFI_STATUS
(*SATA_DEV_SLP_ENABLE) (
  IN  SATA_CONTROLLER   *SataController,
  IN  UINT8             Port,
  IN  UINT32            ResetType
  );

/**
  Configures GPIO pins for SATA Hot Plug

  @param[in]  SataController    Pointer to SATA Controller structure
  @param[in]  Port              Port number (0 based) for Hot Plug enable

  @retval EFI_STATUS      Status returned by worker function
**/
typedef
EFI_STATUS
(*SATA_HOTPLUG_ENABLE) (
  IN  SATA_CONTROLLER    *SataController,
  IN  UINT8              Port
  );

/**
  Configures GPIO pins for SATA Serial GPIO

  @param[in]  SataController     Pointer to SATA Controller structure

  @retval EFI_STATUS      Status returned by worker function
**/
typedef
EFI_STATUS
(*SATA_SGPIO_ENABLE) (
  IN  SATA_CONTROLLER    *SataController
  );

/**
  Configures GPIO pins for SATA LED

  @param[in]  SataController     Pointer to SATA Controller structure

  @retval EFI_STATUS      Status returned by worker function
**/
typedef
EFI_STATUS
(*SATA_LED_ENABLE) (
  IN  SATA_CONTROLLER    *SataController
  );

/**
  Disables SATA controller in power controller and fabric

  @param[in]  SataController     Pointer to SATA Controller structure

**/
typedef
VOID
(*SATA_CTRL_DISABLE) (
  IN  SATA_CONTROLLER    *SataController
  );

/**
  Enables SATA test mode in PHY

  @param[in]  SataController     Pointer to SATA Controller structure

**/
typedef
VOID
(*SATA_TEST_MODE_ENABLE) (
  IN  SATA_CONTROLLER    *SataController
  );

/**
  SATA callbacks
  List of function pointers can passed to IB Block driver
**/
typedef struct {
  SATA_PHY_CONNECTED          SataIsPhyConnected;
  SATA_TEST_MODE_ENABLE       SataTestModeEnable;
  SATA_DEV_SLP_ENABLE         SataDevSleepEnable;
  SATA_HOTPLUG_ENABLE         SataHotPlugEnable;
  SATA_SGPIO_ENABLE           SataSgpioEnable;
  SATA_LED_ENABLE             SataLedEnable;
  SATA_CTRL_DISABLE           SataControllerDisable;
} SATA_CALLBACK;

  /**
    SATA controller structure
    Stores all data necessary to initialize SATA IP block
  **/
typedef struct {
  SATA_CONTROLLER             *Controller;
  SATA_CONFIG                 *SataConfig;
  SATA_PRIVATE_CONFIG         *PrivateConfig;
  SATA_CALLBACK               *Callback;
  /**
    Temporary Bar
  **/
  UINT64                      TempBar;
} SATA_HANDLE;

/**
  Initializes SATA device

  @param[in]  Sata          Pointer to SATA controller structure

**/
VOID
ConfigureSata (
  IN  SATA_HANDLE   *Sata
  );

/**
  Disable SATA Controller if needed after RST Remapping code

  @param[in]  Sata          Pointer to SATA controller structure

**/
VOID
ConfigureSataAfterRst (
  IN  SATA_HANDLE   *Sata
  );
#endif // _PEI_SATA_LIB_H_
