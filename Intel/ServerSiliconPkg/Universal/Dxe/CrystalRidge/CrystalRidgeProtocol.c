/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2020 Intel Corporation. <BR>

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

#include <IndustryStandard/SmBios.h>
#include <Chip/Include/SysHostChip.h>
#include <Protocol/PcatProtocol.h>
#include <Library/MemFmcIpLib.h>
#include <Protocol/AcpiPlatformProtocol.h>
#include <Protocol/Dcpmm.h>
#include <Library/MemTypeLib.h>

#include <Library/MemDecodeLib.h>

#include <Library/PmcLib.h>

#include "CrystalRidge.h"
#include <Protocol/SmbiosMemInfo.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/Mesh2MemIpLib.h>

#include "RdRand.h"
#include "Dsm.h"
#include "Fis.h"
#include "Nfit.h"
#include "Pcat.h"
#include "XlateFunctions/XlateFunctions.h"
#include "Ars/ArsFlows.h"
#include "Ars/ArsErrorInject.h"
#include <UncoreCommonIncludes.h>
#include <Library/KtiApi.h>
#include <Library/MemMapDataLib.h>
#include <Library/TimerLib.h>
#include <Chip/Include/FnvAccessCommon.h>
#include <Library/PchRtcLib.h>
#include <Protocol/SmmCpu.h>
#include "NvdimmAcpiConfig.h"
#include "FlushCacheLineOpt.h"
#include <Register/Cpuid.h>
#include <Library/ProcSmbIpLib.h>
#include "NvdimmSmbusAccess.h"
#include "AcpiNotify.h"

//
// For backward compatibility we support latched and unlatched LSS interpretation
// functions in Crystal Ridge protocol, but these functions are moved to
// DCPMM Protocol.
//
extern EFI_STATUS EFIAPI
DcpmmGetPlatformInterpretedLss (
  IN  UINT32                      NfitHandle,
  OUT NVM_INTERPRETED_LSS        *InterpretedLssPtr,
  OUT NVM_INTERPRETED_LSS_REASON *InterpretedLssReasonPtr
  );


/*****************************************************************************
 * Definitions.
 *****************************************************************************/
#define GET_ARS_POLLING_INTERVAL_US 100
#define GET_ARS_MAX_ITER 10

/*****************************************************************************
 * Global variables
 *****************************************************************************/
STATIC EFI_CRYSTAL_RIDGE_PROTOCOL mCrystalRidgeProtocol;
extern EFI_NVDIMM_SMBUS_SMM_INTERFACE_PROTOCOL  *mNvdimmSmbusSmmInterfaceProtocol;
extern NVDIMM_SMBUS_SMM_INTERFACE               *mNvdimmSmbusSmmInterface;

/*****************************************************************************
 * Functions
 *****************************************************************************/
/**
  Routine Description: This function converts given Dpa
  to Spa and also populates the socket channel and dimm information with
  only system phy address as the input.
  CR protocol function.

  @param[in]  Spa        - System Physical address SPA to be translated
  @param[out] *Skt       - socket number of the given SPA
  @param[out] *Ch        - channel number of the given SPA
  @param[out] *Dimm      - dimm corresponding to the given SPA
  @param[out] *Dpa       - Device physical address

  @return EFI_STATUS - status of the conversion effort
**/
EFI_STATUS
EFIAPI
SpaToNvmDpa (
  UINT64       Spa,
  UINT8        *Skt,
  UINT8        *Ch,
  UINT8        *Dimm,
  UINT64       *Dpa
  )
{
  UINT64        SpaStart;
  EFI_STATUS    Status = EFI_NOT_FOUND;
  NVDIMM        *NvmDimm = NULL;

  if (!IsSystemWithDcpmm ()) {
    return Status;
  }

  Status = GetDimmInfoFromSpa (Spa, &SpaStart, Skt, Ch, Dimm, NULL);
  if (Status == EFI_SUCCESS) {
    NvmDimm = GetDimm (&mCrInfo.NvdimmInfo, *Skt, *Ch, *Dimm, NVDIMM_FLAG_ENABLED);
  }

  if (NvmDimm == NULL) {
    Status = EFI_NOT_FOUND;
    return Status;
  }

  Status = XlateSpaToDpaOffset (NvmDimm, SpaStart, Spa, Dpa);
  if (IsPmemRgn (&mCrInfo.NvdimmInfo, SpaStart)) {
    *Dpa += GetPmemDpaBase (SpaStart, *Skt, *Ch, *Dimm);
  }
  return Status;
}


/**
  CR interface function, see documentation in Protocol/CrystalRidge.h
 **/
EFI_STATUS
EFIAPI
NvmCtlrReadCfgMem (
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT32    reg,
  UINT32    *data
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  NVDIMM          *pDimm = NULL;
  INT16           Index;
  UINT64          RDpa;

  pDimm = GetDimm (&mCrInfo.NvdimmInfo, socket, ch, dimm, NVDIMM_FLAG_ENABLED);

  if (pDimm == NULL) {
    return (EFI_INVALID_PARAMETER);
  }

  Index = GetRegIndex ((UINT64) reg);
  if (Index == -1) {
    Index = GetSPARegIndex (pDimm, (UINT64) reg);
  }

  if (Index != -1) {
    Status = GetDpaFromRegIndex (pDimm, Index, &RDpa);
  } else {
    Status = XlateSpaToDpaOffset (pDimm, pDimm->SADSpaBase, (UINT64) reg, &RDpa);
  }

  if (Status != EFI_SUCCESS) {
    return (EFI_INVALID_PARAMETER);
  }
  //
  // Now that we have DPA for this register, go and read it.
  //
  *data = *(volatile UINT32 *)(UINTN)(RDpa);
  return Status;
}


/**
 CR interface function, see documentation in Protocol/CrystalRidge.h
 **/
EFI_STATUS
EFIAPI
NvmCtlrWriteCfgMem (
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT32    reg,
  UINT32    data
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  NVDIMM          *pDimm = NULL;
  INT16           Index;
  UINT64          RDpa;

  pDimm = GetDimm (&mCrInfo.NvdimmInfo, socket, ch, dimm, NVDIMM_FLAG_ENABLED);

  if (pDimm == NULL) {
    return (EFI_INVALID_PARAMETER);
  }

  //
  //
  Index = GetRegIndex ((UINT64) reg);
  if (Index == -1) {
    Index = GetSPARegIndex (pDimm, (UINT64) reg);
  }

  if (Index != -1) {
    Status = GetDpaFromRegIndex (pDimm, Index, &RDpa);
  } else {
    Status = XlateSpaToDpaOffset (pDimm, pDimm->SADSpaBase, (UINT64) reg, &RDpa);
  }

  if (Status != EFI_SUCCESS) {
    return (EFI_INVALID_PARAMETER);
  }
  //
  // Now that we have Dpa for this register, go and write the data to it.
  *(volatile UINT32 *)(UINTN)(RDpa) = data;
  return Status;
}


/**
  @brief This function sets command access in DCPMM FW.

  @param[in]  Socket    - Socket Number
  @param[in]  Ch        - DDR Channel ID
  @param[in]  Dimm      - DIMM Number
  @param[in]  Command   - Command affected (subopcode | opcode).
  @param[in]  SmmOnly   - True (i.e. 1) if the command should be restricted to SMM mailbox only.

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
NvmCtlrSetCommandAccessPolicy (
  IN UINT8   Skt,
  IN UINT8   Ch,
  IN UINT8   Dimm,
  IN UINT16  Command,
  IN UINT8   SmmOnly
  )
{
  EFI_STATUS Status;
  FIS_SET_CMD_ACCESS_POLICY_REQ FisReq;

  ZeroMem (&FisReq, sizeof (FisReq));
  FisReq.Bits.Command = Command;
  FisReq.Bits.SmmOnly = SmmOnly;

  Status = FisRequest (Skt, Ch, Dimm, &FisReq.DWord[0], sizeof (FisReq), NULL, SET_COMMAND_ACCESS_POLICY, FIS_DEFTIMEOUT, NULL);
  if (!EFI_ERROR (Status)) {

    DsmUpdateFisCmdAccessPolicy(Skt, Ch, Command, SmmOnly & 1);

  } else {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("CRP") "S%d.C%d.D%d: Setting Command Access Policy for command 0x%04X failed (%r)\n",
            Skt, Ch, Dimm, Command, Status));
  }
  return Status;
}


/**
  Routine Description: This function gets latest error logs form NVMCTLR.
  FIS 1.3 compatible.

  @param[in]  Socket    - Socket Number
  @param[in]  Ch        - DDR Channel ID
  @param[in]  Dimm      - DIMM Number
  @param[in]  LogLevel  - Log Level: 0 - Low Priority, 1 - High Priority
  @param[in]  LogType   - Log Type: 0 - Media, 1 - Thermal
  @param[in]  Count     - Max number of log entries requested
  @param[out] ErrLog    - Pointer to data buffer

  @return EFI_STATUS    - Status of the Command Sent
**/
EFI_STATUS
EFIAPI
NvmCtlrGetLatestErrorLog (
  IN  UINT8        Socket,
  IN  UINT8        Ch,
  IN  UINT8        Dimm,
  IN  UINT8        LogLevel,
  IN  UINT8        LogType,
  IN  UINT8        Count,
  OUT ERR_LOG_DATA *ErrLog
  )
{
  EFI_STATUS        Status;
  UINT16            SequenceNumber;
  UINT16            RequestCount;
  ERR_LOG_INFO_DATA ErrLogInfoData;

  //
  // Send first command to get log info
  //
  Status = SendFnvGetErrorLog (Socket, Ch, Dimm, LogLevel, LogType, RETRIEVE_LOG_INFO, 0, 0, (VOID *) &ErrLogInfoData,
    sizeof (ErrLogInfoData));
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "SendFnvGetErrorLog failed: %r. No error log info has been retreived.",
      Status));
    return Status;
  }

  // Determine starting sequence number and entry count for error log to return
  RequestCount = 1;
  SequenceNumber = ErrLogInfoData.CurrentSequenceNumber;
  while ((RequestCount < Count) &&
         (RequestCount < ErrLogInfoData.MaxLogEntries) &&
         (SequenceNumber != ErrLogInfoData.OldestSequenceNumber)) {
    RequestCount++;
    SequenceNumber--;
    if (SequenceNumber == 0) {    // Sequence number wraps from 0xFFFF to 1
      SequenceNumber = 0xFFFF;
    }
  }

  // Send second command to get log entries
  Status = SendFnvGetErrorLog (Socket, Ch, Dimm, LogLevel, LogType, RETRIEVE_LOG_DATA, SequenceNumber, RequestCount,
                               ErrLog, sizeof (*ErrLog));
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "SendFnvGetErrorLog failed: %r. No error log has been retreived.",
      Status));
    return Status;
  }

  return EFI_SUCCESS;
}


