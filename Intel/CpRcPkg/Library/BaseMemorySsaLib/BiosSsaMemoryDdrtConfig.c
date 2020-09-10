/** @file
  This file contains the BIOS implementation of the BIOS-SSA DDR-T Memory Configuration API.

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

#include <Library/MemoryCoreLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemorySsaLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SysHostPointerLib.h>

#include <Memory/MemoryCheckpointCodes.h>

/**
  Function used to set the bit validation mask of Early Read ID.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based controller number.
  @param[in]      Channel    - Zero based channel number.
  @param[in]      EridMask   - Early Read ID bitmask.  Bit value 1 = perform validation; bit value 0 = do not perform validation.

  @retval Nothing.
**/
VOID
EFIAPI 
BiosSetEridValidationMask (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT8                 Socket,
  IN      UINT8                 Controller,
  IN      UINT8                 Channel,
  IN      UINT8                 EridMask
  )
{
  UINT8     channelInSocket;
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosSetEridValidationMask (Socket:%d, Controller:%d, Channel:%d, Dimm:%d, EridMask:0x%x)\n", Socket, Controller, Channel, EridMask);
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);

  BiosSetEridValidationMaskChip (Host, Socket, channelInSocket, ~EridMask); // 0 means corresponding bit is enabled for checking errors

  RcDebugPrint (SDBG_BSSA, "End: BiosSetEridValidationMask \n");
}

/**
  Function used to get the Early Read ID bit error status.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based controller number.
  @param[in]      Channel    - Zero based channel number.
  @param[out]     Status     - Pointer to where the Early Read ID bit error status will be stored.  Bit value 1 = at least one error occurred; bit value 0 = no errors occurred.
  @param[out]     Count      - Pointer to where the error counter value will be stored.

  @retval Nothing.
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
  )
{
  UINT8     channelInSocket;
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosGetEridErrorStatus (Socket:%d, Controller:%d, Channel:%d, Dimm:%d)\n", Socket, Controller, Channel);
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);

  BiosGetEridErrorStatusChip (Host, Socket, channelInSocket, Status, Count);

  RcDebugPrint (SDBG_BSSA, "End: BiosGetEridErrorStatus Status:0x%x Count:%d\n", *Status, *Count);
}

/**
  Function used to clear the Early Read ID bit error status.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based controller number.
  @param[in]      Channel    - Zero based channel number.

  @retval Nothing.
**/
VOID
EFIAPI 
BiosClearEridErrorStatus (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT8                 Socket,
  IN      UINT8                 Controller,
  IN      UINT8                 Channel
  )
{
  UINT8     channelInSocket;
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosClearEridErrorStatus (Socket:%d, Controller:%d, Channel:%d)\n", Socket, Controller, Channel);
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);

  ClearEridLfsrErr (Host, Socket, channelInSocket);

  RcDebugPrint (SDBG_BSSA, "End: BiosClearEridErrorStatus \n");
}

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

  @retval Nothing.
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
  )
{
  UINT8     channelInSocket;
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosSetEridPattern (Socket:%d, Controller:%d, Channel:%d, Signal:%d PatternMode:%d EnableReload:%d EnableReset:%d Seed:0x%x)\n",
             Socket, Controller, Channel, Signal, PatternMode, EnableReload, EnableReset, Seed);
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);

  BiosSetEridPatternChip (
    Host,
    Socket,
    channelInSocket,
    Signal,
    PatternMode,
    EnableReload,
    EnableReset,
    Seed
    );

  RcDebugPrint (SDBG_BSSA, "End: BiosSetEridPattern \n");
}

/**
  Function used to set the Early Read ID coarse training configuration.

  @param[in, out] This           - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket         - Zero based CPU socket number.
  @param[in]      Controller     - Zero based controller number.
  @param[in]      Channel        - Zero based channel number.
  @param[in]      EnableTraining - Specifies whether Early Read ID coarse training mode is enabled.

  @retval Nothing.
**/
VOID
EFIAPI 
BiosSetEridTrainingConfig (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT8                 Socket,
  IN      UINT8                 Controller,
  IN      UINT8                 Channel,
  IN      BOOLEAN               EnableTraining
  )
{
  UINT8     channelInSocket;
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosSetEridTrainingConfig (Socket:%d, Controller:%d, Channel:%d, EnableTraining:%d)\n",
             Socket, Controller, Channel, EnableTraining);
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);

  BiosSetEridTrainingConfigChip (Host, Socket, channelInSocket, EnableTraining);

  RcDebugPrint (SDBG_BSSA, "End: BiosSetEridTrainingConfig \n");
}

