/** @file
  Implementation of IEH tree creation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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

extern NODE_DEVICE   *mGlobalIEH;
extern RAS_MAIL_BOX  *mMailBox2;
extern BOOLEAN       mIsSouthIehInLookupTable;

/**
  print IEH device.
**/
VOID
EFIAPI
PrintIehNode (
  NODE_DEVICE    *NodeList,
  UINT8          DevCount,
  BOOLEAN        SatIehFlag
  )
{
  UINT8                     Index;
  NODE_DEVICE               *NodeList2;
  NODE_DEVICE               *SouthIeh;
  UINT8                     BitIdx;
  IEH_DEVICE_TYPE           Type;
  IEH_END_POINT_TYPE        IehEdPtType;

  if (NodeList == NULL || DevCount == 0) {
    return;
  }

  for (Index = 0; Index < DevCount; Index ++) {

    //
    // Skip invalid Node
    //
    if (NodeList[Index].Common.Valid == 0) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, " Skip Invalid Node\n"));
      continue;
    }

    if (SatIehFlag) {
      RAS_DEBUG((LEVEL_BASIC_FLOW,"  "));
    }

    Type = NodeList[Index].Common.Type;
    RAS_DEBUG((LEVEL_BASIC_FLOW, "    ShareIdx:0x%x socket:0x%x  Bus:0x%x  Dev:0x%x  Func:0x%x  --  ", Index, NodeList[Index].Common.Socket,
                            NodeList[Index].Common.Bus, NodeList[Index].Common.Dev, NodeList[Index].Common.Func));
    DisplayIehDevType (Type, FALSE);
    DisplayIehDevSubType (&NodeList[Index], TRUE);

    if (Type == SatelliteIehNorth || Type == SatelliteIehSouth) {
      for (BitIdx = 0; BitIdx <= NodeList[Index].SatelliteIEH.MaxBitIdx; BitIdx++) {
        NodeList2 = (NODE_DEVICE *)NodeList[Index].SatelliteIEH.ConnectedDeviceList[BitIdx];
        RAS_DEBUG((LEVEL_BASIC_FLOW, "      BitIndex :0x%x  DevCount on this bit :0x%x, \n", BitIdx, NodeList2[0].Common.DevCount));
        PrintIehNode (NodeList2, NodeList2[0].Common.DevCount, TRUE);
      }
    } else if (Type == IehEndPoint ) {
      IehEdPtType = NodeList[Index].IehEndPoint.IehEndPointType;
      if (IehEdPtType == RlinkDevice) {
        SouthIeh = (NODE_DEVICE *)NodeList[Index].Common.DevSpecInfo.DsiDmi.pSouthIeh;
        if(SouthIeh != NULL) {
          RAS_DEBUG((LEVEL_BASIC_FLOW,"       PCH Sat IEH Start \n"));
          for (BitIdx = 0; BitIdx <= SouthIeh->SatelliteIEH.MaxBitIdx; BitIdx++) {
            NodeList2 = (NODE_DEVICE *)SouthIeh->SatelliteIEH.ConnectedDeviceList[BitIdx];
            RAS_DEBUG((LEVEL_BASIC_FLOW, "  BitIndex :0x%x  DevCount on this bit :0x%x, \n", BitIdx, NodeList2[0].Common.DevCount));
            PrintIehNode (NodeList2, NodeList2[0].Common.DevCount, TRUE);
          }
          RAS_DEBUG((LEVEL_BASIC_FLOW,"       PCH Sat IEH Done \n"));
        }
      }
    }
  }
}

