/** @file
  FalconValley Access mechanisms and mailbox commands.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include <Memory/CpgcDefinitions.h>
#include <Library/MemFmcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include "Include/MemFmcRegs.h"
#include "Include/MemFmcAccess.h"
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/CteNetLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/SimulationServicesLib.h>
#include <Chip/Include/FnvAccessCommon.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemRcLib.h>
#include "Include/MemFmcIpLibInternal.h"

#define BSR_STRING_LENGTH 32 // Avoid magic numbers in MRC string arrays

/**

  Returns a pointer to the DIMM_PARTITION_DATA structure within an FNVCIS buffer

  @param FnvBuffer  Pointer to FNVCIS structure

  @retval Address of the DIMM partition structure array in FnvBuffer

**/
DIMM_PARTITION_DATA (*GetFnvCisBufferDimmPartition (
  FNVCISBUFFER FnvBuffer
  ))[MAX_SOCKET][MAX_CH][MAX_DIMM]
{
  return (&FnvBuffer->dimmPart);
}

//
// Local Prototypes
//

/**

Routine Description: Determines if we need to continue polling

  @param bitmask     - Bit mask that identifies the Bit field in the Boot Status Register
  @retval status - 0 Stop Polling
  @retval status - 1 Continue Polling

**/
VOID
EmulationPollOnBootStatusRegister (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT32   Bitmask
);

/**
Routine Description:
  Retrieve default (predefined) system time (RTC failure)

  @param[out] TimeInSecond - pointer to return calculated time

  @retval 0 SUCCESS
  @retval 1 FAILURE
**/
UINT32
GetTimestampForRtcFailure (
  OUT UINT32 *TimeInSecond
  )
{
  EFI_TIME Time;

  if (NULL == TimeInSecond) {
    return FAILURE;
  }

  //
  // initialize timestamp
  //
  *TimeInSecond = 0;

  //
  // Initialize time data with default date: Monday, January 1, 1990 12:00:00 AM
  //
  Time.Year = SET_SYS_TIME_DEFAULT_YEAR;
  Time.Month = SET_SYS_TIME_DEFAULT_MONTH;
  Time.Day = SET_SYS_TIME_DEFAULT_DAY;
  Time.Hour = SET_SYS_TIME_DEFAULT_HOUR;
  Time.Minute = SET_SYS_TIME_DEFAULT_MINUTE;
  Time.Second = SET_SYS_TIME_DEFAULT_SECOND;

  return CalculateTimeStamp (Time, TimeInSecond);
}

/**
*Updated 10/15/2014*
*NVMCTLR CIS 0.81*

Routine Description:
  @param Host         - Pointer to the system Host (root) structure
  @param socket       - Socket Number
  @param ch           - DDR Channel ID
  @param dimm         - DIMM Number
  @param mbStatus     - Pointer to 8 bit status code from status register
  @retval 0 SUCCESS
  @retval 1 FAILURE
**/
UINT32
SetFnvSystemTime ( PSYSHOST     Host,
                   UINT8        socket,
                   UINT8        ch,
                   UINT8        dimm,
                   UINT8        *mbStatus
)
{
  UINT32 inputPayload[NUM_PAYLOAD_REG];
  UINT32 outputPayload[NUM_PAYLOAD_REG];
  UINT32 status;
  UINT32 TimeInSecond = 0;

  ZeroMem (inputPayload, NUM_PAYLOAD_REG * sizeof(UINT32));
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "Set System Time Starts\n");

  status = GetSystemTime (&TimeInSecond);
  if (!TimeInSecond) {
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "Getting system time failed - use default time\n");

    status = GetTimestampForRtcFailure (&TimeInSecond);
    if (FAILURE == status) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "Getting default time failed\n");
    }
  }

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "NVMCTL Sending command. Timestamp = %u\n", TimeInSecond);

  inputPayload [0] = TimeInSecond;
  status = FnvMailboxInterface(Host, socket, ch, dimm, inputPayload, outputPayload, SET_SYSTEM_TIME, mbStatus);

  return status;
}// Set System Time

/**
  Display FNV/EKV Media/Thermal Error Log Information & Entries

  @param Host           Pointer to the system host (root) structure
  @param Socket         Socket Number
  @param Ch             DDR Channel ID
  @param Dimm           DIMM Number
  @param LogType        Log type
                        0: Media Log
                        1: Thermal Log
  @param LogInfo        Log info
                        0: Retrieve Log Data
                        1: Retrieve Log Info (SequenceNumber and RequestCount are ignored)
  @param ReturnData     Pointer to the structure to hold the results

**/
VOID
FnvDisplayErrorLog (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Ch,
  UINT8                 Dimm,
  UINT8                 LogType,
  UINT8                 LogInfo,
  ERROR_LOG_RETURN_DATA *ReturnData
  )
{
  MEDIA_LOG_RETURN_DATA   *MediaLogData = &ReturnData->MediaLogData;
  THERMAL_LOG_RETURN_DATA *ThermalLogData = &ReturnData->ThermalLogData;
  LOG_INFO_RETURN_DATA    *LogInfoData;
  MEDIA_LOG_ENTRY *MediaLog;
  THERMAL_LOG_ENTRY *ThermalLog;
  UINT32 i;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PMem DisplayErrorLog Starts \n");
  if (LogInfo == NGN_RETRIEVE_LOG_INFO) {
    //
    // Display Error Log Info
    //
    LogInfoData = &ReturnData->LogInfoData;
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Display Error Log Information \n");
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "MaxLogEntries = 0x%x \n", LogInfoData->MaxLogEntries);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CurrentSequenceNumber = 0x%x \n", LogInfoData->CurrentSequenceNumber);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "OldestSequenceNumber = 0x%x \n", LogInfoData->OldestSequenceNumber);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "OldestEntryTimestamp.lo = 0x%x \n", LogInfoData->OldestEntryTimestamp.lo);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "OldestEntryTimestamp.hi = 0x%x \n", LogInfoData->OldestEntryTimestamp.hi);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "NewestEntryTimestamp.lo = 0x%x \n", LogInfoData->NewestEntryTimestamp.lo);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "NewestEntryTimestamp.hi = 0x%x \n", LogInfoData->NewestEntryTimestamp.hi);
  } else if (LogInfo == NGN_RETRIEVE_LOG_DATA) {
    if (LogType == FNV_MEDIA_LOG_REQ) {
      //
      // Display Media Error Log Entries
      //
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Display Media Error Log Entries \n");
      for (i = 0; i < MediaLogData->ReturnCount; i++) {
        MediaLog = &MediaLogData->MediaLog[i];
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "SysTimeStamp.lo = 0x%x \n", MediaLog->SysTimeStamp.lo);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "SysTimeStamp.hi = 0x%x \n", MediaLog->SysTimeStamp.hi);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Dpa.lo = 0x%x \n", MediaLog->Dpa.lo);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Dpa.hi = 0x%x \n", MediaLog->Dpa.hi);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Pda.lo = 0x%x \n", MediaLog->Pda.lo);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Pda.hi = 0x%x \n", MediaLog->Pda.hi);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Range = 0x%x \n", MediaLog->Range);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "ErrorType = 0x%x \n", MediaLog->ErrorType);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "ErrorFlags = 0x%x \n", MediaLog->ErrorFlags);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "TransactionType = 0x%x \n", MediaLog->TransactionType);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "SequenceNumber = 0x%x \n", MediaLog->SequenceNumber);
      }
    } else if (LogType == FNV_THERMAL_LOG_REQ) {
      //
      // Display Thermal Error Log Entries
      //
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Display Thermal Error Log Entries \n");
      for (i = 0; i < ThermalLogData->ReturnCount; i++) {
        ThermalLog = &ThermalLogData->ThermalLog[i];
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "SysTimestamp.lo = 0x%x \n", ThermalLog->SysTimestamp.lo);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "SysTimestamp.hi = 0x%x \n", ThermalLog->SysTimestamp.hi);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Temperature = 0x%x \n", ThermalLog->Temperature);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Sign = 0x%x \n", ThermalLog->Sign);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Reported = 0x%x \n", ThermalLog->Reported);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "TempType = 0x%x \n", ThermalLog->TempType);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "SequenceNumber = 0x%x \n", ThermalLog->SequenceNumber);
      }
    }
  }
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PMem DisplayErrorLog Ends \n");
}

/**
  Retrieves Error Log from FNV/EKV FW

  @param Host           Pointer to the system host (root) structure
  @param Socket         Socket Number
  @param Ch             DDR Channel ID
  @param Dimm           DIMM Number
  @param LogType        Log type
                        0: Media Log
                        1: Thermal Log
  @param LogLevel       Log level
                        0: Low Priority Log
                        1: High Priority Log
  @param LogInfo        Log info
                        0: Retrieve Log Data
                        1: Retrieve Log Info (SequenceNumber and RequestCount are ignored)
  @param SequenceNumber Sequence number to start at
  @param RequestCount   Number of log entries to request from FW
  @param ReturnData     Pointer to the structure to hold the results
  @param MailboxStatus  Pointer to 8 bit status code from status register

  @retval 0 SUCCESS
  @retval 1 FAILURE

**/
UINT32
FnvGetErrorLog (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Ch,
  UINT8                 Dimm,
  UINT8                 LogType,
  UINT8                 LogLevel,
  UINT8                 LogInfo,
  UINT16                SequenceNumber,
  UINT16                RequestCount,
  ERROR_LOG_RETURN_DATA *ReturnData,
  UINT8                 *MailboxStatus
)
{
  GET_ERROR_LOG_PARAMS ErrorLogParams;
  UINT32               Status;

  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "PmemGetErrorLog Starts\n");

  ZeroMem (&ErrorLogParams, sizeof (ErrorLogParams));
  ErrorLogParams.LogType = LogType;
  ErrorLogParams.LogLevel = LogLevel;
  ErrorLogParams.LogInfo = LogInfo;
  ErrorLogParams.LogPayload = 0;
  ErrorLogParams.SequenceNumber = SequenceNumber;
  ErrorLogParams.RequestCount = RequestCount;

  Status = FnvMailboxInterface(Host, Socket, Ch, Dimm, (UINT32 *) &ErrorLogParams, ReturnData->SmallPayload,
                               GET_ERROR_LOG, MailboxStatus);

  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "PmemGetErrorLog Ends\n");

  return Status;
}

/**
  Retrieves then displays Error Log information from FNV/EKV over SMBUS mailbox

  @param Host           Pointer to the system host (root) structure
  @param Socket         Socket Number
  @param Ch             DDR Channel ID
  @param Dimm           DIMM Number
  @param LogType        Log type
                        0: Media Log
                        1: Thermal Log
  @param LogLevel       Log level
                        0: Low Priority Log
                        1: High Priority Log

  @retval 0 SUCCESS
  @retval 1 FAILURE

**/
UINT32
FnvGetSmbusErrorLog (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Ch,
  UINT8                 Dimm,
  UINT8                 LogType,
  UINT8                 LogLevel
)
{
  UINT8                 LogInfo = 0;
  UINT16                SequenceNumber;
  UINT16                RequestCount;
  ERROR_LOG_RETURN_DATA ReturnData;
  UINT8                 MailboxStatus;
  GET_ERROR_LOG_PARAMS  ErrorLogParams;
  UINT32                Status;
  UINT8                 Count = MAX_MEDIA_LOGS;


  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "PMem GetSmbusErrorLog Starts\n");

  ZeroMem (&ErrorLogParams, sizeof (ErrorLogParams));

  ErrorLogParams.LogType = LogType;
  ErrorLogParams.LogLevel = LogLevel;
  ErrorLogParams.LogInfo = NGN_RETRIEVE_LOG_INFO;
  ErrorLogParams.LogPayload = 0;
  ErrorLogParams.SequenceNumber = 0;
  ErrorLogParams.RequestCount = 0;

  Status = FnvMailboxInterface (Host, Socket, Ch, Dimm, (UINT32 *) &ErrorLogParams, ReturnData.SmallPayload,
    GET_ERROR_LOG, &MailboxStatus);

  if (Status == SUCCESS) {
    // Display Error Log Info
    FnvDisplayErrorLog (Host, Socket, Ch, Dimm, LogType, NGN_RETRIEVE_LOG_INFO, &ReturnData);

    if (!SIMICS_FLAG) {

      LogInfo = NGN_RETRIEVE_LOG_DATA;
      Count = LogType == FNV_MEDIA_LOG_REQ ? MAX_MEDIA_LOGS : MAX_THERMAL_LOGS;
      // Determine starting sequence number and entry count for error log to return
      RequestCount = 1;
      SequenceNumber = ReturnData.LogInfoData.CurrentSequenceNumber;
      while ((RequestCount < Count) &&
        (RequestCount < ReturnData.LogInfoData.MaxLogEntries) &&
        (SequenceNumber != ReturnData.LogInfoData.OldestSequenceNumber)) {
        RequestCount++;
        SequenceNumber--;
        if (SequenceNumber == 0) {    // Sequence number wraps from 0xFFFF to 1
          SequenceNumber = 0xFFFF;
        }
      }
      ErrorLogParams.SequenceNumber = SequenceNumber;
      ErrorLogParams.RequestCount = RequestCount;
      ErrorLogParams.LogInfo = NGN_RETRIEVE_LOG_DATA;

      // Get Error Log Entries
      Status = FnvMailboxInterface (Host, Socket, Ch, Dimm, (UINT32 *)&ErrorLogParams, ReturnData.SmallPayload,
        GET_ERROR_LOG, &MailboxStatus);

      if (Status == SUCCESS) {
        // Display Error Log Entries
        FnvDisplayErrorLog (Host, Socket, Ch, Dimm, LogType, LogInfo, &ReturnData);
      }
    }
  }
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "PMem GetSmbusErrorLog Ends\n");
  return Status;
}

/**
  Gets power management policy from FNV/EKV FW

  This routine issues the Get Features / Power Management Policy command and
  returns the data in the provided buffer.

  @param[in]  Host                  - Pointer to System Host (root) structure
  @param[in]  Socket                - Socket Number
  @param[in]  Ch                    - DDR Channel ID
  @param[in]  Dimm                  - Dimm number of the channel
  @param[out] PowerManagementPolicy - Pointer to structure containing DIMM power information
  @param[out] MailboxStatus         - Pointer to 8 bit status code from status register

  @retval 0                         - SUCCESS
  @retval 1                         - FAILURE
**/
UINT32
GetPowerManagementPolicy (
  IN  SYSHOST                 *Host,
  IN  UINT8                    Socket,
  IN  UINT8                    Ch,
  IN  UINT8                    Dimm,
  IN  UINT16                   FisVersion,
  OUT POWER_MANAGEMENT_POLICY *PowerManagementPolicy,
  OUT UINT8                   *MailboxStatus
  )
{
  UINT32 InputPayload[NUM_PAYLOAD_REG];
  UINT32 OutputPayload[NUM_PAYLOAD_REG];
  UINT32 Status;

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Get Power Management Policy Starts\n");

  if (FisVersion >= 0x201) {
    Status = FnvMailboxInterface (Host, Socket, Ch, Dimm, InputPayload, OutputPayload, DEVICE_CHARACTERISTICS, MailboxStatus);
    if (Status == SUCCESS) {
      PowerManagementPolicy->Fis201.MaxAveragePower = (UINT16)(OutputPayload[3] & 0x0000FFFF);
      PowerManagementPolicy->Fis201.MaxMbbPower = (UINT16)((OutputPayload[3] & 0xFFFF0000) >> 16);
      PowerManagementPolicy->Fis201.MaxMbbAveragePowerTimeConstant = OutputPayload[4];
      PowerManagementPolicy->Fis201.MbbAveragePowerTimeConstantStep = OutputPayload[5];
      PowerManagementPolicy->Fis201.MaxAveragePowerReportingTimeConstant = OutputPayload[6];
      PowerManagementPolicy->Fis201.AveragePowerReportingTimeConstantStep = OutputPayload[7];
    }
    Status = FnvMailboxInterface (Host, Socket, Ch, Dimm, InputPayload, OutputPayload, GET_POWER_MANAGEMENT, MailboxStatus);
    if (Status == SUCCESS) {
      PowerManagementPolicy->Fis201.AveragePowerLimit = (UINT16)(((OutputPayload[1] & 0x000000FF) << 8) | ((OutputPayload[0] & 0xFF000000) >> 24));
      PowerManagementPolicy->Fis201.MbbFeature = (UINT8)((OutputPayload[1] & 0x00FF0000) >> 16);
      PowerManagementPolicy->Fis201.MbbMaxPowerLimit = (UINT16)(((OutputPayload[2] & 0x000000FF) << 8) | ((OutputPayload[1] & 0xFF000000) >> 24));
      PowerManagementPolicy->Fis201.MbbAveragePowerTimeConstant = (((OutputPayload[3] & 0x000000FF) << 24) | ((OutputPayload[2] & 0xFFFFFF00) >> 8));
    }

    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "OutputPayload[0]=%x, OutputPayload[1]=%x, OutputPayload[2]=%x, OutputPayload[3]=%x, OutputPayload[4]=%x, OutputPayload[5]=%x, OutputPayload[6]=%x, OutputPayload[7]=%x\n",
       OutputPayload[0], OutputPayload[1], OutputPayload[2], OutputPayload[3], OutputPayload[4], OutputPayload[5], OutputPayload[6], OutputPayload[7]);
  } else if (FisVersion == 0x200) {
    Status = FnvMailboxInterface (Host, Socket, Ch, Dimm, InputPayload, OutputPayload, DEVICE_CHARACTERISTICS, MailboxStatus);
    if (Status == SUCCESS) {
      PowerManagementPolicy->Fis200.MaxAveragePower = (UINT16)(OutputPayload[3] & 0x0000FFFF);
      PowerManagementPolicy->Fis200.MaxTurboPower = (UINT16)((OutputPayload[3] & 0xFFFF0000) >> 16);
    }
    Status = FnvMailboxInterface (Host, Socket, Ch, Dimm, InputPayload, OutputPayload, GET_POWER_MANAGEMENT, MailboxStatus);
    if (Status == SUCCESS) {
     PowerManagementPolicy->Fis200.AveragePowerLimit = (UINT16)(((OutputPayload[1] & 0x000000FF) << 8) | ((OutputPayload[0] & 0xFF000000)>>24));
     PowerManagementPolicy->Fis200.AveragePowerTimeConstant = (UINT8)((OutputPayload[1] & 0x0000FF00) >> 8);
     PowerManagementPolicy->Fis200.TurboModeState = (UINT8)((OutputPayload[1] & 0x00FF0000) >> 16);
     PowerManagementPolicy->Fis200.TurboPowerLimit = (UINT16)(((OutputPayload[2] & 0x000000FF) << 8) | ((OutputPayload[1] & 0xFF000000) >> 24));
    }
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "OutputPayload[0]=%x, OutputPayload[1]=%x, OutputPayload[2]=%x, OutputPayload[3]=%x\n", OutputPayload[0], OutputPayload[1], OutputPayload[2], OutputPayload[3]);
  } else {
    Status = FnvMailboxInterface (Host, Socket, Ch, Dimm, InputPayload, OutputPayload, GET_POWER_MANAGEMENT, MailboxStatus);
    if (Status == SUCCESS) {
      PowerManagementPolicy->Fis1x.PeakPowerBudget = (UINT16)((OutputPayload[0] & 0x00FFFF00) >> 8);
      PowerManagementPolicy->Fis1x.AveragePowerBudget = (UINT16)(((OutputPayload[1] & 0xFF) << 8) | ((OutputPayload[0] & 0xFF000000) >> 24));
    }
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "OutputPayload[0]=%x, OutputPayload[1]=%x\n", OutputPayload[0], OutputPayload[1]);
  }
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Get Power Management Policy Ends\n");
  return Status;
}

