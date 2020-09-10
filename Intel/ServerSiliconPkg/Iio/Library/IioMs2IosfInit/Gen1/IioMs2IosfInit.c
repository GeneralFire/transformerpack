/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>

#include <IioPlatformData.h>
#include <Library/OemIioInit.h>
#include <IndustryStandard/Pci22.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <RcRegs.h>
#include <Chip/Include/CpuPciAccess.h>
#include "IioMs2IosfInitCommon.h"
#include "IioBankDecoder.h"
#include "IioCredits.h"
#include <Library/SystemInfoLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/IioSysInfoLib.h>
#include <Library/KtiApi.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/IioMiscFeatureLib.h>
#include <IioSetupDefinitions.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/CxlIpLib.h>
#include <Register/Intel/ArchitecturalMsr.h>


#include <LibraryPrivate/IioDebug.h>

extern BANK_DECODER_HEADER       BankDecoderVer;
extern BANK_DECODER_ENTRY        BankDecoderTable[506];

extern CREDITS_HEADER       ItcCreditsVer;
extern CREDITS_ENTRY        ItcCreditDefaultTable[ITC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        ItcCreditDmiTable[ITC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        ItcCreditSinglePcie3x16Table[ITC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        ItcCreditTicFecTable[ITC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        ItcCreditNacPsf0Table[ITC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        ItcCreditNacPsfCpkTable[ITC_NUMBER_ENTRIES];

extern CREDITS_HEADER       OtcCreditsVer;
extern CREDITS_ENTRY        OtcCreditDefaultTable[OTC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        OtcCreditDmiTable[OTC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        OtcCreditSinglePcie3x16Table[OTC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        OtcCreditTicFecTable[OTC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        OtcCreditNacPsf0Table[OTC_NUMBER_ENTRIES];
extern CREDITS_ENTRY        OtcCreditNacPsfCpkTable[OTC_NUMBER_ENTRIES];

/**

  Program MS2IOSF bank decoder shadowed BAR

  @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param IioIndex           - Index of CPU/IIO

  @retval None

**/
VOID
OverrideMs2iosfShadowedBar (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  )
{
  UINT8                 BusNumber;
  UINT16                BankRegister;
  UINT8                 StackIndex;
  UINT8                 MaxIioStackNumber;
  UINT16                Type0BankStop;

  MaxIioStackNumber = GetMaxStackNumPerSocket ();
  for (StackIndex = 0; StackIndex < MaxIioStackNumber; StackIndex++) {
    // Avoid programming if the stack does not include any MS2IOSF devices
    if ((IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_NONE) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_RESERVED) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX)) {
      continue;
    }

    BusNumber = IioGlobalData->IioVar.IioVData.SocketStackBus[IioIndex][StackIndex];

    //
    // Program the upper 32 bits of the shadowed BAR value for type 0 banks with zeroes.
    //
    Type0BankStop = TYPE0_BANK_STOP;
    for (BankRegister = TYPE0_BANK_START; BankRegister < Type0BankStop; BankRegister += TYPE0_OFFSET_PER_BANK) {
      IioPciExpressWrite32 (IioIndex, BusNumber, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, BankRegister + BAR_N1_OFFSET, 0);
    }
  }
}

/**

  MS2IOSF bank decoder EOI broadcast message

  @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param IioIndex           - Index of CPU/IIO

  @retval None

**/
VOID
OverrideMs2iosfEoiBroadcast (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  )
{
  EOI_BROADCAST_EN_IIO_VTD_STRUCT   EoiBroadcastEn;
  UINT8                             StackIndex;
  UINT8                             MaxIioStackNumber;

  MaxIioStackNumber = GetMaxStackNumPerSocket ();
  for (StackIndex = 0; StackIndex < MaxIioStackNumber; StackIndex++) {
    //
    // Avoid to program bank decoder values if the stack does not include any MS2IOSF device
    //
    if ((IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_NONE) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_RESERVED) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX)) {
      continue;
    }

    EoiBroadcastEn.Data = IioReadCpuCsr32 (IioIndex, StackIndex, EOI_BROADCAST_EN_IIO_VTD_REG);

    //
    // Changes to ms2iosf0 (stack 0), EOI Broadcast Enable register.
    // 1. Disable EOI on Bank0 and Bank27 DMI for non-legacy sockets on Gen1.
    // 2. Disable EOI on Bank19 DMI for non-legacy sockets on Gen2.
    // 3. Disable EOI on Banks 23 to 26 which are associated with NPK.
    // 4. Disable Bank 2 NPK (ACPI) for SNR and ICX-D.
    //
    if (StackIndex == IIO_STACK0) {
      if (IioIndex != 0) {
        EoiBroadcastEn.Bits.eoi_bcast_en &= ~(BIT19);
      }
      if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
        EoiBroadcastEn.Bits.eoi_bcast_en &= (~(BIT26 | BIT25 | BIT24 | BIT23));
        if (IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
          EoiBroadcastEn.Bits.eoi_bcast_en &= (~BIT2);
        }
      }
    }

    //
    // HQM WA: Changes to ms2iosf3 (Stack Index IIO_STACK2), EOI Broadcast Enable register.
    // 1. Disable EOI on all Banks for HQM (SNR)
    //
    if (IIO_HQM == IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex] ) {
      if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
        if (IsSiliconWorkaroundEnabled ("S1409665287")) {
          EoiBroadcastEn.Bits.eoi_bcast_en = 0; // Disable EOI for all banks
        }
      }
    }

    IioWriteCpuCsr32 (IioIndex, StackIndex, EOI_BROADCAST_EN_IIO_VTD_REG, EoiBroadcastEn.Data);
  }
}

/**

  Enable/Disable which MS2IOSF decoder banks should be the target of broadcast MCTP messages

  @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param IioIndex           - Index of CPU/IIO

  @retval None

**/
VOID
OverrideMs2iosfMctpBroadcast (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  )
{
  MCTP_BROADCAST_EN_IIO_VTD_STRUCT   MctpBroadcastEn;
  UINT8                              StackIndex;
  UINT8                              PortIndex;
  UINT8                              Bank;
  UINT8                              MaxPortNumberPerSocket;
  UINT8                              MaxIioStackNumber;

  MaxIioStackNumber = GetMaxStackNumPerSocket ();
  for (StackIndex = 0; StackIndex < MaxIioStackNumber; StackIndex++) {
    //
    // Avoid to program bank decoder values if the stack does not include any MS2IOSF device
    //
    if ((IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_NONE) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_RESERVED) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX)) {
      continue;
    }

    MctpBroadcastEn.Data = IioReadCpuCsr32 (IioIndex, StackIndex, MCTP_BROADCAST_EN_IIO_VTD_REG);

    if (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX_IIO) {
      //
      // Enable/Disable MCTP broadcast on PCIe Root Port
      //
      for (Bank = BANK0_DEC; Bank < BANK8_DEC; Bank++) {

        PortIndex = GetPortIndexbyStack (StackIndex, Bank);
        MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];

        if (IioGlobalData->SetupData.MctpEn[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == IIO_OPTION_DISABLE) {
          MctpBroadcastEn.Bits.mctp_bcast_en &= (~(1 << Bank));
        }
      }
    }

    //
    // Changes to ms2iosf0 (stack 0)
    // 1. Disable MCTP on Banks 23 to 26 which are associated with NPK.
    // 2. Disable Bank 2 NPK (ACPI) for SNR and ICX-D.
    //
    if (StackIndex == IIO_STACK0) {
      if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
        MctpBroadcastEn.Bits.mctp_bcast_en &= (~(BIT26 | BIT25 | BIT24 | BIT23));
        if (IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
          MctpBroadcastEn.Bits.mctp_bcast_en &= (~BIT2);
        }
      }
    }

    //
    // HQM WA: Changes to ms2iosf3 (Stack Index IIO_STACK2), MCTP Broadcast Enable register.
    // 1. Disable MCTP on all Banks for HQM (SNR)
    //
    if (IIO_HQM == IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex] ) {
      if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
        if (IsSiliconWorkaroundEnabled ("S1409665287")) {
          MctpBroadcastEn.Bits.mctp_bcast_en = 0; // Disable MCTP for all banks
        }
      }
    }

    IioWriteCpuCsr32 (IioIndex, StackIndex, MCTP_BROADCAST_EN_IIO_VTD_REG, MctpBroadcastEn.Data);
  }

}

/**

  Enable/Disable ENQ commands for selected MS2IOSF decoder banks

  @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param IioIndex           - Index of CPU/IIO

  @retval None

**/
VOID
Ms2iosfEnqCapEnable (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  )
{
  //
  // Not supported
  //
}

/**
  SetBankBarSize             This function programs MS2IOSF bank decoders BarSize for NTB

  @param IioIndex           - index to IIO (socket number)
  @param StackIndex         - Stack index value of CPU/IIO
  @param BankNumber         - Bank number being programmed
  @param Data               - Current data value that will be programmed

  @retval None

**/
VOID
SetBankBarSize (
  UINT8                          IioIndex,
  UINT8                          StackIndex,
  UINT8                          BankNumber,
  UINT8                          Value
  )
{
  BANK13_BARSIZE_IIO_VTD_STRUCT  BankBarSize;
  UINT32                         Csr;

  switch (BankNumber) {
    case BANK13_DEC:
      Csr = BANK13_BARSIZE_IIO_VTD_REG;
      break;
    case BANK14_DEC:
      Csr = BANK14_BARSIZE_IIO_VTD_REG;
      break;
    default:
      IIO_D_ERR ("%a: MS2IOSF bank number %d unsupported\n", __FUNCTION__, BankNumber);
      ASSERT (FALSE);
      return;
    }
  BankBarSize.Data = IioReadCpuCsr8 (IioIndex, StackIndex, Csr);
  BankBarSize.Bits.size = Value;
  IioWriteCpuCsr8 (IioIndex, StackIndex, Csr, BankBarSize.Data);
}


