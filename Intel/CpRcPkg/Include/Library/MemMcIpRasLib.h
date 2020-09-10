/** @file
  MC IP RAS library class header file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#ifndef _MEM_MC_IP_RAS_LIB_H_
#define _MEM_MC_IP_RAS_LIB_H_

#include <Uefi.h>
#include <Library/MemMcIpLib.h>

/**
  Get DIMM alert information.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.

  @return     DIMM alert information.
**/
UINT32
EFIAPI
GetDimmAlert (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt
  );

/**
  Clear CAP error status.

  @param[in]  Socket          Socket index.
  @param[in]  ChOnSkt         Channel on socket.
**/
VOID
EFIAPI
ClearCAPErrorPerCh (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt
  );

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
  );

/**
  Get current channel and rank info of patrol scrub complete.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[out] ChOnSkt         Channel on socket.
  @param[out] Rank            Rank index.
**/
EFI_STATUS
EFIAPI
GetScrubbedRank (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  OUT UINT8   *ChOnSkt,
  OUT UINT8   *Rank
  );

/**
  Get tad range for the TAD.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         channel on socket.
  @param[out] MinTad          Minital tad index for the patrol scrub.
  @param[out] MaxTad          Minital tad index for the patrol scrub.
**/
VOID
EFIAPI
GetPatrolScrubRange (
  IN  UINT8     Skt,
  IN  UINT8     ChOnSkt,
  IN OUT UINT8 *MinTad,
  IN OUT UINT8 *MaxTad
  );

/**
  Get tad range for the TAD.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         channel on socket.
  @param[in]  TadIndex        Tad index.
  @param[out] TadBase         Tad base.
  @param[out] TadLimit        Tad limit.
**/
VOID
EFIAPI
GetTadInfoRange (
  IN     UINT8      Skt,
  IN     UINT8      ChOnSkt,
  IN     UINT8      TadIndex,
  IN OUT UINT64    *TadBase,
  IN OUT UINT64    *TadLimit
  );
/**
  Set ADDDC region register.

  Failed bank and non-failed bank are optional; if 0xff is passed in, the
  corresponding field is not updated.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  RegionNum       ADDDC region index.
  @param[in]  FainRank        Failed rank.
  @param[in]  FailSubRank     Failed sub-rank.
  @param[in]  NonFailRank     Non-failed rank.
  @param[in]  NonFailSubRank  Non-failed sub-rank.
  @param[in]  RegionSize      Region size.
  @param[in]  FailedBank      Failed bank.
  @param[in]  NonFailedBank   Non-failed bank.
**/
VOID
EFIAPI
SetAdddcRegion (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT8   RegionNum,
  IN  UINT8   FailRank,
  IN  UINT8   FailSubRank,
  IN  UINT8   NonFailRank,
  IN  UINT8   NonFailSubRank,
  IN  UINT8   RegionSize,
  IN  UINT8   FailedBank,       OPTIONAL
  IN  UINT8   NonFailedBank     OPTIONAL
  );


/**

  Set ADDDC region register in scheduler.

  Failed bank and non-failed bank are optional; if 0xff is passed in, the
  corresponding field is not updated.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  RegionNum       ADDDC region index.
  @param[in]  FainRank        Failed rank.
  @param[in]  FailSubRank     Failed sub-rank.
  @param[in]  NonFailRank     Non-failed rank.
  @param[in]  NonFailSubRank  Non-failed sub-rank.
  @param[in]  RegionSize      Region size.
  @param[in]  FailedBank      Failed bank.
  @param[in]  NonFailedBank   Non-failed bank.
**/
VOID
EFIAPI
SetSchedulerAdddcRegion (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT8   RegionNum,
  IN  UINT8   FailRank,
  IN  UINT8   FailSubRank,
  IN  UINT8   NonFailRank,
  IN  UINT8   NonFailSubRank,
  IN  UINT8   RegionSize,
  IN  UINT8   FailedBank,       OPTIONAL
  IN  UINT8   NonFailedBank     OPTIONAL
  );

