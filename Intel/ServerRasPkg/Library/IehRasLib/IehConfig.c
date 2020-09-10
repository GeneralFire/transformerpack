/** @file
  Implementation of all the devices' configuration including IEH.

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

#include "IehRasLibInternal.h"
#include <Library/WorkaroundLib.h>

extern NODE_DEVICE   *mGlobalIEH;
extern RAS_MAIL_BOX  *mMailBox2;


/**
  Enable system error interrupt map.

  @param [IN]   Ieh  --  IEH Node

  @retval none
..
**/
VOID
EFIAPI
EnableSysMap (
  IN   NODE_DEVICE   *Ieh
  )
{
  UINT8               Socket;
  UINT8               Bus;
  UINT8               Device;
  UINT8               Function;
  IEH_DEVICE_TYPE     IehDevType;

  Bus = Ieh->Common.Bus;
  Socket = Ieh->Common.Socket;
  Device = Ieh->Common.Dev;
  Function = Ieh->Common.Func;
  IehDevType = Ieh->Common.Type;

  SetGsysevtmapReg (Socket, Bus, Device, Function, IehDevType, mMailBox2->IioInitPar.IoMcaEn);
}

/**
  disable system error event register.

  @param [IN]   Ieh  --  IEH Node

  @retval none
..
**/
VOID
EFIAPI
DisableSysCtl (
  IN   NODE_DEVICE   *Ieh
  )
{
  UINT8               Socket;
  UINT8               Bus;
  UINT8               Device;
  UINT8               Function;
  UINT8               IehVer;
  IEH_DEVICE_TYPE     IehDevType;

  Bus = Ieh->Common.Bus;
  Socket = Ieh->Common.Socket;
  Device = Ieh->Common.Dev;
  Function = Ieh->Common.Func;
  IehDevType = Ieh->Common.Type;
  IehVer = (IehDevType == GlobalIeh) ? Ieh->GlobalIEH.IehVer : Ieh->SatelliteIEH.IehVer;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "Disabling GlobalSysCtl S:0x%x B:0x%x D:0x%x F:0x%x \n ",
    Socket, Bus, Device, Function));
  SetGsysevtctlReg (Socket, Bus, Device, Function, IehDevType, IehVer, FALSE);
}

/**
  Enable system error event register.

  @param [IN]   Ieh  --  IEH Node

  @retval none
..
**/
VOID
EFIAPI
EnableSysCtl (
  IN   NODE_DEVICE   *Ieh
  )
{
  UINT8               Socket;
  UINT8               Bus;
  UINT8               Device;
  UINT8               Function;
  UINT8               IehVer;
  IEH_DEVICE_TYPE     IehDevType;

  Bus = Ieh->Common.Bus;
  Socket = Ieh->Common.Socket;
  Device = Ieh->Common.Dev;
  Function = Ieh->Common.Func;
  IehDevType = Ieh->Common.Type;
  IehVer = (IehDevType == GlobalIeh) ? Ieh->GlobalIEH.IehVer : Ieh->SatelliteIEH.IehVer;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "Enabling GlobalSysCtl S:0x%x B:0x%x D:0x%x F:0x%x \n ",
    Socket, Bus, Device, Function));
  SetGsysevtctlReg (Socket, Bus, Device, Function, IehDevType, IehVer, TRUE);
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "after Enabling GlobalSysCtl S:0x%x B:0x%x D:0x%x F:0x%x \n ",
    Socket, Bus, Device, Function));
}

/**
  Enable Error pin.

  @param [IN]   Skt  -- socket index
  @param [IN]   Bus  -- Bus number
  @param [IN]   Device  -- Device number
  @param [IN]   Function -- Function number

  @retval none
..
**/
VOID
EFIAPI
EnableErrorPin (
  IN  UINT8         Socket,
  IN  UINT8         Bus,
  IN  UINT8         Device,
  IN  UINT8         Function
  )
{
  UINT8         IioErrorPin0En;
  UINT8         IioErrorPin1En;
  UINT8         IioErrorPin2En;

  ClearErrorPinStatus (Socket, Bus, Device, Function);

  IioErrorPin0En = mMailBox2->IioInitPar.IioErrorPin0En;
  IioErrorPin1En = mMailBox2->IioInitPar.IioErrorPin1En;
  IioErrorPin2En = mMailBox2->IioInitPar.IioErrorPin2En;

  SetErrpinctlReg (Socket, Bus, Device, Function, IioErrorPin0En, IioErrorPin1En, IioErrorPin2En);

}