/**

OverrideMs2iosfBarSzForNtb:      This function re-configures BAR size for banks for NTB ports

@param IioGlobalData           - Pointer to IIO Universal Data Structure (UDS)
@param IioIndex                - Index of CPU/IIO

@retval None

**/
VOID
OverrideMs2iosfBarSzForNTB (
  IIO_GLOBALS                    *IioGlobalData,
  UINT8                          IioIndex
  )
{
  UINT8                          StackIndex = 0;
  UINT8                          NtbPort;
  UINT8                          PortIndex;
  UINT8                          MaxNtbPortNumberPerSocket;
  UINT8                          BarSize23 = 0;
  UINT8                          BarSize45 = 0;
  EFI_STATUS                     Status;

  for (PortIndex = 0; PortIndex < GetMaxPciePortNumberPerSocket (IioIndex); PortIndex++) {
    Status = GetNtbPortPerPortIndex (PortIndex, &NtbPort);
    if ((Status != EFI_SUCCESS) || (!CheckNtbPortConfSetting (IioIndex, PortIndex))) {
      continue;
    }

    MaxNtbPortNumberPerSocket = GetMaxNtbNumPerSocket ();
    if (IioGlobalData->SetupData.NtbSplitBar[(IioIndex * MaxNtbPortNumberPerSocket) + NtbPort]) {
     BarSize23 = IioGlobalData->SetupData.NtbBarSizeImBar2_0[(IioIndex * MaxNtbPortNumberPerSocket) + NtbPort];
     BarSize45 = IioGlobalData->SetupData.NtbBarSizeImBar2_1[(IioIndex * MaxNtbPortNumberPerSocket) + NtbPort];
    } else {
      BarSize23 = IioGlobalData->SetupData.NtbBarSizeImBar1[(IioIndex * MaxNtbPortNumberPerSocket) + NtbPort];
      BarSize45 = IioGlobalData->SetupData.NtbBarSizeImBar2[(IioIndex * MaxNtbPortNumberPerSocket) + NtbPort];
    }

    Status = GetPcieStackPerPort (IioIndex, PortIndex, &StackIndex);
    if (Status != EFI_SUCCESS) {
      return;
    }
    SetBankBarSize ( IioIndex, StackIndex, BANK13_DEC, BarSize23);
    SetBankBarSize ( IioIndex, StackIndex, BANK14_DEC, BarSize45);
  }
}

