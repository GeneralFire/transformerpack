/** @file
  Mesh2MemIpLib.h

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

#ifndef  _MESH_2_MEM_IP_LIB_
#define  _MESH_2_MEM_IP_LIB_

//#include "CommonIncludes.h"
#include <MemCommon.h>
#include <Uefi/UefiBaseType.h>

//
// Macros defined for GetCmiMiscCfgChCreditInitedMask()
//
#define CREDIT_INITED_CH_0 BIT0
#define CREDIT_INITED_CH_1 BIT1
#define CREDIT_INITED_CH_2 BIT2
#define CREDIT_INITED_CH_3 BIT3
#define CREDIT_INITED_CH_4 BIT4
#define CREDIT_INITED_CH_5 BIT5
#define CREDIT_INITED_CH_6 BIT6
#define CREDIT_INITED_CH_7 BIT7


//
// Macros defined for GetM2mModeMirrorType()
//
#define MIRROR_DDR4 BIT0
#define MIRROR_DDRT BIT1
#define MIRROR_DDR5 BIT2

//
// Macros defined for G/SetM2mMirrorChs()
//
#define CH0_SEC_MASK 0x000000FF
#define CH0_SHIFT_COUNT 0
#define CH1_SEC_MASK 0x0000FF00
#define CH1_SHIFT_COUNT 8
#define CH2_SEC_MASK 0x00FF0000
#define CH2_SHIFT_COUNT 16
#define CH3_SEC_MASK 0xFF000000
#define CH3_SHIFT_COUNT 24


typedef enum {
  M2mLbMcheckWRacN0RegIndex = 0,
  M2mLbMcheckWRacN1RegIndex = 1,
  M2mLbMcheckWRacN2RegIndex = 2,
  M2mLbMcheckWRacN3RegIndex = 3,
  M2mLbMcheckWWacN0RegIndex = 4,
  M2mLbMcheckWWacN1RegIndex = 5,
  M2mLbMcheckWWacN2RegIndex = 6,
  M2mLbMcheckWWacN3RegIndex = 7,
  M2mLbMcheckWCpN0RegIndex  = 8,
  M2mLbMcheckWCpN1RegIndex  = 9,
  M2mLbMcheckWCpN2RegIndex  = 10,
  M2mLbMcheckWCpN3RegIndex  = 11,
  M2mLbMcheckRegIndexMax    = 12
  } M2M_SAI_REG_ENUM;

typedef enum {
  DisableM2m2lmDirOpt,
  EnableM2m2lmDirOpt
} M2MEM_2LMDIROPT;

typedef struct {
  UINT32              MirrTrkrPwrThresh;
  UINT32              MirrTrkrThresh;
  UINT16              AkwrCmphystHi;
  UINT16              AkwrCmphystLo;
  UINT32              MirrTrkrFakeTgrThresh;
} WRTRKRALLOC_M2MEM_MIRROR_MODE;

typedef struct {
  BOOLEAN             CfgMcaCmciOnFailover;
  BOOLEAN             CfgMcaCmciOnMirrorcorr;
  BOOLEAN             CfgMcaSmiOnMirrorcorr;
  BOOLEAN             CfgMcaCmciOnCorrCountthr;
  BOOLEAN             CfgMcaSmiOnCorrCountthr;
  BOOLEAN             CfgMcaCmciOnCorr;
  BOOLEAN             CfgMcaSmiOnCorr;
  BOOLEAN             CfgMcaCmciOnRecov;
  BOOLEAN             CfgMcaSmiOnRecov;
} EXRAS_CONFIG_SMI_EN;

typedef struct {
  UINT32              SadTadId;
  UINT32              Ddr4TadId;
  UINT32              DdrtTadId;
  UINT32              AddressLimit;
  BOOLEAN             SadTadValid;
  BOOLEAN             PmemValid;
  BOOLEAN             BlockValid;
  BOOLEAN             SadTadWrEn;
  BOOLEAN             Mirror;
  BOOLEAN             NmCacheableValid;
  BOOLEAN             Ddr4;
  BOOLEAN             NonPersistentFm;  // Used only where FM is non-persistent.
} SAD_TAD_CONFIG;

typedef struct {
  BOOLEAN             IngressBypassDis;
  BOOLEAN             EgressBypassDis;
  BOOLEAN             Wait4BothHalves;
  BOOLEAN             d2k4cisgrdis;
  UINT32              Ddr4MaxRetries;
  UINT32              DdrtMaxRetries;
} M2M_DEFEATURE0_NORMAL_MODE;

typedef struct {
  BOOLEAN             IngressBypassDis;
  BOOLEAN             EgressBypassDis;
  BOOLEAN             Wait4BothHalves;
  BOOLEAN             EccOnRetry;
  BOOLEAN             DemandScrubDis;
  BOOLEAN             BadChnlFirst;
} M2M_DEFEATURE0_MIRROR_MODE;

typedef struct {
  UINT32  TimeoutMultiplier;
  UINT8   TimeoutBase;
  BOOLEAN TimeoutLock;
  BOOLEAN TimeoutEn;
} M2MEM_CR_TIMEOUT;

typedef struct {
  BOOLEAN             Enable;                  ///< Enable or disable biased 2-way near memory cache (10nm wave 3 only)
  UINT16              NonPreferredWayMask;     ///< A 10-bit mask to control the bias counter ratio (10nm wave 3 only)
  UINT8               PreferredReadFirst;      ///< Reads are issued to the non-preferred or preferred way first (10nm wave 3 only)
} M2M_TWO_WAY_CACHE;


/**

  Set Defeatures1[EgrEco[26]] = 1 when BIOS initiates sparing

  @param[in]  Socket      - Socket number
  @param[in]  Mc          - Mc number
  @param[in]  EgrEco      - bit EgrEco

  @retval None

**/
VOID
EFIAPI
SetM2MEgreco (
  IN UINT8    Socket,
  IN UINT8    Mc,
  IN UINT32   EgrEco
);


