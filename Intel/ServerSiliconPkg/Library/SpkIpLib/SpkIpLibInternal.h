/** @file
  Internal header file used for SpkIp library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019-2020 Intel Corporation. <BR>

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

#ifndef _SPK_IP_LIB_INTERNAL_H
#define _SPK_IP_LIB_INTERNAL_H

/**
  MMIO Write wrapper to print assembly code for emulation

  @param MmioBar         - The MemBar0 of SPK device.
  @param PciBarOffset    - The BAR offset for each SPK Instance.
  @param Reg             - The register to be accessed.
  @param Value           - The value to be programmed into the register.

  @return Value.
**/
UINT16
EFIAPI
SpkMmioWrite16 (
  IN UINT32 MmioBar,
  IN UINT32 PciBarOffset,
  IN UINT32 Reg,
  IN UINT16 Value
  );

#endif // _SPK_IP_LIB_INTERNAL_H
