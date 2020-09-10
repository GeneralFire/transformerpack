/** @file
  SsaMemoryDdrtConfig.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2015 Intel Corporation. <BR>

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

#ifndef _SsaMemoryDdrtConfig_h_
#define _SsaMemoryDdrtConfig_h_

// BIOS-SSA DDR-T Memory Configuration API revision
#define SSA_REVISION_MEMORY_DDRT ((0x01 << 24) | (0x01 << 16) | (0x00 << 8) | 0x00)

#pragma pack (push, 1)

///
/// Falcon Valley training mode.
///
typedef enum {
  FnvTrainModeEnableTraining,
  FnvTrainModeDisableTraining,
  FnvTrainModeEarlyCmdCk,
  FnvTrainModeLateCmdCk,
  FnvTrainModeEarlyRidFine,
  FnvTrainModeEarlyRidCoarse,
  FnvTrainModeCmdVrefCentering,
  FnvTrainModeRxBacksidePhase,
  FnvTrainModeDisableTrainingStep,
  FnvTrainingModeMax,                     ///< FNV_TRAINING_MODE enumeration maximum value.
  FnvTrainingModeDelim = MAX_INT32        ///< This value ensures the enum size is consistent on both sides of the PPI.
} FNV_TRAINING_MODE;

///
/// ERID signal name.
///
typedef enum {
  EridSignal0,
  EridSignal1,
  EridSignalMax,                          ///< ERID_SIGNAL enumeration maximum value.
  EridSignalDelim = MAX_INT32             ///< This value ensures the enum size is consistent on both sides of the PPI.
} ERID_SIGNAL;

///
/// ERID pattern mode.
///
typedef enum {
  EridLfsrMode,
  EridBufferMode,
  EridPatternModeMax,                     ///< ERID_PATTERN_MODE enumeration maximum value.
  EridPatternDelim = MAX_INT32            ///< This value ensures the enum size is consistent on both sides of the PPI.
} ERID_PATTERN_MODE;

/**
  Function used to set the bit validation mask of Early Read ID.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based controller number.
  @param[in]      Channel    - Zero based channel number.
  @param[in]      EridMask   - Early Read ID bitmask.  Bit value 1 = perform validation; bit value 0 = do not perform validation.  Bit position 3: CKP Early Read ID (even).  Bit position 2: CKP Early Read ID (odd).  Bit position 1: CKN Early Read ID (even).  Bit position 0: CKN Early Read ID (odd).

  @retval None
**/
typedef
VOID
(EFIAPI * SET_ERID_VALIDATION_MASK) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  IN UINT8                     EridMask
  );

/**
  Function used to get the Early Read ID bit error status.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based controller number.
  @param[in]      Channel    - Zero based channel number.
  @param[out]     Status     - Pointer to where the Early Read ID bit error status will be stored.  Bit value 1 = at least one error occurred; bit value 0 = no errors occurred.  Bit position 3: CKP Early Read ID (even).  Bit position 2: CKP Early Read ID (odd).  Bit position 1: CKN Early Read ID (even).  Bit position 0: CKN Early Read ID (odd).
  @param[out]     Count      - Pointer to where the error counter value will be stored.

  @retval None
**/
typedef
VOID
(EFIAPI * GET_ERID_ERROR_STATUS) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  OUT UINT8                    *Status,
  OUT UINT16                   *Count
  );

/**
  Function used to clear the Early Read ID bit error status.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based controller number.
  @param[in]      Channel    - Zero based channel number.

  @retval None
**/
typedef
VOID
(EFIAPI * CLEAR_ERID_ERROR_STATUS) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel
  );

/**
  Function used to set up the Early Read ID pattern generator.

  @param[in, out] This          - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket        - Zero based CPU socket number.
  @param[in]      Controller    - Zero based controller number.
  @param[in]      Channel       - Zero based channel number.
  @param[in]      Signal        - ERID signal name.
  @param[in]      PatternMode   - ERID pattern mode.
  @param[in]      EnableReload  - Enable or disable seed reload for the Early Read ID.
  @param[in]      EnableReset   - Enable or disable seed reset for the Early Read ID.
  @param[in]      Seed          - Seed value for Early Read ID.

  @retval None
**/
typedef
VOID
(EFIAPI * SET_ERID_PATTERN) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  IN ERID_SIGNAL               Signal,
  IN ERID_PATTERN_MODE         PatternMode,
  IN BOOLEAN                   EnableReload,
  IN BOOLEAN                   EnableReset,
  IN UINT32                    Seed
  );

/**
  Function used to set the Early Read ID coarse training configuration.

  @param[in, out] This            - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket          - Zero based CPU socket number.
  @param[in]      Controller      - Zero based controller number.
  @param[in]      Channel         - Zero based channel number.
  @param[in]      EnableTraining  - Specifies whether Early Read ID coarse training mode is enabled.

  @retval None
**/
typedef
VOID
(EFIAPI * SET_ERID_TRAINING_CONFIG) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  IN BOOLEAN                   EnableTraining
  );

/**
  Function used to set the training mode of the AEP DIMMs for a given socket.

  @param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket           - Zero based CPU socket number.
  @param[in]      Mode             - FalconValley (controller on AEP DIMM) training mode.

  @retval None
**/
typedef
VOID
(EFIAPI * SET_AEP_DIMM_TRAINING_MODE) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN FNV_TRAINING_MODE         Mode
  );

