/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2020 Intel Corporation. <BR>

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

#include "CpuHotAdd.h"
#include "ArchSpecificDef.h"
#include "FlashMap.h"
#include <Protocol/CpuCsrAccess.h>
#include <Library/CpuAndRevisionLib.h>
#include <Register/LocalApic.h>

CPU_HOT_ADD_DATA                           mCpuHotAddData;
ACPI_CPU_DATA                              *mAcpiCpuData;
UINT8                                      mSaveState[0x10000 - SMM_CPU_STATE_OFFSET];

volatile HOT_ADD_CPU_EXCHANGE_INFO         *mExchangeInfo;
volatile UINTN                             mRelocatedCpu;


EFI_CPU_PPM_PROTOCOL                        *mCpuPpmProtocol;
EFI_CPU_PM_STRUCT                           *mPpmLib, mLocalPpmStruct;
PPM_FROM_PPMINFO_HOB                        mPpmInfoHob;
EFI_CPU_CSR_ACCESS_PROTOCOL                 *mCpuCsrAccess;
EFI_MP_SERVICES_PROTOCOL                    *mMpService;
PPM_POLICY_CONFIGURATION                    *mPpmPolicy, mLocalPpmPolicy;
PPM_WRAP_STRUCT                             mPpmWrapPointer = {&mLocalPpmStruct, &mLocalPpmPolicy};

VOID
SmmInitHandler (
  VOID
  )
/*++

Routine Description:

  This is funtion used by all threads to relocate the SMM base

Arguments:

  None

Returns:
  None

--*/
{
  UINT32                            ApicId;
  UINTN                             Index;
  SOCKET_LGA_775_SMM_CPU_STATE      *CpuState;

  ApicId = GetApicId ();
  SmmInitializeSmrrEmrr ();

  CpuState = (SOCKET_LGA_775_SMM_CPU_STATE *)(UINTN)(SMM_DEFAULT_SMBASE + SMM_CPU_STATE_OFFSET);

  for (Index = 0; Index < FixedPcdGet32 (PcdCpuMaxLogicalProcessorNumber); Index++) {
      if (mExchangeInfo->CpuSmmInitData.ApicId[Index] == INVALID_APIC_ID) {
        CpuState->x86.SMBASE  = (UINT32)(mExchangeInfo->CpuSmmInitData.SmBase[Index]);
        mExchangeInfo->CpuSmmInitData.ApicId[Index] = ApicId;
        break;
      }
  }

  AsmAcquireMPLock ((UINT8 *)&mExchangeInfo->Lock);
  mRelocatedCpu++;
  AsmReleaseMPLock ((UINT8 *)&mExchangeInfo->Lock);

  AsmRsm ();
}

VOID
MPRendezvousProcedure (
  VOID
  )
/*++

Routine Description:

  This is the routine that all threads other than PBSP will be running after the hot add event

Arguments:

  None

Returns:
  None

--*/
{
  InitialProcessorInit();

  // Check-in counter for threads
  AsmAcquireMPLock ((UINT8 *)&mExchangeInfo->Lock);
  mExchangeInfo->NumCpuCheckedIn++;
  AsmReleaseMPLock ((UINT8 *)&mExchangeInfo->Lock);

  PpmMsrProgramming (&mLocalPpmStruct,&mLocalPpmPolicy);

  return;
}

VOID
PackageBspCpuEntry (
  VOID
  )
{

  UINT32                      Index = 0;
  UINT32                      CpuIndex;
  UINT32                      ApicId;
  UINT8                       SktId;
  UINT32                      SocketLevelBitShift;


  ApicId = GetApicId ();
  // use SBSP data as we don't support different SKU CPU O*L
  AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, 1, &SocketLevelBitShift, NULL, NULL, NULL);
  SocketLevelBitShift &= 0x1F;
  SktId = (UINT8)(ApicId >> SocketLevelBitShift);

  mExchangeInfo = (HOT_ADD_CPU_EXCHANGE_INFO *)mCpuHotAddData.MpExchangeInfoPtr;

  InitialProcessorInit();

  // Power Management CSR Progoramming.
  PpmCsrProgramming(SktId);

  // Set up the exchange data structure for APs' waking up
  mExchangeInfo->ApFunction      = (VOID *)(UINTN)MPRendezvousProcedure;
  mExchangeInfo->Lock            = 0;
  mExchangeInfo->InitFlag        = 3;
  mExchangeInfo->SignalFlag      = 0;

  // Send INIT IPI/SIPI to all APs, blocking them at entry

  //
  // In Simics,  the SIPI is broadcasted to SBSP as well
  // So, sending the targeted SIPI to S1C0T0
  //

  SendInitSipiSipiAllExcludingSelf ((UINT32)mCpuHotAddData.StartupVector);


  // Wait for all APs to response to the IPI
  MicroSecondDelay  (2000);

