/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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
#include <Include/MdfisIpRegisters.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MdfisIpLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/PcuMailBoxLib.h>



enum MDFIS_CREG_INDEX {
  MDFIS_STANDALONE_SEQUENCE_REG2_LOWER_CREG_INDEX = 0,
  MDFIS_STANDALONE_SEQUENCE_REG2_UPPER_CREG_INDEX,
  MDFIS_DIE2DIE_SEQUENCE_REG2_LOWER_CREG_INDEX,
  MDFIS_DIE2DIE_SEQUENCE_REG2_UPPER_CREG_INDEX,
  MDFIS_STANDALONE_SEQUENCE_HI_GV_REG0_LOWER_CREG_INDEX,
  MDFIS_STANDALONE_SEQUENCE_HI_GV_REG0_UPPER_CREG_INDEX,
  MDFIS_STANDALONE_SEQUENCE_HI_GV_REG1_LOWER_CREG_INDEX,
  MDFIS_STANDALONE_SEQUENCE_HI_GV_REG1_UPPER_CREG_INDEX,
  MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG0_LOWER_CREG_INDEX,
  MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG0_UPPER_CREG_INDEX,
  MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG1_LOWER_CREG_INDEX,
  MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG1_UPPER_CREG_INDEX,
  MDFIS_RXOFF_PHASE_CTRL_CREG_INDEX,
  MDFIS_RCOMP_PHASE_CTRL_CREG_INDEX,
  MDFIS_ILW_PHASE_CTRL_CREG_INDEX,
  MDFIS_DCC_PHASE_CTRL_CREG_INDEX,
  MDFIS_SLOOP_PHASE_CTRL_CREG_INDEX,
  MDFIS_LLOOP_PHASE_CTRL_CREG_INDEX,
  MDFIS_RXVREF_PHASE_CTRL_CREG_INDEX,
  MDFIS_COUNTER_1MS_CTRL_CREG_INDEX,
  MDFIS_TRAINING_FSM_CTRL0_CREG_INDEX,
};

