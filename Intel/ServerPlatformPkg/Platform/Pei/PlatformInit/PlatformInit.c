/** @file
  Source code file for Platform Init PEI module

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include <Library/PeiPolicyInitLib.h>
#include <Library/ConfigBlockLib.h>
#include <FirmwareConfigurations.h>
#include <PchSetupVariable.h>
#include <Library/DebugLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiServicesLib.h>
#include <Library/PchInfoLib.h>
#include <Library/BaseMemoryLib.h>
#include <PchPreMemPolicyCommon.h>
#include <Ppi/SiPolicy.h>

#ifdef CDF_SC_FLAG
#include <Library/PmcLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#endif  // #ifdef CDF_SC_FLAG

#define GET_OCCUPIED_SIZE(ActualSize, Alignment) \
  ((ActualSize) + (((Alignment) - ((ActualSize) & ((Alignment) - 1))) & ((Alignment) - 1)))

/**
  This function handles PlatformInit task at the end of PEI

  @param[in]  PeiServices  Pointer to PEI Services Table.
  @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                           caused this function to execute.
  @param[in]  Ppi          Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/
EFI_STATUS
EFIAPI
PlatformInitEndOfPei (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mEndOfPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) PlatformInitEndOfPei
};



/**
  This function handles PlatformInit task at the end of PEI

  @param[in]  PeiServices  Pointer to PEI Services Table.
  @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                           caused this function to execute.
  @param[in]  Ppi          Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/
EFI_STATUS
EFIAPI
PlatformInitEndOfPei (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS     Status = EFI_SUCCESS;

  return Status;
}


/**
  Get the BIOS Guard Module pointer.

  @param[in, out] ModulePtr - Input is a NULL pointer,
                              and output points BIOS Guard module address if found.

  @retval EFI_SUCCESS       - BIOS Guard Module found.
  @retval EFI_NOT_FOUND     - BIOS Guard Module not found.
**/
EFI_STATUS
UpdateBiosGuardModulePtr (
  IN OUT EFI_PHYSICAL_ADDRESS   *ModulePtr
  )
{
  EFI_STATUS                    Status;
  Status                      = EFI_SUCCESS;

  return Status;
}


/**
  Program SpiEiss and LE bit for Variable Write service.

**/
VOID
DisableBiosLockForPeiWriteVariable (
  VOID
  )
{

  return;
}

