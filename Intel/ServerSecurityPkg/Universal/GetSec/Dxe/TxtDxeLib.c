/** @file
  This file contains an implementation of the function call interfaces
  required by the main LT DXE file.  Hopefully, future platform porting
  tasks will be mostly limited to modifying the functions in this file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2019 Intel Corporation. <BR>

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

#include "TxtDxeLib.h"
#include <Library/LtDxeLib.h>
#include <../Common/LaunchBiosAcm.h>

#include <Library/PreSiliconEnvDetectLib.h> //for IsSimicsEnvironment()
#include <Register/ArchitecturalMsr.h>
#include <Cpu/CpuCoreRegs.h>

VOID
SendIpi (
  IN UINT32          IcrLow,
  IN UINT32          ApicId
  );

VOID
EFIAPI
SendInitIpiAllExcludingSelf (
  VOID
  );

VOID
EFIAPI
SendInitSipiSipiAllExcludingSelf (
  IN UINT32          StartupRoutine
  );

UINT8
EFIAPI
IoRead8 (
  IN      UINTN                     Port
  );

UINT8
EFIAPI
IoWrite8 (
  IN      UINTN                     Port,
  IN      UINT8                     Value
  );

//
// LT Configuration Space Register Definitions
//
#define LT_CONFIG_SPACE_BASE_ADDRESS        0xfed20000

#define LT_PUBLIC_SPACE_BASE_ADDRESS        0xfed30000
#define LT_ESTS_OFF                         0x8
#define   LT_TXT_RESET_STS                  0x1
#define LT_NODMA_BASE_REG_OFF               0x260
#define LT_NODMA_SIZE_REG_OFF               0x268
#define LT_SINIT_BASE_REG_OFF               0x270
#define LT_SINIT_SIZE_REG_OFF               0x278
#define LT_HEAP_BASE_REG_OFF                0x300
#define LT_HEAP_SIZE_REG_OFF                0x308
#define LT_DMA_PROTECTED_RANGE_REG_OFF      0x330

#define LT_TPM_SPACE_BASE_ADDRESS           0xfed40000
#define LT_TPM_SPACE_ACCESS0                0x0

#define B_GBL_SMI_EN  1

#define FEATURE_LOCK_BIT                0x0001
#define FEATURE_VMON_IN_SMX             0x0002
#define FEATURE_VMON_OUT_SMX            0x0004
#define FEATURE_SENTER_ENABLE           0xFF00

#ifndef DELIVERY_MODE_INIT
#define DELIVERY_MODE_INIT                    0x05
#endif
#ifndef DELIVERY_MODE_SIPI
#define DELIVERY_MODE_SIPI                    0x06
#endif

#define ONE_MEGABYTE                        0x100000

#define ACMOD_SIZE                      24

#ifndef CPUID_EXTENDED_TOPOLOGY
#define CPUID_EXTENDED_TOPOLOGY     0x0B
#endif

#define CMOS_LTSX_OFFSET              0x2A
// APTIOV_SERVER_OVERRIDE_RC_START : Use LTSX_TXT_CMOS_ADDRESS token value and comment the hardcoded value when IntelLtsxFit_SUPPORT is Enabled
#ifdef LTSX_TXT_CMOS_ADDRESS
#undef CMOS_LTSX_OFFSET
#define CMOS_LTSX_OFFSET             LTSX_TXT_CMOS_ADDRESS
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Use LTSX_TXT_CMOS_ADDRESS token value and comment the hardcoded value when IntelLtsxFit_SUPPORT is Enabled

#define SGX_GLOBAL_ENABLE             BIT18

//
// Ap init function
//
VOID
DoApInit (
  VOID
  );

VOID
ApSaveConfig (
  VOID
  );

VOID
ApRestoreConfig (
  VOID
  );

//
// SGX function
//
STATIC
BOOLEAN
IsSgxEnabled(
  VOID
  );

#define  IA32_MTRR_PHYSBASE0  0x200
#define  IA32_MTRR_PHYSMASK7  0x20F

UINTN   mAcmBase;
UINT64  mApIdt[2];
UINTN   mApCr4;
UINT64  mApMtrrTab[2*(IA32_MTRR_PHYSMASK7 - IA32_MTRR_PHYSBASE0 + 1) + 1];

//
// LtDxeLibLaunchBiosAcm is an internal routine used to
// wrap the BIOS ACM initialization code for the various
// functions inside of it.
//
STATIC
EFI_STATUS
LtDxeLibLaunchBiosAcm (
  IN LT_DXE_LIB_CONTEXT      *LtDxeCtx,
  IN UINT32                  AcmFunction
  );

STATIC
EFI_STATUS
GetApVector (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

// APTIOV_SERVER_OVERRIDE_RC_START : TXT Error and status Register message printing.
VOID PrintLtsxErrors(
)
{
  UINT32                      Data32;
	
  Data32 = *(volatile UINT32 *)(UINTN)TXT_ACM_STATUS_LO;
  DEBUG ((EFI_D_ERROR, "TXT_ACM_STATUS_LO (0xFED300A0): %X\n", Data32));
	
  Data32 = *(volatile UINT32 *)(UINTN)TXT_ACM_STATUS_HI;
  DEBUG ((EFI_D_ERROR, "TXT_ACM_STATUS_HI (0xFED300A4): %X\n", Data32));
	
  Data32 = *(volatile UINT32 *)(UINTN)TXT_ACM_ERRORCODE;
  DEBUG ((EFI_D_ERROR, "TXT_ACM_ERRORCODE (0xFED30030): %X\n", Data32));
	
  Data32 = *(volatile UINT32 *)(UINTN)TXT_BIOS_ACM_ERRORCODE;
  DEBUG ((EFI_D_ERROR, "TXT_BIOS_ACM_ERRORCODE (0xFED30328): %X\n", Data32));
}

// APTIOV_SERVER_OVERRIDE_RC_END : TXT Error and status Register message printing.
/**

  This routine initializes and collects all Protocols and data required
  by the routines in this file.

  @param ImageHandle   - A pointer to the Image Handle for this file.
  @param SystemTable   - A pointer to the EFI System Table
  @param LtDxeCtx      - A pointer to a caller allocated data structure that contains
                         all of the Protocols and data required by the routines
                         in this file.

  @retval EFI_SUCCESS     - Always.

**/
EFI_STATUS
InitializeLtDxeLib(
  IN      EFI_HANDLE              ImageHandle,
  IN      EFI_SYSTEM_TABLE        *SystemTable,
  IN OUT  LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
{
  EFI_STATUS                Status;
  UINTN                     CpuCount;
  UINTN                     CpusEnabled;
  UINTN                     CpuIndex;
  EFI_PROCESSOR_INFORMATION ProcInfo;
  EFI_HOB_GUID_TYPE         *GuidHob;
  VOID                      *DataInHob;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  //
  // Disable software SMI timer
  //
  SetDisableSoftwareSmi ();

  LtDxeCtx->ImageHandle = ImageHandle;
  LtDxeCtx->SystemTable = SystemTable;

  //
  // Find the CpuIo protocol
  //
  Status = gBS->LocateProtocol (
                  &gEfiCpuIo2ProtocolGuid,
                  NULL,
                  &(LtDxeCtx->CpuIo)
                  );
  ASSERT_EFI_ERROR (Status) ;
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "\tCpuIo Protocol was not found\n"));
  }

  //
  // Find the MpService Protocol to use with LT Config lock
  //
  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  &(LtDxeCtx->MpService)
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "\tMpService Protocol was not found\n"));
    return Status;
  }

  if (!PcdGetBool (PcdProcessorLtsxEnable)) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((EFI_D_ERROR, "\tTXT is disabled\n"));
    return Status;
  }

  //
  // Find the PlatformTxtDeviceMemory HOB
  //
  GuidHob = GetFirstGuidHob (&gEfiPlatformTxtDeviceMemoryGuid);
  if (GuidHob == NULL) {
    DEBUG ((EFI_D_ERROR, "\tgEfiPlatformTxtDeviceMemoryGuid was not found\n"));
    return EFI_NOT_FOUND;
  }
  DataInHob = GET_GUID_HOB_DATA (GuidHob);
  LtDxeCtx->PlatformTxtDeviceMemory = (EFI_PLATFORM_TXT_DEVICE_MEMORY*)DataInHob;

  //
  // Find the PlatformTxtPolicyData HOB
  //
  GuidHob = GetFirstGuidHob (&gEfiPlatformTxtPolicyDataGuid);
  if (GuidHob == NULL) {
    DEBUG ((EFI_D_ERROR, "\tgEfiPlatformTxtPolicyDataGuid was not found\n"));
    return EFI_NOT_FOUND;
  }
  DataInHob = GET_GUID_HOB_DATA (GuidHob);
  LtDxeCtx->PlatformTxtPolicyData = (EFI_PLATFORM_TXT_POLICY_DATA*)DataInHob;

  DEBUG ((EFI_D_INFO, "\tPlatformTxtDeviceMemory:\n"));
  DEBUG ((EFI_D_INFO, "\tNoDMATableAddress                = 0x%08x\n", LtDxeCtx->PlatformTxtDeviceMemory->NoDMATableAddress));
  DEBUG ((EFI_D_INFO, "\tNoDMATableSize                   = 0x%08x\n", LtDxeCtx->PlatformTxtDeviceMemory->NoDMATableSize));
  DEBUG ((EFI_D_INFO, "\tSINITMemoryAddress               = 0x%08x\n", LtDxeCtx->PlatformTxtDeviceMemory->SINITMemoryAddress));
  DEBUG ((EFI_D_INFO, "\tSINITMemorySize                  = 0x%08x\n", LtDxeCtx->PlatformTxtDeviceMemory->SINITMemorySize));
  DEBUG ((EFI_D_INFO, "\tTXTHeapMemoryAddress             = 0x%08x\n", LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemoryAddress));
  DEBUG ((EFI_D_INFO, "\tTXTHeapMemorySize                = 0x%08x\n", LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemorySize));
  DEBUG ((EFI_D_INFO, "\tDMAProtectionMemoryRegionAddress = 0x%08x\n", LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionAddress));
  DEBUG ((EFI_D_INFO, "\tDMAProtectionMemoryRegionSize    = 0x%08x\n", LtDxeCtx->PlatformTxtDeviceMemory->DMAProtectionMemoryRegionSize));
  DEBUG ((EFI_D_INFO, "\tPlatformTxtPolicy:\n"));
  DEBUG ((EFI_D_INFO, "\tBiosAcmAddress           = 0x%08x\n", LtDxeCtx->PlatformTxtPolicyData->BiosAcmAddress));
  DEBUG ((EFI_D_INFO, "\tStartupAcmAddressInFit   = 0x%08x\n", LtDxeCtx->PlatformTxtPolicyData->StartupAcmAddressInFit));
  DEBUG ((EFI_D_INFO, "\tBiosOsDataRegionRevision = 0x%08x\n", LtDxeCtx->PlatformTxtPolicyData->BiosOsDataRegionRevision));
  DEBUG ((EFI_D_INFO, "\tLcpPolicyDataBase        = 0x%08x\n", LtDxeCtx->PlatformTxtPolicyData->LcpPolicyDataBase));
  DEBUG ((EFI_D_INFO, "\tLcpPolicyDataSize        = 0x%08x\n", LtDxeCtx->PlatformTxtPolicyData->LcpPolicyDataSize));
  DEBUG ((EFI_D_INFO, "\tTxtScratchAddress        = 0x%08x\n", LtDxeCtx->PlatformTxtPolicyData->TxtScratchAddress));
  DEBUG ((EFI_D_INFO, "\tBiosAcmPolicy            = 0x%08x\n", LtDxeCtx->PlatformTxtPolicyData->BiosAcmPolicy));

  if (LtDxeCtx->PlatformTxtPolicyData->BiosAcmAddress == 0) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((EFI_D_ERROR, "\tBiosAcmAddress is invalid\n"));
    return Status;
  }
  //
  // Get CPU Information
  //
  Status = LtDxeCtx->MpService->GetNumberOfProcessors (
                           LtDxeCtx->MpService,
                           &CpuCount,
                           &CpusEnabled
                           );
  ASSERT_EFI_ERROR (Status);
  LtDxeCtx->ApCount = (UINT16)(CpuCount - 1);
  DEBUG ((EFI_D_INFO, "\tApCount - 0x%08x\n", LtDxeCtx->ApCount));

  if (LtDxeCtx->ApCount > 0) {
    Status = GetApVector (LtDxeCtx);
    ASSERT_EFI_ERROR (Status);
    DEBUG ((EFI_D_INFO, "\tApVector - 0x%08x\n", LtDxeCtx->ApVector));

    //
    // Get All processor APIC ID info
    //
    for (CpuIndex = 0; CpuIndex < CpuCount; CpuIndex ++) {
      Status = LtDxeCtx->MpService->GetProcessorInfo (
                                        LtDxeCtx->MpService,
                                        CpuIndex,
                                        &ProcInfo);
      ASSERT_EFI_ERROR (Status);
      LtDxeCtx->ApicId[CpuIndex] = (UINT32)ProcInfo.ProcessorId;
      DEBUG ((EFI_D_INFO, "\tApicId[%d] = 0x%08x\n", CpuIndex, LtDxeCtx->ApicId[CpuIndex]));
    }
  }
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return EFI_SUCCESS ;
}

