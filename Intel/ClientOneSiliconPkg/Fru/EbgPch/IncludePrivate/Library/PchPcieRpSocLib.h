/** @file
  Header file for PCIE Root Port SoC library.

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

#ifndef _PCIE_RP_SOC_LIB_H_
#define _PCIE_RP_SOC_LIB_H_

#include <Library/PchPcieRpDev.h>

/**
  Get PCIE RP instance

  @param[in, out] RpIndex             Root Port index
  @param[in, out] RpDevice            Root Port device pointer

  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - RpDevice NULL or PchId is out of range
**/
EFI_STATUS
PchGetRpDevice (
  IN     UINT32             RpIndex,
  IN OUT PCH_RP_DEVICE      *RpDevice
  );

/**
  Get IOE PCIE RP instance

  @param[in     ] IoeId               I/O Expander Id (0 ... n - I/O Expander)
  @param[in, out] RpIndex             Root Port index
  @param[in, out] RpDevice            Root Port device pointer

  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - RpDevice NULL or IoeId is out of range
**/
EFI_STATUS
IoeGetRpDevice (
  IN     UINT32             IoeId,
  IN     UINT32             RpIndex,
  IN OUT PCH_RP_DEVICE      *RpDevice
  );

#endif // _PCIE_RP_SOC_LIB_H_
