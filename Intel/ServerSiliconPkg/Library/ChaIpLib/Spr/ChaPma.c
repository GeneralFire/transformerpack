/** @file
  Configure CHA_PMA* registers for 10nm.

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
#include "Include/ChaRegs.h"
#include <Library/UsraCsrLib.h>
#include <Library/ChaLib.h>
#include <Library/KtiApi.h>
#include <Upi/KtiHost.h>
#include <Library/MemoryCoreLib.h>
#include <Library/UpiHostDataLib.h>
#include <KtiSetupDefinitions.h>
#include <UncoreCommonIncludes.h>
#include <Guid/SocketCommonRcVariable.h>
#include <Library/BaseLib.h>
#include <Library/SiliconWorkaroundLib.h>

EFI_STATUS
EFIAPI
ChaConfigureQpiXncmask(
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   QpiXncmask
);

/**
  CHA: Configure ISOC Mode

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param IsocEnable           - Enable ISOC mode. This will be used for TOR
                                pipeline to reserve TOR entries for ISOC

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureIsoc (
  UINT8    SocId,
  UINT8    BoxInst,
  BOOLEAN  IsocEnable
  )
{
  CBO_ISOC_CONFIG_CHA_PMA_STRUCT             CboIsocConfig;

  CboIsocConfig.Data = UsraCsrRead (SocId, BoxInst, CBO_ISOC_CONFIG_CHA_PMA_REG);
  if (IsocEnable == TRUE) {
    CboIsocConfig.Bits.isoc_enable = 1;     //Set ISOC flag
  }
  UsraCsrWrite (SocId, BoxInst, CBO_ISOC_CONFIG_CHA_PMA_REG, CboIsocConfig.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Configure Demand Priority for local

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param EnableDemandPriority - Enable demand priority

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureDemandPriorityLocal (
  UINT8    SocId,
  UINT8    BoxInst,
  BOOLEAN  EnableDemandPriority
  )
{
  CBO_ISOC_CONFIG_CHA_PMA_SPRC0_SPRMCC_SPRUCC_STRUCT       CboIsocConfigC0MccUcc;

  CboIsocConfigC0MccUcc.Data = UsraCsrRead (SocId, BoxInst, CBO_ISOC_CONFIG_CHA_PMA_REG);
  CboIsocConfigC0MccUcc.Bits.enabledemandprioritylocal = EnableDemandPriority;
  UsraCsrWrite (SocId, BoxInst, CBO_ISOC_CONFIG_CHA_PMA_REG, CboIsocConfigC0MccUcc.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Configure Demand Priority Remote

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param EnableDemandPriority - Enable demand priority

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureDemandPriorityRemote (
  UINT8    SocId,
  UINT8    BoxInst,
  BOOLEAN  EnableDemandPriority
  )
{
  CBO_ISOC_CONFIG_CHA_PMA_SPRC0_SPRMCC_SPRUCC_STRUCT       CboIsocConfigC0Mcc;

  CboIsocConfigC0Mcc.Data = UsraCsrRead (SocId, BoxInst, CBO_ISOC_CONFIG_CHA_PMA_REG);
  CboIsocConfigC0Mcc.Bits.enabledemandpriorityremote = EnableDemandPriority;
  UsraCsrWrite (SocId, BoxInst, CBO_ISOC_CONFIG_CHA_PMA_REG, CboIsocConfigC0Mcc.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Use PA6 xor PA12 value to decide the Intel UPI link to use in 2S-2K case

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based
  @param EnableXorBasedKtiLink - Use PA6 xor PA12 value to decide the Intel UPI
                                 link to use in 2S-2K case

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaCboCohEnableXorBasedKtilink (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   EnableXorBasedKtiLink
  )
{
  return EFI_UNSUPPORTED;
}

/**
  CHA: Check whether Overall Two LM Enable

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param TwoLMEnabled         - TRUE: 2LM enabled; FALSE: 2LM not enabled

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaCheck2LMEnabled (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* TwoLMEnabled
  )
{
  TWO_LM_CONFIG_CHA_PMA_STRUCT            cboTwoLMConfig;

  cboTwoLMConfig.Data = UsraCsrRead (SocId, BoxInst, TWO_LM_CONFIG_CHA_PMA_REG);

  *TwoLMEnabled = (cboTwoLMConfig.Bits.enable == 1) ? TRUE : FALSE;

  return EFI_SUCCESS;
}

/**
  CHA: Configure HOPs Number

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param NumOfHops            - Number of HOPs

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureHopsNum (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   NumOfHops
)
{
  UsraCsrWrite (SocId, BoxInst, NUM_OF_HOPS_CHA_PMA_REG, NumOfHops);

  return EFI_SUCCESS;
}

/**
  CHA: Configure System snoop filter for FPGA

  @param CpuList              - List of CPUs populated in the system
  @param SocketData           - Socket specific data structure

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureSnoopFilterForFpga (
  UINT32                     CpuList,
  KTI_SOCKET_DATA            *SocketData
  )
{
  UINT8   Socket, Ctr;
  REMOTE_SF_CHA_PMA_STRUCT        RemoteSF;
  UINT32                          QpiXncmask;

  RemoteSF.Data = 0;
  QpiXncmask      = 0;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((SocketData->Cpu[Socket].Valid == TRUE) && (SocketData->Cpu[Socket].SocType == SOCKET_TYPE_FPGA)) {
      if (RemoteSF.Bits.fpga_0_valid == 0) {
        RemoteSF.Bits.fpga_0_nodeid = Socket;
        RemoteSF.Bits.fpga_0_valid = 1;
      } else {
        RemoteSF.Bits.fpga_1_nodeid = Socket;
        RemoteSF.Bits.fpga_1_valid = 1;
      }
    }
  }

  //
  // if fpga installed, activate the filter and program
  //
  if (RemoteSF.Bits.fpga_0_valid == 1) {
    RemoteSF.Bits.snoop_filter_active = 1;
    RemoteSF.Bits.snoop_filter_size = 0;

    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (CpuList & (1 << Socket)) {
        for (Ctr = 0; Ctr < GetTotChaCount (Socket); Ctr++) {
          UsraCsrWrite (Socket, Ctr, REMOTE_SF_CHA_PMA_REG, RemoteSF.Data);
        }
        ChaConfigureQpiXncmask (Socket, 0, QpiXncmask);
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  CHA: Programm SNC register in Non-SNC mode

  @param SocId                - CPU Socket Node number (Socket ID)
  @param ChaCount             - Total CHA Number

  @retval                     - EFI_SUCCESS: success
                              - EFI_UNSUPPORTED: unsupported register on this HW IP
                              - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaProgrammSncInNonSncMode (
  UINT8    SocId,
  UINT8    ChaCount
  )
{
  UINT8                           Cha;
  CHA_SNC_CONFIG_CHA_PMA_STRUCT   SncConfigPma10nm;

  SncConfigPma10nm.Data = UsraCsrRead (SocId, 0, CHA_SNC_CONFIG_CHA_PMA_REG);
  SncConfigPma10nm.Bits.full_snc_en = 0;
  SncConfigPma10nm.Bits.snc_ind_en = 1;

  for (Cha = 0; Cha < ChaCount; Cha++) {
    UsraCsrWrite (SocId, Cha, CHA_SNC_CONFIG_CHA_PMA_REG, SncConfigPma10nm.Data);
  }

  return EFI_SUCCESS;
}

/**
  CHA: Configure Cbo Coherency

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based
  @param FpgaList              - FPGA List
  @param SbspSoc               - SBSP socket Node number

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter
**/
EFI_STATUS
EFIAPI
ChaConfigureCboCoh (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   FpgaList,
  UINT8    SbspSoc
  )
{
  CBO_COH_CONFIG_CHA_PMA_STRUCT     CboCoh;
  KTI_HOST_OUT                      *KtiHostOutPtr;
  KTI_HOST_IN                       *KtiHostInPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;
  KtiHostInPtr  = KTI_HOST_IN_PTR;

  CboCoh.Data = UsraCsrRead (SocId, BoxInst, CBO_COH_CONFIG_CHA_PMA_REG);
  //
  // CHA Uncore Perf CSRs ICX-SP
  // Set mtoibias to 0 for all configurations on ICX-SP
  //
  CboCoh.Bits.mtoibias = 0;

  CboCoh.Bits.biasfwd = 1;
  CboCoh.Bits.biasfwddoubledata = 1;
  CboCoh.Bits.waitfordatacmp = 0;
  CboCoh.Bits.llcmissendrd = 1;
  CboCoh.Bits.drdgosone = 0;

  if ((KtiHostOutPtr->SysConfig == SYS_CONFIG_1S) && (FpgaList == 0)) {
    CboCoh.Bits.ego = 1;
    CboCoh.Bits.rrq_disable = 1;
  }

  if (KtiHostOutPtr->OutIsocEn == TRUE && SocId != SbspSoc) {
    CboCoh.Bits.disisoctorres = 1;
  }

  if (KtiHostOutPtr->OutIoDcMode == IODC_EN_REM_INVITOM_AND_WCILF) {
    CboCoh.Bits.enableitomforwcilf = 1;
  }

  if (KtiHostInPtr->DfxParm.DfxBiasFwdMode == BIAS_FWD_MODE0) {
    CboCoh.Bits.biasfwd = 1;
    CboCoh.Bits.biasfwddoubledata = 0;
  } else if (KtiHostInPtr->DfxParm.DfxBiasFwdMode == BIAS_FWD_MODE1) {
    CboCoh.Bits.biasfwd = 1;
    CboCoh.Bits.biasfwddoubledata = 0;
  } else if (KtiHostInPtr->DfxParm.DfxBiasFwdMode == BIAS_FWD_MODE2) {
    CboCoh.Bits.biasfwd = 0;
  } else if (KtiHostInPtr->DfxParm.DfxBiasFwdMode == BIAS_FWD_MODE3) {
    CboCoh.Bits.biasfwd = 1;
    CboCoh.Bits.biasfwddoubledata = 1;
  } else if (KtiHostInPtr->DfxParm.DfxBiasFwdMode == BIAS_FWD_MODE4) {
    CboCoh.Bits.biasfwd = 1;
    CboCoh.Bits.biasfwddoubledata = 1;
  }

  if (KtiHostInPtr->DfxParm.DfxFrcfwdinv < KTI_AUTO) {
    CboCoh.Bits.frcfwdinv = KtiHostInPtr->DfxParm.DfxFrcfwdinv;
  }

  if (KtiHostInPtr->DfxParm.DfxLlcShareDrdCrd < KTI_AUTO) {
    CboCoh.Bits.llcsharedrdcrd = KtiHostInPtr->DfxParm.DfxLlcShareDrdCrd;
  }

  UsraCsrWrite (SocId, BoxInst, CBO_COH_CONFIG_CHA_PMA_REG, CboCoh.Data);

  return EFI_SUCCESS;
}