/**

  Determines whether or not the current processor is LT Capable.

  @param  LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

  @retval TRUE          - If the current processor supports LT
  @retval FALSE         - If the current processor does not support LT

**/
BOOLEAN
IsLtProcessor (
  IN      LT_DXE_LIB_CONTEXT  *LtDxeCtx
  )
{
  BOOLEAN                     Status;
  CPUID_VERSION_INFO_ECX      RegEcx;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &RegEcx.Uint32, NULL);
  if (RegEcx.Bits.SMX == 1) {
    Status = TRUE;
  } else {
    Status = FALSE;
  }
  DEBUG ((EFI_D_INFO, "\tProcessor supports SMX: %a\n", Status ? "TRUE" : "FALSE"));
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return Status;
}

/**

  Determines whether or not the platform requires initialization for LT use.

  @param  LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

  @retval TRUE          - If the the platform should be configured for LT.
  @retval FALSE         - If LT is not to be used.

**/
BOOLEAN
IsLtEnabled (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
{
  BOOLEAN   Status;
  UINT64    Ia32FeatureControl;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  Ia32FeatureControl = AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL);
  DEBUG ((EFI_D_INFO, "\tMSR_IA32_FEATURE_CONTROL[0x%08x] = 0x%016lx\n", MSR_IA32_FEATURE_CONTROL, AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL)));
  if (((Ia32FeatureControl & FEATURE_SENTER_ENABLE) != FEATURE_SENTER_ENABLE) ||
      ((Ia32FeatureControl & FEATURE_VMON_IN_SMX)   != FEATURE_VMON_IN_SMX)) {
    Status = FALSE;
  } else {
    Status = TRUE;
  }
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return Status;
}

