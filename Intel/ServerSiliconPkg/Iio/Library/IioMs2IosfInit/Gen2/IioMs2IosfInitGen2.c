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
#include "IioBankDecoderGen2.h"
#include "IioCreditsGen2.h"
#include <Library/SystemInfoLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/IioSysInfoLib.h>
#include <Library/KtiApi.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/IioMiscFeatureLib.h>
#include <IioSetupDefinitions.h>
#include <LibraryPrivate/IioDebug.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/CxlIpLib.h>
#include <Library/IioNpkInit.h>
#include <Register/Intel/ArchitecturalMsr.h>
#include <Library/PreSiliconEnvDetectLib.h>

extern BANK_DECODER_HEADER_GEN2  BankDecoderVerGen2;
extern BANK_DECODER_ENTRY_GEN2   BankDecoderTableGen2[506];

extern CREDITS_HEADER       ItcCreditsVerGen2;
extern CREDITS_ENTRY        ItcCreditDefaultTableGen2[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditDmiTableGen2[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditPcieGen5x16Table[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditDinoAxTable[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditDinoTable[ITC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        ItcCreditDinoHcbTable[ITC_NUMBER_ENTRIES_GEN2];

extern CREDITS_HEADER       OtcCreditsVerGen2;
extern CREDITS_ENTRY        OtcCreditDefaultTableGen2[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditDmiTableGen2[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditPcieGen5x16Table[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditDinoAxTable[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditDinoTable[OTC_NUMBER_ENTRIES_GEN2];
extern CREDITS_ENTRY        OtcCreditDinoHcbTable[OTC_NUMBER_ENTRIES_GEN2];
/**
  IIO: This routine programs DFX register access control in BIOS.

  @param SocId                 - Socket ID
  @param StackId                 - Controller ID
  @param AccessEnableOrDisable - Enable or disable register access

**/

VOID
IioEnableDisableDfxRegisterAccess (
  UINT8  SocId,
  UINT8  StackId,
  BOOLEAN AccessEnableOrDisable
  )
{
  DFX_PG_RAC_N0_IIO_VTD_STRUCT   DfxPgRacN0;
  DFX_PG_WAC_N0_IIO_VTD_STRUCT   DfxPgWacN0;

  DfxPgRacN0.Data = IioReadCpuCsr32 (SocId, StackId, DFX_PG_RAC_N0_IIO_VTD_REG);
  DfxPgWacN0.Data = IioReadCpuCsr32 (SocId, StackId, DFX_PG_WAC_N0_IIO_VTD_REG);

  DfxPgRacN0.Bits.dfx_sai_pol_4 = AccessEnableOrDisable;
  DfxPgWacN0.Bits.dfx_sai_pol_4 = AccessEnableOrDisable;

  IioWriteCpuCsr32 (SocId, StackId, DFX_PG_RAC_N0_IIO_VTD_REG, DfxPgRacN0.Data);
  IioWriteCpuCsr32 (SocId, StackId, DFX_PG_WAC_N0_IIO_VTD_REG, DfxPgWacN0.Data);

}
/**
    Convert Bank Number to PCIe port index in a socket
    @param StackIndex     - Stack Index
    @param BankNumber     - Port Index

    @retval port index in a socket

**/
UINT8
GetPortIndexbyBankNumber (
  IN UINT8    StackIndex,
  IN UINT8    BankNumber
)
{
  UINT8       PortOffsetInStack = 0;
  UINT8       PortIndex = 0;

  if (!IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    return PORT_A_INDEX;
  }

  if (BankNumber <= PORT_x16_D_BANK_GEN2) {
    PortOffsetInStack = 2 * (BankNumber - PORT_x16_A_BANK_GEN2);
  } else if (BankNumber <= PORT_x8_D_BANK_GEN2) {
    PortOffsetInStack = 7 - (2 * (BankNumber - PORT_x8_A_BANK_GEN2));
  } else if (BankNumber <= PORT_x16_D_BAR_BANK_GEN2) {
    PortOffsetInStack = 2 * (BankNumber - PORT_x16_A_BAR_BANK_GEN2);
  } else if (BankNumber <= PORT_x8_D_BAR_BANK_GEN2) {
    PortOffsetInStack = 7 - (2 * (BankNumber - PORT_x8_A_BAR_BANK_GEN2));
  } else {
    ASSERT (FALSE);
  }

  PortIndex = GetPortIndexbyStack (StackIndex, PortOffsetInStack);
  return PortIndex;
}

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
    // Enable DFX access before PCIE porting
    IioEnableDisableDfxRegisterAccess (IioIndex, StackIndex, TRUE);

    BusNumber = IioGlobalData->IioVar.IioVData.SocketStackBus[IioIndex][StackIndex];
    //
    // Program the upper 32 bits of the shadowed BAR value for type 0 banks with zeroes.
    //
    Type0BankStop = TYPE0_BANK_STOP_GEN2;
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
    //
    // Only supported on subtractive bank on the MS2IOSF instance that has DMI connected
    //
    if (!(IioIndex == 0 && StackIndex == IIO_STACK0)) {
      EoiBroadcastEn.Data = IioReadCpuCsr32 (IioIndex, StackIndex, EOI_BROADCAST_EN_IIO_VTD_REG);
      EoiBroadcastEn.Bits.eoi_bcast_en = 0;
      IioWriteCpuCsr32 (IioIndex, StackIndex, EOI_BROADCAST_EN_IIO_VTD_REG, EoiBroadcastEn.Data);
    }
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
  UINT8                              OobBusOwnership;

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

    if (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_DINO) {
      //
      // Only OOB_MSM primary supports MCTP in DINO.
      // OOB_MSM is not MCTP capable when OOB is not the MCTP bus owner.
      // Need to check OOB for ownership.
      // Msm scratch pad register BIT0 is set when OOB is the MCTP bus owner.
      //
      MctpBroadcastEn.Data = 0;
      if (StackIndex == IIO_STACK8) {
        OobBusOwnership = IioReadCpuCsr32 (IioIndex, 0, MSM_SCRATCH_PAD_DISCMSM_SCRATCH_PAD_DISC_MSM_PMON_REG) & BIT0;
        MctpBroadcastEn.Bits.mctp_bcast_en = (OobBusOwnership << MSM_BANK_GEN2);
      }
    }

    if (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX_IIO) {
      //
      // Enable/Disable MCTP broadcast on PCIe Root Port
      //
      for (Bank = BANK0_DEC; Bank < BANK8_DEC; Bank++) {

        PortIndex = GetPortIndexbyBankNumber (StackIndex, Bank);
        MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];

        if (IioGlobalData->SetupData.MctpEn[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == IIO_OPTION_DISABLE) {
          MctpBroadcastEn.Bits.mctp_bcast_en &= (~(1 << Bank));
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
  ENQ_CAPABLE_IIO_VTD_STRUCT   EnqCapable;
  UINT8                        StackIndex;
  UINT8                        MaxIioStackNumber;

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

    EnqCapable.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ENQ_CAPABLE_IIO_VTD_REG);

    if (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_DINO) {
      //
      // Supports ENQ in DINO: DSA, IAX and CPM
      //
      if (!IsHbmSku ()) { // No HCx on HBM SKU
        EnqCapable.Bits.enq_capable |= (1 << CPM_SRIOV_BANK_GEN2);
      }
      EnqCapable.Bits.enq_capable |= (1 << DSA_BAR_0_BANK_GEN2);
      EnqCapable.Bits.enq_capable |= (1 << DSA_BAR_1_BANK_GEN2);
      EnqCapable.Bits.enq_capable |= (1 << IAX_BAR_0_BANK_GEN2);
      EnqCapable.Bits.enq_capable |= (1 << IAX_BAR_1_BANK_GEN2);
      //
      // OOB_MSM_DFD is the subtractive bank in fabric decode mode which is enabled by default
      //
      EnqCapable.Bits.enq_capable &= ~(1 << FABRIC_DECODE_BANK_GEN2);
    }

    IioWriteCpuCsr32 (IioIndex, StackIndex, ENQ_CAPABLE_IIO_VTD_REG, EnqCapable.Data);
  }
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
    case BANK17_DEC:
      Csr = BANK17_BARSIZE_IIO_VTD_REG;
      break;
    case BANK18_DEC:
      Csr = BANK18_BARSIZE_IIO_VTD_REG;
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
    if (Status != EFI_SUCCESS) {
      continue;
    }
    if (CheckNtbPortConfSetting (IioIndex, PortIndex) != TRUE) {
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
      continue;
    }
    SetBankBarSize ( IioIndex, StackIndex, NTB_BAR_1_BANK_GEN2, BarSize23);
    SetBankBarSize ( IioIndex, StackIndex, NTB_BAR_2_BANK_GEN2, BarSize45);
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
  UINT8       MaxPortNumberPerSocket;
  UINT8       MaxIioStackNumber;
  UINT8       DinoInst = 0;
  BANK_DECODER_HEADER_GEN2  *BankDecoderVerGen2Ptr = NULL;
  BANK_DECODER_ENTRY_GEN2   *BankDecoderTableGen2Ptr = NULL;

  IIO_D_LOG ("MS2IOSF_BANK_DECODER_INIT_START\n");

  BankDecoderVerGen2Ptr = (BANK_DECODER_HEADER_GEN2*)&BankDecoderVerGen2;
  BankDecoderTableGen2Ptr = (BANK_DECODER_ENTRY_GEN2*)&BankDecoderTableGen2;
  TableSize = BankDecoderVerGen2Ptr->NumberEntries;
  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
  MaxIioStackNumber = GetMaxStackNumPerSocket ();

  IIO_D_LOG ("MS2IOSF BankDecoder: TableSize %d, Recipe Revision %a\n", TableSize, BankDecoderVerGen2Ptr->Revision);
  for (TableIndex = 0; TableIndex < TableSize; TableIndex++) {
    for (StackIndex = 0; StackIndex < MaxIioStackNumber; StackIndex++) {
      // Avoid to program bank decoder values if the stack does not include any MS2IOSF device
      if ((IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_NONE) ||
          (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_RESERVED) ||
          (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX)) {
        continue;
      }
      BusNumber = IioGlobalData->IioVar.IioVData.SocketStackBus[IioIndex][StackIndex];

      Data = Ms2iosfRegRead (IioGlobalData, IioIndex, BusNumber,
                             BankDecoderTableGen2Ptr[TableIndex].RegisterSize, BankDecoderTableGen2Ptr[TableIndex].RegisterAddress);

      IIO_D_DBG ("MS2IOSF BankDecoder: StackID[%x][%x] = %x, Bank = %d\n", IioIndex, StackIndex,
        IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex], BankDecoderTableGen2Ptr[TableIndex].BankNumber);

      switch (IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex]) {
      case IIO_DMI_CB_SINGLE_PCIE:
        switch (BankDecoderTableGen2Ptr[TableIndex].BankNumber) {
        case DMI_DECODE_BANK_GEN2:
          if (IioDmiIsPortIdentity (IioIndex, PORT_0_INDEX, IioDmiPortActive)) {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.Pi5Dmi);
          } else {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.Disable);
          }
          break;
        default:
          Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.Disable);
          break;
        }
        break;
      case IIO_SINGLE_PCIE_0:
      case IIO_SINGLE_PCIE_1:
      case IIO_SINGLE_PCIE_2:
      case IIO_SINGLE_PCIE_3:
      case IIO_SINGLE_PCIE_4:
      case IIO_SINGLE_PCIE_5:
      case IIO_SINGLE_PCIE_6:
        switch (BankDecoderTableGen2Ptr[TableIndex].BankNumber) {
        case PORT_x16_A_BANK_GEN2:
        case PORT_x16_B_BANK_GEN2:
        case PORT_x16_C_BANK_GEN2:
        case PORT_x16_D_BANK_GEN2:
        case PORT_x8_A_BANK_GEN2:
        case PORT_x8_B_BANK_GEN2:
        case PORT_x8_C_BANK_GEN2:
        case PORT_x8_D_BANK_GEN2:
        case PORT_x16_A_BAR_BANK_GEN2:
        case PORT_x16_B_BAR_BANK_GEN2:
        case PORT_x16_C_BAR_BANK_GEN2:
        case PORT_x16_D_BAR_BANK_GEN2:
        case PORT_x8_A_BAR_BANK_GEN2:
        case PORT_x8_B_BAR_BANK_GEN2:
        case PORT_x8_C_BAR_BANK_GEN2:
        case PORT_x8_D_BAR_BANK_GEN2:
          PortIndex = GetPortIndexbyBankNumber (StackIndex, BankDecoderTableGen2Ptr[TableIndex].BankNumber);
          if ((IioGlobalData->IioVar.IioOutData.PciePortOwnership[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == REGULAR_PCIE_OWNERSHIP) &&
              (IioGlobalData->SetupData.PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == FALSE)) {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.Pi5PcieMode);
          } else {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.Disable);
          }
          break;
        case NTB_BAR_0_BANK_GEN2:
        case NTB_BAR_1_BANK_GEN2:
        case NTB_BAR_2_BANK_GEN2:
          PortIndex = GetPortIndexbyStack (StackIndex, IioPortH);
          if (CheckNtbPortConfSetting (IioIndex, PortIndex) == TRUE) {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.Pi5PcieMode);
          } else {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.Disable);
          }
          break;
        case CXL_DECODE_BANK_GEN2:
          //
          // For CXL, enable Gen5 bank decoders before link training.
          //
          Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.Disable);
          break;
        default:
          Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.Disable);
          break;
        }
        break;
      case IIO_DINO_0:
      case IIO_DINO_1:
      case IIO_DINO_2:
      case IIO_DINO_3:
        DinoInst = StackIndex - IIO_STACK8;
        switch (BankDecoderTableGen2Ptr[TableIndex].BankNumber) {
        case CPM_SRIOV_BANK_GEN2:
        case HQM_SRIOV_BANK_GEN2:
          if (GetHcxType (IioIndex) == IIO_HCB) {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.DinoHcb);
          } else if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.DinoHcxB0);
          } else {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.DinoHcx);
          }
          break;
        case MSM_BANK_GEN2:
        case MSM_PMON_1_BANK_GEN2:
        case MSM_PMON_2_BANK_GEN2:
        case FABRIC_DECODE_BANK_GEN2: // MSM_DFX set as subtractive decode
          if (GetHcxType (IioIndex) == IIO_HCB) {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.DinoHcb);
          } else if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.DinoHcxB0);
          } else {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.DinoHcx);
          }
          //
          // Disable PSF when the bank is in source decode
          //
          IioMsmPsfFunctionDisable (IioGlobalData, IioIndex, StackIndex);
          break;
        case NPK_1_BANK_GEN2:
        case NPK_2_BANK_GEN2:
        case NPK_3_BANK_GEN2:
        case NPK_FWBAR_BANK_GEN2:
          if (GetHcxType (IioIndex) == IIO_HCB) {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.DinoHcb);
          } else if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.DinoHcxB0);
          } else {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.DinoHcx);
          }
          //
          // Disable PSF when the bank is in source decode
          //
          IioCpuTraceHubPsfFunctionDisable (IioIndex, DinoInst);
          break;
        case DSA_BAR_0_BANK_GEN2:
        case DSA_BAR_1_BANK_GEN2:
          if (IioGlobalData->SetupData.DsaEn[IioIndex * NUM_DSA + DinoInst] == IIO_OPTION_DISABLE) {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.Disable);
          } else if (GetHcxType (IioIndex) == IIO_HCB) {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.DinoHcb);
          } else if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.DinoHcxB0);
          } else {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.DinoHcx);
          }
          //
          // Disable PSF when the bank is in source decode
          //
          if (!IsSimicsEnvironment()) {
            IioDsaPsfFunctionDisable (IioGlobalData, IioIndex, StackIndex);
          }
          break;
        case IAX_BAR_0_BANK_GEN2:
        case IAX_BAR_1_BANK_GEN2:
          if (IioGlobalData->SetupData.IaxEn[IioIndex * NUM_IAX + DinoInst] == IIO_OPTION_DISABLE) {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.Disable);
          } else if (GetHcxType (IioIndex) == IIO_HCB) {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.DinoHcb);
          } else if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.DinoHcxB0);
          } else {
            Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.DinoHcx);
          }
          //
          // Disable PSF when the bank is in source decode
          //
          if (!IsSimicsEnvironment()) {
            IioIaxPsfFunctionDisable (IioGlobalData, IioIndex, StackIndex);
          }
          break;
        default:
          Data = ((Data & BankDecoderTableGen2Ptr[TableIndex].FieldMask) | BankDecoderTableGen2Ptr[TableIndex].StackTypeData.Disable);
          break;
        }
        break;
      default:
        Data = 0;
        break;
      }

      IIO_D_DBG ("MS2IOSF BankDecoder: S:%x B:%x D:%x F:%x Off:0x%x = 0x%x\n",
        IioIndex, BusNumber, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, BankDecoderTableGen2Ptr[TableIndex].RegisterAddress, Data);

      if (!(IsSiliconWorkaroundEnabled ("S1208984927") && (BankDecoderTableGen2Ptr[TableIndex].RegisterAddress & OFFSET_MASK) == DESTID_OFFSET &&
            BankDecoderTableGen2Ptr[TableIndex].BankNumber == BANK19_DEC && IioIndex == 0 && StackIndex == IIO_STACK0)) {

        DisableMs2IosfType1BankDecoder (IioIndex, StackIndex, BusNumber, BankDecoderTableGen2Ptr[TableIndex].BankNumber,
                                        BankDecoderTableGen2Ptr[TableIndex].RegisterAddress, &Data);
        Ms2iosfRegWrite (IioGlobalData, IioIndex, BusNumber,
                         BankDecoderTableGen2Ptr[TableIndex].RegisterSize, BankDecoderTableGen2Ptr[TableIndex].RegisterAddress, Data);
      }
    }
  }

  IIO_D_LOG ("MS2IOSF_BANK_DECODER_INIT_END\n");
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
  PI5_MODES   StackMode;

  ItcCreditsVerPtr = (CREDITS_HEADER*) &ItcCreditsVerGen2;
  ItcCreditsTablePtr = (CREDITS_ENTRY*) &ItcCreditDefaultTableGen2;

  TableSize = ItcCreditsVerPtr->NumberEntries;
  MaxIioStackNumber = GetMaxStackNumPerSocket ();

  IIO_D_LOG ("MS2IOSF Credits ITC: TableSize %d, Recipe Revision %a\n", TableSize, ItcCreditsVerPtr->Revision);

  //
  // If statements to resolve KW issue
  //
  if (IioIndex >= MaxIIO || MaxIioStackNumber > NELEMENTS (IioGlobalData->IioVar.IioVData.StackId[IioIndex])) {
    ASSERT (FALSE);
    return;
  }

  for (StackIndex = 0; StackIndex < MaxIioStackNumber; StackIndex++) {
    // Avoid to program credits values if the stack does not include any MS2IOSF device
    if ((IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_NONE) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_RESERVED) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX)) {
      continue;
    }

    if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
      switch (IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex]) {
      case IIO_DMI_CB_SINGLE_PCIE:
        ItcCreditsTablePtr = (CREDITS_ENTRY*) &ItcCreditDmiTableGen2;
        break;
      case IIO_SINGLE_PCIE_0:
      case IIO_SINGLE_PCIE_1:
      case IIO_SINGLE_PCIE_2:
      case IIO_SINGLE_PCIE_3:
      case IIO_SINGLE_PCIE_4:
      case IIO_SINGLE_PCIE_5:
      case IIO_SINGLE_PCIE_6:
        PortIndex = GetPortIndexbyStack (StackIndex, IioPortA);
        StackMode = GetStackMode (IioGlobalData, IioIndex, StackIndex);
        Bifurcation = CheckBifurcationMapPort (IioGlobalData, IioIndex, PortIndex);
        ItcCreditsTablePtr = GetPi5CreditTable (CREDIT_TYPE_ITC, Bifurcation, StackMode);
        break;
      case IIO_DINO_0:
      case IIO_DINO_1:
      case IIO_DINO_2:
      case IIO_DINO_3:
        if (GetHcxType (IioIndex) == IIO_HCB) {
          ItcCreditsTablePtr = (CREDITS_ENTRY*) &ItcCreditDinoHcbTable;
        } else {
          if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
            ItcCreditsTablePtr = (CREDITS_ENTRY*) &ItcCreditDinoTable;
          } else {
            ItcCreditsTablePtr = (CREDITS_ENTRY*) &ItcCreditDinoAxTable;
          }
        }
        break;
      default:
        ItcCreditsTablePtr = (CREDITS_ENTRY*) &ItcCreditDefaultTableGen2;
        break;
      }
    }

    BusNumber = IioGlobalData->IioVar.IioVData.SocketStackBus[IioIndex][StackIndex];

    for (TableIndex = 0; TableIndex < TableSize; TableIndex++) {
      if (!IsSiliconWorkaroundEnabled ("S14010043240") &&
          (ItcCreditsTablePtr[TableIndex].RegisterAddress & TCPRHPEND_OFFSET_MASK) == TCPRHPEND_OFFSET) {
        //
        // On MS2IOSF Gen2, only program TCPRHPEND registers for silicon workarounds.
        //
        continue;
      }

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
  PI5_MODES   StackMode;

  OtcCreditsVerPtr = (CREDITS_HEADER*) &OtcCreditsVerGen2;
  OtcCreditsTablePtr = (CREDITS_ENTRY*) &OtcCreditDefaultTableGen2;

  TableSize = OtcCreditsVerPtr->NumberEntries;
  MaxIioStackNumber = GetMaxStackNumPerSocket ();

  IIO_D_LOG ("MS2IOSF Credits OTC: TableSize %d, Recipe Revision %a\n", TableSize, OtcCreditsVerPtr->Revision);

  //
  // If statements to resolve KW issue
  //
  if (IioIndex >= MaxIIO || MaxIioStackNumber > NELEMENTS (IioGlobalData->IioVar.IioVData.StackId[IioIndex])) {
    ASSERT (FALSE);
    return;
  }

  for (StackIndex = 0; StackIndex < MaxIioStackNumber; StackIndex++) {
    // Avoid to program credits values if the stack does not include any MS2IOSF device
    if ((IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_NONE) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_RESERVED) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX)) {
      continue;
    }

    if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
      switch (IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex]) {
      case IIO_DMI_CB_SINGLE_PCIE:
        OtcCreditsTablePtr = (CREDITS_ENTRY*) &OtcCreditDmiTableGen2;
        break;
      case IIO_SINGLE_PCIE_0:
      case IIO_SINGLE_PCIE_1:
      case IIO_SINGLE_PCIE_2:
      case IIO_SINGLE_PCIE_3:
      case IIO_SINGLE_PCIE_4:
      case IIO_SINGLE_PCIE_5:
      case IIO_SINGLE_PCIE_6:
        PortIndex = GetPortIndexbyStack (StackIndex, IioPortA);
        StackMode = GetStackMode (IioGlobalData, IioIndex, StackIndex);
        Bifurcation = CheckBifurcationMapPort (IioGlobalData, IioIndex, PortIndex);
        OtcCreditsTablePtr = GetPi5CreditTable (CREDIT_TYPE_OTC, Bifurcation, StackMode);
        break;
      case IIO_DINO_0:
      case IIO_DINO_1:
      case IIO_DINO_2:
      case IIO_DINO_3:
        if (GetHcxType (IioIndex) == IIO_HCB) {
          OtcCreditsTablePtr = (CREDITS_ENTRY*) &OtcCreditDinoHcbTable;
        } else {
          if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
            OtcCreditsTablePtr = (CREDITS_ENTRY*) &OtcCreditDinoTable;
          } else {
            OtcCreditsTablePtr = (CREDITS_ENTRY*) &OtcCreditDinoAxTable;
          }
        }
        break;
      default:
        OtcCreditsTablePtr = (CREDITS_ENTRY*) &OtcCreditDefaultTableGen2;
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
  ITCCTRL01_N0_IIO_VTD_STRUCT               ItcCtrl01N0;
  ITC_MISC_HDR_CRDT_CH_08_09_IIO_VTD_STRUCT ItcMiscVTD08_09;
  ITC_MISC_HDR_CRDT_CH_10_11_IIO_VTD_STRUCT ItcMiscVTD10_11;
  ITCMINDATTOTCRDTS0_N0_IIO_VTD_STRUCT      ItcMinDatTotCrdts0N0;
  ITCMINDATTOTCRDTS0_N1_IIO_VTD_STRUCT      ItcMinDatTotCrdts0N1;
  ITCMINDATTOTCRDTS1_N0_IIO_VTD_STRUCT      ItcMinDatTotCrdts1N0;
  ITCMINDATTOTCRDTS1_N1_IIO_VTD_STRUCT      ItcMinDatTotCrdts1N1;
  UINT8                                     ItcCredits;
  UINT8                                     StackIndex;
  UINT8                                     MaxIioStackNumber;

  OTCMAXTOTCRDTS_C1_N0_IIO_VTD_STRUCT       OtcMaxTotCrdtsC1N0;
  OTCMAXTOTCRDTS_C1_N1_IIO_VTD_STRUCT       OtcMaxTotCrdtsC1N1;

  MaxIioStackNumber = GetMaxStackNumPerSocket ();
  for (StackIndex = 0; StackIndex < MaxIioStackNumber; StackIndex++) {
    // Avoid programming if the stack does not include any MS2IOSF devices
    if ((IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_NONE) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_RESERVED) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX)) {
      continue;
    }

    //
    // When completion combining is enabled, ordering state can be corrupted for outbound completions and cause a hang.
    // Apply WA for SPR-SP Ax.
    //
    if (IsSiliconWorkaroundEnabled ("S22010113831")) {
      ItcCtrl01N0.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ITCCTRL01_N0_IIO_VTD_REG);
      ItcCtrl01N0.Bits.enable_cmpl_comb = 0;
      IioWriteCpuCsr32 (IioIndex, StackIndex, ITCCTRL01_N0_IIO_VTD_REG, ItcCtrl01N0.Data);
    }

    if (IsSiliconWorkaroundEnabled ("S14011008707")) {
      ItcMiscVTD08_09.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ITC_MISC_HDR_CRDT_CH_08_09_IIO_VTD_REG);
      ItcMiscVTD10_11.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ITC_MISC_HDR_CRDT_CH_10_11_IIO_VTD_REG);

      ItcMiscVTD08_09.Bits.ch08_p_credits = 1;
      ItcMiscVTD08_09.Bits.ch08_c_credits = 1;
      ItcMiscVTD10_11.Bits.ch10_p_credits = 1;
      ItcMiscVTD10_11.Bits.ch10_np_credits = 1;

      IioWriteCpuCsr32 (IioIndex, StackIndex, ITC_MISC_HDR_CRDT_CH_08_09_IIO_VTD_REG, ItcMiscVTD08_09.Data);
      IioWriteCpuCsr32 (IioIndex, StackIndex, ITC_MISC_HDR_CRDT_CH_10_11_IIO_VTD_REG, ItcMiscVTD10_11.Data);
    }
    //
    // IOSF hdr/data credit management incorrectly ignores available resources in ITC.
    // Potential buffer overflow in configurations with one channel enabled (e.g. x16, only one x8) under
    // extremely optimized performance settings.
    // CH*_NP_MIN must match CH*_P_MIN and must be at least 9.
    //
    if (IsSiliconWorkaroundEnabled ("S14010996611")) {
      ItcMinDatTotCrdts0N0.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ITCMINDATTOTCRDTS0_N0_IIO_VTD_REG);
      ItcMinDatTotCrdts0N1.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ITCMINDATTOTCRDTS0_N1_IIO_VTD_REG);
      ItcCredits = (UINT8) ((ItcMinDatTotCrdts0N1.Bits.ch02_p_min << 2) | ItcMinDatTotCrdts0N0.Bits.ch02_p_min);

      if (ItcMinDatTotCrdts0N0.Bits.ch00_p_min != 0) {
        if (ItcMinDatTotCrdts0N0.Bits.ch00_p_min < 9) {
          ItcMinDatTotCrdts0N0.Bits.ch00_p_min = 9;
        }

        if (ItcMinDatTotCrdts0N0.Bits.ch00_p_min != ItcMinDatTotCrdts0N0.Bits.ch00_np_min) {
          ItcMinDatTotCrdts0N0.Bits.ch00_np_min = ItcMinDatTotCrdts0N0.Bits.ch00_p_min;
        }
      }

      if (ItcMinDatTotCrdts0N0.Bits.ch01_p_min != 0) {
        if (ItcMinDatTotCrdts0N0.Bits.ch01_p_min < 9) {
          ItcMinDatTotCrdts0N0.Bits.ch01_p_min = 9;
        }

        if (ItcMinDatTotCrdts0N0.Bits.ch01_p_min != ItcMinDatTotCrdts0N0.Bits.ch01_np_min) {
          ItcMinDatTotCrdts0N0.Bits.ch01_np_min = ItcMinDatTotCrdts0N0.Bits.ch01_p_min;
        }
      }

      if (ItcCredits != 0) {
        if (ItcCredits < 9) {
          ItcCredits = 9;
        }

        if (ItcCredits != ItcMinDatTotCrdts0N1.Bits.ch02_np_min) {
          ItcMinDatTotCrdts0N1.Bits.ch02_np_min = ItcCredits;
        }
        ItcMinDatTotCrdts0N1.Bits.ch02_p_min = ItcCredits >> 2;  // Upper
        ItcMinDatTotCrdts0N0.Bits.ch02_p_min = ItcCredits & 0x3; // Lower
      }

      if (ItcMinDatTotCrdts0N1.Bits.ch03_p_min != 0) {
        if (ItcMinDatTotCrdts0N1.Bits.ch03_p_min < 9) {
          ItcMinDatTotCrdts0N1.Bits.ch03_p_min = 9;
        }

        if (ItcMinDatTotCrdts0N1.Bits.ch03_p_min != ItcMinDatTotCrdts0N1.Bits.ch03_np_min) {
          ItcMinDatTotCrdts0N1.Bits.ch03_np_min = ItcMinDatTotCrdts0N1.Bits.ch03_p_min;
        }
      }

      ItcMinDatTotCrdts1N0.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ITCMINDATTOTCRDTS1_N0_IIO_VTD_REG);
      ItcMinDatTotCrdts1N1.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ITCMINDATTOTCRDTS1_N1_IIO_VTD_REG);
      ItcCredits = (UINT8) ((ItcMinDatTotCrdts1N1.Bits.ch06_p_min << 2) | ItcMinDatTotCrdts1N0.Bits.ch06_p_min);

      if (ItcMinDatTotCrdts1N0.Bits.ch04_p_min != 0) {
        if (ItcMinDatTotCrdts1N0.Bits.ch04_p_min < 9) {
          ItcMinDatTotCrdts1N0.Bits.ch04_p_min = 9;
        }

        if (ItcMinDatTotCrdts1N0.Bits.ch04_p_min != ItcMinDatTotCrdts1N0.Bits.ch04_np_min) {
          ItcMinDatTotCrdts1N0.Bits.ch04_np_min = ItcMinDatTotCrdts1N0.Bits.ch04_p_min;
        }
      }

      if (ItcMinDatTotCrdts1N0.Bits.ch05_p_min != 0) {
        if (ItcMinDatTotCrdts1N0.Bits.ch05_p_min < 9) {
          ItcMinDatTotCrdts1N0.Bits.ch05_p_min = 9;
        }

        if (ItcMinDatTotCrdts1N0.Bits.ch05_p_min != ItcMinDatTotCrdts1N0.Bits.ch05_np_min) {
          ItcMinDatTotCrdts1N0.Bits.ch05_np_min = ItcMinDatTotCrdts1N0.Bits.ch05_p_min;
        }
      }

      if (ItcCredits != 0) {
        if (ItcCredits < 9) {
          ItcCredits = 9;
        }

        if (ItcCredits != ItcMinDatTotCrdts1N1.Bits.ch06_np_min) {
          ItcMinDatTotCrdts1N1.Bits.ch06_np_min = ItcCredits;
        }
        ItcMinDatTotCrdts1N1.Bits.ch06_p_min = ItcCredits >> 2;  // Upper
        ItcMinDatTotCrdts1N0.Bits.ch06_p_min = ItcCredits & 0x3; // Lower
      }

      if (ItcMinDatTotCrdts1N1.Bits.ch07_p_min != 0) {
        if (ItcMinDatTotCrdts1N1.Bits.ch07_p_min < 9) {
          ItcMinDatTotCrdts1N1.Bits.ch07_p_min = 9;
        }

        if (ItcMinDatTotCrdts1N1.Bits.ch07_p_min != ItcMinDatTotCrdts1N1.Bits.ch07_np_min) {
          ItcMinDatTotCrdts1N1.Bits.ch07_np_min = ItcMinDatTotCrdts1N1.Bits.ch07_p_min;
        }
      }

      IioWriteCpuCsr32 (IioIndex, StackIndex, ITCMINDATTOTCRDTS0_N0_IIO_VTD_REG, ItcMinDatTotCrdts0N0.Data);
      IioWriteCpuCsr32 (IioIndex, StackIndex, ITCMINDATTOTCRDTS0_N1_IIO_VTD_REG, ItcMinDatTotCrdts0N1.Data);
      IioWriteCpuCsr32 (IioIndex, StackIndex, ITCMINDATTOTCRDTS1_N0_IIO_VTD_REG, ItcMinDatTotCrdts1N0.Data);
      IioWriteCpuCsr32 (IioIndex, StackIndex, ITCMINDATTOTCRDTS1_N1_IIO_VTD_REG, ItcMinDatTotCrdts1N1.Data);
    }
    //
    // BIOS shall override OTCMAXTOTCRDTS_C1_1_0_0_CFG in every enabled M2IOSF before triggering hw_init.
    // Apply w/a for SPR B0
    //
    if (IsSiliconWorkaroundEnabled ("S22010438890")) {
      OtcMaxTotCrdtsC1N0.Data = IioReadCpuCsr32 (IioIndex, StackIndex, OTCMAXTOTCRDTS_C1_N0_IIO_VTD_REG);
      OtcMaxTotCrdtsC1N1.Data = IioReadCpuCsr32 (IioIndex, StackIndex, OTCMAXTOTCRDTS_C1_N1_IIO_VTD_REG);
      OtcMaxTotCrdtsC1N0.Bits.ch09_c_max=0x0;   //Bits[31:30], Access Type=RW/P, default=0x00000000*
      OtcMaxTotCrdtsC1N1.Bits.ch09_c_max=0x2;  //Bits[6:0], Access Type=RW/P, default=0x00000001*
      IioWriteCpuCsr32 (IioIndex, StackIndex, OTCMAXTOTCRDTS_C1_N0_IIO_VTD_REG, OtcMaxTotCrdtsC1N0.Data);
      IioWriteCpuCsr32 (IioIndex, StackIndex, OTCMAXTOTCRDTS_C1_N1_IIO_VTD_REG, OtcMaxTotCrdtsC1N1.Data);
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
  UINT8                          WaitTime;
  TC_MISC_UCR_SB_M2PCIE_STRUCT   TcMiscUcr;

  MaxIioStackNumber = GetMaxStackNumPerSocket ();
  //
  // Skip HW_INIT for Rlink/DMI by skipping stackIndex 0.
  //
  for (StackIndex = 1; StackIndex < MaxIioStackNumber; StackIndex++) {
    // Avoid programming if the stack does not include any MS2IOSF devices
    if ((IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_NONE) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_RESERVED) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX)) {
      continue;
    }

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
      DEBUG ((DEBUG_ERROR, "\nLABEL_%a_waithwinit_%d_%d:\n", CheckCurrentReset (POST_RESET_POWERGOOD) ? "c" : "w",
        IioIndex, StackIndex));
      DEBUG ((DEBUG_ERROR, "\nmov eax, DWORD PTR ds:[0%8xh]\n",
        UsraGetCsrRegisterAddress (IioIndex, StackIndex, TC_MISC_UCR_SB_M2PCIE_REG)));
      DEBUG ((DEBUG_ERROR, "\nbt  eax, 31\n"));
      DEBUG ((DEBUG_ERROR, "\njc LABEL_%a_waithwinit_%d_%d\n", CheckCurrentReset (POST_RESET_POWERGOOD) ? "c" : "w",
        IioIndex, StackIndex));
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

  for (Index = 0; Index < BankDecoderVerGen2.NumberEntries; Index++) {
    if (BankDecoderTableGen2[Index].BankNumber != BankNumber) {
      continue;
    }

    Data = Ms2iosfRegRead (IioGlobalData, IioIndex, Bus,
                           BankDecoderTableGen2[Index].RegisterSize, BankDecoderTableGen2[Index].RegisterAddress);
    Data = (Data & BankDecoderTableGen2[Index].FieldMask) | BankDecoderTableGen2[Index].StackTypeData.Disable;

    DisableMs2IosfType1BankDecoder (IioIndex, StackIndex, Bus,
                                    BankDecoderTableGen2[Index].BankNumber, BankDecoderTableGen2[Index].RegisterAddress, &Data);
    Ms2iosfRegWrite (IioGlobalData, IioIndex, Bus,
                     BankDecoderTableGen2[Index].RegisterSize, BankDecoderTableGen2[Index].RegisterAddress, Data);
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
  if (IioIndex >= MaxIIO || IioPort >= GetMaxPciePortNumberPerSocket (IioIndex)) {

    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  IIO_D_WARN ("Disabling MS2IOSF gen2 bank decoder not supported\n");
  return EFI_UNSUPPORTED;
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
  UINT32          Data;
  UINT32          OldData;
  UINT8           TableIndex;
  UINT8           BusNumber;

  if (GetCxlStatus (IioIndex, StackIndex) != AlreadyInCxlMode) {
    return EFI_UNSUPPORTED;
  }

  BusNumber = IioGlobalData->IioVar.IioVData.SocketStackBus[IioIndex][StackIndex];

  for (TableIndex = 0; TableIndex < BankDecoderVerGen2.NumberEntries; TableIndex++) {
    OldData = Ms2iosfRegRead (IioGlobalData, IioIndex, BusNumber, BankDecoderTableGen2[TableIndex].RegisterSize,
                           BankDecoderTableGen2[TableIndex].RegisterAddress);
    Data = OldData;
    Data = (Data & BankDecoderTableGen2[TableIndex].FieldMask) | BankDecoderTableGen2[TableIndex].StackTypeData.Pi5CxlMode;
    //
    // Skip programming the already disabled bank decoders
    //
    if (Data == OldData) {
      continue;
    }
    DisableMs2IosfType1BankDecoder (IioIndex, StackIndex, BusNumber, BankDecoderTableGen2[TableIndex].BankNumber,
                                    BankDecoderTableGen2[TableIndex].RegisterAddress, &Data);
    Ms2iosfRegWrite (IioGlobalData, IioIndex, BusNumber, BankDecoderTableGen2[TableIndex].RegisterSize,
                     BankDecoderTableGen2[TableIndex].RegisterAddress, Data);
  }

  return EFI_SUCCESS;
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
  BANK9_BAR_N0_IIO_VTD_STRUCT    PmonLBarBank;
  BANK9_BAR_N1_IIO_VTD_STRUCT    PmonUBarBank;
  BANK10_BAR_N0_IIO_VTD_STRUCT   TelemetryLBarBank;
  BANK10_BAR_N1_IIO_VTD_STRUCT   TelemetryUBarBank;

  //
  // Avoid programming if the stack does not contain OOB MSM.
  // OOB MSM is only on DINO instance 0.
  //
  if (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][IIO_STACK8] != TYPE_DINO) {
    return;
  }

  PmonLBarBank.Data = IioReadCpuCsr32 (IioIndex, 0, PCI_CFG_BAR0_DISC_MSM_PMON_REG) & (~0xF);
  PmonUBarBank.Data = 0;

  TelemetryLBarBank.Data = IioReadCpuCsr32 (IioIndex, 0, PCI_CFG_BAR1_DISC_MSM_PMON_REG) & (~0xF);
  TelemetryUBarBank.Data = 0;

  //
  // Update PMON and Telemetry BAR bank shadow register
  //
  IioWriteCpuCsr32 (IioIndex, IIO_STACK8, BANK9_BAR_N0_IIO_VTD_REG, PmonLBarBank.Data);
  IioWriteCpuCsr32 (IioIndex, IIO_STACK8, BANK9_BAR_N1_IIO_VTD_REG, PmonUBarBank.Data);
  IioWriteCpuCsr32 (IioIndex, IIO_STACK8, BANK10_BAR_N0_IIO_VTD_REG, TelemetryLBarBank.Data);
  IioWriteCpuCsr32 (IioIndex, IIO_STACK8, BANK10_BAR_N1_IIO_VTD_REG, TelemetryUBarBank.Data);
}

