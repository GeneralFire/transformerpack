/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

//
// Do not include any of this functionality in sim builds
//

#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/Pci22.h>

#include <Upi/KtiHost.h>
#include <Library/IioAccessLib.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioInitLib.h>
#include <Library/IioPcieInit.h>
#include <Library/IioNtbInit.h>
#include <Library/IioVmdInit.h>
#include <Library/OemIioInit.h>
#include <Library/PmcLib.h>
#include <PchReservedResources.h>
#include <Register/PchRegsPcr.h>
#include <Register/PchRegsPmc.h>
#include "IioEarlyInit.h"
#include <RcRegs.h>
#include <Fpga.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/X2ApicIdDetectLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/KtiApi.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/IioSysInfoLib.h>
#include <Library/IioDmiLib.h>
#include <Library/PeiServicesLib.h>
#include <PcieRegs.h>
#include <IioSetupDefinitions.h>
#include <Library/IioMs2iosfInit.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/BasicIpBlockServicesLib.h>
#include <Library/PcuIpLib.h>
#include <Library/OobIpLib.h>
#include <Ppi/IioSiPolicyPpi.h>
#include <Library/EnhancedWarningLogLib.h>

#include <Guid/MrcPlatformHooksGuid.h>
#include <Library/PchMultiPch.h>
#ifndef CDF_SC_FLAG
#include <Ppi/PchInit.h>
#endif
#include <Guid/PlatformInfo.h>

#include <LibraryPrivate/IioDebug.h>

/**********************************************************************************
 * Local definitions.                                                             *
 **********************************************************************************/
#ifndef BCTRL_IIO_PCIE_REG
//
// In SPR header this register is called BCTL_IIO_PCIE_G5_REG.
// In TNR header this register is called BCTL_IIO_PCIEDMI_REG.
// In others BCTRL_IIO_PCIE_REG.
//
#ifdef SPR_HOST
  #define BCTRL_IIO_PCIE_REG       BCTL_IIO_PCIE_G5_REG
#elif  TNR_HOST
  #define BCTRL_IIO_PCIE_REG       BCTL_IIO_PCIEDMI_REG
#else
  #define BCTRL_IIO_PCIE_REG       BCTRL_IIO_PCIE_REG
#endif
#endif

#ifndef BCTRL_IIO_PCIE_STRUCT
//
// In SPR header this struct is called BCTL_IIO_PCIE_G5_STRUCT.
// In TNR header this struct is called BCTL_IIO_PCIEDMI_STRUCT.
// In others BCTRL_IIO_PCIE_STRUCT.
//
#ifdef SPR_HOST
  #define BCTRL_IIO_PCIE_STRUCT    BCTL_IIO_PCIE_G5_STRUCT
#elif  TNR_HOST
  #define BCTRL_IIO_PCIE_STRUCT    BCTL_IIO_PCIEDMI_STRUCT
#else
  #define BCTRL_IIO_PCIE_STRUCT    BCTRL_IIO_PCIE_STRUCT
#endif
#endif

EFI_PEI_PPI_DESCRIPTOR mIioEarlyInitSignalPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gIioEarlyInitSignalPpiGuid,
  NULL
};

/**********************************************************************************
 * Functions.                                                                     *
 **********************************************************************************/
/**
   Initialize the EWL_ENTRY_CONTEXT member of an Enhanced
   Warning Log (EWL) entry.

   For now all fields are set to 0xff.
 */
VOID
InitEwlContextHeader (
  IN OUT EWL_ENTRY_CONTEXT *Context
  )
{
  ASSERT (Context != NULL);
  if (Context == NULL) {
    return;
  }

  Context->MajorCheckpoint = 0xff;
  Context->MinorCheckpoint = 0xff;
  Context->MajorWarningCode = 0xff;
  Context->MinorWarningCode = 0xff;
}


/**
   Initialize EWL_ENTRY_HEADER that is common for all Enhanced
   Warning Logs (EWL).

   @param Header    Pointer to header to be initialized
   @param Type      The type of the EWL entry being generated
   @param Size      The size of the EWL entry being generated
   @param Severity  The severity of the EWL entry being
                    generated
 */
VOID
InitEwlEntryHeader (
  IN OUT  EWL_ENTRY_HEADER *Header,
  IN      EWL_TYPE         Type,
  IN      UINT16           Size,
  IN      EWL_SEVERITY     Severity
  )
{
  ASSERT (Header != NULL);
  if (Header == NULL) {
    return;
  }

  Header->Type = Type;
  Header->Size = Size;
  Header->Severity = Severity;
}


/**
  Initialize the LinkDescription field for an Enhanced Warning Log (EWL) entry type 8 or 9.

  @param  LinkDesciption  pointer to the data structure to be initialized
  @param  IioIndex        index to IIO (socket number)
  @param  Stack           stack on the socket where the port
                          with issues resides
  @param  PortIndex       the port number for which the
                          LinkDescription is being initialized
*/
VOID
InitIioLinkDescriptor (
  OUT EWL_IIO_LINK_DESCRIPTION  *LinkDescription,
  IN  UINT8                     IioIndex,
  IN  UINT8                     Stack,
  IN  UINT8                     PortIndex
  )
{
  // These asserts provide quick diagnosis that the caller passed in garbage
  // the if statement below does the error handling.
  ASSERT (LinkDescription != NULL);
  if (LinkDescription == NULL) {
    return;
  }

  LinkDescription->Socket = IioIndex;
  LinkDescription->Stack = Stack;
  LinkDescription->Port = PortIndex;

  return;
}

