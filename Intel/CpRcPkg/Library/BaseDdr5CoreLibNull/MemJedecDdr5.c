/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include "Ddr5Core.h"
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>

// -------------------------------------
// Code section
// -------------------------------------

/**

Send MRW Command.

  @param Host     - Pointer to sysHost
  @param socket   - Socket Id
  @param ch       - Channel number (0-based)
  @param dimm     - DIMM number (0-based)
  @param rank     - Rank number (0-based)
  @param address  - address for the MRS write
  @param cw       - CW flag
  @param data     - Data to write

  @retval N/A

**/
UINT32
SendMrw (
  IN PSYSHOST Host,
  IN UINT8    socket,
  IN UINT8    ch,
  IN UINT8    SubChMask,
  IN UINT8    dimm,
  IN UINT8    rank,
  IN UINT16   address,
  IN UINT8    cw,
  IN UINT8    data
)
{
  return SUCCESS;
} // SendMrw

/**

Send MRW Command by PBA.

@param Host      - Pointer to sysHost
@param Socket    - Socket Id
@param Ch        - Channel number (0-based)
@param SubChMask - Sub Channel Mask
@param Dimm      - DIMM number (0-based)
@param Rank      - Rank number (0-based)
@param Buffer    - Data Buffer device number (0-based)
@param Address   - Address for the MRW write
@param Data      - Data to write

@retval NA

**/
VOID
EFIAPI
SendMrwPba (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Buffer,
  IN UINT8    Address,
  IN UINT8    Data
  )
{
  return;
} // WriteMrwPda

/**

  Return the time required for the parity recovery for DDR5 in DCLKs

  @param[in]    Socket        Current socket under test (0-based)
  @param[in]    RatioIndex    Current ratio index (frequency)

  @return       return the parity recovery in DCLKs

**/
UINT32
EFIAPI
GetParityRecoveryDdr5 (
  IN        UINT8           Socket,
  IN        UINT8           RatioIndex
)
{
  RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_253);
  return 0;
}

  /**

  Send NOP Command.

  @param Host     - Pointer to sysHost
  @param socket   - Socket Id
  @param ch       - Channel number (0-based)
  @param dimm     - DIMM number (0-based)
  @param rank     - Rank number (0-based)
  @param address  - address for the MRS write
  @param cw       - CW flag
  @param data     - Data to write

  @retval N/A

  **/
UINT32
SendNop (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Cw,
  IN BOOLEAN  PassThroughMode,
  IN BOOLEAN  ExitCATM
  )
{
  return SUCCESS;
} // SendNop

/**

Send MRW Command by PDA.

@param Host      - Pointer to sysHost
@param Socket    - Socket Id
@param Ch        - Channel number (0-based)
@param SubChMask - Sub Channel Mask
@param Dimm      - DIMM number (0-based)
@param Rank      - Rank number (0-based)
@param Dram      - Dram device number (0-based)
@param Address   - Address for the MRW write
@param Data      - Data to write

@retval NA

**/
VOID
EFIAPI
SendMrwPda (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT8    Address,
  IN UINT8    Data
  )
{
  return;
} // WriteMrwPda

/**
Writes Multi-Purpose Command

@param Host       - Pointer to sysHost
@param socket     - Socket Id
@param ch         - Channel number (0-based)
@param dimm       - DIMM number (0-based)
@param rank       - Rank number (0-based)
@param opcode     - opcode for MPC command
@param timingmode - support MPC_1N, MPC_2N, MPC_4N.

@retval N/A

**/
VOID
WriteMPC (
  IN PSYSHOST Host,
  IN UINT8    socket,
  IN UINT8    ch,
  IN UINT8    dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    rank,
  IN UINT16   opcode,
  IN UINT8    timingmode,
  IN BOOLEAN  PassThroughMode
  )
{
  return;
} // WriteMPC

/**
  Determine if the given DQ nibble number is valid for ECC mode on current socket for DDR5.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Nibble        - DQ nibble number to check

  @retVal   1     DQ nibble number is not valid
  @retVal   0     DQ nibble number is valid
**/
BOOLEAN
IsNibbleNotValidDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Nibble
  )
{
  return 0;
} // IsNibbleNotValidDdr5

