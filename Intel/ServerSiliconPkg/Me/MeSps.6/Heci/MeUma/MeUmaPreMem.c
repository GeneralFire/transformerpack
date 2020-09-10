/** @file
  MeUmaPreMem.c

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
#include <Library/BaseMemoryLib.h>
#include <PchAccess.h>
#include <Register/PchRegsPmc.h>
#include <Library/PeiPchPcieLib.h>
#include "Library/PerformanceLib.h"
//
#include "Ppi/Wdt.h"
#include "Library/IoLib.h"
#include "Library/MeTypeLib.h"
#include <BupMsgs.h>
#include <Library/PeiMeLib.h>
#include <Library/PmcLib.h>
#include <PchResetPlatformSpecific.h>
#include <Protocol/MePlatformGetResetType.h>
#include <ConfigBlock.h>
#include <Private/Library/PmcPrivateLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <UncoreCommonIncludes.h>
#include <ConfigBlock/MeGetConfigBlock.h>
#include <Library/MeUefiFwHealthStatusLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/HobLib.h>
#include <MeUmaCommon.h>

/*****************************************************************************
 * Local definitions
 */
CONST CHAR8  *DebugMeUmaPrefix = "ME UMA PreMem:";

/*****************************************************************************
 * Local prototypes
 */
BOOLEAN
EFIAPI
MeUmaIsEnabled (
  VOID
  );

UINT32
EFIAPI
MeUmaGetSize (
  IN EFI_FFS_FILE_HEADER  *FfsHeader,
  OUT UINT32              *Alignment
  );

EFI_STATUS
EFIAPI
MeUmaSaveDramInitDoneParameters (
  IN UINT32                MrcStatus,
  IN UINT32                MemSize,
  IN UINT8                 PreviousBootError,
  IN UINT8                 SubBootMode,
  IN UINT32                MeNcMemLowBaseAddr,
  IN UINT32                MeNcMemHighBaseAddr,
  IN UINT32                MeNcMemLowLimit,
  IN UINT32                MeNcMemHighLimit
  );

EFI_STATUS
EFIAPI
MeUmaCpuReplacementCheck (
  IN EFI_FFS_FILE_HEADER  *FfsHeader,
  UINT8                   *ForceFullTraining,
  UINT8                   *nextStep
  );

EFI_STATUS
EFIAPI
MeUmaSendHostResetWarning (
  OUT UINT8 *pAction
  );

/*****************************************************************************
 * Global variables
 */
// MeUma PPI implementation
static ME_UMA_PPI mMeUmaPpi = {
  MeUmaIsEnabled,
  MeUmaGetSize,
  MeUmaSaveDramInitDoneParameters,
  MeUmaCpuReplacementCheck,
  MeUmaSendHostResetWarning,
  MeUmaHandleBiosAction,
  MeUmaGetInfo
};

static EFI_PEI_PPI_DESCRIPTOR mMeUmaPpiList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gMeUmaPpiGuid,
    &mMeUmaPpi
  }
};

/**
  This function reads configurations for UMA.

  @retval Return Status based on errors that occurred while waiting for time to expire.
**/
BOOLEAN
EFIAPI
MeUmaIsEnabled (
  VOID
  )
{
  return MeUmaInternalIsEnabled (DebugMeUmaPrefix);
}

