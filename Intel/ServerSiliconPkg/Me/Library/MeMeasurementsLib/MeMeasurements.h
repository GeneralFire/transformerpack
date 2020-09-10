/** @file
  ME Measurements Library

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 Intel Corporation.

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

**/

#ifndef _ME_MEASUREMENTS_H_
#define _ME_MEASUREMENTS_H_

#define MAX_HER_REGISTERS 8

#ifndef B_ME_EXTEND_REG_ALGORITHM
#define B_ME_EXTEND_REG_ALGORITHM     (BIT3 | BIT2 | BIT1 | BIT0)   // Bits3:0 = 0x0 - SHA1; 0x2 - SHA256; 0x4 - SHA384
#endif // B_ME_EXTEND_REG_ALGORITHM

#pragma pack(1)
typedef union {
  UINT32  Data32;
  UINT16  Data16[2];
  UINT8   Data8[4];
} DATA32_UNION;
#pragma pack()

typedef struct _EFI_TCG_EV_POST_CODE {
  EFI_PHYSICAL_ADDRESS              PostCodeAddress;
  UINT64                            PostCodeLength;
} EFI_TCG_EV_POST_CODE;

#endif // _ME_MEASUREMENTS_H_