/**
  CHA: configure Configure Cbo Coherency 1 to cover IPQ disable and EnablePushMtoIIodcTwoLMOpt
  in this routine.

  @param SocId                       - CPU Socket Node number (Socket ID)
  @param BoxInst                     - Box Instance, 0 based
  @param IpqDisable                  - IPQ disable
  @param EnablePushMtoIIodcTwoLMOpt  - Enable PushMtoI IODC 2LM Optimization.

  @retval                            - EFI_SUCCESS: success
                                     - EFI_UNSUPPORTED: unsupported register on this HW IP
                                     - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureIpqAndEnablePushMtoIIodcTwoLMOpt (
  UINT8     SocId,
  UINT8     BoxInst,
  BOOLEAN   IpqDisable,
  BOOLEAN   EnablePushMtoIIodcTwoLMOpt
  )
{
  CBO_COH_CONFIG1_CHA_PMA_STRUCT    CboCohCfg1;

  CboCohCfg1.Data = UsraCsrRead (SocId, BoxInst, CBO_COH_CONFIG1_CHA_PMA_REG);

  CboCohCfg1.Bits.ipq_disable = IpqDisable;
  CboCohCfg1.Bits.enablepushmtoiiodctwolmopt = EnablePushMtoIIodcTwoLMOpt;

  UsraCsrWrite (SocId, BoxInst, CBO_COH_CONFIG1_CHA_PMA_REG, CboCohCfg1.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Configure DBP

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureDbp (
  UINT8    SocId,
  UINT8    BoxInst
  )
{
  DBP_CONFIG_CHA_PMA_STRUCT         DbpConfigCha;
  KTI_HOST_IN                       *KtiHostInPtr;

  KtiHostInPtr  = KTI_HOST_IN_PTR;

  DbpConfigCha.Data = UsraCsrRead (SocId, BoxInst, DBP_CONFIG_CHA_PMA_REG);

  DbpConfigCha.Bits.enable = 1;
  if (KtiHostInPtr->LLCDeadLineAlloc == KTI_DISABLE) {
    DbpConfigCha.Bits.deadonvalidllc = 1;
  } else {
    DbpConfigCha.Bits.deadonvalidllc = 0;
  }

  if (KtiHostInPtr->DfxParm.DfxCleanEvictAlwaysLive < KTI_AUTO) {
    DbpConfigCha.Bits.cleanevictalwayslive = KtiHostInPtr->DfxParm.DfxCleanEvictAlwaysLive;
  }
  if (KtiHostInPtr->DfxParm.DfxModifiedEvictAlwaysLive < KTI_AUTO) {
    DbpConfigCha.Bits.modifiedevictalwayslive = KtiHostInPtr->DfxParm.DfxModifiedEvictAlwaysLive;
  }
  if (KtiHostInPtr->DfxParm.DfxDbpEnable < KTI_AUTO) {
    DbpConfigCha.Bits.enable = KtiHostInPtr->DfxParm.DfxDbpEnable;
  }

  UsraCsrWrite (SocId, BoxInst, DBP_CONFIG_CHA_PMA_REG, DbpConfigCha.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Check Xpt Prefetch Disabled status

  @param SocId                    - CPU Socket Node number (Socket ID)
  @param BoxInst                  - Box Instance, 0 based
  @param XptPrefetchDisableStatus - 1: Xpt Prefetch disabled
                                  - 0: Xpt Prefetch not disabled


  @retval                         - EFI_UNSUPPORTED: the function not implemented
                                    EFI_SUCCESS: the function is excuted successfully
**/
EFI_STATUS
EFIAPI
ChaCheckXptPrefetchDisableStatus (
  IN  UINT8   SocId,
  IN  UINT8   BoxInst,
  OUT UINT8*  XptPrefetchDisableStatus
  )
{
  return EFI_UNSUPPORTED;
}

