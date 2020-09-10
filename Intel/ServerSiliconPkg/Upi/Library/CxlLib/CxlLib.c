/** @file
  This is an implementation of CxlLib library.

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
#include <Library/BaseLib.h>
#include <Library/KtiApi.h>
#include <Library/UpiHostDataLib.h>
#include <IndustryStandard/Cxl.h>
#include <Library/CxlIpLib.h>
#include <Library/CxlLib.h>
#include <Library/IoLib.h>
#include <Library/PeiIioInitLib.h>
#include <Library/KtiSimLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Guid/IioIpInterface.h>
#include <KtiSetupDefinitions.h>
#include <Library/RcrbAccessLib.h>
#include <Library/ChaLib.h>
#include <Library/PcuMailBoxLib.h>

/**
  MmioRead32 wrapper to print assembly code for emulation

  @param Address  - The MMIO register to Read.

  @return Value.
**/
UINT32
EFIAPI
KtiMmioRead32 (
  IN UINTN    Address
  )
{
  if (UbiosGenerationEnabled ()) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  mov eax, DWORD PTR ds:[0%08xh]\n", Address));
  }

  return MmioRead32 (Address);
}

/**
  Routine to get the next CXL port to which a CXL device is attached.

  @param[in]  CurrentSocket       Socket Index of the current CXL port
  @param[in]  CurrentStack        Stack number of the current CXL port
  @param[out] NextSocket          Pointer to the Socket Index of next CXL port with a valid CXL device
  @param[out] NextStack           Pointer to the Stack number of next CXL port with a valid CXL device
  @param[in]  IncludeCurrentPort  Indicates if search from current CXL port specified by CurrentSocket/CurrentStack

  @retval KTI_SUCCESS    - Successfully find a valid CXL device, NextSocket/NextStack are valid.
  @retval KTI_FAILURE    - Not find any valid CXL device, NextSocket/NextStack are undetermined.
**/
KTI_STATUS
EFIAPI
GetNextCxlDevice (
  IN   UINT8                     CurrentSocket,
  IN   UINT8                     CurrentStack,
  OUT  UINT8                     *NextSocket,
  OUT  UINT8                     *NextStack,
  IN   BOOLEAN                   IncludeCurrentPort
  )
{
  UINT8        SocketId, StackId;

  if ((NextSocket == NULL) || (NextStack == NULL)) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "Error: Input NULL pointer!\n"));
    return KTI_FAILURE;
  }

  if (CurrentSocket >= MAX_SOCKET) {
    return KTI_FAILURE;
  }

  SocketId = CurrentSocket;

  if (IncludeCurrentPort) {
    StackId = CurrentStack;
  } else {
    StackId = CurrentStack + 1;
  }

  //
  // For Socket[SocketId], check its other stacks
  //
  for (; StackId < MAX_CXL_PER_SOCKET; StackId++) {
    if (GetCxlStatus (SocketId, StackId) == AlreadyInCxlMode) {
      *NextSocket = SocketId;
      *NextStack = StackId;
      return KTI_SUCCESS;
    }
  }

  //
  // Check other sockets's stacks
  //
  SocketId += 1;
  for (; SocketId < MAX_SOCKET; SocketId++) {
    for (StackId = 0; StackId < MAX_CXL_PER_SOCKET; StackId++) {
      if (GetCxlStatus (SocketId, StackId) == AlreadyInCxlMode) {
        *NextSocket = SocketId;
        *NextStack = StackId;
        return KTI_SUCCESS;
      }
    }
  }

  return KTI_FAILURE;
}

