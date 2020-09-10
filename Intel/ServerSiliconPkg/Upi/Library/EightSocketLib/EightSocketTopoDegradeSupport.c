/** @file
  This file includes functions to support 8S topology degrade.

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

#include <KtiMisc.h>
#include <Library/KtiApi.h>
#include <Library/UncoreLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/EightSocketLib.h>
#include <Library/MultiSocketLib.h>

/**
  Degrade the system to 6S topology.

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.
  @param CommonSoc         - Socket that is part of any 2 rings

  @retval KTI_SUCCESS

**/
KTI_STATUS
DegradeTo6S (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData,
  UINT8                    CommonSoc
  )
{
  UINT8                   Idx1, Idx2, RingNumber;
  KTI_DISC_DATA           TmpDiscEngData1;

  //
  // Prepare a mask based on the sockets that part of the 2 rings. By removing the sockets that are not
  // part of the 2 rings and the links connecting to those sockets, the topology can be reduced to 6S.
  //
  DiscEngData->CpuList = 0;
  for (Idx1 = 1; Idx1 < 3; Idx1++) {
    RingNumber = DiscEngData->RingInfo[CommonSoc][Idx1];
    for (Idx2 = 0; Idx2 < CPUS_PER_RING; Idx2++) {
      DiscEngData->CpuList = DiscEngData->CpuList | (1 << DiscEngData->Rings[RingNumber][Idx2]);
    }
  }

  // Make sure there are 6 sockets present in the 2 rings of the CommonSoc
  DiscEngData->TotCpus = 0;
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (DiscEngData->CpuList & (1 << Idx1)) {
      DiscEngData->TotCpus++;
    }
  }
  KTI_ASSERT ((DiscEngData->TotCpus == 6), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);

  // Mark the remaining 2 sockets and the link connecting to them as invalid
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if ((DiscEngData->CpuList & (1 << Idx1)) == 0) {
      ZeroMem ((VOID *)(&DiscEngData->Cpu[Idx1]), sizeof (DiscEngData->Cpu[Idx1]));
    }
  }
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt; Idx2++) {
        if ((DiscEngData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) &&
            ((DiscEngData->CpuList & (1 << DiscEngData->Cpu[Idx1].LinkData[Idx2].PeerSocId)) == 0)) {
          ZeroMem ((VOID *)(&DiscEngData->Cpu[Idx1].LinkData[Idx2]), sizeof (DiscEngData->Cpu[Idx1].LinkData[Idx2]));
        }
      }
    }
  }

  // Make sure the topology is reduced correctly (2 rings present and the legacy socket is part of at least one of the rings)
  ZeroMem ((VOID *)(&TmpDiscEngData1), sizeof (TmpDiscEngData1));
  CopyMem ((UINT8 *) TmpDiscEngData1.Cpu, (UINT8 *) DiscEngData->Cpu, sizeof (TmpDiscEngData1.Cpu));
  TmpDiscEngData1.TotCpus = DiscEngData->TotCpus;
  TmpDiscEngData1.CpuList = DiscEngData->CpuList;
  LocateRings (KtiInternalGlobal, &TmpDiscEngData1);
  KTI_ASSERT ((TmpDiscEngData1.TotRings == 2), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  KTI_ASSERT ((TmpDiscEngData1.RingInfo[CommonSoc][0] == 2), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);

  //
  // Update SocketData structure with the reduced topology info.
  // System will still be considered as 8S for all configuration purposes
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  Reducing it to 6S Topology\n"));
  KTIVAR->SysConfig = SYS_CONFIG_8S;
  CopyMem ((UINT8 *) SocketData->Cpu, (UINT8 *) TmpDiscEngData1.Cpu, sizeof (SocketData->Cpu));

  return KTI_SUCCESS;
}

