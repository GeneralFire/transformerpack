/** @file
  Host OS Socket Access Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2017 Intel Corporation. <BR>

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

#include "BaseCteNetLibNull.h"

//
// API functions available to
// users of this library
//

/**
  Read a register address from an MRC server.

  @param [in] Address      Address to read.
  @param [in] Data         Contents of address.

  @retval  EFI_SUCCESS      Read succeeded.
  @retval  !EFI_SUCCESS     Read failed.

**/

EFI_STATUS
EFIAPI
CteReadAddr (
  IN UINT32 Address,
  OUT UINT32 *Data
  )
{

  return EFI_UNSUPPORTED;

} // CteReadAddr

/**
  Write to a register address from an MRC server.

  @param [in] Address      Address to write.
  @param [in] Data         Data to write

  @retval  EFI_SUCCESS      Write succeeded.
  @retval  !EFI_SUCCESS     Write failed.

**/

EFI_STATUS
EFIAPI
CteWriteAddr (
  IN UINT32 Address,
  IN UINT32 Data
  )
{

  return EFI_UNSUPPORTED;

} // CteWriteAddr

/**
  Send write name command to MRC server.

  @param [in] RegFile      Unknown.
  @param [in] Name         Unknown.
  @param [in] Data         data to write.

  @retval  EFI_SUCCESS      Write succeeded.
  @retval  !EFI_SUCCESS     Write failed.

**/

EFI_STATUS
EFIAPI
CteWriteName (
  IN CHAR8 *RegFile,
  IN CHAR8 *Name,
  IN UINT32 Data
  )
{

  return EFI_UNSUPPORTED;

} // CteWriteName

/**
  Send read name command to MRC server.

  @param [in] RegFile        Unknown.
  @param [in] Name           Unknown.
  @param [out] Data          data to read.

  @retval  EFI_SUCCESS  Read succeeded.
  @retval  !EFI_SUCCESS Read failed.

**/

EFI_STATUS
EFIAPI
CteReadName (
  IN CHAR8 *RegFile,
  IN CHAR8 *Name,
  OUT UINT32 *Data
  )
{

  return EFI_UNSUPPORTED;

} // CteReadName

/**
  Send delay time command to MRC server.

  @param [in] DelayTime          Time to delay

  @retval  EFI_SUCCESS      Delay succeeded.
  @retval  !EFI_SUCCESS     Delay failed.

**/

EFI_STATUS
EFIAPI
CteDelayTime (
  IN UINT32 DelayTime
  )
{

  return EFI_UNSUPPORTED;

} // CteDelayTime

/**
  Read SPD data from MRC server

  @param [in] ChannelNumber      Channel Number
  @param [in] DimmNumber         Dimm Number
  @param [out] SpdData           Ptr to buffer
  @param [in] Size               Size of buffer

  @retval  EFI_SUCCESS      Read succeeded.
  @retval  !EFI_SUCCESS     Read failed.

**/

EFI_STATUS
EFIAPI
CteSpdRead (
  IN UINT32 ChannelNumber,
  IN UINT32 DimmNumber,
  OUT UINT8 *SpdData,
  IN OUT UINT32 *Size
  )
{

  return EFI_UNSUPPORTED;

} // CteSpdRead

/**
  Write MRS data to BFM directly

  @param [in] ChannelNumber      Channel Number
  @param [in] DimmNumber         Dimm Number
  @param [in] MrsAddress            MRS addr
  @param [in] MrsData            MRS Data

  @retval  EFI_SUCCESS      Read succeeded.
  @retval  !EFI_SUCCESS     Read failed.

**/

EFI_STATUS
EFIAPI
CteMrsWrite (
  IN UINT32 ChannelNumber,
  IN UINT32 RankNumber,
  IN UINT32 MrsAddress,
  IN UINT32 MrsData
  )
{

  return EFI_UNSUPPORTED;

} // CteMrsWrite

/**
  Send get options command to MRC server.

  @param [in] OptionName    Option name
  @param [out] Data         Data

  @retval  EFI_SUCCESS      Send succeeded.
  @retval  !EFI_SUCCESS     Send failed.

**/

