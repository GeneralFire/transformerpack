/** @file
  Implementation for corrected error handling for Far Memory Controller.

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
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <IioUniversalData.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/CrystalRidgeLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/PlatPolicyLib.h>

#include "ImcRoutingPrivate.h"
#include <Library/RasDebugLib.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/AddressDecodeLib.h>
#include <Library/PlatMemErrorDataLib.h>
#include <Library/PlatMemErrorDataLib.h>
#include <Library/ErrorReportLib.h>
#include <IndustryStandard/Acpi.h>
#include <Library/MemFuncLib.h>
#include <Library/MemRcLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/RasGlobalDataLib.h>

#include <Library/TimerLib.h>
#include <Library/MpExtensionLib.h>
#include <Library/RasRegisterClearLib.h>

#define DDRT_ECC_MODE_A 0x1F
#define WRITE_WRITE_DCLK_WA 0x9

#define FAILED_NIBBLE_HISTORY_COUNT     3     // keep the last N failed nibbles
#define FAILED_NIBBLE_HISTORY_THRESHOLD 2     // do mode switch when M match
#define INVALID_FAILED_NIBBLE           0xFF  // invalid value for failed nibble

///
/// Tracker for failed nibble of one socket-channel-rank.
///
typedef struct {
  UINT32  Index;                                ///< buffer index to fill next
  UINT8   History[FAILED_NIBBLE_HISTORY_COUNT]; ///< circular buffer
} FAILED_NIBBLE_TRACKER;

///
/// Failed nibble data.
///
typedef struct {
  FAILED_NIBBLE_TRACKER Tracker[MAX_SOCKET][MAX_CH][MAX_RANK_CH * SUB_CH];
} FAILED_NIBBLE_DATA;

FAILED_NIBBLE_DATA  *mFailedNibbleData = NULL;

CONST UINT16 mErrorTypeBitmap[5] = {ALERT_POLICY_UNC_EN, ALERT_POLICY_DPA_ERR_EN, ALERT_POLICY_AIT_ERR_EN, ALERT_POLICY_DATAPATH_ERR_EN, ALERT_POLICY_ILL_ACC_EN};
IMC_RAS_STRUCTURE   *mImcRas = NULL;

HOST_ALERT_POLICY mHostAlertPolicySmi[NUM_HOST_TRANSACTIONS] = {
//{LogLevel,      Rsvd,  Rsvd0,     DpaErrViralEn, DpaErrINTEn,  UncErrViralEn, UncErrINTEn, DataPathErrINTEn, DataPathErrViralEn, IllegalAccessINTEn, IllegalAccessViralEn, Rsvd1, Rsvd2, Rsvd3, Rsvd4},
  {HIGH_PRIORITY, 0,     0,         DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  ENABLE_FLAG,  ENABLE_FLAG,     DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_2LM_READ
  {HIGH_PRIORITY, 0,     0,         DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  ENABLE_FLAG,  ENABLE_FLAG,     DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_2LM_WRITE
  {HIGH_PRIORITY, 0,     0,         DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  ENABLE_FLAG,  ENABLE_FLAG,     DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_PM_READ
  {HIGH_PRIORITY, 0,     0,         DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  ENABLE_FLAG,  ENABLE_FLAG,     DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_PM_WRITE
  {HIGH_PRIORITY, 0,     0,         DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,    DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_BW_READ
  {HIGH_PRIORITY, 0,     0,         DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,    DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_BW_WRITE
  {HIGH_PRIORITY, 0,     0,         DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,    DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_AIT_READ
  {HIGH_PRIORITY, 0,     0,         DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,    DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_AIT_WRITE
  {HIGH_PRIORITY, 0,     0,         DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  ENABLE_FLAG,  ENABLE_FLAG,     DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_WEAR_LEVEL_MOVE
  {HIGH_PRIORITY, 0,     0,         DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  ENABLE_FLAG,  ENABLE_FLAG,     DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_PATROL_SCRUB
  {LOW_PRIORITY,  0,     0,         DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,    DISABLE_FLAG,       ENABLE_FLAG,        DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_CSR_READ
  {LOW_PRIORITY,  0,     0,         DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,    DISABLE_FLAG,       ENABLE_FLAG,        DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_CSR_WRITE
  {HIGH_PRIORITY, 0,     0,         DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,    DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_ADDRESS_RANGE_SCRUB
  {HIGH_PRIORITY, 0,     0,         DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  ENABLE_FLAG,  ENABLE_FLAG,     DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}  // FNV_TT_MEDIA_TRANSACTION_TYPE_UNAVAILABLE
};

HOST_ALERT_POLICY mHostAlertPolicyNoSmi[NUM_HOST_TRANSACTIONS] = {
//{LogLevel,      Rsvd, Rsvd0,  DpaErrViralEn, DpaErrINTEn,  UncErrViralEn, UncErrINTEn, DataPathErrINTEn, DataPathErrViralEn, IllegalAccessINTEn, IllegalAccessViralEn, Rsvd1, Rsvd2, Rsvd3, Rsvd4},
  {HIGH_PRIORITY, 0,    0,      DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  ENABLE_FLAG,  ENABLE_FLAG,     DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_2LM_READ
  {HIGH_PRIORITY, 0,    0,      DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  ENABLE_FLAG,  ENABLE_FLAG,     DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_2LM_WRITE
  {HIGH_PRIORITY, 0,    0,      DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  ENABLE_FLAG,  ENABLE_FLAG,     DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_PM_READ
  {HIGH_PRIORITY, 0,    0,      DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  ENABLE_FLAG,  ENABLE_FLAG,     DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_PM_WRITE
  {HIGH_PRIORITY, 0,    0,      DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  ENABLE_FLAG,  ENABLE_FLAG,     DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_BW_READ
  {HIGH_PRIORITY, 0,    0,      DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  ENABLE_FLAG,  ENABLE_FLAG,     DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_BW_WRITE
  {HIGH_PRIORITY, 0,    0,      DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  ENABLE_FLAG,  ENABLE_FLAG,     DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_AIT_READ
  {HIGH_PRIORITY, 0,    0,      DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  ENABLE_FLAG,  ENABLE_FLAG,     DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_AIT_WRITE
  {HIGH_PRIORITY, 0,    0,      DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  ENABLE_FLAG,  ENABLE_FLAG,     DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_WEAR_LEVEL_MOVE
  {HIGH_PRIORITY, 0,    0,      DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,    DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_PATROL_SCRUB
  {LOW_PRIORITY,  0,    0,      DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,    DISABLE_FLAG,       ENABLE_FLAG,        DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_CSR_READ
  {LOW_PRIORITY,  0,    0,      DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,    DISABLE_FLAG,       ENABLE_FLAG,        DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_CSR_WRITE
  {HIGH_PRIORITY, 0,    0,      DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,    DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}, // FNV_TT_ADDRESS_RANGE_SCRUB
  {HIGH_PRIORITY, 0,    0,      DISABLE_FLAG,  DISABLE_FLAG, DISABLE_FLAG,  ENABLE_FLAG,  ENABLE_FLAG,     DISABLE_FLAG,       DISABLE_FLAG,       DISABLE_FLAG,         0,     0,     0,     0}  // FNV_TT_MEDIA_TRANSACTION_TYPE_UNAVAILABLE
};


VOID
EFIAPI
InitializeHostAlert (
  IN   UINT8     NgnHostAlertDpa,
  IN   UINT8     NgnHostAlertPatrolScrubUNC,
  IN   UINT16*   HostAlerts,
  IN   BOOLEAN   SmiEn
  )
{
  UINT8                 TransType;
  HOST_ALERT_POLICY     *AlertPolicy;

  RAS_DEBUG((LEVEL_BASIC_FLOW, RAS_INFO_STR ("DDRT") "InitializeHostAlert: Function called\n"));

  AlertPolicy = SmiEn ? mHostAlertPolicySmi : mHostAlertPolicyNoSmi;

  for (TransType = 0; TransType < NUM_HOST_TRANSACTIONS; TransType++) {
    if (NgnHostAlertDpa != 0x00) {
      AlertPolicy[TransType].DpaErrInterruptEn = ENABLE_FLAG;
    }
    if ((TransType == FNV_TT_PATROL_SCRUB) && (NgnHostAlertPatrolScrubUNC != 0x00)) {
      AlertPolicy[TransType].UncErrInterruptEn = ENABLE_FLAG;
    }
    HostAlerts[TransType] = *(UINT16*) (AlertPolicy + TransType);
  }
}

BOOLEAN
IsAlertEnabled (
  UINT16        Policy,
  UINT8         ErrorIndex
  )
{
  RAS_DEBUG ((LEVEL_FUNC_FLOW, RAS_ERROR_STR ("DDRT") "Alert for transaction type(Policy) %xh with error type(ErrorIndex) %xh\n", Policy, ErrorIndex));
  if (ErrorIndex >= ARRAY_SIZE (mErrorTypeBitmap)) {
    RAS_ASSERT (ErrorIndex < ARRAY_SIZE (mErrorTypeBitmap));
    return FALSE;
  }

  if ((Policy & (BIT1 | BIT0)) == 0) {
    return FALSE;
  }
  return (Policy & (mErrorTypeBitmap[ErrorIndex])) != 0;
}

/**
  Enables error logging for DDRT errors

  @param Socket                     - Socket index
  @param ScrubEnable                - DDRT scrub enable
  @param FnvErrorHighPrioritySignal - high priority signal
  @param FnvErrorLowPrioritySignal  - low priority signal
  @param NgnHostAlertDpa            - Alert policy for Dpa error
  @param NgnHostAlertPatrolScrubUNC - Alert policy for PatrolScrubUNC

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
  )
{
  UINT8                ChOnSkt;
  UINT8                Dimm;
  RETURN_STATUS        Status;
  MEM_TOPOLOGY        *MemTopology;

  mImcRas = GetImcRasData ();
  if (mImcRas == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, RAS_INFO_STR ("DDRT") "mImcRas == NULL\n"));
    ASSERT(FALSE);
    return;
  }

  MemTopology = GetMemTopology ();
  if (MemTopology == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, RAS_INFO_STR ("DDRT") "MemTopology == NULL\n"));
    ASSERT(FALSE);
    return;
  }

  if ((FnvErrorLowPrioritySignal == NO_LOG) && (FnvErrorHighPrioritySignal == NO_LOG)) {
    return;
  }
  RAS_DEBUG((LEVEL_BASIC_FLOW, RAS_INFO_STR ("DDRT") "DdrtEnableError: Function called\n"));

  if (mImcRas->DdrtInternalAlertEn) {
    InitializeHostAlert (
      NgnHostAlertDpa,
      NgnHostAlertPatrolScrubUNC,
      mImcRas->HostAlerts,
      FnvErrorHighPrioritySignal == SMI_FLAG
      );
  }

  for (ChOnSkt = 0; ChOnSkt < MAX_CH; ChOnSkt++) {
    if (IsChannelPresentPerSocket (Socket, ChOnSkt) == FALSE) {
      continue;
    }
    EnableInterruptForDdrtAlerts (Socket, ChOnSkt);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (!IsDcpmm (Socket,ChOnSkt,Dimm)) {
        continue;
      }
      Status = SendFnvSetHostAlert (Socket, ChOnSkt, Dimm, mImcRas->HostAlerts);
      if (EFI_ERROR (Status)) {
        continue;
      }

      Status = McDdrtErrorLogProgramSignal (Socket, ChOnSkt, Dimm, LOW_PRIORITY_EVENT, FnvErrorLowPrioritySignal);
      ASSERT_EFI_ERROR (Status);

      Status = McDdrtErrorLogProgramSignal (Socket, ChOnSkt, Dimm, HIGH_PRIORITY_EVENT, FnvErrorHighPrioritySignal);
      ASSERT_EFI_ERROR (Status);

      if ((FnvErrorLowPrioritySignal & DDRT_SIGNAL_SMI_FLAG) ||
          (FnvErrorHighPrioritySignal & DDRT_SIGNAL_SMI_FLAG)) {

        *DdrtErrorsEnabled = TRUE;
      }

      //
      //tocheck: DDRT_FNV_INTR_CTL_MCDDC_DP_REG
      //
    } // for (Dimm)
  } // for (Ch)
}

/**
 * @brief Classify NVDIMM access error.
 *
 * @return 'True' if error is fatal, 'false' otherwise.
**/
BOOLEAN
DdrtNvdimmFisErrorIsFatal (
  IN EFI_STATUS Status
  )
{
  switch (Status) {
    case EFI_DEVICE_ERROR:
    case EFI_NO_RESPONSE:
    case EFI_NO_MEDIA:
    case EFI_COMPROMISED_DATA:
      return TRUE;
    default:
      return FALSE;
  }
}