/**
  This implementation is to set each MCChan with LBITMAP.

  All McChan under MC share the same bitmap value.

  @param  Socket -- socket index
  @param  Mc -- Mc index
  @param  Ch -- Channel index
  @param  LBitmap  -- bitmap value
  @param  Bus         Bus number
  @param  Device      Device number
  @param  Function    Function number

  @retval  none.
**/
VOID
EFIAPI
SetMcLBitmap (
  IN    UINT8    Socket,
  IN    UINT8    Mc,
  IN    UINT8    ChOnMc,
  IN    UINT8    LBitmap,
  IN    UINT8    Bus,
  IN    UINT8    Device,
  IN    UINT8    Function
  )
{
  SetMcLBitmapMcChan (Socket, Mc * MAX_MC_CH + ChOnMc, LBitmap, Bus, Device, Function);
}

/**
  This implementation is to Program ABM, AbnLbn, AbnNbn for RCEC

  @param  Ieh -- Which IEH we are proccessing

  @retval  none.
**/
VOID
EFIAPI
SetRcecAbmNbnLbn (
  IN  NODE_DEVICE * Ieh
  )
{
  UINT8               Socket;
  UINT8               Bus;
  UINT8               Device;
  UINT8               Function;
  UINT16              RcecCapOffset;
  UINT32              Data32;

  Socket = Ieh->Common.Socket;
  Bus = Ieh->Common.Bus;
  Device = Ieh->Common.Dev;
  Function = Ieh->Common.Func;

  RcecCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
                    NULL, PCI_EXPRESS_EXTENDED_CAPABILITY_EVENT_COLLECTOR_ENDPOINT_ASSOCIATION_ID, 0xff);

  //
  // Program ABM, AbnLbn, AbnNbn for RCEC
  //
  if (RcecCapOffset != 0) {
    //
    // According to the PCIE spec, the Association Bitmap for RCiEPs register
    // should set the bits that corresponding to:
    //   1) Device numbers of RCiEPs associated with the RCEC on the same bus;
    //   2) Device number of the RCEC.
    //
    Data32 = Ieh->SatelliteIEH.ABMRCiEP | (BIT0 << Device);
    //MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, RcecCapOffset + ABMRCIEP_OFFSET, NULL), Data32);
    WriteCsrRegTable (MmPciAddressExt (Socket, Bus, Device, Function, RcecCapOffset + ABMRCIEP_OFFSET, NULL), Data32);
    RAS_DEBUG ((LEVEL_FUNC_FLOW,"SetRcecAbmNbnLbn ABMRCIEP_OFFSET = 0x%x\n", Data32));
    Data32 = (Ieh->SatelliteIEH.RcecAbnLbn << 16) + (Ieh->SatelliteIEH.RcecAbnNbn << 8);
    //MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, RcecCapOffset + ABNRCIEP_OFFSET, NULL), Data32);
    WriteCsrRegTable (MmPciAddressExt (Socket, Bus, Device, Function, RcecCapOffset + ABNRCIEP_OFFSET, NULL), Data32);
    RAS_DEBUG ((LEVEL_FUNC_FLOW,"SetRcecAbmNbnLbn ABNRCIEP_OFFSET = 0x%x\n", Data32));
  }
}

