/** @file

  Crash Log CPU Library

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
#include <Library/UsraCsrLib.h>
#include <Library/CrashLogCpuLib.h>
#include <IndustryStandard/PciExpress50.h>
#include <Library/UpiHostDataLib.h>
#include <Library/SiliconWorkaroundLib.h>

/**
  Find the address of Cpu CrashLog Capability

  @param[IN]  SocId                Socket ID of the processor
  @param[OUT] CrashLogDevscCap     Pointer to structure of CrashLog DEVSC Data

  @retval EFI_UNSUPPORTED          Cpu CrashLog ID not found
  @retval EFI_SUCCESS              Cpu CrashLog ID found
**/
EFI_STATUS
GetCpuCrashLogCapability (
  IN  UINT8                                    SocId,
  OUT PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC  *CrashLogDevscCap
  )
{
  UINT32                                    CsrRegBase;
  UINT32                                    CsrRegAddr;

  CsrRegBase = PCI_CFG_DEVICE_VENDOR_ID_DISC_MSM_PMON_REG;

  if (UsraCsrRead (SocId, 0, CsrRegBase) == 0xFFFFFFFF) {
    DEBUG ((DEBUG_ERROR, "GetCpuCrashLogCapability - OOB-MSM.PMON PCI device is disabled\n"));
    return EFI_UNSUPPORTED;
  }

  CsrRegAddr = CsrRegBase + EFI_PCIE_CAPABILITY_BASE_OFFSET;

  while (TRUE) {
    //
    // Check if it's Telemetry Aggregator Capbility
    //
    CrashLogDevscCap->Header.Data32 = UsraCsrRead (SocId, 0, CsrRegAddr);
    //
    // Check if it's CrashLog entry. If not, check the next entry
    //
    if (CrashLogDevscCap->Header.Bits.CapabilityId == PCI_EXPRESS_EXTENDED_CAPABILITY_DVSEC_ID) {
      CrashLogDevscCap->Capablities.Data32 = UsraCsrRead (SocId, 0, CsrRegAddr + PCI_EXPRESS_REG_DVSEC_CAPABILITIES_OFFSET);
      if (CrashLogDevscCap->Capablities.Bits.DvsecVid == 0x8086) {
        CrashLogDevscCap->DiscoveryData.Data32[0] = UsraCsrRead (SocId, 0, CsrRegAddr + INTEL_REG_DVSEC_DISCOVERY_OFFSET);
        if (CrashLogDevscCap->DiscoveryData.Bits.DevscId == V_CRASHLOG_DVSEC_ID) {
          //
          // CrashLog DEVSC capability is found
          //
          CrashLogDevscCap->DiscoveryData.Data32[1] = UsraCsrRead (SocId, 0, CsrRegAddr + INTEL_REG_DVSEC_DISCOVERY_OFFSET + 4);
          DEBUG ((DEBUG_INFO, "GetCpuCrashLogCapability - Discovery Buffer offset = 0x%X\n", LShiftU64 (CrashLogDevscCap->DiscoveryData.Bits.DiscoveryTableOffset, 3)));
          DEBUG ((DEBUG_INFO, "GetCpuCrashLogCapability - NumEntries = %d\n", CrashLogDevscCap->DiscoveryData.Bits.NumEntries));
          if(CrashLogDevscCap->DiscoveryData.Bits.NumEntries > MAX_CPU_CRASHLOG_REGIONS) {
            DEBUG ((DEBUG_ERROR, "GetCpuCrashLogCapability - NumEntries = %d > MAX_CPU_CRASHLOG_REGIONS cutting off\n", CrashLogDevscCap->DiscoveryData.Bits.NumEntries));
            CrashLogDevscCap->DiscoveryData.Bits.NumEntries = MAX_CPU_CRASHLOG_REGIONS;
          }
          return EFI_SUCCESS;
        }
      }
    }
    //
    // If the reading of Next Cap offset is not valid number, then break
    //
    if (CrashLogDevscCap->Header.Bits.NextCapabilityOffset == 0 || CrashLogDevscCap->Header.Bits.NextCapabilityOffset == 0xFFF) {
      break;
    } else {
      CsrRegAddr = CsrRegBase + CrashLogDevscCap->Header.Bits.NextCapabilityOffset;
    }
  }

  DEBUG ((DEBUG_INFO, "GetCpuCrashLogCapability - Crashlog DVSEC not found!\n"));

  return EFI_UNSUPPORTED;
}

