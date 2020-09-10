/** @file
  This file contains an implementation of the function call interfaces
  required by the main LT PEIM file.  Hopefully, future platform porting
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

#include "TxtPeiLib.h"

#include <Library/LtPeiLib.h>
#include "../Common/LaunchBiosAcm.h"

//
// LT Configuration Space Register Definitions
//
#define LT_PUBLIC_SPACE_BASE_ADDRESS     0xfed30000
#define LT_ESTS_OFF                      0x8
#define   LT_TXT_RESET_STS               0x1
#define   LT_WAKE_ERROR_STS              (0x1 << 6)
#define LT_HEAP_BASE_REG_OFF             0x300
#define LT_HEAP_SIZE_REG_OFF             0x308

#define LT_TPM_SPACE_BASE_ADDRESS        0xfed40000
#define LT_TPM_SPACE_ACCESS0             0x0

#define N_MSR_BSP_FLAG                   8

#define GETSEC_PARAMETER_TYPE_MASK                0x1F
#define GETSEC_PARAMETER_TYPE_LT_EXTENSIONS       0x05
#define GETSEC_PARAMETER_TYPE_LT_EXTENSIONS_FLAGS 0x20

#define FEATURE_LOCK_BIT                0x0001
#define FEATURE_VMON_IN_SMX             0x0002
#define FEATURE_VMON_OUT_SMX            0x0004
#define FEATURE_SENTER_ENABLE           0xFF00

#define SGX_GLOBAL_ENABLE               BIT18

EFI_GUID mTxtApInitGuid = { 0x9b4a2f66, 0x6966, 0x4c44, { 0xa1, 0x92, 0x4f, 0xee, 0x28, 0x84, 0x4d, 0xb5 } };

STATIC
EFI_STATUS
GetApVector (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

STATIC
BOOLEAN
IsSgxEnabled(
  VOID
  );

/**

  This routine initializes and collects all PPIs and data required
  by the routines in this file.

  @param PeiServices      - A pointer to the PEI Service Table
  @param LtPeiCtx         - A pointer to a caller allocated data structure that contains
                            all of the PPIs and data required by the routines in this file

  @retval EFI_UNSUPPORTED - If any of the required PPIs or data are unavailable
  @retval EFI_SUCCESS     - In all cases not listed above

**/
EFI_STATUS
InitializeLtPeiLib (
  IN      CONST EFI_PEI_SERVICES  **PeiServices,
  IN OUT  LT_PEI_LIB_CONTEXT      *LtPeiCtx
  )
{
  EFI_STATUS                      Status;
  EFI_HOB_GUID_TYPE               *GuidHob;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  //
  // Initialize all pointers
  //
  LtPeiCtx->PeiServices           = PeiServices;
  LtPeiCtx->CpuIoPpi              = (*(LtPeiCtx->PeiServices))->CpuIo;
  LtPeiCtx->PciCfgPpi             = (*(LtPeiCtx->PeiServices))->PciCfg;

  //Status = (*PeiServices)->LocatePpi(PeiServices, &gEfiPeiStallPpiGuid, 0, NULL, &(LtPeiCtx->PeiStall));
  //ASSERT_EFI_ERROR(Status);

  //
  // Find PeiMpServices PPI, only need success locating at end of S3 resume
  //
  Status = GetPeiMpServices (PeiServices, & (LtPeiCtx->PeiMpServices));

  //
  // TXT Memory HOB
  //
  GuidHob = GetFirstGuidHob (&gEfiPlatformTxtDeviceMemoryGuid);
  if (GuidHob == NULL) {
    DEBUG ((EFI_D_ERROR, "\tTXT Memory HOB gEfiPlatformTxtDeviceMemoryGuid was not found\n"));
    LtPeiCtx->PlatformTxtDeviceMemory = NULL;
  } else {
    LtPeiCtx->PlatformTxtDeviceMemory = (EFI_PLATFORM_TXT_DEVICE_MEMORY *) GET_GUID_HOB_DATA (GuidHob);
  }

  //
  // TXT Policy HOB
  //
  GuidHob = GetFirstGuidHob (&gEfiPlatformTxtPolicyDataGuid);
  if (GuidHob == NULL) {
    DEBUG ((EFI_D_ERROR, "\tTXT Policy HOB gEfiPlatformTxtPolicyDataGuid was not found\n"));
    return EFI_NOT_FOUND;
  }
  LtPeiCtx->PlatformTxtPolicyData = (EFI_PLATFORM_TXT_POLICY_DATA *) GET_GUID_HOB_DATA (GuidHob);

  DEBUG ((EFI_D_INFO, "\tPlatformTxtPolicy:\n"));
  DEBUG ((EFI_D_INFO, "\tBiosAcmAddress = 0x%08x\n", LtPeiCtx->PlatformTxtPolicyData->BiosAcmAddress));
  DEBUG ((EFI_D_INFO, "\tTxtScratchAddress = 0x%08x\n", LtPeiCtx->PlatformTxtPolicyData->TxtScratchAddress));
  DEBUG ((EFI_D_INFO, "\tBiosAcmPolicy = 0x%08x\n", LtPeiCtx->PlatformTxtPolicyData->BiosAcmPolicy));

  Status = GetApVector (LtPeiCtx);
  DEBUG ((EFI_D_INFO, "\tApVector = 0x%08x\n", LtPeiCtx->ApVector));
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return EFI_SUCCESS ;
}

