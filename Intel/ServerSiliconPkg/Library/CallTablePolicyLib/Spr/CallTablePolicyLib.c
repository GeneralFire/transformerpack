/** @file
  Memory Policy Library, to handle post\pre MRC Task in Calltable.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Base.h>
#include <SysHost.h>
#include <Library/CallTablePolicyLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemCallTableLib.h>
#include <Library/SsaBuiltInRmtLib.h>

// Function prototype to call BSSA RMT on advance memory training
VOID
BssaRmtOnAdvancedTraining (
  VOID
  );

/**
  Hook for executing function before a specified MRC Calltable function

  @param[in] PolicFlag   -   MRC Call table Policy Flag

  @retval   EFI_SUCCESS  -  Function executed successfully
**/
EFI_STATUS
EFIAPI
PreMrcTaskPolicy (
  IN UINT8 PolicyFlag
  )
{
  return EFI_SUCCESS;
}

/**
  Hook for executing function after a specified MRC Calltable function

  @param[in] PolicFlag   -   MRC Call table Policy Flag

  @retval   EFI_SUCCESS  -  Function executed successfully
**/
EFI_STATUS
EFIAPI
PostMrcTaskPolicy (
  IN UINT8 PolicyFlag
  )
{
  PSYSHOST Host;
  Host = GetSysSetupPointer ();

  if  ((PolicyFlag & MRC_PF_RMT) != 0) {
    BssaRmtOnAdvancedTraining (); //If DDR4 call BSSA RMT
  }
  return EFI_SUCCESS;
}

/**
  Function to call BSSA RMT based on Setup Option.

  retval  -  N/A
**/
VOID
BssaRmtOnAdvancedTraining (
  VOID
  )
{
  SYS_SETUP     *Setup;

  Setup = GetSysSetupPointer ();

  if (Setup->mem.RmtOnAdvancedTraining != 0) {
    // Call BSSA RMT
    RunBuiltInBssaRmt ();
  }
  return;
}