/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation. <BR>

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

// Definition Flag:
//                  1. KTI_SW_SIMULATION -> run with KTIRC Simulation
//                  2. IA32              -> run with IA32 mode


#ifndef _KTI_HOST_H_
#define _KTI_HOST_H_

#include <ReferenceCodeDataTypes.h>
#include <Upi/KtiSi.h>
#include <Upi/KtiDisc.h>
#include "MemHostChipCommon.h"

#pragma pack(1)

typedef INT32  KTI_STATUS;
#define CONST       const
#define STATIC      static
#define VOID        void
#define VOLATILE    volatile
#define KTI_SUCCESS 0
#define KTI_REBOOT  1
#define KTI_SNC_CHANGED 2
#define KTI_IGNORE  3
#define KTI_FAILURE -1

//
// Warning log
//
#define MAX_WARNING_LOGS      16
#define WARNING_LOG_OVERFLOW  - 1

#define SR_LEGACY_BUSCFG_SYNC_CSR BIOSNONSTICKYSCRATCHPAD13_UBOX_MISC_REG
#define BUSCFGSYNC_PBSP_READY     0x0ABCDEF01 // Indicate PBSP ready for programming
#define BUSCFGSYNC_PBSP_GO        0x0ABCDEF02 // Indicate SBSP done, and PBSP should finish enabling
#define BUSCFGSYNC_PBSP_DONE      0x0ABCDEF03 // Indicate PBSP completed programming
#define BUSCFGSYNC_PBSP_CPL       0x000000000 // Release PBSP from Bus programming

typedef enum {
  NORMAL_OPERATION = 0,
  RECOVERY_OPERATION
} SNC_COLDRESET_REGISTER_OPERATION_TYPE;

typedef enum {
  KTI_GROUP = 0,
} GROUP_TYPE;

/*********************************************************
                KTIRC Host Structure Related
*********************************************************/

typedef enum {
  KTI_LINK0 =  0x0,
  KTI_LINK1,
  KTI_LINK2,
  KTI_LINK3,
  KTI_LINK4,
  KTI_LINK5
} KTI_LOGIC_LINK;

typedef enum {
  FULL_SPEED = 0,
  HALF_SPEED
} KTI_LINK_SPEED_TYPE;

//
// LL credit allocation
//
#define  KTI_LL_VNA_NORMAL_SKX       66   // if KTI_LL_VNA_SETUP_MAX requested and Vn1=0
#define  KTI_LL_VNA_NORMAL_SPR       0x8A // if KTI_LL_VNA_SETUP_MAX requested and CpuType == SPR
#define  KTI_LL_VNA_WITH_VN1_SKX     48   // if KTI_LL_VNA_SETUP_MAX requested and Vn1=1
#define  KTI_LL_VNA_MIN              0    // if KTI_LL_VNA_SETUP_MIN requested
#define  KTI_LL_VNA_FGPA_SKX         75   // KTI FPGA LINK VNA credits Maxmize (VN1 disable, VN0.NCS, VN0.NCB and VN0.SNP are disabled)
#define  KTI_LL_VNA_NORMAL_WHR       0x7C // default number of WHR LL VNA credits

#define  KTI_LL_AD_VNA_SPRUCC        56   // AD VNA credit number for UPIMDF port
#define  KTI_LL_AK_VNA_SPRUCC        56   // AK VNA credit number for UPIMDF port
#define  KTI_LL_BL_VNA_SPRUCC        36   // BL VNA credit number for UPIMDF port

#define MSR_BDF_PLA_VMSR_62H         0x0062
#define MSR_IRQ_VMSR_64H             0x0064
#define MSR_PMON_ACC_CTRL            0x1F7F
#define MASK_IRQ_VMSR_IRQ_TH         0x0000001F  // bits [4:0] are R/W
#define MASK_PMON_ACC_CTRL           0x00000001  // bits [0] controls PMON access
#define IRQ_THRESHOLD_2LM_2S2L       0x0008
#define IRQ_THRESHOLD_2LM_2S3L       0x0004

#define IRQ_THRESHOLD_DIS             0x1F
#define IRQ_THRESHOLD_4SRING_DEFAULT  0xA
#define IRQ_THRESHOLD_8S_DEFAULT      0xC
#define RRQ_IRQ_THRESHOLD_DIS         0x1F1F

#if defined(CPX_HOST)
#define RRQ_IRQ_2S4SFC_LOW            0x0B0B                //RRQ=0xB, IRQ=0xB
#define RRQ_IRQ_2S4SFC_MEDIUM         0x080E                //RRQ=0x8, IRQ=0xE
#define RRQ_IRQ_2S4SFC_HIGH           0x0511                //RRQ=0x5, IRQ=0x11
#define RRQ_IRQ_2S4SFC_AUTO           RRQ_IRQ_2S4SFC_MEDIUM
#else
#define RRQ_IRQ_2S4SFC_LOW            0x061F                //RRQ=6, IRQ=0x1F
#define RRQ_IRQ_2S4SFC_MEDIUM         0x071F                //RRQ=7, IRQ=0x1F
#define RRQ_IRQ_2S4SFC_HIGH           0x071F                //RRQ=7, IRQ=0x1F
#define RRQ_IRQ_2S4SFC_AUTO           RRQ_IRQ_2S4SFC_MEDIUM
#endif

#define RRQ_IRQ_4SRING_LOW            0x0704                //RRQ=7, IRQ=0x04
#define RRQ_IRQ_4SRING_MEDIUM         0x0707                //RRQ=7, IRQ=0x07
#define RRQ_IRQ_4SRING_HIGH           0x090A                //RRQ=9, IRQ=0x0A
#define RRQ_IRQ_4SRING_AUTO           RRQ_IRQ_4SRING_MEDIUM

#define RRQ_IRQ_8S_LOW                0x0704                //RRQ=7, IRQ=0x04
#define RRQ_IRQ_8S_MEDIUM             0x0707                //RRQ=7, IRQ=0x07
#define RRQ_IRQ_8S_HIGH               0x080A                //RRQ=8, IRQ=0x0A
#define RRQ_IRQ_8S_AUTO               RRQ_IRQ_8S_MEDIUM

#define RRQ_IRQ_8SDPW_LOW             0x0802                //RRQ=8, IRQ=0x02
#define RRQ_IRQ_8SDPW_MEDIUM          0x080A                //RRQ=8, IRQ=0x0A
#define RRQ_IRQ_8SDPW_HIGH            0x0C0C                //RRQ=0xC, IRQ=0x0C
#define RRQ_IRQ_8SDPW_AUTO            RRQ_IRQ_8SDPW_MEDIUM

//
// loctorem_thresh_norm
//
#define LOCTOREM_THRESH_NORM_DIS                   0x1F
#define LOCTOREM_THRESH_NORM_LOW                   7
#define LOCTOREM_THRESH_NORM_MEDIUM                11
#define LOCTOREM_THRESH_NORM_HIGH                  15

#define LOCTOREM_THRESH_NORM_AUTO                  LOCTOREM_THRESH_NORM_DIS
#define LOCTOREM_THRESH_NORM_4SRING_AUTO           LOCTOREM_THRESH_NORM_MEDIUM
#define LOCTOREM_THRESH_NORM_8S_AUTO               LOCTOREM_THRESH_NORM_LOW

//
// loctorem_thresh_empty
//
#define LOCTOREM_THRESH_EMPTY_DIS                   0x1F
#define LOCTOREM_THRESH_EMPTY_LOW                   11
#define LOCTOREM_THRESH_EMPTY_MEDIUM                15
#define LOCTOREM_THRESH_EMPTY_HIGH                  19

#define LOCTOREM_THRESH_EMPTY_AUTO                  LOCTOREM_THRESH_EMPTY_DIS
#define LOCTOREM_THRESH_EMPTY_4SRING_AUTO           LOCTOREM_THRESH_EMPTY_MEDIUM
#define LOCTOREM_THRESH_EMPTY_8S_AUTO               LOCTOREM_THRESH_EMPTY_LOW

//
// KtiPrefetch or Remote xpt prefetch
//
#define KTI_XPT_PREFETCH_EN                         0
#define REMOTE_XPT_PREFETCH_EN                      1
#define KTI_XPT_PREFETCH_DISABLE                    2
#define REMOTE_XPT_PREFETCH_DISABLE                 3

//
// Definitions to be used in Eparam tables:
//
typedef enum {
  PER_LANES_TXEQ_ENABLED   = 0,         // each lane use different TXEQ value
  ALL_LANES_TXEQ_ENABLED                // all lanes use same TXEQ value
} LANE_TXEQ_TYPE;

