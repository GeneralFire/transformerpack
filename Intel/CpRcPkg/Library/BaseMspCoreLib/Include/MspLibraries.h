/** @file
  MspLibraries.h
  Library specific macros and function declarations used within the MspSP.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2017 Intel Corporation. <BR>

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

#ifndef _MSPLIBRARIES_H_
#define _MSPLIBRARIES_H_

#include "MspData.h"
#include "MspForwardDefinitions.h"
#include <Library/SysHostPointerLib.h>

//**********************Definitions used for the MSP************************//
#define MSP_NA                0xFF
#define NO_PRINT              0xFFFF
#define SKIP_ACTION           0
#define TAKE_ACTION           1
#define PFCT_INDEPENDENT      0
//
// Support for Broadcasting during phyinit's when looping on modules.
//
#define BROADCAST_SUP         0

typedef enum  {
  Pfct =  0,
  PfctT,
  PfctC,
  PfctCT,
  PfctF,
  PfctFT,
  PfctFC,
  PfctFCT,
  PfctP,
  PfctPT,
  PfctPC,
  PfctPCT,
  PfctPF,
  PfctPFT,
  PfctPFC,
  PfctPFCT,
  MaxPfct,
} ENUM_PCFT_TYPE;

//*********************Function Defines used for the MSP*********************//
#define DYNAMIC_VARS(y,x)         MspData->DynamicVars[y*MspData->MaxDynamicVarsX + x]
#define DYNAMIC_VARS_B(y,x)       DynamicVars[y*MspData->MaxDynamicVarsX + x]
#define INSTANCE_PORT_MAP(x)      MspData->InstancePortMap[MspData->CurrentBlueprint*MspData->MaxBoxes + x]
#define STROBE_TO_REGISTER(y,x)   MspData->StrobeToRegister[MspData->CurrentBlueprint*(MspData->MaxStrobes*2)+ y*2 + x]
#define FLOOR_PLAN(y,x)           MspData->FloorPlan[MspData->CurrentBlueprint*(MspData->MaxChannels*MspData->MaxStrobes) + y*MspData->MaxStrobes + x]
#define UNIT_STRINGS(y)           &MspData->UnitStrings[MspData->CurrentBlueprint*(MspData->MaxBoxes*MspData->MaxBoxNameLength) + y*MspData->MaxBoxNameLength]
#define RANK_TO_REGISTER(y,x)     MspData->RankToRegister[MspData->CurrentBlueprint*(MspData->MaxRanks*2)+ y*2 + x]
#define CHANNEL_TO_INSTANCE(y,x)  MspData->ChannelToInstanceMap[MspData->CurrentBlueprint*(MspData->MaxBoxes*MspData->MaxChannels)+ y*MspData->MaxChannels + x]
#define INSTANCE_PORT_OFFSET(x)   MspData->InstancePortOffset[MspData->CurrentBlueprint*MspData->MaxInstancePorts + x]

//**********************Structures used for the MSP*************************//
typedef struct {
  UINT32  Pfct;
  UINT8   LengthVariation;
} PFCT_VARIATIONS;

//
//  Structure for the PHY init registers. This contains
//  information on how to find registers for a given bytelane,
//  module, rank, etc. Also indicates whether we should write a
//  register to all enabled channels or just to a specified
//  channel.
//
typedef struct {
  UINT8  RankLoop         :1;   ///> Loop on rank.
  UINT8  StrobeLoop       :1;   ///> Loop on strobe.
  UINT8  Offset16         :1;   ///> When set, the offset field is 2-bytes long, otherwise 1-byte long.
  UINT8  CondExec         :1;   ///> When set, Conditional Execution.
  UINT8  MaskPresent      :1;   ///> When set, mask is present in the Assignment.
  UINT8  NeedSecondByte   :1;   ///> When set, REGISTER_LOCATION_ATTRIBUTES2 is present.
  UINT8  UseBB2           :1;   ///> If the boxtype is above 15 or the value being written is greater than 32 bits
                                 ///> then we need to use the BB2 macro to allow 8 bits for boxtype and 8 for byte enables.
  UINT8  Done             :1;   ///> When set, this is the final entry for this task.
} REGISTER_LOCATION_ATTRIBUTES;
//
//  Optional Byte 2 of register attributes.
//  Only used if we need bits in this byte set to 1.
//
typedef struct {
  UINT8  StringPresent    :1;   ///> There is a string that needs to be output associated with this function entry.
  UINT8  InstanceDependent:1;   ///> When set, only specific instance will be written to.
  UINT8  ChannelDependent :1;   ///> When set, only specific channel will be written.
  UINT8  Reserved         :5;   ///> Reserved
} REGISTER_LOCATION_ATTRIBUTES2;

#endif
