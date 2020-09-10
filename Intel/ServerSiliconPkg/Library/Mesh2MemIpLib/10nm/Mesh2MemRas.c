/** @file
  Mesh2Mem RAS support Module.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation. <BR>

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

//
// Include files
//

#include "UncoreCommonIncludes.h"
#include <Library/Mesh2MemIpLib.h>
#include "Include/Mesh2MemRegs.h"
#include <Library/SysHostPointerLib.h>
#include <Library/KtiApi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/CteNetLib.h>

/**

  Get Mesh2Mem field Value for Mode register - NmCaching.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
BOOLEAN
EFIAPI
GetM2mModeNmCaching (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  MODE_M2MEM_MAIN_STRUCT                MeshCrMode;

  MeshCrMode.Data = UsraCsrRead (Socket, McId, MODE_M2MEM_MAIN_REG);
  return (BOOLEAN)MeshCrMode.Bits.nmcaching;
}

/**

  Get Mesh2Mem field Value for SysFeatures0 - ImFailoverEn.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
BOOLEAN
EFIAPI
GetM2mSysFeatures0ImFailoverEn (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  SYSFEATURES0_M2MEM_MAIN_STRUCT   SysFeatures0;

  SysFeatures0.Data = UsraCsrRead ( Socket, McId, SYSFEATURES0_M2MEM_MAIN_REG);
  return (BOOLEAN)SysFeatures0.Bits.immediatefailoveractionena;
}


/**

  Get Mesh2Mem field Value TimeoutLock from register TimeOut.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE: locked or FALSE: unlock

**/
BOOLEAN
EFIAPI
GetM2mTimeoutLock (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return FALSE;

}


/**

  Get Mesh2Mem field Value BadChRetry from register Defeatures0

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE: retry for bad channel or FALSE: not to retry

**/
BOOLEAN
EFIAPI
GetM2mDefeatures0BadChRetry (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return FALSE;
}


/**

  Get Mesh2Mem field Value ScrChkReadDis from register Defeatures0

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE: disabled or FALSE: enabled

**/
BOOLEAN
EFIAPI
GetM2mDefeatures0ScrChkReadDis (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  DEFEATURES0_M2MEM_MAIN_STRUCT   M2MemDefeatures0;

  M2MemDefeatures0.Data = UsraCsrRead (Socket, McId,  DEFEATURES0_M2MEM_MAIN_REG);
  return (BOOLEAN)M2MemDefeatures0.Bits.scrubcheckrddis;
}


/**

  Get Mesh2Mem field Value egreco from Defeatures1

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mDefeatures1EgEcoMask (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  DEFEATURES1_M2MEM_MAIN_STRUCT   M2MemDefeatures1;

  M2MemDefeatures1.Data = UsraCsrRead (Socket, McId,  DEFEATURES1_M2MEM_MAIN_REG);
  return (UINT32)M2MemDefeatures1.Bits.egreco;
}



/**

  Get Mesh2Mem field Value akwrcmphyst from WrTrKrAlloc.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32  - high 16 bits: akwrcmphysthi
                     low  16 bits: akwrcmphystlo

**/
UINT32
EFIAPI
GetM2mWrTrKrAllocAkwrCmphyst (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  //
  // only NULL stub for ICX
  //

  return 0;
}


/**

  Get Mesh2Mem Register Value for Vid.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mVid (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, VID_M2MEM_MAIN_REG);
}


/**

  Get Mesh2Mem Register Value for Did.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mDid (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, DID_M2MEM_MAIN_REG);
}


/**

  Get Mesh2Mem Register Value for ExRasConfig.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mExRasConfig (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, EXRAS_CONFIG_M2MEM_MAIN_REG);
}



/**

  Get Mesh2Mem field Mirror from register TAD_WR_N0

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE: mirrored or FALSE: not mirror

**/
BOOLEAN
EFIAPI
GetM2mTadMirror (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  TAD_WR_N0_M2MEM_MAIN_STRUCT    MeshN0Tad;

  MeshN0Tad.Data = UsraCsrRead (Socket, McId, TAD_WR_N0_M2MEM_MAIN_REG);
  return (BOOLEAN) MeshN0Tad.Bits.mirror;
}