/**
  Function used to reset the I/O for a given AEP memory DIMM.

  @param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket           - Zero based CPU socket number.
  @param[in]      Controller       - Zero based controller number.
  @param[in]      Channel          - Zero based channel number.
  @param[in]      Dimm             - Zero based DIMM number.

  @retval None
**/
typedef
VOID
(EFIAPI * AEP_IO_RESET) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  IN UINT8                     Dimm
  );

/**
  Function used to drain the RPQ for a given AEP memory DIMM.

  @param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket           - Zero based CPU socket number.
  @param[in]      Controller       - Zero based controller number.
  @param[in]      Channel          - Zero based channel number.
  @param[in]      Dimm             - Zero based DIMM number.

  @retval None
**/
typedef
VOID
(EFIAPI * DRAIN_AEP_RPQ) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     Channel,
  IN UINT8                     Dimm
  );

/**
  Function used to clear prefetch cache miss of AEP DIMMs for a given socket.

  @param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket           - Zero based CPU socket number.

  @retval None
**/
typedef
VOID
(EFIAPI * CLEAR_AEP_PREFETCH_CACHE) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket
  );

/**
  Function used to perform FIFO reset for all the AEP DIMMs for a given socket.

  @param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket           - Zero based CPU socket number.

  @retval None
**/
typedef
VOID
(EFIAPI * RESET_AEP_FIFO_TRAIN) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket
  );

/**
Function used to enable/disable timer for active channels with the credit limit set.

@param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
@param[in]      Socket           - Zero based CPU socket number.
@param[in]      Controller       - Zero based controller number.
@param[in]      ChannelMask      - Channel mask.  Each bit represents a channel to be checked.  Bit value 1 = perform check; bit value 0 = do not perform check.  Bit position 0 = channel 0; bit position 1 = channel 1, etc.
@param[in]      Enable           - TRUE: enable the timer for active channels with credit limit set;
                                   FALSE: disable the timer and clear DDRT scheduler error bits

@retval None.
**/
typedef
VOID
(EFIAPI * SET_AEP_TIMER_CONFIG) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     ChannelMask,
  IN BOOLEAN                   Enable
);

/**
Function used to get the DDRT scheduler time out error status for active channels with the credit limit set.

@param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
@param[in]      Socket           - Zero based CPU socket number.
@param[in]      Controller       - Zero based controller number.
@param[in]      ChannelMask      - Channel mask.  Each bit represents a channel to be checked.  Bit value 1 = perform check; bit value 0 = do not perform check.  Bit position 0 = channel 0; bit position 1 = channel 1, etc.
@param[out]     Status           - Pointer to where the scheduler timer error will be stored. Bit value 1 indicate there is an error
                                   The status is in channel bit mask format. Bit nth corresponds to channel nth inside the Controller

@retval None.
**/
typedef
VOID
(EFIAPI * GET_AEP_TIMER_ERROR_STATUS) (
  IN OUT SSA_BIOS_SERVICES_PPI *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     ChannelMask,
  OUT UINT8 *                   Status
);

/**
  BIOS-SSA DDR-T Memory Configuration
**/
typedef struct _SSA_MEMORY_DDRT_CONFIG {
  UINT32                     Revision;               ///< Incremented when a backwards compatible binary change is made to the PPI.
  SET_ERID_VALIDATION_MASK   SetEridValidationMask;  ///< Function used to set the bit validation mask of Early Read ID.
  GET_ERID_ERROR_STATUS      GetEridErrorStatus;     ///< Function used to get the Early Read ID bit error status.
  CLEAR_ERID_ERROR_STATUS    ClearEridErrorStatus;   ///< Function used to clear the Early Read ID bit error status.
  SET_ERID_PATTERN           SetEridPattern;         ///< Function used to set up the Early Read ID pattern generator.
  SET_ERID_TRAINING_CONFIG   SetEridTrainingConfig;  ///< Function used to set the Early Read ID coarse training configuration.
  SET_AEP_DIMM_TRAINING_MODE SetAepDimmTrainingMode; ///< Function used to set the training mode of the controller chip of the AEP DIMM (FalconValley).
  AEP_IO_RESET               AepIoReset;             ///< Function used to reset the I/O for a given AEP memory DIMM.
  DRAIN_AEP_RPQ              DrainAepRpq;            ///< Function used to drain the RPQ for a given AEP memory DIMM.
  CLEAR_AEP_PREFETCH_CACHE   ClearAepPrefetchCache;  ///< Function used to clear prefetch cache miss of AEP DIMMs for a given socket.
  RESET_AEP_FIFO_TRAIN       ResetAepFifoTrain;      ///< Function used to perform FIFO reset for all the AEP DIMMs for a given socket.
  SET_AEP_TIMER_CONFIG       SetAepTimerConfig;      ///< Function used to enable/disable timer for active channels with the credit limit set.
  GET_AEP_TIMER_ERROR_STATUS GetAepTimerErrorStatus; ///< Function used to get the DDRT scheduler time out error status for active channels with the credit limit set.
} SSA_MEMORY_DDRT_CONFIG, *PSSA_MEMORY_DDRT_CONFIG;

#pragma pack (pop)
#endif // _SsaMemoryDdrtConfig_h_
