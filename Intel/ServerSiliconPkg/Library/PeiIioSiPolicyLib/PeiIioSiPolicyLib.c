/** @file
  This file provides services for PEI policy default initialization

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation. <BR>

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

#include <Library/BaseMemoryLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/IioSysInfoLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <IioSetupDefinitions.h>
#include <IioRegs.h>
#include <Ppi/IioSiPolicyPpi.h>
#include <IioConfig.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <IioPlatformData.h>
#include <Upi/KtiHost.h>
#include <Library/PeiIioSiPolicyLib.h>
#include <Library/ConfigBlockLib.h>




/**
  CreateDefaultIioDfxSiPolicy generates the default Iio Pei Policy.

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CreateDefaultIioDfxSiPolicy (
  IIO_GLOBALS                 *IioGlobalData
  )
{
  UINT16                      PortIndex;
  UINT8                       Socket;
  UINT8                       Stack;
  UINT8                       Func;


  IioGlobalData->SetupData.DfxLtssmLogger               = IIO_OPTION_DISABLE;
  IioGlobalData->SetupData.DfxLtssmLoggerStop           = 0x99;
  IioGlobalData->SetupData.DfxLtssmLoggerSpeed          = 0x1;
  IioGlobalData->SetupData.DfxLtssmLoggerMask           = 0xff;
  IioGlobalData->SetupData.DfxJitterLogger              = IIO_OPTION_DISABLE;

  IioGlobalData->SetupData.DfxIioRcFlow                 = 0x2;
  IioGlobalData->SetupData.DfxIioLinkTraining           = 0x2;
  IioGlobalData->SetupData.DfxOtcPipeHazardThresh       = 0;

  IioGlobalData->SetupData.DfxBadTransactionTypeWa      = 0x2;
  IioGlobalData->SetupData.DfxNpkAllocatingFlow         = IIO_OPTION_DISABLE;

  IioGlobalData->SetupData.DFXEnable                    = IIO_OPTION_DISABLE;
  IioGlobalData->SetupData.DfxDisableBiosDone           = IIO_OPTION_DISABLE;

  for (PortIndex=0; PortIndex < MAX_TOTAL_PORTS; PortIndex++) {

#if (defined(SNR_HOST) || defined(SKX_HOST) || defined(CLX_HOST) || defined(CPX_HOST))
    //
    // DFX Gen3 in Gen3
    //
    IioGlobalData->SetupData.IioPcieConfig.DfxGen3OverrideMode[PortIndex]         = DFX_TXEQ_OVERRIDE_MODE_PHY;
    IioGlobalData->SetupData.IioPcieConfig.DfxGen3TestCard[PortIndex]             = DFX_TXEQ_TEST_CARD_LAGUNA;
    IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh2_Precursor[PortIndex]  = TXEQ_PH2_G3_PRECURSOR_DEFAULT;
    IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh2_Postcursor[PortIndex] = TXEQ_PH2_G3_POSTCURSOR_DEFAULT;
    IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh2_Cursor[PortIndex]     = TXEQ_PH2_G3_CURSOR_DEFAULT;
    IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh3_Precursor[PortIndex]  = TXEQ_PH3_PRECURSOR_DEFAULT;
    IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh3_Cursor[PortIndex]     = TXEQ_PH3_CURSOR_DEFAULT;
    IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh3_Postcursor[PortIndex] = TXEQ_PH3_POSTCURSOR_DEFAULT;

#else //(defined(SNR_HOST) || defined(SKX_HOST) || defined(CLX_HOST) || defined(CPX_HOST))
    //
    // DFX Gen3 in Gen4
    //
    IioGlobalData->SetupData.IioPcieConfig.DfxGen3OverrideMode[PortIndex]         = DFX_TXEQ_OVERRIDE_MODE_PHY;
    IioGlobalData->SetupData.IioPcieConfig.DfxGen3TestCard[PortIndex]             = DFX_TXEQ_TEST_CARD_LAGUNA;

    if (PortIndex % NUMBER_PORTS_PER_SOCKET == 0) {
      IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh2_Precursor[PortIndex]  = TXEQ_PH2_G3_PRECURSOR_DEFAULT;
      IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh2_Postcursor[PortIndex] = TXEQ_PH2_G3_POSTCURSOR_DEFAULT;
      IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh2_Cursor[PortIndex]     = TXEQ_PH2_G3_CURSOR_DEFAULT;
      IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh3_Cursor[PortIndex]     = TXEQ_PH3_CURSOR_DEFAULT;
    } else {
      IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh2_Precursor[PortIndex]  = TXEQ_PH2_G4_PRECURSOR_DEFAULT;
      IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh2_Postcursor[PortIndex] = TXEQ_PH2_G4_POSTCURSOR_DEFAULT;
      IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh2_Cursor[PortIndex]     = TXEQ_PH2_G4_CURSOR_DEFAULT;
      IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh3_Cursor[PortIndex]     = 0;  //not clear where this value comes from
    }

    IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh3_Precursor[PortIndex]  = TXEQ_PH3_PRECURSOR_DEFAULT;

    IioGlobalData->SetupData.IioPcieConfig.DfxGen3ManualPh3_Postcursor[PortIndex] = TXEQ_PH3_POSTCURSOR_DEFAULT;

#endif  //(defined(SNR_HOST) || defined(SKX_HOST) || defined(CLX_HOST) || defined(CPX_HOST))
    //
    // DFX Gen4
    //
    IioGlobalData->SetupData.IioPcieConfig.DfxGen4OverrideMode[PortIndex]         = DFX_TXEQ_OVERRIDE_MODE_PHY;
    IioGlobalData->SetupData.IioPcieConfig.DfxGen4TestCard[PortIndex]             = DFX_TXEQ_TEST_CARD_LAGUNA;
    IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh2_Precursor[PortIndex]  = TXEQ_PH2_G4_PRECURSOR_DEFAULT;
    IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh2_Postcursor[PortIndex] = TXEQ_PH2_G4_POSTCURSOR_DEFAULT;
    if (PortIndex == 0) {
      IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh2_Cursor[PortIndex]     = 0;  //not clear where this value comes from
      IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh3_Precursor[PortIndex]  = 0;  //not clear where this value comes from
      IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh3_Postcursor[PortIndex] = 0;  //not clear where this value comes from
    } else {
      IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh2_Cursor[PortIndex]     = TXEQ_PH2_G4_CURSOR_DEFAULT;
      IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh3_Precursor[PortIndex]  = TXEQ_PH3_PRECURSOR_DEFAULT;
     IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh3_Postcursor[PortIndex]  = TXEQ_PH3_POSTCURSOR_DEFAULT;
    }


    IioGlobalData->SetupData.IioPcieConfig.DfxGen4ManualPh3_Cursor[PortIndex]     = 0;  //not defined in hfr


    //
    // DFX Common
    //
    IioGlobalData->SetupData.IioPcieConfig.DfxDnTxPreset[PortIndex]               = DFX_TX_PRESET_AUTO;
    IioGlobalData->SetupData.IioPcieConfig.DfxRxPreset[PortIndex]                 = DFX_RX_PRESET_AUTO;
    IioGlobalData->SetupData.IioPcieConfig.DfxUpTxPreset[PortIndex]               = DFX_TX_PRESET_AUTO;

    IioGlobalData->SetupData.IioPcieConfig.DfxDnTxPresetGen4[PortIndex]           = DFX_TX_PRESET_AUTO;
    IioGlobalData->SetupData.IioPcieConfig.DfxUpTxPresetGen4[PortIndex]           = DFX_TX_PRESET_AUTO;

    IioGlobalData->SetupData.DfxLinkRetrainEn[PortIndex]     = 0;
    IioGlobalData->SetupData.InboundConfiguration[PortIndex] = 0;
  } // for (Index ...

  IioGlobalData->SetupData.SkipPortPersonalityLock = IIO_OPTION_DISABLE;
  IioGlobalData->SetupData.CxlHeaderBypass = IIO_OPTION_DISABLE;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++ ) {
    for (Stack = 0; Stack < NUM_DEVHIDE_UNCORE_STACKS; Stack++) {
      for (Func = 0; Func < NUM_DEVHIDE_REGS_PER_STACK; Func++) {
        IioGlobalData->SetupData.DfxDevHideOnIioStacks[Socket][Stack][Func] = 0;
        IioGlobalData->SetupData.DfxDevHideOnUncoreStacks[Socket][Stack][Func] = 0;
      }
    }
  }

  IioGlobalData->IioVar.IioVData.EVMode = IIO_OPTION_DISABLE;

  return EFI_SUCCESS;

}


/**
  IioCreateDefaultPolicy generates the default Iio Pei Policy.

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CreateDefaultIioSiPolicy (
  VOID
  )
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  VOID*                       *GuidHob;
  IIO_GLOBALS                 *IioGlobalData;
  UINT16                      Index;
  UINT16                      IioIndex;
  UINT16                      PortIndex;
  UINT8                       MaxNtbPort;
  UINT8                       Socket;
  UINT8                       Iou;

  IioGlobalData = AllocateZeroPool (sizeof (*IioGlobalData));
  if (IioGlobalData == NULL) {
    ASSERT (IioGlobalData != NULL);
    return EFI_OUT_OF_RESOURCES;
  }
  MaxNtbPort = MAX_SOCKET * NUMBER_NTB_PORTS_PER_SOCKET;
  //
  // Override Only the ones that are not Zero by default
  //
  IioGlobalData->SetupData.VTdSupport                   = IIO_OPTION_ENABLE;
  IioGlobalData->SetupData.ATS                          = IIO_OPTION_ENABLE;
  IioGlobalData->SetupData.CoherencySupport             = IIO_OPTION_ENABLE;
  IioGlobalData->SetupData.VtdAcsWa                     = IIO_OPTION_ENABLE;
  IioGlobalData->SetupData.CompletionTimeoutGlobal      = IIO_OPTION_ENABLE;
  IioGlobalData->SetupData.CompletionTimeoutGlobalValue = COMPLETION_TIMEOUT_260MS_900MS;
  IioGlobalData->SetupData.SnoopResponseHoldOff         = SNOOP_RESP_DEF_VALUE;
  IioGlobalData->SetupData.IioPcieConfig.PCIe_LTR       = IIO_OPTION_AUTO;
  IioGlobalData->SetupData.PcieExtendedTagField         = IIO_OPTION_ENABLE;
  IioGlobalData->SetupData.Pcie10bitTag                 = IIO_OPTION_DISABLE;
  IioGlobalData->SetupData.PCIe_AtomicOpReq             = IIO_OPTION_ENABLE;
  IioGlobalData->SetupData.IioPcieConfig.PciePtm        = IIO_OPTION_ENABLE;
  IioGlobalData->SetupData.DualCvIoFlow                 = IIO_OPTION_ENABLE;
  IioGlobalData->SetupData.PcieBiosTrainEnable          = IIO_OPTION_ENABLE;
  IioGlobalData->SetupData.McastIndexPosition           = MC_INDEX_POS_12;
  IioGlobalData->SetupData.McastNumGroup                = MC_NUM_GROUP_8;
  IioGlobalData->SetupData.DmiAllocatingFlow            = IIO_OPTION_ENABLE;
  IioGlobalData->SetupData.PcieAllocatingFlow           = IIO_OPTION_ENABLE;

  for (Socket = 0; Socket < ARRAY_SIZE (IioGlobalData->SetupData.ConfigIOU); Socket++) {
    for (Iou = 0; Iou < ARRAY_SIZE (IioGlobalData->SetupData.ConfigIOU[0]); Iou++) {
      IioGlobalData->SetupData.ConfigIOU[Socket][Iou] = CONFIG_IOU_AUTO;
    }
  }

  for (Socket = 0; Socket < ARRAY_SIZE (IioGlobalData->SetupData.CompletionTimeoutValue); Socket++) {
    IioGlobalData->SetupData.CompletionTimeoutValue[Socket] = COMPLETION_TIMEOUT_260MS_900MS;
  }

  for (Socket = 0; Socket < ARRAY_SIZE (IioGlobalData->SetupData.CpuTraceHubConfig); Socket++) {
    for (Index = 0; Index < NUMBER_TRACE_HUB_PER_SOCKET; Index++) {
      IioGlobalData->SetupData.CpuTraceHubConfig[Socket][Index].EnableMode = IIO_OPTION_DISABLE;
    }
  }

  for (Index = 0; Index < MAX_TOTAL_PORTS; Index++) {
    if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {

      IioIndex = Index / NUMBER_PORTS_PER_SOCKET;
      PortIndex = Index % NUMBER_PORTS_PER_SOCKET;

      if ((IioGlobalData->IioVar.IioVData.SkuPersonality[IioIndex] == TYPE_FPGA) &&
          (PortIndex >= PORT_4A_INDEX) && (PortIndex <= PORT_4D_INDEX)) {
        IioGlobalData->SetupData.IioPcieConfig.PcieAspm[IioIndex][PortIndex] = PCIE_ASPM_L0S_ONLY;
      } else {
        IioGlobalData->SetupData.IioPcieConfig.PcieAspm[IioIndex][PortIndex] = PCIE_ASPM_AUTO;
      }
    }
    IioGlobalData->SetupData.PcieCommonClock[Index]     = IIO_OPTION_ENABLE;
    IioGlobalData->SetupData.IioPcieConfig.PcieMaxPayload[Index]      = PCIE_MAX_PAYLOAD_AUTO;
    IioGlobalData->SetupData.PcieL1Latency[Index]       = PCIE_L1_8US_16US;
    //
    // PCIE setup options for MISCCTRLSTS
    //
    IioGlobalData->SetupData.EOI[Index]                 = IIO_OPTION_ENABLE;
  }

  for (Index=0; Index < TOTAL_CB3_DEVICES; Index++){
    SetMem (IioGlobalData->SetupData.Cb3DmaEn,IIO_OPTION_ENABLE,TOTAL_CB3_DEVICES * sizeof(UINT8));
  }
  IioGlobalData->SetupData.NtbLinkBiosTrainEn = IIO_OPTION_DISABLE;
  for (Index = 0; Index < MaxNtbPort; Index++){
    IioGlobalData->SetupData.NtbBarSizeImBar1[Index]      = NTB_BARSIZE_IMBAR1_DEFAULT;
    IioGlobalData->SetupData.NtbBarSizeImBar2[Index]      = NTB_BARSIZE_IMBAR2_DEFAULT;
    IioGlobalData->SetupData.NtbBarSizeImBar2_0[Index]    = NTB_BARSIZE_IMBAR2_0_DEFAULT;
    IioGlobalData->SetupData.NtbBarSizeImBar2_1[Index]    = NTB_BARSIZE_IMBAR2_1_DEFAULT;
    IioGlobalData->SetupData.NtbBarSizeEmBarSZ1[Index]    = NTB_BARSIZE_EMBAR1_DEFAULT;
    IioGlobalData->SetupData.NtbBarSizeEmBarSZ2[Index]    = NTB_BARSIZE_EMBAR2_DEFAULT;
    IioGlobalData->SetupData.NtbBarSizeEmBarSZ2_0[Index]  = NTB_BARSIZE_EMBAR2_0_DEFAULT;
    IioGlobalData->SetupData.NtbBarSizeEmBarSZ2_1[Index]  = NTB_BARSIZE_EMBAR2_0_DEFAULT;
    IioGlobalData->SetupData.NtbXlinkCtlOverride[Index]   = NTB_IIO_XLINK_CTL_DSD_USP;
  }

  for (Socket = 0; Socket < MaxIIO; Socket++) {
    for (Index = 0; Index < MAX_VMD_STACKS_PER_SOCKET; Index++) {
      IioGlobalData->SetupData.VMDCfgBarSz[Socket][Index]  = VMD_CFG_BAR_SIZE_DEFAULT;
      IioGlobalData->SetupData.VMDMemBarSz1[Socket][Index] = VMD_MEM_BAR_SIZE1_DEFAULT;
      IioGlobalData->SetupData.VMDMemBarSz2[Socket][Index] = VMD_MEM_BAR_SIZE2_DEFAULT;
    }
  }

  CreateDefaultIioDfxSiPolicy (IioGlobalData);


  GuidHob = BuildGuidDataHob (&gIioSiPolicyHobGuid, IioGlobalData, sizeof (*IioGlobalData));
  if (GuidHob == NULL) {
    ASSERT (GuidHob != NULL);
    Status = EFI_OUT_OF_RESOURCES;
  } else {
    DEBUG ((DEBUG_INFO, "[IIO] %a published default IIO HOB\n", __FUNCTION__));
  }

  FreePool (IioGlobalData);
  return Status;
}

/**
  Get IIO PCIe config block table total size.

  @return  UINT16   Size of PCH config block table
**/
UINT16
EFIAPI
IioGetPcieConfigBlockTotalSize (
  VOID
)
{
  return (UINT16) sizeof (IIO_PCIE_CONFIG);
}

