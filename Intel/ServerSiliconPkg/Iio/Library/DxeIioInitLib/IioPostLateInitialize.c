/** @file
  Implement IIO initialization protocol to provide IIO initialization functions.
  This interfaces will be called by PciHostBridge driver in the PCI bus enumeration.

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

#include <Base.h>
#include <Uefi.h>
#include <Guid/SocketPciResourceData.h>
#include <Library/DebugLib.h>
#include <IioPlatformData.h>
#include <Library/OemIioInit.h>
#include <Library/OobIpLib.h>
#include <Library/PrintLib.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <Library/IioInitLib.h>
#include <Library/IioInitDxeLib.h>
#include <Library/IioPcieInit.h>
#include <Library/IioNtbInit.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PcieCommonInitLib.h>
#include <Library/IioDmiLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Protocol/IioSystem.h>
#include <Protocol/PciCallback.h>
#include <IioSetupDefinitions.h>
#include <IioUniversalData.h>
#include <RcRegs.h>
#include <Library/UefiLib.h>
#include <Library/IioIoatLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/IioSysInfoLib.h>
#include <Library/IioSecurePlatformLib.h>
#include <Private/Library/DxeTraceHubLib.h>
#include <PcieRegs.h>
#include <Guid/IioIpInterface.h>
#include <Library/IioMs2iosfInit.h>
#include <Library/IioIoApicInit.h>
#include <Library/SystemInfoLib.h>
#include <Library/IioSpkInit.h>
#include <LibraryPrivate/IioPcieLib.h>
#include <Library/PciMmio32AssignLib.h>
#include <Library/EmulationConfigurationLib.h>

#include <LibraryPrivate/IioDebug.h>

#include "IioDevHide.h"
#include <Library/CheckpointLib.h>


/******************************************************************************
 * Local definitions.
 ******************************************************************************/
#ifndef DEVCTL_IIO_PCIEDMI_REG
//
// In ICX header this reg is called DEVCTRL, in others DEVCTL.
//
#define DEVCTL_IIO_PCIEDMI_REG DEVCTRL_IIO_PCIEDMI_REG
#endif
//
// This structure is a buffer to build PCIe topology tree for PcieCommonInitLib.
// Every stack and port list ends with entry of special type, so size must be +1.
//
typedef struct {
  PCIE_LIB_TOPOLOGY_INF          IioTopology;
  PCIE_LIB_HOST_BRIDGE_INF       IioHostStack[MAX_SOCKET * MAX_IIO_STACK + 1];
  PCIE_LIB_ROOT_PORT_INF         IioRootPort[MAX_SOCKET * MAX_IIO_STACK][MAX_IIO_PORTS_PER_STACK + 1];
  PCIE_LIB_ROOTPORT_FEATURE_NODE IioRootPortFeature[MAX_SOCKET * MAX_IIO_STACK][MAX_IIO_PORTS_PER_STACK][PCIE_LIB_FEATURE_MAX];
} PCIE_LIB_TOPOLOGY_BUFFER;


/******************************************************************************
 * Variables.
 ******************************************************************************/
//
// Capabilities to set for DMI port.
//
PCIE_LIB_ROOTPORT_FEATURE_NODE DmiFeaturesTemplate[] =
  {
    {PCIE_LIB_FEATURE_MAX}
  };
//
// Capabilities to set for PCI port.
//
PCIE_LIB_ROOTPORT_FEATURE_NODE PciFeaturesTemplate[] =
  {
    {PCIE_LIB_FEATURE_COMMONCLOCK}, // Common clock must be set before ASPM
    {PCIE_LIB_FEATURE_ASPM},        // becaue it may influence latency for ASPM.
    {PCIE_LIB_FEATURE_CTO},
    {PCIE_LIB_FEATURE_MAXPAYLOAD},
    {PCIE_LIB_FEATURE_EXTTAGFIELD},
    {PCIE_LIB_FEATURE_ATOMICOP},
    {PCIE_LIB_FEATURE_LTR},
    {PCIE_LIB_FEATURE_MRRS},
    {PCIE_LIB_FEATURE_RELAXORDER},
    {PCIE_LIB_FEATURE_PTM},
    {PCIE_LIB_FEATURE_10BIT_TAG},
    {PCIE_LIB_FEATURE_MAX}
  };
struct {
  PCIE_LIB_ROOTPORT_FEATURE_NODE *FeatureTable;
  UINT32                          FeatureTableSize; // Size of FeatureTable table in bytes
} FeaturesTmpl[PCIE_LIB_PORT_TYPE_MAX] =
  {
    {&DmiFeaturesTemplate[0], sizeof(DmiFeaturesTemplate)},
    {&PciFeaturesTemplate[0], sizeof(PciFeaturesTemplate)}
  };


/******************************************************************************
 * Functions.
 ******************************************************************************/
