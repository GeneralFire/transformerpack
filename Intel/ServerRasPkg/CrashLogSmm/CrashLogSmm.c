/** @file
  SMM driver for CrashLog support.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2018 Intel Corporation. <BR>

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

#include <PiSmm.h>
#include <IndustryStandard/Acpi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/ErrorReportLib.h>
#include <Library/HobLib.h>
#include <Library/PlatPolicyLib.h>
#include <Library/PmcLib.h>
#include <Library/RasDebugLib.h>
#include <Library/WheaSupportLib.h>
#include "CrashLogSmm.h"
#include <Library/SystemInfoLib.h>
#include <Guid/CrashLog.h>

STATIC BOOLEAN                       mPmcCrashLogPresent = FALSE;
STATIC BOOLEAN                       mCpuCrashLogPresent = FALSE;
STATIC UINT8                         *mPmcCrashLogDataBuffer = NULL;
STATIC UINT32                        mPmcCrashLogSize = 0;

/**
  Get the build flag to identify if it is legacy crash log which is supported on ICL and CDF

  @retval TRUE            It is legacy crashlog
  @retval FALSE           It is converged crashlog
**/
BOOLEAN
IsLegacyCrashLog (
  VOID
)
{
  BOOLEAN      Legacy = FALSE;

  //
  // only CDF support legacy crashlog in server platform
  //
#ifdef CDF_SC_FLAG
 #ifndef EBG_SC_FLAG
  Legacy = TRUE;
 #endif
#endif //#ifdef CDF_SC_FLAG
 return Legacy;
}
/**
  Get PMC, PUNIT, CPU CrashLog, and initialize the APEI BERT GENERIC_ERROR_STATUS structure

  @param[in]  BertAddress             BERT boot error region address.
  @param[in]  CrashLogDataBuffer      Crash Log Data Buffer.
  @param[in]  CrashLogSize            Crash Log Data Buffer Size.
  @param[in]  ErrorType               The firmware-specific error type.
  @param[in]  RecordIdGuid            The record ID GUID.

  @retval EFI_SUCCESS             The function completes successfully.
  @retval EFI_NOT_FOUND           The BERT address is zero.
  @retval EFI_NOT_FOUND           The CrashLog data buffer has a version of 0.
  @retval EFI_OUT_OF_RESOURCES    The error record could not be allocated.
  @retval EFI_UNSUPPORTED         The BERT boot error region is full.
**/
EFI_STATUS
GenFwBootErrorlog (
  IN UINT64       BertAddress,
  IN UINT8        *CrashLogDataBuffer,
  IN UINT32       CrashLogSize,
  IN UINT8        ErrorType,
  IN EFI_GUID     *RecordIdGuid
  )
{
  EFI_STATUS                     Status;
  UINT64                         RecordId;
  CRASHLOG_FIELD                 *CrashLogField;
  CRASHLOG_RECORD_HEADER_COMMON  *CrashlogRecHdr;

  if (BertAddress == 0) {
    return EFI_NOT_FOUND;
  }

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "GenFwBootErrorlog - Start to generate GEDE.\n"));

  if (IsLegacyCrashLog ()) {
    //
    // Get the version of each component
    //
    CrashLogField = (CRASHLOG_FIELD *) CrashLogDataBuffer;
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "CrashLogVersion = 0x%x\n", CrashLogField->CrashLogVersion));
    if (CrashLogField->CrashLogVersion == 0) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "GenFwBootErrorlog - CrashLogVersion is incorrect! \n"));
      return EFI_NOT_FOUND;
    }
    RecordId = LShiftU64 (CrashLogField->CrashLogVersion, 32) | 0;
  } else {
    CrashlogRecHdr = (CRASHLOG_RECORD_HEADER_COMMON *) CrashLogDataBuffer;
    RecordId = LShiftU64 (CrashlogRecHdr->RecordVersion.Fields.ProductId, 32) | 0;
  }

  Status = ReportFirmwareBootErrorLog (
    ErrorType,
    EFI_ACPI_6_1_ERROR_SEVERITY_FATAL,
    RecordId,
    RecordIdGuid,
    CrashLogDataBuffer,
    CrashLogSize,
    BertAddress
    );

  if ((EFI_ERROR (Status))) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "GenFwBootErrorlog -  fail to generate GEDE!\n"));
  } else {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "GenFwBootErrorlog -  End of GEDE Generated.\n"));
  }
  return Status;
}

/**
  Log firmware boot error log in APEI BERT.

  @param[in]  CrashLogDataBuffer      Crash Log Data Buffer.
  @param[in]  CrashLogSize            Crash Log Data Buffer Size.
  @param[in]  ErrorType               The firmware-specific error type.
  @param[in]  RecordIdGuid            The record ID GUID.

  @retval EFI_SUCCESS             The function completes successfully.
  @retval EFI_NOT_FOUND           The BERT address cannot be found.
**/
EFI_STATUS
LogFwBootErrorlog (
  IN UINT8        *CrashLogDataBuffer,
  IN UINT32       CrashLogSize,
  IN UINT8        ErrorType,
  IN EFI_GUID     *RecordIdGuid
  )
{
  EFI_STATUS                     Status = EFI_NOT_FOUND;
  UINT64                         BertAddress;

  GetBERTErrorAddress (&BertAddress);
  if (BertAddress == 0) {
    return EFI_NOT_FOUND;
  }

  Status = GenFwBootErrorlog (BertAddress, CrashLogDataBuffer, CrashLogSize, ErrorType, RecordIdGuid);

  return Status;
}

