/** @file

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

//
// Include files
//
#include <Uefi.h>
#include "Include/UboxRegs.h"
#include <Library/UsraCsrLib.h>
#include "SysHost.h"
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/RcDebugLib.h>
#include <ScratchpadList.h>
#include <Upi/KtiHost.h>
#include <Library/UpiHostDataLib.h>
#include <Library/EmulationConfigurationLib.h>

#define UBOX_SCRATCHPAD_BOX_INSTANCE  0

UINT32  BiosnonStickyScratchPadRegistersOff[] = {
  BIOSNONSTICKYSCRATCHPAD0_UBOX_MISC_REG,
  BIOSNONSTICKYSCRATCHPAD1_UBOX_MISC_REG,
  BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG,
  BIOSNONSTICKYSCRATCHPAD3_UBOX_MISC_REG,
  BIOSNONSTICKYSCRATCHPAD4_UBOX_MISC_REG,
  BIOSNONSTICKYSCRATCHPAD5_UBOX_MISC_REG,
  BIOSNONSTICKYSCRATCHPAD6_UBOX_MISC_REG,
  BIOSNONSTICKYSCRATCHPAD7_UBOX_MISC_REG,
  BIOSNONSTICKYSCRATCHPAD8_UBOX_MISC_REG,
  BIOSNONSTICKYSCRATCHPAD9_UBOX_MISC_REG,
  BIOSNONSTICKYSCRATCHPAD10_UBOX_MISC_REG,
  BIOSNONSTICKYSCRATCHPAD11_UBOX_MISC_REG,
  BIOSNONSTICKYSCRATCHPAD12_UBOX_MISC_REG,
  BIOSNONSTICKYSCRATCHPAD13_UBOX_MISC_REG,
  BIOSNONSTICKYSCRATCHPAD14_UBOX_MISC_REG,
  BIOSNONSTICKYSCRATCHPAD15_UBOX_MISC_REG
  };

/**
  UBOX: Check warm boot flag set or not

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param bWarmBootFlagSet - Check WarmBoot Flag Set in Scratchpad or not

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxCheckWarmBootFlagSetInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* bWarmBootFlagSet
  )
{
  BIOS_SCRATCHPAD0_STRUCT Sp0;

  Sp0.Data = UsraCsrRead (SocId, BoxInst, BIOSSCRATCHPAD0_UBOX_MISC_REG);

  if (Sp0.Bits.WarmBoot == 0) {
    *bWarmBootFlagSet = FALSE;
  } else {
    *bWarmBootFlagSet = TRUE;
  }

  return EFI_SUCCESS;
}

/**
  UBOX: Check flag in scratch pad1 register to see whether Fpga Initialized

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param bFpgaInitialied  - Whether FPGA initialized or not

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxCheckFpgasInitialized (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* bFpgaInitialied
  )
{
  BIOS_SCRATCHPAD1_STRUCT Sp1;
  //
  // if Blue Bitstream already downloaded, return true
  //
  Sp1.Data = UsraCsrRead (SocId, BoxInst, BIOSSCRATCHPAD1_UBOX_MISC_REG);

  if (Sp1.Bits.FpgaBitStreamLoaded == 1) {
    *bFpgaInitialied = TRUE;
  } else {
    *bFpgaInitialied = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  UBOX: Check warm boot flag set or not

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param Flag             - 0 or 1

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSetResetAllowedFlagInScratchpad (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Flag
  )
{
  BIOS_SCRATCHPAD1_STRUCT    Sp1;

  Sp1.Data = UsraCsrRead (SocId, BoxInst, BIOSSCRATCHPAD1_UBOX_MISC_REG);
  Sp1.Bits.ResetAllowed = Flag;
  UsraCsrWrite (SocId, BoxInst, BIOSSCRATCHPAD1_UBOX_MISC_REG, Sp1.Data);

  return EFI_SUCCESS;
}

/**
  UBOX: Get Kti Topology in BIOS sticky scratchpad

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param KtiTop           - Return the Kti topology in UBox reg

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxGetKtiTopologyInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT64*  KtiTop
  )
{
  *KtiTop = UsraCsrRead (SocId, BoxInst, BIOSSCRATCHPAD2_UBOX_MISC_REG);

  return EFI_SUCCESS;
}

/**
  UBOX: Set Kti Topology in BIOS sticky scratchpad

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param KtiTopology      - Kti Topology

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

  **/