/**
  Initialize PCIE Config block with default values

  @return   EFI_STATUS
**/
EFI_STATUS
EFIAPI
LoadIioPcieConfigBlockDefault (
IN VOID *ConfigBlockPointer
)
{
  IIO_PCIE_CONFIG   *IioPcieConfig;

  IioPcieConfig = ConfigBlockPointer;

  return EFI_SUCCESS;
}

/**
  This routine add PCIE Config block in IIO POLICY Config Table

  @retval               EFI_STATUS
**/
EFI_STATUS
EFIAPI
IioAddPcieConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
)
{
  VOID                 *ConfigBlockPointer;
  EFI_STATUS           Status;
  CONFIG_BLOCK_HEADER  IioPcieBlock;

  //
  // Initalize & add Pcie Config Block
  //
  CopyMem (&(IioPcieBlock.GuidHob.Name), &gIioPcieConfigGuid, sizeof(EFI_GUID));
  IioPcieBlock.GuidHob.Header.HobLength = sizeof (IIO_PCIE_CONFIG);
  IioPcieBlock.Revision = IIO_PCIE_CONFIG_REVISION;

  ConfigBlockPointer = (VOID *)&IioPcieBlock;
  Status = AddConfigBlock (ConfigBlockTableAddress, (VOID *)&ConfigBlockPointer);
  ASSERT_EFI_ERROR (Status);

  LoadIioPcieConfigBlockDefault ((VOID *)ConfigBlockPointer);

  return Status;
}

