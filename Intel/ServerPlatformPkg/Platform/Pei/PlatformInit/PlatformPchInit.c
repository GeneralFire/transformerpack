/** @file
  PlatformPchInit.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2020 Intel Corporation.

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

#include "PlatformInitPreMem.h"

#include <Register/PchRegsPcr.h>
#include <Library/PchPcrLib.h>
#include <Library/PciSegmentLib.h>

VOID
EarlyPlatformPchInit (
  IN EFI_PEI_SERVICES            **PeiServices,
  IN SYSTEM_CONFIGURATION        *SystemConfiguration,
  IN PCH_SETUP                   *PchSetup
  )
{
  UINT16                          Data16;
  UINT8                           Data8;
  UINTN                           LpcBaseAddress;
  UINT8                           TcoRebootHappened;
  UINTN                           SpiBaseAddress;
  UINTN                           P2sbBase;
#ifdef ASPEED_ENABLE
  UINT16                          LpcIoeOrg;
#endif

  DEBUG((DEBUG_INFO, "%a() - Start\n", __FUNCTION__));

  LpcBaseAddress = PCI_SEGMENT_LIB_ADDRESS (
                      DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_LPC,
                      PCI_FUNCTION_NUMBER_PCH_LPC,
                      0
                   );
  SpiBaseAddress = PCI_SEGMENT_LIB_ADDRESS (
                      DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_SPI,
                      PCI_FUNCTION_NUMBER_PCH_SPI,
                      0
                   );
  //
  // Program bar
  //
  P2sbBase = PCI_SEGMENT_LIB_ADDRESS (
                DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                DEFAULT_PCI_BUS_NUMBER_PCH,
                PCI_DEVICE_NUMBER_PCH_P2SB,
                PCI_FUNCTION_NUMBER_PCH_P2SB,
                0
             );

  PciSegmentWrite32 (P2sbBase + R_P2SB_CFG_SBREG_BAR, PCH_PCR_BASE_ADDRESS);
  PciSegmentOr8 (P2sbBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // LPC I/O Configuration
  //
  PchLpcIoDecodeRangesSet (
    (V_LPC_CFG_IOD_LPT_378  << N_LPC_CFG_IOD_LPT)  |
    (V_LPC_CFG_IOD_COMB_2F8 << N_LPC_CFG_IOD_COMB) |
    (V_LPC_CFG_IOD_COMA_3F8 << N_LPC_CFG_IOD_COMA)
    );

  PchLpcIoEnableDecodingSet (PcdGet16 (PcdLpcEnableDecodingSet));

#ifdef ASPEED_ENABLE
  LpcIoeOrg = PciSegmentRead16 (LpcBaseAddress + R_LPC_CFG_IOE);
  DEBUG ((DEBUG_INFO, "[IPMI_DEBUG]: LpcIoeOrg: 0x%x\n", LpcIoeOrg));
#endif
  //
  // Enable the upper 128-byte bank of RTC RAM
  //
  PchPcrAndThenOr32 (PID_RTC_HOST, R_RTC_PCR_CONF, (UINT32)~0, B_RTC_PCR_CONF_UCMOS_EN);

// APTIOV_SERVER_OVERRIDE_RC_START : BMC KCS base address decoding is done by IPMI module
#if 0  
#ifdef ASPEED_ENABLE
  //
  // Enable LPC decode at 0xCA2 for Pilot BMC, 0x600 for Mailbox
  //
  PchLpcGenIoRangeSet ((IPMI_DEFAULT_SMM_IO_BASE  & 0xFF0), 0x10);
  DEBUG ((EFI_D_INFO, "[IPMI_DEBUG]: PchLpcGenIoRangeSet CA0!\n"));

  PchLpcGenIoRangeSet ((PILOTIV_MAILBOX_BASE_ADDRESS  & 0xFF0), 0x10);
  DEBUG ((EFI_D_INFO, "[IPMI_DEBUG]: PchLpcGenIoRangeSet 600!\n"));
#endif
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : BMC KCS base address decoding is done by IPMI module
  //
  // Disable the Watchdog timer expiration from causing a system reset
  //
  PchPcrAndThenOr32 (PID_ITSS, R_ITSS_PCR_GIC, (UINT32)~0, B_ITSS_PCR_GIC_AME);
  //
  // Halt the TCO timer
  //
  Data16 = IoRead16 (PCH_TCO_BASE_ADDRESS + R_TCO_IO_TCO1_CNT);
  Data16 |= B_TCO_IO_TCO1_CNT_TMR_HLT;
  IoWrite16 (PCH_TCO_BASE_ADDRESS + R_TCO_IO_TCO1_CNT, Data16);

  //
  // Read the Second TO status bit
  //
  Data8 = IoRead8 (PCH_TCO_BASE_ADDRESS + R_TCO_IO_TCO2_STS);
  if ((Data8 & B_TCO_IO_TCO2_STS_SECOND_TO) == B_TCO_IO_TCO2_STS_SECOND_TO) {
    TcoRebootHappened = 1;
    DEBUG ((EFI_D_INFO, "%a - TCO Second TO status bit is set. This might be a TCO reboot\n", __FUNCTION__));
  } else {
    TcoRebootHappened = 0;
  }

  //
  // Clear the Second TO status bit
  //
  Data8 |= (UINT8) B_TCO_IO_TCO2_STS_SECOND_TO;
  Data8 &= (UINT8) ~B_TCO_IO_TCO2_STS_INTRD_DET;
  IoWrite8 (PCH_TCO_BASE_ADDRESS + R_TCO_IO_TCO2_STS, Data8);

  //
  // Disable SERR NMI and IOCHK# NMI in port 61
  //
  Data8 = IoRead8 (R_PCH_IO_NMI_SC);
  Data8 |= (B_PCH_IO_NMI_SC_PCI_SERR_EN | B_PCH_IO_NMI_SC_IOCHK_NMI_EN);
  IoWrite8 (R_PCH_IO_NMI_SC, Data8);

  PchPcrAndThenOr32 (PID_ITSS, R_ITSS_PCR_GIC, (UINT32)~B_ITSS_PCR_GIC_AME, 0);

  //
  // Clear EISS bit to allow for SPI use
  //
  PciSegmentAnd8 (SpiBaseAddress + R_SPI_CFG_BC, (UINT8)~B_SPI_CFG_BC_EISS);

  DEBUG((DEBUG_INFO, "%a() - End\n", __FUNCTION__));

}

