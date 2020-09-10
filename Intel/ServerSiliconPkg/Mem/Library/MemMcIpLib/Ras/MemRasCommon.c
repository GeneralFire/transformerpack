/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include <Library/CpuCsrAccessLib.h>
#include <Library/BaseLib.h>
#include "Include/MemMcRegs.h"
#include <UncoreCommonIncludes.h>
#include <SiliconSetting.h>
#include <Library/CpuAndRevisionLib.h>

/**
  Check if a DIMM is a DDRT DIMM or not.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Dimm            DIMM index.

  @retval     TRUE            The DIMM is a DDRT DIMM.
  @retval     FALSE           The DIMM is not a DDRT DIMM or not present.
**/
BOOLEAN
EFIAPI
CheckForDdrtDimm (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT8   Dimm
  )
{
  MCDDRTCFG_MC_MAIN_STRUCT         McDdrtCfg;

  McDdrtCfg.Data = ReadCpuCsr (Skt, ChOnSkt, MCDDRTCFG_MC_MAIN_REG);
  if ( ((Dimm == 0 ) && (McDdrtCfg.Bits.slot0 == 1)) || ((Dimm == 1 ) && (McDdrtCfg.Bits.slot1 == 1)) )  {
    return TRUE;
  }
  return FALSE;
}

/**

  Set Imc MciStatus shadow register N0.

  @param Socket   - Socket ID
  @param ChOnSkt  - Channel on socket
  @param RegValue - RegValue

  @retval N/A

**/
VOID
EFIAPI
SetImcMciStatusShadowN0 (
  IN UINT8               Socket,
  IN UINT8               ChOnSkt,
  IN UINT32              RegValue
  )
{
  WriteCpuCsr (Socket, ChOnSkt, IMC0_MC_STATUS_SHADOW_N0_MCDDC_DP_REG, RegValue);
}

/**

  Set Imc MciStatus shadow register N1.

  @param Socket   - Socket ID
  @param ChOnSkt  - Channel on socket
  @param RegValue - RegValue

  @retval N/A

**/
VOID
EFIAPI
SetImcMciStatusShadowN1 (
  IN UINT8               Socket,
  IN UINT8               ChOnSkt,
  IN UINT32              RegValue
  )
{
  WriteCpuCsr (Socket, ChOnSkt, IMC0_MC_STATUS_SHADOW_N1_MCDDC_DP_REG, RegValue);
}

