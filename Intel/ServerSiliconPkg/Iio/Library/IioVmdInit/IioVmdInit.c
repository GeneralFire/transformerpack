/** @file
  This main VMD initialization

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
#include <Uefi/UefiBaseType.h>

#include <Library/DebugLib.h>
#ifdef IA32
#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Ppi/EndOfPeiPhase.h>
#endif

#include <Library/PchInfoLib.h>
#include <Library/PchPcieRpLib.h>

#include <IioSetupDefinitions.h>
#include <Library/IioSecurePlatformLib.h>
#include <Library/IioAccessLib.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioSysInfoLib.h>
#include <Library/IioVmdInit.h>

#include <LibraryPrivate/IioVmdConf.h>
#include <LibraryPrivate/IioDebug.h>



/**
  Rootports assigned to VMD needs to be visible for common initialization.
  Because of that it is necessary to disable VMD on early boot stage and enable VMD again
  (this is significant for the flow after warm reset).

  @param[in]  IioIndex      - index of the socket
**/
VOID
IioDisableVmdForInit (
  IN  UINT8  IioIndex
  )
{
  UINT8  StackIndex;

  IIO_D_VMDLOG ("[%d] VMD disabled for RPs init.\n", IioIndex);

  for (StackIndex = 0; StackIndex < MAX_STACKS_PER_SOCKET; StackIndex++) {

    if (IsDmiStack (StackIndex) && !IsVmd20Supported ()) {
      continue;
    }

    ClearVmdAssigning (IioIndex, StackIndex);
  }
}

/**
    Volume Management Device (VMD) main initialization
    It needs to be done on End-Of-Pei to be sure that all rootports are initialized
    especially PCH rootports.
**/

EFI_STATUS
IioVmdInitOnEndOfPei (
#ifdef IA32
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi)
#else
  VOID )
#endif
{
  IIO_GLOBALS           *IioGlobalData;
  UINT8                 IioIndex;
  UINT8                 Device;
  UINT8                 Function;
  UINT8                 PortInStackIdx;
  UINT8                 PortIndex;
  UINT8                 StackIndex;
  UINT8                 MaxPortNumberPerSocket;
  IIO_DEVFUNHIDE_TABLE  IioDevFuncHide[MaxIIO];

  //
  // Clean IioDevFuncHide before use it
  //
  ZeroMem (IioDevFuncHide, sizeof (IIO_DEVFUNHIDE_TABLE) * MaxIIO);

  //
  // GlobalData on EndOfPei must be fetched from HOB
  //
  if (EFI_ERROR (GetIioGlobalDataEx (&IioGlobalData, TRUE))) {
    IIO_D_VMDERR ("%a: Failed to get IioGlobalData.\n", __FUNCTION__);
    return EFI_UNSUPPORTED;
  }

  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
    for (StackIndex = 0; StackIndex < MAX_STACKS_PER_SOCKET; StackIndex++) {

      //
      // Skip initialization if stack is not present
      //
      if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << (StackIndex)))) {
        continue;
      }

      //
      // Skip initialization if VMD is not enabled for this stack.
      //
      if (!IsVMDEnabledForStack (IioIndex, StackIndex)) {
        continue;
      }

      IIO_D_VMDLOG ("[%d.%d] VMD setup for stack.\n", IioIndex, StackIndex);

      //
      // Check VMD device presence
      //
      if (!IsVmdDevicePresent (IioIndex, StackIndex)) {
        IIO_D_VMDWARN ("[%d.%d] VMD device not present.\n", IioIndex, StackIndex);
        continue;
      }

      if (IsDmiStack (StackIndex)) {
        //
        // Check if PCH rootports are supported by VMD and if this is active DMI
        //
        if (IsVmd20Supported () && IioDmiIsPortIdentity (IioIndex, PORT_0_INDEX, IioDmiPortActive)) {
          //
          // Program PCH rootports
          //
          for (PortIndex = 0; PortIndex < GetPchMaxPciePortNum (); PortIndex++) {
            if (IioGlobalData->SetupData.VMDPchPortEnable[IioIndex][PortIndex] == 0) {
              // VMD is not enabled for this rootport
              continue;
            }
            GetPchPcieRpDevFun (PortIndex, (UINTN*) &Device, (UINTN*) &Function);
            AssigningDeviceToVmd (IioIndex, StackIndex, Device, Function, TRUE);
          } // For each PCH rootport
        } else {
          //
          // VMD 1.0 doesn't support PCH rootports
          //
          continue;
        }
      } else {
        //
        // Program IIO root ports which are working as VMD
        //
        for (PortInStackIdx = 0; PortInStackIdx < GetMaxPortNumPerStack (StackIndex); PortInStackIdx++) {

          PortIndex = GetPortIndexbyStack (StackIndex, PortInStackIdx);
          if (!IsVMDEnabledForPort (IioIndex, PortIndex)) {
            continue;
          }

          //
          // VMD is enabled for this port
          //
          Device = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Device;
          Function = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].Function;

          AssigningDeviceToVmd (IioIndex, StackIndex, Device, Function, TRUE);
          if (!IsVmd20Supported ()) {
            //
            // Rootports assigned to VMD needs to be hidden by bios in VMD1.0 (SKX/CPX)
            //
            IioDevFuncHide[IioIndex].IioStackDevHide[StackIndex].DevToHide[Function] |= (1 << Device);

            MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
            IioGlobalData->SetupData.PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PortIndex] = 1;
          }
        } // For each IIO rootport
      }

      //
      // Set VMD BAR sizes and attributes
      //
      ConfigureVmdBars (IioIndex, StackIndex);

      //
      // Program VMD-config register
      //
      SetVmdConfig (IioIndex, StackIndex);

      //
      // Apply VMD specific workarounds
      //
      ApplyVmdWorkarounds (IioIndex, StackIndex);

    } // For each stack

    if (!IsVmd20Supported ()) {
      //
      // Hide devices assigned to VMD - this applied only to 14nm cpus (SKX/CLX/CPX)
      //
      IioDevicesHide (IioGlobalData, IioIndex, IioEndOfPei, IioDevFuncHide);
    }
  } // For each socket

  return EFI_SUCCESS;
}