/**
  This function returns the Address and size of discovery Header

  @param[IN]  SocId                Socket ID of the processor
  @param[IN]  Entry                Entry number of this header
  @param[IN]  CrashLogDevscCap     Structure of CrashLog DEVSC Data
  @param[OUT] Address              Crashlog Discovery Header memory address
  @param[OUT] SizeInDwords         Crashlog Discovery Header size in dwords

  @retval EFI_UNSUPPORTED          Cpu CrashLog Discovery Header not found
  @retval EFI_SUCCESS              Cpu CrashLog Discovery Header found
**/
EFI_STATUS
GetCpuCrashLogDiscHdrAddrSize (
  IN  UINT8                                    SocId,
  IN  UINT8                                    Entry,
  IN  PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC  *CrashLogDevscCap,
  OUT UINT32                                   *Address,
  OUT UINT32                                   *SizeInDwords
  )
{
  UINT32                                    CsrRegBase;
  UINT32                                    BaseAddr;

  CsrRegBase = PCI_CFG_DEVICE_VENDOR_ID_DISC_MSM_PMON_REG;

  BaseAddr = 0;
  if (CrashLogDevscCap->DiscoveryData.Bits.TBir == V_CRASHLOG_DVSEC_TBIR_BAR0) {
    BaseAddr = UsraCsrRead (SocId, 0, CsrRegBase + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFFFF0;
  } else if (CrashLogDevscCap->DiscoveryData.Bits.TBir == V_CRASHLOG_DVSEC_TBIR_BAR1) {
    BaseAddr = UsraCsrRead (SocId, 0, CsrRegBase + PCI_BASE_ADDRESSREG_OFFSET + 4) & 0xFFFFFFF0;
  }

  if (BaseAddr == 0) {
    return EFI_DEVICE_ERROR;
  }

  *SizeInDwords = (UINT32) CrashLogDevscCap->DiscoveryData.Bits.EntrySize;
  *Address = BaseAddr + (UINT32) LShiftU64 (CrashLogDevscCap->DiscoveryData.Bits.DiscoveryTableOffset, 3);
  *Address += *SizeInDwords * Entry * 4;
  DEBUG ((DEBUG_INFO, "CrashLogDiscorveryHeader Address 0x%X, SizeInDwords 0x%x .\n", *Address, *SizeInDwords));
  return EFI_SUCCESS;
}

/**
  This function returns the address and size of CPU CrashLog Region.

  @param[IN]  SocId                Socket ID of the processor
  @param[IN]  Entry                Entry number of this header
  @param[IN]  CrashLogDevscCap     Structure of CrashLog DEVSC Data
  @param[IN]  CrashLogDiscHdr      Structure of CrashLog Discovery Header
  @param[OUT] Address              Crashlog Discovery Header memory address
  @param[OUT] SizeInDwords         Crashlog Discovery Header size in dwords

  @retval EFI_UNSUPPORTED          Cpu CrashLog Region not found
  @retval EFI_SUCCESS              Cpu CrashLog Region found
**/
EFI_STATUS
GetCpuCrashLogRecRegAddrSize (
  IN  UINT8                                    SocId,
  IN  UINT8                                    Entry,
  IN  PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC  *CrashLogDevscCap,
  IN  CPU_CRASHLOG_DISCOVERY_HEADER            *CrashLogDiscHdr,
  OUT UINT32                                   *Address,
  OUT UINT32                                   *SizeInDwords
  )
{
  EFI_STATUS                                Status;
  UINT32                                    HdrAddr;
  UINT32                                    HdrSizeInDwords;

  switch (CrashLogDiscHdr->Fields.AccessType) {
    case CRASH_LOG_ACCESS_TYPE_BDF_BARID:
      //
      // Not support yet
      //
      Status = EFI_UNSUPPORTED;
      break;
    case CRASH_LOG_ACCESS_TYPE_LCL_BARID:
      //
      // Not support yet
      //
      Status = EFI_UNSUPPORTED;
      break;
    case CRASH_LOG_ACCESS_TYPE_LOCAL:
      Status = GetCpuCrashLogDiscHdrAddrSize (SocId, Entry, CrashLogDevscCap, &HdrAddr, &HdrSizeInDwords);
      if (!EFI_ERROR (Status)) {
        *Address = HdrAddr + HdrSizeInDwords * 4 + CrashLogDiscHdr->Fields.Address.Local.Offset;
        *SizeInDwords = CrashLogDiscHdr->Fields.Address.Local.SizeInDwords;
      }
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;

  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "can't get CPU CrashLog Region Address and size !\n"));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "CrashLog Region Address 0x%X, SizeInDwords 0x%x .\n", *Address, *SizeInDwords));
  return Status;
}

