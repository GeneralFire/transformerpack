/** @file
  Header file for CpuDmiInfoLib.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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
#ifndef CPU_DMI_INFO_LIB_H_
#define CPU_DMI_INFO_LIB_H_

#define LANE_MULTIPLIER                    0x1000

#include <CpuAccess.h>
/**
  Determine if PCH Link is DMI/OPI

  @retval TRUE                    DMI
  @retval FALSE                   OPI
**/
BOOLEAN
IsPchLinkDmi (
  VOID
  );

/**
  Returns the number of DMI lanes for current CPU

  @retval UINT8       Number of DMI lanes
**/
UINT8
GetMaxDmiLanes (
  );

/**
  Returns the number of DMI bundles for current CPU

  @retval UINT8       Number of DMI bundles
**/
UINT8
GetMaxDmiBundles (
  );
/**
  Checks if Cpu Dmi is Enabled

  @retval TRUE     Cpu Dmi is Enabled
  @retval FALSE    Cpu Dmi is not Enabled
**/
BOOLEAN
IsCpuDmiEnabled (
  VOID
  );
/**
  Checks if Cpu Dmi Gen4 is Enabled

  @retval TRUE     Cpu Dmi Gen4 is Enabled
  @retval FALSE    Cpu Dmi Gen4 is not Enabled
**/
BOOLEAN
IsDmiGen4Supported (
  VOID
  );

#endif // CPU_DMI_INFO_LIB_H_