/**

    ProgramMs2IosfBankDecoder: This function programs all MS2IOSF bank decoders
                               based on the StackID, so all the transactions can be
                               routed as required.

    @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
    @param IioIndex           - Index of CPU/IIO

    @retval None

**/
VOID
ProgramMs2IosfBankDecoderBanks (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  )
{
  UINT8       StackIndex;
  UINT8       TableIndex;
  UINT16      TableSize;
  UINT8       BusNumber;
  UINT32      Data;
  UINT8       PortIndex = 0;
  UINT8       CbDmaIndex = 0;
  UINT8       MaxPortNumberPerSocket;
  UINT8       MaxIioStackNumber;
  BANK_DECODER_HEADER *BankDecoderVerPtr = NULL;
  BANK_DECODER_ENTRY  *BankDecoderTablePtr = NULL;

  BankDecoderVerPtr = (BANK_DECODER_HEADER*)&BankDecoderVer;
  BankDecoderTablePtr = (BANK_DECODER_ENTRY*)&BankDecoderTable;
  TableSize = BankDecoderVerPtr->NumberEntries;
  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
  MaxIioStackNumber = GetMaxStackNumPerSocket ();

  IIO_D_LOG ("MS2IOSF BankDecoder: TableSize %d, Recipe Revision %a\n", TableSize, BankDecoderVerPtr->Revision);

  for (TableIndex = 0; TableIndex < TableSize; TableIndex++) {

    for (StackIndex = 0; StackIndex < MaxIioStackNumber; StackIndex++) {
      //
      // Avoid to program bank decoder values if the stack does not include any MS2IOSF device
      //
      if ((IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_NONE) ||
          (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_RESERVED) ||
          (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX)) {
        continue;
      }

      BusNumber = IioGlobalData->IioVar.IioVData.SocketStackBus[IioIndex][StackIndex];

      Data = Ms2iosfRegRead (IioGlobalData, IioIndex, BusNumber,
                             BankDecoderTablePtr[TableIndex].RegisterSize, BankDecoderTablePtr[TableIndex].RegisterAddress);

      IIO_D_DBG ("MS2IOSF BankDecoder: StackID[%x][%x] = %x, Bank = %d\n", IioIndex, StackIndex, IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex], BankDecoderTablePtr[TableIndex].BankNumber);

      if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
        switch (IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex]) {
        case IIO_DMI_CB_SINGLE_PCIE:
          switch (BankDecoderTablePtr[TableIndex].BankNumber) {
          case PORT_0A_BANK:
            if ((!IioDmiIsPortIdentity (IioIndex, PORT_0_INDEX, IioDmiPortActive)) && (IioIndex == 0) &&
                (IioGlobalData->SetupData.PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PORT_0_INDEX] == FALSE)) {
              Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.DmiCbSinglePcie);
            } else {
              Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
            }
            break;
          case PORT_1A_BANK:
          case PORT_1B_BANK:
          case PORT_1C_BANK:
          case PORT_1D_BANK:
            //
            // Single PCIE Gen3 port in stack0 is not pinned out for ICX-SP Redefined,
            // so it shouldn't set any bank for that port.
            //
            Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
            break;
          case CBDMA_CH0_BANK:
          case CBDMA_CH1_BANK:
          case CBDMA_CH2_BANK:
          case CBDMA_CH3_BANK:
          case CBDMA_CH4_BANK:
          case CBDMA_CH5_BANK:
          case CBDMA_CH6_BANK:
          case CBDMA_CH7_BANK:
            CbDmaIndex = BankDecoderTablePtr[TableIndex].BankNumber - CBDMA_CH0_BANK;
            if (IioGlobalData->SetupData.Cb3DmaEn[(IioIndex * IOAT_TOTAL_FUNCS) + CbDmaIndex] == TRUE) {
              Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.DmiCbSinglePcie);
            } else {
              Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
            }
            break;
          case NTB_1A_BANK_1:
          case NTB_1A_BANK_2:
          case NTB_1A_BANK_3:
            //
            // Single PCIE Gen3 port in stack0 is not pinned out for ICX-SP Redefined,
            // so it shouldn't set any bank for that port.
            //
            Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
            break;
          case MSM_BANK:
          case MSM_PMON_BANK_1:
          case MSM_PMON_BANK_2:
          case MSM_DFX_BANK:
          case NPK_1_BANK:
          case NPK_2_BANK:
          case NPK_3_BANK:
          case NPK_FWBAR_BANK:
            Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.DmiCbSinglePcie);
            break;
          case DMI_BANK:
            if (!(IsSiliconWorkaroundEnabled ("S1208984927") && ((BankDecoderTablePtr[TableIndex].RegisterAddress & OFFSET_MASK) == DESTID_OFFSET))) {
              if (IioDmiIsPortIdentity (IioIndex, PORT_0_INDEX, IioDmiPortActive)) {
                Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.DmiCbSinglePcie);
              } else {
                Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
              }
            }
            break;
          default:
            Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
            break;
          }
          break;
        case IIO_SINGLE_PCIE_0:
        case IIO_SINGLE_PCIE_1:
        case IIO_SINGLE_PCIE_2:
        case IIO_SINGLE_PCIE_3:
        case IIO_SINGLE_PCIE_4:
          switch (BankDecoderTablePtr[TableIndex].BankNumber) {
          case PORT_0A_BANK:
          case PORT_0B_BANK:
          case PORT_0C_BANK:
          case PORT_0D_BANK:
            PortIndex = GetPortIndexbyStack (StackIndex, BankDecoderTablePtr[TableIndex].BankNumber);
            if ((IioGlobalData->IioVar.IioOutData.PciePortOwnership[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == REGULAR_PCIE_OWNERSHIP) &&
                (IioGlobalData->SetupData.PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == FALSE)) {
              Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.SinglePcieGen4);
            } else {
              Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
            }
            if (IsSiliconWorkaroundEnabled ("S1406760259")) {
              if ((CheckNtbPortConfSetting (IioIndex, PortIndex) == TRUE) &&
                  (BankDecoderTablePtr[TableIndex].RegisterAddress & OFFSET_MASK) == BANKCTRL_OFFSET) {
                Data |= MC_BANK_DECODE_ENABLE | MC_SHADOW_ENABLE;
              }
            }
            break;
          case PORT_0A_BAR_BANK:
          case PORT_0B_BAR_BANK:
          case PORT_0C_BAR_BANK:
          case PORT_0D_BAR_BANK:
            if (!IsSiliconWorkaroundEnabled ("S1406647183")) {
              PortIndex = GetPortIndexbyStack (StackIndex, (BankDecoderTablePtr[TableIndex].BankNumber - PORT_0A_BAR_BANK));
              if ((IioGlobalData->IioVar.IioOutData.PciePortOwnership[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == REGULAR_PCIE_OWNERSHIP) &&
                  (IioGlobalData->SetupData.PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == FALSE)) {
                Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.SinglePcieGen4);
              } else {
                Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
              }
            } else {
              Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
            }
            break;
          case NTB_0A_BANK_1:
          case NTB_0A_BANK_2:
          case NTB_0A_BANK_3:
            PortIndex = GetPortIndexbyStack (StackIndex, IioPortA);
            if (CheckNtbPortConfSetting (IioIndex, PortIndex) == TRUE) {
              Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.SinglePcieGen4);
            } else {
              Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
            }
            break;
          default:
            Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
            break;
          }
          break;
        default:
          Data = 0;
          break;
        }
      } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
        // Update the data with the values from the bank decoder table
        switch (IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex]) {
          case IIO_CB_RLINK:
            switch (BankDecoderTablePtr[TableIndex].BankNumber) {
              case CBDMA_CH0_BANK:
              case CBDMA_CH1_BANK:
              case CBDMA_CH2_BANK:
              case CBDMA_CH3_BANK:
              case CBDMA_CH4_BANK:
              case CBDMA_CH5_BANK:
              case CBDMA_CH6_BANK:
              case CBDMA_CH7_BANK:
                CbDmaIndex = BankDecoderTablePtr[TableIndex].BankNumber - CBDMA_CH0_BANK;
                if (IioGlobalData->SetupData.Cb3DmaEn[(IioIndex * IOAT_TOTAL_FUNCS) + CbDmaIndex] == TRUE) {
                  Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.CbRlink);
                } else {
                  Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
                }
                break;
              case NPK_ACPI_BANK:
              case MSM_BANK:
              case MSM_PMON_BANK_1:
              case MSM_PMON_BANK_2:
              case MSM_DFX_BANK:
              case NPK_1_BANK:
              case NPK_2_BANK:
              case NPK_3_BANK:
              case NPK_FWBAR_BANK:
                Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.CbRlink);
                break;
              case RLINK_BANK:
                if (!(IsSiliconWorkaroundEnabled ("S1208984927") && ((BankDecoderTablePtr[TableIndex].RegisterAddress & OFFSET_MASK) == DESTID_OFFSET))) {
                  if (IioDmiIsPortIdentity (IioIndex, PORT_0_INDEX, IioDmiPortActive)) {
                    Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.CbRlink);
                  } else {
                    Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
                  }
                }
                break;
              default:
                Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
                break;
            }
            break;
          case IIO_SINGLE_PCIE_0:
            switch (BankDecoderTablePtr[TableIndex].BankNumber) {
              case PORT_0A_BANK:
              case PORT_0B_BANK:
              case PORT_0C_BANK:
              case PORT_0D_BANK:
                PortIndex = BankDecoderTablePtr[TableIndex].BankNumber - PORT_0A_BANK + PORT_1A_INDEX_S1;
                if ((IioGlobalData->IioVar.IioOutData.PciePortOwnership[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == REGULAR_PCIE_OWNERSHIP) &&
                    (IioGlobalData->SetupData.PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == FALSE)) {
                  Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.FlippedSinglePcieGen4);
                } else {
                  Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
                }
                if (IsSiliconWorkaroundEnabled("S1406760259")) {
                  if ((CheckNtbPortConfSetting(IioIndex, PortIndex) == TRUE) &&
                    (BankDecoderTablePtr[TableIndex].RegisterAddress & OFFSET_MASK) == BANKCTRL_OFFSET) {
                    Data |= MC_BANK_DECODE_ENABLE | MC_SHADOW_ENABLE;
                  }
                }
                break;
              case PORT_0A_BAR_BANK:
              case PORT_0B_BAR_BANK:
              case PORT_0C_BAR_BANK:
              case PORT_0D_BAR_BANK:
                if (!IsSiliconWorkaroundEnabled("S1406647183")) {
                  PortIndex = GetPortIndexbyStack (StackIndex, (BankDecoderTablePtr[TableIndex].BankNumber - PORT_0A_BAR_BANK));
                  if ((IioGlobalData->IioVar.IioOutData.PciePortOwnership[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == REGULAR_PCIE_OWNERSHIP) &&
                    (IioGlobalData->SetupData.PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == FALSE)) {
                    Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.FlippedSinglePcieGen4);
                  } else {
                    Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
                  }
                } else {
                  Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
                }
                break;
              case NTB_0A_BANK_1:
              case NTB_0A_BANK_2:
              case NTB_0A_BANK_3:
                PortIndex = GetPortIndexbyStack (StackIndex, IioPortA);
                if (CheckNtbPortConfSetting(IioIndex, PortIndex) == TRUE) {
                  Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.FlippedSinglePcieGen4);
                } else {
                  Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
                }
                break;
            }
            break;
          case IIO_CPM_NAC_ICXD:
            switch (BankDecoderTablePtr[TableIndex].BankNumber) {
              case CPM_VRP_BANK:
                if (IioIsHccSku (IioIndex)) {
                  Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.CpmNac);
                } else {
                  Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
                }
                break;
              case SPMC_BANK:
              case CPK_VRP_BANK:
                Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.CpmNac);
                // When CPK NAC (PSFCPK) MS2IOSF instance is disabled set Bank_Decode_Enable bit
                if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << IIO_CPK_NAC_ICXD)) &&
                    ((BankDecoderTablePtr[TableIndex].RegisterAddress & OFFSET_MASK) == BANKCTRL_OFFSET)) {
                  Data |= BIT3;
                }
                break;
              default :
                Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
            }
            break;
          case IIO_CPK_NAC_ICXD:
            if (BankDecoderTablePtr[TableIndex].BankNumber == CPK_VRP_BANK) {
            Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.CpkNac);
            } else {
            Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
            }
            break;
          case IIO_SINGLE_PCIE_4:
            switch (BankDecoderTablePtr[TableIndex].BankNumber) {
              case PORT_0A_BANK:
              case PORT_0B_BANK:
              case PORT_0C_BANK:
              case PORT_0D_BANK:
                PortIndex = GetPortIndexbyStack(StackIndex, BankDecoderTablePtr[TableIndex].BankNumber);
                if ((IioGlobalData->IioVar.IioOutData.PciePortOwnership[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == REGULAR_PCIE_OWNERSHIP) &&
                  (IioGlobalData->SetupData.PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == FALSE)) {
                  Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.SinglePcieGen4);
                } else {
                  Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
                }
                if (IsSiliconWorkaroundEnabled("S1406760259")) {
                  if ((CheckNtbPortConfSetting(IioIndex, PortIndex) == TRUE) &&
                    (BankDecoderTablePtr[TableIndex].RegisterAddress & OFFSET_MASK) == BANKCTRL_OFFSET) {
                    Data |= MC_BANK_DECODE_ENABLE | MC_SHADOW_ENABLE;
                  }
                }
                break;
              case PORT_0A_BAR_BANK:
              case PORT_0B_BAR_BANK:
              case PORT_0C_BAR_BANK:
              case PORT_0D_BAR_BANK:
                if (!IsSiliconWorkaroundEnabled("S1406647183")) {
                  PortIndex = GetPortIndexbyStack(StackIndex, (BankDecoderTablePtr[TableIndex].BankNumber - PORT_0A_BAR_BANK));
                  if ((IioGlobalData->IioVar.IioOutData.PciePortOwnership[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == REGULAR_PCIE_OWNERSHIP) &&
                    (IioGlobalData->SetupData.PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == FALSE)) {
                    Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.SinglePcieGen4);
                  } else {
                    Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
                  }
                } else {
                  Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
                }
                break;
              case NTB_0A_BANK_1:
              case NTB_0A_BANK_2:
              case NTB_0A_BANK_3:
                PortIndex = GetPortIndexbyStack(StackIndex, IioPortA);
                if (CheckNtbPortConfSetting(IioIndex, PortIndex) == TRUE) {
                  Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.SinglePcieGen4);
                }
                else {
                  Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
                }
                break;
              default:
                Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
                break;
            }
            break;
          default:
            Data = 0;
            break;
        }
      } else if (IsCpuAndRevision (CPU_TNR, REV_ALL)) {
        switch (IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex]) {
          case IIO_CB_RLINK_NAC:
            switch (BankDecoderTablePtr[TableIndex].BankNumber) {
            case CBDMA_CH0_BANK:
            case CBDMA_CH1_BANK:
            case CBDMA_CH2_BANK:
            case CBDMA_CH3_BANK:
            case CBDMA_CH4_BANK:
            case CBDMA_CH5_BANK:
            case CBDMA_CH6_BANK:
            case CBDMA_CH7_BANK:
              CbDmaIndex = BankDecoderTablePtr[TableIndex].BankNumber - CBDMA_CH0_BANK;
              if (IioGlobalData->SetupData.Cb3DmaEn[(IioIndex * IOAT_TOTAL_FUNCS) + CbDmaIndex] == TRUE) {
                Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.CbRlinkNac);
              } else {
                Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
              }
              break;
            case NPK_ACPI_BANK:
            case MSM_BANK:
            case MSM_PMON_BANK_1:
            case MSM_PMON_BANK_2:
            case MSM_DFX_BANK:
            case NPK_1_BANK:
            case NPK_2_BANK:
            case NPK_3_BANK:
            case NPK_FWBAR_BANK:
              Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.CbRlinkNac);
              break;
            case RLINK_BANK:
              if (!(IsSiliconWorkaroundEnabled ("S1208984927") && ((BankDecoderTablePtr[TableIndex].RegisterAddress & OFFSET_MASK) == DESTID_OFFSET))) {
                if (IioDmiIsPortIdentity (IioIndex, PORT_0_INDEX, IioDmiPortActive)) {
                  Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.CbRlinkNac);
                } else {
                  Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
                }
              }
              break;
            case CPK_VRP_BANK:
              Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.CbRlinkNac);
              break;
            default:
              Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
              break;
            }
            break;
        default:
          Data = 0;
          break;
        }
      } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
        switch (IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex]) {
        case IIO_CB_RLINK:
          switch (BankDecoderTablePtr[TableIndex].BankNumber) {
          case CBDMA_CH0_BANK:
          case CBDMA_CH1_BANK:
          case CBDMA_CH2_BANK:
          case CBDMA_CH3_BANK:
          case CBDMA_CH4_BANK:
          case CBDMA_CH5_BANK:
          case CBDMA_CH6_BANK:
          case CBDMA_CH7_BANK:
            CbDmaIndex = BankDecoderTablePtr[TableIndex].BankNumber - CBDMA_CH0_BANK;
            if (IioGlobalData->SetupData.Cb3DmaEn[(IioIndex * IOAT_TOTAL_FUNCS) + CbDmaIndex] == TRUE) {
              Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.CbRlink);
            } else {
              Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
            }
            break;
          case NPK_ACPI_BANK:
          case MSM_BANK:
          case MSM_PMON_BANK_1:
          case MSM_PMON_BANK_2:
          case MSM_DFX_BANK:
          case NPK_1_BANK:
          case NPK_2_BANK:
          case NPK_3_BANK:
          case NPK_FWBAR_BANK:
            Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.CbRlink);
            break;
          case RLINK_BANK:
            if (!(IsSiliconWorkaroundEnabled ("S1208984927") && ((BankDecoderTablePtr[TableIndex].RegisterAddress & OFFSET_MASK) == DESTID_OFFSET))) {
              if (IioDmiIsPortIdentity (IioIndex, PORT_0_INDEX, IioDmiPortActive)) {
                Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.CbRlink);
              } else {
                Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
              }
            }
            break;
          default:
            Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
            break;
          }
          break;
        case IIO_SINGLE_PCIE_0:
          switch (BankDecoderTablePtr[TableIndex].BankNumber) {
          case PORT_0A_BANK:
          case PORT_0B_BANK:
          case PORT_0C_BANK:
          case PORT_0D_BANK:
            PortIndex = BankDecoderTablePtr[TableIndex].BankNumber - PORT_0A_BANK + PORT_1A_INDEX_S1;
            if ((IioGlobalData->IioVar.IioOutData.PciePortOwnership[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == REGULAR_PCIE_OWNERSHIP) &&
                (IioGlobalData->SetupData.PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == FALSE)) {
              Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.SinglePcieGen3);
            } else {
              Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
            }

            if (IsSiliconWorkaroundEnabled ("S1406760259")) {
              if ((CheckNtbPortConfSetting (IioIndex, PortIndex) == TRUE) &&
                  (BankDecoderTablePtr[TableIndex].RegisterAddress & OFFSET_MASK) == BANKCTRL_OFFSET) {
                Data |= MC_BANK_DECODE_ENABLE | MC_SHADOW_ENABLE;
              }
            }
            break;
          case NTB_0A_BANK_1:
          case NTB_0A_BANK_2:
          case NTB_0A_BANK_3:
            if (CheckNtbPortConfSetting (IioIndex, PORT_1A_INDEX_S1) == TRUE) {
              Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.SinglePcieGen3);
            } else {
              Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
            }
            break;
          default:
            Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
            break;
          }
          break;
        case IIO_HQM:
          if (BankDecoderTablePtr[TableIndex].BankNumber == HQM_SRIOV_BANK) {
            Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Hqm);
          } else {
            Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
          }
          break;
        case IIO_CPK_NAC:
          if (BankDecoderTablePtr[TableIndex].BankNumber == CPK_VRP_BANK ) {
            Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.FlippedCpkNac);
          } else {
            Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
          }
          break;
        case IIO_CPM_NAC:
          switch (BankDecoderTablePtr[TableIndex].BankNumber) {
          case CPM_VRP_BANK:
          case SPMC_BANK:
            Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.CpmNac);
            break;
          case CPK_VRP_BANK:
            Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.CpmNac);
            // When CPK NAC (PSFCPK) MS2IOSF instance is disabled set Bank_Decode_Enable bit
            if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << IIO_CPK_NAC)) &&
                ((BankDecoderTablePtr[TableIndex].RegisterAddress & OFFSET_MASK) == BANKCTRL_OFFSET)) {
              Data |= BIT3;
            }
            break;
          default:
            Data = ((Data & BankDecoderTablePtr[TableIndex].FieldMask) | BankDecoderTablePtr[TableIndex].StackTypeData.Disable);
          }
          break;

        default:
          Data = 0;
          break;
        }
      }
      IIO_D_DBG ("MS2IOSF BankDecoder: S:%x B:%x D:%x F:%x Off:0x%x = 0x%x\n", IioIndex, BusNumber, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, BankDecoderTablePtr[TableIndex].RegisterAddress, Data);

      if (!(IsSiliconWorkaroundEnabled ("S1208984927") && ((BankDecoderTablePtr[TableIndex].RegisterAddress & OFFSET_MASK) == DESTID_OFFSET) &&
            (BankDecoderTablePtr[TableIndex].BankNumber == BANK27_DEC))) {

        DisableMs2IosfType1BankDecoder (IioIndex, StackIndex, BusNumber, BankDecoderTablePtr[TableIndex].BankNumber,
                                        BankDecoderTablePtr[TableIndex].RegisterAddress, &Data);
        Ms2iosfRegWrite (IioGlobalData, IioIndex, BusNumber,
                         BankDecoderTablePtr[TableIndex].RegisterSize, BankDecoderTablePtr[TableIndex].RegisterAddress, Data);
      }
    }
  }
}