/**
  Search and identify the physical address of a file module

  @retval  EFI_SUCCESS  If address has been found
  @retval  Others       If address has not been found
**/
EFI_STATUS
FindModuleInFlash2 (
  IN EFI_FIRMWARE_VOLUME_HEADER *FvHeader,
  IN EFI_GUID                   *GuidPtr,
  IN OUT UINT32                 *ModulePtr,
  IN OUT UINT32                 *ModuleSize
)
{
  EFI_FFS_FILE_HEADER        *FfsHeader      = NULL;
  UINT32                     ModuleAddr       = 0;
  UINT32                     FileLength       = 0;
  UINT32                     FileOccupiedSize = 0;

  if (FvHeader == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // FvHeader
  //
  DEBUG ((DEBUG_VERBOSE, "FvHeader 0x%X:\n", FvHeader, FvHeader));
  DEBUG ((DEBUG_VERBOSE, " FileSystemGuid = 0x%g\n", &FvHeader->FileSystemGuid));
  DEBUG ((DEBUG_VERBOSE, " FvLength       = 0x%X\n", FvHeader->FvLength));
  DEBUG ((DEBUG_VERBOSE, " FvHeader Size  = 0x%X\n", FvHeader->HeaderLength));

  FfsHeader = (EFI_FFS_FILE_HEADER *)((UINT8 *) FvHeader + FvHeader->HeaderLength);

  //
  // Traverse through the FFS of the FV
  //
  while (ModuleAddr < (((UINT32) (UINT8 *) FvHeader) + FvHeader->FvLength)) {
    //
    // check for matching FFS guid
    //
    if (ModuleAddr != 0 && FileLength != 0) {
      if(CompareGuid (GuidPtr, &FfsHeader->Name)) {
        *ModulePtr  = ModuleAddr + sizeof(EFI_FFS_FILE_HEADER);
        *ModuleSize = FileLength - sizeof(EFI_FFS_FILE_HEADER);
        DEBUG (( DEBUG_VERBOSE, "Module found at = 0x%X, Module Size = 0x%X\n", *ModulePtr, *ModuleSize));
        return EFI_SUCCESS;
      }
    }

    //
    // Next FFS = FfsHeader + FfsHeader->Size
    //
    FfsHeader = (EFI_FFS_FILE_HEADER *)(((UINT32) (UINT8 *)FfsHeader) + FileOccupiedSize);
    ModuleAddr = (UINT32) (UINT8 *) FfsHeader;
    if (IS_FFS_FILE2 (FfsHeader)) {
      FileLength = FFS_FILE2_SIZE (FfsHeader);
      ASSERT (FileLength > 0x00FFFFFF);
    } else {
      FileLength = FFS_FILE_SIZE (FfsHeader);
    }

    //
    // FileLength is adjusted to FileOccupiedSize as it is 8 byte aligned.
    //
    FileOccupiedSize = (UINT32) GET_OCCUPIED_SIZE (FileLength, 8);
    DEBUG ((DEBUG_VERBOSE, "FfsHeader 0x%X:\n", ModuleAddr));
    DEBUG ((DEBUG_VERBOSE, " Name = 0x%g\n", &FfsHeader->Name));
    DEBUG ((DEBUG_VERBOSE, " Size = 0x%X\n", FileLength));
    DEBUG ((DEBUG_VERBOSE, " Type = 0x%X\n", FfsHeader->Type));
    DEBUG ((DEBUG_VERBOSE, " Occupied Size         = 0x%X\n", FileOccupiedSize));
    if (FileLength == 0 || FileOccupiedSize == 0) {
      DEBUG (( DEBUG_ERROR, "File Length is zero. Exit\n"));
      break;
    }
  }

  DEBUG (( DEBUG_ERROR, "Module not found\n"));
  return EFI_NOT_FOUND;
}

/**
  Collect CrashLog data from PMC SSRAM.
**/
VOID
CrashLogCollectDataFromPmcSsram (
  VOID
  )
{
#ifdef CDF_SC_FLAG
  EFI_STATUS                  Status;
  PMC_IPC_DISCOVERY_BUF       DiscoveryBuffer;
  UINT64                      PmcSsramBaseAddress;
  UINT32                      PmcSsramBar0;
  UINT32                      CrashLogAddr;
  EFI_PHYSICAL_ADDRESS        AllocateAddress;
  UINT32                      *Destination = NULL;
  UINT32                      CrashLogSize = 0;
  UINT32                      CopiedSize = 0;

  DEBUG ((DEBUG_INFO, "CrashLogCollectDataFromPmcSsram - start\n"));

  //
  // Check for the availability of CrashLog feature
  //
  ZeroMem (&DiscoveryBuffer, sizeof (PMC_IPC_DISCOVERY_BUF));

  Status = PmcCrashLogDiscovery (&DiscoveryBuffer);

  DEBUG ((
    DEBUG_INFO,
    "PCH CrashLog Discovery Value = 0x%x, Status = %r, Avail = %d, Dis = %d\n",
    DiscoveryBuffer.Uint32,
    Status,
    DiscoveryBuffer.Bits.Avail,
    DiscoveryBuffer.Bits.Dis
    ));

  if (EFI_ERROR (Status) || (DiscoveryBuffer.Bits.Avail != 1) || (DiscoveryBuffer.Bits.Dis == 1)) {
    DEBUG ((DEBUG_INFO, "PCH CrashLog feature is not supported\n"));
    return;
  }

  //
  // Start to access PMC SSARM MMIO
  //
  PmcSsramBaseAddress = PCI_SEGMENT_LIB_ADDRESS (
                          DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                          DEFAULT_PCI_BUS_NUMBER_PCH,
                          PCI_DEVICE_NUMBER_PCH_PMC_SSRAM,
                          PCI_FUNCTION_NUMBER_PCH_PMC_SSRAM,
                          0
                          );

  if (PciSegmentRead16 (PmcSsramBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "PMC SSRAM PCI device is disabled\n"));
    return;
  }

  //
  // Program BAR 0 and enable command register memory space decoding
  //
  PmcSsramBar0 = PcdGet32 (PcdSiliconInitTempMemBaseAddr);
  PciSegmentWrite32 (PmcSsramBaseAddress + PCI_BASE_ADDRESSREG_OFFSET, PmcSsramBar0);
  PciSegmentOr16 (PmcSsramBaseAddress + PCI_COMMAND_OFFSET, (UINT16) (EFI_PCI_COMMAND_MEMORY_SPACE));

  //
  // Check for the presence of CrashLog data
  //
  CrashLogAddr = PmcSsramBar0 + DiscoveryBuffer.Bits.BaseOffset;
  DEBUG ((DEBUG_INFO, "Value at CrashLogAddress 0x%x = 0x%x\n ", CrashLogAddr, MmioRead32 (CrashLogAddr)));
  DEBUG ((DEBUG_INFO, "Value at CrashLogAddress 0x%x = 0x%x\n ", CrashLogAddr + 4, MmioRead32 (CrashLogAddr + 4)));

  if ((MmioRead32 (CrashLogAddr) != 0) && ((MmioRead32 (CrashLogAddr + 4) & 0x0F) != 0)) {
    DEBUG ((DEBUG_INFO, "PMC CrashLog is present\n"));
  } else {
    DEBUG ((DEBUG_INFO, "PMC CrashLog is not present\n"));
    goto Exit;
  }

  //
  // Allocate memory buffer for CrashLog data
  //
  CrashLogSize = (DiscoveryBuffer.Bits.Size != 0) ? (DiscoveryBuffer.Bits.Size * sizeof (UINT32)) : 0xC00;
  DEBUG ((DEBUG_INFO, "CrashLogSize in discovery buffer is = 0x%x\n ", DiscoveryBuffer.Bits.Size));
  DEBUG ((DEBUG_INFO, "Use CrashLogSize as 0x%x\n ", CrashLogSize));

  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES (CrashLogSize),
             &AllocateAddress
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Memory out of resource\n"));
    goto Exit;
  }

  //
  // Build of location for Crash log in Hob
  //
  BuildGuidDataHob (
    &gPmcCrashLogDataBufferHobGuid,
    (VOID *) &AllocateAddress,
    sizeof (UINT64)
    );
  DEBUG ((DEBUG_INFO, "CrashLogDataBuffer = 0x%x\n", AllocateAddress));

  ZeroMem ((VOID *) (UINTN) AllocateAddress, EFI_SIZE_TO_PAGES (CrashLogSize) * EFI_PAGE_SIZE);

  //
  // Copy CrashLog data from SSRAM to allocated memory buffer
  //
  Destination = (UINT32 *)(UINTN) AllocateAddress;
  CopiedSize = 0;
  while (CopiedSize < CrashLogSize) {
    *Destination = MmioRead32 (CrashLogAddr); // Byte access is not allowed to PMC SSRAM, hence copying DW by DW
    Destination++;
    CrashLogAddr += 4;
    CopiedSize   += 4;
  }

  //
  // Clear the SSRAM region after copying the error log
  //
  Status = PmcCrashLogClear ();
  DEBUG ((DEBUG_INFO, " Clear CrashLog Status = %r\n", Status));

