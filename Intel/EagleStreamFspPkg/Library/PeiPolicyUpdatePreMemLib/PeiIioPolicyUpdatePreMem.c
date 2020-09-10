/** @file
  This file is SampleCode of the library for Intel PCH PEI Policy initialization.

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

#include <Library/DebugLib.h>
#include <Ppi/IioSiPolicyPpi.h>
#include <Library/FspCommonLib.h>
#include <Library/ConfigBlockLib.h>
#include <FspmUpd.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>

/**
  This function performs IIO PEI Policy update in pre-memory.

  @param[in, out] IioSiPolicyPpi  The IIO PreMem Policy PPI instance
  @param[in]      FspmUpd         The pointer of MemoryInitUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiIioPolicyPreMem (
  IN OUT IIO_SI_POLICY_PPI        *IioSiPolicyPpi,
  IN     FSPM_UPD                 *FspmUpd
  )
{
  IIO_GLOBALS                 *IioGlobalData = NULL;
  EFI_PEI_PPI_DESCRIPTOR      *InstallIioSiPolicyPpiDesc;
  EFI_HOB_GUID_TYPE           *GuidHob = NULL;
  EFI_STATUS                  Status;
  UINT8                       Index;
  UINT8                       PortIndex;
  UINT8                       Socket;

  GuidHob = GetFirstGuidHob (&gIioSiPolicyHobGuid);
  ASSERT(GuidHob != NULL);
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_ERROR, "Cannot locate IIO HOB Data !!\n"));
    return EFI_NOT_FOUND;
  }
  IioGlobalData = (IIO_GLOBALS *) GET_GUID_HOB_DATA (GuidHob);

  //Update the IioGlobalData data with native bios IioGlobalData data
  IioGlobalData->SetupData.VTdSupport = 0x0;
  IioGlobalData->SetupData.InterruptRemap = 0x1;
  IioGlobalData->SetupData.CoherencySupport = 0x1;
  IioGlobalData->SetupData.PostedInterrupt = 0x1;
  IioGlobalData->SetupData.CompletionTimeoutGlobal = 0x0;
  IioGlobalData->SetupData.PcieGlobalAspm = 0x1;
  IioGlobalData->SetupData.SnoopResponseHoldOff = 0x9;
  IioGlobalData->SetupData.PcieMaxReadRequestSize = 0x7;
  IioGlobalData->SetupData.DfxLtssmLoggerStop = 0x99;
  IioGlobalData->SetupData.DfxLtssmLoggerSpeed = 0x1;
  IioGlobalData->SetupData.DfxLtssmLoggerMask = 0xFF;
  IioGlobalData->SetupData.DfxIioRcFlow = 0x2;
  IioGlobalData->SetupData.DfxIioLinkTraining = 0x2;
  IioGlobalData->SetupData.NoSnoopWrCfg  = 0x1;
  IioGlobalData->SetupData.PcieSlotOprom1 = 0x1;
  IioGlobalData->SetupData.PcieSlotOprom2 = 0x1;
  IioGlobalData->SetupData.PcieSlotOprom3 = 0x1;
  IioGlobalData->SetupData.PcieSlotOprom4 = 0x1;
  IioGlobalData->SetupData.PcieSlotOprom5 = 0x1;
  IioGlobalData->SetupData.PcieSlotOprom6 = 0x1;
  IioGlobalData->SetupData.PcieSlotOprom7 = 0x1;
  IioGlobalData->SetupData.PcieSlotOprom8 = 0x1;
  IioGlobalData->SetupData.PcieRelaxedOrdering = 0x1;
  IioGlobalData->SetupData.Pci64BitResourceAllocation = 0x1;
  IioGlobalData->SetupData.LockChipset = 0x1;
  IioGlobalData->SetupData.PeciInTrustControlBit = 0x1;
  IioGlobalData->SetupData.PciePllSsc = 0xFE;
  IioGlobalData->SetupData.IioErrorEn = 0x1;

  IioGlobalData->SetupData.ConfigIOU[0][0] = 0x4;
  IioGlobalData->SetupData.ConfigIOU[0][1] = 0x4;
  IioGlobalData->SetupData.ConfigIOU[0][2] = 0x4;
  IioGlobalData->SetupData.ConfigIOU[0][3] = 0x0;
  IioGlobalData->SetupData.ConfigIOU[0][4] = 0x4;
  IioGlobalData->SetupData.ConfigIOU[1][0] = 0x4;
  IioGlobalData->SetupData.ConfigIOU[1][1] = 0x4;
  IioGlobalData->SetupData.ConfigIOU[1][2] = 0x4;
  IioGlobalData->SetupData.ConfigIOU[1][3] = 0x0;
  IioGlobalData->SetupData.ConfigIOU[1][4] = 0x4;
  IioGlobalData->SetupData.ConfigIOU[2][0] = 0x0;
  IioGlobalData->SetupData.ConfigIOU[2][1] = 0x0;
  IioGlobalData->SetupData.ConfigIOU[2][2] = 0x0;

  for (Socket = 0; Socket < 2; Socket++) {
    for (Index = 0; Index < MAX_VMD_STACKS_PER_SOCKET; Index++) {

      IioGlobalData->SetupData.VMDCfgBarAttr[Socket][Index] = 0x2;
      IioGlobalData->SetupData.VMDMemBar2Attr[Socket][Index] = 0x1;
    }

    for (Index = 0; Index < 21; Index++) {
      PortIndex = Socket * 21 + Index;
      IioGlobalData->SetupData.IioPcieConfig.PcieAspm[Socket][Index] = 0x2;
      IioGlobalData->SetupData.IioPcieConfig.PciePortEnable[PortIndex] = 0x1;
      IioGlobalData->SetupData.DeEmphasis[PortIndex] = 0x1;
      IioGlobalData->SetupData.IODC[PortIndex] = 0x6;
      IioGlobalData->SetupData.MctpEn[PortIndex] = 0x1;
      IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh2_Precursor[PortIndex] = 0xB;
      IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh2_Cursor[PortIndex] = 0x29;
      IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh3_Precursor[PortIndex] = 0xB;
      IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh3_Cursor[PortIndex] = 0x29;
      IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh3_Postcursor[PortIndex] = 0xB;
      IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh2_Precursor[PortIndex] = 0xB;
      IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh2_Cursor[PortIndex] = 0x29;
      IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh3_Precursor[PortIndex] = 0xB;
      IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh3_Cursor[PortIndex] = 0x29;
      IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh3_Cursor[PortIndex] = 0xB;
      IioGlobalData->SetupData.IioPcieConfig.DfxDnTxPreset[PortIndex] = 0xFF;
      IioGlobalData->SetupData.IioPcieConfig.DfxRxPreset[PortIndex] = 0xFF;
      IioGlobalData->SetupData.IioPcieConfig.DfxUpTxPreset[PortIndex] = 0xFF;
      IioGlobalData->SetupData.IioPcieConfig.DfxDnTxPresetGen4[PortIndex] = 0xFF;
      IioGlobalData->SetupData.IioPcieConfig.DfxUpTxPresetGen4[PortIndex] = 0xFF;
    }
  }
  IioGlobalData->SetupData.IioPcieConfig.PciePortEnable[0] = 0x0;
  IioGlobalData->SetupData.DeEmphasis[0] = 0x0;
  IioGlobalData->SetupData.EOI[0] = 0x0;
  IioGlobalData->SetupData.HidePEXPMenu[0] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[2] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[2] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[3] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[3] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[4] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[4] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[6] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[6] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[7] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[7] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[8] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[8] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[10] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[10] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[11] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[11] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[12] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[12] = 0x1;
  IioGlobalData->SetupData.SLOTHPCAP[13] = 0x1;
  IioGlobalData->SetupData.SLOTHPCAP[14] = 0x1;
  IioGlobalData->SetupData.SLOTHPCAP[15] = 0x1;
  IioGlobalData->SetupData.SLOTHPCAP[16] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[18] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[18] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[19] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[19] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[20] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[20] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[21] = 0x1;
  IioGlobalData->SetupData.SLOTHPCAP[22] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[23] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[23] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[24] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[24] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[25] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[25] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[27] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[27] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[28] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[28] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[29] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[29] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[31] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[31] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[32] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[32] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[33] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[33] = 0x1;
  IioGlobalData->SetupData.SLOTHPCAP[34] = 0x1;
  IioGlobalData->SetupData.SLOTHPCAP[35] = 0x1;
  IioGlobalData->SetupData.SLOTHPCAP[36] = 0x1;
  IioGlobalData->SetupData.SLOTHPCAP[37] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[39] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[39] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[40] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[40] = 0x1;
  IioGlobalData->SetupData.PEXPHIDE[41] = 0x1;
  IioGlobalData->SetupData.HidePEXPMenu[41] = 0x1;

  for (Index=0; Index<10; Index++) {
    IioGlobalData->SetupData.NtbBarSizeImBar1[Index] = 0x16;
    IioGlobalData->SetupData.NtbBarSizeImBar2[Index] = 0x16;
    IioGlobalData->SetupData.NtbBarSizeEmBarSZ1[Index] = 0x16;
    IioGlobalData->SetupData.NtbBarSizeEmBarSZ2[Index] = 0x16;
    IioGlobalData->SetupData.NtbXlinkCtlOverride[Index] = 0x3;
  }

  for (Socket=0; Socket<2; Socket++) {
    for (Index=0; Index<4; Index++) {
      IioGlobalData->IioVar.IioVData.StackPerPort[Socket][Index] = 0x3;
    }
  }

  for (Index=0; Index<32; Index++) {
    IioGlobalData->SetupData.Cb3DmaEn[Index] = 0x1;
  }
  IioGlobalData->IioVar.IioVData.PlatformType = 0xA;
  IioGlobalData->IioVar.IioVData.IODC = 0x5;
  IioGlobalData->IioVar.IioOutData.MemResetRequired = 0x2;
  IioGlobalData->IioVar.IioVData.SocketPresent[0] = 0x1;
  IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[0] = 0x15;
  IioGlobalData->IioVar.IioVData.SocketPresent[1] = 0x1;
  IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[1] = 0x15;

  IioSiPolicyPpi->IioGlobalData = IioGlobalData;

  // todo: Update some fields with UPD data.

  InstallIioSiPolicyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  ASSERT (InstallIioSiPolicyPpiDesc != NULL);
  if (InstallIioSiPolicyPpiDesc != NULL) {
    InstallIioSiPolicyPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
    InstallIioSiPolicyPpiDesc->Guid  = &gIioSiPolicyPpiGuid;
    IioSiPolicyPpi->Revision         = IIO_SI_POLICY_PPI_REVISION;
    InstallIioSiPolicyPpiDesc->Ppi   = IioSiPolicyPpi;
    DEBUG ((DEBUG_INFO, "Installing IioSiPolicy \n"));
    Status = PeiServicesInstallPpi (InstallIioSiPolicyPpiDesc);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Installing IioSiPolicy Error = %r\n", Status));
      ASSERT_EFI_ERROR (Status);
    }
  }

  return EFI_SUCCESS;
}
