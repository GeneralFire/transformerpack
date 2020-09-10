/** @file
  Implementation of IEH common library.

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

#include "IehRasLibInternal.h"
#include <Library/CpuAndRevisionLib.h>

NODE_DEVICE   *mGlobalIEH = NULL;
RAS_MAIL_BOX  *mMailBox2 = NULL;

/**
  This routine purpose is to dump IEH error status.

  @param  [IN]  IEH      -- IEH pointer

  @retval  none
**/
VOID
EFIAPI
DumpIehErrorStatus (
  IN  NODE_DEVICE    *IEH
  )
{
  //
  // Dump Local status first
  //
  DumpIehLocalErrorStatus(IEH);
  //
  // Dump Global status
  //
  DumpIehGlobalErrorStatus(IEH);
}

/**
  This routine purpose is to dump IEH Node error status.

  @param  NodeList -- Device array that include IEH Local error source and RCiEP devices
  @param  DevCount -- How many device in this list
  @param  SatIehFlag -- TRUE it is satellite IEH

  @retval  none
**/
VOID
EFIAPI
DumpIehNodeErrorStatus (
  NODE_DEVICE    *NodeList,
  UINT8          DevCount,
  BOOLEAN        SatIehFlag
  )
{
  UINT8             Index;
  NODE_DEVICE       *NodeList2;
  NODE_DEVICE       *SouthIeh;
  UINT8             BitIdx;
  UINT8             Socket;
  UINT8             Bus;
  UINT8             Device;
  UINT8             Function;

  if (NodeList == NULL || DevCount == 0) {
    return;
  }

  for (Index = 0; Index < DevCount; Index++) {
    //
    // Skip invalid Node
    //
    if (NodeList[Index].Common.Valid == 0) {
      continue;
    }

    if (SatIehFlag) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW,"  "));
    }

    Socket    = NodeList[Index].Common.Socket;
    Bus       = NodeList[Index].Common.Bus;
    Device    = NodeList[Index].Common.Dev;
    Function  = NodeList[Index].Common.Func;

    RAS_DEBUG ((LEVEL_BASIC_FLOW, "  Dev:0x%x socket:0x%x Bus:0x%x Dev:0x%x Func:0x%x - ", Index, Socket, Bus, Device, Function));
    DisplayIehDevType (NodeList[Index].Common.Type, TRUE);

    switch (NodeList[Index].Common.Type) {
      case IehLocalDevice:
        break;
      case RCiEP:
        DumpPcieRegisters (Socket, Bus, Device, Function, NULL);
        break;
      case IehEndPoint:
        switch (NodeList[Index].IehEndPoint.IehEndPointType) {
          case IioRootPort:
          case PcieRootPort:
            DumpPcieBridgeRegisters (Socket, Bus, Device, Function, NULL);
            break;
          case PcieEndPoint:
          case DMIDevice:
            DumpPcieRegisters (Socket, Bus, Device, Function, NULL);
            break;
          case RlinkDevice:
            DumpPcieRegisters (Socket, Bus, Device, Function, NULL);
            SouthIeh = (NODE_DEVICE *)NodeList[Index].Common.DevSpecInfo.DsiDmi.pSouthIeh;
            if(SouthIeh != NULL) {
              RAS_DEBUG ((LEVEL_BASIC_FLOW," PCH Sat IEH Start \n"));
              DumpIehErrorStatus (SouthIeh);
              for (BitIdx = 0; BitIdx <= SouthIeh->SatelliteIEH.MaxBitIdx; BitIdx++) {
                NodeList2 = (NODE_DEVICE *)SouthIeh->SatelliteIEH.ConnectedDeviceList[BitIdx];
                DumpIehNodeErrorStatus (NodeList2, NodeList2[0].Common.DevCount, TRUE);
              }
              RAS_DEBUG ((LEVEL_BASIC_FLOW," PCH Sat IEH Done \n"));
            }
            break;
          default:
            break;
        }
        break;
      case SatelliteIehNorth:
      case SatelliteIehSouth:
        DumpIehErrorStatus (&NodeList[Index]);
        for (BitIdx = 0; BitIdx <= NodeList[Index].SatelliteIEH.MaxBitIdx; BitIdx++) {
          NodeList2 = (NODE_DEVICE *)NodeList[Index].SatelliteIEH.ConnectedDeviceList[BitIdx];
          RAS_DEBUG ((LEVEL_BASIC_FLOW, "    BitIndex :0x%x  DevCount on this bit:0x%x, \n", BitIdx, NodeList2[0].Common.DevCount));
          DumpIehNodeErrorStatus (NodeList2, NodeList2[0].Common.DevCount, TRUE);
        }
        break;
      default:
        break;
    }
  }
}