/**
  Clear the 'region_enable' bit of the ADDDC region control register.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  RegionNum       ADDDC region index.
**/
VOID
EFIAPI
FreeAdddcRegion (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT8   RegionNum
  );

/**
  Get ADDDC region register.

  All output parameters are optional. The caller may pass NULL for parameters
  that it does not need.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  RegionNum       ADDDC region index.
  @param[out] RegionEnable    ADDDC region enable.
  @param[out] FailRank        Failed rank.
  @param[out] FailSubRank     Failed sub-rank.
  @param[out] NonFailRank     Non-failed rank.
  @param[out] NonFailSubRank  Non-failed sub-rank.
  @param[out] RegionSize      Region size.
  @param[out] FailedBank      Failed bank.
  @param[out] NonFailedBank   Non-failed bank.
**/
VOID
EFIAPI
GetAdddcRegion (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT8   RegionNum,
  OUT UINT8   *RegionEnable,    OPTIONAL
  OUT UINT8   *FailRank,        OPTIONAL
  OUT UINT8   *FailSubRank,     OPTIONAL
  OUT UINT8   *NonFailRank,     OPTIONAL
  OUT UINT8   *NonFailSubRank,  OPTIONAL
  OUT UINT8   *RegionSize,      OPTIONAL
  OUT UINT8   *FailedBank,      OPTIONAL
  OUT UINT8   *NonFailedBank    OPTIONAL
  );


/**
  Get Failed device to be mapped for this ADDDC region.


  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  RegionNum       ADDDC region index.
  @param[OUT] Faildevice      Failed device to be mapped out for this adddc region.
**/
UINT8
EFIAPI
GetAdddcRegionFaildevice (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT8   RegionNum
  );


/**
  Set error injection high match register.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Addr            Match address to be set.
**/
VOID
EFIAPI
SetErrorInjMatchHi (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT64  Addr
  );

/**
  Set error injection high mask register.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Mask            Mask to be set.
**/
VOID
EFIAPI
SetErrorInjMaskHi (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT64  Mask
  );


/**
  Set dis_tgr_inj in RANK_BANK_MATCH Register

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.

**/
VOID
EFIAPI
SetDisTgrInj (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt
  );

/**
  Set ADDDC error injection register.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
**/
VOID
EFIAPI
SetAdddcErrorInj (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt
  );

/**
  Get VLS t_ccd_wr value.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.

  @return     VLS t_ccd_wr value.
**/
UINT32
EFIAPI
GetVlsSparingTccdWr (
  IN  UINT8    Skt,
  IN  UINT8    ChOnSkt
  );

/**
  Check if sparing engine is currently doing ADDDC sparing copy.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @retval     TRUE            Doing ADDDC sparing copy.
  @retval     FALSE           Not doing ADDDC sparing copy.
**/
BOOLEAN
EFIAPI
IsAdddcSparing (
  IN  UINT8       Skt,
  IN  UINT8       Mc,
  IN  UINT8       ChOnSkt
  );

/**
  Check if sparing engine is currently doing rank sparing copy.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @retval     TRUE            Doing rank sparing copy.
  @retval     FALSE           Not doing rank sparing copy.
**/
BOOLEAN
EFIAPI
IsRankSparing (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  );

/**
  Check if sparing engine is currently doing SDDC sparing copy.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @retval     TRUE            Doing SDDC sparing copy.
  @retval     FALSE           Not doing SDDC sparing copy.
**/
BOOLEAN
EFIAPI
IsSddcSparing (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  );

/**
  Check if patrol scrub is complete.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @retval     TRUE            Patrol scrub is complete.
  @retval     FALSE           Patrol scrub is not complete.
**/
BOOLEAN
EFIAPI
IsPatrolScrubComplete (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  );

