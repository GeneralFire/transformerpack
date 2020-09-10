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
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/PcuMailBoxTargetLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/IoLib.h>

/**
  Write data to PCU using BiosToPcu mailbox command

  @param Socket         CPU Socket Node number (Socket ID)
  @param DwordCommand   BiosToPCU Command
  @param DwordData      Data to write

  @retval If error return the value of the last read. If success return 0;
**/
UINT32
WriteBios2PcuMailboxCommand (
  IN UINT8              Socket,
  IN UINT32             DwordCommand,
  IN UINT32             DwordData
  );

/**
  Print wait loop for micro bios in asm

  @param socket        - CPU Socket Node number (Socket ID)

  @retval  None
**/

VOID
EFIAPI
PrintWaitOnRegister (
  UINT8   Socket
  )
{
  UINTN         RegAddr;
  UINT32        LabelNumber;

  RegAddr = UsraGetCsrRegisterAddress (Socket, 0, BIOS_MAILBOX_INTERFACE);

  if (GetUbiosLoopbackLabelNumber (&LabelNumber) == EFI_SUCCESS) {

    DEBUG ((EFI_D_INFO, "\nLABEL_w_mbox%d:\n", LabelNumber));

    DEBUG ((EFI_D_INFO, "mov eax, DWORD PTR ds:[0%8xh]\n", RegAddr));
    DEBUG ((EFI_D_INFO, "bt  eax, 31\n"));
    DEBUG ((EFI_D_INFO, "jc LABEL_w_mbox%d\n", LabelNumber));

    LabelNumber++;
    SetUbiosLoopbackLabelNumber (LabelNumber);
  }
}


/**
   Clear the RUN_BUSY flag by performing a UsraCsrRead() poll with
   a timeout of 10 milliseconds.

  @param Socket        - CPU Socket Node number (Socket ID)
  @param BoxInst       - Box Instance, 0 based
  @param RegOffset     - Register offset

  @retval If error return the value of the last read. If success return 0;
**/

