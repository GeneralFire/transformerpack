/** @file
  This file includes functions to determine cluster base/limit for each cluster.

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
#include <Guid/MemoryMapData.h>
#include <Library/CxlLib.h>
#include <Guid/SocketCommonRcVariable.h>
#include <Library/SncPrefetchLib.h>
#include "SncPrefetchLibInternal.h"


/**
  API callable from outside UncoreRC to calculate  Clustering (SNC) base

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param MemInfo           - Memory Information structure.
  @param Socket            - Socket ID

  @retval KTI_SUCCESS      - SNC cluster number match with requirement
  @retval KTI_FAILURE      - SNC cluster number doesn't match with requirement

**/
KTI_STATUS
SanityCheckSncClusterNumber (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo,
  UINT8                      Socket
  )
{
  if ((KTIVAR->OutSncEn == 4) || (KTIVAR->OutSncEn == 2)) {
    if (MemInfo->SncInfo[Socket].NumOfCluster != KTIVAR->OutSncEn) {
      return KTI_FAILURE;
    }
  }

  return KTI_SUCCESS;
}

/**
  Verify any memory topology change after warm reset

  This routine is to verify any memory topology change after warm reset.

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param MemInfo           -  Memory Information structure
  @param Socket            -  Socket

  @retval KTI_STATUS       -  KTI_SUCCESS if no changes

**/
KTI_STATUS
VerifyConfigChangedAfterWarmReset (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo,
  UINT8                      Socket
  )
{
  BOOLEAN IOwn4GInColdReset;
  UINT8   Soc, TempNumOfCluster;
  BOOLEAN IsSncRegsInDefault;
  BOOLEAN NeedCheck4GOwnerChanged;

  //
  // Step 1: check the socket owns 0-4G in cold reset flow still valid in warm reset
  //        if any of SNC_BASE registers are different, then the socket owned 0-4G in cold reset path
  //
  IOwn4GInColdReset = FALSE;
  NeedCheck4GOwnerChanged = TRUE;

  IsSncRegsInDefault = IsSncRegistersInHardwareDefaultSetting (Socket);
  if (Ms2IdiGetSncBaseAddr (Socket, 0) == 0) {
    IOwn4GInColdReset = TRUE;
  }
  if (KTIVAR->WarmResetEliminationEn && IsSncRegsInDefault) {
    NeedCheck4GOwnerChanged = FALSE;
  }
  // Socket owns 4G in cold reset but Socket doesn't own 4G in warm reset, then exit with failure
  if (NeedCheck4GOwnerChanged && IOwn4GInColdReset && (MemInfo->SocketIdWithFirst4G & 0x7F) != Socket) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket owns 4G is changed in warm reset"));
    return KTI_FAILURE;
  }

  //
  // Step 2: check if # of cluster still remain the same as cold reset flow
  //         OutNumOfCluster is updated by reading SNC_CONFIG_MS2IDI_MAIN_REG
  //
  if (KTIVAR->OutSncEn == KTI_DISABLE) {
    return KTI_SUCCESS;
  }

  if (SanityCheckSncClusterNumber (KtiInternalGlobal, MemInfo, Socket) == KTI_FAILURE) {
    return KTI_FAILURE;
  }

  if (MemInfo->SncInfo[Socket].NumOfCluster != KTIVAR->OutNumOfCluster[Socket]) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n number of cluster is changed on Socket %d", Socket));

    TempNumOfCluster = 0xFF;
    for (Soc = 0; Soc < MAX_SOCKET; Soc++) {
      if (SocketPresent (Soc)) {
        if ( TempNumOfCluster == 0xFF) {            // 1st time to update this variable
          TempNumOfCluster = MemInfo->SncInfo[Soc].NumOfCluster;
        } else if ( TempNumOfCluster != MemInfo->SncInfo[Soc].NumOfCluster) {  //if any sockets have different number of Mc then exit to disable SNC
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nDifferent Cluster numbers on existing sockets. Disable SNC!!"));
          return KTI_FAILURE;
        }
      }
    }
    return KTI_SNC_CHANGED;      //number of clusers changes on all existing sockets, all sockets have the same number of clustsrs
  }

  return KTI_SUCCESS;
}

