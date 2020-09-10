/** @file
  Platform Variable Initialization PEIM.

  This PEIM determines whether to load variable defaults. Ordinarily, the
  decision is based on the boot mode and an OEM hook is provided to override
  that. The appropriate HOBs and PCDs are created to signal DXE code to update
  the variable default values.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#include "PlatformVariableInitPei.h"

EFI_PEI_PPI_DESCRIPTOR  mPpiListPlatformVariableInit = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPlatformVariableInitPpiGuid,
  NULL
};

EFI_PEI_NOTIFY_DESCRIPTOR mVariableNotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK),
    &gEfiPeiReadOnlyVariable2PpiGuid,
    VariableServicesCallback
  },
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gUpdateBootModePpiGuid,
    BootModeChangeCallback
  }
};

EFI_PEI_NOTIFY_DESCRIPTOR mMemoryDiscoveredNotifyList = {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiMemoryDiscoveredPpiGuid,
    MemoryDiscoveredNotifyCallback
};

//
// Board Id is currently 0 for all platforms
//
UINT16 mBoardId = BOARD_ID_DEFAULT;

/**
  Apply platform variable defaults.

  Create HOBs and set PCDs to prompt the (re-)loading of variable defaults.
  Each step is attempted regardless of whether the previous steps succeeded.
  If multiple errors occur, only the last error code is returned.

  @param[in]  Events            Bitmap of events that occurred as described
                                in PlatformVariableCommon api:
                                0, NULL_VARIABLE_EVENT, MFG_MODE_EVENT
                                RECOVERY_MODE_EVENT, CMOS_CLEAR_EVENT
  @param[in]  DefaultId         Default store ID, STANDARD or MANUFACTURING.

  @retval EFI_SUCCESS           All steps completed successfully.
  @retval EFI_OUT_OF_RESOURCES  One of the HOBs could not be created.
  @retval EFI_NOT_FOUND         The default data could not be found in FFS.
**/
EFI_STATUS
ApplyPlatformVariableDefaults (
  IN UINT8  Events,
  IN UINT16 DefaultId
  )
{
  VOID        *Hob;
  EFI_STATUS  Status;
  EFI_STATUS  ReturnStatus;

  DEBUG ((DEBUG_INFO, "Platform Var Events = 0x%02x\n", Events));
  DEBUG ((DEBUG_INFO, "Platform Var DefaultId = 0x%04x\n", DefaultId));

  //
  // Assume success up front. This will be overwritten if errors occur.
  //
  ReturnStatus = EFI_SUCCESS;

  //
  // Send the bitmap of events to the platform variable DXE driver.
  //
  Hob = BuildGuidDataHob (&gPlatformVariableHobGuid, &Events, sizeof (Events));
  if (Hob == NULL) {
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    ReturnStatus = EFI_OUT_OF_RESOURCES;
  }

  Status = DetermineAndSetVpdBaseAddress (&gVpdFileNameGuid);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    ReturnStatus = Status;
  }

  //
  // Set the PCD SKU ID.
  //
  LibPcdSetSku (mBoardId);

  //
  // Set the PCD default store ID.
  //
  Status = PcdSet16S (PcdSetNvStoreDefaultId, DefaultId);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    ReturnStatus = Status;
  }

  return ReturnStatus;
}

