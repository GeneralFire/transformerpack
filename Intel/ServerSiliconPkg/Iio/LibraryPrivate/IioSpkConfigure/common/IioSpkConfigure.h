/** @file
  This file implements an API for enabling Block and drain operations in
  IIO Sierra Peak Initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _IIO_SPK_CONFIGURE_H_
#define _IIO_SPK_CONFIGURE_H_

#define SPK_BAR_ALIGN                     12

//
// SPK Registers offsets from the beginning of SPK instance MMIO
//
#define SPK_LBAR_OFFSET                   0x10
#define SPK_UBAR_OFFSET                   0x14
#define SPK_SIZE_OFFSET                   0x18

#define ADL_SUBSYSTEM_INFO_REG_OFFSET     0
#define ADL_SUBSYSTEM_SPK_BASE_OFFSET     0xF40

#define ADL_SUBSYSTEM_INFO_SPK_PRESENT    BIT22

VOID
InitOobMsmBdf (
  IN UINT8                                IioIndex,
  IN PCI_ROOT_BRIDGE_PCI_ADDRESS         *PciAddress
  );

#endif // _IIO_SPK_CONFIGURE_H_
