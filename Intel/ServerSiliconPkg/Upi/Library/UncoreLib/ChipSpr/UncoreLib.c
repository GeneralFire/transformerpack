/** @file
  This file includes UNCORE related functions which are specific to SPR.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation. <BR>

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


#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>
#include <KtiMisc.h>
#include <KtiSetupDefinitions.h>
#include <Upi/KtiSi.h>
#include <UncoreCommonIncludes.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemRcLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Library/IoAccessLib.h>
#include <ScratchpadList.h>
#include <Library/ReferenceCodePolicyLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/KtiSimLib.h>
#include <Library/IoLib.h>
#include <RcRegs.h>
#include <Library/ChaLib.h>
#include <Library/ChaLibSpr.h>
#include <Guid/UboxIpInterface.h>
#include <Library/Ms2IdiLib.h>
#include <Library/OobIpLib.h>
#include <Library/PcuIpLib.h>
#include <Library/M2IalIpLib.h>
#include <Guid/KtiIpInterface.h>
#include <Guid/IioIpInterface.h>
#include <Guid/SpkIpInterface.h>
#include <Guid/MsmIpInterface.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemTypeLib.h>
#include <Library/MdfisIpLib.h>
#include <Library/CxlLib.h>
#include <Library/CxlIpLib.h>
#include <Library/RcrbAccessLib.h>
#include <Guid/SocketCommonRcVariable.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/SysHostPointerLib.h>
#include "UncoreLibInternal.h"
#include <Library/SncPrefetchLib.h>
#include <Library/PipeSyncLib.h>
#include <Register/Intel/Msr.h>
#include <Register/Intel/Cpuid.h>

typedef enum {
  MULTI_SKT_UPT_MDFIS_TRAINING = 0,
  MULTI_SKT_UPT_LATE_ACTION_REG_UPDATE
} MULTI_SKT_UPT_TYPE;

//
//SPR Stack Personalities - from Table 2-1 in 10nm MS2IOSF HAS
//
UINT8 SprSpStackPersonalities[MAX_LOGIC_IIO_STACK] = {
  TYPE_UBOX_IIO,
  TYPE_UBOX_IIO,
  TYPE_UBOX_IIO,
  TYPE_UBOX_IIO,
  TYPE_UBOX_IIO,
  TYPE_UBOX_IIO,
  TYPE_UBOX_IIO,
  TYPE_UBOX_IIO,
  TYPE_DINO,
  TYPE_DINO,
  TYPE_DINO,
  TYPE_DINO,
  TYPE_RESERVED,
  TYPE_UBOX
};

UINT32 SprSpSpkInstancesBarOffset[] = {
  0x58000, // SCF_IOCOH0_SPK
  0x59000, // SCF_IOCOH1_SPK
  0x5A000, // SCF_IOCOH2_SPK
  0x5B000, // SCF_IOCOH3_SPK
  0x5C000, // SCF_IOCOH4_SPK
  0x5D000, // SCF_IOCOH5_SPK
  0x5E000, // SCF_IOCOH6_SPK
  0x5F000, // SCF_IOCOH7_SPK
  0x60000, // SCF_IOCOH8_SPK
  0x61000, // SCF_IOCOH9_SPK
  0x62000, // SCF_IOCOH10_SPK
  0x63000, // SCF_IOCOH11_SPK
  0x70000, // SCF_UPI_1X_0_SPK
  0x71000, // SCF_UPI_1X_1_SPK
  0x72000, // SCF_UPI_1X_2_SPK
  0x73000, // SCF_UPI_1X_3_SPK
};

/**
  Returns the total Iios enabled for a given CPU socket.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param SocId             - Soc ID of the CPU who's enabled IIOs info is requested, 0,1,2,3,etc
  @param IIOCount          - Total IIOs in given socket

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
GetIioCount (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId,
  OUT UINT8                     *IioCount
  )
{
  UINT8   Ctr;
  UINT8   MaxStackNum, Count = 0;
  UINT32  Ms2IosfList, Ms2IosfStacksEn;

  MaxStackNum = RSVD_STACK;

  //
  // Get MS2IOSF in stack n is enabled or not
  //
  Ms2IosfStacksEn = PcuGetMs2IosfStacksEn (SocId);
  Ms2IosfList     = BitFieldRead32 (Ms2IosfStacksEn, 0, MaxStackNum - 1);
  for (Ctr = 0; Ctr < MaxStackNum; Ctr++) {
    if ((Ms2IosfList & BIT0) != 0) {
      Count++;
      SetStackPresent (SocId, Ctr);
    }
    Ms2IosfList = Ms2IosfList >> 1;
  }

  KTIVAR->CpuInfo[SocId].CpuRes.stackPresentBitmap = GetStackPresentBitMap (SocId);
  *IioCount = Count;

  return KTI_SUCCESS;
}

/**
  Hide MCP devices on all valid sockets

  @param KtiInternalGlobal - KTIRC internal variables.
  @param SocketData        - Socket Data

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
HideMcpDevices (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_SOCKET_DATA            *SocketData
  )
{
  return KTI_SUCCESS;
}

/**
  Assigns resources for CPUs.

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.
  @param BusSize           - Bus Size
  @param TmpTotCpu         - NumberofCpus for purpose of resource allocation

  @retval KTI_SUCCESS

**/
KTI_STATUS
CalculateBusResources (
  KTI_SOCKET_DATA     *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8               *BusSize,
  UINT8               TmpTotCpu
  )
{
  UINT8          Socket;
  UINT8          Stack;
  UINT32         BusBase;
  UINT32         NumberBusPerSocket;
  UINT8          NumberIio[MAX_SOCKET];
  UINT8          NumberMcp[MAX_SOCKET];
  UINT8          NumberFpga[MAX_SOCKET];
  UINT8          NumberHfi[MAX_SOCKET];
  UINT8          NumberNac[MAX_SOCKET];
  UINT8          NumberDINO[MAX_SOCKET];
  UINT8          BusPerIio;
  UINT8          BusRsvdRootBusC;
  UINT8          BusRsvdDino;
  UINT8          RemainingBus;
  UINT8          TmpBusBase;
  UINT8          TmpBusLimit;
  UINT8          ReservedBus;
  UINT8          StackPersonality;

  ZeroMem ((VOID *) NumberIio, sizeof (NumberIio));
  ZeroMem ((VOID *) NumberMcp, sizeof (NumberMcp));
  ZeroMem ((VOID *) NumberFpga, sizeof (NumberFpga));
  ZeroMem ((VOID *) NumberHfi, sizeof (NumberHfi));
  ZeroMem ((VOID *) NumberNac, sizeof (NumberNac));
  ZeroMem ((VOID *) NumberDINO, sizeof (NumberDINO));

  //
  // Check input
  //
  KTI_ASSERT ((TmpTotCpu <= MAX_SOCKET), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_C);

  //
  // Initialize the Base value of the resources
  //
  BusBase = TmpBusBase = TmpBusLimit = BusPerIio = 0;
  KtiInternalGlobal->MmcfgBase = KTIVAR->mmCfgBase;
  KtiInternalGlobal->IsMmcfgAboveMmiol= FALSE;

  //
  // Determine number of each type stack per socket.
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketData->Cpu[Socket].Valid == TRUE || KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE ) {
      if (SocketData->Cpu[Socket].SocType == SOCKET_TYPE_CPU) {
        NumberIio[Socket] = NumberMcp[Socket] = NumberHfi[Socket] = NumberNac[Socket] = NumberDINO[Socket] = 0;
        for (Stack = 0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
          switch (KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].Personality) {
            case TYPE_NONE:
              break;
            case TYPE_HFI:
              NumberHfi[Socket] = NumberHfi[Socket] + 1;
              break;
            case TYPE_UBOX_IIO:
              //
              // We only evenly split across Pcie Stacks.   Dmi stack will be handled as special case below
              //
              if (Stack != IIO_STACK0) {
                  NumberIio[Socket] = NumberIio[Socket] + 1;
              }
              break;
            case TYPE_NAC:
              NumberNac[Socket] = NumberNac[Socket] + 1;
              break;
            case TYPE_MCP:
              NumberMcp[Socket] = NumberMcp[Socket] + 1;
              break;
            case TYPE_FPGA:
              NumberFpga[Socket] = NumberFpga[Socket] + 1;
              break;
            case TYPE_DINO:
              NumberDINO[Socket] = NumberDINO[Socket] + 1;
              break;
            default:
              //Automated add of default case. Please review.
              break;
          }
        }
      } else {
        //
        // Fpga, reserve 1 bus, TBD on Whitley FPGA
        //
        NumberIio[Socket] = NumberMcp[Socket] = NumberHfi[Socket] = NumberNac[Socket] = NumberDINO[Socket] = 0;
      }
    }
  }

  Socket = KtiInternalGlobal->SbspSoc;
  KTIVAR->CpuInfo[Socket].CpuRes.SegMmcfgBase.hi = 0;
  KTIVAR->CpuInfo[Socket].CpuRes.SegMmcfgBase.lo = KTIVAR->mmCfgBase + (KTIVAR->CpuInfo[Socket].CpuRes.PciSegment * 256*1024*1024);
  KTIVAR->CpuInfo[Socket].CpuRes.BusBase  = (UINT8)BusBase;
  KTIVAR->CpuInfo[Socket].CpuRes.BusLimit = (UINT8)(BusBase + (BusSize[Socket] * SAD_BUS_GRANTY) - 1);
  NumberBusPerSocket = KTIVAR->CpuInfo[Socket].CpuRes.BusLimit - BusBase + 1;
  BusRsvdRootBusC = 0;

  if (KtiInternalGlobal->ChopType[Socket] != TypeMcc) {
    BusRsvdDino = BUS_PER_DINO_HCA_SPR;
  } else {
    BusRsvdDino = BUS_PER_DINO_HCB_SPR;
  }

  if (KTIVAR->CpuInfo[Socket].CpuRes.BusLimit == 0xFF) {
    //Only reserve RootBusC and RootBusC+1 (0xFC and 0xFD) on last socket in segment (Socket BusLimit == 0xFF)
    BusRsvdRootBusC = BUS_RSVD_10nm;
  }
  ReservedBus = BUS_PER_UBOX_10nm + (BUS_PER_MCP * NumberMcp[Socket]) + (BUS_PER_FPGA * NumberFpga[Socket])
                + (BUS_PER_NAC * NumberNac[Socket]) + BUS_PER_LEG_STK + (BusRsvdDino * NumberDINO[Socket]) + BusRsvdRootBusC;
  if ((NumberIio[Socket] + NumberHfi[Socket]) != 0) {
    BusPerIio = (UINT8)(NumberBusPerSocket - ReservedBus) / (NumberIio[Socket] + NumberHfi[Socket]);
    RemainingBus = (UINT8)(NumberBusPerSocket - ReservedBus) % (NumberIio[Socket] + NumberHfi[Socket]);
  } else {
    RemainingBus = (UINT8)(NumberBusPerSocket - ReservedBus);
  }

  //
  // Assign the resources for CPUs. Legacy socket's resource range starts at the base of the resource.
  //
  for (Stack = 0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
    StackPersonality = KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].Personality;
    switch (StackPersonality) {
      case TYPE_HFI:
        TmpBusBase = (UINT8)BusBase;
        TmpBusLimit = TmpBusBase + BusPerIio - 1;
        break;
      case TYPE_UBOX_IIO:
        TmpBusBase = (UINT8)BusBase;
        if (Stack == IIO_STACK0) {
          //
          // Assign preallocated and remaining buses to Cstack,
          //
          TmpBusLimit = TmpBusBase + BUS_PER_LEG_STK + RemainingBus - 1;
        } else {
          TmpBusLimit = TmpBusBase + BusPerIio - 1;
        }
        break;
      case TYPE_UBOX:
        TmpBusBase = (UINT8)BusBase;
        TmpBusLimit = TmpBusBase + BUS_PER_UBOX_10nm - 1;
        break;
      case TYPE_NAC:
        TmpBusBase = (UINT8)BusBase;
        TmpBusLimit = TmpBusBase + BUS_PER_NAC - 1;
        break;
      case TYPE_MCP:
        TmpBusBase = (UINT8)BusBase;
        TmpBusLimit = TmpBusBase + BUS_PER_MCP - 1;
        break;
      case TYPE_FPGA:
        TmpBusBase = (UINT8)BusBase;
        TmpBusLimit = TmpBusBase + BUS_PER_FPGA - 1;
        break;
      case TYPE_DINO:
        TmpBusBase = (UINT8)BusBase;
        TmpBusLimit = TmpBusBase + BusRsvdDino - 1;
        break;
      case TYPE_RESERVED:
        if (BusRsvdRootBusC != 0) {
          TmpBusBase = (UINT8)BusBase;
          TmpBusLimit = TmpBusBase + BusRsvdRootBusC - 1;
        } else {
          TmpBusBase = 0xFF;
          TmpBusLimit = 0;
        }
        break;

      //Do not assign bus numbers to these stack types
      case TYPE_NONE:
      default:
        TmpBusBase = 0xff;
        TmpBusLimit = 0;
        break;
    }

    KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].BusBase = TmpBusBase;
    KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].BusLimit = TmpBusLimit;

    //
    // Only increment if stack was enabled
    //
    if (TmpBusLimit) {
      BusBase = TmpBusLimit + 1;
    }

  }

  //
  // Assign the resources for CPUs. Legacy socket's resource range starts at the base of the resource.
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (Socket == KtiInternalGlobal->SbspSoc) {
      continue;
    }
    //
    // if socket exists or Hot Plug enabled (reserved for onlining sockets)
    //
    if (((SocketData->Cpu[Socket].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) ) {
      // Update Bus Base & Limit
      KTIVAR->CpuInfo[Socket].CpuRes.SegMmcfgBase.hi = 0;
      KTIVAR->CpuInfo[Socket].CpuRes.SegMmcfgBase.lo = KTIVAR->mmCfgBase + (KTIVAR->CpuInfo[Socket].CpuRes.PciSegment * 256*1024*1024);
      KTIVAR->CpuInfo[Socket].CpuRes.BusBase  = (UINT8)BusBase;
      KTIVAR->CpuInfo[Socket].CpuRes.BusLimit = (UINT8)(BusBase + (BusSize[Socket] * SAD_BUS_GRANTY) - 1);
      NumberBusPerSocket = KTIVAR->CpuInfo[Socket].CpuRes.BusLimit - KTIVAR->CpuInfo[Socket].CpuRes.BusBase + 1;
      BusRsvdRootBusC = 0;
      if (KTIVAR->CpuInfo[Socket].CpuRes.BusLimit == 0xFF) {
        //Only reserve RootBusC and RootBusC+1 (0xFC and 0xFD) on last socket in segment (Socket BusLimit == 0xFF)
        BusRsvdRootBusC = BUS_RSVD_10nm;
      }
      ReservedBus = BUS_PER_UBOX_10nm + (BUS_PER_MCP * NumberMcp[Socket]) + (BUS_PER_FPGA * NumberFpga[Socket])
                    + BUS_PER_NONLEG_STK_10nm + (BusRsvdDino * NumberDINO[Socket]) + BusRsvdRootBusC;
      BusPerIio = ((NumberIio[Socket] + NumberHfi[Socket]) == 0) ? 0 : (UINT8)(NumberBusPerSocket - ReservedBus) / (NumberIio[Socket] + NumberHfi[Socket]);
      RemainingBus = ((NumberIio[Socket] + NumberHfi[Socket]) == 0) ? 0 : (UINT8)(NumberBusPerSocket - ReservedBus) % (NumberIio[Socket] + NumberHfi[Socket]);

      if ((KtiInternalGlobal->TotIio[Socket] - NumberDINO[Socket]) == 1) {
        //
        // Only 1 IIO stack (DINO stacks not included, BIOS reserves "BusRsvdDino" for DINO) per socket, assign all possible bus to this stack
        //
        ReservedBus = BUS_PER_UBOX_10nm + (BUS_PER_MCP * NumberMcp[Socket]) + (BUS_PER_FPGA * NumberFpga[Socket])
                     + (BusRsvdDino * NumberDINO[Socket]) + BusRsvdRootBusC;
        BusPerIio = (UINT8)(NumberBusPerSocket - ReservedBus);
        RemainingBus = 0;
      }

      for (Stack=0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
        StackPersonality = KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].Personality;
        switch (StackPersonality) {
          case TYPE_HFI:
            TmpBusBase = (UINT8)BusBase;
            TmpBusLimit = TmpBusBase + BusPerIio - 1;
            break;
          case TYPE_UBOX_IIO:
            TmpBusBase = (UINT8)BusBase;
            if ((Stack == IIO_STACK0) && ((KtiInternalGlobal->TotIio[Socket] - NumberDINO[Socket]) > 1)) {
              //
              // Assign preallocated and remaining buses to Cstack
              //
              TmpBusLimit = TmpBusBase + BUS_PER_NONLEG_STK_10nm + RemainingBus - 1;
            } else {
              TmpBusLimit = TmpBusBase + BusPerIio -1;
            }
            break;
          case TYPE_UBOX:
            TmpBusBase = (UINT8)BusBase;
            TmpBusLimit = TmpBusBase + BUS_PER_UBOX_10nm - 1;
            break;
          case TYPE_MCP:
            TmpBusBase = (UINT8)BusBase;
            TmpBusLimit = TmpBusBase + BUS_PER_MCP - 1;
            break;
          case TYPE_NAC:
            TmpBusBase = (UINT8)BusBase;
            TmpBusLimit = TmpBusBase + BUS_PER_NAC - 1;
            break;
          case TYPE_FPGA:
            TmpBusBase = (UINT8)BusBase;
            TmpBusLimit = TmpBusBase + BUS_PER_FPGA - 1;
            break;
          case TYPE_DINO:
            TmpBusBase = (UINT8)BusBase;
            TmpBusLimit = TmpBusBase + BusRsvdDino - 1;
            break;
          case TYPE_RESERVED:
            if (BusRsvdRootBusC != 0) {
              TmpBusBase = (UINT8)BusBase;
              TmpBusLimit = TmpBusBase + BusRsvdRootBusC - 1;
            } else {
              TmpBusBase = 0xFF;
              TmpBusLimit = 0;
            }
            break;

          //Do not assign bus numbers to these stack types
          case TYPE_NONE:
          default:
            TmpBusBase = 0xff;
            TmpBusLimit = 0;
            break;
        }

        KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].BusBase = TmpBusBase;
        KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].BusLimit = TmpBusLimit;

        //
        // Only increment if stack was enabled
        //
        if (TmpBusLimit) {
          BusBase = TmpBusLimit + 1;
        }
      }
      //
      // Init next BusBase.  This handles case of socket types that do not full utilize their allotted bus assignments (FPGA)
      //
      BusBase = KTIVAR->CpuInfo[Socket].CpuRes.BusLimit + 1;
    } else {
      KTIVAR->CpuInfo[Socket].CpuRes.BusBase = 0xFF;
      KTIVAR->CpuInfo[Socket].CpuRes.BusLimit = 0;
      KTIVAR->CpuInfo[Socket].CpuRes.SegMmcfgBase.lo = 0xffffffff;
      KTIVAR->CpuInfo[Socket].CpuRes.SegMmcfgBase.hi = 0xffffffff;
    }
  }

  return KTI_SUCCESS;
}

/**
  Routine to program IIO BUS NUM for the given socket

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
UpdateCpuBusOnIio (
  KTI_HOST_INTERNAL_GLOBAL    *KtiInternalGlobal,
  UINT8                       SocId
  )
{
  UINT32  BusBase;
  UINT8   Idx1;
  UINT8   StackPersonality;

  //
  // Scan all valid iio stack and update root bus field
  //
  for (Idx1 = 0; Idx1 < MAX_LOGIC_IIO_STACK; Idx1++) {
    //
    // Check for valid resources
    //
    StackPersonality = KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Idx1].Personality;
    if ((StackPersonality == TYPE_UBOX_IIO)
      || (StackPersonality == TYPE_HFI)
      || (StackPersonality == TYPE_NAC)
      || (StackPersonality == TYPE_DINO))
    {
      if (KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Idx1].BusLimit >= KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Idx1].BusBase) {
        BusBase = KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Idx1].BusBase;
        IioSetRootBusBaseEn (SocId, Idx1, BusBase, BUS_NUM_ROOTBUSC);
      }
    }
  }
  return KTI_SUCCESS;
}

/**
  Routine to get KTI routing in MS2IOSF.

  @param SocId              - Soc ID of the CPU
  @param SocketData         - Pointer to socket specific data
  @param KtiInternalGlobal  - KTIRC Internal Global data
  @param IioIndex           - Iio Index
  @param KtiRtInMs2Iosf     - Return KTI Routing in MS2IOSF
  @param KtiRtModeInMs2Iosf - Routing mode

  @retval None

**/
VOID GetKtiRtInMs2Iosf (
  UINT8                      SocId,
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      IioIndex,
  UINT32                     *KtiRtInMs2Iosf,
  UINT8                      *KtiRtModeInMs2Iosf
  )
{
  UINT8          TwoSPeerSocId;
  UINT32         Data32;
  UINT64         KtiRtInCha;
  UINT8          RtTable[4] = {0x24, 0x39, 0xE, 0x13};

  KtiRtInCha = SocketData->Route[SocId].ChaKtiRt | LShiftU64 (SocketData->Route[SocId].ChaKtiRt1, 24);
  if (KTIVAR->SysConfig == SYS_CONFIG_2S2L) {
    TwoSPeerSocId = (UINT8) SocketData->Cpu[SocId].LinkData[0].PeerSocId;
    // CHA is using UpiRoutingModeB2
    Data32 = (UINT32)RShiftU64 (KtiRtInCha, TwoSPeerSocId * 6);
    //
    // KtiRtInCha, each entry has 3 bits, KtiRtInMs2Iosf, each entry has 2 bits
    // BIT5 BIT4 BIT3 BIT2 BIT1 BIT0  (CHA)
    //      ---------      ---------
    //         \              \
    //          \              \
    //           \              \
    //           --------- ---------
    //           BIT3 BIT2 BIT1 BIT0  (MS2IOSF)
    //
    *KtiRtInMs2Iosf = (Data32 & 0x3) | ((Data32 >> 1) & 0xC);
    *KtiRtModeInMs2Iosf = 1;
  } else if (KTIVAR->SysConfig == SYS_CONFIG_2S3L) {
    TwoSPeerSocId = (UINT8) SocketData->Cpu[SocId].LinkData[0].PeerSocId;
    // CHA is using UpiRoutingModeC1
    Data32 = (UINT32)RShiftU64 (KtiRtInCha, TwoSPeerSocId * 12);
    //
    // KtiRtInCha, each entry has 3 bits, KtiRtInMs2Iosf, each entry has 2 bits
    // BIT8 BIT7 BIT6 BIT5 BIT4 BIT3 BIT2 BIT1 BIT0  (CHA)
    //      ---------      ---------      ---------
    //         \             \              \
    //          \             \              \
    //           \             \              \
    //            -----         ---            --
    //                 \           \             \
    //                  |           |             |
    //                --------- ---------  ---------
    //                BIT5 BIT4 BIT3 BIT2  BIT1 BIT0  (MS2IOSF)
    //
    *KtiRtInMs2Iosf = (Data32 & 0x3) | ((Data32 >> 1) & 0xC) | ((Data32 >> 2) & 0x30);
    *KtiRtModeInMs2Iosf = 2;
  } else if (KTIVAR->SysConfig == SYS_CONFIG_2S4L) {
    //
    // MS2IOSF doesn’t support 2S-4L interleaving, we should configure it for 2S-3L, and use
    // 3 of the 4 available links evenly distributed among the MS2IOSF instances. For example:
    // MS2IOSF Instance | UPI links
    // ----------------------------
    // 0                | 0, 1, 2
    // 1                | 1, 2, 3
    // 2                | 2, 3, 0
    // 3                | 3, 0, 1
    // ...
    //
    *KtiRtInMs2Iosf = RtTable[IioIndex % 4];
    *KtiRtModeInMs2Iosf = 2;
  } else {
    *KtiRtInMs2Iosf = SocketData->Route[SocId].M2PcieKtiRt[0];
    *KtiRtModeInMs2Iosf = 0;
  }
}

