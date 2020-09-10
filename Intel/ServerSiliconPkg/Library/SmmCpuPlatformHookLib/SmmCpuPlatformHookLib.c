/** @file
  CPU Configuration Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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

#include <Library/SmmCpuPlatformHookLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/BaseLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/SmmServicesTableLib.h>


typedef struct _SMM_RUNTIME_CTL_HOOKS_PROTOCOL {
  BOOLEAN PendSmiFlag;
  volatile UINT64 EmcaHandlerState;
} SMM_RUNTIME_CTL_HOOKS_PROTOCOL;

// {7B87048D-5272-472B-A7B5-4166ADA05256}
static EFI_GUID mSmmRuntimeCtlHooksGuid =
{ 0x7b87048d, 0x5272, 0x472b, { 0xa7, 0xb5, 0x41, 0x66, 0xad, 0xa0, 0x52, 0x56 } };

SMM_RUNTIME_CTL_HOOKS_PROTOCOL *mSmmRuntimeCtlHooksProtocol;
SMM_RUNTIME_CTL_HOOKS_PROTOCOL  SmmRuntimeCtlHooksProtocol;

/**
  Checks if platform produces a valid SMI.

  This function checks if platform produces a valid SMI. This function is
  called at SMM entry to detect if this is a spurious SMI. This function
  must be implemented in an MP safe way because it is called by multiple CPU
  threads.

  @retval TRUE              There is a valid SMI
  @retval FALSE             There is no valid SMI

**/
BOOLEAN
EFIAPI
PlatformValidSmi (
  VOID
  )
{
  return TRUE;
}


/**
  Clears platform top level SMI status bit.

  This function clears platform top level SMI status bit.

  @retval TRUE              The platform top level SMI status is cleared.
  @retval FALSE             The paltform top level SMI status cannot be cleared.

**/
BOOLEAN
EFIAPI
ClearTopLevelSmiStatus (
  VOID
  )
{
  return TRUE;
}


/**
  Performs platform specific way of SMM BSP election.

  This function performs platform specific way of SMM BSP election.

  @param  IsBsp             Output parameter. TRUE: the CPU this function executes
                            on is elected to be the SMM BSP. FALSE: the CPU this
                            function executes on is to be SMM AP.

  @retval EFI_SUCCESS       The function executes successfully.
  @retval EFI_NOT_READY     The function does not determine whether this CPU should be
                            BSP or AP. This may occur if hardware init sequence to
                            enable the determination is yet to be done, or the function
                            chooses not to do BSP election and will let SMM CPU driver to
                            use its default BSP election process.
  @retval EFI_DEVICE_ERROR  The function cannot determine whether this CPU should be
                            BSP or AP due to hardware error.

**/
EFI_STATUS
EFIAPI
PlatformSmmBspElection (
  OUT BOOLEAN     *IsBsp
  )
{
  if(IsBsp != NULL) {
    *IsBsp = FALSE;
  }

  return EFI_NOT_READY;
}

/**
  Get platform page table attribute .

  This function gets page table attribute of platform.

  @param  Address        Input parameter. Obtain the page table entries attribute on this address.
  @param  PageSize       Output parameter. The size of the page.
  @param  NumOfPages     Output parameter. Number of page.
  @param  PageAttribute  Output parameter. Paging Attributes (WB, UC, etc).

  @retval EFI_SUCCESS       The platform page table attribute from the address is determined.
  @retval EFI_UNSUPPORTED   The paltform not supoort to get page table attribute from the address.

**/
EFI_STATUS
EFIAPI
GetPlatformPageTableAttribute (
  IN  UINT64                Address,
  IN OUT SMM_PAGE_SIZE_TYPE *PageSize,
  IN OUT UINTN              *NumOfPages,
  IN OUT UINTN              *PageAttribute
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Init internal variables

**/
EFI_STATUS
EFIAPI
InitSmmRuntimeCtlHooks(
  VOID
)
{
  EFI_STATUS Status;
  EFI_HANDLE Handle;

  mSmmRuntimeCtlHooksProtocol = NULL;

  Status = gSmst->SmmLocateProtocol (
                    &mSmmRuntimeCtlHooksGuid,
                    NULL,
                    &mSmmRuntimeCtlHooksProtocol
                    );
  if(Status == EFI_NOT_FOUND) {
    // Register Platform Error Handling Protocol
    Handle = NULL;
    SmmRuntimeCtlHooksProtocol.EmcaHandlerState = 0;
    SmmRuntimeCtlHooksProtocol.PendSmiFlag = 0;
    Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &mSmmRuntimeCtlHooksGuid,
                    EFI_NATIVE_INTERFACE,
                    &SmmRuntimeCtlHooksProtocol
                    );
    if(Status != EFI_SUCCESS) {
      return Status;
    }

    Status = gSmst->SmmLocateProtocol (
                    &mSmmRuntimeCtlHooksGuid,
                    NULL,
                    &mSmmRuntimeCtlHooksProtocol
                    );

    if(Status != EFI_SUCCESS) {
      return Status;
    }
  }

  if(mSmmRuntimeCtlHooksProtocol != NULL) {
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_READY;
  }
}


