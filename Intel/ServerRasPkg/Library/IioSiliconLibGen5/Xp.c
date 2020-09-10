/** @file
  Implementation of XP IIO RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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

#include <Library/CpuCsrAccessLib.h>
#include <RcRegs.h>
#include <Library/RasDebugLib.h>
#include <Library/RasMailBoxLib.h>
#include <Library/MpExtensionLib.h>
#include <Library/MmPciExtLib.h>
#include <Library/IoLib.h>
#include <Register/IioRegDef.h>



/**
Program Iio XP  errors(Correctable, Uncorrectable, Fatal) .

@param Bus       - Bus
@param Device    - Device
@param Function  - Function
@param CorrErrMsk  - Correctable error mask

@retval  none
**/
VOID
EFIAPI
IioXPProgramCorrErrors (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  //
  // Gen5 IIO doesn't support XP errors
  //
}

/**
  Program Iio XP  errors(Correctable, Uncorrectable, Fatal) .

  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param CorrErrMsk  - Correctable error mask

**/
VOID
EFIAPI
IioXPProgramUnCorrErrors (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  //
  // Gen5 IIO doesn't support XP errors
  //
}

/**
  Program Iio XP  Sev .

  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param CorrErrMsk  - Correctable error mask

**/
VOID
EFIAPI
IioXPProgramErrSev (
  IN UINT8   Socket,
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
)
{
  //
  // Gen5 IIO doesn't support XP errors
  //
}

/**
    Program Iio Correctable error count .

    @param Bus       - Bus
    @param Device    - Device
    @param Function  - Function
    @param CorrErrMsk  - Correctable error mask

**/
VOID
EFIAPI
IioProgramCorrErrCnt (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
}

/**
  This function is to get correctable error counter mask value

  @param[in] Socket         Device's Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.

  @retval    correctable error counter mask value
**/
UINT32
EFIAPI
IioGetCorrCntErrMsk (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  UINT32  Data32;

  Data32 = 0;
  RAS_DEBUG ((LEVEL_REG, "IioGetCorrCntErrMsk CorrErrCntMsk = 0x%x\n", Data32));
  return Data32;
}

/**
    Clear XP errors.

    @param Bus       - Bus
    @param Device    - Device
    @param Function  - Function
    @param CorrErrMsk  - Correctable error mask

**/
VOID
EFIAPI
IioXpClearErr (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  //
  // Gen5 IIO doesn't support XP errors
  //
}

/**
    set XP mask to disable.

    @param Socket    - Socket
    @param Bus       - Bus
    @param Device    - Device
    @param Function  - Function

**/
VOID
EFIAPI
SetXPMaskError (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  //
  // Gen5 IIO doesn't support XP errors
  //
}

/**
  This function is to get the XP Global Error Status registers and to see if one of the enabled errors
  has occurred.

  @param[in] Socket         PCIe root port device's socket number.
  @param[in] Bus            PCIe root port device's bus number.
  @param[in] Device         PCIe root port device's device number.
  @param[in] Function       PCIe root port device's function number.
  @param[out] Xpcorsts      Xp correctable error status.
  @param[out] Xpuncsts      Xp uncorrectable error status.
  @param[out] xpglbsts      xp global error status.
  @retval    BOOLEAN        TRUE   - An error was detected.
                            FALSE  - An error was not detected, or the device is not a root port.
--*/
BOOLEAN
IioGetXpErrorStatus (
  IN  UINT8    Socket,
  IN  UINT8    Bus,
  IN  UINT8    Device,
  IN  UINT8    Function,
  OUT UINT32   *Xpcorsts,
  OUT UINT32   *Xpuncsts,
  OUT UINT32   *Xpglbsts
  )
 {
  //
  // Gen5 IIO doesn't support XP errors
  //
  return FALSE;
 }