/** @file
  SimulationServicesLib.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef _SIMULATION_SERVICES_LIB_H_
#define _SIMULATION_SERVICES_LIB_H_

#include <SysHost.h>
#include <Library/MemFmcIpLib.h>

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
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Dimm,
  IN UINT8 Rank,
  IN UINT8 Ch,
  IN UINT8 Bit,
  IN UINT8 MemPhase,
  IN UINT8 ErrorDevice,
  IN UINT32 *FeedbackValue
  );

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
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN UINT32 InputPayload[NUM_PAYLOAD_REG],
  IN UINT32 OutputPayload[NUM_PAYLOAD_REG],
  IN UINT32 Opcode,
  OUT UINT8 *MbStatus
  );

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
  IN PSYSHOST Host,
  IN UINT8 socket,
  IN UINT8 ch,
  IN UINT8 dimm,
  IN UINT8 rankPerDimm,
  IN UINT8 rank,
  IN UINT8 scope,
  IN MRC_GT group,
  IN MRC_LT level,
  IN UINT32 patternLength,
  IN INT16 previousMargin,
  OUT UINT32 bwSerr[MAX_CH][3]
  );

/**
  Cache the training values after basic training is done.

  @param[in] Host            - Pointer to sysHost

  @retval None

**/

UINT32
EFIAPI
RCSIMCacheDDRIO (
  IN PSYSHOST        Host
  );

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
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Slot,
  OUT UINT8   *Spdbin,
  OUT UINT32  *SpdbinLength
  );

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
  );

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
  IN VOID *IniFile,
  IN CHAR8 **NvramFileName
  );

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
  IN VOID *ValuePtr
  );

/**

  Skip setup parameter override.

  Disable the setup parameter from being updated by RcSimSetSetupParamOverride.
  If called before MemIniFileProcessing, prevent the parameter from being
  processed from the INI file in the first place.

  @param[in] ParamName  - parameter name string

  @retval SUCCESS/FAILURE

**/

UINT32
EFIAPI
RcSimSkipSetupParamOverride (
  IN CHAR8 *ParamName
  );

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
  );

/**

  Exit RcSim, returning exit code to the OS

  @param[in] ExitCode - Code to return

  @retval N/A

**/

VOID
ExitRcSim (
  IN INT32 ExitCode
  );

#endif // #ifndef _SIMULATION_SERVICES_LIB_H_
