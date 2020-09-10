/** @file
  This library is used for UNCORE multi-sockets support, including topology discovery, UPI speed transition etc.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef _MULTI_SOCKET_LIB_H_
#define _MULTI_SOCKET_LIB_H_

//
// KTI RAS related build options
//
#define RAS_KTI_TIME_SLICE
#define RAS_KTI_DEBUG
#define RAS_KTI_DEBUG_REG_DETAIL
//#define RAS_KTI_UNIT_TEST
#define RAS_KTI_MEASURE_TIME

//
// KTI RAS debug message should not be enabled in PEI phase
//
#ifdef IA32
#undef RAS_KTI_DEBUG
#undef RAS_KTI_DEBUG_REG_DETAIL
#endif

//
// KTI Defined constants for MultiSocket Library
//
#define UPI_PORT_INVALID 0xFF

//
// Delays for emulation environment
//
#define EMU_DELAY_TO_START_TRAINING         0x40000
#define EMU_DELAY_FOR_TRAINING              0x10000

/**
  Routine to sync up the PBSPs and update the necessary scratch registers. As part of that,
  the sockets that are not connected to legacy are issued "BOOT_GO" command to bring them
  under BIOS control.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param RstType           - Boot path type.  powergood or warm reset flow

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
SyncUpPbspForReset (
  IN KTI_SOCKET_DATA            *SocketData,
  OUT KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN UINT8                      RstType
);

/**
  Prime the KTIRC data structure based on fpga input option and system topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0 - Successful
  @retval Other - failure

**/
KTI_STATUS
PrimeHostStructureFpga (
  IN KTI_SOCKET_DATA            *SocketData,
  OUT KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
);

/**
  Prime the KTIRC data structure based on input option and system capabilities.
  Also normalize DFX per-link options.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
PrimeHostStructurePhyLinkLayer (
  IN KTI_SOCKET_DATA            *SocketData,
  OUT KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
);

/**
  Program BGF Pointer Separation Overrides as workarounds for KTI RC.
  SBSP writes these values via Mailbox Commands to all populated sockets.  Values will take effect after a warm reset.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
KtiProgramBGFOverrides (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
);

/**
   Perform the KTI Topology Discovery of the system. It discovers the system
   topology from each socket's perspective and calculates the route. This
   function must be invoked only by SBSP after collecting LEP of each socket populated.
   This function doesn't program any H/W registers. Route Table of each socket is
   returned in SocketData. The caller is responsible for porgramming the Route Table into
   the appropriate registers upon successful exit from this function.

   @param SocketData        - Pointer to socket specific data
   @param KtiInternalGlobal - KTIRC internal variables.

   @retval 0     - Success
   @retval Other - Failure

**/
KTI_STATUS
KtiTopologyDiscovery (
  OUT KTI_SOCKET_DATA           *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
);

/**
   Transition the KTI links to full speed operation.

   To transition the link to full speed, for each link we need to:
      1. Validate the phy/link layer input parameters; force safe values if needed
      2. Ensure that no partial loading of parameters by setting LA Disable
      3. Program the Link & Phy layer parameters
      4. Clear LA Disable

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
KtiTransitionFullSpeed (
  IN KTI_SOCKET_DATA           *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
);

/**
   Transition the KTI links to full speed operation on warm reset.

   To transition the link to full speed, for each link we need to:
      1. Validate the phy/link layer input parameters; force safe values if needed
      2. Ensure that no partial loading of parameters by setting LA Disable
      3. Program the Link & Phy layer parameters
      4. Clear LA Disable

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
KtiTransitionFullSpeedOnWarmReset (
  IN KTI_SOCKET_DATA           *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
);

/**
  KTI Physical Layer programming performed after warm reset.

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS - on successful procedure execution

**/
KTI_STATUS
KtiPhyLinkAfterWarmReset (
  IN KTI_SOCKET_DATA           *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
);

/**
  TSC Sync for all sockets
  1. For TSC alignment, BIOS forces assertion of TSC_SYNC ( MAILBOX_BIOS_CMD_TSC_SYNC ) to SBSP.
  2. This triggers TSC capture (need to delay for 20us after completing this on all sockets).
  3. BIOS deasserts TSC_SYNC via same processor command as for assertion (except with deassert state).
  4. BIOS calculates delta between Largest TSC and all sockets ( or BSP and new sockets' TSCs for on-line). Delta =(BSP TSC - AP TSC).
  5. BIOS writes TSC_HOT_PLUG_OFFSET register in sockets or new skt (one time update only).

  @param SocketData        - add argument and description to function comment
  @UINT8 PendingSocId      - The pending sock is on-lined
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval Nothing

**/
VOID
Tsc_Sync (
  IN KTI_SOCKET_DATA          *SocketData,
  IN UINT8                    PendingSocId,
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
);

