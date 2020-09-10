/** @file
  Implementation of IEH error handling.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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
#include <Library/PcieStdLib.h>
#include <Library/WorkaroundLib.h>
#include <Library/WheaSciLib.h>
#include <Library/PlatPolicyLib.h>
#include <Library/PerformanceLib.h>
#include <IndustryStandard/Acpi.h>

extern NODE_DEVICE   *mGlobalIEH;
extern RAS_MAIL_BOX  *mMailBox2;


/**
  This routine purpose is to check IEH global error for particular socket.

  @param  Socket  -- socket index.
  @param  Severity  -- IEH error severity
  @retval TRUE -- error happens at IEH system; FALSE -- no error happens at IEH system

**/
BOOLEAN
EFIAPI
GetGlobalIEHError (
  IN UINT8       Socket,
  OUT UINT32     *Severity
  )
{
  UINT8    Bus;
  UINT8    Device;
  UINT8    Function;
  UINT32   SysSts;
  UINT32   SysCtl;
  UINT32   GlobalEventStatus;

  Bus = mGlobalIEH[Socket].Common.Bus;
  Device = mGlobalIEH[Socket].Common.Dev;
  Function = mGlobalIEH[Socket].Common.Func;

  SysSts = GetGlobalSysEvtSts (Socket, Bus, Device, Function, TRUE);
  SysCtl = GetGlobalSysEvtCtl (Socket, Bus, Device, Function, TRUE);
  GlobalEventStatus = SysCtl & SysSts;

  if (GlobalEventStatus & IEH_GSYSEVTSTS_MASK) {
    *Severity = GlobalEventStatus & IEH_GSYSEVTSTS_MASK;
    return TRUE;
  }

  return FALSE;
}

/**
  This routine purpose is to check IEH global error for all sockets.

  @param  None
  @retval UINT32 All the Global IEHs Global Error Status or together in the system

**/
UINT32
EFIAPI
AnyIehErrorHappen (
  VOID
  )
{
  UINT8    Skt;
  UINT32   SktSysEvtSts = 0;
  UINT32   GlbSysEvtSts = 0;

  for (Skt = 0; Skt< MAX_SOCKET; Skt++) {
    if (IsSocketPresent (Skt)) {
      if (GetGlobalIEHError (Skt, &SktSysEvtSts)) {
        GlbSysEvtSts |= SktSysEvtSts;
      }
    }
  }
  return GlbSysEvtSts;
}

/**
  This routine is to process a single IEH Local error source

  @param  Ieh -- Which IEH we are proccessing
  @param  LclDev -- Local Device Node that report this error
  @param  ErrSrcId -- Local Error Source ID
  @param  ErrSeverity  --  Severity input

  @retval none

**/
VOID
EFIAPI
IehLocalErrSrcXHandler(
  IN  NODE_DEVICE * Ieh,
  IN  NODE_DEVICE * LclDev,
  IN  UINT8       ErrSrcId,
  IN  UINT8       ErrSeverity
)
{
  UINT32              ErrorStatus;
  PCI_EXT_MM_INFO     MmInfo;
  //
  // Clear IEH Local Error status and report local error log
  //
  ErrorStatus = GetIehLocalErrSrcStatus (Ieh, ErrSrcId, ErrSeverity);
  if (ErrorStatus != 0) {
    RAS_DEBUG((LEVEL_BASIC_FLOW,"  Clearing Local Error Status for Error Source %d, Error Severity %d,  Error Status %d\n", ErrSrcId, ErrSeverity, ErrorStatus));
    ReportIehErrorLog (Ieh->Common.Socket, Ieh->Common.Type, Ieh->Common.Stack, ErrorStatus, ErrSrcId, ErrSeverity);
    ClearIehLocalErrorStatus (Ieh, ErrSrcId, ErrSeverity);
  }

  //
  // Clear specific local device error status
  //
  switch (LclDev->IehLocalDevice.IehLclDevType) {
    case PsfDino:
      break;
    case PsfPciePi5:
      break;
    case PsfHcX:
      break;
    case PcieGen4DmiPi5:
      MmInfo.PciExtMmType = PCI_EXT_MM_TYPE_RCRB;
      MmInfo.PciExtMmSubType = PCI_EXT_MM_SUB_TYPE_RCRB_DMI;
      MmInfo.Stack = LclDev->Common.Stack;
      PcieDeviceErrorHandler (LclDev->Common.Socket, 0, 0, 0, &MmInfo, ErrSeverity);
      break;
    case PcieGen5IalPi5:
      if (LclDev->IehLocalDevice.LclSpecInfo.CxlDevInfo.CxlValid) {
        CxlDpUpErrorHandler (LclDev->Common.Socket, LclDev->Common.Stack, ErrSeverity);
      }
      break;
    case SpdI3cBus:
      SmbusRecoverySmiHandler (Ieh->Common.Socket, LclDev->Common.SbPortId);
      break;
    case CxpSmBus:
      break;
    default:
      break;
  }
}

