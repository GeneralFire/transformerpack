/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include "SysHost.h"
#include "MrcCommonTypes.h"
#include <Library/MemoryCoreLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <PlatformInfoTypes.h>

/**

  Get the memory subsystem type.

  @param[in] Socket  - Socket to check

  @retval memory subsystem type.

**/

MRC_MST
EFIAPI
GetMemSsType (
  IN        UINT8           Socket
  )
{
  return GetSysCpuCsrAccessVar()->MemSsType[Socket];
}

/**

  Set the memory subsystem type.

  @param[in] Socket     - Socket to check
  @param[in] MemSsType  - Subsytem type

  @retval None.

**/

VOID
EFIAPI
SetMemSsType (
  IN        UINT8           Socket,
  IN        MRC_MST         MemSsType
  )
{
  GetSysCpuCsrAccessVar()->MemSsType[Socket] = MemSsType;
}

/**

  Check if operating in a Simics environment.

  @param None.

  @retval TRUE is Simics, FALSE if not.

**/

BOOLEAN
EFIAPI
IsSimics (
  VOID
  )
{
  if ((GetEmulation() & SIMICS_FLAG) != 0) {
    return TRUE;
  }
  return FALSE;
}

/**

  Get the number of memory subsystem types.

  @param[in] Socket  - Socket to check

  @retval number of memory subsystem types.

**/

UINT32
EFIAPI
GetNumMemSs (
  IN        UINT8           Socket
  )
{
  SYSHOST *Host;
  Host = GetSysHostPointer ();

  return Host->var.mem.socket[Socket].NumMemSs;
}

/**

  Get the memory subsystem type from the subsystem list.

  @param[in] Socket  - Socket to check
  @param[in] MemSs   - Subsystem to check

  @retval Memory subsystem type.

**/

MRC_MST
EFIAPI
GetMemSsTypeFromSsList (
  IN        UINT8           Socket,
  IN        UINT8           MemSs
  )
{
  SYSHOST *Host;
  Host = GetSysHostPointer ();

  return Host->var.mem.socket[Socket].MemSsList[MemSs].MemSsType;
}

/**

  Get the number of data channels from the subsystem list.

  @param[in] Socket  - Socket to check
  @param[in] MemSs   - Subsystem to check

  @retval Number of channels.

**/

UINT32
EFIAPI
GetNumDataChFromSsList (
  IN        UINT8           Socket,
  IN        UINT8           MemSs
  )
{
  SYSHOST *Host;
  Host = GetSysHostPointer ();

  return Host->var.mem.socket[Socket].MemSsList[MemSs].NumDataCh;
}

/**

  Get the number of IO FUBs per channel
  from the subsystem list.

  @param[in] Socket  - Socket to check
  @param[in] MemSs   - Subsystem to check

  @retval Number of IO FUBs.

**/

UINT32
EFIAPI
GetNumDataIoFubsPerChFromSsList (
  IN        UINT8           Socket,
  IN        UINT8           MemSs
  )
{
  SYSHOST *Host;
  Host = GetSysHostPointer ();

  return Host->var.mem.socket[Socket].MemSsList[MemSs].NumDataIoFubsPerCh;
}

/**

  Get the number of IO FUBs per subchannel
  from the subsystem list.

  @param[in] Socket  - Socket to check
  @param[in] MemSs   - Subsystem to check

  @retval Number of IO FUBs.

**/

UINT32
EFIAPI
GetNumDataIoFubsPerSubChFromSsList (
  IN        UINT8           Socket,
  IN        UINT8           MemSs
  )
{
  SYSHOST *Host;
  Host = GetSysHostPointer ();

  return Host->var.mem.socket[Socket].MemSsList[MemSs].NumDataIoFubsPerSubCh;
}

/**

  Get the number of DQ Lanes per channel
  from the subsystem list.

  @param[in] Socket  - Socket to check
  @param[in] MemSs   - Subsystem to check

  @retval Number of DQ Lanes per channel.

**/

UINT32
EFIAPI
GetNumDqLanesPerChFromSsList (
  IN        UINT8           Socket,
  IN        UINT8           MemSs
  )
{
  SYSHOST *Host;
  Host = GetSysHostPointer ();

  return Host->var.mem.socket[Socket].MemSsList[MemSs].NumDqLanesPerCh;
}

