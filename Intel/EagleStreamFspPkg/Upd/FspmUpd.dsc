## @file
#  Platform description.
#
# @copyright
#  INTEL CONFIDENTIAL
#  Copyright 2019 - 2020 Intel Corporation.
#
#  The source code contained or described herein and all documents related to the
#  source code ("Material") are owned by Intel Corporation or its suppliers or
#  licensors. Title to the Material remains with Intel Corporation or its suppliers
#  and licensors. The Material may contain trade secrets and proprietary and
#  confidential information of Intel Corporation and its suppliers and licensors,
#  and is protected by worldwide copyright and trade secret laws and treaty
#  provisions. No part of the Material may be used, copied, reproduced, modified,
#  published, uploaded, posted, transmitted, distributed, or disclosed in any way
#  without Intel's prior express written permission.
#
#  No license under any patent, copyright, trade secret or other intellectual
#  property right is granted to or conferred upon you by disclosure or delivery
#  of the Materials, either expressly, by implication, inducement, estoppel or
#  otherwise. Any license under such intellectual property rights must be
#  express and approved by Intel in writing.
#
#  Unless otherwise agreed by Intel in writing, you may not remove or alter
#  this notice or any other notice embedded in Materials by Intel or
#  Intel's suppliers or licensors in any way.
#
#  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
#  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
#  the terms of your license agreement with Intel or your vendor. This file may
#  be modified by the user, subject to additional terms of the license agreement.
#
# @par Specification
##

  ################################################################################
  #
  # UPDs consumed in FspMemoryInit API (FSP-M)
  #
  ################################################################################

  # !BSF FIND:{ICXUPD_M}
  # !HDR COMMENT:{FSP_UPD_HEADER:FSP UPD Header}
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:START}
  # FspmUpdSignature: {ICXUPD_M}
  gEagleStreamFspPkgTokenSpaceGuid.Signature                   | * | 0x08 | 0x4D5F445055434F53
  # !BSF NAME:{FspmUpdRevision}  TYPE:{None}
  gEagleStreamFspPkgTokenSpaceGuid.Revision                    | * | 0x01 | 0x01
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:END}
  gEagleStreamFspPkgTokenSpaceGuid.Reserved                    | * | 0x17 | {0x00}

  # !HDR COMMENT:{FSPM_ARCH_UPD:FSP-M Architectural UPD}
  # !HDR EMBED:{FSPM_ARCH_UPD:FspmArchUpd:START}
  # !BSF PAGE:{MEM}

  gEagleStreamFspPkgTokenSpaceGuid.Revision                    | * | 0x01 | 0x01
  gEagleStreamFspPkgTokenSpaceGuid.Reserved                    | * | 0x03 | {0x00}

  # !HDR STRUCT:{VOID*}
  gEagleStreamFspPkgTokenSpaceGuid.NvsBufferPtr                | * | 0x04 | 0x00000000

  # !HDR STRUCT:{VOID*}
  # !BSF NAME:{StackBase}
  # !BSF HELP:{Stack base for FSP use. Default: 0xFEFB0000}
  gEagleStreamFspPkgTokenSpaceGuid.StackBase                   | * | 0x04 | 0xFDF40000

  # !BSF NAME:{StackSize}
  # !BSF HELP:{To pass the stack size for FSP use. Bootloader can programmatically get the FSP requested StackSize by using the defaults in the FSP-M component. This is the minimum stack size expected by this revision of FSP. Default: 0x4FF00}
  gEagleStreamFspPkgTokenSpaceGuid.StackSize                   | * | 0x04 | 0x0001FF00

  # !BSF NAME:{BootLoaderTolumSize}
  # !BSF HELP:{To pass Bootloader Tolum size.}
  gEagleStreamFspPkgTokenSpaceGuid.BootLoaderTolumSize         | * | 0x04 | 0x00000000

  # !BSF NAME:{Bootmode}
  # !BSF HELP:{To maintain Bootmode details.}
  gEagleStreamFspPkgTokenSpaceGuid.Bootmode                    | * | 0x04 | 0x00000000

  # !HDR EMBED:{FSPM_ARCH_UPD:FspmArchUpd:END}
  gEagleStreamFspPkgTokenSpaceGuid.Reserved1                   | * | 0x08 | {0x00}

  # !HDR COMMENT:{FSPM_CONFIG:FSP-M Configuration}
  # !HDR EMBED:{FSPM_CONFIG:FspmConfig:START}

  # !BSF PAGE:{PLT}
  # !BSF NAME:{Customer Revision} TYPE:{EditText}
  # !BSF HELP:{The Customer can set this revision string for their own purpose.}
  gEagleStreamFspPkgTokenSpaceGuid.CustomerRevision            | * | 0x20 | {0x76,0x65,0x72,0x73,0x69,0x6F,0x6E,0x20,0x78,0x78,0x78,0x00}

  # !BSF NAME:{Bus Ratio} TYPE:{EditNum, HEX, (0x0, 0x8)}
  # !BSF HELP:{Indicates the ratio of Bus/MMIOL/IO resource to be allocated for each CPU's IIO}
  gEagleStreamFspPkgTokenSpaceGuid.BusRatio                    | * | 0x08 | {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}

  # !BSF NAME:{D2K Credit Config} TYPE:{Combo} OPTION:{1:Min, 2:Med, 3:Max}
  # !BSF HELP:{Set the D2K Credit Config}
  gEagleStreamFspPkgTokenSpaceGuid.D2KCreditConfig             | * | 0x01 | 0x02

  # !BSF NAME:{Snoop Throttle Config} TYPE:{Combo} OPTION:{0:DIS, 1:Min, 2:Med, 3:Max}
  # !BSF HELP:{Set the Snoop Throttle Config}
  gEagleStreamFspPkgTokenSpaceGuid.SnoopThrottleConfig         | * | 0x01 | 0x00

  # !BSF NAME:{Legacy VGA Soc} TYPE:{EditNum, HEX, (0x00,0x03)}
  # !BSF HELP:{Socket that claims the legacy VGA range}
  gEagleStreamFspPkgTokenSpaceGuid.LegacyVgaSoc                | * | 0x01 | 0x00

  # !BSF NAME:{Legacy VGA Stack} TYPE:{EditNum, HEX, (0x00,0x06)}
  # !BSF HELP:{Stack that claims the legacy VGA range}
  gEagleStreamFspPkgTokenSpaceGuid.LegacyVgaStack              | * | 0x01 | 0x00

  # !BSF NAME:{Pcie P2P Performance Mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Determine if to enable PCIe P2P Performance Mode}
  gEagleStreamFspPkgTokenSpaceGuid.P2pRelaxedOrdering          | * | 0x01 | 0x00

  # !BSF NAME:{Debug Print Level} TYPE:{Combo} OPTION:{1:Fatal, 2:Warning, 4:Summary, 8:Detail, 0x0F:All}
  # !BSF HELP:{Set Debug Print Level}
  gEagleStreamFspPkgTokenSpaceGuid.DebugPrintLevel             | * | 0x01 | 0x0F

  # !BSF NAME:{SNC} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable SNC}
  gEagleStreamFspPkgTokenSpaceGuid.SncEn                       | * | 0x01 | 0x01

  # !BSF NAME:{UMA Clustering} TYPE:{Combo} OPTION:{0:Disable, 2:Two Clusters, 4:Four Clusters}
  # !BSF HELP:{Set UMA Clusters}
  gEagleStreamFspPkgTokenSpaceGuid.UmaClustering               | * | 0x01 | 0x00

  # !BSF NAME:{IODC Mode} TYPE:{Combo} OPTION:{0:Disable, 1:Auto, 2:Push, 3:AllocFlow 4:NonAlloc, 5:WCILF}
  # !BSF HELP:{IODC Setup Option}
  gEagleStreamFspPkgTokenSpaceGuid.IoDcMode                    | * | 0x01 | 0x01

  # !BSF NAME:{Degrade Precedence} TYPE:{Combo} OPTION:{0:Topology, 1:Feature}
  # !BSF HELP:{Setup Degrade Precedence}
  gEagleStreamFspPkgTokenSpaceGuid.DegradePrecedence           | * | 0x01 | 0x00

  # !BSF NAME:{Degrade 4 Socket Preference} TYPE:{Combo} OPTION:{0:Fully Connect, 1:Dual Link Ring}
  # !BSF HELP:{Setup Degrade 4 Socket Preference}
  gEagleStreamFspPkgTokenSpaceGuid.Degrade4SPreference         | * | 0x01 | 0x00

  # !BSF NAME:{Directory Mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable Directory Mode}
  gEagleStreamFspPkgTokenSpaceGuid.DirectoryModeEn             | * | 0x01 | 0x01

  # !BSF NAME:{XPT Prefetch Enable} TYPE:{Combo} OPTION{0:Disable, 1:Enable, 2:Auto)}
  # !BSF HELP:{Enable or Disable XPT Prefetch}
  gEagleStreamFspPkgTokenSpaceGuid.XptPrefetchEn               | * | 0x01 | 0x02

  # !BSF NAME:{KTI Prefetch Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable KTI Prefetch}
  gEagleStreamFspPkgTokenSpaceGuid.KtiPrefetchEn               | * | 0x01 | 0x01

  # !BSF NAME:{XPT Remote Prefetch Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable XPT Remote Prefetch Enable}
  gEagleStreamFspPkgTokenSpaceGuid.XptRemotePrefetchEn         | * | 0x01 | 0x00

  # !BSF NAME:{KTI FPGA} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable KTI FPGA}
  gEagleStreamFspPkgTokenSpaceGuid.KtiFpgaEnable               | * | 0x08 | {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}

  # !BSF NAME:{DDRT QoS Mode} TYPE:{Combo} OPTION{0:Mode 0, 1:Mode 1, 2:Mode 2)}
  # !BSF HELP:{Setup DDRT QoS}
  gEagleStreamFspPkgTokenSpaceGuid.DdrtQosMode                 | * | 0x01 | 0x00

  # !BSF NAME:{KTI Link Speed Mode} TYPE:{Combo} OPTION{0:Slow, 1:Full)}
  # !BSF HELP:{Choose KTI Link Speed Mode}
  gEagleStreamFspPkgTokenSpaceGuid.KtiLinkSpeedMode            | * | 0x01 | 0x01

  # !BSF NAME:{KTI Link Speed} TYPE:{Combo} OPTION{0:128GT, 1:144GT, 2:160GT, 3:Max KTI Link Speed, 4:Frequency Per Link)}
  # !BSF HELP:{Setup KTI Link Speed}
  gEagleStreamFspPkgTokenSpaceGuid.KtiLinkSpeed                | * | 0x01 | 0x03

  # !BSF NAME:{KTI Link L0p} TYPE:{Combo} OPTION{0:Disable, 1:Enable, 2: Auto)}
  # !BSF HELP:{Enable or Disable KTI Link L0p}
  gEagleStreamFspPkgTokenSpaceGuid.KtiLinkL0pEn                | * | 0x01 | 0x02

  # !BSF NAME:{KTI Link L1} TYPE:{Combo} OPTION{0:Disable, 1:Enable, 2: Auto)}
  # !BSF HELP:{Enable or Disable KTI Link L1}
  gEagleStreamFspPkgTokenSpaceGuid.KtiLinkL1En                 | * | 0x01 | 0x02

  # !BSF NAME:{KTI Failover} TYPE:{Combo} OPTION{0:Disable, 1:Enable, 2: Auto)}
  # !BSF HELP:{Enable or Disable KTI Failover}
  gEagleStreamFspPkgTokenSpaceGuid.KtiFailoverEn               | * | 0x01 | 0x02

  # !BSF NAME:{KTI LB Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable KTI LB}
  gEagleStreamFspPkgTokenSpaceGuid.KtiLbEn                     | * | 0x01 | 0x00

  # !BSF NAME:{KTI CRC Mode} TYPE:{Combo} OPTION:{0:16bit, 1:32bit, 2:Auto}
  # !BSF HELP:{Select KTI CRC Mode}
  gEagleStreamFspPkgTokenSpaceGuid.KtiCrcMode                  | * | 0x01 | 0x02

  # !BSF NAME:{KTI CPU Socket Hotplug} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable KTI CPU Socket Hotplug}
  gEagleStreamFspPkgTokenSpaceGuid.KtiCpuSktHotPlugEn          | * | 0x01 | 0x00

  # !BSF NAME:{KTI CPU Socket HotPlug Topology} TYPE:{Combo} OPTION:{0:4Socket, 1:8Socket}
  # !BSF HELP:{Select KTI CPU Socket HotPlug Topology}
  gEagleStreamFspPkgTokenSpaceGuid.KtiCpuSktHotPlugTopology    | * | 0x01 | 0x00

  # !BSF NAME:{KTI SKU Mismatch Check} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable KTI SKU Mismatch Check}
  gEagleStreamFspPkgTokenSpaceGuid.KtiSkuMismatchCheck         | * | 0x01 | 0x01

  # !BSF NAME:{IRQ Threshold} TYPE:{Combo} OPTION:{0:Disable, 1:Auto, 2:Low, 3:Medium, 4:High}
  # !BSF HELP:{Select IRQ Threshold}
  gEagleStreamFspPkgTokenSpaceGuid.IrqThreshold                | * | 0x01 | 0x01

  # !BSF NAME:{IRQ Threshold} TYPE:{Combo} OPTION:{0:Disable, 1:Auto, 2:Low, 3:Medium, 4:High}
  # !BSF HELP:{Enable or Disable }
  gEagleStreamFspPkgTokenSpaceGuid.TorThresLoctoremNorm        | * | 0x01 | 0x01

  # !BSF NAME:{TOR threshold - Loctorem threshold empty} TYPE:{Combo} OPTION:{0:Disable, 1:Auto, 2:Low, 3:Medium, 4:High}
  # !BSF HELP:{Select TOR threshold - Loctorem threshold empty}
  gEagleStreamFspPkgTokenSpaceGuid.TorThresLoctoremEmpty       | * | 0x01 | 0x01

  # !BSF NAME:{TSC Sync in Sockets} TYPE:{Combo} OPTION{0:Disable, 1:Enable, 2:Auto)}
  # !BSF HELP:{Enable or Disable TSC Sync in Sockets}
  gEagleStreamFspPkgTokenSpaceGuid.TscSyncEn                   | * | 0x01 | 0x02

  # !BSF NAME:{HA A to S directory optimization} TYPE:{Combo} OPTION{0:Disable, 1:Enable, 2:Auto)}
  # !BSF HELP:{Enable or Disable HA A to S directory optimization}
  gEagleStreamFspPkgTokenSpaceGuid.StaleAtoSOptEn              | * | 0x01 | 0x02

  # !BSF NAME:{LLC Deadline Allocation} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable LLC Deadline Allocation}
  gEagleStreamFspPkgTokenSpaceGuid.LLCDeadLineAlloc            | * | 0x01 | 0x01

  # !BSF NAME:{Split Lock} TYPE:{Combo} OPTION{0:Disable, 1:Enable, 2:Auto)}
  # !BSF HELP:{Enable or Disable Split Lock}
  gEagleStreamFspPkgTokenSpaceGuid.SplitLock                   | * | 0x01 | 0x00

  # !BSF NAME:{MMCFG Base Address} TYPE:{Combo} OPTION:{0:1G, 1:1.5G, 2:1.75G, 3:2G, 4:2.25G, 5:3G, 6:Auto}
  # !BSF HELP:{Setup MMCFG Base Address}
  gEagleStreamFspPkgTokenSpaceGuid.mmCfgBase                   | * | 0x01 | 0x06

  # !BSF NAME:{MMCFG Size} TYPE:{Combo} OPTION:{0:64M, 1:128M, 2:256M, 3:512M, 4:1G, 5:2G, 6: Auto}
  # !BSF HELP:{Select MMCFG Size}
  gEagleStreamFspPkgTokenSpaceGuid.mmCfgSize                   | * | 0x01 | 0x06

  # !BSF NAME:{MMIO High Base Address} TYPE:{EditNum, HEX, (0x0000, 0xFFFFFFFF)}
  # !BSF HELP:{MMIO High Base Address, a hex number for Bit[51:32]}
  gEagleStreamFspPkgTokenSpaceGuid.mmiohBase                   | * | 0x04 | 0x06

  # !BSF NAME:{High Gap} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable High Gap}
  gEagleStreamFspPkgTokenSpaceGuid.highGap                     | * | 0x01 | 0x00

  # !BSF NAME:{MMIO High Size} TYPE:{EditNum, DEC, (1, 1024)}
  # !BSF HELP:{ MMIO High Size, Number of 1GB contiguous regions to be assigned for MMIOH space per CPU.  Range 1-1024}
  gEagleStreamFspPkgTokenSpaceGuid.mmiohSize                   | * | 0x02 | 0x03

  # !BSF NAME:{} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable }
  gEagleStreamFspPkgTokenSpaceGuid.isocEn                      | * | 0x01 | 0x01

  # !BSF NAME:{DCA} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable DCA}
  gEagleStreamFspPkgTokenSpaceGuid.dcaEn                       | * | 0x01 | 0x00

  # !BSF NAME:{} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable }
  gEagleStreamFspPkgTokenSpaceGuid.BoardTypeBitmask            | * | 0x04 | 0x01

  # !BSF NAME:{} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable }
  gEagleStreamFspPkgTokenSpaceGuid.AllLanesPtr                 | * | 0x04 | 0x01

  # !BSF NAME:{} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable }
  gEagleStreamFspPkgTokenSpaceGuid.PerLanePtr                  | * | 0x04 | 0x01

  # !BSF NAME:{} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable }
  gEagleStreamFspPkgTokenSpaceGuid.AllLanesSizeOfTable         | * | 0x04 | 0x01

  # !BSF NAME:{} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable }
  gEagleStreamFspPkgTokenSpaceGuid.PerLaneSizeOfTable          | * | 0x04 | 0x01

  # !BSF NAME:{} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable }
  gEagleStreamFspPkgTokenSpaceGuid.WaitTimeForPSBP             | * | 0x04 | 0x01

  # !BSF NAME:{} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable }
  gEagleStreamFspPkgTokenSpaceGuid.IsKtiNvramDataReady         | * | 0x01 | 0x01

  # !BSF NAME:{} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable }
  gEagleStreamFspPkgTokenSpaceGuid.WaSerializationEn           | * | 0x01 | 0x01

  # !BSF NAME:{} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable }
  gEagleStreamFspPkgTokenSpaceGuid.KtiInEnableMktme            | * | 0x01 | 0x01

  # !BSF NAME:{} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable }
  #gEagleStreamFspPkgTokenSpaceGuid.             | * | 0x01 | 0x01

  # !HDR EMBED:{FSPM_CONFIG:FspmConfig:END}
  gEagleStreamFspPkgTokenSpaceGuid.ReservedMemoryInitUpd        | * | 0x10 | {0x00}

  # Note please keep "UpdTerminator" at the end of each UPD region.
  # The tool will use this field to determine the actual end of the UPD data
  # structure.
  gEagleStreamFspPkgTokenSpaceGuid.UpdTerminator                | * | 0x02 | 0x55AA