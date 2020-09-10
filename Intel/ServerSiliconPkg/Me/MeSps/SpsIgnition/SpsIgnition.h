/** @file
  Definitions for SPS Ignition support.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _SPS_IGNITION_H_
#define _SPS_IGNITION_H_

#include <Base.h>
#include <Uefi.h>


/******************************************************************************
 * Fields in R_ME_H_GS. Bits [31:8] are undefined
 */
#define B_ME_CONFIGURATION_DONE        BIT0

#define V_IGN_END_OF_POST              BIT1
#define V_IGN_CORE_BIOS_DONE           BIT2
#define V_IGN_DRAM_INIT_DONE           BIT3
#define B_IGN_BOOT_STATUS              (BIT4 | BIT3 | BIT2 | BIT1)

#define V_IGN_SSC_DEFAULT              0
#define V_IGN_SSC_DISABLE              BIT5
#define V_IGN_SSC_FORCE_EVEN           BIT6
#define B_IGN_SSC_CONFIGURATION        (BIT6 | BIT5)

#define B_IGN_SVN_COMMIT               BIT7

#define B_IGN_DXE_INDICATORS           ( V_IGN_END_OF_POST \
                                       | V_IGN_CORE_BIOS_DONE \
                                       )
#define B_IGN_INDICATORS               ( B_IGN_DXE_INDICATORS \
                                       | B_IGN_SVN_COMMIT \
                                       | B_IGN_SSC_CONFIGURATION \
                                       | B_IGN_BOOT_STATUS \
                                       | B_ME_CONFIGURATION_DONE \
                                       )


/******************************************************************************
 * Function prototypes
 */
/**
  This function clears all Ignition FW related fields of HECI-1 H_GS register.
**/
VOID
IgnitionClearIndicators (
  VOID
  );

/**
  This function clears Ignition FW related fields of HECI-1 H_GS register which
  are supposed to be set in DXE.
**/
VOID
IgnitionClearDxeIndicators (
  VOID
  );

/**
  This function sets END_OF_POST bit in Boot Status field of HECI-1 H_GS
  register.
**/
VOID
IgnitionIndicateEndOfPost (
  VOID
  );

/**
  This function sets CORE_BIOS_DONE bit in Boot Status field of HECI-1 H_GS
  register.
**/
VOID
IgnitionIndicateCoreBiosDone (
  VOID
  );

/**
  This function sets DRAM_INIT_DONE bit in Boot Status field of HECI-1 H_GS
  register.
**/
VOID
IgnitionIndicateDramInitDone (
  VOID
  );

/**
  This function sets SSC Configuration field of HECI-1 H_GS register
  to SSC disable.
**/
VOID
IgnitionIndicateSscDisable (
  VOID
  );

#endif // _SPS_IGNITION_H_