/**
  This routine is to process IEH Local error source

  @param  Ieh -- Which IEH we are proccessing
  @param  NodeList -- Device array that include IEH Local error source and RCiEP devices
  @param  ErrSeverity  --  Severity input

  @retval none

**/
VOID
EFIAPI
IehLocalErrorHandler(
  IN  NODE_DEVICE * Ieh,
  IN  NODE_DEVICE * NodeList,
  IN  UINT8        ErrSeverity
)
{
  UINT8                             Socket;
  UINT8                             Bus;
  UINT8                             Device;
  UINT8                             Function;
  UINT32                            UncErrSts;
  UINT32                            CorErrSts;
  UINT32                            RootErrSts;
  UINT8                             LclErrSrc;
  UINT8                             ErrSrcId;
  UINT8                             StartErrSrcId;
  UINT8                             EndErrSrcId;
  UINT8                             ErrType;
  UINT8                             IehVer;
  UINT8                             i;
  EFI_STATUS                        Status;
  BOOLEAN                           RcecSupport;
  BOOLEAN                           InternalError = FALSE;

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

  Socket = Ieh->Common.Socket;
  Bus = Ieh->Common.Bus;
  Device = Ieh->Common.Dev;
  Function = Ieh->Common.Func;

  if (RcecSupport) {
    //
    //  Handle UNC firstly
    //
    if (ErrSeverity & (IEH_NON_FATAL_ERROR | IEH_FATAL_ERROR)) {
      //
      // Check if it is local error, if yes, it is not RCEC error
      //
      UncErrSts = PcieAerGetUncErrSts (Socket, Bus, Device, Function, NULL);
      if (UncErrSts & B_PCIE_AER_UES_UNC_INT_ERR) {
        InternalError = TRUE;
        //
        // Check if multiple NON-FATAL/FATAL ERROR happened
        //
        Status = PcieAerGetRpErrSts (Socket, Bus, Device, Function, NULL, &RootErrSts);
        RAS_ASSERT (Status == EFI_SUCCESS);
        if (RootErrSts & B_PCIE_AER_ROOT_ERR_STS_MUL_FAT_NONFAT){
          StartErrSrcId = 0;
          EndErrSrcId = LclErrSrc;
        } else {
          //
          // if only single device error, check Local UNC First Error pointer to get the error source
          //
          GetIehLocalUncFirstErr (Ieh, &StartErrSrcId, &ErrType);
          EndErrSrcId = StartErrSrcId;
        }
      }
    }
    //
    // Then Process Correctable error
    //
    if (ErrSeverity & IEH_CORRECT_ERROR) {
      CorErrSts = PcieAerGetCorrErrSts (Socket, Bus, Device, Function, NULL);
      //
      // Check if it is local error, if yes, it is not RCEC error
      //
      if (CorErrSts & B_PCIE_AER_CES_COR_INT_ERR) {
          InternalError = TRUE;
          //
          // no correctable error for IEH Internal and PSF error , skip Local error source 0
          //
          StartErrSrcId = 1;
          EndErrSrcId = LclErrSrc;
      }
    }
  } else if (IehVer == IEH_VER_RCEC_DIS) {
    //
    // Wave3 Global IEH that doesn't have AER/RCEC support, but have multiple sets of Local error registers
    //
    InternalError = TRUE;
    StartErrSrcId = 0;
    EndErrSrcId = LclErrSrc;
  } else {
    //
    // For Wave1/2, Only one set of local error registers
    //
    InternalError = TRUE;
    StartErrSrcId = 0;
    EndErrSrcId = 0;
  }

  //
  // Handle Local Error Sources
  //
  if (InternalError) {
    for (ErrSrcId = StartErrSrcId; ErrSrcId <= EndErrSrcId; ErrSrcId++) {
      //
      // Find Corresponding Local device Node
      //
      for ( i = 0; i < NodeList[0].Common.DevCount; i++) {
        if (NodeList[i].Common.Type == IehLocalDevice ) {
          if (NodeList[i].IehLocalDevice.ErrorSourceId == ErrSrcId) {
            IehLocalErrSrcXHandler (Ieh, &NodeList[i], ErrSrcId, ErrSeverity);
          }
        }
      }
    }
  }
}