#ifdef LT_FLAG
  LTSXHotPlug();
#endif

  // Assign index for all CPUs for stack allocation
  CpuIndex = 0;
  for (Index = 0; Index < FixedPcdGet32 (PcdCpuMaxLogicalProcessorNumber); Index++) {
    if (mExchangeInfo->BistBuffer[Index].Number != 0) {
      mExchangeInfo->BistBuffer[Index].Number = CpuIndex + 1;
      CpuIndex++;
    }
  }

  // Unblock APs, everybody shoud go to MPRendezvousProcedure with stack set properly
  mExchangeInfo->NumCpuCheckedIn = 1;
  mExchangeInfo->SignalFlag = 1;
  while (mExchangeInfo->NumCpuCheckedIn != CpuIndex) {
    CpuPause ();
  }


  PpmMsrProgramming (&mLocalPpmStruct, &mLocalPpmPolicy);

  SetThreadLockBit (&mPpmWrapPointer);

  SetPackageLockBit (SktId, &mLocalPpmStruct, &mLocalPpmPolicy);
  SetBiosInitDone (SktId, &mPpmWrapPointer);
  SetConfigControlLockBit (&mLocalPpmPolicy);
  SetLockBitAfterBiosInitDone (SktId, &mPpmWrapPointer);

  // Notify SBSP the completion of MPINIT
  mExchangeInfo->SignalFlag = (UINT32)(-1);

  return;

}

VOID
EFIAPI
PackageSmmRelocate (
  VOID
  )
/*++

Routine Description:

  This funtion will relocate the SMM base for all threads including PBSP
  by sending SMI IPI to everybody. The SMI IPI has to be sent in a serilaized
  manner to avoid the SMM saving region overlap with each other at the default
  base

Arguments:

  None

Returns:
  None

--*/
{
  UINTN                       LastNumber;
  UINTN                       Index;
  UINTN                       CpuIndex;
  UINT32                      ApicId;
  UINTN                       OldData;
  UINT8                      *U8Ptr;
  VOID                       *OldApFunction;

  OldApFunction  = mExchangeInfo->ApFunction;

  // Backup original contents @ 0x38000
  U8Ptr = (UINT8*)(UINTN)(SMM_DEFAULT_SMBASE + SMM_HANDLER_OFFSET);

  // Set SMM jmp to AP waking vector
  OldData                = *(UINT64 *)U8Ptr;
  *U8Ptr                 = 0xEA;   //jmp
  *(UINT32 *)(U8Ptr + 1) = (UINT32)( (mCpuHotAddData.StartupVector >> 4) << 16 );
  CopyMem (mSaveState, (UINT8*)(UINTN)(SMM_DEFAULT_SMBASE + SMM_CPU_STATE_OFFSET), sizeof(mSaveState));

  // Relocate SMM base for all threads including itself
  mExchangeInfo->InitFlag        = 0;
  mExchangeInfo->ApFunction      = (VOID *)(UINTN)SmmInitHandler;

  CpuIndex      = 0;
  mRelocatedCpu = 0;
  LastNumber    = mRelocatedCpu;

  // Send the SMI IPI one by one for all detected threads
  for (Index = 0; Index < FixedPcdGet32 (PcdCpuMaxLogicalProcessorNumber); Index++) {
    if (mExchangeInfo->BistBuffer[Index].Number != 0) {

      ApicId = (UINT8)Index;
      SendSmiIpi ((UINT32)ApicId);

      // Wait for its completion
      while (mRelocatedCpu == LastNumber) {
        CpuPause ();
      }

      // Delay a little bit so that the processor can exit SMM
      MicroSecondDelay  (100);
      LastNumber = mRelocatedCpu;
      CpuIndex++;
    }
  }

  // Restore the 0x38000 contents
  *(UINT64 *)U8Ptr = OldData;
  CopyMem ((UINT8*)(UINTN)(SMM_DEFAULT_SMBASE + SMM_CPU_STATE_OFFSET), mSaveState, sizeof(mSaveState));
  mExchangeInfo->ApFunction  = OldApFunction;

  mExchangeInfo->ApFunction = (VOID *)((EFI_PHYSICAL_ADDRESS)PackageBspCpuEntry);

  return;
}

