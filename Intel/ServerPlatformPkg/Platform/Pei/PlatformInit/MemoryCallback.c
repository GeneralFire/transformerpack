/** @file
  EFI 2.0 PEIM to provide the platform support functionality on the Bridgeport.

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

#include "PlatformInitPreMem.h"
#include <Guid/FirmwareFileSystem3.h>
#include <Guid/SetupVariable.h>
#include <Guid/MemoryMapData.h>
#include <Ppi/AtaPolicy.h>
#include <Library/MtrrLib.h>

#include <Cpu/CpuCoreRegs.h>
#include <Register/Cpuid.h>
#include <FlashMap.h>
#include <Library/CpuAndRevisionLib.h>

#include "FitAccess.h"

#define FLASH_REGION_OPROM_FV_BASE  FixedPcdGet32(PcdFlashFvOpromBase)
#define FLASH_REGION_OPROM_FV_SIZE  FixedPcdGet32(PcdFlashFvOpromSize)

#define FLASH_REGION_FV_OA_BASE     FixedPcdGet32(PcdFlashFvOaBase);
#define FLASH_REGION_FV_OA_SIZE     FixedPcdGet32(PcdFlashFvOaSize);

#define HASHED_FVMAIN_FV_NUM        1
#define HASHED_MICROCODE_FV_NUM     1
#define HASHED_OEM_FV_NUM           1
#define HASHED_OPROM_FV_NUM         1
#define HASHED_FV_POSTMEM_NUM       ((PcdGetBool (PcdObbVerificationPostMemEnable) == TRUE)? 1 : 0)
#define HASHED_FV_FSPS_NUM          ((PcdGetBool (PcdObbVerificationFspsFVEnable) == TRUE)? 1 : 0)
#define HASHED_OA_FV_NUM            ((PcdGetBool (PcdObbVerificationOaFVEnable) == TRUE)? 1 : 0)
#define HASHED_ADDITIONAL_FV_NUM    (HASHED_FVMAIN_FV_NUM +\
                                     HASHED_FV_POSTMEM_NUM +\
                                     HASHED_FV_FSPS_NUM +\
                                     HASHED_MICROCODE_FV_NUM +\
                                     HASHED_OEM_FV_NUM +\
                                     HASHED_OA_FV_NUM +\
                                     HASHED_OPROM_FV_NUM \
                                    )

extern EFI_GUID      gEfiPeiCpuCpxPpiGuid;
extern EFI_GUID      gEfiPeiCpuIcxPpiGuid;

static EFI_PEI_FIRMWARE_VOLUME_INFO_PPI mFvMainInfoPpi = {
  EFI_FIRMWARE_FILE_SYSTEM3_GUID,
  (VOID*)FLASH_REGION_FVMAIN_BASE,
  FLASH_REGION_FVMAIN_SIZE,
  NULL,
  NULL
};

static EFI_PEI_PPI_DESCRIPTOR     mPpiListMain = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiFirmwareVolumeInfoPpiGuid,
  &mFvMainInfoPpi
};

/**

  PEI termination callback.

  @param PeiServices       -  General purpose services available to every PEIM.
  @param NotifyDescriptor  -  Not uesed.
  @param Ppi               -  Not uesed.

  @retval EFI_SUCCESS  -  If the interface could be successfully
                  @retval installed.

**/
EFI_STATUS
EndOfPeiPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  IIO_UDS               *IioUds;

  GetIioUdsHob(PeiServices, &IioUds);

  if (IioUds != NULL ) {

    DEBUG ((EFI_D_INFO, "Memory TOLM: %X\n", IioUds->PlatformData.MemTolm));
    DEBUG (
      (EFI_D_INFO,
      "PCIE   BASE: %lX     Size : %X\n",
      IioUds->PlatformData.PciExpressBase,
      IioUds->PlatformData.PciExpressSize)
      );
    DEBUG (
      (EFI_D_INFO,
      "PCI32  BASE: %X     Limit: %X\n",
      IioUds->PlatformData.PlatGlobalMmio32Base,
      IioUds->PlatformData.PlatGlobalMmio32Limit)
      );
    DEBUG (
      (EFI_D_INFO,
      "PCI64  BASE: %lX     Limit: %lX\n",
      IioUds->PlatformData.PlatGlobalMmio64Base,
      IioUds->PlatformData.PlatGlobalMmio64Limit)
      );
    DEBUG ((EFI_D_INFO, "UC    START: %lX     End  : %lX\n", IioUds->PlatformData.PlatGlobalMmio64Base, (IioUds->PlatformData.PlatGlobalMmio64Limit + 1)));
  } else {
    DEBUG ((EFI_D_ERROR, "Error: IioUds = NULL\n"));
  }

  return EFI_SUCCESS;
}

