/** @file
  This file implements an API for enabling IIO Sierra Peak Initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Base.h>
#include <PiDxe.h>

#include <Library/DebugLib.h>
#include <Library/IoLib.h>

#include <IioSetupDefinitions.h>
#include <IioPlatformData.h>

#include <Library/IioAccessLib.h>

#include <LibraryPrivate/IioSpkConfigure.h>
#include <LibraryPrivate/IioDebug.h>

#include "IioSpkConfigure.h"
#include <Library/IioSpkInit/IioSpkInitDefs.h>
#include <Library/IioAccessLib.h>
#include <Library/IioDataHubLib.h>
#include <Library/BaseMemoryLib.h>

/**
  This routine detects active SPK instances in ADL subsystems and count offsets to the SPK regs.

  @param[in]       IioIndex           - Index with the changed IIO (Socket)
  @param[in, out] *SpkInstancesCount  - On entry SpkOffsets[] table length,
                                        on exit the number of elements provided in SpkOffsets[]
  @param[in, out]  SpkOffsets[]       - Array of SPK base registers offsets

  @retval EFI_SUCCESS                 - SPK instancecs detected successfully.
  @retval EFI_BUFFER_TOO_SMALL        - More SPK instances is discovered than expected.
  @retval EFI_UNSUPPORTED             - If SPK not supported.
**/
EFI_STATUS
IioSpkDetectInstances (
  IN UINT8                         IioIndex,
  IN OUT UINT8                    *SpkInstancesCount,
  IN OUT UINT32                    SpkOffsets[]
  )
{
  EFI_STATUS                       Status;
  EFI_PHYSICAL_ADDRESS             DfdBar;
  EFI_PHYSICAL_ADDRESS             AdlSubsystemBase;
  PCI_ROOT_BRIDGE_PCI_ADDRESS      OobMsmDev;
  UINT16                           AdlIndex;
  UINT32                           Reg32;
  IIO_GLOBALS                     *IioGlobalData;

  Status = GetIioGlobalData (&IioGlobalData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *SpkInstancesCount = 0;

  ZeroMem (SpkOffsets, sizeof (SpkOffsets[0]) * MAX_SPK_INSTANCES_COUNT);

  //
  // Complete the BDF for OOB-MSM device
  //
  InitOobMsmBdf (IioIndex, &OobMsmDev);

  //
  // Get OOB_MSM_CFG_BAR0 value
  //
  DfdBar = GetAddressFromMBar01 (IioIndex, OobMsmDev);
  if (DfdBar == 0) {
    IIO_D_SPKERR ("IioIndex: %d Can't set OOB-MSM!\n", IioIndex);
    return EFI_UNSUPPORTED;
  }

  EnableMBarAccess (IioIndex, OobMsmDev);

  //
  // Iterate over 4k blocks in 1MB of DFD BAR MMIO
  //
  for (AdlIndex = 0; AdlIndex < 256; AdlIndex++) {
    //
    // Count the address of the ADL subsystem beginning
    //
    AdlSubsystemBase = DfdBar + (AdlIndex * SIZE_4KB);
    //
    // Check ADL Subsystem Info Register bit 22 (SPK Present)
    //
    Reg32 = IioMmioRead32 (AdlSubsystemBase + ADL_SUBSYSTEM_INFO_REG_OFFSET);
    if (Reg32 == 0xFFFFFFFF || Reg32 == 0 || (Reg32 & ADL_SUBSYSTEM_INFO_SPK_PRESENT) == 0) {
      continue;
    }
    //
    // SPK is present and active in this ADL subsystem
    //
    SpkOffsets[*SpkInstancesCount] = (AdlIndex * SIZE_4KB) + ADL_SUBSYSTEM_SPK_BASE_OFFSET;
    *SpkInstancesCount += 1;

    if (*SpkInstancesCount > MAX_SPK_INSTANCES_COUNT) {
      //
      // This is strange - more SPK instances is discovered than expected
      //
      return EFI_BUFFER_TOO_SMALL;
    }
  }

  DisableMBarAccess (IioIndex, OobMsmDev);

  return EFI_SUCCESS;
}

/**
  This routine writes values of SPK memory base and size registers.

  @param[in] *IioGlobalData      - Pointer to Global Data Structure.
  @param[in]  IioIndex           - Index with the changed IIO (Socket)
  @param[in] *BufferBase         - Array of base addresses of buffers
  @param[in]  BufferSize         - Size of each allocated buffer in bytes
  @param[in] *SpkInstancesCount  - Number of detected SPK instances
  @param[in]  SpkOffsets[]       - Array of SPK base registers offsets

  @retval EFI_SUCCESS       - SPK Trace Buffer configured successfully.
  @retval EFI_UNSUPPORTED   - If SPK not supported.
**/
EFI_STATUS
IioSpkConfigureTraceBuffers (
  IN IIO_GLOBALS             *IioGlobalData,
  IN UINT8                    IioIndex,
  IN EFI_PHYSICAL_ADDRESS    *BufferBase,
  IN UINT32                   BufferSize,
  IN UINT8                   *SpkInstancesCount,
  IN UINT32                   SpkOffsets[]
  )
{
  EFI_STATUS                  Status;
  EFI_PHYSICAL_ADDRESS        DfdBar;
  IIO_PTR_ADDRESS             SpkMemBuffer;
  PCI_ROOT_BRIDGE_PCI_ADDRESS OobMsmDev;
  UINT8                       SpkIndex;
  UINT32                      Offset;

  if (*SpkInstancesCount == 0) {
    IIO_D_SPKLOG ("IioIndex: %d No SPK instances detected!\n", IioIndex);
    return EFI_SUCCESS;
  }

  Status = GetIioGlobalData (&IioGlobalData);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Complete the BDF for OOB-MSM device
  //
  InitOobMsmBdf (IioIndex, &OobMsmDev);

  //
  // Get OOB_MSM_CFG_BAR0 value
  //
  DfdBar = GetAddressFromMBar01 (IioIndex, OobMsmDev);
  if (DfdBar == 0) {
    IIO_D_SPKERR ("IioIndex: %d Can't set OOB-MSM!\n", IioIndex);
    return EFI_UNSUPPORTED;
  }

  EnableMBarAccess (IioIndex, OobMsmDev);

  //
  // Write SPK_LBAR/UBAR/SIZE registers for each SPK instance
  //
  for (SpkIndex = 0; SpkIndex < *SpkInstancesCount; SpkIndex++) {
    //
    // Entire upper and lower register is shifted 12 bits.
    // Example, lower bar is used as [43:12] in SPK Bar
    //
    SpkMemBuffer.Address64bit = BufferBase[SpkIndex] >> SPK_BAR_ALIGN;

    Offset = SpkOffsets[SpkIndex];

    IioMmioWrite32 ((UINTN) (DfdBar + Offset + SPK_LBAR_OFFSET), (SpkMemBuffer.Address32bit.Value));
    IioMmioWrite32 ((UINTN) (DfdBar + Offset + SPK_UBAR_OFFSET), (SpkMemBuffer.Address32bit.ValueHigh));
    //
    // Write SPK Buffer size as number of 4k blocks
    //
    IioMmioWrite32 ((UINTN) (DfdBar + Offset + SPK_SIZE_OFFSET), BufferSize >> SPK_BAR_ALIGN);

    IIO_D_SPKLOG ("IioIndex: %d Spk: %d UBAR | LBAR | SIZE = [0x%X | 0x%X | 0x%X]\n",
        IioIndex,
        SpkIndex,
        IioMmioRead32 ((UINTN) (DfdBar + Offset + SPK_UBAR_OFFSET)),
        IioMmioRead32 ((UINTN) (DfdBar + Offset + SPK_LBAR_OFFSET)),
        IioMmioRead32 ((UINTN) (DfdBar + Offset + SPK_SIZE_OFFSET))
    );
  }

  DisableMBarAccess (IioIndex, OobMsmDev);

  return EFI_SUCCESS;
}