EFI_STATUS
EFIAPI
UBoxSetKtiTopologyInScratchpad (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT64   KtiTopology
  )
{
  UsraCsrWrite (SocId, BoxInst, BIOSSCRATCHPAD2_UBOX_MISC_REG, (UINT32) (KtiTopology & 0xFFFFFFFF));

  return EFI_SUCCESS;
}

/**
  UBOX: Update CPU List in Scratchpad register

  @param SocId            - CPU Socket Node number (Socket ID)
  @param AllCpus          - TRUE: update for all CPUs, FALSE: only update for socket "SocId"
  @param *SocketData      - Socket Data

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

  **/
EFI_STATUS
EFIAPI
UBoxUpdateCpuListInScratchpad (
  IN UINT8           SocId,
  IN BOOLEAN         AllCpus,
  IN KTI_SOCKET_DATA *SocketData
  )
{
  UINT8                           Index, Port;
  UINT32                          Sr02;

  //
  // Update the Cpu List
  // [07:00]   - CPU list; this list reflects the CPUs after the topology is degraded, if needed
  //
  // Update the TopologyBitmap
  // Each socket has KtiPortCnt bits
  // [(KtiPortCnt - 1)   + 8:               8] - link bit mask for socket 0
  // [(2*KtiPortCnt - 1) + 8:  KtiPortCnt + 8] - link bit mask for socket 1
  // ....
  // [(7*KtiPortCnt - 1) + 8:6*KtiPortCnt + 8] - link bit mask for socket 6
  // [(8*KtiPortCnt - 1) + 8:7*KtiPortCnt + 8] - link bit mask for socket 7
  //
  Sr02 = 0;
  for (Index = 0; Index < MAX_SOCKET; ++Index) {
    if (SocketData->Cpu[Index].Valid == TRUE) {
      //
      // bit[7:0] used for CPU list
      //
      Sr02 |= (1 << Index);
      for (Port = 0; Port < KTI_HOST_OUT_PTR->KtiPortCnt; ++Port) {
          if (SocketData->Cpu[Index].LinkData[Port].Valid == TRUE) {
            Sr02 = Sr02 | (1 << (((Index * KTI_HOST_OUT_PTR->KtiPortCnt) + Port) + 8));
          }
      }
    }
  }

  if (AllCpus) {
    //
    // Update it on all CPUs populated
    //
    for (Index = 0; Index < MAX_SOCKET; ++Index) {
      if (SocketData->Cpu[Index].Valid == TRUE){
        UsraCsrWrite (Index, 0, BIOSSCRATCHPAD2_UBOX_MISC_REG, Sr02);
      }
    }
  } else {
    //
    // Only on Onlined CPU socket
    //
    UsraCsrWrite (SocId, 0, BIOSSCRATCHPAD2_UBOX_MISC_REG, Sr02);
  }

  return EFI_SUCCESS;
}

/**
  UBOX: Store the following in Sticky scratchpad 7 bit [30] - Set SNC Mode Enable failure/recovery flag

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param Flag             - 0 or 1

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

  **/
EFI_STATUS
EFIAPI
UBoxSetSncModeFailRecoveryFlagInScratchpad (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Flag
  )
{
  BIOS_SCRATCHPAD7_STRUCT Sp7;

  //
  // Store the following in Sticky scratchpad 7 bit [30] - Set SNC Mode Enable failure/recovery flag
  //
  Sp7.Data = UsraCsrRead (SocId, BoxInst, BIOSSCRATCHPAD7_UBOX_MISC_REG);
  Sp7.Bits.SncFailRecovery = Flag;
  UsraCsrWrite (SocId, BoxInst, BIOSSCRATCHPAD7_UBOX_MISC_REG, Sp7.Data);

  return EFI_SUCCESS;
}

