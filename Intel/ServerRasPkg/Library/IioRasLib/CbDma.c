/** @file
  Implementation of CBDMA IIO RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

     Enable DMA Errors

     @param None

     @retval None

 **/
VOID
EFIAPI
IioEnableDmaErrors (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function
  )
{
  RAS_MAIL_BOX  *MailBox;
  UINT32        Data32;
  MailBox = GetRasMailBox ();

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "      IioEnableDma Skt = 0x%x, IioDmaErrorEn = 0x%x,IioDmaSevBitMap = 0x%x, IioDmaBitMap = 0x%x \n",
        MailBox->IioInitPar.IioDmaErrorEn, MailBox->IioInitPar.IioDmaSevBitMap, MailBox->IioInitPar.IioDmaBitMap));

  Data32 = MailBox->IioInitPar.IioDmaSevBitMap & IIO_DMA_MSK;
  if (MailBox->IioInitPar.IioDmaErrorEn) {
   IioSetDmaErrorsSev (Socket, Bus, Device, Function, Data32);
  }

  Data32 = IIO_DMA_MSK;
  if (MailBox->IioInitPar.IioDmaErrorEn) {
    Data32 &= MailBox->IioInitPar.IioDmaBitMap | BIT31; // Per EDS always set BIT31
  }

  IioSetDmaErrorsMsk (Socket, Bus, Device, Function, Data32);

}

 /**

  Clear DMA Errors

  @param  [in]  Socket  -- socket index
  @param  [in]  Bus     -- bus index
  @param  [in]  Device  -- device index
  @param  [in]  Function-- Function index
  @param  [in]  ErrSeverity -- Error Severity

     @retval None

 **/
VOID
EFIAPI
IioClearDmaErrors (
  IN   UINT8    Socket,
  IN   UINT8    Bus,
  IN   UINT8    Device,
  IN   UINT8    Function,
  IN   UINT32   ErrSeverity
  )
{
  IioClearDmaErrorsSi (Socket, Bus, Device, Function);
}