/**
  Routine to check if CXL device attached to the current CXL port(specified by CurrentSocket/CurrentStack)
  is a ganged device that has two CXL links.

  @param[in]  KtiInternalGlobal   Pointer to the KTI RC internal global structure
  @param[in]  CurrentSocket       Socket Index of the current CXL port
  @param[in]  CurrentStack        Stack number of the current CXL port
  @param[out] NextSocket          Pointer to the Socket Index of the 2nd CXL port if it is a ganged CXL device
  @param[out] NextStack           Pointer to the Stack number of the 2nd CXL port if it is a ganged CXL device

  @retval TRUE    - It is a ganged CXL device, 2nd CXL port is returned in NextSocket/NextStack.
  @retval FALSE   - It is not a ganged CXL device, NextSocket/NextStack are undetermined.
**/
BOOLEAN
EFIAPI
IsGangedCxlDevice (
  IN   KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN   UINT8                     CurrentSocket,
  IN   UINT8                     CurrentStack,
  OUT  UINT8                     *NextSocket,
  OUT  UINT8                     *NextStack
  )
{
  KTI_STATUS   KtiStatus;
  UINT8        SocketId, StackId;

  if ((NextSocket == NULL) || (NextStack == NULL)) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "Error: Input NULL pointer!\n"));
    return FALSE;
  }

  SocketId = CurrentSocket;
  StackId = CurrentStack;

  do {
    KtiStatus = GetNextCxlDevice (
                  SocketId,
                  StackId,
                  NextSocket,
                  NextStack,
                  FALSE
                  );

    if (KtiStatus == KTI_SUCCESS) {  //Find another CXL device
      if ((KTIVAR->CxlInfo[CurrentSocket][CurrentStack].VendorId == KTIVAR->CxlInfo[*NextSocket][*NextStack].VendorId) &&
        (KTIVAR->CxlInfo[CurrentSocket][CurrentStack].SerialNum == KTIVAR->CxlInfo[*NextSocket][*NextStack].SerialNum)) {
        //
        // It is a ganged CXL accelerator that has two CXL links
        //
        return TRUE;
      }

      SocketId = *NextSocket;
      StackId = *NextStack;
    }
  } while (KtiStatus == KTI_SUCCESS);

  return FALSE;
}

/**
  This routine will program the Secondary and Subodinate bus registers in CXL DP's
  configuration space so as to enable the PCIe hierarchy behind the CXL DP.

  @param[in]  KtiInternalGlobal - KTIRC Internal Global data
  @param[in]  Socket            - Socket ID
  @param[in]  Stack             - Stack number

  @retval  KTI_SUCCESS
**/
KTI_STATUS
EFIAPI
EnablePcieHierarchyForCxlDp (
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  UINT8                     Socket,
  IN  UINT8                     Stack
  )
{
  UINT8       BusBase;
  UINT32      RcrbBaseAddr;
  EFI_STATUS  Status;

  BusBase = KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].BusBase;

  RcrbBaseAddr = (UINT32) GetRcrbBar (Socket, Stack, TYPE_CXL_RCRB);
  if (RcrbBaseAddr == 0) {
    return KTI_FAILURE;
  }

  //
  // CXL DP's RCRB follows PCIe spec and looks like Type1 configuration space.
  // Program CXL DP Secondary and Subordinate bus to be BusBase + 1.
  //
  Status = SetCxlDownstreamBus ((UINTN) RcrbBaseAddr, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, BusBase + 1);
  Status |= SetCxlDownstreamBus ((UINTN) RcrbBaseAddr, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, BusBase + 1);

  return EFI_ERROR (Status)?KTI_FAILURE:KTI_SUCCESS;
}

/**
  Routine to program the CLX RCRB BAR for the specified CLX enabled stack
  This function performs 3 different operations.
  1. Programs RCRB register
  2. Programs Secondary and Subordinate Bus numbers for DP
  3. Indirectly programs UP RCRB by issuing memory access operation.
     Refer CXL 1.1 Rev. 1.1 Section 7.2.1.2. "the upstream port captures the
     upper address bits[63:12] of the first memory access received after link
     training as the base address for the upstream port RCRB"

  @param[in]  KtiInternalGlobal   Pointer to the KTI RC internal global structure
  @param[in]  Socket              Socket Index
  @param[in]  Stack               Stack number

  @retval KTI_SUCCESS
**/
KTI_STATUS
EFIAPI
ProgramCXLRcrbBar (
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  UINT8                     Socket,
  IN  UINT8                     Stack
  )
{
  UINT32     CxlDpRcrbBar;
  EFI_STATUS KtiStatus;
  UINT32     Data;

  CxlDpRcrbBar = KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolBase;
  KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].RcrbBase = CxlDpRcrbBar;

  //
  // Set the RCRBBAR with the first 8KB mmiol within the stack
  //
  SetRcrbBar (Socket, Stack, TYPE_CXL_RCRB, CxlDpRcrbBar);

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nCXL: Program Secondary and Subordinate bus for DP.\n"));
  KtiStatus = EnablePcieHierarchyForCxlDp (KtiInternalGlobal, Socket, Stack);
  if (KtiStatus != KTI_SUCCESS) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n\nProgramming Downstream Port Bus registers failed!!\n\n"));
    return KTI_FAILURE;
  }

  if (GetCxlStatus (Socket, Stack) == AlreadyInCxlMode) {  //Only needed when CXL port enabled
    //
    // Set up the CXL UP RCRB BAR by trigger a read to the CXL UP RCRB BAR
    // HW will help to set the BAR for CXL UP. Use MmioRead for now.
    //
    Data = KtiMmioRead32 (CxlDpRcrbBar + CXL_RCRB_BAR_SIZE_PER_PORT);
  }

  return KTI_SUCCESS;
}

