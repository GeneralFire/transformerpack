/** @file
  Definitions for Voltage Regulator Serial VID (SVID) programming

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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

#ifndef _VR_SERIAL_VID_H_
#define _VR_SERIAL_VID_H_

//
// VR12 VID table encodings for 5 mV and 10mV step VID
// These values are taken from VID Table found in:
// "VR12.5 Pulse Width Modulation (PWM) Specification" Rev 1.5
// Intel Resource & Design Center document ID 564571
// https://www.intel.com/content/www/us/en/secure/design/internal/content-details.html?DocID=564571
//
#define SVID_DATA_110V_RAMP_ENCODING_VR12                                0xAB
#define SVID_DATA_110V_RAMP_ENCODING_VR12_5                              0x3D
#define SVID_DATA_120V_RAMP_ENCODING_VR12                                0xBF
#define SVID_DATA_120V_RAMP_ENCODING_VR12_5                              0x47

//
// VR13 VID table encodings for 5 mV and 10mV step VID
// These values are taken from VID Table found in:
// "VR13. VR13.HC and VR14 Pulse Width Modulation, Server VR Vendor PWM Enabling Specification" Rev 1.85
// Intel Resource & Design Center document ID 544905
// https://www.intel.com/content/www/us/en/secure/design/internal/content-details.html?DocID=544905
//
#define SVID_DATA_110V_RAMP_ENCODING_VR13_5                              0xAB
#define SVID_DATA_110V_RAMP_ENCODING_VR13_10                             0x3D
#define SVID_DATA_120V_RAMP_ENCODING_VR13_5                              0xBF
#define SVID_DATA_120V_RAMP_ENCODING_VR13_10                             0x47

//
// VDD SVID encoding lookup
//
typedef struct {
  UINT8               ddrVoltageValue;
  UINT32              Vr12Encoding;
  UINT32              Vr125Encoding;
  UINT32              Vr135Encoding;
  UINT32              Vr1310Encoding;
  CHAR8               *string_ptr;
} VddValues;

//
// 5mV and 10mV VDD SVID encoding formula
// Derived from VID Tables in VR12/VR13/VR14 PWM Specs (see document references above)
//
#define VDD_SVID_ENCODE_5MV(_VddValue)  (((_VddValue - 250) / 5) + 1)
#define VDD_SVID_ENCODE_10MV(_VddValue) (((_VddValue - 500) / 10) + 1)

#endif // ifndef _VR_SERIAL_VID_H_