/**

  Determines whether or not the platform has executed an LT launch by
  examining the TPM Establishment bit.

  @param  LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

  @retval TRUE          - If the TPM establishment bit is asserted.
  @retval FALSE         - If the TPM establishment bit is unasserted.

**/
BOOLEAN
IsLtEstablished (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
{
  EFI_STATUS              Status;
  UINT8                   Access;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  //
  // Read TPM status register
  //
  Status = LtDxeCtx->CpuIo->Mem.Read(
                                  LtDxeCtx->CpuIo,
                                  EfiCpuIoWidthUint8,
                                  (UINT64)(LT_TPM_SPACE_BASE_ADDRESS + LT_TPM_SPACE_ACCESS0),
                                  1,
                                  &Access
                                  );
  ASSERT_EFI_ERROR (Status);

  //
  // The bit we're interested in uses negative logic:
  // If bit 0 == 1 then return False
  // Else return True
  //
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return (Access & 0x1) ? FALSE : TRUE;
}

/**

  Determines presence of TPM in system

  @param  LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

  @retval TRUE          - If the TPM is present.
  @retval FALSE         - If the TPM is not present.

**/
BOOLEAN
IsTpmPresent (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
{
/*
  Replaced next block with line below. might need to check setup option rather than actual TPM presence.
  TPM existence and/or functionality is not essential for DPR programming (which is part of LT code),
  nor TPM functionality is needed for LT functionality (according to the calling function)

  EFI_STATUS              Status;
  UINT8                   Access;

  //
  // Read TPM status register
  //
  Status = LtDxeCtx->CpuIo->Mem.Read (
                                  LtDxeCtx->CpuIo,
                                  EfiCpuIoWidthUint8,
                                  (UINT64)(LT_TPM_SPACE_BASE_ADDRESS + LT_TPM_SPACE_ACCESS0),
                                  1,
                                  &Access
                                  );
  ASSERT_EFI_ERROR (Status);

  return (Access == 0xff) ? FALSE : TRUE;
*/
  return TRUE;
}

/**

  Determines whether TXT Reset Status is set

  @param  LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

  @retval TRUE          - If TXT Reset Status is set.
  @retval FALSE         - If TXT Reset Status is not set.

**/
BOOLEAN
IsLtResetStatus (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
{
  UINT8       Ests;
  EFI_STATUS  Status;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  //
  // Read LT.ESTS register
  //
  Status = LtDxeCtx->CpuIo->Mem.Read (
                              LtDxeCtx->CpuIo,
                              EfiCpuIoWidthUint8,
                              (UINT64)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_ESTS_OFF),
                              1,
                              &Ests
                              );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return (Ests & LT_TXT_RESET_STS) ? TRUE : FALSE;
}

/**

  Get image from FV

  @param NameGuid      - image file guid
  @param SectionType   - image file section type
  @param Buffer        - buffer to hold the image
  @param Size          - image file size
  @param ImageHandle   - FV handle

  @retval EFI_SUCCESS  the image is returned.
  @retval EFI_ERROR    the image is not found.

**/
EFI_STATUS
GetFvImage (
  IN      EFI_GUID                  *NameGuid,
  IN      EFI_SECTION_TYPE          SectionType,
  IN OUT  VOID                      **Buffer,
  IN OUT  UINTN                     *Size,
  IN      EFI_HANDLE                ImageHandle
  )
{
  EFI_STATUS                    Status;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *Fv;
  UINT32                        AuthenticationStatus;
  EFI_LOADED_IMAGE_PROTOCOL     *LoadedImageInfo;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  *Buffer = NULL;
  *Size = 0;
  Status = gBS->HandleProtocol (
                  ImageHandle,
                  &gEfiLoadedImageProtocolGuid,
                  &LoadedImageInfo
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Find desired image in Fv where SmmBase is
  //
  Status = gBS->HandleProtocol (
                  LoadedImageInfo->DeviceHandle,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  &Fv
                  );
  ASSERT_EFI_ERROR (Status);

  Status = Fv->ReadSection (
                 Fv,
                 NameGuid,
                 SectionType,
                 0,
                 Buffer,
                 Size,
                 &AuthenticationStatus
                 );
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return Status;
}

#ifdef SKX_HOST
/**

  Invokes LtDxeLibLaunchBiosAcm to execute the SCHECK function.

  @param LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

  @retval EFI_SUCCESS   - Always.

**/
EFI_STATUS
DoScheck (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
{
  EFI_STATUS              Status;

  if (LtDxeCtx->PlatformTxtPolicyData->StartupAcmAddress != 0) {
    Status = LtDxeLibLaunchBiosAcm (LtDxeCtx, LT_LAUNCH_SCHECK);
  } else {
    DEBUG ((EFI_D_INFO, "Did not call ACM. The ACM module is not enabled...\n"));
    Status = EFI_SUCCESS ;
  }

  return Status;
}
#endif //SKX_HOST

/**

  Invokes LtDxeLibLaunchBiosAcm to execute the lock config function
    with the input buffer defined as type.

  @param LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

  @retval EFI_SUCCESS  - Always

**/
EFI_STATUS
DoLockConfig  (
  IN LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
{
  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  if (LtDxeCtx->PlatformTxtPolicyData->StartupAcmAddress != 0) {
    DEBUG ((EFI_D_INFO, "\tInside the Lock routine of Func6\n"));
    LtDxeLibLaunchBiosAcm (LtDxeCtx, LT_LOCK_CONFIG);
  } else {
    DEBUG ((EFI_D_INFO, "\tDid not call ACM. The ACM module is not enabled...\n"));
  }
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return EFI_SUCCESS ;
}

/**

  Invokes LtDxeLibLaunchBiosAcm to execute the lock config function.

  @param LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

  @retval EFI_SUCCESS   - Always.

**/
VOID
LockConfig (
  IN      VOID      *LtDxeCtx
  )
{
  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  LtDxeLibLaunchBiosAcm ((LT_DXE_LIB_CONTEXT *)LtDxeCtx, LT_LOCK_CONFIG);
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));
}

/**

  Invokes LtDxeLibLaunchBiosAcm to execute the lock config function
    with the input buffer defined as type for all cores.

  @param LtDxeCtx - A pointer to an initialized LT DXE Context data structure

  @retval None

**/
VOID
DoLockConfigForAll (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
{
  UINTN       CpuNumber;
  UINTN       Index;
  EFI_STATUS  Status;
  UINT32      Socket_Index[MAX_SOCKET_COUNT];
  UINT32      SocketID_Found[MAX_SOCKET_COUNT];
  UINT32      Temp_SocketID,SocketID_BSP,Index1;
  UINT32      mNumOfBitShift;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  //
  // Fill in Socket_Index[] and SocketID_Found[] with 0xFFFFFFFF (unknown)
  //
  for (Index = 0; Index < MAX_SOCKET_COUNT; Index++) {
    Socket_Index[Index] = SocketID_Found[Index] = 0xFFFFFFFF;
  }

  //
  // Now call LockConfig
  //
  LtDxeCtx->MpService->WhoAmI (LtDxeCtx->MpService, &CpuNumber);     // CpuNumber = Original BSP
  DEBUG ((EFI_D_INFO, "\tDoLockConfig for BSP - %d\n", CpuNumber));
  LockConfig ((VOID *) LtDxeCtx);

  AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, 1, &mNumOfBitShift, NULL, NULL, NULL);
  mNumOfBitShift &= 0x1F;
  DEBUG ((EFI_D_INFO, "\tmNumOfBitShift! %d\n", mNumOfBitShift));
  SocketID_BSP = (UINT32)(LtDxeCtx->ApicId[CpuNumber])>>mNumOfBitShift;
  DEBUG ((EFI_D_INFO, "\tSocketID_BSP %d\n", SocketID_BSP));
  for (Index = 0; Index < LtDxeCtx->ApCount + 1; Index++) {
    Temp_SocketID = (UINT32)(LtDxeCtx->ApicId[Index])>>mNumOfBitShift;  //Get socket ID
    if (Temp_SocketID == SocketID_BSP) {
      continue;
    }
    for (Index1 = 0; Index1 < MAX_SOCKET_COUNT; Index1++) {
      if (SocketID_Found[Index1] == Temp_SocketID) {
        break;
      }
      if (SocketID_Found[Index1] == 0xFFFFFFFF) {
        SocketID_Found[Index1] = Temp_SocketID;
        Socket_Index[Index1] = (UINT32)Index;
        break;
      }
    }
  }

  for (Index = 0; Index < MAX_SOCKET_COUNT; Index++) {
    DEBUG ((EFI_D_INFO, "\tSocket_Index[%d] = %d\n", Index, Socket_Index[Index]));
  }

  for (Index = 0; Index < MAX_SOCKET_COUNT; Index++) {
    if (Socket_Index[Index] == 0xFFFFFFFF) {
      break;
    }

    DEBUG ((EFI_D_INFO, "\tSwitch BSP to Index=%d, Socket_Index=%d\n", (UINTN)Index,(UINTN)Socket_Index[Index]));

    // Index = The new BSP number
    //
    // call an ASM function to exchange rolls with the currently AP (but future BSP)
    //
    Status = LtDxeCtx->MpService->SwitchBSP (
              LtDxeCtx->MpService,
              (UINTN)Socket_Index[Index],               // The new BSP number
              TRUE
              );
    if (EFI_ERROR (Status)) {
      goto Done;
    }

    //
    // now call LockConfig
    //
    DEBUG ((EFI_D_INFO, "\tDoLockConfig for AP - %d\n", (UINTN)Socket_Index[Index]));
    LockConfig ((VOID *) LtDxeCtx);
  }

  if (Socket_Index[0] != 0xFFFFFFFF) {    //BSP is switched, switch back to original BSP
    // call the ASM function to exchange rolls with the original BSP
    Status = LtDxeCtx->MpService->SwitchBSP (
               LtDxeCtx->MpService,
               CpuNumber,                // Original BSP
               TRUE
               );

    if (EFI_ERROR (Status)) {
      goto Done;
    }
  }

Done:
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return;
}

/**

  Invokes LtDxeLibLaunchBiosAcm to execute the lock config function.

  @param LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

  @retval EFI_SUCCESS   - Always.

**/
EFI_STATUS
TxtLockConfigForAll (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
{
  EFI_STATUS            Status        = EFI_SUCCESS;
  volatile UINT64 *     Ptr64         = NULL;
  UINT64                Value64       = 0;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  //
  // Below code is from BecktonMpService.c
  //
  DoLockConfigForAll (LtDxeCtx);

  //
  // TXTHEAP Registers reset after LockConfig
  //
  DEBUG ((EFI_D_INFO, "\tSet BASE/SIZEs after LockConfig...\n"));
  Ptr64       = (UINT64 *)(UINTN)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_HEAP_BASE_REG_OFF);
  Value64     = (UINT64)LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemoryAddress;
  DEBUG ((EFI_D_INFO, "\tAllocate TXTHeapMemoryAddress = 0x%08x\n", Value64));
  *Ptr64      = Value64;
  DEBUG ((EFI_D_INFO, "\tAfter setting, HEAP.BASE[0x%08x] = 0x%08x\n",(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_HEAP_BASE_REG_OFF), *Ptr64));

  Ptr64       = (UINT64 *)(UINTN)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_HEAP_SIZE_REG_OFF);
  Value64     = (UINT64)LtDxeCtx->PlatformTxtDeviceMemory->TXTHeapMemorySize;
  DEBUG ((EFI_D_INFO, "\tAllocate TXTHeapMemorySize = 0x%08x\n", Value64));
  *Ptr64      = Value64;
  DEBUG ((EFI_D_INFO, "\tAfter setting, HEAP.SIZE[0x%08x] = 0x%08x\n", (LT_PUBLIC_SPACE_BASE_ADDRESS + LT_HEAP_SIZE_REG_OFF), *Ptr64));

  Ptr64       = (UINT64 *)(UINTN)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_SINIT_BASE_REG_OFF);
  Value64     = (UINT64)LtDxeCtx->PlatformTxtDeviceMemory->SINITMemoryAddress;
  DEBUG ((EFI_D_INFO, "\tAllocate SINITMemoryAddress = 0x%08x\n", Value64));
  *Ptr64      = Value64;
  DEBUG ((EFI_D_INFO, "\tAfter setting, SINIT.BASE[0x%08x] = 0x%08x\n", (LT_PUBLIC_SPACE_BASE_ADDRESS + LT_SINIT_BASE_REG_OFF), *Ptr64));

  Ptr64       = (UINT64 *)(UINTN)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_SINIT_SIZE_REG_OFF);
  Value64     = (UINT64)LtDxeCtx->PlatformTxtDeviceMemory->SINITMemorySize;
  DEBUG ((EFI_D_INFO, "\tAllocate SINITMemorySize = 0x%08x\n", Value64));
  *Ptr64      = Value64;
  DEBUG ((EFI_D_INFO, "\tAfter setting, SINIT.SIZE[0x%08x] = 0x%08x\n", (LT_PUBLIC_SPACE_BASE_ADDRESS + LT_SINIT_SIZE_REG_OFF), *Ptr64));
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return Status;

}

