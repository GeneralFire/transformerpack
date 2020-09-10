/** @file
  Interface of imc routine library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef __IMC_ROUTING_LIB_H__
#define __IMC_ROUTING_LIB_H__

#include <SiliconSetting.h>
#include <Library/CrystalRidgeLib.h>
#include <MemCommon.h>
#include <Library/MemMcIpLib.h>

#define  PCLS_INDEX        0
#define  ADDDC_INDEX       1
#define  RANK_SPARE_INDEX  2
#define  SDDC_INDEX        3
#define  ECC_FLOW_SWITCH   4
#define  INDEPENDENT_INDEX 5
#define  END_INDEX         6

#define MAX_ALLOWED_TRANSACTION_TYPE   64

#define NUM_PAYLOAD_REG             32

#define ONE_SECOND_TIME_IN_NANOSEC  1000000000
#define TEN_SECOND_TIME_IN_NANOSEC  10000000000

typedef enum _AP_DIMM_STATE {
  AP_STOP = 0,
  AP_ERROR,
  AP_IN_PROGRESS,
  AP_FINISHED
} AP_DIMM_STATE;

//
//0 is highest priority, 3 is the lowest priority
//
typedef union {
  struct {
    UINT32 rank_exist : 1;
    /* rank_exist - Bits[0],
    1, for spare rank, it exist; for non-spare rank, it means it CE overflow happened.
    0, for spare rank, it doesn't exist; for non-spare rank, it means it didn't CE overflow happened.
     */
    UINT32 rank_fail : 1;
    /* rank_fail - Bits[1],
    1, CE overflow on this rank.
    0, CE is not overflow.
     */
    UINT32 rsvd_1 : 6;
    /* rsvd_8 - Bits[7:2],
     */
    UINT32 non_spare_rank : 1;
    /* non_spare_rank - Bits[8],
    it is a non_spare_rank.
     */
    UINT32 non_spare_status : 4;
    /* non_spare_status - Bits[12:9], n/a, default = n/a
       Padding added by header generation tool.
     */
    UINT32 rsvd_2 : 3;
    /* rsvd_8 - Bits[15:13],
     */
    UINT32 spare_rank:1;
    /* spare_rank - Bits[16],
     */
    UINT32 spare_rank_status:2;
    /* spare_rank_status - Bits[18:17],
     */
    UINT32 rsvd_3: 5;
    /* rsvd_3 - Bits[23:19],
     */
     UINT32 spare_rank_sequence:2;
    /* spare_rank_sequence - Bits[25:24],
    0 is highest priority, 3 is the lowest priority
     */
    UINT32 rsvd_4: 6;
    /* rsvd_4- Bits[31:26],
     */
  } Bits;
  UINT32 Data;
} RANK_STATUS_TYPE;

//
// RAS Event type
//
typedef enum {
  NEW_EVENTS,             // New events (events yet to be handled)
  EVENTS_IN_PROGRESS,     // Events that are in progress
} EVENT_TYPE;

typedef struct _MEDIA_ERR_LOG_DATA_RSP {
  MEDIA_ERR_LOG_DATA MediaErrLog;
  EFI_STATUS         ErrorMediaLogStatus;
  UINT16             NewSeqNumber;
} MEDIA_ERR_LOG_DATA_RSP;

typedef struct _THERMAL_ERR_LOG_DATA_RSP {
  THERMAL_ERR_LOG_DATA  ThermalErrLog;
  EFI_STATUS            ErrorThermalLogStatus;
  UINT16                NewSeqNumber;
} THERMAL_ERR_LOG_DATA_RSP;

typedef struct _DIMM_ERROR_LOG_DATA {
  THERMAL_ERR_LOG_DATA_RSP    ThermalErrLogDataRsp[MAX_THERMAL_PAYLOADS];
  MEDIA_ERR_LOG_DATA_RSP      MediaErrLogDataRsp[MAX_MEDIA_PAYLOADS];
  UINT32                      LongOpRsp[NUM_PAYLOAD_REG];
  UINT32                      LongOpSeqNum;
  EFI_STATUS                  LongOpStatus;
  EFI_STATUS                  DetectErrorStatus;
  DDRT_EVENT_DATA             DetectErrorData;
  UINT16                      MediaErrSeqFirst;
  UINT16                      ThermalErrSeqFirst;
  BOOLEAN                     ProcessLongOp;
  BOOLEAN                     ProcessMediaErrLogData;
  BOOLEAN                     ProcessThermalErrLogData;
  BOOLEAN                     ProcessFwHang;
  BOOLEAN                     ProcessFwFatalErr;
} DIMM_ERROR_LOG_DATA;

