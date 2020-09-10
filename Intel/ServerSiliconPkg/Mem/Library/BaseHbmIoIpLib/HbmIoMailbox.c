/** @file
  Source code file for Gfsp HBMIO Training for SPR HBM

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation.

  This software and the related documents are Intel copyrighted materials, and
  your use of them is governed by the express license under which they were provided
  to you ("License"). Unless the License provides otherwise, you may not use, modify,
  copy, publish, distribute, disclose or transmit this software or the related documents
  without Intel's prior written permission.

  This software and the related documents are provided as is, with no express or implied
  warranties, other than those that are expressly stated in the License.

  @par Specification
**/
#include "Include/HbmIoIpInternal.h"
#include <Library/PcuMailBoxLib.h>

HBMIO_RUNNING_MODE
GetHbmIoMode (
  VOID
  )
{
  //
  // HBM todo Need to put this into HbmCore Host structure.
  //
  return HBMIO_RUNNING_MODE_EMULATION_TEST;

}

VOID
PrintHbmioMboxRspStatus (
  IN HBMIO_MBX_OPCODES    HbmMbOpcode,
  IN HBMIO_RETURN_STATUS  HbmMbReturn,
  IN UINT32               RspData0,
  IN UINT32               RspData1
  )
{
  switch (HbmMbReturn)  {
  case HBM_MBX_NO_ERROR:
    HbmDebugPrintLine ("HbmioMailbox: Case No Error\n");
    break;

  case HBM_MBX_INV_OP:
    HbmDebugPrintLine ("HbmioMailbox: Case Invalid Opcode\n");
    break;

  case HBM_MBX_FW_CHK_FAIL:
    HbmDebugPrintLine ("HbmioMailbox: Case FW Checks Fail\n");
    break;

  case HBM_MBX_TRAINING_FAIL:
    HbmDebugPrintLine ("HbmioMailbox: Case Training Fail\n");
    break;

  case HBM_MBX_WATCHDOG_TMOUT:
    HbmDebugPrintLine ("HbmioMailbox: Case Watchdog timeout\n");
    break;

  case HBM_MBX_BAD_FW:
    HbmDebugPrintLine ("HbmioMailbox: Case Bad firmware\n");
    break;

  default:
    HbmDebugPrintLine ("HbmioMailbox: Error in Mailbox return\n");
    break;
  }

  HbmDebugPrintDevice (HBM_SDBG_NONE, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "HbmioMailbox Status 0x%x\n", HbmMbReturn);

  if (HbmMbOpcode == HBM_MBOX_DLL_CTRL_OPCODE) {
    HbmDebugPrintDevice (HBM_SDBG_NONE, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Hbmio - RspData HBMSS0 Data0: 0x%X\n", RspData0);
    HbmDebugPrintDevice (HBM_SDBG_NONE, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Hbmio - RspData HBMSS0 Data1: 0x%X\n", RspData1);
  }
}

/**
    @brief Executing Hbmio mailbox by setting the runbusy bit
    writting to the hbmio ip and offset for each tile

    @param[In] HbmIoId&interface
*/
EFI_STATUS
EFIAPI
HbmioExecuteMbox (
  IN UINT8                Socket,
  IN UINT8                HbmIoId,
  IN HBMIO_MBX_OPCODES    HbmMbOpcode,
  IN MBOX_PARAMETER       HbmMbParam,
  IN OUT UINT8            *Data,
  IN OUT UINT32           DataLength
  )
{
  UINT32 pollingTimeElapsed = 0;
  UINT32 HbmData0SS0 = 0;
  UINT32 HbmData1SS0 = 0;

  UC_BIOS_MAILBOX_CTL_CREG_HBM_MIDSTACKGBL_STRUCT MboxCtlCreg;

  MboxCtlCreg.Data = 0;
  MboxCtlCreg.Bits.run_busy    = BIT_SET;
  MboxCtlCreg.Bits.opcode      = HbmMbOpcode;
  MboxCtlCreg.Bits.parameters  = HbmMbParam;

  if (FixedPcdGetBool (PcdHbmIoCrWritable)) {
    IoRegisterWrite (MemTechHbm, Socket, HbmIoId, UC_BIOS_MAILBOX_CTL_CREG_HBM_MIDSTACKGBL_REG, MboxCtlCreg.Data);
  } else {
    PcuWriteCrProxyReg (Socket, HbmIoId, 0, MAILBOX_BIOS_CMD_CR_PROXY_UC_BIOS_MAILBOX_CTL_CREG_INDEX, MboxCtlCreg.Data);
  }

  //wait until uC FW slave is done
  do {
    MboxCtlCreg.Data = IoRegisterRead (MemTechHbm, Socket, HbmIoId, UC_BIOS_MAILBOX_CTL_CREG_HBM_MIDSTACKGBL_REG);

    FixedDelayMicroSecond (HBMIO_POLLING_INTERVAL);
    pollingTimeElapsed += HBMIO_POLLING_INTERVAL;
  } while ((BIT_SET == MboxCtlCreg.Bits.run_busy) && (pollingTimeElapsed <= HBMIO_MAILBOX_TIMEOUT));

  if (BIT_CLR == MboxCtlCreg.Bits.run_busy)  {
    // MboxCtlCreg.Bits.opcode == HBM_MBOX_DLL_CTRL_OPCODE
    HbmData0SS0 = IoRegisterRead (MemTechHbm, Socket, HbmIoId, UC_BIOS_MAILBOX_DATA0_CREG_HBM_MIDSTACKGBL_REG);
    HbmData1SS0 = IoRegisterRead (MemTechHbm, Socket, HbmIoId, UC_BIOS_MAILBOX_DATA1_CREG_HBM_MIDSTACKGBL_REG);

    if (Data == NULL) {
      DataLength = 0;
    } else if (DataLength <= 4) {
      CopyMem ((VOID *)Data, &HbmData0SS0, DataLength);
    } else if ((DataLength > 4) && (DataLength <= 8)) {
      CopyMem ((VOID *)Data, &HbmData0SS0, 4);
      CopyMem ((VOID *) (Data + 4), &HbmData1SS0, DataLength - 4);
    } else {
      HbmDebugPrintLine ("HbmioMailbox - currently don't support bigger than 8 bytes of data read, this API need expanding for logic. \n");
      ASSERT (FALSE);
    }
  }

  if (pollingTimeElapsed < HBMIO_MAILBOX_TIMEOUT) {
    PrintHbmioMboxRspStatus (HbmMbOpcode, MboxCtlCreg.Bits.return_code, HbmData0SS0, HbmData1SS0);
  }  else  {
    HbmDebugPrintLine ("HbmioMailbox -timeout reached HBMIO_MAILBOX_TIMEOUT \n");

    if (!MboxCtlCreg.Bits.return_code) {
      //Return code never got set.  Set to internal polling timeout code
      MboxCtlCreg.Bits.return_code = HBM_MBX_POLLING_TMOUT;
    }
  }

  return (UINT8)MboxCtlCreg.Bits.return_code;
}

UINT32
HbmioExecuteMboxAllIo (
  IN UINT8                Socket,
  IN HBMIO_MBX_OPCODES    HbmMbOpcode,
  IN MBOX_PARAMETER       HbmMbParam
  )
{
  UINT32 StatusCummulator = 0;
  UINT8  HbmIoId = 0;
  UINT8  HbmIoCount = 0;

  HbmIoCount = GetMaxIoHbm();

  for (HbmIoId = 0 ; HbmIoId < HbmIoCount ; HbmIoId++) {
    StatusCummulator += (UINT32) HbmioExecuteMbox (Socket, HbmIoId, HbmMbOpcode, HbmMbParam, NULL, 0);
  }

  if (StatusCummulator != 0) {
    HbmDebugPrintLine ("ERROR in HbmioExecuteMboxAllIo()\n");
  }

  return StatusCummulator;
}

/**
    @brief Disable Hbmio Controller

    Disable Hbmio Controller by clearing uc_en and uc_clk_en bits
    from HBMIO Midstack uC Hardware Control

*/
EFI_STATUS
HbmioMailboxDisable (
  IN UINT8                Socket,
  IN UINT8                HbmIoId
  )
{
  EFI_STATUS                                      Status = EFI_SUCCESS;
  UC_HW_CONTROL_CREG_HBM_MIDSTACKGBL_STRUCT       MboxCregControl;
  MBOX_PARAM_INITIALIZE                           TestMboxParam;
  UC_BIOS_MAILBOX_CTL_CREG_HBM_MIDSTACKGBL_STRUCT MboxCtlCregSS;

  HbmDebugPrintLine ("HbmioMailbox - Disabling HBMIO uController\n");
  //
  // Disabling HBM uController after mailbox commands are processed
  //
  MboxCregControl.Data           = IoRegisterRead (MemTechDdr, Socket, HbmIoId, UC_HW_CONTROL_CREG_HBM_MIDSTACKGBL_REG);
  MboxCregControl.Bits.uc_en     = BIT_CLR;
  MboxCregControl.Bits.uc_clk_en = BIT_CLR;

  IoRegisterWrite (MemTechDdr, Socket, HbmIoId, UC_HW_CONTROL_CREG_HBM_MIDSTACKGBL_REG, MboxCregControl.Data);

  //
  // Running Initialize MBOX to verify hbm controller is disabled.
  //
  TestMboxParam.Data = 0;
  TestMboxParam.Bits.SkipAllInternal = 1;

  HbmDebugPrintLine ("HbmioMailbox -In Mailbox Initialize \n");
  HbmioExecuteMbox (Socket, HbmIoId, HBM_MBOX_INITIALIZE_OPCODE, TestMboxParam.Data, NULL, 0);

  MboxCtlCregSS.Data = IoRegisterRead (MemTechDdr, Socket, HbmIoId, UC_BIOS_MAILBOX_CTL_CREG_HBM_MIDSTACKGBL_REG);

  if (BIT_SET == MboxCtlCregSS.Bits.run_busy) {
    HbmDebugPrintDevice (HBM_SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                         "HBM didn't respond when HBM controller is disabled. HBMSS%d: 0x%X\n", HbmIoId, MboxCtlCregSS.Data);
  } else   {
    HbmDebugPrintDevice (HBM_SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                         "ERROR - HBM controller has responded when it's disabled. HBMSS%d: 0x%X\n", HbmIoId, MboxCtlCregSS.Data);
    Status = EFI_UNSUPPORTED;
  }

  return Status;
}

EFI_STATUS
HbmioMailboxDisableAll (
  IN UINT8 Socket
  )
{
  UINT8            HbmIoId, HbmIoCount;
  EFI_STATUS       Status = EFI_SUCCESS;

  HbmIoCount = GetMaxIoHbm();

  for (HbmIoId = 0 ; HbmIoId < HbmIoCount ; HbmIoId ++) {
    HbmDebugPrintLine ("HbmioMailboxDisableAll HBMIO\n");
    Status = HbmioMailboxDisable (Socket, HbmIoId);
  }

  return Status;
}

EFI_STATUS
HbmioMailboxInitialize (
  UINT8 Socket
  )
{
  EFI_STATUS                Status = EFI_SUCCESS;
  MBOX_PARAM_INITIALIZE     TestMboxParam;

  TestMboxParam.Data = 0;
  TestMboxParam.Bits.SkipAllInternal = 1;

  HbmDebugPrintLine ("HbmioMailbox -In Mailbox Initialize \n");

  Status = HbmioExecuteMboxAllIo (Socket, HBM_MBOX_INITIALIZE_OPCODE, TestMboxParam.Data);
  return Status;
}

EFI_STATUS
HbmioDllCtrl (UINT8 Socket)
{

  MBOX_PARAM_DLL_CTRL  TestMboxParam;
  EFI_STATUS           Status = EFI_SUCCESS;

  TestMboxParam.Data = 0;
  TestMboxParam.Bits.Enable = 1;

  HbmDebugPrintLine ("HbmioMailbox -In Mailbox DllCtrl \n");

  Status = HbmioExecuteMboxAllIo (Socket, HBM_MBOX_DLL_CTRL_OPCODE, TestMboxParam.Data);
  return Status;
}

EFI_STATUS
HbmioMailboxResetWrstNCtrl (
  IN UINT8 Socket
  )
{

  MBOX_PARAM_RESET_WRST_N_CTRL TestMboxParam;
  EFI_STATUS                   Status = EFI_SUCCESS;

  TestMboxParam.Data     = 0;
  TestMboxParam.Bits.DeAssert = 1;

  HbmDebugPrintLine ("HbmioMailbox -In Mailbox Reset Wrst & Ctrl \n");

  Status = HbmioExecuteMboxAllIo (Socket, HBM_MBOX_RESET_WRST_N_CTRL_OPCODE, TestMboxParam.Data);
  return Status;
}

EFI_STATUS
HbmioMailboxHbmPhyConfig (
  UINT8 Socket
  )
{
  EFI_STATUS                                      Status = EFI_SUCCESS;
  MBOX_PARAM_HBM_PHY_CONFIG                       TestMboxParam;

  TestMboxParam.Data = 0;
  TestMboxParam.Bits.ChannelEnable = 1;
  HbmDebugPrintLine ("HbmioMailbox -In Mailbox Hbm Phy Config \n");

  Status = HbmioExecuteMboxAllIo (Socket, HBM_MBOX_HBM_PHY_CONFIG_OPCODE, TestMboxParam.Data);
  return Status;
}

EFI_STATUS
HbmioMailboxDA28LockSecurity (
  UINT8 Socket
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  HbmDebugPrintLine ("HbmioMailbox -In Mailbox DA28 Lock Security \n");

  Status = HbmioExecuteMboxAllIo (Socket, HBM_MBOX_DA28_LOCK_SECURITY_OPCODE, 0);

  return Status;
}

EFI_STATUS
HbmioMailboxPhyInitComplete (
  UINT8 Socket
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  HbmDebugPrintLine ("HbmioMailbox -In Mailbox Phy Init Complete \n");

  Status = HbmioExecuteMboxAllIo (Socket, HBM_MBOX_PHY_INIT_COMPLETE_OPCODE, 0);

  return Status;
}

/**
  This function reads the Device ID data by issuing ieee1500_read_device_id HBMIO mailbox command.

  @param[in]  SocketId                Socket index.
  @param[in]  HbmIoId                 HBM IO index on socket.
  @param[out] DeviceIdData            Pointer to the returned Device ID data

  @retval EFI_SUCCESS                 This function read the the Device ID data successfully.
  @retval EFI_NOT_READY               There is no Device ID data pending upon return.
  @retval EFI_INVALID_PARAMETER       Some of input parameters are invalid.
  @retval EFI_UNSUPPORTED             This operation is not supported.
**/
EFI_STATUS
EFIAPI
HbmioMailboxReadDeviceId (
  IN  UINT8           SocketId,
  IN  UINT8           HbmIoId,
  OUT DEVICE_ID_WDR   *DeviceIdData
  )
{
  EFI_STATUS                          Status;
  UINT64_STRUCT                       MBoxData;
  MBOX_PARAM_IEEE1500_READ_DEVICE_ID  MboxParam;

  if (DeviceIdData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MboxParam.Data = 0;
  HbmDebugPrint (HBM_SDBG_MINMAX, "HbmioMailbox -IEEE 1500 Read Device ID for Socket %d HbmId %d\n", SocketId, HbmIoId);

  //
  // Get the first 64-bits of Device ID data.
  //
  MboxParam.Bits.MsbSel = 0;
  Status = HbmioExecuteMbox (SocketId, HbmIoId, HBM_MBOX_IEEE_1500_READ_DEVID_OPCODE, MboxParam.Data, (UINT8 *) (&MBoxData), sizeof (MBoxData));
  if (Status != HBM_MBX_NO_ERROR) {
    return EFI_NOT_READY;
  }
  DeviceIdData->Data[0] = MBoxData.lo;
  DeviceIdData->Data[1] = MBoxData.hi;

  //
  // Get the remaining 18-bits of Device ID data.
  //
  MboxParam.Bits.MsbSel = 1;
  Status = HbmioExecuteMbox (SocketId, HbmIoId, HBM_MBOX_IEEE_1500_READ_DEVID_OPCODE, MboxParam.Data, (UINT8 *) (&MBoxData.lo), sizeof (MBoxData.lo));
  if (Status != HBM_MBX_NO_ERROR) {
    return EFI_NOT_READY;
  }
  DeviceIdData->Data[2] = MBoxData.lo;

  //
  // HBM_TODO 1507709639: Add BaseHbmIoIpLib implementation for RcSim
  //

  return EFI_SUCCESS;
}

EFI_STATUS
HbmioMailboxLaneRepairLoad (
  UINT8 Socket
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  HbmDebugPrintLine ("HbmioMailbox -In Mailbox Lane Repair Load \n");

  Status = HbmioExecuteMboxAllIo (Socket, HBM_MBOX_LANE_REPAIR_LOAD_OPCODE, 0);

  return Status;
}

EFI_STATUS
HbmioMboxFullProdTraining (
  UINT8 Socket
  )
{
  MBOX_PARAMETER TestMboxParam;
  EFI_STATUS     Status = EFI_SUCCESS;

  // opcode order:  0x0B->0xC->0xD->0xE->0xF

  HbmDebugPrintLine ("HbmioMailbox -In Mailbox Full Prod Training \n");

  TestMboxParam = 0;
  HbmDebugPrintLine ("HbmioMailbox -HBM_MBOX_LANE_REPAIR_LOAD_OPCODE \n");
  Status = HbmioExecuteMboxAllIo (Socket, HBM_MBOX_LANE_REPAIR_LOAD_OPCODE, TestMboxParam);

  HbmDebugPrintLine ("HbmioMailbox -HBM_MBOX_TRAIN_XOVER_OPCODE \n");
  Status = HbmioExecuteMboxAllIo (Socket, HBM_MBOX_TRAIN_XOVER_OPCODE, TestMboxParam);

  HbmDebugPrintLine ("HbmioMailbox -HBM_MBOX_TRAIN_AWORD_LFSR_OPCODE \n");
  Status = HbmioExecuteMboxAllIo (Socket, HBM_MBOX_TRAIN_AWORD_LFSR_OPCODE, TestMboxParam);

  HbmDebugPrintLine ("HbmioMailbox -HBM_MBOX_TRAIN_DWORD_TX_LFSR_OPCODE \n");
  Status = HbmioExecuteMboxAllIo (Socket, HBM_MBOX_TRAIN_DWORD_TX_LFSR_OPCODE, TestMboxParam);

  HbmDebugPrintLine ("HbmioMailbox -HBM_MBOX_TRAIN_DWORD_RX_LFSR_OPCODE \n");
  Status = HbmioExecuteMboxAllIo (Socket, HBM_MBOX_TRAIN_DWORD_RX_LFSR_OPCODE, TestMboxParam);

  return Status;
}

EFI_STATUS
HbmioMboxProdTraining (
  UINT8 Socket
  )
{
  MBOX_PARAMETER   TestMboxParam;
  EFI_STATUS       Status = EFI_SUCCESS;

  HbmDebugPrintLine ("HbmioMailbox -In Mailbox Prod Training \n");

  TestMboxParam = 0;
  Status = HbmioExecuteMboxAllIo (Socket, HBM_MBOX_FULL_PROD_TRAINING_OPCODE, TestMboxParam);

  return Status;
}

EFI_STATUS
HbmioMboxFullHvmSort (
  UINT8 Socket
  )
{
  MBOX_PARAMETER   TestMboxParam;
  EFI_STATUS       Status = EFI_SUCCESS;

  //
  // opcode order: 0x10->0xF
  //
  TestMboxParam   = 0;
  HbmDebugPrintLine ("HbmioMailbox -In Mailbox Full HVM Sort - HBM_MBOX_TRAIN_ACIO_LB_OPCODE\n");
  Status = HbmioExecuteMboxAllIo (Socket, HBM_MBOX_TRAIN_ACIO_LB_OPCODE, TestMboxParam);

  HbmDebugPrintLine ("HbmioMailbox -In Mailbox Full HVM Sort - HBM_MBOX_TRAIN_DWORD_RX_LFSR_OPCODE\n");
  Status = HbmioExecuteMboxAllIo (Socket, HBM_MBOX_TRAIN_DWORD_RX_LFSR_OPCODE, TestMboxParam);

  return Status;
}

/**
  HBMIO Mailbox Physical Initialization Start.

  @param[in] Socket       Socket Index

  @retval EFI_SUCCESS     This function was executed successfully.
  @retval !EFI_SUCCESS    This function failed to be executed.
**/
EFI_STATUS
EFIAPI
HbmioMboxPhyInitStart (
  UINT8 Socket
  )
{
  //
  // opcode order: 0x0->0x1->0x2->0x3
  //
  EFI_STATUS Status = EFI_SUCCESS;

  Status = HbmioMailboxInitialize (Socket);
  HbmDebugPrintDevice (HBM_SDBG_NONE, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"HbmioMboxInitialize return Status: 0x%X\n", Status);

  Status = HbmioDllCtrl (Socket);
  HbmDebugPrintDevice (HBM_SDBG_NONE, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"HbmioDllCtrl return Status: 0x%X\n", Status);

  Status = HbmioMailboxResetWrstNCtrl (Socket);
  HbmDebugPrintDevice (HBM_SDBG_NONE, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"HbmioMailboxResetWrstNCtrl return Status: 0x%X\n", Status);

  Status = HbmioMailboxHbmPhyConfig(Socket);
  HbmDebugPrintDevice (HBM_SDBG_NONE, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"HbmioMailboxHbmPhyConfig return Status: 0x%X\n", Status);

  return Status;
}

EFI_STATUS
HbmioMboxPhyInitEnd (
  UINT8 Socket
  )
{
  //
  // opcode order: 0x4->0x5
  //
  EFI_STATUS Status = EFI_SUCCESS;

  Status = HbmioMailboxDA28LockSecurity (Socket);
  HbmDebugPrintDevice (HBM_SDBG_NONE, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"HbmioMailboxDA28LockSecurity return Status: 0x%X\n", Status);

  Status = HbmioMailboxPhyInitComplete (Socket);
  HbmDebugPrintDevice (HBM_SDBG_NONE, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"HbmioMailboxPhyInitComplete return Status: 0x%X\n", Status);

  return Status;
}

/**

  HbmIo Mailbox Initialize API

  @param[In]               Socket              Init the HbmIo instances for a specific socket
  @param[In]               DisableuC           Whether to disable the uC of HBM after init

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
HbmioMboxInit (
  UINT8   Socket,
  BOOLEAN DisableuC
  )
{
  EFI_STATUS         Status = EFI_SUCCESS;
  HBMIO_RUNNING_MODE RunningMode;

  RunningMode = GetHbmIoMode();

  //
  // Do middle tasks according to prod or hvm mode
  //
  switch (RunningMode) {

  case HBMIO_RUNNING_MODE_EMULATION_TEST:
    // do nothing;
    break;

  case HBMIO_RUNNING_MODE_FULL_HVM_SORT:
    HbmioMboxFullHvmSort (Socket);
    break;

  case HBMIO_RUNNING_MODE_FULL_PROD_TRAINING:
  case HBMIO_RUNNING_MODE_FULL_HVM_CLASS: //TBD
    HbmioMboxFullProdTraining (Socket);
    break;

  case HBMIO_RUNNING_MODE_PROD_TRAINING:
    HbmioMboxProdTraining (Socket);
    break;

  default:
    break;
  }

  if (RunningMode != HBMIO_RUNNING_MODE_PROD_TRAINING) {
    HbmioMboxPhyInitEnd (Socket);
  }

  if (DisableuC) {
    Status = HbmioMailboxDisableAll (Socket);
    HbmDebugPrintDevice (HBM_SDBG_NONE, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"HbmioMboxDisable return Status: 0x%X\n", Status);
  }

  return Status;
}