Exit:
  //
  // Disable PMC SSRAM MMIO
  //
  PciSegmentAnd16 (PmcSsramBaseAddress + PCI_COMMAND_OFFSET, (UINT16) ~(EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_IO_SPACE));
  PciSegmentWrite32 (PmcSsramBaseAddress + PCI_BASE_ADDRESSREG_OFFSET, 0);
  DEBUG ((DEBUG_INFO, "CrashLogCollectDataFromPmcSsram - end\n"));
#endif  // #ifdef CDF_SC_FLAG
}

/**
  Platform Init PEI module entry point

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS          The function completes successfully
  @retval     EFI_OUT_OF_RESOURCES Insufficient resources to create database
**/
EFI_STATUS
EFIAPI
PlatformInitEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                       Status = EFI_SUCCESS;
  UINT8                            FwConfig;
  PCH_SETUP                        PchSetup;
  UINTN                            VariableSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariableServices;

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,  // GUID
             0,                                 // INSTANCE
             NULL,                              // EFI_PEI_PPI_DESCRIPTOR
             (VOID **) &VariableServices        // PPI
             );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (PCH_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"PchSetup",
                               &gPchSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &PchSetup
                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Fail to get Pch Setup and set the configuration to production mode!\n"));
    FwConfig = FwConfigDefault;
  } else {
    FwConfig = PchSetup.FirmwareConfiguration;
  }

  //
  // Collect CrashLog data before Energy Reporting is enabled, which occurs
  // after policy init below. only for CDF PCH. EBG PCH crash log is in CrashLogDxe driver
  //
  if (IsCdfPch()) {
    CrashLogCollectDataFromPmcSsram ();
  }
  PeiPolicyInit (PeiServices, FwConfig);

  return Status;
}