/**
  @brief Get specific ErrLog details and validate ErrLog details by checking sequence number.

  @param[in]     ErrLog                 Pointer to ERR_LOG_DATA union to store retrieved log entries in.
  @param[in]     LogType                Log type
                                        MEDIA_LOG_REQ for media log
                                        THERMAL_LOG_REQ for thermal log
  @param[out]    *FirstSequenceNumber   Pointer to first element of sequence number from ErrLog list
  @param[out]    *LastSequenceNumber    Pointer to last element of sequence number from ErrLog list
  @param[out]    *ReturnCount           Pointer to count number of ErrLog elements



  @retval        FALSE                  If any element from sequence number is not one by one or ReturnCount has incorrect value.
  @retval        TRUE                   If whole sequence number sets is correct
**/
BOOLEAN
ValidateAndGetErrorLogDetails (
  IN     ERR_LOG_DATA *ErrLog,
  IN     UINT8        LogType,
     OUT UINT16       *FirstSequenceNumber,
     OUT UINT16       *LastSequenceNumber,
     OUT UINT16       *ReturnCount
  )
{
  UINT8   Index = 0;

  if (FirstSequenceNumber == NULL || LastSequenceNumber == NULL || ReturnCount == NULL) {
    return FALSE;
  }

  switch (LogType) {
    case MEDIA_LOG_REQ:
      *ReturnCount = ErrLog->MediaLogData.ReturnCount;

      if (*ReturnCount == 0) {
        break;
      }

      if (*ReturnCount > 0 && *ReturnCount <= MAX_MEDIA_LOGS) {
        *FirstSequenceNumber = ErrLog->MediaLogData.MediaLogs[0].SequenceNumber;
        *LastSequenceNumber = ErrLog->MediaLogData.MediaLogs[*ReturnCount - 1].SequenceNumber;

        for (Index = 0; Index < *ReturnCount - 1; Index++) {
          if (ErrLog->MediaLogData.MediaLogs[Index].SequenceNumber == NGN_ERROR_LOG_MAX_SEQ_NUM &&
              ErrLog->MediaLogData.MediaLogs[Index + 1].SequenceNumber == NGN_ERROR_LOG_MIN_SEQ_NUM) {
            continue;
          }
          if (ErrLog->MediaLogData.MediaLogs[Index].SequenceNumber + 1 == ErrLog->MediaLogData.MediaLogs[Index + 1].SequenceNumber) {
            continue;
          }
          return FALSE;
        }
        break;
      }
      return FALSE;
    case THERMAL_LOG_REQ:
      *ReturnCount = ErrLog->ThermalLogData.ReturnCount;

      if (*ReturnCount == 0) {
        break;
      }

      if (*ReturnCount > 0 && *ReturnCount <= MAX_THERMAL_LOGS) {
        *FirstSequenceNumber = ErrLog->ThermalLogData.ThermalLogs[0].SequenceNumber;
        *LastSequenceNumber = ErrLog->ThermalLogData.ThermalLogs[*ReturnCount - 1].SequenceNumber;

        for (Index = 0; Index < *ReturnCount - 1; Index++) {
          if (ErrLog->ThermalLogData.ThermalLogs[Index].SequenceNumber == NGN_ERROR_LOG_MAX_SEQ_NUM &&
              ErrLog->ThermalLogData.ThermalLogs[Index + 1].SequenceNumber == NGN_ERROR_LOG_MIN_SEQ_NUM) {
            continue;
          }
          if (ErrLog->ThermalLogData.ThermalLogs[Index].SequenceNumber + 1 == ErrLog->ThermalLogData.ThermalLogs[Index + 1].SequenceNumber) {
            continue;
          }
          return FALSE;
        }
        break;
      }
      return FALSE;
    default:
      ASSERT (FALSE);
      break;
  }

  return TRUE;
}


/**
  @brief Retrieves specified error log starting at specified sequence number.

  Stores new sequence number (to be used with subsequent calls) in SequenceNumber.

  @param[in]      Socket                 Socket index
  @param[in]      Ch                     DDR channel index
  @param[in]      Dimm                   DIMM index
  @param[in]      LogType                Log type
                                         MEDIA_LOG_REQ for media log
                                         THERMAL_LOG_REQ for thermal log
  @param[in]      LogLevel               Log level, LOW_LEVEL_LOG or HIGH_LEVEL_LOG
  @param[in,out]  SequenceNumber         Pointer to sequence number to start retrieving log entries from.
                                         When function returns EFI_SUCCESS or EFI_NOT_FOUND, sequence number of next
                                         event is stored at location pointed by SequenceNumber.
  @param[out]     ErrLog                 Pointer to ERR_LOG_DATA union to store retrieved log entries in.

  @retval         EFI_SUCCESS            Success.
  @retval         EFI_INVALID_PARAMETER  Invalid Log Type or Log Level.
                                         SequenceNumber or ErrLog is NULL.
                                         Specified DIMM is not found.
  @retval         EFI_NOT_FOUND          No log matching *SequenceNumber found.
  @retval         MailboxStatusDecode    Error returned by MailboxStatusDecode.
**/
EFI_STATUS
EFIAPI
NvmCtlrGetErrorLog (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        Dimm,
  IN     UINT8        LogType,
  IN     UINT8        LogLevel,
  IN     BOOLEAN      CurrentBoot,
  IN OUT UINT16       *SequenceNumber,
     OUT ERR_LOG_DATA *ErrLog
  )
{
  EFI_STATUS                          Status;
  UINT16                              Count = 0;
  UINT16                              ReturnCount = 0;
  UINT16                              FirstSequenceNumber = 0;
  UINT16                              LastSequenceNumber = 0;
  HOST_DDRT_DIMM_DEVICE_INFO_STRUCT   *HostDdrtDimmInfo;
  UINT16                              MaxLogEntries;
  UINT16                              LogEnd;

  if (ErrLog == NULL || SequenceNumber == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (LogLevel >= LOG_LEVEL_NUM) {
    return EFI_INVALID_PARAMETER;
  }

  switch (LogType) {
  case MEDIA_LOG_REQ:
    Count = ARRAY_SIZE (ErrLog->MediaLogData.MediaLogs);
    break;
  case THERMAL_LOG_REQ:
    Count = ARRAY_SIZE (ErrLog->ThermalLogData.ThermalLogs);
    break;
  default:
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  HostDdrtDimmInfo = &(mSystemMemoryMap->HostDdrtDimmInfo[Socket][Ch]);

  if (*SequenceNumber == 0 && CurrentBoot) {
    *SequenceNumber = HostDdrtDimmInfo->NgnLogSeqNum[LogType][LogLevel];
  }
  MaxLogEntries = HostDdrtDimmInfo->NgnMaxLogEntries[LogType][LogLevel];

  Status = SendFnvGetErrorLog (Socket, Ch, Dimm, LogLevel, LogType, RETRIEVE_LOG_DATA, *SequenceNumber, Count, ErrLog,
                               sizeof (*ErrLog));
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: NvmCtlrGetErrorLog: ", Socket, Ch, Dimm));
    CRDEBUG ((DEBUG_ERROR, "SendFnvGetErrorLog (..., %d, %d, RETRIEVE_LOG_DATA, %d, ...) failed: %r\n",
            LogLevel, LogType, *SequenceNumber, Status));
    return Status;
  }

  if (!ValidateAndGetErrorLogDetails (ErrLog, LogType, &FirstSequenceNumber, &LastSequenceNumber, &ReturnCount)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("CRP") "S%d.C%d.D%d: NvmCtlrGetErrorLog: Invalid error log data", Socket, Ch, Dimm));
    return EFI_COMPROMISED_DATA;
  }

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "S%d.C%d.D%d: NvmCtlrGetErrorLog: ", Socket, Ch, Dimm));
  CRDEBUG ((DEBUG_INFO, "LogType: %u, LogLevel: %u, *SequenceNumber: %u, ReturnCount: %u\n",
          LogType, LogLevel, *SequenceNumber, ReturnCount));
  if (ReturnCount != 0) {
    LogEnd = FirstSequenceNumber + MaxLogEntries;
    if (LogEnd == 0) {
      LogEnd = 1;
    }
    CRDEBUG ((DEBUG_INFO, "                                         "
                        "FirstSequenceNumber: %u, LastSequenceNumber: %u, LogEnd: %u\n",
            FirstSequenceNumber, LastSequenceNumber, LogEnd));
    if (LogEnd == *SequenceNumber) {
      // false positive, e.g.
      // [         LLLLLLLL]
      //  ^
      return EFI_NOT_FOUND;
    }
    // anything else is fine
    *SequenceNumber = LastSequenceNumber + 1;
    if (*SequenceNumber == 0) {
      *SequenceNumber = 1;
    }
    return EFI_SUCCESS;
  }
  if (*SequenceNumber <= 1 || !CurrentBoot) {
    return EFI_NOT_FOUND;
  }

  Status = SendFnvGetErrorLog (Socket, Ch, Dimm, LogLevel, LogType, RETRIEVE_LOG_DATA, 0, Count, ErrLog,
                               sizeof (*ErrLog));
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: NvmCtlrGetErrorLog: ", Socket, Ch, Dimm));
    CRDEBUG ((DEBUG_ERROR, "SendFnvGetErrorLog (..., %d, %d, RETRIEVE_LOG_DATA, 0, ...) failed: %r\n",
            LogLevel, LogType, Status));
    return Status;
  }

  if (!ValidateAndGetErrorLogDetails (ErrLog, LogType, &FirstSequenceNumber, &LastSequenceNumber, &ReturnCount)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("CRP") "S%d.C%d.D%d: NvmCtlrGetErrorLog: Invalid error log data", Socket, Ch, Dimm));
    return EFI_COMPROMISED_DATA;
  }

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR("CRP") "S%d.C%d.D%d: NvmCtlrGetErrorLog: ", Socket, Ch, Dimm));
  CRDEBUG ((DEBUG_INFO, "LogType: %u, LogLevel: %u, ReturnCount: %u\n",
          LogType, LogLevel, ReturnCount));
  if (ReturnCount != 0) {
    LogEnd = FirstSequenceNumber + MaxLogEntries;
    if (LogEnd == 0) {
      LogEnd = 1;
    }
    CRDEBUG ((DEBUG_INFO, "                                         "
                        "FirstSequenceNumber: %u, LastSequenceNumber: %u, LogEnd: %u\n",
            FirstSequenceNumber, LastSequenceNumber, LogEnd));
    if (LogEnd == *SequenceNumber || FirstSequenceNumber == 1) {
      // true negative
      return EFI_NOT_FOUND;
    }
    // false negative, e.g.
    // [LLLL           LLLL]
    //               ^
    *SequenceNumber = LastSequenceNumber + 1;
    if (*SequenceNumber == 0) {
      *SequenceNumber = 1;
    }
    return EFI_SUCCESS;
  }
  // no log, DIMM overwritten
  *SequenceNumber = 1;
  return EFI_NOT_FOUND;
}