/**
  This function initializes input for PcieInfoTreePaser() function in PcieCommonInitLib
  as a structure created at IioGlobalData->IioVar.IioOutData.PtrPcieTopology.
  Then the library initializes topology dependend PCIe capabilities.
  This function shall not fail in normal flow. Failures are captured by ASSERT.
  If failure happens the PCie capabilities would not initialize in some root ports.

  @param[in,out] IioGlobalData - Pointer to IIO global data structure.
**/
VOID
InitLibParameters (
  IN OUT IIO_GLOBALS             *IioGlobalData
  )
{
  EFI_STATUS                      Status;
  UINT8                           MaxPortPerSocket;
  UINT8                           MaxStackPort;
  UINT8                           IioIndex;
  UINT8                           PortIndex;
  UINT8                           Stack;
  UINT8                           Port;

  PCIE_LIB_TOPOLOGY_BUFFER       *TopologyPtr;
  PCIE_LIB_HOST_BRIDGE_INF       *HostBridgeInfoPtr;
  PCIE_LIB_ROOT_PORT_INF         *RootPortInfoPtr;
  PCIE_LIB_ROOTPORT_FEATURE_NODE *FeaturePtr;

  Status = gBS->AllocatePool (EfiBootServicesData, sizeof(*TopologyPtr), (VOID**)&TopologyPtr);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    IioGlobalData->IioVar.IioOutData.PtrPcieTopology.Address64bit = 0;
    return;
  }
  IioGlobalData->IioVar.IioOutData.PtrPcieTopology.Address64bit = (UINT64)TopologyPtr;
  //
  // Init Hostbridge information
  //
  ZeroMem (TopologyPtr, sizeof(*TopologyPtr));
  TopologyPtr->IioTopology.Revision = PCIE_LIB_TOPOLOGY_REVISION_1;
  TopologyPtr->IioTopology.HostBridgeList = &TopologyPtr->IioHostStack[0];
  HostBridgeInfoPtr = TopologyPtr->IioTopology.HostBridgeList;
  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {

    if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
      continue;
    }
    MaxPortPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
    for (Stack = 0; Stack < MAX_IIO_STACK; HostBridgeInfoPtr++, Stack++) {

      AsciiSPrint (&HostBridgeInfoPtr->Name[0], sizeof(HostBridgeInfoPtr->Name), "IIO[%X.%X]", IioIndex, Stack);
      HostBridgeInfoPtr->Revision = PCIE_LIB_HOSTBRIDGE_REVISION_SKIP;
      HostBridgeInfoPtr->RootPortList = &TopologyPtr->IioRootPort[IioIndex * MAX_IIO_STACK + Stack][0];
      if (!IioIsStackPresent (IioIndex, Stack)) {
        continue;
      }
      //
      // Verify if the stack has PCIe Root Ports implemented.
      // This check allows DMI stack so the PcieCommonInitLib goes over PCH root ports under DMI stack.
      //
      if (!IsPcieRootPortStack (IioGlobalData, IioIndex, Stack)) {

        IIO_D_PCIDBG ("[%d.%d] Skip non root port stack\n", IioIndex, Stack);
        continue;
      }

      //
      // Continue if the stack has not assigned PCI buses
      //
      if (IioGlobalData->IioVar.IioVData.SocketStackBaseBusNumber[IioIndex][Stack] >
                                           IioGlobalData->IioVar.IioVData.SocketStackLimitBusNumber[IioIndex][Stack]) {
        continue;
      }
      HostBridgeInfoPtr->Revision = PCIE_LIB_HOSTBRIDGE_REVISION_1;
      HostBridgeInfoPtr->BusUsedNum = 1; // At least one bus is used for rootport device
      HostBridgeInfoPtr->BusBase = IioGlobalData->IioVar.IioVData.SocketStackBaseBusNumber[IioIndex][Stack];
      HostBridgeInfoPtr->BusLimit = IioGlobalData->IioVar.IioVData.SocketStackLimitBusNumber[IioIndex][Stack];
      HostBridgeInfoPtr->Attribute = 0;
      IIO_D_PCILOG ("[%d.%d] Host bridge '%a' bus range: %02X..%02X\n",
                    IioIndex, Stack, HostBridgeInfoPtr->Name, HostBridgeInfoPtr->BusBase, HostBridgeInfoPtr->BusLimit);
      //
      // Init Root port Information
      //
      MaxStackPort = GetMaxPortNumPerStack (Stack);
      for (RootPortInfoPtr = HostBridgeInfoPtr->RootPortList,
           Port = 0; Port < MaxStackPort; RootPortInfoPtr++, Port++) {

        AsciiSPrint (&RootPortInfoPtr->Name[0], sizeof(RootPortInfoPtr->Name), "IIO[%X.%X.%X]", IioIndex, Stack, Port);
        RootPortInfoPtr->Valid = FALSE;
        //
        // Check if port is configured as VMD, if so we need to skip the pci scan process
        //
        PortIndex = GetPortIndexbyStack (Stack, Port);
        if (IsVMDEnabledForPort (IioIndex, PortIndex)) {
          continue;
        }
        RootPortInfoPtr->Valid = TRUE;
        RootPortInfoPtr->Type = IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPortActive) ? PCIE_LIB_DMI : PCIE_LIB_PCIE;
        RootPortInfoPtr->Segment = IioGlobalData->IioVar.IioVData.SegmentSocket[IioIndex];
        RootPortInfoPtr->Bus = IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex][PortIndex];
        RootPortInfoPtr->Dev = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Device;
        RootPortInfoPtr->Fun = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Function;
        RootPortInfoPtr->Attribute = 0;
        RootPortInfoPtr->NestedLevel = 0;
        RootPortInfoPtr->FeatureList = &TopologyPtr->IioRootPortFeature[IioIndex * MAX_IIO_STACK + Stack][Port][0];
        CopyMem (RootPortInfoPtr->FeatureList, FeaturesTmpl[RootPortInfoPtr->Type].FeatureTable,
                                               FeaturesTmpl[RootPortInfoPtr->Type].FeatureTableSize);
        IIO_D_PCILOG ("[%d.%d.%d] Root port '%a' device %02X:%02X:%02X.%X type %a(%d)\n",
                      IioIndex, Stack, Port, RootPortInfoPtr->Name,
                      RootPortInfoPtr->Segment, RootPortInfoPtr->Bus, RootPortInfoPtr->Dev, RootPortInfoPtr->Fun,
                      RootPortInfoPtr->Type == PCIE_LIB_DMI ? "DMI" : "PCI", RootPortInfoPtr->Type);
        for (FeaturePtr = RootPortInfoPtr->FeatureList; FeaturePtr->Common.Type < PCIE_LIB_FEATURE_MAX; FeaturePtr++) {

          switch (FeaturePtr->Common.Type) {
            case PCIE_LIB_FEATURE_CTO:
              FeaturePtr->Cto.Policy = PcieLibCompletionTO_Disabled;
              if (IioGlobalData->SetupData.CompletionTimeoutGlobal) {
                FeaturePtr->Cto.Policy = IioGlobalData->SetupData.CompletionTimeoutGlobalValue;
              } else if (IioGlobalData->SetupData.CompletionTimeout[IioIndex]) {
                FeaturePtr->Cto.Policy = IioGlobalData->SetupData.CompletionTimeoutValue[IioIndex];
              }
              break;
            case PCIE_LIB_FEATURE_ASPM:
              FeaturePtr->Aspm.Policy = GetAspmSettingForPort (IioGlobalData, IioIndex, PortIndex);
              break;
            case PCIE_LIB_FEATURE_MAXPAYLOAD:
              FeaturePtr->MaxPayload.MaxPayload =
                                IioGlobalData->SetupData.IioPcieConfig.PcieMaxPayload[IioIndex * MaxPortPerSocket + PortIndex];
              break;
            case PCIE_LIB_FEATURE_COMMONCLOCK:
// APTIOV_SERVER_OVERRIDE_RC_START : Pcie Common Clock programming 
              FeaturePtr->CommonClock.Flag= IioGlobalData->SetupData.PcieCommonClock[PCIE_LIB_FEATURE_COMMONCLOCK];
              FeaturePtr->CommonClock.Type = PCIE_LIB_FEATURE_COMMONCLOCK;
// APTIOV_SERVER_OVERRIDE_RC_END : Pcie Common Clock programming 
              break;
            case PCIE_LIB_FEATURE_EXTTAGFIELD:
              FeaturePtr->ExtendedTagField.Flag = IioGlobalData->SetupData.PcieExtendedTagField;
              break;
            case PCIE_LIB_FEATURE_ATOMICOP:
              FeaturePtr->AtomicOp.Flag = IioGlobalData->SetupData.PCIe_AtomicOpReq;
              break;

            case PCIE_LIB_FEATURE_LTR:
              if (IioGlobalData->SetupData.IioPcieConfig.PCIe_LTR == PCIE_LTR_ENABLE_AUTO) {
                FeaturePtr->Ltr.Flag = PCIE_LTR_ENABLE_YES;
              } else {
                FeaturePtr->Ltr.Flag = IioGlobalData->SetupData.IioPcieConfig.PCIe_LTR;
              }
              break;
            case PCIE_LIB_FEATURE_MRRS:
              FeaturePtr->Mrrs.Flag = PCIE_LIB_MRRS_OVERRIDE;
              if (IioGlobalData->SetupData.PcieMaxReadRequestSize != PCIE_LIB_MRRS_VALUE_AUTO) {
                FeaturePtr->Mrrs.MaxReadReqSize = IioGlobalData->SetupData.PcieMaxReadRequestSize;
              } else {
                FeaturePtr->Mrrs.MaxReadReqSize = PCIE_LIB_MRRS_4096B;
              }
              break;
            case PCIE_LIB_FEATURE_RELAXORDER:
              FeaturePtr->RelaxedOrder.Flag = IioGlobalData->SetupData.PcieRelaxedOrdering;
              break;
            case PCIE_LIB_FEATURE_PTM:
              FeaturePtr->Ptm.Flag = IioGlobalData->SetupData.IioPcieConfig.PciePtm;
              FeaturePtr->Ptm.UpStreamPtmCtrl.Uint32 = 0;
              break;
            case PCIE_LIB_FEATURE_10BIT_TAG:
              FeaturePtr->TenBitTag.Flag = IioGlobalData->SetupData.Pcie10bitTag;
              break;
            default:
              IIO_D_PCIERR ("[%d.%d.%d] Unknown feature type %d\n", IioIndex, Stack, Port, FeaturePtr->Common.Type);
              ASSERT (FALSE);
              break;
          } // switch (Type)
        } // for (FeatuurePtr...)
      } // for (Port...)
      RootPortInfoPtr->Type = PCIE_LIB_PORT_TYPE_MAX;
    } // for (Stack...)
  } // for (IioIndex...)
  HostBridgeInfoPtr->Revision = PCIE_LIB_HOSTBRIDGE_REVISION_END;
} // InitLibParameters()