/**
  Get ScrubAddressLo value

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @retval[out]  ScrubAddressLo         Value of ScrubAddrLo register
**/
UINT32
EFIAPI
GetScrubAddressLo (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  );

/**
  Check if mirror mode is enabled.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @retval     TRUE            Mirror mode is enabled.
  @retval     FALSE           Mirror mode is disabled.
**/
BOOLEAN
EFIAPI
IsMirrorEanbled (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  );

/**
  Set corrected error threshold to max.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[out] Rank            Rank index.
**/
VOID
EFIAPI
SetThresholdMax (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT8   Rank
  );

/**
  Disable rank corrected error SMI.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[out] Rank            Rank index.
**/
VOID
EFIAPI
DisableRankCorrError (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT8   Rank
  );

/**
  Enable PCLS region.

  @param[in]  Socket          Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  PclsIndex       Index of PCLS register.
  @param[in]  ChipSelect      Rank ID.
  @param[in]  Cbit            Sub-rank ID.
  @param[in]  Bg              Bank group.
  @param[in]  Ba              Bank address.
  @param[in]  Row             Row.
  @param[in]  Col             Column.
  @param[in]  DataOffset      Offset of nibble to be spared.
**/
VOID
EFIAPI
EnablePcls (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt,
  IN  UINT16  PclsIndex,
  IN  UINT32  ChipSelect,
  IN  UINT32  Cbit,
  IN  UINT32  Bg,
  IN  UINT32  Ba,
  IN  UINT32  Row,
  IN  UINT32  Col,
  IN  UINT32  DataOffset
  );

/**
  Disable (reverse) PCLS region.

  @param[in]  Socket          Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  PclsIndex       Index of PCLS register.
**/
VOID
EFIAPI
DisablePcls (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt,
  IN  UINT16  PclsIndex
  );

/**
  Check if it is duplicate PCLS.

  @param[in]  NodeId          Node index.
  @param[in]  ChOnNode        Channel on node.
  @param[in]  ChipSelect      Rank ID.
  @param[in]  Cbit            Sub-rank ID.
  @param[in]  Bg              Bank group.
  @param[in]  Ba              Bank address.
  @param[in]  Row             Row.
  @param[in]  Col             Column.

  @retval     TRUE            Duplicate PCLS.
  @retval     FALSE           Non-duplicate PCLS.
**/
BOOLEAN
EFIAPI
IsDuplicatePcls (
  IN  UINT8   NodeId,
  IN  UINT8   ChOnNode,
  IN  UINT32  ChipSelect,
  IN  UINT32  Cbit,
  IN  UINT32  Bg,
  IN  UINT32  Ba,
  IN  UINT32  Row,
  IN  UINT32  Col
  );

/**
  Enable eMCA gen 1 CSMI morphing.

  The setting applies to each channel on the MC.

  @param[in]  Socket          Socket index.
  @param[in]  Mc              Memory controller on socket.
**/
VOID
EFIAPI
EnableMcChanCsmiGen1 (
  IN  UINT8   Socket,
  IN  UINT8   Mc
  );

/**
  Check if the patrol scrub engine is in System Address (SA) mode.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @retval     TRUE            The patrol scrub engine is in SA mode.
  @retval     FALSE           The patrol scrub engine is in legacy mode.
**/
BOOLEAN
EFIAPI
IsPatrolScrubSaMode (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  );

/**
  Switch the patrol scrub engine to SA mode or legacy mode.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  LegacyMode      TRUE for legacy mode; FALSE for SA mode.
**/
VOID
EFIAPI
SwitchPatrolScrubMode (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  BOOLEAN LegacyMode
  );

/**
  Set the patrol scrub address.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  PhyAddr         Physical addresss.
**/
VOID
EFIAPI
SetPatrolScrubAddress (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT64  PhyAddr
  );

/**
  Set the patrol scrub legacy (rank) address.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Rank            Rank index.
  @param[in]  RankAddress     Rank address.
**/
VOID
EFIAPI
SetPatrolScrubLegacyAddress (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT8   Rank,
  IN  UINT32  RankAddress
  );