/**

  Determines whether or not the current processor is LT Capable.

  @param  LtPeiCtx      - A pointer to an initialized LT PEI Context data structure

  @retval TRUE          - If the current processor supports LT
  @retval FALSE         - If the current processor does not support LT

**/
BOOLEAN
IsLtProcessor (
  IN      LT_PEI_LIB_CONTEXT  *LtPeiCtx
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

  Enable TXT on BSP.

  @param LtPeiCtx      - A pointer to an initialized LT PEI Context data structure

  @retval None

**/
VOID
EnableTxt (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  )
{
  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  AsmWriteMsr64 (MSR_IA32_FEATURE_CONTROL, AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL) | (FEATURE_VMON_IN_SMX | FEATURE_VMON_OUT_SMX));
  DEBUG ((EFI_D_INFO, "\tMSR_IA32_FEATURE_CONTROL[0x%08x] = 0x%016lx\n", MSR_IA32_FEATURE_CONTROL, AsmReadMsr64(MSR_IA32_FEATURE_CONTROL)));
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return;
}

/**

  Sets the Lock Bit in the LT Control MSR.  Called by all threads.

  @param Not used

  @retval EFI_SUCCESS   - Always.

**/
VOID
LockLtMsr (
  IN      VOID      *Buffer
  )
{
  UINT64              MsrLtCtrlSts;

  //
  // Set LT_CONTROL_LOCK
  //
  MsrLtCtrlSts = 0x1;
  AsmWriteMsr64 (MSR_LTCTRLSTS, MsrLtCtrlSts);

  return;
}

/**

  Lock LT MSR on all CPU threads.

  @param  LtPeiCtx            - A pointer to an initialized LT PEI Context data structure

  @retval EFI_SUCCESS         - Multiple processors re-load microcode patch

**/
VOID
LockLtMsrOnAll(
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  )
{
  EFI_STATUS  Status;

  //
  // Lock LT Control MSR on BSP, then on all APs
  //
  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  LockLtMsr (NULL);
  Status = LtPeiCtx->PeiMpServices->StartupAllAPs (
            LtPeiCtx->PeiServices,
            LtPeiCtx->PeiMpServices,
            LockLtMsr,
            FALSE,
            0,
            (VOID*)NULL
            );
  DEBUG ((EFI_D_INFO, "\tMSR_LTCTRLSTS[0x%08x] = 0x%016lx\n", MSR_LTCTRLSTS, AsmReadMsr64(MSR_LTCTRLSTS)));
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "\tError: Not able to Lock LT MSR. Status: %r\n", Status));
  }
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return;
}

