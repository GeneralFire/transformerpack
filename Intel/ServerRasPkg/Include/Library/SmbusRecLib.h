/** @file
  Interface of CxlRasLib Interface.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef __SMBUS_REC_LIB_H__
#define __SMBUS_REC_LIB_H__

typedef union {
  struct {
    UINT32 SbPortId: 32;
    UINT32 periodic_poll_cmd_en : 16;
    UINT32 Reserved: 16;
  } Bits;
  UINT64 Data;
} SMBUS_REC_DATA_STRUCT;

/**
  This function is for Smbus Recovery initialize flow.

  @param [IN]   Socket      --  Socket index
  @param [IN]   SbPortId    --  Side Band Id for I3C

  @retval  None
**/
VOID
EFIAPI
SmbusRecoveryConfig (
  IN  UINT8         Socket,
  IN  UINT32        SbPortId
  );

/**
  This function is the entry point of the Smbus recovery SMM handler

  @param [IN]   Socket      --  Socket index
  @param [IN]   SbPortId    --  Side Band Id for I3C

  @retval  None

**/
VOID
EFIAPI
SmbusRecoverySmiHandler(
  IN  UINT8         Socket,
  IN  UINT32        SbPortId
  );

#endif
