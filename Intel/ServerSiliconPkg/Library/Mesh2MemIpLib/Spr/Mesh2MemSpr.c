/** @file
  Mesh2Mem SPR implementation.

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
#include <Library/MemRcLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <KtiSetupDefinitions.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/MemMap2SgxInterface.h>
#include "Include/Mesh2MemIpLibInternal.h"

/**

  SetM2mDefeatures1NormalMode.

  @param[in] Socket                - Socket ID
  @param[in] McId                  - Mc Controller Id
  @param[in] UcOnNmCacheRdUcdata   - UcOnNmCacheRdUcdata
  @param[in] UcOnNmCachePwrUcData  - UcOnNmCachePwrUcData
  @param[in] IngEcoMask            - IngEcoMask
  @param[in] EgEcoMask             - EgEcoMask

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
  )
{
  DEFEATURES1_M2MEM_MAIN_SPRA0_SPRB0_SPRHBM_STRUCT    M2MemDefeatures1A0B0HBM;
  DEFEATURES1_M2MEM_MAIN_SPRC0_SPRMCC_SPRUCC_STRUCT   M2MemDefeatures1C0;

  if (IsCpuAndRevision (CPU_SPRSP, REV_A0) || IsCpuAndRevision (CPU_SPRSP, REV_B0)) {
    M2MemDefeatures1A0B0HBM.Data = UsraCsrRead (Socket, McId,  DEFEATURES1_M2MEM_MAIN_REG);
    M2MemDefeatures1A0B0HBM.Bits.uconnmcacherducdata = UcOnNmCacheRdUcdata;
    M2MemDefeatures1A0B0HBM.Bits.ingeco = IngEcoMask;
    M2MemDefeatures1A0B0HBM.Bits.egreco = EgEcoMask;
    UsraCsrWrite (Socket, McId, DEFEATURES1_M2MEM_MAIN_REG, M2MemDefeatures1A0B0HBM.Data);
  } else {
    M2MemDefeatures1C0.Data = UsraCsrRead (Socket, McId,  DEFEATURES1_M2MEM_MAIN_REG);
    M2MemDefeatures1C0.Bits.uconnmcacherducdata = UcOnNmCacheRdUcdata;
    M2MemDefeatures1C0.Bits.ingeco = IngEcoMask;
    M2MemDefeatures1C0.Bits.egreco = EgEcoMask;
    UsraCsrWrite (Socket, McId, DEFEATURES1_M2MEM_MAIN_REG, M2MemDefeatures1C0.Data);
  }
}

/**

  SetM2mTopologySnc.

  @param[in] Socket         - Socket ID
  @param[in] McId           - Mc Controller Id
  @param[in] SncCluster     - SncCluster
  @param[in] BaseCluster1   - BaseCluster1
  @param[in] BaseCluster2   - BaseCluster2
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
  )
{
  TOPOLOGY_M2MEM_MAIN_STRUCT       TopologyM2Mem;

  TopologyM2Mem.Data = UsraCsrRead (Socket, McId, TOPOLOGY_M2MEM_MAIN_REG);
  TopologyM2Mem.Bits.sncluster     = SncCluster;
  TopologyM2Mem.Bits.basecluster1  = BaseCluster1;
  if (BaseCluster2 != 0) {
    TopologyM2Mem.Bits.basecluster2  = BaseCluster2;
  }
  UsraCsrWrite (Socket, McId, TOPOLOGY_M2MEM_MAIN_REG, TopologyM2Mem.Data);
}

/**

  Initialize topology information in M2Mem.

  @param[in] Socket         - Socket ID
  @param[in] McId           - Mc Controller Id
  @param[in] SncCluster     - SncCluster
  @param[in] BaseCluster1   - BaseCluster1

**/
VOID
InitM2mTopology (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               SncCluster,
  IN UINT32              BaseCluster1
  )
{
  TOPOLOGY_M2MEM_MAIN_STRUCT       TopologyM2Mem;

  //
  // Update NodeId for d2c and PCLS
  //
  TopologyM2Mem.Data = UsraCsrRead (Socket, McId, TOPOLOGY_M2MEM_MAIN_REG);
  TopologyM2Mem.Bits.mynid = Socket;
  TopologyM2Mem.Bits.sncluster = SncCluster;
  TopologyM2Mem.Bits.basecluster1 = BaseCluster1;
  UsraCsrWrite (Socket, McId, TOPOLOGY_M2MEM_MAIN_REG, TopologyM2Mem.Data);
}