/**

  After resource allocation configure MS2IOSF Gen2 for routing transactions.

  @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param IioIndex           - Index of CPU/IIO

**/
VOID
Ms2iosfPostResources (
  IIO_GLOBALS    *IioGlobalData,
  UINT8          IioIndex
  )
{
  UINT8                          StackIndex;
  UINT8                          MaxIioStackNumber;
  UINT8                          Rootbus;
  BANK0_PCICMD_IIO_VTD_STRUCT    PcieCommand;
  BANK0_DEVCON_IIO_VTD_STRUCT    PcieDevCtrl;
  BANK0_PBAS_IIO_VTD_STRUCT      BasePMemLoValue;
  BANK0_PLIM_IIO_VTD_STRUCT      MemPLimLoValue;
  BANK0_PBASU_IIO_VTD_STRUCT     BasePMemHiValue;
  BANK0_PLIMU_IIO_VTD_STRUCT     MemPLimHiValue;
  KTI_STACK_RESOURCE             StackResource;
  UINT64                         BaseMem = 0;
  UINT64                         MemLim  = 0;

  MSR_IA32_DEBUG_INTERFACE_REGISTER  DebugMsr;

  MaxIioStackNumber = GetMaxStackNumPerSocket ();
  for (StackIndex = 0; StackIndex < MaxIioStackNumber; StackIndex++) {
    if ((IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_NONE) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_RESERVED) ||
        (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX)) {
      continue;
    }
    // Disable DFX access in DXE
    IioEnableDisableDfxRegisterAccess (IioIndex, StackIndex, FALSE);
  }
  for (StackIndex = 0; StackIndex < MaxIioStackNumber; StackIndex++) {
    //
    // Avoid programming if the stack does not include a device that needs to be shadowed
    //
    if (!(IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_DINO)) {
      continue;
    }

    GetStackResouce (IioIndex, StackIndex, &StackResource);
    Rootbus = StackResource.BusBase;

    /*
     * Memory Base/Limit - MMIO Base and Limit
     * PCIe Command - BME and ME
     * Device Control - maxpayload.
     */
    BaseMem = ((UINT64) StackResource.MmiohBase.hi << 32) | StackResource.MmiohBase.lo;
    MemLim = ((UINT64) StackResource.MmiohLimit.hi << 32) | StackResource.MmiohLimit.lo;
    if ((MemLim != 0) && (BaseMem != (-1))) {
      MemLim = BaseMem + CPM_MMIO_SIZE - 1;
      BasePMemLoValue.Data = (UINT16) (BaseMem >> 16);
      MemPLimLoValue.Data  = (UINT16) (MemLim >> 16);
      BasePMemHiValue.Data = (BaseMem >> 32);
      MemPLimHiValue.Data  = (MemLim >> 32);

      IioWriteCpuCsr16 (IioIndex, StackIndex, BANK0_PBAS_IIO_VTD_REG, BasePMemLoValue.Data);
      IioWriteCpuCsr16 (IioIndex, StackIndex, BANK0_PLIM_IIO_VTD_REG, MemPLimLoValue.Data);
      IioWriteCpuCsr32 (IioIndex, StackIndex, BANK0_PBASU_IIO_VTD_REG, BasePMemHiValue.Data);
      IioWriteCpuCsr32 (IioIndex, StackIndex, BANK0_PLIMU_IIO_VTD_REG, MemPLimHiValue.Data);
      IIO_D_LOG ("[%d.%d] MS2IOSF CPM BusNumber 0x%X BaseMemH 0x%lX MemLimH 0x%lX\n", IioIndex, StackIndex,
        Rootbus + CPM_BUS_OFFSET, BaseMem, MemLim);

      BaseMem = BaseMem + CPM_MMIO_SIZE;
      MemLim = BaseMem + HQM_MMIO_SIZE - 1;
      BasePMemLoValue.Data = (UINT16) (BaseMem >> 16);
      MemPLimLoValue.Data  = (UINT16) (MemLim >> 16);
      BasePMemHiValue.Data = (BaseMem >> 32);
      MemPLimHiValue.Data  = (MemLim >> 32);

      IioWriteCpuCsr16 (IioIndex, StackIndex, BANK1_PBAS_IIO_VTD_REG, BasePMemLoValue.Data);
      IioWriteCpuCsr16 (IioIndex, StackIndex, BANK1_PLIM_IIO_VTD_REG, MemPLimLoValue.Data);
      IioWriteCpuCsr32 (IioIndex, StackIndex, BANK1_PBASU_IIO_VTD_REG, BasePMemHiValue.Data);
      IioWriteCpuCsr32 (IioIndex, StackIndex, BANK1_PLIMU_IIO_VTD_REG, MemPLimHiValue.Data);
      IIO_D_LOG ("[%d.%d] MS2IOSF HQM BusNumber 0x%X BaseMemH 0x%lX MemLimH 0x%lX\n", IioIndex, StackIndex,
        Rootbus + HQM_BUS_OFFSET, BaseMem, MemLim);
    }

    PcieDevCtrl.Data = IioReadCpuCsr16 (IioIndex, StackIndex, BANK0_DEVCON_IIO_VTD_REG);
    PcieDevCtrl.Bits.max_payload_size = 2; //max_payload_size: 1 = 256, 2 = 512
    IioWriteCpuCsr16 (IioIndex, StackIndex, BANK0_DEVCON_IIO_VTD_REG, PcieDevCtrl.Data);
    PcieDevCtrl.Data = IioReadCpuCsr16 (IioIndex, StackIndex, BANK1_DEVCON_IIO_VTD_REG);
    PcieDevCtrl.Bits.max_payload_size = 2; //max_payload_size: 1 = 256, 2 = 512
    IioWriteCpuCsr16 (IioIndex, StackIndex, BANK1_DEVCON_IIO_VTD_REG, PcieDevCtrl.Data);

    PcieCommand.Data = IioReadCpuCsr16 (IioIndex, StackIndex, BANK0_PCICMD_IIO_VTD_REG);
    PcieCommand.Bits.mse = 1;
    PcieCommand.Bits.bme = 1;
    IioWriteCpuCsr16 (IioIndex, StackIndex, BANK0_PCICMD_IIO_VTD_REG, PcieCommand.Data);
    PcieCommand.Data = IioReadCpuCsr16 (IioIndex, StackIndex, BANK1_PCICMD_IIO_VTD_REG);
    PcieCommand.Bits.mse = 1;
    PcieCommand.Bits.bme = 1;
    IioWriteCpuCsr16 (IioIndex, StackIndex, BANK1_PCICMD_IIO_VTD_REG, PcieCommand.Data);
  }

  IioMs2iosfUpdateOobMsmBarShadowReg (IioGlobalData, IioIndex);
  //
  // Check debug consent before hiding OOBMSM DFX config space from OS.
  //
  DebugMsr.Uint32 = AsmReadMsr32 (MSR_IA32_DEBUG_INTERFACE);
  if (DebugMsr.Bits.Enable == 0) {
    Ms2iosfBankDecoderDisable (IioGlobalData, IioIndex, IIO_STACK8, FABRIC_DECODE_BANK_GEN2);
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
  BANK1_SECBUS_IIO_VTD_STRUCT  Bank1Secbus;
  BANK1_SUBBUS_IIO_VTD_STRUCT  Bank1Subbus;
  UINT8                        RootBus;

  MaxIioStackNumber = GetMaxStackNumPerSocket ();

  for (StackIndex = 0; StackIndex < MaxIioStackNumber; StackIndex++) {
    // Avoid programming if the stack does not include a device that needs to be shadowed
    if (!(IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_DINO)) {
      continue;
    }

    RootBus = IioGlobalData->IioVar.IioVData.SocketStackBaseBusNumber[IioIndex][StackIndex];
    Bank0Secbus.Data = RootBus + CPM_BUS_OFFSET;
    Bank0Subbus.Data = Bank0Secbus.Data;
    Bank1Secbus.Data = RootBus + HQM_BUS_OFFSET;
    Bank1Subbus.Data = Bank1Secbus.Data;

    IIO_D_LOG ("[%d.%d] MS2IOSF CPM BusNumber 0x%X \n", IioIndex, StackIndex, Bank0Secbus.Data);
    IIO_D_LOG ("[%d.%d] MS2IOSF HQM BusNumber 0x%X \n", IioIndex, StackIndex, Bank1Secbus.Data);

    IioWriteCpuCsr8 (IioIndex, StackIndex, BANK0_SECBUS_IIO_VTD_REG, Bank0Secbus.Data);
    IioWriteCpuCsr8 (IioIndex, StackIndex, BANK0_SUBBUS_IIO_VTD_REG, Bank0Subbus.Data);
    IioWriteCpuCsr8 (IioIndex, StackIndex, BANK1_SECBUS_IIO_VTD_REG, Bank1Secbus.Data);
    IioWriteCpuCsr8 (IioIndex, StackIndex, BANK1_SUBBUS_IIO_VTD_REG, Bank1Subbus.Data);
  }
}