EFI_STATUS
EFIAPI
CteGetOptions (
  IN CHAR8 *OptionName,
  OUT UINT32 *Data
  )
{

  *Data = 0;

  return EFI_UNSUPPORTED;

} // CteGetOptions

/**
  Send lockstep command to MRC server.

  @param [in] LockStepCount      Unknown

  @retval  EFI_SUCCESS      LockStep succeeded.
  @retval  !EFI_SUCCESS     LockStep failed.

**/

EFI_STATUS
EFIAPI
CteLockstep (
  IN UINT32 LockStepCount
  )
{

  return EFI_UNSUPPORTED;

} // CteLockstep

/**
  Send check version command to MRC server.

  @param [in] Project            Unknown
  @param [in] ClientVersion      Unknown
  @param [out] ServerVersion     Unknown

  @retval  EFI_SUCCESS      Check succeeded.
  @retval  !EFI_SUCCESS     Check failed.

**/

EFI_STATUS
EFIAPI
CteCheckVersion (
  IN CHAR8 *Project,
  IN UINT32 ClientVersion,
  OUT UINT32 *ServerVersion
  )
{

  return EFI_UNSUPPORTED;

} // CteCheckVersion

/**
  Send check function command to MRC server.

  @param [in] Function           Unknown
  @param [out] Valid             Unknown

  @retval  EFI_SUCCESS      Check succeeded.
  @retval  !EFI_SUCCESS     Check failed.

**/

EFI_STATUS
EFIAPI
CteCheckFunction (
  IN CHAR8 *Function,
  OUT INT32 *Valid
  )
{

  return EFI_UNSUPPORTED;

} // CteCheckFunction

/**
  Send debug command to MRC server.

  @param [in] HookName           Name of hook

  @retval  EFI_SUCCESS      Send succeeded.
  @retval  !EFI_SUCCESS     Send failed.

**/

EFI_STATUS
EFIAPI
CteDebugHook (
  IN CHAR8 *HookName
  )
{

  return EFI_UNSUPPORTED;

} // CteDebugHook

/**
  Send save data command to MRC server.

  @param [in] File           Unknown
  @param [in] Data           Unknown
  @param [in] Size           Unknown

  @retval  EFI_SUCCESS  Save succeeded.
  @retval  !EFI_SUCCESS Save failed.

**/

EFI_STATUS
EFIAPI
CteSaveData (
  IN CHAR8 *File,
  IN CHAR8 *Data,
  IN UINT64 Size
  )
{

  return EFI_UNSUPPORTED;

} // CteSaveData

/**
  Send read data command to MRC server.

  @param [in] File           Unknown
  @param [in] OutputSize     Unknown

  @retval  EFI_SUCCESS  Send succeeded.
  @retval  !EFI_SUCCESS Send failed.

**/

VOID *
CteReadData (
  IN CHAR8 *File,
  IN UINT64 *OutputSize
  )
{

  return NULL;

} // CteReadData

/**
  Send WriteRcdCfgSmb command to MRC server.

  @param [in] ChannelNumber    Channel number
  @param [in] DimmNumber       Dimm number
  @param [in] RegisterAddress  Register Address
  @param [in] RegisterSize     Register size
  @param [in] Data             Data to write

  @retval  EFI_SUCCESS    Send succeeded.
  @retval  !EFI_SUCCESS   Send failed.

**/

EFI_STATUS
EFIAPI
CteWriteRcdCfgSmb (
  IN UINT32 ChannelNumber,
  IN UINT32 DimmNumber,
  IN UINT32 RegisterAddress,
  IN UINT32 RegisterSize,
  IN UINT32 Data
  )
{

  return EFI_UNSUPPORTED;

} // CteWriteRcdCfgSmb