/**
  Determine if the given Strobe number is valid for ECC mode on current socket for DDR5.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Strobe        - Strobe number to check

  @retVal   1     Strobe number is not valid
  @retVal   0     Strobe number is valid
**/
BOOLEAN
IsStrobeNotValidDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Strobe
  )
{
  return 0;
}

/**
  Determine if the given Strobe number is valid for ECC mode on current socket for DDR5 full channel.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Strobe        - Strobe number to check

  @retVal   1     Strobe number is not valid
  @retVal   0     Strobe number is valid
**/
BOOLEAN
EFIAPI
IsStrobeNotValidChDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Strobe
  )
{
  return 0;
}

/**
  Determine if the given Dimm and Strobe number is valid for ECC mode on current socket for DDR5.

  @param[in]  DimmNvramStructPtr  Pointer to dimmNvram structure to check
  @param[in]  Strobe              Strobe number to check

  @retVal   TRUE    Strobe number is not valid
  @retVal   FALSE   Strobe number is valid
**/
BOOLEAN
EFIAPI
IsDimmStrobeNotValidDdr5 (
  IN struct dimmNvram *DimmNvramStructPtr,
  IN UINT8            Strobe
  )
{
  return FALSE;
} // IsDimmStrobeNotValidDdr5


  /**
  Determines a bit mask of valid strobe numbers for ECC mode on current socket for DDR5.

  @param[in]      Host          - Pointer to sysHost

  @retVal   UINT32    Bit mask of valid Strobe numbers
  **/
UINT32
GetSubChStrobeMaskDdr5(
  IN PSYSHOST  Host
)
{
  return 0;
} // GetSubChStrobeMaskDdr5

/**

Writes Multi-Purpose Command for DDR5

@param Host                 - Pointer to sysHost, the system Host (root) structure
@param socket               - Socket Id
@param ch                   - Channel number (0-based)
@param dimm                 - DIMM number (0-based)
@param subchannelmask       - sub channel mask
@param rank                 - Rank number (0-based)
@param opcode               - opcode for the MPC command
@param timingmode           - support MPC_1N, MPC_2N, MPC_4N.

@retval None

**/
VOID
WriteMpcDdr5 (
  IN PSYSHOST Host,
  IN UINT8    socket,
  IN UINT8    ch,
  IN UINT8    dimm,
  IN UINT8    subChannelMask,
  IN UINT8    rank,
  IN UINT16   opcode,
  IN UINT8    timingmode,
  IN BOOLEAN  PassThroughMode
  )
{
  return;
} // WriteMpcDdr5

/**

Writes Vref CA Command for DDR5

@param[in] Host             - Pointer to sysHost, the system Host (root) structure
@param[in] Socket           - Socket Id
@param[in] Ch               - Channel number (0-based)
@param[in] Dimm             - DIMM number (0-based)
@param[in] Subchannelmask   - Sub channel mask
@param[in] Rank             - Rank number (0-based)
@param[in] Opcode           - Vref CA command opcode
@param[in] Timingmode       - Supports TIMING_SINGLE_CS, TIMING_MULTI_CS
@param[in] PassThroughMode  - PassThrough mode or not

@retval None

**/
VOID
WriteVrefCaDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Subchannelmask,
  IN UINT8    Rank,
  IN UINT16   Opcode,
  IN UINT8    Timingmode,
  IN BOOLEAN  PassThroughMode
  )
{
  return;
} // WriteVrefCaDdr5

//
// New for DDR5
//

/**
  Determine if a DDR memory subsystem is present in the given Socket

  @param[in]      Host          Pointer to sysHost structure
  @param[in]      Socket        Processor socket

  @retval 1 DDR memory subsystem is presetn
  @retval 0 DDR memory subsystem is not present
**/
BOOLEAN
IsDdr5Present (
  IN PSYSHOST  Host,
  IN UINT32    Socket
  )
{
  return 0;
} // IsDdr5Present

