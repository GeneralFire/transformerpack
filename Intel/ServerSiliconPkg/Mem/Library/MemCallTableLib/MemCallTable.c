/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2018 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>

extern CallTableEntryType CallTableMain[];
extern UINT8              CallTableMainSize;
extern CallTableEntryType CallTableDdrTraining[];
extern UINT8              CallTableDdrTrainingSize;

/**
Provide the caller with pointers to call tables and call table sizes

@param MainCallTable   - Pointer to pointer to the main call table
@param MainTableSize   - Pointer to main table size

retval    MRC_STATUS_SUCCESS
**/
MRC_STATUS
GetCallTablePtrs (
  CallTableEntryType    **MainCallTable,
  UINT8                 *MainTableSize
)
{
  *MainCallTable = CallTableMain;
  *MainTableSize = CallTableMainSize;

  return MRC_STATUS_SUCCESS;
} // GetCallTablePtrs

/**
Provide the caller with pointers to training call table and call table size

@param TrainingCallTable - Pointer to pointer to the training call table
@param TrainingTableSize - Pointer to training table size

retval    MRC_STATUS_SUCCESS
**/
MRC_STATUS
GetTrainingCallTablePtrs (
  CallTableEntryType    **TrainingCallTable,
  UINT8                 *TrainingTableSize
)
{
  *TrainingCallTable = CallTableDdrTraining;
  *TrainingTableSize = CallTableDdrTrainingSize;

  return MRC_STATUS_SUCCESS;
} // GetTrainingCallTablePtrs