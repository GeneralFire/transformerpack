/** @file
  MemFmcIpLib.h

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

#include "SysHost.h"
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/MemFmcIpLib.h>

//
// FMC IP interface
//

/**
  GetSetTxVrefFnv

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval status
**/
MRC_STATUS
GetSetTxVrefFnv (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  return MRC_STATUS_SUCCESS;
}

MRC_STATUS
CacheTxVrefFnv (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Rank
  )
{
  return MRC_STATUS_SUCCESS;
}

MRC_STATUS
InitDdrioInterfaceLateFmc (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  return MRC_STATUS_SUCCESS;
}

MRC_STATUS
SetTxPiOnFmc (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT32    TxPiOn
  )
{
  return MRC_STATUS_SUCCESS;
}

/**
  Resets the DDRT DDRIO

  @param[in]  Host              Pointer to sysHost
  @param[in]  Socket            Socket Id
  @param[in]  ChannelInSocket   Channel number (0-based)
  @param[in]  Dimm              Zero based DIMM number

  @retval N/A
**/
VOID
EFIAPI
BiosAepIoResetChip (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     ChannelInSocket,
  IN  UINT8     Dimm
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**

  Checks if the Nvm is POR or not

  @param[in] NvmStepping  - Nvm Stepping
  @param[in] FmcModRev    - Nvm Module Revision and Type
  @param[in] NvmCapacity  - Nvm Capacity

  @retval FALSE The Nvm is not Supported
  @retval TRUE  The Nvm is Supported

**/
BOOLEAN
DdrtPorCheck (
  IN UINT8                        NvmStepping,
  IN SPD_AEP_MOD_REVISION_STRUCT  FmcModRev,
  IN UINT16                       NvmCapacity
  )
{
  //
  // Place holder function for NULL Library
  //
  return FALSE;
}

VOID
DisableScrambling (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  //
  // Place holder function for NULL Library
  //
}

VOID
InvalidateDdrtPrefetchCache (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  //
  // Place holder function for NULL Library
  //
}

VOID
ProgramDdrtGnt2Erid (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  //
  // Place holder function for NULL Library
  //
}

VOID
ProgramtCLtCWLAdd (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  //
  // Place holder function for NULL Library
  //
}

VOID
SwitchToNormalModeFmc1 (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  //
  // Place holder function for NULL Library
  //
}

VOID
ReadBcwDimmFmc (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     bcwFunc,
  UINT8     bcwByte,
  UINT8     bcwNibble,
  UINT8     MprData[MAX_STROBE]
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Disable FNV

  @param Host                 - Pointer to sysHost, the system Host (root) structure
  @param socket               - Socket Id
  @param ch                   - Channel number (0-based)
  @param chipErlyCmdCKStruct  - Pointer to Early CMD CK structure

  @retval None
**/
VOID
DisableChipFNV (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Clears DqSwizzle Register in the AEP DIMM

  @param Host - Pointer to sysHost, the system Host(root) structure
  @param socket - CPU Socket Node number(Socket ID)

  @retval None
**/
VOID
ClearDqSwizzleAep (
  IN  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  IN  UINT8       Socket                            // CPU Socket Node number (Socket ID)
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Write control word data

  @param Host             - Pointer to sysHost, the system Host (root) structure
  @param socket           - CPU Socket Node number (Socket ID)
  @param ch               - Channel number (0-based)
  @param dimm             - Current dimm
  @param controlWordData  - Data to be written
  @param controlWordAddr  - Control word address (RDIMM_RC0A or RDIMM_RC0E)

  @retval None
**/
VOID
WriteRCFnv (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    controlWordData,
  UINT8    controlWordAddr
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Enables/Disables Training Mode for FNV

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket number
  @param mode    - Training step

  @retval SUCCESS
**/
UINT32
SetAepTrainingMode (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    mode
  )
{
  return SUCCESS;
}

/**
  Routine Description: Clears Prefetch Cache miss for NVMCTLR

  @param Host    - Pointer to sysHost
  @param socket  - Socket number

  @retval SUCCESS
**/
UINT32
ClrPcheMiss (
  PSYSHOST Host,
  UINT8    socket
  )
{
  return SUCCESS;
}

/**
  Routine Description: Enables/Disables Training Mode for NVMCTLR, SMBUS version

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param mode    - training mode

  @retval SUCCESS
**/
UINT32
SetAepTrainingModeSMB (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    mode
  )
{
  return SUCCESS;
}

/**
  Routine Description: Enables/Disables Training Mode for NVMCTLR, SMBUS version

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Ch      - Channel number
  @param[in] GetFlag - True: Get, False: Restore
  @param[in/out] DaTrainingEnOrg - DCPMM training mode register setting
  @param[in/out] DTrainingEnOrg  - DCPMM training mode register setting
  @param[in/out] HsioTrainingEnOrg - DCPMM training mode register setting

  @retval SUCCESS
**/
UINT32
GetRestoreAepTrainingModeSMB (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  BOOLEAN     GetFlag,
  IN OUT  UINT32  *DaTrainingEnOrg,
  IN OUT  UINT32  *DTrainingEnOrg,
  IN OUT  UINT32  *HsioTrainingEnOrg
  )
{
  return SUCCESS;
}

/**
  This resets the NVMCTLR DDRTIO FIFO Pointers

  @param Host    - Pointer to sysHost
  @param socket    - Socket number

  @retval N/A
**/
VOID
IODdrtReset (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    dimm
  )
{
  //
  // Place holder function for NULL Library
  //
}

VOID
IODdrtResetAll (
  PSYSHOST Host,
  UINT8 socket
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Resets the FNV DDRTIO FIFO Pointers

  @param[in]  Host    Pointer to sysHost
  @param[in]  Socket  Socket number
  @param[in]  Channel Channel number
  @param[in]  Dimm    Dimm number

  @retval N/A
**/
VOID
EFIAPI
IODdrtResetPerCh (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Channel,
  IN  UINT8    Dimm
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  This sets the CTL loopback mode in the buffer

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket number
  @param[in] Ch        - Channel number
  @param[in] Dimm      - Dimm number
  @param[in] TrainFlag - TRUE:  Set Rc0c
                         False: Clear Rc0c

  @retval N/A
**/
VOID
ChipSetCTLLoopbackFmc (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch,
  IN  UINT8    Dimm,
  IN  BOOLEAN  TrainFlag
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Put CS0_n into loopback mode in the buffer

  @param Host     - Pointer to sysHost
  @param socket   - Socket number

  @retval N/A
**/
VOID
ChipCS0ToLoopback (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  This functions enables TxOn in datacontrol0

  @param Host     - Pointer to sysHost
  @param socket   - Socket number

  @retval - None
**/
VOID
ToggleTxOnFnv (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   chBitmask,
  UINT8    dimm,
  UINT8    value
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  This functions enables InternalClocks On in datacontrol0

  @param Host     - Pointer to sysHost
  @param socket   - Socket number

  @retval - None
**/
VOID
ToggleInternalClocksOnFnv (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   chBitmask,
  UINT8    dimm,
  UINT8    value
  )
{
  //
  // Place holder function for NULL Library
  //
}

VOID
RPQDrain (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank
  )
{
  //
  // Place holder function for NULL Library
  //
}

VOID
FifoTrainReset (
  PSYSHOST Host,
  UINT8    socket
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Enable/Disable BCOM Mode for DDRT

  @param[in] Host     - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket   - Socket Id
  @param[in] Value    - Enable/Disable

  @retval None
**/
VOID
ToggleBcomFlopEn (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Value
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Enable/Disable BCOM Mode for DDRT on specified Channel

  @param[in] Host     - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket   - Socket Id
  @param[in] Ch       - Channel number (0-based)
  @param[in] Value    - Enable/Disable

  @retval None
**/
VOID
ToggleBcomFlopEnPerCh (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Value
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Enable/Disable DDRT Dq BCOM Ctrl

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - current socket
  @param value   - Enable/Disable

  @retval None
**/
VOID
ToggleBcom (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    value
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Program DA Pattern

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket Id
  @param ch       - Channel number (0-based)
  @param dimm     - Current dimm
  @param pattern  - Pattern to program

  @retval None
**/
VOID
ProgramDAFnvMprPattern (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    pattern
  )
{
  //
  // Place holder function for NULL Library
  //
}

VOID
ProgramDFnvMprPattern (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    pattern
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Set Rank DA

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket Id
  @param ch      - Channel number (0-based)
  @param dimm    - Current dimm
  @param mprMode - Either 0 for Disable or MR3_MPR to Enable

  @retval None
**/
VOID
SetRankDAMprFnv (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    mprMode
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Set Rank D

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket Id
  @param ch       - Channel number (0-based)
  @param dimm     - Current dimm
  @param mprMode  - Either 0 for Disable or MR3_MPR to Enable

  @retval None
**/
VOID
SetRankDMprFnv (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    mprMode
  )
{
  //
  // Place holder function for NULL Library
  //
}

VOID
DdrtBufferReset (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm
  )
{
  //
  // Place holder function for NULL Library
  //
}

VOID
SetFmcTempCrit (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT32   TempCrit
  )
{
  //
  // Place holder function for NULL Library
  //
}

UINT32
GetFmcCoreFreq (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm
  )
{
  return 0;
}  //GetFmcCoreFreq

VOID
ConfigNvmdimmtRidFeedback (
  IN PSYSHOST HOST,
  IN BOOLEAN  RidFeedbackDis
  )
{
  //
  // Place holder function for NULL Library
  //
}

MRC_STATUS
GetFfwDigest (
  IN  PSYSHOST Host,
  IN  UINT8    SocketId,
  IN  UINT8    ChId,
  IN  UINT8    DimmId,
  IN  UINT8    DigestLen,
  OUT UINT32   *FfwDigest,
  OUT UINT32   *FfwDigest1
  )
{
  return MRC_STATUS_SUCCESS;
}

UINT32
SetFnvSystemTime (
  PSYSHOST     Host,
  UINT8        socket,
  UINT8        ch,
  UINT8        dimm,
  UINT8        *mbStatus
  )
{
  return SUCCESS;
}

/**
  Initializes Dcpmm Power Management Policy

  This routine issues GetPowerManagementPolicy and SetPowerManagementPolicy command.

  @param[in]  Host                  - Pointer to System Host (root) structure
  @param[in]  Socket                - Socket Number
  @param[in]  Ch                    - DDR Channel ID
  @param[in]  Dimm                  - Dimm number of the channel
  @param[in]  FisVersion            - FisVersion of the firmware interface
**/
VOID
InitDcpmmPowerManagementPolicy (
  IN  SYSHOST *Host,
  IN  UINT8   Socket,
  IN  UINT8   Ch,
  IN  UINT8   Dimm,
  IN  UINT16  FisVersion
  )
{
  //
  // Place holder function for NULL Library
  //
}

UINT32
SetPlatformConfigData (
  IN  SYSHOST      *Host,
  IN  UINT8         Socket,
  IN  UINT8         Ch,
  IN  UINT8         Dimm,
  IN  UINT8         PartitionId,
  IN  UINT32        Offset,
  IN  FNVCISBUFFER  FnvBuffer,
  OUT UINT8        *MailboxStatus
  )
{
  //
  // Place holder function for NULL Library
  //
  return SUCCESS;
}

UINT32
GetPlatformConfigData (
  IN  SYSHOST      *Host,
  IN  UINT8         Socket,
  IN  UINT8         Ch,
  IN  UINT8         Dimm,
  IN  UINT8         PartitionId,
  IN  UINT8         RetrieveOption,
  IN  UINT32        Offset,
  OUT FNVCISBUFFER  FnvBuffer,
  OUT UINT8        *MailboxStatus
  )
{
  return SUCCESS;
}

/**
  Sets Latch System Shutdown State

  @param[in]  Host     - Pointer to System Host (root) structure
  @param[in]  Socket   - Socket Number
  @param[in]  Ch       - DDR Channel ID
  @param[in]  Dimm     - Dimm number of the channel
  @parma[in]  State    - Latch System Shutdown State (enable/disable)
  @param[out] MbStatus - Pointer to 8 bit status code from status register
**/
UINT32
SetLatchSystemShutdownState (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     State,
  OUT  UINT8    *MbStatus
  )
{
  return SUCCESS;
}

/**
  Sets Extended ADR State

  @param[in]  Host     - Pointer to System Host (root) structure
  @param[in]  Socket   - Socket Number
  @param[in]  Ch       - DDR Channel ID
  @param[in]  Dimm     - Dimm number of the channel
  @parma[in]  State    - Extended ADR State (enable/disable)
  @param[out] MbStatus - Pointer to 8 bit status code from status register
**/
UINT32
SetExtendedAdrState (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     State,
  OUT  UINT8    *MbStatus
  )
{
  return SUCCESS;
}

UINT32
GetSecurityState (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT32   *security,
  UINT32   *OptStatus,
  UINT8    *mbStatus
  )
{
  return SUCCESS;
}

UINT32
SecureEraseUnit (
  PSYSHOST      Host,
  UINT8         socket,
  UINT8         ch,
  UINT8         dimm,
  UINT8         *passphrase,
  UINT8         *mbStatus
  )
{
  return SUCCESS;
}

UINT32
SetDimmPartitionInfo (
  PSYSHOST      Host,
  UINT8         socket,
  UINT8         ch,
  UINT8         dimm,
  UINT32        twoLM,
  UINT32        PM,
  FNVCISBUFFER fnvBuffer,
  UINT8        *mbStatus
  )
{
  return SUCCESS;
}

UINT32
GetDimmPartitionInfo (
  PSYSHOST     Host,
  UINT8        socket,
  UINT8        ch,
  UINT8        dimm,
  FNVCISBUFFER fnvBuffer,
  UINT8        *mbStatus
  )
{
  return SUCCESS;
}

/**
  Routine Description:
  SaveDdrtTrainingFailureCode will send the BIOS training failure code to the DCMPMM DIMM

  @param[in]  WarningCode           - Major warning code.
  @param[in]  MinorWarningCode      - Minor warning code.
  @param[in]  Socket                - Socket Number
  @param[in]  Channel               - DDR Channel ID
  @param[in]  Dimm                  - Dimm number of the channel

  @retval 0 SUCCESS
  @retval 1 FAILURE
**/
UINT32
SaveDdrtTrainingFailureCode (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm
)
{
  return SUCCESS;
}

/**
  Issues Identify command to DIMM and returns info provided.

  @param[in]  Host          - Pointer to the system Host (root) structure
  @param[in]  Socket        - Socket Number
  @param[in]  Ch            - DDR Channel ID
  @param[in]  Dimm          - DIMM number
  @param[in]  AitDramFlag   - AIT Dram Flag to check if BIOS support for FIS 1.11 is enabled/disabled
  @param[out] DimmInfo      - Pointer to DIMM information structure
  @param[out] MailboxStatus - Pointer to 8 bit status code from status register

  @retval 0 SUCCESS
  @retval 1 FAILURE
**/
UINT32
IdentifyDimm (
  IN  SYSHOST          *Host,
  IN  UINT8             Socket,
  IN  UINT8             Ch,
  IN  UINT8             Dimm,
  IN  BOOLEAN           AitDramFlag,
  OUT DIMM_INFO_STRUCT *DimmInfo,
  OUT UINT8            *MailboxStatus
  )
{
  return SUCCESS;
}

UINT32
GetFnvInfo (
  PSYSHOST            Host,
  UINT8               socket,
  UINT8               ch,
  UINT8               dimm,
  FNVINFORMATION      FnvInformation,
  UINT8               *mbStatus
  )
{
  return SUCCESS;
}

UINT32
FnvPollingBootStatusRegisterCh (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT32   bitmask
  )
{
  return SUCCESS;
}

UINT32
FnvPollingBootStatusRegister (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   bitmask
  )
{
  return SUCCESS;
}

UINT32
FnvDdrtIoInit (
  PSYSHOST      Host,
  UINT8         socket,
  UINT8         payload
  )
{
  return SUCCESS;
}

UINT32
FnvMailboxInterfaceStart (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT32   inputPayload[NUM_PAYLOAD_REG],
  UINT32   opcode
  )
{
  return SUCCESS;
}

UINT32
FnvMailboxInterfaceFinish (
  PSYSHOST     Host,
  UINT8        socket,
  UINT8        ch,
  UINT8        dimm,
  UINT32       outputPayload[NUM_PAYLOAD_REG],
  UINT32       opcode,
  UINT8        *mbStatus
  )
{
  return SUCCESS;
}

UINT32
SetDieSparingPolicy (
  PSYSHOST       Host,
  UINT8          socket,
  UINT8          ch,
  UINT8          dimm,
  UINT8          Enable,
  UINT8          *mbStatus
  )
{
  return SUCCESS;
}

UINT32
GetDieSparingPolicy (
  PSYSHOST       Host,
  UINT8          socket,
  UINT8          ch,
  UINT8          dimm,
  UINT8          *Enable,
  UINT8          *supported,
  UINT8          *mbStatus
  )
{
  return SUCCESS;
}

VOID
ReadBootStatusRegisters (
  IN  SYSHOST                   *Host,
  IN  UINT8                      Socket,
  IN  UINT8                      Ch,
  IN  UINT8                      Dimm,
  OUT BOOT_STATUS_REGISTER      *BootStatusRegister,
  OUT BOOT_STATUS_HIGH_REGISTER *BootStatusHighRegister
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Clears FW logs & resets registers to default state.

  @param[in]  Host     - Pointer to System Host (root) structure
  @param[in]  Socket   - Socket Number
  @param[in]  Ch       - DDR Channel ID
  @param[in]  Dimm     - Dimm number of the channel
  @param[out] MbStatus - Pointer to 8 bit status code from status register
**/
UINT32
FactoryResetClear (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       Dimm,
  IN  UINT8      *MbStatus
  )
{
  return SUCCESS;
}

/**
  Routine Description: GetRegIndex will return the index of the NVMDIMM
  Mailbox Register that are #defined in the NVMCTLR Register header.

  Arguments.

  @param RegSPA      - Register that is in MBSpa.

  @retval Index of the register if found, else -1.
**/
INT16
GetRegIndex (
  UINT64    Reg
  )
{
  return -1;
}

/**
  Muli-use function to either get or set control delays based on the provided group number

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param dimm     - DIMM number
  @param mode     - GSM_READ_CSR - Read the data from hardware and not cache
                  -  GSM_READ_ONLY - Do not write
                  -  GSM_WRITE_OFFSET - Write offset and not value
                  -  GSM_FORCE_WRITE - Force the write
  @param value    - Value to program or offset
  @param *minVal  - Current minimum control delay
  @param *maxVal  - Current maximum control delay

  @retval Pi delay value
**/
MRC_STATUS
GetSetCtlDelayFnv (
  PSYSHOST    Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    mode,
  INT16    *value,
  UINT16   *minDelay,
  UINT16   *maxDelay
  )
{
  return MRC_STATUS_SUCCESS;
}

/**
  Muli-use function to either get or set command delays based on the provided group number

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param dimm     - DIMM number
  @param group    - Group number
  @param mode     - GSM_READ_CSR - Read the data from hardware and not cache
                  -  GSM_READ_ONLY - Do not write
                  -  GSM_WRITE_OFFSET - Write offset and not value
                  -  GSM_FORCE_WRITE - Force the write
  @param value    - Value to program
  @param *minVal  - Current minimum command delay
  @param *maxVal  - Current maximum command delay

  @retval minVal and maxVal
**/
MRC_STATUS
GetSetCmdGroupDelayFnv (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value,
  UINT16    *minVal,
  UINT16    *maxVal
  )
{
  return MRC_STATUS_SUCCESS;
}

/**
  Muli-use function to either get or set command delays based on the provided group number

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param ioGroup  - Group number
  @param side     - Side number
  @param mode     - GSM_READ_CSR - Read the data from hardware and not cache
                  -  GSM_READ_ONLY - Do not write
                  -  GSM_WRITE_OFFSET - Write offset and not value
                  -  GSM_FORCE_WRITE - Force the write
  @param value    - Value to program or offset

  @retval Pi delay value
**/
MRC_STATUS
GetSetCmdDelayFnv (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  MRC_GT   group,
  UINT8    mode,
  INT16    *value
  )
{
  return MRC_STATUS_SUCCESS;
}

MRC_STATUS
GetSetCmdVrefFnv (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     mode,
  INT16     *value
  )
{
  return MRC_STATUS_SUCCESS;
}

MRC_STATUS
GetSetCmdVrefFnvBackside (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     mode,
  INT16     *value
  )
{
  return MRC_STATUS_SUCCESS;
}

/**
  Muli-use function to either get or set clock delays based on the provided group number

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param clk      - Clock number
  @param mode     - GSM_READ_CSR - Read the data from hardware and not cache
                  -  GSM_READ_ONLY - Do not write
                  -  GSM_WRITE_OFFSET - Write offset and not value
                  -  GSM_FORCE_WRITE - Force the write
  @param value    - Value to program

  @retval Pi delay value
**/
MRC_STATUS
GetSetClkDelayFnv (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    mode,
  INT16    *value
  )
{
  return MRC_STATUS_SUCCESS;
}

/**
  Display FNV/EKV Media/Thermal Error Log Information & Entries

  @param Host           Pointer to the system host (root) structure
  @param Socket         Socket Number
  @param Ch             DDR Channel ID
  @param Dimm           DIMM Number
  @param LogType        Log type
                        0: Media Log
                        1: Thermal Log
  @param LogInfo        Log info
                        0: Retrieve Log Data
                        1: Retrieve Log Info (SequenceNumber and RequestCount are ignored)
  @param ReturnData     Pointer to the structure to hold the results
**/
VOID
FnvDisplayErrorLog (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Ch,
  UINT8                 Dimm,
  UINT8                 LogType,
  UINT8                 LogInfo,
  ERROR_LOG_RETURN_DATA *ReturnData
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Retrieves Error Log from FNV/EKV FW

  @param Host           Pointer to the system host (root) structure
  @param Socket         Socket Number
  @param Ch             DDR Channel ID
  @param Dimm           DIMM Number
  @param LogType        Log type
                        0: Media Log
                        1: Thermal Log
  @param LogLevel       Log level
                        0: Low Priority Log
                        1: High Priority Log
  @param LogInfo        Log info
                        0: Retrieve Log Data
                        1: Retrieve Log Info (SequenceNumber and RequestCount are ignored)
  @param SequenceNumber Sequence number to start at
  @param RequestCount   Number of log entries to request from FW
  @param ReturnData     Pointer to the structure to hold the results
  @param MailboxStatus  Pointer to 8 bit status code from status register

  @retval 0 SUCCESS
  @retval 1 FAILURE
**/
UINT32
FnvGetErrorLog (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Ch,
  UINT8                 Dimm,
  UINT8                 LogType,
  UINT8                 LogLevel,
  UINT8                 LogInfo,
  UINT16                SequenceNumber,
  UINT16                RequestCount,
  ERROR_LOG_RETURN_DATA *ReturnData,
  UINT8                 *MailboxStatus
  )
{
  return SUCCESS;
}

/**
  Retrieves then displays Error Log information from FNV/EKV over SMBUS mailbox

  @param Host           Pointer to the system host (root) structure
  @param Socket         Socket Number
  @param Ch             DDR Channel ID
  @param Dimm           DIMM Number
  @param LogType        Log type
                        0: Media Log
                        1: Thermal Log
  @param LogLevel       Log level
                        0: Low Priority Log
                        1: High Priority Log

  @retval 0 SUCCESS
          1 FAILURE
**/
UINT32
FnvGetSmbusErrorLog (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Ch,
  UINT8                 Dimm,
  UINT8                 LogType,
  UINT8                 LogLevel
  )
{
  return SUCCESS;
}

/**
  This routine caches the FNV DDRTIO CSRs required by training steps

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Ch                                - Channel number (0-based)

  @retval N/A
**/
VOID
UpdateFnvDdrioCache10nm (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  //
  // Place holder function for NULL Library
  //
}

VOID
EarlyFnvConfigAccess (
  PSYSHOST                  Host,
  UINT8                     socket
  )
{
  //
  // Place holder function for NULL Library
  //
}

VOID
LateFnvConfigAccess (
  PSYSHOST                  Host,
  UINT8                     socket
  )
{
  //
  // Place holder function for NULL Library
  //
}

UINT32
ReadFnvCfg (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT32    Reg
  )
{
  return 0;
}

UINT32
WriteFnvCfg (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT32    Reg,
  IN  UINT32    Data
  )
{
  return SUCCESS;
}

/**
  Routine Description: Collect necessary SPD data for CR DXE/SMM driver

  @param[in] Host         - Pointer to the system host (root) structure
  @param[in] Socket       - Socket Number
  @param[in] Channel      - DDR Channel ID
  @param[in] Dimm         - DIMM Number

  @retval N/A
**/
VOID
GatherDdrtSPDData (
  IN  SYSHOST                 *Host,
  IN  UINT8                    Socket,
  IN  UINT8                    Ch,
  IN  UINT8                    Dimm
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Returns the capacity of the DCPMM DIMM at the input Socket, Channel, and DIMM
  index in units of GB. Returns a size of zero if an error is encountered.

  @param[in] Socket:       Socket index
  @param[in] Channel:      Channel index within the socket
  @param[in] Dimm:         DIMM slot index within the channel

  @return The capacity of the DCPMM DIMM in units of GB

**/
UINT32
GetDcPmmDimmSize (
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm
  )
{
  //
  // Place holder function for NULL Library
  //
  return 0;
}

/**
  Checks DQ swizzling and programs registers to account for it

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket Id

  @retval Non
**/
VOID
CheckAndProgramDqSwizzleAep (
  PSYSHOST Host,
  UINT8 socket
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Enable / disable 32 B2B GNTs without NODATA. Needed only for DDRT

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Value                             - Enable / Disable

  @retval N/A
**/
VOID
Grant32ContEn (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Value
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**

  BIOS shall drive tristate on RespA and RespB

  @param[in]  Socket    - Socket

  @retval status

  **/
VOID
EFIAPI
DdrtDriveRespARespBHigh (
  IN  UINT8     Socket
  )
{
  // NULL
} // DdrtDriveRespARespBHigh

/**
  Routine Description: EmulationPoll is an assembly polling loop for reading CSRs
  EmulationPoll allows you to poll multiple bits

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket Id
  @param BoxInst - Box Instance, 0 based
  @param Offset  - Register offset; values come from the auto generated header file
  @param mask    - Bit mask of bits we are polling
  @param logic   - Polling for 0 or 1

  @retval None
**/
VOID
EmulationPoll (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 BoxInst,
  UINT32 Offset,
  UINT32 mask,
  UINT8 logic
  )
{
  //
  // Place holder function for NULL Library
  //
}

VOID
Resync (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    clk
  )
{
  //
  // Place holder function for NULL Library
  //
}

VOID
RelockPll (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    clk
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  DDRT Clock Movement

  @param[in]      Socket      - Socket number
  @param[in]      Ch          - Memory channel number
  @param[in]      OrgClk      - Original clk number with step size of clkpi/128
  @param[in]      TargetClk   - Target clk number with step size of clkpi/128
  @param[in, out] NewClk      - Pointer to New clk number with step size of clkpi/128

  @retval TRUE  - Allowed
          FALSE - Not allowed
*/
BOOLEAN
EFIAPI
IsDdrtClockMovementAllowed (
  IN     UINT8  Socket,
  IN     UINT8  Ch,
  IN     UINT8  OrgClk,
  IN     UINT8  TargetClk,
  IN OUT UINT8  *NewClk
  )
{
  return TRUE;
}

/**
  Get min/max limits for Command, Control and Clock signals

  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Ch          - Channel number
  @param[in]  Dimm        - DIMM number
  @param[in]  Level       - IO level to access
  @param[in]  Group       - Command, clock or control group to access
  @param[out] *MinLimit   - Minimum delay value allowed
  @param[out] *MaxLimit   - Maximum delay value allowed

  @retval EFI_SUCESS if signal max/mins are returned
          EFI_NOT_FOUND otherwise
**/
EFI_STATUS
GetCmdGroupLimitsFmc (
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     Dimm,
  IN    MRC_LT    Level,
  IN    MRC_GT    Group,
  OUT   UINT16    *MinLimit,
  OUT   UINT16    *MaxLimit
  )
{
  return EFI_SUCCESS;
}

/**
  Determine if disabled Nvmdimm got recovered by DFX format from previous boot

  @param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket      - Processor socket within the system (0-based)

  @retval TRUE  - NVMDIMM is recovered
          FALSE - NVMDIMM is not recovered
**/
BOOLEAN
DetectNvmdimmDisableChange (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  return FALSE;
}

/**
  Routine Description: This function returns an valid Index ID 0-5 to decide Top 6 frequently accessed registers need to be cached
  @param Host      - pointer to System Host (root) structure
  @param Socket    - Socket ID
  @param Ch        - Channel Number
  @param Dimm      - Dimm Number
  @param Register  - Register

  @retval status
**/
UINT8
GetFMCCacheIndex (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT32    Reg
  )
{
  return 0;
}

/**
  Return the access mode for a given channel's far memory controller (FMC).

  @param  Socket  The socket where the channel of interest is located
  @param  Channel The channel of interest

  @retval The access mode for the far memory controller
**/
BUS_TYPE
EFIAPI
GetFmcAccessMode (
  IN  UINT8 Socket,
  IN  UINT8 Channel
  )
{
  return SMBUS;
}

/**
  Set the access mode for a given channel's far memory controller (FMC).

  @param  Socket  The socket where the channel of interest is located
  @param  Channel The channel of interest
  @param  Mode    The access mode for the far memory controller

  @retval N/A
**/
VOID
EFIAPI
SetFmcAccessMode (
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  BUS_TYPE  Mode
  )
{
  //
  // Place holder function for NULL Library
  //
}

/** Get the type of FMC on a given channel

  @param[in]  Socket  The socket containing the DDRT DIMM
  @param[in]  Channel The channel containing the DDRT DIMM

  @retval The type of FMC (e.g. FMC_EKV_TYPE, FMC_BWV_TYPE, etc.)
**/
UINT8
EFIAPI
GetFmcType (
  IN  UINT8 Socket,
  IN  UINT8 Channel
  )
{
  return FMC_NONE_TYPE;
}

/**
  Enable/Disable NVMDIMM C2

  @param[in] Host    - Pointer to sysHost, the system host (root) structure struct
  @param[in] Socket  - current socket
  @param[in] Value   - TRUE: Disable, FALSE: Enable

  @retval None
**/
VOID
ConfigNvmDimmC2 (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN BOOLEAN Value
  )
{
  //
  // Place holder function for NULL Library
  //
}

/** DDRT 2.0: For CWV A0, BIOS must issue BRST to the databuffer when it asserts DRST low.

  @param[in] Host           Pointer to the system host (root) structure
  @param[in] Socket         The socket number where the DIMM resides
  @param[in] Channel        The channel number where the DIMM resides on the provided socket

  @retval None

**/
VOID
DdrtAssertBrst (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Channel
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Displays the contents of the FW Boot Status Registers.

  @param[in] Host   - Pointer to System Host (root) structure
  @param[in] Socket - Socket Number
  @param[in] Ch     - DDR Channel ID
  @param[in] Dimm   - Dimm number of the channel

  @retval VOID      - None
**/
VOID
ShowBsr (
  IN SYSHOST *Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm
  )
{
  //
  // Place holder function for NULL Library
  //
}

/**
  Sets performance knob value based on knob and CR modes to FNV/EKV FW

  This routine issues the Set Features / Performance Knob command and
  sets the data from the provided buffer.

  @param[in]  Host                  - Pointer to System Host (root) structure
  @param[in]  Socket                - Socket Number
  @param[in]  Ch                    - DDR Channel ID
  @param[in]  Dimm                  - Dimm number of the channel
  @param[in]  PerformanceKnob       - Pointer to structure containing DIMM performance knob information
  @param[out] MailboxStatus         - Pointer to 8 bit status code from status register

  @retval 0                         - SUCCESS
  @retval 1                         - FAILURE
**/
UINT32
SetPerformanceKnob (
  IN  SYSHOST                 *Host,
  IN  UINT8                    Socket,
  IN  UINT8                    Ch,
  IN  UINT8                    Dimm,
  IN  PERFORMANCE_KNOB        *PerformanceKnob,
  OUT UINT8                   *MailboxStatus
  )
{
  return SUCCESS;
}

/**
  Check if training has failed on previous boot

  @param[in]  Socket                - Socket Number

  @retval                           - MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
CheckDdrtIoTrainingStatus (
  IN  UINT8                    Socket
  )
{
  return MRC_STATUS_SUCCESS;
}

/**
  Checks to see if the DDRT Round Trip value timing should be used

  @param[in]  Socket                - Socket Number
  @param[in]  Channel               - DDR Channel ID

  @retval  TRUE                     - DDRT Round Trip value should be used
  @retval  FALSE                    - Default Round Trip value should be used
**/
BOOLEAN
EFIAPI
DdrtRoundTripIsNeeded (
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  return FALSE;
}

/**
  Determine if clock movement should be avoided to prevent FMC from hanging.

  @param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket      - Processor socket within the system (0-based)

  @retval TRUE  - clock movement should be avoided
  @retval FALSE - clock movement is allowed.

**/
BOOLEAN
FmcAvoidClockMovementInSocket (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  return FALSE;
}

/**
  Determine if clock movement should be avoided to prevent FMC from hanging.

  @param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Channel     - Channel within the Socket

  @retval TRUE  - clock movement should be avoided
  @retval FALSE - clock movement is allowed.

**/
BOOLEAN
FmcAvoidClockMovementInChannel (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  return FALSE;
}

/**
  Determine if clock movement should be avoided to prevent FMC from hanging.

  @param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Channel     - Channel within the Socket
  @param[in]  Dimm        - Dimm number within the Channel

  @retval TRUE  - clock movement should be avoided
  @retval FALSE - clock movement is allowed.

**/
BOOLEAN
FmcAvoidClockMovementInDimm (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm
  )
{
  return FALSE;
}

/**
  FMC CSR unlock sequence for a channel

  @param[in] Socket      - Processor socket ID
  @param[in] Ch          - Current channel

  @retval EFI_UNSUPPORTED if the feature is not supported
  @retval EFI_SUCCESS otherwise

**/
EFI_STATUS
EFIAPI
FmcCsrUnlockCh (
  IN  UINT8    Socket,
  IN  UINT8    Ch
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Remap MRA to CWV address

  @param[in]      Host        - Pointer to sysHost
  @param[in]      Socket      - Socket Id
  @param[in]      Ch          - Ch number
  @param[in]      Dimm        - Dimm number (zero-based)
  @param[in out]  Address     - Address for the MRS write
  @param[in out]  Cw          - CW flag

  @retval none
**/
VOID
RemapMraAddress (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      Dimm,
  IN OUT UINT16 *Address,
  IN OUT UINT8  *Cw
  )
{
}

/**
  Enable Resync FSM on all DDRT DIMMs present on the given socket

  @param Socket   Socket number

  @retval         None
**/
VOID
FnvEnableResyncFsm (
  UINT8    Socket
  )
{
  return;
}

/**
  Determine if the given socket and channel is populated with a DIMM that may require frequency limitations

  @param[in]    Host    Pointer to host structure
  @param[in]    Socket  Socket number to check
  @param[in]    Channel Channel number to check

  @retval   FALSE   No DCPMM DIMMs found on the given socket & channel that require frequency limitations
  @retval   TRUE    DCPMM DIMMs found on the given socket & channel that may require frequency limitations
**/
BOOLEAN
EFIAPI
FmcEnforceDcpmmFreqLimit (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  return FALSE;
}

/**
  Collects DDRT Freq from B2P Mailbox, and compare if setup current Freq limit same as DIMM init freq

  @param[in] Host      - Point to sysHost
  @param[in] Socket    - CPU socket ID

  @retval TRUE  - Dcpmem freq is changed
          FALSE - Dcpmem freq is not changed

**/
BOOLEAN
FmcCheckDdrtFreqChanged (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket
  )
{
  return FALSE;
}

/**
  Program direct snoop settings to DQ buffer

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket Id
  @param[in] Ch          - Ch number
  @param[in] SubChMask   - Mask of subchannel numbers to be updated
  @param[in] Dimm        - Dimm number (zero-based)
  @param[in] Rank        - Rank number (zero-based)
  @param[in] Address     - Address for the MRS write
  @param[in] Cw          - Data to write

  @retval none
**/
VOID
EFIAPI
ProgramSnoopSettingsToDb (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      SubChMask,
  IN UINT8      Dimm,
  IN UINT8      Rank,
  IN UINT8      Address,
  IN UINT8      Data
  )
{
  // Null
}

/**
  Program DDRT2 Dram TxDqDelay

  @param[in] Host -  Pointer to sysHost
  @param[in] Dimm - Dimm number(zero - based)
  @param[in] Value - value for ReadDqsOffset

  @retval N / A
**/
VOID
ProgramDramTxDqDelayDdrt2 (
  IN   PSYSHOST  Host,
  IN   UINT8     Dimm,
  IN   UINT8     Value
  )
{
  return;
}

/**
Puts the BGF into reset. Prevents the Core from decoding a misaligned command.

@param[in]  Socket                The socket containing the target DIMM.
@param[in]  Channel               The channel containing the target DIMM.
@param[in]  Dimm                  The target DIMM
@param[in]  Value                 Value to put BFG in reset or not

@retval   None
**/

VOID
BgfReset(
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm,
  IN  UINT8     Value
)
{
  return;
}

/**
  Get internal write leveling WL_ADJ_end value.

  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Channel     - Channel within the Socket
  @param[in]  Dimm        - Dimm number within the Channel

  @retval   - write leveling WL_ADJ_end result

**/
UINT16
GetDdrt2WLAdjEndValue (
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm
  )
{
  return 0;
}

/**
General function for Get/Set CS Vref for CWV

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket Id
  @param[in] Ch          - Ch number
  @param[in] Dimm        - Dimm number (zero-based)
  @param[in] SubCh       - Sub channel number
  @param[in] Signal      - Signal to train (zero-based)
  @param[in] Mode        - GSM_READ_CSR - Read the data from hardware and not cache
                         - GSM_FORCE_WRITE - Force the write
  @param[in out] *CsVrefValue - Vref setting % of VDD (zero-based)

  @retval MRC_STATUS_SUCCESS if entry is returned
  @retval MRC_STATUS_FAILURE if the entry is not supported
  @retval MRC_STATUS_UNSUPPORTED if input not supported mode
  @retval MRC_STATUS_SIGNAL_NOT_SUPPORTED if signal is not supported
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
GetSetDcsVrefDdrt2 (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      Dimm,
  IN UINT8      SubCh,
  IN UINT8      Signal,
  IN UINT8      Mode,
  IN OUT UINT8  *CsVrefValue
)
{
  return MRC_STATUS_SUCCESS;
}

/**
General function for Get/Set CA Vref for FMC

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket Id
  @param[in] Ch          - Ch number
  @param[in] Dimm        - Dimm number (zero-based)
  @param[in] SubCh       - Sub channel number
  @param[in] Signal      - Signal to train (zero-based)
  @param[in] Mode        - GSM_READ_CSR - Read the data from hardware and not cache
                         - GSM_FORCE_WRITE - Force the write
  @param[in out] *CaVrefValue - Vref setting % of VDD (zero-based)

  @retval MRC_STATUS_SUCCESS if entry is returned
  @retval MRC_STATUS_FAILURE if the entry is not supported
  @retval MRC_STATUS_UNSUPPORTED if input not supported mode
  @retval MRC_STATUS_SIGNAL_NOT_SUPPORTED if signal is not supported
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
GetSetDcaVrefDdrt2 (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Ch,
  IN UINT8        Dimm,
  IN UINT8        SubCh,
  IN GSM_CSN      Signal,
  IN UINT8        Mode,
  IN OUT UINT8    *CaVrefValue
)
{
  return MRC_STATUS_SUCCESS;
}