/**
  This routine purpose is to dump IEH Node error status.

  @param   none

  @retval  none
**/
VOID
EFIAPI
DumpIehSysErrorStatus (
  VOID
  )
{
  UINT8          Socket;
  NODE_DEVICE    *NodeList;
  UINT8          BitIdx;

  RAS_DEBUG ((LEVEL_FUNC_FLOW,"\n [IEH]-- Dump IEH System error status start -- \n"));

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (IsSocketPresent (Socket) == FALSE  || mGlobalIEH[Socket].Common.Valid == 0) {
      continue;
    }

    RAS_DEBUG ((LEVEL_FUNC_FLOW, "socket:%x  Bus:%x  Dev:%x  Func:%x ", mGlobalIEH[Socket].Common.Socket,
              mGlobalIEH[Socket].Common.Bus, mGlobalIEH[Socket].Common.Dev, mGlobalIEH[Socket].Common.Func));
    DisplayIehDevType (mGlobalIEH[Socket].Common.Type, TRUE);

    DumpIehErrorStatus (&mGlobalIEH[Socket]);

    for (BitIdx = 0; BitIdx <= mGlobalIEH[Socket].GlobalIEH.MaxBitIdx; BitIdx++) {
      NodeList = (NODE_DEVICE *)mGlobalIEH[Socket].GlobalIEH.ConnectedDeviceList[BitIdx];
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "  BitIndex :0x%x  DevCount on this bit:0x%x, \n", BitIdx, NodeList[0].Common.DevCount));
      DumpIehNodeErrorStatus (NodeList, NodeList[0].Common.DevCount, FALSE);
    }
  }

  RAS_DEBUG((LEVEL_FUNC_FLOW,"\n [IEH]-- Dump IEH System error status end  -- \n"));
}

/**
  This routine purpose is to clear IEH device error status for a single bit.

  @param  [IN]  IEH      -- IEH pointer
  @param  [IN]  Index    -- device bitmap in IEH
  @param  [IN]  severity -- error severity

  @retval  none
**/
VOID
EFIAPI
ClearIehErrorStatusPerBit (
  IN  NODE_DEVICE    *IEH,
  IN  UINT8           Index,
  IN  UINT8           Severity
  )
{
  UINT8           Socket;
  UINT8           Bus;
  UINT8           Device;
  UINT8           Function;
  IEH_DEVICE_TYPE Type;

  Socket = IEH->Common.Socket;
  Bus    =  IEH->Common.Bus;
  Device =  IEH->Common.Dev;
  Function =  IEH->Common.Func;
  Type = IEH->Common.Type;

  ClearIehErrorStatusPerBitSi (Socket, Bus, Device, Function, Index, Severity, Type);

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "Clear IEH error status S:0x%x B:0x%x D:0x%x F:0x%x Sev : ",
    Socket, Bus, Device, Function));
  DisplaySeverityStatus (Severity, TRUE);
}

/**
  This routine purpose is to Clean IEH device error status.

  @param  [IN]  IEH      -- IEH pointer

  @retval  none
**/
VOID
EFIAPI
ClearIehErrorStatus (
  IN  NODE_DEVICE    *IEH
  )
{
  UINT8           Socket;
  UINT8           Bus;
  UINT8           Device;
  UINT8           Function;
  IEH_DEVICE_TYPE Type;

  Socket = IEH->Common.Socket;
  Bus    =  IEH->Common.Bus;
  Device =  IEH->Common.Dev;
  Function =  IEH->Common.Func;
  Type = IEH->Common.Type;

  ClearIehGlobalErrorStatus (Socket, Bus, Device, Function, Type);

}

/**
  get device counter. wave1/2 -- get from lookup table; wave 3 -- read from regiser.

  @param   Ieh  -- ieh device pointer.

  @retval  device counter value.

**/
UINT8
EFIAPI
GetDevCount (
  IN  NODE_DEVICE   *Ieh
  )
{
  return GetMaxBitMap (Ieh) + 1;
}

