/** @file
  This file initialize the IoApic device on IIO

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation. <BR>

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

#include <Upi/KtiHost.h>
#include <RcRegs.h>

#include <Library/DebugLib.h>
#include <Library/IioAccessLib.h>
#include <Library/IioIoApicInit.h>
#include <Library/SystemInfoLib.h>

#include <LibraryPrivate/IioDebug.h>


/**
    Enable IOxAPIC: If the stack is present enables Memory space and Bus Master, making the
    IOxAPIC PCI area visible.

    @param IioGlobalData - pointer to IIO_GLOBALS
    @param IioIndex - IIO (Socket) index number.
    @param Stack    - Stack number.

**/
VOID
Enable_IOxAPIC (
  IN  IIO_GLOBALS     *IioGlobalData,
  IN  UINT8           IioIndex,
  IN  UINT8           Stack
  )
{
  PCICMD_IIO_IOAPIC_STRUCT PciCmd_Iio_Ioapic;

  if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << Stack))) {
    return;
  }

  IIO_D_DBG ("Enable IIO[%d] IOxAPIC\n", IioIndex);

  PciCmd_Iio_Ioapic.Data = IioReadCpuCsr16 (IioIndex, Stack, PCICMD_IIO_IOAPIC_REG);
  PciCmd_Iio_Ioapic.Bits.mse = 1;
  PciCmd_Iio_Ioapic.Bits.bme = 1;
  IioNonS3WriteCpuCsr16 (IioIndex, Stack, PCICMD_IIO_IOAPIC_REG, PciCmd_Iio_Ioapic.Data);
}

