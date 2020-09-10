/** @file
  Interface of Opportunistic Spare Core library.

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

#ifndef __RAS_ACPI_LIB_H__
#define __RAS_ACPI_LIB_H__
#include <RasAcpi.h>

/**
  This routine is to initialize RAS ACPI BIOS Parameters for IIO. currently
  only OsNativeAerSupport is needed.

  @param[in]  NULL

  @retval     NULL
**/
VOID
EFIAPI
IioInitRasAcpiParam (
  VOID
  );

/**
  This routine is to add an entry in RAS ACPI BIOS Parameters for root port or RCEC
  If the root port/RCEC is not exist, each value is 0xff.

  @param[in] Socket         Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.

  @retval     NULL
**/
VOID
EFIAPI
RasAcpiParamAddRootPortEntry (
  UINT8         Socket,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
  );

/**
  This routine is to initialize RAS ACPI BIOS Parameters for Edpc info data.

  @param[in] Socket         Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.

  @retval     NULL
**/
VOID
EFIAPI
EdpcInitRasAcpiParam (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

#endif