/**
  Send WriteFnvCfgSmb command to MRC server.

  @param [in] ChannelNumber    Channel number
  @param [in] DimmNumber       Dimm number
  @param [in] RegisterAddress  Register Address
  @param [in] RegisterSize     Register size
  @param [in] Data             Data to write

  @retval  EFI_SUCCESS    Send succeeded.
  @retval  !EFI_SUCCESS   Send failed.

**/

EFI_STATUS
EFIAPI
CteWriteFnvCfgSmb (
  IN UINT32 ChannelNumber,
  IN UINT32 DimmNumber,
  IN UINT32 RegisterAddress,
  IN UINT32 RegisterSize,
  IN UINT32 Data
  )
{

  return EFI_UNSUPPORTED;

} // CteWriteFnvCfgSmb

/**
  Send ReadRcdCfgSmb command to MRC server.

  @param [in] ChannelNumber    Channel number
  @param [in] DimmNumber       Dimm number
  @param [in] RegisterAddress  Register Address
  @param [in] RegisterSize     Register size
  @param [out] Data            Data to read

  @retval  EFI_SUCCESS    Send succeeded.
  @retval  !EFI_SUCCESS   Send failed.

**/

EFI_STATUS
EFIAPI
CteReadRcdCfgSmb (
  IN UINT32 ChannelNumber,
  IN UINT32 DimmNumber,
  IN UINT32 RegisterAddress,
  IN UINT32 RegisterSize,
  OUT UINT32 *Data
  )
{

  return EFI_UNSUPPORTED;

} // CteReadRcdCfgSmb

/**
  Send ReadFnvCfgSmb command to MRC server.

  @param [in] ChannelNumber    Channel number
  @param [in] DimmNumber       Dimm number
  @param [in] RegisterAddress  Register Address
  @param [in] RegisterSize     Register size
  @param [out] Data             Data to read

  @retval  EFI_SUCCESS    Send succeeded.
  @retval  !EFI_SUCCESS   Send failed.

**/

EFI_STATUS
EFIAPI
CteReadFnvCfgSmb (
  IN UINT32 ChannelNumber,
  IN UINT32 DimmNumber,
  IN UINT32 RegisterAddress,
  IN UINT32 RegisterSize,
  OUT UINT32 *Data
  )
{

  return EFI_UNSUPPORTED;

} // CteReadFnvCfgSmb

/**
  Send programCADB command to MRC server.

  @param [in] ChannelNumber    Channel number
  @param [in] CadbEntry        Unknown
  @param [in] Cadb0            Unknown
  @param [in] Cadb1            Unknown

  @retval  EFI_SUCCESS    Send succeeded.
  @retval  !EFI_SUCCESS   Send failed.

**/

EFI_STATUS
EFIAPI
CteProgramCadb (
  IN UINT32 ChannelNumber,
  IN UINT32 CadbEntry,
  IN UINT32 Cadb0,
  IN UINT32 Cadb1
  )
{

  return EFI_UNSUPPORTED;

} // CteProgramCadb

/**
  Send programWBD function to MRC server.

  @param [in] ChannelNumber    Channel Number
  @param [in] WdbEntry         Unknown
  @param [in] Wdb0             Unknown
  @param [in] Wdb1             Unknown

  @retval  EFI_SUCCESS    Send succeeded.
  @retval  !EFI_SUCCESS   Send failed or server returned error.

**/

EFI_STATUS
EFIAPI
CteProgramWdb (
  IN UINT32 ChannelNumber,
  IN UINT32 WdbEntry,
  IN UINT32 Wdb0,
  IN UINT32 Wdb1
  )
{

  return EFI_UNSUPPORTED;

} // CteProgramWdb

/**
  Send IsFnvFeedbackPresent command to MRC server.

  @param None

  @retval  Command result from server

**/

UINT32
EFIAPI
CteIsFnvFeedbackPresent (
  VOID
  )
{

  return 0;

} // CteIsFnvFeedbackPresent

/**
  Report an error to the MRC server.

  @param [in] ErrorCode      Error code
  @param [in] Description    Description of error

  @retval  EFI_SUCCESS  Report succeeded.
  @retval  !EFI_SUCCESS Report failed.

**/