/**
  Routine to allocate MMIO resource and program the MEMBAR0 register for CXL DP and UP RCRB.

  ---------------------  ==> Base = KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolBase
  |    DP RCRB(4KB)   |
  ---------------------  ==> Base = DP RCRB Base + 4KB
  |    UP RCRB(4KB)   |
  ---------------------  ==> Reserved to meet MEMBAR0 alignment
  |     Reserved      |
  |      xxxKB        |
  ---------------------  ==> DP MEMBAR0 Base
  |    DP MEMBAR0     |
  |      128KB        |
  ---------------------  ==> Base = DP MEMBAR0 Base + 128KB
  |    UP MEMBAR0     |
  |      128KB        |
  ---------------------

  @param[in]  KtiInternalGlobal   Pointer to the KTI RC internal global structure
  @param[in]  Socket              Socket Index
  @param[in]  Stack               Stack number

  @retval KTI_SUCCESS             - Function completed successfully
  @retval KTI_FAILURE             - Function failed

**/
KTI_STATUS
EFIAPI
AllocateMmioForCxlPort (
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  UINT8                     Socket,
  IN  UINT8                     Stack
  )
{
  EFI_STATUS Status;
  UINT32     DpRcrbBar, MemResBase;
  UINT64     MemBar0Base, MemBar0Size;

  DpRcrbBar = (UINT32) GetRcrbBar (Socket, Stack, TYPE_CXL_RCRB);
  if (DpRcrbBar == 0) {
    return KTI_FAILURE;
  }

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  Program MEMBAR0 for CXL Downstream port\n"));
  MemResBase = DpRcrbBar + CXL_RCRB_BAR_SIZE;
  Status = ProgramMemBar0Register (DpRcrbBar,
                                   CXL_PORT_RCRB_MEMBAR0_LOW_OFFSET,
                                   MemResBase,
                                   &MemBar0Base,
                                   &MemBar0Size
                                  );

  //
  // Enable PCICMD.mse for CXL DP port
  //
  ProgramPciCommandRegister (Socket, DpRcrbBar, TRUE);

  if (GetCxlStatus (Socket, Stack) == AlreadyInCxlMode) {  //Only needed when CXL port enabled
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  Program MEMBAR0 for CXL Upstream port\n"));
    MemResBase = (UINT32) (MemBar0Base + MemBar0Size);
    Status |= ProgramMemBar0Register (DpRcrbBar + CXL_RCRB_BAR_SIZE_PER_PORT,
                                      CXL_PORT_RCRB_MEMBAR0_LOW_OFFSET,
                                      MemResBase,
                                      &MemBar0Base,
                                      &MemBar0Size
                                     );

    //
    // Enable PCICMD.mse for CXL UP port
    //
    ProgramPciCommandRegister (Socket, DpRcrbBar + CXL_RCRB_BAR_SIZE_PER_PORT, TRUE);
  }

  return EFI_ERROR (Status)?KTI_FAILURE:KTI_SUCCESS;
}

