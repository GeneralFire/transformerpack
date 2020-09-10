/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include <Library/VictimAggresorTableLib.h>
#include "Chip10nm/Include/RcRegs10nm.h"
#include <Library/ProcMemInitChipLib.h>
#include "Chip10nm/Include/MemHostChip10nm.h"
#include <Library/MemDdrioIpLib.h>
#include <Library/MemRcLib.h>

#include <Library/MemMcIpLib.h>
#include "Include/MemIoControl.h"
#include "Include/MemDdrioRegs.h"
#include <Library/SystemInfoLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SwizzleLib.h>

#define UNASSIGNED 0xFF
//
// PXCCBB initial values
//
#define PXCCBB_PXC_EN           1
#define PXCCBB_PXC_AND_DDJC_EN  3

// Local function prototypes
/**

  This function sets default attackers for both attacker0select and attacker1select.

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket
  @param[in] Ch         - Channel

@retval SUCCESS

**/
STATIC
UINT32
PxcAttackerInit (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch
  );


/**
  Return the initial value for PXCCBB based on the current configuration

  @param[in]    Host    - Pointer to SysHost

  @retval The value to initialize PXCCBB to

**/
UINT32
GetPxcCbbInitValue (
  IN     PSYSHOST          Host
  )
{
  UINT8     Socket;
  UINT8     PxcEnable   = 0;
  UINT8     DdjcEnable  = 0;
  UINT32    PxcCcbb     = 0;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();
  Socket = Host->var.mem.currentSocket;
  //
  // Return setting for usepxccbb based on the current configuration
  //
  // 2'bX0 = disable both PXC + DDJC
  // 2'bX1 for enabling PXC
  // 2'b11 For enabling DDJC
  // 2'b11 For enabling both DDJC + PXC
  //

  if (GetTrainingStepEnableChip (Socket, CHECKPOINT_MINOR_PXC_TRAINING) || (Setup->mem.ForcePxcInit != 0)) {
    PxcEnable = 1;
  }

  if (GetTrainingStepEnableChip (Socket, CHECKPOINT_MINOR_DDJC_TRAINING)) {
    DdjcEnable = 1;
  }

  if ((PxcEnable == 1) || (DdjcEnable == 1)) {
    PxcCcbb = PXCCBB_PXC_EN;
    if (DdjcEnable == 1) {
      PxcCcbb = PXCCBB_PXC_AND_DDJC_EN;
    }
  }

  return PxcCcbb;
}


/**

  This function performs DDRIO initialization related to Phase Based Cross-talk Cancellation (PXC)

  @param[in]  Host           - Pointer to sysHost
  @param[in]  Socket         - Socket
  @param[in]  Ch             - Channel
  @param[in]  PxcEnDisable   - Enable/disable PXC

  @retval none

**/
VOID
PxcSetupDdrio (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     PxcEnDisable
  )
{
  UINT8                                 Strobe;
  struct channelNvram                   (*ChannelNvList)[MAX_CH];
  DATACONTROL1N0_0_MCIO_DDRIO_STRUCT    DataControl1;

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Strobe = 0; Strobe < MAX_STROBE; Strobe++) {
    //
    // Skip if this is an ECC Strobe when ECC is disabled
    //
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }

    DataControl1.Data = (*ChannelNvList)[Ch].dataControl1[Strobe];

    DataControl1.Bits.usepxccbb = GetPxcCbbInitValue (Host);

    //
    // Disable xtalkpienable for now - will be enabled when PXC training executes
    //
    DataControl1.Bits.xtalkpienable = 0;

    DataControl1.Bits.xtalkpisign = 1;

    (*ChannelNvList)[Ch].dataControl1[Strobe] = DataControl1.Data;
    WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL1N0_0_MCIO_DDRIO_REG, DataControl1.Data);
  } //Strobe

} //PxcSetupDdrio