/**
  Routine Description: Gets error logs from NVMCTLR. Uses small payload, so
  request count shouldn't be more than 3 for media logs and 7 for thermal logs.

  @param[in] Socket            - Socket Number
  @param[in] Ch                - DDR Channel ID
  @param[in] Dimm              - DIMM Number
  @param[in] LogLevel          - Log Level 0 - low priority 1 - high priority
  @param[in] LogType           - 0 - for media logs, 1 for thermal
  @param[in] LogInfo           - 0 - retrieve log data, 1 - retrieve log info
  @param[in] Sequence          - Reads log from specified sequence number
  @param[in] RequestCount      - Max number of log entries requested for this access
  @param[out] *ErrLog          - pointer to LogData structure.
                                 Caller must assure this data struct is big enough.

  @retval EFI_INVALID_PARAMETER  - one of parameters is not valid
  @retval EFI_OUT_OF_RESOURCES   - requested number of log entries does not fit in small payload
  @retval EFI_SUCCESS            - success
  @retval other                  - see return codes of FisRequest

**/
EFI_STATUS
SendFnvGetErrorLog (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     LogLevel,
  IN  UINT8     LogType,
  IN  UINT8     LogInfo,
  IN  UINT16    Sequence,
  IN  UINT16    RequestCount,
  OUT VOID      *ErrLog,
  IN  UINT32    ErrLogSize
  )
{
  EFI_STATUS                        Status;
  UINT32                            EntrySize;
  UINT32                            LogSize;
  FIS_GET_ERROR_LOG_REQ             Data;
  UINT32                            OutPayload[NUM_PAYLOAD_REG];

  if ((ErrLog  == NULL)              ||
      (LogLevel > HIGH_LEVEL_LOG)    ||
      (LogInfo  > RETRIEVE_LOG_INFO) ||
      (LogType  > THERMAL_LOG_REQ)) {

    return EFI_INVALID_PARAMETER;
  }

  EntrySize = (LogType == MEDIA_LOG_REQ) ? sizeof(MEDIA_ERR_LOG) : sizeof(THERMAL_ERR_LOG);

  if (LogInfo == RETRIEVE_LOG_INFO) {
    LogSize = sizeof (ERR_LOG_INFO_DATA);
  } else {
    LogSize = ERROR_LOG_HEAD_SIZE + EntrySize * RequestCount;
  }
  if (LogSize > CR_OUT_PAYLOAD_SIZE) {
    return EFI_OUT_OF_RESOURCES;
  }
  if (LogSize > ErrLogSize) {
    return EFI_BUFFER_TOO_SMALL;
  }

  ZeroMem (&Data, sizeof (Data));
  Data.Bits.LogLevel       = LogLevel;
  Data.Bits.LogType        = LogType;
  Data.Bits.LogInfo        = LogInfo;
  Data.Bits.LogPayload     = USE_SMALL_PAYLOAD;
  Data.Bits.SequenceNumber = Sequence;
  Data.Bits.RequestCount   = RequestCount;

  Status = FisRequest (Socket, Ch, Dimm, (UINT32*)&Data, sizeof (Data), &OutPayload[0], GET_ERROR_LOG, FIS_DEFTIMEOUT, NULL);
  if (!EFI_ERROR(Status)) {
    CopyMem (ErrLog, &OutPayload[0], LogSize);
  }
  return Status;
}

/**
  @brief Get the state of long operation for given DIMM.

  @param[in]  Skt    Socket ID for given DIMM (0 based)
  @param[in]  Ch     Channel ID (0 based, socket based)
  @param[in]  Dimm   DIMM slot ID
  @param[out] FisRsp LongOp Fis response
  @param[out] SeqNum Fis sequence number

  @return EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
NvmCtlrLongOpCompleteGet (
  IN  UINT8   Skt,
  IN  UINT8   Ch,
  IN  UINT8   Dimm,
  OUT UINT32 *FisRspPtr,
  OUT UINT32 *SeqNum
  )
{
  EFI_STATUS Status;

  if (!IsNvmDimm (Skt, Ch, Dimm)) {

    ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }

  Status = FisRequest (Skt, Ch, Dimm, NULL, 0, FisRspPtr, GET_LONG_OPERATION, FIS_DEFTIMEOUT, NULL);

  if (!EFI_ERROR(Status) && (SeqNum != NULL)) {
    *SeqNum = FisLongOpSeqNum[Skt][Ch];
  }

  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: Cannot retrieve long op status (%r)\n", Skt, Ch, Dimm, Status));
  }

  return Status;
}


/**
  @brief Verify the state of long operation for given DIMM.

  Internal function.

  @param[in]   Skt      Socket ID for given DIMM (0 based)
  @param[in]   Ch       Channel ID (0 based, socket based)
  @param[in]   Dimm     DIMM slot ID
  @param[out]  FisRsp   LongOp Fis response
  @param[in]   Recurse  TRUE:  allow recursion into NvmCtlrIntLongOpCompleteProcess
                        FALSE: do not allow recursion into
                               NvmCtlrIntLongOpCompleteProcess
  @param[in]   SeqNum   Fis sequence number

**/
VOID
NvmCtlrIntLongOpCompleteProcess (
  IN UINT8   Skt,
  IN UINT8   Ch,
  IN UINT8   Dimm,
  IN UINT32  *FisRsp,
  IN BOOLEAN Recurse,
  IN UINT32  *SeqNum
  )
{
  FIS_GET_LONG_OPERATION_STATUS_RSP *FisRspPtr = (FIS_GET_LONG_OPERATION_STATUS_RSP *) FisRsp;
  EFI_STATUS                        Status;

  if (FisRspPtr->Bits.StatusCode != FIS_STS_DEVICE_BUSY &&   // If operation not running and
      FisLongOpRunning[Skt][Ch] != NULL_COMMAND &&           // status was not consumed yet
      *SeqNum == FisLongOpSeqNum[Skt][Ch]) {

    if (FisRspPtr->Bits.Command != FisLongOpRunning[Skt][Ch]) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: completed long op mismatch: %04X, %04X expected\n",
              Skt, Ch, Dimm, FisRspPtr->Bits.Command, FisLongOpRunning[Skt][Ch]));
    }

    FisLongOpRunning[Skt][Ch] = NULL_COMMAND;
    mNvdimmAcpiSmmInterface->LongOpStatus[Skt][Ch] = NULL_COMMAND;
    //
    // Call long operation type specific handler. Ignore result, the operation
    // might have been started via pass-through and not tracked in these subsystems.
    //
    switch (FisRspPtr->Bits.Command) {
      case SET_ADDRESS_RANGE_SCRUB:
        Status = ArsProcessResults (Skt, Ch, Dimm, Recurse);
        if (Status == EFI_NOT_STARTED) {
          ArsProcessForAllDimms ();
        }
        break;
      case UPDATE_FNV_FW:
        DsmProcessUpdateResults (Skt, Ch, Dimm, FisRspPtr);
        if (ArsInBackground () && ArsDimmInCurrentIntSet (Skt, Ch, Dimm)) {
          AcpiRootNotifyEx (TRUE);
        }
        break;
      case OVERWRITE_DIMM:
        DsmProcessOverwriteDimmResults (Skt, Ch, Dimm, FisRspPtr);
        if (ArsInBackground () && ArsDimmInCurrentIntSet (Skt, Ch, Dimm)) {
          AcpiRootNotifyEx (TRUE);
        }
        break;
      default:
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: unknown long op 0x%04X\n",
                  Skt, Ch, Dimm, FisRspPtr->Bits.Command));
    }
  } else {
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "S%d.C%d.D%d: long op 0x%04X incomplete/unknown: ",
              Skt, Ch, Dimm, FisRspPtr->Bits.Command));
    CRDEBUG ((DEBUG_INFO, "status %02X, last command: %04X, PrevSeqNum %d, ActualSeqNum %d\n",
              FisRspPtr->Bits.StatusCode, FisLongOpRunning[Skt][Ch], *SeqNum, FisLongOpSeqNum[Skt][Ch]));
  }
}

/**
  @brief Verify the state of long operation for given DIMM.

  @param[in]   Skt      Socket ID for given DIMM (0 based)
  @param[in]   Ch       Channel ID (0 based, socket based)
  @param[in]   Dimm     DIMM slot ID
  @param[out]  FisRsp   LongOp Fis response
  @param[in]   SeqNum   Fis sequence number

**/
VOID
EFIAPI
NvmCtlrLongOpCompleteProcess (
  IN UINT8 Skt,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN UINT32 *FisRsp,
  IN UINT32 *SeqNum
  )
{
  NvmCtlrIntLongOpCompleteProcess (Skt, Ch, Dimm, FisRsp, TRUE, SeqNum);
}