VOID
InitializeSmmCpuPpmStruct()
{

  CopyMem(&(mLocalPpmStruct.Version), &(mPpmLib->Version), sizeof(PPM_VERSION));

  mLocalPpmStruct.Info = &(mPpmInfoHob);
  CopyMem(mLocalPpmStruct.Info, mPpmLib->Info, sizeof(PPM_FROM_PPMINFO_HOB));

  CopyMem (&mLocalPpmPolicy, mPpmPolicy, sizeof (PPM_POLICY_CONFIGURATION));

}

EFI_STATUS
EFIAPI
CpuHotAddEntry (
    IN EFI_HANDLE           ImageHandle,
    IN EFI_SYSTEM_TABLE     *SystemTable
    )
/*++

Routine Description:

  Entry point for Cpu Hot Add driver.

Arguments:

  ImageHandle  -  EFI_HANDLE
  SystemTable  -  EFI_SYSTEM_TABLE

Returns:
  EFI_SUCCESS      - Success.
  EFI_UNLOAD_IMAGE - Unload the image for the first loading
  others           - Error occurred.

--*/
{
  EFI_HANDLE                  Handle = NULL;
  HOT_ADD_CPU_EXCHANGE_INFO  *ExchangeInfo;
  VOID                       *SmmStartupDataArea;
  MP_ASSEMBLY_ADDRESS_MAP    AddressMap;
  EFI_PHYSICAL_ADDRESS       WorkingBuffer, WakeUpBufferAddress;
  CPU_INIT_INFO              *CpuInitInfo;
  EFI_STATUS                 Status = EFI_SUCCESS;

  DEBUG((EFI_D_LOAD|EFI_D_INFO, "\nCPU Hot add Driver\n"));

  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, 0x1000, &SmmStartupDataArea);
  ASSERT_EFI_ERROR (Status);
  DEBUG((EFI_D_LOAD|EFI_D_INFO, "\nThe address of SmmStartupData area is %x\n",SmmStartupDataArea));

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &mMpService);
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmLocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gPpmPolicyProtocolGuid, NULL, &mPpmPolicy);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateProtocol (&gEfiCpuPpmProtocolGuid, NULL, &mCpuPpmProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  mPpmLib = mCpuPpmProtocol->CpuPpmStruct;

  // Copy the data from mCpuPpmProtocl to the local structure.
  InitializeSmmCpuPpmStruct();

  mAcpiCpuData = (ACPI_CPU_DATA *)(UINTN)PcdGet64 (PcdCpuS3DataAddress);

  // Allocate and Initialize the MP Exchange Info data in ACPI NVS Region.
  Status = gBS->AllocatePool (
             EfiACPIMemoryNVS,
             sizeof (HOT_ADD_CPU_EXCHANGE_INFO),
             &ExchangeInfo
             );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
  }


  ExchangeInfo->StackStart  = (VOID *) (UINTN) mAcpiCpuData->StackAddress;
  ExchangeInfo->StackSize   = PcdGet32 (PcdCpuApStackSize);
  ExchangeInfo->ApFunction  = (VOID *)((EFI_PHYSICAL_ADDRESS)PackageBspCpuEntry);

  // Need to program the following legacy MSRS on the incoming skt. The value is to be same as that of SBSP
  // So, saving the MSR offset and values of SBSP here, and will be programmed when the socket comes online
  ExchangeInfo->SbspMsrInfo[0].MsrOffset = MSR_CLOCK_CST_CONFIG_CONTROL;
  ExchangeInfo->SbspMsrInfo[0].MsrValue = AsmReadMsr64 (MSR_CLOCK_CST_CONFIG_CONTROL);;

  ExchangeInfo->SbspMsrInfo[1].MsrOffset = MSR_PMG_IO_CAPTURE_BASE;
  ExchangeInfo->SbspMsrInfo[1].MsrValue = AsmReadMsr64 (MSR_PMG_IO_CAPTURE_BASE);

  ExchangeInfo->SbspMsrInfo[2].MsrOffset = ICX_MSR_POWER_CTL1;
  ExchangeInfo->SbspMsrInfo[2].MsrValue = AsmReadMsr64 (ICX_MSR_POWER_CTL1);

  ExchangeInfo->SbspMsrInfo[3].MsrOffset = MSR_IA32_MISC_ENABLE;
  ExchangeInfo->SbspMsrInfo[3].MsrValue = AsmReadMsr64 (MSR_IA32_MISC_ENABLE);

  //
  // Allocate 4K aligned bytes below 1M memory.
  //
  WakeUpBufferAddress = BASE_1MB;
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiACPIMemoryNVS,
                  EFI_SIZE_TO_PAGES (0x1000),
                  &WakeUpBufferAddress
                  );
  ASSERT_EFI_ERROR (Status);
  //
  // Get the address map of startup code for AP,
  // including code size, and offset of long jump instructions to redirect.
  //
  ZeroMem (&AddressMap, sizeof (AddressMap));
  AsmGetHotAddCodeAddressMap (&AddressMap);

  WorkingBuffer = (EFI_PHYSICAL_ADDRESS) AllocateZeroPool (0x1000);

  //
  // Copy AP startup code to a working buffer first and then patch the long jump
  // offsets for mode switching.
  //
  CopyMem ((VOID *)WorkingBuffer, AddressMap.RendezvousFunnelAddress, AddressMap.Size);
  *(UINT32 *) (UINTN) (WorkingBuffer + AddressMap.FlatJumpOffset + 3) = (UINT32) (WakeUpBufferAddress + AddressMap.PModeEntryOffset);
  if (AddressMap.LongJumpOffset != 0) {
    *(UINT32 *) (UINTN) (WorkingBuffer + AddressMap.LongJumpOffset + 1) = (UINT32) (WakeUpBufferAddress + AddressMap.LModeEntryOffset);
  }


  // Initialize the CPU Init data and place it right after the startup vector code.
  CpuInitInfo = (CPU_INIT_INFO *) (UINTN) (WorkingBuffer + AddressMap.Size);
  ZeroMem ((VOID *) CpuInitInfo, sizeof (CPU_INIT_INFO));
  CpuInitInfo->BufferStart = (UINT32)WakeUpBufferAddress;
  CpuInitInfo->Cr3         =   (UINTN)Gen4GPageTable ();;  /// 0x 7fcd0000 -> needs to be updated with new page table created outside smm
  CopyMem ((VOID *) (UINTN) &CpuInitInfo->GdtrProfile, (VOID *) (UINTN) mAcpiCpuData->GdtrProfile, sizeof (IA32_DESCRIPTOR));
  CopyMem ((VOID *) (UINTN) &CpuInitInfo->IdtrProfile, (VOID *) (UINTN) mAcpiCpuData->IdtrProfile, sizeof (IA32_DESCRIPTOR));
  CpuInitInfo->MpExchangeInfoPtr = ExchangeInfo;

  //
  // copy working buffer to  AP wakeup buffer and AP Smm startup data area
  //
  CopyMem((VOID *)WakeUpBufferAddress, (VOID *)WorkingBuffer, 0x1000);
  CopyMem(SmmStartupDataArea, (VOID *) (UINTN)WorkingBuffer, 0x1000);

  FreePool((VOID *)WorkingBuffer);

  mCpuHotAddData.StartupVector = (EFI_PHYSICAL_ADDRESS)WakeUpBufferAddress;
  mCpuHotAddData.MpExchangeInfoPtr = ExchangeInfo;
  mCpuHotAddData.PackageSmmRelocate = PackageSmmRelocate;
  mCpuHotAddData.CpuPmStruct = &mLocalPpmStruct;
  mCpuHotAddData.CpuPmPolicy = &mLocalPpmPolicy;
  mCpuHotAddData.SmmStartupCodeArea = (EFI_PHYSICAL_ADDRESS)SmmStartupDataArea;


  // Finally indicate that the Startup Vector code and data has been initialized.
  mCpuHotAddData.MpExchangeInfoPtr->InitFlag    = 2;

  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEfiCpuHotAddDataProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mCpuHotAddData
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}