/**

  Check if running in emulation.

  @param  None

  @retval TRUE - Is running in emulation.

**/

BOOLEAN
EFIAPI
IsEmulation (
  VOID
  )
{
  if (GetEmulation() == 0) {
    return FALSE;
  }
  return TRUE;
}

/**

  Check if we are on the fast boot path.

  @param  None

  @retval TRUE - Is on the fast boot path.

**/

BOOLEAN
EFIAPI
IsFastBootPath (
  VOID
  )
{
  SYSHOST *Host;

  Host = GetSysHostPointer ();

  if ((GetSysBootMode () == S3Resume) || (Host->var.mem.subBootMode == WarmBootFast) || (Host->var.mem.subBootMode == ColdBootFast)){
    return TRUE;
  }
  return FALSE;
}

/**
  Check if the system boot mode is cold boot.

  @retval TRUE        Boot mode is cold boot.
  @retval FALSE       Boot mode is not cold boot.

**/
BOOLEAN
EFIAPI
IsSystemColdBoot (
  VOID
  )
{
  PSYSHOST       Host;

  Host = GetSysHostPointer ();

  if ((GetSysBootMode () == NormalBoot) && ((Host->var.mem.subBootMode == ColdBoot) || (Host->var.mem.subBootMode == ColdBootFast))) {
    return TRUE;
  }
  return FALSE;
}

/**

  Check if mem size is zero or not.

  @param[in] Socket  - Socket to check

  @retval TRUE - mem size is not zero.

**/

BOOLEAN
EFIAPI
IsMemSizePerSocket (
  IN        UINT8           Socket
  )
{
  SYSHOST *Host;

  Host = GetSysHostPointer ();

  if (Host->var.mem.socket[Socket].memSize != 0) {
    return TRUE;
  }
  return FALSE;
}

/**

  Get the mem size for a memory controller.

  @param[in] Socket  - Socket to check
  @param[in] McIndex - Memory Controller Index

  @retval TRUE - mem size is not zero.

**/

UINT32
EFIAPI
GetMemSize (
  IN        UINT8           Socket,
  IN        UINT8           McIndex
  )
{
  SYSHOST *Host;

  Host = GetSysHostPointer ();

  return Host->var.mem.socket[Socket].imc[McIndex].memSize;
}

/**

  Check if the vol size is zero or not.

  @param[in] Socket  - Socket to check

  @retval TRUE - vol size is not zero.

**/

BOOLEAN
EFIAPI
IsVolSizePerSocket (
  IN        UINT8           Socket
  )
{
  SYSHOST *Host;

  Host = GetSysHostPointer ();

  if (Host->var.mem.socket[Socket].volSize != 0) {
    return TRUE;
  }
  return FALSE;
}

/**

  Get the vol size for a memory controller.

  @param[in] Socket  - Socket to check
  @param[in] McIndex - Memory Controller Index

  @retval Vol size.

**/

UINT32
EFIAPI
GetVolSize (
  IN        UINT8           Socket,
  IN        UINT8           McIndex
  )
{
  SYSHOST *Host;

  Host = GetSysHostPointer ();

  return Host->var.mem.socket[Socket].imc[McIndex].volSize;
}

/**

  Get a SAD Table entry.

  @param[in] Socket   - Socket to check
  @param[in] SadIndex - SAD Index

  @retval SAD Table entry.

**/

SAD_TABLE *
EFIAPI
GetSadTableEntry (
  IN        UINT8           Socket,
  IN        UINT8           SadIndex
  )
{
  SYSHOST *Host;

  Host = GetSysHostPointer ();

  return &Host->var.mem.socket[Socket].SAD[SadIndex];
}

/**

  Get if ECC is enabled.

  @param None.

  @retval TRUE - ECC is enabled.

**/

BOOLEAN
EFIAPI
IsEccEn (
  VOID
  )
{
  SYSHOST *Host;

  Host = GetSysHostPointer ();

  if (Host->nvram.mem.eccEn != 0) {
    return TRUE;
  }
  return FALSE;
}

/**

  Get if Wipe is enabled.

  @param None.

  @retval TRUE - Wipe is enabled.

**/