/**
  @brief Updates the state of long operation for given DIMM.

  @param[in] Skt   Socket ID for given DIMM (0 based)
  @param[in] Ch    Channel ID (0 based, socket based)
  @param[in] Dimm  DIMM slot ID

  @return EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
NvmCtlrLongOpComplete (
  IN UINT8 Skt,
  IN UINT8 Ch,
  IN UINT8 Dimm
  )
{
  EFI_STATUS Status;
  FIS_GET_LONG_OPERATION_STATUS_RSP FisRsp;
  UINT32 SeqNum;

  Status = NvmCtlrLongOpCompleteGet (Skt, Ch, Dimm, &FisRsp.DWord[0], &SeqNum);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  NvmCtlrIntLongOpCompleteProcess (Skt, Ch, Dimm, &FisRsp.DWord[0], FALSE, &SeqNum);
  return Status;
}


/**
  @brief Set DDRT Alerts in NVDIMM

  @param[in] Socket  - Socket number
  @param[in] Ch      - Per socket channel number
  @param[in] Dimm    - DIMM number in channel
  @param[out] Alerts - Buffer with alerts table to set

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
NvmCtlrSetHostAlert (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT16    Alerts[MAX_HOST_TRANSACTIONS]
  )
{
  return FisRequest (Socket, Ch, Dimm, (UINT32*)Alerts, sizeof (UINT16) * MAX_HOST_TRANSACTIONS, NULL, SET_DDRT_ALERTS, FIS_DEFTIMEOUT, NULL);
}


/**
  @brief Read DDRT Alerts table from NVDIMM

  @param[in] Socket  - Socket number
  @param[in] Ch      - Per socket channel number
  @param[in] Dimm    - DIMM number in channel
  @param[out] Alerts - Buffer for alerts table

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
NvmCtlrGetHostAlert (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT16    Alerts[MAX_HOST_TRANSACTIONS]
  )
{
  return FisRequest (Socket, Ch, Dimm, NULL, 0, (UINT32*)Alerts, GET_DDRT_ALERTS, FIS_DEFTIMEOUT, NULL);
}


/**
  Routine Description: this function will send
  Set Address Range Scrub command to NVMCTLR.
  CR protocol function.

  @param[in] Socket             - Socket Number
  @param[in] Ch                 - DDR Channel ID
  @param[in] Dimm               - DIMM number
  @param[in] DpaStartAddress    - Start address for scrubbing
  @param[in] DpaEndAddress      - End address for scrubbing
  @param[in] Enable             - Enable\Disable scrubbing

  @return EFI_STATUS            - Status of the command sent
**/
EFI_STATUS
EFIAPI
NvmCtlrSetAddressRangeScrub (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT64    DpaStartAddress,
  UINT64    DpaEndAddress,
  BOOLEAN   Enable
  )
{
  FIS_SET_ARS_REQ Data;
  FIS_GET_ARS_RSP GetArsRsp;
  EFI_STATUS      Status;
  UINT32          Iter;

  if (Enable){
    if (DpaStartAddress >= DpaEndAddress) {
      return EFI_INVALID_PARAMETER;
    }
    if (FisLongOpRunning[Socket][Ch] != NULL_COMMAND) {
      //
      // If some long operation is in progress check if still running.
      // If so return retry-suggested.
      //
      NvmCtlrLongOpComplete (Socket, Ch, Dimm);
      if (FisLongOpRunning[Socket][Ch] != NULL_COMMAND) {

        return EFI_NOT_READY;
      }
    }
  }
  ZeroMem (&Data, sizeof (Data));
  Data.Bits.Enable           = Enable ? 0x1 : 0x0;
  Data.Bits.DpaStartAddress  = DpaStartAddress;
  Data.Bits.DpaEndAddress    = DpaEndAddress;

  Status = FisRequest (Socket, Ch, Dimm, (UINT32*)&Data, sizeof (Data), NULL, SET_ADDRESS_RANGE_SCRUB, FIS_DEFTIMEOUT, NULL);

  if (Enable) {
    return Status;
  }

  for (Iter = 0; Iter < GET_ARS_MAX_ITER; Iter++) {
    Status = FisRequest (Socket, Ch, Dimm, NULL, 0, &GetArsRsp.DWord[0], GET_ADDRESS_RANGE_SCRUB, 0, NULL);
    if (!EFI_ERROR (Status) && GetArsRsp.Bits.Enabled == 0) {
      break;
    }
    MicroSecondDelay (GET_ARS_POLLING_INTERVAL_US);
  }
  if (EFI_ERROR (Status)) {
    return Status;
  }
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "S%d.C%d.D%d: NvmCtlrSetAddressRangeScrub: ARS running: %d (iter %d)\n",
          Socket, Ch, Dimm, GetArsRsp.Bits.Enabled, Iter));
  return GetArsRsp.Bits.Enabled == 0 ? EFI_SUCCESS : EFI_ALREADY_STARTED;
}


/**
  Routine Description: this function will send
  Get Address Range Scrub command to NVMCTLR.
  CR protocol function.

  @param[in]  Socket                 - Socket Number
  @param[in]  Ch                     - DDR Channel ID
  @param[in]  Dimm                   - DIMM number
  @param[out] *DpaStartAddress       - Start address for scrubbing
  @param[out] *DpaCurrentAddress     - Current address of scrub
  @param[out] *DpaEndAddress         - End address for scrubbing
  @param[out] *Enabled               - TRUE if scrub is enabled

  @return EFI_STATUS            - Status of the command sent
**/
EFI_STATUS
EFIAPI
NvmCtlrGetAddressRangeScrub (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  OUT UINT64    *DpaStartAddress,
  OUT UINT64    *DpaEndAddress,
  OUT UINT64    *DpaCurrentAddress,
  OUT BOOLEAN   *Enabled
  )
{
  EFI_STATUS          Status;
  FIS_GET_ARS_RSP     Data;

  if ((DpaStartAddress == NULL) ||
      (DpaCurrentAddress == NULL) ||
      (DpaEndAddress == NULL) ||
      (Enabled == NULL) ||
      (GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED) == NULL)) {

    return EFI_INVALID_PARAMETER;
  }
  Status = FisRequest (Socket, Ch, Dimm, NULL, 0, (UINT32*)&Data, GET_ADDRESS_RANGE_SCRUB, FIS_DEFTIMEOUT, NULL);
  if (!EFI_ERROR (Status)) {

    *Enabled           = Data.Bits.Enabled != 0;
    *DpaStartAddress   = Data.Bits.DpaStartAddress;
    *DpaEndAddress     = Data.Bits.DpaEndAddress;
    *DpaCurrentAddress = Data.Bits.DpaCurrentAddress;
  }
  return Status;
}


/**
  @brief Enable Error Injection in NVDIMM

  @param[in] Socket  - Socket number
  @param[in] Ch      - Per socket channel number
  @param[in] Dimm    - DIMM number in channel

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
NvmCtlrEnableErrInjection (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm
  )
{
  FIS_ENABLE_ERR_INJECTION_REQ  Data;

  ZeroMem (&Data, sizeof (Data));

  Data.Bits.Enable = 0x1; // Enable Error Injection

  return FisRequest (Socket, Ch, Dimm, (UINT32*)&Data, sizeof (Data), NULL, ENABLE_ERR_INJECTION, FIS_DEFTIMEOUT, NULL);
}


/**
  @brief Enable Error Injection in NVDIMM

  @param[in] Socket  - Socket number
  @param[in] Ch      - Per socket channel number
  @param[in] Dimm    - DIMM number in channel

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
NvmCtlrDisableErrInjection (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm
  )
{
  FIS_ENABLE_ERR_INJECTION_REQ  Data;

  ZeroMem (&Data, sizeof (Data));

  Data.Bits.Enable = 0x0; // Disable Error Injection

  return FisRequest (Socket, Ch, Dimm, (UINT32*)&Data, sizeof (Data), NULL, ENABLE_ERR_INJECTION, FIS_DEFTIMEOUT, NULL);
}


/**
  Enables error injection in all NVDIMMs in the platform

  @return EFI_STATUS - success if all NVDIMMs enabled error injection
**/
EFI_STATUS
EFIAPI
NvmCtlrPlatformEnableErrInjection (
  VOID
  )
{
  FIS_ENABLE_ERR_INJECTION_REQ  Data;
  EFI_STATUS                    Status;
  EFI_STATUS                    FinalStatus = EFI_SUCCESS;
  NVDIMM                        *DimmPtr;
  UINT32                        Index;
  BOOLEAN                       InjectionEnabled = FALSE;
  UINT8                         Socket;
  UINT8                         Ch;
  UINT8                         Dimm;

  ZeroMem (&Data, sizeof (Data));
  Data.Bits.Enable = 0x1;

  for (Index = 0; Index < mCrInfo.NvdimmInfo.NumNvdimms; Index++) {
    DimmPtr = &mCrInfo.NvdimmInfo.Nvdimms[Index];
    if (DimmPtr->SADSpaBase == SPA_NOT_MAPPED) {
      continue;
    }
    Socket = (UINT8)DimmPtr->SocketId;
    Ch = DimmPtr->Ch;
    Dimm = DimmPtr->Dimm;
    Status = FisRequest (Socket, Ch, Dimm, (UINT32*)&Data, sizeof (Data), NULL, ENABLE_ERR_INJECTION, FIS_DEFTIMEOUT, NULL);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: Enabling error injection failed (%r)\n",
             Socket, Ch, Dimm, Status));
      if (!EFI_ERROR (FinalStatus)) {
        // capture first error but continue -- best effort
        FinalStatus = Status;
      }
    } else {
      InjectionEnabled = TRUE;
    }
  }
  mNvdimmAcpiSmmInterface->InjectionEnabled = InjectionEnabled;
  return FinalStatus;
}


/**
  Routine Description: this function injects an error for a specified
  memory type at a particular device physical address.

  @param[in]  Socket        - Socket Number
  @param[in]  Ch            - DDR Channel ID
  @param[in]  Dimm          - DIMM number
  @param[in]  Dpa           - DPA of the device
  @param[in]  MemoryType    - Type of memory mapped to DPA
  @param[in]  Enable        - Enable/disable poison for this address

  @return EFI_STATUS - success if command sent
**/
EFI_STATUS
EFIAPI
NvmCtlrInjectPoisonError (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT64    Dpa,
  IN  UINT8     MemoryType,
  IN  UINT8     Enable
  )
{
  EFI_STATUS                   Status;
  FIS_INJECT_POISON_ERROR_REQ  Data;

  ZeroMem (&Data, sizeof (Data));

  Data.Bits.Enable = Enable;
  Data.Bits.MemoryType = MemoryType;
  Data.Bits.DpaPoisonAddress = Dpa;

  Status = FisRequest (Socket, Ch, Dimm, (UINT32*)&Data, sizeof (Data), NULL, INJECT_POISON_ERROR, FIS_DEFTIMEOUT, NULL);

  if (EFI_ERROR(Status)) {
    CRDEBUG((DEBUG_ERROR, CR_ERROR_STR("CRP") "S%d.C%d.D%d: Poison error injection failed (%r)\n", Socket, Ch, Dimm, Status));
  }
  return Status;
}