/**
  IsCpuAttachedToBranch ()
  Check to see if the target CPU is attached to a branch of the current topology.
  In other words, is the target CPU is attached to one of the CPUs identified in the CPU list.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param SocketData        - Socket specific data structure
  @param CpuId             - Socket specific data structure
  @param CpuList           - List of CPUs to check
  @param CpuListCnt        - # of CPUs in the list
  @param *LinkId           - ID (num) of the link when the target CPU is attached.

  @retval TRUE, FALSE

**/
BOOLEAN
IsCpuAttachedToBranch (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_CPU_SOCKET_DATA       CpuSocketData[],
  UINT8                     TargetCpuId,
  UINT8                     CpuList[],
  UINT8                     CpuListCnt,
  UINT8                    *AttachedLinkIdPtr
  )
{
  KTI_LINK_DATA*          LinkPtr;              //Roving ptr to target CPU's links
  UINT8                   LinkId;

  KTI_CPU_SOCKET_DATA*    PeerCpuPtr;           //Ptr to peer CPU
  KTI_LINK_DATA*          PeerLinkPtr;          //Ptr to peer CPU's link

  UINT8                   CpuIdx;               //Roving index into CPU list
  UINT8                   CpuId;


  //
  // Examine each CPU link to see if it is attached to one of the CPUs in the list.
  //
  LinkPtr = &CpuSocketData[TargetCpuId].LinkData[0];
  for (LinkId=0; LinkId < KtiInternalGlobal->KtiPortCnt; LinkId++, LinkPtr++) {
    if (LinkPtr->Valid) {
      // Target CPU's link is valid so it's attached to something.
      // Scan the CPU list to see if the peer CPU is in the list.
      for (CpuIdx=0; CpuIdx < CpuListCnt; CpuIdx++) {
        CpuId = CpuList[CpuIdx];
        if (CpuId == LinkPtr->PeerSocId) {
          // Target CPU is attached to one of the CPUs in the list.
          // Check the peer CPU's info (status,link) to ensure everything is consistent.
          // If the peer CPU or the peer link is not valid, then the Target CPU is not (properly) attached.
          PeerCpuPtr = &CpuSocketData[LinkPtr->PeerSocId];
          if (PeerCpuPtr->Valid) {
            PeerLinkPtr = &PeerCpuPtr->LinkData[LinkPtr->PeerPort];
            if (PeerLinkPtr->Valid) {
              KTI_ASSERT (PeerLinkPtr->PeerSocId == TargetCpuId, ERR_TOPOLOGY_DISCOVERY, MINOR_ERR_INTERNAL_DATA_STRUCTURE);
              KTI_ASSERT (PeerLinkPtr->PeerPort  == LinkId,      ERR_TOPOLOGY_DISCOVERY, MINOR_ERR_INTERNAL_DATA_STRUCTURE);
              if (AttachedLinkIdPtr != NULL) {
                *AttachedLinkIdPtr = LinkId;   //If wanted, tell caller which target CPU's link is being used
              }
              return TRUE;
            }//Peer link valid
          }//Peer CPU valid
        }//Found attached CPU
      }//For each CPU in list
    }//CPU's link valid
  }//For each CPU link

  // Target CPU not attached to any of the CPUs in the CPU list.
  return FALSE;
}


/**
  TrimTopology ()
  Trim the current topology by removing the specified CPU.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - DiscEngDatat structure
  @param CpuId             - Socket specific data structure

  @retval KTI_SUCCESS

**/
KTI_STATUS
TrimTopology (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  KTI_DISC_DATA             *DiscEngData,
  UINT8                     CpuId
  )
{
  KTI_CPU_SOCKET_DATA*    CpuPtr;               //Ptr to info of CPU to be trimmed
  KTI_LINK_DATA*          LinkPtr;              //Roving ptr to target CPU's links
  UINT8                   LinkId;

  KTI_CPU_SOCKET_DATA*    PeerCpuPtr;           //Roving rtr to peer CPU
  KTI_LINK_DATA*          PeerLinkPtr;          //Roving ptr to peer CPU's link

  //
  // Trim the CPU from the topology tree by setting Valid=FALSE for
  // the CPU, all it's links, and all it's peer links (but not the peer CPUs).
  //
  CpuPtr = &DiscEngData->Cpu[CpuId];
  if (CpuPtr->Valid) {
    // Process all Links
    LinkPtr = &CpuPtr->LinkData[0];
    for (LinkId=0; LinkId < KtiInternalGlobal->KtiPortCnt; LinkId++, LinkPtr++) {
      if (LinkPtr->Valid) {
        PeerCpuPtr = &DiscEngData->Cpu[LinkPtr->PeerSocId];
        //JGR - Question:  Better to replace the following 'if valid' with QPI_ASSERT (PeerCpuPtr->Valid);
        if (PeerCpuPtr->Valid) {
          PeerLinkPtr = &PeerCpuPtr->LinkData[LinkPtr->PeerPort];
          //JGR - Question:  Better to replace the following 'if valid' with QPI_ASSERT (PeerLinkPtr->Valid);
          if (PeerLinkPtr->Valid) {
            KTI_ASSERT (PeerLinkPtr->PeerSocId == CpuId,  ERR_TOPOLOGY_DISCOVERY, MINOR_ERR_INTERNAL_DATA_STRUCTURE);
            KTI_ASSERT (PeerLinkPtr->PeerPort  == LinkId, ERR_TOPOLOGY_DISCOVERY, MINOR_ERR_INTERNAL_DATA_STRUCTURE);
            PeerLinkPtr->Valid = FALSE;
          }//if peer link valid
          // Don't set Peer CPU to FALSE - PeerCpuPtr->Valid = FALSE;
        }//if peer CPU valid
        //Note: Peer CPU remains valid since some of its other links may be attached somewhereelse
      }//if CPU link valid
      LinkPtr->Valid = FALSE;
    }//For each CPU link
    CpuPtr->Valid = FALSE;
  }//if CPU valid

  return KTI_SUCCESS;
}

