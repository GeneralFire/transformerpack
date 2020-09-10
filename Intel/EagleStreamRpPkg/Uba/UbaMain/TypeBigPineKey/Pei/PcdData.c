/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include "PeiBoardInit.h"
#include <ImonVrSvid.h>
#include <Library/UbaPcdUpdateLib.h>
#include <Library/PcdLib.h>

#define BIOSGUARD_SUPPORT_ENABLED BIT0

/**
  Update Big Pine Key IMON SVID Information

  retval N/A
**/
VOID
TypeBigPineKeyPlatformUpdateImonAddress (
  VOID
  )
{
  VCC_IMON *VccImon = NULL;
  UINTN Size = 0;

  Size = sizeof (VCC_IMON);
  VccImon = (VCC_IMON *) PcdGetPtr (PcdImonAddr);
  if (VccImon == NULL) {
    DEBUG ((EFI_D_ERROR, "UpdateImonAddress() - PcdImonAddr == NULL\n"));
    return;
  }

  VccImon->VrSvid[0] = PcdGet8 (PcdArcherCitySvidVrVccfaEhvFivr);
  VccImon->VrSvid[1] = IMON_ADDR_LIST_END; // End array with 0xFF

  PcdSetPtrS (PcdImonAddr, &Size, (VOID *) VccImon);
}

EFI_STATUS
TypeBigPineKeyPlatformPcdUpdateCallback (
  VOID
  )
{
  CHAR8     FamilyName[]  = "EagleStream";

  CHAR8     BoardName[]   = "EPRP";
  UINT32    Data32;
  UINTN     Size;
  UINTN     PlatformFeatureFlag = 0;

  CHAR16    PlatformName[]   = L"TypeBigPineKey";
  UINTN     PlatformNameSize = 0;
  EFI_STATUS Status;

  //#Integer for BoardID, must match the SKU number and be unique.
  Status = PcdSet16S (PcdOemSkuBoardID                      , TypeBigPineKey);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  Status = PcdSet16S (PcdOemSkuBoardFamily                  , 0x30);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //# Number of Sockets on Board.
  Status = PcdSet32S (PcdOemSkuBoardSocketCount, 2);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  // Max channel and max DIMM
  Status = PcdSet32S (PcdOemSkuMaxChannel , 8);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  Status = PcdSet32S (PcdOemSkuMaxDimmPerChannel , 1);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  Status = PcdSetBoolS (PcdOemSkuDimmLayout, TRUE);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //#
  //# Misc.
  //#
  //# V_PCIE_PORT_PXPSLOTCTRL_ATNLED_OFF
  Status = PcdSet16S (PcdOemSkuMrlAttnLed                   , 0xc0);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //SDP Active Flag
  Status = PcdSet8S (PcdOemSkuSdpActiveFlag                , 0x0);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //# Zero terminated string to ID family
  Size = AsciiStrSize (FamilyName);
  Status = PcdSetPtrS (PcdOemSkuFamilyName             , &Size, FamilyName);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //# Zero terminated string to Board Name
  Size = AsciiStrSize (BoardName);
  Status = PcdSetPtrS (PcdOemSkuBoardName              , &Size, BoardName);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  PlatformNameSize = sizeof (PlatformName);
  Status = PcdSet32S (PcdOemSkuPlatformNameSize           , (UINT32)PlatformNameSize);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  Status = PcdSetPtrS (PcdOemSkuPlatformName              , &PlatformNameSize, PlatformName);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //# FeaturesBasedOnPlatform
  PlatformFeatureFlag |= BIOSGUARD_SUPPORT_ENABLED;
  Status = PcdSet32S (PcdOemSkuPlatformFeatureFlag           , (UINT32)PlatformFeatureFlag);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  //# Assert GPIO
  Data32 = 0;
  Status = PcdSet32S (PcdOemSkuAssertPostGPIOValue, Data32);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  Status = PcdSet32S (PcdOemSkuAssertPostGPIO, GPIO_VER3_GPP_C17);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //# UplinkPortIndex
  Status = PcdSet8S (PcdOemSkuUplinkPortIndex, 5);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  DEBUG ((EFI_D_INFO, "Uba Callback: PlatformPcdUpdateCallback is called!\n"));
  Status = TypeBigPineKeyPlatformUpdateAcpiTablePcds ();
  //# BMC Pcie Port Number
  PcdSet8S (PcdOemSkuBmcPciePortNumber, 5);
  ASSERT_EFI_ERROR(Status);

  //# Board Type Bit Mask
  PcdSet32S (PcdBoardTypeBitmask, CPU_TYPE_F_MASK | (CPU_TYPE_F_MASK << 4));
  ASSERT_EFI_ERROR(Status);

  //Update IMON Address
  TypeBigPineKeyPlatformUpdateImonAddress ();

  return Status;
}

PLATFORM_PCD_UPDATE_TABLE    TypeBigPineKeyPcdUpdateTable =
{
  PLATFORM_PCD_UPDATE_SIGNATURE,
  PLATFORM_PCD_UPDATE_VERSION,
  TypeBigPineKeyPlatformPcdUpdateCallback
};

EFI_STATUS
TypeBigPineKeyInstallPcdData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
  )
{
  EFI_STATUS                            Status;

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformPcdConfigDataGuid,
                                 &TypeBigPineKeyPcdUpdateTable,
                                 sizeof(TypeBigPineKeyPcdUpdateTable)
                                 );

  return Status;
}