/**
  Function used to set the training mode of the MC(FalconValley) of the AEP DIMM for a given socket.

  @param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket           - Zero based CPU socket number.
  @param[in]      Mode             - Training mode to set

  @retval Nothing.
**/
VOID
EFIAPI 
BiosSetAepDimmTrainingMode (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT8                 Socket,
  IN      FNV_TRAINING_MODE     Mode
  )
{
  PSYSHOST  Host;
  UINT8     modeVal = FnvTrainModeDisableTraining;
  UINT32    PrevDebugLevel;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosSet PMem Dimm TrainingMode (Socket:%d, Mode:0x%x)\n", Socket, Mode);

  // Convert FNV_TRAINING_MODE enum to mode value that used by the MRC code.
  switch (Mode){
  case FnvTrainModeDisableTraining:
    modeVal = DISABLE_TRAINING_MODE;
    break;
  case FnvTrainModeEarlyCmdCk:
    modeVal = CHECKPOINT_MINOR_EARLY_CMD_CLK;
    break;
  case FnvTrainModeEarlyRidFine:
    modeVal = CHECKPOINT_MINOR_EARLY_RID_FINE;
    break;
  case FnvTrainModeEarlyRidCoarse:
    modeVal = CHECKPOINT_MINOR_EARLY_RID_COARSE;
    break;
  case FnvTrainModeCmdVrefCentering:
    modeVal = CHECKPOINT_MINOR_CMD_VREF_CENTERING;
    break;
  case FnvTrainModeLateCmdCk:
    modeVal = CHECKPOINT_MINOR_LATE_CMD_CLK;
    break;
  case FnvTrainModeRxBacksidePhase:
    modeVal = CHECKPOINT_MINOR_RX_BACKSIDE_PHASE_TRAINING;
    break;
  case FnvTrainModeDisableTrainingStep:
    modeVal = DISABLE_TRAINING_STEP;
    break;
  case FnvTrainModeEnableTraining:
    modeVal = ENABLE_TRAINING_MODE;
    break;
  default:
    break;
  }

  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }
  SetAepTrainingMode (Host, Socket, modeVal);
  SetDebugLevel (PrevDebugLevel);

  RcDebugPrint (SDBG_BSSA, "End: BiosSet PMem Dimm TrainingMode \n");
}

/**
  Function used to reset the I/O for a given AEP memory DIMM.

  @param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket           - Zero based CPU socket number.
  @param[in]      Controller       - Zero based controller number.
  @param[in]      Channel          - Zero based channel number.
  @param[in]      Dimm             - Zero based DIMM number.

  @retval Nothing.
**/
VOID
EFIAPI 
BiosAepIoReset (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT8                 Socket,
  IN      UINT8                 Controller,
  IN      UINT8                 Channel,
  IN      UINT8                 Dimm
  )
{
  UINT8     ChannelInSocket;
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosPMemIoReset (Socket:%d, Controller:%d Channel:%d Dimm:%d)\n", Socket, Controller, Channel, Dimm);
  ChannelInSocket = SSAGetChannelInSocket(Controller, Channel);

  BiosAepIoResetChip (Host, Socket, ChannelInSocket, Dimm);

  RcDebugPrint (SDBG_BSSA, "End: BiosPmemIoReset \n");
}

/**
  Function used to drain the RPQ for a given PMEM memory DIMM.

  @param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket           - Zero based CPU socket number.
  @param[in]      Controller       - Zero based controller number.
  @param[in]      Channel          - Zero based channel number.
  @param[in]      Dimm             - Zero based DIMM number.

  @retval Nothing.
**/
VOID
EFIAPI 
BiosAepRpqDrain (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT8                 Socket,
  IN      UINT8                 Controller,
  IN      UINT8                 Channel,
  IN      UINT8                 Dimm
  )
{
  UINT8     channelInSocket;
  PSYSHOST  Host;
  UINT32    PrevDebugLevel;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosPMemRpqDrain (Socket:%d, Controller:%d Channel:%d Dimm:%d)\n", Socket, Controller, Channel, Dimm);
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);

  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }
  RPQDrain(Host, Socket, channelInSocket, Dimm, 0); // PMEM dimm has only 1 rank.
  SetDebugLevel (PrevDebugLevel);

  RcDebugPrint (SDBG_BSSA, "End: BiosPMemRpqDrain \n");
}

/**
  Function used to clear prefetch cache miss of PMEM DIMMs for a given socket.

  @param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket           - Zero based CPU socket number.

  @retval Nothing.
**/
VOID
EFIAPI 
BiosAepClrPrefetchCache (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT8                 Socket
  )
{
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosPMemClrPrefetchCache (Socket:%d)\n", Socket);
  ClrPcheMiss(Host, Socket);

  RcDebugPrint (SDBG_BSSA, "End: BiosPMemClrPrefetchCache \n");
}

/**
  Function used to perform FIFO reset for all the AEP DIMMs for a given socket.

  @param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket           - Zero based CPU socket number.

  @retval Nothing.
**/
VOID
EFIAPI 
BiosAepFifoTrainReset (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT8                 Socket
  )
{
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosPMemFifoTrainReset (Socket:%d)\n", Socket);
  FifoTrainReset(Host, Socket);

  RcDebugPrint (SDBG_BSSA, "End: BiosPMemFifoTrainReset \n");
}
/**
Function used to enable/disable timer for active channels with the credit limit set.
NOTE: This function was depreciated. It was kept to avoid existing BSSA test content code changes.
NOTE: RPQ timer is configured by the MRC training routines.
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
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     ChannelMask,
  IN BOOLEAN                   Enable
  )
{
}

/**
Function used to get the DDRT scheduler time out error status for active channels with the credit limit set. 
NOTE: This function was depreciated. It was kept to avoid existing BSSA test content code changes. 
NOTE: RPQ timer timeout status get applied into CPGC error status. BSSA test content no long need to directly read 
the timer error status. 

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
  IN OUT SSA_BIOS_SERVICES_PPI    *This,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     ChannelMask,
  IN UINT8 *                   Status
  )
{
  *Status = 0;
}

// end file BiosSsaMemoryDdrtConfig.c
