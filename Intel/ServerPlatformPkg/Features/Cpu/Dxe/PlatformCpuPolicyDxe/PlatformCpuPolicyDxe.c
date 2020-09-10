/** @file
  CPU Policy Platform DXE Driver implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation. <BR>

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

#include <PiDxe.h>
#include <Protocol/CpuPolicyProtocol.h>
#include <Protocol/PpmPolicyProtocol.h>
#include <IioUniversalData.h>
#include <Protocol/PlatformType.h>
#include <Guid/SetupVariable.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/CpuConfigLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/CpuPpmLib.h>
#include <Library/SetupLib.h>
#include <Library/MemMapDataLib.h>
#include <Platform.h>
#include <Protocol/CpuCsrAccess.h>
#include <RcRegs.h>
#include <PlatformInfoTypes.h>
#include <BiosGuardDefinitions.h>
#include <Cpu/CpuCoreRegs.h>
#include <Cpu/CpuPpmB2p.h>
#include <Library/EmulationConfigurationLib.h>
#include <Register/Cpuid.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/KtiApi.h>
#include <Library/PpmPolicyUpdateCommonLib.h>
#include <Library/CpuPolicyUpdateCommonLib.h>
#include <UncoreCommonIncludes.h>

extern EFI_GUID gBiosGuardHobGuid;

VOID
EFIAPI
InitializeBiosGuardToolsIntCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  );

CHAR16 mCpuSocketStr[8][5] = {L"CPU0", L"CPU1", L"CPU2", L"CPU3", L"CPU4", L"CPU5", L"CPU6", L"CPU7"};
CHAR16 mCpuAssetTagStr[] = L"UNKNOWN";
EFI_CPU_CSR_ACCESS_PROTOCOL *mCpuCsrAccess;
IIO_UDS                     *mIioUds;
CPU_POLICY_CONFIGURATION    mCpuPolicyConfiguration;
PPM_POLICY_CONFIGURATION    mPpmPolicyConfiguration;

/**
  Set platform CPU data that related to SMBIOS.
**/
VOID
PlatformCpuSmbiosData (
  VOID
  )
{
  UINT32                        CpuSocketCount;
  UINTN                         Index;
  CHAR16                        **CpuSocketNames;
  CHAR16                        **CpuAssetTags;
  EFI_STATUS                    Status = EFI_SUCCESS;

  //
  // Set the count of CPU sockets on the board.
  //
  CpuSocketCount = PcdGet32(PcdOemSkuBoardSocketCount);

  Status = PcdSet32S (PcdPlatformCpuSocketCount, CpuSocketCount);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return;
  }

  CpuSocketNames = AllocatePool (CpuSocketCount * sizeof (UINTN));

  if (CpuSocketNames == NULL) {
    DEBUG ((EFI_D_ERROR, "\nEFI_OUT_OF_RESOURCES!!! AllocatePool() returned NULL pointer.\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return;
  }

  CpuAssetTags = AllocatePool (CpuSocketCount * sizeof (UINTN));
  if (CpuAssetTags == NULL) {
    DEBUG ((EFI_D_ERROR, "\nEFI_OUT_OF_RESOURCES!!! AllocatePool() returned NULL pointer.\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    gBS->FreePool (CpuSocketNames);
    return;
  }

  for (Index = 0; Index < CpuSocketCount; Index++) {
    CpuSocketNames[Index] = mCpuSocketStr[Index];
    CpuAssetTags[Index] = mCpuAssetTagStr;
  }

  mCpuPolicyConfiguration.PlatformCpuSocketNames = (UINT64) (UINTN) CpuSocketNames;

  mCpuPolicyConfiguration.PlatformCpuAssetTags = (UINT64) (UINTN) CpuAssetTags;
}

/**
  Check and allocate memory for IED trace, aslo report
  base and size (in 1M boundary) to scrtachpad register
  BIOSNONSTICKYSCRATCHPAD3 (Base address) and
  BIOSNONSTICKYSCRATCHPAD3 (size)
**/
VOID
CheckAndPopulateIedTraceMemory (
  UINTN       IedTraceSize
  )
{
  EFI_STATUS          Status;
  UINTN               NumPages;
  UINTN               Addr;
  UINTN               AllocateSize;
  UINT8               i;
  UINT32              RegData;

  AllocateSize = IedTraceSize;

  //valid Ied trace size 4M to 1GB
  ASSERT ((IedTraceSize >= 0x400000) && (IedTraceSize <= 0x40000000));

  while (1) {
    NumPages = EFI_SIZE_TO_PAGES (AllocateSize + 0x200000); //IED need to start at even 1M boundary
    Status = gBS->AllocatePages (
                          AllocateAnyPages,
                          EfiReservedMemoryType,
                          NumPages,
                          &Addr
                          );
    if (Status == EFI_SUCCESS) {
      //make IedTrace start at even 1M boundary
      if (Addr & 0x100000) { //Addr starts somewhere inside odd Meg address
        Addr = (Addr & 0xFFF00000) + 0x100000; //make it at even 1M boundary
      } else { //Addr starts somehwere in an even Meg
        Addr = (Addr & 0xFFF00000) + 0x200000;
      }

      gBS->SetMem((UINT8 *)Addr, AllocateSize, 0);
      //set "INTEL RSVD" signature on begging address of IedTrace memory
      gBS->CopyMem((UINT8 *)Addr, "INTEL RSVD", 10);
      break;
    } else {
      AllocateSize /= 2;
      if (AllocateSize < 0x400000) { //min 4M
        DEBUG ((EFI_D_ERROR, "Error(%x): Failed to allocate IedTrace memory\n", Status));
        Addr = 0;
        AllocateSize = 0;
        break;
      }
    }

  }

  if (AllocateSize != IedTraceSize) {
    DEBUG ((EFI_D_WARN, "Warning: Request Iedtrace Size: 0x%x, only allocate 0x%x != \n", IedTraceSize, AllocateSize));
  } else {
    DEBUG ((EFI_D_INFO, "Allocate 0x%x byte IedTrace Memory @0x%x\n", AllocateSize, Addr));
  }

  //now store info into scratchpad
  for (i = 0; i < MAX_SOCKET; i++) {
    if (mIioUds->PlatformData.CpuQpiInfo[i].Valid) {
      // populate IEDTrace Base (MB bounded) to NonSticky Scratchpad3
      RegData = (UINT32)(UINTN)(Addr >> 20);
      mCpuCsrAccess->WriteCpuCsr(i, 0, BIOSNONSTICKYSCRATCHPAD3_UBOX_MISC_REG, RegData);
      DEBUG ((EFI_D_INFO, "Scratchpad_Debug CheckAndPopulateIedTraceMemory: Verify Non Sticky Scratchpad3 %d\n", RegData));

      // populate IEDTrace Size in MB to NonSticky Scratchpad11
      RegData = (UINT32)(UINTN)(AllocateSize >> 20);
      mCpuCsrAccess->WriteCpuCsr(i, 0, BIOSNONSTICKYSCRATCHPAD11_UBOX_MISC_REG, RegData);
      DEBUG ((EFI_D_INFO, "Scratchpad_Debug CheckAndPopulateIedTraceMemory: Write Non Sticky Scratchpad11 %d\n", RegData));
    }
  }
}

/**

    Re-assign socket id when both X2APIC and ForceX2Apic are enabled.

    @param None

    @retval None

**/
VOID
CheckAndReAssignSocketId(
  VOID
  )
{
  CPU_SOCKET_ID_INFO    *pcdSktIdPtr;
  UINT32                i, IntraPackageIdBits;
  UINTN                 PcdSize;
  EFI_STATUS            Status;
  UINT32                MaxSocketCount;

  MaxSocketCount = FixedPcdGet32(PcdMaxCpuSocketCount);
  DEBUG ((DEBUG_INFO, "::SocketCount %08x\n", MaxSocketCount));
  pcdSktIdPtr = (CPU_SOCKET_ID_INFO *)PcdGetPtr(PcdCpuSocketId);
  PcdSize = LibPcdGetSize (PcdToken (PcdCpuSocketId)); //MAX_SOCKET * sizeof(CPU_SOCKET_ID_INFO);
  ASSERT(PcdSize == (MAX_SOCKET * sizeof(CPU_SOCKET_ID_INFO)));
  Status = PcdSetPtrS (PcdCpuSocketId, &PcdSize, (VOID *)pcdSktIdPtr);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return;
  }
  DEBUG ((EFI_D_INFO, "::SockeId Pcd at %08x, size %x\n", PcdGetPtr(PcdCpuSocketId), PcdSize));

  for(i = 0; i < MAX_SOCKET; i++) {
    if (mIioUds->PlatformData.CpuQpiInfo[i].Valid) {
      pcdSktIdPtr[i].DefaultSocketId = mIioUds->PlatformData.CpuQpiInfo[i].SocId;
      pcdSktIdPtr[i].NewSocketId     = mIioUds->PlatformData.CpuQpiInfo[i].SocId;
    } else {
      pcdSktIdPtr[i].DefaultSocketId = (UINT32)-1;   //make sure Default and New are same
      pcdSktIdPtr[i].NewSocketId     = (UINT32)-1;
    }
  }

  AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, 1, &IntraPackageIdBits, NULL, NULL, NULL);
  //assign new socketId
  for(i = 0; i < MAX_SOCKET; i++) {

    if (pcdSktIdPtr[i].DefaultSocketId == (UINT32)-1) {
      continue;
    }

    switch(IntraPackageIdBits) {
      case 4: //socket bit starts from bit4 of ApicId
      case 5: //socket bit starts from bit5 of ApicId
        if (MAX_SOCKET == 4) {
          pcdSktIdPtr[i].NewSocketId |= (APICID_MASK_BIT14_8 << (8 - IntraPackageIdBits));
        } else {
          //3bit in lower 8bit as skt field, to avoid ApicID= FFs, leave bit8 untouched for 8S
          pcdSktIdPtr[i].NewSocketId |= (0x7E << (8 - IntraPackageIdBits));      //leave bit8 to 0 so we don't have FFs in ApicId
        }
        break;

     case 6: //socket bit starts from bit6 of ApicId
        if (MAX_SOCKET == 4) {
          //only 2bit in lower 8bit as skt field, to avoid ApicID= FFs, leave bit8 untouched for 4S
          pcdSktIdPtr[i].NewSocketId |= (0x7E << (8 - IntraPackageIdBits));
        } else {
          //only 2bit in lower 8bit as skt field, to avoid ApicID= FFs, leave bit9 untouched for 8S
          pcdSktIdPtr[i].NewSocketId |= (0x7C << (8 - IntraPackageIdBits));
        }
        break;

     default:
        DEBUG ((EFI_D_INFO, "::Need more info to make sure we can support!!!\n"));
        break;

    } //end switch
  }
}

/**
  Platform CPU policy DXE driver entry point.

  @param ImageHandle     - Handle for the image of this driver.
  @param SystemTable     - Pointer to the EFI System Table.

  @retval EFI_SUCCESS     - Protocol installed sucessfully.

**/
EFI_STATUS
EFIAPI
PlatformCpuPolicyDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                       Status;
  // APTIOV_SERVER_OVERRIDE_RC_START: To solve structure redefinition error in AMI.
  INTEL_SETUP_DATA                SetupData;
// APTIOV_SERVER_OVERRIDE_RC_END: To solve structure redefinition error in AMI.
  EFI_HANDLE                       Handle;
  EFI_EVENT                        EndOfDxeEvent;
  EFI_GUID                         UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
  EFI_HOB_GUID_TYPE                *GuidHob;
  SYSTEM_MEMORY_MAP_HOB            *SystemMemoryMap;
  BOOLEAN                          Is14nmCpu;

  GuidHob   = GetFirstGuidHob (&UniversalDataGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  mIioUds   = GET_GUID_HOB_DATA(GuidHob);

  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    return EFI_NOT_FOUND;
  }

  SetMem (&mCpuPolicyConfiguration, sizeof (CPU_POLICY_CONFIGURATION), 0x00);
  SetMem (&mPpmPolicyConfiguration, sizeof (PPM_POLICY_CONFIGURATION), 0x00);

  Is14nmCpu = IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL);

  PlatformCpuSmbiosData ();

  ///
  /// Create an EndOfDxe event callback event for BIOS Guard.
  ///
  Status = gBS->CreateEventEx (
                EVT_NOTIFY_SIGNAL,
                TPL_CALLBACK,
                InitializeBiosGuardToolsIntCallback,
                NULL,
                &gEfiEndOfDxeEventGroupGuid,
                &EndOfDxeEvent
                );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess);
  ASSERT_EFI_ERROR (Status);

  //
  // Read the current system configuration variable store.
  //
  // APTIOV_SERVER_OVERRIDE_RC_START: To solve structure redefinition error in AMI. 
  ZeroMem(&SetupData, sizeof(INTEL_SETUP_DATA));
  // APTIOV_SERVER_OVERRIDE_RC_END: To solve structure redefinition error in AMI.  
  Status = GetEntireConfig(&SetupData);

  if (EFI_ERROR(Status)) {
    // Should bring up attention if happened
    ASSERT(FALSE);
  } else {
    //
    //  CPU Policy starts
    //
    UpdateCpuPolicyFromSetup (&mCpuPolicyConfiguration.Policy);

    //
    // Verify that the value being set is within the valid range 0 to MAX_SOCKET - 1
    //
    if (SetupData.SocketConfig.SocketProcessorCoreConfiguration.BspSelection > MAX_SOCKET) {
      SetupData.SocketConfig.SocketProcessorCoreConfiguration.BspSelection= 0xFF;
    }
    mCpuPolicyConfiguration.SbspSelection = SetupData.SocketConfig.SocketProcessorCoreConfiguration.BspSelection;

    //
    // HSD 4926912 SKX: CPU Knobs add a eSMM setup option.
    //
    Status = PcdSetBoolS (PcdCpuSmmMsrSaveStateEnable, SetupData.SocketConfig.SocketProcessorCoreConfiguration.eSmmSaveState? TRUE : FALSE);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    if (SetupData.SystemConfig.EmcaMsmiEn != 0 && IsSiliconWorkaroundEnabled("S1408146425") == FALSE) {
      Status = PcdSetBoolS (PcdCpuSmmProtectedModeEnable, TRUE);
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR(Status)) {
        return Status;
      }
    } else {
      Status = PcdSetBoolS (PcdCpuSmmProtectedModeEnable, FALSE);
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR(Status)) {
        return Status;
      }
    }