/**
  Print created tree.
**/
VOID
EFIAPI
PrintTree (
  VOID
  )
{
  UINT8          Socket;
  NODE_DEVICE    *NodeList;
  UINT8          BitIdx;

  RAS_DEBUG((LEVEL_BASIC_FLOW,"\n [IEH]--------------    Print created IEH tree    ----------- \n"));


  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (IsSocketPresent (Socket) == FALSE || mGlobalIEH[Socket].Common.Valid == 0) {
      continue;
    }

    RAS_DEBUG((LEVEL_BASIC_FLOW, "socket:0x%x  Bus:0x%x  Dev:0x%x  Func:0x%x  --  Max Bit Index:0x%x ",
      mGlobalIEH[Socket].Common.Socket, mGlobalIEH[Socket].Common.Bus, mGlobalIEH[Socket].Common.Dev,
                                    mGlobalIEH[Socket].Common.Func, mGlobalIEH[Socket].GlobalIEH.MaxBitIdx));
    DisplayIehDevType (mGlobalIEH[Socket].Common.Type, TRUE);

    for (BitIdx = 0; BitIdx <= mGlobalIEH[Socket].GlobalIEH.MaxBitIdx; BitIdx++) {
      NodeList = (NODE_DEVICE *)mGlobalIEH[Socket].GlobalIEH.ConnectedDeviceList[BitIdx];
      RAS_DEBUG((LEVEL_BASIC_FLOW, "  BitIndex :0x%x  DevCount on this bit :0x%x, \n", BitIdx, NodeList[0].Common.DevCount));
      PrintIehNode (NodeList, NodeList[0].Common.DevCount, FALSE);
    }


  }
}

/**
  Search IEH array to find specific type IEH.

  @param [IN]   TempIeh   --  IEH array point
  @param [IN]   Socket    --  Socket index to search
  @param [IN]   Stack     --  Stack index to search, 0xff means don't care stack
  @param [IN]   Type      --  IEH type
  @param [OUT]  Index     --  index of IEH array point, 0xff means search all specific type IEH

  @retval TRUE  -- found the device, otherwise not found.

**/
BOOLEAN
EFIAPI
FindIehDevice (
  IN     NODE_DEVICE      *TempIeh,
  IN     UINT8            Socket,
  IN     UINT8            Stack,
  IN     IEH_DEVICE_TYPE  Type,
  OUT    UINT8            *IndexIEH
  )
{
  UINT8     Index;

  for (Index = 0; Index < MAX_IEH_DEVICE; Index++) {
    if ((TempIeh[Index].Common.Socket == Socket) &&
      (TempIeh[Index].Common.Type == Type)) {
        if ((Stack != 0xff) && (TempIeh[Index].Common.Stack != Stack)) {
          continue;
        }
       RAS_DEBUG((LEVEL_FUNC_FLOW,"[IEH]  IEH device found! Socket:0x%x  index:0x%x ",Socket,  Index));
       DisplayIehDevType (Type, TRUE);

       *IndexIEH = Index;
       return TRUE;
     }
  }
  return FALSE;

}

