/** @file
  This file contains the implementation of the BIOS version of the SSA services PPI.
  This version contains code for both client and server implementations.
  For Client:
  Delete include of BiosSsaMemoryServerConfig.h and BiosSsaMemoryDdrtConfig.h.
  Delete definitions of SSA_REVISION_MEMORY_SERVER and SSA_REVISION_MEMORY_DDRT.
  Delete declarations of SsaMemoryServerConfig and SsaMemoryDdrtConfig.
  Replace SsaMemoryServerConfig and SsaMemoryDdrtConfig field values in SsaBiosServicesPpi with NULL.
  For Server:
  Delete include of BiosSsaMemoryClientConfig.h.
  Delete definition of SSA_REVISION_MEMORY_CLIENT.
  Delete declaration of SsaMemoryClientConfig.
  Replace SsaMemoryClientConfig field value in SsaBiosServicesPpi with NULL.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2019 Intel Corporation. <BR>

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
#include "MemCpgcRegs.h"
#include <Library/MemMcIpLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemorySsaLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/MemRcLib.h>

//
//This BSSA file contains subsets of all BSSA function calls made from the Core files; all CSR accesses are made here. This is DDRT Code and hence not available on Broadwell Server(BDX)
//

/**

  This routine sets the CSRs required by BiosSetEridValidationMask

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param EridMask                          - Early Read ID bitmask.  Bit value 1 = perform validation; bit value 0 = do not perform validation.  Bit position 3: CKP Early Read ID (even).  Bit position 2: CKP Early Read ID (odd).  Bit position 1: CKN Early Read ID (even).  Bit position 0: CKN Early Read ID (odd).

  @retval N/A

**/
VOID
BiosSetEridValidationMaskChip (
  PSYSHOST                              Host,
  UINT8                                 Socket,
  UINT8                                 ChannelInSocket,
  UINT8                                 EridMask
  )
{
  CPGC_ERID_LFSR_ERR_MC_2LM_STRUCT        cpgcEridLfsrErr;

  cpgcEridLfsrErr.Data = MemReadPciCfgEp (Socket, ChannelInSocket, CPGC_ERID_LFSR_ERR_MC_2LM_REG);
  cpgcEridLfsrErr.Bits.mask = EridMask; //0x80;
  MemWritePciCfgEp (Socket, ChannelInSocket, CPGC_ERID_LFSR_ERR_MC_2LM_REG, cpgcEridLfsrErr.Data);
}

/**

  This routine gets the CSRs required by BiosGetEridErrorStatus

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param Status                            - Pointer to where the Early Read ID bit error status will be stored.  Bit value 1 = at least one error occurred; bit value 0 = no errors occurred.  Bit position 3: CKP Early Read ID (even).  Bit position 2: CKP Early Read ID (odd).  Bit position 1: CKN Early Read ID (even).  Bit position 0: CKN Early Read ID (odd).
  @param Count                             - Pointer to where the error counter value will be stored.

  @retval N/A

**/
VOID
BiosGetEridErrorStatusChip (
  PSYSHOST                              Host,
  UINT8                                 Socket,
  UINT8                                 ChannelInSocket,
  UINT8                                 *Status,
  UINT16                                *Count
  )
{
  CPGC_ERID_LFSR_ERR_MC_2LM_STRUCT  cpgcEridLfsrErr;

  cpgcEridLfsrErr.Data = MemReadPciCfgEp (Socket, ChannelInSocket, CPGC_ERID_LFSR_ERR_MC_2LM_REG);
  *Status = (UINT8)cpgcEridLfsrErr.Bits.status;
  *Count = (UINT16)cpgcEridLfsrErr.Bits.count;
}