/**

  This function sets default attackers for both attacker0select and attacker1select.

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket
  @param[in] Ch         - Channel

  @retval SUCCESS

**/
STATIC
UINT32
PxcAttackerInit (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch
  )
{
  UINT8               Strobe;
  UINT8               MSVx8 = GetMaxStrobeValid (Host) / 2;
  PXC_ATTACKER_SEL    PxcAttackerN0;
  PXC_ATTACKER_SEL    PxcAttackerN1;
  UINT8               SwizzleBit[MAX_BITS_IN_BYTE];
  UINT8               StrobeN[MAX_NIBBLES_PER_BYTE];
  UINT8               Index;

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Program default values for PXC Attackers\n");

  for (Strobe = 0; Strobe < MSVx8; Strobe++) {

    StrobeN[INDEX_NIBBLE0] = Strobe;
    StrobeN[INDEX_NIBBLE1] = Strobe + MSVx8;

    for (Index = 0; Index < MAX_BITS_IN_BYTE; Index++) {
      SwizzleBit[Index] = GetDqSwizzle (Ch, StrobeN[INDEX_NIBBLE0], Index, DdrLevel, PxcDelta);
    }
    //
    // PXC is disabled when the attackers for a bit in a byte are set to the bit number
    // For strobes 0-8, the bit numbers are 0-3
    // For strobes 9-17, the bit numbers are 4-7
    // GetSetPxcAttackSel() swizzles the strobes before accessing the registers. Use the
    // swizzled strobe number in the calculations below.
    //

    //
    // Nibble 0
    //
    PxcAttackerN0.Bits.AttackerBit0 = SwizzleBit[0];
    PxcAttackerN0.Bits.AttackerBit1 = SwizzleBit[1];
    PxcAttackerN0.Bits.AttackerBit2 = SwizzleBit[2];
    PxcAttackerN0.Bits.AttackerBit3 = SwizzleBit[3];
    //
    // Nibble 1
    //
    PxcAttackerN1.Bits.AttackerBit0 = SwizzleBit[4];
    PxcAttackerN1.Bits.AttackerBit1 = SwizzleBit[5];
    PxcAttackerN1.Bits.AttackerBit2 = SwizzleBit[6];
    PxcAttackerN1.Bits.AttackerBit3 = SwizzleBit[7];

    GetSetPxcAttackSel (Host, Socket, Ch, Strobe, GSM_FORCE_WRITE, PxcAttackerN0, PxcAttackerN1);
  } // Strobe

  return SUCCESS;
} // PxcAttackerInit

/**

  This function programs PxcDelta from the NVRAM cache settings

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket
  @param[in] Ch         - Channel

  @retval SUCCESS

**/
STATIC
UINT32
PxcDeltaRestore (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch
  )
{
  UINT8                                 Dimm;
  UINT8                                 Rank;
  UINT8                                 LogRank;
  UINT8                                 Strobe;
  XTALKDELTARANK0_MCIO_DDRIO_STRUCT     XtalkDeltaN0;
  XTALKDELTARANK0_MCIO_DDRIO_STRUCT     XtalkDeltaN1;
  struct channelNvram                   (*ChannelNvList)[MAX_CH];
  struct dimmNvram                      (*DimmNvList)[MAX_DIMM];

  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Skip if no DIMM present
    //
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }
    for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
      //
      // Skip if no rank
      //
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }
      //
      // Get the logical rank #
      //
      LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

      for (Strobe = 0; Strobe < MAX_STROBE/2; Strobe++) {
        //
        // Read from cache and write to HW
        //
        XtalkDeltaN0.Data = (UINT32)(*ChannelNvList)[Ch].XtalkDeltaN0[LogRank][Strobe % (MAX_STROBE / 2)];
        XtalkDeltaN1.Data = (UINT32)(*ChannelNvList)[Ch].XtalkDeltaN1[LogRank][(Strobe % (MAX_STROBE / 2)) + (MAX_STROBE / 2)];
        WriteDdrioDataReg (Host, Socket, Ch, LogRank, (Strobe % (MAX_STROBE / 2)), XTALKDELTARANK0_MCIO_DDRIO_REG, XtalkDeltaN0.Data);
        WriteDdrioDataReg (Host, Socket, Ch, LogRank, (Strobe % (MAX_STROBE / 2)) + (MAX_STROBE / 2), XTALKDELTARANK0_MCIO_DDRIO_REG, XtalkDeltaN1.Data);
      } // Strobe
    } // Rank
  } // Dimm

  return SUCCESS;
} // PxcDeltaRestore

