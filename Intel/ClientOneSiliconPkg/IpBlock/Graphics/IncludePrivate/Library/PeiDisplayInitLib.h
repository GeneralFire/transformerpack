/** @file
  PEI Graphics Display header file

@copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation.

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
#ifndef _DISPLAY_PEIM_INIT_H_
#define _DISPLAY_PEIM_INIT_H_

#include <Library/BaseLib.h>
#include <PiPei.h>
#include <IndustryStandard/Bmp.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci22.h>
#include <Library/HobLib.h>
#include <Guid/GraphicsInfoHob.h>
#include <Ppi/SiPolicy.h>
#include <Hda.h>
#include <Library/GraphicsInfoLib.h>

/**
  PeiDisplayInit: Initialize the IGD using GraphicsInitPeim and display logo in PEI

  @param[in] GtPreMemConfig  - GRAPHICS_PEI_PREMEM_PREMEM_CONFIG to access the GtPreMemConfig related information
  @param[in] GtConfig        - GRAPHICS_PEI_CONFIG to access the GtConfig related information
    GtConfig.GraphicsConfigPtr- Address of the Graphics Configuration Table
    GtConfig.LogoPtr          - Address of Logo to be displayed
    GtConfig.LogoSize         - Logo Size

  @retval     EFI_STATUS
**/
EFI_STATUS
PeiDisplayInit (
  IN   GRAPHICS_PEI_PREMEM_CONFIG      *GtPreMemConfig,
  IN   GRAPHICS_PEI_CONFIG             *GtConfig
  );

/**
  FillFrameBufferAndShowLogo: Fill frame buffer with the image

  @param[in] GtConfig  - GRAPHICS_PEI_CONFIG to access the GtConfig related information
    GtConfig.GraphicsConfigPtr- Address of the Graphics Configuration Table

  @retval    EFI_STATUS
**/
EFI_STATUS
FillFrameBufferAndShowLogo(
IN GRAPHICS_PEI_CONFIG                    *GtConfig
);

/**
  PeiGraphicsNotifyCallback -   Run this function after PEI_GRAPHICS_PPI is installed to CallPpiAndFillFrameBuffer.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
PeiGraphicsNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**
  CallPpiAndFillFrameBuffer: Call GraphicsInitPeim PeiGraphicsPpi to initalize display and get Mode info.
  Publish GraphicsInfoHob and call FillFrameBufferAndShowLogo

  @param[in] GtConfig  - GRAPHICS_PEI_CONFIG to access the GtConfig related information

  @retval    EFI_OUT_OF_RESOURCES - no enough reosurce for completing this task.
  @retval    EFI_STATUS - status code from calling several sub functions.

**/
EFI_STATUS
EFIAPI
CallPpiAndFillFrameBuffer (
  IN   GRAPHICS_PEI_CONFIG             *GtConfig
  );


/**
  DisplayNativeGpioInit: Initialize the Display Native Gpio

  @param[in] GtPreMemConfig        - GRAPHICS_PEI_PREMEM_CONFIG to access the GtConfig related information

**/
VOID
DisplayNativeGpioInit (
  IN   GRAPHICS_PEI_PREMEM_CONFIG      *GtPreMemConfig
  );
/**
  DisplayInitPreMem: Initialize the Display in PreMem phase

  @param[in] GtPreMemConfig        - GRAPHICS_PEI_PREMEM_CONFIG to access the GtConfig related information

**/
VOID
DisplayInitPreMem (
  IN   GRAPHICS_PEI_PREMEM_CONFIG      *GtPreMemConfig
  );
/**
  Program the max Cd Clock supported by the platform

  @param[in] GtConfig            - Instance of GRAPHICS_PEI_CONFIG
  @param[in] GttMmAdr            - Base Address of IGFX MMIO BAR

  @retval EFI_SUCCESS            - CD Clock value programmed.
  @retval EFI_INVALID_PARAMETER  - The input parameter is invalid

**/
EFI_STATUS
ProgramCdClkReg (
  IN       GRAPHICS_PEI_CONFIG          *GtConfig,
  IN       UINT32                       GttMmAdr
  );

/**
  Initialize the full CD clock as per Bspec sequence

  @param[in] GtConfig            - Instance of GRAPHICS_PEI_CONFIG
  @param[in] GtPreMemConfig      - Instance of GRAPHICS_PEI_PREMEM_CONFIG

  @retval EFI_SUCCESS            - CD Clock Init successful.
  @retval EFI_INVALID_PARAMETER  - The input parameter is invalid
  @retval EFI_UNSUPPORTED        - iGfx is not present.
**/
EFI_STATUS
CdClkInit (
  IN  GRAPHICS_PEI_CONFIG             *GtConfig,
  IN  GRAPHICS_PEI_PREMEM_CONFIG      *GtPreMemConfig
  );

/**
  This function will get value of reference clock from DSSM strap register.

  @param[in] GttMmAdr            - Base Address of IGFX MMIO BAR

  @retval ReferenceFreq          - Returns value of reference frequency found in DSSM strap register.
                                   Unit of freuquency is in KHz.
**/
UINT32
GetDssmReferenceFrequency (
  IN  UINT32                GttMmAdr
  );

/**
  Program the Display Power Wells supported by platform

  @param[in] GttMmAdr            - Base Address of IGFX MMIO BAR

  @retval EFI_SUCCESS            - Power well programming finished successfully
  @retval EFI_UNSUPPORTED        - Power well programming failed
  @retval EFI_TIMEOUT            - Timed out
**/
EFI_STATUS
EnablePowerWell (
  IN  UINT32     GttMmAdr
);

/**
  ConfigureIDispAudioFrequency: Configures iDisplay Audio BCLK frequency and T-Mode

  @param[in] RequestedBclkFrequency     IDisplay Link clock frequency to be set
  @param[in] RequestedTmode             IDisplay Link T-Mode to be set

  @retval EFI_NOT_FOUND                 SA Policy PPI or GT config block not found, cannot initialize GttMmAdr
  @retval EFI_UNSUPPORTED               iDisp link unsupported frequency
  @retval EFI_SUCCESS                   The function completed successfully
**/
EFI_STATUS
ConfigureIDispAudioFrequency (
  IN       HDAUDIO_LINK_FREQUENCY  RequestedBclkFrequency,
  IN       HDAUDIO_IDISP_TMODE     RequestedTmode
  );

/**
  InitializeDisplayAudio: Initialize display engine for iDisplay Audio programming

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_ABORTED             S3 boot - display already initialized
  @retval EFI_UNSUPPORTED         iGfx disabled, iDisplay Audio not present
  @retval EFI_NOT_FOUND           SaPolicy or temporary GTT base address not found
**/
EFI_STATUS
InitializeDisplayAudio (
  VOID
  );
#endif