/**

  Get Mesh2Mem Register Value for ErrCountCtrl.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mErrCountCtrl (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, ERR_CNTR_CTL_M2MEM_MAIN_REG);
}



/**

  Get Mesh2Mem Register Value for MirrorChDdr4.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Ch   - Channel ID

  @retval UINT8

**/
UINT8
EFIAPI
GetM2mMirrorChDdr4 (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               Ch
  )
{
  MIRRORCHNLS_M2MEM_MAIN_STRUCT          MirrorChannels;
  UINT8                                  ChSec;

  ChSec = 0;
  MirrorChannels.Data = UsraCsrRead (Socket, McId, MIRRORCHNLS_M2MEM_MAIN_REG);
  if (Ch == 0)  {
    ChSec = (UINT8)MirrorChannels.Bits.ddr4chnl0secondary;
  }
  if (Ch == 1)  {
    ChSec = (UINT8)MirrorChannels.Bits.ddr4chnl1secondary;
  }
  if (Ch == 2)  {
    ChSec = (UINT8)MirrorChannels.Bits.ddr4chnl2secondary;
  }
  return ChSec;
}



/**

  Get Mesh2Mem Register Value for MirrorChDdrt.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Ch   - Channel ID

  @retval UINT8

**/
UINT8
EFIAPI
GetM2mMirrorChDdrt (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               Ch
  )
{
  MIRRORCHNLS_M2MEM_MAIN_STRUCT          MirrorChannels;
  UINT8                                  ChSec;

  ChSec = 0;
  MirrorChannels.Data = UsraCsrRead (Socket, McId, MIRRORCHNLS_M2MEM_MAIN_REG);
  if (Ch == 0)  {
    ChSec = (UINT8)MirrorChannels.Bits.ddrtchnl0secondary;
  }
  if (Ch == 1)  {
    ChSec = (UINT8)MirrorChannels.Bits.ddrtchnl1secondary;
  }
  if (Ch == 2)  {
    ChSec = (UINT8)MirrorChannels.Bits.ddrtchnl2secondary;
  }
  return ChSec;
}


