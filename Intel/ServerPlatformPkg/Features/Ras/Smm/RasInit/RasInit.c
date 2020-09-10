/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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

#include "RasInit.h"
#include <Library/PmcLib.h>

#ifndef __GNUC__
#pragma optimize("", off)
#endif  //__GNUC__

UINT32                  PmRegSmiEnSave;
UINT16                  PmRegAltSmiEnSave;
UINT16                  mAcpiBaseAddr;
BOOLEAN                 mIsSoftSmiRequest;
BOOLEAN                 mExitImmediately;
UINT32                  mSmiParam[4];
UINT8                   mFirstTimerEntry;
UINT8                   mRasTableIndex;
UINT8                   mKtiCpuSktHotPlugEn;
UINT8                   FirstCpuRasEntry;
UINT8                   FirstIioRasEntry;
UINT32                  *mStatusAddress;
BIOS_ACPI_PARAM         *mAcpiParameter;
UINT64                  mTsegUcBase = (UINT64)-1;
UINT8                   mHpRequest[6];
UINT32                  mSwSmiStatus;
BOOLEAN                 mSwSmiHandlerEnabled;
EFI_CPU_RAS_PROTOCOL                        *mCpuRas;
EFI_IIO_RAS_PROTOCOL                        *mIioRasProtocol;
EFI_HP_IOX_ACCESS_PROTOCOL                  *mHpIOXAccess;
EFI_SMM_PERIODIC_TIMER_DISPATCH2_PROTOCOL   *mSmmPeriodicTimerDispatch;
static EFI_SMM_CPU_SERVICE_PROTOCOL         *mSmmCpuServiceProtocol;
EFI_HANDLE                                  mPeriodicTimerHandle;
CPU_CSR_ACCESS_VAR                          *mCpuCsrAccessVarPtr;
NEXT_STATE_ENTRY                            mNSE;
SPS_RAS_NOTIFY_PROTOCOL                     *mSpsRasNotifyProtocol;

UINTN RasDispatchTable[] = {

  (UINTN)&GetSourceOfGpi,
// APTIOV_SERVER_OVERRIDE_RC_START
#if  RAS_CPU_ONLINE_OFFLINE_ENABLE == 1
// APTIOV_SERVER_OVERRIDE_RC_END
  (UINTN)&CpuRasHandler,
// APTIOV_SERVER_OVERRIDE_RC_START
#endif
// APTIOV_SERVER_OVERRIDE_RC_END
  //(UINTN)&IioRasHandler,
  //(UINTN)&MemRasHandler,
  (UINTN)-1,

};

//
// Driver entry point
//

EFI_STATUS
EFIAPI
RasInitStart (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{

  EFI_STATUS                     Status;
  EFI_GLOBAL_NVS_AREA_PROTOCOL   *AcpiNvsProtocol;
  EFI_IIO_UDS_PROTOCOL           *IioUds;
  IIO_UDS                        *IioUdsData;
  SYSTEM_MEMORY_MAP_HOB          *SystemMemoryMap;

  AcpiNvsProtocol = NULL;
  IioUds = NULL;
  IioUdsData = NULL;

  Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid, NULL, &IioUds);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RasInitStart : Can not locate EfiIioUdsProtocol \n"));
    return Status;
  }

  IioUdsData = (IIO_UDS *)IioUds->IioUdsPtr;

  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    ASSERT_EFI_ERROR (EFI_NOT_FOUND);
    DEBUG ((DEBUG_ERROR, "RasInitStart : GetSystemMemoryMapData failed \n"));
    return EFI_UNSUPPORTED;
  }

  if (SystemMemoryMap->SystemRasType != ADVANCED_RAS) {
    return Status;
  }

  Status = gSmst->SmmLocateProtocol (&gEfiSmmPeriodicTimerDispatch2ProtocolGuid, NULL, (VOID **)&mSmmPeriodicTimerDispatch);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gEfiGlobalNvsAreaProtocolGuid, NULL, &AcpiNvsProtocol);
  ASSERT_EFI_ERROR (Status);
  mAcpiParameter = AcpiNvsProtocol->Area;
  ASSERT (mAcpiParameter);

  Status = gSmst->SmmLocateProtocol (&gEfiHpIoxAccessGuid, NULL, &mHpIOXAccess);
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmLocateProtocol (&gEfiCpuRasProtocolGuid, NULL, &mCpuRas);
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmLocateProtocol (&gEfiIioRasProtocolGuid, NULL, &mIioRasProtocol);
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmLocateProtocol (&gEfiSmmCpuServiceProtocolGuid, NULL, &mSmmCpuServiceProtocol);
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmLocateProtocol (&gSpsRasNotifyProtocolGuid, NULL,&mSpsRasNotifyProtocol);

  mKtiCpuSktHotPlugEn = IioUdsData->SystemStatus.OutKtiCpuSktHotPlugEn;

  mCpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();

  // Init Memory Ras
  InitMemoryRas ();

  InitHpGPI();

  Status = EnableGpiChildHandler ();


  return Status;
}

