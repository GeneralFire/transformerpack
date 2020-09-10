/** @file

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
#ifndef  __DDR5_DFE_SETTINGS_H_
#define  __DDR5_DFE_SETTINGS_H_

#ifdef LRDIMM_SUPPORT
#define DB_TA_PAGE            9
#define DB_TA_MAX_REG_VALUE   256
#define INNER_LOOP_START_MAX  255
#define INNER_LOOP_START_MIN  -256
#define OUTER_LOOP_START_MAX  127
#define DB_TA_MAX_STEP_SIZE   16
#define INNER_LOOP_INCR_MAX   511
#define OUTER_LOOP_INCR_MAX   127
#define WRITE_LIMIT_MAX       65536
#define TAP1_MIN              -50
#define TAP2_MIN              -20
#define TAP34_MIN             -15
#define TAP_MAX               15
#define VREF_DFE_MIN          -200
#define VREF_DFE_MAX          200
#define VREF_DQ_MIN           0
#define VREF_DQ_MAX           125

#define VREF_RANGE            416 // Need to store 400 VREF range training results. 
#define DB_DFE_ERRORS         VREF_RANGE / 32  // Need to store a maximum possible range of 401 (sweep size) - 1 bit to indicate error, 32 bits
#define TAP_RANGE             66
#define MAX_SUBCH_BITS        MAX_BITS / 2
#define MIN_DB_GAIN           -6
#define MAX_DB_GAIN           6
#define MIN_DB_GAIN_REG       0
#define MAX_DB_GAIN_REG       7

#pragma pack(1)

typedef struct {
  INT16 LeftEdge;   // Stores left edge eye point
  INT16 RightEdge;  // Stores right edge eye point
} DB_DFE_EYE;

typedef struct {
  UINT32 DbDfeErrorResult[MAX_SUBCH_BITS][DB_DFE_ERRORS];  // Stores error results read from CPGC
} DB_DFE_ERROR_RESULT;

typedef struct {
  INT8    TapCoeff;       // Stores the Tap coefficient value
  UINT16  FigureOfMerit;  // Stores the figure of merit (EH, EW)
} DB_SWEEP_RESULTS;

#pragma pack()

typedef enum {
  DbVrefNull = 0,  // Inner loop paramter of NULL
  DbDfeVref = 1,  // Inner loop paramter of DFE Vref
  DbVrefDq = 2,  // Inner loop paramter of Vref DQ
  DbVrefTypeMax,
} DB_DFE_VREF_TYPE;

typedef enum {
  DbTap1 = 1,  // Outer loop paratmer for Tap 1
  DbTap2 = 2,  // Outer loop paramter for Tap 2
  DbTap3 = 3,  // Outer loop paramter for Tap 3
  DbTap4 = 4,  // Outer loop paramter for Tap 4
  DbTapNull = 0xFE,  // Outer loop paramter of NULL
  DbTapMax,
} DB_DFE_TAPS;

typedef enum {
  DbDfeTaStop = 0, // Stop DFE Training Accelerator
  DbDfeTaStart = 1, // Start DFE Training Accelerator
  DbDfeTaMax,
} DB_DFE_TA_ENABLE;
#endif // #ifdef LRDIMM_SUPPORT


#endif //__DDR5_DFE_SETTINGS_H_
