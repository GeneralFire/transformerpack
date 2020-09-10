/** @file

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

#include <Uefi/UefiBaseType.h>
#include <MemCommon.h>

/**

  Set HBM MC Mode

  @param[in]                Socket             Current Socket (0-Based)
  @param[in]                HbmCh              Current HBM Ch (0-Based)

**/
VOID
EFIAPI
HbmSetMcMode (
  IN        UINT8           Socket,
  IN        UINT8           HbmCh
  )
{
  return;

}

/**

  Initialize HBM MC page policy

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Ch             Current Ch (0-Based)

**/
VOID
EFIAPI
HbmInitPagePolicy (
  IN        UINT8           Socket,
  IN        UINT8           Ch
  )
{
  return;
}

/**

  Initialize HBM MC page policy

  @param[in]                Socket         Current Socket (0-Based)
  @param[in]                Ch             Current Ch (0-Based)
  @param[in]                EnableDbi      BOOLEAN, to enable DBI or disable it

**/
VOID
EFIAPI
HbmInitDbi (
  IN        UINT8           Socket,
  IN        UINT8           Ch,
  IN        BOOLEAN         EnableDbi
  )
{
  return;
}

/**
  This function programs HBM MC registers required after training and before normal mode.

  @param[in] SocketId                     Socket index.
  @param[in] ChId                         Channel index within socket.

  @retval MRC_STATUS_SUCCESS              This function is executed successfully.
  @retval MRC_STATUS_UNSUPPORTED          Register programming is not supported.
  @retval MRC_STATUS_INVALID_PARAMETER    Some of the input parameters are invalid.
**/
MRC_STATUS
EFIAPI
HbmMcLateConfig (
  IN UINT8     SocketId,
  IN UINT8     ChId
  )
{
  //
  // Stub function for API consistency
  //
  return MRC_STATUS_UNSUPPORTED;
}

/**

  Program HBM MC into normal mode

  @param[in] Socket                 Socket index (0-Based).
  @param[in] Ch                     Channel index within Socket (0-Based).

  @retval EFI_SUCCESS               HBM MC were programmed into normal mode.
  @retval EFI_UNSUPPORTED           Register programming for normal mode is unsupported.
  @retval EFI_INVALID_PARAMETER     Some of input parameters are invalid.

**/
EFI_STATUS
EFIAPI
HbmMcSetNormalMode (
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  return EFI_UNSUPPORTED;
}

/**

  Program HBM DIMM MTR register

  @param[in]   Socket    Current Socket (0-Based)

  @retval  EFI_SUCCESS             This function completed execution with no warnings/errors
  @retval  EFI_INVALID_PARAMETER   Invalid density detected on one of HBM IO modules.

**/
EFI_STATUS
EFIAPI
HbmDimmMtrConfig (
  IN        UINT8           Socket
  )
{
  return EFI_UNSUPPORTED;
}

/**

  Configures registers for Memory hot feature

  @param[in] Socket  - Socket Id, 0 based

  @retval N/A

**/
VOID
HbmConfigureMemHotFeature (
  IN UINT8 Socket
  )
{
  // Null
}

/**
  This function toggles the reset_io register field.

  @param[in] SocketId             Socket index.

  @retval MRC_STATUS_SUCCESS             This function is executed successfully.
  @retval MRC_STATUS_UNSUPPORTED         The toggling of reset_io is not supported.
  @retval MRC_STATUS_INVALID_PARAMETER   Some of the input parameters are invalid.
**/
MRC_STATUS
EFIAPI
HbmMcSetResetIo (
  IN UINT8   SocketId
  )
{
  //
  // Stub function for API consistency
  //
  return MRC_STATUS_UNSUPPORTED;
}

/**

  Programs timing parameters

  @param Socket      - Socket number

  @retval EFI_SUCCESS     Success
          !EFI_SUCCESS    Failed

**/
EFI_STATUS
EFIAPI
ProgramTimingsHbm (
  UINT8    Socket
  )

{
  return EFI_SUCCESS;
}

/**

  Program scheduler registers after training

  @param[in] Socket - Socket number

  @retval EFI_SUCCESS     Success
          !EFI_SUCCESS    Failed

**/
EFI_STATUS
EFIAPI
SchedulerConfigLateHbm (
  IN    UINT8    Socket
  )
{
  return EFI_SUCCESS;
}
