/** @file
  PCH ADR setup library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#ifndef _PCH_ADR_LIB_H_
#define _PCH_ADR_LIB_H_

#include <Register/PchRegsPmcCdf.h>

#define PCH_ADR_TIMER_0US        0
#define PCH_ADR_TIMER_25US      25
#define PCH_ADR_TIMER_50US      50
#define PCH_ADR_TIMER_100US    100
#define PCH_ADR_TIMER_400US    400
#define PCH_ADR_TIMER_600US    600
#define PCH_ADR_TIMER_800US    800
#define PCH_ADR_TIMER_1000US  1000
#define PCH_ADR_TIMER_1200US  1200
#define PCH_ADR_TIMER_1800US  1800
#define PCH_ADR_TIMER_2200US  2200
#define PCH_ADR_TIMER_2400US  2400
#define PCH_ADR_TIMER_3600US  3600
#define PCH_ADR_TIMER_7200US  7200
#define PCH_ADR_TIMER_9600US  9600

#define PM_SYNC_GPIO_B           0
#define PM_SYNC_GPIO_C           1

#define ADR_MULT_SETUP_DEFAULT_POR 0
#define ADR_TMR_SETUP_DEFAULT_POR 0

typedef enum {
  AdrSrcPwrBtnOvr, // Power Button Override
  AdrSrcMePwrBtnOvr, // ME-Initiated Power Button Override
  AdrSrcPmcParityError, // SoC PMC SUS RAM Parity Error
  AdrSrcSysPwrOk, // SYS_PWROK Failure
  AdrSrcPmcWtdTmr, // SoC PMC Firmware Watchdog Timer
  AdrSrcMeUncorrectableErr, // ME Uncorrectable Error
  AdrSrcMeWtdTmr, // ME Firmware Watchdog Timer
  AdrSrcMeGlobalRst, // ME-Initiated Global Reset
  AdrSrcPmcGloblRst, // SoC PMC Firmware-Initiated Global Reset
  AdrSrcMax
} ADR_SOURCES;


typedef struct {
  UINT8 AdrSrcOptionNumber;
  UINT32 AdrSrcBitNumber;
} PCH_ADR_TABLE;

/**
  PCH ADR Enable master switch.

  This function enables/disables PCH-side ADR.

  @param EnableAdr  TRUE:  Enables PCH-side ADR
                    FALSE: Disables PCH-side ADR
**/
VOID
PchAdrEnableAdr (
  BOOLEAN  EnableAdr
  );

/**
  Configures PCH-side ADR.

  This function must always be called by MRC even if ADR is not being enabled.
  This is to allow PCH DFX overrides to work.

**/
VOID
PchAdrConfigure (
  VOID
  );

/**
  @brief Reconfigure PCH-side ADR for NVDIMMs.

  @param[in] TimerVal Timer value [us] to configure, a finite set of values is supported (see PCH_ADR_TIMER_<*>).

  @return Void.
**/
VOID
PchAdrConfigureForNvdimm (
  IN UINT32 TimerVal
  );

/**
  @brief Reconfigure PCH-side ADR for DDRT DIMMs.

  @param[in] TimerVal Timer value [us] to configure, a finite set of values is supported (see PCH_ADR_TIMER_<*>).

  @return Void.
**/
VOID
PchAdrConfigureForDdrt (
  IN UINT32 TimerVal
  );

/**
  Clears PCH ADR Reset Status
**/
VOID
PchAdrClearAdrResetStatus (
  VOID
  );

/**
  Gets PCH ADR Reset Status

  @retval TRUE  ADR flow was executed during a prior global reset entry.
  @retval FALSE ADR flow was *not* executed during a prior global reset entry.

**/
BOOLEAN
PchAdrGetAdrResetStatus (
  VOID
  );

#endif // _PCH_ADR_LIB_H_
