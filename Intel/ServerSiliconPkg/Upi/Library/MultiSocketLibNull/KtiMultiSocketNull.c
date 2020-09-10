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

#include <Library/MemoryCoreLib.h>
#include <UncoreCommonIncludes.h>
#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>
#include <KtiMisc.h>
#include <KtiSetupDefinitions.h>
#include <RcRegs.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/KtiApi.h>
#include <Guid/UboxIpInterface.h>
#include <Library/PcuIpLib.h>
#include <Library/CpuAndRevisionLib.h>

KTI_STATUS
SyncUpPbspForReset (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      RstType
  )
{
  UINT32                             IntList, StopStartList, MiscList, IntPrioList, BcastList;

  if ((RstType == POST_RESET_POWERGOOD) || (KtiInternalGlobal->ForceColdResetFlow == TRUE)) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n    Setting Ubox Sticky SR02 to 0x%08X \n", 0x0001));
    UBoxSetKtiTopologyInScratchpad (0, 0, 0x0001);
  }

  if ((RstType == POST_RESET_POWERGOOD) || (KtiInternalGlobal->ForceColdResetFlow == TRUE) || (GetSysResetRequired () == POST_RESET_POWERGOOD)) {
    UBoxConfigureCpuLocalId (0, 0, 0, 0);

    UBoxConfigureGidNidMapping (0, 0, 0);

    UBoxConfigureCpuEnabled (0, 0, KtiInternalGlobal->CpuList);

    // Bcastlist
    IntList = StopStartList = MiscList = IntPrioList = KtiInternalGlobal->CpuList;
    UBoxConfigureCpuBroadcastList (0, 0, IntList, StopStartList, MiscList, IntPrioList, &BcastList);

    // CpuNodeID
    PcuSetGblNid0to3Map (0, (1 << 28));
    SetNewResetRequired (POST_RESET_WARM);
  }

  return KTI_SUCCESS;
}

KTI_STATUS
PrimeHostStructureFpga (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  return KTI_SUCCESS;
}

KTI_STATUS
PrimeHostStructurePhyLinkLayer (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  return KTI_SUCCESS;
}

KTI_STATUS
KtiTopologyDiscovery (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  return KTI_SUCCESS;
}

KTI_STATUS
KtiTransitionFullSpeed (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  return KTI_SUCCESS;
}

KTI_STATUS
KtiPhyLinkAfterWarmReset (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  return KTI_SUCCESS;
}

VOID
Tsc_Sync (
  IN KTI_SOCKET_DATA          *SocketData,
  IN UINT8                    PendingSocId,
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
)
{
  return;
}

KTI_STATUS
SetupSbspPathToAllSockets (
  OUT KTI_SOCKET_DATA          *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  //
  // Set Sbsp to the root node in MinPathTree
  //
  SocketData->Cpu[KtiInternalGlobal->SbspSoc].Valid = TRUE;
  SocketData->SbspMinPathTree[KtiInternalGlobal->SbspSoc].Node.Valid = TRUE;
  SocketData->SbspMinPathTree[KtiInternalGlobal->SbspSoc].Node.SocId = KtiInternalGlobal->SbspSoc;
  SocketData->SbspMinPathTree[KtiInternalGlobal->SbspSoc].Node.SocType = SOCKET_TYPE_CPU;
  SocketData->SbspMinPathTree[KtiInternalGlobal->SbspSoc].Node.ParentPort = 0xF;
  SocketData->SbspMinPathTree[KtiInternalGlobal->SbspSoc].Node.ParentIndex = 0x7F;

  return KTI_SUCCESS;
}