/**
  Query and process NVMDIMM thermal log.

  This function will only handle correctable errors.

  @param[in]  Socket      - The socket
  @param[in]  Ch          - The channel
  @param[in]  Dimm        - The NVMDIMM
  @param[in]  Priority    - The priority of thermal log to query

  @retval None
**/
VOID
ProcessThermalLog (
  IN UINT8     CONST  Socket,
  IN UINT8     CONST  Ch,
  IN UINT8     CONST  Dimm,
  IN UINT8     CONST  Priority
  )
{
  EFI_PLATFORM_MEMORY_ERROR_DATA  MemRecord;
  RAS_DDRT_ALERT_ERROR_LOG        DdrtAlertErrorLog;
  IMC_RAS_STRUCTURE               *ImcRas;
  THERMAL_ERR_LOG                 *ThermalLogPtr;
  EFI_STATUS                      Status;
  UINT32                          VerboseLevel = LEVEL_REG;
  UINT16                          *SequenceNumber;
  UINT16                          i;
  UINT8                           LogsIndex = 0;

  ImcRas = GetImcRasData ();

  SequenceNumber = &ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ThermalErrSeqFirst;
  RAS_DEBUG ((LEVEL_REG, RAS_INFO_STR ("DDRT") "ProcessThermalLog: S%d.C%d.D%d P%d: ", Socket, Ch, Dimm, Priority));
  RAS_DEBUG ((LEVEL_REG, "checking thermal log starting at sequence number %d\n", *SequenceNumber));

  for (LogsIndex = 0; LogsIndex < MAX_THERMAL_PAYLOADS; LogsIndex++) {
    if (LogsIndex > MAX_THERMAL_PAYLOADS / 2) {
      //
      // We should never have loop more times the max required loop. If so, turn on more debug messages
      //
      VerboseLevel = LEVEL_BASIC_FLOW;
    }

    Status = ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ThermalErrLogDataRsp[LogsIndex].ErrorThermalLogStatus;
    if (Status == EFI_NOT_FOUND || ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ThermalErrLogDataRsp[LogsIndex].ThermalErrLog.ReturnCount == 0) {
      RAS_DEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "ProcessThermalLog: S%d.C%d.D%d P%d: ", Socket, Ch, Dimm, Priority));
      RAS_DEBUG ((VerboseLevel, "no thermal logs found, new sequence number: %d\n",
                  ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ThermalErrLogDataRsp[LogsIndex].NewSeqNumber));
      break;
    }

    if (EFI_ERROR (Status)) { //We should break the while loop whenever there is error status and already retried.
      RAS_DEBUG ((LEVEL_BASIC_FLOW, RAS_INFO_STR ("DDRT") "ProcessThermalLog: S%d.C%d.D%d P%d: ", Socket, Ch, Dimm, Priority));
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "NvmCtlrGetErrorLog (..., THERMAL_LOG_REQ, ..., %d, ...) failed: %r\n", *SequenceNumber, Status));
      FillFnvErrorData (Socket, Ch, Dimm, EFI_PLATFORM_MEMORY_ERROR_UNKNOWN, NULL, FALSE, &MemRecord);
      Status = ReportMemErrorLog (DdrtNvdimmFisErrorIsFatal (Status)? EFI_ACPI_6_2_ERROR_SEVERITY_FATAL : EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED, &MemRecord);
      break;
    }

    RAS_DEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "ProcessThermalLog: S%d.C%d.D%d P%d: ", Socket, Ch, Dimm, Priority));
    RAS_DEBUG ((VerboseLevel, "found %d thermal log(s), new sequence number: %d\n",
                ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ThermalErrLogDataRsp[LogsIndex].ThermalErrLog.ReturnCount,
                ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ThermalErrLogDataRsp[LogsIndex].NewSeqNumber));

    for (i = 0; (i < ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ThermalErrLogDataRsp[LogsIndex].ThermalErrLog.ReturnCount) && (i < MAX_THERMAL_LOGS); i++) {
      ThermalLogPtr = &ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ThermalErrLogDataRsp[LogsIndex].ThermalErrLog.ThermalLogs[i];
      RAS_DEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "ProcessThermalLog: S%d.C%d.D%d P%d: ", Socket, Ch, Dimm, Priority));
      RAS_DEBUG ((VerboseLevel, "thermal log: seq: %d, timestamp: %lld, temp type %d\n",
                  ThermalLogPtr->SequenceNumber,
                  ThermalLogPtr->SysTimestamp,
                  ThermalLogPtr->TempType));
      RAS_DEBUG ((VerboseLevel, "                                                     "
                                "temp: 0x%x, sign: %d, reported: %d\n",
                  ThermalLogPtr->Temperature,
                  ThermalLogPtr->Sign,
                  ThermalLogPtr->Reported));

      DdrtAlertErrorLog.ThermalLog.Reported = (UINT8) ThermalLogPtr->Reported;
      DdrtAlertErrorLog.ThermalLog.TempType = (UINT8) ThermalLogPtr->TempType;
      ReportDdrtAlertErrorLog (Socket, Ch, Dimm, DdrtAlertErrotTypeThermalErrorLog, &DdrtAlertErrorLog);

      //
      // SMART Health Change, ACPI Notification 0x81 to NVDIMM leaf node device
      // Thermal Error Log: Reported: User Alarm Trip
      // WHEA Logging: None generated
      //
      if (ThermalLogPtr->Reported == FNV_HRTD_REPORTED_USER_ALARM_TRIP) {
        FnvAcpiAddLeafNotification (Socket, Ch, Dimm);
        continue;
      }
    }

    if (ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ThermalErrLogDataRsp[LogsIndex].ThermalErrLog.ReturnCount !=
        ARRAY_SIZE (ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ThermalErrLogDataRsp[LogsIndex].ThermalErrLog.ThermalLogs)) {
      break;
    }
  }

  if (LogsIndex == MAX_THERMAL_PAYLOADS) {
    RAS_DEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "ProcessThermalLog: S%d.C%d.D%d P%d: ", Socket, Ch, Dimm, Priority));
    RAS_DEBUG ((DEBUG_ERROR, "thermal log loop reached the limit.\n"));
  }
}