typedef
BOOLEAN (*MEM_RAS_EVENT_HNDLR) (
    IN UINT8        NodeId,
    IN EVENT_TYPE   EventType
 );

#pragma  pack(1)
typedef struct {
  UINT32               LastErrorTime[MC_MAX_NODE][MAX_MC_CH][MAX_RANK_CH * SUB_CH];
  UINT32               RankErrorCounter[MC_MAX_NODE][MAX_MC_CH][MAX_RANK_CH * SUB_CH];
  UINT32               ErrorInterval;
  UINT32               RankIntervalErrorrCnt;
} CE_INTERVAL_ERROR_STRUCT;

typedef struct {
  UINT32                    EventProgressBitmap;
  RANK_STATUS_TYPE          RankStatus[MC_MAX_NODE][MAX_MC_CH][MAX_RANK_CH * SUB_CH];
  CE_INTERVAL_ERROR_STRUCT  RankIntervalError;
  MEM_RAS_EVENT_HNDLR       RasEventHndlrTable[END_INDEX+1];
  UINT16                    HostAlerts[MAX_ALLOWED_TRANSACTION_TYPE];
  UINT8                     PatrolErrorDowngradeEn;
  BOOLEAN                   WaStatusS2207801634;
  UINT8                     DdrtInternalAlertEn;
  UINT8                     ReportAlertSPA;
  UINT8                     DcpmmUncPoison;
  BOOLEAN                   RankVlsAlways;
  UINT8                     DdrtPreviousErrorId[MAX_SOCKET][MAX_CH][MAX_DIMM][DDRT_SIGNAL_PRIORITY_EVENT_NUM];
  NVDIMM_INFO               DimmInfo[MAX_SOCKET][MAX_CH][MAX_DIMM];
  EFI_STATUS                DimmInfoStatus[MAX_SOCKET][MAX_CH][MAX_DIMM];
  AP_DIMM_STATE             ApState[MAX_SOCKET];
  DIMM_ERROR_LOG_DATA       DimmErrorLogData[MAX_SOCKET][MAX_CH][MAX_DIMM][LOG_LEVEL_NUM];
  UINT16                    FnvSequenceNumberMatrix[MAX_SOCKET][MAX_CH][MAX_DIMM][LOG_TYPE_NUM][LOG_LEVEL_NUM];
  UINT64                    GetErrorLogTimeoutCounter;
  UINT8                     TriggerSWErrThEn;
  UINT16                    SpareSwErrTh;
  UINT16                    TimeWindow;
  BOOLEAN                   CloakingEn;
  UINT16                    SpareErrTh;
} IMC_RAS_STRUCTURE;

typedef struct{
  UINT16                Counter;
  UINT16                HeadOfTsc;
  UINT32               *Tsc;
} ENHANCED_ERROR_RECORD_DATA;
#pragma  pack()

VOID
EFIAPI
InitEnhancedDimmErrRecList (
  UINT16    Threshold
  );

BOOLEAN
EFIAPI
UpdateEnhancedDimmErrRecord (
  IN    UINT8     Node,
  IN    UINT16    Threshold,
  IN    UINT16    TimeWindow
  );

BOOLEAN
EFIAPI
CheckImcInterruptInUbox (
  IN UINT8  Socket
  );


VOID
EFIAPI
ClearImcInterruptInUbox (
  IN   UINT8     Socket
  );

VOID
EFIAPI
ConfigSystemRetryRegister (
  IN  UINT8   Socket
  );

VOID
EFIAPI
ImcCorrectableErrorEnable (
  IN  UINT8   Socket,
  IN  UINT8   SpareIntSelect
  );

BOOLEAN
EFIAPI
GetNewAndPendingError (
  IN      UINT8   Socket,
  IN OUT  UINT32  *NewErrorBitmap,
  IN OUT  UINT32  *PendingErrorBitmap
  );