/**
  Routine to enumerate the CXL devices under the specified CXL DP & UP port.

  @param[in]  KtiInternalGlobal   Pointer to the KTI RC internal global structure
  @param[in]  Socket              Socket Index
  @param[in]  Stack               Stack number

  @retval KTI_SUCCESS
**/
KTI_STATUS
EFIAPI
EnumerateCxlDevice (
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  UINT8                     Socket,
  IN  UINT8                     Stack
  )
{
  CXL_DVSEC_FLEX_BUS_DEVICE_CAPABILITY  CxlDevCap;
  EFI_STATUS                            Status;

  if (GetCxlStatus (Socket, Stack) != AlreadyInCxlMode) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "The Stack is not enabled as CXL!\n"));
    return KTI_FAILURE;
  }
  //
  // Check if the CXL endpoint device is existed
  //
  KTIVAR->CxlInfo[Socket][Stack].VendorId = GetCxlDevVid (Socket, Stack);
  if (KTIVAR->CxlInfo[Socket][Stack].VendorId == 0xFFFF) {
    SetCxlStatus (Socket, Stack, NotInCxlMode);
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "No CXL endpoint device is attached! Disable the CXL PORT\n"));
    return KTI_FAILURE;
  }

  KTIVAR->CxlInfo[Socket][Stack].SerialNum = GetCxlDevSerialNum (Socket, Stack);

  Status = CxlDeviceDvsecRegisterAccess (
              Socket,
              Stack,
              OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_DEVICE, DeviceCapability),
              UsraWidth16,
              CxlOperationRead,
              &CxlDevCap.Uint16
              );
  if ((CxlDevCap.Uint16 == 0) || (Status != EFI_SUCCESS)) {
    SetCxlStatus (Socket, Stack, NotInCxlMode);
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1,
                         "Can't find DVSEC structure's Flex Bus Capability on the Cxl endpoint device! Disable the CXL Port\n"));
    return KTI_FAILURE;
  }

  if (CxlDevCap.Bits.HdmCount && (!CxlDevCap.Bits.MemCapable)) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "CXL Capabilty Structure's HDM_Count is illegal!\n"));
  } else {
    KTIVAR->CxlInfo[Socket][Stack].Hdm_Count = (UINT8) CxlDevCap.Bits.HdmCount;
  }

  KTIVAR->CxlInfo[Socket][Stack].Cache_Capable    = (UINT8) CxlDevCap.Bits.CacheCapable;
  KTIVAR->CxlInfo[Socket][Stack].Mem_Capable      = (UINT8) CxlDevCap.Bits.MemCapable;
  KTIVAR->CxlInfo[Socket][Stack].Mem_HwInit_Mode  = (UINT8) CxlDevCap.Bits.MemHwInitMode;
  KTIVAR->CxlInfo[Socket][Stack].Viral_Capable    = (UINT8) CxlDevCap.Bits.ViralCapable;

  return KTI_SUCCESS;
}

/**
  Routine to detect the CXL topology.
  Note:
  This routine must be invoked after CXL device enumeration.

  @param[in]  KtiInternalGlobal   Pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS
**/
KTI_STATUS
EFIAPI
DetectCxlTopology (
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  UINT8         CurrentSocket, CurrentStack;
  UINT8         NextSocket, NextStack;
  KTI_STATUS    KtiStatus;
  BOOLEAN       IsGangedDevice;

  //
  // Try to find the first valid CXL device if any
  //
  KtiStatus = GetNextCxlDevice (
                0,
                0,
                &NextSocket,
                &NextStack,
                TRUE
                );

  if (KtiStatus == KTI_FAILURE) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nNot find any valid CXL device!\n"));
    return KTI_SUCCESS;
  }

  while (KtiStatus == KTI_SUCCESS) {
    CurrentSocket = NextSocket;
    CurrentStack = NextStack;

    if (KTIVAR->CxlInfo[CurrentSocket][CurrentStack].CxlTopologyType == CXL_TOPOLOGY_UNDETERMINED) {
      //
      // Detected topology type for current CXL device if its topology type is undetermine
      //
      IsGangedDevice = IsGangedCxlDevice (
                         KtiInternalGlobal,
                         CurrentSocket,
                         CurrentStack,
                         &NextSocket,
                         &NextStack
                         );
      if (IsGangedDevice) {
        //
        // This accelerator has two CXL links
        //
        if (CurrentSocket == NextSocket) {
          KTIVAR->CxlInfo[CurrentSocket][CurrentStack].CxlTopologyType = TWO_CXL_LINK_ONE_SOCKET;
          KTIVAR->CxlInfo[NextSocket][NextStack].CxlTopologyType = TWO_CXL_LINK_ONE_SOCKET;
        } else {
          KTIVAR->CxlInfo[CurrentSocket][CurrentStack].CxlTopologyType = TWO_CXL_LINK_TWO_SOCKET;
          KTIVAR->CxlInfo[NextSocket][NextStack].CxlTopologyType = TWO_CXL_LINK_TWO_SOCKET;
        }

        KTIVAR->CxlInfo[CurrentSocket][CurrentStack].SecondPortSocket = NextSocket;
        KTIVAR->CxlInfo[CurrentSocket][CurrentStack].SecondPortStack = NextStack;

        KTIVAR->CxlInfo[NextSocket][NextStack].SecondPortSocket = CurrentSocket;
        KTIVAR->CxlInfo[NextSocket][NextStack].SecondPortStack = CurrentStack;
      } else {
        //
        // This accelerator has only one CXL link
        //
        KTIVAR->CxlInfo[CurrentSocket][CurrentStack].CxlTopologyType = ONE_CXL_LINK_ONE_SOCKET;
        KTIVAR->CxlInfo[CurrentSocket][CurrentStack].SecondPortSocket = CurrentSocket;  //Invalid field, make it point to itself
        KTIVAR->CxlInfo[CurrentSocket][CurrentStack].SecondPortStack = CurrentStack;  //Invalid field, make it point to itself
      }
    }

    KtiStatus = GetNextCxlDevice (
                  CurrentSocket,
                  CurrentStack,
                  &NextSocket,
                  &NextStack,
                  FALSE
                  );
  }

  return KTI_SUCCESS;
}