//
// Number of Clusters.
//
typedef enum {
  CLUSTER_MODE_1,
  CLUSTER_MODE_2,
  CLUSTER_MODE_3,
  CLUSTER_MODE_4,
} CLUSTER_MODE;

typedef struct {
  BOOLEAN          Enabled;
  UINT32           Base;
  UINT32           Limit;
} PREFETCH_RANGE;

typedef struct {
   UINT8      Enabled;
   UINT32     McMemSize;
   UINT32     ChannelMemSize[MAX_MC_CH];
   UINT32     MemBase;
   UINT32     MemLimit;
   UINT8      ChannelInterBitmap[MAX_IMC];
   UINT32     FirstPfTadID;
   UINT32     SecondPfTadID;
   UINT16     PrefLogic2PhyChl;        // prefsadconfiglogical2physicalchnl
   PREFETCH_RANGE PrefetchRange[2];    // 1LM/2LM/1LM+2LM
} MC_INFO;

typedef struct {
  UINT8      Enabled;
  UINT32     MemBase;
  UINT32     MemLimit;
} CLUSTER_RANGE_IN_LOCAL;

typedef struct {
   UINT8      NumOfMcEnabled;
   UINT8      NumOfHbmioEnabled;
   UINT8      NumOfMcPerCluster[MAX_CLUSTERS];
   UINT8      NumOfChaPerCluster;
   UINT8      NumOfCluster;         //0: N/A  1: 1 cluster  2: 2 clusters  4: 4 clusters
   UINT8      InterleaveEn;         //0: NUMA-non-interleave-cross-MC 1: NUMA interleave-cross-MC (for 1LM/XPT_Prefetch only)
                                    //   If 2LM, it must be NUMA
   UINT8      Interleave_256B;      //0: interleave 64B   1: Interleave 256B  if InterleaveEn = 1
   UINT8      PrefetchEn;           //0: KTI and XPT capability Disabled  1: Enabled
   UINT8      LeftMc;               // MC in left side
   UINT8      RightMc;              // MC in right side
   UINT32     McBitmapPerCluster[MAX_CLUSTERS];
} SNC_INFO;

typedef struct {
   UINT8      Enabled;
   UINT32     MemSize;
   UINT32     MemBase;
   UINT32     MemLimit;
} HBM_INFO;

//
// ICXD_TODO: XPT is required to ICXDE/SNR but for now XPT headers are not available
//            skip for now the programming but it will be fixed in the CRIF ICXDE/SNR update at ww37.
//
typedef struct {
  UINT8    Xptminisadmodeselect;
  UINT8    IntGranularity;
  BOOLEAN  XorDefeature;
  UINT32   MicMode;
  UINT32   XptMcMiniSadMode;
  UINT32   Xpt32EntryPrefetchBase[MAX_SOCKET][MAX_CLUSTERS];
  UINT32   MiniSadTable32Entry[MAX_SOCKET][MAX_MINISAD_32_ENTRY];
  UINT32   MiniSadTable32EntryRegValue[MAX_SOCKET][NUM_OF_REGSTERS_MINISAD_32_ENTRY];
  UINT32   UPIMiniSad[MAX_SOCKET];
  UINT8    UpiMiniSADMode[MAX_SOCKET];
} MINISAD_INFO;

#define KTI_MC0_BITMAP 1
#define KTI_MC1_BITMAP 2
#define KTI_MC2_BITMAP 4
#define KTI_MC3_BITMAP 8

#define KTI_MC0 0
#define KTI_MC1 1
#define KTI_MC2 2
#define KTI_MC3 3

#define SNC_CLUSTER_0  0
#define SNC_CLUSTER_1  1
#define SNC_CLUSTER_2  2
#define SNC_CLUSTER_3  3

#define KTI_MAX_HBM_IO   4

typedef enum {
  MC_TECH_DDR = 0, // for DDR/DDRT
  MC_TECH_HBM,     // for HBMIO
  MAX_MC_TECH_TYPES
} MC_TECH_TYPE;

typedef struct {
  MC_INFO    McInfo[MAX_SOCKET][MAX_CLUSTERS]; // it is only used as 2 clusters for SKX, but for 10nm, it is used up to 4 clusters.
  CLUSTER_RANGE_IN_LOCAL ClusterRngInLocal[MAX_SOCKET][MAX_CLUSTERS];
  HBM_INFO   HbmInfo[MAX_SOCKET][KTI_MAX_HBM_IO];
  SNC_INFO   SncInfo[MAX_SOCKET];
  UINT32     ClusterMemBase[MAX_SOCKET][MAX_CLUSTERS+1];
  UINT8      SocketIdWithFirst4G;
  UINT8      PrefetchAll;
  MINISAD_INFO MiniSadInfo;
  UINT32     imcBitmap[MAX_SOCKET][MAX_MC_TECH_TYPES];
  UINT32     McMirrorBitmap[MAX_SOCKET][MAX_MC_TECH_TYPES];   // bit set for each MC that participates in mirror
  UINT8      Ways[MAX_SOCKET][MAX_CLUSTERS]; // Interleave ways for NM
} MEM_INFO;

typedef enum {
  LCC = 0, // 10c
  MCC,     // 14c
  HCC,     // 22c
  XCC,     // 28c
  MAX_CHOP_TYPES
} PHYSICAL_CHOP;

typedef struct {

  UINT8   DfxSystemWideParmStart;           // This must be the first variable
  UINT8   DfxHaltLinkFailReset;             // 2 - Auto; 1 - Enable; 0 - Disable  (Auto);
  UINT8   DfxKtiMaxInitAbort;               // 0 - Disable; 1 - Enable; 2 - AUTO (default)
  UINT8   DfxLlcShareDrdCrd;                // Enable migration from SF to LLC and to leave shared lines in the LLC for Drd and Crd: 1 - Enable; 1 - Disable; 2 - Auto (default)
  UINT8   DfxBiasFwdMode;                   // 0 - Mode 0 (Fwd only when Hom != Req); 1 - Mode 1 (Fwd when Hom != Req & Hom != Local); 2 - Mode 2 (Disable Bias Fwd) ..  5 - Auto (Default)
  UINT8   DfxSnoopFanoutEn;                 // Snoop fanout enable 0: disable 1: enable 2 - Auto (default)
  UINT8   DfxHitMeEn;                       // CHA HitME$ Enable: 1 - Enable; 0 - Disable; 2 - Auto (Default)
  UINT8   DfxFrcfwdinv;                     // Enable alias all conflict flows to FwdInvItoE behaviour: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8   DfxDbpEnable;                     // Dbp Enable : 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8   DfxCleanEvictAlwaysLive;          // Clean Evict Always Live: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8   DfxModifiedEvictAlwaysLive;       // Modified Evict Always Live: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8   DfxOsbEn;                         // OSB Enable: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8   DfxHitMeRfoDirsEn;                // Enable HitME DIR=S RFO optimization: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8   DfxGateOsbIodcAllocEn;            // When OSB indicates that there aren't enough snoop credits don't allocate IODC entry: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8   DfxDualLinksInterleavingMode;     // In 2S 2KTI can: 2 - Auto - do nothing (default); 1 - use legacy CHA interleaving (disable SNC, turn off XOR interleave); 0 - disable D2C
  UINT8   DfxSystemDegradeMode;             // 0 - Degrade to 1S; 1 - Degarde to supported topology (default); 2 - Leave the topology as is.
  UINT8   DfxVn1En;                         // VN1 enable 0: disable 1: enable 2 - Auto (default) (8S parts only)
  UINT8   DfxD2cEn;                         // Direct To Core enable: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8   DfxD2kEn;                         // Direct To Kti enable: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8   DfxLockMaster;                    // NodeId of the lock master: 0 - Socket0; 1 - Socket1; 2 - Socket2; 3 - Socket3; 4 - Auto (default)
  UINT8   DfxOsbLocRd;                      // OSB Local Rd Enable: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8   DfxOsbLocRdCur;                   // OSB Local RdCur Enable: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8   DfxOsbRmtRd;                      // OSB Remote Rd Enable: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8   DfxM3KtiCountMismatchEn;          // Allow M3KTI count mismatch between SBSP and other sockets: 1 - Enable; 0 - Disable (default)
  UINT8   DfxSnoopFanoutChaInterleaveEn;    // Balancing snoop fanout via CHA Interleave. 0 - Disable, 1 - Enable, 2 - AUTO.
  UINT8   DfxXptFifoEnabledCredit;          // XPT FIFO enabled credit number
  UINT8   DfxClockModulationEn;             // Clock Modulation Enabled. 0 - Disable, 1 - Enable, 2 - AUTO.
  UINT8   DfxMdfisAdvancedTrainingEn;       // MDFIS Advanced training Enabled. 0 - Disable, 1 - Enable, 2 - AUTO.
  UINT8   DfxCxlSecLvl;                     // CXL Security Level: 0 - Trusted CXL Device, 1 - Trusted for Device Attached Memory Range Only,
                                            // 2 - Untrusted IAL Device, 3 - AUTO (default)
  UINT8   DfxCxlStcge;                      // CXL Arb-Mux Side Clock Trunk Clock Gating Enable: 1 - Enable, 0 - Disable, 2 - AUTO
  UINT8   DfxCxlSdcge;                      // CXL Arb-Mux Side Clock Dynamic Clock Gating Enable: 1 - Enable, 0 - Disable, 2 - AUTO
  UINT8   DfxCxlDlcge;                      // CXL Arb-Mux Dynamic Link Clock Gating Enable: 1 - Enable, 0 - Disable, 2 - AUTO
  UINT8   DfxCxlLtcge;                      // CXL Arb-Mux Link Clock Trunk Clock Gating Enable: 1 - Enable, 0 - Disable, 2 - AUTO
  UINT8   DfxCxlVid;                        // CXL Set Vid to 0x198: 0 - Disable, 1 - Enable, 2 - AUTO.
  UINT8   DfxPmonConfig;                    // 1 - Uncore,MC,PCIe only, 2 - Uncore,MC,PCIe & M2IOSF,  3 - Auto (default), 0 - Disable
  UINT8   DfxM2IosfPmonAccessControl;       // 0 - M2IOSF PMON cannot be accessed, 1 - M2IOSF PMON can be accessed,  2 - Auto (default)
  UINT8   DfxIioDfxEnabled;                 // DFX boot in IIO enabled
  UINT32  DfxIioStackEn;                    // MS2IOSF strap override: Auto(default),
                                            // Bitmap - order must match CAPID. 1 - behaves as defined by internal fuse 0 - given ms2iosf is disabled.
  UINT8   DfxHqmEn;                        // HQM  strap override: 1 - Enable HQM; 0 - Disable HQM; 2 - Auto (default) do not modify strap
  UINT8   DfxSystemWideParmEnd;             // This must be the last variable

} KTI_DFX_PARM;

