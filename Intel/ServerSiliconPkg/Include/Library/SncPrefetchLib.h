/** @file
  Header file for SncPrefetch library

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

#ifndef _SNC_PREFETCH_LIB_H
#define _SNC_PREFETCH_LIB_H

#include <Upi/KtiHost.h>
#include <Library/RcDebugLib.h>

/**
  Disable SNC Flag

  This routine to disable SNC flag for warm reset

  @param  KtiInternalGlobal - KTIRC Internal Global data

  @retval None

**/
VOID
DisabledSncFlag (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Disable Prefetch Flag

  This routine to disable Prefetch flag for warm reset

  @param  KtiInternalGlobal - KTIRC Internal Global data

  @retval None

**/
VOID
DisabledPrefetchFlag (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Programs SNC and Prefetch Mode

  This routine programs the SNC and Prefetch mode.

  @param  KtiInternalGlobal- KTIRC Internal Global data
  @param  MemInfo          - Memory Information structure.

  @retval UNCORE_SUCCESS   - SNC mode is programmed successfully.
  @retval UNCORE_FAILURE   - Memory profile is not suitable for SNC mode operation.

**/
KTI_STATUS
EnableSncMode (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  MEM_INFO                 *MemInfo
  );

/**
  Determine the channel configuration meets the symmetric requirement or not
  1-Cluster: Need all enabled MCs and their populated channels symmetric
  2-Cluster (SNC2): Need MCs in the same cluster symmetric. Channels within
                    MC also need to be symmetric.
  4-cluster (SNC4): Populated channels within the MC should be symmetric

  @param[in] KtiInternalGlobal  KTIRC Internal Global data
  @param[in] Socket             Socket Node ID
  @param[in] MemInfo            Memory Information structure

  @retval KTI_FAILURE  Doesn't meet prefetch symmetric requirement
          KTI_SUCCESS  Meet prefetch symmetric requirement, or
                       prefetch was set to disable during SAD entries scanning
                       and therefore symmetric check can be skipped.

**/
KTI_STATUS
CheckPrefetchSymmetric (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      Socket,
  IN MEM_INFO                   *MemInfo
  );

/**
  Routine to update KTIVAR KtiPrefetch value as the register in KTI Agent

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval None

**/
VOID
UpdateKtiPrefetchStatusAsRegisterValue (
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  );

/**
  Dump SNC and XPT register in Kti Agents and M3KTI

  @param  KtiInternalGlobal    -  KTIRC Internal Global data
  @param  Soc                  -  Socket Id

  @retval None

**/
VOID
DumpKtiAgentM3KtiSncXptRegister (
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN UINT8                    Soc
  );


/**
  Program Uma Cluster Config

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param  Socket           - Socket

  @retval None

**/
VOID
ProgramUmaClusterConfig (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    Socket
  );

/**
  Routine to Check UMA based clustering downgrade
  SKX will do nothing in the routine.

  @param[in]  KtiInternalGlobal   Pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS
  @retval KTI_IGNORE

**/
KTI_STATUS
CheckUmaBasedClusteringDowngrade (
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

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
  );

/**
  SNC Misc and Recovrry if failure

  @param SocketData        - pointer to the socket data structure
  @param KtiInternalGlobal - pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
SncMisc (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  API to dump Sub Numa Clustering (SNC) and XPT register.

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval UNCORE_SUCCESS   - SNC mode is programmed successfully; or system is not configured for SNC mode operation.
  @retval UNCORE_FAILURE   - Memory profile is not suitable for SNC mode operation.

**/
KTI_STATUS
DumpSncXptKtiRegister (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Program SNC shadow register by mailbox command

  This routine sends the Mesh Mode BIOS to Pcode Mailbox command.

  @param  KtiInternalGlobal -   KTIRC Internal Global data

  @retval None

**/
VOID
ProgramSncShadowReg (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Check DDR Memory Population.

  This routine calculates number of channels populated on each memory controller, channel
  memory size and indicate if all channels are of same size.

  @param  KtiInternalGlobal -  KTIRC Internal Global data
  @param  MemInfo           -    Memory Information structure.

  @retval None

**/
KTI_STATUS
CheckMemoryPopulation (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo
  );

VOID
DisableXptPrefetch (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    Socket
  );

/**
  Update XptPrefetch/KtiPrefetch enabled status in the KtiHostOut for 10nm

  This routine query the register of XptPrefetch/KtiPrefetch enabled status,
  then update them in KtiHostOut.

  @param[in] KtiInternalGlobal - Pointer to the KTI RC internal global structure

  @retval    None
**/
VOID
KtiUpdateXptKtiPrefetchStatus (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

VOID
XptKtiPrefetchWaSkx (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Disable all prefetch for "Socket"

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Socket            - Socket

**/
VOID
DisableAllPrefetch (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Socket
  );

/**
  Reset SNC_BASE to zero for "Socket" when all SNC/Prefetch disabled

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Socket            - Socket
  @param MemInfo           - Memory Information structure.

**/
VOID
ResetAllSncBase (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Socket,
  MEM_INFO                   *MemInfo
  );
#endif // _SNC_PREFETCH_LIB_H
