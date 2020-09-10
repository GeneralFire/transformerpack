/** @file
  Kti IP Lib functions which is used for single socket.

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

#include <Uefi.h>
#include <Base.h>

/**
Empty routine to collect and clear KTI Agent errors

@param SocketId               - Socket to program
@param Port                   - Port to program
@param LinkValid              - Is there a valid link on this port - True/False
@param OutKtiFpgaPresent      - Is FPGA Present for this socket - True/False
@param OutKtiFpgaEnable       - Is FPGA enabled by KTIRC - True/False
@param SocketFpgasInitialized - Is FPGA Initialized - True/False (SocketFpgasInitialized() adds a scratchpad read each time this function is called)

@retval 0     - Successful
@retval Other - failure

**/
VOID
EFIAPI
CollectAndClearKtiPortErrors (
  IN UINT8                    SocketId,
  IN UINT8                    Port,
  IN BOOLEAN                  LinkValid,
  IN BOOLEAN                  OutKtiFpgaPresent,
  IN BOOLEAN                  OutKtiFpgaEnable,
  IN BOOLEAN                  SocketFpgasInitialized
) {
  return;
}

/**
Returns the total number of M3KTI agents
KtiIpLibNull will always report zero M3KTI agents

@param SocId             - Soc ID with (or without) M3KTI agents

@retval number of M3KTI

**/
UINT8
EFIAPI
GetM3KtiCount (
  UINT8                      SocId,
  UINT8                      MaxM3kti
  )
{
  return 0;
}

/**
Empty routine to setup M3KTI misc
Called at end of SetupKtiMisc()

@param SocId             - Socket to program
@param M3Kti             - M3Kti number

@retval None
**/
VOID
EFIAPI
SetupM3KtiMisc (
  IN UINT8                    Soc,
  UINT8                       M3Kti
) {
  return;
}

/**
  Routine to program M3KTI Snoop Fanout

  @param[in] SocId             Socket to program
  @param[in] M3Kti             M3Kti number
  @param[in] M3KtiSnpFanOut    Pointer to an array of Snoop Fanout route setting (Element 0 is
                               for Port 0, element 1 is for Port 1. Port 1 is only applicable to
                               SOC that has shared KTI agent.
  @param[in] M3KtiSnpFanOut1   Pointer to another array of Snoop Fanout route setting for SOC.
                               Currently this parameter is only used by CPX for destination node
                               IDs 4 to 7.
  @param[in] ProgramPeMode     PE bit setting. True/False

  @retval None
**/
VOID
EFIAPI
ProgramM3KtiSnoopFanout (
  IN UINT8                    SocId,
  IN UINT8                    M3Kti,
  IN UINT32                   *M3KtiSnpFanOut,
  IN UINT32                   *M3KtiSnpFanOut1,
  IN BOOLEAN                  ProgramPeMode
  )
{
  return;
}

/**
  Empty routine to program SNC Upper Base in KTI Agent
  called by ProgramSncUpperBaseFor52BitExtend() on cold and warm reset flows

  @param Soc               - Socket Id
  @param Port              - Port Id
  @param SncUpperBase      - SNC Upper Base value

**/
VOID
EFIAPI
KtiAgentSetSncUpperBase (
  IN UINT8                    Soc,
  IN UINT8                    Port,
  IN UINT32                   SncUpperBase
  )
{
  //
  //Empty function to keep API consistency
  //
  return;
}

/**
  Empty routine to program all SNC_BASE for all clusters
  Called on warm-reset-path only
  Called from ConfigureSncMode()

  @param Soc               - Socket ID
  @param Port              - Port ID
  @param Cluster           - Cluster Index
  @param ClusterSncBase    - Cluster SNC Base

**/
VOID
EFIAPI
KtiAgentSetSncBaseAddr (
  IN UINT8  Soc,
  IN UINT8  Port,
  IN UINT8  Cluster,
  IN UINT32 ClusterSncBase
  )
{
  return;
}

/**
  Empty routine to program UMA Cluster register in Kti Agents
  Called from ProgramUmaClusterConfig(), loop for each socket and each port

  @param  Socket               -  Socket Id
  @param  Port                 -  Port Id
  @param  UmaClusterEnable     -  Enable bit for UMA based clustering mode
  @param  DefeatureXor         -  Defeature Xor

  @retval None
**/
VOID
EFIAPI
ProgramKtiAgentUmaCluster (
  IN UINT8                    Socket,
  IN UINT8                    Port,
  IN UINT8                    UmaClusterEnable,
  IN UINT8                    DefeatureXor
  )
{
  return;
}

/**
Empty routine to program M3EFQPrivC2 register

@param SocId             - Socket ID
@param M3Kti             - M3KTI number

@retval None
**/
VOID
EFIAPI
ProgramM3EFQPrivC2 (
  UINT8                      SocId,
  UINT8                      M3Kti
  )
{
  return;
}

/**
Empty routine to program SNC_Base_1 and SNC_BASE_2, for 2LM case
Called from Program2LMHashRegister()

@param  Socket               -  Socket Id
@param  Port                 -  Port Id
@param  TwoLM_Address_Mask   -  Pointer to KTI_AGENT_2LM_ADDRESS_MASK

@retval None
**/
VOID
EFIAPI
ProgramKtiAgent2LMAddrMask (
  IN UINT8                    Socket,
  IN UINT8                    Port,
  IN UINT32                   *TwoLM_Address_Mask
  )
{
  return;
}

/**
Empty routine to program Number of CHA per Cluster in UPI Uncore_Snc_Config
This routine is to program UMA Cluster register in Kti Agents
Called from ProgramNumOfChaPerCluster() in a loop for all Sockets and Ports

@param  SocketId            -  Socket Id
@param  Port                -  Port Id
@param  SncConfigData       -  Value program to KTI UNCORE_SNC_CONFIG register

@retval None
**/
VOID
EFIAPI
KtiUncoreChaPerCluster (
  IN UINT8                    SocketId,
  IN UINT8                    Port,
  IN UINT32                   SncConfigData
  )
{
  return;
}

/**
  Program values for KTIREUT_PH_CTR2 register.

  @param SocketId        - Socket ID
  @param KtiPort         - Kti Port number
  @param RedriverStatus  - Kit Port redriver status, TRUE: redriver present; FALSE: no redriver or not care

  @retval None
**/
VOID
EFIAPI
ProgramKtiReutPhCtr2 (
  IN UINT8                    SocketId,
  IN UINT8                    KtiPort,
  IN BOOLEAN                  RedriverStatus
  )
{
  return;
}

/**
  Increasing the L0p lock time to workaround the issue where UPI L0p transitions
  appear to be hitting a "phase wander" condition where the inactive lanes are
  not tracking like the active ones are and upon exit a relock is necessary
  creating a correctable CRC.

  @param[in] SocketId      Socket ID
  @param[in] Port          UPI Port number
**/
VOID
EFIAPI
ExtendKtiL0pExitTime (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  )
{
  //
  // Empty function to keep API consistency
  //
}
