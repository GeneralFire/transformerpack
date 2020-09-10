/** @file
  Implementation of the Bios to Pcode Mailbox library for IA32 build

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
#include <Library/UpiHostDataLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/PcuMailBoxTargetLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemoryServicesLib.h>
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
  UINT8   socket
  )
{
  UINT8         *RegAddr;
  UINT32        LabelNumber;
  KTI_HOST_OUT  *KtiHostOutPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (socket, 0, BIOS_MAILBOX_INTERFACE);

  if (GetUbiosLoopbackLabelNumber (&LabelNumber) == EFI_SUCCESS) {
    if (KtiHostOutPtr->CurrentReset == POST_RESET_POWERGOOD) {
      RcDebugPrint (SDBG_DEFAULT, "\nLABEL_c_mbox%d:\n", LabelNumber);
    } else {
      RcDebugPrint (SDBG_DEFAULT, "\nLABEL_w_mbox%d:\n", LabelNumber);
    }
    RcDebugPrint (SDBG_DEFAULT, "mov eax, DWORD PTR ds:[0%8xh]\n", RegAddr);
    RcDebugPrint (SDBG_DEFAULT, "bt  eax, 31\n");
    if (KtiHostOutPtr->CurrentReset == POST_RESET_POWERGOOD) {
      RcDebugPrint (SDBG_DEFAULT, "jc LABEL_c_mbox%d\n", LabelNumber);
    } else {
      RcDebugPrint (SDBG_DEFAULT, "jc LABEL_w_mbox%d\n", LabelNumber);
    }
    LabelNumber++;
    SetUbiosLoopbackLabelNumber (LabelNumber);
  }
} // PrintWaitOnRegister


/**
   Clear the RUN_BUSY flag by performing a UsraCsrRead() poll with
   a timeout of 10 milliseconds.

  @param socket        - CPU Socket Node number (Socket ID)
  @param BoxInst       - Box Instance, 0 based
  @param RegOffset     - Register offset

  @retval If error return the value of the last read. If success return 0;
**/

UINT32
EFIAPI
ClearRunBusyRegFlag (
  UINT8    socket,
  UINT8    BoxInst,
  UINT32   RegOffset
  )
{
  UINT32    StartCount     = 1000;
  UINT32    MailBoxStatus  = 0;

  MailBoxStatus = UsraCsrRead (socket, BoxInst, RegOffset); //returns back data

  //After 10ms if run_busy is still 1, then SW cannot write to the CSRs; error needs to be O/P(last read is O/P)
  //Poll with a max timeout of 10ms
  while (MailBoxStatus & BIT31) {
    if ((EFI_ERROR (ClearRunBusyFlagDelayTarget ()))) {
      break;
    }
    MailBoxStatus = UsraCsrRead (socket, BoxInst, RegOffset);
    StartCount--;
    if (StartCount == 0) {
      break;
    }
  }

  if (MailBoxStatus & BIT31) {
    return MailBoxStatus;
  }

  return 0;
} //ClearRunBusyRegFlag

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
  UINT32     MailBoxStatus = MAILBOX_BIOS_CC_NO_ERROR;
  BOOLEAN    ReadCommandEnabled;
  BOOLEAN    WriteCommandEnabled;
  UINT8      *RegAddr = 0;
  UINT8      OldValue = 0;

  //
  // Before send command
  //
  RcDebugPrint (SDBG_MAX, "Send ReadModifyWritePcuMailbox.  Socket = %u ReadCommand == 0x%x  WriteCommand == 0x%x  Data = 0x%x  MaskValue = 0x%x  OrValue = 0x%x\n",
          Socket, DwordReadCommand, DwordWriteCommand, *DwordData, DwordMaskValue, DwordOrValue);

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
        RcDebugPrint (SDBG_DEFAULT, "  Data received from mailbox: 0x%x\n", *DwordData);
        //
        // To be modified with Write command
        //
        if (WriteCommandEnabled && (DwordWriteCommand != MAILBOX_BIOS_CMD_ZERO)) {
          *DwordData = ((*DwordData) & DwordMaskValue) | DwordOrValue;
          if (UbiosGenerationEnabled ()) {
            RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (Socket, 0, BIOS_MAILBOX_DATA);
            RcDebugPrint (SDBG_DEFAULT, "\nmov eax, DWORD PTR ds:[0%08xh]", RegAddr);
            RcDebugPrint (SDBG_DEFAULT, "\nand eax, 0%08xh", DwordMaskValue);
            RcDebugPrint (SDBG_DEFAULT, "\nor  eax, 0%08xh\n", DwordOrValue);
          }
        }
      }
    }
  } else if (DwordWriteCommand != MAILBOX_BIOS_CMD_ZERO) {
    if (WriteCommandEnabled && UbiosGenerationEnabled ()) {
      RcDebugPrint (SDBG_DEFAULT, "\nmov eax, 0%08xh\n", *DwordData);
    }
  }

  //
  // Write
  //
  if ((DwordWriteCommand == MAILBOX_BIOS_CMD_ZERO) || !WriteCommandEnabled) {
    return MailBoxStatus;
  }

  if (UbiosGenerationEnabled ()) {
    RcDebugPrint (SDBG_DEFAULT, "\npush eax\n");
    PrintWaitOnRegister (Socket);
    RcDebugPrint (SDBG_DEFAULT, "\npop eax\n");
    RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (Socket, 0, BIOS_MAILBOX_DATA);
    RcDebugPrint (SDBG_DEFAULT, "\nmov DWORD PTR ds:[0%08xh], eax\n", RegAddr);
    RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (Socket, 0, BIOS_MAILBOX_INTERFACE);
    RcDebugPrint (SDBG_DEFAULT, "\nmov DWORD PTR ds:[0%08xh], 0%08xh\n", RegAddr, DwordWriteCommand | BIT31);
    if ((DwordWriteCommand & MAILBOX_BIOS_CMD_MULTI_SKT_UPD_INTERFACE_COMMAND_MASK) == MAILBOX_BIOS_CMD_MULTI_SKT_UPD) {
      RcDebugPrint (SDBG_DEFAULT, "\nhlt\n");
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
    RcDebugPrint (SDBG_DEFAULT, "  Data received from mailbox: 0x%x\n", *DwordData);
  }
  SetUbiosOutputMode (OldValue);

  return MailBoxStatus;
}