/**
  Sets power management policy to FNV/EKV FW

  This routine issues the Set Features / Power Management Policy command and
  sets the data from the provided buffer.

  @param[in]  Host                  - Pointer to System Host (root) structure
  @param[in]  Socket                - Socket Number
  @param[in]  Ch                    - DDR Channel ID
  @param[in]  Dimm                  - Dimm number of the channel
  @param[in]  PowerManagementPolicy - Pointer to structure containing DIMM power information
  @param[out] MailboxStatus         - Pointer to 8 bit status code from status register

  @retval 0                         - SUCCESS
  @retval 1                         - FAILURE
**/
UINT32
SetPowerManagementPolicy (
  IN  SYSHOST                 *Host,
  IN  UINT8                    Socket,
  IN  UINT8                    Ch,
  IN  UINT8                    Dimm,
  IN  UINT16                   FisVersion,
  IN  POWER_MANAGEMENT_POLICY *PowerManagementPolicy,
  OUT UINT8                   *MailboxStatus
  )
{
  UINT32            InputPayload[NUM_PAYLOAD_REG];
  UINT32            OutputPayload[NUM_PAYLOAD_REG];
  UINT32            Status;

  ZeroMem (InputPayload, NUM_PAYLOAD_REG * sizeof (UINT32));
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Set Power Management Policy Starts\n");

  if (FisVersion >= 0x201) {
    InputPayload[0] = (PowerManagementPolicy->Fis201.AveragePowerLimit & 0x00FF) << 24;
    InputPayload[1] = (((PowerManagementPolicy->Fis201.MbbMaxPowerLimit & 0x00FF) << 24) | ((PowerManagementPolicy->Fis201.MbbFeature) << 16)) |
                      ((PowerManagementPolicy->Fis201.AveragePowerLimit & 0xFF00) >> 8);
    InputPayload[2] = ((PowerManagementPolicy->Fis201.MbbAveragePowerTimeConstant & 0x00FFFFFF) << 8) | ((PowerManagementPolicy->Fis201.MbbMaxPowerLimit & 0xFF00) >> 8);
    InputPayload[3] = ((PowerManagementPolicy->Fis201.MbbAveragePowerTimeConstant & 0xFF000000) >> 24);

    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "InputPayload[0]=%x, InputPayload[1]=%x, InputPayload[2]=%x, InputPayload[3]=%x\n", InputPayload[0], InputPayload[1], InputPayload[2], InputPayload[3]);

    Status = FnvMailboxInterface (Host, Socket, Ch, Dimm, InputPayload, OutputPayload, SET_POWER_MANAGEMENT, MailboxStatus);
  } else if (FisVersion == 0x200) {
    InputPayload[0] = (PowerManagementPolicy->Fis200.AveragePowerLimit & 0x00FF) << 24;
    InputPayload[1] = ((PowerManagementPolicy->Fis200.TurboPowerLimit & 0x00FF) << 24) | ((PowerManagementPolicy->Fis200.TurboModeState) << 16) |
      ((PowerManagementPolicy->Fis200.AveragePowerTimeConstant) << 8) | ((PowerManagementPolicy->Fis200.AveragePowerLimit & 0xFF00) >> 8);
    InputPayload[2] = (PowerManagementPolicy->Fis200.TurboPowerLimit & 0xFF00) >> 8;

    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "InputPayload[0]=%x, InputPayload[1]=%x, InputPayload[2]=%x\n", InputPayload[0], InputPayload[1], InputPayload[2]);

    Status = FnvMailboxInterface (Host, Socket, Ch, Dimm, InputPayload, OutputPayload, SET_POWER_MANAGEMENT, MailboxStatus);
  } else {
    // Set the new policy
    InputPayload[0] = ((PowerManagementPolicy->Fis1x.AveragePowerBudget & 0xFF) << 24) | (PowerManagementPolicy->Fis1x.PeakPowerBudget << 8);
    InputPayload[1] = (PowerManagementPolicy->Fis1x.AveragePowerBudget & 0xFF00) >> 8;

    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "InputPayload[0]=%x, InputPayload[1]=%x\n", InputPayload[0], InputPayload[1]);

    Status = FnvMailboxInterface (Host, Socket, Ch, Dimm, InputPayload, OutputPayload, SET_POWER_MANAGEMENT, MailboxStatus);
  }
  return Status;
}

/**
  Initializes Dcpmm Power Management Policy

  This routine issues GetPowerManagementPolicy and SetPowerManagementPolicy command

  @param[in]  Host                  - Pointer to System Host (root) structure
  @param[in]  Socket                - Socket Number
  @param[in]  Ch                    - DDR Channel ID
  @param[in]  Dimm                  - Dimm number of the channel
**/
VOID
InitDcpmmPowerManagementPolicy (
  IN  SYSHOST                  *Host,
  IN  UINT8                    Socket,
  IN  UINT8                    Ch,
  IN  UINT8                    Dimm,
  IN  UINT16                   FisVersion
  )
{
  POWER_MANAGEMENT_POLICY                      NewPowerManagementPolicy;
  POWER_MANAGEMENT_POLICY                      CurrentPowerManagementPolicy;
  DIMM_NVRAM_STRUCT                            (*DimmNvList)[MAX_DIMM];
  UINT8                                        MailboxStatus;
  UINT32                                       Status;
  UINT16                                       NvmCapacity;
  SYS_SETUP                                    *Setup;

  Setup = GetSysSetupPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  //
  // Get current power management policy from DIMM
  //
  MailboxStatus = MB_STAT_SUCCESS;
  Status = GetPowerManagementPolicy (Host, Socket, Ch, Dimm, FisVersion, &CurrentPowerManagementPolicy, &MailboxStatus);

  if (Status == SUCCESS) {
    if (FisVersion >= 0x201) {
      //
      // FIS 2.1 Get requested power management policy from setup option
      //
      NewPowerManagementPolicy.Fis201.AveragePowerLimit = Setup->mem.DcpmmAveragePowerLimit;
      NewPowerManagementPolicy.Fis201.MbbAveragePowerTimeConstant = Setup->mem.DcpmmMbbAveragePowerTimeConstant;
      NewPowerManagementPolicy.Fis201.MbbMaxPowerLimit = Setup->mem.DcpmmMbbMaxPowerLimit;
      NewPowerManagementPolicy.Fis201.MbbFeature = Setup->mem.DcpmmMbbFeature;

      //
      // Store maximum value of Average Power limit, Mbb Power Limit, Mbb Average Power Time Constant
      // obtained from device characteristics to Host
      //
      Host->var.mem.MaxAveragePowerLimit = CurrentPowerManagementPolicy.Fis201.MaxAveragePower;
      Host->var.mem.MaxMbbPowerLimit = CurrentPowerManagementPolicy.Fis201.MaxMbbPower;
      Host->var.mem.MaxMbbAveragePowerTimeConstant = CurrentPowerManagementPolicy.Fis201.MaxMbbAveragePowerTimeConstant;

      if (CurrentPowerManagementPolicy.Fis201.MaxMbbPower == 0) {
        NewPowerManagementPolicy.Fis201.MbbFeature = TURBO_MODE_STATE_DISABLED;
      }

      //
      // Check if policy has changed for FIS 2.1
      //
      if ((NewPowerManagementPolicy.Fis201.AveragePowerLimit != CurrentPowerManagementPolicy.Fis201.AveragePowerLimit) ||
        (NewPowerManagementPolicy.Fis201.MbbMaxPowerLimit != CurrentPowerManagementPolicy.Fis201.MbbMaxPowerLimit) ||
        (NewPowerManagementPolicy.Fis201.MbbAveragePowerTimeConstant != CurrentPowerManagementPolicy.Fis201.MbbAveragePowerTimeConstant)) {

        if (NewPowerManagementPolicy.Fis201.AveragePowerLimit < AVERAGE_POWER_LIMIT_MIN) {
          NewPowerManagementPolicy.Fis201.AveragePowerLimit = AVERAGE_POWER_LIMIT_MIN;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "Invalid Average Power Limit requested so setting to minimum\n");
        }

        if (NewPowerManagementPolicy.Fis201.AveragePowerLimit > AVERAGE_POWER_LIMIT_MAX) {
          NewPowerManagementPolicy.Fis201.AveragePowerLimit = AVERAGE_POWER_LIMIT_MAX;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "Invalid Average Power Limit requested so setting to maximum\n");
        }

        if (NewPowerManagementPolicy.Fis201.AveragePowerLimit % AVERAGE_POWER_LIMIT_INCREMENT) {
          NewPowerManagementPolicy.Fis201.AveragePowerLimit = AVERAGE_POWER_LIMIT_DEFAULT;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "Invalid Average Power Limit requested so setting to default\n");
        }

        if (NewPowerManagementPolicy.Fis201.MbbMaxPowerLimit < TURBO_POWER_LIMIT_MIN) {
          NewPowerManagementPolicy.Fis201.MbbMaxPowerLimit = TURBO_POWER_LIMIT_MIN;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "Invalid Memory Bandwidth Boost Max Power Limit requested so setting minimum\n");
        }

        if (NewPowerManagementPolicy.Fis201.MbbMaxPowerLimit > TURBO_POWER_LIMIT_MAX) {
          NewPowerManagementPolicy.Fis201.MbbMaxPowerLimit = TURBO_POWER_LIMIT_MAX;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "Invalid Memory Bandwidth Boost Max Power Limit requested so setting maximum\n");
        }

        if (NewPowerManagementPolicy.Fis201.MbbMaxPowerLimit % AVERAGE_POWER_LIMIT_INCREMENT) {
          NewPowerManagementPolicy.Fis201.MbbMaxPowerLimit = TURBO_POWER_LIMIT_DEFAULT;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "Invalid Memory Bandwidth Boost Max Power Limit requested so setting default\n");
        }

        if (NewPowerManagementPolicy.Fis201.MbbAveragePowerTimeConstant < AVERAGE_POWER_TIME_CONSTANT_MIN_2_1_OR_LATER) {
          NewPowerManagementPolicy.Fis201.MbbAveragePowerTimeConstant = AVERAGE_POWER_TIME_CONSTANT_MIN_2_1_OR_LATER;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "Invalid Memory Bandwidth Boost Average Power Time Constant so setting minimum\n");
        }

        if (NewPowerManagementPolicy.Fis201.MbbAveragePowerTimeConstant > AVERAGE_POWER_TIME_CONSTANT_MAX_2_1_OR_LATER) {
          NewPowerManagementPolicy.Fis201.MbbAveragePowerTimeConstant = AVERAGE_POWER_TIME_CONSTANT_MAX_2_1_OR_LATER;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "Invalid Memory Bandwidth Boost Average Power Time Constant so setting maximum\n");
        }

        if (NewPowerManagementPolicy.Fis201.MbbAveragePowerTimeConstant % AVERAGE_POWER_TIME_CONSTANT_INCREMENT_2_1_OR_LATER) {
          NewPowerManagementPolicy.Fis201.MbbAveragePowerTimeConstant = AVERAGE_POWER_TIME_CONSTANT_DEFAULT_2_1_OR_LATER;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "Invalid Memory Bandwidth Boost Average Power Time Constant so setting default\n");
        }

      }
    } else if (FisVersion == 0x200) {
      //
      // FIS 2.0 Get requested power management policy from setup option
      //
      NewPowerManagementPolicy.Fis200.AveragePowerLimit = Setup->mem.DcpmmAveragePowerLimit;
      NewPowerManagementPolicy.Fis200.AveragePowerTimeConstant = Setup->mem.DcpmmAveragePowerTimeConstant;
      NewPowerManagementPolicy.Fis200.TurboPowerLimit = Setup->mem.DcpmmMbbMaxPowerLimit;
      NewPowerManagementPolicy.Fis200.TurboModeState = Setup->mem.DcpmmMbbFeature;

      if (CurrentPowerManagementPolicy.Fis200.MaxTurboPower == 0) {
        NewPowerManagementPolicy.Fis200.TurboModeState = TURBO_MODE_STATE_DISABLED;
      }

      //
      // Check if policy has changed for FIS 2.0
      //
      if ((NewPowerManagementPolicy.Fis200.AveragePowerLimit != CurrentPowerManagementPolicy.Fis200.AveragePowerLimit) ||
        (NewPowerManagementPolicy.Fis200.TurboPowerLimit != CurrentPowerManagementPolicy.Fis200.TurboPowerLimit)) {

        if (NewPowerManagementPolicy.Fis200.AveragePowerLimit < AVERAGE_POWER_LIMIT_MIN) {
          NewPowerManagementPolicy.Fis200.AveragePowerLimit = AVERAGE_POWER_LIMIT_MIN;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Invalid Average Power Limit requested so setting to minimum\n");
        }

        if (NewPowerManagementPolicy.Fis200.AveragePowerLimit > AVERAGE_POWER_LIMIT_MAX) {
          NewPowerManagementPolicy.Fis200.AveragePowerLimit = AVERAGE_POWER_LIMIT_MAX;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Invalid Average Power Limit requested so setting to maximum\n");
        }

        if (NewPowerManagementPolicy.Fis200.AveragePowerLimit % AVERAGE_POWER_LIMIT_INCREMENT) {
          NewPowerManagementPolicy.Fis200.AveragePowerLimit = AVERAGE_POWER_LIMIT_DEFAULT;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Invalid Average Power Limit requested so setting to default\n");
        }

        if (NewPowerManagementPolicy.Fis200.TurboPowerLimit < TURBO_POWER_LIMIT_MIN) {
          NewPowerManagementPolicy.Fis200.TurboPowerLimit = TURBO_POWER_LIMIT_MIN;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Invalid Turbo Power Limit requested so setting minimum\n");
        }

        if (NewPowerManagementPolicy.Fis200.TurboPowerLimit > TURBO_POWER_LIMIT_MAX) {
          NewPowerManagementPolicy.Fis200.TurboPowerLimit = TURBO_POWER_LIMIT_MAX;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Invalid Turbo Power Limit requested so setting maximum\n");
        }

        if (NewPowerManagementPolicy.Fis200.TurboPowerLimit % AVERAGE_POWER_LIMIT_INCREMENT) {
          NewPowerManagementPolicy.Fis200.TurboPowerLimit = TURBO_POWER_LIMIT_DEFAULT;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Invalid Turbo Power Limit requested so setting default\n");
        }
      }
    } else {
      //
      // Get requested NGN DIMM power management policy (Average Power value) from setup option
      //
      NewPowerManagementPolicy.Fis1x.PeakPowerBudget = NGN_PEAK_POWER_DEFAULT;
      NewPowerManagementPolicy.Fis1x.AveragePowerBudget = Setup->mem.NgnAveragePower;

      //
      // Force Average Power budget to match Peak Power budget if necessary
      //
      if (NewPowerManagementPolicy.Fis1x.AveragePowerBudget > NewPowerManagementPolicy.Fis1x.PeakPowerBudget) {
        NewPowerManagementPolicy.Fis1x.AveragePowerBudget = NewPowerManagementPolicy.Fis1x.PeakPowerBudget;
      }
      //
      // Check if policy has changed for FIS 1.0
      //
      if ((NewPowerManagementPolicy.Fis1x.PeakPowerBudget != CurrentPowerManagementPolicy.Fis1x.PeakPowerBudget) ||
        (NewPowerManagementPolicy.Fis1x.AveragePowerBudget != CurrentPowerManagementPolicy.Fis1x.AveragePowerBudget)) {

        //
        // Check that requested power management policy is valid
        //
        if ((NewPowerManagementPolicy.Fis1x.PeakPowerBudget < NGN_PEAK_POWER_MIN) ||
          (NewPowerManagementPolicy.Fis1x.PeakPowerBudget > NGN_PEAK_POWER_MAX) ||
          (NewPowerManagementPolicy.Fis1x.PeakPowerBudget % NGN_PEAK_POWER_INCREMENT)) {
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Invalid PMem DIMM peak power requested so setting default\n");
          NewPowerManagementPolicy.Fis1x.PeakPowerBudget = NGN_PEAK_POWER_DEFAULT;
        }

        //
        // check whether Average power is in valid range based on EDS table per AEP DIMM capacity.
        // NVM capacity based on SPD bytes
        //
        NvmCapacity = (UINT16)(((*DimmNvList)[Dimm].NvmDevDensity & 0xf) << 5) + (((*DimmNvList)[Dimm].NvmDevType & 0x7c) >> 2);
        switch (NvmCapacity) {
        case CAPACITYSPD64GB:
        case CAPACITYSPD128GB:
          if ((NewPowerManagementPolicy.Fis1x.AveragePowerBudget < NGN_128GB_AVERAGE_POWER_MIN) ||
            (NewPowerManagementPolicy.Fis1x.AveragePowerBudget > NGN_128GB_AVERAGE_POWER_MAX) ||
            (NewPowerManagementPolicy.Fis1x.AveragePowerBudget % NGN_AVERAGE_POWER_INCREMENT)) {
            RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Invalid PMem DIMM average power requested for 128GB; so setting default\n");
            NewPowerManagementPolicy.Fis1x.AveragePowerBudget = NGN_128GB_AVERAGE_POWER_DEFAULT;
          }
          break;
        case CAPACITYSPD256GB:
          if ((NewPowerManagementPolicy.Fis1x.AveragePowerBudget < NGN_256GB_AVERAGE_POWER_MIN) ||
            (NewPowerManagementPolicy.Fis1x.AveragePowerBudget > NGN_256GB_AVERAGE_POWER_MAX) ||
            (NewPowerManagementPolicy.Fis1x.AveragePowerBudget % NGN_AVERAGE_POWER_INCREMENT)) {
            RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Invalid PMem DIMM average power requested for 256GB; so setting default\n");
            NewPowerManagementPolicy.Fis1x.AveragePowerBudget = NGN_256_512GB_AVERAGE_POWER_DEFAULT;
          }
          break;
        case CAPACITYSPD512GB:
          if ((NewPowerManagementPolicy.Fis1x.AveragePowerBudget < NGN_512GB_AVERAGE_POWER_MIN) ||
            (NewPowerManagementPolicy.Fis1x.AveragePowerBudget > NGN_512GB_AVERAGE_POWER_MAX) ||
            (NewPowerManagementPolicy.Fis1x.AveragePowerBudget % NGN_AVERAGE_POWER_INCREMENT)) {
            RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Invalid PMem DIMM average power requested for 256GB; so setting default\n");
            NewPowerManagementPolicy.Fis1x.AveragePowerBudget = NGN_256_512GB_AVERAGE_POWER_DEFAULT;
          }
          break;
        default:
          //
          // unsupported AEP DIMM capacity
          //
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Unsupported PMem DIMM capacity\n");
          break;
        }
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "AVG power of %d for DIMM capacity 0x%x\n", NewPowerManagementPolicy.Fis1x.AveragePowerBudget, NvmCapacity);
      }
    }
    // Log error and disable channel if NGN communication failure
  } else if (MailboxStatus != MB_STAT_SUCCESS) {

    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Communication with the PMem DIMM failed getting power management policy\n");

    if (MailboxStatus != MB_STAT_MEDIA_DISABLED) {   // Allow media disabled case to continue booting
      // log data format:
      // Byte 0 (Bit 0 - DIMM Slot Number, Bits 1-3 - Channel Number, Bits 4-6 - Socket Number,Bit 7 - Reserved)
      // Byte 1 - FW MB Opcode,Byte 2 - FW MB Sub-Opcode,Byte 3 - FW MB Status Code
      EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS, Socket, Ch, Dimm, NO_RANK);
      DisableDIMM (Host, Socket, Ch, Dimm);
    }
  }

  //
  // Set new power management policy on DIMM
  //
  MailboxStatus = MB_STAT_SUCCESS;
  Status = SetPowerManagementPolicy (Host, Socket, Ch, Dimm, FisVersion, &NewPowerManagementPolicy, &MailboxStatus);
  if (Status == SUCCESS) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Set Power Management Policy Successful\n");
    // Log error and disable channel if NGN communication failure
  } else if (MailboxStatus != MB_STAT_SUCCESS) {

    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Communication with the PMem DIMM failed setting power management policy\n");

    if (MailboxStatus != MB_STAT_MEDIA_DISABLED) {   // Allow media disabled case to continue booting
      // log data format:
      // Byte 0 (Bit 0 - DIMM Slot Number, Bits 1-3 - Channel Number, Bits 4-6 - Socket Number,Bit 7 - Reserved)
      // Byte 1 - FW MB Opcode,Byte 2 - FW MB Sub-Opcode,Byte 3 - FW MB Status Code
      EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS, Socket, Ch, Dimm, NO_RANK);
      DisableDIMM (Host, Socket, Ch, Dimm);
    }
  }
}