//
// DFX Link layer Options (per Port)
//
typedef struct {
  UINT32  DfxCrcMode:2;           // CRC_MODE_16BIT, CRC_MODE_ROLLING_32BIT or CRC_MODE_AUTO
  UINT32  DfxL0pEnable:2;         // Value to determine if this port of the link may enter L0p, but both sides must agree to enter L0p.  Default uses system wide setting
  UINT32  DfxL1Enable:2;          // Value to determine if this port of the link may enter L1, but both sides must agree to enter L1.  Default uses system wide setting
  UINT32  Rsvd1 : 26;
} KTI_DFX_CPU_LINK_SETTING;

//
// DFX Phy Layer Options (per Port) - Currently unused; A place holder for future extension
//
typedef struct {
  UINT8  DfxKtiFailoverEn : 2; // 0 - Disable; 1 - Enable; 3 - Auto (Default)
  UINT8  Rsvd1 : 6;
} KTI_DFX_CPU_PHY_SETTING;

//
// Per CPU Port DFX settings
//
typedef struct {
  KTI_DFX_CPU_LINK_SETTING Link[MAX_FW_KTI_PORTS];
  KTI_DFX_CPU_PHY_SETTING  Phy[MAX_FW_KTI_PORTS];
} KTI_DFX_CPU_SETTING;

//
// PHY settings that are system dependent.   Need 1 of these for each socket/link/freq.
//

typedef struct {
  UINT8  SocketID;
  UINT8  AllLanesUseSameTxeq;
  UINT8  Freq;
  UINT32 Link;
  UINT32 TXEQL[20];
  UINT32 CTLEPEAK[5];
} PER_LANE_EPARAM_LINK_INFO;

//
// This is for full speed mode, all lanes have the same TXEQ setting
//
typedef struct {
  UINT8  SocketID;
  UINT8  Freq;
  UINT32 Link;
  UINT32 AllLanesTXEQ;
  UINT8  CTLEPEAK;
} ALL_LANES_EPARAM_LINK_INFO;

//
// This is for KTI EParam data, include full speed all lanes and per lane info.
//
typedef struct {
  UINT32 CountOfPerLaneEparamLink;
  UINT32 CountOfAllLanesEparamLink;
  PER_LANE_EPARAM_LINK_INFO PerLaneEparamLinkInfo[MAX_SOCKET*MAX_FW_KTI_PORTS];
  ALL_LANES_EPARAM_LINK_INFO AllLanesEparamLinkInfo[MAX_SOCKET*MAX_FW_KTI_PORTS];
} KTI_EPARAM_LINK_INFO;

#define ADAPTIVE_CTLE 0x3f
#define PER_LANE_ADAPTIVE_CTLE 0X3f3f3f3f

#define MAX_SET_OF_PRESET_COEFF     11

//
// SPR Preset All lane tx coefficient table --- used for PH_TCC
//
typedef struct {
  UINT32  AllLanesPresetCoeff;
} ALL_LANES_PRESET_COEFF_INFO;

typedef struct {
  UINT8   SocketID;
  UINT32  Link;
  BOOLEAN RedriverStatus;
  ALL_LANES_PRESET_COEFF_INFO LinkAllLanesPresetCoeff[KTI_SPEEDS_SUPPORTED];
} ALL_LANES_PRESET_COEFF_LINK_INFO;

//
// SPR preset per lane tx coefficient table
//   AllLaneSafeCoeffFlag --- TRUE: indicate the AllLanePresetCoeff is valid, otherwise not valid
//   AllLanePresetCoeff --- PH_TCC for all lanes
//   PerLanePresetCoeffIndex --- PH_TCL[Index] (as eparam)
//
typedef struct {
  BOOLEAN AllLanesSafeCoeffFlag;
  UINT32  AllLanesPresetCoeff;
  UINT8   PerLanePresetCoeffIndex[MAX_LANES_PER_LINK_SPR];
} PER_LANE_PRESET_COEFF_INFO;

typedef struct {
  UINT8   SocketID;
  UINT32  Link;
  BOOLEAN RedriverStatus;
  PER_LANE_PRESET_COEFF_INFO LinkPerLanePresetCoeff[KTI_SPEEDS_SUPPORTED];
} PER_LANE_PRESET_COEFF_LINK_INFO;

typedef struct {
  UINT32 CountOfAllLanesEparamLink;
  UINT32 CountOfPerLaneEparamLink;
  ALL_LANES_PRESET_COEFF_LINK_INFO AllLanesPresetCoeffLinkInfo[MAX_SOCKET*MAX_FW_KTI_PORTS];
  PER_LANE_PRESET_COEFF_LINK_INFO PerLanePresetCoeffLinkInfo[MAX_SOCKET*MAX_FW_KTI_PORTS];
} KTI_PRESET_COEFF_INFO;

typedef struct {
  //
  //Credits to be programmed in KTIA2RCRCTRL
  //
  UINT8 egrndr_credits;
  UINT8 egrhom_credits;
  UINT8 egrsnp_credits;

  //
  // Credits to be programmed in M3KRTE0CR/M3KRTE1CR for KTI -> KTI communication.
  // Except AD/BL VNA credits, other credits are per VN (i.e VN0 & VN1). But both VNs
  // have same settings.
  //
  UINT8 advnakti;
  UINT8 snpkti;
  UINT8 homkti;
  UINT8 ndrkti;
  UINT8 blvnakti;
  UINT8 drskti;
  UINT8 ncskti;
  UINT8 ncbkti;

  //
  //Credits to be programmed in M3KPCIR0CR/M3KPCIR1CR for M3KTI -> M2PCIe communication
  //
  UINT8 pcicredit;
} M3KTI_RING_CREDIT_PARAM;


//
// Stack resource alloation info
//
typedef struct {
  UINT8         Personality;  // Ubox, ubox+iio, etc of this stack
  UINT8         BusBase;      // Bus base of each stack
  UINT8         BusLimit;     // Bus limit of each stack
  UINT16        IoBase;       // IO base of each stack
  UINT16        IoLimit;      // IO limit for each stack
  UINT32        IoApicBase;   // IoApic base of each stack
  UINT32        IoApicLimit;  // IoApic limit for each stack
  UINT32        MmiolBase;    // Mmiol base of each stack
  UINT32        MmiolLimit;   // Mmiol limit of each stack
  UINT64_STRUCT MmiohBase;    // Mmioh base of each stack
  UINT64_STRUCT MmiohLimit;   // Mmioh limit of each stack
  UINT32        RcrbBase;     // Rcrb(Root Complex Register Block)base address of each stack
} KTI_STACK_RESOURCE;

