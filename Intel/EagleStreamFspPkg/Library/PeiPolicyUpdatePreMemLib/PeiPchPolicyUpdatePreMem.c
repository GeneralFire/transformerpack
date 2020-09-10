/** @file
  This file is SampleCode of the library for Intel PCH PEI Policy initialization.

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

#include <Library/DebugLib.h>
#include <Ppi/PchPolicy.h>
#include <Library/FspCommonLib.h>
#include <Library/ConfigBlockLib.h>
#include <FspmUpd.h>
#include <Library/PchInfoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

/**
  This function performs PCH PEI Policy update in pre-memory.

  @param[in, out] PchPolicyPpi    The PCH PreMem Policy PPI instance
  @param[in]      FspmUpd         The pointer of MemoryInitUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiPchPolicyPreMem (
  IN OUT PCH_POLICY_PPI           *PchPolicyPpi,
  IN     FSPM_UPD                 *FspmUpd
  )
{
/*
  PCH_TRACE_HUB_CONFIG            *PchTraceHubConfig;
  PCH_SMBUS_CONFIG                *SmbusConfig;
  UINT8                           *RsvdSmbusTable;
  UINT8                           NumRsvdSmbusTable;
  PCH_DCI_CONFIG                  *DciConfig;
*/
/*
  UINTN                           Index;
  UINTN                           MaxPcieRootPorts;
  PCH_HSIO_SATA_CONFIG            *HsioSataConfig;
  PCH_HSIO_SATA_CONFIG            *HsiosSataConfig;
  PCH_HSIO_PCIE_CONFIG            *HsioPcieConfig;
  PCH_HSIO_PCIE_WM20_CONFIG       *HsioPcieConfigFIAWM20;
  EFI_STATUS                      Status;

  PCH_GENERAL_CONFIG              *PchConfig;
  PCH_PCIE_CONFIG                 *PcieConfig;
  PCH_SATA_CONFIG                 *SataConfig;
  PCH_USB_CONFIG                  *UsbConfig;
  PCH_IOAPIC_CONFIG               *IoApicConfig;
  PCH_HPET_CONFIG                 *HpetConfig;
  PCH_HDAUDIO_CONFIG              *HdAudioConfig;
  PCH_LAN_CONFIG                  *LanConfig;
  PCH_LOCK_DOWN_CONFIG            *LockDownConfig;
  PCH_THERMAL_CONFIG              *ThermalConfig;
  PCH_PM_CONFIG                   *PmConfig;
  PCH_DMI_CONFIG                  *DmiConfig;
  PCH_LPC_SIRQ_CONFIG             *SerialIrqConfig;
  PCH_INTERRUPT_CONFIG            *PchInterruptConfig;
  PCH_PORT61H_SMM_CONFIG          *Port61hSmmConfig;
  PCH_FLASH_PROTECTION_CONFIG     *FlashProtectConfig;
  PCH_SATA_CONFIG                 *sSataConfig;
  PCH_WDT_CONFIG                  *WdtConfig;
  PCH_P2SB_CONFIG                 *P2sbConfig;
  PCH_LPC_CONFIG                  *LpcConfig;
  PCH_SKYCAM_CIO2_FLS_CONFIG      *PchCio2FlsConfig;
  PCH_SPI_CONFIG                  *SpiConfig;
  PCH_PCIE_CONFIG2                *PcieConfig2;
  PCH_ADR_CONFIG                  *AdrConfig;
*/
/*
  PchTraceHubConfig = &PchPolicyPpi->PchTraceHubConfig;
  SmbusConfig       = &PchPolicyPpi->SmbusConfig;
  DciConfig         = &PchPolicyPpi->DciConfig;
  //
  // Update TraceHub policies
  //
  PchTraceHubConfig->TraceHub.EnableMode  = FspmUpd->FspmConfig.PchTraceHubMode;
  PchTraceHubConfig->TraceHub.MemReg0Size = FspmUpd->FspmConfig.PchTraceHubMemReg0Size;
  PchTraceHubConfig->TraceHub.MemReg1Size = FspmUpd->FspmConfig.PchTraceHubMemReg1Size;

  //
  // Update Smbus policies
  //
  SmbusConfig->Enable                = FspmUpd->FspmConfig.SmbusEnable;
  SmbusConfig->ArpEnable             = FspmUpd->FspmConfig.SmbusArpEnable;
  SmbusConfig->DynamicPowerGating    = FspmUpd->FspmConfig.SmbusDynamicPowerGating;
  SmbusConfig->SmbusIoBase           = FspmUpd->FspmConfig.PchSmbusIoBase;
  SmbusConfig->NumRsvdSmbusAddresses = FspmUpd->FspmConfig.PchNumRsvdSmbusAddresses;


  if (FspmUpd->FspmConfig.PchNumRsvdSmbusAddresses != 0) {
    NumRsvdSmbusTable = FspmUpd->FspmConfig.PchNumRsvdSmbusAddresses;
    RsvdSmbusTable = (UINT8 *)FspmUpd->FspmConfig.RsvdSmbusAddressTablePtr;
    ZeroMem (
      SmbusConfig->RsvdSmbusAddressTable,
      PCH_MAX_SMBUS_RESERVED_ADDRESS * sizeof (UINT8)
      );
    CopyMem (
      SmbusConfig->RsvdSmbusAddressTable,
      RsvdSmbusTable,
      NumRsvdSmbusTable * sizeof (UINT8)
      );
  }
/*
  //
  // Update Dci policies
  //
  DciConfig->DciEn              = FspmUpd->FspmConfig.DciEn;
*/
/*
  MaxPcieRootPorts = GetPchMaxPciePortNum ();

  //
  // Update HSIO PCIE policies
  //
  for (Index = 0; Index < MaxPcieRootPorts; Index ++) {
    HsioPciePreMemConfig->Lane[Index].HsioRxSetCtleEnable           = FspmUpd->FspmConfig.PchPcieHsioRxSetCtleEnable[Index];
    HsioPciePreMemConfig->Lane[Index].HsioRxSetCtle                 = FspmUpd->FspmConfig.PchPcieHsioRxSetCtle[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen1DownscaleAmpEnable  = FspmUpd->FspmConfig.PchPcieHsioTxGen1DownscaleAmpEnable[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen1DownscaleAmp        = FspmUpd->FspmConfig.PchPcieHsioTxGen1DownscaleAmp[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen2DownscaleAmpEnable  = FspmUpd->FspmConfig.PchPcieHsioTxGen2DownscaleAmpEnable[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen2DownscaleAmp        = FspmUpd->FspmConfig.PchPcieHsioTxGen2DownscaleAmp[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen3DownscaleAmpEnable  = FspmUpd->FspmConfig.PchPcieHsioTxGen3DownscaleAmpEnable[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen3DownscaleAmp        = FspmUpd->FspmConfig.PchPcieHsioTxGen3DownscaleAmp[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen1DeEmphEnable        = FspmUpd->FspmConfig.PchPcieHsioTxGen1DeEmphEnable[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen1DeEmph              = FspmUpd->FspmConfig.PchPcieHsioTxGen1DeEmph[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph3p5Enable     = FspmUpd->FspmConfig.PchPcieHsioTxGen2DeEmph3p5Enable[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph3p5           = FspmUpd->FspmConfig.PchPcieHsioTxGen2DeEmph3p5[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph6p0Enable     = FspmUpd->FspmConfig.PchPcieHsioTxGen2DeEmph6p0Enable[Index];
    HsioPciePreMemConfig->Lane[Index].HsioTxGen2DeEmph6p0           = FspmUpd->FspmConfig.PchPcieHsioTxGen2DeEmph6p0[Index];
  }

  //
  // Update HSIO SATA policies
  //
  for (Index = 0; Index < PCH_MAX_SATA_PORTS; Index ++) {
    HsioSataPreMemConfig->PortLane[Index].HsioRxGen1EqBoostMagEnable    = FspmUpd->FspmConfig.PchSataHsioRxGen1EqBoostMagEnable[Index];
    HsioSataPreMemConfig->PortLane[Index].HsioRxGen1EqBoostMag          = FspmUpd->FspmConfig.PchSataHsioRxGen1EqBoostMag[Index];
    HsioSataPreMemConfig->PortLane[Index].HsioRxGen2EqBoostMagEnable    = FspmUpd->FspmConfig.PchSataHsioRxGen2EqBoostMagEnable[Index];
    HsioSataPreMemConfig->PortLane[Index].HsioRxGen2EqBoostMag          = FspmUpd->FspmConfig.PchSataHsioRxGen2EqBoostMag[Index];
    HsioSataPreMemConfig->PortLane[Index].HsioRxGen3EqBoostMagEnable    = FspmUpd->FspmConfig.PchSataHsioRxGen3EqBoostMagEnable[Index];
    HsioSataPreMemConfig->PortLane[Index].HsioRxGen3EqBoostMag          = FspmUpd->FspmConfig.PchSataHsioRxGen3EqBoostMag[Index];
    HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DownscaleAmpEnable  = FspmUpd->FspmConfig.PchSataHsioTxGen1DownscaleAmpEnable[Index];
    HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DownscaleAmp        = FspmUpd->FspmConfig.PchSataHsioTxGen1DownscaleAmp[Index];
    HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DownscaleAmpEnable  = FspmUpd->FspmConfig.PchSataHsioTxGen2DownscaleAmpEnable[Index];
    HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DownscaleAmp        = FspmUpd->FspmConfig.PchSataHsioTxGen2DownscaleAmp[Index];
    HsioSataPreMemConfig->PortLane[Index].HsioTxGen3DownscaleAmpEnable  = FspmUpd->FspmConfig.PchSataHsioTxGen3DownscaleAmpEnable[Index];
    HsioSataPreMemConfig->PortLane[Index].HsioTxGen3DownscaleAmp        = FspmUpd->FspmConfig.PchSataHsioTxGen3DownscaleAmp[Index];
    HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DeEmphEnable        = FspmUpd->FspmConfig.PchSataHsioTxGen1DeEmphEnable[Index];
    HsioSataPreMemConfig->PortLane[Index].HsioTxGen1DeEmph              = FspmUpd->FspmConfig.PchSataHsioTxGen1DeEmph[Index];
    HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DeEmphEnable        = FspmUpd->FspmConfig.PchSataHsioTxGen2DeEmphEnable[Index];
    HsioSataPreMemConfig->PortLane[Index].HsioTxGen2DeEmph              = FspmUpd->FspmConfig.PchSataHsioTxGen2DeEmph[Index];
    HsioSataPreMemConfig->PortLane[Index].HsioTxGen3DeEmphEnable        = FspmUpd->FspmConfig.PchSataHsioTxGen3DeEmphEnable[Index];
    HsioSataPreMemConfig->PortLane[Index].HsioTxGen3DeEmph              = FspmUpd->FspmConfig.PchSataHsioTxGen3DeEmph[Index];
  }

  //
  // Update LPC policies
  //
  LpcPreMemConfig->EnhancePort8xhDecoding = FspmUpd->FspmConfig.PchLpcEnhancePort8xhDecoding;
*/
/*
  //
  // Update port 80 policies
  //
  PchPolicyPpi->Port80Route = FspmUpd->FspmConfig.PchPort80Route;
*/
/*
  //
  // Update Wdt policies
  //
  WdtPreMemConfig->DisableAndLock = FspmUpd->FspmConfig.WdtDisableAndLock;

  //
  // HdAudioConfig
  //
  HdaPreMemConfig->Enable = FspmUpd->FspmConfig.PchHdaEnable;

  //
  // IshConfig
  //
  IshPreMemConfig->Enable = FspmUpd->FspmConfig.PchIshEnable;


*/
  return EFI_SUCCESS;
}
