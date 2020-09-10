/** @file
  Platform Variable Initialization PEIM API.

  This PEIM API includes necessary macros, api and library information to support
  the platform variable PEIM using the VPD region. This version of the PEIM includes
  an implementaion of Platform Variable init that signals DXE code to get default
  variable values from a VPD region instead of the FFS produced by the original
  version. The appropriate base address value for the VPD region can be discovered
  and set for later DXE operations.

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

#ifndef _PLATFORM_VARIABLE_INIT_PEI_H_
#define _PLATFORM_VARIABLE_INIT_PEI_H_

#include <PiPei.h>

#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/FirmwareVolumeInfo.h>
#include <Ppi/CpuIo.h>

#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/MultiPlatSupportLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PlatformVariableHookLib.h>
#include <Library/PlatformSetupVariableSyncLib.h>

#include <Guid/PlatformVariableCommon.h>

#include <Uefi/UefiInternalFormRepresentation.h>

//
// We only have one ID for all the platforms.
//
#define BOARD_ID_DEFAULT  0

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

#endif  // #ifndef _PLATFORM_VARIABLE_INIT_PEI_H_