/**
  This function will enable the Cxl.$ if the CXL device is cache capable.

  @param[in]     Socket                Device's socket number.
  @param[in]     Stack                 Box Instane, 0 based.

  @retval        KTI_SUCCESS           Successfully enabled Cxl.$.
  @retval        KTI_FAILURE           Failed to enable Cxl.$.
**/
KTI_STATUS
EFIAPI
EnableCxlCache (
  IN       UINT8              Socket,
  IN       UINT8              Stack
  )
{
  EFI_STATUS                            Status;
  CXL_DVSEC_FLEX_BUS_DEVICE_CAPABILITY  CxlDevCapReg;
  CXL_DVSEC_FLEX_BUS_DEVICE_CONTROL     CxlDevCtlRegSetMask,    // both these locals
                                        CxlDevCtlRegClearMask;  // are for same purpose
  CXL_1_1_DVSEC_FLEX_BUS_PORT_CONTROL   CxlPortControl;

  //
  //Step 1: Check if the CXL(IAL) device is Cache capable
  //
  Status = CxlDeviceDvsecRegisterAccess (
                Socket,
                Stack,
                OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_DEVICE, DeviceCapability),
                UsraWidth16,
                CxlOperationRead,
                (VOID *) &CxlDevCapReg.Uint16
                );
  if (Status != EFI_SUCCESS) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nFailed to read the CXL device's Capability register!\n"));
    return KTI_FAILURE;
  }

  if (CxlDevCapReg.Bits.CacheCapable == 0) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nCXL.Cache is not supported by this device!\n"));
    return KTI_FAILURE;
  }

  //
  //Step 2: Set the Cache_Enable for CXL DP, UP and CXL device if CXL device is Cache capable
  //
  CxlPortControl.Uint16 = 0;

  //Step 2.1: Set the Cache_Enable for CXL DP
  CxlPortControl.Bits.CacheEnable = 1;
  Status = CxlPortDvsecRegisterAndThenOr16 (
              Socket,
              Stack,
              FALSE,
              OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_PORT, PortControl),
              ~CxlPortControl.Uint16,
              CxlPortControl.Uint16
              );
  if (Status != EFI_SUCCESS) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nFailed to set Control.Cache_Enable for CXL downstream port!\n"));
    return KTI_FAILURE;
  }

  //Step 2.2: Set the Cache_Enable for CXL UP
  Status = CxlPortDvsecRegisterAndThenOr16 (
              Socket,
              Stack,
              TRUE,
              OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_PORT, PortControl),
              ~CxlPortControl.Uint16,
              CxlPortControl.Uint16
              );
  if (Status != EFI_SUCCESS) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nFailed to set Control.Cache_Enable for CXL upstream port!\n"));
    return KTI_FAILURE;
  }

  //Step 2.3: Set the Cache_Enable for CXL Device
  CxlDevCtlRegSetMask.Uint16 = 0;         // init w/ 0 for the fields to be set
  CxlDevCtlRegSetMask.Bits.CacheEnable = 1;
  //
  //Step 3. Set the Cache_SF_Coverage, Cache_SF_Granularity and Cache_Clean_Eviction on the CXL device side.
  //
  // ToDo: Need to get the detailed settings for below fields
  //
  CxlDevCtlRegClearMask.Uint16 = 0xFFFF;  // init w/ 1 for the fields to be cleared
  CxlDevCtlRegClearMask.Bits.CacheSfCoverage = 0;
  CxlDevCtlRegClearMask.Bits.CacheSfGranularity = 0;
  CxlDevCtlRegClearMask.Bits.CacheCleanEviction = 0;
  //
  Status = CxlDeviceDvsecRegisterAndThenOr16 (
              Socket,
              Stack,
              OFFSET_OF (CXL_1_1_DVSEC_FLEX_BUS_DEVICE, DeviceControl),
              CxlDevCtlRegSetMask.Uint16
              ^ CxlDevCtlRegClearMask.Uint16,  //XOR set & clear masks value for AND mask
              CxlDevCtlRegSetMask.Uint16
              );

  if (Status != EFI_SUCCESS) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nFailed to config Control register for CXL Device!\n"));
    return KTI_FAILURE;
  }

  //
  //Step 4. Configure host side CHA to indicate the corresponding mesh stop is connected to an IDI agent if needed.
  //
  // Q: Which CHA registers? Which bit field? Haven't found it in 0p8 crif.
  // ToDo: Need to get the detaild register settings for CHA.

  return KTI_SUCCESS;
}