/**
  Writes the requested platform configuration data to the DIMM

  @param[in]  Host           - Pointer to the system Host (root) structure
  @param[in]  Socket         - Socket Number
  @param[in]  Ch             - DDR Channel ID
  @param[in]  Dimm           - DIMM number
  @param[in]  PartitionId    - 0 - BIOS (only accessible via the BIOS mailbox), 1 - OEM, 2 - Namespace Label Storage Area
  @param[in]  Offset        - Offset in bytes of the partition to start reading
  @param[in]  FnvBuffer     - Pointer to structure containing everything NVMCTLR
  @param[out] MailboxStatus - Pointer to 8 bit status code from status register

  @retval 0 SUCCESS
  @retval 1 FAILURE
**/
UINT32
SetPlatformConfigData (
  IN  SYSHOST      *Host,
  IN  UINT8         Socket,
  IN  UINT8         Ch,
  IN  UINT8         Dimm,
  IN  UINT8         PartitionId,
  IN  UINT32        Offset,
  IN  FNVCISBUFFER  FnvBuffer,
  OUT UINT8        *MailboxStatus
  )
{
  UINT32                    InputPayload[NUM_PAYLOAD_REG];
  UINT32                    OutputPayload[NUM_PAYLOAD_REG];
  UINT32                    Status;
  UINT8                     PayloadType = SMALL_PAYLOAD;
  UINT8                     i;
  BOOT_STATUS_REGISTER      Bsr;
  BOOT_STATUS_HIGH_REGISTER BsrHi;

  ZeroMem (InputPayload, NUM_PAYLOAD_REG * sizeof(UINT32));
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "SetPlatformConfigData Starts\n");

  ReadBootStatusRegisters (Host, Socket, Ch, Dimm, &Bsr, &BsrHi);
  if (Bsr.Bits.MediaDisabled == 1) {
    return WARN_NVMCTRL_MEDIA_INERROR;
  }

  if (Bsr.Bits.MediaReady == MEDIA_READY) {
    InputPayload[0] = PartitionId | (PayloadType << 8) | (Offset << 16);
    InputPayload[1] = Offset >> 16;
    for (i = 0; i < 16; i++){
      InputPayload[i+16] = FnvBuffer->platformData[i];
    }
    Status = FnvMailboxInterface (Host, Socket, Ch, Dimm, InputPayload, OutputPayload, SET_PLATFORM_CONFIG, MailboxStatus);
  } else if (Bsr.Bits.MediaReady == MEDIA_NOTREADY) {
    Status = WARN_NVMCTRL_MEDIA_NOTREADY;
  } else if (Bsr.Bits.MediaReady == MEDIA_INERROR) {
    Status = WARN_NVMCTRL_MEDIA_INERROR;
  } else {
    Status = WARN_NVMCTRL_MEDIA_RESERVED;
  }

  return Status;
}

/**
  Gets either the size of the platform configuration or platform configuration data itself

  @param[in]  Host           - Pointer to the system Host (root) structure
  @param[in]  Socket         - Socket Number
  @param[in]  Ch             - DDR Channel ID
  @param[in]  Dimm           - DIMM number
  @param[in]  PartitionId    - 0 - BIOS (only accessible via the BIOS mailbox), 1 - OEM, 2 - Namespace Label Storage Area
  @param[in]  RetreiveOption - 0 - Partition data (default), 1 - Partition size
  @param[in]  Offset         - Offset in bytes of the partition to start reading
  @param[out] FnvBuffer      - Pointer to structure containing everything NVMCTLR
  @param[out] MboxStatusPtr  - Pointer to 8 bit status code from status register

  @retval 0 SUCCESS
  @retval 1 FAILURE
**/
UINT32
GetPlatformConfigData (
  IN  SYSHOST      *Host,
  IN  UINT8         Socket,
  IN  UINT8         Ch,
  IN  UINT8         Dimm,
  IN  UINT8         PartitionId,
  IN  UINT8         RetrieveOption,
  IN  UINT32        Offset,
  OUT FNVCISBUFFER  FnvBuffer,
  OUT UINT8        *MboxStatusPtr
  )
{
  UINT32                    InputPayload[NUM_PAYLOAD_REG] ;
  UINT32                    OutputPayload[NUM_PAYLOAD_REG];
  UINT32                    Status;
  UINT8                     PayloadType = SMALL_PAYLOAD;
  UINT8                     i;
  BOOT_STATUS_REGISTER      Bsr;
  BOOT_STATUS_HIGH_REGISTER BsrHi;

  ZeroMem (InputPayload, NUM_PAYLOAD_REG * sizeof(UINT32));
  ZeroMem (OutputPayload, NUM_PAYLOAD_REG * sizeof(UINT32));
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "GetPlatformConfigData Starts\n");

  ReadBootStatusRegisters (Host, Socket, Ch, Dimm, &Bsr, &BsrHi);
  if (Bsr.Bits.MediaDisabled == 1) {
    return WARN_NVMCTRL_MEDIA_INERROR;
  }


  if (Bsr.Bits.MediaReady == MEDIA_READY) {
    InputPayload[0] = PartitionId | (PayloadType << 8) | (RetrieveOption << 9) | (Offset << 16);
    InputPayload[1] = Offset >> 16;
    Status = FnvMailboxInterface (Host, Socket, Ch, Dimm, InputPayload, OutputPayload, GET_PLATFORM_CONFIG, MboxStatusPtr);
    if (Status == SUCCESS) {
      if (RetrieveOption == 1) {
        FnvBuffer->platformSize = OutputPayload[0];
      } else {
        for (i = 0; i < NUM_PAYLOAD_REG; i++) {
          FnvBuffer->platformData[i] = OutputPayload[i];
        }
      }
    } else if ((*MboxStatusPtr == MB_STAT_MEDIA_DISABLED) || (*MboxStatusPtr == MB_STAT_INTERNAL_DEVICE_ERROR)) {
      Status = WARN_NVMCTRL_MEDIA_INERROR;
    }
  } else if (Bsr.Bits.MediaReady == MEDIA_NOTREADY) {
    Status = WARN_NVMCTRL_MEDIA_NOTREADY;
  } else if (Bsr.Bits.MediaReady == MEDIA_INERROR) {
    Status = WARN_NVMCTRL_MEDIA_INERROR;
  } else {
    Status = WARN_NVMCTRL_MEDIA_RESERVED;
  }

  return Status;
}

/**
  Sets Latch System Shutdown State

  @param[in]  Host     - Pointer to System Host (root) structure
  @param[in]  Socket   - Socket Number
  @param[in]  Ch       - DDR Channel ID
  @param[in]  Dimm     - Dimm number of the channel
  @parma[in]  State    - Latch System Shutdown State (enable/disable)
  @param[out] MbStatus - Pointer to 8 bit status code from status register
**/
UINT32
SetLatchSystemShutdownState (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     State,
  OUT  UINT8    *MbStatus
  )
{
  SET_LATCH_SYSTEM_SHUTDOWN_STATE Request;
  UINT32                          InputPayload[NUM_PAYLOAD_REG];
  UINT32                          OutputPayload[NUM_PAYLOAD_REG];
  UINT32                          Status;

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "SetLatchSystemShutdownState Starts\n");

  if (State >= LSSS_MAX) {
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "SetLatchSystemShutdownState (invalid LSSS state - %d)\n", State);
    return FAILURE;
  }

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "SetLatchSystemShutdownState (setting LSSS state - %d)\n", State);

  ZeroMem ((UINT8 *) &Request, sizeof (Request));
  Request.Bits.LatchSystemShutdownStateEn = State;
  InputPayload[0] = Request.DWord;

  Status = FnvMailboxInterface (Host, Socket, Ch, Dimm, InputPayload, OutputPayload, SET_LATCH_SYSTEM_SHUTDOWN_STATUS, MbStatus);

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "SetLatchSystemShutdownState Ends\n");

  return Status;
}

/**
  Sets Extended ADR State

  @param[in]  Host     - Pointer to System Host (root) structure
  @param[in]  Socket   - Socket Number
  @param[in]  Ch       - DDR Channel ID
  @param[in]  Dimm     - Dimm number of the channel
  @parma[in]  State    - Extended ADR State (enable/disable)
  @param[out] MbStatus - Pointer to 8 bit status code from status register
**/
UINT32
SetExtendedAdrState (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     State,
  OUT  UINT8    *MbStatus
  )
{
  SET_EXTENDED_ADR_STATE  Request;
  UINT32                  InputPayload[NUM_PAYLOAD_REG];
  UINT32                  OutputPayload[NUM_PAYLOAD_REG];
  UINT32                  Status;

  if (NULL == MbStatus) {
    return FAILURE;
  }
  *MbStatus = MB_STAT_UNDEFINED;

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "SetExtendedAdrState Starts\n");

  if (State >= EXTENDED_ADR_MAX) {
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "SetExtendedAdrState (invalid Extended ADR state - %d)\n", State);
    return FAILURE;
  }

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "SetExtendedAdrState (setting Extended ADR state - %d)\n", State);

  ZeroMem ((UINT8 *) &Request, sizeof (Request));
  Request.Bits.ExtendedAdrEn = State;
  InputPayload[0] = Request.DWord;

  Status = FnvMailboxInterface (Host, Socket, Ch, Dimm, InputPayload, OutputPayload, SET_EADR_STATE, MbStatus);

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "SetExtendedAdrState Ends\n");

  return Status;
}

/**
*Updated 10/15/2014*
*NVMCTLR CIS 0.81*

Routine Description: GetSecurityState will get the security state of the dimm
1. Read Security from SMBus mailbox
2. If mailbox read is successful, transfer output Payload register to security
3. return status

  @param Host        - Pointer to the system Host (root) structure
  @param socket      - Socket Number
  @param ch          - DDR Channel ID
  @param dimm        - DIMM number
  @param security    - pass by reference to security
  @param mbStatus    - Pointer to 8 bit status code from status register
@retval 0 SUCCESS
@retval 1 FAILURE

**/
UINT32
GetSecurityState (
                  PSYSHOST      Host,
                  UINT8         socket,
                  UINT8         ch,
                  UINT8         dimm,
                  UINT32        *security,
                  UINT32        *OptStatus,
                  UINT8         *mbStatus
)
{
  UINT32 inputPayload[NUM_PAYLOAD_REG];
  UINT32 outputPayload[NUM_PAYLOAD_REG];
  UINT32 status;

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "GetSecurityState Starts\n");

  ZeroMem (inputPayload, NUM_PAYLOAD_REG * sizeof (UINT32));
  ZeroMem (outputPayload, NUM_PAYLOAD_REG * sizeof (UINT32));

  status = FnvMailboxInterface (Host, socket, ch, dimm, inputPayload, outputPayload, GET_SECURITY_STATE, mbStatus);
  if (status == SUCCESS){
    *security = outputPayload[0];
    *OptStatus = outputPayload[1];
  }

  return status;
}

/**
Clears FW logs & resets registers to default state.

@param[in]  Host     - Pointer to System Host (root) structure
@param[in]  Socket   - Socket Number
@param[in]  Ch       - DDR Channel ID
@param[in]  Dimm     - Dimm number of the channel
@param[out] MbStatus - Pointer to 8 bit status code from status register
**/
UINT32
FactoryResetClear (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8    *MbStatus
  )
{
  UINT32 InputPayload[NUM_PAYLOAD_REG];
  UINT32 OutputPayload[NUM_PAYLOAD_REG];
  UINT32 Status;

  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "FactoryResetClear Starts\n");

  Status = FnvMailboxInterface (Host, Socket, Ch, Dimm, InputPayload, OutputPayload, CR_FACTORY_RESET, MbStatus);

  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "FactoryResetClear Ends\n");

  return Status;
}

/**
*NVMCTLR CIS 0.82*
Routine Description: Secure Erase Unit will Erase the dimm
1. Transfer passphrase to input payload registers
2. Write to SMBus mailbox
3. if mailbox read fails, return failure
4. return status

  @param Host        - Pointer to the system Host (root) structure
  @param socket      - Socket Number
  @param ch          - DDR Channel ID
  @param dimm        - DIMM number
  @param passphrase  - passphrase
  @param mbStatus    - Pointer to 8 bit status code from status register
@retval 0 SUCCESS
@retval 1 FAILURE
**/
UINT32
SecureEraseUnit (
                  PSYSHOST      Host,
                  UINT8         socket,
                  UINT8         ch,
                  UINT8         dimm,
                  UINT8         *passphrase,
                  UINT8         *mbStatus
)
{
  UINT32 inputPayload[NUM_PAYLOAD_REG];
  UINT32 outputPayload[NUM_PAYLOAD_REG];
  UINT32 status;
  UINT8  i,j;

  ZeroMem (inputPayload, NUM_PAYLOAD_REG * sizeof(UINT32));
  ZeroMem (outputPayload, NUM_PAYLOAD_REG * sizeof(UINT32));

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "SecureEraseUnit Starts\n");
  i = 0;
  for (j = 0; j < PASSPHRASE_COUNTER; j++) {
    inputPayload[j] =  (passphrase[i + 3] << 24) | (passphrase[i + 2] << 16) | (passphrase[i + 1] << 8) | (passphrase[i]) ;
    i +=4;
  }
  inputPayload[8] = 0;

  status = FnvMailboxInterface (Host, socket, ch, dimm, inputPayload, outputPayload, SECURE_ERASE_UNIT, mbStatus);

  return status;
}