/**

  Sets aggressor bits for each bit lane

  @param[in]  Host         - Pointer to sysHost
  @param[in]  Socket       - Socket
  @param[in]  Ch           - Channel

  @retval SUCCESS

**/
UINT32
PxcProgramAggressors (
  IN     PSYSHOST        Host,
  IN     UINT8           Socket,
  IN     UINT8           Ch
  )
{
  UINT8                   DqBit;
  UINT8                   AggressorDq;
  UINT8                   AggressorBit;
  UINT8                   AggressorByte;
  UINT8                   DqBitRegVictim = 0;
  UINT8                   SwizzledAggressorDq = 0;
  UINT8                   BitIndex;
  UINT8                   Byte;
  PXC_ATTACKER_SEL        PxcAttackerN0;
  PXC_ATTACKER_SEL        PxcAttackerN1;
  PXC_VIC_AGGR            *VicAggrProg = NULL;
  UINT8                   MSVx8 = GetMaxStrobeValid (Host) / 2;

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Program assignment values for PXC Attackers\n");

  GetVictimAggressorTable (&VicAggrProg);

  PxcAttackerN0.Data = 0;
  PxcAttackerN1.Data = 0;

  for (Byte = 0; Byte < MSVx8; Byte++) {
    for (DqBit = 0; DqBit <= 7; DqBit++) {
      //
      // Index of the victim
      //
      BitIndex = Byte * MAX_BITS_IN_BYTE + DqBit;

      //
      // Because of swizzling, we have to figure out which register bit corresponds to which table bit
      // This converts the physical lane on the package to a bit in the register
      // Note: The register swizzling is performed inside GetSetPxcAttackSel
      //
      DqBitRegVictim = DqBit;
      AggressorDq = VicAggrProg->vicAggrLookup[Ch][BitIndex][PxcAttacker1];
      AggressorBit = AggressorDq % MAX_BITS_IN_BYTE;
      AggressorByte = AggressorDq / MAX_BITS_IN_BYTE;
      //
      // The aggressor swizzing is required at this point since the swizzling performed at GetSetPxcAttackSel
      // does take care of the register mapping only
      //
      SwizzledAggressorDq = GetDqSwizzle (Ch, AggressorByte, AggressorBit, DdrLevel, PxcControl);

      //
      // If the Aggressor is not UNASSIGNED...
      //
      if (VicAggrProg->vicAggrLookup[Ch][BitIndex][PxcAttacker1] != UNASSIGNED) {
        if (DqBitRegVictim < BITS_PER_NIBBLE) {
          //
          // Clear the bits corresponding to the Aggressor DQ, keeping the previous assignments
          //
          PxcAttackerN0.Data &= ~(0x7 << (3 * DqBitRegVictim));
          //
          // Perform the assignment of the Aggressor DQ
          //
          PxcAttackerN0.Data |= (SwizzledAggressorDq << (3 * DqBitRegVictim));
        } else {
          //
          // Clear the bits corresponding to the Aggressor DQ, keeping the previous assignments
          //
          PxcAttackerN1.Data &= ~(0x7 << (3 * (DqBitRegVictim - BITS_PER_NIBBLE)));
          //
          // Perform the assignment of the Aggressor DQ
          //
          PxcAttackerN1.Data |= (SwizzledAggressorDq << (3 * (DqBitRegVictim - BITS_PER_NIBBLE)));
        }
      }
    } // DqBit

    //
    // Program new set of values for attackr0 and attackr1
    //
    GetSetPxcAttackSel (Host, Socket, Ch, Byte, GSM_FORCE_WRITE, PxcAttackerN0, PxcAttackerN1);
  } // Strobe

  return SUCCESS;
} //PxcProgramAggressors

/**
  This function sets capacitive/inductive values.

  @param[in]  Host         - Pointer to sysHost
  @param[in]  Socket       - Socket
  @param[in]  Ch           - Channel

  @retval SUCCESS

**/
UINT32
PxcProgramCapInd (
  IN     PSYSHOST               Host,
  IN     UINT8                  Socket,
  IN     UINT8                  Ch
  )
{
  UINT8                                 Strobe = 0;
  PXC_CAP_IND_SETTING                   *CapIndProg = NULL;
  DATACONTROL1N0_0_MCIO_DDRIO_STRUCT    DataControl1;
  struct channelNvram                   (*ChannelNvList)[MAX_CH];
  UINT8                                 MSVx4 = GetMaxStrobeValid (Host);

  ChannelNvList = GetChannelNvList (Host, Socket);

  GetCapacitiveInductiveTable (&CapIndProg);

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    //
    // Skip if this is an ECC strobe when ECC is disabled
    //
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }

    //
    // Read cached values
    //
    DataControl1.Data = (*ChannelNvList)[Ch].dataControl1[Strobe];

    //
    // Set capacitive or inductive coupling
    //
    DataControl1.Bits.xtalkpisign = CapIndProg->CapIndLookup[Ch][Strobe];

    //
    // Update cache and write new values
    //
    (*ChannelNvList)[Ch].dataControl1[Strobe] = DataControl1.Data;
    WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL1N0_0_MCIO_DDRIO_REG, DataControl1.Data);
  } //Strobe

  return SUCCESS;
} //PxcProgramCapInd

