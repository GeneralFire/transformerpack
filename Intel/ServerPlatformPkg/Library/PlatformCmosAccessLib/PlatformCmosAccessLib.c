/** @file
  Platform CMOS Access Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation. <BR>

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

#include <Library/IoLib.h>
#include <Library/PlatformCmosAccessLib.h>
#include <Platform.h>
#include <CmosMap.h>
#include <Uefi.h>
#include <Library/PchRtcLib.h>

/**
  Return the platform CMOS entries.

  @param [out]  EnryCount Number of platform CMOS entries.

  @return Platform CMOS entries.
**/
CMOS_ENTRY *
EFIAPI
PlatformCmosGetEntry (
  OUT UINTN       *EntryCount
  )
{

  *EntryCount = 0;
  return NULL;
}

/**
  Return the NMI enable status.
**/

BOOLEAN
EFIAPI
PlatformCmosGetNmiState (
  VOID
 )
{
  volatile UINT32 Data32;
  BOOLEAN           Nmi;
  Data32  = 0;

  if ((ReadNmiEn ()) & 0x80){
    Nmi = TRUE;
  }
  else
    Nmi = FALSE;

  return Nmi;
}
