/** @file
  SetMarginParam.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2020 Intel Corporation. <BR>

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


#include "ChkRetStat.h"
#include "MemPointTest.h"
#include "MarginParam.h"
#include <Library/MemoryEvContentLib.h>
#include "SetMarginParamResultMetadata.h"
#include "SetMarginParamResultColumns.h"
#include "SetMarginParam.h"

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/SsaUtilLib.h>
#include <Library/MemoryCoreLib.h>


// flag to enable BIOS-SSA debug messages
#define ENBL_BIOS_SSA_API_DEBUG_MSGS (0)

// flag to enable function trace debug messages
#define ENBL_FUNCTION_TRACE_DEBUG_MSGS (0)

// flag to enable verbose log messages
#define ENBL_VERBOSE_LOG_MSGS (0)

// define a data structure to make it easy to iterate the configure setting.
#pragma pack (push, 1)
typedef struct MARGIN_PARAM_CONFIG {
  MRC_LT IoLevel;
  MRC_GT MarginGroup;
  INT16  Offset;
}MARGIN_PARAM_CONFIG;
#pragma pack (pop)

/**
@brief
  This function sets up the results.  It creates the results buffer and gets a
  pointer to the metadata structure.  It also zeros the metadata space and sets
  the version information metadata values.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[out]      ppMetadata         Pointer to pointer to results metadata structure.
**/
UINT8 SetupSetMarginParamResults(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  OUT SET_MARGIN_PARAM_RESULT_METADATA **ppMetadata)
{
  SSA_STATUS RetStat;
  UINT32 ResultEleCnt = 1;
  UINT32 MetadataSize = sizeof(SET_MARGIN_PARAM_RESULT_METADATA);
  UINT32 ResultElementSize = sizeof(SET_MARGIN_PARAM_RESULT_COLUMNS);
  EFI_GUID MetadataGuid = SET_MARGIN_PARAM_RESULT_METADATA_GUID;
  EFI_GUID ColumnsGuid = SET_MARGIN_PARAM_RESULT_COLUMNS_GUID;
  SET_MARGIN_PARAM_RESULT_METADATA *pMetadata;
  TEST_STATUS *pTestStat;
  SET_MARGIN_PARAM_RESULT_COLUMNS *pElement;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "SetupResults(ResultEleCnt=%d)\n", ResultEleCnt);
#endif

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "CreateResultBuffer()\n"
    "  MetadataSize=%u ResultElementSize=%u ResultsCapacity=%u\n",
    MetadataSize, ResultElementSize, ResultEleCnt);
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->CreateResultBuffer(
    SsaServicesHandle, MetadataSize, ResultElementSize, ResultEleCnt);
  if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__, NULL)) {
    return 1;
  }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "GetMetadata()\n");
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->GetMetadata(
    SsaServicesHandle, ppMetadata);
  if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__, NULL)) {
    return 1;
  }

  pMetadata = *ppMetadata;

  // zero the metadata
  ZeroMem (pMetadata, sizeof(*pMetadata));

  // set the version information
  pMetadata->MajorVer = BSSA_TC_MAJOR_VER;
  pMetadata->MinorVer = BSSA_TC_MINOR_VER;
  pMetadata->RevVer = BSSA_TC_REV_VER;
  pMetadata->BuildVer = BSSA_TC_BUILD_VER;

  pTestStat = (TEST_STATUS*) &pMetadata->StatusCode;

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "SetMetadataType()\n");
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->SetMetadataType(
    SsaServicesHandle, &MetadataGuid);
  if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__, pTestStat)) {
    return 1;
  }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "SetResultElementType()\n");
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->SetResultElementType(
    SsaServicesHandle, &ColumnsGuid);
  if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__, pTestStat)) {
    return 1;
  }

  // we need to process the one element so that the columnar GUID shows up in the result
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "GetNextResultElement()\n");
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->GetNextResultElement(
    SsaServicesHandle, &pElement);
  if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__, pTestStat)) {
    return 1;
  }

  // just set the element values to all 0's
  ZeroMem (pElement, sizeof(*pElement));

  return 0;
} // end function SetupSetMarginParamResults