/**
  Discover CrashLog.

  @retval TRUE    CrashLog is enabled and data is available.
  @retval FALSE   CrashLog is disabled or no data is available.
**/
BOOLEAN
DiscoverCrashLog (
  VOID
  )
{
  EFI_STATUS                      Status;
  PMC_IPC_DISCOVERY_BUF           DiscoveryBuffer;

  Status = EFI_SUCCESS;
  ZeroMem (&DiscoveryBuffer, sizeof (PMC_IPC_DISCOVERY_BUF));

  //
  // PCH CrashLog Discovery
  //
  Status = PmcCrashLogDiscovery (&DiscoveryBuffer);

  RAS_DEBUG ((
    LEVEL_BASIC_FLOW,
    "PCH CrashLog Discovery Value = 0x%x, Status = %r, Avail = %d, Dis = %d\n",
    DiscoveryBuffer.Uint32,
    Status,
    DiscoveryBuffer.Bits.Avail,
    DiscoveryBuffer.Bits.Dis
    ));

  if (EFI_ERROR (Status) || (DiscoveryBuffer.Bits.Avail != 1) || (DiscoveryBuffer.Bits.Dis == 1)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "PCH CrashLog feature not supported\n"));
    return FALSE;
  }

  //
  // Update CrashLog Size on PMC SSRAM
  //
  mPmcCrashLogSize = (DiscoveryBuffer.Bits.Size != 0) ? (DiscoveryBuffer.Bits.Size * sizeof (UINT32)) : 0xC00;
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "PMC CrashLog Size = 0x%x\n", mPmcCrashLogSize));

  return TRUE;
}