BOOLEAN
EFIAPI
IsWipeEn (
  VOID
  )
{
  SYSHOST *Host;

  Host = GetSysHostPointer ();

  if (Host->var.mem.wipeMemory != 0) {
    return TRUE;
  }
  return FALSE;
}

/**

  Get if Skip is enabled.

  @param None.

  @retval TRUE - Skip is enabled.

**/

BOOLEAN
EFIAPI
IsSkipEn (
  VOID
  )
{
  SYSHOST *Host;

  Host = GetSysHostPointer ();

  if (Host->var.mem.skipMemoryInit != 0) {
    return TRUE;
  }

  return FALSE;
}

/**

  Get RMT pattern length.

  @param None.

  @retval RMT pattern length.

**/


UINT32
EFIAPI
GetRmtPatternLength (
  VOID
  )
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  return Setup->mem.rmtPatternLength;
}

/**
  This function returns the memory topology configuration for the socket and
  channel specified in the parameters list

  @param[in] Host   - Pointer to sysHost structure on stack
  @param[in] Socket - Socket
  @param[in] Ch     - Channel

  @retval Memory Topology present in the channel:
          DaisyChainTopology
          InvSlotsDaisyChainTopology
          TTopology
**/
EFI_MEMORY_TOPOLOGY_TYPE
EFIAPI
GetMemoryTopologyConfiguration (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  return Setup->mem.MemoryTopology[Socket][Ch];
}

/*
  This function returns the memory channel connector type
  for the channel specified in the parameters list

  @param[in] Host   - Pointer to sysHost structure on stack
  @param[in] Socket - Socket
  @param[in] Ch     - Channel

  @retval Memory connectory type present in the channel:
          DimmConnectorPth        - Through hole connector
          DimmConnectorSmt        - Surface mount connector
          DimmConnectorMemoryDown - Platform soldered DRAMs
**/
EFI_MEMORY_DIMM_CONNECTOR_TYPE
EFIAPI
GetMemoryConnectorType (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  return Setup->mem.MemoryConnectorType[Socket][Ch];
}

