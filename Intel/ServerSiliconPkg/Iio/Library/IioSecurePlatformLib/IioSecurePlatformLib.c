/** @file
  Implement IIO initialization protocol to provide IIO initialization functions.
  This interfaces will be called by PciHostBridge driver in the PCI bus enumeration.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#include <RcRegs.h>

#include <IndustryStandard/Pci22.h>
#include <Chip/Include/SysHostChip.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/PcuMailBoxLib.h>

#include <IioPlatformData.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <Library/IioSysInfoLib.h>
#include <Library/IioPcieInit.h>
#include <LibraryPrivate/IioDebug.h>

#define Is14nm() (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL))

#define CPX_UBOX_STACK_INDEX  3
#define CPX_M3KTI_CMS_FUNC    3

VOID
IioDevFuncHideM2iosf (
  IN IIO_GLOBALS                *IioGlobalData,
  IN UINT8                      IioIndex,
  IN IIO_DEVFUNHIDE_TABLE       *IioDevFuncHide
  );

VOID
IioDevHideUncore (
  IN IIO_GLOBALS            *IioGlobalData,
  IN UINT8                  IioIndex,
  IN IIO_DEVFUNHIDE_TABLE   *IioDevFuncHide
  );


/**
  This function will update IioDevFuncHide table based on setup knobs:
  - DfxDevHideOnUncoreStacks
  - DfxDevHideOnIioStacks
  If any override for stack is defined the previous value is overridden.

  @param[in]     IioGlobalData   Pointer to IIO_GLOBALS
  @param[in]     IioIndex        Index of Iio
  @param[in]     IsFinalPhase    Point the final phase where all overrides should be applied
  @param[in out] IioDevFuncHide  Pointer to table with devices to hide
**/
VOID
DfxDevHideMenuOverrides (
  IN     IIO_GLOBALS                *IioGlobalData,
  IN     UINT8                      IioIndex,
  IN     BOOLEAN                    IsFinalPhase,
  IN OUT IIO_DEVFUNHIDE_TABLE       *IioDevFuncHide
  )
{
  UINT8        Stack;
  UINT8        FuncNum;

  if (IioIndex >= MaxIIO) {
    ASSERT (FALSE);
    return;
  }

  if (Is14nm ()) {
    //
    // Update values in DevFuncHide table with values from DfxDevHide menu in setup
    // This make sense only for 14nm programs - for 10nm there is per-IP hiding so there are no control it on this way
    //
    for (Stack = 0; Stack < NUM_DEVHIDE_IIO_STACKS; Stack++) {
      for (FuncNum = 0; FuncNum < NUM_DEVHIDE_REGS_PER_STACK; FuncNum++) {
        if (IioGlobalData->SetupData.DfxDevHideOnIioStacks[IioIndex][Stack][FuncNum] != 0) {
          if (IsFinalPhase) {
            //
            // This is a final phase so override settings with DFX devHide menu settings
            //
            IioDevFuncHide[IioIndex].IioStackDevHide[Stack].DevToHide[FuncNum] =
                IioGlobalData->SetupData.DfxDevHideOnIioStacks[IioIndex][Stack][FuncNum];
          } else {
            //
            // This is not a final phase of hiding so just mask-out all devices
            // not planned for hiding in DFX devHide menu - they will be hiding later
            //
            IioDevFuncHide[IioIndex].IioStackDevHide[Stack].DevToHide[FuncNum] &=
                IioGlobalData->SetupData.DfxDevHideOnIioStacks[IioIndex][Stack][FuncNum];
          }
        }
      } // foreach Func
    } // foreach Stack
  } else { // !Is14nm ()
    //
    // Update values in DevFuncHide table with values from DfxDevHide menu in setup for uncore stacks
    //
    for (Stack = 0; Stack < NUM_DEVHIDE_UNCORE_STACKS; Stack++) {
      for (FuncNum = 0; FuncNum < NUM_DEVHIDE_REGS_PER_STACK; FuncNum++) {
        if (IioGlobalData->SetupData.DfxDevHideOnUncoreStacks[IioIndex][Stack][FuncNum] != 0) {
          if (IsFinalPhase) {
            //
            // This is a final phase so override settings with DFX devHide menu settings
            //
            IioDevFuncHide[IioIndex].UncoreStackDevHide[Stack].DevToHide[FuncNum] =
              IioGlobalData->SetupData.DfxDevHideOnUncoreStacks[IioIndex][Stack][FuncNum];
          } else {
            //
            // This is not a final phase of hiding so just mask-out all devices
            // not planned for hiding in DFX devHide menu - they will be hiding later
            //
            IioDevFuncHide[IioIndex].UncoreStackDevHide[Stack].DevToHide[FuncNum] &=
              IioGlobalData->SetupData.DfxDevHideOnUncoreStacks[IioIndex][Stack][FuncNum];
          }
        }
      } // foreach Func
    } // foreach Stack
  }
}

