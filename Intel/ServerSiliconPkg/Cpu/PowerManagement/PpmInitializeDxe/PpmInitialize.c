/** @file
  Code File for CPU Power Management DXE Driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include "PpmInitialize.h"
#include <Protocol/DxeSmmReadyToLock.h>
#include <Protocol/CpuPpmProtocol.h>
#include <Library/CpuConfigLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/KtiApi.h>

EFI_CPU_PPM_PROTOCOL              mCpuPpmProtocol;
EFI_CPU_PM_STRUCT                 mPpmLib ={{0, 0, 0, 0 }};
extern PPM_WRAP_STRUCT            mPpmWrapPointer;

VOID
EFIAPI
CpuPmProgramMsrs (
  );

VOID   CpuPmProgramCsrs (
  VOID
  );

EFI_STATUS
CpuPpmInitDone (
  VOID
  );

VOID
EFIAPI
PpmOnEndOfDxe (
  IN      EFI_EVENT                 Event,
  IN      VOID                      *Context
  );

/**

  The function puts MSR programing and P state transition together to improve boot performance.

  Arguments: None

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
CpuPmMpInitialize (
  VOID
  )
{
  CpuPmProgramMsrs ();

  return EFI_SUCCESS;
}

/**

  The function puts SetConfigControlLockBit() and other PM routines (if any) together to improve boot performance.

  Arguments: None

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
CpuPmMpInitDone (
  VOID
  )
{
  UINTN                       ProcessorNumber;
  EFI_STATUS                  Status;
  UINT32                      ThreadNumber;

  Status = mMpService->WhoAmI (
                mMpService,
                &ProcessorNumber
                );
  ASSERT_EFI_ERROR (Status);

  GetProcessorLocation (
    ProcessorNumber,
    NULL,
    NULL,
    &ThreadNumber
    );

  //
  // Limit core scoped MSR manipulation to thread 0 for each core
  //
  if (ThreadNumber == 0) {
    //
    // Set LOCK bit in MSR_PMG_CST_CONFIG_CONTROL Bit[15]
    //
    SetConfigControlLockBit (mPpmPolicy);
  }

  return EFI_SUCCESS;
}

/**
  An empty function to pass error checking of CreateEventEx ().

  This empty function ensures that EVT_NOTIFY_SIGNAL_ALL is error
  checked correctly since it is now mapped into CreateEventEx() in UEFI 2.0.

  @param[in]  Event                 Event whose notification function is being invoked.
  @param[in]  Context               The pointer to the notification function's context,
                                which is implementation-dependent.
**/
VOID
EFIAPI
DummyFunction (
  IN EFI_EVENT                Event,
  IN VOID                     *Context
  )
{
  return;
}

/**
  Function to signal we are about to be done with BIOS INIT phase in PPM

  @retval N/A
**/
VOID
SignalPpmBeforeBiosInitDone (
  VOID
  )
{
  EFI_STATUS                 Status;
  EFI_EVENT                  PpmBiosInitDoneEvent;


  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  DummyFunction,
                  NULL,
                  &gPpmBeforeBiosInitDoneGuid,
                  &PpmBiosInitDoneEvent
                  );

  ASSERT_EFI_ERROR (Status);
  DEBUG ((EFI_D_INFO, "Signaling gPpmBeforeBiosInitDoneGuid Event..Before setting CPL3 and CPL4\n"));
  gBS->SignalEvent (PpmBiosInitDoneEvent);
  gBS->CloseEvent (PpmBiosInitDoneEvent);
}