/**
  This function calls PcieCommonLib to set the proper values for PCIe capabilities that
  require PCIe hierarchy analysis like ASPM, LTR, AtomicOp, Max Payload Size, Common Clock,
  Extented Tag and 10-Bit Tag for IIO rootports and endpoints.
  The PcieCommonLib also provides infromation about PCI bus numbers used to enumerate
  the hierarchy. If bus shortage was reported this function trigers rebalance procedure.

  @param[in] IioGlobalPtr - Pointer to IIO_GLOBALS
**/
VOID
PcieLateCommonLibInit (
  IN     IIO_GLOBALS          *IioGlobalPtr
  )
{
  EFI_GUID                     IioUdsHobGuid = IIO_UNIVERSAL_DATA_GUID;
  SYSTEM_PCI_BUS_CONFIGURATION SysPciBusConfig;
  UINTN                        VarSize;
  EFI_STATUS                   Status;
  BOOLEAN                      SysPciBusConfigChanged;
  BOOLEAN                      SocketBusConfigChanged[MAX_SOCKET];
  UINT16                       SocketFreeBusPoolSize[MAX_SOCKET];
  UINT8                        SocketStackPciBusPoolSize[MAX_SOCKET][MAX_LOGIC_IIO_STACK];
  UINT8                        SocketStacksWithBusShortage[MAX_SOCKET];
  INT16                        PciBusPoolSize;
  UINT8                        Segment;
  UINT8                        Socket;
  UINT8                        Stack;

  EFI_HOB_GUID_TYPE           *GuidHobPtr;
  IIO_UDS                     *IioUdsPtr;
  PCIE_LIB_TOPOLOGY_INF       *TopologyPtr;
  PCIE_LIB_HOST_BRIDGE_INF    *HostInfoPtr;
  PCIE_LIB_ROOT_PORT_INF      *RootInfoPtr;

  InitLibParameters (IioGlobalPtr);
  //
  // Call PcieInfoTreePaser() to apply PCIe capabilities that depend on PCI hierarchy.
  // Check the status for out-of-resources error returned in case of lack of PCI bus numbers.
  // If it occurs create request to KTI to rebalance PCI bus numbers.
  //
  TopologyPtr = (PCIE_LIB_TOPOLOGY_INF*)IioGlobalPtr->IioVar.IioOutData.PtrPcieTopology.Address64bit;
  Status = PcieInfoTreePaser (TopologyPtr);

  if (!EFI_ERROR (Status)) {
    //
    // No need for rebalancing, we are done.
    //
    return;

  } else if (Status != EFI_OUT_OF_RESOURCES) {
    //
    // Error other than out-of-resources is topology structure parse error, so error in InitLibParameters().
    //
    ASSERT_EFI_ERROR (Status);
    return;
  }
  GuidHobPtr = GetFirstGuidHob (&IioUdsHobGuid);
  if (GuidHobPtr == NULL) {

    IIO_D_PCIERR ("Could not locate IioUdsHobGuid, cannot perform PCI bus rebalance\n");
    return;
  }
  IioUdsPtr = GET_GUID_HOB_DATA (GuidHobPtr);
  VarSize = sizeof(SysPciBusConfig);
  Status = gRT->GetVariable (SYSTEM_PCI_BUS_CONFIGURATION_DATA_NAME, &gEfiSocketPciResourceDataGuid,
                             NULL, &VarSize, &SysPciBusConfig);
  if (EFI_ERROR (Status)) {

    if (Status == EFI_BUFFER_TOO_SMALL) {

      IIO_D_PCIERR ("Found PCI bus rebalance request variable '%s' larger than expected - overwrite\n",
                    SYSTEM_PCI_BUS_CONFIGURATION_DATA_NAME);
      Status = EFI_NOT_FOUND;
    }
    if (Status != EFI_NOT_FOUND) {

      IIO_D_PCIERR ("Reading PCI bus rebalance request variable '%s' failed (%r)\n",
                    SYSTEM_PCI_BUS_CONFIGURATION_DATA_NAME, Status);
      return;
    }
 OverwriteInvalidVariable:
    //
    // If the rebalance request variable does not exist in flash yet, or is invalid, copy current config
    // to the structure. For empty stack we have zero in SysPciBusConfig.
    //
    ZeroMem (&SysPciBusConfig, sizeof(SysPciBusConfig));
    for (Socket = 0; Socket < NELEMENTS (SysPciBusConfig.Socket); Socket++) {

      if (!IioUdsPtr->PlatformData.CpuQpiInfo[Socket].Valid) {
        continue;
      }
      SysPciBusConfig.Socket[Socket].Segment = IioUdsPtr->PlatformData.CpuQpiInfo[Socket].PcieSegment;
      for (Stack = 0; Stack < NELEMENTS (SysPciBusConfig.Socket[Socket].StackPciBusPoolSize); Stack++) {

        if (IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].BusLimit <
                                                IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].BusBase) {
          continue;
        }
        PciBusPoolSize = (INT16)IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].BusLimit -
                                              IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].BusBase + 1;
        if (PciBusPoolSize > 0 && PciBusPoolSize < PCI_MAX_BUS - MAX_IIO_STACK) {

          SysPciBusConfig.Socket[Socket].StackPciBusPoolSize[Stack] = (UINT8)PciBusPoolSize;
        }
      }
    }
  }
  if (VarSize != sizeof(SysPciBusConfig)) {

    IIO_D_PCIERR ("Found PCI bus rebalance request variable '%s' shorter (%d) than expected (%d) - reset variable\n",
                  SYSTEM_PCI_BUS_CONFIGURATION_DATA_NAME, VarSize, sizeof(SysPciBusConfig));
    VarSize = sizeof(SysPciBusConfig);
    goto OverwriteInvalidVariable; // overwrite variable of unexpected size
  }
  ZeroMem (&SocketBusConfigChanged[0], sizeof(SocketBusConfigChanged));
  ZeroMem (&SocketFreeBusPoolSize[0], sizeof(SocketFreeBusPoolSize));
  ZeroMem (&SocketStackPciBusPoolSize[0], sizeof(SocketStackPciBusPoolSize));
  ZeroMem (&SocketStacksWithBusShortage[0], sizeof(SocketStacksWithBusShortage));
  //
  // Now verify whether the previous request was applied. If KTI does not appy request we have to drop
  // rebalancing and continue boot. First verify segments assignment and stacks presence, because request
  // variable could be rejected because of topology change. If it is topology change then reset the request.
  //
  for (Socket = 0; Socket < NELEMENTS (SysPciBusConfig.Socket); Socket++) {

    if (!IioUdsPtr->PlatformData.CpuQpiInfo[Socket].Valid) {
      continue;
    }
    Segment = IioUdsPtr->PlatformData.CpuQpiInfo[Socket].PcieSegment;
    if (Segment != SysPciBusConfig.Socket[Socket].Segment) {

      IIO_D_PCIWARN ("[%d] Segment change %d -> %d - reset variable\n",
                     Socket, SysPciBusConfig.Socket[Socket].Segment, Segment);
      goto OverwriteInvalidVariable;
    }
    for (Stack = 0; Stack < NELEMENTS (SysPciBusConfig.Socket[Socket].StackPciBusPoolSize); Stack++) {

      if (IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].BusLimit <
                                                IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].BusBase) {
        if (SysPciBusConfig.Socket[Socket].StackPciBusPoolSize[Stack] > 0) {

          IIO_D_PCIWARN ("[%d.%d] Stack seen but not present now - reset variable\n", Socket, Stack);
          goto OverwriteInvalidVariable;
        }
        continue;
      }
      if (SysPciBusConfig.Socket[Socket].StackPciBusPoolSize[Stack] == 0) {

        IIO_D_PCIWARN ("[%d.%d] Not present stack is now present - reset variable\n", Socket, Stack);
        goto OverwriteInvalidVariable;
      }
      if (SysPciBusConfig.Socket[Socket].StackPciBusPoolSize[Stack] > PCI_MAX_BUS - MAX_IIO_STACK) {

        IIO_D_PCIERR ("PCI bus rebalance request variable '%s' invalid ([%d.%d] pool %d > max %d) - reset variable\n",
                      SYSTEM_PCI_BUS_CONFIGURATION_DATA_NAME, Socket, Stack,
                      SysPciBusConfig.Socket[Socket].StackPciBusPoolSize[Stack], PCI_MAX_BUS - MAX_IIO_STACK);
        goto OverwriteInvalidVariable;
      }
    }
  }
  //
  // Confirmed no topology change, now verify if the request was applied or ignored by KTI.
  // If ignored because of some unknown reason do not reset variable, just continue boot
  // to aboud reset loop.
  //
  for (Socket = 0; Socket < NELEMENTS (SysPciBusConfig.Socket); Socket++) {

    if (!IioUdsPtr->PlatformData.CpuQpiInfo[Socket].Valid) {
      continue;
    }
    for (Stack = 0; Stack < NELEMENTS (SysPciBusConfig.Socket[Socket].StackPciBusPoolSize); Stack++) {

      PciBusPoolSize = (INT16)IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].BusLimit -
                                              IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].BusBase + 1;
      if (PciBusPoolSize > 0 && PciBusPoolSize != SysPciBusConfig.Socket[Socket].StackPciBusPoolSize[Stack]) {

        IIO_D_PCIERR ("[%d.%d] PCI bus rebalance request not applied, req pool %d, got %d - skip rebalance\n",
                      Socket, Stack, SysPciBusConfig.Socket[Socket].StackPciBusPoolSize[Stack], PciBusPoolSize);
        return;
      }
      SocketStackPciBusPoolSize[Socket][Stack] = SysPciBusConfig.Socket[Socket].StackPciBusPoolSize[Stack];
    }
  }
  //
  // Now let's check how many buses we need in each stack. We need two runs:
  // 1. First save the number we actually need for stacks that were enumerated
  // 2. Then distribute what is left for the stacks that could not be enumerated.
  // First loop works on on bus configuration copy in SocketStackPciBusPoolSize[][].
  // Result is copied to SysPciBusConfig.Socket[].StackPciBusPoolSize[] in second loop.
  //
  for (SysPciBusConfigChanged = FALSE, HostInfoPtr = TopologyPtr->HostBridgeList;
       HostInfoPtr->Revision != PCIE_LIB_HOSTBRIDGE_REVISION_END; HostInfoPtr++) {

    switch (HostInfoPtr->Revision) {
      case PCIE_LIB_HOSTBRIDGE_REVISION_1:
        Segment = HostInfoPtr->RootPortList->Segment;
        Socket = HostInfoPtr->Name[4] - '0';
        Stack = HostInfoPtr->Name[6] - '0';
        if (HostInfoPtr->Attribute & PCIE_LIB_ATTR_BUS_SHORTAGE) {

          SocketStacksWithBusShortage[Socket]++;

        } else {

          PciBusPoolSize = HostInfoPtr->BusUsedNum;
          for (RootInfoPtr = HostInfoPtr->RootPortList; RootInfoPtr->Type < PCIE_LIB_PORT_TYPE_MAX; RootInfoPtr++) {

            if (RootInfoPtr->Valid && (RootInfoPtr->Attribute & PCIE_LIB_ATTR_HOTPLUG)) {

              PciBusPoolSize += 4;
            }
          }
          if (PciBusPoolSize < SocketStackPciBusPoolSize[Socket][Stack]) {

            SocketFreeBusPoolSize[Socket] += SocketStackPciBusPoolSize[Socket][Stack] - (UINT8)PciBusPoolSize;
            SocketStackPciBusPoolSize[Socket][Stack] = (UINT8)PciBusPoolSize;
          }
        }
        break;
      default:
        break;
    }
  }
  for (HostInfoPtr = TopologyPtr->HostBridgeList;
       HostInfoPtr->Revision != PCIE_LIB_HOSTBRIDGE_REVISION_END; HostInfoPtr++) {

    switch (HostInfoPtr->Revision) {
      case PCIE_LIB_HOSTBRIDGE_REVISION_1:
        Segment = HostInfoPtr->RootPortList->Segment;
        Socket = HostInfoPtr->Name[4] - '0';
        Stack = HostInfoPtr->Name[6] - '0';
        if (HostInfoPtr->Attribute & PCIE_LIB_ATTR_BUS_SHORTAGE) {

          if (SocketStacksWithBusShortage[Socket] > 0 && SocketFreeBusPoolSize[Socket] > 0) {

            SocketStackPciBusPoolSize[Socket][Stack] += (UINT8)(SocketFreeBusPoolSize[Socket] / SocketStacksWithBusShortage[Socket]);
            if (SocketFreeBusPoolSize[Socket] % SocketStacksWithBusShortage[Socket] != 0) {
              //
              // If free bus numbers cannot be distributed equally over stacks with shortage,
              // give the odd numbers to the first stack with shortage.
              //
              SocketStackPciBusPoolSize[Socket][Stack] += (UINT8)SocketFreeBusPoolSize[Socket] % SocketStacksWithBusShortage[Socket];
              SocketFreeBusPoolSize[Socket] -= SocketFreeBusPoolSize[Socket] % SocketStacksWithBusShortage[Socket];
            }
            CopyMem (&SysPciBusConfig.Socket[Socket].StackPciBusPoolSize[0],
                     &SocketStackPciBusPoolSize[Socket][0], sizeof(SysPciBusConfig.Socket[Socket].StackPciBusPoolSize));
            SysPciBusConfigChanged = TRUE;
          }
        }
        break;
      default:
        break;
    }
  }
  if (SysPciBusConfigChanged) {

    DEBUG ((DEBUG_ERROR, "[IIO] PCI bus rebalance request:\n"));
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

      DEBUG ((DEBUG_ERROR, "[IIO] Socket %d, segment %d, stack pool size:", Socket, SysPciBusConfig.Socket[Socket].Segment));
      for (Stack = 0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
        DEBUG ((DEBUG_ERROR, " %d", SysPciBusConfig.Socket[Socket].StackPciBusPoolSize[Stack]));
      }
      DEBUG ((DEBUG_ERROR, "\n"));
    }
    VarSize = sizeof(SysPciBusConfig);
    Status = gRT->SetVariable (SYSTEM_PCI_BUS_CONFIGURATION_DATA_NAME, &gEfiSocketPciResourceDataGuid,
                                EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS, VarSize, &SysPciBusConfig);
    if (EFI_ERROR (Status)) {

      IIO_D_PCIERR ("Writing PCI bus rebalance request variable '%s' failed (%r)\n",
                    SYSTEM_PCI_BUS_CONFIGURATION_DATA_NAME, Status);
      IIO_D_PCILOG ("Continue boot without PCI bus rebalance\n");

    } else {

      IIO_D_PCILOG ("PCI bus rebalance request '%s' written, reboot system...\n", SYSTEM_PCI_BUS_CONFIGURATION_DATA_NAME);
      gRT->ResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);
    }
  } // if (SysPciBusConfigChanged)
} // PcieLateCommonLibInit()


