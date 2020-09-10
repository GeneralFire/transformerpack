/** @file
  This file contains the implementation of the BIOS version of the SSA services PPI.
  This version contains code for both client and server implementations.
  For Client:
  Delete include of BiosSsaMemoryServerConfig.h, BiosSsaMemoryDdrtConfig.h, and BiosSsaMemoryHbmConfig.h
  Delete definitions of SSA_REVISION_MEMORY_SERVER, SSA_REVISION_MEMORY_DDRT, and SSA_REVISION_MEMORY_HBM.
  Delete declarations of SsaMemoryServerConfig, SsaMemoryDdrtConfig, and SsaMemoryHbmConfig.
  Replace SsaMemoryServerConfig, SsaMemoryDdrtConfig, and SsaMemoryHbmConfig field values in SsaBiosServicesPpi with NULL.
  For Server:
  Delete include of BiosSsaMemoryClientConfig.h.
  Delete definition of SSA_REVISION_MEMORY_CLIENT.
  Delete declaration of SsaMemoryClientConfig.
  Replace SsaMemoryClientConfig field value in SsaBiosServicesPpi with NULL.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2017 Intel Corporation. <BR>

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
#include <Library/SystemInfoLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/CsrAccessLib.h>

//
//This BSSA file contains subsets of all BSSA function calls made from the Core files; all CSR accesses are made here.
//



//
// In the interest of code brevity, we are not doing error checking
// for Host->BiosGetSetMemSsTypeChip(). We assume that it points to
// BiosGetSetMemSsTypeChip, which is defined in this file and
// cannot fail. If this changes in the future, add the appropriate
// checks.
//


/**

  Function used to set the DqDB Byte Group pattern select.

  @param[in out] host                          - Pointer to sysHost
  @param[in]     Socket                        - Socket Id
  @param[in]     ChannelInSocket               - Channel number (0-based)
  @param[in]     SelBitmask                    - Specifies the Byte Group pattern selection options.

  Bits 7:0 of the bitmask correspond to pseudo channel 0 "Byte groups" and bits
  15:8 correspond to pseudo channel 1 "Byte groups".  "Byte groups" is in
  quotes because, for HBM, it consists of 8 Dq signals and the corresponding
  ECC signal.  A bit value of 0 indicates Byte Group option 0 which maps the
  Byte Group bit lanes [ECC+DQ7:DQ0] to the WDB bit lanes [DQ8:DQ0].  A bit
  value of 1 indicates Byte Group option 1 which maps the Byte Group bit lanes
  [ECC+DQ7:DQ0] to the WDB bit lanes [DQ17:DQ9].

  @retval N/A

**/
VOID
BiosSetDqdbByteGroupPatSelChip (
  IN OUT PSYSHOST                          Host,
  IN UINT8                                 Socket,
  IN UINT8                                 ChannelInSocket,
  IN UINT16                                SelBitmask
  )
{
  CPGC_HBM_PATTERNSEL_MCDDC_DP_STRUCT HbmPatSel     = {0};
  MRC_MST                             HbmMemSsType  = Hbm2MemSs;
  MRC_MST                             OrigMemSsType = 0;

  RcDebugPrint (SDBG_BSSA, "BiosSetDqdbByteGroupPatSelChip\n");

  BiosGetSetMemSsTypeChip (Host, Socket, TRUE, (MRC_LT*)&OrigMemSsType);
  BiosGetSetMemSsTypeChip (Host, Socket, FALSE, (MRC_LT*)&HbmMemSsType);

  HbmPatSel.Data = MemReadPciCfgEp (Socket, ChannelInSocket, CPGC_HBM_PATTERNSEL_MCDDC_DP_REG);

  HbmPatSel.Bits.hbm_pch0_bytegroup_mapping = (UINT8)((SelBitmask) & 0xff);
  HbmPatSel.Bits.hbm_pch1_bytegroup_mapping = (UINT8)((SelBitmask >> 8) & 0xff);

  MemWritePciCfgEp (Socket, ChannelInSocket, CPGC_HBM_PATTERNSEL_MCDDC_DP_REG, HbmPatSel.Data);

  BiosGetSetMemSsTypeChip (Host, Socket, FALSE, (MRC_LT*)&OrigMemSsType);
}