/**
  This routine is to Process RCiEP devices Error that connect to this IEH RCEC

  @param  Ieh -- Which IEH we are proccessing
  @param  NodeList -- Device array that include IEH Local error source and RCiEP devices
  @param  ErrSeverity  --  Severity input

  @retval none

**/
VOID
EFIAPI
IehRciepDevicesErrorHandler(
  IN  NODE_DEVICE * Ieh,
  IN  NODE_DEVICE * NodeList,
  IN  UINT8      ErrSeverity
)
{
  UINT8         Socket;
  UINT8         Bus;
  UINT8         Device;
  UINT8         Function;
  UINT8         RcecBus;
  UINT8         RcecDevice;
  UINT8         RcecFunction;
  UINT8         Index;
  UINT32        RootErrSts;
  UINT32        AerErrSid;
  EFI_STATUS    Status;

  Socket = Ieh->Common.Socket;
  RcecBus = Ieh->Common.Bus;
  RcecDevice = Ieh->Common.Dev;
  RcecFunction = Ieh->Common.Func;

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "IehRciepDevicesErrorHandler - Socket %x, Bus %x, device %x, fuction %x\n", Socket, RcecBus, RcecDevice, RcecFunction));

  //
  // Check if system error reporting is enabled for this RCEC device
  //
  if (!IsPcieRootPortErrReportingEnabled (Socket, RcecBus, RcecDevice, RcecFunction, NULL)) {
    return ;
  }

  Status = PcieAerGetRpErrSts (Socket, RcecBus, RcecDevice, RcecFunction, NULL, &RootErrSts);
  RAS_ASSERT (Status == EFI_SUCCESS);
  //
  //  Loop COR and NON-FAT/FAT Error Severity
  //
  for (Index = 0; Index < 2; Index++) {
    //
    // Check how many errors happened
    //
    switch ((RootErrSts >> (Index*2)) & (BIT1 + BIT0)) {
      case NO_ERROR:
        //
        // There is no error on this severity, try next severity
        //
        continue;
      case SINGLE_ERROR:
        //
        // if it is single  error, get the error source from AER Error Source ID
        //
        AerErrSid = PcieAerGetErrSid (Socket, RcecBus, RcecDevice, RcecFunction, NULL);
        AerErrSid = AerErrSid >> (Index*16);
        Bus = (UINT8) (AerErrSid >> 8);
        Device = (UINT8) ((AerErrSid >> 3) & 0x1f);
        Function = (UINT8) (AerErrSid  & 0x07);
        //
        //  This is RCiEP or RCEC device error
        //
        PcieDeviceErrorHandler (Socket, Bus, Device, Function, NULL, ErrSeverity);
        break;
      case MULTIPLE_ERRORS:
        //
        // Check each RCiEP devices and handle the error
        //
        PcieRcecMultiErrorsHandler (Socket, RcecBus, RcecDevice, RcecFunction, NULL, ErrSeverity);
        break;
      default:
        RAS_DEBUG ((LEVEL_BASIC_FLOW, "IehRciepDevicesErrorHandler invalid RootErrorStatus\n", Socket, RcecBus, RcecDevice, RcecFunction));
        RAS_ASSERT (FALSE);
        break;
    }
  }
  //
  // Clear RPERRSTS
  //
  if (RootErrSts & 0xf) {
    PcieAerClearRootErrSts (Socket, RcecBus, RcecDevice, RcecFunction, NULL, ErrSeverity);
  }
}