/**
  Query and process NVMDIMM media log.

  This function will only handle correctable errors.

  @param[in]  Socket      - The socket
  @param[in]  Ch          - The channel
  @param[in]  Dimm        - The NVMDIMM
  @param[in]  Priority    - The priority of media log to query

  @retval None
**/
VOID
ProcessMediaLog (
  IN UINT8     CONST  Socket,
  IN UINT8     CONST  Ch,
  IN UINT8     CONST  Dimm,
  IN UINT8     CONST  Priority
  )
{
  EFI_PLATFORM_MEMORY_ERROR_DATA  MemRecord;
  RAS_DDRT_ALERT_ERROR_LOG        DdrtAlertErrorLog;
  IMC_RAS_STRUCTURE              *ImcRas;
  EFI_STATUS                      Status;
  MEDIA_ERR_LOG                  *MediaLogPtr;
  volatile UINT64                *PhysicalAddressPointer;
  UINT32                          VerboseLevel = LEVEL_REG;
  UINT16                         *SequenceNumber;
  UINT16                          i;
  UINT8                           MemErrorType = EFI_PLATFORM_MEMORY_ERROR_UNKNOWN;
  UINT8                           MemErrorSev = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED;
  UINT8                           LogsIndex = 0;

  ImcRas = GetImcRasData ();

  RAS_DEBUG ((LEVEL_REG, "%a: MemErrorSev %d\n", __FUNCTION__, MemErrorSev));

  SequenceNumber = &ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrSeqFirst;
  RAS_DEBUG ((LEVEL_REG, RAS_INFO_STR ("DDRT") "ProcessMediaLog: S%d.C%d.D%d P%d: ", Socket, Ch, Dimm, Priority));
  RAS_DEBUG ((LEVEL_REG, "checking media log starting at sequence number %d\n", *SequenceNumber));

  for (LogsIndex = 0; LogsIndex < MAX_MEDIA_PAYLOADS; LogsIndex++) {
    if (LogsIndex > MAX_MEDIA_PAYLOADS / 2) {
      //
      // We should never have loop more times the max required loop. If so, turn on more debug messages
      //
      VerboseLevel = LEVEL_BASIC_FLOW;
    }
    Status = ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrLogDataRsp[LogsIndex].ErrorMediaLogStatus;
    if (Status == EFI_NOT_FOUND || ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrLogDataRsp[LogsIndex].MediaErrLog.ReturnCount == 0) {
      //
      // This is the case when numbers of new media log is times of MAX_MEDIA_LOGS(3)
      //
      RAS_DEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "ProcessMediaLog: S%d.C%d.D%d P%d: ", Socket, Ch, Dimm, Priority));
      RAS_DEBUG ((VerboseLevel, "no media logs found, new sequence number: %d\n",
                  ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrLogDataRsp[LogsIndex].NewSeqNumber));
      break;
    }

    if (EFI_ERROR (Status)) {
      //
      // We should break the while loop whenever there is error status and already retried
      //
      RAS_DEBUG ((LEVEL_BASIC_FLOW, RAS_ERROR_STR ("DDRT") "ProcessMediaLog: S%d.C%d.D%d P%d: ", Socket, Ch, Dimm, Priority));
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "NvmCtlrGetErrorLog (..., MEDIA_LOG_REQ, ..., %d, ...) failed: %r\n", *SequenceNumber, Status));
      FillFnvErrorData (Socket, Ch, Dimm, EFI_PLATFORM_MEMORY_ERROR_UNKNOWN, NULL, FALSE, &MemRecord);
      Status = ReportMemErrorLog (DdrtNvdimmFisErrorIsFatal (Status)? EFI_ACPI_6_2_ERROR_SEVERITY_FATAL : EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED, &MemRecord);
      if (EFI_ERROR (Status)) {
        RAS_DEBUG ((LEVEL_BASIC_FLOW, RAS_ERROR_STR ("DDRT") "Failed to report the memory error record (%r)\n", Status));
      }
      break;
    }

    RAS_DEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "ProcessMediaLog: S%d.C%d.D%d P%d: ", Socket, Ch, Dimm, Priority));
    RAS_DEBUG ((VerboseLevel, "found %d media logs, new sequence number: %d\n",
                ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrLogDataRsp[LogsIndex].MediaErrLog.ReturnCount,
                ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrLogDataRsp[LogsIndex].NewSeqNumber));

    for (i = 0; (i < ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrLogDataRsp[LogsIndex].MediaErrLog.ReturnCount) && (i < MAX_MEDIA_LOGS); i++) {
      MediaLogPtr = &ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrLogDataRsp[LogsIndex].MediaErrLog.MediaLogs[i];
      RAS_DEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "ProcessMediaLog: S%d.C%d.D%d P%d: ", Socket, Ch, Dimm, Priority));
      RAS_DEBUG ((VerboseLevel, "media log: seq: %d, timestamp: %lld, error type %d\n",
                  MediaLogPtr->SequenceNumber,
                  MediaLogPtr->SysTimeStamp,
                  MediaLogPtr->ErrorType));
      RAS_DEBUG ((VerboseLevel, "                                                        "));
      RAS_DEBUG ((VerboseLevel, "error flags: 0x%2x, transaction type: %d\n",
                  MediaLogPtr->ErrorFlags,
                  MediaLogPtr->TransactionType));
      RAS_DEBUG ((VerboseLevel, "                                                        "));
      RAS_DEBUG ((VerboseLevel, "dpa: 0x%llx, pda: 0x%llx, range: %d\n",
                  MediaLogPtr->Dpa,
                  MediaLogPtr->Pda,
                  MediaLogPtr->Range));

      DdrtAlertErrorLog.MediaLog.ErrorType = MediaLogPtr->ErrorType;
      DdrtAlertErrorLog.MediaLog.TransactionType = MediaLogPtr->TransactionType;

      if ((MediaLogPtr->ErrorFlags & VALID_DPA) && (ImcRas->ReportAlertSPA)) {
        DdrtAlertErrorLog.MediaLog.Dpa = MediaLogPtr->Dpa;
      } else {
        DdrtAlertErrorLog.MediaLog.Dpa = 0;
      }
      ReportDdrtAlertErrorLog (Socket, Ch, Dimm, DdrtAlertErrotTypeMediaErrorLog, &DdrtAlertErrorLog);

      //
      // 6)SMART Health Change, ACPI Notification 0x81 to NVDIMM leaf node device
      // Valid Modes: Can occur with any mode
      // Media Error Log: Transaction Type (TT): -ANY TRANSACTION TYPE
      // Media Error Log: Error Type (ET): 0x05: User % Remaining Alarm OR 0x06: SMART Health Status Change
      // WHEA Logging: None generated
      //
      if (MediaLogPtr->ErrorType == FNV_ET_USER_PERCENT_REM_ALARM_TRIP ||
          MediaLogPtr->ErrorType == FNV_ET_SMART_HEALTH_STATUS_CHANGED) {
        FnvAcpiAddLeafNotification (Socket, Ch, Dimm);
        continue;
      }

      //
      // If the TT and ET is not enabled in the Host alerts table, move on to next
      //
      if (!IsAlertEnabled(mImcRas->HostAlerts[MediaLogPtr->TransactionType], MediaLogPtr->ErrorType)) {
        RAS_DEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "Alert for transaction type %xh with error type %xh is not enabled. Skip report\n",
                    MediaLogPtr->TransactionType, MediaLogPtr->ErrorType));
        continue;
      }

      //
      // 1)Poison Creation -ACPI Notification 0x81 to ACPI0012 Root Device
      // Valid Modes: -PMEM Regions
      // AppDirect Mode only-Must have Valid DPA
      // Media Error Log: Transaction Type (TT): -NOT ARS
      // Media Error Log: Error Type (ET): 0x00: UNC, 0x03: DataPathError
      // WHEA Logging: None generated
      //
      if ((MediaLogPtr->TransactionType != FNV_TT_ADDRESS_RANGE_SCRUB) && ((MediaLogPtr->ErrorType == FNV_ET_UNCORRECTABLE) || (MediaLogPtr->ErrorType == FNV_ET_DATA_PATH_ERROR))
          && (MediaLogPtr->ErrorFlags & VALID_DPA)) {
        if ((MediaLogPtr->ErrorType == FNV_ET_UNCORRECTABLE) && (MediaLogPtr->TransactionType == FNV_TT_PM_READ || MediaLogPtr->TransactionType == FNV_TT_PM_WRITE)) {
          FnvAcpiAddToErrorList (Socket, Ch, Dimm, MediaLogPtr->Dpa, MediaLogPtr->Range);
        } else {
          FnvAcpiAddRootNotification (Socket, Ch, Dimm, MediaLogPtr->Dpa, MediaLogPtr->Range);
        }
      }

      //
      // 2)Poison Creation -ACPI Log - Patrol Scrubber
      //Valid Modes: -PMEM Regions - AppDirect Mode
      //             -FM Regions - Memory Mode
      //AppDirect Mode only-Must have Valid DPA
      //Media Error Log: Transaction Type (TT): -NOT ARS, PATROL SCRUBBER
      //Media Error Log: Error Type (ET): 0x00: UNC, 0x03: DataPathError
      //WHEA Logging: ErrorSeverity: Correctable, MemoryErrorType: ScrubUncorrectedError
      //
      if ((MediaLogPtr->TransactionType == FNV_TT_PATROL_SCRUB)
          && ((MediaLogPtr->ErrorType == FNV_ET_UNCORRECTABLE) || (MediaLogPtr->ErrorType == FNV_ET_DATA_PATH_ERROR))
          && (MediaLogPtr->ErrorFlags & VALID_DPA)) {
          MemErrorType = EFI_PLATFORM_MEMORY_ERROR_SCRUB_UNCORRECTED;
          MemErrorSev = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE;
          RAS_DEBUG ((LEVEL_REG, "%a: MemErrorSev:%d; MemErrorType:%d\n", __FUNCTION__, MemErrorSev, MemErrorType));
      }

      //
      // 3)Poison Creation -ACPI Log - Demand IO
      //Valid Modes: -PMEM Regions - AppDirect Mode
      //             -FM Regions - Memory Mode
      //AppDirect Mode only-Must have Valid DPA
      //Media Error Log: Transaction Type (TT): -NOT ARS, -2LM READ/WRITE -PM READ/WRITE
      //Media Error Log: Error Type (ET): 0x00: UNC, 0x03: DataPathError
      //WHEA Logging: -ErrorSeverity: Correctable, -MemoryErrorType: MultiBitECC
      //
      if ((((MediaLogPtr->TransactionType == FNV_TT_2LM_READ) || (MediaLogPtr->TransactionType == FNV_TT_2LM_WRITE))
             || ((MediaLogPtr->TransactionType == FNV_TT_PM_READ) || (MediaLogPtr->TransactionType == FNV_TT_PM_WRITE)))
          && ((MediaLogPtr->ErrorType == FNV_ET_UNCORRECTABLE) || (MediaLogPtr->ErrorType == FNV_ET_DATA_PATH_ERROR))
          && (MediaLogPtr->ErrorFlags & VALID_DPA)) {
          MemErrorType = EFI_PLATFORM_MEMORY_ERROR_MLTIBIT_ECC;
          MemErrorSev = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE;
          RAS_DEBUG ((LEVEL_REG, "%a: MemErrorSev:%d; MemErrorType:%d\n", __FUNCTION__, MemErrorSev, MemErrorType));
      }

      //
      // 4) Poison Creation -ACPI Log - Unknown
      //Valid Modes: -PMEM Regions - AppDirect Mode
      //             -FM Regions - Memory Mode
      //AppDirect Mode only-Must have Valid DPA
      //Media Error Log: Transaction Type (TT): -NOT ARS, -AIT READ/WRITE -CSR READ/WRITE WEAR LEVEL MOVE
      //Media Error Log: Error Type (ET): 0x00: UNC, 0x03: DataPathError
      //WHEA Logging: -ErrorSeverity: Correctable, -MemoryErrorType: Unknown
      //
      if ((((MediaLogPtr->TransactionType == FNV_TT_AIT_READ) || (MediaLogPtr->TransactionType == FNV_TT_AIT_WRITE))
             || ((MediaLogPtr->TransactionType == FNV_TT_CSR_READ) || (MediaLogPtr->TransactionType == FNV_TT_CSR_WRITE))
             || (MediaLogPtr->TransactionType == FNV_TT_WEAR_LEVEL_MOVE))
          && ((MediaLogPtr->ErrorType == FNV_ET_UNCORRECTABLE) || (MediaLogPtr->ErrorType == FNV_ET_DATA_PATH_ERROR))
          && (MediaLogPtr->ErrorFlags & VALID_DPA)) {
          MemErrorType = EFI_PLATFORM_MEMORY_ERROR_UNKNOWN;
          MemErrorSev = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE;
          RAS_DEBUG ((LEVEL_REG, "%a: MemErrorSev:%d; MemErrorType:%d\n", __FUNCTION__, MemErrorSev, MemErrorType));
      }

      //
      //5) Other ACPI Log - Unknown
      //     -Valid Modes: -PMEM Regions - AppDirect Mode
      //                   -FM Regions - Memory Mode
      //AppDirect Mode only-Must have Valid DPA
      //Media Error Log: Transaction Type (TT): -ANY TRANSACTION TYPE
      //Media Error Log: Error Type (ET): 0x01: DPA Mismatch, 0x02: AIT Error, 0x04: Locked 0x04: Illegal Access, 0x07: Persistent Write ECC
      //WHEA Logging: -ErrorSeverity: Correctable, -MemoryErrorType: Unknown
      //
      if ( (((MediaLogPtr->ErrorType == FNV_ET_DPA_MISMATCH) || (MediaLogPtr->ErrorType == FNV_ET_AIT_ERROR))
             || ((MediaLogPtr->ErrorType == FNV_ET_LOCKED_ILLEGAL_ACCESS) || (MediaLogPtr->ErrorType == FNV_ET_PERSISTENT_WRITE_ECC)))
          && (MediaLogPtr->ErrorFlags & VALID_DPA)) {
          MemErrorType = EFI_PLATFORM_MEMORY_ERROR_UNKNOWN;
          MemErrorSev = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE;
          RAS_DEBUG ((LEVEL_REG, "%a: MemErrorSev:%d; MemErrorType:%d\n", __FUNCTION__, MemErrorSev, MemErrorType));
      }
      //
      // Do UNC Poison explicitly for Read Transactions
      //
      if ((ImcRas->DcpmmUncPoison) && (MediaLogPtr->ErrorType == FNV_ET_UNCORRECTABLE) &&
          ((MediaLogPtr->TransactionType == FNV_TT_2LM_READ) ||(MediaLogPtr->TransactionType == FNV_TT_PM_READ))) {

        RAS_DEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "MediaLogPtr->ErrorType = type 0x%X\n", MediaLogPtr->ErrorType));
        RAS_DEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "MediaLogPtr->TransactionType = type 0x%X\n", MediaLogPtr->TransactionType));

        FillFnvErrorData (Socket, Ch, Dimm, MediaLogPtr->ErrorType, &(MediaLogPtr->Dpa), FALSE, &MemRecord);

        RAS_DEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "Calling AsmWriteFlushCacheLine on SPA 0x%lx\n", MemRecord.PhysicalAddress));

        MemoryFence ();
        PhysicalAddressPointer = (UINT64*)(UINTN)MemRecord.PhysicalAddress;
        *PhysicalAddressPointer = 0;
        MemoryFence ();
        AsmFlushCacheLine ((VOID*)PhysicalAddressPointer);
      }

      //
      // Only try to report SPA when 1. DPA valid bit is set AND 2. ReportSPA is enabled in setup and 3. DDRT alert is enabled for the transaction across error type
      //
      if ((MediaLogPtr->ErrorFlags & VALID_DPA) && ImcRas->ReportAlertSPA) {
        FillFnvErrorData (Socket, Ch, Dimm, MemErrorType, &(MediaLogPtr->Dpa), FALSE, &MemRecord);
      } else {
        FillFnvErrorData (Socket, Ch, Dimm, MemErrorType, NULL, FALSE, &MemRecord);
      }

      Status = ReportMemErrorLog (MemErrorSev, &MemRecord);
      if (EFI_ERROR (Status)) {
        RAS_DEBUG ((VerboseLevel, RAS_ERROR_STR ("DDRT") "Failed to report the memory error record (%r)\n", Status));
      }
    } // MAX_MEDIA_LOGS

    if (ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrLogDataRsp[LogsIndex].MediaErrLog.ReturnCount !=
        ARRAY_SIZE (ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrLogDataRsp[LogsIndex].MediaErrLog.MediaLogs)) {
      //
      // The only condition we want to continue reading is ReturnCount == MediaLogData array size
      //
      break;
    }
  }

  if (LogsIndex == MAX_MEDIA_PAYLOADS) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, RAS_ERROR_STR ("DDRT") "ProcessMediaLog: S%d.C%d.D%d P%d: ", Socket, Ch, Dimm, Priority));
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "media log loop reached the limit.\n"));
  }
}