/**

  This routine sets the CSRs required by BiosSetEridPattern

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param Signal                            - Zero based ERID signal number.
  @param PatternMode                       - LFSR or pattern buffer mode.
  @param EnableReload                      - Enable or disable seed reload for the Early Read ID.
  @param EnableReset                       - Enable or disable seed reset for the Early Read ID.
  @param Seed                              - Seed value for Early Read ID.

  @retval N/A

**/
VOID
BiosSetEridPatternChip (
  PSYSHOST                              Host,
  UINT8                                 Socket,
  UINT8                                 ChannelInSocket,
  ERID_SIGNAL                           Signal,
  ERID_PATTERN_MODE                     PatternMode,
  BOOLEAN                               EnableReload,
  BOOLEAN                               EnableReset,
  UINT32                                Seed
  )
{
  CPGC_ERID_LFSR_INIT_REQ0_MC_2LM_STRUCT    cpgcEridLfsrInitReq0;
  CPGC_ERID_LFSR_INIT_REQ1_MC_2LM_STRUCT    cpgcEridLfsrInitReq1;
  CPGC_ERID_LFSR_INIT_RESP0_MC_2LM_STRUCT   cpgcEridLfsrInitResp0;
  CPGC_ERID_LFSR_INIT_RESP1_MC_2LM_STRUCT   cpgcEridLfsrInitResp1;
  CPGC_ERID_LFSR_CTL_MC_2LM_STRUCT          cpgcEridLfsrCtl;

  cpgcEridLfsrCtl.Data =  MemReadPciCfgEp (Socket, ChannelInSocket, CPGC_ERID_LFSR_CTL_MC_2LM_REG);

  if (Signal == EridSignal0) {

#ifndef DDR5_SUPPORT
    cpgcEridLfsrCtl.Bits.mode0 = (PatternMode == EridLfsrMode)? 0 : 1; // 0 selects LFSR
#endif // DDR5_SUPPORT

    // program the pattern buffers with the Seed value
    cpgcEridLfsrInitReq0.Data = MemReadPciCfgEp (Socket, ChannelInSocket, CPGC_ERID_LFSR_INIT_REQ0_MC_2LM_REG);
    cpgcEridLfsrInitReq0.Bits.patbuffer = Seed;
    MemWritePciCfgEp (Socket, ChannelInSocket, CPGC_ERID_LFSR_INIT_REQ0_MC_2LM_REG, cpgcEridLfsrInitReq0.Data);
    cpgcEridLfsrInitResp0.Data = MemReadPciCfgEp (Socket, ChannelInSocket, CPGC_ERID_LFSR_INIT_RESP0_MC_2LM_REG);
    cpgcEridLfsrInitResp0.Bits.patbuffer = Seed;
    MemWritePciCfgEp (Socket, ChannelInSocket, CPGC_ERID_LFSR_INIT_RESP0_MC_2LM_REG, cpgcEridLfsrInitResp0.Data);

  } else {

#ifndef DDR5_SUPPORT
    cpgcEridLfsrCtl.Bits.mode1 = (PatternMode == EridLfsrMode)? 0 : 1;  // 0 selects LFSR
#endif

    // program the pattern buffers with the Seed value
    cpgcEridLfsrInitReq1.Data = MemReadPciCfgEp (Socket, ChannelInSocket, CPGC_ERID_LFSR_INIT_REQ1_MC_2LM_REG);
    cpgcEridLfsrInitReq1.Bits.patbuffer = Seed;
    MemWritePciCfgEp (Socket, ChannelInSocket, CPGC_ERID_LFSR_INIT_REQ1_MC_2LM_REG, cpgcEridLfsrInitReq1.Data);
    cpgcEridLfsrInitResp1.Data = MemReadPciCfgEp (Socket, ChannelInSocket, CPGC_ERID_LFSR_INIT_RESP1_MC_2LM_REG);
    cpgcEridLfsrInitResp1.Bits.patbuffer = Seed;
    MemWritePciCfgEp (Socket, ChannelInSocket, CPGC_ERID_LFSR_INIT_RESP1_MC_2LM_REG, cpgcEridLfsrInitResp1.Data);

  }

  MemWritePciCfgEp (Socket, ChannelInSocket, CPGC_ERID_LFSR_CTL_MC_2LM_REG, cpgcEridLfsrCtl.Data);
}

/**

  This routine sets the CSRs required by BiosSetEridTrainingConfig

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param EnableTraining                    - Specifies whether Early Read ID coarse training mode is enabled.

  @retval N/A

**/
VOID
BiosSetEridTrainingConfigChip (
  PSYSHOST                              Host,
  UINT8                                 Socket,
  UINT8                                 ChannelInSocket,
  BOOLEAN                               EnableTraining
  )
{
  CPGC_ERID_LFSR_CTL_MC_2LM_STRUCT  cpgcEridLfsrCtl;
  CPGC_DDRT_MISC_CTL_MC_2LM_STRUCT  cpgcDdrtMiscCtl;

  cpgcEridLfsrCtl.Data = MemReadPciCfgEp (Socket, ChannelInSocket, CPGC_ERID_LFSR_CTL_MC_2LM_REG);
  cpgcEridLfsrCtl.Bits.enable_erid_lfsr_training = (EnableTraining == TRUE)? 1:0;
  cpgcDdrtMiscCtl.Data = MemReadPciCfgEp (Socket, ChannelInSocket, CPGC_DDRT_MISC_CTL_MC_2LM_REG);
  cpgcDdrtMiscCtl.Bits.enable_erid_return  = (EnableTraining == TRUE)? 0:1;
  MemWritePciCfgEp (Socket, ChannelInSocket, CPGC_ERID_LFSR_CTL_MC_2LM_REG, cpgcEridLfsrCtl.Data);
  MemWritePciCfgEp (Socket, ChannelInSocket, CPGC_DDRT_MISC_CTL_MC_2LM_REG, cpgcDdrtMiscCtl.Data);
}

