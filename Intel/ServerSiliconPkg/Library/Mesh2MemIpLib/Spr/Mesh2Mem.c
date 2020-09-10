/** @file
  Mesh2Memory configure Module.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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
#include <Library/Mesh2MemIpLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/KtiApi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <MemHost.h>
#include <Library/UsraCsrLib.h>
#include "Include/Mesh2MemRegs.h"
#include <Library/CteNetLib.h>
#include <Cpu/CpuIds.h>
#include <CmiPerformance.h>
#include <Library/MemRcLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include "Mesh2MemCmiCredits.h"
#include <KtiSetupDefinitions.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/MemMap2SgxInterface.h>
#include "Include/Mesh2MemIpLibInternal.h"


STATIC const UINT32 CmiRdCplCreditChnlConfig1RegAddr[MAX_MC_CH] = {
  CMIRDCPLCREDITCHNLCONFIG_0_M2MEM_MAIN_REG,
#if MAX_MC_CH > 1
  CMIRDCPLCREDITCHNLCONFIG_1_M2MEM_MAIN_REG
#endif
};

STATIC const UINT32 CmiRdCplCreditChnlConfig2RegAddr[MAX_MC_CH] = {
  CMIRDCPLCREDITCHNLCONFIG_2_0_M2MEM_MAIN_REG,
#if MAX_MC_CH > 1
  CMIRDCPLCREDITCHNLCONFIG_2_1_M2MEM_MAIN_REG
#endif
};

STATIC const UINT32 CmiRspCreditChnlConfigRegAddr[MAX_MC_CH] = {
  CMIRSPCREDITCHNLCONFIG_0_M2MEM_MAIN_REG,
#if MAX_MC_CH > 1
  CMIRSPCREDITCHNLCONFIG_1_M2MEM_MAIN_REG
#endif
};

UINT32 mPadN0M2mRegs[MAX_PAD] = {
  PAD0_N0_M2MEM_MAIN_REG,
  PAD1_N0_M2MEM_MAIN_REG,
  PAD2_N0_M2MEM_MAIN_REG
};

UINT32 mPadN1M2mRegs[MAX_PAD] = {
  PAD0_N1_M2MEM_MAIN_REG,
  PAD1_N1_M2MEM_MAIN_REG,
  PAD2_N1_M2MEM_MAIN_REG
};

STATIC const UINT32    PrefechSadConfigRegTable[2] = { PREFETCHSADCONFIG_0_M2MEM_MAIN_REG,
                                          PREFETCHSADCONFIG_1_M2MEM_MAIN_REG
                                        };

STATIC const UINT32    PrefechSadStartAddrRegTable[3] = {
                                            PREFETCHSADSTARTADDR_0_M2MEM_MAIN_REG,
                                            PREFETCHSADSTARTADDR_1_M2MEM_MAIN_REG,
                                            PREFETCHSADSTARTADDR_2_M2MEM_MAIN_REG
                                           };

STATIC const UINT32    PrefechSadLimitAddrRegTable[3] = {
                                            PREFETCHSADLIMITADDR_0_M2MEM_MAIN_REG,
                                            PREFETCHSADLIMITADDR_1_M2MEM_MAIN_REG,
                                            PREFETCHSADLIMITADDR_2_M2MEM_MAIN_REG
                                           };

typedef struct {
  PREFETCHSADCONFIG_0_M2MEM_MAIN_STRUCT    PrefetchSadConfig[2];
  PREFETCHSADSTARTADDR_0_M2MEM_MAIN_STRUCT PrefetchSadStartAddr[3];
  PREFETCHSADLIMITADDR_0_M2MEM_MAIN_STRUCT PrefetchSadLimitAddr[3];
} M2MEM_PREFETCH_SAD_INFO;


/**

  Keep for SKX support.
  Fixed by ICX silicon. To be removed until remove SKX support.

**/
VOID
EFIAPI
SetM2MEgreco (
  IN UINT8    Socket,
  IN UINT8    Mc,
  IN UINT32   EgrEco
)
{
}

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
  )
{
  SYSFEATURES0_M2MEM_MAIN_STRUCT  SysFeatures0;

  SysFeatures0.Data = UsraCsrRead (Socket, MemoryController, SYSFEATURES0_M2MEM_MAIN_REG);

  if ((Mode & GSM_READ_ONLY) != 0) {
    *Data = (UINT8) SysFeatures0.Bits.akearlycmpen;
  } else {
    SysFeatures0.Bits.akearlycmpen = *Data;
    UsraCsrWrite (Socket, MemoryController, SYSFEATURES0_M2MEM_MAIN_REG, SysFeatures0.Data);
  }

  return EFI_SUCCESS;
}

/**

  Get Mesh2Mem Register Value for CmiRdCplCreditChnlConfig1.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Ch       - Channel ID

  @retval UINT32

**/
UINT32
GetM2mCmiRdCplCreditChnlConfig1 (
  IN UINT8    Socket,
  IN UINT8    McId,
  IN UINT8    Ch
  )
{
  ASSERT(Ch < sizeof(CmiRdCplCreditChnlConfig1RegAddr) / sizeof(CmiRdCplCreditChnlConfig1RegAddr[0]));
  if (Ch >= sizeof(CmiRdCplCreditChnlConfig1RegAddr) / sizeof(CmiRdCplCreditChnlConfig1RegAddr[0])){
    return 0;
  }

  return UsraCsrRead (Socket, McId, CmiRdCplCreditChnlConfig1RegAddr[Ch]);
}

/**

  Get Mesh2Mem Register Value for CmiRdCplCreditChnlConfig2.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Ch       - Channel ID

  @retval UINT32

**/
UINT32
GetM2mCmiRdCplCreditChnlConfig2 (
  IN UINT8    Socket,
  IN UINT8    McId,
  IN UINT8    Ch
  )
{
  ASSERT(Ch < sizeof(CmiRdCplCreditChnlConfig2RegAddr) / sizeof(CmiRdCplCreditChnlConfig2RegAddr[0]));
  if (Ch >= sizeof(CmiRdCplCreditChnlConfig2RegAddr) / sizeof(CmiRdCplCreditChnlConfig2RegAddr[0])){
    return 0;
  }

  return UsraCsrRead (Socket, McId, CmiRdCplCreditChnlConfig2RegAddr[Ch]);
}

/**

  Get Mesh2Mem Register Value for CmiRspCreditChnlConfig.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Ch       - Channel ID

  @retval UINT32

**/
UINT32
GetM2mCmiRspCreditChnlConfig (
  IN UINT8    Socket,
  IN UINT8    McId,
  IN UINT8    Ch
  )
{
  ASSERT(Ch < sizeof(CmiRspCreditChnlConfigRegAddr) / sizeof(CmiRspCreditChnlConfigRegAddr[0]));
  if (Ch >= sizeof(CmiRspCreditChnlConfigRegAddr) / sizeof(CmiRspCreditChnlConfigRegAddr[0])){
    return 0;
  }

  return UsraCsrRead (Socket, McId, CmiRspCreditChnlConfigRegAddr[Ch]);
}

/**

  Get Mesh2Mem field Value for CmiMiscCfg - Channel IDannle's Credit Init Mask.
  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT8   - CREDIT_INITED_CH_0 = 1 means ch0 is inited, and so on.

**/
UINT8
EFIAPI
GetM2mCmiMiscCfgChCreditInitedMask (
  IN UINT8    Socket,
  IN UINT8    McId
  )
{
  CMIMISCCONFIG_M2MEM_MAIN_STRUCT RegData;
  UINT8                           ChInited;

  ChInited = 0;
  RegData.Data  = UsraCsrRead (Socket, McId, CMIMISCCONFIG_M2MEM_MAIN_REG);
  if (RegData.Bits.chnl0_credits_initialized){
    ChInited |= CREDIT_INITED_CH_0;
  }
  if (RegData.Bits.chnl1_credits_initialized){
    ChInited |= CREDIT_INITED_CH_1;
  }

  return ChInited;
}

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
  )
{
  return UsraCsrRead (Socket, McId, SYSFEATURES0_M2MEM_MAIN_REG);
}

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
  )
{
  SYSFEATURES0_M2MEM_MAIN_STRUCT      SysFeatures0;

  SysFeatures0.Data = UsraCsrRead (Socket, McId, SYSFEATURES0_M2MEM_MAIN_REG);

  return (BOOLEAN)(SysFeatures0.Bits.prefdisable);
}

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
  )
{
  return UsraCsrRead (Socket, McId, MODE_M2MEM_MAIN_REG);
}