#ifdef IA32
STATIC EFI_PEI_NOTIFY_DESCRIPTOR mNotifyDescriptor = {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiEndOfPeiSignalPpiGuid,
    IioVmdInitOnEndOfPei
};
#endif

/**
    Volume Management Device (VMD) callback registration.
    Main VMD initialization needs to be done on End-Of-Pei
    to be sure that all rootports are initialized
**/
VOID
IioVmdInit ( VOID )
{
#ifdef IA32
  EFI_STATUS   Status;
  //
  // This is normal flow in PEI - just register callback on end-of-pei
  //
  Status = PeiServicesNotifyPpi (&mNotifyDescriptor);
  if (EFI_ERROR (Status)) {
    IIO_D_VMDERR ("Failed to register callback on end-of-pei\n");
    return;
  }
  IIO_D_VMDLOG ("VMD init registered on endOfPei.\n");

#else
  //
  // strange that this is not called in PEI but if so just call VMD init
  //
  IioVmdInitOnEndOfPei ();
#endif
}


/**
  Returns TRUE if given PCH rootport can be assigned to VMD

  @param[in] IioIndex   - IIO instance (usually socket).
  @param[in] PchRpIndex     - Index of the PCH rootport.

  @return TRUE if given PCH rootport can be assigned to VMD
 */
BOOLEAN
EFIAPI
IioIsPchPortAllowed (
  IN UINT8  IioIndex,
  IN UINT8  PchRpIndex
  )
{
  IIO_GLOBALS  *IioGlobalData;
  UINT8        Bus;
  UINT8        Device;
  UINT8        Function;
  UINT8        PCIeCapOffset;
  UINT16       CapReg;

  if (PchRpIndex >= GetPchMaxPciePortNum ()) {
    //
    // There is no such rootport
    //
    return FALSE;
  }

  if (EFI_ERROR (GetIioGlobalData (&IioGlobalData))) {
    IIO_D_VMDERR ("%a: Failed to get IioGlobalData.\n", __FUNCTION__);
    return FALSE;
  }

  if (IioGlobalData->SetupData.VMDPchPortEnable[IioIndex][PchRpIndex] != 0) {
    //
    // Already enabled - it means VMD is allowed
    //
    return TRUE;
  }

  Bus = IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex][PORT_0_INDEX];

  //
  // Find device and function of this Pch rootport
  //
  if (EFI_ERROR (GetPchPcieRpDevFun (PchRpIndex, (UINTN*)&Device, (UINTN*)&Function))) {
    return FALSE;
  }

  //
  // Read slot-implemented bit from PCH rootport
  //
  PCIeCapOffset = IioPCIeGetCapOffset (IioIndex, Bus, Device, Function, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (PCIeCapOffset == 0) {
    return FALSE;
  }

  CapReg = IioPciExpressRead16 (IioIndex, Bus, Device, Function, PCIeCapOffset + R_PCIE_XCAP_OFFSET);
  if (CapReg & B_PCIE_XCAP_SI) {
    //
    // Rootport is connected to the slot
    //
    return TRUE;
  } else {
    //
    // Rootport is not connected to the slot
    //
    return FALSE;
  }
}