/**
  This routine is to init a single IEH Local error source

  @param  Ieh -- Which IEH we are proccessing
  @param  LclDev -- Local Device Node that report this error
  @param  ErrSrcId -- Local Error Source ID

  @retval none

**/
VOID
EFIAPI
IehLocalErrSrcXInit(
  IN  NODE_DEVICE * Ieh,
  IN  NODE_DEVICE * LclDev,
  IN  UINT8       ErrSrcId
)
{
  UINT32          CorrErrMask;
  UINT32          UnCorrErrMask;

  DisplayIehDevType (LclDev->Common.Type, FALSE);
  DisplayIehDevSubType (LclDev, TRUE);

  //
  // First to enable Local Error Source 0, as it is IEH Local and psf errors which  is
  // different from other local error source
  //

  //
  // Ieh Local and psf error are always fatal error and no correctable error, always enable them
  //

  if (ErrSrcId == 0) {
    CorrErrMask = 0;
    UnCorrErrMask = 0;
  } else {
    //
    // For other Local error sources, there are AER style mask regiters, init them as AER style
    //
    CorrErrMask = PCIEAER_CORRERR_MSK;
    UnCorrErrMask = PCIEAER_UNCORRERR_MSK;

    if (mMailBox2->PcieInitPar.PcieAerCorrErrEn == 1) {
      CorrErrMask &= ~mMailBox2->PcieInitPar.PcieAerCorrErrBitMap;
    }

    if (mMailBox2->PcieInitPar.PcieAerNfatErrEn == 1) {
      UnCorrErrMask &= ~mMailBox2->PcieInitPar.PcieAerNfatErrBitMap;
    }

    if (mMailBox2->PcieInitPar.PcieAerFatErrEn == 1) {
      UnCorrErrMask &= ~mMailBox2->PcieInitPar.PcieAerFatErrBitMap;
    }
  }
  //
  // programm local error masks
  //
  SetIehLocalErrorMask (Ieh, ErrSrcId, CorrErrMask, UnCorrErrMask);

  //
  // Clear Error status Registers for Ieh Local and Psf error
  //
  ClearIehLocalErrorStatus (Ieh, ErrSrcId, IEH_CORRECT_ERROR | IEH_NON_FATAL_ERROR | IEH_FATAL_ERROR);

  //
  // Init specific local device error
  //
  switch (LclDev->IehLocalDevice.IehLclDevType) {
    case PsfDino:
      break;
    case PsfPciePi5:
      break;
    case PsfHcX:
      break;
    case PcieGen4DmiPi5:
      if (mMailBox2->IioInitPar.IioDmiErrorEn) {
        EnableDmiDevice (LclDev->Common.Socket, LclDev->Common.Stack, 0, 0);
      }
      break;
    case PcieGen5IalPi5:
      if (LclDev->IehLocalDevice.LclSpecInfo.CxlDevInfo.CxlValid) {
        CxlRasConfigDpUp (LclDev->Common.Socket, LclDev->Common.Stack);
      }
      break;
    case SpdI3cBus:
      SmbusRecoveryConfig (Ieh->Common.Socket, LclDev->Common.SbPortId);
      break;
    case CxpSmBus:
      break;
    default:
      break;
  }
}

/**
  This routine is to initialize IEH Local error source

  @param  Ieh -- Which IEH we are proccessing
  @param  NodeList -- Device array that include IEH Local error source and RCiEP devices

  @retval none

**/
VOID
EFIAPI
IehLocalErrorSourceInit(
  IN  NODE_DEVICE * Ieh,
  IN  NODE_DEVICE * NodeList
)
{
  UINT8                           IehVer;
  UINT8                           ErrSrcId;
  UINT8                           StartErrSrcId;
  UINT8                           EndErrSrcId;
  UINT8                           LclErrSrc;
  UINT8                           i;
  BOOLEAN                         RcecSupport;

  RAS_DEBUG((LEVEL_FUNC_FLOW,"   [Init Device] Internal device  \n"));
  //
  // Get IEH Attributes
  //
  if (Ieh->Common.Type == GlobalIeh) {
    RcecSupport = Ieh->GlobalIEH.RcecSupport;
    LclErrSrc = Ieh->GlobalIEH.LocalErrorSource;
    IehVer = Ieh->GlobalIEH.IehVer;
  } else {
    RcecSupport = Ieh->SatelliteIEH.RcecSupport;
    LclErrSrc = Ieh->SatelliteIEH.LocalErrorSource;
    IehVer = Ieh->SatelliteIEH.IehVer;
  }

  if (RcecSupport) {
    StartErrSrcId = 0;
    EndErrSrcId = LclErrSrc;
  } else if (IehVer == IEH_VER_RCEC_DIS) {
    //
    // Wave3 Global IEH that doesn't have AER/RCEC support, but have multiple sets of Local error registers
    //
    StartErrSrcId = 0;
    EndErrSrcId = LclErrSrc;
  } else {
    //
    // For Wave1/2, Only one set of local error registers
    //
    StartErrSrcId = 0;
    EndErrSrcId = 0;
  }

  //
  // Init Local Error Sources
  //
  for (ErrSrcId = StartErrSrcId; ErrSrcId <= EndErrSrcId; ErrSrcId++) {
    //
    // Find Corresponding Local device Node
    //
    for ( i = 0; i < NodeList[0].Common.DevCount; i++) {
      if (NodeList[i].Common.Type == IehLocalDevice ) {
        if (NodeList[i].IehLocalDevice.ErrorSourceId == ErrSrcId) {
          IehLocalErrSrcXInit (Ieh, &NodeList[i], ErrSrcId);
        }
      }
    }
  }
}