VOID
EFIAPI
CorrectableMemoryErrorHandler (
  IN       UINT8    Socket,
  IN       UINT32  *NewErrorBitmap,
  IN       UINT32  *PendingErrorBitmap,
  IN       UINT8    MemCeFloodPolicy
  );





VOID
EFIAPI
InitializeRankSparing (
  VOID
  );

VOID
EFIAPI
InitializeIndependentMode (
  VOID
  );

VOID
EFIAPI
InitializeSDDCPlusOne (
  VOID
  );

BOOLEAN
EFIAPI
IsPatrolEnabled (
  VOID
  );

VOID
EFIAPI
CheckAndHandlePatrolEvent (
  IN   UINT8     Socket
  );


/**
    DDRT Error Handler

    @retval VOID
**/
VOID
EFIAPI
DdrtErrorHandler (
  VOID
  );

/**
  Enables error logging for DDRT errors

  @param Socket                      - Socket index
  @param ScrubEnable                 - DDRT scrub enable
  @param FnvErrorHighPrioritySignal  - high priority signal
  @param FnvErrorLowPrioritySignal   - low priority signal
  @param NgnHostAlertDpa             - Alert policy for Dpa error
  @param NgnHostAlertPatrolScrubUNC  - Alert policy for PatrolScrubUNC

  @retval None

**/
VOID
EFIAPI
DdrtEnableError (
  IN     UINT8    Socket,
  IN     UINT8    ScrubEnable,
  IN     UINT8    FnvErrorHighPrioritySignal,
  IN     UINT8    FnvErrorLowPrioritySignal,
  IN     UINT8    NgnHostAlertDpa,
  IN     UINT8    NgnHostAlertPatrolScrubUNC,
  IN OUT BOOLEAN  *DdrtErrorsEnabled
  );

/**
  This implementation is to get subrank value on certain Channel from retry register.

  @param[in]     Node    Memory controller index info per whole system
  @param[in]     ChOnNode  channel index on memory controller

  @retval     if SUCCESS, means DIMM error found; otherwise no DIMM error found.

**/
UINT8
EFIAPI
GetSubRank (
  IN      UINT8    Node,
  IN      UINT8    ChOnSkt
  );

/**
  Initialize ADDDC sparing handler
**/
EFI_STATUS
EFIAPI
InitAdddcSparing (
  VOID
  );

/**
  Initialize PCLS sparing handler
**/
EFI_STATUS
EFIAPI
InitPclsSparing (
  VOID
  );

/**
  Initialize ADDDC Error Injection in all enabled channels.

  @param  [in]  Socket    Socket to be programmed.

  @retval  None.

**/
VOID
InitializeAdddcErrInj (
  UINT8 IN  Socket
  );

IMC_RAS_STRUCTURE *
EFIAPI
GetImcRasData (
  VOID
  );


/**
  Save imc S3 RAS data.

  @param   none.
  @retval  none.

**/
VOID
InitImcRasS3Save (
  VOID
  );

/**
  This function will set up threshold values for number of retries for DDRT ECC Functionality

  @param[in]    Socket

  @retval    None

**/
VOID
EFIAPI
DcpmmEccModeInit (
  UINT8 Socket
  );

/**
  This function will set up  threshold for host memory controller to switch form ECC Mode A to ECC Mode B when there is erroneous lane
  We will setup the error threshold value of the DDRT rank with a threshold value that will indicate how many errors will be
  corrected before determining the erroneous lane and switching over to DDRT ECC Mode B which will perform error correction
  without attempting retry for this lane.

  @param[in]    Socket

  @retval    None

**/
VOID
EFIAPI
DcpmmEccModeSwitchSetup  (
  UINT8 Socket
  );

/**
  This function will perform the switch in Ecc Flow for far memory controller from Ecc Flow A to Flow B.
  This is applicable for error from DDRT Dimm

  @param  NodeId          - Memory controller ID
  @param  EventType       - NEW_EVENTS or EVENTS_IN_PROGRESS

  @retval                 - TRUE -  event was handled
  @retval                 - FALSE - event not handled

**/
BOOLEAN
EFIAPI
DcpmmEccModeSwitchAction  (
  IN  UINT8         Node,
  IN  EVENT_TYPE    EventType
  );
#endif
