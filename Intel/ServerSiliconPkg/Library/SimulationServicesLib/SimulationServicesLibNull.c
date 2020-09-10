/** @file
  SimulationServicesLib.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019-2020 Intel Corporation. <BR>

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

#include <SysHost.h>
#include <Library/MemFmcIpLib.h>
#include <Library/SimulationServicesLib.h>

//
// These data structures are defined in SimulationServicesLib
// We also need to define it in NULL instance to avoid link error
//
BOOLEAN ForceAdr = FALSE;
UINT8 WarmBootFlag = 0;
const INT16 RCSimSenseAmp [MAX_STROBE][4] = {0};

/**

  Inject errors for validation

  @param[in] Host           - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket         - Socket number
  @param[in] Dimm           - DIMM number (0-based)
  @param[in] Rank           - Rank number (0-based)
  @param[in] Ch             - Channel number (0-based)
  @param[in] Bit            - Bit number
  @param[in] MemPhase       - Memory training phase
  @param[in] ErrorDevice    - TBD
  @param[in] FeedbackValue  - Feedback value where the error will be injected

  @retval None

**/

VOID
EFIAPI
ErrorInject (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 dimm,
  UINT8 rank,
  UINT8 ch,
  UINT8 bit,
  UINT8 memPhase,
  UINT8 errorDevice,
  UINT32 *feedbackValue
  )
{
  return;
}

/**

  MMS_FnvMailboxInterface is a generic read/write to the BIOS/SMM mailbox

  @param[in] Host          - Pointer to the system Host (root) structure
  @param[in] Socket        - Socket Number
  @param[in] Ch            - DDR Channel ID
  @param[in] Dimm          - DIMM number
  @param[in] InputPayload  - input Payload Register
  @param[in] OutputPayload - Output Payload Register
  @param[in] Opcode        - FNV command (subopcode | opcode)
  @param[out] MbStatus     - FNV mb status

  @retval 0            - SUCCESS
  @retval 1            - FAILURE

**/

UINT32
EFIAPI
MMS_FnvMailboxInterface (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT32 inputPayload[NUM_PAYLOAD_REG],
  UINT32 outputPayload[NUM_PAYLOAD_REG],
  UINT32 opcode,
  UINT8  *mbStatus
  )
{
  return 0;
}

/**
  RCSIM feedback

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Ch              - Channel number
  @param[in] Dimm            - Dimm
  @param[in] RankPerDimm     - Rank per Dimm
  @param[in] Rank            - Dimm Rank
  @param[in] Scope           - Scope
  @param[in] Group           - Group
  @param[in] Level           - Level
  @param[in] PatternLength   - Pattern Length
  @param[in] PreviousMargin  - Previous Margin
  @param[out] BwSerr         - BitWise error status

  @retval None

**/

VOID
EFIAPI
RCSIMFeedbackResults (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rankPerDimm,
  UINT8 rank,
  UINT8 scope,
  MRC_GT group,
  MRC_LT level,
  UINT32 patternLength,
  INT16 previousMargin,
  UINT32 bwSerr[MAX_CH][3]
  )
{
  return;
}

/**
  Cache the training values after basic training is done.

  @param[in] Host            - Pointer to sysHost

  @retval None

**/

UINT32
EFIAPI
RCSIMCacheDDRIO (
  IN PSYSHOST        Host
  )
{
  return 0;
}

/**

  Check if DIMM is present in socket, ch, slot

  @param[in]  Socket         - Socket
  @param[in]  Ch             - Channel
  @param[in]  Slot           - DIMM Slot
  @param[out] Spdbin         - Pointer to the SPD Data
  @param[out] SpdbinLength   - Length of spdbin

  @retval SUCCESS/FAILURE

**/

UINT32
EFIAPI
MemSpdbinRead (
  IN UINT8    socket,
  IN UINT8    ch,
  IN UINT8    slot,
  OUT UINT8    *spdbin,
  OUT UINT32   *spdbinLength
  )
{
  return 0;
}

/**

  Write SPD data into global mMemSpdbin array

  @param[in]  Socket      - Processor socket ID
  @param[in]  Ch          - Channel of Dimm SPD to be read
  @param[in]  Slot        - Dimm number to be read
  @param[in]  SpdByte     - Spd Byte Offset
  @param[in]  Data        - pointer to data to write

  @retval SUCCESS/FAILURE

**/

UINT32
EFIAPI
MemSpdbinWrite (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Slot,
  IN UINT16   SpdByte,
  IN UINT8    *Data
  )
{
  return 0;
}

/**
  Process an INI file for use with RcSim or KtiSim.

  The IniFile parameter will get cast to type FILE. Using VOID
  in this header so that that every file that includes this file
  does not have to comprehend the FILE datatype.

  @param[in]  Host              A pointer to the Host data.
  @param[in]  IniFile           Ptr to the IniFile handle.
  @param[in]  nvramFileName     The Nvram file name.

  @retval     FAILURE           Failure
  @retval     SUCCESS           Success

**/

UINT32
EFIAPI
MemIniFileProcessing (
  IN PSYSHOST Host,
  IN VOID *iniFile,
  IN CHAR8 **nvramFileName
  )
{
  return 0;
}

//
// Definitions for use with RcSimSetSetupParamOverride
// ParamName parameter.
//

#define RCSIMCAPID0               "capid0"
#define RCSIMCAPID1               "capid1"
#define RCSIMCAPID2               "capid2"
#define RCSIMCAPID3               "capid3"
#define RCSIMCAPID4               "capid4"
#define RCSIMCAPID5               "capid5"
#define RCSIMCAPID6LO             "capid6lo"
#define RCSIMCAPID6HI             "capid6hi"
#define RCSIMMEMFLOWS             "memFlows"
#define RCSIMMEMFLOWSEXT          "memFlowsExt"
#define RCSIMMEMFLOWSEXT2         "memFlowsExt2"
#define RCSIMMEMFLOWSEXT3         "memFlowsExt3"
#define RCSIMCPUSTEPPING          "cpuStepping"
#define RCSIMCPUFAMILY            "cpuFamily"
#define RCSIMCPUREVISION          "cpuRevision"
#define RCSIMEXTCPUID7EAX         "extcpuid7eax"
#define RCSIMEXTCPUID7EBX         "extcpuid7ebx"
#define RCSIMEXTCPUID7ECX         "extcpuid7ecx"
#define RCSIMEXTCPUID7EDX         "extcpuid7edx"

/**

  RcSimSetSetupParamOverride

  Set override parameter if use override flag is enabled

  @param[in] ParamName  - parameter name string
  @param[in] ValuePtr   - pointer to target location to set

  @retval SUCCESS/FAILURE

**/

UINT32
EFIAPI
RcSimSetSetupParamOverride (
  IN CHAR8 *ParamName,
  IN VOID *valuePtr
  )
{
  return 0;
}

/**

  Initialize options configured by BIOS Setup

  @param[in] Host           - Pointer to sysHost
  @param[in] KtiHostInput   - Pointer to KtiHostInput
  @param[in] KtiHostOutput  - Pointer to KtiHostOutput

  @retval N/A

**/

VOID
EFIAPI
InitSetupOptions (
  IN PSYSHOST Host,
  IN VOID     *KtiHostInput,
  IN VOID     *KtiHostOutput
  )
{
  return;
}

/**

  Exit RcSim, returning exit code to the OS

  @param[in] ExitCode - Code to return

  @retval N/A

**/

VOID
ExitRcSim (
  IN INT32 ExitCode
  )
{
  return;
}