EFI_STATUS
AllocateMemoryBelow4G (
  IN   EFI_MEMORY_TYPE MemoryType,
  IN   UINTN           Size,
  OUT  VOID           **Buffer
  )
/*++

Routine Description:

  Allocate MemoryType below 4G memory address.

Arguments:

  Size   - Size of memory to allocate.
  Buffer - Allocated address for output.

Returns:

  EFI_SUCCESS - Memory successfully allocated.
  Other       - Other errors occur.

--*/
{
  UINTN                 Pages;
  EFI_PHYSICAL_ADDRESS  Address;
  EFI_STATUS            Status;

  Pages = EFI_SIZE_TO_PAGES (Size);
  Address = 0xffffffff;

  Status  = (gBS->AllocatePages) (
                   AllocateMaxAddress,
                   MemoryType,
                   Pages,
                   &Address
                   );

  *Buffer = (VOID *) (UINTN) Address;

  return Status;
};


UINT32
Gen4GPageTable (
  VOID
  )
{
  EFI_STATUS                        Status;
  UINTN                             i, j;
  UINT64                            *PDE, *PTE;
  UINT64                            *PML4;
  EFI_PHYSICAL_ADDRESS              PageTable;

  Status = AllocateMemoryBelow4G (EfiReservedMemoryType,  6 * EFI_PAGE_SIZE, (VOID **)&PageTable);

  ASSERT_EFI_ERROR (Status);

  //
  // Zero out all page table entries first
  //
  ZeroMem ((VOID*)(UINTN)PageTable, EFI_PAGES_TO_SIZE (6));

  //
  // This step is needed only in 64-bit mode
  //
  PML4 = (UINT64*)(UINTN)PageTable;
  PageTable += EFI_PAGE_SIZE;
  *PML4 = PageTable | IA32_PG_P | IA32_PG_RW;


  PDE = (UINT64*)(UINTN)PageTable;
  PTE = PDE + EFI_PAGE_SIZE / sizeof (*PDE);

  for (i = 0; i < 4; i++) {
    *PDE = (UINTN)PTE | IA32_PG_P |  IA32_PG_RW;
    PDE++;

    for (j = 0; j < EFI_PAGE_SIZE / sizeof (*PTE); j++) {
      *PTE = (((i << 9) + j) << 21) |
        IA32_PG_PS | IA32_PG_RW | IA32_PG_P;
      PTE++;
    }
  }

  //
  // This step is needed only in 64-bit mode
  //
  PageTable = (EFI_PHYSICAL_ADDRESS)(UINTN)PML4;

  return (UINT32)PageTable;
}