/**
  Generate and log and Enhanced Warning Log entry for a PCIe port that
  is not configured for maximum link speed.

  @param[in] Socket        The socket/package/die containing the degraded port
  @param[in] Stack         The stack on the socket containing the degraded port
  @param[in] StackPort     The port on the stack which is degraded
  @param[in] VendorId      Vendor's ID
  @param[in] DeviceId      Device's ID
  @param[in] ExpLinkSpeed  Expected Link Speed value
  @param[in] ActLinkSpeed  Actual Link Speed value
**/
VOID
LogSpeedDegradation (
  IN UINT8                 Socket,
  IN UINT8                 Stack,
  IN UINT8                 StackPort,
  IN UINT16                VendorId,
  IN UINT16                DeviceId,
  IN UINT8                 ExpLinkSpeed,
  IN UINT8                 ActLinkSpeed
  )
{
  EWL_ENTRY_TYPE9          Type9;

  InitEwlEntryHeader (&Type9.Header, EwlType9, sizeof(Type9), EwlSeverityWarning);
  InitEwlContextHeader (&Type9.Context);
  InitIioLinkDescriptor (&Type9.LinkDescription, Socket, Stack, StackPort);

  Type9.LinkDescription.DidVid = DeviceId << 16 | VendorId;
  Type9.ExpectedLinkSpeed = ExpLinkSpeed;
  Type9.ActualLinkSpeed = ActLinkSpeed;

  EwlLogEntry (&Type9.Header);
}

/**
  Generate and log and Enhanced Warning Log entry for a PCIe port that
  is not configured for maximum link width.

  @param[in] Socket        The socket/package/die containing the degraded port
  @param[in] Stack         The stack on the socket containing the degraded port
  @param[in] StackPort     The port on the stack which is degraded
  @param[in] VendorId      Vendor's ID
  @param[in] DeviceId      Device's ID
  @param[in] ExpLinkWidth  Expected Link Width value
  @param[in] ActLinkWidth  Actual Link Width value
**/
VOID
LogWidthDegradation (
  IN UINT8                 Socket,
  IN UINT8                 Stack,
  IN UINT8                 StackPort,
  IN UINT16                VendorId,
  IN UINT16                DeviceId,
  IN UINT8                 ExpLinkWidth,
  IN UINT8                 ActLinkWidth
  )
{
  EWL_ENTRY_TYPE8          Type8;

  InitEwlEntryHeader (&Type8.Header, EwlType8, sizeof(Type8), EwlSeverityWarning);
  InitEwlContextHeader (&Type8.Context);
  InitIioLinkDescriptor (&Type8.LinkDescription, Socket, Stack, StackPort);

  Type8.LinkDescription.DidVid = DeviceId << 16 | VendorId;
  Type8.ExpectedLinkWidth = ExpLinkWidth;
  Type8.ActualLinkWidth = ActLinkWidth;

  EwlLogEntry (&Type8.Header);
}

VOID
IioPreLinkDataPcieInfoInit (
  IIO_GLOBALS  *IioGlobalData
  )
{
  EFI_STATUS              Status;
  INTEL_DMI_PCIE_INFO    *PcieInfoDefaultsTable;

  Status = GetPcieInfoDefaultsTable (&PcieInfoDefaultsTable);
  ASSERT_EFI_ERROR (Status);
  CopyMem (&(IioGlobalData->IioVar.PreLinkData.PcieInfo), PcieInfoDefaultsTable, sizeof (INTEL_DMI_PCIE_INFO));
 }

