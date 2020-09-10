/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2020 Intel Corporation. <BR>

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

#include <Uefi/UefiBaseType.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcieCommonInitLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/TimerLib.h>
#include <Library/SiliconWorkaroundLib.h>

#include "PcieCommonHelper.h"


/******************************************************************************
 * Local definitions.
 ******************************************************************************/
/**
  Uncomment the PCIDEBUG macro to enable tracing the library activity in a test build.
 **/
#define PCIDEBUG(...) // { DEBUG((DEBUG_INFO, "[PCI] " __VA_ARGS__)); }

#define PCIE_DEVICE_PORT_TYPE_INVALID 0xFF


/******************************************************************************
 * Functions.
 ******************************************************************************/
/**
  Parse top level of PCI topology.

  Platform developer needs to fill out the information of PCIE_LIB_TOPOLOGY_INF
  and its sub-content in HostBridgeList and call PcieInfoTreePaser
  to initialize whole PCI tree.

  @param[in,out] TopologyPtr PCI topology info.

  @retval EFI_SUCCESS           - Everything went fine.
  @retval EFI_OUT_OF_RESOURCES  - Because of lack of PCI bus numbers not all topology could be enumerated.
  @retval EFI_INVALID_PARAMETER - Invalid topology structure provide.
**/
EFI_STATUS
PcieInfoTreePaser (
  PCIE_LIB_TOPOLOGY_INF    *TopologyPtr
  )
{
  EFI_STATUS                Status = EFI_SUCCESS;
  UINT8                     BusUsedNumBkp;

  PCIE_LIB_HOST_BRIDGE_INF *HostInfoPtr;
  PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr;

  if (TopologyPtr == NULL || TopologyPtr->Revision != PCIE_LIB_TOPOLOGY_REVISION_1 ||
      TopologyPtr->HostBridgeList == NULL || TopologyPtr->HostBridgeList->RootPortList == NULL) {

    return EFI_INVALID_PARAMETER;
  }
  for (HostInfoPtr = TopologyPtr->HostBridgeList; HostInfoPtr->Revision != PCIE_LIB_HOSTBRIDGE_REVISION_END; HostInfoPtr++) {

    switch (HostInfoPtr->Revision) {
      case PCIE_LIB_HOSTBRIDGE_REVISION_1:
        DEBUG ((DEBUG_INFO, "[PCI] Initialize host bridge '%a'\n", &HostInfoPtr->Name[0]));
        for (RootInfoPtr = HostInfoPtr->RootPortList; RootInfoPtr->Type < PCIE_LIB_PORT_TYPE_MAX; RootInfoPtr++) {

          if (RootInfoPtr->Valid) {

            DEBUG ((DEBUG_INFO, "[PCI] Initialize root port '%a' at %02X:%02X:%02X.%X and downstream devices\n",
                    &RootInfoPtr->Name[0], RootInfoPtr->Segment, RootInfoPtr->Bus, RootInfoPtr->Dev, RootInfoPtr->Fun));
            BusUsedNumBkp = HostInfoPtr->BusUsedNum;
            PciInitBridgeDownstreamDevices (PciCapPhase0, HostInfoPtr, RootInfoPtr, NULL, NULL,
                                            FixedPcdGetBool (PcdCleanTempBusAssignment));
            if (RootInfoPtr->Valid) {

              HostInfoPtr->BusUsedNum = BusUsedNumBkp;
              RootInfoPtr->NestedLevel = 0;
              PciInitBridgeDownstreamDevices (PciCapPhase1, HostInfoPtr, RootInfoPtr, NULL, NULL,
                                              FixedPcdGetBool (PcdCleanTempBusAssignment));
            }
          }
        }
        if (HostInfoPtr->Attribute & PCIE_LIB_ATTR_BUS_SHORTAGE) {

          DEBUG ((DEBUG_ERROR, "[PCI] WARNING: Host bridge '%a' lacks PCI bus numbers\n", &HostInfoPtr->Name[0]));
          Status = EFI_OUT_OF_RESOURCES;
        }
        break;
      default:
        DEBUG ((DEBUG_ERROR, "[PCI] ERROR: Unknown PCIE_LIB_HOST_BRIDGE_INF revision %d found in '%a' - skip\n",
                HostInfoPtr->Revision, &HostInfoPtr->Name[0]));
      case PCIE_LIB_HOSTBRIDGE_REVISION_SKIP:
        PCIDEBUG ("Skip host bridge '%a'\n", &HostInfoPtr->Name[0]);
        break;
    }
  } // for (HostInfoPtr...)
  return Status;
}


