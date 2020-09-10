/** @file
  Interface of ProcSmbIpRasLib Interface.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef __PROC_SMB_IP_RAS_LIB__
#define __PROC_SMB_IP_RAS_LIB__

/**
  Convert I3C port id to instance id
  SbPortId 466,467 map to Instance id 0,1.

  @param [IN]   SbPortId    --  Sb port Id for I3C

  @retval  Instance Id
**/
UINT8
GetI3cInstFromSbPortId (
  IN  UINT32         SbPortId
  );

/**
  Read INTR_STATUS_ENABLE_SB_I3C0_REG

  @param [IN]   Socket      --  Socket index
  @param [IN]   Inst    --  Instance Id for I3C

  @retval  Value of register
**/
UINT32
ReadIntrStatusEn (
  IN  UINT8         Socket,
  IN  UINT8         Inst
  );

/**
  Read INTR_SIGNAL_ENABLE_SB_I3C0_REG

  @param [IN]   Socket      --  Socket index
  @param [IN]   Inst    --  Instance Id for I3C

  @retval  Value of register
**/
UINT32
ReadIntrSignalEn (
  IN  UINT8         Socket,
  IN  UINT8         Inst
  );

/**
  Write INTR_STATUS_ENABLE_SB_I3C0_REG

  @param [IN]   Socket      --  Socket index
  @param [IN]   Inst    --  Instance Id for I3C
  @param [IN]   Data    --  Data to write

  @retval  None
**/
VOID
WriteIntrStatusEn (
  IN  UINT8         Socket,
  IN  UINT8         Inst,
  IN  UINT32        Data
  );

/**
  Write INTR_SIGNAL_ENABLE_SB_I3C0_REG

  @param [IN]   Socket      --  Socket index
  @param [IN]   Inst    --  Instance Id for I3C
  @param [IN]   Data    --  Data to write

  @retval  None
**/
VOID
WriteIntrSignalEn (
  IN  UINT8         Socket,
  IN  UINT8         Inst,
  IN  UINT32        Data
  );

/**
  Enable error for I3C status enable register.

  @param [IN]   Socket      --  Socket index
  @param [IN]   Inst    --  Instance Id for I3C

  @retval  None
**/
VOID
SetIntrStatusErrorEn (
  IN  UINT8         Socket,
  IN  UINT8         Inst
  );

/**
  Enable error for I3C signal enable register.

  @param [IN]   Socket      --  Socket index
  @param [IN]   Inst    --  Instance Id for I3C

  @retval  None
**/
VOID
SetIntrSignalErrorEn (
  IN  UINT8         Socket,
  IN  UINT8         Inst
  );

/**
  This function will check if the error happened on SMBUS.

  @param [IN]   Socket  --  socket index
  @param [IN]   Inst  -- Instance Id of I3C

  @retval FALSE: If no error happened
  @retval TRUE:  If there was an error
**/
BOOLEAN
IsSmbusError (
  IN  UINT8             Socket,
  IN  UINT8             Inst
  );

/**
  This function set bus reset done status.

  @param [IN]   Socket  --  socket index
  @param [IN]   Inst  -- Instance Id of I3C

  @retval None
**/
VOID
SetI3cStatusResetDone (
  IN  UINT8             Socket,
  IN  UINT8             Inst
  );

/**
  This function set bus reset done signal.

  @param [IN]   Socket  --  socket index
  @param [IN]   Inst  -- Instance Id of I3C

  @retval None
**/
VOID
SetI3cSignalResetDone (
  IN  UINT8             Socket,
  IN  UINT8             Inst
  );

/**
  This function clear bus reset done status.

  @param [IN]   Socket  --  socket index
  @param [IN]   Inst  -- Instance Id of I3C

  @retval None
**/
VOID
ClearI3cStatusResetDone (
  IN  UINT8             Socket,
  IN  UINT8             Inst
  );

/**
  This function clear bus reset done signal.

  @param [IN]   Socket  --  socket index
  @param [IN]   Inst  -- Instance Id of I3C

  @retval None
**/
VOID
ClearI3cSignalResetDone (
  IN  UINT8             Socket,
  IN  UINT8             Inst
  );

/**
  Send bus reset.
  Set RESET_CTRL_SB.BUS_RESET_TYPE = 011b,
       AND RESET_CTRL_SB.BUS_RESET = 1.

  @param [IN]   Socket  --  socket index
  @param [IN]   Inst - Instance Id of I3C

  @retval  None
**/
VOID
I3cReset (
  IN  UINT8             Socket,
  IN  UINT8             Inst
  );

/**
   Read register

  @param [IN]   Socket  --  socket index
  @param [IN]   Inst - Instance Id of I3C

  @retval  Data of periodic_poll_cmd_en
**/
UINT16
ReadTsodCmd (
  IN  UINT8             Socket,
  IN  UINT8             Inst
  );

/**
  Write Tsod

  @param [IN]   Socket  --  socket index
  @param [IN]   Inst - Instance Id of I3C
  @param [IN]   Data - Data to write to periodic_poll_cmd_en

  @retval  None
**/
VOID
WriteTsodCmd (
  IN  UINT8             Socket,
  IN  UINT8             Inst,
  IN  UINT16            Data
  );

/**
  Check Bus reset complete.

  @param [IN]   Socket  --  socket index
  @param [IN]   Inst - Instance Id of I3C

  @retval FALSE: If reset not done.
  @retval TRUE:  Bus reset is done.
**/
BOOLEAN
IsSmbusResetDone (
  IN  UINT8             Socket,
  IN  UINT8             Inst
  );
#endif