/**

    GC_TODO: add routine description

    @param PeiServices  - GC_TODO: add arg description
    @param VolumeHandle - GC_TODO: add arg description

    @retval None

**/
VOID
EFIAPI
InstallFvInfoPpi (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_FV_HANDLE           VolumeHandle
  )
{
  EFI_STATUS                     Status;
  EFI_PEI_PPI_DESCRIPTOR         *mPpiList;
  PEI_FV_INFO_PPI_PRIVATE        *FvInfoInstance;

  //
  // Prepare to install FirmwareVolumeInfo PPI to expose new FV to PeiCore.
  //
  FvInfoInstance =  AllocatePool (sizeof (PEI_FV_INFO_PPI_PRIVATE));

  if (FvInfoInstance != NULL) {


    mPpiList = &FvInfoInstance->PpiList;

    CopyMem (
      &FvInfoInstance->FvInfoPpi.FvFormat,
      &(((EFI_FIRMWARE_VOLUME_HEADER*)VolumeHandle)->FileSystemGuid),
      sizeof (EFI_GUID)
     );

    FvInfoInstance->FvInfoPpi.FvInfo = VolumeHandle;
    FvInfoInstance->FvInfoPpi.FvInfoSize = (UINT32)((EFI_FIRMWARE_VOLUME_HEADER*)VolumeHandle)->FvLength;
    FvInfoInstance->FvInfoPpi.ParentFvName = NULL;
    FvInfoInstance->FvInfoPpi.ParentFileName = NULL;

    //
    // Install FirmwareVolumeInfo PPI to export new Firmware Volume to Core.
    //
    mPpiList->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
    mPpiList->Guid  = &gEfiPeiFirmwareVolumeInfoPpiGuid;
    mPpiList->Ppi   = &FvInfoInstance->FvInfoPpi;
    Status          = (**PeiServices).InstallPpi (PeiServices, mPpiList);
    ASSERT_EFI_ERROR (Status);
  } else {
    DEBUG ((EFI_D_ERROR, "Error: FvInfoInstance = NULL\n"));
    ASSERT(FvInfoInstance);
  }

}