VOID
GetErrorLogFromSocket (
  IN OUT VOID    *Context
  )
{
  IMC_RAS_STRUCTURE  *ImcRas;
  MP_CPU_INFO        CpuInfo;
  UINTN              ProcNumber;
  EFI_STATUS         Status;
  ERR_LOG_DATA       ErrLog;
  UINT64             TotalTimeInNanoSeconds = 0;
  UINT64             StartTime;
  UINT64             ActualTime;
  UINT64             CounterStart = 0;
  UINT64             CounterEnd = 0;
  INT64              WaitTime;
  INT64              CounterCycle;
  UINT16             *SequenceNumber;
  UINT8              Priority = 0;
  UINT8              Socket = 0;
  UINT8              Ch = 0;
  UINT8              Dimm = 0;
  UINT8              Index = 0;
  BOOLEAN            ReadMediaLog = FALSE;
  BOOLEAN            ReadThermalLog = FALSE;
  BOOLEAN            CheckLongOpState = FALSE;

  ImcRas = GetImcRasData ();

  StartTime = GetPerformanceCounter ();

  ProcNumber = GetProcessorInfo(&CpuInfo);
  Socket = (UINT8)CpuInfo.Package;
  ImcRas->ApState[Socket] = AP_IN_PROGRESS;

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (!IsDcpmm(Socket, Ch, Dimm)) {
                continue;
      }
      for (Priority = 0; Priority <= HIGH_LEVEL_LOG; Priority++) {

        ActualTime = GetPerformanceCounter ();

        GetPerformanceCounterProperties (&CounterStart, &CounterEnd);
        CounterCycle = CounterEnd - CounterStart;
        if (CounterCycle < 0) {
          CounterCycle = -CounterCycle;
        }
        CounterCycle++;

        WaitTime = ActualTime - StartTime;
        if (CounterStart > CounterEnd) {
          WaitTime = -WaitTime;
        }
        if (WaitTime < 0) {
          WaitTime += CounterCycle;
        }

        TotalTimeInNanoSeconds = GetTimeInNanoSecond (WaitTime);
        if (TotalTimeInNanoSeconds > ImcRas->GetErrorLogTimeoutCounter) {
          ImcRas->ApState[Socket] = AP_ERROR;
          return;
        }

        //
        // Retrieve the error log data
        //
        Status = McDdrtErrorLogDetectError(Socket, Ch, Dimm, Priority, CLEAR_ON_READ, &ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].DetectErrorData);
        MemoryFence();
        if  (!EFI_ERROR(Status) && ImcRas->WaStatusS2207801634 == TRUE) {
          //
          // Always set these alert bits to handle them via SMI, as a WA(for ICX A0 only)
          //
          ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].DetectErrorData.Bits.Log |= (BIT2 | BIT1 | BIT0); //(FNVERR_LONGOP_COMPLETE=BIT2 | FNVERR_THERMAL_ERROR_LOG=BIT1 | FNVERR_MEDIA_ERROR_LOG=BIT0);
          ImcRas->DdrtPreviousErrorId[Socket][Ch][Dimm][Priority] = (UINT8) ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].DetectErrorData.Bits.ErrorId;
        }

        ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].DetectErrorStatus = Status;

        if (EFI_ERROR (Status)) {
          continue;
        }

        //
        // Check log if there is anything to do
        //
        if (ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].DetectErrorData.Bits.Log == 0) {
          continue;
        }

        //
        // Retrive the NVDIMM info data
        //
        if (EFI_ERROR (ImcRas->DimmInfoStatus[Socket][Ch][Dimm])) {
          ImcRas->DimmInfoStatus[Socket][Ch][Dimm] = GetIdentifyDimm(Socket, Ch, Dimm, &ImcRas->DimmInfo[Socket][Ch][Dimm]);
          if (EFI_ERROR (ImcRas->DimmInfoStatus[Socket][Ch][Dimm])) {
            continue;
          }
        }

        //
        // Firmware may set multiple bits in DDRT interrupt packet
        // so ensure we check all bits we handle in every iteration
        //
        if (!ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].DetectErrorData.Bits.Overflow) {
          if (Priority == LOW_LEVEL_LOG) {
            if (ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].DetectErrorData.Bits.Log & FNVERR_LONGOP_COMPLETE) {
              CheckLongOpState = TRUE;
            }
          }
          if (ImcRas->DimmInfo[Socket][Ch][Dimm].FisVersion >= FIS_1_13) {
            if (ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].DetectErrorData.Bits.Log & FNVERR_MEDIA_ERROR_LOG) {
              ReadMediaLog = TRUE;
            }
            if (Priority == LOW_LEVEL_LOG) {
              if (ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].DetectErrorData.Bits.Log & FNVERR_THERMAL_ERROR_LOG) {
                ReadThermalLog = TRUE;
              }
            }
          } else {
            if (ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].DetectErrorData.Bits.Log & FNVERR_NGNVM_ERROR_LOG) {
              ReadMediaLog = TRUE;
              if (Priority == LOW_LEVEL_LOG) {
                ReadThermalLog = TRUE;
              }
            }
          }
        } else {
          ReadMediaLog = TRUE;
          if (Priority == LOW_LEVEL_LOG) {
            CheckLongOpState = TRUE;
            ReadThermalLog = TRUE;
          }
        }

        if (Priority == HIGH_LEVEL_LOG) {
          //
          // FIS 1.6 and below the bit (FNVERR_FATAL + FNVERR_FW_HANG) are valid
          //
          if (ImcRas->DimmInfo[Socket][Ch][Dimm].FisVersion <= FIS_1_06) {
            if ( ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].DetectErrorData.Bits.Log & FNVERR_FW_HANG ) {
              ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ProcessFwHang = TRUE;
            }

            if (ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].DetectErrorData.Bits.Log & FNVERR_FATAL) {
              ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ProcessFwFatalErr = TRUE;
            }
          }
        }

        if (CheckLongOpState) {
          Status = FnvLongOpCompleteGet (Socket, Ch, Dimm, &ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].LongOpRsp[0],
                                                           &ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].LongOpSeqNum);
          ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].LongOpStatus = Status;
          ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ProcessLongOp = TRUE;
        }

        if (ReadMediaLog) {
          ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ProcessMediaErrLogData = TRUE;
          SequenceNumber = &ImcRas->FnvSequenceNumberMatrix[Socket][Ch][Dimm][MEDIA_LOG_REQ][Priority];
          ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrSeqFirst = *SequenceNumber;
          for (Index = 0; Index < MAX_MEDIA_PAYLOADS; Index++) {
            Status = NvmCtlrGetErrorLog (Socket, Ch, Dimm, MEDIA_LOG_REQ, Priority, TRUE, SequenceNumber, &ErrLog);

            ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrLogDataRsp[Index].ErrorMediaLogStatus = Status;
            ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrLogDataRsp[Index].MediaErrLog = ErrLog.MediaLogData;
            ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrLogDataRsp[Index].NewSeqNumber = *SequenceNumber;

            if (EFI_ERROR (Status) || ErrLog.MediaLogData.ReturnCount == 0 ||
                ErrLog.MediaLogData.ReturnCount != ARRAY_SIZE (ErrLog.MediaLogData.MediaLogs)) {
              break;
            }
          }
        }

        if (ReadThermalLog) {
          ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ProcessThermalErrLogData = TRUE;
          SequenceNumber = &ImcRas->FnvSequenceNumberMatrix[Socket][Ch][Dimm][THERMAL_LOG_REQ][Priority];
          ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ThermalErrSeqFirst = *SequenceNumber;

          for (Index = 0; Index < MAX_THERMAL_PAYLOADS; Index++) {

            Status = NvmCtlrGetErrorLog (Socket, Ch, Dimm, THERMAL_LOG_REQ, Priority, TRUE, SequenceNumber, &ErrLog);

            ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ThermalErrLogDataRsp[Index].ErrorThermalLogStatus = Status;
            ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ThermalErrLogDataRsp[Index].ThermalErrLog = ErrLog.ThermalLogData;
            ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ThermalErrLogDataRsp[Index].NewSeqNumber = *SequenceNumber;

            if (EFI_ERROR (Status) || ErrLog.ThermalLogData.ReturnCount == 0 ||
              ErrLog.ThermalLogData.ReturnCount != ARRAY_SIZE (ErrLog.ThermalLogData.ThermalLogs)) {
              break;
            }
          }
        }
      }
    }
  }
  ImcRas->ApState[Socket] = AP_FINISHED;
  MemoryFence();
}

