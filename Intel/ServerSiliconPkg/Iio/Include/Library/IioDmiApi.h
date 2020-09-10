/** @file
  Header file for DMI API. Contains functions prepared to use outside IIO code.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _IIO_DMI_API_H_
#define _IIO_DMI_API_H_


/**
  Get Max Payload Size set for DMI in Device Control register.
  Should be used after IioPostLateInitialize () completed.

  @retval  DMI Max Payload Size as defined in PCIe Device Control register
           (000b - 128B, 001b - 256B, 010b - 512B, 011b - 1024B, 100b - 2048B, 101b - 4096B)
**/
UINT8
EFIAPI
IioDmiGetMaxPayloadSize (
  VOID
  );

/**
  Return Max Payload Size Supported set for DMI in Device Capabilities register.

  @retval  DMI Max Payload Size Supported as defined in PCIe Device Capabilities register
           (000b - 128B, 001b - 256B, 010b - 512B, 011b - 1024B, 100b - 2048B, 101b - 4096B)
**/
UINT8
IioDmiGetMaxPayloadSizeSupported (
  VOID
  );

#endif // _IIO_DMI_API_H_