/**
  This routine purpose is to handle internal device error.
  @param  [IN]   Ieh -- IEH device pointer
  @param  [IN]   IEHDevice  -- IEH device pointer
  @param  [IN]   ErrSeverity  --  Severity input

  @retval none

**/
VOID
EFIAPI
InternalDeviceHandler (
  IN  NODE_DEVICE   * Ieh,
  IN  NODE_DEVICE   *NodeList,
  IN  UINT8         ErrSeverity
  )
{
  BOOLEAN             RcecSupport;

  IehLocalErrorHandler (Ieh, NodeList, ErrSeverity);

  RcecSupport = Ieh->Common.Type == GlobalIeh ? Ieh->GlobalIEH.RcecSupport : Ieh->SatelliteIEH.RcecSupport;
  //
  // For the IEH that support RCEC
  //
  if (RcecSupport){
    IehRciepDevicesErrorHandler(Ieh, NodeList, ErrSeverity);
  }
}


/**
  This routine purpose is to handle device level error, not include any IEH device.
  Device scope is root port/DMI/CBDMA/FXR...  if any new device have specific error handling beyond PCIE AER,
  please add hook here.

  @param  [IN]   NodeDev  -- IEH End Point Node pointer
  @param  [IN]   SevPerStack  --  Severity input
  @retval none

**/
VOID
EFIAPI
IehEndPointErrorHandler (
  IN     NODE_DEVICE    *NodeDev,
  IN     UINT8           SevPerStack
  )
{
  UINT8               Socket;
  UINT8               Stack;
  UINT8               Bus;
  UINT8               Device;
  UINT8               Function;
  IEH_END_POINT_TYPE  IehEndPointType;
  NODE_DEVICE         *SouthIeh;
  UINT16              SouthIehBdf;
  UINT32              AerErrSid;

  Socket = NodeDev->Common.Socket;
  Stack =  NodeDev->Common.Stack;
  Bus    =  NodeDev->Common.Bus;
  Device =  NodeDev->Common.Dev;
  Function =  NodeDev->Common.Func;

  RAS_DEBUG((LEVEL_BASIC_FLOW,"  [Device Error] error on skt:0x%x Bus:0x%x Device:0x%x func:0x%x  \n", Socket, Bus, Device, Function));

  IehEndPointType = NodeDev->IehEndPoint.IehEndPointType;

  switch (IehEndPointType) {
    case VTDDevice:
      IioIntDevErrHandler (Socket, Stack, Bus, Device, Function, SevPerStack, RasIioInternalErrorVtd);
      break;
    case RASDevice:
      IioIntDevErrHandler (Socket, Stack, Bus, Device, Function, SevPerStack, RasIioInternalErrorItc);
      IioIntDevErrHandler (Socket, Stack, Bus, Device, Function, SevPerStack, RasIioInternalErrorOtc);
      IioIntDevErrHandler (Socket, Stack, Bus, Device, Function, SevPerStack, RasIioInternalErrorIrp);
      IioIntDevErrHandler (Socket, Stack, Bus, Device, Function, SevPerStack, RasIioInternalErrorRing);
      break;
    case CbDMADevice:
      IioIntDevErrHandler (Socket, Stack, Bus, Device, Function, SevPerStack, RasIioInternalErrorCBDma);
      //
      // ICX/SNR/SPR CbDMA Device support AER compability.
      //
      PcieDeviceErrorHandler (Socket, Bus, Device, Function, NULL, SevPerStack);
      break;
    case M2PCIeDevice:
      IioIntDevErrHandler (Socket, Stack, Bus, Device, Function, SevPerStack, RasIioInternalErrorM2PCIE);
      break;
    case IioRootPort:
    case PcieRootPort:
      IioPcieLeakyBucketHandler (Socket, Bus, Device, Function, SevPerStack);
      PcieRootPortErrorHandler (Socket, Bus, Device, Function, NULL, SevPerStack);
      IioXpErrHandler  (Socket, Bus, Device, Function, SevPerStack);
      break;
    case DMIDevice:
      PcieDmiDeviceErrorHandler (Socket, Bus, Device, Function, NULL, SevPerStack);
      IioXpErrHandler  (Socket, Bus, Device, Function, SevPerStack);
      //
      //handle PCH part
      //
      PchErrorHandler (SevPerStack);
      PchDmiErrorHandler ();
      break;
    case RlinkDevice:
      SouthIeh = NodeDev->Common.DevSpecInfo.DsiDmi.pSouthIeh;
      if (SouthIeh != NULL) {
        SouthIehBdf = (SouthIeh->Common.Bus << 8) + (SouthIeh->Common.Dev << 3) + (SouthIeh->Common.Func);
        AerErrSid = PcieAerGetErrSid (Socket, Bus, Device, Function, NULL);
        if (((SevPerStack == IEH_CORRECT_ERROR) && ((AerErrSid & 0xFFFF) == SouthIehBdf)) ||
            (((SevPerStack == IEH_NON_FATAL_ERROR) || (SevPerStack == IEH_FATAL_ERROR)) &&
                                                                ((AerErrSid >> 16) == SouthIehBdf))) {
          IEHHandler (SouthIeh, SevPerStack);
        }
      }
      PcieDeviceErrorHandler (Socket, Bus, Device, Function, NULL, SevPerStack);
      PchDmiErrorHandler ();
      break;
    default:
      //
      //standard PCIE endpointer or legacy PCI Device.
      //
      IsPcieDevice (Socket, Bus, Device, Function, NULL) ? PcieDeviceErrorHandler (Socket, Bus, Device, Function, NULL, SevPerStack) : PciDeviceErrorHandler (Socket, Bus, Device, Function, NULL);
      break;
  }

  if (mMailBox2->IioInitPar.IioErrRegistersClearEn) {
    switch (IehEndPointType) {
      case VTDDevice:
        ClearVtdStatus (Socket, Bus, Device, Function, SevPerStack);
        PcieClearDeviceStatus (Socket, Bus, Device, Function, NULL, SevPerStack);
        break;
      case RASDevice:
        ClearRasDevice (Socket, Bus, Device, Function, SevPerStack);
        PcieClearDeviceStatus (Socket, Bus, Device, Function, NULL, SevPerStack);
        break;
      case CbDMADevice:
        IioClearDmaErrors (Socket, Bus, Device, Function, SevPerStack);
        PcieClearDeviceStatus (Socket, Bus, Device, Function, NULL, SevPerStack);
        break;
      case DMIDevice:
        IioXpClearErr (Socket, Bus, Device, Function);
        break;
      case IioRootPort:
      case PcieRootPort:
        ClearRootPortStatus (Socket, Bus, Device, Function, SevPerStack);
        break;
      default:
        PcieClearDeviceStatus (Socket, Bus, Device, Function, NULL, SevPerStack);
        break;
    }
  }
}

