/** @file
  Implement IIO initialization protocol to provide IIO initialization functions.
  This interfaces will be called by PciHostBridge driver in the PCI bus enumeration.

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

#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>

#include <Library/IioAccessLib.h>
#include <Library/IioDataHubLib.h>
#include <Library/OemIioInit.h>
#include <Library/IioInitLib.h>
#include <Library/IioPcieInit.h>
#include <Library/IioNtbInit.h>
#include <RcRegs.h>
#include "IioWorkAround.h"
#include <Library/IioDmiLib.h>
#include "IioEarlyInitialize.h"
#include <Chip/Include/CpuPciAccess.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/IioSysInfoLib.h>
#include <IioSetupDefinitions.h>
#include <Library/PciMmio32AssignLib.h>
#include <Library/KtiApi.h>
#include <Library/IioDfxInit.h>
#include <Library/CxlIpLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/PmcLib.h>
#include <Library/IioMiscFeatureLib.h>
#include <Library/IioCxlInit.h>
#include <Library/IioMs2iosfInit.h>

#include <LibraryPrivate/IioDebug.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/CheckpointLib.h>

/**
  IioOtherVariableInit: Initialize any platform port specific settings here for ex, Uplink port

  @param IioGlobalData - Pointer to Universal Data Structure
**/
VOID
IioOtherVariableInit (
  IN IIO_GLOBALS  *IioGlobalData,
  IN UINT8        IioIndex
  )
{
  UINT8                 IioUplinkPortIndex;
  UINT8                 Bus;
  UINT8                 Dev;
  UINT8                 Func;
  UINT8                 MaxPortNumberPerSocket;

  //
  // init any platform port specific settings here
  // for ex, Uplink port
  //
  if (IioGlobalData == NULL) {
    ASSERT (IioGlobalData != NULL);
    return;
  }
  if (IioIndex >= MaxIIO) {
    ASSERT (IioIndex < MaxIIO);
    return;
  }

  IIO_D_DBG ("IioOtherVariableInit %d\n", IioIndex);
  OemIioUplinkPortDetails(IioGlobalData, IioIndex, &IioUplinkPortIndex, &Bus, &Dev, &Func);
  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];

  if ((IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) &&
      (IioUplinkPortIndex < MaxPortNumberPerSocket)) {

    IIO_D_DBG ("Found uplink port @ skt %d port %d\n", IioIndex, IioUplinkPortIndex);

    IioGlobalData->IioVar.PreLinkData.UplinkInfo[IioIndex].Valid = 1;
    IioGlobalData->IioVar.PreLinkData.UplinkInfo[IioIndex].IioUplinkPortIndex = IioUplinkPortIndex;
    IioGlobalData->IioVar.PreLinkData.UplinkInfo[IioIndex].UplinkPortInfo.Device = Dev;
    IioGlobalData->IioVar.PreLinkData.UplinkInfo[IioIndex].UplinkPortInfo.Function = Func;

  } else {
    IIO_D_WARN ("Platform does not support uplink port!\n");
  }
}


/**

    This routine process all Port Config Space setting
    before link trainig is done

    @param IioGlobalData - Pointer to IioGlobalData
    @param IioIndex      - Iio Index

    @retval None

**/
VOID
IioEarlyPortInitialization (
  IN IIO_GLOBALS                         *IioGlobalData,
  IN UINT8                               IioIndex
  )
{
  UINT8   PortIndex;
  UINT8   MaxPortNumberPerSocket;

  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
  for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {

    if (!IioVData_ConfigurePciePort (IioGlobalData, IioIndex, PortIndex)) {
      continue;
    }
    if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {
      IioDmiLinkInit (IioGlobalData, IioIndex, PortIndex);
    } else {
      PciePortEarlyInit (IioGlobalData, IioIndex, PortIndex);
    }
  }
}


/**

    This routine process initializate and apply all Si WA
    before link trainig is done

    @param IioGlobalData - Pointer to IioGlobalData
    @param IioIndex      - Iio Index

    @retval None

**/
VOID
IioEarlyPreLinkTrainingPhase (
  IN  IIO_GLOBALS               *IioGlobalData,
  IN  UINT8                     Iio
  )
{
  IIO_D_DBG ("[%d] IioEarlyPreLinkTrainingPhase Start \n", Iio);
  //
  //1404610230:IIO Break Points needs to be improved.
  //
  OutputCheckpointSocket (Iio, STS_EARLY_PRELINK_TRAINING, 1, Iio);
  ProgramIioClockConfiguration (IioGlobalData, Iio);
  IioWorkAround (IioGlobalData, Iio, IioBeforeBifurcation);
  DfxPcieInit (IioGlobalData, Iio);
  IIO_D_DBG ("[%d] IioEarlyPreLinkTrainingPhase End\n", Iio);
}