/**

  Determines presence of TPM in system

  @param  LtPeiCtx      - A pointer to an initialized LT PEI Context data structure

  @retval TRUE          - If the TPM is present.
  @retval FALSE         - If the TPM is not present.

**/
BOOLEAN
IsTpmPresent (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  )
{
/*Replaced next block with line below. might need to check setup option rather than actual TPM presence.
  TPM existence and/or functionality is not essential for DPR programming (which is part of LT code),
  nor TPM functionality is needed for LT functionality (according to the calling function)

  UINT8                           Access ;

  //
  // Read TPM status register
  //
  Access = LtPeiCtx->CpuIoPpi->MemRead8 (
                                 LtPeiCtx->PeiServices,
                                 LtPeiCtx->CpuIoPpi,
                                 LT_TPM_SPACE_BASE_ADDRESS + LT_TPM_SPACE_ACCESS0
                                 );
  return (Access == 0xff) ? FALSE : TRUE;
*/
  return TRUE;
}

/**

  Determines whether or not the platform has executed an LT launch by
  examining the TPM Establishment bit.

  @param  LtPeiCtx      - A pointer to an initialized LT PEI Context data structure

  @retval TRUE          - If the TPM establishment bit is asserted.
  @retval FALSE         - If the TPM establishment bit is unasserted.

**/
BOOLEAN
IsLtEstablished (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  )
{
  UINT8                           Access;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  //
  // Read TPM status register
  //
  Access = LtPeiCtx->CpuIoPpi->MemRead8 (
                                 LtPeiCtx->PeiServices,
                                 LtPeiCtx->CpuIoPpi,
                                 LT_TPM_SPACE_BASE_ADDRESS + LT_TPM_SPACE_ACCESS0
                                 );

  //
  // The bit we're interested in uses negative logic:
  // If bit 0 == 1 then return False
  // Else return True
  //
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return (Access & 0x1) ? FALSE : TRUE;
}

/**

  Determines whether or not the platform has encountered an error during
  a sleep or power-off state.

  @param LtPeiCtx      - A pointer to an initialized LT PEI Context data structure

  @retval TRUE          - If the LT_WAKE_ERROR bit is asserted.
  @retval FALSE         - If the LT_WAKE_ERROR bit is unasserted.

**/
BOOLEAN
IsLtWakeError (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  )
{
  UINT8       Ests;

  //
  // Read LT.ESTS register
  //
  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  Ests = LtPeiCtx->CpuIoPpi->MemRead8(
                               LtPeiCtx->PeiServices,
                               LtPeiCtx->CpuIoPpi,
                               LT_PUBLIC_SPACE_BASE_ADDRESS + LT_ESTS_OFF
                               );

  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return (Ests & LT_WAKE_ERROR_STS) ? TRUE : FALSE;
}

/**

  Determines whether TXT Reset Status is set

  @param LtPeiCtx      - A pointer to an initialized LT PEI Context data structure

  @retval TRUE          - If the LT_RESET_STATUS bit is asserted.
  @retval FALSE         - If the LT_RESET_STATUS bit is unasserted.

**/
BOOLEAN
IsLtResetStatus (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  )
{
  UINT8       Ests;

  //
  // Read LT.ESTS register
  //
  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  Ests = LtPeiCtx->CpuIoPpi->MemRead8(
                               LtPeiCtx->PeiServices,
                               LtPeiCtx->CpuIoPpi,
                               LT_PUBLIC_SPACE_BASE_ADDRESS + LT_ESTS_OFF
                               );
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return (Ests & LT_TXT_RESET_STS) ? TRUE : FALSE;
}

/**

  Determines whether or not the platform requires initialization for LT use.

  @param LtPeiCtx       - A pointer to an initialized LT PEI Context data structure

  @retval TRUE          - If the the platoform should be configured for LT.
  @retval FALSE         - If LT is not to be used.

**/
BOOLEAN
IsLtEnabled (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  )
{
  BOOLEAN   Status;
  UINT64    Ia32FeatureControl;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  Ia32FeatureControl = AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL);
  DEBUG ((EFI_D_INFO, "\tMSR_IA32_FEATURE_CONTROL[0x%08x] = 0x%016lx\n", MSR_IA32_FEATURE_CONTROL, AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL)));
  if (((Ia32FeatureControl & FEATURE_SENTER_ENABLE) != FEATURE_SENTER_ENABLE) ||
      ((Ia32FeatureControl & FEATURE_VMON_IN_SMX)   != FEATURE_VMON_IN_SMX) ) {
    Status = FALSE;
  } else {
    Status = TRUE;
  }
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return Status;
}