/**
  UBOX: Store the following in Sticky scratchpad 7 bit [20] - Set Prefetch Mode Enable failure/recovery flag

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param Flag             - 0 or 1

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSetPrefetchModeFailRecoveryFlagInScratchpad (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Flag
  )
{
  BIOS_SCRATCHPAD7_STRUCT Sp7;

  //
  // Store the following in Sticky scratchpad 7 bit [20] - Set Prefetch Mode Enable failure/recovery flag
  //
  Sp7.Data = UsraCsrRead (SocId, BoxInst, BIOSSCRATCHPAD7_UBOX_MISC_REG);
  Sp7.Bits.PrefetchFailRecovery = Flag;
  UsraCsrWrite (SocId, BoxInst, BIOSSCRATCHPAD7_UBOX_MISC_REG, Sp7.Data);

  return EFI_SUCCESS;
}

/**
  UBOX: Store the following in Sticky scratchpad 7 bit [22:21]
        Set UMA based clustering downgrade

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param Downgrade        - 0/1/2/3
                            0:default; 1: Quad-> Hemi 2: Quad->Disable 3: Hemi->Disable
  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSetUmaBasedClusteringDowngradeInScratchpad (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Downgrade
  )
{
  BIOS_SCRATCHPAD7_STRUCT Sp7;

  Sp7.Data = UsraCsrRead (SocId, BoxInst, BIOSSCRATCHPAD7_UBOX_MISC_REG);
  Sp7.Bits.UmaBasedClusteringDowngrade = Downgrade;
  UsraCsrWrite (SocId, BoxInst, BIOSSCRATCHPAD7_UBOX_MISC_REG, Sp7.Data);

  return EFI_SUCCESS;
}

/**
  UBOX: Get UMA based clustering downgrade from
        Sticky scratchpad 7 bit [22:21]

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param Downgrade        - 0/1/2/3
                            0:default; 1: Quad-> Hemi 2: Quad->Disable 3: Hemi->Disable
  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxGetUmaBasedClusteringDowngradeInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT8    *Downgrade
  )
{
  BIOS_SCRATCHPAD7_STRUCT Sp7;

  Sp7.Data = UsraCsrRead (SocId, BoxInst, BIOSSCRATCHPAD7_UBOX_MISC_REG);
  *Downgrade = (UINT8)Sp7.Bits.UmaBasedClusteringDowngrade;

  return EFI_SUCCESS;
}

/**
  UBOX: Update SBSP SocId in Scratchpad register

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param SbspSocId        - SBSP SocId

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

  **/
EFI_STATUS
EFIAPI
UBoxUpdateSbspSocIdInScratchpad (
  IN UINT8           SocId,
  IN UINT8           BoxInst,
  IN UINT8           SbspSocId
  )
{
  BIOS_SCRATCHPAD7_STRUCT              Sp7;

  //
  // Update the Ubox sticky scratch register 07:
  // [19:16]  - SBSP socket id
  //
  Sp7.Data = UsraCsrRead (SocId, BoxInst, BIOSSCRATCHPAD7_UBOX_MISC_REG);
  Sp7.Bits.SbspSocketId = SbspSocId;
  UsraCsrWrite (SocId, BoxInst, BIOSSCRATCHPAD7_UBOX_MISC_REG, Sp7.Data);

  return EFI_SUCCESS;
}

/**
  UBOX: Check SncModeFailRecovery flag set or not

  @param SocId                - CPU Socket Node number (Socket ID)
  @param BoxInst              - Box Instance, 0 based
  @param SncModeFailReccovery - SNC mode fail recovery set or not

  @retval                     - EFI_UNSUPPORTED: the function not implemented
                                EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxCheckSncModeFailRecoveryFlagSetInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* SncModeFailReccovery
  )
{
  BIOS_SCRATCHPAD7_STRUCT Sp7;

  Sp7.Data = UsraCsrRead (SocId, BoxInst, BIOSSCRATCHPAD7_UBOX_MISC_REG);

  if (Sp7.Bits.SncFailRecovery == 0) {
    *SncModeFailReccovery = FALSE;
  } else {
    *SncModeFailReccovery = TRUE;
  }

  return EFI_SUCCESS;
}

/**
  UBOX: Check PrefetchModeFailRecovery flag set or not

  @param SocId                    - CPU Socket Node number (Socket ID)
  @param BoxInst                  - Box Instance, 0 based
  @param PrefetchModeFailRecovery - SNC mode fail recovery set or not

  @retval                         - EFI_UNSUPPORTED: the function not implemented
                                    EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxCheckPrefetchModeFailRecoveryFlagSetInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* PrefetchModeFailReccovery
  )
{
  BIOS_SCRATCHPAD7_STRUCT Sp7;

  Sp7.Data = UsraCsrRead (SocId, BoxInst, BIOSSCRATCHPAD7_UBOX_MISC_REG);

  if (Sp7.Bits.PrefetchFailRecovery == 0) {
    *PrefetchModeFailReccovery = FALSE;
  } else {
    *PrefetchModeFailReccovery = TRUE;
  }

  return EFI_SUCCESS;
}

/**
  UBOX: Set RemoteSocketReleased flag in Scratchpad

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param Flag             - 0 or 1

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

  **/
