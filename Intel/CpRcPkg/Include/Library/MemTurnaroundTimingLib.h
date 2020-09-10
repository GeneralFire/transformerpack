/** @file
  API for the memory turnaround timings library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#ifndef _MEM_TURNAROUND_LIB_H_
#define _MEM_TURNAROUND_LIB_H_

#include <ReferenceCodeDataTypes.h>

#pragma pack (push, 1)
//
// Electrical turnaround constants, per frequency
//
typedef struct {
  UINT8   Frequency;     // DDR/HBM freq
  UINT8   t_rwsg;        // read to write, same bank, same rank, same DIMM
  UINT8   t_rwsr;        // read to write, different bank, same rank, same DIMM
  UINT8   t_rrdr;        // read to read, different rank, same DIMM
  UINT8   t_wwdr;        // write to write, different rank, same DIMM
  UINT8   t_rwdr;        // read to write, different rank, same DIMM
  UINT8   t_wrdr;        // write to read, different rank, same DIMM
  UINT8   t_rrdd;        // read to read, different DIMM
  UINT8   t_wwdd;        // write to write, different DIMM
  UINT8   t_rwdd;        // read to write, different DIMM
  UINT8   t_wrdd;        // write to read, different DIMM
  UINT8   tcrwp_t_rwsr;  // read to write, different bank, same rank, same DIMM
} TURNAROUND_CONSTANTS;

//
// Electrical DCPMM turnaround constanst
//
typedef struct {
  UINT8  DdrFrequency;     // DDR freq
  UINT8  RdRdDDConst;      // read to read, different DIMM
  UINT8  WrRdDDConst;      // write to read, different DIMM
  UINT8  RdRdSCosnt;       // read to read, same DIMM
  UINT8  WrRdSConst;       // write to read, same DIMM
  UINT8  RdWrDDConst;      // read to write, different DIMM
  UINT8  RdWrSConst;       // read to write, same DIMM
  UINT8  WrWrDDConst;      // write to write, different DIMM
  UINT8  WrWrSConst;       // write to write, same DIMM
  UINT8  GntRdDDConst;     // grant to read, different DIMM
  UINT8  GntRdSConst;      // grant to read, same DIMM
  UINT8  GntWrDDConst;     // grant to write, different DIMM
  UINT8  GntWrSConst;      // grant to write, same DIMM
  UINT8  RdGntDDConst;     // read to grant, different DIMM
  UINT8  RdGntSConst;      // read to grant, same DIMM
  UINT8  WrGntDDConst;     // write to grant, different DIMM
  UINT8  WrGntSConst;      // write to grant, same DIMM
  UINT8  GntGntDDConst;    // grant to grant, different DIMM
  UINT8  GntGntSConst;     // grant to grant, same DIMM
} DCPMM_TURNAROUND_CONSTANTS;

//
// Turnaround Time types
//
typedef enum {
  t_RRSG = 0,    // read to read, same bank, same rank, same DIMM
  t_WWSG,        // write to write, same bank, same rank, same DIMM
  t_RWSG,        // read to write, same bank, same rank, same DIMM
  t_WRSG,        // write to read, same bank, same rank, same DIMM
  t_RRSR,        // read to read, different bank, same rank, same DIMM
  t_WWSR,        // write to write, different bank, same rank, same DIMM
  t_RWSR,        // read to write, different bank, same rank, same DIMM
  t_WRSR,        // write to read, different bank, same rank, same DIMM
  t_RRDR,        // read to read, different rank, same DIMM
  t_WWDR,        // write to write, different rank, same DIMM
  t_RWDR,        // read to write, different rank, same DIMM
  t_WRDR,        // write to read, different rank, same DIMM
  t_RRDD,        // read to read, different DIMM
  t_WWDD,        // write to write, different DIMM
  t_RWDD,        // read to write, different DIMM
  t_WRDD,        // write to read, different DIMM
  t_RRDS,        // read to read, different subrank, same DIMM
  t_WWDS,        // write to write, different subrank, same DIMM
  t_RWDS,        // read to write, different subrank, same DIMM
  t_WRDS,        // write to read, different subrank, same DIMM
  t_RRLDS,       // read to read, different subrank, same bankgroup
  t_WWLDS,       // write to write, different subrank, same bankgroup
  t_RRDLR,       // activate to activate, different 3DS logical rank
  tcrwp_t_RWSR,  // read to write, different bank, same rank, same DIMM
  TurnaroundTypeMax
} TURNAROUND_TYPE;

//
// DCPMM Turnaround Time types
//
typedef enum {
  t_RDRD_DD = 0,  // read to read, different DIMM
  t_WRRD_DD,      // write to read, different DIMM
  t_RDRD_S,       // read to read, same DIMM
  t_WRRD_S,       // write to read, same DIMM
  t_RDWR_DD,      // read to write, different DIMM
  t_RDWR_S,       // read to write, same DIMM
  t_WRWR_DD,      // write to write, different DIMM
  t_WRWR_S,       // write to write, same DIMM
  t_GNTRD_DD,     // grant to read, different DIMM
  t_GNTRD_S,      // grant to read, same DIMM
  t_GNTWR_DD,     // grant to write, different DIMM
  t_GNTWR_S,      // grant to write, same DIMM
  t_RDGNT_DD,     // read to grant, different DIMM
  t_RDGNT_S,      // read to grant, same DIMM
  t_WRGNT_DD,     // write to grant, different DIMM
  t_WRGNT_S,      // write to grant, same DIMM
  t_GNTGNT_DD,    // grant to grant, different DIMM
  t_GNTGNT_S,     // grant to grant, same DIMM
  DcpmmTurnaroundTypeMax
} DCPMM_TURNAROUND_TYPE;
#pragma pack (pop)

/**
  Returns the loop latency.  Loop Latency represents the CAS to CAS turnaround delay through the iMC.

  @param None

  @retval the loop latency for this SOC
**/
UINT8
EFIAPI
GetLoopLatency (
  VOID
  );