/**
@brief
  This function gets the given margin parameter's minimum and maximum offset
  values.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       IoLevel            I/O level.
  @param[in]       MarginGroup        Margin group.
  @param[in]       Specificity        Margin parameter specificity.
  @param[out]      pMinOffset         Pointer to where minimum offset value will be placed.
  @param[out]      pMaxOffset         Pointer to where maximum offset value will be placed.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 GetMarginParamMinMax(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN MRC_LT IoLevel,
  IN MRC_GT MarginGroup,
  IN MARGIN_PARAM_SPECIFICITY Specificity,
  OUT INT16 *pMinOffset,
  OUT INT16 *pMaxOffset,
  OUT TEST_STATUS *pTestStat)
{
  SSA_STATUS RetStat;
  UINT8 Socket, Controller, Channel, Dimm, Rank;
  UINT8 RankCount;
  UINT8 LaneBitmasks[MAX_BUS_WIDTH / 8];
  INT16 TmpMinOffset, TmpMaxOffset;
  UINT16 SettleDelay;
  UINT16 StepUnit;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "GetMarginParamMinMax(IoLevel=%d MarginGroup=%u Specificity=%u)\n",
    IoLevel, MarginGroup, Specificity);
#endif

  *pMinOffset = -30000; // initialize to large negative value
  *pMaxOffset = 30000; // initialize to large positive value

  // FOR each socket
  for (Socket = 0; Socket < pSystemInfo->MaxNumberSockets; Socket++) {
    // FOR each controller:
    for (Controller = 0; Controller < pSystemInfo->MaxNumberControllers; Controller++) {
      // FOR each channel:
      for (Channel = 0; Channel < pSystemInfo->MaxNumberChannels; Channel++) {
        // IF this channel has no lanes enabled for validation THEN skip it
        if (!IsChannelValEnabled(SsaServicesHandle, pSystemInfo, pMemCfg,
          pMemPointTestCfg, Socket, Controller, Channel)) {
          continue;
        }

        // FOR each DIMM:
        for (Dimm = 0; Dimm < pSystemInfo->MaxNumberDimms; Dimm++){
          // IF this DIMM is not enabled for testing THEN skip it
          if (!IsDimmATestingEnabled(SsaServicesHandle, pSystemInfo, pMemCfg,
            pMemPointTestCfg, Socket, Controller, Channel, Dimm)) {
            continue;
          }

          RankCount = pMemCfg->RankCounts[Socket][Controller][Channel][Dimm];

          // FOR each rank:
          for (Rank = 0; Rank < RankCount; Rank++) {
            // IF this rank is not enabled for testing THEN skip it
            if (!IsRankATestingEnabled(SsaServicesHandle, pSystemInfo, pMemCfg,
              pMemPointTestCfg, Socket, Controller, Channel, Dimm, Rank)) {
              continue;
            }

            ZeroMem (LaneBitmasks, sizeof(LaneBitmasks));
            CopyMem (LaneBitmasks,
              pMemPointTestCfg->LaneValBitmasks[Controller][Channel],
              (pSystemInfo->BusWidth / 8));

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
            NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
              "GetMarginParamLimits(\n"
              "  Socket=%u Controller=%u Channel=%u Dimm=%u Rank=%u\n"
              "  LaneBitmasks=0x",
              Socket, Controller, Channel, Dimm, Rank);
            LogByteArrayAsHex(
              pMemPointTestCfg->LaneValBitmasks[Controller][Channel],
              (pSystemInfo->BusWidth / 8));
            NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
              " IoLevel=%u MarginGroup=%u)\n", IoLevel, MarginGroup);
#endif
            RetStat = SsaServicesHandle->SsaMemoryConfig->GetMarginParamLimits(
              SsaServicesHandle, Socket, Controller, Channel, Dimm, Rank,
              LaneBitmasks, IoLevel, MarginGroup,
              &TmpMinOffset, &TmpMaxOffset, &SettleDelay, &StepUnit);
            if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__, pTestStat)) {
              return 1;
            }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
            NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
              "  TmpMinOffset=%d TmpMaxOffset=%d TmpSettleDelay=%u\n",
              TmpMinOffset, TmpMaxOffset, SettleDelay);
#endif
            // check for new min-offset and max-offset values
            if (TmpMinOffset > *pMinOffset) {
              *pMinOffset = TmpMinOffset;
            }

            if (TmpMaxOffset < *pMaxOffset) {
              *pMaxOffset = TmpMaxOffset;
            }

            // IF the margin parameter is not rank specific THEN exit the rank for loop
            if ((Specificity & RankSpecific) == 0) {
              break;
            }
          } // end for each rank

          // IF the margin parameter is not rank specific THEN exit the DIMM for loop
          if ((Specificity & RankSpecific) == 0) {
            break;
          }
        } // end for each DIMM

        // IF the margin parameter is not channel specific THEN exit the channel for loop
        if ((Specificity & ChannelSpecific) == 0) {
          break;
        }
      } // end for each channel

      // IF the margin parameter is not controller specific THEN exit the controller for loop
      if ((Specificity & ControllerSpecific) == 0) {
        break;
      }
    } // end for each controller

    // IF the margin parameter is not socket specific THEN exit the socket for loop
    if ((Specificity & SocketSpecific) == 0) {
      break;
    }
  } // end for each socket

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "  MinOffset=%d MaxOffset=%d\n",
    *pMinOffset, *pMaxOffset);
#endif

  return 0;
} // end function GetMarginParamMinMax

/**
@brief
  This function checks the given margin parameters offset's range.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       IoLevel            I/O level.
  @param[in]       MarginGroup        Margin group.
  @param[in]       Specificity        Margin parameter specificity.
  @param[in]       Offset             Offset from current/trained value.
  @param[in]       LineNumber         Line number for error reporting.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 ChkMarginParamOffset(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN MRC_LT IoLevel,
  IN MRC_GT MarginGroup,
  IN MARGIN_PARAM_SPECIFICITY Specificity,
  IN INT16 Offset,
  IN UINT32 LineNumber,
  OUT TEST_STATUS *pTestStat)
{
  INT16 MinOffset;
  INT16 MaxOffset;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "ChkMarginParamOffset(IoLevel=%d MarginGroup=%u Specificity=0x%x Offset=%d)\n",
    IoLevel, MarginGroup, Specificity, Offset);
#endif

  if (GetMarginParamMinMax(SsaServicesHandle, pSystemInfo, pMemCfg,
    pMemPointTestCfg, IoLevel, MarginGroup, Specificity, &MinOffset,
    &MaxOffset, pTestStat)) {
    return 1;
  }

  if ((Offset < MinOffset) || (Offset > MaxOffset)) {
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "Offset (%d) out of range (%d <= val <= %d)\n",
      Offset, MinOffset, MaxOffset);
    ProcError(SsaServicesHandle, UnsupportedValue, __FILE__, LineNumber, pTestStat);
    return 1;
  }

  return 0;
} // end function ChkMarginParamOffset

/**
@brief
  This function checks the given margin parameter's configuration for
  correctness/consistency.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       Offset             Margin parameter offset.
  @param[in]       IoLevel            I/O level.
  @param[in]       MarginGroup        Margin group.
  @param[in]       LineNumber         Line number for error reporting.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 ChkMarginParam(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN INT16 Offset,
  IN MRC_LT IoLevel,
  IN MRC_GT MarginGroup,
  IN UINT32 LineNumber,
  OUT TEST_STATUS *pTestStat)
{
  MARGIN_PARAM_SPECIFICITY Specificity;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "ChkMarginParam(Offset=%d IoLevel=%d MarginGroup=%u)\n",
    Offset, IoLevel, MarginGroup);
#endif

  if (GetMarginParamSpecificity(SsaServicesHandle, IoLevel, MarginGroup,
    LineNumber, &Specificity, pTestStat)) {
    return 1;
  }

  if (ChkMarginParamOffset(SsaServicesHandle, pSystemInfo, pMemCfg,
    pMemPointTestCfg, IoLevel, MarginGroup, Specificity, Offset, LineNumber,
    pTestStat)) {
    return 1;
  }

  return 0;
} // end function ChkMarginParam

/**
@brief
  This function init the param parameter.
  It iterate through the test configuration and determine whether need to call
  the initMarginParam API to deNormalize or not. 
  It is also used to Normalize after SetMarginParams()

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       pTestCfg           Pointer to test configuration structure.
  @param[in]       SetupCleanup       Flag to indicate if the initMarginParam is Setup or Cleanup
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 InitMarginParams(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST SET_MARGIN_PARAM_CONFIG *pTestCfg,
  IN CONST SETUP_CLEANUP SetupCleanup,
  OUT TEST_STATUS *pTestStat)
{
  MRC_GT MarginParamGroup, DdrLevelMp;
  MARGIN_PARAM_CONFIG* pMarginParamConfig;
  UINT8 Socket;
  UINT8 i;
  BOOLEAN HasCalledInitMarginParamApiForDdrLevel;
  BOOLEAN HasCalledInitMarginParamApiForLrbufLevel;
  SETUP_CLEANUP CurrentSetupCleanup, LastSetupCleanup;
  SSA_STATUS RetStat;
  CHAR8 *DdrLevelGroupString = NULL;
  CHAR8 *LrbufLevelGroupString = NULL;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "InitMarginParams( SetupCleanup:%u)\n", SetupCleanup);
#endif

  CurrentSetupCleanup = SetupCleanup;
  LastSetupCleanup = CurrentSetupCleanup;
  HasCalledInitMarginParamApiForDdrLevel = FALSE;
  HasCalledInitMarginParamApiForLrbufLevel = FALSE;
  DdrLevelMp = MrcGtMax;

  // Get the first configuration entry
  pMarginParamConfig = (MARGIN_PARAM_CONFIG*)(&(pTestCfg->IoLevel));

  // Check all Ddrlevel margin parameters
  for (i = 0; i < pTestCfg->ParameterCnt; i++ ) {
    
    if (pMarginParamConfig->IoLevel != DdrLevel) {
      continue;
    }

    MarginParamGroup = pTestCfg->MarginGroup;
    if (CheckCMDGroup (MarginParamGroup) || CheckCTLGroup (MarginParamGroup)) {
        
      if (!HasCalledInitMarginParamApiForDdrLevel){ 
        // FOR each socket:
        for (Socket = 0; Socket < pSystemInfo->MaxNumberSockets; Socket++) {
          if (!(pMemPointTestCfg->SocketBitmask & (0x1 << Socket))) continue;

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "InitMarginParam(Socket=%u IoLevel=%u MarginGroup=%u SetupCleanup=0x%x)\n",
            Socket, DdrLevel, MarginParamGroup, CurrentSetupCleanup);
#endif
          RetStat = SsaServicesHandle->SsaMemoryConfig->InitMarginParam(
            SsaServicesHandle, Socket, DdrLevel, MarginParamGroup, CurrentSetupCleanup);
          if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__, pTestStat)) {
            return 1;
          }
        } // Socket
        HasCalledInitMarginParamApiForDdrLevel = TRUE;
        DdrLevelMp = MarginParamGroup; // save it for error checking message
      }
    }
    pMarginParamConfig++;
  }

  // Check all margin groups in LrbufLevel
  pMarginParamConfig = (MARGIN_PARAM_CONFIG*)(&(pTestCfg->IoLevel));

  for (i = 0; i < pTestCfg->ParameterCnt; i++) {
    if (pMarginParamConfig->IoLevel != LrbufLevel) {
      continue;
    }
    CurrentSetupCleanup = SetupCleanup;

    MarginParamGroup = pTestCfg->MarginGroup;
    if (CheckCMDGroup (MarginParamGroup) || CheckCTLGroup (MarginParamGroup)) {
      // we can not offset the front and backside Cmd/CmdVref/Ctl at the same time
      // because they require different deNormalization. 
      if (HasCalledInitMarginParamApiForDdrLevel){
        if (EFI_ERROR (GetGroupString (DdrLevelMp, &DdrLevelGroupString))) {
          DdrLevelGroupString = "Unknown group";
        }
        if (EFI_ERROR (GetGroupString (MarginParamGroup, &LrbufLevelGroupString))) {
          LrbufLevelGroupString = "Unknown group";
        }
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "DdrLevel Cmd/Ctlmargin parameter:%s can NOT be offset simultaneously with any LrbufLevel Cmd/Ctl"
          "margin parameter:%s "
          "please adjust them separately.\n",
          DdrLevelGroupString, LrbufLevelGroupString);
        return 1;
      }
        // Add 0x1 in the bit 1 to indicate that the offset is applied for the positive direction.
        // only negative direction requires backside shift.
      if ( pMarginParamConfig->Offset > 0 ) {
        CurrentSetupCleanup |= (0x1 << 1);
      }
      if (!HasCalledInitMarginParamApiForLrbufLevel){
        // FOR each socket:
        for (Socket = 0; Socket < pSystemInfo->MaxNumberSockets; Socket++) {
          if (!(pMemPointTestCfg->SocketBitmask & (0x1 << Socket))) continue;
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "InitMarginParam(Socket=%u IoLevel=%u MarginGroup=%u SetupCleanup=0x%x)\n",
            Socket, LrbufLevel, MarginParamGroup, CurrentSetupCleanup);
#endif
          RetStat = SsaServicesHandle->SsaMemoryConfig->InitMarginParam(
            SsaServicesHandle, Socket, LrbufLevel, MarginParamGroup, CurrentSetupCleanup);
          if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__, pTestStat)) {
            return 1;
          }
        } // Socket
        HasCalledInitMarginParamApiForLrbufLevel = TRUE;
        LastSetupCleanup = CurrentSetupCleanup;
      } else {
        // we already call the initMarginParam API. we will not call it again. 
        // but we want to perform error checking to ensure that there is no conflict 
        // SetupCleanup value.
        if (CurrentSetupCleanup != LastSetupCleanup){
          if (EFI_ERROR (GetGroupString (MarginParamGroup, &LrbufLevelGroupString))) {
            LrbufLevelGroupString = "Unknown group";
          }
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "The setting: LrbufLevel %s Offset=%d conflict with other LrbufLevel margin parameter setting, "
            "please adjust them separately.\n"
            "LrbufLevel CmdAll, CmdVref and CtlAll can NOT be set in opposite direction ",
            LrbufLevelGroupString, pMarginParamConfig->Offset);
          return 1;
        }
      }
    }
    pMarginParamConfig++;
  } // i

  return 0;
} // end function InitMarginParams


/**
@brief
  This function checks the test configuration for correctness/consistency.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pTestCfg           Pointer to test configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 ChkSetMarginParamTestCfg(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST SET_MARGIN_PARAM_CONFIG *pTestCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  OUT TEST_STATUS *pTestStat)
{
  MARGIN_PARAM_CONFIG* pMarginParamConfig;
  UINT8 i;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ChkTestCfg()\n");
#endif

  if (ChkMemPointTestCfg(SsaServicesHandle, pSystemInfo, pMemCfg,
    pMemPointTestCfg, pTestStat)) {
    return 1;
  }
  
  // Initialize margin parameters for "Setup" operation
  if (InitMarginParams(SsaServicesHandle, pSystemInfo, pMemPointTestCfg, pTestCfg, Setup, pTestStat)) {
      return 1;
  }

  // Get the first configuration entry
  pMarginParamConfig = (MARGIN_PARAM_CONFIG*)(&(pTestCfg->IoLevel));

  // Check all margin parameters
  for (i = 0; i < pTestCfg->ParameterCnt; i++) {
    if (ChkMarginParam(SsaServicesHandle, pSystemInfo, pMemCfg, pMemPointTestCfg,
      pMarginParamConfig->Offset, pMarginParamConfig->IoLevel, pMarginParamConfig->MarginGroup, 
      __LINE__, pTestStat)) {
        return 1;
    }
    pMarginParamConfig++;
  } 

  return 0;
} // end function ChkSetMarginParamTestCfg

/**
@brief
  This function logs the test configuration.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pTestCfg           Pointer to test configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.
**/
VOID LogSetMarginParamTestCfg(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST SET_MARGIN_PARAM_CONFIG *pTestCfg)
{
  MARGIN_PARAM_CONFIG* pMarginParamConfig;
  UINT8 i;
  CHAR8 *GroupString = NULL;

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "\n SetMarginParam Test Configuration:\n");
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  SocketBitMask=0x%X\n", pTestCfg->SocketBitMask);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  ControllerBitmasks=0x%X\n", pTestCfg->ControllerBitMask);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  ChannelBitMask=0x%X\n", pTestCfg->ChannelBitMask);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  DimmBitMask=0x%X\n", pTestCfg->DimmBitMask);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  RankBitMask=0x%X\n", pTestCfg->RankBitMask);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  BusWidth=%d\n", pTestCfg->BusWith);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  LaneBitmasks=0x");
  LogByteArrayAsHex (pTestCfg->LaneBitMask, sizeof(pTestCfg->LaneBitMask));
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "\n");

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  ParameterCnt=%d\n", pTestCfg->ParameterCnt);

  // Get the first configuration entry
  pMarginParamConfig = (MARGIN_PARAM_CONFIG*)(&(pTestCfg->IoLevel));

  for ( i = 0; i < pTestCfg->ParameterCnt; i++ ) {

    if (EFI_ERROR (GetGroupString (pMarginParamConfig->MarginGroup, &GroupString))) {
      GroupString = "Unknown group";
    }
    
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  Param:%d \n", i);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  IoLevel:%s \n", GetLevelStr (pMarginParamConfig->IoLevel));
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  MarginGroup:%d (%s) \n", pMarginParamConfig->MarginGroup, GroupString);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  Offset:%d \n", pMarginParamConfig->Offset);

    pMarginParamConfig++;
  } 

  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR, "\n");
} // end function LogSetMarginParamTestCfg

