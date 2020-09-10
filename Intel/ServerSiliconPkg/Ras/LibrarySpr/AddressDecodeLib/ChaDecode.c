/** @file
  Implementation of CHA DRAM decode.

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
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include "AddressDecodeInternal.h"

extern AD_SOCKET_INFO              *mSadTadScope;

/**
  Get socket index per input system address.

  This is to check remote rules of socket 0. Caller code should ensure this address belong to dram address.

  @param[in]       SystemAddress    System address to be translated


  @retval          socket index.

**/
UINT8
EFIAPI
GetSocketFromRemoteRule (
  IN     UINT64    Sa
  )
{
  UINT8      Index;
  UINT8      ReturnSocket = 0;

  for (Index = 0; Index < MAX_REMOTE_DRAM_RULE; Index ++) {
    if (mSadTadScope[0].RemoteDramRule[Index].RemoteDramRuleValid) {
      if ((Sa >= mSadTadScope[0].RemoteDramRule[Index].StartAddress)  &&
        (Sa <= mSadTadScope[0].RemoteDramRule[Index].EndAddress)) {
          ReturnSocket = mSadTadScope[0].RemoteDramRule[Index].Socket;
          break;
        }
    }
  }

  //
  //if it is not found in remote dram rule, it should locate at socket 0.
  //
  if (Index == MAX_REMOTE_DRAM_RULE) {
    ReturnSocket = 0;
  }

  return ReturnSocket;
}


/**
  Get cluster index per system address & socket.

  @param[in]       SystemAddress    System address to be translated
  @param[in]       Socket    --     Socket Index
  @param[in]       ClusterXorDefeature -- Cluster xor feature mode.

  @retval          socket index.

**/
UINT8
EFIAPI
GetClusterIndex (
  IN      UINT64      Sa,
  IN      UINT8       Socket,
  IN      BOOLEAN     ClusterXorDefeature
  )
{
  UINT8              ClusterIndex;

  if (IsUbcModeEnabled (Socket)) {

    if (ClusterXorDefeature == 0) {
      //
      //cluster xor is always enabled under ubc mode.
      //
      //UBC hemi xor: bit8 ^ bit11^bit17^bit25.
      //UBC quad xor: bit8,9 ^bit11,12^bit17,18^bit25:26
      //
      ClusterIndex = (UINT8) (BitFieldRead64 (Sa, 8, 9) ^ BitFieldRead64 (Sa, 11, 12) ^ BitFieldRead64 (Sa, 17, 18) ^ BitFieldRead64 (Sa, 25, 26));
    } else {
      ClusterIndex = (UINT8) (BitFieldRead64 (Sa, 11, 12));
    }


    if (GetClusterNum (Socket) == 2) {
      ClusterIndex &= 1;
    }

  } else {
    ClusterIndex = 0;
  }

  return ClusterIndex;
}




/**
  Get cluster index per system mode.

  @param[in]       Sa    System address to be translated
  @param[in]       Socket    --     Socket Index


  @retval          cluster index. 0xFF means all clusters on this socket.

**/
UINT8
EFIAPI
GetClusterPerSystemMode (
  IN     UINT64       Sa,
  IN     UINT8        Socket
  )
{
  UINT8        ClusterIndex;
  if (IsUbcModeEnabled(Socket)) {
    ClusterIndex = GetClusterIndex (Sa, Socket, GetXorDefeatureMode());
  } else {
    //
    //in case SNC mode, scan all cluster on the socket.
    //
    if (IsSncModeEnabled (Socket)) {
      ClusterIndex = 0xff;
    } else {
      ClusterIndex = 0;
    }
  }

  return ClusterIndex;
}

/**
  Calculate interleave list index per system address and local DRAM rule info.

InterleaveListIndex = (SystemAddress >> 6 ) & 7     --- if interleave_mode = 0  64B
InterleaveListIndex = (SystemAddress >> 8 ) & 7     --- if interleave_mode = 1  256B
InterleaveListIndex = (SystemAddress >> 12 ) & 7   --- if interleave_mode = 2  4KB
InterleaveListIndex = (SystemAddress >> 13 ) & 7   --- if interleave_mode = 3  8KB


  @param[in]       Socket        -- Socket index
  @param[in]       Cluster       -- Cluster index, if value 0xff, scan all cluster on the socket.
  @param[in, out]  LocalDramRuleIndex --  local dram rule index
  @param[in]       Sa            -- System address to be translated

  @retval          Interleave list index

**/
UINT8
EFIAPI
CalcInterleaveListIndex (
  IN      UINT8     Socket,
  IN      UINT8     ClusterIndex,
  IN      UINT8     DramRuleIndex,
  IN      UINT64    Sa
  )
{
  UINT8      InterleaveMode;

  InterleaveMode =  mSadTadScope[Socket].ClusterInfo[ClusterIndex].DramRule[DramRuleIndex].InterleaveMode;

  return (UINT8)(RShiftU64(Sa, InterleaveMode) & 7);
}