/**

  Do system power cycle reset

  @param LtPeiCtx      - A pointer to an initialized LT PEI Context data structure

  @retval None

**/
VOID
PowerCycleReset (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  )
{
  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  LtPeiCtx->CpuIoPpi->IoWrite8 (
                          LtPeiCtx->PeiServices,
                          LtPeiCtx->CpuIoPpi,
                          0xCF9,
                          0xA
                          );
  LtPeiCtx->CpuIoPpi->IoWrite8 (
                          LtPeiCtx->PeiServices,
                          LtPeiCtx->CpuIoPpi,
                          0xCF9,
                          0xE
                          );
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  while (1);
}

/**

Re-initialize APs after running ACM commands

  @param LtPeiCtx      - A pointer to an initialized LT PEI Context data structure

  @retval - None

**/
STATIC
VOID
InitializeAps (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  )
{
  UINT64   MsrData;
  UINT32   McuRevision;

  //
  // 1. Check BSP Microcode update done. (Minimal check)
  //
  MsrData = AsmReadMsr64 (MSR_IA32_BIOS_SIGN_ID);
  McuRevision = (UINT32)RShiftU64 (MsrData, 32);
  ASSERT (McuRevision != 0);

  //
  // 2. Send SIPI to let AP do something, currently only Microcode update.
  //    Platform-specific code could also be put here and/or ApInit code.
  //
  if (LtPeiCtx->ApVector != 0) {
    SendInitSipiSipiAllExcludingSelf(LtPeiCtx->ApVector << 12);  //Library function shifts right 12 bits

    //
    // We delay some time to let ap finish its task.
    // 40ms delay (Hardcoded now, should use semaphore or something else)
    //
    //LtPeiCtx->PeiStall->Stall (LtPeiCtx->PeiServices, LtPeiCtx->PeiStall, 40000);
    MicroSecondDelay(40000);
  }

  return;
}

/**

  Searches FFS for file with pGuid

  @param LtPeiCtx      - A pointer to an initialized LT PEI Context data structure
  @param Guid          - FFS guid
  @param Module        - FFS module address

  @retval EFI_SUCCESS   - File is found.
  @retval Other status  - File not found.

**/
STATIC
EFI_STATUS
FindModuleInFlash (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx,
  IN      EFI_GUID                *Guid,
  OUT     UINTN                   *Module
  )
{
#if (PI_SPECIFICATION_VERSION < 0x00010000)
  EFI_FIRMWARE_VOLUME_HEADER  *FvHeader;
  EFI_FFS_FILE_HEADER         *FileHeader;
#else
  EFI_PEI_FV_HANDLE           VolumeHandle;
  EFI_PEI_FILE_HANDLE         *FileHandle;
  EFI_PEI_FV_HANDLE           *FvHeader;
  EFI_FFS_FILE_HEADER         *FileHeader;
#endif
  EFI_STATUS                  Status;
  UINTN                       Instance;

  Instance = 0;
  FvHeader = NULL;
  while (TRUE) {
#if (PI_SPECIFICATION_VERSION < 0x00010000)
    Status = (*(LtPeiCtx->PeiServices))->FfsFindNextVolume (LtPeiCtx->PeiServices, Instance, &FvHeader);
#else
    VolumeHandle = NULL;
    Status = (*(LtPeiCtx->PeiServices))->FfsFindNextVolume (LtPeiCtx->PeiServices, Instance, &VolumeHandle);
    FvHeader = &VolumeHandle;
#endif
    if (EFI_ERROR (Status)) {
      return Status;
    }

    FileHeader = NULL;
#if (PI_SPECIFICATION_VERSION >= 0x00010000)
    FileHandle = (EFI_PEI_FILE_HANDLE *)&FileHeader;
#endif
    while (TRUE) {
#if (PI_SPECIFICATION_VERSION < 0x00010000)
      Status = (*(LtPeiCtx->PeiServices))->FfsFindNextFile (LtPeiCtx->PeiServices, EFI_FV_FILETYPE_RAW, FvHeader, &FileHeader);
#else
      //APTIOV_SERVER_OVERRIDE_START: pointer mismatch
      Status = (*(LtPeiCtx->PeiServices))->FfsFindNextFile (LtPeiCtx->PeiServices, EFI_FV_FILETYPE_RAW, FvHeader, FileHandle);
      //APTIOV_SERVER_OVERRIDE_END: pointer mismatch
      // According to PI spec, FileHandle is FileHeader.
      FileHeader = (EFI_FFS_FILE_HEADER *)*FileHandle;
#endif
      if (EFI_ERROR (Status)) {
        break;
      }

      if (CompareGuid (&(FileHeader->Name), Guid)) {
        *Module = (UINTN)FileHeader + sizeof(EFI_FFS_FILE_HEADER);
        return EFI_SUCCESS;
      }
    }
    Instance ++;
  }
}