/**

  Get or Set AK early completions.

  @param[in]      Socket            The target socket
  @param[in]      MemoryController  The target memory controller in the target socket
  @param[in]      Mode              Get/Set mode flags
  @param[in,out]  Data              AK early completions value. The current value is always returned,
                                    while an input value is used in accordance with Mode flags.

  @retval   EFI_SUCCESS   If success to get or set
  @retval   Others        Failed

**/
EFI_STATUS
EFIAPI
GetSetM2mAkEarlyCompletionsEnabled (
  IN     UINT8  Socket,
  IN     UINT8  MemoryController,
  IN     UINT8  Mode,
  IN OUT UINT8  *Data
  );

/**

  Get Mesh2Mem field Value for CmiMiscCfg - Channel IDannle's Credit Init Mask.
  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT8    - CREDIT_INITED_CH_0 = 1 means ch0 is inited, and so on.

**/
UINT8
EFIAPI
GetM2mCmiMiscCfgChCreditInitedMask (
  IN UINT8    Socket,
  IN UINT8    McId
  );

/**

  Get Mesh2Mem Register Value for SysFeatures0.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mSysFeatures0 (
  IN UINT8    Socket,
  IN UINT8    McId
  );

/**

  Get Mesh2Mem field Value for SysFeatures0 - Preftch Disable.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
BOOLEAN
EFIAPI
GetM2mSysFeatures0PreftchDis (
  IN UINT8    Socket,
  IN UINT8    McId
  );

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
  );

//
// Macros defined for GetM2mModeValue ()
//
#define ISOCHRONOUS_FLOW_ENABLED_BIT 0x10   //BIT 4
/**

  Get Mesh2Mem Register Value for ModeValue.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mModeValue (
  IN UINT8    Socket,
  IN UINT8    McId
  );

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
  );

/**

  Get Mesh2Mem Register Value for PrefetchSad.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mPrefetchSad (
  IN UINT8               Socket,
  IN UINT8               McId
  );

/**

  Get Mesh2Mem Register Value for Sad2Tad.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mSad2Tad (
  IN UINT8               Socket,
  IN UINT8               McId
  );
/**

  Get Mesh2Mem Register Value for Timeout.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mTimeout (
  IN UINT8    Socket,
  IN UINT8    McId
  );

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
  );

/**

  Get Mesh2Mem field Value TimeoutEn from register TimeOut.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE: timeout enabled or FALSE: disabled

**/
BOOLEAN
EFIAPI
GetM2mTimeoutEn (
  IN UINT8               Socket,
  IN UINT8               McId
  );

/**

  Get Mesh2Mem Register Value for Defeatures0.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mDefeatures0 (
  IN UINT8               Socket,
  IN UINT8               McId
  );

/**

  Get Mesh2Mem field Value IngressBypassDis from register Defeatures0

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE: disabled or FALSE: enabled

**/
BOOLEAN
EFIAPI
GetM2mDefeatures0IngressBypassDis (
  IN UINT8               Socket,
  IN UINT8               McId
  );

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
  );

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
  );