/**
  SMM Runtime Control in SMM Rendezvous
  
  This function clears pending SMI and turns on MSMI to MCE morphing.
  
  @param[in,out] Buffer  Pointer to private data buffer.

**/
VOID
EFIAPI
SmmRuntimeControlInSmmRendezvous (
  IN OUT VOID  *Buffer
)
{
  MSR_SMM_CLEAR_SMI_REGISTER ClearSmiReg;

  //
  //Note: MSR_SMM_CLEAR_SMI is write-only for BIOS.
  //
  ClearSmiReg.Uint64 = 0;
  ClearSmiReg.Bits.ClearSmi = 1;
  ClearSmiReg.Bits.MsmiToMceEn = 1;
  AsmWriteMsr64 (MSR_SMM_CLEAR_SMI, ClearSmiReg.Uint64);
}

/**
  Morphing off

  This function turns off MSMI to MCE morphing by clearing MsmiToMceEn bit in SMM_CLEAR_SMI MSR.

  @param[in,out] Buffer  Pointer to private data buffer.

**/
VOID
EFIAPI
TurnOffMorphing (
  IN OUT VOID  *Buffer
)
{
  //
  //BIOS turns off MSMI to MCE morphing before exit SMM.
  //
  AsmWriteMsr64 (MSR_SMM_CLEAR_SMI, 0);
}

/**
  Set Flag to Cause Smm Cpu to pend SMI IPI at end of the flow

  @param Value             Value of the flag.
**/
VOID
EFIAPI
SetPendSmiFlag(
  BOOLEAN Value
  )
{
  if(mSmmRuntimeCtlHooksProtocol != NULL) {
    mSmmRuntimeCtlHooksProtocol->PendSmiFlag = Value;
  }
}

/**
  Get value of Flag that Cause Smm Cpu to pend SMI IPI at end of the flow

  @retval TRUE              SMI will be pended.
  @retval FALSE             SMI will not pended.

**/
BOOLEAN
EFIAPI
GetPendSmiFlag(
  VOID
  )
{
  if(mSmmRuntimeCtlHooksProtocol != NULL) {
    return mSmmRuntimeCtlHooksProtocol->PendSmiFlag;
  } else {
    return FALSE;
  }
}

/**
  Performs an atomic compare exchange operation on EmcaHandlerState.

  Performs an atomic compare exchange operation on the EmcaHandlerState to make state transition.
  If original value is equal to CompareValue, then Value is set to ExchangeValue and
  CompareValue is returned.  If original value is not equal to CompareValue, then original value is returned.
  The compare exchange operation must be performed using MP safe mechanisms.


  @param  CompareValue  64-bit value used in compare operation.
  @param  ExchangeValue 64-bit value used in exchange operation.

  @return The original EmcaHandlerState before exchange.

**/
UINT64
EFIAPI
TransitionEmcaHandlerState(
  IN UINT64 CompareValue,
  IN UINT64 ExchangeValue
  )
{
  if(mSmmRuntimeCtlHooksProtocol != NULL) {
    return InterlockedCompareExchange64((UINT64*) &mSmmRuntimeCtlHooksProtocol->EmcaHandlerState, CompareValue, ExchangeValue);
  } else {
    return 0;
  }
}

/**
  Get value of EmcaHandlerState

  @return EmcaHandlerState value.

**/
UINT64
EFIAPI
GetEmcaHandlerState(
  VOID
  )
{
  if(mSmmRuntimeCtlHooksProtocol != NULL) {
    return mSmmRuntimeCtlHooksProtocol->EmcaHandlerState;
  } else {
    return 0;
  }
}

/**
  Set value of EmcaHandlerState

  @param EmcaHandlerState value.

**/
VOID
EFIAPI
SetEmcaHandlerState(
  UINT64 Value
  )
{
  if(mSmmRuntimeCtlHooksProtocol != NULL) {
    mSmmRuntimeCtlHooksProtocol->EmcaHandlerState = Value;
  }
}