EFI_STATUS
EFIAPI
UBoxSetRemoteSocketReleasedFlagInScratchpad (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Flag
  )
{
  BIOS_SCRATCHPAD7_STRUCT              Sp7;

  Sp7.Data = UsraCsrRead (SocId, BoxInst, BIOSSCRATCHPAD7_UBOX_MISC_REG);
  Sp7.Bits.RemoteSocketReleased = Flag;
  UsraCsrWrite (SocId, BoxInst, BIOSSCRATCHPAD7_UBOX_MISC_REG, Sp7.Data);

  return EFI_SUCCESS;
}

/**
  UBOX: Check CompletedColdResetFlow flag set or not

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param bCmpColdReset    - Complete cold reset or not

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxCheckCompletedColdResetFlowFlagSetInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* bCmpColdReset
  )
{
  BIOS_SCRATCHPAD7_STRUCT             Sp7;

  //
  // Extract the current system reset type from Sticky Scratch 7 (updated by CPURC)
  //
  Sp7.Data = UsraCsrRead (SocId, BoxInst, BIOSSCRATCHPAD7_UBOX_MISC_REG);
  if (Sp7.Bits.CompletedColdResetFlow == 1) {
    *bCmpColdReset = TRUE;
  } else {
    *bCmpColdReset = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  UBOX: Check AepDimmPresent flag set or not

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @bAepDimmPresent        - AEP DIMM present or not

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxCheckAepDimmPresentFlagSetInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* bAepDimmPresent
  )
{
  BIOS_SCRATCHPAD7_STRUCT BiosScratchPad7;

  //
  // If AEP DIMM present (indicated bit StickyPad7[31])
  //
  BiosScratchPad7.Data = UsraCsrRead (SocId, BoxInst, BIOSSCRATCHPAD7_UBOX_MISC_REG);
  if (BiosScratchPad7.Bits.AepDimmPresent == 1) {
    *bAepDimmPresent = TRUE;
  } else {
    *bAepDimmPresent = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  UBOX: Set Cold reset flag in Scratchpad

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param Flag             - 0 or 1

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

  **/
EFI_STATUS
EFIAPI
UBoxSetColdResetFlowFlagInScratchpad (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    Flag
  )
{
  BIOS_SCRATCHPAD7_STRUCT  Sp7;

  //
  // Warm reset has to be passed to CPU RC code via scratchpad7 bit5.
  //
  Sp7.Data = UsraCsrRead (SocId, BoxInst, BIOSSCRATCHPAD7_UBOX_MISC_REG);
  Sp7.Bits.CompletedColdResetFlow = Flag;
  UsraCsrWrite (SocId, BoxInst, BIOSSCRATCHPAD7_UBOX_MISC_REG, Sp7.Data);

  return EFI_SUCCESS;
}

//
// NonStickyScratchpad
//

/**
  UBOX: Get Bios Non-Sticky ScratchPad Registers' value

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param RegIndex         - Non-Sticky ScratchPad Register index
  @param RegValue         - Register value

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxGetBiosNonStickyScratchPadRegisterValue (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  IN  UINT8    RegIndex,
  OUT UINT32*  RegValue
  )
{
  *RegValue = UsraCsrRead (SocId, BoxInst, BiosnonStickyScratchPadRegistersOff [RegIndex]);

  return EFI_SUCCESS;
}

/**
  UBOX: Set Bios Non-Sticky ScratchPad Registers' value

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param RegIndex         - Non-Sticky ScratchPad Register index
  @param RegValue         - Register value

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

  **/
EFI_STATUS
EFIAPI
UBoxSetBiosNonStickyScratchPadRegisterValue (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT8    RegIndex,
  IN UINT32   RegValue
  )
{
  UsraCsrWrite (SocId, BoxInst, BiosnonStickyScratchPadRegistersOff [RegIndex], RegValue);

  return EFI_SUCCESS;
}