VOID
EFIAPI
IioCleanAssignedTempMmio (
  IN  EFI_HANDLE                                   RootBridgeHandle,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS  Address,
  IN  EFI_PCI_ENUMERATION_PHASE                    PciPhase,
  IN  EFI_PCI_CALLBACK_CONTEXT                     *Context
  )
{
  CleanAssignedTempMmio ();
}


/**
  This function is a callback on EfiPciEnumerationResourceAssigned event.
  It does the final IIO initialization intended for callback invokation.

  @param[in] RootBridgeHandle - Handle to RootBridge
  @param[in] Address          - Pcie Address of the RootBridge
  @param[in] PciPhase         - Pci Phase
  @param[in] Context          - Contex of the function
**/
VOID
EFIAPI
IioInitAfterResources (
  IN  EFI_HANDLE                                   RootBridgeHandle,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS  Address,
  IN  EFI_PCI_ENUMERATION_PHASE                    PciPhase,
  IN  EFI_PCI_CALLBACK_CONTEXT                     *Context
  )
{
  EFI_STATUS               Status;
  UINT8                    IioIndex;
  IIO_DEVFUNHIDE_TABLE    *DevHideTable;
  EFI_IIO_SYSTEM_PROTOCOL *IioSystemProtocol = NULL;
  IIO_GLOBALS             *IioGlobalData;

  //
  // Check whether this is real IioSystemProtocol notification, or just a SignalEvent
  //
  Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &IioSystemProtocol);
  if (EFI_ERROR (Status)) {
    return;
  }
  IioGlobalData = IioSystemProtocol->IioGlobalData;

  //
  // If this callback routine on its ready to boot event, than avoid executing
  // multiple times (fix for s4030764)
  //
  if (IioGlobalData->IioVar.IioVData.CompletedReadyToBootEventServices == TRUE) {
    return;
  }
  //
  //1404610230:IIO Break Points needs to be improved.
  //
  OutputCheckpointSocket (0, STS_IIO_ON_READY_TO_BOOT, 0, 0xFF);
  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {

    if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
      continue;
    }
    //
    // After PCI resources are allocated notify OOB-MSM.
    //
    OobSetPciCfgCmpl (IioIndex);

    if ((IioGlobalData->SetupData.LockChipset) && (!IioGlobalData->IioVar.IioVData.EVMode)) {
      TxtSecurePlatform (IioGlobalData, IioIndex);
    }
    //
    // init IOAT features here as the CB BAR needs to be programmed by the
    // system PCI bus driver
    //
    IIO_D_LOG ("Calling IoatInitBootEvent IioIndex: %d\n", IioIndex);
    IioIoatInitBootEvent (IioGlobalData, IioIndex);

    //
    // Program RootbusSn+1 (CPM, CPK for ICX-D and also HQM for SNR) mappings in OOBMSM.
    // Set Bus Number after PCI enumeration in DXE
    //
    IioSetRootBusNoOfExtendedStack (IioIndex);

    //
    // IOSF parity detection must be enabled by BIOS as part of supported RAS features.
    //
    IioParityEnable (IioIndex);

    //
    // Set MPS for End Point IPs located in Virtual Stacks
    //
    IioSetMpsForIpInExtendedStack (IioIndex);

    IIO_D_LOG ("Calling IioDisableLinkPorts IioIndex: %d\n", IioIndex);
    // Disable Link Ports if no EndPoints behind
    IioDisableLinkPorts (IioGlobalData, IioIndex);

    Status = IioMiscGetFuncHideTable (&DevHideTable);
    if (EFI_ERROR (Status)) {
      IIO_D_LOG ("No DevHideTable for IioIndex: %d\n", IioIndex);
      ASSERT_EFI_ERROR (Status);
    }
    IioDevicesHide (IioGlobalData, IioIndex, IioAfterResources, DevHideTable);

    Ms2iosfPostResources (IioGlobalData, IioIndex);

    IIO_D_LOG ("Calling NtbLateInit IioReadyToBoot IioAfterResources IioIndex: %d\n", IioIndex);
    NtbLateInit (IioGlobalData, IioIndex, IioAfterResources);
  }

  // Update SVID for IIO devices
  UpdateSsids (IioGlobalData);

  IioGlobalData->IioVar.IioVData.CompletedReadyToBootEventServices = TRUE;    //fix for s4030764
}


