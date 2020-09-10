/** @file
  BiosGuardInit.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2019 Intel Corporation. <BR>

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

#ifndef _BIOSGUARD_INIT_H_
#define _BIOSGUARD_INIT_H_

#include "BiosGuardDefinitions.h"
#include <Library/MemoryCoreLib.h>
#include <Guid/PlatformInfo.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/ProcMemInitLib.h>
#include <Library/DebugLib.h>
#include <Ppi/Reset.h>
#include <Guid/SetupVariable.h>
#include <PchAccess.h>
#include <Library/MmPciBaseLib.h>

#define BIOSGUARD_SUPPORT_ENABLED BIT0

/**
  Platform policies for BIOS Guard Configuration for all processor security features configuration.
  Platform code can pass relevant configuration data through this structure.
  @note The policies are marked are either <b>(Required)</b> or <b>(Optional)</b>
  - <b>(Required)</b> : This policy is recommended to be properly configured for proper functioning of reference code and silicon initialization
  - <b>(Optional)</b> : This policy is recommended for validation purpose only.
**/
typedef struct {
  BGUP_HEADER          BgupHeader;          ///< BIOS Guard update package header that will be packaged along with BIOS Guard script and update data.
  BGPDT                Bgpdt;               ///< BIOS Guard Platform Data Table contains all the platform data that will be parsed by BIOS Guard module.
  UINT64               BgpdtHash[4];        ///< Hash of the BGPDT that will be programmed to PLAT_FRMW_PROT_HASH_0/1/2/3 MSR.
  UINT8                BiosGuardMemSize;    ///< BIOS Guard memory size.
  UINT8                EcCmdDiscovery;      ///< EC Command discovery.
  UINT8                EcCmdProvisionEav;   ///< EC Command Provision Eav.
  UINT8                EcCmdLock;           ///< EC Command Lock.
  BIOSGUARD_LOG        BiosGuardLog;        ///< BIOS Guard log.
  EFI_PHYSICAL_ADDRESS BiosGuardModulePtr;  ///< Pointer to the BIOS Guard Module.
  UINT32               TotalFlashSize;      ///< Size of entire SPI flash region
} BIOSGUARD_CONFIG;

///
/// Function Prototypes
///
/**
  Execute Early-Post initialization of BIOS Guard specific MSRs

  @param[in] PeiServices                  - Indirect reference to the PEI Services Table.
  @param[in] TotalSystemConfigurationPtr  - Pointer to Setup Configuration structure
**/
VOID
BiosGuardInit (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN SYSTEM_CONFIGURATION       *TotalSystemConfigurationPtr
  );

#endif