/**

    ProgramItcCredits: This function programs all MS2IOSF ITC credits
                       based on the StackID.

    @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
    @param IioIndex           - Index of CPU/IIO

    @retval None

**/
VOID
ProgramItcCredits (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  )
{
  UINT8       StackIndex;
  UINT8       PortIndex = 0;
  UINT8       TableIndex;
  UINT16      TableSize;
  UINT8       BusNumber;
  UINT32      Data = 0;
  UINT8       MaxIioStackNumber;
  CREDITS_HEADER *ItcCreditsVerPtr = NULL;
  CREDITS_ENTRY  *ItcCreditsTablePtr = NULL;
  UINT8       Bifurcation = 0;

  ItcCreditsVerPtr = (CREDITS_HEADER*) &ItcCreditsVer;
  ItcCreditsTablePtr = (CREDITS_ENTRY*) &ItcCreditDefaultTable;


  TableSize = ItcCreditsVerPtr->NumberEntries;
  MaxIioStackNumber = GetMaxStackNumPerSocket ();

  IIO_D_LOG ("MS2IOSF Credits ITC: TableSize %d, Recipe Revision %a\n", TableSize, ItcCreditsVerPtr->Revision);

  for (StackIndex = 0; StackIndex < MaxIioStackNumber; StackIndex++) {
    // Avoid to program credits values if the stack does not include any MS2IOSF device
    if ((IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_NONE) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_RESERVED) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX)) {
      continue;
    }

    if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
      switch (IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex]) {
      case IIO_DMI_CB_SINGLE_PCIE:
        ItcCreditsTablePtr = (CREDITS_ENTRY*) &ItcCreditDmiTable;
        break;
      case IIO_SINGLE_PCIE_0:
      case IIO_SINGLE_PCIE_1:
      case IIO_SINGLE_PCIE_2:
      case IIO_SINGLE_PCIE_3:
      case IIO_SINGLE_PCIE_4:
        PortIndex = GetPortIndexbyStack (StackIndex, IioPortA);
        Bifurcation = CheckBifurcationMapPort (IioGlobalData, IioIndex, PortIndex);
        ItcCreditsTablePtr = GetItcCreditSinglePcieTable (Bifurcation, IIO_GEN4);
        break;
      default:
        Data = 0;
        break;
      }
    } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
      switch (IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex]) {
      case IIO_CB_RLINK:
        //Skip credit initialization for Rlink
        continue;
      case IIO_SINGLE_PCIE_0:
        Bifurcation = IioGlobalData->SetupData.ConfigIOU[IioIndex][0];
        //
        // This PCIE Gen4 is in a flipped configuration where its in M2IOSF port1 instead of port0.
        // Use Gen3 ITC credits since Gen4 IP is now on M2IOSF port1 which matches Gen3 value settings.
        //
        ItcCreditsTablePtr = GetItcCreditSinglePcieTable (Bifurcation, IIO_GEN3);
        break;
      case IIO_CPM_NAC_ICXD:
        ItcCreditsTablePtr = (CREDITS_ENTRY*) &ItcCreditSinglePcie3x16Table;
        break;
      case IIO_CPK_NAC_ICXD:
        //
        // This NAC is in a flipped configuration where its in M2IOSF port0 instead of port1.
        // Use same ITC credits values as Gen4 x16 which matches the new NAC value settings.
        //
        Bifurcation = IIO_BIFURCATE_xxxxxx16;
        ItcCreditsTablePtr = GetItcCreditSinglePcieTable (Bifurcation, IIO_GEN4);
        break;
      case IIO_SINGLE_PCIE_4:
        //
        // This PCIE Gen4 RP(Iou1) is second PCIE stack in ICXD and present at Ms2iosf Stack5
        // Bifurcation knob this PCIE is ConfigIOU1[0], i.e, ConfigIOU[IioIndex][1]
        //
        Bifurcation = IioGlobalData->SetupData.ConfigIOU[IioIndex][1];
        ItcCreditsTablePtr = GetItcCreditSinglePcieTable (Bifurcation, IIO_GEN4);
        break;
      default:
        ItcCreditsTablePtr = (CREDITS_ENTRY*) &ItcCreditDefaultTable;
        break;
      }
    } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
      switch (IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex]) {
      case IIO_CB_RLINK:
        //Skip credit initialization for Rlink
        continue;
      case IIO_SINGLE_PCIE_0:
        Bifurcation = IioGlobalData->SetupData.ConfigIOU[IioIndex][0];
        ItcCreditsTablePtr = GetItcCreditSinglePcieTable (Bifurcation, IIO_GEN3);
        break;
      case IIO_CPK_NAC:
        // Use same credit table values as NAC PSF0 to reflect SNR NAC PSFCPK on MS2IOSF port1
        ItcCreditsTablePtr = (CREDITS_ENTRY*) &ItcCreditNacPsf0Table;
        break;
      case IIO_CPM_NAC:
        ItcCreditsTablePtr = (CREDITS_ENTRY*) &ItcCreditNacPsf0Table;
        break;
      case IIO_HQM:
        ItcCreditsTablePtr = (CREDITS_ENTRY*) &ItcCreditSinglePcie3x16Table;
        break;
      default:
        ItcCreditsTablePtr = (CREDITS_ENTRY*) &ItcCreditDefaultTable;
        break;
      }
    }

    BusNumber = IioGlobalData->IioVar.IioVData.SocketStackBus[IioIndex][StackIndex];

    for (TableIndex = 0; TableIndex < TableSize; TableIndex++) {
      Data = Ms2iosfRegRead (IioGlobalData, IioIndex, BusNumber,
                             ItcCreditsTablePtr[TableIndex].RegisterSize, ItcCreditsTablePtr[TableIndex].RegisterAddress);
      Data = ((Data & ItcCreditsTablePtr[TableIndex].FieldMask) | ItcCreditsTablePtr[TableIndex].Value);
      IIO_D_DBG ("MS2IOSF ITC Credit: StackID[%x][%x] = %x, Register = %d, Data = 0x%x\n", IioIndex, StackIndex, IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex], ItcCreditsTablePtr[TableIndex].RegisterAddress, Data);

      Ms2iosfRegWrite (IioGlobalData, IioIndex, BusNumber,
                       ItcCreditsTablePtr[TableIndex].RegisterSize, ItcCreditsTablePtr[TableIndex].RegisterAddress, Data);
    }
  }
}