/**
  This routine is to initialize RCiEP devices that connect to this IEH RCEC

  @param  Ieh -- Which IEH we are proccessing
  @param  NodeList -- Device array that include IEH Local error source and RCiEP devices

  @retval none

**/
VOID
EFIAPI
IehRciepDevicesInit(
  IN  NODE_DEVICE * Ieh,
  IN  NODE_DEVICE * NodeList
)
{
  UINT8         Skt;
  UINT8         Bus;
  UINT8         Dev;
  UINT8         Fun;
  UINT16        RcecCapOffset;

  Skt = Ieh->Common.Socket;
  Bus = Ieh->Common.Bus;
  Dev = Ieh->Common.Dev;
  Fun = Ieh->Common.Func;

  if (mMailBox2->IioInitPar.IioRootCtlOverride == 1){
    SetIioRootCtlOverride (Skt, Bus, Dev, Fun);
  }

  RcecCapOffset = PcieGetExtendedCapabilitiesOffset (Skt, Bus, Dev, Fun,
                    NULL, PCI_EXPRESS_EXTENDED_CAPABILITY_EVENT_COLLECTOR_ENDPOINT_ASSOCIATION_ID, 0xff);

  if (RcecCapOffset != 0) {
    RasAcpiParamAddRootPortEntry (Skt, Bus, Dev, Fun);
  }

  //
  // Program RCEC AER and error control Settings and
  // For each RCiEP devices, config AER and Error Control
  //
  PcieConfigRcecAndAssociateRcieps (Skt, Bus, Dev, Fun, NULL);

}

