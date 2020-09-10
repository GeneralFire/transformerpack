/** @file
  MeUmaPostMem.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2010 - 2020 Intel Corporation. <BR>

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

///
/// Statements that include other files
///
#include <Base.h>
#include <PiPei.h>
#include <Uefi.h>
#include "Library/MmPciBaseLib.h"
#include "MeAccess.h"
#include <Register/HeciRegs.h>
#include "IndustryStandard/Pci22.h"
#include "Ppi/MeUmaPpi.h"
#include "Library/DebugLib.h"
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <PchAccess.h>
#include <Register/PchRegsPmc.h>
#include <Library/PeiPchPcieLib.h>
#include "Library/PerformanceLib.h"
//
#include "Ppi/Wdt.h"
#include "Library/IoLib.h"
#include "Library/MeTypeLib.h"
#include <Library/PreSiliconEnvDetectLib.h>
#include <BupMsgs.h>
#include <Library/PeiMeLib.h>
#include <Library/PmcLib.h>
#include <PchResetPlatformSpecific.h>
#include <Protocol/MePlatformGetResetType.h>
#include <ConfigBlock.h>
#include <Private/Library/PmcPrivateLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <UncoreCommonIncludes.h>
#include <ConfigBlock/MeGetConfigBlock.h>
#include <Library/MeUefiFwHealthStatusLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemoryCoreLib.h>
#include <Ppi/MemoryDiscovered.h>
#include <Library/HobLib.h>
#include <MeUmaCommon.h>

/*****************************************************************************
 * Local definitions
 */
CONST CHAR8  *DebugMeUmaPrefix = "ME UMA PostMem:";

/*****************************************************************************
 * Local prototypes
 */
EFI_STATUS
SendDramInitDone (
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  );

/*****************************************************************************
 * Global variables
 */
EFI_PEI_PPI_DESCRIPTOR mMeBeforeDidSendPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gMeBeforeDidSentPpiGuid,
  NULL
};

extern EFI_GUID gMeReadyForSendingDidPpiGuid;

// Notification MeUma callback about gHeciControlPpiGuid called on gEfiPeiMemoryDiscoveredPpiGuid
static EFI_PEI_NOTIFY_DESCRIPTOR mMeUmaNotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK),
    &gEfiPeiMemoryDiscoveredPpiGuid,
    SendDramInitDone
  },
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK),
    &gHeciControlPpiGuid,
    SendDramInitDone
  },
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gMeReadyForSendingDidPpiGuid,
    SendDramInitDone
  }
};

// After sending DramInitDone to ME
// MeUma will notify others with gDramInitDoneSentPpiGuid
static EFI_PEI_PPI_DESCRIPTOR mMemoryInitDoneSentSignalPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gDramInitDoneSentPpiGuid,
  NULL
};

/**
  For non SKX/CLX/CPX family correct memory address format

  @param[in, out] MeNcMemLowBaseAddr           Low part of UMA base address
  @param[in, out] MeNcMemHighBaseAddr          High part of UMA base address
  @param[in, out] MeNcMemLowLimit              Low part of UMA base address limit
  @param[in, out] MeNcMemHighLimit             High part of UMA base address limit
**/
VOID
CorrectImrParametersMe (
  IN OUT UINT32 *MeNcMemLowBaseAddr,
  IN OUT UINT32 *MeNcMemHighBaseAddr,
  IN OUT UINT32 *MeNcMemLowLimit,
  IN OUT UINT32 *MeNcMemHighLimit
)
{
  EFI_PHYSICAL_ADDRESS  Address;
  UINT64                Limit;

  if (IsCpuAndRevision (CPU_SKX, REV_ALL) ||
      IsCpuAndRevision (CPU_CLX, REV_ALL) ||
      IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    return;
  }

  ASSERT (MeNcMemLowBaseAddr != NULL);
  ASSERT (MeNcMemHighBaseAddr != NULL);
  if ((MeNcMemLowBaseAddr == NULL) || (MeNcMemHighBaseAddr == NULL)) {
    DEBUG ((EFI_D_ERROR, "%a Received NULL arguments\n", DebugMeUmaPrefix));
    return;
  }

  Address = LShiftU64 ((UINT64)*MeNcMemHighBaseAddr, 32) + *MeNcMemLowBaseAddr;
  Address &= 0xFFFFFFFFFF; // Get 40 bits only
  Address = LShiftU64 (Address, 12);
  *MeNcMemHighBaseAddr = (UINT32)RShiftU64 (Address, 32);
  *MeNcMemLowBaseAddr = (UINT32)Address;

  if ((MeNcMemLowLimit != NULL) && (MeNcMemHighLimit != NULL)) {
    Limit = LShiftU64 ((UINT64)*MeNcMemHighLimit, 32) + *MeNcMemLowLimit;
    Limit = LShiftU64 (Limit, 12 + 12); // Move 12 bits and more 12 bit to omit high bits
    Limit = ~Limit;
    Limit = RShiftU64 (Limit, 12); // return with 12 bit shift -> high bits are 0
    Limit = Limit + Address;
    *MeNcMemHighLimit = (UINT32)RShiftU64 (Limit, 32);
    *MeNcMemLowLimit = (UINT32)Limit;
  }
}