/**

  Program M2Mem credits.

  @param[in] Socket            - Socket ID
  @param[in] McId              - MC index
  @param[in] Port              - Port
  @param[in] CrdtType          - Credit type
  @param[in] CrdtCnt           - Credit count
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
  )
{
  CREDITS_M2MEM_MAIN_STRUCT     CreditsM2mem;

  CreditsM2mem.Data = 0;
  CreditsM2mem.Bits.crdttype = (UINT32)(CrdtType | (Port << 4));
  CreditsM2mem.Bits.crdtcnt = (UINT32)CrdtCnt;      // Credit count
  CreditsM2mem.Bits.crdtwren = 0x0;                 // clear write enable bit
  UsraCsrWrite (Socket, McId, CREDITS_M2MEM_MAIN_REG, CreditsM2mem.Data);
  CreditsM2mem.Bits.crdtwren = 0x1;                 // Credit Write Enable
  UsraCsrWrite (Socket, McId, CREDITS_M2MEM_MAIN_REG, CreditsM2mem.Data);
  CreditsM2mem.Bits.crdtwren = 0x0;                 // clear write enable bit
  UsraCsrWrite (Socket, McId, CREDITS_M2MEM_MAIN_REG, CreditsM2mem.Data);
}

/**

  SetM2mCreditWrEn.

  @param[in] Socket       - Socket ID
  @param[in] McId         - Mc Controller Id
  @param[in] CreditWrEn   - CreditWrEn

  @retval N/A

**/
VOID
EFIAPI
SetM2mCreditWrEn (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN BOOLEAN             CreditWrEn
  )
{
  CREDITS_M2MEM_MAIN_STRUCT                     CreditsM2mem;

  CreditsM2mem.Data = UsraCsrRead (Socket, McId,  CREDITS_M2MEM_MAIN_REG);
  CreditsM2mem.Bits.crdtwren = CreditWrEn;
  UsraCsrWrite (Socket, McId, CREDITS_M2MEM_MAIN_REG, CreditsM2mem.Data);
}

/**

  SetM2mCreditConfigs.

  @param[in] Socket       - Socket ID
  @param[in] McId         - Mc Controller Id
  @param[in] CreditWrEn   - CreditWrEn
  @param[in] Credittype   - Credittype
  @param[in] CreditCount  - CreditCount

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
  )
{
  CREDITS_M2MEM_MAIN_STRUCT      CreditsM2mem;

  CreditsM2mem.Data = UsraCsrRead (Socket, McId, CREDITS_M2MEM_MAIN_REG);
  CreditsM2mem.Bits.crdttype = Credittype;
  CreditsM2mem.Bits.crdtwren = CreditWrEn;
  CreditsM2mem.Bits.crdtcnt = CreditCount;
  UsraCsrWrite (Socket, McId, CREDITS_M2MEM_MAIN_REG, CreditsM2mem.Data);
}

/**

  SetM2mDefeatures1EgEcoMask.

  @param[in] Socket      - Socket ID
  @param[in] McId        - Mc Controller Id
  @param[in] EgressEco   - EgressEco

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures1EgEcoMask (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              EgressEco
  )
{
  DEFEATURES1_M2MEM_MAIN_SPRA0_SPRB0_SPRHBM_STRUCT    M2MemDefeatures1A0B0HBM;
  DEFEATURES1_M2MEM_MAIN_SPRC0_SPRMCC_SPRUCC_STRUCT   M2MemDefeatures1C0;

  if (IsCpuAndRevision (CPU_SPRSP, REV_A0) || IsCpuAndRevision (CPU_SPRSP, REV_B0)) {
    M2MemDefeatures1A0B0HBM.Data = UsraCsrRead (Socket, McId,  DEFEATURES1_M2MEM_MAIN_REG);
    M2MemDefeatures1A0B0HBM.Bits.egreco = EgressEco;
    UsraCsrWrite (Socket, McId, DEFEATURES1_M2MEM_MAIN_REG, M2MemDefeatures1A0B0HBM.Data);
  } else {
    M2MemDefeatures1C0.Data = UsraCsrRead (Socket, McId,  DEFEATURES1_M2MEM_MAIN_REG);
    M2MemDefeatures1C0.Bits.egreco = EgressEco;
    UsraCsrWrite (Socket, McId, DEFEATURES1_M2MEM_MAIN_REG, M2MemDefeatures1C0.Data);
  }
}

/**

  Set max number of DDRT writes allowed per channel store tracker.

  @param[in] Socket       - Socket ID
  @param[in] McId         - Mc Controller Id
  @param[in] DdrtThresh   - Max number of DDRT writes allowed per channel store tracker

**/
VOID
SetM2mWrTrKrAllocDdrtThresh (
  IN UINT8                         Socket,
  IN UINT8                         McId,
  IN UINT8                         DdrtThresh
  )
{
  WRTRKRALLOC_M2MEM_MAIN_STRUCT    WrTrkrAlloc;

  WrTrkrAlloc.Data = UsraCsrRead (Socket, McId, WRTRKRALLOC_M2MEM_MAIN_REG);
  WrTrkrAlloc.Bits.wrtrkrddrtthresh = (UINT32) DdrtThresh;
  UsraCsrWrite (Socket, McId, WRTRKRALLOC_M2MEM_MAIN_REG, WrTrkrAlloc.Data);
}

