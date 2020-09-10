/** @file
  CPGC Test Table for Memory Training

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _MEMCPGCTESTTABLE_H
#define _MEMCPGCTESTTABLE_H

#include <ReferenceCodeDataTypes.h>

//
// Include files
//
#include <MrcCommonTypes.h>
#include <Memory/CpgcDefinitions.h>
#include "CpgcChip.h"

typedef struct _CPGC_TEST_PARAM {
  MRC_TT                   TestType;
  UINT8                    CmdPat;
  TCPGCAddress             *CpgcAddress;
  UINT8                    Mux[CPGC_MAX_NUMBER_DPAT_UNISEQ];    // Uniseq mode
  UINT32                   Poly[CPGC_MAX_NUMBER_DPAT_UNISEQ];   // Lfsr polynomial
  UINT32                   Patbuf[CPGC_MAX_NUMBER_DPAT_UNISEQ]; // Pattern buffer
  TWDBPattern              WdbPattern;
  BOOLEAN                  IsGenPrbsAddr;
  UINT16                   StartDelay[SUB_CH];
  CPGC2_REQUEST_DATA_SIZE  RequestDataSize;
} CPGC_TEST_PARAM;

typedef struct _CPGC_TEST_ENV_PARAM {
  MRC_TT         TestType;
  UINT32         LoopCount;
  UINT16         NumCacheLine;
  UINT16         SubSeqWait;
} CPGC_TEST_ENV_PARAM;

/**
  Provide the caller with pointers to cpgc test parameter table

  @param[in]   TestType                          - Specify memory training steps
  @param[out]  CpgcTestTable                     - Pointer to pointer to the cpgc test parameter table

  @retval MRC_STATUS_SUCCESS                     - Find the table
          MRC_STATUS_FAILURE                     - Not find the table

**/
MRC_STATUS
GetCpgcTestParamPtrs (
  IN  MRC_TT             TestType,
  OUT CPGC_TEST_PARAM    **CpgcTestTable
  );

/**
  Provide the caller with pointers to cpgc test enrivonment table

  @param[in]   TestType                          - Specify memory training steps
  @param[out]  CpgcTestEnvTable                  - Pointer to pointer to the cpgc test environment table

  @retval MRC_STATUS_SUCCESS                     - Find the table
          MRC_STATUS_FAILURE                     - Not find the table

**/
MRC_STATUS
GetCpgcTestEnvPtrs (
  IN  MRC_TT               TestType,
  OUT CPGC_TEST_ENV_PARAM  **CpgcTestEnvTable
  );

/**

  Get Receive Enable Loop Count.

  @param[out]  LoopCount     - Receive enable loop count.

  @retval  EFI_SUCCESS       - This function completed execution with no warnings/errors
           EFI_NOT_FOUND     - Receive enable loop count is not found

**/
EFI_STATUS
GetRcvEnLoopCount (
  OUT  UINT32  *LoopCount
  );

/**

  Get burst wait for reads

  @param[out]  SubSeqWait    - Burst wait for reads.
                             - Unit: native CPGC clock (1/4 the DDR frequency)

  @retval  EFI_SUCCESS       - This function completed execution with no warnings/errors
           EFI_NOT_FOUND     - Receive enable loop count is not found

**/
EFI_STATUS
GetRdDqBurstWait (
  OUT  UINT16  *SubSeqWait
  );

/**

  Get burst wait for writes, wirtes->reads, and reads->writes

  @param[out]  SubSeqWait    - Burst wait for writes, wirtes->reads, and reads->writes
                             - Unit: native CPGC clock (1/4 the DDR frequency)

  @retval  EFI_SUCCESS       - This function completed execution with no warnings/errors
           EFI_NOT_FOUND     - Receive enable loop count is not found

**/
EFI_STATUS
GetWrDqBurstWait (
  OUT  UINT16  *SubSeqWait
  );

/**

  Excute Cpgc Test
  It is used to setup training patterns for all channels defined by input parameter "ChannelMask"
  cpgc test worker procedual can be used for regular data path training and BSSA

  @param[in] Socket                - Socket to train
  @param[in] ChannelMask           - Ch Bit mask for which test should be setup for
  @param[in] LogicalRank           - Logical Rank number
  @param[in] SeqCnt                - Optional to override default loopcount for given test type
  @param[in] CPGC_TEST_PARAM       - Struct pointer to Cpgc Test Parameter
  @param[in] CPGC_TEST_ENV_PARAM   - Struct poniter to Cpgc Test Enviroment Parameter
  @param[in] BaseColRepeats        - Base Col Repeats Value

  @retval    MRC_STATUS

**/
MRC_STATUS
CpgcTestWorker (
  IN UINT8                    Socket,
  IN UINT32                   ChannelMask,
  IN UINT8                    LogicalRank,
  IN UINT32                   SeqCnt,
  IN CPGC_TEST_PARAM          *CpgcTestParamPtr,
  IN CPGC_TEST_ENV_PARAM      *CpgcTestEnvPtr,
  IN UINT32                   BaseColRepeats
  );
#endif