/**
  Get local DRAM rule target id per input address & socket-cluster.

  @param[in]       Sa            -- System address to be translated
  @param[in]       Socket        -- Socket index
  @param[in]       Cluster       -- Cluster index, if value 0xff, scan all cluster on the socket.
  @param[in, out]  TargetId      --  target ID. bit3 = 1 means local dram rule.
  @param[in, out]  LocalDramRuleIndex --  local dram rule index

  @retval          TRUE -- get targetId successfully; FALSE -- get targetId failed.

**/
BOOLEAN
EFIAPI
GetTargetId (
  IN     UINT64    Sa,
  IN     UINT8     Socket,
  IN OUT UINT8    *Cluster,
  IN OUT UINT8    *TargetId,
  IN OUT UINT8    *LocalDramRuleIndex
  )
{
  UINT8           StartCluster;
  UINT8           EndCluster;
  UINT8           ClusterIndex;
  UINT8           DramRuleIndex;
  UINT8           InterleaveListIndex;

  if(Cluster == NULL){
    return FALSE;
  }

  if (*Cluster == 0xff) {
    StartCluster = 0;
    EndCluster = (UINT8)(GetClusterNum (Socket) - 1);
  } else {
    StartCluster = *Cluster;
    EndCluster = *Cluster;
  }

  DEBUG ((EFI_D_INFO, "start cluster:%d  end cluster:%d \n", StartCluster, EndCluster));

  for (ClusterIndex = StartCluster; (ClusterIndex <= EndCluster) && (ClusterIndex < MAX_CLUSTER); ClusterIndex ++) {

    for (DramRuleIndex = 0; DramRuleIndex < MAX_LOCAL_DRAM_RULE; DramRuleIndex ++) {

      if (mSadTadScope[Socket].ClusterInfo[ClusterIndex].DramRule[DramRuleIndex].LocalDramRuleValid == FALSE) {
        continue;
      }

      DEBUG ((EFI_D_INFO, "GetTargetId  Socket:%d cluster:%d DramRule:%d Base:%lx End:%lx \n", Socket, ClusterIndex, DramRuleIndex,
        mSadTadScope[Socket].ClusterInfo[ClusterIndex].DramRule[DramRuleIndex].StartAddress, mSadTadScope[Socket].ClusterInfo[ClusterIndex].DramRule[DramRuleIndex].EndAddress));

      if ((Sa >= mSadTadScope[Socket].ClusterInfo[ClusterIndex].DramRule[DramRuleIndex].StartAddress) &&
          (Sa <= mSadTadScope[Socket].ClusterInfo[ClusterIndex].DramRule[DramRuleIndex].EndAddress)) {
        InterleaveListIndex = CalcInterleaveListIndex (Socket, ClusterIndex, DramRuleIndex, Sa);
        if (TargetId != NULL) {
          *TargetId = mSadTadScope[Socket].ClusterInfo[ClusterIndex].DramRule[DramRuleIndex].TargetTable[InterleaveListIndex];
        }

        if (LocalDramRuleIndex != NULL) {
          *LocalDramRuleIndex = DramRuleIndex;
        }

        if (Cluster != NULL) {
          *Cluster = ClusterIndex;
        }

        return TRUE;

      }
    }
  }
  return FALSE;
}



/**
  check whether this target ID belong to remote socket.

  @param[in]       TargetId  -- target id
  @param[in, out]  Socket           Point to socket.

  @retval          TRUE  ---  this target ID indicates remote socket
                   FALSE ---  this target ID indicates local socket target ID.

**/
BOOLEAN
EFIAPI
IsRemoteSocketTargetId (
  IN       UINT8      TargetId,
  IN OUT   UINT8     *Socket
  )
{
  BOOLEAN      Flag;

  if ((TargetId & 0x08) == 0) {
    Flag = TRUE;
    if (Socket != NULL) {
      *Socket = TargetId & 7;
    }
  } else {
      Flag = FALSE;
  }

return Flag;
}



