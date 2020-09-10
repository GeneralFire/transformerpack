/** @file

  Crash Log PCH Library

@copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#include <RcRegs.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/CrashLogPchLib.h>
#include <Library/PmcLib.h>
#include <Library/PciSegmentLib.h>
#include <PchAccess.h>

/**
  This function returns the PCH crash log descriptor table.

  @param[IN]  SocId                Socket ID of the processor
  @param[IN]  CrashLogDevscCap     Structure of CrashLog DEVSC Data
  @param[IN]  CrashLogDiscHdr      Structure of CrashLog Discovery Header
  @param[OUT] DescriptorTable      Structure of CrashLog Descriptor Table

  @retval EFI_UNSUPPORTED          Cpu Descriptor Table not found
  @retval EFI_SUCCESS              Cpu Descriptor Table found
**/
EFI_STATUS
GetPchCrashLogDescriptorTable (
  IN  PCH_CRASHLOG_DISCOVERY_HEADER            *CrashLogDiscHdr,
  OUT PCH_CRASHLOG_DESCRIPTOR_TABLE            *DescriptorTable
  )
{
  UINT32                                    DescriptorTableAddr;
  UINT32                                    i;

  DescriptorTableAddr = GetPmcSsramMmioBar() + CrashLogDiscHdr->Fields.DescriptorTableOffset;

  DescriptorTable->NumOfRegions = MmioRead32 (DescriptorTableAddr);

  if (DescriptorTable->NumOfRegions > MAX_PCH_CRASHLOG_REGIONS) {
    DEBUG ((DEBUG_ERROR,
      "DescriptorTable->NumOfRegions = %d which is bigger than MAX_PCH_CRASHLOG_REGIONS %d! cutting off...\n",
      DescriptorTable->NumOfRegions,
      MAX_PCH_CRASHLOG_REGIONS
      ));
    DescriptorTable->NumOfRegions = MAX_PCH_CRASHLOG_REGIONS;
  }

  for (i = 0; i < DescriptorTable->NumOfRegions; i++) {
    DescriptorTable->Entry[i].Data = MmioRead32 (DescriptorTableAddr + 4 + i*4);
    DEBUG ((DEBUG_INFO, "PCH CrashLog Region Address 0x%X, SizeInDwords 0x%x .\n",
            DescriptorTable->Entry[i].Fields.RegionOffset, DescriptorTable->Entry[i].Fields.RegionSize));
  }

  return EFI_SUCCESS;
}

/**
  Get PCH CrashLog Discovery Header

  @param[OUT] CrashLogDiscHdr   Structure of CrashLog Discovery Header

  @retval EFI_UNSUPPORTED          Get PCH Crashlog discovery header fail
  @retval EFI_SUCCESS              PCH Crashlog discovery header found
**/
EFI_STATUS
GetPchCrashLogDiscHdr (
  OUT PCH_CRASHLOG_DISCOVERY_HEADER           *CrashLogDiscHdr
  )
{
  EFI_STATUS             Status;
  PMC_IPC_DISCOVERY_BUF  DiscoveryBuffer;

  //
  // Check for the availability of CrashLog feature
  //
  ZeroMem (&DiscoveryBuffer, sizeof (PMC_IPC_DISCOVERY_BUF));

  Status = PmcCrashLogDiscovery (&DiscoveryBuffer);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Fail to get PCH CrashLog Header!\n"));
    return Status;
  }

  ZeroMem (CrashLogDiscHdr, sizeof (PCH_CRASHLOG_DISCOVERY_HEADER));
  CopyMem (CrashLogDiscHdr, &DiscoveryBuffer, sizeof (PMC_IPC_DISCOVERY_BUF));

  return EFI_SUCCESS;
}

