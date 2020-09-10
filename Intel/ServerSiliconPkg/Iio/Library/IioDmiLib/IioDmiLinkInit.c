/** @file
  This file initialize the DMI device

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <IndustryStandard/PciExpress21.h>

#include <Library/DebugLib.h>
#include <Library/CpuAndRevisionLib.h>

#include <IioPlatformData.h>
#include <IioSetupDefinitions.h>

#include <Library/IioAccessLib.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioDmiLib.h>

#include <LibraryPrivate/IioDebug.h>
#include <LibraryPrivate/IioDmiConf.h>

#include <Private/Library/PeiPchDmiLib.h>


/**
 This function will issue a link re-train process in DMI if required

 This function is not called for RLINK!

 @param[in] IioGlobalData  Pointer to the Internal Data Storage (UDS) for this driver
 @param[in] IioIndex       Socket Index
 @param[in] PortIndex      Index port that will be used
**/
#define LINKTRAINING_TIME_OUT                     5000
#define LINKTRAINING_DELAY                        2
VOID
IioDmiLinkReTrain (
  IN  IIO_GLOBALS         *IioGlobalData,
  IN  UINT8               IioIndex,
  IN  UINT8               PortIndex
  )
{
  UINT8                     DmiPortNegWidth;
  UINT8                     DmiPortNegSpeed;
  PCI_REG_PCIE_LINK_STATUS  LnkSts;
  PCI_REG_PCIE_LINK_CONTROL LnkCon;
  UINT32                    WaitTime;

  static CHAR8 *LinkSpeedStr[] = {"unknown", "2.5GT/s", "5GT/s", "8GT/s", "16GT/s", "32GT/s"};

  IIO_D_DMIDBG ("[%d p%d] DMI Link Retrain\n", IioIndex, PortIndex);
  //
  // Assumption: the DMI link speed control bits are already set to desired value, thus
  // this routine will only take care of DMI link retrain and status
  //

  WaitTime = LINKTRAINING_TIME_OUT;
  do {
    IioStall (LINKTRAINING_DELAY);
    LnkSts.Uint16 = IioDmiReadLinkStatus (IioIndex, PortIndex);
    WaitTime--;
  } while ((LnkSts.Bits.LinkTraining == 1) && (WaitTime > 0));

  if (WaitTime == 0){
    IIO_D_DMIERR ("DMI Link Training bit not cleared\n");
    if (IioGlobalData->SetupData.HaltOnDmiDegraded == 1) {
      //
      // "halt on DMI degraded" setup option enabled => stop boot
      //
      CpuDeadLoop ();
    }
  }
  //
  // Now order the link to retrain.
  //
  LnkCon.Uint16 = IioDmiReadLinkControl (IioIndex, PortIndex);
  LnkCon.Bits.RetrainLink = 1;
  IioDmiWriteLinkControl (IioIndex, PortIndex, LnkCon.Uint16);

  //
  // Retrain again.
  //
  LnkCon.Uint16 = IioDmiReadLinkControl (IioIndex, PortIndex);
  LnkCon.Bits.RetrainLink = 1;
  IioDmiWriteLinkControl (IioIndex, PortIndex, LnkCon.Uint16);

  //
  // Clear Error
  //
  LnkSts.Uint16 = IioDmiReadLinkStatus (IioIndex, PortIndex);
  LnkSts.Bits.LinkAutonomousBandwidth = 1;
  LnkSts.Bits.LinkBandwidthManagement = 1;
  IioDmiWriteLinkStatus (IioIndex, PortIndex, LnkSts.Uint16);

  WaitTime = LINKTRAINING_TIME_OUT;
  do {
    IioStall (LINKTRAINING_DELAY);
    LnkSts.Uint16 = IioDmiReadLinkStatus (IioIndex, PortIndex);
    WaitTime--;
  } while ((LnkSts.Bits.LinkTraining == 1) && (WaitTime > 0));

  if (WaitTime == 0) {
    IIO_D_DMIERR ("DMI Link Training timeout\n");
    if (IioGlobalData->SetupData.HaltOnDmiDegraded == 1) {
      //
      // "halt on DMI degraded" setup option enabled => stop boot
      //
      CpuDeadLoop ();
    }
  }
  LnkSts.Uint16 = IioDmiReadLinkStatus (IioIndex, PortIndex);

  IIO_D_DMIDBG ("DMI speed is gen%d (%a)\n", LnkSts.Bits.CurrentLinkSpeed,
                LnkSts.Bits.CurrentLinkSpeed >= NELEMENTS(LinkSpeedStr) ? LinkSpeedStr[0] :
                LinkSpeedStr[LnkSts.Bits.CurrentLinkSpeed]);
  //
  // Determine Negotiated link width and speed for DMI port.
  //
  if (LnkSts.Bits.DataLinkLayerLinkActive == 1) {
    //
    // Link is active
    //
    DmiPortNegWidth = (UINT8) LnkSts.Bits.NegotiatedLinkWidth;
    DmiPortNegSpeed = (UINT8) LnkSts.Bits.CurrentLinkSpeed;
  } else {
    //
    // Link is NOT active
    //
    DmiPortNegWidth = 0;
    DmiPortNegSpeed = 0;
  }
  //
  // Save LinkWidth and LinkSpeed for DMI - this is needed to show it in setup menu.
  //
  IioGlobalData->IioVar.IioVData.DmiSocketMap |= 1 << IioIndex;
  IioGlobalData->IioVar.IioVData.DmiSocketData[IioIndex].LinkWidth = DmiPortNegWidth;
  IioGlobalData->IioVar.IioVData.DmiSocketData[IioIndex].LinkSpeed = DmiPortNegSpeed;

  //
  // Degraded DMI link width is not POR. So error and halt if DMI negotiated width is reduced!
  //
  if (DmiPortNegWidth != GetAssignedPortIndexWidth (IioGlobalData, IioIndex, PortIndex)) {
    IIO_D_DMIERR ("DMI in a degraded state! Expected width: 0x4, currently: 0x%X!\n", DmiPortNegWidth);
    if (IioGlobalData->SetupData.HaltOnDmiDegraded == 1) {
      //
      // "halt on DMI degraded" setup option enabled => stop boot
      //
      CpuDeadLoop ();
    }
  }
  //
  // Degraded DMI link speed is not POR. So error and halt if DMI link speed is reduced!
  //
  if (IioGlobalData->SetupData.IioPcieConfig.PciePortLinkSpeed[PORT_0_INDEX] != PCIE_LINK_SPEED_AUTO) {
    //
    // ignore if it is auto
    //
    if (DmiPortNegSpeed != IioGlobalData->SetupData.IioPcieConfig.PciePortLinkSpeed[PORT_0_INDEX]) {
      IIO_D_DMIERR ("DMI in a degraded state! Expected link_speed: %x, currently: %x!\n",
          IioGlobalData->SetupData.IioPcieConfig.PciePortLinkSpeed[PORT_0_INDEX], DmiPortNegSpeed);
      if (IioGlobalData->SetupData.HaltOnDmiDegraded == 1) {
        //
        // "halt on DMI degraded" setup option enabled => stop boot
        //
        CpuDeadLoop ();
      }
    }
  } // if-PciePortLinkSpeed != PCIE_LINK_SPEED_AUTO

  //
  // Clear errors after link retraining
  //
  IioDmiClearErrorsAfterLinkTraining (IioIndex, PortIndex);
}