/**
  UBOX: Get Bios Non-Sticky ScratchPad Registers' flat address

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param RegIndex         - Non-Sticky ScratchPad Register index
  @param RegVal           - Register value

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxGetBiosNonStickyScratchPadRegisterFlatAddr (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  IN  UINT8    RegIndex,
  OUT UINTN    *RegVal
  )
{
  *RegVal = UsraGetCsrRegisterAddress (SocId, BoxInst, BiosnonStickyScratchPadRegistersOff [RegIndex]);

  return EFI_SUCCESS;
}

/**

  Computes address of BIOSNONSTICKYSCRATCHPAD10_UBOX_MISC_REG

  @param[in] SocId      - CPU Socket Node number
  @param[in] BoxInst    - Box Instance, 0 based
  @param[in, out] Size  - Ptr to register size in bytes (may be updated if pseudo-offset)
  @param[out] Address   - Return the address

  @retval               - EFI_UNSUPPORTED: the function not implemented
                          EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxGetBiosNonStickyScratchPad10CsrAddress (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT8    *Size,
  OUT UINT64   *Address
  )
{
  *Address = (UINT64) UsraGetCsrRegisterAddress (SocId, BoxInst, BIOSNONSTICKYSCRATCHPAD10_UBOX_MISC_REG);

  return EFI_SUCCESS;
}

/**
  UBOX: Get CPU Stepping in BIOS sticky scratchpad

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param LlcBist          - LLC BITS Result

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxGetLlcBistResultsInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT32*  LlcBist
  )
{
  BIOS_NON_STICKY_SCRATCHPAD0_STRUCT Sp0;

  Sp0.Data = UsraCsrRead (SocId, BoxInst, BIOSNONSTICKYSCRATCHPAD0_UBOX_MISC_REG);

  *LlcBist = Sp0.Bits.LlcBistResults; // Bits [15:8] should be 0x2 (LLC BIST complete) before the socket can be accessed.

  return EFI_SUCCESS;
}

/**
  UBOX: Issue BOOT_GO command

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

  **/
EFI_STATUS
EFIAPI
UBoxIssueBootGoCommand (
  IN UINT8    SocId,
  IN UINT8    BoxInst
  )
{
  BIOS_NON_STICKY_SCRATCHPAD1_STRUCT   Nssp1;

  //
  // Issue BOOT_GO command
  //
  Nssp1.Data = UsraCsrRead (SocId, BoxInst, BIOSNONSTICKYSCRATCHPAD1_UBOX_MISC_REG);
  Nssp1.Bits.ReqToPcode = BOOT_GO_CMD;
  UsraCsrWrite (SocId, BoxInst, BIOSNONSTICKYSCRATCHPAD1_UBOX_MISC_REG, Nssp1.Data);

  return EFI_SUCCESS;
}