UINT32 MdfisRegAndCRProxyIndexMappingTable [][2] = {
  // 0: MDFIS_STANDALONE_SEQUENCE_REG2_LOWER_CREG_INDEX
  {
    MDFIS_STANDALONE_SEQUENCE_REG2_LOWER_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_STANDALONE_SEQUENCE_REG2_LOWER_CREG_INDEX
  },
  // 1: MDFIS_STANDALONE_SEQUENCE_REG2_UPPER_CREG_INDEX
  {
    MDFIS_STANDALONE_SEQUENCE_REG2_UPPER_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_STANDALONE_SEQUENCE_REG2_UPPER_CREG_INDEX
  },
  // 2: MDFIS_DIE2DIE_SEQUENCE_REG2_LOWER_CREG_INDEX
  {
    MDFIS_DIE2DIE_SEQUENCE_REG2_LOWER_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_DIE2DIE_SEQUENCE_REG2_LOWER_CREG_INDEX
  },
  // 3: MDFIS_DIE2DIE_SEQUENCE_REG2_UPPER_CREG_INDEX
  {
    MDFIS_DIE2DIE_SEQUENCE_REG2_UPPER_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_DIE2DIE_SEQUENCE_REG2_UPPER_CREG_INDEX
  },
  // 4: MDFIS_STANDALONE_SEQUENCE_HI_GV_REG0_LOWER_CREG_INDEX
  {
    MDFIS_STANDALONE_SEQUENCE_HI_GV_REG0_LOWER_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_STANDALONE_SEQUENCE_HI_GV_REG0_LOWER_CREG_INDEX
  },
  // 5: MDFIS_STANDALONE_SEQUENCE_HI_GV_REG0_UPPER_CREG_INDEX
  {
    MDFIS_STANDALONE_SEQUENCE_HI_GV_REG0_UPPER_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_STANDALONE_SEQUENCE_HI_GV_REG0_UPPER_CREG_INDEX
  },
  // 6: MDFIS_STANDALONE_SEQUENCE_HI_GV_REG1_LOWER_CREG_INDEX
  {
    MDFIS_STANDALONE_SEQUENCE_HI_GV_REG1_LOWER_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_STANDALONE_SEQUENCE_HI_GV_REG1_LOWER_CREG_INDEX
  },
  // 7: MDFIS_STANDALONE_SEQUENCE_HI_GV_REG1_UPPER_CREG_INDEX
  {
    MDFIS_STANDALONE_SEQUENCE_HI_GV_REG1_UPPER_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_STANDALONE_SEQUENCE_HI_GV_REG1_UPPER_CREG_INDEX
  },
  // 8: MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG0_LOWER_CREG_INDEX
  {
    MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG0_LOWER_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG0_LOWER_CREG_INDEX
  },
  // 9: MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG0_UPPER_CREG_INDEX
  {
    MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG0_UPPER_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG0_UPPER_CREG_INDEX
  },
  // 10: MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG1_LOWER_CREG_INDEX
  {
    MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG1_LOWER_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG1_LOWER_CREG_INDEX
  },
  // 11: MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG1_UPPER_CREG_INDEX
  {
    MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG1_UPPER_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_DIE2DIE_SEQUENCE_HI_GV_REG1_UPPER_CREG_INDEX
  },
  // 12：MDFIS_RXOFF_PHASE_CTRL_CREG_INDEX
  {
    MDFIS_RXOFF_PHASE_CTRL_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_RXOFF_PHASE_CTRL_CREG_INDEX
  },
  // 13: MDFIS_RCOMP_PHASE_CTRL_CREG_INDEX
  {
    MDFIS_RCOMP_PHASE_CTRL_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_RCOMP_PHASE_CTRL_CREG_INDEX
  },
  // 14: MDFIS_ILW_PHASE_CTRL_CREG_INDEX
  {
    MDFIS_ILW_PHASE_CTRL_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_ILW_PHASE_CTRL_CREG_INDEX
  },
  // 15: MDFIS_DCC_PHASE_CTRL_CREG_INDEX
  {
    MDFIS_DCC_PHASE_CTRL_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_DCC_PHASE_CTRL_CREG_INDEX
  },
  // 16: MDFIS_SLOOP_PHASE_CTRL_CREG_INDEX
  {
    MDFIS_SLOOP_PHASE_CTRL_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_SLOOP_PHASE_CTRL_CREG_INDEX
  },
  // 17: MDFIS_LLOOP_PHASE_CTRL_CREG_INDEX
  {
    MDFIS_LLOOP_PHASE_CTRL_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_LLOOP_PHASE_CTRL_CREG_INDEX
  },
  // 18: MDFIS_RXVREF_PHASE_CTRL_CREG_INDEX
  {
    MDFIS_RXVREF_PHASE_CTRL_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_RXVREF_PHASE_CTRL_CREG_INDEX
  },
  // 19: MDFIS_COUNTER_1MS_CTRL_CREG_INDEX
  {
    MDFIS_COUNTER_1MS_CTRL_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_COUNTER_1MS_CTRL_CREG_INDEX
  },
  // 20: MDFIS_TRAINING_FSM_CTRL0_CREG_INDEX
  {
    MDFIS_TRAINING_FSM_CTRL0_CREG_MDFIS_MAIN_REG,
    MAILBOX_BIOS_CMD_CR_PROXY_MDFIS_TRAINING_FSM_CTRL0_CREG_INDEX
  },
};