VOID
InitHpGPI(
  VOID
  )
{
  UINT32 Data32;
  GPIO_CONFIG                 GpioPadConfig;
  GPIO_PAD                    CpuHpSmiPad;
  EFI_STATUS                  Status;

  ZeroMem (&GpioPadConfig, sizeof(GPIO_CONFIG));
  Status = GpioGetCpuHpSmiPad (&CpuHpSmiPad);

  GpioGetPadConfig (CpuHpSmiPad, &GpioPadConfig);

  // set the GPIO PAD mode to GPIO
  GpioPadConfig.PadMode = GpioPadModeGpio;

  // Set the GPIO PAD as IN and enable INV
  GpioPadConfig.Direction = GpioDirInInv;

  // Enable SMI Interrupt
  GpioPadConfig.InterruptConfig = GpioIntSmi | GpioIntLevel;

  // Set the Pad Ownership to ACPI
  GpioPadConfig.HostSoftPadOwn = GpioHostOwnAcpi;

  // Do the PAD configuration
  GpioSetPadConfig (CpuHpSmiPad, &GpioPadConfig);

  // Get PM ACPI Base Address (Pmbase)
  mAcpiBaseAddr = PmcGetAcpiBase ();

  // Setup the GBL_SMI_EN=1 to only cause SMI.
  Data32 = IoRead32 ((UINTN) (mAcpiBaseAddr + R_ACPI_IO_SMI_EN));
  Data32 |= B_ACPI_IO_SMI_EN_GBL_SMI;
  IoWrite32 ((mAcpiBaseAddr + R_ACPI_SMI_EN), Data32);

  // clear the status
  GpioClearGpiSmiSts (CpuHpSmiPad);


}

EFI_STATUS
EnableGpiChildHandler (
  )
{

  EFI_STATUS                    Status = EFI_SUCCESS;

  EFI_SMM_GPI_DISPATCH2_PROTOCOL *mGpiDispatch;
  EFI_SMM_GPI_REGISTER_CONTEXT   mGpiContext;
  EFI_HANDLE                     GpiHandle;
  GPIO_PAD                       CpuSwSmiPad;

  //
  // Register SMI handler through a child protocol
  //
  Status = gSmst->SmmLocateProtocol (&gEfiSmmGpiDispatch2ProtocolGuid, NULL, &mGpiDispatch);
  ASSERT_EFI_ERROR (Status);

  Status = GpioGetCpuHpSmiPad (&CpuSwSmiPad);
  GpioGetGpiSmiNum (CpuSwSmiPad, &mGpiContext.GpiNum);

  Status = mGpiDispatch->Register (
                          mGpiDispatch,
                          RasInitGpiChildCallback,
                          &mGpiContext,
                          &GpiHandle
                          );
  ASSERT_EFI_ERROR (Status);

  return Status;
}


