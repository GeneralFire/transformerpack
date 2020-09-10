/** @file
  PCIE controller initialization routines.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Library/IioAccessLib.h>
#include <RcRegs.h>
#include <Library/IioDmiLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/IioSysInfoLib.h>
#include <IioSetupDefinitions.h>
#include <Library/PciMmio32AssignLib.h>
#include <Library/KtiApi.h>
#include <Library/IioPcieInit.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioNtbInit.h>
#include <Library/UsraCsrLib.h>
#include <Library/EmulationConfigurationLib.h>

#include <LibraryPrivate/IioDebug.h>
#include <LibraryPrivate/IioPwrConf.h>
#include <LibraryPrivate/IioPcieLib.h>
#include <PcieRegs.h>
#include <Library/CxlIpLib.h>
#include "PcieControllerInit.h"
#include <Library/IioMs2iosfInit.h>
#include <Library/SiliconWorkaroundLib.h>

/**

  Update our PCIEXpress configuration in our status information
  loop through each PCIE port and its respective IOU

  @param[in]  IioGlobalData - Pointer to IioGlobalData
  @param[in]  IioIndex      - Index to Iio

**/
VOID
IioUpdatePcieConfigurationData (
  IN  IIO_GLOBALS *IioGlobalData,
  IN  UINT8       IioIndex
  )
{
  UINT8           PortIndex;
  UINT8           *Pxp = NULL;
  UINT8           MaxPortNumberPerSocket;
  IIO_CONFIG      *SetupData;
  IIO_OUT_DATA    *IioOutData;

  SetupData = GetIioConfig ();
  IioOutData = GetIioOutData ();

  IIO_D_DBG ("%a -  Start IioIndex = %x!\n", __FUNCTION__, IioIndex);

  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
//  IioOutData->CurrentPXPMap[(IioIndex * MaxPortNumberPerSocket)] = 0x08;  // DMI port in SPR is Gen4 8x
  Pxp = &(IioOutData->CurrentPXPMap[0]);  // setup array to update status of PCIe ports

  for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    Pxp[(IioIndex * MaxPortNumberPerSocket) + PortIndex] = IioGetLanesNumOfPort (IioGlobalData, IioIndex, PortIndex);
  }

  //
  // Initialize PCIe port link width limit from setup options before early port initialization
  //
  InitPcieLinkWidthLimit (IioGlobalData, IioIndex);
}