/**
  Initialize IIO Global Data

  @param IioGlobalData    Pointer to IIO_GLOBALS
  @param IioIndex         Index to Socket
--*/
VOID
InitializeIioGlobalDataPerSocket (
  IN OUT IIO_GLOBALS *IioGlobalData,
  IN UINT8            IioIndex
  )
{
  UINT8               PortIndex;
  UINT8               StackIndex;
  UINT8               DmiBus;
  UINT8               MaxStackPort;
  UINT8               Port;
  UINT8               MaxStackSocket;
  KTI_STACK_RESOURCE  StackResource;
  KTI_CPU_RESOURCE    SocketResource;
  UINT32              VtdDmiBarWidth = 0;
  IIO_HCX_TYPE        HcxType;
#if !defined(SKX_HOST) && !defined(SPR_HOST)
  SAD_CONTROL_IIO_VTD_STRUCT    SadControlIio10nm;
#endif
  UINT8               NumberOfPch = 1;
  EFI_HOB_GUID_TYPE   *GuidHob;
  EFI_PLATFORM_INFO   *PlatformInfo;
#ifndef CDF_SC_FLAG
  EFI_STATUS          Status;
  PCH_INIT_PPI        *PchInitPpi;

  Status = PeiServicesLocatePpi (&gPchInitPpiGuid, 0, NULL, (VOID**)&PchInitPpi);
  if (EFI_ERROR (Status)) {
    IIO_D_ERR ("%a: Locating gPchInitPpiGuid failed (%r)\n", __FUNCTION__, Status);
    ASSERT_EFI_ERROR (Status);
    return;
  }
#endif
  //
  // Enable MultiPCH if platform support it
  //
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob != NULL) {
    PlatformInfo  = GET_GUID_HOB_DATA(GuidHob);
    NumberOfPch = PlatformInfo->MaxNumOfPchs;
  }
  IioGlobalData->IioVar.IioVData.MultiPch = NumberOfPch > 1 ? 1 : 0;

  GetSocketResouce (IioIndex, &SocketResource);
  //
  // Get MS2IOSF Stack enabled bit map from CAPID_10 register
  //
  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL) ||
      IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL) ||
      IsCpuAndRevision (CPU_ICXD, REV_ALL)
      ) {
    IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] = SocketResource.stackPresentBitmap;
  } else {
    IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] = PcuGetMs2IosfStacksEn (IioIndex);
  }
  IIO_D_DBG ("[%d] StackPresentBitmap: 0x%04X (KTI: 0x%04X)\n", IioIndex,
             IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex], SocketResource.stackPresentBitmap);

  IioGlobalData->IioVar.IioVData.SocketBaseBusNumber[IioIndex] = SocketResource.BusBase;
  IioGlobalData->IioVar.IioVData.SocketLimitBusNumber[IioIndex] = SocketResource.BusLimit;
  IioGlobalData->IioVar.IioVData.SegmentSocket[IioIndex] = GetSegmentNum (IioIndex);
  IioGlobalData->IioVar.IioVData.PciResourceMem32Base[IioIndex] = SocketResource.MmiolBase;
  IioGlobalData->IioVar.IioVData.PciResourceMem32Limit[IioIndex] =  SocketResource.MmiolLimit;

  MaxStackSocket = GetMaxStackNumPerSocket ();
  for (StackIndex = 0; StackIndex < MaxStackSocket; StackIndex++) {

    GetStackResouce(IioIndex, StackIndex, &StackResource);
    if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << StackIndex))) {
      //
      // Initialize Ports with the Stack Index even due is not active.
      //
      MaxStackPort = GetMaxPortNumPerStack (StackIndex);
      if (MaxStackPort == 0) {
        continue;
      }
      for (PortIndex = 0; PortIndex < MaxStackPort; PortIndex++) {
        Port = GetPortIndexbyStack (StackIndex, PortIndex);
        IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex][Port] = StackIndex;
        IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex][Port] = StackResource.BusBase;
      }
      continue;
    }
    IioGlobalData->IioVar.IioVData.SocketStackBus[IioIndex][StackIndex] = StackResource.BusBase;
    IioGlobalData->IioVar.IioVData.SocketStackBaseBusNumber[IioIndex][StackIndex] = StackResource.BusBase;
    IioGlobalData->IioVar.IioVData.SocketStackLimitBusNumber[IioIndex][StackIndex] = StackResource.BusLimit;
    if ((((BIT0 << IioIndex) & GetPchPresentBitMap()) != 0) && (StackIndex == 0) && IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
      IioGlobalData->IioVar.IioVData.SocketStackMmiolBase[IioIndex][StackIndex] =
        StackResource.MmiolBase + DMI_RCRB_BAR_SIZE + GetDmiMemBarSize(IioIndex); // Reserve 8KB for DMI RCRB bar
    } else {
      IioGlobalData->IioVar.IioVData.SocketStackMmiolBase[IioIndex][StackIndex] = StackResource.MmiolBase;
    }
    IioGlobalData->IioVar.IioVData.SocketStackMmiolLimit[IioIndex][StackIndex] = StackResource.MmiolLimit;
    if ((IioIndex == 0) && (StackIndex == 0) && (IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_SNR, REV_ALL))){
      VtdDmiBarWidth = (V_VT_D_IIO_BAR_SIZE + V_RLINKEXPPTMBAR_SIZE)-1; // 40KB reserved in total, 8KB for Vtd and 32KB for EXPPTM bar
    } else {
      VtdDmiBarWidth = 0x3FFF; // 16KB reserved in total, 8KB for Vtd and 8KB for DMI bar
    }
    IioGlobalData->IioVar.IioVData.VtdBarAddress[IioIndex][StackIndex] = (StackResource.MmiolLimit) & (~VtdDmiBarWidth);
    if ((IioGlobalData->IioVar.IioVData.FpgaActive[IioIndex]) && (StackIndex == IIO_PSTACK3)) {
      IioGlobalData->IioVar.IioVData.VtdBarAddress[IioIndex][StackIndex] = (StackResource.MmiolLimit - FPGA_PREALLOCATE_MEM_SIZE) & (~VtdDmiBarWidth);
    }
    IioGlobalData->IioVar.IioVData.IoApicBase[IioIndex][StackIndex] = StackResource.IoApicBase;
    IioGlobalData->IioVar.IioVData.StackPciResourceMem32Limit[IioIndex][StackIndex] = StackResource.MmiolLimit;
    IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] = StackResource.Personality;

#if !defined(SKX_HOST) && !defined(SPR_HOST)
    if (IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
     SadControlIio10nm.Data = IioReadCpuCsr32 (IioIndex, StackIndex, SAD_CONTROL_IIO_VTD_REG);
     IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex] = (UINT8)SadControlIio10nm.Bits.local_stackid;
    }
#endif
#if defined(SPR_HOST)
    IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex] = StackIndex;  // Use StackIndex as StackId for SPR
#endif

    MaxStackPort = GetMaxPortNumPerStack (StackIndex);
    IIO_D_DBG ("MaxStackPort = %x\n", MaxStackPort);
    if (MaxStackPort == 0) {
      continue;
    }
    for(PortIndex = 0; PortIndex < MaxStackPort; PortIndex++) {
      Port = GetPortIndexbyStack (StackIndex, PortIndex);
      IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex][Port] = StackIndex;
      IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex][Port] = StackResource.BusBase;
      IIO_D_DBG ("SocketPortBusNumber = %x, Stack = %x, PortIndex = %x\n", IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex][Port], IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex][Port], Port);
    }

    if ((IioIndex == SOCKET_0_INDEX) && IsDmiStack(StackIndex)) {
      IioGlobalData->IioVar.IioVData.RcBaseAddress = IioGlobalData->IioVar.IioVData.VtdBarAddress[IioIndex][StackIndex] + V_VT_D_IIO_BAR_SIZE;

      IioGlobalData->IioVar.IioVData.PchIoApicBase = IioGlobalData->IioVar.IioVData.IoApicBase[IioIndex][StackIndex];

      DmiBus =  IioGlobalData->IioVar.IioVData.SocketStackBus[0][StackIndex];
      //
      // Update the global data structure with the system configuration data
      //
      IioGlobalData->IioVar.IioVData.PmBase[IioIndex] = PmcGetAcpiBase ();
      IioGlobalData->IioVar.IioVData.PchSegRegBaseAddress[IioIndex] = PCH_PCR_BASE_ADDRESS;
    }  // Socket0

    if ((IioGlobalData->IioVar.IioVData.MultiPch != 0) && (IioIndex != SOCKET_0_INDEX) && IsDmiStack (StackIndex)) {
      IioGlobalData->IioVar.IioVData.PmBase[IioIndex] = 0;
      // Setup MMIO base for Non-Legacy PCH at the end of MMIO stack, also account VT-d MMIO region to prevent overlap each over
      // MMIO base must be aligned to 16MB for PCH Sideband MMIO
      IioGlobalData->IioVar.IioVData.PchSegRegBaseAddress[IioIndex] = ( (StackResource.MmiolLimit + 1) - (NON_LEGACY_PCH_MMIO_SIZE + VtdDmiBarWidth + 1) ) & 0xFF000000;
    }
  }

  if (!IsCpuAndRevision (CPU_SKX, REV_ALL) && !IsCpuAndRevision (CPU_CLX, REV_ALL) && !IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    GetStackResouce (IioIndex, UBOX_STACK, &StackResource);
    IioGlobalData->IioVar.IioVData.SocketUncoreBusNumber [IioIndex][UNCORE_STACK0] = StackResource.BusBase;
    IioGlobalData->IioVar.IioVData.SocketUncoreBusNumber [IioIndex][UNCORE_STACK1] = StackResource.BusLimit;
  }

