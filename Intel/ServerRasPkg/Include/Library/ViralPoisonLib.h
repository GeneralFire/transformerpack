/** @file
  Interface of viral and poison library interface

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

#ifndef __VIRAL_POISON_LIB_H__
#define __VIRAL_POISON_LIB_H__

/**
  Implementation of poison and Viral.

  @param  ViralFlag   TRUE -- Viral enable
  @param  PoisonFlag  TRUE -- Poison enable
  @param  ClearFlag   TRUE -- clear Viral status.

**/
VOID
EFIAPI
EnableSystemViralAndPoison (
  IN   BOOLEAN  ViralFlag,
  IN   BOOLEAN  PoisonFlag,
  IN   BOOLEAN  ClearFlag,
  IN   UINT8    IoMcaEn
  );


/**
  Viral handler implementation for KTI, UBOX, and IIO

  @retval none.

**/
VOID
EFIAPI
ProcessKtiFatalAndViralError (
  VOID
  );

////////////////////////////////////////////////////////////////////////
//VIRAL function definitions
////////////////////////////////////////////////////////////////////////
BOOLEAN
SocketInViralState (
  UINT8 Socket
  );


RETURN_STATUS
EFIAPI
IioEnableViral (
  IN      UINT8      Skt,
  IN      BOOLEAN    ClearFlg,
  IN      UINT8      IoMcaEn
  );

RETURN_STATUS
EFIAPI
IioEnablePoison (
  IN      UINT8      Skt
  );
#endif
