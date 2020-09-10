/** @file
  MemXoverCalib.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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

#ifndef  __MEM_XOVER_CALIB_H__
#define  __MEM_XOVER_CALIB_H__

#include "SysHost.h"
#include <Include/MemDdrioRegs.h>

#define   X_OVER_STEP_SIZE  1
#define   X_OVER_PI_START   0
#define   X_OVER_MAX_SAMPLE 128
#define   X_OVER_OFFSET     58

#define   MAX_CMDPI       6

#if (MEM_IP_VER >= MEM_IP_17ww37e)
#define   MAX_CLKPI       1
#else
#define   MAX_CLKPI       8
#endif
#define   CMDN_INDEX      0
#define   CMDS_INDEX      1
#define   CTL_INDEX       2
#define   CKE_INDEX       3
#define   MAX_CMD_FUB     4
#define   CMDPIREF0_INDEX (MAX_CMDPI - 1)
#define XOVER_CAL_ENABLE  1
#define XOVER_CAL_DISABLE 0
#define CMD_CTL_NUM       4
#define CLK_NUM           2

#define XOVER_CALIB_SAMPLE_COUNT      16

typedef struct {
  UINT8                   DataOffset;
  UINT8                   ClkOffset;
  UINT8                   CmdOffset;
  UINT8                   CmdPi0Offset;
} XOVER_OFFSETS;

#if (MEM_IP_VER >= MEM_IP_17ww37e)
//
// Local Parameter Structures
//
typedef struct {
  UINT8     CmdCtl[MAX_CH][SUB_CH][CMD_CTL_NUM];
  UINT8     Clk[MAX_CH][CLK_NUM];
  UINT8     Strobe[MAX_CH][MAX_STROBE];
} COUNT1_STRUCT;

typedef struct {
  UINT16    CmdCtl[MAX_CH][SUB_CH][CMD_CTL_NUM];
  UINT16    Clk[MAX_CH][CLK_NUM];
  UINT16    Offset[MAX_CH][MAX_STROBE];
} OFFSET_STRUCT;

typedef struct {
  UINT8     CmdCtl[MAX_CH][SUB_CH][CMD_CTL_NUM];
  UINT8     Clk[MAX_CH][CLK_NUM];
  UINT8     Data[MAX_CH][MAX_STROBE];
} PREVIOUS_STATUS_STRUCT;

typedef struct {
  UINT8     CmdCtl[MAX_CH][SUB_CH][CMD_CTL_NUM];
  UINT8     Clk[MAX_CH][CLK_NUM];
  UINT8     Data[MAX_CH][MAX_STROBE];
} SAMPLE_COUNT_STRUCT;

typedef struct {
  UINT8     CmdCtl[MAX_CH][SUB_CH][CMD_CTL_NUM];
  UINT8     Clk[MAX_CH][CLK_NUM];
  UINT8     Data[MAX_CH][MAX_STROBE];
} RESULTS_STRUCT;

/**

  Enable or Disable crossover calibration.

  @param Host   - Pointer to sysHost, the system Host (root) structure struct
  @param Socket - Current CPU socket
  @param Ch     - Current channel
  @param State  - Enable = 1, Disable = 0

  @retval SUCCESS

**/
EFI_STATUS
EnableDisableXoverCalRegs (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch,
  IN  UINT8    State
  );

/**

  Initialize registers used during crossover calibration.

  @param Host                   - Pointer to sysHost, the system Host (root) structure struct
  @param Ch                     - Current channel
  @param COUNT1_STRUCT          - Structure containing Count1 variables
  @param OFFSET_STRUCT          - Structure containing offset variables
  @param PREVIOUS_STATUS_STRUCT - Structure containing previous status variables

  @retval SUCCESS

**/
EFI_STATUS
InitializeLocalsPerCh (
  IN  PSYSHOST                Host,
  IN  UINT8                   Ch,
  OUT COUNT1_STRUCT           *Count1,
  OUT OFFSET_STRUCT           *OffsetSt,
  OUT PREVIOUS_STATUS_STRUCT  *PreviousStatus,
  OUT RESULTS_STRUCT          *Results
  );

/**

  Set piDelay value to test crossover calibration.

  @param Host       - Pointer to sysHost, the system Host (root) structure struct
  @param Socket     - Current CPU socket
  @param Ch         - Current channel
  @param PiDelay    - PiDelay

  @retval SUCCESS

**/
EFI_STATUS
SetCmdRefPiClkDelay (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT16   PiDelay
  );

