/** @file
  This file contains definitions required to use the LT BIOS
  Authenticated Code Module Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2014 Intel Corporation. <BR>

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

#ifndef _LT_BIOS_ACM_LIB_H_
#define _LT_BIOS_ACM_LIB_H_

//
// LtSetupAndLaunchBiosAcm is platform specific code that can be used
// to setup and launch any of the LT BIOS Authenticated Code Module
// functions.
//
extern
VOID
LaunchBiosAcm (
  IN      UINTN                  AcmBase,
  IN      UINTN                  EsiValue
  );

//
// AsmGetSecParamters will return the parameter
//
VOID
AsmGetSecParameters (
  IN UINT32       Index,
  OUT UINT32      *EaxValue OPTIONAL,
  OUT UINT32      *EbxValue OPTIONAL,
  OUT UINT32      *EcxValue OPTIONAL
  );

//
// The following are values that are placed in the esi register when
// calling the BIOS ACM.  These constants are used as parameters to the
// LtSetupandLaunchBiosAcm function.
//
#define     LT_LAUNCH_SCLEAN        0x00
#define     LT_RESET_EST_BIT        0x01
#define     LT_CLEAR_TPM_AUX_INDEX  0x02
#define     LT_LAUNCH_SCHECK        0x04
#define     LT_CLEAR_SECRETS        0x05            // Used only in LTSX  (envoked by system bios when secrets are found in memory)
#define     LT_LOCK_CONFIG          0x06            // Used only in LTSX  (envoked by system BIOS after memory and LT configuration is done)
#define     LT_MEASURE_STARTUP_BIOS 0xFF            // Used only in LTSX  (envoked by microcode before reset vector)

#endif