/**
  This routine is to initialize IEH Local error source and RCiEP devices

  @param  Ieh -- Which IEH we are proccessing
  @param  NodeList -- Device array that include IEH Local error source and RCiEP devices

  @retval none

**/
VOID
EFIAPI
IehInternalDeviceInit(
  IN  NODE_DEVICE * Ieh,
  IN  NODE_DEVICE * NodeList
)
{
  BOOLEAN             RcecSupport;

  IehLocalErrorSourceInit (Ieh, NodeList);

  RcecSupport = Ieh->Common.Type == GlobalIeh ? Ieh->GlobalIEH.RcecSupport : Ieh->SatelliteIEH.RcecSupport;
  //
  // For the IEH that support RCEC
  //
  if (RcecSupport){
    IehRciepDevicesInit (Ieh, NodeList);
  }
}
/**
  This routine purpose is to enable PCIE device include rootport/DMI/CBDMA ....
  If any device has ras feature beyond standard AER, please add hook here.

  @param [IN]   NodeDev  --  Node Device of IEH End Point

  @retval none

**/
VOID
EFIAPI
IehEndPointDevInit (
  IN   NODE_DEVICE   * NodeDev
  )
{
  UINT8               BitIndex;
  UINT8               Skt;
  UINT8               Bus;
  UINT8               Dev;
  UINT8               Func;
  IEH_END_POINT_TYPE  Type;
  UINT32              Severity;

  Bus = NodeDev->Common.Bus;
  Skt = NodeDev->Common.Socket;
  Dev = NodeDev->Common.Dev;
  Func = NodeDev->Common.Func;
  Type = NodeDev->IehEndPoint.IehEndPointType;
  BitIndex = NodeDev->IehEndPoint.BitIndex;
  Severity = NONFATAL_ERROR_TYPE | FATAL_ERROR_TYPE | COR_ERROR_TYPE;

  RAS_DEBUG((LEVEL_FUNC_FLOW,"   [Init Device] - BitIndex:0x%x Skt:0x%x Bus:0x%x Dev:0x%x F:0x%x ", BitIndex, Skt, Bus, Dev, Func));
  DisplayIehDevType (NodeDev->Common.Type, FALSE);
  DisplayIehDevSubType (NodeDev, TRUE);

  switch (Type) {
    case McDDRDevice:
    //
    // Due to security requirements, some registers cannot be updated after EndOfDxe
    // McDDRDevice will be handled in InstantiateChildNode() instead
    //
    break;
    case DMIDevice:
      PcieClearDeviceStatus (Skt, Bus, Dev, Func, NULL, Severity);
      IioXpClearErr (Skt, Bus, Dev, Func);
      if (mMailBox2->IioInitPar.IioDmiErrorEn) {
        EnableDmiDevice (Skt, Bus, Dev, Func);
        PchRasEnableDmiError (
          mMailBox2->PcieInitPar.PcieErrEn,
          mMailBox2->PcieInitPar.PcieCorrErrEn,
          mMailBox2->PcieInitPar.PcieUncorrErrEn,
          ~mMailBox2->PcieInitPar.PcieAerCorrErrBitMap,
          ~(mMailBox2->PcieInitPar.PcieAerNfatErrBitMap | mMailBox2->PcieInitPar.PcieAerFatErrBitMap),
          mMailBox2->PcieInitPar.PcieAerFatErrBitMap
          );
        PchEnableErrors ();
      } else {
        IioPcieMaskErrors (Skt, Bus, Dev, Func);
      }

      if (mMailBox2->IioInitPar.IioPoisonEn) {
        IioEnablePoison (Skt, Bus, Dev, Func);
      } else {
        IioDisablePoison (Skt, Bus, Dev, Func);
      }
      break;
    case RlinkDevice:
      if (NodeDev->Common.DevSpecInfo.DsiDmi.pSouthIeh != NULL) {
        SatalliteIehInit (NodeDev->Common.DevSpecInfo.DsiDmi.pSouthIeh);
      }
      PcieClearDeviceStatus (Skt, Bus, Dev, Func, NULL, Severity);
      if (mMailBox2->IioInitPar.IioDmiErrorEn) {
        PcieConfigDevice (Skt, Bus, Dev, Func, NULL);
        PchRasEnableDmiError (
            mMailBox2->PcieInitPar.PcieErrEn,
            mMailBox2->PcieInitPar.PcieCorrErrEn,
            mMailBox2->PcieInitPar.PcieUncorrErrEn,
            ~mMailBox2->PcieInitPar.PcieAerCorrErrBitMap,
            ~(mMailBox2->PcieInitPar.PcieAerNfatErrBitMap | mMailBox2->PcieInitPar.PcieAerFatErrBitMap),
            mMailBox2->PcieInitPar.PcieAerFatErrBitMap
            );
      }
      break;
    case CbDMADevice:
      IioClearDmaErrors (Skt, Bus, Dev, Func, Severity);
      PcieClearDeviceStatus (Skt, Bus, Dev, Func, NULL, Severity);
      if (mMailBox2->IioInitPar.IioDmaErrorEn) {
        IioEnableDmaErrors (Skt, Bus, Dev, Func);
      }
      PcieConfigDevice (Skt, Bus, Dev, Func, NULL);
      break;
    case VTDDevice:
      ClearVtdStatus (Skt, Bus, Dev, Func, Severity);
      IioEnableVtd (Skt, Bus, Dev, Func);
      break;
    case RASDevice:
      ClearRasDevice (Skt, Bus, Dev, Func, Severity);
      EnableRasDevice (Skt, Bus, Dev, Func);
      break;
    case IioRootPort:
      if (mMailBox2->PcieInitPar.PcieErrEn) {
        IioRootPortErrorEnable (Skt, Bus, Dev, Func);
        if (mMailBox2->IioInitPar.IioPoisonEn) {
          IioEnablePoison (Skt, Bus, Dev, Func);
        } else {
          IioDisablePoison (Skt, Bus, Dev, Func);
        }
        if (IsLerSupported (Skt, Bus, Dev, Func, NULL)) {
          if (mMailBox2->IioInitPar.LerEn) {
            IioLerEnable (Skt, Bus, Dev, Func);
            if (mMailBox2->IioInitPar.DisableLerMAerrorLogging == 0) {
              IioLerDisMcaErrLog (Skt);
            }
          }
        }
      } else {
        IioPcieMaskErrors (Skt, Bus, Dev, Func);
      }
      //
      // During Root Port Initialization, due to pci probe, UR/ADVNF error will happen,
      // although we cleared device status for this stituation, but root error status is
      // keeping there, need to clear again.
      //
      ClearRootPortStatus (Skt, Bus, Dev, Func, Severity);
      RasAcpiParamAddRootPortEntry (Skt, Bus, Dev, Func);
      break;
    case PcieRootPort:
      PcieConfigDeviceAndSubDevices (Skt, Bus, Dev, Func, NULL);
      RasAcpiParamAddRootPortEntry (Skt, Bus, Dev, Func);
      break;
    default:
      PcieClearDeviceStatus (Skt, Bus, Dev, Func, NULL, Severity);
      PcieConfigDevice (Skt, Bus, Dev, Func, NULL);
      break;
  }

}

