/** @file
  Header file for SATA SoC library.

@copyright
 INTEL CONFIDENTIAL
 Copyright (c) 2020 Intel Corporation. All rights reserved
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
#ifndef _SATA_SOC_LIB_H_
#define _SATA_SOC_LIB_H_

#include <SataCtrl.h>

#define SATA_1_CONTROLLER_INDEX             0
#define SATA_2_CONTROLLER_INDEX             1
#define SATA_3_CONTROLLER_INDEX             2

/**
  Get SATA controller instance

  @param[in]  SataCtrlIndex       SATA controller index
  @param[out] SataController      Pointer to SATA controller structure

  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - SataController NULL or SataCtrlIndex out of the range
**/
EFI_STATUS
SataGetController (
  IN  UINT32              SataCtrlIndex,
  OUT SATA_CONTROLLER     *SataController
  );

/**
  Get Maximum Sata Controller Number

  @retval Maximum Sata Controller Number
**/
UINT8
MaxSataControllerNum (
  VOID
  );

/**
  Get Maximum Sata Port Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval     Maximum Sata Port Number
**/
UINT8
MaxSataPortNum (
  IN UINT32      SataCtrlIndex
  );

/**
  Check if SATA controller supports RST remapping

  @param[in]  SataCtrlIndex       SATA controller index

  @retval     TRUE                Controller supports remapping
  @retval     FALSE               Controller does not support remapping
**/
BOOLEAN
IsRemappingSupportedOnSata (
  IN UINT32  SataCtrlIndex
  );

/**
  Checks if SoC supports the SATA PGD power down on given
  SATA controller.

  @param[in] SataCtrlIndex  SATA controller index

  @retval TRUE   SATA PGD power down supported
  @retval FALSE  SATA PGD power down not supported
**/
BOOLEAN
IsSataPowerGatingSupported (
  IN UINT32 SataCtrlIndex
  );

#endif // _SATA_SOC_LIB_H_
