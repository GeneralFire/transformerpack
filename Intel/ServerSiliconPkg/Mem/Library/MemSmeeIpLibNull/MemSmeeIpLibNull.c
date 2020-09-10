/** @file
  Defines SMEE IP API related NULL functions

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

#include <Uefi.h>

/**
  Program TME CMI registers

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number
  @param[in] ProgramableValue  - Programmable CMI credit value

  @retval N/A
**/
VOID
EFIAPI
ProgramTmeCmiRegisters (
  IN UINT8   Socket,
  IN UINT8   Ch,
  IN UINT8   ProgramableValue
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Check if TME enabled in given Socket

  @param[in] Socket       - Socket number, 0 based

  @retval TRUE -  TME is enabled
  @retval FALSE - TME is disabled
**/
BOOLEAN
EFIAPI
IsTmeEnabled (
  IN UINT8 Socket
  )
{
  //
  // Place holder function for NULL Library
  //
  return FALSE;
}


/**
  Program config done bit in TME CMI registers

  @param[in] Socket       - Socket number
  @param[in] McId         - Memory Controller number

  @retval N/A
**/
VOID
EFIAPI
ProgramTmeCmiConfigDoneAllCh (
  IN UINT8 Socket,
  IN UINT8 McId
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Wait for Initialized bit set for TME CMI registers. The CMI initialized
  bit will be polled till CATCHALL_TIMEOUT = 100 milliseconds. If bit dint get
  set then its a fatal condition and fatal error will be reported by this function.

  @param[in] Socket       - Socket number
  @param[in] McId         - Memory Controller number

  @retval N/A
**/
VOID
EFIAPI
WaitForTmeCmiConfigCompleteAllCh (
  IN UINT8 Socket,
  IN UINT8 McId
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Enable PRMRR Security

  @param[in] Socket       - Socket number
  @param[in] ChId         - Channel Number

  @retval N/A
**/
VOID
EFIAPI
EnablePrmrrSecurity (
  IN UINT8 Socket,
  IN UINT8 ChId
  )
{
  //
  // Place holder function for NULL Library
  //
}