/**

  Get multiple samples based on the current piDelay value used to test crossover calibration.

  @param Host       - Pointer to sysHost, the system Host (root) structure struct
  @param Socket     - Current CPU socket
  @param Ch         - Current channel
  @param PiDelay    - PiDelay
  @param Results    - Pointer to the struct containing the results of crossover calibration

  @retval NA

**/
VOID
GetCrossoverCalibrationResults (
  IN  PSYSHOST             Host,
  IN  UINT8                Socket,
  IN  UINT8                Ch,
  IN  UINT16               PiDelay,
  OUT RESULTS_STRUCT       *Results
  );

/**

  Display the results from crossover calibration for this PiDelay.

  @param Host           - Pointer to sysHost, the system Host (root) structure struct
  @param Ch             - Current channel
  @param PiDelay        - PiDelay
  @param Results        - Pointer to the struct containing the results of crossover calibration
  @param Offset         - Pointer to the struct containing the offsets for crossover calibration
  @param Count1         - Pointer to the struct containing the Count of positive results
  @param PreviousStatus - Pointer to the struct containing the Count of positive results

  @retval SUCCESS

**/
EFI_STATUS
DisplayCrossoverCalibrationResults (
  IN  PSYSHOST                 Host,
  IN  UINT8                    Ch,
  IN  UINT16                   PiDelay,
  IN  RESULTS_STRUCT           *Results,
  IN  OFFSET_STRUCT            *OffsetSt,
  OUT COUNT1_STRUCT            *Count1,
  OUT PREVIOUS_STATUS_STRUCT   *PreviousStatus
  );

/**

  Program the offsets found during crossover calibration.

  @param Host       - Pointer to sysHost, the system Host (root) structure struct
  @param Socket     - Current CPU socket
  @param Ch         - Current channel
  @param OffsetSt   - Pointer to the struct containing the offsets found during crossover calibration

  @retval SUCCESS

**/
EFI_STATUS
ProgramOffsets (
  IN  PSYSHOST             Host,
  IN  UINT8                Socket,
  IN  UINT8                Ch,
  IN  OFFSET_STRUCT        *OffsetSt
  );

/**

  Peform crossover calibration using CRIF version 17ww37e or later.

  @param Host - Pointer to sysHost, the system Host (root) structure struct

  @retval SUCCESS

**/
UINT32
XoverCalib (
  PSYSHOST  Host
  );
#else // #if (MEM_IP_VER >= MEM_IP_17ww37e)
/**

  Peform crossover calibration CRIF versions prior to 17ww37e.

  @param Host - Pointer to sysHost, the system Host (root) structure struct

  @retval SUCCESS

**/
UINT32
XoverCalibPre17ww37e (
  PSYSHOST  Host
  );

#endif // #if (MEM_IP_VER >= MEM_IP_17ww37e)


//
// Function prototypes that are private to this lib
//

/**

  Peform crossover calibration

  @param Host - Pointer to sysHost, the system Host (root) structure struct

  @retval SUCCESS

**/
UINT32 CrossoverCalib10nm (PSYSHOST Host);

/**
  Return Xover offsets for the current DDR frequency and channel

  @param[in]   Socket        - Socket number
  @param[in]   Ch            - Channel number
  @param[out]  XoverOffsets  - Pointer to caller's offset structure pointer

  @retval  EFI_SUCCESS if offsets found
           EFI_NOT_FOUND or EFI_INVALID_PARAMETER otherwise
**/
EFI_STATUS
GetXoverOffsets (
  IN    UINT8           Socket,
  IN    UINT8           Ch,
  OUT   XOVER_OFFSETS   **XoverOffsets
  );

/**
  RcSim Cmd Xover Feedback build target function.

  @param[in]   SubCh               Sub channel.
  @param[in]   PiDelay             PiDelay.
  @param[out]  DdrclkTrainResult0  Ptr to training struct.
  @param[out]  DdrccTrainResult0   Ptr to training struct.

  @retval EFI_SUCCESS           Handled successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
RcSimCmdXoverFeedbackTarget (
  IN UINT8                                      SubCh,
  IN UINT16                                     PiDelay,
  OUT DDRCLK_TRAIN_RESULT0_MCIO_DDRIOEXT_STRUCT *DdrclkTrainResult0,
  OUT DDRCC_TRAIN_RESULT0_MCIO_DDRIOEXT_STRUCT  *DdrccTrainResult0
  );

#endif   // __MEM_XOVER_CALIB_H__