/**

  Invokes LtDxeLibLaunchBiosAcm to clear the TPM's aux index.

  @param LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

  @retval EFI_SUCCESS   - Always.

**/
EFI_STATUS
ClearTpmAuxIndex (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
{
  EFI_STATUS              Status;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  //
  // We cannot do getsec if we haven't enabled this instruction in processor
  //
  if ((LtDxeCtx->PlatformTxtPolicyData->StartupAcmAddress != 0) && IsLtEnabled (LtDxeCtx)) {
    Status = LtDxeLibLaunchBiosAcm (LtDxeCtx, LT_CLEAR_TPM_AUX_INDEX);
  } else {
    DEBUG ((EFI_D_INFO, "\tDid not call ACM. The ACM module is not enabled...\n"));
    Status = EFI_SUCCESS ;
  }
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return Status;
}

/**

  Invokes LtDxeLibLaunchBiosAcm to reset the TPM's establishment bit.

  @param LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

  @retval EFI_SUCCESS   - Always.

**/
EFI_STATUS
ResetTpmEstBit (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  )
{
  EFI_STATUS              Status;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  //
  // We cannot do getsec if we haven't enabled this instruction in processor
  //
  if ((LtDxeCtx->PlatformTxtPolicyData->StartupAcmAddress != 0) && IsLtEnabled (LtDxeCtx)) {
    Status = LtDxeLibLaunchBiosAcm (LtDxeCtx, LT_RESET_EST_BIT);
  } else {
    DEBUG ((EFI_D_INFO, "\tDid not call ACM. The ACM module is not enabled ...\n"));
    Status = EFI_SUCCESS ;
  }
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return Status;
}

/**

  Search legacy region for compatibility16 table.

  @param None

  @retval EFI_COMPATIBILITY16_TABLE   - EFI_COMPATIBILITY16_TABLE is found
  @retval NULL                        - EFI_COMPATIBILITY16_TABLE is not found

**/
STATIC
EFI_COMPATIBILITY16_TABLE *
GetCompatibility16Table (
  VOID
  )
{
  UINT8                             *Ptr;
  EFI_COMPATIBILITY16_TABLE         *Table;

  //
  // Scan legacy region
  //
  for (Ptr = (UINT8 *)(UINTN)0xE0000; Ptr < (UINT8 *)(UINTN)(0x100000); Ptr += 0x10) {
    if (*(UINT32 *) Ptr == SIGNATURE_32 ('I', 'F', 'E', '$')) {
      Table   = (EFI_COMPATIBILITY16_TABLE *) Ptr;
      return Table;
    }
  }

  return NULL;
}

#define EFI_COMPATIBILITY16_TABLE_SIGNATURE SIGNATURE_32 ('I', 'F', 'E', '$')

/**
  Find the EFI_COMPATIBILITY16_TABLE in E/F segment.

  @retval The found EFI_COMPATIBILITY16_TABLE or NULL if not found.

**/
UINT8 *
FindCompatibility16Table (
  VOID
  )
{
  UINT8  *Table;

  for (Table = (UINT8 *) (UINTN) 0xE0000;
  Table < (UINT8 *) (UINTN) 0x100000;
  Table = (UINT8 *) Table + 0x10
  ) {
    if (*(UINT32 *) Table == EFI_COMPATIBILITY16_TABLE_SIGNATURE) {
      return Table;
    }
  }

  return NULL;
}

/**

  Find a memory in E/F segment for AP reset vector.

  @param LtDxeCtx      - A pointer to an initialized LT DXE Context data structure

  @retval EFI_SUCCESS   - The ApVector in E/F segment.
  @retval EFI_NOT_FOUND - No enough space for ApVector in E/F segment.

**/
STATIC
EFI_STATUS
GetApVector (
  IN      LT_DXE_LIB_CONTEXT  *LtDxeCtx
  )
{
  EFI_LEGACY_BIOS_PROTOCOL    *LegacyBios;
  EFI_LEGACY_REGION2_PROTOCOL *LegacyRegionProtocol;
  EFI_IA32_REGISTER_SET       Regs;
  EFI_PHYSICAL_ADDRESS        LegacyRegionAddress;
  EFI_STATUS                  Status;
  UINT32                      WakeUpBufferAddr;
  UINT32                      BackupBuffer;
  EFI_COMPATIBILITY16_TABLE   *Table;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  WakeUpBufferAddr = 0;
  LtDxeCtx->ApVector = 0;
  Status = gBS->LocateProtocol (&gEfiLegacyBiosProtocolGuid, NULL, &LegacyBios);
  if (!EFI_ERROR (Status)) {
    Table = (EFI_COMPATIBILITY16_TABLE *) FindCompatibility16Table ();
    ASSERT (Table != NULL);
    if (Table == NULL) {
      return EFI_NOT_FOUND;
    }
    //
    // Allocate 0x1004 bytes from below 1M memory to make sure I can
    // get a 4k aligned spaces of 0x1000 bytes, since Alignment argument does not work.
    //
    Regs.X.AX = (UINT16)Legacy16GetTableAddress;
    Regs.X.CX = (UINT16)(sizeof(BackupBuffer) + EFI_PAGE_SIZE);
    Regs.X.BX = (UINT16)0x2;//Location 0xE0000 or 0xF0000; BIT0: F seg, BIT1: E seg
    Regs.X.DX = (UINT16)0;//Alignment 2^0 = 1 byte

    Status = gBS->LocateProtocol (&gEfiLegacyRegion2ProtocolGuid, NULL, &LegacyRegionProtocol);
    ASSERT_EFI_ERROR (Status);
    LegacyRegionProtocol->UnLock (LegacyRegionProtocol, 0xC0000, 0x40000, NULL);
    LegacyBios->FarCall86 (
                  LegacyBios,
                  Table->Compatibility16CallSegment,
                  Table->Compatibility16CallOffset,
                  &Regs,
                  NULL,
                  0
                  );
    LegacyRegionProtocol->Lock (LegacyRegionProtocol, 0xC0000, 0x40000, NULL);
    ASSERT (Regs.X.AX == 0);
    if (Regs.X.AX == 0) {
      WakeUpBufferAddr = (UINT32)((Regs.X.DS << 4) + Regs.X.BX + 0x4);
      WakeUpBufferAddr = (WakeUpBufferAddr + 0x0fff) & 0xfffff000;
    } else {
      return EFI_NOT_FOUND;
    }

  //
  // Fill in machine code for cli, hlt, and jmp $-2
  //
  DEBUG ((EFI_D_INFO, "\tWakeUpBufferAddr         = 0x%08x\n", (UINTN)WakeUpBufferAddr));
  LegacyRegionProtocol->UnLock (LegacyRegionProtocol, 0xC0000, 0x40000, NULL);
  *(UINT32 *)(UINTN)WakeUpBufferAddr = 0xFCEBF4FA;
  LegacyRegionProtocol->Lock (LegacyRegionProtocol, 0xC0000, 0x40000, NULL);

  LtDxeCtx->ApVector = (UINT8)(WakeUpBufferAddr >> 12);

  return EFI_SUCCESS;

  } else {
    //
    // No LegacyBios Protocol means NO-CSM
    //

    //
    // Allocate 0x1004 bytes from below 1M memory to make sure I can
    // get a 4k aligned spaces of 0x1000 bytes, since Alignment argument does not work.
    //
    LegacyRegionAddress = 0xFFFFF;
    Status = gBS->AllocatePages (
                    AllocateMaxAddress,
                    EfiReservedMemoryType,
                    EFI_SIZE_TO_PAGES(sizeof(BackupBuffer) + EFI_PAGE_SIZE),
                    &LegacyRegionAddress
                    );
    ASSERT_EFI_ERROR (Status);

    WakeUpBufferAddr = (UINT32) LegacyRegionAddress;
    WakeUpBufferAddr = (WakeUpBufferAddr + 0x0fff) & 0xfffff000;

    //
    // Fill in machine code for cli, hlt, and jmp $-2
    //
    DEBUG ((EFI_D_INFO, "\tWakeUpBufferAddr         = 0x%08x\n", (UINTN)WakeUpBufferAddr));
    *(UINT32 *)(UINTN)WakeUpBufferAddr = 0xFCEBF4FA;

    LtDxeCtx->ApVector = (UINT8)(WakeUpBufferAddr >> 12);
    DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

    return EFI_SUCCESS;
  }
}

/**
  Determines if SGX is enabled. If enabled, bypass all MCi_CTL reads/writes in LtDxeLibLaunchBiosAcm
  @param  None
  @retval TRUE          - If SGX is enabled
  @retval FALSE         - If SGX is disabled
**/
STATIC
BOOLEAN
IsSgxEnabled (
  )
{
  BOOLEAN   Status;
  UINT64    Ia32FeatureControl;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  Ia32FeatureControl = AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL);
  DEBUG ((EFI_D_INFO, "\tMSR_IA32_FEATURE_CONTROL[0x%08x] = 0x%016lx\n", MSR_IA32_FEATURE_CONTROL, Ia32FeatureControl));
  Status = ((Ia32FeatureControl & SGX_GLOBAL_ENABLE) == SGX_GLOBAL_ENABLE) ? TRUE : FALSE;
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return Status;
}