/**

  Get Mesh2Mem Register Value for MirrorFailOverDdr4Failed.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
BOOLEAN
EFIAPI
GetM2mMirrorFailOverDdr4Failed (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  MIRRORFAILOVER_M2MEM_MAIN_STRUCT  MirrorFailover;
  MirrorFailover.Data = UsraCsrRead (Socket, McId, MIRRORFAILOVER_M2MEM_MAIN_REG);
  return (BOOLEAN)MirrorFailover.Bits.ddr4chnlfailed;
}

/**

  Get Mesh2Mem Register Value for MirrorFailOver.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
UINT32
GetM2mMirrorFailOver (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  MIRRORFAILOVER_M2MEM_MAIN_STRUCT  MirrorFailover;
  MirrorFailover.Data = UsraCsrRead (Socket, McId, MIRRORFAILOVER_M2MEM_MAIN_REG);
  return MirrorFailover.Data;
}


/**

  Get Mesh2Mem Register Value for MirrorFailOverDdrtFailed.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
BOOLEAN
EFIAPI
GetM2mMirrorFailOverDdrtFailed (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  MIRRORFAILOVER_M2MEM_MAIN_STRUCT  MirrorFailover;
  MirrorFailover.Data = UsraCsrRead (Socket, McId, MIRRORFAILOVER_M2MEM_MAIN_REG);
  return (BOOLEAN)MirrorFailover.Bits.ddrtchnlfailed;
}



/**

  Get Mesh2Mem Register Value for MciStatusShadowN0.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mMciStatusShadowN0 (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, MCI_STATUS_SHADOW_N0_M2MEM_MAIN_REG);
}


/**

  Get Mesh2Mem field Value for McaCod from reg MciStatusShadowN0

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mMciStatusShadowN0McaCod (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  MCI_STATUS_SHADOW_N0_M2MEM_MAIN_STRUCT   MciStatusShadow0;

  MciStatusShadow0.Data = UsraCsrRead (Socket, McId, MCI_STATUS_SHADOW_N0_M2MEM_MAIN_REG);
  return (UINT32)MciStatusShadow0.Bits.mcacod;
}



/**

  Get Mesh2Mem Register Value for MciStatusShadowN1.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mMciStatusShadowN1 (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, MCI_STATUS_SHADOW_N1_M2MEM_MAIN_REG);
}

BOOLEAN
EFIAPI
GetM2mMciStatusShadowN1Valid (
  UINT8               Socket,
  UINT8               McId
  )
{
  MCI_STATUS_SHADOW_N1_M2MEM_MAIN_STRUCT   MciStatusShadow1;

  MciStatusShadow1.Data = UsraCsrRead (Socket, McId, MCI_STATUS_SHADOW_N1_M2MEM_MAIN_REG);
  return (BOOLEAN)MciStatusShadow1.Bits.valid;
}


/**

  Get Mesh2Mem Register Value for MciStatusShadowN1AddValid.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
BOOLEAN
EFIAPI
GetM2mMciStatusShadowN1AddValid (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  MCI_STATUS_SHADOW_N1_M2MEM_MAIN_STRUCT   MciStatusShadow1;

  MciStatusShadow1.Data = UsraCsrRead (Socket, McId, MCI_STATUS_SHADOW_N1_M2MEM_MAIN_REG);
  return (BOOLEAN)MciStatusShadow1.Bits.addrv;
}



/**

  Get Mesh2Mem Register Value for MciAddrShadow0.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mMciAddrShadow0 (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, MCI_ADDR_SHADOW0_M2MEM_MAIN_REG);
}

/**

  Get Mesh2Mem Register Value for MciAddrShadow1.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mMciAddrShadow1 (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, MCI_ADDR_SHADOW1_M2MEM_MAIN_REG);
}


/**

  Get Mesh2Mem Register Value for MciMiscShadowN0.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mMciMiscShadowN0 (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, MCI_MISC_SHADOW_N0_M2MEM_MAIN_REG);
}


/**

  Get Mesh2Mem Register Value for MciMiscShadowN1.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mMciMiscShadowN1 (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return UsraCsrRead (Socket, McId, MCI_MISC_SHADOW_N1_M2MEM_MAIN_REG);
}


/**

  Get Mesh2Mem Register Value for MciMiscShadowN1MirrFailover.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
BOOLEAN
EFIAPI
GetM2mMciMiscShadowN1MirrFailover (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  MCI_MISC_SHADOW_N1_M2MEM_MAIN_STRUCT MciMiscShadow1;

  MciMiscShadow1.Data = UsraCsrRead (Socket, McId, MCI_MISC_SHADOW_N1_M2MEM_MAIN_REG);
  return (BOOLEAN)MciMiscShadow1.Bits.mirrorfailover;
}


/**

  Get Mesh2Mem Register Value for MciMiscShadowN1McCmdVld.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
BOOLEAN
EFIAPI
GetM2mMciMiscShadowN1McCmdVld (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  MCI_MISC_SHADOW_N1_M2MEM_MAIN_STRUCT MciMiscShadow1;

  MciMiscShadow1.Data = UsraCsrRead (Socket, McId, MCI_MISC_SHADOW_N1_M2MEM_MAIN_REG);
  return (BOOLEAN)MciMiscShadow1.Bits.mccmdvld;
}


/**

  Get Mesh2Mem Register Value for MciMiscShadowN1McCmdMemRegion.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mMciMiscShadowN1McCmdMemRegion (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  MCI_MISC_SHADOW_N1_M2MEM_MAIN_STRUCT MciMiscShadow1;

  MciMiscShadow1.Data = UsraCsrRead (Socket, McId, MCI_MISC_SHADOW_N1_M2MEM_MAIN_REG);
  return (UINT32)MciMiscShadow1.Bits.mccmdmemregion;
}


/**

  Get Mesh2Mem Register bus dev fun in high bits 31~12, the lower 12bit offsets (bit 0~11) will be 0.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mCsrBusDevFunAddress (
  IN UINT8    Socket,
  IN UINT8    McId
  )
{
  return ((UINT32)UsraGetCsrRegisterAddress (Socket, McId, VID_M2MEM_MAIN_REG) & 0xFFFF000);
}

/**

  SetM2mSysFeatures0ForMirror.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Pcommitforcebroadcast   - Pcommitforcebroadcast
  @param Immediatefailoveractionena   - Immediatefailoveractionena

  @retval N/A

**/
VOID
EFIAPI
SetM2mSysFeatures0ForMirror (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             Pcommitforcebroadcast,
  IN BOOLEAN             Immediatefailoveractionena
  )
{
  SYSFEATURES0_M2MEM_MAIN_STRUCT   SysFeatures0;

  SysFeatures0.Data = UsraCsrRead ( Socket, McId, SYSFEATURES0_M2MEM_MAIN_REG);

  SysFeatures0.Bits.immediatefailoveractionena = Immediatefailoveractionena;
  UsraCsrWrite (Socket, McId, SYSFEATURES0_M2MEM_MAIN_REG, SysFeatures0.Data);

}


/**

  SetM2mModeMirrorType.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param MirrorDdr4   - MirrorDdr4
  @param MirrorDdrt   - MirrorDdrt

  @retval N/A

**/
VOID
EFIAPI
SetM2mModeMirrorType (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             MirrorDdr4,
  IN BOOLEAN             MirrorDdrt
  )
{
  MODE_M2MEM_MAIN_STRUCT                MeshMode;

  MeshMode.Data = UsraCsrRead (Socket, McId, MODE_M2MEM_MAIN_REG);
  MeshMode.Bits.mirrorddr4 = MirrorDdr4;
  MeshMode.Bits.mirrorddrt = MirrorDdrt;
  UsraCsrWrite (Socket, McId, MODE_M2MEM_MAIN_REG, MeshMode.Data);
}