/**
    This routine will initialize all the process for link training

    @param IioGlobalData - Pointer to IioGlobalData
    @param IioIndex      - Iio Index

    @retval None
**/
VOID
IioEarlyPcieLinkTrainingPhase (
  IN IIO_GLOBALS                         *IioGlobalData,
  IN UINT8                               IioIndex
  )
{
  OutputCheckpointSocket (IioIndex, STS_GEN3_OVERRIDE, 3, IioIndex);

  CxlIoPortPreTrainInit (IioGlobalData, IioIndex);

  FblpPreTrainInit (IioGlobalData, IioIndex);

  PciePhyPreLinkOverride (IioGlobalData, IioIndex);

  IioEarlyPortInitialization (IioGlobalData, IioIndex);

  if (!IsCpuAndRevision (CPU_SKX, REV_ALL) && !IsCpuAndRevision (CPU_CLX, REV_ALL) && !IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    if (!IsPCIeGen4 ()) {
      NtbInit (IioGlobalData, IioIndex);
    }
  }

  OutputCheckpointSocket (IioIndex, STS_LINK_TRAINING, 2, IioIndex);

  //
  // For Emulation minibios: Please skip the IIOINIT bifurcation/link training
  //
  if (IioGlobalData->SetupData.DfxIioLinkTraining == DFX_IIO_LNK_TRAIN_DISABLE) {
    return;
  }
  PcieLinkTrainingInitPerSocket (IioGlobalData, IioIndex);
}


/**
    This routine apply all Si configuration
    after link training is done

    @param IioGlobalData - Pointer to IioGlobalData
    @param IioIndex      - Iio Index

    @retval None
**/
VOID
IioEarlyPostLinkTrainingPhase (
  IN  IIO_GLOBALS                         *IioGlobalData,
  IN  UINT8                               IioIndex
  )
{
  IIO_D_LOG ("[%d] IioEarlyPostLinkTrainingPhase Start\n", IioIndex);
  //early post link training phase
  //Note: make sure before this routine is call there is a big delay for PCie link
  // training for that same IIO is covered before this routine is invoked.
  IioWorkAround (IioGlobalData, IioIndex, IioAfterBifurcation);
  //
  //IIO Isoc Enabling here instead of IioDmiPeim driver for minibios
  //
  IioIsocConfiguration (IioGlobalData, IioIndex);
}

/**
  Allocate Mmio Resource for IIO stack
  @param IioIndex       - Index to Socket

  @retval None
**/
VOID
EFIAPI
IioAllocateMmioResource (
  IN  UINT8               IioIndex,
  IN  IIO_GLOBALS         *IioGlobalData
  )
{
#ifdef IA32
  UINT8               StackIndex;
  UINT8               MaxStackPerSocket;
  UINT16              SegmentNum;
  MMIO_MAP_INFO       *AllocateMemoryMap;
  UINT16              NumberofMemoryMapRecords;
  UINT8               StackBusBase;
  UINT8               StackBusLimit;
  UINT32              StackMmiolBase;
  UINT32              StackMmiolLimit;

  AllocateMemoryMap = NULL;

  if (PmcIsDwrBootMode ()) {
    return;
  }

  SegmentNum = GetSegmentNum (IioIndex);
  MaxStackPerSocket = GetMaxStackNumPerSocket ();

  IIO_D_LOG ("[%d] %a (): SegmentNum=%d, MaxStackPerSocket=%d\n", IioIndex, __FUNCTION__, SegmentNum, MaxStackPerSocket);

  for (StackIndex = 0; StackIndex < MaxStackPerSocket; StackIndex++) {

    if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << StackIndex))) {
      continue;
    }

    StackBusBase = IioGlobalData->IioVar.IioVData.SocketStackBaseBusNumber[IioIndex][StackIndex];
    StackBusLimit = IioGlobalData->IioVar.IioVData.SocketStackLimitBusNumber[IioIndex][StackIndex];
    StackMmiolBase = IioGlobalData->IioVar.IioVData.SocketStackMmiolBase[IioIndex][StackIndex] + TEMP_MMIOL_RESOURCE_OFFSET;
    StackMmiolLimit = IioGlobalData->IioVar.IioVData.SocketStackMmiolLimit[IioIndex][StackIndex];

    IIO_D_LOG ("  Socket=%d, Stack=%d\n", IioIndex, StackIndex);
    IIO_D_LOG ("  Temp Bus   Resource: Base=0x%08x, Limit=0x%08x\n", StackBusBase, StackBusLimit);
    IIO_D_LOG ("  Temp Mmiol Resource: Base=0x%08x, Limit=0x%08x\n", StackMmiolBase, StackMmiolLimit);

    AssignMmio32_PciBusRange (SegmentNum, StackBusBase, StackBusLimit, StackMmiolBase, StackMmiolLimit, &AllocateMemoryMap, &NumberofMemoryMapRecords);
  }
#endif
}