/**
  @brief Build NVDIMM Firmware Interface Tables (NFIT) in ACPI tables.
         NOTICE: Implementation moved to NfitTableUpdateProtocol

  @retval EFI_UNSUPPORTED         Protocol does not support NFIT Update now

**/
EFI_STATUS
EFIAPI
UpdateNvmAcpiTables (
  IN OUT UINT64 *NfitTablePtr
  )
{
  CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("CRP") "Unsupported function call: UpdateNvmAcpiTables()\n"));
  return EFI_UNSUPPORTED;
}


/**
 CR interface function, see documentation in Protocol/CrystalRidge.h
 **/
EFI_STATUS
EFIAPI
UpdateNvmAcpiPcatTable (
  UINT64 *PcatTablePtr
  )
{
  EFI_STATUS Status;
  EFI_ACPI_PCAT_PROTOCOL *Pcat;
  UINTN                  PcatTableSize = sizeof(NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE);

  if (NULL == PcatTablePtr) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "NULL pointer passed for PCAT table\n"));
    return EFI_INVALID_PARAMETER;
  }

  // Access to ACPI PCAT supporting functions
  Status = gBS->LocateProtocol (&gAcpiPcatProtocolGuid, NULL, (VOID**)&Pcat);
  if (EFI_ERROR (Status)) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "Cannot locate PCAT protocol (%r)\n", Status));
    return EFI_NOT_STARTED;
  }
  //
  // Initialize ACPI PCAT only if not initialized yet
  //
  if (!Pcat->IsInitialized ()) {
    Status = Pcat->InitializeAcpiPcat (PcatTablePtr, PcatTableSize);
    if (Status != EFI_SUCCESS) {

      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "Cannot initialize PCAT header (%r)\n", Status));
      return Status;
    }
  }
  return EFI_SUCCESS;
}


/**
 CR interface function, see documentation in Protocol/CrystalRidge.h
 **/
BOOLEAN
EFIAPI
IsNvmAddress (
  IN UINT64 Spa
  )
{
  UINT8            Socket;
  UINT8            Sad;
  UINT64           SpaStart;
  UINT64           SpaEnd;
  SAD_TABLE       *SadPtr;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Traverse thru all SAD entries to check for the SPA being in the range of one
    //
    for (Sad = 0; Sad < MAX_SAD_RULES; Sad++) {

      SadPtr = &mSystemMemoryMap->Socket[Socket].SAD[Sad];
      //
      // If the Sad type is for an NVMDIMM..
      //
      if (SadPtr->type != MemType1lmDdr) {

        if (SadPtr->Enable == 0 || SadPtr->local == 0) {
          continue;
        }
        SpaStart = NvdimmGetSadBase (Socket, Sad);
        SpaEnd = (UINT64)SadPtr->Limit << BITS_64MB_TO_BYTES;
        //
        // If the passed SPA is in this SAD SPA Range
        //
        if ((Spa >= SpaStart) && (Spa <= SpaEnd)) {
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}


/*
  Routine Description: Does this system have at least one NVMDIMM

  @return TRUE if NVMDIMM present
**/
BOOLEAN
EFIAPI
IsSystemWithDcpmm(
  VOID
  )
{
  if (mSystemMemoryMap->DcpmmPresent != 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**

  Routine Description: Is this Dimm the NVMDIMM.
    CR interface protocol function.

  @param[in] Socket      - Socket Number
  @param[in] Ch          - DDR Channel ID
  @param[in] Dimm        - DIMM number

  @return TRUE if NVMDIMM otherwise FALSE

**/
BOOLEAN
EFIAPI
IsNvmDimm (
  UINT8 Socket,
  UINT8 Ch,
  UINT8 Dimm
  )
{
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  if (Socket >= MAX_SOCKET || Ch >= MaxChDdr || Dimm >= MAX_DIMM) {
    return FALSE;
  }

  if (mSystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].Present != 0 &&
      mSystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].DcpmmPresent != 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}


/**
  CR interface function, see documentation in Protocol/CrystalRidge.h
 **/
EFI_STATUS
EFIAPI
NvmCtlrSetCfgDataPolicy (
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     Dimm,
  IN    UINT32    Data
  )
{
  return FisRequest (Socket, Ch, Dimm, &Data, sizeof (Data), NULL, SET_CONFIG_DATA_POLICY, FIS_DEFTIMEOUT, NULL);
}


/**
 CR interface function, see documentation in Protocol/CrystalRidge.h
 **/
EFI_STATUS
EFIAPI
NvmCtlrGetCfgDataPolicy (
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     Dimm,
  OUT   UINT32    *Data
  )
{
  EFI_STATUS  Status;
  UINT32      OutputPayload[NUM_PAYLOAD_REG];

  Status = FisRequest (Socket, Ch, Dimm, NULL, 0, OutputPayload, GET_CONFIG_DATA_POLICY, FIS_DEFTIMEOUT, NULL);
  *Data = OutputPayload[0];

  return Status;
}


/**
 CR interface function, see documentation in Protocol/CrystalRidge.h
 **/
EFI_STATUS
EFIAPI
NvmCtlrSetViralPolicy (
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     Dimm,
  IN    UINT32    Data
  )
{
  return FisRequest (Socket, Ch, Dimm, &Data, sizeof (Data), NULL, SET_VIRAL_POLICY, FIS_DEFTIMEOUT, NULL);
}


/**
  CR interface function, see documentation in Protocol/CrystalRidge.h
 **/
EFI_STATUS
EFIAPI
NvmCtlrGetViralPolicy (
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     Dimm,
  OUT   UINT32    *Data
  )
{
  EFI_STATUS  Status;
  UINT32      OutputPayload[NUM_PAYLOAD_REG];

  Status = FisRequest (Socket, Ch, Dimm, NULL, 0, OutputPayload, GET_VIRAL_POLICY, FIS_DEFTIMEOUT, NULL);
  *Data =  OutputPayload[0];

  return Status;
}


/**
 CR interface function, see documentation in Protocol/CrystalRidge.h
 **/
EFI_STATUS
EFIAPI
NvmCtlrGetDimmInfo (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       Dimm,
  OUT NVDIMM_INFO *pDimmInfo
  )
{
  struct  DimmDevice *pDimmDevice;

  if (NULL == pDimmInfo) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if this is NVDIMM
  //
  if (!IsNvmDimm (Socket, Ch, Dimm)){
    return EFI_NOT_FOUND;
  }

  //
  // Get DIMM device data location
  //
  pDimmDevice = &mSystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm];

  ZeroMem (pDimmInfo, sizeof(NVDIMM_INFO));

  pDimmInfo->DeviceId = pDimmDevice->DeviceID;
  pDimmInfo->VendorId = pDimmDevice->VendorID;
  pDimmInfo->RevisionId = pDimmDevice->RevisionID;
  pDimmInfo->FisVersion = pDimmDevice->FisVersion;
  pDimmInfo->Ifc = pDimmDevice->InterfaceFormatCode;
  pDimmInfo->Sku = pDimmDevice->DimmSku;

  CopyMem (pDimmInfo->SerialNumber, pDimmDevice->serialNumber, (sizeof(UINT8) * NGN_MAX_SERIALNUMBER_STRLEN));
  CopyMem (pDimmInfo->PartNumber, pDimmDevice->PartNumber, (sizeof(UINT8) * NGN_MAX_PARTNUMBER_STRLEN));

  pDimmInfo->FirmwareVersion.MinorVersion = pDimmDevice->FirmwareVersion.minorVersion;
  pDimmInfo->FirmwareVersion.MajorVersion = pDimmDevice->FirmwareVersion.majorVersion;
  pDimmInfo->FirmwareVersion.HotfixVersion = pDimmDevice->FirmwareVersion.hotfixVersion;
  pDimmInfo->FirmwareVersion.BuildVersion = pDimmDevice->FirmwareVersion.buildVersion;

  return EFI_SUCCESS;
}


/**
 CR interface function, see documentation in Protocol/CrystalRidge.h
 **/
UINT32
EFIAPI
GetNumNvmDimms (
  VOID
  )
{
  UINT8       Socket;
  UINT8       Ch;
  UINT8       Dimm;
  UINT8       MaxChDdr;
  UINT32      NumNgnDimms = 0;

  MaxChDdr = GetMaxChDdr ();

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if (mSystemMemoryMap->Socket[Socket].ChannelInfo[Ch].Enabled == 0) {
        continue;
      }
      // Get the dimmNvList to see if any of them are NVDIMMs
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (IsNvmDimm(Socket, Ch, Dimm)) {
          NumNgnDimms += 1;
        }
      }
    }
  }
  return (NumNgnDimms);
}


/**
  This function returns Platform Config Data Area size.
  Currently this is not implemented in NVMCTLR F/W and hence this
  call is not implemented. However, we will need to revisit this
  when the F/W start to support this call.

  @param Socket          - Socket index at system level
  @param Ch              - Channel index at socket level
  @param Dimm            - DIMM slot index within DDR channel
  @param PartitionId     - Partition ID for which the PCD size is needed
  @param *Size           - Pointer to return the size data.

  @return EFI status of the operation
**/
EFI_STATUS
GetPlatformConfigDataAreaSize (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     PartitionId,
  UINT32    *Size
  )
{
  EFI_STATUS                  Status;
  NVDIMM                      *DimmPtr;
  FIS_GET_PLATFORM_CONFIG_REQ Req;
  FIS_GET_PLATFORM_CONFIG_RSP Rsp;

  DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED);
  if (DimmPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  ZeroMem (&Req, sizeof (Req));
  Req.Bits.PartitionId = PartitionId;
  Req.Bits.InSmallPayload = TRUE;
  Req.Bits.JustSize = TRUE;

  if (GetMediaStatus (DimmPtr) == MEDIA_READY) {

    Status = FisRequest (Socket, Ch, Dimm, &Req.DWord[0], sizeof (Req), &Rsp.DWord[0], GET_PLATFORM_CONFIG, 0, NULL);
    *Size = Rsp.Bits.PartitionSize;

  } else {

    Status = EFI_DEVICE_ERROR;
    *Size = 0;
  }
  return Status;
}


