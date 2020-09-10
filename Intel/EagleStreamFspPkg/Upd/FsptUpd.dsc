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

  #
  # Comments with !BSF will be used to generate BSF file
  # Comments with !HDR will be used to generate H header file
  #

  # Global definitions in BSF
  # !BSF PAGES:{MRC:"MRC & Early SoC", SOC:"SoC", PLT:"Platform Specific"}
  # !BSF BLOCK:{NAME:"10nmSoc Platform", VER:"0.1"}

  ################################################################################
  #
  # UPDs consumed in FspTempRamInit API (FSP-T)
  #
  ################################################################################

  # !BSF FIND:{ICXUPD_T}
  # !HDR COMMENT:{FSP_UPD_HEADER:FSP UPD Header}
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:START}
  # FsptUpdSignature: {ICXUPD_T}
  gEagleStreamFspPkgTokenSpaceGuid.Signature                      | * | 0x08 | 0x545F445055434F53
  # !BSF NAME:{FsptUpdRevision}  TYPE:{None}
  gEagleStreamFspPkgTokenSpaceGuid.Revision                       | * | 0x01 | 0x01
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:END}
  gEagleStreamFspPkgTokenSpaceGuid.Reserved                       | * | 0x17 | {0x00}

  # !HDR COMMENT:{FSPT_CORE_UPD:FSP-T Core UPD}
  # !HDR EMBED:{FSPT_CORE_UPD:FsptCoreUpd:START}

  # Base address of the microcode region.
  gEagleStreamFspPkgTokenSpaceGuid.MicrocodeRegionBase            | * | 0x04 | 0x00000000

  # Length of the microcode region.
  gEagleStreamFspPkgTokenSpaceGuid.MicrocodeRegionLength          | * | 0x04 | 0x00000000

  # Base address of the cacheable flash region.
  gEagleStreamFspPkgTokenSpaceGuid.CodeRegionBase                 | * | 0x04 | 0x00000000

  # Length of the cacheable flash region.
  gEagleStreamFspPkgTokenSpaceGuid.CodeRegionLength               | * | 0x04 | 0x00000000

  # !HDR EMBED:{FSPT_CORE_UPD:FsptCoreUpd:END}
  gEagleStreamFspPkgTokenSpaceGuid.Reserved1                      | * | 0x10 | {0x00}

  # !HDR COMMENT:{FSPT_CONFIG:FSP-T Configuration}
  # !HDR EMBED:{FSPT_CONFIG:FsptConfig:START}

  #
  # Enable/Disable diversion of LPC Port80 writes
  #   0:VPD-Style[Default], 1:Skip LGIR check/disable [DEFAULT], 2:Override LPC Decode Enable for Port 80 (LGIRx)
  #
  # !BSF PAGE:{ICX}
  # !BSF NAME:{Disable Port80 output in FSP-T} TYPE:{Combo}
  # !BSF OPTION:{0:VPD-Style, 1:Enable Port80 Output[Default], 2:Disable Port80 Output}
  # !BSF HELP:{Select Port80 Control in FSP-T (0:VPD-Style, 1:Enable Port80 Output, 2:Disable Port80 Output, refer to FSP Integration Guide for details},
  gEagleStreamFspPkgTokenSpaceGuid.FsptPort80RouteDisable         | * | 0x01 | 0x01

  # !HDR EMBED:{FSPT_CONFIG:FsptConfig:END}
  gEagleStreamFspPkgTokenSpaceGuid.ReservedTempRamInitUpd         | * | 0x1f | {0x00}

  # Note please keep "UpdTerminator" at the end of each UPD region.
  # The tool will use this field to determine the actual end of the UPD data
  # structure.
  gEagleStreamFspPkgTokenSpaceGuid.UpdTerminator                  | * | 0x02 | 0x55AA