/**

  Communicate to Pcode which PI5 instance is connected to CXL device.

  @param IioGlobalData    Pointer to IIO_GLOBALS

  @retval IIO_SUCCESS Initialization completed without problems
--*/
VOID
NotifyCxlInst (
  IN IIO_GLOBALS  *IioGlobalData
  )
{
  EFI_STATUS          Status = EFI_SUCCESS;
  UINT8               IioIndex = 0;
  UINT8               CxlIndex = 0;
  UINT8               StackIndex = 0;
  UINT8               LastCxlStack = 0;

  if (!IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    return;
  }

  if (IsSiliconWorkaroundEnabled ("S2209870115")) {
      IIO_D_LOG ("SI_WA: Skip CXL notification to disable CXL PM \n");
      return;
  }

  IIO_D_LOG ("CXL_NOTIFY_PCODE_START\n");

  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {

    if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
       continue;
    }

    LastCxlStack = 0;
    for (CxlIndex = 0; CxlIndex < GetMaxIouNumPerSocket (); CxlIndex++) {
      StackIndex = CxlIndex + 1;
      if (GetCxlStatus (IioIndex, StackIndex) == AlreadyInCxlMode) {
        LastCxlStack = StackIndex;
        Status = PcuNotifyCxlInst (IioIndex, StackIndex, FALSE);
      }
    }

    if (LastCxlStack != 0) {
      Status = PcuNotifyCxlInst (IioIndex, LastCxlStack, TRUE);
    }
  }

  IIO_D_LOG ("CXL_NOTIFY_PCODE_END\n");

  return;
}

/**

    This function is used as interface to initialize IIO Si features
    for DXE/SMM drivers

    @param IioGlobalData - IioGlobalData Pointer

    @retval None

**/
IIO_STATUS
IioEarlyInitializeEntry (
  IN IIO_GLOBALS  *IioGlobalData
  )
{
  IIO_STATUS    Status = EFI_SUCCESS;
  UINT8       IioIndex;
  //
  //1404610230:IIO Break Points needs to be improved.
  //
  OutputCheckpointSocket (0, STS_IIO_EARLY_INIT_ENTRY, 0, 0xFF );

  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {

    if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
       continue;
    }

    IioMiscFeatureEarlyInit (IioGlobalData, IioIndex);

    //
    // Allocate Mmio Resource for IIO stack
    //
    IioAllocateMmioResource (IioIndex, IioGlobalData);
    IioMs2iosfUpdateOobMsmBarShadowReg (IioGlobalData, IioIndex);

    //
    // Execute IIO Configuration only if not reset is required.
    // with this we avoid to configure IIO in the first boot after flash BIOS
    //
    IIO_D_LOG ("[%d] IIO Early Link Training Starting...\n", IioIndex);
    IioEarlyPreLinkTrainingPhase (IioGlobalData, IioIndex);
    IioEarlyPcieLinkTrainingPhase (IioGlobalData,IioIndex);
    IIO_D_LOG ("[%d] IIO Early Link Training Completed!\n", IioIndex);

    //
    //   DMI Port should be disabled for non-legacy socket
    //
    IioDmiDisableDmiOnNonLegacySkt (IioIndex);
  }

  WaitTrainingFinishedEx (IioGlobalData);

  //
  // Communicate to Pcode which PI5 instance is connected to CXL device.
  //
  NotifyCxlInst (IioGlobalData);

  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {

    if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
       continue;
    }
    ArbMuxClockGatingEn (IioGlobalData, IioIndex);
  }

#ifdef IA32
  IIO_D_DMILOG ("IIO Early Link Tc/Vc Configuration Start\n");
  //
  // Configure Dmi Tc/Vc
  //
  // 5331816: [LBG Val Critical] After warm reset, ME11 access to UMA does not work immediately after DID
  IioDmiTcVcSetup (IioGlobalData);
  IIO_D_DMILOG ("IIO Early Link Tc/Vc Configuration End\n");
  //
  // 5371608: BIOS should train DMI before DID/MRC
  //
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL) || IsCpuAndRevision (CPU_ICXSP, REV_ALL) ||
      IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
      if (((BIT0 << IioIndex) & GetPchPresentBitMap()) != 0) {
        IioDmiLinkReTrain (IioGlobalData, IioIndex, 0);
      }
    }
  }
#else
  if (IioGlobalData->IioVar.IioVData.RasOperation) {
    //
    // For IIO Online add any link training delay which would be common for all (out of spec)
    //
    IioStall (LINK_TRAINING_DELAY);   //400ms delay
    DumpIioPciePortPcieLinkStatus (IioGlobalData);    //dump link status
  }
#endif
  //
  //1404610230:IIO Break Points needs to be improved.
  //
  OutputCheckpointSocket (0, STS_IIO_EARLY_INIT_EXIT, 0, 0xFF );

  return Status;
}