/**

  Wait until Link is trained based on the PortIndex

  @param[in]  IioGlobalData - Pointer to IioGlobalData
  @param[in]  IioIndex      - Index to Iio
  @param[in] PortIndex      - Index to Iio Port 0 /1A/2A/3A/4A/5A

**/
BOOLEAN
CheckCxlCap (
  IN  IIO_GLOBALS                         *IioGlobalData,
  IN  UINT8                               IioIndex,
  IN  UINT8                               IouIndex
  )
{
  BOOLEAN                              PcieLinkTrained;
  LTSSMSMSTS_SB_FBLP_STRUCT            LtssmSts;
  UINT8                                MaxPortPerIou;
  UINT8                                PortIndex;
  UINT8                                StackIndex;
  PI5_MODES                            StackMode;
  RTLLCTL_SB_FBLP_STRUCT               RtllCtl;
  RTLLCTL3_SB_FBLP_STRUCT              RtllCtl3;
  LINKSTS2_SB_FBLP_STRUCT              LinkSts2;
  UINT8                                Index;
  UINT8                               MaxPortNumberPerSocket;

  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);

  StackIndex = GetStackIndexByIouIndex (IouIndex);
  MaxPortPerIou = GetMaxPortNumPerIou ();
  PortIndex = PORT_A_INDEX + MaxPortPerIou * IouIndex; // Always detect port A for CXL capability.
  StackMode = GetStackMode (IioGlobalData, IioIndex, StackIndex);
  //
  // We need to wait 50ms according to SA BS to let training completed
  // We used not to care about this, but now we do the port init immediately
  // after Bifurcation, hence have  to wait enough time and also poll on
  // LNKSTS.DLLA bit as per the IIO PCIe HAS.
  // LTSSMSMSTS.DLACTIVE bit can be used for status if the controller is unavailable at the time.
  //

  PcieLinkTrained = FALSE;

  //
  // If statements to resolve KW issue
  //
  if (IioIndex >= MaxIIO || StackIndex >= NELEMENTS (IioGlobalData->IioVar.IioVData.StackId[IioIndex])) {
    ASSERT (FALSE);
    return PcieLinkTrained;
  }

  if (StackMode != PI5_MODE_IAL) {
    PcieLinkTrained = TRUE; // Defer polling, after MRC link should be trained
    SetCxlStatus (IioIndex, StackIndex, NotSupportCxlMode); // Don't attempt protocol check
    return PcieLinkTrained;
  }

  if (IioGlobalData->IioVar.IioOutData.PciePortOwnership[(IioIndex * NUMBER_PORTS_PER_SOCKET) + PortIndex] == PCIE_PORT_NTB_MODE){
    PcieLinkTrained = TRUE;
    SetCxlStatus (IioIndex, StackIndex, NotSupportCxlMode); // Always report NotSupportCxlMode if NTB
    return PcieLinkTrained;
  }

  if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << StackIndex))) {
    PcieLinkTrained = TRUE;
    SetCxlStatus (IioIndex, StackIndex, NotSupportCxlMode); // Always report NotSupportCxlMode if stack not present.
    return PcieLinkTrained;
  }

  LtssmSts.Data = IioReadCpuCsr32 (IioIndex, PortIndex, LTSSMSMSTS_SB_FBLP_REG);
  if (LtssmSts.Bits.dlactive == 1) {   //wait on DLACTIVE. Bit would never set for unpopulated slots.
    //
    // if completed training
    //
    PcieLinkTrained = TRUE;
    //
    // APNSTS 17:16 Alternate Protocol Negotiation Status
    //      00 - APN not enabled
    //      01 - APN enabled, but no link partner support (no modified TS exchange)
    //      10 - APN resulted in failed negotiation or no agreement
    //      11 - APN was successful
    //
    if(LtssmSts.Bits.apnsts == 3 || (IsSiliconWorkaroundEnabled ("S22010581486") &&
       LtssmSts.Bits.apnsts == 1) || (IsSiliconWorkaroundEnabled ("S14011768962") &&
       IioGlobalData->SetupData.PcieSubSystemMode[IioIndex][IouIndex] == IIO_MODE_FORCE_CXL)) { // if APN was successful
      SetCxlStatus (IioIndex, StackIndex, AlreadyInCxlMode);   // Report CXL enable to KTI
      SetCxlBitMap (IioIndex, StackIndex);
      //
      // Do not config ports in CXL stack
      //
      for (Index = 0; Index < MaxPortPerIou; Index ++) {
        DEBUG ((DEBUG_INFO, "Disable port config for IIO %d Port %d:\n", IioIndex, PortIndex + Index));
        IioGlobalData->IioVar.IioOutData.PciePortConfig[(IioIndex * MaxPortNumberPerSocket) + PortIndex + Index] = 0x00;
      }
      //
      // 14010882657:VID is reversed if lane reversed which cause retimer failures in entering low latency mode
      //

      LinkSts2.Data = IioReadCpuCsr16 (IioIndex, PortIndex, LINKSTS2_SB_FBLP_REG);
      if ((LinkSts2.Bits.rpd == 1) && IsSiliconWorkaroundEnabled ("S14010882657")) {
        if (LtssmSts.Bits.lnkreversed) {
          RtllCtl3.Data = IioReadCpuCsr32 (IioIndex, PortIndex, RTLLCTL3_SB_FBLP_REG);
          RtllCtl3.Bits.vid = 0x6808; // reverse VID from 0x8086 to 0x6808 if lane reversed
          IioWriteCpuCsr32 (IioIndex, PortIndex, RTLLCTL3_IIO_PCIE_G5_REG, RtllCtl3.Data);
        }
        RtllCtl.Data = IioReadCpuCsr32 (IioIndex, PortIndex, RTLLCTL_SB_FBLP_REG);
        RtllCtl.Bits.enable = 3; // Enable low latency mode for IAL retimer
        IioWriteCpuCsr32 (IioIndex, PortIndex, RTLLCTL_SB_FBLP_REG, RtllCtl.Data);
      }
    } else {
      SetCxlStatus (IioIndex, StackIndex, NotInCxlMode);
    }
  }

  if (UbiosGenerationEnabled () && PcieLinkTrained) {
    //
    // Do a dummy read to point SBREG_PTE0 to FBLP, which can be removed after accesses above is switch to SB
    // Poll DLLLA bit until the bit is set.
    //
    //
    LtssmSts.Data = IioReadCpuCsr32 (IioIndex, PortIndex, LTSSMSMSTS_SB_FBLP_REG);
    DEBUG ((DEBUG_ERROR, "\nLABEL_waitforDL_Active_%d_%d:\n", IioIndex, StackIndex));
    DEBUG ((DEBUG_ERROR, "\nmov eax, DWORD PTR ds:[0%8xh]\n",
    UsraGetCsrRegisterAddress (IioIndex, PortIndex, LTSSMSMSTS_SB_FBLP_REG)));
    DEBUG ((DEBUG_ERROR, "\nbt  eax, 1\n"));
    DEBUG ((DEBUG_ERROR, "\njnc LABEL_waitforDL_Active_%d_%d\n", IioIndex, StackIndex));
  }

  return PcieLinkTrained;
}

