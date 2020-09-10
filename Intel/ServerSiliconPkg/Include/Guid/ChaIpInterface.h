/** @file
  CHA generation specific silicon access interface

  This library abstracts silicon register differences between different versions
  of CHA IP blocks.
  This library is designed to be directly linked into the Uncore consumer of these
  services
  OR
  To be used to expose a dynamic interface (PPI, DXE Protocol, or SMM Protocol) as
  appropriate for the phase being executed.

  Furthermore, this subsystem is designed to support multiple different versions
  dynamically linked at runtime.

  The expected valid uses for this library are thus:
  (1) One Uncore PEIM supporting a homogenous system and the single instance is statically linked
  (2) One Uncore PEIM supporting a heterogeneous system where a router instance is statically linked
    and multiple PEIM produce silicon specific instances of the library and the router selects the
    correct instance per socket.

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

#ifndef _CHA_IP_INTERFACE_H_
#define _CHA_IP_INTERFACE_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/ChaLib.h>
#include <Library/ChaLibSkx10nm.h>

typedef struct {
  //
  // Function pointers
  //
  CHA_GET_SET_SAI_REGS                                ChaGetSetSaiRegisters;
  CHA_CFG_KTI_TOR_TIMEOUT                             ChaConfigureKtiTorTimeout;
  CHA_FIXUP_PAM_ATTRIBUTES                            ChaFixupPamAttributes;
  PROGRAM_ALL_PAM_ACCESS_TO_DRAM                      ProgramAllPamAccessToDram;
  CHA_SAD_TARGET_EN_MODULO3_BASED_KTI_LINK            ChaSadTargetEnableModulo3BasedKtiLink;
  CHA_SAD_TARGET_CFG                                  ChaSadTargetCfg;
  CHA_CFG_LOCAL_SOCKET_ID                             ChaConfigureLocalSocketId;
  CHA_GET_LT_TARGET_STATUS                            ChaGetLtTargetAndStatus;
  CHA_SET_LT_TARGET_STATUS                            ChaSetLtTargetAndStatus;
  CHA_GET_MMCFG_TARGET_LIST                           ChaGetMmcfgTargetList;
  CHA_SET_MMCFG_TARGET_LIST                           ChaSetMmcfgTargetList;
  CHA_CFG_MMCFG_RULES                                 ChaConfigureMmcfgRules;
  CHA_GET_MMCFG_TARGET_LIST_RULES                     ChaGetMmcfgTargetListAndRules;
  CHA_SET_MMCFG_RULES                                 ChaSetMmcfgRule;
  CHA_SET_MMCFG_TARGET_LIST_RULES                     ChaSetMmcfgTargetListAndRules;
  CHA_SET_TEMP_BUSNUM_MMCFG                           ChaSetTempBusNumAndMmcfg;
  CHA_GET_MMCFG_RULE_VALID_TARGET                     ChaGetMmcfgRuleValidTarget;
  CHA_SET_MMCFG_RULE_VALID_TARGET                     ChaSetMmcfgRuleValidTarget;
  CHA_CFG_IOPORT_TARGET_LIST                          ChaConfigureIoPortTargetList;
  CHA_CFG_IOAPIC_TARGET_LIST                          ChaConfigureIoApicTargetList;
  CHA_GET_MMIOH_TARGET_INTERLEAVE_LIST                ChaGetMmiohTargetInterleaveList;
  CHA_CFG_MMIOH_TARGET_INTERLEAVE_LIST                ChaConfigureMmiohTargetInterleaveList;
  CHA_CFG_NEM_RULE_BASE                               ChaConfigureNemRuleBase;
  CHA_RAS_REPLICATE_ME_SEG                            ChaRasReplicateMeSeg;
  CHA_RAS_REPLICATE_DRAM_SETTING                      ChaRasReplicateDramSetting;
  CHA_GET_REM_STACK_TGT_LIST                          ChaGetRemStackTgtLst;
  CHA_CFG_CPU_MMIOH_ENTRY                             ChaConfigureCpuMmiohEntry;
  CHA_CFG_SNOOP_FILTER_FOR_FPGA                       ChaConfigureSnoopFilterForFpga;
  CHA_PROG_KTIRT_FOR_INDIVIDUAL_DEST_NODE_ID          ChaProgramRouteTableForIndividualDestNodeId;
  CHA_PROG_FULL_KTI_ROUTE_TABLE                       ChaProgramFullKtiRouteTable;
  CHA_PROGRAMM_SNC_CONFIG                             ChaProgramSncConfig;
  CHA_CFG_CHA_TO_M2UPI_CREDIT                         ChaConfigureChaToM2UpiCredit;
  CHA_CFG_CHA_TO_M2PCIE_CREDIT                        ChaConfigureChaToM2PCIeCredit;
  CHA_ENABLE_ISOC_IN_CHA                              ChaEnableIsocInCha;
  CHA_CFG_SNOOP_FANOUT                                ChaConfigureSnoopFanout;
  CHA_DISABLE_EGRESS_ISOC_RES                         ChaDisableEgressIsocRes;
  CHA_CFG_HA_COH_TOR                                  ChaConfigureHaCohTor;
  CHA_CFG_HA_COH1                                     ChaConfigureHaCoh1;
  CHA_CFG_STALE_ATOS_OPT_EN                           ChaConfigureStaleAtoSOptEn;
  CHA_CFG_OSB_SETTING                                 ChaConfigureOsbSetting;
  CHA_CFG_HA_COH_KTI_ENABLE_ACTIVE_NODE_DATAM         ChaConfigureHaCohKtiEnableActiveNodeAndDataM;
  CHA_CHECK_2LM_ENABLED                               ChaCheck2LMEnabled;
  CHA_CFG_PWR_HEURISTICS                              ChaConfigurePowerHeuristics;
  REPROGRAM_PBSP_BUS_MMCFG                            ReprogramPbspBusmmcfg;
  CHA_CFG_ISOC                                        ChaConfigureIsoc;
  CHA_CBO_COH_ENABLE_XOR_BASED_KTI_LINK               ChaCboCohEnableXorBasedKtilink;
  CHA_CFG_CBO_COH                                     ChaConfigureCboCoh;
  CHA_CONFIGURE_IPQ_AND_ENABLEPUSHMTOIIODCTWOLMOPT    ChaConfigureIpqAndEnablePushMtoIIodcTwoLMOpt;
  CHA_CFG_DBP                                         ChaConfigureDbp;
  CHA_CFG_HOPS_NUM                                    ChaConfigureHopsNum;
  CHA_CFG_CBSADS_UNICAST_CFG_SPARE                    ChaConfigureCbsadsUnicastCfgSpare;
  CHA_ENABLE_CREDIT_CHANGE                            ChaEnableCreditChange;
  CHA_CFG_TOR_THRESHOLDS                              ChaConfigureTorThresholds;
  CHA_CFG_DDRT_QOS_TOR_THRESHOLDS                     ChaConfigureDDRTQoSTorThresholds;
  CHA_KTI_BST_RT_ENABLE                               ChaKtiBroadcastRouteTableEnable;
  CHA_CFG_HA_OSB_SAD                                  ChaConfigureHaOsbSad;
  CHA_CFG_UBOX_MMIO                                   ChaConfigureUboxMmio;
  CHA_PROGRAMM_SNC_IN_NON_SNC_MODE                    ChaProgrammSncInNonSncMode;
  CHA_CFG_CHA2M2M_CREDIT_WITHOUT_CLUSTER              ChaConfigureChaToM2MCreditWithoutCluster;
  CHA_CFG_CHA2M2M_WITH_CLUSTER                        ChaConfigureChaToM2MCreditWithCluster;
  CHA_H0_CH_RT_READ                                   ChaH0ChRouteTableRead;
  CHA_CFG_SNC                                         ChaConfigureSnc;
  CHA_GET_SNC_CONFIG                                  ChaGetSncConfig;
  CHA_CHECK_XPT_PREFETCH_DISABLE_STATUS               ChaCheckXptPrefetchDisableStatus;
  CHA_DUMP_SNC_XPT_KTI_REG                            ChaDumpSncXptKtiRegister;
  CHA_CFG_XPT_MINI_SAD                                ChaConfigureXptMiniSAD;
  CHA_CFG_XPT_PREFETCH                                ChaConfigureXptPrefetch;
  CHA_SET_SNC_BASE_ADDRESS                            ChaSetSncBaseAddress;
  CHA_CHECK_SOCKET_OWN_MEM_BELOW_4G                   ChaCheckSocketOwnMemoryBelow4G;
  CHA_CHECK_2LM_ADD_MASK_CONFIGED                     ChaCheck2LMAddressMaskConfigured;
  CHA_CFG_2LM_ADD_MASK                                ChaConfigure2LMAddressMask;
  CHA_GET_SNC_CONFIGURATION                           ChaGetSncConfiguration;
  CHA_CFG_CHA2M2M_CREDIT                              ChaConfigureChaToM2MCredit;
  CHA_WA_CFG_HABL_CREDIT                              ChaWAToConfigureHaBlCredit;
  CHA_SET_SAD_DATA                                    ChaSetSadData;
  CHA_SET_REMOTE_SAD_RULE                             ChaSetRemoteSadRule;
  CHA_SET_RT_MODE                                     ChaSetRtMode;
  CHA_SET_RT_TYPE                                     ChaSetRtType;
  CHA_SET_BLOCK_DECODER_BASE                          ChaSetBlockDecoderBase;
  CHA_SET_BLOCK_DECODER_ENABLE_CFG                    ChaSetBlockDecoderEnableCfg;
  CHA_SET_GLOBAL_INTERLEAVE                           ChaSetGlobalInterleave;
  CHA_SET_ROUTE_TABLE                                 ChaSetRouteTable;
  CHA_SET_2LM_REGS                                    ChaSet2lmRegisters;
  CHA_DISABLE_EARLY_READ                              ChaDisableEarlyRead;
  CHA_ELIM_DIR_IN_FM                                  ChaEliminateDirectoryInFarMemory;
  CHA_DIS_DIR_FOR_AD                                  ChaDisableDirForAppDirect;
  CHA_CFG_MMIO_TARGET_LIST                            ChaConfigureMmioTargetList;
  CHA_CFG_MMIOL_RULE                                  ChaConfigureMmiolRule;
  CHA_SET_HA_CREDIT_CHANGE                            ChaSetHaCreditChange;
  CHA_UNLOCK_DRAM_CLUSTER                             ChaUnlockDramCluster;
  CHA_LOCK_DRAM_CLUSTERS                              ChaLockDramClusters;
  CHA_GET_CLUSTER_DOMAIN_HANDLE                       ChaGetClusterDomainHandle;
  CHA_CONFIGURE_CLOCK_MODULATION                      ChaConfigureClockModulation;
  CHA_SET_SNOOP_THROTTLE                              ChaSetSnoopThrottle;
  CHA_SNP_FANOUT_TABLE_PER_ENTRY_SIZE                 ChaSnpFanoutTablePerEntrySize;
} CHA_IP_INTERFACE;

//
// This API supports multiple instances with the GUID identifying the instance
//
#define CHA_IP_INTERFACE_CPU_GUID \
  { 0xaa83b114, 0xb7c9, 0x4cfa, {0xb9, 0x8e, 0x8d, 0x7a, 0x26, 0xbb, 0x8c, 0x69 }} // {AA83B114-B7C9-4CFA-B98E-8D7A26BB8C69}
extern EFI_GUID gChaIpInterfaceCpuGuid;

#define CHA_IP_INTERFACE_PCIE_GEN4_GUID \
  { 0xe2e90cf1, 0x214c, 0x469d, {0xb5, 0xcf, 0xc7, 0xb4, 0x86, 0x7f, 0xf1, 0xf8}} // {E2E90CF1-214C-469D-B5CF-C7B4867FF1F8}
extern EFI_GUID gChaIpInterfacePcieGen4Guid;

#endif // _CHA_IP_INTERFACE_H_
