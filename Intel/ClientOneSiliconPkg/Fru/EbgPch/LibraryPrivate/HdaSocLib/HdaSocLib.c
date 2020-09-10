/** @file
  HDA SoC library. Library allows to configure HDA controller to work on Ebg platforms.
  All functions in this library are available for PEI.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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

#include <IndustryStandard/Pci30.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PostCodeLib.h>
#include <Library/PsfLib.h>
#include <Library/PsfSocLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/GpioPrivateLib.h>
#include <Library/GpioNativePads.h>
#include <Library/PeiHdaLib.h>
#include <Library/PeiItssLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PeiCavsInitLib.h>
#include <Library/PmcSocLib.h>
#include <Library/HdaLib.h>
#include <Register/IgdRegs.h>
#include <Register/PchRegs.h>
#include <Ppi/SiPolicy.h>
#include <PchConfigHob.h>
#include <Library/HobLib.h>
/*
  Configure HDA Audio Link GPIOs

  @param[in] HdaPreMemConfig      Hda PreMem Configuration.
*/
VOID
HdaConfigureHdaLink (
  IN HDAUDIO_PREMEM_CONFIG   *HdaPreMemConfig
  )
{
  GpioSetNativePadByFunction (GPIO_FUNCTION_HDA_BCLK, 0);
  GpioSetNativePadByFunction (GPIO_FUNCTION_HDA_RSTB, 0);
  GpioSetNativePadByFunction (GPIO_FUNCTION_HDA_SYNC, 0);
  GpioSetNativePadByFunction (GPIO_FUNCTION_HDA_SDO, 0);
  if (HdaPreMemConfig->AudioLinkHda.SdiEnable[0]) {
    GpioSetNativePadByFunction (GPIO_FUNCTION_HDA_SDI_0, 0);
  }
  if (HdaPreMemConfig->AudioLinkHda.SdiEnable[1]) {
    GpioSetNativePadByFunction (GPIO_FUNCTION_HDA_SDI_1, 0);
  }

  DEBUG ((DEBUG_INFO, "HDA: HD Audio Link pins configuration\n"));
}

/*
  Function disables HDA Device on PMC level.
*/
VOID
HdaPmcDisableDevice (
  VOID
  )
{
  PmcDisableHda ();
}

/*
  Function disables HDA Device on PSF level.
*/
VOID
HdaPsfDisableDevice (
  VOID
  )
{
  PsfDisableHdaDevice ();
}

/*
  Function disables DSP Bar on PSF level.
*/
VOID
HdaPsfDisableDspBar (
  VOID
  )
{
  PsfDisableDspBar ();
}

/*
  Function allow to access to SB register.

  @param[in] Offset            Offset of SB register.
  @param[in] AndData           Mask to AND with the SB register.
  @param[in] OrData            Mask to OR with the SB register.
*/
VOID
HdaPcrPidDspAndThenOr32 (
  IN  UINT32                            Offset,
  IN  UINT32                            AndData,
  IN  UINT32                            OrData
  )
{
  PchPcrAndThenOr32 (PID_DSP, Offset, AndData, OrData);
}

/*
  Function allow to access to SB register.

  @param[in] Offset            Offset of SB register.

  @retval Value of SB register
*/
UINT32
HdaPcrPidDspRead32 (
  IN  UINT32                            Offset
  )
{
  return PchPcrRead32 (PID_DSP, Offset);
}

/*
  Function base on type of link and number of link return if link is supported.

  @param[in] HdaHandle            Pointer to Hda controller configuration structure
  @param[in] HdaController        Pointer to Hda device structure

  @retval Function return TRUE if Audio link is supported and FALSE if not.
*/
BOOLEAN
HdaIsAudioInterfaceSupported (
  IN HDAUDIO_LINK_TYPE     AudioLinkType,
  IN UINT32                AudioLinkIndex
  )
{
  ///
  /// Interfaces supported:
  /// 1. HDA Link (SDI0/SDI1)
  ///
  switch (AudioLinkType) {
    case HdaLink:
      return TRUE;
    default:
      return FALSE;
  }
}

/*
  Function check if Tmode is supported for IDisp codec.

  @retval Function return TRUE if Tmode is supported and FALSE if not.
*/
BOOLEAN
HdaIsAudioIDispTmodeSupported (
  IN HDAUDIO_IDISP_TMODE Tmode
  )
{
  return FALSE;
}

