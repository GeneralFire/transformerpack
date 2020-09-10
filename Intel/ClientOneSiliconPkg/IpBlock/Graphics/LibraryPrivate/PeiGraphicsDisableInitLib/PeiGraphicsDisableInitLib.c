/** @file
  PEIM to Disable IGFX.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2019 Intel Corporation.

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

@par Specification
**/
#include <Library/PeiGraphicsInitLib.h>

///
/// Driver Consumed PPI Prototypes
///
#include <Ppi/SiPolicy.h>

/**
  Check and Force Vdd bit On

  @param[in] GRAPHICS_PEI_PREMEM_CONFIG      GtPreMemConfig

  @retval EFI_NOT_READY                   H/W Power Cycle sequence in progress, Vdd on not enabled or Policy not found
  @retval EFI_SUCCESS                     Vdd On already enabled or enabled successfully

**/
EFI_STATUS
CheckAndForceVddOn (
  IN   GRAPHICS_PEI_PREMEM_CONFIG      *GtPreMemConfig
  )
{
  return EFI_SUCCESS;
}

/**
  Check and Force Vdd On in End Of PEI

  @retval EFI_NOT_READY                   H/W Power Cycle sequence in progress, Vdd on not enabled or Policy not found
  @retval EFI_SUCCESS                     Vdd On already enabled or enabled successfully

**/
EFI_STATUS
EndOfPeiCheckAndForceVddOn (
  VOID
  )
{
  return EFI_SUCCESS;
}


/**
  Check and Force Vdd On in PostMem Phase

  @param[in] GRAPHICS_PEI_PREMEM_CONFIG      GtPreMemConfig

**/
VOID
PostMemCheckAndForceVddOn (
  IN   GRAPHICS_PEI_PREMEM_CONFIG      *GtPreMemConfig
  )
{
  return;
}


/**
  GraphicsInit: Initialize the IGD if no other external graphics is present

  @param[in] GtPreMemConfig  - GtPreMemConfig to access the GtPreMemConfig related information
  @param[in] PrimaryDisplay       - Primary Display - default is IGD
  @param[in, out] PegMmioLength   - Total IGFX MMIO length

**/
VOID
GraphicsInit (
  IN       GRAPHICS_PEI_PREMEM_CONFIG   *GtPreMemConfig,
  IN       DISPLAY_DEVICE               *PrimaryDisplay,
  IN       UINT32                       *IGfxMmioLength
  )
{
  AdditionalStepsForDisablingIgfx (GtPreMemConfig);
  return;
}

/**
Initialize GT Power management

  @param[in] GRAPHICS_PEI_PREMEM_CONFIG      GtPreMemConfig
  @param[in] GRAPHICS_PEI_CONFIG             GtConfig

  @retval EFI_SUCCESS          - GT Power management initialization complete
**/
EFI_STATUS
GraphicsPmInit (
  IN       GRAPHICS_PEI_PREMEM_CONFIG      *GtPreMemConfig,
  IN       GRAPHICS_PEI_CONFIG             *GtConfig
  )
{
  return EFI_SUCCESS;
}

/**
  Initialize PAVP feature of SystemAgent.

  @param[in] GRAPHICS_PEI_CONFIG             GT config block from SA Policy PPI
  @param[in] SA_MISC_PEI_CONFIG              MISC config block from SA Policy PPI

  @retval EFI_SUCCESS     - PAVP initialization complete
  @retval EFI_UNSUPPORTED - iGFX is not present so PAVP not supported
**/
EFI_STATUS
PavpInit (
  IN       GRAPHICS_PEI_CONFIG             *GtConfig,
  IN       SA_MISC_PEI_CONFIG              *MiscPeiConfig,
  IN       GRAPHICS_PEI_PREMEM_CONFIG      *GtPreMemConfig
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Program PSMI registers.

  @param[in] GRAPHICS_PEI_CONFIG             *GtConfig
  @param[in] GRAPHICS_PEI_PREMEM_CONFIG      *GtPreMemConfig

  @retval EFI_SUCCESS     - PSMI registers programmed.
**/
EFI_STATUS
ProgramPsmiRegs (
  IN       GRAPHICS_PEI_CONFIG             *GtConfig,
  IN       GRAPHICS_PEI_PREMEM_CONFIG      *GtPreMemConfig
  )
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
SaRegisterMirror (
  IN       GRAPHICS_PEI_PREMEM_CONFIG             *GtPreMemConfig
  )
{
  return EFI_UNSUPPORTED;
}

/**
  This function is to set Gfx Memory map (RC6 base, Doorbell base, Dfdbase etc)

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
SetGfxMemMap (
  VOID
  )
{
  return EFI_SUCCESS;
}