/**

  Get Mesh2Mem field Value Ingeco from Defeatures1.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mDefeatures1IngEcoMask (
  IN UINT8               Socket,
  IN UINT8               McId
  );

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
  );

/**

  Get Mesh2Mem field Value SkipBadChnlPwr from Defeatures1

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE: skip bad ch, FALSE: not skip bad ch

**/
BOOLEAN
EFIAPI
GetM2mDefeatures1SkipBadChnlPwr (
  IN UINT8               Socket,
  IN UINT8               McId
  );

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
  );

/**

  Get Mesh2Mem Register Value for Topology.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mTopology (
  IN UINT8               Socket,
  IN UINT8               McId
  );

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
  );

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
  );

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
  );

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
  );


/**

  Get Mesh2Mem Register Value for Nid2Kti.

  @param[in] Socket              Socket ID
  @param[in] McId                Mc Controller Id
  @param[in] RegisterSelection   0 to select NID2KTI_M2MEM_MAIN_REG or NID2KTI_N0_M2MEM_MAIN_REG.
                                 1 to select NID2KTI_N1_M2MEM_MAIN_REG register. This field is always
                                 zero for SOC that does not support NID2KTI_N1_M2MEM_MAIN_REG register.

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mNid2Kti (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               RegisterSelection
  );

/**

  Get Mesh2Mem field Value for SwapKtiLinkIntlv from reg Nid2Kti

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
BOOLEAN
EFIAPI
GetM2mNid2KtiSwapKtiLinkIntlv (
  IN UINT8               Socket,
  IN UINT8               McId
  );

/**

  Get Mesh2Mem Register Value for Add1LutN1.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mAdd1LutN1 (
  IN UINT8               Socket,
  IN UINT8               McId
  );

/**

  Get Mesh2Mem Register Value for PrefSadConfig.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mPrefSadConfig (
  IN UINT8               Socket,
  IN UINT8               McId
  );

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
  );


/**

  Get Mesh2Mem ch ddr4chnl*secondary field Value from Mirror register.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Ch   - Channel ID

  @retval UINT8 the ddr4chnl*secondary for that channel

**/
UINT8
EFIAPI
GetM2mMirrorChDdr4 (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               Ch
  );


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
  );

/**

  Get Mesh2Mem Register Value for CreditThrsh.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mCreditThrsh (
  IN UINT8               Socket,
  IN UINT8               McId
  );
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
  );

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
  );

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
  );

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
  );

/**

  Get Mesh2Mem Register Value for MciStatusShadowN0McaCod.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mMciStatusShadowN0McaCod (
  IN UINT8               Socket,
  IN UINT8               McId
  );

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
  );

/**

  Get Mesh2Mem Register Value for MciStatusShadowN1Valid.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval BOOLEAN - TRUE or FALSE

**/
BOOLEAN
EFIAPI
GetM2mMciStatusShadowN1Valid (
  IN UINT8               Socket,
  IN UINT8               McId
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );


//
// All sets begin here
//



/**

  Initialize M2Mem tiles.

  @param Socket             - socket ID
  @param McId               - M2Mem index
  @param Direct2CoreEn      - Direct to core enable
  @param Direct2KtiEn       - Direct to KTI enable
  @param DirModeEn          - DirectoryMode enable
  @param IsocEn             - Isoc enable
  @param FpgaList           - Fpga list

  @retval N/A

**/
VOID
EFIAPI
InitM2mem (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             Direct2CoreEn,
  IN BOOLEAN             Direct2KtiEn,
  IN BOOLEAN             DirModeEn,
  IN BOOLEAN             IsocEn,
  IN UINT32              FpgaList
  );

/**

  SetM2mPrefetchConfigs.

  @param Socket   - Socket ID
  @param *MemInfoKti   - *MemInfoKti

  @retval N/A

**/
VOID
EFIAPI
SetM2mPrefetchConfigs (
  IN UINT8                      Socket,
  IN UINT8                      UmaBasedClustering,
  IN VOID                       *MemInfoKti
  );

/**

  SetM2mCmiEgressCreditChnlConfigDone.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Ch   - Channel ID
  @param ConfigDone   - ConfigDone

  @retval N/A

**/
VOID
EFIAPI
SetM2mCmiEgressCreditChnlConfigDone (
  IN UINT8    Socket,
  IN UINT8    McId,
  IN UINT8    Ch,
  IN BOOLEAN  ConfigDone
  );

/**

  SetM2mCmiMiscConfigDoneAllCh.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param ConfigDone   - ConfigDone

  @retval N/A

**/
VOID
EFIAPI
SetM2mCmiMiscConfigDoneAllCh (
  IN UINT8    Socket,
  IN UINT8    McId,
  IN BOOLEAN  ConfigDone
  );