/**

  This routine is to wait link to be trained based on the
  bifurcation

  @param[in]  IioGlobalData - Pointer to IioGlobalData
  @param[in]  IioIndex      - Index to Iio
  @param[in] iou_bif        - Iio Bifurcation

**/
VOID
WaitTrainingFinishedEx (
  IN  IIO_GLOBALS                     *IioGlobalData
  )
{
  UINT8         IioIndex;
  UINT8         MaxIouNumPerSocket;
  UINT8         Iou;
  UINT8         Loop;
  BOOLEAN       PcieLinkTrained;
  BOOLEAN       AllStackTrained;
  UINT8         TrainedStackMap[MaxIIO];
  UINT8         CxlStackPerSocket;
  UINT8         StackIndex;
  UINT8         PortIndex;
  UINT8         PortOffset;
  UINT8         MaxPortPerIou;

  Loop = 0;
  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
    TrainedStackMap[IioIndex] = 0;
  }

  do {
    //
    // Wait for 10ms
    //
    IioStall (GEN3_LINK_POLL_TIMEOUT);
    Loop++;
    AllStackTrained = TRUE;
    MaxIouNumPerSocket = GetMaxIouNumPerSocket();

    for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
      if (IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
        for (Iou = 0; Iou < MaxIouNumPerSocket; Iou++) {
          if (((TrainedStackMap[IioIndex]) & (1 << Iou)) != 0) {
            continue;                 // Skip detect if already done
          }
          PcieLinkTrained = CheckCxlCap (IioGlobalData, IioIndex, Iou);
          if (PcieLinkTrained) {
            TrainedStackMap[IioIndex] |= 1 << Iou;
          } else {
            AllStackTrained = FALSE;  // at least one stack train on going, need more loop
          }
        }// for Iou
      }// if SocketPresent
    }// for IioIndex

  } while ((Loop < 10) && !AllStackTrained);

  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
    if (IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
      CxlStackPerSocket = 0; // Reset for each socket
      for (Iou = 0; Iou < MaxIouNumPerSocket; Iou++) {
        StackIndex = GetStackIndexByIouIndex (Iou);

        //
        // If statements to resolve KW issue
        //
        if (IioIndex >= MaxIIO || StackIndex >= NELEMENTS (IioGlobalData->IioVar.IioVData.StackId[IioIndex])) {
          ASSERT (FALSE);
          continue;
        }

        if (IioGlobalData->SetupData.PcieSubSystemMode[IioIndex][Iou] == IIO_MODE_FORCE_CXL) {
          //
          // Do not config ports in CXL stack
          //
          MaxPortPerIou = GetMaxPortNumPerIou ();
          for (PortOffset = 0; PortOffset < MaxPortPerIou; PortOffset++) {
            PortIndex = GetPortIndexbyStack (StackIndex, PortOffset);
            IIO_D_DBG ("%a - Disable port config for IIO %d Port %d:\n", __FUNCTION__, IioIndex, PortIndex);
            IioGlobalData->IioVar.IioOutData.PciePortConfig[(IioIndex * NUMBER_PORTS_PER_SOCKET) + PortIndex] = 0x00;
          }
        }

        if (GetCxlStatus (IioIndex, StackIndex) == AlreadyInCxlMode) {
          CxlStackPerSocket += 1;
        }
        //
        // SPR has a limitation from Pcode that only 4 CXL stacks are supported per socket.
        //
        if (CxlStackPerSocket > 4) {
          IIO_D_PCIWARN ("CXL on socket %d stack %d is disabled as CXL amount limited to 4 on SPR !\n", IioIndex, StackIndex);
          SetCxlStatus (IioIndex, StackIndex, NotSupportCxlMode);
          continue;
        }
        //
        // CXL stacks are now known, so enable bank decoder as CXL
        //
        Ms2iosfBankDecoderEnableCxl (IioGlobalData, IioIndex, StackIndex);
      }// for Iou
    }// if SocketPresent
  }
}


