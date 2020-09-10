/** @file
  Implementation of RAS EFI runtime safe debug print routine.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/SerialPortLib.h>
#include <Library/RasDebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#define         RAS_MAX_DEBUG_MESSAGE_LENGTH  0x200

CORE_PRINT_FUNC mOsPrintRealFunc = 0;


/**
  The constructor function initialize the Serial Port Library

  @retval EFI_SUCCESS   The constructor always returns RETURN_SUCCESS.

**/
RETURN_STATUS
EFIAPI
RuntimeRasDebugLibInit  (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{

  return SerialPortInitialize ();
}

/**
  This procedue is to using OS print routine for RAS code debugging. This is dfx function.
  For example, BIOS serial port print can't show in windbg program. Only windows self-build print routine could show on windbg through serial port.
  Hence we need to use this routine to pass OS self-build print to make BIOS code print on windbg program. Such like DbgPrintEx in windows.

  @param [in]  OsPrint -- windows print function address

  @retval none.

**/
VOID
EFIAPI
SaveOsPrint (
  IN  CORE_PRINT_FUNC   OsPrint
  )
{
    mOsPrintRealFunc = OsPrint;

    //
    // use OS routine to print log over serial port to windbg console.
    //
    OsPrint (101, 0xffffffff,"OsPrint saved buffer address  %llx and OsPrint address: %llx \n", &mOsPrintRealFunc, mOsPrintRealFunc);

}



VOID
EFIAPI
RasDebugPrint (
  IN  UINTN        ErrorLevel,
  IN  CHAR8       *Format,
  ...
  )
{
  CHAR8             Buffer[RAS_MAX_DEBUG_MESSAGE_LENGTH];
  VA_LIST           Marker;
  UINTN             Start;

  if ((ErrorLevel & GetDebugPrintErrorLevel ()) == 0) {
    return;
  }

  if (mOsPrintRealFunc == 0) {
    Start = 0;
    VA_START (Marker, Format);
    AsciiVSPrint (&Buffer[Start], sizeof (Buffer) - Start, Format, Marker);
    VA_END (Marker);
    SerialPortWrite ((UINT8 *)Buffer, AsciiStrLen (Buffer));

  } else {
    Start = 0;
    VA_START (Marker, Format);
    AsciiVSPrint (&Buffer[Start], sizeof (Buffer) - Start, Format, Marker);
    VA_END (Marker);

    mOsPrintRealFunc (101, 0xffffffff, Buffer);
  }
}