/**
  Check if patrol scrub engine is scrubbing a given target rank (legacy mode).

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  TargetChOnMc    Target channel on MC.
  @param[in]  TargetRank      Target rank.

  @retval     TRUE            Currently scrubbing the target rank.
  @retval     FALSE           Not legacy mode, or not scrubbing the target rank.
**/
BOOLEAN
EFIAPI
IsLegacyScrubbingRank (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT8   TargetChOnMc,
  IN  UINT8   TargetRank
  );

VOID
EFIAPI
UpdateCurrentTad (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT64  Systemaddress,
  IN  BOOLEAN Is2lmMode
  );

/**
  Set Scrub control register TAD rule.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Is2lmMode       TRUE if 2LM mode; FALSE if not.
**/
VOID
EFIAPI
SetScrubCtrlTadRule (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  BOOLEAN Is2lmMode
  );

/**
  Set sparing control register TAD rule.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Is2lmMode       TRUE if 2LM mode; FALSE if not.
**/
VOID
EFIAPI
SetSparingCtrlTadRule (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  BOOLEAN Is2lmMode
  );

/**
  Set AMAP register for legacy patrol scrub engine.

  Spare rank is optional; if 0xff is passed in, the field is not updated.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  SpareRank       Rank index to scrub.
**/
VOID
EFIAPI
SetAmapForLegacyPatrol (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT8   SpareRank       OPTIONAL
  );

/**
  Set AMAP register after patrol scrub legacy mode.

  Skipped rank is optional; if 0xff is passed in, the field is not updated.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  AmapData        Original AMAP register value.
  @param[in]  SpareRank       Rank index to scrub.
  @param[in]  SkppedRank      Rank index to be skipped by patrol scrub.
**/
VOID
EFIAPI
SetAmapFromLegacyPatrol (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT32  AmapData,
  IN  UINT8   SpareRank,
  IN  UINT8   SkppedRank      OPTIONAL
  );

/**
  Check if sparing engine is currently doing sparing copy.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @retval     TRUE            Doing sparing copy.
  @retval     FALSE           Not doing sparing copy.
**/
BOOLEAN
EFIAPI
IsSparingEnable (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  );

/**
  Check if sparing engine currently has enable virtual lock step enabled.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @retval     TRUE            Virtual lockstep enabled.
  @retval     FALSE           Virtual lockstep disabled.
**/
BOOLEAN
EFIAPI
IsVirtualLockStepEnable (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  );

/**
  Configure Spare Interval based on the bw_limit_tf.

  @param[in] Socket  - Socket number
  @param[in] Mc      - Mc number in the Socket.
  @param[in] SktChBitMap - Socket channel bit map.

  @retval none
**/
VOID
SetSpareInterval (
  UINT8   Socket,
  UINT8   Mc,
  UINT16  SktChBitMap
  );

/**
  Set sparing control parameters.

  Virtual lockstep, reverse, mirror, and partial mirror are optional;
  if 0xff is passed in, the correpsonding field is not updated.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  RankSparing     Rank sparing flag.
  @param[in]  AdddcSparing    ADDDC sparing flag
  @param[in]  SddcSparing     SDDC Sparing flag
  @param[in]  RegionSize      Sparing region size
  @param[in]  VirtualLockStep Virtual lockstep flag.
  @param[in]  Reverse         Reverse flag.
  @param[in]  Mirror          Mirror flag.
  @param[in]  PartialMirror   Partial mirror flag.
**/
VOID
EFIAPI
SetSparingCtrl (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  BOOLEAN RankSparing,
  IN  BOOLEAN AdddcSparing,
  IN  BOOLEAN SddcSparing,
  IN  UINT8   RegionSize,
  IN  BOOLEAN SpareEnable,
  IN  UINT8   VirtualLockStep, OPTIONAL
  IN  UINT8   Reverse,        OPTIONAL
  IN  UINT8   Mirror,         OPTIONAL
  IN  UINT8   PartialMirror   OPTIONAL
  );

