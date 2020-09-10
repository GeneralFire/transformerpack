/** @file
  MspData.h
  Internal and external data structures, Macros, and enumerations.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2018 Intel Corporation. <BR>

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

#ifndef _MSPDATA_H_
#define _MSPDATA_H_

#include "AutoGenDefinitions.h"
#include "MspDataTypes.h"
#include "SysHost.h"
#include "MspForwardDefinitions.h"


//*****************************Common definitions****************************//
//
// Types of functions
//
#define EXTERNAL      0xFE
#define INTERNAL      0xFE
//
// Delay types
//
#define MILLI_DEL             0
#define MICRO_DEL             1
#define NANO_DEL              2
//
// Indication when all tasks are done
//
#define TASK_FUNCTION_DESC_DONE 0, 0, ((MSP_STATUS(*)(MSP_DATA * , UINT16, UINT16, UINT8)) 0), 0, 0, 0, 0, 0, 0, 0
//
// GSM Index definition
//
#define MRC_GT_INDEX            0xE000                                  // Starting index for MRC_GT enumerated type.
#define GSM_CSN_INDEX           0xF000                                  // Starting index for GSM_CSN enumerated type.
//
// The GetSet functions support a level parameter to select the appropriate DDRIO interface in the memory hierarchy
//
typedef enum {
  MspPreDdrLevel = 0,          // Near memory before DDR such as wide IO.
  MspDdrLevel,                 // DDR memory.
  MspLrbufLevel,               // Refers to data level at backside of LRDIMM or AEP buffer
  MspRegLevelA,                // Refers to cmd level at backside of register - side A
  MspRegLevelB,                // Refers to cmd level at backside of register - side B
  MspGsmLtMax,
  MspMrcLtDelim  = 0xFF
} MSP_MRC_LT;

typedef enum {
  MspRecEnDelay  = MRC_GT_INDEX,
  MspMrcGtMax,
  MspMrcGtDelim  = 0xFFFF
} MSP_MRC_GT;

typedef enum {
  MspSigRasN     = GSM_CSN_INDEX,
  MspGsmCsnMax,
  MspGsmCsnDelim = 0xFFFF
} MSP_GSM_CSN;

enum PLATFORM_TYPE_MSP_ENUM {
  IcxDdr = 0,
  IcxHbmio,
  IcxUsb,
  IcxOther,
  SkxDdr,
  MaxPlatformTypeMsp
};
//
// Channel definition
//
enum CHANNEL_OR_PORT_SELECTION_ENUM{
  ChNone  = 0xFE,
  ChAll   = 0xFF,
  ChAllP  = 0xE0,
};

//******Structures used for Auto-Generated and other MSP functions**********//
//
// Subtask information
//
struct SUBTASK_INDEX_OFFSET_STRUCT {
  UINT16  StrIndex;
  UINT32  FncOffset;
};
//
// Data structure to convert MRC_LT level to a blueprint.
//
struct PFCT_AND_IO_LEVEL_TO_BLUEPRINT_STRUCT {
  UINT16  PMask;
  UINT16  FMask;
  UINT16  CMask;
  UINT16  TMask;
  UINT8   IoLevel;
  UINT8   Blueprint;
};
//
// Description of each FUB/Boxtype
//
struct INSTANCE_PORT_MAP_STRUCT {
  UINT8   TotalInstances;
  UINT8   Instances;
  UINT16  InstanceStartIndex;
  UINT8   ReadOperation;
  UINT8   WriteOperation;
  UINT8   AccessMethod;
  UINT16  AccessBase;
  UINT8   SimulationSupported;
  UINT32  StringIndex;
  UINT8   ForceHbmLevel;
};
//
// Description of each instance per FUB/Boxtype
//
struct INSTANCE_PORT_OFFSET_STRUCT {
  UINT16  Port;
  UINT16  Offset;
} ;
//
//  The structure definition for the floorplan which is given a
//  channel and strobelane, to provide a physical channel and
//  strobelane.
//
struct FLOORPLAN_STRUCT {
  UINT8   Channel;
  UINT8   Strobelane;
};
//
// Structures used to display registers.
//
struct RegisterInformationStruct {
  CHAR8   *RegisterName;
  CHAR8   *RegisterDescription;
  UINT32 Offset;
  UINT8  NumFields;
  UINT32 Mask;
  UINT64 DefaultValue;
};

struct RegisterFieldStruct {
  UINT8  StartBit;
  UINT8  EndBit;
  UINT32 DefaultFieldValue;
  CHAR8  *AccessType;
  CHAR8  *FieldName;
  CHAR8  *Description;
};

struct RegisterStruct {
  RegisterInformation *RegInfo;
  RegisterField       *RegFields;
};

//*********************************MSP_DATA Structure***********************//

struct MSP_DATA_STRUCT {
  UINT8               BootMode;
  UINT8               DebugMsgLevel;
  UINT8               CurrentBlueprint;
  //
  // Platform Type dependent variables
  //
  UINT8               MaxBlueprints;
  UINT8               MaxChannels;
  UINT16              MaxDynVar;
  UINT16              MaxBoxes;
  UINT8               MaxStrobes;
  UINT8               MaxRanks;
  UINT8               NumType;
  UINT8               NumConf;
  UINT8               NumFreq;
  UINT8               NumPlat;
  UINT16              MaxInstancePorts;
  UINT8               MaxBoxNameLength;
  UINT8               MaxRegisterNameLength;
  BOOLEAN             Use64bVariables;
  UINT8               **CapsuleStrings;
  UINT8               **OutputStrings;
  UINT8               *UnitStrings;
  BOOLEAN             CapsuleStringsEn;
  BOOLEAN             DumpRegisterNamesEn;
  TASK_DESCRIPTOR     *InitTask;
  UINT8               *InitData;
  SUBTASK_INDEX_OFFSET  *SubtaskOffsetIndex;
  UINT16              MaxSubTasks;
  INSTANCE_PORT_MAP   *InstancePortMap;
  INT16               *StrobeToRegister;
  FLOORPLAN           *FloorPlan;
  Register            *Registers;
  INT16               *RankToRegister;
  UINT8               *ChannelToInstanceMap;
  INSTANCE_PORT_OFFSET  *InstancePortOffset;
  DynamicFunctionPtr  DynamicPtr;
  //
  // Platform Type common variables
  //
  BOOLEAN             *ChannelEnabled;
  BOOLEAN             *ExecuteOnThisChannel;
  UINT16              *CurrentPlatform;
  UINT16              *CurrentFrequency;
  UINT16              *CurrentConfiguration;
  UINT16              *CurrentTechnology;
  UINT32              *DynamicVars;
  UINT16              MaxDynamicVarsX;
  PLATFORM_TYPE_MSP   PlatformType;
};

//
// Descriptor for each Init Task
//
struct TASK_DESCRIPTOR_STRUCT {
  UINT8   PostCode;
  UINT8   BootMode;
  MSP_STATUS (*Function) (MSP_DATA *, UINT16, UINT16, UINT8);
  UINT16  CapsuleStartIndex;
  UINT16  StringIndex;
  UINT8   Channel;
  UINT16  CondExec;
  UINT16  PList;
  UINT16  FList;
  UINT16  CList;
  UINT16  TList;
};

#endif
