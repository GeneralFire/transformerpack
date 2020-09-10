/** @file
  Source code file for Platform Init PEI module

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

#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/RasRcDataLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CrashLogCpuLib.h>
#include <Library/CrashLogPchLib.h>
#include <Library/IoLib.h>


CPU_CRASHLOG_RECORD_REGION_PROTOCOL      mCpuCrashLogProt;
PCH_CRASHLOG_RECORD_REGION_PROTOCOL      mPchCrashLogProt;
/**
  Enable/disable CrashLog base on policy.

**/
VOID
CrashLogEnableDisable (
  BOOLEAN      Enable
  )
{
  EFI_STATUS          Status = EFI_UNSUPPORTED;
  UINT8               SocId;
  CPU_CSR_ACCESS_VAR  *CpuCsrAccessVarPtr;

  //
  // CPU Crashlog enable or disable
  //
  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();
  for (SocId = 0; SocId < MAX_SOCKET; SocId++) {
    if (!(CpuCsrAccessVarPtr->socketPresentBitMap & (1 << SocId))) {
      continue;
    }
    Status = CpuCrashLogEnableDisable (SocId, Enable);
  }

  DEBUG ((DEBUG_INFO, "CpuCrashLogEnableDisable %r\n", Status));

  //
  // PCH Crashlog enable or disable
  //
  Status = PchCrashLogEnableDisable (Enable);

  DEBUG ((DEBUG_INFO, "PchCrashLogEnableDisable %r\n", Status));

}

/**
  Set CrashLog on all reset policy.

**/
EFI_STATUS
EFIAPI
CrashLogOnAllReset (
  VOID
  )
{
  EFI_STATUS                      Status = EFI_SUCCESS;
  UINT8                           SocId;
  CPU_CSR_ACCESS_VAR              *CpuCsrAccessVarPtr;

  //
  // Enable CPU Crash log on all resets
  //
  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();
  for (SocId = 0; SocId < MAX_SOCKET; SocId++) {
    if (!(CpuCsrAccessVarPtr->socketPresentBitMap & (1 << SocId))) {
      continue;
    }
    Status = CpuCrashLogOnAllReset (SocId, TRUE);
  }

  DEBUG ((DEBUG_INFO, "CpuCrashLogOnAllReset %r\n", Status));

  //
  // Enable PCH Crash log on all resets
  //
  Status = PchCrashLogOnAllReset ();

  DEBUG ((DEBUG_INFO, "PchCrashLogOnAllReset %r\n", Status));

  return Status;
}