//
// Definitions for the CXL topology types
//
typedef enum {
  CXL_TOPOLOGY_UNDETERMINED = 0,
  ONE_CXL_LINK_ONE_SOCKET,      // Accelerator has only one CXL link, attached to one socket
  TWO_CXL_LINK_ONE_SOCKET,      // Accelerator has two CXL links, which are attached to one socket
  TWO_CXL_LINK_TWO_SOCKET,      // Accelerator has two CXL links, which are attached to two sockets
  CHAINED_CXL_TOPOLOGY,         // Two accelerators are chained and attached to a CXL port of a socket
  MAX_CXL_TOPOLOGY_TYPES
} CXL_ACCELERATOR_TOPOLOGY_TYPE;

//
// CXL Information structure
//
typedef struct {
  UINT32           UncorrectableErrorStatus;  // It is used to record the CXL DP's Uncorrectable Error Status
  UINT32           CorrectableErrorStatus;    // It is used to record the CXL DP's Correctable Error Status

  UINT64           SerialNum;                 // It is used to record the CXL device's Serial Number
  UINT16           VendorId;                  // It is used to record the CXL device's VID

  UINT16           CxlStatus : 2;             // It is used to indicate if the Stack is traind as CXL port, please see enum IIO_LINK_STATUS
  UINT16           CxlTopologyType : 3;       // Please refer to the enum definitions in CXL_ACCELERATOR_TOPOLOGY_TYPE
  UINT16           Rsvd1 : 3;
  UINT16           SecondPortSocket : 3;      // The socket ID of the 2nd CXL port to which the accelerator is attached
                                              // Note: This field is valid only if the accelerator has two CXL links
  UINT16           SecondPortStack : 3;       // The stack ID of the 2nd CXL port to which the accelerator is attached
                                              // Note: This field is valid only if the accelerator has two CXL links
  UINT16           Rsvd2 : 2;

  UINT8            Cache_Capable : 1;         // It is used to indicate if it supports the CXL.Cache
  UINT8            Mem_Capable : 1;           // It is used to indicate if it supports the CXL.Mem
  UINT8            Mem_HwInit_Mode : 1;       // It is used to indicate if it is MemHwInitMode
  UINT8            Hdm_Count : 2;             // It is used to indicate the number of HDM ranges
  UINT8            Viral_Capable : 1;         // It is used to indicate if it supports Viral handling
  UINT8            Cache_Enabled : 1;         // It is used to indicate if it CXL.Cache is enabled
  UINT8            Rsvd3 : 1;
} CXL_INFO;

typedef enum {
  TYPE_UBOX = 0,
  TYPE_UBOX_IIO,
  TYPE_MCP,
  TYPE_FPGA,
  TYPE_HFI,
  TYPE_NAC,
  TYPE_GRAPHICS,
  TYPE_DINO,
  TYPE_RESERVED,
  TYPE_DISABLED,              // This item must be prior to stack specific disable types
  TYPE_UBOX_IIO_DIS,
  TYPE_MCP_DIS,
  TYPE_FPGA_DIS,
  TYPE_HFI_DIS,
  TYPE_NAC_DIS,
  TYPE_GRAPHICS_DIS,
  TYPE_DINO_DIS,
  TYPE_RESERVED_DIS,
  TYPE_NONE
} STACK_TYPE;

#define MAX_CLUMPS_NUMBER_IN_8S_TOPOLOGY    2
#define SOCKETS_NUMBER_IN_EACH_CLUMP        4

typedef enum {
  Topology8S2ClumpsUndetermined = 0,
  Topology8S2ClumpsUnsupported,
  Topology8S2ClumpsSingleLinked,
  Topology8S2ClumpsDoubleLinked
} TOPO_8S2CLUMPS_LINK_STS;

typedef struct {
  UINT8                     Clumps[MAX_CLUMPS_NUMBER_IN_8S_TOPOLOGY][SOCKETS_NUMBER_IN_EACH_CLUMP]; // Sockets in each of the two clumps.(If Clump does not exist, each element of the clump is set to 0xFF.)
  UINT8                     ClumpsPeerList[SOCKETS_NUMBER_IN_EACH_CLUMP]; // A list of Socket IDs(0xFF if no peer socket) in Clump[1] that links to each socket in Clump[0]
  UINT8                     ClumpWithLegacySocket; // The index(0 or 1) of the clump that includes the legacy socket. (0xFF indicates legacy socket is not in any clump.)
  TOPO_8S2CLUMPS_LINK_STS   LinkStatus;            // TRUE: 2 Clumps are linked by DoubleLink; FALSE: 2 Clumps are linked by SingleLink
} TOPO_8S2CLUMPS_INFO;

//
// Socket resource allocation info
//
typedef struct {
  UINT8               PciSegment;
  UINT8               TotEnabledStacks;
  UINT8               BusBase;
  UINT8               BusLimit;
  UINT16              IoBase;
  UINT16              IoLimit;
  UINT32              IoApicBase;
  UINT32              IoApicLimit;
  UINT32              MmiolBase;
  UINT32              MmiolLimit;
  UINT32              UboxMmioBase;                 //For 10nm server, ubox mmio base
  UINT32              UboxMmioLimit;                //For 10nm server, ubox mmio limit
  UINT64_STRUCT       MmiohBase;
  UINT64_STRUCT       MmiohLimit;
  UINT64_STRUCT       SegMmcfgBase;
  UINT16              stackPresentBitmap;           // bitmap of present stacks per socket
  KTI_STACK_RESOURCE  StackRes[MAX_LOGIC_IIO_STACK];
} KTI_CPU_RESOURCE;

//M3KTI
typedef struct {
  UINT32           M3Egrerrlog0;
  UINT32           M3Egrerrlog1;
  UINT32           M3Ingerrlog0;
  UINT32           M3Ingerrlog1;
} KTI_CPU_M3KTI_ERRLOG;

//M2PCIe
typedef struct {
  UINT32           R2Egrerrlog;
  UINT32           R2Ingerrlog0;
  UINT32           R2Egrerrlog2;
} KTI_CPU_M2PCIE_ERRLOG;


typedef struct {
  KTI_CPU_M3KTI_ERRLOG    M3Kti;
  KTI_CPU_M2PCIE_ERRLOG   M2Pcie;
} KTI_CPU_ERRLOG;

// CPU socket KTI info
//
typedef struct {
  UINT32              Valid : 1;
  UINT32              SocId : 3;
  UINT32              SocType : 2;                    // Socket Type
  UINT32              Rsvd1 : 26;
  KTI_LINK_DATA       LepInfo[MAX_FW_KTI_PORTS];      // KTI LEP info
  UINT8               TotM2Pcie;                      // Tot M2Pcie Enabled
  UINT8               TotM3Kti;
  UINT8               TotCha;
  UINT32              ChaList[MAX_CHA_MAP];
  UINT8               TotM2m;
  UINT32              M2mBitmap;
  UINT8               CpuType;
  TOPOLOGY_TREE_NODE  TopologyInfo[MAX_SOCKET];       // Topology info describing how this soecket is connected to all other sockets
  KTI_CPU_RESOURCE    CpuRes;                         // System resources assigned per CPU
  KTI_CPU_ERRLOG      CpuErrLog;                      // Log of errors that occured during previous boot
  BOOLEAN             ChopTypeValid;                  // Signals that chop type on SBSP is saved
  UINT8               ChopType;                       // ChopType, 11:XCC; 10:HCC; 01:MCC; 00:LCC
  UINT8               DieCount;                       // Die count in the socket
} KTI_CPU_INFO;


//
// Link layer settings, per link
//
typedef struct {
  UINT8 KtiPortDisable:1;     // TRUE - Port disabled;    FALSE- Port enabled (default)
  UINT8 KtiLinkVnaOverride:7; // Numeric value 0x00-0x7f
  UINT8 Rsvd:8;
} KTI_CPU_LINK_SETTING;


//
// Phy general setting, per link
//
typedef struct {
  UINT32  KtiLinkSpeed:3;
  UINT32  Rsvd:29;
} KTI_CPU_PHY_SETTING;

//
// Per CPU setting
//
typedef struct {
  KTI_CPU_LINK_SETTING  Link[MAX_FW_KTI_PORTS];
  KTI_CPU_PHY_SETTING   Phy[MAX_FW_KTI_PORTS];
} KTI_CPU_SETTING;