/**
  This routine will configurate the registers for CXL CM IP.

  @param[in]  KtiInternalGlobal  - Pointer to the KTI RC internal global structure
  @param[in]  Socket             - Socket number
  @param[in]  Stack              - Stack number

  @retval     KTI_SUCCESS        - Routine completed successfully
  @retval     KTI_FAILURE        - Routine failed

**/
KTI_STATUS
EFIAPI
ConfigCxlCacheMemIpRegs (
  IN  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN  UINT8                      Socket,
  IN  UINT8                      Stack
  )
{
  UINT8         ResetPrepSxAutoAck;
  UINT32        DpRcrbBar;
  KTI_STATUS    KtiStatus;

  ResetPrepSxAutoAck = 0;

  //
  // Note:
  // Per IIO team's comment, a stack will be trained to be CXL port as long as there is CXL device attached.
  // Hence, if a stack is not CXL port, there must be no CXL device attached.
  //
  if (GetCxlStatus (Socket, Stack) == NotInCxlMode) {
    //
    // Program the RCRBBar for CXL DP port
    //
    ProgramCXLRcrbBar (KtiInternalGlobal, Socket, Stack);

    //
    // Allocate temporary MMIO resource for the registers of CXL DP subsystem component
    //
    KtiStatus = AllocateMmioForCxlPort (KtiInternalGlobal, Socket, Stack);
    if (KtiStatus != KTI_SUCCESS) {
      return KTI_FAILURE;
    }

    //
    // Below programming is only needed when in all protocol enabled mode.
    //
    if (PI5_IP_DISABLE_CONFIG_MUTI_PROTOCOL_VALUE == GetPi5ProtocolStatus (Socket, Stack)) {

      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nCXL: Socket=%X, Stack=%X - In all protocol mode and no CXL device detected.\n", Socket, Stack));

      ResetPrepSxAutoAck = 1;
    }

    //
    // Program CLK_GATE_CTRL register.
    //
    ProgramCxlCmClkGateCtrlReg (Socket, Stack, ResetPrepSxAutoAck);

    //
    // Clear the MEMBAR0 register of CXL DP RCRB region
    //
    DpRcrbBar = (UINT32) GetRcrbBar (Socket, Stack, TYPE_CXL_RCRB);
    if (DpRcrbBar != 0) {
      //
      // Clear the PCICMD.mse bit first
      //
      ProgramPciCommandRegister (Socket, DpRcrbBar, FALSE);
      ClearMmioForCxlPort (DpRcrbBar, CXL_PORT_RCRB_MEMBAR0_LOW_OFFSET);
    }

    //
    // Clear the RCRBBAR register of CXL DP port
    //
    ClearRcrbBar (Socket, Stack, TYPE_CXL_RCRB);
  }

  return KTI_SUCCESS;
}