/**

  Get Mesh2Mem Register Value for PREFETCH SAD.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mPrefetchSad (
  UINT8               Socket,
  UINT8               McId
  )
{
  //
  // only for NULL stub
  //
  return 0;
}

/**

  Get Mesh2Mem Register Value for Sad2Tad.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mSad2Tad (
  UINT8               Socket,
  UINT8               McId
  )
{
  //
  // only for NULL stub
  //
  return 0;
}

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
  )
{
  return UsraCsrRead (Socket, McId, TIMEOUT_M2MEM_MAIN_REG);
}

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
  )
{
  TIMEOUT_M2MEM_MAIN_STRUCT                       M2mTimeout;

  M2mTimeout.Data = UsraCsrRead (Socket, McId, TIMEOUT_M2MEM_MAIN_REG);

  return (BOOLEAN)M2mTimeout.Bits.timeouten;
}

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
  )
{
  return UsraCsrRead (Socket, McId, DEFEATURES0_M2MEM_MAIN_REG);
}

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
  )
{
  DEFEATURES0_M2MEM_MAIN_STRUCT   M2MemDefeatures0;

  M2MemDefeatures0.Data = UsraCsrRead (Socket, McId,  DEFEATURES0_M2MEM_MAIN_REG);

  return (BOOLEAN)M2MemDefeatures0.Bits.ingbypdis;
}

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
  )
{
  DEFEATURES1_M2MEM_MAIN_STRUCT   M2MemDefeatures1;

  M2MemDefeatures1.Data = UsraCsrRead (Socket, McId,  DEFEATURES1_M2MEM_MAIN_REG);

  return (UINT32)M2MemDefeatures1.Bits.ingeco;
}

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
  )
{
  //
  // only for NULL stub
  //
  return 0;
}

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
  )
{
  return UsraCsrRead (Socket, McId, TOPOLOGY_M2MEM_MAIN_REG);
}

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
  )
{
  return 0;
}

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
  )
{
  return FALSE;
}

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
  )
{
  return 0;
}

/**

  Get Mesh2Mem Register Value for  PREFSADCONFIG.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id

  @retval UINT32

**/
UINT32
EFIAPI
GetM2mPrefSadConfig (
  UINT8               Socket,
  UINT8               McId
  )
{
  //
  // only for NULL stub
  //
  return 0;
}

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
  )
{
  return UsraCsrRead (Socket, McId, CREDITTHRESHOLDS_M2MEM_MAIN_REG);
}

//
// All Sets begin here
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
  )
{
  MODE_M2MEM_MAIN_STRUCT                          ModeM2Mem;
  SYSFEATURES0_M2MEM_MAIN_STRUCT                  SysFeatures0;

  //
  // M2MEM settings
  //
  // * A single socket system is expected to at least configure following
  // * (different from reset values):
  /* *         CHA HA_COH_CFG[Dis_Directory] = 1 (functionality?TBD?)
  *         M2M SysFeatures[FrcDirI]      = 1 (functionality)
  *         M2M SysFeatures[DirWrDisable] = 1 (performance)
  *
  * A multi-socket directory disabled system is expected to at least configure
  * following (different from reset values):
  *         CHA HA_COH_CFG[Dis_Directory] = 1 (functionality)
  *         M2M SysFeatures[D2kDisable]   = 1 (functionality)
  *         M2M SysFeatures[D2cDisable]   = 1 (functionality)
  *         M2M SysFeatures[DirWrDisable] = 1 (performance) */

  SysFeatures0.Data = UsraCsrRead (Socket, McId, SYSFEATURES0_M2MEM_MAIN_REG);
  if (Direct2KtiEn) {
    SysFeatures0.Bits.d2kdisable = 0;
  } else {
    SysFeatures0.Bits.d2kdisable = 1;
  }

  if (Direct2CoreEn) {
    SysFeatures0.Bits.d2cdisable = 0;
  } else {
    SysFeatures0.Bits.d2cdisable = 1;
  }

  if (CheckSysConfig (SYS_CONFIG_1S) && (FpgaList == 0)){
    SysFeatures0.Bits.frcdiri = 1;
  } else {
    SysFeatures0.Bits.frcdiri = 0;

  }

  if (!DirModeEn) {
    SysFeatures0.Bits.dirwrdisable = 1;
  } else {
    SysFeatures0.Bits.dirwrdisable = 0;
  }

  UsraCsrWrite (Socket, McId, SYSFEATURES0_M2MEM_MAIN_REG, SysFeatures0.Data);

  ModeM2Mem.Data = UsraCsrRead (Socket, McId, MODE_M2MEM_MAIN_REG);
  if (IsocEn) {
    ModeM2Mem.Bits.isoch = 1;
  } else {
    ModeM2Mem.Bits.isoch = 0;
  }
  UsraCsrWrite (Socket, McId, MODE_M2MEM_MAIN_REG, ModeM2Mem.Data);

  InitM2mTopology (Socket, McId, 1, 0);
}

/*++
  Update prefetch Sad Config to specify how the memory channel is calculated for an XPT/UPI  prefetch.

  @param  Host               - Pointer to the system Host (root) structure
  @param  Socket             - Socket Id
  @param  M2mem              - M2mem Id
  @param  MemInfo            - Memory Information structure
  @param  PrefetchM2mRegInfo - Pointer to the PREFETCH SAD M2M that is going to be configured
  @param  Config             - Config Id

  @retval None
--*/
VOID
UpdatePrefetchSadConfig (
  UINT8                      Socket,
  UINT8                      M2mem,
  UINT8                      Cluster,
  MEM_INFO                   *MemInfo,
  M2MEM_PREFETCH_SAD_INFO    *PrefetchM2mRegInfo,
  UINT8                      Config
  )
{
  UINT8  MaxIMC;

  MaxIMC = GetMaxImc ();
  //
  // Update prefetch Sad Config to specify how the memory channel is calculated for an XPT/UPI  prefetch.
  //
  if (M2mem < MaxIMC) {
    //
    // prefsadconfiglogical2physicalchnl should sync to channel route table(H0_CH_ROUTE_TABLE_1_CHA_MISC_REG)
    //
    if (MemInfo->Ways[Socket][Cluster] == 3) {
      PrefetchM2mRegInfo->PrefetchSadConfig[Config].Bits.prefsadconfigintlv = 2;
    } else {
      PrefetchM2mRegInfo->PrefetchSadConfig[Config].Bits.prefsadconfigintlv = 0;
    }
    PrefetchM2mRegInfo->PrefetchSadConfig[Config].Bits.prefsadconfigpower2shiftright = 2;
    PrefetchM2mRegInfo->PrefetchSadConfig[Config].Bits.prefsadconfiglogical2physicalchnl = MemInfo->McInfo[Socket][Cluster].PrefLogic2PhyChl;
  }
}

/*++
  Update prefetch Sad Config to specify how the memory channel is calculated for an XPT/UPI  prefetch.

  @param  Host               - Pointer to the system Host (root) structure
  @param  Socket             - Socket Id
  @param  M2mem              - M2mem Id
  @param  PrefetchM2mRegInfo - Pointer to the PREFETCH SAD M2M that is going to be configured

  @retval None
--*/
VOID
ProgramPrefetchSadRegOnM2m (
  UINT8                      Socket,
  UINT8                      M2mem,
  M2MEM_PREFETCH_SAD_INFO    *PrefetchM2mRegInfo
  )
{
  UINT8                      Config;
  UINT8                      Region;

  for (Config = 0; Config < 2; Config++) {
    if (PrefetchM2mRegInfo->PrefetchSadConfig[Config].Data != 0) {
      DEBUG ((DEBUG_ERROR, " M2mem=%x PrefetchM2mRegInfo->PrefetchSadConfig[%x].Data=%x\n", M2mem, Config, PrefetchM2mRegInfo->PrefetchSadConfig[Config].Data));
      UsraCsrWrite (Socket, M2mem, PrefechSadConfigRegTable[Config], PrefetchM2mRegInfo->PrefetchSadConfig[Config].Data);
    }
  }

  for (Region = 0; Region < 3; Region++) {
    if (PrefetchM2mRegInfo->PrefetchSadLimitAddr[Region].Data != 0) {
      UsraCsrWrite (Socket, M2mem, PrefechSadStartAddrRegTable[Region], PrefetchM2mRegInfo->PrefetchSadStartAddr[Region].Data);
      UsraCsrWrite (Socket, M2mem, PrefechSadLimitAddrRegTable[Region], PrefetchM2mRegInfo->PrefetchSadLimitAddr[Region].Data);
    }
  }

  //
  // prefetch enabled
  //
  SetM2mSysFeatures0PreftchDis (Socket, M2mem, FALSE);
}

