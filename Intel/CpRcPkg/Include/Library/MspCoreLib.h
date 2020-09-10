/** @file
  Interface header file for the MSP (Modular System Programming) library class.

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

#ifndef _MSP_CORE_LIB_H_
#define _MSP_CORE_LIB_H_
/**
  Decodes and executes the Capsule initialization.
  The routine starts at a beginning index in the CapsuleInit
  compressed data structure and continues until a ASSIGN_DONE
  flag is detected.

  @param[in, out]   MspData             Host structure for all data related to MSP.
  @param[in]        Socket              Socket under test
  @param[in]        CapsuleStartIndex   Starting point within theCapsuleData structure where the Init should begin.
  @param[in]        StringIndex         When provided, this is the index into the string table to show the register names.
  @param[in]        Channel             Current Channel being examined.
  @retval           MSP_SUCCESS
**/
MSP_STATUS
MspExecuteTask (
  IN      MSP_DATA          *MspData,
  IN      UINT16            CapsuleStartIndex,
  IN      UINT16            StringIndex,
  IN      UINT8             Channel
  );

/**

  Returns the MSP   Host structure from an MMX register.

  @param[in,out]    Destination Pointer
  @param[in]        Init Value
  @param[in]        Number of bytes to write

  @return           Destination Pointer

**/
VOID *
MspSet (
  VOID      *dst,
  UINT8     value,
  UINT32    cnt
  );

/**
  Single entry point to Msp. MRC calls this function
  and then Msp acts as a black box, performing the requested
  stage of DDRIO init and returns when complete or when
  an error is encountered.

  @param[in, out]  MspData         Host structure for all data related to Msp.
  @retval          Status
**/
MSP_STATUS
MspEntry (
  IN  OUT   MSP_DATA            *MspData
  );

/**
  Initialize dynamically all the pointer to the structures for the MSP Library

  @param[in]              MspData                 Host structure for all data related to Msp.

  @param[in]              MaxBlueprints           Max number of blueprints supported
  @param[in]              MaxChannels             Max number of channels supported
  @param[in]              MaxDynVar               Max number of dynamic variables supported
  @param[in]              MaxBoxes                Max number of box types supported
  @param[in]              MaxStrobes              Max number of strobes supported
  @param[in]              MaxRanks                Max number of ranks supported

  @param[in]              NumType                 Max number of technologies supported (DDR3, DDR4,...)
  @param[in]              NumConf                 Max number of configurations supported (A0, B0, C0,...)
  @param[in]              NumFreq                 Max number of frequencies supported (1600, 1866, 2133,...)
  @param[in]              NumPlat                 Max number of platforms supported (CRB, SV, SIMICS,...)

  @param[in]              MaxInstancePorts        Max number of instances supported
  @param[in]              MaxBoxNameLength        Max string length for box names
  @param[in]              MaxRegisterNameLength   Max string length for register names
  @param[in]              Use64bVariables

  @param[in]              **CapsuleStrings        Pointer to tasks names strings
  @param[in]              **OutputStrings         Pointer to output log strings
  @param[in]              *UnitStrings            Pointer to box name strings
  @param[in]              CapsuleStringsEn        Specifies if information about boxes names,
                                                  output logs and/or task names have to be printed
  @param[in]              DumpRegisterNamesEn     Specifies if information about register names have to be printed

  @param[in]              *InitTask               Pointer to array that contains the table that describes order of execution
  @param[in]              *InitData               Pointer to array that contains all tasks
  @param[in]              *SubtaskOffsetIndex     Pointer to array that contains all subtasks
  @param[in]              MaxSubTasks             Max number of subtasks

  @param[in]              *InstancePortMap        Pointer to array that contains the instance port mapping
  @param[in]              *StrobeToRegister       Pointer to array that contains Strobe to Register relationship
  @param[in]              *FloorPlan              Pointer to array that contains floorplan
  @param[in]              *Registers              Pointer to array for all defines registers
  @param[in]              *RankToRegister         Pointer to array that contains Rank to Register relationship
  @param[in]              *ChannelToInstanceMap   Pointer to array that contains Channel to Instance relationship
  @param[in]              *InstancePortOffset     Pointer to array that contains the instance port offset
  @param[in]              DynamicPtr              Pointer to function that will be used to execute dynamic assignments during the MSP execution

  @param[in]              *ChannelEnabled         Pointer to array that contains information about which channels are enabled
  @param[in]              *ExecuteOnThisChannel   Pointer to array that contains information about which channels are under test

  @param[in]              *CurrentPlatform        Pointer to array that contains information about current platform
  @param[in]              *CurrentFrequency       Pointer to array that contains information about current frequency
  @param[in]              *CurrentConfiguration   Pointer to array that contains information about current configuration
  @param[in]              *CurrentTechnology      Pointer to array that contains information about current technology

  @param[in]              *DynamicVars            Pointer to array that contains information about dynamic variables
  @param[in]              MaxDynamicVarsX         Max number of the dynamic variables array

**/
VOID
InitializeMspDataPointers (
  IN    MSP_DATA          *MspData,

  IN    UINT8             MaxBlueprints,
  IN    UINT8             MaxChannels,
  IN    UINT16            MaxDynVar,
  IN    UINT16            MaxBoxes,
  IN    UINT8             MaxStrobes,
  IN    UINT8             MaxRanks,

  IN    UINT8             NumType,
  IN    UINT8             NumConf,
  IN    UINT8             NumFreq,
  IN    UINT8             NumPlat,

  IN    UINT16            MaxInstancePorts,
  IN    UINT8             MaxBoxNameLength,
  IN    UINT8             MaxRegisterNameLength,
  IN    BOOLEAN           Use64bVariables,

  IN    UINT8             **CapsuleStrings,
  IN    UINT8             **OutputStrings,
  IN    UINT8             *UnitStrings,
  IN    BOOLEAN           CapsuleStringsEn,
  IN    BOOLEAN           DumpRegisterNamesEn,

  IN    TASK_DESCRIPTOR   *InitTask,
  IN    UINT8             *InitData,
  IN    SUBTASK_INDEX_OFFSET  *SubtaskOffsetIndex,
  IN    UINT16            MaxSubTasks,

  IN    INSTANCE_PORT_MAP *InstancePortMap,
  IN    INT16             *StrobeToRegister,
  IN    FLOORPLAN         *FloorPlan,
  IN    Register          *Registers,
  IN    INT16             *RankToRegister,
  IN    UINT8             *ChannelToInstanceMap,
  IN    INSTANCE_PORT_OFFSET  *InstancePortOffset,
  IN    DynamicFunctionPtr DynamicPtr,

  IN    BOOLEAN           *ChannelEnabled,
  IN    BOOLEAN           *ExecuteOnThisChannel,

  IN    UINT16            *CurrentPlatform,
  IN    UINT16            *CurrentFrequency,
  IN    UINT16            *CurrentConfiguration,
  IN    UINT16            *CurrentTechnology,

  IN    UINT32            *DynamicVars,
  IN    UINT16            MaxDynamicVarsX
  );

#endif // _MSP_CORE_LIB_H_