/**

  Starts the JEDEC initialization sequence

  @param Host    - Pointer to sysHost
  @param socket  - Socket to initizlize

  @retval SUCCESS

**/
UINT32
JedecInitPreTraining (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  return SUCCESS;
} // JedecInitPreTraining

/**
This function will setup CKEMask with Selected Chip Selects. This is used for reset and self-refresh exit

  @param[in]      Host          - Pointer to sysHost
  @param[in]      socket        - Socket Id

@retVal None
**/
VOID
SetCSMask(
  IN  PSYSHOST Host,
  IN  UINT8    socket
  )
{
  return;
} // SetCSMask

/**

  Choose QLBD/QLBS As The Feedback

  @param[in]  Socket          - Socket number
  @param[in]  Channel         - Channel number
  @param[in]  Dimm            - Dimm number
  @param[in]  Rank            - Rank number

  @retval: None
**/
VOID
EFIAPI
ChooseQLBAsFeedback (
  IN      UINT8    Socket,
  IN      UINT8    Channel,
  IN      UINT8    Dimm,
  IN      UINT8    Rank
  )
{
  return;
}

/**

  Check if RCD loopback mode should be used

  @param Host    - Pointer to sysHost

  @retval FALSE

**/
BOOLEAN
CheckRCDLoopback (
  PSYSHOST Host
  )
{
  return FALSE;
} // CheckRCDLoopback

/*

  This function Gets or Sets the QCA backside signals

  @param[in]      Host    Pointer to SysHost
  @param[in]      Socket  Current socket under test (0-based)
  @param[in]      Channel Current channel under test (0-based)
  @param[in]      Dimm    Current dimm under test (0-based)
  @param[in]      Rank    Current rank under test (0-based)
  @param[in]      Level   DDR level. LrbufLevel is the only allowed parameter
  @param[in]      Group   Group to be accessed (allowed: CmdAll, Cmd0All, Cmd1All)
  @param[in]      Mode    Mode of operation:
                          read (GSM_READ_ONLY <| GSM_READ_CSR>),
                          write (GSM_WRITE_OFFSET | GSM_FORCE_WRITE),
                          update cache (GSM_UPDATE_CACHE)
  @param[in,out]  Value   Pointer to memory location that contains the value to be written
                          or the location where the read value will be placed
  @param[in,out]  MinVal  Pointer to a memory location that keep track of the minimum value
  @param[in,out]  MaxVal  Pointer to a memory location that keep track of the maximum value

  @retval   EFI_INVALID_PARAMETER     if Level is not supported
  @retval   EFI_INVALID_PARAMETER     if Socket or Channel or Dimm are not within the allowed limits
  @retval   EFI_INVALID_PARAMETER     if any pointer to Value or MinValue or MaxValue is NULL
  @retval   EFI_UNSUPPORTED           for not supported groups
  @retval   EFI_UNSUPPORTED           if Max or Min thresholds are not met

*/
EFI_STATUS
EFIAPI
GetSetCaBacksideDdr5 (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value,
  IN OUT  UINT16    *MinVal,
  IN OUT  UINT16    *MaxVal
  )
{
  return EFI_UNSUPPORTED;
}

