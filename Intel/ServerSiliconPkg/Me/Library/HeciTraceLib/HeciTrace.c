/** @file
  Heci driver core. For PEI/Dxe Phase, determines the HECI device and initializes it.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/HeciTraceLib.h>
#include <Library/HeciConfigureLib.h>

/**
  Prints a message to the provided buffer

  @param[in]  Buffer      Buffer for output string
  @param[in]  Size        Buffer size
  @param[in]  Format      Format string for the message to print.
  @param[in]  ...         Variable argument list whose contents are accessed
                          based on the format string specified by Format.

**/
VOID
EFIAPI
HeciCreateTrace (
  IN OUT CHAR8        *Buffer,
  IN     UINTN        Size,
  IN     CONST CHAR8  *Format,
  ...
  )
{
  if (Buffer != NULL) {
    VA_LIST  Marker;

    //
    // Convert the message to an ASCII String
    //
    VA_START (Marker, Format);
    AsciiVSPrint (Buffer, Size, Format, Marker);
    VA_END (Marker);
  }
} // HeciCreateTrace ()

/**
 @brief
    Trace the message to the terminal.
    Normally just trace the header, if needed dump whole message to terminal.

 @param[in] HeciIdentifier Heci Identification string
 @param[in] pPrefix        Text prefix, whether it is receive or send
 @param[in] MsgHdr         The header of the message
 @param[in] pMsgBody       Pointer to the message body
 @param[in] MsgBodyLen     The length of message body
**/
VOID HeciPostTrace (
  CONST CHAR8  *HeciIdentifier,
  CONST CHAR8  *pPrefix,
  CONST UINT32 MsgHdr,
  CONST UINT8  *pMsgBody,
        UINT32 MsgBodyLen
  )
{
  if (MsgBodyLen > sizeof (UINT32)) {
    DEBUG ((DEBUG_INFO, "%a %a%08X %08X ...\n", HeciIdentifier, pPrefix, MsgHdr, *(UINT32*)pMsgBody));
  } else if (MsgBodyLen > 0) {
    DEBUG ((DEBUG_INFO, "%a %a%08X %08X\n", HeciIdentifier, pPrefix, MsgHdr, *(UINT32*)pMsgBody));
  } else {
    DEBUG ((DEBUG_INFO, "%a %a%08X\n", HeciIdentifier, pPrefix, MsgHdr));
  }
} // HeciPostTrace ()