/*++
  Program Prefetech M2MEM registers for XPT and KTI prefetech

  @param  Socket                -  Socket Id
  @param  UmaBasedClustering    -  UMA Based clustering (0 = disable, 2 = Hemi, 4 = Quad)
  @param  MemInfo               -  Memory Information structure.

  @retval None
--*/
VOID
EFIAPI
SetM2mPrefetchConfigs (
  IN UINT8                      Socket,
  IN UINT8                      UmaBasedClustering,
  IN VOID                       *MemInfoKti
  )
{
  UINT8                                    Ctr;
  UINT8                                    Cluster;
  UINT8                                    PrefSadRegion;
  UINT8                                    PrefetchSadUseIntlv;
  UINT8                                    MaxIMC;
  UINT16                                   TolmLimit;
  M2MEM_PREFETCH_SAD_INFO                  PrefetchM2mRegInfo;
  MEM_INFO                                 *MemInfo;

  MaxIMC = GetMaxImc ();
  MemInfo = (MEM_INFO *)MemInfoKti;
  TolmLimit = GetKtiTolmLimit ();

  //
  // There are 3 prefetch SAD regions and 2 Prefetch SAD config.
  // If no hbm exists (DDR4 1LM, DDR4 NM$ 2LM) , we allows different MC has differnt channel bit mapping (i.e. we will use 2 PrefectchSad Config to specify channel bit mapping)
  // But if "1LM hbm and 1LM DDR4", we cannot allows to have different channel bit mapping because it will need 3 prefetchSad config.
  //
  for (Ctr = 0; Ctr < MaxIMC; Ctr++) {
    if ((MemInfo->imcBitmap[Socket][MC_TECH_DDR] & (1<< Ctr)) == 0) {
      continue;
    }

    Cluster = 0;
    if (UmaBasedClustering != 0) {
      //
      // MemInfo SncInfo[Socket][Cluster] is based on SNC clusters, with separate prefetch sad regions per clusters.
      // UMA Based clustering has one prefetch sad region for all clusters
      //
      Cluster = 0;
    } else {
      if (MaxIMC == 4) {
        //
        // When there are 4 IMC, it can support the SNC4, SNC2
        //
        if (MemInfo->SncInfo[Socket].NumOfCluster == 1) {
          Cluster = 0;
        } if (MemInfo->SncInfo[Socket].NumOfCluster == 2) {
          if (Ctr >= 2) {
            Cluster = 1;
          } else {
            Cluster = 0;
          }
        } else if (MemInfo->SncInfo[Socket].NumOfCluster == 4) {
          Cluster = Ctr;
        }
      } else if (MaxIMC == 2) {
        //
        // When there are 2 IMCs, it can only support SNC2
        //
        if (MemInfo->SncInfo[Socket].NumOfCluster == 1) {
          Cluster = 0;
        } if (MemInfo->SncInfo[Socket].NumOfCluster == 2) {
          Cluster = Ctr;
        }
      }
    }

    PrefSadRegion = 0;
    PrefetchSadUseIntlv = 0;
    ZeroMem ((UINT8 *)&PrefetchM2mRegInfo, sizeof (PrefetchM2mRegInfo));
    //
    // Update prefetch Sad Config to specify how the memory channel is calculated for an XPT/UPI  prefetch.
    //
    UpdatePrefetchSadConfig (Socket, Ctr, Cluster, MemInfo, &PrefetchM2mRegInfo, PrefetchSadUseIntlv);

    //
    // Update prefetch Sad Start & Limit address, andd select which Config will be used
    //
    PrefetchM2mRegInfo.PrefetchSadStartAddr[PrefSadRegion].Bits.prefetchsaduseintlv = PrefetchSadUseIntlv;
    PrefetchM2mRegInfo.PrefetchSadStartAddr[PrefSadRegion].Bits.prefetchsadregionenable = 1;
    if (MemInfo->McInfo[Socket][Cluster].MemBase == 0) {
      if (MemInfo->McInfo[Socket][Cluster].PrefetchRange[0].Base == 0) {
        // 0 -> TolmLimit
        PrefetchM2mRegInfo.PrefetchSadStartAddr[PrefSadRegion].Bits.prefetchsadstartaddr = 0;
        PrefetchM2mRegInfo.PrefetchSadLimitAddr[PrefSadRegion].Bits.prefetchsadlimitaddr = (UINT32)(TolmLimit);

        // 4G -> MemLimit
        PrefSadRegion++;
        PrefetchM2mRegInfo.PrefetchSadStartAddr[PrefSadRegion].Bits.prefetchsaduseintlv = PrefetchSadUseIntlv;
        PrefetchM2mRegInfo.PrefetchSadStartAddr[PrefSadRegion].Bits.prefetchsadstartaddr = FOUR_GB_MEM;
        PrefetchM2mRegInfo.PrefetchSadLimitAddr[PrefSadRegion].Bits.prefetchsadlimitaddr = MemInfo->McInfo[Socket][Cluster].PrefetchRange[0].Limit;
        PrefetchM2mRegInfo.PrefetchSadStartAddr[PrefSadRegion].Bits.prefetchsadregionenable = 1;
      } else { // prefetch base not start from 0, it is above 4G
        PrefetchM2mRegInfo.PrefetchSadStartAddr[PrefSadRegion].Bits.prefetchsadstartaddr = MemInfo->McInfo[Socket][Cluster].PrefetchRange[0].Base;
        PrefetchM2mRegInfo.PrefetchSadLimitAddr[PrefSadRegion].Bits.prefetchsadlimitaddr = MemInfo->McInfo[Socket][Cluster].PrefetchRange[0].Limit;
      }

      if (MemInfo->McInfo[Socket][Cluster].PrefetchRange[1].Enabled) {
        if (MemInfo->McInfo[Socket][Cluster].PrefetchRange[0].Limit <
            MemInfo->McInfo[Socket][Cluster].PrefetchRange[1].Base) {
          PrefSadRegion++;
          PrefetchM2mRegInfo.PrefetchSadStartAddr[PrefSadRegion].Bits.prefetchsaduseintlv  = PrefetchSadUseIntlv;
          PrefetchM2mRegInfo.PrefetchSadStartAddr[PrefSadRegion].Bits.prefetchsadstartaddr = MemInfo->McInfo[Socket][Cluster].PrefetchRange[1].Base;
          PrefetchM2mRegInfo.PrefetchSadLimitAddr[PrefSadRegion].Bits.prefetchsadlimitaddr = MemInfo->McInfo[Socket][Cluster].PrefetchRange[1].Limit;
          PrefetchM2mRegInfo.PrefetchSadStartAddr[PrefSadRegion].Bits.prefetchsadregionenable = 1;
        } else {
          PrefetchM2mRegInfo.PrefetchSadLimitAddr[PrefSadRegion].Bits.prefetchsadlimitaddr = MemInfo->McInfo[Socket][Cluster].PrefetchRange[1].Limit;
        }
      }
    } else {
      // 1LM + 2LM mixed mode
      if (MemInfo->McInfo[Socket][Cluster].PrefetchRange[0].Enabled &&
          MemInfo->McInfo[Socket][Cluster].PrefetchRange[1].Enabled) {
        //
        // If there is gap between 1LM and 2LM (because of address alignment), use 2 Prefetch SAD
        // ------ 2LM Limit
        // | 2LM
        // ------ 2LM Base
        // | NXM
        // ------ 1LM Limit
        // | 1LM
        // -------1LM Base
        //
        if (MemInfo->McInfo[Socket][Cluster].PrefetchRange[0].Limit <
            MemInfo->McInfo[Socket][Cluster].PrefetchRange[1].Base) {
          PrefetchM2mRegInfo.PrefetchSadStartAddr[PrefSadRegion].Bits.prefetchsadstartaddr = MemInfo->McInfo[Socket][Cluster].PrefetchRange[0].Base;
          PrefetchM2mRegInfo.PrefetchSadLimitAddr[PrefSadRegion].Bits.prefetchsadlimitaddr = MemInfo->McInfo[Socket][Cluster].PrefetchRange[0].Limit;
          PrefSadRegion++;

          PrefetchM2mRegInfo.PrefetchSadStartAddr[PrefSadRegion].Bits.prefetchsaduseintlv = PrefetchSadUseIntlv;
          PrefetchM2mRegInfo.PrefetchSadStartAddr[PrefSadRegion].Bits.prefetchsadregionenable = 1;
          PrefetchM2mRegInfo.PrefetchSadStartAddr[PrefSadRegion].Bits.prefetchsadstartaddr = MemInfo->McInfo[Socket][Cluster].PrefetchRange[1].Base;
          PrefetchM2mRegInfo.PrefetchSadLimitAddr[PrefSadRegion].Bits.prefetchsadlimitaddr = MemInfo->McInfo[Socket][Cluster].PrefetchRange[1].Limit;
        } else {
          PrefetchM2mRegInfo.PrefetchSadStartAddr[PrefSadRegion].Bits.prefetchsadstartaddr = MemInfo->McInfo[Socket][Cluster].PrefetchRange[0].Base;
          PrefetchM2mRegInfo.PrefetchSadLimitAddr[PrefSadRegion].Bits.prefetchsadlimitaddr = MemInfo->McInfo[Socket][Cluster].PrefetchRange[1].Limit;
        }
      } else { // 1LM or 2LM enabled, only range[0] is enabled
        PrefetchM2mRegInfo.PrefetchSadStartAddr[PrefSadRegion].Bits.prefetchsadstartaddr = MemInfo->McInfo[Socket][Cluster].PrefetchRange[0].Base;
        PrefetchM2mRegInfo.PrefetchSadLimitAddr[PrefSadRegion].Bits.prefetchsadlimitaddr = MemInfo->McInfo[Socket][Cluster].PrefetchRange[0].Limit;
      }
    }

    ProgramPrefetchSadRegOnM2m (Socket, Ctr, &PrefetchM2mRegInfo);
  } // Mc loop
}