/**

  Set M2M Fm in same tile configure

  @param[in] SocketId      - Socket ID
  @param[in] McId          - Mc Controller Id
  @param[in] InTileCaching - Flag to indicate whether it is in tile caching.

**/
VOID
EFIAPI
SetM2mFmInSameTile (
  IN UINT8            SocketId,
  IN UINT8            McId,
  IN BOOLEAN          InTileCaching
  );

/**

  Set M2M DDRT exists field

  @param[in] SocketId      - Socket ID
  @param[in] McId          - Mc Controller Id
  @param[in] DdrtExists    - Flag to indicate whether ddrt exists.

**/
VOID
EFIAPI
SetM2mDdrtExists (
  IN UINT8            SocketId,
  IN UINT8            McId,
  IN BOOLEAN          DdrtExists
  );

/**

  SetM2mSysFeatures0PreftchDis.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param DisablePrefetch   - DisablePrefetch

  @retval N/A

**/
VOID
EFIAPI
SetM2mSysFeatures0PreftchDis (
  IN UINT8         Socket,
  IN UINT8         McId,
  IN BOOLEAN       DisablePrefetch
  );

/**

  SetM2mSysFeatures0FarMcMode.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param FmcMode   - FmcMode

  @retval N/A

**/
VOID
EFIAPI
SetM2mSysFeatures0FarMcMode (
  IN UINT8         Socket,
  IN UINT8         McId,
  IN UINT8         FmcMode
  );

/**

  GetM2mSysFeatures0FarMcMode.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval FMC mode

**/
UINT32
EFIAPI
GetM2mSysFeatures0FarMcMode (
  IN UINT8         Socket,
  IN UINT8         McId
  );

/**

  SetM2mEnableDir4BlockRegion.

  @param[in] Socket                - Socket ID
  @param[in] McId                  - Mc Controller Id
  @param[in] EnableDir4BlockRegion - EnableDir4BlockRegion

  @retval N/A

**/
VOID
EFIAPI
SetM2mEnableDir4BlockRegion (
  IN UINT8         Socket,
  IN UINT8         McId,
  IN UINT8         EnableDir4BlockRegion
  );

/**

  SetM2mDisableDir4AppDirect.

  @param[in] Socket               - Socket ID
  @param[in] McId                 - Mc Controller Id
  @param[in] DisableDir4AppDirect - DisableDir4AppDirect

  @retval N/A

**/
VOID
EFIAPI
SetM2mDisableDir4AppDirect (
  IN UINT8         Socket,
  IN UINT8         McId,
  IN UINT8         DisableDir4AppDirect
  );

/**

  This function sets the AllowDirUpdToBlock field of Defeatures0 register.

  @param[in] SocketId               Socket index.
  @param[in] McId                   Memory controller index within socket.
  @param[in] AllowDirUpdToBlock     Data to be written to AllowDirUpdToBlock field.

**/
VOID
EFIAPI
SetM2mAllowDirUpdToBlock (
  IN UINT8    SocketId,
  IN UINT8    McId,
  IN UINT8    AllowDirUpdToBlock
  );

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
  );


/**

    Program M2Mem credits.

    @param Socket            - Socket ID
    @param McId              - MC index
    @param Port              - Port
    @param CrdtType          - Credit type
    @param CrdtCnt           - Credit count
    @retval N/A

**/
VOID
EFIAPI
SetM2mCreditReg (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               Port,
  IN UINT8               CrdtType,
  IN UINT8               CrdtCnt
  );

/**

  SetM2mModeMmap.

  @param[in] Socket          Socket ID
  @param[in] McId            Mc Controller Id
  @param[in] Nmcaching       Nmcaching
  @param[in] Pmem            Pmem
  @param[in] Blockregion     Blockregion
  @param[in] InTileCaching   In tile caching flag for 10nm wave 3 only

  @retval N/A

**/
VOID
EFIAPI
SetM2mModeMmap (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             Nmcaching,
  IN BOOLEAN             Pmem,
  IN BOOLEAN             Blockregion,
  IN BOOLEAN             InTileCaching
  );


/**

  SetM2mPrefetchSadValid.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param PfSadEntryValid   - PfSadEntryValid

  @retval N/A

**/
VOID
EFIAPI
SetM2mPrefetchSadValid (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             PfSadEntryValid
  );

