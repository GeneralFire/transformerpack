/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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
#ifndef __MDFIS_LIB_H__
#define __MDFIS_LIB_H__

#include <Uefi.h>

typedef enum {
  MDFIS_TRAIN_PHASE_NULL = 0,
  STDALONE_RX_OFFSET_EVEN,
  STDALONE_RX_OFFSET_ODD,
  STDALONE_TX_RCOMP,
  STDALONE_TX_PA,
  STDALONE_TX_DCC,
  STDALONE_RX_OFFSET_DQS,
  STDALONE_TX_NELB,
  STDALONE_RX_NELB,
  DIE2DIE_RX_VREF = 8,
  DIE2DIE_IN_LOCK_WINDOW = 0xB,
  DIE2DIE_QDS_SLOOP = 0xC,
  DIE2DIE_D2C_LLOOP = 0xE
} BIOS_ASSISTED_TRAIN_PHASE;

typedef enum {
  SEQ_REG0_FULL_TRAINING = 0,
  SEQ_REG1_PARTIAL_TRAINING,
  SEQ_REG2_BIOS_ASSISTED,
  SEQ_REG3_SPARE
} SEQUENCE_REG_SELECT;

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
  );

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
  );

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
  );

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
  );

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
  );
#endif // __MDFIS_LIB_H__