/**
  Return MeUmaSizeError field from ME_UMA_PPI

  @retval 0        No error
  @retval 1 - 255  InitStart value for error indication
**/
UINT8
GetMeUmaSizeError (
  VOID
 )
{
  ME_UMA_INFO *MeUmaInfo;
  UINT8       MeUmaSizeError;

  MeUmaSizeError = ME_MEMORY_INITIALIZATION_ERROR;
  MeUmaInfo = MeUmaGetInfo ();
  if (MeUmaInfo != NULL) {
    MeUmaSizeError = MeUmaInfo->MeUmaSizeError;
  }

  return MeUmaSizeError;
}

/**
  This procedure will configure the ME Host General Status register,
  indicating that DRAM Initialization is complete and ME FW may
  begin using the allocated ME UMA space.

  @param[in]  FfsHeader            Pointer to the FFS file header
  @param[in]  InitStat             H_GS[27:24] Status
  @param[in]  MeNcMemLowBaseAddr   Low part of UMA base address
  @param[in]  MeNcMemHighBaseAddr  High part of UMA base address
  @param[out] nextStep             Returns information about required next action

  @retval EFI_SUCCESS
**/
EFI_STATUS
MeConfigDidReg (
  IN EFI_FFS_FILE_HEADER  *FfsHeader,
  IN UINT8                 InitStat,
  IN UINT32                MeNcMemLowBaseAddr,
  IN UINT32                MeNcMemHighBaseAddr,
  OUT UINT8                *nextStep
  )
{
  EFI_STATUS                  Status;
  UINT8                       Flags = 0;
  UINT8                       MkhiReturnValue = 0;
  UINT32                      PciImrBaseLow = 0;
  UINT32                      PciImrBaseHigh = 0;
  UINT8                       MeUmaSizeError;
#if ME_TESTMENU_FLAG
  ME_PEI_PREMEM_CONFIG        *MePreMemConfig = GetMePreMemConfigBlock (NULL);
#endif // ME_TESTMENU_FLAG

  *nextStep = CBM_DIR_DO_NOTHING;

  if (!MeUmaInternalIsEnabled (DebugMeUmaPrefix)) {
    DEBUG ((DEBUG_INFO, "%a ME UMA feature disabled (%a()).\n", DebugMeUmaPrefix, __FUNCTION__));
    Status = EFI_SUCCESS;
    goto MemoryInitDoneSentNotify;
  }

  if (!MeTypeIsDfx ()) {
    HECI_FWS_REGISTER MeHfs;

    if (MeTypeIsDisabled ()) {
      DEBUG ((DEBUG_INFO, "%a ME debug mode, do not check for ME UMA.\n", DebugMeUmaPrefix));
      Status = EFI_SUCCESS;
      goto MemoryInitDoneSentNotify;
    }

    MeHfs.ul  = HeciPciRead32 (R_ME_HFS);
    if (MeHfs.r.ErrorCode != ME_ERROR_CODE_NO_ERROR) {
      DEBUG ((DEBUG_INFO, "%a ME error, do not check for ME UMA.\n", DebugMeUmaPrefix));
      Status = EFI_SUCCESS;
      goto MemoryInitDoneSentNotify;
    }
  }

  DEBUG ((DEBUG_INFO, "%a Entered ME DRAM Init Done procedure.\n", DebugMeUmaPrefix));

  CorrectImrParametersMe (
    &MeNcMemLowBaseAddr,
    &MeNcMemHighBaseAddr,
    NULL,
    NULL
    );

  ///
  /// Read MESEGBASE value
  ///
  DEBUG ((DEBUG_INFO, "%a MeUmaBase read: %x\n", DebugMeUmaPrefix, MeNcMemLowBaseAddr));
  InitStat &= 0x7F;

  ///
  /// Status
  ///   0x0 = Success
  ///   0x1 = No Memory in channels
  ///   0x2 = Memory Init Error
  ///   0x3 = Memory not preserved across reset
  ///   0x4-0xF = Reserved
  ///
  ///
  /// If TestDidInitStat is not set to "Auto" (0), provide the TestDidInitStat
  /// value specified in the setup option to ME FW.
  ///
  MeUmaSizeError = GetMeUmaSizeError ();
  if (MeUmaSizeError != ME_MEMORY_INITIALIZATION_SUCCESSS) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Override InitStat due to MeUmaSize Error: %x\n", DebugMeUmaPrefix, MeUmaSizeError));
    InitStat = MeUmaSizeError;
  }
