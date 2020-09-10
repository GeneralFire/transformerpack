/** @file
  Implementation of error injection library.

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

#include <Library/BaseLib.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/RasDebugLib.h>

#define   PS_INTERVAL   0x100000

/**
  Address calculation for XOR mode. Calculation formula,
    a. In case cluster XOR enabled by checking ddr_half_xor_mode_enable [21:21].
      i. In case two-way interleave:
          B[11] = A[11] ^ A[8] ^ A[17] ^ A[25]
      ii.In case four-way interleave:
          B[11] = A[11] ^ A[8] ^ A[17] ^ A[25]
          B[12] = A[12] ^ A[9] ^ A[18] ^ A[26]
    b. In case channel XOR enabled by checking ddr_xor_mode_enable [20:20]
      i. In case two-way interleave:
        B[8] = A[8] ^ A[14] ^ A[22]
      ii. In case four-way interleave:
        B[8] = A[8] ^ A[14] ^ A[22]
        B[9] = A[9] ^ A[15] ^ A[23]
      iii. In case eight-way interleave:
        B[8] = A[8] ^ A[14] ^ A[22]
        B[9] = A[9] ^ A[15] ^ A[23]
        B[10] = A[10] ^ A[16] ^ A[24]

  @param[in]  PhyAddr   Original physical address
  @param[in]  ClusterXorModeEn   Cluster XOR mode enable
  @param[in]  ChXorModeEn        Channel XOR mode enable
  @param[in]  TargetWays         Target interleave ways
  @param[in]  ChannelWays        Channel interleave ways

  @retval       calculated address

**/
UINT64
EFIAPI
XorModeAddressCalc (
  IN  UINT64      PhyAddr,
  IN  UINT32      ClusterXorModeEn,
  IN  UINT32      ChXorModeEn,
  IN  UINT8       TargetWays,
  IN  UINT8       ChannelWays
  )
{
  UINT64 PhyAddrB;

  PhyAddrB = PhyAddr;

  if (ClusterXorModeEn) {
    switch (TargetWays) {
      case 2:                 //2-Way
        PhyAddrB = BitFieldWrite64 (PhyAddrB, 11, 11,
                (BitFieldRead64 (PhyAddr, 11, 11) ^
                BitFieldRead64 (PhyAddr, 8, 8) ^
                BitFieldRead64 (PhyAddr, 17, 17) ^
                BitFieldRead64 (PhyAddr, 25, 25)));
        break;
      case 4:                 //4-Way
        PhyAddrB = BitFieldWrite64 (PhyAddrB, 11, 11,
                (BitFieldRead64 (PhyAddr, 11, 11) ^
                BitFieldRead64 (PhyAddr, 8, 8) ^
                BitFieldRead64 (PhyAddr, 17, 17) ^
                BitFieldRead64 (PhyAddr, 25, 25)));
        PhyAddrB = BitFieldWrite64 (PhyAddrB, 12, 12,
                (BitFieldRead64 (PhyAddr, 12, 12) ^
                BitFieldRead64 (PhyAddr, 9, 9) ^
                BitFieldRead64 (PhyAddr, 18, 18) ^
                BitFieldRead64 (PhyAddr, 26, 26)));
        break;
      default:
        break;
    }
  }
  if (ChXorModeEn) {
    switch (ChannelWays) {
      case 2:                 //2-Way
        PhyAddrB = BitFieldWrite64 (PhyAddrB, 8, 8,
                (BitFieldRead64 (PhyAddr, 8, 8) ^
                BitFieldRead64 (PhyAddr, 14, 14) ^
                BitFieldRead64 (PhyAddr, 22, 22)));
        break;
      case 4:                 //4-Way
        PhyAddrB = BitFieldWrite64 (PhyAddrB, 8, 8,
                (BitFieldRead64 (PhyAddr, 8, 8) ^
                BitFieldRead64 (PhyAddr, 14, 14) ^
                BitFieldRead64 (PhyAddr, 22, 22)));
        PhyAddrB = BitFieldWrite64 (PhyAddrB, 9, 9,
                (BitFieldRead64 (PhyAddr, 9, 9) ^
                BitFieldRead64 (PhyAddr, 15, 15) ^
                BitFieldRead64 (PhyAddr, 23, 23)));
        break;
      case 8:                 //8-Way
        PhyAddrB = BitFieldWrite64 (PhyAddrB, 8, 8,
                (BitFieldRead64 (PhyAddr, 8, 8) ^
                BitFieldRead64 (PhyAddr, 14, 14) ^
                BitFieldRead64 (PhyAddr, 22, 22)));
        PhyAddrB = BitFieldWrite64 (PhyAddrB, 9, 9,
                (BitFieldRead64 (PhyAddr, 9, 9) ^
                BitFieldRead64 (PhyAddr, 15, 15) ^
                BitFieldRead64 (PhyAddr, 23, 23)));
        PhyAddrB = BitFieldWrite64 (PhyAddrB, 10, 10,
                (BitFieldRead64 (PhyAddr, 10, 10) ^
                BitFieldRead64 (PhyAddr, 16, 16) ^
                BitFieldRead64 (PhyAddr, 24, 24)));
        break;
      default:
        break;
    }
  }

  return PhyAddrB;
}




/**
  Find address for patrol scrub to start up. Patrol scrub will take 1 or 2 seconds to reach target address.


  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  TargetAddr      target address for patrol scrub to reach.

  @retval     calculate address before XOR.

**/
UINT64
EFIAPI
GetPsStartAddress (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT64  TargetAddr
  )
{
  UINT64      AddrB;
  UINT8       MinTad;
  UINT8       MaxTad;
  UINT8       TadIndex;
  UINT64      TadBase;
  UINT64      TadLimit;
  UINT64      TadBase2;
  UINT64      TadLimit2;
  BOOLEAN     RoundUp2MaxTad = FALSE;

  if (TargetAddr >=  PS_INTERVAL) {
    AddrB = TargetAddr - PS_INTERVAL;
  } else {
    RoundUp2MaxTad = TRUE;
  }

  //
  //Get patrol scrub min/max tad.
  //
  GetPatrolScrubRange (Skt, ChOnSkt, &MinTad, &MaxTad);
  RAS_DEBUG((LEVEL_REG, "MinTad:%d   MaxTad:%d  \n", MinTad, MaxTad));

  for (TadIndex = MinTad; (TadIndex <= MaxTad) && (RoundUp2MaxTad == FALSE); TadIndex ++) {

    GetTadInfoRange (Skt, ChOnSkt, TadIndex, &TadBase, &TadLimit);

    //
    //hit the TAD's range.
    //
    if ((AddrB >=  TadBase) && (AddrB < TadLimit)) {
      break;
    }

    if ((TadIndex == MinTad) &&
       (AddrB < TadBase)) {
       RoundUp2MaxTad = TRUE;
       break;
    }

    //
    //hit memory hole.
    //
    if ((TadIndex > MinTad) &&
       (AddrB < TadBase)) {
       GetTadInfoRange (Skt, ChOnSkt, TadIndex - 1, &TadBase2, &TadLimit2);
       //
       //move to previous TAD.
       //
       AddrB -= (TadBase - TadLimit2);
       break;
    }
  }

  if (RoundUp2MaxTad == TRUE) {
    GetTadInfoRange (Skt, ChOnSkt, MinTad, &TadBase, &TadLimit);
    GetTadInfoRange (Skt, ChOnSkt, MaxTad, &TadBase2, &TadLimit2);

    AddrB = TadLimit2 - (TargetAddr - TadBase);
  }

  return AddrB;
}