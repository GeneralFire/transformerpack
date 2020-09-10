/** @file
  MarginParam.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation. <BR>

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

#include "ssabios.h"
#include "MarginParam.h"
#include <Library/SsaUtilLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemoryEvContentLib.h>
#include <Library/DebugLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemorySsaLib.h>
#include "EvDebugPrint.h"

#define X4_STROBE_WIDTH  4
#define X8_STROBE_WIDTH  8

/**
  This function is used to determine if the given margin parameter requires
  an I/O reset after an error is detected.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       IoLevel            I/O level.
  @param[in]       MarginGroup        Margin group.

  @retval  TRUE   I/O reset is required.
  @retval  FALSE  I/O reset is not required.
**/
BOOLEAN IsIoResetAfterErrRequired(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN MRC_LT IoLevel,
  IN MRC_GT MarginGroup)
{
  BOOLEAN RetVal = FALSE;

  // JEDEC initialization should be preceded by I/O reset
  if (IsJedecInitAfterErrRequired(SsaServicesHandle, pSystemInfo, pMemCfg,
    pMemPointTestCfg, IoLevel, MarginGroup))
  {
    RetVal = TRUE;
  }
  else {
    switch (MarginGroup) {
      case RecEnDelay:
        RetVal = TRUE;
        break;
      default:
        break;
    }
  }
  return RetVal;
} // end function IsIoResetAfterErrRequired

/**
  This function is used to determine if the given margin parameter requires
  a JEDEC initialization after an error is detected.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       IoLevel            I/O level.
  @param[in]       MarginGroup        Margin group.

  @retval  TRUE   Incremental stepping is required.
  @retval  FALSE  Incremental stepping is not required.
**/
BOOLEAN IsJedecInitAfterErrRequired(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN MRC_LT IoLevel,
  IN MRC_GT MarginGroup)
{
  BOOLEAN RetVal = FALSE;
  UINT8 Socket, Controller, Channel, Dimm;
  MEMORY_TECHNOLOGY_TYPES MemoryTech;

  switch (MarginGroup) {
    case CmdAll:
    case CmdGrp0:
    case CmdGrp1:
    case CmdGrp2:
    case CmdGrp3:
    case CmdGrp4:
    case CmdGrp5:
    case CtlAll:
    case CtlGrp0:
    case CtlGrp1:
    case CtlGrp2:
    case CtlGrp3:
    case CtlGrp4:
    case CtlGrp5:
    case CmdCtlAll:
    case CkAll:
    case CmdVref:
    case AlertVref:
    case CmdRon:
    case ReqVref:
      RetVal = TRUE;
      break;
    case TxVref:
      // IF this is for the frontside
      if (IoLevel == DdrLevel) {
        // FOR each socket:
        for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
          // FOR each controller:
          for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
            // FOR each channel:
            for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
              // IF this channel has no lanes enabled for validation THEN skip it
              if (!IsChannelValEnabled(SsaServicesHandle, pSystemInfo, pMemCfg,
                pMemPointTestCfg, Socket, Controller, Channel)) {
                  continue;
              }

              for (Dimm = 0; Dimm < MAX_DIMM_CNT; Dimm++) {
                // IF this DIMM is not enabled for testing THEN skip it
#if SUPPORT_FOR_TURNAROUNDS
                if (!IsDimmATestingEnabled(SsaServicesHandle, pSystemInfo,
                  pMemCfg, pMemPointTestCfg, Socket, Controller, Channel,
                  Dimm) &&
                  ((pMemPointTestCfg->RankTestModes[Socket][Controller][Channel] != Turnarounds) ||
                  !IsDimmBTestingEnabled(SsaServicesHandle, pSystemInfo,
                  pMemCfg, pMemPointTestCfg, Socket, Controller, Channel,
                  Dimm))) {
                  continue;
                }
#else
                if (!IsDimmATestingEnabled(SsaServicesHandle, pSystemInfo,
                  pMemCfg, pMemPointTestCfg, Socket, Controller, Channel,
                  Dimm)) {
                  continue;
                }
#endif // !SUPPORT_FOR_TURNAROUNDS

                MemoryTech = pMemCfg->MemoryTech[Socket][Controller][Channel][Dimm];

                // IF the DIMM is not DDR3
                if (MemoryTech != SsaMemoryDdr3) {
                  RetVal = TRUE;
                  break;
                }
              } // end for each DIMM

              if (RetVal) {
                break;
              }
            } // end for each channel

            if (RetVal) {
              break;
            }
          } // end for each controller

          if (RetVal) {
            break;
          }
        } // end for each socket
      } // end if this is for the frontside
      break;
    default:
      break;
  }

  return RetVal;
} // end function IsJedecInitAfterErrRequired