/**

  GetSetM2mSaiRegisters.

  @param[in] Socket        - Socket ID
  @param[in] McId          - Mc Controller Id
  @param[in] SaiRegIndex   - SaiRegIndex
  @param[in] Get           - TRUE: read register into *Value, FALSE: write register using data in *Value
  @param[in] *Value        - *Value: pointer of UINT32, can be input & output

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
  )
{
  UINT32 RegAddress;

  switch (SaiRegIndex) {
    case M2mLbMcheckWRacN0RegIndex:
        RegAddress = M2M_LB_MCHECK_W_RAC_N0_M2MEM_MAIN_REG;
        break;

    case M2mLbMcheckWRacN1RegIndex:
        RegAddress = M2M_LB_MCHECK_W_RAC_N1_M2MEM_MAIN_REG;
        break;

    case M2mLbMcheckWWacN0RegIndex:
        RegAddress = M2M_LB_MCHECK_W_WAC_N0_M2MEM_MAIN_REG;
        break;

    case M2mLbMcheckWWacN1RegIndex:
        RegAddress = M2M_LB_MCHECK_W_WAC_N1_M2MEM_MAIN_REG;
        break;

    case M2mLbMcheckWCpN0RegIndex:
        RegAddress = M2M_LB_MCHECK_W_CP_N0_M2MEM_MAIN_REG;
        break;

    case M2mLbMcheckWCpN1RegIndex:
        RegAddress = M2M_LB_MCHECK_W_CP_N1_M2MEM_MAIN_REG;
        break;
    default:
        RegAddress = 0;
        break;
  }

  if (!RegAddress){
    //
    // Error returning
    //
    return ;
  }

  if (Get) {
    *Value = UsraCsrRead (Socket, McId, RegAddress);
  } else {
    UsraCsrWrite (Socket, McId, RegAddress, *Value);
  }
}

/**

  SetM2mTadConfigs.

  @param[in] Socket   - Socket ID
  @param[in] McId     - Mc Controller Id
  @param[in] *Config  - *Config

  @retval N/A

**/
VOID
EFIAPI
SetM2mTadConfigs (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN SAD_TAD_CONFIG      *Config
  )
{
  TAD_WR_N0_M2MEM_MAIN_STRUCT    MeshN0Tad;
  TAD_WR_N1_M2MEM_MAIN_STRUCT    MeshN1Tad;

  MeshN0Tad.Data = 0;
  MeshN1Tad.Data = 0;

  MeshN0Tad.Bits.tadvld          = Config->SadTadValid;
  MeshN0Tad.Bits.blkvld          = Config->BlockValid;
  MeshN0Tad.Bits.pmemvld         = Config->PmemValid;
  MeshN0Tad.Bits.nonpersistentfm = Config->NonPersistentFm;
  MeshN0Tad.Bits.nmcacheablevld  = Config->NmCacheableValid;
  MeshN0Tad.Bits.ddr4            = Config->Ddr4;
  MeshN0Tad.Bits.mirror          = Config->Mirror;
  MeshN0Tad.Bits.tadid           = Config->SadTadId;

  if ((Config->BlockValid) || (Config->PmemValid)) {
    MeshN0Tad.Bits.ddrtadid     = Config->DdrtTadId;
    //
    // Detect DDRTQosEn
    //
    if (GetDdrtQosMode () == DDRT_QOS_MODE_2) {   //M2M Qos Enable, CHA Qos Enable
      //
      // Program TAD_WR Low Bandwidth enable
      //
      MeshN0Tad.Bits.lowbw = 1;
    } else {
      MeshN0Tad.Bits.lowbw = 0;
    }
  } else {
    MeshN0Tad.Bits.ddrtadid     = Config->Ddr4TadId;
  }

  //
  // Only low 16bit part of AddressLimit is in N0 reg
  //
  MeshN0Tad.Bits.addresslimit   = Config->AddressLimit & 0x3F; //Bits[31:26]
  MeshN0Tad.Bits.tadwren        = Config->SadTadWrEn;

  //
  // Only high 16bit part of AddressLimit is in N1 reg
  //
  MeshN1Tad.Bits.addresslimit   = (Config->AddressLimit) >> 6; //Bits[51:32]

  UsraCsrWrite (Socket, McId, TAD_WR_N1_M2MEM_MAIN_REG, MeshN1Tad.Data);
  UsraCsrWrite (Socket, McId, TAD_WR_N0_M2MEM_MAIN_REG, MeshN0Tad.Data);
}