/**

  Sets up the system and then launches the LT BIOS ACM to run the function
  requested by AcmFunction.

  @param AcmFunction   - Constant that represents the function from the BIOS ACM
                         that should be executed.

  @retval EFI_SUCCESS   - Always.

**/
STATIC
EFI_STATUS
LtDxeLibLaunchBiosAcm (
  IN LT_DXE_LIB_CONTEXT       *LtDxeCtx,
  IN UINT32                   AcmFunction
  )
{
  EFI_STATUS                  Status ;
  EFI_TPL                     OldTpl;
  UINTN                       MyCpuNumber ;
// BEGIN_OVERRIDE_HSD_5331848
  UINT32                      Ia32_Mc9_Ctl_Org ;
  UINT32                      Ia32_Mc10_Ctl_Org;
  UINT32                      Ia32_Mc11_Ctl_Org;
  UINT32                      Ia32_Mc9_Ctl_New;
  UINT32                      Ia32_Mc10_Ctl_New;
  UINT32                      Ia32_Mc11_Ctl_New;
  UINT32                      Ia32_Mcg_Contain_Org = 0;
  UINT32                      Ia32_Mcg_Contain_New = 0;
  BOOLEAN                     SgxStatus;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  SgxStatus = IsSgxEnabled ();
  if (SgxStatus == FALSE) {
    if ((AsmReadMsr32 (MSR_IA32_MCG_CAP) & BIT24) != 0) {
      // If IA32_MCG_CAP.MCG_SER_P equal to 1, write a value of 0 to MCG_CONTAIN
      Ia32_Mcg_Contain_Org = AsmReadMsr32 (MSR_MCG_CONTAIN);
      AsmWriteMsr32 (MSR_MCG_CONTAIN, Ia32_Mcg_Contain_New);
      DEBUG ((EFI_D_INFO, "\tMSR_MCG_CONTAIN[0x%08x] = 0x%08x\n", MSR_MCG_CONTAIN, AsmReadMsr32 (MSR_MCG_CONTAIN)));
    }
    // Backup MSR_IA32_MCx_CTL register
    Ia32_Mc9_Ctl_Org  = AsmReadMsr32 (MSR_IA32_MC9_CTL);
    Ia32_Mc10_Ctl_Org = AsmReadMsr32 (MSR_IA32_MC10_CTL);
    Ia32_Mc11_Ctl_Org = AsmReadMsr32 (MSR_IA32_MC11_CTL);

    // Clear BIT7 SAD_ERR_NON_CORRUPTING_OTHER
    Ia32_Mc9_Ctl_New  = Ia32_Mc9_Ctl_Org & ~BIT7;
    Ia32_Mc10_Ctl_New = Ia32_Mc10_Ctl_Org & ~BIT7;
    Ia32_Mc11_Ctl_New = Ia32_Mc11_Ctl_Org & ~BIT7;

    AsmWriteMsr32 (MSR_IA32_MC9_CTL, Ia32_Mc9_Ctl_New);
    AsmWriteMsr32 (MSR_IA32_MC10_CTL, Ia32_Mc10_Ctl_New);
    AsmWriteMsr32 (MSR_IA32_MC11_CTL, Ia32_Mc11_Ctl_New);
    DEBUG ((EFI_D_INFO, "\tMSR_IA32_MC9_CTL[0x%08x]  = 0x%08x\n", MSR_IA32_MC9_CTL,  AsmReadMsr32 (MSR_IA32_MC9_CTL)));
    DEBUG ((EFI_D_INFO, "\tMSR_IA32_MC10_CTL[0x%08x] = 0x%08x\n", MSR_IA32_MC10_CTL, AsmReadMsr32 (MSR_IA32_MC10_CTL)));
    DEBUG ((EFI_D_INFO, "\tMSR_IA32_MC11_CTL[0x%08x] = 0x%08x\n", MSR_IA32_MC11_CTL, AsmReadMsr32 (MSR_IA32_MC11_CTL)));
  }
// END_OVERRIDE_HSD_5331848
  MyCpuNumber = 0;
  if (LtDxeCtx->ApCount > 0) {
    Status = LtDxeCtx->MpService->WhoAmI (
                                    LtDxeCtx->MpService,
                                    &MyCpuNumber
                                    );
    ASSERT_EFI_ERROR (Status);

    //
    // Save AP configuration. Run on one AP since all must be programmed
    // identically
    SendInitIpiAllExcludingSelf ();
  }

  //
  // Launch the BIOS ACM to run the requested function
  //
  DEBUG ((EFI_D_INFO, "\tRaising TPL\n"));
  OldTpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
  DEBUG ((EFI_D_INFO, "\tBiosAcmAddress = 0x%08x\tAcmFunction = 0x%08x\n", (UINTN)LtDxeCtx->PlatformTxtPolicyData->BiosAcmAddress, AcmFunction));
  DEBUG ((EFI_D_INFO, "\tLaunchBiosAcm (0x%08x)\n", AcmFunction));
  LaunchBiosAcm ((UINTN)LtDxeCtx->PlatformTxtPolicyData->BiosAcmAddress, (UINTN)AcmFunction);
  DEBUG ((EFI_D_INFO, "\tRestoring TPL\n"));
  gBS->RestoreTPL (OldTpl);
  DEBUG ((EFI_D_INFO, "\tSending SIPI\n"));
  SendInitSipiSipiAllExcludingSelf (LtDxeCtx->ApVector << 12);

// BEGIN_OVERRIDE_HSD_5331848
  if (SgxStatus == FALSE) {
    if ((AsmReadMsr32 (MSR_IA32_MCG_CAP) & BIT24) != 0) {
      // If IA32_MCG_CAP.MCG_SER_P equal to 1, restore MCG_CONTAIN to its previous value
      AsmWriteMsr32 (MSR_MCG_CONTAIN, Ia32_Mcg_Contain_Org);
      DEBUG ((EFI_D_INFO, "\tMSR_MCG_CONTAIN[0x%08x] = 0x%08x\n", MSR_MCG_CONTAIN, AsmReadMsr32 (MSR_MCG_CONTAIN)));
    }

    AsmWriteMsr32 (MSR_IA32_MC9_STATUS,  0);
    AsmWriteMsr32 (MSR_IA32_MC10_STATUS, 0);
    AsmWriteMsr32 (MSR_IA32_MC11_STATUS, 0);

    AsmWriteMsr32 (MSR_IA32_MC9_CTL,  Ia32_Mc9_Ctl_Org);
    AsmWriteMsr32 (MSR_IA32_MC10_CTL, Ia32_Mc10_Ctl_Org);
    AsmWriteMsr32 (MSR_IA32_MC11_CTL, Ia32_Mc11_Ctl_Org);
    DEBUG ((EFI_D_INFO, "\tMSR_IA32_MC9_CTL[0x%08x]  = 0x%08x\n", MSR_IA32_MC9_CTL,  AsmReadMsr32 (MSR_IA32_MC9_CTL)));
    DEBUG ((EFI_D_INFO, "\tMSR_IA32_MC10_CTL[0x%08x] = 0x%08x\n", MSR_IA32_MC10_CTL, AsmReadMsr32 (MSR_IA32_MC10_CTL)));
    DEBUG ((EFI_D_INFO, "\tMSR_IA32_MC11_CTL[0x%08x] = 0x%08x\n", MSR_IA32_MC11_CTL, AsmReadMsr32 (MSR_IA32_MC11_CTL)));
  }
// END_OVERRIDE_HSD_5331848
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return EFI_SUCCESS ;
}

