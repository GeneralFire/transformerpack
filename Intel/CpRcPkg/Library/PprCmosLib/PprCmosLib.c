/** @file
  Implementation of Ppr CMOS Library

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


#include <Uefi.h>
#include <Base.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PprCmosLib.h>
#include <Library/IoLib.h>
#include <MrcCmos.h>
#include <Platform.h>
#include <BackCompatible.h> 
#include <Library/SemaphoreLib.h>

/**
  Function to check if we already consumed the PPR Data
  Mapping of Bits as follows, Bit0 -> Socket0..Bit7 -> Socket7
  Accesses CMOS with IO operation on Multithreaded environment, so
  added semaphore to avoid race condtion

  @param[in] Socket - 0 Based Socket number

  @retval TRUE   -  PPR Data has been consumed
  @retval FALSE  -  PPR DATA has not been consumed
**/
BOOLEAN
EFIAPI
IsPprDataConsumed (
  IN UINT8  Socket
  )
{
  BOOLEAN DataConsumed = FALSE;
  UINT8   PprFlowStatus = 0;
  BOOLEAN     SemaphoreAcquired = FALSE;
  UINT32      QueueNumber = 0;

  SemaphoreAcquired = AcquireSocketSemaphore (Socket, SystemSemaphore0, ADD_TO_QUEUE, &QueueNumber);

  if (!SemaphoreAcquired) {
    while (!IsSocketSemaphoreOwned (Socket, SystemSemaphore0, QueueNumber));
  }

  IoWrite8 (R_IOPORT_CMOS_STANDARD_INDEX, PPR_MRC_STATUS_OFFSET);
  PprFlowStatus = IoRead8 (R_IOPORT_CMOS_STANDARD_DATA);
  if ((PprFlowStatus & (1 << Socket)) != 0) {
    DataConsumed = TRUE;
  }

  ReleaseSocketSemaphore (Socket, SystemSemaphore0);
  return DataConsumed;
}

/**
  Clear PPR CMOS Bit to 0, for first boot
**/
VOID
EFIAPI
InitalizePprCmosStatus (
  VOID
  )
{
  ResetPprDataConsumed ();
}

/**
  Function to indicate PPR data is consumed
  Mapping of Bits as follows, Bit0 -> Socket0..Bit7 -> Socket7
  Accesses CMOS with IO operation on Multithreaded environment, so
  added semaphore to avoid race condtion

  @param[in] Socket - 0 Based Socket number

  @retval None
**/
VOID
EFIAPI
SetPprDataConsumed (
  IN UINT8  Socket
  )
{
  UINT8 PprFlowStatus = 0;
  BOOLEAN     SemaphoreAcquired = FALSE;
  UINT32      QueueNumber = 0;

  SemaphoreAcquired = AcquireSocketSemaphore (Socket, SystemSemaphore0, ADD_TO_QUEUE, &QueueNumber);

  if (!SemaphoreAcquired) {
    while (!IsSocketSemaphoreOwned (Socket, SystemSemaphore0, QueueNumber));
  }

  IoWrite8 (R_IOPORT_CMOS_STANDARD_INDEX, PPR_MRC_STATUS_OFFSET);
  PprFlowStatus = IoRead8 (R_IOPORT_CMOS_STANDARD_DATA);

  PprFlowStatus |= (1 << Socket);

  IoWrite8 (R_IOPORT_CMOS_STANDARD_DATA, PprFlowStatus);

  ReleaseSocketSemaphore (Socket, SystemSemaphore0);
}

/**
  Function to Get PPR CMOS Status byte

  @retval UINT8  - CMOS Value of PPR Status
**/
UINT8
EFIAPI
GetPprCmosStatus (
  VOID
  )
{
  UINT8 CmosValue = 0;

  IoWrite8 (R_IOPORT_CMOS_STANDARD_INDEX, PPR_MRC_STATUS_OFFSET);
  IoRead8 (R_IOPORT_CMOS_STANDARD_DATA);

  return CmosValue;
}

/**
  Function to indicate fresh PPR data may be available
**/
VOID
EFIAPI
ResetPprDataConsumed (
  VOID
  )
{
  UINT8 PprFlowStatus = 0;

  IoWrite8 (R_IOPORT_CMOS_STANDARD_INDEX, PPR_MRC_STATUS_OFFSET);
  PprFlowStatus = IoRead8 (R_IOPORT_CMOS_STANDARD_DATA);
  PprFlowStatus = 0;
  IoWrite8 (R_IOPORT_CMOS_STANDARD_DATA, PprFlowStatus);
}
