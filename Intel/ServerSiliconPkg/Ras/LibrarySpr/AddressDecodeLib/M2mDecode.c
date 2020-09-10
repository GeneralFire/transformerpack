/** @file
  Implementation of M2M SAD2TAD decode.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <IioUniversalData.h>
#include <RcRegs.h>
#include <Library/CpuCsrAccessLib.h>
#include "AddressDecodeInternal.h"

extern AD_SOCKET_INFO              *mSadTadScope;





/**
 This routine is to get sad2tad info

 @param[in]    Socket       --   socket index
 @param[in]    Mc           --   mc index
 @param[in]    Sad2TadIndex -- Sad2Tad index
 @param[in]    Limit        --   address limit of this sad2tad
 @param[in]    ChTad        -- tad index on the channel.
 @param[in]    Ddr4Flag     -- ddr4 flag
 @param[in]    NmCacheableVld  -- Nm cacheable valid
 @param[in]    BlkVld       --  block valid
 @param[in]    PmemVld      -- persistent memory valid
 @param[in]    Mirror       -- mirror flag

 @retval       TRUE-- this sad2tad is valid.

**/
BOOLEAN
EFIAPI
GetSad2Tad (
  IN      UINT8  Socket,
  IN      UINT8  Mc,
  IN      UINT8  Sad2TadIndex,
  IN OUT  UINT64 *Limit,
  IN OUT  UINT8  *ChTad,
  IN OUT  UINT8  *Ddr4Flag,
  IN OUT  UINT8  *NmCacheableVld,
  IN OUT  UINT8  *BlkVld,
  IN OUT  UINT8  *PmemVld,
  IN OUT  BOOLEAN  *Mirror
  )
{
  TAD_RD_N0_M2MEM_MAIN_STRUCT                       MeshN0TadRd;
  TAD_RD_N1_M2MEM_MAIN_STRUCT                       MeshN1TadRd;
  UINT64                                            MeshData;

  MeshN0TadRd.Data = 0;
  MeshN0TadRd.Bits.tadid = Sad2TadIndex;
  MeshN0TadRd.Bits.tadrden = 1;

  WriteCpuCsr (Socket, Mc, TAD_RD_N0_M2MEM_MAIN_REG, MeshN0TadRd.Data);
  MeshN0TadRd.Data = ReadCpuCsr (Socket, Mc, TAD_RD_N0_M2MEM_MAIN_REG);
  MeshN1TadRd.Data = ReadCpuCsr (Socket, Mc, TAD_RD_N1_M2MEM_MAIN_REG);

  if (MeshN0TadRd.Bits.tadid == Sad2TadIndex &&
      MeshN0TadRd.Bits.tadvld == 1) {
    MeshData = LShiftU64 (MeshN1TadRd.Data, 32) | MeshN0TadRd.Data;

    if (Limit != NULL) {
      *Limit = LShiftU64(BitFieldRead64 (MeshData, 26, 51) + 1, SAD_UNIT) -1;
    }

#ifndef GNRSRF_HOST
    //
    // To check if the following bitfields have moved or obsolete for GNR/SRF
    //
    if (ChTad != NULL) {
      *ChTad = (UINT8)MeshN0TadRd.Bits.ddrtadid;
    }

    if (Ddr4Flag != NULL) {
      *Ddr4Flag = (UINT8)MeshN0TadRd.Bits.ddr4;
    }
#endif // ! GNRSRF_HOST

    if (NmCacheableVld != NULL) {
      *NmCacheableVld = (UINT8)MeshN0TadRd.Bits.nmcacheablevld;
    }

    if (BlkVld != NULL) {
      *BlkVld = (UINT8)MeshN0TadRd.Bits.blkvld;
    }

#ifndef GNRSRF_HOST
    //
    // To check if the following bitfield has moved or obsolete for GNR/SRF
    //
    if (PmemVld != NULL) {
      *PmemVld = (UINT8)MeshN0TadRd.Bits.pmemvld;
    }
#endif // !GNRSRF_HOST

    if (Mirror != NULL) {
      *Mirror = (BOOLEAN)MeshN0TadRd.Bits.mirror;
    }

    return TRUE;
  } else {
    return FALSE;
  }

}


