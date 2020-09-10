/** @file
  PMC SoC configuration

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

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef _PMC_SOC_CONFIGURATION_H_
#define _PMC_SOC_CONFIGURATION_H_

typedef enum {
  AdrSinglePhase = 0,
  AdrDualPhase
} ADR_PHASE_TYPE;

typedef enum {
  AdrGpioB = 0,
  AdrGpioC
} ADR_GPIO;

typedef struct {
  ADR_PHASE_TYPE  AdrPhaseType;
  ADR_GPIO        AdrGpio;
  //
  // On some designs ADR_GEN_CFG has been moved in the HW.
  // Set this to if ADR_GEN_CFG is located at 0x1908
  //
  BOOLEAN         AdrGenCfgMoved;
} PMC_ADR_SOC_CONFIG;

typedef struct {
  BOOLEAN  CppmCgInterfaceVersion;
  BOOLEAN  LpmSupported;
  UINT8    LpmInterfaceVersion;
  BOOLEAN  OsIdleSupported;
  BOOLEAN  TimedGpioSupported;
  UINT32   CpuIovrRampTime;
  BOOLEAN  PsOnSupported;
  BOOLEAN  ModPhySusPgSupported;
  UINT8    SciIrq;
  BOOLEAN  FabricPowerGatingCppmQualificationEnable;
  BOOLEAN  EspiBoot;
  BOOLEAN  UsbDbcConnected;
  UINT32   Usb3LanesConnectedBitmask;
  BOOLEAN  DisableIosfSbClockGating;
  BOOLEAN  SkipModPhyGatingPolicy;
} PMC_SOC_CONFIG;

typedef struct {
  BOOLEAN  OverrideFetRampTime;
  UINT8    FetRampTime;
  UINT8    IsFetRampTime;
  UINT16   FuseDownloadDelayUs;
} PMC_FIVR_SOC_CONFIG;

#endif