/**

  Function used to set the DqDB source for the DBI signals.

  @param[in out] Host                          - Pointer to sysHost
  @param[in]     Socket                        - Socket Id
  @param[in]     ChannelInSocket               - Channel number (0-based)
  @param[in]     SourceFromDqBit               - Specifies whether the DBI is sourced from DBI calculation in memory controller (FALSE) or duplicated from one of the DQ bits (TRUE).
  @param[out]    DqBit                         - Specifies the zero based DQ bit (0 to 7) when the SourceFromDqLane is TRUE.

  @retval N/A

**/
VOID
BiosSetDqdbDbiDataSourceChip (
  IN OUT PSYSHOST                          Host,
  IN UINT8                                 Socket,
  IN UINT8                                 ChannelInSocket,
  IN BOOLEAN                               SourceFromDqBit,
  OUT UINT8                                DqBit
  )
{
  CPGC_HBM_MISC_MCDDC_DP_STRUCT HbmMisc       = {0};
  MRC_MST                       HbmMemSsType  = Hbm2MemSs;
  MRC_MST                       OrigMemSsType = 0;

  RcDebugPrint (SDBG_BSSA, "BiosSetDqdbDbiDataSourceChip\n");

  BiosGetSetMemSsTypeChip (Host, Socket, TRUE, (MRC_LT*)&OrigMemSsType);
  BiosGetSetMemSsTypeChip (Host, Socket, FALSE, (MRC_LT*)&HbmMemSsType);

  HbmMisc.Data = MemReadPciCfgEp (Socket, ChannelInSocket, CPGC_HBM_MISC_MCDDC_DP_REG);

  HbmMisc.Bits.hbm_dbi_datasrc_sel = SourceFromDqBit;

  if (SourceFromDqBit) {
    HbmMisc.Bits.hbm_dbi_dq_copy = DqBit;
  }

  MemWritePciCfgEp (Socket, ChannelInSocket, CPGC_HBM_MISC_MCDDC_DP_REG, HbmMisc.Data);

  // Reset to previous value.
  BiosGetSetMemSsTypeChip (Host, Socket, FALSE, (MRC_LT*)&OrigMemSsType);
}

/**

  Function used to set the DqDB source for the data parity signals.

  @param[in]  Host                              - Pointer to sysHost
  @param[in]  Socket                            - Socket Id
  @param[in]  ChannelInSocket                   - Channel number (0-based)
  @param[in]  SourceFromDqBit                   - Specifies whether the data parity is sourced from data parity calculation in memory controller (FALSE) or duplicated from one of the DQ bits (TRUE).
  @param[out] DqBit                             - Specifies the zero based DQ bit (0 to 7) when the SourceFromDqLane is TRUE

  @retval N/A

**/
VOID
BiosSetDqdbDataParityDataSourceChip (
  IN OUT PSYSHOST                          Host,
  IN UINT8                                 Socket,
  IN UINT8                                 ChannelInSocket,
  IN BOOLEAN                               SourceFromDqBit,
  OUT UINT8                                DqBit
  )
{
  CPGC_HBM_MISC_MCDDC_DP_STRUCT HbmMisc       = {0};
  MRC_MST                       HbmMemSsType  = Hbm2MemSs;
  MRC_MST                       OrigMemSsType = 0;

  RcDebugPrint (SDBG_BSSA, "BiosSetDqdbDataParityDataSourceChip\n");

  BiosGetSetMemSsTypeChip (Host, Socket, TRUE, (MRC_LT*)&OrigMemSsType);
  BiosGetSetMemSsTypeChip (Host, Socket, FALSE, (MRC_LT*)&HbmMemSsType);

  HbmMisc.Data = MemReadPciCfgEp (Socket, ChannelInSocket, CPGC_HBM_MISC_MCDDC_DP_REG);

  HbmMisc.Bits.hbm_dparity_datasrc_sel = SourceFromDqBit;

  if (SourceFromDqBit) {
    HbmMisc.Bits.hbm_dparity_dq_copy = DqBit;
  }

  MemWritePciCfgEp (Socket, ChannelInSocket, CPGC_HBM_MISC_MCDDC_DP_REG, HbmMisc.Data);

  BiosGetSetMemSsTypeChip (Host, Socket, FALSE, (MRC_LT*)&OrigMemSsType);
}

