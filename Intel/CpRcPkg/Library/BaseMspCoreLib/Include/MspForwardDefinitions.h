/** @file
  MspForwardDefinitions.h
  Forward definitions for MSP structures.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2016 Intel Corporation. <BR>

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

#ifndef _MSP_FORWARD_DEFINITIONS_H_
#define _MSP_FORWARD_DEFINITIONS_H_


//*******************************Type Definitions****************************//

typedef enum PLATFORM_TYPE_MSP_ENUM                   PLATFORM_TYPE_MSP;
typedef struct SUBTASK_INDEX_OFFSET_STRUCT            SUBTASK_INDEX_OFFSET;
typedef struct PFCT_AND_IO_LEVEL_TO_BLUEPRINT_STRUCT  PFCT_AND_IO_LEVEL_TO_BLUEPRINT;
typedef struct INSTANCE_PORT_MAP_STRUCT               INSTANCE_PORT_MAP;
typedef struct INSTANCE_PORT_OFFSET_STRUCT            INSTANCE_PORT_OFFSET;
typedef struct FLOORPLAN_STRUCT                       FLOORPLAN;
typedef struct RegisterInformationStruct              RegisterInformation;
typedef struct RegisterFieldStruct                    RegisterField;
typedef struct RegisterStruct                         Register;
typedef struct TASK_DESCRIPTOR_STRUCT                 TASK_DESCRIPTOR;
typedef struct MSP_DATA_STRUCT                        MSP_DATA;
typedef UINT32 MSP_STATUS;
typedef MSP_STATUS(*DynamicFunctionPtr)(MSP_DATA *, UINT32 *, UINT8, UINT16, UINT32 *);

#endif