/**
  Routine to program the final route, VN selection and fanout setting for the given topology.
  Refer to KtiDisc.c file for more info on how the route settings are calculated.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramSystemRoute (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  TOPOLOGY_TREE_NODE  *SbspTree;
  TOPOLOGY_TREE_NODE  *TopologyTree;
  TOPOLOGY_TREE_NODE  TreeNode;
  UINT8               TreeIdx;
  UINT8               Ctr, Ctr1;
  UINT32              Data32;
  UINT8               sbspSktId;
  UINT32              TempR2pGnCtrl[MAX_SOCKET][MAX_IIO_STACK];
  UINT32              NumOfHops[MAX_SOCKET];
  UINT32              Nid2KtiMapLow;
  UINT32              Nid2KtiMapHigh;
  UINT64              Nid2KtiMap;
  UINT32              KtiRtInMs2Iosf;
  UINT8               M2mUpiInterleaveMode;
  BOOLEAN             ProgramPeMode;
  UINT8               TwoSPeerSocId;
  UINT8               UpiPort[2];
  UINT8               NodeEntriesWidth;
  UINT8               PortFieldWidth;
  UINT8               KtiRtModeInMs2Iosf;
  UINT8               Mesh2M2UpiMapping_4C4L[] = {0, 2, 1, 3};

  ZeroMem ((UINT8 *) TempR2pGnCtrl, sizeof (TempR2pGnCtrl));
  ZeroMem ((UINT8 *) UpiPort, sizeof (UpiPort));
  KtiCheckPoint (0xFF, 0xFF, 0xFF, STS_PROGRAM_FINAL_ROUTE, MINOR_STS_PROGRAM_FINAL_ROUTE_START);

  sbspSktId = KtiInternalGlobal->SbspSoc;
  SbspTree = SocketData->CpuTree[sbspSktId];

  ProgramPeMode = FALSE;
  //
  // Both ISOC Enabled and demand read over core-prefetch priorityization features need to
  // program PE mode.
  //
  if ((IsCpuAndRevisionOrLater (CPU_SPRSP, REV_C0) && (KTIVAR->OutIsocEn != TRUE) && (KTIVAR->OutXptRemotePrefetch == TRUE) && (KTIVAR->SysConfig <= SYS_CONFIG_4S)) ||
      (KTIVAR->OutIsocEn == TRUE)) {
    ProgramPeMode = TRUE;
  }

  // Program NonBC registers
  if ((KTIVAR->RasInProgress != TRUE) || (KTIVAR->ProgramNonBC == TRUE)) {
    //
    // While the route tables are being programmed, the route is in transient state; since we have to do read-modify-write,
    // we have to read it and remember the register settings so while programming we just modify and write it.
    //
    for (Ctr = 0; Ctr < MAX_SOCKET; Ctr++) {
      if ((SocketData->Cpu[Ctr].Valid == TRUE) && (SocketData->Cpu[Ctr].SocType == SOCKET_TYPE_CPU)) {
        for (Ctr1 = 0; Ctr1 < MAX_IIO_STACK; Ctr1++) {
          if (KTI_HOST_OUT_PTR->M2PciePresentBitmap[Ctr] & (1 << Ctr1)) {
            TempR2pGnCtrl[Ctr][Ctr1] = M2IalGetR2GenCtrl (Ctr, Ctr1);
          }
        }
      }
    }

    //
    // Program the M2PCIe & M2MCP,  M2MEM (for D2K) route table
    //
    for (TreeIdx = 0; SbspTree[TreeIdx].Node.Valid == TRUE; TreeIdx++) {
      TreeNode = SbspTree[TreeIdx];
      if (TreeNode.Node.SocType == SOCKET_TYPE_CPU) {
        //program m2ubox route table
        UBoxConfigureRouteTable ((UINT8) TreeNode.Node.SocId, 0, SocketData->Route[TreeNode.Node.SocId].M2PcieKtiRt[0]);
        //program m2pcie route table
        Ctr = 0;
        for (Ctr1 = 0; Ctr1 < MAX_IIO_STACK; Ctr1++) {
          if (KTI_HOST_OUT_PTR->M2PciePresentBitmap[TreeNode.Node.SocId] & (1 << Ctr1)) {
            GetKtiRtInMs2Iosf ((UINT8) TreeNode.Node.SocId, SocketData, KtiInternalGlobal, Ctr, &KtiRtInMs2Iosf, &KtiRtModeInMs2Iosf);
            M2IalSetRouteTable ((UINT8) TreeNode.Node.SocId, Ctr1, KtiRtModeInMs2Iosf, KtiRtInMs2Iosf);
            Ctr++;
          }
        }

        // setup D2kEn in M2MEM
        if (KtiInternalGlobal->D2kEn == TRUE) {
          if (KTIVAR->UpiAffinityEn) {
            //
            // 4Die 4UPI layout                 4Die 2UPI layout
            // ------------------------     ------------------------
            // |UPI0      |       UPI1|     |UPI0      |       UPI1|
            // |  Die0    |   Die2    |     |  Die0    |   Die2    |
            // |M2M0      |       M2M2|     |M2M0      |       M2M2|
            // ------------------------     ------------------------
            // |  Die1    |   Die3    |     |  Die1    |   Die3    |
            // |M2M1      |       M2M3|     |M2M1      |       M2M3|
            // |UPI2      |       UPI3|     |          |           |
            // ------------------------     ------------------------
            //
            PortFieldWidth = 3;
            TwoSPeerSocId = (UINT8) SocketData->Cpu[TreeNode.Node.SocId].LinkData[0].PeerSocId;
            for (Ctr1 = 0; Ctr1 < KTIVAR->MaxSupportedM2m; Ctr1++) {
              if ((KTIVAR->CpuInfo[(UINT8) TreeNode.Node.SocId].M2mBitmap & (1 << Ctr1)) == 0) {
                continue;
              }
              if (KTIVAR->SysConfig == SYS_CONFIG_2S2L) {
                if ((Ctr1 & BIT1) == BIT1) { // M2M2/M2M3
                  UpiPort[0] = 1;
                  UpiPort[1] = 1;
                } else { // M2M0/M2M1
                  UpiPort[0] = 0;
                  UpiPort[1] = 0;
                }
              } else { // 2S4L
                if ((KTIVAR->OutSncEn == 4) ||
                   ((KTIVAR->OutSncEn == 0) && (KTIVAR->OutUmaClustering == UMABASEDCLUSTERING_QUADRANT))) {
                  UpiPort[0] = Mesh2M2UpiMapping_4C4L[Ctr1];
                  UpiPort[1] = Mesh2M2UpiMapping_4C4L[Ctr1];
                } else {
                  // SNC2/Hemi mode, UPI4: M2M0/M2M1 interleave between U0/U2
                  //                       M2M2/M2M3 interleave between U1/U3
                  if ((Ctr1 & BIT1) == BIT1) { // M2M2/M2M3
                    UpiPort[0] = 1;
                    UpiPort[1] = 3;
                  } else { // M2M0/M2M1
                    UpiPort[0] = 0;
                    UpiPort[1] = 2;
                  }
                }
              }
              if (KtiInternalGlobal->UpiRoutingMode == UpiRoutingModeB2) { // 2S2L
                Data32 = UpiPort[0] | (UpiPort[1] << 3);
                NodeEntriesWidth = PortFieldWidth * 2;
              } else { // 2S4L: UpiRoutingModeD0
                Data32 = UpiPort[0] | (UpiPort[1] << 3) | (UpiPort[0] << 6) | (UpiPort[1] << 9);
                NodeEntriesWidth = PortFieldWidth * 4;
              }
              Nid2KtiMap = LShiftU64 (Data32, NodeEntriesWidth * TwoSPeerSocId);
              Nid2KtiMapLow  = (UINT32)(Nid2KtiMap & 0xFFFFFFFF);
              Nid2KtiMapHigh = (UINT32)RShiftU64 (Nid2KtiMap, 32);
              SetM2mNid2KtiMap ((UINT8) TreeNode.Node.SocId, Ctr1, Nid2KtiMapLow, Nid2KtiMapHigh);
            }
          } else {
            //
            // Construct Mesh2Mem NID2KTIMap data from ChaKtiRt tables.
            // Bits[23:00] in ChaKtiRt contains first 8 entries (NodeID 0-3 for B2 mode; NodeID 0-1 for C1 mode).
            // Bits[23:00] in ChaKtiRt1 contains the last 8 entries (NodeID 4-7 for B2 mode; NodeID 2-3 for C1 mode).
            // Mesh2mem NID2KTI register is a 64-bit CR, where nid2ktimap field is a 48-bit bitfield.
            // Therefore, the first 32 bits reside in NID2KTI_N0 and the remaining 16 bits reside in NID2KTI_N1 register.
            //
            Nid2KtiMapLow = SocketData->Route[TreeNode.Node.SocId].ChaKtiRt | (SocketData->Route[TreeNode.Node.SocId].ChaKtiRt1 << 24);
            Nid2KtiMapHigh = SocketData->Route[TreeNode.Node.SocId].ChaKtiRt1 >> 8;
            for (Ctr1 = 0; Ctr1 < KTIVAR->MaxSupportedM2m; Ctr1++) {
              if ((KTIVAR->CpuInfo[(UINT8) TreeNode.Node.SocId].M2mBitmap & (1 << Ctr1)) == 0) {
                continue;
              }
              SetM2mNid2KtiMap (
                (UINT8) TreeNode.Node.SocId,
                Ctr1,
                Nid2KtiMapLow,
                Nid2KtiMapHigh
                );
            }
          }
          if (KtiInternalGlobal->UpiRoutingMode == UpiRoutingModeB2) {        // 2S2L
            M2mUpiInterleaveMode = 0;
          } else if (KtiInternalGlobal->UpiRoutingMode == UpiRoutingModeC1) { // 2S3L
            // Config 1: Index = {NID[1:0], MOD3[1:0]} (interleaves 1 to 3 UPI links between 4 nodes)
            M2mUpiInterleaveMode = 1;
          } else if (KtiInternalGlobal->UpiRoutingMode == UpiRoutingModeD0) { // 2S4L
            M2mUpiInterleaveMode = 2;
          } else { // other topologies
            M2mUpiInterleaveMode = 0;
          }
          for (Ctr1 = 0; Ctr1 < KTIVAR->MaxSupportedM2m; Ctr1++) {
            if ((KTIVAR->CpuInfo[(UINT8) TreeNode.Node.SocId].M2mBitmap & (1 << Ctr1)) == 0) {
              continue;
            }
            SetM2mUpiInterleaveMode ((UINT8) TreeNode.Node.SocId, Ctr1, M2mUpiInterleaveMode);
          }
        }
      } else if (TreeNode.Node.SocType == SOCKET_TYPE_FPGA) {
      } else {
        KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_75);
      }
    }

    //
    // Calculate NumberOfHops for Performance Response Encoding
    //
    for (Ctr = 0; Ctr < MAX_SOCKET; Ctr++) {
      NumOfHops[Ctr]=0;
      if ((SocketData->Cpu[Ctr].Valid == TRUE) && (SocketData->Cpu[Ctr].SocType == SOCKET_TYPE_CPU)) {
        TopologyTree = SocketData->CpuTree[Ctr];
        for (Ctr1 = 0; TopologyTree[Ctr1].Node.Valid != FALSE; Ctr1++) {
          if (TopologyTree[Ctr1].Node.Hop > MAX_NUM_HOPS) {
            TopologyTree[Ctr1].Node.Hop = MAX_NUM_HOPS;              //Max hop is 3 to avoid overflow. This feature is for performance only
          }
          NumOfHops[Ctr] |= TopologyTree[Ctr1].Node.Hop << (2 * TopologyTree[Ctr1].Node.SocId);
        }

        for (Ctr1 = 0; Ctr1 < KtiInternalGlobal->TotCha[Ctr]; Ctr1++) {
          ChaConfigureHopsNum (Ctr, Ctr1, NumOfHops[Ctr]);
        }
      }
    }

    if (UbiosGenerationEnabled()) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n;FafWA Start"));
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n call GetEip"));
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n call FafWa"));
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n;FafWA End\n"));
    }

    for (TreeIdx = 0; SbspTree[TreeIdx].Node.Valid == TRUE; TreeIdx++) {
      TreeNode = SbspTree[TreeIdx];
      if (TreeNode.Node.SocType == SOCKET_TYPE_CPU) {
        //
        // Enable the M2PCIe responses to use the RT for all topologies except 2S
        // Must start with M2PCIE instance 1 (it contains UBOX) for response to use new route-through programming
        //
        if (KTIVAR->SysConfig >= SYS_CONFIG_4S) {
          //
          // program UBOX_M2U portion
          // Set qpi_rt_sel_ndr_drs
          //
          UBoxSelectR2PCIeRouteNdrAndDrs ((UINT8)TreeNode.Node.SocId, 0, 1);
        }

        for (Ctr1 = 0; Ctr1 < MAX_IIO_STACK; Ctr1++) {
          if (KTI_HOST_OUT_PTR->M2PciePresentBitmap[TreeNode.Node.SocId] & (1 << Ctr1)) {
            Data32 = TempR2pGnCtrl[TreeNode.Node.SocId][Ctr1];
            //
            // WA for ICX-SP/SNR/ICXDE: Extra dummy write to ensure the data write to LLC
            //
            M2IalSetR2GenCtrl ((UINT8)TreeNode.Node.SocId, Ctr1, Data32);

            if (KTIVAR->SysConfig >= SYS_CONFIG_4S) {
              //
              // Set qpi_rt_sel_ndr_drs and upi_rt_prog_done
              //
              Data32 |= 0x6;
            } else {
              //
              // Set upi_rt_prog_done
              //
              Data32 |= 0x4;
            }
            M2IalSetR2GenCtrl ((UINT8)TreeNode.Node.SocId, Ctr1, Data32);
          }
        }

        //
        // Program CHA Route Table; it is non BC register. So we need to program in each cha.
        //
        ChaProgramFullKtiRouteTable (
          (UINT8) TreeNode.Node.SocId,
          KtiInternalGlobal->TotCha[TreeNode.Node.SocId],
          KTIVAR->SysConfig,
          KtiInternalGlobal->DualLinksInterleavingMode,
          sbspSktId,
          SocketData,
          KtiInternalGlobal->UpiRoutingMode
          );

        //
        // Program M3KTI and KTI Agent Route Table
        //
        ProgramM3KtiAndKtiAgentRouteTable (SocketData, KtiInternalGlobal, (UINT8) TreeNode.Node.SocId, ProgramPeMode);
      } else if (TreeNode.Node.SocType == SOCKET_TYPE_FPGA) {
      } else {
        KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_76);
      }
    }
  }

  // Program BC registers
  if ((KTIVAR->RasInProgress != TRUE) || (KTIVAR->ProgramBC == TRUE)) {
    if (KtiInternalGlobal->SnoopFanoutEn == TRUE) {
      for (TreeIdx = 0; SbspTree[TreeIdx].Node.Valid == TRUE; TreeIdx++) {
        TreeNode = SbspTree[TreeIdx];
        if (TreeNode.Node.SocType == SOCKET_TYPE_CPU) {
          //
          // Program CHA Snoop Fanout
          //
          ChaConfigureSnoopFanout ((UINT8) TreeNode.Node.SocId, 0, SocketData->Route[TreeNode.Node.SocId].ChaKtiFanOut[ALL_CHA_DATA], SocketData->Route[TreeNode.Node.SocId].ChaKtiFanOut1[ALL_CHA_DATA]);

          for (Ctr = 0; Ctr < KtiInternalGlobal->KtiPortCnt; Ctr++) {
            if (KTIVAR->CpuInfo[(UINT8)TreeNode.Node.SocId].LepInfo[Ctr].Valid == TRUE) {
              ProgramM3KtiSnoopFanout ((UINT8) TreeNode.Node.SocId, Ctr, SocketData->Route[TreeNode.Node.SocId].M3KtiSnpFanOut[Ctr], NULL, ProgramPeMode);
            }
          }
        } //if (TreeNode.Node.SocType == SOCKET_TYPE_CPU)
      } //for (TreeIdx = 0; SbspTree[TreeIdx].Node.Valid == TRUE; TreeIdx++)
    } //if (KtiInternalGlobal->SnoopFanoutEn == TRUE)
  } //if ((KTIVAR->RasInProgress != TRUE) || (KTIVAR->ProgramBC == TRUE))

  return KTI_SUCCESS;
}

/**
  Programs hash mask to 4G as default if 2LM Mode on KTI and IIO according to MS2IDI

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Socket            - Socket
  @param Enable            - 2LM enabled or disabled

  @retval None

**/
VOID
Program2LMHashRegisterOnKtiAndIio (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Socket,
  BOOLEAN                    Enable
  )
{
  KTI_AGENT_2LM_ADDRESS_MASK_10nm         TwoLM_Address_Mask;
  UINT8                                   Ctr;

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nConfigure  Socket %d 2LM Hash on KTI and IIO", Socket));
  if (Enable == TRUE) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "-Enabled\n"));
    TwoLM_Address_Mask.Address_Mask_51_46 = 0x3f;
    TwoLM_Address_Mask.Address_Mask_45_40 = 0x3f;
    TwoLM_Address_Mask.Address_Mask_39_28 = 0xff0;           //Set TO 4G
  } else {                            //Clear to 0
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "-Disabled\n"));
    TwoLM_Address_Mask.Address_Mask_45_40 = 0;
    TwoLM_Address_Mask.Address_Mask_39_28 = 0;
    TwoLM_Address_Mask.Address_Mask_51_46 = 0;
  }

  //
  // KTI
  //
  for (Ctr = 0; Ctr < KtiInternalGlobal->KtiPortCnt; Ctr++) {
    if (KTIVAR->CpuInfo[Socket].LepInfo[Ctr].Valid == TRUE) {
      ProgramKtiAgent2LMAddrMask (Socket, Ctr, (UINT32 *) &TwoLM_Address_Mask);
    }
  }

  //
  // IIO : 10nm start from IIO stack 0
  //
  for (Ctr = 0; Ctr < MAX_IIO_STACK; Ctr++) {
    if (StackPresent (Socket, Ctr)) {
      IioSetSncBaseTwoLmMask (Socket,
                              Ctr,
                              TwoLM_Address_Mask.Address_Mask_45_40,
                              TwoLM_Address_Mask.Address_Mask_51_46,
                              TwoLM_Address_Mask.Address_Mask_39_28);
    }
  }
}

/**
  Programs hash mask to 4G as default if 2LM Mode

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Socket            - Socket
  @param Enable            - 2LM enabled or disabled

  @retval None

**/
VOID
Program2LMHashRegister (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Socket,
  BOOLEAN                    Enable
  )
{
  KTI_AGENT_2LM_ADDRESS_MASK_10nm         TwoLM_Address_Mask;

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nConfigure  Socket %d 2LM Hash ", Socket));
  if (Enable == TRUE) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "-Enabled\n"));
    TwoLM_Address_Mask.Address_Mask_51_46 = 0x3f;
    TwoLM_Address_Mask.Address_Mask_45_40 = 0x3f;
    TwoLM_Address_Mask.Address_Mask_39_28 = 0xff0;           //Set TO 4G
  } else {                            //Clear to 0
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "-Disabled\n"));
    TwoLM_Address_Mask.Address_Mask_45_40 = 0;
    TwoLM_Address_Mask.Address_Mask_39_28 = 0;
    TwoLM_Address_Mask.Address_Mask_51_46 = 0;
  }

  //
  // MS2IDI
  //
  Ms2IdiSetTwoLmMaskSncCfg (Socket, TwoLM_Address_Mask.Address_Mask_45_40,
                            TwoLM_Address_Mask.Address_Mask_51_46,
                            TwoLM_Address_Mask.Address_Mask_39_28);

  //
  // KTI and IIO
  //
  Program2LMHashRegisterOnKtiAndIio (KtiInternalGlobal, Socket, Enable);
}

/**
  Programs the CHA AtoS enable bit for all CHAs on all sockets

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval None
**/
VOID
SetAtoS(
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8                         Socket, Ctr;
  UINT8                         AtosStatus;
  UINT8                         AepPresent;
  BOOLEAN                       FpgaPresent;

  FpgaPresent = (KtiInternalGlobal->FpgaList != 0);
  if (KTIVAR->SysConfig == SYS_CONFIG_1S) {
    //AtoS has to be default if only one socket in the system
    return;
  }

  if (KTIVAR->OutStaleAtoSOptEn == KTI_AUTO) {
    UBoxCheckAepDimmPresentFlagSetInScratchpad (KtiInternalGlobal->SbspSoc, 0, &AepPresent);
    AtosStatus = AepPresent ?  KTI_ENABLE : KTI_DISABLE;
  } else {
    AtosStatus = KTIVAR->OutStaleAtoSOptEn;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent (Socket)) {
      //
      // StaleAtoS is programmed in a broadcast-CSR
      //
      ChaConfigureStaleAtoSOptEn (Socket, 0, AtosStatus);
      for (Ctr = 0; Ctr < KTIVAR->MaxSupportedM2m; Ctr++) {
        if ((KTIVAR->CpuInfo[Socket].M2mBitmap & (1 << Ctr)) == 0) {
          continue;
        }
        Set2SD2KEnable (Socket, Ctr, FpgaPresent, AtosStatus);
      }
    }
  }

}

/**
  Programs hash mask to 4G as default if 2LM Mode

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval UNCORE_SUCCESS
  @retval UNCORE_FAILURE

**/
KTI_STATUS
TwoLmHashSetting (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  KTI_STATUS     Status;
  UINT8          Socket;
  UINT8          VolMemMode;
  UINT8          CacheMemType;
  UINT16         DramType;
  BOOLEAN        CboTwoLMEnabled;
  MEM_NVRAM_STRUCT *nvramMem;
  SYS_SETUP      *Setup;
  UINT32         AddressMask_39_28;
  UINT32         AddressMask_45_40;
  UINT32         AddressMask_51_46;

  Setup      = GetSysSetupPointer ();
  VolMemMode = Setup->mem.volMemMode;
  CacheMemType = Setup->mem.CacheMemType;
  DramType   = GetDramType ();
  Status     = KTI_SUCCESS;
  nvramMem   = GetMemNvramPointer ();

  if (CheckCurrentReset (POST_RESET_POWERGOOD) && GetSysResetRequired () != 0) {
    //
    // cold reset path with a reset pending
    //
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (SocketPresent (Socket)) {
        BOOLEAN AepDimmPresent = CheckSocketDcpmmPresent (Socket);

        //
        //check if user setup is 2LM and PMEM present, or HBM is caching DDR
        //then set to 4G (PA32) for hash fuction in CHA/IIO/KTI
        //
        if (((VolMemMode == VOL_MEM_MODE_2LM) || (VolMemMode == VOL_MEM_MODE_MIX_1LM2LM)) &&
            (AepDimmPresent || ((KTIVAR->HbmSku == TRUE) && (CacheMemType == CACHE_TYPE_HBM_CACHE_DDR)))) {
          //
          // if 2LM is disabled in previous warm reset, avoid to enable the 2LM in cold boot of S3 resume
          //
          if (((GetSysBootMode () == S3Resume) && (nvramMem->volMemMode == VOL_MEM_MODE_1LM))) {
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nAvoid to enable 2LM in S3 resume\n"));
          } else {
            Program2LMHashRegister (KtiInternalGlobal, Socket, TRUE);
          }
        } // 2LM enabled
      } // Socket valid
    } // scan Socket
  } else {
    //
    // warm reset flow, or cold-reset flow with warm-reset elimination
    //

    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (SocketPresent (Socket)) {
        if (UbiosGenerationEnabled ()) {
          ChaCheck2LMEnabled (Socket, 0, &CboTwoLMEnabled);
          if (CboTwoLMEnabled) {
            //
            // Only program the KTI and IIO 2LM Settings when 2LM enabled.
            //
            Program2LMHashRegisterOnKtiAndIio (KtiInternalGlobal, Socket, TRUE);
          }
        } else {
          //
          // Compare between 2LM enabled bit (is set by warm reset MRC flow) and 2LM hash register (is set by this routine during cold reset)
          //
          ChaCheck2LMEnabled (Socket, 0, &CboTwoLMEnabled);
          Ms2IdiGetTwoLmMaskSncCfg (Socket, &AddressMask_45_40, &AddressMask_51_46, &AddressMask_39_28);
          // Two cases Need to be re-program 2LM hashing setting based on MRC 2LM setting and issues a warm reset
          // Case 1: MRC sets to 1LM during warm reset but this routine set to 2LM during cold reset prediction
          if ((CboTwoLMEnabled == FALSE) && (AddressMask_39_28 != 0)) {
            Program2LMHashRegister (KtiInternalGlobal, Socket, FALSE);
            SetNewResetRequired (POST_RESET_WARM);
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n 2LM is %a in Warm Reset but it is %a in cold boot\n", "disabled", "enabled"));
          // Case 2: MRC sets to 2LM during warm reset but this routine set to 1LM during cold reset prediction (but this case, it seems impossible)
          } else if ((CboTwoLMEnabled == TRUE) && (AddressMask_39_28 == 0)) {
            Program2LMHashRegister (KtiInternalGlobal, Socket, TRUE);
            SetNewResetRequired (POST_RESET_WARM);
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n 2LM is %a in Warm Reset but it is %a in cold boot\n", "enabled", "disabled"));
          //
          // Case 3: MRC set to 2LM during warm reset, and MS2IDI's record is also 2LM; then need to update the reigsters
          //         in IIO and KTI
          //
          } else if ((CboTwoLMEnabled == TRUE) && (AddressMask_39_28 != 0)) {
            Program2LMHashRegisterOnKtiAndIio (KtiInternalGlobal, Socket, TRUE);
          }
        }
      } // socket valid
    } // scan socket

    //
    // Disable AtoS depending on 1LM/2LM configuration
    //
    SetAtoS (KtiInternalGlobal);
  } // warm reset

  return Status;
}