/**
  DMI link specific initialization before training.

  @param[in] IioGlobalData   Pointer to the IIO global data
  @param[in] IioIndex        Socket/IIO complex index
  @param[in] PortIndex      Index port that will be used
 */
VOID
IioDmiLinkInit (
  IN  IIO_GLOBALS *IioGlobalData,
  IN  UINT8        IioIndex,
  IN  UINT8        PortIndex
  )
{
#ifndef IA32
  ASSERT(FALSE);
#else
  UINT8            PchSlsv;  // PCH supported link speeds vector
  UINT8            CpuSlsv;  // CPU supported link speeds vector
  UINT8            Slsv;
  UINT8            Tls;      // target link speed

  IIO_D_DMILOG ("[%d p%d] %a\n", IioIndex, PortIndex, __FUNCTION__);
  ASSERT (PortIndex == PORT_0_INDEX);
  //
  // Current PCH design does not support multi PCH. When it is added call PchGetPchIdBySocketId()
  // to get mapping of IioIndex to PCH ID.
  //
  PchSlsv = PchDmiGetSupportedLinkSpeedsVector ();
  CpuSlsv = IioDmiGetLinkSpeedVector (IioIndex, PortIndex);
  //
  // User setup may override chip capabilities.
  //
  switch (IioGlobalData->SetupData.IioPcieConfig.PciePortLinkSpeed[PORT_0_INDEX]) {
    case 1:
      CpuSlsv &= BIT0;               // Gen1 only
      break;
    case 2:
      CpuSlsv &= BIT1 | BIT0;        // Gen2 + Gen1
      break;
    case 3:
      CpuSlsv &= BIT2 | BIT1 | BIT0; // Gen3 + Gen2 + Gen1
      break;
    case 4:
      CpuSlsv &= BIT3 | BIT2 | BIT1 | BIT0; // Gen4 + Gen3 + Gen2 + Gen1
      break;
    default:
      IIO_D_DMIERR ("[%d p%d] Unknown link gen%d requested\n", IioIndex, PortIndex,
                    IioGlobalData->SetupData.IioPcieConfig.PciePortLinkSpeed[PORT_0_INDEX]);
    case 0: //Auto, just keep defaults as got from Link Capabilities2 register
      break;
  }
  //
  // Find the highest speed supported by both CPU and PCH.
  //
  Slsv = CpuSlsv & PchSlsv;
  for (Tls = 0; Slsv != 0; Tls++) {
    Slsv >>= 1;
  }
  IIO_D_DMILOG ("[%d p%d] DMI link speed vector IIO 0x%X, PCH 0x%X -> target speed %d\n",
                IioIndex, PortIndex, CpuSlsv, PchSlsv, Tls);
  if (Tls > 1) {
    //
    // Gen1 is default configured by chipset, no need to set it. Also there is assert in PCH code.
    //
    PchDmiSetTargetLinkSpeed (Tls);
  }
  //
  // Setup the DMI port LinkCap features (which have RW-O attribute)
  //
  IioDmiProgramLinkCap (IioGlobalData, IioIndex, PortIndex);

  //
  // Setup the DMI port LinkCap2 features (which have RW-O attribute)
  //
  IioDmiProgramLinkCap2 (IioGlobalData, IioIndex, PortIndex);

  //
  // This is need only for 14nm CPU
  //
  DisableGen2Override (IioIndex, PortIndex);

  //
  // Setup the DMI port LinkCtl, LinkCtl2, LinkCtl3
  //
  IioDmiProgramLinkCtls (IioGlobalData, IioIndex, PortIndex, Tls);

  //
  // Program XPTDEFs registers
  //
  IioDmiProgramXptDefs (IioIndex, PortIndex);
#endif
}

