/** @file
  Header file for the PCH Init PEIM

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019-2020 Intel Corporation.

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
#ifndef _PCH_INIT_PEI_H_
#define _PCH_INIT_PEI_H_

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/TimerLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/EspiLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PcieRpLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchSbiAccessLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SataSocLib.h>
#include <Library/PchTraceHubLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchPolicyLib.h>
#include <Library/PchFiaLib.h>
#include <Library/PciExpressHelpersLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/PmcLib.h>
#include <Library/GpioPrivateLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/MeRegs.h>
#include "PchUsb.h"
#include <Library/DciPrivateLib.h>
#include <Library/PsfLib.h>
#include <Library/PsfSocLib.h>
#include <PchResetPlatformSpecific.h>
#include <PchInfoHob.h>
#include <Library/PmcPrivateLib.h>
#include <PchConfigHob.h>
#include <PchLimits.h>
#include <Library/PeiThermalLib.h>
#include <Library/PeiPchPcieClocksLib.h>
#include <Library/PeiCnviLib.h>
#include <Library/PeiP2sbPrivateLib.h>
#include <Library/PeiPchDmiLib.h>
#include <Library/PeiItssLib.h>
#include <Library/SiScheduleResetLib.h>
#include <Library/PeiScsLib.h>
#include <Library/PeiLpcLib.h>
#include <Library/PeiIehInitLib.h>
#include <Library/HdaSocLib.h>
#include <Library/PcieRpHandleLib.h>

extern EFI_GUID gPchDeviceTableHobGuid;

/**
  Disable unused PCIe Ports

**/
VOID
PchPcieRpDisableUnusedPorts (
  VOID
  );

/**
  Initialize EBG PCH PMC controller.

  @param[in] SiPolicy  The Silicon Policy PPI instance
**/
VOID
EbgPchPmcInit (
  IN SI_POLICY_PPI  *SiPolicy
  );

/**
  Initialize EBG PCH PMC controller.

  @param[in] SiPolicy  The Silicon Policy PPI instance
  @param[in] IoeId     I/O Expander Id (0 ... n - I/O Expander)
**/
VOID
EbgPchIoExpanderPmcInit (
  IN  SI_POLICY_PPI  *SiPolicy,
  IN  UINT32         IoeId
  );

/**
  Program MMIO for EBG PCH PMC controller.

  @param[in] SiPolicy  The Silicon Policy PPI instance
  @param[in] IoeId     I/O Expander Id (0 ... n - I/O Expander)
**/
VOID
EbgPchPmcProgramMmio (
  IN SI_POLICY_PPI  *SiPolicy,
  IN UINT32         IoeId
  );

/**
  Init PMC at end of PEI.

  @param[in] SiPolicy  Pointer to SiPolicy
**/
VOID
EbgPchPmcInitEndOfPei (
  IN SI_POLICY_PPI  *SiPolicy
  );

/**
  Init PMC at end of PEI.

  @param[in] SiPolicy  Pointer to SiPolicy
**/
VOID
EbgPchIoExpanderPmcInitEndOfPei (
  IN  SI_POLICY_PPI  *SiPolicy
  );

/**
  Initialize LAN device on PCH

  @param[in]      SiPolicy       Pointer to Silicon Policy
**/
VOID
EbgPchGbeInit (
  IN  SI_POLICY_PPI  *SiPolicy
  );

/**
  Initialize LAN device on I/O Expander

  @param[in]      SiPolicy       Pointer to Silicon Policy
  @param[in]      IoeId          I/O Expander Id (0 ... n - I/O Expander)
**/
VOID
EbgIoExpanderGbeInit (
  IN  SI_POLICY_PPI  *SiPolicy,
  IN  UINT32         IoeId
  );

/**
  Disable unused SATA Controllers

**/
VOID
PchSataDisableUnusedCtrls (
  VOID
  );

/**
  Initializes SATA Controller

**/
VOID
PchSataInit (
  IN  SI_POLICY_PPI  *SiPolicy
  );

/**
  Initializes SATA Controller on I/O Expander

**/
VOID
IoExpanderSataInit (
  IN  SI_POLICY_PPI  *SiPolicy,
  IN  UINT32         IoeId
  );

/**
  Initializes SATA Controller after RST

**/
VOID
PchSataAfterRstInit (
  IN  SI_POLICY_PPI  *SiPolicy
  );

//TODO move to seprate lib
/**
  Initialize the Intel Touch Host Controller

  @param[in] SiPolicy             Policy

**/
VOID
ThcInit (
  IN SI_POLICY_PPI           *SiPolicy
  );

/**
  Initializes McSmb device

  @param[in] SiPolicy             Pointer to silicon policy structure
**/
VOID
PchMcSmbInit (
  IN  SI_POLICY_PPI       *SiPolicy
);

/**
  Configure PCIe Grant Counts

  @param[in] SiPolicy            The SI Policy PPI

**/
VOID
PchConfigurePcieGrantCounts (
  IN  SI_POLICY_PPI         *SiPolicy
  );

/**
  Configure BiosLock according to policy setting.

  @param[in] LockDownConfig       LockDownConfig policy
**/
VOID
ConfigureBiosLock (
  IN  SI_POLICY_PPI  *SiPolicy
  );

/**
  The function performs PSTH specific programming.

  @param[in] SiPolicy          The SI Policy instance
**/
VOID
PchPsthConfigure (
  IN  SI_POLICY_PPI           *SiPolicy
  );

/**
  The function performs iClk specific programming.
**/
VOID
PchIClkConfigure (
  VOID
  );

/**
  The function update pch config hob in the end of PchInit.

  @param[in]      SiPolicy               The SI Policy PPI instance
**/
VOID
BuildPchConfigHob (
  IN     SI_POLICY_PPI          *SiPolicy
  );

/**
  The function update pch info hob in the end of PchInit.
**/
VOID
BuildPchInfoHob (
  VOID
  );

/**
  Get PCIE RP handle

  @param[in     ] SiPreMemPolicyPpi   The SI PreMem Policy PPI
  @param[in     ] SiPolicy            The SI Policy PPI
  @param[in     ] RpIndex             PCIE RP index
  @param[in, out] RpHandle            Root Port device handle
  @param[in, out] RpPrivate           Pointer to PCIE RP private structure
  @param[in, out] RpDevice            Pointer to PCIE RP device structure
  @param[in, out] RpCallback          Pointer to PCIE RP callback structure
 **/
VOID
PchRpHandleInit (
  IN       SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi,
  IN CONST SI_POLICY_PPI         *SiPolicy,
  IN       UINT32                RpIndex,
  IN OUT   PCH_RP_HANDLE         *RpHandle,
  IN OUT   PCH_RP_PRIVATE_CONFIG *RpPrivate,
  IN OUT   PCH_RP_DEVICE         *RpDevice,
  IN OUT   PCH_RP_CALLBACK       *RpCallback
  );

#endif