/**
  Clear the 'reverse' bit in sparing control register.

  @param[in]  Socket          Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
**/
VOID
EFIAPI
ClearReverseBit (
  IN  UINT8   Socket,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  );

/**
  Check if VLS is triggered.

  @param[in]  Socket          Socket index.
  @param[in]  Mc              Memory controller on socket.

  @retval     TRUE            VLS is triggered.
  @retval     FALSE           VLS is not triggered.
**/
BOOLEAN
EFIAPI
CheckVlsTriggered (
  IN  UINT8   Socket,
  IN  UINT8   Mc
  );

/**
  Set sparing control source register.

  Sub-rank, bank, buddy CS, and buddy CS enable are optional;
  if 0xff is passed in, the correpsonding field is not updated.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Rank            Rank index to set.
  @param[in]  SubRank         Sub-rank index to set.
  @param[in]  Bank            Bank group in [3:2], index in group in [1:0].
  @param[in]  BuddyCs         Buddy rank.
  @param[in]  BuddyCsEn       Buddy rank enable.
**/
VOID
EFIAPI
SetSparingCtlSrc (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT8   Rank,
  IN  UINT8   SubRank,        OPTIONAL
  IN  UINT8   Bank,           OPTIONAL
  IN  UINT8   BuddyCs,        OPTIONAL
  IN  UINT8   BuddyCsEn       OPTIONAL
  );

/**
  Set sparing control destination register.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Rank            Destination Rank for sparing copy.
**/
VOID
EFIAPI
SetSparingCtlDest (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT8   Rank
  );

/**
  Set spare_rank_diff value of CMPL_TO_DATA_DELAY.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  SpareRankDiff   Spare rank different value to set.
**/
VOID
EFIAPI
SetSpareRankDiff (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT8   SpareRankDiff
  );

/**
  Get link fail value of scrub mask.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @return     Current link fail value.
**/
UINT8
EFIAPI
GetLinkFail (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  );

/**
  Set link fail value of scrub mask.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  LinkFail        Link fail value to set.
**/
VOID
EFIAPI
SetLinkFail (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT8   LinkFail
  );

/**
  Get scrub enable.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @return     Current scrub enable value.
 */
UINT8
EFIAPI
GetScrubEnable (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  );

/**
  Set scrub enable.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  ScrubEn         Scrub enable value to set.
 */
VOID
EFIAPI
SetScrubEnable (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT8   ScrubEn
  );

/**
  Get scrubCtl Address.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @return     The address of the ScrubCtl Register.
 */
UINT64
EFIAPI
GetScrubCtlAddress (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  );

/**
  Get scrub enable.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @return     Current scrub ctl value.
 */
UINT32
EFIAPI
GetScrubCtl (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  );

/**
  Get scrub enable.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Data            Data input value to be written into scrubctl

  @return   None
 */
VOID
EFIAPI
SetScrubCtl (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT32  Data
  );

/**
  Get scrub ctrl register information required for patrol.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[out]  *PtlCmpl,      return value PtlCmpl OPTIONAL
  @param[out]  *PtlStopped,   return value PtlStopped OPTIONAL
  @param[out]  *ScrubIssued,  return value ScrubIssued OPTIONAL

  @return  NONE
 */
VOID
EFIAPI
GetScrubCtlForPatrol (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  OUT UINT32  *PtlCmpl,         OPTIONAL
  OUT UINT32  *PtlStopped,       OPTIONAL
  OUT UINT32  *ScrubIssued       OPTIONAL
  );

