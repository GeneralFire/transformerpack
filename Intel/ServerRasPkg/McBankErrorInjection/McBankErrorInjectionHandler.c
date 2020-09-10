/** @file
  This file is called in runtime of the Asynchronous Machine-check
  Error Injection(AMEI).

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

#include "McBankErrorInjection.h"


extern MCA_ERR_INJ_PARAM_BUFFER             *mMcejParam;
extern EFI_ACPI_MCEJ_TRIGGER_ACTION_TABLE   *mMcaInjTriggerAction;
extern EFI_ACPI_MCEJ_TRIGGER_ACTION_TABLE   mTriggerActionTable;

typedef struct {
  UINT32  MsrNumber;
  UINT64  MsrValue;
} MSR_UPADTE_STRUCT;

MSR_UPADTE_STRUCT mMsrUpdateStruc;

/**

  This function updates the DEBUG_ERR_INJ_CTL MSr 1e3 with the input requested.
  the request could be Enable/Disable Mcbanks write request or Enable /Disable MCA or CMCI sigannling request.

   @param i/p : MSR_UPADTE_STRUCT *MsrUpdateStruc :  MSR address and MSR value.

  @retval Success if MCEJ ACPI table is published else Failure

**/
VOID
EFIAPI
McbankUpdate (
  MSR_UPADTE_STRUCT *MsrUpdateStruc
  )
{
  UINT64      Data64;

  Data64 = AsmReadMsr64 (MsrUpdateStruc->MsrNumber);
  Data64 &= ~(3);
  Data64 |= MsrUpdateStruc->MsrValue;
  AsmWriteMsr64 (MsrUpdateStruc->MsrNumber, Data64);
}

