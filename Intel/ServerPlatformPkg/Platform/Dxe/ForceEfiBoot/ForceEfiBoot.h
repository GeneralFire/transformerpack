/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2018 Intel Corporation. <BR>

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

#ifndef _FORCE_EFI_BOOT_H_
#define _FORCE_EFI_BOOT_H_

#include <PiDxe.h>

#include <Guid/FileInfo.h>

#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/LoadFile.h>
#include <Protocol/DevicePath.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/BlockIo.h>
#include <Protocol/SimpleNetwork.h>
#include <Protocol/SimpleFileSystem.h>

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/IoLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/MemoryAllocationLib.h>

#define MARKER_FILE_NAME                L"STARTUP.NSH"
#define MARKER_FILE_MAX_LENGTH          200
#define MAX_BUFFER                      128
#define FORCE_BOOT_SIGNATURE            "# FORCE_EFI_BOOT"

#ifndef EFI_FIELD_OFFSET
#define EFI_FIELD_OFFSET(TYPE,Field)    ((UINTN)(&(((TYPE *) 0)->Field)))
#endif

EFI_STATUS LaunchInternalShell (EFI_HANDLE ParentImageHandle);
BOOLEAN IsForceEfiBoot (VOID);

#endif
