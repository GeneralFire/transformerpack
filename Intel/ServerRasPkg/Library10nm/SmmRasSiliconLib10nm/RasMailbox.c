/** @file
  Implementation of Mailbox related interface

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

#include <RcRegs.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/PcuMailBoxRasLib.h>
#include <Library/RasDebugLib.h>
#include <SiliconSetting.h>

/**

 Send command to pcode to quiesce and set mirrorfailover for the MC for DDR4

 @param Socket -- socket index
 @param Mc  -- Memory controller index
 @param ChBitmap -- channel bit map for channel being failed over

 @return mailbox status
**/
UINT32
EFIAPI
Ddr4FailoverMailbox (
  IN   UINT8        Socket,
  IN   UINT8        Mc,
  IN   UINT8        ChBitmap
  )
{
  UINT32                              MailBoxCommand;
  UINT32                              MailboxData;
  //
  // B2P mailbox Interface:
  // Command[7:0]  = 0x0   NA
  // Data:
  // Command[7:0]  = 0x81   RAS_MC_QUIESCE_UNQUIESCE
  // Sub Command [11:8] = 0x1  0 for ADDDC and 1 for Mirror Failover
  // Mc Index [15:12]  = Mc Number
  // DDR Failed Channel Index [19:16] - Channel number for Spare Enable
  // Chan Index [22:20] = Ch Number one hot encoded for Mirror Failover
  // MC Mirror Enable [23] = 1 for Mirror failover operation
  // DDR Select [24]  = 0 for DDR4 failover
  //
  MailBoxCommand    = ((((BIT8) | (Mc << 12) | (ChBitmap << 20) | BIT23) & (~BIT24)) | MAILBOX_BIOS_CMD_RAS_QUIESCE_UNQUIESCE);
  MailboxData       = 0x0;
  return Bios2PcodeMailBoxWrite (Socket, MailBoxCommand, MailboxData);
}

/**
  Send command to pcode to quiesce and set mirrorfailover for the MC for DDRT

  @param Socket -- socket index
  @param Mc  -- Memory controller index
  @param ChBitmap -- channel bit map

  @return mailbox status
**/
UINT32
EFIAPI
DdrtFailoverMailbox (
  IN   UINT8        Socket,
  IN   UINT8        Mc,
  IN   UINT8        ChBitmap
  )
{
  UINT32                              MailBoxCommand;
  UINT32                              MailboxData;
  //
  /* B2P mailbox Interface:
   * Command[7:0]  = 0x81   RAS_MC_QUIESCE_UNQUIESCE
   * Data:
   * Sub Command [11:8] = 0x1  0 for ADDDC and 1 for Mirror Failover
   * Mc Index [15:12]  = Mc Number
   * DDR Failed Channel Index [19:16] - Channel number for Spare Enable
   * Chan Index [22:20] = Ch Number one hot encoded for Mirror failover
   * MC Mirror Enable [23] = 1 for Mirror failover operation
   * DDR Select [24]  = 1 for DDRT
  */
  //
  MailBoxCommand = (UINT32) MAILBOX_BIOS_CMD_RAS_QUIESCE_UNQUIESCE;
  MailboxData    = BIT8 | (Mc << 12) | (ChBitmap << 20) | BIT23 | BIT24 ;
  return Bios2PcodeMailBoxWrite (Socket, MailBoxCommand, MailboxData);
}


/**
 send VLS sparing mailbox command for callbackusage

 @param Socket -- socket index
 @param MailboxData -- mailbox data

 @return mailbox status
**/
UINT32
EFIAPI
SendVlsSparing4Callback (
  IN   UINT8    Socket,
  IN   UINT32   MailboxData
  )
{
  return Bios2PcodeMailBoxWrite (Socket, MAILBOX_BIOS_CMD_RAS_QUIESCE_UNQUIESCE, MailboxData);
}

/**
  get VLS sparing mailbox command.

 @param Mc -- Node index
 @param ChOnMc  -- channel on memory controller

 @return VLS sparing mailbox command
**/
UINT32
EFIAPI
GetRasQuiesceUnquiesceCmd (
  IN  UINT8    NodeId,
  IN  UINT8    ChOnMc
  )
{
  UINT8 Mc;
  Mc = NODE_TO_MC(NodeId);
  //
  // B2P mailbox Interface: Command Portion
  // Command[7:0]  = 0x81   RAS_MC_QUIESCE_UNQUIESCE
  // Sub Command [11:8] = 0 for ADDDC and 1 for Mirror Failover
  // Mc Index [15:12]  = Mc Number
  // DDR Failed Channel Index [19:16] - Channel number for Spare Enable
  // Chan Index [22:20] = Ch Number one hot encoded for Mirror Failover
  // MC Mirror Enable [23] = 1 for Mirror failover operation
  // DDR Select [24]  = 0 for DDR4 failover
  // B2P mailbox Interface: Data Portion
  // [32:0] = 0
  //
  return (((Mc << 12) | (ChOnMc << 16)) | MAILBOX_BIOS_CMD_RAS_QUIESCE_UNQUIESCE);
}

/**
 Get VLS sparing mailbox command data.

 @param Unused in this implementation of the library

 @return VLS sparing mailbox command data
**/
UINT32
EFIAPI
GetVlsSparingData (
  IN  UINT8    NodeId,
  IN  UINT8    ChOnMc,
  IN  BOOLEAN  IsPlusOneSparing
  )
{
  UINT8 Mc;
  Mc = NODE_TO_MC(NodeId);
  //
  // B2P mailbox Interface: Command Portion
  // Command[7:0]  = 0x81   RAS_MC_QUIESCE_UNQUIESCE
  // Sub Command [11:8] = 0 for ADDDC and 1 for Mirror Failover
  // Mc Index [15:12]  = Mc Number
  // DDR Failed Channel Index [19:16] - Channel number for Spare Enable
  // Chan Index [22:20] = Ch Number one hot encoded for Mirror Failover
  // MC Mirror Enable [23] = 1 for Mirror failover operation
  // DDR Select [24]  = 0 for DDR4 failover
  // B2P mailbox Interface: Data Portion
  // [32:0] = 0
  //
  return 0 ;
}