/**
  Set scrub ctrl register for patrol.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  StartScrub,     value to program startscrub OPTIONAL
  @param[in]  StopOnErr,      value to program stop_on_err OPTIONAL
  @param[in]  PtlCmpl,        value to program ptl_cmpl OPTIONAL
  @param[in]  StopOnCmpl,     value to program stop_on_cmpl OPTIONAL
  @param[in]  PtlStopped      value to program ptl_stopped OPTIONAL
  @param[in]  IssueOnce       value to program issueonce OPTIONAL
  @param[in]  StopOnRank      value to program stop_on_rank OPTIONAL

  @return    NONE

 */
VOID
EFIAPI
SetScrubCtlForPatrol (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN UINT32   StartScrub,      OPTIONAL
  IN UINT32   StopOnErr,       OPTIONAL
  IN UINT32   PtlCmpl,         OPTIONAL
  IN UINT32   StopOnCmpl,      OPTIONAL
  IN UINT32   IssueOnce,       OPTIONAL
  IN UINT32   StopOnRank       OPTIONAL
  );

/**
  Set MCMTR register.

  ADDDC enable and CTL delay are optional; if 0xff is passed in, the
  correpsonding field is not updated.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  AdddcEn         ADDDC enable.
  @param[in]  CtlDelay        CTL delay enable.
**/
VOID
EFIAPI
SetMcmtr (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT8   AdddcEn,        OPTIONAL
  IN  UINT8   CtlDelay        OPTIONAL
  );

/**
  Get MCMTR register values.

  This routine returns close_page, bank_xor_enable, ddr_xor_mode_enable, ddr_half_xor_mode_enable bit field values

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[out]  ClosePg        close_page value return  OPTIONAL Pass NULL if not needed
  @param[out]  BankXorEn      bank_xor_enable value return   OPTIONAL Pass NULL if not needed
  @param[out]  ChXorModeEn          ddr_xor_mode_enable value return   OPTIONAL Pass NULL if not needed
  @param[out]  ClusterXorModeEn      ddr_half_xor_mode_enable value return   OPTIONAL Pass NULL if not needed
**/
VOID
EFIAPI
GetMcmtr (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  OUT  UINT32   *ClosePg,              OPTIONAL
  OUT  UINT32   *BankXorEn,            OPTIONAL
  OUT  UINT32   *ChXorModeEn,          OPTIONAL
  OUT  UINT32   *ClusterXorModeEn      OPTIONAL
  );

/**
  Configure DDRT viral.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
**/
VOID
EFIAPI
DdrtViralConfig (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt
  );

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
  );

/**
  Enable Interrupt for DDRT Alerts

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
**/
VOID
EFIAPI
EnableInterruptForDdrtAlerts (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt
  );

/**
  Configure Retry Register for Error logging

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
**/
VOID
EFIAPI
ConfigRetryRegister (
  IN    UINT8      Socket,
  IN    UINT8      ChOnSkt
  );

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
  );

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
);


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
  );

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
  );

/**
  Workaround to downgrade patrol scrubbing, correctable -> uncorreactable errors for this TAD range

  @param[in]  Socket          - Socket number
  @param[in]  Mc              - Mc number

  @retval EFI_SUCCESS  if successful
  @retval EFI_UNSUPPORTED if unsuccessful

**/
UINT8
EFIAPI
TadRestorePsSeverity (
  UINT8 Socket,
  UINT8 Mc
  );

/**
  Is this current TAD mirrored

  @param[in]  Socket          - Socket number
  @param[in]  Mc              - Mc number
  @param[in]  ChOnSkt         - Ch on socket number
  @param[in]  TadIndex        - Tad Index

  @retval TRUE  if TAD is mirrored
  @retval FALSE if TAD is not mirrored

**/
BOOLEAN
EFIAPI
IsTadMirrored (
  UINT8 Socket,
  UINT8 Mc,
  UINT8 ChOnSkt,
  UINT8 TadIndex
  );

