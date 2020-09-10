/** @file
  Implementation of Amap decode.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include "AddressDecodeInternal.h"
#include "Mirror.h"
#include <Library/AddressDecodeLib.h>
#include <Library/RasRuntimeStatusLib.h>

extern AD_SOCKET_INFO *mSadTadScope;


 /**

 Check if mirror failover happend.

 @param[in]    Socket    --   socket index
 @param[in]    Mc        --   MC index
 @param[in]    Channel   --   Channel index

 @retval TRUE/FALSE      -- Return TRUE if failover. Otherwise FALSE.


**/
BOOLEAN
EFIAPI
IsMirrorFailover (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Ch
  )
{
  UINT8     FailedCh;

  FailedCh = GetMirrorFailoverFailedChannel(Socket, Mc);

  if (FailedCh == Ch) {
    DEBUG ((EFI_D_ERROR, "IsMirrorFailover TRUE\n"));
    return TRUE;
  }

  return FALSE;
}

/**
 This is routine to get primary channel address if mirror

 @param[in]    Sa               --   System address
 @param[in]    Socket           --   socket index
 @param[in]    Mc               --   MC index
 @param[in]    ChannelAddress   --   Channel address

 @ret  return primary channel address if mirror,  return channel address if not mirror, or -1 in error
**/
UINT64
EFIAPI
MirrorPriCa (
  IN      UINT64    Sa,
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT64    ChannelAddress
  )
{
  UINT8     Sad2TadIndex;

  Sad2TadIndex = GetSad2TadIndexFromSa(Sa, Socket, Mc);
  if (Sad2TadIndex == (UINT8) -1) {
    DEBUG ((EFI_D_ERROR, "[MirrorPriCa] invalid Sa 0x%llx skt:%d  Mc:%d\n", Sa, Socket, Mc));
    return (UINT64) -1;
  }

  if (mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Mirror) {
    //
    // MirrorFailover == 1, means mirror failover happend and the secondary channel is in use.
    // MirrorFailover == 0, means the primary channel is in use.
    // BIT19 = BIT18 + 1 PCH bit
    //
    ChannelAddress = BitFieldInsert64(ChannelAddress, 19, 19, 0);
    DEBUG ((EFI_D_ERROR, "[MirrorPriCa] mirror support, insert BIT19[0] to CA, CA 0x%lx\n", ChannelAddress));
  }

  return ChannelAddress;
}

/**
 This function is called by TranslateMc when ddr act as FM or 1LM
 This function is to get secondary channel and dimm info if mirror

 @param  [in/out] TA  -- pointer to the structure containing DIMM Address

 @retval EFI_SUCCESS / Error code
**/
EFI_STATUS
EFIAPI
TranslateMirror (
  IN OUT PTRANSLATED_ADDRESS    TA
  )
{
  UINT8     Sad2TadIndex;
  UINT8     Socket, Mc, Ch, SecCh, SecRank, SecSubRank, SecBank;
  UINT32    SecRow, SecColumn;
  UINT64    Sa, SecCa, SecRa;

  if ((TA->Flag & (SOCKET_VALID | MC_VALID | CHN_VALID | CHN_ADDR_VALID)) == 0) {
    return EFI_INVALID_PARAMETER;
  }
  Socket = TA->SocketId;
  Mc     = TA->MemoryControllerId;
  Ch     = TA->ChannelId;
  Sa     = TA->SystemAddress;

  Sad2TadIndex = GetSad2TadIndexFromSa(TA->SystemAddress, Socket, Mc);
  if (Sad2TadIndex == (UINT8) -1) {
    DEBUG ((EFI_D_ERROR, "[TranslateMirror] invalid Sa 0x%llx skt:%d  Mc:%d\n", Sa, Socket, Mc));
    return EFI_INVALID_PARAMETER;
  }

  if (!mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Mirror) {
    return EFI_SUCCESS;
  }

  DEBUG ((EFI_D_ERROR, "\n[TranslateMirror] Starting translate mirror\n"));
  SecCh = mSadTadScope[Socket].McInfo[Mc].ChInfo[Ch].SecCh;
  SecCa = Pri2SecCa(TA->ChannelAddress);
  TA->SecChannelId = SecCh;
  TA->SecChannelAddress = SecCa;

  ForwardRirDecode (Sa, Socket, Mc, SecCh, SecCa, &SecRank, &SecRa);
  ForwardAmapDecode (SecRa, Socket, Mc, SecCh, SecRank >> 2, &SecSubRank, &SecBank, &SecRow, &SecColumn);

  TA->SecRankAddress = SecRa;
  TA->SecChipSelect = SecRank;
  TA->SecPhysicalRankId = (SecRank) % MAX_RANK_DIMM;
  TA->SecDimmSlot = SecRank >> 2;

  DEBUG ((EFI_D_ERROR, "[TranslateMirror] Sec RankAddress:0x%lx   ChipSelect:0x%x   PhysicalRankId:0x%x   DimmSlot:0x%x\n",
  TA->SecRankAddress,TA->SecChipSelect, TA->SecPhysicalRankId, TA->SecDimmSlot));


  TA->SecCol = SecColumn;
  TA->SecRow = SecRow;
  TA->SecBank = SecBank & 3;
  TA->SecBankGroup = SecBank >> 2;
  DEBUG ((EFI_D_ERROR, "[TranslateMirror] Sec   Col:0x%x   Row:0x%x    BankGroup:0x%x   Bank:0x%x\n",
  TA->SecCol, TA->SecRow, TA->SecBankGroup, TA->SecBank));

  if (SecSubRank != 0xff) {
    TA->SecChipId = SecSubRank;
    DEBUG ((EFI_D_ERROR, "[TranslateMirror] Sec   TA->ChipId  is 0x%x\n", TA->SecChipId));
  }

  TA->RasModesApplied |= BIT3;
  return EFI_SUCCESS;
}


/**
 This function is to move secondary info to primary.

 @param  [in/out] TA  -- pointer to the structure containing DIMM Address

 @retval EFI_SUCCESS / Error code
**/
VOID
EFIAPI
MoveSec2Pri (
  IN OUT PTRANSLATED_ADDRESS    TA
  )
{
  TA->ChannelId          = TA->SecChannelId;
  TA->SecChannelId       = (UINT8)-1;
  TA->ChannelAddress     = TA->SecChannelAddress;
  TA->SecChannelAddress  = (UINT64)-1;
  TA->RankAddress        = TA->SecRankAddress;
  TA->SecRankAddress     = (UINT64)-1;
  TA->DimmSlot           = TA->SecDimmSlot;
  TA->SecDimmSlot        = (UINT8)-1;
  TA->PhysicalRankId     = TA->SecPhysicalRankId;
  TA->SecPhysicalRankId  = (UINT8)-1;
  TA->Row                = TA->SecRow;
  TA->SecRow             = (UINT32)-1;
  TA->Col                = TA->SecCol;
  TA->SecCol             = (UINT32)-1;
  TA->Bank               = TA->SecBank;
  TA->SecBank            = (UINT8)-1;
  TA->BankGroup          = TA->SecBankGroup;
  TA->SecBankGroup       = (UINT8)-1;
  TA->ChipSelect         = TA->SecChipSelect;
  TA->SecChipSelect      = (UINT8)-1;
  TA->ChipId             = TA->SecChipId;
  TA->SecChipId          = (UINT8)-1;
}