#ifndef CDF_SC_FLAG
  //
  // Set MMIO base for PCH
  //
  PchInitPpi->PchSetMmioBase (IioIndex, IioGlobalData->IioVar.IioVData.PchSegRegBaseAddress[IioIndex]);
#endif
  //
  // Get uplink Pcie Port information
  //
  IioOtherVariableInit (IioGlobalData, IioIndex);
  IioPreLinkDataPcieInfoInit (IioGlobalData);

  //
  // Get HCX sub-system type
  //
  HcxType = GetHcxType (IioIndex);
  IIO_D_LOG ("HcxType[%d] = %a\n",
            IioIndex,
            ((HcxType == IIO_HCA) ? "HCA" :
            (HcxType == IIO_HCB) ? "HCB" :
            (HcxType == IIO_HCX_NONE) ? "NONE" : "Invalid"));
  GetSysCpuCsrAccessVar ()->HcxType[IioIndex] = (UINT8) HcxType;
}


/**
  Disable unused PI5 IP

  @param IioGlobalData    Pointer to IIO_GLOBALS
  @param IioIndex         Index to Socket

  @retval IIO_SUCCESS Initialization completed without problems
--*/
IIO_STATUS
Pi5IpDisable (
  IN OUT IIO_GLOBALS      *IioGlobalData,
  IN UINT8                IioIndex
  )
{
  EFI_STATUS          Status = EFI_SUCCESS;
  UINT8               MaxIioStack;
  UINT8               Stack;
  PI5_MODES           StackMode;
  UINT8               Pi5Protocol;

  if (!IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    return Status;
  }

  MaxIioStack = GetMaxIouNumPerSocket () + 1; // PCIE + DMI
  for (Stack = 0; Stack < MaxIioStack; Stack++) {
    if ((IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][Stack] != TYPE_UBOX_IIO)) {
      continue;
    }

    StackMode = GetStackMode (IioGlobalData, IioIndex, Stack);
    switch (StackMode) {
    case PI5_MODE_DMI:
    case PI5_MODE_GEN4_ONLY:
      Pi5Protocol = PI5_IP_DISABLE_CONFIG_DMI_PCIEGEN4_ONLY_VALUE;
      break;
    case PI5_MODE_NTB:
    case PI5_MODE_GEN5_AND_GEN4:
      Pi5Protocol = PI5_IP_DISABLE_CONFIG_SINGLE_PROTOCOL_VALUE;
      break;
    case PI5_MODE_IAL:
      Pi5Protocol = PI5_IP_DISABLE_CONFIG_MUTI_PROTOCOL_VALUE;
      break;
    default:
      Pi5Protocol = PI5_IP_DISABLE_CONFIG_MUTI_PROTOCOL_VALUE;
      break;
    }
    SetPi5ProtocolStatus (IioIndex, Stack, Pi5Protocol);
    Status = PcuSetPi5IpDisable (IioIndex, Stack, (UINT32) Pi5Protocol);
  }

  return Status;
}