/**

  This function invoked by the OS with input copmmand request in SET_CMD action entry

  @param i/p : i/p from MCEJ table:  SET_CMD action entry value =
  @param CMD_GENERATE_CMCI or CMD_GENERATE_MCA or
  @param MCBANK_OVERWRITE_EN or MCA_CMCI_SIGNAL_EN
  @param function I/P:
  @param IN  EFI_HANDLE                          DispatchHandle,
  @param IN  CONST EFI_SMM_SW_REGISTER_CONTEXT   *DispatchContext, OPTIONAL
  @param IN OUT VOID                             *CommBuffer,     OPTIONAL
  @param IN OUT UINTN                            *CommBufferSize  OPTIONAL


  @retval MCA_ERR_INJ_GET_CMD_STATUS action entry in MCEJ ACPI table is updated with status correponding to the input request.
  @retval 0x0 Success, 0x1 Unknown Failure, 0x2 Invalid Access

  Function returns - Success;

**/
EFI_STATUS
EFIAPI
McejSwSmiCallBack (
  IN        EFI_HANDLE                    DispatchHandle,
  IN  CONST EFI_SMM_SW_REGISTER_CONTEXT   *DispatchContext, OPTIONAL
  IN  OUT   VOID                          *CommBuffer,     OPTIONAL
  IN  OUT   UINTN                         *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT32      SetCmd;
  UINT32      InjCtrlMsrCmd;
  UINT8       i =0;
  UINTN       Thread = 0;
  BOOLEAN     McaCmciSignalCmdValid = FALSE;
  UINT64      Data64;

  RAS_DEBUG((LEVEL_FUNC_FLOW,"McejSwSmiCallBack!\n"));

  mMcejParam->BusyStatus = 0;
  mMcejParam->CmdStatus = 0;

  //
  // Invalidate the trigger action entries
  //
  CopyMem (mMcaInjTriggerAction, &mTriggerActionTable, sizeof (EFI_ACPI_MCEJ_TRIGGER_ACTION_TABLE));

  //
  // Read the command passed in the set command action structure
  //
  SetCmd = (UINT32) mMcejParam->CmdValue;

  //
  // Check MCA/CMCI signalling command passed in SET_COMMAND is valid or not
  //
  if ((SetCmd & (CMD_GENERATE_CMCI | CMD_GENERATE_MCA)) != 0) {
    if ((SetCmd & CMD_MCA_CMCI_SIGNAL_EN) != CMD_MCA_CMCI_SIGNAL_EN) {
      mMcejParam->CmdStatus = MCA_ERR_INJ_CMD_INVALID_ACCESS;
      McaCmciSignalCmdValid = FALSE;
      return EFI_SUCCESS;
    } else {
      McaCmciSignalCmdValid = TRUE;
    }
  }

  //
  // Set Bit0(MCBW_E) and Bit1(MCA_CMCI_SE)
  //
  mMsrUpdateStruc.MsrNumber  = MSR_DEBUG_ERR_INJ_CTL;
  InjCtrlMsrCmd = SetCmd & 03;
  mMsrUpdateStruc.MsrValue   = InjCtrlMsrCmd;

  for (Thread = 0; Thread < gSmst->NumberOfCpus; Thread++) {
    Status = gSmst->SmmStartupThisAp (McbankUpdate, Thread, &mMsrUpdateStruc);
  }

  McbankUpdate (&mMsrUpdateStruc);

  //
  // Read back the DEBUG_ERR_INJ_CTL MSR value to chekc if the MC bank write capability or MCA/CMCI signaling capability bits are set.
  // check Bit0(MCBW_E) or Bit1(MCA_CMCI_SE) is set? If not, could be feature is not supported.
  //
  Data64 = AsmReadMsr64 (MSR_DEBUG_ERR_INJ_CTL);

  if ((Data64 & (MCBANK_OVERWRITE_EN | MCA_CMCI_SIGNAL_EN )) == 0) {
      mMcejParam->CmdStatus = MCA_ERR_INJ_CMD_INVALID_ACCESS;
      return EFI_SUCCESS;
  }

  i =0;
  //
  // Bit0(MCBW_E) and Bit1(MCA_CMCI_SE)  of DEBUG_ERR_INJ_CTL are valid
  //
  if ((SetCmd & CMD_GENERATE_CMCI) != 0) {

    mMcaInjTriggerAction->Trigger[i].Operation                  = MCA_ERR_INJ_TRIGGER_ERROR_ACTION;
    mMcaInjTriggerAction->Trigger[i].Instruction                = INSTRUCTION_WRITE_REGISTER_VALUE;
    mMcaInjTriggerAction->Trigger[i].Flags                      = FLAG_NOTHING;
    mMcaInjTriggerAction->Trigger[i].Reserved8                  = 00;
    mMcaInjTriggerAction->Trigger[i].Register.AddressSpaceId    = EFI_ACPI_6_2_FUNCTIONAL_FIXED_HARDWARE;
    mMcaInjTriggerAction->Trigger[i].Register.RegisterBitWidth  = 0x40;
    mMcaInjTriggerAction->Trigger[i].Register.RegisterBitOffset = 0x00;
    mMcaInjTriggerAction->Trigger[i].Register.AccessSize        = EFI_ACPI_6_2_QWORD;
    //
    // Bit0(MCA_G ) and BIT1(CMCI_G)
    //
    mMcaInjTriggerAction->Trigger[i].Register.Address           = MSR_DEBUG_ERR_INJ_CTL2;
    mMcaInjTriggerAction->Trigger[i].Value                      = GENERATE_CMCI;
    mMcaInjTriggerAction->Trigger[i].Mask                       = GENERATE_CMCI;
    i++;
  }

 if ((SetCmd & CMD_GENERATE_MCA) != 0) {

    mMcaInjTriggerAction->Trigger[i].Operation                  = MCA_ERR_INJ_TRIGGER_ERROR_ACTION;
    mMcaInjTriggerAction->Trigger[i].Instruction                = INSTRUCTION_WRITE_REGISTER_VALUE;
    mMcaInjTriggerAction->Trigger[i].Flags                      = FLAG_NOTHING;
    mMcaInjTriggerAction->Trigger[i].Reserved8                  = 00;
    mMcaInjTriggerAction->Trigger[i].Register.AddressSpaceId    = EFI_ACPI_6_2_FUNCTIONAL_FIXED_HARDWARE;
    mMcaInjTriggerAction->Trigger[i].Register.RegisterBitWidth  = 0x40;
    mMcaInjTriggerAction->Trigger[i].Register.RegisterBitOffset = 0x00;
    mMcaInjTriggerAction->Trigger[i].Register.AccessSize        = EFI_ACPI_6_2_QWORD;
    //
    // Bit0(MCA_G ) and BIT1(CMCI_G)
    //
    mMcaInjTriggerAction->Trigger[i].Register.Address           = MSR_DEBUG_ERR_INJ_CTL2;
    mMcaInjTriggerAction->Trigger[i].Value                      = GENERATE_MCA;
    mMcaInjTriggerAction->Trigger[i].Mask                       = GENERATE_MCA;
  }

  return EFI_SUCCESS;
}