/**

  SetM2mPrefetchSadWrEn.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Prefsadwren   - Prefsadwren

  @retval N/A

**/
/**

  SetM2mPrefetchSadWrEn.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Prefsadwren   - Prefsadwren

  @retval N/A

**/
VOID
EFIAPI
SetM2mPrefetchSadWrEn (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             Prefsadwren
  );

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
  );

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
  );

/**

  SetM2mPrefetchEntryInterlv.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Prefsadentry   - Prefsadentry
  @param Prefsadwren   - Prefsadwren
  @param Prefsadchnls   - Prefsadchnls
  @param InterleaveGrain   - InterleaveGrain

  @retval N/A

**/
VOID
EFIAPI
SetM2mPrefetchEntryInterlv (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              Prefsadentry,
  IN BOOLEAN             Prefsadwren,
  IN UINT32              Prefsadchnls,
  IN UINT32              InterleaveGrain
  );

/**

  SetM2mSad2TadClearEn.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval N/A

**/
VOID
EFIAPI
SetM2mSad2TadClearEn (
  IN UINT8               Socket,
  IN UINT8               McId
  );

/**

  SetM2mSad2TadWrEn.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param WrEnable   - WrEnable

  @retval N/A

**/
VOID
EFIAPI
SetM2mSad2TadWrEn (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             WrEnable
  );

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
  );

/**

  SetM2mTimeoutEn.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param TimeoutEn   - TimeoutEn

  @retval N/A

**/
VOID
EFIAPI
SetM2mTimeoutEn (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             TimeoutEn
  );


/**

  Writes to Mesh2Mem register to set memory channel population

  @param[in]  Socket             Socket ID number
  @param[in]  McId               Memory controller ID number
  @param[in]  DdrtChPopulation   Bit mask indicating which channels have DDRT present
                                    BIT0 = CH0, BIT1 = CH1, etc
                                    1 = DDRT memory present
                                    0 = DDRT memory not present
  @param[in]  Ddr4ChPopulation   Bit mask indicating which channels have DDR4 present
                                    BIT0 = CH0, BIT1 = CH1, etc
                                    1 = DDR4 memory present
                                    0 = DDR4 memory not present

  @retval N/A

**/
VOID
EFIAPI
SetM2mMemoryChPopulation (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               DdrtChPopulation,
  IN UINT8               Ddr4ChPopulation
  );

/**

  SetM2mCreditWrEn.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param CreditWrEn   - CreditWrEn

  @retval N/A

**/
VOID
EFIAPI
SetM2mCreditWrEn (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             CreditWrEn
  );

/**

  SetM2mCreditConfigs.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param CreditWrEn   - CreditWrEn
  @param Credittype   - Credittype
  @param CreditCount   - CreditCount

  @retval N/A

**/
VOID
EFIAPI
SetM2mCreditConfigs (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             CreditWrEn,
  IN UINT32              Credittype,
  IN UINT32              CreditCount
  );

/**

  SetM2mDefeatures0NormalMode.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param *Mode   - *Mode

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures0NormalMode (
  IN UINT8                     Socket,
  IN UINT8                     McId,
  IN M2M_DEFEATURE0_NORMAL_MODE *Mode
  );

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
  );

/**

  SetM2MDefeatures0BadChnlFirsts.

  @param[in] Socket   - Socket ID
  @param[in] McId     - Mc Controller Id
  @param[in] BadChnlFirst

  @retval N/A

**/
VOID
EFIAPI
SetM2MDefeatures0BadChnlFirst (
  IN UINT8    Socket,
  IN UINT8    McId,
  IN UINT32   BadChnlFirst
  );

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
  );

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
  );

/**
  This function sets the DisCritChunk bit field of DEFEATURES0_M2MEM_MAIN_REG register.

  @param[in] SocketId         Socket index.
  @param[in] McId             Memory controller index on socket.
  @param[in] DisCritChunk     Flag indicates if critical chunk needs to be disabled.

  @retval N/A
**/
VOID
EFIAPI
SetM2mDefeatures0DiscritChunk (
  IN UINT8       SocketId,
  IN UINT8       McId,
  IN BOOLEAN     DisCritChunk
  );

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
  );

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
  );