/**
  CheckForDegraded6S ()

  Try to degrade to a 6SG1 topology.

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - DiscEngDatat structure

  @retval KTI_SUCCESS

**/
KTI_STATUS
CheckForDegraded6S (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_DISC_DATA              *DiscEngData
  )
{
  UINT8                   RingId;               //Roving CPU Ring Id
  UINT8                  *RingCpuIdList;        //Roving ptr to the list of CPU Ids belonging to a CPU ring

  UINT8                   CpuId;                //Roving CPU Id
  KTI_CPU_SOCKET_DATA    *CpuPtr;               //Roving ptr to CPU's info

  UINT8                   NonRingCpuIdList[MAX_SOCKET]; //List of CPU Ids not part of the (one-and-only) CPU ring
  UINT8                   NonRingCpuCnt;        //Num of valid non-ring CPUsd

  UINT8                   NonRingCpuId1;        //CPU Id of a non-ring CPU
  UINT8                   NonRingToRingLinkId1; //Non-ring CPU's link Id used to attach to the CPU ring
  UINT8                   RingCpuId1;           //CPU Id of ring CPU to which the non-ring CPU is attached

  UINT8                   NonRingCpuId2;        //Same as above
  UINT8                   NonRingToRingLinkId2;
  UINT8                   RingCpuId2;

  BOOLEAN                 Found6SG1Topo;        //Indicates if a valid 6SG1 topology has been found
  BOOLEAN                 LegacyCpuInRing;      //Indicates if legacy CPU is part of the 'current' CPU ring

  UINT8                   Idx1, Idx2;           //Generic array index

  // Init key local vars
  Found6SG1Topo = FALSE;
  NonRingCpuCnt = 0;

  // Keep compiler from complaining about "possible uninitialized local var"
  NonRingCpuId1 = 0;
  NonRingCpuId2 = 0;
  NonRingCpuIdList[0] = 0;

  // Verify prerequisits
  if (DiscEngData->TotCpus < 6) {
    return KTI_FAILURE;
  }

  // Since there may be more than one CPU ring (e.g. a broken 8S topology),
  // check each ring to see if it forms the core ring of an 6SG1 topology.
  // Note: Any ring containing CPU-0 should have preference.  However, we
  //       assume any ring(s) containing CPU-0 will be listed first.
  for (RingId=0; RingId < DiscEngData->TotRings; RingId++) {
    RingCpuIdList = &DiscEngData->Rings[RingId][0];
    LegacyCpuInRing = FALSE;

    // Scan all CPUs and identify the CPUs that are not part of the current CPU ring.
    NonRingCpuCnt = 0;
    CpuPtr = &DiscEngData->Cpu[0];
    for (CpuId=0; CpuId < MAX_SOCKET; CpuId++, CpuPtr++) {
      if (CpuPtr->Valid) {
        // Check if CPU is part of current CPU ring
        for (Idx1=0; Idx1 < CPUS_PER_RING; Idx1++) {
          if (RingCpuIdList[Idx1] == CpuId) {  //CPU in ring?
            if (CpuId == KtiInternalGlobal->SbspSoc) {
              LegacyCpuInRing = TRUE;
            }
            break;  //CPU part of current CPU ring
          }
        }//For each CPU in ring
        if (Idx1 == CPUS_PER_RING) {
          NonRingCpuIdList[NonRingCpuCnt++] = CpuId;  //Add non-ring CPU to list
        }
      }//If CPU valid
    }//For each CPU

    // Examine each pair of non-ring CPUs looking for:
    // 1) A pair of non-ring CPUs attached to each other.
    // 2) Each CPU (in the pair) attached to the CPU ring.
    // 3) The two ring CPU attachment points must be opposite corners of the ring.
    // 4) A valid 6SG1 topology that includes the legacy CPU either within the CPU ring or as one of the non-ring CPUs.
    // If found, then we have a valid 6SG1 topology.
    for (Idx1=0; Idx1 < NonRingCpuCnt; Idx1++) {
      NonRingCpuId1 = NonRingCpuIdList[Idx1];
      for (Idx2=Idx1+1; Idx2 < NonRingCpuCnt; Idx2++) {
        NonRingCpuId2 = NonRingCpuIdList[Idx2];
        // Verify legacy CPU is included in the current set of CPUs under test: Either in the CPU ring or the two non-ring CPUs.
        if ((LegacyCpuInRing == FALSE) &&
            (NonRingCpuId1 != KtiInternalGlobal->SbspSoc) &&
            (NonRingCpuId2 != KtiInternalGlobal->SbspSoc)) {
          continue;  //Legacy CPU not included so don't bother checking.
        }
        // Check if both non-ring CPUs are directly attached to each other
        // Note: Both non-ring CPUs are known to be valid from code above.
        if (IsCpuAttachedToBranch (KtiInternalGlobal, DiscEngData->Cpu, NonRingCpuId1, &NonRingCpuId2, 1, NULL)) {  //Non-ring CPUs neighbors?
          // Check if both non-ring CPUs are directly attached to a CPU on the ring.
          // Note: All ring CPUs has atmost 1 non-ring link so both non-ring CPUs CAN NOT be attached to the same ring CPU.
          if ((IsCpuAttachedToBranch (KtiInternalGlobal, DiscEngData->Cpu, NonRingCpuId1, &RingCpuIdList[0], CPUS_PER_RING, &NonRingToRingLinkId1)) &&
              (IsCpuAttachedToBranch (KtiInternalGlobal, DiscEngData->Cpu, NonRingCpuId2, &RingCpuIdList[0], CPUS_PER_RING, &NonRingToRingLinkId2))) {
            // Verify the two (attached) ring CPUs are on opposite corners of the ring.
            // Otherwise, it's not a 6SG1 topology since some routes have more than 2 hops.
            RingCpuId1 = (UINT8) DiscEngData->Cpu[NonRingCpuId1].LinkData[NonRingToRingLinkId1].PeerSocId;
            RingCpuId2 = (UINT8) DiscEngData->Cpu[NonRingCpuId2].LinkData[NonRingToRingLinkId2].PeerSocId;
            if (IsCpuAttachedToBranch (KtiInternalGlobal, DiscEngData->Cpu, RingCpuId1, &RingCpuId2, 1, NULL) == FALSE) { //Ring CPUs NOT neighbors?
              Found6SG1Topo = TRUE;  //Ring CPUs not directly attached i.e. on opposite corners
              goto EndOfOuterLoop;
            };
          }//If non-ring CPUs pair attached to ring
        }//If non-ring CPU pair attached to each other
      }//For each "remaining" non-ring CPU
    }//For each non-ring CPU
  }//For each CPU ring
EndOfOuterLoop:

  // Check if a valid 6SG1 topology was found.  If not, error out.
  if (Found6SG1Topo == TRUE) {
    // A valid 6SG1 topology was found.  Now, trim off any excess CPUs not part of the 6SG1 topology.
    // Note: Only non-ring CPUs need to be checked.
    for (Idx1=0; Idx1 < NonRingCpuCnt; Idx1++) {
      CpuId = NonRingCpuIdList[Idx1];
      if ((CpuId != NonRingCpuId1) && (CpuId != NonRingCpuId2)) {
        TrimTopology (KtiInternalGlobal, DiscEngData, CpuId);
      }
    }//For each non-ring CPU

    // Update system vars to reflect the final toloplgy.
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Reducing to 6SG1 Topology\n"));
    KTIVAR->SysConfig = SYS_CONFIG_8S;
    CopyMem ((UINT8*)SocketData->Cpu, (UINT8*)DiscEngData->Cpu, sizeof (SocketData->Cpu));
    return KTI_SUCCESS;

  } else {
    return KTI_FAILURE;
  }//If-else 6SG1 found

}