/**
  get Ieh stack max direct connect device number.

  @param   Ieh  -- ieh device pointer.

  @retval  number of direct connect device.

**/
UINT8
EFIAPI
GetMaxBitMap (
  IN  NODE_DEVICE  *Ieh
  )
{
  UINT8  MaxBitMap;

  MaxBitMap = 1;

  while (GetIehLookupTableEntry (Ieh->Common.Stack, MaxBitMap, 0) != NULL) {
    MaxBitMap ++;
  }

  MaxBitMap --;
  return MaxBitMap;
}

/**
  To return how many device connect to the same bit.

  @param   Ieh  -- ieh device pointer.
  @param   BitIndex  -- bitmap index.

  @retval  Device count that connect to BitIndex

**/
UINT8
EFIAPI
GetDevCountOnBit (
  IN  NODE_DEVICE     *Ieh,
  IN  UINT8           BitIndex
  )
{
  LOOKUP_TBL_ENTRY    *LookupTblEntry;
  UINT8               Counter = 0;

  LookupTblEntry = GetIehLookupTableEntry (Ieh->Common.Stack, BitIndex, 0);
  if ( LookupTblEntry != NULL) {
    while (LookupTblEntry->IehDevType != IehDevTypeEnd) {
      if (LookupTblEntry->BitIndex == BitIndex && LookupTblEntry->Stack == Ieh->Common.Stack) {
                    Counter++;
      }
      LookupTblEntry++;
    }
    return  Counter;
  } else {
    RAS_DEBUG ((LEVEL_FUNC_FLOW,"[IEH] ERROR: Did not find look uptable entry for BitIndex 0x%x of Stack 0x%x. \n",
      BitIndex, Ieh->Common.Stack));
    return 0;
  }
}

/**
  Display IEH device Type to serial port.

  @param   IehDevType  -- Ieh device Type
  @param   CrLf        -- output CRLF or not

  @retval  None

**/
VOID
EFIAPI
DisplayIehDevType (
  IN  IEH_DEVICE_TYPE IehDevTye,
  IN  BOOLEAN         CrLf
  )
{
  switch (IehDevTye){
    case GlobalIeh:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"Global IEH "));
      break;

    case SatelliteIehNorth:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"North Sat IEH "));
      break;

    case SatelliteIehSouth:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"South Sat IEH "));
      break;

    case IehEndPoint:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"Ieh End Point "));
      break;

    case RCiEP:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"RCiEP "));
      break;

    case IehLocalDevice:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"Ieh Local Device "));
      break;

    default:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"Unkown IEH Dev Type "));
      break;
  }
  if (CrLf) {
    RAS_DEBUG((LEVEL_BASIC_FLOW,"\n"));
  }
}

/**
  Display Device Type that direct connect to IEH to serial port.

  @param   IehEndPointType  -- Device Type that direct connect to IEH
  @param   CrLf        -- output CRLF or not

  @retval  None

**/
VOID
EFIAPI
DisplayIehEndPointDevType (
  IN  IEH_END_POINT_TYPE  IehEndPointType,
  IN  BOOLEAN             CrLf
  )
{
  switch (IehEndPointType) {
    case IioRootPort:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"iio root port "));
      break;

    case PcieRootPort:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"Pcie root port "));
      break;

    case PcieEndPoint:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"PCIe endpoint "));
      break;

    case InternalDevice:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"IEH internal device"));
      break;

    case DMIDevice:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"DMI "));
      break;

    case RlinkDevice:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"Rlink "));
      break;

    case CbDMADevice:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"CBDMA "));
      break;

    case VTDDevice:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"VTD "));
      break;

    case RASDevice:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"RAS Device "));
      break;

    case M2PCIeDevice:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"M2PCIe "));
      break;

    case SPDDevice:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"SPD SMBUS device "));
      break;

    case McDDRDevice:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"McDDR  "));
      break;

    case VppDevice:
      RAS_DEBUG((LEVEL_BASIC_FLOW,"VPP device "));
      break;

    default:
      RAS_DEBUG((LEVEL_BASIC_FLOW," Unknown IEH End Point device "));
      break;
  }
  if (CrLf) {
    RAS_DEBUG((LEVEL_BASIC_FLOW,"\n"));
  }
}