/**
  This function will update IioDevFuncHide Table based on setup values
  for any device in specific (10nm)

  @param[in]     IioGlobalData   Pointer to IIO_GLOBALS
  @param[in]     IioIndex        Index to CPU/IIO
  @param[in out] IioDevFuncHide  Pointer to table with devices to hide
**/
VOID
HideOtherDevicesBeforeEnumerator (
  IN      IIO_GLOBALS                *IioGlobalData,
  IN      UINT8                      IioIndex,
  IN OUT  IIO_DEVFUNHIDE_TABLE       *IioDevFuncHide
  )
{
  UINT8  Stack;
  UINT8  Index;
  UINT16 StackBit;
  UINT8  FuncX;

  if (IioIndex >=  MaxIIO) {
    ASSERT (FALSE);
    return;
  }

  Stack = GetCbDmaStack ();
  for (Index = 0; Index < IOAT_TOTAL_FUNCS; Index++) {
    if (IioGlobalData->SetupData.Cb3DmaEn[(IioIndex * IOAT_TOTAL_FUNCS) + Index] == 0) {
      //
      // Hide CBDMA
      //
      if (Is14nm ()) {
        IioDevFuncHide[IioIndex].IioStackDevHide[Stack].DevToHide[IOAT_FUNC_START + Index] |= BIT4;
      } else {
        IioDevFuncHide[IioIndex].IioStackDevHide[Stack].DevToHide[IOAT_FUNC_START + Index] |= BIT1;
      }
    }
  } // End Index For

  for (Stack = 0; Stack < GetMaxStackNumPerSocket (); Stack++) {
    if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << Stack))) {
      continue;
    }

    if (Is14nm ()) {
      //
      // Hide IoApic if required (only for SKX|CLX|CPX)
      //
      if (IioGlobalData->SetupData.DevPresIoApicIio[(IioIndex * MAX_IIO_STACK)+Stack] == 0) {
        IioDevFuncHide[IioIndex].IioStackDevHide[Stack].DevToHide[APIC_FUNC_NUM] |= BIT5;
        IIO_D_PCIDBG ("[%d.%d] Hide IoApic\n", IioIndex, Stack);
      }
    }

    //
    // Hide VMD devices if IIO port is not configured as VMD on each socket
    //
    if (!IsVMDEnabledForStack (IioIndex, Stack)) {
      if (Is14nm ()) {
        //
        // VMD in SKX|CLX\CPX is on BDF: *:5:5
        //
        IioDevFuncHide[IioIndex].IioStackDevHide[Stack].DevToHide[IIO_VMD_FUNC] |= BIT5;
      } else {
        //
        // VMD in 10nm and later is on BDF: *:0:5
        //
        IioDevFuncHide[IioIndex].IioStackDevHide[Stack].DevToHide[IIO_VMD_FUNC] |= BIT0;
      }
    }
  } // foreach stack

  if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    //
    // For CPX devices 4-7 on Ubox stacks are NOT hidden by KTI (as usually)
    // They are still exposed because of recipe programming and then needed to be hidden
    //
    for (StackBit = 1, Stack = 0; Stack < MAX_IIO_STACK; StackBit <<= 1, Stack++) {

      if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & StackBit)) {
        //
        // This is Ubox stack - hide all funcs for devices 4-7
        //
        for (FuncX = 0; FuncX <= 7; FuncX++) {
          IioDevFuncHide[IioIndex].IioStackDevHide[Stack].DevToHide[FuncX] |= BIT4 | BIT5 | BIT6 | BIT7;
        } // for (FuncX...)
      }
    } // for each stack

    //
    // Hide M3KTI23/45 devs: 19/20 func: 3 in UBOX stacks as this is not supported in OS
    //
    IioDevFuncHide[IioIndex].IioStackDevHide[CPX_UBOX_STACK_INDEX].DevToHide[CPX_M3KTI_CMS_FUNC] |= BIT19 | BIT20;
  }
}

/**
  Check and hide another devices after enumeration

  @param[in]     IioGlobalData   Pointer to IIO_GLOBALS
  @param[in]     IioIndex        Index to CPU/IIO
  @param[in out] IioDevFuncHide  Pointer to table with devices to hide
**/
VOID
HideOtherDevicesAfterEnumerator (
  IN     IIO_GLOBALS           *IioGlobalData,
  IN     UINT8                 IioIndex,
  IN OUT IIO_DEVFUNHIDE_TABLE  *IioDevFuncHide
  )
{
  if (Is14nm ()) {
    // Hide PCU F6 if required
    if (IioGlobalData->SetupData.PCUF6Hide != 0){
      IioDevFuncHide[IioIndex].IioStackDevHide[IIO_PSTACK0].DevToHide[PCU22_FUNC_NUM] |= BIT30;
    }
  }
}