/**
  Is this current TAD valid

  @param[in]  Socket          - Socket number
  @param[in]  Mc              - Mc number
  @param[in]  ChOnSkt         - Ch on socket number
  @param[in]  TadIndex        - Tad Index

  @retval TRUE  if TAD is Valid
  @retval FALSE if TAD is not Valid

**/
BOOLEAN
EFIAPI
IsTadValid (
  UINT8 Socket,
  UINT8 Mc,
  UINT8 ChOnSkt,
  UINT8 TadIndex
  );

/**
  Set error threshold value for per rank threshold register to the input threhsold value

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Rank            Rank number
  @param[in]  ErrThreshold    Threshold value to be programmed into the per rank threshold register

  @retval N/A

**/
VOID
EFIAPI
SetErrorThresholdPerRank (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT8   Rank,
  IN  UINT32  ErrThrshld
  );

/**
  Check if imc_fuse_download.mc_mca_recovery is enabled.

  @retval     TRUE            mca_recovery is enabled.
  @retval     FALSE           mca_recovery is disabled.
**/
BOOLEAN
EFIAPI
CheckMcMcaRecovery (
  VOID
  );

/*Get SMISPARECTL register value.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @return   sparectl register value
*/
UINT32
EFIAPI
GetSmiSpareCtl (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  );

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
  );

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
  );

/**
  Get sparing patrol status register.

  @param[in]  Skt       Socket index.
  @param[in]  Mc        Memory controller on socket.
  @param[in]  ChOnSkt   Channel on socket.

  @return               Sparing patrol status value
**/
UINT32
EFIAPI
GetSparingPatrolStatus (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  );

/**
  Clear Patrol complete status

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @return  NONE
 */
VOID
EFIAPI
ClearPatrolStatus (
    IN  UINT8   Skt,
    IN  UINT8   Mc,
    IN  UINT8   ChOnSkt
  );

/**
  Clear spare copy status

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @return  NONE
 */
VOID
EFIAPI
ClearSpareCopyStatus (
    IN  UINT8   Skt,
    IN  UINT8   Mc,
    IN  UINT8   ChOnSkt
  );

/**
  Configure patrol error log disable feature

  @param[in]  Skt              Socket index.
  @param[in]  Mc               Memory controller on socket.
  @param[in]  ChOnSkt          Channel on socket.
  @param[in]  Dis              True -  disable patrol error log
                               False - enable patrol error log

  @return  NONE
**/
VOID
EFIAPI
SetPatrolErrLogDisable (
  IN  UINT8     Skt,
  IN  UINT8     Mc,
  IN  UINT8     ChOnSkt,
  IN  BOOLEAN   Dis
  );

/**
  Clear Fnv viral received flag.

  Fnv_viral_received indicates viral is seen in a FNV read packet. This bit is sticky and will not be cleared by a warm reset.
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
  );

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
  );

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
  );

/**
  Set threshold for number of times UC error is retried by FMC

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  CntThreshold    threshold value to be programmed.

  @retval  None
**/
VOID
EFIAPI
SetDdrtRetryUcErrorCounterThreshold (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT32  CntThreshold
  );

/**
  Set threshold for number of retries performed on a Corrected error before attempting to correct with ECC
  This is applicable in ECC mode A operation

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket
  @param[in]  CntThreshold    threshold value to be programmed.
  @retval  None
**/
VOID
EFIAPI
SetDdrtEccModeAThreshold (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT32  CntThreshold
  );


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
  );

/**
  Set threshold for number of retries performed on a Corrected error before attempting to correct with ECC
  This is applicable in ECC mode A operation

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket
  @param[in]  ModeInfo        Mode A vs Mode B values to be programmed
  @retval  None
**/
VOID
EFIAPI
SetDdrtEccMode (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT32  ModeInfo
  );

/**
  Get DDRT ECC Mode information for this channel

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket

  @return  Ddrteccmode
**/

UINT32
EFIAPI
GetDdrtEccMode (
  IN   UINT8   Skt,
  IN   UINT8   ChOnSkt
  );

