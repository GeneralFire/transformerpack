/** @file
  Routines to access scratch pad register

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2018 Intel Corporation. <BR>

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

#include "ScratchPadReg.h"
#include "Chip10nm/Include/RcRegs10nm.h"
#include <SysHostChip.h>
#include "Chip10nm/Include/MemHostChip10nm.h"
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/CsrAccessLib.h>
#include <ScratchpadList.h>
#include <Library/UsraCsrLib.h>

// BIOS scratchpad registers can not be assumed to have continuous offsets.
// Use the table below to map Scratchpad# to  CSR address offset
UINT32 BiosNonStickyScratchPadCsr[SCRATCH_PAD_NUM] = {
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
  BIOSNONSTICKYSCRATCHPAD12_UBOX_MISC_REG
};

//
// Routines to access scratch pad register
//
UINT32
readScratchPad_CMD (
  VOID
  )
{
    UINT32 val = 0;
    val = UsraCsrRead (0,  0, EV_AGENT_SCRATCHPAD_CMD_REG_ADDRESS);
    return val;
}

VOID
writeScratchPad_CMD (
  UINT32               val
  )
{
    UsraCsrWrite (0, 0, EV_AGENT_SCRATCHPAD_CMD_REG_ADDRESS, val);
}

UINT32
readScratchPad_DATA0 (
  VOID
  )
{
    UINT32 val = 0;
    val = UsraCsrRead (0,  0, EV_AGENT_SCRATCHPAD_DATA0_REG_ADDRESS);
    return val;
}

VOID
writeScratchPad_DATA0 (
  UINT32                val
  )
{
    UsraCsrWrite (0, 0, EV_AGENT_SCRATCHPAD_DATA0_REG_ADDRESS, val);
}

UINT32
readScratchPad_DATA1 (
  )
{
    UINT32 val = 0;
    val = UsraCsrRead (0,  0, EV_AGENT_SCRATCHPAD_DATA1_REG_ADDRESS);
    return val;
}

VOID
writeScratchPad_DATA1 (
  UINT32                val
  )
{
    UsraCsrWrite (0, 0, EV_AGENT_SCRATCHPAD_DATA1_REG_ADDRESS, val);
}

VOID
writeScratchPad_currentCheckPoint (
  UINT32               val
  )
{
    UsraCsrWrite (0, 0, EV_AGENT_SCRATCHPAD_CURRENT_CHECKPOINT_REG_ADDRESS, val);
}

/**

  Walks through all of the stored data and restores the scratchpad registers upon S3 Resume.

  @param socket      - Socket number
  @param Buffer      - Pointer to Dword buffer to restore
  @param DwordCount  - number of dword count
  @param DstBuffer   - Pointer to destination buffer to restore

  @retval None

**/
VOID
MultiThreadS3ResumeScratchPadRestore (
  UINT8                 socket,
  UINT32                *Buffer,
  UINT32                NumberOfDwords,
  UINT32                *DstBuffer
  )
{
    UINT32  CurrentDwordCount;
    UINT32  HostDataDword;
    UINT32  i;
    UINT32  currentDword;


    RcDebugPrint (SDBG_DEFAULT, "Buffer address is %x dword num:%x \n",  (UINT32)(UINTN) Buffer, NumberOfDwords);
    //
    // Transfer
    //
    for (CurrentDwordCount = 0; CurrentDwordCount < NumberOfDwords; CurrentDwordCount += currentDword) {
        while (UsraCsrRead (socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR) != 0x00);

        currentDword = ((NumberOfDwords - CurrentDwordCount) <  SCRATCH_PAD_NUM) ? (NumberOfDwords - CurrentDwordCount) : SCRATCH_PAD_NUM;

        for (i = 0; i < currentDword; i++) {
            HostDataDword = Buffer[CurrentDwordCount + i];
            UsraCsrWrite (socket, 0, BiosNonStickyScratchPadCsr[i], HostDataDword);
        }

        UsraCsrWrite (socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR, 0x1);
    }

    while (UsraCsrRead (socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR) != 0x00);
    UsraCsrWrite (socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR, 0x1);

    RcDebugPrint (SDBG_DEFAULT, "Transfer complete\n");
}

/**

  Returns the Dispatch Pipe CSR which is currently stored in Non Sticky Scratchpad 13.

  @param socket      - Socket number

  @retval UINT32 - returns Dispatch Pipe CSR.

**/
UINT32
GetDispatchPipeCsr (
  UINT8                   socket
  )
{
    UINT32 spReg;
    spReg = UsraCsrRead (socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR);

    return spReg;
}