/**
  This API performs silicon CSR register read operations.

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] BoxInst            Box Instance, 0 based
  @param[in] CsrIndex           Register Index

  @retval UINT32                The data from CSR register returned.
**/
UINT32
EFIAPI
MdfisCsrRead (
  IN  UINT8              SocketId,
  IN  UINT8              BoxInst,
  IN  UINT32             CsrIndex
  )
{
  UINT32     Data32;

  if (IsSiliconWorkaroundEnabled ("S1409517281")) {
    PcuReadCrProxyReg (
      SocketId,
      BoxInst,
      0,
      (UINT8) MdfisRegAndCRProxyIndexMappingTable [CsrIndex][1],
      &Data32
      );
  } else {
    Data32 = UsraCsrRead (SocketId, BoxInst, MdfisRegAndCRProxyIndexMappingTable [CsrIndex][0]);
  }

  return Data32;
}

/**
  This API performs silicon CSR register write operations.

  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] BoxInst            Box Instance, 0 based
  @param[in] CsrIndex           Register Index
  @param[in] Data32             The data needs to be written to CSR.

**/
VOID
EFIAPI
MdfisCsrWrite (
  IN  UINT8              SocketId,
  IN  UINT8              BoxInst,
  IN  UINT32             CsrIndex,
  IN  UINT32             Data32
  )
{
  if (IsSiliconWorkaroundEnabled ("S1409517281")) {
    PcuWriteCrProxyReg (
      SocketId,
      BoxInst,
      0,
      (UINT8) MdfisRegAndCRProxyIndexMappingTable [CsrIndex][1],
      Data32
      );
  } else {
    UsraCsrWrite (SocketId, BoxInst, MdfisRegAndCRProxyIndexMappingTable [CsrIndex][0], Data32);
  }
}