#ifdef GCC_BIOS_BUILD
    //
    // GCC_TODO: Disabling Smm Proctected Mode due to a possibly assembly issue in
    //           SmiEntry.S Code (PiSmmCpuDxeSmm Driver)
    //
    Status = PcdSetBoolS (PcdCpuSmmProtectedModeEnable, FALSE);
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR(Status)) {
        return Status;
    }
#endif

    //
    // HSD 4928956  SKX: option for SV to turnoff the SMM protected mode Entry
    //


    if((!IsSimicsEnvironment ()) && (Is14nmCpu || IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_SPRSP, REV_ALL))) {
      Status = PcdSetBoolS (PcdCpuSmmRuntimeCtlHooks, TRUE);
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR(Status)) {
        return Status;
      }
    } else {
      Status = PcdSetBoolS (PcdCpuSmmRuntimeCtlHooks, FALSE);
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR(Status)) {
        return Status;
      }
    }
    DEBUG ((EFI_D_INFO, ":: PcdCpuSmmRuntimeCtlHooks= %x\n", PcdGetBool(PcdCpuSmmRuntimeCtlHooks)));

    // allocate memory for IedTrace
    if ((SetupData.SocketConfig.SocketProcessorCoreConfiguration.IedTraceSize != 0) && (PcdGet32 (PcdCpuIEDRamSize) != 0)) {
      CheckAndPopulateIedTraceMemory(0x400000 << (SetupData.SocketConfig.SocketProcessorCoreConfiguration.IedTraceSize - 1));
    }

    //
    //  PPM Policy starts
    //
    UpdatePpmPolicyFromSetup (&mPpmPolicyConfiguration);

  } //end - else

  //
  // Install CPU PPM Policy Protocol for platform PPM configuration
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gPpmPolicyProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mPpmPolicyConfiguration
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Install CPU Policy Protocol for platform configuration
  // Cpu Driver could be dispatched after this protocol installed.
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEfiCpuPolicyProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mCpuPolicyConfiguration
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**

   Drop into SMM to register IOTRAP for BIOS Guard tools interface

   @param Event     - A pointer to the Event that triggered the callback.
   @param Context   - A pointer to private data registered with the callback function.

   @retval None

**/
VOID
EFIAPI
InitializeBiosGuardToolsIntCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_PHYSICAL_ADDRESS BaseAddress;
  BIOSGUARD_HOB        *BiosGuardHobPtr;
  EFI_GUID             BiosGuardHobGuid = gBiosGuardHobGuid;

  if (HybridSystemLevelEmulationEnabled ()) {
   return;
  }

  BiosGuardHobPtr = GetFirstGuidHob (&BiosGuardHobGuid);
  if (BiosGuardHobPtr != NULL) {
    BaseAddress = (EFI_PHYSICAL_ADDRESS) BiosGuardHobPtr->BiosGuardToolsIntIoTrapAdd;
    ///
    /// IOTRAP TO SMM
    ///
    IoRead8 (BaseAddress);
  }
  return;
}