EFI_STATUS
CreateStoredHashFvPpi (
  IN EFI_BOOT_MODE                                  BootMode,
  IN UINT64                                         FvMainBase
  )
{
// APTIOV_SERVER_OVERRIDE_RC_START: OOB not supported
#if 0  
  EDKII_PEI_FIRMWARE_VOLUME_INFO_STORED_HASH_FV_PPI *StoredHashFvPpi;
  UINTN                                             FvNumber;
  HASH_STRUCTURE                                    *ObbHash;
  UINT64                                            FvFlag;
  EFI_STATUS                                        Status;

  StoredHashFvPpi = NULL;
  FvNumber        = 0;
  ObbHash         = NULL;
  FvFlag          = 0;
  Status          = EFI_SUCCESS;

  StoredHashFvPpi = AllocateZeroPool (
                    sizeof(EDKII_PEI_FIRMWARE_VOLUME_INFO_STORED_HASH_FV_PPI)
                    + sizeof(HASHED_FV_INFO) * 4);
  if (StoredHashFvPpi == NULL) {
    DEBUG ((EFI_D_ERROR, "\tFailing to allocate resources for StoredHashFvPpi\n"));
    Status = EFI_OUT_OF_RESOURCES;
    return Status;
  }
  // FVs taken into account (or hashed) for the OBB verification, must match the ones used by the BpmGen2 tool in the BPM generation
  FvFlag = GetBaseHashFvFlag ();
  StoredHashFvPpi->FvInfo[FvNumber].Base    = FLASH_REGION_FV_SECPEI_BASE;
  StoredHashFvPpi->FvInfo[FvNumber].Length  = FLASH_REGION_FV_SECPEI_SIZE;
  StoredHashFvPpi->FvInfo[FvNumber].Flag    = HASHED_FV_FLAG_MEASURED_BOOT |
                                              HASHED_FV_FLAG_REPORT_FV_HOB |
                                              HASHED_FV_FLAG_SKIP_ALL;
  FvNumber++;
  StoredHashFvPpi->FvInfo[FvNumber].Base    = FvMainBase;
  StoredHashFvPpi->FvInfo[FvNumber].Length  = FLASH_REGION_FVMAIN_SIZE;
  StoredHashFvPpi->FvInfo[FvNumber].Flag    = FvFlag |
                                              HASHED_FV_FLAG_REPORT_FV_HOB |
                                              HASHED_FV_FLAG_REPORT_FV_INFO_PPI |
                                              HASHED_FV_FLAG_SKIP_BOOT_MODE(BOOT_ON_S3_RESUME) |
                                              HASHED_FV_FLAG_SKIP_BOOT_MODE(BOOT_IN_RECOVERY_MODE);
  FvNumber++;
  if (PcdGetBool (PcdObbVerificationPostMemEnable) == TRUE) {
    // [FV.FvPostMemorySilicon]
    StoredHashFvPpi->FvInfo[FvNumber].Base    = FLASH_REGION_FV_POST_MEMORY_BASE;
    StoredHashFvPpi->FvInfo[FvNumber].Length  = FLASH_REGION_FV_POST_MEMORY_SIZE;
    StoredHashFvPpi->FvInfo[FvNumber].Flag    = FvFlag |
                                                HASHED_FV_FLAG_REPORT_FV_HOB |
                                                HASHED_FV_FLAG_SKIP_BOOT_MODE(BOOT_ON_S3_RESUME) |
                                                HASHED_FV_FLAG_SKIP_BOOT_MODE(BOOT_IN_RECOVERY_MODE);
    FvNumber++;
  }
  // MICROCODE_FV is skipped because is already verified by the Microcode ROM
  StoredHashFvPpi->FvInfo[FvNumber].Base    = FLASH_REGION_MICROCODE_FV_BASE;
  StoredHashFvPpi->FvInfo[FvNumber].Length  = FLASH_REGION_MICROCODE_FV_SIZE;
  StoredHashFvPpi->FvInfo[FvNumber].Flag    = FvFlag |
                                              HASHED_FV_FLAG_REPORT_FV_HOB |
                                              HASHED_FV_FLAG_SKIP_ALL;
  FvNumber++;
  if (PcdGetBool (PcdObbVerificationFspsFVEnable) == TRUE) {
    // [FV.FVFspS]
    StoredHashFvPpi->FvInfo[FvNumber].Base    = FLASH_REGION_FV_FSPS_BASE;
    StoredHashFvPpi->FvInfo[FvNumber].Length  = FLASH_REGION_FV_FSPS_SIZE;
    StoredHashFvPpi->FvInfo[FvNumber].Flag    = FvFlag |
                                                HASHED_FV_FLAG_REPORT_FV_HOB |
                                                HASHED_FV_FLAG_REPORT_FV_INFO_PPI |
                                                HASHED_FV_FLAG_SKIP_BOOT_MODE(BOOT_ON_S3_RESUME) |
                                                HASHED_FV_FLAG_SKIP_BOOT_MODE(BOOT_IN_RECOVERY_MODE);
    FvNumber++;
  }
  StoredHashFvPpi->FvInfo[FvNumber].Base    = FLASH_REGION_OEM_FV_BASE;
  StoredHashFvPpi->FvInfo[FvNumber].Length  = FLASH_REGION_OEM_FV_SIZE;
  StoredHashFvPpi->FvInfo[FvNumber].Flag    = FvFlag |
                                              HASHED_FV_FLAG_REPORT_FV_HOB |
                                              HASHED_FV_FLAG_SKIP_BOOT_MODE(BOOT_ON_S3_RESUME);
  FvNumber++;
  StoredHashFvPpi->FvInfo[FvNumber].Base    = FLASH_REGION_OPROM_FV_BASE;
  StoredHashFvPpi->FvInfo[FvNumber].Length  = FLASH_REGION_OPROM_FV_SIZE;
  StoredHashFvPpi->FvInfo[FvNumber].Flag    = FvFlag |
                                              HASHED_FV_FLAG_REPORT_FV_HOB |
                                              HASHED_FV_FLAG_SKIP_ALL;
  FvNumber++;
  if (PcdGetBool (PcdObbVerificationOaFVEnable) == TRUE) {
    // [FV.OA_FV]
    StoredHashFvPpi->FvInfo[FvNumber].Base    = FLASH_REGION_FV_OA_BASE;
    StoredHashFvPpi->FvInfo[FvNumber].Length  = FLASH_REGION_FV_OA_SIZE;
    StoredHashFvPpi->FvInfo[FvNumber].Flag    = FvFlag |
                                                HASHED_FV_FLAG_REPORT_FV_HOB |
                                                HASHED_FV_FLAG_REPORT_FV_INFO_PPI |
                                                HASHED_FV_FLAG_SKIP_BOOT_MODE(BOOT_ON_S3_RESUME) |
                                                HASHED_FV_FLAG_SKIP_BOOT_MODE(BOOT_IN_RECOVERY_MODE);
    FvNumber++;
  }
  // [FV.FVWHEA] Should not be checked as this FV is allowed to be changed
  StoredHashFvPpi->FvNumber = FvNumber;
  ObbHash = FindObbHash (BootMode);
  Status = InstallStoredHashFvPpi (StoredHashFvPpi, ObbHash, BootMode);
  if (Status != EFI_SUCCESS) {
    DEBUG ((EFI_D_ERROR, "\tFailing to install StoredHashFvPpi\n"));
    ASSERT_EFI_ERROR (Status);
  }
#endif   

  return EFI_SUCCESS;
  // APTIOV_SERVER_OVERRIDE_RC_END: OOB not supported
}

