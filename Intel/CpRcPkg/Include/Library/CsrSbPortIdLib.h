/** @file
  Implementation of CsrSbPortId class library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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
#ifndef _GET_CSR_SB_PORT_ID_H_
#define _GET_CSR_SB_PORT_ID_H_

#include <UsraAccessType.h>

#define SB_REG_PORT_ID_INVALID      1023

/**
  This API gets the side band port ID based on the Box instant and function block info.

  @param[in]  UINT8             BoxInst
  @param[in]  UINT32            CsrOffset

  @retval PortId                The sideband port id as request
**/
UINT32
EFIAPI
GetSbRegPortId (
  IN  UINT8    BoxInst,
  IN  UINT32   CsrOffset
  );
#endif // _GET_CSR_SB_PORT_ID_H_