/**
  UBOX: Get CPU Stepping in BIOS sticky scratchpad

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param CpuStepping      - CPU Stepping in BIOS sticky scratchpad

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxGetCpuSteppingInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT8*   CpuStepping
  )
{
  BIOS_NON_STICKY_SCRATCHPAD2_STRUCT  Sp2;

  Sp2.Data = UsraCsrRead (SocId, BoxInst, BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG);
  //
  // CSR bit[30:27] = CPU Stepping
  //
  *CpuStepping = (UINT8) (Sp2.Bits.CpuStepping);

  return EFI_SUCCESS;
}

/**
  UBOX: Read CheckInBit to determine if AP has checked-in

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param bCheckInBitsSet  - CheckIn Bits set or not

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxCheckInBitsIsSetInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* bCheckInBitsSet
  )
{
  BIOS_NON_STICKY_SCRATCHPAD2_STRUCT  Sp2;

  Sp2.Data = UsraCsrRead (SocId, BoxInst, BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG);

  if (Sp2.Bits.CheckInBit != 0) {
    *bCheckInBitsSet = TRUE;
  } else {
    *bCheckInBitsSet = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  UBOX: Read InHltState to determine if AP enter halt state

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param bCheckInBitsSet  - CheckIn Bits set or not

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxInHltStateIsSetInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT BOOLEAN* bSetInHltState
  )
{
  BIOS_NON_STICKY_SCRATCHPAD2_STRUCT  Sp2;

  Sp2.Data = UsraCsrRead (SocId, BoxInst, BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG);

  if (Sp2.Bits.InHltState != 0) {
    *bSetInHltState = TRUE;
  } else {
    *bSetInHltState = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  UBOX: Clear the flag in scratchpad register which indicate
        PBSP in hlt state which is used for MDFS training

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxClearPbspInHltStateFlagInScratchpad (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst
  )
{
  BIOS_NON_STICKY_SCRATCHPAD2_STRUCT  Sp2;

  Sp2.Data = UsraCsrRead (SocId, BoxInst, BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG);
  Sp2.Bits.InHltState = 0;
  UsraCsrWrite (SocId, BoxInst, BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG, Sp2.Data);

  return EFI_SUCCESS;
}

/**
  UBOX: Check whether socket needs BootGo

  @param SocId            - CPU Socket Node number (Socket ID)
  @param RequireBootGo    - Check whether socket SocId needs BootGo

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxCheckSocketRequireBootGo (
  IN  UINT8    SocId,
  OUT BOOLEAN* RequireBootGo
  )
{
  UINT32  Data32 = 0;

  //
  // Check if socket fetched code
  //
  Data32 = UsraCsrRead (SocId, 0, BIOSNONSTICKYSCRATCHPAD0_UBOX_MISC_REG);
  // Bits [15:8] should be 0x2 (LLC BIST complete) before the socket can be accessed.
  // Bits [7:0] are 0xF8 indicating in SSP loop
  if ((Data32 & 0xFFFF) == BOOTGO_REQUIRED) {
    *RequireBootGo = TRUE;
  } else {
    *RequireBootGo = FALSE;
  }

  return EFI_SUCCESS;
}


/**
  UBOX: Store Link Fix Status

  @param SocId            - CPU Socket Node number (Socket ID)
  @param LinkFixStatus    - Link Fix Status

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxStoreLinkFixStatus (
  IN UINT8    SocId,
  IN UINT32   LinkFixStatus
  )
{
  return EFI_UNSUPPORTED;
}

/**
  UBOX: Set link failure status

  @param SocId            - CPU Socket Node number (Socket ID)
  @param bLinkFailure     - TRUE: Link Failure
                            FALSE: Link success

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSetLinkFailureStatus (
  IN UINT8    SocId,
  IN BOOLEAN  bLinkFailure
  )
{
  return EFI_UNSUPPORTED;
}

/**
  UBOX: Record slow mode WA reset times

  @param SocId            - CPU Socket Node number (Socket ID)

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxRecordSlowModeWARstTimes (
  IN UINT8    SocId
  )
{
  return EFI_UNSUPPORTED;
}

/**
  UBOX: Set/Clear boot flow enters cold reset flag in Sticky BIOS Scratchpad register

  @param SocId            - CPU Socket Node number (Socket ID)
  @param Operation        - 1: set the flag
                            0: clear the flag

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSetBiosEnterColdRstFlowFlag (
  IN UINT8    SocId,
  IN UINT8    Operation
  )
{
  return EFI_UNSUPPORTED;
}

/**
  UBOX: Send the Dispatch PIPE and Data PIPE via BIOSNONSTICKYSCRATCHPAD13 & BIOSNONSTICKYSCRATCHPAD15
  BIOSNONSTICKYSCRATCHPAD13 --- Dispatch PIPE
  BIOSNONSTICKYSCRATCHPAD15 --- Data PIPE

  @param SocId              - CPU Socket Node number (Socket ID)
  @param BoxInst            - Box Instance, 0 based
  @param DispatchPipeData   - Dispatch info for NBSP
  @param DataPipeData       - Data info for NBSP

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

  **/
EFI_STATUS
EFIAPI
UBoxPipeDispatchData (
  IN UINT8    SocId,
  IN UINT8    BoxInst,
  IN UINT32   DispatchPipeData,
  IN UINT32   DataPipeData
  )
{
  BIOSNONSTICKYSCRATCHPAD13_UBOX_MISC_STRUCT  NonSticky13;
  BIOSNONSTICKYSCRATCHPAD15_UBOX_MISC_STRUCT  NonSticky15;
  UINTN   RegVal = 0;

  NonSticky13.Data = DispatchPipeData;
  NonSticky15.Data = DataPipeData;

  //
  // Write the PIPE data first, then send the dispatch command to NBSP.
  //
  UsraCsrWrite (SocId, 0, BIOSNONSTICKYSCRATCHPAD15_UBOX_MISC_REG, NonSticky15.Data);
  UsraCsrWrite (SocId, 0, BIOSNONSTICKYSCRATCHPAD13_UBOX_MISC_REG, NonSticky13.Data);

  if (UbiosGenerationEnabled()) {
    UBoxGetBiosNonStickyScratchPadRegisterFlatAddr (SocId, 0, 15, &RegVal);
    DEBUG ((EFI_D_ERROR, "\n  mov DWORD PTR ds:[0%08xh], 0%08xh\n", RegVal, NonSticky15.Data));

    UBoxGetBiosNonStickyScratchPadRegisterFlatAddr (SocId, 0, 13, &RegVal);
    DEBUG ((EFI_D_ERROR, "\n  mov DWORD PTR ds:[0%08xh], 0%08xh\n", RegVal, NonSticky13.Data));
  }
  return EFI_SUCCESS;
}