/**
  Routine to initialize the Compute Express Link(CXL).

  @param[in]  SocketData         - Pointer to socket specific data
  @param[in]  KtiInternalGlobal  - Pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS
**/
KTI_STATUS
EFIAPI
InitializeComputeExpressLink (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8         SocketId, StackId;
  BOOLEAN       CxlDevPresent;
  KTI_STATUS    KtiStatus;
  EFI_STATUS    Status;
  UINT8         CxlSecLvl;
  UINT8         CxlStcge, CxlSdcge, CxlDlcge, CxlLtcge;

#ifdef IA32
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nCXL: IIO EarlyLink Init Start.\n"));
  IioEarlyLinkInit ();

  KTIVAR->IioEarlyLinkInitDone = TRUE;
#endif //IA32

  CxlDevPresent = FALSE;
  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {


    if (SocketData->Cpu[SocketId].Valid == FALSE) {
      continue;
    }
    if (SocketData->Cpu[SocketId].SocType != SOCKET_TYPE_CPU) {
      continue;
    }

    for (StackId = 0; StackId < MAX_CXL_PER_SOCKET; StackId++) {
      if (!IfStackPresent (SocketId, StackId)) {
        continue;
      }


      if (KTIVAR->CxlInfo[SocketId][StackId].CxlStatus != AlreadyInCxlMode) {
        if ((SocketId == GetSysSbspSocketIdNv ()) && (StackId == 0)) {  //DMI port
          continue;
        }

        if (KTIVAR->CxlInfo[SocketId][StackId].CxlStatus == NotInCxlMode) {
          ConfigCxlCacheMemIpRegs (KtiInternalGlobal, SocketId, StackId);
        }

        continue;
      }

      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n\nCXL: Port location: Socket=%X, Stack=%X\n", SocketId, StackId));

      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nCXL: Program the RCRB Bar.\n"));
      KtiStatus = ProgramCXLRcrbBar (KtiInternalGlobal, SocketId, StackId);

      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nCXL: Enumerate CXL devices.\n"));
      KtiStatus = EnumerateCxlDevice (KtiInternalGlobal, SocketId, StackId);
      if (KtiStatus == KTI_SUCCESS) {
        CxlDevPresent = TRUE;

        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nCXL: Allocate MMIO resource for CXL DP and UP.\n"));
        KtiStatus = AllocateMmioForCxlPort (KtiInternalGlobal, SocketId, StackId);
        if (KtiStatus != KTI_SUCCESS) {
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n\nProgramming MEMBAR0 failed!!\n\n"));
        }

        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nCXL: Collect and clear Error Status for RAS.\n"));
        Status = CxlCollectAndClearErrors (SocketId, StackId);
        if (Status != EFI_SUCCESS) {
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n\nFailed to collect and clear Error Status for RAS.\n\n"));
        }

        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nCXL: Configure Security Policy.\n"));
        CxlSecLvl = CXL_SECURITY_FULLY_TRUSTED;
        if (KTISETUP->DfxParm.DfxCxlSecLvl < CXL_SECURITY_AUTO) {
          CxlSecLvl = KTISETUP->DfxParm.DfxCxlSecLvl;
        }
        Status = CxlConfigureSecurityPolicy (SocketId, StackId, CxlSecLvl);
        if (Status != EFI_SUCCESS) {
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n\nFailed to configure CXL security policy!!\n\n"));
        }

        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nCXL: Enable CXL.$ if capable.\n"));
        KtiStatus = EnableCxlCache (SocketId, StackId);
        if (KtiStatus == KTI_SUCCESS) {
          KTIVAR->CxlInfo[SocketId][StackId].Cache_Enabled = 1;
        }

        ConfigureLinkPowerManagement (SocketId, StackId);

        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nCXL: Configure the CXL.Arb-Mux Clock Gating Enable bits\n"));
        CxlStcge = CxlSdcge = CxlDlcge = CxlLtcge = KTI_ENABLE;
        if (KTISETUP->DfxParm.DfxCxlStcge < KTI_AUTO) {
          CxlStcge = KTISETUP->DfxParm.DfxCxlStcge;
        }
        if (KTISETUP->DfxParm.DfxCxlSdcge < KTI_AUTO) {
          CxlSdcge = KTISETUP->DfxParm.DfxCxlSdcge;
        }
        if (KTISETUP->DfxParm.DfxCxlDlcge < KTI_AUTO) {
          CxlDlcge = KTISETUP->DfxParm.DfxCxlDlcge;
        }
        if (KTISETUP->DfxParm.DfxCxlLtcge < KTI_AUTO) {
          CxlLtcge = KTISETUP->DfxParm.DfxCxlLtcge;
        }
        ConfigureCxlArbMuxCge (SocketId, StackId, CxlStcge, CxlSdcge, CxlDlcge, CxlLtcge);

        //
        // Silicon Workaround
        //
        WorkaroundWithCxlEpDeviceInstalled (SocketId, KtiInternalGlobal->TotCha[SocketId]);
      }
    }
  }

  if (CxlDevPresent) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nCXL: Detect topology.\n"));
    DetectCxlTopology (KtiInternalGlobal);
  }

  return KTI_SUCCESS;
}

