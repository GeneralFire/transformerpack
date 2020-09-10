/** @file
  Implementation IIO ITP/TC/OTC

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#include "IioRasLibInternal.h"


/**
  enable RAS pci device, each iio stack have one RAS device.

  @param  [in]  Socket  -- socket index
  @param  [in]  Bus     -- bus index
  @param  [in]  Device  -- device index
  @param  [in]  Function-- Function index

  @retval none
**/
VOID
EFIAPI
EnableRasDevice (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function
  )
{
  RAS_MAIL_BOX  *MailBox;

  MailBox = GetRasMailBox ();

  IioEnableIrpError(Socket, Bus, Device, Function);
  IioEnableItc(Socket, Bus, Device, Function);
  IioEnableOtc (Socket, Bus, Device, Function);
  IioEnableTc(Socket, Bus, Device, Function);
  IioSetIioMisc(Socket, Bus, Device, Function, MailBox->IioInitPar.IioMiscErrorEn, MailBox->IioInitPar.IioMiscErrorSevMap, MailBox->IioInitPar.IioMiscErrorBitMap);

  if (MailBox->IioInitPar.IioViralEn) {
    IioEnableViral (Socket, Bus, Device, Function, MailBox->IioInitPar.IioClearViralStatus, MailBox->IioInitPar.IoMcaEn);
  }
}