/**
  This routine purpose is to enable IIO error for socket.

  @param [IN] Skt  -- socket index

  @retval none
**/

VOID
EFIAPI
IehErrorEnableSocket (
  IN  UINT8    Skt
  )
{
  UINT8                             BitIdx;
  UINT8                             MaxBitIdx;
  UINT8                             Index;
  UINT8                             Bus;
  UINT8                             Dev;
  UINT8                             Func;
  IEH_DEVICE_TYPE                   DevType;
  NODE_DEVICE                       **NodeListHead;
  NODE_DEVICE                       *NodeList;

  //
  // Assign IEH bus number.
  //
  RAS_DEBUG((LEVEL_BASIC_FLOW," [Init Global IEH] on Skt:0x%x \n", Skt));
  ProgramIehBusNumber (&mGlobalIEH[Skt]);

  NodeListHead = (NODE_DEVICE **)mGlobalIEH[Skt].GlobalIEH.ConnectedDeviceList;
  MaxBitIdx = mGlobalIEH[Skt].GlobalIEH.MaxBitIdx;

  //
  // Enable each direct connected device include satellite IEH
  //
  for (BitIdx = 0; BitIdx <= MaxBitIdx; BitIdx++) {
    //
    // Get Device List that connected to this Bit
    //
    NodeList = NodeListHead[BitIdx];
    if (BitIdx == 0) {
      IehInternalDeviceInit (&mGlobalIEH[Skt], NodeList);
    } else {
      //
      // For other bits , each bit may have one or more direct connect device(IEH End Point) or one satelliteIeh
      //
      for (Index = 0; Index < NodeList[0].Common.DevCount; Index++){
        //
        // Skip invalid Node
        //
        if (NodeList[Index].Common.Valid == 0) {
          continue;
        }
        DevType = NodeList[Index].Common.Type;
        Bus = NodeList[Index].Common.Bus;
        Dev = NodeList[Index].Common.Dev;
        Func = NodeList[Index].Common.Func;
        //
        //skip hidden devices.
        //
        if (!PcieIsDevicePresent (Skt, Bus, Dev, Func, NULL)) {
          RAS_DEBUG((LEVEL_BASIC_FLOW,"  [Init Satallite IEH] Device on BitIdx:0x%x doesn't exist \n", BitIdx));
          continue;
        }
        //
        // if it is not North/south satellite IEH, it must be Ieh End Point
        //
        if (DevType == SatelliteIehNorth || DevType == SatelliteIehSouth) {
          RAS_DEBUG((LEVEL_BASIC_FLOW,"  [Init Satallite IEH] on BitIdx:0x%x \n", BitIdx));
          SatalliteIehInit (&NodeList[Index]);
        } else {
          IehEndPointDevInit (&NodeList[Index]);
        }
      }
    }
  }

  Wa_1407595452 (Skt);

  RAS_DEBUG((LEVEL_BASIC_FLOW, " Clear IEH Global Error Status before enabling IEH errors\n"));
  ClearIehErrorStatus (&mGlobalIEH[Skt]);

  Wa_1407595452(Skt);
  //
  // Enable Global Error Control
  //
  EnableSysCtl (&mGlobalIEH[Skt]);
  EnableSysMap (&mGlobalIEH[Skt]);
  EnableErrorPin (Skt, mGlobalIEH[Skt].Common.Bus, mGlobalIEH[Skt].Common.Dev, mGlobalIEH[Skt].Common.Func);

}

/**
  This routine purpose is to enable IIO error handling for whole system.

  @param   none
  @retval  none
**/