/*
  Create HDA controller instance.

  @param[in] HdaHandle            Pointer to Hda controller configuration structure
  @param[in] HdaController        Pointer to Hda device structure
  @param[in] HdaCallbacks         Pointer to Hda callbacks structure
  @param[in] SiPolicy             Pointer to SiPolicy
  @param[in] SiPreMemPolicy       Pointer to SiPreMemPolicy
*/
EFI_STATUS
CreateHdaHandle (
  OUT  HDA_HANDLE                 *HdaHandle,
  IN   HDA_PRIVATE_CONFIG         *HdaPrivateConfig, OPTIONAL
  IN   HDA_CONTROLLER             *HdaController,    OPTIONAL
  IN   HDA_CALLBACK               *HdaCallback,      OPTIONAL
  IN   SI_POLICY_PPI              *SiPolicy,         OPTIONAL
  IN   SI_PREMEM_POLICY_PPI       *SiPreMemPolicy    OPTIONAL
  )
{
  EFI_STATUS                    Status;

  HDAUDIO_CONFIG                *HdaConfig;
  HDAUDIO_PREMEM_CONFIG         *HdaPreMemConfig;

  if (SiPolicy != NULL) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gHdAudioConfigGuid, (VOID *) &HdaConfig);
    ASSERT_EFI_ERROR (Status);
    HdaHandle->HdaConfig = HdaConfig;
  } else {
    HdaHandle->HdaConfig = NULL;
  }

  if (SiPreMemPolicy != NULL) {
    Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHdAudioPreMemConfigGuid, (VOID *) &HdaPreMemConfig);
    ASSERT_EFI_ERROR (Status);
    HdaHandle->HdaPreMemConfig = HdaPreMemConfig;
  } else {
    HdaHandle->HdaPreMemConfig = NULL;
  }

  if (HdaController != NULL) {
    HdaController->Segment        = DEFAULT_PCI_SEGMENT_NUMBER_PCH;
    HdaController->Bus            = DEFAULT_PCI_BUS_NUMBER_PCH;
    HdaController->Device         = HdaDevNumber ();
    HdaController->Function       = HdaFuncNumber ();
    HdaController->PciCfgBaseAddr = HdaPciCfgBase ();
    HdaController->HdaMmioAddress = PcdGet32 (PcdSiliconInitTempMemBaseAddr);
    HdaController->DspMmioAddress = PcdGet32 (PcdSiliconInitTempMemBaseAddr) + HdaBarSize ();
  }

  if (HdaCallback != NULL) {
    HdaCallback->HdaConfigureHdaLink                  = &HdaConfigureHdaLink;
    HdaCallback->HdaPmcDisableDevice                  = &HdaPmcDisableDevice;
    HdaCallback->HdaPsfDisableDevice                  = &HdaPsfDisableDevice;
    HdaCallback->HdaPsfDisableDspBar                  = &HdaPsfDisableDspBar;
    HdaCallback->HdaPcrPidDspAndThenOr32              = &HdaPcrPidDspAndThenOr32;
    HdaCallback->HdaPcrPidDspRead32                   = &HdaPcrPidDspRead32;
    HdaCallback->HdaIsAudioInterfaceSupported         = &HdaIsAudioInterfaceSupported;
    HdaCallback->HdaIsAudioIDispTmodeSupported        = &HdaIsAudioIDispTmodeSupported;
  }

  if (HdaPrivateConfig != NULL) {
    HdaPrivateConfig->NodeId             = 0x8;
    HdaPrivateConfig->HdaMaxSndwLinkNum  = GetPchHdaMaxSndwLinkNum ();
    HdaPrivateConfig->HdaMaxDmicLinkNum  = GetPchHdaMaxDmicLinkNum ();
    HdaPrivateConfig->HdaMaxSspLinkNum   = GetPchHdaMaxSspLinkNum ();
    HdaPrivateConfig->IGfxEnabled        = FALSE;
  }

  HdaHandle->HdaController     = HdaController;
  HdaHandle->HdaCallback       = HdaCallback;
  HdaHandle->HdaPrivateConfig  = HdaPrivateConfig;

  return EFI_SUCCESS;
}

/**
  Initialize the Intel High Definition Audio Controller on Pre-Mem

  @param[in] SiPreMemPolicyPpi       Pre-mem Policy

**/
VOID
HdaPreMemInit (
  IN SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi
  )
{
  HDA_HANDLE                    HdaHandle;
  HDA_CONTROLLER                HdaController;
  HDA_CALLBACK                  HdaCallback;
  HDA_PRIVATE_CONFIG            HdaPrivateConfig;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  CreateHdaHandle (&HdaHandle, &HdaPrivateConfig, &HdaController, &HdaCallback, NULL, SiPreMemPolicyPpi);
  PrintHdaHandle (&HdaHandle);
  cAvsConfigureHdAudioOnPreMem (&HdaHandle);

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
}

/**
  Initialize the Intel High Definition Audio Controller on Post-Mem. Program of FNCFG register before P2SB Lock.
  Initialize HDA Link Codecs.

  @param[in] SiPreMemPolicyPpi       Pre-mem Policy
  @param[in] SiPolicy                Post-mem Policy

**/
VOID
HdaInit (
  IN SI_PREMEM_POLICY_PPI       *SiPreMemPolicyPpi,
  IN SI_POLICY_PPI              *SiPolicyPpi
  )
{
  HDA_HANDLE                    HdaHandle;
  HDA_CONTROLLER                HdaController;
  HDA_CALLBACK                  HdaCallback;
  HDA_PRIVATE_CONFIG            HdaPrivateConfig;
  UINT8                         InterruptPin;
  PCH_CONFIG_HOB                *PchConfigHob;
  EFI_PEI_HOB_POINTERS          HobList;
  UINT32                        XtalValue;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  CreateHdaHandle (&HdaHandle, &HdaPrivateConfig, &HdaController, &HdaCallback, SiPolicyPpi, SiPreMemPolicyPpi);
  PrintHdaHandle (&HdaHandle);
  cAvsConfigureHdAudioOnPostMem (&HdaHandle, &XtalValue);

  //
  // Get PCH Config HOB.
  //
  HobList.Guid = GetFirstGuidHob (&gPchConfigHobGuid);
  ASSERT (HobList.Guid != NULL);
  PchConfigHob = (PCH_CONFIG_HOB *) GET_GUID_HOB_DATA (HobList.Guid);
  PchConfigHob->HdAudio.AudioXtal = XtalValue;

  InterruptPin = ItssGetDevIntPin (SiPolicyPpi, HdaController.Device, HdaController.Function);
  cAvsConfigureHdAudioCodec (&HdaHandle, InterruptPin);

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
}