/**
  Get MAD (Max Allowed Delay) by stack.

  @param UINT8         - StackIndex

  @return UINT8        - MAD value
 */
UINT8
GetMadByStack (
  IN  UINT8                                 StackIndex
  )
{
  UINT8     Mad = 0;

  switch (StackIndex) {
  case 0:
  case 1:
    Mad = 0;
    break;
  case 2:
  case 6:
  case 3:
  case 4:
    Mad = 1;
    break;
  case 5:
  case 7:
    Mad = 2;
    break;
  default:
    Mad = 0;
    break;
  }

  return Mad;
}

/**
  Init IIO MAD.
  TSC Hammock Harbor support.

  @param[in]  IioGlobalData - Pointer to IioGlobalData
  @param[in]  IioIndex      - Index to Iio

 */
VOID
IioMadInit (
  IN  IIO_GLOBALS                           *IioGlobalData,
  IN  UINT8                                 IioIndex
  )
{
  UINT8                               StackIndex;
  UINT8                               PortIndex;
  UINT8                               PortOffset;
  UINT8                               MaxIouStack;
  UINT8                               MaxPortPerStack;
  UINT8                               MaxPortNumberPerSocket;
  PTMGTECSR_IIO_PCIE_G5_STRUCT        PtmGteCsr;

  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  MaxIouStack = GetMaxIouNumPerSocket ();

  //
  // Stack0 on Die0, it work with default MAD = 0.
  //
  for (StackIndex = 1; StackIndex < (MaxIouStack + 1); StackIndex++) {
    if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << StackIndex))) {
      continue;
    }

    MaxPortPerStack = GetMaxPortNumPerStack (StackIndex);
    for (PortOffset = 0; PortOffset < MaxPortPerStack; PortOffset++) {
      PortIndex = GetPortIndexbyStack (StackIndex, PortOffset);
      if ((IioVData_ConfigurePciePort (IioGlobalData, IioIndex, PortIndex) == FALSE) ||
          (IioGlobalData->SetupData.PEXPHIDE[(IioIndex * MaxPortNumberPerSocket) + PortIndex] == TRUE)) {
        continue;
      }

      PtmGteCsr.Data = IioReadCpuCsr32 (IioIndex, PortIndex, PTMGTECSR_IIO_PCIE_G5_REG);
      PtmGteCsr.Bits.iosfmadp = GetMadByStack (StackIndex);
      if (IsSiliconWorkaroundEnabled ("S14010197754")) {
        PtmGteCsr.Bits.pgtscfe = 0x1;
        PtmGteCsr.Bits.pgtscff = 0x7;
      }
      IioWriteCpuCsr32 (IioIndex, PortIndex, PTMGTECSR_IIO_PCIE_G5_REG, PtmGteCsr.Data);
    }
  }
}