/**
  Execute the Phase 1 and 2 of IIO Ports initialization

  @retval IIO_SUCCESS Initialization completed without problems
  @retval EFI_NOT_FOUND failed to locate IIO silicon policy ppi guid; initialization skipped/aborted.
--*/
IIO_STATUS
IioEarlyLinkInit (
  VOID
  )
{
  UINT8               IioIndex;
  IIO_GLOBALS         *IioGlobalData;
  IIO_SI_POLICY_PPI   *IioSiPolicyPpi;
  EFI_STATUS          Status;

  IioGlobalData = NULL;
  IioSiPolicyPpi = NULL;

  Status = PeiServicesLocatePpi (&gIioSiPolicyPpiGuid, 0, NULL, (VOID **)&IioSiPolicyPpi);
  if (EFI_ERROR (Status)) {
    IIO_D_ERR ("%a: Locating gIioSiPolicyPpiGuid failed (%r)\n", __FUNCTION__, Status);
    ASSERT_EFI_ERROR (Status);
    return EFI_NOT_FOUND;
  }
  IioGlobalData = IioSiPolicyPpi->IioGlobalData;

  //
  // Initialize Local IioGlobalData with Host->var.iio values
  //
  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {

    if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
      continue;
    }
    //
    // init IIO global data structure
    //
    InitializeIioGlobalDataPerSocket (IioGlobalData, IioIndex);

    //
    // Disable VMD for rootports init
    //
    if (IsVmd20Supported ()) {
      //
      // This is not necessary for 14nm programs
      // as there rootports are hidden later in DXE
      //
      IioDisableVmdForInit (IioIndex);
    }

    //
    // Program MS2IOSF credits.
    // BIOS only programs credits when warm reset is not still issued
    //
    if (CheckCurrentReset (POST_RESET_POWERGOOD)) {
      ProgramMs2IosfCredits (IioGlobalData, IioIndex);
    } else if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_C0)) {
      //
      // Always trigger HW_INIT for autonomous IOSF credit initialization to
      // begin credit exchange even in warm boot path.
      // It's also called in cold boot or no warm reset path by ProgramMs2IosfCredits()
      //
      TriggerCreditsHwInit (IioGlobalData, IioIndex);
    }

    //
    // Program bifurcation control private sideband register (PPBIFCTL_PRIV).
    // In order to access Pcie Configuration Space and set Controller lane select before PI5 IP enable/disable
    //
    PcieProgramBifurcationPerSocket (IioGlobalData, IioIndex);

    //
    // Disable unused PI5 IP
    //
    Pi5IpDisable (IioGlobalData, IioIndex);

    if (GetSysResetRequired () == 0) {
      //
      // Program MS2IOSF bank decoders
      //
      ProgramMs2IosfBankDecoder (IioGlobalData, IioIndex);

      //
      // Early NTB init prior to link train (if any)
      //
      NtbEarlyInit (IioGlobalData, IioIndex);

      //
      // Scan and record the PCIe port visible
      //
      IioPreLinkDataInitPerSocket (IioGlobalData, IioIndex);
      OverrideMs2iosfBankDecoder (IioGlobalData, IioIndex);

      if (IioGlobalData->SetupData.PeciInTrustControlBit) {
        //
        // After credits and bank decoders are initalized we can enable PECI in OOB-MSM.
        //
        OobSetPeciTrusted (IioIndex);
      }
    }
    //
    // Even when IIO RC needs to be skipped, we still want to execute
    // bank programming routines of MS2IOSF
    //
    if (IioGlobalData->SetupData.DfxIioRcFlow == DFX_IIO_RC_FLOW_DISABLE) {
      continue;
    }
    //
    // Update our PCIEXpress configuration in our status information
    // loop through 10 PCIE ports that correspond to the three IOUs
    //
    IioUpdatePcieConfigurationData (IioGlobalData, IioIndex);

  } // completion of IIO Global data for all IIO sockets

  if (CheckCurrentReset (POST_RESET_POWERGOOD)) {
    //
    // Only need to program on cold boot path before warm reset.
    // Currently only needed for ICX-LBG platform.
    // This should be moved to PCH code.
    //
    if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
      ProgramCustomPchCredits ();
    }
  }

  if (IioGlobalData->SetupData.DfxIioRcFlow == DFX_IIO_RC_FLOW_DISABLE) {
    //
    // Update Host->var.iio values with IioGlobalData
    //
    OemPublishOpaSocketMapHob (IioGlobalData);

    return IIO_SUCCESS;
  }
  //
  // FSP API mode WA for simics booting.
  //
  IioGlobalData->IioVar.IioVData.Emulation = GetEmulation ();

  //
  // Execute IIO Configuration only if not reset is required.
  // with this we avoid to configure IIO in the first boot after flash BIOS
  // ICX-D & SNR doesn't need this step
  //
  if (GetSysResetRequired () == 0) {
    IIO_D_LOG ("Calling IioEarlyIntiazeEntry Start\n");
    IioEarlyInitializeEntry (IioGlobalData);

    Status = PeiServicesInstallPpi (&mIioEarlyInitSignalPpi);
    if (EFI_ERROR (Status)) {
      IIO_D_LOG ("\tmIioEarlyInitSignalPpi was NOT installed (gIioEarlyInitSignalPpiGuid = %g)\n", &gIioEarlyInitSignalPpiGuid);
      ASSERT_EFI_ERROR (Status);
    } else {
      IIO_D_DBG ("\tmIioEarlyInitSignalPpi was installed (gIioEarlyInitSignalPpiGuid = %g)\n", &gIioEarlyInitSignalPpiGuid);
    }

    IIO_D_LOG ("Calling IioEarlyIntiazeEntry Stop\n");
  }
  //
  // Check if warm reset is required due to VMD changes and override
  // resetRequired Host variable.
  //
  if (IioGlobalData->IioVar.IioOutData.resetRequired ) {
    SetSysResetRequired (GetSysResetRequired () | POST_RESET_WARM);
  }
  //
  // Update Host->var.iio values with IioGlobalData
  //
  OemPublishOpaSocketMapHob (IioGlobalData);
  return IIO_SUCCESS;
}