VOID
EFIAPI
IehErrorEnable (
  VOID
  )
{
  UINT8          Skt;

  for ( Skt = 0; Skt < MAX_SOCKET; Skt++) {
    if (IsSocketPresent (Skt) == FALSE || mGlobalIEH[Skt].Common.Valid == 0) {
      continue;
    }

    RAS_DEBUG((LEVEL_BASIC_FLOW,"[IEH INIT] Init Socket:0x%x \n", Skt));
    IehErrorEnableSocket (Skt);
    RAS_DEBUG((LEVEL_BASIC_FLOW,"[IEH INIT] Init Socket:0x%x  End\n", Skt));
  }

}

/**
  This routine purpose is to enable satellite IEH.

  @param [IN]   SatelliteIeh  -- Satellite IEH pointer

  @retval  none

**/

VOID
EFIAPI
SatalliteIehInit (
  NODE_DEVICE * SatelliteIeh
  )
{
  UINT8                           BitIdx;
  UINT8                           MaxBitIdx;
  UINT8                           Skt;
  UINT8                           Bus;
  UINT8                           Dev;
  UINT8                           Func;
  UINT8                           Index;
  NODE_DEVICE                     **NodeListHead;
  NODE_DEVICE                     *NodeList;

  //
  // Get Bitmap based Node List Pointers and how many bits are connected to devices
  //
  NodeListHead = (NODE_DEVICE **)SatelliteIeh->SatelliteIEH.ConnectedDeviceList;
  MaxBitIdx = SatelliteIeh->SatelliteIEH.MaxBitIdx;

  Skt = SatelliteIeh->Common.Socket;
  Bus = SatelliteIeh->Common.Bus;
  Dev = SatelliteIeh->Common.Dev;
  Func = SatelliteIeh->Common.Func;

  RAS_DEBUG((LEVEL_BASIC_FLOW,"  [Init Satallite IEH] S:%x B:0x%x D:0x%x F:0x%x \n", Skt, Bus, Dev, Func));
  //
  // Program IEH Bus Number and BitMap
  //
  ProgramIehBusNumber (SatelliteIeh);
  SetSatIehBitmap (SatelliteIeh);

  //
  // Enable each direct connected device to each bit
  //
  for (BitIdx = 0; BitIdx <= MaxBitIdx; BitIdx++) {
    //
    // Get Device List that connected to this Bit
    //
    NodeList = NodeListHead[BitIdx];
    //
    // IEH Local Device and RCiEP device always sits on Bit 0
    //
    if (BitIdx == 0) {
      IehInternalDeviceInit (SatelliteIeh, NodeList);
    } else {
      //
      // For other bits , each bit may have one or more direct connect device(IEH End Point)
      //
      for (Index = 0; Index < NodeList[0].Common.DevCount; Index++) {
        //
        //skip hidden devices.
        //
        if (!PcieIsDevicePresent(Skt, NodeList[Index].Common.Bus, NodeList[Index].Common.Dev,
                                                      NodeList[Index].Common.Func, NULL)) {
          RAS_DEBUG ((LEVEL_BASIC_FLOW,"  [Init Satallite IEH] Device on BitIdx:0x%x doesn't exist \n", BitIdx));
          continue;
        }
        IehEndPointDevInit (&NodeList[Index]);
      }
    }
  }

  RAS_DEBUG((LEVEL_BASIC_FLOW, "  Clear IEH Global Error Status before enabling IEH error...\n"));
  ClearIehErrorStatus (SatelliteIeh);

  //
  // Satellite IEH level error enable
  //
  EnableSysCtl (SatelliteIeh);

  RAS_DEBUG((LEVEL_BASIC_FLOW,"  [Init Satallite IEH] S:%x B:0x%x D:0x%x F:0x%x  End \n", Skt, Bus, Dev, Func));
}

/**
  This function will enable Ieh error reporting.

  @param[in] Protocol        Protocol unique ID.
  @param[in] Interface            Interface instance.
  @param[in] Handle      The handle on which the interface is installed..

  @retval    Status.
**/
EFI_STATUS
EFIAPI
IioErrorEnableCallBack (
  IN      CONST EFI_GUID   *Protocol,
  IN      VOID             *Interface,
  IN      EFI_HANDLE        Handle
  )
{
  IehErrorEnable ();

  if (mMailBox2->IioInitPar.IioErrorEn == 1) {
    if (mMailBox2->IioInitPar.IoMcaEn) {
      IioCpuEnableIoMca ();
      UboxEnableIoMca (mMailBox2->IioInitPar.IioSev1Pcc);
    } else {
      UboxDisableIoMca ();
    }
  }

  return EFI_SUCCESS;
}