/**
  This procedure will send the information to CSME about the IMRs which need to be
  enabled and retrieve the information about total size of memory and alignment
  required for IMRs.

  @param[in]  FfsHeader           Pointer to the FFS file header
  @param[out] Alignment           Pointer to required address alignment

  @retval UINT32                  Return ME UMA Size
  @retval 0                       Do not check for ME UMA
**/
UINT32
EFIAPI
MeUmaGetSize (
  IN EFI_FFS_FILE_HEADER  *FfsHeader,
  OUT UINT32              *Alignment
  )
{
  UINT8             MeReturnValue;
  UINT32            ImrsSize = 0;
  UINT32            PcieImrSize = 0;
  UINT32            BiosImrDisableMask0;
  UINT32            BiosImrDisableMask1;
  EFI_STATUS        Status;
  HECI_FWS_REGISTER MeHfs;
  UINT32            AlignmentValue;
  UINT8             MeUmaSizeError;

  if (Alignment == NULL) {
    Alignment = &AlignmentValue;
  }
  *Alignment = 1;

  if (!MeUmaInternalIsEnabled (DebugMeUmaPrefix)) {
    DEBUG ((DEBUG_INFO, "%a ME UMA feature disabled (%a()).\n", DebugMeUmaPrefix, __FUNCTION__));
    return 0;
  }

  MeHfs.ul = HeciPciRead32 (R_ME_HFS);
  if (!MeTypeIsDfx ()) {
    if (MeTypeIsDisabled ()) {
      DEBUG ((DEBUG_INFO, "%a ME debug mode, do not check for ME UMA.\n", DebugMeUmaPrefix));
      return 0;
    }

    if (MeHfs.r.ErrorCode != ME_ERROR_CODE_NO_ERROR) {
      DEBUG ((DEBUG_INFO, "%a ME error, do not check for ME UMA.\n", DebugMeUmaPrefix));
      return 0;
    }
  }

  MeUmaSizeError = ME_MEMORY_INITIALIZATION_SUCCESSS;
  MeReturnValue = MkhiStatusGeneralError;
  BiosImrDisableMask1 = 0xFFFFFFFF;
  BiosImrDisableMask0 = 0xFFFFFFFF;
  if (MeHfs.r.MeOperationMode != ME_OPERATION_MODE_DEBUG) {
    BiosImrDisableMask0 &= (UINT32)~(BIT5);  // CSME
    DEBUG ((DEBUG_INFO, "%a CSME Imr is requested\n", DebugMeUmaPrefix));
  }
  if (PchIsPcieImrEnabled ()) {
    BiosImrDisableMask0 &= (UINT32)~(BIT7);  // PCIe
    PcieImrSize = PchGetPcieImrSize ();
  }
  DEBUG ((DEBUG_INFO, "%a ME UMA BiosImrDisableMask0 = %x\n", DebugMeUmaPrefix, BiosImrDisableMask0));
  DEBUG ((DEBUG_INFO, "%a ME UMA PCIe IMR Size       = %x\n", DebugMeUmaPrefix, PcieImrSize));

  PERF_START (FfsHeader, "MUSZV", NULL, AsmReadTsc ());
  Status = PeiHeciGetImrSizeMsg (
    BiosImrDisableMask0,
    BiosImrDisableMask1,
    (UINT32)LShiftU64(PcieImrSize, 20), // PCI IMR Size in bytes
    &MeReturnValue,
    &ImrsSize,                          // ImrsSize in bytes
    Alignment                           // Alignment in bytes
    );

  PERF_END (FfsHeader, "MUSZV", NULL, AsmReadTsc ());
  if (EFI_ERROR (Status) || (MeReturnValue != MkhiStatusSuccess)) {
    DEBUG ((DEBUG_INFO, "%a ERROR: PeiHeciGetImrSizeMsg() return status %r,"\
      "with MeReturnValue = %d\n", DebugMeUmaPrefix, Status, MeReturnValue));
    ImrsSize = 0;
    *Alignment = 0;
    MeUmaSizeError = ME_MEMORY_INITIALIZATION_NO_MEMORY_IN_CHANNELS;
    SetMeUefiFwHealthStatus (MeUmaSizeInfoReceivingStatus, StatusError);
  } else {
    SetMeUefiFwHealthStatus (MeUmaSizeInfoReceivingStatus, StatusOk);
  }
  ImrsSize = ImrsSize >> 20;       // Value in MB
  *Alignment = (*Alignment) >> 20; // Value in MB

  if (ImrsSize > ME_UMA_SIZE_UPPER_LIMIT) {
    DEBUG ((
      DEBUG_ERROR,
      "%a ERROR: ME UMA Size Requested: %x is too big,"
      "Limiting to %x\n",
      DebugMeUmaPrefix,
      ImrsSize,
      ME_UMA_SIZE_UPPER_LIMIT
      ));
    ImrsSize = ME_UMA_SIZE_UPPER_LIMIT;
    ASSERT (FALSE);
    // If ASSERT () is disabled we should set error in DramInitDone
    MeUmaSizeError = ME_MEMORY_INITIALIZATION_ERROR; // value for setting in InitStart: 0x2 = Memory Init Error
    DEBUG ((
      DEBUG_ERROR,
      "%a ERROR: Setting ME UMA Size Error: %x to be sent in DramInitDone\n",
      DebugMeUmaPrefix,
      MeUmaSizeError
      ));
  }

  DEBUG ((DEBUG_INFO, "%a ME UMA Size Requested: %x\n", DebugMeUmaPrefix, ImrsSize));
  DEBUG ((DEBUG_INFO, "%a ME UMA Alignment: %x\n", DebugMeUmaPrefix, *Alignment));

  ///
  /// Update ME UMA info with requested ME UMA size
  ///
  Status = MeUmaUpdatePpiMeUmaSizeInfo (DebugMeUmaPrefix, ImrsSize, MeUmaSizeError);
  ASSERT_EFI_ERROR (Status);

  return ImrsSize;
}

