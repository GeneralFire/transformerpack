/** @file
  Code File for CPU Power Management

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include "Common/CpuPpmIncludes.h"
#include <Library/PcuMailBoxLib.h>
#include <Protocol/CpuPolicyProtocol.h>
#include <Library/UefiBootServicesTableLib.h>

/**

  This routine is called to program THERMAL_REPORT. It will be call multiple time passing any the SocketNumber to be programmed.

  @param SocketNumber          Socket index that is executing.
  @param ThermalReport         Pointer to TM_STATUS_Filter instance.
  @param CpuThermalManagement  CPU Thermal Management value.

  @retval                      None

**/
VOID
EFIAPI
InitializeThermalReport (
  UINT8                  SocketNumber,
  TM_STATUS_Filter       *ThermalReport,
  UINT8                  CpuThermalManagement
  )
{
  UINT32                                  Tempdata = 0;
  B2P_THERTMAL_REPORT_INTERFACE_STRUCT    ThermalReportCmdB2p;
  B2P_THERTMAL_REPORT_DATA_STRUCT         ThermalReportDataB2p;


  if (CpuThermalManagement == 1) {
    if (ThermalReport->ThermalMonitorStatusFilter != 1) {
      if (SocketNumber == 0) {
        DEBUG ((EFI_D_INFO, " ThermalMonitorStatusFilter is disabled.\n"));
      }
      return ;
    }
    ThermalReportCmdB2p.Data = 0;
    ThermalReportDataB2p.Data = 0;
    //
    // ThermalReport Register need to mask
    //
    ThermalReportDataB2p.Bits.WindowSize  = ThermalReport->ThermalMonitorStatusFilterTimeWindow;
    ThermalReportDataB2p.Bits.Enable_EWMA = ThermalReport->ThermalMonitorStatusFilter;

    ThermalReportCmdB2p.Bits.command = (UINT32) MAILBOX_BIOS_CMD_THERMAL_REPORT;
    ThermalReportCmdB2p.Bits.read_write = 1;
    ThermalReportCmdB2p.Bits.SubCommand = (UINT32) MAILBOX_BIOS_CMD_THERMAL_REPORT_INTERFACE_SUB_COMMAND_THERM_MONITOR_STATUS;
    Tempdata = ReadModifyWritePcuMailbox (
            SocketNumber,
            0,
            ThermalReportCmdB2p.Data,
            &ThermalReportDataB2p.Data,
            0,
            0
            );
    PpmS3BootScriptSaveB2PWrite (SocketNumber, ThermalReportCmdB2p.Data, ThermalReportDataB2p.Data);
    if (Tempdata != 0) {
      DEBUG ((EFI_D_ERROR, " \n\n :: !!! BIOS failed to program B2P ThermalReport on socket (%d). Pcode returns (%d) !!!. \n\n", SocketNumber, Tempdata));
    }
  }

}