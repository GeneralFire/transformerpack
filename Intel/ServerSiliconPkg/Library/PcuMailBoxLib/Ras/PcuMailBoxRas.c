/** @file
  Implementation of the Bios to Pcode Mailbox library for X64 build

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

#include <Library/CpuLib.h>
#include <Library/MemoryCoreLib.h>
#include <RcRegs.h>
#include <Library/DebugLib.h>
#include <Library/PcuMailBoxRasLib.h>
#include <Library/PcuMailBoxTargetLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/IoLib.h>

UINT8 mAssertCommand = B2P_ERROR_ASSERT_ON_NO_COMMAND;

/**
  Write data to PCU using BiosToPcu mailbox command

  @param Socket             - CPU Socket Node number (Socket ID)
  @param dwordCommand       - BiosToPCU Command
  @param dwordDat           - Data to write

  @retval If error return the value of the last read. If success return 0;
**/

UINT32
EFIAPI
WriteBios2PcuMailboxCommandRas (
  UINT8  Socket,
  UINT32 dwordCommand,
  UINT32 dworddata
  )
{
  UINT32    MailBoxStatus = 0;
  UINTN     RegFlatAddr;

  //
  // Wait until pCode Mailbox is available (i.e. run_busy flag is clear)
  //
  if (ClearRunBusyFlagTarget (Socket, BIOS_MAILBOX_INTERFACE, &MailBoxStatus) == EFI_TIMEOUT) {
    return MailBoxStatus;
  }

  //
  // Write data to the BIOS to Pcode Mailbox data register (now that run_busy != 1)
  //
  //DEBUG ((EFI_D_ERROR, "  Send Data portion of command.  Socket = %u Data sent == 0x%x\n", Socket, dworddata));
  UsraCsrWrite (Socket, 0, BIOS_MAILBOX_DATA, (UINT32) dworddata);

  //
  // Write pcode mailbox command code (now that run_busy != 1)
  //
  //DEBUG ((EFI_D_ERROR, "  Send Pcode mailbox command. Socket = %u Command sent == 0x%x\n", Socket, dwordCommand));
  if ((dwordCommand & MAILBOX_BIOS_CMD_MULTI_SKT_UPD_INTERFACE_COMMAND_MASK) == MAILBOX_BIOS_CMD_MULTI_SKT_UPD) {
    //
    // Get register address first
    //
    RegFlatAddr = (UINTN) UsraGetCsrRegisterAddress (Socket, 0, BIOS_MAILBOX_INTERFACE);

    MmioWrite32 (RegFlatAddr, (UINT32) (dwordCommand | BIT31));
    CpuSleep ();
  } else {
    UsraCsrWrite (Socket, 0, BIOS_MAILBOX_INTERFACE, (UINT32) (dwordCommand | BIT31));
  }


  if (ClearRunBusyFlagTarget (Socket, BIOS_MAILBOX_INTERFACE, &MailBoxStatus) == EFI_TIMEOUT) {
    return MailBoxStatus;
  }

  //
  // Read Mailbox data (updated/read by Pcode)
  //
  MailBoxStatus = UsraCsrRead (Socket, 0, BIOS_MAILBOX_INTERFACE);

  // This function returns the error code from the PCU mailbox (0 = NO ERROR)
  // PCODE responds to the CMD(dwordCommand) and reads data(dworddata) written by SW but
  // if it finds an error in execution the error gets entered by PCODE in bits (7:0) and is O/P as below
  if (MailBoxStatus & 0xFF) {
    DEBUG ((EFI_D_ERROR, "\n  **ERROR! PCODE returned MailBox(MB) Error code = 0x%x, MBCommand = 0x%x, MBdata = 0x%x. \n", (MailBoxStatus & 0xFF), dwordCommand, dworddata));
    if (((mAssertCommand != B2P_ERROR_ASSERT_ON_NO_COMMAND) && (mAssertCommand == (UINT8) dwordCommand)) || (mAssertCommand == B2P_ERROR_ASSERT_ON_ALL_COMMAND)) {
      ASSERT (FALSE);
    }
  }

  return MailBoxStatus;
} // WriteBios2PcuMailboxCommand

/**
  Writes the given command to BIOS2PCU Mailbox and read back the data from PCU

  @param Socket       - CPU Socket number (Socket ID)
  @param Command      - Pcu mailbox command to write
  @param Data         - Pcu mailbox data

  @retval error code from the Pcu mailbox (0 = NO ERROR)

**/
UINT32
EFIAPI
Bios2PcodeMailBoxRead (
  IN UINT8  Socket,
  IN UINT32 Command,
  IN UINT32 *Data
  )
{
  UINT32    MailBoxStatus = 0;
  MailBoxStatus = WriteBios2PcuMailboxCommandRas (Socket, Command, *Data);
  if (MailBoxStatus == MAILBOX_BIOS_CC_NO_ERROR) {
    *Data = UsraCsrRead (Socket, 0, BIOS_MAILBOX_DATA);
  }
  return MailBoxStatus;
}

/**
  Writes the given command to BIOS to PCU Mailbox Interface CSR register

  @param Socket       - CPU Socket number (Socket ID)
  @param Command      - Pcu mailbox command to write
  @param Data         - Pcu mailbox data

  @retval error code from the Pcu mailbox (0 = NO ERROR)

**/
UINT32
EFIAPI
Bios2PcodeMailBoxWrite (
  IN UINT8  Socket,
  IN UINT32 Command,
  IN UINT32 Data
  )
{
  return WriteBios2PcuMailboxCommandRas (Socket, Command, Data);
}