/**
  Get CPU CrashLog Discovery Header

  @param[IN]  SocId                Socket ID of the processor
  @param[IN]  Entry                Entry number of this header
  @param[IN]  CrashLogDevscCap     Structure of CrashLog DEVSC Data
  @param[OUT] CrashLogDiscHdr      Structure of CrashLog Discovery Header

  @retval EFI_UNSUPPORTED         Get Cpu CrashLog Discovery Header fail
  @retval EFI_SUCCESS             Get Cpu CrashLog Discovery Header success
**/
EFI_STATUS
GetCpuCrashLogDiscHdr (
  IN  UINT8                                   SocId,
  IN  UINT8                                   Entry,
  IN PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC  *CrashLogDevscCap,
  OUT CPU_CRASHLOG_DISCOVERY_HEADER           *CrashLogDiscHdr
  )
{
  EFI_STATUS             Status;
  UINT32                 Address;
  UINT32                 SizeInDwords;
  UINT8                  i;

  Status = GetCpuCrashLogDiscHdrAddrSize (SocId, Entry, CrashLogDevscCap, &Address, &SizeInDwords);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Fail to get CPU CrashLog Header!\n"));
    return Status;
  }

  ZeroMem (CrashLogDiscHdr, sizeof (CPU_CRASHLOG_DISCOVERY_HEADER));
  for (i = 0; i < SizeInDwords/2; i ++) {
    //
    // 64bit Read is neccessary for OOBMSM crashlog header
    //
    CrashLogDiscHdr->Data[i] = MmioRead64 (Address + i*8);
    DEBUG ((DEBUG_INFO, "CPU %d CrashLogDiscHdr->Data64[0x%x] = 0x%lx!\n", SocId, i, CrashLogDiscHdr->Data[i]));
  }

  return EFI_SUCCESS;
}