/**
  This routine purpose is to handle global IEH and north IEH.

  @param [IN]  IEH  -- pointer to global IEH or north IEH.
  @param [IN]  InputSeverity  -- The severity that this handler will process.

  @retval none

**/
VOID
EFIAPI
IEHHandler (
  IN  NODE_DEVICE   * IEH,
  IN  UINT32        InputSeverity
  )
{
  NODE_DEVICE       **NodeListHead;
  NODE_DEVICE       *NodeList;
  UINT8             Index;
  UINT8             DevIdx;
  IEH_DEVICE_TYPE   Type;
  UINT32            GlobalEventStatus;
  UINT32            ExecuteStatus;
  UINT32            SeverityStatus;
  UINT32            ErrorStatus;
  UINT8             BitIdx;
  UINT8             MaxBitIdx;
  UINT8             Socket;
  UINT8             Bus;
  UINT8             Device;
  UINT8             Function;
  BOOLEAN           IsGlobalIeh = FALSE;

  if (IEH == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW,"[IEH] ERROR: IEH is NULL.\n"));
    RAS_ASSERT (IEH != NULL);
    return;
  }

  Socket = IEH->Common.Socket;
  Bus    =  IEH->Common.Bus;
  Device =  IEH->Common.Dev;
  Function =  IEH->Common.Func;

  if (IEH->Common.Type == GlobalIeh) {
    IsGlobalIeh = TRUE;
    NodeListHead = (NODE_DEVICE **)IEH->GlobalIEH.ConnectedDeviceList;
    MaxBitIdx = IEH->GlobalIEH.MaxBitIdx;
  } else {
    NodeListHead = (NODE_DEVICE **)IEH->SatelliteIEH.ConnectedDeviceList;
    MaxBitIdx = IEH->SatelliteIEH.MaxBitIdx;
  }

  GlobalEventStatus = GetGlobalSysEvtSts (Socket, Bus, Device, Function, IsGlobalIeh);

  Wa_2006679835_reset (IEH->Common.Type == SatelliteIehSouth, GlobalEventStatus);

  GlobalEventStatus = GlobalEventStatus & IEH_GSYSEVTSTS_MASK;
  ExecuteStatus = GlobalEventStatus & InputSeverity;

  if (ExecuteStatus == 0) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW,"[IEH] No error on Skt:0x%x Bus:0x%x Device:0x%x Function:0x%x\n", Socket, Bus, Device, Function));
    return;
  }

  //
  // To prevent additional interrupt.this code should put late of GSYSEVTSTS_IEH_GLOBAL_REG access, otherwise status register value will change.
  //
  DisableSysCtl (IEH);

  for (Index = 3; Index > 0; Index --) {
    SeverityStatus = ExecuteStatus & (1 << (Index -1));
    if (SeverityStatus == 0) {
      continue;
    }

    RAS_DEBUG((LEVEL_BASIC_FLOW,"[IEH] error found at IEH(S:0x%x B:0x%x D:0x%x F:0x%x) Sev: ", Socket, Bus, Device, Function));
    DisplaySeverityStatus (SeverityStatus, TRUE);

    //
    //handle higher severity error first.
    //
    if (SeverityStatus == IEH_FATAL_ERROR) {
      ErrorStatus = GetGlobalFatErrSts (Socket, Bus, Device, Function, IEH->Common.Type);
    } else if (SeverityStatus == IEH_NON_FATAL_ERROR) {
      ErrorStatus = GetGlobalNfatErrSts (Socket, Bus, Device, Function, IEH->Common.Type);
    } else if (SeverityStatus == IEH_CORRECT_ERROR) {
      ErrorStatus = GetGlobalCorErrSts (Socket, Bus, Device, Function, IEH->Common.Type);
    } else {
      continue;
    }

    RAS_DEBUG((LEVEL_BASIC_FLOW,"[IEH] ErrorStatus 0x%x, MaxBitIdx 0x%x\n", ErrorStatus, MaxBitIdx));

    //
    // Set output severity according to IEH severity, IEH end point error handler may override this
    // setting, it is allowed so that some workaround or platform policy take control
    //
    AdjustOutputSeverity (GetSevByErrType (SeverityStatus));

    for (BitIdx = 0; BitIdx <= MaxBitIdx; BitIdx++) {
      if (ErrorStatus & ((UINT32) 1 << BitIdx)) {
        NodeList = NodeListHead[BitIdx];
        //
        // IEH Local device and RCiEP devices always sit on bit0
        //
        if (BitIdx == 0) {
            InternalDeviceHandler (IEH, NodeList, (UINT8) SeverityStatus);
        } else {
          for (DevIdx = 0; DevIdx < NodeList[0].Common.DevCount; DevIdx++) {
            RAS_DEBUG((LEVEL_BASIC_FLOW,"[IEH] BitIdx 0x%x, ShareIdx 0x%x\n", BitIdx, DevIdx));
            //
            // Skip invalid Node
            //
            if (NodeList[DevIdx].Common.Valid == 0) {
              continue;
            }
            Type = NodeList[DevIdx].Common.Type;
            RAS_DEBUG((LEVEL_BASIC_FLOW,"[IEH] error device is (S:0x%x B:0x%x D:0x%x F:0x%x) BitIdx 0x%x, ShareIdx 0x%x ",
              NodeList[DevIdx].Common.Socket, NodeList[DevIdx].Common.Bus, NodeList[DevIdx].Common.Dev,
              NodeList[DevIdx].Common.Func, BitIdx, DevIdx));
            DisplayIehDevType (Type, FALSE);
            DisplayIehDevSubType (&NodeList[DevIdx], TRUE);

            switch (Type) {
            //
            // if it is North IEH or South IEH , the process is going to be recursive
            //
              case SatelliteIehNorth:
              case SatelliteIehSouth:
                IEHHandler (&NodeList[DevIdx], SeverityStatus);
                break;
              case IehEndPoint:
                //
                // Process individual Ieh End Point error
                //
                IehEndPointErrorHandler (&NodeList[DevIdx], (UINT8) SeverityStatus);
                break;
              default:
                break;
            }
          }
        }
        ClearIehErrorStatusPerBit (IEH, (UINT8) BitIdx, (UINT8) SeverityStatus);
      }
    }
  }
  //
  // Clear Error Pin status only when setup knob "IioErrRegistersClearEn" is enabled
  //
  if (IEH->Common.Type == GlobalIeh) {
    if (mMailBox2->IioInitPar.IioErrRegistersClearEn) {
      ClearErrorPinStatus (Socket, Bus, Device, Function);
    }
  }

  EnableSysCtl (IEH);
}