/**
  IioCreateConfigBlocks creates the config blocks of IIO Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[in,out] IioPolicyPpi         The pointer to get IIO Policy PPI instance

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
IioCreateConfigBlocks (
  OUT  IIO_POLICY_STRUCT **IioPolicyPpi
)
{
  UINT16        TotalBlockSize;
  EFI_STATUS    Status;
  IIO_POLICY_STRUCT *IioPolicy;
  UINT16        RequiredSize;

  IioPolicy = NULL;

  TotalBlockSize = IioGetPcieConfigBlockTotalSize();
  DEBUG ((DEBUG_INFO, "TotalBlockSize = 0x%x\n", TotalBlockSize));

  RequiredSize = sizeof (CONFIG_BLOCK_TABLE_HEADER) + TotalBlockSize;

  Status = CreateConfigBlockTable (RequiredSize, (VOID *)&IioPolicy);
  ASSERT_EFI_ERROR (Status);

  IioPolicy->TableHeader.Header.Revision = IIO_POLICY_PPI_REVISION;

  //
  // Add config blocks.
  //
  Status = IioAddPcieConfigBlocks ((VOID *)IioPolicy);
  ASSERT_EFI_ERROR (Status);

  *IioPolicyPpi = IioPolicy;
  return Status;
}

/**
  This routine installs IioPolicyPpi.

  @param[in] IioPolicyPpi         The pointer to Iio Policy PPI instance

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
IioInstallPolicyPpi (
IN  IIO_POLICY_STRUCT *IioPolicyPpi
)
{
  EFI_STATUS             Status;
  EFI_PEI_PPI_DESCRIPTOR *IioPolicyPpiDesc;
  IIO_PCIE_CONFIG        *IioPcieConfig;

  IioPolicyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *)AllocateZeroPool(sizeof(EFI_PEI_PPI_DESCRIPTOR));
  if (IioPolicyPpiDesc == NULL) {
    ASSERT(FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  IioPolicyPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  IioPolicyPpiDesc->Guid = &gIioPolicyPpiGuid;
  IioPolicyPpiDesc->Ppi = IioPolicyPpi;
  Status = GetConfigBlock ((VOID *)IioPolicyPpi, &gIioPcieConfigGuid, (VOID *)&IioPcieConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Install IIO Policy PPI
  //
  Status = PeiServicesInstallPpi (IioPolicyPpiDesc);
  ASSERT_EFI_ERROR (Status);
  return Status;
}