/**
  Set scrub interval in PCU via mailbox command.

  @param[in]  Socket          Socket number.
  @param[in]  Mc              MC number on socket.
  @param[in]  ScrubInterval   Value of the scrub interval.

  @retval EFI_SUCCESS         Function execution successful.
  @retval EFI_DEVICE_ERROR    Error in command execution.
**/
EFI_STATUS
EFIAPI
SetScrubIntervalMailbox (
  IN UINT8  Socket,
  IN UINT8  Mc,
  IN UINT32 ScrubInterval
  )
{
  UINT32  MailboxCommand;
  UINT32  MailboxData;
  UINT32  MailboxStatus;

  //
  // The command dword contains a bitmap of MCs. Set the bit for this MC.
  //
  MailboxCommand = (
    MAILBOX_BIOS_CMD_MC_PATROL_SCRUB_INTERVAL_CONFIG |
    1 << (MAILBOX_BIOS_CMD_MC_PATROL_SCRUB_INTERVAL_CONFIG_INTERFACE_MC_INSTANCE_MASK_LSB + Mc)
    );

  MailboxData = ScrubInterval;

  MailboxStatus = Bios2PcodeMailBoxWrite (Socket, MailboxCommand, MailboxData);
  if (MailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "SetScrubIntervalMailbox - failure! Error Code -> 0x%08x", MailboxStatus));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}
/**

  Read CR Proxy register via B2P MailBox

  @param Socket          - Socket Id
  @param ChOnSkt         - Channel number
  @param RegisterIndex   - CR Register Index
  @param *Data           - Buffer to read data

  @retval                - MailBox Status

**/
UINT32
ReadCRProxyMailBox (
  IN UINT8     Socket,
  IN UINT8     ChOnSkt,
  IN UINT8     RegisterIndex,
  IN UINT32    *Data
  )
{
  UINT32   ReadCommand;
  UINT8    Node;

  Node = SKT_CH_TO_NODE (Socket, ChOnSkt);

  //
  // B2P MailBox command Format :
  // [08:08] read_write, 0 = Read; 1 = Write
  // [16:09] CR index
  // [24:17] CR instance
  // [28:25] CR channel index
  //
  ReadCommand = MAILBOX_BIOS_CR_READ_CMD (NODE_TO_MC (Node), SKTCH_TO_NODECH(ChOnSkt), RegisterIndex);

  return  ReadWritePcuMailbox (
            Socket,
            ReadCommand,
            MAILBOX_BIOS_CMD_ZERO,
            Data,
            0,
            0
            );
}
/**

  Write to CR Proxy register via B2P MailBox

  @param Socket          - Socket Id
  @param ChOnSkt         - Channel number
  @param RegisterIndex   - CR Register Index
  @param Data            - Data to write

  @retval               -  MailBox Status

**/
UINT32
WriteCRProxyMailBox (
  IN UINT8     Socket,
  IN UINT8     ChOnSkt,
  IN UINT8     RegisterIndex,
  IN UINT32    Data
  )
{

  UINT32   WriteCommand;
  UINT8    Node;

  Node = SKT_CH_TO_NODE (Socket, ChOnSkt);

  //
  // B2P MailBox command Format :
  // [08:08] read_write, 0 = Read; 1 = Write
  // [16:09] CR index
  // [24:17] CR instance
  // [28:25] CR channel index
  //
  WriteCommand = MAILBOX_BIOS_CR_WRITE_CMD (NODE_TO_MC (Node), SKTCH_TO_NODECH(ChOnSkt), RegisterIndex);

  return ReadWritePcuMailbox (
           Socket,
           MAILBOX_BIOS_CMD_ZERO,
           WriteCommand,
           &Data,
           0,
           0
           );
}

/**

  Read CR Proxy register via B2P MailBox

  @param Socket          - Socket Id
  @param ChOnSkt         - Channel number
  @param RegisterIndex   - CR Register Index

  @retval                - Value read from Proxy Register

**/
UINT32
ReadCRProxyRegister (
  IN UINT8     Socket,
  IN UINT8     ChOnSkt,
  IN UINT8     RegisterIndex
  )
{
  UINT32   MailBoxStatus;
  UINT32   Data;

  MailBoxStatus = ReadCRProxyMailBox(Socket, ChOnSkt, RegisterIndex, &Data);
  if (MailBoxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "ReadCRProxyRegister - failure! Error Code -> 0x%08x", MailBoxStatus));
    return 0;
  }
  return Data;
}

/**

  Write to CR Proxy register via B2P MailBox

  @param Socket          - Socket Id
  @param ChOnSkt         - Channel number
  @param RegisterIndex   - CR Register Index
  @param Data            - Data to write

  @retval               - None

**/
VOID
WriteCRProxyRegister (
  IN UINT8     Socket,
  IN UINT8     ChOnSkt,
  IN UINT8     RegisterIndex,
  IN UINT32    Data
  )
{

  UINT32   MailBoxStatus;

  MailBoxStatus = WriteCRProxyMailBox (Socket, ChOnSkt, RegisterIndex, Data);
  if (MailBoxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "WriteCRProxyRegister - failure! Error Code -> 0x%08x", MailBoxStatus));
  }

}