/**

  Find a memory in E/F segment for AP reset vector.

  @param LtPeiCtx      - A pointer to an initialized LT PEI Context data structure

  @retval ApVector      - The ApVector in E/F segment.

**/
STATIC
EFI_STATUS
GetApVector (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  )
{
  EFI_STATUS                  Status;
  UINTN                       ApStartup;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  LtPeiCtx->ApVector = 0;
  Status = FindModuleInFlash (LtPeiCtx, &mTxtApInitGuid, &ApStartup);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  //
  // Found ApInit module
  //
  if (((ApStartup & 0xFFF) == 0) &&
      (ApStartup >= 0xFFFE0000) &&
      (ApStartup <= 0xFFFFF000)) {
    LtPeiCtx->ApVector = (UINT8)(ApStartup >> 12);
    DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));
    return EFI_SUCCESS;
  } else {
    ASSERT ((ApStartup & 0xFFF) == 0);
    ASSERT ((ApStartup >= 0xFFFE0000) &&  (ApStartup <= 0xFFFFF000));
    DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));
    return EFI_NOT_FOUND;
  }
}

//
//  Start of LT SX function interface into BIOS ACM module. Note: Only function 5 is here. Function 6 is in DXE
//

//
// Function 5 interface into StartupACM
//
/**

  Invokes the LT_CLEAR_SECRETS function in the LT StartupACM.

  @param LtPeiCtx       - A pointer to an initialized LT PEI Context data structure

  @retval EFI_SUCCESS   - Always.

**/
EFI_STATUS
DoClearSecrets (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  )
{
  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  if (LtPeiCtx->PlatformTxtPolicyData->StartupAcmAddress != 0) {
    InitializeAps (LtPeiCtx);
    SendInitIpiAllExcludingSelf ();
    LtPeiLibLaunchBiosAcm (LtPeiCtx, LT_CLEAR_SECRETS);
  } else {
    DEBUG ((EFI_D_ERROR, "\tDid not call ACM. The ACM module is not enabled\n"));
  }
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return EFI_SUCCESS ;
}

/**

  Calls DoLockConfigForAll to run LockConfig on all sockets then restores HEAP registers.

  @param LtPeiCtx      - A pointer to an initialized LT PEI Context data structure

  @retval EFI_SUCCESS   - Always.

**/
EFI_STATUS
PeiTxtLockConfigForAll (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  )
{
  UINT64          *Ptr64;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  PeiDoLockConfigForAll (LtPeiCtx);

  //
  // TXTHEAP Registers reset after lockconfig
  //
  Ptr64       = (UINT64 *)(UINTN)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_HEAP_BASE_REG_OFF);
  DEBUG ((EFI_D_INFO, "\tRestoring TXTHeapMemoryAddress = 0x%08x\n", LtPeiCtx->PlatformTxtDeviceMemory->TXTHeapMemoryAddress));
  *Ptr64      = LtPeiCtx->PlatformTxtDeviceMemory->TXTHeapMemoryAddress;

  Ptr64       = (UINT64 *)(UINTN)(LT_PUBLIC_SPACE_BASE_ADDRESS + LT_HEAP_SIZE_REG_OFF);
  DEBUG ((EFI_D_INFO, "\tRestoring TXTHeapMemorySize = 0x%08x\n", LtPeiCtx->PlatformTxtDeviceMemory->TXTHeapMemorySize));
  *Ptr64      = LtPeiCtx->PlatformTxtDeviceMemory->TXTHeapMemorySize;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));
  return EFI_SUCCESS;
}

