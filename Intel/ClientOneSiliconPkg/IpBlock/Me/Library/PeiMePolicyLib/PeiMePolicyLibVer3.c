/** @file
  This file is PeiMePolicy library.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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
#include "PeiMePolicyLibrary.h"
#include "Library/MeTypeLib.h"
#include "PeiMePolicy.h"
#include <Library/MeUtilsLib.h>


#include <Register/MeRegs.h>
#include <MeDefines.h>
typedef enum {
  ME_AUTO_CONFIG_HEC1         = HECI_FUNCTION_NUMBER,
  ME_AUTO_CONFIG_HEC2         = HECI2_FUNCTION_NUMBER,
  ME_AUTO_CONFIG_HEC3         = HECI3_FUNCTION_NUMBER,
  ME_AUTO_CONFIG_HEC4         = HECI4_FUNCTION_NUMBER,
  ME_AUTO_CONFIG_SOL          = SOL_FUNCTION_NUMBER,
  ME_AUTO_CONFIG_IDER         = IDER_FUNCTION_NUMBER
} ME_AUTO_CONFIG;

/**
  Function resolves AutoSettings for particular configuration entry

  @param[in] Device               Device to resolve auto-configuration
  @param[in] ConfigToResolve      Auto-configuration to be resolved
  @param[in] MeIsCorporateAmt     True if it is corporate AMT running in ME

  @retval resolved auto-configuration
**/
UINT32
MeConfigDeviceAutoResolve (
  IN     ME_AUTO_CONFIG Device,
  IN     UINT32         ConfigToResolve,
  IN     BOOLEAN        MeIsCorporateAmt
  );

/**
  Load default settings for ME config block in pre-mem phase.

  @param[in] ConfigBlockPointer                 The pointer to the config block
**/
VOID
LoadMePeiPreMemDefault (
  IN VOID                           *ConfigBlockPointer
  )
{
  ME_PEI_PREMEM_CONFIG *MePeiPreMemConfig;
  BOOLEAN               MeIsCorporateWs;

  MeIsCorporateWs = MeClientIsCorporate ();

  MePeiPreMemConfig = ConfigBlockPointer;

  MePeiPreMemConfig->HeciTimeouts                  = 1;
  MePeiPreMemConfig->HeciFullTrace                 = 1;

  MePeiPreMemConfig->Heci1BarAddress               = 0xFEDA2000;
  MePeiPreMemConfig->Heci2BarAddress               = 0xFEDA3000;
  MePeiPreMemConfig->Heci3BarAddress               = 0xFEDA4000;

  //
  // Test policies
  //
  MePeiPreMemConfig->KtDeviceEnable                = 1;
  MePeiPreMemConfig->HeciCommunication2            = MeConfigDeviceAutoResolve (
                                                       ME_AUTO_CONFIG_HEC2,
                                                       ME_DEVICE_AUTO,
                                                       MeIsCorporateWs
                                                       );
}

/**
  Print values of ME config block in pre-mem phase.

  @param[in] MePeiPreMemConfig                     The pointer to the config block
**/
VOID
EFIAPI
PrintMePeiPreMemConfig (
  IN ME_PEI_PREMEM_CONFIG               *MePeiPreMemConfig
  )
{
  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INFO, "------------------------ ME_PEI_PREMEM_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision                  : 0x%x\n", MePeiPreMemConfig->Header.Revision));
  ASSERT (MePeiPreMemConfig->Header.Revision == ME_PEI_PREMEM_CONFIG_REVISION);

  DEBUG ((DEBUG_INFO, " HeciTimeouts              : 0x%x\n", MePeiPreMemConfig->HeciTimeouts));
  DEBUG ((DEBUG_INFO, " HeciFullTrace             : 0x%x\n", MePeiPreMemConfig->HeciFullTrace));
  DEBUG ((DEBUG_INFO, " DidInitStat               : 0x%x\n", MePeiPreMemConfig->DidInitStat));
  DEBUG ((DEBUG_INFO, " DisableCpuReplacedPolling : 0x%x\n", MePeiPreMemConfig->DisableCpuReplacedPolling));
  DEBUG ((DEBUG_INFO, " DisableMessageCheck       : 0x%x\n", MePeiPreMemConfig->DisableMessageCheck));
  DEBUG ((DEBUG_INFO, " SkipMbpHob                : 0x%x\n", MePeiPreMemConfig->SkipMbpHob));
  DEBUG ((DEBUG_INFO, " HeciCommunication2        : 0x%x\n", MePeiPreMemConfig->HeciCommunication2));
  DEBUG ((DEBUG_INFO, " KtDeviceEnable            : 0x%x\n", MePeiPreMemConfig->KtDeviceEnable));
  DEBUG ((DEBUG_INFO, " Heci1BarAddress           : 0x%x\n", MePeiPreMemConfig->Heci1BarAddress));
  DEBUG ((DEBUG_INFO, " Heci2BarAddress           : 0x%x\n", MePeiPreMemConfig->Heci2BarAddress));
  DEBUG ((DEBUG_INFO, " Heci3BarAddress           : 0x%x\n", MePeiPreMemConfig->Heci3BarAddress));
  DEBUG ((DEBUG_INFO, " HeciFullTrace             : %d\n", MePeiPreMemConfig->HeciFullTrace));
  DEBUG_CODE_END ();
}