//
// structure w/ system parameters needed for OEM_GET_RESOURCE_MAP_UPDATE
//
typedef struct{
  UINT32            SizeOfCpuRes;
  UINT32            CpuList;
  UINT8             Sbsp;
  UINT32            mmcfgSize;
  UINT64_STRUCT     mmiohGranularity;
} KTI_OEMCALLOUT;

//
// Structure of TXEQ Adaptation Table
// Only valid for SPR
//
typedef struct {
  UINT8                            TcaCoeffIndex[MAX_LANES_PER_LINK_SPR];
} ADAPTED_EPARAM_LINK_INFO;

typedef struct {
  BOOLEAN                      TableValid;           // TRUE - Valid; otherwise invalid.
  UINT8                        Speed;                // Use KTI_LINK_SPEED definition excluding FREQ_PER_LINK, MAX_KTI_LINK_SPEED
  ADAPTED_EPARAM_LINK_INFO     AdaptedEQs[MAX_SOCKET][SI_MAX_KTI_PORTS_SPR];
} KTI_ADAPTATION_TABLE;

#define KTI_ADAPTATION_TABLE_DATA_NAME L"KtiAdaptationTable"

typedef enum {
  UPI_TXEQ_ADAPTATION_LOOPBACK_PHASE = 0,
  UPI_TXEQ_ADAPTATION_SLOWMODE_PHASE,
  UPI_TXEQ_ADAPTATION_UNSUPPORTED_PHASE = 0xFF,
} UPI_TXEQ_ADAPTATION_PHASE;

/**
  OEM hook at the end of topology discovery in KTIRC to be used for error reporting.

  @param None

  @retval     - N/A
**/
typedef
VOID
(EFIAPI *OEM_HOOK_POST_TOPOLOGY_DISCOVERY) (
  VOID
  );

/**

   Callout to allow OEM to adjust the resource map.
   Used to allow adjustment of IO ranges, bus numbers, mmioh and mmiol resource assignments
   Calling code assumes the data structure is returned in a good format.
   SBSP resources must be assigned first, with rest of sockets assigned in ascending order of NID
   Resources must be assigned in ascending consecutive order with no gaps.
   Notes: This does not allow for changing the mmcfg size of mmioh granularity
          However you can allocate multiple consecutive blocks of mmioh to increase size in a particular stack
          and adjust the mmiohbase


   @param CpuResources - Ptr to array of Cpu Resources
   @param SystemParams - structure w/ system parameters needed

**/
typedef
VOID
(EFIAPI *OEM_GET_RESOURCE_MAP_UPDATE) (
  KTI_CPU_RESOURCE  CpuResources[MAX_SOCKET],
  KTI_OEMCALLOUT    SystemParams
  );

/**
  Platform hook to get TXEQ adaptation parameters

  @param[out] AdaptTbl - The adapation table of the speed

  @retval TRUE / FALSE

**/
typedef
BOOLEAN
(EFIAPI *OEM_GET_ADAPTED_EQ_SETTINGS) (
  KTI_ADAPTATION_TABLE     *AdaptTbl
  );

/**

  Platform hook to check if the CPU parts are changed or swapped

  @retval TRUE    - CPU parts are changed or swapped
          FALSE   - No change on CPU parts

**/
typedef
BOOLEAN
(EFIAPI *OEM_CHECK_CPU_PARTS_CHANGE_SWAP) (
  VOID
  );

//
// KTIRC input structure
//
typedef struct {
  //
  // Protocol layer and other general options; note that "Auto" is provided only options whose value will change depending
  // on the topology, not for all options.
  //

  //
  // Indicates the ratio of Bus/MMIOL/IO resource to be allocated for each CPU's IIO.
  // Value 0 indicates, that CPU is not relevant for the system. If resource is
  // requested for an CPU that is not currently populated, KTIRC will assume
  // that the ratio is 0 for that CPU and won't allocate any resources for it.
  // If resource is not requested for an CPU that is populated, KTIRC will force
  // the ratio for that CPU to 1.
  //


  UINT8               BusRatio[MAX_SOCKET];

  UINT8               D2KCreditConfig;             // 1 - Min, 2 - Med (Default), 3- Max
  UINT8               SnoopThrottleConfig;  // 0 - Disabled (Default), 1 - Min, 2 - Med, 3- Max
  UINT8               LegacyVgaSoc;       // Socket that claims the legacy VGA range; valid values are 0-7; 0 is default.
  UINT8               LegacyVgaStack;     // Stack that claims the legacy VGA range; valid values are 0-3; 0 is default.
  UINT8               ColdResetRequestStart;
  UINT8               P2pRelaxedOrdering; // 0 - Disable(default) 1 - Enable
  UINT8               DebugPrintLevel;    // Bit 0 - Fatal, Bit1 - Warning, Bit2 - Info Summary; Bit 3 - Info detailed. 1 - Enable; 0 - Disable
  UINT8               SncEn;              // 0 - Disable, (default) 1 - Enable
  UINT8               UmaClustering;      // 0 - Disable, 2 - 2Clusters UMA, 4 - 4Clusters UMA
  UINT8               IoDcMode;           // 0 - Disable IODC,  1 - AUTO (default), 2 - IODC_EN_REM_INVITOM_PUSH, 3 - IODC_EN_REM_INVITOM_ALLOCFLOW
                                          // 4 - IODC_EN_REM_INVITOM_ALLOC_NONALLOC, 5 - IODC_EN_REM_INVITOM_AND_WCILF
  UINT8               DegradePrecedence;  // Use DEGRADE_PRECEDENCE definition; TOPOLOGY_PRECEDENCE is default
  UINT8               Degrade4SPreference;// 4S1LFullConnect topology is default; another option is 4S2LRing topology.
  UINT8               DirectoryModeEn;    // 0 - Disable; 1 - Enable (default)
  UINT8               XptPrefetchEn;      // Xpt Prefetch :  1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8               KtiPrefetchEn;      // Kti Prefetch :  1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8               XptRemotePrefetchEn;     // Xpt Remote Prefetch :  1 - Enable; 0 - Disable; 2 - Auto (default)   (ICX only)
  UINT8               RdCurForXptPrefetchEn;   // RdCur for XPT Prefetch :  0 - Disable, 1 - Enable, 2- Auto (default)
  UINT8               KtiFpgaEnable[MAX_SOCKET];  // Indicate if should enable Fpga device found in this socket :  0 - Disable, 1 - Enable, 2- Auto
  UINT8               DdrtQosMode;          // DDRT QoS Feature:  0 - Disable (default), 1 - M2M QoS Enable, Cha QoS Disable
                                            // 2 - M2M QoS Enable, Cha QoS Enable

  //
  // Phy/Link Layer Options (System-wide and per socket)
  //
  UINT8               KtiLinkSpeedMode;   // Link speed mode selection; 0 - Slow Speed; 1- Full Speed (default)
  UINT8               KtiLinkSpeed;       // Use KTI_LINKSPEED definition
  UINT8               KtiAdaptationEn;    // 0 - Disable, 1 - Enable
  UINT8               KtiAdaptationSpeed; // Use KTI_LINK_SPEED definition; MAX_KTI_LINK_SPEED - Auto (i.e BIOS choosen speed)
  UINT8               KtiLinkL0pEn;       // 0 - Disable, 1 - Enable, 2- Auto  (default)
  UINT8               KtiLinkL1En;        // 0 - Disable, 1 - Enable, 2- Auto  (default)
  UINT8               KtiFailoverEn;      // 0 - Disable, 1 - Enable, 2- Auto (default)
  UINT8               KtiLbEn;            // 0 - Disable(default), 1 - Enable
  UINT8               KtiCrcMode;         // CRC_MODE_16BIT, CRC_MODE_ROLLING_32BIT, CRC_MODE_AUTO or CRC_MODE_PER_LINK

  UINT8               KtiCpuSktHotPlugEn;       // 0 - Disable (default), 1 - Enable
  UINT8               KtiCpuSktHotPlugTopology; // 0 - 4S Topology (default), 1 - 8S Topology
  UINT8               KtiSkuMismatchCheck;      // 0 - No, 1 - Yes (default)
  UINT8               IrqThreshold;             // IRQ Threshold setting
  UINT8               TorThresLoctoremNorm;     // TOR threshold - Loctorem threshold normal
  UINT8               TorThresLoctoremEmpty;    // TOR threshold - Loctorem threshold empty
  UINT8               TscSyncEn;                // TSC sync in sockets: 0 - Disable, 1 - Enable, 2 - AUTO (Default)
  UINT8               StaleAtoSOptEn;           // HA A to S directory optimization: 1 - Enable; 0 - Disable; 2 - Auto (Default)
  UINT8               LLCDeadLineAlloc;         // LLC dead line alloc: 1 - Enable(Default); 0 - Disable
  UINT8               SplitLock;
  UINT8               ColdResetRequestEnd;

  //
  // Phy/Link Layer Options (per Port)
  //
  KTI_CPU_SETTING     PhyLinkPerPortSetting[MAX_SOCKET];


  UINT8               mmCfgBase; ///< MMCFG Base address, must be 64MB (SKX, HSX, BDX) / 256MB (GROVEPORT) aligned. Options: {0:1G, 1:1.5G, 2:1.75G, 3:2G, 4:2.25G, 5:3G, 6: Auto}
  UINT8               mmCfgSize; ///< MMCFG Size address, must be 64M, 128M or 256M. Options: {0:64M, 1:128M, 2:256M, 3:512M, 4:1G, 5:2G, 6: Auto}
  UINT32              mmiolBase; ///< MMIOL Base address, must be 64MB aligned
  UINT32              mmiolSize; ///< MMIOL Size address
  UINT32              mmiohBase; ///< Address bits above 4GB, i,e, the hex value here is address Bit[45:32] for SKX/KNH, Bit[51:32] for ICX
  UINT8               lowGap;
  UINT8               highGap;
  UINT16              mmiohSize; ////<< Number of 1GB contiguous regions to be assigned for MMIOH space per CPU.  Range 1-1024
  UINT8               isocEn;    ///< 1 - Enable; 0 - Disable (BIOS will force this for 4S)
  UINT8               dcaEn;     ///< 1 - Enable; 0 - Disable

  /*
  BoardTypeBitmask:
    Bits[3:0]   - Socket0
    Bits[7:4]   - Socket1
    Bits[11:8]  - Socket2
    Bits[15:12] - Socket3
    Bits[19:16] - Socket4
    Bits[23:20] - Socket5
    Bits[27:24] - Socket6
    Bits[31:28] - Socket7

  Within each Socket-specific field, bits mean:
    Bit0 = CPU_TYPE_STD support; always 1 on Socket0
    Bit1 = CPU_TYPE_F support
    Bit2 = CPU_TYPE_P support
    Bit3 = reserved
  */
  UINT32              BoardTypeBitmask;
  UINT32              AllLanesPtr;
  UINT32              PerLanePtr;
  UINT32              AllLanesSizeOfTable;
  UINT32              PerLaneSizeOfTable;
  UINT32              WaitTimeForPSBP; // the wait time in units of 1000us for PBSP to check in.
  BOOLEAN             IsKtiNvramDataReady;
  UINT32              OemHookPostTopologyDiscovery;
  UINT32              OemGetResourceMapUpdate;
  UINT32              OemGetAdaptedEqSettings;
  UINT32              OemCheckCpuPartsChangeSwap;

  BOOLEAN             WaSerializationEn;      // Enable BIOS serialization WA by PcdWaSerializationEn
  //
  // DFX options should be placed at the end of structure.
  //
  KTI_DFX_PARM        DfxParm;
  KTI_DFX_CPU_SETTING DfxCpuCfg[MAX_SOCKET];  // Per CPU socket specific config info
  UINT8               KtiInEnableMktme;       // 0 - Disabled; 1 - Enabled; MkTme status decides D2Kti feature state
  UINT32              CFRImagePtr;
  UINT8               S3mCFRCommit;           // 0 - Disable S3m CFR flow.    1 - Provision S3m CFR but not Commit.    2 - Provsion and Commit S3M CFR.
  UINT8               PucodeCFRCommit;        // 0 - Disable Pucode CFR flow. 1 - Provision Pucode CFR but not Commit. 2 - Provsion and Commit Pucode CFR.
} KTI_HOST_IN;