/**
  Check if imc_fuse_download.mc_mca_recovery is enabled.

  @retval     TRUE            mca_recovery is enabled.
  @retval     FALSE           mca_recovery is disabled.
**/
BOOLEAN
EFIAPI
CheckMcMcaRecovery (
  VOID
  )
{
  IMC_FUSE_DOWNLOAD_SHADOW_MC_MAIN_STRUCT FuseDownLoadShadowMcCfg;

  FuseDownLoadShadowMcCfg.Data = ReadCpuCsr (0, 0, IMC_FUSE_DOWNLOAD_SHADOW_MC_MAIN_REG);
  if (FuseDownLoadShadowMcCfg.Bits.fuse_shadow_mc_mca_recovery == 1) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**

  Enable or disable Patrol Error Downgrade
  @param Socket                   - Socket ID
  @param ChOnSkt                  - Channel on socket
  @param PatrolErrorDowngradeEn   - Patrol Error Downgrade setting
  @param RegAddr                  - CSR address
  @param RegValue                 - CSR value
**/
VOID
EFIAPI
SetPatrolErrorDowngrade (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt,
  IN  UINT8   PatrolErrorDowngradeEn,
  OUT UINT32  *RegAddr,
  OUT UINT32  *RegValue
  )
{
  MC0_DP_CHKN_BIT_MCDDC_DP_STRUCT     MC0DpChknBit;

  MC0DpChknBit.Data = ReadCpuCsr(Socket, ChOnSkt, MC0_DP_CHKN_BIT_MCDDC_DP_REG);
  MC0DpChknBit.Bits.ign_ptrl_uc = PatrolErrorDowngradeEn;
  *RegAddr = MC0_DP_CHKN_BIT_MCDDC_DP_REG;
  *RegValue = MC0DpChknBit.Data;
}
/**
  Get SMISPARECTL register value.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @return   scrubctl register value
 */
UINT32
EFIAPI
GetSmiSpareCtl (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  )
{
  SMISPARECTL_MC_MAIN_STRUCT     SmiSpareCtl;

  SmiSpareCtl.Data = ReadCpuCsr (Skt, ChOnSkt, SMISPARECTL_MC_MAIN_REG);
  return (UINT8)SmiSpareCtl.Data;
}

/**
  Set SMISPARECTL register value.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  SelSmi          Value for intrpt_sel_smi
  @param[in]  SelCmci         Value for intrpt_sel_cmci
  @param[in]  SelPin          Value for intrpt_sel_pin

  @return  None
 */
VOID
EFIAPI
SetSmiSpareCtl (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT32  SelSmi,      OPTIONAL
  IN  UINT32  SelCmci,     OPTIONAL
  IN  UINT32  SelPin       OPTIONAL
  )
{
  SMISPARECTL_MC_MAIN_STRUCT     SmiSpareCtl;

  SmiSpareCtl.Data = ReadCpuCsr (Skt, ChOnSkt, SMISPARECTL_MC_MAIN_REG);
  if (SelSmi != 0xff) {
    SmiSpareCtl.Bits.intrpt_sel_smi = SelSmi;
  }
  if (SelCmci != 0xff) {
    SmiSpareCtl.Bits.intrpt_sel_cmci = SelCmci;
  }
  if (SelPin != 0xff) {
    SmiSpareCtl.Bits.intrpt_sel_pin = SelPin;
  }

  WriteCpuCsr (Skt, ChOnSkt, SMISPARECTL_MC_MAIN_REG, SmiSpareCtl.Data);
  return;
}



/**
  Clear the Error logged Retry register

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
**/
VOID
EFIAPI
ClearRetryRegister (
  IN    UINT8      Skt,
  IN    UINT8      ChOnSkt
  )
{
  RETRY_RD_ERR_LOG_MCDDC_DP_STRUCT          RetryRdErrLog;
  RETRY_RD_ERR_SET2_LOG_MCDDC_DP_STRUCT     RetryRdErrLogSet2;

  RetryRdErrLog.Data      = ReadCpuCsr (Skt, ChOnSkt, RETRY_RD_ERR_LOG_MCDDC_DP_REG);
  RetryRdErrLogSet2.Data  = ReadCpuCsr (Skt, ChOnSkt, RETRY_RD_ERR_SET2_LOG_MCDDC_DP_REG);

  if (RetryRdErrLog.Bits.v == 0x01 || RetryRdErrLog.Bits.uc == 0x01) {
    RetryRdErrLog.Bits.v = 0;
    RetryRdErrLog.Bits.uc = 0;
    WriteCpuCsr (Skt, ChOnSkt, RETRY_RD_ERR_LOG_MCDDC_DP_REG, RetryRdErrLog.Data);
  }

  if (RetryRdErrLogSet2.Bits.v == 0x01 || RetryRdErrLogSet2.Bits.uc == 0x01) {
    RetryRdErrLogSet2.Bits.v = 0;
    RetryRdErrLogSet2.Bits.uc = 0;
    WriteCpuCsr (Skt, ChOnSkt, RETRY_RD_ERR_SET2_LOG_MCDDC_DP_REG, RetryRdErrLogSet2.Data);
  }
}


/**

  Clear Bit 0 of COR_CNT_CP/WAC/RAC
  @param[in]  Socket             Socket index
  @param[in]  ChOnSkt            Channel

**/
VOID
EFIAPI
ClearOsToCorCntPg (
  IN    UINT8      Socket,
  IN    UINT8      ChOnSkt
)
{
  MC_2LMCNTL_SAI_COR_CNT_PG_RD_POLICY_N0_MC_2LM_STRUCT        CorCntPGRead;
  MC_2LMCNTL_SAI_COR_CNT_PG_WR_POLICY_N0_MC_2LM_STRUCT        CorCntPGwrite;

  CorCntPGRead.Data = ReadCpuCsr (Socket, ChOnSkt, MC_2LMCNTL_SAI_COR_CNT_PG_RD_POLICY_N0_MC_2LM_REG);
  CorCntPGRead.Data &= ~BIT0;
  WriteCpuCsr (Socket, ChOnSkt, MC_2LMCNTL_SAI_COR_CNT_PG_RD_POLICY_N0_MC_2LM_REG, CorCntPGRead.Data);

  CorCntPGwrite.Data = ReadCpuCsr (Socket, ChOnSkt, MC_2LMCNTL_SAI_COR_CNT_PG_WR_POLICY_N0_MC_2LM_REG);
  CorCntPGwrite.Data &= ~BIT0;
  WriteCpuCsr (Socket, ChOnSkt, MC_2LMCNTL_SAI_COR_CNT_PG_WR_POLICY_N0_MC_2LM_REG, CorCntPGwrite.Data);

}


/**
  Get information from the retry-read register.

  All output parameters are optional. The caller may pass NULL for parameters
  that it does not need. If the function return value is FALSE, then none of
  the requested output parameters are returned.

  @param[in]  Socket          Socket index.
  @param[in]  ChannelOnSocket Channel on socket.
  @param[in]  SystemAddress   Physical address read from Mcibank.
  @param[out] Uncorrectable   Uncorrectable error.
  @param[out] Mode            ECC mode.
  @param[out] Rank            Rank (encoded chip select).
  @param[out] SubRank         Encoded subrank.
  @param[out] Bank            Bank ID (group and address).
  @param[out] Device          Device number.
  @param[out] Column          Column address.
  @param[out] Row             Row address.
  @param[out] SingleBit       Indicates if there was a single-bit correction.
  @param[out] Transfer        Transfer number of the single-bit correction.
  @param[out] InterLocSyn     Intermediate Locator Syndrome from correction path which are only valid when the mode is sddc 1lm.

  @retval     TRUE            Retry-read register is valid.
  @retval     FALSE           Retry-read register is not valid.
**/
BOOLEAN
EFIAPI
GetRetryReadInfo (
  IN  UINT8   Socket,
  IN  UINT8   ChannelOnSocket,
  IN  UINT64  SystemAddress,
  OUT UINT8   *Uncorrectable,   OPTIONAL
  OUT UINT32  *Mode,            OPTIONAL
  OUT UINT8   *Rank,            OPTIONAL
  OUT UINT8   *SubRank,         OPTIONAL
  OUT UINT8   *Bank,            OPTIONAL
  OUT UINT8   *Device,          OPTIONAL
  OUT UINT32  *Column,          OPTIONAL
  OUT UINT32  *Row,             OPTIONAL
  OUT UINT8   *SingleBit,       OPTIONAL
  OUT UINT8   *Transfer,        OPTIONAL
  OUT UINT16  *InterLocSyn      OPTIONAL
  )
{

  RETRY_RD_ERR_LOG_MCDDC_DP_STRUCT                  ReturnRetryRdErrLog;
  RETRY_RD_ERR_LOG_ADDRESS1_MCDDC_DP_STRUCT         ReturnRetryRdErrLogAddr1;
  RETRY_RD_ERR_LOG_ADDRESS2_MCDDC_DP_STRUCT         ReturnRetryRdErrLogAddr2;
  RETRY_RD_ERR_LOG_MISC_MCDDC_DP_STRUCT             ReturnRetryRdErrMisc;

  RETRY_RD_ERR_LOG_MCDDC_DP_STRUCT                  RetryRdErrLog;
  RETRY_RD_ERR_LOG_ADDRESS1_MCDDC_DP_STRUCT         RetryRdErrLogAddr1;
  RETRY_RD_ERR_LOG_ADDRESS2_MCDDC_DP_STRUCT         RetryRdErrLogAddr2;
  RETRY_RD_ERR_LOG_ADDRESS3_N0_MC_MAIN_STRUCT       RetryRdErrLogAddr3N0;
  RETRY_RD_ERR_LOG_ADDRESS3_N1_MC_MAIN_STRUCT       RetryRdErrLogAddr3N1;
  RETRY_RD_ERR_LOG_MISC_MCDDC_DP_STRUCT             RetryRdErrMisc;

  RETRY_RD_ERR_SET2_LOG_MCDDC_DP_STRUCT             RetryRdErrLogSet2;
  RETRY_RD_ERR_SET2_LOG_ADDRESS1_MCDDC_DP_STRUCT    RetryRdErrLogAddr1Set2;
  RETRY_RD_ERR_SET2_LOG_ADDRESS2_MCDDC_DP_STRUCT    RetryRdErrLogAddr2Set2;
  RETRY_RD_ERR_SET2_LOG_ADDRESS3_N0_MC_MAIN_STRUCT  RetryRdErrLogAddr3Set2N0;
  RETRY_RD_ERR_SET2_LOG_ADDRESS3_N1_MC_MAIN_STRUCT  RetryRdErrLogAddr3Set2N1;
  RETRY_RD_ERR_SET2_LOG_MISC_MCDDC_DP_STRUCT        RetryRdErrMiscSet2;
  BOOLEAN                                           RetryRdInformationFound;
  UINT64                                            Data64;
  UINT32                                            SystemAddrValid;

  RetryRdInformationFound = FALSE;

  RetryRdErrLog.Data         = ReadCpuCsr (Socket, ChannelOnSocket, RETRY_RD_ERR_LOG_MCDDC_DP_REG);
  if (RetryRdErrLog.Bits.v == 1) {
    RetryRdErrLogAddr1.Data    = ReadCpuCsr (Socket, ChannelOnSocket, RETRY_RD_ERR_LOG_ADDRESS1_MCDDC_DP_REG);
    RetryRdErrLogAddr2.Data    = ReadCpuCsr (Socket, ChannelOnSocket, RETRY_RD_ERR_LOG_ADDRESS2_MCDDC_DP_REG);
    RetryRdErrLogAddr3N0.Data  = ReadCpuCsr (Socket, ChannelOnSocket, RETRY_RD_ERR_LOG_ADDRESS3_N0_MC_MAIN_REG);
    RetryRdErrLogAddr3N1.Data  = ReadCpuCsr (Socket, ChannelOnSocket, RETRY_RD_ERR_LOG_ADDRESS3_N1_MC_MAIN_REG);
    RetryRdErrMisc.Data        = ReadCpuCsr (Socket, ChannelOnSocket, RETRY_RD_ERR_LOG_MISC_MCDDC_DP_REG);
  }

  RetryRdErrLogSet2.Data         = ReadCpuCsr (Socket, ChannelOnSocket, RETRY_RD_ERR_SET2_LOG_MCDDC_DP_REG);
  if (RetryRdErrLogSet2.Bits.v == 1) {
    RetryRdErrLogAddr1Set2.Data    = ReadCpuCsr (Socket, ChannelOnSocket, RETRY_RD_ERR_SET2_LOG_ADDRESS1_MCDDC_DP_REG);
    RetryRdErrLogAddr2Set2.Data    = ReadCpuCsr (Socket, ChannelOnSocket, RETRY_RD_ERR_SET2_LOG_ADDRESS2_MCDDC_DP_REG);
    RetryRdErrLogAddr3Set2N0.Data  = ReadCpuCsr (Socket, ChannelOnSocket, RETRY_RD_ERR_SET2_LOG_ADDRESS3_N0_MC_MAIN_REG);
    RetryRdErrLogAddr3Set2N1.Data  = ReadCpuCsr (Socket, ChannelOnSocket, RETRY_RD_ERR_SET2_LOG_ADDRESS3_N1_MC_MAIN_REG);
    RetryRdErrMiscSet2.Data        = ReadCpuCsr (Socket, ChannelOnSocket, RETRY_RD_ERR_SET2_LOG_MISC_MCDDC_DP_REG);
  }

  if ((RetryRdErrLog.Bits.v == 0) && (RetryRdErrLogSet2.Bits.v == 0)) {
    return FALSE;
  }

  ReturnRetryRdErrLog.Data      = 0;
  ReturnRetryRdErrLogAddr1.Data = 0;
  ReturnRetryRdErrLogAddr2.Data = 0;

  if (SystemAddress != (UINT64)-1) {
    //
    // Valid SystemAddress passed to this function. Check Which Retry_rd register is matching to this address
    //
    if (IsCpuAndRevisionOrLater(CPU_SPRSP, REV_B0)) {//for SPR B0 CPU, system_addr_valid bit is BIT23
      SystemAddrValid = (UINT32) ((RetryRdErrLog.Data & (BIT23)) >> 23);
    } else {
      SystemAddrValid = (UINT32) ((RetryRdErrLog.Data & (BIT22)) >> 22);
    }
    Data64 = ((LShiftU64 ((UINT64)RetryRdErrLogAddr3N1.Bits.sysaddress ,32)) | (RetryRdErrLogAddr3N0.Bits.sysaddress));
    if ((RetryRdErrLog.Bits.v == 1) && (Data64 == SystemAddress ) && (SystemAddrValid == 1)) {
      ReturnRetryRdErrLog.Data      = RetryRdErrLog.Data;
      ReturnRetryRdErrLogAddr1.Data = RetryRdErrLogAddr1.Data;
      ReturnRetryRdErrLogAddr2.Data = RetryRdErrLogAddr2.Data;
      ReturnRetryRdErrMisc.Data     = RetryRdErrMisc.Data;
      RetryRdInformationFound = TRUE;
    }

    if (IsCpuAndRevisionOrLater(CPU_SPRSP, REV_B0)) {//for SPR B0 CPU, system_addr_valid bit is BIT23
      SystemAddrValid = (UINT32) ((RetryRdErrLogSet2.Data & (BIT23)) >> 23);
    } else {
      SystemAddrValid = (UINT32) ((RetryRdErrLogSet2.Data & (BIT22)) >> 22);
    }
    Data64 = ((LShiftU64 ((UINT64)RetryRdErrLogAddr3Set2N1.Bits.sysaddress , 32)) | (RetryRdErrLogAddr3Set2N0.Bits.sysaddress));
    if ((RetryRdErrLogSet2.Bits.v == 1) && (Data64 == SystemAddress) && (SystemAddrValid == 1)) {
      ReturnRetryRdErrLog.Data      = RetryRdErrLogSet2.Data;
      ReturnRetryRdErrLogAddr1.Data = RetryRdErrLogAddr1Set2.Data;
      ReturnRetryRdErrLogAddr2.Data = RetryRdErrLogAddr2Set2.Data;
      ReturnRetryRdErrMisc.Data     = RetryRdErrMiscSet2.Data;
      RetryRdInformationFound = TRUE;
    }
  }
  if (RetryRdInformationFound == FALSE && SystemAddress != (UINT64)-1) {
    return FALSE;
  }else if (RetryRdInformationFound == FALSE && SystemAddress == (UINT64)-1) {
    //
    // if SystemAddress is not valid one OR retry information was not found above
    // then following code get retry information.
    //
    if ((RetryRdErrLog.Bits.v == 1) && (RetryRdErrLogSet2.Bits.v == 0)) {
      ReturnRetryRdErrLog.Data      = RetryRdErrLog.Data;
      ReturnRetryRdErrLogAddr1.Data = RetryRdErrLogAddr1.Data;
      ReturnRetryRdErrLogAddr2.Data = RetryRdErrLogAddr2.Data;
      ReturnRetryRdErrMisc.Data     = RetryRdErrMisc.Data;
    } else if ((RetryRdErrLog.Bits.v == 0) && (RetryRdErrLogSet2.Bits.v == 1)) {
      ReturnRetryRdErrLog.Data      = RetryRdErrLogSet2.Data;
      ReturnRetryRdErrLogAddr1.Data = RetryRdErrLogAddr1Set2.Data;
      ReturnRetryRdErrLogAddr2.Data = RetryRdErrLogAddr2Set2.Data;
      ReturnRetryRdErrMisc.Data     = RetryRdErrMiscSet2.Data;
    } else if ((RetryRdErrLog.Bits.v == 1) && (RetryRdErrLogSet2.Bits.v == 1)) {
      if ((RetryRdErrLog.Bits.en_patspr == 0) && (RetryRdErrLog.Bits.patspr == 0)) {
        ReturnRetryRdErrLog.Data      = RetryRdErrLog.Data;
        ReturnRetryRdErrLogAddr1.Data = RetryRdErrLogAddr1.Data;
        ReturnRetryRdErrLogAddr2.Data = RetryRdErrLogAddr2.Data;
        ReturnRetryRdErrMisc.Data     = RetryRdErrMisc.Data;
      } else if ((RetryRdErrLogSet2.Bits.en_patspr == 0) && (RetryRdErrLogSet2.Bits.patspr == 0)) {
        ReturnRetryRdErrLog.Data      = RetryRdErrLogSet2.Data;
        ReturnRetryRdErrLogAddr1.Data = RetryRdErrLogAddr1Set2.Data;
        ReturnRetryRdErrLogAddr2.Data = RetryRdErrLogAddr2Set2.Data;
        ReturnRetryRdErrMisc.Data     = RetryRdErrMiscSet2.Data;
      } else {
        ReturnRetryRdErrLog.Data      = RetryRdErrLog.Data;
        ReturnRetryRdErrLogAddr1.Data = RetryRdErrLogAddr1.Data;
        ReturnRetryRdErrLogAddr2.Data = RetryRdErrLogAddr2.Data;
        ReturnRetryRdErrMisc.Data     = RetryRdErrMisc.Data;
      }
    }
  }

  if (Uncorrectable != NULL) {
    *Uncorrectable = (UINT8) ReturnRetryRdErrLog.Bits.uc;
  }

  if (Mode != NULL) {
    *Mode = (UINT8) ReturnRetryRdErrLog.Bits.mode;
  }

  if (Rank != NULL) {
    *Rank = (UINT8) ReturnRetryRdErrLogAddr1.Bits.chip_select;
  }

  if (SubRank != NULL) {
    *SubRank = (UINT8) ReturnRetryRdErrLogAddr1.Bits.cbit;
  }

  if (Bank != NULL) {
    *Bank = (UINT8) ReturnRetryRdErrLogAddr1.Bits.bank;
  }

  if (Device != NULL) {
    *Device = (UINT8) ReturnRetryRdErrLogAddr1.Bits.failed_dev;
  }

  if (Column != NULL) {
    *Column = ReturnRetryRdErrLogAddr1.Bits.col << 2;
  }

  if (Row != NULL) {
    *Row = ReturnRetryRdErrLogAddr2.Bits.row;
  }

  if (SingleBit != NULL) {
    *SingleBit = (UINT8) ReturnRetryRdErrLog.Bits.single_bit_error;
  }

  if (Transfer != NULL) {
    if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
      *Transfer = (UINT8) ((ReturnRetryRdErrLog.Data & (BIT18 | BIT19 | BIT20 | BIT21)) >> 18);
    } else {
      *Transfer = (UINT8) ((ReturnRetryRdErrLog.Data & (BIT18 | BIT19 | BIT20)) >> 18);
    }
  }

  if (InterLocSyn != NULL) {
    *InterLocSyn = (UINT16) ReturnRetryRdErrMisc.Bits.inter_loc_syn;
  }

  return TRUE;
}