/**
Function used to enable/disable timer for active channels with the credit limit set required by BiosSetAepTimerConfig.

@param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
@param[in]      Socket           - Zero based CPU socket number.
@param[in]      Controller       - Zero based controller number.
@param[in]      ChannelMask      - Channel mask.  Each bit represents a channel to be checked.  Bit value 1 = perform check; bit value 0 = do not perform check.  Bit position 0 = channel 0; bit position 1 = channel 1, etc.
@param[in]      Enable           - TRUE: enable the timer for active channels with credit limit set;
                                   FALSE: disable the timer and clear DDRT scheduler error bits

@retval None.
**/
VOID
BiosSetAepTimerConfigChip (
  IN PSYSHOST                  Host,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     ChannelMask,
  IN BOOLEAN                   Enable
  )
{
  UINT8 ch;
  CPGC_DDRT_ERR_CTL_STS_MC_2LM_STRUCT  CpgcDdrtErrCtlSts;
  UINT8 channelInSocket;
  UINT8 NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();

  for (ch = 0; ch < NumChPerMc; ch++) {
    if (!(ChannelMask & (1 << ch))) continue;
      channelInSocket = SSAGetChannelInSocket (Controller, ch);
      if (!(Host->var.mem.DdrtReadPendingQueueCreditLimitSet[Socket][channelInSocket])) continue;
      if (!IsDdrtDimm (Host, Socket, channelInSocket)) continue;

      if (Enable) { //  Enable timers for active channels with the credit limit set if the rank under test has DDRT
        CpgcDdrtErrCtlSts.Data = MemReadPciCfgEp (Socket, channelInSocket, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG);
        CpgcDdrtErrCtlSts.Bits.timeout_base_clk = FixedPcdGet32 (PcdReadPendingQueueTimeoutBaseClock);
        CpgcDdrtErrCtlSts.Bits.timeout_threshold =  FixedPcdGet32 (PcdReadPendingQueueTimeoutThreshold);
        MemWritePciCfgEp (Socket, channelInSocket, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG, CpgcDdrtErrCtlSts.Data);

        // Toggle error bits to clear errors and restart the timer. Step 1 of 2: Setting bits.
        CpgcDdrtErrCtlSts.Data = MemReadPciCfgEp (Socket, channelInSocket, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG);
        CpgcDdrtErrCtlSts.Bits.crdt_limit_hit = 1;
        CpgcDdrtErrCtlSts.Bits.timeout_error = 1;
        MemWritePciCfgEp (Socket, channelInSocket, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG, CpgcDdrtErrCtlSts.Data);

        // Toggle error bits to clear errors and restart the timer. Step 2 of 2: Clearing bits.
        CpgcDdrtErrCtlSts.Data = MemReadPciCfgEp (Socket, channelInSocket, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG);
        CpgcDdrtErrCtlSts.Bits.crdt_limit_hit = 0;
        CpgcDdrtErrCtlSts.Bits.timeout_error = 0;
        MemWritePciCfgEp (Socket, channelInSocket, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG, CpgcDdrtErrCtlSts.Data);

      } else {
        //  clear the DDRT scheduler's error bits and disable the associated timer.
        CpgcDdrtErrCtlSts.Data = MemReadPciCfgEp (Socket, channelInSocket, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG);
        CpgcDdrtErrCtlSts.Bits.crdt_limit_hit = 0;
        CpgcDdrtErrCtlSts.Bits.timeout_error = 0;
        CpgcDdrtErrCtlSts.Bits.timeout_base_clk = 0;
        CpgcDdrtErrCtlSts.Bits.timeout_threshold = 0;
        MemWritePciCfgEp (Socket, channelInSocket, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG, CpgcDdrtErrCtlSts.Data);
      }
  } // ch
}

/**
Function used to get the DDRT scheduler time out error status for active channels with the credit limit set required by BiosGetAepTimerErrorStatus.

@param[in, out] This             - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
@param[in]      Socket           - Zero based CPU socket number.
@param[in]      Controller       - Zero based controller number.
@param[in]      ChannelMask      - Channel mask.  Each bit represents a channel to be checked.  Bit value 1 = perform check; bit value 0 = do not perform check.  Bit position 0 = channel 0; bit position 1 = channel 1, etc.
@param[out]     Status           - Pointer to where the scheduler timer error will be stored. Bit value 1 indicate there is an error
                                   The status is in channel bit mask format. Bit nth corresponds to channel nth inside the Controller

@retval None.
**/
VOID
BiosGetAepTimerErrorStatusChip (
  IN PSYSHOST                  Host,
  IN UINT8                     Socket,
  IN UINT8                     Controller,
  IN UINT8                     ChannelMask,
  OUT UINT8 *                  Status
  )
{
  UINT8 ch;
  CPGC_DDRT_ERR_CTL_STS_MC_2LM_STRUCT  cpgcDdrtErrCtlSts;
  UINT8 channelInSocket;
  UINT8 timeoutErrorPerCh = 0;
  UINT8 NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();

  for (ch = 0; ch < NumChPerMc; ch++) {
    if (!(ChannelMask & (1 << ch))) continue;
    channelInSocket = SSAGetChannelInSocket (Controller, ch);
    if (!(Host->var.mem.DdrtReadPendingQueueCreditLimitSet[Socket][channelInSocket])) continue;
    if (!IsDdrtDimm (Host, Socket, channelInSocket)) continue;

    cpgcDdrtErrCtlSts.Data = MemReadPciCfgEp (Socket, channelInSocket, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG);
    if (cpgcDdrtErrCtlSts.Bits.timeout_error == 1) {
      timeoutErrorPerCh |= (1 << ch);
    }
  } // ch

  *Status = timeoutErrorPerCh;
}