/**
 This routine is to get whether the target is DDRT RIR or DDR RIR.

 Note:Caller code should ensure to invoke this routine at normal TAD flow as above case only exist at NM TAD flow.

 @param[in]    Socket       --   socket index
 @param[in]    Mc           --   mc index
 @param[in]    Sad2TadIndex --   sad2tad index


 @retval       TRUE -- this is for DDRT RIR.

**/
BOOLEAN
EFIAPI
GetDdrFlag (
  IN    UINT8    Socket,
  IN    UINT8    Mc,
  IN    UINT8    Sad2TadIndex
  )
{
  BOOLEAN    Flag = FALSE;

  if(Sad2TadIndex >= MAX_SAD2TAD){
    return Flag;
  }

  if (IsHbmDimm (Socket, Mc)== FALSE) {
    if ((mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].BlkVld == 1)
      ||  (mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].PmemVld == 1)
      ||  ((mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Ddr4Flag == 0) &&
      (mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].NmCacheableVld == 1))) {
        Flag = TRUE;
      }

  }

  return Flag;
}

/**
 This routine is to check whether the SA locate in specified M2M tad.

 @param[in]    Socket       --   socket index
 @param[in]    Mc           --   mc index
 @param[in]    Sad2TadIndex --   sad2tad index

 @retval       TRUE -- it is located.

**/
BOOLEAN
EFIAPI
IsM2mTadRange (
  IN    UINT64     Sa,
  IN    UINT8      Socket,
  IN    UINT8      Mc,
  IN    UINT8      Sad2TadIndex
  )
{
  if ((Sa >= mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Base) &&
        (Sa <= mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Limit)) {
    return TRUE;
  } else {
    return FALSE;
  }

}

/**
 This routine is to know whether this M2M tad entry belong to block region.


 @param[in]    Socket       --   socket index
 @param[in]    Mc           --   mc index
 @param[in]    Sad2TadIndex --   sad2tad index


 @retval       TRUE -- this is block region.

**/
BOOLEAN
EFIAPI
IsLocateBlockMode (
  IN    UINT8    Socket,
  IN    UINT8    Mc,
  IN    UINT8    Sad2TadIndex
  )
{
  if (mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].BlkVld == 1) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
 This routine is to get channel Tad index.

 @param[in]    Sa           --   System address
 @param[in]    Socket       --   socket index
 @param[in]    Mc           --   mc index



 @retval       Ch Tad index.

**/
UINT8
EFIAPI
GetChTad (
  IN     UINT64     Sa,
  IN     UINT64     Socket,
  IN     UINT64     Mc
  )
{
  UINT8     Sad2TadIndex;

  for (Sad2TadIndex = 0; Sad2TadIndex < MAX_SAD2TAD; Sad2TadIndex ++) {
    if (mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Sad2TadValid == TRUE) {
      if ((Sa >= mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Base) &&
        (Sa <= mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Limit)) {

          return mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].ChTad;
        }
    }
  }

  DEBUG ((EFI_D_ERROR, "[GetChTad] not able to find the right Ch tad -- Socket:%d  Mc:%d \n", Socket, Mc));
  return 0xff;
}

/**
 This routine is to get SAD2TAD index.

 @param[in]    Sa           --   System address
 @param[in]    Socket       --   socket index
 @param[in]    Mc           --   mc index



 @retval       SAD2Tad index.

**/
UINT8
EFIAPI
GetSad2TadIndexFromSa (
  IN     UINT64     Sa,
  IN     UINT64     Socket,
  IN     UINT64     Mc
  )
{
  UINT8     Sad2TadIndex;

  for (Sad2TadIndex = 0; Sad2TadIndex < MAX_SAD2TAD; Sad2TadIndex ++) {
    if (mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Sad2TadValid == TRUE) {
      if ((Sa >= mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Base) &&
        (Sa <= mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Limit)) {

          return Sad2TadIndex;
        }
    }
  }

  DEBUG ((EFI_D_ERROR, "[GetSad2TadIndexFromSa] not able to find the right Sad2TadIndex -- Socket:%d  Mc:%d \n", Socket, Mc));
  return 0xff;
}