/**
  Find USED_SIZE FV_EXT_TYPE entry in FV extension header and get the FV used size.

  @param[in]  FvHeader      Pointer to FV header.
  @param[out] FvUsedSize    Pointer to FV used size returned,
                            only valid if USED_SIZE FV_EXT_TYPE entry is found.
  @param[out] EraseByte     Pointer to erase byte returned,
                            only valid if USED_SIZE FV_EXT_TYPE entry is found.

  @retval TRUE              USED_SIZE FV_EXT_TYPE entry is found,
                            FV used size and erase byte are returned.
  @retval FALSE             No USED_SIZE FV_EXT_TYPE entry found.

**/
BOOLEAN
GetFvUsedSize (
  IN EFI_FIRMWARE_VOLUME_HEADER     *FvHeader,
  OUT UINT32                        *FvUsedSize,
  OUT UINT8                         *EraseByte
  )
{
  UINT16                                        ExtHeaderOffset;
  EFI_FIRMWARE_VOLUME_EXT_HEADER                *ExtHeader;
  EFI_FIRMWARE_VOLUME_EXT_ENTRY                 *ExtEntryList;
  EFI_FIRMWARE_VOLUME_EXT_ENTRY_USED_SIZE_TYPE  *ExtEntryUsedSize;

  ExtHeaderOffset = ReadUnaligned16 (&FvHeader->ExtHeaderOffset);
  if (ExtHeaderOffset != 0) {
    ExtHeader    = (EFI_FIRMWARE_VOLUME_EXT_HEADER *) ((UINT8 *) FvHeader + ExtHeaderOffset);
    ExtEntryList = (EFI_FIRMWARE_VOLUME_EXT_ENTRY *) (ExtHeader + 1);
    while ((UINTN) ExtEntryList < ((UINTN) ExtHeader + ReadUnaligned32 (&ExtHeader->ExtHeaderSize))) {
      if (ReadUnaligned16 (&ExtEntryList->ExtEntryType) == EFI_FV_EXT_TYPE_USED_SIZE_TYPE) {
        //
        // USED_SIZE FV_EXT_TYPE entry is found.
        //
        ExtEntryUsedSize = (EFI_FIRMWARE_VOLUME_EXT_ENTRY_USED_SIZE_TYPE *) ExtEntryList;
        *FvUsedSize = ReadUnaligned32 (&ExtEntryUsedSize->UsedSize);
        if ((ReadUnaligned32 (&FvHeader->Attributes) & EFI_FVB2_ERASE_POLARITY) != 0) {
          *EraseByte = 0xFF;
        } else {
          *EraseByte = 0;
        }
        DEBUG ((
          DEBUG_INFO,
          "FV at 0x%x has 0x%x used size, and erase byte is 0x%02x\n",
          FvHeader,
          *FvUsedSize,
          *EraseByte
          ));
        return TRUE;
      }
      ExtEntryList = (EFI_FIRMWARE_VOLUME_EXT_ENTRY *)
                     ((UINT8 *) ExtEntryList + ReadUnaligned16 (&ExtEntryList->ExtEntrySize));
    }
  }

  //
  // No USED_SIZE FV_EXT_TYPE entry found.
  //
  return FALSE;
}