KTI_STATUS
PreProcessKtiLinks (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  )
{
  //
  // Update sysHost for Single Socket
  //
  KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].Valid = SocketData->Cpu[KtiInternalGlobal->SbspSoc].Valid;
  KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].SocId = SocketData->Cpu[KtiInternalGlobal->SbspSoc].SocId;
  KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].SocType = SocketData->Cpu[KtiInternalGlobal->SbspSoc].SocType;
  CopyMem ( (UINT8 *)KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].LepInfo,
           (UINT8 *)SocketData->Cpu[KtiInternalGlobal->SbspSoc].LinkData,
           sizeof(KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].LepInfo)
          );

  //
  // Update the system topology for Single Socket
  //
  KTIVAR->SysConfig = SYS_CONFIG_1S;
  SocketData->CpuTree[KtiInternalGlobal->SbspSoc]->Node.Valid = TRUE;
  SocketData->CpuTree[KtiInternalGlobal->SbspSoc]->Node.SocId = SocketData->Cpu[KtiInternalGlobal->SbspSoc].SocId;
  SocketData->CpuTree[KtiInternalGlobal->SbspSoc]->Node.SocType = SocketData->Cpu[KtiInternalGlobal->SbspSoc].SocType;
  SocketData->CpuTree[KtiInternalGlobal->SbspSoc]->Node.ParentPort = 0xF;
  SocketData->CpuTree[KtiInternalGlobal->SbspSoc]->Node.ParentIndex = 0x7F;

  //
  // Update KtiInternalGlobal for Single Socket
  //
  KtiInternalGlobal->OrigTotCpu = 1;
  KtiInternalGlobal->OrigCpuList = 1;
  KtiInternalGlobal->CpuList = 1;
  KtiInternalGlobal->TotCpu = 1;

  return KTI_SUCCESS;
}


KTI_STATUS
KtiFinalPbspBusCfg (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Targets,
  UINT32                     MmcfgTgtlist,
  UINT8                      SocId
    )
{
  return KTI_SUCCESS;
}

VOID
Init_IRQ_Threshold (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
)
{
  KTIVAR->OutIRQThreshold = RRQ_IRQ_THRESHOLD_DIS & 0x00FF;
  KTIVAR->OutRrqThreshold = (RRQ_IRQ_THRESHOLD_DIS & 0xFF00) >> 8;
  return;
}

/**
  Program the TOR Threshold loctorem_thresh_norm and loctorem_thresh_empty for 4S ring or 8S

  @param KtiInternalGlobal - KTIRC internal variables.

  @retval VOID

**/
VOID
Init_TOR_Threshold_Loctorem (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
)
{
  KTIVAR->OutTorThresLoctoremNorm = LOCTOREM_THRESH_NORM_DIS;
  KTIVAR->OutTorThresLoctoremEmpty = LOCTOREM_THRESH_EMPTY_DIS;

  return;
}

/**
  Program all the credits for destination M2UPI.

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
ProgramCreditForDestM2Upi (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  )
{
  return KTI_SUCCESS;
}

/**
Program all the credits for destination CHA.

@param KtiInternalGlobal - pointer to the KTI RC internal global structure
@param Soc               - Socket ID

@retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
ProgramCreditForDestCha(
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  )
{
  return KTI_SUCCESS;
}

KTI_STATUS
ProgramM3KtiCreditSetting (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8               Soc
  )
{
  return KTI_SUCCESS;
}

/**
  Program Kti Agent credits.

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramKtiCreditSetting (
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN UINT8                     Soc
  )
{
  return KTI_SUCCESS;
}

/**
  Routine to program the final IO SAD setting for the given topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuMmiohEntriesKti (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId
  )
{
  return KTI_SUCCESS;
}

KTI_STATUS
SetupSnoopFilter (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  return KTI_SUCCESS;
}

BOOLEAN
SocketFpgasInitialized (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  return FALSE;
}

KTI_STATUS
GetKtiAgentCount (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId,
  OUT UINT8                     *KtiAgentCount
  )
{
  *KtiAgentCount = 0;
  return KTI_SUCCESS;
}

KTI_STATUS
SetFpgaBusNumber (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  return KTI_SUCCESS;
}

KTI_STATUS
KtiFinalFpgaBusCfg (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  return KTI_SUCCESS;
}

KTI_STATUS
CheckForTopologyChange (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  return KTI_SUCCESS;
}

/**

  This routine assigns IO resources for Pbsp CPUs/stacks that are populated. It consults the appropriate inputs given to KTIRC and
  applies the following set of rules:
    1. Resource will be allocated only for the CPUs that are currently populated
     1a. If hot pluggable system, will preallocate resources for all sockets
     1b. On EX Hotpluggable systems, assume same number stacks per socket as sbsp

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.
  @param IoAlloc           - IoAllocation value for each sokcet, stack
  @param IoBase            - IoBase for resource allocation

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
CalculateIoResourcesForPbsp (
  IN KTI_SOCKET_DATA            *SocketData,
  OUT KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN UINT8                      IoAlloc[MAX_SOCKET][MAX_LOGIC_IIO_STACK],
  IN UINT32                     IoBase
  )
{
  return KTI_SUCCESS;
}

/**
  Routine to program the route table for M3KTI and KTI Agent.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket Id
  @param ProgramPeMode     - PE bit setting. True/False

  @retval None

**/
VOID
ProgramM3KtiAndKtiAgentRouteTable (
  IN KTI_SOCKET_DATA          *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN UINT8                    SocId,
  IN BOOLEAN                  ProgramPeMode
  )
{
  return;
}

