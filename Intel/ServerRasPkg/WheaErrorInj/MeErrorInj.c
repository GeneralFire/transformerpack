/** @file
  This file contains the functionality for the ME Error Injection.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Ppi/MeUmaPpi.h>
#include <Library/MmPciBaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/RasDebugLib.h>
#include <Library/MpExtensionLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/MeTypeLib.h>
#include <Protocol/RasPolicy.h>
#include <Protocol/SmmBase2.h>
#include <Library/PlatPolicyLib.h>
#include <Library/SmmPeriodicTimerLib.h>
#include <Guid/MemoryMapData.h>
#include <RcRegs.h>
#include <SiliconSetting.h>
#include "WheaErrorInj.h"
#include <Library/ChaIpLib/Include/ChaRegs.h>

   UINT32                    mMESegInjAddr;


extern   WHEA_EINJ_TRIGGER_ACTION_TABLE                *mEinjAction;
extern   WHEA_EINJ_PARAM_BUFFER                        *mEinjParam ;
extern   UINT64                                        mUmaErrorInjectionType;


/**
  Set MC9_CTL[bit 5] to 0 before ME Error Injection

  @param NONE

  @retval NONE
**/
VOID
PreMC9CtlSetupForMEErrInj (
  IN OUT VOID * Context
  )
{
  AsmMsrAnd64((EFI_IA32_MC0_CTL + (9*4)),~(UINT64)(BIT5));
}

/**
  Set MC9_CTL[bit 5] to 1 after ME Error Injection

  @param NONE

  @retval NONE
**/
VOID
PostMC9CtlSetupForMEErrInj (
  IN OUT  VOID * Context
  )
{
  AsmMsrOr64((EFI_IA32_MC0_CTL + (9*4)),BIT5);
}

/**
  Do the pre-setup for ME Error Injection

  @param NONE

  @retval NONE
**/
VOID
PreMESegErrInjSetup (
  VOID
  )
{
  UINT8                             Skt;
  MESEG_LIMIT_N0_CHABC_SAD_STRUCT   MeMemLimit;

  // Step 1:  set MC9[Bit5] = 0
  MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, PreMC9CtlSetupForMEErrInj, NULL);


  // Step 2: MESEG_LIMIT.EN = 0
  for (Skt = 0; Skt < MAX_SOCKET; Skt++) {
    if (IsSocketPresent(Skt) == FALSE) {
      continue;
    }

    MeMemLimit.Data = ReadCpuCsr (Skt, 0, MESEG_LIMIT_N0_CHABC_SAD_REG);
    MeMemLimit.Bits.en = 0;
    WriteCpuCsr (Skt, 0, MESEG_LIMIT_N0_CHABC_SAD_REG, MeMemLimit.Data);
  }
}

/**
  Do the post-setup for ME Error Injection

  @param NONE

  @retval NONE
**/
VOID
PostMESegErrInjSetup (
  VOID
  )
{
  UINT8                             Skt;
  MESEG_LIMIT_N0_CHABC_SAD_STRUCT   MeMemLimit;

  // Step 1: MESEG_LIMIT.EN = 1
  for (Skt = 0; Skt < MAX_SOCKET; Skt++) {
    if (IsSocketPresent(Skt) == FALSE) {
      continue;
    }

    MeMemLimit.Data = ReadCpuCsr (Skt, 0, MESEG_LIMIT_N0_CHABC_SAD_REG);
    MeMemLimit.Bits.en = 1;
    WriteCpuCsr (Skt, 0, MESEG_LIMIT_N0_CHABC_SAD_REG, MeMemLimit.Data);
  }

  // Step 2:  Set MC9[Bit5] to 1
  MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, PostMC9CtlSetupForMEErrInj, NULL);

}

/**
  Corrupt the data the ME Seg Injection address offset

  @param NONE

  @retval NONE
**/
VOID
CorruptMESegInjOffset (
  VOID
)
{
  UINT32 Data32;

  if (mMESegInjAddr != 0) {
    Data32  = *( UINT32 *)(UINTN)(mMESegInjAddr);
    Data32 = Data32 ^ ((UINT32)-1);
    *( UINT32 *)(UINTN)(mMESegInjAddr) = Data32;
  }
  return;
}

/**

  Call ME interface to get a page offset for ME Error Injection

  @param NONE

  @retval EFI_SUCCESS:              ME Page allocated Successfully
  @retval EFI_OUT_OF_RESOURCES:     ME Page cannot be allocated
  @retval EFI_UNSUPPORTED           Unsupported, No SPS support
**/
EFI_STATUS
GetMEPageForErrorInj (
  VOID
  )
{
  EFI_STATUS   Status = EFI_SUCCESS;

#if ME_SUPPORT_FLAG
  Status = EFI_UNSUPPORTED;
#endif // ME_SUPPORT_FLAG

  return Status;
}

/**
  Periodic SMI handler to get the status from ME Seg Error consumption.
  ME will provide the status in around 5 ms after the error consumption is initiated

  @param Handle             - The unique handle assigned to this handler by SmiHandlerRegister().Type
                                            EFI_HANDLE is defined in InstallProtocolInterface() in the UEFI 2.1 Specification.
  @param mPeriodicTimer     -  Pointer to the DispatchContext

  @param CommBuffer         - A pointer to a collection of data in memory that will be conveyed from a non-SMM
                                           environment into an SMM environment. The buffer must be contiguous, physically mapped, and be a physical address.
  @param CommBufferSize     - The size of the CommBuffer.

  @retval EFI_SUCCESS       - The interrupt was handled and quiesced. No other handlers should still be called.

**/


EFI_STATUS
GetMeEInjConsumeStatus (
  IN  EFI_HANDLE     Handle,
  IN  CONST VOID     *mPeriodicTimer,
  IN  OUT VOID      *CommBuffer  OPTIONAL,
  IN  OUT UINTN     *CommBufferSize OPTIONAL
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  //
  // Place holder for the Get Status call from ME for Error consumption
  // Replace with the correct call after ME Provided the support
  //

  //
  // There is an error loading HeciSmm3 protocol, which is causing the WheaErrorInj driver not loading
  // So, not including the MESEG code below in the build.  This will be enabled once the HeciSmm3 protocol issue is fixed
  //
  //Status = MeUmaEinjConsume (MeEinjModeDeffered);

  mEinjParam->OpStatus = 0;

  if (Status == EFI_SUCCESS){
    mEinjParam->CmdStatus = WHEA_EINJ_CMD_SUCCESS;
  } else {
    mEinjParam->CmdStatus = WHEA_EINJ_CMD_UNKNOWN_FAILURE;
  }

  // Unregister periodic SMI handler
  EndSmmPeriodicTimer();

  return Status;

}

/**

Call ME interface to initiate the Error consumption

@retval EFI_SUCCESS : ME EINJ_CONSUME command successful.
@retval EFI_ABORTED             ME EINJ_CONSUME command Failed

**/

EFI_STATUS
InitiateUmaErrorConsume(
  VOID
)
{
  EFI_STATUS                                Status = EFI_SUCCESS;
#if ME_SUPPORT_FLAG

  RAS_DEBUG((LEVEL_REG, " In MeSeg Error Injection SWSMI Handler\n"));

  Status = EFI_UNSUPPORTED;
#endif // ME_SUPPORT_FLAG

  return Status;
}


