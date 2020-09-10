/** @file
  Header file for SataLib.

@copyright
 INTEL CONFIDENTIAL
 Copyright (c) 2016 - 2020 Intel Corporation. All rights reserved
 This software and associated documentation (if any) is furnished
 under a license and may only be used or copied in accordance
 with the terms of the license. Except as permitted by the
 license, no part of this software or documentation may be
 reproduced, stored in a retrieval system, or transmitted in any
 form or by any means without the express written consent of
 Intel Corporation.
 This file contains an 'Intel Peripheral Driver' and is uniquely
 identified as "Intel Reference Module" and is licensed for Intel
 CPUs and chipsets under the terms of your license agreement with
 Intel or your vendor. This file may be modified by the user, subject
 to additional terms of the license agreement.

@par Specification Reference:
**/
#ifndef _SATA_LIB_H_
#define _SATA_LIB_H_

#include <SataCtrl.h>

/**
  Get SATA controller's Port Present Status

  @param[in]  SataCtrlIndex       SATA controller index

  @retval     Port Present Status
**/
UINT8
GetSataPortPresentStatus (
  IN UINT32  SataCtrlIndex
  );

/**
  Get SATA controller Function Disable Status

  @param[in]  SataCtrlIndex       SATA controller index

  @retval 0 SATA Controller is not Function Disabled
  @retval 1 SATA Controller is Function Disabled
**/
BOOLEAN
SataControllerFunctionDisableStatus (
  IN UINT32  SataCtrlIndex
  );

/**
  Get SATA controller ABAR size

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller ABAR size
**/
UINT32
GetSataAbarSize (
  IN UINT32  SataCtrlIndex
  );

/**
  Get SATA controller AHCI base address

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller AHCI base address
**/
UINT32
GetSataAhciBase (
  IN UINT32  SataCtrlIndex
  );

#endif // _SATA_LIB_H_