/**
  Collect CPU CrashLog data from OOB-MSM SRAM and store in HOB.

**/
VOID
CpuCrashLogCollectDataFromOobMsmSRAM (
  VOID
  )
{
  EFI_STATUS                               Status;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC  CrashLogDevscCap;
  EFI_PHYSICAL_ADDRESS                     AllocateAddress;
  UINT32                                   CrashLogSize;
  UINT32                                   CpuCrashLogAddr;
  CPU_CRASHLOG_RECORD_REGIONS_STRUCT       CpuCrashLogRecRegs;
  UINT64                                   *Destination = NULL;
  UINT32                                   CopiedSize;
  UINT32                                   SavedStsCmd;
  UINT8                                    SocId;
  UINT8                                    Entry;
  CPU_CSR_ACCESS_VAR                       *CpuCsrAccessVarPtr;
  EFI_HANDLE                               CpuCrashLogProtHdl = NULL;
  BOOLEAN                                  ClearBar;
  BOOLEAN                                  InstallProtocol = FALSE;

  DEBUG ((DEBUG_INFO, "CpuCrashLogCollectDataFromOobMsmSRAM - start\n"));

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();
  ZeroMem (&CpuCrashLogRecRegs, sizeof (CPU_CRASHLOG_RECORD_REGIONS_STRUCT));
  ZeroMem (&mCpuCrashLogProt, sizeof (CPU_CRASHLOG_RECORD_REGION_PROTOCOL));
  //
  // Collect crash log data for each CPU
  //
  for (SocId = 0; SocId < MAX_SOCKET; SocId++) {
    CrashLogSize = 0;
    CopiedSize = 0;
    if (!(CpuCsrAccessVarPtr->socketPresentBitMap & (1 << SocId))) {
      continue;
    }
    //
    // Check for the availability of CPU CrashLog feature
    //
    ZeroMem (&CrashLogDevscCap, sizeof (PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC));
    Status = GetCpuCrashLogCapability (SocId, &CrashLogDevscCap);
    if (!EFI_ERROR (Status)) {
      //
      // Program BAR address and enable command register memory space decoding
      //
      ClearBar = InitOobMsmPmonCrashlogBar (SocId, &CrashLogDevscCap, &SavedStsCmd);

      Status = CpuCrashLogDiscovery (SocId, &CpuCrashLogRecRegs);

      if (EFI_ERROR (Status)) {
        continue;
      }

      for (Entry = 0; Entry < CrashLogDevscCap.DiscoveryData.Bits.NumEntries; Entry++) {
        CrashLogSize = CpuCrashLogRecRegs.RegionInfo[SocId][Entry].Fields.SizeInDword * 4;
        if (CrashLogSize == 0) {
          DEBUG ((DEBUG_INFO, "Socket 0x%X CPU CrashLog Entry %d is not present \n", SocId, Entry));
          continue;
        }

        DEBUG ((DEBUG_INFO, "Socket 0x%X CPU CrashLog Entry %d Size : 0x%X \n", SocId, Entry, CrashLogSize));

        //
        // Allocate memory buffer for CPU CrashLog data
        //
        Status = gBS->AllocatePages (
                  AllocateAnyPages,
                  EfiBootServicesData,
                  EFI_SIZE_TO_PAGES (CrashLogSize),
                  &AllocateAddress
                  );
        DEBUG ((DEBUG_INFO, "Socket 0x%X Entry %d  AllocateAddress : 0x%X \n", SocId, Entry, AllocateAddress));

        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Memory out of resource!\n"));
          break;
        }

        mCpuCrashLogProt.RegionBuffer[SocId][Entry].Address = AllocateAddress;
        mCpuCrashLogProt.RegionBuffer[SocId][Entry].Size = CrashLogSize;

        ZeroMem ((VOID *) (UINTN) AllocateAddress, EFI_SIZE_TO_PAGES (CrashLogSize) * EFI_PAGE_SIZE);

        //
        // copy CPU crash log region
        //
        CpuCrashLogAddr = CpuCrashLogRecRegs.RegionInfo[SocId][Entry].Fields.Address;
        Destination =  (UINT64*)(UINTN) AllocateAddress;
        while (CopiedSize < CrashLogSize) {
          *Destination = MmioRead64 (CpuCrashLogAddr);
          if (CopiedSize < CRASHLOG_SIZE_DEBUG_PURPOSE) { // Dumping only few bytes to help debug
            DEBUG ((DEBUG_INFO, "CPU %d Entry %d CrashData[0x%x] = 0x%lx\n", SocId, Entry, CopiedSize, *Destination));
          }
          Destination++;
          CpuCrashLogAddr += 8;
          CopiedSize   += 8;
        }
      }
      //
      // Clear the OOB-MSM SRAM region after copying the error log
      //
      CpuCrashLogClear (SocId);

      //
      // Disable OOB-MSM SRAM MMIO
      //
      ClearOobMsmPmonCrashlogBar (SocId, &CrashLogDevscCap, SavedStsCmd, ClearBar);
      InstallProtocol = TRUE;
    } else {
      DEBUG ((DEBUG_ERROR, "Socket 0x%X CPU CrashLog feature is not supported\n", SocId));
    }
  }

  if (InstallProtocol) {
    //
    // Install Cpu CrashLog Record Region Protocol
    //
    Status = gBS->InstallProtocolInterface (
                      &CpuCrashLogProtHdl,
                      &gCpuCrashLogRecordRegionProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      &mCpuCrashLogProt
                      );
    ASSERT_EFI_ERROR (Status);
    DEBUG ((DEBUG_INFO, "mCpuCrashLogProt = 0x%X, sizeof (CPU_CRASHLOG_RECORD_REGION_PROTOCOL) : 0x%X \n", mCpuCrashLogProt, sizeof (CPU_CRASHLOG_RECORD_REGION_PROTOCOL)));
  }

  DEBUG ((DEBUG_INFO, "CpuCrashLogCollectDataFromOobMsmSRAM - end\n"));

  return;
}