/**
  Get socket id per system address.

  It assumes this system address belong to Dram rule.

  @param[in]       Sa  --  system address

  @retval          socket id

**/
UINT8
EFIAPI
GetSocketId (
  IN    UINT64      Sa
  )
{
  UINT8      Socket;
  UINT8      ClusterIndex;
  UINT8      TargetId;
  UINT8      Package;

  //
  // check remote dram rule.
  //
  Socket = GetSocketFromRemoteRule (Sa);

  //
  //if socket is 0, need to get scan local dram rules.
  //
  if (Socket == 0) {

    ClusterIndex = GetClusterPerSystemMode (Sa, Socket);

    if (GetTargetId(Sa, Socket, &ClusterIndex, &TargetId, NULL)) {
      if (IsRemoteSocketTargetId (TargetId, &Package)) {
        Socket = Package;
      }
    } else {
      DEBUG ((EFI_D_ERROR, "[GetSocketId] error on socket:%d cluster:%d\n", Socket, ClusterIndex));
    }

  }
  return Socket;
}


/**
  Get interleave ways.

  @param[in]       Socket        -- Socket index
  @param[in]       ClusterIndex       -- Cluster index.
  @param[in]       DramRuleIndex --  local dram rule index
  @param[in]       TargetId      -- local target ID
  @param[in]       RouteTableType -- route table type.

  @retval         interleave ways

**/
UINT8
EFIAPI
GetInterleaveWays (
  IN      UINT8     Socket,
  IN      UINT8     ClusterIndex,
  IN      UINT8     DramRuleIndex,
  IN      UINT8     TargetId,
  IN      UINT8     RouteTableType
  )
{
  UINT8       Index;

  //
  //index: 3b{die, routeType}
  //
  Index = ((TargetId & 1) << 2 ) | RouteTableType;
  return mSadTadScope[Socket].ClusterInfo[ClusterIndex].DramRule[DramRuleIndex].InterleaveWays[Index];
}

/**
  Get route table shift address.

  @param[in]       Sa            -- System address to be translated
  @param[in]       RouteTableType -- route table type.
  @param[in]       InterleaveShift  -- interleave shift mode
  @param[in]       InterleaveWays  -- interleave ways

  @retval        shift address

**/
UINT8
EFIAPI
CalcShiftAddress (
  IN      UINT64    Sa,
  IN      UINT8     RouteTypeType,
  IN      UINT8     InterleaveShift,
  IN      UINT8     InterleaveWays
  )
{
  UINT8             ShiftAddress = 0;

  //
  //RT0 and RT1
  //
  if ((RouteTypeType == 0) || (RouteTypeType == 1)) {
    switch (InterleaveShift) {
      case 0:
        if ((InterleaveWays == INT_THREE_WAY) || (InterleaveWays == INT_SIX_WAY)) {
          ShiftAddress = BitFieldRead64(Sa, 6, 63) % 3;
        } else {
          ShiftAddress = (UINT8)BitFieldRead64 (Sa, 6,8);
        }
        break;

      case 2:
        if ((InterleaveWays == INT_THREE_WAY) || (InterleaveWays == INT_SIX_WAY)) {
          ShiftAddress = BitFieldRead64(Sa, 8, 63) % 3;
        } else {
          ShiftAddress = (UINT8)BitFieldRead64 (Sa, 8,10);
        }
        break;

      case 6:
        if ((InterleaveWays == INT_THREE_WAY) || (InterleaveWays == INT_SIX_WAY)) {
          ShiftAddress = BitFieldRead64(Sa, 12, 63) % 3;
        } else {
          ShiftAddress = (UINT8)BitFieldRead64 (Sa, 12,14);
        }
        break;

      case 7:
        ShiftAddress = (UINT8)(BitFieldRead64(Sa, 8, 10) ^ BitFieldRead64(Sa, 14, 16) ^ BitFieldRead64(Sa, 22, 24));
        if ((InterleaveWays == INT_THREE_WAY) || (InterleaveWays == INT_SIX_WAY)) {
          ShiftAddress = (UINT8)((ShiftAddress | (BitFieldRead64(Sa, 11, 63) << 3)) % 3);
        }
        break;
      default:
        DEBUG ((EFI_D_ERROR, "[CalcShiftAddress] error on route table type:%d InterleaveShift:%d\n", RouteTypeType, InterleaveShift));
        //
        //reserved
        //
        break;
    }
  }

  //
  //RT2 and RT2LM
  //
  if ((RouteTypeType == 2) || (RouteTypeType == 3)) {
    switch (InterleaveShift) {
      case 2:
        if ((InterleaveWays == INT_THREE_WAY) || (InterleaveWays == INT_SIX_WAY)) {
          ShiftAddress = BitFieldRead64(Sa, 8, 63) % 3;
        } else {
          ShiftAddress = (UINT8)BitFieldRead64 (Sa, 8,9);
        }
        break;

      case 6:
        if ((InterleaveWays == INT_THREE_WAY) || (InterleaveWays == INT_SIX_WAY)) {
          ShiftAddress = BitFieldRead64(Sa, 12, 63) % 3;
        } else {
          ShiftAddress = (UINT8)BitFieldRead64 (Sa, 12,13);
        }
        break;

      default:
        DEBUG ((EFI_D_ERROR, "[CalcShiftAddress] error on route table type:%d InterleaveShift:%d\n", RouteTypeType, InterleaveShift));
        //
        //reserved
        //
        break;
    }
  }

  return ShiftAddress;
}