/**
  Locate and save the VPD region base address

  Given the GUID of the VPD region, locate VPD region, and set the VPD PCD
  to the base address of the VPD region.
  Allows the (re-)loading of variable defaults from the VPD region, when the
  VPD region is stored as a FFS or in a FV.

  @param[in]  FileGuid          The GUID of the VPD File.

  @retval EFI_SUCCESS           Search completed successfully.
  @retval EFI_NOT_FOUND         Something went wrong in searching FFS files.
**/
EFI_STATUS
DetermineAndSetVpdBaseAddress (
  IN EFI_GUID *FileGuid
  )
{
  UINTN                  Instance;
  EFI_PEI_FV_HANDLE      VolumeHandle;
  EFI_PEI_FILE_HANDLE    FileHandle;
  VOID                   *VpdBin;
  EFI_STATUS             Status;

  Instance = 0;
  FileHandle = NULL;
  VpdBin = NULL;

  while (TRUE) {
    //
    // Traverse all firmware volume instances and get a VolumeHandle
    //
    Status = PeiServicesFfsFindNextVolume (Instance, &VolumeHandle);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      break;
    }

    FileHandle = NULL;

    //
    // Find VPD FileHandle from the beginning in this firmware volume.
    //
    Status = PeiServicesFfsFindFileByName (FileGuid, VolumeHandle, &FileHandle);
    if (!EFI_ERROR (Status)) {
      //
      // Found VPD FileHandle in this volume
      //
      break;
    }
    //
    // We cannot find VPD in this firmware volume, then search the next volume.
    //
    Instance++;
  }

  //
  // Find PEI VPD and Set PCD
  //
  Status = PeiServicesFfsFindSectionData (EFI_SECTION_RAW, FileHandle, &VpdBin);
  ASSERT_EFI_ERROR (Status);

  Status = PcdSet64S (PcdVpdBaseAddress64, (UINT64) (UINTN) VpdBin);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
  This is a callback function that will be notified if memory has been installed
  and determine and set VPD base address in a PCD for new VPD region in memory.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Pointer to Notify PPI descriptor.
  @param[in]  Interface         Pointer to PPI.

  @retval EFI_SUCCESS           VPD base address found and set.
  @retval Other Values          Can't find the matched default setting.
**/
EFI_STATUS
EFIAPI
MemoryDiscoveredNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Interface
  )
{
  EFI_STATUS                      Status;

  Status = DetermineAndSetVpdBaseAddress (&gVpdFileNameGuid);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

/**
  This is a notification function that will check if the board wants default
  settings applied or if the setup variables are not present and will publish
  defaults if needed.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Pointer to Notify PPI descriptor.
  @param[in]  Interface         Pointer to PPI.

  @retval EFI_SUCCESS           Default setting is initialized into variable.
  @retval Other Values          Can't find the matched default setting.
**/
EFI_STATUS
EFIAPI
VariableServicesCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Interface
  )
{
  EFI_STATUS                      Status;
  UINT8                           *SystemConfiguration;
  EFI_GUID                        *SystemConfigurationGuid;
  UINTN                           DataSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  UINT8                           Events;
  UINT16                          DefaultId;
  BOOLEAN                         ApplyDefaults;

  SystemConfigurationGuid = PcdGetPtr (PcdSetupVariableGuid);
  Events = 0;
  DefaultId = EFI_HII_DEFAULT_CLASS_STANDARD;

  if (PlatformVariableHookForHobGeneration (Interface, &Events, &DefaultId)) {
    //
    // Use events bitmap and default ID returned by PlatformVariableHook.
    //
    ApplyDefaults = TRUE;

  } else {
    //
    // If the setup variable does not exist (yet), defaults should be applied.
    //
    VariableServices = (EFI_PEI_READ_ONLY_VARIABLE2_PPI *) Interface;
    SystemConfiguration = NULL;
    DataSize = 0;
    Status = VariableServices->GetVariable (
                                 VariableServices,
                                 PLATFORM_SETUP_VARIABLE_NAME,
                                 SystemConfigurationGuid,
                                 NULL,
                                 &DataSize,
                                 SystemConfiguration
                                 );
    //
    // Setup variable is not found. So, set the default setting.
    //
    if (Status == EFI_NOT_FOUND) {
      Events = NULL_VARIABLE_EVENT;
      DefaultId = EFI_HII_DEFAULT_CLASS_STANDARD;
      ApplyDefaults = TRUE;
    } else {
      ApplyDefaults = FALSE;
    }
  }


  //
  // Populate the Platform Variable HOB with appropriate values
  //
  if (ApplyDefaults) {
    Status = ApplyPlatformVariableDefaults (Events, DefaultId);
  } else {
    //
    // Normal case boot flow
    //
    Events = 0; // no events occurred
    //
    // Build the HOB as it was, do not apply defaults
    //
    BuildGuidDataHob (&gPlatformVariableHobGuid, &Events, sizeof (UINT8));

    //
    // Patch RP variable value with PC variable in the begining of PEI
    //
    Status = CreateRPVariableHob (EFI_HII_DEFAULT_CLASS_STANDARD, mBoardId);
  }

  PeiServicesInstallPpi (&mPpiListPlatformVariableInit);
  return Status;
}