/**
  This function will re-train selected PCIe device to reduced link width or link speed if it's needed.

  @param[in] IioGlobalData  - Pointer to IioGlobalData
  @param[in] IioIndex       - Socket ID
  @param[in] PortIndex      - Controller ID
**/
#define LINKTRAINING_TIME_OUT 5000
#define LINKTRAINING_DELAY    2
VOID
IioLinkReTrain (
  IN IIO_GLOBALS              *IioGlobalData,
  IN UINT8                    IioIndex,
  IN UINT8                    PortIndex
  )
{
  UINT8                       BusNum;
  UINT8                       DevNum;
  UINT8                       FuncNum;
  UINT8                       SegmentNum;
  UINT8                       PCIeCapOffset;
  UINT8                       LoopCount;
  UINT16                      TargetLinkSpeedStatus;
  UINT16                      CurrentLinkSpeedStatus;
  UINT16                      NegotiatedLinkWidthStatus;
  UINT32                      MaxLinkWidthStatus;
  UINT32                      SecBusResReg;
  UINT32                      WaitTime;
  EFI_STATUS                  Status;
  PCI_REG_PCIE_LINK_STATUS    LnkSts;
  BCTRL_IIO_PCIE_STRUCT       SecBusReset;

  IIO_D_PCIDBG ("[%d p%d] PCIe devices Link Re-train started\n", IioIndex, PortIndex);

  BusNum     = IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex][PortIndex];
  DevNum     = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Device;
  FuncNum    = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Function;
  SegmentNum = IioGlobalData->IioVar.IioVData.SegmentSocket[IioIndex];

  SecBusResReg = BCTRL_IIO_PCIE_REG;

  PCIeCapOffset = IioPCIeGetCapOffset (IioIndex, BusNum, DevNum, FuncNum, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (PCIeCapOffset == 0) {
    IIO_D_PCIERR ("%a: Can't find PCIeCapOffset for: %02X:%02X:%02X.%X!\n",
                  __FUNCTION__, SegmentNum, BusNum, DevNum, FuncNum);
    ASSERT (PCIeCapOffset != 0);
  } else {
    for (LoopCount = 0; LoopCount < 3; LoopCount++) {
      //
      // Set Secondary Bus Reset (SBR) Bit for re-trained link
      //
      SecBusReset.Data = IioReadCpuCsr16 (IioIndex, PortIndex, SecBusResReg);
      SecBusReset.Bits.sbr = 1;
      IioWriteCpuCsr16 (IioIndex, PortIndex, SecBusResReg, SecBusReset.Data);

      IioStall (5000);   // wait 5ms

      //
      // Deassert Secondary Bus Reset (SBR) Bit for re-trained link
      //
      SecBusReset.Data = IioReadCpuCsr16 (IioIndex, PortIndex, SecBusResReg);
      SecBusReset.Bits.sbr = 0;
      IioWriteCpuCsr16 (IioIndex, PortIndex, SecBusResReg, SecBusReset.Data);

      IioStall (100000); // wait 100ms

      //
      // Polling link training status till it is 0
      //
      WaitTime = LINKTRAINING_TIME_OUT;
      do {
        IioStall (LINKTRAINING_DELAY);
        LnkSts.Uint16 = IioPciExpressRead16 (IioIndex, BusNum, DevNum, FuncNum, PCIeCapOffset + R_PCIE_LSTS_OFFSET);
        WaitTime--;
      } while (LnkSts.Bits.LinkTraining && WaitTime > 0);

      //
      // Compare expected speed and width with current speed and width values
      //
      Status = IioGetPcieTargetLinkSpeed (IioIndex, PortIndex, &TargetLinkSpeedStatus);
      if (EFI_ERROR (Status)) {
        IIO_D_PCIWARN ("%02X:%02X:%02X.%X:\t\t Target Link Speed not found! (%r)\n",
                      SegmentNum, BusNum, DevNum, FuncNum, Status);
        return;
      }
      Status = IioGetPcieCurrentLinkSpeed (IioIndex, PortIndex, &CurrentLinkSpeedStatus);
      if (EFI_ERROR (Status)) {
        IIO_D_PCIWARN ("%02X:%02X:%02X.%X:\t\tCurrent Link Speed not found! (%r)\n",
                      SegmentNum, BusNum, DevNum, FuncNum, Status);
        return;
      }
      Status = IioGetPcieMaxLinkWidth (IioIndex, PortIndex, &MaxLinkWidthStatus);
      if (EFI_ERROR (Status)) {
        IIO_D_PCIWARN ("%02X:%02X:%02X.%X:\t\tMax Link Width not found! (%r)\n",
                      SegmentNum, BusNum, DevNum, FuncNum, Status);
        return;
      }
      Status = IioGetPcieNegotiatedLinkWidth (IioIndex, PortIndex, &NegotiatedLinkWidthStatus);
      if (EFI_ERROR (Status)) {
        IIO_D_PCIWARN ("%02X:%02X:%02X.%X:\t\tNegotiated Link Width not found! (%r)\n",
                      SegmentNum, BusNum, DevNum, FuncNum, Status);
        return;
      }
      if ((TargetLinkSpeedStatus == CurrentLinkSpeedStatus) &&
          (MaxLinkWidthStatus == NegotiatedLinkWidthStatus)) {
        IIO_D_PCIDBG ("Link Re-Trained successfully for: %02X:%02X:%02X.%X\n",
                     SegmentNum, BusNum, DevNum, FuncNum);
        return;
      }
    } // End of for (LoopCount < 3)

    //
    // If after 3 SBR attempts the link still isn’t at the expected speed, continue
    //
    if (TargetLinkSpeedStatus != CurrentLinkSpeedStatus) {
      IIO_D_PCIDBG ("Link Speed Re-Training failed for: %02X:%02X:%02X.%X\n",
                    SegmentNum, BusNum, DevNum, FuncNum);
    }
    if (MaxLinkWidthStatus != NegotiatedLinkWidthStatus) {
      IIO_D_PCIDBG ("Link Width Re-Training failed for: %02X:%02X:%02X.%X\n",
                    SegmentNum, BusNum, DevNum, FuncNum);
    }
  } // End of else (PCIeCapOffset != 0)

  return;
}

