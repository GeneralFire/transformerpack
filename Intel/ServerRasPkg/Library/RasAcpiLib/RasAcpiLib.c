/** @file
  Implementation of Opportunistic Spare Core feature.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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
#include <PiSmm.h>
#include <Library/BaseLib.h>
#include <Library/RasGlobalDataLib.h>
#include <Library/RasDebugLib.h>
#include <Library/IioTopologyLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/RasMailBoxLib.h>
#include <Library/MmPciExtLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/PcieRasLib.h>
#include <RasAcpi.h>

//
// Global variable to cache pointer to SMM Ras Acpi protocol.
//
RAS_ACPI_PARAM   *mRasAcpiParam = NULL;

/**
  This routine is to initialize RAS ACPI BIOS Parameters for IIO. currently
  only OsNativeAerSupport is needed.

  @param[in]  NULL

  @retval     NULL
**/
VOID
EFIAPI
IioInitRasAcpiParam (
  VOID
  )
{
  RAS_MAIL_BOX                *MailBox = NULL;

  if (mRasAcpiParam == NULL) {
    return;
  }

  MailBox = GetRasMailBox ();
  mRasAcpiParam->OsNativeAerSupport = (UINT16)MailBox->IioInitPar.OsNativeAerSupport;
}

/**
  This routine is to add an entry in RAS ACPI BIOS Parameters for root port or RCEC
  If the root port/RCEC is not exist, each value is 0xff.

  @param[in] Socket         Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.

  @retval     NULL
**/
VOID
EFIAPI
RasAcpiParamAddRootPortEntry (
  UINT8         Socket,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
  )
{
  UINT8                       Seg;
  UINT32                      RortPortMmioBase;
  UINT32                      RootPortIndex;
  ROOT_PORT_INFO              *RootPortInfo;

  if (mRasAcpiParam == NULL) {
    return;
  }

  if (!PcieIsDevicePresent (Socket, Bus, Device, Function, NULL)) {
    return;
  }

  RootPortInfo = &mRasAcpiParam->RootPortInfo[0];
  RootPortIndex = mRasAcpiParam->PcieRootPortCount;
  if (RootPortIndex >= MAX_ROOTPORT) {
    RAS_DEBUG ((
      LEVEL_FUNC_FLOW,
      "%s :RootPortIndex: 0x%x bigger than 0x%x\n",
      __FUNCTION__, RootPortIndex, MAX_ROOTPORT
      ));
    return;
  }

  Seg = GetPcieSegment (Socket);

  RortPortMmioBase = (UINT32) MmPciAddressExt(Socket, Bus, Device, Function, 0, NULL);
  RootPortInfo[RootPortIndex].SegNumber = Seg;
  RootPortInfo[RootPortIndex].BaseBusNumber = Bus;
  RootPortInfo[RootPortIndex].BaseMmmioAddress = RortPortMmioBase;
  mRasAcpiParam->PcieRootPortCount++;

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "Root Port info Count:0x%x Socket:0x%x Seg:0x%x Bus:0x%x Mmio: 0x%x\n",
    mRasAcpiParam->PcieRootPortCount, Socket, Seg, Bus, RortPortMmioBase
    ));
}

/**
  This routine is to initialize RAS ACPI BIOS Parameters for Edpc info data.

  @param[in] Socket         Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.

  @retval     NULL
**/
VOID
EFIAPI
EdpcInitRasAcpiParam (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  if (mRasAcpiParam == NULL) {
    return;
  }

  mRasAcpiParam->EdpcInfo.Valid = 0;
  mRasAcpiParam->EdpcInfo.EdpcTrigger.Segment = GetPcieSegment (Socket);
  mRasAcpiParam->EdpcInfo.EdpcTrigger.Bus = Bus;
  mRasAcpiParam->EdpcInfo.EdpcTrigger.Device = Device;
  mRasAcpiParam->EdpcInfo.EdpcTrigger.Function = Function;

  if (GetRootPortInfoOfPcieDev (Socket, Bus, Device, Function,
        &mRasAcpiParam->EdpcInfo.EdpcRootPort.Segment,
        &mRasAcpiParam->EdpcInfo.EdpcRootPort.Bus,
        &mRasAcpiParam->EdpcInfo.EdpcRootPort.Device,
        &mRasAcpiParam->EdpcInfo.EdpcRootPort.Function)) {
    RAS_DEBUG ((
      LEVEL_FUNC_FLOW,
      "EDPC Root Port - Socket:%d Bus:%d Dev:%d Fun:%x \n",
      Socket,
      mRasAcpiParam->EdpcInfo.EdpcRootPort.Bus,
      mRasAcpiParam->EdpcInfo.EdpcRootPort.Device,
      mRasAcpiParam->EdpcInfo.EdpcRootPort.Function
      ));
    mRasAcpiParam->EdpcInfo.Valid = 1;
  }

  mRasAcpiParam->EdpcInfo.NotifyType = EDPC_NOTIFY_ERROR_INJECT;
}
/**
  The constructor function caches the pointer to SMM RAS ACPI protocol

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
RasAcpiLibConstructor (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  BOOLEAN           NewAllocation;

  mRasAcpiParam = GetRasGlobalDataEfiReservedMemory (
                    L"RasAcpiParam",
                    sizeof (RAS_ACPI_PARAM),
                    &NewAllocation);

  if (mRasAcpiParam == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "RasAcpiParam buffer is not allocated\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  if (NewAllocation) {
    //
    // Initialize the PCIE Root Port information buffer as 0xff due to match ASL code.
    //
    SetMem (&mRasAcpiParam->RootPortInfo[0], sizeof(ROOT_PORT_INFO) * MAX_ROOTPORT, 0xFF);
    //
    // Initialize the PCIE Root Port counter
    //
    mRasAcpiParam->PcieRootPortCount = 0;
  }
  return EFI_SUCCESS;
}