/*

  This function Gets or Sets the QCS backside signals based on Dimm/Rank

  @param[in]      Host    Pointer to SysHost
  @param[in]      Socket  Current socket under test (0-based)
  @param[in]      Channel Current channel under test (0-based)
  @param[in]      Dimm    Current dimm under test (0-based)
  @param[in]      Rank    Current rank under test (0-based)
  @param[in]      Level   DDR level. LrbufLevel is the only allowed parameter
  @param[in]      Group   Group to be accessed (allowed: CtlAll, Ctl0Grp, Ctl1Grp)
  @param[in]      Mode    Mode of operation:
                          read (GSM_READ_ONLY <| GSM_READ_CSR>),
                          write (GSM_WRITE_OFFSET | GSM_FORCE_WRITE),
                          update cache (GSM_UPDATE_CACHE)
  @param[in,out]  Value   Pointer to memory location that contains the value to be written
                          or the location where the read value will be placed
  @param[in,out]  MinVal  Pointer to a memory location that keep track of the minimum value
  @param[in,out]  MaxVal  Pointer to a memory location that keep track of the maximum value

  @retval   EFI_INVALID_PARAMETER     if Level is not supported
  @retval   EFI_INVALID_PARAMETER     if Socket or Channel or Dimm are not within the allowed limits
  @retval   EFI_INVALID_PARAMETER     if any pointer to Value or MinValue or MaxValue is NULL
  @retval   EFI_UNSUPPORTED           for not supported groups
  @retval   EFI_UNSUPPORTED           if Max or Min thresholds are not met

*/
EFI_STATUS
EFIAPI
GetSetCsBacksideDdr5 (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value,
  IN OUT  UINT16    *MinVal,
  IN OUT  UINT16    *MaxVal
  )
{
  return EFI_UNSUPPORTED;
}

/**

  Programs CAS Latency for DDR devices

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Ch number

  @retval N/A

**/
VOID
ProgramCasLDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  return;
} // ProgramClDdr5

/**

  Determine if Backside CMD Margin is enabled for DDR5

  @param  VOID

  @retval TRUE  - Backside CMD Margin is enabled
  @retval FALSE - Backside CMD Margin is not enabled

**/
BOOLEAN
IsDdr5BacksideCmdMarginEnabled (
  VOID
  )
{
  return FALSE;
} // IsDdr5BacksideCmdMarginEnabled

/**
  Programs CAS Latency for DDR5 ranks

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Ch number
  @param[in] Dimm    - Dimm number (zero-based)
  @param[in] Rank    - Rank number (zero-based)

  @retval N/A

**/
VOID
ProgramMr0Ddr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  return;
}

/**
  Program DQS Interval Control (MR45).

  @param[in]  Host          Pointer to sysHost
  @param[in]  Socket        Socket number
  @param[in]  Channel       Channel number on socket
  @param[in]  TimerRunTime  DQS interval timer run time (see MR45 spec)
**/
VOID
EFIAPI
ProgramDqsIntervalControl (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    TimerRunTime
  )
{
  return;
}

/**
Enables Write Leveling Mode for DDR5 ranks

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket Id
  @param[in] Ch                - Ch number
  @param[in] Dimm              - Dimm number (zero-based)
  @param[in] Rank              - Rank number (zero-based)
  @param[in] Mode              - Write Leveling Mode
  @retval N/A

**/
VOID
ConfigureDramForWriteLeveling (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Mode
)
{
  return;
}

/**

  This function returns the value of tREFI

  @param[in] Host         - Point to sysHost
  @param[in] Socket       - Socket Id
  @param[in] Ch           - Channel number
  @param[in] RefreshMode  - Refresh Mode

  @retval UINT32 Value of tREFI

**/
UINT32
GettREFIDdr5 (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN REFRESH_MODE   RefreshMode
)
{
  return 0;
}

/**

  Enables Write CRC in the DRAM

  @param[in] Socket   - Socket number
  @param[in] Ch       - Channel number within the socket
  @param[in] Dimm     - DIMM number within the channel
  @param[in] Rank     - Rank number within the DIMM

  @retval N/A

**/
VOID
EFIAPI
Ddr5DramEnableWrCrc (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     Rank
  )
{
}

/*
  Convert a given Tx DQ VREF physical value to percentage and then finally to a linear Tx DQ VREF value
  Where:
  Physical    Percentage(%)   Linear
    0x69          45            0
    0x68          45.5          1
    0x67          46            2
    0x66          46.5          3
    ...
    0x02          96.5          103
    0x01          97            104
    0x00          97.5          105

  @param[in]      Physical      Tx VREF percentage (0x00 - 0x69)

  @return         Linear value from physical

*/
INT16
EFIAPI
ConvertPhysicalToLinear (
  IN        INT16   Physical
  )
{
  //
  // Do nothing
  //
  return Physical;
}