/**
*Updated 10/15/2014*
*NVMCTLR CIS 0.81*

Routine Description: SetDimmPartitionInfo will send in new partitions to NVMCTLR Dimm
1. Get the security state of dimm
2. If mailbox read fails, return failure
3. If security state is locked, then return failure
4. Get the Pointer to dimmPartition
5. Transfer new partition values into input payload register
6. Write to SMBus mailbox
7. if mailbox read fails, return failure
8. Call GetDimmPartition to check if SetDimmPartitionInfo was successful. Older values will be store in FNVBuffer. Set Status accordingly.
9. return Status

  @param Host        - Pointer to the system Host (root) structure
  @param socket      - Socket Number
  @param ch          - DDR Channel ID
  @param dimm        - DIMM number
  @param twoLM       - 2LM volatile memory partition
  @param PM          - persistent memory partition
  @param fnvBuffer   - structure containing everything NVMCTLR
  @param mbStatus    - Pointer to 8 bit status code from status register
@retval 0 SUCCESS
@retval 1 FAILURE

**/
UINT32
SetDimmPartitionInfo(
                      PSYSHOST      Host,
                      UINT8         socket,
                      UINT8         ch,
                      UINT8         dimm,
                      UINT32        twoLM,
                      UINT32        PM,
                      FNVCISBUFFER fnvBuffer,
                      UINT8        *mbStatus
)
{
  UINT32 inputPayload[NUM_PAYLOAD_REG];
  UINT32 outputPayload[NUM_PAYLOAD_REG];
  UINT32 status;
  DIMM_PARTITION_DATA (*dimmPartitionList)[MAX_SOCKET][MAX_CH][MAX_DIMM];
  UINT32  Security;
  UINT32  OptStatus;

  ZeroMem (inputPayload, NUM_PAYLOAD_REG * sizeof(UINT32));

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "SetDimmPartitionInfo Starts\n");

  status = GetSecurityState (Host, socket, ch, dimm, &Security, &OptStatus, mbStatus);

  if (status == FAILURE) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "GetSecurityState failure; mbStatus=%d\n", mbStatus);
    return status;
  }

  if (Security & SECURITY_LOCKED) {
    RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Security State - Locked\n");
    return FAILURE;
  }

  dimmPartitionList = GetFnvCisBufferDimmPartition(fnvBuffer);

  inputPayload[0] = twoLM;
  inputPayload[1] = PM;
  status = FnvMailboxInterface (Host, socket, ch, dimm, inputPayload, outputPayload, SET_DIMM_PARTITION, mbStatus);

  //If mailbox read fails, return failure

  if (status == FAILURE) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "SetDimmPartitionInfo failure; mbStatus=%d\n", mbStatus);
    return status;
  }

  // Get DIMM Partition Info
  status = GetDimmPartitionInfo(Host, socket, ch, dimm, fnvBuffer, mbStatus);
  // if mailbox read fails, return failure
  if (status == FAILURE) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "GetDimmPartitionInfo failure; mbStatus=%d\n", mbStatus);
    return status;
  }

  // If values are different, then write failed
  if (((*dimmPartitionList)[socket][ch][dimm].volatileCap == twoLM) &&
      ((*dimmPartitionList)[socket][ch][dimm].persistentCap == PM)) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "SetDimmPartitionInfo and GetDimmPartitionInfo values matched\n");
    status = SUCCESS;
  } else {
    RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "SetDimmPartitionInfo(4KB); twoLM=%d; PM=%d; GetDimmPartitionInfo; volatileCap=%d; persistentCap=%d; NOT matched\n", twoLM, PM, (*dimmPartitionList)[socket][ch][dimm].volatileCap, (*dimmPartitionList)[socket][ch][dimm].persistentCap);
    status = FAILURE;
  }

  return status;
}
/**
*Updated 10/15/2014*
*NVMCTLR CIS 0.81*

Routine Description: GetDimmPartitionInfo will store the partition info into the fnvBuffer.
1. Get the Pointer to dimmPartition
2. Read partition information from the SMBus mailbox
3. If read is succesful, then transfer all information from output payload registers into the fnvBuffer
4. return status

Manufacturer, Serial Number, and Model Number are dumped into a data buffer
  @param Host        - Pointer to the system Host (root) structure
  @param socket      - Socket Number
  @param ch          - DDR Channel ID
  @param dimm        - DIMM number
  @param fnvBuffer   - structure containing everything NVMCTLR
  @param mbStatus     - Pointer to 8 bit status code from status register

@retval 0 SUCCESS
@retval 1 FAILURE

**/
UINT32
GetDimmPartitionInfo(
                       PSYSHOST     Host,
                       UINT8        socket,
                       UINT8        ch,
                       UINT8        dimm,
                       FNVCISBUFFER fnvBuffer,
                       UINT8        *mbStatus
)
{
  UINT32 inputPayload [NUM_PAYLOAD_REG];
  UINT32 outputPayload [NUM_PAYLOAD_REG];
  DIMM_PARTITION_DATA (*dimmPartitionList)[MAX_SOCKET][MAX_CH][MAX_DIMM];
  UINT32 status;

  ZeroMem (outputPayload, NUM_PAYLOAD_REG * sizeof(UINT32));

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "GetDimmPartitionInfo Starts\n");

  dimmPartitionList = GetFnvCisBufferDimmPartition(fnvBuffer);

  status = FnvMailboxInterface(Host, socket, ch, dimm, inputPayload, outputPayload, GET_DIMM_PARTITION, mbStatus);
  if (status == SUCCESS) {
    (*dimmPartitionList)[socket][ch][dimm].volatileCap        = outputPayload[0]; // Volatile Capacity in 4KB granularity 3:0
    (*dimmPartitionList)[socket][ch][dimm].volatileStart.lo   = outputPayload[2]; // DPA start address of 2LM region Least Significant 11:8
    (*dimmPartitionList)[socket][ch][dimm].volatileStart.hi   = outputPayload[3]; // DPA start address of 2LM region Most Significant 15:12
    (*dimmPartitionList)[socket][ch][dimm].persistentCap      = outputPayload[4]; // Persisent Memory Capacitiy in 4KB granularity 19:16
    (*dimmPartitionList)[socket][ch][dimm].persistentStart.lo = outputPayload[6]; // DPA start address of PM region Least Significant 27:24
    (*dimmPartitionList)[socket][ch][dimm].persistentStart.hi = outputPayload[7]; // DPA start address of PM region Most Significant 31:28
    (*dimmPartitionList)[socket][ch][dimm].rc                 = outputPayload[8]; // Raw usable size of Volatile and Persisent in 4KB granularity 35:32
  }

  return status;
}

/**
  Routine Description:
  SaveDdrtTrainingFailureCode will send the BIOS training failure code to the DCMPMM DIMM

  @param[in]  WarningCode           - Major warning code.
  @param[in]  MinorWarningCode      - Minor warning code.
  @param[in]  Socket                - Socket Number
  @param[in]  Channel               - DDR Channel ID
  @param[in]  Dimm                  - Dimm number of the channel

  @retval 0 SUCCESS
  @retval 1 FAILURE
**/
UINT32
SaveDdrtTrainingFailureCode (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm
)
{
  return SUCCESS;
}

/**
  Issues Identify command to DIMM and returns info provided.

  @param[in]  Host          - Pointer to the system Host (root) structure
  @param[in]  Socket        - Socket Number
  @param[in]  Ch            - DDR Channel ID
  @param[in]  Dimm          - DIMM number
  @param[in]  AitDramFlag   - AIT Dram Flag to check if BIOS support for FIS 1.11 is enabled/disabled
  @param[out] DimmInfo      - Pointer to DIMM information structure
  @param[out] MailboxStatus - Pointer to 8 bit status code from status register

  @retval 0 SUCCESS
  @retval 1 FAILURE
**/
UINT32
IdentifyDimm (
  IN  SYSHOST          *Host,
  IN  UINT8             Socket,
  IN  UINT8             Ch,
  IN  UINT8             Dimm,
  IN  BOOLEAN           AitDramFlag,
  OUT DIMM_INFO_STRUCT *DimmInfo,
  OUT UINT8            *MailboxStatus
  )
{
  UINT32              Status;
  UINT32              InputPayload[NUM_PAYLOAD_REG];
  UINT32              OutputPayload[NUM_PAYLOAD_REG];
  UINT8               i;
  DIMM_NVRAM_STRUCT (*DimmNvList)[MAX_DIMM];

  ZeroMem (InputPayload, NUM_PAYLOAD_REG * sizeof(UINT32));
  ZeroMem (OutputPayload, NUM_PAYLOAD_REG * sizeof (UINT32));

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "IdentifyDimm Starts\n");

  if (AitDramFlag) {
    InputPayload[0] = (UINT32) FIS_AIT_DRAM_MGC_NUMBER;            // Magic Number Byte 3:0
    InputPayload[1] = (UINT16) FIS_CURR_VERSION_FNV;               // API revision Byte 5:4

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, FIS_VERSION_DBG_MSG (FIS_CURR_VERSION_FNV));

    Host->var.mem.BiosFisRevision = FIS_CURR_VERSION_FNV;
  }

  Status = FnvMailboxInterface (Host, Socket, Ch, Dimm, InputPayload, OutputPayload, IDENTIFY_DIMM, MailboxStatus);
  if (Status == SUCCESS) {
    DimmInfo->vendorID          = (UINT16) OutputPayload[0];          // Vendor ID Byte 1:0
    DimmInfo->deviceID          = (UINT16) (OutputPayload[0] >> 16);  // Device ID Byte 3:2
    DimmInfo->revisionID        = (UINT16) OutputPayload[1];          // Revision ID Byte 5:4
    DimmInfo->ifc               = (UINT16) (OutputPayload[1] >> 16);  // Interface Format Code Byte 7:6

    DimmInfo->fwr.buildVersion  = (UINT16) (OutputPayload[2]);      // Firmware Revision Build Version Byte 9:8
    DimmInfo->fwr.hotfixVersion = (UINT8) (OutputPayload[2] >> 16); // Firmware Revision Hot Fix Version Byte 10
    DimmInfo->fwr.minorVersion  = (UINT8) (OutputPayload[2] >> 24); // Firmware Revision Minor Version Byte 11
    DimmInfo->fwr.majorVersion  = (UINT8) (OutputPayload[3]);       // Firmware Revision Major Version Byte 12

    DimmInfo->fswr              = (UINT8) (OutputPayload[3] >> 16);   // Featured SW Required Mask Byte 14
    DimmInfo->nbw               = (UINT16) OutputPayload[4];          // Number of Block Windows Byte 17:16
    DimmInfo->ombcr             = OutputPayload[7];                   // Offset of Block Mode Control Region 31:28
    DimmInfo->rc                = OutputPayload[8];                   // Raw Capacity 35:32

    for (i = 0; i < MAX_MANUFACTURER_STRLEN; i++){
      DimmInfo->mf[i] = (UINT8) (OutputPayload[9 + i /4] >> (8 * ((i) % 4)));            // Manufacturer 37:36
    }
    for (i = 0; i < MAX_SERIALNUMBER_STRLEN; i++){
      DimmInfo->sn[i] = (UINT8) (OutputPayload[9 + (i + 2)/4] >> (8 * ((i + 2) % 4)));  // Serial Number 41:38
    }
    for (i = 0; i < NGN_MAX_PARTNUMBER_STRLEN; i++){
      DimmInfo->Pn[i] = (UINT8) (OutputPayload[10 + (i + 2)/4] >> (8 * ((i + 2) % 4))); // Part Number 61:42
    }
    DimmInfo->dimmSku = (UINT8) (((OutputPayload[15] >> 16) & 0x0000f) | ((OutputPayload[16] << 4) & 0x0030)); //DimmSku 65:62
    DimmInfo->ifce    = (UINT16) (OutputPayload[16] >> 16);                               // Interface Format Code Extra
    DimmInfo->api     = (UINT16) OutputPayload[17];                                       // API Version BCD aa.bb

    for (i = 0; i < NGN_MAX_UID_STRLEN; i++) {
      DimmInfo->Uid[i] = (UINT8) (OutputPayload[17 + (i + 2) / 4] >> (8 * ((i + 2) % 4)));  // Unique ID 78:70
    }

    // Also save the VendorID & DeviceID also in the DimmNvList struct
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    (*DimmNvList)[Dimm].VendorIDIdentifyDIMM = DimmInfo->vendorID;
    (*DimmNvList)[Dimm].DeviceIDIdentifyDIMM = DimmInfo->deviceID;
  }

  return Status;
}

/*
Routine Description: GetFnvInfo will store dimm information into the fnvBuffer.
1. Get the Pointer to dimmInfoList
2. Read dimm information from the SMBus mailbox
3. If read is succesful, then transfer all information from output payload registers into the fnvBuffer
4. return status

Manufacturer, Serial Number, and Model Number are dumped into a data buffer
  @param Host           - Pointer to the system Host (root) structure
  @param socket         - Socket Number
  @param ch             - DDR Channel ID
  @param dimm           - DIMM number
  @param FnvInformation - structure containing FNVINFORMATION
  @param mbStatus       - Pointer to 8 bit status code from status register
@retval 0 SUCCESS
@retval 1 FAILURE

*/
UINT32
GetFnvInfo (
  PSYSHOST            Host,
  UINT8               socket,
  UINT8               ch,
  UINT8               dimm,
  FNVINFORMATION      FnvInformation,
  UINT8               *mbStatus
)
{
  UINT32 inputPayload [NUM_PAYLOAD_REG];
  UINT32 outputPayload [NUM_PAYLOAD_REG];
  UINT8  i;
  UINT32 status;
  status = 0;

  ZeroMem (outputPayload, NUM_PAYLOAD_REG * sizeof(UINT32));

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "GetFnvInfo Starts\n");

  status = FnvMailboxInterface (Host, socket, ch, dimm, inputPayload, outputPayload, GET_FIRMWARE_IMAGE, mbStatus);
  if (status == SUCCESS) {
    FnvInformation->fwr.buildVersion   = (UINT16) outputPayload[0];                     // Firmware Revision Build Version Byte 1:0
    FnvInformation->fwr.hotfixVersion  = (UINT8) (outputPayload[0] >> 16);              // Firmware Revision Hot Fix Version Byte 2
    FnvInformation->fwr.minorVersion   = (UINT8) (outputPayload[0] >> 24);              // Firmware Revision Minor Version Byte 3
    FnvInformation->fwr.majorVersion   = (UINT8) outputPayload[1];                      // Firmware Revision Major Version Byte 4
    FnvInformation->fwrType            = (UINT8) (outputPayload[1] >> 8);               // Firmware Type Byte 5
    FnvInformation->sfwr.buildVersion  = (UINT16) outputPayload[4];
    FnvInformation->sfwr.hotfixVersion = (UINT8) (outputPayload[4] >> 16);
    FnvInformation->sfwr.minorVersion  = (UINT8) (outputPayload[4] >> 24);
    FnvInformation->sfwr.majorVersion  = (UINT8) outputPayload[5];
    FnvInformation->sfwrType           = (UINT8) (outputPayload[5] >> 8);
    FnvInformation->Lfus               = (UINT8) (outputPayload[5] >> 16);
    for (i = 0; i < COMMIT_ID; i++) {
      FnvInformation->commitId[i] = (UINT8) (outputPayload[8 + i/4] >> ((i % 4) * 8));
    }
  }
  return status;
}


/**

Routine Description: Decodes bit mask to a string to make debug message easier to read

  @param bitmask     - Bit mask that identifies the Bit field in the Boot Status Register
  @retval string

**/
static CHAR8 *GetBSRFieldStr(UINT32 bitmask, CHAR8 *StringBuffer) {
#ifdef DEBUG_CODE_BLOCK
  switch (bitmask) {
    case FNV_MB_READY:
      AsciiStrCpyS (StringBuffer, BSR_STRING_LENGTH, "Mailbox Ready");
      break;
    case FNV_DT_DONE:
      AsciiStrCpyS (StringBuffer, BSR_STRING_LENGTH, "NVMDIMM IO Init Complete");
      break;
    case FNV_MEDIA_DONE:
      AsciiStrCpyS (StringBuffer, BSR_STRING_LENGTH, "Media Ready");
      break;
    case FNV_CREDIT_READY:
      AsciiStrCpyS (StringBuffer, BSR_STRING_LENGTH, "Credit Ready");
      break;
    case FNV_CSR_UNLOCK:
      AsciiStrCpyS (StringBuffer, BSR_STRING_LENGTH, "CSR Unlock");
      break;
    case FNV_DRAM_READY:
      AsciiStrCpyS (StringBuffer, BSR_STRING_LENGTH, "AIT DRAM Ready");
      break;
    default:
      AsciiStrCpyS (StringBuffer, BSR_STRING_LENGTH, "Unknown BIT Field");
      break;
  }
#endif  // DEBUG_CODE_BLOCK
  return StringBuffer;
}

/**

Routine Description: Decodes bit mask to a minor warning code

  @param bitmask     - Bit mask that identifies the Bit field in the Boot Status Register
  @retval minor Warning Code

**/
static UINT8
GetBSRMinorWarningCode (
                        UINT32 bitmask
)
{
  UINT8 minorWarning = 0;
  switch (bitmask) {
    case FNV_DT_DONE:
      minorWarning = WARN_DT_ERROR;
      break;
    case FNV_MEDIA_DONE:
      minorWarning = WARN_MEDIA_READY_ERROR;
      break;
    default:
      break;
  }
  return minorWarning;
}

/**

Routine Description: Returns simulation feedback for RcSim, CTE, and Simics

  @param bitmask     - Bit mask that identifies the Bit field in the Boot Status Register
  @retval feedback - Boot Status Register data

**/