/**

    ProgramOtcCredits: This function programs all MS2IOSF OTC credits
                       based on the StackID.

    @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
    @param IioIndex           - Index of CPU/IIO

    @retval None

**/
VOID
ProgramOtcCredits (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  )
{
  UINT8       StackIndex;
  UINT8       PortIndex = 0;
  UINT8       TableIndex;
  UINT16      TableSize;
  UINT8       BusNumber;
  UINT32      Data = 0;
  UINT8       MaxIioStackNumber;
  CREDITS_HEADER *OtcCreditsVerPtr = NULL;
  CREDITS_ENTRY  *OtcCreditsTablePtr = NULL;
  UINT8       Bifurcation = 0;

  OtcCreditsVerPtr = (CREDITS_HEADER*) &OtcCreditsVer;
  OtcCreditsTablePtr = (CREDITS_ENTRY*) &OtcCreditDefaultTable;

  TableSize = OtcCreditsVerPtr->NumberEntries;
  MaxIioStackNumber = GetMaxStackNumPerSocket ();

  IIO_D_LOG ("MS2IOSF Credits OTC: TableSize %d, Recipe Revision %a\n", TableSize, OtcCreditsVerPtr->Revision);

  for (StackIndex = 0; StackIndex < MaxIioStackNumber; StackIndex++) {
    // Avoid to program credits values if the stack does not include any MS2IOSF device
    if ((IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_NONE) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_RESERVED) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX)) {
      continue;
    }

    if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
      switch (IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex]) {
      case IIO_DMI_CB_SINGLE_PCIE:
        OtcCreditsTablePtr = (CREDITS_ENTRY*) &OtcCreditDmiTable;
        break;
      case IIO_SINGLE_PCIE_0:
      case IIO_SINGLE_PCIE_1:
      case IIO_SINGLE_PCIE_2:
      case IIO_SINGLE_PCIE_3:
      case IIO_SINGLE_PCIE_4:
        PortIndex = GetPortIndexbyStack (StackIndex, IioPortA);
        Bifurcation = CheckBifurcationMapPort (IioGlobalData, IioIndex, PortIndex);
        OtcCreditsTablePtr = GetOtcCreditSinglePcieTable (Bifurcation, IIO_GEN4);
        break;
      default:
        Data = 0;
        break;
      }
    } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
      switch (IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex]) {
      case IIO_CB_RLINK:
        //Skip credit initialization for Rlink
        continue;
      case IIO_SINGLE_PCIE_0:
        Bifurcation = IioGlobalData->SetupData.ConfigIOU[IioIndex][0];
        //
        // This PCIE Gen4 is in a flipped configuration where its in M2IOSF port1 instead of port0.
        // Use Gen3 OTC credits since Gen4 IP is now on M2IOSF port1 which matches Gen3 value settings.
        //
        OtcCreditsTablePtr = GetOtcCreditSinglePcieTable (Bifurcation, IIO_GEN3);
        break;
      case IIO_CPM_NAC_ICXD:
        OtcCreditsTablePtr = (CREDITS_ENTRY*) &OtcCreditSinglePcie3x16Table;
        break;
      case IIO_CPK_NAC_ICXD:
        //
        // This NAC is in a flipped configuration where its in M2IOSF port0 instead of port1.
        // Use same OTC credits values as Gen4 x16 which matches the new NAC value settings.
        //
        Bifurcation = IIO_BIFURCATE_xxxxxx16;
        OtcCreditsTablePtr = GetOtcCreditSinglePcieTable (Bifurcation, IIO_GEN4);
        break;
      case IIO_SINGLE_PCIE_4:
        //
        // This PCIE Gen4 RP(Iou1) is second PCIE stack in ICXD and present at Ms2iosf Stack5
        // Bifurcation knob this PCIE is ConfigIOU1[0], i.e, ConfigIOU[IioIndex][1]
        //
        Bifurcation = IioGlobalData->SetupData.ConfigIOU[IioIndex][1];
        OtcCreditsTablePtr = GetOtcCreditSinglePcieTable (Bifurcation, IIO_GEN4);
        break;
      default:
        OtcCreditsTablePtr = (CREDITS_ENTRY*) &OtcCreditDefaultTable;
        break;
      }
    } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
      switch (IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex]) {
      case IIO_CB_RLINK:
        //Skip credit initialization for Rlink
        continue;
      case IIO_SINGLE_PCIE_0:
        Bifurcation = IioGlobalData->SetupData.ConfigIOU[IioIndex][0];
        OtcCreditsTablePtr = GetOtcCreditSinglePcieTable (Bifurcation, IIO_GEN3);
        break;
      case IIO_CPK_NAC:
        OtcCreditsTablePtr = (CREDITS_ENTRY*) &OtcCreditNacPsfCpkTable;
        break;
      case IIO_CPM_NAC:
        OtcCreditsTablePtr = (CREDITS_ENTRY*) &OtcCreditNacPsf0Table;
        break;
      case IIO_HQM:
        OtcCreditsTablePtr = (CREDITS_ENTRY*) &OtcCreditSinglePcie3x16Table;
        break;
      default:
        OtcCreditsTablePtr = (CREDITS_ENTRY*) &OtcCreditDefaultTable;
        break;
      }
    }

    BusNumber = IioGlobalData->IioVar.IioVData.SocketStackBus[IioIndex][StackIndex];

    for (TableIndex = 0; TableIndex < TableSize; TableIndex++) {

      Data = Ms2iosfRegRead (IioGlobalData, IioIndex, BusNumber,
                             OtcCreditsTablePtr[TableIndex].RegisterSize, OtcCreditsTablePtr[TableIndex].RegisterAddress);
      Data = ((Data & OtcCreditsTablePtr[TableIndex].FieldMask) | OtcCreditsTablePtr[TableIndex].Value);
      IIO_D_DBG ("MS2IOSF OTC Credit: StackID[%x][%x] = %x, Register = %d, Data = 0x%x\n", IioIndex, StackIndex, IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex], OtcCreditsTablePtr[TableIndex].RegisterAddress, Data);

      Ms2iosfRegWrite (IioGlobalData, IioIndex, BusNumber,
                       OtcCreditsTablePtr[TableIndex].RegisterSize, OtcCreditsTablePtr[TableIndex].RegisterAddress, Data);
    }
  }
}