/**

  Writes the Dispatch Pipe CSR which is currently stored in Non Sticky Scratchpad 13.

  @param socket      - Socket number
  @param data        - data to be written out

  @retval None.

**/
VOID
WriteDispatchPipeCsr (
  UINT8                 socket,
  UINT32                data
  )
{

    UsraCsrWrite (socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR, data);
}

/**

  If the mode passed in is Get (0) then return the state of the request to perform the JEDEC Init Sequence.
  This state is currently stored in BIOS Sticky Scratchpad 5 bit 24 (1 << 24).
  If this state is TRUE then perform some initialization prior to running the JEDEC Init Sequence.
  If the mode passed in is Set (1) then restore the scratchpad5 and scratchpad 6 to their original values with
  the exception of JEDEC Init Sequence request bit being cleared.

  @param socket  - Socket number
  @param mode    - 0 = Get, 1 = SET

  @retval UINT8 - returns the scratchpad value that indicates if the JEDEC Init Sequence Request should execute.

**/
UINT32
GetSetValFunctions (
  UINT8                   socket,
  UINT8                   mode,
  UINT32                  *valFuncs
  )
{
  if (mode == GET_REQ) {
    //
    // Get the JEDEC Init Sequence Request value
    //
    *valFuncs = UsraCsrRead (socket, 0, BIOSSCRATCHPAD5_UBOX_MISC_REG);

  } else {
    //
    // Restore the registers affected by the JEDEC Init Sequence
    //
    UsraCsrWrite (socket, 0, BIOSSCRATCHPAD5_UBOX_MISC_REG, *valFuncs);

  }

  return *valFuncs;
}


/**

  Write the provided checkpoint code and data into the Non sticky scratchpad register 7.

  @param socket     - Socket number
  @param checkpoint - The major and minor codes combined in a UINT16 value
  @param data       - data to be written out

  @retval UINT32 - returns the value of the data that was written to the scratchpad register.

**/
UINT32
SetPostCode (
  UINT8                   socket,
  UINT16                  checkpoint,
  UINT16                  data
  )
{
    UINT32 csrReg;

    csrReg = (UINT32)((checkpoint << 16) | data);
    //
    // Write checkpoint to local scratchpad
    //

    SetNonStickyScratchpad (socket, SR_POST_CODE, csrReg);
    return csrReg;
}

/**

  Return the value of the break point CSR currently stored in the upper 16 bits of Sticky Scratchpad 6.

  @param socket  - Socket number

  @retval UINT32 - returns the scratchpad value for the breakpoint.

**/
UINT32
GetBreakpointCsr (
  UINT8                   socket
  )
{
    BIOS_SCRATCHPAD6_STRUCT spReg;
    spReg.Data = GetStickyScratchpad (socket, SR_BIOS_SERIAL_DEBUG);

    return (spReg.Bits.EvToolBreakPoint);
}

/**

  Set the break point scratch pad

  @param[in] Socket       - Socket number
  @param[in] CheckPoint   - Check point to break on: 0xMMmm, where MM = Major Code, mm = Minor Code

  @retval N/A

**/
VOID
SetBreakpointCsr (
  IN    UINT8     Socket,
  IN    UINT32    CheckPoint
  )
{
  BIOS_SCRATCHPAD6_STRUCT ScratchPad6;

  ScratchPad6.Data = GetStickyScratchpad (Socket, SR_BIOS_SERIAL_DEBUG);
  ScratchPad6.Bits.EvToolBreakPoint = CheckPoint;

  SetStickyScratchpad (Socket, SR_BIOS_SERIAL_DEBUG, ScratchPad6.Data);
}

/**

  Return the value of the SSA Agent Data currently stored in the lower 16 bits of Sticky Scratchpad 6.

  @param socket  - Socket number

  @retval UINT32 - returns the scratchpad value for the SSA Agent Opcode.

**/
UINT32
GetSsaAgentData (
  UINT8                   socket
  )
{
    BIOS_SCRATCHPAD6_STRUCT spReg;
    spReg.Data = GetStickyScratchpad (socket, SR_BIOS_SERIAL_DEBUG);

    return (spReg.Bits.SsaMailbox);
}

/**

  Clears the SSA Agent Data in the lower 16 bits of Sticky Scratchpad 6.

  @param socket  - Socket number

  @retval VOID

**/
VOID
ClearSsaAgentData (
  UINT8                   socket
  )
{
    UINT32 spReg;
    spReg = GetStickyScratchpad (socket, SR_BIOS_SERIAL_DEBUG);

    SetStickyScratchpad (socket, SR_BIOS_SERIAL_DEBUG, (spReg & 0xFFFF0000));
}