/**
  Variable Init BootMode CallBack
  Get the boot mode and based on boot mode choose which default values will be
  applied or re-applied, if any.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Pointer to Notify PPI descriptor.
  @param[in]  Interface         Pointer to PPI.

  @retval EFI_SUCCESS           Boot mode was discovered and default variable
                                application decision was made
  @retval Other Values          Something went wrong in discovering boot mode
                                or applying defaults
**/
EFI_STATUS
EFIAPI
BootModeChangeCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Interface
  )
{
  EFI_BOOT_MODE                   BootMode;
  BOOLEAN                         ApplyDefaults;
  UINT8                           Events;
  UINT16                          DefaultId;
  EFI_STATUS                      Status;

  Events = 0;
  DefaultId = EFI_HII_DEFAULT_CLASS_STANDARD;

  //
  // Certain boot modes require defaults to be (re-)applied.
  //
  Status = PeiServicesGetBootMode (&BootMode);
  if (EFI_ERROR (Status)) {
    //
    // Booting with defaults, but this should never happen
    //
    ASSERT_EFI_ERROR (Status);
    BootMode = BOOT_WITH_DEFAULT_SETTINGS;
  }

  switch (BootMode) {
    case(BOOT_WITH_MFG_MODE_SETTINGS):
      Events = MFG_MODE_EVENT;
      DefaultId = EFI_HII_DEFAULT_CLASS_MANUFACTURING;
      ApplyDefaults = TRUE;
      break;

    case(BOOT_IN_RECOVERY_MODE):
      Events = RECOVERY_MODE_EVENT;
      DefaultId = EFI_HII_DEFAULT_CLASS_STANDARD;
      ApplyDefaults = TRUE;
      break;

    case(BOOT_WITH_DEFAULT_SETTINGS):
      Events = CMOS_CLEAR_EVENT;
      DefaultId = EFI_HII_DEFAULT_CLASS_STANDARD;
      ApplyDefaults = TRUE;
      break;

    default:
      ApplyDefaults = FALSE;
      break;
  }

  if (ApplyDefaults) {
    Status = ApplyPlatformVariableDefaults (Events, DefaultId);
  }

  return Status;
}

/**
  Installs notifications on variable service and boot mode changes with OEM hook.

  @param[in]  FileHandle        A handle for the File that is initializing this driver.
  @param[in]  PeiServices       General purpose services available to every PEIM.

  @retval EFI_SUCCESS           Driver initialized successfully.
  @retval EFI_LOAD_ERROR        Failed to Initialize or has been loaded.
  @retval EFI_OUT_OF_RESOURCES  Could not allocate needed resources.
**/
EFI_STATUS
EFIAPI
PlatformVariableInitPeimEntry (
  IN EFI_PEI_FILE_HANDLE            FileHandle,
  IN CONST EFI_PEI_SERVICES         **PeiServices
  )
{
  EFI_STATUS                    Status;

  PlatformVariableHookForEntry ();

  //
  // Register notify to set default variable once variable service is ready.
  //
  Status = PeiServicesNotifyPpi (&mVariableNotifyList[0]);

  //
  // Register notify to determine and set VPD base address, after permanent memory is installed.
  //
  Status = PeiServicesNotifyPpi (&mMemoryDiscoveredNotifyList);

  return Status;
}