/**
 This routine is to initialize M2M mirror channel info.

 @param[in]    Socket    --   socket index
 @param[in]    Mc        --   memory controller index

 @retval       none.

**/
VOID
EFIAPI
InitM2mMirrorCh (
  IN     UINT8       Socket,
  IN     UINT8       Mc
  )
{
  UINT8       Channel;
  MIRRORCHNLS_M2MEM_MAIN_STRUCT    MirrorChannels;

  MirrorChannels.Data = ReadCpuCsr (Socket, Mc, MIRRORCHNLS_M2MEM_MAIN_REG);

  for (Channel = 0; Channel < MAX_CH_PER_MC; Channel ++) {
    if (IsChannelExist (Socket, Mc, Channel) == FALSE) {
      continue;
    }

    if (Channel == 0) {
      mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].SecCh = (UINT8) MirrorChannels.Bits.ddr4chnl0secondary;
    }
    if (Channel == 1) {
      mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].SecCh = (UINT8) MirrorChannels.Bits.ddr4chnl1secondary;
    }
    if (Channel == 2) {
      mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].SecCh = (UINT8) MirrorChannels.Bits.ddr4chnl2secondary;
    }

    if (Channel == MirrorChannels.Bits.ddr4chnl0secondary) {
      mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].PriCh = 0;
    }
    if (Channel == MirrorChannels.Bits.ddr4chnl1secondary) {
      mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].PriCh = 1;
    }
    if (Channel == MirrorChannels.Bits.ddr4chnl2secondary) {
      mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].PriCh = 2;
    }
  }
}
/**
 This routine is to initialize M2M sad2tad.

 @param[in]    Socket    --   socket index

 @retval       none.

**/
VOID
EFIAPI
InitM2mSadTad (
  IN     UINT8       Socket
  )
{
  UINT8     Mc;
  UINT8     Sad2TadIndex;
  UINT64    Limit;
  UINT64    Base = 0;
  UINT8     ChTad;
  UINT8     Ddr4Flag;
  UINT8     NmCacheableVld;
  UINT8     BlkVld;
  UINT8     PmemVld;
  BOOLEAN   Mirror;

  for (Mc = 0; Mc < MAX_SYS_IMC; Mc ++) {
    if(IsMcExist (Socket, Mc) == FALSE) {
      continue;
    }

    for (Sad2TadIndex = 0; Sad2TadIndex < MAX_SAD2TAD; Sad2TadIndex ++) {

      if (Sad2TadIndex == 0) {
        Base = 0;
      }

      if (GetSad2Tad (Socket, Mc, Sad2TadIndex, &Limit, &ChTad, &Ddr4Flag, &NmCacheableVld, &BlkVld, &PmemVld, &Mirror)) {
        mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Sad2TadValid = TRUE;
        mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Base = Base;
        mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Limit = Limit;
        mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].ChTad = ChTad;
        mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Ddr4Flag = Ddr4Flag;
        mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].NmCacheableVld = NmCacheableVld;
        mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].BlkVld = BlkVld;
        mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].PmemVld = PmemVld;
        mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Mirror = Mirror;

        if (Mirror) {
          InitM2mMirrorCh(Socket, Mc);
        }

        DEBUG ((EFI_D_INFO, " [M2mTad] Skt:%d Mc:%d Idx:%d Base:%013lx Limit:%013lx ChTad:%d Ddr4Flag:%d NmCacheableVld:%d, Mirror:%d\n",
             Socket, Mc, Sad2TadIndex, Base, Limit, ChTad, Ddr4Flag, NmCacheableVld, Mirror));
        Base = Limit + 1;
      }

    }
  }
}