/**

  SetM2mCmiEgressCreditChnlConfig.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Ch       - Channel ID
  @param Value    - Value

  @retval N/A

**/
VOID
SetM2mCmiEgressCreditChnlConfig (
  IN UINT8    Socket,
  IN UINT8    McId,
  IN UINT8    Ch,
  IN UINT32   Value
  )
{
  return;
}

/**

  SetM2mCmiEgressCreditChnlConfigDone.

  @param Socket     - Socket ID
  @param McId       - Mc Controller Id
  @param Ch         - Channel ID
  @param ConfigDone - ConfigDone

  @retval N/A

**/
VOID
EFIAPI
SetM2mCmiEgressCreditChnlConfigDone (
  IN UINT8    Socket,
  IN UINT8    McId,
  IN UINT8    Ch,
  IN BOOLEAN  ConfigDone
  )
{
  return;
}

/**

  SetM2mCmiRdCplCreditChnlConfig1.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Ch       - Channel ID
  @param Value    - Value

  @retval N/A

**/
VOID
SetM2mCmiRdCplCreditChnlConfig1 (
  IN UINT8    Socket,
  IN UINT8    McId,
  IN UINT8    Ch,
  IN UINT32   Value
  )
{
  UsraCsrWrite (Socket, McId, CmiRdCplCreditChnlConfig1RegAddr[Ch], Value);
}

/**

  SetM2mCmiRdCplCreditChnlConfig2.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Ch       - Channel ID
  @param Value    - Value

  @retval N/A

**/
VOID
SetM2mCmiRdCplCreditChnlConfig2 (
  IN UINT8    Socket,
  IN UINT8    McId,
  IN UINT8    Ch,
  IN UINT32   Value
  )
{
  UsraCsrWrite (Socket, McId, CmiRdCplCreditChnlConfig2RegAddr[Ch], Value);
}

/**

  SetM2mCmiRspCreditChnlConfig.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param Ch       - Channel ID
  @param Value    - Value

  @retval N/A

**/
VOID
SetM2mCmiRspCreditChnlConfig (
  IN UINT8    Socket,
  IN UINT8    McId,
  IN UINT8    Ch,
  IN UINT32   Value
  )
{
  ASSERT(Ch < sizeof(CmiRspCreditChnlConfigRegAddr) / sizeof(CmiRspCreditChnlConfigRegAddr[0]));
  if (Ch >= sizeof(CmiRspCreditChnlConfigRegAddr) / sizeof(CmiRspCreditChnlConfigRegAddr[0])){
    return;
  }

  UsraCsrWrite (Socket, McId, CmiRspCreditChnlConfigRegAddr[Ch], Value);
}

/**

  SetM2mCmiMiscConfigDoneAllCh.

  @param Socket       - Socket ID
  @param McId         - Mc Controller Id
  @param ConfigDone   - ConfigDone

  @retval N/A

**/
VOID
EFIAPI
SetM2mCmiMiscConfigDoneAllCh (
  IN UINT8    Socket,
  IN UINT8    McId,
  IN BOOLEAN  ConfigDone
  )
{
  CMIMISCCONFIG_M2MEM_MAIN_STRUCT RegData;
  UINT8 Ch = 0;
  UINT8 ChId = 0;
  UINT8 NumChPerMc;

  struct channelNvram (*ChannelNvList) [MAX_CH];
  PSYSHOST Host = GetSysHostPointer ();
  ChannelNvList = (&Host->nvram.mem.socket[Socket].channelList);

  NumChPerMc = GetNumChannelPerMc ();

  RegData.Data = UsraCsrRead (Socket, McId, CMIMISCCONFIG_M2MEM_MAIN_REG);
  for (ChId = 0; ChId < NumChPerMc; ChId++) {
    Ch = (McId * NumChPerMc) + ChId;
      if ((*ChannelNvList)[Ch].enabled == 1) {
        if (ChId == 0) {
          RegData.Bits.chnl0_credit_config_done = (ConfigDone ? 1 : 0);
        } else if (ChId == 1) {
          RegData.Bits.chnl1_credit_config_done = (ConfigDone ? 1 : 0);
        }
      }
    UsraCsrWrite (Socket, McId, CMIMISCCONFIG_M2MEM_MAIN_REG, RegData.Data);
  }
}

/**

  SetM2mSysFeatures0PreftchDis.

  @param Socket            - Socket ID
  @param McId              - Mc Controller Id
  @param DisablePrefetch   - DisablePrefetch

  @retval N/A

**/
VOID
EFIAPI
SetM2mSysFeatures0PreftchDis (
  IN UINT8         Socket,
  IN UINT8         McId,
  IN BOOLEAN       DisablePrefetch
  )
{
  SYSFEATURES0_M2MEM_MAIN_STRUCT   SysFeatures0;

  SysFeatures0.Data = UsraCsrRead (Socket, McId, SYSFEATURES0_M2MEM_MAIN_REG);
  SysFeatures0.Bits.prefdisable = DisablePrefetch;
  UsraCsrWrite (Socket, McId, SYSFEATURES0_M2MEM_MAIN_REG, SysFeatures0.Data);
}

/**

  SetM2mSysFeatures0FarMcMode.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param FmcMode  - FmcMode

  @retval N/A

**/
VOID
EFIAPI
SetM2mSysFeatures0FarMcMode (
  IN UINT8         Socket,
  IN UINT8         McId,
  IN UINT8         FmcMode
  )
{
  MODE_M2MEM_MAIN_STRUCT                MeshCrMode;

  MeshCrMode.Data = UsraCsrRead (Socket, McId, MODE_M2MEM_MAIN_REG);
  if (MeshCrMode.Bits.fmcmode != FmcMode) {
    MeshCrMode.Bits.fmcmode = FmcMode;
    UsraCsrWrite (Socket, McId, MODE_M2MEM_MAIN_REG, MeshCrMode.Data);
  }
}

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
  )
{
  MODE_M2MEM_MAIN_STRUCT                MeshCrMode;

  MeshCrMode.Data = UsraCsrRead (Socket, McId, MODE_M2MEM_MAIN_REG);

  return MeshCrMode.Bits.fmcmode;
}

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
  )
{
  SYSFEATURES0_M2MEM_MAIN_STRUCT   SysFeatures0;

  SysFeatures0.Data = UsraCsrRead (Socket, McId, SYSFEATURES0_M2MEM_MAIN_REG);
  SysFeatures0.Bits.disabledir4appdirect = DisableDir4AppDirect;
  UsraCsrWrite (Socket, McId, SYSFEATURES0_M2MEM_MAIN_REG, SysFeatures0.Data);
}

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
  )
{
  DEFEATURES0_M2MEM_MAIN_STRUCT  Defeatures0Reg;

  Defeatures0Reg.Data = UsraCsrRead (SocketId, McId, DEFEATURES0_M2MEM_MAIN_REG);
  Defeatures0Reg.Bits.allowdirupdtoblock = AllowDirUpdToBlock;
  UsraCsrWrite (SocketId, McId, DEFEATURES0_M2MEM_MAIN_REG, Defeatures0Reg.Data);
}