/**

  Enable/Disable Error injection bits on each MC

  @param  KtiInternalGlobal -  KTIRC Internal Global data

  @retval N/A

**/
VOID
UncoreSetErrInjVal (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  MEM_TECH_TYPE   McTechType;
  UINT8           Socket;
  UINT8           Idx;
  UINT8           McIdx;

  //
  // MCDECS
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!SocketPresent (Socket)) {
      continue;
    }
    for (Idx = 0; Idx < KTIVAR->MaxSupportedM2m; Idx++) {
      if ((KTIVAR->CpuInfo[Socket].M2mBitmap & (1 << Idx)) == 0) {
        continue;
      }
      //
      // 5372503: CLONE SKX Sighting: Unexpected failover seen with UC mirror scrub injection.
      // H0 stepping SKX workaround; Each MC (M2mem) has 3 McDecs
      // The SKX W/A has been adjusted for 10nm compatibility.
      //
      if (Idx < MAX_SPR_M2MEM) {
        McTechType = MemTechDdr;
        McIdx = Idx;
      } else {
        McTechType = MemTechHbm;
        McIdx = Idx - MAX_SPR_M2MEM;
      }

      SetErrInjValPerMc (McTechType, Socket, McIdx, (KTISETUP->DirectoryModeEn == KTI_ENABLE));
    }
  }
}