/**
  Clear Fnv viral received flag.

  Fnv_viral_received indicates viral is seen in a FNV read packet.
  This bit is sticky and will not be cleared by a warm reset.
  So BIOS needs to remember to clear it.

  @param Socket                   - Socket ID
  @param ChOnSkt                  - Channel on socket
  @param RegAddr                  - CSR address
  @param RegValue                 - CSR value
**/
VOID
EFIAPI
ClearFnvViralReceivedFlag (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt,
  OUT UINT32  *RegAddr,
  OUT UINT32  *RegValue
  )
{
#ifndef GNRSRF_HOST // GNRSRF_TODO - 22011066572
  MC0_DP_CHKN_BIT2_MCDDC_DP_STRUCT     MC0DpChknBit2;

  MC0DpChknBit2.Data = ReadCpuCsr(Socket, ChOnSkt, MC0_DP_CHKN_BIT2_MCDDC_DP_REG);
  MC0DpChknBit2.Bits.fnv_viral_received = 0;
  *RegAddr = MC0_DP_CHKN_BIT2_MCDDC_DP_REG;
  *RegValue = MC0DpChknBit2.Data;
#endif // GNRSRF_HOST
}

/**
  Set LMCE downgrade.

  This allows a SRAO patrol error to be logged/signaled as UCNA.
  Set this bit to a 0 to switch back to SRAO logging/signaling if required.
  Logging as UCNA is the required default behavior.

  @param Socket                   - Socket ID
  @param ChOnSkt                  - Channel on socket
  @param LmceDowngradeEn          - Lmce downgrade setting
  @param RegAddr                  - CSR address
  @param RegValue                 - CSR value
**/
VOID
EFIAPI
SetLmceDowngrade (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt,
  IN  UINT8   LmceDowngradeEn,
  OUT UINT32  *RegAddr,
  OUT UINT32  *RegValue
  )
{
  MC0_DP_CHKN_BIT_LMCE_PAT_DOWNGRADE_MCDDC_DP_STRUCT     MC0DpChknBitLmcePatDowngrade;

  MC0DpChknBitLmcePatDowngrade.Data = (UINT8) ReadCpuCsr(Socket, ChOnSkt, MC0_DP_CHKN_BIT_LMCE_PAT_DOWNGRADE_MCDDC_DP_REG);
  MC0DpChknBitLmcePatDowngrade.Bits.lmcedowngrade = LmceDowngradeEn;
  *RegAddr = MC0_DP_CHKN_BIT_LMCE_PAT_DOWNGRADE_MCDDC_DP_REG;
  *RegValue = MC0DpChknBitLmcePatDowngrade.Data;
}