/**
  UBOX: Get Kti Topology status in BIOS sticky lockbypass register

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param TopoType         - Topology Type: 0b=DPW; 1b=AFC
  @param TopoTypeValid    - Topology Type valid or not: 0b=Invalid; 1b=Valid

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxGetKtiTopologyStatusInLockbypassReg (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  OUT UINT8    *TopoType,
  OUT UINT8    *TopoTypeValid
  )
{
  return EFI_UNSUPPORTED;
}

/**
  UBOX: Set Kti Topology status in BIOS sticky lockbypass register

  @param SocId            - CPU Socket Node number (Socket ID)
  @param BoxInst          - Box Instance, 0 based
  @param TopoType         - Topology Type: 0b=DPW; 1b=AFC
  @param TopoTypeValid    - Topology Type valid or not: 0b=Invalid; 1b=Valid

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS:     the function is excuted successfully

**/
EFI_STATUS
EFIAPI
UBoxSetKtiTopologyStatusInLockbypassReg (
  IN  UINT8    SocId,
  IN  UINT8    BoxInst,
  IN  UINT8    TopoType,
  IN  UINT8    TopoTypeValid
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get the "completion delay reset count" value.

  @param[in]  Socket  The socket to read the count from.
  @param[out] Count   The "completion delay reset count" value.

  @retval   EFI_SUCCESS       The "completion delay reset count" value was successfully retrieved.
            EFI_UNSUPPORTED   Getting the "completion delay reset count" is not supported.
**/
EFI_STATUS
EFIAPI
UBoxGetCompletionDelayResetCount (
  IN  UINT8   Socket,
  OUT UINT32  *Count
  )
{
  BIOS_SCRATCHPAD7_STRUCT BiosScratchPad7;

  BiosScratchPad7.Data = UsraCsrRead (Socket, UBOX_SCRATCHPAD_BOX_INSTANCE, BIOSSCRATCHPAD7_UBOX_MISC_REG);
  *Count = BiosScratchPad7.Bits.CompletionDelayResetCount;

  return EFI_SUCCESS;
}

/**
  Set the "completion delay reset count" value.

  @param[in]  Socket  The socket to write the count to.
  @param[in]  Count   The "completion delay reset count" value.

  @retval   EFI_ACCESS_DENIED   The "completion delay reset count" value could not be set (such as due to the value
                                being greater than what is supported).
            EFI_SUCCESS         The "completion delay reset count" value was successfully set.
            EFI_UNSUPPORTED     Setting the "completion delay reset count" is not supported.
**/
EFI_STATUS
EFIAPI
UBoxSetCompletionDelayResetCount (
  IN  UINT8   Socket,
  IN  UINT32  Count
  )
{
  BIOS_SCRATCHPAD7_STRUCT BiosScratchPad7;
  EFI_STATUS              Status = EFI_ACCESS_DENIED;

  if (Count < (1 << BIOS_SCRATCHPAD7_COMPLETION_DELAY_RESET_COUNT_SIZE)) {
    BiosScratchPad7.Data = UsraCsrRead (Socket, UBOX_SCRATCHPAD_BOX_INSTANCE, BIOSSCRATCHPAD7_UBOX_MISC_REG);
    BiosScratchPad7.Bits.CompletionDelayResetCount = Count;
    UsraCsrWrite (Socket, UBOX_SCRATCHPAD_BOX_INSTANCE, BIOSSCRATCHPAD7_UBOX_MISC_REG, BiosScratchPad7.Data);
    Status = EFI_SUCCESS;
  }

  return Status;
}