/**
  Set Ddrtdata path delay to failed nibble and enable force correction

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket
  @param[in]  FailedNibble    Failed nibble/device to be programmed
  @param[in]  ForceCorrection value to be programmed into ddrt_force_correction
  @retval     None
**/
VOID
EFIAPI
SetDdrtDataPathDelay (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT8   FailedNibble,       OPTIONAL
  IN  UINT8   ForceCorrection     OPTIONAL
  );

/**
  Program DDRTCnstrnt.ddr_wrwr_s with value passed

  @param[in]  Socket                   - Socket ID
  @param[in]  ChOnSkt                  - Channel on socket
  @param[in]   DdrtWrWrS                - value to prgram in t_ddrt_wrwr_s

**/
VOID
EFIAPI
SetDdrtCnsTrnt (
  IN   UINT8    Socket,
  IN   UINT8    ChOnSkt,
  IN   UINT32   DdrtWrWrS
  );

/**
  Enable VmseMcaCtl Err0

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket

  @retval  N/A
**/
VOID
EFIAPI
EnableVmseMcaCtlErr0 (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt
  );


/**
  Check if VmseMcaCtl Err0 is Enabled

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket

  @retval TRUE  if VmseMcaCtl Err0 is Enabled
  @retval FALSE if VmseMcaCtl Err0 is Disabled
**/
BOOLEAN
EFIAPI
IsVmseMcaCtlErr0Enabled (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt
  );


/**
  Clear VmseMcaCtl Err0

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket

  @retval N/A
**/
VOID
EFIAPI
ClearVmseMcaCtlErr0Log (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt
  );


/**
Update TadBaseForMirrorFailover register

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  FailedCh        Failed Channel
  @param[in]  TadIndex        Tad Index

  @retval N/A
**/
VOID
EFIAPI
UpdateTadBaseForMirrorFailover (
  IN  UINT8 Socket,
  IN  UINT8 ChOnSkt,
  IN  UINT8 FailedCh,
  IN  UINT8 TadIndex
  );


/**
Disable LinkFail

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket

  @retval N/A
**/
VOID
EFIAPI
DisableLinkFail (
  IN  UINT8 Socket,
  IN  UINT8 ChOnSkt
  );

/**
Get EFIAPILinkErrFsm Current State

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket

  @retval Current State of LinkErrFsmState
**/
UINT32
EFIAPI
GetLinkErrFsmCurrentState (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt
  );

/**
Get DdrtRetryFsm Current State

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket

  @retval Current State of DdrtRetryFsm
**/
UINT32
EFIAPI
GetDdrtRetryFsmCurrentState (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt
  );

/**
Adjust DDRT RD Credit Value

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket
  @param[in]  RdCreditValue   RD Credit Value

  @retval N/A
**/
VOID
EFIAPI
AdjustDdrtRdCreditValue (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt,
  IN  UINT32  RdCreditValue
  );

/**
Update Scrub Ctl value and Return Previous value

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket
  @param[in]  ScrubEn         ScrubEn Value

  @retval ScrubEnSave
**/
UINT32
EFIAPI
UpdateScrubCtl (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt,
  IN  UINT32  ScrubEn
  );

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
  );

/**
  Check whether the ADDDC meta bit field is set/clear

  @param[in]  Socket                   - Socket ID
  @param[in]  ChOnSkt                  - Channel on socket

  @param[out] TRUE/FALSE               - Check whether the ADDDC meta bit field is set/clear
**/
BOOLEAN
EFIAPI
CheckADDDCMetaBit (
  IN  UINT8   Socket,
  IN  UINT8   SktCh
  );

/**
  Set error counter value for the rank

  @param[in]  Socket          Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Rank            Rank number
  @param[in]  ErrorCount      ErrorCount value to be programmed.

  @retval None

**/
VOID
SetErrorCounterPerRank (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt,
  IN  UINT8   Rank,
  IN  UINT16  ErrorCount
  );

#endif // _MEM_MC_IP_RAS_LIB_H_