/**

  Gets the timing variables needed to calculate turnaround times

  @param[in]  Socket              - Socket number
  @param[in]  Channel             - Channel number
  @param[out] BusTimingData       - Structure that holds the variables
  @param[in]  DdrtTurnaroundFlag  - DDRT turnaround flag

  @retval N/A

**/
VOID
EFIAPI
InitBusTimingData (
  IN UINT8             Socket,
  IN UINT8             Channel,
  OUT BUS_TIMING_DATA  *BusTimingData,
  IN BOOLEAN           DdrtTurnaroundFlag
  );

/**
  Early turnaround times programed before training according to relax values given by TA doc.
  display at the end of training for final results.

  @param[in] Socket  - Socket number
  @param[in] Ch      - Channel number

  @retval None
**/
EFI_STATUS
EFIAPI
InitRelaxTurnaroundTimes (
  IN UINT8  Socket,
  IN UINT8  Channel
  );

/**

  Early turnaround times for DCPMM programed before training according to relax values given by TA doc.
  Display at the end of training for final results.

  @param[in] Socket   - Socket number
  @param[in] Channel  - Channel number

  @retval None

**/
VOID
InitRelaxTurnAroundTimesDcpmm (
  IN UINT8 Socket,
  IN UINT8 Channel
  );

/**
  Early turnaround times programed before training according to relax values given by TA doc.
  display at the end of training for final results.

  @param[in] Socket  - Socket number

  @retval None
**/
EFI_STATUS
EFIAPI
InitRelaxTurnaroundTimesHbm (
  IN UINT8  Socket
  );

/**

  Display turnaround times

  @param[in] MemTechType   - Memory Technology Type
  @param[in] Socket        - Socket number
  @param[in] Ch            - Channel number

  @retval None

**/
VOID
EFIAPI
DisplayTurnaroundTimes (
  IN     MEM_TECH_TYPE  MemTechType,
  IN     UINT8          Socket,
  IN     UINT8          Ch
  );

/**

  Programs turnaround times after training.

  @param[in] Socket - Socket number

  @retval EFI_SUCCESS - Turnaround CSRs read/write successful

**/
EFI_STATUS
EFIAPI
OptimizeTurnaroundTimings (
  IN UINT8 Socket
  );

/**

  Override turnaround times for select environments

  @param[in] Socket - Socket number

  @retval EFI_SUCCESS - Turnaround CSRs read/write successful

**/
EFI_STATUS
EFIAPI
OverrideTurnaroundTimes (
  IN UINT8 Socket
  );

/**

  Calculate DDR5 turnaround timing

  @param[in] Socket - Socket number

  @retval EFI_SUCCESS - Turnaround CSRs read/write successful

**/
EFI_STATUS
EFIAPI
OptimizeTurnaroundTimingsDdr5 (
  IN UINT8 Socket
  );

/**
  Evaluates if the Early WrCRC feature is applicable to the current configuration

  @param[in] Channel - Channel number (0-based)

  @retval FALSE - Not applicable
          TRUE  - Applicable
**/
BOOLEAN
IsEarlyWrCrcRequired (
  IN UINT8 Channel
  );

/**
  Calculate any additional delay due to WR_CRC settings (in Dclks).

  This is an adder to the adjusted burst length, as WR_CRC increases the default
  burst length from 8 to 10 Qclks (4 to 5 Dclks).

  @param[in] Channel - DDR channel number (0-based)

  @retval Any additional delay due to WR_CRC settings (in Dclks).
**/
UINT8
CalculateWrCrcAdder (
  IN UINT8 Channel
  );

/**

  Programs DCPMM turnaround times after training.

  @param[in] Socket  - Socket number

  @retval EFI_SUCCESS   Turnaround CSRs read/write successful

**/
EFI_STATUS
EFIAPI
OptimizeTurnaroundTimingsDcpmm (
  IN UINT8  Socket
  );

/**

  Programs safe (non-optimized) DDRT turnaround timings

  @param[in] Socket  - Socket number

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
SetSafeTurnaroundTimingsDcpmm (
  UINT8  Socket
  );

/**

  Check to ensure no Rank Switch Fix issue

  @param[in] Socket  - Socket number

  @retval EFI_SUCCESS

**/
MRC_STATUS
EFIAPI
DcpmmTurnaroundEquationCheck (
  IN UINT8  Socket
  );

/**

  Check turnaround times programmed after training against optimized values
  and display at the end of training for final results.

  @param[in] Socket  - Socket number

  @retval None

**/
VOID
EFIAPI
ComparePerformanceTurnaroundTimes (
  IN UINT8  Socket
  );

#ifdef LRDIMM_SUPPORT
/**

  This function calculates the minimum Rank to Rank timing requirement for DDR4DB02 in RDWR transactions

  @param[in] Host        - Point to sysHost
  @param[in] Socket      - Socket number
  @param[in] Channel     - Channel number

  @retval Minimum RWDR based on the spec.

**/
UINT8
DataBufferSpecMinRWDR (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  );

/**

  This function calculates the minimum Rank to Rank timing requirement for DDR4DB02 in WRRD transactions

  @param[in] Host        - Point to sysHost
  @param[in] Socket      - Socket number
  @param[in] Channel     - Channel number

  @retval Minimum WRRD based on the spec.

**/
UINT8
DataBufferSpecMinWRDR (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  );
#endif //LRDIMM_SUPPORT

#endif //_MEM_TURNAROUND_LIB_H