/**
  Initialize MultiCast as per validation requirement.
  (It is specific DualCast configuration using NTB as documented in the IIO spec.
  Regular system-wide Multicast configuration/policy is expected to be configured by OS/Device drivers.)


  @param[in]  IioGlobalData      Platform configuration info
  @param[in]  IioIndex           Current socket.
  @param[in]  PortIndex          Current port.

  @retval     MCAST_RET_SUCCESS Configured without any error
  @retval     Non-zero values   Other specific failures
**/
UINT8
EnableMultiCast (
  IN  IIO_GLOBALS           *IioGlobalData,
  IN  UINT8                 IioIndex,
  IN  UINT8                 PortIndex
  )
{
  EFI_STATUS                             Status;
  UINT8                                  Stack;
  EFI_PHYSICAL_ADDRESS                   McastBaseAddress;
  EFI_ALLOCATE_TYPE                      McastMemRangeType;
  UINTN                                  McastRsvdMemSize;
  UINT8                                  McastNumGrp;
  UINT8                                  McastIndxPos;
#if !defined (SKX_HOST)
  OTCERRCTL_IIO_RAS_STRUCT               OtcErrCtl;
  OTCERRSEV_N0_IIO_RAS_STRUCT            OtcErrSev;
  OTCERRST_IIO_RAS_STRUCT                OtcErrSt;
#else //!defined(SKX_HOST)
  TCERRCTL_IIO_RAS_STRUCT                TcErrCtl;
  OTCERRSEV_IIO_RAS_STRUCT               OtcErrSev;
  TCERRST_IIO_RAS_STRUCT                 TcErrSt;
#endif //!defined(SKX_HOST)

  if ((!IsCpuAndRevision (CPU_SKX, REV_ALL) && !IsCpuAndRevision (CPU_CLX, REV_ALL) && !IsCpuAndRevision (CPU_CPX, REV_ALL))
    && (!IsCpuAndRevision (CPU_ICXSP, REV_ALL) && !IsCpuAndRevision (CPU_ICXD, REV_ALL) && !IsCpuAndRevision (CPU_SNR, REV_ALL))) {
    return MCAST_ERR_NOT_SUPPORTED;
  }

  if (IioIndex >= MaxIIO || PortIndex >= NUMBER_PORTS_PER_SOCKET) {
    ASSERT (IioIndex < MaxIIO && PortIndex < NUMBER_PORTS_PER_SOCKET);
    return MCAST_ERR_NOT_SUPPORTED;
  }

  //
  // Read in configured number of groups and index position
  //
  McastNumGrp   = IioGlobalData->SetupData.McastNumGroup;
  McastIndxPos  = IioGlobalData->SetupData.McastIndexPosition;

  //
  // Allocate memory for Mcast test if not already allocated
  //
  if (IioGlobalData->IioVar.IioOutData.McastRsvdMemory != 0) {
    //
    // Already allocated, get the address
    //
    McastBaseAddress = IioGlobalData->IioVar.IioOutData.McastRsvdMemory;

  } else {
    //
    // Allocate memory for Mcast as per user config
    //
    if (IioGlobalData->SetupData.McastBaseAddrRegion == MCAST_MEM_AUTO) {
      //
      // Any location
      //
      McastMemRangeType = AllocateAnyPages;
      McastBaseAddress  = 0;

    } else if (IioGlobalData->SetupData.McastBaseAddrRegion == MCAST_MEM_BELOW_4G) {
      //
      // Force below 4G
      //
      McastMemRangeType = AllocateMaxAddress;
      McastBaseAddress  = 0xFFFFFFFF;          // Below 4G

    } else {

      return MCAST_ERR_MEM_ALLOC_FAIL;
    }

    //
    // Calculate size required as per number of groups (McastNumGroup) and size as per (McastIndexPosition)
    //
    McastRsvdMemSize = McastNumGrp * (UINTN)( 1 << McastIndxPos );

    //
    // Now allocate memory
    //
    Status = gBS->AllocatePages (
                     McastMemRangeType,
                     EfiBootServicesData,
                     EFI_SIZE_TO_PAGES ((UINTN)McastRsvdMemSize),
                     &McastBaseAddress
                     );

    if (McastBaseAddress == 0) {
      IIO_D_WARN ("MCAST: Memory Alloc Error\n");
      return MCAST_ERR_MEM_ALLOC_FAIL;

    } else {
      IioGlobalData->IioVar.IioOutData.McastRsvdMemory = McastBaseAddress;
      IIO_D_DBG ("MCAST: Allocated Membase at 0x%x, Size 0x%x\n", IioGlobalData->IioVar.IioOutData.McastRsvdMemory, McastRsvdMemSize);
    }

  }

  //
  // Set MC Base Address and MC Index Position
  //
  Status = IioSetMcBaseAdressAndIndexPosition (IioIndex, PortIndex, McastIndxPos, McastBaseAddress);
  ASSERT_EFI_ERROR (Status);

  //
  // Clear MCAST block all
  //
  Status = IioClearMcBlockAll (IioIndex, PortIndex);
  ASSERT_EFI_ERROR (Status);
  //
  //  MCAST Receive should be set for NTB
  //
  if ((IioGlobalData->IioVar.IioOutData.PciePortOwnership[(IioIndex * NUMBER_PORTS_PER_SOCKET) + PortIndex] == NTB_PORT_DEF_NTB_NTB ) ||
      (IioGlobalData->IioVar.IioOutData.PciePortOwnership[(IioIndex * NUMBER_PORTS_PER_SOCKET) + PortIndex] == NTB_PORT_DEF_NTB_RP )) {

    Status = IioSetMmCastForNtb (IioIndex, PortIndex, McastNumGrp);
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Set MC Number of groups MC_NUM_GROUP and MC_ENABLE
  //
  Status = IioSetMcNumberOfGroupsAndMcEnable (IioIndex, PortIndex, McastNumGrp);
  ASSERT_EFI_ERROR (Status);

  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_SNR, REV_ALL)) {
#if !defined (SKX_HOST)
    //
    // 4929386: Cloned From SKX Si Bug Eco: OTC incorrectly logs error for a legal multicast region hit
    //
    Stack = IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex][PortIndex];
    OtcErrCtl.Data = IioReadCpuCsr32 (IioIndex, Stack, OTCERRCTL_IIO_RAS_REG);
    if (OtcErrCtl.Bits.otc_mabort != 0) {
      OtcErrCtl.Bits.otc_mabort = 0;
      IioWriteCpuCsr32 (IioIndex, Stack, OTCERRCTL_IIO_RAS_REG, OtcErrCtl.Data);
    }

    OtcErrSev.Data = IioReadCpuCsr32 (IioIndex, Stack, OTCERRSEV_N0_IIO_RAS_REG);
    if (OtcErrSev.Bits.otc_mabort != 1) {
      OtcErrSev.Bits.otc_mabort = 1;
      IioWriteCpuCsr32 (IioIndex, Stack, OTCERRSEV_N0_IIO_RAS_REG, OtcErrSev.Data);
    }

    OtcErrSt.Data = IioReadCpuCsr32 (IioIndex, Stack, OTCERRST_IIO_RAS_REG);
    if (OtcErrSt.Bits.otc_mabort == 1) {
      IioWriteCpuCsr32 (IioIndex, Stack, OTCERRST_IIO_RAS_REG, OtcErrSt.Data);
    }
