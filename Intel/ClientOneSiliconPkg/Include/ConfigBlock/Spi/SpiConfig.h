/** @file
  PCH SPI Flash Controller config block

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

#ifndef _SPI_CONFIG_H_
#define _SPI_CONFIG_H_

#define SPI_CONFIG_REVISION 1
extern EFI_GUID gSpiConfigGuid;

#pragma pack (push,1)

/**
  Basic configuration for option features of PCH SPI Flash controller
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;           ///< Config Block Header
  /**
    Enable extended BIOS Direct Read Region feature
    Enabling this will make all memory accesses in a decode range to be translated
    to BIOS region reads from SPI flash
    <b>0: Disabled</b>, 1: Enabled
  **/
  UINT32    ExtendedBiosDecodeRangeEnable   :  1;
  UINT32    RsvdBits0                       : 31;       ///< Reserved bits
  /**
    Base address that will be used for Extended Decode Range.
    This will be ignored when ExtendedBiosDecodeRangeEnable is set to 0.
  **/
  UINT32    ExtendedBiosDecodeRangeBase;
  /**
    Limit address that will be used for Extended Decode Range.
    This will be ignored when ExtendedBiosDecodeRangeEnable is set to 0.
  **/
  UINT32    ExtendedBiosDecodeRangeLimit;
} SPI_CONFIG;

#pragma pack (pop)

#endif // _SPI_CONFIG_H_