/**
  This routine purpose is to handle IEH error in whole system.
  This routine can be used to MCA driver for IOMCA handling.

  @param  [IN]  InputSeverity  - The severity that this handler will process
  @retval  none
**/
VOID
EFIAPI
IEHHandleError (
  IN  UINT32   InputSeverity
  )
{
  UINT8   Skt;
  UINT32  Severity;

  for (Skt = 0; Skt < MAX_SOCKET; Skt++) {
    if (!IsSocketPresent (Skt) || mGlobalIEH[Skt].Common.Valid == 0) {
      continue;
    }

    if (GetGlobalIEHError (Skt, &Severity)) {
      if (InputSeverity & Severity) {
        IEHHandler (&mGlobalIEH[Skt], InputSeverity);
      }
    }
  }
}

/**
  This routine purpose is to handle IEH error for IOMCA.
  This routine can be used to MCA driver for IOMCA handling.

  @param   NONE
  @retval  none
**/
VOID
EFIAPI
IOMCAErrorhandler (
  VOID
  )
{
  OverrideOutputSeverity (EFI_ACPI_6_2_ERROR_SEVERITY_NONE);
  IEHHandleError (FATAL_ERROR_TYPE | NONFATAL_ERROR_TYPE);
}

/**
   Ieh error enter pointer for non-IOMCA

  @param[in]      DispatchHandle  The unique handle assigned to this handler.
  @param[in]      Context         Standard SMI handler parameter; not used here.
  @param[in, out] CommBuffer      Standard SMI handler parameter; not used here.
  @param[in, out] CommBufferSize  Standard SMI handler parameter; not used here.

  @retval EFI_SUCCESS -- error handled.
**/
EFI_STATUS
EFIAPI
IioErrorDetectAndHandle (
  IN EFI_HANDLE             DispatchHandle,
  IN CONST VOID            *DispatchContext,
  IN OUT VOID              *CommBuffer,
  IN OUT UINTN             *CommBufferSize
  )
{
  UINT32   GlbSysEvtSts;



  GlbSysEvtSts = AnyIehErrorHappen ();
  //
  // If no error or NON COR error with IOMCA enabled, should not proceed
  //
  if (!GlbSysEvtSts || (((GlbSysEvtSts & COR_ERROR_TYPE) == 0) && mMailBox2->IioInitPar.IoMcaEn)) {
    return EFI_SUCCESS;
  }

  PERF_START (NULL, "SmmIehHandler", "RAS", 0);

  OverrideOutputSeverity (EFI_ACPI_6_2_ERROR_SEVERITY_NONE);
  if (mMailBox2->IioInitPar.IoMcaEn) {
    //
    // If IOMCA Enabled, IOMCA Handler will process NF/F error, this handler
    // only process Correcteable error
    //
    IEHHandleError (COR_ERROR_TYPE);
  } else {
    IEHHandleError (COR_ERROR_TYPE | FATAL_ERROR_TYPE | NONFATAL_ERROR_TYPE);
    //
    // Trigger NMI for Non-Fatal or Fatal Error in case IoMca disable, this is WHEA way notification
    // but also could signal NMI under shell.
    //
    if (GetOutputSeverity () == EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE
         || GetOutputSeverity () == EFI_ACPI_6_2_ERROR_SEVERITY_FATAL) {
      SignalWheaNmi();
    }
  }

  PERF_END (NULL, "SmmIehHandler", "RAS", 0);
  return EFI_SUCCESS;
}

