/** @file
  This file is for DFX functions

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

#include <Base.h>


/**
  Enable access to IOMT tool.

  @param[in] IioIndex - IIO instance (usually socket) being configured.
**/
VOID
IioEnableIomtRegisterAccess (
  IN  UINT8    IioIndex
  )
{
}

/**
  Enable OS Access to DMI and DFX functions.

  @param IioIndex      - Socket number of the port being configured.

**/
VOID
IioEnableOsAccessDmiDfx (
  UINT8                           IioIndex
)
{
}

/**
  This function disables treating MsgD with len>16 as malformed TLP

  @param IioIndex      - Socket number of the port being configured.
  @param PortIndex     - Index to Iio Root Port.
**/
VOID
IioDisableMsgDLenCheck (
  IN  UINT8                           IioIndex,
  IN  UINT8                           PortIndex
)
{
}

/**
  This function enabled the Completion Timeout Mask bit for all Root Ports
  in order RAS to work properly.

  @param IioIndex      - Socket number of the port being configured.
  @param PortIndex     - Index to Iio Root Port
**/
VOID
IioEnableCtoMaskBit (
  IN  UINT8                           IioIndex,
  IN  UINT8                           PortIndex
  )
{
}