/**

  SetM2mModeMmap.

  @param[in] Socket          Socket ID
  @param[in] McId            Mc Controller Id
  @param[in] Nmcaching       Nmcaching
  @param[in] Pmem            Pmem
  @param[in] Blockregion     Blockregion
  @param[in] IsXTileCaching  Flag indicating MC is caching x-tile for 10nm wave 3 use onlyy

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
  IN BOOLEAN             IsXTileCaching
  )
{
  MODE_M2MEM_MAIN_STRUCT                MeshCrMode;

  MeshCrMode.Data = UsraCsrRead (Socket, McId, MODE_M2MEM_MAIN_REG);
  MeshCrMode.Bits.nmcaching = Nmcaching;
  MeshCrMode.Bits.pmem = Pmem;
  MeshCrMode.Bits.blockregion = Blockregion;
  if (Nmcaching) {
    if (IsXTileCaching) {
      MeshCrMode.Bits.fmcmode = 0;
      MeshCrMode.Bits.nmcmode = 1;
    } else {
      MeshCrMode.Bits.fmcmode = 1;
      MeshCrMode.Bits.nmcmode = 0;
    }
  }

  UsraCsrWrite (Socket, McId, MODE_M2MEM_MAIN_REG, MeshCrMode.Data);
}


/**

  Set M2M Fm in same tile configure

  @param[in] SocketId       - Socket ID
  @param[in] McId           - Mc Controller Id
  @param[in] IsXTileCaching - Flag indicating MC is caching x-tile or in-tile.

**/
VOID
EFIAPI
SetM2mFmInSameTile (
  IN UINT8            SocketId,
  IN UINT8            McId,
  IN BOOLEAN          IsXTileCaching
  )
{
  SYSFEATURES0_M2MEM_MAIN_STRUCT        SysFeatures0;

  SysFeatures0.Data = UsraCsrRead (SocketId, McId, SYSFEATURES0_M2MEM_MAIN_REG);

  //
  // In 2LM mode, BIOS need to program this field to indicate in-tile or X-tile 2LM.
  //
  if (IsXTileCaching) {
    SysFeatures0.Bits.fminsametile = 0;
  } else {
    SysFeatures0.Bits.fminsametile = 1;
  }

  UsraCsrWrite (SocketId, McId, SYSFEATURES0_M2MEM_MAIN_REG, SysFeatures0.Data);
}

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
  )
{
  SYSFEATURES0_M2MEM_MAIN_STRUCT        SysFeatures0;

  SysFeatures0.Data = UsraCsrRead (SocketId, McId,  SYSFEATURES0_M2MEM_MAIN_REG);
  if (DdrtExists) {
    SysFeatures0.Bits.ddrt_exists = 1;
  } else {
    SysFeatures0.Bits.ddrt_exists = 0;
  }
  UsraCsrWrite (SocketId, McId, SYSFEATURES0_M2MEM_MAIN_REG, SysFeatures0.Data);
}


/**

  SetM2mPrefetchSadValid.

  @param Socket            - Socket ID
  @param McId              - Mc Controller Id
  @param PfSadEntryValid   - PfSadEntryValid

  @retval N/A

**/
VOID
EFIAPI
SetM2mPrefetchSadValid (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             PfSadEntryValid
  )
{
  //
  // only for NULL stub
  //
  return;
}

/**

  Set prefetch Sad write enable.

  @param Socket        - Socket ID
  @param McId          - Mc Controller Id
  @param Prefsadwren   - enable/disable

  @retval N/A

**/
VOID
EFIAPI
SetM2mPrefetchSadWrEn (
  UINT8               Socket,
  UINT8               McId,
  BOOLEAN             Prefsadwren
  )
{
  //
  // only for NULL stub
  //
  return;
}

/**

  SetM2mPrefetchEntryInterlv.

  @param Socket          - Socket ID
  @param McId            - Mc Controller Id
  @param Prefsadentry    - Prefsadentry
  @param Prefsadwren     - Prefsadwren
  @param Prefsadchnls    - Prefsadchnls
  @param InterleaveGrain - InterleaveGrain

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
  )
{
  //
  // only for NULL stub
  //
  return;
}

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
  )
{
  //
  // only for NULL stub
  //
  return;
}

/**

  SetM2mSad2TadWrEn.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param WrEnable - WrEnable

  @retval N/A

**/
VOID
EFIAPI
SetM2mSad2TadWrEn (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             WrEnable
  )
{
  //
  // only for NULL stub
  //
  return;
}

/**

  SetM2mTimeoutEn.

  @param Socket     - Socket ID
  @param McId       - Mc Controller Id
  @param TimeoutEn  - TimeoutEn

  @retval N/A

**/
VOID
EFIAPI
SetM2mTimeoutEn (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             TimeoutEn
  )
{
  TIMEOUT_M2MEM_MAIN_STRUCT                       M2mTimeout;

  M2mTimeout.Data = UsraCsrRead (Socket, McId, TIMEOUT_M2MEM_MAIN_REG);
  M2mTimeout.Bits.timeouten = TimeoutEn;
  UsraCsrWrite (Socket, McId, TIMEOUT_M2MEM_MAIN_REG, M2mTimeout.Data);
}

/**

  Writes to Mesh2Mem register to set memory channel population

  @param[in]  Socket             - Socket ID number
  @param[in]  McId               - Memory controller ID number
  @param[in]  DdrtChPopulation   - Bit mask indicating which channels have DDRT present
                                     BIT0 = CH0, BIT1 = CH1, etc
                                     1 = DDRT memory present
                                     0 = DDRT memory not present
  @param[in]  Ddr4ChPopulation   - Bit mask indicating which channels have DDR4 present
                                     BIT0 = CH0, BIT1 = CH1, etc
                                     1 = DDR4 memory present
                                     0 = DDR4 memory not present

  @retval N/A

**/
VOID
EFIAPI
SetM2mMemoryChPopulation (
  IN UINT8  Socket,
  IN UINT8  McId,
  IN UINT8  DdrtChPopulation,
  IN UINT8  Ddr4ChPopulation
  )
{

  MEMORY_M2MEM_MAIN_STRUCT                     M2mMem;

  M2mMem.Data = UsraCsrRead (Socket, McId, MEMORY_M2MEM_MAIN_REG);
  M2mMem.Bits.ddrtchnlpopulation = DdrtChPopulation;
  M2mMem.Bits.ddr4chnlpopulation = Ddr4ChPopulation;
  M2mMem.Bits.chnlenabled = DdrtChPopulation | Ddr4ChPopulation;
  UsraCsrWrite (Socket, McId,  MEMORY_M2MEM_MAIN_REG, M2mMem.Data);
}