/**
  Enable Mesh Mode

  This routine performs the necessary setup for mesh mode and send Mesh Mode
  mailbox command to Pcode.

  @param  None

  @retval None

**/
VOID
UncoreEnableMeshMode (
  VOID
  )
{
  MEM_INFO                  MemInfo;
  KTI_STATUS                Status = KTI_SUCCESS;
  KTI_HOST_INTERNAL_GLOBAL  KtiInternalGlobal;
  BOOLEAN                   IsPostResetPowerGood;
  UINT8                     Socket;
  UINT8                     UbcDeterminedByMemoryMap;
  BOOLEAN                   SystemResetRequired;
  BOOLEAN                   Mem2LMEnabled = FALSE;
  UINT8                     Mem2LMFlag;

  Mem2LMFlag = 0;
  UbcDeterminedByMemoryMap = GetUmaBasedClustering ();
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Configure Mesh Mode - %a *******\n", "\n\n", "START"));

  ZeroMem ((UINT8 *) &KtiInternalGlobal, sizeof (KTI_HOST_INTERNAL_GLOBAL));

  //
  // Retrieve setup, var and nvram pointers
  //
  KtiInternalGlobal.KtiSetup = (UINT32) (UINTN)KTI_HOST_IN_PTR;
  KtiInternalGlobal.KtiVar = (UINT32) (UINTN)KTI_HOST_OUT_PTR;
  KtiInternalGlobal.KtiNvram = (UINT32) (UINTN) KTI_HOST_NVRAM_PTR;
  KtiInternalGlobal.KtiPortCnt = GetKtiPortCnt ();
  KtiInternalGlobal.SbspSoc =  GetSysSbspSocketIdNv ();

  //
  // Initialize CpuType in KtiInternalGlobal
  //
  KtiGetCpuTypeInfo (&KtiInternalGlobal);

  //
  // Get the number of KtiAgents present
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent (Socket)) {
      GetKtiAgentCount (&KtiInternalGlobal, Socket, &(KtiInternalGlobal.TotKtiAgent[Socket]));
      ChaCheck2LMEnabled (Socket, 0, &Mem2LMEnabled);  // a socket is in 2LM Mode
      Mem2LMFlag |= (Mem2LMEnabled << Socket);         // Mem2LMFlag indicates the 2LM flag info for each socket.
    }
  }

  //
  // Update IsocEn setting based on AEP DIMM presence. This has to be done before ChaToM2M credit programming.
  //
  UpdateIsocEnSetting (&KtiInternalGlobal);

  //
  // setup 2LM hasting setting
  //
  TwoLmHashSetting (&KtiInternalGlobal);
  IsPostResetPowerGood = CheckCurrentReset (POST_RESET_POWERGOOD);
  SystemResetRequired = GetSysResetRequired ();

  //
  // Program MS2IOSF Credit settings which depend on DIMM Configuration
  //
  if (IsPostResetPowerGood) {
    UpdateM2PcieCreditSetting (&KtiInternalGlobal);
  }


  if (SystemResetRequired == 0) {
    //
    // MSM is connected to one of the M2IOSF channel. MSM registers cannot be accessed untill M2IOSF bank decoder has been enabled in IioEarlyLinkInit.
    // It is safe now to update CpuBusNo and MCTP Eid in MSM.since we have already passed that point.
    //
    UpdateCpuBusAndMctpEidOnMsm (&KtiInternalGlobal);
    UncoreSetErrInjVal (&KtiInternalGlobal);
  }

  //
  // Set the flag in scratchpad to indicate the populated memory requires a degraded UMA Clustering mode.
  // If degraded UMA clustering, then a warm reset is needed.
  //
  if ((UbcDeterminedByMemoryMap == UMABASEDCLUSTERING_HEMISPHERE) &&
      (((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutUmaClustering == UMABASEDCLUSTERING_QUADRANT)) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nUMA based clustering downgrade from Quad to Hemi mode!\n"));
    UBoxSetUmaBasedClusteringDowngradeInScratchpad (GetSysSbspSocketIdNv (), 0, QUAD2HEMI);
    SetNewResetRequired (POST_RESET_WARM);
  } else if ((UbcDeterminedByMemoryMap == UMABASEDCLUSTERING_DISABLED) &&
      (((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutUmaClustering == UMABASEDCLUSTERING_QUADRANT)) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nUMA based clustering downgrade from Quad to Disabled mode!\n"));
    UBoxSetUmaBasedClusteringDowngradeInScratchpad (GetSysSbspSocketIdNv (), 0, QUAD2DISABLE);
    SetNewResetRequired (POST_RESET_WARM);
  } else if ((UbcDeterminedByMemoryMap == UMABASEDCLUSTERING_DISABLED) &&
      (((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutUmaClustering == UMABASEDCLUSTERING_HEMISPHERE)) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nUMA based clustering downgrade from Hemi to Disabled mode!\n"));
    UBoxSetUmaBasedClusteringDowngradeInScratchpad (GetSysSbspSocketIdNv (), 0, HEMI2DISABLE);
    SetNewResetRequired (POST_RESET_WARM);
  }

  // MRC code may downgrade UMA based clustering, here using value set by MRC code.
  if (((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutUmaClustering > 0) {
    if (((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutUmaClustering != UbcDeterminedByMemoryMap) {
      ((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->UmaClusteringDowngrade = TRUE;
    }
    ((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutUmaClustering = UbcDeterminedByMemoryMap;
  }

  //
  // Verify memory population for SNC configuration
  //
  ZeroMem ((UINT8 *)&MemInfo, sizeof (MemInfo));
  Status = CheckMemoryPopulation (&KtiInternalGlobal, &MemInfo);

  if (((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutSncPrefetchEn) {
    if (Status == KTI_FAILURE) {    // Update MemInfo Data Structure
      if (IsPostResetPowerGood && (SystemResetRequired != 0)) {
        //
        // Normal powergood path with warm-reset request pending
        //
        if (((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutSncEn != KTI_DISABLE) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nRequest disable SNC and issue WARM RESET!"));
          DisabledSncFlag (&KtiInternalGlobal);
        }
        if (((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutUmaClustering == UMABASEDCLUSTERING_HEMISPHERE) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nUMA based clustering downgrade from Hemi to Disabled mode!\n"));
          UBoxSetUmaBasedClusteringDowngradeInScratchpad (GetSysSbspSocketIdNv (), 0, HEMI2DISABLE);
          ((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutUmaClustering = UMABASEDCLUSTERING_DISABLED;
        }
        if (((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutUmaClustering == UMABASEDCLUSTERING_QUADRANT) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nUMA based clustering downgrade from Quad to Disabled mode!\n"));
          UBoxSetUmaBasedClusteringDowngradeInScratchpad (GetSysSbspSocketIdNv (), 0, QUAD2DISABLE);
          ((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutUmaClustering = UMABASEDCLUSTERING_DISABLED;
        }
        //
        // OutUmaClustering might be changed in the above flow, so update OutSncPretechEn here.
        //
        ((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutSncPrefetchEn = ((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutUmaClustering |
                                                                                ((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutSncEn |
                                                                                ((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutXptPrefetch |
                                                                                ((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutKtiPrefetch |
                                                                                ((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutXptRemotePrefetch;
        //
        // still need to program some Snc/Prefetch registers in cold reset flow if Prefetch is enabled
        //
        if (((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutSncPrefetchEn) {
          EnableSncMode (&KtiInternalGlobal, &MemInfo);
        }
      }
    } else {
      EnableSncMode (&KtiInternalGlobal, &MemInfo);
    }
  } else {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (SocketPresent (Socket)) {
        DisableAllPrefetch (&KtiInternalGlobal, Socket);
        ResetAllSncBase (&KtiInternalGlobal, Socket, &MemInfo);
      }
    }
  }

  if (SystemResetRequired == 0) {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (SocketPresent (Socket)) {
        //
        // Program performance monitor discovery state table in warm reset path
        //
        ProgramPmonDiscoveryTableOnOob (&KtiInternalGlobal, Socket);

        //
        // Sierra Peak needs SNC/UMA register programming
        //
        if (IsSpkSupported (&KtiInternalGlobal, Socket)) {
          ProgramSncRegistersOnSpk (&KtiInternalGlobal, Socket);
        }

        //
        // Initialize SNC related registers for CXL
        //
        ProgramSncRegistersOnCxl (&KtiInternalGlobal, Socket);
      }
    }
  }

  if (SystemResetRequired == 0) {
    DumpSncXptKtiRegister (&KtiInternalGlobal);
  }

  if (!(((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->OutSncPrefetchEn && (GetNumaEn () == KTI_ENABLE))) {
    if (SystemResetRequired == 0) {
      //
      // For KTI OL flow or 2LM mode detected, it needs to reprogram the CHA to M2M credit
      //
      if (((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->RasInProgress ||
          (Mem2LMFlag != 0) ||
          ((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->WarmResetEliminationEn) {
        ProgramNonSncChaToM2mCredit (&KtiInternalGlobal, &MemInfo);
      }
    } else {
      ProgramNonSncChaToM2mCredit (&KtiInternalGlobal, &MemInfo);
    }
  }

  if (SystemResetRequired == 0) {
    //
    // program m2mem ad/bl and XptPrefetch credits
    //
    // Credit Type 0xc and 0xd : per Mc
      ProgramM2mTransgressCredits (&KtiInternalGlobal, &MemInfo);
      if (((KTI_HOST_OUT *) (UINTN) KtiInternalGlobal.KtiVar)->HbmSku == FALSE) {
        // Credit Type 0xb : per Mc
        ProgramM2mPrefetchCredit (&KtiInternalGlobal, &MemInfo);
      }
    //
    // Set DDRT QoS Cfg
    //
    SetDDRTQosCfg (&KtiInternalGlobal);

    //
    // Program cbo_coh_config1_cfg based on CHA performance register request
    //
    ProgramSystemCoherencyConfig (&KtiInternalGlobal);

    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (SocketPresent (Socket)) {
        //
        // Program all the PAM region to access to DRAM for legacy BIOS free.
        //
        ProgramAllPamAccessToDram (Socket);
        //
        // Program Platform QoS Memory BW Enforcement
        //
        UBoxSetMemoryBWEnforcement(Socket, 0);
      }
    }

    SetDmiCfgType0 (&KtiInternalGlobal);
  }

  //
  // Send Mesh Mode mailbox command to Pcode
  // It should be called in Cold and Warm reset.
  // This is required by SNC/Prefetch/2LM Hash
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nProgramSncShadowReg\n"));
  ProgramSncShadowReg (&KtiInternalGlobal);

  KtiUpdateXptKtiPrefetchStatus (&KtiInternalGlobal);

  //
  // Unmask the cbo prq credit overflow at the end of KtiRc, if there is no reset anymore
  //
  if (SystemResetRequired == 0) {
    MCAErrWa (FALSE);
  }

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%a******* Configure Mesh Mode - %a *******\n", "\n", "END"));
  KtiCheckPoint (0xFF, 0xFF, 0xFF, STS_KTI_COMPLETE, MINOR_STS_UNCORE_MESH_MODE_COMPLETE);
}

/**
  Dump current bus assignments

  @param KtiInternalGlobal   - KTIRC Internal Global data
  @param SocketData          - Pointer to socket specific data
  @param PrintMissingSockets - TRUE/FALSE

  @retval None

**/
VOID
DumpCurrentBusInfo (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN KTI_SOCKET_DATA            *SocketData,
  IN BOOLEAN                    PrintMissingSockets
    )
{
  UINT8 Soc, Stack;
  UINT8 StackPersonality;

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nCurrent bus numbers:"));
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket | Stack0 | Stack1 | Stack2 | Stack3 | Stack4 | Stack5 | Stack6 | Stack7 | Stack8 | Stack9 | StackA | StackB |  Rsvd  | Ubox0  | Ubox1  | LastBus | Seg "));
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO1, "\n---------------------------------------------------------------------------------------------------------------------------------------------------------------%a", ""));

  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {
    if (PrintMissingSockets || ((SocketData->Cpu[Soc].Valid == TRUE) && (SocketData->Cpu[Soc].SocType == SOCKET_TYPE_CPU))) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  %u    | ", Soc));

      if (GetStackPresentBitMap (Soc) != 0) {
        //
        // StackPresentBitmap has been initialized, print the buses assigned to each valid stack
        //
        for (Stack = 0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
          StackPersonality = KTIVAR->CpuInfo[Soc].CpuRes.StackRes[Stack].Personality;
          if ((StackPersonality == TYPE_UBOX_IIO)
            || (StackPersonality == TYPE_HFI)
            || (StackPersonality == TYPE_NAC)
            || (StackPersonality == TYPE_DINO))
          {
            KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, " 0x%02X  | ", GetStackBus (Soc, Stack)));
          } else if ((StackPersonality == TYPE_NONE) || (StackPersonality == TYPE_RESERVED)) {
            KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, " %a  | ", "----"));
          } else if (StackPersonality == TYPE_UBOX) {
            KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, " 0x%02X  |  0x%02X  | ", GetSocketLastBus (Soc) - 1, GetSocketLastBus (Soc)));
          }
        }
      } else {
        //
        // StackPresentBitmap has not been initialized yet, p early bus number assignments
        //
        for (Stack = 0; Stack < RSVD_STACK; Stack++) {
          if (PrintMissingSockets || StackPresent (Soc, Stack)) {
            KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, " 0x%02X  | ", GetStackBus (Soc, Stack)));
          } else {
            KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "  %a  | ", "---"));
          }
        }
        //
        // Print buses reserved for RSVD stack, Ubox0, and Ubox1, for each socket
        //
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, " ----  |  0x%02X  |  0x%02X  | ", GetSocketLastBus (Soc) - 1, GetSocketLastBus (Soc)));
      }

      //
      //Print maximum bus number, and segment number
      //
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "  0x%02X  |  %1u", GetSocketLastBus (Soc), KTI_HOST_OUT_PTR->segmentSocket[Soc]));
    }
  }
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO1, "\n---------------------------------------------------------------------------------------------------------------------------------------------------------------%a", "\n"));

  return;
}

/**
  Prime the KTIRC stackres data structure based on input option and system topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
PrimeHostStructureStack (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8 Socket;
  UINT8 Stack;
  UINT8 TotTargetCount;
  UINT8 *StackPersonalities;
  UINT8 CpuType;

  //
  // Get the appropriate Stack Personality Array pointer for this CPU type
  //
  CpuType  = KtiInternalGlobal->CpuType;

  switch (CpuType) {
    case CPU_SPRSP:
      StackPersonalities = &SprSpStackPersonalities[0];
      break;
    default:
      StackPersonalities = &SprSpStackPersonalities[0];
      break;
  }

  //
  // Update StackRes info with type of stack that is present and count number of SAD targets
  //
  TotTargetCount = 0;
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (KtiInternalGlobal->CpuList & (1 << Socket)) {
      //
      // DMI/CStack is always present
      //
      SetStackPresent (Socket, IIO_CSTACK);
      KTIVAR->CpuInfo[Socket].CpuRes.TotEnabledStacks = 0;

      for (Stack = 0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
        if (StackPresent (Socket, Stack)) {
          //
          // Physical IIO Stack is present, assign personality from respective StackPersonality array
          //
          KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].Personality = StackPersonalities[Stack];
          TotTargetCount += 1;
          KTIVAR->CpuInfo[Socket].CpuRes.TotEnabledStacks += 1;
        } else {
          //
          //Stack not present, assign type NONE
          //
          KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].Personality = TYPE_NONE;
        }
      }
      //
      //Assign personalities to 'logical' stacks
      //
      KTIVAR->CpuInfo[Socket].CpuRes.StackRes[UBOX_STACK].Personality = TYPE_UBOX;
      KTIVAR->CpuInfo[Socket].CpuRes.StackRes[RSVD_STACK].Personality = TYPE_RESERVED;
    }

    if (SocketData->Cpu[Socket].SocType == SOCKET_TYPE_FPGA) {
      //
      // FPGA only have 1 enabled stack
      //
      SetStackPresent (Socket, IIO_CSTACK);
      KTIVAR->CpuInfo[Socket].CpuRes.stackPresentBitmap = GetStackPresentBitMap (Socket);
      KTIVAR->CpuInfo[Socket].CpuRes.TotEnabledStacks = 1;
    }
  }

  KtiInternalGlobal->TotSadTargets = TotTargetCount;

  return KTI_SUCCESS;
}

/**
  Setup MMIOL for the cold-reset path.

  This routine configures MMIOL during the cold-reset path. It configures UBOX MMIO Base
  which is required for CSRs access in UPI and MRC.

  @param[in] SocketData           Pointer to socket specific data
  @param[in] KtiInternalGlobal    KTIRC Internal Global data

  @retval KTI_SUCCESS             Successful

**/
KTI_STATUS
EarlyIoSad (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8                 Index;

  KtiCheckPoint (0xFF, 0xFF, 0xFF, STS_PROGRAM_FINAL_IO_SAD, MINOR_STS_EARLY_IO_SAD);
  //
  // Setup MMIOL only during powergood. It is mainly ubox mmio base setup for cold reset MRC CSR access
  //
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if ((SocketData->Cpu[Index].Valid == TRUE) && (SocketData->Cpu[Index].SocType == SOCKET_TYPE_CPU)) {
      ProgramCpuMmiolEntries (SocketData, KtiInternalGlobal, Index);
    }
  }

  //
  // Dump ubox mmio bar assignment
  //
  DumpCurrentBarInfo (SocketData, FALSE);

  return KTI_SUCCESS;
}

/**
  Setup all IO SAD and address map for all sockets.

  This routine is called during the warm-reset path or cold-reset path with warm-reset
  skipped. It configures all IO SAD and Address Map related registers for all sockets/
  boxes.

  @param[in] SocketData           Pointer to socket specific data
  @param[in] KtiInternalGlobal    KTIRC Internal Global data

  @retval KTI_SUCCESS             Successful

**/
KTI_STATUS
SetupSystemIoSadEntries (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8                 Index;

  KtiCheckPoint (0xFF, 0xFF, 0xFF, STS_PROGRAM_FINAL_IO_SAD, MINOR_STS_FILL_SAD_TGTLST);
  //
  // Setup all the IO SAD & Address Map related registers for all sockets/boxes
  //
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if ((SocketData->Cpu[Index].Valid == TRUE) && (SocketData->Cpu[Index].SocType == SOCKET_TYPE_CPU)) {
      KtiCheckPoint ((UINT8)(1 << Index), SOCKET_TYPE_CPU, 0xFF, STS_PROGRAM_FINAL_IO_SAD, MINOR_STS_PROGRAM_CPU_SAD_ENTRIES);
      ProgramCpuIoEntries (SocketData, KtiInternalGlobal, Index);
      ProgramCpuIoApicEntries (SocketData, KtiInternalGlobal, Index);
      ProgramCpuMmiolEntries (SocketData, KtiInternalGlobal, Index);
      ProgramCpuMmiohEntries (SocketData, KtiInternalGlobal, Index);
      ProgramCpuMiscSadEntries (SocketData, KtiInternalGlobal, Index);
      ProgramSystemAddressMap (SocketData, KtiInternalGlobal, Index);
    }
  }

  //
  // Dump ubox mmio bar assignment
  //
  DumpCurrentBarInfo (SocketData, FALSE);

  ProgramMctpBroadcastSettings (SocketData, KtiInternalGlobal);

  return KTI_SUCCESS;
}

/**
  Routine to program the final Mmmiol rules for given socket on 10nm projects

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuMmiolEntries (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId
  )
{
  UINT32    UboxMmioBase, UboxMmioLimit;
  UINT32    TmpMmioNemRuleBase;
  UINT8     Stack, Socket, TargetIndex = 0, TmpTarget;
  UINT8     TmpTotCpu;

  //
  // Mmio11 is used to cover NEM/legacy Mmio region at 4G-64M to 4G region.
  // Up to 6 rules are used in < 8S for local socket Mmiol to route to each possible IIO stack
  // Up to 4 rules are used in 8S capable parts for local socket Mmiol to route to each possible IIO stack
  // Rest of rules are used to route off socket to other possible 7 cpus
  //

  if (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE) {
    if (KTISETUP->KtiCpuSktHotPlugTopology == 0) {
      TmpTotCpu = 4;         //online/offline based on 4S topology
    } else {
      TmpTotCpu = 8;         //online/offline based on 8S topology
    }
  } else {
    TmpTotCpu = MAX_SOCKET;
  }

  KTI_ASSERT ((TmpTotCpu <= MAX_SOCKET), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_77);

  for (Socket = 0; Socket < TmpTotCpu; Socket++) {
    //
    // Socket present?
    //
    if ((SocketData->Cpu[Socket].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) {
      if (Socket == SocId) {
        for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
          //
          // Program a local rule for each enabled stack
          //
          if (StackPresent (Socket, Stack)) {
            if (KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolBase < KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolLimit) {
              TmpTarget = (Stack | SOCKET_LOCAL_5BITS_NID_MSB);
              ChaConfigureMmiolRule (SocId,
                                     0,
                                     TargetIndex,
                                     1,
                                     KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolBase,
                                     KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolLimit,
                                     TmpTarget);
              TargetIndex++;
            }
          }
        }
      } else {
        //
        // Program off socket rule
        //
        // MMIOL Rule
        //
        if (KTIVAR->CpuInfo[Socket].CpuRes.MmiolBase < KTIVAR->CpuInfo[Socket].CpuRes.MmiolLimit) {
          if (SocketData->Cpu[Socket].Valid == FALSE) {
            TmpTarget = SOCKET_LOCAL_5BITS_NID_MSB;
          } else {
            TmpTarget = Socket;
          }
          ChaConfigureMmiolRule (SocId,
                                 0,
                                 TargetIndex,
                                 1,
                                 KTIVAR->CpuInfo[Socket].CpuRes.MmiolBase,
                                 KTIVAR->CpuInfo[Socket].CpuRes.MmiolLimit,
                                 TmpTarget);
          TargetIndex++;
        }
      }
    }
  }
  if (KtiInternalGlobal->IsMmcfgAboveMmiol == TRUE) {
    //
    // Handle nonval case where OEM attempts to put mmiol below mmcfg.
    //
    TmpMmioNemRuleBase = KtiInternalGlobal->MmcfgBase + KTIVAR->mmCfgSize + 1;
    ChaConfigureNemRuleBase (SocId, 0, TmpMmioNemRuleBase);
  }

  //
  // Always point rule19 to legacy cstack
  //
  if (SocId != KtiInternalGlobal->SbspSoc) {
    ChaConfigureMmiolRule (SocId, 0, 19, 1, 0, 0, KtiInternalGlobal->SbspSoc);
  }

  //
  // Program UBOX mmio base, each MMIO bar and CHA mmio Rule 12
  //
  UboxMmioBase = KTIVAR->CpuInfo[SocId].CpuRes.UboxMmioBase;
  UboxMmioLimit = UboxMmioBase + UBOX_MMIO_BAR_SIZE - 1;

  //
  //Program CHA mmio Rule 12 for ubox mmio bars
  //
  ChaConfigureUboxMmio (SocId, 0, UboxMmioBase, UboxMmioLimit);

  //
  // Program ubox mmio base & each UBOX MMIO bar
  //
  UBoxConfigureUboxMmio (SocId, UboxMmioBase);

  ProgramCpuMmiolEntriesIio (SocketData, KtiInternalGlobal, SocId);
  ProgramCpuMmiolEntriesKti (SocketData, KtiInternalGlobal, SocId);

  return KTI_SUCCESS;
}

/**
  Routine to program the final Mmmiol rules for given socket

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0 - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuMmiolEntriesIio (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  )
{
  UINT32 Rule[TOTAL_MMIO_VTD_PER_STACK_SPR];
  UINT32 MmiolTgtLst[TOTAL_MMIO_VTD_PER_STACK_SPR];
  UINT8  Stack, Socket, TargetIndex = 0;
  UINT32 TmpTarget;
  UINT8  MaxSocket;
  UINT8  SerialValue;
  UINT8  RemoteStackId;

  ZeroMem ((UINT8 *) Rule, (sizeof (UINT32) * TOTAL_MMIO_VTD_PER_STACK_SPR));
  ZeroMem (MmiolTgtLst, sizeof (UINT32) * TOTAL_MMIO_VTD_PER_STACK_SPR);

  if (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE) {
    if (KTISETUP->KtiCpuSktHotPlugTopology == 0) {
      MaxSocket =  4;         //online/offline based on 4S topology
    } else {
      MaxSocket =  8;         //online/offline based on 8S topology
    }
  } else {
    MaxSocket =  MAX_SOCKET;
  }

  KTI_ASSERT ((MaxSocket <= MAX_SOCKET), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_78);


  //
  // Mmio11 is used to cover NEM/legacy Mmio region at 4G-64M to 4G region.
  // Up to 6 rules are used in < 8S for local socket Mmiol to route to each possible IIO stack
  // Up to 4 rules are used in 8S capable parts for local socket Mmiol to route to each possible IIO stack
  // Rest of rules are used to route off socket to other possible 7 cpus
  //
  for (Socket = 0; Socket < MaxSocket; Socket++) {
    //
    // Socket present?
    //
    if ((SocketData->Cpu[Socket].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) {
      if (Socket == SocId) {
        for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
          //
          // Program a local rule for each enabled stack
          //
          if (StackPresent (Socket, Stack)) {
            if (KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolBase < KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolLimit) {
              SerialValue = 1;
              RemoteStackId = 0;
              IioNewMmiolRule (SocId, 1, SerialValue,
                               KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolBase,
                               KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolLimit,
                               RemoteStackId,
                               &Rule[TargetIndex]);
              TmpTarget = (Stack | SOCKET_LOCAL_FLAG);
              MmiolTgtLst[TargetIndex] = TmpTarget;
              TargetIndex++;
            }
          }
        }
      } else {
        if ((KTIVAR->SysConfig < SYS_CONFIG_4S) && (KTISETUP->P2pRelaxedOrdering == KTI_DISABLE)) {
          //
          // Program off socket rule
          //
          // MMIOL Rule
          //
          for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
            if (StackPresent (SocId, Stack)) {
              if (KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolBase < KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolLimit) {
                SerialValue = 1;
                RemoteStackId = Stack;
                IioNewMmiolRule (SocId, 1, SerialValue,
                                 KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolBase,
                                 KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiolLimit,
                                 RemoteStackId,
                                 &Rule[TargetIndex]);
                if (SocketData->Cpu[Socket].Valid == TRUE) {
                  TmpTarget = Socket | SOCKET_REMOTE_FLAG;
                } else {
                  //
                  // Program to local DMI for hotplug reservation
                  //
                  TmpTarget = 0;
                }
                MmiolTgtLst[TargetIndex] = TmpTarget;
                TargetIndex++;
              }
            }
          }
        } else { //if ((KTIVAR->SysConfig < SYS_CONFIG_4S) && (KTISETUP->P2pRelaxedOrdering == KTI_DISABLE)) {
          //
          // Program off socket rule
          //
          // MMIOL Rule
          //
          if (KTIVAR->CpuInfo[Socket].CpuRes.MmiolBase < KTIVAR->CpuInfo[Socket].CpuRes.MmiolLimit) {
            SerialValue = (KTISETUP->P2pRelaxedOrdering == KTI_ENABLE) ? 1 : 2;
            RemoteStackId = 0;
            IioNewMmiolRule (SocId, 1, SerialValue,
                             KTIVAR->CpuInfo[Socket].CpuRes.MmiolBase,
                             KTIVAR->CpuInfo[Socket].CpuRes.MmiolLimit,
                             RemoteStackId,
                             &Rule[TargetIndex]);
            if (SocketData->Cpu[Socket].Valid == TRUE) {
              TmpTarget = Socket | SOCKET_REMOTE_FLAG;
            } else {
              //
              // Program to local DMI for hotplug reservation
              //
              TmpTarget = 0;
            }
            MmiolTgtLst[TargetIndex] = TmpTarget;
            TargetIndex++;
          }
        }
      }
    }
  }

  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    if (StackPresent (SocId, Stack)) {
      //
      // Program the MmioRules
      //
      for (TargetIndex = 0; TargetIndex < TOTAL_MMIO_VTD_PER_STACK_SPR; TargetIndex++) {
        IioSetMmiolRuleIdx (SocId, Stack, TargetIndex, Rule[TargetIndex]);
      }
      //
      // Program the MMIO_TARGET list for this socket
      //
      IioSetMmiolListIdx (SocId, Stack, TOTAL_MMIO_VTD_PER_STACK_SPR, MmiolTgtLst);

      //
      // Mmiol granularity.
      // lsb of address check for serialize rule 2 is mmiol_granularity + 22.
      // So when allocating low MMIO with a minimum granularity of 4MB per IIO stack program to 0, for 8MB program to 1, etc.
      //
      IioSetMmiolGranularity (SocId, Stack, KtiInternalGlobal->MmiolGranularity);
    }
  }

  return KTI_SUCCESS;
}

/**
  Routine to program the final IO SAD setting for the given topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuMmiohEntriesIio (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId
  )
{
  UINT8         Stack, Socket, MaxSocket, TargetIndex = 0;
  UINT64_STRUCT MmiohBaseTmp, MmiohLimitTmp;
  UINT64_STRUCT MmiohBase[TOTAL_MMIO_VTD_PER_STACK_SPR];
  UINT64_STRUCT MmiohLimit[TOTAL_MMIO_VTD_PER_STACK_SPR];
  UINT8         MmiohTarget[TOTAL_MMIO_VTD_PER_STACK_SPR];
  UINT8         SerialValueMmioh[TOTAL_MMIO_VTD_PER_STACK_SPR];
  UINT8         RemoteStackIdMmioh[TOTAL_MMIO_VTD_PER_STACK_SPR];

  SetMem (MmiohBase, sizeof (MmiohBase[0]) * TOTAL_MMIO_VTD_PER_STACK_SPR, 0xFF);
  SetMem (MmiohLimit, sizeof (MmiohLimit[0]) * TOTAL_MMIO_VTD_PER_STACK_SPR, 0x00);
  SetMem (MmiohTarget, sizeof (MmiohTarget[0]) * TOTAL_MMIO_VTD_PER_STACK_SPR, 0x00);
  SetMem (SerialValueMmioh, sizeof (SerialValueMmioh[0]) * TOTAL_MMIO_VTD_PER_STACK_SPR, 0x00);
  SetMem (RemoteStackIdMmioh, sizeof (RemoteStackIdMmioh[0]) * TOTAL_MMIO_VTD_PER_STACK_SPR, 0x00);

  if (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE) {
    if (KTISETUP->KtiCpuSktHotPlugTopology == 0) {
      MaxSocket = 4;         //online/offline based on 4S topology
    } else {
      MaxSocket = 8;         //online/offline based on 8S topology
    }
  } else {
    MaxSocket = MAX_SOCKET;
  }

  KTI_ASSERT ((MaxSocket <= MAX_SOCKET), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_7A);

  for (Socket = 0; Socket < MaxSocket; Socket++) {
    //
    // Socket present?
    //
    if ((SocketData->Cpu[Socket].Valid == TRUE) || (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE)) {
      if (Socket == SocId) {
        for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
          //
          // Program a local rule for each enabled stack
          //
          if (StackPresent (Socket, Stack)) {
            MmiohBaseTmp = KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiohBase;
            if ((MmiohBaseTmp.hi != 0xFFFFFFFF) && (MmiohBaseTmp.lo != 0xFFFFFFFF)) {
              MmiohLimitTmp = KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiohLimit;
              if ((MmiohBaseTmp.hi < MmiohLimitTmp.hi) || ((MmiohBaseTmp.hi == MmiohLimitTmp.hi) && (MmiohBaseTmp.lo < MmiohLimitTmp.lo))) {
                MmiohBase[TargetIndex] = MmiohBaseTmp;
                MmiohLimit[TargetIndex] = MmiohLimitTmp;
                MmiohTarget[TargetIndex] = Stack | SOCKET_LOCAL_5BITS_NID_MSB;
                SerialValueMmioh[TargetIndex] = 1;
                RemoteStackIdMmioh[TargetIndex] = 0;
                TargetIndex++;
              }
            }
          } //if (StackPresent..
        } //for (Stack = 0; Stack < MAX_IIO_STACK...
      } else {
        if ((KTIVAR->SysConfig < SYS_CONFIG_4S) && (KTISETUP->P2pRelaxedOrdering == KTI_DISABLE)) {
          for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
            //
            // Program a local rule for each enabled stack
            //
            if (StackPresent (Socket, Stack)) {
              //
              // Program off socket rule
              //
              // MMIOH Rule
              //
              MmiohBaseTmp = KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiohBase;
              //
              // Don't process if socket starved
              //
              if ((MmiohBaseTmp.hi != 0xFFFFFFFF) && (MmiohBaseTmp.lo != 0xFFFFFFFF)) {
                MmiohLimitTmp = KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].MmiohLimit;
                if ((MmiohBaseTmp.hi < MmiohLimitTmp.hi) || ((MmiohBaseTmp.hi == MmiohLimitTmp.hi) && (MmiohBaseTmp.lo < MmiohLimitTmp.lo))) {
                  MmiohBase[TargetIndex] = MmiohBaseTmp;
                  MmiohLimit[TargetIndex] = MmiohLimitTmp;
                  MmiohTarget[TargetIndex] = Socket;
                  SerialValueMmioh[TargetIndex] = 1;
                  RemoteStackIdMmioh[TargetIndex] = Stack;
                  TargetIndex++;
                }
              } //if ((MmiohBaseTmp.hi != 0xFFFFFFFF)...
            } // if (StackPresent (Socket, Stack))
          }
        } else { //((KTIVAR->SysConfig >= SYS_CONFIG_4S) || (KTISETUP->P2pRelaxedOrdering == KTI_ENABLE))
          //
          // Program off socket rule
          //
          // MMIOH Rule
          //
          MmiohBaseTmp = KTIVAR->CpuInfo[Socket].CpuRes.MmiohBase;
          //
          // Don't process if socket starved
          //
          if ((MmiohBaseTmp.hi != 0xFFFFFFFF) && (MmiohBaseTmp.lo != 0xFFFFFFFF)) {
            MmiohLimitTmp = KTIVAR->CpuInfo[Socket].CpuRes.MmiohLimit;
            if ((MmiohBaseTmp.hi < MmiohLimitTmp.hi) || ((MmiohBaseTmp.hi == MmiohLimitTmp.hi) && (MmiohBaseTmp.lo < MmiohLimitTmp.lo))) {
              MmiohBase[TargetIndex] = MmiohBaseTmp;
              MmiohLimit[TargetIndex] = MmiohLimitTmp;
              MmiohTarget[TargetIndex] = Socket;
              SerialValueMmioh[TargetIndex] = (KTISETUP->P2pRelaxedOrdering == KTI_ENABLE) ? 1 : 2;
              RemoteStackIdMmioh[TargetIndex] = 0;
              TargetIndex++;
            }
          } //if ((MmiohBaseTmp.hi != 0xFFFFFFFF)...
        }
      } //if (Socket == SocId) {...
    } //if ((SocketData->Cpu[Socket].Valid == TRUE)...
  } //for (Socket = 0; Socket < TmpTotCpu;

  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    if (StackPresent (SocId, Stack)) {
      //
      // Program the MmioRules
      //
      for (TargetIndex = 0; TargetIndex < TOTAL_MMIO_VTD_PER_STACK_SPR; TargetIndex++) {
        IioSetMmiohRuleIdx (SocId, Stack, MmiohTarget[TargetIndex], TargetIndex,
                            SerialValueMmioh[TargetIndex],
                            RemoteStackIdMmioh[TargetIndex],
                            MmiohBase[TargetIndex].hi, MmiohBase[TargetIndex].lo,
                            MmiohLimit[TargetIndex].hi,MmiohLimit[TargetIndex].lo);
      }

      //
      // Mmioh granularity.
      // lsb of address check for serialize rule 2 is mmiol_granularity + 24.
      // So when allocating high MMIOH with a minimum granularity of 16MB per IIO stack, program to 0, 32MB, program to 1, etc.
      //
      IioSetMmiohGranularity (SocId, Stack, KtiInternalGlobal->MmiohGranularity);
    }
  }

  return KTI_SUCCESS;
}

/**
  Routine to program the final IO SAD setting for the given topology on 10nm project.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuMmiohEntries (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  )
{
  UINT8     MaxSocket;

  if (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE) {
    if (KTISETUP->KtiCpuSktHotPlugTopology == 0) {
      MaxSocket = 4;         //online/offline based on 4S topology
    } else {
      MaxSocket = 8;         //online/offline based on 8S topology
    }
  } else {
    MaxSocket = MAX_SOCKET;
  }

  KTI_ASSERT ((MaxSocket <= MAX_SOCKET), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_79);

  ChaConfigureCpuMmiohEntry (
    SocId,
    MaxSocket,
    SocketData,
    KtiInternalGlobal->KtiCpuSktHotPlugEn,
    KtiInternalGlobal->MmiohTargetSize,
    KtiInternalGlobal->UseNonInterleavedMmioh
    );

  //
  // Program entries in IIO and KTI
  //
  ProgramCpuMmiohEntriesIio (SocketData, KtiInternalGlobal, SocId);

  if (KTIVAR->SysConfig != SYS_CONFIG_1S) {
    ProgramCpuMmiohEntriesKti (SocketData, KtiInternalGlobal, SocId);
  }

  return KTI_SUCCESS;
}

/**
  Routine to program the IIO registers related to system address space.

  @param SocketData        - CPU socket info
  @param KtiInternalGlobal - KTIRC internal variables.
  @param SocId             - processor socket ID

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramSystemAddressMap (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  )
{
  UINT8  Index, IioStack, Target_Offset;
  UINT32 MmcfgRule, MmcfgTarget, MmcfgLimit;
  UINT32 GlobBusIdRule, MmcfgGranularity;
  UINT32 *MmcfgLocalRule, *MmcfgLocalTarget;

  if (KTIVAR->mmCfgSize <= 0x10000000) {
    MmcfgGranularity = SAD_BUS_GRANTY;
  } else {
    MmcfgGranularity = (KTIVAR->mmCfgSize / 1024 / 1024 / SAD_BUS_TARGETS);
  }

  //
  // Get global mmcfg rules from CHA
  //
  ChaGetMmcfgTargetListAndRules (SocId, 0, &MmcfgTarget, &MmcfgRule, NULL);
  MmcfgRule = MmcfgRule & 0x3ffffff;

  // s5372108: [FPGA] ME in Operational mode causes Inband PECI messages to FPGA
  // FPGA does not support inband PECI messages and will not respond to them. SPS FW is placing a fix to
  // restrict inband PECI cycles to listed valid targets in iio_busid_glob_rule_b0d05f0 register.
  //
  // Clear the related bits which indicate the bus range of FPGA NID in valid_targets field
  //
  GlobBusIdRule = MmcfgRule;
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    Target_Offset = (UINT8)(KTIVAR->CpuInfo[Index].CpuRes.BusBase / MmcfgGranularity);
    IioGetUpdatedBusIdRule (SocId,
                            SocketData->Cpu[Index].Valid,
                            SocketData->Cpu[Index].SocType,
                            Target_Offset,
                            GlobBusIdRule,
                            &GlobBusIdRule);
  }

  //
  // Local rules previously calculated
  //
  MmcfgLocalRule   = &KtiInternalGlobal->MmcfgLocalRules[SocId][MMCFG_LOCAL_RULE_IDX_ADDR_CHA];
  MmcfgLocalTarget = &KtiInternalGlobal->MmcfgLocalRules[SocId][MMCFG_LOCAL_RULES_IDX_TARGET_IIO];

  MmcfgLimit = KtiInternalGlobal->MmcfgBase + KTIVAR->mmCfgSize - 1;

  //
  // Program MMCFG rules, BUSID* registers, CPUBUSNO, and MCTP Segment ID size in each stack
  //
  for (IioStack = 0; IioStack < MAX_IIO_STACK; IioStack++) {
    if (StackPresent (SocId, IioStack)) {
      //
      // MmcfgBase
      //
      IioSetMmcfgBase (SocId, IioStack, KtiInternalGlobal->MmcfgBase);
      //
      // MmcfgLimit
      //
      IioSetMmcfgLimit (SocId, IioStack, MmcfgLimit);
      //
      // BUSID
      //
      IioCfgBusId (SocId, IioStack, GlobBusIdRule, MmcfgTarget,
                   MmcfgLocalTarget, MmcfgLocalRule);
      // MCTP Segment ID size
      ProgramMctpSegmentIdSize (SocId, IioStack, MmcfgRule);
    }
  }

  return KTI_SUCCESS;
}

/**
  Setup system coherency

  @param SocketData        - pointer to the socket data structure
  @param KtiInternalGlobal - pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
SetupSystemCoherency (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8                             Idx1;
  UINT8                             Ctr;
  UINT8                             DisEgressIsocRes = 0;
  UINT32                            HitMEEnable = 0;
  UINT32                            HitMERfoDirs = 0;
  UINT32                            HitMEShar = 0;
  UINT32                            IodcEnable = 0;
  UINT32                            DisDirectory = 0;
  UINT32                            HaMiniBypassDis = 0;
  UINT32                            StaleAtoSOptEn = 0;
  UINT32                            GateOsbIodcAlloc = 0;
  UINT32                            EnableOsbLocRd = 0;
  UINT32                            EnableOsbLocRdCur = 0;
  UINT32                            EnableOsbRmtRd = 0;
  BOOLEAN                           Is4SFC, SocCnt;
  UINT8                             Is2SConfig;
  BOOLEAN                           DirModeEn, D2cEn;

  Is2SConfig = (KTIVAR->SysConfig == SYS_CONFIG_2S1L ||
                KTIVAR->SysConfig == SYS_CONFIG_2S2L ||
                KTIVAR->SysConfig == SYS_CONFIG_2S3L);

  KtiCheckPoint (0xFF, 0xFF, 0xFF, STS_SYSTEM_COHERENCY_SETUP, MINOR_STS_SYSTEM_COHERENCY_START);

  //
  // Check the system is 4S Full Connection or not
  //
  Is4SFC=FALSE;
  SocCnt=0;
  if ((KTIVAR->SysConfig == SYS_CONFIG_4S) && !KtiInternalGlobal->Is4SRing) {
    for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
      if ((SocketData->Cpu[Idx1].Valid == TRUE) && (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU)) {
        if (CountActiveKtiLink (KtiInternalGlobal, Idx1) < 3) {
          break;
        }
        SocCnt++;
      }
    }
    if (SocCnt == 4) {
      Is4SFC=TRUE;
    }
  }

  DirModeEn = (KTIVAR->OutSysDirectoryModeEn == KTI_ENABLE) ? TRUE : FALSE;
  D2cEn       = KtiInternalGlobal->D2cEn;

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if ((SocketData->Cpu[Idx1].Valid == TRUE) && (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU)) {

      //
      // M2MEM
      //
      for (Ctr = 0; Ctr < KTIVAR->MaxSupportedM2m; Ctr++) {
        if ((KTIVAR->CpuInfo[Idx1].M2mBitmap & (1 << Ctr)) == 0) {
          continue;
        }
        InitM2mem (
          Idx1,
          Ctr,
          D2cEn,
          KtiInternalGlobal->D2kEn,
          DirModeEn,
          (KTIVAR->OutIsocEn == KTI_ENABLE) ? TRUE : FALSE,
          KtiInternalGlobal->FpgaList
          );
      }

      //
      //MS2IDI
      //
      Ms2IdiSetD2C (Idx1, FALSE);

      //
      // CHA
      //

      //
      // Cha coherency settings
      //   mtoibias, biasfwd, biasfwdlocalhome : Set to 1 for all configs
      //   biasfwddoubledata: Set 1 for all configs
      //   ego: Set 1 for 1S-noncod; leave it to default for all other configs
      //   disisoctorres, disisocegrres, disisocrtidres: Set all to 1 for non-ISOCH configs; leave it to default for all other configs
      //   These are removed in HSX: conddisablef, localkeepffwds: Set to 1 for all Directory Snoop modes; leave it to default for Home Snoop mode
      //
      for (Ctr = 0; Ctr < KtiInternalGlobal->TotCha[Idx1]; Ctr++) {
        ChaConfigureCboCoh (Idx1, Ctr, KtiInternalGlobal->FpgaList, KtiInternalGlobal->SbspSoc);

        //
        // CHA Uncore Perf CSRs ICX-SP
        //
        ChaConfigureDbp (Idx1, Ctr);
      }

      //
      // CHABC settings
      //
      if (KTIVAR->OutIoDcMode != IODC_DISABLE) {
        IodcEnable = 1;
      } else {
        IodcEnable = 0;
      }

      if (KTIVAR->OutSysDirectoryModeEn == KTI_DISABLE) {
        DisDirectory = 1;
      } else {
        DisDirectory = 0;
      }

      ChaConfigureOsbSetting (
        Idx1,
        0,
        IodcEnable,
        DisDirectory,
        KtiInternalGlobal->SysOsbEn,
        KTIVAR->SysConfig,
        KtiInternalGlobal->Is4SRing,
        Is4SFC
        );

      if (Is2SConfig) {
        ChaConfigurePowerHeuristics ((UINT8)Idx1, 0);
      }
      //
      // Enable OSB on Read opcode for 2S
      //
      if (KTISETUP->DfxParm.DfxOsbLocRd != KTI_DISABLE) {
        EnableOsbLocRd = 1;
      } else {
        EnableOsbLocRd = 0;
      }

      if (KTISETUP->DfxParm.DfxOsbLocRdCur != KTI_DISABLE) {
        EnableOsbLocRdCur = 1;
      } else {
        EnableOsbLocRdCur = 0;
      }

      if (KTISETUP->DfxParm.DfxOsbRmtRd != KTI_DISABLE) {
        EnableOsbRmtRd = 1;
      } else {
        EnableOsbRmtRd = 0;
      }

      ChaConfigureHaOsbSad ((UINT8)Idx1, 0, EnableOsbLocRd, EnableOsbLocRdCur, EnableOsbRmtRd, Is2SConfig);

      if (KTIVAR->OutIsocEn == TRUE) {
        DisEgressIsocRes = 0;
        ChaDisableEgressIsocRes ((UINT8)Idx1, 0, DisEgressIsocRes);
      }

      if (KTIVAR->OutHitMeEn == TRUE) {
        HitMEEnable = 1;
        HitMERfoDirs = 1;
        if (KTISETUP->DfxParm.DfxHitMeRfoDirsEn < KTI_AUTO) {
          HitMERfoDirs = KTISETUP->DfxParm.DfxHitMeRfoDirsEn;
        }
        HitMEShar = 1;
      } else {
        HitMEEnable = 0;
        HitMERfoDirs = 0;
        HitMEShar = 0;
      }
      ChaConfigureHaCohTor ((UINT8)Idx1, 0, HitMEEnable, HitMERfoDirs, HitMEShar, Is2SConfig);

      HaMiniBypassDis = 0;
      //
      // Set the StaleAtoSOptEn if StaleAtoSOptEn is Enable AND it is not 2S glueless topology without FPGA
      //
      if ((KTIVAR->OutStaleAtoSOptEn == KTI_ENABLE) &&
          !(((KTIVAR->SysConfig == SYS_CONFIG_2S1L) || (KTIVAR->SysConfig == SYS_CONFIG_2S2L) || (KTIVAR->SysConfig == SYS_CONFIG_2S3L)) &&
            (KtiInternalGlobal->FpgaList == 0))) {
        StaleAtoSOptEn = KTI_ENABLE;
      } else {
        StaleAtoSOptEn = KTI_DISABLE;
      }

      //
      // 0 for 1-socket, 2-socket
      // 1 for 4-socket, 8-socket
      //
      if (KTIVAR->SysConfig < SYS_CONFIG_4S) {
        GateOsbIodcAlloc = 0;
      } else {
        GateOsbIodcAlloc = 1;
      }

      if (KTISETUP->DfxParm.DfxGateOsbIodcAllocEn < KTI_AUTO) {
        GateOsbIodcAlloc  = KTISETUP->DfxParm.DfxGateOsbIodcAllocEn;
      }
      ChaConfigureHaCoh1 ((UINT8)Idx1, 0, HaMiniBypassDis, StaleAtoSOptEn, GateOsbIodcAlloc, Is2SConfig);

      //
      // KTIAgent
      //
      ProgramD2C (SocketData, KtiInternalGlobal, Idx1);
    }
  }

  //
  // Program snoopfilter if fpga devices enabled
  //
  SetupSnoopFilter (SocketData, KtiInternalGlobal);

  return KTI_SUCCESS;
}

/**
  Initialize M2Mem tiles.

  @param SocketData        - pointer to the socket data structure
  @param KtiInternalGlobal - pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
SetupKtiMisc (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8   Idx1, Idx2, Ctr, IioStack;
  UINT32  Misclist, CpuList;
  UINT32  GidNidMap, GblNidMap0, GblNidMap1;
  UINT32  RdInvOwnAcceptSm = 0;
  UINT32  RdDataMigAcceptSm = 0;
  UINT32  ActiveNodeMask = 0;
  UINT8   MaxAddrLine;
  UINT8   LocalSocId;
  UINT8   LockNodeId;
  UINT8   LegacyNodeId;
  UINT32  IntList, StopStartList, IntPrioList, BcastList;
  BOOLEAN OrderingFlag;
  UINT8   KtiEnabledMask;
  UINT8   ClockModulationEn;
  UINT8   M2m;
  M2M_DEFEATURE0_NORMAL_MODE  M2mDefeature0Mode;

  KtiCheckPoint (0xFF, 0xFF, 0xFF, STS_PROTOCOL_LAYER_SETTING, MINOR_STS_KTI_MISC_SETUP);

  //
  // Prepare the NID map
  //
  GblNidMap0 = 0;         // for socket 0-3
  GblNidMap1 = 0;         // for socket 4-7
  GidNidMap = 0;

  KtiEnabledMask = 0;
  Misclist = KtiInternalGlobal->CpuList;                //Broadcast list for WbInvdAck/InvdAck/EOI

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (KtiInternalGlobal->CpuList & (1 << Idx1)) {
      if (Idx1 < 4) {
        GblNidMap0 = GblNidMap0 | (1 << (28 + Idx1)) | (Idx1 << (4 * Idx1));
      } else {
        GblNidMap1 = GblNidMap1 | (1 << (28 + (Idx1 - 4))) | (Idx1 << (4 * (Idx1 - 4)));
      }

      GidNidMap = GidNidMap | (Idx1 << (3 * Idx1));
    }
  }

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (KtiInternalGlobal->CpuList & (1 << Idx1)) {

      //
      //Program M2M credit setting
      //
      ProgramM2mCreditSetting (KtiInternalGlobal, Idx1);
      //
      //Silicon WA: set the Defeature0.D2K4CisgrDis=1 for SPR A0 and B0,
      //as the bit controls the d2k return for cisgress requests on 2LM mode
      //
      if (IsSiliconWorkaroundEnabled ("S22010499709")) {
        ZeroMem ((VOID *)(UINTN)&M2mDefeature0Mode, sizeof (M2M_DEFEATURE0_NORMAL_MODE));
        for (M2m = 0; M2m < KTIVAR->MaxSupportedM2m; M2m++) {
          if ((KTIVAR->CpuInfo[Idx1].M2mBitmap & (1 << M2m)) == 0) {
            continue;
          }
          M2mDefeature0Mode.d2k4cisgrdis = 1;
          SetM2mDefeatures0NormalMode (Idx1, M2m, &M2mDefeature0Mode);
        }
      }
      //
      // IIO Interface
      //
      for (IioStack = 0; IioStack < MAX_IIO_STACK; IioStack++) {
        if (StackPresent (Idx1, IioStack)) {
          //
          // Coherent Interface Protocol Control
          //
          IioSetWrtCombDca (Idx1, IioStack, 0, FALSE);
          //
          // IIO Misc Control
          //
          MaxAddrLine = KTIVAR->MaxAddress;
          OrderingFlag = (KTIVAR->OutIsocEn == TRUE && Idx1 == KtiInternalGlobal->SbspSoc && IioStack == IIO_CSTACK);
          IioSetMiscCtrl (Idx1, IioStack, KTIVAR->OutLegacyVgaSoc, KTISETUP->SplitLock, MaxAddrLine, OrderingFlag);

          if (KTIVAR->OutIsocEn == TRUE && Idx1 == KtiInternalGlobal->SbspSoc && IioStack == IIO_STACK0) {
            IioSetIsocEn (Idx1, IioStack, 0x1);
          } // if IsocEn, SBSP Cstack
        } // if IioStack is present
      } // for IioStack loop

      //
      // s5330373: CLONE SKX Sighting: PCIE:  CSR Value for Isoch credits causes ITC credit overflow to be logged
      // Prior to programming ItcCtrl45.max_isoch, set tcerrdis.itc_irp_cred_of = 1; clear after WR; permanent W/A
      IioSetItcIrpOflEn (Idx1, IIO_CSTACK, 0);

      //
      // Ubox
      //

      //
      // Local Node id
      //
      if (KTISETUP->DfxParm.DfxLockMaster == LOCK_MASTER_AUTO) {
        LocalSocId   = Idx1;
        LockNodeId   = KtiInternalGlobal->SbspSoc;
        LegacyNodeId = KtiInternalGlobal->SbspSoc;
      } else {
        LocalSocId   = Idx1;
        LockNodeId   = KTISETUP->DfxParm.DfxLockMaster;
        LegacyNodeId = KtiInternalGlobal->SbspSoc;
      }
      UBoxConfigureCpuNodeId (Idx1, 0, LocalSocId, LockNodeId, LegacyNodeId);

      //
      // CpuEnable
      //
      UBoxConfigureCpuEnabled (Idx1, 0, KtiInternalGlobal->CpuList);

      //
      // Bcastlist
      //
      if (KTIVAR->RasInProgress == TRUE && KTIVAR->RasSocId == Idx1) {
        //
        // CPU Socket being onlined should not be in the system interrupt domain until the last quiesce operation
        //
        IntList = StopStartList = Misclist = IntPrioList = 0;
      } else if (KTIVAR->RasInProgress == TRUE && KTIVAR->RasSocId != Idx1) {
        //
        // CPU Socket being onlined should not be in the system interrupt domain until the last quiesce operation
        //
        Misclist &= (~(1 << (KTIVAR->RasSocId)));
        CpuList = (KtiInternalGlobal->CpuList & (~(1 << (KTIVAR->RasSocId))));
        IntList = StopStartList = IntPrioList = CpuList;
      } else {
        IntList = StopStartList = IntPrioList = KtiInternalGlobal->CpuList;
      }

      //
      // Whitley FPGA --- BBS will be >= 8.0.0, so FPGA shall always be added to startstoplist; no version check necessary.
      //
      StopStartList |= (KtiInternalGlobal->FpgaList);
      UBoxConfigureCpuBroadcastList (Idx1, 0, IntList, StopStartList, Misclist, IntPrioList, &BcastList);

      //
      // Lock Control, Enable Lock
      //
      UBoxConfigureLockControlRegister (Idx1, 0, KTISETUP->SplitLock);

      //
      // Group Id to Node Id Map
      //
      UBoxConfigureGidNidMapping (Idx1, 0, GidNidMap);

      UBoxConfigureUboxErrorCtrl (Idx1, 0);

      //
      // PCU
      //

      //
      // CpuNodeID
      //
      PcuSetGblNid0to3Map (Idx1, GblNidMap0);  // Set the NID of CPUs in the system and enable the register
      if (GblNidMap1 != 0) {
        PcuSetGblNid4to7Map (Idx1, GblNidMap1);
      }

      //
      // VCU
      //

      //
      // CHABC
      //

      //
      // Program the TOR Thresholds loctorem_norm and loctorem_empty
      //
      ChaConfigureTorThresholds ((UINT8)Idx1, 0, KTIVAR->OutTorThresLoctoremNorm, KTIVAR->OutTorThresLoctoremEmpty, KTIVAR->OutRrqThreshold);

      //
      // Program the clock modulation setting in CHA_BC
      //
      ClockModulationEn = KTI_ENABLE;

      if (IsSiliconWorkaroundEnabled ("S14011240682")) {
        ClockModulationEn = KTI_DISABLE;
      }

      if (KTISETUP->DfxParm.DfxClockModulationEn < KTI_AUTO) {
        ClockModulationEn = KTISETUP->DfxParm.DfxClockModulationEn;
      }
      //
      // If clock moulation enable is true and it is not SPR A0, program clock modulation enable.
      // In addtion, the register's hardware default value is disable.
      //
      if (ClockModulationEn == KTI_ENABLE) {
        ChaConfigureClockModulation ((UINT8)Idx1, 0, TRUE);
      }

      //
      // Isoc or MESEG Enable
      //
      for (Ctr = 0; Ctr < KtiInternalGlobal->TotCha[Idx1]; Ctr++) {
        ChaConfigureIsoc (Idx1, Ctr, KTIVAR->OutIsocEn);
      }

      if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_C0) && ((KTIVAR->OutIsocEn != TRUE) && (KTIVAR->SysConfig <= SYS_CONFIG_4S))) {
        if (KTIVAR->OutXptPrefetch == TRUE) {
          for (Ctr = 0; Ctr < KtiInternalGlobal->TotCha[Idx1]; Ctr++) {
            ChaConfigureDemandPriorityLocal (Idx1, Ctr, TRUE);
          }
          Ms2IdiConfigPriorityPrefetchLocal (Idx1, TRUE);
        }

        if (KTIVAR->OutXptRemotePrefetch == TRUE) {
          for (Ctr = 0; Ctr < KtiInternalGlobal->TotCha[Idx1]; Ctr++) {
            ChaConfigureDemandPriorityRemote (Idx1, Ctr, TRUE);
          }
          Ms2IdiConfigPriorityPrefetchRemote (Idx1, TRUE);
        }
      }
      //
      // CHABC
      //
      ActiveNodeMask = KtiInternalGlobal->CpuList;   // Program active_node_mask_bit  field
      ActiveNodeMask |= KtiInternalGlobal->FpgaList; // include any fpga
      KtiEnabledMask = 0;
      for (Idx2 = 0; Idx2 < (KtiInternalGlobal->KtiPortCnt); Idx2++) {
        if (SocketData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) {
          KtiEnabledMask |= (1 << Idx2);
        }
      }

      if (KTIVAR->SysConfig == SYS_CONFIG_1S) {
        RdInvOwnAcceptSm = 0;
        RdDataMigAcceptSm = 0;
      } else {
        RdInvOwnAcceptSm = 1;
        RdDataMigAcceptSm = 1;
      }

      ChaConfigureHaCohKtiEnableActiveNodeAndDataM (Idx1, 0, ActiveNodeMask, (UINT32) KtiEnabledMask, RdInvOwnAcceptSm, RdDataMigAcceptSm);
      //
      // Isoc enabling in CHA
      //
      for (Ctr = 0; Ctr < KtiInternalGlobal->TotCha[Idx1]; Ctr++) {
        if (KTIVAR->OutIsocEn == TRUE) {
          ChaEnableIsocInCha (Idx1, Ctr);
        } // Isoc Enabled
      } // CHA loop

      //
      // M2MEm
      //
      if (KTIVAR->OutIsocEn == TRUE) {
        for (Ctr = 0; Ctr < KTIVAR->MaxSupportedM2m; Ctr++) {
          if ((KTIVAR->CpuInfo[Idx1].M2mBitmap & (1 << Ctr)) == 0) {
            continue;
          }
          SetM2mCreditThrshConfigs (Idx1, Ctr, 0x2, 0x2, 0x2);
        }
      }

      for (Ctr = 0; Ctr < KtiInternalGlobal->KtiPortCnt; Ctr++) {
        if (((KtiInternalGlobal->KtiPortPresentBitmap[Idx1] & (1 << Ctr)) != 0) && (!IsUpiPortUpimdf (Idx1, Ctr))) {
          SetupM3KtiMisc (Idx1, Ctr);
        }
      }
    }
  }
  //
  // Program M2IOSF->M2PCIE P2P Credits
  //
  ProgramM2iosfP2pCredits (KtiInternalGlobal);

  //
  // Trigger credit reload without issuing warm-reset
  //
  LateActionCreditUpdateWithoutWarmReset (KtiInternalGlobal);

  return KTI_SUCCESS;
}

/**
  SBSP programs bus numbers and mmcfg rules for self.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Targets           - Enabled socket targets
  @param MmcfgTgtlist      - Common mmcfg target list
  @param SocId             - Socket to program

**/
KTI_STATUS
KtiFinalSbspBusCfg (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Targets,
  UINT32                     MmcfgTgtlist,
  UINT8                      SocId
  )
{
  MMCFG_RULE_N0_CHABC_SAD1_STRUCT mmcfgrule0;
  UINT32                          Data32;
  UINT32                          ChaMmcfgLocalRule[MAX_MMCFG_LOCAL_RULES_TARGET_CHA] = {0};
  UINT8                           Idx1, Idx2, Idx3, IdxReg, IdxEntry;
  UINT8                           MmcfgLength = 0;
  UINT8                           CpuType;
  UINT8                           StackPersonality;
  UINT32                          MmcfgBaseS0U1;
  UINT32                          MmcfgBaseS1U1;
  UINT32                          UcodeTrapMmcfgBase;
  UINT64_STRUCT                   MsrData;
  BOOLEAN                         UcodeTrapSupported;

  CpuType     = KtiInternalGlobal->CpuType;
  switch (KTIVAR->mmCfgSize / 1024 / 1024) {
    case 64:
      MmcfgLength = 0x6;
      break;
    case 128:
      MmcfgLength = 0x5;
      break;
    case 256:
      MmcfgLength = 0x4;
      break;
    case 512:
      MmcfgLength = 0x2;
      break;
    case 1024:
      MmcfgLength = 0x1;
      break;
    case 2048:
      MmcfgLength = 0x0;
      break;
    default:
      //
      //Automated add of default case. Please review.
      //
      break;
  }

  //
  // Reprogram bus/mmcfg on SBSP socket
  //
  //
  // This code depends on SBSP residing on Bus0,1,2,3,4,5.  We take advantage of the default bus decoding when
  // MMCFG and CPUBUSNO are disabled to complete the final programming.
  //

  //
  // Disable current CPUBUSNO Valid bit
  //
  UBoxSetCpuBusNoValid (SocId, 0, 0);

  //
  // At this point, local CSRs are moved back to default buses
  // Disable current MMCFG_RULE (at default bus#)
  //
  KtiOutPort32 (0x0cf8, LEGACY_CSR_MMCFG_RULE_N0);
  mmcfgrule0.Data = InPort32 (0x0cfc);
  mmcfgrule0.Bits.rule_enable = 0;
  KtiOutPort32 (0x0cfc, mmcfgrule0.Data);

  //
  // At this point, only local CSRs are accessiable via CF8/CFC IO ports and on default buses
  //
  // Write new value for MMCFG_TARGET_LIST
  //
  KtiOutPort32 (0x0cf8, LEGACY_CSR_MMCFG_TARGET_LIST);
  Data32 = MmcfgTgtlist;

  for (Idx1 = 0; Idx1 < SI_MAX_CPU_SOCKETS; Idx1++) {
    if (((Data32 >> Idx1*SADTGT_SIZE) & 0xF) == SocId) {
      Data32 = (Data32 & ~(0xF << (SADTGT_SIZE * Idx1))) | ((8 | SocId) << (SADTGT_SIZE * Idx1));
    }
  }

  KtiOutPort32 (0x0cfc, Data32);

  //
  // Build and write new value for MMCFG_RULE_LOCAL_ADDRESS
  // Build and write final value for MMCFG_RULE_LOCAL
  //
  Idx3 = 0;
  KtiInternalGlobal->MmcfgLocalRules[SocId][MMCFG_LOCAL_RULE_IDX_ADDR_CHA] = (UINT8) KTIVAR->CpuInfo[SocId].CpuRes.StackRes[IIO_STACK0].BusBase;
  for (Idx1 = 0; Idx1 < MAX_LOGIC_IIO_STACK; Idx1++) {
    //
    // Check for valid resources
    //
    StackPersonality = KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Idx1].Personality;
    if ((StackPersonality == TYPE_UBOX_IIO)
      || (StackPersonality == TYPE_HFI)
      || (StackPersonality == TYPE_NAC)
      || (StackPersonality == TYPE_DINO))
    {
      if (KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Idx1].BusLimit >= KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Idx1].BusBase) {
        if (Idx3 < MAX_IIO_STACK) {                  // iio stack
          IdxEntry = (Idx3 + 1) % 4;
          IdxReg   = (Idx3 + 1) / 4;
          KtiInternalGlobal->MmcfgLocalRules[SocId][IdxReg] |= (KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Idx1].BusLimit << (8 * IdxEntry));
          //
          // This is for CHA target list
          //
          IdxEntry = Idx3 % 6;
          IdxReg   = Idx3 / 6;
          ChaMmcfgLocalRule[IdxReg] |= Idx1 << (IdxEntry * 4);
          //
          // This is for IIO target list
          //
          IdxEntry = Idx3 % 8;
          IdxReg   = MMCFG_LOCAL_RULES_IDX_TARGET_IIO + (Idx3 / 8);
          KtiInternalGlobal->MmcfgLocalRules[SocId][IdxReg] |= Idx1 << (IdxEntry * 4);
        }
        Idx3++;
      }
    }
  }

  for (Idx1 = 0; Idx1 < MAX_MMCFG_LOCAL_RULES_ADDR_CHA; Idx1++) {
    KtiOutPort32 (0x0cf8, LEGACY_CSR_MMCFG_LOCAL_RULE_ADDRESS_CFG(Idx1));
    KtiOutPort32 (0x0cfc, KtiInternalGlobal->MmcfgLocalRules[SocId][Idx1]);
  }

  for (Idx1 = 0; Idx1 < MAX_MMCFG_LOCAL_RULES_TARGET_CHA; Idx1++) {
    KtiOutPort32 (0x0cf8, LEGACY_CSR_MMCFG_LOCAL_RULE(Idx1));
    KtiOutPort32 (0x0cfc, ChaMmcfgLocalRule[Idx1]);
  }

  //
  // Write new value for CPUBUSNO0/1
  //
  Data32 = 0;
  Data32 |= KTIVAR->CpuInfo[SocId].CpuRes.StackRes[UBOX_STACK].BusBase;
  Data32 |= (KTIVAR->CpuInfo[SocId].CpuRes.StackRes[UBOX_STACK].BusLimit << 8);

  KtiOutPort32 (0x0cf8, LEGACY_CSR_CPUBUSNO_UBOX);
  KtiOutPort32 (0x0cfc, Data32);

  //
  // Write new value for MMCFG_RULE
  // And enable MMCFG
  //
  Data32 = 0;
  KtiOutPort32 (0x0cf8, LEGACY_CSR_MMCFG_RULE_N1);
  KtiOutPort32 (0x0cfc, Data32);

  KtiOutPort32 (0x0cf8, LEGACY_CSR_MMCFG_RULE_N0);
  mmcfgrule0.Data = InPort32 (0x0cfc);
  mmcfgrule0.Bits.rule_enable = 1;
  mmcfgrule0.Bits.length = MmcfgLength;
  mmcfgrule0.Bits.base_address = KtiInternalGlobal->MmcfgBase >> 26;
  mmcfgrule0.Bits.valid_targets= Targets;
  KtiOutPort32 (0x0cfc, mmcfgrule0.Data);

  //
  // Update struc so that we begin accessing socket via new bus number
  //
  SetSocketFirstBus (SocId, KTIVAR->CpuInfo[SocId].CpuRes.BusBase);
  SetSocketLastBus (SocId, KTIVAR->CpuInfo[SocId].CpuRes.BusLimit);

  KTI_HOST_OUT_PTR->mmCfgBase = KtiInternalGlobal->MmcfgBase;
  KTI_HOST_OUT_PTR->mmCfgBaseH[SocId] = KTIVAR->CpuInfo[SocId].CpuRes.SegMmcfgBase.hi;
  KTI_HOST_OUT_PTR->mmCfgBaseL[SocId] = KTIVAR->CpuInfo[SocId].CpuRes.SegMmcfgBase.lo;
  KTI_HOST_OUT_PTR->segmentSocket[SocId] = KTIVAR->CpuInfo[SocId].CpuRes.PciSegment;

  KTI_HOST_OUT_PTR->Socket10nmUboxBus0[SocId] = KTIVAR->CpuInfo[SocId].CpuRes.StackRes[UBOX_STACK].BusBase;

  for (Idx1 = 0; Idx1 < MAX_LOGIC_IIO_STACK; Idx1++) {
    SetStackBus (SocId, Idx1, KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Idx1].BusBase);
  }
  UpdateAndDisplayBusInformation (KtiInternalGlobal, GetSysCpuCsrAccessVar (), SocketData, FALSE);

  // APTIOV_SERVER_OVERRIDE_START : Added support to update PcdPciExpressBaseAddress for MMCFG change
  PcdSet64 (PcdPciExpressBaseAddress, KTI_HOST_OUT_PTR->mmCfgBase);
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "PcdPciExpressBaseAddress = 0x%x\n", PcdGet64 (PcdPciExpressBaseAddress)));
  
  //
  // WA for SPR A stepping CHA BDF PLA issue
  // Note: This WA only support up to 2 sockets configuration (S1507951535)
  //

  MsrData.hi = 0;
  MsrData.lo = 0;
  WriteMsrPipeHL (SocId, MSR_IA32_BIOS_SIGN_ID, MsrData);  //Write 0 to MSR 8Bh
  ExecuteCpuidPipe (SocId, CPUID_VERSION_INFO, 0, NULL, NULL, NULL, NULL);  //CPUID(1) will update MSR 8Bh
  MsrData = ReadMsrPipeHL (SocId, MSR_IA32_BIOS_SIGN_ID);  //Read MSR 8Bh back

  //
  // Store the MicroCodeRevision in KtiHostOut datastructure
  //
  KTI_HOST_OUT_PTR->MicroCodeRevision = MsrData.hi;

  if (IsSiliconWorkaroundEnabled ("S1507951535")) {
    //
    // Check Microcode Revision
    //
    UcodeTrapSupported = ((KTI_HOST_OUT_PTR->MicroCodeRevision & 0xFF) >= 0x13) ? TRUE : FALSE;

    if (UcodeTrapSupported && ((KtiInternalGlobal->CpuList & 0xFFFFFFFC) == 0)) {  //Only do this for a system with up to two sockets

      MmcfgBaseS0U1 = KTIVAR->CpuInfo[SocId].CpuRes.SegMmcfgBase.lo + ((KTI_HOST_OUT_PTR->Socket10nmUboxBus0[SocId] + 1) << 20);  //MmcfgBase of S0 U1 Bus
      UcodeTrapMmcfgBase = (MmcfgBaseS0U1 >> 20);

      if ((KtiInternalGlobal->CpuList & BIT1) != 0) {  // Socket 1 is present
        MmcfgBaseS1U1 = KTIVAR->CpuInfo[1].CpuRes.SegMmcfgBase.lo + ((KTI_HOST_OUT_PTR->Socket10nmUboxBus0[1] + 1) << 20);      //MmcfgBase of S1
        UcodeTrapMmcfgBase |= ((MmcfgBaseS1U1 >> 4) & 0x0FFF0000);
      }

      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  UcodeTrapMmcfgBase = 0x%08x\n", UcodeTrapMmcfgBase));

      MsrData.hi = 0;
      MsrData.lo = UcodeTrapMmcfgBase;
      if (!(GetEmulation() & SIMICS_FLAG)) {
        WriteMsrPipeHL (SocId, MSR_BDF_PLA_VMSR_62H, MsrData);
      }
    }
  }

  //
  // Dump out newly activated strucs
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n KTI_HOST_OUT_PTR->mmCfgBase         = 0x%08x\n", KTI_HOST_OUT_PTR->mmCfgBase));
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if ((SocketData->Cpu[Idx1].Valid == TRUE) ) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, " KTI_HOST_OUT_PTR->segmentSocket[%u]  =       0x%02x\n", Idx1, KTI_HOST_OUT_PTR->segmentSocket[Idx1]));
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, " KTI_HOST_OUT_PTR->SocketFirstBus[%u] =       0x%02x\n", Idx1, GetSocketFirstBus (Idx1)));
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, " KTI_HOST_OUT_PTR->SocketLastBus[%u]  =       0x%02x\n", Idx1, GetSocketLastBus (Idx1)));
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, " KTI_HOST_OUT_PTR->mmCfgBaseH[%u]     = 0x%08x\n", Idx1, KTI_HOST_OUT_PTR->mmCfgBaseH[Idx1]));
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, " KTI_HOST_OUT_PTR->mmCfgBaseL[%u]     = 0x%08x\n\n", Idx1, KTI_HOST_OUT_PTR->mmCfgBaseL[Idx1]));

    }
  }
  // APTIOV_SERVER_OVERRIDE_END : Added support to update PcdPciExpressBaseAddress for MMCFG change

  //
  // Enable Ubox decoder.  This works since always on Bus0 of the socket
  //
  DisableCsrAddressCache ();
  UBoxSetCpuBusNoValid (SocId, 0, 1);
  EnableCsrAddressCache ();

  //
  // Make missing socket/stacks -1
  //
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (SocketData->Cpu[Idx1].Valid == FALSE) {
      for (Idx2 = 0; Idx2 < MAX_LOGIC_IIO_STACK; Idx2++) {
        SetStackBus (Idx1, Idx2, 0xFF);
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  Get RAS Type from CAP Register

  @param  SocId             - Soc ID of the CPU who's enabled Ras info is requested, 0...3

  @retval RasType

**/
UINT32
GetRasTypeByCapRegister (
  UINT8                      SocId
  )
{
  UINT32 RasType;

  RasType = PcuGetRasType (SocId);

  return RasType;
}

/**
  Collect and clear error info.

  @param SocketData        - pointer to the socket data structure
  @param KtiInternalGlobal - pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
CollectAndClearErrors (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8                             Soc;
  UINT8                             Ctr;
  UINT32                            Data32;

  Data32 = 0;

  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {
    if ((SocketData->Cpu[Soc].Valid == TRUE) && (SocketData->Cpu[Soc].SocType == SOCKET_TYPE_CPU)) {
      CollectAndClearM3KtiErrors (SocketData, KtiInternalGlobal, Soc);
      //
      // M2PCIe
      //
      for (Ctr = 0; Ctr < MAX_IIO_STACK; Ctr++) {
        if ((KTI_HOST_OUT_PTR->M2PciePresentBitmap[Soc] & (1 << Ctr)) == 0) {
          continue;
        }

        KTIVAR->CpuInfo[Soc].CpuErrLog.M2Pcie.R2Egrerrlog = M2IalR2WriteBackEgressErrLog (Soc, Ctr);
        KTIVAR->CpuInfo[Soc].CpuErrLog.M2Pcie.R2Ingerrlog0 = M2IalR2WriteBackIngressErrLog (Soc, Ctr);

        //
        // Si W/A 310710: Mask parity error on rsingrblpm with CSR bit
        //
        M2IalR2MaskParIngressErr (Soc, Ctr);
      } //scan M2PCIE

      //
      //M2UBOX
      //
      UBoxCollectAndClearErrors (Soc, 0, &(KTIVAR->CpuInfo[Soc].CpuErrLog.M2Pcie));

      for (Ctr = 0; Ctr < KtiInternalGlobal->KtiPortCnt; Ctr++) {
        if ((KtiInternalGlobal->KtiPortPresentBitmap[Soc] & (1 << Ctr)) != 0) {
          CollectAndClearKtiPortErrors (Soc,
                                        Ctr,
                                        (BOOLEAN) KTIVAR->CpuInfo[Soc].LepInfo[Ctr].Valid,
                                        KTIVAR->OutKtiFpgaPresent[Soc],
                                        KTIVAR->OutKtiFpgaEnable[Soc],
                                        SocketFpgasInitialized (KtiInternalGlobal));
        }
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  Routine to program the final IO SAD setting for the given topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuMiscSadEntries (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  )
{
  UINT8  IioStack;
  UINT32 SadLegacyPchTarget = CHA_DEFAULT_CFG;
  UINT32 SadVgaTarget = CHA_DEFAULT_CFG;
  UINT32 SadSourceId = CHA_DEFAULT_CFG;
  UINT32 SadSourceIdEnable = CHA_DEFAULT_CFG;
  //
  // SAD Target - program the legacy VGA range target id, SourceID and Enable the SourceID featrue
  //
  if (SocId == KTIVAR->OutLegacyVgaSoc) {
    SadVgaTarget = SOCKET_LOCAL_5BITS_NID_MSB | KTIVAR->OutLegacyVgaStack;
  } else {
    SadVgaTarget = KTIVAR->OutLegacyVgaSoc;
  }

  //
  // Program legacy_pch_target in CHA
  //
  if (SocId == KtiInternalGlobal->SbspSoc) {
    SadLegacyPchTarget = SOCKET_LOCAL_5BITS_NID_MSB;
  } else {
    SadLegacyPchTarget = KtiInternalGlobal->SbspSoc;
  }

  SadSourceId = SocId;
  SadSourceIdEnable = 1;
  ChaSadTargetCfg (SocId, 0, CHA_DEFAULT_CFG, SadLegacyPchTarget, SadVgaTarget, SadSourceId, SadSourceIdEnable);

  //
  // Sad Control and Sad Target in IIO
  //
  for (IioStack = 0; IioStack < MAX_IIO_STACK; IioStack++) {
    if (StackPresent (SocId, IioStack)) {
      //
      // For SPR, SAD_TARGET_IIO_VTD_REG still use 4bits for VGA/PCH/Flash target.
      //
      IioSetSadTargets (SocId, IioStack, KtiInternalGlobal->SbspSoc,
                        IIO_CSTACK, (SOCKET_LOCAL_4BITS_NID_MSB | IIO_CSTACK),
                        (SOCKET_LOCAL_5BITS_NID_MSB | KTIVAR->OutLegacyVgaStack),
                        KTIVAR->OutLegacyVgaSoc, KTIVAR->OutLegacyVgaStack);
      IioSetNodeId (SocId, IioStack);
    }
  }

  //
  // Kti SadTarget
  //
  ProgramKtiMiscSadEntries (SocketData, KtiInternalGlobal, SocId);

  return KTI_SUCCESS;
}

/**
  Program misc registers that need to be programmed before WR.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

**/
KTI_STATUS
ProgramMiscBeforeWarmReset (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8 Idx;

  for (Idx = 0; Idx < MAX_SOCKET; Idx++) {
    if ((SocketData->Cpu[Idx].Valid == TRUE) && (SocketData->Cpu[Idx].SocType == SOCKET_TYPE_CPU)) {
      ChaConfigureKtiTorTimeout (Idx, 0, 0x6, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16);

      //
      // s5330373: CLONE SKX Sighting: PCIE:  CSR Value for Isoch credits causes ITC credit overflow to be logged
      // Prior to programming ItcCtrl45.max_isoch, set tcerrdis.itc_irp_cred_of = 1; clear after WR; permanent W/A
      //
      IioSetItcIrpOflEn (Idx, IIO_CSTACK, 1);
      //
      // Isoc/MeSeg enabling in IIO, OK to program on every boot regardless of Isoc/MeSeg enabled
      // To be programmed only on Cstacks before warm reset
      // 4928588: Cloned From SKX Si Bug Eco: Isoch trafiic does not get priority over Non-isoch traffic for credit returns
      // Note: permanent workaround for SKX, but this workaround won't be needed for ICX
      // This bitfield only makes a difference if there is Isoc traffic, otherwise benign:
      //
      IioSetIsocMeSegCfg (Idx, IIO_CSTACK);
    }
  }

  return KTI_SUCCESS;
}

//
// XPT should be supported but no headers available for ICXDE/SNR so program will be skip for now
//           this will be fixed in the WW37 headers integration
//
/**
  A routine for programming the DDRT QoS Cfg

  @param  KtiInternalGlobal -   KTIRC Internal Global data

  @retval KTI_SUCCESS       -   DDRT QoS Cfg successfully
          KTI_IGNORE        -   Ignore to program DDRT QoS
**/
KTI_STATUS
SetDDRTQosCfg (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8            Socket;
  UINT8            IioStack;
  SAD_TABLE        *SADEntry;
  MEM_VAR_STRUCT   *MemVar;
  UINT32           SadEntryBase;
  UINT32           DdrtBaseAddress  = 0;
  UINT32           DdrtLimitAddress = 0;
  BOOLEAN          DdrtBaseFound = FALSE;
  UINT8            DdrtQosMode;
  KTI_HOST_IN      *KtiHostInPtr;
  UINT8            Ctr;
  UINT8            MaxIMC;
  UINT8            ClusterIdx;
  UINT8            SadRuleIdx;
  UINT8            SadRuleIdxInCluster;
  UINT8            DistressSel;
  UINT8            NumOfCluster;
  BOOLEAN          UnusedSadClusterFound = FALSE;

  MaxIMC = GetMaxImc ();
  MemVar = GetMemVarPointer ();
  NumOfCluster = GetNumOfClusterPerSystem ();

  KtiHostInPtr  = KTI_HOST_IN_PTR;
  DdrtQosMode = KtiHostInPtr->DdrtQosMode;
  //
  // Dump the SAD Entry info for checking
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket ++) {
    if (SocketPresent (Socket)) {
      for (ClusterIdx = 0; ClusterIdx < MAX_DRAM_CLUSTERS; ClusterIdx++) {
        if (UnusedSadClusterFound) {
          break;
        }
        for (SadRuleIdxInCluster = 0; SadRuleIdxInCluster < MAX_SAD_RULES; SadRuleIdxInCluster++) {
          SadRuleIdx = ClusterIdx * MAX_SAD_RULES + SadRuleIdxInCluster;

          SADEntry = &MemVar->socket[Socket].SAD[SadRuleIdx];
          if (SADEntry->Enable == 0) {
            if (SadRuleIdxInCluster == 0) {
              UnusedSadClusterFound = TRUE;
            }
            break; // end of SAD table
          }
          if (IsMemTypeAppDirect (SADEntry->type)) {
            SadEntryBase = SADEntry->Base;
            // Set the DDRT Base, SadEntryBase is the [51:26] of the Address.
            if ((SadEntryBase <= DdrtBaseAddress) || !DdrtBaseFound) {
              DdrtBaseAddress = SadEntryBase;
              DdrtBaseFound = TRUE;
            }
            if ((DdrtLimitAddress < SADEntry->Limit) || (DdrtLimitAddress == 0)) {
              DdrtLimitAddress = SADEntry->Limit;
            }
          }
        }
      }
    }
  }

  if (DdrtLimitAddress > DdrtBaseAddress) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nGlobal DDRT Range Base = %x, Limit = %x\n", DdrtBaseAddress, DdrtLimitAddress));
    for (Socket = 0; Socket < MAX_SOCKET; Socket ++) {
      if (SocketPresent (Socket)) {
        //
        // Program MS2IDI registers for DDRT QoS Feature
        // Program DDRT Range Base and Limit
        //
        Ms2IdiSetDdrtRange (Socket, DdrtBaseAddress, DdrtLimitAddress);

        //
        // Enable the DDRT QoS Feature
        //
        Ms2IdiSetEnDdrtQoS (Socket, DdrtQosMode);

        //
        // Program CHA registers for DDRT QoS Feature
        //
        ChaConfigureDDRTQoSTorThresholds (Socket, 0, DdrtQosMode);

        //
        // Program MS2IOSF registers for DDRT QoS Feature
        //
        for (IioStack = 0; IioStack < MAX_IIO_STACK; IioStack++) {
          if (StackPresent (Socket, IioStack)) {
            //
            // MS2IOSF RdTrackerLowBWCreditsEn, WrTrackerLowBWCreditsEn for DDRT
            //
            IioSetDdrtCrdEn (Socket, IioStack, DdrtQosMode);

            //
            // A0-Stepping WA:
            // Defeature for DDRT credit feature if the MS2IAL instance actually connects to an CXL device.
            // In addition, set the DDRT base and limit address ranges to all 0s to completely turn off AppDirect QoS from M2IOSF.
            // Note: As the register default data happens to be able to meet the WA programming requirements, it just needs to skip
            // the below register programming if it is A0-Stepping and CXL device is detected in the MS2IAL.
            //
            if (!IsSiliconWorkaroundEnabled ("S1409930097") || (GetCxlStatus (Socket, IioStack) != AlreadyInCxlMode)) {
              //
              // A0,B0,C0-Stepping WA:
              // Keep the R2LBWPRQCFG register default 0x01 if A0,BO,C0 silicon detected.
              //
              if (!IsSiliconWorkaroundEnabled ("S14011903038")) {
                //
                // Set MS2IOSF DDRT_Defeature[0:0] for DDRT
                //
                M2IalClearDdrtDefeature (Socket, IioStack, DdrtQosMode);
              }
              if (DdrtQosMode == DDRT_QOS_MODE_2) {
                //
                // Only configure DDRT range in Mode 2 where ddrt_defeature bit is set to 0
                //
                IioSetDdrtBase (Socket, IioStack, DdrtBaseAddress, DdrtLimitAddress,
                                FLD_MASK (LOWBWMEM_BASE_LIMIT_N0_IIO_VTD_SIZE, 0),
                                LOWBWMEM_BASE_LIMIT_N0_IIO_VTD_SIZE);
              }
            }

            //
            // Set Crystal Ridge QoS settings
            //
            M2IalSetCrQosPerformance (Socket, IioStack, DdrtQosMode);

          }
        }

        for (Ctr = 0; Ctr < MaxIMC; Ctr++) {
          DistressSel = 0;

          if ((2 == NumOfCluster) || (4 == NumOfCluster)) {  // SNC-2 or SNC-4
            if (Ctr >= (MaxIMC / 2)) {  //Select 2nd distress wire for the 2nd half MCs
              DistressSel = 1;
            }
          }

          //
          // Set Crystal Ridge QoS settings - CfgEnableDistressSignalling
          //
          ProgramMesh2MemDistressSignallingForCrQos (Socket, Ctr, DdrtQosMode, DistressSel);
        }
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  Routine to program MSM BUS NUM and MCTP Eid information

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0                - Successful
  @retval Other            - failure

**/
KTI_STATUS
UpdateCpuBusAndMctpEidOnMsm (
  KTI_HOST_INTERNAL_GLOBAL    *KtiInternalGlobal
  )
{
  UINT32  RootBusEnableBitMap;
  UINT8   Index;
  UINT8   Idx1, Idx2;
  UINT8   StackPersonality;
  UINT8   MctpEid = 0;
  UINT8   SegmentId = 0;
  UINT8   MctpEidNidIndexPerSegment[MAX_SOCKET] = {0};
  UINT8   MctpSegIdSize = 0;
  UINT8   RootBus[MAX_IIO_STACK] = {0};
  UINT8   RootBusu0;
  UINT8   RootBusu1;

  //
  // The rule for MSM EID setup would be like this:
  // No segmentation:           <NID 7:5><0x1D 4:0>
  // 2 segments:                <SegID 7:7><NID&0x3 6:5><0x1D 4:0>
  // 4 segments:                <SegID 7:6><NID&0x1 5:5><0x1D 4:0>
  // 8 segments:                <SegID 7:5><0x1D 4:0>
  //

  //
  // Explicitly call out the legacy socket eid = 0x1D
  //
  OobSetEid (KtiInternalGlobal->SbspSoc, 0x1D);
  MctpEidNidIndexPerSegment[0]++;

  IioGetMctpSegIdSize (KtiInternalGlobal->SbspSoc, 0, &MctpSegIdSize);

  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if (SocketPresent (Index)) {
      ZeroMem (RootBus, sizeof(RootBus));
      RootBusEnableBitMap  = 0;

      //
      // Scan all valid iio stack and update root bus field
      //
      for (Idx1 = 0; Idx1 < MAX_IIO_STACK; Idx1++) {
        //
        // Check for valid resources
        //
        StackPersonality = KTIVAR->CpuInfo[Index].CpuRes.StackRes[Idx1].Personality;
        if ((StackPersonality == TYPE_UBOX_IIO)
          || (StackPersonality == TYPE_HFI)
          || (StackPersonality == TYPE_NAC)
          || (StackPersonality == TYPE_DINO))
        {
          if (KTIVAR->CpuInfo[Index].CpuRes.StackRes[Idx1].BusLimit >= KTIVAR->CpuInfo[Index].CpuRes.StackRes[Idx1].BusBase) {
            RootBusEnableBitMap  |= (1 << Idx1);
            RootBus[Idx1] = (UINT8) KTIVAR->CpuInfo[Index].CpuRes.StackRes[Idx1].BusBase;
          }
        }
      }

      RootBusu0 = KTIVAR->CpuInfo[Index].CpuRes.StackRes[UBOX_STACK].BusBase;
      RootBusu1 = KTIVAR->CpuInfo[Index].CpuRes.StackRes[UBOX_STACK].BusLimit;

      //
      // Program BUS_NO0/1/2, SOCKET_BUS_RANGE and BUS_ENABLE in MSM
      //
      OobSetBusNoRoot (Index, RootBusu0, RootBusu1, RootBus);
      OobSetCpuBusEnable (Index, RootBusEnableBitMap );
      OobSetSocketBusRange (Index, KTIVAR->CpuInfo[Index].CpuRes.BusBase,
                                KTIVAR->CpuInfo[Index].CpuRes.BusLimit,
                                KTIVAR->CpuInfo[Index].CpuRes.PciSegment);

      //
      // Update Mctp Eid information here
      //
      if (Index != KtiInternalGlobal->SbspSoc) {
        Idx2 = Index;
        SegmentId = KTIVAR->CpuInfo[Idx2].CpuRes.PciSegment;

        if (MctpSegIdSize) {
          //
          // Multi-segment case:
          //
          MctpEid = (SegmentId << (8 - MctpSegIdSize)) + \
                    (MctpEidNidIndexPerSegment [SegmentId] << 5) + 0x1D;
          MctpEidNidIndexPerSegment[SegmentId]++;
        } else {
          //
          // 1 segment case:
          //
          MctpEid = (MctpEidNidIndexPerSegment[0] << 5) + 0x1D;
          MctpEidNidIndexPerSegment[0]++;
        }
        OobSetEid (Index, MctpEid);
      }

    }
  }

  return KTI_SUCCESS;
}

/**
  Set SET_DMI_CFG_TYPE0 to one when stack zero of PBSP is configured as HOST bridge (DMI).

  DMI register can only be accessed after M2IOSF bank decoder is programmed by BIOS.

  @param[in] KtiInternalGlobal - Pointer to the KTI RC internal global structure

  @retval None

**/
VOID
SetDmiCfgType0 (
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  UINT8   Socket;
  UINT32  ClassCode;

  ClassCode = 0;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!SocketPresent (Socket)) {
      continue;
    }

    if (Socket == KtiInternalGlobal->SbspSoc) {
      continue;
    }

    IioGetClassCode (Socket, 0, &ClassCode);

    if (ClassCode == CCR_PCI_HOST_BRIDGE) {
      IioSetDmiCfgType0 (Socket, 0, 1);
    }
  }
}

/**
  Dump current bar assignments

  @param SocketData          - Pointer to socket specific data
  @param PrintMissingSockets - TRUE/FALSE

  @retval None

**/
VOID
DumpCurrentBarInfo (
  KTI_SOCKET_DATA         *SocketData,
  BOOLEAN                 PrintMissingSockets
  )
{
  UINT8 SocId, Ctr;

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\nCurrent Bars:"));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nSocket |     SCF    |     PCU    |    MEM0    |    MEM1    |    MEM2    |    MEM3    |    MEM4    |    MEM5    |    MEM6    |    MEM7    |    SBREG   |"));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n-------------------------------------------------------------------------------------------------------------------------------------------------------%a", ""));
  for (SocId = 0; SocId < MAX_SOCKET; SocId++) {
    if ((PrintMissingSockets || SocketData->Cpu[SocId].Valid == TRUE) && (SocketData->Cpu[SocId].SocType == SOCKET_TYPE_CPU)) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  %u    |", SocId));
      for (Ctr = 0; Ctr < TYPE_MAX_MMIO_BAR; Ctr++) {
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, " 0x%08X |", KTI_HOST_OUT_PTR->MmioBar[SocId][Ctr]));
      }
    }
  }
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n-------------------------------------------------------------------------------------------------------------------------------------------------------%a", "\n"));
  return;
}



