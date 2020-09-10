/** @file
  File contains MRC Thermal Hob Structure and Guid
  Structure is used to publish Thermal Data to be consumed in DXE Phase

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
#ifndef _MRC_THERMAL_HOB_H_
#define _MRC_THERMAL_HOB_H_

#include <UncoreCommonIncludes.h>

#pragma pack(1)
typedef struct {
    BOOLEAN DimmValid;                       // True - Dimm Present, False - Dimm not Present
    UINT8   DimmTempThrottleThresholdLow;    // Low throttling value
    UINT8   DimmTempThrottleThresholdMid;    // Mid throttling value
    UINT8   DimmTempThrottleThresholdHigh;   // High throttling value
    UINT8   DimmTempThrottleHysteresis;      // Dimm Hysteresis value
    UINT8   DimmTempOffset;                  // Programmed temperature offset
} DIMM_THROTTLE_VALUES;

typedef struct {
  DIMM_THROTTLE_VALUES DimmThrottleValues[MAX_SOCKET][MAX_CH][MAX_DIMM];
} MRC_THERMAL_HOB;

#pragma pack()

extern EFI_GUID gMrcThermalHobGuid;

#endif
