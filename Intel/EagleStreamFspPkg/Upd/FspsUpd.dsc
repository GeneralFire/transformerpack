## @file
#  Platform description.
#
# @copyright
#  INTEL CONFIDENTIAL
#  Copyright 2019 Intel Corporation.
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
  # UPDs consumed in FspSiliconInit API (FSP-S)
  #
  ################################################################################

  # !BSF FIND:{ICXUPD_S}
  # !HDR COMMENT:{FSP_UPD_HEADER:FSP UPD Header}
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:START}
  # FspsUpdSignature: {ICXUPD_S}
  gEagleStreamFspPkgTokenSpaceGuid.Signature                   | * | 0x08 | 0x535F445055434F53
  # !BSF NAME:{FspsUpdRevision}  TYPE:{None}
  gEagleStreamFspPkgTokenSpaceGuid.Revision                    | * | 0x01 | 0x01
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:END}
  gEagleStreamFspPkgTokenSpaceGuid.Reserved                    | * | 0x17 | {0x00}

  # !HDR COMMENT:{FSPS_CONFIG:FSP-S Configuration}
  # !HDR EMBED:{FSPS_CONFIG:FspsConfig:START}
  # !BSF PAGE:{ICX}

  # !BSF NAME:{PCIe Controller 0 Bifurcation} TYPE:{Combo}
  # !BSF OPTION:{0:X2X2X2X2, 1:X2X2X4, 2:X4X2X2, 3:X4X4, 4:X8}
  # !BSF HELP:{Configure PCI Express controller 0 bifurcation.}
  gEagleStreamFspPkgTokenSpaceGuid.BifurcationPcie0            | * | 0x01 | 4

  # !BSF NAME:{PCIe Controller 1 Bifurcation} TYPE:{Combo}
  # !BSF OPTION:{0:X2X2X2X2, 1:X2X2X4, 2:X4X2X2, 3:X4X4, 4:X8}
  # !BSF HELP:{Configure PCI Express controller 1 bifurcation.}
  gEagleStreamFspPkgTokenSpaceGuid.BifurcationPcie1            | * | 0x01 | 3

  # !BSF NAME:{Active Core Count} TYPE:{Combo}
  # !BSF OPTION:{0:ALL, 1:1, 2:2, 3:3, 4:4, 5:5, 6:6, 7:7, 8:8, 9:9, 10:10, 11:11, 12:12, 13:13, 14:14, 15:15}
  # !BSF HELP:{Select # of Active Cores (Default: 0, 0:ALL, 1..15 = 1..15 Cores)}
  gEagleStreamFspPkgTokenSpaceGuid.ActiveCoreCount             | * | 0x01 | 0

  gEagleStreamFspPkgTokenSpaceGuid.CpuMicrocodePatchBase       | * | 0x04 | 0
  gEagleStreamFspPkgTokenSpaceGuid.CpuMicrocodePatchSize       | * | 0x04 | 0

  # !BSF NAME:{PCIe Controller 0} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable / Disable PCI Express controller 0}
  gEagleStreamFspPkgTokenSpaceGuid.EnablePcie0                 | * | 0x01 | 1

  # !BSF NAME:{PCIe Controller 1} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable / Disable PCI Express controller 1}
  gEagleStreamFspPkgTokenSpaceGuid.EnablePcie1                 | * | 0x01 | 1


  # !BSF NAME:{Embedded Multi-Media Controller (eMMC)} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable / Disable Embedded Multi-Media controller}
  gEagleStreamFspPkgTokenSpaceGuid.EnableEmmc                  | * | 0x01 | 1

  # !BSF NAME:{LAN Controllers} TYPE:{Combo}
  # !BSF OPTION:{0:Disable LAN 0 & LAN 1, 1:Enable LAN 0 & LAN 1, 2:Disable LAN 1 only}
  # !BSF HELP:{Enable / Disable LAN controllers, refer to FSP Integration Guide for details.}
  gEagleStreamFspPkgTokenSpaceGuid.EnableGbE                   | * | 0x01 | 1

  gEagleStreamFspPkgTokenSpaceGuid.FiaMuxConfigRequestPtr      | * | 0x04 | 0

  # !BSF NAME:{PCIe Root Port 0 DeEmphasis} TYPE:{Combo}
  # !BSF OPTION:{0:6dB, 1:3.5dB}
  # !BSF HELP:{Desired DeEmphasis level for PCIE root port}
  gEagleStreamFspPkgTokenSpaceGuid.PcieRootPort0DeEmphasis     | * | 0x01 | 0

  # !BSF NAME:{PCIe Root Port 1 DeEmphasis} TYPE:{Combo}
  # !BSF OPTION:{0:6dB, 1:3.5dB}
  # !BSF HELP:{Desired DeEmphasis level for PCIE root port}
  gEagleStreamFspPkgTokenSpaceGuid.PcieRootPort1DeEmphasis     | * | 0x01 | 0

  # !BSF NAME:{PCIe Root Port 2 DeEmphasis} TYPE:{Combo}
  # !BSF OPTION:{0:6dB, 1:3.5dB}
  # !BSF HELP:{Desired DeEmphasis level for PCIE root port}
  gEagleStreamFspPkgTokenSpaceGuid.PcieRootPort2DeEmphasis     | * | 0x01 | 0

  # !BSF NAME:{PCIe Root Port 3 DeEmphasis} TYPE:{Combo}
  # !BSF OPTION:{0:6dB, 1:3.5dB}
  # !BSF HELP:{Desired DeEmphasis level for PCIE root port}
  gEagleStreamFspPkgTokenSpaceGuid.PcieRootPort3DeEmphasis     | * | 0x01 | 0

  # !BSF NAME:{PCIe Root Port 4 DeEmphasis} TYPE:{Combo}
  # !BSF OPTION:{0:6dB, 1:3.5dB}
  # !BSF HELP:{Desired DeEmphasis level for PCIE root port}
  gEagleStreamFspPkgTokenSpaceGuid.PcieRootPort4DeEmphasis     | * | 0x01 | 0

  # !BSF NAME:{PCIe Root Port 5 DeEmphasis} TYPE:{Combo}
  # !BSF OPTION:{0:6dB, 1:3.5dB}
  # !BSF HELP:{Desired DeEmphasis level for PCIE root port}
  gEagleStreamFspPkgTokenSpaceGuid.PcieRootPort5DeEmphasis     | * | 0x01 | 0

  # !BSF NAME:{PCIe Root Port 6 DeEmphasis} TYPE:{Combo}
  # !BSF OPTION:{0:6dB, 1:3.5dB}
  # !BSF HELP:{Desired DeEmphasis level for PCIE root port}
  gEagleStreamFspPkgTokenSpaceGuid.PcieRootPort6DeEmphasis     | * | 0x01 | 0

  # !BSF NAME:{PCIe Root Port 7 DeEmphasis} TYPE:{Combo}
  # !BSF OPTION:{0:6dB, 1:3.5dB}
  # !BSF HELP:{Desired DeEmphasis level for PCIE root port}
  gEagleStreamFspPkgTokenSpaceGuid.PcieRootPort7DeEmphasis     | * | 0x01 | 0

  gEagleStreamFspPkgTokenSpaceGuid.EMMCDLLConfigPtr            | * | 0x04 | 0

  # !HDR EMBED:{FSPS_CONFIG:FspsConfig:END}
  gEagleStreamFspPkgTokenSpaceGuid.ReservedSiliconInitUpd      | * | 0x10 | {0x00}

  # Note please keep "UpdTerminator" at the end of the UPD region.
  # The tool will use this field to determine the actual end of the UPD data
  # structure. Anything after this field will still be in the UPD binary block,
  # but will be excluded in the final UPD_DATA_REGION data structure.
  gEagleStreamFspPkgTokenSpaceGuid.UpdTerminator               | * | 0x02 | 0x55AA