/**
  Update ASPM for root port hierarchy according to device capabilities.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciDstrUpdateAspm (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_ASPM_INF        *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  if (Phase != PciCapPhase0) {
    //
    // ASPM capability is analyzed in the hierarchy scan.
    //
    return;
  }
  if (ParAddrPtr == NULL) {
    if (FeaturePtr->Policy > PcieLibAspmL0sL1) {
      FeaturePtr->Policy = PcieLibAspmL0sL1;
    }
    FeaturePtr->L0sLatency = 0;
    FeaturePtr->L1Latency = 0;
    FeaturePtr->L0sAcceptLatency = 7;
    FeaturePtr->L1AcceptLatency = 7;
  }
  PCIDEBUG ("%02X:%02X:%02X.%X: D%d LinkCap.Aspm %X & %X -> %X\n",
            DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
            Phase, DevCapPtr->LinkCapability.Bits.Aspm, FeaturePtr->Policy,
            DevCapPtr->LinkCapability.Bits.Aspm & FeaturePtr->Policy);
  FeaturePtr->Policy &= (PCIE_LIB_ASPM_POLICY)DevCapPtr->LinkCapability.Bits.Aspm; // 2-bit field never exceeds PcieLibAspmL0sL1
  if ((DevCapPtr->LinkCapability.Bits.Aspm & PcieLibAspmL0s) &&
      FeaturePtr->L0sLatency < DevCapPtr->LinkCapability.Bits.L0sExitLatency) {

    PCIDEBUG ("%02X:%02X:%02X.%X: D%d LinkCap.L0sExitLatency %d -> %d\n",
              DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
              Phase, FeaturePtr->L0sLatency, DevCapPtr->LinkCapability.Bits.L0sExitLatency);
    FeaturePtr->L0sLatency = (UINT8)DevCapPtr->LinkCapability.Bits.L0sExitLatency;
  }
  if ((DevCapPtr->LinkCapability.Bits.Aspm & PcieLibAspmL1) &&
      FeaturePtr->L1Latency < DevCapPtr->LinkCapability.Bits.L1ExitLatency) {

    PCIDEBUG ("%02X:%02X:%02X.%X: D%d LinkCap.L1ExitLatency %d -> %d\n",
              DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
              Phase, FeaturePtr->L1Latency, DevCapPtr->LinkCapability.Bits.L1ExitLatency);
    FeaturePtr->L1Latency = (UINT8)DevCapPtr->LinkCapability.Bits.L1ExitLatency;
  }
  switch (DevCapPtr->Capability.Bits.DevicePortType) {
    case PCIE_DEVICE_PORT_TYPE_LEGACY_PCIE_ENDPOINT:
    case PCIE_DEVICE_PORT_TYPE_PCIE_ENDPOINT:
      if ((DevCapPtr->LinkCapability.Bits.Aspm & PcieLibAspmL0s) &&
          FeaturePtr->L0sAcceptLatency > DevCapPtr->DeviceCapability.Bits.EndpointL0sAcceptableLatency) {

        PCIDEBUG ("%02X:%02X:%02X.%X: D%d DevCap.EndpointL0sAcceptableLatency %d -> %d\n",
                  DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
                  Phase, FeaturePtr->L0sAcceptLatency, DevCapPtr->DeviceCapability.Bits.EndpointL0sAcceptableLatency);
        FeaturePtr->L0sAcceptLatency = (UINT8)DevCapPtr->DeviceCapability.Bits.EndpointL0sAcceptableLatency;

        if (FeaturePtr->L0sAcceptLatency < FeaturePtr->L0sLatency) {

          PCIDEBUG ("%02X:%02X:%02X.%X: D%d L0sAccept %d < L0sLatency %d Policy %X -> %X\n",
                    DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
                    Phase, FeaturePtr->L0sAcceptLatency, FeaturePtr->L0sLatency, FeaturePtr->Policy,
                    ~PcieLibAspmL0s & FeaturePtr->Policy);
          FeaturePtr->Policy &= ~PcieLibAspmL0s;
        }
      }
      if ((DevCapPtr->LinkCapability.Bits.Aspm & PcieLibAspmL1) &&
          FeaturePtr->L1AcceptLatency > DevCapPtr->DeviceCapability.Bits.EndpointL1AcceptableLatency) {

        PCIDEBUG ("%02X:%02X:%02X.%X: D%d DevCap.EndpointL1AcceptableLatency %d -> %d\n",
                  DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
                  Phase, FeaturePtr->L1AcceptLatency, DevCapPtr->DeviceCapability.Bits.EndpointL1AcceptableLatency);
        FeaturePtr->L1AcceptLatency = (UINT8)DevCapPtr->DeviceCapability.Bits.EndpointL1AcceptableLatency;

        if (FeaturePtr->L1AcceptLatency < FeaturePtr->L1Latency) {

          PCIDEBUG ("%02X:%02X:%02X.%X: D%d L1Accept %d < L1Latency %d Policy %X -> %X\n",
                    DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
                    Phase, FeaturePtr->L1AcceptLatency, FeaturePtr->L1Latency, FeaturePtr->Policy,
                    ~PcieLibAspmL1 & FeaturePtr->Policy);
          FeaturePtr->Policy &= ~PcieLibAspmL1;
        }
      }
      break;
    default:
      break;
  }
  return;
}


/**
  Set ASPM to the endpoint and up stream according to feature node.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciUstrUpdateAspm (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_ASPM_INF        *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  USRA_ADDRESS                     DevAddr;

  if (Phase != PciCapPhase1) {
    //
    // ASPM capability is applied in the second hierarchy scan.
    //
    return;
  }
  PCIDEBUG ("%02X:%02X:%02X.%X: D%d LinkCap %08X LinkCtl %04X AspmPolicy %X\n",
            DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
            Phase, DevCapPtr->LinkCapability.Uint32, DevCapPtr->LinkControl.Uint16, FeaturePtr->Policy);
  if (RootInfoPtr->Attribute & PCIE_LIB_ATTR_ENDPOINT) {
    //
    // Set ASPM support in accordance with policy and capabilities.
    //
    if (DevCapPtr->LinkControl.Bits.AspmControl != FeaturePtr->Policy) {

      PCIDEBUG ("%02X:%02X:%02X.%X: U%d LinkCtl.AspmControl %d -> %d\n",
                DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
                Phase, DevCapPtr->LinkControl.Bits.AspmControl, FeaturePtr->Policy);
      DevCapPtr->LinkControl.Bits.AspmControl = FeaturePtr->Policy;
      USRA_ADDRESS_COPY (&DevAddr, DevAddrPtr);
      DevAddr.Attribute.AccessWidth = UsraWidth16;
      DevAddr.Attribute.AddrType = AddrTypePCIE;
      DevAddr.Pcie.Offset += OFFSET_OF (PCI_CAPABILITY_PCIEXP, LinkControl);
      RegisterWrite (&DevAddr, &DevCapPtr->LinkControl);
    }
  }
  return;
}


/**
  Update Extended Tag for root port hierarchy according to device capabilities.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciDstrUpdateExtendedTagField (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_EXTTAGFIELD_INF *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  if (Phase != PciCapPhase0) {
    //
    // This capability is analyzed in the first hierarchy scan.
    //
    return;
  }
  PCIDEBUG ("%02X:%02X:%02X.%X: D%d DevCap.ExtendedTag %d & %d -> %d\n",
            DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
            Phase, DevCapPtr->DeviceCapability.Bits.ExtendedTagField, FeaturePtr->Flag,
            DevCapPtr->DeviceCapability.Bits.ExtendedTagField & FeaturePtr->Flag);
  FeaturePtr->Flag &= (UINT8)DevCapPtr->DeviceCapability.Bits.ExtendedTagField;
  return;
}


/**
  Set Extended Tag Field in capabilities structure in memory.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciUstrUpdateExtendedTagField (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_EXTTAGFIELD_INF *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  USRA_ADDRESS                     DevAddr;

  if (Phase != PciCapPhase1) {
    //
    // This capability is applied in the second hierarchy scan.
    //
    return;
  }
  if (RootInfoPtr->Attribute & PCIE_LIB_ATTR_ENDPOINT) {
    //
    // Set Extended Tag only for rootport with endpoint conntected. This policy handle hot-plug.
    // If OS does not re-evaluate path support it stays at safe default.
    //
    if (DevCapPtr->DeviceControl.Bits.ExtendedTagField != FeaturePtr->Flag) {

      PCIDEBUG ("%02X:%02X:%02X.%X: U%d DevCtl.ExtendedTag %d -> %d\n",
                DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
                Phase, DevCapPtr->DeviceControl.Bits.ExtendedTagField, FeaturePtr->Flag);
      DevCapPtr->DeviceControl.Bits.ExtendedTagField = FeaturePtr->Flag;
      USRA_ADDRESS_COPY (&DevAddr, DevAddrPtr);
      DevAddr.Attribute.AccessWidth = UsraWidth16;
      DevAddr.Attribute.AddrType = AddrTypePCIE;
      DevAddr.Pcie.Offset += OFFSET_OF (PCI_CAPABILITY_PCIEXP, DeviceControl);
      RegisterWrite (&DevAddr, &DevCapPtr->DeviceControl);
    }
  }
  return;
}


/**
  Update 10-Bit Tag for rootport hierarchy according to device capabilities.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciDstrUpdate10BitTagField (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_10BIT_TAG_INF   *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  UINT8  Supported;

  if (Phase != PciCapPhase0) {
    //
    // This capability is analyzed in the first hierarchy scan.
    //
    return;
  }
  switch (DevCapPtr->Capability.Bits.DevicePortType) {
    case PCIE_DEVICE_PORT_TYPE_UPSTREAM_PORT:
    case PCIE_DEVICE_PORT_TYPE_DOWNSTREAM_PORT:
      //
      // Switches that lack 10-Bit Tag Completer capability are still able to forward NPRs and Completions carrying
      // 10-Bit Tags correctly, since the two new Tag bits are in TLP Header bits that were formerly Reserved, and
      // Switches are required to forward Reserved TLP Header bits without modification. However, if such a Switch
      // detects an error with an NPR carrying a 10-Bit Tag, and that Switch handles the error by acting as the
      // Completer for the NPR, the resulting Completion will have an invalid 10-Bit Tag. Thus, it is strongly
      // recommended that Switches between any components using 10-Bit Tags support 10-Bit Tag Completer capability.
      //
      Supported = (UINT8)DevCapPtr->DeviceCapability2.Bits.TenBitTagCompleterSupported;
      break;
    default:
      Supported = (UINT8)DevCapPtr->DeviceCapability2.Bits.TenBitTagRequesterSupported;
      break;
  }
  PCIDEBUG ("%02X:%02X:%02X.%X: D%d DevCap2.10BitTag %d & %d -> %d\n",
            DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
            Phase, Supported, FeaturePtr->Flag, Supported & FeaturePtr->Flag);
  FeaturePtr->Flag &= Supported;
  return;
}


/**
  Set 10-bit Tag Field in capabilities structure in memory.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciUstrUpdate10BitTagField (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_10BIT_TAG_INF   *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  USRA_ADDRESS                     DevAddr;

  if (Phase != PciCapPhase1) {
    //
    // This capability is applied in the second hierarchy scan.
    //
    return;
  }
  if (RootInfoPtr->Attribute & PCIE_LIB_ATTR_ENDPOINT) {
    //
    // Set 10-Bit Tag only for rootport with endpoint conntected. This policy handle hot-plug.
    // If OS does not re-evaluate path support it stays at safe default.
    //
    if (DevCapPtr->DeviceControl2.Bits.TenBitTagRequesterEnable != FeaturePtr->Flag) {

      PCIDEBUG ("%02X:%02X:%02X.%X: U%d DevCtl2.TenBitTagRequester %d -> %d\n",
                DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
                Phase, DevCapPtr->DeviceControl2.Bits.TenBitTagRequesterEnable, FeaturePtr->Flag);
      DevCapPtr->DeviceControl2.Bits.TenBitTagRequesterEnable = FeaturePtr->Flag;
      USRA_ADDRESS_COPY (&DevAddr, DevAddrPtr);
      DevAddr.Attribute.AccessWidth = UsraWidth16;
      DevAddr.Attribute.AddrType = AddrTypePCIE;
      DevAddr.Pcie.Offset += OFFSET_OF (PCI_CAPABILITY_PCIEXP, DeviceControl2);
      RegisterWrite (&DevAddr, &DevCapPtr->DeviceControl2);
    }
  }
  return;
}


/**
  Update Atomic Operation Routing for root port hierarchy according to device capabilities.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciDstrUpdateAtomicOpRouting (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_ATOMICOP_INF    *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  if (Phase != PciCapPhase0) {
    //
    // This capability is analyzed in the first hierarchy scan.
    //
    return;
  }
  switch (DevCapPtr->Capability.Bits.DevicePortType) {
    case PCIE_DEVICE_PORT_TYPE_ROOT_PORT:
    case PCIE_DEVICE_PORT_TYPE_UPSTREAM_PORT:
    case PCIE_DEVICE_PORT_TYPE_DOWNSTREAM_PORT:
      PCIDEBUG ("%02X:%02X:%02X.%X: D%d DevCap2.AtomicOpRouting %d & %d -> %d\n",
                DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
                Phase, DevCapPtr->DeviceCapability2.Bits.AtomicOpRouting, FeaturePtr->Flag,
                DevCapPtr->DeviceCapability2.Bits.AtomicOpRouting & FeaturePtr->Flag);
      FeaturePtr->Flag &= (UINT8)DevCapPtr->DeviceCapability2.Bits.AtomicOpRouting;
      break;
    default:
      break;
  }
  return;
}


/**
  Set AtomicOp Routing to the endpoint and up stream according to feature node.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciUstrUpdateAtomicOpRouting (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_ATOMICOP_INF    *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  USRA_ADDRESS                     DevAddr;
  BOOLEAN                          Changed = FALSE;

  if (Phase != PciCapPhase1) {
    //
    // This capability is applied in the second hierarchy scan.
    //
    return;
  }
  switch (DevCapPtr->Capability.Bits.DevicePortType) {
    case PCIE_DEVICE_PORT_TYPE_ROOT_PORT:
    case PCIE_DEVICE_PORT_TYPE_PCIE_ENDPOINT:
    case PCIE_DEVICE_PORT_TYPE_LEGACY_PCIE_ENDPOINT:
      //
      // Set AtomicOpRequester according to the AtomicOpRouting capability in this hierarchy.
      // AtomicOpRequester applies only to root port and endpoint, hardwired to 0 for other.
      //
      PCIDEBUG ("%02X:%02X:%02X.%X: U%d DevCtl2.AtomicOpRequester %d -> %d\n",
                DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
                Phase, DevCapPtr->DeviceControl2.Bits.AtomicOpRequester, FeaturePtr->Flag);
      DevCapPtr->DeviceControl2.Bits.AtomicOpRequester = FeaturePtr->Flag;
      Changed = TRUE;
      break;
    default:
      break;
  }
  if (!FeaturePtr->Flag && DevCapPtr->DeviceCapability2.Bits.AtomicOpRouting) {
    //
    // For bridge that supports AtomicOpRouting, if AtomicOpRouting is not supported in all bridges
    // in this hierarchy, enable blocking atomic operations in these that support routing.
    //
    PCIDEBUG ("%02X:%02X:%02X.%X: U%d DevCtl2.AtomicOpEgressBlocking %d -> 1\n",
              DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
              Phase, DevCapPtr->DeviceControl2.Bits.AtomicOpEgressBlocking);
    DevCapPtr->DeviceControl2.Bits.AtomicOpEgressBlocking = 1;
    Changed = TRUE;
  }
  if (Changed) {

    USRA_ADDRESS_COPY (&DevAddr, DevAddrPtr);
    DevAddr.Attribute.AccessWidth = UsraWidth16;
    DevAddr.Attribute.AddrType = AddrTypePCIE;
    DevAddr.Pcie.Offset += OFFSET_OF (PCI_CAPABILITY_PCIEXP, DeviceControl2);
    RegisterWrite (&DevAddr, &DevCapPtr->DeviceControl2);
  }
  return;
}


/**
  Update Latency Tolerance Reporting Mechanism for root port hierarchy according to device capabilities.

  Enable LTR in hierarchy starting at root until encounter bridge that does not support LTR mechanism.
  Disable in all devices starting at the not supporting bridge and downstream.

  NOTE: For a Multi-Function Device associated with an Upstream Port of a device that implements LTR,
        the bit in Function 0 is RW, and only Function 0 controls the component’s Link behavior.
        In all other Functions of that device, this bit is RsvdP.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciDstrUpdateLtr (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_LTR_INF         *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  USRA_ADDRESS                     DevAddr;

  if (DevAddrPtr->Pcie.Func > 0) {
    //
    // Only function zero controls LTR.
    //
    return;
  }
  if (Phase == PciCapPhase0) {

    switch (DevCapPtr->Capability.Bits.DevicePortType) {

      case PCIE_DEVICE_PORT_TYPE_ROOT_PORT:
      case PCIE_DEVICE_PORT_TYPE_UPSTREAM_PORT:
      case PCIE_DEVICE_PORT_TYPE_DOWNSTREAM_PORT:
        PCIDEBUG ("%02X:%02X:%02X.%X: D%d DevCap2.LtrMechanism %d & %d -> %d\n",
                  DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
                  Phase, DevCapPtr->DeviceCapability2.Bits.LtrMechanism, FeaturePtr->Flag,
                  DevCapPtr->DeviceCapability2.Bits.LtrMechanism & FeaturePtr->Flag);
        FeaturePtr->Flag &= (UINT8)DevCapPtr->DeviceCapability2.Bits.LtrMechanism;
        break;

      default:
        break;
    }
    if (DevCapPtr->DeviceControl2.Bits.LtrMechanism != (FeaturePtr->Flag & DevCapPtr->DeviceCapability2.Bits.LtrMechanism)) {

      PCIDEBUG ("%02X:%02X:%02X.%X: D%d DevCtl2.LtrMechanism %d -> %d\n",
                DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
                Phase, DevCapPtr->DeviceControl2.Bits.LtrMechanism,
                FeaturePtr->Flag & DevCapPtr->DeviceCapability2.Bits.LtrMechanism);
      DevCapPtr->DeviceControl2.Bits.LtrMechanism = (FeaturePtr->Flag & DevCapPtr->DeviceCapability2.Bits.LtrMechanism);
      USRA_ADDRESS_COPY (&DevAddr, DevAddrPtr);
      DevAddr.Attribute.AccessWidth = UsraWidth16;
      DevAddr.Attribute.AddrType = AddrTypePCIE;
      DevAddr.Pcie.Offset += OFFSET_OF (PCI_CAPABILITY_PCIEXP, DeviceControl2);
      RegisterWrite (&DevAddr, &DevCapPtr->DeviceControl2);
    }
  }
  return;
}


/**
  LTR is configured in downstream.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciUstrUpdateLtr (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_LTR_INF         *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  return;
}


/**
  Update Max Payload Size info for rootport hierarchy according to device capabilities.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciDstrUpdateMaxPayload (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_MAXPAYLOAD_INF  *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  if (Phase != PciCapPhase0) {
    //
    // This capability is analyzed in the first hierarchy scan.
    //
    return;
  }
  PCIDEBUG ("%02X:%02X:%02X.%X: D%d DevCap.MaxPayloadSize min(%d,%d) -> %d\n",
            DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
            Phase, DevCapPtr->DeviceCapability.Bits.MaxPayloadSize, FeaturePtr->MaxPayload,
            MIN (FeaturePtr->MaxPayload, (UINT16)DevCapPtr->DeviceCapability.Bits.MaxPayloadSize));
  FeaturePtr->MaxPayload = MIN (FeaturePtr->MaxPayload, (UINT16)DevCapPtr->DeviceCapability.Bits.MaxPayloadSize);
  return;
}


/**
  Set Max Payload Size in capabilities structure in memory.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciUstrUpdateMaxPayload (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_MAXPAYLOAD_INF  *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  USRA_ADDRESS                     DevAddr;

  if (Phase != PciCapPhase1) {
    //
    // This capability is applied in the second hierarchy scan.
    //
    return;
  }
  if (RootInfoPtr->Attribute & PCIE_LIB_ATTR_ENDPOINT) {
    //
    // Set MPS only for rootport with endpoint conntected. This policy handle hot-plug.
    // If OS does not re-evaluate path MPS it stays at safe default 128 bytes.
    //
    if (DevCapPtr->DeviceControl.Bits.MaxPayloadSize != FeaturePtr->MaxPayload) {

      PCIDEBUG ("%02X:%02X:%02X.%X: U%d DevCtl.MaxPayloadSize %d -> %d\n",
                DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
                Phase, DevCapPtr->DeviceControl.Bits.MaxPayloadSize, FeaturePtr->MaxPayload);
      DevCapPtr->DeviceControl.Bits.MaxPayloadSize = FeaturePtr->MaxPayload;
      USRA_ADDRESS_COPY (&DevAddr, DevAddrPtr);
      DevAddr.Attribute.AccessWidth = UsraWidth16;
      DevAddr.Attribute.AddrType = AddrTypePCIE;
      DevAddr.Pcie.Offset += OFFSET_OF (PCI_CAPABILITY_PCIEXP, DeviceControl);
      RegisterWrite (&DevAddr, &DevCapPtr->DeviceControl);
    }
  }
  return;
}


/**
  Update MRRS for root port hierarchy according to device capabilities.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciDstrUpdateMaxReadRequestSize (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_MRRS_INF        *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  //
  // Nothing to do in this phase.
  //
  return;
}


/**
  Set Max Read Request Size to the endpoint and up stream according to feature node.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciUstrUpdateMaxReadRequestSize (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_MRRS_INF        *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  USRA_ADDRESS                     DevAddr;

  if (FeaturePtr->Flag == PCIE_LIB_MRRS_OVERRIDE) {
    //
    // Set Max Read Request Size.
    //
    if (DevCapPtr->DeviceControl.Bits.MaxReadRequestSize != FeaturePtr->MaxReadReqSize) {

      PCIDEBUG ("%02X:%02X:%02X.%X: U%d DevCtl.MaxReadRequestSize %d -> %d\n",
                DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
                Phase, DevCapPtr->DeviceControl.Bits.MaxReadRequestSize, FeaturePtr->MaxReadReqSize);
      DevCapPtr->DeviceControl.Bits.MaxReadRequestSize = FeaturePtr->MaxReadReqSize;
      USRA_ADDRESS_COPY (&DevAddr, DevAddrPtr);
      DevAddr.Attribute.AccessWidth = UsraWidth16;
      DevAddr.Attribute.AddrType = AddrTypePCIE;
      DevAddr.Pcie.Offset += OFFSET_OF (PCI_CAPABILITY_PCIEXP, DeviceControl);
      RegisterWrite (&DevAddr, &DevCapPtr->DeviceControl);
    }
  }
  return;
}


/**
  Update Relaxed Ordering for root port hierarchy according to device capabilities.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciDstrUpdateRelaxedOrdering (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_RELAXORDER_INF  *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  //
  // No additional checks required, policy value determines this capability.
  //
  return;
}


/**
  Pcie standard feature called Enable Relaxed Ordering(ENRO).

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciUstrUpdateRelaxedOrdering (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_RELAXORDER_INF  *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  USRA_ADDRESS                     DevAddr;

  switch (DevCapPtr->Capability.Bits.DevicePortType) {
    case PCIE_DEVICE_PORT_TYPE_LEGACY_PCIE_ENDPOINT:
    case PCIE_DEVICE_PORT_TYPE_PCIE_ENDPOINT:
    case PCIE_DEVICE_PORT_TYPE_ROOT_PORT:
    case PCIE_DEVICE_PORT_TYPE_UPSTREAM_PORT:
    case PCIE_DEVICE_PORT_TYPE_DOWNSTREAM_PORT:
      if (DevCapPtr->DeviceControl.Bits.RelaxedOrdering != FeaturePtr->Flag) {

        PCIDEBUG ("%02X:%02X:%02X.%X: U%d DevCtl.RelaxedOrdering %d -> %d\n",
                  DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
                  Phase, DevCapPtr->DeviceControl.Bits.RelaxedOrdering, FeaturePtr->Flag);
        DevCapPtr->DeviceControl.Bits.RelaxedOrdering = FeaturePtr->Flag;
        USRA_ADDRESS_COPY (&DevAddr, DevAddrPtr);
        DevAddr.Attribute.AccessWidth = UsraWidth16;
        DevAddr.Attribute.AddrType = AddrTypePCIE;
        DevAddr.Pcie.Offset += OFFSET_OF (PCI_CAPABILITY_PCIEXP, DeviceControl);
        RegisterWrite (&DevAddr, &DevCapPtr->DeviceControl);
      }
    default:
      break;
  }
  return;
}


/**
  Update No Snoop Capability for root port hierarchy according to device capabilities.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciDstrUpdateNoSnoop (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_NOSNOOP_INF     *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  //
  // No additional checks required, policy value determines this capability.
  //
  return;
}


/**
  Pcie standard feature called Enable No Snoop (ENOSNP)

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciUstrUpdateNoSnoop (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_NOSNOOP_INF     *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  USRA_ADDRESS                     DevAddr;

  switch (DevCapPtr->Capability.Bits.DevicePortType) {
    case PCIE_DEVICE_PORT_TYPE_LEGACY_PCIE_ENDPOINT:
    case PCIE_DEVICE_PORT_TYPE_PCIE_ENDPOINT:
      if (DevCapPtr->DeviceControl.Bits.NoSnoop != FeaturePtr->Flag) {
        PCIDEBUG ("%02X:%02X:%02X.%X: U%d DevCtl.NoSnoop %d -> %d\n",
                  DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
                  Phase, DevCapPtr->DeviceControl.Bits.NoSnoop, FeaturePtr->Flag);
        DevCapPtr->DeviceControl.Bits.NoSnoop = FeaturePtr->Flag;
        USRA_ADDRESS_COPY (&DevAddr, DevAddrPtr);
        DevAddr.Attribute.AccessWidth = UsraWidth16;
        DevAddr.Attribute.AddrType = AddrTypePCIE;
        DevAddr.Pcie.Offset += OFFSET_OF (PCI_CAPABILITY_PCIEXP, DeviceControl);
        RegisterWrite (&DevAddr, &DevCapPtr->DeviceControl);
      }
    default:
      break;
  }
  return;
}


/**
  Update ExtendedSynch for root port hierarchy according to device capabilities.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciDstrUpdateExtendedSynch (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_EXTSYNC_INF     *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  //
  // No additional checks required, plicy value determines this capability.
  //
  return;
}


/**
  Set Extended Synch feature according to configured policy.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciUstrUpdateExtendedSynch (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_EXTSYNC_INF     *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  USRA_ADDRESS                     DevAddr;

  if (DevCapPtr->LinkControl.Bits.ExtendedSynch != FeaturePtr->Flag) {

    PCIDEBUG ("%02X:%02X:%02X.%X: U%d LinkCtl.ExtendedSynch %d -> %d\n",
              DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
              Phase, DevCapPtr->LinkControl.Bits.ExtendedSynch, FeaturePtr->Flag);
    DevCapPtr->LinkControl.Bits.ExtendedSynch = FeaturePtr->Flag;
    USRA_ADDRESS_COPY (&DevAddr, DevAddrPtr);
    DevAddr.Attribute.AccessWidth = UsraWidth16;
    DevAddr.Attribute.AddrType = AddrTypePCIE;
    DevAddr.Pcie.Offset += OFFSET_OF (PCI_CAPABILITY_PCIEXP, LinkControl);
    RegisterWrite (&DevAddr, &DevCapPtr->LinkControl);
  }
  return;
}


/**
  Update PTM for root port hierarchy according to device capabilities.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciDstrUpdatePtm (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_PTM_INF         *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  USRA_ADDRESS                          DevAddr;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_PTM ExtCapPtm = {0};

  if (!FeaturePtr->Flag) {
    //
    // PTM already disabled for this hierarchy.
    //
    return;
  }
  if (Phase != PciCapPhase0) {
    //
    // This capability is applied in the first hierarchy scan.
    //
    return;
  }
  USRA_ADDRESS_COPY (&DevAddr, DevAddrPtr);

  DevAddr.PcieBlk.Offset = PciFindExtendedCapId (&DevAddr, PCI_EXPRESS_EXTENDED_CAPABILITY_PTM_ID);
  if (DevAddr.Pcie.Offset != 0) {

    DevAddr.Attribute.AccessWidth = UsraWidth32;
    DevAddr.Attribute.AddrType = AddrTypePCIEBLK;
    DevAddr.PcieBlk.Count = sizeof(ExtCapPtm) / sizeof(UINT32);
    RegisterRead (&DevAddr, &ExtCapPtm);
  }
  //
  // PTM can be enabled if upstream device is PTM capable, and
  // requester role is implemented in device.
  //
  switch (DevCapPtr->Capability.Bits.DevicePortType) {

    case PCIE_DEVICE_PORT_TYPE_ROOT_PORT:
      if (DevAddr.Pcie.Offset == 0 || !ExtCapPtm.Capability.Bits.RootCapable || !ExtCapPtm.Capability.Bits.ResponderCapable) {
        //
        // If rootport is not PTM capable mark PTM disabled for any downstream devices.
        //
        PCIDEBUG ("%02X:%02X:%02X.%X: D%d Rootport not PTM capable (offset %d, cap 0x%04X)\n",
                  DevAddr.Pcie.Seg, DevAddr.Pcie.Bus, DevAddr.Pcie.Dev, DevAddr.Pcie.Func,
                  Phase, DevAddr.Pcie.Offset, ExtCapPtm.Capability.Uint32);
        FeaturePtr->Flag = 0;
        return;
      }
      ExtCapPtm.Control.Bits.Enable = 1;
      ExtCapPtm.Control.Bits.RootSelect = 1;
      ExtCapPtm.Control.Bits.EffectiveGranularity = ExtCapPtm.Capability.Bits.LocalClockGranularity;
      FeaturePtr->UpStreamPtmCtrl.Uint32 = ExtCapPtm.Control.Uint32;
      break;

    case PCIE_DEVICE_PORT_TYPE_UPSTREAM_PORT:
      if (DevAddr.Pcie.Offset == 0 || !ExtCapPtm.Capability.Bits.RequesterCapable) {
        //
        // If upsteam port is not PTM capable, disable PTM for any downstream devices.
        //
        PCIDEBUG ("%02X:%02X:%02X.%X: D%d Upstream port not PTM capable (offset %d, cap 0x%04X)\n",
                  DevAddr.Pcie.Seg, DevAddr.Pcie.Bus, DevAddr.Pcie.Dev, DevAddr.Pcie.Func,
                  Phase, DevAddr.Pcie.Offset, ExtCapPtm.Capability.Uint32);
        FeaturePtr->Flag = 0;
        FeaturePtr->UpStreamPtmCtrl.Uint32 = 0;
        return;
      }
      ExtCapPtm.Control.Bits.Enable = 1;
      if (ExtCapPtm.Capability.Bits.LocalClockGranularity == 0 ||
          FeaturePtr->UpStreamPtmCtrl.Bits.EffectiveGranularity < ExtCapPtm.Capability.Bits.LocalClockGranularity) {

        FeaturePtr->UpStreamPtmCtrl.Bits.EffectiveGranularity = ExtCapPtm.Capability.Bits.LocalClockGranularity;
      }
      ExtCapPtm.Control.Bits.EffectiveGranularity = FeaturePtr->UpStreamPtmCtrl.Bits.EffectiveGranularity;
      break;

    case PCIE_DEVICE_PORT_TYPE_DOWNSTREAM_PORT:
      if (DevAddr.Pcie.Offset == 0 || !ExtCapPtm.Capability.Bits.ResponderCapable) {
        //
        // If downstream port is not PTM capable, disable PTM for any downstream devices.
        //
        PCIDEBUG ("%02X:%02X:%02X.%X: D%d Downstream port not PTM capable (offset %d, cap 0x%04X)\n",
                  DevAddr.Pcie.Seg, DevAddr.Pcie.Bus, DevAddr.Pcie.Dev, DevAddr.Pcie.Func,
                  Phase, DevAddr.Pcie.Offset, ExtCapPtm.Capability.Uint32);
        FeaturePtr->Flag = 0;
        FeaturePtr->UpStreamPtmCtrl.Uint32 = 0;
        return;
      }
      ExtCapPtm.Control.Bits.Enable = 1;
      if (ExtCapPtm.Capability.Bits.LocalClockGranularity == 0 ||
          FeaturePtr->UpStreamPtmCtrl.Bits.EffectiveGranularity < ExtCapPtm.Capability.Bits.LocalClockGranularity) {

        FeaturePtr->UpStreamPtmCtrl.Bits.EffectiveGranularity = ExtCapPtm.Capability.Bits.LocalClockGranularity;
      }
      ExtCapPtm.Control.Bits.EffectiveGranularity = FeaturePtr->UpStreamPtmCtrl.Bits.EffectiveGranularity;
      break;

    case PCIE_DEVICE_PORT_TYPE_PCIE_ENDPOINT:
    case PCIE_DEVICE_PORT_TYPE_LEGACY_PCIE_ENDPOINT:
      if (DevAddr.Pcie.Offset == 0 || !ExtCapPtm.Capability.Bits.RequesterCapable) {
        //
        // If endpoint is not PTM capable just skip this endpoint.
        //
        PCIDEBUG ("%02X:%02X:%02X.%X: D%d Endpoint not PTM capable (offset %d, cap 0x%04X)\n",
                  DevAddr.Pcie.Seg, DevAddr.Pcie.Bus, DevAddr.Pcie.Dev, DevAddr.Pcie.Func,
                  Phase, DevAddr.Pcie.Offset, ExtCapPtm.Capability.Uint32);
        return;
      }
      ExtCapPtm.Control.Bits.Enable = 1;
      ExtCapPtm.Control.Bits.EffectiveGranularity = FeaturePtr->UpStreamPtmCtrl.Bits.EffectiveGranularity;
      break;

    default:
      //
      // Other port types don't support PTM.
      //
      return;
  }
  PCIDEBUG ("%02X:%02X:%02X.%X: D%d Set PTM ctrl: 0x%04X (capability: 0x%04X)\n",
            DevAddr.Pcie.Seg, DevAddr.Pcie.Bus, DevAddr.Pcie.Dev, DevAddr.Pcie.Func,
            Phase, ExtCapPtm.Control.Uint32, ExtCapPtm.Capability.Uint32);
  RegisterWrite (&DevAddr, &ExtCapPtm);
  return;
} // PciDstrUpdatePtm()


/**
  Set PTM feature for root port.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciUstrUpdatePtm (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_PTM_INF         *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  //
  // Nothing to do, PTM was configured during downstream traverse.
  //
  return;
}


/**
  Update Completion Timeout for root port hierarchy according to device capabilities.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciDstrUpdateCto (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_CTO_INF         *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
 USRA_ADDRESS                     DevAddr;
 BOOLEAN                          Changed = FALSE;

 if (Phase != PciCapPhase0) {
   //
   // This capability is applied in the first hierarchy scan.
   //
   goto GetOut;
 }
 switch (FeaturePtr->Policy) {
   case PcieLibCompletionTO_Default:
     goto GetOut;
   case PcieLibCompletionTO_50_100us:
     if (!(DevCapPtr->DeviceCapability2.Bits.CompletionTimeoutRanges & BIT0)) { // Lower Range A
       goto GetOut;
     }
     break;
   case PcieLibCompletionTO_1_10ms:
     if (!(DevCapPtr->DeviceCapability2.Bits.CompletionTimeoutRanges & BIT0)) { // Upper Range A
       goto GetOut;
     }
     break;
   case PcieLibCompletionTO_16_55ms:
     if (!(DevCapPtr->DeviceCapability2.Bits.CompletionTimeoutRanges & BIT1)) { // Lower Range B
       goto GetOut;
     }
     break;
   case PcieLibCompletionTO_65_210ms:
     if (!(DevCapPtr->DeviceCapability2.Bits.CompletionTimeoutRanges & BIT1)) { // Upper Range B
       goto GetOut;
     }
     break;
   case PcieLibCompletionTO_260_900ms:
     if (!(DevCapPtr->DeviceCapability2.Bits.CompletionTimeoutRanges & BIT2)) { // Lower Range C
       goto GetOut;
     }
     break;
   case PcieLibCompletionTO_1_3P5s:
     if (!(DevCapPtr->DeviceCapability2.Bits.CompletionTimeoutRanges & BIT2)) { // Upper Range C
       goto GetOut;
     }
     break;
   case PcieLibCompletionTO_4_13s:
     if (!(DevCapPtr->DeviceCapability2.Bits.CompletionTimeoutRanges & BIT3)) { // Lower Range D
       goto GetOut;
     }
     break;
   case PcieLibCompletionTO_17_64s:
     if (!(DevCapPtr->DeviceCapability2.Bits.CompletionTimeoutRanges & BIT3)) { // Upper Range D
       goto GetOut;
     }
     break;
   case PcieLibCompletionTO_Disabled:
     if (DevCapPtr->DeviceCapability2.Bits.CompletionTimeoutDisable) {
       DevCapPtr->DeviceControl2.Bits.CompletionTimeoutDisable = 1;
       Changed = TRUE;
     }
     goto GetOut;
   default:
     DEBUG ((DEBUG_ERROR, "[PCI] ERROR: Unsupported Completion Timeout Range 0x%X requested for rootport %a\n",
             FeaturePtr->Policy, &RootInfoPtr->Name[0]));
     goto GetOut;
  }
  PCIDEBUG ("%02X:%02X:%02X.%X: U%d DevCtl2.CompletionTimeout %d -> %d\n",
           DevAddrPtr->Pcie.Seg, DevAddrPtr->Pcie.Bus, DevAddrPtr->Pcie.Dev, DevAddrPtr->Pcie.Func,
           Phase, DevCapPtr->DeviceControl2.Bits.CompletionTimeoutValue, FeaturePtr->Policy);
  DevCapPtr->DeviceControl2.Bits.CompletionTimeoutValue = FeaturePtr->Policy;
  Changed = TRUE;
 GetOut:
  if (Changed) {

    USRA_ADDRESS_COPY (&DevAddr, DevAddrPtr);
    DevAddr.Attribute.AccessWidth = UsraWidth16;
    DevAddr.Attribute.AddrType = AddrTypePCIE;
    DevAddr.Pcie.Offset += OFFSET_OF (PCI_CAPABILITY_PCIEXP, DeviceControl2);
    RegisterWrite (&DevAddr, &DevCapPtr->DeviceControl2);
  }
  return;
}


/**
  Set Completion Timeout according to policy and device capabilities.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to PCIe device header in config space.
  @param[in,out] DevCapPtr     - Pointer to PCIe capabilities structure.
**/
VOID
PciUstrUpdateCto (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_CTO_INF         *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  //
  // Nothing to do.
  //
  return;
}