#if ME_TESTMENU_FLAG
  if ((MePreMemConfig != NULL) && (MePreMemConfig->DidInitStat > 0)) {
    InitStat = (UINT8) MePreMemConfig->DidInitStat - 1;
  }
#endif // ME_TESTMENU_FLAG
  DEBUG ((DEBUG_INFO, "%a InitStat: %x\n", DebugMeUmaPrefix, InitStat));

  Status = PeiServicesInstallPpi (&mMeBeforeDidSendPpi);
  ASSERT_EFI_ERROR (Status);

  PERF_START (FfsHeader, "DID", NULL, AsmReadTsc ());
  //
  // Send DID using HECI message
  //
  DEBUG ((DEBUG_INFO, " Sending DID as MKHI message\n"));
  Status = PeiHeciSendDid (
    MeNcMemLowBaseAddr,
    MeNcMemHighBaseAddr,
    InitStat,
    &MkhiReturnValue,
    &Flags,
    nextStep,
    &PciImrBaseLow,
    &PciImrBaseHigh
  );

  SetMeUefiFwHealthEfiStatus (DidSendingStatus, Status);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Sending DID failed! Error: %r\n", Status));
  } else {
    HECI_FWS_REGISTER MeHfs;

    DEBUG ((DEBUG_INFO, "%a ME DRAM Init Done ACK received.\n", DebugMeUmaPrefix));
    MeHfs.ul  = HeciPciRead32 (R_ME_HFS);
    DEBUG ((DEBUG_INFO, "%a HFS read after DID ACK: %x\n", DebugMeUmaPrefix, MeHfs.r));
    BuildPciImrHob (PciImrBaseLow, PciImrBaseHigh);
  }

  PERF_END (FfsHeader, "DID", NULL, AsmReadTsc ());

  DEBUG ((DEBUG_ERROR, "%a BiosAction = %x\n", DebugMeUmaPrefix, *nextStep));

