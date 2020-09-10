/** @file
  API for the memory POR table library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#ifndef _MEM_POR_LIB_H_
#define _MEM_POR_LIB_H_

#include <Uefi.h>

/**

  Find the highest DDR frequency supported by this configuration

  @param[in] Socket             - Socket Id
  @param[in] Ch                 - Channel number (0-based)
  @param[in] Vdd                - Vdd
  @param[out] PorDdrFrequency   - Highest frequency supported by the DDR configuration on the specified channel.
                                  = NOT_SUP if the configuration is not supported.

  @retval EFI_SUCCESS if configuration is supported
          EFI_UNSUPPORTED otherwise

**/
EFI_STATUS
EFIAPI
GetPorDdrFrequency (
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Vdd,
     OUT  UINT8     *PorDdrFrequency
  );

/** Print the system's socket configuration that corresponds with POR table entries for the DIMM Info table.

  @retval EFI_SUCCESS   The configuration was successfully printed.
  @retval !EFI_SUCCESS  The configuration was not successfully printed.

**/
EFI_STATUS
EFIAPI
PrintPorSocketConfigDimmInfo (
  VOID
  );

#endif //_MEM_POR_LIB_H_