/**
    Post initialization IO APIC. To be called after initialization completes.

    @param IioGlobalData - pointer to IIO_GLOBALS
    @param IioIndex - IIO (Socket) index number.
**/
VOID
IioIoApicInit (
  IN  IIO_GLOBALS               *IioGlobalData,
  IN  UINT8                     IioIndex
  )
{
  UINT32    IoApicAddress=0;
  UINT16    Data16;
  UINT8     Stack;

  VID_IIO_IOAPIC_STRUCT     Vid_Iio_Ioapic;
  SVID_IIO_IOAPIC_STRUCT    Svid_Iio_Ioapic;
  DID_IIO_IOAPIC_STRUCT     Did_Iio_Ioapic;
  TOMMIOL_OB_IIO_VTD_STRUCT Tommiol_Ob_Iio_Vtd;
  PMCAP_IIO_IOAPIC_STRUCT   Pmcap_Iio_Ioapic;
  SDID_IIO_IOAPIC_STRUCT    Sdid_Iio_Ioapic;
  IIOMISCCTRL_N1_IIO_VTD_STRUCT IioMisCCtrlN1;


  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    // Check if the Stack is present.
    if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1<<Stack))) {
      continue;
    }

    // Skip enabling IOAPIC on specific disabled stack
    if (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][Stack] == TYPE_DISABLED) {
      continue;
    }

    IoApicAddress = (UINT32)IioGlobalData->IioVar.IioVData.IoApicBase[IioIndex][Stack];
    if (IoApicAddress == (UINT32)IioGlobalData->IioVar.IioVData.PchIoApicBase) {
      IoApicAddress = IoApicAddress + 0x1000;       // dont collide with PCH IOAPIC!!
    }

    //
    // Read enable/disable from Setup for this IIO
    //

    if (IioGlobalData->SetupData.DevPresIoApicIio[(IioIndex * MAX_IIO_STACK) + Stack]) {
      IIO_D_DBG ("Initialize IIO[%d][%d] IOxAPIC...\n", IioIndex,Stack);
    }

    //
    // Set IOAPIC ABAR
    //
    Data16 = B_APIC_ABAR_ABAR_EN | (0x00000FFF & (UINT16)(IoApicAddress >> 8));
    IioWriteCpuCsr16 (IioIndex,Stack,ABAR_IIO_IOAPIC_REG, Data16);
    IIO_D_DBG ("IIO[%d], Stack=%d, IOxAPIC Base=%X\n",IioIndex,Stack,IoApicAddress);

    //
    // Enable accesses by us and the OS to the IOAPIC register address area from 0xFC00_0000 to 0xFEFF_FFFF
    // Block accesses from 0xFF00_0000 to 0xFFFF_FFFF by anybody but us
    //
    Tommiol_Ob_Iio_Vtd.Data = IioReadCpuCsr32 (IioIndex,Stack, TOMMIOL_OB_IIO_VTD_REG);
    Tommiol_Ob_Iio_Vtd.Bits.tommiol_ob = 0xFEF;
    IioWriteCpuCsr32 (IioIndex, Stack, TOMMIOL_OB_IIO_VTD_REG, Tommiol_Ob_Iio_Vtd.Data);
    IIO_D_DBG ("IIO[%d] Stack[%d] TOMMIOL_OB = %08X\n",IioIndex,Stack, Tommiol_Ob_Iio_Vtd.Data);

    if (FeaturePcdGet (PcdLockCsrSsidSvidRegister)) {
      //
      // Lock Subsystem Vendor ID
      //

      //
      // 4927266: BARS not filled in for IOxAPICS on the p-stacks 1-5
      // Various register related to IOxAPICs on the p-stacks 1-5 were not initialized
      //
      Vid_Iio_Ioapic.Data = IioReadCpuCsr16 (IioIndex,Stack,VID_IIO_IOAPIC_REG);
      Svid_Iio_Ioapic.Data = IioReadCpuCsr16 (IioIndex,Stack,SVID_IIO_IOAPIC_REG);
      //
      // Update Subsystem Vendor ID with the same value as Vendor ID
      //
      if (Svid_Iio_Ioapic.Data != Vid_Iio_Ioapic.Data) {
        Svid_Iio_Ioapic.Data = Vid_Iio_Ioapic.Data;
      }
      IioWriteCpuCsr16 (IioIndex,Stack,SVID_IIO_IOAPIC_REG,Svid_Iio_Ioapic.Data);
      //
      // Lock Subsystem Device ID
      //
      //
      // 4927266: BARS not filled in for IOxAPICS on the p-stacks 1-5
      // Device id values need to be set for for c-stack and p-stacks;
      // the value for the p-stacks is different from the c-stack.
      //
      if (!(IioGlobalData->IioVar.IioVData.Emulation & SIMICS_FLAG)) {
        if (Stack == IIO_CSTACK) {
          Did_Iio_Ioapic.Data = 0x2026;
        } else {
          Did_Iio_Ioapic.Data = 0x2036;
        }
      } else {
        Did_Iio_Ioapic.Data = IioReadCpuCsr16 (IioIndex,Stack,DID_IIO_IOAPIC_REG);
      }

      Sdid_Iio_Ioapic.Data = IioReadCpuCsr16 (IioIndex,Stack,SDID_IIO_IOAPIC_REG);
      //
      // Update Subsystem Device ID with the same value as Device ID
      //
      Sdid_Iio_Ioapic.Data = Did_Iio_Ioapic.Data;
      IioWriteCpuCsr16 (IioIndex,Stack, SDID_IIO_IOAPIC_REG, Sdid_Iio_Ioapic.Data);
    }
    //
    // Lock PMCAP
    //
    Pmcap_Iio_Ioapic.Data = IioReadCpuCsr32 (IioIndex, Stack, PMCAP_IIO_IOAPIC_REG);
    IioWriteCpuCsr32 (IioIndex, Stack, PMCAP_IIO_IOAPIC_REG, Pmcap_Iio_Ioapic.Data);

    //
    // s3875843:
    // (Interrupts dropped intermittently due to faulty workround)
    // Note: applicable to IVT
    //
    IioMisCCtrlN1.Data = IioReadCpuCsr32 (IioIndex, Stack, IIOMISCCTRL_N1_IIO_VTD_REG);
    IioMisCCtrlN1.Bits.disable_new_apic_ordering = 1;
    IioWriteCpuCsr32 (IioIndex, Stack, IIOMISCCTRL_N1_IIO_VTD_REG, IioMisCCtrlN1.Data);
  }
}

/**

    Boot event handler. Enable IOxAPIC before passing control to OS.

    @param IioGlobalData - Pointer to IIO_GLOBALS
    @param IioIndex      - Index to the current IIO (Socket)

    @retval None

**/
VOID
IioIoApicInitBootEvent (
  IN  IIO_GLOBALS         *IioGlobalData,
  IN  UINT8               IioIndex
  )
{
  UINT8     Stack;

  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    if (IioGlobalData->SetupData.DevPresIoApicIio[(IioIndex * MAX_IIO_STACK) + Stack]) {
      Enable_IOxAPIC (IioGlobalData, IioIndex, Stack);
    }
  }
}
