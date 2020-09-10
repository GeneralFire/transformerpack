/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#include "SmmAccessDriver.h"



STATIC SMM_ACCESS_PRIVATE_DATA  mSmmAccess;
static EFI_IIO_UDS_PROTOCOL            *mIioUds;

VOID
EFIAPI
SmmAccessOnBoot (
  IN EFI_EVENT                          Event,
  IN VOID                               *Context
);

/**

  Installs an SMM Access Protocol.

  @param ImageHandle  -  Handle for the image of this driver.
  @param SystemTable  -  Pointer to the EFI System Table.

  @retval EFI_SUCCESS     -  Protocol successfully started and installed.
  @retval EFI_UNSUPPORTED -  Protocol can't be started.
  @retval EFI_NOT_FOUND   -  Protocol not found.
**/
EFI_STATUS
EFIAPI
SmmAccessDriverEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{

  EFI_STATUS                      Status;
  STATIC EFI_EVENT                BootEvent;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo;
  UINTN                           Index;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK  *DescriptorBlock;
  UINT32                          TsegSize;
  UINT32                          TsegBase;
  UINT32                          IedSize;
  EFI_HOB_GUID_TYPE               *GuidHob;


  //
  // Initialize private data
  //
  ZeroMem (&mSmmAccess, sizeof (mSmmAccess));

  Status = gBS->LocateProtocol (
                  &gEfiPciRootBridgeIoProtocolGuid,
                  NULL,
                  &PciRootBridgeIo
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Build SMM related information
  //
  mSmmAccess.Signature        = SMM_ACCESS_PRIVATE_DATA_SIGNATURE;
  mSmmAccess.Handle           = NULL;
  mSmmAccess.PciRootBridgeIo  = PciRootBridgeIo;

  //
  // Get Hob list
  //
  GuidHob    = GetFirstGuidHob (&gEfiSmmPeiSmramMemoryReserveGuid);
  ASSERT(GuidHob != NULL);
  if (GuidHob == NULL) {
      return EFI_NOT_FOUND;
  }
  DescriptorBlock = GET_GUID_HOB_DATA(GuidHob);

  //
  // Located the IIO Protocol Interface
  //
  Status = gBS->LocateProtocol (
                  &gEfiIioUdsProtocolGuid,
                  NULL,
                  &mIioUds
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Use the hob to publish SMRAM capabilities
  //
  ASSERT (DescriptorBlock->NumberOfSmmReservedRegions <= MAX_SMRAM_RANGES);
  for (Index = 0; Index < DescriptorBlock->NumberOfSmmReservedRegions; Index++) {
    mSmmAccess.SmramDesc[Index].PhysicalStart = DescriptorBlock->Descriptor[Index].PhysicalStart;
    mSmmAccess.SmramDesc[Index].CpuStart      = DescriptorBlock->Descriptor[Index].CpuStart;
    mSmmAccess.SmramDesc[Index].PhysicalSize  = DescriptorBlock->Descriptor[Index].PhysicalSize;
    mSmmAccess.SmramDesc[Index].RegionState   = DescriptorBlock->Descriptor[Index].RegionState;
    DEBUG ((EFI_D_INFO, "SM RAM index[%d] startaddr:%08X Size :%08X\n", Index, mSmmAccess.SmramDesc[Index].CpuStart,
      mSmmAccess.SmramDesc[Index].PhysicalSize));
  }

  mSmmAccess.NumberRegions              = Index;
  mSmmAccess.SmmAccess.Open             = Open;
  mSmmAccess.SmmAccess.Close            = Close;
  mSmmAccess.SmmAccess.Lock             = Lock;
  mSmmAccess.SmmAccess.GetCapabilities  = GetCapabilities;
  mSmmAccess.SmmAccess.LockState        = FALSE;
  mSmmAccess.SmmAccess.OpenState        = FALSE;
  mSmmAccess.SMMRegionState             = EFI_SMRAM_CLOSED;

  //
  // Install our protocol interfaces on the device's handle
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                &mSmmAccess.Handle,
                &gEfiSmmAccess2ProtocolGuid,
                &mSmmAccess.SmmAccess,
                NULL
                );
  ASSERT_EFI_ERROR (Status);

  //
  // We require the last entry of SMRAM must be TSEG smram (not including IED region)
  // TSEG base must be the same as SMRAM base
  // IED  base is right after SMRAM, and it extends to TOLM.
  //

  if ( mSmmAccess.NumberRegions == 0 ) {
    return EFI_UNSUPPORTED;
  }

  TsegBase = (UINT32)(mSmmAccess.SmramDesc[mSmmAccess.NumberRegions-1].PhysicalStart);
  TsegSize = (UINT32)(mIioUds->IioUdsPtr->PlatformData.MemTsegSize);
  DEBUG ((EFI_D_INFO, "TSEG Base: %08X\n", TsegBase));
  DEBUG ((EFI_D_INFO, "TSEG Size: %08X\n", TsegSize));

  IedSize = (UINT32)(mIioUds->IioUdsPtr->PlatformData.MemIedSize);
  DEBUG ((EFI_D_INFO, "IED  Base: %08X\n", TsegBase + (UINT32)(mSmmAccess.SmramDesc[mSmmAccess.NumberRegions-1].PhysicalSize)));
  DEBUG ((EFI_D_INFO, "IED  Size: %08X\n", IedSize));

  DEBUG ((EFI_D_INFO, "SMM  Base: %08X\n", (UINT32)(mSmmAccess.SmramDesc[mSmmAccess.NumberRegions-1].PhysicalStart)));
  DEBUG ((EFI_D_INFO, "SMM  Size: %08X\n", (UINT32)(mSmmAccess.SmramDesc[mSmmAccess.NumberRegions-1].PhysicalSize)));

  mSmmAccess.TsegSize = (UINT8)TsegSize;
  //
  // T Seg setting done in QPI RC
  //

  //
  // Prior ReadyToBoot, lock CSEG as required by the sighting 3427224
  //
  Status = EfiCreateEventReadyToBootEx(
           TPL_NOTIFY,
           SmmAccessOnBoot,
           NULL,
           &BootEvent );
  ASSERT (!EFI_ERROR (Status));
  return EFI_SUCCESS;
}

/**

  This routine accepts a request to "open" a region of SMRAM.  The
  region could be legacy ABSEG, HSEG, or TSEG near top of physical memory.
  The use of "open" means that the memory is visible from all boot-service
  and SMM agents.

  @param This             -  Pointer to the SMM Access Interface.
  @param DescriptorIndex  -  Region of SMRAM to Open.

  @retval EFI_SUCCESS            -  The region was successfully opened.
  @retval EFI_DEVICE_ERROR       -  The region could not be opened because locked by
                            @retval chipset.
  @retval EFI_INVALID_PARAMETER  -  The descriptor index was out of bounds.

**/
EFI_STATUS
EFIAPI
Open (
  IN EFI_SMM_ACCESS2_PROTOCOL    *This
  )
{
  SMM_ACCESS_PRIVATE_DATA *SmmAccess;
  UINT64                  RegOffset;


  RegOffset = 0;
  SmmAccess = SMM_ACCESS_PRIVATE_DATA_FROM_THIS (This);

  if (mSmmAccess.SMMRegionState & EFI_SMRAM_LOCKED) {
    DEBUG ((EFI_D_ERROR, "Cannot open a locked SMRAM region\n"));
    return EFI_DEVICE_ERROR;
  }
  mSmmAccess.SMMRegionState &= ~(EFI_SMRAM_CLOSED | EFI_ALLOCATED);
  SyncRegionState2SmramDesc(FALSE, (UINT64)(UINTN)(~(EFI_SMRAM_CLOSED | EFI_ALLOCATED)));
  mSmmAccess.SMMRegionState |= EFI_SMRAM_OPEN;
  SyncRegionState2SmramDesc(TRUE, EFI_SMRAM_OPEN);
  SmmAccess->SmmAccess.OpenState = TRUE;

  return EFI_SUCCESS;
}

/**

  This routine accepts a request to "close" a region of SMRAM.  This is valid for
  compatible SMRAM region.

  @param This             -  Pointer to the SMM Access Interface.
  @param DescriptorIndex  -  Region of SMRAM to Close.

  @retval EFI_SUCCESS            -  The region was successfully closed.
  @retval EFI_DEVICE_ERROR       -  The region could not be closed because locked by
                            @retval chipset.
  @retval EFI_INVALID_PARAMETER  -  The descriptor index was out of bounds.

**/
EFI_STATUS
EFIAPI
Close (
  IN EFI_SMM_ACCESS2_PROTOCOL *This
  )
{
  SMM_ACCESS_PRIVATE_DATA *SmmAccess;
  BOOLEAN                 OpenState;
  UINTN                   Index;
  UINTN                   PciAddress;

  PciAddress = 0;

  SmmAccess     = SMM_ACCESS_PRIVATE_DATA_FROM_THIS (This);

  if (mSmmAccess.SMMRegionState & EFI_SMRAM_LOCKED) {
    //
    // Cannot close a "locked" region
    //
    DEBUG ((EFI_D_WARN, "Cannot close the locked SMRAM Region\n"));
    return EFI_DEVICE_ERROR;
  }

  if (mSmmAccess.SMMRegionState & EFI_SMRAM_CLOSED) {
    return EFI_DEVICE_ERROR;
  }
  //
  // Find out if any regions are still open
  //
  OpenState = FALSE;
  for (Index = 0; Index < mSmmAccess.NumberRegions; Index++) {
    if ((SmmAccess->SmramDesc[Index].RegionState & EFI_SMRAM_OPEN) == EFI_SMRAM_OPEN) {
      OpenState = TRUE;
    }
  }

  SmmAccess->SmmAccess.OpenState = OpenState;

  return EFI_SUCCESS;
}

/**

  This routine accepts a request to "lock" SMRAM.  The
  region could be legacy AB or TSEG near top of physical memory.
  The use of "lock" means that the memory can no longer be opened
  to BS state..

  @param This             -  Pointer to the SMM Access Interface.
  @param DescriptorIndex  -  Region of SMRAM to Lock.

  @retval EFI_SUCCESS            -  The region was successfully locked.
  @retval EFI_DEVICE_ERROR       -  The region could not be locked because at least
                            @retval one range is still open.
  @retval EFI_INVALID_PARAMETER  -  The descriptor index was out of bounds.

**/
EFI_STATUS
EFIAPI
Lock (
  IN EFI_SMM_ACCESS2_PROTOCOL   *This
  )
{
  SMM_ACCESS_PRIVATE_DATA *SmmAccess;

  SmmAccess = SMM_ACCESS_PRIVATE_DATA_FROM_THIS (This);

  if (SmmAccess->SmmAccess.OpenState) {
    return EFI_DEVICE_ERROR;
  }

  mSmmAccess.SMMRegionState |= EFI_SMRAM_LOCKED;
  SyncRegionState2SmramDesc(TRUE, EFI_SMRAM_LOCKED);
  SmmAccess->SmmAccess.LockState                     = TRUE;
  return EFI_SUCCESS;
}

/**

  This routine services a user request to discover the SMRAM
  capabilities of this platform.  This will report the possible
  ranges that are possible for SMRAM access, based upon the
  memory controller capabilities.

  @param This          -  Pointer to the SMRAM Access Interface.
  @param SmramMapSize  -  Pointer to the variable containing size of the
                          buffer to contain the description information.
  @param SmramMap      -  Buffer containing the data describing the Smram
                          region descriptors.
  @retval EFI_BUFFER_TOO_SMALL  -  The user did not provide a sufficient buffer.
  @retval EFI_SUCCESS           -  The user provided a sufficiently-sized buffer.

**/
EFI_STATUS
EFIAPI
GetCapabilities (
  IN CONST  EFI_SMM_ACCESS2_PROTOCOL     *This,
  IN OUT UINTN                       *SmramMapSize,
  IN OUT EFI_SMRAM_DESCRIPTOR        *SmramMap
  )
{
  EFI_STATUS                Status;
  SMM_ACCESS_PRIVATE_DATA  *SmmAccess;
  UINTN                     BufferSize;

  SmmAccess           = SMM_ACCESS_PRIVATE_DATA_FROM_THIS (This);
  BufferSize          = SmmAccess->NumberRegions * sizeof (EFI_SMRAM_DESCRIPTOR);

  if (*SmramMapSize < BufferSize) {
    Status = EFI_BUFFER_TOO_SMALL;
  } else {
    CopyMem (SmramMap, SmmAccess->SmramDesc, *SmramMapSize);
    Status = EFI_SUCCESS;
  }
  *SmramMapSize = BufferSize;

  return Status;
}

/**

    GC_TODO: add routine description

    @param Event   - GC_TODO: add arg description
    @param Context - GC_TODO: add arg description

    @retval None

**/
VOID
EFIAPI
SmmAccessOnBoot (
  IN EFI_EVENT                          Event,
  IN VOID                               *Context
)
{
  gBS->CloseEvent(Event);
}
/**

    GC_TODO: add routine description

    @param OrAnd - GC_TODO: add arg description
    @param Value - GC_TODO: add arg description

    @retval None

**/
VOID
SyncRegionState2SmramDesc(
  IN BOOLEAN  OrAnd,
  IN UINT64   Value
  )
{
  UINT32 Index;

  for (Index = 0; Index < mSmmAccess.NumberRegions; Index++) {
    if (OrAnd) {
      mSmmAccess.SmramDesc[Index].RegionState |= Value;
    } else {
      mSmmAccess.SmramDesc[Index].RegionState &= Value;
    }
  }
}