/**
  This function will Hide all Devices in a Socket
  based on IioDevFuncHide Table after Resource Allocation

  @param[in]     IioGlobalData   Pointer to IIO_GLOBALS
  @param[in]     IioIndex        Index to CPU/IIO
  @param[in]     Phase           Phase in IIO flow
  @param[in out] IioDevFuncHide  Pointer to table with devices to hide
**/
VOID
IioDevicesHide (
  IN     IIO_GLOBALS           *IioGlobalData,
  IN     UINT8                 IioIndex,
  IN     IIO_INIT_ENUMERATION  Phase,
  IN OUT IIO_DEVFUNHIDE_TABLE  *IioDevFuncHide
  )
{
  IIO_D_PCILOG ("[%d] Hide devices; Phase = %x\n", IioIndex, Phase);

  //
  // Skip hiding anything here in EV mode. VMD must be always hidden if not in use.
  // Running EDK PCI Enumerator on not initialized VMD device causes that all devices
  // in its stack get hidden.
  //
  if ((IioGlobalData->IioVar.IioVData.EVMode) && (Is14nm () || Phase == IioAfterResources)) {
    //
    // Everything should stay not hidden in EV mode but this makes a problem in ICX
    // so for now it works for 10nm as before - skip hiding only in AfterResoureces phase
    //
    IIO_D_LOG ("[%d] %a: nothing is hidden in EV mode.\n", IioIndex, __FUNCTION__);
    return;
  }

  switch (Phase) {
    case IioBeforeResources:
      //
      // Allow PCIe code to decide which rootports should be hidden
      //
      IioRootportsHide (IioGlobalData, IioIndex, IioDevFuncHide);

      //
      // Check if CBDMA, IOAPIC and VMD also should be hidden
      //
      HideOtherDevicesBeforeEnumerator (IioGlobalData, IioIndex, IioDevFuncHide);
      break;
    case IioEndOfPei:
      //
      // This can be called on EndOfPei from VMD init function only for 14nm cpus (SKX|CLX|CPX)
      // This is necessary to hide rootports assigned to VMD
      //
      if (!Is14nm ()) {
        return;
      }
      break;
    case IioAfterResources:

      HideOtherDevicesAfterEnumerator (IioGlobalData, IioIndex, IioDevFuncHide);

      break;
    default:
      ASSERT (FALSE);
      return;
  }

  //
  // At the end check if DFX devHide menu should override something here
  //
  DfxDevHideMenuOverrides (IioGlobalData, IioIndex, Phase == IioAfterResources, IioDevFuncHide);


  if (!Is14nm ()) {
    //
    // Hide BDFs under M2IOSF stacks
    //
    IioDevFuncHideM2iosf (IioGlobalData, IioIndex, IioDevFuncHide);
  }

  //
  // Hide BDFs in UBOX
  // - for 14nm cpus: Hides all devs from IioDevFuncHide in UBOX|VCU|PCU registers
  // - other:         Hides only uncore stacks devices in UBOX registers
  //
  IioDevHideUncore (IioGlobalData, IioIndex, IioDevFuncHide);
}


/**
 @brief Remove BAR configuration from PCI device that is going to be hidden.

 @param[in] IioIndex - Index to IIO instance
 @param[in] Seg      - PCI segment number
 @param[in] Bus      - PCI bus number
 @param[in] Dev      - PCI device number
 @param[in] Func     - PCI function number
**/
VOID
IioPciDevUnconfigure (
  IN     UINT8  IioIndex,
  IN     UINT8  Seg,
  IN     UINT8  Bus,
  IN     UINT8  Dev,
  IN     UINT8  Func
  )
{
  UINT32 BarReg;
  UINT16 CmdReg;
  UINT8  HdrReg;
  UINT8  BarOff, MaxBarOff;

  HdrReg = IioPciExpressRead8 (IioIndex, Bus, Dev, Func, PCI_HEADER_TYPE_OFFSET);
  switch (HdrReg & HEADER_LAYOUT_CODE) {
    case HEADER_TYPE_DEVICE:
      MaxBarOff = PCI_BASE_ADDRESSREG_OFFSET + PCI_BAR_IDX5 * sizeof (UINT32);
      break;
    case HEADER_TYPE_PCI_TO_PCI_BRIDGE:
      MaxBarOff = PCI_BASE_ADDRESSREG_OFFSET + PCI_BAR_IDX1 * sizeof (UINT32);
      break;
    default:
      return;
  }
  CmdReg = IioPciExpressRead16 (IioIndex, Bus, Dev, Func, PCI_COMMAND_OFFSET);
  if (CmdReg != 0xFFFF &&
      (CmdReg & (EFI_PCI_COMMAND_IO_SPACE | EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER)) != 0) {

    IioPciExpressWrite16 (IioIndex, Bus, Dev, Func, PCI_COMMAND_OFFSET,
                          CmdReg & ~(EFI_PCI_COMMAND_IO_SPACE | EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER));
  }
  for (BarOff = PCI_BASE_ADDRESSREG_OFFSET; BarOff < MaxBarOff; BarOff +=4) {

    BarReg = IioPciExpressRead32 (IioIndex, Bus, Dev, Func, BarOff);
    IioPciExpressWrite32 (IioIndex, Bus, Dev, Func, BarOff, BarReg & 0xF);
    if ((BarReg & 7) == 4) { // if 64-bit MMIO

      BarOff += 4;
      IioPciExpressWrite32 (IioIndex, Bus, Dev, Func, BarOff, 0);
    }
  }
} // IioPciDevUnconfigure()

