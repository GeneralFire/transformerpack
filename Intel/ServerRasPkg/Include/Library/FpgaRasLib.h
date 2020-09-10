/** @file
  Interface of Fpga library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#ifndef __FPGA_RAS_LIB_H__
#define __FPGA_RAS_LIB_H__

/**
  Create the Fpga error record
  @param[in] ErrSts     - A pointer to the Processor Error Record.

  @retval Sev   Severity

**/
EFI_STATUS
EFIAPI
FpgaLogError (
  VOID
  );

/**
  Enable FPGA error reporting feature.

  @retval    Status.
**/
EFI_STATUS
FpgaEnErrReport (
  VOID
  );

/**
  Disable FPGA error reporting feature.

  @retval    Status.
**/
EFI_STATUS
FpgaDisErrReport (
  UINT32 DeviceType,
  UINT32 EventType,
  UINT32 *ExtData
  );

/**
  Clear FPGA related error. This only way to clear FPGA error is power cycle. So, cold reset will be issued within this func.

  @retval    BOOLEAN.  Indicating if error is found on FPGA.
**/
BOOLEAN
FpgaIsErrPinSmi (
  VOID
  );

/**
  Check if FPGA Fme error.

  @retval    BOOLEAN.  Indicating if error is found on FPGA.
**/
BOOLEAN
FpgaIsFmeError (
  VOID
  );

/**
  Check if FPGA Kti error.

  @retval    BOOLEAN.  Indicating if error is found on FPGA.
**/
BOOLEAN
FpgaIsKtiError (
  VOID
  );

/**
  Clear FPGA related error. This only way to clear FPGA error is power cycle. So, cold reset will be issued within this func.

  @retval    Status.
**/

EFI_STATUS
FpgaClearErrStatus (
  VOID
  );

/**
  Hide or unhide FPGA device.

  @param[in]    Hide    To hide or unhide FPGA devices.
  @retval EFI_SUCCESS   The interrupt was handled successfully.
**/
EFI_STATUS
EFIAPI
FpgaHideDevices (
  BOOLEAN  Hide
  );

/**
  Fpga collect Fme bar.
  The constructor function initialize Fpga library.
  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
FpgaCollectFmeBar (
  VOID
  );

#endif