/**
  MDFIS: program BIOS_Assisted valid standalone phases.

  @param SocId            - Socket ID
  @param CtrId            - MDFS Instance
  @param NumOfPhases      - Number of phases in the sequences
  @param StandalonePhases - Standalone Phases for training

  @retval EFI_STATUS      - EFI_SUCCESS: success
                          - EFI_UNSUPPORTED: unsupported register on this HW IP
                          - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
MdfsStandaloneSequences (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 NumOfPhases,
  IN UINT8 StandalonePhases []
  )
{
  MdfisCsrWrite (SocId, CtrId, MDFIS_STANDALONE_SEQUENCE_REG2_UPPER_CREG_INDEX, 0);

  return EFI_SUCCESS;
}

/**
  MDFIS: program BIOS_Assisted valid Die2Die phases.

  @param SocId            - Socket ID
  @param CtrId            - MDFS Instance
  @param NumOfPhases      - Number of phases in the sequences
  @param Die2DiePhases    - Die2Die Phases for training


  @retval EFI_STATUS      - EFI_SUCCESS: success
                          - EFI_UNSUPPORTED: unsupported register on this HW IP
                          - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
MdfsDie2DieSequences (
  IN UINT8 SocId,
  IN UINT8 CtrId,
  IN UINT8 NumOfPhases,
  IN UINT8 Die2DiePhases []
  )
{
  UINT32              MdfisDie2DieSequenceReg2Lower;
  UINT32              MdfisDie2DieSequenceReg2Upper;

  MdfisDie2DieSequenceReg2Upper = 0;

  if (IsSiliconWorkaroundEnabled ("S14011776702")) {
    //
    // Change default register setting as below for C0 and later stepping
    //
    MdfisDie2DieSequenceReg2Lower = 0xCCCCE8CB;
    MdfisDie2DieSequenceReg2Upper = 0;

    MdfisCsrWrite (SocId, CtrId, MDFIS_DIE2DIE_SEQUENCE_REG2_LOWER_CREG_INDEX, MdfisDie2DieSequenceReg2Lower);
  }

  MdfisCsrWrite (SocId, CtrId, MDFIS_DIE2DIE_SEQUENCE_REG2_UPPER_CREG_INDEX, MdfisDie2DieSequenceReg2Upper);

  if (UbiosGenerationEnabled ()) {
    //
    // Per emulation request, with one of the BIOS training phases programmed taking an exceptionally long time,
    // let ubios write a different value to a register than the real BIOS does so we can disable that one phase
    // MDFIS_DIE2DIE_SEQUENCE_REG2_LOWER_CREG_MDFIS_MAIN_REG default value is 0x00CCCE8C
    //
    MdfisCsrWrite (SocId, CtrId, MDFIS_DIE2DIE_SEQUENCE_REG2_LOWER_CREG_INDEX, 0x00CCCE0C);
  }
  return EFI_SUCCESS;
}

/**
  MDFS: Set Sequence Select in MDFIS TRAINING FSM CTRL0.

  @param SocId                 - Socket ID
  @param CtrId                 - MDFS Instance
  @param SeqRegSelect          - Sequence Register Select
  @parma OriginalValInFsmCtrl0 - Original value in FsmCtrl0 before configure SeqRegSelect

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
MdfsSetSeqRegSelectInFsmCtrl0 (
  IN UINT8                SocId,
  IN UINT8                CtrId,
  IN SEQUENCE_REG_SELECT  SeqRegSelect,
  OUT UINT32              *OriginalValInFsmCtrl0
  )
{
  MDFIS_TRAINING_FSM_CTRL0_CREG_MDFIS_MAIN_STRUCT  FsmCtrl0;
  BOOLEAN                                          IsWorkaroundEn;

  IsWorkaroundEn = IsSiliconWorkaroundEnabled("S14011408944");

  FsmCtrl0.Data = MdfisCsrRead (SocId, CtrId, MDFIS_TRAINING_FSM_CTRL0_CREG_INDEX);

  if (OriginalValInFsmCtrl0) {
    if (IsWorkaroundEn) {
      //
      // Assert ColdBoot_Done=1 after BIOS assisted training is initiated
      //
      FsmCtrl0.Bits.cold_boot_done = 1;
    }
    *OriginalValInFsmCtrl0 = FsmCtrl0.Data;
  }

  if (IsWorkaroundEn) {
    //
    // Deassert ColdBoot_Done=0 before BIOS assisted training is initiated
    //
    FsmCtrl0.Bits.cold_boot_done = 0;
  }
  FsmCtrl0.Bits.sequence_reg_sel = SeqRegSelect;
  MdfisCsrWrite (SocId, CtrId, MDFIS_TRAINING_FSM_CTRL0_CREG_INDEX, FsmCtrl0.Data);

  return EFI_SUCCESS;
}

/**
  MDFS: program the whole register of FSM CTRL0.

  @param SocId                 - Socket ID
  @param CtrId                 - MDFS Instance
  @parma ConfigForFsmCtrl0     - Configuration for FsmCtrl0

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
MdfsSetFsmCtrl0 (
  IN UINT8                SocId,
  IN UINT8                CtrId,
  IN UINT32               ConfigForFsmCtrl0
  )
{
  MdfisCsrWrite (SocId, CtrId, MDFIS_TRAINING_FSM_CTRL0_CREG_INDEX, ConfigForFsmCtrl0);

  return EFI_SUCCESS;
}

/**
  MDFS: program phase ctrl registers.

  @param SocId         - Socket ID
  @param CtrId         - MDFS Instance

  @retval EFI_STATUS   - EFI_SUCCESS: success
                       - EFI_UNSUPPORTED: unsupported register on this HW IP
                       - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
MdfsProgramPhasesCtrl (
  IN UINT8 SocId,
  IN UINT8 CtrId
  )
{
  UINT8  Index;
  UINT32 RegArray[] = {
    MDFIS_RXVREF_PHASE_CTRL_CREG_INDEX,                    0x00005458,
    MDFIS_SLOOP_PHASE_CTRL_CREG_INDEX,                     0x064FF814,
    MDFIS_LLOOP_PHASE_CTRL_CREG_INDEX,                     0x003FF800,
  };

  for (Index = 0; Index < sizeof (RegArray) / sizeof (RegArray[0]); Index += 2) {
    MdfisCsrWrite (SocId, CtrId, RegArray[Index], RegArray[Index + 1]);
  }

  return EFI_SUCCESS;
}