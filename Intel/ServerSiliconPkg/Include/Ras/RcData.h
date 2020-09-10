/** @file
  RAS RC data header file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _RAS_RC_DATA_H_
#define _RAS_RC_DATA_H_

///
/// RAS policy being requested of RC.
///
typedef struct {
  UINT8    RasModes;             ///< modes requested per policy
  UINT16   RasModesEx;           ///< EX modes requested per policy
  BOOLEAN  McBankWarmBootClearError;
  UINT8    PoisonEn;
  UINT8    PfdEn;
  UINT8    CrashLogFeature;
  UINT8    CrashLogOnAllReset;
  UINT8    CrashLogClear;
  UINT8    CrashLogReArm;
} RAS_RC_POLICY;

///
/// RAS configuration coming out of RC.
///
typedef struct {
  UINT8   SupportedRasModes;    ///< modes supported per population
  UINT16  SupportedRasModesEx;  ///< EX modes supported per population
  UINT8   Reserved[1];
  UINT32  PatrolScrubInterval[MAX_SOCKET][MAX_IMC]; ///< patrol scrub interval
} RAS_RC_CONFIG;

//
// Bit definitions for RasModes
//
#define FULL_MIRROR_1LM         BIT0
#define FULL_MIRROR_2LM         BIT1
#define CH_LOCKSTEP             BIT2
#define RK_SPARE                BIT3
#define PARTIAL_MIRROR_1LM      BIT5
#define PARTIAL_MIRROR_2LM      BIT6
#define STAT_VIRT_LOCKSTEP      BIT7

//
// Bit masks for RasModes
//
#define CH_INDEPENDENT          0
#define CH_ML                   (FULL_MIRROR_1LM | FULL_MIRROR_2LM | STAT_VIRT_LOCKSTEP)
#define CH_SL                   (RK_SPARE | STAT_VIRT_LOCKSTEP)
#define CH_MS                   (RK_SPARE | FULL_MIRROR_1LM | FULL_MIRROR_2LM)
#define CH_MLS                  (FULL_MIRROR_1LM | FULL_MIRROR_2LM | STAT_VIRT_LOCKSTEP | RK_SPARE)
#define CH_ALL_MIRROR           (FULL_MIRROR_1LM | FULL_MIRROR_2LM | PARTIAL_MIRROR_1LM | PARTIAL_MIRROR_2LM)

//
// Bit definitions for RasModesEx
//
#define SDDC_EN                 BIT0
#define DMNDSCRB_EN             BIT1
#define PTRLSCRB_EN             BIT2
#define ADDDC_ERR_INJ_EN        BIT3
#define PCLS_EN                 BIT4
#define MIRROR_SCRUB_EN         BIT5
#define ADDDC_EN                BIT6
#define PTRLSCRB_EOP_EN         BIT8

#endif // _RAS_RC_DATA_H_