/**
  Get route table index

  @param[in]       Sa            -- System address to be translated
  @param[in]       RouteTableType -- route table type.
  @param[in]       RtShiftAddress  -- route table shifted address
  @param[in]       InterleaveWays  -- interleave ways
  @param[in]       TargetId      -- local target ID

  @retval        route table index

**/
UINT8
EFIAPI
CalcRouteTableIndex (
  IN      UINT64    Sa,
  IN      UINT8     RouteTypeType,
  IN      UINT8     RtShiftAddress,
  IN      UINT8     InterleaveWays,
  IN      UINT8     TargetId
  )
{
  UINT8     RouteTableIndex = 0;
  //
  //RT0
  //
  if (RouteTypeType == 0) {
    switch (InterleaveWays) {
      case INT_ONE_WAY:
        RouteTableIndex = (TargetId >> 1) & 0x03;
        break;
      case INT_TWO_WAY:
        if (GetXorDefeatureMode()) {
          RouteTableIndex = (UINT8)BitFieldRead64(Sa,8,8);
        } else {
          RouteTableIndex = (UINT8) (BitFieldRead64(Sa,8,8) ^ BitFieldRead64(Sa,14,14) ^ BitFieldRead64(Sa,22,22));
        }
        break;
      case INT_THREE_WAY:
        if (GetXorDefeatureMode()) {
          RouteTableIndex = RtShiftAddress % 3;
        } else {
          RouteTableIndex = (UINT8) (BitFieldRead64(Sa,8,9) ^ BitFieldRead64(Sa,14,15) ^ BitFieldRead64(Sa,22,23));
          RouteTableIndex = RouteTableIndex % 3;
        }
        break;

      case INT_FOUR_WAY:
        if (GetXorDefeatureMode()) {
          RouteTableIndex = (UINT8)BitFieldRead64(Sa,8,9);
        } else {
          RouteTableIndex = (UINT8) (BitFieldRead64(Sa,8,9) ^ BitFieldRead64(Sa,14,15) ^ BitFieldRead64(Sa,22,23));
        }
        break;

      case INT_SIX_WAY:
        if (GetXorDefeatureMode()) {
          RouteTableIndex = (RtShiftAddress % 3) | (RtShiftAddress & 0x04);
        } else {
          RouteTableIndex = (UINT8) (BitFieldRead64(Sa,8,10) ^ BitFieldRead64(Sa,14,16) ^ BitFieldRead64(Sa,22,24));
          RouteTableIndex = RouteTableIndex % 6;
        }
        break;

      case INT_EIGHT_WAY:
        if (GetXorDefeatureMode()) {
          RouteTableIndex = (UINT8)BitFieldRead64(Sa,8,10);
        } else {
          RouteTableIndex = (UINT8) (BitFieldRead64(Sa,8,10) ^ BitFieldRead64(Sa,14,16) ^ BitFieldRead64(Sa,22,24));
        }
        break;

      default:
        DEBUG ((EFI_D_ERROR, "[CalcRouteTableIndex] error on route table type:%d InterleaveWays:%d\n", RouteTypeType, InterleaveWays));
    }
  }


  //
  //RT1
  //
  if (RouteTypeType == 1) {
    switch (InterleaveWays) {
      case INT_ONE_WAY:
        RouteTableIndex = (TargetId >> 1) & 0x03;
        break;
      case INT_TWO_WAY:
          RouteTableIndex = RtShiftAddress & 1;
        break;
      case INT_THREE_WAY:
          RouteTableIndex = RtShiftAddress % 3;
        break;
      case INT_FOUR_WAY:
          RouteTableIndex = RtShiftAddress & 3;
        break;
      case INT_SIX_WAY:
          RouteTableIndex = (RtShiftAddress % 3) | ((RtShiftAddress & 1) << 2);
        break;

      case INT_EIGHT_WAY:
          RouteTableIndex = RtShiftAddress & 7;
        break;

      default:
        DEBUG ((EFI_D_ERROR, "[CalcRouteTableIndex] error on route table type:%d InterleaveWays:%d\n", RouteTypeType, InterleaveWays));
    }
  }

  //
  //RT2, RT2LM
  //
  if ((RouteTypeType == 2) || (RouteTypeType == 3)) {
    switch (InterleaveWays) {
      case INT_ONE_WAY:
        RouteTableIndex = (TargetId >> 1) & 0x03;
        break;
      case INT_TWO_WAY:
          RouteTableIndex = RtShiftAddress & 1;
        break;
      case INT_THREE_WAY:
          RouteTableIndex = RtShiftAddress % 3;
        break;
      case INT_FOUR_WAY:
          RouteTableIndex = RtShiftAddress & 3;
        break;
      case INT_SIX_WAY:
          RouteTableIndex = (RtShiftAddress % 3) | ((RtShiftAddress & 1) << 2);
        break;

      default:
        DEBUG ((EFI_D_ERROR, "[CalcRouteTableIndex] error on route table type:%d InterleaveWays:%d\n", RouteTypeType, InterleaveWays));
    }
  }

  return RouteTableIndex;
}