/**
  Set Dimm Mtr register

  @param Socket                   - Socket ID
  @param ChOnSkt                  - Channel on socket
  @param Dimm                     - Dimm position in channel
  @param RnkCnt                   - value to be programmed in rank_cnt bit field
**/
VOID
EFIAPI
SetDimmMtr (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt,
  IN  UINT8   Dimm,
  IN  UINT32  RankCnt
  )
{
  DIMMMTR_0_MC_MAIN_STRUCT DimmMtr;
  UINT32 DimmMtrOffset[MAX_DIMM] = {
    DIMMMTR_0_MC_MAIN_REG, DIMMMTR_1_MC_MAIN_REG
  };

  DimmMtr.Data = ReadCpuCsr (Socket, ChOnSkt, DimmMtrOffset[Dimm]);
  DimmMtr.Bits.rank_cnt = RankCnt;
  WriteCpuCsr (Socket, ChOnSkt, DimmMtrOffset[Dimm], DimmMtr.Data);
}


/**
  Get bit field value of DimmMtr register

  @param[in]  Socket                   - Socket ID
  @param[in]  ChOnSkt                  - Channel on socket
  @param[in]  Dimm                     - Dimm position in channel
  @param[out] DimmPop                   - value to be programmed in rank_cnt bit field
**/
VOID
EFIAPI
GetDimmMtr (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt,
  IN  UINT8   Dimm,
  OUT UINT32  *DimmPop
  )
{
  DIMMMTR_0_MC_MAIN_STRUCT DimmMtr;
  UINT32 DimmMtrOffset[MAX_DIMM] = {
    DIMMMTR_0_MC_MAIN_REG, DIMMMTR_1_MC_MAIN_REG
  };

  DimmMtr.Data = ReadCpuCsr (Socket, ChOnSkt, DimmMtrOffset[Dimm]);
  if (DimmPop != NULL) {
    *DimmPop = DimmMtr.Bits.dimm_pop;
  }
}