/**

  Set BIOS init done to enable PPM features.

  @param PPMPolicy Pointer to PPM Policy protocol instance

  @retval EFI_SUCCESS
**/
EFI_STATUS
CpuPpmInitDone (
    VOID
    )
{
  UINT8                       SocketNumber;
  UINT8                       Index;
  UINTN                       ProcessorId;
  EFI_PROCESSOR_INFORMATION   ProcInfo;
  UINTN                       NumberOfProcessors;
  UINTN                       NumberEnabledProcessors;
  UINT16                      PackageDoneBitmap;
  UINT32                      AvailableMasterDieMap;
  UINT32                      AvailableSlaveDieMap;

  //Fix HSD 4986378: PROMOTE from haswell_server: BIOS 18D15 fails to achieve package C6 under Red Hat
  //Because IioInit driver may be after PPM driver
  //Set PCIE PkgCstEntryCriteria before hand off to OS
  for (SocketNumber = 0; SocketNumber < MAX_SOCKET; SocketNumber++) {
    if (mPpmInfo->SocketPresentBitMap & (1 << SocketNumber)){
      SetupPCIEPkgCstEntryCriteria (SocketNumber, mPpmPolicy);
    }
  }

  SocketNumber = 0;
  PackageDoneBitmap = 0;
  mMpService->GetNumberOfProcessors(
                  mMpService,
                  &NumberOfProcessors,
                  &NumberEnabledProcessors
                  );

  //
  // Count number of sockets present
  //
  for(Index = 0; Index < MAX_SOCKET; Index++){
    if(mPpmInfo->SocketPresentBitMap & (1 << Index)) {
      SocketNumber++;
    }
  }

  //
  // Set Package MSR lock bits for each socket
  //
  for(ProcessorId = 0; ProcessorId < NumberOfProcessors; ProcessorId++){
    mMpService->GetProcessorInfo(
                    mMpService,
                    ProcessorId,
                    &ProcInfo
                    );
    //
    // Only call SetThreadLockBit on each socket one time
    //
    if(!(PackageDoneBitmap & (1 << ProcInfo.Location.Package)) && (ProcInfo.StatusFlag & PROCESSOR_ENABLED_BIT)){
      PackageDoneBitmap |= (1 << ProcInfo.Location.Package);
      SocketNumber--;
      if(ProcInfo.StatusFlag & PROCESSOR_AS_BSP_BIT) {
        SetThreadLockBit (&mPpmWrapPointer);
      } else {
        mMpService->StartupThisAP(mMpService,
                            SetThreadLockBit,
                            ProcessorId,
                            NULL,
                            0,
                            (VOID *)&mPpmWrapPointer,
                            NULL
                            );
      }
      //
      // If we have reached the last socket, break loop
      //
      if(SocketNumber == 0){
        break;
      }
    }
  }

  //
  // Set CSR lock bits on all processors.
  //
  //DEBUG ( (EFI_D_ERROR, ":: Set LOCK bit in CSR 1:30:1:0xb0 & 1:30:2:0xc8...\n") );
  for (SocketNumber = 0; SocketNumber < MAX_SOCKET; SocketNumber++) {
    if (mPpmInfo->SocketPresentBitMap & (1 << SocketNumber)){
      SetPackageLockBit (SocketNumber, &mPpmLib, mPpmPolicy);
    }
  }

  //
  // Announce that we are going to be done with PPM
  //
  SignalPpmBeforeBiosInitDone ();

  //
  // Set BIOS Init Done on all processors.
  //
  // Rules for setting CPL:
  //    For each n, BIOS must set Slave Die RST_CPLn before setting the Master Die RST_CPLn in the same socket.
  //    For each n, BIOS must set Master Die RST_CPLn of all non legacy sockets before setting the Master Die RST_CPLn in legacy socket.
  //
  // Setting sequence:
  //    1. set each Slave Die in all sockets
  //    2. set each Master Die for all NonSBSP's
  //    3. set Master Die for SBSP
  //

  AvailableMasterDieMap = mPpmInfo->SocketPresentBitMap & ~(mPpmInfo->SlaveDieBitMap);
  AvailableSlaveDieMap  = mPpmInfo->SocketPresentBitMap & mPpmInfo->SlaveDieBitMap;


  if (!mPpmInfo->SkipSignalPpmDone) {
    // For each Slave Die in all sockets
    for (SocketNumber = 0; SocketNumber < MAX_SOCKET; SocketNumber++) {
      if (AvailableSlaveDieMap & (1 << SocketNumber)) {
        DEBUG((EFI_D_INFO, "Setting CPL on socket:%d\n", SocketNumber));
        SetBiosInitDone (SocketNumber, &mPpmWrapPointer);
      }
    }

    // Set each Master Die, first NonSBSP's, last SBSP
    for (SocketNumber = MAX_SOCKET; SocketNumber > 0; SocketNumber--) {
      if (AvailableMasterDieMap & (1 << (SocketNumber - 1))) {
        DEBUG((EFI_D_INFO, "Setting CPL on socket:%d\n", SocketNumber - 1));
        SetBiosInitDone (SocketNumber - 1, &mPpmWrapPointer);
      }
    }
  }

  mMpService->StartupAllAPs(mMpService,
                            (EFI_AP_PROCEDURE) CpuPmMpInitDone,
                            FALSE,
                            NULL,
                            0,
                            NULL,
                            NULL
                            );

  CpuPmMpInitDone ();

  //
  // Set Lock bit after BIOS Init Done on all processors.
  //
  for (SocketNumber = 0; SocketNumber < MAX_SOCKET; SocketNumber++) {
    if (mPpmInfo->SocketPresentBitMap & (1 << SocketNumber)){
      SetLockBitAfterBiosInitDone (SocketNumber, &mPpmWrapPointer);
    }
  }

  return EFI_SUCCESS;

}