/**
  CHA: Dump SNC XPT KTI related register

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based


  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
ChaDumpSncXptKtiRegister (
  UINT8    SocId,
  UINT8    BoxInst
  )
{
  return EFI_UNSUPPORTED;
}

/**
  CHA: Populate the specific destination socket field in the requested SocID's
  KTI Routing Table.

  @param[in] SocId                CPU Socket Node number (Socket ID)
  @param[in] ChaCount             Total number of CHAs for socket "SocId"
  @param[in] FieldMask            Clear the entry (or entries) of the desination socket
  @param[in] PortSetting          Values to be written into entry (or entries) of destination socket

  @retval  EFI_SUCCESS            CHA KTI_ROUTING_TABLE register was updated successfully.
           EFI_UNSUPPORTED        Unsupported register on this HW IP
           EFI_INVALID_PARAMETER  Invalid parameter

**/
EFI_STATUS
EFIAPI
ChaProgramRouteTableForIndividualDestNodeId (
  IN UINT8    SocId,
  IN UINT8    ChaCount,
  IN UINT64   FieldMask,
  IN UINT64   PortSetting
  )
{
  UINT64              UpiRouting64;

  UpiRouting64  = UsraCsrRead (SocId, 0, UPI_ROUTING_TABLE_BCST_N0_CHABC_SAD1_REG) + \
                  LShiftU64 (UsraCsrRead (SocId, 0, UPI_ROUTING_TABLE_BCST_N1_CHABC_SAD1_REG), 32);
  UpiRouting64 = UpiRouting64 & ~FieldMask;
  UpiRouting64 = UpiRouting64 | PortSetting;

  UsraCsrWrite (SocId, 0, UPI_ROUTING_TABLE_BCST_N0_CHABC_SAD1_REG, (UINT32)(UpiRouting64 & 0xFFFFFFFF));
  UsraCsrWrite (SocId, 0, UPI_ROUTING_TABLE_BCST_N1_CHABC_SAD1_REG, (UINT32)(RShiftU64 (UpiRouting64, 32)));

  return EFI_SUCCESS;
}

/**
  CHA: Determin the UPI routing when UPI affinity is enabled
  This is an internal function only used in this file

  @param[in] SocId                      CPU Socket Node number (Socket ID)
  @param[in] UpiAffinityTopology        UPI affinity topology 2C2L/4C2L/2C4L/4C4L
  @param[in] ClusterIndex               Cluster Id
  @param[in] UpiRoutingMode             UPI routing mode, see UPI_ROUTING_MODE enum.

  @retval  UPI routing for CHA

**/
UINT64
EFIAPI
ChaDeterminUpiRoutingForUpiAffinity (
  IN UINT8              SocId,
  IN UINT8              UpiAffinityTopology,
  IN UINT8              ClusterIndex,
  IN UINT8              UpiRoutingMode
  )
{
  UINT8                             C2UMapping_4C4L[] = {0, 2, 1, 3};
  UINT8                             C2UMapping_2C4L[2][2] = {{0, 2}, {1, 3}};
  UINT8                             C2UMapping_2C2L[] = {0, 1};
  UINT8                             C2UMapping_4C2L[] = {0, 0, 1, 1};
  UINT64                            ChaRouting = 0;
  UINT32                            BitOffset;
  UINT8                             NodeEntriesWidth;

  if (UpiRoutingMode == UpiRoutingModeB2) {
    NodeEntriesWidth = 6;  // 2 entries, each entry has 3 bits
  } else if (UpiRoutingMode == UpiRoutingModeD0) {
    NodeEntriesWidth = 12; // 4 entries, each entry has 3 bits
  } else {
    ASSERT (FALSE);
  }
  BitOffset = SocId * NodeEntriesWidth;

  if (UpiAffinityTopology == UPI_AFFINITY_2C2L) {
    //
    // 2L SNC2/Hemi: Cluster0->U0;Cluster1->U1
    //
    ChaRouting = LShiftU64 ((C2UMapping_2C2L[ClusterIndex] | LShiftU64 (C2UMapping_2C2L[ClusterIndex], 3)), BitOffset);
  } else if (UpiAffinityTopology == UPI_AFFINITY_4C2L) {
    //
    // 2L SNC4/Quad: Cluster0->U0;Cluster1->U0;Cluster2->U1;Cluster3->U1
    //
    ChaRouting = LShiftU64 ((C2UMapping_4C2L[ClusterIndex] | LShiftU64 (C2UMapping_4C2L[ClusterIndex], 3)), BitOffset);

  } else if (UpiAffinityTopology == UPI_AFFINITY_2C4L) {
    //
    // 4L SNC2/Hemi: Cluster0->U0/U2;Cluster1->U1/U3
    //
    ChaRouting = LShiftU64 ((C2UMapping_2C4L[ClusterIndex][0] | LShiftU64 (C2UMapping_2C4L[ClusterIndex][1], 3) | \
                             LShiftU64 (C2UMapping_2C4L[ClusterIndex][0], 6) | LShiftU64 (C2UMapping_2C4L[ClusterIndex][1], 9)), BitOffset);
  } else {
    //
    // SNC4/Quad: Cluster0->U0;Cluster1->U2;Cluster2->U1;Cluster3->U3
    //
  ChaRouting = LShiftU64 ((C2UMapping_4C4L[ClusterIndex] | LShiftU64 (C2UMapping_4C4L[ClusterIndex], 3) | \
                           LShiftU64 (C2UMapping_4C4L[ClusterIndex], 6) | LShiftU64 (C2UMapping_4C4L[ClusterIndex], 9)), BitOffset);
  }

  return ChaRouting;
}