/**
  This function is used to get strobe width for the given margin parameter and
  DIMM.

  Note that the logic used in this function is derived from the GetMaxStrobe
  function in the MemProjectSpecific.c file in the SKX BIOS code.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       IoLevel            I/O level.
  @param[in]       MarginGroup        Margin group.
  @param[in]       Socket             Zero-based Socket.
  @param[in]       Controller         Zero-based Controller.
  @param[in]       Channel            Zero-based Channel.
  @param[in]       Dimm               Zero-based DIMM.

  @retval  Strobe width.
**/
UINT8 GetMarginParamStrobeWidth(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MEM_CFG *pMemCfg,
  IN MRC_LT IoLevel,
  IN MRC_GT MarginGroup,
  IN UINT8 Socket,
  IN UINT8 Controller,
  IN UINT8 Channel,
  IN UINT8 Dimm)
{
  PSYSHOST Host;
  UINT8    MSVx4;

  Host = GetSysHostPointer ();
  MSVx4 = GetMaxStrobeValid(Host);

  if (GetMaxStrobe (Host, Socket, SSAGetChannelInSocket (Controller, Channel), Dimm, MarginGroup, IoLevel) == MSVx4) {
    return X4_STROBE_WIDTH;
  } else {
    return X8_STROBE_WIDTH;
  }

} // end function GetMarginParamStrobeWidth

  /**
    This function is used to determine if the given margin group is specific to
    DDR-T.

    @param[in]       MarginGroup        Margin group.

    @retval  TRUE   The margin group is specific to DDR-T.
    @retval  FALSE  The margin group is not specific to DDR-T.
  **/
BOOLEAN 
IsMarginGroupDdrTSpecific (
  IN MRC_GT MarginGroup)
{
  BOOLEAN IsDdrtSpecific = FALSE;

  if ((MarginGroup == EridDelay) ||
    (MarginGroup == EridVref) ||
    (MarginGroup == ErrorVref) ||
    (MarginGroup == ReqVref)) {
    IsDdrtSpecific = TRUE;
  }

  return IsDdrtSpecific;
} // end function IsMarginGroupDdrTSpecific

  /**
    This function is used to determine if the given margin group is for
    Early-Read-ID.

    @param[in]       MarginGroup        Margin group.

    @retval  TRUE   The margin group is for Early-Read-ID.
    @retval  FALSE  The margin group is not for Early-Read-ID.
  **/
BOOLEAN 
IsMarginGroupForErid (
  IN MRC_GT MarginGroup)
{
  BOOLEAN IsForErid = FALSE;

  if ((MarginGroup == EridDelay) ||
    (MarginGroup == EridVref)) {
    IsForErid = TRUE;
  }

  return IsForErid;
} // end function IsMarginGroupForErid

  /**
  @brief
  This function gets the given margin parameter's specificity.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       IoLevel            I/O level.
  @param[in]       MarginGroup        Margin group.
  @param[in]       LineNumber         Line number for error reporting.
  @param[out]      pSpecificity       Pointer to where margin parameter specificity will be placed.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
  **/
UINT8
GetMarginParamSpecificity (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MRC_LT IoLevel,
  IN MRC_GT MarginGroup,
  IN UINT32 LineNumber,
  OUT MARGIN_PARAM_SPECIFICITY *pSpecificity,
  OUT TEST_STATUS *pTestStat
)
{
  SSA_STATUS RetStat;

  EvDebugPrint (SDBG_INFO, BSSA_API_DEBUG_MSGS,
    "GetMarginParamSpecificity(IoLevel=%u MarginGroup=%u)\n",
    IoLevel, MarginGroup);

  RetStat = SsaServicesHandle->SsaMemoryConfig->GetMarginParamSpecificity (
    SsaServicesHandle, IoLevel, MarginGroup, pSpecificity);

  if (RetStat) {
    RcDebugPrint (SDBG_ERROR,
      "ERROR: IoLevel=%u MarginGroup=%u not supported.\n",
      IoLevel,
      MarginGroup);
    ProcError (SsaServicesHandle, RetStat, __FILE__, LineNumber, pTestStat);
    return 1;
  }

  EvDebugPrint (SDBG_INFO, BSSA_API_DEBUG_MSGS,
    "  Specificity=0x%X (", *pSpecificity);
  if (IsCheckMsgTypeEnabled (BSSA_API_DEBUG_MSGS)) {
    LogSpecificity (*pSpecificity);
  }
  EvDebugPrint (SDBG_INFO, BSSA_API_DEBUG_MSGS, ")\n");

  return 0;
} // end function GetMarginParamSpecificity
// end file MarginParam.c