/**
  Program Snc configuration in CHA before memory is ready

  @param SocketData          - Pointer to socket specific data
  @param KtiInternalGlobal   - KTIRC Internal Global data

  @retval KTI_SUCCESS

**/
KTI_STATUS
ProgramSncConfigureInChaBeforeMemoryReady (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8   Socket;
  UINT8   ChaId;
  UINT8   NumOfChaPerCluster;
  UINT8   NumOfCluster;
  UINT8   BaseChaOfCluster1;
  UINT8   BaseChaOfCluster2;
  UINT8   BaseChaOfCluster3;
  UINT8   FullSncEnable;
  UINT8   SncIndEnable;
  UINT8   NumClusters;
  UINT32  SncConfiguration;
  UINT8   UmaEn;
  UINT8   DefeatureXor;

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n ProgramSncConfigureInChaBeforeMemoryReady\n"));

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((SocketData->Cpu[Socket].Valid == TRUE) && (SocketData->Cpu[Socket].SocType == SOCKET_TYPE_CPU)) {
      Ms2IdiGetUncoreSncCfg (Socket, &NumOfChaPerCluster, &BaseChaOfCluster1, &BaseChaOfCluster2, &BaseChaOfCluster3);
      Ms2IdiGetFieldsSncCfg (Socket, &FullSncEnable, &SncIndEnable, &NumClusters);
      Ms2IdiGetFieldsUmaClusterCfg (Socket, &UmaEn, &DefeatureXor);
      if (NumClusters == 1) {
        NumOfCluster = 2;
      } else if (NumClusters == 3) {
        NumOfCluster = 4;
      } else {
        if (SncIndEnable == 0 && FullSncEnable == 0 && UmaEn == 0 && NumClusters == 0) {
          // Hardware default
          NumOfCluster = 0;
        } else {
          NumOfCluster = 1;
        }
      }
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n   Socket%d: NumOfCluster=%d, UmaEn=%d, BaseChaOfCluster1=%d,\
                           BaseChaOfCluster2=%d, BaseChaOfCluster3=%d\n", Socket, NumOfCluster, UmaEn, \
                           BaseChaOfCluster1, BaseChaOfCluster2, BaseChaOfCluster3));

      for (ChaId = 0; ChaId < KTIVAR->CpuInfo[Socket].TotCha; ChaId++) {
        ChaProgramSncConfig (
          Socket,
          ChaId,
          KTIVAR->CpuInfo[Socket].TotCha,
          NORMAL_OPERATION,
          UmaEn,
          NumOfCluster,
          BaseChaOfCluster1,
          BaseChaOfCluster2,
          BaseChaOfCluster3,
          &SncConfiguration
        );
      }
    }
  }

  return KTI_SUCCESS;
}