// ICX only
/**

  SetM2mPrmrrBase.

  @param[in] Socket        - Socket ID
  @param[in] McId          - Mc Controller Id
  @param[in] PrmrrBaseLo   - PrmrrBase Low
  @param[in] PrmrrBaseHi   - PrmrrBase Hi

  @retval N/A

**/
VOID
EFIAPI
SetM2mPrmrrBase (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              PrmrrBaseLo,
  IN UINT32              PrmrrBaseHi
  )
{
  UsraCsrWrite (Socket, McId, M2M_PRMRR_BASE_N0_M2MEM_MAIN_REG, PrmrrBaseLo);
  UsraCsrWrite (Socket, McId, M2M_PRMRR_BASE_N1_M2MEM_MAIN_REG, PrmrrBaseHi);
}

/**

  SetM2mPrmrrMask.

  @param[in] Socket        - Socket ID
  @param[in] McId          - Mc Controller Id
  @param[in] PrmrrMaskLo   - PrmrrMaskLo
  @param[in] PrmrrMaskHi   - PrmrrMaskHi

  @retval N/A

**/
VOID
EFIAPI
SetM2mPrmrrMask (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT32              PrmrrMaskLo,
  IN UINT32              PrmrrMaskHi
  )
{
  UsraCsrWrite (Socket, McId, M2M_PRMRR_MASK_N0_M2MEM_MAIN_REG, PrmrrMaskLo);
  UsraCsrWrite (Socket, McId, M2M_PRMRR_MASK_N1_M2MEM_MAIN_REG, PrmrrMaskHi);
}

/**
  Overwrite M2mem registers, chip specific

  @param[in] Socket - Socket ID

  @retval N/A
**/
VOID
ModifyMesh2MemCsrsSPR (
  IN UINT8 Socket
  )
{
  UINT8                             McIndex;
  UINT8                             MaxImc;
  SYSFEATURES0_M2MEM_MAIN_STRUCT    M2MemSysFeatures0;
  MEMORY_MODE_ATTRIBUTE             MemMode;
  UINT32                            M2MemMode = 0;

  if (!IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    return;
  }

  MaxImc = GetMaxImc ();
  GetMemMode (Socket, &MemMode);

  for (McIndex = 0; McIndex < MaxImc; McIndex++) {
    if (!CheckSocketMcEnabled (Socket, McIndex)) {
      continue;
    }

    M2MemSysFeatures0.Data = UsraCsrRead (Socket, McIndex, SYSFEATURES0_M2MEM_MAIN_REG);
    if ((MemMode & MEMORY_MODE_ATTRIBUTE_2LM_XTILE) != 0) {
      M2MemSysFeatures0.Bits.numegrentries4cisgr = 4;
    } else {
      M2MemSysFeatures0.Bits.numegrentries4cisgr = 0;
    }

    if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_C0)) {
      M2MemMode = GetM2mModeValue (Socket, McIndex);
      //
      // Check not in 1LM Iso Mode. This feature should not be enabled together with ISOCH.
      // If this is disabled, but ISOCH is enabled, then ISOCH transactions will be sent with
      // to CMI with the priority encodings programmed in Sysfeatures0[CMIRd/WrPriority].
      //
      if ((M2MemMode & ISOCHRONOUS_FLOW_ENABLED_BIT) == 0) {
        M2MemSysFeatures0.Bits.cmipriorityen = 1;
      }
    }
    UsraCsrWrite (Socket, McIndex, SYSFEATURES0_M2MEM_MAIN_REG, M2MemSysFeatures0.Data);
  }
}

/**
  Overwrite M2mem registers

  @param[in] Socket - Socket ID

  @retval N/A
**/
VOID
EFIAPI
ModifyMesh2MemCsrsChip (
  IN UINT8 Socket
  )
{
  ModifyMesh2MemCsrsSPR (Socket);
}