/**

  Program MS2IOSF bank decoder Device Control

  @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param IioIndex           - Index of CPU/IIO

**/
VOID
OverrideMs2iosfBankDevCtrl (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  )
{
  UINT8                               StackIndex;
  UINT8                               MaxIioStackNumber;
  BANK0_DEVCON_OFFSET_IIO_VTD_STRUCT  PcieDevCtrlOffset;
  UINT8                               BankIndex;
  UINT16                              BankOffset;

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

    if (IsSiliconWorkaroundEnabled ("S22010986909")) {
      BankOffset = 0;
      for (BankIndex = 0; BankIndex < BANK19_DEC; BankIndex++) {
        PcieDevCtrlOffset.Data = IioReadCpuCsr8 (IioIndex, StackIndex, BANK0_DEVCON_OFFSET_IIO_VTD_REG + BankOffset);
        PcieDevCtrlOffset.Bits.devcon_shadowen = 0;
        IioWriteCpuCsr8 (IioIndex, StackIndex, BANK0_DEVCON_OFFSET_IIO_VTD_REG + BankOffset, PcieDevCtrlOffset.Data);
        if (BankIndex < BANK8_DEC) {
            BankOffset = BankOffset + TYPE1_OFFSET_PER_BANK;
        } else {
          BankOffset = BankOffset + TYPE0_OFFSET_PER_BANK;
        }
      }
    }
  }
}

/**
  Update Trace Hub FW_BAR shadow register. Additional specific programming for 10nm wave 3.

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
  BANK14_BAR_N0_IIO_VTD_STRUCT    FwLBarBank;
  BANK14_BAR_N1_IIO_VTD_STRUCT    FwUBarBank;

  // Avoid programming if the stack does not contain NPK FWBAR
  if (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] != TYPE_DINO) {
    return;
  }

  FwLBarBank.Data = (UINT32) FwBar;
  FwUBarBank.Data = (UINT32) (FwBar >> 32);

  //
  // Update FW_BAR shadow register
  //
  IioWriteCpuCsr32 (IioIndex, StackIndex, BANK14_BAR_N0_IIO_VTD_REG, FwLBarBank.Data);
  IioWriteCpuCsr32 (IioIndex, StackIndex, BANK14_BAR_N1_IIO_VTD_REG, FwUBarBank.Data);
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
  //
  // Not needed in Gen2 MS2IOSF.
  //
  return EFI_SUCCESS;
}