/**
  Entry point of the CrashLog support driver.

  @param[in]  ImageHandle   A handle for the image that is initializing this driver
  @param[in]  SystemTable   A pointer to the EFI system table

  @retval EFI_SUCCESS               Driver initialized successfully.
  @retval EFI_LOAD_ERROR            Failed to Initialize or to Load the driver.
  @retval EFI_OUT_OF_RESOURCES      Could not allocate needed resources.
**/
EFI_STATUS
EFIAPI
InstallCrashLogSupport (
  IN EFI_HANDLE               ImageHandle,
  IN EFI_SYSTEM_TABLE         *SystemTable
  )
{
  EFI_STATUS                           Status;
  EFI_HOB_GUID_TYPE                    *CrashLogDataBufferHob;
  UINT8                                *LogDataBuffer = NULL;
  UINT32                               LogDataSize = 0;
  UINT8                                SocId;
  UINT8                                i;
  CPU_CRASHLOG_RECORD_REGION_PROTOCOL  *CpuCrashLogProt;
  PCH_CRASHLOG_RECORD_REGION_PROTOCOL  *PchCrashLogProt;
  RAS_POLICY                           *RasPolicy;
  CPU_CSR_ACCESS_VAR                   *CpuCsrAccessVarPtr;


  RAS_DEBUG ((LEVEL_FUNC_FLOW, "CrashLog Entry Point\n"));

  RasPolicy = GetRasPolicy ();
  ASSERT (RasPolicy != NULL);
  if (RasPolicy == NULL || RasPolicy->CrashLogFeature == 0) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "CrashLog is disabled by policy\n"));
    return EFI_UNSUPPORTED;
  }

  if (IsLegacyCrashLog ()) {
    //
    // This path is only for CDF, which is legacy crashlog supported on Jacobsville
    //

    if (RasPolicy->CrashLogOnAllReset) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "CrashLog on All Reset\n"));
      //
      // Collect crash data on every boot
      //
      Status = PmcCrashLogOnAllReset ();
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "PMC IPC to collect CrashLog on every reset successful\n"));
    }

    //
    // Check for the PMC CrashLog feature, if it is available or not.
    //
    if (!DiscoverCrashLog ()) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "CrashLog is disabled/unavailable in PMC HW\n"));
      return EFI_UNSUPPORTED;
    }

    //
    // Get PMC Crash log region from HOB
    //
    CrashLogDataBufferHob = GetFirstGuidHob (&gPmcCrashLogDataBufferHobGuid);
    if (CrashLogDataBufferHob != NULL) {
      mPmcCrashLogDataBuffer = (UINT8 *) *((UINTN *) GET_GUID_HOB_DATA (CrashLogDataBufferHob));
    }
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "PmcCrashLogDataBuffer = 0x%p\n", mPmcCrashLogDataBuffer));

    //
    // Publish PMC Crash Log Data to BERT Table
    //
    if (mPmcCrashLogDataBuffer) {
      DEBUG ((DEBUG_ERROR, "PMC CrashLog data is present \n"));
      Status = LogFwBootErrorlog (mPmcCrashLogDataBuffer,
                                  mPmcCrashLogSize,
                                  EFI_FIRMWARE_ERROR_TYPE_SOC_TYPE1,
                                  NULL
                                  );
      if (EFI_ERROR (Status)) {
        RAS_DEBUG ((LEVEL_BASIC_FLOW, "Failed to log PMC crash data to BERT\n"));
      }
      mPmcCrashLogPresent = TRUE;
    } else {
      DEBUG ((DEBUG_ERROR, "PMC CrashLog data is not present \n"));
      mPmcCrashLogPresent = FALSE;
    }
  } else { //if (IsLegacyCrashLog ())
    //
    // Get CPU Crash log region from protocol
    //
    Status = gBS->LocateProtocol(&gCpuCrashLogRecordRegionProtocolGuid, NULL, &CpuCrashLogProt);
    if (EFI_ERROR (Status)) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "Failed to locate CpuCrashLogRecordRegionProtocol !\n"));
      CpuCrashLogProt = NULL;
    }

    if (CpuCrashLogProt != NULL) {
      CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();
      for (SocId = 0; SocId < MAX_SOCKET; SocId++) {
        if (!(CpuCsrAccessVarPtr->socketPresentBitMap & (1 << SocId))) {
          continue;
        }

        for (i = 0; i < MAX_CPU_CRASHLOG_REGIONS; i++) {
          LogDataBuffer = (UINT8*) (CpuCrashLogProt->RegionBuffer[SocId][i].Address);
          LogDataSize = CpuCrashLogProt->RegionBuffer[SocId][i].Size;
          RAS_DEBUG ((LEVEL_FUNC_FLOW, "Socket 0x%X CPU CrashLog Entry %d LogDataBuffer = 0x%lx\n", SocId, i, LogDataBuffer));
          RAS_DEBUG ((LEVEL_FUNC_FLOW, "Socket 0x%X CPU CrashLog Entry %d LogDataSize = 0x%x\n", SocId, i, LogDataSize));
          //
          // Publish CPU Crash Log Data to BERT Table
          //
          if (LogDataSize) {
            RAS_DEBUG ((LEVEL_BASIC_FLOW, "Socket 0x%X CPU CrashLog Entry %d data is present\n", SocId, i));
            Status = LogFwBootErrorlog ( LogDataBuffer,
                                        LogDataSize,
                                        EFI_FIRMWARE_ERROR_TYPE_SOC_TYPE2,
                                        &gCrashLogRecordIdGuid
                                        );
            if (EFI_ERROR (Status)) {
              RAS_DEBUG ((LEVEL_BASIC_FLOW, "Failed to log to BERT\n"));
            }
            mCpuCrashLogPresent = TRUE;
          } else {
            RAS_DEBUG ((LEVEL_FUNC_FLOW, "Socket 0x%X CPU CrashLog entry %d data is not present\n", SocId, i));
          }
        }
      }
    } //if (CpuCrashLogProt != NULL)

    //
    // Get PCH Crash log region from protocol
    //
    Status = gBS->LocateProtocol(&gPchCrashLogRecordRegionProtocolGuid, NULL, &PchCrashLogProt);
    if (EFI_ERROR (Status)) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "Failed to locate PchCrashLogRecordRegionProtocol !\n"));
      PchCrashLogProt = NULL;
    }

    if (PchCrashLogProt != NULL) {
      for (i = 0; i < MAX_PCH_CRASHLOG_REGIONS; i++) {
        if (PchCrashLogProt->RegionBuffer[i].Size == 0) {
          break;
        }
        LogDataBuffer = (UINT8*) (PchCrashLogProt->RegionBuffer[i].Address);
        LogDataSize = PchCrashLogProt->RegionBuffer[i].Size;
        RAS_DEBUG ((LEVEL_FUNC_FLOW, "PCH CrashLog LogDataBuffer[0x%X] = 0x%x\n", i, LogDataBuffer));
        RAS_DEBUG ((LEVEL_FUNC_FLOW, "PCH CrashLog LogDataSize[0x%X] = 0x%x\n", i, LogDataSize));

        //
        // Publish PCH Crash Log Data to BERT Table
        //
        if (LogDataSize) {
          Status = LogFwBootErrorlog ( LogDataBuffer,
                                      LogDataSize,
                                      EFI_FIRMWARE_ERROR_TYPE_SOC_TYPE2,
                                      &gCrashLogRecordIdGuid
                                      );
          if (EFI_ERROR (Status)) {
            RAS_DEBUG ((LEVEL_BASIC_FLOW, "Failed to log to BERT\n"));
          }
          mPmcCrashLogPresent = TRUE;
        }
      }
    } //if (PchCrashLogProt != NULL)
  }//if (IsLegacyCrashLog ())

  if ((mCpuCrashLogPresent == FALSE) && (mPmcCrashLogPresent == FALSE)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "CrashLog is not present. Skip BERT creation \n"));
    return EFI_NOT_READY;
  }

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "CrashLog Exit\n"));
  return EFI_SUCCESS;
}
