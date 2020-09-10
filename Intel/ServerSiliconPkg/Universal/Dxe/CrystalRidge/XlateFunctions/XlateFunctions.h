/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2018 Intel Corporation. <BR>

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

#ifndef _XLATEFUNCTIONS_H_
#define _XLATEFUNCTIONS_H_

#include "CrystalRidge.h"

typedef struct _DPA_ADDRESS_ {
  UINT64 Offset;
  UINT8  IntDimmNum;
  UINT8  Ch;
  UINT8  Imc;
  UINT8  Skt;
} DPA_ADDRESS;

/**

Routine Description: This function returns line multiplier for given dimm and
  line numbers taking part in interleaving.

  @param[in] ImcSize     - memory controller granularity
  @param[in] LineSize    - channel granularity
  @param[in] ImcWays     - Imc interleaving ways
  @param[in] ChWays      - channel interleaving ways
  @param[in] DimmNum     - dimm number in interleaving set (starting from 0)
  @param[in] LineNum     - line number

  @return UINT32     - line multiplier for given arguments

**/
UINT32
GetLineMultiplier (
  UINT32 ImcSize,
  UINT32 LineSize,
  UINT8  ImcWays,
  UINT8  ChWays,
  UINT8  DimmNum,
  UINT8  LineNum
  );

/**

Routine Description: This function converts given SPA to DPA Address

  @param[in]  SpaBaseAddr - SAD Base Address
  @param[in]  Spa         - Spa for which we need the DPA
  @param[in]  ImcWays     - Imc interleaving ways
  @param[in]  ChWays      - channel interleaving ways
  @param[in]  LineSize    - channel granularity
  @param[in]  ImcSize     - memory controller granularity
  @param[out] *DpaAddress - Full DPA address

  @return EFI_STATUS - status of the conversion effort

**/
EFI_STATUS
XlateSpaToDpaAddress (
  IN  UINT64      SpaBaseAddr,
  IN  UINT64      Spa,
  IN  UINT8       ImcWays,
  IN  UINT8       ChWays,
  IN  UINT32      LineSize,
  IN  UINT32      ImcSize,
  OUT DPA_ADDRESS *DpaAddress
  );

/**

Routine Description: XlateSpaToDpa - This function converts
given Spa to Dpa in the case of the Dimm involved in all Channel
ways and up to 8 Imc Ways. Converts only PMEM and CTRL regions.

  @param[in] *NvmDimmPtr - Pointer to Dimm structure
  @param[in] SpaBaseAddr - SAD Base Address
  @param[in] Spa         - Spa for which we need the DPA
  @param[out] *Dpa       - Pointer to the Dpa that this function
                           returns.

  @return EFI_STATUS - status of the conversion effort

**/
EFI_STATUS
XlateSpaToDpaOffset (
  IN  NVDIMM    *NvmDimmPtr,
  IN  UINT64    SpaBaseAddr,
  IN  UINT64    Spa,
  OUT UINT64    *Dpa
  );

/**

Routine Description: XlateDpaToSpa - This function converts given Dpa
to Spa in the case of the Dimm involved in all Channel ways and
up to 8 Imc Ways. Converts only PMEM and CTRL regions.

  @param[in] *NvmDimmPtr - Pointer to Dimm structure
  @param[in] SpaBaseAddr - Start Address of the Spa for Dpa Translation
  @param[in] Dpa         - Dpa offset for which we need the SPA
  @param[out] *Spa       - Pointer to the Spa.

  @return EFI_STATUS - status of the conversion effort

**/
EFI_STATUS
XlateDpaOffsetToSpa (
  IN  NVDIMM    *NvmDimmPtr,
  IN  UINT64    SpaBaseAddr,
  IN  UINT64    Dpa,
  OUT UINT64    *Spa
  );

#endif // _XLATEFUNCTIONS_H_