/**
  This function discovers PCH crashLog region

  @param[out] PchCrashLogRecRegs  - Stucture of PCH CrashLog Regions

  @retval EFI_UNSUPPORTED         PCH CrashLog is not supported
  @retval EFI_SUCCESS             PCH CrashLog is supported
**/
EFI_STATUS
PchCrashLogDiscovery (
  OUT PCH_CRASHLOG_RECORD_REGIONS_STRUCT  *PchCrashLogRecRegs
  )
{
  EFI_STATUS                                Status;
  UINT32                                    PmcSsramBar;
  UINT32                                    i;
  UINT32                                    CrashLogAddr;
  UINT32                                    CrashLogSize = 0;
  PCH_CRASHLOG_DISCOVERY_HEADER             CrashLogDiscHdr;
  PCH_CRASHLOG_DESCRIPTOR_TABLE             DescriptorTable;
  CRASHLOG_RECORD_HEADER_COMMON             CrashLogRecHdr;

  ZeroMem (&CrashLogDiscHdr, sizeof (PCH_CRASHLOG_DISCOVERY_HEADER));
  ZeroMem (&DescriptorTable, sizeof (PCH_CRASHLOG_DESCRIPTOR_TABLE));

  Status = GetPchCrashLogDiscHdr (&CrashLogDiscHdr);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PCH CrashLog feature is not supported\n"));
    return Status;
  }

  if (!CrashLogDiscHdr.Fields.CrashlogCapability) {
    DEBUG ((DEBUG_ERROR, "CrashlogCapability is zero in PCH CrashLog Discovery Header!\n"));
    return EFI_UNSUPPORTED;
  }

  if (CrashLogDiscHdr.Fields.CrashLogDisabled) {
    DEBUG ((DEBUG_ERROR, "CrashLogDisabled is set in PCH CrashLog Discovery Header!\n"));
    return EFI_UNSUPPORTED;
  }

  Status = GetPchCrashLogDescriptorTable (&CrashLogDiscHdr, &DescriptorTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PchCrashLogDiscovery - fail to get Descriptor Table %r\n", Status));
    return Status;
  }

  PmcSsramBar = GetPmcSsramMmioBar();
  DEBUG ((DEBUG_INFO, "PchCrashLogDiscovery - PmcSsramBar = 0x%X\n", PmcSsramBar));

  Status = EFI_UNSUPPORTED;
  for (i = 0; i < DescriptorTable.NumOfRegions; i++) {
    CrashLogAddr = PmcSsramBar + DescriptorTable.Entry[i].Fields.RegionOffset;
    DEBUG ((DEBUG_ERROR, "PchCrashLogDiscovery - CrashLogAddr %x \n", CrashLogAddr));
    CrashLogSize = DescriptorTable.Entry[i].Fields.RegionSize;
    CrashLogRecHdr.RecordVersion.Data = MmioRead32 (CrashLogAddr);
    if (CrashLogRecHdr.RecordVersion.Data == 0) {
      DEBUG ((DEBUG_ERROR, "PchCrashLogDiscovery - first DWORD of Record %d is Zero\n", i));
      break;
    }

    if (CrashLogRecHdr.RecordVersion.Fields.Consumed) {
      DEBUG ((DEBUG_INFO, "Crash Record %d Already consumed!\n", i));
      break;
    }

    if (CrashLogRecHdr.RecordVersion.Fields.HeadType == CRASHLOG_HEADER_TYPE_LEGACY ||
        CrashLogRecHdr.RecordVersion.Fields.HeadType > CRASHLOG_HEADER_TYPE_4) {
      DEBUG ((DEBUG_INFO, "Crash Record HeadType %d of record %d is not supported!\n", CrashLogRecHdr.RecordVersion.Fields.HeadType, i));
      break;
    }

    PchCrashLogRecRegs->RegionInfo[i].Fields.SizeInDword = CrashLogSize;
    PchCrashLogRecRegs->RegionInfo[i].Fields.Address = CrashLogAddr;
    DEBUG ((DEBUG_INFO, "PchCrashLogRecRegs->RegionInfo[0x%X].Fields.Address = 0x%X\n", i, CrashLogAddr));
    DEBUG ((DEBUG_INFO, "PchCrashLogRecRegs->RegionInfo[0x%X].Fields.SizeInDword = 0x%X\n", i, CrashLogSize));
    Status = EFI_SUCCESS;
  }
  return Status;
}

/**
  This function clears PCH crash log

  @retval EFI_SUCCESS

**/
EFI_STATUS
PchCrashLogClear (
  VOID
  )
{
  EFI_STATUS                  Status;

  //
  // Clear the SSRAM region after copying the error log
  //
  Status = PmcCrashLogClear ();
  DEBUG ((DEBUG_INFO, " Clear Pmc CrashLog Status = %r\n", Status));

  return Status;
}

/**
  This function re-arm PCH crash log

  @retval EFI_SUCCESS

**/
EFI_STATUS
PchCrashLogReArm (
  VOID
  )
{
  EFI_STATUS                  Status;

  //
  // ReArm the SSRAM region after copying the error log
  //
  Status = PmcCrashLogReArm ();
  DEBUG ((DEBUG_INFO, " ReArm Pmc CrashLog Status = %r\n", Status));
  return Status;
}

/**
  This function enable/disables PCH crash log

  @param[in] Enable                    - TRUE = Enable, FALSE = Disable

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED

**/
EFI_STATUS
PchCrashLogEnableDisable (
  BOOLEAN     Enable
  )
{

  if (Enable) {
    //
    // PMC crash log default is enabled, no command to enable it.
    //
  } else {
    PmcCrashLogDisable ();

  }

  return EFI_SUCCESS;
}

/**
  This function enable/disables PCH crash log collect on all resets

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED
**/
EFI_STATUS
PchCrashLogOnAllReset (
  VOID
  )
{
  EFI_STATUS                  Status;

  //
  // Collect crash data on every boot
  //
  Status = PmcCrashLogOnAllReset ();
  DEBUG ((DEBUG_INFO, " PMC IPC to collect CrashLog on every reset  Status = %r\n", Status));

  return Status;
}

