/** @file
  API for the memory call table library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef _MEM_CALL_TABLE_LIB_H_
#define _MEM_CALL_TABLE_LIB_H_

#include <ReferenceCodeDataTypes.h>

#ifdef DEBUG_CODE_BLOCK
  #define CALL_TABLE_STRING(x) x
#else //DEBUG_CODE_BLOCK
  #define CALL_TABLE_STRING(x) ""
#endif //DEBUG_CODE_BLOCK

typedef enum {
  MRC_PF_NULL,                // All policy flags turned off.
  MRC_PF_COLD = BIT0,         // Execute MRC function on cold reset.
  MRC_PF_FAST = BIT1,         // Execute MRC function on cold reset when S3 data is present.
  MRC_PF_WARM = BIT2,         // Execute MRC function on warm reset.
  MRC_PF_S3   = BIT3,         // Execute MRC function on S3 exit.
  MRC_PF_RMT  = BIT4,         // Execute BSSA RMT after this MRC function.
  MRC_PF_ALL  = 0xF           // All policy flags turned off.
  } PFSelectorType;

typedef enum {
  MRC_MP_NULL,                                                  // All policy flags turned off
  MRC_MP_SERIAL         = BIT0,                                 // Execute function when in serial mode
  MRC_MP_SERIAL_LOOP    = BIT1,                                 // Execute function when in serial mode for each socket
  MRC_MP_PARALLEL       = BIT2,                                 // Execute function when in parallel mode
  MRC_MP_PARALLEL_SBSP  = BIT3,                                 // Execute function when in parallel mode on SBSP only
  MRC_MP_BOTH           = MRC_MP_SERIAL | MRC_MP_PARALLEL,      // Execute function in both modes
  MRC_MP_BOTH_LOOP      = MRC_MP_SERIAL_LOOP | MRC_MP_PARALLEL, // Execute function in both modes amd loop when in serial mode
  MRC_MP_BOTH_SBSP      = MRC_MP_SERIAL | MRC_MP_PARALLEL_SBSP, // Execute function in both modes and SBSP only when in parallel mode
} MPSelectorType;

typedef struct CallTableEntryType {
  UINT32(*mrcTask) (PSYSHOST Host);       // Ptr to function to execute, with parameter list.
  UINT8               postCodeMajor;      // BIOS post code output to the debug port if value <> 0.
  UINT8               postCodeMinor;      // BIOS post code output to the debug port if value <> 0.
  UINT8               policyFlag;         // Call table flags
  UINT8               mpFlag;             // Multi-threaded flags
  CHAR8               *debug_string_ptr;  // Output string describing this task (potentially output to debug serial port).
} CallTableEntryType;

/**
Provide the caller with pointers to call tables and call table sizes

@param MainCallTable   - Pointer to pointer to the main call table
@param MainTableSize   - Pointer to main table size

retval    MRC_STATUS_SUCCESS
**/
MRC_STATUS
GetCallTablePtrs (
  OUT CallTableEntryType  **MainCallTable,
  OUT UINT8               *MainTableSize
  );

/**
Provide the caller with pointers to training call table and call table size

@param TrainingCallTable - Pointer to pointer to the training call table
@param TrainingTableSize - Pointer to training table size

retval    MRC_STATUS_SUCCESS
**/
MRC_STATUS
GetTrainingCallTablePtrs (
  OUT CallTableEntryType  **TrainingCallTable,
  OUT UINT8               *TrainingTableSize
  );

#endif //_MEM_CALL_TABLE_LIB_H_