typedef struct{
  //
  // These variables are initialized and used like global variable by KTIRC. Not meant for outside KTIRC usage.
  //
  UINT8         CurrentReset;                  // Reset type witnessed by the system (viz PWRGD, WARM)
  UINT8         SbspSoc;                       // System BSP socket ID
  UINT8         SocketWays[MAX_SOCKET];        // Indicates NIDs that are allowed
  UINT8         RasType[MAX_SOCKET];           // Ras type supported by the socket
  UINT8         TotCha[MAX_SOCKET];            // Total Chas/LLCs; All CPUs must have same number of Cbo/LLCs
  UINT32        ChaList[MAX_SOCKET][MAX_CHA_MAP]; // List of Chas/LLCs
  UINT8         TotIio[MAX_SOCKET];            // Total Iio; All CPUs must have same number of IioLink
  UINT8         TotKtiAgent[MAX_SOCKET];       // Total KTIAgents; All CPUs must have same number of KtiAgents
  UINT16        M2PcieActiveBitmap[MAX_SOCKET];// bitmap of activet m2pcie stop with IIO stack per socket
  UINT8         TotActiveM2Pcie[MAX_SOCKET];   // Total active M2PCIE with IIO stack enabled
  UINT8         TotSadTargets;                 // Count of all SAD targets in the system
  UINT32        MmcfgLocalRules[MAX_SOCKET][MMCFG_STORE_SIZE];// Var to store programming to reuse in KTI/IIO BusId programming
                                                              // Number of MMCFG_LOCAL_RULE_ADDRESS (see MAX_MMCFG_LOCAL_RULES_ADDR_CHA) +
                                                              // Number of MMCFG_LOCAL_RULE_TGT_IIO (see MAX_MMCFG_LOCAL_RULES_TARGET_IIO)
  UINT8         TotCpu;                        // Total CPUs populated in the system
  UINT32        CpuList;                       // List of CPUs populated in the system
  UINT32        FpgaList;                      // List of Fpga populated in the system
  UINT8         ProgramCreditFpga;
  UINT8         OrigTotCpu;                    // Total CPUs populated in the system before topology reduction
  UINT32        OrigCpuList;                   // List of CPUs populated in the system before topology reduction
  UINT8         SysOsbEn;                      // OSB Enable: 1 - Enable; 0 - Disable
  UINT8         SnoopFanoutEn;                 // Enable snoop fanout
  UINT8         D2cEn;                         // Direct To Core Enable: 1 - Enable; 0 - Disable
  UINT8         D2kEn;                         // Direct To KTI Enable:  1 - Enable; 0 - Disable
  BOOLEAN       ForceColdResetFlow;            // Force cold reset flow
  BOOLEAN       Is4SRing;                      // Indicates if 4S ring
  BOOLEAN       IsDoubleLinks;                 // Indicate if double links 3S-Ring/4S-Ring/4S-FullyConnect
  BOOLEAN       IsRouteThrough;                // Indicates if 3S chain, 4s-chain, 4S-ring, 8S topology
  UINT8         MmiolGranularity;              // min mmiol granualarity to each stack
  UINT8         MmiohGranularity;              // min mmioh granualarity to each stack
  UINT16        MmiohTargetSize;               // mmioh target granularity in use  0-5

  UINT8         KtiCpuSktHotPlugEn;            // hot plug enabled or not.  Only valid for hotplug capable parts

  UINT32        MmcfgBase;                     // Local storage of mmcfgbase
  BOOLEAN       IsMmcfgAboveMmiol;             // Track relative location of mmcfgbase
  BOOLEAN       UseNonInterleavedMmioh;        // Indicate if Mmioh allocation requires use of both decoders
  UINT8         DualLinksInterleavingMode;     // In 2S 2KTI can: 2 - Auto - do nothing (default); 1 - use legacy CHA interleaving (disable SNC, turn off XOR interleave); 0 - disable D2C
  UINT8         FpgaEnable[MAX_SOCKET];        // Indicate if should enable Fpga device found in this socket :  0 - Disable, 1 - Enable
  BOOLEAN       ProgramPhyLink;                // TRUE to indicate to Phy/Link code to program Phy/Link
  UINT8         SnoopFilter;                   // Enable:Disable snoopfilter
  KTI_CPU_SETTING PhyLinkPerPortSetting[MAX_SOCKET];  //Local LinkSpeed
  UINT8         TSCSyncEn;                     // TSC sync in sockets: 0 - Disable, 1 - Enable, 2 - AUTO (Default, enable by LR board)
  UINT8         KtiPortCnt;                    // num KTI ports residing on each Socket
  UINT8         PackageBspStepping[MAX_SOCKET];// Package BSP Stepping (per socket).
  UINT8         CpuType;                       // Cpu Type
  UINT8         ChopType[MAX_SOCKET];          // Cpu ChopType
  UINT32        KtiSetup;                      // Pointers to KTI_HOST_IN structure
  UINT32        KtiVar;                        // Pointers to KTI_HOST_OUT structure
  UINT32        KtiNvram;                      // Pointers to KTI_HOST_NVRAM structure
  UINT8         EvMode;
  BOOLEAN       SkipWarmResetRequest;          // TRUE to skip warm-reset request on the cold-reset path. Default is FALSE.
  BOOLEAN       WaSlowModeWaResetRequired;     // Indicate the wa needs a warm reset
  UINT32        WaSlowModeWaCpl;               // bitmask of sockets*links to track where phy wa has been applied
  BOOLEAN       DualLinksEn;                   // When set to FALSE, the system will treat duallinks as a single link. Default is TRUE.
                                               // This flag has no effect if no dual links detected on the system.
  UINT8         UpiRoutingMode;                // UPI Routing Mode.  See UPI_ROUTING_MODE enum.
  UINT8         EightSocketTopology;           // 8-socket topology. See TOPOLOGY_8S enum.
  UINT8         DieCount[MAX_SOCKET];          // Die count in the socket
  UINT8         KtiPortPresentBitmap[MAX_SOCKET];   // Bitmap of present Kti port
  UINT8         SnoopFanoutChaInterleaveEn;    // Balancing snoop fanout via CHA Interleave. 0 - Disable, 1 - Enable, 2 - AUTO.
  TOPO_8S2CLUMPS_INFO Topo8S2ClumpsInfo;       // Information about the 8S 2Clumps topology.
  BOOLEAN       UpiMinisadCfgDone[MAX_SOCKET]; // TURE: has programmed UpiMiniSad for the socket
  BOOLEAN       KtiRunAdaptation;              // Whether to run the adaptation or not
  UINT8         CurrentSpeedAdapted;           // Speed that is being adapted; Bit0 - 12.8, Bit1 - 14.4, etc
  UINT8         AdaptSpeedsCompleted;          // Bit map of Speeds for which adaptation is completed; Bit0 - 12.8, Bit1 - 14.4, etc
  UINT8         KtiAdaptTcaIndex;              // This is used to indicate TCA coeff index it is adapting
  UINT8         KtiAdaptFomArray[MAX_NUMBER_OF_FOM];  // used to store the max FOM of each lane in the txeq adapatation process
  BOOLEAN       Reconfig2LMAddrMaskTo4G;       // TRUE indicates UPI RC reprogrammed 2LM Hash Address Mask to 4GB in post MRC warm-reset flow,
                                               // and requested a warm-reset. Default is FALSE.
  UINT8         UpiRedriverStatus[MAX_SOCKET]; // UPI redriver Status, the bitmap of each socket to indicate if the link has redriver. 1: redriver present; 0: no redriver or not care
  UINT8         DfxMdfisAdvancedTrainingEn;
} KTI_HOST_INTERNAL_GLOBAL;