/**
  Once LT BIOS ACM error, disable Ltsx and re-boot the system.

  @param None

  @retval No return

**/
EFI_STATUS
DisableLtsxAndReset (
  VOID
  )
{
  UINT8                                 Data8;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  //
  // Read the current system configuration variable store.
  //
  IoWrite8 (0x70, CMOS_LTSX_OFFSET);
  Data8 = IoRead8 (0x71);
  Data8 &= ~(BIT4 | BIT5);
  DEBUG ((EFI_D_INFO, "\tCMOS_LTSX_OFFSET[0x%02x] = 0x%02x: for LT/TPM policy\n", CMOS_LTSX_OFFSET, Data8));
  //
  // Write back CMOS Bit4 and Bit5
  //
  IoWrite8 (0x70, CMOS_LTSX_OFFSET);
  IoWrite8 (0x71, Data8);

  IoWrite8 (0x70, CMOS_LTSX_OFFSET);
  Data8 = IoRead8 (0x71);
  DEBUG ((EFI_D_INFO, "\tCMOS_LTSX_OFFSET[0x%02x] = 0x%02x: for LT/TPM policy\n", CMOS_LTSX_OFFSET, Data8));

  SetDisableLtsx ();

  //
  // Do this powergood reset only if we're not in HSX PO
  // where a bootscript is needed to start the CPU
  //
  DEBUG ((EFI_D_ERROR, "\tACM Fails : do powergood reset!\n"));
  gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);

  // Do powergood reset
  //
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return EFI_SUCCESS;
}