/**
  Read or write PXC Enable or Disable on all channels

  @param[in]      Host      - Pointer to SYSHOST
  @param[in]      Socket    - Socket number
  @param[in]      Channel   - Channel number (ALL_CH for all channels)
  @param[in]      Mode      - Bit-field of different modes
  @param[in,out]  State     - Pointer to value to get or set - PXC_ENABLE or PXC_DISABLE

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
GetSetPxcControl (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      UINT8             Channel,
  IN      UINT8             Mode,
  IN      INT16             *State
  )
{
  UINT8                                 Strobe;
  UINT8                                 Ch;
  UINT8                                 ChStart;
  UINT8                                 MaxCh;
  DATACONTROL1N0_0_MCIO_DDRIO_STRUCT    DataControl1;
  struct channelNvram                   (*ChannelNvList)[MAX_CH];
  UINT8                                 MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  //
  // Caller can specify all channels or a single, specific channel
  // Sanity check Channel to avoid an invalid array index
  //
  if ((Channel == ALL_CH) || (Channel > MaxChDdr)) {
    ChStart = 0;
    MaxCh = MaxChDdr;
  } else {
    ChStart = Channel;
    MaxCh = Channel + 1;
  }

  for (Ch = ChStart; Ch < MaxCh; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    //
    // Check if read only
    //
    if (Mode & (GSM_READ_ONLY)) {
      //
      // Assume PXC_DISABLE
      //
      *State = PXC_DISABLE;
      //
      // Only check channel if PXC Training will be or has been performed on this channel
      //
      if (Host->var.mem.pxcEnabled || (1 << Ch)) {
        //
        // All strobes are set the same, so just check strobe 0
        //
        DataControl1.Data = (*ChannelNvList)[Ch].dataControl1[0];
        if (DataControl1.Bits.xtalkpienable == 1) {
          //
          // If any channel has xtalkpienable, return PXC_ENABLE
          //
          *State = PXC_ENABLE;
          break;
        }
      }
    } else { // GSM_READ_ONLY
     //
     // Only perform this step if PXC Training will be or has been performed on this channel
     //
     if (Host->var.mem.pxcEnabled || (1 << Ch)) {
       for (Strobe = 0; Strobe < MAX_STROBE; Strobe++) {
         DataControl1.Data = (*ChannelNvList)[Ch].dataControl1[Strobe];
         if (*State == PXC_ENABLE) {
           DataControl1.Bits.xtalkpienable = 1;
         } else {
           DataControl1.Bits.xtalkpienable = 0;
         }
         (*ChannelNvList)[Ch].dataControl1[Strobe] = DataControl1.Data;
         WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL1N0_0_MCIO_DDRIO_REG, DataControl1.Data);
       } // Strobe
     }
    } // GSM_READ_ONLY
  } // Ch loop

  return MRC_STATUS_SUCCESS;
}

/**
  Early initialization required when TX Phase-based Xtalk Cancellation (PXC)
  or Data Dependent Jitter Cancellation (DDJC) is to be enabled.

  @param[in]  Host         - Pointer to sysHost
  @param[in]  Socket       - Socket

  @retval SUCCESS

**/
UINT32
PxcDdjcInitialize (
  IN     PSYSHOST               Host,
  IN     UINT8                  Socket
  )
{
  UINT8                   Ch;
  UINT8                   PxcEnable   = 0;
  UINT8                   DdjcEnable  = 0;
  struct channelNvram     (*ChannelNvList)[MAX_CH];
  SYS_SETUP               *Setup;
  UINT8                   MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  if (GetTrainingStepEnableChip (Socket, CHECKPOINT_MINOR_PXC_TRAINING) || (Setup->mem.ForcePxcInit != 0)) {
    PxcEnable = 1;
  }

  if (GetTrainingStepEnableChip (Socket, CHECKPOINT_MINOR_DDJC_TRAINING)) {
    DdjcEnable = 1;
  }

  //
  // Return if both PXC and DDJC are disabled
  //
  if ((PxcEnable == 0) && (DdjcEnable == 0)) {
    return SUCCESS;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    //
    // Enable PXC
    //
    PxcSetup (Host, Socket, Ch, PxcEnable);

    if ((GetSysBootMode () == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot)) {
      //
      // Set default values for attacker 0 & attacker 1
      //
      PxcAttackerInit (Host, Socket, Ch);
    } else {
      //
      // Program Attackers and CapInd values from the static tables if PXC is enabled
      // RestoreDDRIO will restore xtalkpienable value from NVRAM
      //
      if (PxcEnable == 1) {
        PxcProgramAggressors (Host, Socket, Ch);
        PxcProgramCapInd (Host, Socket, Ch);
      }
      //
      // Restore HW programming of PxcDeltas from cached values if either PXC or DDJC is enabled
      //
      PxcDeltaRestore (Host, Socket, Ch);
    }

  } // Ch loop
  return SUCCESS;

} // PxcInit

/**

  PXC Training. Unused in 10nm

  @param[in] Host   - Pointer to sysHost, the system Host (root) structure struct

  @retval  MRC_STATUS_SUCCESS if no failure
  @retval  !MRC_STATUS_SUCCESS otherwise

**/

UINT32 
PXCTraining (
  IN PSYSHOST  Host
  )
{
  ASSERT (FALSE);

  return 0;

} // PXCTraining