/**
  Program SNC registers on CXL

  @param[in] KtiInternalGlobal - KTIRC Internal Global data
  @param[in] SocketId          - Socket

  @retval KTI_SUCCESS
**/
KTI_STATUS
EFIAPI
ProgramSncRegistersOnCxl (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocketId
  )
{
  UINT8        StackId;
  UINT32       Data32;
  UINT8        SncBaseIndex;
  UINT8        NumChaPerCluster;
  UINT8        BaseChaCluster1;
  UINT8        BaseChaCluster2;
  UINT8        BaseChaCluster3;
  UINT8        UmaClusterEn;
  UINT8        XorDefeatur;
  UINT32       TwoLmMask;
  UINT8        ChaCount;

  ChaCount = GetTotChaCount (SocketId);
  for (StackId = 0; StackId < MAX_CXL_PER_SOCKET; StackId++) {
    if ((GetCxlStatus (SocketId, StackId) == AlreadyInCxlMode) &&
        (KTIVAR->CxlInfo[SocketId][StackId].Cache_Enabled == 1)) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "Program CXL SNC registers for socket: %d, stack: %d\n", SocketId, StackId));

      for (SncBaseIndex = 0; SncBaseIndex < 5; SncBaseIndex++) {
        //
        // First read from IIO register, then write to CXL register
        //
        IioGetSncIdx (SocketId, 0, SncBaseIndex, &Data32);
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "CXL SNC Base Address %d: 0x%08X \n", (SncBaseIndex + 1), Data32));
        CxlSetSncBaseAddr (SocketId, StackId, SncBaseIndex, Data32);
      }

      IioGetSncUpperBase (SocketId, 0, &Data32);
      CxlSetSncUpperBase (SocketId, StackId, Data32);
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "CXL SNC Upper Base Address: 0x%08X \n", Data32));

      Data32 = IioGetSncCfg (SocketId, 0);
      CxlSetSncCfg (SocketId, StackId, Data32);
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "CXL_SNC_CONFIG: 0x%08X \n", Data32));

      IioGetSncUncoreCfg (SocketId, 0, &NumChaPerCluster, &BaseChaCluster1, &BaseChaCluster2, &BaseChaCluster3);
      CxlSetSncUncoreCfg (SocketId, StackId, NumChaPerCluster, BaseChaCluster1, BaseChaCluster2, BaseChaCluster3);
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "CXL_UNCORE_SNC_CONFIG: NumChaPerCluster:%d, BaseChaCluster1:%d, BaseChaCluster2:%d, BaseChaCluster3:%d\n", NumChaPerCluster, BaseChaCluster1, BaseChaCluster2, BaseChaCluster3));

      IioGetUmaClusterCfg (SocketId, 0, &UmaClusterEn, &XorDefeatur);
      CxlSetUmaClusterCfg (SocketId, StackId, UmaClusterEn, XorDefeatur);
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "CXL_UMA_CLUSTER_CFG: UmaClusterEn: %d, XorDefeatur: %d\n", UmaClusterEn, XorDefeatur));

      IioGetSnc2LmAddrMask(SocketId , 0, &TwoLmMask);
      CxlSetHashAddrMask (SocketId, StackId, TwoLmMask, ChaCount);
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "CXL_HASHADDRMASK: TwoLmMask: %d, ChaCount: %d\n", TwoLmMask, ChaCount));
    }
  }

  return KTI_SUCCESS;
}
