/** @file
  Register definition for ME Devices.

  Conventions:

  - Prefixes:
    - Definitions beginning with "R_" are registers
    - Definitions beginning with "B_" are bits within registers
    - Definitions beginning with "V_" are meaningful values of bits within the registers
    - Definitions beginning with "S_" are register sizes
    - Definitions beginning with "N_" are the bit position
  - Registers / bits that are different between PCH generations are denoted by
    "_ME_[generation_name]_" in register/bit names.
  - Registers / bits that are specific to PCH-H denoted by "PCH_H_" in register/bit names.
    Registers / bits that are specific to PCH-LP denoted by "PCH_LP_" in register/bit names.
    e.g., "_ME_PCH_H_", "_ME_PCH_LP_"
    Registers / bits names without _PCH_H_ or _PCH_LP_ apply for both H and LP.
  - Registers / bits that are different between SKUs are denoted by "_[SKU_name]"
    at the end of the register/bit names
  - Registers / bits of new devices introduced in a PCH generation will be just named
    as "_ME_" without [generation_name] inserted.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2020 Intel Corporation.

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
#ifndef _ME_REGS_H_
#define _ME_REGS_H_

#include <Library/MeChipsetLib.h>

#define ME_SEGMENT            0
#define ME_BUS                0
#define ME_DEVICE_NUMBER      22
#define HECI_MIN_FUNC         0
#define HECI_MAX_FUNC         5

#define HECI_FUNCTION_NUMBER  0x00
#define HECI2_FUNCTION_NUMBER 0x01
#define IDER_FUNCTION_NUMBER  0x02
#define SOL_FUNCTION_NUMBER   0x03
#define HECI3_FUNCTION_NUMBER 0x04
#define HECI4_FUNCTION_NUMBER 0x05

typedef enum {
  HECI1_DEVICE = 0,
  HECI2_DEVICE = 1,
  HECI3_DEVICE = 4,
  HECI4_DEVICE = 5,
  ISH_HECI     = 8
} HECI_DEVICE;

///
/// Convert to HECI# defined in BWG from Fun#
///
#define HECI_NAME_MAP(a) ((a < 2) ? (a + 1) : (a - 1))

///
/// Default Vendor ID and Device ID
///
#define V_INTEL_VENDOR_ID             0x8086

#define V_ME_HECI_VENDOR_ID           V_INTEL_VENDOR_ID
#define V_ME_SOL_VENDOR_ID            V_INTEL_VENDOR_ID

///
/// Ver1 PCH-N ME Device IDs
///
#define V_ME_VER1_PCH_N_HECI_DEVICE_ID     0x4DE0
#define V_ME_VER1_PCH_N_HECI2_DEVICE_ID    0x4DE1
#define V_ME_VER1_PCH_N_SOL_DEVICE_ID      0x4DE3
#define V_ME_VER1_PCH_N_HECI3_DEVICE_ID    0x4DE4
#define V_ME_VER1_PCH_N_HECI4_DEVICE_ID    0x4DE5

///
/// Ver2 PCH-H/LP ME Device IDs
///
#define V_ME_VER2_PCH_H_HECI_DEVICE_ID     0x43E0
#define V_ME_VER2_PCH_H_HECI2_DEVICE_ID    0x43E1
#define V_ME_VER2_PCH_H_SOL_DEVICE_ID      0x43E3
#define V_ME_VER2_PCH_H_HECI3_DEVICE_ID    0x43E4
#define V_ME_VER2_PCH_H_HECI4_DEVICE_ID    0x43E5

#define V_ME_VER2_PCH_LP_HECI_DEVICE_ID    0xA0E0
#define V_ME_VER2_PCH_LP_HECI2_DEVICE_ID   0xA0E1
#define V_ME_VER2_PCH_LP_SOL_DEVICE_ID     0xA0E3
#define V_ME_VER2_PCH_LP_HECI3_DEVICE_ID   0xA0E4
#define V_ME_VER2_PCH_LP_HECI4_DEVICE_ID   0xA0E5

///
/// Ver3 PCH ME Device IDs
///
#define V_ME_VER3_PCH_HECI_DEVICE_ID       0x1BE0
#define V_ME_VER3_PCH_HECI2_DEVICE_ID      0x1BE1
#define V_ME_VER3_PCH_SOL_DEVICE_ID        0x1BE3
#define V_ME_VER3_PCH_HECI3_DEVICE_ID      0x1BE4
#define V_ME_VER3_PCH_HECI4_DEVICE_ID      0x1BE5

///
/// Ver4 PCH-S/P/M ME Device IDs
///
#define V_ME_VER4_PCH_S_HECI_DEVICE_ID     0x7AE0
#define V_ME_VER4_PCH_S_HECI2_DEVICE_ID    0x7AE1
#define V_ME_VER4_PCH_S_SOL_DEVICE_ID      0x7AE3
#define V_ME_VER4_PCH_S_HECI3_DEVICE_ID    0x7AE4
#define V_ME_VER4_PCH_S_HECI4_DEVICE_ID    0x7AE5

#define V_ME_VER4_PCH_P_HECI_DEVICE_ID     0x51E0
#define V_ME_VER4_PCH_P_HECI2_DEVICE_ID    0x51E1
#define V_ME_VER4_PCH_P_SOL_DEVICE_ID      0x51E3
#define V_ME_VER4_PCH_P_HECI3_DEVICE_ID    0x51E4
#define V_ME_VER4_PCH_P_HECI4_DEVICE_ID    0x51E5

#define V_ME_VER4_PCH_M_HECI_DEVICE_ID     0x54E0
#define V_ME_VER4_PCH_M_HECI2_DEVICE_ID    0x54E1
#define V_ME_VER4_PCH_M_SOL_DEVICE_ID      0x54E3
#define V_ME_VER4_PCH_M_HECI3_DEVICE_ID    0x54E4
#define V_ME_VER4_PCH_M_HECI4_DEVICE_ID    0x54E5


#define R_ME_HFS                      0x40
#define R_ME_HFS_2                    0x48
#define R_ME_PMCSR                    0x54
#define V_ME_PMCSR                    0x03
#define R_ME_HFS_3                    0x60
#define R_ME_HFS_4                    0x64
#define R_ME_HFS_5                    0x68
#define R_ME_HFS_6                    0x6C

#define R_ME_H_GS                     0x4C
#define R_ME_H_GS2                    0x70
#define B_ME_H_GS2_PROXY_PRESENT            BIT8
#define B_ME_H_GS2_STORAGE_OP_IN_PROGRESS   BIT9
#define B_ME_H_GS2_IBB_DATA_READY           BIT10
#define R_ME_HIDM                     0xA0
#define V_ME_HIDM_MSI                 0
#define V_ME_HIDM_SCI                 1
#define V_ME_HIDM_SMI                 2
#define B_ME_HIDM_MODE                (BIT0 | BIT1)
#define B_ME_HIDM_L                   BIT2
#define R_ME_HERS                     0xBC
#define B_ME_EXTEND_REG_ALGORITHM     (BIT3 | BIT2 | BIT1 | BIT0)   // Bits3:0 = 0x0 - SHA1; 0x2 - SHA256; 0x4 - SHA384
#define B_ME_EXTEND_COMPLETE          BIT29
#define B_ME_EXTEND_FEAT_PRESENT      BIT30
#define B_ME_EXTEND_REG_VALID         BIT31
#define V_ME_SHA_1                    0x00
#define V_ME_SHA_256                  0x02
#define V_ME_SHA_384                  0x04
#define R_ME_HER1                     0xC0
#define R_ME_HER2                     0xC4
#define R_ME_HER3                     0xC8
#define R_ME_HER4                     0xCC
#define R_ME_HER5                     0xD0
#define R_ME_HER6                     0xD4
#define R_ME_HER7                     0xD8
#define R_ME_HER8                     0xDC

#define V_ME_PCH_LP_USBR1_PORT_NUMBER 0xA
#define V_ME_PCH_LP_USBR2_PORT_NUMBER 0xB
#define V_ME_PCH_H_USBR1_PORT_NUMBER  0xE
#define V_ME_PCH_H_USBR2_PORT_NUMBER  0xF

///
/// SOL Private CR Space Definitions
///
#define V_ME_SOL_FID    0xB3

///
/// ME-related Chipset Definition
///
#define HeciEnable()    MeDeviceControl (HECI1, Enabled);
#define Heci2Enable()   MeDeviceControl (HECI2, Enabled);
#define Heci3Enable()   MeDeviceControl (HECI3, Enabled);
#define Heci4Enable()   MeDeviceControl (HECI4, Enabled);
#define IderEnable()    MeDeviceControl (IDER, Enabled);
#define SolEnable()     MeDeviceControl (SOL, Enabled);

#define HeciDisable()   MeDeviceControl (HECI1, Disabled);
#define Heci2Disable()  MeDeviceControl (HECI2, Disabled);
#define Heci3Disable()  MeDeviceControl (HECI3, Disabled);
#define IderDisable()   MeDeviceControl (IDER, Disabled);
#define SolDisable()    MeDeviceControl (SOL, Disabled);

#define DisableAllMeDevices() \
  HeciDisable (); \
  Heci2Disable (); \
  Heci3Disable (); \
  IderDisable (); \
  SolDisable ();

///
/// SoL Device Id Definitions
///
#define IS_VER1_PCH_SOL_DEVICE_ID(DeviceId) \
  ( \
    (DeviceId == V_ME_VER1_PCH_N_SOL_DEVICE_ID) \
  )

#define IS_VER2_PCH_SOL_DEVICE_ID(DeviceId) \
  ( \
    (DeviceId == V_ME_VER2_PCH_H_SOL_DEVICE_ID) || \
    (DeviceId == V_ME_VER2_PCH_LP_SOL_DEVICE_ID) \
  )

#define IS_VER3_PCH_SOL_DEVICE_ID(DeviceId) \
  ( \
    (DeviceId == V_ME_VER3_PCH_SOL_DEVICE_ID) \
  )

#define IS_VER4_PCH_SOL_DEVICE_ID(DeviceId) \
  ( \
    (DeviceId == V_ME_VER4_PCH_S_SOL_DEVICE_ID) || \
    (DeviceId == V_ME_VER4_PCH_P_SOL_DEVICE_ID) || \
    (DeviceId == V_ME_VER4_PCH_M_SOL_DEVICE_ID) \
  )

#define IS_PCH_SOL_DEVICE_ID(DeviceId) \
  ( \
    IS_VER1_PCH_SOL_DEVICE_ID(DeviceId) || \
    IS_VER2_PCH_SOL_DEVICE_ID(DeviceId) || \
    IS_VER3_PCH_SOL_DEVICE_ID(DeviceId) || \
    IS_VER4_PCH_SOL_DEVICE_ID(DeviceId) \
  )

#endif