/**
  Collect CrashLog data from PMC SSRAM.
**/
VOID
CrashLogCollectDataFromPmcSsram (
  VOID
  )
{
  EFI_STATUS                          Status;
  PCH_CRASHLOG_DISCOVERY_HEADER       DiscoveryHeader;
  PCH_CRASHLOG_RECORD_REGIONS_STRUCT  PchCrashLogRecRegs;
  EFI_HANDLE                          PchCrashLogProtHdl = NULL;
  UINT32                              CrashLogAddr;
  EFI_PHYSICAL_ADDRESS                AllocateAddress;
  UINT32                              *Destination = NULL;
  UINT32                              CrashLogSize = 0;
  UINT32                              CopiedSize = 0;
  BOOLEAN                             ClearBar;
  UINT16                              SavedStsCmd;
  UINT8                               i;
  BOOLEAN                             InstallProtocol = FALSE;

  DEBUG ((DEBUG_INFO, "CrashLogCollectDataFromPmcSsram - start\n"));

  //
  // Check for the availability of CrashLog feature
  //
  ZeroMem (&DiscoveryHeader, sizeof (PCH_CRASHLOG_DISCOVERY_HEADER));

  Status = GetPchCrashLogDiscHdr (&DiscoveryHeader);

  DEBUG ((
    DEBUG_INFO,
    "PCH CrashLog Discovery Value = 0x%x, Status = %r, CrashlogCapability = %d, CrashLogDisabled = %d\n",
    DiscoveryHeader.Data[0],
    Status,
    DiscoveryHeader.Fields.CrashlogCapability,
    DiscoveryHeader.Fields.CrashLogDisabled
    ));

  if (EFI_ERROR (Status) || (DiscoveryHeader.Fields.CrashlogCapability != 1) || (DiscoveryHeader.Fields.CrashLogDisabled == 1)) {
    DEBUG ((DEBUG_INFO, "PCH CrashLog feature is not supported\n"));
    return;
  }

  //
  // Start to access PMC SSARM MMIO
  //
  if (IsPmcSsramDeviceDisabled ()) {
    DEBUG ((DEBUG_INFO, "PMC SSRAM PCI device is disabled\n"));
    return;
  }

  //
  // Program BAR 0 and enable command register memory space decoding
  //
  ClearBar = InitPmcSsramMmioBar (&SavedStsCmd);

  ZeroMem (&PchCrashLogRecRegs, sizeof (PCH_CRASHLOG_RECORD_REGIONS_STRUCT));
  Status = PchCrashLogDiscovery (&PchCrashLogRecRegs);
  if (EFI_ERROR (Status)) {
    return;
  }

  for (i = 0; i < MAX_PCH_CRASHLOG_REGIONS; i++) {
    CrashLogSize = PchCrashLogRecRegs.RegionInfo[i].Fields.SizeInDword * 4;
    if (CrashLogSize == 0) {
      break;
    }
    DEBUG ((DEBUG_INFO, "PchCrashLogRecRegs.RegionInfo[0x%X]  Size : 0x%X \n", i, CrashLogSize));

    //
    // Allocate memory buffer for CrashLog data
    //
    Status = gBS->AllocatePages (
              AllocateAnyPages,
              EfiBootServicesData,
              EFI_SIZE_TO_PAGES (CrashLogSize),
              &AllocateAddress
              );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Memory out of resource\n"));
      break;
    }

    mPchCrashLogProt.RegionBuffer[i].Address = AllocateAddress;
    mPchCrashLogProt.RegionBuffer[i].Size = CrashLogSize;

    ZeroMem ((VOID *) (UINTN) AllocateAddress, EFI_SIZE_TO_PAGES (CrashLogSize) * EFI_PAGE_SIZE);

    //
    // Copy CrashLog data from SSRAM to allocated memory buffer
    //
    CrashLogAddr = PchCrashLogRecRegs.RegionInfo[i].Fields.Address ;
    Destination = (UINT32 *)(UINTN) AllocateAddress;
    CopiedSize = 0;
    while (CopiedSize < CrashLogSize) {
      *Destination = MmioRead32 (CrashLogAddr); // Byte access is not allowed to PMC SSRAM, hence copying DW by DW
      if (CopiedSize < CRASHLOG_SIZE_DEBUG_PURPOSE) { // Dumping only few bytes to help debug
        DEBUG ((DEBUG_INFO, "PCH CrashLog Region %d CrashData[0x%x] = 0x%x\n", i, CopiedSize, *Destination));
      }
      Destination++;
      CrashLogAddr += 4;
      CopiedSize   += 4;
    }
    InstallProtocol = TRUE;
  }

  if (InstallProtocol) {
    //
    // Install Pch CrashLog Record Region Protocol
    //
    Status = gBS->InstallProtocolInterface (
                      &PchCrashLogProtHdl,
                      &gPchCrashLogRecordRegionProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      &mPchCrashLogProt
                      );
    ASSERT_EFI_ERROR (Status);
    DEBUG ((DEBUG_INFO, "mPchCrashLogProt = 0x%X, sizeof (PCH_CRASHLOG_RECORD_REGION_PROTOCOL) : 0x%X \n", mPchCrashLogProt, sizeof (PCH_CRASHLOG_RECORD_REGION_PROTOCOL)));
    //
    // Clear or Re-Arm the SSRAM region after copying the error log
    //
    if (DiscoveryHeader.Fields.ReARMSupported) {
      Status = PchCrashLogReArm ();
      DEBUG ((DEBUG_INFO, " Re-Arm CrashLog Status = %r\n", Status));
    } else {
      Status = PchCrashLogClear ();
      DEBUG ((DEBUG_INFO, " Clear CrashLog Status = %r\n", Status));
    }
  }

  //
  // Disable PMC SSRAM MMIO
  //
  ClearPmcSsramMmioBar (SavedStsCmd, ClearBar);
  DEBUG ((DEBUG_INFO, "CrashLogCollectDataFromPmcSsram - end\n"));
}