#define KTISETUP ((KTI_HOST_IN *) (UINTN) KtiInternalGlobal->KtiSetup)
#define KTIVAR ((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal->KtiVar)
#define KTINVRAM ((KTI_HOST_NVRAM *) (UINTN) KtiInternalGlobal->KtiNvram)

//
// KTIRC output structure
//
typedef struct {

  //
  // All CPUs are assigned pre-determined array element, viz, element 0 is for CPU0,
  // element 1 is for CPU1 etc. There is valid bit to track if the CPU is populated or not.
  //
  KTI_CPU_INFO  CpuInfo[MAX_SOCKET];           // KTI related info per CPU
  UINT8         SysConfig;                     // System Topology

  //
  // There is a corresponding input option for these options. If the input option can not be met,
  // due to the current system topology not allowing it, the options will be forced to supported
  // value and platform BIOS can use these variables to know if any such overriding has happened.
  //
  UINT8         OutM2IosfPmonAccessControl;     // 0 - M2IOSF PMON cannot be accessed, 1 - M2IOSF PMON can be accessed,  2 - Auto (default)
  UINT8         OutPmonConfig;                  // 1 - Uncore,MC,PCIe only, 2 - Uncore,MC,PCIe & M2IOSF,  3 - Auto (default), 0 - Disable
  UINT8         OutD2KCreditConfig;             // 1 - Min, 2 - Med (Default), 3- Max
  UINT8         OutSnoopThrottleConfig;         // 0 - Disabled (Default), 1 - Min, 2 - Med, 3- Max
  UINT8         OutLegacyVgaSoc;                // Socket that claims the legacy VGA range
  UINT8         OutLegacyVgaStack;              // Stack that claims the legacy VGA range
  UINT8         OutIsocEn;                      // 1 - Enable; 0 - Disable (BIOS will force this for 4S)
  UINT8         KtiCurrentLinkSpeedMode;        // Current link speed mode; 0 - Slow Speed; 1- Full Speed.
  UINT8         OutKtiLinkSpeed;                // Link speed/freq
  UINT8         OutPerLinkSpeed[MAX_SOCKET][MAX_FW_KTI_PORTS]; // Per link speed
  UINT8         OutKtiLinkL0pEn;                // 0 - Disabled, 1 - Enabled
  UINT8         OutKtiLinkL1En;                 // 0 - Disabled, 1 - Enabled
  UINT8         OutKtiFailoverEn;               // 0 - Disabled, 1 - Enabled
  UINT8         OutKtiCpuSktHotPlugEn;          // 0 - Disabled, 1 - Enabled
  UINT8         OutSystemRasType;               // See RasTypes.h for encodings

  UINT8         OutKtiCrcMode;                  // CRC_MODE_16BIT, CRC_MODE_ROLLING_32BIT
  UINT8         OutVn1En;                       // 0 - Disabled, 1 - Enabled
  UINT8         OutIsRouteThrough;              // 0 - Disabled, 1 - Enabled
  UINT8         OutHitMeEn;                     // 0 - Disabled, 1 - Enabled
  UINT8         OutIoDcMode;                    // 0 - Disable IODC,  1 - AUTO (default), 2 - IODC_EN_REM_INVITOM_PUSH, 3 - IODC_EN_REM_INVITOM_ALLOCFLOW
                                                // 4 - IODC_EN_REM_INVITOM_ALLOC_NONALLOC, 5 - IODC_EN_REM_INVITOM_AND_WCILF
  UINT8         OutSncEn;                       // 0 - Disabled, 1 - Enabled(Not support in 4MC SKU), 2 - SNC2, 4 - SNC4
  UINT8         OutUmaClustering;               // 0 - Disabled, 2 - 2 cluster UMA, 4 - 4 clusters UMA
  UINT8         OutNumOfClusterPerSystem;       // 0 - N/A, 1: 1 cluster, 2: 2 clusters ( per system) Number of Cluster per socket for SNC enable
  UINT8         OutNumOfCluster[MAX_SOCKET];    // Number of Cluster per socket
  UINT8         OutBoardVsCpuConflict;          // 8-bit bitmask of CPUs that have a conflict detected with a board
  UINT16        OutIoGranularity;               // Io Granularity
  UINT32        OutMmiolGranularity;            // Mmiol Granularity
  UINT64_STRUCT OutMmiohGranularity;            // Mmioh Granularity
  BOOLEAN       OutKtiPerLinkL1En[SI_MAX_CPU_SOCKETS][MAX_FW_KTI_PORTS]; // output kti link enabled status for PM
  UINT8         OutKtiPrefetch;                 // 0 - Disabled; 1 - Enabled
  UINT8         OutKtiFpgaEnable[MAX_SOCKET];   // Indicate fpga enabled in this socket - 0 - Disable, 1 - Enable
  UINT8         OutKtiFpgaPresent[MAX_SOCKET];  // Indicate fpga present in this socket - 0 - Disable, 1 - Enable
  UINT8         OutFpgaHomeAgent[MAX_SOCKET];   // Indicate fpga home agent present in this socket - 0 - Disable, 1 - Enable
  UINT8         OutFpgaCacheAgent[MAX_SOCKET];  // Indicate fpga cache agent present in this socket - 0 - Disable, 1 - Enable
  UINT8         OutRrqThreshold;                // RRQ threshold
  UINT8         OutIRQThreshold;                // IRQ RRQ threshold
  UINT8         OutTorThresLoctoremNorm;        // TOR threshold - Loctorem threshold normal
  UINT8         OutTorThresLoctoremEmpty;       // TOR threshold - Loctorem threshold empty
  UINT8         OutSncGbAlignRequired;          // 0 - no GB Alignment required, 1 - GB Alignment required
  UINT8         OutSncPrefetchEn;               // OR the SNC and XPT KTI prefetch
  UINT8         OutSysDirectoryModeEn;          // Directory Mode Enable: 1 - Enable; 0 - Disable
  UINT8         OutXptPrefetch;                 // 0 - Disabled; 1 - Enabled
  UINT8         OutXptRemotePrefetch;           // 0 - Disabled; 1 - Enabled
  UINT32        OutDfxIioStackEn;              // Disable MS2IOSF strap override: Auto(default),
                                                // Bitmap - order must match CAPID. 1 - behaves as defined by internal fuse 0 - given ms2iosf is disabled.
  UINT8         OutDfxHqmEn;                   // HQM strap override: 1 - Enable HQM; 0 - Disable HQM; 2 - Auto (default) do not modify strap
  UINT32        OutUboxMmioSize;                // Ubox Mmio BAR Size
  UINT8         OutDfxCxlVid;                   // CXL Set Vid to 0x198: 0 - Disable, 1 - Enable, 2 - AUTO.
  UINT8         OutStaleAtoSOptEn;              // HA A to S directory optimization: 1 - Enable; 0 - Disable; 2 - Auto (Default)

  //
  // These variables are initialized and used like global variable by KTIRC.
  // Not meant for outside KTIRC consumption.
  //
  UINT8         RasInProgress;                   // Set to TRUE when the KTIRC is called to handle a CPU/IOH O*L event
  UINT8         RasEvtType;                      // 0 - Online; 1 - Offline
  UINT8         RasSocId;                        // RAS event socket ID
  BOOLEAN       ProgramNonBC;                    // Flag to indicate if the Non BC RTA entries to be programmed
  BOOLEAN       ProgramBC;                       // Flag to indicate if the BC RTA entries to be programmed
  BOOLEAN       Is4SRing;                        // Indicates if 4S ring

  //
  // The following items are moved from SysHost.var.common
  //
  UINT8         CurrentReset;                    // Reset type witnessed by the system (viz PWRGD, WARM)
  UINT8         numCpus;                         // < 1,..4. Total number of CPU packages installed and detected (1..4)by KTI RC
  UINT32        FpgaPresentBitMap;               // bitmap of fpga NID detected by KTI RC
  UINT8         StackBus[MAX_SOCKET][MAX_LOGIC_IIO_STACK];  /// Bus of each stack
  UINT8         SocketLastBus[MAX_SOCKET];            /// Last bus of socket
  UINT8         SocketFirstBus[MAX_SOCKET];
  UINT8         segmentSocket[MAX_SOCKET];
  UINT8         KtiPortCnt;                           /// num KTI ports residing on each Socket
  BOOLEAN       SharedKtiPort0And1;                   /// Indicates whether KTI Port 0 and 1 share a single mesh stop
  UINT16        stackPresentBitmap[MAX_SOCKET];       /// bitmap of present stacks per socket
  UINT16        CxlPresentBitmap[MAX_SOCKET];         /// CXL bitmap per socket [Bit0 - Stack 0, Bit1 - stack1, ...]
  UINT16        M2PciePresentBitmap[MAX_SOCKET];      /// bitmap of present m2pcie ring stop per socket
  UINT8         Socket10nmUboxBus0[MAX_SOCKET];       /// 10nm CPU use only
  UINT32        mmCfgBaseH[MAX_SOCKET];
  UINT32        mmCfgBaseL[MAX_SOCKET];
  UINT32        MmioBar[MAX_SOCKET][TYPE_MAX_MMIO_BAR];
  UINT32        HostAddress;
  UINT32        SlavePipeAddress;
  UINT8         MemRasFlag;
  UINT8         inComingSktId;
  UINT8         SystemRasType;
  UINT16        tolmLimit;
  UINT32        mmCfgBase; ///< MMCFG Base address, must be 64MB (SKX, HSX, BDX) / 256MB (GROVEPORT) aligned
  UINT32        mmCfgSize; ///< MMCFG Size address, must be 64M, 128M or 256M
  UINT32        mmiolBase; ///< MMIOL Base address, must be 64MB aligned
  UINT32        mmiolSize; ///< MMIOL Size address
  UINT8         lowGap;
  UINT32        SocketPresentBitMap;                     /// bitmap of socket Id with CPUs present detected by KTI RC
  UINT32        SlaveDieBitMap;                          /// bitmap of Slave Dies present in the system. 1 indicates Slave Die, 0 indicates Master Die.
                                                         /// The bit is only valid if the corresponding bit in SocketPresentBitMap is set to 1.
  UINT8         PchPresentBitMap;                        /// Bitmap of socketId which has legacy PCH or PCH.IO expander
  UINT8         MaxAddress;                              /// Maximum number of address bits (42, 46, 52, etc.)
  UINT8         OutD2kEn;                                /// Whether D2K (Direct-to-KTI) is enabled
  BOOLEAN       WaSlowModeEn;                            /// Enable the BIOS enable the UPI SlowMode WA by PcdWaSlowModeEnable
  UINT8         LatestConsumedPA;                        /// Latest consumed physical address by system
  UINT8         MaxKtiAgent[MAX_SOCKET];                 /// Max Kti Agent
  UINT8         MaxM3Kti[MAX_SOCKET];                    /// Max M3Kti
  UINT8         DdrtQosMode;                             /// DDRT QoS Feature:  0 - Disable (default), 1 - M2M QoS Enable, Cha QoS Disable
                                                         /// 2 - M2M QoS Enable, Cha QoS Disable
  UINT8         KtiPortPresentBitmap[MAX_SOCKET];        /// Bitmap of present Kti Port
  BOOLEAN       UpiAffinityEn;                           /// Indicate whether UPI Affinity feature eanbled or not.
  BOOLEAN       TwistedKtiLinks;                         /// Indicates if the links are twisted in 2S config
  CXL_INFO      CxlInfo[MAX_SOCKET][MAX_CXL_PER_SOCKET]; /// CXL information
  UINT32        DmiMemBarBase[MAX_SOCKET];               /// DMI MemBar Base Address
  UINT32        DmiMemBarSize[MAX_SOCKET];               /// DMI MemBar Size
  BOOLEAN       UmaClusteringDowngrade;                  /// Indicate UMA based clustering downgrade
  BOOLEAN       WarmResetEliminationEn;
  BOOLEAN       HbmSku;                                  /// Indicate whether it's HBM sku
  UINT8         PcieGen4SpeedLimit[MAX_SOCKET];          /// Each bit map to a IIO stack: 1 - Limit the max PCIE speed to only Gen4, 0 - No limit(Default).
  UINT8         OverrideStrapSetCpuMask;                 /// Override S3M strap sets for a given socket instance. Default is 0.
                                                         /// Bit-0 represents Socket 0, ..., Bit-7 represents Socket 7.
                                                         /// If set to 1, BIOS needs to update S3M soft straps for the respective Socket instance.
  UINT32        S3mInfo[MAX_SOCKET][S3M_CPLD_DATA_BLOCK_SIZE]; /// Information read from S3M
  UINT8         Pi5Protocol[MAX_SOCKET][MAX_CXL_PER_SOCKET]; /// PI5 supported protocol
  UINT8         OutKtiAdaptationEnable;                  /// Kti txeq adaptation enable, 0 -- Disable; 1 -- Enable
  KTI_ADAPTATION_TABLE   KtiAdaptationTable[KTI_SPEEDS_SUPPORTED];     /// Kti Adaptation table pointer
  UINT8         MaxSupportedM2m;                                       /// Max supported Mesh2Mem instance
  BOOLEAN       IioEarlyLinkInitDone;                    /// Indicate if the IioEarlyLinkInit () routine already be invoked - TRUE: Invoked, FALSE: Not invoked yet
  UINT32        MicroCodeRevision;                       /// Revision of the MicroCode loaded on Sbsp
  UINT8         UpimdfPortBitmap[MAX_SOCKET];            /// Indicate the bitmap of the upimdf subsystem (1 on bit1 means UPI1 is UPIMDF...)
} KTI_HOST_OUT;

typedef struct {
  UINT32           Cpus;                                 /// List of CPUs populated in the system
  UINT32           Chas[MAX_CHA_MAP];                    /// List of Chas/LLCs in SBSP
  KTI_LINK_DATA    LepInfo[MAX_SOCKET][MAX_FW_KTI_PORTS];/// KTI LEP info
  KTI_HOST_IN      SaveSetupData;                        /// Kti Setup save in variable
} KTI_HOST_NVRAM;


#pragma pack()

#endif // _KTI_HOST_H_