/**

  SetM2mDefeatures0NormalMode.

  @param Socket   - Socket ID
  @param McId     - Mc Controller Id
  @param *Mode    - *Mode

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures0NormalMode (
  IN UINT8                      Socket,
  IN UINT8                      McId,
  IN M2M_DEFEATURE0_NORMAL_MODE *Mode
  )
{
  DEFEATURES0_M2MEM_MAIN_STRUCT   M2MemDefeatures0;

  M2MemDefeatures0.Data = UsraCsrRead (Socket, McId, DEFEATURES0_M2MEM_MAIN_REG);
  M2MemDefeatures0.Bits.d2k4cisgrdis = Mode->d2k4cisgrdis;
  UsraCsrWrite (Socket, McId, DEFEATURES0_M2MEM_MAIN_REG, M2MemDefeatures0.Data);
}

/**

  SetM2mDefeatures1IngEcoMask.

  @param Socket       - Socket ID
  @param McId         - Mc Controller Id
  @param IngEcoMask   - IngEcoMask

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures1IngEcoMask (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              IngEcoMask
  )
{
  DEFEATURES1_M2MEM_MAIN_STRUCT   M2MemDefeatures1;

  M2MemDefeatures1.Data = UsraCsrRead (Socket, McId,  DEFEATURES1_M2MEM_MAIN_REG);
  M2MemDefeatures1.Bits.ingeco = IngEcoMask;
  UsraCsrWrite (Socket, McId, DEFEATURES1_M2MEM_MAIN_REG, M2MemDefeatures1.Data);
}

/**

  SetM2mWrTrKrAllocPwrThresh.

  @param Socket      - Socket ID
  @param McId        - Mc Controller Id
  @param PwrThresh   - PwrThresh

  @retval N/A

**/
VOID
EFIAPI
SetM2mWrTrKrAllocPwrThresh (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              PwrThresh
  )
{
}


/**
  Set the Mesh2Mem Pad N0 Configs.

  @param[in] Socket      - Socket index
  @param[in] McId        - Memory controller index
  @param[in] ChId        - Channel index on Memory controller
  @param[in] Enable      - Indicate if the PAD should be enabled
  @param[in] Address     - Address specifying the directed pcommit region

  @retval N/A
**/
VOID
EFIAPI
SetM2mPadN0Configs (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               ChId,
  IN BOOLEAN             Enable,
  IN UINT32              Address
  )
{
  PAD0_N0_M2MEM_MAIN_STRUCT PadN0;

  PadN0.Data = UsraCsrRead (Socket, McId, mPadN0M2mRegs[ChId]);
  PadN0.Bits.enable = Enable;
  PadN0.Bits.address = Address;
  UsraCsrWrite (Socket, McId, mPadN0M2mRegs[ChId], PadN0.Data);
  DEBUG ((DEBUG_INFO, "Socket%d:Mc%d PAD%d_N0_M2MEM_MAIN_REG = 0x%08x\n", Socket, McId, ChId, PadN0.Data));
}

/**
  Set the Mesh2Mem Pad N1 Configs.

  @param[in] Socket      - Socket index
  @param[in] McId        - Memory controller index
  @param[in] ChId        - Channel index on Memory controller
  @param[in] Mask        - Mask specifying the directed pcommit region size
  @param[in] Address     - Address specifying the directed pcommit region

  @retval N/A
**/
VOID
EFIAPI
SetM2mPadN1Configs (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               ChId,
  IN UINT32              Mask,
  IN UINT32              Address
  )
{
  PAD0_N1_M2MEM_MAIN_STRUCT PadN1;

  PadN1.Data = UsraCsrRead (Socket, McId, mPadN1M2mRegs[ChId]);
  PadN1.Bits.mask = Mask;
  PadN1.Bits.address = Address;
  UsraCsrWrite (Socket, McId, mPadN1M2mRegs[ChId], PadN1.Data);
  DEBUG ((DEBUG_INFO, "Socket%d:Mc%d PAD%d_N1_M2MEM_MAIN_REG = 0x%08x\n", Socket, McId, ChId, PadN1.Data));
}

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
  )
{
  UPIROUTINGTABLE_N1_M2MEM_MAIN_STRUCT UpiRtTableN1;

  UsraCsrWrite (Socket, McId, UPIROUTINGTABLE_N0_M2MEM_MAIN_REG, Nid2KtiMapLow);
  UpiRtTableN1.Data = UsraCsrRead (Socket, McId, UPIROUTINGTABLE_N1_M2MEM_MAIN_REG);
  UpiRtTableN1.Bits.logicalupitarget = Nid2KtiMapHigh;
  UsraCsrWrite (Socket, McId, UPIROUTINGTABLE_N1_M2MEM_MAIN_REG, UpiRtTableN1.Data);
}

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
  )
{
  UPIROUTINGCONFIG_M2MEM_MAIN_STRUCT UpiRtConfig;

  UpiRtConfig.Data = UsraCsrRead (Socket, McId, UPIROUTINGCONFIG_M2MEM_MAIN_REG);
  UpiRtConfig.Bits.upiinterleavemode  = UpiInterleaveMode;
  UsraCsrWrite (Socket, McId, UPIROUTINGCONFIG_M2MEM_MAIN_REG, UpiRtConfig.Data);
}

/**

  SetM2mNid2D2k2s3KtiEn.

  @param Socket        - Socket ID
  @param McId          - Mc Controller Id
  @param D2k2s3KtiEn   - D2k2s3KtiEn

  @retval N/A

**/
VOID
EFIAPI
SetM2mNid2D2k2s3KtiEn (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             D2k2s3KtiEn
  )
{
}

