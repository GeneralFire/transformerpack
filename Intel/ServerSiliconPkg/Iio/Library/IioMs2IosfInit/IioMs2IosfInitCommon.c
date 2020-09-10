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
#include "Include/IioBankDecoderCommon.h"
#include "Include/IioCreditsCommon.h"
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
#include <Library/CheckpointLib.h>

extern
VOID
OverrideMs2iosfShadowedBar (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

extern
VOID
OverrideMs2iosfEoiBroadcast (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

extern
VOID
OverrideMs2iosfMctpBroadcast (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

extern
VOID
Ms2iosfEnqCapEnable (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

extern
VOID
OverrideMs2iosfBarSzForNTB (
  IIO_GLOBALS                    *IioGlobalData,
  UINT8                          IioIndex
  );

extern
VOID
OverrideMs2iosfBankDevCtrl (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

extern
VOID
ProgramItcCredits (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

extern
VOID
ProgramOtcCredits (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

extern
VOID
ProgramMs2IosfCreditsMisc (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

extern
VOID
TriggerCreditsHwInit (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

extern
VOID
OverrideMs2iosfShadowedBus (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

extern
VOID
ProgramMs2IosfBankDecoderBanks (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

/**
  Check if this MS2IOSF Bank Decoder Gen2 support.

  @param   NONE

  @retval  TRUE    support Bank Decoder Gen2
           FALE    not support Bank Decoder Gen2
**/
BOOLEAN
EFIAPI
IsMs2IosfGen2 (
  VOID
  )
{
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    return TRUE;
  }
  return FALSE;
}

/**
  Check if CPU supports the flow for HW_INIT to be triggered.

  @param   NONE

  @retval  TRUE    supports HW_INIT to be triggered
           FALE    Doesn't support HW_INIT trigger flow
**/
BOOLEAN
EFIAPI
IsHwInitSupported (
  VOID
  )
{
  if (IsSiliconWorkaroundEnabled ("S14010451750")) {
     return FALSE;
  }

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL) || IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**

  Apply silicon permanent workaround for disabled MS2IOSF Type 1 bank decoder.

  @param IioIndex           - index to IIO (socket number)
  @param StackIndex         - Stack index value of CPU/IIO
  @param BusNumber          - Bus number for the given stack
  @param BankNumber         - Bank number being programmed
  @param RegisterAddress    - Bank register address being accessed
  @param DataPtr            - Current data value that will be programmed by ProgramMs2IosfBankDecoder function

**/
VOID
DisableMs2IosfType1BankDecoder (
  IN UINT8        IioIndex,
  IN UINT8        StackIndex,
  IN UINT8        BusNumber,
  IN UINT8        BankNumber,
  IN UINT32       RegisterAddress,
  IN OUT UINT32  *DataPtr
  )
{
  UINT16  BankRegister;
  UINT16  BankRegisterData;

  // Verify the bank the workaround is needed and bank decoder enable is cleared
  BankRegister = BankNumber * TYPE1_OFFSET_PER_BANK + TYPE1_BANK_START + BANKCTRL_OFFSET;
  if (BankNumber > BANK7_DEC || RegisterAddress != BankRegister || (*DataPtr & BANK_DECODE_ENABLE) == BANK_DECODE_ENABLE) {
    return;
  }

  BankRegister = BankNumber * TYPE1_OFFSET_PER_BANK + TYPE1_BANK_START + BCTRL_OFFSET;
  BankRegisterData = IioPciExpressRead16 (IioIndex, BusNumber, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, BankRegister);
  BankRegisterData &= ~VGAEN;
  IioPciExpressWrite16 (IioIndex, BusNumber, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, BankRegister, BankRegisterData);

  BankRegister = BankNumber * TYPE1_OFFSET_PER_BANK + TYPE1_BANK_START + PCICMD_OFFSET;
  BankRegisterData = IioPciExpressRead16 (IioIndex, BusNumber, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, BankRegister);
  BankRegisterData &= ~(EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_IO_SPACE);
  IioPciExpressWrite16 (IioIndex, BusNumber, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, BankRegister, BankRegisterData);

  *DataPtr &= ~MC_BANK_DECODE_ENABLE;
}

/**
 @brief Read register described by Bank Decoder Table entry.

 @param[in] IioGlobalPtr - Pointer to IIO global data structure
 @param[in] IioIndex     - Index of IIO instance
 @param[in] PciBus       - PCI bus (IIO stack) to ready from MS2IOSF device
 @param[in] RegSize      - Size of the register to read
 @param[in] RegAddr      - Address to read (offset in MS2IOSF device config space)

 @return Register value is returned.
**/
UINT32
Ms2iosfRegRead (
  IN IIO_GLOBALS *IioGlobalData,
  IN UINT8        IioIndex,
  IN UINT8        PciBus,
  IN UINT8        RegSize,
  IN UINT32       RegAddr)
{
  UINT32 Data;
  //
  // Check register size and read data.
  //
  switch (RegSize) {
  case sizeof(UINT32):
    Data = IioPciExpressRead32 (IioIndex, PciBus, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, RegAddr);
    break;
  case sizeof(UINT16):
    Data = IioPciExpressRead16 (IioIndex, PciBus, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, RegAddr);
    break;
  case sizeof(UINT8):
    Data = IioPciExpressRead8 (IioIndex, PciBus, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, RegAddr);
    break;
  default:
    IIO_D_ERR ("%02X:%02X: Unsupported size %d for MS2IOSF register 0x%08X\n",
               IioGlobalData->IioVar.IioVData.SegmentSocket[IioIndex], PciBus, RegSize, RegAddr);
    ASSERT (FALSE);
    Data = 0;
    break;
  }
  return Data;
}


/**
 @brief Write register described by Bank Decoder Table entry.

 @param[in] IioGlobalPtr - Pointer to IIO global data structure
 @param[in] IioIndex     - Index of IIO instance
 @param[in] PciBus       - PCI bus (IIO stack) to ready from MS2IOSF device
 @param[in] RegSize      - Size of the register to read
 @param[in] RegAddr      - Address to read (offset in MS2IOSF device config space)
**/
VOID
Ms2iosfRegWrite (
  IN IIO_GLOBALS *IioGlobalData,
  IN UINT8        IioIndex,
  IN UINT8        PciBus,
  IN UINT8        RegSize,
  IN UINT32       RegAddr,
  IN UINT32       Data)
{
  switch (RegSize) {
  case sizeof(UINT32):
    IioPciExpressWrite32 (IioIndex, PciBus, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, RegAddr, Data);
    break;
  case sizeof(UINT16):
    IioPciExpressWrite16 (IioIndex, PciBus, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, RegAddr, (UINT16)Data);
    break;
  case sizeof(UINT8):
    IioPciExpressWrite8 (IioIndex, PciBus, MS2IOSF_DEV_NUM, MS2IOSF_FUNC_NUM, RegAddr, (UINT8)Data);
    break;
  default:
    IIO_D_ERR ("%02X:%02X: Unsupported size %d for MS2IOSF register 0x%08X\n",
               IioGlobalData->IioVar.IioVData.SegmentSocket[IioIndex], PciBus, RegSize, RegAddr);
    ASSERT (FALSE);
    break;
  }
}

/**
  Program capability offsets in pcie controller in stack.
  Capability offsets have default HW values for SDG-gen3 controller.
  It needs to be set for EIP-gen4.

  mc_offset should points to register:
    - SDG gen3:           MCAST_CTRL_IIO_PCIE_REG  = 0x030E (HW default)
    - EIP gen4:           MCSTCTL_IIO_PCIEGen4_REG = 0x0156
    - EIP gen5 (PI5):     MCSTCTL_IIO_PCIE_G5_REG  = 0x022E

  @param      IioIndex           Current socket.
 */
#define MCSTCTL_EIP_GEN4_OFFSET  0x0156
#define MCSTCTL_EIP_PI5_OFFSET   0x022E

VOID
ProgrammCapOffsets (
    IN  UINT8                 IioIndex
  )
{
  CAP_STRUCT_OFFSETS_IIO_VTD_STRUCT      CapStructOffsets;
  UINT8                                  StackIndex;
  UINT8                                  MaxIioStackNumber;

  MaxIioStackNumber = GetMaxStackNumPerSocket ();
  for (StackIndex = 0; StackIndex < MaxIioStackNumber; StackIndex++) {
    if (IsDmiStack (StackIndex)) {
      //
      // DMI doesn't need this settings
      //
      return;
    }

    CapStructOffsets.Data = IioReadCpuCsr32 (IioIndex, StackIndex, CAP_STRUCT_OFFSETS_IIO_VTD_REG);

    //
    // Update mc_offset only for ICX-SP,ICX-D and SPR - no changes needed for gen3 programs
    //
    if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
      CapStructOffsets.Bits.mc_offset = MCSTCTL_EIP_GEN4_OFFSET;

    } else if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
      CapStructOffsets.Bits.mc_offset = MCSTCTL_EIP_PI5_OFFSET;
    }

    IioWriteCpuCsr32 (IioIndex, StackIndex, CAP_STRUCT_OFFSETS_IIO_VTD_REG, CapStructOffsets.Data);
  }
}

VOID
ProgramMs2IosfBankDecoder (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  )
{
  OutputCheckpointSocket (IioIndex, STS_IIO_EARLY_INIT_ENTRY, STS_IIO_MS2IOSF_BANK_DECODE_ENTRY, 0xFF);
  ProgramMs2IosfBankDecoderBanks (IioGlobalData, IioIndex);
  OverrideMs2iosfShadowedBar (IioGlobalData, IioIndex);
  OverrideMs2iosfEoiBroadcast (IioGlobalData, IioIndex);
  OverrideMs2iosfMctpBroadcast (IioGlobalData, IioIndex);
  OverrideMs2iosfBarSzForNTB (IioGlobalData, IioIndex);
  OverrideMs2iosfBankDevCtrl (IioGlobalData, IioIndex);
  Ms2iosfEnqCapEnable (IioGlobalData, IioIndex);

  ProgrammCapOffsets (IioIndex);
  OutputCheckpointSocket (IioIndex, STS_IIO_EARLY_INIT_ENTRY, STS_IIO_MS2IOSF_BANK_DECODE_EXIT, 0xFF);
}

/**
    ProgramMs2IosfCredits: Main function to program and override all MS2IOSF ITC/OTC credits

    @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
    @param IioIndex           - Index of CPU/IIO

    @retval None
**/
VOID
ProgramMs2IosfCredits (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  )
{
  //
  // Program ITC/OTC credits given on credits tables
  //
  OutputCheckpointSocket (IioIndex, STS_IIO_EARLY_INIT_ENTRY, STS_IIO_MS2IOSF_CREDITS_ENTRY, 0xFF);
  ProgramItcCredits (IioGlobalData, IioIndex);
  ProgramOtcCredits (IioGlobalData, IioIndex);
  ProgramMs2IosfCreditsMisc (IioGlobalData, IioIndex);

  if (IsHwInitSupported ()) {
    TriggerCreditsHwInit (IioGlobalData, IioIndex);
  } else {
    SetSysResetRequired (GetSysResetRequired () | POST_RESET_WARM);
  }

  OutputCheckpointSocket (IioIndex, STS_IIO_EARLY_INIT_ENTRY, STS_IIO_MS2IOSF_CREDITS_EXIT, 0xFF);
}

/**

ConfigureStackWihoutRP: This function configure MS2IOSF for routing transactions to them. (Case stack X + 1 )

@param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
@param IioIndex           - Index of CPU/IIO

@retval None

**/
VOID
OverrideMs2iosfWithoutRP (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  )
{
  OverrideMs2iosfShadowedBus (IioGlobalData, IioIndex);
}

/**

OverrideMs2iosfBarSzOnPcie: This function re-configures BAR size for banks decoding PCIE ports
                            (supporting EMPPTMBAR).

@param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
@param IioIndex           - Index of CPU/IIO

@retval None

**/
VOID
OverrideMs2iosfBarSzOnPcie (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  )
{
  BANK8_BARSIZE_IIO_VTD_STRUCT   BarSzBank;
  UINT32                         RegOffset;
  UINT64                         ExpPtMbarSize;
  UINT8                          MaxStackPort;
  UINT8                          MaxIioStackNumber;
  UINT8                          MaxPortNumberPerSocket;
  UINT8                          StackIndex;
  UINT8                          BitSize = 0;
  UINT8                          PortIndex = 0;
  UINT8                          Port = 0;

  MaxIioStackNumber = GetMaxStackNumPerSocket ();
  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];

  for (StackIndex = IIO_STACK1; StackIndex < MaxIioStackNumber; StackIndex++) {
    if (IfStackPresent (IioIndex, StackIndex) == FALSE) {
      continue;
    }
    MaxStackPort = GetMaxPortNumPerStack (StackIndex);
    if (MaxStackPort == 0) {
      continue;
    }
    for (PortIndex = 0; PortIndex < MaxStackPort; PortIndex++) {
      Port = GetPortIndexbyStack (StackIndex, PortIndex);
      if ((IioVData_ConfigurePciePort (IioGlobalData, IioIndex, Port) == FALSE) ||
          (IioGlobalData->SetupData.PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + Port] == TRUE)) {
        continue;
      }

      ExpPtMbarSize = GetPortBarSize (IioGlobalData, IioIndex, Port);
      BitSize = 0;
      while (!(ExpPtMbarSize & 0x1)) {
        ExpPtMbarSize = RShiftU64(ExpPtMbarSize, 1);
        BitSize++;
      }
      if (IsMs2IosfGen2()) {
        if (Port & BIT0) {
          RegOffset = BANK8_BARSIZE_IIO_VTD_REG + ((PortIndex / 2) * TYPE0_OFFSET_PER_BANK);
        } else {
          RegOffset = BANK15_BARSIZE_IIO_VTD_REG - ((PortIndex / 2) * TYPE0_OFFSET_PER_BANK);
        }
      } else {
        RegOffset = BANK8_BARSIZE_IIO_VTD_REG + (PortIndex * TYPE0_OFFSET_PER_BANK);
      }
      BarSzBank.Data = IioReadCpuCsr8 (IioIndex, StackIndex, RegOffset);
      BarSzBank.Bits.size = BitSize;
      IioWriteCpuCsr8 (IioIndex, StackIndex, RegOffset, BarSzBank.Data);
    }
  }
}


/**
  OverrideMs2iosfBankDecoder: This function configures overrides for bank decoders of MS2IOSF

  @param[in] IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param[in] IioIndex           - Index of CPU/IIO
**/
VOID
OverrideMs2iosfBankDecoder (
  IN  IIO_GLOBALS                      *IioGlobalData,
  IN  UINT8                            IioIndex
  )
{
  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    OverrideMs2iosfBarSzOnPcie (IioGlobalData, IioIndex);
  }

  if (IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    OverrideMs2iosfWithoutRP (IioGlobalData, IioIndex);
  }
}

/**

  IIO Stack Initialize

  @param Phase         - Initialization phase (pre, init, post).
  @param IioGlobalData - Pointer to IIO global data/methods
  @param IioIndex      - Current socket.
  @param StackIndex    - Stack number being initialized.

**/
VOID
IioStackInit (
  IN  IIO_INIT_PHASE                 Phase,
  IN  IIO_GLOBALS                    *IioGlobalData,
  IN  UINT8                          IioIndex,
  IN  UINT8                          StackIndex
  )
{
  Ms2iosfChannelMiscInit (Phase, IioGlobalData, IioIndex, StackIndex);
}