/**
  @brief Determines if we need to continue polling

  @param bitmask[in] Bit mask that identifies the Bit field in the Boot Status Register
  @param data[in]    Lower 32 bits of Boot Status Register
  @param error[out]  Set to 1 if it is fatal error

  @retval status - 0 Stop Polling
  @retval status - 1 Continue Polling

**/
static UINT8
CheckforBSRError(
  IN  UINT32 bitmask,
  IN  UINT32 data,
  OUT UINT8  *error
)
{
  UINT8 status = 1;
  // This switch statement mirrors the one in EmulationPollOnBootStatusRegister. Any changes to
  // one should be reflected in the other.
  switch (bitmask) {
    case FNV_MB_READY:
      if (data == ((UINT32) -1)) {
        status = 1;
      } else if (data & FNV_MB_READY) {
        status = 0;
      } else {
        status = 1;
      }
      break;
    case FNV_DT_DONE:
      if (data & FNV_DT_DONE) {
        status = 0;
      } else {
        status = 1;
      }
      break;
    case FNV_CSR_UNLOCK:
      if (data & FNV_CSR_UNLOCK) {
        status = 1;
      } else {
        status = 0;
      }
      break;
    case FNV_MEDIA_DONE:
      //
      // If media ready or media error is signaled we are done,
      // otherwise wait until one of these bits is set.
      //
      if ((data & (FNV_MEDIA_READY | FNV_MEDIA_ERROR | FNV_MEDIA_DISABLED)) != 0) {
        status = 0;
        if ((data & (FNV_MEDIA_ERROR | FNV_MEDIA_DISABLED)) != 0) {
          *error = 1;
        }
      } else {
        status = 1;
      }
      break;
    case FNV_CREDIT_READY:
      if (data & FNV_CREDIT_READY) {
        status = 0;
      } else {
        status = 1;
      }
      break;
    case FNV_DRAM_READY:
      if ((data & FNV_DRAM_READY) == FNV_DRAM_READY) {
        status = 0;
      } else if ((data & FNV_DRAM_READY) == FNV_DRAM_ERROR) {
        status = 0;
        *error = 2;
      } else {
        status = 1;
      }
      break;
    default:
      break;
  }
  return status;
}

/**

Routine Description: Produce ubios code to poll on a specific bitfield in the Boot Status Register.

  @param Host        - Pointer to the system Host (root) structure
  @param Socket      - Socket Number
  @param Channel     - Channel Number
  @param Dimm        - DIMM Number
  @param Bitmask     - Bitmask that identifies the bitfield in the Boot Status Register
  @retval N/A

**/
VOID
EmulationPollOnBootStatusRegister (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT32   Bitmask
)
{
  // This switch statement mirrors the one in CheckforBSRError. Any changes to one should be
  // reflected in the other.
  switch (Bitmask) {
    case FNV_MB_READY:
      // The condition where data is 0xFFFFFFFF, which is due to a timeout in ReadFnvCfgSmb, is
      // intentionally not handled here, as an FMC timeout should not occur in emulation.
      EmulationReadFnvCfgSmbus (Host, Socket, Channel, Dimm, D_FW_STATUS_FNV_D_UNIT_0_REG, FNV_MB_READY, 1);
      break;
    case FNV_DT_DONE:
      EmulationReadFnvCfgSmbus (Host, Socket, Channel, Dimm, D_FW_STATUS_FNV_D_UNIT_0_REG, FNV_DT_DONE, 1);
      break;
    case FNV_CSR_UNLOCK:
      EmulationReadFnvCfgSmbus (Host, Socket, Channel, Dimm, D_FW_STATUS_FNV_D_UNIT_0_REG, FNV_CSR_UNLOCK, 0);
      break;
    case FNV_MEDIA_DONE:
      // FNV_MEDIA_ERROR and FNV_MEDIA_DISABLED are not handled here. Either result would lead to
      // an OutputWarning on silicon, which becomes a fatal error--and then a hang--with test
      // BIOSes. In this implementation, if either of those conditions are hit, a hang will occur,
      // similar to a test BIOS on silicon (but without the warning log).
      EmulationReadFnvCfgSmbus (Host, Socket, Channel, Dimm, D_FW_STATUS_FNV_D_UNIT_0_REG, FNV_MEDIA_READY, 1);
      break;
    case FNV_CREDIT_READY:
      EmulationReadFnvCfgSmbus (Host, Socket, Channel, Dimm, D_FW_STATUS_FNV_D_UNIT_0_REG, FNV_CREDIT_READY, 1);
      break;
    default:
      // No handling, same as CheckforBSRError.
      break;
  }
}

/**

Routine Description: Returns Timeout required of each bit field

  @param bitmask     - Bit mask that identifies the Bit field in the Boot Status Register
  @retval delay - Returns the delay in microseconds

**/
static UINT32
GetDelayBSRTimeout (
                    UINT32 bitmask
)
{
  UINT32 delay = 0;

  switch (bitmask) {
    case FNV_MB_READY:
      delay = FNV_MB_READY_TIMEOUT;
      break;
    case FNV_DT_DONE:
    case FNV_CSR_UNLOCK:
      delay = FNV_DDRTIO_INIT_TIMEOUT;
      break;
    case FNV_MEDIA_DONE:
      delay = FNV_MEDIA_READY_TIMEOUT;
      break;
    case FNV_CREDIT_READY:
      delay = FNV_CREDIT_READY_TIMEOUT;
      break;
    case FNV_DRAM_READY:
      delay = FNV_DRAM_READY_TIMEOUT;
      break;
    default:
      break;
  }
  return delay;
}

/**

Routine Description: Converged Polling function of the Boot Status Register

  @param Host        - Pointer to the system Host (root) structure
  @param socket      - Socket Number
  @param bitmask     - Bit mask that identifies the Bit field in the Boot Status Register
  @retval 0 - success

**/
UINT32
FnvPollingBootStatusRegisterCh(
PSYSHOST Host,
UINT8    socket,
UINT8    ch,
UINT32   bitmask
)
{
  UINT8               dimm;
  UINT64              StartCount;
  UINT32              Timeout;
  UINT8               Error;
  CHAR8                StringBuffer[BSR_STRING_LENGTH] = {0};
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  D_FW_STATUS_FNV_D_UNIT_0_STRUCT fwStatusReg;
  UINT8 OldValue = 0;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if (Host->nvram.mem.socket[socket].DcpmmPresent == 0) {
    return SUCCESS;
  }

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "FnvPollingBootStatusRegisterCh Starts: %a\n", GetBSRFieldStr(bitmask, StringBuffer));

  Timeout = GetDelayBSRTimeout (bitmask);

  dimmNvList = GetDimmNvList(Host, socket, ch);
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) {
      continue;
    }
    if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
      continue;
    }

    //
    // Wait for mailbox ready with timeout
    //
    if (!(UbiosGenerationOrHsleEnabled ())) {

      Error = 0;
      StartCount = GetCount ();

      do {
        fwStatusReg.Data = ReadFnvCfg (Host, socket, ch, dimm, D_FW_STATUS_FNV_D_UNIT_0_REG);
        // Check for TimeOut
        if (GetDelay (StartCount) > Timeout) {
          ShowBsr(Host, socket, ch, dimm);
          OutputWarning (WARN_FNV_BSR, WARN_POLLING_LOOP_TIMEOUT, socket, ch, dimm, NO_RANK);
          // Disable the channel on FW timeout
          DisableChannelSw (Host, socket, ch);
          break;
        }

      } while (CheckforBSRError (bitmask, fwStatusReg.Data, &Error));

      if (Error != 0) {

        ShowBsr(Host, socket, ch, dimm);
        if (GetBSRMinorWarningCode (bitmask) != 0) {
          EwlOutputType11 (WARN_FNV_BSR, GetBSRMinorWarningCode(bitmask), socket, ch, dimm, 0xFF, EwlSeverityWarning, fwStatusReg.Data);
        } else {
          OutputWarning (WARN_FNV_BSR, GetBSRMinorWarningCode(bitmask), socket, ch, dimm, NO_RANK);
        }
        //
        // Disable the DIMM on media failure
        //
        DisableDIMM (Host, socket, ch, dimm);
      }
    } else {
      if (Setup->common.ddrtXactor == 0) {
        OldValue = GetUbiosOutputMode ();
        SetUbiosOutputMode (ASM_OUTPUT_ENABLE_ON_SMBUS);
        EmulationPollOnBootStatusRegister (Host, socket, ch, dimm, bitmask);
        SetUbiosOutputMode (OldValue);
      }
    }
  } //dimm

  return SUCCESS;
}


/**
 Check if mailbox is ready

  @param[in] Socket      - Processor socket ID
  @param[in] Ch          - Current channel
  @param[in] Dimm        - Dimm on current channel

  @retval 0 - SUCCESS
  @retval 1 - FAILURE

**/
UINT32
CheckMailboxReady (
  UINT8  Socket,
  UINT8  Ch,
  UINT8  Dimm
  )
{
  UINT32  Status = SUCCESS;
  SYSHOST *Host;
  struct  dimmNvram(*DimmNvList)[MAX_DIMM];

  if (!(GetEmulation () & SIMICS_FLAG)) {

    Host = GetSysHostPointer();

    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    if ((*DimmNvList)[Dimm].mailboxReady == 0) {
      Status = FAILURE;
    }

  }

  return Status;

} // CheckMailboxReady


/**

Routine Description: Converged Polling function of the Boot Status Register

  @param Host        - Pointer to the system Host (root) structure
  @param socket      - Socket Number
  @param bitmask     - Bit mask that identifies the Bit field in the Boot Status Register
  @retval 0 - success

**/
UINT32
FnvPollingBootStatusRegister (
                              PSYSHOST Host,
                              UINT8    socket,
                              UINT32   bitmask
)
{
  UINT8               ch;
  UINT8               dimm;
  UINT32              timeout;
  UINT8               error;
  UINT64              StartCount;
  CHAR8                StringBuffer[BSR_STRING_LENGTH] = {0};
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];
  HOST_DDRT_DIMM_DEVICE_INFO_STRUCT    *DimmPtr;
  UINT8 OldValue = 0;
  SYS_SETUP *Setup;
  UINT8     MaxChDdr;

  Setup = GetSysSetupPointer ();

  if (Host->nvram.mem.socket[socket].DcpmmPresent == 0) {
    return SUCCESS;
  }
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "FnvPollingBootStatusRegister Starts: %a\n", GetBSRFieldStr(bitmask, StringBuffer));

  timeout = GetDelayBSRTimeout (bitmask);
  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList(Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        continue;
      }
      if (((*dimmNvList)[dimm].AitDramPoll == AIT_DRAM_POLL_DISABLE) && (bitmask == FNV_DRAM_READY)) {
        continue;
      }

      DimmPtr = &Host->var.mem.socket[socket].channelList[ch].DdrtDimmList[0];
      //
      // Wait for mailbox ready with timeout
      //
      error = 0;
      StartCount = GetCountTsc ();

      if (!(UbiosGenerationOrHsleEnabled ())) {
        do {
          DimmPtr->NgnBsr.lo = ReadFnvCfg (Host, socket, ch, dimm, D_FW_STATUS_FNV_D_UNIT_0_REG);
          DimmPtr->NgnBsr.hi = ReadFnvCfg (Host, socket, ch, dimm, D_FW_STATUS_H_FNV_D_UNIT_0_REG);

          // Check for TimeOut
          if (GetDelayTsc (StartCount) > timeout) {
            //
            // If warning to fatal promotion is enabled then we will stop in OutputWarning().
            // Print the content of BSR so that the error reason can be investigated.
            //
            ShowBsr (Host, socket, ch, dimm);
            //
            // Report timeout error so that subsequent code could have a chance to check
            // specific error status.
            //
            error = 1;
            OutputWarning (WARN_FNV_BSR, WARN_POLLING_LOOP_TIMEOUT, socket, ch, dimm, NO_RANK);
            break;
          }
        } while (CheckforBSRError (bitmask, DimmPtr->NgnBsr.lo, &error));

        if (error != 0) {
          //
          // Got some error from BSR. Decide whether channel or DIMM should be disabled,
          // or nothing in case of DIMM media not ready, because we still need to talk to the DIMM.
          //
          ShowBsr(Host, socket, ch, dimm);
          switch (bitmask) {
            case FNV_DT_DONE:
              EwlOutputType11 (WARN_FNV_BSR, WARN_DT_ERROR,
                               socket, ch, dimm, 0xFF, EwlSeverityWarning, DimmPtr->NgnBsr.lo);
            case FNV_MB_READY:
              error = 2; // Disable channel
              break;
            case FNV_CSR_UNLOCK:
            case FNV_CREDIT_READY:
              // Keep error=1 to disable just DIMM
              break;
            case FNV_MEDIA_DONE:
              EwlOutputType11 (WARN_FNV_BSR, WARN_MEDIA_READY_ERROR,
                               socket, ch, dimm, 0xFF, EwlSeverityWarning, DimmPtr->NgnBsr.lo);
              error = 0; // Don't disable, keep the DIMM available for communication and continue booting
              break;
            case FNV_DRAM_READY:
              error = 0; // Don't disable, keep the DIMM available for communication and continue
              EwlOutputType11 (WARN_FNV_BSR, WARN_DR_READY_ERROR,
                               socket, ch, dimm, 0xFF, EwlSeverityWarning, DimmPtr->NgnBsr.lo);
              break;
          }
          switch (error) {
            case 2:
              DisableChannelSw (Host, socket, ch);
              break;
            case 1:
              DisableDIMM(Host, socket, ch, dimm);
              break;
          }
        }
        if ((GetEmulation () & SIMICS_FLAG) == 0) {
          (*dimmNvList)[dimm].mailboxReady = !!(DimmPtr->NgnBsr.lo & FNV_MB_READY);
        }
      } else {
        if (Setup->common.ddrtXactor == 0){
          OldValue = GetUbiosOutputMode ();
          SetUbiosOutputMode (ASM_OUTPUT_ENABLE_ON_SMBUS);
          EmulationPollOnBootStatusRegister (Host, socket, ch, dimm, bitmask);
          SetUbiosOutputMode (OldValue);
        }
      }
    } //dimm
  } //ch

  return SUCCESS;
}

//
// Structure containing all required information for protected DCPMM register mailbox accesses
//
typedef struct {
  UINT32  BiosAddress;
  UINT32  AhbAddress;
  UINT32  WriteMask;
} PROTECTED_DCPMM_REG;

//
// Array of protected DCPMM registers
//    Column 1 is the register address, encoded for normal BIOS CSR accesses
//    Column 2 is the register address, encoded for access via protected CSR mailbox command
//      This address encoding is calculated as base address + offset
//      Base address and offset can be found in the BWV C-Spec
//    Column 3 is the write mask that must be sent when using the protected CSR mailbox command
//      This mask is defined by Crystal Ridge firmware, and it defines which bits in the
//      register BIOS is allowed to write
//
const PROTECTED_DCPMM_REG mProtectedDcpmmRegisters[] = {
  //
  // direct CSR access                          // mailbox access     // mailbox write bitmask
  //
  {DA_DDRT_FUNC_DEFEATURE1_FNV_DA_UNIT_0_REG,   0xC4000308,           BIT14},
  {D_FUNC_DEFEATURE1_FNV_D_UNIT_0_REG,          0xC8000130,           BIT21},
  {MSC_PLL_CTRL_FNV_DFX_MISC_0_REG,             0xD8000198,           (BIT12 | BIT13 | BIT31)}
};

/**
  Fetches the entry in the protected DCPMM register table corresponding to the input register

  @param[in]  Register    DCPMM DIMM register to retrieve the table entry for

  @retval NULL    Register is not in the protected DCPMM register table
  @retval !NULL   Address of the protected DCPMM register table entry for the input register
**/
const PROTECTED_DCPMM_REG*
EFIAPI
GetProtectedRegisterPtr (
  IN  UINT32  Register
  )
{
  UINT8                       RegIndex;
  const PROTECTED_DCPMM_REG   *RegisterEntry = NULL;

  for (RegIndex = 0; RegIndex < ARRAY_SIZE (mProtectedDcpmmRegisters); RegIndex++) {
    if (Register == mProtectedDcpmmRegisters[RegIndex].BiosAddress) {
      RegisterEntry = &(mProtectedDcpmmRegisters[RegIndex]);
      break;
    }
  }

  return RegisterEntry;
}


#define PROTECT_REG_ADDR    0
#define PROTECT_REG_VAL     1
#define PROTECT_REG_MASK    2

/**
  Writes to protected registers using SET_PROTECTED_REGISTER mailbox command

  @param[in]  Host        Pointer to syshost
  @param[in]  Socket      Current socket index
  @param[in]  Channel     Current channel index
  @param[in]  Dimm        Current DIMM index
  @param[in]  Register    Pointer to PROTECTED_DCPMM_REG structure details for the register being written to
  @param[in]  WriteMask   Indicates which bits CRFW should write
                            A "1" indicates the corresponding register bit will be set or cleared by CRFW according
                              to the state of the corresponding bit in Value
                            A "0" indicates the corresponding register bit will not be modified by CRFW
  @param[in]  Value       Value to write to the register

  @retval MRC_STATUS_SUCCESS    All operations completed successfully
  @retval MRC_STATUS_FAILURE    One or more operations could not be completed successfully
**/
MRC_STATUS
EFIAPI
WriteProtectedDcpmmRegister (
  IN  PSYSHOST                  Host,
  IN  UINT8                     Socket,
  IN  UINT8                     Channel,
  IN  UINT8                     Dimm,
  IN  const PROTECTED_DCPMM_REG *Register,
  IN  UINT32                    WriteMask,
  IN  UINT32                    Value
  )
{
  MRC_STATUS  Status = MRC_STATUS_FAILURE;
  UINT32      InputPayload[NUM_PAYLOAD_REG];
  UINT32      OutputPayload[NUM_PAYLOAD_REG];
  UINT8       MbStatus;
  UINT32      FnvMailboxStatus;

  RC_FATAL_ERROR ((Register != NULL), ERR_MRC_POINTER, RC_FATAL_ERROR_MRC_POINTER_MINOR_CODE_005);

  ZeroMem (&InputPayload, sizeof (InputPayload));
  ZeroMem (&OutputPayload, sizeof (OutputPayload));

  //
  // Setup payload for mailbox access
  //
  InputPayload[PROTECT_REG_ADDR] = Register->AhbAddress;
  InputPayload[PROTECT_REG_VAL]  = Value;
  InputPayload[PROTECT_REG_MASK] = WriteMask;

  FnvMailboxStatus = FnvMailboxInterface (Host, Socket, Channel, Dimm, InputPayload, OutputPayload, SET_PROTECTED_REG, &MbStatus);
  //
  // Don't need to do anything with the OutputPayload or MbStatus here
  //

  if (FnvMailboxStatus == SUCCESS) {
    Status = MRC_STATUS_SUCCESS;
  }

  return Status;
}