/**
  Routine to set minimum required route to allow config accesses coming from one of SBSP's core
  to be routed to the agent that claims the region. It also constructs the SBSP minimum tree
  and collects information such as CPU sockets populated and their LEP info to facilitate
  the topology discovery process to be done without any config access. The route set by this
  routine may not be the optimum path. Note that, the route set by this routine allows only the
  SBSP be able to access the config space of other CPU sockets in the system; not from
  one PBSP to other PBSPs.

  It also sets up the route to the Legacy CPU so that >1 hop away CPUs can be released from reset
  and brought under BIOS control.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
SetupSbspPathToAllSockets (
  OUT KTI_SOCKET_DATA          *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
);

/**
  Routine to check if the system has supported topology. Following terminology is used in
  describing the system topology:
  1S    - Only socket 0 is populated
  2S1L -Socket 0 & 1 are populated; there is only one link between them. Link number doesn't matter.
  2S2L -Socket 0 & 1 are populated; there are two links between them. (Link 0 & 1 only)
  4S -Total number of populated sockets is <= 4, populated sockets have ID < 4, and it is not a 1S/2S system.
  8S -All other configurations.

  Note:
    When degraded topology is detected (i.e one or more links failed), BIOS will try to reduce it to one of the
    supported topology by forcing certain links and/or sockets out. Link/Socket removal is done by marking the
    appropriate data structures; nothing specifically done to the h/w that will disable a link or socket. That will
    take effect only after next reset. We track the number of and list of sockets populated before and after
    topology reduction is applied. While KTIRC uses only the reduced topology information, other modules
    such as the code that issues Warm reset needs to be aware of what is the list of sockets populated
    currently. So we update the Host numCpus/socketPresentBitMap structure with the current number of and
    list of sockets populated, not based on the reduced topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
PreProcessKtiLinks (
  OUT KTI_SOCKET_DATA          *SocketData,
  OUT KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
);

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
);

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
);

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
);

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
  );

/**
Program all the credits for destination CHA.

@param KtiInternalGlobal - pointer to the KTI RC internal global structure
@param Soc               - Socket ID

@retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
ProgramCreditForDestCha (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
);

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
);
/**

  Program M3KTI credits

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramM3KtiCreditSetting (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      Soc
);
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
);

/**
  SBSP programs bus numbers and mmcfg rules to each socket.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Targets           - Enabled socket targets
  @param MmcfgTgtlist      - Common mmcfg target list
  @param SocId             - Socket to program

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
KtiFinalPbspBusCfg (
  IN KTI_SOCKET_DATA            *SocketData,
  OUT KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN UINT8                      Targets,
  IN UINT32                     MmcfgTgtlist,
  IN UINT8                      SocId
);

/**
  s5331840: Program the KTI IRQ Threshold for 4S ring or 8S

  @param KtiInternalGlobal - KTIRC internal variables.

  @retval VOID

**/
VOID
Init_IRQ_Threshold (
  OUT KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
);

/**
  Program the TOR Threshold loctorem_thresh_norm and loctorem_thresh_empty for 4S ring or 8S

  @param KtiInternalGlobal - KTIRC internal variables.

  @retval VOID

**/
VOID
Init_TOR_Threshold_Loctorem (
  OUT KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Program KTI link credits

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramKtiCreditSetting (
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN UINT8                    Soc
);

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
);

/**
  Setup system snoop filter for fpga.

  @param SocketData        - pointer to the socket data structure
  @param KtiInternalGlobal - pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
SetupSnoopFilter (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
);

/**
  Returns boolean if FPGA is initialized.

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval TRUE  - Blue Bitstream previously downloaded
  @retval FALSE - Blue Bitstream not already downloaded

**/
BOOLEAN
SocketFpgasInitialized (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
);


/**
Returns the total number of M3KTI agents
KtiIpLibNull will always report zero M3KTI agents

@param SocId             - Soc ID with (or without) M3KTI agents
@param MaxM3kti          - Max M3KTI agents

@retval number of M3KTI

**/
UINT8
EFIAPI
GetM3KtiCount (
  UINT8                      SocId,
  UINT8                      MaxM3kti
);