// max channel number:  MAX_CH = MC x CH, to cover ICX-SP, ICX-DE and SNR.
UINT16 McUnitId[4][3] = {
  {0, 1, 8},
  {2, 3, 9},
  {4, 5, 10},
  {6, 7, 11}
};

/**
  MmioWrite32 wrapper to print assembly code for emulation

  @param Address  - The MMIO register to write.
  @param Value    - The value to write to the MMIO register.

  @return Value.
**/
UINT32
EFIAPI
KtiMmioWrite32 (
  IN UINTN    Address,
  IN UINT32   Value
  )
{
  if (UbiosGenerationEnabled ()) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  mov DWORD PTR ds:[0%08xh], 0%08xh\n", Address, Value));
  }

  return MmioWrite32 (Address, Value);
}

/**
  Init PMon temp Bar 0

  @param SocketId                     - KtiInternalGlobal
  @param SocketId                     - Socket ID
  @param StatusCommandDefaultValuePtr - pointer to the power on default value of PMon PCICFG status and command register

**/
UINT32
InitOobMsmPmonBar (
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN UINT8                     SocketId,
  OUT BOOLEAN                  *CleanMsmBarNeeded,
  OUT UINT32                   *StatusCommandDefaultValuePtr
  )
{
  PCI_CFG_STATUS_COMMAND_DISC_MSM_PMON_STRUCT PmonCfgCommand;
  PCI_CFG_BAR0_DISC_MSM_PMON_STRUCT           PmonBar;
  UINT32                                      Data32;

  PmonBar.Data = KtiReadPciCfg (SocketId, 0, PCI_CFG_BAR0_DISC_MSM_PMON_REG);

  if ( (PmonBar.Data & 0xFFFFFFF0) != 0 ) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "PMon Bar0 already set\n"));
    *CleanMsmBarNeeded = FALSE;
    return PmonBar.Data;
  }

  PmonCfgCommand.Data = KtiReadPciCfg (SocketId, 0, PCI_CFG_STATUS_COMMAND_DISC_MSM_PMON_REG);
  *StatusCommandDefaultValuePtr = PmonCfgCommand.Data;
  PmonCfgCommand.Bits.memory_space_ena = 0;
  PmonCfgCommand.Bits.bus_master_ena = 0;
  KtiWritePciCfg (SocketId, 0, PCI_CFG_STATUS_COMMAND_DISC_MSM_PMON_REG, (UINT16) PmonCfgCommand.Data);

  PmonBar.Data = KtiReadPciCfg (SocketId, 0, PCI_CFG_BAR0_DISC_MSM_PMON_REG);
  KtiWritePciCfg (SocketId, 0, PCI_CFG_BAR0_DISC_MSM_PMON_REG, 0xFFFFFFFF);
  Data32 = KtiReadPciCfg (SocketId, 0, PCI_CFG_BAR0_DISC_MSM_PMON_REG);
  Data32 = Data32 & 0xFFFFFFF0;
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "PMon Bar0 required size: 0x%8X \n", (~Data32) + 1));

  //
  // MSM device is on Stack8 in SPR
  //
  Data32 = KTIVAR->CpuInfo[SocketId].CpuRes.StackRes[8].MmiolBase;
  PmonBar.Data = Data32 & 0xFFFFFFF0;
  KtiWritePciCfg (SocketId, 0, PCI_CFG_BAR0_DISC_MSM_PMON_REG, PmonBar.Data);

  *CleanMsmBarNeeded = TRUE;

  PmonCfgCommand.Data = KtiReadPciCfg (SocketId, 0, PCI_CFG_STATUS_COMMAND_DISC_MSM_PMON_REG);
  PmonCfgCommand.Bits.memory_space_ena = 1;
  PmonCfgCommand.Bits.bus_master_ena = 1;
  KtiWritePciCfg (SocketId, 0, PCI_CFG_STATUS_COMMAND_DISC_MSM_PMON_REG, (UINT16) PmonCfgCommand.Data);

  return PmonBar.Data;
}