/**
  Used to write to registers that Crystal Ridge Firwmare (CRFW) has restricted access to

  @param[in]      Host              Pointer to syshost
  @param[in]      Socket            Current socket index
  @param[in]      Channel           Current channel index
  @param[in]      Dimm              Current DIMM index
  @param[in]      Mode              Indicates a read or a write - GSM_FORCE_WRITE and GSM_READ_ONLY are the only allowed values
                                      GSM_FORCE_WRITE and GSM_READ_ONLY may not be used at the same time
  @param[in]      Register          DCPMM DIMM register address to access
  @param[in]      DesiredWriteMask  For write accesses, indicates which bits CRFW should write
                                      A "1" indicates the corresponding register bit will be set or cleared by CRFW according
                                        to the state of the corresponding bit in Value
                                      A "0" indicates the corresponding register bit will not be modified by CRFW
  @param[in, out] Value             Pointer to buffer containing either the value to write or to place read data in

  @retval MRC_STATUS_SUCCESS    All operations completed successfully
  @retval MRC_STATUS_FAILURE    One or more operations could not be completed successfully
**/
MRC_STATUS
EFIAPI
GetSetProtectedDcpmmRegister (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      UINT8       Channel,
  IN      UINT8       Dimm,
  IN      UINT8       Mode,
  IN      UINT32      Register,
  IN      UINT32      DesiredWriteMask,
  IN OUT  UINT32      *Value
  )
{
  MRC_STATUS                  Status = MRC_STATUS_FAILURE;
  UINT32                      AllowedWriteMask;
  UINT32                      WriteValue;
  UINT32                      ReadValue;
  const PROTECTED_DCPMM_REG   *RegisterEntry = NULL;

  RC_FATAL_ERROR ((Value != NULL), ERR_MRC_POINTER, RC_FATAL_ERROR_MRC_POINTER_MINOR_CODE_003);
  RC_FATAL_ERROR (
    (GsmReadOnly (Mode) ^ GsmForceWrite (Mode)),  // READ_ONLY and FORCE_WRITE are mutually exclusive
    ERR_INVALID_REG_ACCESS,
    ERR_INVALID_ACCESS_METHOD
  );

  if (GsmReadOnly (Mode)) {
    //
    // Reads don't require protected mailbox command
    //

    *Value = ReadFnvCfg (Host, Socket, Channel, Dimm, Register);
    Status = MRC_STATUS_SUCCESS;

  } else if (GsmForceWrite (Mode)) {

    RegisterEntry = GetProtectedRegisterPtr (Register);
    RC_FATAL_ERROR ((RegisterEntry != NULL), ERR_INVALID_REG_ACCESS, ERR_INVALID_WRITE_REG_OFFSET);

    //
    // Validate desired write mask
    //
    AllowedWriteMask = RegisterEntry->WriteMask;
    if ((AllowedWriteMask | DesiredWriteMask) != AllowedWriteMask) {
      RC_FATAL_ERROR (FALSE, ERR_INVALID_REG_ACCESS, ERR_INVALID_BIT_ACCESS);
    }

    //
    // Apply write mask to write value without modifying caller data
    // This step is not strictly necessary, but is done so logging reflects values CRFW will allow to be written
    //
    WriteValue = *Value & DesiredWriteMask;

    if (Host->var.mem.socket[Socket].channelList[Channel].FisVersion[Dimm] < FIS_204_VERSION) {
      //
      // Fall back on CSR access for writes if FIS < 2.04
      //
      ReadValue = ReadFnvCfg (Host, Socket, Channel, Dimm, Register);
      WriteValue = (ReadValue & (~DesiredWriteMask)) | WriteValue;
      if (WriteFnvCfg (Host, Socket, Channel, Dimm, Register, WriteValue) == SUCCESS) {
        Status = MRC_STATUS_SUCCESS;
      }

    } else {
      //
      // FIS version is >= 2.04, use protected register mailbox command
      //

      Status = WriteProtectedDcpmmRegister (Host, Socket, Channel, Dimm, RegisterEntry, DesiredWriteMask, WriteValue);
    }

  } else {
    //
    // No caching of these registers. Only valid options are to read from or write to DIMM hardware,
    // so GSM_READ_ONLY and GSM_FORCE_WRITE are the only valid values of the mode input parameter
    //

    RC_FATAL_ERROR (FALSE, ERR_INVALID_REG_ACCESS, ERR_INVALID_ACCESS_METHOD);
  }

  return Status;
}

/**
  Routine Description: Concurrently runs NVMCTLR NVMDIMM IO init on all NVMDIMMs attached to the socket

  @param Host        Pointer to the system Host (root) structure
  @param Socket      Socket Number

  @retval 0         Success
**/
UINT32
FnvDdrtIoInit(
              PSYSHOST      Host,
              UINT8         Socket,
              UINT8         Payload
)
{
/*
    //
    // NVMCTLR NVMDIMM-IO init sequence
    //
    //   Assumptions:
    //   - Assume NVMCTLR data bus is isolated behind LR buffers, so no need to do DDR4 JEDEC init first.
    //     Need to test this assumption... e.g. has buffer been properly reset?
    //     What about IBT on CA signals?
    //   - Assume sufficient time has elapsed from NVMCTLR power-good until first SMBus transaction to SPD, RCD or NVMCTLR
    //
    //   Steps:
    //   1. Poll for NVMCTLR mailbox ready on each NVMDIMM per socket
    //   2. If NVMDIMM IO init not done, then issue mailbox command on each NVMDIMM per socket
    //   3. Poll for completion on each NVMDIMM and update command results
    //   4. Map-out any channels failing NVMCTLR NVMDIMM IO init command
    //
*/

  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                Status;
  struct dimmNvram      (*DimmNvList)[MAX_DIMM];
  struct channelNvram   (*ChannelNvList)[MAX_CH];
  UINT32                OutputPayload[NUM_PAYLOAD_REG];
  UINT32                InputPayload[NUM_PAYLOAD_REG];
  UINT32                OpCode;
  UINT8                 MbStatus = 0;
  UINT32                Org_InputPayload;
  UINT8                 OldValue = 0;
  SYS_SETUP             *Setup;
  UINT8                 TempFreq;
  CMD_CTL_OUTPUT_BUFFER CmdCtlOutputState[MAX_CH];
  UINT8                 MaxChDdr;
  BOOLEAN               Is14nmCpu;
  BOOLEAN               Is10nmWave1;

  Is14nmCpu = (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL));
  Is10nmWave1 = (IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL));

  ZeroMem (&CmdCtlOutputState, sizeof (CmdCtlOutputState));

  Setup = GetSysSetupPointer ();

  if (Host->nvram.mem.socket[Socket].DcpmmPresent == 0) {
    return SUCCESS;
  }
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "FnvPmemIoInit Starts\n");

  ChannelNvList = GetChannelNvList (Host, Socket);
  MaxChDdr      = GetMaxChDdr ();
  //
  // Tristate CMD pins for FMC types/steppings that require it
  //
  if (Payload == DDRT_FREQ) {
    for (Channel = 0; Channel < MaxChDdr; Channel++) {
      if (IsChannelEnabled (Socket, Channel) == FALSE) {
        continue;
      }
      if ((*ChannelNvList)[Channel].ddrtEnabled  == 0) {
        continue;
      }

      DimmNvList = GetDimmNvList(Host, Socket, Channel);
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
          continue;
        }
        if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
          continue;
        }

        if (Host->var.mem.socket[Socket].channelList[Channel].FisVersion[Dimm] >= FIS_202_VERSION) {
          if ((ChannelRequiresCmdTristate (Host, Socket, Channel) == TRUE) && Is10nmWave1) {
            CmdCtlOutputEnable (Host, Socket, Channel, CmdCtlOutputGet, &(CmdCtlOutputState[Channel]));
            CmdCtlOutputEnable (Host, Socket, Channel, CmdCtlOutputDisableCmd, NULL);
            break;
          }
        }
      }
    }
  }

  //
  // Issue NVMDIMM IO init command
  // Note: NVMCTLR sense amp calibration does not run on CMD/CTL signals
  //
  OpCode = DDRT_IO_INIT;
  if (Payload == PLL_LOCK){
    InputPayload[0] = BIT7;
  } else if (Payload == DDRT_FREQ){
    // Intel(R) Optane(Tm) DC Persistent Memory Module FIS Rev:1.15
    // Operating Frequency: Valid Values:
    // 0b0000 = 1600 MT/s
    // 0b0001 = 1866 MT/s
    // 0b0010 = 2133 MT/s
    // 0b0011 = 2400 MT/s
    // 0b0100 = 2600 MT/s
    // 0b0101 = 2933 MT/s
    // 0b0110 = 3200 MT/s
    TempFreq = FMC_OPFREQ_1600MTS;
    switch (Host->nvram.mem.socket[Socket].ddrtFreq) {
      case DDRT_FREQUENCY_INDEX0:
        TempFreq = FMC_OPFREQ_1600MTS;
        break;
      case DDRT_FREQUENCY_INDEX1:
        TempFreq = FMC_OPFREQ_1866MTS;
        break;
      case DDRT_FREQUENCY_INDEX2:
        TempFreq = FMC_OPFREQ_2133MTS;
        break;
      case DDRT_FREQUENCY_INDEX3:
        TempFreq = FMC_OPFREQ_2400MTS;
        break;
      case DDRT_FREQUENCY_INDEX4:
        TempFreq = FMC_OPFREQ_2666MTS;
        break;
      case DDRT_FREQUENCY_INDEX5:
        TempFreq = FMC_OPFREQ_2933MTS;
        break;
      case DDRT_FREQUENCY_INDEX6:
        TempFreq = FMC_OPFREQ_3200MTS;
        break;
      default:
        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
               "Out of Pmem Supported Frequencies index\n");
        break;
    }
    InputPayload[0] =  TempFreq;
    if (GetPreambleState (Socket, READ_PREAMBLE) == PREAMBLE_2TCLK) {
      InputPayload[0] |= BIT6;
    }
    if (GetPreambleState (Socket, WRITE_PREAMBLE) == PREAMBLE_2TCLK) {
      InputPayload[0] |= BIT5;
    }
  } else if (Payload == DDRT_TRAINING_COMPLETE) {
    InputPayload[0] = BIT8;
  } else if (Payload == DDRT_TRAINING_S3_COMPLETE) {
    InputPayload[0] = BIT8 | BIT9;
  } else if (Payload == DDRT_ENTER_NORMAL_MODE) {
    InputPayload[0] = BIT10;
  } else {
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "Invalid payload value\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL,  RC_FATAL_ERROR_MINOR_CODE_55);
    return FAILURE;
  }

  //
  // Save original InputPayload[0] value for channel loop use
  //
  Org_InputPayload = InputPayload[0];

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    DimmNvList = GetDimmNvList(Host, Socket, Channel);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }
      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        continue;
      }

      InputPayload[0] = Org_InputPayload;
      //
      // Update payload to "Training failure -channel" for channel disabled case
      //
      if (IsChannelEnabled (Socket, Channel) == FALSE) {
        InputPayload[0] = BIT9;
        (*ChannelNvList)[Channel].mb3trainingfailure = 1;
      } else {
        (*ChannelNvList)[Channel].mb3trainingfailure = 0;
      }

      Status = SUCCESS;

      //
      // Need to enable DDRT Resync 10 Clk Space(MB1 BIT16) once below conditions are satisifed
      // CLX/CPX & FIS version >= 2.02
      //
      if ((InputPayload[0] == BIT7) && (Host->var.mem.socket[Socket].channelList[Channel].FisVersion[Dimm] >= FIS_202_VERSION) &&
          Is14nmCpu) {
        InputPayload[0] |= BIT16;
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "DDRT Resync 10 Clk Space is enabled\n");
      }

      //
      // FIS 2.02 start to support 4th ddrt io init - enter normal mode(MB4)
      //
      if ((Host->var.mem.socket[Socket].channelList[Channel].FisVersion[Dimm] < FIS_202_VERSION) && (InputPayload[0] == BIT10)) {
        continue;
      }

      //
      // Need to enable CMD VOC training(MB1 BIT17) once below conditions are satisifed
      // ICX & BPS A1 & FIS version >= 2.02
      //
      if ((InputPayload[0] == BIT7) && (Host->var.mem.socket[Socket].channelList[Channel].FisVersion[Dimm] >= FIS_202_VERSION) &&
          (ChannelRequiresCmdTristate (Host, Socket, Channel) == TRUE) && Is10nmWave1) {
          InputPayload[0] |= BIT17;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
               "CMD VOC is enabled\n");
      }

      if (UbiosGenerationEnabled ()) {
        if (Setup->common.ddrtXactor == 0){
          OldValue = GetUbiosOutputMode ();
          SetUbiosOutputMode (ASM_OUTPUT_ENABLE_ON_SMBUS);
        }
      }

      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "PMem IO Init InputPayload[0] = %x\n", InputPayload[0]);

      Status = FnvMailboxInterfaceStart (Host, Socket, Channel, Dimm, InputPayload, OpCode);
      if (UbiosGenerationEnabled ()) {
        if (Setup->common.ddrtXactor == 0){
          SetUbiosOutputMode (OldValue);
        }
      }
      if (Status != SUCCESS) {
        //Disable channel
        DisableChannelSw (Host, Socket, Channel);
      }
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "PMem DIMM IO Init started\n");
    } //Dimm
  } //Channel

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    DimmNvList = GetDimmNvList(Host, Socket, Channel);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }
      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        continue;
      }

      //
      // FIS 2.02 start to support 4th ddrt io init - enter normal mode(MB4)
      //
      if ((Host->var.mem.socket[Socket].channelList[Channel].FisVersion[Dimm] < FIS_202_VERSION) && (InputPayload[0] == BIT10)) {
        continue;
      }

      if (UbiosGenerationEnabled ()) {
        if (Setup->common.ddrtXactor == 0){
          OldValue = GetUbiosOutputMode ();
          SetUbiosOutputMode (ASM_OUTPUT_ENABLE_ON_SMBUS);
        }
      }
      Status = FnvMailboxInterfaceFinish (Host, Socket, Channel, Dimm, OutputPayload, OpCode, &MbStatus);
      if (UbiosGenerationEnabled ()) {
        if (Setup->common.ddrtXactor == 0){
          SetUbiosOutputMode (OldValue);
        }
      }
      if (Status != SUCCESS) {
        //Disable channel
        DisableChannelSw (Host, Socket, Channel);
      }
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "PMem DIMM IO Init completed\n");
    } //Dimm

    if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
      //
      // Update FNV access mode
      //
      SetFmcAccessMode (Socket, Channel, EMRS);
    }

    if (UbiosGenerationOrHsleEnabled ()) {
      //
      // Update NVMCTLR access mode
      //
      SetFmcAccessMode (Socket, Channel, SMBUS);
    }

  } //Channel

  //
  // Restore CMD pin states for FMC types/steppings that required tristate
  // There is a required delay of ~5ms for DDRIO settings to be propogated
  //
  if (Payload == DDRT_FREQ) {
    FixedDelayMicroSecond (5000);
    for (Channel = 0; Channel < MaxChDdr; Channel++) {
      if (IsChannelEnabled (Socket, Channel) == FALSE) {
        continue;
      }
      if ((*ChannelNvList)[Channel].ddrtEnabled  == 0) {
        continue;
      }
      DimmNvList = GetDimmNvList(Host, Socket, Channel);
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
          continue;
        }
        if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
          continue;
        }
        if (Host->var.mem.socket[Socket].channelList[Channel].FisVersion[Dimm] >= FIS_202_VERSION) {
          if ((ChannelRequiresCmdTristate (Host, Socket, Channel) == TRUE) && Is10nmWave1) {
            CmdCtlOutputEnable (Host, Socket, Channel, CmdCtlOutputRestore, &(CmdCtlOutputState[Channel]));
            break;
          }
        }
      }
    }
  }

  return SUCCESS;
}