/**

  Function used to configure whether duplicate the same cacheline transaction on both pseudo channels.

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] ChannelInSocket                   - Channel number (0-based)
  @param[in] PseudoChannel                     - Pseudo channel configuration setting.

  @retval N/A

**/
VOID
BiosSetPseudoChannelConfigChip (
  IN OUT PSYSHOST                          Host,
  IN UINT8                                 Socket,
  IN UINT8                                 ChannelInSocket,
  IN PSEUDO_CH_SEL                         PseudoChannel
  )
{
  CPGC_HBM_MISC_MCDDC_DP_STRUCT             HbmMisc       = {0};
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT BaseAddrHi    = {0};
  MRC_MST                                   HbmMemSsType  = Hbm2MemSs;
  MRC_MST                                   OrigMemSsType = 0;

  RcDebugPrint (SDBG_BSSA, "BiosSetPseudoChannelConfigChip\n");

  BiosGetSetMemSsTypeChip (Host, Socket, TRUE, (MRC_LT*)&OrigMemSsType);
  BiosGetSetMemSsTypeChip (Host, Socket, FALSE, (MRC_LT*)&HbmMemSsType);

  HbmMisc.Data    = MemReadPciCfgEp (Socket, ChannelInSocket, CPGC_HBM_MISC_MCDDC_DP_REG);
  BaseAddrHi.Data = MemReadPciCfgEp (Socket, ChannelInSocket, CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_REG);

  if (PseudoChannel == PSEUDO_CH_BOTH) {
    HbmMisc.Bits.hbm_invert_pseudo_chan = 1;
    BaseAddrHi.Bits.pseudo_channel_addr = 0;
  } else {
    HbmMisc.Bits.hbm_invert_pseudo_chan = 0;
    switch (PseudoChannel) {
    case PSEUDO_CH_0:
      BaseAddrHi.Bits.pseudo_channel_addr = 0;
      break;
    case PSEUDO_CH_1:
      BaseAddrHi.Bits.pseudo_channel_addr = 1;
      break;
    default:
      ASSERT(!"Invalid code path. The only supported channels are PSEUDO_CH_0 PSEUDO_CH_1.");
      break;
    }
  }

  MemWritePciCfgEp (Socket, ChannelInSocket, CPGC_HBM_MISC_MCDDC_DP_REG, HbmMisc.Data);

  BiosGetSetMemSsTypeChip (Host, Socket, FALSE, (MRC_LT*)&OrigMemSsType);
}

/**
  Function used to configure the parity error event capture.

  @param[in out] Host                              - Pointer to sysHost
  @param[in]     Socket                            - Socket Id
  @param[in]     ChannelInSocket                   - Channel number (0-based)
  @param[in]     IgnoreAwordParityError            - Specifies whether to ignore Aword parity for the given channel.
  @param[in]     IgnoreDwordParityError            - Specifies whether to ignore Dword parity for the given channel.

  @retval NotAvailable if the system does not provide parity error event capture. Else Success.

**/
SSA_STATUS
BiosSetParityErrorConfigChip (
  IN OUT PSYSHOST                          Host,
  IN UINT8                                 Socket,
  IN UINT8                                 ChannelInSocket,
  IN BOOLEAN                               IgnoreAwordParityError,
  IN BOOLEAN                               IgnoreDwordParityError
  )
{
  CPGC_HBM_MISC_MCDDC_DP_STRUCT HbmMisc       = {0};
  MRC_MST                       HbmMemSsType  = Hbm2MemSs;
  MRC_MST                       OrigMemSsType = 0;

  RcDebugPrint (SDBG_BSSA, "BiosSetParityErrorConfigChip\n");

  BiosGetSetMemSsTypeChip (Host, Socket, TRUE, (MRC_LT*)&OrigMemSsType);
  BiosGetSetMemSsTypeChip (Host, Socket, FALSE, (MRC_LT*)&HbmMemSsType);

  HbmMisc.Data = MemReadPciCfgEp (Socket, ChannelInSocket, CPGC_HBM_MISC_MCDDC_DP_REG);

  HbmMisc.Bits.ignore_hbm_aerr = IgnoreAwordParityError;
  HbmMisc.Bits.ignore_hbm_derr = IgnoreDwordParityError;

  MemWritePciCfgEp (Socket, ChannelInSocket, CPGC_HBM_MISC_MCDDC_DP_REG, HbmMisc.Data);

  BiosGetSetMemSsTypeChip (Host, Socket, FALSE, (MRC_LT*)&OrigMemSsType);

  // HBM supports parity error checks.
  return Success;
}