/**
  This function discovers CPU crashLog region and gets the size and base address

  @param[in]  SocketId            - Socket ID
  @param[out] CpuCrashLogRecRegs  - Stucture of CPU CrashLog Regions

  @retval EFI_UNSUPPORTED         Cpu CrashLog is not supported
  @retval EFI_SUCCESS             Cpu CrashLog is supported
**/
EFI_STATUS
CpuCrashLogDiscovery (
  IN  UINT8                               SocId,
  OUT CPU_CRASHLOG_RECORD_REGIONS_STRUCT  *CpuCrashLogRecRegs
  )
{
  EFI_STATUS                                Status;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC   CrashLogDevscCap;
  CRASHLOG_RECORD_REGION_INFO               CrashLogRecReg;
  CPU_CRASHLOG_DISCOVERY_HEADER             CrashLogDiscHdr;
  CRASHLOG_RECORD_HEADER_COMMON             CrashLogRecHdr;
  UINT8                                     Entry;
  BOOLEAN                                   ValidLogFound = FALSE;

  ZeroMem (&CrashLogDevscCap, sizeof (PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC));

  Status = GetCpuCrashLogCapability (SocId, &CrashLogDevscCap);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CPU CrashLog feature is not supported\n"));
    return Status;
  }

  for (Entry = 0; Entry < CrashLogDevscCap.DiscoveryData.Bits.NumEntries; Entry++) {
    ZeroMem (&CrashLogDiscHdr, sizeof (CPU_CRASHLOG_DISCOVERY_HEADER));
    Status = GetCpuCrashLogDiscHdr (SocId, Entry, &CrashLogDevscCap, &CrashLogDiscHdr);
    if (EFI_ERROR (Status)) {
      continue;
    }
/*
    //
    // SPR Crashlog doesn't support following two bits in discovery header
    //
    if (!CrashLogDiscHdr.Fields.CrashlogCapability) {
      DEBUG ((DEBUG_ERROR, "Socket %d entry %d CrashlogCapability is zero in CPU CrashLog Discovery Header!\n"));
      continue;
    }

    if (CrashLogDiscHdr.Fields.CrashLogDisabled) {
      DEBUG ((DEBUG_ERROR, "Socket %d entry %d CrashLogDisabled is set in CPU CrashLog Discovery Header!\n", SocId, Entry));
      continue;
    }
*/

    ZeroMem (&CrashLogRecReg, sizeof (CRASHLOG_RECORD_REGION_INFO));
    Status = GetCpuCrashLogRecRegAddrSize (SocId,
                                            Entry,
                                            &CrashLogDevscCap,
                                            &CrashLogDiscHdr,
                                            &CrashLogRecReg.Fields.Address,
                                            &CrashLogRecReg.Fields.SizeInDword
                                            );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "CpuCrashLogDiscovery - Socket %d entry %d -fail to get crash log region %r\n", SocId, Entry, Status));
      continue;
    }

    ZeroMem (&CrashLogRecHdr, sizeof (CRASHLOG_RECORD_HEADER_COMMON));
    CrashLogRecHdr.RecordVersion.Data = MmioRead32 (CrashLogRecReg.Fields.Address);
    if (CrashLogRecHdr.RecordVersion.Data == 0) {
      DEBUG ((DEBUG_ERROR, "CpuCrashLogDiscovery - Socket %d entry %d - first DWORD of Record is Zero\n", SocId, Entry));
      break;
    }

    if (CrashLogRecHdr.RecordVersion.Fields.Consumed) {
      DEBUG ((DEBUG_INFO, "Socket %d entry %d -Crash Record Already consumed!\n", SocId, Entry));
      break;
    }

    if (CrashLogRecHdr.RecordVersion.Fields.HeadType == CRASHLOG_HEADER_TYPE_LEGACY ||
        CrashLogRecHdr.RecordVersion.Fields.HeadType > CRASHLOG_HEADER_TYPE_4) {
      DEBUG ((DEBUG_INFO, "Socket %d entry %d Crash Record HeadType %d of record is not supported!\n", SocId, Entry, CrashLogRecHdr.RecordVersion.Fields.HeadType));
      break;
    }

    CpuCrashLogRecRegs->RegionInfo[SocId][Entry].Fields.SizeInDword = CrashLogRecReg.Fields.SizeInDword;
    CpuCrashLogRecRegs->RegionInfo[SocId][Entry].Fields.Address = CrashLogRecReg.Fields.Address;
    DEBUG ((DEBUG_INFO, "CpuCrashLogRecRegs->RegionInfo[0x%X][%d].Fields.Address = 0x%X\n", SocId, Entry, CpuCrashLogRecRegs->RegionInfo[SocId][Entry].Fields.Address));
    DEBUG ((DEBUG_INFO, "CpuCrashLogRecRegs->RegionInfo[0x%X][%d].Fields.SizeInDword = 0x%X\n", SocId, Entry, CpuCrashLogRecRegs->RegionInfo[SocId][Entry].Fields.SizeInDword));
    ValidLogFound = TRUE;
  }

  if (ValidLogFound) {
    return EFI_SUCCESS;
  } else {
    return EFI_UNSUPPORTED;
  }

}