/**

    Program and override all MS2IOSF related registers needed before Hardware init trigger

    @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
    @param IioIndex           - Index of CPU/IIO

    @retval None

**/
VOID
ProgramMs2IosfCreditsMisc (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  )
{
  OTCCTRL23_N0_IIO_VTD_STRUCT               OtcCtrl23N0;
  OTCPOOLCRDTS_N0_IIO_VTD_STRUCT            OtcPoolCrdtsN0;
  OTCMAXTOTCRDTS0_N0_IIO_VTD_STRUCT         OtcMaxTotCrdts0N0;
  OTCMINTOTCRDTS0_N0_IIO_VTD_STRUCT         OtcMinTotCrdts0N0;
  TC_CTRL0_IIO_DFX_VTD_STRUCT               TcCtrl0;
  IRP_MISC_DFX1_IIO_DFX_VTD_STRUCT          IrpMiscDfx1;
  UINT8                                     StackIndex;
  UINT8                                     MaxIioStackNumber;
  UINT8                                     ChannelIndex;
  UINT8                                     *Channels = NULL;
  IOSF_REQ_CREDITS_CH_00_01_IIO_VTD_STRUCT  IosfReqCred;
  UINT32                                    IosfReqCredCh[] = {IOSF_REQ_CREDITS_CH_00_01_IIO_VTD_REG,
                                                               IOSF_REQ_CREDITS_CH_02_03_IIO_VTD_REG,
                                                               IOSF_REQ_CREDITS_CH_04_05_IIO_VTD_REG,
                                                               IOSF_REQ_CREDITS_CH_06_07_IIO_VTD_REG};

  MaxIioStackNumber = GetMaxStackNumPerSocket ();
  for (StackIndex = 0; StackIndex < MaxIioStackNumber; StackIndex++) {
    // Avoid programming if the stack does not include any MS2IOSF devices
    if ((IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_NONE) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_RESERVED) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX)) {
      continue;
    }
    //
    // Program performance register before hw_init is toggled.
    //
    if (IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
      IrpMiscDfx1.Data = IioReadCpuCsr32 (IioIndex, StackIndex, IRP_MISC_DFX1_IIO_DFX_VTD_REG);
      IrpMiscDfx1.Data |= BIT30; // Set wb_arb_pri_ovr_pf_pri_disable bit30
      IioWriteCpuCsr32 (IioIndex, StackIndex, IRP_MISC_DFX1_IIO_DFX_VTD_REG, IrpMiscDfx1.Data);
    }

    //
    // When VMD is enabled, OTC credit counter for IRP NP credits can underflow in buffer full cases and cause a hang.
    // Apply WA for ICX-SP and ICX-D A0.
    //
    if (IsSiliconWorkaroundEnabled ("S1406892652")) {
      if (IsVMDEnabledForStack (IioIndex, StackIndex)) {
        OtcCtrl23N0.Data = IioReadCpuCsr32 (IioIndex, StackIndex, OTCCTRL23_N0_IIO_VTD_REG);
        OtcCtrl23N0.Bits.dis_np_pool_borrow = 1;
        IioWriteCpuCsr32 (IioIndex, StackIndex, OTCCTRL23_N0_IIO_VTD_REG, OtcCtrl23N0.Data);

        OtcPoolCrdtsN0.Data = IioReadCpuCsr32 (IioIndex, StackIndex, OTCPOOLCRDTS_N0_IIO_VTD_REG);
        //
        // Set NP min and max pool credits
        //
        OtcPoolCrdtsN0.Bits.pool_np_min = 0x9;
        OtcPoolCrdtsN0.Bits.pool_np_max = 0xa;
        IioWriteCpuCsr32 (IioIndex, StackIndex, OTCPOOLCRDTS_N0_IIO_VTD_REG, OtcPoolCrdtsN0.Data);
      }
    }

    //
    // A bad transaction type is received from M2IOSF after buffer overflow.
    // Apply silicon WA for ICX-SP, SNR ICX-D A0.
    //
    if (IsSiliconWorkaroundEnabled ("S1706839317") &&
        (IioGlobalData->SetupData.DfxBadTransactionTypeWa == IIO_OPTION_AUTO ||
         IioGlobalData->SetupData.DfxBadTransactionTypeWa == IIO_OPTION_ENABLE)) {

      //
      // Set OTC P/NP max total credits
      //
      OtcMaxTotCrdts0N0.Data = IioReadCpuCsr32 (IioIndex, StackIndex, OTCMAXTOTCRDTS0_N0_IIO_VTD_REG);
      OtcMaxTotCrdts0N0.Bits.ch00_np_max = 2;
      OtcMaxTotCrdts0N0.Bits.ch00_p_max = 2;
      IioWriteCpuCsr32 (IioIndex, StackIndex, OTCMAXTOTCRDTS0_N0_IIO_VTD_REG, OtcMaxTotCrdts0N0.Data);

      // Set OTC P/NP min total credits
      OtcMinTotCrdts0N0.Data = IioReadCpuCsr32 (IioIndex, StackIndex, OTCMINTOTCRDTS0_N0_IIO_VTD_REG);
      OtcMinTotCrdts0N0.Bits.ch00_np_min = 2;
      OtcMinTotCrdts0N0.Bits.ch00_p_min = 2;
      IioWriteCpuCsr32 (IioIndex, StackIndex, OTCMINTOTCRDTS0_N0_IIO_VTD_REG, OtcMinTotCrdts0N0.Data);

      // Set buffer credit count
      TcCtrl0.Data = IioReadCpuCsr32 (IioIndex, StackIndex, TC_CTRL0_IIO_DFX_VTD_REG);
      TcCtrl0.Bits.itc_skid_buffer_crdt_cnt = 8;
      IioWriteCpuCsr32 (IioIndex, StackIndex, TC_CTRL0_IIO_DFX_VTD_REG, TcCtrl0.Data);
    }

    if ((IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL)) && IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_NAC) {
      Channels = GetMs2iosfChannels (StackIndex);
      if (Channels == NULL) {
        IIO_D_ERR ("[%d.%d] Ms2Iosf Channel list is NULL\n",IioIndex, StackIndex);
        ASSERT (Channels != NULL);
        continue;
      }
      for (ChannelIndex = 0; ChannelIndex < MAX_MS2IOSF_CHANNEL_PER_STACK / 2; ChannelIndex++) {
        IosfReqCred.Data = IioReadCpuCsr32 (IioIndex, StackIndex, IosfReqCredCh[ChannelIndex]);

        if (((Channels[0] == Ch0) && (IosfReqCredCh[ChannelIndex] == IOSF_REQ_CREDITS_CH_00_01_IIO_VTD_REG)) ||
            ((Channels[0] == Ch4) && (IosfReqCredCh[ChannelIndex] == IOSF_REQ_CREDITS_CH_04_05_IIO_VTD_REG))) {
          //
          // Keep first channel to defaults for NAC devices
          //
          IosfReqCred.Data &= 0xC0007FFF; // Bits[31:30] are reserved
        } else {
          IosfReqCred.Data &= 0xC0000000; // Bits[31:30] are reserved
        }
        IioWriteCpuCsr32 (IioIndex, StackIndex, IosfReqCredCh[ChannelIndex], IosfReqCred.Data);
      }
    }
  }
}

/**

    Trigger hw_init for credit programming to take effect.

    @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
    @param IioIndex           - Index of CPU/IIO

    @retval None

**/
VOID
TriggerCreditsHwInit (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  )
{
  UINT8                          StackIndex;
  UINT8                          MaxIioStackNumber;
  EFI_STATUS                     Status;
  UINT8                          WaitTime;
  UINT8                          MeshId10nmSoc[5] = {59, 63, 62, 60, 61};
  TC_MISC_UCR_SB_M2PCIE_STRUCT   TcMiscUcr;

  MaxIioStackNumber = GetMaxStackNumPerSocket ();
  //
  // Skip HW_INIT for Rlink by skipping stackIndex 0.
  //
  for (StackIndex = 1; StackIndex < MaxIioStackNumber; StackIndex++) {
    // Avoid programming if the stack does not include any MS2IOSF devices
    if ((IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_NONE) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_RESERVED) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX)) {
      continue;
    }

    if (IsSiliconWorkaroundEnabled ("S2202134789")) {
      IIO_D_LOG ("Program HW_INIT using mailbox\n");
      //
      // Send Pcode mailbox command to set hw_init bit
      // Command[7:0] = 0x82; Addr[8] = 1; Addr[23:16] = Mesh ID of the IIO stack
      //
      Status = PcuSetMs2iosfHwInit (IioIndex, MeshId10nmSoc[StackIndex]);
      if (EFI_ERROR (Status)) {
        IIO_D_ERR ("Issue setting HW_INIT from PCU, Status %r\n", Status);
        return;
      }

      //
      // Poll hw_init bit until the bit is cleared
      // Command[7:0] = 0x82; Addr[8] = 0; Addr[23:16] = Mesh ID of the IIO stack
      //
      Status = PcuGetMs2iosfHwInit (IioIndex, MeshId10nmSoc[StackIndex], &TcMiscUcr.Data);
      if (EFI_ERROR (Status)) {
        IIO_D_ERR ("Issue reading PCU, Status %r\n", Status);
        return;
      }

      WaitTime = 0;
      while ((TcMiscUcr.Bits.hw_init != 0) && (WaitTime < 10)) {
        IioStall (1);
        Status = PcuGetMs2iosfHwInit (IioIndex, MeshId10nmSoc[StackIndex], &TcMiscUcr.Data);
        if (EFI_ERROR (Status)) {
          IIO_D_ERR ("Issue reading HW_INIT from PCU, Status %r\n", Status);
          return;
        }
        WaitTime++;
      }
      if (WaitTime >= 10) {
        // Timeout value for hw_init to be completed is 10us.
        IIO_D_ERR ("hw_init Timeout happened for stack %d\n", StackIndex);
      }

    } else {
      IIO_D_LOG ("Program HW_INIT using SB access\n");
      //
      // Set hw_init bit 31
      //
      TcMiscUcr.Bits.hw_init = 1;
      IioWriteCpuCsr32 (IioIndex, StackIndex, TC_MISC_UCR_SB_M2PCIE_REG, TcMiscUcr.Data);

      if (!UbiosGenerationEnabled ()) {
        //
        // Poll hw_init bit until the bit is cleared
        //
        TcMiscUcr.Data = IioReadCpuCsr32 (IioIndex, StackIndex, TC_MISC_UCR_SB_M2PCIE_REG);
        WaitTime = 0;
        while ((TcMiscUcr.Bits.hw_init != 0) && (WaitTime < 10)) {
          IioStall (1);
          TcMiscUcr.Data = IioReadCpuCsr32 (IioIndex, StackIndex, TC_MISC_UCR_SB_M2PCIE_REG);
          WaitTime++;
        }
        if (WaitTime >= 10) {
          // Timeout value for hw_init to be completed is 10us.
          IIO_D_ERR ("HW_INIT Timeout stack %d\n", StackIndex);
        }
      } else {
        //
        // Poll hw_init bit until the bit is cleared
        //
        DEBUG ((DEBUG_ERROR, "\nLABEL_waithwinit_%d_%d:\n", IioIndex, StackIndex));
        DEBUG ((DEBUG_ERROR, "\nmov eax, DWORD PTR ds:[0%8xh]\n",
          UsraGetCsrRegisterAddress (IioIndex, StackIndex, TC_MISC_UCR_SB_M2PCIE_REG)));
        DEBUG ((DEBUG_ERROR, "\nbt  eax, 31\n"));
        DEBUG ((DEBUG_ERROR, "\njc LABEL_waithwinit_%d_%d\n", IioIndex, StackIndex));
      }
    }
  }
}

