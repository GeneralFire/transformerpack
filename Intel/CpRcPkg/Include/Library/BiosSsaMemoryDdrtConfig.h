/** @file
  BiosSsaMemoryDdrtConfig.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2018 Intel Corporation. <BR>

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

#ifndef __BIOS_SSA_MEMORY_DDRT_CONFIG__
#define __BIOS_SSA_MEMORY_DDRT_CONFIG__

#pragma pack (push, 1)

/**
  Function used to set the bit validation mask of Early Read ID.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based controller number.
  @param[in]      Channel    - Zero based channel number.
  @param[in]      EridMask   - Early Read ID bitmask.  Bit value 1 = perform validation; bit value 0 = do not perform validation.  Bit position 3: CKP Early Read ID (even).  Bit position 2: CKP Early Read ID (odd).  Bit position 1: CKN Early Read ID (even).  Bit position 0: CKN Early Read ID (odd).

  @retval None.
**/
VOID
EFIAPI 
BiosSetEridValidationMask (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT8                 Socket,
  IN      UINT8                 Controller,
  IN      UINT8                 Channel,
  IN      UINT8                 EridMask
  );

/**
  Function used to get the Early Read ID bit error status.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based controller number.
  @param[in]      Channel    - Zero based channel number.
  @param[out]     Status     - Pointer to where the Early Read ID bit error status will be stored.  Bit value 1 = at least one error occurred; bit value 0 = no errors occurred.  Bit position 3: CKP Early Read ID (even).  Bit position 2: CKP Early Read ID (odd).  Bit position 1: CKN Early Read ID (even).  Bit position 0: CKN Early Read ID (odd).
  @param[out]     Count      - Pointer to where the error counter value will be stored.

  @retval None.
**/
VOID
EFIAPI 
BiosGetEridErrorStatus (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT8                 Socket,
  IN      UINT8                 Controller,
  IN      UINT8                 Channel,
  OUT     UINT8                 *Status,
  OUT     UINT16                *Count
  );

/**
  Function used to clear the Early Read ID bit error status.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based controller number.
  @param[in]      Channel    - Zero based channel number.

  @retval None.
**/
VOID
EFIAPI 
BiosClearEridErrorStatus (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT8                 Socket,
  IN      UINT8                 Controller,
  IN      UINT8                 Channel
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

  @retval None.
**/
VOID
EFIAPI 
BiosSetEridPattern (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT8                 Socket,
  IN      UINT8                 Controller,
  IN      UINT8                 Channel,
  IN      ERID_SIGNAL           Signal,
  IN      ERID_PATTERN_MODE     PatternMode,
  IN      BOOLEAN               EnableReload,
  IN      BOOLEAN               EnableReset,
  IN      UINT32                Seed
  );

/**
  Function used to set the Early Read ID coarse training configuration.

  @param[in, out] This           - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket         - Zero based CPU socket number.
  @param[in]      Controller     - Zero based controller number.
  @param[in]      Channel        - Zero based channel number.
  @param[in]      EnableTraining - Specifies whether Early Read ID coarse training mode is enabled.

  @retval None.
**/
VOID
EFIAPI 
BiosSetEridTrainingConfig (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT8                 Socket,
  IN      UINT8                 Controller,
  IN      UINT8                 Channel,
  IN      BOOLEAN               EnableTraining
  );

/**
  Function used to set the training mode of the controller chip of the AEP DIMM (FalconValley).

  @param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket           - Zero based CPU socket number.
  @param[in]      Mode             - Training mode to set

  @retval None.
**/
VOID
EFIAPI 
BiosSetAepDimmTrainingMode (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT8                 Socket,
  IN      FNV_TRAINING_MODE     Mode
  );

/**
  Function used to reset the I/O for a given AEP memory DIMM.

  @param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket           - Zero based CPU socket number.
  @param[in]      Controller       - Zero based controller number.
  @param[in]      Channel          - Zero based channel number.
  @param[in]      Dimm             - Zero based DIMM number.

  @retval None.
**/
VOID
EFIAPI 
BiosAepIoReset (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT8                 Socket,
  IN      UINT8                 Controller,
  IN      UINT8                 Channel,
  IN      UINT8                 Dimm
  );

/**
  Function used to drain the RPQ for a given AEP memory DIMM.

  @param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket           - Zero based CPU socket number.
  @param[in]      Controller       - Zero based controller number.
  @param[in]      Channel          - Zero based channel number.
  @param[in]      Dimm             - Zero based DIMM number.

  @retval None.
**/
VOID
EFIAPI 
BiosAepRpqDrain (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT8                 Socket,
  IN      UINT8                 Controller,
  IN      UINT8                 Channel,
  IN      UINT8                 Dimm
  );

/**
  Function used to clear prefetch cache miss of AEP DIMMs for a given socket.

  @param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket           - Zero based CPU socket number.

  @retval None.
**/
VOID
EFIAPI 
BiosAepClrPrefetchCache (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT8                 Socket
  );

/**
  Function used to perform FIFO reset for all the AEP DIMMs for a given socket.

  @param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket           - Zero based CPU socket number.

  @retval None.
**/
VOID
EFIAPI 
BiosAepFifoTrainReset (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT8                 Socket
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
VOID
EFIAPI
BiosSetAepTimerConfig (
  IN OUT SSA_BIOS_SERVICES_PPI     *This,
  IN UINT8                         Socket,
  IN UINT8                         Controller,
  IN UINT8                         ChannelMask,
  IN BOOLEAN                       Enable
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
VOID
EFIAPI
BiosGetAepTimerErrorStatus (
  IN OUT SSA_BIOS_SERVICES_PPI     *This,
  IN UINT8                         Socket,
  IN UINT8                         Controller,
  IN UINT8                         ChannelMask,
  OUT UINT8 *                      Status
);

#pragma pack (pop)
#endif // __BIOS_SSA_MEMORY_DDRT_CONFIG__

// end file BiosSsaMemoryDdrtConfig.h