/**

  SetM2mModeAdddc.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Adddc   - Adddc

  @retval N/A

**/
VOID
EFIAPI
SetM2mModeAdddc (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             Adddc
  )
{
  MODE_M2MEM_MAIN_STRUCT                MeshMode;

  MeshMode.Data = UsraCsrRead (Socket, McId, MODE_M2MEM_MAIN_REG);
  MeshMode.Bits.adddc = Adddc;
  UsraCsrWrite (Socket, McId, MODE_M2MEM_MAIN_REG, MeshMode.Data);
}



/**

  SetM2mTimeoutLock.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param TimeoutLock   - TimeoutLock

  @retval N/A

**/
VOID
EFIAPI
SetM2mTimeoutLock (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             TimeoutLock
  )
{
  //
  // only NULL stub for ICX
  //
  return;
}

/**

  SetM2mDefeatures0MirrorMode.

  @param[in] Socket   - Socket ID
  @param[in] McId     - Mc Controller Id
  @param[in] Wait4BothHalves, bit field value
  @param[in] EgressBypassDis, bit field value
  @param[in] IngressBypassDis,
  @param[in] DemandScrubDis

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures0MirrorMode (
  IN UINT8    Socket,
  IN UINT8    McId,
  IN UINT32   Wait4BothHalves,
  IN UINT32   EgrBypassDis,
  IN UINT32   IngBypassDis,
  IN UINT32   DemandScrubWrDis
  )
{
  DEFEATURES0_M2MEM_MAIN_STRUCT   M2MemDefeatures0;

  M2MemDefeatures0.Data = UsraCsrRead (Socket, McId,  DEFEATURES0_M2MEM_MAIN_REG);

  if (Wait4BothHalves != 0xFF) {
    M2MemDefeatures0.Bits.wait4bothhalves    = Wait4BothHalves;
  }
  if (EgrBypassDis != 0xFF) {
    M2MemDefeatures0.Bits.egrbypdis          = EgrBypassDis;
  }
  if (IngBypassDis != 0xFF) {
    M2MemDefeatures0.Bits.ingbypdis         = IngBypassDis;
  }
  if (DemandScrubWrDis != 0xFF) {
    M2MemDefeatures0.Bits.demandscrubwrdis   = DemandScrubWrDis;
  }

  UsraCsrWrite (Socket, McId, DEFEATURES0_M2MEM_MAIN_REG, M2MemDefeatures0.Data);
}

/**
  Bit field not applicable for ICX
**/
VOID
EFIAPI
SetM2MDefeatures0BadChnlFirst (
  IN UINT8    Socket,
  IN UINT8    McId,
  IN UINT32   BadChnlFirst
  )
{
  return;
}