UINT32
EFIAPI
ClearRunBusyRegFlag (
  UINT8    Socket,
  UINT8    BoxInst,
  UINT32   RegOffset
  )
{
  UINT32    StartCount     = 1000;
  UINT32    MailBoxStatus  = 0;

  MailBoxStatus = UsraCsrRead (Socket, BoxInst, RegOffset); //returns back data

  //After 5ms if run_busy is still 1, then SW cannot write to the CSRs; error needs to be O/P(last read is O/P)
  //Poll with a max timeout of 5ms
  while (MailBoxStatus & BIT31) {
    if ((EFI_ERROR (ClearRunBusyFlagDelayTarget ()))) {
      break;
    }
    MailBoxStatus = UsraCsrRead (Socket, BoxInst, RegOffset);
    StartCount--;
    if (StartCount == 0) {
      break;
    }
  }

  if (MailBoxStatus & BIT31) {
    return MailBoxStatus;
  }

  return 0;
} // ClearRunBusyRegFlag

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
ReadModifyWritePcuMailbox (
  IN     UINT8      Socket,
  IN     UINT32     DwordReadCommand,
  IN     UINT32     DwordWriteCommand,
  IN OUT UINT32     *DwordData,
  IN     UINT32     DwordMaskValue,
  IN     UINT32     DwordOrValue
  )
{
  UINT32    MailBoxStatus = MAILBOX_BIOS_CC_NO_ERROR;
  BOOLEAN   ReadCommandEnabled;
  BOOLEAN   WriteCommandEnabled;
  UINTN     RegAddr;
  UINT8     OldValue = 0;

  //
  // Before send command
  //
  DEBUG ((EFI_D_INFO, "\n:X64:  Send ReadModifyWritePcuMailbox.  Socket = %u ReadCommand == 0x%x  WriteCommand == 0x%x  Data = 0x%x  MaskValue = 0x%x  OrValue = 0x%x\n",
          Socket, DwordReadCommand, DwordWriteCommand, *DwordData, DwordMaskValue, DwordOrValue));

  ReadCommandEnabled = IsBios2PcuMailboxCommandEnabled ((UINT8) (DwordReadCommand & 0xFF));
  WriteCommandEnabled = IsBios2PcuMailboxCommandEnabled ((UINT8) (DwordWriteCommand & 0xFF));

  //
  // Read
  //
  if (DwordReadCommand != MAILBOX_BIOS_CMD_ZERO) {
    if (ReadCommandEnabled) {
      MailBoxStatus = WriteBios2PcuMailboxCommand (Socket, DwordReadCommand, *DwordData);

      if (MailBoxStatus == MAILBOX_BIOS_CC_NO_ERROR) {
        *DwordData = UsraCsrRead (Socket, 0, BIOS_MAILBOX_DATA);
        DEBUG ((EFI_D_INFO, "  Data received from mailbox: 0x%x\n", *DwordData));
        //
        // To be modified with Write command
        //
        if (WriteCommandEnabled && (DwordWriteCommand != MAILBOX_BIOS_CMD_ZERO)) {
          *DwordData = ((*DwordData) & DwordMaskValue) | DwordOrValue;
          if (UbiosGenerationEnabled ()) {
            RegAddr = UsraGetCsrRegisterAddress (Socket, 0, BIOS_MAILBOX_DATA);
            DEBUG ((EFI_D_INFO, "\nmov eax, DWORD PTR ds:[0%08xh]", RegAddr));
            DEBUG ((EFI_D_INFO, "\nand eax, 0%08xh", DwordMaskValue));
            DEBUG ((EFI_D_INFO, "\nor  eax, 0%08xh\n", DwordOrValue));
          }
        }
      }
    }
  } else if (DwordWriteCommand != MAILBOX_BIOS_CMD_ZERO) {
    if (WriteCommandEnabled && UbiosGenerationEnabled ()) {
      DEBUG ((EFI_D_INFO, "\nmov eax, 0%08xh\n", *DwordData));
    }
  }

  //
  // Write
  //
  if ((DwordWriteCommand == MAILBOX_BIOS_CMD_ZERO) || !WriteCommandEnabled) {
    return MailBoxStatus;
  }

  if (UbiosGenerationEnabled ()) {
    DEBUG ((EFI_D_INFO, "\npush eax\n"));
    PrintWaitOnRegister (Socket);
    DEBUG ((EFI_D_INFO, "\npop eax\n"));
    RegAddr = UsraGetCsrRegisterAddress (Socket, 0, BIOS_MAILBOX_DATA);
    DEBUG ((EFI_D_INFO, "\nmov DWORD PTR ds:[0%08xh], eax\n", RegAddr));
    RegAddr = UsraGetCsrRegisterAddress (Socket, 0, BIOS_MAILBOX_INTERFACE);
    DEBUG ((EFI_D_INFO, "\nmov DWORD PTR ds:[0%08xh], 0%08xh\n", RegAddr, DwordWriteCommand | BIT31));
    if ((DwordWriteCommand & MAILBOX_BIOS_CMD_MULTI_SKT_UPD_INTERFACE_COMMAND_MASK) == MAILBOX_BIOS_CMD_MULTI_SKT_UPD) {
      DEBUG ((EFI_D_INFO, "\n  hlt\n"));
    }
    PrintWaitOnRegister (Socket);
  }

  //
  // Since ReadModifyWrite asm commands are already in log, disable asm output for B2P write calls
  //
  OldValue = GetUbiosOutputMode ();
  SetUbiosOutputMode (ASM_OUTPUT_DISABLE);
  MailBoxStatus = WriteBios2PcuMailboxCommand (Socket, DwordWriteCommand, *DwordData);
  if (MailBoxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    SetUbiosOutputMode (OldValue);
    return MailBoxStatus;
  }

  if (ReadCommandEnabled && (DwordReadCommand != MAILBOX_BIOS_CMD_ZERO)) {
    //
    // Read data back with DwordReadCommand
    //
    MailBoxStatus = WriteBios2PcuMailboxCommand (Socket, DwordReadCommand, *DwordData);
    *DwordData = UsraCsrRead (Socket, 0, BIOS_MAILBOX_DATA);
    DEBUG ((EFI_D_INFO, "  Data received from mailbox: 0x%x\n", *DwordData));
  }
  SetUbiosOutputMode (OldValue);

  return MailBoxStatus;
}