#else //!defined(SKX_HOST)
    //
    // 4929386: Cloned From SKX Si Bug Eco: OTC incorrectly logs error for a legal multicast region hit
    //
    Stack = IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex][PortIndex];
    TcErrCtl.Data = IioReadCpuCsr32 (IioIndex, Stack, TCERRCTL_IIO_RAS_REG);
    if (TcErrCtl.Bits.otc_mabort != 0) {
      TcErrCtl.Bits.otc_mabort = 0;
      IioWriteCpuCsr32 (IioIndex, Stack, TCERRCTL_IIO_RAS_REG, TcErrCtl.Data);
    }

    OtcErrSev.Data = IioReadCpuCsr32 (IioIndex, Stack, OTCERRSEV_IIO_RAS_REG);
    if (OtcErrSev.Bits.otc_mabort != 1) {
      OtcErrSev.Bits.otc_mabort = 1;
      IioWriteCpuCsr32 (IioIndex, Stack, OTCERRSEV_IIO_RAS_REG, OtcErrSev.Data);
    }

    TcErrSt.Data = IioReadCpuCsr32 (IioIndex, Stack, TCERRST_IIO_RAS_REG);
    if (TcErrSt.Bits.otc_mabort == 1) {
      IioWriteCpuCsr32 (IioIndex, Stack, TCERRST_IIO_RAS_REG, TcErrSt.Data);
    }