/**

  SetM2mDefeatures0BadChRetry.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param BadChRetryFirst   - BadChRetryFirst

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures0BadChRetry (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             BadChRetryFirst
  )
{
  //
  // only NULL stub for ICX
  //

  return;
}


/**

  SetM2mDefeatures0Wait4BothHalves.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Wait4BothHalves   - Wait4BothHalves

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures0Wait4BothHalves (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             Wait4BothHalves
  )
{
  DEFEATURES0_M2MEM_MAIN_STRUCT   M2MemDefeatures0;

  M2MemDefeatures0.Data = UsraCsrRead (Socket, McId,  DEFEATURES0_M2MEM_MAIN_REG);
  M2MemDefeatures0.Bits.wait4bothhalves = Wait4BothHalves;
  UsraCsrWrite (Socket, McId, DEFEATURES0_M2MEM_MAIN_REG, M2MemDefeatures0.Data);
}

/**

  SetM2mDefeatures0ScrChkReadDis.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param ScrubChkReadDis   - ScrubChkReadDis

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures0ScrChkReadDis (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             ScrubChkReadDis
  )
{
  DEFEATURES0_M2MEM_MAIN_STRUCT   M2MemDefeatures0;

  M2MemDefeatures0.Data = UsraCsrRead (Socket, McId,  DEFEATURES0_M2MEM_MAIN_REG);
  M2MemDefeatures0.Bits.scrubcheckrddis = ScrubChkReadDis;
  UsraCsrWrite (Socket, McId, DEFEATURES0_M2MEM_MAIN_REG, M2MemDefeatures0.Data);
}



/**

  SetM2mDefeatures0EgrBypDis.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param EgrBypDis   - EgrBypDis

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures0EgrBypDis (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             EgrBypDis
  )
{
  DEFEATURES0_M2MEM_MAIN_STRUCT   M2MemDefeatures0;

  M2MemDefeatures0.Data = UsraCsrRead (Socket, McId,  DEFEATURES0_M2MEM_MAIN_REG);
  M2MemDefeatures0.Bits.egrbypdis = EgrBypDis;
  UsraCsrWrite (Socket, McId, DEFEATURES0_M2MEM_MAIN_REG, M2MemDefeatures0.Data);
}


/**

  SetM2mDefeatures1MirrorMode.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param DdrtMirrScrubWrDis   - DdrtMirrScrubWrDis
  @param SkipBadChnlForRd   - SkipBadChnlForRd
  @param SkipBadChnlForPwr   - SkipBadChnlForPwr
  @param EgEcoMask   - EgEcoMask

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures1MirrorMode (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             DdrtMirrScrubWrDis,
  IN BOOLEAN             SkipBadChnlForRd,
  IN BOOLEAN             SkipBadChnlForPwr,
  IN UINT32              EgEcoMask
  )
{
  DEFEATURES1_M2MEM_MAIN_STRUCT   M2MemDefeatures1;

  M2MemDefeatures1.Data = UsraCsrRead (Socket, McId,  DEFEATURES1_M2MEM_MAIN_REG);
  M2MemDefeatures1.Bits.ddrtmirrscrubwrdis = DdrtMirrScrubWrDis;
  M2MemDefeatures1.Bits.skipbadchnlforrd   = SkipBadChnlForRd;
  M2MemDefeatures1.Bits.egreco             = EgEcoMask;
  UsraCsrWrite (Socket, McId, DEFEATURES1_M2MEM_MAIN_REG, M2MemDefeatures1.Data);

}


/**

  SetM2mDefeatures1Cfgfailover.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Scrubfailover   - Scrubfailover

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures1Cfgfailover (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              Scrubfailover
  )
{
  DEFEATURES1_M2MEM_MAIN_STRUCT   M2MemDefeatures1;

  M2MemDefeatures1.Data = UsraCsrRead (Socket, McId,  DEFEATURES1_M2MEM_MAIN_REG);
  M2MemDefeatures1.Bits.cfgfailoveronscrubchkrdcorr = Scrubfailover;
  UsraCsrWrite (Socket, McId, DEFEATURES1_M2MEM_MAIN_REG, M2MemDefeatures1.Data);
}


/**

  SetM2mWrTrKrAllocMirrorMode.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param *Mode   - *Mode

  @retval N/A

**/
VOID
EFIAPI
SetM2mWrTrKrAllocMirrorMode (
  IN UINT8                         Socket,
  IN UINT8                         McId,
  IN WRTRKRALLOC_M2MEM_MIRROR_MODE *Mode
  )
{
  WRTRKRALLOC_M2MEM_MAIN_STRUCT WrTrKrAlloc;

  WrTrKrAlloc.Data = UsraCsrRead (Socket, McId,  WRTRKRALLOC_M2MEM_MAIN_REG);
  WrTrKrAlloc.Bits.mirrtrkrpwrthresh     = Mode->MirrTrkrPwrThresh;
  WrTrKrAlloc.Bits.mirrtrkrthresh        = Mode->MirrTrkrThresh;
  WrTrKrAlloc.Bits.mirrtrkrfaketgrthresh = Mode->MirrTrkrFakeTgrThresh;
  UsraCsrWrite (Socket, McId, WRTRKRALLOC_M2MEM_MAIN_REG, WrTrKrAlloc.Data);
}

/**

  SetM2mTadMirror.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Mirror   - Mirror

  @retval N/A

**/
VOID
EFIAPI
SetM2mTadMirror (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             Mirror
  )
{
  TAD_WR_N0_M2MEM_MAIN_STRUCT    MeshN0Tad;

  MeshN0Tad.Data = UsraCsrRead (Socket, McId, TAD_WR_N0_M2MEM_MAIN_REG);
  MeshN0Tad.Bits.mirror = Mirror;
  UsraCsrWrite (Socket, McId, TAD_WR_N0_M2MEM_MAIN_REG, MeshN0Tad.Data);
}


/**

  SetM2mBiosQuiesceRtTimer.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param TrainRtTimer   - TrainRtTimer

  @retval N/A

**/
VOID
EFIAPI
SetM2mBiosQuiesceRtTimer (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              TrainRtTimer
  )
{
  M2MBIOSQUIESCE_M2MEM_MAIN_STRUCT        M2mBiosQuiesce;
  M2mBiosQuiesce.Data = UsraCsrRead (Socket, McId, M2MBIOSQUIESCE_M2MEM_MAIN_REG);
  M2mBiosQuiesce.Bits.drainrttimer =  TrainRtTimer;
  UsraCsrWrite (Socket, McId, M2MBIOSQUIESCE_M2MEM_MAIN_REG, M2mBiosQuiesce.Data);
}

