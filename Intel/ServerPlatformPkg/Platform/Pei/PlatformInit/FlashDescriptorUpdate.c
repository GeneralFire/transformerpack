/** @file
  FlashDescriptorUpdate.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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

#include "PlatformInitPreMem.h"

#include <Ppi/PchReset.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <PchResetPlatformSpecific.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/UbaSoftStrapUpdateLib.h>
#if defined(ME_WS_SUPPORT) && ME_WS_SUPPORT
#include "Library/MeTypeLib.h"
#endif //ME_WS_SUPPORT
#include <Library/EmulationConfigurationLib.h>

#define SPI_DW_OFFSET_FLVALSIG             4
#define SPI_DW_OFFSET_FLMAP0               5
#define SPI_DW_OFFSET_FLMAP1               6


/**
  isFlashDescriptorLocked -         Checks Flash Descriptor access for write

  @param[in] SpiProtocol            Pointer to the SpiProtocol instance
  @param[in] FlashDescriptor        Pointer to the Flash Descriptor copy

  @retval TRUE                      Flash descriptor is locked for write
  @retval FALSE                     Flash descriptor is open for write
**/
BOOLEAN
isFlashDescriptorLocked(
  IN PCH_SPI_PROTOCOL *SpiProtocol,
  IN UINT32           *FlashDescriptor
  )
{
  UINT32             Flmap1;
  UINT32             Flmstr;
  UINT32             Data32;

  DEBUG((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  //
  // Check Host CPU/BIOS Master Region Write Access for Descriptor region
  //
  Data32 = *(FlashDescriptor + SPI_DW_OFFSET_FLVALSIG);
  if(Data32 != V_SPI_FLASH_FDBAR_FLVALSIG) {
    //Valid signature NOT found
    DEBUG((EFI_D_ERROR, "Error! Valid flash signature NOT found!\n"));
    return TRUE;
  }

  // Read Flash MAP 1
  Flmap1 = *(FlashDescriptor + SPI_DW_OFFSET_FLMAP1);

  //Flash Master 1 - Host CPU / BIOS
  Flmstr = *(UINT32 *)((UINT8 *)FlashDescriptor + ((Flmap1 & 0x000000FF) << 4));

  DEBUG((EFI_D_INFO, "Flash MAP 1= 0x%08x, Flash Master 1= 0x%08x\n", Flmap1, Flmstr));

  if ((Flmstr & BIT20) != BIT20) {
    // Host CPU/BIOS Master Region Write Access for Descriptor region DISABLED
    DEBUG((EFI_D_INFO, "isFlashDescriptorLocked(): Host CPU/BIOS Master Write Access for descriptor region DISABLED\n"));
    return TRUE;
  }

  DEBUG((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
  return FALSE;
}

/**
  PchSoftStrapFixup - Overrides PCH softstraps based on Board ID

  @param[in] *FlashDescriptor         Pointer to copy of Flash Descriptor

  @retval VOID
**/
VOID
PchSoftStrapFixup (
  IN UINT32                             *FlashDescriptor
  )
{
  EFI_STATUS                            Status;
  UINT32                                CurrentStrapValue;
  UINT32                                NewStrapValue;
  PLATFORM_PCH_SOFTSTRAP_FIXUP_ENTRY    *SoftstrapFixupEntry;
  UINT64                                Mask;
  UINTN                                 Count;
  UINT32                                Flmap1;
  UINT32                                *StrapBaseAddr;
  UINT32                                StrapMax;

  DEBUG ((DEBUG_INFO, "%a() - Start\n", __FUNCTION__));
  // APTIOV_SERVER_OVERRIDE_RC_START : Do not fixup Straps for Simics
  if (IsSimicsEnvironment() == TRUE) {
  	return ;
  }
  // APTIOV_SERVER_OVERRIDE_RC_END : Do not fixup Straps for Simics

  Flmap1 = *(FlashDescriptor + SPI_DW_OFFSET_FLMAP1);
  StrapBaseAddr = (UINT32 *)((UINT8 *)FlashDescriptor + ((Flmap1 & 0x00FF0000) >> 12));
  StrapMax = (Flmap1 & 0xFF000000) >> 24;

  SoftstrapFixupEntry = NULL;
  Status = GetPchSoftSoftStrapTable (&SoftstrapFixupEntry);
  if (EFI_ERROR (Status) || SoftstrapFixupEntry == NULL) {
    return;
  }

  while (SoftstrapFixupEntry->BitfieldLength != 0) {
    if (SoftstrapFixupEntry->SoftStrapNumber > StrapMax) {
      SoftstrapFixupEntry++;
      continue;
    }
    NewStrapValue = CurrentStrapValue = *(StrapBaseAddr + SoftstrapFixupEntry->SoftStrapNumber);
    Mask = LShiftU64((UINT64)1, SoftstrapFixupEntry->BitfieldOffset);
    Count = 1;
    while (Count++ < SoftstrapFixupEntry->BitfieldLength) {
      Mask |= MultU64x32(Mask, 2);
    }
    NewStrapValue &= ~Mask;
    NewStrapValue |= (SoftstrapFixupEntry->Value << SoftstrapFixupEntry->BitfieldOffset) & Mask;
    *(StrapBaseAddr + SoftstrapFixupEntry->SoftStrapNumber) = NewStrapValue;
    SoftstrapFixupEntry++;
  }

  DEBUG((DEBUG_INFO, "%a() - End\n", __FUNCTION__));
}

/**
  PchLockDescriptorRegion - Lock Access Permission to Flash Descriptor for Host

  @param[in] FlashDescriptor - Pointer to copy of Flash Descriptor

  @retval VOID
**/
VOID
PchLockDescriptorRegion (
  IN UINT32 *FlashDescriptor
  )
{
  UINT32    Flmap1;
  UINT32    Flmstr;

  DEBUG ((DEBUG_INFO, "%a() - Start\n", __FUNCTION__));

#if defined(ME_WS_SUPPORT) && ME_WS_SUPPORT
  if (MeTypeIsClient ()) {
    DEBUG((EFI_D_INFO, "For Ws do not execute %a()\n", __FUNCTION__));
    return;
  }
#endif // ME_WS_SUPPORT

  // Read Flash MAP 1
  Flmap1 = *(FlashDescriptor + SPI_DW_OFFSET_FLMAP1);

  //Flash Master 1 - Host CPU / BIOS
  Flmstr = *(UINT32 *)((UINT8 *)FlashDescriptor + ((Flmap1 & 0x000000FF) << 4));

  if ((Flmstr & BIT20) == BIT20) {
    Flmstr &= (UINT32)~BIT20;
  }

  *(UINT32 *)((UINT8 *)FlashDescriptor + ((Flmap1 & 0x000000FF) << 4)) = Flmstr;

  DEBUG ((DEBUG_INFO, "%a() - End\n", __FUNCTION__));
}

/**
  SpiFlashDescriptorUpdate - Overrides SPI Flash Descriptor content if necessary

  @param[in] PeiServices                Pointer to the PeiServices

  @retval EFI_SUCCESS                   FD updated
  @retval EFI_ABORTED                   FD update not needed or not possible due to FD access denied
**/
EFI_STATUS
EFIAPI
SpiFlashDescriptorUpdate (
  IN EFI_PEI_SERVICES            **PeiServices
  )
{
  PCH_SPI_PROTOCOL            *SpiProtocol = NULL;
  EFI_STATUS                  Status;
  UINT32                      *BackUpBlock;
  UINT32                      *NewBlock;
  UINT32                      BackUpSize;
  PCH_RESET_DATA              ResetData;

  DEBUG ((DEBUG_INFO, "%a() - Start\n", __FUNCTION__));

  Status = (**PeiServices).LocatePpi (
                              PeiServices,
                              &gPchSpiPpiGuid,
                              0,
                              NULL,
                              &SpiProtocol
                              );
  ASSERT_EFI_ERROR (Status);

  BackUpSize = SIZE_4KB;
  Status   = (*PeiServices)->AllocatePool (
                              PeiServices,
                              BackUpSize,
                              &BackUpBlock
                              );
  if(Status!= EFI_SUCCESS){
    DEBUG((DEBUG_ERROR,  "%a: ERROR: Unable to allocate buffer of size=%x\n", __FUNCTION__, BackUpSize));
    return(Status);
  }

  Status   = (*PeiServices)->AllocatePool (
                              PeiServices,
                              BackUpSize,
                              &NewBlock
                              );
  if(Status!= EFI_SUCCESS) {
    DEBUG((DEBUG_ERROR,  "%a: ERROR: Unable to allocate buffer of size=%x\n", __FUNCTION__, BackUpSize));
    return(Status);
  }

  Status = SpiProtocol->FlashRead (SpiProtocol, FlashRegionDescriptor, 0, BackUpSize, (UINT8 *)BackUpBlock);
  ASSERT_EFI_ERROR (Status);

  CopyMem ((VOID *)NewBlock, (VOID *)BackUpBlock, BackUpSize);

  if (isFlashDescriptorLocked (SpiProtocol, NewBlock) == TRUE) {
    return EFI_ABORTED;
  }

  //
  // Fix PCH Softstraps based on Board ID
  //
  PchSoftStrapFixup (NewBlock);

  //
  // Other Platform Specific PCH SoftStrap Update
  //
  PlatformSpecificPchSoftStrapUpdate ((UINT8 *)NewBlock);

  if (PcdGetBool (PcdDfxAdvDebugJumper) == FALSE) {
    //
    // Finaly Lock SPI Flash Descriptor
    //
// APTIOV_SERVER_OVERRIDE_RC_START : Use token to control FDT lock
#if defined FDT_LOCK_ENABLE && FDT_LOCK_ENABLE == 1
    PchLockDescriptorRegion (NewBlock);
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Use token to control FDT lock
  }

  DEBUG ((DEBUG_INFO, "%a() - End\n", __FUNCTION__));
  if (CompareMem(NewBlock, BackUpBlock, BackUpSize)) {
    Status = SpiProtocol->FlashErase (SpiProtocol, FlashRegionDescriptor, 0, BackUpSize);
    ASSERT_EFI_ERROR (Status);
    Status = SpiProtocol->FlashWrite (SpiProtocol, FlashRegionDescriptor, 0, BackUpSize, (UINT8 *)NewBlock);
    ASSERT_EFI_ERROR (Status);
    if (!(HybridSystemLevelEmulationEnabled ())) {
      CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
      StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
      (*GetPeiServicesTablePointer ())->ResetSystem2 (EfiResetPlatformSpecific, EFI_SUCCESS, sizeof (PCH_RESET_DATA), &ResetData);
    }
    return EFI_SUCCESS;
  } else {
    return EFI_ABORTED;
  }
}