/**

  SetNid2KtiConfigs.

  @param Socket             - Socket ID
  @param McId               - Mc Controller Id
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
  )
{
}

/**

  SetM2mPrefSadConfig.

  @param Socket             - Socket ID
  @param McId               - Mc Controller Id
  @param FirstPfTadId       - FirstPfTadId
  @param SecondPfTadId      - SecondPfTadId
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
  )
{
  //
  // only for NULL stub
  //
  return;
}
/**

  SetM2mPrefetchTuningConfigs.

  @param Socket                 - Socket ID
  @param McId                   - Mc Controller Id
  @param PfTuningEn             - PfTuningEn
  @param CfgDirUpdpfcameVictDis - CfgDirUpdpfcameVictDis
  @param PrefRpqThreshold       - PrefRpqThreshold
  @param PrefWpqThreshold       - PrefWpqThreshold
  @param PrefKtiCamThreshold    - PrefKtiCamThreshold
  @param PrefXptCamThreshold    - PrefXptCamThreshold

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
  )
{
  //
  // only for NULL stub
  //
  return;
}

/**

  SetM2mCreditThrshConfigs.

  @param Socket                  - Socket ID
  @param McId                    - Mc Controller Id
  @param CritRdThresh            - CritRdThresh
  @param CritWrThresh            - CritWrThresh
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
  )
{
}

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
  )
{
  UINT8                 ChId                            = 0;
  UINT8                 Ch                              = 0;
  UINT8                 IndexVal                        = 0;
  UINT32                CmiRdCplCredit                  = 0;
  UINT32                CmiRspCredit                    = 0;
  UINT32                RegData                         = 0;
  CMI_DEFAULT_REG_VALUE *CmiRdCplCreditChnlConfig1Value = NULL;
  CMI_DEFAULT_REG_VALUE *CmiRdCplCreditChnlConfig2Value = NULL;
  CMI_DEFAULT_REG_VALUE *CmiRspCreditChnlConfigValue    = NULL;
  UINT8                 CmiRdCplCreditChnlConfig1Len    = 0;
  UINT8                 CmiRdCplCreditChnlConfig2Len    = 0;
  UINT8                 CmiRspCreditChnlConfigLen       = 0;
  PSYSHOST              Host;
  struct channelNvram   (*ChannelNvList)[MAX_CH];

  PrintMesh2MemCmiDataVersion ();
  Host = GetSysHostPointer ();
  ChannelNvList = (&Host->nvram.mem.socket[Socket].channelList);

  // Here the "ChId" variable is mapping to channel number within a given MC
  // Config registers for the RdCpl\RspCredit exists one per Channel in MC
  for (ChId = 0; ChId < MAX_MC_CH; ChId++) {
    //Program perf values for M2M  Requestor: M2M->BGF...Starts
    Ch = (McId * MAX_MC_CH) + ChId;
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    GetCmiRdCplCreditChnlConfig1ValueChannelSpr (Socket, Ch, &CmiRdCplCreditChnlConfig1Value, &CmiRdCplCreditChnlConfig1Len);
    GetCmiRdCplCreditChnlConfig2ValueChannelSpr (Socket, Ch, &CmiRdCplCreditChnlConfig2Value, &CmiRdCplCreditChnlConfig2Len);
    GetCmiRspCreditChnlConfigValueChannelSpr (Socket, Ch, &CmiRspCreditChnlConfigValue, &CmiRspCreditChnlConfigLen);

    CmiRdCplCredit = GetM2mCmiRdCplCreditChnlConfig1 (Socket, McId, ChId);
    if (CmiRdCplCreditChnlConfig1Value != NULL) {
      for (IndexVal = 0; IndexVal < CmiRdCplCreditChnlConfig1Len; IndexVal++) {
        RegData = (CmiRdCplCredit >> CmiRdCplCreditChnlConfig1Value[IndexVal].BitStart) & CmiRdCplCreditChnlConfig1Value[IndexVal].AndVal;
        CmiRdCplCredit &= ~(CmiRdCplCreditChnlConfig1Value[IndexVal].AndVal << CmiRdCplCreditChnlConfig1Value[IndexVal].BitStart);
        switch (ProgramableValue[Ch]) {
          case CMI_DDRT_VAL:
            RegData = CmiRdCplCreditChnlConfig1Value[IndexVal].DdrtVal;
            break;
          case CMI_1LM_VAL:
            RegData = CmiRdCplCreditChnlConfig1Value[IndexVal].OneLMVal;
            break;
          case CMI_CR_VAL:
            RegData = CmiRdCplCreditChnlConfig1Value[IndexVal].CRVal;
            break;
          case CMI_1LM_ISOCH_VAL:
            RegData = CmiRdCplCreditChnlConfig1Value[IndexVal].OneLMIsoChVal;
            break;
          case CMI_1LM_2WAY_VAL:
            RegData = CmiRdCplCreditChnlConfig1Value[IndexVal].OneLM2WAYVal;
            break;
          case CMI_CR_2WAY_VAL:
            RegData = CmiRdCplCreditChnlConfig1Value[IndexVal].CR2WAYVal;
            break;
          default:  //Do nothing for default...So that register defaults will be left alone
            break;
        }
        CmiRdCplCredit |=  (RegData << CmiRdCplCreditChnlConfig1Value[IndexVal].BitStart);
      }
    }
    SetM2mCmiRdCplCreditChnlConfig1 (Socket, McId, ChId, CmiRdCplCredit);
  }

  // Here the "ChId" variable is mapping to channel number within a given MC
  // Config registers for the RdCpl\RspCredit exists one per Channel in MC
  if (CmiRdCplCreditChnlConfig2Value != NULL) {
    for (ChId = 0; ChId < MAX_MC_CH; ChId++) {
      //Program perf values for M2M  Requestor: M2M->BGF...Starts
      Ch = (McId * MAX_MC_CH) + ChId;
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }

      CmiRdCplCredit = GetM2mCmiRdCplCreditChnlConfig2 (Socket, McId, ChId);

      for (IndexVal = 0; IndexVal < CmiRdCplCreditChnlConfig2Len; IndexVal++) {
        RegData = (CmiRdCplCredit >> CmiRdCplCreditChnlConfig2Value[IndexVal].BitStart) & CmiRdCplCreditChnlConfig2Value[IndexVal].AndVal;
        CmiRdCplCredit &= ~(CmiRdCplCreditChnlConfig2Value[IndexVal].AndVal << CmiRdCplCreditChnlConfig2Value[IndexVal].BitStart);
        switch (ProgramableValue[Ch]) {
          case CMI_DDRT_VAL:
            RegData = CmiRdCplCreditChnlConfig2Value[IndexVal].DdrtVal;
            break;
          case CMI_1LM_VAL:
            RegData = CmiRdCplCreditChnlConfig2Value[IndexVal].OneLMVal;
            break;
          case CMI_CR_VAL:
            RegData = CmiRdCplCreditChnlConfig2Value[IndexVal].CRVal;
            break;
          case CMI_1LM_ISOCH_VAL:
            RegData = CmiRdCplCreditChnlConfig2Value[IndexVal].OneLMIsoChVal;
            break;
          case CMI_1LM_2WAY_VAL:
            RegData = CmiRdCplCreditChnlConfig2Value[IndexVal].OneLM2WAYVal;
            break;
          case CMI_CR_2WAY_VAL:
            RegData = CmiRdCplCreditChnlConfig2Value[IndexVal].CR2WAYVal;
            break;
          default:  //Do nothing for default...So that register defaults will be left alone
            break;
        }
        CmiRdCplCredit |=  (RegData << CmiRdCplCreditChnlConfig2Value[IndexVal].BitStart);
      }

      SetM2mCmiRdCplCreditChnlConfig2 (Socket, McId, ChId, CmiRdCplCredit);
    }
  }

  if (CmiRspCreditChnlConfigValue != NULL) {
    for (ChId = 0; ChId < MAX_MC_CH; ChId++) {
      Ch = (McId * MAX_MC_CH) + ChId;
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }
      CmiRspCredit = GetM2mCmiRspCreditChnlConfig (Socket, McId, ChId);

      for (IndexVal = 0; IndexVal < CmiRspCreditChnlConfigLen; IndexVal++) {
        RegData = (CmiRspCredit >> CmiRspCreditChnlConfigValue[IndexVal].BitStart) & CmiRspCreditChnlConfigValue[IndexVal].AndVal;
        CmiRspCredit &= ~(CmiRspCreditChnlConfigValue[IndexVal].AndVal << CmiRspCreditChnlConfigValue[IndexVal].BitStart);
        switch (ProgramableValue[Ch]) {
          case CMI_DDRT_VAL:
            RegData = CmiRspCreditChnlConfigValue[IndexVal].DdrtVal;
            break;
          case CMI_1LM_VAL:
            RegData = CmiRspCreditChnlConfigValue[IndexVal].OneLMVal;
              break;
          case CMI_CR_VAL:
            RegData = CmiRspCreditChnlConfigValue[IndexVal].CRVal;
            break;
          case CMI_1LM_ISOCH_VAL:
            RegData = CmiRspCreditChnlConfigValue[IndexVal].OneLMIsoChVal;
            break;
          case CMI_1LM_2WAY_VAL:
            RegData = CmiRspCreditChnlConfigValue[IndexVal].OneLM2WAYVal;
            break;
          case CMI_CR_2WAY_VAL:
            RegData = CmiRspCreditChnlConfigValue[IndexVal].CR2WAYVal;
            break;
          default:  //Do nothing for default...So that register defaults will be left alone
            break;
        }
        CmiRspCredit |= (RegData << CmiRspCreditChnlConfigValue[IndexVal].BitStart);
      }

      SetM2mCmiRspCreditChnlConfig (Socket, McId, ChId, CmiRspCredit);
    }
  }
}

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
  )
{
  DISTRESSSIGNALLING_N0_M2MEM_MAIN_STRUCT    DistressSignallingN0;
  DISTRESSSIGNALLING_N1_M2MEM_MAIN_STRUCT    DistressSignallingN1;
  DISTRESSSIGNALLINGENHANCEMENT_M2MEM_MAIN_STRUCT    DistressSignallingEnhancement;

  DistressSignallingN0.Data = UsraCsrRead (Socket, McId, DISTRESSSIGNALLING_N0_M2MEM_MAIN_REG);
  if (DdrtQosMode == DDRT_QOS_MODE_0) {
    DistressSignallingN0.Bits.distressd2csignalen = 0;
    DistressSignallingN0.Bits.distressdatasignalen = 0;
    DistressSignallingN0.Bits.distresscmpsignalen = 0;
  } else {
    DistressSignallingN0.Bits.distressd2csignalen = 1;
    DistressSignallingN0.Bits.distressdatasignalen = 1;
    DistressSignallingN0.Bits.distresscmpsignalen = 1;
  }
  DistressSignallingN0.Bits.distressdetection2lmonly = 0;
  DistressSignallingN0.Bits.adegressoccupancythresholdlow = 0; // Part 1 (2 bit) of The lower threshold of AD Egress Occupancy(per channel).

  DistressSignallingN0.Bits.localrpqoccupancythresholdlow = 0x46;
  DistressSignallingN0.Bits.localrpqoccupancythresholdhigh = 0x4c;
  DistressSignallingN0.Bits.localwpqoccupancythresholdlow = 0x01;
  DistressSignallingN0.Bits.localwpqoccupancythresholdhigh = 0x02;
  UsraCsrWrite (Socket, McId, DISTRESSSIGNALLING_N0_M2MEM_MAIN_REG, DistressSignallingN0.Data);

  DistressSignallingN1.Data = UsraCsrRead(Socket, McId, DISTRESSSIGNALLING_N1_M2MEM_MAIN_REG);

  if (DdrtQosMode == DDRT_QOS_MODE_0) {
    DistressSignallingN1.Bits.distresscmpsignalen4appdirect = 0;
  } else {
    DistressSignallingN1.Bits.distresscmpsignalen4appdirect = 1;
  }
  DistressSignallingN1.Bits.distressglobalhalfdieselector = DistressSel;
  DistressSignallingN1.Bits.adegressoccupancythresholdhigh = 0;
  DistressSignallingN1.Bits.adegressoccupancythresholdlow = 0; // Part 2 (3 bit) of The lower threshold of AD Egress Occupancy(per channel).
  DistressSignallingN1.Bits.blegressoccupancythresholdhigh = 0;
  DistressSignallingN1.Bits.blegressoccupancythresholdlow = 0;
  UsraCsrWrite(Socket, McId, DISTRESSSIGNALLING_N1_M2MEM_MAIN_REG, DistressSignallingN1.Data);

  SetM2mWrTrKrAllocDdrtThresh (Socket, McId, 0x8);

  DistressSignallingEnhancement.Data = UsraCsrRead (Socket, McId, DISTRESSSIGNALLINGENHANCEMENT_M2MEM_MAIN_REG);
  DistressSignallingEnhancement.Bits.remoterpqoccupancythresholdlow = 0x36;
  DistressSignallingEnhancement.Bits.remoterpqoccupancythresholdhigh = 0x3c;
  DistressSignallingEnhancement.Bits.remotewpqoccupancythresholdlow = 0x01;
  DistressSignallingEnhancement.Bits.remotewpqoccupancythresholdhigh = 0x01;
  UsraCsrWrite (Socket, McId, DISTRESSSIGNALLINGENHANCEMENT_M2MEM_MAIN_REG, DistressSignallingEnhancement.Data);
}

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
  )
{
  return UsraCsrRead (Socket, McId, DISTRESSSIGNALLING_N0_M2MEM_MAIN_REG);
}

/**
  This function programs the Mesh2Mem pcommit registers

  @param[in] Socket          - Socket index
  @param[in] McId            - Memory controller index
  @param[in] Address         - Address array to configure for pcommit

  @return N/A
**/
VOID
EFIAPI
ProgramM2mPcommit (
  IN UINT8  Socket,
  IN UINT8  McId,
  IN UINT64 Address[MAX_MC_CH]
  )
{
  UINT8     ChId;

  for (ChId = 0; ChId < MAX_MC_CH; ChId++) {
    if (Address[ChId] != 0) {
      SetM2mPadN1Configs (Socket, McId, ChId, 0x0F, (UINT32) (RShiftU64 (Address[ChId], 32)));
      SetM2mPadN0Configs (Socket, McId, ChId, TRUE, (UINT32) (RShiftU64 (Address[ChId], 6)));
    }
  }
}