/**

  SetM2mBiosQuiesceusecmiidlefordrain.

  @param[in] Socket               - Socket ID
  @param[in] McId                 - Mc Controller Id
  @param[in] UseCmiIdleForDrain   - UseCmiIdleForDrain value to program

  @retval N/A

**/
VOID
EFIAPI
SetM2mBiosQuiesceUseCmiIdleForDrain (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              UseCmiIdleForDrain
  )
{
  M2MBIOSQUIESCE_M2MEM_MAIN_STRUCT        M2mBiosQuiesce;
  M2mBiosQuiesce.Data = UsraCsrRead (Socket, McId, M2MBIOSQUIESCE_M2MEM_MAIN_REG);
  M2mBiosQuiesce.Bits.usecmiidlefordrain =  UseCmiIdleForDrain;
  UsraCsrWrite (Socket, McId, M2MBIOSQUIESCE_M2MEM_MAIN_REG, M2mBiosQuiesce.Data);
}


/**

  SetM2mExRasConfig.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param RegValue   - RegValue

  @retval N/A

**/
VOID
EFIAPI
SetM2mExRasConfig (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              RegValue
  )
{
  UsraCsrWrite(Socket, McId, EXRAS_CONFIG_M2MEM_MAIN_REG, RegValue);
}


/**

  SetM2mExRasConfigMcaSmiOnCorr.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param CfgMcaSmiOnCorr   - CfgMcaSmiOnCorr

  @retval N/A

**/
VOID
EFIAPI
SetM2mExRasConfigMcaSmiOnCorr (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             CfgMcaSmiOnCorr
  )
{

  EXRAS_CONFIG_M2MEM_MAIN_STRUCT      ExRasCfg;

  ExRasCfg.Data = UsraCsrRead (Socket, McId, EXRAS_CONFIG_M2MEM_MAIN_REG);
  ExRasCfg.Bits.cfgmcasmioncorr          = CfgMcaSmiOnCorr;
  UsraCsrWrite (Socket, McId, EXRAS_CONFIG_M2MEM_MAIN_REG, ExRasCfg.Data);
}


/**

  SetM2mExRasConfigSmiEn.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param *Enables   - *Enables

  @retval N/A

**/
VOID
EFIAPI
SetM2mExRasConfigSmiEn (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN EXRAS_CONFIG_SMI_EN *Enables
  )
{
  EXRAS_CONFIG_M2MEM_MAIN_STRUCT      ExRasCfg;

  ExRasCfg.Data = UsraCsrRead (Socket, McId, EXRAS_CONFIG_M2MEM_MAIN_REG);
  ExRasCfg.Bits.cfgmcacmcionfailover     = Enables->CfgMcaCmciOnFailover;
  ExRasCfg.Bits.cfgmcacmcionmirrorcorr   = Enables->CfgMcaCmciOnMirrorcorr;
  ExRasCfg.Bits.cfgmcasmionmirrorcorr    = Enables->CfgMcaSmiOnMirrorcorr;
  ExRasCfg.Bits.cfgmcacmcioncorrcountthr = Enables->CfgMcaCmciOnCorrCountthr;
  ExRasCfg.Bits.cfgmcasmioncorrcountthr  = Enables->CfgMcaSmiOnCorrCountthr;
  ExRasCfg.Bits.cfgmcacmcioncorr         = Enables->CfgMcaCmciOnCorr;
  ExRasCfg.Bits.cfgmcasmioncorr          = Enables->CfgMcaSmiOnCorr;
  ExRasCfg.Bits.cfgmcacmcionrecov        = Enables->CfgMcaCmciOnRecov;
  ExRasCfg.Bits.cfgmcasmionrecov         = Enables->CfgMcaSmiOnRecov;
  UsraCsrWrite (Socket, McId, EXRAS_CONFIG_M2MEM_MAIN_REG, ExRasCfg.Data);
}


/**

  SetM2mExRasConfigFailOverAct.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param CfgMcaCmciOnFailover   - CfgMcaCmciOnFailover
  @param CfgMcaSmiOnFailover   - CfgMcaSmiOnFailover

  @retval N/A

**/
VOID
EFIAPI
SetM2mExRasConfigFailOverAct (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             CfgMcaCmciOnFailover,
  IN BOOLEAN             CfgMcaSmiOnFailover
  )
{

  EXRAS_CONFIG_M2MEM_MAIN_STRUCT      ExRasCfg;

  ExRasCfg.Data = UsraCsrRead (Socket, McId, EXRAS_CONFIG_M2MEM_MAIN_REG);
  ExRasCfg.Bits.cfgmcacmcionfailover     = CfgMcaCmciOnFailover;
  ExRasCfg.Bits.cfgmcasmionfailover      = CfgMcaSmiOnFailover;

  UsraCsrWrite (Socket, McId, EXRAS_CONFIG_M2MEM_MAIN_REG, ExRasCfg.Data);
}