/**

  Performs the action of calling DoLockConfig on all sockets and all necessary initialization

  @param LtPeiCtx      - A pointer to an initialized LT PEI Context data structure

  @retval EFI_SUCCESS   - Always.

**/
EFI_STATUS
PeiDoLockConfigForAll (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  )
{
  UINTN                       Index, Index1;
  EFI_STATUS                  Status;
  UINTN                       CpuCount;
  UINTN                       CpuCountEnabled;
  EFI_PROCESSOR_INFORMATION   CpuInfoBuffer;
  LT_PEI_SOCKET_INFO          SocketInfo[MAX_SOCKET_COUNT];
  UINT8                       Temp_SocketID, SocketID_BSP;
  UINT32                      mNumOfBitShift;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  //
  // Initialize SocketInfo to all 0xFF
  //
  SetMem (&SocketInfo, sizeof (LT_PEI_SOCKET_INFO) * MAX_SOCKET_COUNT, 0xff);

  //
  // Get total threads to be used to find all sockets
  //
  Status = LtPeiCtx->PeiMpServices->GetNumberOfProcessors(
              LtPeiCtx->PeiServices,
              LtPeiCtx->PeiMpServices,
              &CpuCount,
              &CpuCountEnabled
              );
  ASSERT_EFI_ERROR (Status);

  //
  // Get BSP index so we can get the processor information
  //
  LtPeiCtx->PeiMpServices->WhoAmI (LtPeiCtx->PeiServices, LtPeiCtx->PeiMpServices, &Index);

  //
  // Find out the bit shift so it can be used to identify APs on other sockets
  //
  AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, 1, &mNumOfBitShift, NULL, NULL, NULL);
  mNumOfBitShift &= 0x1F;
  DEBUG ((EFI_D_INFO, "\tmNumOfBitShift! %d\n", mNumOfBitShift));

  Status = LtPeiCtx->PeiMpServices->GetProcessorInfo (
              LtPeiCtx->PeiServices,
              LtPeiCtx->PeiMpServices,
              Index,
              &CpuInfoBuffer
              );
  ASSERT_EFI_ERROR (Status);

  //
  // Set the socket ID of the BSP to be used in identifying APs on other sockets
  //
  SocketID_BSP = (UINT8)(CpuInfoBuffer.ProcessorId)>>mNumOfBitShift;

  //
  // Loop through all threads to find all the sockets present
  //
  for (Index = 0; Index < CpuCount + 1; Index++) {
    Status = LtPeiCtx->PeiMpServices->GetProcessorInfo (
              LtPeiCtx->PeiServices,
              LtPeiCtx->PeiMpServices,
              Index,
              &CpuInfoBuffer
              );
    Temp_SocketID = (UINT8)(CpuInfoBuffer.ProcessorId)>>mNumOfBitShift;  //Get socket ID
    if (Temp_SocketID == SocketID_BSP) {
      continue;
    }
    for (Index1 = 0; Index1 < MAX_SOCKET_COUNT; Index1++) {
      if (SocketInfo[Index1].SocketFoundId == Temp_SocketID) {
        break;
      }
      //
      // Make sure this entry is unused
      //
      if (SocketInfo[Index1].SocketIndex == 0xFF) {
        //
        // Found an AP on another socket, store the index, Socket ID, and APIC ID of AP
        //
        SocketInfo[Index1].SocketFoundId = Temp_SocketID;
        SocketInfo[Index1].SocketIndex = (UINT8)Index;
        SocketInfo[Index1].ApApicId = (UINT16)CpuInfoBuffer.ProcessorId;
        break;
      }
    }
  }

  //
  // Send init to all APs so they are waiting for SIPI
  //
  SendInitIpiAllExcludingSelf ();

  //
  // Call BIOS ACM to lock config on BSP
  //
  DoLockConfig ((VOID *) LtPeiCtx);

  for (Index = 0; Index < MAX_SOCKET_COUNT; Index++) {
    if (SocketInfo[Index].SocketIndex == 0xFF) {
      break;
    }
    // Index = AP on other socket
    //
    // Call StartupThisAP to run DoLockConfig
    //
    DEBUG ((EFI_D_INFO, "\tDoLockConfigOnAP for Socket: %d AP: %d\n", SocketInfo[Index].SocketFoundId + 1, (UINTN)SocketInfo[Index].SocketIndex));
    Status = LtPeiCtx->PeiMpServices->StartupThisAP (
              LtPeiCtx->PeiServices,
              LtPeiCtx->PeiMpServices,
              DoLockConfigOnAP,
              (UINTN)SocketInfo[Index].SocketIndex,
              0,
              (VOID*)LtPeiCtx
              );
    //
    // Set the AP we just used back into wait for SIPI state
    //
    SendInitIpi ((UINT32)SocketInfo[Index].ApApicId);

    DEBUG ((EFI_D_INFO, "\tDone!\n"));
    if (EFI_ERROR (Status)) {
      break;
    }
  }
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return EFI_SUCCESS;
}