/**
  @brief Program Mesh2mem 2LM Directory Optimization registers.

  @param[in] Socket        - Socket ID
  @param[in] McId          - Mc Controller Id
  @param[in] M2m2lmDirOpt  - Emum value specifying whether to enable or disable the optimization.

  @return VOID.
**/
VOID
EFIAPI
ProgramM2m2lmDirOpt (
  IN UINT8           Socket,
  IN UINT8           McId,
  IN M2MEM_2LMDIROPT M2m2lmDirOpt
  )
{
  DEFEATURES0_M2MEM_MAIN_STRUCT       Defeature0;
  SYSFEATURES0_M2MEM_MAIN_STRUCT      SysFeatures0;

  Defeature0.Data = GetM2mDefeatures0 (Socket, McId);
  SysFeatures0.Data = GetM2mSysFeatures0 (Socket, McId);

  //
  // Perf. Arch and Design recommend BIOS to program the following settings for 2LM Directory Optimization feature:
  // M2M: Defeatures0.NMCleanDis = 0x0 and SysFeatures0.Enable2LMDirOpt = 0x1
  // CHA: SAD.ha_coh_cfg_2.disnmclean = 0x0 (default from SPR B0).
  //
  if (M2m2lmDirOpt == EnableM2m2lmDirOpt) {
    Defeature0.Bits.nmcleandis = 0;
    SysFeatures0.Bits.enable2lmdiropt = 1;
  } else {
    SysFeatures0.Bits.enable2lmdiropt = 0;
  }

  UsraCsrWrite (Socket, McId, DEFEATURES0_M2MEM_MAIN_REG, Defeature0.Data);
  UsraCsrWrite (Socket, McId, SYSFEATURES0_M2MEM_MAIN_REG, SysFeatures0.Data);
}

/**

  SetM2mTimeoutValues.

  @param[in] Socket               - Socket ID
  @param[in] McId                 - Mc Controller Id
  @param[in] TimeOutData          - Pointer to M2MEM_CR_TIMEOUT struct

  @retval N/A

**/
VOID
EFIAPI
SetM2mTimeoutValues (
  IN UINT8            Socket,
  IN UINT8            McId,
  IN M2MEM_CR_TIMEOUT *TimeOutData
  )
{
  return;
}

/**
  This function programs the Mesh2mem biased 2-way near memory cache support register.

  @param[in] ScktId               - Socket index.
  @param[in] McId                 - Memory controller index.
  @param[in] M2M_TWO_WAY_CACHE    - Control data of two way cache.

  @retval N/A
**/
VOID
EFIAPI
SetM2mTwoWayNmCache (
  IN UINT8              ScktId,
  IN UINT8              McId,
  IN M2M_TWO_WAY_CACHE  *M2mTwoWayCache
  )
{
  TWOWAYCACHE_M2MEM_MAIN_STRUCT  TwoWayCacheReg;

  TwoWayCacheReg.Data = UsraCsrRead (ScktId, McId, TWOWAYCACHE_M2MEM_MAIN_REG);

  TwoWayCacheReg.Bits.enable = M2mTwoWayCache->Enable;
  TwoWayCacheReg.Bits.nonpreferredwaymask = M2mTwoWayCache->NonPreferredWayMask;
  TwoWayCacheReg.Bits.preferredreadfirst = M2mTwoWayCache->PreferredReadFirst;
  UsraCsrWrite (ScktId, McId, TWOWAYCACHE_M2MEM_MAIN_REG, TwoWayCacheReg.Data);
}

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
  )
{
  SYSFEATURES0_M2MEM_MAIN_STRUCT SysFeatures0;

  //
  // Allow D2K irrespective of directory state in 2S configs
  //
  SysFeatures0.Data = UsraCsrRead (Socket, McId, SYSFEATURES0_M2MEM_MAIN_REG);

  if ((CheckSysConfig (SYS_CONFIG_2S1L) || CheckSysConfig (SYS_CONFIG_2S2L) || CheckSysConfig (SYS_CONFIG_2S3L) || CheckSysConfig (SYS_CONFIG_2S4L)) &&
      !FpgaPres) {
    if (AtosStatus == 1) {
      SysFeatures0.Bits.cfg2sd2kopten = 0;
    } else {
      SysFeatures0.Bits.cfg2sd2kopten = 1;
    }
  } else {
    SysFeatures0.Bits.cfg2sd2kopten = 0;
  }

  UsraCsrWrite (Socket, McId, SYSFEATURES0_M2MEM_MAIN_REG, SysFeatures0.Data);
}

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
  )
{
  DEFEATURES0_M2MEM_MAIN_STRUCT   M2MemDefeatures0;

  M2MemDefeatures0.Data = UsraCsrRead (SocketId, McId, DEFEATURES0_M2MEM_MAIN_REG);
  M2MemDefeatures0.Bits.wrcmpfromnm = IsXTileCaching;
  UsraCsrWrite (SocketId, McId, DEFEATURES0_M2MEM_MAIN_REG, M2MemDefeatures0.Data);
}

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
  )
{
  DEFEATURES0_M2MEM_MAIN_STRUCT   M2MemDefeatures0;

  if ((SocketId >= MAX_SOCKET) || (McId >= GetMaxImc ())) {
    return;
  }

  M2MemDefeatures0.Data = UsraCsrRead (SocketId, McId, DEFEATURES0_M2MEM_MAIN_REG);
  M2MemDefeatures0.Bits.wciloptdis = WcilOptDis;
  UsraCsrWrite (SocketId, McId, DEFEATURES0_M2MEM_MAIN_REG, M2MemDefeatures0.Data);
}
