/** @file
  MemMcChip.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#ifndef  _memmcchip_h
#define  _memmcchip_h

#include <ReferenceCodeDataTypes.h>
#include "SysHost.h"
#include "Include/MemMcRegs.h"

//
// private definitions used in this library
//

//
// lpmode_entry_latency values
//
#define LPMODE_ENTRY_LATENCY_SAFE_SNR  7
#define LPMODE_ENTRY_LATENCY_LARGE_SNR 13

#define ECC_RETRY_FAIL_LIMIT           4

//
// Function prototypes that are private to this lib
//
/**
  Internal worker function to configure XOR mode bit fields in MCMTR_MC_MAIN register

  @param[in, out] McMainReg    Pointer to MC main register
  @param[in]      XorMode      Bitmap for XOR fields
  @param[in]      DimmNvList   Pointer to DIMM_NVRAM_STRUCT array

**/
VOID
McSetXorModeCfg (
  IN OUT MCMTR_MC_MAIN_STRUCT   *McMainReg,
  IN     UINT8                  XorMode,
  IN     DIMM_NVRAM_STRUCT      (*DimmNvList)[MAX_DIMM]
  );

/**
  Program scheduler registers after training

  @param[in] Socket - Socket number

  @retval   MRC_STATUS_SUCCESS
**/
MRC_STATUS
DdrSchedulerConfigLate (
  IN    UINT8    Socket
  );

/**
   Determine the min and max values of roundtrip per channel for ddr4 dimms in qclks

   @param[in] Host            - Pointer to sysHost
   @param[in] Socket          - Socket Id
   @param[in] Ch              - Channel number
   @param[out] MinRoundTrip   - Pointer to caller's min roundtrip parameter
   @param[out] MaxRoundTrip   - Pointer to caller's max roundtrip parameter

   @retval EFI_SUCCESS if at least 1 DDR4 DIMM installed in channel
           EFI_NOT_FOUND otherwise - MinRoundTrip and MaxRoundTrip not updated
**/
EFI_STATUS
GetMinMaxRoundTrip (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  OUT   UINT8     *MinRoundTrip,
  OUT   UINT8     *MaxRoundTrip
  );

/**
Get t_ddrt_twl for Chip

@param[in]  Host    - Pointer to sysHost structure
@param[in]  Socket  - Socket number
@param[in]  Ch      - Channel number

@retval void
**/
UINT8
GetDdrtTwl(PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch);

#endif   // _memmcchip_h