EFI_STATUS
EFIAPI
RasInitGpiChildCallback (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID     *mGpiContext,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{

  EFI_SMM_GPI_REGISTER_CONTEXT *Context;
  BOOLEAN                      GpioSmiStsStatus;

  EFI_STATUS                   Status;
  GPIO_PAD                     CpuHpSmiPad;

  Status = GpioGetCpuHpSmiPad (&CpuHpSmiPad);

  Context = (EFI_SMM_GPI_REGISTER_CONTEXT *)mGpiContext;

  Status = GpioGetGpiSmiSts (CpuHpSmiPad, &GpioSmiStsStatus);

  if(GpioSmiStsStatus) {
    HpGPIHandler ();
  }

  return EFI_SUCCESS;
}


VOID
HpGPIHandler(
)
{
  BOOLEAN     OnlineOfflineRequestFlag;
  UINT32      Data32 = 0;
  UINTN       Idx;
  EFI_STATUS  Status;
  GPIO_PAD    CpuHpSmiPad;

  DEBUG_ACCESS(0x9FFF0);

  //
  // If it is the middle of another RAS service, do not response to the new request
  //
  if (mPeriodicTimerHandle) {
    DEBUG((EFI_D_LOAD|EFI_D_INFO, "\nAlready processing a RAS periodic timer event. Ignoring this event!\n"));
    return;
  }

  Status = GpioGetCpuHpSmiPad (&CpuHpSmiPad);

  //
  // Check the cause of the SMI
  // It could be the real RAS HPGPI SMI triggered by hardware
  // or faked HPGPI SMI requested from OS or other RAS modules
  // The difference is the INV bit
  // If it is 1, then it is the real  HPGPI SMI
  // If it is 0, then it is the faked HPGPI SMI
  //

  DEBUG     ((EFI_D_INFO, "\n\n  RAS FLOW START\n  RAS FLOW START\n  RAS FLOW START\n\n"));

  Status = GpioGetInputInversion (CpuHpSmiPad, &Data32);

  if (mSwSmiHandlerEnabled == TRUE) {
    Data32 = 0;
  }

  if (!(Data32)) {//Soft SMI detected
    GpioSetInputInversion (CpuHpSmiPad, TRUE); //TRUE - means invert input

    if (mAcpiParameter->SmiRequestParam[0] > 0) {
      DEBUG((EFI_D_LOAD|EFI_D_INFO, "\nThe request is a Soft SMI Request\n"));
      mIsSoftSmiRequest = TRUE;
    } else {
      DEBUG((EFI_D_LOAD|EFI_D_INFO, "\nRogue Soft SMI Detected.\n"));
      for (Idx=0; Idx < 4; Idx++) { //Clear out these parameters or we'll continue to process Rogue SMIs.
        mAcpiParameter->SmiRequestParam[Idx] = 0;
      }
      mIsSoftSmiRequest = FALSE;
    }
  } else {
    DEBUG((EFI_D_LOAD|EFI_D_INFO, "\nThe request is a Real SMI Request\n"));
    mIsSoftSmiRequest = FALSE;
  }

  //
  // Check cause of GPI source
  //
  Status = IN_PROGRESS;
  OnlineOfflineRequestFlag  = FALSE;
  mStatusAddress = NULL;
  if (mIsSoftSmiRequest) {
    //
    // Zero out the request data structure so that caller will know it acknowledged the request
    //
    for (Idx = 0; Idx < 4; Idx++) {
      mSmiParam[Idx] = mAcpiParameter->SmiRequestParam[Idx];
    }
    if (mSmiParam[0]) {
      OnlineOfflineRequestFlag = TRUE;
    }
    if (mSmiParam[3]) {
       mStatusAddress  = (UINT32*)(UINTN)mSmiParam[3];
      //
      // All errors returned at this address need to be limited to 1 byte
      // for compatibility with various OS interfaces.
      //
      *mStatusAddress  = (UINT8) Status;
    }
  } else {
    mAcpiParameter->SmiRequestParam[0] = (UINT32)(-1);
    OnlineOfflineRequestFlag = mHpIOXAccess->CheckOnlineOfflineRequest();
  }

  mSmiParam[3] = (UINT32)Status;
  mAcpiParameter->SmiRequestParam[3] = (UINT32)Status;

  // If not related to RAS HP, return.
  if (OnlineOfflineRequestFlag == FALSE) {
    DEBUG     ((EFI_D_INFO | EFI_D_ERROR, "    Confirmed that no RAS action detected. RAS FLOW COMPLETE.\n"));
    return;
  }

  if (mSwSmiHandlerEnabled == FALSE) {
    // Setup periodic SMI handler and Disable all other sources SMI.
    Status  = gSmst->SmmIo.Io.Read (&gSmst->SmmIo, SMM_IO_UINT32, (mAcpiBaseAddr + R_ACPI_SMI_EN), 1, &PmRegSmiEnSave);
    Data32  = PmRegSmiEnSave & (BIT31 + BIT30 + BIT0 + BIT1);
    Status  = gSmst->SmmIo.Io.Write (&gSmst->SmmIo, SMM_IO_UINT32, (mAcpiBaseAddr + R_ACPI_SMI_EN), 1, &Data32);
    Data32  |= BIT14; //Periodic SMI Enable
    Status  = gSmst->SmmIo.Io.Write (&gSmst->SmmIo, SMM_IO_UINT32, (mAcpiBaseAddr + R_ACPI_SMI_EN), 1, &Data32);

    //
    // TO DO:  Need to add code to save SMI setting on all GPIOs and disable SMI on all GPIO
    //

    // Register periodic SMI handler
    RegisterPeriodicTimer ();
  } else {
    Status = RasSmiHandler ();
  }

}

VOID
RegisterPeriodicTimer(
  VOID
  )
{
  EFI_SMM_PERIODIC_TIMER_REGISTER_CONTEXT   PeriodicTimer;
  EFI_STATUS  Status;

  mFirstTimerEntry  = 01;

  if(!mPeriodicTimerHandle) {
    PeriodicTimer.Period = 160000;
    PeriodicTimer.SmiTickInterval = 160000;
    Status = mSmmPeriodicTimerDispatch->Register(mSmmPeriodicTimerDispatch, RasPeriodicChildCallback, &PeriodicTimer, &mPeriodicTimerHandle);
    ASSERT_EFI_ERROR (Status);
    DEBUG((EFI_D_LOAD|EFI_D_INFO, "\nRAS Periodic Handler Registered\n"));
  }
}


EFI_STATUS
EFIAPI
RasPeriodicChildCallback (
  IN  EFI_HANDLE    PeriodicTimerHandle,
  IN  CONST VOID    *mPeriodicTimer,
  IN  OUT VOID      *CommBuffer  OPTIONAL,
  IN  OUT UINTN     *CommBufferSize OPTIONAL
  )
{
  EFI_STATUS  Status;

  // Call the RAS periodic timer handler.
  Status = RasPeriodicTimerSmiHandler ();

  return Status;
}


EFI_STATUS
RasPeriodicTimerSmiHandler(
  VOID
  )
{
  EFI_STATUS  Status;
  BOOLEAN     UnRegister;
  UINT32      RegData;
  UINT32      Idx;
  GPIO_PAD    CpuHpSmiPad;

  Status      = EFI_SUCCESS;
  UnRegister  = FALSE;

  DEBUG((EFI_D_LOAD|EFI_D_INFO, "\nRasInit - RasPeriodicTimerSmiHandler \n"));

  if(mFirstTimerEntry)  {

    DEBUG((EFI_D_LOAD|EFI_D_INFO, "\nRasInit - First Periodic Timer SMI Handler entry\n"));
    mFirstTimerEntry  = 00;

    //
    // First time call initialize with the start of the RasDispatcherTable.
    //
    FirstCpuRasEntry      = 1;
    mRasTableIndex        = 0;
    mExitImmediately      = 0;
    mNSE.NextEntryPointer = (NEXT_ENTRY_POINTER)RasDispatchTable[mRasTableIndex];
  }

  //
  // Call the entry in the dispatcher table
  //
  Status  = mNSE.NextEntryPointer();

  if (Status != IN_PROGRESS) {
    //RasDevHideBack();
    if (Status != NO_REQUEST) {
      mSmiParam[3] = (UINT32)Status;
    }

    //
    // Goto the next entry in the dispatcher anyway.
    //
    mRasTableIndex++;
    mNSE.NextEntryPointer  = (NEXT_ENTRY_POINTER)RasDispatchTable[mRasTableIndex];
  }

  //
  // Unregister the periodic SMI handler if it is last entry.
  //
  if(((UINTN)RasDispatchTable[mRasTableIndex] == (UINTN)-1) || (mExitImmediately)) {
      UnRegister  = TRUE;
  }

  if(mPeriodicTimerHandle)  {
    if (UnRegister == TRUE) {

      DEBUG((EFI_D_LOAD|EFI_D_INFO, "\nUnregistering the Periodic Timer.\n"));

      Status = mSmmPeriodicTimerDispatch->UnRegister(mSmmPeriodicTimerDispatch, mPeriodicTimerHandle);
      mPeriodicTimerHandle  = NULL;
      ASSERT_EFI_ERROR (Status);

      //
      // Restore the PM SMI enable registers.
      //
      Status  = gSmst->SmmIo.Io.Write (&gSmst->SmmIo, SMM_IO_UINT32, (mAcpiBaseAddr + R_ACPI_SMI_EN), 1, &PmRegSmiEnSave);

      //
      // Clear interrupts in IO Expander
      //
      mHpIOXAccess->ClearInterrupt(FALSE);

      Status = GpioGetCpuHpSmiPad (&CpuHpSmiPad);
      Status = GpioClearGpiSmiSts (CpuHpSmiPad);

      // Indicate to OS that we've completed the RAS operation.  If Status == 0, then no error.
      if (mSmiParam[3] == (UINT32)(EFI_ALREADY_STARTED & 0xFFFFFFFF)) {
        Status = ERROR_EXIT;
        mSmiParam[3] = (UINT32)Status;
      }

      mAcpiParameter->SmiRequestParam[3] = mSmiParam[3];

      if (mStatusAddress) {
        *(UINT32 *)mStatusAddress = mSmiParam[3];
      }

      // Clear out all the OS requests.
      for (Idx = 0 ;Idx < 3; Idx++) {
        mAcpiParameter->SmiRequestParam[Idx] = 0;
        mSmiParam[Idx] = 0;
      }

      // Clear the mHpRequest structure
      for (Idx=0; Idx < 6; Idx++) {
        mHpRequest[Idx] = 0;
      }

      //Set to FALSE so the next Valid SMI starts fresh, or it will prematurely exit on the next try.
      UnRegister = FALSE;
      mExitImmediately = FALSE;

      DEBUG ((EFI_D_INFO, "\n  RAS FLOW HAS COMPLETED!!\n  RAS FLOW HAS COMPLETED!!\n  RAS FLOW HAS COMPLETED!!\n\n"));

    } else {
      //
      // WA: SW periodic SMI timer will not work if the timer expires during the debug halt
      // Here try to reset the timer to trigger the next SMI
      //
      gSmst->SmmIo.Io.Read  (&gSmst->SmmIo, SMM_IO_UINT32, mAcpiBaseAddr + R_ACPI_SMI_EN, 1, &RegData);
      RegData = RegData & (~B_ACPI_IO_SMI_EN_SWSMI_TMR);
      gSmst->SmmIo.Io.Write (&gSmst->SmmIo, SMM_IO_UINT32, mAcpiBaseAddr + R_ACPI_SMI_EN, 1, &RegData);
      RegData = RegData | B_ACPI_IO_SMI_EN_SWSMI_TMR;
      gSmst->SmmIo.Io.Write (&gSmst->SmmIo, SMM_IO_UINT32, mAcpiBaseAddr + R_ACPI_SMI_EN, 1, &RegData);
    }
  }
  return EFI_SUCCESS;
}


EFI_STATUS
GetSourceOfGpi(
  VOID
)
{
  UINT8       Idx;
  EFI_STATUS  Status;

  DEBUG((EFI_D_LOAD|EFI_D_INFO, "\n Executing first RAS handler function - GetSourceOfGpi\n"));

  Idx = *((volatile UINT8 *)(UINTN)0x9FFFF);

  if (mIsSoftSmiRequest) {

    DEBUG((EFI_D_LOAD|EFI_D_INFO, "It is SoftSmiRequest\n"));
    if (mSmiParam[0] == SMI_CPU_ONLINE_OFFLINE || mSmiParam[0] == SMI_CPU_HOT_ADD_REMOVE) {
      //
      // Override the HpRequest with the user inputted value for socketID.
      //
      DEBUG((EFI_D_LOAD|EFI_D_INFO, "It is SoftSmiRequest for CPU Online/Offline\n"));
      if (mSmiParam[1] & BIT0)  { // Cpu hot removal
        mHpRequest[0] = 0x0F & (UINT8)(~(1 << mSmiParam[2]) & (UINT8)mCpuCsrAccessVarPtr->socketPresentBitMap);
        DEBUG((EFI_D_LOAD|EFI_D_INFO, "It is SoftSmiRequest for CPU Offline\n"));
      } else {                 // Cpu hot add
        mHpRequest[0] = 0x0F & (UINT8)((1 << mSmiParam[2])  | (UINT8)mCpuCsrAccessVarPtr->socketPresentBitMap);
        DEBUG((EFI_D_LOAD|EFI_D_INFO, "It is SoftSmiRequest for CPU Online\n"));
      }
    } else {
      mHpRequest[0] = 0;
      mHpRequest[1] = 0;
      mHpRequest[2] = 0;
      mHpRequest[3] = 0;
      mHpRequest[4] = 0;
    }
  } else {
    Status = mHpIOXAccess->ReadIoxAndUpdateHpRequest(mHpRequest,TRUE);
    if(Status == IN_PROGRESS) {
      return IN_PROGRESS;
    }
  }

  return STATUS_DONE;
}


EFI_STATUS
EFIAPI
RasInitSwSmiChildCallback (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID     *mGpiContext,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{

  UINT8 Data;

  DEBUG((EFI_D_LOAD|EFI_D_INFO, "\nDebugging CPU online in SIMICS\n"));

  Data = IoRead8 (R_PCH_IO_APM_STS);

  if (Data == 0xFF) {
    DEBUG((EFI_D_LOAD|EFI_D_INFO, "\n SwSMI parameter is %x\n", Data));

    // The following need to be set for the CPU RAS handler to initiate CPU1 online
    mAcpiParameter->SmiRequestParam[0] = SMI_CPU_ONLINE_OFFLINE;
    mAcpiParameter->SmiRequestParam[1] = 0;  // Online
    mAcpiParameter->SmiRequestParam[2] = 1;  // Skt 1
    mAcpiParameter->SmiRequestParam[3] = mSwSmiStatus;

    mFirstTimerEntry  = 01;
    mHpRequest[1] = 3;

    mSwSmiHandlerEnabled = TRUE;

    HpGPIHandler ();

  }

  return EFI_SUCCESS;
}

EFI_STATUS
RasSmiHandler(
  VOID
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  BOOLEAN     UnRegister = FALSE;
  UINT32      Idx;

  DEBUG((EFI_D_LOAD|EFI_D_INFO, "\nRasInit - RasSmiHandler \n"));

  //
  // First time call initialize with the start of the RasDispatcherTable.
  //
  FirstCpuRasEntry      = 1;
  mRasTableIndex        = 0;
  mExitImmediately      = 0;
  mNSE.NextEntryPointer = (NEXT_ENTRY_POINTER)RasDispatchTable[mRasTableIndex];

  while (1) {
    //
    // Call the entry in the dispatcher table
    //
    Status  = mNSE.NextEntryPointer();

    if (Status != IN_PROGRESS) {
      //RasDevHideBack();
      if (Status != NO_REQUEST) {
        mSmiParam[3] = (UINT32)Status;
      }

      //
      // Goto the next entry in the dispatcher anyway.
      //
      mRasTableIndex++;
      mNSE.NextEntryPointer  = (NEXT_ENTRY_POINTER)RasDispatchTable[mRasTableIndex];
    }

    //
    // Unregister the periodic SMI handler if it is last entry.
    //
    if(((UINTN)RasDispatchTable[mRasTableIndex] == (UINTN)-1) || (mExitImmediately)) {
       UnRegister  = TRUE;
    }

    if (UnRegister == TRUE) {

      //
      // Restore the PM SMI enable registers.
      //
      Status  = gSmst->SmmIo.Io.Write (&gSmst->SmmIo, SMM_IO_UINT32, (mAcpiBaseAddr + R_ACPI_SMI_EN), 1, &PmRegSmiEnSave);

      mHpIOXAccess->ClearInterrupt(FALSE);

      // Indicate to OS that we've completed the RAS operation.  If Status == 0, then no error.
      if (mSmiParam[3] == (UINT32)(EFI_ALREADY_STARTED & 0xFFFFFFFF)) {
        Status = ERROR_EXIT;
        mSmiParam[3] = (UINT32)Status;
      }

      mAcpiParameter->SmiRequestParam[3] = mSmiParam[3];

      if (mStatusAddress) {
        *(UINT32 *)mStatusAddress = mSmiParam[3];
      }

      // Clear out all the OS requests.
      for (Idx = 0 ;Idx < 3; Idx++) {
        mAcpiParameter->SmiRequestParam[Idx] = 0;
        mSmiParam[Idx] = 0;
      }

      // Clear the mHpRequest structure
      for (Idx=0; Idx < 6; Idx++) {
        mHpRequest[Idx] = 0;
      }

      //Set to FALSE so the next Valid SMI starts fresh, or it will prematurely exit on the next try.
      UnRegister = FALSE;
      mExitImmediately = FALSE;

      DEBUG ((EFI_D_INFO, "\n  RAS FLOW HAS COMPLETED!!\n  RAS FLOW HAS COMPLETED!!\n  RAS FLOW HAS COMPLETED!!\n\n"));
      break;

    }
  }
  return EFI_SUCCESS;
}