/**
  Display IEH Local device Type to serial port.

  @param   IehLclDevType  -- Ieh Local device Type
  @param   CrLf        -- output CRLF or not

  @retval  None

**/
VOID
EFIAPI
DisplayLocalDeviceType (
  IN  IEH_LOCAL_DEV_TYPE  IehLclDevType,
  IN  BOOLEAN             CrLf
  )
{
  if (IehLclDevType == PsfDino) {
    RAS_DEBUG((LEVEL_BASIC_FLOW,"PSF Dino "));
  }

  if (IehLclDevType == PsfPciePi5) {
    RAS_DEBUG((LEVEL_BASIC_FLOW,"PSF PCIE Pi5 "));
  }

  if (IehLclDevType == PsfHcX) {
    RAS_DEBUG((LEVEL_BASIC_FLOW,"PSF HCx "));
  }

  if (IehLclDevType == PcieGen4DmiPi5) {
    RAS_DEBUG((LEVEL_BASIC_FLOW,"PCIE GEN4 DMI Pi5 "));
  }

  if (IehLclDevType == PcieGen5IalPi5) {
    RAS_DEBUG((LEVEL_BASIC_FLOW,"PCIE GEN5 IAL Pi5 (CXL) "));
  }

  if (IehLclDevType == SpdI3cBus) {
    RAS_DEBUG((LEVEL_BASIC_FLOW,"SPD I3C Bus "));
  }

  if (IehLclDevType == CxpSmBus) {
    RAS_DEBUG((LEVEL_BASIC_FLOW,"CXP SMBUS"));
  }

  if (CrLf) {
    RAS_DEBUG((LEVEL_BASIC_FLOW,"\n"));
  }
}

/**
  Display IEH Device sub Type to serial port.

  @param   Node  --  device Node
  @param   CrLf  -- output CRLF or not

  @retval  None

**/
VOID
EFIAPI
DisplayIehDevSubType (
  IN  NODE_DEVICE   * Node,
  IN  BOOLEAN       CrLf
  )
{
  if (Node->Common.Type == IehLocalDevice) {
    DisplayLocalDeviceType (Node->IehLocalDevice.IehLclDevType, FALSE);
  }

  if (Node->Common.Type == RCiEP) {

  }

  if (Node->Common.Type == IehEndPoint) {
    DisplayIehEndPointDevType (Node->IehEndPoint.IehEndPointType, FALSE);
  }
  if (CrLf) {
    RAS_DEBUG((LEVEL_BASIC_FLOW,"\n"));
  }
}

/**
  Display Error Severity.

  @param   SeverityStatus  -- SeverityStatus
  @param   CrLf        -- output CRLF or not

  @retval  None

**/
VOID
EFIAPI
DisplaySeverityStatus (
  IN  UINT32            SeverityStatus,
  IN  BOOLEAN           CrLf
  )
{
  if (SeverityStatus == IEH_FATAL_ERROR) {
    RAS_DEBUG((LEVEL_BASIC_FLOW,"IEH FATAL ERROR "));
  } else if (SeverityStatus == IEH_NON_FATAL_ERROR) {
    RAS_DEBUG((LEVEL_BASIC_FLOW,"IEH NON FATAL ERROR "));
  } else if (SeverityStatus == IEH_CORRECT_ERROR) {
    RAS_DEBUG((LEVEL_BASIC_FLOW,"IEH CORRECT ERROR "));
  } else {
    RAS_DEBUG((LEVEL_BASIC_FLOW,"UNKNOWN SEVERITY "));
  }

  if (CrLf) {
    RAS_DEBUG((LEVEL_BASIC_FLOW,"\n"));
  }
}

/**

  Check Ieh End Point Type

  @param [in]  Socket  --  Socket index
  @param [in]  Bus     --  Bus address
  @param [in]  Device  --  Device address
  @param [in]  Func    --  Function address

  @retval device type
**/
IEH_END_POINT_TYPE
EFIAPI
GetIehEndPointType (
  IN  UINT8  Socket,
  IN  UINT8  Bus,
  IN  UINT8  Device,
  IN  UINT8  Func
  )
{
  UINT32                           DevID;
  IEH_END_POINT_TYPE               IehEndPointType;
  DevID = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Func, 0, NULL));

  if (GetIehEndPointTypeSilicon (DevID, &IehEndPointType)) {
    return IehEndPointType;
  }

  if (PcieIsDmiPort (Socket, Bus, Device, Func, NULL)) {
    return DMIDevice;
  }

  if (PcieIsRlink (Socket, Bus, Device, Func, NULL)) {
    return RlinkDevice;
  }

  if (IsIioRootPort (Socket, Bus, Device, Func, NULL)) {
    return IioRootPort;
  }

  if (IsPcieRootPort (Socket, Bus, Device, Func, NULL)) {
    return PcieRootPort;
  }

  return PcieEndPoint;
}