VOID
EFIAPI
DoLockConfigOnAP (
  IN      VOID      *Buffer
  )
/**

  Invokes the LOCK CONFIG function from the LT BIOS ACM for the AP that called.

  @param Buffer -      LtPeiCtx - A pointer to an initialized LT PEI Context data structure

  @retval EFI_SUCCESS   - Always.

**/
{
  LT_PEI_LIB_CONTEXT      *LtPeiCtx = (LT_PEI_LIB_CONTEXT*)Buffer;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  //
  // Set BSP flag for this AP before launching BiosAcm
  //
  AsmMsrBitFieldWrite64 (MSR_IA32_APIC_BASE, N_MSR_BSP_FLAG, N_MSR_BSP_FLAG, 1);

  if (LtPeiCtx->PlatformTxtPolicyData->StartupAcmAddress != 0) {
// BEGIN_OVERRIDE_HSD_5331848
   LtPeiLibLaunchBiosAcm (LtPeiCtx, LT_LOCK_CONFIG);
// END_OVERRIDE_HSD_5331848
  }

  //
  // Clear BSP flag for this AP after done launching BiosAcm
  //
  AsmMsrBitFieldWrite64 (MSR_IA32_APIC_BASE, N_MSR_BSP_FLAG, N_MSR_BSP_FLAG, 0);

  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));
  return;

}

/**

  Invokes the LOCK CONFIG function from the LT BIOS ACM.

  @param Buffer -      LtPeiCtx - A pointer to an initialized LT PEI Context data structure

  @retval EFI_SUCCESS   - Always.

**/
VOID
DoLockConfig (
  IN      VOID      *Buffer
  )
{

  LT_PEI_LIB_CONTEXT      *LtPeiCtx = (LT_PEI_LIB_CONTEXT*)Buffer;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  if (LtPeiCtx->PlatformTxtPolicyData->StartupAcmAddress != 0) {
    LtPeiLibLaunchBiosAcm (LtPeiCtx, LT_LOCK_CONFIG);
  }
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return;
}