/**
  Determines if a mem flow is enabled or disabled

  @param[in]  Flow    Mem flow to check for enable state

  @retval     True    Flow is enabled
  @retval     False   Flow is disabled
**/
BOOLEAN
EFIAPI
IsMemFlowEnabled (
  MEMFLOWS  Flow
  )
{
  PSYSHOST  Host;
  UINT32    *MemFlowField;
  UINT32    FlowBit;

  Host = GetSysHostPointer ();

  FlowBit = 0x1 << (Flow % MEMFLOWS_BLOCK_SIZE);

  if (Flow < MEMFLOWS_BLOCK_SIZE) {

    MemFlowField = &(Host->memFlows);

  } else if (Flow < MEMFLOWS_BLOCK_SIZE * 2) {

    MemFlowField = &(Host->memFlowsExt);

  } else if (Flow < MEMFLOWS_BLOCK_SIZE * 3) {

    MemFlowField = &(Host->memFlowsExt2);

  } else if (Flow < MEMFLOWS_BLOCK_SIZE * 4) {

    MemFlowField = &(Host->memFlowsExt3);

  } else {
    ASSERT (FALSE);
  }

  if ((FlowBit & *MemFlowField) != 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Enables a mem flow

  @param[in]  Flow    Mem flow to enable

  @retval N/A
**/
VOID
EFIAPI
EnableMemFlow (
  MEMFLOWS  Flow
  )
{
  PSYSHOST  Host;
  UINT32    *MemFlowField;
  UINT32    FlowBit;

  Host = GetSysHostPointer ();

  FlowBit = 0x1 << (Flow % MEMFLOWS_BLOCK_SIZE);

  if (Flow < MEMFLOWS_BLOCK_SIZE) {

    MemFlowField = &(Host->memFlows);

  } else if (Flow < MEMFLOWS_BLOCK_SIZE * 2) {

    MemFlowField = &(Host->memFlowsExt);

  } else if (Flow < MEMFLOWS_BLOCK_SIZE * 3) {

    MemFlowField = &(Host->memFlowsExt2);

  } else if (Flow < MEMFLOWS_BLOCK_SIZE * 4) {

    MemFlowField = &(Host->memFlowsExt3);

  } else {
    ASSERT (FALSE);
  }

  *MemFlowField |= FlowBit;
}

/**
Disables a mem flow

  @param[in]  Flow    Mem flow to enable

  @retval N/A
**/
VOID
EFIAPI
DisableMemFlow (
  MEMFLOWS  Flow
  )
{
  PSYSHOST  Host;
  UINT32    *MemFlowField;
  UINT32    FlowBit;

  Host = GetSysHostPointer ();

  FlowBit = 0x1 << (Flow % MEMFLOWS_BLOCK_SIZE);

  if (Flow < MEMFLOWS_BLOCK_SIZE) {

    MemFlowField = &(Host->memFlows);

  } else if (Flow < MEMFLOWS_BLOCK_SIZE * 2) {

    MemFlowField = &(Host->memFlowsExt);

  } else if (Flow < MEMFLOWS_BLOCK_SIZE * 3) {

    MemFlowField = &(Host->memFlowsExt2);

  } else if (Flow < MEMFLOWS_BLOCK_SIZE * 4) {

    MemFlowField = &(Host->memFlowsExt3);

  } else {
    ASSERT (FALSE);
  }

  *MemFlowField &= ~FlowBit;

} // DisableMemFlow

/**
  Enables all mem flows

  @retval N/A
**/
VOID
EFIAPI
EnableAllMemFlows (
  VOID
  )
{
  PSYSHOST  Host;
  Host = GetSysHostPointer ();

  Host->memFlows      = ~(Host->memFlows & 0);
  Host->memFlowsExt   = ~(Host->memFlowsExt & 0);
  Host->memFlowsExt2  = ~(Host->memFlowsExt2 & 0);
  Host->memFlowsExt3  = ~(Host->memFlowsExt3 & 0);

} // EnableAllMemFlows

/**
  Disables all mem flows

  @retval N/A
**/
VOID
EFIAPI
DisableAllMemFlows (
  VOID
  )
{
  PSYSHOST  Host;
  Host = GetSysHostPointer ();

  Host->memFlows = 0;
  Host->memFlowsExt = 0;
  Host->memFlowsExt2 = 0;
  Host->memFlowsExt3 = 0;

} // DisableAllMemFlows

/**

  Check to see if any mem flow is enabled.

  @retval TRUE - some mem flow is enabled.

**/

BOOLEAN
EFIAPI
IsAnyMemFlowEnabled (
  VOID
  )
{
  PSYSHOST  Host;
  Host = GetSysHostPointer ();

  if ((Host->memFlows == 0) &&
      (Host->memFlowsExt == 0) &&
      (Host->memFlowsExt2 == 0) &&
      (Host->memFlowsExt3 == 0)) {
    return FALSE;
  }

  return TRUE;

} // IsAnyMemFlowEnabled

/**
  Check to see if a given socket has a Dimm populated.

  @param[in]  Host    A pointer to the host data.
  @param[in]  Socket  The socket to check.

  @retval TRUE        The socket does have a Dimm populated
  @retval FALSE       The socket does not have a dimm populated, or  the socket is disabled
**/

BOOLEAN
EFIAPI
IsSocketDimmPopulated (
  IN PSYSHOST Host,
  IN UINT8 Socket
  )
{

  ASSERT (Host != NULL);

  if ((Host == NULL) || (Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return FALSE;
  }

  return TRUE;

} // IsSocketDimmPopulated

/**
  Set Syshost pointer to indicate memory needs to be wiped

  retval N/A
**/
VOID
EFIAPI
SetWipeMemory (
  VOID
  )
{
  PSYSHOST Host = GetSysHostPointer ();
  // Indicate we want to wipe memory
  Host->var.mem.wipeMemory = 1;
}

/**
  Return the last known training step running on the socket.

  @param[in] Socket   - The socket number

  @retval The last known training step run on this socket

**/
MRC_TT
EFIAPI
GetCurrentTestType (
  IN  UINT8   Socket
  )
{
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  return Host->var.mem.socket[Socket].CurrentTestType;
}

/**
  Set the current training step for the socket.

  @param[in] Socket   - The socket number
  @param[in] TestType - The test type to set

  @retval N/A

**/
VOID
EFIAPI
SetCurrentTestType (
  IN  UINT8   Socket,
  IN  MRC_TT  TestType
  )
{
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  Host->var.mem.socket[Socket].CurrentTestType = TestType;
}