/**
  CHA: Program KTI routing table register(s)

  @param[in] SocId                      CPU Socket Node number (Socket ID)
  @param[in] ChaCount                   Total number of CHAs for socket "SocId"
  @param[in] SysConfig                  Topology type
  @param[in] DualLinksInterleavingMode  Dual Links Interleaving Mode
  @param[in] SbspSktId                  SBSP CPU Node ID
  @param[in] SocketData                 Socket specific data structure
  @param[in] UpiRoutingMode             UPI routing mode, see UPI_ROUTING_MODE enum.

  @retval  EFI_SUCCESS            CHA KTI_ROUTING_TABLE register(s) was updated successfully.
           EFI_UNSUPPORTED        Unsupported register on this HW IP
           EFI_INVALID_PARAMETER  Invalid parameter

**/
EFI_STATUS
EFIAPI
ChaProgramFullKtiRouteTable (
  IN UINT8              SocId,
  IN UINT8              ChaCount,
  IN UINT8              SysConfig,
  IN UINT8              DualLinksInterleavingMode,
  IN UINT8              SbspSktId,
  IN KTI_SOCKET_DATA    *SocketData,
  IN UINT8              UpiRoutingMode
  )
{
  UINT8                             Ctr;
  UINT32                            OddChaData;
  UINT32                            EvenChaData;
  UINT32                            FieldMask;
  UINT32                            ChaKtiRtLow;
  UINT32                            ChaKtiRtHigh;
  KTI_HOST_OUT                      *KtiHostOutPtr;
  UINT8                             PeerSocId;
  UINT8                             ClusterIndex;
  UINT8                             ClusterNum;
  UINT8                             ChaPerCluster;
  UINT8                             UpiInterleaveMode;
  UINT64                            Data64;
  TOPOLOGY_UPI_AFFINITY_ENUM        UpiAffinityTopology;
  UPI_ROUTING_TABLE_BCST_N1_CHABC_SAD1_STRUCT KtiRoutingTableBcst;
  UPI_ROUTING_TABLE_N0_CHA_PMA_STRUCT         KtiRoutingTable;
  UPI_ROUTING_CONFIG_CHABC_SAD1_STRUCT        KtiRoutingConfig;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;
  FieldMask = 0;
  OddChaData = 0;
  EvenChaData = 0;

  //
  // Program CHA Route Table; it is non BC register. So we need to program in each cha.
  // For 2S 2KTI/2S 3KTI case, we need to enable the CHA and M2mem interleave mode.
  //
  if ((DualLinksInterleavingMode == DFX_DUAL_LINK_INTLV_MODE_CHA_INTLV) && (SysConfig == SYS_CONFIG_2S2L)) {
    ChaInterleaveKtiRouteTableSettingFor2S2L (
      SocId,
      SbspSktId,
      SocketData,
      UpiRoutingMode,
      &OddChaData,
      &EvenChaData,
      &FieldMask
      );
  }

  //
  // 4Die layout
  // ------------------------
  // |UPI0      |       UPI1|
  // |  Die0    |   Die2    |
  // |          |           |
  // ------------------------
  // |  Die1    |   Die3    |
  // |          |           |
  // |UPI2      |       UPI3|
  // ------------------------
  if (KtiHostOutPtr->UpiAffinityEn) {
    PeerSocId = (UINT8) KtiHostOutPtr->CpuInfo[SocId].LepInfo[0].PeerSocId;
    if (SysConfig == SYS_CONFIG_2S2L) {
      if (KtiHostOutPtr->OutSncEn == 4 ||
          ((KtiHostOutPtr->OutSncEn == 0) && (KtiHostOutPtr->OutUmaClustering == UMABASEDCLUSTERING_QUADRANT))) {
        ChaPerCluster = ChaCount >> 2;
        ClusterNum = 4;
        UpiAffinityTopology = UPI_AFFINITY_4C2L;
      } else {
        ChaPerCluster = ChaCount >> 1;
        ClusterNum = 2;
        UpiAffinityTopology = UPI_AFFINITY_2C2L;
      }
    } else { // 2S4L
      if (KtiHostOutPtr->OutSncEn == 4 ||
          ((KtiHostOutPtr->OutSncEn == 0) && (KtiHostOutPtr->OutUmaClustering == UMABASEDCLUSTERING_QUADRANT))) {
        ChaPerCluster = ChaCount >> 2;
        ClusterNum = 4;
        UpiAffinityTopology = UPI_AFFINITY_4C4L;
      } else {
        ChaPerCluster = ChaCount >> 1;
        ClusterNum = 2;
        UpiAffinityTopology = UPI_AFFINITY_2C4L;
      }
    }

    for (ClusterIndex = 0; ClusterIndex < ClusterNum; ClusterIndex++) {
      for (Ctr = 0; Ctr < ChaPerCluster; Ctr++) {
        Data64 = ChaDeterminUpiRoutingForUpiAffinity (PeerSocId, UpiAffinityTopology, ClusterIndex, UpiRoutingMode);

        ChaKtiRtLow  = (UINT32) (Data64 & 0xFFFFFFFF);
        ChaKtiRtHigh = (UINT32) RShiftU64 (Data64, 32);
        UsraCsrWrite (SocId, ClusterIndex * ChaPerCluster + Ctr, UPI_ROUTING_TABLE_N0_CHA_PMA_REG, ChaKtiRtLow);

        KtiRoutingTable.Data = UsraCsrRead (SocId, ClusterIndex * ChaPerCluster + Ctr, UPI_ROUTING_TABLE_N1_CHA_PMA_REG);
        KtiRoutingTable.Bits.logical_upi_target = ChaKtiRtHigh;
        UsraCsrWrite (SocId, ClusterIndex * ChaPerCluster + Ctr, UPI_ROUTING_TABLE_N1_CHA_PMA_REG, ChaKtiRtHigh);
      }
    }
  } else {
    // s4930277
    if (DualLinksInterleavingMode == DFX_DUAL_LINK_INTLV_MODE_CHA_INTLV && SysConfig == SYS_CONFIG_2S2L) {
      for (Ctr = 0; Ctr < ChaCount; Ctr++) {
        //
        // Interleave based on CHA ID[0]:
        //    even CHAs - route to Socket1 through Port0
        //    odd CHAs  - route to Socket1 through Port1 or Port2
        //
        KtiRoutingTable.Data = (Ctr % 2) ? OddChaData : EvenChaData;
        KtiRoutingTable.Data |= (FieldMask & (UsraCsrRead (SocId, Ctr, UPI_ROUTING_TABLE_N0_CHA_PMA_REG)));
        UsraCsrWrite (SocId, Ctr, UPI_ROUTING_TABLE_N0_CHA_PMA_REG, KtiRoutingTable.Data);
      }
    } else {
      ChaKtiRtLow  = SocketData->Route[SocId].ChaKtiRt | (SocketData->Route[SocId].ChaKtiRt1 << 24);
      ChaKtiRtHigh = SocketData->Route[SocId].ChaKtiRt1 >> 8;
      UsraCsrWrite (SocId, 0, UPI_ROUTING_TABLE_BCST_N0_CHABC_SAD1_REG, ChaKtiRtLow);

      KtiRoutingTableBcst.Data = UsraCsrRead (SocId, 0, UPI_ROUTING_TABLE_BCST_N1_CHABC_SAD1_REG);
      KtiRoutingTableBcst.Bits.logical_upi_target = ChaKtiRtHigh;
      UsraCsrWrite (SocId, 0, UPI_ROUTING_TABLE_BCST_N1_CHABC_SAD1_REG, ChaKtiRtHigh);
    }
  }

  switch (UpiRoutingMode) {
  case UpiRoutingModeB2:
    UpiInterleaveMode = 0;
    break;
  case UpiRoutingModeC1:
    UpiInterleaveMode = 1;
    break;
  case UpiRoutingModeD0:
    UpiInterleaveMode = 2;
    break;
  default:
    UpiInterleaveMode = 0;
    break;
  }

  //
  // Configure UPI interleave mode
  //
  KtiRoutingConfig.Data = (UINT16)UsraCsrRead (SocId, 0, UPI_ROUTING_CONFIG_CHABC_SAD1_REG);
  KtiRoutingConfig.Bits.upi_interleave_mode = UpiInterleaveMode;
  UsraCsrWrite (SocId, 0, UPI_ROUTING_CONFIG_CHABC_SAD1_REG, KtiRoutingConfig.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Get SNC Base Starting Address

  @param SocId                  - CPU Socket Node number (Socket ID)
  @param BoxInst                - Box Instance, 0 based
  @param SocketOwnMemoryBelow4G - TRUE  : The socket owns 0-4G in cold reset path
                                  FALSE : the socket doesn't own 0-4G in cold reset path

  @retval                       - EFI_UNSUPPORTED: the function not implemented
                                  EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
ChaCheckSocketOwnMemoryBelow4G (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* SocketOwnMemoryBelow4G
  )
{
  return EFI_UNSUPPORTED;
}

/**
  CHA: Set SNC Base Address

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param Cluster              - Cluster Number
  @param ClusterMemBase       - Cluster memory base address

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
ChaSetSncBaseAddress (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT8    Cluster,
  UINT32   ClusterMemBase
  )
{
  return EFI_UNSUPPORTED;
}

/**
  CHA: Programm SNC Configuration

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based
  @param ChaCount              - Total CHA Number
  @param Operation             - Operation type: Normal or Recovery operation
  @param UmaEn                 - UMA based clustering is enabled
  @param NumOfCluster          - Number Of Cluster
  @param BaseChaCluster1       - Number Of CHA per cluster 1
  @param BaseChaCluster2       - Number Of CHA per cluster 2
  @param BaseChaCluster3       - Number Of CHA per cluster 3
  @param SncConfiguration      - SNC configuration register value

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaProgramSncConfig (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    ChaCount,
  IN UINT8    Operation,
  IN BOOLEAN  UmaEn,
  IN UINT8    NumOfCluster,
  IN UINT8    BaseChaCluster1,
  IN UINT8    BaseChaCluster2,
  IN UINT8    BaseChaCluster3,
  OUT UINT32  *SncConfiguration
  )
{
  CHA_SNC_CONFIG_CHA_PMA_STRUCT   SncConfigPma10nm;

  SncConfigPma10nm.Data = 0;

  if (Operation == NORMAL_OPERATION) {    // SNC normal cold reset setting in cold reset flow
    //
    // Program SNC registers. In SNC2 case each cluster has 19 CHAs. In SNC4 case, SNC 0&2 have 10 CHAs; SNC 1&3 have 9 CHAs.
    // num_clusters definition in Core/CBPMA: 2'b00 2 Cluster, 2'b01 3 Clusters, 2'b10 4 Clusters, 2'b11 RSVD
    // num_clusters definition in MS2IDI/IIO/UPI/CHA: 2'b00 1 Cluster, 2'b01 2 Clusters, 2'b10 RSVD, 2'b11 4 Clusters
    //
    if (NumOfCluster == 2) {
      if (UmaEn) {
        SncConfigPma10nm.Bits.full_snc_en = 0;
      } else {
        SncConfigPma10nm.Bits.full_snc_en = 1;
      }
      SncConfigPma10nm.Bits.snc_ind_en = 1;
      SncConfigPma10nm.Bits.num_snc_clu = 1;
    } else if (NumOfCluster == 4) {
      if (UmaEn) {
        SncConfigPma10nm.Bits.full_snc_en = 0;
      } else {
        SncConfigPma10nm.Bits.full_snc_en = 1;
      }
      SncConfigPma10nm.Bits.snc_ind_en = 1;
      SncConfigPma10nm.Bits.num_snc_clu = 3;
    } else if (NumOfCluster == 1) {
      SncConfigPma10nm.Bits.snc_ind_en = 1;
    }

    SncConfigPma10nm.Bits.sncbase1id = BaseChaCluster1;
    SncConfigPma10nm.Bits.sncbase2id = BaseChaCluster2;
    SncConfigPma10nm.Bits.sncbase3id = BaseChaCluster3;


    DEBUG ((DEBUG_INFO, "\nSocket %d ClusterNum %d requested, CHA %d, SNC_CONFIG = 0x%08x",
      SocId, NumOfCluster, BoxInst, SncConfigPma10nm.Data));
    DEBUG ((DEBUG_INFO, " (num_clusters = %02d clus1= %02d clus2= %02d clus3= %02d)\n",
      SncConfigPma10nm.Bits.num_snc_clu, SncConfigPma10nm.Bits.sncbase1id,
      SncConfigPma10nm.Bits.sncbase2id, SncConfigPma10nm.Bits.sncbase3id));
  }

  UsraCsrWrite (SocId, BoxInst, CHA_SNC_CONFIG_CHA_PMA_REG, SncConfigPma10nm.Data);

  *SncConfiguration = SncConfigPma10nm.Data;

  return EFI_SUCCESS;
}
/**
  CHA: Check whether 2LM Address Mask is configured

  @param SocId                      - CPU Socket Node number (Socket ID)
  @param BoxInst                    - Box Instance, 0 based
  @param TwoLMAddressMaskConfigured - TRUE  : 2LM Address Mask is configured
                                      FALSE : 2LM Address Mask is not configured

  @retval                           - EFI_UNSUPPORTED: the function not implemented
                                      EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
ChaCheck2LMAddressMaskConfigured (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* TwoLMAddressMaskConfigured
  )
{
  return EFI_UNSUPPORTED;
}

/**
  CHA: Configure 2LM Address Mask

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based
  @param TwoLM_Address_Mask    - Point to 2LM Address Mass struct

  @retval                      - EFI_UNSUPPORTED: the function not implemented
                                 EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
ChaConfigure2LMAddressMask (
  UINT8    SocId,
  UINT8    BoxInst,
  UINTN    *TwoLM_Address_Mask
  )
{
  return EFI_UNSUPPORTED;
}

/**
  CHA: SNC Full enabled status and SNC INC Enabled status

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based
  @param SncEnableStatus       - 1: Enabled; 0: Not enabled
  @param SncIndEnabled         - 1: Enabled; 0: Not enabled
  @param SncConfiguration      - Snc configuration register value

  @retval                      - EFI_UNSUPPORTED: the function not implemented
                                 EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
ChaGetSncConfiguration (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  IN  UINT8    *SncEnableStatus,
  IN  UINT8    *SncIndEnabled,
  OUT UINT32*  SncConfiguration
  )
{
  return EFI_UNSUPPORTED;
}

/**
  CHA: Programm credits to Cha -> M2UPI

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based
  @param AdVnaCreditPerCha     - AD VNA credit per CHA
  @param BlVnaCreditPerCha     - BL VNA credit per CHA

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureChaToM2UpiCredit (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   AdVnaCreditPerCha,
  UINT32   BlVnaCreditPerCha
  )
{
  AD_VNA_CREDIT_CONFIG_N0_CHA_PMA_STRUCT  AdVnaCreditConfig0;
  BL_VNA_CREDIT_CONFIG_N0_CHA_PMA_STRUCT  BlVnaCreditConfig0;

  AdVnaCreditConfig0.Data = UsraCsrRead (SocId, BoxInst, AD_VNA_CREDIT_CONFIG_N0_CHA_PMA_REG);
  AdVnaCreditConfig0.Bits.ad_vna_port0 = AdVnaCreditPerCha;          // UPI0
  AdVnaCreditConfig0.Bits.ad_vna_port1 = AdVnaCreditPerCha;          // UPI1
  AdVnaCreditConfig0.Bits.ad_vna_port2 = AdVnaCreditPerCha;          // UPI2
  AdVnaCreditConfig0.Bits.ad_vna_port3 = AdVnaCreditPerCha;          // UPI3
  UsraCsrWrite (SocId, BoxInst, AD_VNA_CREDIT_CONFIG_N0_CHA_PMA_REG, AdVnaCreditConfig0.Data);

  BlVnaCreditConfig0.Data = UsraCsrRead (SocId, BoxInst, BL_VNA_CREDIT_CONFIG_N0_CHA_PMA_REG);
  BlVnaCreditConfig0.Bits.bl_vna_port0 = BlVnaCreditPerCha;          // UPI0
  BlVnaCreditConfig0.Bits.bl_vna_port1 = BlVnaCreditPerCha;          // UPI1
  BlVnaCreditConfig0.Bits.bl_vna_port2 = BlVnaCreditPerCha;          // UPI2
  BlVnaCreditConfig0.Bits.bl_vna_port3 = BlVnaCreditPerCha;          // UPI3
  UsraCsrWrite (SocId, BoxInst, BL_VNA_CREDIT_CONFIG_N0_CHA_PMA_REG, BlVnaCreditConfig0.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Programm credits to Cha -> Ubox

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based
  @param BlVnaCreditPerCha     - BL VNA credit per CHA

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureChaToUboxCredit (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   BlVnaCreditPerCha
  )
{
  BL_VNA_CREDIT_CONFIG_N1_CHA_PMA_STRUCT  BlVnaCreditConfig1;

  BlVnaCreditConfig1.Data = UsraCsrRead (SocId, BoxInst, BL_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG);
  BlVnaCreditConfig1.Bits.bl_vna_port10 = BlVnaCreditPerCha;          // BL VNA count for PORT10. UBOX
  UsraCsrWrite (SocId, BoxInst, BL_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG, BlVnaCreditConfig1.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Programm credits to Cha -> M2PCIe

  @param SocId                   - CPU Socket Node number (Socket ID)
  @param BoxInst                 - Box Instance, 0 based
  @param BlCreditsToM2pciePerCha - BL Credits to M2PCIe per CHA

  @retval                        - EFI_SUCCESS: success
                                 - EFI_UNSUPPORTED: unsupported register on this HW IP
                                 - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureChaToM2PCIeCredit (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   BlCreditsToM2pciePerCha
  )
{
  BL_VNA_CREDIT_CONFIG_N0_CHA_PMA_STRUCT  BlVnaCreditConfig0;
  BL_VNA_CREDIT_CONFIG_N1_CHA_PMA_STRUCT  BlVnaCreditConfig1;
  BL_VNA_CREDIT_CONFIG2_N0_CHA_PMA_STRUCT BlVnaCreditConfig2;
  AD_VNA_CREDIT_CONFIG_N1_CHA_PMA_STRUCT  AdVnaCreditConfig1;

  BlVnaCreditConfig0.Data = UsraCsrRead (SocId, BoxInst, BL_VNA_CREDIT_CONFIG_N0_CHA_PMA_REG);
  BlVnaCreditConfig0.Bits.bl_vna_port4 = BlCreditsToM2pciePerCha;   // PCIE0
  BlVnaCreditConfig0.Bits.bl_vna_port5 = BlCreditsToM2pciePerCha;   // PCIE1
  BlVnaCreditConfig0.Bits.bl_vna_port6 = BlCreditsToM2pciePerCha;   // PCIE2
  BlVnaCreditConfig0.Bits.bl_vna_port7 = BlCreditsToM2pciePerCha;   // PCIE3
  UsraCsrWrite (SocId, BoxInst, BL_VNA_CREDIT_CONFIG_N0_CHA_PMA_REG, BlVnaCreditConfig0.Data);

  BlVnaCreditConfig1.Data = UsraCsrRead (SocId, BoxInst, BL_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG);
  BlVnaCreditConfig1.Bits.bl_vna_port8 = BlCreditsToM2pciePerCha;   // PCIE4
  BlVnaCreditConfig1.Bits.bl_vna_port9 = BlCreditsToM2pciePerCha;   // PCIE5
  UsraCsrWrite (SocId, BoxInst, BL_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG, BlVnaCreditConfig1.Data);

  BlVnaCreditConfig2.Data = UsraCsrRead (SocId, BoxInst, BL_VNA_CREDIT_CONFIG2_N0_CHA_PMA_REG);
  BlVnaCreditConfig2.Bits.bl_vna_port11 = BlCreditsToM2pciePerCha;   // PCIE6
  BlVnaCreditConfig2.Bits.bl_vna_port12 = BlCreditsToM2pciePerCha;   // PCIE7
  BlVnaCreditConfig2.Bits.bl_vna_port13 = BlCreditsToM2pciePerCha;   // PCIE8
  BlVnaCreditConfig2.Bits.bl_vna_port14 = BlCreditsToM2pciePerCha;   // PCIE9
  BlVnaCreditConfig2.Bits.bl_vna_port15 = BlCreditsToM2pciePerCha;   // PCIE10
  BlVnaCreditConfig2.Bits.bl_vna_port16 = BlCreditsToM2pciePerCha;   // PCIE11
  UsraCsrWrite (SocId, BoxInst, BL_VNA_CREDIT_CONFIG2_N0_CHA_PMA_REG, BlVnaCreditConfig2.Data);

  //
  // Set disisocvnres field to disable reserving credits for isoch
  // If isoch is enabled, this will be reversed after WR
  //
  AdVnaCreditConfig1.Data = UsraCsrRead (SocId, BoxInst, AD_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG);
  AdVnaCreditConfig1.Bits.disisocvnres = 1;
  UsraCsrWrite (SocId, BoxInst, AD_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG, AdVnaCreditConfig1.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Programm credits to Cha -> M2IAL AD VNA

  @param SocId                   - CPU Socket Node number (Socket ID)
  @param BoxInst                 - Box Instance, 0 based
  @param AdCreditsToM2pciePerCha - Ad Credits to M2IAL per CHA
  @param HbmSku                  - Hbm Sku Flag

  @retval                        - EFI_SUCCESS: success
                                 - EFI_UNSUPPORTED: unsupported register on this HW IP
                                 - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaConfigureChaToM2IalAdVnaCredit (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   AdCreditsToM2pciePerCha,
  IN BOOLEAN  HbmSku
  )
{
  HA_AD_CREDITS1_CHA_MISC_STRUCT HaAdCredits1;
  HA_AD_CREDITS2_CHA_MISC_STRUCT HaAdCredits2;

  HaAdCredits1.Data = UsraCsrRead (SocId, BoxInst, HA_AD_CREDITS1_CHA_MISC_REG);

  HaAdCredits1.Bits.ad_vna_port8  = AdCreditsToM2pciePerCha;
  HaAdCredits1.Bits.ad_vna_port9  = AdCreditsToM2pciePerCha;
  HaAdCredits1.Bits.ad_vna_port10 = AdCreditsToM2pciePerCha;
  HaAdCredits1.Bits.ad_vna_port11 = AdCreditsToM2pciePerCha;
  HaAdCredits1.Bits.ad_vna_port12 = AdCreditsToM2pciePerCha;
  HaAdCredits1.Bits.ad_vna_port13 = AdCreditsToM2pciePerCha;

  if (HbmSku) {
    HaAdCredits2.Data = UsraCsrRead (SocId, BoxInst, HA_AD_CREDITS2_CHA_MISC_REG);
    HaAdCredits2.Bits.ad_vna_port14 = AdCreditsToM2pciePerCha;
    HaAdCredits2.Bits.ad_vna_port15 = AdCreditsToM2pciePerCha;
    UsraCsrWrite (SocId, BoxInst, HA_AD_CREDITS2_CHA_MISC_REG, HaAdCredits2.Data);
  } else {
    HaAdCredits1.Bits.ad_vna_port6  = AdCreditsToM2pciePerCha;
    HaAdCredits1.Bits.ad_vna_port7  = AdCreditsToM2pciePerCha;
  }

  UsraCsrWrite (SocId, BoxInst, HA_AD_CREDITS1_CHA_MISC_REG, HaAdCredits1.Data);

  return EFI_SUCCESS;
}


/**
  CHA: Enable Isoc in CHA

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaEnableIsocInCha (
  UINT8    SocId,
  UINT8    BoxInst
  )
{
  BL_VNA_CREDIT_CONFIG_N1_CHA_PMA_STRUCT     BlVnaCreditConfig1;
  HA_AD_CREDITS_CHA_MISC_STRUCT              HaAdCredit;
  HA_BL_CREDITS_CHA_MISC_STRUCT              HaBlCredit;


  BlVnaCreditConfig1.Data = UsraCsrRead (SocId, BoxInst, BL_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG);
  BlVnaCreditConfig1.Bits.disisocvnres = 0;
  UsraCsrWrite (SocId, BoxInst, BL_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG, BlVnaCreditConfig1.Data);

  HaAdCredit.Data = UsraCsrRead (SocId, BoxInst, HA_AD_CREDITS_CHA_MISC_REG);
  HaAdCredit.Bits.disadvnaisocres = 0;

  //
  // Set disblvnaisocres when one of VNA_PORT0/1 is 1
  // This can happen with SNC programming
  //
  if ((HaAdCredit.Bits.ad_vna_port0 == 1) || (HaAdCredit.Bits.ad_vna_port1 == 1)) {
    HaAdCredit.Bits.disadvnaisocres = 1;
  }
  UsraCsrWrite (SocId, BoxInst, HA_AD_CREDITS_CHA_MISC_REG, HaAdCredit.Data);

  HaBlCredit.Data = UsraCsrRead (SocId, BoxInst, HA_BL_CREDITS_CHA_MISC_REG);
  HaBlCredit.Bits.disblvnaisocres = 0;
  //
  // Set disblvnaisocres when one of VNA_PORT0/1 is 1
  // This can happen with SNC programming
  //
  if ((HaBlCredit.Bits.bl_vna_port0 == 1) || (HaBlCredit.Bits.bl_vna_port1 == 1)) {
    HaBlCredit.Bits.disblvnaisocres = 1;
  }
  UsraCsrWrite (SocId, BoxInst, HA_BL_CREDITS_CHA_MISC_REG, HaBlCredit.Data);

  return EFI_SUCCESS;
}

/**
  CHA: Get SNC Attributes

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based
  @param SncConfiguration      - SNC Attributes

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaGetSncConfig (
  IN  UINT8   SocId,
  IN  UINT8   BoxInst,
  OUT UINT32* SncConfiguration
  )
{
  *SncConfiguration = UsraCsrRead (SocId, BoxInst, CHA_SNC_CONFIG_CHA_PMA_REG);

  return EFI_SUCCESS;
}

/**
  CHA: Configure Cbsads Unicast Cfg Spare.

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param CfgValue             - Configurations

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
ChaConfigureCbsadsUnicastCfgSpare (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   CfgValue
  )
{
  return EFI_UNSUPPORTED;
}

/**
  CHA: Enable credit change without triggering warm-reset.

  @param SocId                 - CPU Socket Node number (Socket ID)
  @param BoxInst               - Box Instance, 0 based

  @retval                      - EFI_SUCCESS: success
                               - EFI_UNSUPPORTED: unsupported register on this HW IP
                               - EFI_INVALID_PARAMETER: invalid parameter

**/
EFI_STATUS
EFIAPI
ChaEnableCreditChange (
  UINT8    SocId,
  UINT8    BoxInst
  )
{
  BL_VNA_CREDIT_CONFIG_N1_CHA_PMA_STRUCT     BlVnaCreditConfig1;
  AD_VNA_CREDIT_CONFIG_N1_CHA_PMA_STRUCT     AdVnaCreditConfig1;

  AdVnaCreditConfig1.Data = UsraCsrRead (SocId, BoxInst, AD_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG);
  AdVnaCreditConfig1.Bits.vnacreditchange = 1;
  UsraCsrWrite (SocId, BoxInst, AD_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG, AdVnaCreditConfig1.Data);

  BlVnaCreditConfig1.Data = UsraCsrRead (SocId, BoxInst, BL_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG);
  BlVnaCreditConfig1.Bits.vnacreditchange = 1;
  UsraCsrWrite (SocId, BoxInst, BL_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG, BlVnaCreditConfig1.Data);

  //
  // Credits are applied on the falling edge
  //
  AdVnaCreditConfig1.Data = UsraCsrRead (SocId, BoxInst, AD_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG);
  AdVnaCreditConfig1.Bits.vnacreditchange = 0;
  UsraCsrWrite (SocId, BoxInst, AD_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG, AdVnaCreditConfig1.Data);

  BlVnaCreditConfig1.Data = UsraCsrRead (SocId, BoxInst, BL_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG);
  BlVnaCreditConfig1.Bits.vnacreditchange = 0;
  UsraCsrWrite (SocId, BoxInst, BL_VNA_CREDIT_CONFIG_N1_CHA_PMA_REG, BlVnaCreditConfig1.Data);

  return EFI_SUCCESS;
}

/**
  Silicon workaround for systems with CXL endpoint device installed.

  @param[in] SocId     - CPU Socket Node number (Socket ID)
  @param[in] NumOfCha  - The total number of Chas of this socket

  @retval              - N/A
**/
VOID
EFIAPI
WorkaroundWithCxlEpDeviceInstalled (
  IN  UINT8        SocId,
  IN  UINT8        NumOfCha
  )
{
  UINT8                             ChaIdx;
  CBO_COH_CONFIG_CHA_PMA_STRUCT     CboCoh;

  if (IsSiliconWorkaroundEnabled ("S14010086544")) {
    for (ChaIdx = 0; ChaIdx < NumOfCha; ChaIdx++) {
      CboCoh.Data = UsraCsrRead (SocId, ChaIdx, CBO_COH_CONFIG_CHA_PMA_REG);
      CboCoh.Bits.llcmissendrd = 0;
      UsraCsrWrite (SocId, ChaIdx, CBO_COH_CONFIG_CHA_PMA_REG, CboCoh.Data);
    }
  }

  return;
}