/**

  SetM2mExRasConfigMcaLogMemCorr.

  @param Socket          - Socket ID
  @param McId            - Mc Controller Id
  @param McaLogMemCorr   - McaLogMemCorr

  @retval N/A

**/
VOID
EFIAPI
SetM2mExRasConfigMcaLogMemCorr (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               McaLogMemCorr
  )
{

  EXRAS_CONFIG_M2MEM_MAIN_STRUCT      ExRasCfg;

  ExRasCfg.Data = UsraCsrRead (Socket, McId, EXRAS_CONFIG_M2MEM_MAIN_REG);
  ExRasCfg.Bits.cfgmcalogmemcorr = McaLogMemCorr;
  UsraCsrWrite (Socket, McId, EXRAS_CONFIG_M2MEM_MAIN_REG, ExRasCfg.Data);
}

/**

  SetM2mExRasConfigMcaCorrOnDataUc

  @param Socket          - Socket ID
  @param McId            - Mc Controller Id
  @param McaLogMemCorr   - McaCorrOnDataUc

  @retval N/A

**/
VOID
EFIAPI
SetM2mExRasConfigMcaCorrOnDataUc (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               McaCorrOnDataUc
  )
{

  EXRAS_CONFIG_M2MEM_MAIN_STRUCT      ExRasCfg;

  ExRasCfg.Data = UsraCsrRead (Socket, McId, EXRAS_CONFIG_M2MEM_MAIN_REG);
  ExRasCfg.Bits.cfgmcacorrondatauc = McaCorrOnDataUc;
  UsraCsrWrite (Socket, McId, EXRAS_CONFIG_M2MEM_MAIN_REG, ExRasCfg.Data);
}


/**

  SetM2mMirrorChs.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Ddr4ChnlxSec   - Ddr4ChnlxSec // [0:7] ch0; [8:15] ch1; [16:23] ch2; [24: 31] rsv.
  @param DdrtChnlxSec   - DdrtChnlxSec // [0:7] ch0; [8:15] ch1; [16:23] ch2; [24: 31] rsv.

  @retval N/A

**/
VOID
EFIAPI
SetM2mMirrorChs (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              Ddr4ChnlxSec,
  IN UINT32              DdrtChnlxSec
  )
{
  MIRRORCHNLS_M2MEM_MAIN_STRUCT          MirrorChannels;


  MirrorChannels.Data = UsraCsrRead (Socket, McId, MIRRORCHNLS_M2MEM_MAIN_REG);

  MirrorChannels.Bits.ddr4chnl0secondary = (Ddr4ChnlxSec & CH0_SEC_MASK) >> CH0_SHIFT_COUNT;
  MirrorChannels.Bits.ddr4chnl1secondary = (Ddr4ChnlxSec & CH1_SEC_MASK) >> CH1_SHIFT_COUNT;
  MirrorChannels.Bits.ddr4chnl2secondary = (Ddr4ChnlxSec & CH2_SEC_MASK) >> CH2_SHIFT_COUNT;;
  MirrorChannels.Bits.ddrtchnl0secondary = (DdrtChnlxSec & CH0_SEC_MASK) >> CH0_SHIFT_COUNT;;
  MirrorChannels.Bits.ddrtchnl1secondary = (DdrtChnlxSec & CH1_SEC_MASK) >> CH1_SHIFT_COUNT;;
  MirrorChannels.Bits.ddrtchnl2secondary = (DdrtChnlxSec & CH2_SEC_MASK) >> CH2_SHIFT_COUNT;;

  UsraCsrWrite(Socket, McId, MIRRORCHNLS_M2MEM_MAIN_REG, MirrorChannels.Data);
}




/**

  SetM2mErrCountCtrl.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param CntEna   - CntEna
  @param ClearCount   - ClearCount
  @param ThresholdWr   - ThresholdWr
  @param CountIndex   - CountIndex
  @param ThresholdIndex   - ThresholdIndex

  @retval N/A

**/
VOID
EFIAPI
SetM2mErrCountCtrl (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             CntEna,
  IN BOOLEAN             ClearCount,
  IN BOOLEAN             ThresholdWr,
  IN UINT32              CountIndex,
  IN UINT32              ThresholdIndex
  )
{
  ERR_CNTR_CTL_M2MEM_MAIN_STRUCT ErrCtrlM2MCtl;

  ErrCtrlM2MCtl.Data = UsraCsrRead (Socket, McId, ERR_CNTR_CTL_M2MEM_MAIN_REG);
  ErrCtrlM2MCtl.Bits.cntena       = CntEna; // Enable counting by all the M2M internal error counters
  ErrCtrlM2MCtl.Bits.clrcntrs     = ClearCount; // Clear all the M2M internal error counters
  ErrCtrlM2MCtl.Bits.cntridx      = CountIndex;
  ErrCtrlM2MCtl.Bits.thresholdidx = ThresholdIndex;
  ErrCtrlM2MCtl.Bits.thresholdwr  = ThresholdWr; // Indicate that we want to trigger because of MIRRSCRUBRD_ERR_TYPE
  UsraCsrWrite (Socket, McId, ERR_CNTR_CTL_M2MEM_MAIN_REG, ErrCtrlM2MCtl.Data);
}