MemoryInitDoneSentNotify:

  if (EFI_ERROR (Status) || ((InitStat & 0x3) == 0x1) || ((InitStat & 0x3) == 0x2)) {
    ///
    /// Error when sending DID or Memory Initialization was not successful,
    /// clear reserved ME UMA size info in the ME UMA PPI
    ///
    Status = MeUmaUpdatePpiMeUmaSizeInfo (DebugMeUmaPrefix, 0, ME_MEMORY_INITIALIZATION_ERROR);
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

/**
  Entry point for ME UMA Post-Mem.

  @param[in] FfsHeader            Pointer to the FFS file header
  @param[in] PeiServices          Pointer to the PEI services table

  @retval Return Status based on errors that occurred while waiting for time to expire.
**/
EFI_STATUS
EFIAPI
MeUmaPostMemEntry (
  IN EFI_PEI_FILE_HANDLE      FileHandle,
  IN CONST EFI_PEI_SERVICES   **PeiServices
  )
{
  EFI_STATUS Status;

  DEBUG ((DEBUG_INFO, "%a ME UMA PPI Driver EntryPoint\n", DebugMeUmaPrefix));

  Status = PeiServicesNotifyPpi (&mMeUmaNotifyList[0]);
  DEBUG ((DEBUG_INFO, "%a DramInitDone callback installation status %r\n", DebugMeUmaPrefix, Status));
  ASSERT_EFI_ERROR (Status);
  Status = EFI_SUCCESS;

  return Status;
}

#if TESTMENU_FLAG || ME_TESTMENU_FLAG
/**
  This function tests UMA location stored in MRC against read from register.

  @param[in] MeNcMemLowBaseAddr           Low part of UMA base address
  @param[in] MeNcMemHighBaseAddr          High part of UMA base address
  @param[in] MeNcMemLowLimit              Low part of UMA base address limit
  @param[in] MeNcMemHighLimit             High part of UMA base address limit

  @retval Return Status based on result compare.
**/
EFI_STATUS
EFIAPI
TestUmaLocation (
  IN UINT32 MeNcMemLowBaseAddr,
  IN UINT32 MeNcMemHighBaseAddr,
  IN UINT32 MeNcMemLowLimit,
  IN UINT32 MeNcMemHighLimit
  )
{
  EFI_STATUS   Status = EFI_SUCCESS;
  UINT32       MeUmaSize;
  UINT32       MeUmaSizeM;

  UINT64       MeNcMemLowLimit2;

  if (!MeUmaInternalIsEnabled (DebugMeUmaPrefix)) {
    return Status;
  }

  CorrectImrParametersMe (
    &MeNcMemLowBaseAddr,
    &MeNcMemHighBaseAddr,
    &MeNcMemLowLimit,
    &MeNcMemHighLimit
    );

  MeNcMemLowLimit2 = (((MeNcMemLowLimit >> 19)+1) << 19);

  DEBUG ((DEBUG_INFO, "%a ====================================================\n", DebugMeUmaPrefix));
  DEBUG ((DEBUG_INFO, "%a Stored UMA Location:\n", DebugMeUmaPrefix));
  DEBUG ((DEBUG_INFO, "%a MeNcMemLowBaseAddr  = 0x%x\n", DebugMeUmaPrefix, MeNcMemLowBaseAddr));
  DEBUG ((DEBUG_INFO, "%a MeNcMemHighBaseAddr = 0x%x\n", DebugMeUmaPrefix, MeNcMemHighBaseAddr));
  DEBUG ((DEBUG_INFO, "%a MeNcMemLowLimit     = 0x%x (->0x%x)\n", DebugMeUmaPrefix, MeNcMemLowLimit, MeNcMemLowLimit2));
  DEBUG ((DEBUG_INFO, "%a MeNcMemHighLimit    = 0x%x\n", DebugMeUmaPrefix, MeNcMemHighLimit));
  MeUmaSize = (UINT32)((LShiftU64(MeNcMemHighLimit, 32) + (UINT64)MeNcMemLowLimit2) -
                      (LShiftU64(MeNcMemHighBaseAddr, 32) + (UINT64)MeNcMemLowBaseAddr));
  MeUmaSizeM = MeUmaSize/1024/1024;
  DEBUG ((DEBUG_INFO, "%a MeUmaSize           = 0x%x (%dM)\n", DebugMeUmaPrefix, MeUmaSize, MeUmaSizeM));

  if ((MeNcMemLowBaseAddr == 0) || (MeNcMemLowLimit == 0)) {
    if (MeUmaSize > 0) {
      DEBUG ((DEBUG_WARN, "%a WARNING: Lower part of UMA addresses is 0.\n", DebugMeUmaPrefix));
    }
  } else {
    if ((MeUmaSizeM > ME_UMA_SIZE_UPPER_LIMIT) && (MeUmaSizeM > 0)) {
      DEBUG ((DEBUG_ERROR, "%a ERROR: UMA size error #01.\n", DebugMeUmaPrefix));
      Status = EFI_DEVICE_ERROR;
    } else if (((MeUmaSize/2)*2) != MeUmaSize) {
      // UMA size has to be n*2
      DEBUG ((DEBUG_ERROR, "%a ERROR: UMA size error #02.\n", DebugMeUmaPrefix));
      Status = EFI_DEVICE_ERROR;
    }
  }
  DEBUG ((DEBUG_INFO, "%a ====================================================\n", DebugMeUmaPrefix));

  return Status;
}
#endif // TESTMENU_FLAG || ME_TESTMENU_FLAG

/**
  Check if all Guids specified in notification list mMeUmaNotifyList are present

  @retval TRUE             All Guids are in place
  @retval FALSE            Not all Guids are in place
**/
BOOLEAN
CheckAllNotifiedGuids (
  VOID
  )
{
  UINTN      Index;
  UINTN      GuidCounter;
  UINTN      GuidCountExpected;
  VOID       *TestPpi;
  EFI_STATUS Status;

  GuidCounter = 0;
  GuidCountExpected = sizeof (mMeUmaNotifyList) /  sizeof (EFI_PEI_NOTIFY_DESCRIPTOR);
  for (Index = 0; Index < GuidCountExpected; Index++) {
    Status = PeiServicesLocatePpi (mMeUmaNotifyList[Index].Guid, 0, NULL, &TestPpi);
    if (!EFI_ERROR (Status)) {
      GuidCounter++;
    }
  }
  if (GuidCounter == GuidCountExpected) {
    return TRUE;
  }

  return FALSE;
}

/**
  Function sends DRAM_INIT_DONE, called on notification

  @param[in] PeiServices      Pointer to the PEI services table
  @param NotifyDescriptor     Information about the notify event
  @param Ppi                  Context

  @retval EFI_SUCCESS             Always success
**/
EFI_STATUS
SendDramInitDone (
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  )
{
  UINT8                         nextStep;
  VOID                          *TestPpi;
  ME_UMA_INFO                   *MeUmaInfo;
  EFI_STATUS                    Status;

  // Check if all required Guids are in place
  if (!CheckAllNotifiedGuids ()) {
    DEBUG ((DEBUG_WARN, "%a Warning: %a (): Not all required guids are present\n", DebugMeUmaPrefix, __FUNCTION__));
    // Return without notification
    return EFI_SUCCESS;
  }

  Status = PeiServicesLocatePpi (&gDramInitDoneSentPpiGuid, 0, NULL, &TestPpi);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a Warning: %a (): Multiple call\n", DebugMeUmaPrefix, __FUNCTION__));
    // Return without notification
    return EFI_SUCCESS;
  }

  MeUmaInfo = MeUmaGetInfo ();
  if (MeUmaInfo == NULL) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: %a (): Can't locate MeUmaInfo\n", DebugMeUmaPrefix, __FUNCTION__));
    ASSERT_EFI_ERROR (Status);
    goto SendDramInitDoneExit;
  }

  DEBUG ((DEBUG_ERROR, "%a %a (): InitStat = %d\n", DebugMeUmaPrefix, __FUNCTION__, MeUmaInfo->InitStat));
  if (MeUmaInfo->InitStat == MAX_UINT8) {
    goto SendDramInitDoneExit;
  }