/**
  Write data to PCU using BiosToPcu mailbox command

  @param Socket             CPU Socket Node number (Socket ID)
  @param dwordCommand       BiosToPCU Command
  @param dwordData          Data to write

  @retval If error return the value of the last read. If success return 0;
**/
UINT32
WriteBios2PcuMailboxCommand (
  UINT8  Socket,
  UINT32 dwordCommand,
  UINT32 dwordData
  )
{
  UINT32    MailBoxStatus = 0;
  UINTN     RegAddr;
  UINT32    LabelNumber;
  UINT8     AssertCommand;

  //
  // Wait until pCode Mailbox is available (i.e. run_busy flag is clear)
  //
  DEBUG ((EFI_D_INFO, "\n  Wait for mailbox ready\n"));

  if (ClearRunBusyFlagTarget (Socket, BIOS_MAILBOX_INTERFACE, &MailBoxStatus) == EFI_TIMEOUT) {
    return MailBoxStatus;
  }

  if ((GetUbiosLoopbackLabelNumber (&LabelNumber) == EFI_SUCCESS) && (GetUbiosOutputMode () != ASM_OUTPUT_DISABLE)) {
    RegAddr = UsraGetCsrRegisterAddress (Socket, 0, BIOS_MAILBOX_INTERFACE);
    DEBUG ((EFI_D_INFO, "\nLABEL_w_mbox%d:\n", LabelNumber));
    DEBUG ((EFI_D_INFO, "mov eax, DWORD PTR ds:[0%8xh]\n", RegAddr));
    DEBUG ((EFI_D_INFO, "bt  eax, 31\n"));
    DEBUG ((EFI_D_INFO, "jc LABEL_w_mbox%d\n", LabelNumber));

    LabelNumber++;
    SetUbiosLoopbackLabelNumber (LabelNumber);
  }

  //
  // Write data to the BIOS to Pcode Mailbox data register (now that run_busy != 1)
  //
  DEBUG ((EFI_D_INFO, "  Send Data portion of command.  Socket = %u Data sent == 0x%x\n", Socket, dwordData));
  UsraCsrWrite (Socket, 0, BIOS_MAILBOX_DATA, (UINT32) dwordData);
  if (UbiosGenerationEnabled () && (GetUbiosOutputMode () != ASM_OUTPUT_DISABLE)) {
    RegAddr = UsraGetCsrRegisterAddress (Socket, 0, BIOS_MAILBOX_DATA);
    DEBUG ((EFI_D_INFO, "\nmov DWORD PTR ds:[0%08xh], 0%08xh\n", RegAddr, dwordData));
  }

  //
  // Write pcode mailbox command code (now that run_busy != 1)
  //
  DEBUG ((EFI_D_INFO, "  Send Pcode mailbox command. Socket = %u Command sent == 0x%x\n", Socket, dwordCommand));
  if ((dwordCommand & MAILBOX_BIOS_CMD_MULTI_SKT_UPD_INTERFACE_COMMAND_MASK) == MAILBOX_BIOS_CMD_MULTI_SKT_UPD) {
    //
    // Get register address first
    //
    RegAddr = UsraGetCsrRegisterAddress (Socket, 0, BIOS_MAILBOX_INTERFACE);

    MmioWrite32 (RegAddr, (UINT32) (dwordCommand | BIT31));
    CpuSleep ();
  } else {
    UsraCsrWrite (Socket, 0, BIOS_MAILBOX_INTERFACE, (UINT32) (dwordCommand | BIT31));
  }

  DEBUG ((EFI_D_INFO, "  Wait for mailbox ready\n"));

  if (ClearRunBusyFlagTarget (Socket, BIOS_MAILBOX_INTERFACE, &MailBoxStatus) == EFI_TIMEOUT) {
    return MailBoxStatus;
  }

  if ((GetUbiosLoopbackLabelNumber (&LabelNumber) == EFI_SUCCESS) && (GetUbiosOutputMode () != ASM_OUTPUT_DISABLE)) {
    RegAddr = UsraGetCsrRegisterAddress (Socket, 0, BIOS_MAILBOX_INTERFACE);

    DEBUG ((EFI_D_INFO, "\nLABEL_w_mbox%d:\n", LabelNumber));
    DEBUG ((EFI_D_INFO, "mov eax, DWORD PTR ds:[0%8xh]\n", RegAddr));
    DEBUG ((EFI_D_INFO, "bt  eax, 31\n"));
    DEBUG ((EFI_D_INFO, "jc LABEL_w_mbox%d\n", LabelNumber));
    LabelNumber++;
    SetUbiosLoopbackLabelNumber (LabelNumber);
  }

  //
  // Read Mailbox data (updated/read by Pcode)
  //
  MailBoxStatus = UsraCsrRead (Socket, 0, BIOS_MAILBOX_INTERFACE);


  // This function returns the error code from the PCU mailbox (0 = NO ERROR)
  // PCODE responds to the CMD(dwordCommand) and reads data(dwordData) written by SW but
  // if it finds an error in execution the error gets entered by PCODE in bits (7:0) and is O/P as below
  if (MailBoxStatus & 0xFF) {
    DEBUG ((EFI_D_ERROR, "\n  **ERROR! PCODE returned MailBox(MB) Error code = 0x%x, MBCommand = 0x%x, MBdata = 0x%x. \n", (MailBoxStatus & 0xFF), dwordCommand, dwordData));
    AssertCommand = PcdGet8 (PcdDfxB2PErrAssertAtCommand);
    if (((AssertCommand != B2P_ERROR_ASSERT_ON_NO_COMMAND) && (AssertCommand == (UINT8) dwordCommand)) || (AssertCommand == B2P_ERROR_ASSERT_ON_ALL_COMMAND)) {
      ASSERT (FALSE);
    }
  }

  return MailBoxStatus;
} // WriteBios2PcuMailboxCommand

/**
  This function returns BOOLEAN to indicate if the given BIOS - to - Pcode mailbox command
  is enabled or disabled in DFX setup knob.

  @param[in]  CommandByte         8 - bit BIOS - to - Pcode mailbox command(see B2P_Mailbox.h)

  @retval TRUE    The given BIOS - to - Pcode mailbox command is enabled in the setup knob.
  @retval FALSE   The given BIOS - to - Pcode mailbox command is disabled in the setup knob.
**/

BOOLEAN
EFIAPI
IsBios2PcuMailboxCommandEnabled (
  IN UINT8       CommandByte
)
{
  return TRUE;
} // IsBios2PcuMailboxCommandEnabled