/**
Write data to PCU using BiosToPcu mailbox command

@param socket           CPU Socket Node number (Socket ID)
@param dwordCommand     BiosToPCU Command
@param dwordData        Data to write

@retval If error return the value of the last read. If success return 0;
**/
UINT32
WriteBios2PcuMailboxCommand (
  UINT8  socket,
  UINT32 dwordCommand,
  UINT32 dwordData
  )
{
  UINT32        MailBoxStatus = 0;
#ifdef DEBUG_CODE_BLOCK
  UINT8         *RegAddr;
  UINT32        LabelNumber;
#endif //DEBUG_CODE_BLOCK
  KTI_HOST_OUT  *KtiHostOutPtr;
  UINT8         AssertCommand;
  UINTN         RegFlatAddr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  if (IsHvmModeEnable ()) {
    return MailBoxStatus;
  }

  //
  // Wait until pCode Mailbox is available (i.e. run_busy flag is clear)
  //
  RcDebugPrint (SDBG_MAX,
                 "\n  Wait for mailbox ready\n");

  if (ClearRunBusyFlagTarget (socket, BIOS_MAILBOX_INTERFACE, &MailBoxStatus) == EFI_TIMEOUT) {
    return MailBoxStatus;
  }

#ifdef DEBUG_CODE_BLOCK
  if ((GetUbiosLoopbackLabelNumber (&LabelNumber) == EFI_SUCCESS) && (GetUbiosOutputMode () != ASM_OUTPUT_DISABLE)) {
    RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (socket, 0, BIOS_MAILBOX_INTERFACE);
    if (KtiHostOutPtr->CurrentReset == POST_RESET_POWERGOOD) {
      RcDebugPrint (SDBG_DEFAULT, "\nLABEL_c_mbox%d:\n", LabelNumber);
    } else {
      RcDebugPrint (SDBG_DEFAULT, "\nLABEL_w_mbox%d:\n", LabelNumber);
    }
    RcDebugPrint (SDBG_DEFAULT, "mov eax, DWORD PTR ds:[0%8xh]\n", RegAddr);
    RcDebugPrint (SDBG_DEFAULT, "bt  eax, 31\n");
    if (KtiHostOutPtr->CurrentReset == POST_RESET_POWERGOOD) {
      RcDebugPrint (SDBG_DEFAULT, "jc LABEL_c_mbox%d\n", LabelNumber);
    } else {
      RcDebugPrint (SDBG_DEFAULT, "jc LABEL_w_mbox%d\n", LabelNumber);
    }
    LabelNumber++;
    SetUbiosLoopbackLabelNumber (LabelNumber);
  }
#endif // DEBUG_CODE_BLOCK

  //
  // Write data to the BIOS to Pcode Mailbox data register (now that run_busy != 1)
  //
  RcDebugPrint (SDBG_MAX,
                 "  Send Data portion of command.  Socket = %u Data sent == 0x%x\n", socket, dwordData);

#if defined (DDR5_SUPPORT) && !defined (SPR_HOST)
  /* DDR5_TODO: make sure we set 1.1V per the SPD data  */
  if (dwordData == 0xBF) {
    RcDebugPrint (SDBG_MAX, "######## Got 0xBF (1.2V), overriding to 0xAB (1.1V) for BRS PO ########\n");
    dwordData = 0xAB;
  }
#endif

  UsraCsrWrite (socket, 0, BIOS_MAILBOX_DATA, (UINT32) dwordData);

  //
  // Write pcode mailbox command code (now that run_busy != 1)
  //
  RcDebugPrint (SDBG_MAX,
                 "  Send Pcode mailbox command. Socket = %u Command sent == 0x%x\n", socket, dwordCommand);
  if ((dwordCommand & MAILBOX_BIOS_CMD_MULTI_SKT_UPD_INTERFACE_COMMAND_MASK) == MAILBOX_BIOS_CMD_MULTI_SKT_UPD) {
    //
    // Get register address first
    //
    RegFlatAddr = UsraGetCsrRegisterAddress (socket, 0, BIOS_MAILBOX_INTERFACE);
    MmioWrite32 (RegFlatAddr, (UINT32) (dwordCommand | BIT31));
    CpuSleep ();
  } else {
    UsraCsrWrite (socket, 0, BIOS_MAILBOX_INTERFACE, (UINT32) (dwordCommand | BIT31));
  }

  RcDebugPrint (SDBG_MAX,
                 "  Wait for mailbox ready\n");

  if (ClearRunBusyFlagTarget (socket, BIOS_MAILBOX_INTERFACE, &MailBoxStatus) == EFI_TIMEOUT) {
    return MailBoxStatus;
  }

#ifdef DEBUG_CODE_BLOCK
  if ((GetUbiosLoopbackLabelNumber (&LabelNumber) == EFI_SUCCESS) && (GetUbiosOutputMode () != ASM_OUTPUT_DISABLE)) {
    RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (socket, 0, BIOS_MAILBOX_INTERFACE);
    if (KtiHostOutPtr->CurrentReset == POST_RESET_POWERGOOD) {
      RcDebugPrint (SDBG_DEFAULT, "\nLABEL_c_mbox%d:\n", LabelNumber);
    } else {
      RcDebugPrint (SDBG_DEFAULT, "\nLABEL_w_mbox%d:\n", LabelNumber);
    }
    RcDebugPrint (SDBG_DEFAULT, "mov eax, DWORD PTR ds:[0%8xh]\n", RegAddr);
    RcDebugPrint (SDBG_DEFAULT, "bt  eax, 31\n");
    if (KtiHostOutPtr->CurrentReset == POST_RESET_POWERGOOD) {
      RcDebugPrint (SDBG_DEFAULT, "jc LABEL_c_mbox%d\n", LabelNumber);
    } else {
      RcDebugPrint (SDBG_DEFAULT, "jc LABEL_w_mbox%d\n", LabelNumber);
    }
    LabelNumber++;
    SetUbiosLoopbackLabelNumber (LabelNumber);
  }
#endif // DEBUG_CODE_BLOCK

  //
  // Read Mailbox data (updated/read by Pcode)
  //
  MailBoxStatus = UsraCsrRead (socket, 0, BIOS_MAILBOX_INTERFACE);
  CheckMailboxStatusTarget (MailBoxStatus);

  // This function returns the error code from the PCU mailbox (0 = NO ERROR)
  // PCODE responds to the CMD(dwordCommand) and reads data(dwordData) written by SW but
  // if it finds an error in execution the error gets entered by PCODE in bits (7:0) and is O/P as below
  if (MailBoxStatus & 0xFF) {
    RcDebugPrint (SDBG_DEFAULT,
                 "\n  **ERROR! PCODE returned MailBox(MB) Error code = 0x%x, MBCommand = 0x%x, MBdata = 0x%x. \n", (MailBoxStatus & 0xFF), dwordCommand, dwordData);
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
  UINT8     Bitmask;
  UINT8     CommandGroupNumber;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  Bitmask = 1 << (CommandByte % MAX_B2P_COMMANDS_PER_GROUP);
  CommandGroupNumber = CommandByte / MAX_B2P_COMMANDS_PER_GROUP;

  if (CommandGroupNumber >= MAX_B2P_MAILBOX_GROUPS) {
    return FALSE;
  }

  if ((Setup->common.DfxB2PMailboxCmdEnMask[CommandGroupNumber] & Bitmask) != 0) {
    return TRUE;
  } else {
    return FALSE;
  }
} // IsBios2PcuMailboxCommandEnabled

