/** @file
  File include routines to program SNC related registers

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Library/OemProcMemInitLib.h>
#include <Uefi.h>
#include <Library/PeiServicesLib.h>
#include <KtiMisc.h>
#include <KtiSetupDefinitions.h>
#include <Upi/KtiSi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemRcLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Library/IoAccessLib.h>
#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>
#include <ScratchpadList.h>
#include <Library/ReferenceCodePolicyLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/KtiSimLib.h>
#include <Library/IoLib.h>
#include <RcRegs.h>
#include <Library/ChaLib.h>
#include <Guid/UboxIpInterface.h>
#include <Library/Ms2IdiLib.h>
#include <Library/OobIpLib.h>
#include <Library/PcuIpLib.h>
#include <Library/M2IalIpLib.h>
#include <Guid/KtiIpInterface.h>
#include <Guid/IioIpInterface.h>
#include <Guid/MsmIpInterface.h>
#include <Library/CsrAccessLib.h>
#include <Library/PmcSetStrapMessageLib.h>
#include <Library/MemTypeLib.h>
#include <Library/CxlLib.h>
#include <Guid/SocketCommonRcVariable.h>
#include <Library/SncPrefetchLib.h>
#include "SncPrefetchLibInternal.h"

/**
  Program Number of CHAs per cluster

  @param SocketData          - Pointer to socket specific data
  @param PrintMissingSockets - TRUE/FALSE

  @retval KTI_SUCCESS

**/
KTI_STATUS
ProgramNumOfChaPerCluster (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT32  Ms2IdiUncoreSncCfg;
  UINT8   SocId;
  UINT8   Stack;
  UINT8   LinkIndex;
  UINT8   NumOfChaPerCluster;
  UINT8   BaseChaOfCluster1;
  UINT8   BaseChaOfCluster2;
  UINT8   BaseChaOfCluster3;
  BOOLEAN ProgramSncRegs = FALSE;

  SocId = KtiInternalGlobal->SbspSoc;
  if (KtiInternalGlobal->CpuType == CPU_ICXD ) {
    ProgramSncRegs = IsSncRegistersInHardwareDefaultSetting (SocId);
  } else if (KtiInternalGlobal->SkipWarmResetRequest) {
    return KTI_SUCCESS;
  } else {
    ProgramSncRegs = TRUE;
  }

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n ProgramNumOfChaPerCluster\n"));

  for (SocId = 0; SocId < MAX_SOCKET; SocId++) {
    if (!ProgramSncRegs) {
      continue;
    }
    if ((SocketData->Cpu[SocId].Valid == TRUE) && (SocketData->Cpu[SocId].SocType == SOCKET_TYPE_CPU)) {
      Ms2IdiUncoreSncCfg = Ms2IdiSetNumChaSncCfg (SocId, KtiInternalGlobal->TotCha[SocId]);

      //
      // Program UMA_CLUSTER_CONFIG_IIO_VTD_REG
      //
      Ms2IdiGetUncoreSncCfg (SocId, &NumOfChaPerCluster, &BaseChaOfCluster1, &BaseChaOfCluster2, &BaseChaOfCluster3);
      for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
        if (StackPresent (SocId, Stack)) {
          IioSetSncUncore (SocId, Stack, NumOfChaPerCluster, BaseChaOfCluster1, BaseChaOfCluster2, BaseChaOfCluster3);
        }
      }

      for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
        if (KTIVAR->CpuInfo[SocId].LepInfo[LinkIndex].Valid == TRUE) {
          KtiUncoreChaPerCluster (SocId, LinkIndex, Ms2IdiUncoreSncCfg);
        }
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  API callable from outside UncoreRC to program Sub Numa Clustering (SNC) mode.

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param  MemInfo          - Memory Information structure.
  @param  Socket           - Socket
  @param  Operation        - Operation type: Normal or Recovery operation

  @retval UNCORE_SUCCESS   - SNC mode is programmed successfully; or system is not configured for SNC mode operation.
  @retval UNCORE_FAILURE   - Memory profile is not suitable for SNC mode operation.

**/
KTI_STATUS
ProgramSncColdResetRegister (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo,
  UINT8                      Socket,
  UINT8                      Operation
  )
{
  UINT8   Ctr, Cha, NumOfCluster;
  UINT32  SncConfiguration = 0;
  UINT32  SncConfigMs2Idi = 0;
  UINT32  UncoreSncConfig = 0;
  UINT8   NumChaCluster   = 0;
  UINT8   BaseChaCluster1 = 0;
  UINT8   BaseChaCluster2 = 0;
  UINT8   BaseChaCluster3 = 0;
  UINT8   FullSncEnable   = 0;
  UINT8   SncIndEnable    = 0;
  UINT8   NumClusters     = 0;
  UINT8   UmaClustering   = 0;

  //
  // Calculate the SNC Base Size for each cluster. Note there is no holes allowed between the clusters except for the MMIOL hole.
  //
  UmaClustering = KTIVAR->OutUmaClustering;
  if (Operation == NORMAL_OPERATION) {    // SNC normal cold reset setting in cold reset flow
    NumOfCluster = MemInfo->SncInfo[Socket].NumOfCluster;
    //
    // Program SNC registers. In SNC2 case each cluster has 19 CHAs. In SNC4 case, SNC 0&2 have 10 CHAs; SNC 1&3 have 9 CHAs.
    // num_clusters definition in Core/CBPMA: 2'b00 2 Cluster, 2'b01 3 Clusters, 2'b10 4 Clusters, 2'b11 RSVD
    // num_clusters definition in MS2IDI/IIO/UPI/CHA: 2'b00 1 Cluster, 2'b01 2 Clusters, 2'b10 RSVD, 2'b11 4 Clusters
    //
    if (NumOfCluster == 2) {
      if (KTIVAR->OutSncEn != 0) {
        //
        // FullSncEnable is only for NUMA
        //
        FullSncEnable = 1;
      } else {
        //
        // FullSncEn doesn't set in UMA clustering mode
        //
        FullSncEnable = 0;
      }
      NumClusters   = 1;
    } else if (NumOfCluster == 4) {
      FullSncEnable = 1;
      NumClusters   = 3;
    }
    SncIndEnable = 1;

    NumChaCluster   = MemInfo->SncInfo[Socket].NumOfChaPerCluster;
    BaseChaCluster1 = MemInfo->SncInfo[Socket].NumOfChaPerCluster;

    if (NumOfCluster == 1) {
      BaseChaCluster1 = 0;
      BaseChaCluster2 = 0;
      BaseChaCluster3 = 0;
    } else if (NumOfCluster == 2) {
      BaseChaCluster2 = MemInfo->SncInfo[Socket].NumOfChaPerCluster;
      BaseChaCluster3 = MemInfo->SncInfo[Socket].NumOfChaPerCluster;
    } else if (NumOfCluster == 4) {
      BaseChaCluster2 = MemInfo->SncInfo[Socket].NumOfChaPerCluster * 2;
      BaseChaCluster3 = MemInfo->SncInfo[Socket].NumOfChaPerCluster * 3;
    }

    if (NumOfCluster == 1 || NumOfCluster == 0) {
      ProgramChaToM2mCreditWithoutCluster (KtiInternalGlobal, MemInfo, Socket);
    } else {
      // Program CHA->M2M credit based on NumOfChaPerCluster and SNC configuration
      ProgramChaToM2mCreditWithCluster (KtiInternalGlobal, MemInfo, Socket);
    }
  } else { // RECOVERY_OPERATION, SNC failure detection during warm reset
    // Recovery back to snc disabled (no cluster) credit for CHA->M2M credit
    NumOfCluster = 0;
    // hardware default setting is num_cha_per_cluster = total CHAs
    NumChaCluster = KTIVAR->CpuInfo[Socket].TotCha;
    ProgramChaToM2mCreditWithoutCluster (KtiInternalGlobal, MemInfo, Socket);
  }

  for (Cha = 0; Cha < KTIVAR->CpuInfo[Socket].TotCha; Cha++) {
    ChaProgramSncConfig (
      Socket,
      Cha,
      KTIVAR->CpuInfo[Socket].TotCha,
      Operation,
      UmaClustering > 0,
      NumOfCluster,
      BaseChaCluster1,
      BaseChaCluster2,
      BaseChaCluster3,
      &SncConfiguration
    );
  }
  UncoreSncConfig = Ms2IdiSetClustersSncCfg (Socket, NumChaCluster, BaseChaCluster1, BaseChaCluster2, BaseChaCluster3);
  SncConfigMs2Idi = Ms2IdiSetSncCfg (Socket, FullSncEnable, SncIndEnable, NumClusters);

  if (PcdGetBool (PcdPmcStrapSetVdmSupported)) {
    //
    // Also update SNC settings in PMC set_strap message data register so that
    // the set_straps are in sync with BIOS configuration.
    //
    UpdatePmcSetStrapMsgSncEn (FullSncEnable, SncIndEnable, NumClusters, NULL);
  }

  //
  // Program SNC config register in IIO STACK & Kti Agents
  //
  ProgramSncConfigInKtiAgent (KtiInternalGlobal, Socket, UncoreSncConfig, SncConfigMs2Idi);

  for (Ctr = 0; Ctr < MAX_IIO_STACK; Ctr++) {
    if (StackPresent (Socket, Ctr)) {
      IioSetSncCfg (Socket, Ctr, SncConfigMs2Idi);
      IioSetSncUncore (Socket, Ctr, NumChaCluster, BaseChaCluster1, BaseChaCluster2, BaseChaCluster3);
    }
  }

  //
  // Program UMA config register in MS2IDI
  //
  ProgramUmaClusterConfig (KtiInternalGlobal, Socket);

  return KTI_SUCCESS;
}

/**
  Program SNC BASE for all IIO stacks in "Socket"

  @param Socket            - Socket
  @param SncBaseAddr       - Array size is MAX_CLUSTERS+1, contains all SNC base address

**/
VOID
ProgramSncBaseAddrInIio (
  UINT8                    Socket,
  UINT32                   SncBaseAddr [MAX_CLUSTERS + 1]
  )
{
  UINT8   Ctr;
  UINT8   Cluster;

  for (Cluster = 0; Cluster <= MAX_CLUSTERS; Cluster++) {
    for (Ctr = 0; Ctr < MAX_IIO_STACK; Ctr++) {
      if (StackPresent (Socket, Ctr)) {
        IioSetSncBaseAddr (Socket, Ctr, Cluster, SncBaseAddr[Cluster]);
      }
    } // iio stack loop
  } // Cluster loop
}

/**
  Program SNC BASE for all MS2IDI instances in "Socket"

  @param Socket            - Socket
  @param SncBaseAddr       - Array size is MAX_CLUSTERS+1, contains all SNC base address

**/
VOID
ProgramSncBaseAddrInMs2Idi (
  UINT8                    Socket,
  UINT32                   SncBaseAddr [MAX_CLUSTERS + 1]
  )
{
  UINT8    Cluster;

  if (!IsSiliconWorkaroundEnabled ("S2202532599")) {
    for (Cluster = 0; Cluster <= MAX_CLUSTERS; Cluster++) {
      Ms2IdiSetSncBaseAddr (Socket, Cluster, (SncBaseAddr[Cluster] & 0x0000FFFF));
    }
  }
}

/**
  Program SNC BASE for all KTI agents in "Socket"

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Socket            - Socket
  @param SncBaseAddr       - Array size is MAX_CLUSTERS+1, contains all SNC base address

**/
VOID
ProgramSncBaseAddrInKti (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    Socket,
  UINT32                   SncBaseAddr [MAX_CLUSTERS + 1]
  )
{
  UINT8    Cluster;
  UINT8    Ctr;

  for (Cluster = 0; Cluster <= MAX_CLUSTERS; Cluster++) {
    for (Ctr = 0; Ctr < KtiInternalGlobal->KtiPortCnt; Ctr++) {
      if (KTIVAR->CpuInfo[Socket].LepInfo[Ctr].Valid == TRUE) {
        KtiAgentSetSncBaseAddr (Socket, Ctr, Cluster, SncBaseAddr[Cluster]);
      }
    }
  }
}

/**
  Program SNC Upper BASE for all KTI agent/IIO stacks/MS2IDI in "Socket"

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Socket            - Socket
  @param MemInfo           - Memory Information structure.

  @return KTI_SUCCESS

**/
KTI_STATUS
ProgramSncUpperBaseFor52BitExtend (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    Socket,
  MEM_INFO                 *MemInfo
  )
{
  UINT32                  SncUpperBase;
  UINT8                   Ctr, Cluster;

  SncUpperBase = 0;
  for (Cluster = 0; Cluster <= MAX_CLUSTERS; Cluster++) {
    SncUpperBase |= (((MemInfo->ClusterMemBase[Socket][Cluster] & 0x3F0000) >> 16) << (Cluster * 6));
  }

  //
  // Program SNC upper base register in MS2IDI
  //
  if (!IsSiliconWorkaroundEnabled ("S2202532599")) {
    Ms2IdiSetSncUpperBase (Socket, SncUpperBase);
  }

  //
  // Program SNC upper base register in  KTI Agents & IIO stacks
  //
  for (Ctr = 0; Ctr < KtiInternalGlobal->KtiPortCnt; Ctr++) {
    if (KTIVAR->CpuInfo[Socket].LepInfo[Ctr].Valid == TRUE) {
      KtiAgentSetSncUpperBase (Socket, Ctr, SncUpperBase);
    }
  }

  for (Ctr = 0; Ctr < MAX_IIO_STACK; Ctr++) {
    if (StackPresent (Socket, Ctr)) {
      IioSetSncUpperBase (Socket, Ctr, SncUpperBase);
    }
  } // iio stack loop

  return KTI_SUCCESS;
}

/**
  Reset SNC_BASE to zero for "Socket" when SNC/Prefetch disabled

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Socket            - Socket
  @param MemInfo           - Memory Information structure.

**/
VOID
ResetAllSncBase (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Socket,
  MEM_INFO                   *MemInfo
  )
{
  ZeroMem ((VOID *) &MemInfo->ClusterMemBase[Socket], sizeof (MemInfo->ClusterMemBase[Socket]));

  ProgramSncBaseAddrInIio (Socket, MemInfo->ClusterMemBase[Socket]);
  ProgramSncBaseAddrInKti (KtiInternalGlobal, Socket, MemInfo->ClusterMemBase[Socket]);
  ProgramSncBaseAddrInMs2Idi (Socket, MemInfo->ClusterMemBase[Socket]);
  ProgramSncUpperBaseFor52BitExtend (KtiInternalGlobal, Socket, MemInfo);
}

/**

  When skip warm reset, need to update SNC configurations in KTI and IIO
  according values in MS2IDI

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Socket            - The socket ID

**/
VOID
UpdateSncColdResetRegisterOnKtiAndIio (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Socket
  )
{
  UINT8   Ctr;
  UINT8   NumOfChaPerCluster;
  UINT8   BaseChaOfCluster1;
  UINT8   BaseChaOfCluster2;
  UINT8   BaseChaOfCluster3;
  UINT8   FullSncEnable;
  UINT8   SncIndEnable;
  UINT8   NumClusters;
  UINT8   UmaClusterEnable;
  UINT8   DefeatureXor;
  UINT8   LinkIndex;
  UINT32  SncConfigMs2Idi = 0;
  UINT32  UncoreSncConfig = 0;

  UncoreSncConfig = Ms2IdiGetUncoreSncCfg (Socket, &NumOfChaPerCluster, &BaseChaOfCluster1, &BaseChaOfCluster2, &BaseChaOfCluster3);
  SncConfigMs2Idi = Ms2IdiGetFieldsSncCfg (Socket, &FullSncEnable, &SncIndEnable, &NumClusters);
  Ms2IdiGetFieldsUmaClusterCfg (Socket, &UmaClusterEnable, &DefeatureXor);
  // Defeature_Xor doesn't support in ICX. Should clear it.
  DefeatureXor = 0;
  //
  // Program SNC config registers in IIO stack & Kti agents
  //
  ProgramSncConfigInKtiAgent (KtiInternalGlobal, Socket, UncoreSncConfig, SncConfigMs2Idi);

  for (Ctr = 0; Ctr < MAX_IIO_STACK; Ctr++) {
    if (StackPresent (Socket, Ctr)) {
      IioSetSncCfg (Socket, Ctr, SncConfigMs2Idi);
      IioSetSncUncore (Socket, Ctr, NumOfChaPerCluster, BaseChaOfCluster1, BaseChaOfCluster2, BaseChaOfCluster3);
      IioSetUmaClusterCfg (Socket, Ctr, UmaClusterEnable, DefeatureXor);
    }
  }
  for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
    if (KTIVAR->CpuInfo[Socket].LepInfo[LinkIndex].Valid == TRUE) {
      ProgramKtiAgentUmaCluster (Socket, LinkIndex, UmaClusterEnable, DefeatureXor);
    }
  }
}

/**
  Program XPT when warm reset elemination is enabled, currently needed for SNR/ICXD.

  @param Socket            - The socket ID
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param MemInfo           - Memory Information structure.

  @retval                  - None

**/
VOID
UpdateSncRegistersForSkipWarmReset (
  UINT8                      Socket,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo
  )
{
  if (KTIVAR->OutSncPrefetchEn == 0) {
    return;
  }

  if (!KTIVAR->WarmResetEliminationEn) {
    return;
  }

  if (KtiInternalGlobal->CpuType == CPU_ICXD) {
    UpdateSncColdResetRegisterOnKtiAndIio (KtiInternalGlobal, Socket);
  } else if ((KtiInternalGlobal->CpuType == CPU_SNR || KtiInternalGlobal->CpuType == CPU_TNR) &&
             IsSiliconWorkaroundEnabled ("S1408863895")) {
    ProgramSncColdResetRegister (KtiInternalGlobal, MemInfo, Socket, NORMAL_OPERATION);
  }
}

/**
  API callable from outside UncoreRC to program Sub Numa Clustering (SNC) mode.

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param  MemInfo          - Memory Information structure.
  @param  Status           - SNC mode status

  @retval UNCORE_SUCCESS   - SNC mode is programmed successfully; or system is not configured for SNC mode operation.
  @retval UNCORE_FAILURE   - Memory profile is not suitable for SNC mode operation.

**/
KTI_STATUS
ConfigureSncMode (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo,
  KTI_STATUS                 Status
  )
{
  UINT8   Socket, Ctr, Cluster, PrefetchEn;
  BOOLEAN IsPostResetPowerGood, Mem2LMEnabled;
  BOOLEAN AllPrefetchToDisable;
  UINT8   MaxIMC;
  UINT8   SncNo;
  UINT8   ClusterChaId;
  UINT8   NumOfChaPerCluster;
  UINT8   BaseChaOfCluster1;
  UINT8   BaseChaOfCluster2;
  UINT8   BaseChaOfCluster3;
  UINT8   ChaId;
  UINT8   i;
  UINT32  SncConfiguration = 0;
  UINT32  H0ChRouteTable;
  BOOLEAN IsSncRegsWorkaroundEn;

  SncNo      = 0;
  MaxIMC     = GetMaxImc ();
  PrefetchEn = 0xFF;
  IsSncRegsWorkaroundEn = IsSiliconWorkaroundEnabled ("S1408863895");

  //
  // Calculate the SNC Base Size for each cluster. Note there is no holes allowed between the clusters except for the MMIOL hole.
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n"));

  //
  // Prefetch should be enabled or disabled for all sockets.
  //
  MemInfo->PrefetchAll = 1;
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!SocketPresent (Socket)) {
      continue;
    }

    if (PrefetchEn == 0xFF) {
      PrefetchEn = MemInfo->SncInfo[Socket].PrefetchEn;
    } else {
      if (PrefetchEn != MemInfo->SncInfo[Socket].PrefetchEn) {
        MemInfo->PrefetchAll = 0;
        break;
      }
    }
  }

  IsPostResetPowerGood = CheckCurrentReset (POST_RESET_POWERGOOD);
  AllPrefetchToDisable = FALSE;
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // check if socket is valid and snc enabled
    //
    if (SocketPresent (Socket)) {
      if (KTIVAR->OutSncPrefetchEn) {
        if (IsPostResetPowerGood && GetSysResetRequired () != 0) {
          //
          // powergood reset path with reset pending
          //
          ProgramSncColdResetRegister (KtiInternalGlobal, MemInfo, Socket, NORMAL_OPERATION);
          ProgramSncBaseAddrInIio (Socket, MemInfo->ClusterMemBase[Socket]);
        } else {
          //
          // warm reset path, or
          // powergood reset path with warm-reset elimination
          //
          UpdateSncRegistersForSkipWarmReset (Socket, KtiInternalGlobal, MemInfo);

          if (KTIVAR->OutSncPrefetchEn && (Status == KTI_SUCCESS)) {         // normal warm reset path, i.e. no snc failure detection during warm reset
            if (KTIVAR->UmaClusteringDowngrade) {
              ProgramSncColdResetRegister (KtiInternalGlobal, MemInfo, Socket, NORMAL_OPERATION);
            }
            //
            // Program SNC base register in KTI Agents  & IIO stack
            //
            ProgramSncBaseAddrInKti (KtiInternalGlobal, Socket, MemInfo->ClusterMemBase[Socket]);
            ProgramSncBaseAddrInIio (Socket, MemInfo->ClusterMemBase[Socket]);

            for (Cluster = 0; Cluster < MemInfo->SncInfo[Socket].NumOfCluster; Cluster++) {
              //
              // Update Logic to Physical channel mapping from channel route table
              //
              ClusterChaId = MemInfo->SncInfo[Socket].NumOfChaPerCluster * Cluster;
              ChaH0ChRouteTableRead (Socket, ClusterChaId, 1, &H0ChRouteTable);
              MemInfo->McInfo[Socket][Cluster].PrefLogic2PhyChl = H0ChRouteTable & 0x0000FFFF;
              KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, " MemInfo->McInfo[%d][%x].PrefLogic2PhyChl=%x\n",
                                  Socket, Cluster, MemInfo->McInfo[Socket][Cluster].PrefLogic2PhyChl));

              //
              // WA:From m2mem - logical channel id is decoded as 3 (Address 0x40300 [10:8] = 0b011, [16:14] = 0b000, [24:22] = 0b000 ?  3) and BIOS programs the m2mem register only for logical channel 0 and 1. So for logical channel3 , it is zero and hence prefetch requests are sent to channel id 0 .
              //
              if ((KTIVAR->OutSncPrefetchEn != 0) && (MemInfo->SncInfo[Socket].PrefetchEn == 1)) {
                KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, " Ways[%d][%d]=%x\n", Socket, Cluster, MemInfo->Ways[Socket][Cluster]));

                if ((MemInfo->Ways[Socket][Cluster] == 4) || (MemInfo->Ways[Socket][Cluster] == 3)) {
                  //
                  // Duplicate data from 8 to 15 (0x0050 --> 0x5050)
                  //
                  MemInfo->McInfo[Socket][Cluster].PrefLogic2PhyChl |= MemInfo->McInfo[Socket][Cluster].PrefLogic2PhyChl << 8;
                } else if (MemInfo->Ways[Socket][Cluster] == 2) {
                  //
                  // Duplicate data from 2 to 7 (0x0004 --> 0x4444)
                  //
                  MemInfo->McInfo[Socket][Cluster].PrefLogic2PhyChl |= MemInfo->McInfo[Socket][Cluster].PrefLogic2PhyChl << 4;
                  MemInfo->McInfo[Socket][Cluster].PrefLogic2PhyChl |= MemInfo->McInfo[Socket][Cluster].PrefLogic2PhyChl << 8;
                } else if (MemInfo->Ways[Socket][Cluster] == 1) {
                  for (i = 0; i < 8; i++) {
                    MemInfo->McInfo[Socket][Cluster].PrefLogic2PhyChl |= MemInfo->McInfo[Socket][Cluster].PrefLogic2PhyChl << (i * 2);
                  }
                }
              }

              KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, " MemInfo->McInfo[%d][%x].PrefLogic2PhyChl=%x\n",
                                  Socket, Cluster, MemInfo->McInfo[Socket][Cluster].PrefLogic2PhyChl));
            } // Cluster loop

            //
            // Need to program SNC setting in M2MEM here in warm reset flow because these fields are not sticky
            //
            if (MaxIMC > 0 && MaxIMC <= 4) {
              switch (MemInfo->SncInfo[Socket].NumOfCluster) {
                case 1:
                  for (Ctr = KTI_MC0; Ctr < MaxIMC; Ctr++) {
                    SetM2mTopologySnc (Socket, Ctr, SNC_CLUSTER_0, 0, MemInfo->SncInfo[Socket].NumOfChaPerCluster);
                  }
                  break;

                case 2:
                  for (Ctr = KTI_MC0; Ctr < MaxIMC/2; Ctr++) {
                    SetM2mTopologySnc (Socket, Ctr, SNC_CLUSTER_0, 0, (KTIVAR->CpuInfo[Socket].TotCha/2)-1);
                  }
                  for (Ctr = MaxIMC/2; Ctr < MaxIMC; Ctr++) {
                    SetM2mTopologySnc (Socket, Ctr, SNC_CLUSTER_1, KTIVAR->CpuInfo[Socket].TotCha/2, KTIVAR->CpuInfo[Socket].TotCha-1);
                  }
                  break;

                case 4:
                  SetM2mTopologySnc (Socket, KTI_MC0, SNC_CLUSTER_0, 0, MemInfo->SncInfo[Socket].NumOfChaPerCluster-1);
                  SetM2mTopologySnc (Socket, KTI_MC1, SNC_CLUSTER_1, MemInfo->SncInfo[Socket].NumOfChaPerCluster, MemInfo->SncInfo[Socket].NumOfChaPerCluster*2-1);
                  SetM2mTopologySnc (Socket, KTI_MC2, SNC_CLUSTER_2, MemInfo->SncInfo[Socket].NumOfChaPerCluster*2, MemInfo->SncInfo[Socket].NumOfChaPerCluster*3-1);
                  SetM2mTopologySnc (Socket, KTI_MC3, SNC_CLUSTER_3, MemInfo->SncInfo[Socket].NumOfChaPerCluster*3, KTIVAR->CpuInfo[Socket].TotCha-1);
                  break;

                default:
                  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nWARN: Cannot find proper SNC configuration!!!!\n\n"));
                  break;
              }
            } else {
              KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nWARN: Cannot find proper MaxIMC configuration!!!!\n\n"));
            }

            //
            // CHA SNC is not sticky, program it in Warm Reset
            //
            Ms2IdiGetUncoreSncCfg (Socket, &NumOfChaPerCluster, &BaseChaOfCluster1, &BaseChaOfCluster2, &BaseChaOfCluster3);
            KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "Read the UNCORE_SNC_CONFIG- BaseCha1=%x, BaseCha2=%x, BaseCha3=%x \n", BaseChaOfCluster1, BaseChaOfCluster2, BaseChaOfCluster3));

            for (ChaId = 0; ChaId < KTIVAR->CpuInfo[Socket].TotCha; ChaId++) {
              ChaProgramSncConfig (
                Socket,
                ChaId,
                KTIVAR->CpuInfo[Socket].TotCha,
                NORMAL_OPERATION,
                KTIVAR->OutUmaClustering > 0,
                MemInfo->SncInfo[Socket].NumOfCluster,
                BaseChaOfCluster1,
                BaseChaOfCluster2,
                BaseChaOfCluster3,
                &SncConfiguration
              );
            }

            //
            // Detect issues that may force prefetch disable
            //
            if (CheckXptKtiPrefetchNeedToDisable (KtiInternalGlobal, Socket, MemInfo)) {
              AllPrefetchToDisable = TRUE;
            } else if (CheckPrefetchSymmetric (KtiInternalGlobal, Socket, MemInfo) != KTI_SUCCESS) {
              AllPrefetchToDisable = TRUE;
            }

            //
            // If 2LM is enabled, or warm reset elimination enabled case
            // reprogram the CHA->M2M credit
            //
            ChaCheck2LMEnabled (Socket, 0, &Mem2LMEnabled);
            if (Mem2LMEnabled || KTIVAR->WarmResetEliminationEn) {
              if (MemInfo->SncInfo[Socket].NumOfCluster < 2) {
                ProgramChaToM2mCreditWithoutCluster (KtiInternalGlobal, MemInfo, Socket);
              } else {
                ProgramChaToM2mCreditWithCluster (KtiInternalGlobal, MemInfo, Socket);
              }
            }
          } else if (KTIVAR->OutSncPrefetchEn && (Status == KTI_SNC_CHANGED)) {     // warm reset path, but number of cluster changed & all sockets have the same number of cluters
            ProgramSncColdResetRegister (KtiInternalGlobal, MemInfo, Socket, NORMAL_OPERATION);
          }
        }  // warm reset

        if (KTIVAR->OutSncPrefetchEn && (Status != KTI_SNC_CHANGED)) {              // if cold reset, warm reset with KTI_SUCCESS & KTI_FAILURE, need to program snc_base
          //
          // Program SNC base register in MS2IDI (need to program during cold reset/warm reset flow
          //
          ProgramSncBaseAddrInMs2Idi (Socket, MemInfo->ClusterMemBase[Socket]);
          ProgramSncUpperBaseFor52BitExtend (KtiInternalGlobal, Socket, MemInfo);
        }
      } else {
        // recovery to snc disabled  i.e.  snc failure detection during warm reset
        ProgramSncColdResetRegister (KtiInternalGlobal, MemInfo, Socket, RECOVERY_OPERATION);
      }
    } // socket valid and snc enabled
  } // socket loop

  //
  // if warm reset flow detects prefetch must be disabled, disable on all sockets
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent (Socket)) {
      if ((!IsPostResetPowerGood) || (GetSysResetRequired () == 0)) {
        if (AllPrefetchToDisable) {
          //
          // SNR/TNR requires BIOS to always set snc_config.snc_ind_en = 1 and program snc_base register even when xpt prefetch is disabled.
          // See Silicon workaround "S1408863895". Therefore skip the following recovery action for SNR/TNR.
          //
          if (!IsSncRegsWorkaroundEn &&
              ((KTIVAR->OutKtiPrefetch == KTI_ENABLE) || (KTIVAR->OutXptPrefetch == KTI_ENABLE) || (KTIVAR->OutXptRemotePrefetch == KTI_ENABLE))) {
            //
            // OutKtiPrefetch, OutXptPrefetch, and OutXptRemotePrefetch variables indicate prefetch was expected to be enabled at end of KTI (Pre-MRC) flow.
            // Need warm reset to fix MRC/Memory Map programming that depends on prefetch enable status
            // e.g. if GB Alignment is no longer required in prefetch disable case, or MRC changes the delay between command completion and data completion based on prefetch enable
            //
            DisabledPrefetchFlag (KtiInternalGlobal);
            SetNewResetRequired (POST_RESET_WARM);
            if (KTIVAR->OutSncPrefetchEn == 0) {
              //
              // need to clear snc_ind_en and SNC_BASE_* on all sockets
              //
              for (Ctr = 0; Ctr < MAX_SOCKET; Ctr++) {
                if (SocketPresent (Ctr)) {
                  ProgramSncColdResetRegister (KtiInternalGlobal, MemInfo, Ctr, RECOVERY_OPERATION);
                }
              }
            }
          }
          DisableAllPrefetch (KtiInternalGlobal, Socket);
        } else {
          ProgramPrefetchEntry (KtiInternalGlobal, Socket, MemInfo);
        }
      }
    }
  }
  return KTI_SUCCESS;
}

/**
  Disable all prefetch for "Socket"

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Socket            - Socket

  @retval none

**/
VOID
DisableAllPrefetch (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Socket
  )
{
  // disable XPT local prefetch
  DisableXptPrefetch (KtiInternalGlobal, Socket);
  // disable remote xpt prefetch
  DisableRemoteXpt (KtiInternalGlobal, Socket);
  // disable kti prefetch
  EnablePrefetchInKtiAgent (KtiInternalGlobal, Socket, KTI_XPT_PREFETCH_DISABLE);
}