/**

  SetM2mDefeatures1NormalMode.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param UcOnNmCacheRdUcdata   - UcOnNmCacheRdUcdata
  @param UcOnNmCachePwrUcData   - UcOnNmCachePwrUcData
  @param IngEcoMask   - IngEcoMask
  @param EgEcoMask   - EgEcoMask

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures1NormalMode (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             UcOnNmCacheRdUcdata,
  IN BOOLEAN             UcOnNmCachePwrUcData,
  IN UINT32              IngEcoMask,
  IN UINT32              EgEcoMask
  );

/**

  SetM2mDefeatures1IngEcoMask.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param IngEcoMask   - IngEcoMask

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures1IngEcoMask (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              IngEcoMask
  );

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
  );

/**

  SetM2mDefeatures1EgEcoMask.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param EgressEco   - EgressEco

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures1EgEcoMask (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              EgressEco
  );

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
  );

/**

  SetM2mWrTrKrAllocPwrThresh.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param PwrThresh   - PwrThresh

  @retval N/A

**/
VOID
EFIAPI
SetM2mWrTrKrAllocPwrThresh (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              PwrThresh
  );

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
  );

/**

  SetM2mTopologySnc.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param SncCluster   - SncCluster
  @param BaseCluster1   - BaseCluster1
  @param BaseCluster1   - BaseCluster2
  @retval N/A

**/
VOID
EFIAPI
SetM2mTopologySnc (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               SncCluster,
  IN UINT32              BaseCluster1,
  IN UINT32              BaseCluster2
  );

/**

  SetM2mTadConfigs.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param *Config   - *Config

  @retval N/A

**/
VOID
EFIAPI
SetM2mTadConfigs (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN SAD_TAD_CONFIG      *Config
  );

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
  );

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
  );

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
  );

/**

  SetM2mNid2KtiMap.

  @param[in] Socket         - Socket ID
  @param[in] McId           - Mc Controller Id
  @param[in] Nid2KtiMapLow  - Lower 32-bits of Nid2KtiMap
  @param[in] Nid2KtiMapHigh - Upper 16 bits of Nid2KtiMap (only applicable to SOC that has more than 3 UPI links)

  @retval N/A

**/
VOID
EFIAPI
SetM2mNid2KtiMap (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              Nid2KtiMapLow,
  IN UINT32              Nid2KtiMapHigh
  );

/**

  SetM2mUpiInterleaveMode.

  @param[in] Socket            - Socket ID
  @param[in] McId              - Mc Controller Id
  @param[in] UpiInterleaveMode - Upi Interleave Mode

  @retval N/A

**/
VOID
EFIAPI
SetM2mUpiInterleaveMode (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               UpiInterleaveMode
  );

/**

  SetM2mNid2D2k2s3KtiEn.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param D2k2s3KtiEn   - D2k2s3KtiEn

  @retval N/A

**/
VOID
EFIAPI
SetM2mNid2D2k2s3KtiEn (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             D2k2s3KtiEn
  );

/**

  SetNid2KtiConfigs.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Kti2SlinkIntlvEn   - Kti2SlinkIntlvEn
  @param SwapKtiLinkIntlv   - SwapKtiLinkIntlv

  @retval N/A

**/
VOID
EFIAPI
SetNid2KtiConfigs (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             Kti2SlinkIntlvEn,
  IN BOOLEAN             SwapKtiLinkIntlv
  );
/**

  SetM2mPrefSadConfig.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param FirstPfTadId   - FirstPfTadId
  @param SecondPfTadId   - SecondPfTadId
  @param PrefMmioBaseAddr   - PrefMmioBaseAddr

  @retval N/A

**/
VOID
EFIAPI
SetM2mPrefSadConfig (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              FirstPfTadId,
  IN UINT32              SecondPfTadId,
  IN UINT32              PrefMmioBaseAddr
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );


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
  );


/**

  SetM2mPrefetchTuningConfigs.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param PfTuningEn   - PfTuningEn
  @param CfgDirUpdpfcameVictDis   - CfgDirUpdpfcameVictDis
  @param PrefRpqThreshold   - PrefRpqThreshold
  @param PrefWpqThreshold   - PrefWpqThreshold
  @param PrefKtiCamThreshold   - PrefKtiCamThreshold
  @param PrefXptCamThreshold   - PrefXptCamThreshold

  @retval N/A

**/
VOID
EFIAPI
SetM2mPrefetchTuningConfigs (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             PfTuningEn,
  IN BOOLEAN             CfgDirUpdpfcameVictDis,
  IN UINT32              PrefRpqThreshold,
  IN UINT32              PrefWpqThreshold,
  IN UINT32              PrefKtiCamThreshold,
  IN UINT32              PrefXptCamThreshold
  );

/**

  SetM2mCreditThrshConfigs.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param CritRdThresh   - CritRdThresh
  @param CritWrThresh   - CritWrThresh
  @param CritPartialWdsbthresh   - CritPartialWdsbthresh

  @retval N/A

**/
VOID
EFIAPI
SetM2mCreditThrshConfigs (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              CritRdThresh,
  IN UINT32              CritWrThresh,
  IN UINT32              CritPartialWdsbthresh
  );

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
  );