/**
  This function provides capability of either reading BIOS-to-PCode command,
  writing BIOS-to-PCode command or doing read-modify-write BIOS-to-Pcode
  commands.

  For read capability:
  1. Write the data *DwordData to BIOS_MAILBOX DATA CSR and
     write "read-type" BIOS-to-PCode mailbox command to BIOS_MAILBOX_INTERFACE CSR
  2. Read and return the data from BIOS_MAILBOX_DATA CSR

  For write capability:
  1. Write the data *DwordData to BIOS_MAILBOX DATA CSR and
     write "write-type" BIOS-to-PCode mailbox command to BIOS_MAILBOX_INTERFACE CSR

  For read-modify-write capability:
  1. Write the data *DwordData to BIOS_MAILBOX DATA CSR and
     write "read-type" BIOS-to-PCode mailbox command to BIOS_MAILBOX_INTERFACE CSR
  2. Read the data from BIOS_MAILBOX_DATA CSR
  3. Modify the said returned data with the mask provided by the user
  4. Write the data back to BIOS_MAILBOX DATA CSR and
     write "write-type" BIOS-to-PCode mailbox command to BIOS_MAILBOX_INTERFACE CSR
  5. Write the data to BIOS_MAILBOX DATA CSR and
     write "read-type" BIOS-to-PCode mailbox command to BIOS_MAILBOX_INTERFACE CSR
  6. Read and return the data from BIOS_MAILBOX_DATA CSR

  @param[in]      Socket                CPU Socket Node number (Socket ID)
  @param[in]      DwordReadCommand      "Read-type" BIOS-to-PCode mailbox command to write into
                                        BIOS_MAILBOX_INTERFACE CSR
  @param[in]      DwordWriteCommand     "Write-type" BIOS-to-PCode mailbox command to write into
                                        BIOS_MAILBOX_INTERFACE CSR
  @param[in, out] Dworddata             As input, the data to Pcode by writing to BIOS_MAILBOX_DATA
                                        CSR. As output, the data returned from Pcode by reading
                                        from BIOS_MAILBOX_DATA CSR.
  @param[in]      DwordMaskValue        To clear specific bits in the data returned from Pcode
  @param[in]      DwordOrdata           To set specific bits in the data returned from Pcode

  @retval Error code from the PCU mailbox (0 = NO ERROR)

**/
UINT32
EFIAPI
ReadWritePcuMailbox (
  IN     UINT8      Socket,
  IN     UINT32     DwordReadCommand,
  IN     UINT32     DwordWriteCommand,
  IN OUT UINT32     *DwordData,
  IN     UINT32     DwordMaskValue,
  IN     UINT32     DwordOrValue
  )
{
  UINT32    MailBoxStatus;

  MailBoxStatus = MAILBOX_BIOS_CC_NO_ERROR;
  //
  // Before send command
  //
  DEBUG ((EFI_D_INFO, "\n:Ras:  Send ReadModifyWritePcuMailbox.  Socket = %u ReadCommand == 0x%x  WriteCommand == 0x%x  Data = 0x%x\n",
         Socket, DwordReadCommand, DwordWriteCommand, *DwordData));

  //
  // Read
  //
  if (DwordReadCommand != MAILBOX_BIOS_CMD_ZERO) {
    MailBoxStatus = WriteBios2PcuMailboxCommandRas (Socket, DwordReadCommand, *DwordData);
    if (MailBoxStatus == MAILBOX_BIOS_CC_NO_ERROR) {
      *DwordData = UsraCsrRead (Socket, 0, BIOS_MAILBOX_DATA);
      //DEBUG ((EFI_D_ERROR, "  Data received from mailbox: 0x%x\n", *DwordData));
      //
      // To be modified with Write command
      //
      if (DwordWriteCommand != MAILBOX_BIOS_CMD_ZERO) {
        *DwordData = ((*DwordData) & DwordMaskValue) | DwordOrValue;
      }
    }
  }

  //
  // Write
  //
  if (DwordWriteCommand == MAILBOX_BIOS_CMD_ZERO) {
    return MailBoxStatus;
  }

  MailBoxStatus = WriteBios2PcuMailboxCommandRas (Socket, DwordWriteCommand, *DwordData);
  if (MailBoxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    return MailBoxStatus;
  }

  if (DwordReadCommand != MAILBOX_BIOS_CMD_ZERO) {
    //
    // Read data back with DwordReadCommand
    //
    MailBoxStatus = WriteBios2PcuMailboxCommandRas (Socket, DwordReadCommand, *DwordData);
    *DwordData = UsraCsrRead (Socket, 0, BIOS_MAILBOX_DATA);
    //DEBUG ((EFI_D_ERROR, "  Data received from mailbox: 0x%x\n", *DwordData));
  }
  return MailBoxStatus;
}


/**
  The constructor function of PCU mailbox RAS library.
  Note: When CU mailbox RAS library is used in another library's constructor(for example Lib A),
        current build tool does not put SmmUsraLibConstructor in front of Lib A's constructor correctly.
        So add empty constructor to adjust SmmUsraLibConstructor order.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
PcuMailBoxRasLibConstructor (
  VOID
  )
{
  mAssertCommand = PcdGet8 (PcdDfxB2PErrAssertAtCommand);
  return EFI_SUCCESS;
}