/**
*Updated 8/19/2014*
*NVMCTLR CIS 0.70*

Routine Description: FnvMailboxInterface is a generic read/write to the Smm mailbox.
Smm mailbox will support SMBus or CPGC reads and SMbus, EMRS, or CPGC writes

Program Flow:
1. Look if Opcode has been implemented. If not, return failure
2. Setup Timeout for the doorbell
3. Read the Command Register for the Smm Mailbox. If Doorbell is idle, then breakout of while loop.
   If timeout has expired, log a warning and return failure.
4. Write the opcode into the Command Register
5. Fill in payload registers if necessary
6. Write Security nonce. (Not implemented yet. No nonce register defined in Headerfile 10/25)
7. Set Doorbell to busy
  @param Host          - Pointer to the system Host (root) structure
  @param socket        - Socket Number
  @param ch            - DDR Channel ID
  @param dimm          - DIMM number
  @param inputPayload  - input Payload Register
  @param opcode        - NVMCTLR command (subopcode | opcode)
@retval 0 SUCCESS
@retval 1 FAILURE
**/
UINT32
FnvMailboxInterfaceStart (
                     PSYSHOST Host,
                     UINT8    socket,
                     UINT8    ch,
                     UINT8    dimm,
                     UINT32   inputPayload[NUM_PAYLOAD_REG],
                     UINT32   opcode
)
{
  MB_SMBUS_CMD_FNV_D_UNIT_0_STRUCT                 SMBusCommandRegister;
  MB_SMM_CMD_FNV_DPA_MAPPED_0_STRUCT               SmmCommandRegister;
  UINT64                                           StartCount;
  UINT8                                            opcodeIndex;
  UINT32                                           payloadCount;
  UINT8                                            i;
  UINT32                                           doorbell;
  UINT8                                            smm = 0;
  UINT64_STRUCT                                    csrData64;
  CSR_OFFSET                                       RegOffset;
  struct channelNvram                              (*channelNvList)[MAX_CH];
  SYS_SETUP                                         *Setup;

  Setup = GetSysSetupPointer ();

  channelNvList = GetChannelNvList(Host, socket);

  if (CheckMailboxReady(socket, ch, dimm)) {
    return FAILURE;
  }

  // Choose appropriate CSRs
  switch (GetFmcAccessMode (socket, ch)) {
    case SMBUS:
    case EMRS:
      smm = 0;
      break;
    case CPGC:
    case SAD:
      smm = 1;
      break;
    default:
      break;
  }

  // Find NVMCTLR Opcode
  for (i = 0; i < FNV_OPCODE_ARRAY_SIZE; i++){
    if (opcode == FnvPayloadLookupTable [i][OPCODE_INDEX]) {
      break;
    }
  }
  opcodeIndex = i;
  if (i >= FNV_OPCODE_ARRAY_SIZE || (FnvPayloadLookupTable[opcodeIndex][OPCODE_INDEX] == 0)){
    OutputWarning (WARN_INVALID_FNV_OPCODE, WARN_INVALID_FNV_SUBOPCODE, socket, ch, dimm, NO_RANK);
    DisableChannelSw (Host, socket, ch);
    return FAILURE;
  }

  // Enable Simics Call for NVMDIMM X'tor
  if ((UbiosGenerationOrHsleEnabled ()) && (Setup->common.ddrtXactor == 0)){
    if (smm) {
      EmulationReadFnvCfg (Host, socket, ch, dimm, MB_SMM_CMD_FNV_DPA_MAPPED_0_REG, BIT16, 0);
    } else {
      if (GetUbiosOutputMode () == ASM_OUTPUT_ENABLE_ON_SMBUS) {
        EmulationReadFnvCfgSmbus (Host, socket, ch, dimm, MB_SMBUS_CMD_FNV_D_UNIT_0_REG, BIT16, 0);
      }
    }
  } else {
    // Check/Wait for doorbell idle status
    StartCount = GetCount ();

    while (TRUE) {
      if (smm) {
        SmmCommandRegister.Data = ReadFnvCfg (Host, socket, ch, dimm, MB_SMM_CMD_FNV_DPA_MAPPED_0_REG);
        doorbell = SmmCommandRegister.Bits.doorbell;
      } else {
        SMBusCommandRegister.Data = ReadFnvCfg (Host, socket, ch, dimm, MB_SMBUS_CMD_FNV_D_UNIT_0_REG);
        doorbell = SMBusCommandRegister.Bits.doorbell;
      }

      // Break out of polling loop
      if (doorbell == 0) {
        break;
      }

      // Check for TimeOut
      if (GetDelay (StartCount) > DOORBELL_TIMEOUT) {
        OutputWarning (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_START_DOORBELL_TIMEOUT, socket, ch, dimm, NO_RANK);
        DisableChannelSw (Host, socket, ch);
        return FAILURE;
      }
    }
  }
  //Send Opcode to Mailbox
  if (smm) {
    SmmCommandRegister.Bits.opcode = opcode;
    SmmCommandRegister.Bits.spares = 0;
    RegOffset.Data = MB_SMM_CMD_FNV_DPA_MAPPED_0_REG;
    // RegOffset.Bits.size = 0;      //select quadword
    csrData64.lo = SmmCommandRegister.Data;
    csrData64.hi = 0;
    WriteFnvCfgDpa (Host, socket, ch, dimm, RegOffset.Data, csrData64);
  } else {
    SMBusCommandRegister.Bits.opcode = opcode;
    WriteFnvCfg (Host, socket, ch, dimm, MB_SMBUS_CMD_FNV_D_UNIT_0_REG, SMBusCommandRegister.Data);
  }

  // Fill Input Payload Registers
  payloadCount = FnvPayloadLookupTable [opcodeIndex][INPUT_PAYLOAD_INDEX];
  for (i = 0; i < payloadCount; i++){
    if (smm) {
      RegOffset.Data = MB_SMM_INPUT_PAYLOAD_0_FNV_DPA_MAPPED_0_REG + (i * 4);
      RegOffset.Bits.size = 0;      //select quadword
      csrData64.lo = inputPayload[i++];
      csrData64.hi = inputPayload[i];
      WriteFnvCfgDpa (Host, socket, ch, dimm, RegOffset.Data, csrData64);
    } else {
      WriteFnvCfg (Host, socket, ch, dimm, MB_SMBUS_INPUT_PAYLOAD_0_FNV_D_UNIT_0_REG + (i * 4) , inputPayload[i]);
    }
  }

  // Reg should be the address of the NONCE register
  // if (FnvPayloadLookupTable [opcodeIndex][RW_INDEX]){
  //   WriteFnvCfg(Host, socket, ch, dimm, CPGC, reg, fnvBuffer->securityNonce.lo);
  //   WriteFnvCfg(Host, socket, ch, dimm, CPGC, reg, fnvBuffer->securityNonce.hi);
  // }

  //Set doorbell to busy status
  if (smm) {
    SmmCommandRegister.Bits.doorbell = BUSY;
    SmmCommandRegister.Bits.spares = 0;
    RegOffset.Data = MB_SMM_CMD_FNV_DPA_MAPPED_0_REG;
    // RegOffset.Bits.size = 0;      //select quadword
    csrData64.lo = SmmCommandRegister.Data;
    csrData64.hi = 0;
    WriteFnvCfgDpa (Host, socket, ch, dimm, RegOffset.Data, csrData64);
  } else {
    SMBusCommandRegister.Bits.doorbell = BUSY;
    WriteFnvCfg (Host, socket, ch, dimm, MB_SMBUS_CMD_FNV_D_UNIT_0_REG, SMBusCommandRegister.Data);
  }

  return SUCCESS;
}

/**
*Updated 8/19/2014*
*NVMCTLR CIS 0.70*

Routine Description: FnvMailboxInterface is a generic read/write to the Smm mailbox.
Smm mailbox will support SMBus or CPGC reads and SMbus, EMRS, or CPGC writes

Program Flow:
1. Look if Opcode has been implemented. If not, return failure
2. Poll for Completion in the status register
3. Read the status code in the status register. Return if any failures occured.
4. Read output payload registers if necessary
5. Return SUCCESS
  @param Host          - Pointer to the system Host (root) structure
  @param socket        - Socket Number
  @param ch            - DDR Channel ID
  @param dimm          - DIMM number
  @param outputPayload - Output Payload Register
  @param opcode        - NVMCTLR command (subopcode | opcode)
@retval 0 SUCCESS
@retval 1 FAILURE
**/
UINT32
FnvMailboxInterfaceFinish (
                           PSYSHOST     Host,
                           UINT8        socket,
                           UINT8        ch,
                           UINT8        dimm,
                           UINT32       outputPayload[NUM_PAYLOAD_REG],
                           UINT32       opcode,
                           UINT8        *mbStatus
)
{
  MB_SMBUS_STATUS_FNV_D_UNIT_0_STRUCT              SMBusStatusRegister;
  MB_SMM_STATUS_FNV_DPA_MAPPED_0_STRUCT            SmmStatusRegister;
  MB_SMM_CMD_FNV_DPA_MAPPED_0_STRUCT               SmmCommandRegister;
  MB_SMBUS_CMD_FNV_D_UNIT_0_STRUCT                 SMBusCommandRegister;
  UINT32                                           PayloadCount;
  UINT8                                            i;
  UINT64                                           StartCount;
  UINT8                                            OpcodeIndex;
  UINT8                                            Smm = 0;
  UINT32                                           Comp;
  UINT32                                           Stat = 0;
  UINT64_STRUCT                                    CsrData64;
  UINT32                                           Doorbell;
  CSR_OFFSET                                       RegOffset;
  struct channelNvram                              (*channelNvList)[MAX_CH];
  UINT32                                           localData;
  SYS_SETUP                                        *Setup;

  Setup = GetSysSetupPointer ();

  channelNvList = GetChannelNvList(Host, socket);

  if (CheckMailboxReady(socket, ch, dimm)) {
    return FAILURE;
  }

  localData = 0;

  // Choose appropriate CSRs
  switch (GetFmcAccessMode (socket, ch)) {
    case SMBUS:
    case EMRS:
      Smm = 0;
      break;
    case CPGC:
    case SAD:
      Smm = 1;
      break;
    default:
      break;
  }

  // Find NVMCTLR Opcode
  for (i = 0; i < FNV_OPCODE_ARRAY_SIZE; i++){
    if (opcode == FnvPayloadLookupTable [i][OPCODE_INDEX]) {
      break;
    }
  }
  OpcodeIndex = i;
  if (i >= FNV_OPCODE_ARRAY_SIZE || (FnvPayloadLookupTable[OpcodeIndex][OPCODE_INDEX] == 0)){
    OutputWarning (WARN_INVALID_FNV_OPCODE, WARN_OPCODE_INDEX_LOOKUP , socket, ch, dimm, NO_RANK);
    return FAILURE;
  }

  // Enable Simics Call for NVMDIMM X'tor
  if ((UbiosGenerationOrHsleEnabled ()) && (Setup->common.ddrtXactor == 0)){
    if (Smm) {
      EmulationReadFnvCfg (Host, socket, ch, dimm, MB_SMM_STATUS_FNV_DPA_MAPPED_0_REG, BIT0, 1);
    } else {
      if (GetUbiosOutputMode () == ASM_OUTPUT_ENABLE_ON_SMBUS) {
        EmulationReadFnvCfgSmbus (Host, socket, ch, dimm, MB_SMBUS_STATUS_FNV_D_UNIT_0_REG, BIT0, 1);
      }
    }
  } else {
    // Check/Wait for doorbell idle status
    StartCount = GetCount ();

    while (TRUE) {
      if (Smm) {
        SmmCommandRegister.Data = ReadFnvCfg (Host, socket, ch, dimm, MB_SMM_CMD_FNV_DPA_MAPPED_0_REG);
        Doorbell = SmmCommandRegister.Bits.doorbell;
      } else {
        SMBusCommandRegister.Data = ReadFnvCfg (Host, socket, ch, dimm, MB_SMBUS_CMD_FNV_D_UNIT_0_REG);
        Doorbell = SMBusCommandRegister.Bits.doorbell;
      }

      // Break out of polling loop
      if (Doorbell == 0) {
        break;
      }
      // Check for TimeOut
      if (GetDelay (StartCount) > COMP_TIMEOUT) {
        OutputWarning (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FINISH_DOORBELL_TIMEOUT, socket, ch, dimm, NO_RANK);
        DisableChannelSw (Host, socket, ch);
        return FAILURE;
      }
    }

    // Read completion and status

    if (Smm) {
      SmmStatusRegister.Data = ReadFnvCfg (Host, socket, ch, dimm, MB_SMM_STATUS_FNV_DPA_MAPPED_0_REG);
      Comp = SmmStatusRegister.Bits.comp;
      Stat = SmmStatusRegister.Bits.stat;
      *mbStatus = (UINT8) Stat;
    } else {
      SMBusStatusRegister.Data = ReadFnvCfg (Host, socket, ch, dimm, MB_SMBUS_STATUS_FNV_D_UNIT_0_REG);
      Comp = SMBusStatusRegister.Bits.comp;
      Stat = SMBusStatusRegister.Bits.stat;
      *mbStatus = (UINT8) Stat;
    }


    if (Comp == 0) {
      OutputWarning (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FINISH_COMPLETE_TIMEOUT, socket, ch, dimm, NO_RANK);
      DisableChannelSw (Host, socket, ch);
      return FAILURE;
    }
  }


  //
  // check only on Hardware(added to support CLV team, as they run on simics and check for media disabled tests)
  //
  if (!(GetEmulation () & SIMICS_FLAG)) {
    if (Stat == MB_STAT_MEDIA_DISABLED) {
      //
      // don't process EWL (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_MAILBOX_FAILED) but return failure as the
      // Set/Get partition info should not process commands when media disabled.
      //
      return FAILURE;
    }
  }
  // Read Status; do not check for media disabled error code
  if (StatusDecoder (Host, socket, ch, dimm, Stat, opcode) == FAILURE) {

    ReadBootStatusRegisters (Host, socket, ch, dimm,
                             (BOOT_STATUS_REGISTER*)&Host->var.mem.socket[socket].channelList[ch].DdrtDimmList[0].NgnBsr.lo,
                             (BOOT_STATUS_HIGH_REGISTER*)&Host->var.mem.socket[socket].channelList[ch].DdrtDimmList[0].NgnBsr.hi);
    RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "BSR 0x%X%08X\n",
                    Host->var.mem.socket[socket].channelList[ch].DdrtDimmList[0].NgnBsr.hi,
                    Host->var.mem.socket[socket].channelList[ch].DdrtDimmList[0].NgnBsr.lo);

    return FAILURE;
  }
  // Read Payload Registers
  PayloadCount = FnvPayloadLookupTable [OpcodeIndex][OUTPUT_PAYLOAD_INDEX];

  for (i = 0; i < PayloadCount; i++){
    if (Smm) {
      RegOffset.Data = MB_SMM_OUTPUT_PAYLOAD_0_FNV_DPA_MAPPED_0_REG + (i * 4);
      RegOffset.Bits.size = 0;      //select quadword
      ReadFnvCfgDpa (Host, socket, ch, dimm, RegOffset.Data, &CsrData64);
      outputPayload [i++] = CsrData64.lo;
      outputPayload [i] = CsrData64.hi;
    } else {
      outputPayload [i] = ReadFnvCfg (Host, socket, ch, dimm, MB_SMBUS_OUTPUT_PAYLOAD_0_FNV_D_UNIT_0_REG + (i * 4));
    }
  }

  return SUCCESS;
}

/**
*Updated 8/19/2014*
*NVMCTLR CIS 0.70*

Routine Description: FnvMailboxInterface is a generic read/write to the BIOS/SMM mailbox. BIOS mailbox will always use CPGC interface
Program Flow:

  @param Host          - Pointer to the system host (root) structure
  @param Socket        - Socket Number
  @param Ch            - DDR Channel ID
  @param Dimm          - DIMM number
  @param InputPayload  - input Payload Register
  @param OutputPayload - Output Payload Register
  @param Opcode        - NVMCTLR command (subopcode | opcode)
@retval 0 SUCCESS
@retval 1 FAILURE
**/
UINT32
FnvMailboxInterface (
  SYSHOST   *Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT32    InputPayload[NUM_PAYLOAD_REG],
  UINT32    OutputPayload[NUM_PAYLOAD_REG],
  UINT32    Opcode,
  UINT8     *MbStatus
  )
{
  UINT32    Status;
  UINT16    Command;
  UINT16    PercentComplete;
  UINT32    Etc;
  UINT8     StatusCode;
  UINT8     MbStatus2;
#ifdef DEBUG_CODE_BLOCK
  UINT32    TimeInSecond = 0;

  Status = GetSystemTime (&TimeInSecond);
  if (!TimeInSecond) {
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "Getting system time failed - use default time\n");

    Status = GetTimestampForRtcFailure (&TimeInSecond);
    if (FAILURE == Status) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "Getting default time failed\n");
    }
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "NVMCTL Sending command. Timestamp = %u Opcode = %x\n", TimeInSecond, Opcode);
#endif // DEBUG_CODE_BLOCK

  Status = FnvMailboxInterfaceStart(Host, Socket, Ch, Dimm, InputPayload, Opcode);
  if (Status == SUCCESS) {
    Status = FnvMailboxInterfaceFinish(Host, Socket, Ch, Dimm, OutputPayload, Opcode, MbStatus);
    if ((Status == FAILURE) && (*MbStatus == MB_STAT_DEVICE_BUSY)) {
      // Possible long operation in progress
      // Need to check long operation status and re-execute command if necessary
      if (CheckLongOperationStatus(Host, Socket, Ch, Dimm, &Command, &PercentComplete, &Etc, &StatusCode, &MbStatus2) == SUCCESS) {
        // Check whether long operation is complete
        // PercentComplete is in BCD
        while ((PercentComplete < 0x100) && (StatusCode == MB_STAT_DEVICE_BUSY)) {
          // Long operation ongoing
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "NVMCTL Long Operation in progress. %x %% Complete\n", PercentComplete);
          // Need to delay based on Etc then recheck
          FixedDelayMicroSecond (Etc * 1000 * 1000); // Etc is in seconds

          CheckLongOperationStatus(Host, Socket, Ch, Dimm, &Command, &PercentComplete, &Etc, &StatusCode, &MbStatus2);
        }

        // Command finished.  Resend previous command
        Status = FnvMailboxInterface(Host, Socket, Ch, Dimm, InputPayload, OutputPayload, Opcode, MbStatus);
      }
    }
  }
  return Status;
}