/**

  SetM2mMirrorFailOver.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Ddr4ChnlFailed   - Ddr4ChnlFailed
  @param DdrtChnlFailed   - DdrtChnlFailed

  @retval N/A

**/
VOID
EFIAPI
SetM2mMirrorFailOver (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              Ddr4ChnlFailed,
  IN UINT32              DdrtChnlFailed
  )
{
  MIRRORFAILOVER_M2MEM_MAIN_STRUCT  MirrorFailover;
  MirrorFailover.Data = UsraCsrRead (Socket, McId, MIRRORFAILOVER_M2MEM_MAIN_REG);
  MirrorFailover.Bits.ddr4chnlfailed = Ddr4ChnlFailed;
  MirrorFailover.Bits.ddrtchnlfailed = DdrtChnlFailed;
  UsraCsrWrite (Socket, McId, MIRRORFAILOVER_M2MEM_MAIN_REG, MirrorFailover.Data);
}


/**

  SetM2mMciStatusShadowN0.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param RegValue - RegValue

  @retval N/A

**/
VOID
EFIAPI
SetM2mMciStatusShadowN0 (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              RegValue
  )
{
  UsraCsrWrite(Socket, McId, MCI_STATUS_SHADOW_N0_M2MEM_MAIN_REG, RegValue);
}


/**

  SetM2mMciStatusShadowN1.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param RegValue  - RegValue

  @retval N/A

**/
VOID
EFIAPI
SetM2mMciStatusShadowN1 (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              RegValue
  )
{
  UsraCsrWrite(Socket, McId, MCI_STATUS_SHADOW_N1_M2MEM_MAIN_REG, RegValue);
}



/**

  SetM2mClearShadowRegs.

  @param Socket   - Socket ID
  @param Mc       - Mc

  @retval N/A

**/
VOID
EFIAPI
SetM2mClearShadowRegs (
   IN UINT8 Socket,
   IN UINT8 Mc
  )
{
  UsraCsrWrite (Socket, Mc, MCI_ADDR_SHADOW0_M2MEM_MAIN_REG, 0);
  UsraCsrWrite (Socket, Mc, MCI_ADDR_SHADOW1_M2MEM_MAIN_REG, 0);
  UsraCsrWrite (Socket, Mc, MCI_STATUS_SHADOW_N0_M2MEM_MAIN_REG, 0);
  UsraCsrWrite (Socket, Mc, MCI_STATUS_SHADOW_N1_M2MEM_MAIN_REG, 0);
  UsraCsrWrite (Socket, Mc, MCI_MISC_SHADOW_N0_M2MEM_MAIN_REG, 0);
  UsraCsrWrite (Socket, Mc, MCI_MISC_SHADOW_N1_M2MEM_MAIN_REG, 0);
}

/**

  SetM2mExRasConfigCorr2CorroverwriteDis

  @param Socket                  - Socket ID
  @param McId                    - Mc Controller Id
  @param Corr2CorroverwriteDis   - Corr2CorroverwriteDis

  @retval N/A

**/
VOID
EFIAPI
SetM2mExRasConfigCorr2CorroverwriteDis (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               Corr2CorroverwriteDis
  )
{
  EXRAS_CONFIG_M2MEM_MAIN_STRUCT      ExRasCfg;

  ExRasCfg.Data = UsraCsrRead (Socket, McId, EXRAS_CONFIG_M2MEM_MAIN_REG);
  ExRasCfg.Bits.corr2corroverwritedis = Corr2CorroverwriteDis;
  UsraCsrWrite (Socket, McId, EXRAS_CONFIG_M2MEM_MAIN_REG, ExRasCfg.Data);
}

/**

  SetM2mDefeatures1UcOnNmCacheRdUcData

  @param[in]  Host      Pointer to the SYSHOST structure.
  @param[in]  Socket    Socket number.

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures1UcOnNmCacheRdUcData (
  IN UINT8               Socket,
  IN UINT8               McId
  )
{
  return;
}