/*
  Convert a given Tx DQ VREF linear value to physical Tx DQ VREF value
  Where:
  Linear        Physical
    0             0x69
    1             0x68
    2             0x67
    3             0x66
    ...           ...
    103           0x02
    104           0x01
    105           0x00

  @param[in]      Linear      Value between (0-105) that represents an specific VREF DQ percentage

  @return         Physical value from linear

*/
INT16
EFIAPI
ConvertLinearToPhysical (
  IN        INT16   Linear
  )
{
  //
  // Do nothing
  //
  return Linear;
}

/**

  Prints all DRAM, Register and Buffer CSR data from all DIMMs on the socket.
  Note: this function assumes that the read training has been successfully
  completed in order to read DRAM and Buffer data.

  @param[in] Socket  - Socket Id

  @retval N/A

**/
VOID
EFIAPI
DumpDimmStateDdr5 (
  IN  UINT8     Socket
  )
{
}

/**

  Starts the JEDEC initialization sequence for DDR5

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket number
  @param[in] ChBitMask  - Bit Mask of Channels to operate on

  @retval MRC_STATUS_SUCCESS if Jedec Init sequences is successful
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if function is not implemented
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
JedecInitSequenceDdr5 (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT32    ChBitMask
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**

  Executes the JEDEC initialization sequence for each channel on the provided socket number

  @param Host      - Pointer to sysHost
  @param Socket    - Socket Id
  @param ChBitMask - Bit Mask of Channels Present on the Socket

  @retval MRC_STATUS_SUCCESS if sequence is successful
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if function is not implemented
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
JedecInitDdrAllDdr5 (
  PSYSHOST      Host,
  UINT8         Socket,
  UINT32        ChBitMask
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**

  Executes the JEDEC initialization sequence for the given channel

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket number
  @param[in] Ch       - Channel number within the socket

  @retval MRC_STATUS_SUCCESS if sequence is successful
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if function is not implemented
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
JedecInitDdrChannelDdr5 (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket,
  IN  UINT8       Ch
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**

  This function returns the value of Ddr5 tXP

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of Ddr5 tXP

**/
UINT32
GettXPDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{

  return 0;
} // GettXPDdr5

/**

  This function returns the value of tSRX2SRX.  The minimum number of valid
  input clock cycles between SRX command to release QCS and SRX to take
  DRAM out of Self Refresh (DDR5 JEDEC RCD Specification)

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tSRX2SRX in clocks

**/
UINT32
EFIAPI
GettCKSRX2SRX (
  IN UINT8    Socket
  )
{
  return 0;
} // GettCKSRX2SRX

/**

  This function returns the value of tCSSR.
  Minimum DCS[1:0]_n LOW time (after SRE) when entering DCK stop
  (DDR5 JEDEC RCD Specification)

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tCSSR in clocks

**/
UINT32
EFIAPI
GettCSSR (
  IN UINT8    Socket
  )

{
  return 0;
} // GettCSSR

/**

  This function returns the value of tCKACT.
  Maximum time from DCS_n transition HIGH to DCK start
  (DDR5 JEDEC RCD Specification)

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tCKACT in clocks

**/
UINT32
EFIAPI
GettCKACT (
  IN UINT8    Socket
  )
{
  return 0;
} // GettCKACT

/**

  Program all DRAM in a channel with the refresh mode

  @param[in] Socket       - Socket Id
  @param[in] Ch           - Channel number
  @param[in] RefreshMode  - Refresh Mode
                              RefabNormal
                              RefabFineGrain
                              RefsbFineGrain

  @retval MRC_STATUS_SUCCSS if refresh mode is programmed
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
SetDramRefreshMode (
  IN  UINT8         Socket,
  IN  UINT8         Ch,
  IN  REFRESH_MODE  RefreshMode
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**

  Return the DDR5 Refresh Mode policy setting

  @retval Refresh Mode - RefabNormal
                       - RefabFineGrain
                       - RefsbFineGrain

**/
REFRESH_MODE
EFIAPI
GetRefreshModePolicy (
  VOID
  )
{
  return  RefabNormal;
}

