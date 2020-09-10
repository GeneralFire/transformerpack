/** @file
  ProcSmbChipCommon.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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
**/

#ifndef  _PROC_SMB_CHIP_COMMON_H_
#define  _PROC_SMB_CHIP_COMMON_H_

#include <Memory/MemDefaults.h>

//
// Available SMBus clock periods to be programmed.
//
typedef enum {
  SmbClk100K = SMB_CLK_100K,     // 100 Khz in I2C mode; 4Mhz in I3C mode
  SmbClk400K = SMB_CLK_400K,     // 400 Khz in I2C mode; 6Mhz in I3C mode
  SmbClk700K = SMB_CLK_700K,     // 700 Khz in I2C mode; 8Mhz in I3C mode
  SmbClk1M   = SMB_CLK_1M,       // 1 Mhz in I2C mode; 10Mhz in I3C mode
  SmbClkPeriodMax
} SMB_CLOCK_FREQUENCY;

// Clock Period values (for SMBUS freq) are based on 100Mhz FXCLK
#define SMB_100K_CLK_PERIOD    1000  // Clock period to be programmed for 100Khz
#define SMB_100K_CLK_OFFSET    35    // Clock offset to be programmed for 100Khz
#define SMB_400K_CLK_PERIOD    250   // Clock period to be programmed for 400Khz
#define SMB_400K_CLK_OFFSET    35    // Clock offset to be programmed for 400Khz
#define SMB_700K_CLK_PERIOD    144   // Clock period to be programmed for 700Khz
#define SMB_700K_CLK_OFFSET    30    // Clock offset to be programmed for 700Khz
#define SMB_1M_CLK_PERIOD      100   // Clock period to be programmed for 1Mhz
#define SMB_1M_CLK_OFFSET      19    // Clock offset to be programmed for 1Mhz

#define SMB_10M_CLK_PERIOD     10    // Clock period to be programmed for 10Mhz

//
// CLK_PERIOD = (CLK_LOW_COUNT + CLK_HIGH_COUNT)
// CLK_HIGH_COUNT = i3c_pp_hcnt = SMB_10M_CLK_PERIOD / 2 = 5
//
#define SMB_2M_CLK_LOW_COUNT  45    // low count of Clock period to be programmed for 2Mhz
#define SMB_4M_CLK_LOW_COUNT  20    // low count of Clock period to be programmed for 4Mhz
#define SMB_6M_CLK_LOW_COUNT  12    // low count of Clock period to be programmed for 6Mhz
#define SMB_8M_CLK_LOW_COUNT  8     // low count of Clock period to be programmed for 8Mhz

//
// CLK_PERIOD = (CLK_LOW_COUNT + CLK_HIGH_COUNT)
// CLK_HIGH_COUNT = i3c_pp_hcnt = SMB_1M_CLK_OFFSET / 2 = 50
//
#define SMB_100K_CLK_LOW_COUNT  950    // low count of Clock period to be programmed for 100Khz
#define SMB_400K_CLK_LOW_COUNT  200    // low count of Clock period to be programmed for 400Khz
#define SMB_700K_CLK_LOW_COUNT  94     // low count of Clock period to be programmed for 700Khz

//
// SMBCNTL Register smb_dti bit definitions
//
#define DTI_TSOD            0x03  // '0011' specifies TSOD specific for DDR3\DDR4
#define DTI_EEPROM          0x0A  // '1010' specifies EEPROM's
#define DTI_WP_EEPROM       0x06  // '0110' specifies a write-protect operation for an EEPROM
#define DTI_LRDIMM          0x0B  // '1011' specifies LRDIMM buffer
#define DTI_DCP_FIVE        0x05  // '0101' is used for DDR3 Vref control on the reference platform
#define DTI_DCP_SEVEN       0x07  // '0111' could be used as an alternate for Vref control
#define DTI_NVDIMM_I2C      0x08  // '1000' specifies a Byte Addressible Energy Backed Interface (JEDEC NVDIMM) I2C module
#define DTI_FNV             0x0B  // '1011' specifies NVMCTLR device
#define DTI_PMIC            0x09  // '1001' specifies PMIC

// DDR5 specific 4-bit DTI(SPD5 Device Type ID) / LID(Local Device Type ID) Code
#define DDR5_TS0            0x02  // '0010' specifies TS0
#define DDR5_TS1            0x06  // '0110' specifies TS1

//
// i2cni3c: This field indicates the addressed slave is a I2C device or I3C device.
//
#define  I3C_DEVICE  0x0  // I3C device
#define  I2C_DEVICE  0x1  // I2C device

#define MAX_SMB_INSTANCE       2     // Maximum number of SMBUS Instances
#define INVALID_BUS_SEG        0xFF  // Used to indicate no valid bus segment

typedef enum {
  AttrUnSup = 0x0,
  AttrCccRead,
  AttrCccWrite,
  AttrNormalRead,
  AttrNormalWrite,
  AttrNormalWriteReStartRead,
  AttrSmbMax
} SMB_ATTR_FLAG;

#endif // _PROC_SMB_CHIP_COMMON_H_