/**
 CR interface function, see documentation in Protocol/CrystalRidge.h
 **/
EFI_STATUS
EFIAPI
GetPlatformConfigDataAreaData (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     *OutputBuffer,
  UINT32    *OutputSize,
  UINT32    Offset,
  UINT8     PartitionId,
  BOOLEAN   UseLargePayload
  )
{
  EFI_STATUS                  Status;
  NVDIMM                      *NvmDimm = NULL;
  FIS_GET_PLATFORM_CONFIG_REQ Req;
  UINT32                      Size;
  UINT32                      OutputPayload[NUM_PAYLOAD_REG];

  NvmDimm = GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED);

  if (NvmDimm == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Find the size of the partition to be read.
  Status = GetPlatformConfigDataAreaSize (Socket, Ch, Dimm, PartitionId, &Size);

  if (EFI_ERROR(Status)) {
    return Status;
  }

  if ((UINT64) Offset + (UINT64) *OutputSize > (UINT64) Size) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&Req, sizeof (Req));
  Req.Bits.PartitionId = PartitionId;
  Req.Bits.InSmallPayload = !UseLargePayload;
  Req.Bits.JustSize = FALSE;

  if (UseLargePayload == USE_SMALL_PAYLOAD) {

    Req.Bits.Offset = Offset;
    if (*OutputSize > sizeof (OutputPayload)) {

      *OutputSize = sizeof (OutputPayload);
    }
  }

  if (GetMediaStatus (NvmDimm) == MEDIA_READY) {

    Status = FisRequest (Socket, Ch, Dimm, &Req.DWord[0], sizeof (Req), OutputPayload, GET_PLATFORM_CONFIG, 0, NULL);

  } else {

    Status = EFI_DEVICE_ERROR;
  }

  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (!UseLargePayload) {

    CopyMem (OutputBuffer, &OutputPayload[0], *OutputSize);

  } else {
    //
    // if Offset > 0, we don't want the whole PCD
    //
    if (Offset > Size) {
      Offset = Size;
    }
    Size = Size - Offset;

    if (*OutputSize < Size) {
      Size = *OutputSize;
    } else {
      *OutputSize = Size;
    }

    Status = CopyFromLargePayloadToBuffer (Socket, Ch, Dimm, Offset, OutputBuffer, 0, Size, LARGE_PAYLOAD_OUTPUT);
    ASSERT_EFI_ERROR(Status);
  }
  return Status;
}


/**
 CR interface function, see documentation in Protocol/CrystalRidge.h
 **/
