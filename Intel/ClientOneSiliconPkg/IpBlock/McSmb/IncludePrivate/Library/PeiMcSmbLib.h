/** @file
  McSmb routines

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification
**/
#ifndef _PEI_MCSMB_LIB_H_
#define _PEI_MCSMB_LIB_H_

#include <McSmbCtrl.h>

/**
  Speed modes for McSmb device.
**/
typedef enum {
  Standard0,
  Standard1,
  Fast,
  FastPlus
} MCSMB_SPEED_MODE;

/**
  Internal McSmb policy options
**/
typedef struct {
  /**
    Description: Contains De-Glitch pulse time
    Possible values: [0x00h-0xFF] - unit: 10ns, range: [0ns:2550ns]
  **/
  UINT8                        DeGlitchPulse;
  /**
    Description: Contains McSmbus speed mode
    Possible values: MCSMB_SPEED_MODE options
  **/
  MCSMB_SPEED_MODE             SpeedMode;
  /**
    Tsusto - Stop Condition setup time
    Description: Contains timing value for Tsusto field
    Possible values: [0x0-0xF], unit: [+/- 10ns], time range: [-70ns:+70ns]
  **/
  UINT8                        TimingTsusto;
  /**
    Tsusta - Repeated Start Condition setup time
    Description: Contains timing value for Tsusta field
    Possible values: [0x0-0xF], unit: [+/- 10ns], time range: [-70ns:+70ns]
  **/
  UINT8                        TimingTsusta;
  /**
    Tbuf - Bus free time between Stop and Start Condition
    Description: Contains timing value for Tbuf field
    Possible values: [0x0-0xF], unit: [+/- 10ns], time range: [-70ns:+70ns]
  **/
  UINT8                        TimingTbuf;
  /**
    Thdsta - Hold time after (Repeated) Start Condition
    Description: Contains timing value for Thdsta field
    Possible values: [0x0-0xF], unit: [+/- 10ns], time range: [-70ns:+70ns]
  **/
  UINT8                        TimingThdsta;

} MCSMB_PRIVATE_CONFIG;

/**
  McSmb controller structure
  Stores all data necessary to initialize McSmb IP block
**/
typedef struct {
  MCSMB_CONTROLLER             *Controller;
  MCSMB_PRIVATE_CONFIG         *PrivateConfig;
} MCSMB_HANDLE;

/**
  Performs McSmb configuration

  @param[in]  McSmbHandle       Pointer to McSmb controller configuration structure

  @retval None
**/
VOID
ConfigureMcSmb (
  IN  MCSMB_HANDLE        *McSmbHandle
);

#endif // _PEI_MCSMB_LIB_H_