/**

  Get particualr IEH counter from array list.

  @param [IN]   TempIeh   --  IEH array point
  @param [IN]   Socket    --  socket index to search
  @param [IN ]  Type     --   IEH Type
  @param [OUT]  DeviceCount -- IEH counter of this Type.

  @retval TRUE  -- particular IEH exist.
          FALSE -- particualr IEH not exist.

**/
BOOLEAN
EFIAPI
GetIehDeviceCount (
  IN     NODE_DEVICE      *TempIeh,
  IN     UINT8            Socket,
  IN     IEH_DEVICE_TYPE  Type,
  OUT    UINT8            *DeviceCount
  )
{
  UINT8     Index;
  UINT8     Counter = 0;

  for (Index = 0; Index < MAX_IEH_DEVICE; Index++) {
    if ((TempIeh[Index].Common.Socket == Socket)  &&
      (TempIeh[Index].Common.Type == Type)) {
      Counter ++;
    }
  }

  *DeviceCount = Counter;

  if (Counter != 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**

  Initialize the attribute of NODE Device according to Lookup Table.

  @param [IN]   DestIEH   --  The IEH that Node Device connect to
  @param [IN]   ChildNode    --  Node Device to initialize
  @param [IN]   BitIndex     --  The bit index that the Node device connect to IEH
  @param [IN]   ShareIndex --  The index to the device list that share the same Bit Map

  @retval EFI_SUCCESS  -- Initialize success.

**/
EFI_STATUS
EFIAPI
InstantiateChildNode (
  IN     NODE_DEVICE   *DestIEH,
  IN     NODE_DEVICE   *ChildNode,
  IN     UINT8          BitIndex,
  IN     UINT8          SharedIndex
  )

{

  LOOKUP_TBL_ENTRY  *LookupTblEntry;
  BOOLEAN           CxlValid;
  UINT8             Mc;
  UINT8             ChOnMc;
  UINT8             SecBus;
  UINT8             SubBus;

  LookupTblEntry = GetIehLookupTableEntry (DestIEH->Common.Stack, BitIndex, SharedIndex);

  if (LookupTblEntry == NULL || !IsLookupTableEntryValid (LookupTblEntry)) {
    return EFI_NOT_FOUND;
  }
  //
  // Initialize Child Node according to Lookup table
  //
  ChildNode->Common.Type = LookupTblEntry->IehDevType;
  ChildNode->Common.Socket = DestIEH->Common.Socket;
  ChildNode->Common.Stack = LookupTblEntry->Stack;
  //
  // In case some devices(eg. HQM) bus number is assigned to Stack BusLimit - Reserved Bus
  //
  if (LookupTblEntry->BusOffset < 0) {
    if (GetStackLimitBus (ChildNode->Common.Socket, ChildNode->Common.Stack, &ChildNode->Common.Bus)) {
      ChildNode->Common.Bus += LookupTblEntry->BusOffset;
    } else {
      return EFI_NOT_FOUND;
    }
  } else {
    ChildNode->Common.Bus = DestIEH->Common.Bus + LookupTblEntry->BusOffset;
  }
  ChildNode->Common.Dev = LookupTblEntry->Device;
  ChildNode->Common.Func = LookupTblEntry->Function;
  ChildNode->Common.SbPortId = LookupTblEntry->SbPortId;
  ChildNode->Common.DevSpecInfo.Data = LookupTblEntry->DevSpecInfo;

  if (LookupTblEntry->IehDevType == SatelliteIehNorth || LookupTblEntry->IehDevType == SatelliteIehSouth){
    ChildNode->SatelliteIEH.BitIndex = BitIndex;
  } else if (LookupTblEntry->IehDevType == IehEndPoint){
    ChildNode->IehEndPoint.BitIndex = BitIndex;
    ChildNode->IehEndPoint.IehEndPointType = GetIehEndPointType (ChildNode->Common.Socket,
                                        ChildNode->Common.Bus, ChildNode->Common.Dev, ChildNode->Common.Func);
    //
    // Due to PCIE_ERROR_MSG_MC_2LM_REG can't be written in SMM, need to use CsrRegTableLib to write this register,
    // and it is too late if do it at smmreadytolock, hence we do it here.
    //
    if (ChildNode->IehEndPoint.IehEndPointType == McDDRDevice) {
      Mc = (UINT8) ChildNode->Common.DevSpecInfo.DsiMcddr.McId;
      ChOnMc = (UINT8) ChildNode->Common.DevSpecInfo.DsiMcddr.ChOnMc;
      SetMcLBitmap (ChildNode->Common.Socket, Mc, ChOnMc, ChildNode->IehEndPoint.BitIndex,
        ChildNode->Common.Bus, ChildNode->Common.Dev, ChildNode->Common.Func);
    }
  } else if (LookupTblEntry->IehDevType == IehLocalDevice) {
    ChildNode->IehLocalDevice.ErrorSourceId = (UINT8) ChildNode->Common.DevSpecInfo.DsiIehLclDev.ErrorSourceId;
    ChildNode->IehLocalDevice.IehLclDevType = (UINT8) ChildNode->Common.DevSpecInfo.DsiIehLclDev.LclDevType;
    if (ChildNode->IehLocalDevice.IehLclDevType == PcieGen5IalPi5) {
      GetStackCxlInfo (
        ChildNode->Common.Socket,
        ChildNode->Common.Stack,
        NULL,
        &CxlValid
      );
      RAS_DEBUG((LEVEL_BASIC_FLOW, "CXL Stack found, CxlValid 0x%lx", CxlValid));
      ChildNode->IehLocalDevice.LclSpecInfo.CxlDevInfo.CxlValid = CxlValid;
      if (CxlValid) {
        //
        // Init CXL RAS registers on host side
        // this WA is for SPR A0 only, should be fixed in SPR B0 Si
        //
        if (IsSiliconWorkaroundEnabled ("S2209596922")) {
          SetPiGen5IalBitmap (ChildNode->Common.Socket, ChildNode->Common.Stack, ChildNode->Common.SbPortId);
        }
        //
        // Get CXL.DP Secondary and Subordinate Bus number,
        // update RCEC Associated Bus Number for CXL RCiEP devices
        //
        CxlRasGetCxlDevBusRange (ChildNode->Common.Socket, ChildNode->Common.Stack, &SecBus, &SubBus);
        if (SecBus < DestIEH->SatelliteIEH.RcecAbnNbn) {
          DestIEH->SatelliteIEH.RcecAbnNbn = SecBus;
        }
        if (SubBus > DestIEH->SatelliteIEH.RcecAbnLbn) {
          DestIEH->SatelliteIEH.RcecAbnLbn = SubBus;
        }
      }
    }
  }
  return EFI_SUCCESS;
}
/**
  This routine is to search IEH device in the system, and create IEH tree.
  No register writing allowed at create tree phase.
  If any register need to set, please do it at enable phase.

  @param [IN]  GlobalIEH  -- global IEH pointer
  @retval  none

**/
EFI_STATUS
EFIAPI
InitializeIEH (
  )
{
   NODE_DEVICE    *TempIEH;

   RAS_DEBUG ((LEVEL_BASIC_FLOW,"[IEH]   Start IEH initialization! \n"));
   TempIEH = AllocateZeroPool (sizeof(NODE_DEVICE) * MAX_IEH_DEVICE);
   if (TempIEH == NULL) {
     RAS_DEBUG ((LEVEL_BASIC_FLOW,"[IEH] ERROR: AllocateZeroPool returned NULL pointer.\n"));
     RAS_ASSERT (TempIEH != NULL);
     return EFI_OUT_OF_RESOURCES;
   }
   if (SearchIehDevice (TempIEH)) {
     BuildIehTree (TempIEH);
     PrintTree ();
   }

   return EFI_SUCCESS;
}

/**

  The constructor function initialize IEH library.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
InitIEHLibrary (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  BOOLEAN        NewAllocation;

  mGlobalIEH = GetRasGlobalData (L"IEHDeviceStore", sizeof(NODE_DEVICE) * MAX_SOCKET, &NewAllocation);
  ASSERT (mGlobalIEH != NULL);

  mMailBox2 = GetRasMailBox ();

  if (NewAllocation) {
    return InitializeIEH ();
  }

  return EFI_SUCCESS;
}