/**

  SetM2mPrmrrBase.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param PrmrrBaseLo   - PrmrrBase Low
  @param PrmrrBaseHi   - PrmrrBase Hi

  @retval N/A

**/
VOID
EFIAPI
SetM2mPrmrrBase (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              PrmrrBaseLo,
  IN UINT32              PrmrrBaseHi
  );

/**

  SetM2mPrmrrMask.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param PrmrrMaskLo   - PrmrrMaskLo
  @param PrmrrMaskHi   - PrmrrMaskHi

  @retval N/A

**/
VOID
EFIAPI
SetM2mPrmrrMask (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              PrmrrMaskLo,
  IN UINT32              PrmrrMaskHi
  );

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
  );

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
  );

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
  );


/**

  GetSetM2mSaiRegisters.

  @param Socket        - Socket ID
  @param McId          - Mc Controller Id
  @param SaiRegIndex   - SaiRegIndex
  @param Get           - TRUE: read register into *Value, FALSE: write register using data in *Value
  @param *Value        - *Value: pointer of UINT32, can be input & output

  @retval N/A

**/
VOID
EFIAPI
GetSetM2mSaiRegisters (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN M2M_SAI_REG_ENUM    SaiRegIndex,
  IN BOOLEAN             Get,
  IN OUT UINT32          *Value
  );

/**
  Overwrite Mesh2Mem registers for project specific requirements
  needing different setting for certain fields

  @param[in] Socket - Socket ID

  @retval N/A
**/
VOID
EFIAPI
ModifyMesh2MemCsrsChip (
  IN UINT8 Socket
  );

/**

  Program M2mem CMI registers

  @param[in] ProgramableValue[MAX_CH]    - Array containing type of value to program per channel (From 0 to Max Channel in Socket)
                                           1 = DDRT,2 = 1LM,3 = CR, 4 = 1LM ISOCH..CMI value
  @param[in] Socket                      - Socket number
  @param[in] McId                        - Memory Controller number

  @retval N/A

**/
VOID
EFIAPI
ProgramMesh2MemCmiRegisters (
  IN UINT8 ProgramableValue[MAX_CH],
  IN UINT8 Socket,
  IN UINT8 McId
  );

/**

  Poll CMI Misc register for Init done. This function generates ASM polling logic
  if FCE bios enabled

  @param[in] Socket   - Socket number
  @param[in] McId     - Memory controller Id

  @retval  N/A

**/
VOID
EFIAPI
PollM2mCmiMiscCfgChInit (
  IN UINT8    Socket,
  IN UINT8    McId
  );

/**

  Program M2mem DistressSignalling registers

  @param[in] Socket        - Socket ID
  @param[in] McId          - Mc Controller Id
  @param[in] DdrtQosMode   - DDRT QoS Mode
  @param[in] DistressSel   - Distress Global Half Die selector
                             0 : Signal distress on SNC 0 global distress
                             1 : Signal distress on SNC 1 global distress

  @retval N/A

**/
VOID
EFIAPI
ProgramMesh2MemDistressSignallingForCrQos (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT8 DdrtQosMode,
  IN UINT8 DistressSel
  );

/**

  Get value of M2mem DistressSignalling registers

  @param[in] Socket        - Socket ID
  @param[in] McId          - Mc Controller Id

  @retval Value of this register

**/
UINT32
EFIAPI
GetM2mDistressSignallingRegisters (
  IN UINT8 Socket,
  IN UINT8 McId
  );

/**
  This function programs the Mesh2Mem pcommit registers

  @param[in] Socket          Socket index
  @param[in] McId            Memory controller index
  @param[in] Address         Address array to configure for pcommit

  @return N/A
**/
VOID
EFIAPI
ProgramM2mPcommit (
  IN UINT8  Socket,
  IN UINT8  McId,
  IN UINT64 Address[MAX_MC_CH]
  );

/**
  @brief Program Mesh2mem 2LM Directory Optimization registers.

  @param[in] Socket        - Socket ID
  @param[in] McId          - Mc Controller Id
  @param[in] M2m2lmDirOpt  - Emum value specifying whether to enable or disable the optimization.

  @return Void.
**/
VOID
EFIAPI
ProgramM2m2lmDirOpt (
  IN UINT8           Socket,
  IN UINT8           McId,
  IN M2MEM_2LMDIROPT M2m2lmDirOpt
  );