EFI_STATUS
EFIAPI
CteReportError (
  IN INT32 ErrorCode,
  IN CHAR8 *Description
  )
{

  return EFI_UNSUPPORTED;

} // CteReportError

/**
  Send delay to MRC server

  @param [in] i    Delay time

  @retval     None

**/

VOID
CteDelayQclk (
  IN INT32 i
  )
{

  return;

} // CteDelayQclk

/**
  Send delay to MRC server

  @param[in]                Delay     Delay time in QCLKS

**/
VOID
HvmDelayQclk (
  IN        INT32           Delay
  )
{

  return;

} // HvmDelayQclk

/**
  Send delay to MRC server

  @param [in] i    Delay time

  @retval     None

**/

VOID
CteDelayDclk (
  IN INT32 i
  )
{

  return;

} // CteDelayDclk

/**
  Send delay to MRC server

  @param [in] i    Delay time

  @retval     None

**/

VOID
CteDelayRef (
  IN INT32 i
  )
{

  return;

} // CteDelayRef

/**
  Connect to a MRC server.

  @param [in]  ConnectionName Name of the connection.
  @param [out] Host           Host to connect to.
  @param [in]  Port           Port to connect on.


  @retval  EFI_SUCCESS      Connection succeeded.
  @retval  !EFI_SUCCESS     Connection failed.

**/

EFI_STATUS
EFIAPI
CteConnect (
  IN CHAR8 *ConnectionName,
  IN CHAR8 *Host,
  IN UINT16 Port
  )
{

  return EFI_UNSUPPORTED;

} // CteConnect

/**
  Disconnect from a MRC server.

  @retval  EFI_SUCCESS      Disconnection succeeded.
  @retval  !EFI_SUCCESS     Disconnection failed.

**/

EFI_STATUS
EFIAPI
CteClientDisconnect (
  VOID
  )
{

  return EFI_UNSUPPORTED;

} // CteClientDisconnect

/**
  Send get_time function to MRC server.

  @param [in] TimeMsb    Unknown
  @param [in] TimeLsb    Unknown

  @retval  EFI_SUCCESS    Send succeeded.
  @retval  !EFI_SUCCESS   Send failed or server returned error.

**/

EFI_STATUS
EFIAPI
CteGetTime (
  OUT UINT32 *TimeMsb,
  OUT UINT32 *TimeLsb
  )
{

  return EFI_UNSUPPORTED;

} // CteGetTime

/**
  Send exit function to MRC server.

  @param [in] ServerStatus    Status to send

  @retval N/A
**/

VOID
EFIAPI
CteExit (
  IN INT32 StatusToSend
  )
{

  return;

} // CteExit

/**
  Send read pattern command to MRC server.

  @param [in] Pattern             Unknown.
  @param [in] PatternInfo         Unknown.
  @param [in] ChannelMask         Unknown.
  @param [in] Ddr5SubChannelMask  Unknown.
  @param [in] Rank                Unknown.

  @retval  EFI_SUCCESS  Send succeeded.
  @retval  !EFI_SUCCESS Send failed.

**/

EFI_STATUS
EFIAPI
CteSendPattern (
  IN CHAR8 *Pattern,
  IN UINT32 PatternInfo,
  IN UINT32 ChannelMask,
  IN UINT32 Ddr5SubChannelMask,
  IN UINT32 Rank
  )
{

  return EFI_UNSUPPORTED;

} // CteSendPattern

/**
  Send get mc error command to MRC server.

  @param [in]  ChannelMask           Channel Mask.
  @param [in]  Ddr5SubChannelMask    Ddr5 SubChannel Mask.
  @param [out] DataEven              DataEven.
  @param [out] EccEven               EccEven.
  @param [out] DataOdd               DataOdd.
  @param [out] EccOdd                EccOdd.

  @retval  EFI_SUCCESS  Send succeeded.
  @retval  !EFI_SUCCESS Send failed.

**/