/**
  If there is a mismatch between the value expected and actual in lane width and lane speed,
  the issue is logged it in EWL (Enhanced Warning Log) structure.

  @param[in] IioGlobalData        Pointer to IIO Universal Data Structure (UDS)
**/
VOID
IioPciePortEWL (
  IN  IIO_GLOBALS                *IioGlobalData
  )
{
  UINT8                           IioIndex;
  UINT8                           Stack;
  UINT8                           StackPort;
  UINT8                           PortIndex;
  UINT8                           SetupIndex;
  UINT8                           MaxPortNumberPerSocket;
  PCI_REG_PCIE_LINK_CONTROL2      RpLnkCtl2;
  PCI_REG_PCIE_LINK_CAPABILITY    RpLnkCap;
  PCI_REG_PCIE_LINK_STATUS        RpLnkSts;
  UINT8                           PCIeCapOffset;
  UINT8                           EpCapOffset;
  UINT8                           SegmentNum;
  UINT8                           BusNum;
  UINT8                           DevNum;
  UINT8                           FuncNum;
  UINT16                          VendorId;
  UINT16                          DeviceId;
  PCI_TYPE_GENERIC                DevHdr;
  PCI_REG_PCIE_LINK_CONTROL2      EpLnkCtl2;
  PCI_REG_PCIE_LINK_CAPABILITY    EpLnkCap;
  PCI_REG_PCIE_LINK_STATUS        EpLnkSts;
  UINT8                           BusBase;
  UINT8                           BusLimit;
  UINT8                           EpDev;
  UINT8                           OrgSecondaryBus = 0;
  UINT8                           OrgSubordinateBus = 0;

  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
    if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
      continue;
    }

    MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
    for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {

      SetupIndex = (IioIndex * MaxPortNumberPerSocket) + PortIndex;
      //
      // Check whether the PCIe port can be configured
      //
      if (IioGlobalData->SetupData.PEXPHIDE[SetupIndex]) {
        continue;   // Port is hidden
      }
      if (IioGlobalData->IioVar.IioOutData.PciePortConfig[SetupIndex] == 0) {
        continue;
      }
      if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {   // Skip DMI ports
        continue;
      }
      if (IsVMDEnabledForPort (IioIndex, PortIndex)) { // Skip ports assigned to VMD
        continue;
      }
      Stack      = IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex][PortIndex];
      BusNum     = IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex][PortIndex];
      DevNum     = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Device;
      FuncNum    = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Function;
      SegmentNum = IioGlobalData->IioVar.IioVData.SegmentSocket[IioIndex];

      VendorId = (UINT16)IioPciExpressRead16 (IioIndex, BusNum, DevNum, FuncNum, PCI_VENDOR_ID_OFFSET);
      DeviceId = (UINT16)IioPciExpressRead16 (IioIndex, BusNum, DevNum, FuncNum, PCI_DEVICE_ID_OFFSET);

      if (!IioIsDevicePresent (IioIndex, BusNum, DevNum, FuncNum, &DevHdr) || !IS_PCI_BRIDGE (&DevHdr.Device)) {
        IIO_D_PCIWARN ("Skipping IioPciePortEWL for IIO[%x] PCIE PortIndex:%x...\n", IioIndex, PortIndex);
        continue;
      }

      IIO_D_PCILOG ("IioPciePortEWL:%d Port:%d Bus:%x Dev:%d Func:%d Slot Power...\n",
        IioIndex, PortIndex, BusNum, DevNum, FuncNum);

      PCIeCapOffset = IioPCIeGetCapOffset (IioIndex, BusNum, DevNum, FuncNum, EFI_PCI_CAPABILITY_ID_PCIEXP);
      if (PCIeCapOffset == 0) {
        ASSERT (PCIeCapOffset != 0);
        continue;
      }
      RpLnkSts.Uint16  = IioPciExpressRead16 (IioIndex, BusNum, DevNum, FuncNum, PCIeCapOffset + R_PCIE_LSTS_OFFSET);
      RpLnkCtl2.Uint16 = IioPciExpressRead16 (IioIndex, BusNum, DevNum, FuncNum, PCIeCapOffset + R_PCIE_LCTL2_OFFSET);
      RpLnkCap.Uint32  = IioPciExpressRead32 (IioIndex, BusNum, DevNum, FuncNum, PCIeCapOffset + R_PCIE_LCAP_OFFSET);

      if ((RpLnkCtl2.Bits.TargetLinkSpeed == RpLnkSts.Bits.CurrentLinkSpeed) &&
          (RpLnkCap.Bits.MaxLinkWidth == RpLnkSts.Bits.NegotiatedLinkWidth)) {
        continue;
      }
      //
      // Basing on knob, check if port should be Re-Trained
      //
      if (IioGlobalData->SetupData.DfxLinkRetrainEn[SetupIndex]) {
        IioLinkReTrain (IioGlobalData, IioIndex, PortIndex);
      }
      //
      // Before printing EWL, verify if Speed or Width is really lower than supported by both sides.
      //
      BusBase  = IioGlobalData->IioVar.IioVData.SocketStackBaseBusNumber[IioIndex][Stack];
      BusLimit = IioGlobalData->IioVar.IioVData.SocketStackLimitBusNumber[IioIndex][Stack];

      //
      // This is bridge or rootport, so assign secondary and subordinate bus numbers and
      // start enumeration at the secondary bus.
      //
      if (BusBase >= BusLimit) {
        IIO_D_PCIWARN ("%02X:%02X:%02X.%X: Bus number shortage (got %02X..%02X), cannot verify endpoint\n",
                        SegmentNum, BusNum, DevNum, FuncNum, BusBase, BusLimit);
        continue;
      }

      OrgSecondaryBus = IioPciExpressRead8 (IioIndex, BusNum, DevNum, FuncNum, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
      OrgSubordinateBus = IioPciExpressRead8 (IioIndex, BusNum, DevNum, FuncNum, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET);

      IioPciExpressWrite8 (IioIndex, BusNum, DevNum, FuncNum, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, BusBase + 1);
      IioPciExpressWrite8 (IioIndex, BusNum, DevNum, FuncNum, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, BusLimit);

      IIO_D_PCIDBG ("%02X:%02X:%02X.%X: EWL: Using tmp bus secondary: %02X, subordinate: %02X\n",
                    SegmentNum, BusNum, DevNum, FuncNum, BusBase + 1, BusLimit);

      //
      // Enumerate the secondary bus.
      //
      for (EpDev = 0; EpDev <= PCI_MAX_DEVICE; EpDev++) {

        if (!IioIsDevicePresent (IioIndex, BusBase + 1, EpDev, 0, NULL)) {
          continue;
        }

        EpCapOffset = IioPCIeGetCapOffset (IioIndex, BusBase + 1, EpDev, 0, EFI_PCI_CAPABILITY_ID_PCIEXP);

        if (EpCapOffset == 0) {
          IIO_D_PCIERR ("%02X:%02X:%02X.%X: Device Capability %d not found, device is not PCIe compatible\n",
                        SegmentNum, BusBase + 1, EpDev, 0, EFI_PCI_CAPABILITY_ID_PCIEXP);
          break;
        }
        EpLnkSts.Uint16  = IioPciExpressRead16 (IioIndex, BusBase + 1, EpDev, 0, EpCapOffset + OFFSET_OF (PCI_CAPABILITY_PCIEXP, LinkStatus));
        EpLnkCtl2.Uint16 = IioPciExpressRead16 (IioIndex, BusBase + 1, EpDev, 0, EpCapOffset + OFFSET_OF (PCI_CAPABILITY_PCIEXP, LinkControl2));
        EpLnkCap.Uint32  = IioPciExpressRead32 (IioIndex, BusBase + 1, EpDev, 0, EpCapOffset + OFFSET_OF (PCI_CAPABILITY_PCIEXP, LinkCapability));

        StackPort = IioPortIndexToStackPort (PortIndex, &Stack);
        //
        // Print EWL if device is trained below its capacity
        //
        if (EpLnkSts.Bits.CurrentLinkSpeed < MIN (RpLnkCtl2.Bits.TargetLinkSpeed, EpLnkCtl2.Bits.TargetLinkSpeed)) {
          LogSpeedDegradation (IioIndex, Stack, StackPort, VendorId, DeviceId,
                              (UINT8)MIN (RpLnkCtl2.Bits.TargetLinkSpeed, EpLnkCtl2.Bits.TargetLinkSpeed), (UINT8)EpLnkSts.Bits.CurrentLinkSpeed);
          IIO_D_PCIWARN ("%02X:%02X:%02X.%X: Device %04X-%04X link speed trained below capability\n",
                          SegmentNum, BusNum, DevNum, FuncNum, VendorId, DeviceId);
          IIO_D_PCIWARN ("                  (RPLINKCTL2 0x%08X, RPLINKSTS 0x%08X)\n", RpLnkCtl2.Uint16, RpLnkSts.Uint16);
          IIO_D_PCIWARN ("                  (EPLINKCAP  0x%08X, EPLINKSTS 0x%08X)\n", EpLnkCap.Uint32, EpLnkSts.Uint16);
        }
        if (EpLnkSts.Bits.NegotiatedLinkWidth < MIN (RpLnkCap.Bits.MaxLinkWidth, EpLnkCap.Bits.MaxLinkWidth)) {
          LogWidthDegradation (IioIndex, Stack, StackPort, VendorId, DeviceId,
                              (UINT8)MIN (RpLnkCap.Bits.MaxLinkWidth, EpLnkCap.Bits.MaxLinkWidth), (UINT8)EpLnkSts.Bits.NegotiatedLinkWidth);
          IIO_D_PCIWARN ("%02X:%02X:%02X.%X: Device %04X-%04X link width trained below capability\n",
                          SegmentNum, BusNum, DevNum, FuncNum, VendorId, DeviceId);
          IIO_D_PCIWARN ("                  (RPLINKCAP 0x%08X, RPLINKSTS 0x%08X)\n", RpLnkCap.Uint32, RpLnkSts.Uint16);
          IIO_D_PCIWARN ("                  (EPLINKCAP 0x%08X, EPLINKSTS 0x%08X)\n", EpLnkCap.Uint32, EpLnkSts.Uint16);
        }
      } // for (EpDev...)

      //
      // Restore the original bus numbers.
      //
      IioPciExpressWrite8 (IioIndex, BusNum, DevNum, FuncNum, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, OrgSecondaryBus);
      IioPciExpressWrite8 (IioIndex, BusNum, DevNum, FuncNum, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, OrgSubordinateBus);
    } // for (PortIndex...)
  } // for (IioIndex...)
  return;
}