/**
  Shadow the given FV into memory.

  @param[in]  FvBase          Base address of the FV on flash.
  @param[in]  FvSize          Length of the FV on flash.
  @param[out] FvShadowBase    Base address of the FV after being successfully
                              shadowed into memory.

  @retval EFI_SUCCESS         The given FV is shadowed into memory.
  @retval others              The given FV is not shadowed.

**/
EFI_STATUS
ShadowFvToMemory (
  IN  UINTN     FvBase,
  IN  UINTN     FvSize,
  OUT UINT64    *FvShadowBase
  )
{
  EFI_STATUS              Status;
  UINT32                  FvUsedSize;
  UINT8                   EraseByte;
  EFI_PHYSICAL_ADDRESS    FvShadowPhyAddr;
  UINT8                   *FvShadowPtr;

  Status = PeiServicesAllocatePages (
             EfiBootServicesCode,
             EFI_SIZE_TO_PAGES (FvSize),
             (EFI_PHYSICAL_ADDRESS *) &FvShadowPhyAddr
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  FvShadowPtr = (UINT8 *)(UINTN) FvShadowPhyAddr;
  if (FvShadowPtr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Shadow the FV into memory if allocation succeeds
  //
  if (GetFvUsedSize (
        (EFI_FIRMWARE_VOLUME_HEADER *) FvBase,
        &FvUsedSize,
        &EraseByte)
        ) {
    ASSERT (FvSize >= FvUsedSize);
    if (FvSize < FvUsedSize) {
      PeiServicesFreePages (FvShadowPhyAddr, EFI_SIZE_TO_PAGES (FvSize));
      return EFI_VOLUME_CORRUPTED;
    }

    CopyMem (FvShadowPtr, (VOID *) FvBase, FvUsedSize);
    SetMem  (FvShadowPtr + FvUsedSize, FvSize - FvUsedSize, EraseByte);

    DEBUG ((
      DEBUG_INFO,
      "%a: FvUsedSize = 0x%x, EraseByte = 0x%x.\n",
      __FUNCTION__, FvUsedSize, EraseByte
      ));
  } else {
    CopyMem (FvShadowPtr, (VOID *) FvBase, FvSize);
  }

  *FvShadowBase = FvShadowPhyAddr;
  return EFI_SUCCESS;
}

/**

  Install Firmware Volume Hob's once there is main memory

  @param PeiServices       General purpose services available to every PEIM.
  @param NotifyDescriptor  Notify that this module published.
  @param Ppi               PPI that was installed.

  @retval EFI_SUCCESS     The function completed successfully.

**/
EFI_STATUS
EFIAPI
SiliconRcHobsReadyPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                    Status;
  EFI_BOOT_MODE                 BootMode;
  UINT64                        FlashBase;
  UINT64                        FlashSize;
  UINT64                        LowUncableBase;
  EFI_CPUID_REGISTER            FeatureInfo;
  UINT8                         CpuAddressWidth;
  UINT16                        Pm1Cnt;
  IIO_UDS                       *IioUds;
  EFI_PLATFORM_INFO             *PlatformInfo;
  EFI_HOB_GUID_TYPE             *GuidHob;
  MTRR_SETTINGS                 MtrrSettings;
// APTIOV_SERVER_OVERRIDE_RC_START : Do not build flash HOB's here. Flash region will be reserved through RSVChipset. 
  //EFI_PEI_PPI_DESCRIPTOR      *PpiListCpu;
  UINT64                        FvMainBase;

  GetIioUdsHob (PeiServices, &IioUds);

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  PlatformInfo  = GET_GUID_HOB_DATA(GuidHob);

  //
  // Check if user wants to turn off in PEI phase
  //
  if (BootMode != BOOT_ON_S3_RESUME) {
    CheckPowerOffNow ();
  } else {
    Pm1Cnt  = IoRead16 (PCH_ACPI_BASE_ADDRESS + R_ACPI_IO_PM1_CNT);
    Pm1Cnt &= ~B_ACPI_IO_PM1_CNT_SLP_TYP;
    IoWrite16 (PCH_ACPI_BASE_ADDRESS + R_ACPI_IO_PM1_CNT, Pm1Cnt);
  }

  //
  // Set initial MTRRs for under 4GB
  //
  ZeroMem (&MtrrSettings, sizeof (MTRR_SETTINGS));

  //
  // Set fixed cache for memory range below 1MB
  //
  Status = MtrrSetMemoryAttributeInMtrrSettings (
    &MtrrSettings,
    0,
    0xA0000,
    EFI_CACHE_WRITEBACK
    );
  ASSERT_EFI_ERROR (Status);

  Status = MtrrSetMemoryAttributeInMtrrSettings (
    &MtrrSettings,
    0xA0000,
    0x60000,
    EFI_CACHE_UNCACHEABLE
    );
  ASSERT_EFI_ERROR (Status);

  if (IioUds != NULL) {
    if (IioUds->PlatformData.PciExpressBase < IioUds->PlatformData.PlatGlobalMmio32Base) {
      LowUncableBase = IioUds->PlatformData.PciExpressBase;
    } else {
      LowUncableBase = IioUds->PlatformData.PlatGlobalMmio32Base;
    }
    LowUncableBase &= (0x0FFF00000);

    //
    // Cache memory under 4GB to WB
    //
    Status = MtrrSetMemoryAttributeInMtrrSettings (
      &MtrrSettings,
      BASE_1MB,
      LowUncableBase - BASE_1MB,
      EFI_CACHE_WRITEBACK
      );
  } else {
    DEBUG ((EFI_D_ERROR, "Error: IioUds = NULL\n"));
    ASSERT (IioUds);
  }

  //
  // Set IMR2 to be un-cachable if enabled
  //
  if (PcdGetBool (PcdImr2Enable) == TRUE && PcdGet64 (PcdImr2Size) != 0) {
    //
    // Get IMR2 Base Address HOB if it was published
    //
    GuidHob = GetFirstGuidHob (&gImr2BaseAddressHobGuid);
    if (GuidHob != NULL) {
      Status = MtrrSetMemoryAttributeInMtrrSettings (
        &MtrrSettings,
        (EFI_PHYSICAL_ADDRESS)(*(EFI_PHYSICAL_ADDRESS *)GET_GUID_HOB_DATA (GuidHob)),
        (UINT64)PcdGet64 (PcdImr2Size),
        EFI_CACHE_UNCACHEABLE
        );
      ASSERT_EFI_ERROR (Status);    // Assert if IMR2 enabled and no MTRR available to set it to UC.
    }
  }

#if ME_SUPPORT_FLAG
  //
  // Set MESEG memory range to be un-cachable if MESEG enabled
  //
  if( PlatformInfo->MemData.MemMESEGSize != 0 ) {
    Status = MtrrSetMemoryAttributeInMtrrSettings (
      &MtrrSettings,
      (EFI_PHYSICAL_ADDRESS)PlatformInfo->MemData.MemMESEGBase,
      (UINT64)PlatformInfo->MemData.MemMESEGSize,
      EFI_CACHE_UNCACHEABLE
      );
    ASSERT_EFI_ERROR (Status);    // assert if MESEG enabled and no MTRR available to set it to UC.
  }
#endif //ME_SUPPORT_FLAG

#if IE_SUPPORT
  //
  // Set IESEG memory range to be un-cachable if MESEG enabled
  //
  if( PlatformInfo->MemData.MemIESEGSize != 0 ) {
    Status = MtrrSetMemoryAttributeInMtrrSettings (
      &MtrrSettings,
      (EFI_PHYSICAL_ADDRESS)PlatformInfo->MemData.MemIESEGBase,
      (UINT64)PlatformInfo->MemData.MemIESEGSize,
      EFI_CACHE_UNCACHEABLE
      );
    ASSERT_EFI_ERROR (Status);    // assert if IESEG enabled and no MTRR available to set it to UC.
  }
#endif // IE_SUPPORT


  //
  // Set flash region to WP until end of PEI
  //
// APTIOV_SERVER_OVERRIDE_RC_START : Use FLASH SDL tokens
#if 0
  FlashBase = MAX (FixedPcdGet32 (PcdFlashFdMainBase), (BASE_4GB - SIZE_16MB));
  FlashSize = (UINT64)EFI_MAX_ADDRESS - FlashBase + 1;
#else
  FlashBase = PCH_BIOS_RESERVED_REGION_BASE;
  FlashSize = PCH_BIOS_RESERVED_REGION_SIZE;
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Use FLASH SDL tokens
  Status = MtrrSetMemoryAttributeInMtrrSettings (
    &MtrrSettings,
    FlashBase,
    FlashSize,
    EFI_CACHE_WRITEPROTECTED
    );
  ASSERT_EFI_ERROR (Status);


  MtrrSetAllMtrrs (&MtrrSettings);

  //
  // Create the firmware volume HOB's
  //
  if (BootMode != BOOT_IN_RECOVERY_MODE) {
    DEBUG ((EFI_D_INFO, "=======================\n"));
    DEBUG ((EFI_D_INFO, " Boot in Normal mode\n"));
    DEBUG ((EFI_D_INFO, "=======================\n"));

    FvMainBase = FLASH_REGION_FVMAIN_BASE;
    if (PcdGetBool (PcdShadowFvMainInMemory)) {
      //
      // Shadow FVMAIN into memory
      //
      Status = ShadowFvToMemory (FLASH_REGION_FVMAIN_BASE, FLASH_REGION_FVMAIN_SIZE, &FvMainBase);
      if (EFI_ERROR (Status)) {
        DEBUG ((
          DEBUG_ERROR,
          "%a: Fail to shadow FVMAIN into memory, will use the one in Flash.\n",
          __FUNCTION__
          ));
      }
    }
    DEBUG ((DEBUG_INFO, "%a: FvMainBase = 0x%lx.\n", __FUNCTION__, FvMainBase));

    if (PcdGetBool (PcdObbVerificationEnable) == TRUE) {
      Status = CreateStoredHashFvPpi (BootMode, FvMainBase);
      if (Status != EFI_SUCCESS) {
        DEBUG ((EFI_D_ERROR, "\tFailing to create or install StoredHashFvPpi\n"));
        ASSERT_EFI_ERROR(Status);
        return Status;
      }
    }

// APTIOV_SERVER_OVERRIDE_RC_START : Do not build flash HOB's here. Flash region will be reserved through RSVChipset.     
#if 0
    BuildFvHob (
      FLASH_REGION_FV_SECPEI_BASE,
      FLASH_REGION_FV_SECPEI_SIZE
      );

    BuildFvHob (
      FvMainBase,
      FLASH_REGION_FVMAIN_SIZE
      );

    PpiListCpu = AllocatePool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
    if (PpiListCpu == NULL) {
      ASSERT (!(PpiListCpu == NULL));
      return EFI_OUT_OF_RESOURCES;
    } else {
      PpiListCpu->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
      PpiListCpu->Ppi = NULL;
      if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
        PpiListCpu->Guid = &gEfiPeiCpuCpxPpiGuid;
        Status = (**PeiServices).InstallPpi (PeiServices, PpiListCpu);
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR (Status)) {
          FreePool (PpiListCpu);
        }
      } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
        PpiListCpu->Guid = &gEfiPeiCpuIcxPpiGuid;
        Status = (**PeiServices).InstallPpi (PeiServices, PpiListCpu);
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR (Status)) {
          FreePool (PpiListCpu);
        }
      }
    }

    if (FvMainBase == FLASH_REGION_FVMAIN_BASE) {
      //
      // FVMAIN not shadowed into memory
      //
      Status = (**PeiServices).InstallPpi (PeiServices, &mPpiListMain);
      ASSERT_EFI_ERROR (Status);
    } else {
      //
      // FVMAIN shadowed into memory
      //
      InstallFvInfoPpi (PeiServices, (EFI_PEI_FV_HANDLE)(UINTN) FvMainBase);
    }

    BuildFvHob (
      FLASH_REGION_MICROCODE_FV_BASE,
      FLASH_REGION_MICROCODE_FV_SIZE
      );

    BuildFvHob (
      FLASH_REGION_OEM_FV_BASE,
      FLASH_REGION_OEM_FV_SIZE
      );

    BuildFvHob (
      FLASH_REGION_OPROM_FV_BASE,
      FLASH_REGION_OPROM_FV_SIZE
      );
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Do not build flash HOB's here. Flash region will be reserved through RSVChipset.     
  } else {
    DEBUG ((EFI_D_INFO, "=======================\n"));
    DEBUG ((EFI_D_INFO, " Boot in Recovery mode\n"));
    DEBUG ((EFI_D_INFO, "=======================\n"));

    //
    // Prepare the recovery service
    //
    Status = PeimInitializeRecovery (PeiServices);
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Do not need to build HOB for NV Storage FV
  //
  /*BuildFvHob (
    FLASH_REGION_RUNTIME_UPDATABLE_BASE,
    (FLASH_REGION_RUNTIME_UPDATABLE_SIZE + FLASH_REGION_NV_FTW_SPARE_SIZE)
    );*/
// APTIOV_SERVER_OVERRIDE_RC_START : Do not build flash HOB's here. Flash region will be reserved through RSVChipset.   
#if 0
  BuildResourceDescriptorHob (
    EFI_RESOURCE_FIRMWARE_DEVICE,
    (EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
    FLASH_BASE,
    FLASH_SIZE
    );
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Do not build flash HOB's here. Flash region will be reserved through RSVChipset. 
  //
  // Create a CPU hand-off information
  //
  CpuAddressWidth = 32;
  AsmCpuid (CPUID_EXTENDED_FUNCTION, &FeatureInfo.RegEax, NULL, NULL, NULL);
  if (FeatureInfo.RegEax >= CPUID_VIR_PHY_ADDRESS_SIZE) {
    AsmCpuid (CPUID_VIR_PHY_ADDRESS_SIZE, &FeatureInfo.RegEax, NULL, NULL, NULL);
    CpuAddressWidth = (UINT8) (FeatureInfo.RegEax & 0xFF);
  }

  DEBUG ((EFI_D_INFO, "CpuAddressWidth: %d\n", CpuAddressWidth));

  BuildCpuHob (CpuAddressWidth, 16);

  // APTIOV_SERVER_OVERRIDE_RC_START : Do not build flash HOB's here. Flash region will be reserved through RSVChipset.
#if 0
  if (PpiListCpu != NULL) {
    FreePool (PpiListCpu);
  }
#endif
  // APTIOV_SERVER_OVERRIDE_RC_START : Do not build flash HOB's here. Flash region will be reserved through RSVChipset.

  return Status;
}

/**

  Check if the files in the Backup Blocks are valid.

  @param PeiServices  -  General purpose services available to every PEIM.
  @param Address      -  Address of the backup block.

  @retval TRUE   -  If all the files are valid.
  @retval FALSE  -  If one the files are invalid.

**/
BOOLEAN
CheckIfFvIsValid (
  IN EFI_PEI_SERVICES       **PeiServices,
  IN UINTN                  Address
  )
{
  return TRUE;
}

/**

    GC_TODO: add routine description

    @param FwVolHeader - GC_TODO: add arg description

    @retval EFI_NOT_FOUND - GC_TODO: add retval description
    @retval EFI_SUCCESS   - GC_TODO: add retval description

**/
EFI_STATUS
ValidateFvHeader (
  IN EFI_FIRMWARE_VOLUME_HEADER            *FwVolHeader
  )
{
  UINT16  *Ptr;
  UINT16  HeaderLength;
  UINT16  Checksum;

  //
  // Verify the header revision, header signature, length
  // Length of FvBlock cannot be 2**64-1
  // HeaderLength cannot be an odd number
  //
  if ((FwVolHeader->Revision != EFI_FVH_REVISION) ||
      (FwVolHeader->Signature != EFI_FVH_SIGNATURE) ||
      (FwVolHeader->FvLength == ((UINT64) -1)) ||
      ((FwVolHeader->HeaderLength & 0x01) != 0)
      ) {
    return EFI_NOT_FOUND;
  }
  //
  // Verify the header checksum
  //
  HeaderLength  = (UINT16) (FwVolHeader->HeaderLength / 2);
  Ptr           = (UINT16 *) FwVolHeader;
  Checksum      = 0;
  while (HeaderLength > 0) {
    Checksum = *Ptr++;
    HeaderLength--;
  }

  if (Checksum != 0) {
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

