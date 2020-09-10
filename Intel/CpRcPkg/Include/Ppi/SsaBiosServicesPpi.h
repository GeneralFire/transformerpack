/** @file
  SsaBiosServicesPpi.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2017 Intel Corporation. <BR>

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

#ifndef _SSA_BIOS_SERVICES_PPI_H_
#define _SSA_BIOS_SERVICES_PPI_H_

// BIOS-SSA PPI revision
// PPI/API revisions - Major.Minor.Release.Build
#define SSA_REVISION_BIOS ((0x01 << 24) | (0x01 << 16) | (0x00 << 8) | 0x00)

typedef struct _SSA_BIOS_SERVICES_PPI SSA_BIOS_SERVICES_PPI;
typedef struct _SSA_BIOS_SERVICES_PPI* PSSA_BIOS_SERVICES_PPI;

#ifdef SSA_CLIENT_FLAG
#if 0 // Change this to #if [0|1] to turn [off|on] additional SSA debug messages.
#define SSA_DEBUG_PRINT                          (1)
#define SSA_DEBUG_MSG(DEBUG, LEVEL, FORMAT, ...) MrcPrintf (DEBUG, LEVEL, FORMAT, ##__VA_ARGS__)
#define SSA_PRINT_MEMORY(DEBUG, START, SIZE)     MrcPrintMemory (DEBUG, START, SIZE)
#else
#define SSA_DEBUG_MSG(DEBUG, LEVEL, FORMAT, ...)
#define SSA_PRINT_MEMORY(DEBUG, START, SIZE)
#endif
#ifdef SSA_DEBUG_PRINT
#define SSA_PARAM_ERROR_CHECK           (1)
#endif
#endif // SSA_CLIENT_FLAG

#include "MrcCommonTypes.h"
#include <Library/DebugLib.h>
#include <PiPei.h>

///
/// SSA function return error codes.
///
typedef enum {
  Success,                                ///< The function completed successfully.
  Failure,                                ///< Generic, non specified failure.
  NotYetAvailable,                        ///< The function is not yet available.
  NotAvailable,                           ///< The function or selected item is not available .
  UnsupportedValue,                       ///< A function parameter is incorrect.
  SsaStatusMax,                           ///< SSA_STATUS enumeration maximum value.
  SsaStatusDelim = MAX_INT32              ///< This value ensures the enum size is consistent on both sides of the PPI.
} SSA_STATUS;

#ifdef SSA_CLIENT_FLAG
#include <Library/DebugLib.h>
#include "MrcInterface.h"
#define SSA_CONST CONST
#endif // SSA_CLIENT_FLAG

#include "SsaCommonConfig.h"
#include "SsaResultsConfig.h"
#include "SsaMemoryConfig.h"
#include "SsaMemoryClientConfig.h"
#include "SsaMemoryServerConfig.h"
#include "SsaMemoryDdrtConfig.h"
#include "SsaMemoryHbmConfig.h"

///
/// SSA BIOS Services PPI
///
struct _SSA_BIOS_SERVICES_PPI {
  UINT32                   Revision;                  ///< Incremented when a backwards compatible binary change is made to the PPI.
  SSA_COMMON_CONFIG        *SsaCommonConfig;          ///< Pointer to the BIOS-SSA common functions.
  SSA_RESULTS_CONFIG       *SsaResultsConfig;         ///< Pointer to the BIOS results related functions.
  SSA_MEMORY_CONFIG        *SsaMemoryConfig;          ///< Pointer to the BIOS-SSA memory related functions.
  SSA_MEMORY_CLIENT_CONFIG *SsaMemoryClientConfig;    ///< Pointer to the BIOS-SSA client memory related functions.
  SSA_MEMORY_SERVER_CONFIG *SsaMemoryServerConfig;    ///< Pointer to the BIOS-SSA server memory related functions.
  SSA_MEMORY_DDRT_CONFIG   *SsaMemoryDdrtConfig;      ///< Pointer to the BIOS-SSA DDR-T memory related functions.
};

#endif // _SSA_BIOS_SERVICES_PPI_H_