/**
  This routine programs the Clock Configuration

  @param[in] IioGlobalData  Pointer to IioGlobalData
  @param[in] IioIndex       Index to Iio
**/
VOID
ProgramIioClockConfiguration (
  IN IIO_GLOBALS                  *IioGlobalData,
  IN UINT8                        IioIndex
  )
{
  UINT8                           PortIndex;
  UINT8                           MaxPortNumberPerSocket;
  UINT16                          LnkCtl;
  IIO_CONFIG                      *SetupData;
  PCI_ROOT_BRIDGE_PCI_ADDRESS     PciAddress;
  UINT8                           PcieCapOffset;
  CAPSR_IIO_PCIEDMI_STRUCT        DmiCapsr;
  CAPSR_IIO_PCIE_G5_STRUCT        PcieCapsr;
  PCICMD_IIO_PCIE_G5_STRUCT       PciCmd;

  SetupData = GetIioConfig ();
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);

  for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {

    if (!IioVData_ConfigurePciePort(IioGlobalData, IioIndex, PortIndex)) { // check whether the PCIe port can be configured
      continue;
    }
    //
    // DMI is in RCRBBAR allocated by KTI-RC.
    // Set memory space enable bit for EXPPTMBAR on DMI/PCIe for initialization since IioAllocateMmioResource
    // won't be able to find it.
    //
    PciCmd.Data = IioReadCpuCsr16 (IioIndex, PortIndex, PCICMD_IIO_PCIE_G5_REG);
    PciCmd.Bits.mse = 1;
    IioWriteCpuCsr16 (IioIndex, PortIndex, PCICMD_IIO_PCIE_G5_REG, PciCmd.Data);

    //
    // Porting SRIS for DMI by CSR access. As DMI on SPR has no PCI device.
    //
    if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {
      DmiCapsr.Data = IioReadCpuCsr32 (IioIndex, PortIndex, CAPSR_IIO_PCIEDMI_REG);
      LnkCtl        = IioReadCpuCsr16 (IioIndex, PortIndex, LINKCTL_IIO_PCIEDMI_REG);
      if (SetupData->SRIS[(IioIndex * NUMBER_PORTS_PER_SOCKET) + PortIndex]) {
        DmiCapsr.Bits.linksts_scc = 0;
        LnkCtl &= ~B_PCIE_LCTL_CCC;
      } else {
        DmiCapsr.Bits.linksts_scc = 1;
        LnkCtl |= B_PCIE_LCTL_CCC;
      }
      IioWriteCpuCsr32 (IioIndex, PortIndex, CAPSR_IIO_PCIEDMI_REG, DmiCapsr.Data);
      IioWriteCpuCsr16 (IioIndex, PortIndex, LINKCTL_IIO_PCIEDMI_REG, LnkCtl);
      continue;
    }

    GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);
    PcieCapOffset = IioPCIeGetCapOffset (
                      IioIndex,
                      PciAddress.Bus,
                      PciAddress.Device,
                      PciAddress.Function,
                      EFI_PCI_CAPABILITY_ID_PCIEXP
                      );
    if (PcieCapOffset == 0) {

      IIO_D_PCIERR ("ProgramIioClockConfiguration Can't find PcieCapOffset!\n");
      continue;
    }
    PcieCapsr.Data = IioReadCpuCsr32 (IioIndex, PortIndex, CAPSR_IIO_PCIE_G5_REG);
    LnkCtl = IioPciExpressRead16 (
               IioIndex,
               PciAddress.Bus,
               PciAddress.Device,
               PciAddress.Function,
               PcieCapOffset + R_PCIE_LCTL_OFFSET
               );

    //
    // Uniphy recipe indicate to set common_clock to 0 if SRIS enabled
    //
    if (SetupData->SRIS[(IioIndex * NUMBER_PORTS_PER_SOCKET) + PortIndex]) {
      PcieCapsr.Bits.linksts_scc = 0;
      LnkCtl &= ~B_PCIE_LCTL_CCC;
    } else {
      PcieCapsr.Bits.linksts_scc = 1;
      LnkCtl |= B_PCIE_LCTL_CCC;
    }
    IioWriteCpuCsr32 (IioIndex, PortIndex, CAPSR_IIO_PCIE_G5_REG, PcieCapsr.Data);
    IioPciExpressWrite16 (
      IioIndex,
      PciAddress.Bus,
      PciAddress.Device,
      PciAddress.Function,
      PcieCapOffset + R_PCIE_LCTL_OFFSET,
      LnkCtl
      );
  }
  //
  // Enabling SAI policy before PCIe clock gating programming
  //
  IioEnableAccessPerSocket(IioGlobalData, IioIndex);

  IioMadInit (IioGlobalData, IioIndex);

  IioPciePortPowerConfig (IioGlobalData, IioIndex);
}

/**

  The purpose of this routine is to do the necessary work
  required to configure the PCIE Port Bifurcation Control
  register in orde to acces Pcie Configuration Space.

  @param[in]  IioGlobalData - Pointer to IioGlobalData
  @param[in]  IioIndex      - Index to Iio

**/
VOID
PcieProgramBifurcationPerSocket (
  IIO_GLOBALS             *IioGlobalData,
  UINT8                   IioIndex
)
{
  PcieDynamicPortSubdivision (IioGlobalData, IioIndex);
}

/**

  The purpose of this routine is to do the necessary work
  required to enable the PCIE POrts. The implementaiton is
  slightly different from Seaburg so we will check the straps
  and make the necessary adjustments before the links begin
  training.  In addition this routine could and should be be
  used to do any othe initialization required before PCI
  Express training.

  @param[in]  IioGlobalData - Pointer to IioGlobalData
  @param[in]  IioIndex      - Index to Iio

**/
VOID
PcieLinkTrainingInitPerSocket (
  IIO_GLOBALS             *IioGlobalData,
  UINT8                   IioIndex
  )

{
  LimitPcieLinkWidth (IioGlobalData, IioIndex);
  PcieLinkTrainingInit (IioGlobalData, IioIndex);
}