/**
  Retrieve Event Data From All Dimms

  This function checking if on specific socket occurs any DDRT alters

  @retval None
**/
EFI_STATUS
RetrieveEventDataFromAllDimms (
  IN UINT8  *SocketPtr
  )
{
  IMC_RAS_STRUCTURE         *ImcRas;
  UINT8                     Socket;

  ImcRas = GetImcRasData ();

  MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, FALSE, GetErrorLogFromSocket, NULL);

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (ImcRas->ApState[Socket] != AP_FINISHED && ImcRas->ApState[Socket] != AP_STOP) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, RAS_ERROR_STR ("DDRT") "RetrieveEventDataFromAllDimms: Invalid ImcRas->ApState %d on S:%d\n", ImcRas->ApState[Socket], Socket));
      *SocketPtr = Socket;
      return EFI_ABORTED;
    }
  }

  return EFI_SUCCESS;
}

/*
  Handles DDRT Priority errors

  @param[in]  Socket    Socket index
  @param[in]  Ch        Channel index (at socket level)
  @param[in]  Dimm      Dimm index (within the channel)
  @param[in]  Priority  Priority of the error

*/
VOID
HandleNvdimmPriorityError (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN UINT8 Priority
  )
{

  EFI_PLATFORM_MEMORY_ERROR_DATA  MemRecord;
  IMC_RAS_STRUCTURE               *ImcRas;
  EFI_STATUS                      Status;
  BOOLEAN                         FwHang = FALSE;
  BOOLEAN                         FwFatalErr = FALSE;

  ImcRas = GetImcRasData ();

  //
  // Retrieve the error log data
  //
  if (Priority != HIGH_LEVEL_LOG && Priority != LOW_LEVEL_LOG) {
    RAS_DEBUG ((LEVEL_REG, RAS_ERROR_STR ("DDRT") "HandleNvdimmPriorityError: S%d.C%d.D%d P%d: invalid priority level\n", Socket, Ch, Dimm, Priority));
    return;
  }

  Status = ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].DetectErrorStatus;

  if (EFI_ERROR (Status)) {
    if (Status != EFI_NOT_FOUND) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, RAS_ERROR_STR ("DDRT") "HandleNvdimmPriorityError: S%d.C%d.D%d P%d: failed to detect errors: %r\n", Socket, Ch, Dimm, Priority, Status));
    } else {
      RAS_DEBUG ((LEVEL_REG, RAS_INFO_STR ("DDRT") "HandleNvdimmPriorityError: S%d.C%d.D%d P%d: no errors\n", Socket, Ch, Dimm, Priority));
    }
    return;
  }

  RAS_DEBUG ((LEVEL_REG, RAS_INFO_STR ("DDRT") "HandleNvdimmPriorityError: S%d.C%d.D%d P%d, DDRT Event Reg: 0x%08x\n", Socket, Ch, Dimm, Priority,
              ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].DetectErrorData));

  if (ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ProcessLongOp) {
    FnvLongOpCompleteProcess (Socket, Ch, Dimm, &ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].LongOpRsp[0],
                                                &ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].LongOpSeqNum);

    Status = ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].LongOpStatus;
    if (EFI_ERROR (Status)) {
      RAS_DEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "Failed to handle long operation completion (%r)\n", Status));
      FillFnvErrorData (Socket, Ch, Dimm, EFI_PLATFORM_MEMORY_ERROR_UNKNOWN, NULL, FALSE, &MemRecord);
      Status = ReportMemErrorLog (DdrtNvdimmFisErrorIsFatal (Status)? EFI_ACPI_6_2_ERROR_SEVERITY_FATAL : EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED, &MemRecord);
      if (EFI_ERROR (Status)) {
        RAS_DEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "Failed to report the memory error record (%r)\n", Status));
      }
    }
  }

  if (Priority == HIGH_LEVEL_LOG) {
    FwHang = ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ProcessFwHang;
    FwFatalErr = ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ProcessFwFatalErr;

    if (FwHang || FwFatalErr) {
      FillFnvErrorData (Socket, Ch, Dimm, EFI_PLATFORM_MEMORY_ERROR_UNKNOWN, NULL, FALSE, &MemRecord);

      Status = ReportMemErrorLog (EFI_ACPI_6_2_ERROR_SEVERITY_FATAL, &MemRecord);
      if (EFI_ERROR (Status)) {
        RAS_DEBUG ((LEVEL_BASIC_FLOW, RAS_ERROR_STR ("DDRT") "Failed to report the memory error record (%r)\n", Status));
      }

      if (FwFatalErr) {
        FnvAcpiAddLeafNotification (Socket, Ch, Dimm);
        ReportDdrtAlertErrorLog (Socket, Ch, Dimm, DdrtAlertErrotTypeFatal, NULL);
      } else {
        ReportDdrtAlertErrorLog (Socket, Ch, Dimm, DdrtAlertErrotTypeFwHang, NULL);
      }
    }
  }

  if (ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ProcessMediaErrLogData) {
    ProcessMediaLog (Socket, Ch, Dimm, Priority);
  }

  if (ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority].ProcessThermalErrLogData) {
    ProcessThermalLog (Socket, Ch, Dimm, Priority);
  }
  ZeroMem(&ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority], sizeof(ImcRas->DimmErrorLogData[Socket][Ch][Dimm][Priority]));
}

