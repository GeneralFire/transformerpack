/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2018 Intel Corporation. <BR>

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

#ifndef _CTE_NET_LIB_H_
#define _CTE_NET_LIB_H_

#include <Uefi.h>

#define NO_CARE_PARAMETER 0xFFFFFFFF
/**
  Read a register address from an MRC server.

  @param [in]  Address      Address to read.
  @param [out] Data         Contents of address.

  @retval  EFI_SUCCESS      Read succeeded.
  @retval  !EFI_SUCCESS     Read failed.

**/

EFI_STATUS
EFIAPI
CteReadAddr (
  IN UINT32 Address,
  OUT UINT32 *Data
  );

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
  );

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
  );

/**
  Send read name command to MRC server.

  @param [in]  RegFile       Unknown.
  @param [in]  Name          Unknown.
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
  );

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
  );

/**
  Read SPD data from MRC server

  @param [in]      ChannelNumber      Channel Number
  @param [in]      DimmNumber         Dimm Number
  @param [out]     SpdData            Ptr to buffer
  @param [in, out] Size               Size of buffer

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
  );

/**
  Write MRS data to BFM directly

  @param [in] ChannelNumber      Channel Number
  @param [in] DimmNumber         Dimm Number
  @param [in] MrsAddress         MRS addr
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
  );

//
// Definitions for CteGetOptions
//

#define SIM_PREAMBLE_1TCLK      0
#define SIM_PREAMBLE_2TCLK      1
#define SIM_PREAMBLE_AUTO       2
#define SIM_PREAMBLE_TYPE_WRITE 0
#define SIM_PREAMBLE_TYPE_READ  1

#define SIM_OPTION_DISABLE      0
#define SIM_OPTION_ENABLE       1
#define SIM_OPTION_AUTO         2

/**
  Send get options command to MRC server.

  @param [in]  OptionName    Option name
  @param [out] Data          Data

  @retval  EFI_SUCCESS      Send succeeded.
  @retval  !EFI_SUCCESS     Send failed.

**/

EFI_STATUS
EFIAPI
CteGetOptions (
  IN CHAR8 *OptionName,
  OUT UINT32 *Data
  );

/**
  Send lockstep command to MRC server.

  @param [in] LockStepCount      Lock step count.

  @retval  EFI_SUCCESS      LockStep succeeded.
  @retval  !EFI_SUCCESS     LockStep failed.

**/

EFI_STATUS
EFIAPI
CteLockstep (
  IN UINT32 LockStepCount
  );

/**
  Send check version command to MRC server.

  @param [in]  Project            Unknown
  @param [in]  ClientVersion      Unknown
  @param [out] ServerVersion      Unknown

  @retval  EFI_SUCCESS      Check succeeded.
  @retval  !EFI_SUCCESS     Check failed.

**/

EFI_STATUS
EFIAPI
CteCheckVersion (
  IN  CHAR8  *Project,
  IN  UINT32 ClientVersion,
  OUT UINT32 *ServerVersion
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

/**
  Send ReadRcdCfgSmb command to MRC server.

  @param [in]  ChannelNumber    Channel number
  @param [in]  DimmNumber       Dimm number
  @param [in]  RegisterAddress  Register Address
  @param [in]  RegisterSize     Register size
  @param [out] Data             Data to read

  @retval  EFI_SUCCESS    Send succeeded.
  @retval  !EFI_SUCCESS   Send failed.

**/

EFI_STATUS
EFIAPI
CteReadRcdCfgSmb (
  IN  UINT32 ChannelNumber,
  IN  UINT32 DimmNumber,
  IN  UINT32 RegisterAddress,
  IN  UINT32 RegisterSize,
  OUT UINT32 *Data
  );

/**
  Send ReadFnvCfgSmb command to MRC server.

  @param [in]  ChannelNumber    Channel number
  @param [in]  DimmNumber       Dimm number
  @param [in]  RegisterAddress  Register Address
  @param [in]  RegisterSize     Register size
  @param [out] Data             Data to read

  @retval  EFI_SUCCESS    Send succeeded.
  @retval  !EFI_SUCCESS   Send failed.

**/

EFI_STATUS
EFIAPI
CteReadFnvCfgSmb (
  IN  UINT32 ChannelNumber,
  IN  UINT32 DimmNumber,
  IN  UINT32 RegisterAddress,
  IN  UINT32 RegisterSize,
  OUT UINT32 *Data
  );

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
  );

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
  );

/**
  Send IsFnvFeedbackPresent command to MRC server.

  @retval  Command result from server

**/

UINT32
EFIAPI
CteIsFnvFeedbackPresent (
  VOID
  );

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
  );

/**
  Send delay to MRC server

  @param [in] i    Delay time

  @retval     None

**/

VOID
CteDelayQclk (
  IN INT32 i
  );

/**
  Send delay to MRC server

  @param[in] Delay     Delay time in QCLKS

**/
VOID
HvmDelayQclk (
  IN INT32 Delay
  );

/**
  Send delay to MRC server

  @param [in] i    Delay time

  @retval     None

**/

VOID
CteDelayDclk (
  IN INT32 i
  );

/**
  Send delay to MRC server

  @param [in] i    Delay time

  @retval     None

**/

VOID
CteDelayRef (
  IN INT32 i
  );

/**
  Connect to a MRC server.

  @param [in]  ConnectionName Name of the connection.
  @param [in]  Host           Host to connect to.
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
  );

/**
  Disconnect from a MRC server.

  @retval  EFI_SUCCESS      Disconnection succeeded.
  @retval  !EFI_SUCCESS     Disconnection failed.

**/

EFI_STATUS
EFIAPI
CteClientDisconnect (
  VOID
  );

/**
  Send get_time function to MRC server.

  @param [out] TimeMsb    Pointer to most significant 32 bits of the 64 bit time.
  @param [out] TimeLsb    Pointer to least significant 32 bits of the 64 bit time.

  @retval  EFI_SUCCESS    Send succeeded.
  @retval  !EFI_SUCCESS   Send failed or server returned error.

**/

EFI_STATUS
EFIAPI
CteGetTime (
  OUT UINT32 *TimeMsb,
  OUT UINT32 *TimeLsb
  );

/**
  Send exit function to MRC server.

  @param [in] ServerStatus    Status to send

  @retval N/A
**/

VOID
EFIAPI
CteExit (
  IN INT32 StatusToSend
  );

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
  );

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
  );

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
  IN CHAR8  *CmdType,
  IN UINT32 ChMask,
  IN UINT32 Ddr5SubChMask,
  IN UINT32 Rank,
  IN UINT32 Data0,
  IN UINT32 Data1
  );

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
  IN CHAR8  *Pattern,
  IN UINT32 PatternInfo,
  IN UINT32 ChMask,
  IN UINT32 Ddr5SubChMask,
  IN UINT32 Rank
  );

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
  );

/**
  Send debug command to MRC server.

  @retval  EFI_SUCCESS      Send succeeded.
  @retval  !EFI_SUCCESS     Send failed.

**/

EFI_STATUS
EFIAPI
CteStopPattern (
  VOID
  );

/**
  Send wait pattern done command to MRC server.

  @retval  EFI_SUCCESS      Send succeeded.
  @retval  !EFI_SUCCESS     Send failed.

**/

EFI_STATUS
EFIAPI
CteWaitPatternDone (
  VOID
  );


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
  IN CHAR8  *OptionName,
  IN UINT32 ChMask,
  IN UINT32 SubChMask,
  IN UINT32 Rank,
  IN UINT32 Data
  );

#endif // _CTE_NET_LIB_H_

