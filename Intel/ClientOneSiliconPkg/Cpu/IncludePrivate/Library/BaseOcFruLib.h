/** @file
  Overclocking tolopolgy for specific generation. header file

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/
#ifndef _BASE_OC_FRU_LIB_H_
#define _BASE_OC_FRU_LIB_H_

/**
  This function return whether AVX2 and AVX512 voltage Guard Band supported.

  @retval TRUE  - AVX voltage Guard Band supported.
  @retval FALSE - AVX voltage Guard Band unsupported.
**/
BOOLEAN
IsAvxVoltageGuardBandSupport (
  VOID
  );

/**
  This function return whether Per Core HyperThread Disable supported.

  @retval TRUE  - Per Core HyperThread Disable supported.
  @retval FALSE - Per Core HyperThread Disable unsupported.
**/
BOOLEAN
IsPerCoreHtDisableSupport (
  VOID
  );

/**
  This function return whether ATOM Core OC supported.

  @retval TRUE  - ATOM Core OC supported.
  @retval FALSE - ATOM Core OC unsupported.
**/
BOOLEAN
IsAtomCoreOcSupport (
  VOID
  );

/**
  This function return PerCore HT Disable Command.

  @param[out]  ReadCommand   PerCore HT Disable Read Command
  @param[out]  WriteCommand  PerCore HT Disable Write Command
**/
VOID
OcGetPerCoreHtDisableCmd (
  OUT UINT32 *ReadCommand,
  OUT UINT32 *WriteCommand
  );

/**
  This function return TjMax Offset Command.

  @param[out]  ReadCommand   TjMax Offset Read Command
  @param[out]  WriteCommand  TjMax Offset Write Command
**/
VOID
OcGetTjMaxOffsetCmd (
  OUT UINT32 *ReadCommand,
  OUT UINT32 *WriteCommand
  );

/**
  This function return Realtime Memory Timing Enable/Disable Command

  @param[out]  MailboxType   Return mailbox type
  @param[out]  ReadCommand   Realtime Memory Timing Read Command
  @param[out]  WriteCommand  Realtime Memory Timing Write Command
**/
VOID
OcGetRealtimeMemoryTimingCmd (
  OUT UINT32 *MailboxType,
  OUT UINT32 *ReadCommand,
  OUT UINT32 *WriteCommand
  );
#endif // _BASE_OC_FRU_LIB_H_