#ifdef LT_FLAG
VOID
LTSXHotPlug (
  VOID
  )
/*++

Routine Description:

 Do LTSX CPU Hot-Plug setting here, include

 1. set LT.SPAD bit 61 for microcode SIPI function in OS
 2. LockConfig


Arguments:

  None

Returns:
  None

--*/
{

  EFI_CPUID_REGISTER  Register;
  UINT32              Data32;
  UINT64              Bus_No_Msr;
  UINT32              Stack;
  UINT32              IioDfxLockAddr;
  BOOLEAN             IsLTSXEnabled;
  MSR_IA32_FEATURE_CONTROL_REGISTER    FeatureControl;

  AsmCpuid (CPUID_VERSION_INFO, &Register.RegEax, &Register.RegEbx, &Register.RegEcx, &Register.RegEdx);

  Data32 = LT_EXIST_VALUE;              // Use LT_EXIST as LTSX strapping enable or not
  IioDfxLockAddr = IIO_DFX_LCK_GLOBAL;  // IIO_DFX_LCK GLOBAL at Bx:D7:F7:0x504

  if(  (Data32) ) {   // check if is IVT-EX and lt-sx enabled
      IsLTSXEnabled = TRUE;
  }else {
      IsLTSXEnabled = FALSE;
  }

  Data32  = LT_SPAD_HIGH_VALUE;         // Check if ACm report any error so far
  Data32 &= (BIT31+BIT30);
  FeatureControl.Uint32 = (UINT32) AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL);  // Check if SMXE enabled

  // Suppose IA32_FEATURE shoud be copied from BSP at InitializeFeatures  ()
  // sometimes it failed, patch here first
  if((Data32 == BIT31) && (FeatureControl.Uint32 ==0) ) {
     AsmWriteMsr64 (MSR_IA32_FEATURE_CONTROL, 0xFF07);
     FeatureControl.Uint32 = 0xFF07;
  }

  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    Bus_No_Msr =  AsmReadMsr64 (SKX_MSR_CPU_BUSNUMBER);                  // get IIO bus number
  } else {
    Bus_No_Msr =  AsmReadMsr64 (MSR_CPU_BUSNUMBER);             // get IIO bus number
  }
  if(Bus_No_Msr & BIT63) {                      // check if bus data is valid
    //do IIO DFX lock
    for(Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      IioDfxLockAddr &= 0xF00FFFFF;             // clear Bus field
      IioDfxLockAddr |= (UINT32)(((UINT32)(RShiftU64(Bus_No_Msr, (Stack * 8))) & 0xFF) << 20);
      *(volatile UINT32 *) (UINTN) (IioDfxLockAddr) = 0x000003FF; // set IIO_DFX_Lock
    }
  } else {                                      //we have invalid Bus# data
    //SKX TODO: caller has no idea how to handle the error
    //Temp WA to softhang for dedbug
    CpuDeadLoop();
  }

  // Check if:
  // 1. LT-SX strapping ON
  // 2. ACM not report error so far
  // 3. SMXE in IA32_FEATURE_CONTROL enabled
  if((IsLTSXEnabled == TRUE) &&
     (Data32 == BIT31) &&
     (FeatureControl.Bits.SenterGlobalEnable == 1) &&
     (FeatureControl.Bits.EnableVmxInsideSmx == 1) ) {

        //SmiEnSave = __inbyte (0x430);
        //Data8 = SmiEnSave & 0xFE;   //~B_GBL_SMI_EN; Ddisable SMI Here ?
        //__outbyte  (0x430, Data8);
        //AcpiStall (4000);

        SetApicBSPBit (TRUE);

        ProgramVirtualWireMode_RAS (TRUE, 0x0);

        SendInterrupt (
            BROADCAST_MODE_ALL_EXCLUDING_SELF,
            0,
            0,
            LOCAL_APIC_DELIVERY_MODE_INIT,
            TRIGGER_MODE_EDGE,
            TRUE
        );

        // Wait for all APs to response to the IPI
        MicroSecondDelay  (10000);

        LaunchBiosAcm ((UINTN)FLASH_REGION_BIOS_ACM_BASE, (UINTN)LT_LOCK_CONFIG);

        //
        // Start AP's, or SMI will not be delivered.
        //
        SendInterrupt (
            BROADCAST_MODE_ALL_EXCLUDING_SELF,
            0,
            (UINT32) RShiftU64 (mCpuHotAddData.StartupVector, 12),
            LOCAL_APIC_DELIVERY_MODE_STARTUP,
            TRIGGER_MODE_EDGE,
            TRUE
        );

        SendInterrupt (
            BROADCAST_MODE_ALL_EXCLUDING_SELF,
            0,
            (UINT32) RShiftU64 (mCpuHotAddData.StartupVector, 12),
            LOCAL_APIC_DELIVERY_MODE_STARTUP,
            TRIGGER_MODE_EDGE,
            TRUE
        );

        //
        // Wait for all APs to response to the IPI
        //
        MicroSecondDelay  (10000);

        SetApicBSPBit (FALSE);

        Data32 = LT_SPAD_TXT_CPU_HOT_PLUG;     //
        LT_SPAD_SET_HIGH_VALUE = Data32;       // set CPU hot-plug bit LT.SPAD[61]

  }// if((IsLTSXEnabled == TRUE)...

}
#endif //LT_FLAG