/**
  Routine to program D2C register

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket Id

  @retval None

**/
VOID
ProgramD2C (
  IN KTI_SOCKET_DATA          *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN UINT8                    SocId
  )
{
  return;
}

/**
  Routine to program the final IOPORT SAD for the given socket

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuIoEntriesKti (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId
  )
{
  return KTI_SUCCESS;
}

/**
  Routine to program the final IoApic SAD for given socket

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuIoApicEntriesKti (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId
  )
{
  return KTI_SUCCESS;
}

/**
  Routine to program the final Mmmiol rules for given socket

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0 - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuMmiolEntriesKti (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId
  )
{
  return KTI_SUCCESS;
}

/**
  Routine to program MISC SAD register in KTI Agent

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramKtiMiscSadEntries (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId
  )
{
  return KTI_SUCCESS;
}

/**
  Routine to program the final MMCFG SAD for the given socket links

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
KtiFinalBusCfgKti (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId
  )
{
  return KTI_SUCCESS;
}

/**
  Routine to collect and clear M3KTI errors

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
VOID
CollectAndClearM3KtiErrors (
  IN KTI_SOCKET_DATA          *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN UINT8                    Soc
  )
{
  return;
}

/**
  Routine to collect and clear KTI Agent errors

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
VOID
CollectAndClearKtiErrors (
  IN KTI_SOCKET_DATA          *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN UINT8                    Soc
  )
{
  return;
}

/**
  Routine to get KTI current link speed

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program
  @param MaxKtiPort        - Max KTI port number

  @retval current KTI Link Speed Mode

**/
UINT8
GetKtiCurrentLinkSpeedMode (
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN UINT8 SocId,
  IN UINT8 MaxKtiPort
  )
{
  return KTI_LINK_SLOW_SPEED_MODE;
}

/**
Program M2IOSF to M2UPI credits

@param KtiInternalGlobal - pointer to the KTI RC internal global structure
@param Soc               - Socket ID

@retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramM2iosfToM2upiCreditSetting (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  )
{
  return KTI_SUCCESS;
}

/**
Program M2IOSF to M2UPI BL P2P credit ring bitfields

@param KtiInternalGlobal - pointer to the KTI RC internal global structure
@param Soc               - Socket ID

@retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramM2iosfToM2upiP2pCredits (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  )
{
  return KTI_SUCCESS;
}

/**
Program M2IOSF to M2UPI credits

@param KtiInternalGlobal - pointer to the KTI RC internal global structure
@param Soc               - Socket ID

@retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramChaToM2upiCreditSetting (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  )
{
  return KTI_SUCCESS;
}
/**
Program M2MEM to M2UPI credits

@param KtiInternalGlobal - pointer to the KTI RC internal global structure
@param Soc               - Socket ID

@retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramM2memToM2upiCreditSetting (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  )
{
  return KTI_SUCCESS;
}

/**
  PCI read during for RAS routines.

  @param SocId   - Socket ID
  @param BoxInst - Box Instance, 0 based
  @param Offset  - Register Offset

  @retval PCI address contents

**/
UINT32
ReadCpuPciCfgRas (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset
  )
{
  return KTI_SUCCESS;
}