/**
  This procedure will determine whether or not the CPU was replaced
  during system power loss or via dynamic fusing.
  Calling this procedure could result in a warm reset (if ME FW is requesting one).

  @param[in] FfsHeader            Not used.
  @param[in] ForceFullTraining    When set = 0x1, MRC will be forced to perform
                                  a full memory training cycle.
  @param[in] nextStep             returns information about required next action

  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
EFIAPI
MeUmaCpuReplacementCheck (
  IN EFI_FFS_FILE_HEADER *FfsHeader,
  UINT8                  *ForceFullTraining,
  UINT8                  *nextStep
  )
{
  return EFI_UNSUPPORTED;
}

/**
  This procedure will Host Reset Warning message over HECI to ME.
  Host Reset Warning message is used in server BIOS to warn ME about
  a reset of Host partition done on Sx resume path that will not break
  the Sx resume. I.e. after reset Host continues the same Sx flow,
  so ME should ignore this reset and sustaing previews flow.

  @param[out] pAction   optional pointer to buffer for action requested by ME

  @retval EFI_SUCCESS
 **/
EFI_STATUS
MeUmaSendHostResetWarning (
    OUT UINT8                  *pAction
  )
{
  DEBUG ((DEBUG_INFO, "%a %a skips message for EBG\n", DebugMeUmaPrefix, __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  Creates the ME UMA Info HOB

  @retval EFI_SUCCESS           The ME UMA Info HOB has been created
  @retval EFI_OUT_OF_RESOURCES  Insufficient resources for HOB creation
**/
EFI_STATUS
CreateMeUmaInfoHob (
  VOID
  )
{
  EFI_STATUS            Status;
  ME_UMA_INFO           MeUmaInfo;
  ME_UMA_INFO           *ExistingMeUmaInfoHob;
  EFI_HOB_GUID_TYPE     *GuidHobPointer;

  Status = EFI_OUT_OF_RESOURCES;
  ExistingMeUmaInfoHob = NULL;
  GuidHobPointer = GetFirstGuidHob (&gMeUmaInfoHobGuid);
  if (GuidHobPointer == NULL) {
    MeUmaInfo.MeUmaSize = 0;
    MeUmaInfo.MeUmaSizeError = ME_MEMORY_INITIALIZATION_SUCCESSS;
    MeUmaInfo.InitStat = MAX_UINT8;
    BuildGuidDataHob (&gMeUmaInfoHobGuid, &MeUmaInfo, sizeof (ME_UMA_INFO));
    GuidHobPointer = GetFirstGuidHob (&gMeUmaInfoHobGuid);
    if (GuidHobPointer != NULL) {
      ExistingMeUmaInfoHob = GET_GUID_HOB_DATA (GuidHobPointer);
    }
  } else {
    ExistingMeUmaInfoHob = GET_GUID_HOB_DATA (GuidHobPointer);
    if (ExistingMeUmaInfoHob != NULL) {
      DEBUG ((DEBUG_WARN, "%a Warning: Duplicate HOB creation\n", DebugMeUmaPrefix));
    }
  }
  if (ExistingMeUmaInfoHob != NULL) {
    Status = EFI_SUCCESS;
  }

  return Status;
}

/**
  Save MeUma parameters used to send DramInitDone message on MemoryDiscovered signal

  @param [in] MrcStatus           MRC status for checking if MRC returns SUCCESS status
  @param [in] MemSize             Size of system memory discovered by MRC
  @param [in] PreviousBootError   Previous boot error
  @param [in] SubBootMode         Sub boot mode
  @param [in] MeNcMemLowBaseAddr  ME UMA low part of base address
  @param [in] MeNcMemHighBaseAddr ME UMA high part of base address
  @param [in] MeNcMemLowLimit     ME UMA low address part of area limit
  @param [in] MeNcMemHighLimit    ME UMA high address part of area limit

  @retval EFI_SUCCESS             Parameters successfully saved
**/
EFI_STATUS
MeUmaSaveDramInitDoneParameters (
  IN UINT32                MrcStatus,
  IN UINT32                MemSize,
  IN UINT8                 PreviousBootError,
  IN UINT8                 SubBootMode,
  IN UINT32                MeNcMemLowBaseAddr,
  IN UINT32                MeNcMemHighBaseAddr,
  IN UINT32                MeNcMemLowLimit,
  IN UINT32                MeNcMemHighLimit
  )
{
  EFI_STATUS  Status;
  ME_UMA_INFO *MeUmaInfo;
  UINT8       InitStat;

  Status = EFI_DEVICE_ERROR;
  MeUmaInfo = MeUmaGetInfo ();
  if (MeUmaInfo != NULL) {
    if (MrcStatus != SUCCESS) {
      if (MemSize == 0) {
        DEBUG ((DEBUG_ERROR, "%a No Memory in Channels.\n", DebugMeUmaPrefix));
        // Set the flag to tell FW No memory in Channels detected InitStat bits 3:0 = (1).
        InitStat = ME_MEMORY_INITIALIZATION_NO_MEMORY_IN_CHANNELS;
      } else {
        DEBUG ((DEBUG_ERROR, "%a Memory Initialization Error.\n", DebugMeUmaPrefix));
        // Set the flag to tell FW that memory initialization error occurred InitStat bits 3:0 = (2).
        InitStat = ME_MEMORY_INITIALIZATION_ERROR;
      }
    } else {
      if ((MeUmaInfo->MeUmaSize > ME_UMA_SIZE_UPPER_LIMIT)) {
        DEBUG ((DEBUG_INFO, "%a Invalid ME UMA Size requested\n", DebugMeUmaPrefix));
        // Set the flag to tell FW that memory initialization error occurred InitStat bits 3:0 = (2).
        InitStat = ME_MEMORY_INITIALIZATION_ERROR;
      } else {
        InitStat = ME_MEMORY_INITIALIZATION_SUCCESSS;
      }
    }

    MeUmaInfo->InitStat = InitStat;
    MeUmaInfo->MeNcMemLowBaseAddr = MeNcMemLowBaseAddr;
    MeUmaInfo->MeNcMemHighBaseAddr = MeNcMemHighBaseAddr;
    MeUmaInfo->MeNcMemLowLimit = MeNcMemLowLimit;
    MeUmaInfo->MeNcMemHighLimit = MeNcMemHighLimit;
    Status = EFI_SUCCESS;
    DEBUG ((DEBUG_INFO, "%a %a () Saved ME UMA Data:\n", DebugMeUmaPrefix, __FUNCTION__));
    DEBUG ((DEBUG_INFO, "\t\t InitStat = %d\n", InitStat));
    DEBUG ((DEBUG_INFO, "\t\t MeNcMemLowBaseAddr = 0x%x \n\t\t MeNcMemHighBaseAddr = 0x%x \n", MeNcMemLowBaseAddr, MeNcMemHighBaseAddr));
    DEBUG ((DEBUG_INFO, "\t\t MeNcMemLowBaseAddr = 0x%x \n\t\t MeNcMemHighBaseAddr = 0x%x \n", MeNcMemLowBaseAddr, MeNcMemHighBaseAddr));
    DEBUG ((DEBUG_INFO, "\t\t MeNcMemLowLimit = 0x%x \n\t\t MeNcMemHighLimit = 0x%x\n", MeNcMemLowLimit, MeNcMemHighLimit));
  }
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Entry point for ME UMA Pre-Mem.

  @param[in] FfsHeader            Pointer to the FFS file header
  @param[in] PeiServices          Pointer to the PEI services table

  @retval Return Status based on errors that occurred while waiting for time to expire.
**/
EFI_STATUS
EFIAPI
MeUmaPreMemEntry (
  IN EFI_PEI_FILE_HANDLE      FileHandle,
  IN CONST EFI_PEI_SERVICES   **PeiServices
  )
{
  VOID                      *TestPpi;
  EFI_STATUS                Status;
  EFI_PEI_PPI_DESCRIPTOR    *MeUmaOldPpiDesc;

  DEBUG ((DEBUG_INFO, "%a ME UMA PPI Driver EntryPoint\n", DebugMeUmaPrefix));

  Status = PeiServicesLocatePpi (&gEfiPeiMemoryDiscoveredPpiGuid, 0, NULL, &TestPpi);
  if (EFI_ERROR (Status)) {
    // Pre-Mem Tasks
    Status = CreateMeUmaInfoHob ();
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot create ME UMA PPI (%r)\n", DebugMeUmaPrefix, Status));
    } else {
      Status = PeiServicesLocatePpi (&gMeUmaPpiGuid, 0, &MeUmaOldPpiDesc, &TestPpi);
      if (EFI_ERROR (Status)) {
        Status = PeiServicesInstallPpi (mMeUmaPpiList);
      } else {
        Status = PeiServicesReInstallPpi (MeUmaOldPpiDesc, mMeUmaPpiList);
      }
    }
    DEBUG ((DEBUG_INFO, "%a ME UMA PPI Installation status %r\n", DebugMeUmaPrefix, Status));
    ASSERT_EFI_ERROR (Status);
  } else {
    // Post-Mem Task
    DEBUG ((DEBUG_ERROR, "%a LOADED IN WRONG PHASE (POST-MEM)\n", DebugMeUmaPrefix));
    Status = EFI_DEVICE_ERROR;
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}