EFI_STATUS
EFIAPI
SetPlatformConfigDataAreaData (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     *InputBuffer,
  UINT32    InputSize,
  UINT32    Offset,
  UINT8     PartitionId,
  BOOLEAN   UseLargePayload
  )
{
  EFI_STATUS                  Status;
  NVDIMM                      *NvmDimm = NULL;
  FIS_SET_PLATFORM_CONFIG_REQ Data;
  UINT32                      Size = 0;

  NvmDimm = GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED);

  if (NvmDimm == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // for large payload we need to get full PCD contents first
  if (UseLargePayload == USE_LARGE_PAYLOAD) {
    //Find the size of the partition to be read.
    Status = GetPlatformConfigDataAreaSize (Socket, Ch, Dimm, PartitionId, &Size);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    if ((UINT64) Offset + (UINT64) InputSize > (UINT64) Size) {
      return EFI_INVALID_PARAMETER;
    }

    Status = GetPlatformConfigDataAreaData (Socket, Ch, Dimm, mLargePayloadBuffer, &Size, 0, PartitionId, USE_LARGE_PAYLOAD);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  ZeroMem (&Data, sizeof (Data));

  Data.Bits.PartitionId = PartitionId;
  Data.Bits.InSmallPayload = !UseLargePayload;

  if (!UseLargePayload) {

    if (InputSize > sizeof (Data.Bits.Data)) {

      return EFI_INVALID_PARAMETER;
    }
    Data.Bits.Offset = Offset;
    CopyMem (&Data.Bits.Data[0], InputBuffer, InputSize);

  } else {
    //
    // Copy input data
    //
    CopyMem (mLargePayloadBuffer + Offset, InputBuffer, InputSize);

    // Copy data from the large payload buffer to the Large Input Payload
    Status = CopyFromBufferToLargePayload (mLargePayloadBuffer, 0, Socket, Ch, Dimm, 0, Size, LARGE_PAYLOAD_INPUT);
    ASSERT_EFI_ERROR (Status);
    if (!mUseWpqFlush) {
      Status = CopyFromLargePayloadToBuffer (Socket, Ch, Dimm, 0, mLargePayloadBuffer, 0, Size, LARGE_PAYLOAD_INPUT);
    }
  }
  //
  // Send the command and return Status.
  //
  if (GetMediaStatus(NvmDimm) == MEDIA_READY) {

    Status = FisRequest (Socket, Ch, Dimm, &Data.DWord[0], sizeof (Data), NULL, SET_PLATFORM_CONFIG, FIS_DEFTIMEOUT, NULL);

  } else {

    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

/**
  Routine Description: This function returns Error info or Error Log Data by sending
  this command to the NVMCTLR Controller.

  @param[in]  Socket          - Socket index at system level
  @param[in]  Ch              - Channel index at socket level
  @param[in]  Dimm            - DIMM slot index within DDR channel
  @param[in]  LogLevel        - Log levels Low & High Priority logs
  @param[in]  LogType         - Log Type Media & Thermal logs
  @param[in]  LogInfo         - Log information or log data
  @param[in]  Sequence        - sequence number from which to get logs, 0 for oldest available
  @param[in]  RequestCount    - how many logs to retrieve
  @param[out] ErrLog          - pointer to the buffer to which Error log entries
                                copied after successful command completion
  @param[in]  ErrLogSize      - Media/Thermal log data size

  @return Status              - Status of the operation
**/
EFI_STATUS
SendNvmCtrlGetErrorLog (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     LogLevel,
  IN  UINT8     LogType,
  IN  UINT8     LogInfo,
  IN  UINT16    Sequence,
  IN  UINT16    RequestCount,
  OUT VOID      *ErrLog,
  IN  UINT32    ErrLogSize
  )
{
  EFI_STATUS             Status;
  UINT32                 EntrySize;
  UINT32                 LogSize;
  FIS_GET_ERROR_LOG_REQ  Data;
  UINT32                 OutPayload[NUM_PAYLOAD_REG];

  if ((ErrLog  == NULL) || (LogLevel > HIGH_LEVEL_LOG) ||
     (LogInfo  > RETRIEVE_LOG_INFO) ||  (LogType  > THERMAL_LOG_REQ)) {
    return EFI_INVALID_PARAMETER;
  }

  EntrySize = (LogType == MEDIA_LOG_REQ) ? sizeof(MEDIA_ERR_LOG) : sizeof(THERMAL_ERR_LOG);

  if (LogInfo == RETRIEVE_LOG_INFO) {
    LogSize = sizeof (ERR_LOG_INFO_DATA);
  } else {
    LogSize = ERROR_LOG_HEAD_SIZE + EntrySize * RequestCount;
  }
  if (LogSize > CR_OUT_PAYLOAD_SIZE) {
    return EFI_OUT_OF_RESOURCES;
  }
  if (LogSize > ErrLogSize) {
    return EFI_BUFFER_TOO_SMALL;
  }

  ZeroMem (&Data, sizeof (Data));
  Data.Bits.LogLevel   = LogLevel;
  Data.Bits.LogType    = LogType;
  Data.Bits.LogInfo    = LogInfo;
  Data.Bits.LogPayload   = USE_SMALL_PAYLOAD;
  Data.Bits.SequenceNumber = Sequence;
  Data.Bits.RequestCount = RequestCount;

  Status = FisSmbusRequest (Socket, Ch, Dimm, (UINT32*)&Data, sizeof (Data), &OutPayload[0], GET_ERROR_LOG, 0, NULL);
  if (!EFI_ERROR(Status)) {
    CopyMem (ErrLog, &OutPayload[0], LogSize);
  }

  return Status;
}

/**

  Routine Description: This function returns Error Log Data by sending
  this command to the NVMCTLR Controller.

  @param[in]  Socket          - Socket index at system level
  @param[in]  Ch              - Channel index at socket level
  @param[in]  Dimm            - DIMM slot index within DDR channel
  @param[in]  LogLevel        - Log levels Low & High Priority logs
  @param[in]  LogType         - Log Type Media & Thermal logs
  @param[in]  Count           - Number of log entries to read
  @param[out] ErrLog          - pointer to the buffer to which Error log entries
                                copied after successful command completion

  @return Status              - Status of the operation

**/
EFI_STATUS
EFIAPI
NvmCtrlGetErrorLogSmb (
  IN  UINT8         Socket,
  IN  UINT8         Ch,
  IN  UINT8         Dimm,
  IN  UINT8         LogLevel,
  IN  UINT8         LogType,
  IN  UINT8         Count,
  OUT ERR_LOG_DATA  *ErrLog
  )
{
  EFI_STATUS        Status;
  UINT16            SequenceNumber;
  UINT16            RequestCount;
  ERR_LOG_INFO_DATA ErrLogInfoData;

  //
  // Send first command to get log info
  //
  Status = SendNvmCtrlGetErrorLog (Socket, Ch, Dimm, LogLevel, LogType, RETRIEVE_LOG_INFO, 0, 0, (VOID *) &ErrLogInfoData, sizeof (ErrLogInfoData));
  if (EFI_ERROR(Status)) {
    CRDEBUG ((DEBUG_ERROR, "NvmCtrlGetErrorLogSmb failed: %r. No error log info has been retreived.", Status));
    return Status;
  }

  //
  // Determine starting sequence number and entry count for error log to return
  //
  RequestCount = 1;
  SequenceNumber = ErrLogInfoData.CurrentSequenceNumber;

  while ((RequestCount < Count) && (RequestCount < ErrLogInfoData.MaxLogEntries) &&
        (SequenceNumber != ErrLogInfoData.OldestSequenceNumber)) {
    RequestCount++;
    SequenceNumber--;
    if (SequenceNumber == 0) {
      // Sequence number wraps from 0xFFFF to 1
      SequenceNumber = 0xFFFF;
    }
  }

  //
  // Send second command to get log entries
  //
  Status = SendNvmCtrlGetErrorLog (Socket, Ch, Dimm, LogLevel, LogType, RETRIEVE_LOG_DATA, SequenceNumber, RequestCount, ErrLog, sizeof (*ErrLog));
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Crystal Ridge TSOD Polling settings.

  @param[in]     Socket          - Socket index at system level
  @param[in]     Imc             - Imc index at socket level
  @param[in]     PollState      - Enable TSOD polling.(0x00-disable, 0x01-Enable)
  @param[out]    SaveState      - Preserve polling state if provided.

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
NvmCtrlEnableDisablePolling (
  IN     UINT8           Socket,
  IN     UINT8           Imc,
  IN     UINT8           PollState,
  OUT    UINT8  * CONST  SaveState
  )
{
  return NvdimmSmbusSetPollingState (Socket, Imc, PollState, SaveState);
}

/**
  @brief Provide platform interpreted LSS reason enum.

  @param[in]  Skt                     - Socket index at system level
  @param[in]  Ch                      - Channel index at socket level
  @param[in]  Dimm                    - DIMM slot index within DDR channel
  @param[out] InterpretedLssPtr       - Buffer for interpreted LSS type.
  @param[out] InterpretedLssReasonPtr - Buffer for interpreted LSS reason.

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
NvmCtlrGetPlatformInterpretedLss (
  IN  UINT8       Skt,
  IN  UINT8       Ch,
  IN  UINT8       Dimm,
  OUT NVM_INTERPRETED_LSS        *InterpretedLssPtr,
  OUT NVM_INTERPRETED_LSS_REASON *InterpretedLssReasonPtr
  )
{
  UINT8 MaxChDdr;
  UINT8 ImcCh;
  UINT8 Imc;

  MaxChDdr = GetMaxChDdr ();
  ImcCh = ConvertSktChToImcCh (Ch);
  Imc = ConvertSktChToImc (Ch);

  CRDEBUG ((DEBUG_ERROR, CR_WARN_STR ("CRP") "EFI_CRYSTAL_RIDGE_PROTOCOL.NvmCtlrGetPlatformInterpretedLss"));
  CRDEBUG ((DEBUG_ERROR, " is obsolete, use EFI_PMem_PROTOCOL.DcpmmGetPlatformInterpretedLss\n"));
  if (Skt >= MAX_SOCKET || Ch >= MaxChDdr || Dimm >= MAX_DIMM) {

    return EFI_INVALID_PARAMETER;
  }
  return DcpmmGetPlatformInterpretedLss (
    NFIT_HANDLE_BUILD (Skt, Imc, ImcCh, Dimm),
    InterpretedLssPtr,
    InterpretedLssReasonPtr);
} // NvmCtlrGetPlatformInterpretedLss()


/**
  @brief Provide platform interpreted unlatched LSS reason enum.

  NOTE: Although this function shares NVM_INTERPRETED_LSS as the type of
        interpreted LSS value, it never returns the not latched values.
        Only NvmLssClean or NvmLssDirty can be returned.

  @param[in]  Skt                     - Socket index at system level
  @param[in]  Ch                      - Channel index at socket level
  @param[in]  Dimm                    - DIMM slot index within DDR channel
  @param[out] InterpretedLssPtr       - Buffer for interpreted LSS type.
  @param[out] InterpretedLssReasonPtr - Buffer for interpreted LSS reason.

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
NvmCtlrGetPlatformInterpretedUnlatchedDsc (
  IN  UINT8       Skt,
  IN  UINT8       Ch,
  IN  UINT8       Dimm,
  OUT NVM_INTERPRETED_LSS        *InterpretedLssPtr,
  OUT NVM_INTERPRETED_LSS_REASON *InterpretedLssReasonPtr
  )
{
  CRDEBUG ((DEBUG_ERROR, CR_WARN_STR ("CRP") "EFI_CRYSTAL_RIDGE_PROTOCOL.NvmCtlrGetPlatformInterpretedUnlatchedDsc"));
  CRDEBUG ((DEBUG_ERROR, " is not trustworthy, use EFI_PMem_PROTOCOL.DcpmmGetPlatformInterpretedLss\n"));

  return EFI_UNSUPPORTED;
} // NvmCtlrGetPlatformInterpretedUnlatchedDsc()

/**
 Notify Crystal Ridge driver of Host Alert Configuration

  @param[in]  HighPrioritySmi  TRUE: High Priority Alert SMI enabled
  @param[in]  LowPrioritySmi   TRUE: Low Priority Alert SMI enabled

**/
VOID
EFIAPI
NotifyHostAlertConfiguration (
  IN BOOLEAN HighPrioritySmi,
  IN BOOLEAN LowPrioritySmi
  )
{
  if (LowPrioritySmi) {
    mCrInfo.DdrtAlertSmiEn = 1;
  }
}

/**

  Updates Extended ADR (eADR/fADR) LSS state in the PMEM Modules

  @param[in]  LssState  - Extended ADR LSS state

  @return Standard EFI_STATUS is returned

**/
EFI_STATUS
EFIAPI
ExtAdrLssStateUpdate (
  IN UINT32 LssState
  )
{
  UINT8                                       Skt;
  UINT8                                       Ch;
  UINT8                                       Dimm;
  UINT8                                       MaxChDdr;
  NVDIMM                                      *NvmDimm;
  MB_SMM_CMD_FNV_SPA_MAPPED_0_STRUCT          OSNonceReg;
  VOLATILE MB_SMM_CMD_FNV_SPA_MAPPED_0_STRUCT *OSNonceRegPtr;
  CR_MAILBOX                                  *Mailbox;

  MaxChDdr = GetMaxChDdr ();

  //
  // Set eADR LSS state for all enabled DIMMs
  //
  for (Skt = 0; Skt < MAX_SOCKET; Skt++) {
    //
    // Skip disabled sockets
    //
    if (!mSystemMemoryMap->Socket[Skt].SocketEnabled) {
      continue;
    }
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      //
      // Skip disabled channels
      //
      if (!mSystemMemoryMap->Socket[Skt].ChannelInfo[Ch].Enabled) {
        continue;
      }
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        //
        // Skip if no DIMM or not NVDIMM
        //
        if (!mSystemMemoryMap->Socket[Skt].ChannelInfo[Ch].DimmInfo[Dimm].DcpmmPresent) {
          continue;
        }
        //
        // Get NVDIMM data
        //
        NvmDimm = GetDimm (&mCrInfo.NvdimmInfo, Skt, Ch, Dimm, NVDIMM_FLAG_ENABLED);
        if (NvmDimm != NULL) {
          Mailbox = GetMailbox (NvmDimm, TRUE);
          OSNonceRegPtr = (MB_SMM_CMD_FNV_SPA_MAPPED_0_STRUCT *)Mailbox->pNonce0;
          if (mUseWpqFlush) {
            IssueWpqFlush (NvmDimm, TRUE);
            OSNonceRegPtr->Data |= (UINT64)(LssState & CR_EADR_LSS_STATE_UPDATE_DATA_MASK);
            IssueWpqFlush (NvmDimm, TRUE);
          } else {
            OSNonceReg.Data = OSNonceRegPtr->Data;
            DurableCacheLineWrite ((UINTN)  OSNonceRegPtr, OSNonceReg.Data, sizeof (UINT64));
            OSNonceReg.Data |= (UINT64)(LssState & CR_EADR_LSS_STATE_UPDATE_DATA_MASK);
            DurableCacheLineWrite ((UINTN)  OSNonceRegPtr, OSNonceReg.Data, sizeof (UINT64));
          }
        }
      }
    }
  }

  return EFI_SUCCESS;
}

/**

  Enables/Disables Extended ADR (eADR/fADR) on PMEM Modules

  @param[in]  ExtAdrType  - Extended ADR type (eADR/fADR)
  @param[in]  State       - Extended ADR state (enabled/disabled)

  @return Standard EFI_STATUS is returned

**/
EFI_STATUS
EFIAPI
ExtAdrStateUpdate (
  IN EXT_ADR_TYPE   Type,
  IN EXT_ADR_STATE  State
  )
{
  EFI_STATUS  Status;
  UINT8       Skt;
  UINT8       Ch;
  UINT8       Dimm;
  UINT8       MaxChDdr;
  NVDIMM      *NvmDimm;
  UINT32      InputPayload[NUM_PAYLOAD_REG];
  EFI_GUID    ExtAdrStateUefiVariableGuid = CR_EXT_ADR_STATE_UEFI_VARIABLE_GUID;

  if (((Type != EXT_ADR_ENHANCED_ADR) && (Type != EXT_ADR_FAST_ADR)) ||
      ((State != EXT_ADR_ENABLED) && (State != EXT_ADR_DISABLED))){
    return EFI_INVALID_PARAMETER;
  }

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Setting Extended ADR (%s) state to %s\n",
    (Type == EXT_ADR_FAST_ADR) ? L"fADR" : L"eADR", (State == EXT_ADR_ENABLED) ? L"enable" : L"disabled"));

  ZeroMem (InputPayload, NUM_PAYLOAD_REG * sizeof (UINT32));
  InputPayload[0] = State;
  MaxChDdr = GetMaxChDdr ();

  //
  // Set eADR LSS state for all enabled DIMMs
  //
  for (Skt = 0; Skt < MAX_SOCKET; Skt++) {
    //
    // Skip disabled sockets
    //
    if (!mSystemMemoryMap->Socket[Skt].SocketEnabled) {
      continue;
    }
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      //
      // Skip disabled channels
      //
      if (!mSystemMemoryMap->Socket[Skt].ChannelInfo[Ch].Enabled) {
        continue;
      }
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        //
        // Skip if no DIMM or not NVDIMM
        //
        if (!mSystemMemoryMap->Socket[Skt].ChannelInfo[Ch].DimmInfo[Dimm].DcpmmPresent) {
          continue;
        }
        //
        // Get NVDIMM data
        //
        NvmDimm = GetDimm (&mCrInfo.NvdimmInfo, Skt, Ch, Dimm, NVDIMM_FLAG_ENABLED);
        if (NvmDimm != NULL) {
          Status = FisRequest ((UINT8)NvmDimm->SocketId, NvmDimm->Ch, NvmDimm->Dimm, InputPayload, sizeof (InputPayload), NULL, SET_EADR_STATE, FIS_DEFTIMEOUT, NULL);
          if (EFI_ERROR (Status)) {
            CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: Setting Extended ADR (%s) state to %s failed (%r)\n",
              NvmDimm->SocketId, NvmDimm->Ch, NvmDimm->Dimm,
              (Type == EXT_ADR_FAST_ADR) ? L"fADR" : L"eADR",
              (State == EXT_ADR_ENABLED) ? L"enable" : L"disabled",
              Status));
            return Status;
          }
        }
      }
    }
  }

  //
  // Set CR Extended ADR Data
  //
  mCrInfo.ExtAdrData.ExtAdrEnabled = (UINT8)State;
  mCrInfo.ExtAdrData.ExtAdrType = (UINT8)Type;

  //
  // Set CR Extended ADR UEFI variable
  //
  Status = gRT->SetVariable (CR_EXT_ADR_STATE_UEFI_VARIABLE_NAME, &ExtAdrStateUefiVariableGuid, EFI_VARIABLE_BOOTSERVICE_ACCESS, sizeof(EXT_ADR_STATE), &State);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "Setting Extended ADR (%s) state UEFI variable failed (%r)\n",
      (Type == EXT_ADR_FAST_ADR) ? L"fADR" : L"eADR", Status));
    return Status;
  }

  return EFI_SUCCESS;
}