/**
  API callable from outside UncoreRC to calculate  Clustering (SNC) base

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param  MemInfo          - Memory Information structure.

  @retval UNCORE_SUCCESS   - SNC mode is programmed successfully; or system is not configured for SNC mode operation.
  @retval UNCORE_FAILURE   - Memory profile is not suitable for SNC mode operation.

**/
KTI_STATUS
CalculateClusterBase (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo
  )
{
  UINT8           Socket, Mc, Cluster;
  KTI_STATUS      Status;
  UINT8           Idx;
  UINT8           PeerSocId;
  UINT8           SbspSoc;
  UINT8           LowRangeSocId;
  UINT8           HighRangeSocId;

  Status = KTI_SUCCESS;
  //
  // These conditions must be met to enable the SNC mode.
  //     - Each cluster gets multiple of 1GB DDR4 memory, if DDR4 memory is allocated
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // check if socket is valid and snc enabled
    //
    if (SocketPresent (Socket)) {

      if ((KTIVAR->OutSncPrefetchEn == 0) || (MemInfo->SncInfo[Socket].NumOfMcEnabled == 0)) {
        continue;
      }

      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d", Socket));

      if (CheckCurrentReset (POST_RESET_POWERGOOD) && (GetSysResetRequired () != 0)) {
        //
        // Powergood path with reset pending
        //
        for (Cluster = 0; Cluster <= MAX_CLUSTERS; Cluster++) {
          //
          //  If powergood, find which socket & which mc owns 0-4G range
          //
          if ((MemInfo->SocketIdWithFirst4G & 0x7F) == Socket && Cluster == 0) {
            MemInfo->ClusterMemBase[Socket][Cluster] = 0;                    //Cluster 0 starts from 0G
          } else {
            MemInfo->ClusterMemBase[Socket][Cluster] = FOUR_GB_MEM >> 4;     //in GB
          }
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  SNC_Base_%d = %04d GB", Cluster + 1, MemInfo->ClusterMemBase[Socket][Cluster]));
        }
      } else {
        //
        // reset type is warm reset, or powergood reset with warm-reset elimination flow
        //
        Status = VerifyConfigChangedAfterWarmReset (KtiInternalGlobal, MemInfo, Socket);

        //
        // if KTI_FAILURE
        //    a.  socket owns 0-4G have been changed
        //    b.  sockets have been changed with different number of clusters,
        //    c.  sockets have different channel mapping and size if both MC enabled & NUMA & 2 WAY interleave cross both MC,
        //    then, SNC need to be disable
        // else if KTI_SNC_CHANGED, number of clusters have been changed, but all sockets have the same number of clusters
        //
        if (Status != KTI_SUCCESS) {
          return Status;
        }

        for (Cluster = 0; Cluster < MAX_CLUSTERS; Cluster++) {
          if (MemInfo->ClusterRngInLocal[Socket][Cluster].Enabled == 1) {
            if (((MemInfo->ClusterRngInLocal[Socket][Cluster].MemLimit & 0x0F) != 0)) { //check if not in 1G multiple beacause SNC base register is in 1G multiples
              KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Cluster %d Limit = %07d NOT in GB alignment", Socket, Cluster, MemInfo->McInfo[Socket][Cluster].MemLimit));
              return KTI_FAILURE;
            }
          }
        }
        //
        // update cluster memory base
        //
        Cluster = 0;

        //
        // SNC4/SNC2 with UpiAffinity enabled
        //
        if (((KTIVAR->OutSncEn == 2) || (KTIVAR->OutSncEn == 4)) &&
            KTIVAR->UpiAffinityEn) {
          SbspSoc = KtiInternalGlobal->SbspSoc;
          PeerSocId = (UINT8)((Socket == SbspSoc) ? KTIVAR->CpuInfo[Socket].LepInfo[0].PeerSocId : Socket);
          if (MemInfo->ClusterRngInLocal[SbspSoc][0].MemBase < MemInfo->ClusterRngInLocal[PeerSocId][0].MemBase) {
            LowRangeSocId  = SbspSoc;
            HighRangeSocId = PeerSocId;
          } else {
            LowRangeSocId  = PeerSocId;
            HighRangeSocId = SbspSoc;
          }
          //
          // Suppose S0: Legacy socket S1: peer socket
          // SNC settings  | SNC4                                    | SNC2
          // -----------------------------------------------------------------------------
          // normal mode:  |S0C0+S1C0/S0C1+S1C1/S0C2+S1C2/S0C3+S1C3  | S0C0+S1C0/S0C1+S1C1
          // -----------------------------------------------------------------------------
          // twisted mode: |S0C0+S1C2/S0C1+S1C1/S0C2+S1C20/S0C3+S1C3 | S0C0+S1C1/S0C1+S1C0
          // -----------------------------------------------------------------------------
          //
          if (KTIVAR->OutSncEn == 4) {
            for (Idx = 0; Idx < 4; Idx++) {
              MemInfo->ClusterMemBase[Socket][Idx] = MemInfo->ClusterRngInLocal[LowRangeSocId][Idx].MemBase >> 4;
            }
            MemInfo->ClusterMemBase[Socket][4] = MemInfo->ClusterRngInLocal[HighRangeSocId][3].MemLimit >> 4;
          } else {
            MemInfo->ClusterMemBase[Socket][0] =  MemInfo->ClusterRngInLocal[LowRangeSocId][0].MemBase >> 4;
            MemInfo->ClusterMemBase[Socket][1] =  MemInfo->ClusterRngInLocal[LowRangeSocId][1].MemBase >> 4;
            if (KTIVAR->TwistedKtiLinks) {
              MemInfo->ClusterMemBase[Socket][2] =  MemInfo->ClusterRngInLocal[HighRangeSocId][0].MemLimit >> 4;
            } else {
              MemInfo->ClusterMemBase[Socket][2] =  MemInfo->ClusterRngInLocal[HighRangeSocId][1].MemLimit >> 4;
            }
          }
        } else {
          for (Mc = 0; Mc < MAX_CLUSTERS; Mc++) {
            if (MemInfo->ClusterRngInLocal[Socket][Mc].Enabled == 1) {
              if (Cluster == 0) {
                MemInfo->ClusterMemBase[Socket][Cluster] = MemInfo->ClusterRngInLocal[Socket][Mc].MemBase >> 4;
                Cluster++;
              }
              MemInfo->ClusterMemBase[Socket][Cluster] = MemInfo->ClusterRngInLocal[Socket][Mc].MemLimit >> 4;
              Cluster++;
            }
          }
        }

        for (Cluster = 0; Cluster <= MAX_CLUSTERS; Cluster++) {
          //
          // update non-used cluster base to the last cluster base, even this cluster doesn't exist
          //
          if (Cluster != 0 && MemInfo->ClusterMemBase[Socket][Cluster] == 0) {
            MemInfo->ClusterMemBase[Socket][Cluster] = MemInfo->ClusterMemBase[Socket][Cluster - 1];
          }
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  SNC_Base_%d = %04d GB", Cluster + 1, MemInfo->ClusterMemBase[Socket][Cluster]));
        }
      } // reset type
    } // socket valid & snc enable
  } // scan socket

  return KTI_SUCCESS;
}