/**
  Returns the total KTIAgents supported by this CPU socket.

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Soc ID of the CPU who's enabled LLC slice info is requested, 0...3
  @param KtiAgentCount     - Total KTIAgent in given socket

  @retval 0 - Successful
  @retval 1 - Failure

**/
KTI_STATUS
GetKtiAgentCount (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId,
  OUT UINT8                     *KtiAgentCount
);

/**
  Set the Bus Number for WHR and SIP, on all FPGA nodes.
  Also set the valid bit to enable usage of this bus number.
  This function should be defined in an FPGA HW IP library.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
SetFpgaBusNumber (
  IN KTI_SOCKET_DATA             *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL    *KtiInternalGlobal
);

/**
  SBSP programs bus numbers for fpgas.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
KtiFinalFpgaBusCfg (
  IN KTI_SOCKET_DATA             *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL    *KtiInternalGlobal
);

/**
  Routine to check if the topology changed after warm reset. Any such topology change requires
  KTIRC cold reset flow to be forced on the system since the uncore mesh programming is no longer correct for the
  changed topology. Note that when such topology change occurs it could be only due to some links not training; it can not
  be due to some links that failed to train before but trained successfully this time around. The reason is a link
  that failt to train is disabled by clearing c_init_begin. So such links can successfully train only after Cold Reset.

  @param SocketData        - Info such as CPUs populated and their LEP.
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
CheckForTopologyChange (
  IN KTI_SOCKET_DATA            *SocketData,
  OUT KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
);

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
);

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
);

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
);

/**
  Routine to initialize whether KTI Port 0 and 1 share a single mesh stop

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval none

**/
VOID
InitializeKtiSharePort0AndPort1 (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

/**
  Check if the CPU type in EV recipe row is valid.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param ProcessorTypeMask - Supported processor type bitmask from EV recipe row

  @retval TRUE/FALSE

**/
BOOLEAN
KtiIsValidCpuType (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT64                     ProcessorTypeMask
);

/**
  Check if the KTI Port index in EV recipe row is valid.

  @param PortId       - Index of the KTI port being programmed
  @param PortListMask - KTI port bitmask from EV recipe row

  @retval TRUE/FALSE

**/
BOOLEAN
KtiIsValidPort (
  UINT8  PortId,
  UINT32 PortListMask
);

/**
  Apply UniPhy recipe overrides, if necessary.

  @param Soc  - Socket ID being programmed
  @param Link - KTI port being programmed
  @param Type - Register access type
  @param Addr - Register address
  @param Mask - Register bitfield mask
  @param Data - Value to program into the register

  @retval TRUE  - Register override successfully performed
  @retval FALSE - No need to override the register/override failed

**/
BOOLEAN
OverrideUniphyValues (
  UINT8          Soc,
  UINT8          Link,
  UINT8          Type,
  UINT32         Addr,
  UINT32         Mask,
  UINT32         *Data
);

/**
  Check if the Config index in EV recipe row is valid.
  Current supported configs are KTI_CFG, FPGA_CFG, and WILDCARD_16

  @param KtiInternalGlobal - KTIRC internal variables.
  @param Socket     - Socket ID of register to be programmed
  @param PortId       - Index of the KTI port being programmed
  @param Config     - Config value from KTI EV recipe (WILDCARD_16, KTI_CFG, or FPGA_CFG)

  @retval TRUE/FALSE
**/
BOOLEAN
KtiIsValidConfig (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8  Socket,
  UINT8  PortId,
  UINT16 Config
);

/**
   Returns TRUE if the CPU was not already released to fetch code

   @param SocketData        - Pointer to socket specific data
   @param KtiInternalGlobal - KTIRC internal variables.
   @param SocId             - CPU Socket Number, 0 based

   @retval TRUE  - CPU requires boot_go
   @retval FALSE - CPU does not need boot_go

**/
BOOLEAN
SocketRequireBootGo (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
);

//
// 10nm only start
//

//
// 10nm only end
//

/**
  Sets up route-through route on a socket

  @param SocketData - Pointer to socket specific data
  @param SocId      - Pass-thru socket ID
  @param DestSocId  - endpoint processor socket ID
  @param InPort     - pass-thru port towards the root
  @param OutPort    - pass-thru port towards the destination
  @param RootVn     - Root socket's VN

  @retval 0 - Successful
  @retval 1 - Failure

**/
KTI_STATUS
SetPassThruRoute (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    SocId,
  UINT8                    DestSocId,
  UINT8                    InPort,
  UINT8                    OutPort,
  UINT8                    RootVn
);

/**
  Set Root Node Route and its VN

  @param[in,out] SocketData        Pointer to socket specific data
  @param[in] KtiInternalGlobal     KTIRC internal variables.
  @param[in] SocId                 Root socket ID
  @param[in] IntermediateSocId     Intermediate node ID (the node is either the endpoint itself or connected directly to RootNode)
  @param[in] DestSocId             Endpoint processor socket ID
  @param[in] OutPorts              Array to save KTI ports of node
  @param[in] OutPortsNumber        Ports number in array OutPorts
  @param[in] MultiLink             TRUE - Dual or triple or four links exists; FALSE - No Dual link nor triple link nor quad link
  @param[in] AlternateRoute        TRUE indicates there is second route between Root Node (SocId) and Destination
                                   Node (DestSocId); therefore update the DestSocId's second entry with this route.
                                   FALSE otherwise.
  @param[in] RootVn                Root socket's VN

  @retval KTI_SUCCESS  CHA & M2PCIE RT and VN settings data structures are updated.
  @retval KTI_FAILURE  Invalid OutPort1 and OutPort2 port ID. No data structures are updated.

**/
KTI_STATUS
SetRootNodeRoute (
  IN OUT KTI_SOCKET_DATA          *SocketData,
  IN     KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN     UINT8                     SocId,
  IN     UINT8                     IntermediateSocId,
  IN     UINT8                     DestSocId,
  IN     UINT8                     *OutPorts,
  IN     UINT8                     OutPortsNumber,
  IN     BOOLEAN                   MultiLink,
  IN     BOOLEAN                   AlternateRoute,
  IN     UINT8                     RootVn
  );

/**
  Dump topology info to output device

  @param SocketData        - socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval VOID

**/
VOID
RouteTableDump (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
);

/**
  Configure CHA Routing table for SBSP or other node that is connected to SBSP.

  @param[in] SocId                 CPU Socket Node number (Socket ID)
  @param[in] ChaCount              Total number of CHAs for the socket "SocId"
  @param[in] ChaPortFieldWidth     Width of field used for UPI port
  @param[in] LocalPort             Local port used for routing to Destination Socket
  @param[in] DestSocId             Destination CPU Socket Node number (Socket ID)
  @param[in] UpiRoutingMode        UPI routing mode. See UPI_ROUTING_MODE enum.

  @retval KTI_SUCCESS  CHA Route Table was programmed.

**/
KTI_STATUS
ConfigureChaKtiRTForSbspOrRouteToSbsp (
  UINT8    SocId,
  UINT8    ChaCount,
  UINT8    ChaPortFieldWidth,
  UINT8    LocalPort,
  UINT8    DestSocId,
  UINT8    UpiRoutingMode
);

/**
  Determine the CHA KTI Route Table programming values based on the UPI routing mode.
  This routine populates the route table setting into ChaKtiRt and ChaKtiRt1 in
  SocketData structure.

  @param[in,out] SocketData        Pointer to socket specific data
  @param[in] KtiInternalGlobal     Pointer to KTIRC internal variables structure
  @param[in] RootSocId             Root Node node ID (a.k.a. socket ID)
  @param[in] IntermediateSocId     Intermediate node ID (the node is either Destination Node
                                   itself or connected directly to RootNode)
  @param[in] DestSocId             Destination node node ID (a.k.a. socket ID)
  @param[in] OutPorts              Array to save KTI ports of Root Node
  @param[in] MaxPorts              Max ports number in array OutPorts
  @param[in] MultiLink             TRUE - Dual or triple or four links exists;
                                   FALSE - No Dual link nor triple link nor quad link
  @param[in] AlternateRoute        TRUE indicates there is second route between Root Node (SocId)
                                   and Destination Node (DestSocId); therefore update the DestSocId's
                                   second entry with this route.
                                   FALSE otherwise.

  @retval KTI_SUCCESS  ChaKtiRt (and ChaKtiRt1) are populated with valid route table settings.

**/
KTI_STATUS
SetRootNodeRouteInChaKtiRouteTable (
  IN OUT KTI_SOCKET_DATA          *SocketData,
  IN     KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN     UINT8                     SocId,
  IN     UINT8                     IntermediateSocId,
  IN     UINT8                     DestSocId,
  IN     UINT8                     *OutPorts,
  IN     UINT8                     MaxPorts,
  IN     BOOLEAN                   MultiLink,
  IN     BOOLEAN                   AlternateRoute
);

/**
  Dump CHA KTI Route table programming values to output device

  @param[in] SocketData          Socket specific data structure
  @param[in] KtiInternalGlobal   KTIRC internal variables.

**/
VOID
ChaKtiRouteTableDump (
  IN KTI_SOCKET_DATA           *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
);

/**
  Dump KTI Agent Route table programming values to output device

  @param[in] SocketData          Socket specific data structure
  @param[in] KtiInternalGlobal   KTIRC internal variables.

**/
VOID
KtiAgentRouteTableDump (
  IN KTI_SOCKET_DATA           *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

/**
  Dump M3KTI Route table programming values to output device

  @param[in] SocketData          Socket specific data structure
  @param[in] KtiInternalGlobal   KTIRC internal variables.

**/
VOID
M3KtiRouteTableDump (
  IN KTI_SOCKET_DATA           *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

/**
  Dump M2PCIe route table to output device

  @param SocketData        - socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval VOID

**/
VOID
M2PcieRouteTableDump (
  IN KTI_SOCKET_DATA           *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

/**
  Routine to get topology for credit programming.

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval TOPOLOGY4CREDIT  - Topology defined as TOPOLOGY4CREDIT enum.

**/
TOPOLOGY4CREDIT
GetTopology4Credit (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  );

/**
  Returns boolean if it is an internal UPI port for given socket ID and link index

  @param KtiInternalGlobal - KTIRC internal variables.
  @param Soc               - Socket ID.
  @param LinkIndex         - Link index value

  @retval TRUE             - UPI port is internal
  @retval FALSE            - UPI port is not internal

**/
BOOLEAN
IsInternalUpiPort (
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN UINT8                    Soc,
  IN UINT8                    LinkIndex
);

/**
  Get internal Eparameter table information

  @param [in, out]AllLanesPtr             - Pointer of the pointer to all lane table.
  @param [in, out]AllLanesSizeOfTable     - Pointer to all lane table size.
  @param [in, out]PerLanePtr              - Pointer of the pointer to per lane table.
  @param [in, out]PerLaneSizeOfTable      - Pointer to per lane table size.

  @retval KTI_SUCCESS                     - Get internal Eparameter table successfully.
  @retval KTI_FAILURE                     - There is no proper eparameter table .
**/

KTI_STATUS
GetInternalEparameterTable (
  ALL_LANES_EPARAM_LINK_INFO  **AllLanesPtr,
  UINT32                      *AllLanesSizeOfTable,
  PER_LANE_EPARAM_LINK_INFO   **PerLanePtr,
  UINT32                      *PerLaneSizeOfTable
  );

/**
  The Misc. Setting for Kti speed transition.

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.
  @param SpeedMode         - The target speed mode, 1: Fast mode; 0: slow mode

**/
KTI_STATUS
KtiSpeedTransitionMiscSteps (
  IN KTI_SOCKET_DATA           *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN UINT8                     SpeedMode
  );

/**
  This routine will remove a specified socket from the system.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SpecifiedSoc      - The specified socket to be removed

  @retval KTI_SUCCESS      - Successful

**/
KTI_STATUS
EFIAPI
RemoveOneSpecifiedSocket (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  UINT8                     SpecifiedSoc
  );

/**
  Mask address parity error for P2P transactions

  @param[in] KtiInternalGlobal   - KTIRC internal variables.
  @param[in] SocId               - Root socket ID

  @retval                        - VOID.

**/
VOID
P2PTransactionWa (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    SocId
  );

/**
  This routine will collect the topology information for all sockets.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocInfo           - The pointer to the array of SOCKET_TOPOLOGY_INFO type

  @retval                  - VOID.

**/
VOID
EFIAPI
CollectTopologyInfo (
  IN     KTI_SOCKET_DATA             *SocketData,
  IN     KTI_HOST_INTERNAL_GLOBAL    *KtiInternalGlobal,
  IN OUT SOCKET_TOPOLOGY_INFO        *SocInfo
  );

/**
  This routine will find and remove all sockets that are not in the RetainedSocList from the system.
  Note:
  The legacy socket will always be retained no matter it is in the RetainedSocList.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param RetainedSocList   - A pointer to the list of socket IDs that need be retained
  @param RetainedSocCnt    - The number of socket IDs in the list

  @retval KTI_SUCCESS      - Successful

**/
KTI_STATUS
EFIAPI
RemoveUnusedSockets (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  UINT8                     *RetainedSocList,
  IN  UINT8                     RetainedSocCnt
  );

/**
  This routine will disconnet two sockets by disabling the UPI links that connecting them.

  @param[in] SocketData        - Pointer to socket specific data
  @param[in] KtiInternalGlobal - KTIRC Internal Global data
  @param[in] SocId1            - The first socket to be examined
  @param[in] SocId2            - The second socket to be examined

  @retval KTI_SUCCESS          - Successful
  @retvel KTI_FAILURE          - Failed

**/
KTI_STATUS
EFIAPI
DisconnectTwoSockets (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  UINT8                     SocId1,
  IN  UINT8                     SocId2
  );

/*
  Routine to get the number of common peer sockets between two input sockets.
  Note: Caller must make sure that Socket[SocId1] and Socket[SocId2] are peers.

  @param SocId1       - The ID of Socket#1
  @param SocId2       - The ID of Socket#2
  @param SocInfo      - A pointer to the array of SOCKET_TOPOLOGY_INFO type
  @param BufSize      - The size of the buffer(in Byte)
  @param SocBuffer    - A pointer to the buffer to hold the common peer socket IDs

  @retval             - The number of common peer sockets. In addition, if SocBuffer is not NULL,
                        at most BufSize(if they have) of common peer socket IDs will be returned
                        in the buffer pointed by SocBuffer.
*/
UINT8
EFIAPI
GetCommonPeerSocketCnt (
  IN  UINT8                 SocId1,
  IN  UINT8                 SocId2,
  IN  SOCKET_TOPOLOGY_INFO  *SocInfo,
  IN  UINT8                 BufSize,
  OUT UINT8                 *SocBuffer
  );

/*
  Routine to discover clump(4S fully connected) that contains the input socket.

  @param SocketId          - The ID of the input Socket to be examined if it is part of a clump
  @param SocInfo           - A pointer to the array of SOCKET_TOPOLOGY_INFO type
  @param ExcludedSocList   - A pointer to a list of sockets that should be excluded (If NULL, no socket will be excluded)
  @param ExcludedSocCnt    - Number of sockets that should be excluded (If 0, no socket will be excluded)
  @param ClumpSocList      - A pointer to the buffer to store the returned Clump socket list
                             (If ClumpSocList is NULL, then will not return Clump socket list)

  @retval                  - TRUE:  A clump including SocketId is discovered
                           - FALSE: No clump that includes SocketId is discovered
*/
BOOLEAN
EFIAPI
DiscoverClump (
  IN  UINT8                     SocketId,
  IN  SOCKET_TOPOLOGY_INFO      *SocInfo,
  IN  UINT8                     *ExcludedSocList,
  IN  UINT8                     ExcludedSocCnt,
  OUT UINT8                     *ClumpSocList
  );

/*
  Routine to degrade to a single Clump(a.k.a Fully Connect Cube).

  @param[in] SocketData         - Socket specific data structure
  @param[in] KtiInternalGlobal  - KTIRC internal variables
  @param[in] Clump              - A list of all 4 sockets included in the clump

  @retval    VOID
*/
VOID
EFIAPI
DegradeToSingleClump (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  UINT8                     *Clump
  );

/*
  Process the serialized topology degradation according to the degrade rules.

  @param[in] SocketData               - Socket specific data structure
  @param[in] KtiInternalGlobal        - KTIRC internal variables.
  @param[in] SocInfo                  - A pointer to the array of SOCKET_TOPOLOGY_INFO type

  @retval    TRUE                     - Topology degradation is done successfully.
*/
BOOLEAN
EFIAPI
ProcessSerializedTopologyDegradation (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  SOCKET_TOPOLOGY_INFO      *SocInfo
  );

/**
  Copies the socket data to disc engine structure and applies the KTI port mapping to make
  it Si agnostic.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - Pointer to socket specific data

  @retval 0                - Success
  @retval Other            - Failure

**/
KTI_STATUS
PrepareDiscEngData (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  );

UINT32
ReadCpuPciCfgRas (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset
  );

VOID
WriteCpuPciCfgRas (
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset,
  UINT32   Data
  );

/**
  There are two types of UPI routing tables: 16-entry route table, or 8-entry table.

  This routine returns TRUE if the SOC supports 16-entry route table. Else, it returns
  FALSE. SOC that supports more than 3 UPI links implements 16-entry route table.

  @retval  TRUE if 16-entry UPI route table is supported. FALSE otherwise.

**/
BOOLEAN
Is16EntryUpiRoutingTableSupported (
  VOID
  );
#endif // _MULTI_SOCKET_LIB_H_
