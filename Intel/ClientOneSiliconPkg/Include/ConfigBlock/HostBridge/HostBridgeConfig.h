/** @file
  Configurations for HostBridge

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

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
#ifndef _HOST_BRIDGE_CONFIG_H_
#define _HOST_BRIDGE_CONFIG_H_

#include <ConfigBlock.h>

#define HOST_BRIDGE_PREMEM_CONFIG_REVISION 1
#define HOST_BRIDGE_PEI_CONFIG_REVISION    1

extern EFI_GUID gHostBridgePeiPreMemConfigGuid;
extern EFI_GUID gHostBridgePeiConfigGuid;

#pragma pack (push,1)

/**
  This configuration block describes HostBridge settings in PreMem.\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;               ///< Offset 0-27 Config Block Header
  UINT32  MchBar;                            ///< Offset 28 Address of System Agent MCHBAR: <b>0xFEDC0000(TGL)/0xFED10000(RKL)/0xFEA80000(JSL)<b>
  UINT32  DmiBar;                            ///< Offset 32 Address of System Agent DMIBAR: <b>0xFEDA0000</b>
  UINT32  EpBar;                             ///< Offset 36 Address of System Agent EPBAR: <b>0xFEDA1000</b>
  UINT32  GdxcBar;                           ///< Offset 40 Address of System Agent GDXCBAR: <b>0xFED84000</b>
  UINT32  RegBar;                            ///< Offset 44 Address of System Agent REGBAR: <b>0xFB000000</b>
  UINT32  EdramBar;                          ///< Offset 48 Address of System Agent EDRAMBAR: <b>0xFED80000</b>
  /**
    Offset 52 :
    Size of reserved MMIO space for PCI devices\n
    <b>0=AUTO</b>, 512=512MB, 768=768MB, 1024=1024MB, 1280=1280MB, 1536=1536MB, 1792=1792MB,
    2048=2048MB, 2304=2304MB, 2560=2560MB, 2816=2816MB, 3072=3072MB\n
    When AUTO mode selected, the MMIO size will be calculated by required MMIO size from PCIe devices detected.
  **/
  UINT32  MmioSize;
  UINT32  MmioSizeAdjustment;                ///< Offset 56 Increase (given positive value) or Decrease (given negative value) the Reserved MMIO size when Dynamic Tolud/AUTO mode enabled (in MBs): <b>0=no adjustment</b>
  UINT8   EnableAbove4GBMmio;                ///< Offset 60 Enable/disable above 4GB MMIO resource support: 0=Disable, <b>1=Enable</b>
  UINT8   Reserved[3];                       ///< Offset 61 Reserved for future use.
} HOST_BRIDGE_PREMEM_CONFIG;


/**
  This configuration block describes HostBridge settings in Post-Mem.\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;               ///< Offset 0-27 Config Block Header
  UINT8  Device4Enable;                      ///< Offser 28 :This policy is used to control enable or disable System Agent Thermal device (0,4,0). <b>0=FALSE</b>, 1=TRUE.
  UINT8  ChapDeviceEnable;                   ///< Offset 29 :<b>(Test)</b>This policy is used to control enable or disable System Agent Chap device (0,7,0). <b>0=FALSE</b>, 1=TRUE.
  UINT8  SkipPamLock;                        ///< Offset 30 :To skip PAM register locking. @note It is still recommended to set PCI Config space B0: D0: F0: Offset 80h[0]=1 in platform code even Silicon code skipped this.\n <b>0=All PAM registers will be locked in Silicon code</b>, 1=Skip lock PAM registers in Silicon code.
  UINT8  EdramTestMode;                      ///< Offset 28 :EDRAM Test Mode. For EDRAM stepping - 0- EDRAM SW Disable, 1- EDRAM SW Enable, <b> 2- EDRAM HW Mode</b>
} HOST_BRIDGE_PEI_CONFIG;

#pragma pack (pop)

#endif