/**
  Get MC ID accroding socket-cluster-dram rule-target ID.

  @param[in]       Sa            -- System address to be translated
  @param[in]       Socket        -- Socket index
  @param[in]       ClusterIndex       -- Cluster index.
  @param[in]       DramRuleIndex --  local dram rule index
  @param[in]       TargetId      -- local target ID


  @retval         Mc id

**/
UINT8
EFIAPI
GetMcId (
  IN      UINT64    Sa,
  IN      UINT8     Socket,
  IN      UINT8     ClusterIndex,
  IN      UINT8     DramRuleIndex,
  IN      UINT8     TargetId
  )
{
   UINT8        RtShiftAddress;
   UINT8        RouteTableType;
   UINT8        McInterleaveShift;
   UINT8        InterleaveWays;
   UINT8        RouteTableIndex;
   UINT8        Index;

   RouteTableType = mSadTadScope[Socket].ClusterInfo[ClusterIndex].DramRule[DramRuleIndex].RouteTableType;
   McInterleaveShift =  mSadTadScope[Socket].ClusterInfo[ClusterIndex].DramRule[DramRuleIndex].McInterleaveShift;
   InterleaveWays = GetInterleaveWays ( Socket, ClusterIndex, DramRuleIndex, TargetId, RouteTableType);

   RtShiftAddress = CalcShiftAddress (Sa,RouteTableType, McInterleaveShift, InterleaveWays);
   RouteTableIndex = CalcRouteTableIndex(Sa, RouteTableType, RtShiftAddress, InterleaveWays, TargetId);

  //
  //index: 3b{die, routeType}
  //
  Index = ((TargetId & 1) << 2 ) | RouteTableType;

  return  mSadTadScope[Socket].ClusterInfo[ClusterIndex].McTable[Index][RouteTableIndex];
}