/**
  Check if PMC SSRAM device disabled

  @retval FALSE - device not exist
  @retval TRUE -  device exist
**/
BOOLEAN
IsPmcSsramDeviceDisabled (
  VOID
  )
{

  UINT64                      PmcSsramBaseAddress;

  PmcSsramBaseAddress = PCI_SEGMENT_LIB_ADDRESS (
                          DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                          DEFAULT_PCI_BUS_NUMBER_PCH,
                          PCI_DEVICE_NUMBER_PCH_PMC_SSRAM,
                          PCI_FUNCTION_NUMBER_PCH_PMC_SSRAM,
                          0
                          );

  if (PciSegmentRead16 (PmcSsramBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return TRUE;
  }

  return FALSE;

}

/**
  Init PMC SSRAM MMIO Bar

  @param[in] SavedStsCmd               - the saved value of PMC SSRAM device status and command register

  @retval FALSE - PMC SSRAM MMIO Bar already asigned
  @retval TRUE -  PMC SSRAM MMIO Bar asigned by this routine
**/
BOOLEAN
InitPmcSsramMmioBar (
  OUT UINT16              *SavedStsCmd
  )
{
  UINT64                      PmcSsramBaseAddress;
  UINT32                      PmcSsramBar0;
  UINT32                      Data32;
  BOOLEAN                     Return;

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

  Data32 = PciSegmentRead32 (PmcSsramBaseAddress + PCI_BASE_ADDRESSREG_OFFSET);

  if ( (Data32 & 0xFFFFFFF0) == 0 ) {
    //
    // Program BAR 0 if the BAR is not programmed
    //
    PmcSsramBar0 = PcdGet32 (PcdSiliconInitTempMemBaseAddr);
    DEBUG ((DEBUG_INFO, "InitPmcSsramMmioBar Bar 0x%x = 0x%x!\n", PCI_BASE_ADDRESSREG_OFFSET, PmcSsramBar0));
    PciSegmentWrite32 (PmcSsramBaseAddress + PCI_BASE_ADDRESSREG_OFFSET, PmcSsramBar0);
    Return = TRUE;
  } else {
    Return = FALSE;
  }

  //
  // Save and Enable command register memory space decoding
  //

  *SavedStsCmd = PciSegmentRead16 (PmcSsramBaseAddress + PCI_COMMAND_OFFSET);
  DEBUG ((DEBUG_INFO, "InitPmcSsramMmioBar SavedStsCmd = 0x%x!\n", *SavedStsCmd));
  PciSegmentOr16 (PmcSsramBaseAddress + PCI_COMMAND_OFFSET, (UINT16) (EFI_PCI_COMMAND_MEMORY_SPACE));

  return Return;
}

/**
  Clear PMC SSRAM MMIO Bar and restore the PMC SSRAM device status and command register value

  @param[in] SavedStsCmd               - the restore value of PMC SSRAM device status and command register
  @param[in] ClearBar                  - if TRUE need to clear the bar, else don't

  return none
**/
VOID
ClearPmcSsramMmioBar (
  IN UINT16                     SavedStsCmd,
  IN BOOLEAN                    ClearBar
  )
{
  UINT64                      PmcSsramBaseAddress;
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

  //
  // Disable PMC SSRAM MMIO
  //
  PciSegmentAnd16 (PmcSsramBaseAddress + PCI_COMMAND_OFFSET, (UINT16) ~(EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_IO_SPACE));

  //
  // Only clear it when it is not pre-assigned
  //
  if (ClearBar) {
    PciSegmentWrite32 (PmcSsramBaseAddress + PCI_BASE_ADDRESSREG_OFFSET, 0);
  }

  DEBUG ((DEBUG_INFO, "ClearPmcSsramMmioBar SavedStsCmd = 0x%x!\n", SavedStsCmd));
  PciSegmentWrite16 (PmcSsramBaseAddress + PCI_COMMAND_OFFSET, SavedStsCmd);
}

/**
  Clear PMC SSRAM MMIO Bar and restore the PMC SSRAM device status and command register value

  @param[in] SavedStsCmd               - the restore value of PMC SSRAM device status and command register
  @param[in] ClearBar                  - if TRUE need to clear the bar, else don't

  return none
**/
UINT32
GetPmcSsramMmioBar (
  VOID
  )
{
  UINT32                      PmcSsramBar0;
  UINT64                      PmcSsramBaseAddress;
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
  PmcSsramBar0 = PciSegmentRead32 (PmcSsramBaseAddress + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFFFF0;
  DEBUG ((DEBUG_INFO, "GetPmcSsramMmioBar Bar 0x%x = 0x%x!\n", PCI_BASE_ADDRESSREG_OFFSET, PmcSsramBar0));
  return PmcSsramBar0;
}
