/** @file
  Includes implementation for API calls of Memory Throttling\Power Management

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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
#include <Library/HbmCoreLib.h>
#include <Library/RcDebugLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemMcIpLib.h>

// HBM 2X refresh value
#define HBM_TWOXREFRESHVALUE 85

#define TEMP_LO_SPRHBM  93
#define TEMP_MID_SPRHBM 94
#define TEMP_HI_SPRHBM  95

//
// Temp (degree C)    Thrt_level%    BW allowed%    #instructions    THRT shift bits    THRT_*(#instructrions/2POWER(5))    THRT_* for 1DPC    THRT_* for 2DPC
// TEMP_HI = 95       90             10             354.76           5                  11                                  THRT_CRIT = 0x0B   THRT_CRIT = 0x05
// TEMP_MID = 94      60             40             1419.04          5                  44                                  THRT_HI   = 0x2C   THRT_HI   = 0x16
// TEMP_LOW = 93      20             80             2838.08          5                  88                                  THRT_MID  = 0x58   THRT_MID  = 0x2C
//
// SPRHBM use 1DPC setting
#define THRT_CRIT_SPRHBM 0x0B  // 90%
#define THRT_HI_SPRHBM   0x2C  // 60%
#define THRT_MID_SPRHBM  0x58  // 20%

/**

  Initialize HBM memory throttling

  @param[in] Socket   - Socket Number

  @retval SUCCESS

**/
UINT32
HbmInitThrottling (
  IN UINT8   Socket
  )
{
  UINT8                         HbmCh;
  UINT8                         Dimm;
  UINT16                        ThermalThrottlingOptions;
  THROTTLE_VALUE                ThrottleValue;
  SYS_SETUP                     *Setup;

  if (!IsHbmFlowEnabled (HF_THROTTLING)) {
    return SUCCESS;
  }

#ifdef DEBUG_CODE_BLOCK
  AcquirePrintControl ();
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "Hbm Initialize Throttling\n\n");
#endif // DEBUG_CODE_BLOCK

  Setup = GetSysSetupPointer ();
  ThermalThrottlingOptions = Setup->mem.thermalThrottlingOptions;

  HbmConfigureMemHotFeature (Socket);

  ZeroMem ((UINT8 *) &ThrottleValue, sizeof (THROTTLE_VALUE));

  ThrottleValue.OlttEnable = FALSE;

  ThrottleValue.TempThersholdLow = TEMP_LO_SPRHBM;
  ThrottleValue.TempThersholdMid = TEMP_MID_SPRHBM;
  ThrottleValue.TempThersholdHigh = TEMP_HI_SPRHBM;

  ThrottleValue.TempThrottleMid = THRT_MID_SPRHBM;
  ThrottleValue.TempThrottleHigh = THRT_HI_SPRHBM;
  ThrottleValue.TempThrottleCritical = THRT_CRIT_SPRHBM;

  if (!(ThermalThrottlingOptions & (CLTT_EN | CLTT_PECI_EN))) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "\n:MPM: ERROR: SPRHBM only support CLTT! ThermalThrottlingOptions = %d\n ", ThermalThrottlingOptions);
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "\n:MPM: HBM DimmTempThrottlingLimit  thrt_mid:%d,  thrt_hi:%d,  thrt_crit:%d\n ",
                          ThrottleValue.TempThrottleMid, ThrottleValue.TempThrottleHigh, ThrottleValue.TempThrottleCritical);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "\n:MPM: HBM DimmTempThrottling  temp_lo:%d,  temp_mid:%d,  temp_hi:%d\n ",
                          ThrottleValue.TempThersholdLow, ThrottleValue.TempThersholdMid, ThrottleValue.TempThersholdHigh);

  for (HbmCh = 0; HbmCh < GetMaxChHbm (); HbmCh++) {
    if (!IsHbmChannelEnabled (Socket, HbmCh)) {
      continue;
    }

    //
    // Disable OLTT
    //
    GetSetThrottleValues (MemTechHbm, Socket, HbmCh, NO_DIMM_OPTION, WRITE_THROTTLE_VALUES, EnumOlttEnable, &ThrottleValue);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      //
      // Initialize the temp thresholds and bandwidth limits
      //
      GetSetThrottleValues (MemTechHbm, Socket, HbmCh, Dimm, WRITE_THROTTLE_VALUES, EnumTempThersholdLow, &ThrottleValue);
      GetSetThrottleValues (MemTechHbm, Socket, HbmCh, Dimm, WRITE_THROTTLE_VALUES, EnumTempThersholdMid, &ThrottleValue);
      GetSetThrottleValues (MemTechHbm, Socket, HbmCh, Dimm, WRITE_THROTTLE_VALUES, EnumTempThersholdHigh, &ThrottleValue);

      GetSetThrottleValues (MemTechHbm, Socket, HbmCh, Dimm, WRITE_THROTTLE_VALUES, EnumTempThrottleMid, &ThrottleValue);
      GetSetThrottleValues (MemTechHbm, Socket, HbmCh, Dimm, WRITE_THROTTLE_VALUES, EnumTempThrottleHigh, &ThrottleValue);
      GetSetThrottleValues (MemTechHbm, Socket, HbmCh, Dimm, WRITE_THROTTLE_VALUES, EnumTempThrottleCritical, &ThrottleValue);
    } // Dimm loop
  } // HbmCh loop

#ifdef DEBUG_CODE_BLOCK
  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK

  return SUCCESS;
}

/**

  This API programs 2X refresh functionality for HBM in current Socket
  pointed by SysHost Structure. Based on setup option the function
  can enable\disable 2x refresh programming.

  @retval N/A

**/
VOID
EFIAPI
HbmProgramTemperatureRefresh (
  VOID
  )
{
  UINT8               Socket;
  UINT8               HbmCh;
  UINT8               Dimm;
  PSYSHOST            Host;
  THROTTLE_VALUE      ThrottleValue;
  SYS_SETUP           *Setup;

  Setup = GetSysSetupPointer ();
  Host = GetSysHostPointer();
  Socket = Host->var.mem.currentSocket;

  for (HbmCh = 0; HbmCh < GetMaxChHbm (); HbmCh++) {
    if (!IsHbmChannelEnabled (Socket, HbmCh)) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      // Disable halfX refresh
      ThrottleValue.HalfxRefresh = FALSE;
      GetSetThrottleValues (MemTechHbm, Socket, HbmCh, Dimm, WRITE_THROTTLE_VALUES, EnumHalfxRefresh, &ThrottleValue);

      // Disable 4X refresh
      ThrottleValue.FourxRefresh = FALSE;
      GetSetThrottleValues (MemTechHbm, Socket, HbmCh, Dimm, WRITE_THROTTLE_VALUES, EnumFourxRefresh, &ThrottleValue);

      //
      // Enable 2x refresh per request and Program 2X refresh value
      //
      if ((Setup->mem.options & ALLOW2XREF_EN) != 0) {
        ThrottleValue.TwoxRefresh = TRUE;
        ThrottleValue.TwoxRefreshValue = HBM_TWOXREFRESHVALUE;
        GetSetThrottleValues (MemTechHbm, Socket, HbmCh, Dimm, WRITE_THROTTLE_VALUES, EnumTwoxRefreshValue, &ThrottleValue);
        GetSetThrottleValues (MemTechHbm, Socket, HbmCh, Dimm, WRITE_THROTTLE_VALUES, EnumTwoxRefresh, &ThrottleValue);
      } else {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, HbmCh, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "\n:MPM: ERROR: SPRHBM not allow to enable 2X refresh\n");
      }
    } // Dimm
  } // HbmCh
}
