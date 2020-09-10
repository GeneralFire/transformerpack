/** @file
  MspChipFunctions.h
  MSP chip specific function prototypes.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2017 Intel Corporation. <BR>

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

#include "SysHost.h"
#include "MspLibraries.h"
#include <Spr/Ddrio/Include/MMRCPROJECT_DEFINITIONS_GENERATED_DDR.h>

/**
  Function to setup MSP parameters for ICX-DDR

  @param[in]      Host                      Pointer to the system Host (root) structure
  @param[in,out]  ExecuteOnThisChannel      Structure to indicate which channels should run
  @param[in,out]  ChannelEnabled            Array that indicates which channels are enabled
  @param[in,out]  CurrentPlatform           Array that indicates current platform
  @param[in,out]  CurrentFrequency          Array that indicates current frequency
  @param[in,out]  CurrentConfiguration      Array that indicates current configuration
  @param[in,out]  CurrentTechnology         Array that indicates current technology
  @param[in,out]  DynamicVars               Array that indicates dynamic variables

**/
VOID
PopulateMspDdr(
  IN  OUT   PSYSHOST    Host,
  IN  OUT   BOOLEAN     ExecuteOnThisChannel[MAX_CHANNELS_DDR],
  IN  OUT   BOOLEAN     ChannelEnabled[MAX_CHANNELS_DDR],
  IN  OUT   UINT16      CurrentPlatform[MAX_CHANNELS_DDR],
  IN  OUT   UINT16      CurrentFrequency[MAX_CHANNELS_DDR],
  IN  OUT   UINT16      CurrentConfiguration[MAX_CHANNELS_DDR],
  IN  OUT   UINT16      CurrentTechnology[MAX_CHANNELS_DDR],
  IN  OUT   UINT32      DynamicVars[MAX_CHANNELS_DDR][DYNVAR_MAX_DDR]
  );
