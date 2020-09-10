/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2018 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#ifndef _SV_SMM_DRIVER_H
#define _SV_SMM_DRIVER_H


#define SVOS_SMI_MAX_PARAMETER 511
#define QUIESCE_TEST_DATA1 0xa5a5a5a5
#define QUIESCE_TEST_DATA2 0xa5

#define PCI_FUNCTION_NUMBER_PCH_LPC               0
#define PCI_DEVICE_NUMBER_PCH_LPC                 31
#define DEFAULT_PCI_BUS_NUMBER_PCH                0
#define R_PCH_LPC_GPIO_BASE                       0x48

typedef enum {
  SVQuiesceUnQuiesce = 1,   //1
  SVMemoryFailOver,         //2
  SVMemoryOnOffLine,        //3
  SVMemoryMigration,        //4
  SVIohOnOffline,           //5
  SVCpuOnOffline,           //6
  Reserved_1,               //7
  SVChipSparing_StartSpare, //8
  SVRankSparing_SetFail,    //9 - Not supported in Platform
  SVSet_LeakyBucket,        //10- Not supported in Platform
  SVSmiExtender,            //11- Not supported in Platform
  SVMemoryChipUnerase,      //12
  SVMemoryPatrolScrub,      //13- Not supported in Platform
  SVQuiesceCodeRun,         //14
} SV_SMM_COMMAND;

#pragma pack(1)
//
// Related data structures definition
//
typedef struct _SV_SMM_PARAMETER {
  UINT64  Command;
  UINT64  Parameter[SVOS_SMI_MAX_PARAMETER];
} SV_SMM_PARAMETER;
#pragma pack()

// stuff related ASM end
#endif