/**
  Try to discover and degrade to a best 6S topology that include the legacy socket.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - Pointer to socket specific data

  @retval KTI_SUCCESS      - Already degraded to 6S topology.
  @retval KTI_FAILURE      - Can not degrade to 6S topology.

**/
KTI_STATUS
CanItBeDegradedTo6S (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  KTI_DISC_DATA             *DiscEngData
  )
{
  KTI_STATUS    Status;
  UINT8         Idx1, RingNumber, RingNode;

  if (DiscEngData == NULL) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nFatal Error - Invalid parameter: DiscEngData == NULL\n"));
    return KTI_FAILURE;
  }

  Status = KTI_SUCCESS;
  RingNode = 0;

  // Prepare the discovery engine data structure
  PrepareDiscEngData (SocketData, KtiInternalGlobal, DiscEngData);

  // Check and identify the rings present in the topology
  LocateRings (KtiInternalGlobal, DiscEngData);

  //
  // Any degraded 8S topology can have maximum of 3 rings and the legacy socket
  // can be part of upto only 2 rings.
  //
  KTI_ASSERT ((DiscEngData->TotRings <= 3), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  KTI_ASSERT ((DiscEngData->RingInfo[KtiInternalGlobal->SbspSoc][0] <= 2), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);

  // Try to salvage a 6S topology by retaining 2 rings. Preference is given to the rings that
  // the legacy socekt is part of. If the legacy socket is not part of any ring, then we can not salvage
  // a full 6S topology.  However, we may be able to salvage a degraded 6S (6SG1) topology.  If not, use 4S degrading rules.
  if (DiscEngData->RingInfo[KtiInternalGlobal->SbspSoc][0] == 2) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "Discovered full 6S topology with Legacy socket in both rings.\n"));
    // Legacy CPU on both CPU rings.  This is the prefered config.
    DegradeTo6S (SocketData, KtiInternalGlobal, DiscEngData, KtiInternalGlobal->SbspSoc);
    Status = KTI_SUCCESS;

  } else if (DiscEngData->RingInfo[KtiInternalGlobal->SbspSoc][0] == 1) {
    // Legacy CPU only on 1 CPU ring.  6S topology still possible but only if another CPU ring exists and is
    // adjacent to the Legacy CPU's ring i.e. one of the other CPUs on the "Legacy CPU ring" must belong to two CPU ring.
    //
    // Check if any sockets of the legacy socket's ring is part of two rings
    RingNumber = DiscEngData->RingInfo[KtiInternalGlobal->SbspSoc][1];
    for (Idx1 = 0; Idx1 < CPUS_PER_RING; Idx1++) {
      RingNode = DiscEngData->Rings[RingNumber][Idx1];
      if (DiscEngData->RingInfo[RingNode][0] == 2) {
        break;
      }
    }//For each CPU on legacy CPU ring

    if (Idx1 < CPUS_PER_RING) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "Discovered full 6S topology with Legacy socket in one of the two rings.\n"));
      DegradeTo6S (SocketData, KtiInternalGlobal, DiscEngData, RingNode);
      Status = KTI_SUCCESS;
    } else {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "Legacy socket in one ring. Check if a 6SG1 topology is supported.\n"));
      // Legacy CPU belongs to a CPU ring, but no adjacent rings so no full 6S topology.
      // However, if there are at least 6 CPUs, a degraded 6S (6SG1) topology may still be possible.
      // But, if fewer than 6 CPUs or not a 6SG1 topology, just degrade to a 4S toplogy.
      if ((DiscEngData->TotCpus < 6) ||
           (CheckForDegraded6S (SocketData, KtiInternalGlobal, DiscEngData) != KTI_SUCCESS)) {
        Status = KTI_FAILURE;
      }
    }//If-else adjacent ring exist

  } else if (DiscEngData->TotRings >= 1) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "Ring exists but legacy socket not in the ring. Check if a 6SG1 topology is supported.\n"));
    // Atleast one CPU ring exists, but legacy CPU is not part of it.
    // However, if there are at least 6 CPUs, a degraded 6S (6SG1) topology may still be possible.
    // But, if fewer than 6 CPUs or not a 6SG1 topology, just degrade to a 4S toplogy.
    if ((DiscEngData->TotCpus < 6) ||
         (CheckForDegraded6S (SocketData, KtiInternalGlobal, DiscEngData) != KTI_SUCCESS)) {
      Status = KTI_FAILURE;
    }

  } else {
    //
    // No CPU rings exist i.e. no support topologies more than 4 CPUs (e.g. 8S, 6S, 6SG1).
    // So, just degrade to a 4S topology.
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "Not discover any supported 6S topology.\n"));
    Status = KTI_FAILURE;
  }

  return Status;
}