/**
  This routine purpose is to create IEH tree for global IEH and local IEH. It is recursive design.

  @param [IN] DestIEH -- destination pointer to IEH tree node.
  @param [IN] SourceIEH -- source pointer to IEH temporary arrary.
  @param [IN] SourceIndex -- index for source array
  @param [IN] AssignBitmap -- bitmap assigned for north IEH.

  @retval none

**/
VOID
EFIAPI
BuildIeh (
  IN  NODE_DEVICE   *DestIEH,
  IN  NODE_DEVICE   *SourceIEH,
  IN  UINT8          SourceIndex,
  IN  UINT8          AssignBitmap
  )
{
  EFI_STATUS                Status;
  UINT8                     i;
  UINT8                     NodeCount;
  BOOLEAN                   RcecSupport = FALSE;
  UINT32                    *pABMRCiEP = NULL;
  UINT8                     *pRcecAbnNbn = NULL;
  UINT8                     *pRcecAbnLbn = NULL;
  UINT8                     Index = 0;
  UINT8                     BitIdx;
  UINT8                     MaxBitIdx;
  NODE_DEVICE               *ChildNode;
  NODE_DEVICE               **NodeListHead;
  NODE_DEVICE               *NodeList;
  IEH_DEVICE_TYPE           Type;
  IEH_END_POINT_TYPE        IehEdPtType;

  if (DestIEH == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW,"[IEH] ERROR: DestIEH is NULL.\n"));
    RAS_ASSERT (DestIEH != NULL);
    return;
  }

  if (SourceIEH == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW,"[IEH] ERROR: SourceIEH is NULL.\n"));
    RAS_ASSERT (SourceIEH != NULL);
    return;
  }

  DestIEH->Common.Valid    = 0;
  DestIEH->Common.Type     = SourceIEH[SourceIndex].Common.Type;
  DestIEH->Common.Socket   = SourceIEH[SourceIndex].Common.Socket;
  DestIEH->Common.Bus      = SourceIEH[SourceIndex].Common.Bus;
  DestIEH->Common.Dev      = SourceIEH[SourceIndex].Common.Dev;
  DestIEH->Common.Func     = SourceIEH[SourceIndex].Common.Func;
  DestIEH->Common.Stack    = SourceIEH[SourceIndex].Common.Stack;

  //
  // Get Max Bit Map of direct connected devices
  //
  MaxBitIdx = GetMaxBitMap (DestIEH);
  if (DestIEH->Common.Type == GlobalIeh) {
    DestIEH->GlobalIEH.MaxBitIdx = MaxBitIdx;
  } else {
    DestIEH->SatelliteIEH.MaxBitIdx = MaxBitIdx;
  }

  RAS_DEBUG((LEVEL_FUNC_FLOW,"[IEH] Skt:0x%x Bus:0x%x Dev:0x%x Fun:0x%x - MaxBitIdx = 0x%x\n",
            DestIEH->Common.Socket, DestIEH->Common.Bus,
            DestIEH->Common.Dev, DestIEH->Common.Func, MaxBitIdx));

  if (MaxBitIdx == 0) {
    RAS_DEBUG((LEVEL_BASIC_FLOW,"[IEH] There is no direct connected device, exit\n"));
    return;
  }

  //
  // Allocate buffer to store NodeList Pointer
  //
  NodeListHead =  AllocateZeroPool (sizeof(UINT64) * (MaxBitIdx + 1));
  if (NodeListHead == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW,"[IEH] ERROR: AllocateZeroPool returned NULL pointer.\n"));
    RAS_ASSERT (NodeListHead != NULL);
    return;
  }

  if (DestIEH->Common.Type == GlobalIeh) {
    DestIEH->GlobalIEH.ConnectedDeviceList = (IEH_DEVICE_COMMON **) NodeListHead;
    //
    // Global IEH have RCEC capabilitie registers but RCEC is strap disabled
    //
    DestIEH->GlobalIEH.RcecSupport = FALSE;
    DestIEH->GlobalIEH.LocalErrorSource = GetLclErrSrc (DestIEH);
    DestIEH->GlobalIEH.IehVer = SourceIEH[SourceIndex].GlobalIEH.IehVer;
  } else {
    DestIEH->SatelliteIEH.ConnectedDeviceList = (IEH_DEVICE_COMMON **) NodeListHead;
    DestIEH->SatelliteIEH.BitIndex = AssignBitmap;
    //
    // Check if RCEC support
    //
    RcecSupport = IsPcieRcecSupported (DestIEH->Common.Socket, DestIEH->Common.Bus, DestIEH->Common.Dev, DestIEH->Common.Func, NULL);
    DestIEH->SatelliteIEH.RcecSupport = RcecSupport;
    pABMRCiEP = &DestIEH->SatelliteIEH.ABMRCiEP;
    pRcecAbnNbn = &DestIEH->SatelliteIEH.RcecAbnNbn;
    pRcecAbnLbn = &DestIEH->SatelliteIEH.RcecAbnLbn;
    DestIEH->SatelliteIEH.LocalErrorSource = GetLclErrSrc (DestIEH);
    DestIEH->SatelliteIEH.IehVer = SourceIEH[SourceIndex].SatelliteIEH.IehVer;
  }

  DestIEH->Common.Valid = 1;
  //
  // DEBUG Message for the Destination IEH
  //
  RAS_DEBUG ((LEVEL_FUNC_FLOW,"[IEH] Skt:0x%x Bus:0x%x Dev:0x%x Func:0x%x ",DestIEH->Common.Socket,
                            DestIEH->Common.Bus, DestIEH->Common.Dev, DestIEH->Common.Func));
  DisplayIehDevType (DestIEH->Common.Type, FALSE);
  if (DestIEH->Common.Type == SatelliteIehNorth || DestIEH->Common.Type == SatelliteIehSouth) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW,"IehVer:0x%x ", DestIEH->SatelliteIEH.IehVer));
    RAS_DEBUG ((LEVEL_FUNC_FLOW,"BitIndex:0x%x ", DestIEH->SatelliteIEH.BitIndex));
  }
  RAS_DEBUG ((LEVEL_FUNC_FLOW,"Device Node created!\n"));

  //
  // Enumerate devices that connected to each Bit
  //
  for (BitIdx = 0; BitIdx < MaxBitIdx + 1; BitIdx++) {
    //
    // Get Device Count that share the same BITMAP
    //
    NodeCount = GetDevCountOnBit (DestIEH, BitIdx);
    //
    // Allocate Buffer to store device list that connect to this bit
    //
    NodeList = AllocateZeroPool (sizeof(NODE_DEVICE) * NodeCount);
    if (NodeList == NULL) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW,"[IEH] ERROR: AllocateZeroPool returned NULL pointer.\n"));
      RAS_ASSERT (NodeList != NULL);
      return;
    }

    NodeListHead[BitIdx] = NodeList;

    //
    //  Initialize some variable for RCEC capabilities fields
    //
    if (BitIdx == 0) {
      if (DestIEH->Common.Type != GlobalIeh) {
        *pABMRCiEP = 0;
        *pRcecAbnNbn = 0xff;
        *pRcecAbnLbn = 0;
      }
    }
    //
    // Initialize each Node of NodeList
    //
    for (i = 0; i < NodeCount; i++) {
      ChildNode = (NODE_DEVICE*) &NodeList[i];
      ChildNode->Common.Valid = 0;
      Status = InstantiateChildNode (DestIEH, ChildNode, BitIdx, i);
      if (EFI_ERROR(Status)){
        RAS_DEBUG((LEVEL_FUNC_FLOW,"[IEH]  Init IEH direct device - BitIdx:0x%x Share Index:0x%x fail!\n ", BitIdx, i));
        continue;
      }
      //
      //  Initialize RCEC Related attributes, ABMRCiEP and AbnLbn
      //
      if (BitIdx ==0 && DestIEH->Common.Type != GlobalIeh) {
        if (ChildNode->Common.Type == RCiEP) {
          if (ChildNode->Common.Bus == DestIEH->Common.Bus){
            //
            // ABM only refect the device bitmap on the same BUS of IEH
            //
            *pABMRCiEP |= (1 << ChildNode->Common.Dev);
          } else {
            if (ChildNode->Common.Bus < *pRcecAbnNbn){
              *pRcecAbnNbn = ChildNode->Common.Bus;
            }
            if (ChildNode->Common.Bus > *pRcecAbnLbn){
              *pRcecAbnLbn = ChildNode->Common.Bus;
            }
          }
        }
      }
      //
      //The DevCount of Local Device, RCiEP and IEH End Point Node is the device count that share the same bit.
      //
      ChildNode->Common.DevCount = NodeCount;
      //
      // if the ChildNode is satellite IEH, construct it
      //
      Type = ChildNode->Common.Type;
      if (Type == SatelliteIehNorth || Type == SatelliteIehSouth) {
        RAS_DEBUG ((LEVEL_FUNC_FLOW,"[IEH]  creating Satellite IEH on bitIndex:0x%x\n", BitIdx));
        if (FindIehDevice(SourceIEH, ChildNode->Common.Socket,
          (UINT8)ChildNode->Common.DevSpecInfo.DsiSatIeh.Stack, ChildNode->Common.Type, &Index)) {
          BuildIeh (ChildNode, SourceIEH, Index, BitIdx);
        }
        continue;
      }
      ChildNode->Common.Valid = 1;
      RAS_DEBUG((LEVEL_FUNC_FLOW,"[IEH]  created IEH direct device - BitIndex:0x%x ShareIndex:0x%x, Skt:0x%x Bus:0x%x Dev:0x%x F:0x%x ",
        BitIdx, i, ChildNode->Common.Socket, ChildNode->Common.Bus, ChildNode->Common.Dev, ChildNode->Common.Func));
      DisplayIehDevType (Type, TRUE);

      //
      // If it is DMI Device, and south IEH does not directly connect to Global IEH, create South IEH Node
      //
      if (Type == IehEndPoint && !mIsSouthIehInLookupTable) {
        IehEdPtType = ChildNode->IehEndPoint.IehEndPointType;
        if ((IehEdPtType == RlinkDevice) &&
              (FindIehDevice(SourceIEH, DestIEH->Common.Socket, 0xff, SatelliteIehSouth, &Index))) {
          if (SourceIEH[Index].Common.Bus == 0) {
            //
            // Allocate a buffer to store South IEH Node, and record the pointer to DSI.
            //
            ChildNode->Common.DevSpecInfo.DsiDmi.pSouthIeh = AllocateZeroPool(sizeof(NODE_DEVICE));
            if (ChildNode->Common.DevSpecInfo.DsiDmi.pSouthIeh == NULL) {
              RAS_DEBUG ((LEVEL_BASIC_FLOW,"[IEH] ERROR: AllocateZeroPool returned NULL pointer.\n"));
              RAS_ASSERT (ChildNode->Common.DevSpecInfo.DsiDmi.pSouthIeh != NULL);
              return;
            }
            //
            // Build South IEH Tree
            //
            BuildIeh (ChildNode->Common.DevSpecInfo.DsiDmi.pSouthIeh, SourceIEH, Index, 0);
            RAS_DEBUG((LEVEL_FUNC_FLOW,"[IEH]  PCH Sat IEH directly device creat Done.\n"));
          }
        }
      }
    }
  }

  //
  // Satllite IEH RCEC ABM, ABN registers are not writable in SMM, hence have to use
  // CsrRegTableLib to write them later in DXE driver, can't do it in SmmReadyTolock because
  // it is too late to use CsrRegTableLib
  //
  if ((DestIEH->Common.Valid == 1) && (RcecSupport)) {
    SetRcecAbmNbnLbn (DestIEH);
  }

  //
  // Clear the Agent source IDs(6-9) of satellite IEH for wave 3
  // This WA is for SPR A0 only; should be fixed in SPR B0
  //
  if ((DestIEH->Common.Valid == 1) && (DestIEH->Common.Type == SatelliteIehNorth) && IsSiliconWorkaroundEnabled ("S2209596922")) {
    ClearAgentSrcId (DestIEH);
  }

}

/**
  This routine purpose is to creat whole system IEH tree.

  @param [IN] TempIEH  -- IEH device pointer.
  @retval none.
**/
VOID
EFIAPI
BuildIehTree (
  IN  NODE_DEVICE  * TempIEH
  )
{
  UINT8   Socket;
  UINT8   IndexOfRet;

  mIsSouthIehInLookupTable = IsSouthIehInLookupTable ();
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    IndexOfRet = 0;
    if (FindIehDevice (TempIEH, Socket, 0xff, GlobalIeh, &IndexOfRet)) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW,"[IEH]  build Socket:0x%x IEH tree.start...\n", Socket));
      BuildIeh (&mGlobalIEH[Socket], TempIEH, IndexOfRet, 0);
      RAS_DEBUG ((LEVEL_FUNC_FLOW,"[IEH]  build Socket:0x%x IEH tree.end...\n\n", Socket));
    }
  }
}