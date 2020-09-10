/** @file
  Implementation Vtd IIO RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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

   Enable VTD


  @param  [in]  Socket  -- socket index
  @param  [in]  Instance -- instance of satallite IEH per socket.

  @retval None
 **/
VOID
EFIAPI
IioEnableVtd (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Func
  )
{
  RAS_MAIL_BOX  *MailBox;

  MailBox = GetRasMailBox ();

  if (MailBox->IioInitPar.IioVtdErrorEn) {
    SetVtdSeverity (Socket, Bus, Device, Func);
    SetVtdMsk (Socket, Bus, Device, Func);
  }

  if (MailBox->IioInitPar.IioPoisonEn) {
    IioEnablePoisonVtd (Socket, Bus, Device, Func);
  }

  if (MailBox->IioInitPar.IioPcieAerSpecCompEn) {
    IioEnablePoisMsgSpecBehaviorVtd (Socket, Bus, Device, Func);
  }



}