/**
  DDRT Check Status - this function checks DDRT errors status

  @retval TRUE      - detected errors
  @retval FALSE     - not detected errors

**/
BOOLEAN
EFIAPI
DdrtCheckStatus (
  )
{
  EFI_STATUS             Status;
  DDRT_EVENT_DATA        Data;
  UINT8                  Socket;
  UINT8                  Ch;
  UINT8                  Dimm;
  UINT8                  Priority;
  IMC_RAS_STRUCTURE      *ImcRas;
  MEM_TOPOLOGY           *MemTopology;


  MemTopology = GetMemTopology ();
  if (MemTopology == NULL) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetMemTopology failed \n"));
    return FALSE;
  }

  ImcRas = GetImcRasData ();
  if (ImcRas == NULL) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetImcRasData failed \n"));
    return FALSE;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
  // APTIOV_SERVER_OVERRIDE_RC_START : Check Socket Channel Dimm presence before checking status
    if (!IsSocketPresent(Socket)) {
      continue;
    }
  // APTIOV_SERVER_OVERRIDE_RC_END : Check Socket Channel Dimm presence before checking status
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      // APTIOV_SERVER_OVERRIDE_RC_START : Check Socket Channel Dimm presence before checking status
      if (IsChannelPresentPerSocket (Socket, Ch) == FALSE) {
        continue;
      }
      // APTIOV_SERVER_OVERRIDE_RC_END : Check Socket Channel Dimm presence before checking status
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
	   if (MemTopology->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].AepDimmPresent) {
          for ( Priority = LOW_PRIORITY_EVENT; Priority <= HIGH_PRIORITY_EVENT; Priority++ ) {
            Status = McDdrtErrorLogDetectError (Socket, Ch, Dimm, Priority, DO_NOT_CLEAR_ON_READ, &Data);
            if ((!EFI_ERROR (Status)) && (ImcRas->WaStatusS2207801634 == TRUE)) {
              //
              // Always set these alert bits to handle them via SMI, as a WA(for ICX R0 only)
              //
              Data.Bits.Log |= (BIT2 | BIT1 | BIT0); //(FNVERR_LONGOP_COMPLETE=BIT2 | FNVERR_THERMAL_ERROR_LOG=BIT1 | FNVERR_MEDIA_ERROR_LOG=BIT0);
              if (ImcRas->DdrtPreviousErrorId[Socket][Ch][Dimm][Priority] == Data.Bits.ErrorId) {
                continue;
              }
            }
            if (!EFI_ERROR(Status) && (Data.Bits.Log != 0)) {
              return TRUE;
            }
          }
        } // Pmem/DCPMM present
      }
    }
  }

  return FALSE;
}