/**
  Get channel ID accroding socket-cluster-dram rule-target ID.

  @param[in]       Sa            -- System address to be translated
  @param[in]       Socket        -- Socket index
  @param[in]       ClusterIndex       -- Cluster index.
  @param[in]       DramRuleIndex --  local dram rule index
  @param[in]       TargetId      -- local target ID


  @retval         channel id

**/
UINT8
EFIAPI
GetChId (
  IN      UINT64    Sa,
  IN      UINT8     Socket,
  IN      UINT8     ClusterIndex,
  IN      UINT8     DramRuleIndex,
  IN      UINT8     TargetId
  )
{
   UINT8        RtShiftAddress;
   UINT8        RouteTableType;
   UINT8        McInterleaveShift;
   UINT8        InterleaveWays;
   UINT8        RouteTableIndex;
   UINT8        Index;

   RouteTableType = mSadTadScope[Socket].ClusterInfo[ClusterIndex].DramRule[DramRuleIndex].ChannelTabletype;
   McInterleaveShift =  mSadTadScope[Socket].ClusterInfo[ClusterIndex].DramRule[DramRuleIndex].ChInterleaveShift;
   InterleaveWays = GetInterleaveWays ( Socket, ClusterIndex, DramRuleIndex, TargetId, RouteTableType);

   RtShiftAddress = CalcShiftAddress (Sa,RouteTableType, McInterleaveShift, InterleaveWays);
   RouteTableIndex = CalcRouteTableIndex(Sa, RouteTableType, RtShiftAddress, InterleaveWays, TargetId);

  //
  //index: 3b{die, routeType}
  //
  Index = ((TargetId & 1) << 2 ) | RouteTableType;

   return  mSadTadScope[Socket].ClusterInfo[ClusterIndex].ChTable[Index][RouteTableIndex];
}


/**
  Check whether this local dram rule is used for CXL.

  @param[in]       Socket        -- Socket index
  @param[in]       ClusterIndex       -- Cluster index.
  @param[in]       DramRuleIndex --  local dram rule index


  @retval        TRUE -- CXL dram rule.

**/
BOOLEAN
EFIAPI
IsCxlDramRule (
  IN      UINT8     Socket,
  IN      UINT8     ClusterIndex,
  IN      UINT8     DramRuleIndex
  )
{
  if ((mSadTadScope[Socket].ClusterInfo[ClusterIndex].DramRule[DramRuleIndex].CxlAcceleratorMem == 1) ||
     (mSadTadScope[Socket].ClusterInfo[ClusterIndex].DramRule[DramRuleIndex].CxlMemExpander == 1)) {
       return TRUE;
     } else {
       return FALSE;
     }
}


/**
  Check whether this local dram rule is used for NXM (Non Exist Memory) -- memory hole.

  @param[in]       Socket        -- Socket index
  @param[in]       ClusterIndex       -- Cluster index.
  @param[in]       DramRuleIndex --  local dram rule index


  @retval        TRUE -- NXM dram rule.

**/
BOOLEAN
EFIAPI
IsNxmDramRule (
  IN      UINT8     Socket,
  IN      UINT8     ClusterIndex,
  IN      UINT8     DramRuleIndex
  )
{
  if ((mSadTadScope[Socket].ClusterInfo[ClusterIndex].DramRule[DramRuleIndex].NxmFlag == 1)) {
       return TRUE;
     } else {
       return FALSE;
     }
}


/**
  Check whether this local dram rule is NM cache

  @param[in]       Socket        -- Socket index
  @param[in]       ClusterIndex       -- Cluster index.
  @param[in]       DramRuleIndex --  local dram rule index


  @retval        TRUE -- NM cache dram rule.

**/
BOOLEAN
EFIAPI
IsNmCache (
  IN      UINT8     Socket,
  IN      UINT8     ClusterIndex,
  IN      UINT8     DramRuleIndex
  )
{
  if (mSadTadScope[Socket].ClusterInfo[ClusterIndex].DramRule[DramRuleIndex].NmCachable == 1) {
    return TRUE;
  } else {
    return FALSE;
  }
}


/**
  forward decode SA belong to block mode space.

  @param[in]       Sa        -- System address
  @param[in, out]  Socket    -- Socket index
  @param[in, out]  Mc        -- mc index
  @param[in, out]  chn       -- channel


  @retval        TRUE -- this system address belong to block mode space.

**/
BOOLEAN
EFIAPI
ForwardDecodeBlockAddress (
  IN      UINT64    Sa,
  IN OUT  UINT8    *Socket,
  IN OUT  UINT8    *Mc,
  IN OUT  UINT8    *Chn
  )
{
  BOOLEAN  Flag = FALSE;
  if (IsBlockmodeEnable ()) {
    if ((Sa >= mSadTadScope[0].BlockMode.BlockBase)
      && (Sa <= mSadTadScope[0].BlockMode.BlockLimit)) {

     Flag = TRUE;

     if (Socket != NULL) {
       *Socket = (UINT8)BitFieldRead64 (Sa, 16, 18);
     }

     if (Mc != NULL) {
       *Mc = (UINT8)BitFieldRead64 (Sa, 12, 13);
     }

     if (Chn != NULL) {
       *Chn = (UINT8)BitFieldRead64 (Sa, 14, 15);
     }
    }
  }

  return Flag;
}