/**

Routine Description: Status Decoder will print status message after issuing a NVMCTLR Command


  @param[in] Host           - Pointer to the system Host (root) structure
  @param[in] Socket         - Socket Number
  @param[in] Ch             - DDR Channel ID
  @param[in] Dimm           - DIMM number
  @param[in] MailboxStatus  - 8 bit status code from status register

  @retval 0                 - Success
  @retval 1                 - Failure

**/
UINT32
StatusDecoder (
  IN SYSHOST *Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT32   MailboxStatus,
  IN UINT32   CmdOpcode
  )
{
  UINT32 Status = FAILURE;

  switch(MailboxStatus) {
  case MB_STAT_SUCCESS:
    Status = SUCCESS;
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox SUCCESS\n");
    break;
  case MB_STAT_INVALID_COMMAND_PARAMETER:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure Invalid Command Parameter\n");
    break;
  case MB_STAT_DATA_TRANSFER_ERROR:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure Data Transfer Error\n");
    break;
  case MB_STAT_INTERNAL_DEVICE_ERROR:
    if (CmdOpcode == GET_LONG_OPERATION) {
      //
      // Up to FIS 1.4 InternalError is returned if no long op was stared.
      // But if we request Long Op Status while no such operation was started it is wrong flow.
      // Let's keep it reported as error.
      //
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "PMem Mailbox: No Long Operation Started Yet\n");

    } else {

      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "PMem Mailbox Failure Internal Device Error\n");
    }
    break;
  case MB_STAT_UNSUPPORTED_COMMAND:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure Unsupported Command\n");
    break;
  case MB_STAT_DEVICE_BUSY:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure Device Busy\n");
    break;
  case MB_STAT_INCORRECT_PASSPHRASE_OR_SECURITY_NONCE:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure Incorrect Passphrase/Security Nonce\n");
    break;
  case MB_STAT_FW_AUTHENTICATION_FAILED:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure Security Check Fail\n");
    break;
  case MB_STAT_INVALID_SECURITY_STATE:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure Invalid Security State\n");
    break;
  case MB_STAT_SYSTEM_TIME_NOT_SET:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure System Time Not Set\n");
    break;
  case MB_STAT_DATA_NOT_SET:
    if (CmdOpcode == GET_LONG_OPERATION) {
      //
      // Since FIS 1.5 DataNotSet is returned if no long op was stared.
      // But if we request Long Op Status while no such operation was started it is wrong flow.
      // Let's keep it reported as error.
      //
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "PMem Mailbox: No Long Operation Started Yet\n");

    } else {

      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "PMem Mailbox Failure Data Not Set\n");
    }
    break;
  case MB_STAT_ABORTED:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure Aborted\n");
    break;
  case MB_STAT_NO_NEW_FW_TO_EXECUTE:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure No New FW to Execute\n");
    break;
  case MB_STAT_REVISION_FAILURE:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Revision Failure\n");
    break;
  case MB_STAT_INJECTION_NOT_ENABLED:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure Injection Not Enabled\n");
    break;
  case MB_STAT_CONFIG_LOCKED:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure Config Locked Command Invalid\n");
    break;
  case MB_STAT_INVALID_ALIGNMENT:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure Invalid Alignment\n");
    break;
  case MB_STAT_INCOMPATIBLE_DIMM_TYPE:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure Incompatible DIMM Type\n");
    break;
  case MB_STAT_TIMEOUT_OCCURRED:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure Timeout Occurred\n");
    break;
  case MB_STAT_INVALID_COMMAND_VERSION:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure Invalid Commmand Version\n");
    break;
  case MB_STAT_MEDIA_DISABLED:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure Media Disabled\n");
    break;
  case MB_STAT_FW_UPDATE_ALREADY_OCCURRED:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure FW Update Already Occurred\n");
    break;
  case MB_STAT_NO_RESOURCES_AVAILABLE:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure No Resources Available\n");
    break;
  default:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem Mailbox Failure No Status Message: 0x%x\n", MailboxStatus);
  }

  return Status;
}

/**

Routine Description: CheckLongOperationStatus Gets the long operation status

  @param Host             - Pointer to the system host (root) structure
  @param Socket           - Socket Number
  @param Ch               - DDR Channel ID
  @param Dimm             - DIMM Number
  @param Command          - Opcode and SubOpcode of the long operation
  @param PercentComplete  - Percentage of the command that has been completed
  @param Etc              - Estimated time to completion
  @param StatusCode       - Mailbox Status code of the long operation
  @param MbStatus         - Pointer to 8 bit status code from status register
  @retval 0 SUCCESS
  @retval 1 FAILURE

**/

UINT32
CheckLongOperationStatus (
  SYSHOST   *Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT16    *Command,
  UINT16    *PercentComplete,
  UINT32    *Etc,
  UINT8     *StatusCode,
  UINT8     *MbStatus
  )
{
  UINT32  InputPayload[NUM_PAYLOAD_REG];
  UINT32  OutputPayload[NUM_PAYLOAD_REG];
  UINT32  Status = SUCCESS;

  ZeroMem (InputPayload, NUM_PAYLOAD_REG * sizeof (UINT32));
  ZeroMem (OutputPayload, NUM_PAYLOAD_REG * sizeof (UINT32));

  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Calling Check Long Operation Status for dimm %d on channel %d\n", Dimm, Ch);

  Status = FnvMailboxInterface(Host, Socket, Ch, Dimm, InputPayload, OutputPayload, GET_LONG_OPERATION, MbStatus);
  if (Status == SUCCESS) {
    *Command = (UINT16)(OutputPayload[0]);
    *PercentComplete  = (UINT16)(OutputPayload[0] >> 16);
    *Etc = OutputPayload[1];
    *StatusCode = (UINT8)OutputPayload[2];
  }

  return Status;
}

/**
*Updated 8/19/2014*
*NVMCTL CIS 0.70*

Routine Description: SetDieSparingPolicy Sets policy for NGN die sparing.

  @param Host           - Pointer to the system Host (root) structure
  @param socket         - Socket Number
  @param ch             - DDR Channel ID
  @param dimm           - DIMM Number
  @param Enable         - Enable Die sparing
  @param mbStatus       - Pointer to 8 bit status code from status register
  @retval 0 SUCCESS
  @retval 1 FAILURE

**/

UINT32
SetDieSparingPolicy (
                     PSYSHOST       Host,
                     UINT8          socket,
                     UINT8          ch,
                     UINT8          dimm,
                     UINT8          Enable,
                     UINT8          *mbStatus
)
{

  UINT32 inputPayload[NUM_PAYLOAD_REG];
  UINT32 outputPayload[NUM_PAYLOAD_REG];
  UINT32 status = SUCCESS;

  ZeroMem (inputPayload, NUM_PAYLOAD_REG * sizeof(UINT32));
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Setting Die Sparing Policy for dimm %d on channel %d\n", dimm, ch);
  inputPayload[0] = Enable;
  status = FnvMailboxInterface(Host, socket, ch, dimm, inputPayload, outputPayload, SET_DIE_SPARING, mbStatus);

  return status;
}

/**
*Updated 8/19/2014*
*NVMCTLR CIS 0.70*

Routine Description: GetDieSparingPolicy Gets status of NGN die sparing.

  @param Host           - Pointer to the system Host (root) structure
  @param socket         - Socket Number
  @param ch             - DDR Channel ID
  @param dimm           - DIMM Number
  @param Enable         - Enable Die sparing
  @param mbStatus     - Pointer to 8 bit status code from status register

  @retval 0 SUCCESS
  @retval 1 FAILURE

**/

UINT32
GetDieSparingPolicy (
                     PSYSHOST       Host,
                     UINT8          socket,
                     UINT8          ch,
                     UINT8          dimm,
                     UINT8          *Enable,
                     UINT8          *supported,
                     UINT8          *mbStatus
)
{

  UINT32 inputPayload[NUM_PAYLOAD_REG];
  UINT32 outputPayload[NUM_PAYLOAD_REG];
  UINT32 status;

  ZeroMem (outputPayload, NUM_PAYLOAD_REG * sizeof (UINT32));

  status = FnvMailboxInterface(Host, socket, ch, dimm, inputPayload, outputPayload, GET_DIE_SPARING, mbStatus);
  if (status == SUCCESS){
    *Enable               = (UINT8)(outputPayload[0]);
    *supported = (UINT8)(outputPayload[0] >> 16);
  }
  return status;
}

/**
  Reads the FW Boot Status Registers.

  @param[in]  Host                    - Pointer to System Host (root) structure
  @param[in]  Socket                  - Socket Number
  @param[in]  Ch                      - DDR Channel ID
  @param[in]  Dimm                    - Dimm number of the channel
  @param[out] BootStatusRegister      - Pointer to value of Boot Status Register
  @param[out] BootStatusHighRegister  - Pointer to value of Boot Status High Register

  @retval                             - VOID
**/
VOID
ReadBootStatusRegisters (
  IN  SYSHOST                   *Host,
  IN  UINT8                      Socket,
  IN  UINT8                      Ch,
  IN  UINT8                      Dimm,
  OUT BOOT_STATUS_REGISTER      *BootStatusRegister,
  OUT BOOT_STATUS_HIGH_REGISTER *BootStatusHighRegister
  )
{
  BootStatusRegister->Data     = ReadFnvCfg (Host, Socket, Ch, Dimm, D_FW_STATUS_FNV_D_UNIT_0_REG);
  BootStatusHighRegister->Data = ReadFnvCfg (Host, Socket, Ch, Dimm, D_FW_STATUS_H_FNV_D_UNIT_0_REG);
}

/**
  This function reads the FW Feature Status Register.

  @param[in]  Host                  Pointer to the sysHost structure.
  @param[in]  Socket                Socket index.
  @param[in]  Ch                    Channel index on socket.
  @param[in]  Dimm                  Dimm index on channel.
  @param[out] FeatureStatusReg      Pointer to the value of Feature Status Register.

  @retval EFI_SUCCESS               Read the Feature Status Reister successfully.
  @retval EFI_INVALID_PARAMETER     Some of the input parameters are invalid.
**/
EFI_STATUS
EFIAPI
ReadFeatureStatusRegister (
  IN  SYSHOST                 *Host,
  IN  UINT8                   Socket,
  IN  UINT8                   Ch,
  IN  UINT8                   Dimm,
  OUT FEATURE_STATUS_REGISTER *FeatureStatusReg
  )
{
  //
  // Stub function for API consistency.
  //
  if (FeatureStatusReg == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  FeatureStatusReg->Data.lo = 0;
  FeatureStatusReg->Data.hi = 0;
  return EFI_SUCCESS;
}

/**
  Displays the contents of the FW Boot Status Registers.

  @param[in] Host   - Pointer to System Host (root) structure
  @param[in] Socket - Socket Number
  @param[in] Ch     - DDR Channel ID
  @param[in] Dimm   - Dimm number of the channel

  @retval VOID      - None
**/
VOID
ShowBsr (
  IN SYSHOST *Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm
  )
{
  BOOT_STATUS_REGISTER      Bsr;
  BOOT_STATUS_HIGH_REGISTER BsrHi;

  ReadBootStatusRegisters (Host, Socket, Ch, Dimm, &Bsr, &BsrHi);

#ifdef DEBUG_CODE_BLOCK
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "FW_STATUS / Boot Status Register(BSR):\n");
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "============================================\n");
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "FW_STATUS   = 0x%08X\n", Bsr.Data);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "FW_STATUS_H = 0x%08X\n", BsrHi.Data);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "============================================\n");
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [07:00] MajorCheckpoint ---------- = 0x%02X\n", Bsr.Bits.MajorCheckpoint);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [15:08] MinorCheckpoint ---------- = 0x%02X\n", Bsr.Bits.MinorCheckpoint);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [17:16] MR (Media Ready) --------- = 0x%1X (0:Not Ready, 1:Ready, 2:Error, 3:Rsvd)\n", Bsr.Bits.MediaReady);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [18:18] DT (PMem IO Init Cmpl) --- = 0x%1X (0:Not Ready, 1:Ready)\n", Bsr.Bits.DdrtIoInitComplete);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [19:19] PCR (PCR Access Locked) -- = 0x%1X (0:Unlocked, 1:Locked)\n", Bsr.Bits.PcrAccessLocked);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [20:20] MBR (Mailbox Ready) ------ = 0x%1X (0:Not Ready, 1:Ready)\n", Bsr.Bits.MailboxInterfaceReady);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [21:21] WTS ---------------------- = 0x%1X (0:No Change, 1:WDT NMI Generated)\n", Bsr.Bits.WatchdogTimerStatus);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [22:22] FRCF --------------------- = 0x%1X (0:No Change, 1:First Refresh Cycle Completed)\n", Bsr.Bits.FirstRefreshCompletedFlag);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [23:23] CR (Credit Ready) -------- = 0x%1X (0:WDB Not Flushed, 1:WDB Flushed)\n", Bsr.Bits.CreditReady);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [24:24] MD (Media Disabled) ------ = 0x%1X (0:Media Normal, 1:Media Disabled)\n", Bsr.Bits.MediaDisabled);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [25:25] SVNDE (SVN Downgrade Downgrade Enable)----------------= 0x%1X (0:Not Enabled, 1:Enabled)\n", Bsr.Bits.SvnDowngradeOptInEnable);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [26:26] SVNCOIS (SVN Downgrade Capability Opt-In Status) ---- = 0x%1X (0:Never Enabled, 1:Has Been Enabled)\n", Bsr.Bits.SvnDowngradeOptInWasEnabled);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [28:27] DR (Dram Ready (AIT)) ---- = 0x%02X (0:Not Trained, 1:Not Loaded, 2:Error, 3:Loaded)\n", Bsr.Bits.DramReadyAit);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [29:29] RR (Reboot Required) ------ = 0x%1X (0:No Reset needed, 1:Required platform power cycle)\n", Bsr.Bits.RebootRequired);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [30:30] LFOPB (Link Failure On Previous Boot) ------ = 0x%1X (0:No Error, 1:Fatal Link Error)\n", Bsr.Bits.LinkFailureOnPreviousBoot);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [31:31] Reserved\n");
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [32:32] Assertion ---------------- = 0x%1X (1:FW Hit Assert - debug only)\n", BsrHi.Bits.Assertion);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [33:33] MI_Stalled --------------- = 0x%1X (1:Media Interface Stalled - debug only)\n", BsrHi.Bits.MediaInterfaceStalled);
  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "  [63:34] Reserved\n\n");
#endif //DEBUG_CODE_BLOCK
}


/**

  Determine if disabled Nvmdimm got recovered by DFX format from previous boot

  @param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket      - Processor socket within the system (0-based)

  @retval TRUE  - NVMDIMM is recovered
          FALSE - NVMDIMM is not recovered

**/
BOOLEAN
DetectNvmdimmDisableChange (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  UINT8                     Ch;
  UINT8                     Dimm;
  struct dimmNvram          (*DimmNvList)[MAX_DIMM];
  struct channelNvram       (*ChannelNvList)[MAX_CH];
  BOOT_STATUS_REGISTER      Bsr;
  BOOT_STATUS_HIGH_REGISTER BsrHi;
  UINT8                     MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList(Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    DimmNvList = GetDimmNvList(Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) continue;
      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) continue;

      if (Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].NvmDimmDisable == 1) {

        ReadBootStatusRegisters (Host, Socket, Ch, Dimm, &Bsr, &BsrHi);

        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "Read Media Disable BSR for disabled NVMDimm, Bsr=0x%x\n", Bsr);

        if (Bsr.Bits.MediaDisabled == 0) {
          Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].NvmDimmDisable = 0;

          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "Media is enabled, re-train Dimm\n");

          return TRUE;
        }
      }
    }
  }
  return FALSE;
}
/**
  Sets performance knob value based on knob and CR modes to FNV/EKV FW

  This routine issues the Set Features / Performance Knob command and
  sets the data from the provided buffer.

  @param[in]  Host                  - Pointer to System Host (root) structure
  @param[in]  Socket                - Socket Number
  @param[in]  Ch                    - DDR Channel ID
  @param[in]  Dimm                  - Dimm number of the channel
  @param[in]  PerformanceKnob       - Pointer to structure containing DIMM performance knob information
  @param[out] MailboxStatus         - Pointer to 8 bit status code from status register

  @retval 0                         - SUCCESS
  @retval 1                         - FAILURE
**/
UINT32
SetPerformanceKnob (
  IN  SYSHOST                 *Host,
  IN  UINT8                    Socket,
  IN  UINT8                    Ch,
  IN  UINT8                    Dimm,
  IN  PERFORMANCE_KNOB        *PerformanceKnob,
  OUT UINT8                   *MailboxStatus
  )
{
  UINT32            InputPayload[NUM_PAYLOAD_REG];
  UINT32            OutputPayload[NUM_PAYLOAD_REG];
  UINT32            Status;

  ZeroMem (InputPayload, NUM_PAYLOAD_REG * sizeof (UINT32));

#ifdef PRINT_FUNC
  RcDebugPrintWithDevice(SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Set Performance Knob Starts\n");
#endif

  // Set the knob and value
  InputPayload[0] = (((UINT32)PerformanceKnob->PerfKnobValue << 8) | (PerformanceKnob->PerfKnobId));

#ifdef PRINT_FUNC
  RcDebugPrintWithDevice(SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "InputPayload[0]=%x\n" ,InputPayload[0]);
#endif

  // This is put in for debug purpose, pls do not change/delete.
  RcDebugPrintWithDevice(SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "InputPayload[0]=%x\n" ,InputPayload[0]);
  Status = FnvMailboxInterface (Host, Socket, Ch, Dimm, InputPayload, OutputPayload, SET_PERFORMANCE_KNOB, MailboxStatus);
  if (Status == SUCCESS) {
    RcDebugPrintWithDevice(SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Set Performance Knob value is set via MB\n");
  }

#ifdef PRINT_FUNC
  RcDebugPrintWithDevice(SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Set Performance Knob Ends\n");
#endif

  return Status;
}