/**
@brief
  This function sets the given margin parameter to the given offset (if the
  given Enable is TRUE).

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       Offset             Margin parameter offset.
  @param[in]       IoLevel            I/O level.
  @param[in]       MarginGroup        Margin group.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 SetMarginParam(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN INT16 Offset,
  IN MRC_LT IoLevel,
  IN MRC_GT MarginGroup,
  OUT TEST_STATUS *pTestStat)
{
  UINT8 Socket, Controller, Channel, Dimm, Rank;
  UINT8 RankCount;
  UINT8 LaneBitmasks[MAX_BUS_WIDTH / 8];
  MARGIN_PARAM_SPECIFICITY Specificity;
  UINT16 CurrentOffset = 0;
  SSA_STATUS RetStat;
  CHAR8 *GroupString = NULL;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "SetMarginParam(Offset=%d IoLevel=%u MarginGroup=%u)\n",
    Offset, IoLevel, MarginGroup);
#endif

  if (GetMarginParamSpecificity(SsaServicesHandle, IoLevel, MarginGroup,
    __LINE__, &Specificity, pTestStat)) {
    return 1;
  }

  if (EFI_ERROR (GetGroupString (MarginGroup, &GroupString))) {
    GroupString = "Unknown group";
  }

  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "Setting IoLevel=%u (%s) MarginGroup=%u (%s) to Offset=%d.\n",
    IoLevel, GetLevelStr(IoLevel), MarginGroup, GroupString, Offset);


  // FOR each socket:
  for (Socket = 0; Socket < pSystemInfo->MaxNumberSockets; Socket++) {

    // FOR each controller:
    for (Controller = 0; Controller < pSystemInfo->MaxNumberControllers; Controller++) {
      // FOR each channel:
      for (Channel = 0; Channel < pSystemInfo->MaxNumberChannels; Channel++) {
        // IF this channel has no lanes enabled for validation THEN skip it
        if (!IsChannelValEnabled(SsaServicesHandle, pSystemInfo, pMemCfg,
          pMemPointTestCfg, Socket, Controller, Channel)) {
          continue;
        }

        // FOR each DIMM:
        for (Dimm = 0; Dimm < pSystemInfo->MaxNumberDimms; Dimm++){
          // IF this DIMM is not enabled for testing THEN skip it
          if (!IsDimmATestingEnabled(SsaServicesHandle, pSystemInfo, pMemCfg,
            pMemPointTestCfg, Socket, Controller, Channel, Dimm)) {
            continue;
          }

          RankCount = pMemCfg->RankCounts[Socket][Controller][Channel][Dimm];

          // FOR each rank:
          for (Rank = 0; Rank < RankCount; Rank++) {
            // IF this rank is not enabled for testing THEN skip it
            if (!IsRankATestingEnabled(SsaServicesHandle, pSystemInfo, pMemCfg,
              pMemPointTestCfg, Socket, Controller, Channel, Dimm, Rank)) {
              continue;
            }

            ZeroMem (LaneBitmasks, sizeof(LaneBitmasks));
            CopyMem(LaneBitmasks,
              pMemPointTestCfg->LaneValBitmasks[Controller][Channel],
              (pSystemInfo->BusWidth / 8));

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
            NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
              "  SetMarginParamOffset(\n"
              "    Socket=%u Controller=%u Channel=%u Dimm=%u Rank=%u\n"
              "    LaneBitmasks=0x",
              Socket, Controller, Channel, Dimm, Rank);
            LogByteArrayAsHex(
              pMemPointTestCfg->LaneValBitmasks[Controller][Channel],
              (pSystemInfo->BusWidth / 8));
            NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
              " IoLevel=%u MarginGroup=%u\n"
              "    CurrentOffset=%d NewOffset=%d)\n",
              IoLevel, MarginGroup, CurrentOffset, Offset);
#endif
            RetStat = SsaServicesHandle->SsaMemoryConfig->SetMarginParamOffset(
              SsaServicesHandle, Socket, Controller, Channel, Dimm, Rank, LaneBitmasks,
              IoLevel, MarginGroup, CurrentOffset, Offset);
            if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__, pTestStat)) {
              return 1;
            }

            // IF the margin parameter is not rank specific THEN exit the rank for loop
            if ((Specificity & RankSpecific) == 0) {
              break;
            }
          } // end for each rank

          // IF the margin parameter is not rank specific THEN exit the DIMM for loop
          if ((Specificity & RankSpecific) == 0) {
            break;
          }
        } // end for each DIMM

        // IF the margin parameter is not channel specific THEN exit the channel for loop
        if ((Specificity & ChannelSpecific) == 0) {
          break;
        }
      } // end for each channel

      // IF the margin parameter is not controller specific THEN exit the controller for loop
      if ((Specificity > ControllerSpecific) == 0) {
        break;
      }
    } // end for each controller

    // IF the margin parameter is not socket specific THEN exit the socket for loop
    if ((Specificity & SocketSpecific) == 0) {
      break;
    }

    // Don't do cleanup - it will undo the set operation!
//#if ENBL_BIOS_SSA_API_DEBUG_MSGS
//    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
//      "InitMarginParam(Socket=%u IoLevel=%u MarginGroup=%u SetupCleanup=Cleanup)\n",
//      Socket, IoLevel, MarginGroup);
//#endif
//    MarginGroup = GetMarginGroupOpcode(SsaServicesHandle, MarginGroup);
//    RetStat = SsaServicesHandle->SsaMemoryConfig->InitMarginParam(
//      SsaServicesHandle, Socket, IoLevel, MarginGroup, Cleanup);
//    if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__, pTestStat)) {
//      return 1;
//    }
  } // end for each socket

  return 0;
} // end function SetMarginParam

/**
@brief
  This function sets the requested margin parameter offsets.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pTestCfg           Pointer to test configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       CmdCtlSignals      Flag to set CmdCtl signals only. TRUE: only set CmdCtl signal offset. FALSE: onlt set non-CMdCtl signal offsets
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 SetMarginParams(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST SET_MARGIN_PARAM_CONFIG *pTestCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST BOOLEAN CmdCtlSignals, 
  OUT TEST_STATUS *pTestStat)
{
  MARGIN_PARAM_CONFIG* pMarginParamConfig;
  UINT8 i;
  BOOLEAN Apply;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "SetMarginParams()\n");
#endif

  // Get the first configuration entry
  pMarginParamConfig = (MARGIN_PARAM_CONFIG*)(&(pTestCfg->IoLevel));

  // Set all margin parameters
  for ( i = 0; i < pTestCfg->ParameterCnt; i++) {

    Apply = TRUE;

    if (((CheckCMDGroup (pMarginParamConfig->MarginGroup) || CheckCTLGroup (pMarginParamConfig->MarginGroup)) && !CmdCtlSignals) ||
      (!(CheckCMDGroup (pMarginParamConfig->MarginGroup) || CheckCTLGroup (pMarginParamConfig->MarginGroup)) && CmdCtlSignals)) {
        Apply = FALSE;
    }
    
    if (Apply) {
      if (SetMarginParam(SsaServicesHandle, pSystemInfo, pMemCfg, pMemPointTestCfg,
        pMarginParamConfig->Offset, pMarginParamConfig->IoLevel, pMarginParamConfig->MarginGroup, pTestStat)) {
          return 1;
      }
    } // Apply
    pMarginParamConfig++;
  } 

  return 0;
} // end function SetMarginParams

/**
  This function is used to initialize the memory point test configuration
  structure based on the given test configuration.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 InitMemPointTestCfg(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST SET_MARGIN_PARAM_CONFIG* pTestCfg,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  OUT MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  OUT TEST_STATUS *pTestStat)
{
  UINT8 Socket, Controller, Channel, Dimm;
  BOOLEAN IsSocketToBeTested;
  BOOLEAN IsControllerToBeTested;
  BOOLEAN IsChannelToBeTested;
  BOOLEAN IsDimmToBeTested;
  BOOLEAN IsAnyChannelToBeTested = FALSE;

  // start by setting everything to 0
  ZeroMem (pMemPointTestCfg, sizeof(*pMemPointTestCfg));

  // IF the all devices value is used
  if (pTestCfg->SocketBitMask != ALL_POPULATED_SOCKETS) {
    if ((pSystemInfo->SocketBitMask & pTestCfg->SocketBitMask) !=
      pTestCfg->SocketBitMask) {
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "ERROR: Requested SocketBitMask (0x%X) is not available (0x%X).\n",
        pTestCfg->SocketBitMask, pSystemInfo->SocketBitMask);
      ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pTestStat);
      return 1;
    }
  }

  // use the given socket bitmask and screen out unpopulated sockets
  pMemPointTestCfg->SocketBitmask = pTestCfg->SocketBitMask & \
    pSystemInfo->SocketBitMask;

  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    // IF this socket is not to be tested THEN skip it
    if ((pMemPointTestCfg->SocketBitmask & (1 << Socket)) == 0) {
      continue;
    }

    IsSocketToBeTested = FALSE;

    // IF the all devices value is used
    if (pTestCfg->ControllerBitMask != ALL_POPULATED_CONTROLLERS) {
      if ((pMemCfg->ControllerBitmasks[Socket] & pTestCfg->ControllerBitMask) !=
        pTestCfg->ControllerBitMask) {
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "ERROR: Socket:%d Requested ControllerBitMask (0x%X) is not available (0x%X).\n",
          Socket, pTestCfg->ControllerBitMask,
          pMemCfg->ControllerBitmasks[Socket]);
        ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pTestStat);
        return 1;
      }
    }

    // use the given controller bitmask and screen out unpopulated controllers
    pMemPointTestCfg->ControllerBitmasks[Socket] = \
      pTestCfg->ControllerBitMask & \
      pMemCfg->ControllerBitmasks[Socket];

    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      // IF this controller is not to be tested THEN skip it
      if ((pMemPointTestCfg->ControllerBitmasks[Socket] &
        (1 << Controller)) == 0) {
        continue;
      }

      IsControllerToBeTested = FALSE;

      // IF the all devices value is used
      if (pTestCfg->ChannelBitMask != ALL_POPULATED_CHANNELS) {
        if ((pMemCfg->ChannelBitmasks[Socket][Controller] &
          pTestCfg->ChannelBitMask) !=
          pTestCfg->ChannelBitMask) {
            NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
              "ERROR: Socket:%d Controller:%d Requested ChannelBitmasks (0x%X) is not available (0x%X).\n",
              Socket, Controller, pTestCfg->ChannelBitMask,
              pMemCfg->ChannelBitmasks[Socket][Controller]);
            ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pTestStat);
            return 1;
        }
      }

      // use the given channel bitmask and screen out unpopulated channels
      pMemPointTestCfg->ChannelBitmasks[Socket][Controller] = \
        pMemPointTestCfg->ChannelValBitmasks[Socket][Controller] = \
        pTestCfg->ChannelBitMask & \
        pMemCfg->ChannelBitmasks[Socket][Controller];

      // FOR each channel:
      for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
        // IF this channel is not to be tested THEN skip it
        if ((pMemPointTestCfg->ChannelBitmasks[Socket][Controller] &
          (1 << Channel)) == 0) {
          continue;
        }

        IsChannelToBeTested = FALSE;

        if (pTestCfg->DimmBitMask != ALL_POPULATED_DIMMS) {
          if ((pMemCfg->DimmBitmasks[Socket][Controller][Channel] &
            pTestCfg->DimmBitMask) != pTestCfg->DimmBitMask) {
            NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
              "ERROR: Socket:%d Controller:%d Channel:%d Requested DimmBitMask (0x%X) is not available (0x%X).\n",
              Socket, Controller, Channel,
              pTestCfg->DimmBitMask,
              pMemCfg->DimmBitmasks[Socket][Controller][Channel]);
            ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pTestStat);
            return 1;
          }
        }

        // use the given DIMM bitmask and screen out unpopulated DIMMs
        pMemPointTestCfg->DimmABitmasks[Socket][Controller][Channel] = \
          pTestCfg->DimmBitMask & pMemCfg->DimmBitmasks[Socket][Controller][Channel];

        // FOR each DIMM:
        for (Dimm = 0; Dimm < MAX_DIMM_CNT; Dimm++) {
          // IF this DIMM is not populated THEN skip it
          if ((pMemPointTestCfg->DimmABitmasks[Socket][Controller][Channel] &
            (1 << Dimm)) == 0) {
            continue;
          }

          if (pTestCfg->RankBitMask != ALL_POPULATED_RANKS) {
            if ((((1 << pMemCfg->RankCounts[Socket][Controller][Channel][Dimm]) - 1) &
              pTestCfg->RankBitMask) != pTestCfg->RankBitMask) {
              NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
                "ERROR: Socket:%d Controller:%d Channel:%d Dimm:%d Requested RankBitMask (0x%X) is not available (0x%X).\n",
                Socket, Controller, Channel, Dimm, 
                pTestCfg->RankBitMask,
                ((1 << pMemCfg->RankCounts[Socket][Controller][Channel][Dimm]) - 1));
              ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pTestStat);
              return 1;
            }
          }

          // use the given rank bitmask and screen out unpopulated ranks
          pMemPointTestCfg->RankABitmasks[Socket][Controller][Channel][Dimm] = \
            pTestCfg->RankBitMask & ((1 << pMemCfg->RankCounts[Socket][Controller][Channel][Dimm]) - 1);

          // IF there are ranks to test for this DIMM
          if (pMemPointTestCfg->RankABitmasks[Socket][Controller][Channel][Dimm]) {
            IsDimmToBeTested = TRUE;
            IsChannelToBeTested = TRUE;
            IsAnyChannelToBeTested = TRUE;
            IsControllerToBeTested = TRUE;
            IsSocketToBeTested = TRUE;
          }
          // ELSE it turns out that there were no ranks to test on this DIMM
          else {
            // remove the DIMM
            pMemPointTestCfg->DimmABitmasks[Socket][Controller][Channel] &= \
              ~(1 << Dimm);
          }
        } // for each DIMM

        // IF it turns out that there were no ranks to test on this channel
        if (!IsChannelToBeTested) {
          // remove the channel
          pMemPointTestCfg->ChannelBitmasks[Socket][Controller] &= \
            ~(1 << Channel);
        }
      } // for each channel

      // IF it turns out that there were no ranks to test on this controller
      if (!IsControllerToBeTested) {
        // remove the controller
        pMemPointTestCfg->ControllerBitmasks[Socket] &= \
          ~(1 << Controller);
      }
    } // for each controller

    // IF it turns out that there were no ranks to test on this socket
    if (!IsSocketToBeTested) {
      // remove the socket
      pMemPointTestCfg->SocketBitmask &= ~(1 << Socket);
    }
  } // end for each socket

  if (IsAnyChannelToBeTested) {
    CopyMem (pMemPointTestCfg->LaneValBitmasks, pTestCfg->LaneBitMask,
      sizeof (pTestCfg->LaneBitMask));
  }

  return 0;
} // end function InitMemPointTestCfg

/**
@brief
  The function is used to signal the EV module that checkpoint 'StatusCommand' has been reached.

  @param[in] PeiServices    - General purpose services available to every PEIM.
  @param[in] StatusCommand  - The current checkpoint value.
  @param[in] CheckpointData - General data related to the current checkpoint.
  
  @retval EFI_SUCCESS
          EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
SetMarginParamTestEntryPoint (
  SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  SET_MARGIN_PARAM_CONFIG *Config
  )
{
  SET_MARGIN_PARAM_CONFIG* pTestCfg = Config;
  MRC_SYSTEM_INFO SystemInfo;
  MEM_CFG MemCfg;
  MEM_POINT_TEST_CONFIG MemPointTestCfg;
  SET_MARGIN_PARAM_RESULT_METADATA *pMetadata = NULL;
  TEST_STATUS *pTestStat;
  UINT32 ExpectedRevision;
  BOOLEAN CmdCtlSignals; 

  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "Starting SetMarginParam test v%u.%u.%u.%u\n",
    BSSA_TC_MAJOR_VER, BSSA_TC_MINOR_VER, BSSA_TC_REV_VER, BSSA_TC_BUILD_VER);

  if (SetupSetMarginParamResults(SsaServicesHandle, &pMetadata)) {
    return EFI_UNSUPPORTED;
  }
  pTestStat = (TEST_STATUS*) &pMetadata->StatusCode;

  if (Config == NULL) {
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: Config is NULL!\n");
    return EFI_UNSUPPORTED;
  }

#ifdef SSA_REVISION_BIOS
  ExpectedRevision = SSA_REVISION_BIOS;
#else // must be rev 1.0
  ExpectedRevision = 0x01000000;
#endif
  if (SsaServicesHandle->Revision != ExpectedRevision) {
#if ENBL_VERBOSE_ERROR_MSGS
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: BIOS-SSA API revision (0x%08X) not expected value (0x%08X)\n",
      SsaServicesHandle->Revision, ExpectedRevision);
#endif
    ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pTestStat);
    return EFI_UNSUPPORTED;
  }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "GetSystemInfo()\n");
#endif
  // get system information
  SsaServicesHandle->SsaMemoryConfig->GetSystemInfo(SsaServicesHandle,
    &SystemInfo);
#if ENBL_VERBOSE_LOG_MSGS
  LogSystemInfo(SsaServicesHandle, &SystemInfo);
#endif

  // get memory configuration
  if (GetMemCfg(SsaServicesHandle, &SystemInfo, &MemCfg, pTestStat)) {
    return EFI_UNSUPPORTED;
  }
#if ENBL_VERBOSE_LOG_MSGS
  LogMemCfg(SsaServicesHandle, &SystemInfo, &MemCfg);
#endif

  if (InitMemPointTestCfg(SsaServicesHandle, pTestCfg, &SystemInfo, &MemCfg,
    &MemPointTestCfg, pTestStat)) {
    return EFI_UNSUPPORTED;
  }

#if ENBL_VERBOSE_LOG_MSGS
  LogMemPointTestCfg(SsaServicesHandle, &SystemInfo, &MemPointTestCfg);
#endif

#if ENBL_VERBOSE_LOG_MSGS
  LogSetMarginParamTestCfg (SsaServicesHandle, &SystemInfo, pTestCfg);
#endif
  // check the test configuration
  if (ChkSetMarginParamTestCfg(SsaServicesHandle, &SystemInfo, &MemCfg, pTestCfg,
    &MemPointTestCfg, pTestStat)) {
    return EFI_UNSUPPORTED;
  }

  // set the requested Cmd/Ctl margin parameters
  // NOTE: we need to separate Cmd/Ctl margin parameters because the DENORMALIZE/RENORMALIZE
  // retrain the system which wipe off non-CMdCtl signal setting. 
  CmdCtlSignals = TRUE;
  if (SetMarginParams(SsaServicesHandle, &SystemInfo, &MemCfg, pTestCfg,
    &MemPointTestCfg, CmdCtlSignals, pTestStat)) {
    return EFI_UNSUPPORTED;
  }

  // Initialize margin parameters for "Cleanup" operation, "Normalize" the Cmd/Ctl/CmdVref
  if (InitMarginParams(SsaServicesHandle, &SystemInfo, &MemPointTestCfg, pTestCfg, 
    Cleanup, pTestStat)) {
    return EFI_UNSUPPORTED;
  }

  // set the requested non-Cmd/Ctl margin parameters
  CmdCtlSignals = FALSE;
  if (SetMarginParams(SsaServicesHandle, &SystemInfo, &MemCfg, pTestCfg,
    &MemPointTestCfg, CmdCtlSignals, pTestStat)) {
      return EFI_UNSUPPORTED;
  }

  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "SetMarginParam test v%u.%u.%u.%u complete\n",
    BSSA_TC_MAJOR_VER, BSSA_TC_MINOR_VER, BSSA_TC_REV_VER, BSSA_TC_BUILD_VER);

  return EFI_SUCCESS;
} // end function SetMarginParamTestEntryPoint


// end file SetMarginParam.c