#if TESTMENU_FLAG || ME_TESTMENU_FLAG
  Status = TestUmaLocation (
             MeUmaInfo->MeNcMemLowBaseAddr,
             MeUmaInfo->MeNcMemHighBaseAddr,
             MeUmaInfo->MeNcMemLowLimit,
             MeUmaInfo->MeNcMemHighLimit
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Errors in UMA configuration\n", DebugMeUmaPrefix));
    ASSERT_EFI_ERROR (Status);
  }
#endif // TESTMENU_FLAG || ME_TESTMENU_FLAG

  nextStep = CBM_DIR_DO_NOTHING;
  Status = MeConfigDidReg (
             NULL,
             MeUmaInfo->InitStat,
             MeUmaInfo->MeNcMemLowBaseAddr,
             MeUmaInfo->MeNcMemHighBaseAddr,
             &nextStep
             );
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a MeDramInitDone Complete. Checking for reset...\n", DebugMeUmaPrefix));
    if (nextStep == CBM_DIR_NON_PCR) {
      // for this reset use MRC reset mechanism
      SetSysResetRequired (POST_RESET_POWERGOOD);
    } else {
      Status = MeUmaInternalHandleBiosAction (DebugMeUmaPrefix, nextStep);
    }
  } else {
    DEBUG ((DEBUG_ERROR, "%a ERROR: MeDramInitDone FAILED (%r). Checking for reset...\n", DebugMeUmaPrefix, Status));
  }

SendDramInitDoneExit:
  //
  // Send gDramInitDoneSentPpiGuid notification signal
  //
  DEBUG ((DEBUG_INFO, "%a Sending MemoryInitDoneSent Notification ...\n", DebugMeUmaPrefix));
  Status = PeiServicesInstallPpi (&mMemoryInitDoneSentSignalPpi);
  DEBUG ((DEBUG_INFO, "%a Sent MemoryInitDoneSent Notification (%r)\n", DebugMeUmaPrefix, Status));
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
