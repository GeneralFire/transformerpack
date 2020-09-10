/** @file
  SCS UFS controller definition.

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

#ifndef _SCS_UFS_CONTROLLER_H_
#define _SCS_UFS_CONTROLLER_H_

#include <Iosf2OcpPort.h>
#include <ScsConfig.h>
#include <ScsInfoHob.h>
#include <Library/PchPcrLib.h> // PCH_SBI_PID definition

typedef struct _SCS_UFS_HANDLE SCS_UFS_HANDLE;

typedef struct {
  PCH_SBI_PID   PortId;
  UINT8         LaneMask;
} SCS_UFS_MMP;

/**
  Called as a last step in UFS controller disable flow.

  @param[in] ScsUfsHandle  Handle
**/
typedef
VOID
(*SCS_UFS_DISABLE) (
  IN SCS_UFS_HANDLE  *ScsUfsHandle
  );

/**
  Called after disabling BAR1 in IOSF2OCP.

  @param[in] ScsUfsHandle  Handle
**/
typedef
VOID
(*SCS_UFS_BAR1_DISABLE) (
  IN SCS_UFS_HANDLE  *ScsUfsHandle
  );

typedef struct {
  SCS_UFS_DISABLE             Disable;
  SCS_UFS_BAR1_DISABLE        Bar1Disable;
} SCS_UFS_CALLBACKS;

typedef struct {
  BOOLEAN  IsBootMedium;
  UINT8    NumOfLanes;
  UINT8    IntPin;
  UINT8    Irq;
} SCS_UFS_SOC_CONFIG;

typedef struct {
  IOSF2OCP_PORT  *Iosf2OcpPort; // Iosf2OcpPort to which UFS controller belongs.
  SCS_UFS_MMP    *Mmp; // MMP instance used by this controller
  UINT64         PciCfgBase; // PCI configuration base
  UINTN          MmioBase; // MMIO for usage in BAR access
} SCS_UFS_CONTROLLER;

struct _SCS_UFS_HANDLE {
  SCS_UFS_CONTROLLER         Controller; // Describes UFS controller location
  SCS_UFS_SOC_CONFIG         SocConfig; // Describes SoC specific config
  SCS_UFS_CONTROLLER_CONFIG  *Config; // Describes platform config
  SCS_UFS_CALLBACKS          Callbacks; // Set of SoC callbacks
  SCS_UFS_INFO               *UfsInfo; // Contains initialization status
};

#endif