/**
  Update Common Clock for root port hierarchy according to device capabilities.

  For multi-function devices common clock must be enabled in all functions, so
  this procedure is started only on function zero and is executed for all functions.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to current PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to current PCIe device header.
  @param[in,out] DevCapPtr     - Pointer to current PCIe device capabilities structure.
**/
VOID
PciDstrUpdateCommonClock (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_COMMONCLOCK_INF *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  USRA_ADDRESS                     DevAddr;
  USRA_ADDRESS                     ParAddr;
  PCI_CAPABILITY_PCIEXP            ParCap;
  PCI_CAPABILITY_PCIEXP            DevCap;

  if (Phase != PciCapPhase0) {
    //
    // This capability is applied in the first hierarchy scan.
    //
    return;
  }
  if (ParAddrPtr == NULL || !FeaturePtr->Flag || DevAddrPtr->Pcie.Func > 0) {
    return;
  }
  USRA_ADDRESS_COPY (&ParAddr, ParAddrPtr);
  ParAddr.PcieBlk.Offset = PciFindCapId (&ParAddr, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (ParAddr.Pcie.Offset == 0) {

    PCIDEBUG ("%02X:%02X:%02X.%X: D%d Capability %d not found\n",
              ParAddr.Pcie.Seg, ParAddr.Pcie.Bus, ParAddr.Pcie.Dev, ParAddr.Pcie.Func,
              Phase, EFI_PCI_CAPABILITY_ID_PCIEXP);
    return;
  }
  ParAddr.Attribute.AccessWidth = UsraWidth32;
  ParAddr.Attribute.AddrType = AddrTypePCIEBLK;
  ParAddr.PcieBlk.Count = sizeof(ParCap) / sizeof(UINT32);
  RegisterRead (&ParAddr, &ParCap);
  //
  // Check the Slot Clock Configuration bit in parent and child device.
  // If both use slot clock then set common clock bit for them and retrain.
  //
  if (!ParCap.LinkStatus.Bits.SlotClockConfiguration || !DevCapPtr->LinkStatus.Bits.SlotClockConfiguration) {

    PCIDEBUG ("%02X:%02X:%02X.%X: D%d CommonClock rejected (DevLnkSts 0x%X, Parent 0x%X)\n",
              ParAddr.Pcie.Seg, ParAddr.Pcie.Bus, ParAddr.Pcie.Dev, ParAddr.Pcie.Func,
              Phase, DevCapPtr->LinkStatus.Uint16, ParCap.LinkStatus.Uint16);
    return;
  }
  ParCap.LinkControl.Bits.CommonClockConfiguration = 1;
  ParAddr.Attribute.AccessWidth = UsraWidth16;
  ParAddr.Attribute.AddrType = AddrTypePCIE;
  DevAddr.Pcie.Offset += OFFSET_OF (PCI_CAPABILITY_PCIEXP, LinkControl);
  RegisterWrite (&ParAddr, &ParCap.LinkControl);

  DevCapPtr->LinkControl.Bits.CommonClockConfiguration = 1;
  USRA_ADDRESS_COPY (&DevAddr, DevAddrPtr);
  DevAddr.Attribute.AccessWidth = UsraWidth16;
  DevAddr.Attribute.AddrType = AddrTypePCIE;
  DevAddr.Pcie.Offset += OFFSET_OF (PCI_CAPABILITY_PCIEXP, LinkControl);
  RegisterWrite (&DevAddr, &DevCapPtr->LinkControl);

  if (DevHdrPtr->Device.Hdr.HeaderType & HEADER_TYPE_MULTI_FUNCTION) {
    //
    // Loop over all functions until function number wraps.
    // Also check for max to suppress false KW warning.
    //
    while (++DevAddr.PcieBlk.Func != 0 && DevAddr.PcieBlk.Func <= PCI_MAX_FUNC) {

      DevAddr.PcieBlk.Offset = PciFindCapId (&DevAddr, EFI_PCI_CAPABILITY_ID_PCIEXP);
      if (DevAddr.Pcie.Offset == 0) {
        break;
      }
      DevAddr.Pcie.Offset += OFFSET_OF (PCI_CAPABILITY_PCIEXP, LinkControl);
      RegisterRead (&DevAddr, &DevCap.LinkControl);
      DevCap.LinkControl.Bits.CommonClockConfiguration = 1;
      RegisterWrite (&DevAddr, &DevCap.LinkControl);
    }
  }
  //
  // After enabling CommonClock retrain.
  //
  PCIDEBUG ("%02X:%02X:%02X.%X: D%d Re-training link after CommonClock enabled\n",
            ParAddr.Pcie.Seg, ParAddr.Pcie.Bus, ParAddr.Pcie.Dev, ParAddr.Pcie.Func, Phase);
  ParCap.LinkControl.Bits.RetrainLink = 1;
  RegisterWrite (&ParAddr, &ParCap.LinkControl);
  //
  // Polling link training status till it is 0.
  // LinkStatus register is next to LinkControl in capabilities structure. Move offset by 2.
  //
  ParAddr.Pcie.Offset += sizeof (ParCap.LinkControl);
  do {
    RegisterRead (&ParAddr, &ParCap.LinkStatus);

  } while (ParCap.LinkStatus.Bits.LinkTraining);
  return;
} // PciDstrUpdateCommonClock()


/**
  Set common clock to the endpoint and parent according to feature node.

  @param[in]     RootInfoPtr   - Pointer to rootport info in PCI host.
  @param[in,out] FeaturePtr    - Pointer to rootport feature structure.
  @param[in]     ParAddrPtr    - Pointer to parent PCIe device address on PCI.
  @param[in]     DevAddrPtr    - Pointer to PCIe device address on PCI.
  @param[in]     DevHdrPtr     - Pointer to PCIe device header in config space.
  @param[in,out] DevCapPtr     - Pointer to PCIe capabilities structure.
**/
VOID
PciUstrUpdateCommonClock (
  IN     PCI_CAP_PHASE             Phase,
  IN     PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN OUT PCIE_LIB_COMMONCLOCK_INF *FeaturePtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr,
  IN OUT PCI_CAPABILITY_PCIEXP    *DevCapPtr
  )
{
  //
  // Nothing to check here, Common Clock is configured in Set phase.
  //
  return;
}


/**
  This function and its capability specific handler functions are called when
  traversing PCI hierarchy downstream. Depending on feature it may just check
  capabilities, or it may also set it if that better suits capability specification.
  PTM is an example of capability configured in downstream phase, while MaxPayloadSize
  is an example of capability configured in upstream phase.

  @param[in,out] RootInfoPtr - Pointer to root port info in PCI host.
  @param[in]     PcieAddrPtr - Pointer to USRA address of PCI device config space.
  @param[in]     DevHdrPtr   - Pointer to a PCI device header.

  @return Port type of device pointed by PcieAddrPtr is returnded, or
          PCIE_DEVICE_PORT_TYPE_INVALID if the device does not support PCIe capabilities.
**/
UINT8
PciDstrUpdateCapabilities (
  IN     PCI_CAP_PHASE             Phase,
  IN OUT PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr,
  IN     PCI_TYPE_GENERIC         *DevHdrPtr
  )
{
  USRA_ADDRESS                     DevAddr;
  PCI_CAPABILITY_PCIEXP            DevCap;
  UINT8                            Index;

  USRA_ADDRESS_COPY (&DevAddr, DevAddrPtr);

  DevAddr.PcieBlk.Offset = PciFindCapId (&DevAddr, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (DevAddr.Pcie.Offset == 0) {

    PCIDEBUG ("%02X:%02X:%02X.%X: Capability %d not found\n",
              DevAddr.Pcie.Seg, DevAddr.Pcie.Bus, DevAddr.Pcie.Dev, DevAddr.Pcie.Func, EFI_PCI_CAPABILITY_ID_PCIEXP);
    return PCIE_DEVICE_PORT_TYPE_INVALID;
  }
  DevAddr.Attribute.AccessWidth = UsraWidth32;
  DevAddr.Attribute.AddrType = AddrTypePCIEBLK;
  DevAddr.PcieBlk.Count = sizeof(DevCap) / sizeof(UINT32);
  RegisterRead (&DevAddr, &DevCap);

  PCIDEBUG ("%02X:%02X:%02X.%X: Phase%d downstream at device %04X:%04X PortType %d, cap offset %d\n",
            DevAddr.Pcie.Seg, DevAddr.Pcie.Bus, DevAddr.Pcie.Dev, DevAddr.Pcie.Func, Phase,
            DevHdrPtr->Device.Hdr.VendorId, DevHdrPtr->Device.Hdr.DeviceId,
            DevCap.Capability.Bits.DevicePortType, DevAddr.PcieBlk.Offset);

  switch (DevCap.Capability.Bits.DevicePortType) {
    case PCIE_DEVICE_PORT_TYPE_PCIE_ENDPOINT:
    case PCIE_DEVICE_PORT_TYPE_LEGACY_PCIE_ENDPOINT:
      RootInfoPtr->Attribute |= PCIE_LIB_ATTR_ENDPOINT; // Endpoint device present
      break;
    case PCIE_DEVICE_PORT_TYPE_ROOT_PORT:
      if (DevCap.SlotCapability.Bits.HotPlugCapable) {

        PCIDEBUG ("%02X:%02X:%02X.%X: Rootport is hot-plug capable\n",
                  DevAddr.Pcie.Seg, DevAddr.Pcie.Bus, DevAddr.Pcie.Dev, DevAddr.Pcie.Func);
        RootInfoPtr->Attribute |= PCIE_LIB_ATTR_HOTPLUG;
      }
      break;
    case PCIE_DEVICE_PORT_TYPE_ROOT_COMPLEX_INTEGRATED_ENDPOINT:
    case PCIE_DEVICE_PORT_TYPE_ROOT_COMPLEX_EVENT_COLLECTOR:
      //
      // Do not work on root complex functions, just skip. We are interested in PCH RP hierarchy.
      //
      return PCIE_DEVICE_PORT_TYPE_INVALID;
    default:
      break;
  }
  for (Index = 0;
       Index < PCIE_LIB_FEATURE_MAX && RootInfoPtr->FeatureList[Index].Common.Type < PCIE_LIB_FEATURE_MAX;
       Index++) {

    switch (RootInfoPtr->FeatureList[Index].Common.Type) {
      case PCIE_LIB_FEATURE_ASPM:
        PciDstrUpdateAspm (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].Aspm,
                           ParAddrPtr, &DevAddr, DevHdrPtr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_CTO:
        PciDstrUpdateCto (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].Cto,
                          ParAddrPtr, &DevAddr, DevHdrPtr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_MAXPAYLOAD:
        PciDstrUpdateMaxPayload (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].MaxPayload,
                                 ParAddrPtr, &DevAddr, DevHdrPtr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_COMMONCLOCK:
        PciDstrUpdateCommonClock (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].CommonClock,
                                  ParAddrPtr, &DevAddr, DevHdrPtr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_EXTTAGFIELD:
        PciDstrUpdateExtendedTagField (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].ExtendedTagField,
                                       ParAddrPtr, &DevAddr, DevHdrPtr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_ATOMICOP:
        PciDstrUpdateAtomicOpRouting (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].AtomicOp,
                                      ParAddrPtr, &DevAddr, DevHdrPtr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_LTR:
        PciDstrUpdateLtr (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].Ltr,
                          ParAddrPtr, &DevAddr, DevHdrPtr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_MRRS:
        PciDstrUpdateMaxReadRequestSize (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].Mrrs,
                                         ParAddrPtr, &DevAddr, DevHdrPtr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_RELAXORDER:
        PciDstrUpdateRelaxedOrdering (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].RelaxedOrder,
                                      ParAddrPtr, &DevAddr, DevHdrPtr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_NOSNOOP:
        PciDstrUpdateNoSnoop (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].NoSnoop,
                              ParAddrPtr, &DevAddr, DevHdrPtr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_EXTSYNC:
        PciDstrUpdateExtendedSynch (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].ExtendedSync,
                                    ParAddrPtr, &DevAddr, DevHdrPtr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_PTM:
        PciDstrUpdatePtm (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].Ptm,
                          ParAddrPtr, &DevAddr, DevHdrPtr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_10BIT_TAG:
        PciDstrUpdate10BitTagField (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].TenBitTag,
                                    ParAddrPtr, &DevAddr, DevHdrPtr, &DevCap);
        break;
      default:
        break; // Not interested in this type, just skip
    }
  }
  if (RootInfoPtr->FeatureList[Index].Common.Type != PCIE_LIB_FEATURE_MAX) {

    DEBUG ((DEBUG_ERROR, "[PCI] ERROR: Rootport '%a' feature list ends at unexpected type %d (entry no %d)\n",
           RootInfoPtr->Name, RootInfoPtr->FeatureList[Index].Common.Type, Index));
  }
  return (UINT8)DevCap.Capability.Bits.DevicePortType;
} // PciDstrUpdateCapabilities()


/**
  This function and its capability specific handler functions are called after all PCI
  hierarchy has been traversed downstream and recursive calls return upstream.
  Depending on feature its handler usually sets capabilities, but may also just do nothing.
  PTM is an example of capability configured in downstream phase, while MaxPayloadSize
  is an example of capability configured in upstream phase.

  @param[in] RootInfoPtr - Pointer to root port info in PCI host.
  @param[in] ParAddrPtr  - Pointer to USRA address of parent PCI device, or NULL if no parent.
  @param[in] DevAddrPtr  - Pointer to USRA address of PCI device config space.
**/
VOID
PciUstrUpdateCapabilities (
  IN     PCI_CAP_PHASE             Phase,
  IN OUT PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *DevAddrPtr
  )
{
  USRA_ADDRESS                     DevAddr;
  PCI_TYPE_GENERIC                 DevHdr;
  PCI_CAPABILITY_PCIEXP            DevCap;
  UINT8                            Index;

  USRA_ADDRESS_COPY (&DevAddr, DevAddrPtr);
  DevAddr.Attribute.AccessWidth = UsraWidth32;
  DevAddr.Attribute.AddrType = AddrTypePCIEBLK;
  DevAddr.PcieBlk.Count = sizeof(DevHdr) / sizeof(UINT32);
  DevAddr.PcieBlk.Offset = 0;
  RegisterRead (&DevAddr, &DevHdr);

  DevAddr.Attribute.AccessWidth = UsraWidth32;
  DevAddr.Attribute.AddrType = AddrTypePCIEBLK;
  DevAddr.PcieBlk.Count = sizeof(DevCap) / sizeof(UINT32);
  DevAddr.PcieBlk.Offset = PciFindCapId (&DevAddr, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (DevAddr.PcieBlk.Offset == 0) {
    return;
  }
  RegisterRead (&DevAddr, &DevCap);

  PCIDEBUG ("%02X:%02X:%02X.%X: Phase%d upstream at device %04X:%04X PortType %d, cap offset %d\n",
            DevAddr.Pcie.Seg, DevAddr.Pcie.Bus, DevAddr.Pcie.Dev, DevAddr.Pcie.Func, Phase,
            DevHdr.Device.Hdr.VendorId, DevHdr.Device.Hdr.DeviceId,
            DevCap.Capability.Bits.DevicePortType, DevAddr.PcieBlk.Offset);
  for (Index = 0;
       Index < PCIE_LIB_FEATURE_MAX && RootInfoPtr->FeatureList[Index].Common.Type < PCIE_LIB_FEATURE_MAX;
       Index++) {

    switch (RootInfoPtr->FeatureList[Index].Common.Type) {
      case PCIE_LIB_FEATURE_ASPM:
        PciUstrUpdateAspm (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].Aspm,
                           ParAddrPtr, &DevAddr, &DevHdr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_CTO:
        PciUstrUpdateCto (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].Cto,
                          ParAddrPtr, &DevAddr, &DevHdr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_MAXPAYLOAD:
        PciUstrUpdateMaxPayload (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].MaxPayload,
                                 ParAddrPtr, &DevAddr, &DevHdr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_COMMONCLOCK:
        PciUstrUpdateCommonClock (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].CommonClock,
                                  ParAddrPtr, &DevAddr, &DevHdr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_EXTTAGFIELD:
        PciUstrUpdateExtendedTagField (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].ExtendedTagField,
                                       ParAddrPtr, &DevAddr, &DevHdr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_ATOMICOP:
        PciUstrUpdateAtomicOpRouting (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].AtomicOp,
                                      ParAddrPtr, &DevAddr, &DevHdr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_LTR:
        PciUstrUpdateLtr (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].Ltr,
                          ParAddrPtr, &DevAddr, &DevHdr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_MRRS:
        PciUstrUpdateMaxReadRequestSize (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].Mrrs,
                                         ParAddrPtr, &DevAddr, &DevHdr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_RELAXORDER:
        PciUstrUpdateRelaxedOrdering (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].RelaxedOrder,
                                      ParAddrPtr, &DevAddr, &DevHdr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_NOSNOOP:
        PciUstrUpdateNoSnoop (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].NoSnoop,
                              ParAddrPtr, &DevAddr, &DevHdr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_EXTSYNC:
        PciUstrUpdateExtendedSynch (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].ExtendedSync,
                                    ParAddrPtr, &DevAddr, &DevHdr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_PTM:
        PciUstrUpdatePtm (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].Ptm,
                          ParAddrPtr, &DevAddr, &DevHdr, &DevCap);
        break;
      case PCIE_LIB_FEATURE_10BIT_TAG:
        PciUstrUpdate10BitTagField (Phase, RootInfoPtr, &RootInfoPtr->FeatureList[Index].TenBitTag,
                                    ParAddrPtr, &DevAddr, &DevHdr, &DevCap);
        break;
      default:
        break; // Not interested in this type, just skip
    } // switch (Feature type)
  } // for (Index...)
  return;
}


/**
  Initialize rootport in a PCI host (stack) and its downstream devices.

  @param[in,out] HostInfoPtr  - Pointer to PCI host (aka stack) info structure.
  @param[in,out] RootInfoPtr  - Pointer to rootport info in PCI host.
  @param[in]     ParAddrPtr   - Pointer to parent address when function called recursively, NULL for first call.
  @param[in]     BrAddrPtr    - Pointer to bridge address when function called recursively, NULL for first call.
  @param[in]     CleanTmpFlag - True to clean bus assigned and restore original values,
                                False to keep assigned bus numbers.
**/
VOID
PciInitBridgeDownstreamDevices (
  IN     PCI_CAP_PHASE             Phase,
  IN OUT PCIE_LIB_HOST_BRIDGE_INF *HostInfoPtr,
  IN OUT PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *BrAddrPtr,
  IN     BOOLEAN                   CleanTmpBus
  )
{
  USRA_ADDRESS                     BrAddr;
  USRA_ADDRESS                     DevAddr;
  PCI_TYPE_GENERIC                 DevHdr;
  USRA_PCIE_ADDR_TYPE              PciDevTable[(PCI_MAX_DEVICE + 1) * (PCI_MAX_FUNC + 1)];
  UINT16                           PciDevTableLen = 0;
  UINT8                            PciDev;
  UINT8                            PciFun;
  UINT8                            BridgePortType;
  UINT8                            OrgSecondaryBus = 0;
  UINT8                            OrgSubordinateBus = 0;

  static UINT8                     NestedLevel = 0;

  //
  // The rootport can be DMI or PCI. For PCI rootport assign secondary bus and enumerate downstream devices.
  // For DMI root start enumeration at the root bus, becaue DMI stack does not have rootport.
  // Also DMI can be endpoint device in root bus, or can be not present in PCI config space at all.
  //
  if (BrAddrPtr == NULL) {

    ASSERT (RootInfoPtr->NestedLevel == 0);
    USRA_PCIE_SEG_ADDRESS (BrAddr, UsraWidth32, RootInfoPtr->Segment, RootInfoPtr->Bus, RootInfoPtr->Dev, RootInfoPtr->Fun, 0);

  } else {

    USRA_ADDRESS_COPY (&BrAddr, BrAddrPtr);
  }
  if (PciIsDevicePresent (&BrAddr, &DevHdr) && IS_PCI_BRIDGE (&DevHdr.Device)) {
    //
    // This is bridge or rootport, so assign secondary and subordinate bus numbers and
    // start enumeration at the secondary bus.
    //
    if (HostInfoPtr->BusBase + HostInfoPtr->BusUsedNum > HostInfoPtr->BusLimit) {

      DEBUG ((DEBUG_ERROR, "[PCI] WARNING: %02X:%02X:%02X.%X: Bus number shortage (got %02X..%02X, used %X), root init failed\n",
              BrAddr.Pcie.Seg, BrAddr.Pcie.Bus, BrAddr.Pcie.Dev, BrAddr.Pcie.Func,
              HostInfoPtr->BusBase, HostInfoPtr->BusLimit, HostInfoPtr->BusUsedNum));
      HostInfoPtr->Attribute |= PCIE_LIB_ATTR_BUS_SHORTAGE;
      return;
    }
    OrgSecondaryBus = DevHdr.Bridge.Bridge.SecondaryBus;
    OrgSubordinateBus = DevHdr.Bridge.Bridge.SubordinateBus;
    BrAddr.Pcie.Offset = PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET;
    BrAddr.Attribute.AccessWidth = UsraWidth32;
    BrAddr.Attribute.S3Enable = !CleanTmpBus;
    DevHdr.Bridge.Bridge.SecondaryBus = HostInfoPtr->BusBase + HostInfoPtr->BusUsedNum++;
    DevHdr.Bridge.Bridge.SubordinateBus = HostInfoPtr->BusLimit;
    RegisterWrite (&BrAddr, &DevHdr.Bridge.Bridge.PrimaryBus);

    PCIDEBUG ("%02X:%02X:%02X.%X: Using tmp bus secondary: %02X, subordinate: %02X\n",
              BrAddr.Pcie.Seg, BrAddr.Pcie.Bus, BrAddr.Pcie.Dev, BrAddr.Pcie.Func,
              DevHdr.Bridge.Bridge.SecondaryBus, DevHdr.Bridge.Bridge.SubordinateBus);
    //
    // Prepare parent address for devices found on secondary bus of this bridge.
    //
    BrAddrPtr = &BrAddr;
    //
    // Check for capabilities update and store the function address in a table of devices where capability changes shall be applied.
    //
    BridgePortType = PciDstrUpdateCapabilities (Phase, RootInfoPtr, ParAddrPtr, &BrAddr, &DevHdr);

  } else {

    if (BrAddrPtr != NULL || RootInfoPtr->Type != PCIE_LIB_DMI) {

      DEBUG ((DEBUG_WARN, "[PCI] WARNING: %02X:%02X:%02X.%X does not exist\n",
              BrAddr.Pcie.Seg, BrAddr.Pcie.Bus, BrAddr.Pcie.Dev, BrAddr.Pcie.Func));
      RootInfoPtr->Valid = FALSE;
      return;
    }
    BridgePortType = PCIE_DEVICE_PORT_TYPE_INVALID;  // This is not bridge port, it is DMI root endpoint
  }
  //
  // Enumerate the secondary bus.
  //
  USRA_PCIE_SEG_ADDRESS (DevAddr, UsraWidth16, RootInfoPtr->Segment, HostInfoPtr->BusBase + HostInfoPtr->BusUsedNum - 1, 0, 0, 0);
  for (PciDev = 0; PciDev <= PCI_MAX_DEVICE; PciDev++) {

    DevAddr.PcieBlk.Dev = PciDev;
    for (PciFun = 0; PciFun <= PCI_MAX_FUNC; PciFun++) {

      DevAddr.PcieBlk.Func = PciFun;
      if (!PciIsDevicePresent (&DevAddr, &DevHdr)) {

        if (PciFun != 0) {
          continue; // go to next function
        } else {
          break;    // if no function 0 then go to next device
        }
      }
      if (IsSiliconWorkaroundEnabled ("S2007749696")) {
        //
        // snr_sighting S2007749132: [SNR A0-A1][PO] PCIe Rootport allows transactions with Device number different
        //                           than 0 passthrough on ARI Non Capable devices
        // [Cloned to S2007749696]: [SNR PO] [SNR A0-A1 WA] PCIe Rootport allows transactions with Device number
        //                          different than 0 passthrough on ARI Non Capable devices
        // ARI WA: Skip enumeration for End-Point  non-zero Pcie and non-ARI capable devices
        // Explained below four conditions need to be satisfied in order to determine the silicon bug has been met,
        // and Intel enumeration and other configuration should be skipped.
        //
        // Condition 1: The Child device is different than zero. Device 1-31
        //
        if (DevAddr.Pcie.Dev != 0) {
          //
          // Condition 2: The Child device is not ARI capable
          //
          UINT16 ChildAriCapPtr = PciFindExtendedCapId (&DevAddr, PCI_EXPRESS_EXTENDED_CAPABILITY_ARI_CAPABILITY_ID);
          //
          // Determine Device Port type of End Point
          //
          if (ChildAriCapPtr == 0) { // If it is not ARI capable
            //
            // Condition 3: The Parent device is a PCIE root port (Type 4)
            //
            if (BridgePortType == PCIE_DEVICE_PORT_TYPE_ROOT_PORT) {
              //
              // Condition 4: The Child device is one of the below device port type.
              // PCIE_DEVICE_PORT_TYPE_PCIE_ENDPOINT (Type 0). It is an End-point.
              // PCIE_DEVICE_PORT_TYPE_LEGACY_PCIE_ENDPOINT (Type 1). It is an End-point.
              // PCIE_DEVICE_PORT_TYPE_UPSTREAM_PORT (Type 5)
              // PCIE_DEVICE_PORT_TYPE_PCIE_TO_PCI_BRIDGE (Type 7)
              //
              PCI_REG_PCIE_CAPABILITY ChildCap;
              DevAddr.Attribute.AccessWidth = UsraWidth16;
              DevAddr.Pcie.Offset = PciFindCapId (&DevAddr, EFI_PCI_CAPABILITY_ID_PCIEXP) + OFFSET_OF(PCI_CAPABILITY_PCIEXP, Capability);
              RegisterRead (&DevAddr, &ChildCap.Uint16);
              if (ChildCap.Bits.DevicePortType == PCIE_DEVICE_PORT_TYPE_PCIE_ENDPOINT ||
                  ChildCap.Bits.DevicePortType == PCIE_DEVICE_PORT_TYPE_LEGACY_PCIE_ENDPOINT ||
                  ChildCap.Bits.DevicePortType == PCIE_DEVICE_PORT_TYPE_UPSTREAM_PORT ||
                  ChildCap.Bits.DevicePortType == PCIE_DEVICE_PORT_TYPE_PCIE_TO_PCI_BRIDGE) {
                //
                // If all the conditions are met, then skip enumeration of current function
                //
                DEBUG ((DEBUG_INFO, "[PCI] %02X:%02X:%02X.%X: ARI WA conditions met for %04X:%04X - skipping\n",
                        DevAddr.Pcie.Seg, DevAddr.Pcie.Bus, DevAddr.Pcie.Dev, DevAddr.Pcie.Func,
                        DevHdr.Device.Hdr.VendorId, DevHdr.Device.Hdr.DeviceId));
                break;
              }
            }
          }
        }
      } // IsSiliconWorkaroundEnabled()
      //
      // If it is a bridge run this function recursively for the bridge.
      //
      if (IS_PCI_BRIDGE (&DevHdr.Device)) {

        if (NestedLevel < FixedPcdGet32 (PcdMaxNestedLevel)) {
          //
          // Limit number of recursive calls to not exhaust stack.
          //
          if (++NestedLevel > RootInfoPtr->NestedLevel) {
            RootInfoPtr->NestedLevel = NestedLevel;
          }
          PciInitBridgeDownstreamDevices (Phase, HostInfoPtr, RootInfoPtr, BrAddrPtr, &DevAddr, CleanTmpBus);
          NestedLevel--;

        } else {

          DEBUG ((DEBUG_ERROR, "[PCI] ERROR: %02X:%02X:%02X.%X: Bridge %04X:%04X exceeds PcdMaxNestedLevel (%d) - skip\n",
                  DevAddr.Pcie.Seg, DevAddr.Pcie.Bus, DevAddr.Pcie.Dev, DevAddr.Pcie.Func,
                  DevHdr.Device.Hdr.VendorId, DevHdr.Device.Hdr.DeviceId, FixedPcdGet32 (PcdMaxNestedLevel)));
        }
      } else {
        //
        // Check for capabilities update and store the function address in a table of devices where capability changes shall be applied.
        //
        if (PciDstrUpdateCapabilities (Phase, RootInfoPtr, BrAddrPtr, &DevAddr, &DevHdr) != PCIE_DEVICE_PORT_TYPE_INVALID) {

          PciDevTable[PciDevTableLen++] = DevAddr.Pcie;
        }
      }
      if (PciFun == 0 && !(DevHdr.Device.Hdr.HeaderType & HEADER_TYPE_MULTI_FUNCTION)) {
        //
        // No more functions on this device break the function loop, go to next PciDev.
        //
        break;
      }
    } // for (PciFun...)
  } // for (PciDev...)
  //
  // Now set the capabilities for devices found on current bus.
  //
  for (PciDev = 0; PciDev < PciDevTableLen; PciDev++) {

    USRA_PCIE_SEG_ADDRESS (DevAddr, UsraWidth16, PciDevTable[PciDev].Seg,
                           PciDevTable[PciDev].Bus, PciDevTable[PciDev].Dev, PciDevTable[PciDev].Func, 0);
    PciUstrUpdateCapabilities (Phase, RootInfoPtr, BrAddrPtr, &DevAddr);
  }
  if (BridgePortType != PCIE_DEVICE_PORT_TYPE_INVALID) {

    PciUstrUpdateCapabilities (Phase, RootInfoPtr, ParAddrPtr, &BrAddr);
  }
  //
  // If it was bridge then update the bus numbers - either update subordinate bus, or restore original numbers.
  //
  if (PciIsDevicePresent (&BrAddr, &DevHdr) && IS_PCI_BRIDGE (&DevHdr.Device)) {

    BrAddr.Attribute.AccessWidth = UsraWidth32;
    BrAddr.Pcie.Offset = PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET;
    if (!CleanTmpBus) {
      //
      // Fix the subordinate bus
      //
      DevHdr.Bridge.Bridge.SubordinateBus = HostInfoPtr->BusBase + HostInfoPtr->BusUsedNum - 1;

    } else {
      //
      // Restore the original bus numbers.
      //
      DevHdr.Bridge.Bridge.SecondaryBus = OrgSecondaryBus;
      DevHdr.Bridge.Bridge.SubordinateBus = OrgSubordinateBus;
    }
    RegisterWrite (&BrAddr, &DevHdr.Bridge.Bridge.PrimaryBus);
  }
  return;
}