/**
  DDRT Error Handler

  @param  Socket  -  Socket index

  @retval VOID

**/
VOID
EFIAPI
DdrtErrorHandler (
  )
{
  UINT8                           Socket = 0;
  UINT8                           Ch;         // at socket level
  UINT8                           Dimm;       // with in channel
  UINT8                           Skt = 0;
  EFI_STATUS                      Status;
  EFI_PLATFORM_MEMORY_ERROR_DATA  MemRecord;
  IMC_RAS_STRUCTURE               *ImcRas;

  ImcRas = GetImcRasData ();

  if (DdrtCheckStatus () == FALSE) {
    return;
  }

  FnvAcpiClearLeafNotifications ();
  FnvAcpiClearRootNotification ();

  Status = RetrieveEventDataFromAllDimms(&Skt);
  if (EFI_ERROR (Status)) {
    FillFnvErrorData (Skt, 0, 0, EFI_PLATFORM_MEMORY_ERROR_UNKNOWN, NULL, TRUE, &MemRecord);
    ReportMemErrorLog (EFI_ACPI_6_2_ERROR_SEVERITY_FATAL, &MemRecord);
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      for (Dimm = 0 ; Dimm < MAX_DIMM; Dimm++) {
        if (IsDcpmm (Socket, Ch, Dimm)) {
          HandleNvdimmPriorityError (Socket, Ch, Dimm, HIGH_LEVEL_LOG);
          HandleNvdimmPriorityError (Socket, Ch, Dimm, LOW_LEVEL_LOG);
        }
      }
    }
  }

  FnvAcpiLeafNotify ();
  FnvAcpiRootNotify ();

  ZeroMem(&ImcRas->ApState, sizeof(ImcRas->ApState));
  ZeroMem(&ImcRas->DimmErrorLogData, sizeof(ImcRas->DimmErrorLogData));
}

/**
  This function will set up threshold values for number of retries for DDRT ECC Functionality

  @param[in]    Socket

  @retval    None

**/
VOID
EFIAPI
DcpmmEccModeInit (
  UINT8 Socket
  )
{
  UINT8    ChOnSkt;
  BOOLEAN  DcpmmOnCh;
  UINT8    ErrThrshld;
  UINT8    Dimm;

  MEM_TOPOLOGY        *MemTopology;

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "PMemEccModeInit\n"));

  ErrThrshld = 2; // Setup to generate SMI After two corrected errors on DCPMM rank

  MemTopology = GetMemTopology ();
  if (MemTopology == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, RAS_INFO_STR ("DDRT") "MemTopology == NULL\n"));
    ASSERT (FALSE);
    return;
  }

  for (ChOnSkt = 0; ChOnSkt < MAX_CH; ChOnSkt ++) {
    DcpmmOnCh = FALSE;
    for (Dimm = 0 ; Dimm < MAX_DIMM; Dimm++) {
      if (MemTopology->Socket[Socket].ChannelInfo[ChOnSkt].DimmInfo[Dimm].AepDimmPresent) {
        DcpmmOnCh = TRUE;
      }
    }
    if (DcpmmOnCh) { //Identified DCPMM dimm on channel
      //
      //Setup threshold value for number of retry attempts done by FMC for UC error
      //
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "PMemEccModeInit: PMem set Retry Uc Error Counter Thr\n"));
      SetDdrtRetryUcErrorCounterThreshold (Socket, ChOnSkt, ErrThrshld);
    }
  }
  return;
}

/**
  Initialize failed nibble data.

  If this library does not already have the pointer, get it via the
  RasGlobalDataLib. If that library indicates it is a new allocation,
  initialize the data.
**/
VOID
InitializeFailedNibbleData (
  VOID
  )
{
  BOOLEAN               NewAllocation;
  UINT8                 Socket;
  UINT8                 Channel;
  UINT8                 Rank;
  FAILED_NIBBLE_TRACKER *Tracker;

  //
  // Sanity-check the build constants.
  //
  RAS_ASSERT (FAILED_NIBBLE_HISTORY_COUNT >= FAILED_NIBBLE_HISTORY_THRESHOLD);

  //
  // If we already have the pointer, return.
  //
  if (mFailedNibbleData != NULL) {
    return;
  }

  //
  // Allocate if not already allocated, and get the pointer.
  //
  mFailedNibbleData = (FAILED_NIBBLE_DATA *) GetRasGlobalData (
    L"WA S1909300951 Failed Nibble Data",
    sizeof (FAILED_NIBBLE_DATA),
    &NewAllocation
    );

  //
  // If someone else allocated (and initialized) this, return.
  //
  if (!NewAllocation) {
    return;
  }

  //
  // Set default values.
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Channel = 0; Channel < MAX_CH; Channel++) {
      for (Rank = 0; Rank < (MAX_RANK_CH * SUB_CH); Rank++) {
        Tracker = &mFailedNibbleData->Tracker[Socket][Channel][Rank];
        Tracker->Index = 0;
        SetMem (
          Tracker->History,
          sizeof (UINT8) * FAILED_NIBBLE_HISTORY_COUNT,
          INVALID_FAILED_NIBBLE
          );
      }
    }
  }
}