/**
  This function ReArm cpu crash log

  @param[in] SocketId                  - Socket ID

  @retval EFI_SUCCESS

**/
EFI_STATUS
CpuCrashLogReArm (
  UINT8       SocId
  )
{
  MSM_BIOS_CRASHCONTROL_N0_MSM_PMON_STRUCT    CrashlogCtrl;

  CrashlogCtrl.Data = UsraCsrRead (SocId, 0, MSM_BIOS_CRASHCONTROL_N0_MSM_PMON_REG);

  if (!CrashlogCtrl.Bits.crashlogdiscovery) {
    return EFI_UNSUPPORTED;
  }

  CrashlogCtrl.Bits.rearmtrigger = 1;

  UsraCsrWrite (SocId, 0, MSM_BIOS_CRASHCONTROL_N0_MSM_PMON_REG, CrashlogCtrl.Data);
  return EFI_SUCCESS;
}

/**
  This function clears cpu crash log

  @param[in] SocketId                  - Socket ID

  @retval EFI_SUCCESS

**/
EFI_STATUS
CpuCrashLogClear (
  UINT8       SocId
  )
{
  return EFI_SUCCESS;
}

/**
  OOB: Disable ManualTrigger of CRASHLOG_CTRL_MSM_OOB_REG when the DisableCrashlog is 0

  @param SocId             - Socket ID

  @retval                  - None

**/
VOID
EFIAPI
CpuCrashLogDisableManualTrigger (
  UINT8 SocId
  )
{
  MSM_BIOS_CRASHCONTROL_N0_MSM_PMON_STRUCT    CrashlogCtrl;
  CrashlogCtrl.Data = UsraCsrRead (SocId, 0, MSM_BIOS_CRASHCONTROL_N0_MSM_PMON_REG);
  //
  // if the manualtrigger is 1, clear the manualtrigger.
  //
  if (CrashlogCtrl.Bits.manualtrigger == 1) {
    CrashlogCtrl.Bits.manualtrigger = 0;
    DEBUG ((DEBUG_INFO, "CpuCrashLogDisableManualTrigger CrashlogCtrl.Data = 0x%x!\n", CrashlogCtrl.Data));
    UsraCsrWrite (SocId, 0, MSM_BIOS_CRASHCONTROL_N0_MSM_PMON_REG, CrashlogCtrl.Data);
  }
}

/**
  This function enable/disables cpu crash log
  @param[in] SocketId                  - Socket ID
  @param[in] Enable                    - TRUE = Enable, FALSE = Disable

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED

**/
EFI_STATUS
CpuCrashLogEnableDisable (
  UINT8       SocId,
  BOOLEAN     Enable
  )
{
  MSM_BIOS_CRASHCONTROL_N0_MSM_PMON_STRUCT    CrashlogCtrl;
  CRASHLOG_CTRL_MSM_OOB_STRUCT                CrashlogCtrlMsmOob;

  CrashlogCtrl.Data = UsraCsrRead (SocId, 0, MSM_BIOS_CRASHCONTROL_N0_MSM_PMON_REG);

  if (!CrashlogCtrl.Bits.crashlogdiscovery) {
    return EFI_UNSUPPORTED;
  }

  if (Enable) {
    if (IsSiliconWorkaroundEnabled ("S22011038746")) {
      CrashlogCtrlMsmOob.Data = UsraCsrRead (SocId, 0, CRASHLOG_CTRL_MSM_OOB_REG);
      CrashlogCtrlMsmOob.Bits.disabletorcrashlog = 1;
      UsraCsrWrite (SocId, 0, CRASHLOG_CTRL_MSM_OOB_REG, CrashlogCtrlMsmOob.Data);
    }

    if (IsSiliconWorkaroundEnabled ("S14010299479")) {
      CpuCrashLogDisableManualTrigger (SocId);
      CrashlogCtrl.Data = UsraCsrRead (SocId, 0, MSM_BIOS_CRASHCONTROL_N0_MSM_PMON_REG);
    }
    CrashlogCtrl.Bits.disablecrashlog = 0;
  } else {
    CrashlogCtrl.Bits.disablecrashlog = 1;
  }

  UsraCsrWrite (SocId, 0, MSM_BIOS_CRASHCONTROL_N0_MSM_PMON_REG, CrashlogCtrl.Data);
  return EFI_SUCCESS;
}