/**
  Crash Log DXE Driver entry point

  @param[in] ImageHandle  - ImageHandle of the loaded driver
  @param[in] SystemTable  - Pointer to the BIOS System Table

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
CrashLogDxeEntryPoint (
  IN EFI_HANDLE                 ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable
  )
{
  EFI_STATUS                Status = EFI_SUCCESS;
  RAS_RC_POLICY             *Policy;

  DEBUG ((DEBUG_INFO, "CrashLogDxeEntryPoint \n"));

  //
  // Capture the input policy and correct it to conform to restrictions.
  //
  Policy = GetRasRcPolicy ();
  if (Policy == NULL) {
    DEBUG ((DEBUG_INFO, "Failed to get RAS RC Policy!\n"));
    return EFI_UNSUPPORTED;
  }

  if (Policy->CrashLogFeature == 0) {
    DEBUG ((DEBUG_INFO, "CrashLog is disabled by policy\n"));
    CrashLogEnableDisable (FALSE);
    return EFI_UNSUPPORTED;
  } else {
    CrashLogEnableDisable (TRUE);
  }

  if (Policy->CrashLogOnAllReset) {
    DEBUG ((DEBUG_INFO, "CrashLog on All Reset\n"));
    //
    // Collect crash data on every boot
    //
    Status = CrashLogOnAllReset ();
  }

  //
  // Collect CrashLog data from OOB-MSM
  //
  CpuCrashLogCollectDataFromOobMsmSRAM ();

  //
  // Collect CrashLog data from PMC SSRAM
  //
  CrashLogCollectDataFromPmcSsram ();

  return Status;
}