/**
  Check if a failed nibble is at the threshold.

  Update tracking data for a socket-channel-rank's latest failed nibbles.
  If after the update, if the number of matching failed nibbles in the history
  is at least equal to the threshold, return TRUE. Otherwise, return FALSE.

  @param[in]  Socket          CPU socket.
  @param[in]  Channel         Channel on socket.
  @param[in]  Rank            Rank on channel.
  @param[in]  FailedNibble    Failed nibble (device).

  @retval TRUE      This failed nibble is at the threshold.
  @retval FALSE     This failed nibble is not at the threshold.
**/
BOOLEAN
IsFailedNibbleAtThreshold (
  IN  UINT8   Socket,
  IN  UINT8   Channel,
  IN  UINT8   Rank,
  IN  UINT8   FailedNibble
  )
{
  FAILED_NIBBLE_TRACKER *Tracker;
  UINT8                 Index;
  UINT8                 Count;

  RAS_ASSERT (mFailedNibbleData != NULL);
  if (mFailedNibbleData == NULL) {
    return FALSE;
  }

  //
  // Insert at the current index.
  //
  Tracker = &mFailedNibbleData->Tracker[Socket][Channel][Rank];
  RAS_ASSERT (Tracker->Index < FAILED_NIBBLE_HISTORY_COUNT);
  Tracker->History[Tracker->Index] = FailedNibble;

  //
  // Increment and wrap the index.
  //
  Tracker->Index++;
  Tracker->Index %= FAILED_NIBBLE_HISTORY_COUNT;

  //
  // Count the number of matches and return whether the threshold is met.
  //
  Count = 0;
  for (Index = 0; Index < FAILED_NIBBLE_HISTORY_COUNT; Index++) {
    if (Tracker->History[Index] == FailedNibble) {
      Count++;
      if (Count >= FAILED_NIBBLE_HISTORY_THRESHOLD) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

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
  )
{
  UINT8    ChOnSkt;
  UINT8    DcpmmRank;
  UINT8    ErrThrshld;
  UINT8    RetryThrshld;
  UINT8    Dimm;
  IMC_RAS_STRUCTURE         *ImcRas;
  MEM_TOPOLOGY              *MemTopology;

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "PMemEccModeSwitchSetup\n"));

  RetryThrshld = 2; // Number of retry attempts done by FMC before ECC correction
  ErrThrshld = 2; // Setup to generate SMI After two corrected errors on DDRT rank
  if (IsSiliconWorkaroundEnabled ("S1909300951")) {
    ErrThrshld = 1; // override to 1 for the WA
    InitializeFailedNibbleData ();
  }

  MemTopology = GetMemTopology ();
  if (MemTopology == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, RAS_INFO_STR ("DDRT") "MemTopology == NULL\n"));
    ASSERT(FALSE);
    return;
  }

  for (ChOnSkt = 0; ChOnSkt < MAX_CH; ChOnSkt ++) {
    if (!MemTopology->Socket[Socket].ChannelInfo[ChOnSkt].Enabled) {
      continue;
    }
    DcpmmRank = 0xFF;
    for (Dimm = 0 ; Dimm < MAX_DIMM; Dimm++) {
      if (MemTopology->Socket[Socket].ChannelInfo[ChOnSkt].DimmInfo[Dimm].AepDimmPresent) {
        RAS_DEBUG ((LEVEL_BASIC_FLOW, "PMemEccModeSwitchSetup: PMem Dimm :%d \n",Dimm));
        DcpmmRank = (Dimm * (MAX_RANK_DIMM * SUB_CH)); //DCPMM Dimm is single Ranked. Rank 0 - 3 = Dimm 0 and Rank 4 - 7 = Dimm 1
        break;
      }
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "PMemEccModeSwitchSetup: NO PMem Dimm on Socket: %d Ch:%d Dimm:%d \n",Socket,ChOnSkt,Dimm));
    }

    if (DcpmmRank != 0xFF) { //Identified PMem dimm on channel
      //
      //Setup threshold value for number of retry attempts done by FMC before ECC correction
      //
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "PMemEccModeInit: PMem set Ecc Mode A Threshold\n"));
      SetDdrtEccModeAThreshold (Socket, ChOnSkt, RetryThrshld);

      //
      //Setup Rank error threshold for SMI signalling through UBOX when rank error counter exceeds threshold
      //
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "PMemEccModeSwitchSetup: Set Error threshold per rank\n"));
      SetErrorThresholdPerRank (Socket, ChOnSkt, DcpmmRank, ErrThrshld);
    }
  }
  ImcRas = GetImcRasData ();
  if (ImcRas != NULL) {
    ImcRas->RasEventHndlrTable[ECC_FLOW_SWITCH] = DcpmmEccModeSwitchAction;
  }
  return;
}


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
  )
{
  UINT8       Socket;
  UINT8       ChOnSkt;
  UINT8       ChOnNode;
  UINT8       Rank;
  UINT8       RetryRank;
  UINT8       ModeInfo;
  UINT8       FailedNibble;
  BOOLEAN     Valid;
  BOOLEAN     DcpmmRankFound;
  UINT8       Dev;
  UINT32      EccMode;
  MEM_TOPOLOGY        *MemTopology;

  Socket = NODE_TO_SKT (Node);

  MemTopology = GetMemTopology ();
  if (MemTopology == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, RAS_INFO_STR ("DDRT") "MemTopology == NULL\n"));
    ASSERT(FALSE);
    return FALSE;
  }

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "PMemEccModeSwitchAction - Checking for Rank errors \n"));
  if (EventType == NEW_EVENTS) {
    for (ChOnNode = 0; ChOnNode < MAX_MC_CH; ChOnNode++) {
      ChOnSkt = NODECHA_TO_SKTCHA (Node, ChOnNode);
      if (!IsChannelPresentPerSocket (Socket, ChOnSkt)) {
        continue;
      }

      if (!((GetDdrtEccMode (Socket, ChOnSkt) & DDRT_ECC_MODE_A) == DDRT_ECC_MODE_A)) { //Channel already switched to Mode B
        continue;
      }

      DcpmmRankFound = FALSE;
      for (Rank = 0; Rank < (MAX_RANK_CH * SUB_CH); Rank++) {
        if (!IsRankEnabled (Node, ChOnNode, Rank)) {
          continue;
        }
        if (IsDcpmmRank (Socket, ChOnSkt, Rank)) {
          DcpmmRankFound = TRUE;
          break;
        }
      }

      if (!DcpmmRankFound) {
        continue;
      }

      Valid = GetRetryReadInfo (
              Socket,
              ChOnSkt,
              (UINT64)-1,
              NULL,
              &EccMode,
              &RetryRank,
              NULL,
              NULL,
              &Dev,
              NULL,
              NULL,
              NULL,
              NULL,
              NULL
              );

      if (!Valid) { //No failed device on current channel
        continue;
      }

      RAS_DEBUG ((LEVEL_BASIC_FLOW, "PMemEccModeSwitchAction - About to verify Rank \n"));
      if ((EccMode == RETRY_ECC_MODE_DDRT) && (RetryRank == Rank)){

        ChOnSkt = NODECHA_TO_SKTCHA (Node, ChOnNode);

        if (IsSiliconWorkaroundEnabled ("S1909300951")) {
          if (!IsFailedNibbleAtThreshold (Socket, ChOnSkt, Rank, Dev)) {
            RAS_DEBUG ((LEVEL_BASIC_FLOW, "PMemEccModeSwitchAction - Skip mode switch for failed nibble WA\n"));
            return FALSE;
          }
        }

        RAS_DEBUG ((LEVEL_BASIC_FLOW, "PMemEccModeSwitchAction - Switch DDRT Modes \n"));
        RAS_DEBUG ((LEVEL_BASIC_FLOW, "PMemEccModeSwitchAction - Disable rank correctable error logging/signaling  \n"));
        DisableRankError (Node, ChOnNode, Rank);

        FailedNibble = Dev;
        ModeInfo     = Dev;
        if (MemTopology->BwvPresent) {
          if (IsSiliconWorkaroundEnabled ("S2206866694")) {
            SetDdrtCnsTrnt (Socket, ChOnSkt, WRITE_WRITE_DCLK_WA);
          }
        }
        SetDdrtEccMode (Socket, ChOnSkt, ModeInfo);
        SetDdrtDataPathDelay (Socket, ChOnSkt, FailedNibble, 1); //Socket, ChOnSkt, FailedNibble, ForceCorrection
        RAS_DEBUG ((LEVEL_BASIC_FLOW, "PMemEccModeSwitchAction - Successful on Ch:%d, Rank:%d \n", ChOnNode, Rank));

        ReportDdrtEccModeSwitchErrorLog (Socket, ChOnSkt, Rank, FailedNibble);
        RecordPositionToClearRetryRegister(Socket, ChOnSkt);
        for (ChOnNode = 0; ChOnNode < MAX_MC_CH; ChOnNode++) {
          ChOnSkt = NODECHA_TO_SKTCHA (Node, ChOnNode);
          for (Rank = 0; Rank < (MAX_RANK_CH * SUB_CH); Rank++) {
            ClearRankErrors (Socket, ChOnSkt, Rank);
          }
        }
        return TRUE; // Ecc mode switch was done
      }
    } // Ch loop
  } // new events
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "PMemEccModeSwitchAction - No Rank error found for this action \n"));
  return FALSE;
}