/**
  Function used to get the parity error event capture Status.

  @param[in out] Host                              - Pointer to sysHost
  @param[in]     Socket                            - Socket Id
  @param[in]     ChannelInSocket                   - Channel number (0-based)
  @param[out]    CurrentAwordParityError           - Pointer to where the current Aword parity error Status for the given channel will be stored.
  @param[out]    ObservedAwordParityError          - Pointer to where the observed Aword parity error Status for the given channel will be stored.
  @param[out]    CurrentDwordParityError           - Pointer to where the current Dword parity error Status for the given channel will be stored.
  @param[out]    ObservedDwordParityError          - Pointer to where the observed Dword parity error Status for the given channel will be stored.

  @retval NotAvailable if the system does not provide parity error event capture. Else Success.

**/
SSA_STATUS
BiosGetParityErrorStatusChip (
  IN OUT PSYSHOST                          Host,
  IN UINT8                                 Socket,
  IN UINT8                                 ChannelInSocket,
  OUT BOOLEAN                              *CurrentAwordParityError,
  OUT BOOLEAN                              *ObservedAwordParityError,
  OUT BOOLEAN                              *CurrentDwordParityError,
  OUT BOOLEAN                              *ObservedDwordParityError
  )
{
  CPGC_HBM_MISC_MCDDC_DP_STRUCT HbmMisc       = {0};
  MRC_MST                       HbmMemSsType  = Hbm2MemSs;
  MRC_MST                       OrigMemSsType = 0;

  RcDebugPrint (SDBG_BSSA, "BiosGetParityErrorStatusChip\n");

  BiosGetSetMemSsTypeChip (Host, Socket, TRUE, (MRC_LT*)&OrigMemSsType);
  BiosGetSetMemSsTypeChip (Host, Socket, FALSE, (MRC_LT*)&HbmMemSsType);

  HbmMisc.Data = MemReadPciCfgEp (Socket, ChannelInSocket, CPGC_HBM_MISC_MCDDC_DP_REG);

  *CurrentAwordParityError  = (BOOLEAN) HbmMisc.Bits.current_hbm_aerr;
  *ObservedAwordParityError = (BOOLEAN) HbmMisc.Bits.observed_hbm_aerr_assert;

  *CurrentDwordParityError  = (BOOLEAN) HbmMisc.Bits.current_hbm_derr;
  *ObservedDwordParityError = (BOOLEAN) HbmMisc.Bits.observed_hbm_derr_assert;

  BiosGetSetMemSsTypeChip (Host, Socket, FALSE, (MRC_LT*)&OrigMemSsType);

  //
  // HBM supports parity error.
  //
  return Success;
}

/**
  Function used to get the per-strobe error Status.

  @param[in out] Host                              - Pointer to sysHost
  @param[in]     Socket                            - Socket Id
  @param[in]     ChannelInSocket                   - Channel number (0-based)
  @param[out]    Status                            - Pointer to where the strobe error Status will be stored.  Only the bits [3:0] are valid because HBM has a 32-bit strobe Group.  Bit value 1 = at least one error occurred; bit value 0 = no errors occurred.  Bit position 0 = strobe Group 0; bit position 1 = strobe Group 1, etc.

  @retval NotAvailable if the system does not provide strobe error Status.  Else Success.

**/
SSA_STATUS
BiosGetStrobeErrorStatusChip (
  IN OUT PSYSHOST                          Host,
  IN UINT8                                 Socket,
  IN UINT8                                 ChannelInSocket,
  OUT UINT8                                *Status
  )
{
  CPGC_ERR_BYTE_MCDDC_DP_STRUCT ErrByte       = {0};
  MRC_MST                       HbmMemSsType  = Hbm2MemSs;
  MRC_MST                       OrigMemSsType = 0;

  RcDebugPrint (SDBG_BSSA, "BiosGetStrobeErrorStatusChip\n");

  BiosGetSetMemSsTypeChip (Host, Socket, TRUE, (MRC_LT*)&OrigMemSsType);
  BiosGetSetMemSsTypeChip (Host, Socket, FALSE, (MRC_LT*)&HbmMemSsType);

  ErrByte.Data = MemReadPciCfgEp (Socket, ChannelInSocket, CPGC_ERR_BYTE_MCDDC_DP_REG);

  //
  // For HBM, only the 3 least-significant bits are used.
  //
  *Status = (UINT8)(ErrByte.Data & 7);

  BiosGetSetMemSsTypeChip (Host, Socket, FALSE, (MRC_LT*)&OrigMemSsType);

  // HBM Supports strobe error status.
  return Success;
}

/**
  Function used to get or set the IO level flag.

  @param[in out] Host                              - Pointer to sysHost
  @param[in]     Socket                            - Socket Id
  @param[in]     IsGet                             - Specifies whether the operation is a get or set.  TRUE = get; FALSE = set.
  @param[in out] MemSsType                         - Pointer to where the MemSsType value will be/is stored. Set the value pointed to by MemSsType if IsGet is FALSE, while get the MemSsType if IsGet is TRUE

  @retval This function returns Success Status codes
**/

SSA_STATUS
BiosGetSetMemSsTypeChip (
  IN OUT PSYSHOST                          Host,
  IN UINT8                                 Socket,
  IN BOOLEAN                               IsGet,
  IN OUT MRC_LT                            *MemSsType
  )
{
  if (IsGet) {
    // Save current MemSsType
    *MemSsType = GetSysCpuCsrAccessVar()->MemSsType[Socket];
  } else {
    // Set MemSsType
    GetSysCpuCsrAccessVar()->MemSsType[Socket] = *MemSsType;
  }
  return Success;
}