/**
 @brief This function disables decoding bank assigned to a device.

 @param[in] IioGlobalData - Pointer to global IIO data structure
 @param[in] IioIndex      - Index of IIO instance
 @param[in] StackIndex    - Stack index value of CPU/IIO
 @param[in] BankNumber    - Bank number of the device to disable

 @return Standard EFI status is returned.
**/
EFI_STATUS
Ms2iosfBankDecoderDisable (
  IN IIO_GLOBALS *IioGlobalData,
  IN UINT8        IioIndex,
  IN UINT8        StackIndex,
  IN UINT8        BankNumber)
{
  UINT32          Data;
  UINT8           Index;
  UINT8           Bus;

  if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << StackIndex))) {
    return EFI_INVALID_PARAMETER;
  }

  Bus = IioGlobalData->IioVar.IioVData.SocketStackBus[IioIndex][StackIndex];

  for (Index = 0; Index < BankDecoderVer.NumberEntries; Index++) {
    if (BankDecoderTable[Index].BankNumber != BankNumber) {
      continue;
    }

    Data = Ms2iosfRegRead (IioGlobalData, IioIndex, Bus,
                           BankDecoderTable[Index].RegisterSize, BankDecoderTable[Index].RegisterAddress);
    Data = (Data & BankDecoderTable[Index].FieldMask) | BankDecoderTable[Index].StackTypeData.Disable;

    DisableMs2IosfType1BankDecoder (IioIndex, StackIndex, Bus,
                                    BankDecoderTable[Index].BankNumber, BankDecoderTable[Index].RegisterAddress, &Data);
    Ms2iosfRegWrite (IioGlobalData, IioIndex, Bus,
                     BankDecoderTable[Index].RegisterSize, BankDecoderTable[Index].RegisterAddress, Data);
  }
  return EFI_SUCCESS;
}

/**
 @brief This function disables decoding bank assigned to a root port.

 @param[in] IioGlobalData - Pointer to global IIO data structure
 @param[in] IioIndex      - Index of IIO instance
 @param[in] IioPort       - IIO port index

 @return Standard EFI status is returned.
**/
EFI_STATUS
Ms2iosfBankDecoderDisablePciePort (
  IN IIO_GLOBALS *IioGlobalData,
  IN UINT8        IioIndex,
  IN UINT8        IioPort)
{
  UINT8           Stack;
  UINT8           Port;
  UINT8           Bus;
  UINT8           PortBank;
  UINT8           BarBank;

  if (IioIndex >= MaxIIO || IioPort >= GetMaxPciePortNumberPerSocket (IioIndex)) {

    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Port = IioPortIndexToStackPort (IioPort, &Stack);
  Bus = IioGlobalData->IioVar.IioVData.SocketStackBus[IioIndex][Stack];
  //
  // Find the banks to disable.
  //
  PortBank = BarBank = BANK_NONE;
  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {

    switch (IioGlobalData->IioVar.IioVData.StackId[IioIndex][Stack]) {
      case IIO_SINGLE_PCIE_0:
      case IIO_SINGLE_PCIE_1:
      case IIO_SINGLE_PCIE_2:
      case IIO_SINGLE_PCIE_3:
      case IIO_SINGLE_PCIE_4:
        PortBank = PORT_0A_BANK + Port;
        BarBank = PORT_0A_BAR_BANK + Port;
        break;
      case IIO_DMI_CB_SINGLE_PCIE:
        if (IioPort == PORT_0_INDEX) {
          PortBank = PORT_0A_BANK;
          break;
        }
      default:
        IIO_D_WARN ("MS2IOSF skip disabling bank decode for IIO port %d (stack %d of id %X)\n",
                    IioPort, Stack, IioGlobalData->IioVar.IioVData.StackId[IioIndex][Stack]);
        return EFI_SUCCESS;
    }
  } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {

    switch (IioGlobalData->IioVar.IioVData.StackId[IioIndex][Stack]) {
      case IIO_SINGLE_PCIE_0:
      case IIO_SINGLE_PCIE_4:
        PortBank = PORT_0A_BANK + Port;
        BarBank = PORT_0A_BAR_BANK + Port;
        break;
      default:
        IIO_D_WARN ("MS2IOSF skip disabling bank decode for IIO port %d (stack %d of id %X)\n",
                    IioPort, Stack, IioGlobalData->IioVar.IioVData.StackId[IioIndex][Stack]);
        return EFI_SUCCESS;
    }
  } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {

    switch (IioGlobalData->IioVar.IioVData.StackId[IioIndex][Stack]) {
      case IIO_SINGLE_PCIE_0:
        PortBank = PORT_0A_BANK + Port;
        BarBank = PORT_0A_BAR_BANK + Port;
        break;
      default:
        IIO_D_WARN ("MS2IOSF skip disabling bank decode for IIO port %d (stack %d of id %X)\n",
                    IioPort, Stack, IioGlobalData->IioVar.IioVData.StackId[IioIndex][Stack]);
        return EFI_SUCCESS;
    }
  }

  Ms2iosfBankDecoderDisable (IioGlobalData, IioIndex, Stack, PortBank);
  Ms2iosfBankDecoderDisable (IioGlobalData, IioIndex, Stack, BarBank);

  return EFI_SUCCESS;
}

/**
 This function updates bank decoder to decode as CXL.

 @param[in] IioGlobalData - Pointer to global IIO data structure
 @param[in] IioIndex      - Index of IIO instance
 @param[in] StackIndex    - IIO stack index

 @return Standard EFI status is returned.
**/
EFI_STATUS
Ms2iosfBankDecoderEnableCxl (
  IN IIO_GLOBALS *IioGlobalData,
  IN UINT8        IioIndex,
  IN UINT8        StackIndex
  )
{
  return EFI_UNSUPPORTED;
}

/**

  After resource allocation configure MS2IOSF Gen1 for routing transactions.

  @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param IioIndex           - Index of CPU/IIO

**/
VOID
Ms2iosfPostResources (
  IIO_GLOBALS    *IioGlobalData,
  UINT8          IioIndex
  )
{
  UINT8                StackIndex;
  UINT8                MaxIioStackNumber;
  UINT8                BusNumber;
  UINT8                BusBase;
  UINT8                SecBus;
  UINT8                SubBus;
  UINT16               PcieCommand;
  UINT16               Bank0PcieDevCtrl ;
  UINT16               Bank0BaseMemValue; // = 0xE680;
  UINT16               Bank0MemLimValue; // = 0xFB7F;
  UINT16               Bank0BasePMemLoValue;
  UINT16               Bank0MemPLimLoValue;
  UINT32               Bank0BasePMemHiValue;
  UINT32               Bank0MemPLimHiValue;
  KTI_STACK_RESOURCE   StackResource;
  UINT64               MemSize = 0;
  UINT64               BaseMem = 0;
  UINT64               MemLim  = 0;
  BOOLEAN              VmdEnabledWa;

  MSR_IA32_DEBUG_INTERFACE_REGISTER  DebugMsr;

  MaxIioStackNumber = GetMaxStackNumPerSocket ();

  for (StackIndex = 0; StackIndex < MaxIioStackNumber; StackIndex++) {
    if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << StackIndex))) {
      continue;
    }

    if (IsSiliconWorkaroundEnabled ("S1408837770") && IsVMDEnabledForStack (IioIndex, StackIndex)) {
      VmdEnabledWa = 1;
    } else {
      VmdEnabledWa = 0;
    }

    // Avoid to overriding bank register values if the MS2IOSF stack does not need to be configured
    if (IsCpuAndRevision (CPU_ICXD, REV_ALL) && !VmdEnabledWa ||
        IsCpuAndRevision (CPU_SNR, REV_ALL) && (StackIndex != IIO_HQM) && !VmdEnabledWa ||
        IsCpuAndRevision (CPU_ICXSP, REV_ALL) && !VmdEnabledWa ||
        IsCpuAndRevision (CPU_TNR, REV_ALL) && !VmdEnabledWa) {
      continue;
    }

    GetStackResouce (IioIndex, StackIndex, &StackResource);
    BusBase = StackResource.BusBase;
    if (!VmdEnabledWa) {
      BusNumber = StackResource.BusLimit - V_IOV_RESERVED_BUS_NUMBER;
    } else {
      BusNumber = StackResource.BusBase;
      if (IioIndex == 0) {
        SecBus = 0x80;
        SubBus = 0xFF;
      } else {
        SecBus = 0x1;
        SubBus = 0x7F;
      }

      /*
       * Secondary Bus - Root BusS4 + 1 assigned last bus
       * Subordinary bus  - Root BusS4 + 1 assigned last bus
       */
      IIO_D_LOG ("MS2IOSF socket %X stack %X SecBus 0x%X SubBus 0x%X BusNumber 0x%X\n", IioIndex, StackIndex, SecBus, SubBus, BusNumber);

      // Secondary Bus - Root BusS4 + 1 assigned last bus
      IioPciExpressWrite8 (IioIndex, BusBase, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, R_BANK0_SECBUS, SecBus);

      // Subordinary bus  - Root BusS4 + 1 assigned last bus
      IioPciExpressWrite8 (IioIndex, BusBase, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, R_BANK0_SUBBUS, SubBus);
    }

    /*
     * Memory Base/Limit - Stack2 MMIO Base and Limit
     * PCie Command - BME and ME
     * Device Control - maxpayload.
     */
    if ((StackResource.MmiolLimit != 0) && (StackResource.MmiolBase != (-1)) || VmdEnabledWa) {

      if (VmdEnabledWa) {
        BaseMem = IioPciExpressRead32 (IioIndex, BusNumber, MS2IOSF_DEV_NUM, 5, 0x18); // membar1
        BaseMem &= ~((UINT64) 0xFFF);
        Bank0BaseMemValue = (UINT16) (BaseMem >> 16);
        MemLim = IioPciExpressRead8 (IioIndex, BusNumber, MS2IOSF_DEV_NUM, 5, 0x4D); // membar1size
        MemLim = BaseMem + ((1 << MemLim) - 1);
        Bank0MemLimValue = (UINT16) (MemLim >> 16);
      } else {
        MemSize = ((StackResource.MmiolLimit - StackResource.MmiolBase) / 2) + 1;
        BaseMem = StackResource.MmiolBase + MemSize;
        Bank0BaseMemValue = (UINT16) (BaseMem >> 16);
        MemLim = StackResource.MmiolLimit;
        Bank0MemLimValue  = (UINT16) (MemLim >> 16);
      }

      // Memory Base - Stack2 MMIOL Base
      IioPciExpressWrite16 (IioIndex, BusBase, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, R_BANK0_MBAS, Bank0BaseMemValue);
      // Memory Limit - Stack2 MMIOL Limit
      IioPciExpressWrite16 (IioIndex, BusBase, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, R_BANK0_MLIM, Bank0MemLimValue);
      IIO_D_LOG ("MS2IOSF socket %X stack %X, BusNumber 0x%X BaseMemL 0x%X MemLimL 0x%X\n", IioIndex, StackIndex , BusNumber, BaseMem, MemLim);
    }

    BaseMem = ((UINT64) StackResource.MmiohBase.hi << 32) | StackResource.MmiohBase.lo;
    MemLim = ((UINT64) StackResource.MmiohLimit.hi << 32) | StackResource.MmiohLimit.lo;
    if ((MemLim != 0) && (BaseMem != (-1)) || VmdEnabledWa) {
      if (VmdEnabledWa) {
        BaseMem = IioPciExpressRead32 (IioIndex, BusNumber, MS2IOSF_DEV_NUM, 5, 0x20); // membar2 low
        BaseMem = ((UINT64) IioPciExpressRead32 (IioIndex, BusNumber, MS2IOSF_DEV_NUM, 5, 0x24) << 32) | BaseMem; // membar2 high
        MemLim = IioPciExpressRead8 (IioIndex, BusNumber, MS2IOSF_DEV_NUM, 5, 0x4E); // membar1size
        MemLim = BaseMem + ((1 << MemLim) - 1);
        Bank0BasePMemLoValue = (UINT16) (BaseMem >> 16);
        Bank0BasePMemHiValue = BaseMem >> 32;
        Bank0MemPLimLoValue = (UINT16) (MemLim >> 16);
        Bank0MemPLimHiValue = MemLim >> 32;
      } else {
        MemSize = ((MemLim - BaseMem) / 2) + 1;
        BaseMem += MemSize;
        Bank0BasePMemLoValue = (UINT16) (BaseMem >> 16);
        Bank0MemPLimLoValue  = (UINT16) (MemLim >> 16);
        Bank0BasePMemHiValue = (BaseMem >> 32);
        Bank0MemPLimHiValue  = (MemLim >> 32);
      }

      // Memory Base - Stack2 Lower MMIOH Base
      IioPciExpressWrite16 (IioIndex, BusBase, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, R_BANK0_PBAS, Bank0BasePMemLoValue);
      // Memory Limit - Stack2 Lower MMIOH Limit
      IioPciExpressWrite16 (IioIndex, BusBase, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, R_BANK0_PLIM, Bank0MemPLimLoValue);
      // Memory Base - Stack2 Upper MMIOH Base
      IioPciExpressWrite32 (IioIndex, BusBase, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, R_BANK0_PBASU, Bank0BasePMemHiValue);
      // Memory Limit - Stack2 Upper MMIOH Limit
      IioPciExpressWrite32 (IioIndex, BusBase, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, R_BANK0_PLIMU, Bank0MemPLimHiValue);
      IIO_D_LOG ("MS2IOSF socket %X stack %X, BusNumber 0x%X BaseMemH 0x%lX MemLimH 0x%lX\n", IioIndex, StackIndex , BusNumber, BaseMem, MemLim);
    }

    if (!VmdEnabledWa) {
      // Device Control - maxpayload.
      Bank0PcieDevCtrl = IioPciExpressRead16 (IioIndex, BusBase, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, R_BANK0_DEVCON);
      Bank0PcieDevCtrl &= ~(BIT7 | BIT6 | BIT5);
      Bank0PcieDevCtrl |= (2 << 5); //max_payload_size: 1 = 256, 2 = 512
      IioPciExpressWrite16 (IioIndex, BusBase, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, R_BANK0_DEVCON, Bank0PcieDevCtrl);
    }

    // PCie Command - BME and MSE
    PcieCommand = IioPciExpressRead16 (IioIndex, BusBase, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, R_BANK0_PCICMD);
    PcieCommand |= BIT1; //mse
    PcieCommand |= BIT2; //bme
    IioPciExpressWrite16 (IioIndex, BusBase, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, R_BANK0_PCICMD, PcieCommand);
  }

  //
  // Check debug consent before hiding OOBMSM DFX config space from OS.
  //
  DebugMsr.Uint32 = AsmReadMsr32 (MSR_IA32_DEBUG_INTERFACE);
  if (DebugMsr.Bits.Enable == 0) {
    Ms2iosfBankDecoderDisable (IioGlobalData, IioIndex, IIO_STACK0, MSM_DFX_BANK);
  }
}