/**

    Call back routine used to do the Power Management related Register Programming

    @param Event - PpmProcessorSupportProtocol Install

    @retval Status
    @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
PpmInitializeDxeEntry (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )

{
  EFI_STATUS                Status;
  EFI_EVENT                 EndOfDxeEvent;
  EFI_HANDLE                Handle = NULL;

  DEBUG ((EFI_D_ERROR, "DXE PPM Initialization Entry\n"));

  Status = EFI_SUCCESS;

  InitializeCpuInfoStruct();

  InitializeCpuPPMLib((EFI_CPU_PM_STRUCT *)&mPpmLib, mPpmPolicy);

  if (mPpmPolicy->RunCpuPpmInPei == 0) {
    // If Setup choice is to run PPM code in DXE

    DEBUG((EFI_D_ERROR, "Run PPM in DXE\n"));

    if (UbiosGenerationEnabled ()) {
      DEBUG ((EFI_D_INFO, "DXE PPM CSR Trace Start\n"));
    }
    CpuPmProgramCsrs();
    if (UbiosGenerationEnabled ()) {
      DEBUG ((EFI_D_INFO, "DXE PPM CSR Trace End\n"));
    }

    if(MsrTraceEnabled ()) {
      DEBUG ((EFI_D_INFO, "DXE PPM MSR Trace Phase1 Start\n"));
      SetMsrTraceOutputMode (MSR_OUTPUT_ENABLE);
    }

    mMpService->StartupAllAPs(mMpService,
                                (EFI_AP_PROCEDURE) CpuPmMpInitialize,
                                FALSE,
                                NULL,
                                0,
                                NULL,
                                NULL
                                );

    CpuPmMpInitialize ();

    if (MsrTraceEnabled ()) {
      DEBUG ((EFI_D_INFO, "DXE PPM MSR Trace Phase1 End\n"));
      SetMsrTraceOutputMode (MSR_OUTPUT_DISABLE);
    }
  }

  if(MsrTraceEnabled ()) {
    DEBUG ((EFI_D_INFO, "DXE PPM MSR Trace Phase2 Start\n"));
    SetMsrTraceOutputMode (MSR_OUTPUT_ENABLE);
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  PpmOnEndOfDxe,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  mCpuPpmProtocol.CpuPpmStruct = &mPpmLib;
  Status = gBS->InstallProtocolInterface (&Handle, &gEfiCpuPpmProtocolGuid, EFI_NATIVE_INTERFACE, &mCpuPpmProtocol);
  ASSERT_EFI_ERROR (Status);

  if (MsrTraceEnabled ()) {
    DEBUG ((EFI_D_INFO, "DXE PPM MSR Trace Phase2 End\n"));
    SetMsrTraceOutputMode (MSR_OUTPUT_DISABLE);
  }

  return Status;
}

/**

  Ppm EndOfDxe callback function

  @param Event   - The instance of the event triggering this function
  @param Context - Pointer to Context, not used

**/
VOID
EFIAPI
PpmOnEndOfDxe (
  IN   EFI_EVENT    Event,
  IN   VOID         *Context
  )
{
  EFI_STATUS Status;

  if(MsrTraceEnabled ()) {
    SetMsrTraceOutputMode (MSR_OUTPUT_ENABLE);
  }
  Status = CpuPpmInitDone();

  if(MsrTraceEnabled ()) {
    DEBUG ((EFI_D_ERROR, "DXE PPM MSR Trace End\n"));
    SetMsrTraceOutputMode (MSR_OUTPUT_DISABLE);
  }
  DEBUG ((EFI_D_ERROR, "\n\nDXE PPM Config Complete\n"));
  return;
}

/**

    Initialize Power Management MSRs for each thread.

    @param PPMPolicy Pointer to PPM Policy protocol instance

    @retval None
**/
VOID
EFIAPI
CpuPmProgramMsrs (
  VOID
  )
{
  UINTN                     ProcessorNumber = 0;

  mMpService->WhoAmI(mMpService, &ProcessorNumber);

  CommonPpmProgramMsrs (&mPpmLib, mPpmPolicy, ProcessorNumber);

  return;
}

/**

    Initialize Power Management CSRs for each socket.

    @param PPMPolicy Pointer to PPM Policy protocol instance

    @retval None
**/
VOID
CpuPmProgramCsrs (
  VOID
  )
{
  DEBUG ( (EFI_D_ERROR, " \n\n ::DXE Power Management CSR Programming\n\n") );

  CommonPpmProgramCsrs (&mPpmLib, mPpmPolicy, mPpmInfo->SocketPresentBitMap);
}