/**

  Get set RCD DCS vref based on the provided signal number

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] Dimm             - DIMM number
  @param[in] SubChannel       - SubChannel number (0-based)
  @param[in] Signal           - Enumerated signal name
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out] Value      - Value to set or get

  @retval Status
**/
MRC_STATUS
EFIAPI
GetSetRcdDcsVref (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
  IN     UINT8    Channel,
  IN     UINT8    SubChannel,
  IN     UINT8    Dimm,
  IN     GSM_CSN  Signal,
  IN     UINT8    Mode,
  IN OUT INT16   *Value
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**
  Multi-use function to either get or set RCD command DFE vref based on the provided siganl number

  @param[in] Host        - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket      - Socket number
  @param[in] Channel     - Channel number
  @param[in] SubChannel  - SubChannel number
  @param[in] Dimm        - DIMM number within a DDR channel (0-based, farthest slot == 0)
  @param[in] Signal      - Enumerated signal name
  @param[in] Mode        - GSM_READ_CSR - Read the data from hardware and not cache
                         -  GSM_READ_ONLY - Do not write
                         -  GSM_WRITE_OFFSET - Write offset and not value
                         -  GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value - Value to program

  @retval Status

**/
MRC_STATUS
EFIAPI
GetSetDcaDfeVref (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
  IN     UINT8    Channel,
  IN     UINT8    SubChannel,
  IN     UINT8    Dimm,
  IN     GSM_CSN  Signal,
  IN     UINT8    Mode,
  IN OUT INT16    *Value
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/*

  This function Gets or Sets the QCA delay signals

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] SubChannel       - SubChannel number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number
  @param[in] Signal           - Enumerated signal name
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value      - Value to program

*/
MRC_STATUS
EFIAPI
GetSetQcaDelay (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     SubChannel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      GSM_CSN   Signal,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}


/*

  This function Gets or Sets the DRAM CA/CS Vref which are the backside QCA/QCS Vref. 

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number
  @param[in] Strobe           - Strobe number inside the channel. Range from 0 to 19, or ALL_STROBES
  @param[in] Group            - Margin Group. Valid value DramCaVref or DramCsVref 
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value      - Value to program

*/
MRC_STATUS
EFIAPI
GetSetDramCaCsVref (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**

  Get/Set DDR5 QCSx Delay of all copies (A and B)
  QCSx is the CSx Signal from the RCD to the DRAM
  Programs QCS delay depending on the Raw Card detected
  This function shall program A, both A and B copies if applicable based on the Dimm type.

  @param[in]      Host         - Pointer to SysHost
  @param[in]      Socket       - Socket number
  @param[in]      Channel      - Channel number
  @param[in]      SubChannel   - SubChannel number
  @param[in]      Dimm         - DIMM number within the channel
  @param[in]      Rank         - DIMM Rank number
  @param[in]      Mode         - Access Mode
  @param[in,out]  Delay        - Pointer to Delay value
  @param[in,out]  MinVal       - Pointer to a memory location that keep track of the minimum value of all copies
  @param[in,out]  MaxVal       - Pointer to a memory location that keep track of the maximum value of all copies
                                 The MinVal and MaxVal can be used by the MemSweep lib to keep track the delta of
                                 the initial trained values of both copies. It helps the MemSweep lib to adjust the
                                 sweep range.

  @retval   - MRC_STATUS_SUCCESS if command completed successfully
  @retval   - !MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
EFIAPI
GetSetQcsAllCopiesDelay (
  IN      PSYSHOST        Host,
  IN      UINT8           Socket,
  IN      UINT8           Channel,
  IN      UINT8           SubChannel,
  IN      UINT8           Dimm,
  IN      UINT8           Rank,
  IN      UINT8           Mode,
  IN OUT  INT16           *Delay,
  IN OUT  INT16           *MinVal,  OPTIONAL
  IN OUT  INT16           *MaxVal   OPTIONAL
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}
/**

  Get the limits for the group or level requested. The adjustment of the group is done
  by the RCD, DB buffer or DRAM.

  @param Level     - Level to program (DDR, VMSE, Buffer)
  @param Group     - Parameter to program
  @param MinLimit  - Minimum delay value allowed
  @param MaxLimit  - Maximum absolute value allowed
  @param Delay     - Time in usec required for new setting to take effect

  @retval MRC_STATUS

**/
MRC_STATUS
GetDimmParamGroupLimits (
  IN  MRC_LT    Level,
  IN  MRC_GT    Group,
  OUT UINT16    *MinLimit,
  OUT UINT16    *MaxLimit,
  OUT UINT16    *Delay
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**

  Checks to see of supplied group is a member of the "Dimm" parameter group which is adjuested
  by the RCD, DB or DRAM.

  @param group - Group item to check

  @retval TRUE  - Is a member of DIMM Group
  @retval FALSE - Is not a member of DIMM Group

**/
BOOLEAN
CheckDimmParamGroupDdr5 (
  MRC_GT  Group
  )
{
  return FALSE;
}

/**
  Multi-use function to either get or set RCD DCA vref based on the provided siganl number

  @param[in] Host        - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket      - Socket number
  @param[in] Channel     - Channel number
  @param[in] SubChannel  - SubChannel number
  @param[in] Dimm        - DIMM number within a DDR channel (0-based, farthest slot == 0)
  @param[in] Signal      - Enumerated signal name
  @param[in] Mode        - GSM_READ_CSR - Read the data from hardware and not cache
                         - GSM_READ_ONLY - Do not write
                         - GSM_WRITE_OFFSET - Write offset and not value
                         - GSM_FORCE_WRITE - Force the write
  @param[in, out] Value  - Value to get or set

  @retval MRC_STATUS_SUCCESS if cache entry is returned
  @retval MRC_STATUS_FAILURE if the cache entry is not supported
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
GetSetRcdDcaVref (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
  IN     UINT8    Channel,
  IN     UINT8    SubChannel,
  IN     UINT8    Dimm,
  IN     GSM_CSN  Signal,
  IN     UINT8    Mode,
  IN OUT INT16    *Value
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**

  Take DB in or out of reset.

  @param[in] Socket         - Socket number
  @param[in] ChannelBitMask - Bitmask of channels to reset
  @param[in] IsAssert       - Assert/Deassert BRST_n for DB

  @retval  MRC_STATUS_SUCCESS

**/
MRC_STATUS
EFIAPI
SetDbReset (
  IN UINT8     Socket,
  IN UINT32    ChannelBitMask,
  IN BOOLEAN   IsAssert
  )
{
  return MRC_STATUS_SUCCESS;
}

/**

  Get set Data Buffer DFE vref based on the provided group number of a give DQ pin

  NOTE: In order to set the DFE_Vref, the DFE feature (RWA0) and DFE training mode (RWA1) should
        be enabled.
        They are enabled in the RMT pre-sweep function.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] Dimm             - DIMM number
  @param[in] Strobe           - Strobe number inside the channel.
                                Caller shall pass the valid strobes: ALL_STROBES, 0-4 and 10 -14
  @param[in] Group            - Enumerated group name
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value      - Value to program

  @retval Status
**/
MRC_STATUS
GetSetDbDfeVrefDqn (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Channel,
  IN    UINT8     Dimm,
  IN    UINT8     Strobe,
  IN    MRC_GT    Group,
  IN    UINT8     Mode,
  IN OUT INT16    *Value
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/*

  This function Gets or Sets the BCOM signals.
  It assumes all DB in the same DIMM use the same BCOM Vref.
  This function also update cache of PG[2]RWFA.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] SubChannel       - SubChannel number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number
  @param[in] Strobe           - DB index
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value      - Value to program into DB PG[2]RWFA.
                                When Mode is GSM_WRITE_OFFSET, the offset step size is 2,
                                which is 1% Vdd.

*/
MRC_STATUS
EFIAPI
GetSetBcomVref (
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     SubChannel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  return MRC_STATUS_SUCCESS;
}