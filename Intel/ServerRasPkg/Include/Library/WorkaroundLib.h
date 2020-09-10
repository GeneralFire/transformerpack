/** @file
  Interface of SSR library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#ifndef __WORKAROUND_LIB_H__
#define __WORKAROUND_LIB_H__

#define DISABLE_CLK_GATING 0
#define ENABLE_CLK_GATING  1

/**
  @param[in]  Socket      - Socket number
  @param[in]  SktCh       - Socket channel number
  @param[in]  SrcRank     - Source rank number

  @retval None
**/
VOID
EFIAPI
Wa_4930005(
  IN  UINT8       Socket,
  IN  UINT8       SktCh,
  IN  UINT8       SrcRank
  );



/**
  @param[in]  NodeId      - Memory controller ID
  @param[in]  Ch          - Channel number

  @retval None
**/
VOID
EFIAPI
Wa_5330286 (
  IN   UINT8      NodeId,
  IN   UINT8      Ch
  );


/**
  Clear the Work Around bits that was setup for Fast div 3 mode adddc sparing

  @param NodeId - Node ID
**/
VOID
EFIAPI
ClearSparingFastDiv3Wa (
  IN UINT8 NodeId
  );


/**
  @param[in]  Skt      - Socket number

  @retval None
**/
VOID
EFIAPI
Wa_5331846 (
  IN  INT8   Skt
  );

/**
  @param[in]  Skt           - Socket number
  @param[in]  ChOnSkt       - Socket channel number
  @param[in]  Rank          - Rank number

  @retval None
**/
VOID
EFIAPI
Wa_5372419 (
  IN   UINT8   Skt,
  IN   UINT8   ChOnSkt,
  IN   UINT8   Rank
  );


/**
  @param [IN]  IioLocalUcSeverity    current IIO local UC severity.

  @retval      IIO local error UC severity to be set.
 **/
UINT32
EFIAPI
Wa_2006679835 (
  IN  UINT32   IioLocalUcSeverity
  );

/**
  CDF IEH behavior of CSR parity error is inconsistent. In the event of a CSR parity error, IEH will keep on sending URs for
  any register read and gests locked up.

  @param [IN]  SouthIehFlag    indicate this is south IEH device
  @param [IN]  RegValue        register value of south IEH

  @retval      none
 **/
VOID
EFIAPI
Wa_2006679835_reset (
  IN  BOOLEAN   SouthIehFlag,
  IN  UINT32    RegValue
  );

/**
ICXR_RAS: SPD_BITMAP.IP_BITMAP for SPD Smbus defaults to 0xC rather than 0x00 resulting in errors
logged in global error logic rather than local error logic of Global IEH

  @param [IN]  Socket    The socket number
  @retval      none
 **/
VOID
EFIAPI
Wa_1407595452 (
  IN  UINT8   Socket
  );

/**
  When a C2U.MSMI happens, the source id is erroneously getting logged in the SMISRCLOG register.
  BIOS WA: BIOS snoops the SMISRCLOG Register’s SidebandValid and SidebandPortID fields When a C2U.MSMI happenss.
           If the SidebandValid is set, and the logged SidebandPortID is within core range,
           clear the SMISRCLOG.SidebandValid and SMISRCLOG.SidebandPortID.

  @retval      none
 **/

VOID
EFIAPI
Wa_1706854359 (
  VOID
  );

/**
  Zero the scrub interval.

  This function must be called before modifying the scrub enable for any memory
  channel (except the first time after reset).

  @param[in]  Socket    Socket index.
  @param[in]  Mc        MC index within the socket.
**/
VOID
EFIAPI
ClearScrubInterval (
  IN UINT8  Socket,
  IN UINT8  Mc
  );

/**
  Restore the scrub interval.

  This function must be called after modifying the scrub enable for any memory
  channel (except the first time after reset).

  @param[in]  Socket    Socket index.
  @param[in]  Mc        MC index within the socket.
**/
VOID
EFIAPI
RestoreScrubInterval (
  IN UINT8  Socket,
  IN UINT8  Mc
  );

/**
  B37:FW_UPD in MCi_STATUS is incorrectly repurposed to a valid for extra_err_info in MCi_MISC.
  BIOS WA: Skip set B37:FW_UPD in MCi_STATUS for ICX, ICXD and SNR A0 stepping CPU.

  @retval TRUE       Skip FW_UPD setting.
  @retval FALSE      Set FW_UPD according to system configuration.
 **/
BOOLEAN
EFIAPI
SkipFwUpdSettingWa1606892119 (
  VOID
  );

/**
  IEH errors that are disabled in the MCi_CTL2 of the Ubox MCA Bank will still set the SMI_ERR_SRC_STATUS.
  BIOS WA: BIOS add checking MCi_CTL2 during error handling. BIOS skip the bank if MCi_CTL2  bit 34 is cleared.

  @retval TRUE        Error log in MC bank 6 should be skipped.
  @retval FALSE       Error log in MC bank 6 should be used.
 **/
BOOLEAN
EFIAPI
SkikpMc6IehErrorLogWa1506843150 (
  VOID
  );

/**
  Apply the work around required for ADDDC sparing on Standard RAS parts.
  If either the dimm slot is empty or populated with DDRT dimm

  @param Socket    - Socket ID
  @param SktCh     - Channel on socket

  @retval none

**/
VOID
EFIAPI
AdddcStandardRasWa (
  UINT8 Socket,
  UINT8 SktCh
  );

/**
  5370783: Infinite looping SMM error handler when Emcagen2 enabled and system recoverying from IERR.

  @param[in]  Socket           - Socket number

  @retval None
**/
VOID
EFIAPI
ClearPcuMcaErrSrcWa5370783 (
  IN   UINT8   Socket
  );

/**
  1409498645: BIOS workaround to enable internal Punit error sources for GlobalFatal/Viral signaling.

  @param[in]  Socket           - Socket number

  @retval None
**/
VOID
EFIAPI
EnablePunitErrorSourceWa1409498645 (
  IN   UINT8   Socket
  );

/**
  1507442372: BIOS doesn't signal MCE when exit SMM for PCU MSMI.

   @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

  @retval TRUE        Skip MCE signal.
  @retval FALSE       Signal MCE.
**/
BOOLEAN
EFIAPI
SkipMceSignalWa1507442372 (
  IN UINT32 McBankIdx
  );
#endif  // __WORKAROUND_LIB_H__