/**

  Program MS2IOSF bank decoder shadowed Bus number

  @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param IioIndex           - Index of CPU/IIO

**/
VOID
OverrideMs2iosfShadowedBus (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  )
{
  UINT8                        StackIndex;
  UINT8                        MaxIioStackNumber;
  BANK0_SECBUS_IIO_VTD_STRUCT  Bank0Secbus;
  BANK0_SUBBUS_IIO_VTD_STRUCT  Bank0Subbus;

  MaxIioStackNumber = GetMaxStackNumPerSocket ();

  for (StackIndex = 0; StackIndex < MaxIioStackNumber; StackIndex++) {
    // Avoid programming if the stack does not include a device that needs to be shadowed
    if (!(IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_NAC && StackIndex == IIO_HQM)) {
      continue;
    }

    Bank0Secbus.Data = IioGlobalData->IioVar.IioVData.SocketStackLimitBusNumber[IioIndex][StackIndex] - V_IOV_RESERVED_BUS_NUMBER;
    Bank0Subbus.Data = Bank0Secbus.Data;

    /*
     * Secondary Bus - Root BusS4 + 1 assigned last bus
     * Subordinary bus  - Root BusS4 + 1 assigned last bus
     */
    IIO_D_LOG ("[%d.%d] MS2IOSF HQM BusNumber 0x%X \n", IioIndex, StackIndex, Bank0Secbus.Data);

    // Secondary Bus - Root BusS4 + 1 assigned last bus
    IioWriteCpuCsr8 (IioIndex, StackIndex, BANK0_SECBUS_IIO_VTD_REG, Bank0Secbus.Data);

    // Subordinary bus  - Root BusS4 + 1 assigned last bus
    IioWriteCpuCsr8 (IioIndex, StackIndex, BANK0_SUBBUS_IIO_VTD_REG, Bank0Subbus.Data);
  }
}

/**
  Update Trace Hub FW_BAR shadow register. Additional specific programming for 10nm wave 1/2.

  @param[in]  IioGlobalData       Pointer to IIO Universal Data Structure (UDS)
  @param[in]  IioIndex            Index with the changed IIO (Socket)
  @param[in]  StackIndex          Trace Hub stack number
  @param[in]  FwBar               Value of Trace Hub FW_BAR
**/
VOID
IioMs2iosfUpdateFwBarShadowReg (
  IN  IIO_GLOBALS        *IioGlobalData,
  IN  UINT8               IioIndex,
  IN  UINT8               StackIndex,
  IN  UINT64              FwBar
  )
{
  UINT32                          FwLbar;
  UINT32                          FwUbar;
  UINT8                           MaxIioStackNumber;
  UINT32                          BankRegister;
  UINT8                           BusNumber;

  // Avoid programming if the stack does not include any MS2IOSF devices
  if ((IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_NONE) ||
      (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_RESERVED) ||
      (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX)) {
    return;
  }

  MaxIioStackNumber = GetMaxStackNumPerSocket ();

  BusNumber = IioGlobalData->IioVar.IioVData.SocketStackBus[IioIndex][StackIndex];

  BankRegister = TYPE0_BANK_START + TYPE0_OFFSET_PER_BANK * (NPK_FWBAR_BANK - BANK8_DEC);

  FwLbar = (UINT32) FwBar;
  FwUbar = (UINT32) (FwBar >> 32);

  //
  // Update FW_BAR shadow register
  //
  IioPciExpressWrite32 (IioIndex, BusNumber, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, BankRegister + BAR_N0_OFFSET, FwLbar);
  IioPciExpressWrite32 (IioIndex, BusNumber, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, BankRegister + BAR_N1_OFFSET, FwUbar);
}


/**
  Update OOB MSM BAR shadow register. Additional specific programming for 10nm wave 3.

  @param[in]  IioGlobalData       Pointer to IIO Universal Data Structure (UDS)
  @param[in]  IioIndex            Index with the changed IIO (Socket)
**/
VOID
IioMs2iosfUpdateOobMsmBarShadowReg (
  IN  IIO_GLOBALS        *IioGlobalData,
  IN  UINT8               IioIndex
  )
{
  //
  // Not supported, OOB MSM has shadow bit enabled
  //
}


/**
 @brief This function updates DMI Max Payload Size in MS2IOSF DMI bank DevCtl register.

 @param[in] IioGlobalPtr   - Pointer to global IIO data structure.
 @param[in] IioIndex       - Index of IIO instance.
 @param[in] MaxPayloadSize - Max Payload Size as defined for PCIe Device Control register.

 @return Standard EFI status is returned.
**/
EFI_STATUS
IioMs2iosfUpdateDmiMps (
  IN IIO_GLOBALS              *IioGlobalPtr,
  IN UINT8                     IioIndex,
  IN UINT8                     MaxPayloadSize)
{
  BANK27_DEVCON_IIO_VTD_STRUCT B27DevCtl;

  if (IioIndex >= MAX_SOCKET) {
    return EFI_INVALID_PARAMETER;
  }
  B27DevCtl.Data = IioReadCpuCsr16 (IioIndex, IIO_DMI_CB_SINGLE_PCIE, BANK27_DEVCON_IIO_VTD_REG);
  B27DevCtl.Bits.max_payload_size = MaxPayloadSize;
  IioWriteCpuCsr16 (IioIndex, IIO_DMI_CB_SINGLE_PCIE, BANK27_DEVCON_IIO_VTD_REG, B27DevCtl.Data);
  return EFI_SUCCESS;
}