/**
  This function enable/disables cpu crash log collect on all resets

  @param[in] SocketId                  - Socket ID
  @param[in] Enable                    - TRUE = Enable, FALSE = Disable

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED
**/
EFI_STATUS
CpuCrashLogOnAllReset (
  UINT8       SocId,
  BOOLEAN     Enable
  )
{
  MSM_BIOS_CRASHCONTROL_N0_MSM_PMON_STRUCT    CrashlogCtrl;

  CrashlogCtrl.Data = UsraCsrRead (SocId, 0, MSM_BIOS_CRASHCONTROL_N0_MSM_PMON_REG);

  if (!CrashlogCtrl.Bits.crashlogdiscovery) {
    return EFI_UNSUPPORTED;
  }

  if (Enable) {
    CrashlogCtrl.Bits.collectonallresets = 1;
  } else {
    CrashlogCtrl.Bits.collectonallresets = 0;
  }

  UsraCsrWrite (SocId, 0, MSM_BIOS_CRASHCONTROL_N0_MSM_PMON_REG, CrashlogCtrl.Data);
  return EFI_SUCCESS;
}

/**
  Init PMon crash log Bar

  @param[in] SocketId                  - Socket ID
  @param[in] CrashLogDevscCap          - Structure of CrashLog DEVSC Data
  @param[in] SavedStsCmd               - the saved value of PMon PCICFG status and command register

  @retval FALSE - Crash Log Bar already asigned
  @retval TRUE -  Crash Log Bar asigned by this routine
**/
BOOLEAN
InitOobMsmPmonCrashlogBar (
  IN  UINT8                                    SocketId,
  IN  PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC  *CrashLogDevscCap,
  OUT UINT32                                   *SavedStsCmd
  )
{

  PCI_CFG_STATUS_COMMAND_DISC_MSM_PMON_STRUCT PmonCfgCommand;
  UINT32                                      BarCsrAddr = 0;
  UINT32                                      Data32;
  BOOLEAN                                     Return;

  if (CrashLogDevscCap->DiscoveryData.Bits.TBir == V_CRASHLOG_DVSEC_TBIR_BAR0) {
    BarCsrAddr = PCI_CFG_BAR0_DISC_MSM_PMON_REG;
  } else if (CrashLogDevscCap->DiscoveryData.Bits.TBir == V_CRASHLOG_DVSEC_TBIR_BAR1) {
    BarCsrAddr = PCI_CFG_BAR1_DISC_MSM_PMON_REG;
  }

  PmonCfgCommand.Data = UsraCsrRead (SocketId, 0, PCI_CFG_STATUS_COMMAND_DISC_MSM_PMON_REG);
  *SavedStsCmd = PmonCfgCommand.Data;
  DEBUG ((DEBUG_INFO, "InitOobMsmPmonCrashlogBar SavedStsCmd = 0x%x!\n", *SavedStsCmd));

  PmonCfgCommand.Bits.memory_space_ena = 0;
  PmonCfgCommand.Bits.bus_master_ena = 0;
  UsraCsrWrite (SocketId, 0, PCI_CFG_STATUS_COMMAND_DISC_MSM_PMON_REG, PmonCfgCommand.Data);

  Data32 = UsraCsrRead (SocketId, 0, BarCsrAddr);

  if ( (Data32 & 0xFFFFFFF0) == 0 ) {
    //
    // MSM device is on Stack8 in SPR
    //
    Data32 = KTI_HOST_OUT_PTR->CpuInfo[SocketId].CpuRes.StackRes[8].MmiolBase;
    Data32 &= 0xFFFFFFF0;
    DEBUG ((DEBUG_INFO, "InitOobMsmPmonCrashlogBar Bar 0x%x = 0x%x!\n", (BarCsrAddr & 0xfff), Data32));
    UsraCsrWrite (SocketId, 0, BarCsrAddr, Data32);
    Return = TRUE;
  } else {
    Return = FALSE;
  }

  PmonCfgCommand.Data = UsraCsrRead (SocketId, 0, PCI_CFG_STATUS_COMMAND_DISC_MSM_PMON_REG);
  PmonCfgCommand.Bits.memory_space_ena = 1;
  PmonCfgCommand.Bits.bus_master_ena = 1;
  UsraCsrWrite (SocketId, 0, PCI_CFG_STATUS_COMMAND_DISC_MSM_PMON_REG, PmonCfgCommand.Data);
  return Return;
}