/**

  Updates information about Extended ADR (eADR/fADR) enabled flows

  @param[in] Flows - Extended ADR type (eADR/fADR) map of enabled flows

  @return Standard EFI_STATUS is returned

**/

EFI_STATUS
EFIAPI
ExtAdrFlowsUpdate (
  IN UINT8 Flows
  )
{
  EFI_STATUS  Status;
  EFI_GUID    ExtAdrFlowsUefiVariableGuid = CR_EXT_ADR_FLOWS_UEFI_VARIABLE_GUID;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "Updating Extended ADR enabled flows map to 0x%x\n", Flows));

  //
  // Set CR Extended ADR Flows
  //
  mCrInfo.ExtAdrFlows.Flows = Flows;

  //
  // Set CR Extended ADR UEFI variable
  //
  Status = gRT->SetVariable (CR_EXT_ADR_FLOWS_UEFI_VARIABLE_NAME, &ExtAdrFlowsUefiVariableGuid, EFI_VARIABLE_BOOTSERVICE_ACCESS, sizeof(EXT_ADR_FLOWS), &mCrInfo.ExtAdrFlows);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "Setting Extended ADR flows UEFI variable failed (%r)\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}

/**

  Issues WPQ Flush for all iMCs

  @param None

  @return Standard EFI_STATUS is returned

**/
EFI_STATUS
EFIAPI
ImcWpqFlush (
  VOID
  )
{
  return IssueWpqFlushForAllImc ();
}

/**
  @brief This function installs Crystal Ridge protocol.

  Note that this function has two instances, SMM and DXE.

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
CrystalRidgeProtocolInstall (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS Status;
  EFI_HANDLE Handle = NULL;

  ZeroMem (&mCrystalRidgeProtocol, sizeof (mCrystalRidgeProtocol));
  //
  // Set current CRP revision
  //
  mCrystalRidgeProtocol.NvmProtocolVersion                = EFI_CRYSTAL_RIDGE_PROTOCOL_VERSION_CUR;
  //
  // NVDIMM host support functions
  //
  mCrystalRidgeProtocol.IsNvmDimm                         = IsNvmDimm;
  mCrystalRidgeProtocol.GetNumNvmDimms                    = GetNumNvmDimms;
  mCrystalRidgeProtocol.IsNvmAddress                      = IsNvmAddress;
  mCrystalRidgeProtocol.SpaToNvmDpa                       = SpaToNvmDpa;
  //
  // Support for ACPI tables building.
  //
  mCrystalRidgeProtocol.UpdateNvmAcpiTables               = UpdateNvmAcpiTables;
  mCrystalRidgeProtocol.UpdateNvmAcpiPcatTable            = UpdateNvmAcpiPcatTable;
  //
  // Long operation completion SMI handler.
  //
  mCrystalRidgeProtocol.NvmCtlrLongOpComplete             = NvmCtlrLongOpComplete;
  mCrystalRidgeProtocol.NvmCtlrLongOpCompleteGet          = NvmCtlrLongOpCompleteGet;
  mCrystalRidgeProtocol.NvmCtlrLongOpCompleteProcess      = NvmCtlrLongOpCompleteProcess;

  // NVM controller interface
  // Support for NvmController's config mem access
  mCrystalRidgeProtocol.NvmCtlrReadCfgMem                 = NvmCtlrReadCfgMem;
  mCrystalRidgeProtocol.NvmCtlrWriteCfgMem                = NvmCtlrWriteCfgMem;

  // Support for Error Logging
  mCrystalRidgeProtocol.NvmCtlrSetHostAlert               = NvmCtlrSetHostAlert;
  mCrystalRidgeProtocol.NvmCtlrGetHostAlert               = NvmCtlrGetHostAlert;
  mCrystalRidgeProtocol.NvmCtlrSetAddressRangeScrub       = NvmCtlrSetAddressRangeScrub;
  mCrystalRidgeProtocol.NvmCtlrGetAddressRangeScrub       = NvmCtlrGetAddressRangeScrub;
  mCrystalRidgeProtocol.NvmCtlrGetLatestErrorLog          = NvmCtlrGetLatestErrorLog;
  mCrystalRidgeProtocol.NvmCtlrGetErrorLog                = NvmCtlrGetErrorLog;

  // Support for Error Injection
  mCrystalRidgeProtocol.NvmCtlrEnableErrInjection         = NvmCtlrEnableErrInjection;
  mCrystalRidgeProtocol.NvmCtlrDisableErrInjection        = NvmCtlrDisableErrInjection;
  mCrystalRidgeProtocol.NvmCtlrPlatformEnableErrInjection = NvmCtlrPlatformEnableErrInjection;
  mCrystalRidgeProtocol.NvmCtlrInjectPoisonError          = NvmCtlrInjectPoisonError;

  mCrystalRidgeProtocol.NvmCtlrSetCfgDataPolicy           = NvmCtlrSetCfgDataPolicy;
  mCrystalRidgeProtocol.NvmCtlrGetCfgDataPolicy           = NvmCtlrGetCfgDataPolicy;

  // Support for PCD set/get used by external BNT tool - BIOS testing
  mCrystalRidgeProtocol.NvmCtlrGetPcd                     = GetPlatformConfigDataAreaData;
  mCrystalRidgeProtocol.NvmCtlrSetPcd                     = SetPlatformConfigDataAreaData;

  // Support for RSD function
  mCrystalRidgeProtocol.NvmCtlrSetCommandAccessPolicy     = NvmCtlrSetCommandAccessPolicy;

  // Support for SMM error handler
  mCrystalRidgeProtocol.IsSystemWithDcpmm                 = IsSystemWithDcpmm;

  mCrystalRidgeProtocol.AcpiClearLeafNotifications        = AcpiClearLeafNotifications;
  mCrystalRidgeProtocol.AcpiAddLeafNotification           = AcpiAddLeafNotification;
  mCrystalRidgeProtocol.AcpiLeafNotify                    = AcpiLeafNotify;

  mCrystalRidgeProtocol.AcpiClearRootNotification         = AcpiClearRootNotification;
  mCrystalRidgeProtocol.AcpiAddRootNotification           = AcpiAddRootNotification;
  mCrystalRidgeProtocol.AcpiAddToErrorList                = AcpiAddToErrorList;
  mCrystalRidgeProtocol.AcpiRootNotify                    = AcpiRootNotify;
  mCrystalRidgeProtocol.NvmCtlrGetErrorLogSmb             = NvmCtrlGetErrorLogSmb;
  mCrystalRidgeProtocol.NvmCtlrEnableDisablePolling       = NvmCtrlEnableDisablePolling;


  // Support for configuring Viral Policy (for FIS 1.12 and above)
  mCrystalRidgeProtocol.NvmCtlrSetViralPolicy             = NvmCtlrSetViralPolicy;
  mCrystalRidgeProtocol.NvmCtlrGetViralPolicy             = NvmCtlrGetViralPolicy;

  mCrystalRidgeProtocol.NvmCtlrGetPlatformInterpretedLss          = NvmCtlrGetPlatformInterpretedLss;
  mCrystalRidgeProtocol.NvmCtlrGetPlatformInterpretedUnlatchedDsc = NvmCtlrGetPlatformInterpretedUnlatchedDsc;

  // Retrieving the NVDIMM various device characteristics
  mCrystalRidgeProtocol.NvmCtlrGetDimmInfo                = NvmCtlrGetDimmInfo;

  mCrystalRidgeProtocol.NotifyHostAlertConfiguration      = NotifyHostAlertConfiguration;

  mCrystalRidgeProtocol.ExtAdrLssStateUpdate              = ExtAdrLssStateUpdate;
  mCrystalRidgeProtocol.ExtAdrStateUpdate                 = ExtAdrStateUpdate;

  mCrystalRidgeProtocol.ImcWpqFlush                       = ImcWpqFlush;

  mCrystalRidgeProtocol.ExtAdrFlowsUpdate                 = ExtAdrFlowsUpdate;

  if (mInSmm) {

    Status = mSmst->SmmInstallProtocolInterface (&Handle, &gEfiCrystalRidgeSmmGuid, EFI_NATIVE_INTERFACE, &mCrystalRidgeProtocol);

  } else {

    Status = gBS->InstallProtocolInterface (&Handle, &gEfiCrystalRidgeGuid, EFI_NATIVE_INTERFACE, &mCrystalRidgeProtocol);
  }
  if (EFI_ERROR (Status)) {

    CRDEBUG ((EFI_D_ERROR, CR_ERROR_STR ("CRP") "Cannot install Crystal Ridge protocol (%r)\n", Status));
  }
  return Status;
} // CrystalRidgeProtocolInstall()