/**
  Check if ADDDC Sparing should be disabled on this Channel.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.

  @retval     TRUE            The Sparing should be disbaled on this Channel
  @retval     FALSE           The Sparing is allowed on this channel
**/
BOOLEAN
EFIAPI
IsAdddcDisabled (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt
  )
{
  UINT8                          BurstLength;
  TCMR2SHADOW_MCDDC_CTL_STRUCT   TCMr2Shadow;

  TCMr2Shadow.Data = ReadCpuCsr (Skt, ChOnSkt, TCMR2SHADOW_MCDDC_CTL_REG);
  BurstLength = (TCMr2Shadow.Bits.scratch & 0x3);
  if (BurstLength == 1) {
    return FALSE;
  }
  return TRUE;
}

/**
  Check whether the ADDDC meta bit field is set/clear

  @param[in]  Socket                   - Socket ID
  @param[in]  ChOnSkt                  - Channel on socket

  @param[out] TRUE/FALSE               - ADDDC meta bit field is set(return TRUE)/clear(return FALSE)
**/
BOOLEAN
EFIAPI
CheckADDDCMetaBit (
  IN  UINT8   Socket,
  IN  UINT8   SktCh
  )
{
  CORRECTION_DEBUG_LOG_MCDDC_DP_STRUCT      CorrDebugLog;

  CorrDebugLog.Data = ReadCpuCsr(Socket, SktCh, CORRECTION_DEBUG_LOG_MCDDC_DP_REG);
  if (CorrDebugLog.Bits.adddc_meta_bit_failed == 1) {
    return TRUE;
  } else {
    return FALSE;
  }
}