EFI_STATUS
EFIAPI
CteGetMcErrorCounter (
  IN  UINT32 ChannelMask,
  IN  UINT32 Ddr5SubChannelMask,
  OUT UINT32 *DataEven,
  OUT UINT32 *EccEven, /* only lower 8 are valid*/
  OUT UINT32 *DataOdd,
  OUT UINT32 *EccOdd   /*only lower 8 are valid*/
  )
{

  return EFI_UNSUPPORTED;

} // CteGetMcErrorCounter

/**
  Setup a pattern to send to MRC server (ULT BASED training)

  @param [in] CmdType       Unknown.
  @param [in] ChMask        Unknown.
  @param [in] Ddr5SubChMask Unknown.
  @param [in] Rank          Unknown.
  @param [in] Data0         Unknown.
  @param [in] Data1         Unknown.

  @retval  EFI_SUCCESS  Read succeeded.
  @retval  !EFI_SUCCESS Read failed.

**/

EFI_STATUS
EFIAPI
CteDdrCmd (
  IN CHAR8 *CmdType,
  IN UINT32 ChMask,
  IN UINT32 Ddr5SubChMask,
  IN UINT32 Rank,
  IN UINT32 Data0,
  IN UINT32 Data1
  )
{

  return EFI_UNSUPPORTED;

} // CteDdrCmd

/**
  Setup a pattern to send to MRC server (ULT BASED training)

  @param [in] Pattern         Unknown.
  @param [in] PatternInfo     Unknown.
  @param [in] ChMask          Unknown.
  @param [in] Ddr5SubChMask   Unknown.
  @param [in] Rank            Unknown.

  @retval  EFI_SUCCESS  Read succeeded.
  @retval  !EFI_SUCCESS Read failed.

**/

EFI_STATUS
EFIAPI
CteSetupPattern (
  IN CHAR8 *Pattern,
  IN UINT32 PatternInfo,
  IN UINT32 ChMask,
  IN UINT32 Ddr5SubChMask,
  IN UINT32 Rank
  )
{

  return EFI_UNSUPPORTED;

} // CteSetupPattern

/**
  Start a pattern to send to MRC server (ULT BASED training)

  @param [in] ChMask          Unknown.
  @param [in] Ddr5SubChMask   Unknown.
  @param [in] Rank            Unknown.

  @retval  EFI_SUCCESS  Read succeeded.
  @retval  !EFI_SUCCESS Read failed.

**/

EFI_STATUS
EFIAPI
CteStartPattern (
  IN UINT32 ChMask,
  IN UINT32 Ddr5SubChMask,
  IN UINT32 Rank
  )
{

  return EFI_UNSUPPORTED;

} // CteStartPattern

/**
  Send debug command to MRC server.

  @param None

  @retval  EFI_SUCCESS      Send succeeded.
  @retval  !EFI_SUCCESS     Send failed.

**/

EFI_STATUS
EFIAPI
CteStopPattern (
  VOID
  )
{

  return EFI_UNSUPPORTED;

} // CteStartPattern

/**
  Send wait pattern done command to MRC server.

  @param None

  @retval  EFI_SUCCESS      Send succeeded.
  @retval  !EFI_SUCCESS     Send failed.

**/

EFI_STATUS
EFIAPI
CteWaitPatternDone (
  VOID
  )
{

  return EFI_UNSUPPORTED;

} // CteWaitPatternDone


/**
  Send set options command to MRC server.

  @param [in] OptionName    Option name
  @param [in] ChMask        Unknown.
  @param [in] ChSubChMask   Unknown.
  @param [in] Rank          Unknown.
  @param [in] Data          Data

  @retval  EFI_SUCCESS      Send succeeded.
  @retval  !EFI_SUCCESS     Send failed.

**/

EFI_STATUS
EFIAPI
CteSetOptions (
  IN CHAR8 *OptionName,
  IN UINT32 ChMask,
  IN UINT32 SubChMask,
  IN UINT32 Rank,
  IN UINT32 Data
  )
{

  return EFI_UNSUPPORTED;

} // CteSetOptions