/**
  Clear PMon temp Bar 0 and restore the PMon PCICFG status and command register value

  @param SocketId                  - Socket ID
  @param StatusCommandDefaultValue - the power on default value of PMon PCICFG status and command register

**/
VOID
ClearOobMsmPmonBar (
  IN UINT8    SocketId,
  IN UINT32   StatusCommandDefaultValue
  )
{
  PCI_CFG_STATUS_COMMAND_DISC_MSM_PMON_STRUCT PmonCfgCommand;
  PCI_CFG_BAR0_DISC_MSM_PMON_STRUCT           PmonBar;

  PmonCfgCommand.Data = KtiReadPciCfg (SocketId, 0, PCI_CFG_STATUS_COMMAND_DISC_MSM_PMON_REG);
  PmonCfgCommand.Bits.memory_space_ena = 0;
  PmonCfgCommand.Bits.bus_master_ena = 0;
  KtiWritePciCfg (SocketId, 0, PCI_CFG_STATUS_COMMAND_DISC_MSM_PMON_REG, (UINT16) PmonCfgCommand.Data);

  PmonBar.Data = KtiReadPciCfg (SocketId, 0, PCI_CFG_BAR0_DISC_MSM_PMON_REG);
  PmonBar.Bits.base_addr = 0;
  KtiWritePciCfg (SocketId, 0, PCI_CFG_BAR0_DISC_MSM_PMON_REG, PmonBar.Data);

  PmonCfgCommand.Data = StatusCommandDefaultValue;
  KtiWritePciCfg (SocketId, 0, PCI_CFG_STATUS_COMMAND_DISC_MSM_PMON_REG, (UINT16) PmonCfgCommand.Data);
  return;
}

/**
  Setup PMon discovery table registers on OOB-MSM

  @param SocketId              - Socket ID
  @param UnitType              - The unit type to be configured
  @param BlockId               - Block ID
  @param KtiInternalGlobal     - KTIRC Internal Global data
  @param UnitDiscoveryStatePtr - Pointer to Unit Discovery Unit Block structure

**/
KTI_STATUS
SetupPmonDiscoveryTableUnitBlock (
  IN     UINT8                     SocketId,
  IN     UINT8                     PmonIndex,
  IN     UNIT_TYPE                 UnitType,
  IN     KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN OUT UNIT_DISCOVERY_STATE      *UnitDiscoveryStatePtr
  )
{
  UINT64     MemBar0Cxl;
  UINT32     MmcfgBase;
  UINT32     BaseOffset;
  UINT8      UnitID;
  UINT8      McIndex;
  UINT8      ChannelIndex;
  UINT8      Ubox0Bus;
  UINT8      ChopType;
  KTI_STATUS Status;
  UNIT_DISCOVERY_STRUCT UnitDiscoveryDefault;

  ChopType   = KtiInternalGlobal->ChopType[KtiInternalGlobal->SbspSoc];
  UnitID     = OobMsmNewUnitDefaultValue (ChopType, UnitType, PmonIndex, &UnitDiscoveryDefault);

  if (UnitID == PMON_BLOCK_ID_INV || UnitType >= UNIT_TYPE_MAX) {
    return KTI_FAILURE;
  }

  if (UnitType == UNIT_TYPE_IAL_0 || UnitType == UNIT_TYPE_IAL_1) {
    if (UnitID < MAX_CXL_PER_SOCKET) {
      if (GetCxlStatus (SocketId, UnitID) != AlreadyInCxlMode) {
        return KTI_FAILURE;
      }
    } else {
      return KTI_FAILURE;
    }
  }

  KTI_ASSERT ((UnitDiscoveryDefault.UnitType == (UINT32)UnitType), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_87);

  UnitDiscoveryStatePtr->Fields.UnitType               = UnitDiscoveryDefault.UnitType;
  UnitDiscoveryStatePtr->RawData64[0].lo               = UnitDiscoveryDefault.UnitDiscovery0Low;
  UnitDiscoveryStatePtr->RawData64[0].hi               = UnitDiscoveryDefault.UnitDiscovery0High;
  UnitDiscoveryStatePtr->Fields.GlobalStatusPosition   = UnitDiscoveryDefault.GlobalStatPos;
  UnitDiscoveryStatePtr->Fields.UnitID                 = UnitID;
  UnitDiscoveryStatePtr->Fields.UnitCounterControlAddr = UnitDiscoveryDefault.BaseOffset;
  UnitDiscoveryStatePtr->Fields.BlockId                = UnitDiscoveryDefault.GlobalStatPos;

  BaseOffset = UnitDiscoveryDefault.BaseOffset;
  MmcfgBase  = KTIVAR->CpuInfo[SocketId].CpuRes.SegMmcfgBase.lo;
  Ubox0Bus   = GetSocketLastBus(SocketId) - 1;

  Status = KTI_SUCCESS;

  if ((UnitType == UNIT_TYPE_UPI_LL || UnitType == UNIT_TYPE_M3UPI) && UnitID >= KtiInternalGlobal->KtiPortCnt) {
     KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n Invalid UnitID=%u \n", UnitID));
     Status = KTI_FAILURE;
     return Status;
  }

  switch (UnitType) {
    case UNIT_TYPE_MC:
      McIndex = UnitID / 2;
      ChannelIndex = UnitID % 2;
      KTI_ASSERT ((McIndex < 4), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_7D);
      if (McIndex >= 4) {
        return KTI_FAILURE;
      }

      KTI_ASSERT (((2 + McIndex) <= TYPE_MEM_BAR7), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_83);
      if ((2 + McIndex) > TYPE_MEM_BAR7) {
        return KTI_FAILURE;
      }
      UnitDiscoveryStatePtr->Fields.UnitCounterControlAddr = (UINT64) KTIVAR->MmioBar[SocketId][2 + McIndex] + BaseOffset + ChannelIndex * 0x4000;
      UnitDiscoveryStatePtr->Fields.UnitID = McUnitId[McIndex][ChannelIndex];
      break;
    case UNIT_TYPE_M2M:
      UnitDiscoveryStatePtr->Fields.UnitCounterControlAddr = PCI_MMCFG_ADDRESS(MmcfgBase, Ubox0Bus, 12 + UnitID, 0, BaseOffset);
      break;
    case UNIT_TYPE_UPI_LL:
      if (KTIVAR->CpuInfo[SocketId].LepInfo[UnitID].Valid == TRUE) {
        UnitDiscoveryStatePtr->Fields.UnitCounterControlAddr = PCI_MMCFG_ADDRESS(MmcfgBase, Ubox0Bus,  1 + UnitID, 1, BaseOffset);
      }
      break;
    case UNIT_TYPE_M3UPI:
      if (KTIVAR->CpuInfo[SocketId].LepInfo[UnitID].Valid == TRUE) {
        UnitDiscoveryStatePtr->Fields.UnitCounterControlAddr = PCI_MMCFG_ADDRESS(MmcfgBase, Ubox0Bus,  5 + UnitID, 1, BaseOffset);
      }
      break;
    case UNIT_TYPE_IAL_0:
    case UNIT_TYPE_IAL_1:
      MemBar0Cxl = GetMemBar0 (SocketId, UnitID, CXL_PORT_RCRB_MEMBAR0_LOW_OFFSET);
      if (MemBar0Cxl == 0) {
        return KTI_FAILURE;
      }
      UnitDiscoveryStatePtr->Fields.UnitCounterControlAddr += MemBar0Cxl;
      break;
    case UNIT_TYPE_TC:
    case UNIT_TYPE_IRP:
    case UNIT_TYPE_M2PCIE:
      //
      // Validate prescence of Non-Ubox stacks
      //
      if (UnitID != 0) {
        if (!StackPresent (SocketId, UnitID-1)) {
          return KTI_FAILURE;
        }
      }
      break;
    case UNIT_TYPE_CHA:
      if (UnitID >= KTIVAR->CpuInfo[SocketId].TotCha) {
        return KTI_FAILURE;
      }
      break;
    //
    // Skip the UNIT_TYPE_PCIE for now, as there is no PMON register definition there
    //
  }
  return Status;
}

/**
  Dump the data in Global Discovery State address

  @param GlobalDiscoveryStateAddress  - Address of Global Discovery State

**/
VOID
DumpGlobalDiscoveryState (
  IN UINT32 GlobalDiscoveryStateAddress
  )
{
  UINT8 Index;
  GLOBAL_DISCOVERY_STATE  GlobalDiscoveryState;

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n Global Discovery State at address: 0x%08X \n", GlobalDiscoveryStateAddress));
  ZeroMem (&GlobalDiscoveryState , sizeof(GlobalDiscoveryState));
  //
  //Skip the block Id registers is not supposed to be accessed by BIOS .
  //
  for (Index = PMON_DISC_REG_0; Index < PMON_REG_MAX; Index++) {
    GlobalDiscoveryState.RawData64[Index].lo = MmioRead32 (GlobalDiscoveryStateAddress + Index * 8);
    GlobalDiscoveryState.RawData64[Index].hi = MmioRead32 (GlobalDiscoveryStateAddress + Index * 8 + 4);
  }

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "Access Type|Max Blocks|Block Stride|Type ||GlobalControlAddr||NumBlockStatusBits|GblCtrStatAddr \n"));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "[63:62]    |[25:16]   |[15:8]      |[7:0]||[63:0]           ||[23:8]            |[7:0] \n"));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "%d          |%d       |%d           |%d    ||0x%lX            ||%d                 |0x%X \n",
                                        GlobalDiscoveryState.Fields.AccessType,
                                        GlobalDiscoveryState.Fields.MaxBlocks,
                                        GlobalDiscoveryState.Fields.BlockStride,
                                        GlobalDiscoveryState.Fields.Type,

                                        GlobalDiscoveryState.Fields.GlobalControlAddr,

                                        GlobalDiscoveryState.Fields.NumBlockStatusBits,
                                        GlobalDiscoveryState.Fields.GblCtrStatAddr
  ));
}

VOID PrintType (UINT32 Type)
{
  switch (Type) {
  case UNIT_TYPE_CHA:
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n UNIT_TYPE_CHA \n"));
    break;
  case UNIT_TYPE_TC:
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n UNIT_TYPE_TC \n"));
    break;
  case UNIT_TYPE_IRP:
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n UNIT_TYPE_IRP \n"));
    break;
  case UNIT_TYPE_M2PCIE:
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n UNIT_TYPE_M2PCIE \n"));
    break;
  case UNIT_TYPE_PCU:
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n UNIT_TYPE_PCU \n"));
    break;
  case UNIT_TYPE_UBOX:
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n UNIT_TYPE_UBOX \n"));
    break;
  case UNIT_TYPE_MC:
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n UNIT_TYPE_MC \n"));
    break;
  case UNIT_TYPE_M2M:
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n UNIT_TYPE_M2M \n"));
    break;
  case UNIT_TYPE_UPI_LL:
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n UNIT_TYPE_UPI_LL \n"));
    break;
  case UNIT_TYPE_M3UPI:
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n UNIT_TYPE_M3UPI \n"));
    break;
  case UNIT_TYPE_PCIE:
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n UNIT_TYPE_PCIE \n"));
    break;
  case UNIT_TYPE_MDF_H:
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n UNIT_TYPE_MDF_H \n"));
    break;
  case UNIT_TYPE_MDF_V:
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n UNIT_TYPE_MDF_V \n"));
    break;
  case UNIT_TYPE_IAL_0:
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n UNIT_TYPE_IAL_0 \n"));
    break;
  case UNIT_TYPE_IAL_1:
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n UNIT_TYPE_IAL_1 \n"));
    break;
  }
}

/**
  Dump the data in Unit Discovery State address

  @param UnitDiscoveryStateAddress    - Address of Unit Discovery State

**/
VOID
DumpUnitDiscoveryState (
  IN UINT8                       SocketId,
  IN UINT32                      PmonBar,
  IN PMON_DISC_INFO              UnitTable[UNIT_TYPE_MAX],
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UNIT_TYPE             UnitType;
  UINT8                 Index;
  UINT8                 UnitIndex;
  UINT8                 ChopType;
  UINT32                PmonAddress, PmonOffset;
  UNIT_DISCOVERY_STATE  UnitDiscoveryState;

  ChopType   = KtiInternalGlobal->ChopType[KtiInternalGlobal->SbspSoc];

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n Unit Discovery State at address: 0x%08X \n", PmonBar + sizeof(GLOBAL_DISCOVERY_STATE)));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "Access Type|UnitStatus Offset|Counter0 Offset|Ctrl Width|Ctrl0 Offset|Num Ctrl Regs||UnitCounterControlAddr||Gbl Stat Pos|Unit ID|UnitType|Offset\n"));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "[63:62]    |[39:32]          |[31:24]        |[23:16]   |[15:8]      |[7:0]        ||[63:0]                ||[47:32]     |[31:16]|[15:0]|  \n"));

  for (UnitType = UNIT_TYPE_CHA; UnitType < UNIT_TYPE_MAX; UnitType++) {
    for (UnitIndex = 0; UnitIndex < UnitTable[UnitType].MaxSupportedUnits; UnitIndex++) {
      ZeroMem (&UnitDiscoveryState , sizeof(UnitDiscoveryState));
      PmonOffset = OobMsmGetPmonOffset (ChopType, UnitIndex, UnitType, UnitTable[UnitType]);
      if (PmonOffset != 0) {
        PmonAddress = PmonBar + PmonOffset;
        for (Index = PMON_DISC_REG_0; Index < PMON_REG_MAX; Index++) {
          UnitDiscoveryState.RawData64[Index].lo = MmioRead32 (PmonAddress + Index * 8);
          UnitDiscoveryState.RawData64[Index].hi = MmioRead32 (PmonAddress + Index * 8 + 4);
        }
        PrintType (UnitType);
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "%d          |0x%02X             |0x%2X           |%2d        |0x%2X        |%d            ||0x%8lX            ||%2d          |%2d     |%d|0x%4X \n",
                                              UnitDiscoveryState.Fields.AccessType,
                                              UnitDiscoveryState.Fields.UnitStatusAddr,
                                              UnitDiscoveryState.Fields.Counter0Addr,
                                              UnitDiscoveryState.Fields.CtrlWidth,
                                              UnitDiscoveryState.Fields.CtrlAddr,
                                              UnitDiscoveryState.Fields.ControlRegs,
                                              UnitDiscoveryState.Fields.UnitCounterControlAddr,
                                              UnitDiscoveryState.Fields.GlobalStatusPosition,
                                              UnitDiscoveryState.Fields.UnitID,
                                              UnitDiscoveryState.Fields.UnitType,
                                              PmonOffset
                            ));
      }
    }
  }
}


/**
  Program PMon discovery table registers on OOB-MSM

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocketId          - Socket

**/
KTI_STATUS
ProgramPmonDiscoveryTableOnOob (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocketId
  )
{
  UNIT_TYPE              UnitType;
  UINT8                  Index;
  UINT8                  UnitIndex;
  UINT8                  ChopType;
  UINT32                 PmonBar;
  UINT32                 PmonAddress, PmonOffset;
  UINT32                 StatusCommandDefaultValue;
  KTI_STATUS             Status;
  PMON_DISC_INFO         UnitTable[UNIT_TYPE_MAX];
  BOOLEAN                PmonSupported, GlobalPmonSupported;
  GLOBAL_DISCOVERY_STATE GlobalDiscoveryState;
  UNIT_DISCOVERY_STATE   UnitDiscoveryState;
  BOOLEAN                CleanMsmBarNeeded;

  CleanMsmBarNeeded = FALSE;

  PmonBar = InitOobMsmPmonBar (KtiInternalGlobal, SocketId, &CleanMsmBarNeeded, &StatusCommandDefaultValue);
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "PMon Bar0 address: 0x%08X \n", PmonBar));

  ZeroMem (&GlobalDiscoveryState , sizeof(GlobalDiscoveryState));
  ChopType   = KtiInternalGlobal->ChopType[KtiInternalGlobal->SbspSoc];
  PmonSupported       = OobMsmGetPmonOffsetTable (UnitTable, ChopType, KTIVAR->OutPmonConfig);
  GlobalPmonSupported = OobMsmGetPmonGlobalDiscDefault (ChopType, &GlobalDiscoveryState);
  if (PmonSupported == FALSE || GlobalPmonSupported == FALSE) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_WARN, "\n[WARNING]PMONs are not supported.\n"));
    if (CleanMsmBarNeeded) {
      ClearOobMsmPmonBar (SocketId, StatusCommandDefaultValue);
    }
    return KTI_SUCCESS;
  }

  PmonAddress = PmonBar;

  //
  //Skip the block Id registers is not supposed to be accessed by BIOS .
  //
  for (Index = PMON_DISC_REG_0; Index < PMON_REG_MAX; Index++) {
    KtiMmioWrite32 (PmonAddress + Index * 8,     GlobalDiscoveryState.RawData64[Index].lo);
    KtiMmioWrite32 (PmonAddress + Index * 8 + 4, GlobalDiscoveryState.RawData64[Index].hi);
  }
  DumpGlobalDiscoveryState (PmonBar);

  //
  // Program Unit Discovery State
  //
  for (UnitType = UNIT_TYPE_CHA; UnitType < UNIT_TYPE_MAX; UnitType++) {
    for (UnitIndex = 0; UnitIndex < UnitTable[UnitType].MaxSupportedUnits; UnitIndex++) {
      ZeroMem (&UnitDiscoveryState , sizeof(UnitDiscoveryState));
      Status = SetupPmonDiscoveryTableUnitBlock (SocketId, UnitIndex, UnitType, KtiInternalGlobal, &UnitDiscoveryState);
      if (Status == KTI_SUCCESS) {
        PmonOffset = OobMsmGetPmonOffset (ChopType, UnitIndex, UnitType, UnitTable[UnitType]);
        if (PmonOffset != 0) {
          PmonAddress = PmonBar +  PmonOffset;
          for (Index = PMON_DISC_REG_0; Index < PMON_REG_MAX; Index++) {
            KtiMmioWrite32 (PmonAddress + Index * 8,     UnitDiscoveryState.RawData64[Index].lo);
            KtiMmioWrite32 (PmonAddress + Index * 8 + 4, UnitDiscoveryState.RawData64[Index].hi);
          }
        }
      }
    }
  }

  DumpUnitDiscoveryState (SocketId, PmonBar, UnitTable, KtiInternalGlobal);
  if (CleanMsmBarNeeded) {
    ClearOobMsmPmonBar (SocketId, StatusCommandDefaultValue);
  }

  return KTI_SUCCESS;
}

/**
  Program PMon control access

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocketId          - Socket

**/
KTI_STATUS
ProgramPmonControlAccess (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocketId
  )
{
  return KTI_SUCCESS;
}