/**

  Execute the Phase 3 of IIO Ports initialization

  @retval IIO_SUCCESS
*/
IIO_STATUS
IioEarlyPostLinkInit (
  VOID
  )
{
  IIO_GLOBALS         *IioGlobalData;
  IIO_SI_POLICY_PPI   *IioSiPolicyPpi;
  EFI_STATUS          Status;

  IioGlobalData = NULL;
  IioSiPolicyPpi = NULL;

  Status = PeiServicesLocatePpi (&gIioSiPolicyPpiGuid, 0, NULL, (VOID**)&IioSiPolicyPpi);
  if (EFI_ERROR (Status)) {
    IIO_D_ERR ("%a: Locating gIioSiPolicyPpiGuid failed (%r)\n", __FUNCTION__, Status);
    ASSERT_EFI_ERROR (Status);
    return EFI_NOT_FOUND;
  } else {
    IioGlobalData = IioSiPolicyPpi->IioGlobalData;
  }

  //
  // Initialize Local IioGlobalData with IIO_GLOBALS values
  //
  IIO_D_LOG ("IIO Early Post Link Training Starting...\n");

  if (IioGlobalData->SetupData.DfxIioRcFlow == DFX_IIO_RC_FLOW_DISABLE) {
    return IIO_SUCCESS;
  }

  DumpIioPciePortPcieLinkStatus (IioGlobalData);

  IioLateInitializeEntry (IioGlobalData);

  //
  // Check lane width and lane speed
  //
  IioPciePortEWL (IioGlobalData);

  IIO_D_LOG ("IIO Early Post Link Training Completed!\n");

  return IIO_SUCCESS;
}