/**
  Determines if SGX is enabled.  If enabled, bypass all MCi_CTL reads/writes in LtPeiLibLaunchBiosAcm
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

// BEGIN_OVERRIDE_HSD_5331848
/**

Sets up the system and then launches the LT BIOS ACM to run the function
requested by AcmFunction.

@param AcmFunction   - Constant that represents the function from the BIOS ACM
that should be executed.

@retval EFI_SUCCESS   - Always.

**/
STATIC
EFI_STATUS
LtPeiLibLaunchBiosAcm (
IN LT_PEI_LIB_CONTEXT       *LtPeiCtx,
IN UINT32                   AcmFunction
)
{
  UINT32                    Ia32_Mc9_Ctl_Org;
  UINT32                    Ia32_Mc10_Ctl_Org;
  UINT32                    Ia32_Mc11_Ctl_Org;
  UINT32                    Ia32_Mc9_Ctl_New;
  UINT32                    Ia32_Mc10_Ctl_New;
  UINT32                    Ia32_Mc11_Ctl_New;
  UINT32                    Ia32_Mcg_Contain_Org = 0;
  UINT32                    Ia32_Mcg_Contain_New = 0;
  BOOLEAN                   SgxStatus;

  DEBUG ((EFI_D_INFO, "[TXT] %a: Entry\n",__FUNCTION__));
  SgxStatus = IsSgxEnabled ();
  if (SgxStatus == FALSE) {
    if ((AsmReadMsr32 (MSR_IA32_MCG_CAP) & BIT24) != 0) {
      //  If IA32_MCG_CAP.MCG_SER_P is equal to 1, write a value of 0 to MCG_CONTAIN
      Ia32_Mcg_Contain_Org = AsmReadMsr32 (MSR_MCG_CONTAIN);
      AsmWriteMsr32 (MSR_MCG_CONTAIN, Ia32_Mcg_Contain_New);
    }

    Ia32_Mc9_Ctl_Org  = AsmReadMsr32 (MSR_IA32_MC9_CTL);
    Ia32_Mc10_Ctl_Org = AsmReadMsr32 (MSR_IA32_MC10_CTL);
    Ia32_Mc11_Ctl_Org = AsmReadMsr32 (MSR_IA32_MC11_CTL);

    // Clear BIT7 SAD_ERR_NON_CORRUPTING_OTHER
    Ia32_Mc9_Ctl_New  = Ia32_Mc9_Ctl_Org & ~BIT7;
    Ia32_Mc10_Ctl_New = Ia32_Mc10_Ctl_Org & ~BIT7;
    Ia32_Mc11_Ctl_New = Ia32_Mc11_Ctl_Org & ~BIT7;

    AsmWriteMsr32 (MSR_IA32_MC9_CTL,  Ia32_Mc9_Ctl_New);
    AsmWriteMsr32 (MSR_IA32_MC10_CTL, Ia32_Mc10_Ctl_New);
    AsmWriteMsr32 (MSR_IA32_MC11_CTL, Ia32_Mc11_Ctl_New);
    DEBUG ((EFI_D_INFO, "\tMSR_IA32_MC9_CTL[0x%08x]  = 0x%08x\n", MSR_IA32_MC9_CTL,  AsmReadMsr32 (MSR_IA32_MC9_CTL)));
    DEBUG ((EFI_D_INFO, "\tMSR_IA32_MC10_CTL[0x%08x] = 0x%08x\n", MSR_IA32_MC10_CTL, AsmReadMsr32 (MSR_IA32_MC10_CTL)));
    DEBUG ((EFI_D_INFO, "\tMSR_IA32_MC11_CTL[0x%08x] = 0x%08x\n", MSR_IA32_MC11_CTL, AsmReadMsr32 (MSR_IA32_MC11_CTL)));
  }
    //
    // Launch the BIOS ACM to run the requested function
    //
    DEBUG ((EFI_D_INFO, "\tBiosAcmAddress = 0x%08x\tAcmFunction = 0x%08x\n", (UINTN)LtPeiCtx->PlatformTxtPolicyData->BiosAcmAddress, AcmFunction));
    DEBUG ((EFI_D_INFO, "\tLaunchBiosAcm (0x%08x)\n", (UINTN)AcmFunction));
    LaunchBiosAcm ((UINTN)LtPeiCtx->PlatformTxtPolicyData->BiosAcmAddress, (UINTN)AcmFunction);

  if (SgxStatus == FALSE) {
    if ((AsmReadMsr32 (MSR_IA32_MCG_CAP) & BIT24) != 0) {
      // If IA32_MCG_CAP.MCG_SER_P equal to 1, restore MCG_CONTAIN to its previous value
      AsmWriteMsr32 (MSR_MCG_CONTAIN, Ia32_Mcg_Contain_Org);
      DEBUG ((EFI_D_INFO, "\tMSR_MCG_CONTAIN[0x%08x] = 0x%08x\n", MSR_MCG_CONTAIN, AsmReadMsr32(MSR_MCG_CONTAIN)));
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
  DEBUG ((EFI_D_INFO, "[TXT] %a: Exit\n",__FUNCTION__));

  return EFI_SUCCESS;
}
// END_OVERRIDE_HSD_5331848