/**
  Check whether SPK supported or not.
  Currently Simcs not support SPK.

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocketId          - Socket

  @retval TRUE             - SPK supported
  @retval FALSE            - SPK not supported
**/
BOOLEAN
IsSpkSupported (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocketId
  )
{
  if ((GetEmulation() & SIMICS_FLAG) != 0) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
  Get SPK PCI BAR offset list according to CPU sub-Type

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SpkPciBarOffset   - Pointer to all SPK instances' PCI BAR list
  @param SpkInstanceNum    - SPK instance number

  @retval KTI_SUCCESS      - Success
          KTI_IGNORE       - SPK not supported
**/
KTI_STATUS
GetSpkInstances (
  IN  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  OUT UINT32                   **SpkPciBarOffset,
  OUT UINT32                   *SpkInstanceNum
  )
{
  KTI_STATUS Status = KTI_SUCCESS;

  switch (KtiInternalGlobal->CpuType) {
    case CPU_SPRSP:
      *SpkInstanceNum = sizeof (SprSpSpkInstancesBarOffset) / sizeof (SprSpSpkInstancesBarOffset[0]);
      *SpkPciBarOffset = SprSpSpkInstancesBarOffset;
      Status =  KTI_SUCCESS;
      break;

    default:
      *SpkInstanceNum = 0;
      *SpkPciBarOffset = NULL;
      Status = KTI_IGNORE;
      break;
  }

  return Status;
}

/**
  Program SNC registers on Sierra Peak

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocketId          - Socket

  @retval KTI_SUCCESS
**/
KTI_STATUS
ProgramSncRegistersOnSpk (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocketId
  )
{
  UINT32            SpkBar0;
  UINT32            StatusCommandDefaultValue;
  UINT32            Data32;
  UINT32            TempBar;
  UINT8             SncBaseIndex;
  BOOLEAN           SpkBar0Initialized;
  UINT8             NumChaPerCluster;
  UINT8             BaseChaCluster1;
  UINT8             BaseChaCluster2;
  UINT8             BaseChaCluster3;
  UINT8             UmaClusterEn;
  UINT8             XorDefeatur;
  UINT8             SpkIndex;
  UINT32            SpkInstanceNum;
  UINT32            SPK_PCI_BAR_OFFSET;
  UINT32            *SpkPciBarOffsetlst;

  GetSpkInstances (KtiInternalGlobal, &SpkPciBarOffsetlst, &SpkInstanceNum);

  if (SpkInstanceNum == 0) {
    return KTI_SUCCESS;
  }

  TempBar = KTIVAR->CpuInfo[SocketId].CpuRes.StackRes[0].MmiolBase;
  SpkBarInit (TempBar, SocketId, &SpkBar0Initialized, &StatusCommandDefaultValue, &SpkBar0);

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "SPK Bar0 address: 0x%08X \n", SpkBar0));

  for (SpkIndex = 0; SpkIndex < SpkInstanceNum; SpkIndex++) {
    SPK_PCI_BAR_OFFSET = *(SpkPciBarOffsetlst + SpkIndex);
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "SPK instance %d: BAR offset: 0x%08X \n", SpkIndex, SPK_PCI_BAR_OFFSET));
    //
    // SNC_BASE_1_IIO -> SPK_SNC_BASE1
    // SNC_BASE_2_IIO -> SPK_SNC_BASE2
    // SNC_BASE_3_IIO -> SPK_SNC_BASE3
    // SNC_BASE_4_IIO -> SPK_SNC_BASE4
    // SNC_BASE_5_IIO -> SPK_SNC_BASE5
    //
    for (SncBaseIndex = 0; SncBaseIndex < 5; SncBaseIndex++) {
      //
      // First read from IIO register, then write to SPK register
      //
      IioGetSncIdx (SocketId, 0, SncBaseIndex, &Data32);
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "SPK SNC Base Address %d: 0x%08X \n", (SncBaseIndex + 1), Data32));
      SpkSetStartingAddr (SpkBar0, SPK_PCI_BAR_OFFSET, SncBaseIndex, Data32);
    }

    // SPK_SNC_UPPER_BASE
    IioGetSncUpperBase (SocketId, 0, &Data32);
    SpkSetSncUpperBase (SpkBar0, SPK_PCI_BAR_OFFSET, Data32);
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "SPK SNC Upper Base Address: 0x%08X \n", Data32));

    // SPK_SNC_CONFIG
    Data32 = IioGetSncCfg (SocketId, 0);
    SpkSetSncCfg (SpkBar0, SPK_PCI_BAR_OFFSET, Data32);
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "SPK_SNC_CONFIG: 0x%08X \n", Data32));

    // SPK_UNCORE_SNC_CONFIG (SocketId, 0, &Data32);
    IioGetSncUncoreCfg (SocketId, 0, &NumChaPerCluster, &BaseChaCluster1, &BaseChaCluster2, &BaseChaCluster3);
    SpkSetSncUncoreCfg (SpkBar0, SPK_PCI_BAR_OFFSET, NumChaPerCluster, BaseChaCluster1, BaseChaCluster2, BaseChaCluster3);
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "SPK_UNCORE_SNC_CONFIG: NumChaPerCluster:%d, BaseChaCluster1:%d, BaseChaCluster2:%d, BaseChaCluster3:%d\n", NumChaPerCluster, BaseChaCluster1, BaseChaCluster2, BaseChaCluster3));

    // SPK_UMA_CLUSTER_CFG
    IioGetUmaClusterCfg (SocketId, 0, &UmaClusterEn, &XorDefeatur);
    SpkSetUmaClusterCfg (SpkBar0, SPK_PCI_BAR_OFFSET, UmaClusterEn, XorDefeatur);
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "SPK_UMA_CLUSTER_CFG: UmaClusterEn: %d, XorDefeatur: %d\n", UmaClusterEn, XorDefeatur));
  }

  //
  // clear bar if initialized in the routine
  //
  if (!SpkBar0Initialized) {
    ClearSpkBar (SocketId, StatusCommandDefaultValue);
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "Clear SPK temp BAR done\n"));
  }

  return KTI_SUCCESS;
}

/**
  Returns the total M2Pcie enabled for a given CPU socket.

  @param SocId             - Soc ID of the CPU who's enabled M2Pcie info is requested, 0...3
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval 0 - Successful
  @retval 1 - Failure

**/
KTI_STATUS
GetM2PcieCount (
  UINT8                      SocId,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  KtiInternalGlobal->M2PcieActiveBitmap[SocId] = KTI_HOST_OUT_PTR->M2PciePresentBitmap[SocId] = KTIVAR->CpuInfo[SocId].CpuRes.stackPresentBitmap;

  KTIVAR->CpuInfo[SocId].TotM2Pcie = KtiInternalGlobal->TotIio[SocId];

  KtiDebugPrintInfo0 ((KTI_DEBUG_WARN, "\nSocket %d m2pcie bit map:%x.\n", SocId, KTI_HOST_OUT_PTR->M2PciePresentBitmap[SocId]));

  KtiInternalGlobal->TotActiveM2Pcie[SocId] = KTIVAR->CpuInfo[SocId].TotM2Pcie;
  return KTI_SUCCESS;
}

/**
  Process PMC set_strap configuration.

  This routine is part of warm-reset elimination flow (i.e. skipping warm-reset
  on the cold-reset path). The VDM set_straps are the configuration straps
  transferred during Reset Phase 5 from PCH to CPU. Pcode will populate
  specific registers when processing set_strap message data registers. BIOS
  will read these registers and compare their values with BIOS setup options.
  If the registers match the setup option, the routine sets
  KtiInternalGlobal->SkipWarmResetRequest to TRUE. This routine covers
  SNC, TwoLM, UMA, Iio Disable set_strap settings.

  @param[in] SocketData             Pointer to socket specific data
  @param[in,out] KtiInternalGlobal  Pointer to KTIRC Internal Global data structure

  @retval KTI_SUCCESS

**/
KTI_STATUS
ProcessStrapsConfig (
  IN     KTI_SOCKET_DATA            *SocketData,
  IN OUT KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  if (KtiInternalGlobal->CurrentReset != POST_RESET_POWERGOOD) {
    return KTI_SUCCESS;
  }

  if (KTIVAR->WarmResetEliminationEn) {
    KtiInternalGlobal->SkipWarmResetRequest = TRUE;
    KtiDebugPrintInfo1 ((
      KTI_DEBUG_INFO1,
      "Attempt warm-reset elimination on the cold-reset.\n"
      ));
  }

  return KTI_SUCCESS;
}

/**
  Update struct with CPU Segment Type

  @param KtiInternalGlobal - KTIRC internal variables.
  @param SocId             - Soc ID of the CPU who's enabled Ras info is requested, 0...3

  @retval 0 - Successful

**/
KTI_STATUS
EFIAPI
GetCpuSegmentType (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId
  )
{
  return KTI_SUCCESS;
}

/**
  Routine to initialize SocketData

  @param[in]  KtiInternalGlobal   Pointer to the KTI RC internal global structure
  @param[out] SocketData          Pointer to socket specific data

  @retval KTI_SUCCESS

**/
KTI_STATUS
InitSocketData (
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  OUT KTI_SOCKET_DATA           *SocketData
  )
{
  UINT8 SocId, M3kti, Port;

  for (SocId = 0; SocId < MAX_SOCKET; SocId++) {
    for (M3kti = 0; M3kti < MAX_M3KTI; M3kti++) {
      SocketData->Route[SocId].M3KtiRt[M3kti] = 0x3 << (SocId * 2);
    }
    for (Port = 0; Port < (KtiInternalGlobal->KtiPortCnt); Port++) {
      SocketData->Route[SocId].KtiAgentRt[Port] = 0x3 << (SocId * 2);
    }
  }

  return KTI_SUCCESS;
}

/**
  Routine used to tell pbsp go to halt state before calling MULTI_SKT_UPD command

  @param[in]  KtiInternalGlobal -  Pointer to the KTI RC internal global structure
  @param[in]  Socket            -  Socket id

  @retval void

**/
VOID
TellPbspGotoHaltState (
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  UINT8                     Socket
  )
{
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    Dispatching command to notify Pbsp S%d to go to halt state", Socket));
  if (UBoxPipeDispatchData (Socket, 0, PIPE_DISPATCH_COMMAND_MULTI_SKT_UPD_GO_HLT, 0) == KTI_SUCCESS) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    Send the Pipe data to S%d Successfully!", Socket));
  }
}

/**
  Routine used to guarantee all pbsp goto halt state before calling MULTI_SKT_UPD command
  or check pbsp resume from halt state after pcode resume them

  @param[in]  KtiInternalGlobal -  Pointer to the KTI RC internal global structure
  @param[in]  Socket            -  Socket id
  @param[in]  Status            -  1: Enter to halt state 0: resume from halt state

  @retval void

**/
VOID
WaitPbspHaltStateChange (
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  UINT8                     Socket,
  IN  UINT8                     Status
  )
{
  UINT32  WaitTime;
  BOOLEAN InHltState;
  UINT8   *RegAddr;
  UINT32  LabelNumber;
  UINTN   RegVal;

  WaitTime = 5000;
  UBoxInHltStateIsSetInScratchpad (Socket, 0, &InHltState);
  while ((InHltState != Status) && (WaitTime > 0)) {
    KtiFixedDelay (1000); // 1000 microsecs
    WaitTime = WaitTime - 1;
    UBoxInHltStateIsSetInScratchpad (Socket, 0, &InHltState);
  }
  if (GetUbiosLoopbackLabelNumber (&LabelNumber) == EFI_SUCCESS) {
    UBoxGetBiosNonStickyScratchPadRegisterFlatAddr (Socket, 0, 2, &RegVal);
    RegAddr = (UINT8 *)RegVal;
    if (KtiInternalGlobal->CurrentReset == POST_RESET_POWERGOOD) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nLABEL_c_mbox%d:", LabelNumber));
    } else {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nLABEL_w_mbox%d:", LabelNumber));
    }
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nmov eax, DWORD PTR ds:[0%8xh]", RegAddr));
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nbt eax, 1"));
    if (KtiInternalGlobal->CurrentReset == POST_RESET_POWERGOOD) {
      if (Status == 1) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\njnc LABEL_c_mbox%d\n", LabelNumber));
      } else {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\njc LABEL_c_mbox%d\n", LabelNumber));
      }
    } else {
      if (Status == 1) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\njnc LABEL_w_mbox%d\n", LabelNumber));
      } else {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\njc LABEL_w_mbox%d\n", LabelNumber));
      }
    }
    LabelNumber++;
    SetUbiosLoopbackLabelNumber (LabelNumber);
  }
  //
  // Handle the failure
  //
  if ((WaitTime == 0) && (InHltState != Status)) {
    KtiCheckPoint ((UINT8)(1 << KtiInternalGlobal->SbspSoc), SOCKET_TYPE_CPU, 0xFF, ERR_BOOT_MODE, MINOR_ERR_PBSP_ENTER_HLT_FAILURE);
    KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n\n  PBSP: S%d its status is %a, but should be %a !!!", Socket, (InHltState == 0) ? "active" : "halt", (Status == 0) ? "active" : "halt"));
    KTI_ASSERT (FALSE, ERR_BOOT_MODE, MINOR_ERR_PBSP_ENTER_HLT_FAILURE);
  }
}

/**
  Send command to pcode, it issues the MULTI_SKT_UPD_REQ to its connected PCH
  over PMDWN.

  @param[in]  KtiInternalGlobal   Pointer to the KTI RC internal global structure
  @param[in]  UpdFlow: 0 - MDFIS Training
                       1 - Late action register update
  @param[in]  MinRatio: BIOS_MIN_RATIO
  @param[in]  MaxRatio: BIOS_MAX_RATIO

  @retval KTI_SUCCESS
  @retval KTI_IGNORE

**/
static
KTI_STATUS
SendMultiSktUpdatePcodeMailBox (
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN MULTI_SKT_UPT_TYPE        UpdFlow,
  IN UINT8                     MinRatio,
  IN UINT8                     MaxRatio
  )
{
  UINT8   Socket;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent (Socket) && (Socket != KtiInternalGlobal->SbspSoc)) {
      TellPbspGotoHaltState (KtiInternalGlobal, Socket);
    }
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent (Socket) && (Socket != KtiInternalGlobal->SbspSoc)) {
      WaitPbspHaltStateChange (KtiInternalGlobal, Socket, 1);
    }
  }

  if (UpdFlow == MULTI_SKT_UPT_MDFIS_TRAINING) {
    //
    // BIOS sends command to Pcode to stop mesh traffic and relock Mesh PLL to the requested frequency.
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Mdfs training send command to pcode\n"));
    PcuTriggerMdfisTraining (KtiInternalGlobal->SbspSoc, MinRatio, MaxRatio);
  } else {
    //
    // Trigger on legacy socket
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Trigger late action credit update without issuing warm-reset\n"));
    PcuTriggerLateActionRegUpdate (KtiInternalGlobal->SbspSoc);
  }

  //
  // Loop through all PBSPs to make sure Pbsp resume from halt state
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  All PBSPs resume from halt %s\n", "START"));
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent (Socket) && Socket != KtiInternalGlobal->SbspSoc) {
      WaitPbspHaltStateChange (KtiInternalGlobal, Socket, 0);
    }
  }
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  All PBSPs resume from halt %s\n", "END"));

  return KTI_SUCCESS;
}

/**
  Routine to trigger MDFIS advanced training

  @param[in]  KtiInternalGlobal   Pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS
  @retval KTI_IGNORE

**/
KTI_STATUS
ConfigureMDFIAvanacedTraining (
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  UINT8   MinRatio;
  UINT8   MaxRatio;
  UINT8   Socket;
  UINT8   MdfisInstanceNum = 40;
  UINT8   Index = 0;
  UINT32  OriginalValInFsmCtrl0 = 0;

  if (KtiInternalGlobal->DieCount[KtiInternalGlobal->SbspSoc] <= 1) {
    return KTI_IGNORE;
  }

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n******* Mdfs Training %a *******\n", "START"));
  //
  // MinRatio and MaxRatio to programm
  //
  MinRatio = 23;
  MaxRatio = 27;
  //
  // The golden sequence from MDFI_Training_HAS_1p0
  //
  UINT8 StdaloneGlodenSequences[] = {
                                      STDALONE_TX_DCC,
                                    };
  UINT8 Die2DieGlolenSequences[] = {
                                     DIE2DIE_QDS_SLOOP,
                                     DIE2DIE_RX_VREF,
                                     DIE2DIE_D2C_LLOOP,
                                     DIE2DIE_QDS_SLOOP,
                                     DIE2DIE_QDS_SLOOP,
                                     DIE2DIE_QDS_SLOOP,
                                     DIE2DIE_QDS_SLOOP,
                                   };

  //
  // 1.BIOS configures CR's inside MDFIS needed for advanced training
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!SocketPresent (Socket)) {
      continue;
    }
    for (Index = 0; Index < MdfisInstanceNum; Index++) {
      //
      // Program standalone sequences
      //
      MdfsStandaloneSequences (
        Socket,
        Index,
        sizeof (StdaloneGlodenSequences) / sizeof (StdaloneGlodenSequences[0]),
        StdaloneGlodenSequences
      );

      //
      // Program die2die sequences
      //
      MdfsDie2DieSequences (
        Socket,
        Index,
        sizeof (Die2DieGlolenSequences) / sizeof (Die2DieGlolenSequences[0]),
        Die2DieGlolenSequences
      );

      //
      // Program seq_reg2 to indicate it will do bios assisted advanced training
      //
      MdfsSetSeqRegSelectInFsmCtrl0 (
        Socket,
        Index,
        SEQ_REG2_BIOS_ASSISTED,
        &OriginalValInFsmCtrl0
      );
    }
  }

  //
  // 2. BIOS sends command to Pcode to stop mesh traffic and relock Mesh PLL to the requested frequency.
  //
  SendMultiSktUpdatePcodeMailBox (KtiInternalGlobal, MULTI_SKT_UPT_MDFIS_TRAINING, MinRatio, MaxRatio);
  //
  // 3. Restore original values
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!SocketPresent (Socket)) {
      continue;
    }
    for (Index = 0; Index < MdfisInstanceNum; Index++) {
      MdfsSetFsmCtrl0 (
        Socket,
        Index,
        OriginalValInFsmCtrl0
      );
    }
  }
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n******* Mdfs Training %a *******\n", "END  "));

  return KTI_SUCCESS;
}

/**
  Switch core and mesh ratios.

  @param[in]    CoreRatio       - The desired Core ratio
  @param[in]    MeshRatio       - The desired Mesh ratio

  @retval       KTI_SUCCESS
  @retval       KTI_IGNORE
**/
KTI_STATUS
SwitchCoreMeshRatio (
  IN  UINT8     CoreRatio,
  IN  UINT8     MeshRatio
  )
{
  UINT8   Socket;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!SocketPresent (Socket)) {
      continue;
    }

    PcuSetCoreMeshRatio (Socket, CoreRatio, MeshRatio);
  }

  return KTI_SUCCESS;
}

/**
  Get bitmask to determine which BGF pointer separation need to be programmed

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocketId          - Socket ID

  @retval Bitmask to indicate which BGF pointer separation need to be programmed
          It can be the combination of the following values:
             - PTRSEP_OVERRIDE_BGF_M3KTI_KTI
             - PTRSEP_OVERRIDE_BGF_M2PCIE
             - PTRSEP_OVERRIDE_BGF_M2M
**/
UINT8
BgfPtrOverrideSupported (
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN  UINT8                     SocketId
  )
{
  //
  // SPR does not require BGF PTR separation override
  //
  return 0;
}

/**

  Trigger late action credit reload without issuing a warm-reset.

  @param[in] KtiInternalGlobal    Pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS   Successful completion.
  @retval KTI_IGNORE    Triggering late action credit update is not needed.

**/
KTI_STATUS
LateActionCreditUpdateWithoutWarmReset (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  if (!KtiInternalGlobal->SkipWarmResetRequest) {
    //
    // Since BIOS is doing the normal flow (that is, issuing warm-reset), the
    // mailbox method to trigger credit reload is not needed.
    //
    return KTI_IGNORE;
  }

  SendMultiSktUpdatePcodeMailBox (KtiInternalGlobal, MULTI_SKT_UPT_LATE_ACTION_REG_UPDATE, 0, 0);

  return KTI_SUCCESS;
}

/**

  Init Upi Affinity Status

  @param[in] KtiInternalGlobal    Pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS   Successful completion.
  @retval KTI_IGNORE

**/
KTI_STATUS
InitUpiAffinityStatus (
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal)
{
  KTIVAR->UpiAffinityEn = FALSE;

  return KTI_SUCCESS;
}

/**
  This routine returns legacy stack IO Size (in number of SAD Targets).
  Note: Total SAD targets is 32.

  @param[in] KtiInternalGlobal    Pointer to the KTI RC internal global structure

  @retval  Number of SAD Targets for legacy stack IO Size.

**/
UINT8
GetLegacyStackIoSize (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  return SAD_LEG_IO_PCH_SIZE;
}

/**
  Implement the si_wa, just after MMCFG are programmed for all the sockets.

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval
**/
VOID
ProgramMiscSiWa (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8     Socket, Ctr;
  UINT32    BlSharedCdtThreshold, AdSharedCdtThreshold;
  BOOLEAN   Wa0Enabled, Wa1Enabled;

  Wa0Enabled = IsSiliconWorkaroundEnabled ("S2208908676");
  Wa1Enabled = IsSiliconWorkaroundEnabled ("S14010101993");

  if (Wa0Enabled || Wa1Enabled) {
    //
    // Shadow the code in Cache to avoid fetching code from PCH to avoid inbound completions, and
    // before outbound faf enable (iiomiscctrl_1_5_0_CFG.irp_ob_ncs_faf_en) and before SAD programming
    //
    if(UbiosGenerationEnabled()) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n;FafWA Start"));
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n call GetEip"));
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n call FafWa"));
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n;FafWA End\n"));
    }
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!SocketPresent (Socket)) {
      continue;
    }
    for (Ctr = 0; Ctr < MAX_IIO_STACK; Ctr++) {
      if ((KTI_HOST_OUT_PTR->M2PciePresentBitmap[Socket] & (1 << Ctr)) == 0) {
        continue;
      }
      //
      // Prior to DFX restricted register access, it needs to implement unlock action.
      //
      IioProgramDfxRegisterAccessControl (Socket, Ctr, TRUE);

      if (Wa0Enabled) {
        BlSharedCdtThreshold = 0x12;
        IioSetBlSharedCdtThreshold (Socket, Ctr, BlSharedCdtThreshold);
      }

      if (Wa1Enabled) {
        AdSharedCdtThreshold = 0x1e;
        IioSetAdSharedCdtThreshold (Socket, Ctr, AdSharedCdtThreshold);
      }

      if (IsSiliconWorkaroundEnabled ("S2208908874")) {
        //
        // SPR A0, A1, B0 & C0 WA, clear the spare2 (Bit25 of IRP_MISC_DFX0), which is in SV_HOOK.
        //
        IioClearIrpMiscDfx0Spare2 (Socket, Ctr);
      }
      //
      // After DFX restricted register access, it needs to get back to Lock state.
      //
      IioProgramDfxRegisterAccessControl (Socket, Ctr, FALSE);
    }
  }

  if (IsSiliconWorkaroundEnabled ("S14011724136")) {
    //
    // program the iiomiscctrl_1_5_0_CFG.irp_ob_ncs_faf_en to 0 for legacy DMI stack (stack 0 of Sbsp)
    //
    IioClearMiscCtrlN1Ncs(KtiInternalGlobal->SbspSoc, 0);
  }
}

/**
  Get max Mesh2Mem supported

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval Max M2M supported
**/
UINT8
GetMaxM2mSupported (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8      MaxM2mem;

  if (KTIVAR->HbmSku) {
    MaxM2mem = MAX_SPR_HBM_M2MEM; //SPR+HBM
  } else {
    MaxM2mem = MAX_SPR_M2MEM; //SPR
  }

  return MaxM2mem;
}

/**
  This routine assigns MMIOL resources to all supported Ubox BAR regions.

  For hot plug systems, resources will be preallocated.

  @param[in] SocketData                CPU related info
  @param[in, out] KtiInternalGlobal    KTIRC internal variables.

  @retval KTI_SUCCESS    Ubox BAR regions are assigned with MMIOL resources.
  @retval KTI_IGNORE     Unsupported feature.

**/
KTI_STATUS
CalculateUboxMmioResources (
  IN     KTI_SOCKET_DATA            *SocketData,
  IN OUT KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8          Idx1;
  UINT8          Stack;
  UINT8          Ctr;
  UINT8          UboxMmioBarSpaceIndex;

  //
  // Find which stack is last stack, minus 8M for ubox mmio and assign mmio bar data structure
  //
  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (SocketData->Cpu[Idx1].SocType != SOCKET_TYPE_CPU) {
      continue;
    }

    if ((SocketData->Cpu[Idx1].Valid == FALSE) && (KtiInternalGlobal->KtiCpuSktHotPlugEn == FALSE)) {
      continue;
    }

    for (Stack = 0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
      if (KTIVAR->CpuInfo[Idx1].CpuRes.MmiolLimit != KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiolLimit) {
        continue;
      }

      KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiolLimit = KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiolLimit - UBOX_MMIO_BAR_SIZE;
      KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[UBOX_STACK].MmiolBase  = KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[Stack].MmiolLimit + 1;
      KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[UBOX_STACK].MmiolLimit = KTIVAR->CpuInfo[Idx1].CpuRes.MmiolLimit;
      KTIVAR->CpuInfo[Idx1].CpuRes.UboxMmioBase = KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[UBOX_STACK].MmiolBase;
      KTIVAR->CpuInfo[Idx1].CpuRes.UboxMmioLimit = KTIVAR->CpuInfo[Idx1].CpuRes.StackRes[UBOX_STACK].MmiolBase + UBOX_MMIO_BAR_SIZE - 1;

      //
      // assign ubox mmio bar space, each bar has 512K
      //
      UboxMmioBarSpaceIndex = 0;
      for (Ctr = 0; Ctr < TYPE_MAX_MMIO_BAR; Ctr++) {
        KTI_HOST_OUT_PTR->MmioBar[Idx1][Ctr] = KTIVAR->CpuInfo[Idx1].CpuRes.UboxMmioBase + UBOX_MMIO_SIZE_PER_BAR * UboxMmioBarSpaceIndex;
        if (Ctr == TYPE_SCF_BAR) {
          //
          // For SPR, SCF BAR map increased to 2M, so 4*UBOX_MMIO_SIZE_PER_BAR allocated for it
          //
          UboxMmioBarSpaceIndex += 4;
        } else {
          UboxMmioBarSpaceIndex += 1;
        }
      }
    }
  }

  return KTI_SUCCESS;
}