/**

  SetM2mTimeoutValues.

  @param[in] Socket      - Socket ID
  @param[in] McId        - Mc Controller Id
  @param[in] TimeOutData - Pointer to M2MEM_CR_TIMEOUT struct

  @retval N/A

**/
VOID
EFIAPI
SetM2mTimeoutValues (
  IN UINT8            Socket,
  IN UINT8            McId,
  IN M2MEM_CR_TIMEOUT *TimeOutData
  );

/**
  This function programs the Mesh2mem biased 2-way near memory cache support register.

  @param[in] ScktId               Socket index.
  @param[in] McId                 Memory controller index.
  @param[in] M2M_TWO_WAY_CACHE    Control data of two way cache.

  @retval N/A
**/
VOID
EFIAPI
SetM2mTwoWayNmCache (
  IN UINT8              ScktId,
  IN UINT8              McId,
  IN M2M_TWO_WAY_CACHE  *M2mTwoWayCache
  );

/**

  This is a wrapper routine that calls auto-generated routine to configure QoS
  setting based on Recipe Profile setup knob value.

  @param[in] Socket   Processor to initialize

  @retval EFI_SUCCESS       QoS setting were setup
  @retval EFI_NOT_READY     NULL SysSetup and/or Host pointer

**/
EFI_STATUS
EFIAPI
NvmdimmQosPerfSetting (
  IN UINT8    Socket
  );

/**

  Enable D2K irrespective of directory state in 2S configs

  @param Socket     - Socket ID
  @param McId       - Mc Controller Id
  @param FpgaPres   - Indicates if Fpgas are present
  @param AtosStatus - Current StaleAtoSOptEn  setting

  @retval NONE

**/
VOID
EFIAPI
Set2SD2KEnable (
  IN UINT8    Socket,
  IN UINT8    McId,
  IN BOOLEAN  FpgaPres,
  IN UINT8    AtosStatus
  );

/**

  This is a wrapper routine that calls auto-generated routine to configure directory setting
  in far memory or for App direct.

  @param[in] Socket                Socket ID

  @retval EFI_SUCCESS       M2Mem registers are setup based on the requested directory setting
  @retval EFI_NOT_READY     NULL SysSetup pointer

**/
EFI_STATUS
EFIAPI
M2mDirectoryOptimizationSetting (
  IN  UINT8              Socket
  );

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
  );

/**
  This function sets the wrcmpfromnm bit field of DEFEATURES0_M2MEM_MAIN_REG register
  when 2LM NM$ is x-tile.

  @param[in] SocketId         Socket index.
  @param[in] McId             Memory controller index on socket.
  @param[in] IsXTileCaching   Flag indicating MC is caching x-tile or in-tile.

  @retval N/A
**/
VOID
EFIAPI
SetM2mDefeatures0WrCmpFromNm (
  IN UINT8       SocketId,
  IN UINT8       McId,
  IN BOOLEAN     IsXTileCaching
  );

/**

  SetM2mCmiMiscConfigDoneChHbm.

  @param Socket       - Socket ID
  @param HbmCh        - Hbm Channel ID
  @param ConfigDone   - ConfigDone

  @retval N/A

**/
VOID
EFIAPI
SetM2mCmiMiscConfigDoneChHbm (
  IN UINT8    Socket,
  IN UINT8    HbmCh,
  IN BOOLEAN  ConfigDone
  );

/**
  Build Target function to wait for Cmi misc channels to complete.

  @param[in]   Socket           Socket Number.
  @param[in]   HbmCh            Hbm Channel Id

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
WaitForM2mCmiMiscConfigChannelsCompleteTargetHbm (
  IN UINT8 Socket,
  IN UINT8 HbmCh
  );

/**

  Program M2mem CMI registers for HBM

  @param[in] ProgramableValue[]    - Array containing type of value to program per channel.
  @param[in] Socket                - Socket number

  @retval N/A

**/
VOID
EFIAPI
ProgramMesh2MemCmiRegistersHbm (
  IN UINT8 ProgramableValue[],
  IN UINT8 Socket
  );

/**
  This function sets the wciloptdis bit field of DEFEATURES0_M2MEM_MAIN_REG register.

  @param[in] SocketId         Socket index.
  @param[in] McId             Memory controller index within socket.
  @param[in] WcilOptDis       Flag indicates if Wcil optimization needs to be disabled.

**/
VOID
EFIAPI
SetM2mDefeatures0WcilOptDis (
  IN UINT8       SocketId,
  IN UINT8       McId,
  IN BOOLEAN     WcilOptDis
  );

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
  );

#endif