#endif //!defined(SKX_HOST)

  }

  return MCAST_RET_SUCCESS;
}
//
// 1405207980:IIO code needs to be change to use Data structure filed instead of Bitfiled due to a but for PCIE regsiters definitons for ICX
//
/**

   This is the API for the EFI_IIO_SYSTEM_PROTOCOL interface which returns the details
   of the uplink port if connected to the platform

   @param PortDescriptor - Pointer to PORT_DESCRIPTOR data structure which contains the PCI address (Bus, Device, function)
   @param PortStatus     - Pointer to boolean which if TRUE indicates Uplink port is enabled else FALSE for disabled
   @param PortAttrib     - Pointer to PORT_ATTRIB data structure which gives the uplink ports width and speed

   @retval EFI_UNSUPPORTED - if system does not support the uplink port,
           EFI_DEVICE_ERROR - if internal error encountered,
           EFI_SUCCESS - for returning valid data

**/
EFI_STATUS
EFIAPI
IioGetCpuUplinkPort10nm (
  UINT8             IioIndex,
  PORT_DESCRIPTOR   *PortDescriptor,    //Bus, Device, function
  BOOLEAN           *PortStatus,        //TRUE if enabled else disabled
  PORT_ATTRIB       *PortAttrib         //width and speed
  )
{
  EFI_STATUS                      Status;
  UINT8                           PortIndex;
  UINT8                           MaxPortNumberPerSocket;
  EFI_IIO_SYSTEM_PROTOCOL         *IioSystemProtocol = NULL;
  IIO_GLOBALS                     *IioGlobalData = NULL;
  UINT16                          VidIioPcie;
  PCI_REG_PCIE_LINK_STATUS        LnkSts;
  PCI_REG_PCIE_LINK_CONTROL       LnkCon;
  UINT8                           PCIeCapOffset;
  UINT8                           Bus;
  UINT8                           Device;
  UINT8                           Function;

  Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &IioSystemProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  IioGlobalData = IioSystemProtocol->IioGlobalData;
  OemIioUplinkPortDetails (IioGlobalData, IioIndex, &PortIndex, &Bus, &Device, &Function);
  if (IioIndex == 0xFF && PortIndex == 0xFF) {
    return EFI_UNSUPPORTED;
  }             //not uplink port provided in the platform

  //
  // Assume Legacy socket always present (where uplink port is)
  //
  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
  if (PortIndex >= MaxPortNumberPerSocket) {
    return EFI_DEVICE_ERROR;
  }

  PortDescriptor->Bus       = Bus;
  PortDescriptor->Device    = Device;
  PortDescriptor->Function  = Function;

  PCIeCapOffset = IioPCIeGetCapOffset (IioIndex, Bus, Device, Function, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (PCIeCapOffset == 0) {
    IIO_D_ERR ("IioGetCpuUplinkPort10nm Can't find PCIeCapOffset!\n");
    ASSERT (PCIeCapOffset);
  } else {
    VidIioPcie = (UINT16) IioPciExpressRead16 (IioIndex, Bus, Device, Function, PCIeCapOffset + PCI_VENDOR_ID_OFFSET );

    if (VidIioPcie == 0xFFFF) {
      //port not found! could be hidden
      return EFI_DEVICE_ERROR;
    }

    LnkSts.Uint16 = IioPciExpressRead16 (IioIndex, Bus, Device, Function, PCIeCapOffset + R_PCIE_LSTS_OFFSET);

    PortAttrib->PortWidth = (UINT8)LnkSts.Bits.NegotiatedLinkWidth;   //width could be 1, 2, 4, 8, 16
    PortAttrib->PortSpeed = (UINT8)LnkSts.Bits.CurrentLinkSpeed;           //speed : 1=Gen1, 2=Gen2, 3=Gen3

    LnkCon.Uint16 = IioPciExpressRead16 (IioIndex, Bus, Device, Function, PCIeCapOffset + R_PCIE_LCTL_OFFSET);
   *PortStatus = (LnkCon.Bits.LinkDisable) ? FALSE : TRUE;
  }

  return EFI_SUCCESS;
}

/**

   This is the API for the EFI_IIO_SYSTEM_PROTOCOL interface which returns the details
   of the uplink port if connected to the platform

   @param PortDescriptor - Pointer to PORT_DESCRIPTOR data structure which contains the PCI address (Bus, Device, function)
   @param PortStatus     - Pointer to boolean which if TRUE indicates Uplink port is enabled else FALSE for disabled
   @param PortAttrib     - Pointer to PORT_ATTRIB data structure which gives the uplink ports width and speed

   @retval EFI_UNSUPPORTED - if system does not support the uplink port,
           EFI_DEVICE_ERROR - if internal error encountered,
           EFI_SUCCESS - for returning valid data

**/
EFI_STATUS
EFIAPI
IioGetCpuUplinkPortSkx (
  UINT8             IioIndex,
  PORT_DESCRIPTOR   *PortDescriptor,    //Bus, Device, function
  BOOLEAN           *PortStatus,        //TRUE if enabled else disabled
  PORT_ATTRIB       *PortAttrib         //width and speed
  )
{
  EFI_STATUS                      Status;
  UINT8                           PortIndex;
  UINT8                           MaxPortNumberPerSocket;
  EFI_IIO_SYSTEM_PROTOCOL         *IioSystemProtocol = NULL;
  IIO_GLOBALS                     *IioGlobalData = NULL;
  UINT16                          VidIioPcie;
  PCI_REG_PCIE_LINK_STATUS        LnkSts;
  PCI_REG_PCIE_LINK_CONTROL       LnkCon;
  UINT8                           PCIeCapOffset;
  UINT8                           Bus;
  UINT8                           Device;
  UINT8                           Function;

  Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &IioSystemProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  IioGlobalData = IioSystemProtocol->IioGlobalData;
  OemIioUplinkPortDetails (IioGlobalData, IioIndex, &PortIndex, &Bus, &Device, &Function);
  if (IioIndex == 0xFF && PortIndex == 0xFF) {
    return EFI_UNSUPPORTED;
  }//not uplink port provided in the platform

  //
  // Assume Legacy socket always present (where uplink port is)
  //
  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
  if (PortIndex >= MaxPortNumberPerSocket) {
    return EFI_DEVICE_ERROR;
  }

  PortDescriptor->Bus       = Bus;
  PortDescriptor->Device    = Device;
  PortDescriptor->Function  = Function;

  PCIeCapOffset = IioPCIeGetCapOffset (IioIndex, Bus, Device, Function, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (PCIeCapOffset == 0) {
    IIO_D_ERR ("IioGetCpuUplinkPort10nm Can't find PCIeCapOffset!\n");
    ASSERT (PCIeCapOffset);
  } else {

    VidIioPcie = (UINT16) IioPciExpressRead16 (IioIndex, Bus, Device, Function, PCIeCapOffset + PCI_VENDOR_ID_OFFSET );

    if (VidIioPcie == 0xFFFF) {
      //port not found! could be hidden
      return EFI_DEVICE_ERROR;
    }

    LnkSts.Uint16 = IioPciExpressRead16 (IioIndex, Bus, Device, Function, PCIeCapOffset + R_PCIE_LSTS_OFFSET);

    PortAttrib->PortWidth = (UINT8)LnkSts.Bits.NegotiatedLinkWidth;   //width could be 1, 2, 4, 8, 16
    PortAttrib->PortSpeed = (UINT8)LnkSts.Bits.CurrentLinkSpeed;           //speed : 1=Gen1, 2=Gen2, 3=Gen3

    LnkCon.Uint16 = IioPciExpressRead16 (IioIndex, Bus, Device, Function, PCIeCapOffset + R_PCIE_LCTL_OFFSET);
    *PortStatus = (LnkCon.Bits.LinkDisable) ? FALSE : TRUE;
  }

  return EFI_SUCCESS;

}

//
// 1405207980:IIO code needs to be change to use Data structure filed instead of Bitfiled due to a but for PCIE regsiters definitons for ICX
//
/**

   This is the API for the EFI_IIO_SYSTEM_PROTOCOL interface which returns the details
   of the uplink port if connected to the platform

   @param PortDescriptor - Pointer to PORT_DESCRIPTOR data structure which contains the PCI address (Bus, Device, function)
   @param PortStatus     - Pointer to boolean which if TRUE indicates Uplink port is enabled else FALSE for disabled
   @param PortAttrib     - Pointer to PORT_ATTRIB data structure which gives the uplink ports width and speed

   @retval EFI_UNSUPPORTED - if system does not support the uplink port,
           EFI_DEVICE_ERROR - if internal error encountered,
           EFI_SUCCESS - for returning valid data

**/
EFI_STATUS
EFIAPI
IioGetCpuUplinkPort (
  UINT8             IioIndex,
  PORT_DESCRIPTOR   *PortDescriptor,    //Bus, Device, function
  BOOLEAN           *PortStatus,        //TRUE if enabled else disabled
  PORT_ATTRIB       *PortAttrib         //width and speed
  )
{
  EFI_IIO_SYSTEM_PROTOCOL                     *IioSystemProtocol = NULL;
  IIO_GLOBALS                                 *IioGlobalData = NULL;
  EFI_STATUS                                  Status;

  Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &IioSystemProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  IioGlobalData = IioSystemProtocol->IioGlobalData;


  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    Status = IioGetCpuUplinkPortSkx (IioIndex, PortDescriptor, PortStatus, PortAttrib);
  } else {
    Status = IioGetCpuUplinkPort10nm (IioIndex, PortDescriptor, PortStatus, PortAttrib);
  }
  return Status;
}

/**
  IIO initialization before boot to OS

  @param[in] Event                A pointer to the Event that triggered the callback.
  @param[in] Context              A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
IioReadyToBootCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  UINT8                         IioIndex;
  EFI_STATUS                    Status;
  EFI_IIO_SYSTEM_PROTOCOL       *IioSystemProtocol = NULL;
  IIO_GLOBALS                   *IioGlobalData = NULL;

  Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &IioSystemProtocol);
  if (EFI_ERROR (Status)) {
    return;
  }

  IioGlobalData = IioSystemProtocol->IioGlobalData;

  gBS->CloseEvent (Event);

  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
    if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
      continue;
    }

    IIO_D_LOG ("Calling IioIoatReadyToBootEvent IioIndex: %d\n", IioIndex);
    IioIoatReadyToBootEvent (IioGlobalData, IioIndex);
    //
    // Initialize IOAPIC if SKX is present
    //
    if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
      //
      // Enable IOAPIC
      //
      IIO_D_LOG ("Calling IioIoApicInitBootEvent IioIndex: %d\n", IioIndex);
      IioIoApicInitBootEvent (IioGlobalData, IioIndex);
    }

    IIO_D_LOG ("Calling NtbLateInit IioReadyToBoot IioIndex: %d\n", IioIndex);
    NtbLateInit (IioGlobalData, IioIndex, IioReadyToBoot);
  }
}

/**

  Call back routine used to do all IIO post PCie port configuration, initialization

  @param IioGlobalData  - Pointer to IIO_GLOBALS

  @retval None
**/
EFI_STATUS
IioPostLateInitialize (
  IIO_GLOBALS  *IioGlobalData
  )
{
  EFI_STATUS                    Status;
  EFI_PCI_CALLBACK_PROTOCOL     *PciCallBackProtocol;
  UINT8                         IioIndex;
  UINT8                         PortIndex;
  UINT8                         MaxPortNumberPerSocket;
  EFI_EVENT                     IioReadyToBootEvent;

  if (UbiosGenerationEnabled ()) {
    IIO_D_LOG ("IIO DXE Trace Hub & SPK Programming Start\n");
  }

  //
  // initialize Trace Hub (NPK)
  //
  Status = IioTraceHubInitialize ();
  if (EFI_ERROR (Status)) {
    IIO_D_ERR ("IioTraceHubInitialize failed. Status= %r\n", Status);
  }
  //
  // initialize Sierra Peak
  //
  IioSpkInitialize ();

  if (UbiosGenerationEnabled ()) {
    IIO_D_LOG ("IIO DXE Trace Hub & SPK Programming End\n");
  }

  //
  // Configure Multicast if enabled
  //
  if (IioGlobalData->SetupData.MultiCastEnable) {
    for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
      if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
        continue;
      }

      MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
      for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
        if (!IioVData_ConfigurePciePort (IioGlobalData, IioIndex, PortIndex)) {
          //
          //check whether the PCIe port can be configured
          //
          continue;
        }
        EnableMultiCast (IioGlobalData, IioIndex, PortIndex);
      }
    }
  }

  //
  // now set Pcie Features in IIO Root Ports and End Points as need it.
  //
  PcieLateCommonLibInit (IioGlobalData);

  DumpIioPciePortPcieLinkStatus (IioGlobalData);

  Status = gBS->LocateProtocol (
                   &gEfiPciCallbackProtocolGuid,
                   NULL,
                   &PciCallBackProtocol
                   );
  ASSERT_EFI_ERROR (Status);

  Status = PciCallBackProtocol->RegisterPciCallback(
                    PciCallBackProtocol,
                    IioInitAfterResources,
                    EfiPciEnumerationResourceAssigned
                    );
  ASSERT_EFI_ERROR (Status);

  Status = PciCallBackProtocol->RegisterPciCallback(
                    PciCallBackProtocol,
                    IioCleanAssignedTempMmio,
                    EfiPciEnumerationDeviceScanning
                    );
  ASSERT_EFI_ERROR (Status);

  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             IioReadyToBootCallback,
             NULL,
             &IioReadyToBootEvent
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
