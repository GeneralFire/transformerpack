/** @file
  Do Platform Stage System Agent initialization.

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2018 Intel Corporation.

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

#include <Ppi/SiPolicy.h>
#include <Library/DebugLib.h>
#include <FspEas.h>
#include <Library/PeiSaPolicyLib.h>
#include <Library/FspCommonLib.h>
#include <Library/DebugLib.h>
#include <FspsUpd.h>
#include <ConfigBlock/TcssPeiConfig.h>
#include <ConfigBlock/TcssPeiConfig.h>


/**
  UpdatePeiSaPolicy performs SA PEI Policy initialization

  @param[in out] SiPolicyPpi       SI_POLICY PPI
  @param[in]     FspsUpd           The pointer of FspsUpd

  @retval EFI_SUCCESS              The policy is installed and initialized.
**/
EFI_STATUS
FspUpdatePeiSaPolicy (
  IN OUT SI_POLICY_PPI       *SiPolicyPpi,
  IN     FSPS_UPD            *FspsUpd
  )
{
  EFI_STATUS                    Status;
  SA_MISC_PEI_CONFIG            *MiscPeiConfig;
  GRAPHICS_PEI_CONFIG           *GtConfig;
  PCIE_PEI_CONFIG               *PciePeiConfig;
  GNA_CONFIG                    *GnaConfig;
  UINT8                         Index;
  TCSS_PEI_CONFIG               *TcssPeiConfig;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP UpdatePeiSaPolicy\n"));

  MiscPeiConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSaMiscPeiConfigGuid, (VOID *)&MiscPeiConfig);
  ASSERT_EFI_ERROR (Status);

  GtConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gGraphicsPeiConfigGuid, (VOID *)&GtConfig);
  ASSERT_EFI_ERROR (Status);

  GnaConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gGnaConfigGuid, (VOID *)&GnaConfig);
  ASSERT_EFI_ERROR (Status);

  PciePeiConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSaPciePeiConfigGuid, (VOID *)&PciePeiConfig);
  ASSERT_EFI_ERROR (Status);

  TcssPeiConfig = NULL;
  Status = GetConfigBlock((VOID *) SiPolicyPpi, &gTcssPeiConfigGuid, (VOID *) &TcssPeiConfig);
  ASSERT_EFI_ERROR(Status);

  TcssPeiConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gTcssPeiConfigGuid, (VOID *) &TcssPeiConfig);
  ASSERT_EFI_ERROR (Status);

  if (MiscPeiConfig != NULL) {
    MiscPeiConfig->Device4Enable = FspsUpd->FspsConfig.Device4Enable;
    MiscPeiConfig->SkipPamLock = FspsUpd->FspsConfig.SkipPamLock;
    MiscPeiConfig->EdramTestMode = FspsUpd->FspsConfig.EdramTestMode;
  }

  if (PciePeiConfig != NULL) {
    PciePeiConfig->DmiAspm = FspsUpd->FspsConfig.DmiAspm;
    PciePeiConfig->DmiExtSync = FspsUpd->FspsConfig.DmiExtSync;
    PciePeiConfig->DmiIot = FspsUpd->FspsConfig.DmiIot;
    for (Index = 0; Index < SA_PEG_MAX_FUN; Index++) {
      PciePeiConfig->PegDeEmphasis[Index] = FspsUpd->FspsConfig.PegDeEmphasis[Index];
      PciePeiConfig->PegSlotPowerLimitValue[Index] = FspsUpd->FspsConfig.PegSlotPowerLimitValue[Index];
      PciePeiConfig->PegSlotPowerLimitScale[Index] = FspsUpd->FspsConfig.PegSlotPowerLimitScale[Index];
      PciePeiConfig->PegPhysicalSlotNumber[Index] = FspsUpd->FspsConfig.PegPhysicalSlotNumber[Index];
      PciePeiConfig->PegMaxPayload[Index] = FspsUpd->FspsConfig.PegMaxPayload[Index];
    }

  }

  if (GtConfig != NULL) {
    GtConfig->GraphicsConfigPtr = (VOID *) FspsUpd->FspsConfig.GraphicsConfigPtr;
    GtConfig->LogoPtr = (VOID *) FspsUpd->FspsConfig.LogoPtr;
    GtConfig->LogoSize = FspsUpd->FspsConfig.LogoSize;
    GtConfig->PavpEnable = FspsUpd->FspsConfig.PavpEnable;
    GtConfig->CdClock = FspsUpd->FspsConfig.CdClock;
    GtConfig->PeiGraphicsPeimInit = FspsUpd->FspsConfig.PeiGraphicsPeimInit;
    GtConfig->RenderStandby = FspsUpd->FspsConfig.RenderStandby;
    GtConfig->PmSupport = FspsUpd->FspsConfig.PmSupport;
    GtConfig->CdynmaxClampEnable = FspsUpd->FspsConfig.CdynmaxClampEnable;
    GtConfig->GtFreqMax = FspsUpd->FspsConfig.GtFreqMax;
    GtConfig->DisableTurboGt= FspsUpd->FspsConfig.DisableTurboGt;
    GtConfig->SkipCdClockInit = FspsUpd->FspsConfig.SkipCdClockInit;
    DEBUG ((DEBUG_INFO | DEBUG_INIT, "GtConfig->GraphicsConfigPtr from FSP UpdatePeiSaPolicy: 0x%x\n", GtConfig->GraphicsConfigPtr));
    DEBUG ((DEBUG_INFO | DEBUG_INIT, "GtConfig->LogoPtr: 0x%x \n", GtConfig->LogoPtr ));
    DEBUG ((DEBUG_INFO | DEBUG_INIT, "GtConfig->LogoSize: 0x%x \n", GtConfig->LogoSize));


  }

  for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
    if (TcssPeiConfig != NULL) {
      TcssPeiConfig->PcieConfig.PciePortConfig[Index].PtmEnabled = FspsUpd->FspsConfig.PtmEnabled[Index];
      ///
      /// LTR Settings
      ///
      TcssPeiConfig->PcieConfig.PciePortConfig[Index].LtrEnable = FspsUpd->FspsConfig.SaPcieItbtRpLtrEnable[Index];
      TcssPeiConfig->PcieConfig.PciePortConfig[Index].SnoopLatencyOverrideMode = FspsUpd->FspsConfig.SaPcieItbtRpSnoopLatencyOverrideMode[Index];
      TcssPeiConfig->PcieConfig.PciePortConfig[Index].SnoopLatencyOverrideMultiplier = FspsUpd->FspsConfig.SaPcieItbtRpSnoopLatencyOverrideMultiplier[Index];
      TcssPeiConfig->PcieConfig.PciePortConfig[Index].SnoopLatencyOverrideValue = FspsUpd->FspsConfig.SaPcieItbtRpSnoopLatencyOverrideValue[Index];
      TcssPeiConfig->PcieConfig.PciePortConfig[Index].NonSnoopLatencyOverrideMode = FspsUpd->FspsConfig.SaPcieItbtRpNonSnoopLatencyOverrideMode[Index];
      TcssPeiConfig->PcieConfig.PciePortConfig[Index].NonSnoopLatencyOverrideMultiplier = FspsUpd->FspsConfig.SaPcieItbtRpNonSnoopLatencyOverrideMultiplier[Index];
      TcssPeiConfig->PcieConfig.PciePortConfig[Index].NonSnoopLatencyOverrideValue = FspsUpd->FspsConfig.SaPcieItbtRpNonSnoopLatencyOverrideValue[Index];
      TcssPeiConfig->PcieConfig.PciePortConfig[Index].ForceLtrOverride = FspsUpd->FspsConfig.SaPcieItbtRpForceLtrOverride[Index];
      TcssPeiConfig->PcieConfig.PciePortConfig[Index].LtrConfigLock = FspsUpd->FspsConfig.SaPcieItbtRpLtrConfigLock[Index];
    }
  }


  if (GnaConfig != NULL) {
    GnaConfig->GnaEnable = FspsUpd->FspsConfig.GnaEnable;
  }


  if (TcssPeiConfig != NULL){
    TcssPeiConfig->IomConfig.IomInterface.UsbOverride = FspsUpd->FspsConfig.UsbOverride;
    TcssPeiConfig->IomConfig.IomInterface.VccSt = FspsUpd->FspsConfig.VccSt;
    TcssPeiConfig->IomConfig.IomInterface.D3HotEnable = FspsUpd->FspsConfig.D3HotEnable;
    TcssPeiConfig->IomConfig.IomInterface.D3ColdEnable = FspsUpd->FspsConfig.D3ColdEnable;
    TcssPeiConfig->IomConfig.PmcInterface.PmcPdEnable = FspsUpd->FspsConfig.PmcPdEnable;
    for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
      TcssPeiConfig->PcieConfig.PciePortConfig[Index].PtmEnabled = FspsUpd->FspsConfig.PtmEnabled[Index];
    }
    TcssPeiConfig->UsbConfig.EnableComplianceMode = FspsUpd->FspsConfig.TcssXhciEnableComplianceMode;
    TcssPeiConfig->UsbConfig.PortLoopbackModeBitMap = FspsUpd->FspsConfig.TcssLoopbackModeBitMap;
  }
  if (TcssPeiConfig != NULL) {
    TcssPeiConfig->IomConfig.IomAuxPortPad[0].GpioPullN = FspsUpd->FspsConfig.IomTypeCPortPadCfg[0];
    TcssPeiConfig->IomConfig.IomAuxPortPad[0].GpioPullP = FspsUpd->FspsConfig.IomTypeCPortPadCfg[1];
    TcssPeiConfig->IomConfig.IomAuxPortPad[1].GpioPullN = FspsUpd->FspsConfig.IomTypeCPortPadCfg[2];
    TcssPeiConfig->IomConfig.IomAuxPortPad[1].GpioPullP = FspsUpd->FspsConfig.IomTypeCPortPadCfg[3];
    TcssPeiConfig->IomConfig.IomAuxPortPad[2].GpioPullN = FspsUpd->FspsConfig.IomTypeCPortPadCfg[4];
    TcssPeiConfig->IomConfig.IomAuxPortPad[2].GpioPullP = FspsUpd->FspsConfig.IomTypeCPortPadCfg[5];
    TcssPeiConfig->IomConfig.IomAuxPortPad[3].GpioPullN = FspsUpd->FspsConfig.IomTypeCPortPadCfg[6];
    TcssPeiConfig->IomConfig.IomAuxPortPad[3].GpioPullP = FspsUpd->FspsConfig.IomTypeCPortPadCfg[7];

    TcssPeiConfig->IomConfig.IomOverrides.AuxOri = FspsUpd->FspsConfig.TcssAuxOri;
    TcssPeiConfig->IomConfig.IomOverrides.HslOri = FspsUpd->FspsConfig.TcssHslOri;

    TcssPeiConfig->UsbConfig.OverCurrentEnable = FspsUpd->FspsConfig.PchUsbOverCurrentEnable;
    for (Index = 0; Index < TCSS_MAX_USB3_PORTS; Index++) {
      TcssPeiConfig->UsbConfig.PortUsb30[Index].OverCurrentPin = FspsUpd->FspsConfig.CpuUsb3OverCurrentPin[Index];
      TcssPeiConfig->UsbConfig.PortUsb30[Index].Enable = (FspsUpd->FspsConfig.UsbTcPortEn >> Index) & 0x01;
    }
  }
  return EFI_SUCCESS;
}
