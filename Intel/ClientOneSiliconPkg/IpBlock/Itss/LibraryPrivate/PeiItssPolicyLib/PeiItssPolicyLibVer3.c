/** @file
  This file is PeiPchPolicy library Emmitsburg specific.

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
//
// @todo: VER3 ITSS THIS FILE IS NOT FINISHED
// This whole file should be reviewed for VER3 once requierments will be formed
//

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PchInfoLib.h>
#include <ConfigBlock.h>
#include <InterruptConfig.h>

/**
  mDevIntConfig[] table contains data on INTx and IRQ for each device.
  IRQ value for devices which use ITSS INTx->PIRQx mapping need to be set in a way
  that for each multifunctional Dxx:Fy same interrupt pins must map to the same IRQ.
  Those IRQ values will be used to update ITSS.PIRx register.
  In APIC relationship between PIRQs and IRQs is:
  PIRQA -> IRQ16
  PIRQB -> IRQ17
  PIRQC -> IRQ18
  PIRQD -> IRQ19
  PIRQE -> IRQ20
  PIRQF -> IRQ21
  PIRQG -> IRQ22
  PIRQH -> IRQ23

  Devices which use INTx->PIRQy mapping are: cAVS(in PCI mode), SMBus, GbE, TraceHub, PCIe,
  SATA, HECI, IDE-R, KT Redirection, xHCI, Thermal Subsystem, Camera IO Host Controller

  PCI Express Root Ports mapping should be programmed only with values as in below table (D27/28/29)
  otherwise _PRT methods in ACPI for RootPorts would require additional patching as
  PCIe Endpoint Device Interrupt is further subjected to INTx to PIRQy Mapping

  Configured IRQ values are not used if an OS chooses to be in PIC instead of APIC mode
**/
GLOBAL_REMOVE_IF_UNREFERENCED PCH_DEVICE_INTERRUPT_CONFIG mDevIntConfig[] = {
//  {31, 0, PchNoInt, 0}, // LPC/eSPI Interface, doesn't use interrupts
//  {31, 1, PchNoInt, 0}, // P2SB, doesn't use interrupts
//  {31, 2, PchNoInt, 0}, // PMC , doesn't use interrupts
    {31, 3, PchIntA, 16}, // cAVS
    {31, 4, PchIntA, 16}, // SMBus Controller
//  {31, 5, PchNoInt, 0}, // SPI , doesn't use interrupts
    {31, 6, PchIntA, 16}, // GbE Controller, INTA is default, programmed in PciCfgSpace 3Dh
    {31, 7, PchIntA, 16}, // NPK
    {30, 0, PchIntA, 16}, // HSUART0
    {30, 1, PchIntB, 17}, // HSUART1
    {30, 4, PchIntA, 16}, // SMBus 1
    {29, 0, PchIntA, 16}, // PCIE_RP[15]
    {28, 0, PchIntA, 16}, // PCIE_RP[14]
    {27, 0, PchIntA, 16}, // PCIE_RP[13]
    {26, 0, PchIntA, 16}, // PCIE_RP[12]
    {25, 0, PchIntA, 16}, // SATA Controller 2
    {24, 0, PchIntA, 16}, // SATA Controller 1
    {23, 0, PchIntA, 16}, // SATA Controller 0
    {22, 0, PchIntA, 16}, // CSME: HECI #1
    {22, 1, PchIntB, 17}, // CSME: HECI #2
    {22, 2, PchIntC, 18}, // CSME: IDE-Redirection (IDE-R)
    {22, 3, PchIntD, 19}, // CSME: Keyboard and Text (KT) Redirection
    {22, 4, PchIntA, 16}, // CSME: HECI #3
    {22, 5, PchIntB, 17}, // CSME: HECI #4
//  {22, 6, PchNoInt, 0},       // CSME: MROM
    {21, 0, PchIntA, 16}, // SMBus DMA
//  {21, 6, PchNoInt, 0}, // IE: MROM
//  {21, 7, PchNoInt, 0}, // IE: KVMcc
    {20, 0, PchIntA, 16}, // USB 3.0 xHCI Controller
//  {20, 0, PchNoInt, 0}, // sIEH
    {19, 0, PchIntA, 16}, // PCIE_RP[11]
    {18, 0, PchIntA, 16}, // PCIE_RP[10]
    {17, 0, PchIntA, 16}, // PCIE_RP[9]
    {16, 0, PchIntA, 16}, // PCIE_RP[8]
    {15, 0, PchIntA, 16}, // PCIE_RP[7]
    {14, 0, PchIntA, 16}, // PCIE_RP[6]
    {13, 0, PchIntA, 16}, // PCIE_RP[5]
    {12, 0, PchIntA, 16}, // PCIE_RP[4]
    {11, 0, PchIntA, 16}, // PCIE_RP[3]
    {10, 0, PchIntA, 16}, // PCIE_RP[2]
    {9, 0, PchIntA, 16},  // PCIE_RP[1]
    {8, 0, PchIntA, 16},  // PCIE_RP[0]
};

/**
  Adds interrupt configuration for device

  @param[in/out] InterruptConfig         Pointer to interrupt config
**/
VOID
EFIAPI
LoadDeviceInterruptConfig (
  IN OUT UINT8                          *NumOfDevIntConfig,
  IN OUT PCH_DEVICE_INTERRUPT_CONFIG    DevIntConfig[]
  )
{
  UINT8                 IntConfigTableEntries;

  IntConfigTableEntries = ARRAY_SIZE (mDevIntConfig);
  ASSERT (IntConfigTableEntries <= PCH_MAX_DEVICE_INTERRUPT_CONFIG);
  *NumOfDevIntConfig = IntConfigTableEntries;
  CopyMem (
    DevIntConfig,
    mDevIntConfig,
    sizeof (mDevIntConfig)
    );
}
