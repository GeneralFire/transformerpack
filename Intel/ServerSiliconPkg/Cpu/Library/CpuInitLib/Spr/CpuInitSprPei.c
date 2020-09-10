/** @file
  Cpu Initialization Pei Phase Module.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2020 Intel Corporation. <BR>

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

#include "CpuInit.h"
#include <Library/PcuMailBoxLib.h>
#include <Library/VcuMailBoxLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/TimerLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/CsrAccessLib.h>

/**

  Executed by SBSP only. Decouple CLM frequency from core PBM

  @retval VOID

**/
VOID
DeCoupleCLMFrequency (
  )
{
  return;
}


/**

  Restore DESIRED_CORES CSRS for all CPUs on S3 resume path.

  @param socket: socket number

  @retval Type of reset required

**/
UINT8
RestoreDesiredCores(
  UINT8                      socket
  )
{
  UINT8                                     resetNeeded = POST_RESET_NO_RESET;
  UINT64_DATA                               DesiredCoresCsr;
  CSR_DESIRED_CORES_MASK0_PCU_FUN1_STRUCT   csrDesiredCoresMask0;
  CSR_DESIRED_CORES_MASK1_PCU_FUN1_STRUCT   csrDesiredCoresMask1;
  CSR_DESIRED_CORES_PCU_FUN1_STRUCT         csrDesiredCoresPcuFun1;
  CPU_NVRAM                                 *NvRamData;
  UINT8                                     SmtCapable;

  NvRamData = GetCpuNvramData();
  if (NvRamData == NULL) {
    RcDebugPrint (SDBG_DEFAULT, "NvramData Returned NULL pointer\n");
    RC_FATAL_ERROR ((NvRamData != NULL), ERR_NO_MEMORY, RC_FATAL_ERROR_MINOR_CODE_0);
    return POST_RESET_NO_RESET;
  }

  DesiredCoresCsr.Data = NvRamData->DesiredCoresCsr[socket];

  csrDesiredCoresMask0.Data = UsraCsrRead(socket, 0, CSR_DESIRED_CORES_MASK0_PCU_FUN1_REG);
  if (csrDesiredCoresMask0.Data != (UINT32) DesiredCoresCsr.Data32.Low) {
    UsraCsrWrite (socket, 0, CSR_DESIRED_CORES_MASK0_PCU_FUN1_REG, DesiredCoresCsr.Data32.Low);
    resetNeeded |= POST_RESET_WARM;
  }
  csrDesiredCoresMask1.Data = UsraCsrRead(socket, 0, CSR_DESIRED_CORES_MASK1_PCU_FUN1_REG);
  if (csrDesiredCoresMask1.Data != (UINT32) DesiredCoresCsr.Data32.High) {
    UsraCsrWrite (socket, 0, CSR_DESIRED_CORES_MASK1_PCU_FUN1_REG, DesiredCoresCsr.Data32.High);
    resetNeeded |= POST_RESET_WARM;
  }

  SmtCapable  = GetSmtCapability (socket);
  if (SmtCapable) {
    csrDesiredCoresPcuFun1.Data = UsraCsrRead(socket, 0, CSR_DESIRED_CORES_PCU_FUN1_REG);
    if (csrDesiredCoresPcuFun1.Data != NvRamData->DesiredCoresCfg2Csr[socket]) {
      UsraCsrWrite (socket, 0, CSR_DESIRED_CORES_PCU_FUN1_REG, NvRamData->DesiredCoresCfg2Csr[socket]);
      resetNeeded |= POST_RESET_WARM;
    }
  }

  return resetNeeded;
}

/**

 Executed by SBSP only. Program IOT/OCLA config based on Setup data.

 @retval VOID

**/
VOID
Config_IOT_LLC (
  )
{
  return;
}

/**

  *10nm SKUs only*
  Limit CPU physical address to 46 bits to support older Hyper-v.

  @retval EFI_SUCCESS    Function execution successful.
  @retval !EFI_SUCCESS   Error on command execution.

**/
EFI_STATUS
CpuPhysicalAddressLimit (
  )
{
  EFI_STATUS           Status             = EFI_SUCCESS;
  UINT32               PcodeMailboxStatus = 0;
  UINT32               Data32             = 0;
  UINT32               PcuData            = 0;
  CPU_PRE_MEM_POLICY   *CpuPolicy;
  UINT8                Socket;

  RcDebugPrint (SDBG_DEFAULT, "\n:: CpuPhysicalAddressLimit()\n");

  CpuPolicy = GetCpuPreMemPolicy ();

  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                          0,
                          MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG,
                          MAILBOX_BIOS_CMD_ZERO,
                          &PcuData,
                          0,
                          0
                          );
  if (PcodeMailboxStatus == MAILBOX_BIOS_CC_NO_ERROR) {
    PcuData = PcuData & MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG_DATA_LIMIT_PA_TO_46_MASK;
  }

  if (CpuPolicy->CpuPaLimit != (PcuData >> MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG_DATA_LIMIT_PA_TO_46_LSB)) {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (SocketPresent(Socket)) {
        PcodeMailboxStatus = ReadModifyWritePcuMailbox (Socket, MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG, MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG, &Data32, ~(UINT32)MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG_DATA_LIMIT_PA_TO_46_MASK, CpuPolicy->CpuPaLimit << MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG_DATA_LIMIT_PA_TO_46_LSB);
        if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
          RcDebugPrint (SDBG_DEFAULT, "CpuPhysicalAddressLimit, Command->MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG, Error Code -> %x", PcodeMailboxStatus);
          Status = EFI_DEVICE_ERROR;
        }
      }
    }
  }

  return Status;
}


/**
  Send MAILBOX_BIOS_CMD_TSC_RESET_ENABLE to pcode, to enable reset TSC during warm reboot.

  @retval N/A
**/
VOID
EnableTscReset (
  VOID
  )
{
}