/**
  PCI write for RAS routines

  @param SocId   - Socket ID
  @param BoxInst - Box Instance, 0 based
  @param Offset  - Register Offset
  @param Data    - Data to write

  @retval None

**/
VOID
WriteCpuPciCfgRas (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset,
  UINT32   Data
  )
{
  return;
}

/**
  Mask address parity error for P2P transactions

  @param[in] KtiInternalGlobal     KTIRC internal variables.
  @param[in] SocId                 Root socket ID

**/
VOID
P2PTransactionWa (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    SocId
  )
{
  return ;
}

/**
  Routine to get topology for credit programming.

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval Topology index  - Topology defined as defined for each CPU type

**/
UINT8
GetTopologyIndexForCredit (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  )
{
  return 0;
}

/**
  Routine to get the index used to access credits arrays based on the stack number being used

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Stack             - Stack number whose index is to be computed
  @param StackIndex        - Stack Index returned

  @retval SUCCESS if stack number passed is valid, FAILURE otherwise

**/
KTI_STATUS
GetIndexForStack (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Stack,
  UINT8                      *StackIndex
  )
{
  if (StackIndex == NULL) {
    return KTI_FAILURE;
  }

  if (KtiInternalGlobal->CpuType == CPU_CPX) {
    switch (Stack) {
    case 0:
      *StackIndex = 0;
      break;
    case 1:
      *StackIndex = 1;
      break;
    case 3:
      *StackIndex = 2;
      break;
    default:
      return KTI_FAILURE;
    }
    return KTI_SUCCESS;
  }

  return KTI_FAILURE;
}

/**
  Program Vna Bl credits for destination M2UPI (when cleard on reset)

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramM2MVnaBlCreditForDestM2Upi (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      Soc
  )
{
  //
  // Empty function for API consistency
  //
  return KTI_SUCCESS;
}

/**
  This routine checks if adaptation needs to be executed or not. It also updates the Adaptation status scratch register
  with the outcome. Executing the Adaptation engine is necessary only if all the populated sockets support
  adaptation and at least one of the following is TRUE:

      - CPU parts on the platform are changed/swapped
      - A valid adaptation table is not found
      - A valid adaptation table is found but the table entry for the requested adaptation speed is not found

  Note: Single socket doesn't support Txeq Adaptation

  @param KtiInternalGlobal - KTIRC internal variables.

  @retval BOOLEAN - TRUE  - if adaptation needed; FALSE - if not adaptation required

**/
BOOLEAN
CheckIfAdaptationNeeded (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  return FALSE;
}

//
// Single socket returns MAX_KTI_LINK_SPEED, only for compile
//
UINT8
GetMaxSupportedLinkSpeed (
  VOID
  )
{
  return MAX_KTI_LINK_SPEED;
}

/**
  There are two types of UPI routing tables: 16-entry route table, or 8-entry table.

  This routine returns TRUE if the SOC supports 16-entry route table. Else, it returns
  FALSE. SOC that supports more than 3 UPI links implements 16-entry route table.

  @retval  TRUE if 16-entry UPI route table is supported. FALSE otherwise.

**/
BOOLEAN
Is16EntryUpiRoutingTableSupported (
  VOID
  )
{
  //
  // Return FALSE for non-multisocket
  //
  return FALSE;
}