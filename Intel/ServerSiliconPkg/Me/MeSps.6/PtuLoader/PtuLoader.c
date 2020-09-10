/** @file
  SPS Node Manager may require Power-Thermal Utility application to be started
  in Host partition to measure the system power consumption. This application
  is provided as PCI Option ROM for MROM device in PCH. But not all PCH
  implement MROM. On systems where MROM is not implemented PTU is invoked
  by this module.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MeTypeLib.h>
#include <Library/MeUtilsLib.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/Spi.h>
#include <IndustryStandard/Pci22.h>

#include <MeAccess.h>
#include <Sps.h>
#include <ConfigBlock/SpsGetDxeConfigBlock.h>


/**
 * PTU Loader entry point.
 *
 * param[in] ImageHandle    Handle of driver image
 * param[in] pSysTable      Pointer to the system table
 *
 * return EFI_STATUS is returned.
 */
EFI_STATUS
PtuLoaderEntryPoint (
  IN     EFI_HANDLE         ImageHandle,
  IN     EFI_SYSTEM_TABLE   *SystemTable)
{
  EFI_STATUS                                  Status;
  UINT32                                      ImageSize;
  PCH_SPI_PROTOCOL                            *pSpiProtocol;
  EFI_PCI_EXPANSION_ROM_HEADER                PtuRomHeader;
  MEDIA_RELATIVE_OFFSET_RANGE_DEVICE_PATH     EfiOpRomImageNode;
  EFI_DEVICE_PATH_PROTOCOL                    *pPciOptionRomImageDevicePath;
  PCI_DATA_STRUCTURE                          Pcir;
  VOID                                        *pPtuImage = NULL;
  UINT16                                      ImageOffset;
  UINT32                                      InitializationSize;
  HECI_FWS_REGISTER                           Mefs1;
  SPS_NMFS                                    Nmfs;
#ifdef ME_TESTMENU_FLAG
  SPS_DXE_CONFIG                             *pSpsPolicy = NULL;

  pSpsPolicy = GetSpsDxeConfigBlock ();
  if (pSpsPolicy != NULL && pSpsPolicy->NmPtuLoadOverride)
  {
    DEBUG((EFI_D_INFO, "[SPS] PTU execution forced in configuration\n"));
  }
  else
#endif
  {
    Mefs1.ul = HeciPciRead32 (R_ME_HFS);
    Nmfs.UInt32 = Heci2PciRead32 (SPS_REG_NMFS);
    if (!MeTypeIsSps () ||
        !MeSpsIsNm () ||
        !Nmfs.Bits.DirectPtuExecution) {
      DEBUG((EFI_D_INFO, "[SPS] PTU execution not required (MEFS1: 0x%08X, NMFS: 0x%08X)\n",
                         Mefs1.ul, Nmfs.UInt32));
      return EFI_SUCCESS;
    }
  }
  Status = gBS->LocateProtocol(&gPchSpiProtocolGuid, NULL, (VOID**)&pSpiProtocol);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: LocateProtocol gPchSpiProtocolGuid failed (%r)\n", Status));
    return Status;
  }

  Status = pSpiProtocol->FlashRead(pSpiProtocol, FlashRegionDer, 0,
                                   sizeof(EFI_PCI_EXPANSION_ROM_HEADER), (UINT8*)&PtuRomHeader);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Read PCI expansion ROM header from SPI flash failed (%r)\n", Status));
    return Status;
  }

  if (PtuRomHeader.Signature != PCI_EXPANSION_ROM_HEADER_SIGNATURE)
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: PCI expansion ROM signature not found\n"));
    return EFI_NOT_FOUND;
  }

  Status = pSpiProtocol->FlashRead(pSpiProtocol, FlashRegionDer, PtuRomHeader.PcirOffset,
                                   sizeof(PCI_DATA_STRUCTURE), (UINT8 *) &Pcir);

  if (EFI_ERROR(Status) || Pcir.Signature != PCI_DATA_STRUCTURE_SIGNATURE)
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: PCI data signature not found in DER region\n"));
    return EFI_NOT_FOUND;
  }

  EfiOpRomImageNode.Header.Type     = MEDIA_DEVICE_PATH;
  EfiOpRomImageNode.Header.SubType  = MEDIA_RELATIVE_OFFSET_RANGE_DP;
  SetDevicePathNodeLength (&EfiOpRomImageNode.Header, sizeof (EfiOpRomImageNode));

  EfiOpRomImageNode.StartingOffset  = 0x0;
  EfiOpRomImageNode.EndingOffset    = (UINT32)(Pcir.ImageLength * 512) - 1;

  pPciOptionRomImageDevicePath = AppendDevicePathNode(NULL, NULL);
  pPciOptionRomImageDevicePath = AppendDevicePathNode(pPciOptionRomImageDevicePath, &EfiOpRomImageNode.Header);
  if (pPciOptionRomImageDevicePath == NULL)
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Unable to create PCI OpROM image device path\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  ImageOffset        = PtuRomHeader.EfiImageHeaderOffset;
  InitializationSize = PtuRomHeader.InitializationSize * 512;
  ImageSize          = InitializationSize - ImageOffset;

  Status = gBS->AllocatePool(EfiBootServicesData, ImageSize, &pPtuImage);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: AllocatePool for PTU image failed (%r)\n", Status));
    goto ErrExit0;
  }

  Status = pSpiProtocol->FlashRead(pSpiProtocol, FlashRegionDer, ImageOffset, ImageSize, (UINT8*)pPtuImage);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Read PTU image from SPI filed (%r)\n", Status));
    goto ErrExit1;
  }

  ImageHandle = NULL;
  Status = gBS->LoadImage(FALSE, gImageHandle, pPciOptionRomImageDevicePath, pPtuImage,
                          ImageSize, &ImageHandle);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: PTU image load failed (%r)\n", Status));
    goto ErrExit1;
  }

  Status = gBS->StartImage (ImageHandle, NULL, NULL);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: PTU image start failed (%r)\n", Status));
    goto ErrExit1;
  }

  return EFI_SUCCESS;

 ErrExit1:
  gBS->FreePool(pPtuImage);
 ErrExit0:
  gBS->FreePool(pPciOptionRomImageDevicePath);
  return Status;
} // PtuLoaderEntryPoint()