/**
  Clear PMon crash log Bar and restore the PMon PCICFG status and command register value

  @param[in] SocketId                  - Socket ID
  @param[in] CrashLogDevscCap          - Structure of CrashLog DEVSC Data
  @param[in] SavedStsCmd               - the restore value of PMon PCICFG status and command register
  @param[in] ClearBar                  - if TRUE need to clear the bar, else don't

  return none
**/
VOID
ClearOobMsmPmonCrashlogBar (
  IN UINT8                                    SocketId,
  IN PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC  *CrashLogDevscCap,
  IN UINT32                                   SavedStsCmd,
  IN BOOLEAN                                  ClearBar
  )
{
  PCI_CFG_STATUS_COMMAND_DISC_MSM_PMON_STRUCT PmonCfgCommand;
  PCI_CFG_BAR0_DISC_MSM_PMON_STRUCT           PmonBar;
  UINT32                                      BarCsrAddr = 0;

  if (CrashLogDevscCap->DiscoveryData.Bits.TBir == V_CRASHLOG_DVSEC_TBIR_BAR0) {
    BarCsrAddr = PCI_CFG_BAR0_DISC_MSM_PMON_REG;
  } else if (CrashLogDevscCap->DiscoveryData.Bits.TBir == V_CRASHLOG_DVSEC_TBIR_BAR1) {
    BarCsrAddr = PCI_CFG_BAR1_DISC_MSM_PMON_REG;
  }

  PmonCfgCommand.Data = UsraCsrRead (SocketId, 0, PCI_CFG_STATUS_COMMAND_DISC_MSM_PMON_REG);
  PmonCfgCommand.Bits.memory_space_ena = 0;
  PmonCfgCommand.Bits.bus_master_ena = 0;
  UsraCsrWrite (SocketId, 0, PCI_CFG_STATUS_COMMAND_DISC_MSM_PMON_REG, PmonCfgCommand.Data);

  if (ClearBar) {
    PmonBar.Data = UsraCsrRead (SocketId, 0, BarCsrAddr);
    PmonBar.Bits.base_addr = 0;
    UsraCsrWrite (SocketId, 0, BarCsrAddr, PmonBar.Data);
  }

  PmonCfgCommand.Data = SavedStsCmd;
  DEBUG ((DEBUG_INFO, "ClearOobMsmPmonCrashlogBar SavedStsCmd = 0x%x!\n", SavedStsCmd));
  UsraCsrWrite (SocketId, 0, PCI_CFG_STATUS_COMMAND_DISC_MSM_PMON_REG, PmonCfgCommand.Data);
  return;
}
